// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for sim_ctrl.h; prim_hot2bin is combinational.
module prim_hot2bin_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [1:0]  hot2_i,
  output logic [0:0]  bin2_o,

  input  logic [2:0]  hot3_i,
  output logic [1:0]  bin3_o,

  input  logic [7:0]  hot8_i,
  output logic [2:0]  bin8_o,

  input  logic [32:0] hot33_i,
  output logic [5:0]  bin33_o
);

  prim_hot2bin #(
    .HotW(2)
  ) u_hot2 (
    .hot_i(hot2_i),
    .bin_o(bin2_o)
  );

  prim_hot2bin #(
    .HotW(3)
  ) u_hot3 (
    .hot_i(hot3_i),
    .bin_o(bin3_o)
  );

  prim_hot2bin #(
    .HotW(8)
  ) u_hot8 (
    .hot_i(hot8_i),
    .bin_o(bin8_o)
  );

  prim_hot2bin #(
    .HotW(33)
  ) u_hot33 (
    .hot_i(hot33_i),
    .bin_o(bin33_o)
  );

endmodule : prim_hot2bin_tb
/* verilator lint_on UNUSED */
