// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Low-phase to high-phase two-stage capture pair.
//
// Contract:
//   - q_lo_o is the low-phase stage of the pair
//   - q_hi_o is the following high-phase stage
//   - lo_en_i and hi_en_i preserve the original phase-local gate-plus-latch
//     semantics directly inside this self-contained primitive
//   - q_hi_o captures q_lo_o directly; there is no extra combinational rewrite
//
// This is the named composite form of a repeated rlatchn -> rlatch chain when
// both phase outputs remain live in shared RTL. Tie lo_en_i and hi_en_i high
// for an always-open direct pair.

module prim_phase_pair_lo_hi #(
  parameter int unsigned Width = 1
) (
  /* verilator lint_off SYNCASYNCNET */  // The shared phase clock intentionally drives both low-phase and high-phase latch legs in the preserved two-phase pair.
  input  logic             clk_i,
  input  logic             lo_en_i,
  input  logic             hi_en_i,
  input  logic [Width-1:0] d_i,
  output logic [Width-1:0] q_lo_o,
  output logic [Width-1:0] q_hi_o
  /* verilator lint_on SYNCASYNCNET */
);

  /* verilator lint_off UNOPTFLAT */  // Intentional two-phase handoff can look combinationally cyclic when the parent preserves latch-style feedback.
  logic             lo_en_2p;
  logic             hi_en_1p;
  logic [Width-1:0] q_lo_reg;
  logic [Width-1:0] q_hi_reg;

  /* verilator lint_off COMBDLY */
  /* verilator lint_off NOLATCH */  // Intentionally preserves the direct low-phase then high-phase transparent handoff.
  always_latch begin
    if (clk_i) begin
      lo_en_2p <= lo_en_i;
    end
  end

  always_latch begin
    if (!clk_i) begin
      hi_en_1p <= hi_en_i;
      if (lo_en_2p) begin
        q_lo_reg <= d_i;
      end
    end
  end

  always_latch begin
    if (clk_i && hi_en_1p) begin
      q_hi_reg <= q_lo_reg;
    end
  end
  /* verilator lint_on NOLATCH */
  /* verilator lint_on COMBDLY */

  assign q_lo_o = q_lo_reg;
  assign q_hi_o = q_hi_reg;
  /* verilator lint_on UNOPTFLAT */

endmodule
