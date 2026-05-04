// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: prim_rf_3r2w vs rf_latch_3r_2w.

module cosim_rf_3r2w_tb (
  input  logic        clk_i,
  input  logic [5:0]  rd_addr_a_i,
  input  logic [5:0]  rd_addr_b_i,
  input  logic [5:0]  rd_addr_c_i,
  input  logic        wr_en_a_i,
  input  logic [5:0]  wr_addr_a_i,
  input  logic        wr_data_a_en_1p_i,
  input  logic [31:0] wr_data_a_i,
  input  logic        wr_en_b_i,
  input  logic [5:0]  wr_addr_b_i,
  input  logic        wr_data_b_en_1p_i,
  input  logic [31:0] wr_data_b_i,
  output logic [31:0] new_rd_data_a_o,
  output logic [31:0] new_rd_data_b_o,
  output logic [31:0] new_rd_data_c_o,
  output logic [31:0] orig_rd_data_a_o,
  output logic [31:0] orig_rd_data_b_o,
  output logic [31:0] orig_rd_data_c_o
);

  localparam int unsigned Width = 32;
  localparam int unsigned Entries = 64;
  localparam int unsigned Level2 = 1;

  prim_rf_3r2w #(
    .Width(Width),
    .Entries(Entries),
    .Level2CkGate(Level2)
  ) u_new (
    .clk_i,
    .rd_addr_a_i,
    .rd_data_a_o(new_rd_data_a_o),
    .rd_addr_b_i,
    .rd_data_b_o(new_rd_data_b_o),
    .rd_addr_c_i,
    .rd_data_c_o(new_rd_data_c_o),
    .wr_en_a_i,
    .wr_addr_a_i,
    .wr_data_a_en_1p_i,
    .wr_data_a_i,
    .wr_en_b_i,
    .wr_addr_b_i,
    .wr_data_b_en_1p_i,
    .wr_data_b_i
  );

  rf_latch_3r_2w #(
    .WIDTH(Width),
    .ENTRIES(Entries),
    .LEVEL2_CLK_GATE(Level2)
  ) u_orig (
    .clk(clk_i),
    .test_en(1'b0),
    .rd_addr_a(rd_addr_a_i),
    .rd_data_a(orig_rd_data_a_o),
    .rd_addr_b(rd_addr_b_i),
    .rd_data_b(orig_rd_data_b_o),
    .rd_addr_c(rd_addr_c_i),
    .rd_data_c(orig_rd_data_c_o),
    .wr_en_a(wr_en_a_i),
    .wr_addr_a(wr_addr_a_i),
    .wr_data_a_en_1p(wr_data_a_en_1p_i),
    .wr_data_a(wr_data_a_i),
    .wr_en_b(wr_en_b_i),
    .wr_addr_b(wr_addr_b_i),
    .wr_data_b_en_1p(wr_data_b_en_1p_i),
    .wr_data_b(wr_data_b_i)
  );

endmodule
