// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock gating cell — Xilinx FPGA implementation.
//
// Xilinx UltraScale+ and 7-series have BUFGCE for global clock
// gating, but those are scarce resources. For local clock gating
// (the common case in this design), use a negedge-triggered FF
// to sample the enable, then AND with the clock. Vivado and Yosys
// both handle this pattern cleanly.
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
