// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_set_calc_tb (
  input  logic [`SC_SET_ID_SIZE-1:0]      region_op_i,
  input  logic [`SC_SET_ID_SIZE-1:0]      address_set_i,
  input  logic [`SC_SET_ID_SIZE-1:0]      esr_set_base_i,
  input  logic [`SC_SET_ALLOC_SIZE-1:0]   esr_set_size_i,
  input  logic [`SC_SET_ID_SIZE-1:0]      esr_set_mask_i,
  input  logic [`SC_SET_ID_SIZE-1:0]      esr_tag_mask_i,
  output logic [`SC_SET_ID_SIZE-1:0]      new_pipe_set_o,
  output logic [`SC_SET_ID_SIZE-1:0]      orig_pipe_set_o
);

  shirecache_pipe_set_calc u_new (
    .region_op_i    (region_op_i[0]),
    .address_set_i,
    .esr_set_base_i,
    .esr_set_size_i,
    .esr_set_mask_i,
    .esr_tag_mask_i,
    .pipe_set_o     (new_pipe_set_o)
  );

  shire_cache_pipe_set_calc u_orig (
    .region_operation (region_op_i[0]),
    .address_set      (address_set_i),
    .esr_sc_set_base  (esr_set_base_i),
    .esr_sc_set_size  (esr_set_size_i),
    .esr_sc_set_mask  (esr_set_mask_i),
    .esr_sc_tag_mask  (esr_tag_mask_i),
    .pipe_set         (orig_pipe_set_o)
  );

endmodule
