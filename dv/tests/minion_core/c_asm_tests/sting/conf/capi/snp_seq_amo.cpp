#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include "snp_seq_amo.h"
#include "snp_seq_enc.h"
#include "snp_sequence.h"

using ins_generator = std::function<unum_t(CpuResource*, const et_enc_fields&)>;

#define AMOADDG_W    0b0000001 
#define AMOADDL_W    0b0000000 
#define AMOSWAPG_W   0b0000101 
#define AMOSWAPL_W   0b0000100 
#define AMOXORG_W    0b0010001 
#define AMOXORL_W    0b0010000 
#define AMOORG_W     0b0100001 
#define AMOORL_W     0b0100000 
#define AMOANDG_W    0b0110001 
#define AMOANDL_W    0b0110000 
#define AMOMING_W    0b1000001 
#define AMOMINL_W    0b1000000 
#define AMOMAXG_W    0b1010001 
#define AMOMAXL_W    0b1010000 
#define AMOMINUG_W   0b1100001 
#define AMOMINUL_W   0b1100000 
#define AMOMAXUG_W   0b1110001 
#define AMOMAXUL_W   0b1110000 
#define AMOCMPSWPG_W 0b1111001 
#define AMOCMPSWPL_W 0b1111000 
unum_t get_amo_two(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0111011;
   std::vector<unum_t> funct7 {AMOADDG_W, AMOADDL_W, AMOSWAPG_W, AMOSWAPL_W, AMOXORG_W, AMOXORL_W, AMOORG_W, AMOORL_W, \
                               AMOANDG_W, AMOANDL_W, AMOMING_W, AMOMINL_W, AMOMAXG_W, AMOMAXL_W, AMOMINUG_W, \
                               AMOMINUL_W, AMOMAXUG_W, AMOMAXUL_W, AMOCMPSWPG_W, AMOCMPSWPL_W};
   std::vector<unum_t> funct3 {0b010};
   return get_r_type_ins(cpu_resource, funct7, enc.rs1, enc.aligned_ba_reg, funct3, enc.rd, opcode);
}

#define AMOADDG_D    0b0000001
#define AMOADDL_D    0b0000000
#define AMOSWAPG_D   0b0000101
#define AMOSWAPL_D   0b0000100
#define AMOXORG_D    0b0010001
#define AMOXORL_D    0b0010000
#define AMOORG_D     0b0100001
#define AMOORL_D     0b0100000
#define AMOANDG_D    0b0110001
#define AMOANDL_D    0b0110000
#define AMOMING_D    0b1000001
#define AMOMINL_D    0b1000000
#define AMOMAXG_D    0b1010001
#define AMOMAXL_D    0b1010000
#define AMOMINUG_D   0b1100001
#define AMOMINUL_D   0b1100000
#define AMOMAXUG_D   0b1110001
#define AMOMAXUL_D   0b1110000
#define AMOCMPSWPG_D 0b1111001
#define AMOCMPSWPL_D 0b1111000
unum_t get_amo_three(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0111011;
   std::vector<unum_t> funct7 {AMOADDG_D, AMOADDL_D, AMOSWAPG_D, AMOSWAPL_D, AMOXORG_D, AMOXORL_D, AMOORG_D,\
                               AMOORL_D, AMOANDG_D, AMOANDL_D, AMOMING_D, AMOMINL_D, AMOMAXG_D, AMOMAXL_D, AMOMINUG_D,\
                               AMOMINUL_D, AMOMAXUG_D, AMOMAXUL_D, AMOCMPSWPG_D, AMOCMPSWPL_D};
   std::vector<unum_t> funct3 {0b011};
   return get_r_type_ins(cpu_resource, funct7, enc.rs1, enc.aligned_ba_reg, funct3, enc.rd, opcode);
}

#define FAMOADDL_PI  0b0000011 
#define FAMOSWAPL_PI 0b0000111 
#define FAMOANDL_PI  0b0001011 
#define FAMOORL_PI   0b0001111 
#define FAMOXORL_PI  0b0010011 
#define FAMOMINL_PI  0b0010111 
#define FAMOMAXL_PI  0b0011011 
#define FAMOMINUL_PI 0b0011111 
#define FAMOMAXUL_PI 0b0100011 
#define FAMOADDG_PI  0b1000011 
#define FAMOSWAPG_PI 0b1000111 
#define FAMOANDG_PI  0b1001011 
#define FAMOORG_PI   0b1001111 
#define FAMOXORG_PI  0b1010011 
#define FAMOMING_PI  0b1010111 
#define FAMOMAXG_PI  0b1011011 
#define FAMOMINUG_PI 0b1011111 
#define FAMOMAXUG_PI 0b1100011 
unum_t get_famo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FAMOADDL_PI, FAMOSWAPL_PI, FAMOANDL_PI, FAMOORL_PI, FAMOXORL_PI, FAMOMINL_PI, FAMOMAXL_PI,\
                               FAMOMINUL_PI, FAMOMAXUL_PI, FAMOADDG_PI, FAMOSWAPG_PI, FAMOANDG_PI, FAMOORG_PI, FAMOXORG_PI,\
                               FAMOMING_PI, FAMOMAXG_PI, FAMOMINUG_PI, FAMOMAXUG_PI};
   std::vector<unum_t> funct3 {0b100};
   return get_r_type_ins(cpu_resource, funct7, enc.aligned_ba_reg, enc.fs1, funct3, enc.fd, opcode);
}

