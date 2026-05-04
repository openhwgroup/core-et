// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_rf_2r1w_preview #(
  parameter int unsigned         Width        = 32,
  parameter int unsigned         Entries      = 32,
  parameter logic [Entries-1:0] Zero         = '0,
  parameter logic [Entries-1:0] Parallel     = '0,
  parameter int unsigned         ParallelW    = 32,
  parameter int unsigned         Level2CkGate = 0,
  parameter bit                  PreviewClkSameAsRf = 1'b1,
  localparam int unsigned        AddrW        = $clog2(Entries)
) (
  input  logic                   preview_clk_i,
  input  logic                   rf_clk_i,
  input  logic [AddrW-1:0]       rd_addr_a_i,
  output logic [Width-1:0]       rd_data_a_o,
  input  logic [AddrW-1:0]       rd_addr_b_i,
  output logic [Width-1:0]       rd_data_b_o,
  output logic [ParallelW-1:0]   rd_par_o,
  input  logic                   wr_en_i,
  input  logic                   wr_data_en_1p_next_i,
  input  logic [AddrW-1:0]       wr_addr_i,
  input  logic [Width-1:0]       wr_data_i
);

  logic [Width-1:0] rf_q [Entries];
  logic [Width-1:0] wr_data_del_q;
  logic             wr_data_en_1p_q;
  /* verilator lint_off UNUSEDSIGNAL */  // Generic latch model is exact for both clock-relationship modes.
  logic             unused_preview_clk_same_as_rf;
  /* verilator lint_on UNUSEDSIGNAL */

  function automatic int unsigned par_position(input int unsigned pos);
    par_position = 0;
    for (int unsigned j = 0; j < pos; j++) begin
      if (Parallel[j]) begin
        par_position++;
      end
    end
  endfunction

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
      wr_data_en_1p_q <= wr_data_en_1p_next_i;
    end
  end
  /* verilator lint_on NOLATCH */
  /* verilator lint_on COMBDLY */

  always_ff @(negedge rf_clk_i) begin
    if (wr_data_en_1p_q) begin
      wr_data_del_q <= wr_data_i;
    end
  end

  for (genvar g = 0; g < Entries; g++) begin : gen_rf
    localparam logic [AddrW-1:0] EntryIdx = g;
    if (Zero[g]) begin : gen_zero
      assign rf_q[g] = '0;
    end else begin : gen_entry
      always_ff @(posedge rf_clk_i) begin
        if (wr_en_i && (wr_addr_i == EntryIdx)) begin
          rf_q[g] <= wr_data_del_q;
        end
      end
    end
  end

  assign rd_data_a_o = rf_q[rd_addr_a_i];
  assign rd_data_b_o = rf_q[rd_addr_b_i];

  always_comb begin
    rd_par_o = '0;
    for (int i = 0; i < Entries; i++) begin
      if (Parallel[i]) begin
        rd_par_o[Width*par_position(i) +: Width] = rf_q[i];
      end
    end
  end

endmodule : prim_rf_2r1w_preview
