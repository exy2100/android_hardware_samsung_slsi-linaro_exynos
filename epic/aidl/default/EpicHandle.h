/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VENDOR_SAMSUNG_SLSI_HARDWARE_EPIC_EPICHANDLE_H
#define VENDOR_SAMSUNG_SLSI_HARDWARE_EPIC_EPICHANDLE_H

#include <aidl/vendor/samsung_slsi/hardware/epic/BnEpicHandle.h>

#include "EpicType.h"

namespace aidl {
namespace vendor {
namespace samsung_slsi {
namespace hardware {
namespace epic {

class EpicHandle : public BnEpicHandle {
public:
    EpicHandle();
    virtual ~EpicHandle();
    ndk::ScopedAStatus init(int64_t request_handle) override;
    ndk::ScopedAStatus get_handle(int64_t* _aidl_return) override;
    ndk::ScopedAStatus diagonostic() override;
    ndk::ScopedAStatus set_pfn_finalize(free_request_t pfn);
    int64_t mReqHandle;
    free_request_t pfn_free_request;
};

}  // namespace epic
}  // namespace hardware
}  // namespace samsung_slsi
}  // namespace vendor
}  // namespace aidl

#endif  // VENDOR_SAMSUNG_SLSI_HARDWARE_EPIC_EPICHANDLE_H
