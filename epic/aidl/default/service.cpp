/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "EpicRequest.h"

using aidl::vendor::samsung_slsi::hardware::epic::EpicRequest;

int main()
{
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<EpicRequest> service = ndk::SharedRefBase::make<EpicRequest>();

    const std::string instance = std::string() + EpicRequest::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(service->asBinder().get(), instance.c_str());
    if (status != STATUS_OK)
    {
        LOG(ERROR) << "Failed to register Epic service: " << status;
        return EXIT_FAILURE;
    }
    LOG(INFO) << "Epic Service started!";

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE; // should not reach
}
