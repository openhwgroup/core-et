// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX edge function sampler.
//
// Computes the edge sample offset for quad and row stepping using four
// rbox_step_mul instances, then applies the selected offset (quad or row)
// to the input sample.
//
// Stage 1 (combinational → registered): compute quad/row offsets via step_mul
// Stage 2 (combinational): select offset and add to input sample

module rbox_sampler
  import rbox_pkg::*;
(
  input  logic          clk_i,

  // Stage 1 inputs
  input  edge_coef_t    coef_x_i,
  input  edge_coef_t    coef_y_i,
  input  step_t         quad_step_x_i,
  input  step_t         quad_step_y_i,
  input  step_t         row_step_x_i,
  input  step_t         row_step_y_i,
  input  logic          valid_i,

  // Stage 2 input
  input  edge_sample_t  in_sample_i,
  input  logic          sample_row_i,

  // Stage 2 output
  output edge_sample_t  out_sample_o
);

  localparam int unsigned OffsetSz = StepIntSz + EdgeCoefSz;

  logic [OffsetSz-1:0] quad_offset_x_d;
  logic [OffsetSz-1:0] quad_offset_x_q;
  logic [OffsetSz-1:0] quad_offset_y_d;
  logic [OffsetSz-1:0] quad_offset_y_q;
  logic [OffsetSz-1:0] row_offset_x_d;
  logic [OffsetSz-1:0] row_offset_x_q;
  logic [OffsetSz-1:0] row_offset_y_d;
  logic [OffsetSz-1:0] row_offset_y_q;

  rbox_step_mul #(
    .StepIntSz (StepIntSz),
    .StepFrcSz (StepFrcSz),
    .CoefIntSz (EdgeCoefIntSz),
    .CoefFrcSz (EdgeCoefFrcSz)
  ) u_step_mul_quad_x (
    .step_i   (quad_step_x_i),
    .coef_i   (coef_x_i),
    .offset_o (quad_offset_x_d)
  );

  rbox_step_mul #(
    .StepIntSz (StepIntSz),
    .StepFrcSz (StepFrcSz),
    .CoefIntSz (EdgeCoefIntSz),
    .CoefFrcSz (EdgeCoefFrcSz)
  ) u_step_mul_quad_y (
    .step_i   (quad_step_y_i),
    .coef_i   (coef_y_i),
    .offset_o (quad_offset_y_d)
  );

  rbox_step_mul #(
    .StepIntSz (StepIntSz),
    .StepFrcSz (StepFrcSz),
    .CoefIntSz (EdgeCoefIntSz),
    .CoefFrcSz (EdgeCoefFrcSz)
  ) u_step_mul_row_x (
    .step_i   (row_step_x_i),
    .coef_i   (coef_x_i),
    .offset_o (row_offset_x_d)
  );

  rbox_step_mul #(
    .StepIntSz (StepIntSz),
    .StepFrcSz (StepFrcSz),
    .CoefIntSz (EdgeCoefIntSz),
    .CoefFrcSz (EdgeCoefFrcSz)
  ) u_step_mul_row_y (
    .step_i   (row_step_y_i),
    .coef_i   (coef_y_i),
    .offset_o (row_offset_y_d)
  );

  // Pipeline registers (enable-only, no reset)
  always_ff @(posedge clk_i) begin
    if (valid_i) begin
      quad_offset_x_q <= quad_offset_x_d;
      quad_offset_y_q <= quad_offset_y_d;
      row_offset_x_q  <= row_offset_x_d;
      row_offset_y_q  <= row_offset_y_d;
    end
  end

  // Stage 2: select and add offset
  logic [OffsetSz-1:0] offset_x;
  logic [OffsetSz-1:0] offset_y;

  always_comb begin
    offset_x = sample_row_i ? row_offset_x_q : quad_offset_x_q;
    offset_y = sample_row_i ? row_offset_y_q : quad_offset_y_q;

    out_sample_o = in_sample_i + EdgeSampBits'($signed(offset_x)) + EdgeSampBits'($signed(offset_y));
  end

endmodule
