// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX step multiplier.
//
// Computes offset = coef * step, where step is in fixed-point
// (STEP_INT_SZ.STEP_FRC_SZ) and coef is in fixed-point
// (COEF_INT_SZ.COEF_FRC_SZ). The result is sign-extended and
// right-shifted to remove the fractional step bits.
//
// Pure combinational logic — no clock or reset.

module rbox_step_mul #(
  parameter int unsigned StepIntSz = 2,
  parameter int unsigned StepFrcSz = 4,
  parameter int unsigned CoefIntSz = 1,
  parameter int unsigned CoefFrcSz = 40,
  localparam int unsigned StepSz = StepIntSz + StepFrcSz,
  localparam int unsigned CoefSz = CoefIntSz + CoefFrcSz,
  localparam int unsigned MulSz  = CoefSz + StepSz,
  localparam int unsigned ResSz  = CoefSz + StepIntSz
) (
  input  logic [StepSz-1:0] step_i,
  input  logic [CoefSz-1:0] coef_i,
  output logic [ResSz-1:0]  offset_o
);

  logic [MulSz-1:0] offset_pre_shift;

  always_comb begin
    offset_pre_shift = MulSz'($signed(coef_i)) * step_i;
    offset_o = ResSz'(offset_pre_shift >> StepFrcSz);
  end

endmodule
