/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_AMO_H_
#define _ET_SEQ_AMO_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_seq_enc.h"

unum_t get_amo_two(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_amo_three(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_famo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_famo_min_max(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_scalar_amo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_local_gather_amo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_global_gather_amo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_local_vect_load_amo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_global_vect_store_amo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_local_scatter_amo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_global_scatter_amo(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t gen_amo_inst(CpuResource * cpu_resource, unum_t ba_reg, unum_t rd, unum_t rs1, unum_t fd, unum_t fs1, unum_t fs2);
unum_t get_amo_rd_reg(CpuResource * cpu_resource);
unum_t get_amo_rs1_reg(CpuResource * cpu_resource);
unum_t get_amo_rand_ba_reg(CpuResource * cpu_resource);
unum_t get_amo_ba_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
unum_t get_seq_amo_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);

#endif
