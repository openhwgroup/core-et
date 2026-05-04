// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_sampler.
// Module has clk_i; adds rst_ni for sim_ctrl.h.
// Uses default rbox_pkg types for edge_coef_t (34-bit), step_t (6-bit),
// edge_sample_t (43-bit).

/* verilator lint_off UNUSEDSIGNAL */ // rst_ni needed by sim_ctrl.h but unused by DUT
module rbox_sampler_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stage 1 inputs
  input  logic [33:0] coef_x_i,
  input  logic [33:0] coef_y_i,
  input  logic [5:0]  quad_step_x_i,
  input  logic [5:0]  quad_step_y_i,
  input  logic [5:0]  row_step_x_i,
  input  logic [5:0]  row_step_y_i,
  input  logic        valid_i,

  // Stage 2 inputs
  input  logic [42:0] in_sample_i,
  input  logic        sample_row_i,

  // Stage 2 output
  output logic [42:0] out_sample_o
);

  import rbox_pkg::*;

  rbox_sampler u_dut (
    .clk_i         (clk_i),
    .coef_x_i      (coef_x_i),
    .coef_y_i      (coef_y_i),
    .quad_step_x_i (quad_step_x_i),
    .quad_step_y_i (quad_step_y_i),
    .row_step_x_i  (row_step_x_i),
    .row_step_y_i  (row_step_y_i),
    .valid_i       (valid_i),
    .in_sample_i   (in_sample_i),
    .sample_row_i  (sample_row_i),
    .out_sample_o  (out_sample_o)
  );

endmodule
