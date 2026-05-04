// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Clock gating cell — iCE40 FPGA implementation for negative-phase pulses.
//
// iCE40 has no dedicated negedge ICG or latch primitive. Use a posedge FF
// to capture the enable at the start of the high phase, then OR it with the
// root clock. This yields a stable active-low pulse during the following low
// phase without inferring a latch.

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

endmodule
