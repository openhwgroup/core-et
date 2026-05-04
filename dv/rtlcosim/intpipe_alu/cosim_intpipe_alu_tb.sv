// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_alu (new) vs intpipe_alu (original).
//
// Both modules are purely combinational. The testbench drives dw, fn,
// in1, in2 and exposes out and adder_out from both for comparison in C++.

`include "soc.vh"

module cosim_intpipe_alu_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [0:0]  stim_dw,
  input  logic [4:0]  stim_fn,
  input  logic [63:0] stim_in1,
  input  logic [63:0] stim_in2,

  output logic [63:0] new_out_o,
  output logic [63:0] new_adder_out_o,
  output logic [63:0] orig_out_o,
  output logic [63:0] orig_adder_out_o
);

  // -- New module (SystemVerilog, uses minion_pkg) --
  import minion_pkg::*;

  intpipe_alu u_new (
    .dw        (dw_ctrl_e'(stim_dw)),
    .fn        (alu_func_e'(stim_fn)),
    .in1       (stim_in1),
    .in2       (stim_in2),
    .out       (new_out_o),
    .adder_out (new_adder_out_o)
  );

  // -- Original module (Verilog, uses `include "soc.vh") --
  intpipe_alu_orig u_orig (
    .dw        (core_dw_ctrl'(stim_dw)),
    .fn        (core_alu_func'(stim_fn)),
    .in1       (stim_in1),
    .in2       (stim_in2),
    .out       (orig_out_o),
    .adder_out (orig_adder_out_o)
  );

endmodule
