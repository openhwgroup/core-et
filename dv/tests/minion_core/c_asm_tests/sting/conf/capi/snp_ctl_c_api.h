/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_CTL_H
#define _ET_CTL_H

#include "cpu_resource.h"

#define TENB 1
#define NON_TENB 0
#define COOP 1
#define NON_COOP 0

unum_t snp_get_num_neighs(CpuResource * cpu_resource, unum_t num_cpus);
unum_t snp_ctl_set_valid_transformation(CpuResource * cpu_resource, unum_t csr_value);
unum_t snp_ctl_set_invalid_transformation(CpuResource * cpu_resource, unum_t csr_value);
unum_t glbl_snp_get_coop_neigh_mask(CpuResource * cpu_resource);
unum_t snp_invert_coop_neigh_mask(CpuResource * cpu_resource, unum_t rand_neigh_mask);
unum_t snp_cpu_in_mask(CpuResource * cpu_resource, unum_t cpuId, unum_t coop_mask);
unum_t glbl_snp_get_coop_minion_mask(CpuResource * cpu_resource);
unum_t snp_tensor_mask(CpuResource * cpu_resource, unum_t num_cpus, unum_t rand_val, unum_t rand_neigh_mask, unum_t rand_minion_mask);
unum_t snp_cpu_in_mask(CpuResource * cpu_resource, unum_t cpuId, unum_t coop_mask);
unum_t glbl_snp_get_tfma_tenb_value(CpuResource * cpu_resource, unum_t tenb_csr_value, unum_t is_coop);
unum_t snp_get_coop_minion_mask_all_active(CpuResource * cpu_resource);
unum_t snp_get_coop_neigh_mask_all_active(CpuResource * cpu_resource);
unum_t glbl_snp_get_tload_base(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t tenb);
unum_t snp_get_tload_wrapper(CpuResource * cpu_resource);
unum_t glbl_snp_get_coop_tload_wrapper(CpuResource * cpu_resource);
unum_t snp_get_tload_tenb_wrapper(CpuResource * cpu_resource);
unum_t snp_get_coop_tload_tenb_wrapper(CpuResource * cpu_resource);
unum_t glbl_snp_get_tload_x31_wrapper(CpuResource * cpu_resource, unum_t max_cacheline_stride);
unum_t snp_get_tl_pre_ten_waits(CpuResource * cpu_resource, unum_t tl_enc);

#endif
