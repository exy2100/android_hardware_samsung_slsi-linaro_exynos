/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VENDOR_SAMSUNG_SLSI_HARDWARE_EPIC_EPICREQUEST_H
#define VENDOR_SAMSUNG_SLSI_HARDWARE_EPIC_EPICREQUEST_H

#include <aidl/vendor/samsung_slsi/hardware/epic/BnEpicRequest.h>
#include <aidl/vendor/samsung_slsi/hardware/epic/IEpicHandle.h>

#include "EpicType.h"

namespace aidl {
namespace vendor {
namespace samsung_slsi {
namespace hardware {
namespace epic {

struct EpicRequest : public BnEpicRequest {
    EpicRequest();
    virtual ~EpicRequest();

    ndk::ScopedAStatus init(int32_t scenario_id,
                            std::shared_ptr<IEpicHandle>* _aidl_return) override;

    ndk::ScopedAStatus init_multi(const std::vector<int32_t>& scenario_id_list,
                                  std::shared_ptr<IEpicHandle>* _aidl_return) override;

    ndk::ScopedAStatus update_handle_id(const std::shared_ptr<IEpicHandle>& handle,
                                        const std::string& handle_id,
                                        int32_t* _aidl_return) override;

    ndk::ScopedAStatus acquire_lock(const std::shared_ptr<IEpicHandle>& handle,
                                    int32_t* _aidl_return) override;

    ndk::ScopedAStatus release_lock(const std::shared_ptr<IEpicHandle>& handle,
                                    int32_t* _aidl_return) override;

    ndk::ScopedAStatus acquire_lock_option(const std::shared_ptr<IEpicHandle>& handle,
                                           int32_t value,
                                           int32_t usec,
                                           int32_t* _aidl_return) override;

    ndk::ScopedAStatus acquire_lock_multi_option(const std::shared_ptr<IEpicHandle>& handle,
                                                 const std::vector<int32_t>& value_list,
                                                 const std::vector<int32_t>& usec_list,
                                                 int32_t* _aidl_return) override;

    ndk::ScopedAStatus acquire_lock_conditional(const std::shared_ptr<IEpicHandle>& handle,
                                                const std::string& condition_name,
                                                int32_t* _aidl_return) override;

    ndk::ScopedAStatus release_lock_conditional(const std::shared_ptr<IEpicHandle>& handle,
                                                const std::string& condition_name,
                                                int32_t* _aidl_return) override;

    ndk::ScopedAStatus perf_hint(const std::shared_ptr<IEpicHandle>& handle,
                                 const std::string& name,
                                 int32_t* _aidl_return) override;

    ndk::ScopedAStatus hint_release(const std::shared_ptr<IEpicHandle>& handle,
                                    const std::string& name,
                                    int32_t* _aidl_return) override;

    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override;

    void *so_handle;

    init_t               pfn_init;
    term_t               pfn_term;
    alloc_request_t      pfn_alloc_request;
    alloc_multi_request_t pfn_alloc_multi_request;
    update_handle_t      pfn_update_handle;
    free_request_t       pfn_free_request;
    acquire_t            pfn_acquire;
    acquire_option_t     pfn_acquire_option;
    acquire_multi_option_t pfn_acquire_multi_option;
    acquire_conditional_t pfn_acquire_conditional;
    release_conditional_t pfn_release_conditional;
    hint_t               pfn_hint;
    hint_t               pfn_hint_release;
    release_t            pfn_release;
    dump_t               pfn_dump;

    constexpr static const char *PATH_DIR_DUMP  = "/data/vendor/epic/";
    constexpr static const char *PATH_FILE_DUMP = "epic.dump";
    constexpr static const int   MAX_TRIES_DUMP = 1000;
};

}  // namespace epic
}  // namespace hardware
}  // namespace samsung_slsi
}  // namespace vendor
}  // namespace aidl

#endif  // VENDOR_SAMSUNG_SLSI_HARDWARE_EPIC_EPICREQUEST_H
