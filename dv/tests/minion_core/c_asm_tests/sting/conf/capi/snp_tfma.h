/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_TFMA_H
#define _ET_TFMA_H

#include "cpu_resource.h"
#include "board_conf.h"
#include "lib_snip_transform.h"

unum_t snp_tfma_from_scp(CpuResource * cpu_resource, unum_t value);
unum_t snp_tfma_from_tenb(CpuResource * cpu_resource, unum_t value);
unum_t snp_tfma_set_ima8a32(CpuResource * cpu_resource, unum_t csr_value);
unum_t snp_set_valid_tfma_op(CpuResource * cpu_resource, unum_t rand_val, unum_t tenb);
unum_t snp_set_ilegal_tfma_op(CpuResource * cpu_resource, unum_t rand_val, unum_t tenb);
unum_t snp_set_ilegal_tfma_op(CpuResource * cpu_resource, unum_t rand_val, unum_t tenb);
unum_t snp_tima8a32_update_tenc_clear(CpuResource * cpu_resource, unum_t old_a_rows, unum_t csr_value);
unum_t snp_get_tfma_wrapper(CpuResource * cpu_resource);
unum_t snp_get_tfma_tenb_wrapper(CpuResource * cpu_resource);
unum_t snp_get_tfma_pre_ten_waits(CpuResource * cpu_resource, unum_t tfma_enc);
unum_t glbl_snp_sm_tfma_wrapper(CpuResource * cpu_resource, unum_t tenc, unum_t tenb, unum_t first_pass, unum_t tl_tenb_csr_val);
unum_t glbl_snp_get_sm_tfma(CpuResource * cpu_resource);
unum_t glbl_snp_get_sm_tenb_tfma_wrapper(CpuResource * cpu_resource, unum_t tl_tenb_csr_val);
#endif
