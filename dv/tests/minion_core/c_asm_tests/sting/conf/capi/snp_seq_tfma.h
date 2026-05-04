/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TFMA_H_
#define _ET_SEQ_TFMA_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_tfma.h"

#define et_min(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

uint32_t get_tfma_fregs_mask(CpuResource * cpu_resource, unum_t tfma_csr_val);
unum_t get_tfma_ow_unpr_fregs(CpuResource * cpu_resource, unum_t tfma_csr_val);
unum_t get_tfma_ten_waits(CpuResource * cpu_resource, unum_t tfma_csr_val);
unum_t get_mem_size_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t x31_val);
void selfcheck_a_scp_boundaries_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail);
void selfcheck_b_scp_boundaries_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail);
void selfcheck_vpurf_boundaries_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t rf_head, unum_t rf_tail);
void selfcheck_set_tfma(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tfma_csr_val, unum_t selfcheck, unum_t set);
void set_tfma_l1scp_vpurf(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tfma_csr_val);
void selfcheck_tfma(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tfma_csr_val);
unum_t constrain_tfma_a_read_lines(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail);
unum_t constrain_tfma_b_read_lines(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail);
unum_t constrain_tfma_vpu_write_rf(CpuResource * cpu_resource, unum_t tfma_csr_value, unum_t rf_head, unum_t rf_tail);
unum_t snp_seq_tfma_base(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t *ten_seq, unum_t idx, unum_t num_ops, unum_t tenb);
unum_t snp_seq_tfma_wrapper(CpuResource * cpu_resource, unum_t tl_csr_val);

#endif
