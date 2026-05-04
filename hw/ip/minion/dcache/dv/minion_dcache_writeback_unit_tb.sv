// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_writeback_unit.

module minion_dcache_writeback_unit_tb
  import minion_dcache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic                         req_valid_i,
  input  logic [PaTagExtSize-1:0]      req_tag_i,
  input  logic [DcacheSetIdxWidth-1:0] req_set_i,
  input  logic [DcacheWays-1:0]        req_way_en_i,
  input  logic                         req_is_tbox_i,
  input  logic                         req_mem_idx_i,
  input  logic [PaNoTagExtSize-1:0]    req_addr_low_i,
  output logic                         req_ready_o,
  output logic                         data_req_bid_o,
  input  logic                         data_req_grant_i,
  output logic                         data_req_valid_o,
  output logic                         data_req_valid_l_o,
  output logic                         data_req_valid_h_o,
  output logic [DcacheWayIdxWidth-1:0] data_req_way_o,
  output logic [DcacheLramAddrWidth-1:0] data_req_addr_o,
  input  logic                         data_req_nack_i,
  input  logic [63:0]                  data_resp_bank0_i,
  input  logic [63:0]                  data_resp_bank1_i,
  input  logic [63:0]                  data_resp_bank2_i,
  input  logic [63:0]                  data_resp_bank3_i,
  input  logic                         l2_req_ready_i,
  output logic                         l2_req_valid_o,
  output logic [4:0]                   l2_req_id_o,
  output logic                         l2_req_source_o,
  output logic                         l2_req_wdata_o,
  output logic [4:0]                   l2_req_opcode_o,
  output logic [PaSize-1:0]            l2_req_address_o,
  output logic [CoreL2BlockExtSize-1:0] l2_req_data_o,
  output logic [2:0]                   l2_req_size_o,
  output logic [EtLinkQwenSize-1:0]    l2_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0] l2_req_subopcode_o
);

  dcache_wb_req_t req_data;
  dcache_da_read_req_t data_req;
  dcache_da_data_t data_resp;
  et_link_minion_evict_req_info_t l2_req_data;

  always_comb begin
    req_data.tag = req_tag_i;
    req_data.set = req_set_i;
    req_data.way_en = req_way_en_i;
    req_data.is_tbox = req_is_tbox_i;
    req_data.mem_idx = req_mem_idx_i;
    req_data.addr_low = req_addr_low_i;

    data_resp[0] = data_resp_bank0_i;
    data_resp[1] = data_resp_bank1_i;
    data_resp[2] = data_resp_bank2_i;
    data_resp[3] = data_resp_bank3_i;

    data_req_valid_l_o = data_req.valid_l;
    data_req_valid_h_o = data_req.valid_h;
    data_req_way_o = data_req.way;
    data_req_addr_o = data_req.addr;

    l2_req_id_o = l2_req_data.id;
    l2_req_source_o = l2_req_data.source;
    l2_req_wdata_o = l2_req_data.wdata;
    l2_req_opcode_o = l2_req_data.opcode;
    l2_req_address_o = l2_req_data.address;
    l2_req_data_o = l2_req_data.data;
    l2_req_size_o = l2_req_data.size;
    l2_req_qwen_o = l2_req_data.qwen;
    l2_req_subopcode_o = l2_req_data.subopcode;
  end

  minion_dcache_writeback_unit u_dut (
    .clk_i,
    .rst_ni,
    .req_ready_o,
    .req_valid_i,
    .req_data_i       (req_data),
    .data_req_bid_o,
    .data_req_grant_i,
    .data_req_valid_o,
    .data_req_o       (data_req),
    .data_req_nack_i,
    .data_resp_i      (data_resp),
    .l2_req_ready_i,
    .l2_req_valid_o,
    .l2_req_data_o    (l2_req_data)
  );

endmodule : minion_dcache_writeback_unit_tb
