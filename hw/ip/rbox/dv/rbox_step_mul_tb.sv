// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_step_mul.
// Adds clk_i/rst_ni for sim_ctrl.h compatibility (module is combinational).

/* verilator lint_off UNUSEDSIGNAL */ // clk_i/rst_ni needed by sim_ctrl.h but unused by combinational DUT
module rbox_step_mul_tb #(
  parameter int unsigned StepIntSz = 2,
  parameter int unsigned StepFrcSz = 4,
  parameter int unsigned CoefIntSz = 1,
  parameter int unsigned CoefFrcSz = 40,
  localparam int unsigned StepSz = StepIntSz + StepFrcSz,
  localparam int unsigned CoefSz = CoefIntSz + CoefFrcSz,
  localparam int unsigned ResSz  = CoefSz + StepIntSz
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  input  logic [StepSz-1:0] step_i,
  input  logic [CoefSz-1:0] coef_i,
  output logic [ResSz-1:0]  offset_o
);

  rbox_step_mul #(
    .StepIntSz (StepIntSz),
    .StepFrcSz (StepFrcSz),
    .CoefIntSz (CoefIntSz),
    .CoefFrcSz (CoefFrcSz)
  ) u_dut (
    .step_i   (step_i),
    .coef_i   (coef_i),
    .offset_o (offset_o)
  );

endmodule
