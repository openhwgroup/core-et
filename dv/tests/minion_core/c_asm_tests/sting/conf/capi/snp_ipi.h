/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_IPI_H
#define _ET_IPI_H

#include "cpu_resource.h"

unum_t snp_get_ipi_trigger_mask(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_get_ipi_shire_id(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_is_set_redirect_pc(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_get_ipi_redirect_pc_addr(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_set_ipi_redirect_mask(CpuResource * cpu_resource, unum_t cpuid, unum_t mask);
unum_t snp_get_ipi_redirect_filter(CpuResource * cpu_resource, unum_t cpuid, unum_t and_mode);
unum_t snp_get_ipi_redirect_filter_addr(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_get_redirect_trigger_addr(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_is_set_redirect_filter(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_get_trigger_addr(CpuResource * cpu_resource, unum_t cpuid);
unum_t snp_get_trigger_clear_addr(CpuResource * cpu_resource, unum_t cpuid);

unum_t glb_snp_get_ipi_flb_num(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid, unum_t eot_flb_num);
unum_t snp_get_ipi_flb_match_val(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid);
unum_t snp_get_ipi_redirect_trigger_value(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid, unum_t and_mode);
unum_t snp_get_ipi_trigger_value(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid);

unum_t glb_rand_snp_get_ipi_flb_num(CpuResource * cpu_resource, unum_t cpuid, unum_t eot_flb_num);
unum_t rand_snp_get_ipi_flb_match_val(CpuResource * cpu_resource, unum_t cpuid);
unum_t rand_snp_get_ipi_redirect_trigger_value(CpuResource * cpu_resource, unum_t cpuid, unum_t and_mode);
unum_t rand_snp_get_ipi_trigger_value(CpuResource * cpu_resource, unum_t cpuid);

unum_t glb_dev_snp_get_ipi_flb_num(CpuResource * cpu_resource, unum_t cpuid, unum_t eot_flb_num);
unum_t dev_snp_get_ipi_flb_match_val(CpuResource * cpu_resource, unum_t cpuid);
unum_t dev_snp_get_ipi_redirect_trigger_value(CpuResource * cpu_resource, unum_t cpuid, unum_t and_mode);
unum_t dev_snp_get_ipi_trigger_value(CpuResource * cpu_resource, unum_t cpuid);
#endif
