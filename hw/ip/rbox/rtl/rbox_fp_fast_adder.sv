// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX combinational IEEE754 floating-point adder.
//
// Inputs use the internal depth representation (not fully renormalized).
// Input subnormals must have been flushed to zero.
// Output rounding is Round-To-Zero. Output subnormals are flushed to zero.
// Output is not fully renormalized — renormalization shift and exponent
// delta are provided for the next stage.
//
// Pure combinational logic.

module rbox_fp_fast_adder
  import rbox_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */ // internal_depth_t.subnorm unused by adder
  input  internal_depth_t a_i,
  input  internal_depth_t b_i,
  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_off UNOPTFLAT */   // out_o fields assigned from separate combinational paths — no real circular dependency
  output internal_depth_t out_o
  /* verilator lint_on UNOPTFLAT */
);

  /* verilator lint_off UNUSEDSIGNAL */ // Partial bit usage in FP intermediates
  /* verilator lint_off UNOPTFLAT */   // Shifter outputs feed back into same always_comb — acyclic per-field

  // Add Guard and Round bits
  logic [25:0] a_mant;
  logic [25:0] b_mant;
  logic        a_sticky;
  logic        b_sticky;
  logic [25:0] a_shift_mant;
  logic [25:0] b_shift_mant;

  // Add Sticky bit
  logic [26:0] a_grs_mant;
  logic [26:0] b_grs_mant;

  // Add 1 bit for addition carry
  logic [27:0] a_final_mant;
  logic [27:0] b_final_mant;
  logic [27:0] out_added_mant;
  logic [24:0] out_shift_mant;
  logic [23:0] out_round_mant;

  logic [8:0]  a_mant_shift;
  logic [8:0]  a_mant_shift_p1;
  logic [8:0]  a_mant_shift_p2;
  logic [8:0]  a_mant_shift_p3;
  logic [8:0]  b_mant_shift;
  logic [8:0]  b_mant_shift_p1;
  logic [8:0]  b_mant_shift_p2;
  logic [8:0]  b_mant_shift_p3;

  logic [4:0]  out_mant_renorm_shift_pred1;
  logic [4:0]  out_mant_renorm_shift_pred2;
  logic [4:0]  out_mant_renorm_shift_clamp_pred1;
  logic [4:0]  out_mant_renorm_shift_clamp_pred2;

  logic [7:0]  a_exp_with_delta;
  logic [7:0]  b_exp_with_delta;
  logic [8:0]  out_exp_pre_renorm;
  logic [5:0]  out_exp_renorm_delta_pred1;
  logic [5:0]  out_exp_renorm_delta_pred2;
  logic        out_overflow;
  logic        out_underflow_pred1;
  logic        out_underflow_pred2;
  logic        out_underflow;

  logic        sub_op;
  logic        a_exp_is_greater;
  logic        norm_out;
  logic        out_is_zero_pre;
  logic        out_is_inf_pre;

  logic [28:0] ld_propagate;
  logic [28:0] ld_generate;
  logic [28:0] ld_kill;
  logic [27:1] ld_indicator;
  logic [4:0]  ld_prediction;
  logic        ld_pred_down;

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
    out_o.nan       = a_i.nan      || b_i.nan;
    out_is_inf_pre  = a_i.infinite || b_i.infinite;

    a_grs_mant = {a_shift_mant, a_sticky};
    b_grs_mant = {b_shift_mant, b_sticky};

    unique casez ({a_i.zero, b_i.zero, out_is_inf_pre, out_o.nan, sub_op, a_exp_is_greater})
      {1'b1, 1'b0, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               {1'b0,  b_i.m, 3'b0}};
      {1'b0, 1'b1, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, {1'b0,  a_i.m, 3'b0}, 28'h0             };
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b0,  b_grs_mant}};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b1, ~a_grs_mant}, {1'b0,  b_grs_mant}};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b1}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b1, ~b_grs_mant}};
      default:                                {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               28'h0              };
    endcase

    out_added_mant = a_final_mant + b_final_mant + {27'b0, sub_op};

    for (int i = 0; i < 28; i++) begin
      ld_generate[i]  =  a_final_mant[i] &  b_final_mant[i];
      ld_propagate[i] =  a_final_mant[i] ^  b_final_mant[i];
      ld_kill[i]      = ~a_final_mant[i] & ~b_final_mant[i];
    end
     ld_generate[28] = 1'b0;
    ld_propagate[28] = sub_op;
         ld_kill[28] = 1'b0;

    for (int i = 27; i > 0; i--)
      ld_indicator[i] = ( ld_propagate[i+1] & ((ld_generate[i] & ~ld_kill[i-1]) | (    ld_kill[i] & ~ld_generate[i-1])))
                      | (~ld_propagate[i+1] & ((    ld_kill[i] & ~ld_kill[i-1]) | (ld_generate[i] & ~ld_generate[i-1])));

    ld_prediction = 5'd0;
    for (int i = 1; i < 28; i++)
      if (ld_indicator[i]) ld_prediction = 5'($unsigned(i));

    priority case (1'b1)
      ld_indicator[27]: out_shift_mant = out_added_mant[27:3];
      ld_indicator[26]: out_shift_mant = out_added_mant[26:2];
      ld_indicator[25]: out_shift_mant = out_added_mant[25:1];
      default:          out_shift_mant = out_added_mant[24:0];
    endcase

    ld_pred_down = !out_added_mant[ld_prediction];

    out_round_mant = ld_pred_down ? out_shift_mant[23:0] : out_shift_mant[24:1];

    unique casez ({a_i.zero, b_i.zero, out_is_inf_pre, out_o.nan, a_exp_is_greater})
      {1'b1, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = '0;
      {1'b1, 1'b0, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
      {1'b0, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b1}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
      {1'b0, 1'b0, 1'b1, 1'b0, 1'b?}: out_exp_pre_renorm = '1;
      {1'b0, 1'b0, 1'b0, 1'b1, 1'b?}: out_exp_pre_renorm = '1;
      default:                          out_exp_pre_renorm = '0;
    endcase

    out_mant_renorm_shift_pred1 = 5'd25 - ld_prediction;
    out_mant_renorm_shift_pred2 = 5'd25 - ld_prediction - 5'd1;

    out_mant_renorm_shift_clamp_pred1 = (!norm_out || (|ld_indicator[27:25])) ? '0 : out_mant_renorm_shift_pred1;
    out_mant_renorm_shift_clamp_pred2 = (!norm_out || (|ld_indicator[27:24])) ? '0 : out_mant_renorm_shift_pred2;

    out_exp_renorm_delta_pred1 = norm_out ? (out_added_mant[27] ? 6'd1 : 6'($signed({1'b0, ld_prediction})       - $signed(7'd26))) : 6'd0;
    out_exp_renorm_delta_pred2 = norm_out ?                              6'($signed({1'b0, ld_prediction}) - $signed(7'd1) - $signed(7'd26))  : 6'd0;

    out_overflow = (out_exp_pre_renorm == 9'(8'hfe)) && out_added_mant[27];

    out_underflow_pred1 = norm_out && !ld_indicator[27] && (out_exp_pre_renorm == 9'(9'd26 - {4'd0, ld_prediction}));
    out_underflow_pred2 = norm_out                      && (out_exp_pre_renorm == 9'(9'd26 - {4'd0, ld_prediction} - 9'd1));

    out_underflow = ld_pred_down ? out_underflow_pred2 : out_underflow_pred1;

    unique casez ({a_i.zero, b_i.zero, a_exp_is_greater, a_i.nan, b_i.nan, a_i.infinite, b_i.infinite})
      {1'b?, 1'b?, 1'b?, 1'b1, 1'b0, 1'b0, 1'b0}: out_o.s = a_i.s;
      {1'b?, 1'b?, 1'b?, 1'b0, 1'b1, 1'b0, 1'b0}: out_o.s = b_i.s;
      {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b1, 1'b0}: out_o.s = a_i.s;
      {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b0, 1'b1}: out_o.s = b_i.s;
      {1'b1, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_o.s = a_i.s && b_i.s;
      {1'b1, 1'b0, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_o.s = b_i.s;
      {1'b0, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_o.s = a_i.s;
      {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out_o.s = b_i.s;
      {1'b0, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0}: out_o.s = a_i.s;
      default:                                       out_o.s = 1'b0;
    endcase

    out_o.zero    =   out_is_zero_pre
                  || (ld_prediction == 5'd1) && (out_round_mant[1:0] == 2'b0)
                  || (ld_prediction == 5'd0) && !out_round_mant[0]
                  ||  out_underflow;

    out_o.infinite =  out_is_inf_pre
                   || out_overflow;

    out_o.m_shift = ld_pred_down ? out_mant_renorm_shift_clamp_pred2
                                 : out_mant_renorm_shift_clamp_pred1;
    out_o.e_delta = ld_pred_down ? out_exp_renorm_delta_pred2
                                 : out_exp_renorm_delta_pred1;

    out_o.m = (out_overflow || out_underflow) ? '0 : out_round_mant;

    out_o.e = out_o.zero  ? '0 :
             (out_overflow ? 8'hff
                           : out_exp_pre_renorm[7:0]);

    out_o.subnorm = 1'b0;
  end

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNOPTFLAT */

endmodule
