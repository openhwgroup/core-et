// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_phase_pair_lo_hi_tb (
  input  logic [7:0] d_i,
  input  logic       clk_i,
  input  logic       lo_en_i,
  input  logic       hi_en_i,
  output logic [7:0] new_q_lo_o,
  output logic [7:0] new_q_hi_o,
  output logic [7:0] orig_q_lo_o,
  output logic [7:0] orig_q_hi_o
);

  logic orig_clk_lo_n;
  logic orig_clk_hi;

  prim_phase_pair_lo_hi #(
    .Width (8)
  ) u_new (
    .clk_i,
    .lo_en_i,
    .hi_en_i,
    .d_i,
    .q_lo_o (new_q_lo_o),
    .q_hi_o (new_q_hi_o)
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
    .d   (d_i),
    .q   (orig_q_lo_o)
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
    .d   (orig_q_lo_o),
    .q   (orig_q_hi_o)
  );

endmodule
