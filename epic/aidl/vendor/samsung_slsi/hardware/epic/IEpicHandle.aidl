//
// Copyright (C) 2026 The LineageOS Project
//
// SPDX-License-Identifier: Apache-2.0
//

package vendor.samsung_slsi.hardware.epic;

@VintfStability
interface IEpicHandle {
    void init(long request_handle);
    long get_handle();
    void diagonostic();
}
