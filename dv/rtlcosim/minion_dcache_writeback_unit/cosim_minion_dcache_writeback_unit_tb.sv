// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_writeback_unit vs new
// minion_dcache_writeback_unit.

`include "soc.vh"

module cosim_minion_dcache_writeback_unit_tb
  import minion_dcache_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic                       req_valid_i,
  input  logic [PaTagExtSize-1:0]    req_tag_i,
  input  logic [DcacheSetIdxWidth-1:0] req_set_i,
  input  logic [DcacheWays-1:0]      req_way_en_i,
  input  logic                       req_is_tbox_i,
  input  logic                       req_mem_idx_i,
  input  logic [PaNoTagExtSize-1:0]  req_addr_low_i,
  input  logic                       data_req_grant_i,
  input  logic                       data_req_nack_i,
  input  logic [63:0]                data_resp_bank0_i,
  input  logic [63:0]                data_resp_bank1_i,
  input  logic [63:0]                data_resp_bank2_i,
  input  logic [63:0]                data_resp_bank3_i,
  input  logic                       l2_req_ready_i,
  output logic                       orig_req_ready_o,
  output logic                       new_req_ready_o,
  output logic                       orig_data_req_bid_o,
  output logic                       new_data_req_bid_o,
  output logic                       orig_data_req_valid_o,
  output logic                       new_data_req_valid_o,
  output logic                       orig_data_req_valid_l_o,
  output logic                       new_data_req_valid_l_o,
  output logic                       orig_data_req_valid_h_o,
  output logic                       new_data_req_valid_h_o,
  output logic [DcacheWayIdxWidth-1:0] orig_data_req_way_o,
  output logic [DcacheWayIdxWidth-1:0] new_data_req_way_o,
  output logic [DcacheLramAddrWidth-1:0] orig_data_req_addr_o,
  output logic [DcacheLramAddrWidth-1:0] new_data_req_addr_o,
  output logic                       orig_l2_req_valid_o,
  output logic                       new_l2_req_valid_o,
  output logic [4:0]                 orig_l2_req_id_o,
  output logic [4:0]                 new_l2_req_id_o,
  output logic                       orig_l2_req_source_o,
  output logic                       new_l2_req_source_o,
  output logic                       orig_l2_req_wdata_o,
  output logic                       new_l2_req_wdata_o,
  output logic [2:0]                 orig_l2_req_opcode_o,
  output logic [2:0]                 new_l2_req_opcode_o,
  output logic [PaSize-1:0]          orig_l2_req_address_o,
  output logic [PaSize-1:0]          new_l2_req_address_o,
  output logic [255:0]               orig_l2_req_data_o,
  output logic [255:0]               new_l2_req_data_o,
  output logic [2:0]                 orig_l2_req_size_o,
  output logic [2:0]                 new_l2_req_size_o,
  output logic [EtLinkQwenSize-1:0]  orig_l2_req_qwen_o,
  output logic [EtLinkQwenSize-1:0]  new_l2_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0] orig_l2_req_subopcode_o,
  output logic [EtLinkSubopcodeSize-1:0] new_l2_req_subopcode_o
);

  dcache_wb_req_t req_data_new;
  dcache_wb_req   req_data_orig;
  dcache_da_read_req data_req_orig;
  dcache_da_read_req_t data_req_new;
  dcache_da_data   data_resp_orig;
  dcache_da_data_t data_resp_new;
  et_link_minion_evict_req_info_t   l2_req_data_orig;
  et_link_minion_evict_req_info_t   l2_req_data_new;

  assign req_data_new.tag = req_tag_i;
  assign req_data_new.set = req_set_i;
  assign req_data_new.way_en = req_way_en_i;
  assign req_data_new.is_tbox = req_is_tbox_i;
  assign req_data_new.mem_idx = req_mem_idx_i;
  assign req_data_new.addr_low = req_addr_low_i;

  assign req_data_orig.tag = req_tag_i;
  assign req_data_orig.set = req_set_i;
  assign req_data_orig.way_en = req_way_en_i;
  assign req_data_orig.is_tbox = req_is_tbox_i;
  assign req_data_orig.mem_idx = req_mem_idx_i;
  assign req_data_orig.addr_low = req_addr_low_i;

  assign data_resp_new[0] = data_resp_bank0_i;
  assign data_resp_new[1] = data_resp_bank1_i;
  assign data_resp_new[2] = data_resp_bank2_i;
  assign data_resp_new[3] = data_resp_bank3_i;

  assign data_resp_orig[0] = data_resp_bank0_i;
  assign data_resp_orig[1] = data_resp_bank1_i;
  assign data_resp_orig[2] = data_resp_bank2_i;
  assign data_resp_orig[3] = data_resp_bank3_i;

  minion_dcache_writeback_unit u_new (
    .clk_i,
    .rst_ni,
    .req_ready_o       (new_req_ready_o),
    .req_valid_i,
    .req_data_i        (req_data_new),
    .data_req_bid_o    (new_data_req_bid_o),
    .data_req_grant_i,
    .data_req_valid_o  (new_data_req_valid_o),
    .data_req_o        (data_req_new),
    .data_req_nack_i,
    .data_resp_i       (data_resp_new),
    .l2_req_ready_i,
    .l2_req_valid_o    (new_l2_req_valid_o),
    .l2_req_data_o     (l2_req_data_new)
  );

  dcache_writeback_unit u_orig (
    .clock             (clk_i),
    .reset             (!rst_ni),
    .req_ready         (orig_req_ready_o),
    .req_valid         (req_valid_i),
    .req_data          (req_data_orig),
    .data_req_bid      (orig_data_req_bid_o),
    .data_req_grant    (data_req_grant_i),
    .data_req_valid    (orig_data_req_valid_o),
    .data_req          (data_req_orig),
    .data_req_nack     (data_req_nack_i),
    .data_resp         (data_resp_orig),
    .l2_req_ready      (l2_req_ready_i),
    .l2_req_valid      (orig_l2_req_valid_o),
    .l2_req_data       (l2_req_data_orig)
  );

  assign orig_data_req_valid_l_o = data_req_orig.valid_l;
  assign new_data_req_valid_l_o = data_req_new.valid_l;
  assign orig_data_req_valid_h_o = data_req_orig.valid_h;
  assign new_data_req_valid_h_o = data_req_new.valid_h;
  assign orig_data_req_way_o = data_req_orig.way;
  assign new_data_req_way_o = data_req_new.way;
  assign orig_data_req_addr_o = data_req_orig.addr;
  assign new_data_req_addr_o = data_req_new.addr;

  assign orig_l2_req_id_o = l2_req_data_orig.id;
  assign new_l2_req_id_o = l2_req_data_new.id;
  assign orig_l2_req_source_o = l2_req_data_orig.source;
  assign new_l2_req_source_o = l2_req_data_new.source;
  assign orig_l2_req_wdata_o = l2_req_data_orig.wdata;
  assign new_l2_req_wdata_o = l2_req_data_new.wdata;
  assign orig_l2_req_opcode_o = l2_req_data_orig.opcode;
  assign new_l2_req_opcode_o = l2_req_data_new.opcode;
  assign orig_l2_req_address_o = l2_req_data_orig.address;
  assign new_l2_req_address_o = l2_req_data_new.address;
  assign orig_l2_req_data_o = l2_req_data_orig.data;
  assign new_l2_req_data_o = l2_req_data_new.data;
  assign orig_l2_req_size_o = l2_req_data_orig.size;
  assign new_l2_req_size_o = l2_req_data_new.size;
  assign orig_l2_req_qwen_o = l2_req_data_orig.qwen;
  assign new_l2_req_qwen_o = l2_req_data_new.qwen;
  assign orig_l2_req_subopcode_o = l2_req_data_orig.subopcode;
  assign new_l2_req_subopcode_o = l2_req_data_new.subopcode;

endmodule : cosim_minion_dcache_writeback_unit_tb
