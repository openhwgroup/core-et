// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock gating cell — Xilinx FPGA implementation for negative-phase pulses.
//
// This is the FPGA-friendly companion to prim_clk_gate. Capture the enable at
// the start of the high phase with a posedge FF, then OR it with the root
// clock to form the following active-low pulse on clk_no without inferring a
// high-transparent latch.

module prim_clk_gate_n (
  input  logic clk_i,
  input  logic en_i,
  output logic clk_no
);

  logic en_q;

  always_ff @(posedge clk_i) begin
    en_q <= en_i;
  end

  assign clk_no = clk_i | ~en_q;

endmodule : prim_clk_gate_n
