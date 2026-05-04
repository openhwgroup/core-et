/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TSTORE_H_
#define _ET_SEQ_TSTORE_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_cts.h"

#define et_min(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

uint32_t get_tstore_fregs_mask(CpuResource * cpu_resource, unum_t tstore_csr_val);
unum_t get_tstore_ow_unpr_fregs(CpuResource * cpu_resource, unum_t tstore_csr_val);
unum_t get_tstore_ten_waits(CpuResource * cpu_resource, unum_t tstore_csr_val);
unum_t get_mem_size_tstore(CpuResource * cpu_resource, unum_t tstore_csr_val, unum_t x31_val);
unum_t selfcheck_set_tstore(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t tstore_csr_val, unum_t selfcheck, unum_t set);
unum_t selfcheck_boundaries_tstore(CpuResource * cpu_resource, unum_t tstore_csr_val, unum_t head, unum_t tail);
unum_t snp_tstore_is_valid_config(CpuResource * cpu_resource, unum_t csr_value);
unum_t constrain_tstore_vpu_read_rf(CpuResource * cpu_resource, unum_t tstore_csr_val, unum_t rf_head, unum_t rf_tail);
unum_t snp_seq_tstore_base(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t *ten_seq, unum_t idx, unum_t num_ops);

#endif
