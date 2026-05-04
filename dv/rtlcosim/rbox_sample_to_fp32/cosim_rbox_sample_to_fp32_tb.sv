// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_sample_to_fp32 vs new rbox_sample_to_fp32.
//
// Both modules convert a signed 18.25 fixed-point edge sample to FP32
// in a 2-stage pipeline. We inline a renamed copy of the original to
// avoid module name collisions.

`include "soc.vh"

module cosim_rbox_sample_to_fp32_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus
  input  rbox_pkg::edge_sample_t sample_i,   // 43 bits
  input  logic                   valid_i,

  // New module output
  output logic [31:0]            new_out_o,

  // Original module output
  output logic [31:0]            orig_out_o
);

  // ── New module ────────────────────────────────────────

  rbox_sample_to_fp32 u_new (
    .clk_i   (clk_i),
    .in_i    (sample_i),
    .valid_i (valid_i),
    .out_o   (new_out_o)
  );

  // ── Original module ───────────────────────────────────

  rbox_sample_to_fp32_orig u_orig (
    .clock (clk_i),
    .in    (sample_i),
    .valid (valid_i),
    .out   (orig_out_o)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Inlined copy of original rbox_sample_to_fp32 (renamed)
// ════════════════════════════════════════════════════════════

module rbox_sample_to_fp32_orig
#(
    parameter SAMPLE_INT_BITS = 18,
    parameter SAMPLE_FRC_BITS = 25,
    localparam SAMPLE_BITS = SAMPLE_INT_BITS + SAMPLE_FRC_BITS,
    localparam BASE_EXP = 127 - SAMPLE_FRC_BITS
 )
 (
    input  logic              clock,
    input  rbox_edge_sample_t in,
    input  logic              valid,
    output logic [31:0]       out
 );

logic                   sign_nxt;
logic                   sign_ff;
logic [7:0]             exp_nxt;
logic [7:0]             exp_ff;
logic [7:0]             exp_final;
logic [7:0]             norm_shift;
logic [SAMPLE_BITS-1:0] mant_magnitude;
logic [SAMPLE_BITS-1:0] mant_normalized_nxt;
logic [SAMPLE_BITS-1:0] mant_normalized_ff;

logic [$clog2(SAMPLE_BITS)-1:0] leading_one;
logic                           round_up;

always_comb
begin
    sign_nxt = in[SAMPLE_BITS-1];
    mant_magnitude = sign_nxt ? ~in + 'd1 : in;

    leading_one = SAMPLE_BITS-1;
    for(integer b = 0; (b < SAMPLE_BITS) && (~mant_magnitude[SAMPLE_BITS-1 - b]); b++)
        leading_one--;

    exp_nxt = BASE_EXP + leading_one;

    norm_shift = (SAMPLE_BITS - 1) - leading_one;

    mant_normalized_nxt = mant_magnitude << norm_shift;
end

`EN_FF(clock, valid, mant_normalized_ff, mant_normalized_nxt)
`EN_FF(clock, valid, sign_ff,            sign_nxt           )
`EN_FF(clock, valid, exp_ff,             exp_nxt            )

logic [25:0]            mant_normalized_msb;
logic [24:0]            mant_round;
logic [23:0]            mant_final;

always_comb
begin
    mant_normalized_msb = mant_normalized_ff[SAMPLE_BITS-1 -: 26];

    round_up =  (mant_normalized_msb[1:0] == 2'b11)
             || (mant_normalized_msb[2] && (mant_normalized_msb[1:0] == 2'b10));
    mant_round = round_up ? {1'b0, mant_normalized_msb[25:2]} + 1 : {1'b0, mant_normalized_msb[25:2]};

    exp_final = mant_round[24] ? exp_ff + 1 : exp_ff;
    mant_final = mant_round[24] ? mant_round[24:1] : mant_round[23:0];

    out = {sign_ff, exp_final, mant_final[22:0]};
end

endmodule
