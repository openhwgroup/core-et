/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "types.h"
#include "board_conf.h"
#include "cpu_resource.h"

namespace mock {
std::vector<CpuResource> setup(unum_t num_neighs, unum_t num_threads); // Modifies BOARD_CONF
};
