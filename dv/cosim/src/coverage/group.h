/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "cosim_insn.h"

namespace coverage {

enum class group {
#define GROUPDEF(G) G,
#include "def/group.h"
#undef GROUPDEF
};

constexpr auto num_groups = 21;

const char* group_name(group g);

group get_group(uint32_t inst_bits, uint64_t store_addr);
group get_group(const cosim_insn& insn);

} // namespace coverage
