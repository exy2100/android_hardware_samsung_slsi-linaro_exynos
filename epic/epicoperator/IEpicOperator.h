/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace epic {
	class IEpicOperator {
	public:
		IEpicOperator() = default;
		virtual ~IEpicOperator() {};

		virtual bool doAction(int cmd, void *arg) = 0;
	};
}
