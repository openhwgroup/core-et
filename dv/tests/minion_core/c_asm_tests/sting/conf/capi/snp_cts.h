/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_CTS_H
#define _ET_CTS_H

#include "cpu_resource.h"
unum_t snp_cts_set_valid_config(CpuResource * cpu_resource, unum_t csr_value, unum_t rand_init);
unum_t snp_cts_set_invalid_config(CpuResource * cpu_resource, unum_t csr_value, unum_t rand_init);
unum_t snp_align_coop_addr(CpuResource * cpu_resource, unum_t csr_value, unum_t addr);
unum_t snp_get_ts_stride(CpuResource * cpu_resource, unum_t csr_value, unum_t stride);
unum_t glbl_snp_get_min_cpu_cts(CpuResource * cpu_resource, unum_t num_cpus, unum_t csr_value);
unum_t snp_get_max_cpu_cts(CpuResource * cpu_resource, unum_t min_cpu, unum_t csr_value);
unum_t snp_in_cts(CpuResource * cpu_resource, unum_t csr_value);
unum_t snp_get_cts_base_minion(CpuResource * cpu_resource, unum_t cts_way);
unum_t glbl_snp_set_rand_cts_way(CpuResource * cpu_resource, unum_t csr_value);
unum_t snp_get_cts_way(CpuResource * cpu_resource, unum_t csr_value);
unum_t snp_get_tstore_wrapper(CpuResource * cpu_resource);
unum_t glbl_snp_get_coop_tstore_wrapper(CpuResource * cpu_resource);
unum_t snp_get_tstore_scp_wrapper(CpuResource * cpu_resource);
unum_t snp_get_tstore_from_tlcsr(CpuResource * cpu_resource, unum_t tlcsr);
unum_t snp_get_ts_scp_start_line(CpuResource * cpu_resource, unum_t tscsr);
unum_t snp_get_cts_base_lcpuid(CpuResource * cpu_resource, unum_t ts_csr_val);
#endif
