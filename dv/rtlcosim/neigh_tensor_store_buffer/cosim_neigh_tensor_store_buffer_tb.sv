// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_tensor_store_buffer_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [MinPerN*$bits(etlink_pkg::neigh_req_t)-1:0] evict_req_bits_i,
  input  logic [MinPerN-1:0]                                evict_valid_i,
  input  logic                                              evict_out_ready_i,
  input  logic                                              evict_out_disable_next_i,
  input  logic [MinPerN-1:0]                                tstore_min_ack_i,
  input  logic [MinPerN-1:0]                                tstore_min_resp_ready_i,

  output logic [MinPerN-1:0]                                orig_evict_ready_o,
  output logic [MinPerN-1:0]                                new_evict_ready_o,
  output logic                                              orig_evict_out_valid_o,
  output logic                                              new_evict_out_valid_o,
  output logic [$bits(etlink_pkg::neigh_req_t)-1:0]         orig_evict_out_req_bits_o,
  output logic [$bits(etlink_pkg::neigh_req_t)-1:0]         new_evict_out_req_bits_o,
  output logic [MinPerN-1:0]                                orig_tstore_min_resp_valid_o,
  output logic [MinPerN-1:0]                                new_tstore_min_resp_valid_o,
  output logic [MinPerN*$bits(minion_pkg::et_link_minion_rsp_info_t)-1:0] orig_tstore_min_resp_bits_o,
  output logic [MinPerN*$bits(minion_pkg::et_link_minion_rsp_info_t)-1:0] new_tstore_min_resp_bits_o,
  output logic [$bits(neigh_pkg::tstore_dbg_sm_t)-1:0]      orig_dbg_bits_o,
  output logic [$bits(neigh_pkg::tstore_dbg_sm_t)-1:0]      new_dbg_bits_o
);

  localparam int unsigned ReqBits = $bits(etlink_pkg::neigh_req_t);
  localparam int unsigned RspBits = $bits(minion_pkg::et_link_minion_rsp_info_t);

  et_link_neigh_req_info_t [MinPerN-1:0] orig_evict_req;
  et_link_neigh_req_info_t orig_evict_out_req;
  et_link_minion_rsp_info_t [MinPerN-1:0] orig_tstore_min_resp;
  tstore_dbg_sm_t orig_dbg;

  etlink_pkg::neigh_req_t [MinPerN-1:0] new_evict_req;
  etlink_pkg::neigh_req_t new_evict_out_req;
  minion_pkg::et_link_minion_rsp_info_t [MinPerN-1:0] new_tstore_min_resp;
  neigh_pkg::tstore_dbg_sm_t new_dbg;

  always_comb begin
    for (int unsigned m = 0; m < MinPerN; m++) begin
      orig_evict_req[m] = et_link_neigh_req_info_t'(evict_req_bits_i[m*ReqBits +: ReqBits]);
      new_evict_req[m] = etlink_pkg::neigh_req_t'(evict_req_bits_i[m*ReqBits +: ReqBits]);
      orig_tstore_min_resp_bits_o[m*RspBits +: RspBits] = orig_tstore_min_resp[m];
      new_tstore_min_resp_bits_o[m*RspBits +: RspBits] = new_tstore_min_resp[m];
    end
    orig_evict_out_req_bits_o = orig_evict_out_req;
    new_evict_out_req_bits_o = new_evict_out_req;
    orig_dbg_bits_o = orig_dbg;
    new_dbg_bits_o = new_dbg;
  end

  neigh_tensor_store_buffer_orig u_orig (
    .clock                  (clk_i),
    .reset                  (!rst_ni),
    .evict_req              (orig_evict_req),
    .evict_valid            (evict_valid_i),
    .evict_ready            (orig_evict_ready_o),
    .evict_out_ready        (evict_out_ready_i),
    .evict_out_disable_next (evict_out_disable_next_i),
    .evict_out_valid        (orig_evict_out_valid_o),
    .evict_out_req          (orig_evict_out_req),
    .tstore_min_ack         (tstore_min_ack_i),
    .tstore_min_resp_ready  (tstore_min_resp_ready_i),
    .tstore_min_resp_valid  (orig_tstore_min_resp_valid_o),
    .tstore_min_resp        (orig_tstore_min_resp),
    .dbg_sm_signals         (orig_dbg)
  );

  neigh_tensor_store_buffer u_new (
    .clk_i,
    .rst_ni,
    .evict_req_i              (new_evict_req),
    .evict_valid_i            (evict_valid_i),
    .evict_ready_o            (new_evict_ready_o),
    .evict_out_ready_i        (evict_out_ready_i),
    .evict_out_disable_next_i (evict_out_disable_next_i),
    .evict_out_valid_o        (new_evict_out_valid_o),
    .evict_out_req_o          (new_evict_out_req),
    .tstore_min_ack_i         (tstore_min_ack_i),
    .tstore_min_resp_ready_i  (tstore_min_resp_ready_i),
    .tstore_min_resp_valid_o  (new_tstore_min_resp_valid_o),
    .tstore_min_resp_o        (new_tstore_min_resp),
    .dbg_sm_signals_o         (new_dbg)
  );

endmodule : cosim_neigh_tensor_store_buffer_tb
