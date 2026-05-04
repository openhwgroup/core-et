
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include "snp_seq_ps.h"
#include "snp_seq_enc.h"
#include "snp_sequence.h"

using ins_generator = std::function<unum_t(CpuResource*, const et_enc_fields&)>;

std::vector<unum_t> get_all_bits_comb(unum_t num_bits) {
   const unum_t n = pwrtwo(num_bits);
   std::vector<unum_t> mask (n);
   for (int i = 0; i < n; i++) mask[i] = i;
   return mask;
}

std::vector<unum_t> get_all_rm() {
   //std::vector<unum_t> rm (16);
   //for (int i = 0; i < 16; i++) rm[i] = i;
   //return rm;
   std::vector<unum_t> rm {0b111};
   return rm; 
}

/* PS_R_TYPE */
#define FG32B_PS 0b0000100
#define FG32H_PS 0b0001000
#define FG32W_PS 0b0010000
unum_t get_fg32x_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FG32B_PS, FG32H_PS, FG32W_PS};
   std::vector<unum_t> funct3 {0b001};
   return get_r_type_ins(cpu_resource, funct7, enc.ba_reg, enc.rs1, funct3, enc.fd, opcode);
}
#define FGB_PS 0b0100100
#define FGH_PS 0b0101000
#define FGW_PS 0b0110000
unum_t get_fgx_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FGB_PS, FGH_PS, FGW_PS};
   std::vector<unum_t> funct3 {0b001};
   return get_r_type_ins(cpu_resource, funct7, enc.ba_reg, enc.fs1, funct3, enc.fd, opcode);
}
// FCMOVM_PS
unum_t get_fcmovm_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1110111;
   std::vector<unum_t> funct7 {0b0000000};
   std::vector<unum_t> funct3 {0b000};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}
#define FADD_PS 0b0000000
#define FSUB_PS 0b0000100
#define FMUL_PS 0b0001000
#define FDIV_PS 0b0001100 // Emulated by M-CODE, avoid in the mix
unum_t get_faop_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {FADD_PS, FSUB_PS, FMUL_PS};
   std::vector<unum_t> funct3 = get_all_rm();
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}
#define FSGNJ_PS  0b000
#define FSGNJN_PS 0b001
#define FSGNJX_PS 0b010
unum_t get_fsgnx_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0010000};
   std::vector<unum_t> funct3 {FSGNJ_PS, FSGNJN_PS, FSGNJX_PS};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}
#define FMIN_PS 0b000
#define FMAX_PS 0b001
unum_t get_fminmax_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0010100};
   std::vector<unum_t> funct3 {FMIN_PS, FMAX_PS};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}
#define FLE_PS 0b000
#define FLT_PS 0b001
#define FEQ_PS 0b010
unum_t get_fcomp_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b1010000};
   std::vector<unum_t> funct3 {FLE_PS, FLT_PS, FEQ_PS};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}
#define FLEM_PS 0b100
#define FLTM_PS 0b101
#define FEQM_PS 0b110
unum_t get_fcompm_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b1010000};
   std::vector<unum_t> funct3 {FLEM_PS, FLTM_PS, FEQM_PS};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, 0b00111 & enc.md, opcode);
}
#define FSC32B_PS 0b1000100
#define FSC32H_PS 0b1001000
#define FSC32W_PS 0b1010000
unum_t get_fsc32x_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FSC32B_PS, FSC32H_PS, FSC32W_PS};
   std::vector<unum_t> funct3 {0b001};
   return get_r_type_ins(cpu_resource, funct7, enc.ba_reg, enc.rs1, funct3, enc.fs3, opcode);
}
#define FSCB_PS 0b1100100
#define FSCH_PS 0b1101000
#define FSCW_PS 0b1110000
unum_t get_fscx_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b001011;
   std::vector<unum_t> funct7 {FSCB_PS, FSCH_PS, FSCW_PS};
   std::vector<unum_t> funct3 {0b001};
   return get_r_type_ins(cpu_resource, funct7, enc.ba_reg, enc.fs1, funct3, enc.fs3, opcode);
}
// FSQ2_PS
unum_t get_fsq2_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0100111;
   unum_t imm_h = snp_get_bits(cpu_resource, enc.mem_off, 11, 5);
   unum_t imm_l = snp_get_bits(cpu_resource, enc.mem_off,  4, 0);
   std::vector<unum_t> funct7 {imm_h};
   std::vector<unum_t> funct3 {0b101};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.ba_reg, funct3, imm_l, opcode);
}
// FSW_PS
unum_t get_fsw_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   unum_t imm_h = snp_get_bits(cpu_resource, enc.mem_off, 11, 5);
   unum_t imm_l = snp_get_bits(cpu_resource, enc.mem_off,  4, 0);
   std::vector<unum_t> funct7 {imm_h};
   std::vector<unum_t> funct3 {0b110};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.ba_reg, funct3, imm_l, opcode);
}

