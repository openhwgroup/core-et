// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock gating cell.
//
// Behavioral (generic) implementation: latch-based ICG.
// In scan mode, the clock is forced on (bypass gating).
//
// ASIC: swap this for the foundry ICG cell.
// FPGA: synthesis tools convert to clock enable automatically,
//       or this can be swapped for a CE-based implementation.

/* verilator lint_off UNUSED */  // `dft_t` carries scan-reset/SRAM-test fields that this clock-gate primitive does not consume.
module prim_clk_gate
  import dft_pkg::*;
(
  /* verilator lint_off UNOPTFLAT */  // Clock-gate ports participate in the intended latch-based ICG feedback cone when integrated into preserved gated domains.
  input  logic clk_i,
  input  logic en_i,     // clock enable
  input  dft_t dft_i,    // scan mode forces clock on
  output logic clk_o
  /* verilator lint_on UNOPTFLAT */
);

  /* verilator lint_off UNOPTFLAT */  // Latch-based clock gates look cyclic to Verilator, but this is the intended ICG structure.
  logic en_latch;
  /* verilator lint_on UNOPTFLAT */

  // Match the original et_clk_gate event-control form literally.
  // In preserved gated-clock feedback cones this avoids phase drift relative
  // to the original model while keeping the same latch-based contract.
  /* verilator lint_off COMBDLY */  // Intentional original-style nonblocking assignment in a clock-gate latch process.
  always @(clk_i or en_i or dft_i.scanmode) begin
    if (!clk_i) begin
      en_latch <= en_i | dft_i.scanmode;
    end
  end
  /* verilator lint_on COMBDLY */

  assign clk_o = clk_i & en_latch;

endmodule
/* verilator lint_on UNUSED */
