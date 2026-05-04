// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_input_fifo.
// Module already has clk_i/rst_ni. Ties off ram_cfg/dft and decomposes
// the inpck_type_e output for C++ access.

module rbox_input_fifo_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  output logic [4:0]        alloc_ptr_o,
  input  logic              alloc_i,

  input  logic [511:0]      in_data_i,
  input  logic [4:0]        push_ptr_i,
  input  logic              push_i,
  output logic              push_done_o,

  output logic [511:0]      out_data_o,
  output logic [2:0]        out_pck_type_o,
  output logic              out_valid_o,
  input  logic              pop_i,
  output logic              pop_done_o,

  input  logic              buffer_end_i,

  output logic              full_o,
  output logic              empty_o
);

  import rbox_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  inpck_type_e out_pck_type_int;
  assign out_pck_type_o = out_pck_type_int;

  rbox_input_fifo u_dut (
    .clk_i          (clk_i),
    .rst_ni         (rst_ni),
    .ram_cfg_i      ('0),
    .dft_i          ('0),
    .alloc_ptr_o    (alloc_ptr_o),
    .alloc_i        (alloc_i),
    .in_data_i      (in_data_i),
    .push_ptr_i     (push_ptr_i),
    .push_i         (push_i),
    .push_done_o    (push_done_o),
    .out_data_o     (out_data_o),
    .out_pck_type_o (out_pck_type_int),
    .out_valid_o    (out_valid_o),
    .pop_i          (pop_i),
    .pop_done_o     (pop_done_o),
    .buffer_end_i   (buffer_end_i),
    .full_o         (full_o),
    .empty_o        (empty_o)
  );

endmodule
