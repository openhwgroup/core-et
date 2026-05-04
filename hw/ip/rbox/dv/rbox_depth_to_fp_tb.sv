// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_depth_to_fp.
// Exposes internal_depth_t fields as individual ports for C++ access.

/* verilator lint_off UNUSEDSIGNAL */ // clk_i/rst_ni needed by sim_ctrl.h but unused by combinational DUT
module rbox_depth_to_fp_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic        in_infinite,
  input  logic        in_nan,
  input  logic        in_zero,
  input  logic        in_subnorm,
  input  logic [7:0]  in_e,
  input  logic        in_s,
  input  logic [23:0] in_m,
  input  logic [5:0]  in_e_delta,
  input  logic [4:0]  in_m_shift,

  output logic [31:0] out_o
);

  import rbox_pkg::*;

  internal_depth_t in_packed;
  assign in_packed.infinite = in_infinite;
  assign in_packed.nan      = in_nan;
  assign in_packed.zero     = in_zero;
  assign in_packed.subnorm  = in_subnorm;
  assign in_packed.e        = in_e;
  assign in_packed.s        = in_s;
  assign in_packed.m        = in_m;
  assign in_packed.e_delta  = in_e_delta;
  assign in_packed.m_shift  = in_m_shift;

  rbox_depth_to_fp u_dut (
    .in_i  (in_packed),
    .out_o (out_o)
  );

endmodule
