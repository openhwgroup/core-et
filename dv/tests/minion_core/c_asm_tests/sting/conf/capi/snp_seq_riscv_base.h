/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_RISCV_BASE_H
#define _ET_SEQ_RISCV_BASE_H

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_seq_enc.h"

#define pwrtwo(x) (1 << (x))

std::vector<unum_t> get_riscv_base_rm();
unum_t get_flw(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsw(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fused_op(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_faop(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fminmax(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsgnx(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fcomp(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fconv_0(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fconv_1(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fmov_0(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fmov_1(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fclass(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t gen_riscv_base_inst(CpuResource * cpu_resource, unum_t ba_reg, unum_t rs1, unum_t fd, unum_t fs1, unum_t fs2, unum_t fs3, unum_t mem_off, unum_t imm);
unum_t get_s_rd_reg(CpuResource * cpu_resource);
unum_t get_s_rs1_reg(CpuResource * cpu_resource);
unum_t get_s_ba_reg(CpuResource * cpu_resource);
unum_t get_seq_riscv_base_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);

#endif
