/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "EpicHandle.h"

namespace aidl {
namespace vendor {
namespace samsung_slsi {
namespace hardware {
namespace epic {

EpicHandle::EpicHandle() :
    mReqHandle(0),
    pfn_free_request(nullptr)
{
}

EpicHandle::~EpicHandle()
{
    if (mReqHandle == 0)
        return;

    if (pfn_free_request != nullptr)
    {
        pfn_free_request(mReqHandle);
    }
}

ndk::ScopedAStatus EpicHandle::init(int64_t request_handle)
{
    mReqHandle = request_handle;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicHandle::get_handle(int64_t* _aidl_return)
{
    *_aidl_return = mReqHandle;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicHandle::diagonostic()
{
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus EpicHandle::set_pfn_finalize(free_request_t pfn)
{
    pfn_free_request = pfn;
    return ndk::ScopedAStatus::ok();
}

}  // namespace epic
}  // namespace hardware
}  // namespace samsung_slsi
}  // namespace vendor
}  // namespace aidl
