// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: latch-timed 1R1W register file.
// iCE40 implementation using explicit FF-bank storage.

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

  logic [Width-1:0] rf_q [Depth];
  logic [Width-1:0] wr_data_del_q;
  logic [Depth-1:0] wr_sel;

  always_ff @(negedge clk_i) begin
    if (wr_data_en_i) wr_data_del_q <= wr_data_i;
  end

  for (genvar g = 0; g < Depth; g++) begin : gen_rf
    localparam logic [AddrW-1:0] EntryIdx = g;
    assign wr_sel[g] = wr_en_i && (wr_addr_i == EntryIdx);

    always_ff @(posedge clk_i) begin
      if (wr_sel[g]) rf_q[g] <= wr_data_del_q;
    end
  end

  assign rd_data_o = rf_q[rd_addr_i];

endmodule : prim_rf_1r1w
