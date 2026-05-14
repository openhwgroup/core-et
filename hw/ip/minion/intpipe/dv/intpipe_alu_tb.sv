// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for the standard sim_ctrl harness; intpipe_alu is combinational.
module intpipe_alu_tb
  import minion_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        dw_i,
  input  logic [4:0]  fn_i,
  input  logic [63:0] in1_i,
  input  logic [63:0] in2_i,
  output logic [63:0] out_o,
  output logic [63:0] adder_out_o
);
/* verilator lint_on UNUSED */

  intpipe_alu u_dut (
    .dw        (dw_ctrl_e'(dw_i)),
    .fn        (alu_func_e'(fn_i)),
    .in1       (in1_i),
    .in2       (in2_i),
    .out       (out_o),
    .adder_out (adder_out_o)
  );

endmodule
