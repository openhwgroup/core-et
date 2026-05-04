// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock multiplexer -- ECP5 FPGA implementation.
//
// ECP5 has only two DCSC dynamic clock-select blocks. The default FPGA RAM/DFT
// use keeps the DFT override tied off and forwards clk0_i without consuming a
// DCSC. Set UseTechClockMux only for scarce top-level dynamic clock-selection
// sites that actually need hardware switching.

module prim_clk_mux #(
  parameter bit UseTechClockMux = 1'b0
) (
  input  logic clk0_i,
  input  logic clk1_i,
  input  logic sel_i,
  output logic clk_o
);

  generate
    if (UseTechClockMux) begin : gen_dcsc
      (* keep = "true", dont_touch = "true" *) wire clk_muxed;

      DCSC #(
        .DCSMODE("POS")
      ) u_dcsc (
        .CLK1   (clk1_i),
        .CLK0   (clk0_i),
        .SEL1   (sel_i),
        .SEL0   (~sel_i),
        .MODESEL(1'b0),
        .DCSOUT (clk_muxed)
      );

      assign clk_o = clk_muxed;
    end else begin : gen_passthrough
      /* verilator lint_off UNUSEDSIGNAL */  // ECP5 FPGA RAM wrappers tie off the DFT clock override; do not consume scarce DCSC blocks for inactive muxes.
      logic unused_mux_inputs;
      assign unused_mux_inputs = ^{clk1_i, sel_i};
      /* verilator lint_on UNUSEDSIGNAL */

      assign clk_o = clk0_i;
    end
  endgenerate

endmodule : prim_clk_mux
