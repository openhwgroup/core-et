// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_depth_offset.
// Decomposes internal_depth_t output for C++ access.
// Module has clk_i; adds rst_ni for sim_ctrl.h.

/* verilator lint_off UNUSEDSIGNAL */ // rst_ni needed by sim_ctrl.h but unused by DUT
module rbox_depth_offset_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [31:0] depth_coef_a_i,
  input  logic [31:0] depth_coef_b_i,
  input  logic [5:0]  step_x_i,
  input  logic [5:0]  step_y_i,
  input  logic        valid_stg1_i,
  input  logic        valid_stg2_i,

  // Output (decomposed internal_depth_t)
  output logic        out_infinite,
  output logic        out_nan,
  output logic        out_zero,
  output logic        out_subnorm,
  output logic [7:0]  out_e,
  output logic        out_s,
  output logic [23:0] out_m,
  output logic [5:0]  out_e_delta,
  output logic [4:0]  out_m_shift
);

  import rbox_pkg::*;

  internal_depth_t out_packed;

  assign out_infinite = out_packed.infinite;
  assign out_nan      = out_packed.nan;
  assign out_zero     = out_packed.zero;
  assign out_subnorm  = out_packed.subnorm;
  assign out_e        = out_packed.e;
  assign out_s        = out_packed.s;
  assign out_m        = out_packed.m;
  assign out_e_delta  = out_packed.e_delta;
  assign out_m_shift  = out_packed.m_shift;

  rbox_depth_offset u_dut (
    .clk_i          (clk_i),
    .depth_coef_a_i (depth_coef_a_i),
    .depth_coef_b_i (depth_coef_b_i),
    .step_x_i       (step_x_i),
    .step_y_i       (step_y_i),
    .valid_stg1_i   (valid_stg1_i),
    .valid_stg2_i   (valid_stg2_i),
    .depth_offset_o (out_packed)
  );

endmodule
