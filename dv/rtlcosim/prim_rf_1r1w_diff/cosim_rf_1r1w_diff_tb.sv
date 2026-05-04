// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: prim_rf_1r1w_diff vs rf_latch_1r_1w_diff_widths.

`include "soc.vh"

module cosim_rf_1r1w_diff_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus
  input  logic [4:0]  rd_addr_i,
  input  logic [255:0] wr_data_i,
  input  logic [7:0]  wr_data_en_1p_i,
  input  logic [0:0]  wr_addr_i,
  input  logic [7:0]  wr_en_i,

  // Outputs
  output logic [31:0] new_rd_data_o,
  output logic [31:0] orig_rd_data_o
);

  // ── New module ──
  prim_rf_1r1w_diff #(
    .RWidth     (32),
    .RAlignment (16),
    .WWidth     (256),
    .Entries    (2)
  ) u_new (
    .clk_i,
    .rd_data_o      (new_rd_data_o),
    .rd_addr_i      (rd_addr_i),
    .wr_data_i      (wr_data_i),
    .wr_data_en_1p_i(wr_data_en_1p_i),
    .wr_addr_i      (wr_addr_i),
    .wr_en_i        (wr_en_i)
  );

  // ── Original module ──
  rf_latch_1r_1w_diff_widths #(
    .R_WIDTH    (32),
    .R_ALIGNMENT(16),
    .W_WIDTH    (256),
    .ENTRIES    (2),
    .LEVEL2_CLK_GATE(1)
  ) u_orig (
    .clk            (clk_i),
    .test_en        (1'b0),
    .rd_data_a      (orig_rd_data_o),
    .rd_addr_a      (rd_addr_i),
    .wr_data_a      (wr_data_i),
    .wr_data_a_en_1p(wr_data_en_1p_i),
    .wr_addr_a      (wr_addr_i),
    .wr_en_a        (wr_en_i)
  );

endmodule
