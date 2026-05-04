// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: prim_cmp_42 (new) vs r42cmp (original).
//
// Both modules are purely combinational 4:2 compressors. The testbench
// instantiates four width variants (1, 8, 68, 71) and exposes all outputs
// for comparison in C++.

module cosim_prim_cmp_42_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Width=1 stimulus
  input  logic [0:0]  stim_w1_a,
  input  logic [0:0]  stim_w1_b,
  input  logic [0:0]  stim_w1_c,
  input  logic [0:0]  stim_w1_d,
  input  logic        stim_w1_cin,
  output logic [0:0]  new_w1_sv,
  output logic [0:0]  new_w1_cv,
  output logic [0:0]  orig_w1_sv,
  output logic [0:0]  orig_w1_cv,

  // Width=8 stimulus
  input  logic [7:0]  stim_w8_a,
  input  logic [7:0]  stim_w8_b,
  input  logic [7:0]  stim_w8_c,
  input  logic [7:0]  stim_w8_d,
  input  logic        stim_w8_cin,
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
  input  logic [63:0] stim_w68_d_lo,
  input  logic [3:0]  stim_w68_d_hi,
  input  logic        stim_w68_cin,
  output logic [63:0] new_w68_sv_lo,
  output logic [3:0]  new_w68_sv_hi,
  output logic [63:0] new_w68_cv_lo,
  output logic [3:0]  new_w68_cv_hi,
  output logic [63:0] orig_w68_sv_lo,
  output logic [3:0]  orig_w68_sv_hi,
  output logic [63:0] orig_w68_cv_lo,
  output logic [3:0]  orig_w68_cv_hi,

  // Width=71 stimulus (split into 64+7 for Verilator)
  input  logic [63:0] stim_w71_a_lo,
  input  logic [6:0]  stim_w71_a_hi,
  input  logic [63:0] stim_w71_b_lo,
  input  logic [6:0]  stim_w71_b_hi,
  input  logic [63:0] stim_w71_c_lo,
  input  logic [6:0]  stim_w71_c_hi,
  input  logic [63:0] stim_w71_d_lo,
  input  logic [6:0]  stim_w71_d_hi,
  input  logic        stim_w71_cin,
  output logic [63:0] new_w71_sv_lo,
  output logic [6:0]  new_w71_sv_hi,
  output logic [63:0] new_w71_cv_lo,
  output logic [6:0]  new_w71_cv_hi,
  output logic [63:0] orig_w71_sv_lo,
  output logic [6:0]  orig_w71_sv_hi,
  output logic [63:0] orig_w71_cv_lo,
  output logic [6:0]  orig_w71_cv_hi
);

  // ---- Width=1 ----
  prim_cmp_42 #(.Width(1)) u_new_w1 (
    .a   (stim_w1_a),
    .b   (stim_w1_b),
    .c   (stim_w1_c),
    .d   (stim_w1_d),
    .cin (stim_w1_cin),
    .sv  (new_w1_sv),
    .cv  (new_w1_cv)
  );

  r42cmp_orig #(.WIDTH(1)) u_orig_w1 (
    .a   (stim_w1_a),
    .b   (stim_w1_b),
    .c   (stim_w1_c),
    .d   (stim_w1_d),
    .cin (stim_w1_cin),
    .sv  (orig_w1_sv),
    .cv  (orig_w1_cv)
  );

  // ---- Width=8 ----
  prim_cmp_42 #(.Width(8)) u_new_w8 (
    .a   (stim_w8_a),
    .b   (stim_w8_b),
    .c   (stim_w8_c),
    .d   (stim_w8_d),
    .cin (stim_w8_cin),
    .sv  (new_w8_sv),
    .cv  (new_w8_cv)
  );

  r42cmp_orig #(.WIDTH(8)) u_orig_w8 (
    .a   (stim_w8_a),
    .b   (stim_w8_b),
    .c   (stim_w8_c),
    .d   (stim_w8_d),
    .cin (stim_w8_cin),
    .sv  (orig_w8_sv),
    .cv  (orig_w8_cv)
  );

  // ---- Width=68 ----
  logic [67:0] w68_a, w68_b, w68_c, w68_d;
  logic [67:0] new_w68_sv_full, new_w68_cv_full;
  logic [67:0] orig_w68_sv_full, orig_w68_cv_full;

  assign w68_a = {stim_w68_a_hi, stim_w68_a_lo};
  assign w68_b = {stim_w68_b_hi, stim_w68_b_lo};
  assign w68_c = {stim_w68_c_hi, stim_w68_c_lo};
  assign w68_d = {stim_w68_d_hi, stim_w68_d_lo};

  prim_cmp_42 #(.Width(68)) u_new_w68 (
    .a   (w68_a),
    .b   (w68_b),
    .c   (w68_c),
    .d   (w68_d),
    .cin (stim_w68_cin),
    .sv  (new_w68_sv_full),
    .cv  (new_w68_cv_full)
  );

  r42cmp_orig #(.WIDTH(68)) u_orig_w68 (
    .a   (w68_a),
    .b   (w68_b),
    .c   (w68_c),
    .d   (w68_d),
    .cin (stim_w68_cin),
    .sv  (orig_w68_sv_full),
    .cv  (orig_w68_cv_full)
  );

  assign {new_w68_sv_hi,  new_w68_sv_lo}  = new_w68_sv_full;
  assign {new_w68_cv_hi,  new_w68_cv_lo}  = new_w68_cv_full;
  assign {orig_w68_sv_hi, orig_w68_sv_lo} = orig_w68_sv_full;
  assign {orig_w68_cv_hi, orig_w68_cv_lo} = orig_w68_cv_full;

  // ---- Width=71 ----
  logic [70:0] w71_a, w71_b, w71_c, w71_d;
  logic [70:0] new_w71_sv_full, new_w71_cv_full;
  logic [70:0] orig_w71_sv_full, orig_w71_cv_full;

  assign w71_a = {stim_w71_a_hi, stim_w71_a_lo};
  assign w71_b = {stim_w71_b_hi, stim_w71_b_lo};
  assign w71_c = {stim_w71_c_hi, stim_w71_c_lo};
  assign w71_d = {stim_w71_d_hi, stim_w71_d_lo};

  prim_cmp_42 #(.Width(71)) u_new_w71 (
    .a   (w71_a),
    .b   (w71_b),
    .c   (w71_c),
    .d   (w71_d),
    .cin (stim_w71_cin),
    .sv  (new_w71_sv_full),
    .cv  (new_w71_cv_full)
  );

  r42cmp_orig #(.WIDTH(71)) u_orig_w71 (
    .a   (w71_a),
    .b   (w71_b),
    .c   (w71_c),
    .d   (w71_d),
    .cin (stim_w71_cin),
    .sv  (orig_w71_sv_full),
    .cv  (orig_w71_cv_full)
  );

  assign {new_w71_sv_hi,  new_w71_sv_lo}  = new_w71_sv_full;
  assign {new_w71_cv_hi,  new_w71_cv_lo}  = new_w71_cv_full;
  assign {orig_w71_sv_hi, orig_w71_sv_lo} = orig_w71_sv_full;
  assign {orig_w71_cv_hi, orig_w71_cv_lo} = orig_w71_cv_full;

endmodule
