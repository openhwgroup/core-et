// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_traverse.
// Decomposes struct ports for C++ access.
// This is a 9-stage pipeline with complex inputs. The TB
// provides the minimum decomposed interface for basic tests.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
module rbox_traverse_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Triangle state (simplified: all zeros except edge eqs)
  input  logic [511:0]      tr_triangle_raw_i,  // Flat packed triangle_state_t
  // Tile state
  input  logic [511:0]      tr_tile_raw_i,      // Flat packed tile_state_t
  input  logic              tr_tile_valid_i,
  output logic              tr_accept_o,
  output logic              tr_empty_o,

  // Quad data output (selected fields)
  output logic [5:0]        quad_data_x_o,
  output logic [5:0]        quad_data_y_o,
  output logic [3:0]        quad_data_mask_o,
  output logic              quad_data_valid_o,

  // Quad test output (selected fields)
  output logic [14:0]       quad_test_x_o,
  output logic [14:0]       quad_test_y_o,
  output logic [3:0]        quad_test_mask_o,
  output logic              quad_test_valid_o,

  input  logic              ct_quad_accept_i,
  input  logic              ts_quad_accept_i,

  // Configuration
  input  logic              tr_msaa_enable_i,
  input  logic [1:0]        tr_msaa_mode_i,
  input  logic              tr_depth_is_fp32_i,
  input  logic              tr_depth_is_un24_i,
  input  logic              tr_depth_is_un16_i,
  input  logic [14:0]       tr_scissor_min_x_i,
  input  logic [14:0]       tr_scissor_max_x_i,
  input  logic [14:0]       tr_scissor_min_y_i,
  input  logic [14:0]       tr_scissor_max_y_i,
  input  logic              tr_depth_clamp_enable_i,
  input  logic [31:0]       tr_depth_min_i,
  input  logic [31:0]       tr_depth_max_i,
  input  logic              tr_depth_bound_enable_i,
  input  logic [31:0]       tr_depth_bound_min_i,
  input  logic [31:0]       tr_depth_bound_max_i
);

  import rbox_pkg::*;

  triangle_state_t tr_triangle;
  tile_state_t     tr_tile;
  quad_data_t      tr_quad_data;
  quad_test_t      tr_quad_test;

  assign tr_triangle = tr_triangle_raw_i[$bits(triangle_state_t)-1:0];
  assign tr_tile     = tr_tile_raw_i[$bits(tile_state_t)-1:0];

  assign quad_data_x_o     = tr_quad_data.x;
  assign quad_data_y_o     = tr_quad_data.y;
  assign quad_data_mask_o  = tr_quad_data.mask;
  assign quad_test_x_o     = tr_quad_test.x;
  assign quad_test_y_o     = tr_quad_test.y;
  assign quad_test_mask_o  = tr_quad_test.mask;

  rbox_traverse u_dut (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .tr_triangle_i            (tr_triangle),
    .tr_tile_i                (tr_tile),
    .tr_tile_valid_i          (tr_tile_valid_i),
    .tr_accept_o              (tr_accept_o),
    .tr_empty_o               (tr_empty_o),
    .tr_quad_data_o           (tr_quad_data),
    .tr_quad_data_valid_o     (quad_data_valid_o),
    .tr_quad_test_o           (tr_quad_test),
    .tr_quad_test_valid_o     (quad_test_valid_o),
    .ct_quad_accept_i         (ct_quad_accept_i),
    .ts_quad_accept_i         (ts_quad_accept_i),
    .tr_msaa_enable_i         (tr_msaa_enable_i),
    .tr_msaa_mode_i           (msaa_mode_e'(tr_msaa_mode_i)),
    .tr_depth_is_fp32_i       (tr_depth_is_fp32_i),
    .tr_depth_is_un24_i       (tr_depth_is_un24_i),
    .tr_depth_is_un16_i       (tr_depth_is_un16_i),
    .tr_scissor_min_x_i       (tr_scissor_min_x_i),
    .tr_scissor_max_x_i       (tr_scissor_max_x_i),
    .tr_scissor_min_y_i       (tr_scissor_min_y_i),
    .tr_scissor_max_y_i       (tr_scissor_max_y_i),
    .tr_depth_clamp_enable_i  (tr_depth_clamp_enable_i),
    .tr_depth_min_i           (tr_depth_min_i),
    .tr_depth_max_i           (tr_depth_max_i),
    .tr_depth_bound_enable_i  (tr_depth_bound_enable_i),
    .tr_depth_bound_min_i     (tr_depth_bound_min_i),
    .tr_depth_bound_max_i     (tr_depth_bound_max_i)
  );

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNOPTFLAT */
