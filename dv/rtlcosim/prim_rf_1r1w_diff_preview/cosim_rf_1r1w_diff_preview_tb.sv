// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_rf_1r1w_diff_preview_tb (
  input  logic       preview_clk_i,
  input  logic       rf_clk_i,
  input  logic [1:0] wr_data_en_1p_next_i,
  output logic [7:0] orig_rd_data_o,
  output logic [7:0] new_rd_data_o,
  input  logic [1:0] rd_addr_i,
  input  logic [15:0] wr_data_i,
  input  logic       wr_addr_i,
  input  logic [1:0] wr_en_i,

  input  logic [7:0]   wide_wr_data_en_1p_next_i,
  output logic [31:0]  wide_orig_rd_data_o,
  output logic [31:0]  wide_new_rd_data_o,
  input  logic [4:0]   wide_rd_addr_i,
  input  logic [255:0] wide_wr_data_i,
  input  logic         wide_wr_addr_i,
  input  logic [7:0]   wide_wr_en_i
);

  logic [1:0] orig_wr_data_en_1p;
  logic [7:0] wide_orig_wr_data_en_1p;

  rlatchn #(
    .WIDTH (2)
  ) u_orig_preview (
    .clk (preview_clk_i),
    .d   (wr_data_en_1p_next_i),
    .q   (orig_wr_data_en_1p)
  );

  rf_latch_1r_1w_diff_widths #(
    .R_WIDTH         (8),
    .R_ALIGNMENT     (8),
    .W_WIDTH         (16),
    .ENTRIES         (2),
    .LEVEL2_CLK_GATE (0)
  ) u_orig_rf (
    .clk            (rf_clk_i),
    .test_en        (1'b0),
    .rd_data_a      (orig_rd_data_o),
    .rd_addr_a      (rd_addr_i),
    .wr_data_a      (wr_data_i),
    .wr_data_a_en_1p(orig_wr_data_en_1p),
    .wr_addr_a      (wr_addr_i),
    .wr_en_a        (wr_en_i)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (8),
    .RAlignment (8),
    .WWidth     (16),
    .Entries    (2)
  ) u_new_rf (
    .preview_clk_i       (preview_clk_i),
    .rf_clk_i            (rf_clk_i),
    .wr_data_en_1p_next_i(wr_data_en_1p_next_i),
    .rd_data_o           (new_rd_data_o),
    .rd_addr_i           (rd_addr_i),
    .wr_data_i           (wr_data_i),
    .wr_addr_i           (wr_addr_i),
    .wr_en_i             (wr_en_i)
  );

  rlatchn #(
    .WIDTH (8)
  ) u_wide_orig_preview (
    .clk (preview_clk_i),
    .d   (wide_wr_data_en_1p_next_i),
    .q   (wide_orig_wr_data_en_1p)
  );

  rf_latch_1r_1w_diff_widths #(
    .R_WIDTH         (32),
    .R_ALIGNMENT     (16),
    .W_WIDTH         (256),
    .ENTRIES         (2),
    .LEVEL2_CLK_GATE (0)
  ) u_wide_orig_rf (
    .clk            (rf_clk_i),
    .test_en        (1'b0),
    .rd_data_a      (wide_orig_rd_data_o),
    .rd_addr_a      (wide_rd_addr_i),
    .wr_data_a      (wide_wr_data_i),
    .wr_data_a_en_1p(wide_orig_wr_data_en_1p),
    .wr_addr_a      (wide_wr_addr_i),
    .wr_en_a        (wide_wr_en_i)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (32),
    .RAlignment (16),
    .WWidth     (256),
    .Entries    (2)
  ) u_wide_new_rf (
    .preview_clk_i       (preview_clk_i),
    .rf_clk_i            (rf_clk_i),
    .wr_data_en_1p_next_i(wide_wr_data_en_1p_next_i),
    .rd_data_o           (wide_new_rd_data_o),
    .rd_addr_i           (wide_rd_addr_i),
    .wr_data_i           (wide_wr_data_i),
    .wr_addr_i           (wide_wr_addr_i),
    .wr_en_i             (wide_wr_en_i)
  );

endmodule
