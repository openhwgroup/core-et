// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_rf_1r1w_par_preview_tb (
  input  logic        preview_clk_i,
  input  logic        rf_clk_i,
  input  logic        wr_data_en_1p_next_i,
  output logic [31:0] orig_rd_data_o,
  output logic [31:0] new_rd_data_o,
  input  logic [7:0]  wr_data_i,
  input  logic [1:0]  wr_addr_i,
  input  logic        wr_en_i
);

  logic orig_wr_data_en_1p;

  rlatchn #(
    .WIDTH (1)
  ) u_orig_preview (
    .clk (preview_clk_i),
    .d   (wr_data_en_1p_next_i),
    .q   (orig_wr_data_en_1p)
  );

  rf_latch_1r_1w_par #(
    .WIDTH           (8),
    .ENTRIES         (4),
    .LEVEL2_CLK_GATE (0)
  ) u_orig_rf (
    .rd_data_a       (orig_rd_data_o),
    .wr_data_a       (wr_data_i),
    .wr_data_a_en_1p (orig_wr_data_en_1p),
    .wr_addr_a       (wr_addr_i),
    .wr_en_a         (wr_en_i),
    .clk             (rf_clk_i),
    .test_en         (1'b0)
  );

  prim_rf_1r1w_par_preview #(
    .Width (8),
    .Depth (4)
  ) u_new_rf (
    .preview_clk_i        (preview_clk_i),
    .rf_clk_i             (rf_clk_i),
    .wr_data_en_1p_next_i (wr_data_en_1p_next_i),
    .rd_data_o            (new_rd_data_o),
    .wr_data_i            (wr_data_i),
    .wr_addr_i            (wr_addr_i),
    .wr_en_i              (wr_en_i)
  );

endmodule
