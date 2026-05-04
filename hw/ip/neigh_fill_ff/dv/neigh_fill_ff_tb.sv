// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_fill_ff_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  input  logic [EtLinkIdSize-1:0]     fill_fifo_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] fill_fifo_dest_i,
  input  logic                        fill_fifo_wdata_i,
  input  logic [1:0]                  fill_fifo_opcode_i,
  input  logic [63:0]                 fill_fifo_data0_i,
  input  logic [63:0]                 fill_fifo_data1_i,
  input  logic [63:0]                 fill_fifo_data2_i,
  input  logic [63:0]                 fill_fifo_data3_i,
  input  logic [63:0]                 fill_fifo_data4_i,
  input  logic [63:0]                 fill_fifo_data5_i,
  input  logic [63:0]                 fill_fifo_data6_i,
  input  logic [63:0]                 fill_fifo_data7_i,
  input  logic [2:0]                  fill_fifo_size_i,
  input  logic [3:0]                  fill_fifo_qwen_i,
  input  logic                        fill_fifo_valid_i,
  output logic                        fill_fifo_ready_o,

  input  logic [EtLinkIdSize-1:0]     tstore_id_i,
  input  logic                        tstore_dest_i,
  input  logic                        tstore_wdata_i,
  input  logic [1:0]                  tstore_opcode_i,
  input  logic [63:0]                 tstore_data0_i,
  input  logic [63:0]                 tstore_data1_i,
  input  logic [63:0]                 tstore_data2_i,
  input  logic [63:0]                 tstore_data3_i,
  input  logic [2:0]                  tstore_size_i,
  input  logic [3:0]                  tstore_qwen_i,
  input  logic                        tstore_valid_i,
  output logic                        tstore_ready_o,

  input  logic [EtLinkIdSize-1:0]     local_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] local_dest_i,
  input  logic                        local_wdata_i,
  input  logic [1:0]                  local_opcode_i,
  input  logic [63:0]                 local_data0_i,
  input  logic [63:0]                 local_data1_i,
  input  logic [63:0]                 local_data2_i,
  input  logic [63:0]                 local_data3_i,
  input  logic [2:0]                  local_size_i,
  input  logic [3:0]                  local_qwen_i,
  input  logic                        local_valid_i,
  output logic                        local_ready_o,

  input  logic [EtLinkIdSize-1:0]     tbox_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] tbox_dest_i,
  input  logic                        tbox_wdata_i,
  input  logic [1:0]                  tbox_opcode_i,
  input  logic [63:0]                 tbox_data0_i,
  input  logic [63:0]                 tbox_data1_i,
  input  logic [63:0]                 tbox_data2_i,
  input  logic [63:0]                 tbox_data3_i,
  input  logic [2:0]                  tbox_size_i,
  input  logic [3:0]                  tbox_qwen_i,
  input  logic                        tbox_valid_i,
  output logic                        tbox_ready_o,

  output logic [EtLinkIdSize-1:0]     l2_resp_id_o,
  output logic                        l2_resp_dest_o,
  output logic                        l2_resp_wdata_o,
  output logic [1:0]                  l2_resp_opcode_o,
  output logic [63:0]                 l2_resp_data0_o,
  output logic [63:0]                 l2_resp_data1_o,
  output logic [63:0]                 l2_resp_data2_o,
  output logic [63:0]                 l2_resp_data3_o,
  output logic [2:0]                  l2_resp_size_o,
  output logic [3:0]                  l2_resp_qwen_o,
  output logic                        l2_resp_valid_o,
  input  logic                        l2_resp_ready_i,

  output logic                        tstore_min_ack_o,
  output logic                        dbg_local_message_ready_o,
  output logic                        dbg_tbox_min_rsp_ready_o,
  output logic                        dbg_fill_ff_ready_for_no_msg_o
);

  etlink_pkg::rsp_t             fill_fifo_resp;
  et_link_minion_rsp_info_t     tstore_min_resp;
  etlink_pkg::neigh_rsp_t       local_message_info;
  etlink_pkg::neigh_rsp_t       tbox_min_rsp_info;
  et_link_minion_rsp_info_t     l2_resp;
  fill_ff_dbg_sm_t              dbg_sm_signals;

  always_comb begin
    fill_fifo_resp = '0;
    fill_fifo_resp.id = fill_fifo_id_i;
    fill_fifo_resp.dest = fill_fifo_dest_i;
    fill_fifo_resp.wdata = fill_fifo_wdata_i;
    fill_fifo_resp.opcode = etlink_pkg::rsp_opcode_e'(fill_fifo_opcode_i);
    fill_fifo_resp.data = {fill_fifo_data7_i, fill_fifo_data6_i, fill_fifo_data5_i, fill_fifo_data4_i,
                           fill_fifo_data3_i, fill_fifo_data2_i, fill_fifo_data1_i, fill_fifo_data0_i};
    fill_fifo_resp.size = etlink_pkg::size_e'(fill_fifo_size_i);
    fill_fifo_resp.qwen = fill_fifo_qwen_i;

    tstore_min_resp = '0;
    tstore_min_resp.id = tstore_id_i;
    tstore_min_resp.dest = tstore_dest_i;
    tstore_min_resp.wdata = tstore_wdata_i;
    tstore_min_resp.opcode = et_link_rsp_opcode_e'(tstore_opcode_i);
    tstore_min_resp.data = {tstore_data3_i, tstore_data2_i, tstore_data1_i, tstore_data0_i};
    tstore_min_resp.size = et_link_size_e'(tstore_size_i);
    tstore_min_resp.qwen = tstore_qwen_i;

    local_message_info = '0;
    local_message_info.id = local_id_i;
    local_message_info.dest = local_dest_i;
    local_message_info.wdata = local_wdata_i;
    local_message_info.opcode = etlink_pkg::rsp_opcode_e'(local_opcode_i);
    local_message_info.data = {local_data3_i, local_data2_i, local_data1_i, local_data0_i};
    local_message_info.size = etlink_pkg::size_e'(local_size_i);
    local_message_info.qwen = local_qwen_i;

    tbox_min_rsp_info = '0;
    tbox_min_rsp_info.id = tbox_id_i;
    tbox_min_rsp_info.dest = tbox_dest_i;
    tbox_min_rsp_info.wdata = tbox_wdata_i;
    tbox_min_rsp_info.opcode = etlink_pkg::rsp_opcode_e'(tbox_opcode_i);
    tbox_min_rsp_info.data = {tbox_data3_i, tbox_data2_i, tbox_data1_i, tbox_data0_i};
    tbox_min_rsp_info.size = etlink_pkg::size_e'(tbox_size_i);
    tbox_min_rsp_info.qwen = tbox_qwen_i;

    l2_resp_id_o = l2_resp.id;
    l2_resp_dest_o = l2_resp.dest;
    l2_resp_wdata_o = l2_resp.wdata;
    l2_resp_opcode_o = l2_resp.opcode;
    {l2_resp_data3_o, l2_resp_data2_o, l2_resp_data1_o, l2_resp_data0_o} = l2_resp.data;
    l2_resp_size_o = l2_resp.size;
    l2_resp_qwen_o = l2_resp.qwen;

    dbg_local_message_ready_o = dbg_sm_signals.local_message_ready;
    dbg_tbox_min_rsp_ready_o = dbg_sm_signals.tbox_min_rsp_ready;
    dbg_fill_ff_ready_for_no_msg_o = dbg_sm_signals.fill_ff_ready_for_no_msg;
  end

  neigh_fill_ff u_dut (
    .clk_i,
    .rst_ni,
    .fill_fifo_resp_i(fill_fifo_resp),
    .fill_fifo_valid_i,
    .fill_fifo_ready_o,
    .tstore_min_resp_i(tstore_min_resp),
    .tstore_min_resp_valid_i(tstore_valid_i),
    .tstore_min_resp_ready_o(tstore_ready_o),
    .local_message_info_i(local_message_info),
    .local_message_valid_i(local_valid_i),
    .local_message_ready_o(local_ready_o),
    .tbox_min_rsp_info_i(tbox_min_rsp_info),
    .tbox_min_rsp_valid_i(tbox_valid_i),
    .tbox_min_rsp_ready_o(tbox_ready_o),
    .l2_resp_o(l2_resp),
    .l2_resp_valid_o,
    .l2_resp_ready_i,
    .tstore_min_ack_o,
    .dbg_sm_signals_o(dbg_sm_signals)
  );

endmodule
