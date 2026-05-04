// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Direct high-phase to low-phase two-stage capture pair.
//
// Preserve the original high-phase latch followed by low-phase latch
// contract, including the phase-local enables.

module prim_phase_pair_hi_lo #(
  parameter int unsigned Width = 1
) (
  input  logic             clk_i,
  input  logic             hi_en_i,
  input  logic             lo_en_i,
  input  logic [Width-1:0] d_i,
  output logic [Width-1:0] q_hi_o,
  output logic [Width-1:0] q_lo_o
);

  (* keep = "true", dont_touch = "true" *) logic             hi_en_1p;
  (* keep = "true", dont_touch = "true" *) logic             lo_en_2p;
  (* keep = "true", dont_touch = "true" *) logic [Width-1:0] q_hi_reg;
  (* keep = "true", dont_touch = "true" *) logic [Width-1:0] q_lo_reg;

  /* verilator lint_off COMBDLY */
  /* verilator lint_off NOLATCH */  // This primitive intentionally models the direct high-phase to low-phase transparent handoff.
  always_latch begin
    if (!clk_i) begin
      hi_en_1p <= hi_en_i;
    end
  end

  always_latch begin
    if (clk_i) begin
      lo_en_2p <= lo_en_i;
      if (hi_en_1p) begin
        q_hi_reg <= d_i;
      end
    end
  end

  always_latch begin
    if (!clk_i && lo_en_2p) begin
      q_lo_reg <= q_hi_reg;
    end
  end
  /* verilator lint_on NOLATCH */
  /* verilator lint_on COMBDLY */

  assign q_hi_o = q_hi_reg;
  assign q_lo_o = q_lo_reg;

endmodule
