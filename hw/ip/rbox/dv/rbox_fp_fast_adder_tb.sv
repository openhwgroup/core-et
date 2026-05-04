// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_fp_fast_adder.
// Decomposes internal_depth_t struct ports for C++ access.
// Adds clk_i/rst_ni for sim_ctrl.h compatibility (module is combinational).

/* verilator lint_off UNUSEDSIGNAL */ // clk_i/rst_ni needed by sim_ctrl.h but unused by combinational DUT
module rbox_fp_fast_adder_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // A input (decomposed internal_depth_t)
  input  logic        a_infinite,
  input  logic        a_nan,
  input  logic        a_zero,
  input  logic        a_subnorm,
  input  logic [7:0]  a_e,
  input  logic        a_s,
  input  logic [23:0] a_m,
  input  logic [5:0]  a_e_delta,
  input  logic [4:0]  a_m_shift,

  // B input (decomposed internal_depth_t)
  input  logic        b_infinite,
  input  logic        b_nan,
  input  logic        b_zero,
  input  logic        b_subnorm,
  input  logic [7:0]  b_e,
  input  logic        b_s,
  input  logic [23:0] b_m,
  input  logic [5:0]  b_e_delta,
  input  logic [4:0]  b_m_shift,

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

  internal_depth_t a_packed, b_packed, out_packed;

  assign a_packed.infinite = a_infinite;
  assign a_packed.nan      = a_nan;
  assign a_packed.zero     = a_zero;
  assign a_packed.subnorm  = a_subnorm;
  assign a_packed.e        = a_e;
  assign a_packed.s        = a_s;
  assign a_packed.m        = a_m;
  assign a_packed.e_delta  = a_e_delta;
  assign a_packed.m_shift  = a_m_shift;

  assign b_packed.infinite = b_infinite;
  assign b_packed.nan      = b_nan;
  assign b_packed.zero     = b_zero;
  assign b_packed.subnorm  = b_subnorm;
  assign b_packed.e        = b_e;
  assign b_packed.s        = b_s;
  assign b_packed.m        = b_m;
  assign b_packed.e_delta  = b_e_delta;
  assign b_packed.m_shift  = b_m_shift;

  assign out_infinite = out_packed.infinite;
  assign out_nan      = out_packed.nan;
  assign out_zero     = out_packed.zero;
  assign out_subnorm  = out_packed.subnorm;
  assign out_e        = out_packed.e;
  assign out_s        = out_packed.s;
  assign out_m        = out_packed.m;
  assign out_e_delta  = out_packed.e_delta;
  assign out_m_shift  = out_packed.m_shift;

  rbox_fp_fast_adder u_dut (
    .a_i   (a_packed),
    .b_i   (b_packed),
    .out_o (out_packed)
  );

endmodule
