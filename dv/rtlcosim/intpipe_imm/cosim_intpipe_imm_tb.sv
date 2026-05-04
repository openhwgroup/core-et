// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_imm (new) vs intpipe_imm (original).
//
// Both modules are purely combinational. The testbench drives inst_bits
// and sel_imm, then exposes the 64-bit immediate from each module for
// bit-by-bit comparison in C++.

`include "soc.vh"

module cosim_intpipe_imm_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [31:0] stim_inst_bits,
  input  logic [2:0]  stim_sel_imm,

  output logic [63:0] new_imm_o,
  output logic [63:0] orig_imm_o
);

  // -- New module (SystemVerilog, uses minion_pkg) --
  import minion_pkg::*;

  intpipe_imm u_new (
    .inst_bits (stim_inst_bits),
    .sel_imm   (imm_ctrl_e'(stim_sel_imm)),
    .imm       (new_imm_o)
  );

  // -- Original module (Verilog, uses `include "soc.vh") --
  intpipe_imm_orig u_orig (
    .inst_bits (stim_inst_bits),
    .sel_imm   (core_imm_ctrl'(stim_sel_imm)),
    .imm       (orig_imm_o)
  );

endmodule
