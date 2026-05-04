// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// DFT (Design for Test) type definitions.
//
// Provides a clean, consolidated DFT interface struct that replaces
// the CORE-ET dft__* individual signal convention. Modules that
// interact with clocks, resets, or SRAMs accept dft_t as an input.
// In normal operation, dft_i = '0 (scanmode off).

package dft_pkg;

  typedef struct packed {
    logic scanmode;       // Scan mode active — bypass functional clocks/resets
    logic scan_reset_n;   // Scan reset (active-low), used when scanmode=1
    logic sram_clk_override; // Enable DFT SRAM clock override (selects sram_clock via prim_clk_mux)
    logic ram_rei;        // RAM read enable inhibit (DFT)
    logic ram_wei;        // RAM write enable inhibit (DFT)
  } dft_t;

  // SRAM DFT clock — passed separately because it's a clock signal,
  // not a data signal. Cannot be part of a packed struct that's
  // registered or used combinationally.
  //
  // Modules that use prim_clk_mux for DFT SRAM clock override
  // accept this as a separate port: `input logic dft_sram_clk_i`.

endpackage