/* PS_RT_TYPE*/

// FBCX_PS
unum_t get_fbcx_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct12 {0b000000000000};
   std::vector<unum_t> funct3 {0b011};
   return get_rt_type_ins(cpu_resource, funct12, enc.rs1, funct3, enc.fd, opcode);
}
// FCLASS_PS
unum_t get_fclass_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct12 {0b111000000000};
   std::vector<unum_t> funct3 {0b001};
   return get_rt_type_ins(cpu_resource, funct12, enc.fs1, funct3, enc.fd, opcode);
}
#define FSQRT_PS 0b010110000000 // Emulated by M-CODE, avoid in the mix
#define FFRC_PS  0b010110000010
#define FLOG_PS  0b010110000011
#define FEXP_PS  0b010110000100
#define FSIN_PS  0b010110000110 // Emulated by M-CODE, avoid in the mix
#define FRCP_PS  0b010110000111
#define FRSQ_PS  0b010110001000 // Emulated by M-CODE, avoid in the mix
#define FCVT_F16_PS 0b110110001001
#define FCVT_PS_F16 0b110100001010
unum_t get_fops_trans_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct12 {FFRC_PS, FLOG_PS, FEXP_PS, FRCP_PS, FCVT_F16_PS, FCVT_PS_F16};
   std::vector<unum_t> funct3 {0b000};
   return get_rt_type_ins(cpu_resource, funct12, enc.fs1, funct3, enc.fd, opcode);
}
#define FROUND_PS   0b110000000000
#define FCVT_PW_PS  0b110100000000
#define FCVT_PS_PW  0b010110000001
#define FCVT_PWU_PS 0b110000000001
#define FCVT_PS_PWU 0b110100000001
unum_t get_fconv_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct12 {FROUND_PS, FCVT_PW_PS, FCVT_PS_PW, FCVT_PWU_PS, FCVT_PS_PWU};
   std::vector<unum_t> funct3 = get_all_rm();
   return get_rt_type_ins(cpu_resource, funct12, enc.fs1, funct3, enc.fd, opcode);
}
#define FMVZ_X_PS 0b000
#define FMVS_X_PS 0b010
unum_t get_fmov_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct12 = get_all_bits_comb(3);
   const auto add_prefix = [](unum_t idx) {
      unum_t prefix = 0b111000000000;
      return (idx & 0b0111) | prefix;
   };
   std::transform (funct12.begin(), funct12.end(), funct12.begin(), add_prefix);
   const std::vector<unum_t> funct3 {FMVZ_X_PS, FMVS_X_PS};
   return get_rt_type_ins(cpu_resource, funct12, enc.fs1, funct3, enc.rd, opcode);
}
// FSWIZZ_PS
unum_t get_fswizz_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   unum_t imm_h = snp_get_bits(cpu_resource, enc.imm, 7, 3);
   unum_t imm_l = snp_get_bits(cpu_resource, enc.imm, 2, 0);
   unum_t f12_prefix = 0b111001100000;
   std::vector<unum_t> funct12 {(imm_h & 0b011111) | f12_prefix};
   const std::vector<unum_t> funct3 {imm_l};
   return get_rt_type_ins(cpu_resource, funct12, enc.fs1, funct3, enc.fd, opcode);
}

/* PS_RF_TYPE*/

// FCMOV_PS
unum_t get_fcmov_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0111111;
   std::vector<unum_t> funct2 {0b10};
   std::vector<unum_t> funct3 {0b010};
   return get_rf_type_ins(cpu_resource, enc.fs3, funct2, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}
