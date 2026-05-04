// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_traverse vs new rbox_traverse.
//
// The triangle_state_t (~336 bits), tile_state_t (~191 bits), quad_data_t (~468 bits),
// and quad_test_t (~166 bits) are too wide for single Verilator signals.
// We decompose them into scalar ports and reconstruct internally.
//
// All original sub-modules (rbox_sampler, rbox_step_mul, rbox_sample_to_fp32,
// rbox_depth_offset, rbox_fp_adder_2stg, rbox_fp_fast_adder_shifter,
// rbox_depth_to_fp, rbox_depth_to_un) are inlined with _orig suffixes to
// avoid name collisions with the new SystemVerilog versions.

`include "soc.vh"

module cosim_rbox_traverse_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // ── Triangle state (decomposed) ────────────────���─────
  input  logic              tri_facing_i,        // tri_facing_e (1 bit)
  input  logic [33:0]       tri_edge_eq_a0_i,    // edge_coef_t
  input  logic [33:0]       tri_edge_eq_a1_i,
  input  logic [33:0]       tri_edge_eq_a2_i,
  input  logic [33:0]       tri_edge_eq_b0_i,
  input  logic [33:0]       tri_edge_eq_b1_i,
  input  logic [33:0]       tri_edge_eq_b2_i,
  input  logic [2:0]        tri_top_or_left_i,
  input  logic [31:0]       tri_depth_eq_a_i,    // depth_coef_t
  input  logic [31:0]       tri_depth_eq_b_i,
  input  logic [63:0]       tri_data_addr_i,

  // ── Tile state (decomposed) ──────────────────────────
  input  logic [2:0]        tile_size_i,         // tile_size_e
  input  logic [12:0]       tile_left_i,         // [MaxRenderDimL:2]
  input  logic [12:0]       tile_top_i,
  input  logic              tile_fully_covered_i,
  input  logic [42:0]       tile_edge_sample0_i, // edge_sample_t
  input  logic [42:0]       tile_edge_sample1_i,
  input  logic [42:0]       tile_edge_sample2_i,
  input  logic [31:0]       tile_depth_sample_i,

  // ── Control inputs ────────────────────────���──────────
  input  logic              tr_tile_valid_i,
  input  logic              ct_quad_accept_i,
  input  logic              ts_quad_accept_i,

  // ── RBOX state from control ──────────────────────────
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
  input  logic [31:0]       tr_depth_bound_max_i,

  // ── New module outputs ───────────────────────────────
  output logic              new_tr_accept_o,
  output logic              new_tr_empty_o,
  output logic              new_tr_quad_data_valid_o,
  output logic              new_tr_quad_test_valid_o,
  // quad_data decomposed
  output logic [5:0]        new_qd_x_o,
  output logic [5:0]        new_qd_y_o,
  output logic [3:0]        new_qd_mask_o,
  output logic [3:0]        new_qd_sample_idx_o,
  output logic [31:0]       new_qd_i0_o,
  output logic [31:0]       new_qd_i1_o,
  output logic [31:0]       new_qd_i2_o,
  output logic [31:0]       new_qd_i3_o,
  output logic [31:0]       new_qd_j0_o,
  output logic [31:0]       new_qd_j1_o,
  output logic [31:0]       new_qd_j2_o,
  output logic [31:0]       new_qd_j3_o,
  output logic [31:0]       new_qd_depth0_o,
  output logic [31:0]       new_qd_depth1_o,
  output logic [31:0]       new_qd_depth2_o,
  output logic [31:0]       new_qd_depth3_o,
  output logic [15:0]       new_qd_cov0_o,
  output logic [15:0]       new_qd_cov1_o,
  output logic [15:0]       new_qd_cov2_o,
  output logic [15:0]       new_qd_cov3_o,
  // quad_test decomposed
  output logic [14:0]       new_qt_x_o,
  output logic [14:0]       new_qt_y_o,
  output logic [3:0]        new_qt_sample_idx_o,
  output logic [3:0]        new_qt_mask_o,
  output logic [31:0]       new_qt_depth0_o,
  output logic [31:0]       new_qt_depth1_o,
  output logic [31:0]       new_qt_depth2_o,
  output logic [31:0]       new_qt_depth3_o,

  // ── Original module outputs ──────────────────────────
  output logic              orig_tr_accept_o,
  output logic              orig_tr_empty_o,
  output logic              orig_tr_quad_data_valid_o,
  output logic              orig_tr_quad_test_valid_o,
  output logic [5:0]        orig_qd_x_o,
  output logic [5:0]        orig_qd_y_o,
  output logic [3:0]        orig_qd_mask_o,
  output logic [3:0]        orig_qd_sample_idx_o,
  output logic [31:0]       orig_qd_i0_o,
  output logic [31:0]       orig_qd_i1_o,
  output logic [31:0]       orig_qd_i2_o,
  output logic [31:0]       orig_qd_i3_o,
  output logic [31:0]       orig_qd_j0_o,
  output logic [31:0]       orig_qd_j1_o,
  output logic [31:0]       orig_qd_j2_o,
  output logic [31:0]       orig_qd_j3_o,
  output logic [31:0]       orig_qd_depth0_o,
  output logic [31:0]       orig_qd_depth1_o,
  output logic [31:0]       orig_qd_depth2_o,
  output logic [31:0]       orig_qd_depth3_o,
  output logic [15:0]       orig_qd_cov0_o,
  output logic [15:0]       orig_qd_cov1_o,
  output logic [15:0]       orig_qd_cov2_o,
  output logic [15:0]       orig_qd_cov3_o,
  output logic [14:0]       orig_qt_x_o,
  output logic [14:0]       orig_qt_y_o,
  output logic [3:0]        orig_qt_sample_idx_o,
  output logic [3:0]        orig_qt_mask_o,
  output logic [31:0]       orig_qt_depth0_o,
  output logic [31:0]       orig_qt_depth1_o,
  output logic [31:0]       orig_qt_depth2_o,
  output logic [31:0]       orig_qt_depth3_o
);

  import rbox_pkg::*;

  // ── Reconstruct packed structs from decomposed ports ──

  triangle_state_t tr_triangle;
  tile_state_t     tr_tile;

  always_comb begin
    tr_triangle.tri_facing         = tri_facing_e'(tri_facing_i);
    tr_triangle.edge_eq_a[0]       = tri_edge_eq_a0_i;
    tr_triangle.edge_eq_a[1]       = tri_edge_eq_a1_i;
    tr_triangle.edge_eq_a[2]       = tri_edge_eq_a2_i;
    tr_triangle.edge_eq_b[0]       = tri_edge_eq_b0_i;
    tr_triangle.edge_eq_b[1]       = tri_edge_eq_b1_i;
    tr_triangle.edge_eq_b[2]       = tri_edge_eq_b2_i;
    tr_triangle.top_or_left_edge   = tri_top_or_left_i;
    tr_triangle.depth_eq_a         = tri_depth_eq_a_i;
    tr_triangle.depth_eq_b         = tri_depth_eq_b_i;
    tr_triangle.triangle_data_addr = tri_data_addr_i;

    tr_tile.size          = tile_size_e'(tile_size_i);
    tr_tile.left          = tile_left_i;
    tr_tile.top           = tile_top_i;
    tr_tile.fully_covered = tile_fully_covered_i;
    tr_tile.edge_sample[0] = tile_edge_sample0_i;
    tr_tile.edge_sample[1] = tile_edge_sample1_i;
    tr_tile.edge_sample[2] = tile_edge_sample2_i;
    tr_tile.depth_sample  = tile_depth_sample_i;
  end

  // ── Also build old-style structs for the original module ──
  // The original uses rbox_triangle_state_t / rbox_tile_state_t from soc.vh.
  // These are layout-identical to triangle_state_t / tile_state_t.

  rbox_triangle_state_t orig_triangle;
  rbox_tile_state_t     orig_tile;

  always_comb begin
    orig_triangle.tri_facing         = rbox_tri_facing_t'(tri_facing_i);
    orig_triangle.edge_eq_a[0]       = tri_edge_eq_a0_i;
    orig_triangle.edge_eq_a[1]       = tri_edge_eq_a1_i;
    orig_triangle.edge_eq_a[2]       = tri_edge_eq_a2_i;
    orig_triangle.edge_eq_b[0]       = tri_edge_eq_b0_i;
    orig_triangle.edge_eq_b[1]       = tri_edge_eq_b1_i;
    orig_triangle.edge_eq_b[2]       = tri_edge_eq_b2_i;
    orig_triangle.top_or_left_edge   = tri_top_or_left_i;
    orig_triangle.depth_eq_a         = tri_depth_eq_a_i;
    orig_triangle.depth_eq_b         = tri_depth_eq_b_i;
    orig_triangle.triangle_data_addr = tri_data_addr_i;

    orig_tile.size          = rbox_tile_size_t'(tile_size_i);
    orig_tile.left          = tile_left_i;
    orig_tile.top           = tile_top_i;
    orig_tile.fully_covered = tile_fully_covered_i;
    orig_tile.edge_sample[0] = tile_edge_sample0_i;
    orig_tile.edge_sample[1] = tile_edge_sample1_i;
    orig_tile.edge_sample[2] = tile_edge_sample2_i;
    orig_tile.depth_sample  = tile_depth_sample_i;
  end

  // ── New module ──────────────────────────────────���─────

  quad_data_t new_quad_data;
  quad_test_t new_quad_test;

  rbox_traverse u_new (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .tr_triangle_i          (tr_triangle),
    .tr_tile_i              (tr_tile),
    .tr_tile_valid_i        (tr_tile_valid_i),
    .tr_accept_o            (new_tr_accept_o),
    .tr_empty_o             (new_tr_empty_o),
    .tr_quad_data_o         (new_quad_data),
    .tr_quad_data_valid_o   (new_tr_quad_data_valid_o),
    .tr_quad_test_o         (new_quad_test),
    .tr_quad_test_valid_o   (new_tr_quad_test_valid_o),
    .ct_quad_accept_i       (ct_quad_accept_i),
    .ts_quad_accept_i       (ts_quad_accept_i),
    .tr_msaa_enable_i       (tr_msaa_enable_i),
    .tr_msaa_mode_i         (msaa_mode_e'(tr_msaa_mode_i)),
    .tr_depth_is_fp32_i     (tr_depth_is_fp32_i),
    .tr_depth_is_un24_i     (tr_depth_is_un24_i),
    .tr_depth_is_un16_i     (tr_depth_is_un16_i),
    .tr_scissor_min_x_i     (tr_scissor_min_x_i),
    .tr_scissor_max_x_i     (tr_scissor_max_x_i),
    .tr_scissor_min_y_i     (tr_scissor_min_y_i),
    .tr_scissor_max_y_i     (tr_scissor_max_y_i),
    .tr_depth_clamp_enable_i(tr_depth_clamp_enable_i),
    .tr_depth_min_i         (tr_depth_min_i),
    .tr_depth_max_i         (tr_depth_max_i),
    .tr_depth_bound_enable_i(tr_depth_bound_enable_i),
    .tr_depth_bound_min_i   (tr_depth_bound_min_i),
    .tr_depth_bound_max_i   (tr_depth_bound_max_i)
  );

  // Decompose new quad_data
  assign new_qd_x_o          = new_quad_data.x;
  assign new_qd_y_o          = new_quad_data.y;
  assign new_qd_mask_o        = new_quad_data.mask;
  assign new_qd_sample_idx_o  = new_quad_data.sample_idx;
  assign new_qd_i0_o         = new_quad_data.i[0];
  assign new_qd_i1_o         = new_quad_data.i[1];
  assign new_qd_i2_o         = new_quad_data.i[2];
  assign new_qd_i3_o         = new_quad_data.i[3];
  assign new_qd_j0_o         = new_quad_data.j[0];
  assign new_qd_j1_o         = new_quad_data.j[1];
  assign new_qd_j2_o         = new_quad_data.j[2];
  assign new_qd_j3_o         = new_quad_data.j[3];
  assign new_qd_depth0_o     = new_quad_data.depth[0];
  assign new_qd_depth1_o     = new_quad_data.depth[1];
  assign new_qd_depth2_o     = new_quad_data.depth[2];
  assign new_qd_depth3_o     = new_quad_data.depth[3];
  assign new_qd_cov0_o       = new_quad_data.msaa_coverage[0];
  assign new_qd_cov1_o       = new_quad_data.msaa_coverage[1];
  assign new_qd_cov2_o       = new_quad_data.msaa_coverage[2];
  assign new_qd_cov3_o       = new_quad_data.msaa_coverage[3];

  // Decompose new quad_test
  assign new_qt_x_o          = new_quad_test.x;
  assign new_qt_y_o          = new_quad_test.y;
  assign new_qt_sample_idx_o = new_quad_test.sample_idx;
  assign new_qt_mask_o       = new_quad_test.mask;
  assign new_qt_depth0_o     = new_quad_test.depth[0];
  assign new_qt_depth1_o     = new_quad_test.depth[1];
  assign new_qt_depth2_o     = new_quad_test.depth[2];
  assign new_qt_depth3_o     = new_quad_test.depth[3];

  // ── Original module ───────────────────────────────────

  rbox_quad_data_t orig_quad_data;
  rbox_quad_test_t orig_quad_test;

  rbox_traverse_orig u_orig (
    .clock                 (clk_i),
    .reset                 (!rst_ni),
    .tr_triangle           (orig_triangle),
    .tr_tile               (orig_tile),
    .tr_tile_valid         (tr_tile_valid_i),
    .tr_accept             (orig_tr_accept_o),
    .tr_empty              (orig_tr_empty_o),
    .tr_quad_data          (orig_quad_data),
    .tr_quad_data_valid    (orig_tr_quad_data_valid_o),
    .tr_quad_test          (orig_quad_test),
    .tr_quad_test_valid    (orig_tr_quad_test_valid_o),
    .ct_quad_accept        (ct_quad_accept_i),
    .ts_quad_accept        (ts_quad_accept_i),
    .tr_msaa_enable        (tr_msaa_enable_i),
    .tr_msaa_mode          (rbox_msaa_mode_t'(tr_msaa_mode_i)),
    .tr_depth_is_fp32      (tr_depth_is_fp32_i),
    .tr_depth_is_un24      (tr_depth_is_un24_i),
    .tr_depth_is_un16      (tr_depth_is_un16_i),
    .tr_scissor_min_x      (tr_scissor_min_x_i),
    .tr_scissor_max_x      (tr_scissor_max_x_i),
    .tr_scissor_min_y      (tr_scissor_min_y_i),
    .tr_scissor_max_y      (tr_scissor_max_y_i),
    .tr_depth_clamp_enable (tr_depth_clamp_enable_i),
    .tr_depth_min          (tr_depth_min_i),
    .tr_depth_max          (tr_depth_max_i),
    .tr_depth_bound_enable (tr_depth_bound_enable_i),
    .tr_depth_bound_min    (tr_depth_bound_min_i),
    .tr_depth_bound_max    (tr_depth_bound_max_i)
  );

  // Decompose original quad_data
  assign orig_qd_x_o          = orig_quad_data.x;
  assign orig_qd_y_o          = orig_quad_data.y;
  assign orig_qd_mask_o        = orig_quad_data.mask;
  assign orig_qd_sample_idx_o  = orig_quad_data.sample_idx;
  assign orig_qd_i0_o         = orig_quad_data.i[0];
  assign orig_qd_i1_o         = orig_quad_data.i[1];
  assign orig_qd_i2_o         = orig_quad_data.i[2];
  assign orig_qd_i3_o         = orig_quad_data.i[3];
  assign orig_qd_j0_o         = orig_quad_data.j[0];
  assign orig_qd_j1_o         = orig_quad_data.j[1];
  assign orig_qd_j2_o         = orig_quad_data.j[2];
  assign orig_qd_j3_o         = orig_quad_data.j[3];
  assign orig_qd_depth0_o     = orig_quad_data.depth[0];
  assign orig_qd_depth1_o     = orig_quad_data.depth[1];
  assign orig_qd_depth2_o     = orig_quad_data.depth[2];
  assign orig_qd_depth3_o     = orig_quad_data.depth[3];
  assign orig_qd_cov0_o       = orig_quad_data.msaa_coverage[0];
  assign orig_qd_cov1_o       = orig_quad_data.msaa_coverage[1];
  assign orig_qd_cov2_o       = orig_quad_data.msaa_coverage[2];
  assign orig_qd_cov3_o       = orig_quad_data.msaa_coverage[3];

  // Decompose original quad_test
  assign orig_qt_x_o          = orig_quad_test.x;
  assign orig_qt_y_o          = orig_quad_test.y;
  assign orig_qt_sample_idx_o = orig_quad_test.sample_idx;
  assign orig_qt_mask_o       = orig_quad_test.mask;
  assign orig_qt_depth0_o     = orig_quad_test.depth[0];
  assign orig_qt_depth1_o     = orig_quad_test.depth[1];
  assign orig_qt_depth2_o     = orig_quad_test.depth[2];
  assign orig_qt_depth3_o     = orig_quad_test.depth[3];

endmodule


// ════════════════════════════════════════════════════════════════════════════
// INLINED ORIGINALS — all renamed with _orig suffix
// ════════════════════════════════════════════════════════════════════════════
//
// Module hierarchy:
//   rbox_traverse_orig
//     rbox_sampler_orig (x12)
//       rbox_step_mul_orig (x4 each)
//     rbox_sample_to_fp32_orig (x8)
//     rbox_depth_offset_orig (x4)
//       rbox_fp_adder_2stg_orig (x1 each)
//         rbox_fp_fast_adder_shifter_orig (x2 each)
//     rbox_fp_adder_2stg_orig (x4 for depth interpolators)
//     rbox_depth_to_fp_orig (x4)
//     rbox_depth_to_un_orig (x8)

// ── rbox_step_mul_orig ────────────────────────────���────────────────────

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
always_comb begin
    offset_pre_shift = `SX(MUL_SZ, coef) * step;
    offset = offset_pre_shift >> STEP_FRC_SZ;