#define FAMOMAXL_PS 0b0010100 
#define FAMOMINL_PS 0b0011000 
#define FAMOMAXG_PS 0b1010100 
#define FAMOMING_PS 0b1011000 
unum_t get_famo_min_max(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FAMOMAXL_PS, FAMOMINL_PS, FAMOMAXG_PS, FAMOMING_PS};
   std::vector<unum_t> funct3 {0b100};
   return get_r_type_ins(cpu_resource, funct7, enc.aligned_ba_reg, enc.fs1, funct3, enc.fd, opcode);
}

#define SBG 0b0001001 
#define SBL 0b0001000 
#define SHG 0b0001101 
#define SHL 0b0001100 
unum_t get_scalar_amo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0111011;
   std::vector<unum_t> funct7 {SBG, SBL, SHG, SHL};
   std::vector<unum_t> funct3 {0b011};
   return get_r_type_ins(cpu_resource, funct7, enc.rs1, enc.aligned_ba_reg, funct3, 0, opcode);
}

#define FGBL_PS 0b1000000
#define FGHL_PS 0b1000100
#define FGWL_PS 0b1001000
unum_t get_local_gather_amo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FGBL_PS, FGHL_PS, FGWL_PS};
   std::vector<unum_t> funct3 {0b111};
   return get_r_type_ins(cpu_resource, funct7, enc.aligned_ba_reg, enc.fs1, funct3, enc.fd, opcode);
}

#define FGBG_PS 0b1000001 
#define FGHG_PS 0b1000101 
#define FGWG_PS 0b1001001 
unum_t get_global_gather_amo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FGBG_PS, FGHG_PS, FGWG_PS};
   std::vector<unum_t> funct3 {0b111};
   return get_r_type_ins(cpu_resource, funct7, enc.aligned_ba_reg, enc.fs1, funct3, enc.fd, opcode);
}

#define FLWL_PS 0b0001000
#define FLWG_PS 0b0001001
unum_t get_local_vect_load_amo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FLWL_PS, FLWG_PS};
   std::vector<unum_t> funct3 {0b111};
   return get_r_type_ins(cpu_resource, funct7, 0, enc.aligned_ba_reg, funct3, enc.fd, opcode);
}

#define FSWG_PS 0b0101001
#define FSWL_PS 0b0101000
unum_t get_global_vect_store_amo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FSWG_PS, FSWL_PS};
   std::vector<unum_t> funct3 {0b111};
   return get_r_type_ins(cpu_resource, funct7, 0, enc.aligned_ba_reg, funct3, enc.fs1, opcode);
}

#define FSCBL_PS 0b1100000
#define FSCHL_PS 0b1100100
#define FSCWL_PS 0b1101000
unum_t get_local_scatter_amo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FSCBL_PS, FSCHL_PS, FSCWL_PS};
   std::vector<unum_t> funct3 {0b111};
   return get_r_type_ins(cpu_resource, funct7, enc.aligned_ba_reg, enc.fs1, funct3, enc.fs2, opcode);
}

#define FSCBG_PS 0b1100001
#define FSCHG_PS 0b1100101
#define FSCWG_PS 0b1101001
unum_t get_global_scatter_amo(CpuResource * cpu_resource, const et_enc_fields& enc) {
   const unum_t opcode = 0b0001011;
   std::vector<unum_t> funct7 {FSCBG_PS, FSCHG_PS, FSCWG_PS};
   std::vector<unum_t> funct3 {0b111};
   return get_r_type_ins(cpu_resource, funct7, enc.aligned_ba_reg, enc.fs1, funct3, enc.fs2, opcode);
}


unum_t gen_amo_inst(CpuResource * cpu_resource, unum_t aligned_ba_reg, unum_t rd, unum_t rs1, unum_t fd, unum_t fs1, unum_t fs2) {

   et_enc_fields fields;
   fields.aligned_ba_reg = aligned_ba_reg;
   fields.rd = rd;
   fields.rs1 = rs1;
   fields.fd = fd;
   fields.fs1 = fs1;
   fields.fs2 = fs2;

   const std::vector<ins_generator> r_type {&get_amo_two, &get_amo_three, &get_famo, &get_famo_min_max, &get_scalar_amo, &get_local_gather_amo,\
                                            &get_global_scatter_amo, &get_global_gather_amo, &get_local_vect_load_amo, &get_local_scatter_amo};
                                            //&get_global_vect_store_amo};
   unum_t rand_num = snp_urandrange(cpu_resource, 0, r_type.size());
   const auto gen_ins = r_type[rand_num];
   return gen_ins(cpu_resource, fields);
}


unum_t get_amo_rd_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rd_regs {25, 30};
   return get_rand_elem_from_vec(cpu_resource, rd_regs);
}

unum_t get_amo_rs1_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> rs1_regs {13, 14, 24, 25, 30};
   return get_rand_elem_from_vec(cpu_resource, rs1_regs);
}

unum_t get_seq_amo_ins(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {

   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;

   // Integer registers
   unum_t aligned_ba_reg = get_et_ins_aligned_ba_reg(cpu_resource, et_ins_conf_ptr);
   unum_t rd = get_amo_rd_reg(cpu_resource);
   unum_t rs1 = get_amo_rs1_reg(cpu_resource);

   // FP registers
   unum_t fd = et_ins_regs->fd;
   unum_t fs1 = et_ins_regs->fs1;
   unum_t fs2 = et_ins_regs->fs2;

   return gen_amo_inst(cpu_resource, aligned_ba_reg, rd, rs1, fd, fs1, fs2);
}
