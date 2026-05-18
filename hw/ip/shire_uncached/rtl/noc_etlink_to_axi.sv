// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module noc_etlink_to_axi
  import axi_pkg::*;
  import etlink_pkg::*;
  import shire_esr_pkg::*;
  import shire_uncached_pkg::*;
(
  input  esr_uc_config_t              uc_config_i,

  output logic                        wr_req_axi_ready_tol3_o,
  output logic                        wr_req_axi_ready_tosys_o,
  input  logic                        wr_req_axi_valid_i,
  input  uc_noc_master_req_t          wr_req_axi_info_i,

  output logic                        rd_req_axi_ready_tol3_o,
  output logic                        rd_req_axi_ready_tosys_o,
  input  logic                        rd_req_axi_valid_i,
  input  uc_noc_master_req_nodata_t   rd_req_axi_info_i,

  output sc_master_ar_t               to_sys_ar_o,
  output logic                        to_sys_ar_valid_o,
  input  logic                        to_sys_ar_ready_i,
  output sc_master_aw_t               to_sys_aw_o,
  output logic                        to_sys_aw_valid_o,
  input  logic                        to_sys_aw_ready_i,
  output sc_master_w_t                to_sys_w_o,
  output logic                        to_sys_w_valid_o,
  input  logic                        to_sys_w_ready_i,

  output sc_master_ar_t               to_l3_ar_o,
  output logic                        to_l3_ar_valid_o,
  input  logic                        to_l3_ar_ready_i,
  output sc_master_aw_t               to_l3_aw_o,
  output logic                        to_l3_aw_valid_o,
  input  logic                        to_l3_aw_ready_i,
  output sc_master_w_t                to_l3_w_o,
  output logic                        to_l3_w_valid_o,
  input  logic                        to_l3_w_ready_i
);

  assign wr_req_axi_ready_tol3_o  = to_l3_aw_ready_i && to_l3_w_ready_i;
  assign wr_req_axi_ready_tosys_o = to_sys_aw_ready_i && to_sys_w_ready_i;
  assign rd_req_axi_ready_tol3_o  = to_l3_ar_ready_i;
  assign rd_req_axi_ready_tosys_o = to_sys_ar_ready_i;

  logic rd_sys_is_ddr;
  logic rd_l3_is_ddr;
  logic wr_sys_is_mem;
  logic wr_l3_is_mem;

  assign rd_sys_is_ddr = (rd_req_axi_info_i.opcode == etlink_pkg::ReqRead) &&
                         (rd_req_axi_info_i.address[etlink_pkg::PaSize-1:AdDdrRegionStart] == AdDdrRegion);
  assign rd_l3_is_ddr  = rd_sys_is_ddr;
  assign wr_sys_is_mem = ((wr_req_axi_info_i.opcode == etlink_pkg::ReqWrite) &&
                          (wr_req_axi_info_i.address[etlink_pkg::PaSize-1:AdDdrRegionStart] == AdDdrRegion)) ||
                         ((wr_req_axi_info_i.opcode == etlink_pkg::ReqAtomic) &&
                          ((wr_req_axi_info_i.address[etlink_pkg::PaSize-1:AdDdrRegionStart] == AdDdrRegion) ||
                           uc_is_scp(wr_req_axi_info_i.address)));
  assign wr_l3_is_mem = wr_sys_is_mem;

  always_comb begin
    to_sys_ar_o       = '0;
    to_sys_ar_valid_o = rd_req_axi_valid_i && !rd_req_axi_info_i.l3_en;
    to_sys_ar_o.len   = '0;
    to_sys_ar_o.size  = rd_req_axi_info_i.size;
    to_sys_ar_o.burst = '0;
    to_sys_ar_o.lock  = '0;
    to_sys_ar_o.cache = axi_pkg::ScMasterAxCache;
    to_sys_ar_o.prot  = 3'b010;
    to_sys_ar_o.qos   = rd_sys_is_ddr ? {{axi_pkg::AxQosSize-1{1'b0}}, uc_config_i.qos_dram} :
                         (rd_req_axi_info_i.opcode == etlink_pkg::ReqRead) ? EtAxiQosEsr :
                         {axi_pkg::AxQosSize{1'b1}};
    to_sys_ar_o.user  = '0;
    to_sys_ar_o.id    = rd_req_axi_info_i.axi_id;
    to_sys_ar_o.addr  = rd_req_axi_info_i.address;

    to_sys_aw_o       = '0;
    to_sys_aw_valid_o = wr_req_axi_valid_i && !wr_req_axi_info_i.l3_en;
    to_sys_aw_o.len   = '0;
    to_sys_aw_o.size  = uc_is_atomic(wr_req_axi_info_i.opcode) ? etlink_pkg::SizeLine : wr_req_axi_info_i.size;
    to_sys_aw_o.burst = '0;
    to_sys_aw_o.lock  = '0;
    to_sys_aw_o.cache = axi_pkg::ScMasterAxCache;
    to_sys_aw_o.prot  = 3'b010;
    to_sys_aw_o.qos   = wr_sys_is_mem ? {{axi_pkg::AxQosSize-1{1'b0}}, uc_config_i.qos_dram} :
                         (wr_req_axi_info_i.opcode == etlink_pkg::ReqWrite) ? EtAxiQosEsr :
                         (wr_req_axi_info_i.opcode == etlink_pkg::ReqMsgSendData) ? EtAxiQosMessage :
                         {axi_pkg::AxQosSize{1'b1}};
    to_sys_aw_o.user  = wr_req_axi_info_i.opcode;
    to_sys_aw_o.id    = wr_req_axi_info_i.axi_id;
    to_sys_aw_o.addr  = wr_req_axi_info_i.address;

    to_sys_w_o       = '0;
    to_sys_w_valid_o = wr_req_axi_valid_i && !wr_req_axi_info_i.l3_en;
    to_sys_w_o.strb  = uc_axi_strb(wr_req_axi_info_i.opcode, wr_req_axi_info_i.size,
                                    wr_req_axi_info_i.address[UcAlignBitsEnd:UcAlignBitsStart],
                                    wr_req_axi_info_i.data[259:256]);
    to_sys_w_o.data  = uc_aligned_data(wr_req_axi_info_i);
    to_sys_w_o.last  = 1'b1;

    to_l3_ar_o       = '0;
    to_l3_ar_valid_o = rd_req_axi_valid_i && rd_req_axi_info_i.l3_en;
    to_l3_ar_o.len   = '0;
    to_l3_ar_o.size  = rd_req_axi_info_i.size;
    to_l3_ar_o.burst = '0;
    to_l3_ar_o.lock  = '0;
    to_l3_ar_o.cache = axi_pkg::ScMasterAxCache;
    to_l3_ar_o.prot  = 3'b010;
    to_l3_ar_o.qos   = rd_l3_is_ddr ? {{axi_pkg::AxQosSize-1{1'b0}}, uc_config_i.qos_dram} :
                         (rd_req_axi_info_i.opcode == etlink_pkg::ReqRead) ? EtAxiQosEsr :
                         {axi_pkg::AxQosSize{1'b1}};
    to_l3_ar_o.user  = '0;
    to_l3_ar_o.id    = rd_req_axi_info_i.axi_id;
    to_l3_ar_o.addr  = rd_req_axi_info_i.address;

    to_l3_aw_o       = '0;
    to_l3_aw_valid_o = wr_req_axi_valid_i && wr_req_axi_info_i.l3_en;
    to_l3_aw_o.len   = '0;
    to_l3_aw_o.size  = uc_is_atomic(wr_req_axi_info_i.opcode) ? etlink_pkg::SizeLine : wr_req_axi_info_i.size;
    to_l3_aw_o.burst = '0;
    to_l3_aw_o.lock  = '0;
    to_l3_aw_o.cache = axi_pkg::ScMasterAxCache;
    to_l3_aw_o.prot  = 3'b010;
    to_l3_aw_o.qos   = wr_l3_is_mem ? {{axi_pkg::AxQosSize-1{1'b0}}, uc_config_i.qos_dram} :
                         (wr_req_axi_info_i.opcode == etlink_pkg::ReqWrite) ? EtAxiQosEsr :
                         {axi_pkg::AxQosSize{1'b1}};
    to_l3_aw_o.user  = wr_req_axi_info_i.opcode;
    to_l3_aw_o.id    = wr_req_axi_info_i.axi_id;
    to_l3_aw_o.addr  = wr_req_axi_info_i.address;

    to_l3_w_o       = '0;
    to_l3_w_valid_o = wr_req_axi_valid_i && wr_req_axi_info_i.l3_en;
    to_l3_w_o.strb  = uc_axi_strb(wr_req_axi_info_i.opcode, wr_req_axi_info_i.size,
                                   wr_req_axi_info_i.address[UcAlignBitsEnd:UcAlignBitsStart],
                                   wr_req_axi_info_i.data[259:256]);
    to_l3_w_o.data  = uc_aligned_data(wr_req_axi_info_i);
    to_l3_w_o.last  = 1'b1;
  end

endmodule : noc_etlink_to_axi
