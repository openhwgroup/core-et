// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone co-simulation testbench for prim_hot2bin against the original
// CORE-ET hot2bin helper, which internally uses onehot_mux.

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for cosim_ctrl.h; both DUTs are combinational.
module cosim_prim_hot2bin_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [1:0]  hot2_i,
  output logic [0:0]  new_bin2_o,
  output logic [0:0]  orig_bin2_o,

  input  logic [2:0]  hot3_i,
  output logic [1:0]  new_bin3_o,
  output logic [1:0]  orig_bin3_o,

  input  logic [7:0]  hot8_i,
  output logic [2:0]  new_bin8_o,
  output logic [2:0]  orig_bin8_o,

  input  logic [32:0] hot33_i,
  output logic [5:0]  new_bin33_o,
  output logic [5:0]  orig_bin33_o
);

  prim_hot2bin #(
    .HotW(2)
  ) u_new_hot2 (
    .hot_i(hot2_i),
    .bin_o(new_bin2_o)
  );

  hot2bin #(
    .HOT_W(2)
  ) u_orig_hot2 (
    .hot(hot2_i),
    .bin(orig_bin2_o)
  );

  prim_hot2bin #(
    .HotW(3)
  ) u_new_hot3 (
    .hot_i(hot3_i),
    .bin_o(new_bin3_o)
  );

  hot2bin #(
    .HOT_W(3)
  ) u_orig_hot3 (
    .hot(hot3_i),
    .bin(orig_bin3_o)
  );

  prim_hot2bin #(
    .HotW(8)
  ) u_new_hot8 (
    .hot_i(hot8_i),
    .bin_o(new_bin8_o)
  );

  hot2bin #(
    .HOT_W(8)
  ) u_orig_hot8 (
    .hot(hot8_i),
    .bin(orig_bin8_o)
  );

  prim_hot2bin #(
    .HotW(33)
  ) u_new_hot33 (
    .hot_i(hot33_i),
    .bin_o(new_bin33_o)
  );

  hot2bin #(
    .HOT_W(33)
  ) u_orig_hot33 (
    .hot(hot33_i),
    .bin(orig_bin33_o)
  );

endmodule : cosim_prim_hot2bin_tb
/* verilator lint_on UNUSED */
