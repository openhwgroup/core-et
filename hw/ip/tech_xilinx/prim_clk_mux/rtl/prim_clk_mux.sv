// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock mux — Xilinx FPGA implementation.
//
// Route this clock-selection seam through BUFGCTRL instead of a LUT mux. The
// system contract is still that sel_i changes only when the switch is safe;
// current users are SRAM DFT clock overrides rather than live functional clock
// switching.

module prim_clk_mux #(
  parameter bit UseTechClockMux = 1'b1
) (
  input  logic clk0_i,
  input  logic clk1_i,
  input  logic sel_i,
  output logic clk_o
);

  generate
    if (UseTechClockMux) begin : gen_bufgctrl
      BUFGCTRL #(
        .INIT_OUT    (1'b0),
        .PRESELECT_I0("FALSE"),
        .PRESELECT_I1("FALSE")
      ) u_bufgctrl (
        .O       (clk_o),
        .I0      (clk0_i),
        .I1      (clk1_i),
        .S0      (~sel_i),
        .S1      (sel_i),
        .CE0     (1'b1),
        .CE1     (1'b1),
        .IGNORE0 (1'b0),
        .IGNORE1 (1'b0)
      );
    end else begin : gen_passthrough
      /* verilator lint_off UNUSEDSIGNAL */  // Explicit clock-mux opt-out is for tied-off DFT paths that only use clk0_i.
      logic unused_mux_inputs;
      assign unused_mux_inputs = ^{clk1_i, sel_i};
      /* verilator lint_on UNUSEDSIGNAL */

      assign clk_o = clk0_i;
    end
  endgenerate

endmodule : prim_clk_mux
