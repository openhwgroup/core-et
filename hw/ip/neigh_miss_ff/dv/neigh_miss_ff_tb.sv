// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_miss_ff_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                    clk_i,
  input  logic                                    rst_ni,
  input  logic [4:0]                              l2_req_id_i,
  input  logic                                    l2_req_wdata_i,
  input  logic [4:0]                              l2_req_opcode_i,
  input  logic [PaSize-1:0]                       l2_req_address_i,
  input  logic [$bits(tensorcoop_control)-1:0]    l2_req_data_i,
  input  logic [2:0]                              l2_req_size_i,
  input  logic [EtLinkQwenSize-1:0]               l2_req_qwen_i,
  input  logic [EtLinkSubopcodeSize-1:0]          l2_req_subopcode_i,
  input  logic [DcacheL2MissReqPorts-1:0]         l2_dcache_req_valid_i,
  output logic [DcacheL2MissReqPorts-1:0]         l2_dcache_req_ready_o,
  output logic                                    miss_valid_o,
  input  logic                                    miss_ready_i,
  output logic [4:0]                              miss_req_id_o,
  output logic [etlink_pkg::SourceSize-1:0]       miss_req_source_o,
  output logic                                    miss_req_wdata_o,
  output logic [4:0]                              miss_req_opcode_o,
  output logic [PaSize-1:0]                       miss_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0]    miss_req_data_o,
  output logic [2:0]                              miss_req_size_o,
  output logic [EtLinkQwenSize-1:0]               miss_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]          miss_req_subopcode_o,
  output logic [1:0]                              coop_miss_valid_o,
  input  logic [1:0]                              coop_miss_ready_i,
  output logic [4:0]                              coop0_req_id_o,
  output logic [4:0]                              coop0_req_opcode_o,
  output logic [PaSize-1:0]                       coop0_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0]    coop0_req_data_o,
  output logic [2:0]                              coop0_req_size_o,
  output logic [EtLinkQwenSize-1:0]               coop0_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]          coop0_req_subopcode_o,
  output logic [4:0]                              coop1_req_id_o,
  output logic [4:0]                              coop1_req_opcode_o,
  output logic [PaSize-1:0]                       coop1_req_address_o,
  output logic [$bits(tensorcoop_control)-1:0]    coop1_req_data_o,
  output logic [2:0]                              coop1_req_size_o,
  output logic [EtLinkQwenSize-1:0]               coop1_req_qwen_o,
  output logic [EtLinkSubopcodeSize-1:0]          coop1_req_subopcode_o,
  output logic [DcacheL2MissReqPorts-1:0]         dbg_sm_signals_o
);

  et_link_minion_miss_req_info_t l2_req;
  etlink_pkg::neigh_req_t        miss_req;
  et_link_minion_miss_req_info_t [1:0] coop_miss_req;

  always_comb begin
    l2_req = '0;
    l2_req.id        = l2_req_id_i;
    l2_req.wdata     = l2_req_wdata_i;
    l2_req.opcode    = et_link_req_opcode_e'(l2_req_opcode_i);
    l2_req.address   = l2_req_address_i;
    l2_req.data      = l2_req_data_i;
    l2_req.size      = et_link_size_e'(l2_req_size_i);
    l2_req.qwen      = l2_req_qwen_i;
    l2_req.subopcode = l2_req_subopcode_i;

    miss_req_id_o        = miss_req.id[4:0];
    miss_req_source_o    = miss_req.source;
    miss_req_wdata_o     = miss_req.wdata;
    miss_req_opcode_o    = miss_req.opcode;
    miss_req_address_o   = miss_req.address;
    miss_req_data_o      = miss_req.data[$bits(tensorcoop_control)-1:0];
    miss_req_size_o      = miss_req.size;
    miss_req_qwen_o      = miss_req.qwen;
    miss_req_subopcode_o = miss_req.subopcode;

    coop0_req_id_o        = coop_miss_req[0].id;
    coop0_req_opcode_o    = coop_miss_req[0].opcode;
    coop0_req_address_o   = coop_miss_req[0].address;
    coop0_req_data_o      = coop_miss_req[0].data;
    coop0_req_size_o      = coop_miss_req[0].size;
    coop0_req_qwen_o      = coop_miss_req[0].qwen;
    coop0_req_subopcode_o = coop_miss_req[0].subopcode;

    coop1_req_id_o        = coop_miss_req[1].id;
    coop1_req_opcode_o    = coop_miss_req[1].opcode;
    coop1_req_address_o   = coop_miss_req[1].address;
    coop1_req_data_o      = coop_miss_req[1].data;
    coop1_req_size_o      = coop_miss_req[1].size;
    coop1_req_qwen_o      = coop_miss_req[1].qwen;
    coop1_req_subopcode_o = coop_miss_req[1].subopcode;
  end

  neigh_miss_ff u_dut (
    .clk_i,
    .rst_ni,
    .l2_dcache_req_i(l2_req),
    .l2_dcache_req_valid_i,
    .l2_dcache_req_ready_o,
    .miss_req_o(miss_req),
    .miss_valid_o,
    .miss_ready_i,
    .coop_miss_req_o(coop_miss_req),
    .coop_miss_valid_o,
    .coop_miss_ready_i,
    .dbg_sm_signals_o
  );

endmodule
