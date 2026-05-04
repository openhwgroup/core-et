/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_CTL_COV_H
#define _ET_CTL_COV_H

#include "cpu_resource.h"

unum_t glbl_ctl_urandrange(CpuResource * cpu_resource, unum_t is_coop, unum_t min, unum_t max);
unum_t glbl_snp_get_mask_range(CpuResource * cpu_resource, unum_t is_coop, unum_t mask_size, unum_t min, unum_t max);
unum_t glbl_cov_tensor_load_lines(CpuResource * cpu_resource, unum_t csr_value, unum_t is_coop);
unum_t glbl_cov_tensor_load_mask(CpuResource * cpu_resource, unum_t is_coop);

#endif
