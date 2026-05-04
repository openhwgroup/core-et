// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_3r2w_preview #(
  parameter int unsigned Width        = 32,
  parameter int unsigned Entries      = 32,
  parameter int unsigned Level2CkGate = 0,
  parameter bit          PreviewClkSameAsRf = 1'b1,
  localparam int unsigned AddrW       = $clog2(Entries)
) (
  input  logic              preview_clk_i,
  input  logic              rf_clk_i,
  input  logic [AddrW-1:0]  rd_addr_a_i,
  output logic [Width-1:0]  rd_data_a_o,
  input  logic [AddrW-1:0]  rd_addr_b_i,
  output logic [Width-1:0]  rd_data_b_o,
  input  logic [AddrW-1:0]  rd_addr_c_i,
  output logic [Width-1:0]  rd_data_c_o,
  input  logic              wr_en_a_i,
  input  logic [AddrW-1:0]  wr_addr_a_i,
  input  logic              wr_data_a_en_1p_next_i,
  input  logic [Width-1:0]  wr_data_a_i,
  input  logic              wr_en_b_i,
  input  logic [AddrW-1:0]  wr_addr_b_i,
  input  logic              wr_data_b_en_1p_next_i,
  input  logic [Width-1:0]  wr_data_b_i
);

  logic [Width-1:0] rf_q [Entries];
  logic [Width-1:0] wr_data_a_del_q;
  logic [Width-1:0] wr_data_b_del_q;
  logic             wr_data_a_en_1p_q;
  logic             wr_data_b_en_1p_q;
  /* verilator lint_off UNUSEDSIGNAL */  // Generic latch model is exact for both clock-relationship modes.
  logic             unused_preview_clk_same_as_rf;
  /* verilator lint_on UNUSEDSIGNAL */

  /* verilator lint_off UNUSEDPARAM */  // retained for ASIC compatibility, unused by the generic body
  logic [0:0] unused_level2_ck_gate;
  /* verilator lint_on UNUSEDPARAM */

  assign unused_level2_ck_gate = Level2CkGate[0:0];
  assign unused_preview_clk_same_as_rf = PreviewClkSameAsRf;

  // Original LATCH_P2 / rlatchn behavior: transparent while preview clock is low.
  /* verilator lint_off COMBDLY */  // Intentional latch timing, not edge-triggered state.
  /* verilator lint_off NOLATCH */  // Some lint-only parents constant-fold the latch enable.
  always_latch begin
    if (!preview_clk_i) begin
      wr_data_a_en_1p_q <= wr_data_a_en_1p_next_i;
      wr_data_b_en_1p_q <= wr_data_b_en_1p_next_i;
    end
  end
  /* verilator lint_on NOLATCH */
  /* verilator lint_on COMBDLY */

  always_ff @(negedge rf_clk_i) begin
    if (wr_data_a_en_1p_q) begin
      wr_data_a_del_q <= wr_data_a_i;
    end
    if (wr_data_b_en_1p_q) begin
      wr_data_b_del_q <= wr_data_b_i;
    end
  end

  always_ff @(posedge rf_clk_i) begin
    if (wr_en_b_i) begin
      rf_q[wr_addr_b_i] <= wr_data_b_del_q;
    end
    if (wr_en_a_i) begin
      rf_q[wr_addr_a_i] <= wr_data_a_del_q;
    end
  end

  assign rd_data_a_o = rf_q[rd_addr_a_i];
  assign rd_data_b_o = rf_q[rd_addr_b_i];
  assign rd_data_c_o = rf_q[rd_addr_c_i];

endmodule : prim_rf_3r2w_preview
