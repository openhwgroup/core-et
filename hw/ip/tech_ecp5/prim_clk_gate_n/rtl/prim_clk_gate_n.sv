// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock gating cell -- ECP5 FPGA implementation for negative-phase pulses.
//
// Preserve the original et_clk_gate_n waveform while keeping the final exported
// local clock on a DCCA output.
module prim_clk_gate_n (
    input  logic clk_i,
    input  logic en_i,
    output logic clk_no
);

    logic en_latch;
    (* keep = "true", dont_touch = "true" *) wire clk_enabled;
    (* keep = "true", dont_touch = "true" *) wire clk_no_fabric;
    (* keep = "true", dont_touch = "true", BEL = "DCCA" *) wire clk_no_gated;

    /* verilator lint_off COMBDLY */
    /* verilator lint_off LATCH */
    always_latch begin
        if (clk_i) begin
            en_latch <= en_i;
        end
    end
    /* verilator lint_on LATCH */
    /* verilator lint_on COMBDLY */

    DCCA u_dcca_gate (
        .CLKI (clk_i),
        .CE   (en_latch),
        .CLKO (clk_enabled)
    );

    assign clk_no_fabric = clk_enabled | ~en_latch;

    DCCA u_dcca_promote (
        .CLKI (clk_no_fabric),
        .CE   (1'b1),
        .CLKO (clk_no_gated)
    );

    assign clk_no = clk_no_gated;

endmodule : prim_clk_gate_n
