/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <vector>

#include "types.h"

struct board_conf {
    unum_t NUM_CPUS = 0;
    unum_t NUM_LCPUID_TO_PCPUID_MAP = 0;
    std::vector<unum_t> LCPUID_TO_PCPUID_MAP;
};

extern thread_local board_conf BOARD_CONF;
