// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// 2:1 clock-selection primitive.
//
// Selects between clk0_i (sel_i=0) and clk1_i (sel_i=1).
// Used primarily for DFT SRAM clock override.
//
// Behavioral implementation: simple mux. Callers must obey the contract
// documented in README.md and only change sel_i when the switch is safe.
// ASIC tapeout can replace this with a dedicated glitch-managed clock mux.

module prim_clk_mux #(
  parameter bit UseTechClockMux = 1'b1
) (
  input  logic clk0_i,
  input  logic clk1_i,
  input  logic sel_i,    // 0 = clk0, 1 = clk1
  output logic clk_o
);

  generate
    if (UseTechClockMux) begin : gen_mux
      assign clk_o = sel_i ? clk1_i : clk0_i;
    end else begin : gen_passthrough
      /* verilator lint_off UNUSEDSIGNAL */  // Explicit clock-mux opt-out is for tied-off DFT paths that only use clk0_i.
      logic unused_mux_inputs;
      assign unused_mux_inputs = ^{clk1_i, sel_i};
      /* verilator lint_on UNUSEDSIGNAL */

      assign clk_o = clk0_i;
    end
  endgenerate

endmodule
