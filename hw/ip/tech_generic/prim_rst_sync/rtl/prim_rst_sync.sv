// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Reset synchronizer: async assert, sync deassert.
//
// Active-low reset input (rst_ni), active-low reset output (rst_no).
// In scan mode (dft_i.scanmode=1), the synchronizer is bypassed and
// rst_no follows dft_i.scan_reset_n directly.
//
// Behaviorally equivalent to the CORE-ET rst_repeat module.

/* verilator lint_off SYNCASYNCNET */  // rst_int_ni is the intended muxed async-reset source, used in both the sensitivity list and the scan-bypass output selection.
module prim_rst_sync
  import dft_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,   // async assert, sync deassert
  input  dft_t dft_i,    // DFT: bypass synchronizer in scan mode
  output logic rst_no    // synchronized reset output (active-low)
);

  logic rst_q;
  logic rst_int_ni;

  // Match the original rst_repeat contract: in scan mode, scan_reset_n is the
  // reset source for both the repeater flop and the final output bypass.
  assign rst_int_ni = dft_i.scanmode ? dft_i.scan_reset_n : rst_ni;

  always_ff @(posedge clk_i or negedge rst_int_ni) begin
    if (!rst_int_ni) begin
      rst_q <= 1'b0;
    end else begin
      rst_q <= 1'b1;
    end
  end

  // In scan mode, bypass the synchronizer flop.
  assign rst_no = dft_i.scanmode ? dft_i.scan_reset_n : rst_q;

  logic unused_dft;
  assign unused_dft = &{1'b0, dft_i.sram_clk_override, dft_i.ram_rei, dft_i.ram_wei};

endmodule
/* verilator lint_on SYNCASYNCNET */
