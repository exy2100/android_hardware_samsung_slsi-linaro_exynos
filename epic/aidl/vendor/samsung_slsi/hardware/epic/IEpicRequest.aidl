//
// Copyright (C) 2026 The LineageOS Project
//
// SPDX-License-Identifier: Apache-2.0
//

package vendor.samsung_slsi.hardware.epic;
import vendor.samsung_slsi.hardware.epic.IEpicHandle;

@VintfStability
interface IEpicRequest {
    IEpicHandle init(int scenario_id);
    IEpicHandle init_multi(in int[] scenario_id_list);
    int update_handle_id(IEpicHandle req, String handle_id);
    int acquire_lock(IEpicHandle req);
    int release_lock(IEpicHandle req);
    int acquire_lock_option(IEpicHandle req, int value, int usec);
    int acquire_lock_multi_option(IEpicHandle req, in int[] value_list, in int[] usec_list);
    int acquire_lock_conditional(IEpicHandle req, String condition_name);
    int release_lock_conditional(IEpicHandle req, String condition_name);
    int perf_hint(IEpicHandle req, String name);
    int hint_release(IEpicHandle req, String name);
}
