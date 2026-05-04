/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TLOAD_L2_H_
#define _ET_SEQ_TLOAD_L2_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"

unum_t get_tload_l2_ten_waits(CpuResource * cpu_resource, unum_t tl_l2_csr_val);
unum_t get_mem_size_tload_l2(CpuResource * cpu_resource, unum_t tl_l2_csr_val, unum_t x31_val);
void selfcheck_boundaries_tload_l2(CpuResource * cpu_resource, unum_t tl_l2_csr_val);
unum_t snp_seq_tload_l2_base(CpuResource * cpu_resource);

#endif
