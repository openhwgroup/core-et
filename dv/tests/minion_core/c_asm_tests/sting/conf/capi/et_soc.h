/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _ET_SOC_H
#define _ET_SOC_H

#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include <stdlib.h>
#include <stdio.h>
#include <set>

#define NUM_SHIRES            36
#define NUM_NEIGH_PER_SHIRE   4
#define NUM_MINION_PER_NEIGH  8
#define NUM_THREAD_PER_MINION 2

#define NUM_THREAD_PER_NEIGH  (NUM_THREAD_PER_MINION*NUM_MINION_PER_NEIGH)
#define NUM_THREAD_PER_SHIRE  (NUM_THREAD_PER_NEIGH*NUM_NEIGH_PER_SHIRE)
#define NUM_MINION_PER_SHIRE  (NUM_MINION_PER_NEIGH*NUM_NEIGH_PER_SHIRE)

std::set<unum_t> snp_get_active_local_shire_minions_set(CpuResource * cpu_resource);
unum_t get_idx_from_active_local_shire_minions(CpuResource * cpu_resource);
unum_t snp_get_local_active_harts(CpuResource * cpu_resource);
std::set<unum_t> snp_get_active_shires_set(CpuResource * cpu_resource);
unum_t snp_get_shireid_from_set_idx(CpuResource * cpu_resource, unum_t lsid);
unum_t snp_get_num_active_shires(CpuResource * cpu_resource);
std::set<unum_t> snp_get_soc_active_minions_set(CpuResource * cpu_resource);
unum_t snp_get_soc_active_minions(CpuResource * cpu_resource);
unum_t snp_get_phycpu_mask(CpuResource * cpu_resource, unum_t logcpu_mask);
unum_t snp_get_phycpu(CpuResource * cpu_resource, unum_t hartid);
unum_t snp_get_phycpu_shire(CpuResource * cpu_resource, unum_t lcpuid);
unum_t snp_get_shire_id(CpuResource * cpu_resource, unum_t hartid);
unum_t snp_get_neigh_id(CpuResource * cpu_resource, unum_t hartid);
unum_t snp_get_minion_id(CpuResource * cpu_resource, unum_t hartid);
unum_t snp_get_global_minion_id(CpuResource * cpu_resource, unum_t hartid);
unum_t snp_get_thread_id(CpuResource * cpu_resource, unum_t hartid);
unum_t snp_is_min_shire(CpuResource * cpu_resource, unum_t cpuid);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_bit_set_in_mask
//
//    This function returns 1 if the mask[bit] is set.
//
unum_t snp_bit_set_in_mask(CpuResource * cpu_resource, unum_t bit, unum_t mask);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_setc
//
//    This function returns the number of bits set in the provided bitmask.
//
unum_t snp_setc(CpuResource * cpu_resource, unum_t mask);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_first_bit_set
//
//    This function returns the position of the lowest bit that is set in the provided bitmask.
//
unum_t snp_get_first_bit_set(CpuResource * cpu_resource, unum_t mask);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_odd_bits
//
//    This function returns a bitmask composed of all odd bits from the provided bitmask
//
unum_t snp_get_odd_bits(CpuResource * cpu_resource, unum_t mask);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_even_bits
//
//    This function returns a bitmask composed of all even bits from the provided bitmask
//
unum_t snp_get_even_bits(CpuResource * cpu_resource, unum_t mask);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_min
//
//    This function returns the minimum between 2 numbers
//
unum_t snp_min(CpuResource * cpu_resource, unum_t a, unum_t b);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_hartid_biased
//
//    This function is used in tensor snippets. Given a number of harts, returns a value biased toward even values
//
unum_t snp_get_hartid_biased(CpuResource * cpu_resource, unum_t nharts);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_hartid_biased_from_rand
//
//    This function is used in tensor snippets. Given a number of harts, returns a value biased toward even values.
//    This is a special case where the random values are passed from outside instead of calling rand() inside
//
unum_t snp_get_hartid_biased_from_rand(CpuResource * cpu_resource, unum_t nharts, unum_t bias, unum_t rand_hart, unum_t rand_op);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_harts_mask
//
//    This function create a mask according to the number of hart ids given in numofids
//    Each bit in baks set to 1 represents an active hart.
//
unum_t snp_gen_harts_mask(CpuResource * cpu_resource, unum_t num_cpus);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_minion_mask
//
//    This function creates a mask based on the cpuid. The result will contain one single bit set.
//    For example, HART0 will set bit 0, HART1, bit 1, HART2, bit 2...
//
unum_t snp_get_minion_mask(CpuResource * cpu_resource, unum_t cpuid);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_esr_addr_coop_mode
//
//    This function returns the ESR address for COOP_MODE. The shire ID is set
//    based on the logical cpuid passed as argument
//
unum_t snp_get_esr_addr_coop_mode (CpuResource * cpu_resource, unum_t cpuid);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_num_harts_local_shire
//
//    This function returns the number of active harts in the same shire as cpuid
//
unum_t snp_get_num_harts_local_shire(CpuResource * cpu_resource, unum_t cpuid);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_mprot_base
//
//    This function accepts as input an mprot value and takes care of setting the necessary bits
//    when the secure memory mode is enabled
//
unum_t snp_get_mprot_base(CpuResource * cpu_resource, unum_t mprot);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_rand_mprot
//
//    This function returns a random mprot value but takes care of setting the necessary bits
//    when the secure memory mode is enabled
//
unum_t snp_get_rand_mprot(CpuResource * cpu_resource);

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: snp_get_tensor_rand_mprot
//
//    This function returns a random mprot value but takes care of setting the necessary bits
//    when the secure memory mode is enabled. This function is only used in SNP_tensor_store_pma_mprot, SNP_tensor_load_pma_mprot
//
unum_t snp_get_tensor_rand_mprot(CpuResource * cpu_resource);

uint32_t snp_get_unpr_fregs_mask(CpuResource * cpu_resource);
#endif

