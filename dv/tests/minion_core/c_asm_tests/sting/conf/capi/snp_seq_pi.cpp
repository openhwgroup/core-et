
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include "snp_seq_pi.h"
#include "snp_seq_enc.h"
#include "snp_sequence.h"

using ins_generator = std::function<unum_t(CpuResource*, const et_enc_fields&)>;

#define FADD_PI 0b000
#define FSLL_PI 0b001
#define FXOR_PI 0b100
#define FSRL_PI 0b101
#define FOR_PI  0b110
#define FAND_PI 0b111
unum_t get_pi_0(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0000011};
   std::vector<unum_t> funct3 {FADD_PI, FSLL_PI, FXOR_PI, FSRL_PI, FOR_PI, FAND_PI};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

/* FLE_PI, FLT_PI, FEQ_PI, FLTU_PI
 * FMIN_PI, FMAX_PI, FMINU_PI, FMAXU_PI
 */
//FDIV_PI, FDIVU_PI, FREM_PI, FREMU_PI are not included because M-MODE implements it
unum_t get_pi_1(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b1010011, 0b0010111};
   std::vector<unum_t> funct3 {0b000, 0b001, 0b010, 0b011};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

#define FMUL_PI   0b000
#define FMULH_PI  0b001
#define FMULHU_PI 0b010
unum_t get_pi_2(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0001011};
   std::vector<unum_t> funct3 {FMUL_PI, FMULH_PI, FMULHU_PI};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

#define FSLLI_PI 0b001
#define FSRLI_PI 0b101
#define FSRAI_PI 0b111
unum_t get_pi_3(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0100111};
   std::vector<unum_t> funct3 {FSLLI_PI, FSRLI_PI, FSRAI_PI};
   return get_r_type_ins(cpu_resource, funct7, enc.imm & 0x1f, enc.fs1, funct3, enc.fd, opcode);
}

#define FNOT_PI  0b010
#define FSAT8_PI 0b011
unum_t get_pi_4(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0000011};
   std::vector<unum_t> funct3 {FNOT_PI, FSAT8_PI};
   return get_r_type_ins(cpu_resource, funct7, 0, enc.fs1, funct3, enc.fd, opcode);
}

#define FSUB_PI 0b000
#define FSRA_PI 0b101
unum_t get_pi_5(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0000111};
   std::vector<unum_t> funct3 {FSUB_PI, FSRA_PI};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2, enc.fs1, funct3, enc.fd, opcode);
}

#define FADDI_PI 0b000
#define FANDI_PI 0b001
unum_t get_pi_6(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0111111;
   unum_t imm_h = snp_get_bits(cpu_resource, enc.imm, 9, 5);
   unum_t imm_l = snp_get_bits(cpu_resource, enc.imm, 4, 0);
   std::vector<unum_t> funct7 {(imm_h << 2) + 0b10};
   std::vector<unum_t> funct3 {FADDI_PI, FANDI_PI};
   return get_r_type_ins(cpu_resource, funct7, imm_l, enc.fs1, funct3, enc.fd, opcode);
}

#define FPACKREPB_PI 0b000
#define FPACKREPH_PI 0b001
unum_t get_pi_7(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0010011};
   std::vector<unum_t> funct3 {FPACKREPB_PI, FPACKREPH_PI};
   return get_r_type_ins(cpu_resource, funct7, 0,  enc.fs1, funct3, enc.fd, opcode);
}

/*FSATU8_PI*/
unum_t get_fsatu8_pi(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0000011};
   std::vector<unum_t> funct3 {0b011};
   return get_r_type_ins(cpu_resource, funct7, 1,  enc.fs1, funct3, enc.fd, opcode);
}

/*FLTM_PI*/
unum_t get_fltm_pi(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0011111};
   std::vector<unum_t> funct3 {0b000};
   return get_r_type_ins(cpu_resource, funct7, enc.fs2,  enc.fs1, funct3, enc.md & 0x7, opcode);
}

/*FSETM_PI*/
unum_t get_fsetm_pi(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b1010011};
   std::vector<unum_t> funct3 {0b100};
   return get_r_type_ins(cpu_resource, funct7, 0,  enc.fs1, funct3, enc.md & 0x7, opcode);
}

/*FBCI_PI*/
unum_t get_fbci_pi(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1011111;
   return get_j_type_ins(cpu_resource, enc.imm, enc.fd, opcode);
}

/*PACKB*/
unum_t get_packb(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0111011;
   std::vector<unum_t> funct7 {0b1000000};
   std::vector<unum_t> funct3 {0b110};
   return get_r_type_ins(cpu_resource, funct7, enc.rs2, enc.rs1, funct3, enc.rd, opcode); 
}

unum_t gen_pi_inst(CpuResource * cpu_resource, unum_t rs1, unum_t rs2, unum_t rd, unum_t fs1, unum_t fs2, unum_t fd, unum_t imm, unum_t md) {
   et_enc_fields fields;

   fields.fs1 = fs1;
   fields.fs2 = fs2;
   fields.fd  = fd;
   fields.rs1 = rs1;
   fields.rs2 = rs2;
   fields.rd  = rd;
   fields.imm = imm;
   fields.md  = md;

   std::vector<ins_generator> ins_types { &get_pi_0, &get_pi_1, &get_pi_2, &get_pi_3, &get_pi_4, &get_pi_5, &get_pi_6,
                                          &get_pi_7, &get_fsatu8_pi, &get_fltm_pi, &get_fsetm_pi, &get_fbci_pi, &get_packb};

   unum_t rand_num = snp_urandrange(cpu_resource, 0, ins_types.size());
   const auto gen_ins = ins_types[rand_num];
   return gen_ins(cpu_resource, fields);
}

unum_t get_pi_rd_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rd_regs {13, 14, 24, 25, 30};
   return get_rand_elem_from_vec(cpu_resource, rd_regs);
}

unum_t get_pi_rsx_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rsx_regs {13, 14, 24};
   return get_rand_elem_from_vec(cpu_resource, rsx_regs);
}

unum_t get_seq_pi_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {

   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;

   unum_t rs1 = get_pi_rsx_reg(cpu_resource);
   unum_t rs2 = get_pi_rsx_reg(cpu_resource);
   unum_t rd  = get_pi_rd_reg(cpu_resource); 

   unum_t fs1 = et_ins_regs->fs1;
   unum_t fs2 = et_ins_regs->fs2;
   unum_t fd  = et_ins_regs->fd;

   unum_t imm = snp_randnum(cpu_resource);
   unum_t md  = snp_urandrange(cpu_resource, 0, 8);

   return gen_pi_inst(cpu_resource, rs1, rs2, rd, fs1, fs2, fd, imm, md);
}
