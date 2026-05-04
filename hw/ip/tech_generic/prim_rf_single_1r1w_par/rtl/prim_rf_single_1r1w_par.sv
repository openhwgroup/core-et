// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: latch-timed single-entry write-capture register.
// Generic/default implementation.

module prim_rf_single_1r1w_par #(
  parameter int unsigned Width = 32
) (
  input  logic             clk_i,
  output logic [Width-1:0] rd_data_o,
  input  logic [Width-1:0] wr_data_i,
  input  logic             wr_data_en_1p_i,
  input  logic             wr_en_i
);

  logic [Width-1:0] rf_q;
  logic [Width-1:0] wr_data_del_q;

  always_ff @(negedge clk_i) begin
    if (wr_data_en_1p_i) wr_data_del_q <= wr_data_i;
  end

  always_ff @(posedge clk_i) begin
    if (wr_en_i) rf_q <= wr_data_del_q;
  end

  assign rd_data_o = rf_q;

endmodule : prim_rf_single_1r1w_par
