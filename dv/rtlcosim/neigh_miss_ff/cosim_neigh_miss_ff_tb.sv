// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_miss_ff_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                 clk_i,
  input  logic                                 rst_ni,
  input  logic [4:0]                           l2_req_id_i,
  input  logic                                 l2_req_wdata_i,
  input  logic [4:0]                           l2_req_opcode_i,
  input  logic [PaSize-1:0]                    l2_req_address_i,
  input  logic [$bits(tensorcoop_control)-1:0] l2_req_data_i,
  input  logic [2:0]                           l2_req_size_i,
  input  logic [EtLinkQwenSize-1:0]            l2_req_qwen_i,
  input  logic [EtLinkSubopcodeSize-1:0]       l2_req_subopcode_i,
  input  logic [2:0]                           l2_dcache_req_valid_i,
  input  logic                                 miss_ready_i,
  input  logic [1:0]                           coop_miss_ready_i,

  output logic [2:0]                           new_l2_dcache_req_ready_o,
  output logic                                 new_miss_valid_o,
  output logic [4:0]                           new_miss_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]    new_miss_req_source_o,
  output logic                                 new_miss_req_wdata_o,
  output logic [4:0]                           new_miss_req_opcode_o,
  output logic [PaSize-1:0]                    new_miss_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0] new_miss_req_data_o,
  output logic [2:0]                           new_miss_req_size_o,
  output logic [EtLinkQwenSize-1:0]            new_miss_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]       new_miss_req_subopcode_o,
  output logic [1:0]                           new_coop_miss_valid_o,
  output logic [4:0]                           new_coop0_req_id_o,
  output logic [4:0]                           new_coop0_req_opcode_o,
  output logic [PaSize-1:0]                    new_coop0_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0] new_coop0_req_data_o,
  output logic [2:0]                           new_coop0_req_size_o,
  output logic [EtLinkQwenSize-1:0]            new_coop0_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]       new_coop0_req_subopcode_o,
  output logic [4:0]                           new_coop1_req_id_o,
  output logic [4:0]                           new_coop1_req_opcode_o,
  output logic [PaSize-1:0]                    new_coop1_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0] new_coop1_req_data_o,
  output logic [2:0]                           new_coop1_req_size_o,
  output logic [EtLinkQwenSize-1:0]            new_coop1_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]       new_coop1_req_subopcode_o,
  output logic [2:0]                           new_dbg_sm_signals_o,

  output logic [2:0]                           orig_l2_dcache_req_ready_o,
  output logic                                 orig_miss_valid_o,
  output logic [4:0]                           orig_miss_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]    orig_miss_req_source_o,
  output logic                                 orig_miss_req_wdata_o,
  output logic [4:0]                           orig_miss_req_opcode_o,
  output logic [PaSize-1:0]                    orig_miss_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0] orig_miss_req_data_o,
  output logic [2:0]                           orig_miss_req_size_o,
  output logic [EtLinkQwenSize-1:0]            orig_miss_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]       orig_miss_req_subopcode_o,
  output logic [1:0]                           orig_coop_miss_valid_o,
  output logic [4:0]                           orig_coop0_req_id_o,
  output logic [4:0]                           orig_coop0_req_opcode_o,
  output logic [PaSize-1:0]                    orig_coop0_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0] orig_coop0_req_data_o,
  output logic [2:0]                           orig_coop0_req_size_o,
  output logic [EtLinkQwenSize-1:0]            orig_coop0_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]       orig_coop0_req_subopcode_o,
  output logic [4:0]                           orig_coop1_req_id_o,
  output logic [4:0]                           orig_coop1_req_opcode_o,
  output logic [PaSize-1:0]                    orig_coop1_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0] orig_coop1_req_data_o,
  output logic [2:0]                           orig_coop1_req_size_o,
  output logic [EtLinkQwenSize-1:0]            orig_coop1_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]       orig_coop1_req_subopcode_o,
  output logic [2:0]                           orig_dbg_sm_signals_o
);

  logic reset;
  assign reset = ~rst_ni;

  et_link_minion_miss_req_info_t               orig_l2_req;
  minion_dcache_pkg::et_link_minion_miss_req_info_t new_l2_req;
  et_link_neigh_req_info_t                     orig_miss_req;
  etlink_pkg::neigh_req_t                      new_miss_req;
  et_link_minion_miss_req_info_t [1:0]               orig_coop_miss_req;
  minion_dcache_pkg::et_link_minion_miss_req_info_t [1:0] new_coop_miss_req;

  always_comb begin
    orig_l2_req = '0;
    orig_l2_req.id        = l2_req_id_i;
    orig_l2_req.wdata     = l2_req_wdata_i;
    orig_l2_req.opcode    = et_link_req_opcode_e'(l2_req_opcode_i);
    orig_l2_req.address   = l2_req_address_i;
    orig_l2_req.data      = l2_req_data_i;
    orig_l2_req.size      = et_link_size_e'(l2_req_size_i);
    orig_l2_req.qwen      = l2_req_qwen_i;
    orig_l2_req.subopcode = l2_req_subopcode_i;

    new_l2_req = '0;
    new_l2_req.id        = l2_req_id_i;
    new_l2_req.wdata     = l2_req_wdata_i;
    new_l2_req.opcode    = et_link_req_opcode_e'(l2_req_opcode_i);
    new_l2_req.address   = l2_req_address_i;
    new_l2_req.data      = l2_req_data_i;
    new_l2_req.size      = et_link_size_e'(l2_req_size_i);
    new_l2_req.qwen      = l2_req_qwen_i;
    new_l2_req.subopcode = l2_req_subopcode_i;

    new_miss_req_id_o        = new_miss_req.id[4:0];
    new_miss_req_source_o    = new_miss_req.source;
    new_miss_req_wdata_o     = new_miss_req.wdata;
    new_miss_req_opcode_o    = new_miss_req.opcode;
    new_miss_req_address_o   = new_miss_req.address;
    new_miss_req_data_o      = new_miss_req.data[$bits(tensorcoop_control)-1:0];
    new_miss_req_size_o      = new_miss_req.size;
    new_miss_req_qwen_o      = new_miss_req.qwen;
    new_miss_req_subopcode_o = new_miss_req.subopcode;

    orig_miss_req_id_o        = orig_miss_req.id[4:0];
    orig_miss_req_source_o    = orig_miss_req.source;
    orig_miss_req_wdata_o     = orig_miss_req.wdata;
    orig_miss_req_opcode_o    = orig_miss_req.opcode;
    orig_miss_req_address_o   = orig_miss_req.address;
    orig_miss_req_data_o      = orig_miss_req.data[$bits(tensorcoop_control)-1:0];
    orig_miss_req_size_o      = orig_miss_req.size;
    orig_miss_req_qwen_o      = orig_miss_req.qwen;
    orig_miss_req_subopcode_o = orig_miss_req.subopcode;

    new_coop0_req_id_o        = new_coop_miss_req[0].id;
    new_coop0_req_opcode_o    = new_coop_miss_req[0].opcode;
    new_coop0_req_address_o   = new_coop_miss_req[0].address;
    new_coop0_req_data_o      = new_coop_miss_req[0].data;
    new_coop0_req_size_o      = new_coop_miss_req[0].size;
    new_coop0_req_qwen_o      = new_coop_miss_req[0].qwen;
    new_coop0_req_subopcode_o = new_coop_miss_req[0].subopcode;

    new_coop1_req_id_o        = new_coop_miss_req[1].id;
    new_coop1_req_opcode_o    = new_coop_miss_req[1].opcode;
    new_coop1_req_address_o   = new_coop_miss_req[1].address;
    new_coop1_req_data_o      = new_coop_miss_req[1].data;
    new_coop1_req_size_o      = new_coop_miss_req[1].size;
    new_coop1_req_qwen_o      = new_coop_miss_req[1].qwen;
    new_coop1_req_subopcode_o = new_coop_miss_req[1].subopcode;

    orig_coop0_req_id_o        = orig_coop_miss_req[0].id;
    orig_coop0_req_opcode_o    = orig_coop_miss_req[0].opcode;
    orig_coop0_req_address_o   = orig_coop_miss_req[0].address;
    orig_coop0_req_data_o      = orig_coop_miss_req[0].data;
    orig_coop0_req_size_o      = orig_coop_miss_req[0].size;
    orig_coop0_req_qwen_o      = orig_coop_miss_req[0].qwen;
    orig_coop0_req_subopcode_o = orig_coop_miss_req[0].subopcode;

    orig_coop1_req_id_o        = orig_coop_miss_req[1].id;
    orig_coop1_req_opcode_o    = orig_coop_miss_req[1].opcode;
    orig_coop1_req_address_o   = orig_coop_miss_req[1].address;
    orig_coop1_req_data_o      = orig_coop_miss_req[1].data;
    orig_coop1_req_size_o      = orig_coop_miss_req[1].size;
    orig_coop1_req_qwen_o      = orig_coop_miss_req[1].qwen;
    orig_coop1_req_subopcode_o = orig_coop_miss_req[1].subopcode;
  end

  neigh_miss_ff u_new (
    .clk_i,
    .rst_ni,
    .l2_dcache_req_i(new_l2_req),
    .l2_dcache_req_valid_i,
    .l2_dcache_req_ready_o(new_l2_dcache_req_ready_o),
    .miss_req_o(new_miss_req),
    .miss_valid_o(new_miss_valid_o),
    .miss_ready_i,
    .coop_miss_req_o(new_coop_miss_req),
    .coop_miss_valid_o(new_coop_miss_valid_o),
    .coop_miss_ready_i,
    .dbg_sm_signals_o(new_dbg_sm_signals_o)
  );

  neigh_miss_ff_orig u_orig (
    .clock(clk_i),
    .reset(reset),
    .l2_dcache_req(orig_l2_req),
    .l2_dcache_req_valid(l2_dcache_req_valid_i),
    .l2_dcache_req_ready(orig_l2_dcache_req_ready_o),
    .miss_req(orig_miss_req),
    .miss_valid(orig_miss_valid_o),
    .miss_ready(miss_ready_i),
    .coop_miss_req(orig_coop_miss_req),
    .coop_miss_valid(orig_coop_miss_valid_o),
    .coop_miss_ready(coop_miss_ready_i),
    .dbg_sm_signals(orig_dbg_sm_signals_o)
  );

endmodule
