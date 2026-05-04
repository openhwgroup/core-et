// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_rf_3r2w_preview_tb (
  input  logic        preview_clk_i,
  input  logic        rf_clk_i,
  input  logic [2:0]  rd_addr_a_i,
  output logic [15:0] orig_rd_data_a_o,
  output logic [15:0] new_rd_data_a_o,
  input  logic [2:0]  rd_addr_b_i,
  output logic [15:0] orig_rd_data_b_o,
  output logic [15:0] new_rd_data_b_o,
  input  logic [2:0]  rd_addr_c_i,
  output logic [15:0] orig_rd_data_c_o,
  output logic [15:0] new_rd_data_c_o,
  input  logic        wr_en_a_i,
  input  logic [2:0]  wr_addr_a_i,
  input  logic        wr_data_a_en_1p_next_i,
  input  logic [15:0] wr_data_a_i,
  input  logic        wr_en_b_i,
  input  logic [2:0]  wr_addr_b_i,
  input  logic        wr_data_b_en_1p_next_i,
  input  logic [15:0] wr_data_b_i
);

  logic [1:0] orig_wr_data_en_1p;

  rlatchn #(
    .WIDTH (2)
  ) u_orig_preview (
    .clk (preview_clk_i),
    .d   ({wr_data_b_en_1p_next_i, wr_data_a_en_1p_next_i}),
    .q   (orig_wr_data_en_1p)
  );

  rf_latch_3r_2w #(
    .WIDTH           (16),
    .ENTRIES         (8),
    .LEVEL2_CLK_GATE (0)
  ) u_orig_rf (
    .clk             (rf_clk_i),
    .test_en         (1'b0),
    .rd_addr_a       (rd_addr_a_i),
    .rd_data_a       (orig_rd_data_a_o),
    .rd_addr_b       (rd_addr_b_i),
    .rd_data_b       (orig_rd_data_b_o),
    .rd_addr_c       (rd_addr_c_i),
    .rd_data_c       (orig_rd_data_c_o),
    .wr_en_a         (wr_en_a_i),
    .wr_addr_a       (wr_addr_a_i),
    .wr_data_a_en_1p (orig_wr_data_en_1p[0]),
    .wr_data_a       (wr_data_a_i),
    .wr_en_b         (wr_en_b_i),
    .wr_addr_b       (wr_addr_b_i),
    .wr_data_b_en_1p (orig_wr_data_en_1p[1]),
    .wr_data_b       (wr_data_b_i)
  );

  prim_rf_3r2w_preview #(
    .Width   (16),
    .Entries (8)
  ) u_new_rf (
    .preview_clk_i          (preview_clk_i),
    .rf_clk_i               (rf_clk_i),
    .rd_addr_a_i            (rd_addr_a_i),
    .rd_data_a_o            (new_rd_data_a_o),
    .rd_addr_b_i            (rd_addr_b_i),
    .rd_data_b_o            (new_rd_data_b_o),
    .rd_addr_c_i            (rd_addr_c_i),
    .rd_data_c_o            (new_rd_data_c_o),
    .wr_en_a_i              (wr_en_a_i),
    .wr_addr_a_i            (wr_addr_a_i),
    .wr_data_a_en_1p_next_i (wr_data_a_en_1p_next_i),
    .wr_data_a_i            (wr_data_a_i),
    .wr_en_b_i              (wr_en_b_i),
    .wr_addr_b_i            (wr_addr_b_i),
    .wr_data_b_en_1p_next_i (wr_data_b_en_1p_next_i),
    .wr_data_b_i            (wr_data_b_i)
  );

endmodule
