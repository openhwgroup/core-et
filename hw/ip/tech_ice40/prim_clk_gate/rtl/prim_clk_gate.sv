// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock gating cell — iCE40 FPGA implementation.
//
// iCE40 has no dedicated ICG or latch primitives. Uses a negedge-
// triggered flip-flop to sample the enable at the start of the low
// phase, then AND-gates it with the clock. This is the standard FPGA
// approximation of the latch-based ICG and uses only FF + LUT resources.
//
// In scan mode, the clock is forced on (bypass gating).

/* verilator lint_off UNUSEDSIGNAL */
module prim_clk_gate
  import dft_pkg::*;
(
  input  logic clk_i,
  input  logic en_i,
  input  dft_t dft_i,
  output logic clk_o
);

  logic en_q;

  always_ff @(negedge clk_i) begin
    en_q <= en_i | dft_i.scanmode;
  end

  assign clk_o = clk_i & en_q;

endmodule
/* verilator lint_on UNUSEDSIGNAL */
