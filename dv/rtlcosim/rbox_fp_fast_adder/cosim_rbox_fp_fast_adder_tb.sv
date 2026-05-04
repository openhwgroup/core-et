// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_fp_fast_adder vs new rbox_fp_fast_adder.
//
// Both the original rbox_fp_fast_adder and rbox_fp_fast_adder_shifter are
// inlined here (renamed with _orig suffix) to avoid module name collisions
// with the new RTL.
//
// The module is purely combinational (no clock, no valid). We still provide
// clk_i because CosimCtrl/SimCtrl require it for the tick() mechanism.

`include "soc.vh"

module cosim_rbox_fp_fast_adder_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus — internal depth fields for operand A
  input  logic              a_infinite,
  input  logic              a_nan,
  input  logic              a_zero,
  input  logic              a_subnorm,
  input  logic [7:0]        a_e,
  input  logic              a_s,
  input  logic [23:0]       a_m,
  input  logic [5:0]        a_e_delta,
  input  logic [4:0]        a_m_shift,

  // Shared stimulus — internal depth fields for operand B
  input  logic              b_infinite,
  input  logic              b_nan,
  input  logic              b_zero,
  input  logic              b_subnorm,
  input  logic [7:0]        b_e,
  input  logic              b_s,
  input  logic [23:0]       b_m,
  input  logic [5:0]        b_e_delta,
  input  logic [4:0]        b_m_shift,

  // New module outputs (individual fields for C++ access)
  output logic              new_infinite,
  output logic              new_nan,
  output logic              new_zero,
  output logic              new_subnorm,
  output logic [7:0]        new_e,
  output logic              new_s,
  output logic [23:0]       new_m,
  output logic [5:0]        new_e_delta,
  output logic [4:0]        new_m_shift,

  // Original module outputs
  output logic              orig_infinite,
  output logic              orig_nan,
  output logic              orig_zero,
  output logic              orig_subnorm,
  output logic [7:0]        orig_e,
  output logic              orig_s,
  output logic [23:0]       orig_m,
  output logic [5:0]        orig_e_delta,
  output logic [4:0]        orig_m_shift
);

  import rbox_pkg::*;

  // Pack fields for new module
  internal_depth_t a_packed, b_packed, new_out;
  assign a_packed = '{infinite: a_infinite, nan: a_nan, zero: a_zero, subnorm: a_subnorm,
                      e: a_e, s: a_s, m: a_m, e_delta: a_e_delta, m_shift: a_m_shift};
  assign b_packed = '{infinite: b_infinite, nan: b_nan, zero: b_zero, subnorm: b_subnorm,
                      e: b_e, s: b_s, m: b_m, e_delta: b_e_delta, m_shift: b_m_shift};

  assign new_infinite = new_out.infinite;
  assign new_nan      = new_out.nan;
  assign new_zero     = new_out.zero;
  assign new_subnorm  = new_out.subnorm;
  assign new_e        = new_out.e;
  assign new_s        = new_out.s;
  assign new_m        = new_out.m;
  assign new_e_delta  = new_out.e_delta;
  assign new_m_shift  = new_out.m_shift;

  // Pack fields for original module
  rbox_internal_depth_t orig_a, orig_b, orig_out;
  assign orig_a = '{infinite: a_infinite, nan: a_nan, zero: a_zero, subnorm: a_subnorm,
                    e: a_e, s: a_s, m: a_m, e_delta: a_e_delta, m_shift: a_m_shift};
  assign orig_b = '{infinite: b_infinite, nan: b_nan, zero: b_zero, subnorm: b_subnorm,
                    e: b_e, s: b_s, m: b_m, e_delta: b_e_delta, m_shift: b_m_shift};

  assign orig_infinite = orig_out.infinite;
  assign orig_nan      = orig_out.nan;
  assign orig_zero     = orig_out.zero;
  assign orig_subnorm  = orig_out.subnorm;
  assign orig_e        = orig_out.e;
  assign orig_s        = orig_out.s;
  assign orig_m        = orig_out.m;
  assign orig_e_delta  = orig_out.e_delta;
  assign orig_m_shift  = orig_out.m_shift;

  // ── New module ────────────────────────────────────────

  rbox_fp_fast_adder u_new (
    .a_i   (a_packed),
    .b_i   (b_packed),
    .out_o (new_out)
  );

  // ── Original module ───────────────────────────────────

  rbox_fp_fast_adder_orig u_orig (
    .a   (orig_a),
    .b   (orig_b),
    .out (orig_out)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Inlined copy of original rbox_fp_fast_adder_shifter (renamed)
// ════════════════════════════════════════════════════════════

module rbox_fp_fast_adder_shifter_orig
#(
    parameter          SZ = 16,
    parameter SIGN_EXTEND = 0,
    parameter   SHIFT_SZ  = 4
)
(
    input  logic [SZ-1:0]     in,
    input  logic [SHIFT_SZ:0] shift,
    output logic [SZ-1:0]     out,
    output logic              sticky
);
logic                shift_right;
logic [SHIFT_SZ-1:0] shift_value;
logic [SHIFT_SZ-1:0] shift_pos;
logic [3*SZ-1:0]     out_left_shift;
logic [SZ-1:0]       out_right_shift;
always_comb begin
    shift_right = shift[SHIFT_SZ];
    shift_value = shift[SHIFT_SZ-1:0];
    out_left_shift = {{SZ{SIGN_EXTEND ? in[SZ-1] : 1'b0}}, in, {SZ{1'b0}}} >> shift_value;
    out_right_shift = in << shift_value;
    out    = shift_right ? out_right_shift :  out_left_shift[2*SZ-1:SZ];
    sticky = shift_right ? 1'b0            : |out_left_shift[SZ-1:0];
end
endmodule

// ════════════════════════════════════════════════════════════
// Inlined copy of original rbox_fp_fast_adder (renamed)
// ════════════════════════════════════════════════════════════

module rbox_fp_fast_adder_orig (
    input  rbox_internal_depth_t  a,
    input  rbox_internal_depth_t  b,
    output rbox_internal_depth_t  out
);
logic [25:0] a_mant, b_mant;
logic a_sticky, b_sticky;
logic [25:0] a_shift_mant, b_shift_mant;
logic [26:0] a_grs_mant, b_grs_mant;
logic [27:0] a_final_mant, b_final_mant, out_added_mant;
logic [24:0] out_shift_mant;
logic [23:0] out_round_mant;
logic [8:0] a_mant_shift, a_mant_shift_p1, a_mant_shift_p2, a_mant_shift_p3;
logic [8:0] b_mant_shift, b_mant_shift_p1, b_mant_shift_p2, b_mant_shift_p3;
logic [4:0] out_mant_renorm_shift_pred1, out_mant_renorm_shift_pred2;
logic [4:0] out_mant_renorm_shift_clamp_pred1, out_mant_renorm_shift_clamp_pred2;
logic [7:0] a_exp_with_delta, b_exp_with_delta;
logic [8:0] out_exp_pre_renorm;
logic [5:0] out_exp_renorm_delta_pred1, out_exp_renorm_delta_pred2;
logic out_overflow, out_underflow_pred1, out_underflow_pred2, out_underflow;
logic sub_op, a_exp_is_greater, norm_out, out_is_zero_pre, out_is_inf_pre;
logic [28:0] ld_propagate, ld_generate, ld_kill;
logic [27:1] ld_indicator;
logic [4:0] ld_prediction;
logic ld_pred_down, ld_pred_up;

rbox_fp_fast_adder_shifter_orig #(.SZ(26), .SIGN_EXTEND(0), .SHIFT_SZ(8))
a_shifter (.in(a_mant), .shift(a_mant_shift), .out(a_shift_mant), .sticky(a_sticky));
rbox_fp_fast_adder_shifter_orig #(.SZ(26), .SIGN_EXTEND(0), .SHIFT_SZ(8))
b_shifter (.in(b_mant), .shift(b_mant_shift), .out(b_shift_mant), .sticky(b_sticky));

always_comb begin
    sub_op = a.s ^ b.s && !a.zero && !b.zero;
    a_mant = {a.m, 2'b0};
    b_mant = {b.m, 2'b0};
    a_exp_with_delta = a.e + `SX(8, a.e_delta);
    b_exp_with_delta = b.e + `SX(8, b.e_delta);
    a_exp_is_greater = a_exp_with_delta > b_exp_with_delta;
    a_mant_shift_p1 =                                                 `ZX(9, a.m_shift);
    a_mant_shift_p2 =   `ZX(9, b_exp_with_delta - a_exp_with_delta) - `ZX(9, a.m_shift);
    a_mant_shift_p3 = - `ZX(9, b_exp_with_delta - a_exp_with_delta) + `ZX(9, a.m_shift);
    b_mant_shift_p1 =                                                 `ZX(9, b.m_shift);
    b_mant_shift_p2 =   `ZX(9, a_exp_with_delta - b_exp_with_delta) - `ZX(9, b.m_shift);
    b_mant_shift_p3 = - `ZX(9, a_exp_with_delta - b_exp_with_delta) + `ZX(9, b.m_shift);
    a_mant_shift = a_exp_is_greater ? a_mant_shift_p1
                                    : (a_mant_shift_p2[8] ? {1'b1, a_mant_shift_p3[7:0]} : a_mant_shift_p2);
    b_mant_shift = a_exp_is_greater ? (b_mant_shift_p2[8] ? {1'b1, b_mant_shift_p3[7:0]} : b_mant_shift_p2)
                                    : b_mant_shift_p1;
    out_is_zero_pre = a.zero && b.zero;
    out.nan = a.nan || b.nan;
    out_is_inf_pre = a.infinite || b.infinite;
    a_grs_mant = {a_shift_mant, a_sticky};
    b_grs_mant = {b_shift_mant, b_sticky};
    casez ({a.zero, b.zero, out_is_inf_pre, out.nan, sub_op, a_exp_is_greater})
        {1'b1, 1'b0, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               {1'b0,  b.m, 3'b0 }};
        {1'b0, 1'b1, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, {1'b0,  a.m, 3'b0 }, 28'h0              };
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b0,  b_grs_mant}};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b1, ~a_grs_mant}, {1'b0,  b_grs_mant}};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b1}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b1, ~b_grs_mant}};
        default:                                {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               28'h0              };
    endcase
    out_added_mant = a_final_mant + b_final_mant + sub_op;
    for(integer i = 0; i < 28; i++) begin
        ld_generate[i]  =  a_final_mant[i] &  b_final_mant[i];
        ld_propagate[i] =  a_final_mant[i] ^  b_final_mant[i];
        ld_kill[i]      = ~a_final_mant[i] & ~b_final_mant[i];
    end
     ld_generate[28] = 1'b0;
    ld_propagate[28] = sub_op;
         ld_kill[28] = 1'b0;
    for (integer i = 27; i > 0; i--)
        ld_indicator[i] = ( ld_propagate[i+1] & ((ld_generate[i] & ~ld_kill[i-1]) | (    ld_kill[i] & ~ld_generate[i-1])))
                        | (~ld_propagate[i+1] & ((    ld_kill[i] & ~ld_kill[i-1]) | (ld_generate[i] & ~ld_generate[i-1])));
    ld_prediction = 0;
    for (integer i = 1; i < 28; i++)
        if (ld_indicator[i]) ld_prediction = $unsigned(i);
    case (1'b1)
        ld_indicator[27]: out_shift_mant = out_added_mant[27:3];
        ld_indicator[26]: out_shift_mant = out_added_mant[26:2];
        ld_indicator[25]: out_shift_mant = out_added_mant[25:1];
        default:          out_shift_mant = out_added_mant[24:0];
    endcase
    ld_pred_down = !out_added_mant[ld_prediction];
    out_round_mant = ld_pred_down ? out_shift_mant[23:0] : out_shift_mant[24:1];
    casez ({a.zero, b.zero, out_is_inf_pre, out.nan, a_exp_is_greater})
        {1'b1, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = '0;
        {1'b1, 1'b0, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
        {1'b0, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b1}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
        {1'b0, 1'b0, 1'b1, 1'b0, 1'b?}: out_exp_pre_renorm = '1;
        {1'b0, 1'b0, 1'b0, 1'b1, 1'b?}: out_exp_pre_renorm = '1;
        default:                          out_exp_pre_renorm = '0;
    endcase
    out_mant_renorm_shift_pred1 = 'd25 - ld_prediction;
    out_mant_renorm_shift_pred2 = 'd25 - ld_prediction - 'd1;
    out_mant_renorm_shift_clamp_pred1 = (!norm_out || (|ld_indicator[27:25])) ? '0 : out_mant_renorm_shift_pred1;
    out_mant_renorm_shift_clamp_pred2 = (!norm_out || (|ld_indicator[27:24])) ? '0 : out_mant_renorm_shift_pred2;
    out_exp_renorm_delta_pred1 = norm_out ? (out_added_mant[27] ? 'd1 : (ld_prediction       - 'd26)) : 'd0;
    out_exp_renorm_delta_pred2 = norm_out ?                             (ld_prediction - 'd1 - 'd26)  : 'd0;
    out_overflow = (out_exp_pre_renorm == 8'hfe) && out_added_mant[27];
    out_underflow_pred1 = norm_out && !ld_indicator[27] && (out_exp_pre_renorm == ('d26 - ld_prediction      ));
    out_underflow_pred2 = norm_out                      && (out_exp_pre_renorm == ('d26 - ld_prediction - 'd1));
    out_underflow = ld_pred_down ? out_underflow_pred2 : out_underflow_pred1;
    casez({a.zero, b.zero, a_exp_is_greater, a.nan, b.nan, a.infinite, b.infinite})
        {1'b?, 1'b?, 1'b?, 1'b1, 1'b0, 1'b0, 1'b0}: out.s = a.s;
        {1'b?, 1'b?, 1'b?, 1'b0, 1'b1, 1'b0, 1'b0}: out.s = b.s;
        {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b1, 1'b0}: out.s = a.s;
        {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b0, 1'b1}: out.s = b.s;
        {1'b1, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out.s = a.s && b.s;
        {1'b1, 1'b0, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out.s = b.s;
        {1'b0, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out.s = a.s;
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out.s = b.s;
        {1'b0, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0}: out.s = a.s;
        default:                                       out.s = 1'b0;
    endcase
    out.zero    = out_is_zero_pre
                || (ld_prediction == 1) && (out_round_mant[1:0] == 2'b0)
                || (ld_prediction == 0) && !out_round_mant[0]
                ||  out_underflow;
    out.infinite = out_is_inf_pre || out_overflow;
    out.m_shift = ld_pred_down ? out_mant_renorm_shift_clamp_pred2 : out_mant_renorm_shift_clamp_pred1;
    out.e_delta = ld_pred_down ? out_exp_renorm_delta_pred2 : out_exp_renorm_delta_pred1;
    out.m = (out_overflow || out_underflow) ? '0 : out_round_mant;
    out.e = out.zero ? '0 : (out_overflow ? 8'hff : out_exp_pre_renorm[7:0]);
    out.subnorm = 1'b0;
end
endmodule
