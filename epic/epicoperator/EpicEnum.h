/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

enum eOperator {
	eCommon,
        eVideoEncoding,
        eVideoDecoding,
};

enum eCommand {
	eNone,
	eAcquire,
	eRelease,
	eAcquireOption,
	eAcquireConditional,
	eReleaseConditional,
};
