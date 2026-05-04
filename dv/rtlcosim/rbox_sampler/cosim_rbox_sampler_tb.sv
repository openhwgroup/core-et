// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_sampler vs new rbox_sampler.
//
// The module has a 1-cycle pipeline register (stage 1 computes offsets,
// stage 2 selects and adds to sample). Both the original and new module
// instantiate rbox_step_mul internally.

`include "soc.vh"

module cosim_rbox_sampler_tb (
  input  logic                                   clk_i,
  input  logic                                   rst_ni,

  // Shared stimulus — stage 1
  input  logic [`RBOX_EDGE_COEF_SZ-1:0]         coef_x_i,
  input  logic [`RBOX_EDGE_COEF_SZ-1:0]         coef_y_i,
  input  logic [`RBOX_STEP_SZ-1:0]              quad_step_x_i,
  input  logic [`RBOX_STEP_SZ-1:0]              quad_step_y_i,
  input  logic [`RBOX_STEP_SZ-1:0]              row_step_x_i,
  input  logic [`RBOX_STEP_SZ-1:0]              row_step_y_i,
  input  logic                                   valid_i,

  // Shared stimulus — stage 2
  input  logic [`RBOX_EDGE_SAMP_BITS-1:0]       in_sample_i,
  input  logic                                   sample_row_i,

  // New module output
  output logic [`RBOX_EDGE_SAMP_BITS-1:0]       new_out_sample_o,

  // Original module output
  output logic [`RBOX_EDGE_SAMP_BITS-1:0]       orig_out_sample_o
);

  // ── New module ────────────────────────────────────────

  rbox_sampler u_new (
    .clk_i        (clk_i),
    .coef_x_i     (coef_x_i),
    .coef_y_i     (coef_y_i),
    .quad_step_x_i(quad_step_x_i),
    .quad_step_y_i(quad_step_y_i),
    .row_step_x_i (row_step_x_i),
    .row_step_y_i (row_step_y_i),
    .valid_i      (valid_i),
    .in_sample_i  (in_sample_i),
    .sample_row_i (sample_row_i),
    .out_sample_o (new_out_sample_o)
  );

  // ── Original module ───────────────────────────────────

  rbox_sampler_orig u_orig (
    .clock      (clk_i),
    .coef_x     (coef_x_i),
    .coef_y     (coef_y_i),
    .quad_step_x(quad_step_x_i),
    .quad_step_y(quad_step_y_i),
    .row_step_x (row_step_x_i),
    .row_step_y (row_step_y_i),
    .valid      (valid_i),
    .in_sample  (in_sample_i),
    .sample_row (sample_row_i),
    .out_sample (orig_out_sample_o)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Inlined copy of original rbox_sampler (renamed)
// ════════════════════════════════════════════════════════════

module rbox_sampler_orig
(
    input logic               clock,

    //  First stage inputs
    input rbox_edge_coef_t    coef_x,
    input rbox_edge_coef_t    coef_y,
    input rbox_step_t         quad_step_x,
    input rbox_step_t         quad_step_y,
    input rbox_step_t         row_step_x,
    input rbox_step_t         row_step_y,
    input logic               valid,

    // Second stage input
    input rbox_edge_sample_t  in_sample,
    input logic               sample_row,

    // Second stage output
    output rbox_edge_sample_t out_sample

);

localparam OFFSET_SZ = `RBOX_STEP_INT_SZ + `RBOX_EDGE_COEF_SZ;

logic [OFFSET_SZ-1:0] quad_offset_x_nxt;
logic [OFFSET_SZ-1:0] quad_offset_x_ff;
logic [OFFSET_SZ-1:0] quad_offset_y_nxt;
logic [OFFSET_SZ-1:0] quad_offset_y_ff;
logic [OFFSET_SZ-1:0] row_offset_x_nxt;
logic [OFFSET_SZ-1:0] row_offset_x_ff;
logic [OFFSET_SZ-1:0] row_offset_y_nxt;
logic [OFFSET_SZ-1:0] row_offset_y_ff;

rbox_step_mul_orig  #(.STEP_INT_SZ (`RBOX_STEP_INT_SZ),
                      .STEP_FRC_SZ (`RBOX_STEP_FRC_SZ),
                      .COEF_INT_SZ (`RBOX_EDGE_COEF_INT_SZ),
                      .COEF_FRC_SZ (`RBOX_EDGE_COEF_FRC_SZ))
step_mul_quad_x      (.step   (quad_step_x),
                      .coef   (coef_x),
                      .offset (quad_offset_x_nxt));

rbox_step_mul_orig  #(.STEP_INT_SZ (`RBOX_STEP_INT_SZ),
                      .STEP_FRC_SZ (`RBOX_STEP_FRC_SZ),
                      .COEF_INT_SZ (`RBOX_EDGE_COEF_INT_SZ),
                      .COEF_FRC_SZ (`RBOX_EDGE_COEF_FRC_SZ))
step_mul_quad_y      (.step   (quad_step_y),
                      .coef   (coef_y),
                      .offset (quad_offset_y_nxt));

rbox_step_mul_orig  #(.STEP_INT_SZ (`RBOX_STEP_INT_SZ),
                      .STEP_FRC_SZ (`RBOX_STEP_FRC_SZ),
                      .COEF_INT_SZ (`RBOX_EDGE_COEF_INT_SZ),
                      .COEF_FRC_SZ (`RBOX_EDGE_COEF_FRC_SZ))
step_mul_row_x       (.step   (row_step_x),
                      .coef   (coef_x),
                      .offset (row_offset_x_nxt));

rbox_step_mul_orig  #(.STEP_INT_SZ (`RBOX_STEP_INT_SZ),
                      .STEP_FRC_SZ (`RBOX_STEP_FRC_SZ),
                      .COEF_INT_SZ (`RBOX_EDGE_COEF_INT_SZ),
                      .COEF_FRC_SZ (`RBOX_EDGE_COEF_FRC_SZ))
step_mul_row_y       (.step   (row_step_y),
                      .coef   (coef_y),
                      .offset (row_offset_y_nxt));

`EN_FF(clock, valid, quad_offset_x_ff,  quad_offset_x_nxt)
`EN_FF(clock, valid, quad_offset_y_ff,  quad_offset_y_nxt)
`EN_FF(clock, valid, row_offset_x_ff,   row_offset_x_nxt)
`EN_FF(clock, valid, row_offset_y_ff,   row_offset_y_nxt)

logic [OFFSET_SZ-1:0] offset_x;
logic [OFFSET_SZ-1:0] offset_y;

always_comb
begin
    offset_x = sample_row ? row_offset_x_ff : quad_offset_x_ff;
    offset_y = sample_row ? row_offset_y_ff : quad_offset_y_ff;

    out_sample = in_sample + `SX(`RBOX_EDGE_SAMP_BITS, offset_x) + `SX(`RBOX_EDGE_SAMP_BITS, offset_y);
end

endmodule

// ════════════════════════════════════════════════════════════
// Inlined copy of original rbox_step_mul (renamed)
// ════════════════════════════════════════════════════════════

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
