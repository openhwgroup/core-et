// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_3r2w_preview_tb (
  input  logic       preview_clk_i,
  input  logic       rf_clk_i,
  input  logic [2:0] rd_addr_a_i,
  output logic [15:0] rd_data_a_o,
  input  logic [2:0] rd_addr_b_i,
  output logic [15:0] rd_data_b_o,
  input  logic [2:0] rd_addr_c_i,
  output logic [15:0] rd_data_c_o,
  input  logic       wr_en_a_i,
  input  logic [2:0] wr_addr_a_i,
  input  logic       wr_data_a_en_1p_next_i,
  input  logic [15:0] wr_data_a_i,
  input  logic       wr_en_b_i,
  input  logic [2:0] wr_addr_b_i,
  input  logic       wr_data_b_en_1p_next_i,
  input  logic [15:0] wr_data_b_i
);

  prim_rf_3r2w_preview #(
    .Width   (16),
    .Entries (8)
  ) dut (
    .preview_clk_i,
    .rf_clk_i,
    .rd_addr_a_i,
    .rd_data_a_o,
    .rd_addr_b_i,
    .rd_data_b_o,
    .rd_addr_c_i,
    .rd_data_c_o,
    .wr_en_a_i,
    .wr_addr_a_i,
    .wr_data_a_en_1p_next_i,
    .wr_data_a_i,
    .wr_en_b_i,
    .wr_addr_b_i,
    .wr_data_b_en_1p_next_i,
    .wr_data_b_i
  );

endmodule
