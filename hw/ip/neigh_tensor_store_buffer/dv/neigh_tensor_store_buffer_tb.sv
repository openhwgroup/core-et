// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_tensor_store_buffer_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [4*$bits(etlink_pkg::neigh_req_t)-1:0] block_req_bits_i,
  input  logic [3:0]                                  block_valid_i,
  output logic [3:0]                                  block_ready_o,
  input  logic                                        block_out_ready_i,
  input  logic                                        block_out_disable_next_i,
  output logic                                        block_out_valid_o,
  output logic                                        block_out_valid_1st_o,
  output logic                                        block_out_valid_2nd_o,
  output logic [$bits(etlink_pkg::neigh_req_t)-1:0]   block_out_req_bits_o,
  input  logic [3:0]                                  block_ack_i,
  input  logic [3:0]                                  block_resp_ready_i,
  output logic [3:0]                                  block_resp_valid_o,
  output logic [4*$bits(minion_pkg::et_link_minion_rsp_info_t)-1:0] block_resp_bits_o,
  output logic [$bits(neigh_pkg::tstore_block_dbg_sm_t)-1:0] block_dbg_bits_o,

  input  logic [MinPerN*$bits(etlink_pkg::neigh_req_t)-1:0] top_req_bits_i,
  input  logic [MinPerN-1:0]                                top_valid_i,
  output logic [MinPerN-1:0]                                top_ready_o,
  input  logic                                              top_out_ready_i,
  input  logic                                              top_out_disable_next_i,
  output logic                                              top_out_valid_o,
  output logic [$bits(etlink_pkg::neigh_req_t)-1:0]         top_out_req_bits_o,
  input  logic [MinPerN-1:0]                                top_ack_i,
  input  logic [MinPerN-1:0]                                top_resp_ready_i,
  output logic [MinPerN-1:0]                                top_resp_valid_o,
  output logic [MinPerN*$bits(minion_pkg::et_link_minion_rsp_info_t)-1:0] top_resp_bits_o,
  output logic [$bits(neigh_pkg::tstore_dbg_sm_t)-1:0]      top_dbg_bits_o
);

  localparam int unsigned ReqBits = $bits(etlink_pkg::neigh_req_t);
  localparam int unsigned RspBits = $bits(minion_pkg::et_link_minion_rsp_info_t);

  etlink_pkg::neigh_req_t [3:0] block_req;
  etlink_pkg::neigh_req_t block_out_req;
  minion_pkg::et_link_minion_rsp_info_t [3:0] block_resp;
  neigh_pkg::tstore_block_dbg_sm_t block_dbg;

  etlink_pkg::neigh_req_t [MinPerN-1:0] top_req;
  etlink_pkg::neigh_req_t top_out_req;
  minion_pkg::et_link_minion_rsp_info_t [MinPerN-1:0] top_resp;
  neigh_pkg::tstore_dbg_sm_t top_dbg;

  always_comb begin
    for (int unsigned m = 0; m < 4; m++) begin
      block_req[m] = etlink_pkg::neigh_req_t'(block_req_bits_i[m*ReqBits +: ReqBits]);
      block_resp_bits_o[m*RspBits +: RspBits] = block_resp[m];
    end
    block_out_req_bits_o = block_out_req;
    block_dbg_bits_o = block_dbg;

    for (int unsigned m = 0; m < MinPerN; m++) begin
      top_req[m] = etlink_pkg::neigh_req_t'(top_req_bits_i[m*ReqBits +: ReqBits]);
      top_resp_bits_o[m*RspBits +: RspBits] = top_resp[m];
    end
    top_out_req_bits_o = top_out_req;
    top_dbg_bits_o = top_dbg;
  end

  neigh_tensor_store_buffer_block u_block (
    .clk_i,
    .rst_ni,
    .evict_req_i              (block_req),
    .evict_valid_i            (block_valid_i),
    .evict_ready_o            (block_ready_o),
    .evict_out_ready_i        (block_out_ready_i),
    .evict_out_disable_next_i (block_out_disable_next_i),
    .evict_out_valid_o        (block_out_valid_o),
    .evict_out_valid_1st_o    (block_out_valid_1st_o),
    .evict_out_valid_2nd_o    (block_out_valid_2nd_o),
    .evict_out_req_o          (block_out_req),
    .tstore_min_ack_i         (block_ack_i),
    .tstore_min_resp_ready_i  (block_resp_ready_i),
    .tstore_min_resp_valid_o  (block_resp_valid_o),
    .tstore_min_resp_o        (block_resp),
    .dbg_sm_signals_o         (block_dbg)
  );

  neigh_tensor_store_buffer u_top (
    .clk_i,
    .rst_ni,
    .evict_req_i              (top_req),
    .evict_valid_i            (top_valid_i),
    .evict_ready_o            (top_ready_o),
    .evict_out_ready_i        (top_out_ready_i),
    .evict_out_disable_next_i (top_out_disable_next_i),
    .evict_out_valid_o        (top_out_valid_o),
    .evict_out_req_o          (top_out_req),
    .tstore_min_ack_i         (top_ack_i),
    .tstore_min_resp_ready_i  (top_resp_ready_i),
    .tstore_min_resp_valid_o  (top_resp_valid_o),
    .tstore_min_resp_o        (top_resp),
    .dbg_sm_signals_o         (top_dbg)
  );

endmodule : neigh_tensor_store_buffer_tb
