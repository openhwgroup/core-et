/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_PI_H_
#define _ET_SEQ_PI_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_seq_enc.h"

#define pwrtwo(x) (1 << (x))

unum_t get_pi_0(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_pi_1(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_pi_2(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_pi_3(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_pi_4(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_pi_5(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_pi_6(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_pi_7(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsatu8_pi(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fltm_pi(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fsetm_pi(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_fbci_pi(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t get_packb(CpuResource * cpu_resource, const et_enc_fields& enc);
unum_t gen_pi_inst(CpuResource * cpu_resource, unum_t rs1, unum_t rs2, unum_t rd, unum_t fs1, unum_t fs2, unum_t fd, unum_t imm, unum_t md);
unum_t get_pi_rd_reg(CpuResource * cpu_resource);
unum_t get_pi_rsx_reg(CpuResource * cpu_resource);
unum_t get_seq_pi_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);

#endif
