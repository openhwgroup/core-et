// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_prim_write_commit_rst_en_tb (
  input  logic [7:0] d_i,
  input  logic       clk_i,
  input  logic       rst_i,
  input  logic       en_i,
  output logic [7:0] new_q_o,
  output logic [7:0] orig_q_o
);

  localparam logic [7:0] ResetValue = 8'hA5;

  logic orig_clk_hi;

  prim_write_commit_rst_en #(
    .Width      (8),
    .ResetValue (ResetValue)
  ) u_new (
    .clk_i,
    .rst_i,
    .en_i,
    .d_i,
    .q_o (new_q_o)
  );

  et_clk_gate u_orig_clk_gate (
    .enclk (orig_clk_hi),
    .en    (en_i),
    .clk   (clk_i),
    .te    (1'b0)
  );

  /* verilator lint_off COMBDLY */
  /* verilator lint_off NOLATCH */  // Mirrors the original resettable CSR write-latch macro expansion directly.
  always_latch begin
    if (rst_i) begin
      orig_q_o <= ResetValue;
    end else if (orig_clk_hi) begin
      orig_q_o <= d_i;
    end
  end
  /* verilator lint_on NOLATCH */
  /* verilator lint_on COMBDLY */

endmodule
