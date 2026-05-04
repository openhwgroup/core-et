/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_TCONV_H_
#define _ET_SEQ_TCONV_H_

#include "lib_snip_transform.h"

unum_t get_tconv_size_ten_waits(CpuResource * cpu_resource, unum_t tconv_csr_val);
unum_t get_tconv_ctrl_ten_waits(CpuResource * cpu_resource, unum_t tconv_csr_val);
unum_t get_mem_size_tconv_size(CpuResource * cpu_resource, unum_t tconv_csr_val, unum_t x31_val);
unum_t get_mem_size_tconv_ctrl(CpuResource * cpu_resource, unum_t tconv_csr_val, unum_t x31_val);
unum_t snp_seq_tconv_size_base(CpuResource * cpu_resource);
unum_t snp_paired_tconv_ctrl_base(CpuResource * cpu_resource, unum_t tconv_size);
unum_t snp_seq_tconv_ctrl_base(CpuResource * cpu_resource);

#endif
