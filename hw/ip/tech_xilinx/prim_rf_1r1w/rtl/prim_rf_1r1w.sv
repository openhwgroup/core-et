// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: latch-timed 1R1W register file.
// Xilinx implementation using LUTRAM-friendly storage.

module prim_rf_1r1w #(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 32,
  localparam int unsigned AddrW = $clog2(Depth)
) (
  input  logic              clk_i,
  input  logic              wr_data_en_i,
  input  logic              wr_en_i,
  input  logic [AddrW-1:0]  wr_addr_i,
  input  logic [Width-1:0]  wr_data_i,
  input  logic [AddrW-1:0]  rd_addr_i,
  output logic [Width-1:0]  rd_data_o
);

  (* ram_style = "distributed" *) logic [Width-1:0] rf_q [Depth];
  logic [Width-1:0] wr_data_del_q;

  always_ff @(negedge clk_i) begin
    if (wr_data_en_i) wr_data_del_q <= wr_data_i;
  end

  always_ff @(posedge clk_i) begin
    if (wr_en_i) rf_q[wr_addr_i] <= wr_data_del_q;
  end

  assign rd_data_o = rf_q[rd_addr_i];

endmodule : prim_rf_1r1w
