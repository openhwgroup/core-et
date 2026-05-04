// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_phase_pair_lo_hi_tb (
  input  logic [7:0] d_i,
  input  logic       clk_i,
  input  logic       lo_en_i,
  input  logic       hi_en_i,
  output logic [7:0] q_lo_o,
  output logic [7:0] q_hi_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Lint anchors keep companion clock-gate primitives reachable when this wrapper includes the full phase primitive seam set.
  logic lint_clk_gate_o;
  logic lint_clk_no;
  /* verilator lint_on UNUSEDSIGNAL */

  prim_clk_gate u_lint_clk_gate (
    .clk_i (clk_i),
    .en_i  (1'b0),
    .dft_i ('0),
    .clk_o (lint_clk_gate_o)
  );

  prim_clk_gate_n u_lint_clk_gate_n (
    .clk_i  (clk_i),
    .en_i   (1'b0),
    .clk_no (lint_clk_no)
  );

  prim_phase_pair_lo_hi #(
    .Width (8)
  ) u_dut (
    .clk_i,
    .lo_en_i,
    .hi_en_i,
    .d_i,
    .q_lo_o,
    .q_hi_o
  );

endmodule
