// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_2r1w_preview_tb (
  input  logic       preview_clk_i,
  input  logic       rf_clk_i,
  input  logic [2:0] rd_addr_a_i,
  output logic [15:0] rd_data_a_o,
  input  logic [2:0] rd_addr_b_i,
  output logic [15:0] rd_data_b_o,
  output logic [15:0] rd_par_o,
  input  logic       wr_en_i,
  input  logic       wr_data_en_1p_next_i,
  input  logic [2:0] wr_addr_i,
  input  logic [15:0] wr_data_i
);

  prim_rf_2r1w_preview #(
    .Width     (16),
    .Entries   (8),
    .Zero      (8'b0000_0001),
    .Parallel  (8'b1000_0000),
    .ParallelW (16)
  ) dut (
    .preview_clk_i,
    .rf_clk_i,
    .rd_addr_a_i,
    .rd_data_a_o,
    .rd_addr_b_i,
    .rd_data_b_o,
    .rd_par_o,
    .wr_en_i,
    .wr_data_en_1p_next_i,
    .wr_addr_i,
    .wr_data_i
  );

endmodule
