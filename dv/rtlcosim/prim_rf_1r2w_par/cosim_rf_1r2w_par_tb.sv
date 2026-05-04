// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_rf_1r2w_par_tb #(
  parameter int unsigned WIDTH = 32,
  parameter int unsigned DEPTH = 8
) (
  input  logic                     clk_i,
  input  logic [WIDTH-1:0]         wr_data_a_i,
  input  logic                     wr_data_a_en_1p_i,
  input  logic [$clog2(DEPTH)-1:0] wr_addr_a_i,
  input  logic                     wr_en_a_i,
  input  logic [WIDTH-1:0]         wr_data_b_i,
  input  logic                     wr_data_b_en_1p_i,
  input  logic [$clog2(DEPTH)-1:0] wr_addr_b_i,
  input  logic                     wr_en_b_i,
  output logic [WIDTH*DEPTH-1:0]   new_rd_data_o,
  output logic [WIDTH*DEPTH-1:0]   orig_rd_data_o
);

  prim_rf_1r2w_par #(
    .Width (WIDTH),
    .Depth (DEPTH)
  ) u_new (
    .clk_i,
    .rd_data_o         (new_rd_data_o),
    .wr_data_a_i,
    .wr_data_a_en_1p_i,
    .wr_addr_a_i,
    .wr_en_a_i,
    .wr_data_b_i,
    .wr_data_b_en_1p_i,
    .wr_addr_b_i,
    .wr_en_b_i
  );

  rf_latch_1r_2w_par #(
    .WIDTH   (WIDTH),
    .ENTRIES (DEPTH)
  ) u_orig (
    .clk             (clk_i),
    .test_en         (1'b0),
    .rd_data_a       (orig_rd_data_o),
    .wr_addr_a       (wr_addr_a_i),
    .wr_data_a       (wr_data_a_i),
    .wr_en_a         (wr_en_a_i),
    .wr_data_a_en_1p (wr_data_a_en_1p_i),
    .wr_addr_b       (wr_addr_b_i),
    .wr_data_b       (wr_data_b_i),
    .wr_en_b         (wr_en_b_i),
    .wr_data_b_en_1p (wr_data_b_en_1p_i)
  );

endmodule
