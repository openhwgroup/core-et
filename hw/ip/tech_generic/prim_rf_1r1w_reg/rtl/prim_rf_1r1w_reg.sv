// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: latch-timed 1R1W register file with registered read address.
// Generic/default implementation.

module prim_rf_1r1w_reg #(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 32,
  localparam int unsigned AddrW = (Depth > 1) ? $clog2(Depth) : 1
) (
  input  logic              clk_i,
  input  logic [AddrW-1:0]  rd_addr_i,
  input  logic              rd_en_i,
  output logic [Width-1:0]  rd_data_o,
  input  logic [Width-1:0]  wr_data_i,
  input  logic              wr_data_en_1p_i,
  input  logic [AddrW-1:0]  wr_addr_i,
  input  logic              wr_en_i
);

  logic [Width-1:0] rf_q [Depth];
  logic [Width-1:0] wr_data_del_q;
  logic [AddrW-1:0] rd_addr_q;

  always_ff @(negedge clk_i) begin
    if (wr_data_en_1p_i) wr_data_del_q <= wr_data_i;
  end

  always_ff @(posedge clk_i) begin
    if (rd_en_i) rd_addr_q <= rd_addr_i;
    if (wr_en_i) rf_q[wr_addr_i] <= wr_data_del_q;
  end

  assign rd_data_o = rf_q[rd_addr_q];

endmodule : prim_rf_1r1w_reg
