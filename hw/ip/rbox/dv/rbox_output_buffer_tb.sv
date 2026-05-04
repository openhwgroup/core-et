// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_output_buffer.
// Decomposes quad_data_t input and quad_info_t output for C++ access.

/* verilator lint_off UNUSEDSIGNAL */
module rbox_output_buffer_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Traverse quad input (decomposed quad_data_t)
  input  logic [5:0]        tr_quad_x,
  input  logic [5:0]        tr_quad_y,
  input  logic [3:0]        tr_quad_mask,
  input  logic [3:0][31:0]  tr_quad_i_bary,
  input  logic [3:0][31:0]  tr_quad_j_bary,
  input  logic [3:0][31:0]  tr_quad_depth,
  input  logic [3:0]        tr_quad_sample_idx,
  input  logic [3:0][15:0]  tr_quad_msaa_cov,
  input  logic              tr_quad_valid_i,
  output logic              ct_quad_accept_o,

  // Depth test result
  input  logic [3:0]        ts_result_mask_i,
  input  logic              ts_result_valid_i,
  output logic              ct_result_accept_o,

  // Output info (decomposed)
  output logic [5:0]        out_info0_x,
  output logic [5:0]        out_info0_y,
  output logic [3:0]        out_info0_mask,
  output logic              out_info0_valid,
  output logic              out_info_valid_o,
  output logic [255:0]      out_packet_o,
  output logic              out_packet_valid_o,
  input  logic              out_pop_i,
  input  logic [2:0]        out_pending_pcks_i,

  input  logic              out_end_drawcall_i,
  output logic              empty_o,

  // Config
  input  logic [1:0]        hart_tile_width_i,
  input  logic [1:0]        hart_tile_height_i,
  input  logic [1:0]        shire_layout_width_i,
  input  logic [1:0]        shire_layout_height_i,
  input  logic              fsh_reads_coverage_i,
  input  logic              fsh_reads_depth_i,
  input  logic              fsh_disabled_i,
  input  logic              early_frag_tests_enable_i,
  input  logic              rbox_halted_i
);

  import rbox_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  quad_data_t tr_quad;
  assign tr_quad.x = tr_quad_x;
  assign tr_quad.y = tr_quad_y;
  assign tr_quad.mask = tr_quad_mask;
  assign tr_quad.i = tr_quad_i_bary;
  assign tr_quad.j = tr_quad_j_bary;
  assign tr_quad.depth = tr_quad_depth;
  assign tr_quad.sample_idx = tr_quad_sample_idx;
  assign tr_quad.msaa_coverage = tr_quad_msaa_cov;

  quad_info_t [1:0] out_info;

  assign out_info0_x     = out_info[0].x;
  assign out_info0_y     = out_info[0].y;
  assign out_info0_mask  = out_info[0].mask;
  assign out_info0_valid = out_info[0].valid;

  rbox_output_buffer u_dut (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .ram_cfg_i              ('0),
    .dft_i                  ('0),
    .tr_quad_i              (tr_quad),
    .tr_quad_valid_i        (tr_quad_valid_i),
    .ct_quad_accept_o       (ct_quad_accept_o),
    .ts_result_mask_i       (ts_result_mask_i),
    .ts_result_valid_i      (ts_result_valid_i),
    .ct_result_accept_o     (ct_result_accept_o),
    .out_info_o             (out_info),
    .out_info_valid_o       (out_info_valid_o),
    .out_packet_o           (out_packet_o),
    .out_packet_valid_o     (out_packet_valid_o),
    .out_pop_i              (out_pop_i),
    .out_pending_pcks_i     (out_pending_pcks_i),
    .out_end_drawcall_i     (out_end_drawcall_i),
    .empty_o                (empty_o),
    .hart_tile_width_i      (hart_tile_width_i),
    .hart_tile_height_i     (hart_tile_height_i),
    .shire_layout_width_i   (shire_layout_width_i),
    .shire_layout_height_i  (shire_layout_height_i),
    .fsh_reads_coverage_i   (fsh_reads_coverage_i),
    .fsh_reads_depth_i      (fsh_reads_depth_i),
    .fsh_disabled_i         (fsh_disabled_i),
    .early_frag_tests_enable_i (early_frag_tests_enable_i),
    .rbox_halted_i          (rbox_halted_i)
  );

endmodule
