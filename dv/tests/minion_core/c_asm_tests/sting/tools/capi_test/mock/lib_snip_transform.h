/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "cpu_resource.h"

inline unum_t snp_randnum(CpuResource* cpu) { return cpu->rand(); }
inline unum_t snp_randnum_glbl(CpuResource* cpu) { return cpu->rand_glbl(); }
inline unum_t snp_urandrange(CpuResource* cpu, unum_t min, unum_t max) {
    return snp_randnum(cpu) % (max - min) + min;
}
inline unum_t snp_urandrange_glbl(CpuResource* cpu, unum_t min, unum_t max) {
    return snp_randnum_glbl(cpu) % (max - min) + min;
}

inline unum_t snp_cpu_id(CpuResource* cpu) { return cpu->id; }

inline unum_t snp_and(CpuResource*, unum_t a, unum_t b) { return a & b; }
inline unum_t snp_or(CpuResource*, unum_t a, unum_t b) { return a | b; }
inline unum_t snp_lshift(CpuResource*, unum_t a, unum_t b) { return a < b; }
