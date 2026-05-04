// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_depth_buffer.
// Uses single clock for both clk_i and clk_rbox_i (simplifies testing).
// Ties off ram_cfg/dft/debug/SC-response interfaces.

/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off PINCONNECTEMPTY */
module rbox_depth_buffer_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Fetch interface
  input  logic [39:0]       db_fetch_address_i,
  input  logic [6:0]        db_fetch_quad_i,
  input  logic              db_fetch_valid_i,
  output logic [5:0]        db_fetch_ptr_o,
  output logic              db_fetch_done_o,

  // Read interface
  input  logic [5:0]        db_read_ptr_i,
  input  logic              db_read_valid_i,
  output logic [511:0]      db_read_data_o,
  output logic              db_read_data_valid_o,

  // Write interface
  input  logic [5:0]        db_write_ptr_i,
  input  logic [511:0]      db_write_data_i,
  input  logic              db_write_valid_i,
  output logic              db_write_done_o,

  // Dealloc interface
  input  logic [5:0]        db_dealloc_ptr_i,
  input  logic [6:0]        db_dealloc_quad_i,
  input  logic              db_dealloc_valid_i,

  // Flush
  input  logic              db_flush_i,
  output logic              db_flush_done_o,

  // SC req (observe)
  output logic              db_sc_req_valid_any_o
);

  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  etlink_pkg::req_t  db_sc_req;
  logic [ScBanks:0]  db_sc_req_valid;
  logic              db_sc_req_ready;

  etlink_pkg::rsp_t  sc_db_rsp;
  logic              sc_db_rsp_valid;

  assign db_sc_req_valid_any_o = |db_sc_req_valid;

  // Always ready to accept SC requests (no backpressure in test)
  assign db_sc_req_ready = 1'b1;
  assign sc_db_rsp_valid = 1'b0;
  assign sc_db_rsp = '0;

  rbox_depth_buffer u_dut (
    .clk_i                     (clk_i),
    .clk_rbox_i                (clk_i),      // Same clock for simplicity
    .rst_ni                    (rst_ni),
    .ram_cfg_i                 ('0),
    .dft_i                     ('0),
    .db_sc_req_o               (db_sc_req),
    .db_sc_req_valid_o         (db_sc_req_valid),
    .db_sc_req_ready_i         (db_sc_req_ready),
    .sc_db_rsp_i               (sc_db_rsp),
    .sc_db_rsp_valid_i         (sc_db_rsp_valid),
    .db_fetch_address_i        (db_fetch_address_i),
    .db_fetch_quad_i           (db_fetch_quad_i),
    .db_fetch_valid_i          (db_fetch_valid_i),
    .db_fetch_ptr_o            (db_fetch_ptr_o),
    .db_fetch_done_o           (db_fetch_done_o),
    .db_read_ptr_i             (db_read_ptr_i),
    .db_read_valid_i           (db_read_valid_i),
    .db_read_data_o            (db_read_data_o),
    .db_read_data_valid_o      (db_read_data_valid_o),
    .db_write_ptr_i            (db_write_ptr_i),
    .db_write_data_i           (db_write_data_i),
    .db_write_valid_i          (db_write_valid_i),
    .db_write_done_o           (db_write_done_o),
    .db_dealloc_ptr_i          (db_dealloc_ptr_i),
    .db_dealloc_quad_i         (db_dealloc_quad_i),
    .db_dealloc_valid_i        (db_dealloc_valid_i),
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
