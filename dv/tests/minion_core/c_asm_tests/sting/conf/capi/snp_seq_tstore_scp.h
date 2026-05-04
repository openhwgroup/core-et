/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TSTORE_SCP_H_
#define _ET_SEQ_TSTORE_SCP_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_cts.h"

#define et_min(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

unum_t get_ts_scp_ten_waits(CpuResource * cpu_resource, unum_t tstore_csr_val);
unum_t get_mem_size_ts_scp(CpuResource * cpu_resource, unum_t ts_scp_csr_val, unum_t x31_val);
unum_t selfcheck_set_ts_scp(CpuResource * cpu_resource, unum_t *l1_scp, unum_t ts_scp_csr_val, unum_t selfcheck, unum_t set);
void selfcheck_boundaries_ts_scp(CpuResource * cpu_resource, unum_t ts_scp_csr_val, unum_t head, unum_t tail);
unum_t constrain_ts_scp_read(CpuResource * cpu_resource, unum_t ts_scp_csr_val, unum_t scp_head, unum_t scp_tail);
unum_t snp_seq_ts_scp_base(CpuResource * cpu_resource, unum_t *l1_scp);

#endif
