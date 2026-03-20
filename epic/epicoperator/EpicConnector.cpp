/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "EpicConnector.h"
#include <vector>
#include <android-base/logging.h>
#include <android/binder_manager.h>

namespace epic {

EpicConnector::EpicConnector()
{
}

EpicConnector::~EpicConnector()
{
}

void EpicConnector::alloc_request(int scenario_id)
{
    getService();
    if (mRequest == nullptr)
        return;

    std::shared_ptr<IEpicHandle> handle;
    auto status = mRequest->init(scenario_id, &handle);
    if (status.isOk())
        mHandle = handle;
}

void EpicConnector::alloc_request(int *scenario_id_list, int len)
{
    getService();
    if (mRequest == nullptr)
        return;

    std::vector<int32_t> id_vec(scenario_id_list, scenario_id_list + len);
    std::shared_ptr<IEpicHandle> handle;
    auto status = mRequest->init_multi(id_vec, &handle);
    if (status.isOk())
        mHandle = handle;
}

bool EpicConnector::acquire()
{
    if (mRequest == nullptr || mHandle == nullptr)
        return false;

    int32_t ret = 0;
    auto status = mRequest->acquire_lock(mHandle, &ret);
    return status.isOk() && ret;
}

bool EpicConnector::acquire(unsigned int value, unsigned int usec)
{
    if (mRequest == nullptr || mHandle == nullptr)
        return false;

    int32_t ret = 0;
    auto status = mRequest->acquire_lock_option(mHandle, (int32_t)value, (int32_t)usec, &ret);
    return status.isOk() && ret;
}

bool EpicConnector::acquire(unsigned int *value, unsigned int *usec, int len)
{
    if (mRequest == nullptr || mHandle == nullptr)
        return false;

    std::vector<int32_t> value_vec(value, value + len);
    std::vector<int32_t> usec_vec(usec, usec + len);
    int32_t ret = 0;
    auto status = mRequest->acquire_lock_multi_option(mHandle, value_vec, usec_vec, &ret);
    return status.isOk() && ret;
}

bool EpicConnector::acquire_conditional(std::string &condition_name)
{
    if (mRequest == nullptr || mHandle == nullptr)
        return false;

    int32_t ret = 0;
    auto status = mRequest->acquire_lock_conditional(mHandle, condition_name, &ret);
    return status.isOk() && ret;
}

bool EpicConnector::release()
{
    if (mRequest == nullptr || mHandle == nullptr)
        return false;

    int32_t ret = 0;
    auto status = mRequest->release_lock(mHandle, &ret);
    return status.isOk() && ret;
}

bool EpicConnector::release_conditional(std::string &condition_name)
{
    if (mRequest == nullptr || mHandle == nullptr)
        return false;

    int32_t ret = 0;
    auto status = mRequest->release_lock_conditional(mHandle, condition_name, &ret);
    return status.isOk() && ret;
}

void EpicConnector::getService()
{
    if (mRequest != nullptr)
        return;

    const std::string instance = std::string(IEpicRequest::descriptor) + "/default";
    mRequest = IEpicRequest::fromBinder(ndk::SpAIBinder(AServiceManager_getService(instance.c_str())));
    if (mRequest == nullptr)
        LOG(ERROR) << "Couldn't get service EPIC AIDL!";
}

}  // namespace epic
