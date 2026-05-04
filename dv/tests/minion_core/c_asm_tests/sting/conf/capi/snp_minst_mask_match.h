/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _ET_MASK_MATCH
#define _ET_MASK_MATCH

#include "cpu_resource.h"

/**
 * Creates a mask with a number of bits set to one which is between min_one_bits (included) 
 * and max_one_bits (excluded)
*/
unum_t snp_get_minst_mask(CpuResource * cpu_resource, unum_t min_one_bits, unum_t max_one_bits);

#endif