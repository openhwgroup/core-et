// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_evict_ff_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                               clk_i,
  input  logic                               rst_ni,
  input  logic [NumShireIdsBits-1:0]         shire_id_i,
  input  logic [1:0]                         neigh_id_i,
  input  logic [4:0]                         l2_req_id_i,
  input  logic                               l2_req_source_i,
  input  logic                               l2_req_wdata_i,
  input  logic [4:0]                         l2_req_opcode_i,
  input  logic [PaSize-1:0]                  l2_req_address_i,
  input  logic [63:0]                        l2_req_data0_i,
  input  logic [63:0]                        l2_req_data1_i,
  input  logic [63:0]                        l2_req_data2_i,
  input  logic [63:0]                        l2_req_data3_i,
  input  logic [2:0]                         l2_req_size_i,
  input  logic [EtLinkQwenSize-1:0]          l2_req_qwen_i,
  input  logic [EtLinkSubopcodeSize-1:0]     l2_req_subopcode_i,
  input  logic [DcacheL2EvictReqPorts-1:0]   l2_dcache_req_valid_i,
  input  logic [2:0]                         local_channel_mask_i,
  input  logic [2:0]                         local_dest0_i,
  input  logic [2:0]                         local_dest1_i,
  input  logic [2:0]                         local_dest2_i,
  input  logic                               local_message_ready_i,
  input  logic                               evict_ready_i,
  input  logic                               coop_evict_ready_i,

  output logic [DcacheL2EvictReqPorts-1:0]   new_l2_dcache_req_ready_o,
  output logic [2:0]                         new_local_message_valid_o,
  output logic [7:0]                         new_local_message_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  new_local_message_dest_o,
  output logic                               new_local_message_wdata_o,
  output logic [1:0]                         new_local_message_opcode_o,
  output logic [63:0]                        new_local_message_data0_o,
  output logic [63:0]                        new_local_message_data1_o,
  output logic [63:0]                        new_local_message_data2_o,
  output logic [63:0]                        new_local_message_data3_o,
  output logic [2:0]                         new_local_message_size_o,
  output logic [EtLinkQwenSize-1:0]          new_local_message_qwen_o,
  output logic                               new_evict_valid_o,
  output logic [7:0]                         new_evict_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  new_evict_req_source_o,
  output logic                               new_evict_req_wdata_o,
  output logic [4:0]                         new_evict_req_opcode_o,
  output logic [PaSize-1:0]                  new_evict_req_address_o,
  output logic [63:0]                        new_evict_req_data0_o,
  output logic [63:0]                        new_evict_req_data1_o,
  output logic [63:0]                        new_evict_req_data2_o,
  output logic [63:0]                        new_evict_req_data3_o,
  output logic [2:0]                         new_evict_req_size_o,
  output logic [EtLinkQwenSize-1:0]          new_evict_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]     new_evict_req_subopcode_o,
  output logic                               new_coop_evict_valid_o,
  output logic [7:0]                         new_coop_evict_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  new_coop_evict_req_source_o,
  output logic                               new_coop_evict_req_wdata_o,
  output logic [4:0]                         new_coop_evict_req_opcode_o,
  output logic [PaSize-1:0]                  new_coop_evict_req_address_o,
  output logic [63:0]                        new_coop_evict_req_data0_o,
  output logic [63:0]                        new_coop_evict_req_data1_o,
  output logic [63:0]                        new_coop_evict_req_data2_o,
  output logic [63:0]                        new_coop_evict_req_data3_o,
  output logic [2:0]                         new_coop_evict_req_size_o,
  output logic [EtLinkQwenSize-1:0]          new_coop_evict_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]     new_coop_evict_req_subopcode_o,
  output logic [DcacheL2EvictReqPorts-1:0]   new_dbg_sm_signals_o,

  output logic [DcacheL2EvictReqPorts-1:0]   orig_l2_dcache_req_ready_o,
  output logic [2:0]                         orig_local_message_valid_o,
  output logic [7:0]                         orig_local_message_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  orig_local_message_dest_o,
  output logic                               orig_local_message_wdata_o,
  output logic [1:0]                         orig_local_message_opcode_o,
  output logic [63:0]                        orig_local_message_data0_o,
  output logic [63:0]                        orig_local_message_data1_o,
  output logic [63:0]                        orig_local_message_data2_o,
  output logic [63:0]                        orig_local_message_data3_o,
  output logic [2:0]                         orig_local_message_size_o,
  output logic [EtLinkQwenSize-1:0]          orig_local_message_qwen_o,
  output logic                               orig_evict_valid_o,
  output logic [7:0]                         orig_evict_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  orig_evict_req_source_o,
  output logic                               orig_evict_req_wdata_o,
  output logic [4:0]                         orig_evict_req_opcode_o,
  output logic [PaSize-1:0]                  orig_evict_req_address_o,
  output logic [63:0]                        orig_evict_req_data0_o,
  output logic [63:0]                        orig_evict_req_data1_o,
  output logic [63:0]                        orig_evict_req_data2_o,
  output logic [63:0]                        orig_evict_req_data3_o,
  output logic [2:0]                         orig_evict_req_size_o,
  output logic [EtLinkQwenSize-1:0]          orig_evict_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]     orig_evict_req_subopcode_o,
  output logic                               orig_coop_evict_valid_o,
  output logic [7:0]                         orig_coop_evict_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  orig_coop_evict_req_source_o,
  output logic                               orig_coop_evict_req_wdata_o,
  output logic [4:0]                         orig_coop_evict_req_opcode_o,
  output logic [PaSize-1:0]                  orig_coop_evict_req_address_o,
  output logic [63:0]                        orig_coop_evict_req_data0_o,
  output logic [63:0]                        orig_coop_evict_req_data1_o,
  output logic [63:0]                        orig_coop_evict_req_data2_o,
  output logic [63:0]                        orig_coop_evict_req_data3_o,
  output logic [2:0]                         orig_coop_evict_req_size_o,
  output logic [EtLinkQwenSize-1:0]          orig_coop_evict_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]     orig_coop_evict_req_subopcode_o,
  output logic [DcacheL2EvictReqPorts-1:0]   orig_dbg_sm_signals_o
);

  logic reset;
  assign reset = ~rst_ni;

  et_link_minion_evict_req_info_t               orig_l2_req;
  minion_dcache_pkg::et_link_minion_evict_req_info_t new_l2_req;
  logic [2:0][2:0]                              local_dest_id;
  et_link_neigh_rsp_info_t                      orig_local_message_info;
  etlink_pkg::neigh_rsp_t                       new_local_message_info;
  et_link_neigh_req_info_t                      orig_evict_req;
  etlink_pkg::neigh_req_t                       new_evict_req;
  et_link_neigh_req_info_t                      orig_coop_evict_req;
  etlink_pkg::neigh_req_t                       new_coop_evict_req;

  always_comb begin
    orig_l2_req = '0;
    orig_l2_req.id        = et_link_core_miss_id_e'(l2_req_id_i);
    orig_l2_req.source    = l2_req_source_i;
    orig_l2_req.wdata     = l2_req_wdata_i;
    orig_l2_req.opcode    = et_link_req_opcode_e'(l2_req_opcode_i);
    orig_l2_req.address   = l2_req_address_i;
    orig_l2_req.data      = {l2_req_data3_i, l2_req_data2_i, l2_req_data1_i, l2_req_data0_i};
    orig_l2_req.size      = et_link_size_e'(l2_req_size_i);
    orig_l2_req.qwen      = l2_req_qwen_i;
    orig_l2_req.subopcode = l2_req_subopcode_i;

    new_l2_req = '0;
    new_l2_req.id        = minion_dcache_pkg::et_link_core_miss_id_e'(l2_req_id_i);
    new_l2_req.source    = l2_req_source_i;
    new_l2_req.wdata     = l2_req_wdata_i;
    new_l2_req.opcode    = minion_dcache_pkg::et_link_req_opcode_e'(l2_req_opcode_i);
    new_l2_req.address   = l2_req_address_i;
    new_l2_req.data      = {l2_req_data3_i, l2_req_data2_i, l2_req_data1_i, l2_req_data0_i};
    new_l2_req.size      = minion_dcache_pkg::et_link_size_e'(l2_req_size_i);
    new_l2_req.qwen      = l2_req_qwen_i;
    new_l2_req.subopcode = l2_req_subopcode_i;

    local_dest_id[0] = local_dest0_i;
    local_dest_id[1] = local_dest1_i;
    local_dest_id[2] = local_dest2_i;

    new_local_message_id_o     = new_local_message_info.id;
    new_local_message_dest_o   = new_local_message_info.dest;
    new_local_message_wdata_o  = new_local_message_info.wdata;
    new_local_message_opcode_o = new_local_message_info.opcode;
    {new_local_message_data3_o, new_local_message_data2_o, new_local_message_data1_o, new_local_message_data0_o} =
        new_local_message_info.data;
    new_local_message_size_o   = new_local_message_info.size;
    new_local_message_qwen_o   = new_local_message_info.qwen;

    orig_local_message_id_o     = orig_local_message_info.id;
    orig_local_message_dest_o   = orig_local_message_info.dest;
    orig_local_message_wdata_o  = orig_local_message_info.wdata;
    orig_local_message_opcode_o = orig_local_message_info.opcode;
    {orig_local_message_data3_o, orig_local_message_data2_o, orig_local_message_data1_o, orig_local_message_data0_o} =
        orig_local_message_info.data;
    orig_local_message_size_o   = orig_local_message_info.size;
    orig_local_message_qwen_o   = orig_local_message_info.qwen;

    new_evict_req_id_o        = new_evict_req.id;
    new_evict_req_source_o    = new_evict_req.source;
    new_evict_req_wdata_o     = new_evict_req.wdata;
    new_evict_req_opcode_o    = new_evict_req.opcode;
    new_evict_req_address_o   = new_evict_req.address;
    {new_evict_req_data3_o, new_evict_req_data2_o, new_evict_req_data1_o, new_evict_req_data0_o} = new_evict_req.data;
    new_evict_req_size_o      = new_evict_req.size;
    new_evict_req_qwen_o      = new_evict_req.qwen;
    new_evict_req_subopcode_o = new_evict_req.subopcode;

    orig_evict_req_id_o        = orig_evict_req.id;
    orig_evict_req_source_o    = orig_evict_req.source;
    orig_evict_req_wdata_o     = orig_evict_req.wdata;
    orig_evict_req_opcode_o    = orig_evict_req.opcode;
    orig_evict_req_address_o   = orig_evict_req.address;
    {orig_evict_req_data3_o, orig_evict_req_data2_o, orig_evict_req_data1_o, orig_evict_req_data0_o} = orig_evict_req.data;
    orig_evict_req_size_o      = orig_evict_req.size;
    orig_evict_req_qwen_o      = orig_evict_req.qwen;
    orig_evict_req_subopcode_o = orig_evict_req.subopcode;

    new_coop_evict_req_id_o        = new_coop_evict_req.id;
    new_coop_evict_req_source_o    = new_coop_evict_req.source;
    new_coop_evict_req_wdata_o     = new_coop_evict_req.wdata;
    new_coop_evict_req_opcode_o    = new_coop_evict_req.opcode;
    new_coop_evict_req_address_o   = new_coop_evict_req.address;
    {new_coop_evict_req_data3_o, new_coop_evict_req_data2_o, new_coop_evict_req_data1_o, new_coop_evict_req_data0_o} =
        new_coop_evict_req.data;
    new_coop_evict_req_size_o      = new_coop_evict_req.size;
    new_coop_evict_req_qwen_o      = new_coop_evict_req.qwen;
    new_coop_evict_req_subopcode_o = new_coop_evict_req.subopcode;

    orig_coop_evict_req_id_o        = orig_coop_evict_req.id;
    orig_coop_evict_req_source_o    = orig_coop_evict_req.source;
    orig_coop_evict_req_wdata_o     = orig_coop_evict_req.wdata;
    orig_coop_evict_req_opcode_o    = orig_coop_evict_req.opcode;
    orig_coop_evict_req_address_o   = orig_coop_evict_req.address;
    {orig_coop_evict_req_data3_o, orig_coop_evict_req_data2_o, orig_coop_evict_req_data1_o, orig_coop_evict_req_data0_o} =
        orig_coop_evict_req.data;
    orig_coop_evict_req_size_o      = orig_coop_evict_req.size;
    orig_coop_evict_req_qwen_o      = orig_coop_evict_req.qwen;
    orig_coop_evict_req_subopcode_o = orig_coop_evict_req.subopcode;
  end

  neigh_evict_ff u_new (
    .shire_id_i,
    .neigh_id_i,
    .clk_i,
    .rst_ni,
    .l2_dcache_req_i(new_l2_req),
    .l2_dcache_req_valid_i,
    .l2_dcache_req_ready_o(new_l2_dcache_req_ready_o),
    .local_channel_mask_i,
    .local_dest_id_i(local_dest_id),
    .local_message_ready_i,
    .local_message_valid_o(new_local_message_valid_o),
    .local_message_info_o(new_local_message_info),
    .evict_req_o(new_evict_req),
    .evict_valid_o(new_evict_valid_o),
    .evict_ready_i,
    .coop_evict_req_o(new_coop_evict_req),
    .coop_evict_valid_o(new_coop_evict_valid_o),
    .coop_evict_ready_i,
    .dbg_sm_signals_o(new_dbg_sm_signals_o)
  );

  neigh_evict_ff_orig u_orig (
    .clock(clk_i),
    .reset(reset),
    .shire_id(shire_id_i),
    .neigh_id(neigh_id_i),
    .l2_dcache_req(orig_l2_req),
    .l2_dcache_req_valid(l2_dcache_req_valid_i),
    .l2_dcache_req_ready(orig_l2_dcache_req_ready_o),
    .local_channel_mask(local_channel_mask_i),
    .local_dest_id(local_dest_id),
    .local_message_ready(local_message_ready_i),
    .local_message_valid(orig_local_message_valid_o),
    .local_message_info(orig_local_message_info),
    .evict_req(orig_evict_req),
    .evict_valid(orig_evict_valid_o),
    .evict_ready(evict_ready_i),
    .coop_evict_req(orig_coop_evict_req),
    .coop_evict_valid(orig_coop_evict_valid_o),
    .coop_evict_ready(coop_evict_ready_i),
    .dbg_sm_signals(orig_dbg_sm_signals_o)
  );

endmodule
