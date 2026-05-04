/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TQUANT_H_
#define _ET_SEQ_TQUANT_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_tensor_quant.h"

#define et_min(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

uint32_t get_tquant_fregs_mask(CpuResource * cpu_resource, unum_t tquant_csr_val);
unum_t get_tquant_ow_unpr_fregs(CpuResource * cpu_resource, unum_t tquant_csr_val);
unum_t get_tquant_ten_waits(CpuResource * cpu_resource, unum_t tquant_csr_val);
unum_t tquant_is_a_nop(CpuResource * cpu_resource, unum_t tquant_csr_val);
unum_t get_mem_size_tquant(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t x31_val);
void selfcheck_scp_boundaries_tquant(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t scp_head, unum_t scp_tail);
void selfcheck_vpurf_boundaries_tquant(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t rf_head, unum_t rf_tail);
void selfcheck_set_tquant(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tquant_csr_val, unum_t selfcheck, unum_t set);
void set_tquant_l1scp_vpurf(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tquant_csr_val);
void selfcheck_tquant(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tquant_csr_val);
unum_t constrain_tquant_write_vpu_rf(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t rf_head, unum_t rf_tail);
unum_t get_tquant_trans_only_vpurf(CpuResource * cpu_resource);
unum_t constrain_tquant_read_l1scp(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t scp_head, unum_t scp_tail);
unum_t snp_seq_tquant_base(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t *ten_seq, unum_t idx, unum_t num_ops);

#endif
