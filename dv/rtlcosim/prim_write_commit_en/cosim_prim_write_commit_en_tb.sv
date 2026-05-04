// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_prim_write_commit_en_tb (
  input  logic [7:0] d_i,
  input  logic       clk_i,
  input  logic       en_i,
  output logic [7:0] new_q_o,
  output logic [7:0] orig_q_o
);

  logic orig_clk_hi;

  prim_write_commit_en #(
    .Width (8)
  ) u_new (
    .clk_i,
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

  rlatch #(
    .WIDTH (8)
  ) u_orig (
    .clk (orig_clk_hi),
    .d   (d_i),
    .q   (orig_q_o)
  );

endmodule
