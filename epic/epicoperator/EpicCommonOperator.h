/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "IEpicOperator.h"
#include "EpicBaseOperator.h"

namespace epic {
	class EpicCommonOperator : public EpicBaseOperator {
	public:
		EpicCommonOperator();
		EpicCommonOperator(int scenario_id);
		virtual ~EpicCommonOperator() override;

		virtual bool doAction(int cmd, void *arg) override;

	private:
		bool doAcquireOption(void *arg);
		bool doConditional(bool (EpicConnector::*)(std::string &), void *arg);
	};
}
