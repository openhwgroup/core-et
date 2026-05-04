// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_test.
// Decomposes struct ports for C++ access. Uses single clock domain.
// Ties off ram_cfg/dft/debug interfaces and SC response.

/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off PINCONNECTEMPTY */
module rbox_test_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // SC req (observe)
  output logic              db_sc_req_valid_any_o,
  input  logic              db_sc_req_ready_i,

  // Quad test input (decomposed quad_test_t)
  input  logic [14:0]       ts_quad_x,
  input  logic [14:0]       ts_quad_y,
  input  logic [3:0]        ts_quad_sample_idx,
  input  logic [3:0]        ts_quad_mask,
  input  logic [3:0][31:0]  ts_quad_depth,
  input  logic              ts_quad_valid_i,
  output logic              ts_quad_accept_o,

  // Result output
  output logic [3:0]        ts_result_mask_o,
  output logic              ts_result_valid_o,
  input  logic              ct_result_accept_i,

  // State input (simplified — only key fields used)
  input  logic              depth_test_enable_i,
  input  logic [2:0]        depth_test_compare_op_i,
  input  logic              depth_test_write_enable_i,
  input  logic [8:0]        ds_buffer_format_i,
  input  logic [63:0]       ds_buffer_addr_i,
  input  logic              ts_front_triangle_i,

  // Feedback
  output logic              ts_empty_o,

  // Flush
  input  logic              db_flush_i,
  output logic              db_flush_done_o
);

  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  quad_test_t ts_quad;
  assign ts_quad.x = ts_quad_x;
  assign ts_quad.y = ts_quad_y;
  assign ts_quad.sample_idx = ts_quad_sample_idx;
  assign ts_quad.mask = ts_quad_mask;
  assign ts_quad.depth = ts_quad_depth;

  // Build a minimal state_t with depth test fields populated
  state_t ts_state;
  always_comb begin
    ts_state = '0;
    ts_state.depth_test_enable       = depth_test_enable_i;
    ts_state.depth_test_compare_op   = comp_op_e'(depth_test_compare_op_i);
    ts_state.depth_test_write_enable = depth_test_write_enable_i;
    ts_state.ds_buffer_format        = ds_format_e'(ds_buffer_format_i);
    ts_state.ds_buffer_addr          = ds_buffer_addr_i;
  end

  // SC interfaces
  etlink_pkg::req_t  db_sc_req;
  logic [ScBanks:0]  db_sc_req_valid;
  etlink_pkg::rsp_t  sc_db_rsp;
  assign sc_db_rsp = '0;
  assign db_sc_req_valid_any_o = |db_sc_req_valid;

  rbox_test u_dut (
    .clk_i                     (clk_i),
    .clk_rbox_i                (clk_i),
    .rst_ni                    (rst_ni),
    .ram_cfg_i                 ('0),
    .dft_i                     ('0),
    .db_sc_req_o               (db_sc_req),
    .db_sc_req_valid_o         (db_sc_req_valid),
    .db_sc_req_ready_i         (db_sc_req_ready_i),
    .sc_db_rsp_i               (sc_db_rsp),
    .sc_db_rsp_valid_i         (1'b0),
    .ts_quad_test_i            (ts_quad),
    .ts_quad_valid_i           (ts_quad_valid_i),
    .ts_quad_accept_o          (ts_quad_accept_o),
    .ts_result_mask_o          (ts_result_mask_o),
    .ts_result_valid_o         (ts_result_valid_o),
    .ct_result_accept_i        (ct_result_accept_i),
    .ts_state_i                (ts_state),
    .ts_front_triangle_i       (ts_front_triangle_i),
    .ts_empty_o                (ts_empty_o),
    .db_flush_i                (db_flush_i),
    .db_flush_done_o           (db_flush_done_o),
    .dbg_ram_ptr_i             ('0),
    .dbg_db_read_valid_i       (1'b0),
    .dbg_db_read_data_o        (),
    .dbg_db_write_data_i       ('0),
    .dbg_db_write_valid_i      (1'b0),
    .dbg_db_meta_read_data_o   (),
    .dbg_db_meta_write_data_i  ('0),
    .dbg_db_meta_write_valid_i (1'b0)
  );

endmodule
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNUSEDSIGNAL */
