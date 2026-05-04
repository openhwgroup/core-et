// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_buffer_array vs new minion_dcache_buffer_array.

`include "soc.vh"

module cosim_minion_dcache_buffer_array_tb (
  input  logic         clk_i,
  input  logic         rst_ni,
  output logic         new_full_o,
  output logic         new_reduce_full_o,
  output logic         orig_full_o,
  output logic         orig_reduce_full_o,
  input  logic         alloc_req_pre_i,
  input  logic         alloc_req_val_i,
  input  logic         alloc_gsc_i,
  input  logic         reduce_alloc_req_i,
  input  logic         s2_write_needed_core_i,
  input  logic         s2_write_needed_i,
  input  logic         s2_write_en_i,
  input  logic         s2_write_replay_i,
  input  logic [2:0]   s2_write_replay_entry_i,
  input  logic [255:0] s3_write_data_i,
  output logic [2:0]   new_s2_write_entry_o,
  output logic         new_s3_write_en_o,
  output logic [2:0]   orig_s2_write_entry_o,
  output logic         orig_s3_write_en_o,
  input  logic         s3_write_en_ext_i,
  input  logic         s2_write_en_ext_prev_i,
  input  logic [2:0]   s3_write_entry_ext_i,
  input  logic [255:0] s3_write_data_ext_i,
  input  logic         s2_dealloc_i,
  input  logic [2:0]   s2_dealloc_entry_i,
  input  logic         s1_dealloc_uc_i,
  input  logic [2:0]   s1_dealloc_uc_entry_i,
  input  logic         reduce_dealloc_req_i,
  input  logic [2:0]   reduce_dealloc_entry_i,
  input  logic [2:0]   s1_read_entry_i,
  output logic [255:0] new_s1_read_data_o,
  output logic [255:0] orig_s1_read_data_o
);

  logic reset;
  assign reset = ~rst_ni;

  minion_dcache_buffer_array u_new (
    .clk_i,
    .rst_ni,
    .full_o                (new_full_o),
    .reduce_full_o         (new_reduce_full_o),
    .alloc_req_pre_i,
    .alloc_req_val_i,
    .alloc_gsc_i,
    .reduce_alloc_req_i,
    .s2_write_needed_core_i,
    .s2_write_needed_i,
    .s2_write_en_i,
    .s2_write_replay_i,
    .s2_write_replay_entry_i,
    .s3_write_data_i,
    .s2_write_entry_o      (new_s2_write_entry_o),
    .s3_write_en_o         (new_s3_write_en_o),
    .s3_write_en_ext_i,
    .s2_write_en_ext_prev_i,
    .s3_write_entry_ext_i,
    .s3_write_data_ext_i,
    .s2_dealloc_i,
    .s2_dealloc_entry_i,
    .s1_dealloc_uc_i,
    .s1_dealloc_uc_entry_i,
    .reduce_dealloc_req_i,
    .reduce_dealloc_entry_i,
    .s1_read_entry_i,
    .s1_read_data_o        (new_s1_read_data_o)
  );

  dcache_buffer_array u_orig (
    .clock                 (clk_i),
    .reset                 (reset),
    .full                  (orig_full_o),
    .reduce_full           (orig_reduce_full_o),
    .alloc_req_pre         (alloc_req_pre_i),
    .alloc_req_val         (alloc_req_val_i),
    .alloc_gsc             (alloc_gsc_i),
    .reduce_alloc_req      (reduce_alloc_req_i),
    .s2_write_needed_core  (s2_write_needed_core_i),
    .s2_write_needed       (s2_write_needed_i),
    .s2_write_en           (s2_write_en_i),
    .s2_write_replay       (s2_write_replay_i),
    .s2_write_replay_entry (s2_write_replay_entry_i),
    .s3_write_data         (s3_write_data_i),
    .s2_write_entry        (orig_s2_write_entry_o),
    .s3_write_en           (orig_s3_write_en_o),
    .s3_write_en_ext       (s3_write_en_ext_i),
    .s2_write_en_ext_prev  (s2_write_en_ext_prev_i),
    .s3_write_entry_ext    (s3_write_entry_ext_i),
    .s3_write_data_ext     (s3_write_data_ext_i),
    .s2_dealloc            (s2_dealloc_i),
    .s2_dealloc_entry      (s2_dealloc_entry_i),
    .s1_dealloc_uc         (s1_dealloc_uc_i),
    .s1_dealloc_uc_entry   (s1_dealloc_uc_entry_i),
    .reduce_dealloc_req    (reduce_dealloc_req_i),
    .reduce_dealloc_entry  (reduce_dealloc_entry_i),
    .s1_read_entry         (s1_read_entry_i),
    .s1_read_data          (orig_s1_read_data_o)
  );

endmodule
