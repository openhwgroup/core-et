// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module intpipe_decode (
  // Input instruction
  input  logic [`INST_RANGE] inst_bits,
  // Output decode bits
  output minion_control      inst_ctrl
);

  // This is a huge case that decodes all the control bits based on the
  // instruction bits
  logic [$bits(minion_control)-1:0] raw_inst_ctrl; // Raw decoding

  always_comb begin
    //                                        val         jal
    //                                        |  mcode    |  jalr
    //                                        |  |  fp    |  |  renx2                                                                                   rma
    //                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
    //                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
    //                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
    //                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
    //                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
    //                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
    casex(inst_bits)
      // Basic set
      `BNE:          raw_inst_ctrl = { `Y,`N,`N,`Y,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_SB,`DW_X,  `CORE_FUNC_SNE,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `BEQ:          raw_inst_ctrl = { `Y,`N,`N,`Y,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_SB,`DW_X,  `CORE_FUNC_SEQ,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `BLT:          raw_inst_ctrl = { `Y,`N,`N,`Y,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_SB,`DW_X,  `CORE_FUNC_SLT,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `BLTU:         raw_inst_ctrl = { `Y,`N,`N,`Y,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_SB,`DW_X,  `CORE_FUNC_SLTU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `BGE:          raw_inst_ctrl = { `Y,`N,`N,`Y,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_SB,`DW_X,  `CORE_FUNC_SGE,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `BGEU:         raw_inst_ctrl = { `Y,`N,`N,`Y,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_SB,`DW_X,  `CORE_FUNC_SGEU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `JAL:          raw_inst_ctrl = { `Y,`N,`N,`N,`Y,`N,`N,`N,`A2_SIZE,`A1_PC,  `IMM_UJ,`DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `JALR:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`Y,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `AUIPC:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_IMM, `A1_PC,  `IMM_U, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `LB:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_B,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `LH:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_H,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `LW:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `LBU:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_BU,       `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `LHU:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_HU,       `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SB:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_IMM, `A1_RS1, `IMM_S, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XWR,      `MT_B,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `SH:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_IMM, `A1_RS1, `IMM_S, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XWR,      `MT_H,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `SW:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_IMM, `A1_RS1, `IMM_S, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XWR,      `MT_W,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `LUI:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_IMM, `A1_ZERO,`IMM_U, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `ADDI:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SLTI :        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_SLT,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SLTIU:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_SLTU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `ANDI:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_AND,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `ORI:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_OR,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `XORI:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_XOR,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SLLI:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_SL,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRLI:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_SR,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRAI:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_SRA,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `ADD:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SUB:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_SUB,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SLT:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_SLT,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };

//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
      `SLTU:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_SLTU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `AND:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_AND,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `OR:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_OR,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `XOR:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_XOR,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SLL:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_SL,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRL:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_SR,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRA:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_SRA,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `FENCE:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`Y,`N,`N,`N,`N };
      `ECALL:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_I,`N,`N,`N,`N,`N };
      `EBREAK:       raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_ZERO,`A1_PC,  `IMM_X, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_I,`N,`N,`N,`N,`N };
      `MRET:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_I,`N,`N,`N,`N,`N };
      `WFI:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_I,`N,`N,`N,`N,`N };
      `CSRRW:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO,`A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_W,`N,`N,`N,`N,`N };
      `CSRRS:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO,`A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_S,`N,`N,`N,`N,`N };
      `CSRRC:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO,`A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_C,`N,`N,`N,`N,`N };
      `CSRRWI:       raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`N,`A2_IMM, `A1_ZERO,`IMM_Z, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_W,`N,`N,`N,`N,`N };
      `CSRRSI:       raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`N,`A2_IMM, `A1_ZERO,`IMM_Z, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_S,`N,`N,`N,`N,`N };
      `CSRRCI:       raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`N,`A2_IMM, `A1_ZERO,`IMM_Z, `DW_XPR,`CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_C,`N,`N,`N,`N,`N };

      // rets
      `SRET:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_I,`N,`N,`N,`N,`N };
      `DRET:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_I,`N,`N,`N,`N,`N };

      // 64b set
      `LD:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `LWU:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_WU,       `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SD:           raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_IMM, `A1_RS1, `IMM_S, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XWR,      `MT_D,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
      `ADDIW:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_32, `CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SLLIW:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_32, `CORE_FUNC_SL,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRLIW:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_32, `CORE_FUNC_SR,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRAIW:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_32, `CORE_FUNC_SRA,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `ADDW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_ADD,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SUBW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_SUB,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SLLW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_SL,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRLW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_SR,    `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
      `SRAW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_SRA,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };

