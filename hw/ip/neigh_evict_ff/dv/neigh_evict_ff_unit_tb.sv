// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_evict_ff_unit_tb
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
  input  logic                               l2_dcache_req_valid_i,
  output logic                               l2_dcache_req_ready_o,
  input  logic [2:0]                         local_channel_mask_i,
  input  logic [2:0]                         local_dest0_i,
  input  logic [2:0]                         local_dest1_i,
  input  logic [2:0]                         local_dest2_i,
  input  logic                               local_message_ready_i,
  output logic [2:0]                         local_message_valid_o,
  output logic [7:0]                         local_message_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  local_message_dest_o,
  output logic                               local_message_wdata_o,
  output logic [1:0]                         local_message_opcode_o,
  output logic [63:0]                        local_message_data0_o,
  output logic [63:0]                        local_message_data1_o,
  output logic [63:0]                        local_message_data2_o,
  output logic [63:0]                        local_message_data3_o,
  output logic [2:0]                         local_message_size_o,
  output logic [EtLinkQwenSize-1:0]          local_message_qwen_o,
  output logic                               evict_valid_o,
  input  logic                               evict_ready_i,
  output logic [7:0]                         evict_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]  evict_req_source_o,
  output logic                               evict_req_wdata_o,
  output logic [4:0]                         evict_req_opcode_o,
  output logic [PaSize-1:0]                  evict_req_address_o,
  output logic [63:0]                        evict_req_data0_o,
  output logic [63:0]                        evict_req_data1_o,
  output logic [63:0]                        evict_req_data2_o,
  output logic [63:0]                        evict_req_data3_o,
  output logic [2:0]                         evict_req_size_o,
  output logic [EtLinkQwenSize-1:0]          evict_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]     evict_req_subopcode_o,
  output logic                               dbg_sm_signals_o
);

  et_link_minion_evict_req_info_t l2_req;
  logic [2:0][2:0]                local_dest_id;
  etlink_pkg::neigh_rsp_t         local_message_info;
  etlink_pkg::neigh_req_t         evict_req;

  always_comb begin
    l2_req = '0;
    l2_req.id        = et_link_core_miss_id_e'(l2_req_id_i);
    l2_req.source    = l2_req_source_i;
    l2_req.wdata     = l2_req_wdata_i;
    l2_req.opcode    = et_link_req_opcode_e'(l2_req_opcode_i);
    l2_req.address   = l2_req_address_i;
    l2_req.data      = {l2_req_data3_i, l2_req_data2_i, l2_req_data1_i, l2_req_data0_i};
    l2_req.size      = et_link_size_e'(l2_req_size_i);
    l2_req.qwen      = l2_req_qwen_i;
    l2_req.subopcode = l2_req_subopcode_i;

    local_dest_id[0] = local_dest0_i;
    local_dest_id[1] = local_dest1_i;
    local_dest_id[2] = local_dest2_i;

    local_message_id_o     = local_message_info.id;
    local_message_dest_o   = local_message_info.dest;
    local_message_wdata_o  = local_message_info.wdata;
    local_message_opcode_o = local_message_info.opcode;
    {local_message_data3_o, local_message_data2_o, local_message_data1_o, local_message_data0_o} =
        local_message_info.data;
    local_message_size_o   = local_message_info.size;
    local_message_qwen_o   = local_message_info.qwen;

    evict_req_id_o         = evict_req.id;
    evict_req_source_o     = evict_req.source;
    evict_req_wdata_o      = evict_req.wdata;
    evict_req_opcode_o     = evict_req.opcode;
    evict_req_address_o    = evict_req.address;
    {evict_req_data3_o, evict_req_data2_o, evict_req_data1_o, evict_req_data0_o} = evict_req.data;
    evict_req_size_o       = evict_req.size;
    evict_req_qwen_o       = evict_req.qwen;
    evict_req_subopcode_o  = evict_req.subopcode;
  end

  neigh_evict_ff_unit u_dut (
    .clk_i,
    .rst_ni,
    .shire_id_i,
    .neigh_id_i,
    .l2_dcache_req_i(l2_req),
    .l2_dcache_req_valid_i,
    .l2_dcache_req_ready_o,
    .local_channel_mask_i,
    .local_dest_id_i(local_dest_id),
    .local_message_ready_i,
    .local_message_valid_o,
    .local_message_info_o(local_message_info),
    .evict_req_o(evict_req),
    .evict_valid_o,
    .evict_ready_i,
    .dbg_sm_signals_o
  );

endmodule
