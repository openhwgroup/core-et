/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_TR_AUTO_H
#define _ET_TR_AUTO_H

#include "cpu_resource.h"
#include "lib_snip_transform.h"
unum_t snp_get_tensor_reduce_partner(CpuResource * cpu_resource, unum_t nharts, unum_t cpu_x);
unum_t snp_set_legal_reduce_op(CpuResource * cpu_resource, unum_t rand_val, unum_t gl_rand_val);
unum_t snp_set_illegal_reduce_op(CpuResource * cpu_resource, unum_t csr_val);

unum_t snp_tree_compatible(CpuResource * cpu_resource, unum_t tree, unum_t* minion_map);
unum_t glbl_snp_tensor_reduce_set_tree_lvl(CpuResource * cpu_resource, unum_t csr_val);
unum_t snp_get_reduce_partner(CpuResource * cpu_resource, unum_t minion_id, unum_t tree);
unum_t snp_in_reduce(CpuResource * cpu_resource, unum_t csr_val, unum_t cpuid);

unum_t glbl_snp_tensor_reduce_auto(CpuResource * cpu_resource, unum_t num_cpus, unum_t rand_val, unum_t gl_rand_val);
unum_t glbl_snp_tensor_reduce_broadcast(CpuResource * cpu_resource, unum_t num_cpus, unum_t rand_val, unum_t gl_rand_val);

unum_t snp_tensor_reduce_err(CpuResource * cpu_resource, unum_t csr_val);


unum_t snp_is_tensor_reduce_receiver(CpuResource * cpu_resource, unum_t tree_lvl, unum_t minion_id, unum_t is_auto);
unum_t snp_get_valid_tensor_reduce_tree_lvl(CpuResource * cpu_resource, unum_t cpuid, unum_t is_auto);
unum_t glbl_snp_get_treduce_auto_bcast_wrapper(CpuResource * cpu_resource);
unum_t get_partner_lmid(CpuResource * cpu_resource, unum_t cpuid, unum_t csr_val);

#endif
