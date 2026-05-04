// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module intpipe_imm (
  // Instruction and control
  input  logic [`INST_RANGE] inst_bits,
  input  core_imm_ctrl       sel_imm,
  // Output
  output logic [`XREG_RANGE] imm
);

  logic [10:0] b30_b20; // Bits 30..20 of the immediate
  logic [7:0]  b19_b12; // Bits 19..12 of the immediate
  logic        b11;     // Bit 11 of the immediate
  logic [5:0]  b10_b5;  // Bits 10..5 of the immediate
  logic [3:0]  b4_b1;   // Bits 4..1 of the immediate
  logic        b0;      // Bit 0 of the immediate
  logic        sign;    // Sign of the immediate

  always_comb begin
    sign    = (sel_imm == core_imm_ctrl_Z) ? 1'b0
            :                                inst_bits[31];
    b30_b20 = (sel_imm == core_imm_ctrl_U) ? inst_bits[30:20]
            :                                {11{sign}};
    b19_b12 = ((sel_imm != core_imm_ctrl_U) && (sel_imm != core_imm_ctrl_UJ)) ? {8{sign}}
            :                                                                   inst_bits[19:12];
    b11     = ((sel_imm == core_imm_ctrl_U) || (sel_imm == core_imm_ctrl_Z)) ? 1'b0
            : (sel_imm == core_imm_ctrl_UJ)                                  ? inst_bits[20]
            : (sel_imm == core_imm_ctrl_SB)                                  ? inst_bits[7]
            :                                                                  sign;
    b10_b5  = ((sel_imm == core_imm_ctrl_U) || (sel_imm == core_imm_ctrl_Z)) ? 6'b0
            :                                                                  inst_bits[30:25];
    b4_b1   = (sel_imm == core_imm_ctrl_U)                                    ? 4'b0
            : ((sel_imm == core_imm_ctrl_S) || (sel_imm == core_imm_ctrl_SB)) ? inst_bits[11:8]
            : (sel_imm == core_imm_ctrl_Z)                                    ? inst_bits[19:16]
            :                                                                   inst_bits[24:21];
    b0      = (sel_imm == core_imm_ctrl_S) ? inst_bits[7]
            : (sel_imm == core_imm_ctrl_I) ? inst_bits[20]
            : (sel_imm == core_imm_ctrl_Z) ? inst_bits[15]
            :                                1'b0;

    // Concatenates the bits
    imm = { {33{sign}}, b30_b20, b19_b12, b11, b10_b5, b4_b1, b0 };
  end

endmodule


