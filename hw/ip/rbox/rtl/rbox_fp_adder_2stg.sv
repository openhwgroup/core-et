// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX 2-stage pipelined IEEE754 floating-point adder.
//
// Same algorithm as rbox_fp_fast_adder but split into two pipeline stages:
// Stage 1 (registered): mantissa alignment, exponent comparison, sign logic
// Stage 2 (combinational): addition, LZA normalization, overflow/underflow
//
// Inputs use the internal depth representation (not fully renormalized).
// Input subnormals must have been flushed to zero.
// Output rounding is Round-To-Zero. Output subnormals are flushed to zero.

module rbox_fp_adder_2stg
  import rbox_pkg::*;
(
  input  logic            clk_i,
  /* verilator lint_off UNUSEDSIGNAL */ // internal_depth_t.subnorm unused by adder
  input  internal_depth_t a_i,
  input  internal_depth_t b_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic            valid_i,
  output internal_depth_t out_o
);

  // ── Stage 1: alignment and operand preparation ──────────

  /* verilator lint_off UNUSEDSIGNAL */ // Partial bit usage in FP intermediates (mant_shift_p3[8])
  /* verilator lint_off UNOPTFLAT */   // Shifter outputs feed back into same always_comb — acyclic per-field
  logic [25:0] a_mant;
  logic [25:0] b_mant;
  logic        a_sticky;
  logic        b_sticky;
  logic [25:0] a_shift_mant;
  logic [25:0] b_shift_mant;

  logic [26:0] a_grs_mant;
  logic [26:0] b_grs_mant;

  logic [27:0] a_final_mant;
  logic [27:0] b_final_mant;

  logic [8:0]  a_mant_shift;
  logic [8:0]  a_mant_shift_p1;
  logic [8:0]  a_mant_shift_p2;
  logic [8:0]  a_mant_shift_p3;
  logic [8:0]  b_mant_shift;
  logic [8:0]  b_mant_shift_p1;
  logic [8:0]  b_mant_shift_p2;
  logic [8:0]  b_mant_shift_p3;

  logic [7:0]  a_exp_with_delta;
  logic [7:0]  b_exp_with_delta;
  logic [8:0]  out_exp_pre_renorm;

  logic        sub_op;
  logic        a_exp_is_greater;
  logic        norm_out;
  logic        out_is_zero_pre;
  logic        out_is_inf_pre;
  logic        out_is_nan;
  logic        out_sign;

  rbox_fp_shifter #(.Sz(26), .SignExtend(0), .ShiftSz(8))
  u_a_shifter (
    .in_i     (a_mant),
    .shift_i  (a_mant_shift),
    .out_o    (a_shift_mant),
    .sticky_o (a_sticky)
  );

  rbox_fp_shifter #(.Sz(26), .SignExtend(0), .ShiftSz(8))
  u_b_shifter (
    .in_i     (b_mant),
    .shift_i  (b_mant_shift),
    .out_o    (b_shift_mant),
    .sticky_o (b_sticky)
  );

  always_comb begin
    sub_op = a_i.s ^ b_i.s && !a_i.zero && !b_i.zero;

    a_mant = {a_i.m, 2'b0};
    b_mant = {b_i.m, 2'b0};

    a_exp_with_delta = a_i.e + 8'($signed(a_i.e_delta));
    b_exp_with_delta = b_i.e + 8'($signed(b_i.e_delta));

    a_exp_is_greater = a_exp_with_delta > b_exp_with_delta;

    a_mant_shift_p1 =                                          9'(a_i.m_shift);
    a_mant_shift_p2 =   9'(b_exp_with_delta - a_exp_with_delta) - 9'(a_i.m_shift);
    a_mant_shift_p3 = - 9'(b_exp_with_delta - a_exp_with_delta) + 9'(a_i.m_shift);

    b_mant_shift_p1 =                                          9'(b_i.m_shift);
    b_mant_shift_p2 =   9'(a_exp_with_delta - b_exp_with_delta) - 9'(b_i.m_shift);
    b_mant_shift_p3 = - 9'(a_exp_with_delta - b_exp_with_delta) + 9'(b_i.m_shift);

    a_mant_shift = a_exp_is_greater ?                                                       a_mant_shift_p1
                                    : (a_mant_shift_p2[8] ? {1'b1, a_mant_shift_p3[7:0]} : a_mant_shift_p2);

    b_mant_shift = a_exp_is_greater ? (b_mant_shift_p2[8] ? {1'b1, b_mant_shift_p3[7:0]} : b_mant_shift_p2)
                                    :                                                       b_mant_shift_p1;

    out_is_zero_pre = a_i.zero     && b_i.zero;
    out_is_nan      = a_i.nan      || b_i.nan;
    out_is_inf_pre  = a_i.infinite || b_i.infinite;

    a_grs_mant = {a_shift_mant, a_sticky};
    b_grs_mant = {b_shift_mant, b_sticky};

    unique casez ({a_i.zero, b_i.zero, out_is_inf_pre, out_is_nan, sub_op, a_exp_is_greater})
      {1'b1, 1'b0, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               {1'b0,  b_i.m, 3'b0}};
      {1'b0, 1'b1, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, {1'b0,  a_i.m, 3'b0}, 28'h0             };
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b0,  b_grs_mant}};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b1, ~a_grs_mant}, {1'b0,  b_grs_mant}};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b1}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b1, ~b_grs_mant}};
      default:                                {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               28'h0              };
    endcase

    unique casez ({a_i.zero, b_i.zero, out_is_inf_pre, out_is_nan, a_exp_is_greater})
      {1'b1, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = '0;
      {1'b1, 1'b0, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
      {1'b0, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b1}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
      {1'b0, 1'b0, 1'b1, 1'b0, 1'b?}: out_exp_pre_renorm = '1;
      {1'b0, 1'b0, 1'b0, 1'b1, 1'b?}: out_exp_pre_renorm = '1;
      default:                          out_exp_pre_renorm = '0;
    endcase

    unique casez ({a_i.zero, b_i.zero, a_exp_is_greater, a_i.nan, b_i.nan, a_i.infinite, b_i.infinite})
      {1'b?, 1'b?, 1'b?, 1'b1, 1'b0, 1'b0, 1'b0}: out_sign = a_i.s;
      {1'b?, 1'b?, 1'b?, 1'b0, 1'b1, 1'b0, 1'b0}: out_sign = b_i.s;
      {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b1, 1'b0}: out_sign = a_i.s;
      {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b0, 1'b1}: out_sign = b_i.s;
      {1'b1, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = a_i.s && b_i.s;
      {1'b1, 1'b0, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = b_i.s;
      {1'b0, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = a_i.s;
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = b_i.s;
      {1'b0, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = a_i.s;
      default:                                       out_sign = 1'b0;
    endcase
  end

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNOPTFLAT */

  // ── Pipeline register ───────────────────────────────────

  logic        s1_sub_op_q;
  logic        s1_norm_out_q;
  logic        s1_out_is_zero_pre_q;
  logic        s1_out_is_inf_pre_q;
  logic        s1_out_is_nan_q;
  logic        s1_out_sign_q;
  logic [8:0]  s1_out_exp_pre_renorm_q;
  logic [27:0] s1_a_final_mant_q;
  logic [27:0] s1_b_final_mant_q;

  always_ff @(posedge clk_i) begin
    if (valid_i) begin
      s1_sub_op_q             <= sub_op;
      s1_norm_out_q           <= norm_out;
      s1_out_is_zero_pre_q    <= out_is_zero_pre;
      s1_out_is_inf_pre_q     <= out_is_inf_pre;
      s1_out_is_nan_q         <= out_is_nan;
      s1_out_sign_q           <= out_sign;
      s1_out_exp_pre_renorm_q <= out_exp_pre_renorm;
      s1_a_final_mant_q       <= a_final_mant;
      s1_b_final_mant_q       <= b_final_mant;
    end
  end

  // ── Stage 2: add + LZA normalize ────────────────────────

  logic [27:0] s2_out_added_mant;
  logic [24:0] s2_out_shift_mant;
  logic [23:0] s2_out_round_mant;

  logic [4:0]  s2_out_mant_renorm_shift_pred1;
  logic [4:0]  s2_out_mant_renorm_shift_pred2;
  logic [4:0]  s2_out_mant_renorm_shift_clamp_pred1;
  logic [4:0]  s2_out_mant_renorm_shift_clamp_pred2;

  logic [5:0]  s2_out_exp_renorm_delta_pred1;
  logic [5:0]  s2_out_exp_renorm_delta_pred2;
  logic        s2_out_overflow;
  logic        s2_out_underflow_pred1;
  logic        s2_out_underflow_pred2;
  logic        s2_out_underflow;

  logic [28:0] s2_ld_propagate;
  logic [28:0] s2_ld_generate;
  logic [28:0] s2_ld_kill;
  logic [27:1] s2_ld_indicator;
  logic [4:0]  s2_ld_prediction;
  logic        s2_ld_pred_down;

  /* verilator lint_off UNUSEDSIGNAL */ // Partial bit usage in LZA intermediates

  always_comb begin
    s2_out_added_mant = s1_a_final_mant_q + s1_b_final_mant_q + {27'b0, s1_sub_op_q};

    for (int i = 0; i < 28; i++) begin
      s2_ld_generate[i]  =  s1_a_final_mant_q[i] &  s1_b_final_mant_q[i];
      s2_ld_propagate[i] =  s1_a_final_mant_q[i] ^  s1_b_final_mant_q[i];
      s2_ld_kill[i]      = ~s1_a_final_mant_q[i] & ~s1_b_final_mant_q[i];
    end
     s2_ld_generate[28] = 1'b0;
    // NOTE: Original uses combinational sub_op (from current inputs), not
    // the registered s1_sub_op_q. This is intentional for LDA prediction
    // and must match the original for cycle-accurate cosim.
    s2_ld_propagate[28] = sub_op;
         s2_ld_kill[28] = 1'b0;

    for (int i = 27; i > 0; i--)
      s2_ld_indicator[i] = ( s2_ld_propagate[i+1] & ((s2_ld_generate[i] & ~s2_ld_kill[i-1]) | (    s2_ld_kill[i] & ~s2_ld_generate[i-1])))
                         | (~s2_ld_propagate[i+1] & ((    s2_ld_kill[i] & ~s2_ld_kill[i-1]) | (s2_ld_generate[i] & ~s2_ld_generate[i-1])));

    s2_ld_prediction = 5'd0;
    for (int i = 1; i < 28; i++)
      if (s2_ld_indicator[i]) s2_ld_prediction = 5'($unsigned(i));

    priority case (1'b1)
      s2_ld_indicator[27]: s2_out_shift_mant = s2_out_added_mant[27:3];
      s2_ld_indicator[26]: s2_out_shift_mant = s2_out_added_mant[26:2];
      s2_ld_indicator[25]: s2_out_shift_mant = s2_out_added_mant[25:1];
      default:             s2_out_shift_mant = s2_out_added_mant[24:0];
    endcase

    s2_ld_pred_down = !s2_out_added_mant[s2_ld_prediction];

    s2_out_round_mant = s2_ld_pred_down ? s2_out_shift_mant[23:0] : s2_out_shift_mant[24:1];

    s2_out_mant_renorm_shift_pred1 = 5'd25 - s2_ld_prediction;
    s2_out_mant_renorm_shift_pred2 = 5'd25 - s2_ld_prediction - 5'd1;

    s2_out_mant_renorm_shift_clamp_pred1 = (!s1_norm_out_q || (|s2_ld_indicator[27:25])) ? '0 : s2_out_mant_renorm_shift_pred1;
    s2_out_mant_renorm_shift_clamp_pred2 = (!s1_norm_out_q || (|s2_ld_indicator[27:24])) ? '0 : s2_out_mant_renorm_shift_pred2;

    s2_out_exp_renorm_delta_pred1 = s1_norm_out_q ? (s2_out_added_mant[27] ? 6'd1 : 6'($signed({1'b0, s2_ld_prediction})       - $signed(7'd26))) : 6'd0;
    s2_out_exp_renorm_delta_pred2 = s1_norm_out_q ?                                  6'($signed({1'b0, s2_ld_prediction}) - $signed(7'd1) - $signed(7'd26))  : 6'd0;

    s2_out_overflow = (s1_out_exp_pre_renorm_q == 9'(8'hfe)) && s2_out_added_mant[27];

    s2_out_underflow_pred1 = s1_norm_out_q && !s2_ld_indicator[27] && (s1_out_exp_pre_renorm_q == 9'(9'd26 - {4'd0, s2_ld_prediction}));
    s2_out_underflow_pred2 = s1_norm_out_q                         && (s1_out_exp_pre_renorm_q == 9'(9'd26 - {4'd0, s2_ld_prediction} - 9'd1));

    s2_out_underflow = s2_ld_pred_down ? s2_out_underflow_pred2 : s2_out_underflow_pred1;

    out_o.zero    =   s1_out_is_zero_pre_q
                  || (s2_ld_prediction == 5'd1) && (s2_out_round_mant[1:0] == 2'b0)
                  || (s2_ld_prediction == 5'd0) && !s2_out_round_mant[0]
                  ||  s2_out_underflow;

    out_o.infinite =  s1_out_is_inf_pre_q
                   || s2_out_overflow;

    out_o.nan = s1_out_is_nan_q;

    out_o.m_shift = s2_ld_pred_down ? s2_out_mant_renorm_shift_clamp_pred2
                                    : s2_out_mant_renorm_shift_clamp_pred1;
    out_o.e_delta = s2_ld_pred_down ? s2_out_exp_renorm_delta_pred2
                                    : s2_out_exp_renorm_delta_pred1;

    out_o.s = s1_out_sign_q;

    out_o.m = (s2_out_overflow || s2_out_underflow) ? '0 : s2_out_round_mant;

    out_o.e = out_o.zero     ? '0 :
             (s2_out_overflow ? 8'hff
                              : s1_out_exp_pre_renorm_q[7:0]);

    out_o.subnorm = 1'b0;
  end

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
