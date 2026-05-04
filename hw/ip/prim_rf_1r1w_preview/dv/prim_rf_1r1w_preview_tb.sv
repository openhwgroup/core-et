// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_1r1w_preview_tb (
  input  logic       preview_clk_i,
  input  logic       rf_clk_i,
  input  logic       wr_data_en_1p_next_i,
  input  logic       wr_en_i,
  input  logic [1:0] wr_addr_i,
  input  logic [7:0] wr_data_i,
  input  logic [1:0] rd_addr_i,
  output logic [7:0] rd_data_o
);

  prim_rf_1r1w_preview #(
    .Width (8),
    .Depth (4)
  ) dut (
    .preview_clk_i,
    .rf_clk_i,
    .wr_data_en_1p_next_i,
    .wr_en_i,
    .wr_addr_i,
    .wr_data_i,
    .rd_addr_i,
    .rd_data_o
  );

endmodule
