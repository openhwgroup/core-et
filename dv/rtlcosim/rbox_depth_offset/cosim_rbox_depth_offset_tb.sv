// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_depth_offset vs new rbox_depth_offset.
//
// The original module's sub-modules (rbox_fp_adder_2stg, rbox_fp_fast_adder_shifter)
// are resolved via -y library path. We inline a renamed copy of only the
// rbox_depth_offset top module to avoid name collisions.

`include "soc.vh"

module cosim_rbox_depth_offset_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus
  input  logic [31:0]       depth_coef_a,
  input  logic [31:0]       depth_coef_b,
  input  logic [5:0]        step_x,
  input  logic [5:0]        step_y,

  input  logic              valid_stg1,
  input  logic              valid_stg2,

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

  // ── New module ────────────────────────────────────────

  internal_depth_t new_out;

  assign new_infinite = new_out.infinite;
  assign new_nan      = new_out.nan;
  assign new_zero     = new_out.zero;
  assign new_subnorm  = new_out.subnorm;
  assign new_e        = new_out.e;
  assign new_s        = new_out.s;
  assign new_m        = new_out.m;
  assign new_e_delta  = new_out.e_delta;
  assign new_m_shift  = new_out.m_shift;

  rbox_depth_offset u_new (
    .clk_i          (clk_i),
    .depth_coef_a_i (depth_coef_a),
    .depth_coef_b_i (depth_coef_b),
    .step_x_i       (step_x),
    .step_y_i       (step_y),
    .valid_stg1_i   (valid_stg1),
    .valid_stg2_i   (valid_stg2),
    .depth_offset_o (new_out)
  );

  // ── Original module ───────────────────────────────────

  rbox_internal_depth_t orig_out;

  assign orig_infinite = orig_out.infinite;
  assign orig_nan      = orig_out.nan;
  assign orig_zero     = orig_out.zero;
  assign orig_subnorm  = orig_out.subnorm;
  assign orig_e        = orig_out.e;
  assign orig_s        = orig_out.s;
  assign orig_m        = orig_out.m;
  assign orig_e_delta  = orig_out.e_delta;
  assign orig_m_shift  = orig_out.m_shift;

  rbox_depth_offset_orig u_orig (
    .clock       (clk_i),
    .depth_coef_a(depth_coef_a),
    .depth_coef_b(depth_coef_b),
    .step_x      (step_x),
    .step_y      (step_y),
    .valid_stg1  (valid_stg1),
    .valid_stg2  (valid_stg2),
    .depth_offset(orig_out)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Inlined copy of original rbox_depth_offset (renamed)
// ════════════════════════════════════════════════════════════

module rbox_depth_offset_orig
(
    input  logic                 clock,

    input  logic[31:0]           depth_coef_a,
    input  logic[31:0]           depth_coef_b,
    input  rbox_step_t           step_x,
    input  rbox_step_t           step_y,

    input  logic                 valid_stg1,
    input  logic                 valid_stg2,

    output rbox_internal_depth_t depth_offset
);

localparam OFFSET_SZ     = `RBOX_STEP_SZ + 24;
localparam OFFSET_INT_SZ = `RBOX_STEP_INT_SZ + 1;
localparam OFFSET_INT_R  = $clog2(OFFSET_INT_SZ) + 1;

rbox_internal_depth_t    s0_depth_coef_a;
rbox_internal_depth_t    s0_depth_coef_b;
rbox_internal_depth_t    s0_offset_x;
rbox_internal_depth_t    s0_offset_y;
logic [OFFSET_SZ-1:0]    s0_offset_x_mul_mant;
logic [OFFSET_SZ-1:0]    s0_offset_y_mul_mant;
logic [OFFSET_SZ-1:0]    s0_offset_x_shift_mant;
logic [OFFSET_SZ-1:0]    s0_offset_y_shift_mant;
logic [OFFSET_INT_R-1:0] s0_step_x_lead;
logic [OFFSET_INT_R-1:0] s0_step_y_lead;
logic [OFFSET_INT_R-1:0] s0_offset_x_mant_shift;
logic [OFFSET_INT_R-1:0] s0_offset_y_mant_shift;
logic                    s0_offset_x_underflow;
logic                    s0_offset_y_underflow;
logic                    s0_offset_x_overflow;
logic                    s0_offset_y_overflow;
rbox_internal_depth_t    s1_offset_x_ff;
rbox_internal_depth_t    s1_offset_y_ff;

always_comb
begin
    // Convert coefficients to internal float point format
    s0_depth_coef_a = f32_to_depth_internal_f(depth_coef_a);
    s0_depth_coef_b = f32_to_depth_internal_f(depth_coef_b);

    // Get mantissa for step x coefficient
    s0_offset_x_mul_mant = step_x * s0_depth_coef_a.m;
    s0_offset_y_mul_mant = step_y * s0_depth_coef_b.m;

    // Estimate the leading zero for the result of the multiplication
    s0_step_x_lead = `RBOX_STEP_SZ - 1;
    for (integer b = 0; b < `RBOX_STEP_SZ; b++)
        if (step_x[b]) s0_step_x_lead = `RBOX_STEP_SZ - 1 - b;
    s0_step_y_lead = `RBOX_STEP_SZ - 1;
    for (integer b = 0; b < `RBOX_STEP_SZ; b++)
        if (step_y[b]) s0_step_y_lead = `RBOX_STEP_SZ - 1 - b;

    // Correct the leading zero for the result of the multiplication
    s0_offset_x_mant_shift = s0_step_x_lead;
    s0_offset_y_mant_shift = s0_step_y_lead;
    if (!s0_offset_x_mul_mant[OFFSET_SZ - 1 - s0_step_x_lead]) s0_offset_x_mant_shift++;
    if (!s0_offset_y_mul_mant[OFFSET_SZ - 1 - s0_step_y_lead]) s0_offset_y_mant_shift++;

    s0_offset_x_shift_mant = s0_offset_x_mul_mant << s0_offset_x_mant_shift;
    s0_offset_y_shift_mant = s0_offset_y_mul_mant << s0_offset_y_mant_shift;

    s0_offset_x_overflow = (s0_offset_x_mant_shift <= `RBOX_STEP_INT_SZ) && (s0_depth_coef_a.e > (8'hfe - s0_offset_x_mant_shift + `RBOX_STEP_INT_SZ));
    s0_offset_y_overflow = (s0_offset_y_mant_shift <= `RBOX_STEP_INT_SZ) && (s0_depth_coef_b.e > (8'hfe - s0_offset_y_mant_shift + `RBOX_STEP_INT_SZ));

    s0_offset_x_underflow = (s0_offset_x_mant_shift > `RBOX_STEP_INT_SZ) && (s0_depth_coef_a.e <= (s0_offset_x_mant_shift - `RBOX_STEP_INT_SZ));
    s0_offset_y_underflow = (s0_offset_y_mant_shift > `RBOX_STEP_INT_SZ) && (s0_depth_coef_b.e <= (s0_offset_y_mant_shift - `RBOX_STEP_INT_SZ));

    // If step is 0 or coefficients are 0 the offset is 0. Flush subnormals to zero.
    s0_offset_x.zero = (step_x == '0) || s0_depth_coef_a.zero || s0_depth_coef_a.subnorm || s0_offset_x_underflow;
    s0_offset_y.zero = (step_y == '0) || s0_depth_coef_b.zero || s0_depth_coef_b.subnorm || s0_offset_y_underflow;

    s0_offset_x.e = s0_depth_coef_a.e - s0_offset_x_mant_shift + `RBOX_STEP_INT_SZ;
    s0_offset_y.e = s0_depth_coef_b.e - s0_offset_y_mant_shift + `RBOX_STEP_INT_SZ;

    s0_offset_x.infinite = s0_depth_coef_a.infinite || s0_offset_x_overflow;
    s0_offset_y.infinite = s0_depth_coef_b.infinite || s0_offset_y_overflow;

    s0_offset_x.nan = s0_depth_coef_a.nan;
    s0_offset_y.nan = s0_depth_coef_b.nan;

    s0_offset_x.s = s0_depth_coef_a.s && !s0_offset_x.zero;
    s0_offset_y.s = s0_depth_coef_b.s && !s0_offset_y.zero;

    s0_offset_x.m = s0_offset_x_shift_mant[OFFSET_SZ-1 -: 24];
    s0_offset_y.m = s0_offset_y_shift_mant[OFFSET_SZ-1 -: 24];

    s0_offset_x.subnorm = 1'b0;
    s0_offset_y.subnorm = 1'b0;

    s0_offset_x.e_delta = '0;
    s0_offset_y.e_delta = '0;

    s0_offset_x.m_shift = '0;
    s0_offset_y.m_shift = '0;
end

`EN_FF(clock, valid_stg1, s1_offset_x_ff, s0_offset_x)
`EN_FF(clock, valid_stg1, s1_offset_y_ff, s0_offset_y)

rbox_fp_adder_2stg_orig
depth_offset_adder (
                    .clock (clock         ),
                    .a     (s1_offset_x_ff),
                    .b     (s1_offset_y_ff),
                    .valid (valid_stg2    ),
                    .out   (depth_offset  )
                   );

endmodule

// ════════════════════════════════════════════════════════════
// Inlined copy of original rbox_fp_adder_2stg (renamed)
// Includes the shifter sub-module it depends on.
// ════════════════════════════════════════════════════════════

module rbox_fp_fast_adder_shifter_for_offset
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

module rbox_fp_adder_2stg_orig (
    input  clock,
    input  rbox_internal_depth_t  a,
    input  rbox_internal_depth_t  b,
    input  logic                  valid,
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
logic sub_op, a_exp_is_greater, norm_out, out_is_zero_pre, out_is_inf_pre, out_is_nan, out_sign;
logic [28:0] ld_propagate, ld_generate, ld_kill;
logic [27:1] ld_indicator;
logic [4:0] ld_prediction;
logic ld_pred_down, ld_pred_up;

rbox_fp_fast_adder_shifter_for_offset #(.SZ(26), .SIGN_EXTEND(0), .SHIFT_SZ(8))
a_shifter (.in(a_mant), .shift(a_mant_shift), .out(a_shift_mant), .sticky(a_sticky));
rbox_fp_fast_adder_shifter_for_offset #(.SZ(26), .SIGN_EXTEND(0), .SHIFT_SZ(8))
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
    out_is_nan = a.nan || b.nan;
    out_is_inf_pre = a.infinite || b.infinite;
    a_grs_mant = {a_shift_mant, a_sticky};
    b_grs_mant = {b_shift_mant, b_sticky};
    casez ({a.zero, b.zero, out_is_inf_pre, out_is_nan, sub_op, a_exp_is_greater})
        {1'b1, 1'b0, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               {1'b0,  b.m, 3'b0 }};
        {1'b0, 1'b1, 1'b0, 1'b0, 1'b?, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b0, {1'b0,  a.m, 3'b0 }, 28'h0              };
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b?}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b0,  b_grs_mant}};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b1, ~a_grs_mant}, {1'b0,  b_grs_mant}};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b1}: {norm_out, a_final_mant, b_final_mant} = {1'b1, {1'b0,  a_grs_mant}, {1'b1, ~b_grs_mant}};
        default:                                {norm_out, a_final_mant, b_final_mant} = {1'b0, 28'h0,               28'h0              };
    endcase
    casez ({a.zero, b.zero, out_is_inf_pre, out_is_nan, a_exp_is_greater})
        {1'b1, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = '0;
        {1'b1, 1'b0, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
        {1'b0, 1'b1, 1'b?, 1'b?, 1'b?}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b1}: out_exp_pre_renorm = {1'b0, a_exp_with_delta};
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out_exp_pre_renorm = {1'b0, b_exp_with_delta};
        {1'b0, 1'b0, 1'b1, 1'b0, 1'b?}: out_exp_pre_renorm = '1;
        {1'b0, 1'b0, 1'b0, 1'b1, 1'b?}: out_exp_pre_renorm = '1;
        default:                          out_exp_pre_renorm = '0;
    endcase
    casez({a.zero, b.zero, a_exp_is_greater, a.nan, b.nan, a.infinite, b.infinite})
        {1'b?, 1'b?, 1'b?, 1'b1, 1'b0, 1'b0, 1'b0}: out_sign = a.s;
        {1'b?, 1'b?, 1'b?, 1'b0, 1'b1, 1'b0, 1'b0}: out_sign = b.s;
        {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b1, 1'b0}: out_sign = a.s;
        {1'b?, 1'b?, 1'b?, 1'b0, 1'b0, 1'b0, 1'b1}: out_sign = b.s;
        {1'b1, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = a.s && b.s;
        {1'b1, 1'b0, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = b.s;
        {1'b0, 1'b1, 1'b?, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = a.s;
        {1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = b.s;
        {1'b0, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0}: out_sign = a.s;
        default:                                       out_sign = 1'b0;
    endcase
end
logic s1_sub_op_ff, s1_norm_out_ff, s1_out_is_zero_pre_ff, s1_out_is_inf_pre_ff;
logic s1_out_is_nan_ff, s1_out_sign_ff;
logic [8:0] s1_out_exp_pre_renorm_ff;
logic [27:0] s1_a_final_mant_ff, s1_b_final_mant_ff;
`EN_FF(clock, valid, s1_sub_op_ff,             sub_op            )
`EN_FF(clock, valid, s1_norm_out_ff,           norm_out          )
`EN_FF(clock, valid, s1_out_is_zero_pre_ff,    out_is_zero_pre   )
`EN_FF(clock, valid, s1_out_is_inf_pre_ff,     out_is_inf_pre    )
`EN_FF(clock, valid, s1_out_is_nan_ff,         out_is_nan        )
`EN_FF(clock, valid, s1_out_sign_ff,           out_sign          )
`EN_FF(clock, valid, s1_out_exp_pre_renorm_ff, out_exp_pre_renorm)
`EN_FF(clock, valid, s1_a_final_mant_ff,       a_final_mant      )
`EN_FF(clock, valid, s1_b_final_mant_ff,       b_final_mant      )
always_comb begin
    out_added_mant = s1_a_final_mant_ff + s1_b_final_mant_ff + s1_sub_op_ff;
    for(integer i = 0; i < 28; i++) begin
        ld_generate[i]  =  s1_a_final_mant_ff[i] &  s1_b_final_mant_ff[i];
        ld_propagate[i] =  s1_a_final_mant_ff[i] ^  s1_b_final_mant_ff[i];
        ld_kill[i]      = ~s1_a_final_mant_ff[i] & ~s1_b_final_mant_ff[i];
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
    out_mant_renorm_shift_pred1 = 'd25 - ld_prediction;
    out_mant_renorm_shift_pred2 = 'd25 - ld_prediction - 'd1;
    out_mant_renorm_shift_clamp_pred1 = (!s1_norm_out_ff || (|ld_indicator[27:25])) ? '0 : out_mant_renorm_shift_pred1;
    out_mant_renorm_shift_clamp_pred2 = (!s1_norm_out_ff || (|ld_indicator[27:24])) ? '0 : out_mant_renorm_shift_pred2;
    out_exp_renorm_delta_pred1 = s1_norm_out_ff ? (out_added_mant[27] ? 'd1 : (ld_prediction       - 'd26)) : 'd0;
    out_exp_renorm_delta_pred2 = s1_norm_out_ff ?                             (ld_prediction - 'd1 - 'd26)  : 'd0;
    out_overflow = (s1_out_exp_pre_renorm_ff == 8'hfe) && out_added_mant[27];
    out_underflow_pred1 = s1_norm_out_ff && !ld_indicator[27] && (s1_out_exp_pre_renorm_ff == ('d26 - ld_prediction      ));
    out_underflow_pred2 = s1_norm_out_ff                      && (s1_out_exp_pre_renorm_ff == ('d26 - ld_prediction - 'd1));
    out_underflow = ld_pred_down ? out_underflow_pred2 : out_underflow_pred1;
    out.zero    = s1_out_is_zero_pre_ff
                || (ld_prediction == 1) && (out_round_mant[1:0] == 2'b0)
                || (ld_prediction == 0) && !out_round_mant[0]
                ||  out_underflow;
    out.infinite = s1_out_is_inf_pre_ff || out_overflow;
    out.nan = s1_out_is_nan_ff;
    out.m_shift = ld_pred_down ? out_mant_renorm_shift_clamp_pred2 : out_mant_renorm_shift_clamp_pred1;
    out.e_delta = ld_pred_down ? out_exp_renorm_delta_pred2 : out_exp_renorm_delta_pred1;
    out.s = s1_out_sign_ff;
    out.m = (out_overflow || out_underflow) ? '0 : out_round_mant;
    out.e = out.zero ? '0 : (out_overflow ? 8'hff : s1_out_exp_pre_renorm_ff[7:0]);
    out.subnorm = 1'b0;
end
endmodule
