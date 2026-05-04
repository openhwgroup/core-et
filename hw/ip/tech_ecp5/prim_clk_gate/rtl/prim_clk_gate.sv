// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
/* verilator lint_off UNUSED */  // `dft_t` carries scan-reset/SRAM-test fields that this clock-gate primitive does not consume.
module prim_clk_gate
  import dft_pkg::*;
(
    input  logic clk_i,
    input  logic en_i,
    input  dft_t dft_i,
    output logic clk_o
);

    logic ce;

    assign ce = en_i | dft_i.scanmode;

    // Use ECP5's dedicated clock-control enable. Do not pre-gate CLKI in
    // fabric; LUT-generated clocks are not FPGA-safe.
    (* keep = "true", dont_touch = "true", BEL = "DCCA" *) wire clk_gated;

    DCCA u_dcca (
        .CLKI (clk_i),
        .CE   (ce),
        .CLKO (clk_gated)
    );

    assign clk_o = clk_gated;

endmodule
/* verilator lint_on UNUSED */
