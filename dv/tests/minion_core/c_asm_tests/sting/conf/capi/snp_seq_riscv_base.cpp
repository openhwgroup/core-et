

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include "snp_seq_riscv_base.h"
#include "snp_seq_enc.h"
#include "snp_sequence.h"

using ins_generator = std::function<unum_t(CpuResource*, const et_enc_fields&)>;

std::vector<unum_t> get_riscv_base_rm() {
   //std::vector<unum_t> rm (16);
   //for (int i = 0; i < 16; i++) rm[i] = i;
   //return rm;
   std::vector<unum_t> rm {0b111};
   return rm;
}


#define FLW 0b0000111
unum_t get_flw(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = FLW;
   std::vector<unum_t> funct12 {enc.mem_off & 0x0fff}; // re-use RT type layout
   std::vector<unum_t> funct3 {0b010};
   return get_rt_type_ins(cpu_resource, funct12, enc.ba_reg, funct3, enc.fd, opcode);
}

#define FSW 0b0100111
unum_t get_fsw(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = FSW;
   unum_t imm_h = snp_get_bits(cpu_resource, enc.mem_off, 11, 5);
   unum_t imm_l = snp_get_bits(cpu_resource, enc.mem_off,  4, 0);
   std::vector<unum_t> funct7 {imm_h};
   std::vector<unum_t> funct3 {0b010};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.ba_reg, funct3, imm_l, opcode);
}

#define FMADD_S  0b1000011
#define FMSUB_S  0b1000111
#define FNMSUB_S 0b1001011
#define FNMADD_S 0b1001111
unum_t get_fused_op(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const std::vector<unum_t> fused_opcodes {FMADD_S, FMSUB_S, FNMSUB_S, FNMADD_S};
   unum_t opcode = get_rand_elem_from_vec(cpu_resource, fused_opcodes);
   std::vector<unum_t> funct2 {0b00};
   std::vector<unum_t> funct3 = get_riscv_base_rm();
   return get_rf_type_ins(cpu_resource, enc.fs3, funct2, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

#define FADD_S 0b0000000
#define FSUB_S 0b0000100
#define FMUL_S 0b0001000
unum_t get_faop(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   std::vector<unum_t> funct7 {FADD_S, FSUB_S, FMUL_S};
   std::vector<unum_t> funct3 = get_riscv_base_rm();
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

#define FMIN_S 0b000
#define FMAX_S 0b001
unum_t get_fminmax(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   std::vector<unum_t> funct7 {0b0010100};
   std::vector<unum_t> funct3 {FMIN_S, FMAX_S};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

#define FSGNJ_S  0b000
#define FSGNJN_S 0b001
#define FSGNJX_S 0b010
unum_t get_fsgnx(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   std::vector<unum_t> funct7 {0b0010000};
   std::vector<unum_t> funct3 {FSGNJ_S, FSGNJN_S, FSGNJX_S};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

#define FLE_S 0b000
#define FLT_S 0b001
#define FEQ_S 0b010
unum_t get_fcomp(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   std::vector<unum_t> funct7 {0b1010000};
   std::vector<unum_t> funct3 {FLE_S, FLT_S, FEQ_S};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.rd, opcode);
}

#define FCVT_S_W  0b110100000000
#define FCVT_S_WU 0b110100000001
unum_t get_fconv_0(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   std::vector<unum_t> funct12 {FCVT_S_W, FCVT_S_WU};
   std::vector<unum_t> funct3 = get_riscv_base_rm();
   return get_rt_type_ins(cpu_resource, funct12, enc.rs1, funct3, enc.fd, opcode);
}

#define FCVT_W_S  0b110000000000
#define FCVT_WU_S 0b110000000001
unum_t get_fconv_1(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   std::vector<unum_t> funct12 {FCVT_W_S, FCVT_WU_S};
   std::vector<unum_t> funct3 = get_riscv_base_rm();
   return get_rt_type_ins(cpu_resource, funct12, enc.fs1, funct3, enc.rd, opcode);
}

#define FMV_X_W 0b1110000
unum_t get_fmov_0(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   const std::vector<unum_t> funct7 {FMV_X_W};
   const std::vector<unum_t> funct3 {0b000};
   return get_r_type_ins(cpu_resource, funct7, 0, enc.fs1, funct3, enc.rd, opcode);
}

#define FMV_W_X 0b1111000
unum_t get_fmov_1(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   const std::vector<unum_t> funct7 {FMV_W_X};
   const std::vector<unum_t> funct3 {0b000};
   return get_r_type_ins(cpu_resource, funct7, 0, enc.rs1, funct3, enc.fd, opcode);
}

// FCLASS_S
unum_t get_fclass(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1010011;
   std::vector<unum_t> funct12 {0b111000000000};
   std::vector<unum_t> funct3 {0b001};
   return get_rt_type_ins(cpu_resource, funct12, enc.fs1, funct3, enc.rd, opcode);
}

unum_t gen_riscv_base_inst(CpuResource * cpu_resource, unum_t ba_reg, unum_t rd, unum_t rs1, unum_t fd, unum_t fs1, unum_t fs2, unum_t fs3, unum_t mem_off, unum_t imm) {
   et_enc_fields fields;
   fields.ba_reg = ba_reg;
   fields.rs1 = rs1;
   fields.fd = fd;
   fields.fs1 = fs1;
   fields.fs2 = fs2;
   fields.fs3 = fs3;
   fields.mem_off = mem_off;
   fields.imm = imm;
   fields.rd = rd;

   const std::vector<ins_generator> riscv_type {&get_flw, &get_fsw, &get_fused_op, &get_faop, &get_fminmax, &get_fsgnx, &get_fcomp, &get_fconv_0, &get_fconv_1, &get_fmov_0, &get_fmov_1, &get_fclass};

   unum_t rand_num = snp_urandrange(cpu_resource, 0, riscv_type.size());
   const auto gen_ins = riscv_type[rand_num];
   return gen_ins(cpu_resource, fields);
}

unum_t get_s_rd_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rd_regs {13, 14, 24, 25, 30};
   return get_rand_elem_from_vec(cpu_resource, rd_regs);
}

unum_t get_s_rs1_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rs1_regs {13, 14, 24};
   return get_rand_elem_from_vec(cpu_resource, rs1_regs);
}

unum_t get_s_ba_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> ba_regs {15, 27, 29};
   return get_rand_elem_from_vec(cpu_resource, ba_regs);
}

unum_t get_seq_riscv_base_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {

   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;

   // Integer registers
   unum_t ba_reg = get_s_ba_reg(cpu_resource);
   unum_t rd = get_s_rd_reg(cpu_resource);
   unum_t rs1 = get_s_rs1_reg(cpu_resource);

   // FP registers
   unum_t fd = et_ins_regs->fd;
   unum_t fs1 = et_ins_regs->fs1;
   unum_t fs2 = et_ins_regs->fs2;
   unum_t fs3 = et_ins_regs->fs3;

   // Immediate values
   unum_t mem_off = snp_urandrange(cpu_resource, 0, 32) * 64; // 2048 at most
   unum_t imm = snp_randnum(cpu_resource);

   return gen_riscv_base_inst(cpu_resource, ba_reg, rd, rs1, fd, fs1, fs2, fs3, mem_off, imm);
}
