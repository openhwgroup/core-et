// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_3r2w_tb (
  input  logic        clk_i,
  input  logic [5:0]  rd_addr_a_i,
  input  logic [5:0]  rd_addr_b_i,
  input  logic [5:0]  rd_addr_c_i,
  output logic [31:0] rd_data_a_o,
  output logic [31:0] rd_data_b_o,
  output logic [31:0] rd_data_c_o,
  input  logic        wr_en_a_i,
  input  logic [5:0]  wr_addr_a_i,
  input  logic        wr_data_a_en_1p_i,
  input  logic [31:0] wr_data_a_i,
  input  logic        wr_en_b_i,
  input  logic [5:0]  wr_addr_b_i,
  input  logic        wr_data_b_en_1p_i,
  input  logic [31:0] wr_data_b_i
);

  prim_rf_3r2w #(
    .Width(32),
    .Entries(64),
    .Level2CkGate(1)
  ) u_dut (
    .clk_i,
    .rd_addr_a_i,
    .rd_data_a_o,
    .rd_addr_b_i,
    .rd_data_b_o,
    .rd_addr_c_i,
    .rd_data_c_o,
    .wr_en_a_i,
    .wr_addr_a_i,
    .wr_data_a_en_1p_i,
    .wr_data_a_i,
    .wr_en_b_i,
    .wr_addr_b_i,
    .wr_data_b_en_1p_i,
    .wr_data_b_i
  );

endmodule
