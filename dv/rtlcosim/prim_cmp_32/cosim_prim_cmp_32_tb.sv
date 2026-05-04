// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: prim_cmp_32 (new) vs r32cmp (original).
//
// Both modules are purely combinational 3:2 compressors. The testbench
// instantiates three width variants (1, 8, 68) and exposes all outputs
// for comparison in C++.

module cosim_prim_cmp_32_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Width=1 stimulus
  input  logic [0:0]  stim_w1_a,
  input  logic [0:0]  stim_w1_b,
  input  logic [0:0]  stim_w1_c,
  output logic [0:0]  new_w1_sv,
  output logic [0:0]  new_w1_cv,
  output logic [0:0]  orig_w1_sv,
  output logic [0:0]  orig_w1_cv,

  // Width=8 stimulus
  input  logic [7:0]  stim_w8_a,
  input  logic [7:0]  stim_w8_b,
  input  logic [7:0]  stim_w8_c,
  output logic [7:0]  new_w8_sv,
  output logic [7:0]  new_w8_cv,
  output logic [7:0]  orig_w8_sv,
  output logic [7:0]  orig_w8_cv,

  // Width=68 stimulus (split into 64+4 for Verilator)
  input  logic [63:0] stim_w68_a_lo,
  input  logic [3:0]  stim_w68_a_hi,
  input  logic [63:0] stim_w68_b_lo,
  input  logic [3:0]  stim_w68_b_hi,
  input  logic [63:0] stim_w68_c_lo,
  input  logic [3:0]  stim_w68_c_hi,
  output logic [63:0] new_w68_sv_lo,
  output logic [3:0]  new_w68_sv_hi,
  output logic [63:0] new_w68_cv_lo,
  output logic [3:0]  new_w68_cv_hi,
  output logic [63:0] orig_w68_sv_lo,
  output logic [3:0]  orig_w68_sv_hi,
  output logic [63:0] orig_w68_cv_lo,
  output logic [3:0]  orig_w68_cv_hi
);

  // ---- Width=1 ----
  prim_cmp_32 #(.Width(1)) u_new_w1 (
    .a  (stim_w1_a),
    .b  (stim_w1_b),
    .c  (stim_w1_c),
    .sv (new_w1_sv),
    .cv (new_w1_cv)
  );

  r32cmp_orig #(.WIDTH(1)) u_orig_w1 (
    .a  (stim_w1_a),
    .b  (stim_w1_b),
    .c  (stim_w1_c),
    .sv (orig_w1_sv),
    .cv (orig_w1_cv)
  );

  // ---- Width=8 ----
  prim_cmp_32 #(.Width(8)) u_new_w8 (
    .a  (stim_w8_a),
    .b  (stim_w8_b),
    .c  (stim_w8_c),
    .sv (new_w8_sv),
    .cv (new_w8_cv)
  );

  r32cmp_orig #(.WIDTH(8)) u_orig_w8 (
    .a  (stim_w8_a),
    .b  (stim_w8_b),
    .c  (stim_w8_c),
    .sv (orig_w8_sv),
    .cv (orig_w8_cv)
  );

  // ---- Width=68 ----
  logic [67:0] w68_a, w68_b, w68_c;
  logic [67:0] new_w68_sv_full, new_w68_cv_full;
  logic [67:0] orig_w68_sv_full, orig_w68_cv_full;

  assign w68_a = {stim_w68_a_hi, stim_w68_a_lo};
  assign w68_b = {stim_w68_b_hi, stim_w68_b_lo};
  assign w68_c = {stim_w68_c_hi, stim_w68_c_lo};

  prim_cmp_32 #(.Width(68)) u_new_w68 (
    .a  (w68_a),
    .b  (w68_b),
    .c  (w68_c),
    .sv (new_w68_sv_full),
    .cv (new_w68_cv_full)
  );

  r32cmp_orig #(.WIDTH(68)) u_orig_w68 (
    .a  (w68_a),
    .b  (w68_b),
    .c  (w68_c),
    .sv (orig_w68_sv_full),
    .cv (orig_w68_cv_full)
  );

  assign {new_w68_sv_hi,  new_w68_sv_lo}  = new_w68_sv_full;
  assign {new_w68_cv_hi,  new_w68_cv_lo}  = new_w68_cv_full;
  assign {orig_w68_sv_hi, orig_w68_sv_lo} = orig_w68_sv_full;
  assign {orig_w68_cv_hi, orig_w68_cv_lo} = orig_w68_cv_full;

endmodule