//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // Mul/Div
        `MUL:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_MUL,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `MULH:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_MULH,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `MULHU:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_MULHU, `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `MULHSU:       raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_MULHSU,`N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `DIV:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_DIV,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `DIVU:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_DIVU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `REM:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_REM,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `REMU:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR,`CORE_FUNC_REMU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };

//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // Mul/Div 64b
        `MULW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_MUL,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `DIVW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_DIV,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `DIVUW:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_DIVU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `REMW:         raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_REM,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `REMUW:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_32, `CORE_FUNC_REMU,  `N,`M_X,        `MT_X,        `N,`N,`N, `N,`Y,`Y,`CSR_N,`N,`N,`N,`N,`N };

//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // Atomics
        `FAMOADDL_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_ADD,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOSWAPL_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_SWAP, `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOANDL_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_AND,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOORL_PI:   raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_OR,   `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOXORL_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_XOR,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOMINL_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOMAXL_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOMINUL_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MINU, `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOMAXUL_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAXU, `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOMAXL_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX_PS,`MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOMINL_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN_PS,`MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FAMOADDG_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_ADD,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOSWAPG_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_SWAP, `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOANDG_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_AND,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOORG_PI:   raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_OR,   `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOXORG_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_XOR,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOMING_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOMAXG_PI:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX,  `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOMINUG_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MINU, `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOMAXUG_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAXU, `MT_PS_GS32,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOMAXG_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX_PS,`MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FAMOMING_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN_PS,`MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `AMOSWAPG_W:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_SWAP,  `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOSWAPL_W:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_SWAP,  `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOADDG_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_ADD,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOADDL_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_ADD,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOXORG_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_XOR,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOXORL_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_XOR,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOANDG_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_AND,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOANDL_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_AND,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOORG_W:     raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_OR,    `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOORL_W:     raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_OR,    `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMING_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMINL_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMAXG_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMAXL_W:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX,   `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMINUG_W:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MINU,  `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMINUL_W:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MINU,  `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMAXUG_W:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAXU,  `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMAXUL_W:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAXU,  `MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOSWAPL_D:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_SWAP,  `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOADDL_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_ADD,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOXORL_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_XOR,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOANDL_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_AND,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOORL_D:     raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_OR,    `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMINL_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMAXL_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMINUL_D:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MINU,  `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOMAXUL_D:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAXU,  `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `AMOSWAPG_D:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_SWAP,  `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOADDG_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_ADD,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOXORG_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_XOR,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOANDG_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_AND,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOORG_D:     raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_OR,    `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMING_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MIN,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMAXG_D:    raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAX,   `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMINUG_D:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MINU,  `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        `AMOMAXUG_D:   raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_MAXU,  `MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`N };
        
        `AMOCMPSWPL_W: raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_CMPSWP,`MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`Y };
        `AMOCMPSWPG_W: raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_CMPSWP,`MT_W,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`Y };
        `AMOCMPSWPL_D: raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_CMPSWP,`MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`Y };
        `AMOCMPSWPG_D: raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XA_CMPSWP,`MT_D,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`Y,`N,`N,`Y };


//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // instructions for CUDA support
        `SBL:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_S, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_B,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FGBL_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS_GS8,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FSCBL_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS_GS8,   `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `SBG:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_S, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_B,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`Y,`N,`N,`N };
        `FGBG_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS_GS8,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FSCBG_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS_GS8,   `N,`N,`N, `N,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `SHL:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_S, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_H,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FGHL_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS_GS16,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FSCHL_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS_GS16,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `SHG:          raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_ZERO, `A1_RS1, `IMM_S, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_H,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`Y,`N,`N,`N };
        `FGHG_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS_GS16,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FSCHG_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS_GS16,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FLWL_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS,       `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSWL_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS,       `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FGWL_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FSCWL_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS_GS32,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FLWG_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS,       `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`N,`N,`N };
        `FSWG_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_ZERO, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS,       `N,`N,`N, `N,`N,`N,`CSR_N,`N,`Y,`N,`N,`N };
        `FGWG_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XRDB,     `MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };
        `FSCWG_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD,  `Y,`M_XWRB,     `MT_PS_GS32,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`Y,`Y,`N,`N };

//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // Float single
        `FSGNJ_S:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSGNJX_S:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSGNJN_S:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMIN_S:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMAX_S:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FADD_S:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSUB_S:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMUL_S:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMADD_S:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMSUB_S:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FNMADD_S:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FNMSUB_S:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCLASS_S:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FMV_X_S:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_W_S:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_WU_S:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FEQ_S:        raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FLT_S:        raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FLE_S:        raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FMV_S_X:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,   `A1_RS1, `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_S_W:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,   `A1_RS1, `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_S_WU:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,   `A1_RS1, `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLW:          raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_WU,       `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSW:          raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_S, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XWR,      `MT_W,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };


