// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_sample_to_fp32.
// Module has clk_i but needs rst_ni for sim_ctrl.h.

/* verilator lint_off UNUSEDSIGNAL */ // rst_ni needed by sim_ctrl.h but unused by DUT
module rbox_sample_to_fp32_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [42:0] in_i,       // EdgeSampBits = 43
  input  logic        valid_i,
  output logic [31:0] out_o
);

  import rbox_pkg::*;

  rbox_sample_to_fp32 u_dut (
    .clk_i   (clk_i),
    .in_i    (in_i),
    .valid_i (valid_i),
    .out_o   (out_o)
  );

endmodule
