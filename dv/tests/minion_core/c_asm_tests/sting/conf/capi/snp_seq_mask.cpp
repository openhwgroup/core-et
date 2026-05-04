
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include "snp_seq_mask.h"
#include "snp_seq_enc.h"
#include "snp_sequence.h"

using ins_generator = std::function<unum_t(CpuResource*, const et_enc_fields&)>;

#define MASKAND 0b111
#define MASKOR  0b110
unum_t get_maskandor(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0110011};
   std::vector<unum_t> funct3 {MASKAND, MASKOR};
   return get_r_type_ins(cpu_resource, funct7, enc.ms2 & 0x7, enc.ms1 & 0x7, funct3, enc.md & 0x7, opcode);
}
#define MASKPOPC  0b0101001 
#define MASKPOPCZ 0b0101010 
unum_t get_maskpop(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {MASKPOPC, MASKPOPCZ};
   std::vector<unum_t> funct3 {0b000};
   return get_r_type_ins(cpu_resource, funct7,             0, enc.ms1 & 0x7, funct3,       enc.rd, opcode);
}
#define MASKNOT 0b010
unum_t get_masknot(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0110011};
   std::vector<unum_t> funct3 {MASKNOT};
   return get_r_type_ins(cpu_resource, funct7,             0, enc.ms1 & 0x7, funct3, enc.md & 0x7, opcode);
}
#define MASKXOR 0b100
unum_t get_maskxor(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b0110011};
   std::vector<unum_t> funct3 {MASKXOR};
   return get_r_type_ins(cpu_resource, funct7, enc.ms2 & 0x7, enc.ms1 & 0x7, funct3, enc.md & 0x7, opcode);
}
#define MOV_M_X 0b0101011
unum_t get_maskmov_m_x(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   unum_t imm_h = snp_get_bits(cpu_resource, enc.imm, 7, 3);
   unum_t imm_l = snp_get_bits(cpu_resource, enc.imm, 2, 0);
   std::vector<unum_t> funct7 {MOV_M_X};
   std::vector<unum_t> funct3 {imm_l};
   return get_r_type_ins(cpu_resource, funct7,         imm_h,       enc.rs1, funct3, enc.md & 0x7, opcode);
}
#define MOVA_M_X 0b001
unum_t get_maskmova_m_x(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b1101011};
   std::vector<unum_t> funct3 {MOVA_M_X};
   return get_r_type_ins(cpu_resource, funct7,             0,       enc.rs1, funct3,            0, opcode);
}
#define MOVA_X_M 0b000
unum_t get_maskmova_x_m(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b1111011;
   std::vector<unum_t> funct7 {0b1101011};
   std::vector<unum_t> funct3 {MOVA_X_M};
   return get_r_type_ins(cpu_resource, funct7,             0,             0, funct3,       enc.rd, opcode);
}

unum_t gen_mask_inst(CpuResource * cpu_resource, unum_t md, unum_t ms1, unum_t ms2, unum_t rd, unum_t rs1, unum_t imm) {
   et_enc_fields fields;
   fields.md = md;
   fields.ms1 = ms1;
   fields.ms2 = ms2;
   fields.rd = rd;
   fields.rs1 = rs1;
   fields.imm = imm;

   const std::vector<ins_generator> ins_types { &get_maskandor, &get_maskpop, &get_masknot, &get_maskxor, \
                                                &get_maskmov_m_x, &get_maskmova_m_x, &get_maskmova_x_m};

   unum_t rand_num = snp_urandrange(cpu_resource, 0, ins_types.size());
   const auto gen_ins = ins_types[rand_num];
   return gen_ins(cpu_resource, fields);
}

unum_t get_mask_rd_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rd_regs {13, 14, 24, 25, 30};
   return get_rand_elem_from_vec(cpu_resource, rd_regs);
}

unum_t get_mask_rs1_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rs1_regs {13, 14, 24};
   return get_rand_elem_from_vec(cpu_resource, rs1_regs);
}

unum_t get_seq_mask_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {

   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;
   unum_t md  = snp_urandrange(cpu_resource, 0, 8);
   unum_t ms1 = snp_urandrange(cpu_resource, 0, 8);
   unum_t ms2 = snp_urandrange(cpu_resource, 0, 8);
   unum_t rd  = get_mask_rd_reg(cpu_resource); 
   unum_t rs1 = get_mask_rs1_reg(cpu_resource);
   unum_t imm = snp_randnum(cpu_resource);

   return gen_mask_inst(cpu_resource, md, ms1, ms2, rd, rs1, imm);
}