//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // Moscow instructions (to be remove  d)
        `FLQ2:         raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_I, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XRD,      `MT_PS,       `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSQ2:         raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_IMM, `A1_RS1, `IMM_S, `DW_XPR,`CORE_FUNC_ADD,   `Y,`M_XWR,      `MT_PS,       `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };


//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // New Esperanto Instructions
        // Packed Single
        `FLW_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_IMM,  `A1_RS1, `IMM_I, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XRD,      `MT_PS,       `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSW_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_IMM,  `A1_RS1, `IMM_S, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XWR,      `MT_PS,       `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FBC_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_IMM,  `A1_RS1, `IMM_I, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XRD,      `MT_PS_BR,    `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FBCX_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,    `A1_RS1, `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FBCI_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FGW_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XRD,      `MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FGH_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XRD,      `MT_PS_GS16,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FGB_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XRD,      `MT_PS_GS8,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FSCW_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XWR,      `MT_PS_GS32,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FSCH_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XWR,      `MT_PS_GS16,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FSCB_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`N,`A2_RS2,  `A1_FS1, `IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_XWR,      `MT_PS_GS8,   `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`Y,`N,`N };
        `FG32W_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`Y,`A2_RS2,  `A1_ZERO,`IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_G32,      `MT_PS_GS32,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FG32H_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`Y,`A2_RS2,  `A1_ZERO,`IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_G32,      `MT_PS_GS16,  `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FG32B_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`Y,`A2_RS2,  `A1_ZERO,`IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_G32,      `MT_PS_GS8,   `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSC32W_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`Y,`A2_RS2,  `A1_ZERO,`IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_SC32,     `MT_PS_GS32,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSC32H_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`Y,`A2_RS2,  `A1_ZERO,`IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_SC32,     `MT_PS_GS16,  `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSC32B_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`Y,`Y,`A2_RS2,  `A1_ZERO,`IMM_X, `DW_XPR, `CORE_FUNC_ADD, `Y,`M_SC32,     `MT_PS_GS8,   `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FADD_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSUB_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMUL_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMIN_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMAX_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
`ifdef ENABLE_EXTRA_TRANS
        `FRSQ_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
`else
        `FRSQ_PS:      raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
`endif
        `FRCP_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMADD_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMSUB_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FNMSUB_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FNMADD_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FEQ_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLT_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLE_PS:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FEQM_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLTM_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLEM_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCLASS_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_PW_PS:   raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_PWU_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_PS_PW:   raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,    `A1_RS1, `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_PS_PWU:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,    `A1_RS1, `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMVZ_X_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FMVS_X_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `FSGNJ_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSGNJX_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSGNJN_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCMOV_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCMOVM_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSWIZZ_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };   
`ifdef ENABLE_EXTRA_TRANS
        `FSIN_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
`else
        `FSIN_PS:      raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
`endif
        `FEXP_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLOG_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,    `A1_X,   `IMM_X, `DW_X,   `CORE_FUNC_X,   `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };

