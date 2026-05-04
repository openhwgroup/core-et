// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_fill_ff_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                            clk_i,
  input  logic                            rst_ni,

  input  logic [EtLinkIdSize-1:0]         fill_fifo_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] fill_fifo_dest_i,
  input  logic                            fill_fifo_wdata_i,
  input  logic [1:0]                      fill_fifo_opcode_i,
  input  logic [63:0]                     fill_fifo_data0_i,
  input  logic [63:0]                     fill_fifo_data1_i,
  input  logic [63:0]                     fill_fifo_data2_i,
  input  logic [63:0]                     fill_fifo_data3_i,
  input  logic [63:0]                     fill_fifo_data4_i,
  input  logic [63:0]                     fill_fifo_data5_i,
  input  logic [63:0]                     fill_fifo_data6_i,
  input  logic [63:0]                     fill_fifo_data7_i,
  input  logic [2:0]                      fill_fifo_size_i,
  input  logic [3:0]                      fill_fifo_qwen_i,
  input  logic                            fill_fifo_valid_i,

  input  logic [EtLinkIdSize-1:0]         tstore_id_i,
  input  logic                            tstore_dest_i,
  input  logic                            tstore_wdata_i,
  input  logic [1:0]                      tstore_opcode_i,
  input  logic [63:0]                     tstore_data0_i,
  input  logic [63:0]                     tstore_data1_i,
  input  logic [63:0]                     tstore_data2_i,
  input  logic [63:0]                     tstore_data3_i,
  input  logic [2:0]                      tstore_size_i,
  input  logic [3:0]                      tstore_qwen_i,
  input  logic                            tstore_valid_i,

  input  logic [EtLinkIdSize-1:0]         local_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] local_dest_i,
  input  logic                            local_wdata_i,
  input  logic [1:0]                      local_opcode_i,
  input  logic [63:0]                     local_data0_i,
  input  logic [63:0]                     local_data1_i,
  input  logic [63:0]                     local_data2_i,
  input  logic [63:0]                     local_data3_i,
  input  logic [2:0]                      local_size_i,
  input  logic [3:0]                      local_qwen_i,
  input  logic                            local_valid_i,

  input  logic [EtLinkIdSize-1:0]         tbox_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] tbox_dest_i,
  input  logic                            tbox_wdata_i,
  input  logic [1:0]                      tbox_opcode_i,
  input  logic [63:0]                     tbox_data0_i,
  input  logic [63:0]                     tbox_data1_i,
  input  logic [63:0]                     tbox_data2_i,
  input  logic [63:0]                     tbox_data3_i,
  input  logic [2:0]                      tbox_size_i,
  input  logic [3:0]                      tbox_qwen_i,
  input  logic                            tbox_valid_i,

  input  logic                            l2_resp_ready_i,

  output logic                            new_fill_fifo_ready_o,
  output logic                            new_tstore_ready_o,
  output logic                            new_local_ready_o,
  output logic                            new_tbox_ready_o,
  output logic [EtLinkIdSize-1:0]         new_l2_resp_id_o,
  output logic                            new_l2_resp_dest_o,
  output logic                            new_l2_resp_wdata_o,
  output logic [1:0]                      new_l2_resp_opcode_o,
  output logic [63:0]                     new_l2_resp_data0_o,
  output logic [63:0]                     new_l2_resp_data1_o,
  output logic [63:0]                     new_l2_resp_data2_o,
  output logic [63:0]                     new_l2_resp_data3_o,
  output logic [2:0]                      new_l2_resp_size_o,
  output logic [3:0]                      new_l2_resp_qwen_o,
  output logic                            new_l2_resp_valid_o,
  output logic                            new_tstore_min_ack_o,
  output logic [2:0]                      new_dbg_sm_signals_o,

  output logic                            orig_fill_fifo_ready_o,
  output logic                            orig_tstore_ready_o,
  output logic                            orig_local_ready_o,
  output logic                            orig_tbox_ready_o,
  output logic [EtLinkIdSize-1:0]         orig_l2_resp_id_o,
  output logic                            orig_l2_resp_dest_o,
  output logic                            orig_l2_resp_wdata_o,
  output logic [1:0]                      orig_l2_resp_opcode_o,
  output logic [63:0]                     orig_l2_resp_data0_o,
  output logic [63:0]                     orig_l2_resp_data1_o,
  output logic [63:0]                     orig_l2_resp_data2_o,
  output logic [63:0]                     orig_l2_resp_data3_o,
  output logic [2:0]                      orig_l2_resp_size_o,
  output logic [3:0]                      orig_l2_resp_qwen_o,
  output logic                            orig_l2_resp_valid_o,
  output logic                            orig_tstore_min_ack_o,
  output logic [2:0]                      orig_dbg_sm_signals_o
);

  logic reset;
  assign reset = ~rst_ni;

  etlink_pkg::rsp_t                   new_fill_fifo_resp;
  et_link_rsp_info_t                  orig_fill_fifo_resp;
  minion_pkg::et_link_minion_rsp_info_t new_tstore_min_resp;
  et_link_minion_rsp_info_t           orig_tstore_min_resp;
  etlink_pkg::neigh_rsp_t             new_local_message_info;
  et_link_neigh_rsp_info_t            orig_local_message_info;
  etlink_pkg::neigh_rsp_t             new_tbox_min_rsp_info;
  et_link_neigh_rsp_info_t            orig_tbox_min_rsp_info;
  minion_pkg::et_link_minion_rsp_info_t new_l2_resp;
  et_link_minion_rsp_info_t           orig_l2_resp;
  minion_pkg::fill_ff_dbg_sm_t        new_dbg_sm_signals;
  fill_ff_dbg_sm_t                    orig_dbg_sm_signals;

  always_comb begin
    new_fill_fifo_resp = '0;
    new_fill_fifo_resp.id = fill_fifo_id_i;
    new_fill_fifo_resp.dest = fill_fifo_dest_i;
    new_fill_fifo_resp.wdata = fill_fifo_wdata_i;
    new_fill_fifo_resp.opcode = etlink_pkg::rsp_opcode_e'(fill_fifo_opcode_i);
    new_fill_fifo_resp.data = {fill_fifo_data7_i, fill_fifo_data6_i, fill_fifo_data5_i, fill_fifo_data4_i,
                               fill_fifo_data3_i, fill_fifo_data2_i, fill_fifo_data1_i, fill_fifo_data0_i};
    new_fill_fifo_resp.size = etlink_pkg::size_e'(fill_fifo_size_i);
    new_fill_fifo_resp.qwen = fill_fifo_qwen_i;

    orig_fill_fifo_resp = '0;
    orig_fill_fifo_resp.id = fill_fifo_id_i;
    orig_fill_fifo_resp.dest = fill_fifo_dest_i;
    orig_fill_fifo_resp.wdata = fill_fifo_wdata_i;
    orig_fill_fifo_resp.opcode = et_link_rsp_opcode_e'(fill_fifo_opcode_i);
    orig_fill_fifo_resp.data = {fill_fifo_data7_i, fill_fifo_data6_i, fill_fifo_data5_i, fill_fifo_data4_i,
                                fill_fifo_data3_i, fill_fifo_data2_i, fill_fifo_data1_i, fill_fifo_data0_i};
    orig_fill_fifo_resp.size = et_link_size_e'(fill_fifo_size_i);
    orig_fill_fifo_resp.qwen = fill_fifo_qwen_i;

    new_tstore_min_resp = '0;
    new_tstore_min_resp.id = tstore_id_i;
    new_tstore_min_resp.dest = tstore_dest_i;
    new_tstore_min_resp.wdata = tstore_wdata_i;
    new_tstore_min_resp.opcode = et_link_rsp_opcode_e'(tstore_opcode_i);
    new_tstore_min_resp.data = {tstore_data3_i, tstore_data2_i, tstore_data1_i, tstore_data0_i};
    new_tstore_min_resp.size = et_link_size_e'(tstore_size_i);
    new_tstore_min_resp.qwen = tstore_qwen_i;

    orig_tstore_min_resp = '0;
    orig_tstore_min_resp.id = tstore_id_i;
    orig_tstore_min_resp.dest = tstore_dest_i;
    orig_tstore_min_resp.wdata = tstore_wdata_i;
    orig_tstore_min_resp.opcode = et_link_rsp_opcode_e'(tstore_opcode_i);
    orig_tstore_min_resp.data = {tstore_data3_i, tstore_data2_i, tstore_data1_i, tstore_data0_i};
    orig_tstore_min_resp.size = et_link_size_e'(tstore_size_i);
    orig_tstore_min_resp.qwen = tstore_qwen_i;

    new_local_message_info = '0;
    new_local_message_info.id = local_id_i;
    new_local_message_info.dest = local_dest_i;
    new_local_message_info.wdata = local_wdata_i;
    new_local_message_info.opcode = etlink_pkg::rsp_opcode_e'(local_opcode_i);
    new_local_message_info.data = {local_data3_i, local_data2_i, local_data1_i, local_data0_i};
    new_local_message_info.size = etlink_pkg::size_e'(local_size_i);
    new_local_message_info.qwen = local_qwen_i;

    orig_local_message_info = '0;
    orig_local_message_info.id = local_id_i;
    orig_local_message_info.dest = local_dest_i;
    orig_local_message_info.wdata = local_wdata_i;
    orig_local_message_info.opcode = et_link_rsp_opcode_e'(local_opcode_i);
    orig_local_message_info.data = {local_data3_i, local_data2_i, local_data1_i, local_data0_i};
    orig_local_message_info.size = et_link_size_e'(local_size_i);
    orig_local_message_info.qwen = local_qwen_i;

    new_tbox_min_rsp_info = '0;
    new_tbox_min_rsp_info.id = tbox_id_i;
    new_tbox_min_rsp_info.dest = tbox_dest_i;
    new_tbox_min_rsp_info.wdata = tbox_wdata_i;
    new_tbox_min_rsp_info.opcode = etlink_pkg::rsp_opcode_e'(tbox_opcode_i);
    new_tbox_min_rsp_info.data = {tbox_data3_i, tbox_data2_i, tbox_data1_i, tbox_data0_i};
    new_tbox_min_rsp_info.size = etlink_pkg::size_e'(tbox_size_i);
    new_tbox_min_rsp_info.qwen = tbox_qwen_i;

    orig_tbox_min_rsp_info = '0;
    orig_tbox_min_rsp_info.id = tbox_id_i;
    orig_tbox_min_rsp_info.dest = tbox_dest_i;
    orig_tbox_min_rsp_info.wdata = tbox_wdata_i;
    orig_tbox_min_rsp_info.opcode = et_link_rsp_opcode_e'(tbox_opcode_i);
    orig_tbox_min_rsp_info.data = {tbox_data3_i, tbox_data2_i, tbox_data1_i, tbox_data0_i};
    orig_tbox_min_rsp_info.size = et_link_size_e'(tbox_size_i);
    orig_tbox_min_rsp_info.qwen = tbox_qwen_i;

    new_l2_resp_id_o = new_l2_resp.id;
    new_l2_resp_dest_o = new_l2_resp.dest;
    new_l2_resp_wdata_o = new_l2_resp.wdata;
    new_l2_resp_opcode_o = new_l2_resp.opcode;
    {new_l2_resp_data3_o, new_l2_resp_data2_o, new_l2_resp_data1_o, new_l2_resp_data0_o} = new_l2_resp.data;
    new_l2_resp_size_o = new_l2_resp.size;
    new_l2_resp_qwen_o = new_l2_resp.qwen;
    new_dbg_sm_signals_o = new_dbg_sm_signals;

    orig_l2_resp_id_o = orig_l2_resp.id;
    orig_l2_resp_dest_o = orig_l2_resp.dest;
    orig_l2_resp_wdata_o = orig_l2_resp.wdata;
    orig_l2_resp_opcode_o = orig_l2_resp.opcode;
    {orig_l2_resp_data3_o, orig_l2_resp_data2_o, orig_l2_resp_data1_o, orig_l2_resp_data0_o} = orig_l2_resp.data;
    orig_l2_resp_size_o = orig_l2_resp.size;
    orig_l2_resp_qwen_o = orig_l2_resp.qwen;
    orig_dbg_sm_signals_o = orig_dbg_sm_signals;
  end

  neigh_fill_ff u_new (
    .clk_i,
    .rst_ni,
    .fill_fifo_resp_i(new_fill_fifo_resp),
    .fill_fifo_valid_i,
    .fill_fifo_ready_o(new_fill_fifo_ready_o),
    .tstore_min_resp_i(new_tstore_min_resp),
    .tstore_min_resp_valid_i(tstore_valid_i),
    .tstore_min_resp_ready_o(new_tstore_ready_o),
    .local_message_info_i(new_local_message_info),
    .local_message_valid_i(local_valid_i),
    .local_message_ready_o(new_local_ready_o),
    .tbox_min_rsp_info_i(new_tbox_min_rsp_info),
    .tbox_min_rsp_valid_i(tbox_valid_i),
    .tbox_min_rsp_ready_o(new_tbox_ready_o),
    .l2_resp_o(new_l2_resp),
    .l2_resp_valid_o(new_l2_resp_valid_o),
    .l2_resp_ready_i,
    .tstore_min_ack_o(new_tstore_min_ack_o),
    .dbg_sm_signals_o(new_dbg_sm_signals)
  );

  neigh_fill_ff_orig u_orig (
    .clock(clk_i),
    .reset(reset),
    .fill_fifo_resp(orig_fill_fifo_resp),
    .fill_fifo_valid(fill_fifo_valid_i),
    .fill_fifo_ready(orig_fill_fifo_ready_o),
    .tstore_min_resp(orig_tstore_min_resp),
    .tstore_min_resp_valid(tstore_valid_i),
    .tstore_min_resp_ready(orig_tstore_ready_o),
    .local_message_info(orig_local_message_info),
    .local_message_valid(local_valid_i),
    .local_message_ready(orig_local_ready_o),
    .tbox_min_rsp_info(orig_tbox_min_rsp_info),
    .tbox_min_rsp_valid(tbox_valid_i),
    .tbox_min_rsp_ready(orig_tbox_ready_o),
    .l2_resp(orig_l2_resp),
    .l2_resp_valid(orig_l2_resp_valid_o),
    .l2_resp_ready(l2_resp_ready_i),
    .tstore_min_ack(orig_tstore_min_ack_o),
    .dbg_sm_signals(orig_dbg_sm_signals)
  );

endmodule
