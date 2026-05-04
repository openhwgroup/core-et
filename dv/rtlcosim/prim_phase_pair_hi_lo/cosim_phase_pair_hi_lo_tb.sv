// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_phase_pair_hi_lo_tb (
  input  logic [7:0] d_i,
  input  logic       clk_i,
  input  logic       hi_en_i,
  input  logic       lo_en_i,
  output logic [7:0] new_q_hi_o,
  output logic [7:0] new_q_lo_o,
  output logic [7:0] orig_q_hi_o,
  output logic [7:0] orig_q_lo_o
);

  logic orig_clk_hi;
  logic orig_clk_lo_n;

  prim_phase_pair_hi_lo #(
    .Width (8)
  ) u_new (
    .clk_i,
    .hi_en_i,
    .lo_en_i,
    .d_i,
    .q_hi_o (new_q_hi_o),
    .q_lo_o (new_q_lo_o)
  );

  et_clk_gate u_orig_clk_gate_hi (
    .enclk (orig_clk_hi),
    .en    (hi_en_i),
    .clk   (clk_i),
    .te    (1'b0)
  );

  rlatch #(
    .WIDTH (8)
  ) u_orig_hi (
    .clk (orig_clk_hi),
    .d   (d_i),
    .q   (orig_q_hi_o)
  );

  et_clk_gate_n u_orig_clk_gate_lo (
    .enclk_n (orig_clk_lo_n),
    .en_1p   (lo_en_i),
    .clk     (clk_i),
    .te      (1'b0)
  );

  rlatchn #(
    .WIDTH (8)
  ) u_orig_lo (
    .clk (orig_clk_lo_n),
    .d   (orig_q_hi_o),
    .q   (orig_q_lo_o)
  );

endmodule
