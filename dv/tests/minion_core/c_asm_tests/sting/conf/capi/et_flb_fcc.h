/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _ET_FLB_H
#define _ET_FLB_H

unum_t snp_get_flb_addr(CpuResource * cpu_resource, unum_t barrier_num);
unum_t snp_get_flb_match_val(CpuResource * cpu_resource);
unum_t snp_get_fcc_mask(CpuResource * cpu_resource, unum_t shire_id, unum_t thread_id);
unum_t snp_get_fcc_mask_local_shire_same_tid(CpuResource * cpu_resource);
unum_t glbl_snp_get_flb_barrier_num (CpuResource * cpu_resource, unum_t cpuid, unum_t eot_flb_num);
unum_t snp_get_flb_csr_value(CpuResource * cpu_resource, unum_t barrier_num);
unum_t snp_is_reset_cpu(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_get_flb_csr_value_cov(CpuResource * cpu_resource, unum_t barrier_num, unum_t match_val);
unum_t snp_get_num_flb_cov(CpuResource * cpu_resource, unum_t flb_match_val);
unum_t snp_get_cpuy_fcc_overflow(CpuResource * cpu_resource, unum_t cpux);
unum_t snp_get_fcc_overflow_mask(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_get_fcc_addr(CpuResource * cpu_resource, int shire_id, unum_t thread_id, unum_t counter_num);
unum_t snp_get_fcc_addr_local_shire(CpuResource * cpu_resource, unum_t thread_id, unum_t counter_num);
unum_t snp_get_fcc_addr_local_shire_same_tid(CpuResource * cpu_resource, unum_t counter_num);
unum_t snp_get_fcc_total_players(CpuResource * cpu_resource, unum_t per_shire_participants_mask);
unum_t glbl_snp_get_fcc_master(CpuResource * cpu_resource, unum_t per_shire_participants_mask);

#endif

