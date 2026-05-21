// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_ch_apb_mux_tb
  import neigh_ch_apb_mux_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [NeighDmApbAddrWidth-1:0] apb_req_paddr_i,
  input  logic                           apb_req_pwrite_i,
  input  logic                           apb_req_psel_i,
  input  logic                           apb_req_penable_i,
  input  logic [DataWidth-1:0]           apb_req_pwdata_i,

  output logic [DataWidth-1:0] apb_rsp_prdata_o,
  output logic                 apb_rsp_pready_o,
  output logic                 apb_rsp_pslverr_o,

  input  logic [MinPerN-1:0] minion_apb_pready_i,
  input  logic [MinPerN-1:0] minion_apb_pslverr_i,
  input  logic [63:0]        minion_apb_prdata0_i,
  input  logic [63:0]        minion_apb_prdata1_i,
  input  logic [63:0]        minion_apb_prdata2_i,
  input  logic [63:0]        minion_apb_prdata3_i,
  input  logic [63:0]        minion_apb_prdata4_i,
  input  logic [63:0]        minion_apb_prdata5_i,
  input  logic [63:0]        minion_apb_prdata6_i,
  input  logic [63:0]        minion_apb_prdata7_i,
  output logic               minion_apb_penable_o,
  output logic [MinPerN-1:0] minion_apb_psel_o,
  output logic               minion_apb_pwrite_o,
  output logic [MinDmApbAddrWidth-1:0] minion_apb_paddr_o,
  output logic [DataWidth-1:0]         minion_apb_pwdata_o,

  output logic [EsrNeighApbPpAddrWidth-1:0] esr_apb_paddr_o,
  output logic                              esr_apb_pwrite_o,
  output logic                              esr_apb_psel_o,
  output logic                              esr_apb_penable_o,
  output logic [DataWidth-1:0]              esr_apb_pwdata_o,

  output logic [IcacheNeighApbAddrWidth-1:0] icache_apb_paddr_o,
  output logic                               icache_apb_pwrite_o,
  output logic                               icache_apb_psel_o,
  output logic                               icache_apb_penable_o,
  output logic [DataWidth-1:0]               icache_apb_pwdata_o,

  output logic [TboxNeighApbAddrWidth-1:0] tbox_apb_paddr_o,
  output logic                             tbox_apb_pwrite_o,
  output logic                             tbox_apb_psel_o,
  output logic                             tbox_apb_penable_o,
  output logic [DataWidth-1:0]             tbox_apb_pwdata_o,

  input  logic [DataWidth-1:0] esr_rsp_prdata_i,
  input  logic                 esr_rsp_pready_i,
  input  logic                 esr_rsp_pslverr_i,
  input  logic [DataWidth-1:0] icache_rsp_prdata_i,
  input  logic                 icache_rsp_pready_i,
  input  logic                 icache_rsp_pslverr_i,
  input  logic [DataWidth-1:0] tbox_rsp_prdata_i,
  input  logic                 tbox_rsp_pready_i,
  input  logic                 tbox_rsp_pslverr_i
);

  apb_to_neigh_t req_from_shire;
  apb_from_neigh_t rsp_to_shire;
  apb_to_neigh_esrs_t req_esr;
  apb_to_neigh_icache_t req_icache;
  apb_to_neigh_tbox_t req_tbox;
  apb_from_neigh_channel_t rsp_esr;
  apb_from_neigh_channel_t rsp_icache;
  apb_from_neigh_channel_t rsp_tbox;

  logic [MinPerN-1:0][DataWidth-1:0] minion_apb_prdata;

  always_comb begin
    req_from_shire.apb_paddr   = apb_req_paddr_i;
    req_from_shire.apb_pwrite  = apb_req_pwrite_i;
    req_from_shire.apb_psel    = apb_req_psel_i;
    req_from_shire.apb_penable = apb_req_penable_i;
    req_from_shire.apb_pwdata  = apb_req_pwdata_i;

    minion_apb_prdata[0] = minion_apb_prdata0_i;
    minion_apb_prdata[1] = minion_apb_prdata1_i;
    minion_apb_prdata[2] = minion_apb_prdata2_i;
    minion_apb_prdata[3] = minion_apb_prdata3_i;
    minion_apb_prdata[4] = minion_apb_prdata4_i;
    minion_apb_prdata[5] = minion_apb_prdata5_i;
    minion_apb_prdata[6] = minion_apb_prdata6_i;
    minion_apb_prdata[7] = minion_apb_prdata7_i;

    rsp_esr.apb_prdata   = esr_rsp_prdata_i;
    rsp_esr.apb_pready   = esr_rsp_pready_i;
    rsp_esr.apb_pslverr  = esr_rsp_pslverr_i;
    rsp_icache.apb_prdata  = icache_rsp_prdata_i;
    rsp_icache.apb_pready  = icache_rsp_pready_i;
    rsp_icache.apb_pslverr = icache_rsp_pslverr_i;
    rsp_tbox.apb_prdata  = tbox_rsp_prdata_i;
    rsp_tbox.apb_pready  = tbox_rsp_pready_i;
    rsp_tbox.apb_pslverr = tbox_rsp_pslverr_i;
  end

  assign apb_rsp_prdata_o  = rsp_to_shire.apb_prdata;
  assign apb_rsp_pready_o  = rsp_to_shire.apb_pready;
  assign apb_rsp_pslverr_o = rsp_to_shire.apb_pslverr;

  assign esr_apb_paddr_o   = req_esr.apb_paddr;
  assign esr_apb_pwrite_o  = req_esr.apb_pwrite;
  assign esr_apb_psel_o    = req_esr.apb_psel;
  assign esr_apb_penable_o = req_esr.apb_penable;
  assign esr_apb_pwdata_o  = req_esr.apb_pwdata;

  assign icache_apb_paddr_o   = req_icache.apb_paddr;
  assign icache_apb_pwrite_o  = req_icache.apb_pwrite;
  assign icache_apb_psel_o    = req_icache.apb_psel;
  assign icache_apb_penable_o = req_icache.apb_penable;
  assign icache_apb_pwdata_o  = req_icache.apb_pwdata;

  assign tbox_apb_paddr_o   = req_tbox.apb_paddr;
  assign tbox_apb_pwrite_o  = req_tbox.apb_pwrite;
  assign tbox_apb_psel_o    = req_tbox.apb_psel;
  assign tbox_apb_penable_o = req_tbox.apb_penable;
  assign tbox_apb_pwdata_o  = req_tbox.apb_pwdata;

  neigh_ch_apb_mux u_dut (
    .clk_i,
    .rst_ni,
    .apb_req_from_shire_i    (req_from_shire),
    .apb_rsp_to_shire_o      (rsp_to_shire),
    .minion_apb_prdata_i     (minion_apb_prdata),
    .minion_apb_pready_i,
    .minion_apb_pslverr_i,
    .minion_apb_penable_o,
    .minion_apb_psel_o,
    .minion_apb_pwrite_o,
    .minion_apb_paddr_o,
    .minion_apb_pwdata_o,
    .apb_req_struct_esr_o    (req_esr),
    .apb_req_struct_icache_o (req_icache),
    .apb_req_struct_tbox_o   (req_tbox),
    .apb_rsp_struct_esr_i    (rsp_esr),
    .apb_rsp_struct_icache_i (rsp_icache),
    .apb_rsp_struct_tbox_i   (rsp_tbox)
  );

endmodule : neigh_ch_apb_mux_tb
