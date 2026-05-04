// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original rf_latch_1r_1w vs new prim_rf_1r1w.

module cosim_rf_1r1w_tb #(
  parameter int unsigned WIDTH   = 32,
  parameter int unsigned DEPTH   = 32
) (
  input  logic                        clk_i,
  input  logic                        wr_data_en_i,
  input  logic                        wr_en_i,
  input  logic [$clog2(DEPTH)-1:0]    wr_addr_i,
  input  logic [WIDTH-1:0]            wr_data_i,
  input  logic [$clog2(DEPTH)-1:0]    rd_addr_i,
  output logic [WIDTH-1:0]            new_rd_data_o,
  output logic [WIDTH-1:0]            orig_rd_data_o
);

  prim_rf_1r1w #(.Width(WIDTH), .Depth(DEPTH)) u_new (
    .clk_i,
    .wr_data_en_i,
    .wr_en_i,
    .wr_addr_i,
    .wr_data_i,
    .rd_addr_i,
    .rd_data_o (new_rd_data_o)
  );

  rf_latch_1r_1w #(.WIDTH(WIDTH), .ENTRIES(DEPTH)) u_orig (
    .clk              (clk_i),
    .wr_data_a_en_1p  (wr_data_en_i),
    .wr_en_a          (wr_en_i),
    .wr_addr_a        (wr_addr_i),
    .wr_data_a        (wr_data_i),
    .rd_addr_a        (rd_addr_i),
    .rd_data_a        (orig_rd_data_o),
    .test_en          (1'b0)
  );

endmodule
