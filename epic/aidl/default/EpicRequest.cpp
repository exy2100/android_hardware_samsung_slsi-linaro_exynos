/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "EpicRequest.h"
#include "EpicHandle.h"

#include <chrono>
#include <thread>
#include <sstream>

#include <dlfcn.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <android-base/logging.h>

namespace aidl {
namespace vendor {
namespace samsung_slsi {
namespace hardware {
namespace epic {

EpicRequest::EpicRequest() :
    so_handle(nullptr)
{
    if (sizeof(long) == sizeof(int))
        so_handle = dlopen("/vendor/lib/libepic_helper.so", RTLD_NOW);
    else
        so_handle = dlopen("/vendor/lib64/libepic_helper.so", RTLD_NOW);

    if (so_handle == nullptr) {
        pfn_init                 = nullptr;
        pfn_term                 = nullptr;
        pfn_alloc_request        = nullptr;
        pfn_alloc_multi_request  = nullptr;
        pfn_update_handle        = nullptr;
        pfn_free_request         = nullptr;
        pfn_acquire              = nullptr;
        pfn_acquire_option       = nullptr;
        pfn_acquire_multi_option = nullptr;
        pfn_release              = nullptr;
        pfn_acquire_conditional  = nullptr;
        pfn_release_conditional  = nullptr;
        pfn_hint                 = nullptr;
        pfn_hint_release         = nullptr;
        pfn_dump                 = nullptr;
        return;
    }

    pfn_init                 = (init_t)dlsym(so_handle, "epic_init");
    pfn_term                 = (term_t)dlsym(so_handle, "epic_term");
    pfn_alloc_request        = (alloc_request_t)dlsym(so_handle, "epic_alloc_request_internal");
    pfn_alloc_multi_request  = (alloc_multi_request_t)dlsym(so_handle, "epic_alloc_multi_request_internal");
    pfn_update_handle        = (update_handle_t)dlsym(so_handle, "epic_update_handle_id_internal");
    pfn_free_request         = (free_request_t)dlsym(so_handle, "epic_free_request_internal");
    pfn_acquire              = (acquire_t)dlsym(so_handle, "epic_acquire_internal");
    pfn_acquire_option       = (acquire_option_t)dlsym(so_handle, "epic_acquire_option_internal");
    pfn_acquire_multi_option = (acquire_multi_option_t)dlsym(so_handle, "epic_acquire_multi_option_internal");
    pfn_acquire_conditional  = (acquire_conditional_t)dlsym(so_handle, "epic_acquire_conditional_internal");
    pfn_release_conditional  = (release_conditional_t)dlsym(so_handle, "epic_release_conditional_internal");
    pfn_hint                 = (hint_t)dlsym(so_handle, "epic_perf_hint_internal");
    pfn_hint_release         = (hint_t)dlsym(so_handle, "epic_hint_release_internal");
    pfn_release              = (release_t)dlsym(so_handle, "epic_release_internal");
    pfn_dump                 = (dump_t)dlsym(so_handle, "epic_request_dumpstate_internal");

    if (pfn_init != nullptr)
        pfn_init();
}

EpicRequest::~EpicRequest()
{
    if (pfn_term != nullptr)
        pfn_term();

    if (so_handle != nullptr)
        dlclose(so_handle);
}

ndk::ScopedAStatus EpicRequest::init(int32_t scenario_id,
                                     std::shared_ptr<IEpicHandle>* _aidl_return)
{
    if (pfn_alloc_request == nullptr)
    {
        *_aidl_return = nullptr;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    handleType req_handle = pfn_alloc_request(scenario_id);

    std::shared_ptr<EpicHandle> ret_instance = ndk::SharedRefBase::make<EpicHandle>();
    ret_instance->set_pfn_finalize(pfn_free_request);
    ret_instance->init(req_handle);

    *_aidl_return = ret_instance;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::init_multi(const std::vector<int32_t>& scenario_id_list,
                                           std::shared_ptr<IEpicHandle>* _aidl_return)
{
    if (pfn_alloc_multi_request == nullptr)
    {
        *_aidl_return = nullptr;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    handleType req_handle = pfn_alloc_multi_request(scenario_id_list.data(),
                                                    scenario_id_list.size());

    std::shared_ptr<EpicHandle> ret_instance = ndk::SharedRefBase::make<EpicHandle>();
    ret_instance->set_pfn_finalize(pfn_free_request);
    ret_instance->init(req_handle);

    *_aidl_return = ret_instance;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::update_handle_id(const std::shared_ptr<IEpicHandle>& handle,
                                                const std::string& handle_id,
                                                int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_update_handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    pfn_update_handle(req_handle, handle_id.c_str());
    *_aidl_return = 1;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::acquire_lock(const std::shared_ptr<IEpicHandle>& handle,
                                            int32_t* _aidl_return)
{
    if (handle == nullptr) {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_acquire == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = (int32_t)pfn_acquire(req_handle);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::release_lock(const std::shared_ptr<IEpicHandle>& handle,
                                            int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_release == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = (int32_t)pfn_release(req_handle);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::acquire_lock_option(const std::shared_ptr<IEpicHandle>& handle,
                                                    int32_t value, int32_t usec,
                                                    int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_acquire_option == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = (int32_t)pfn_acquire_option(req_handle, (unsigned int)value, (unsigned int)usec);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::acquire_lock_multi_option(const std::shared_ptr<IEpicHandle>& handle,
                                                        const std::vector<int32_t>& value_list,
                                                        const std::vector<int32_t>& usec_list,
                                                        int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    if (value_list.size() != usec_list.size())
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_acquire_multi_option == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::vector<uint32_t> uvalue(value_list.begin(), value_list.end());
    std::vector<uint32_t> uusec(usec_list.begin(), usec_list.end());

    *_aidl_return = (int32_t)pfn_acquire_multi_option(req_handle,
                                                    uvalue.data(),
                                                    uusec.data(),
                                                    uvalue.size());
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::acquire_lock_conditional(const std::shared_ptr<IEpicHandle>& handle,
                                                        const std::string& condition_name,
                                                        int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_acquire_conditional == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    *_aidl_return = (int32_t)pfn_acquire_conditional(req_handle,
                                                    condition_name.c_str(),
                                                    condition_name.size());
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::release_lock_conditional(const std::shared_ptr<IEpicHandle>& handle,
                                                        const std::string& condition_name,
                                                        int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_release_conditional == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = (int32_t)pfn_release_conditional(req_handle,
                                                    condition_name.c_str(),
                                                    condition_name.size());
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::perf_hint(const std::shared_ptr<IEpicHandle>& handle,
                                            const std::string& name,
                                            int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_hint == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = (int32_t)pfn_hint(req_handle, name.c_str(), name.size());
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicRequest::hint_release(const std::shared_ptr<IEpicHandle>& handle,
                                                const std::string& name,
                                                int32_t* _aidl_return)
{
    if (handle == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    int64_t req_handle_val = 0;
    auto status = handle->get_handle(&req_handle_val);
    if (!status.isOk())
    {
        *_aidl_return = 0;
        return status;
    }

    handleType req_handle = (handleType)req_handle_val;
    if (req_handle == 0)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (pfn_hint_release == nullptr)
    {
        *_aidl_return = 0;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = (int32_t)pfn_hint_release(req_handle, name.c_str(), name.size());
    return ndk::ScopedAStatus::ok();
}

binder_status_t EpicRequest::dump(int fd, const char**, uint32_t)
{
    if (pfn_dump == nullptr || pfn_alloc_request == nullptr)
        return STATUS_OK;

    std::string path_dump =
            std::string(PATH_DIR_DUMP) + PATH_FILE_DUMP;

    handleType req_handle = pfn_alloc_request(0);
    if (req_handle == 0)
        return STATUS_OK;

    auto handle_guard = std::unique_ptr<void, std::function<void(void*)>>(
        reinterpret_cast<void*>(req_handle),
        [&](void*) {
            if (pfn_free_request != nullptr)
                pfn_free_request(req_handle);
        }
    );
    pfn_dump(req_handle, path_dump.c_str(), path_dump.length());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    int requestDumpFd = -1;
    for (int i = 0; i < MAX_TRIES_DUMP; ++i)
    {
        requestDumpFd = open(path_dump.c_str(), O_RDONLY);
        if (requestDumpFd >= 0)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    if (requestDumpFd < 0)
        return STATUS_OK;

    bool lockHeld = false;
    for (int i = 0; i < MAX_TRIES_DUMP; ++i)
    {
        if (flock(requestDumpFd, LOCK_EX) == 0)
        {
            lockHeld = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    if (!lockHeld)
    {
        close(requestDumpFd);
        return STATUS_OK;
    }

    off_t file_size = lseek(requestDumpFd, 0, SEEK_END);
    if (file_size <= 0)
    {
        flock(requestDumpFd, LOCK_UN);
        close(requestDumpFd);
        return STATUS_OK;
    }

    lseek(requestDumpFd, 0, SEEK_SET);
    if (sendfile(fd, requestDumpFd, 0, file_size) < 0)
    {
        LOG(ERROR) << "Failed to send dump file";
    }

    if (flock(requestDumpFd, LOCK_UN) == -1)
    {
        LOG(ERROR) << "Failed to unlock file";
    }

    close(requestDumpFd);
    unlink(path_dump.c_str());

    return STATUS_OK;
}

}  // namespace epic
}  // namespace hardware
}  // namespace samsung_slsi
}  // namespace vendor
}  // namespace aidl
