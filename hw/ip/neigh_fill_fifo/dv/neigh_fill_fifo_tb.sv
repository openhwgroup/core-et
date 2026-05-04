// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_fill_fifo_tb
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
  output logic                                      full_o,
  output logic [NeighNumRespAgents-1:0]             out_rsp_valid_o,
  output logic [IdSize-1:0]                         out_rsp_id_o [NeighNumRespAgents-1:0],
  output logic [SourceSize-1:0]                     out_rsp_dest_o [NeighNumRespAgents-1:0],
  output logic [NeighNumRespAgents-1:0]             out_rsp_wdata_o,
  output logic [1:0]                                out_rsp_opcode_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data0_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data1_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data2_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data3_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data4_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data5_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data6_o [NeighNumRespAgents-1:0],
  output logic [63:0]                               out_rsp_data7_o [NeighNumRespAgents-1:0],
  output logic [2:0]                                out_rsp_size_o [NeighNumRespAgents-1:0],
  output logic [3:0]                                out_rsp_qwen_o [NeighNumRespAgents-1:0]
);

  rsp_t push_data;
  rsp_t [NeighNumRespAgents-1:0] out_rsp_info;
  coop_tload_tag_table_data_t coop_data;

  always_comb begin
    push_data = '0;
    push_data.id = push_id_i;
    push_data.dest = push_dest_i;
    push_data.wdata = push_wdata_i;
    push_data.opcode = rsp_opcode_e'(push_opcode_i);
    push_data.data = {push_data7_i, push_data6_i, push_data5_i, push_data4_i,
                      push_data3_i, push_data2_i, push_data1_i, push_data0_i};
    push_data.size = size_e'(push_size_i);
    push_data.qwen = push_qwen_i;

    coop_data = '0;
    coop_data.coop_min_mask = coop_mask_i;
    coop_data.min_miss_id = coop_min_miss_id_i;

    for (int unsigned i = 0; i < NeighNumRespAgents; i++) begin
      out_rsp_id_o[i] = out_rsp_info[i].id;
      out_rsp_dest_o[i] = out_rsp_info[i].dest;
      out_rsp_wdata_o[i] = out_rsp_info[i].wdata;
      out_rsp_opcode_o[i] = out_rsp_info[i].opcode;
      {out_rsp_data7_o[i], out_rsp_data6_o[i], out_rsp_data5_o[i], out_rsp_data4_o[i],
       out_rsp_data3_o[i], out_rsp_data2_o[i], out_rsp_data1_o[i], out_rsp_data0_o[i]} =
          out_rsp_info[i].data;
      out_rsp_size_o[i] = out_rsp_info[i].size;
      out_rsp_qwen_o[i] = out_rsp_info[i].qwen;
    end
  end

  neigh_fill_fifo u_dut (
    .clk_i,
    .rst_ni,
    .push_i,
    .push_data_i (push_data),
    .coop_data_i (coop_data),
    .full_o,
    .out_rsp_ready_i,
    .out_rsp_valid_o,
    .out_rsp_info_o (out_rsp_info)
  );

endmodule
