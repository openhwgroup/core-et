// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_clk_gate_n — Negedge clock gating cell.
//
// Enable latched on posedge (high-transparent), output is inverted gated clock.
// When enabled: output goes low when clock goes low (negedge pulse).
// When disabled: output stays high.
//
// Replaces: et_clk_gate_n (etcore-soc)
//
// Generic behavioral model. ASIC: foundry negedge ICG cell.

module prim_clk_gate_n (
  input  logic clk_i,
  input  logic en_i,
  output logic clk_no       // inverted gated clock (active-low pulse when enabled)
);

  logic en_latch;

  /* verilator lint_off COMBDLY */
  /* verilator lint_off LATCH */
  always_latch
    if (clk_i)
      en_latch <= en_i;
  /* verilator lint_on LATCH */
  /* verilator lint_on COMBDLY */

  assign clk_no = clk_i | ~en_latch;

endmodule
