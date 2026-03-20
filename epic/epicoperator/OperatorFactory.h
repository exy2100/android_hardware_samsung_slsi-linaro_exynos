/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "EpicEnum.h"
#include "IEpicOperator.h"

#ifdef __cplusplus
extern "C" {
#endif

void * createOperator(int operator_id);
void * createScenarioOperator(int operator_id, int scenario_id);
void * createScenarioMultiOperator(int operator_id, int scenario_id_list[], int len);

#ifdef __cplusplus
}
#endif