end
endmodule

// ── rbox_sampler_orig ──────────────────────────────────────────────────

module rbox_sampler_orig
(
    input logic               clock,
    input rbox_edge_coef_t    coef_x,
    input rbox_edge_coef_t    coef_y,
    input rbox_step_t         quad_step_x,
    input rbox_step_t         quad_step_y,
    input rbox_step_t         row_step_x,
    input rbox_step_t         row_step_y,
    input logic               valid,
    input rbox_edge_sample_t  in_sample,
    input logic               sample_row,
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

rbox_step_mul_orig #(.STEP_INT_SZ(`RBOX_STEP_INT_SZ), .STEP_FRC_SZ(`RBOX_STEP_FRC_SZ),
                     .COEF_INT_SZ(`RBOX_EDGE_COEF_INT_SZ), .COEF_FRC_SZ(`RBOX_EDGE_COEF_FRC_SZ))
step_mul_quad_x (.step(quad_step_x), .coef(coef_x), .offset(quad_offset_x_nxt));

rbox_step_mul_orig #(.STEP_INT_SZ(`RBOX_STEP_INT_SZ), .STEP_FRC_SZ(`RBOX_STEP_FRC_SZ),
                     .COEF_INT_SZ(`RBOX_EDGE_COEF_INT_SZ), .COEF_FRC_SZ(`RBOX_EDGE_COEF_FRC_SZ))
step_mul_quad_y (.step(quad_step_y), .coef(coef_y), .offset(quad_offset_y_nxt));

rbox_step_mul_orig #(.STEP_INT_SZ(`RBOX_STEP_INT_SZ), .STEP_FRC_SZ(`RBOX_STEP_FRC_SZ),
                     .COEF_INT_SZ(`RBOX_EDGE_COEF_INT_SZ), .COEF_FRC_SZ(`RBOX_EDGE_COEF_FRC_SZ))
step_mul_row_x (.step(row_step_x), .coef(coef_x), .offset(row_offset_x_nxt));

rbox_step_mul_orig #(.STEP_INT_SZ(`RBOX_STEP_INT_SZ), .STEP_FRC_SZ(`RBOX_STEP_FRC_SZ),
                     .COEF_INT_SZ(`RBOX_EDGE_COEF_INT_SZ), .COEF_FRC_SZ(`RBOX_EDGE_COEF_FRC_SZ))
step_mul_row_y (.step(row_step_y), .coef(coef_y), .offset(row_offset_y_nxt));

`EN_FF(clock, valid, quad_offset_x_ff, quad_offset_x_nxt)
`EN_FF(clock, valid, quad_offset_y_ff, quad_offset_y_nxt)
`EN_FF(clock, valid, row_offset_x_ff,  row_offset_x_nxt)
`EN_FF(clock, valid, row_offset_y_ff,  row_offset_y_nxt)

logic [OFFSET_SZ-1:0] offset_x;
logic [OFFSET_SZ-1:0] offset_y;

