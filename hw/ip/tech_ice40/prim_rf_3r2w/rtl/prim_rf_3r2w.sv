// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: latch-timed 3R2W register file.
// iCE40 implementation using explicit FF-bank storage.

module prim_rf_3r2w #(
  parameter int unsigned Width        = 32,
  parameter int unsigned Entries      = 32,
  /* verilator lint_off UNUSEDPARAM */  // retained for ASIC compatibility, unused by the iCE40 body
  parameter int unsigned Level2CkGate = 0,
  /* verilator lint_on UNUSEDPARAM */
  localparam int unsigned AddrW       = $clog2(Entries)
) (
  input  logic              clk_i,
  input  logic [AddrW-1:0]  rd_addr_a_i,
  output logic [Width-1:0]  rd_data_a_o,
  input  logic [AddrW-1:0]  rd_addr_b_i,
  output logic [Width-1:0]  rd_data_b_o,
  input  logic [AddrW-1:0]  rd_addr_c_i,
  output logic [Width-1:0]  rd_data_c_o,
  input  logic              wr_en_a_i,
  input  logic [AddrW-1:0]  wr_addr_a_i,
  input  logic              wr_data_a_en_1p_i,
  input  logic [Width-1:0]  wr_data_a_i,
  input  logic              wr_en_b_i,
  input  logic [AddrW-1:0]  wr_addr_b_i,
  input  logic              wr_data_b_en_1p_i,
  input  logic [Width-1:0]  wr_data_b_i
);

  logic [Width-1:0] rf_q [Entries];
  logic [Width-1:0] wr_data_a_del_q;
  logic [Width-1:0] wr_data_b_del_q;

  always_ff @(negedge clk_i) begin
    if (wr_data_a_en_1p_i) wr_data_a_del_q <= wr_data_a_i;
    if (wr_data_b_en_1p_i) wr_data_b_del_q <= wr_data_b_i;
  end

  always_ff @(posedge clk_i) begin
    if (wr_en_b_i) rf_q[wr_addr_b_i] <= wr_data_b_del_q;
    if (wr_en_a_i) rf_q[wr_addr_a_i] <= wr_data_a_del_q;
  end

  assign rd_data_a_o = rf_q[rd_addr_a_i];
  assign rd_data_b_o = rf_q[rd_addr_b_i];
  assign rd_data_c_o = rf_q[rd_addr_c_i];

endmodule : prim_rf_3r2w
