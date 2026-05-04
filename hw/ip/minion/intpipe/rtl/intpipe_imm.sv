// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_imm — Immediate value generator.
//
// Pure combinational. Extracts and sign-extends the immediate field
// from a 32-bit RISC-V instruction based on the immediate format
// selector (I, S, SB, U, UJ, Z).
//
// Line-by-line translation of: intpipe_imm (etcore-soc)

module intpipe_imm
  import minion_pkg::*;
(
  // Instruction and control
  /* verilator lint_off UNUSEDSIGNAL */  // Bits [6:0] (opcode) not used by immediate extraction
  input  logic [InstSize-1:0]  inst_bits,
  /* verilator lint_on UNUSEDSIGNAL */
  input  imm_ctrl_e            sel_imm,
  // Output
  output logic [XregSize-1:0]  imm
);

  logic [10:0] b30_b20; // Bits 30..20 of the immediate
  logic [7:0]  b19_b12; // Bits 19..12 of the immediate
  logic        b11;     // Bit 11 of the immediate
  logic [5:0]  b10_b5;  // Bits 10..5 of the immediate
  logic [3:0]  b4_b1;   // Bits 4..1 of the immediate
  logic        b0;      // Bit 0 of the immediate
  logic        sign;    // Sign of the immediate

  always_comb begin
    sign    = (sel_imm == ImmZ) ? 1'b0
            :                    inst_bits[31];
    b30_b20 = (sel_imm == ImmU) ? inst_bits[30:20]
            :                    {11{sign}};
    b19_b12 = ((sel_imm != ImmU) && (sel_imm != ImmUj)) ? {8{sign}}
            :                                             inst_bits[19:12];
    b11     = ((sel_imm == ImmU) || (sel_imm == ImmZ)) ? 1'b0
            : (sel_imm == ImmUj)                       ? inst_bits[20]
            : (sel_imm == ImmSb)                       ? inst_bits[7]
            :                                            sign;
    b10_b5  = ((sel_imm == ImmU) || (sel_imm == ImmZ)) ? 6'b0
            :                                            inst_bits[30:25];
    b4_b1   = (sel_imm == ImmU)                                  ? 4'b0
            : ((sel_imm == ImmS) || (sel_imm == ImmSb))         ? inst_bits[11:8]
            : (sel_imm == ImmZ)                                  ? inst_bits[19:16]
            :                                                      inst_bits[24:21];
    b0      = (sel_imm == ImmS) ? inst_bits[7]
            : (sel_imm == ImmI) ? inst_bits[20]
            : (sel_imm == ImmZ) ? inst_bits[15]
            :                     1'b0;

    // Concatenates the bits
    imm = { {33{sign}}, b30_b20, b19_b12, b11, b10_b5, b4_b1, b0 };
  end

endmodule
