/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_COMMON_H
#define _ET_COMMON_H

#include "cpu_resource.h"

#define IS_GLBL_RAND 1
#define NOT_GLBL_RAND 0

unum_t log2n(CpuResource * cpu_resource, unum_t n);
unum_t snp_get_bits(CpuResource * cpu_resource, unum_t value, unum_t head, unum_t tail);
unum_t snp_set_bits(CpuResource * cpu_resource, unum_t value, unum_t head, unum_t tail, unum_t set_value);
unum_t snp_in_range(CpuResource * cpu_resource, unum_t min, unum_t max, unum_t max_inclusive, unum_t value);
unum_t countones(CpuResource * cpu_resource, unum_t mask_size, unum_t value);
unum_t snp_if_zero_else(CpuResource * cpu_resource, unum_t bias, unum_t if_val, unum_t else_val);
unum_t et_urandrange(CpuResource * cpu_resource, const unum_t is_glbl, const unum_t head, const unum_t tail);
unum_t et_randnum(CpuResource * cpu_resource, const unum_t is_glbl);

#endif