always_comb begin
    offset_x = sample_row ? row_offset_x_ff : quad_offset_x_ff;
    offset_y = sample_row ? row_offset_y_ff : quad_offset_y_ff;
    out_sample = in_sample + `SX(`RBOX_EDGE_SAMP_BITS, offset_x) + `SX(`RBOX_EDGE_SAMP_BITS, offset_y);
end
endmodule

// ── rbox_sample_to_fp32_orig ───────────────────────────────────────────

module rbox_sample_to_fp32_orig
#(
    parameter SAMPLE_INT_BITS = 18,
    parameter SAMPLE_FRC_BITS = 25,
    localparam SAMPLE_BITS = SAMPLE_INT_BITS + SAMPLE_FRC_BITS,
    localparam BASE_EXP = 127 - SAMPLE_FRC_BITS
)
(
    input  logic              clock,
    input  rbox_edge_sample_t in,
    input  logic              valid,
    output logic [31:0]       out
);
logic                   sign_nxt;
logic                   sign_ff;
logic [7:0]             exp_nxt;
logic [7:0]             exp_ff;
logic [7:0]             exp_final;
logic [7:0]             norm_shift;
logic [SAMPLE_BITS-1:0] mant_magnitude;
logic [SAMPLE_BITS-1:0] mant_normalized_nxt;
logic [SAMPLE_BITS-1:0] mant_normalized_ff;
logic [$clog2(SAMPLE_BITS)-1:0] leading_one;
logic                           round_up;

always_comb begin
    sign_nxt = in[SAMPLE_BITS-1];
    mant_magnitude = sign_nxt ? ~in + 'd1 : in;
    leading_one = SAMPLE_BITS-1;
    for(integer b = 0; (b < SAMPLE_BITS) && (~mant_magnitude[SAMPLE_BITS-1 - b]); b++)
        leading_one--;
    exp_nxt = BASE_EXP + leading_one;
    norm_shift = (SAMPLE_BITS - 1) - leading_one;
    mant_normalized_nxt = mant_magnitude << norm_shift;
end

`EN_FF(clock, valid, mant_normalized_ff, mant_normalized_nxt)
`EN_FF(clock, valid, sign_ff,            sign_nxt           )
`EN_FF(clock, valid, exp_ff,             exp_nxt            )

logic [25:0] mant_normalized_msb;
logic [24:0] mant_round;
logic [23:0] mant_final;

always_comb begin
    mant_normalized_msb = mant_normalized_ff[SAMPLE_BITS-1 -: 26];
    round_up =  (mant_normalized_msb[1:0] == 2'b11)
             || (mant_normalized_msb[2] && (mant_normalized_msb[1:0] == 2'b10));
    mant_round = round_up ? {1'b0, mant_normalized_msb[25:2]} + 1 : {1'b0, mant_normalized_msb[25:2]};
    exp_final = mant_round[24] ? exp_ff + 1 : exp_ff;
    mant_final = mant_round[24] ? mant_round[24:1] : mant_round[23:0];
    out = {sign_ff, exp_final, mant_final[22:0]};
end
endmodule

// ── rbox_fp_fast_adder_shifter_orig ────────────────────────────���───────

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

// ── rbox_fp_adder_2stg_orig ───────────────────────────────────��────────

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

// ── rbox_depth_offset_orig ────────────────────────────────��────────────

module rbox_depth_offset_orig (
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

always_comb begin
    s0_depth_coef_a = f32_to_depth_internal_f(depth_coef_a);
    s0_depth_coef_b = f32_to_depth_internal_f(depth_coef_b);
    s0_offset_x_mul_mant = step_x * s0_depth_coef_a.m;
    s0_offset_y_mul_mant = step_y * s0_depth_coef_b.m;
    s0_step_x_lead = `RBOX_STEP_SZ - 1;
    for (integer b = 0; b < `RBOX_STEP_SZ; b++)
        if (step_x[b]) s0_step_x_lead = `RBOX_STEP_SZ - 1 - b;
    s0_step_y_lead = `RBOX_STEP_SZ - 1;
    for (integer b = 0; b < `RBOX_STEP_SZ; b++)
        if (step_y[b]) s0_step_y_lead = `RBOX_STEP_SZ - 1 - b;
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
depth_offset_adder (.clock(clock), .a(s1_offset_x_ff), .b(s1_offset_y_ff), .valid(valid_stg2), .out(depth_offset));
endmodule

// ── rbox_depth_to_fp_orig ──────────────────────────────────────────────

module rbox_depth_to_fp_orig (
    input  rbox_internal_depth_t in,
    output logic [31:0]          out
);
logic        sign;
logic [7:0]  exp_final;
logic [23:0] mant_shifted;
logic [22:0] mant_final;
logic [4:0]  renorm_shift;
always_comb begin
    sign = in.s;
    if (in.infinite)
        out = {sign, 8'hff, 22'h0};
    else if (in.nan)
        out = {sign, 8'hff, 22'h20_0000};
    else if (in.zero)
        out = 0;
    else begin
        renorm_shift = in.m_shift;
        mant_shifted = in.m << renorm_shift;
        exp_final = in.e + `SX(8, in.e_delta);
        mant_final = mant_shifted[22:0];
        out = {sign, exp_final, mant_final};
    end
end
endmodule

// ── rbox_depth_to_un_orig ──────────────────────────────────────────────

