// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rst_repeat vs new prim_rst_sync.
// Adapts reset polarity and ties off DFT ports.

`include "soc.vh"

module cosim_rst_sync_tb (
  input  logic clk_i,
  input  logic rst_ni,     // active-low reset input

  output logic new_rst_no,
  output logic orig_rst_no
);

  // ── New module (active-low in, active-low out) ────────
  prim_rst_sync u_new (
    .clk_i,
    .rst_ni,
    .dft_i  (dft_pkg::dft_t'('0)),
    .rst_no (new_rst_no)
  );

  // ── Original module (active-high in, active-high out) ─
  logic orig_reset_out;

  rst_repeat u_orig (
    .clock          (clk_i),
    .reset_in       (~rst_ni),      // invert: our active-low → original's active-high
    .reset_out      (orig_reset_out),
    .dft__reset_byp (1'b0),         // no DFT bypass
    .dft__reset     (1'b0)
  );

  // Invert original output to match our active-low convention.
  assign orig_rst_no = ~orig_reset_out;

endmodule
