// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_fill_fifo_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import neigh_pkg::*;
(
  input  logic                                      clk_i,
  input  logic                                      rst_ni,
  input  logic                                      push_i,
  input  logic [IdSize-1:0]                         push_id_i,
  input  logic [SourceSize-1:0]                     push_dest_i,
  input  logic                                      push_wdata_i,
  input  logic [1:0]                                push_opcode_i,
  input  logic [63:0]                               push_data0_i,
  input  logic [63:0]                               push_data1_i,
  input  logic [63:0]                               push_data2_i,
  input  logic [63:0]                               push_data3_i,
  input  logic [63:0]                               push_data4_i,
  input  logic [63:0]                               push_data5_i,
  input  logic [63:0]                               push_data6_i,
  input  logic [63:0]                               push_data7_i,
  input  logic [2:0]                                push_size_i,
  input  logic [3:0]                                push_qwen_i,
  input  logic [ShireCoopMinMaskSize-1:0]           coop_mask_i,
  input  logic [ShireCoopMinMaskSize*ShireCoopMissIdSize-1:0] coop_min_miss_id_i,
  input  logic [NeighNumRespAgents-1:0]             out_rsp_ready_i,
  output logic                                      new_full_o,
  output logic                                      orig_full_o,
  output logic [NeighNumRespAgents-1:0]             new_out_rsp_valid_o,
  output logic [NeighNumRespAgents-1:0]             orig_out_rsp_valid_o,
  output logic [IdSize-1:0]                         new_out_rsp_id_o [NeighNumRespAgents-1:0],
  output logic [IdSize-1:0]                         orig_out_rsp_id_o [NeighNumRespAgents-1:0],
  output logic [SourceSize-1:0]                     new_out_rsp_dest_o [NeighNumRespAgents-1:0],
  output logic [SourceSize-1:0]                     orig_out_rsp_dest_o [NeighNumRespAgents-1:0],
  output logic [NeighNumRespAgents-1:0]             new_out_rsp_wdata_o,
  output logic [NeighNumRespAgents-1:0]             orig_out_rsp_wdata_o,
  output logic [1:0]                                new_out_rsp_opcode_o [NeighNumRespAgents-1:0],
  output logic [1:0]                                orig_out_rsp_opcode_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               new_out_rsp_data0_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               orig_out_rsp_data0_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               new_out_rsp_data7_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               orig_out_rsp_data7_o [NeighNumRespAgents-1:0],
  output logic [2:0]                                new_out_rsp_size_o [NeighNumRespAgents-1:0],
  output logic [2:0]                                orig_out_rsp_size_o [NeighNumRespAgents-1:0],
  output logic [3:0]                                new_out_rsp_qwen_o [NeighNumRespAgents-1:0],
  output logic [3:0]                                orig_out_rsp_qwen_o [NeighNumRespAgents-1:0]
);

  logic reset;
  assign reset = ~rst_ni;

  rsp_t new_push_data;
  rsp_t [NeighNumRespAgents-1:0] new_out_rsp_info;
  coop_tload_tag_table_data_t new_coop_data;

  et_link_rsp_info_t orig_push_data;
  et_link_rsp_info_t [`NEIGH_NUM_RESP_AGENTS-1:0] orig_out_rsp_info;
  coop_tload_tag_table_data_t orig_coop_data;

  always_comb begin
    new_push_data = '0;
    new_push_data.id = push_id_i;
    new_push_data.dest = push_dest_i;
    new_push_data.wdata = push_wdata_i;
    new_push_data.opcode = rsp_opcode_e'(push_opcode_i);
    new_push_data.data = {push_data7_i, push_data6_i, push_data5_i, push_data4_i,
                          push_data3_i, push_data2_i, push_data1_i, push_data0_i};
    new_push_data.size = size_e'(push_size_i);
    new_push_data.qwen = push_qwen_i;

    new_coop_data = '0;
    new_coop_data.coop_min_mask = coop_mask_i;
    new_coop_data.min_miss_id = coop_min_miss_id_i;

    orig_push_data = '0;
    orig_push_data.id = push_id_i;
    orig_push_data.dest = push_dest_i;
    orig_push_data.wdata = push_wdata_i;
    orig_push_data.opcode = et_link_rsp_opcode_e'(push_opcode_i);
    orig_push_data.data = {push_data7_i, push_data6_i, push_data5_i, push_data4_i,
                           push_data3_i, push_data2_i, push_data1_i, push_data0_i};
    orig_push_data.size = et_link_size_e'(push_size_i);
    orig_push_data.qwen = push_qwen_i;

    orig_coop_data = '0;
    orig_coop_data.coop_min_mask = coop_mask_i;
    orig_coop_data.min_miss_id = coop_min_miss_id_i;

    for (int unsigned i = 0; i < NeighNumRespAgents; i++) begin
      new_out_rsp_id_o[i] = new_out_rsp_info[i].id;
      new_out_rsp_dest_o[i] = new_out_rsp_info[i].dest;
      new_out_rsp_wdata_o[i] = new_out_rsp_info[i].wdata;
      new_out_rsp_opcode_o[i] = new_out_rsp_info[i].opcode;
      new_out_rsp_data0_o[i] = new_out_rsp_info[i].data[63:0];
      new_out_rsp_data7_o[i] = new_out_rsp_info[i].data[511:448];
      new_out_rsp_size_o[i] = new_out_rsp_info[i].size;
      new_out_rsp_qwen_o[i] = new_out_rsp_info[i].qwen;

      orig_out_rsp_id_o[i] = orig_out_rsp_info[i].id;
      orig_out_rsp_dest_o[i] = orig_out_rsp_info[i].dest;
      orig_out_rsp_wdata_o[i] = orig_out_rsp_info[i].wdata;
      orig_out_rsp_opcode_o[i] = orig_out_rsp_info[i].opcode;
      orig_out_rsp_data0_o[i] = orig_out_rsp_info[i].data[63:0];
      orig_out_rsp_data7_o[i] = orig_out_rsp_info[i].data[511:448];
      orig_out_rsp_size_o[i] = orig_out_rsp_info[i].size;
      orig_out_rsp_qwen_o[i] = orig_out_rsp_info[i].qwen;
    end
  end

  neigh_fill_fifo u_new (
    .clk_i,
    .rst_ni,
    .push_i,
    .push_data_i (new_push_data),
    .coop_data_i (new_coop_data),
    .full_o      (new_full_o),
    .out_rsp_ready_i,
    .out_rsp_valid_o (new_out_rsp_valid_o),
    .out_rsp_info_o  (new_out_rsp_info)
  );

  neigh_fill_fifo_orig u_orig (
    .clock         (clk_i),
    .reset         (reset),
    .push          (push_i),
    .push_data     (orig_push_data),
    .coop_data     (orig_coop_data),
    .full          (orig_full_o),
    .out_rsp_ready (out_rsp_ready_i),
    .out_rsp_valid (orig_out_rsp_valid_o),
    .out_rsp_info  (orig_out_rsp_info)
  );

endmodule
