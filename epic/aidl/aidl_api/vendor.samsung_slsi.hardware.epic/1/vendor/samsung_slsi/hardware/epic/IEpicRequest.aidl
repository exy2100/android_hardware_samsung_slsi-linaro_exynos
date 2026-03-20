//
// Copyright (C) 2026 The LineageOS Project
//
// SPDX-License-Identifier: Apache-2.0
//
///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a snapshot of an AIDL file. Do not edit it manually. There are
// two cases:
// 1). this is a frozen version file - do not edit this in any case.
// 2). this is a 'current' file. If you make a backwards compatible change to
//     the interface (from the latest frozen version), the build system will
//     prompt you to update this file with `m <name>-update-api`.
//
// You must not make a backward incompatible change to any AIDL file built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

package vendor.samsung_slsi.hardware.epic;
@VintfStability
interface IEpicRequest {
  vendor.samsung_slsi.hardware.epic.IEpicHandle init(int scenario_id);
  vendor.samsung_slsi.hardware.epic.IEpicHandle init_multi(in int[] scenario_id_list);
  int update_handle_id(vendor.samsung_slsi.hardware.epic.IEpicHandle req, String handle_id);
  int acquire_lock(vendor.samsung_slsi.hardware.epic.IEpicHandle req);
  int release_lock(vendor.samsung_slsi.hardware.epic.IEpicHandle req);
  int acquire_lock_option(vendor.samsung_slsi.hardware.epic.IEpicHandle req, int value, int usec);
  int acquire_lock_multi_option(vendor.samsung_slsi.hardware.epic.IEpicHandle req, in int[] value_list, in int[] usec_list);
  int acquire_lock_conditional(vendor.samsung_slsi.hardware.epic.IEpicHandle req, String condition_name);
  int release_lock_conditional(vendor.samsung_slsi.hardware.epic.IEpicHandle req, String condition_name);
  int perf_hint(vendor.samsung_slsi.hardware.epic.IEpicHandle req, String name);
  int hint_release(vendor.samsung_slsi.hardware.epic.IEpicHandle req, String name);
}
