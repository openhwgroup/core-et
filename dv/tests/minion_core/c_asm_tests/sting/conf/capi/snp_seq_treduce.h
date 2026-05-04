/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TREDUCE_H_
#define _ET_SEQ_TREDUCE_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"

#define TREDUCE_MAX_REGS 8

#define et_min(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


uint32_t get_treduce_fregs_mask(CpuResource * cpu_resource, unum_t treduce_csr_val);
unum_t get_treduce_ow_unpr_fregs(CpuResource * cpu_resource, unum_t treduce_csr_val);
unum_t get_treduce_ten_waits(CpuResource * cpu_resource, unum_t treduce_csr_val);
unum_t get_mem_size_treduce(CpuResource * cpu_resource, unum_t treduce_csr_val, unum_t x31_val);
unum_t snp_treduce_is_valid_op(CpuResource * cpu_resource, unum_t treduce_csr_val);
unum_t snp_get_vpurf_op_type(CpuResource * cpu_resource, unum_t treduce_csr_val);
void selfcheck_boundaries_treduce(CpuResource * cpu_resource, unum_t treduce_csr_val, unum_t head, unum_t tail);
void selfcheck_set_treduce(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t treduce_csr_val, unum_t selfcheck, unum_t set);
unum_t constrain_treduce_vpu_rf(CpuResource * cpu_resource, unum_t treduce_csr_val, unum_t rf_head, unum_t rf_tail);
unum_t glbl_snp_seq_treduce_base(CpuResource * cpu_resource, unum_t *vpu_rf);

#endif
