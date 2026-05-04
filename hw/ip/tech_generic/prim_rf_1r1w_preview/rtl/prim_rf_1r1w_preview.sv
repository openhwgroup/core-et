// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_1r1w_preview #(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 32,
  parameter bit          PreviewClkSameAsRf = 1'b1,
  localparam int unsigned AddrW = $clog2(Depth)
) (
  input  logic              preview_clk_i,
  input  logic              rf_clk_i,
  input  logic              wr_data_en_1p_next_i,
  input  logic              wr_en_i,
  input  logic [AddrW-1:0]  wr_addr_i,
  input  logic [Width-1:0]  wr_data_i,
  input  logic [AddrW-1:0]  rd_addr_i,
  output logic [Width-1:0]  rd_data_o
);

  logic [Width-1:0] rf_q [Depth];
  logic [Width-1:0] wr_data_del_q;
  logic             wr_data_en_1p;
  /* verilator lint_off UNUSEDSIGNAL */  // Generic latch model is exact for both clock-relationship modes.
  logic             unused_preview_clk_same_as_rf;
  /* verilator lint_on UNUSEDSIGNAL */

  assign unused_preview_clk_same_as_rf = PreviewClkSameAsRf;

  // Original LATCH_P2 / rlatchn behavior: transparent while preview clock is low.
  /* verilator lint_off COMBDLY */  // Intentional latch timing, not edge-triggered state.
  /* verilator lint_off NOLATCH */  // Some lint-only parents constant-fold the latch enable.
  always_latch begin
    if (!preview_clk_i) begin
      wr_data_en_1p <= wr_data_en_1p_next_i;
    end
  end
  /* verilator lint_on NOLATCH */
  /* verilator lint_on COMBDLY */

  always_ff @(negedge rf_clk_i) begin
    if (wr_data_en_1p) begin
      wr_data_del_q <= wr_data_i;
    end
  end

  always_ff @(posedge rf_clk_i) begin
    if (wr_en_i) begin
      rf_q[wr_addr_i] <= wr_data_del_q;
    end
  end

  assign rd_data_o = rf_q[rd_addr_i];

endmodule : prim_rf_1r1w_preview
