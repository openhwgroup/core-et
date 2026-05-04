/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_MASK_H_
#define _ET_SEQ_MASK_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_seq_enc.h"

#define pwrtwo(x) (1 << (x))

unum_t get_maskandor(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_maskpop(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_masknot(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_maskxor(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_maskmov_m_x(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_maskmova_m_x(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_maskmova_x_m(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t gen_mask_inst(CpuResource * cpu_resource, unum_t md, unum_t ms1, unum_t ms2, unum_t rd, unum_t rs1, unum_t imm);
unum_t get_mask_rd_reg(CpuResource * cpu_resource);
unum_t get_mask_rs1_reg(CpuResource * cpu_resource);
unum_t get_seq_mask_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);

#endif
