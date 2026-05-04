// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_step_mul vs new rbox_step_mul.
//
// Parameterized with the edge coefficient widths used by rbox_sampler.
// Both modules are purely combinational.

`include "soc.vh"

module cosim_rbox_step_mul_tb #(
  parameter int unsigned STEP_INT_SZ = `RBOX_STEP_INT_SZ,
  parameter int unsigned STEP_FRC_SZ = `RBOX_STEP_FRC_SZ,
  parameter int unsigned COEF_INT_SZ = `RBOX_EDGE_COEF_INT_SZ,
  parameter int unsigned COEF_FRC_SZ = `RBOX_EDGE_COEF_FRC_SZ,
  localparam int unsigned STEP_SZ = STEP_INT_SZ + STEP_FRC_SZ,
  localparam int unsigned COEF_SZ = COEF_INT_SZ + COEF_FRC_SZ,
  localparam int unsigned RES_SZ  = COEF_SZ + STEP_INT_SZ
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus
  input  logic [STEP_SZ-1:0] step_i,
  input  logic [COEF_SZ-1:0] coef_i,

  // New module output
  output logic [RES_SZ-1:0]  new_offset_o,

  // Original module output
  output logic [RES_SZ-1:0]  orig_offset_o
);

  // ── New module ────────────────────────────────────────

  rbox_step_mul #(
    .StepIntSz (STEP_INT_SZ),
    .StepFrcSz (STEP_FRC_SZ),
    .CoefIntSz (COEF_INT_SZ),
    .CoefFrcSz (COEF_FRC_SZ)
  ) u_new (
    .step_i   (step_i),
    .coef_i   (coef_i),
    .offset_o (new_offset_o)
  );

  // ── Original module ───────────────────────────────────

  rbox_step_mul_orig #(
    .STEP_INT_SZ (STEP_INT_SZ),
    .STEP_FRC_SZ (STEP_FRC_SZ),
    .COEF_INT_SZ (COEF_INT_SZ),
    .COEF_FRC_SZ (COEF_FRC_SZ)
  ) u_orig (
    .step   (step_i),
    .coef   (coef_i),
    .offset (orig_offset_o)
  );

endmodule

// Rename original to avoid module name collision
module rbox_step_mul_orig
#(
    parameter STEP_INT_SZ =  2,
    parameter STEP_FRC_SZ =  4,
    parameter COEF_INT_SZ =  1,
    parameter COEF_FRC_SZ = 40,
    localparam STEP_SZ = STEP_INT_SZ+STEP_FRC_SZ,
    localparam COEF_SZ = COEF_INT_SZ+COEF_FRC_SZ,
    localparam MUL_SZ  = COEF_SZ + STEP_SZ,
    localparam RES_SZ  = COEF_SZ + STEP_INT_SZ
)
(
    input  logic [STEP_SZ-1:0] step,
    input  logic [COEF_SZ-1:0] coef,
    output logic [RES_SZ-1:0] offset
);

logic [MUL_SZ-1:0] offset_pre_shift;

always_comb
begin
    offset_pre_shift = `SX(MUL_SZ, coef) * step;
    offset = offset_pre_shift >> STEP_FRC_SZ;
end

endmodule