module rbox_depth_to_un_orig
#(parameter UN_SZ = 24)
(
    input  logic [31:0]       in,
    output logic [UN_SZ-1:0]  out
);
logic             sign;
logic [7:0]       exp;
logic [22:0]      mant;
logic             infinite, nan, zero, denorm;
logic [23:0]      mant_full;
logic [25:0]      mant_align;
logic [25:0]      val_un;
logic [UN_SZ-1:0] val_out;
logic [7:0]       align_shift;
logic             un_round;
logic             out_round_up;
always_comb begin
    sign = in[31];
    exp  = in[30:23];
    mant = in[22:0];
    zero     = (exp == '0) && (mant == '0);
    denorm   = (exp == '0) && (mant != '0);
    infinite = (exp == '1) && (mant == '0);
    nan      = (exp == '1) && (mant != '0);
    mant_full = {!denorm, mant};
    align_shift  = '0;
    mant_align   = '0;
    out_round_up = '0;
    val_out      = '0;
    if (sign)
        out = '0;
    if (infinite)
        out = '0;
    else if (zero)
        out = '0;
    else if (nan)
        out = '0;
    else if (exp >= 8'h7f)
        out = '1;
    else begin
        align_shift = 8'h7f - exp;
        mant_align = {mant_full, 3'b0} >> align_shift;
        val_un = mant_align - {{UN_SZ{1'b0}}, {mant_align[25:UN_SZ]}};
        out_round_up =      (val_un[(26-UN_SZ-1) -: 2] == 2'b11)
                     || (    val_un[(26-UN_SZ  )     ]
                         && (val_un[(26-UN_SZ-1) -: 2] == 2'b10));
        val_out = (val_un[24 -: UN_SZ] == '1) ? '1 : (val_un[24 -: UN_SZ] + out_round_up);
        out = val_out;
    end
end
endmodule

// ── rbox_traverse_orig ─────────────────────────────────────────────────
// Full inlined copy of original rbox_traverse.v with all sub-module
// instantiations renamed to use the _orig variants above.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off UNOPTFLAT */

module rbox_traverse_orig
(
    input  logic                          clock,
    input  logic                          reset,

    input  rbox_triangle_state_t          tr_triangle,
    input  rbox_tile_state_t              tr_tile,
    input  logic                          tr_tile_valid,
    output logic                          tr_accept,
    output logic                          tr_empty,

    output rbox_quad_data_t               tr_quad_data,
    output logic                          tr_quad_data_valid,
    output rbox_quad_test_t               tr_quad_test,
    output logic                          tr_quad_test_valid,

    input  logic                          ct_quad_accept,
    input  logic                          ts_quad_accept,

    input  logic                          tr_msaa_enable,
    input  rbox_msaa_mode_t               tr_msaa_mode,
    input  logic                          tr_depth_is_fp32,
    input  logic                          tr_depth_is_un24,
    input  logic                          tr_depth_is_un16,
    input  logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_min_x,
    input  logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_max_x,
    input  logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_min_y,
    input  logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_max_y,
    input  logic                          tr_depth_clamp_enable,
    input  logic [31:0]                   tr_depth_min,
    input  logic [31:0]                   tr_depth_max,
    input  logic                          tr_depth_bound_enable,
    input  logic [31:0]                   tr_depth_bound_min,
    input  logic [31:0]                   tr_depth_bound_max
);

// --- Body is identical to original rbox_traverse.v lines 43-958 ---
// --- with ALL sub-module names changed to _orig variants        ---

logic                  s1_tr_pipe_en;
logic                  s1_start_traverse_en;
logic                  s2_start_traverse_ff;
rbox_msaa_sample_idx_t s1_msaa_sample_idx;
logic                  s1_msaa_sample_idx_en;
rbox_msaa_sample_idx_t s2_msaa_sample_idx_ff;
logic                  s1_last_msaa_sample;
logic                  s2_last_msaa_sample_ff;
logic                  s9_quad_data_valid_ff;
logic                  s9_quad_test_valid_ff;

always_comb begin
    s1_start_traverse_en = tr_tile_valid || s2_start_traverse_ff;
    s1_tr_pipe_en =
                     (   !s9_quad_data_valid_ff
                      && !s9_quad_test_valid_ff
                      && (tr_tile_valid || s2_start_traverse_ff || !tr_empty))
                  || (    s9_quad_data_valid_ff && ct_quad_accept
                      &&  s9_quad_test_valid_ff && ts_quad_accept)
                  || (    s9_quad_data_valid_ff && ct_quad_accept
                      && !s9_quad_test_valid_ff)
                  || (   !s9_quad_data_valid_ff
                      &&  s9_quad_test_valid_ff && ts_quad_accept);
    s1_msaa_sample_idx = s2_msaa_sample_idx_ff + 1;
    case (tr_msaa_mode)
        RBOX_MSAA_MODE_2X  : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd01;
        RBOX_MSAA_MODE_4X  : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd03;
        RBOX_MSAA_MODE_8X  : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd07;
        RBOX_MSAA_MODE_16X : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd16;
    endcase
    s1_last_msaa_sample =  (tr_msaa_mode == RBOX_MSAA_MODE_2X)  &&  s1_msaa_sample_idx[0]
                        || (tr_msaa_mode == RBOX_MSAA_MODE_4X)  && (s1_msaa_sample_idx[1:0] == 2'h3)
                        || (tr_msaa_mode == RBOX_MSAA_MODE_8X)  && (s1_msaa_sample_idx[2:0] == 3'h7)
                        || (tr_msaa_mode == RBOX_MSAA_MODE_16X) && (s1_msaa_sample_idx[3:0] == 4'hf);
    s1_msaa_sample_idx_en = tr_msaa_enable && s1_tr_pipe_en;
end

rbox_step_t[3:0]       s1_row_step_x;
rbox_step_t[3:0]       s1_row_step_y;
rbox_step_t[3:0]       s1_quad_step_x;
rbox_step_t[3:0]       s1_quad_step_y;
rbox_step_t[3:0]       s1_row_base_step_x;
rbox_step_t[3:0]       s1_row_base_step_y;
rbox_step_t[3:0]       s1_quad_base_step_x;
rbox_step_t[3:0]       s1_quad_base_step_y;
rbox_msaa_sample_pos_t s1_current_sample_pos;
rbox_msaa_sample_pos_t s1_row_sample_pos;
rbox_msaa_sample_pos_t s1_quad_sample_pos;
rbox_traverse_state_t  s3_traverse_state_ff;

always_comb begin
    s1_current_sample_pos = {$bits(rbox_msaa_sample_pos_t){1'b0}};
    s1_row_sample_pos     = {$bits(rbox_msaa_sample_pos_t){1'b0}};
    s1_quad_sample_pos    = {$bits(rbox_msaa_sample_pos_t){1'b0}};
    if (!tr_msaa_enable)
        s1_current_sample_pos = get_non_msaa_sample_pos_f();
    else
        s1_current_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, s2_msaa_sample_idx_ff);
    if (tr_msaa_enable) begin
        case (tr_msaa_mode)
            RBOX_MSAA_MODE_2X  : s1_row_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_2X_ROW_SAMPLE);
            RBOX_MSAA_MODE_4X  : s1_row_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_4X_ROW_SAMPLE);
            RBOX_MSAA_MODE_8X  : s1_row_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_8X_ROW_SAMPLE);
            RBOX_MSAA_MODE_16X : s1_row_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_16X_ROW_SAMPLE);
        endcase
        case (tr_msaa_mode)
            RBOX_MSAA_MODE_2X  : s1_quad_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_2X_QUAD_SAMPLE);
            RBOX_MSAA_MODE_4X  : s1_quad_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_4X_QUAD_SAMPLE);
            RBOX_MSAA_MODE_8X  : s1_quad_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_8X_QUAD_SAMPLE);
            RBOX_MSAA_MODE_16X : s1_quad_sample_pos = get_msaa_sample_pos_f(tr_msaa_mode, `RBOX_MSAA_MODE_16X_QUAD_SAMPLE);
        endcase
    end
    s1_quad_step_x = '0;
    s1_quad_step_y = '0;
    s1_row_step_x  = '0;
    s1_row_step_y  = '0;
    s1_quad_base_step_x = '0;
    s1_quad_base_step_y = '0;
    s1_row_base_step_x = '0;
    s1_row_base_step_y = '0;
    for (integer p = 0; p < 4; p++) begin
             s1_row_step_x[p][`RBOX_MSAA_SAMP_POS_RNG]  = s1_current_sample_pos[0];
             s1_row_step_y[p][`RBOX_MSAA_SAMP_POS_RNG]  = s1_current_sample_pos[1];
             s1_quad_step_x[p][`RBOX_MSAA_SAMP_POS_RNG] = s1_current_sample_pos[0];
             s1_quad_step_y[p][`RBOX_MSAA_SAMP_POS_RNG] = s1_current_sample_pos[1];
         s1_row_base_step_x[p][`RBOX_MSAA_SAMP_POS_RNG] = s1_row_sample_pos[0];
         s1_row_base_step_y[p][`RBOX_MSAA_SAMP_POS_RNG] = s1_row_sample_pos[1];
        s1_quad_base_step_x[p][`RBOX_MSAA_SAMP_POS_RNG] = s1_quad_sample_pos[0];
        s1_quad_base_step_y[p][`RBOX_MSAA_SAMP_POS_RNG] = s1_quad_sample_pos[1];
    end
    for (integer p = 0; p < 4; p++) begin
        s1_row_step_x[p] =                                              s1_row_base_step_x[p] - s1_row_step_x[p];
        s1_row_step_y[p] = {2'b10, {`RBOX_MSAA_SAMP_POS_BITS{1'b0}}} + (s1_row_base_step_y[p] - s1_row_step_y[p]);
    end
    if (tr_tile_valid && (s3_traverse_state_ff == RBOX_TRAVERSE_END)) begin
        s1_quad_step_x[1][`RBOX_STEP_SZ-1 -: `RBOX_STEP_INT_SZ] = 'd1;
        s1_quad_step_x[3][`RBOX_STEP_SZ-1 -: `RBOX_STEP_INT_SZ] = 'd1;
        s1_quad_step_y[2][`RBOX_STEP_SZ-1 -: `RBOX_STEP_INT_SZ] = 'd1;
        s1_quad_step_y[3][`RBOX_STEP_SZ-1 -: `RBOX_STEP_INT_SZ] = 'd1;
    end else begin
        for (integer p = 0; p < 4; p++) begin
            s1_quad_step_x[p] = {`RBOX_STEP_INT_SZ'd2, {`RBOX_MSAA_SAMP_POS_BITS{1'b0}}} + (s1_quad_base_step_x[p] - s1_quad_step_x[p]);
            s1_quad_step_y[p] =                                                             s1_quad_base_step_y[p] - s1_quad_step_y[p];
        end
    end
end

rbox_traverse_state_t         s2_traverse_state;
logic                         s2_traverse_state_start;
logic                         s2_traverse_state_quad;
logic                         s2_traverse_state_row;
logic                         s2_traverse_state_end;
logic                         s2_next_quad;
logic                         s2_next_row;
logic                         s2_end_row;
logic                         s2_sample_row;
logic                         s1_update_traverse;
rbox_edge_sample_t [3:0][2:0] s1_input_sample;
rbox_edge_sample_t [3:0][2:0] s2_input_sample_ff;
rbox_edge_sample_t [3:0][2:0] s2_output_sample;
rbox_edge_sample_t [3:0][2:0] s3_quad_sample_ff;
rbox_edge_sample_t [3:0][2:0] s3_row_sample_ff;
logic                         s3_fp_accum_bubble_ff;

always_comb begin
    s1_update_traverse = s1_tr_pipe_en && !s3_fp_accum_bubble_ff;
    if (tr_tile_valid) begin
        for (integer p = 0; p < 4; p++)
            for (integer e = 0; e < 3; e++)
                s1_input_sample[p][e] = tr_tile.edge_sample[e];
    end else if (s2_last_msaa_sample_ff) begin
        if (s2_end_row)
            s1_input_sample = s3_row_sample_ff;
        else
            s1_input_sample = s2_output_sample;
    end else
        s1_input_sample = s3_quad_sample_ff;
end

genvar pixel;
genvar edge_eq;
genvar sample;

generate
    for (pixel = 0; pixel < 4; pixel++) begin : RBOX_SAMPLERS_PIXEL
        for (edge_eq = 0; edge_eq < 3; edge_eq++) begin : RBOX_SAMPLER_PIXEL_EDGE
            rbox_sampler_orig sampler(
                .clock      (clock                           ),
                .coef_x     (tr_triangle.edge_eq_a[edge_eq]  ),
                .coef_y     (tr_triangle.edge_eq_b[edge_eq]  ),
                .quad_step_x(s1_quad_step_x[pixel]           ),
                .quad_step_y(s1_quad_step_y[pixel]           ),
                .row_step_x (s1_row_step_x[pixel]            ),
                .row_step_y (s1_row_step_y[pixel]            ),
                .valid      (s1_tr_pipe_en                   ),
                .in_sample  (s2_input_sample_ff[pixel][edge_eq]),
                .sample_row (s2_sample_row                   ),
                .out_sample (s2_output_sample[pixel][edge_eq])
            );
        end
    end
endgenerate

rbox_step_t [3:0]   s2_row_step_x_ff;
rbox_step_t [3:0]   s2_row_step_y_ff;
rbox_step_t [3:0]   s2_quad_step_x_ff;
rbox_step_t [3:0]   s2_quad_step_y_ff;
rbox_depth_coef_t   s2_depth_coef_a_ff;
rbox_depth_coef_t   s2_depth_coef_b_ff;
rbox_depth_sample_t s2_tile_depth_sample_ff;

    `EN_FF(clock,        s1_update_traverse,    s2_row_step_x_ff,        s1_row_step_x              )
    `EN_FF(clock,        s1_update_traverse,    s2_row_step_y_ff,        s1_row_step_y              )
    `EN_FF(clock,        s1_update_traverse,    s2_quad_step_x_ff,       s1_quad_step_x             )
    `EN_FF(clock,        s1_update_traverse,    s2_quad_step_y_ff,       s1_quad_step_y             )
    `EN_FF(clock,        s1_update_traverse,    s2_input_sample_ff,      s1_input_sample            )
    `EN_FF(clock,        tr_tile_valid,         s2_depth_coef_a_ff,      tr_triangle.depth_eq_a     )
    `EN_FF(clock,        tr_tile_valid,         s2_depth_coef_b_ff,      tr_triangle.depth_eq_b     )
    `EN_FF(clock,        tr_tile_valid,         s2_tile_depth_sample_ff, tr_tile.depth_sample       )
`RST_EN_FF(clock, reset, s1_start_traverse_en,  s2_start_traverse_ff,    tr_tile_valid,         1'b0)
`RST_EN_FF(clock, reset, s1_msaa_sample_idx_en, s2_msaa_sample_idx_ff,   s1_msaa_sample_idx,      '0)
`RST_EN_FF(clock, reset, s1_msaa_sample_idx_en, s2_last_msaa_sample_ff,  s1_last_msaa_sample,   1'b1)

logic [`RBOX_MAX_MSAA_SAMP_RNG][3:0] s2_quad_coverage;
logic                                s2_quad_covered;
logic [3:0]                          s2_sample_coverage;
logic                                s2_quad_in_scissor;
logic [3:0]                          s2_pixel_in_scissor;
logic [`RBOX_MAX_MSAA_SAMP_RNG][3:0] s3_quad_coverage_ff;
logic                                s3_quad_covered_ff;

always_comb begin
    s2_quad_coverage = (s2_msaa_sample_idx_ff == 0) ? 0 : s3_quad_coverage_ff;
    for (integer p = 0; p < 4; p++) begin
        s2_sample_coverage[p] =  rbox_sample_inside_triangle_f(s2_output_sample[p], tr_triangle.top_or_left_edge)
                              && s2_pixel_in_scissor[p];
        s2_quad_coverage[s2_msaa_sample_idx_ff][p] = s2_sample_coverage[p];
    end
    s2_quad_covered = (s2_msaa_sample_idx_ff == 0) ? 1'b1 : s3_quad_covered_ff;
    s2_quad_covered = s2_quad_covered && (s2_sample_coverage != 0) && s2_quad_in_scissor;
    s2_sample_row = s2_traverse_state_row;
end

logic [`RBOX_MAX_RENDER_DIM_R]  s2_current_x;
logic [`RBOX_MAX_RENDER_DIM_R]  s2_current_y;
logic [`RBOX_MAX_RENDER_DIM_R]  s2_current_x_n;
logic [`RBOX_MAX_RENDER_DIM_R]  s2_current_y_n;
logic [`RBOX_MAX_TILE_DIM_R]    s2_quad_x_nxt;
logic [`RBOX_MAX_TILE_DIM_R]    s2_quad_y_nxt;
logic [`RBOX_MAX_TILE_DIM_R]    s3_quad_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]    s3_quad_y_ff;
logic [`RBOX_MAX_TILE_DIM_R]    s2_pending_quads_x;
logic [`RBOX_MAX_TILE_DIM_R]    s2_pending_quads_y;
logic [`RBOX_MAX_TILE_DIM_R]    s3_pending_quads_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]    s3_pending_quads_y_ff;
logic                           s3_row_covered_ff;
logic                           s3_row_was_covered_ff;
logic                           s2_update_traverse;
logic                           s2_fp_accum_bubble;
logic                           s2_fp_accum_bubble_en;
logic                           s2_end_tile;
logic                           s2_next_quad_end_row;
logic                           s2_next_quad_out_of_scissor;
logic                           s2_next_row_out_of_scissor;
logic                           s2_row_covered;
logic                           s2_row_was_covered;
logic                           s2_load_row_cov;
logic                           s3_end_row_ff;
logic                           s3_end_tile_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG] s2_msaa_sample_idx_mask;

always_comb begin
    s2_update_traverse =  ((s3_traverse_state_ff != RBOX_TRAVERSE_END) && s1_tr_pipe_en && !s3_fp_accum_bubble_ff)
                       || ((s3_traverse_state_ff == RBOX_TRAVERSE_END) && s2_start_traverse_ff);
    s2_fp_accum_bubble    = !reset && s2_update_traverse && !s3_fp_accum_bubble_ff;
    s2_fp_accum_bubble_en =  reset || s2_update_traverse ||  s3_fp_accum_bubble_ff;
    s2_traverse_state_start =     (s3_traverse_state_ff == RBOX_TRAVERSE_END)  && s2_start_traverse_ff;
    s2_traverse_state_row   =     (s3_traverse_state_ff == RBOX_TRAVERSE_QUAD) && s3_end_row_ff && !s3_end_tile_ff;
    s2_traverse_state_end   =     (s3_traverse_state_ff == RBOX_TRAVERSE_QUAD) && s3_end_tile_ff;
    s2_traverse_state_quad  = (   (s3_traverse_state_ff == RBOX_TRAVERSE_START)
                               || (s3_traverse_state_ff == RBOX_TRAVERSE_ROW)) && s2_last_msaa_sample_ff;
    case (1'b1)
        s2_traverse_state_start : s2_traverse_state = RBOX_TRAVERSE_START;
        s2_traverse_state_quad  : s2_traverse_state = RBOX_TRAVERSE_QUAD;
        s2_traverse_state_row   : s2_traverse_state = RBOX_TRAVERSE_ROW;
        s2_traverse_state_end   : s2_traverse_state = RBOX_TRAVERSE_END;
        default                 : s2_traverse_state = s3_traverse_state_ff;
    endcase
end

always_comb begin
    s2_next_quad =   s1_tr_pipe_en
                 && !s3_fp_accum_bubble_ff
                 &&  s2_last_msaa_sample_ff;
    s2_next_row =      s1_tr_pipe_en
                &&    !s3_fp_accum_bubble_ff
                && (   s2_traverse_state_start
                    || s2_traverse_state_row);
    s2_quad_x_nxt = (s2_traverse_state_start || s2_traverse_state_row) ? get_tile_quad_offset_f(tr_tile.left) : s3_quad_x_ff + 'd2;
    s2_quad_y_nxt =  s2_traverse_state_start                           ? get_tile_quad_offset_f(tr_tile.top)  : s3_quad_y_ff + 'd2;
    s2_pending_quads_x = (s2_traverse_state_start || s2_traverse_state_row) ? get_quads_in_tile_x_f(tr_tile.size) : s3_pending_quads_x_ff - 'd1;
    s2_pending_quads_y =  s2_traverse_state_start                           ? get_quads_in_tile_y_f(tr_tile.size) : s3_pending_quads_y_ff - 'd1;
    s2_current_x = {tr_tile.left[`RBOX_TILE_POS_R], (s2_next_quad ? s2_quad_x_nxt : s3_quad_x_ff)};
    s2_current_y = {tr_tile.top[`RBOX_TILE_POS_R],  (s2_next_row  ? s2_quad_y_nxt : s3_quad_y_ff)};
    s2_current_x_n = s2_current_x | 'd1;
    s2_current_y_n = s2_current_y | 'd1;
    s2_pixel_in_scissor[0] =  !((s2_current_x   < tr_scissor_min_x) || (s2_current_x   > tr_scissor_max_x))
                           && !((s2_current_y   < tr_scissor_min_y) || (s2_current_y   > tr_scissor_max_y));
    s2_pixel_in_scissor[1] =  !((s2_current_x_n < tr_scissor_min_x) || (s2_current_x_n > tr_scissor_max_x))
                           && !((s2_current_y   < tr_scissor_min_y) || (s2_current_y   > tr_scissor_max_y));
    s2_pixel_in_scissor[2] =  !((s2_current_x   < tr_scissor_min_x) || (s2_current_x   > tr_scissor_max_x))
                            && !((s2_current_y_n < tr_scissor_min_y) || (s2_current_y_n > tr_scissor_max_y));
    s2_pixel_in_scissor[3] =  !((s2_current_x_n < tr_scissor_min_x) || (s2_current_x_n > tr_scissor_max_x))
                           && !((s2_current_y_n < tr_scissor_min_y) || (s2_current_y_n > tr_scissor_max_y));
    s2_quad_in_scissor = |s2_pixel_in_scissor;
    s2_next_quad_out_of_scissor = ({tr_tile.left, s3_quad_x_ff[`RBOX_MAX_TILE_DIM_L-1:1]} == tr_scissor_max_x[`RBOX_MAX_RENDER_DIM_L-1:1]);
    s2_next_row_out_of_scissor  = ({tr_tile.top,  s3_quad_y_ff[`RBOX_MAX_TILE_DIM_L-1:1]} == tr_scissor_max_y[`RBOX_MAX_RENDER_DIM_L-1:1]);
    s2_next_quad_end_row = (s3_pending_quads_x_ff == 'd1) ||  s2_next_quad_out_of_scissor || (s3_row_covered_ff && !s2_quad_covered);
    s2_end_row =   s2_next_quad
               && (s3_traverse_state_ff != RBOX_TRAVERSE_END)
               &&  s2_next_quad_end_row;
    s2_end_tile =   s2_next_quad
                && (s3_traverse_state_ff != RBOX_TRAVERSE_END)
                && (   (s3_pending_quads_y_ff == 'd0)
                    ||  s2_next_row_out_of_scissor
                    || (s3_row_was_covered_ff && !s3_row_covered_ff))
                &&  s2_next_quad_end_row;
    s2_row_covered = ((s3_traverse_state_ff != RBOX_TRAVERSE_END) && s3_row_covered_ff && !s2_end_row)
                   || (s2_next_quad && s2_quad_covered);
    s2_row_was_covered = s2_end_row && (s3_row_covered_ff || (s2_next_quad && s2_quad_covered));
    s2_load_row_cov =   s1_tr_pipe_en
                    && !s3_fp_accum_bubble_ff
                    && (s2_traverse_state_start || s2_end_row);
    if (s2_msaa_sample_idx_ff == '0)
        s2_msaa_sample_idx_mask = {`RBOX_MAX_MSAA_SAMPLES{1'b1}};
    else begin
        s2_msaa_sample_idx_mask = '0;
        s2_msaa_sample_idx_mask[s2_msaa_sample_idx_ff] = s1_tr_pipe_en;
    end
end

rbox_step_t [3:0]     s2_step_x;
rbox_step_t [3:0]     s2_step_y;
rbox_internal_depth_t s2_tile_depth_sample_int_fp;

always_comb begin
    s2_step_x = s2_sample_row ? s2_row_step_x_ff : s2_quad_step_x_ff;
    s2_step_y = s2_sample_row ? s2_row_step_y_ff : s2_quad_step_y_ff;
    s2_tile_depth_sample_int_fp = f32_to_depth_internal_f(s2_tile_depth_sample_ff);
end

logic [3:0]                     s3_sample_coverage_ff;
rbox_msaa_sample_idx_t          s3_msaa_sample_idx_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG] s3_msaa_sample_idx_mask_ff;
logic                           s3_next_quad_ff;
logic                           s3_next_row_ff;
rbox_internal_depth_t           s3_tile_depth_sample_ff;
rbox_edge_sample_t [3:0][2:0]   s3_current_sample_ff;

`RST_EN_FF(clock, reset, s2_update_traverse,    s3_traverse_state_ff,          s2_traverse_state,       RBOX_TRAVERSE_END)
    `EN_FF(clock,        s2_fp_accum_bubble_en, s3_fp_accum_bubble_ff,         s2_fp_accum_bubble                        )
`RST_EN_FF(clock, reset, s2_update_traverse,    s3_msaa_sample_idx_mask_ff,    s2_msaa_sample_idx_mask, '0               )
`RST_EN_FF(clock, reset, s2_update_traverse,    s3_end_row_ff,                 s2_end_row,              1'b0             )
`RST_EN_FF(clock, reset, s2_update_traverse,    s3_end_tile_ff,                s2_end_tile,             1'b0             )
`RST_EN_FF(clock, reset, s2_update_traverse,    s3_quad_covered_ff,            s2_quad_covered,         1'b0             )
    `EN_FF(clock,        s2_update_traverse,    s3_msaa_sample_idx_ff,         s2_msaa_sample_idx_ff                     )
    `EN_FF(clock,        s2_update_traverse,    s3_sample_coverage_ff,         s2_sample_coverage                        )
    `EN_FF(clock,        s2_update_traverse,    s3_next_quad_ff,               s2_next_quad                              )
    `EN_FF(clock,        s2_update_traverse,    s3_next_row_ff,                s2_next_row                               )
    `EN_FF(clock,        s2_update_traverse,    s3_row_covered_ff,             s2_row_covered                            )
    `EN_FF(clock,        s2_load_row_cov,       s3_row_was_covered_ff,         s2_row_was_covered                        )
    `EN_FF(clock,        s2_next_quad,          s3_quad_x_ff,                  s2_quad_x_nxt                             )
    `EN_FF(clock,        s2_next_row,           s3_quad_y_ff,                  s2_quad_y_nxt                             )
    `EN_FF(clock,        s2_next_quad,          s3_pending_quads_x_ff,         s2_pending_quads_x                        )
    `EN_FF(clock,        s2_next_row,           s3_pending_quads_y_ff,         s2_pending_quads_y                        )
    `EN_FF(clock,        s2_start_traverse_ff,  s3_tile_depth_sample_ff,       s2_tile_depth_sample_int_fp               )
    `EN_FF(clock,        s2_next_quad,          s3_quad_sample_ff,             s2_output_sample                          )
    `EN_FF(clock,        s2_next_row,           s3_row_sample_ff,              s2_output_sample                          )
    `EN_FF(clock,        s1_tr_pipe_en,         s3_current_sample_ff,          s2_output_sample                          )

generate
    for (sample = 0; sample < `RBOX_MAX_MSAA_SAMPLES; sample++)
        `EN_FF(clock, s2_msaa_sample_idx_mask[sample], s3_quad_coverage_ff[sample], s2_quad_coverage[sample])
endgenerate

// Stages 2-4: depth offset
logic                       s3_update_traverse;
rbox_internal_depth_t [3:0] s4_depth_offset;

generate
    for (pixel = 0; pixel < 4; pixel++) begin : RBOX_DEPTH_OFFSET_PIXEL
        rbox_depth_offset_orig depth_offset(
            .clock        (clock              ),
            .depth_coef_a (s2_depth_coef_a_ff ),
            .depth_coef_b (s2_depth_coef_b_ff ),
            .step_x       (s2_step_x[pixel]   ),
            .step_y       (s2_step_y[pixel]   ),
            .valid_stg1   (s2_update_traverse ),
            .valid_stg2   (s3_update_traverse ),
            .depth_offset (s4_depth_offset[pixel])
        );
    end
endgenerate

logic                           s3_start_traverse;
logic                           s3_next_quad_upd;
logic                           s3_next_row_upd;
logic                           s3_fp_accum_bubble;
logic                           s3_fp_accum_bubble_en;
logic [3:0][1:0][31:0]          s4_current_sample_fp32;
logic                           s4_fp_accum_bubble_ff;
rbox_traverse_state_t           s4_traverse_state_ff;
logic                           s4_quad_covered_ff;
logic [3:0]                     s4_sample_coverage_ff;
rbox_msaa_sample_idx_t          s4_msaa_sample_idx_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG] s4_msaa_sample_idx_mask_ff;
logic                           s4_next_quad_ff;
logic                           s4_next_row_ff;
logic [`RBOX_MAX_TILE_DIM_R]    s4_quad_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]    s4_quad_y_ff;

always_comb begin
    s3_update_traverse =      (s3_traverse_state_ff != RBOX_TRAVERSE_END) && s1_tr_pipe_en && !s4_fp_accum_bubble_ff
                       || (   (s3_traverse_state_ff == RBOX_TRAVERSE_END)
                           && (s4_traverse_state_ff != RBOX_TRAVERSE_END));
    s3_start_traverse =       (s3_traverse_state_ff == RBOX_TRAVERSE_START);
    s3_next_quad_upd = s3_next_quad_ff && s1_tr_pipe_en && !s4_fp_accum_bubble_ff;
    s3_next_row_upd  = s3_next_row_ff  && s1_tr_pipe_en && !s4_fp_accum_bubble_ff;
    s3_fp_accum_bubble    = !reset && s3_update_traverse && !s4_fp_accum_bubble_ff;
    s3_fp_accum_bubble_en =  reset || s3_update_traverse ||  s4_fp_accum_bubble_ff;
end

generate
    for (pixel = 0; pixel < 4; pixel++) begin : RBOX_SAMPLE_TO_FP32
        rbox_sample_to_fp32_orig #(.SAMPLE_INT_BITS(`RBOX_EDGE_SAMP_INT_BITS), .SAMPLE_FRC_BITS(`RBOX_EDGE_SAMP_FRC_BITS))
        barycentric_i (.clock(clock), .in(s3_current_sample_ff[pixel][0]), .valid(s3_update_traverse), .out(s4_current_sample_fp32[pixel][0]));
        rbox_sample_to_fp32_orig #(.SAMPLE_INT_BITS(`RBOX_EDGE_SAMP_INT_BITS), .SAMPLE_FRC_BITS(`RBOX_EDGE_SAMP_FRC_BITS))
        barycentric_j (.clock(clock), .in(s3_current_sample_ff[pixel][1]), .valid(s3_update_traverse), .out(s4_current_sample_fp32[pixel][1]));
    end
endgenerate

`RST_EN_FF(clock, reset, s3_update_traverse,    s4_traverse_state_ff,       s3_traverse_state_ff,       RBOX_TRAVERSE_END)
    `EN_FF(clock,        s3_fp_accum_bubble_en, s4_fp_accum_bubble_ff,      s3_fp_accum_bubble                           )
`RST_EN_FF(clock, reset, s3_update_traverse,    s4_msaa_sample_idx_mask_ff, s3_msaa_sample_idx_mask_ff, '0               )
    `EN_FF(clock,        s3_update_traverse,    s4_msaa_sample_idx_ff,      s3_msaa_sample_idx_ff                        )
    `EN_FF(clock,        s3_update_traverse,    s4_quad_covered_ff,         s3_quad_covered_ff                           )
    `EN_FF(clock,        s3_update_traverse,    s4_sample_coverage_ff,      s3_sample_coverage_ff                        )
    `EN_FF(clock,        s3_update_traverse,    s4_next_quad_ff,            s3_next_quad_ff                              )
    `EN_FF(clock,        s3_update_traverse,    s4_next_row_ff,             s3_next_row_ff                               )
    `EN_FF(clock,        s3_next_quad_upd,      s4_quad_x_ff,               s3_quad_x_ff                                 )
    `EN_FF(clock,        s3_next_row_upd,       s4_quad_y_ff,               s3_quad_y_ff                                 )

logic                           s4_update_traverse;
logic                           s4_start_traverse;
logic                           s4_next_quad_upd;
logic                           s4_next_row_upd;
logic                           s4_fp_accum_bubble;
logic                           s4_fp_accum_bubble_en;
logic                           s5_fp_accum_bubble_ff;
rbox_traverse_state_t           s5_traverse_state_ff;
logic                           s5_quad_covered_ff;
logic [3:0]                     s5_sample_coverage_ff;
rbox_msaa_sample_idx_t          s5_msaa_sample_idx_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG] s5_msaa_sample_idx_mask_ff;
logic                           s5_next_quad_ff;
logic                           s5_next_row_ff;
logic [`RBOX_MAX_TILE_DIM_R]    s5_quad_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]    s5_quad_y_ff;
rbox_internal_depth_t [3:0]     s5_depth_offset_ff;
rbox_internal_depth_t [3:0]     s5_input_sample_depth_ff;
logic [3:0][1:0][31:0]          s5_current_sample_fp32_ff;
rbox_internal_depth_t [3:0]     s7_row_sample_depth_ff;

always_comb begin
    s4_update_traverse =      (s4_traverse_state_ff != RBOX_TRAVERSE_END) && s1_tr_pipe_en && !s5_fp_accum_bubble_ff
                       || (   (s4_traverse_state_ff == RBOX_TRAVERSE_END)
                           && (s5_traverse_state_ff != RBOX_TRAVERSE_END));
    s4_start_traverse =       (s4_traverse_state_ff == RBOX_TRAVERSE_START);
    s4_next_quad_upd = s4_next_quad_ff && s1_tr_pipe_en && !s5_fp_accum_bubble_ff;
    s4_next_row_upd  = s4_next_row_ff  && s1_tr_pipe_en && !s5_fp_accum_bubble_ff;
    s4_fp_accum_bubble    = !reset && s4_update_traverse && !s5_fp_accum_bubble_ff;
    s4_fp_accum_bubble_en =  reset || s4_update_traverse ||  s5_fp_accum_bubble_ff;
end

`RST_EN_FF(clock, reset, s4_update_traverse,    s5_traverse_state_ff,       s4_traverse_state_ff,       RBOX_TRAVERSE_END)
    `EN_FF(clock,        s4_fp_accum_bubble_en, s5_fp_accum_bubble_ff,      s4_fp_accum_bubble                           )
`RST_EN_FF(clock, reset, s4_update_traverse,    s5_msaa_sample_idx_mask_ff, s4_msaa_sample_idx_mask_ff, '0               )
    `EN_FF(clock,        s4_update_traverse,    s5_msaa_sample_idx_ff,      s4_msaa_sample_idx_ff                        )
    `EN_FF(clock,        s4_update_traverse,    s5_quad_covered_ff,         s4_quad_covered_ff                           )
    `EN_FF(clock,        s4_update_traverse,    s5_sample_coverage_ff,      s4_sample_coverage_ff                        )
    `EN_FF(clock,        s4_update_traverse,    s5_next_quad_ff,            s4_next_quad_ff                              )
    `EN_FF(clock,        s4_update_traverse,    s5_next_row_ff,             s4_next_row_ff                               )
    `EN_FF(clock,        s4_update_traverse,    s5_depth_offset_ff,         s4_depth_offset                              )
    `EN_FF(clock,        s4_update_traverse,    s5_current_sample_fp32_ff,  s4_current_sample_fp32                       )
    `EN_FF(clock,        s4_next_quad_upd,      s5_quad_x_ff,               s4_quad_x_ff                                 )
    `EN_FF(clock,        s4_next_row_upd,       s5_quad_y_ff,               s4_quad_y_ff                                 )

logic                       s5_update_traverse;
rbox_internal_depth_t [3:0] s5_input_sample_depth;
rbox_internal_depth_t [3:0] s6_output_sample_depth;
rbox_internal_depth_t [3:0] s7_current_sample_depth_ff;

always_comb begin
    unique case (1'b1)
        s5_traverse_state_ff[RBOX_TRAVERSE_IDX_START] : for (integer p = 0; p < 4; p++) s5_input_sample_depth[p] = s2_tile_depth_sample_int_fp;
        s5_traverse_state_ff[RBOX_TRAVERSE_IDX_QUAD]  :                                 s5_input_sample_depth    = s7_current_sample_depth_ff;
        s5_traverse_state_ff[RBOX_TRAVERSE_IDX_ROW]   :                                 s5_input_sample_depth    = s7_row_sample_depth_ff;
        s5_traverse_state_ff[RBOX_TRAVERSE_IDX_END]   :                                 s5_input_sample_depth    = s7_row_sample_depth_ff;
    endcase
end

generate
    for (pixel = 0; pixel < 4; pixel++) begin : RBOX_DEPTH_INTERPOLATORS_PIXEL
        rbox_fp_adder_2stg_orig
        depth_adder (.clock(clock), .a(s5_input_sample_depth[pixel]), .b(s5_depth_offset_ff[pixel]), .valid(s5_update_traverse), .out(s6_output_sample_depth[pixel]));
    end
endgenerate

logic                                s5_next_quad_upd;
logic                                s5_next_row_upd;
logic                                s5_fp_accum_bubble;
logic                                s5_fp_accum_bubble_en;
rbox_traverse_state_t                s6_traverse_state_ff;
logic                                s6_fp_accum_bubble_ff;
logic [3:0][1:0][31:0]               s6_current_sample_fp32_ff;
logic [3:0]                          s6_sample_coverage_ff;
logic                                s6_quad_covered_ff;
rbox_msaa_sample_idx_t               s6_msaa_sample_idx_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG]      s6_msaa_sample_idx_mask_ff;
logic                                s6_next_quad_ff;
logic                                s6_next_row_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s6_quad_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s6_quad_y_ff;

always_comb begin
    s5_update_traverse =      (s5_traverse_state_ff != RBOX_TRAVERSE_END) && s1_tr_pipe_en && !s6_fp_accum_bubble_ff
                       || (   (s5_traverse_state_ff == RBOX_TRAVERSE_END)
                           && (s6_traverse_state_ff != RBOX_TRAVERSE_END));
    s5_next_quad_upd   = s5_next_quad_ff && s1_tr_pipe_en && !s6_fp_accum_bubble_ff;
    s5_next_row_upd    = s5_next_row_ff  && s1_tr_pipe_en && !s6_fp_accum_bubble_ff;
    s5_fp_accum_bubble    = !reset && s5_update_traverse && !s6_fp_accum_bubble_ff;
    s5_fp_accum_bubble_en =  reset || s5_update_traverse ||  s6_fp_accum_bubble_ff;
end

`RST_EN_FF(clock, reset, s5_update_traverse,    s6_traverse_state_ff,       s5_traverse_state_ff,       RBOX_TRAVERSE_END)
    `EN_FF(clock,        s5_fp_accum_bubble_en, s6_fp_accum_bubble_ff,      s5_fp_accum_bubble                           )
`RST_EN_FF(clock, reset, s5_update_traverse,    s6_msaa_sample_idx_mask_ff, s5_msaa_sample_idx_mask_ff, '0               )
    `EN_FF(clock,        s5_update_traverse,    s6_msaa_sample_idx_ff,      s5_msaa_sample_idx_ff                        )
    `EN_FF(clock,        s5_update_traverse,    s6_quad_covered_ff,         s5_quad_covered_ff                           )
    `EN_FF(clock,        s5_update_traverse,    s6_sample_coverage_ff,      s5_sample_coverage_ff                        )
    `EN_FF(clock,        s5_update_traverse,    s6_current_sample_fp32_ff,  s5_current_sample_fp32_ff                    )
    `EN_FF(clock,        s5_update_traverse,    s6_next_quad_ff,            s5_next_quad_ff                              )
    `EN_FF(clock,        s5_update_traverse,    s6_next_row_ff,             s5_next_row_ff                               )
    `EN_FF(clock,        s5_next_quad_upd,      s6_quad_x_ff,               s5_quad_x_ff                                 )
    `EN_FF(clock,        s5_next_row_upd,       s6_quad_y_ff,               s5_quad_y_ff                                 )

logic                                s6_update_traverse;
logic                                s6_next_quad_upd;
logic                                s6_next_row_upd;
logic                                s6_fp_accum_bubble;
logic                                s6_fp_accum_bubble_en;
rbox_traverse_state_t                s7_traverse_state_ff;
logic                                s7_fp_accum_bubble_ff;
logic [3:0][1:0][31:0]               s7_current_sample_fp32_ff;
logic [3:0]                          s7_sample_coverage_ff;
logic                                s7_quad_covered_ff;
rbox_msaa_sample_idx_t               s7_msaa_sample_idx_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG]      s7_msaa_sample_idx_mask_ff;
logic                                s7_next_quad_ff;
logic                                s7_next_row_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s7_quad_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s7_quad_y_ff;

always_comb begin
    s6_update_traverse =      (s6_traverse_state_ff != RBOX_TRAVERSE_END) && s1_tr_pipe_en && !s7_fp_accum_bubble_ff
                       || (   (s6_traverse_state_ff == RBOX_TRAVERSE_END)
                           && (s7_traverse_state_ff != RBOX_TRAVERSE_END));
    s6_next_quad_upd   = s6_next_quad_ff && s1_tr_pipe_en && !s7_fp_accum_bubble_ff;
    s6_next_row_upd    = s6_next_row_ff  && s1_tr_pipe_en && !s7_fp_accum_bubble_ff;
    s6_fp_accum_bubble    = !reset && s6_update_traverse && !s7_fp_accum_bubble_ff;
    s6_fp_accum_bubble_en =  reset || s6_update_traverse ||  s7_fp_accum_bubble_ff;
end

`RST_EN_FF(clock, reset, s6_update_traverse,    s7_traverse_state_ff,       s6_traverse_state_ff,       RBOX_TRAVERSE_END)
    `EN_FF(clock,        s6_fp_accum_bubble_en, s7_fp_accum_bubble_ff,      s6_fp_accum_bubble                           )
`RST_EN_FF(clock, reset, s6_update_traverse,    s7_msaa_sample_idx_mask_ff, s6_msaa_sample_idx_mask_ff, '0               )
    `EN_FF(clock,        s6_update_traverse,    s7_msaa_sample_idx_ff,      s6_msaa_sample_idx_ff                        )
    `EN_FF(clock,        s6_update_traverse,    s7_quad_covered_ff,         s6_quad_covered_ff                           )
    `EN_FF(clock,        s6_update_traverse,    s7_sample_coverage_ff,      s6_sample_coverage_ff                        )
    `EN_FF(clock,        s6_update_traverse,    s7_next_quad_ff,            s6_next_quad_ff                              )
    `EN_FF(clock,        s6_update_traverse,    s7_next_row_ff,             s6_next_row_ff                               )
    `EN_FF(clock,        s6_next_quad_upd,      s7_quad_x_ff,               s6_quad_x_ff                                 )
    `EN_FF(clock,        s6_next_row_upd,       s7_quad_y_ff,               s6_quad_y_ff                                 )
    `EN_FF(clock,        s6_update_traverse,    s7_current_sample_fp32_ff,  s6_current_sample_fp32_ff                    )
    `EN_FF(clock,        s6_next_quad_upd,      s7_current_sample_depth_ff, s6_output_sample_depth                       )
    `EN_FF(clock,        s6_next_row_upd,       s7_row_sample_depth_ff,     s6_output_sample_depth                       )

logic [3:0][31:0] s7_current_sample_depth_fp32;

generate
    for (pixel = 0; pixel < 4; pixel++) begin : RBOX_DEPTH_INT_FP_TO_FP32
        rbox_depth_to_fp_orig depth_int_fp_to_fp32 (.in(s7_current_sample_depth_ff[pixel]), .out(s7_current_sample_depth_fp32[pixel]));
    end
endgenerate

logic                                s7_update_traverse;
logic                                s7_next_quad_upd;
logic                                s7_next_row_upd;
logic                                s7_fp_accum_bubble;
logic                                s7_fp_accum_bubble_en;
rbox_traverse_state_t                s8_traverse_state_ff;
logic                                s8_fp_accum_bubble_ff;
logic [3:0][1:0][31:0]               s8_current_sample_fp32_ff;
logic [3:0]                          s8_sample_coverage_ff;
logic                                s8_quad_covered_ff;
rbox_msaa_sample_idx_t               s8_msaa_sample_idx_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG]      s8_msaa_sample_idx_mask_ff;
logic                                s8_next_quad_ff;
logic                                s8_next_row_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s8_quad_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s8_quad_y_ff;
logic [3:0][31:0]                    s8_current_sample_depth_fp32_ff;

always_comb begin
    s7_update_traverse =      (s7_traverse_state_ff != RBOX_TRAVERSE_END) && s1_tr_pipe_en && !s8_fp_accum_bubble_ff
                       || (   (s7_traverse_state_ff == RBOX_TRAVERSE_END)
                           && (s8_traverse_state_ff != RBOX_TRAVERSE_END));
    s7_next_quad_upd   = s7_next_quad_ff && s1_tr_pipe_en && !s8_fp_accum_bubble_ff;
    s7_next_row_upd    = s7_next_row_ff  && s1_tr_pipe_en && !s8_fp_accum_bubble_ff;
    s7_fp_accum_bubble    = !reset && s7_update_traverse && !s8_fp_accum_bubble_ff;
    s7_fp_accum_bubble_en =  reset || s7_update_traverse ||  s8_fp_accum_bubble_ff;
end

`RST_EN_FF(clock, reset, s7_update_traverse,    s8_traverse_state_ff,            s7_traverse_state_ff,       RBOX_TRAVERSE_END)
    `EN_FF(clock,        s7_fp_accum_bubble_en, s8_fp_accum_bubble_ff,           s7_fp_accum_bubble                           )
`RST_EN_FF(clock, reset, s7_update_traverse,    s8_msaa_sample_idx_mask_ff,      s7_msaa_sample_idx_mask_ff, '0               )
    `EN_FF(clock,        s7_update_traverse,    s8_msaa_sample_idx_ff,           s7_msaa_sample_idx_ff                        )
    `EN_FF(clock,        s7_update_traverse,    s8_quad_covered_ff,              s7_quad_covered_ff                           )
    `EN_FF(clock,        s7_update_traverse,    s8_sample_coverage_ff,           s7_sample_coverage_ff                        )
    `EN_FF(clock,        s7_update_traverse,    s8_current_sample_fp32_ff,       s7_current_sample_fp32_ff                    )
    `EN_FF(clock,        s7_update_traverse,    s8_current_sample_depth_fp32_ff, s7_current_sample_depth_fp32                 )
    `EN_FF(clock,        s7_update_traverse,    s8_next_quad_ff,                 s7_next_quad_ff                              )
    `EN_FF(clock,        s7_update_traverse,    s8_next_row_ff,                  s7_next_row_ff                               )
    `EN_FF(clock,        s7_next_quad_upd,      s8_quad_x_ff,                    s7_quad_x_ff                                 )
    `EN_FF(clock,        s7_next_row_upd,       s8_quad_y_ff,                    s7_quad_y_ff                                 )

logic [3:0][31:0] s8_current_sample_depth_clamp_fp32;
logic [3:0]       s8_current_sample_depth_is_inf;
logic [3:0]       s8_current_sample_depth_is_nan;
logic [3:0]       s8_clamp_depth_to_min;
logic [3:0]       s8_clamp_depth_to_max;
logic [3:0]       s8_depth_bound_test;
logic [3:0]       s8_sample_coverage;

always_comb begin
    for (integer p = 0; p < 4; p++) begin
        s8_current_sample_depth_is_inf[p] = (s8_current_sample_depth_fp32_ff[p][30:23] == '1) && (s8_current_sample_depth_fp32_ff[p][22:20] == 0);
        s8_current_sample_depth_is_nan[p] = (s8_current_sample_depth_fp32_ff[p][30:23] == '1) && (s8_current_sample_depth_fp32_ff[p][22:20] == 0);
        s8_clamp_depth_to_min[p] =   tr_depth_clamp_enable
                                 && (    s8_current_sample_depth_is_inf[p]
                                     ||  s8_current_sample_depth_is_nan[p]
                                     ||  s8_current_sample_depth_fp32_ff[p][31]
                                     || (s8_current_sample_depth_fp32_ff[p] < tr_depth_min));
        s8_clamp_depth_to_max[p] =   tr_depth_clamp_enable
                                 && (s8_current_sample_depth_fp32_ff[p] > tr_depth_max);
        case (1'b1)
            s8_clamp_depth_to_min[p] : s8_current_sample_depth_clamp_fp32[p] = tr_depth_min;
            s8_clamp_depth_to_max[p] : s8_current_sample_depth_clamp_fp32[p] = tr_depth_max;
            default                  : s8_current_sample_depth_clamp_fp32[p] = s8_current_sample_depth_fp32_ff[p];
        endcase
        s8_depth_bound_test[p] =    !tr_depth_bound_enable
                               || (   (s8_current_sample_depth_clamp_fp32[p] >= tr_depth_bound_min)
                                   && (s8_current_sample_depth_clamp_fp32[p] <= tr_depth_bound_max));
    end
    s8_sample_coverage = s8_sample_coverage_ff & s8_depth_bound_test;
end

logic [3:0][15:0] s8_current_sample_depth_un16;
logic [3:0][23:0] s8_current_sample_depth_un24;

generate
    for (pixel = 0; pixel < 4; pixel++) begin : RBOX_DEPTH_UNORM_CVT
        rbox_depth_to_un_orig #(.UN_SZ(16))
        depth_round_un16 (.in(s8_current_sample_depth_clamp_fp32[pixel]), .out(s8_current_sample_depth_un16[pixel]));
        rbox_depth_to_un_orig #(.UN_SZ(24))
        depth_round_un24 (.in(s8_current_sample_depth_clamp_fp32[pixel]), .out(s8_current_sample_depth_un24[pixel]));
    end
endgenerate

logic                                s8_update_traverse;
logic                                s8_next_quad_upd;
logic                                s8_next_row_upd;
logic                                s8_fp_accum_bubble;
logic                                s8_fp_accum_bubble_en;
logic [`RBOX_MAX_MSAA_SAMP_RNG]      s8_msaa_sample_idx_mask_upd;
logic                                s8_quad_data_valid;
logic                                s8_quad_data_valid_en;
logic                                s8_quad_test_valid;
logic                                s8_quad_test_valid_en;
rbox_traverse_state_t                s9_traverse_state_ff;
logic                                s9_fp_accum_bubble_ff;
logic [3:0][1:0][31:0]               s9_current_sample_fp32_ff;
logic [3:0]                          s9_sample_coverage_ff;
logic [`RBOX_MAX_MSAA_SAMP_RNG][3:0] s9_quad_coverage_ff;
rbox_msaa_sample_idx_t               s9_msaa_sample_idx_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s9_quad_x_ff;
logic [`RBOX_MAX_TILE_DIM_R]         s9_quad_y_ff;
logic [3:0][31:0]                    s9_current_sample_depth_fp32_ff;
logic [3:0][15:0]                    s9_current_sample_depth_un16_ff;
logic [3:0][23:0]                    s9_current_sample_depth_un24_ff;

always_comb begin
    s8_update_traverse =      (s8_traverse_state_ff != RBOX_TRAVERSE_END) && s1_tr_pipe_en && !s9_fp_accum_bubble_ff
                       || (   (s8_traverse_state_ff == RBOX_TRAVERSE_END)
                           && (s9_traverse_state_ff != RBOX_TRAVERSE_END));
    s8_next_quad_upd   = s8_next_quad_ff && s1_tr_pipe_en && !s9_fp_accum_bubble_ff;
    s8_next_row_upd    = s8_next_row_ff  && s1_tr_pipe_en && !s9_fp_accum_bubble_ff;
    s8_msaa_sample_idx_mask_upd = s8_msaa_sample_idx_mask_ff & {$bits(s8_msaa_sample_idx_mask_ff), (s1_tr_pipe_en && !s9_fp_accum_bubble_ff)};
    s8_quad_data_valid =    !reset
                       && (   s9_quad_data_valid_ff && !ct_quad_accept
                           || s8_quad_covered_ff    && s1_tr_pipe_en   && !s9_fp_accum_bubble_ff);
    s8_quad_data_valid_en = reset
                          || ((s8_traverse_state_ff != RBOX_TRAVERSE_END) && s8_quad_covered_ff && s1_tr_pipe_en && !s9_fp_accum_bubble_ff)
                          || ct_quad_accept;
    s8_quad_test_valid =     !reset
                       && (   s9_quad_test_valid_ff && !ts_quad_accept
                           || s8_quad_covered_ff    && s1_tr_pipe_en   && !s9_fp_accum_bubble_ff);
    s8_quad_test_valid_en = reset
                         || ((s8_traverse_state_ff != RBOX_TRAVERSE_END) && s8_quad_covered_ff && s1_tr_pipe_en && !s9_fp_accum_bubble_ff)
                         || ts_quad_accept;
    s8_fp_accum_bubble    = !reset && s8_update_traverse && !s9_fp_accum_bubble_ff;
    s8_fp_accum_bubble_en =  reset || s8_update_traverse ||  s9_fp_accum_bubble_ff;
end

`RST_EN_FF(clock, reset, s8_update_traverse,    s9_traverse_state_ff,            s8_traverse_state_ff,               RBOX_TRAVERSE_END)
    `EN_FF(clock,        s8_fp_accum_bubble_en, s9_fp_accum_bubble_ff,           s8_fp_accum_bubble                                   )
    `EN_FF(clock,        s8_update_traverse,    s9_msaa_sample_idx_ff,           s8_msaa_sample_idx_ff                                )
    `EN_FF(clock,        s8_update_traverse,    s9_sample_coverage_ff,           s8_sample_coverage_ff                                )
    `EN_FF(clock,        s8_update_traverse,    s9_current_sample_fp32_ff,       s8_current_sample_fp32_ff                            )
    `EN_FF(clock,        s8_update_traverse,    s9_current_sample_depth_fp32_ff, s8_current_sample_depth_clamp_fp32                   )
    `EN_FF(clock,        s8_update_traverse,    s9_current_sample_depth_un16_ff, s8_current_sample_depth_un16                         )
    `EN_FF(clock,        s8_update_traverse,    s9_current_sample_depth_un24_ff, s8_current_sample_depth_un24                         )
    `EN_FF(clock,        s8_quad_data_valid_en, s9_quad_data_valid_ff,           s8_quad_data_valid                                   )
    `EN_FF(clock,        s8_quad_test_valid_en, s9_quad_test_valid_ff,           s8_quad_test_valid                                   )
    `EN_FF(clock,        s8_next_quad_upd,      s9_quad_x_ff,                    s8_quad_x_ff                                         )
    `EN_FF(clock,        s8_next_row_upd,       s9_quad_y_ff,                    s8_quad_y_ff                                         )

generate
    for (sample = 0; sample < `RBOX_MAX_MSAA_SAMPLES; sample++)
        `EN_FF(clock, s8_msaa_sample_idx_mask_upd[sample], s9_quad_coverage_ff[sample], s8_sample_coverage)
endgenerate

always_comb begin
    tr_quad_data.x          = s9_quad_x_ff;
    tr_quad_data.y          = s9_quad_y_ff;
    tr_quad_data.mask       = s9_sample_coverage_ff;
    tr_quad_data.sample_idx = s9_msaa_sample_idx_ff;
    for (integer p = 0; p < 4; p++) begin
        tr_quad_data.i[p]     = s9_current_sample_fp32_ff[p][0];
        tr_quad_data.j[p]     = s9_current_sample_fp32_ff[p][1];
        tr_quad_data.depth[p] = s9_current_sample_depth_fp32_ff[p];
        for (integer s = 0; s < `RBOX_MAX_MSAA_SAMPLES; s++)
            tr_quad_data.msaa_coverage[p][s] = s9_quad_coverage_ff[s][p];
    end
    tr_quad_data_valid = s9_quad_data_valid_ff;

    tr_quad_test.x          = s9_quad_x_ff;
    tr_quad_test.y          = s9_quad_y_ff;
    tr_quad_test.sample_idx = s9_msaa_sample_idx_ff;
    tr_quad_test.mask       = s9_sample_coverage_ff;
    for (integer p = 0; p < 4; p++) begin
        unique case (1'b1)
            tr_depth_is_fp32 : tr_quad_test.depth[p] =         s9_current_sample_depth_fp32_ff[p];
            tr_depth_is_un16 : tr_quad_test.depth[p] = {16'b0, s9_current_sample_depth_un16_ff[p]};
            tr_depth_is_un24 : tr_quad_test.depth[p] = { 8'b0, s9_current_sample_depth_un24_ff[p]};
            default          : tr_quad_test.depth[p] = '0;
        endcase
    end
    tr_quad_test_valid = s9_quad_test_valid_ff;
    tr_accept          = (s3_traverse_state_ff == RBOX_TRAVERSE_END);
    tr_empty           = (s3_traverse_state_ff == RBOX_TRAVERSE_END) && (s9_traverse_state_ff == RBOX_TRAVERSE_END);
end

endmodule

/* verilator lint_on UNOPTFLAT */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNUSEDSIGNAL */
