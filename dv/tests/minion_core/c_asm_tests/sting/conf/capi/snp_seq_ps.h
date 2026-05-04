/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_PS_H_
#define _ET_SEQ_PS_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_seq_enc.h"

#define pwrtwo(x) (1 << (x))

std::vector<unum_t> get_all_bits_comb(unum_t num_bits);
std::vector<unum_t> get_all_rm();

/* PS_R_TYPE */
unum_t get_fg32x_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fgx_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fcmovm_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_faop_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsgnx_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fminmax_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fcomp_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fcompm_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsc32x_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fscx_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsq2_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsw_ps(CpuResource * cpu_resource, const et_enc_fields& enc);

/* PS_RT_TYPE*/
unum_t get_fbcx_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fclass_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fops_trans_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fconv_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fmov_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fswizz_ps(CpuResource * cpu_resource, const et_enc_fields& enc);

/* PS_RF_TYPE */
unum_t get_fcmov_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fmop_ps(CpuResource * cpu_resource, const et_enc_fields& enc);

/* PS_I_TYPE */
unum_t get_fbc_flw_ps(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_flq2_ps(CpuResource * cpu_resource, const et_enc_fields& enc);

/* PS_J_TPYE */
unum_t get_fbci_ps(CpuResource * cpu_resource, const et_enc_fields& enc);

unum_t gen_ps_inst(CpuResource * cpu_resource, unum_t ba_reg, unum_t rd, unum_t rs1, unum_t fd, unum_t fs1, unum_t fs2, unum_t fs3, unum_t md, unum_t mem_off, unum_t imm);
unum_t get_ps_rd_reg(CpuResource * cpu_resource);
unum_t get_ps_rs1_reg(CpuResource * cpu_resource);
unum_t get_ps_ba_reg(CpuResource * cpu_resource);
unum_t get_seq_ps_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
#endif
