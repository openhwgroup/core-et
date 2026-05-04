// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX depth interpolation offset calculator.
//
// Computes the depth offset for a quad step as:
//   depth_offset = depth_coef_a * step_x + depth_coef_b * step_y
//
// Three pipeline stages total:
//   Stage 0 (combinational → registered): multiply step by FP32 mantissa,
//            normalize result, compute internal depth representation
//   Stage 1-2 (inside rbox_fp_adder_2stg): add the X and Y offsets

module rbox_depth_offset
  import rbox_pkg::*;
(
  input  logic            clk_i,

  input  logic [31:0]     depth_coef_a_i,
  input  logic [31:0]     depth_coef_b_i,
  input  step_t           step_x_i,
  input  step_t           step_y_i,

  input  logic            valid_stg1_i,
  input  logic            valid_stg2_i,

  output internal_depth_t depth_offset_o
);

  localparam int unsigned OffsetSz    = StepSz + 24;
  localparam int unsigned OffsetIntSz = StepIntSz + 1;
  localparam int unsigned OffsetIntR  = $clog2(OffsetIntSz) + 1;

  /* verilator lint_off UNUSEDSIGNAL */ // internal_depth_t carries m_shift/e_delta/subnorm unused in this context; low shift bits truncated
  internal_depth_t            s0_depth_coef_a;
  internal_depth_t            s0_depth_coef_b;
  internal_depth_t            s0_offset_x;
  internal_depth_t            s0_offset_y;
  logic [OffsetSz-1:0]       s0_offset_x_mul_mant;
  logic [OffsetSz-1:0]       s0_offset_y_mul_mant;
  logic [OffsetSz-1:0]       s0_offset_x_shift_mant;
  logic [OffsetSz-1:0]       s0_offset_y_shift_mant;
  logic [OffsetIntR-1:0]     s0_step_x_lead;
  logic [OffsetIntR-1:0]     s0_step_y_lead;
  logic [OffsetIntR-1:0]     s0_offset_x_mant_shift;
  logic [OffsetIntR-1:0]     s0_offset_y_mant_shift;
  logic                      s0_offset_x_underflow;
  logic                      s0_offset_y_underflow;
  logic                      s0_offset_x_overflow;
  logic                      s0_offset_y_overflow;
  internal_depth_t            s1_offset_x_q;
  internal_depth_t            s1_offset_y_q;

  always_comb begin
    // Convert coefficients to internal floating-point format
    s0_depth_coef_a = f32_to_depth_internal(depth_coef_a_i);
    s0_depth_coef_b = f32_to_depth_internal(depth_coef_b_i);

    // Get mantissa for step × coefficient
    // Multiply step by mantissa — let LHS width (OffsetSz=30) propagate
    s0_offset_x_mul_mant = step_x_i * s0_depth_coef_a.m;
    s0_offset_y_mul_mant = step_y_i * s0_depth_coef_b.m;

    // Estimate leading zero for the multiplication result
    s0_step_x_lead = OffsetIntR'(StepSz - 1);
    for (int b = 0; b < StepSz; b++)
      if (step_x_i[b]) s0_step_x_lead = OffsetIntR'(StepSz - 1 - b);
    s0_step_y_lead = OffsetIntR'(StepSz - 1);
    for (int b = 0; b < StepSz; b++)
      if (step_y_i[b]) s0_step_y_lead = OffsetIntR'(StepSz - 1 - b);

    // Correct the leading zero for the multiplication result
    s0_offset_x_mant_shift = s0_step_x_lead;
    s0_offset_y_mant_shift = s0_step_y_lead;
    if (!s0_offset_x_mul_mant[OffsetSz - 1 - int'(s0_step_x_lead)]) s0_offset_x_mant_shift = s0_offset_x_mant_shift + OffsetIntR'(1);
    if (!s0_offset_y_mul_mant[OffsetSz - 1 - int'(s0_step_y_lead)]) s0_offset_y_mant_shift = s0_offset_y_mant_shift + OffsetIntR'(1);

    s0_offset_x_shift_mant = s0_offset_x_mul_mant << s0_offset_x_mant_shift;
    s0_offset_y_shift_mant = s0_offset_y_mul_mant << s0_offset_y_mant_shift;

    // Overflow/underflow detection — use natural (wide) arithmetic to match original.
    // Do NOT cast to 8 bits: (8'hfe - mant_shift + StepIntSz) must NOT wrap at 256.
    /* verilator lint_off WIDTHEXPAND */ // Intentional mixed-width comparison matching original Verilog semantics
    s0_offset_x_overflow = (s0_offset_x_mant_shift <= OffsetIntR'(StepIntSz)) && (s0_depth_coef_a.e > (8'hfe - s0_offset_x_mant_shift + StepIntSz));
    s0_offset_y_overflow = (s0_offset_y_mant_shift <= OffsetIntR'(StepIntSz)) && (s0_depth_coef_b.e > (8'hfe - s0_offset_y_mant_shift + StepIntSz));

    s0_offset_x_underflow = (s0_offset_x_mant_shift > OffsetIntR'(StepIntSz)) && (s0_depth_coef_a.e <= (s0_offset_x_mant_shift - StepIntSz));
    s0_offset_y_underflow = (s0_offset_y_mant_shift > OffsetIntR'(StepIntSz)) && (s0_depth_coef_b.e <= (s0_offset_y_mant_shift - StepIntSz));
    /* verilator lint_on WIDTHEXPAND */

    // If step is 0, coefficients are 0, or subnormal: offset is 0
    s0_offset_x.zero = (step_x_i == '0) || s0_depth_coef_a.zero || s0_depth_coef_a.subnorm || s0_offset_x_underflow;
    s0_offset_y.zero = (step_y_i == '0) || s0_depth_coef_b.zero || s0_depth_coef_b.subnorm || s0_offset_y_underflow;

    /* verilator lint_off WIDTHEXPAND */ // Mixed-width arithmetic matching original
    s0_offset_x.e = 8'(s0_depth_coef_a.e - s0_offset_x_mant_shift + StepIntSz);
    s0_offset_y.e = 8'(s0_depth_coef_b.e - s0_offset_y_mant_shift + StepIntSz);
    /* verilator lint_on WIDTHEXPAND */

    s0_offset_x.infinite = s0_depth_coef_a.infinite || s0_offset_x_overflow;
    s0_offset_y.infinite = s0_depth_coef_b.infinite || s0_offset_y_overflow;

    s0_offset_x.nan = s0_depth_coef_a.nan;
    s0_offset_y.nan = s0_depth_coef_b.nan;

    s0_offset_x.s = s0_depth_coef_a.s && !s0_offset_x.zero;
    s0_offset_y.s = s0_depth_coef_b.s && !s0_offset_y.zero;

    s0_offset_x.m = s0_offset_x_shift_mant[OffsetSz-1 -: 24];
    s0_offset_y.m = s0_offset_y_shift_mant[OffsetSz-1 -: 24];

    s0_offset_x.subnorm = 1'b0;
    s0_offset_y.subnorm = 1'b0;

    s0_offset_x.e_delta = '0;
    s0_offset_y.e_delta = '0;

    s0_offset_x.m_shift = '0;
    s0_offset_y.m_shift = '0;
  end

  /* verilator lint_on UNUSEDSIGNAL */

  // Pipeline registers (enable-only)
  always_ff @(posedge clk_i) begin
    if (valid_stg1_i) begin
      s1_offset_x_q <= s0_offset_x;
      s1_offset_y_q <= s0_offset_y;
    end
  end

  // Stages 1-2: add X and Y offsets
  rbox_fp_adder_2stg u_depth_offset_adder (
    .clk_i   (clk_i),
    .a_i     (s1_offset_x_q),
    .b_i     (s1_offset_y_q),
    .valid_i (valid_stg2_i),
    .out_o   (depth_offset_o)
  );

endmodule
