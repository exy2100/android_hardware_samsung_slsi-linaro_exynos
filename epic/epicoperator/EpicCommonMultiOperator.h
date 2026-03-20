/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "IEpicOperator.h"
#include "EpicBaseOperator.h"

namespace epic {
class EpicCommonMultiOperator : public EpicBaseOperator {
	public:
		EpicCommonMultiOperator();
		EpicCommonMultiOperator(int *scenario_id_list, int len);
		virtual ~EpicCommonMultiOperator() override;

		virtual bool doAction(int cmd, void *arg) override;

	private:
		bool doAcquireOption(void *arg);
	};
}
