// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Reset synchronizer / repeater — iCE40 FPGA implementation.
//
// iCE40 does not have a dedicated reset-repeater hard macro. Keep the contract
// as a single async-assert / sync-release flop and let Yosys map it onto the
// native async-reset flip-flop resources. In scan mode, scan_reset_n drives
// both the repeater flop's async reset input and the final bypass path, matching
// the original rst_repeat behavior.

module prim_rst_sync
  import dft_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  dft_t dft_i,
  output logic rst_no
);

  logic rst_q;
  logic rst_int_ni;

  assign rst_int_ni = dft_i.scanmode ? dft_i.scan_reset_n : rst_ni;

  always_ff @(posedge clk_i or negedge rst_int_ni) begin
    if (!rst_int_ni) begin
      rst_q <= 1'b0;
    end else begin
      rst_q <= 1'b1;
    end
  end

  assign rst_no = dft_i.scanmode ? dft_i.scan_reset_n : rst_q;

  logic unused_dft;
  assign unused_dft = &{1'b0, dft_i.sram_clk_override, dft_i.ram_rei, dft_i.ram_wei};

endmodule
