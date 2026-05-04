// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_write_preview_en #(
  parameter int unsigned Width = 1
) (
  input  logic             clk_i,
  input  logic             en_i,
  input  logic [Width-1:0] d_i,
  output logic [Width-1:0] q_o
);

  /* verilator lint_off UNOPTFLAT */  // Phase-sampled preview state intentionally participates in latch-style feedback through the enclosing write pipeline.
  logic en_2p;
  /* verilator lint_on UNOPTFLAT */

  /* verilator lint_off COMBDLY */
  /* verilator lint_off NOLATCH */  // Intentionally preserves phase-sampled write-preview behavior.
  always_latch begin
    if (clk_i) begin
      en_2p <= en_i;
    end
  end

  always_latch begin
    if (!clk_i && en_2p) begin
      q_o <= d_i;
    end
  end
  /* verilator lint_on NOLATCH */
  /* verilator lint_on COMBDLY */

endmodule