//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        `FFRC_PS:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FROUND_PS:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };   
        `FCVT_PS_F16:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_PS_F11:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_F10:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_UN24: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_UN16: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_UN10: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_UN8:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_UN2:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_SN16: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_PS_SN8:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
      
        `FCVT_F16_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_F11_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_F10_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_UN24_PS: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_UN16_PS: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_UN10_PS: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_UN8_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_UN2_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_SN16_PS: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `FCVT_SN8_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };

 
//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // Mask
        `MAND:         raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `MOR:          raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `MNOT:         raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `MXOR:         raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `MOVA_X_M:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `Y,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `MOVA_M_X:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,   `A1_RS1, `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`Y, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `MOV_M_X:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`Y,`A2_X,   `A1_RS1, `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `MASKPOPC:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `MASKPOPCZ:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `MASKPOPCR:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`Y,`N };
        
//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // Packed Integer
        `FBCI_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FADD_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSUB_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMUL_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMULH_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMULHU_PI:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMIN_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMAX_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMINU_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FMAXU_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FAND_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLTM_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N }; 
        `FSAT8_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSATU8_PI:    raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };         
        `FOR_PI:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FNOT_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FXOR_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSLL_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSLLI_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSRL_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSRA_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSRLI_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSRAI_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FEQ_PI:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLT_PI:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FLTU_PI:      raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };        
        `FLE_PI:       raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSETM_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`Y,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FADDI_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FANDI_PI:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FPACKREPB_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FPACKREPH_PI: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `CUBEF_PS:     raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `CUBEFIDX_PS:  raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `CUBESGNSC_PS: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };
        `CUBESGNTC_PS: raw_inst_ctrl = { `Y,`N,`Y,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `Y,`N,`N,`CSR_N,`N,`N,`N,`Y,`N };

//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
                                                                                                                                                     
        // Support for render targets                                                                                                                
        `PACKB:        raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_PACKB,   `N,`M_X,        `MT_X,     `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`N,`N };
        `BITMIXB:      raw_inst_ctrl = { `Y,`N,`N,`N,`N,`N,`Y,`Y,`A2_RS2, `A1_RS1, `IMM_X, `DW_XPR, `CORE_FUNC_BITMIXB, `N,`M_X,        `MT_X,     `N,`N,`N, `N,`N,`Y,`CSR_N,`N,`N,`N,`Y,`N };


//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        // M-CODE instructions
        `FCVT_L_S:     raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_LU_S:    raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_S_L:     raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FCVT_S_LU:    raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FDIV_PI:      raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FDIVU_PI:     raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FREMU_PI:     raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FREM_PI:      raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FDIV_S:       raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };              
        `FSQRT_S:      raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FSQRT_PS:     raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FDIV_PS:      raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `FENCE_I:      raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
        `SFENCE_VMA:   raw_inst_ctrl = { `Y,`Y,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
//                                        val         jal                                                                                                     
//                                        |  mcode    |  jalr                                                                                                                
//                                        |  |  fp    |  |  renx2                                                                                   rma
//                                        |  |  |     |  |  |  renx1       s_alu1                                     mem_val                       |         wfd             fence    gfx
//                                        |  |  |  br |  |  |  |  s_alu2   |        imm     dw      alu               |  mem_cmd      mem_type      |     wmad|  div          |  mem_g |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  wmd|   |  |  wxd       |  |     |
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  csr    |  |  gsc|
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  X31
//                                        |  |  |  |  |  |  |  |  |        |        |       |       |                 |  |            |             |  |  |   |  |  |  |      |  |  |  |  |
        default:       raw_inst_ctrl = { `N,`N,`N,`N,`N,`N,`N,`N,`A2_X,   `A1_X,   `IMM_X, `DW_X,  `CORE_FUNC_X,     `N,`M_X,        `MT_X,        `N,`N,`N, `N,`N,`N,`CSR_N,`N,`N,`N,`N,`N };
    endcase

    // Assigns from raw bits to struct
    inst_ctrl = minion_control'(raw_inst_ctrl);
  end

endmodule

