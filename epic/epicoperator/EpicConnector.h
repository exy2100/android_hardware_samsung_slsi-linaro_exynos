/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/samsung_slsi/hardware/epic/IEpicRequest.h>
#include <aidl/vendor/samsung_slsi/hardware/epic/IEpicHandle.h>

#include <string>

using ::aidl::vendor::samsung_slsi::hardware::epic::IEpicRequest;
using ::aidl::vendor::samsung_slsi::hardware::epic::IEpicHandle;

namespace epic {

class EpicConnector {
public:
    EpicConnector();
    ~EpicConnector();

    void alloc_request(int scenario_id);
    void alloc_request(int *scenario_id_list, int len);
    void free_request();
    bool acquire();
    bool acquire(unsigned int value, unsigned int usec);
    bool acquire(unsigned int *value, unsigned int *usec, int len);
    bool acquire_conditional(std::string &condition_name);
    bool release();
    bool release_conditional(std::string &condition_name);

private:
    void getService();
    std::shared_ptr<IEpicRequest> mRequest;
    std::shared_ptr<IEpicHandle> mHandle;
};

}  // namespace epic