#define FMADD_PS 0b00
#define FMSUB_PS 0b01
#define FNMSUB_PS 0b10
#define FNMADD_PS 0b11
unum_t get_fmop_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1011011;
   std::vector<unum_t> funct2 {FMADD_PS, FMSUB_PS, FNMSUB_PS, FNMADD_PS};
   std::vector<unum_t> funct3 = get_all_rm();
   return get_rf_type_ins(cpu_resource, enc.fs3, funct2, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

/* PS_I_TYPE */
#define FBC_PS 0b000
#define FLW_PS 0b010
unum_t get_fbc_flw_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct12 {enc.mem_off & 0x0fff}; // re-use RT type layout
   std::vector<unum_t> funct3 {FBC_PS, FLW_PS};
   return get_rt_type_ins(cpu_resource, funct12, enc.ba_reg, funct3, enc.fd, opcode);
}
unum_t get_flq2_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0000111;
   std::vector<unum_t> funct12 {enc.mem_off & 0x0fff}; // re-use RT type layout
   std::vector<unum_t> funct3 {0b101};
   return get_rt_type_ins(cpu_resource, funct12, enc.ba_reg, funct3, enc.fd, opcode);
}

/* PS_J_TPYE */
unum_t get_fbci_ps(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0011111;
   return get_j_type_ins(cpu_resource, enc.imm, enc.fd, opcode);
}

unum_t gen_ps_inst(CpuResource * cpu_resource, unum_t ba_reg, unum_t rd, unum_t rs1, unum_t fd, unum_t fs1, unum_t fs2, unum_t fs3, unum_t md, unum_t mem_off, unum_t imm) {
   et_enc_fields fields;
   fields.ba_reg = ba_reg;
   fields.rd = rd;
   fields.rs1 = rs1;
   fields.fd = fd;
   fields.fs1 = fs1;
   fields.fs2 = fs2;
   fields.fs3 = fs3;
   fields.md = md;
   fields.mem_off = mem_off;
   fields.imm = imm;

   const std::vector<ins_generator> r_type {&get_fg32x_ps, &get_fgx_ps, &get_fcmovm_ps, &get_faop_ps, &get_fsgnx_ps, &get_fminmax_ps,\
                                               &get_fcomp_ps, &get_fcompm_ps, &get_fsc32x_ps, &get_fscx_ps, &get_fsq2_ps, &get_fsw_ps};
   const std::vector<ins_generator> rt_type {&get_fbcx_ps, &get_fclass_ps, &get_fops_trans_ps, &get_fconv_ps, &get_fmov_ps, &get_fswizz_ps};
   const std::vector<ins_generator> rf_type {&get_fcmov_ps, &get_fmop_ps};
   const std::vector<ins_generator> i_type {&get_fbc_flw_ps, &get_flq2_ps};
   const std::vector<ins_generator> j_type {&get_fbci_ps};

   std::vector<ins_generator> ins_types;
   std::move(r_type.begin(), r_type.end(), std::back_inserter(ins_types));
   std::move(rt_type.begin(), rt_type.end(), std::back_inserter(ins_types));
   std::move(rf_type.begin(), rf_type.end(), std::back_inserter(ins_types));
   std::move(i_type.begin(), i_type.end(), std::back_inserter(ins_types));
   std::move(j_type.begin(), j_type.end(), std::back_inserter(ins_types));

   unum_t rand_num = snp_urandrange(cpu_resource, 0, ins_types.size());
   const auto gen_ins = ins_types[rand_num];
   return gen_ins(cpu_resource, fields);
}

unum_t get_ps_rd_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rd_regs {13, 14, 24, 25, 30};
   return get_rand_elem_from_vec(cpu_resource, rd_regs);
}

unum_t get_ps_rs1_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rs1_regs {13, 14, 24};
   return get_rand_elem_from_vec(cpu_resource, rs1_regs);
}

unum_t get_ps_ba_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> ba_regs {15, 27, 29};
   return get_rand_elem_from_vec(cpu_resource, ba_regs);
}

unum_t get_seq_ps_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {

   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;

   // Integer registers
   unum_t ba_reg = get_ps_ba_reg(cpu_resource);
   unum_t rd = get_ps_rd_reg(cpu_resource);
   unum_t rs1 = get_ps_rs1_reg(cpu_resource);

   // FP registers
   unum_t fd = et_ins_regs->fd;
   unum_t fs1 = et_ins_regs->fs1;
   unum_t fs2 = et_ins_regs->fs2;
   unum_t fs3 = et_ins_regs->fs3;

   // Mask registers
   unum_t md = snp_urandrange(cpu_resource, 0, 8);

   // Immediate values
   unum_t mem_off = snp_urandrange(cpu_resource, 0, 32) * 64; // 2048 at most
   unum_t imm = snp_randnum(cpu_resource);

   return gen_ps_inst(cpu_resource, ba_reg, rd, rs1, fd, fs1, fs2, fs3, md, mem_off, imm);
}
