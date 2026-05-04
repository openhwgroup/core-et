// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_depth_to_un.
// Adds clk_i/rst_ni for sim_ctrl.h compatibility (module is combinational).

/* verilator lint_off UNUSEDSIGNAL */ // clk_i/rst_ni needed by sim_ctrl.h but unused by combinational DUT
module rbox_depth_to_un_tb #(
  parameter int unsigned UnSz = 24
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  input  logic [31:0]       in_i,
  output logic [UnSz-1:0]   out_o
);

  rbox_depth_to_un #(
    .UnSz (UnSz)
  ) u_dut (
    .in_i  (in_i),
    .out_o (out_o)
  );

endmodule
