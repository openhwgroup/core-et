// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_tbox_router_tb
  import etlink_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [$bits(neigh_req_t)-1:0] min_tbox_req_info_bits_i,
  input  logic min_tbox_req_valid_early_i,
  input  logic min_tbox_req_valid_i,
  input  logic tbox_min_rsp_ready_i,
  input  logic tbox_sc_req_disable_next_i,
  input  logic tbox_sc_req_ready_i,
  input  logic [$bits(rsp_t)-1:0] sc_tbox_rsp_info_bits_i,
  input  logic sc_tbox_rsp_valid_i,
  input  logic [$bits(neigh_req_t)-1:0] tbox_neigh_req_info_bits_i,
  input  logic tbox_neigh_req_valid_i,
  input  logic tbox_neigh_rsp_ready_i,

  output logic orig_min_tbox_req_ready_o,
  output logic new_min_tbox_req_ready_o,
  output logic [$bits(neigh_rsp_t)-1:0] orig_tbox_min_rsp_info_bits_o,
  output logic [$bits(neigh_rsp_t)-1:0] new_tbox_min_rsp_info_bits_o,
  output logic orig_tbox_min_rsp_valid_o,
  output logic new_tbox_min_rsp_valid_o,
  output logic [$bits(req_nodata_t)-1:0] orig_tbox_sc_req_info_bits_o,
  output logic [$bits(req_nodata_t)-1:0] new_tbox_sc_req_info_bits_o,
  output logic orig_tbox_sc_req_valid_o,
  output logic new_tbox_sc_req_valid_o,
  output logic orig_sc_tbox_rsp_ready_o,
  output logic new_sc_tbox_rsp_ready_o,
  output logic orig_tbox_neigh_req_ready_o,
  output logic new_tbox_neigh_req_ready_o,
  output logic [$bits(rsp_t)-1:0] orig_tbox_neigh_rsp_info_bits_o,
  output logic [$bits(rsp_t)-1:0] new_tbox_neigh_rsp_info_bits_o,
  output logic orig_tbox_neigh_rsp_valid_o,
  output logic new_tbox_neigh_rsp_valid_o
);

  neigh_req_t min_tbox_req_info_new;
  neigh_req_t tbox_neigh_req_info_new;
  rsp_t sc_tbox_rsp_info_new;
  neigh_rsp_t tbox_min_rsp_info_new;
  req_nodata_t tbox_sc_req_info_new;
  rsp_t tbox_neigh_rsp_info_new;

  et_link_neigh_req_info_t min_tbox_req_info_orig;
  et_link_neigh_req_info_t tbox_neigh_req_info_orig;
  et_link_rsp_info_t sc_tbox_rsp_info_orig;
  et_link_neigh_rsp_info_t tbox_min_rsp_info_orig;
  et_link_nodata_req_info_t tbox_sc_req_info_orig;
  et_link_rsp_info_t tbox_neigh_rsp_info_orig;

  always_comb begin
    min_tbox_req_info_new = neigh_req_t'(min_tbox_req_info_bits_i);
    tbox_neigh_req_info_new = neigh_req_t'(tbox_neigh_req_info_bits_i);
    sc_tbox_rsp_info_new = rsp_t'(sc_tbox_rsp_info_bits_i);
    min_tbox_req_info_orig = et_link_neigh_req_info_t'(min_tbox_req_info_bits_i);
    tbox_neigh_req_info_orig = et_link_neigh_req_info_t'(tbox_neigh_req_info_bits_i);
    sc_tbox_rsp_info_orig = et_link_rsp_info_t'(sc_tbox_rsp_info_bits_i);

    orig_tbox_min_rsp_info_bits_o = tbox_min_rsp_info_orig;
    new_tbox_min_rsp_info_bits_o = tbox_min_rsp_info_new;
    orig_tbox_sc_req_info_bits_o = tbox_sc_req_info_orig;
    new_tbox_sc_req_info_bits_o = tbox_sc_req_info_new;
    orig_tbox_neigh_rsp_info_bits_o = tbox_neigh_rsp_info_orig;
    new_tbox_neigh_rsp_info_bits_o = tbox_neigh_rsp_info_new;
  end

  neigh_tbox_router_orig u_orig (
    .clock                      (clk_i),
    .reset                      (!rst_ni),
    .min_tbox_req_info          (min_tbox_req_info_orig),
    .min_tbox_req_valid_early   (min_tbox_req_valid_early_i),
    .min_tbox_req_valid         (min_tbox_req_valid_i),
    .min_tbox_req_ready         (orig_min_tbox_req_ready_o),
    .tbox_min_rsp_info          (tbox_min_rsp_info_orig),
    .tbox_min_rsp_valid         (orig_tbox_min_rsp_valid_o),
    .tbox_min_rsp_ready         (tbox_min_rsp_ready_i),
    .tbox_sc_req_disable_next   (tbox_sc_req_disable_next_i),
    .tbox_sc_req_info           (tbox_sc_req_info_orig),
    .tbox_sc_req_valid          (orig_tbox_sc_req_valid_o),
    .tbox_sc_req_ready          (tbox_sc_req_ready_i),
    .sc_tbox_rsp_info           (sc_tbox_rsp_info_orig),
    .sc_tbox_rsp_valid          (sc_tbox_rsp_valid_i),
    .sc_tbox_rsp_ready          (orig_sc_tbox_rsp_ready_o),
    .tbox_neigh_req_info        (tbox_neigh_req_info_orig),
    .tbox_neigh_req_valid       (tbox_neigh_req_valid_i),
    .tbox_neigh_req_ready       (orig_tbox_neigh_req_ready_o),
    .tbox_neigh_rsp_info        (tbox_neigh_rsp_info_orig),
    .tbox_neigh_rsp_valid       (orig_tbox_neigh_rsp_valid_o),
    .tbox_neigh_rsp_ready       (tbox_neigh_rsp_ready_i)
  );

  neigh_tbox_router u_new (
    .clk_i,
    .rst_ni,
    .min_tbox_req_info_i          (min_tbox_req_info_new),
    .min_tbox_req_valid_early_i   (min_tbox_req_valid_early_i),
    .min_tbox_req_valid_i         (min_tbox_req_valid_i),
    .min_tbox_req_ready_o         (new_min_tbox_req_ready_o),
    .tbox_min_rsp_info_o          (tbox_min_rsp_info_new),
    .tbox_min_rsp_valid_o         (new_tbox_min_rsp_valid_o),
    .tbox_min_rsp_ready_i         (tbox_min_rsp_ready_i),
    .tbox_sc_req_disable_next_i   (tbox_sc_req_disable_next_i),
    .tbox_sc_req_info_o           (tbox_sc_req_info_new),
    .tbox_sc_req_valid_o          (new_tbox_sc_req_valid_o),
    .tbox_sc_req_ready_i          (tbox_sc_req_ready_i),
    .sc_tbox_rsp_info_i           (sc_tbox_rsp_info_new),
    .sc_tbox_rsp_valid_i          (sc_tbox_rsp_valid_i),
    .sc_tbox_rsp_ready_o          (new_sc_tbox_rsp_ready_o),
    .tbox_neigh_req_info_i        (tbox_neigh_req_info_new),
    .tbox_neigh_req_valid_i       (tbox_neigh_req_valid_i),
    .tbox_neigh_req_ready_o       (new_tbox_neigh_req_ready_o),
    .tbox_neigh_rsp_info_o        (tbox_neigh_rsp_info_new),
    .tbox_neigh_rsp_valid_o       (new_tbox_neigh_rsp_valid_o),
    .tbox_neigh_rsp_ready_i       (tbox_neigh_rsp_ready_i)
  );

endmodule : cosim_neigh_tbox_router_tb
