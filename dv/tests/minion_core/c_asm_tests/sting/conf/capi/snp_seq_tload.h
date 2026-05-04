/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TLOAD_H_
#define _ET_SEQ_TLOAD_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_ctl_c_api.h"

#define et_min(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

unum_t get_tload_ten_waits(CpuResource * cpu_resource, unum_t tl_csr_val);
unum_t get_mem_size_tload_tena_tenb(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val);
unum_t get_mem_size_tload_int8(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val);
unum_t get_mem_size_tload_int16(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val);
unum_t get_mem_size_tload_tra8(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val);
unum_t get_mem_size_tload_tra16(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val);
unum_t get_mem_size_tload_tra32(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val);
unum_t get_mem_size_tload(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val);
void selfcheck_boundaries_tload(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t scp_head, unum_t scp_tail);
void selfcheck_set_tload(CpuResource * cpu_resource, unum_t *l1_scp, unum_t tl_csr_val, unum_t selfcheck, unum_t set);
void set_tload_l1scp(CpuResource * cpu_resource, unum_t *l1_scp, unum_t tl_csr_val);
void selfcheck_tload(CpuResource * cpu_resource, unum_t *l1_scp, unum_t tl_csr_val);
unum_t constrain_tload_write_lines(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t scp_head, unum_t scp_tail);
unum_t snp_seq_tload_base(CpuResource * cpu_resource, unum_t *ten_seq, unum_t idx, unum_t num_ops, unum_t *l1_scp, unum_t tenb);
unum_t snp_seq_tload_wrapper(CpuResource * cpu_resource);

#endif
