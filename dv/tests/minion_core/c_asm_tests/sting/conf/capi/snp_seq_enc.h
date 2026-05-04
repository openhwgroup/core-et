/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_ENC_H_
#define _ET_SEQ_ENC_H_

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"
#include "snp_sequence.h"

struct et_ins_conf {
   unum_t fd;
   unum_t fs1;
   unum_t fs2;
   unum_t fs3;
   unum_t aligned_ba_reg;
};

struct et_enc_fields {
   unum_t ba_reg;
   unum_t aligned_ba_reg;
   unum_t rd;
   unum_t rs1;
   unum_t rs2;
   unum_t fd;
   unum_t fs1;
   unum_t fs2;
   unum_t fs3;
   unum_t md;
   unum_t ms1;
   unum_t ms2;
   unum_t mem_off;
   unum_t imm;
};

unum_t get_rand_elem_from_vec(CpuResource * cpu_resource, const std::vector<unum_t> &ops);
unum_t get_r_type_ins(CpuResource * cpu_resource, const std::vector<unum_t> &funct7, const unum_t xs2, const unum_t xs1, const std::vector<unum_t> &funct3, const unum_t xd_xs3, const unum_t opcode);
unum_t get_rt_type_ins(CpuResource * cpu_resource, const std::vector<unum_t> &funct12, const unum_t xs1, const std::vector<unum_t> &funct3, const unum_t xd, const unum_t opcode);
unum_t get_rf_type_ins(CpuResource * cpu_resource, const unum_t fs3, const std::vector<unum_t> &funct2, const unum_t fs2, const unum_t fs1, const std::vector<unum_t> &funct3, const unum_t fd, const unum_t opcode);
unum_t get_j_type_ins(CpuResource * cpu_resource, const unum_t imm, const unum_t fd, const unum_t opcode);

unum_t snp_init_et_ins_conf(CpuResource * cpu_resource, unum_t vpu_rf_ptr);
unum_t free_et_ins_conf_ptr(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
unum_t snp_set_et_ins_conf(CpuResource * cpu_resource, unum_t vpu_rf_ptr, unum_t amo_conf_ptr);
unum_t get_free_aligned_ba_reg(CpuResource * cpu_resource);
unum_t get_et_ins_fs1_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
unum_t get_et_ins_fs2_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
unum_t get_et_ins_fs3_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
unum_t get_free_fsx(CpuResource * cpu_resource, unum_t *vpu_rf);
unum_t get_et_ins_aligned_ba_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
unum_t et_ins_requires_fs1(CpuResource * cpu_resource, unum_t et_ins_ptr);
unum_t et_ins_requires_fs2(CpuResource * cpu_resource, unum_t et_ins_ptr);
unum_t et_ins_requires_fs3(CpuResource * cpu_resource, unum_t et_ins_conf_ptr);
#endif
