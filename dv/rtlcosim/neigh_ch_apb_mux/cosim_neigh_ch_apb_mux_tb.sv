// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_ch_apb_mux_tb
  import neigh_ch_apb_mux_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [NeighDmApbAddrWidth-1:0] apb_req_paddr_i,
  input  logic                           apb_req_pwrite_i,
  input  logic                           apb_req_psel_i,
  input  logic                           apb_req_penable_i,
  input  logic [DataWidth-1:0]           apb_req_pwdata_i,

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

  input  logic [DataWidth-1:0] esr_rsp_prdata_i,
  input  logic                 esr_rsp_pready_i,
  input  logic                 esr_rsp_pslverr_i,
  input  logic [DataWidth-1:0] icache_rsp_prdata_i,
  input  logic                 icache_rsp_pready_i,
  input  logic                 icache_rsp_pslverr_i,
  input  logic [DataWidth-1:0] tbox_rsp_prdata_i,
  input  logic                 tbox_rsp_pready_i,
  input  logic                 tbox_rsp_pslverr_i,

  output logic [DataWidth-1:0] orig_apb_rsp_prdata_o,
  output logic [DataWidth-1:0] new_apb_rsp_prdata_o,
  output logic                 orig_apb_rsp_pready_o,
  output logic                 new_apb_rsp_pready_o,
  output logic                 orig_apb_rsp_pslverr_o,
  output logic                 new_apb_rsp_pslverr_o,

  output logic                 orig_minion_apb_penable_o,
  output logic                 new_minion_apb_penable_o,
  output logic [MinPerN-1:0]   orig_minion_apb_psel_o,
  output logic [MinPerN-1:0]   new_minion_apb_psel_o,
  output logic                 orig_minion_apb_pwrite_o,
  output logic                 new_minion_apb_pwrite_o,
  output logic [MinDmApbAddrWidth-1:0] orig_minion_apb_paddr_o,
  output logic [MinDmApbAddrWidth-1:0] new_minion_apb_paddr_o,
  output logic [DataWidth-1:0]         orig_minion_apb_pwdata_o,
  output logic [DataWidth-1:0]         new_minion_apb_pwdata_o,

  output logic [EsrNeighApbPpAddrWidth-1:0] orig_esr_apb_paddr_o,
  output logic [EsrNeighApbPpAddrWidth-1:0] new_esr_apb_paddr_o,
  output logic                              orig_esr_apb_pwrite_o,
  output logic                              new_esr_apb_pwrite_o,
  output logic                              orig_esr_apb_psel_o,
  output logic                              new_esr_apb_psel_o,
  output logic                              orig_esr_apb_penable_o,
  output logic                              new_esr_apb_penable_o,
  output logic [DataWidth-1:0]              orig_esr_apb_pwdata_o,
  output logic [DataWidth-1:0]              new_esr_apb_pwdata_o,

  output logic [IcacheNeighApbAddrWidth-1:0] orig_icache_apb_paddr_o,
  output logic [IcacheNeighApbAddrWidth-1:0] new_icache_apb_paddr_o,
  output logic                               orig_icache_apb_pwrite_o,
  output logic                               new_icache_apb_pwrite_o,
  output logic                               orig_icache_apb_psel_o,
  output logic                               new_icache_apb_psel_o,
  output logic                               orig_icache_apb_penable_o,
  output logic                               new_icache_apb_penable_o,
  output logic [DataWidth-1:0]               orig_icache_apb_pwdata_o,
  output logic [DataWidth-1:0]               new_icache_apb_pwdata_o,

  output logic [TboxNeighApbAddrWidth-1:0] orig_tbox_apb_paddr_o,
  output logic [TboxNeighApbAddrWidth-1:0] new_tbox_apb_paddr_o,
  output logic                             orig_tbox_apb_pwrite_o,
  output logic                             new_tbox_apb_pwrite_o,
  output logic                             orig_tbox_apb_psel_o,
  output logic                             new_tbox_apb_psel_o,
  output logic                             orig_tbox_apb_penable_o,
  output logic                             new_tbox_apb_penable_o,
  output logic [DataWidth-1:0]             orig_tbox_apb_pwdata_o,
  output logic [DataWidth-1:0]             new_tbox_apb_pwdata_o
);

  APB_to_neigh_t              orig_req_from_shire;
  APB_from_neigh_t            orig_rsp_to_shire;
  APB_to_neigh_esrs_t         orig_req_esr;
  APB_to_neigh_icache_t       orig_req_icache;
  APB_to_neigh_tbox_t         orig_req_tbox;
  APB_from_neigh_channel_t    orig_rsp_esr;
  APB_from_neigh_channel_t    orig_rsp_icache;
  APB_from_neigh_channel_t    orig_rsp_tbox;

  apb_to_neigh_t              new_req_from_shire;
  apb_from_neigh_t            new_rsp_to_shire;
  apb_to_neigh_esrs_t         new_req_esr;
  apb_to_neigh_icache_t       new_req_icache;
  apb_to_neigh_tbox_t         new_req_tbox;
  apb_from_neigh_channel_t    new_rsp_esr;
  apb_from_neigh_channel_t    new_rsp_icache;
  apb_from_neigh_channel_t    new_rsp_tbox;

  logic [MinPerN-1:0][DataWidth-1:0] minion_apb_prdata;

  always_comb begin
    orig_req_from_shire.apb_paddr   = apb_req_paddr_i;
    orig_req_from_shire.apb_pwrite  = apb_req_pwrite_i;
    orig_req_from_shire.apb_psel    = apb_req_psel_i;
    orig_req_from_shire.apb_penable = apb_req_penable_i;
    orig_req_from_shire.apb_pwdata  = apb_req_pwdata_i;
    new_req_from_shire.apb_paddr    = apb_req_paddr_i;
    new_req_from_shire.apb_pwrite   = apb_req_pwrite_i;
    new_req_from_shire.apb_psel     = apb_req_psel_i;
    new_req_from_shire.apb_penable  = apb_req_penable_i;
    new_req_from_shire.apb_pwdata   = apb_req_pwdata_i;

    minion_apb_prdata[0] = minion_apb_prdata0_i;
    minion_apb_prdata[1] = minion_apb_prdata1_i;
    minion_apb_prdata[2] = minion_apb_prdata2_i;
    minion_apb_prdata[3] = minion_apb_prdata3_i;
    minion_apb_prdata[4] = minion_apb_prdata4_i;
    minion_apb_prdata[5] = minion_apb_prdata5_i;
    minion_apb_prdata[6] = minion_apb_prdata6_i;
    minion_apb_prdata[7] = minion_apb_prdata7_i;

    orig_rsp_esr.apb_prdata   = esr_rsp_prdata_i;
    orig_rsp_esr.apb_pready   = esr_rsp_pready_i;
    orig_rsp_esr.apb_pslverr  = esr_rsp_pslverr_i;
    orig_rsp_icache.apb_prdata  = icache_rsp_prdata_i;
    orig_rsp_icache.apb_pready  = icache_rsp_pready_i;
    orig_rsp_icache.apb_pslverr = icache_rsp_pslverr_i;
    orig_rsp_tbox.apb_prdata  = tbox_rsp_prdata_i;
    orig_rsp_tbox.apb_pready  = tbox_rsp_pready_i;
    orig_rsp_tbox.apb_pslverr = tbox_rsp_pslverr_i;

    new_rsp_esr.apb_prdata   = esr_rsp_prdata_i;
    new_rsp_esr.apb_pready   = esr_rsp_pready_i;
    new_rsp_esr.apb_pslverr  = esr_rsp_pslverr_i;
    new_rsp_icache.apb_prdata  = icache_rsp_prdata_i;
    new_rsp_icache.apb_pready  = icache_rsp_pready_i;
    new_rsp_icache.apb_pslverr = icache_rsp_pslverr_i;
    new_rsp_tbox.apb_prdata  = tbox_rsp_prdata_i;
    new_rsp_tbox.apb_pready  = tbox_rsp_pready_i;
    new_rsp_tbox.apb_pslverr = tbox_rsp_pslverr_i;
  end

  assign orig_apb_rsp_prdata_o  = orig_rsp_to_shire.apb_prdata;
  assign orig_apb_rsp_pready_o  = orig_rsp_to_shire.apb_pready;
  assign orig_apb_rsp_pslverr_o = orig_rsp_to_shire.apb_pslverr;
  assign new_apb_rsp_prdata_o   = new_rsp_to_shire.apb_prdata;
  assign new_apb_rsp_pready_o   = new_rsp_to_shire.apb_pready;
  assign new_apb_rsp_pslverr_o  = new_rsp_to_shire.apb_pslverr;

  assign orig_esr_apb_paddr_o   = orig_req_esr.apb_paddr;
  assign orig_esr_apb_pwrite_o  = orig_req_esr.apb_pwrite;
  assign orig_esr_apb_psel_o    = orig_req_esr.apb_psel;
  assign orig_esr_apb_penable_o = orig_req_esr.apb_penable;
  assign orig_esr_apb_pwdata_o  = orig_req_esr.apb_pwdata;
  assign new_esr_apb_paddr_o    = new_req_esr.apb_paddr;
  assign new_esr_apb_pwrite_o   = new_req_esr.apb_pwrite;
  assign new_esr_apb_psel_o     = new_req_esr.apb_psel;
  assign new_esr_apb_penable_o  = new_req_esr.apb_penable;
  assign new_esr_apb_pwdata_o   = new_req_esr.apb_pwdata;

  assign orig_icache_apb_paddr_o   = orig_req_icache.apb_paddr;
  assign orig_icache_apb_pwrite_o  = orig_req_icache.apb_pwrite;
  assign orig_icache_apb_psel_o    = orig_req_icache.apb_psel;
  assign orig_icache_apb_penable_o = orig_req_icache.apb_penable;
  assign orig_icache_apb_pwdata_o  = orig_req_icache.apb_pwdata;
  assign new_icache_apb_paddr_o    = new_req_icache.apb_paddr;
  assign new_icache_apb_pwrite_o   = new_req_icache.apb_pwrite;
  assign new_icache_apb_psel_o     = new_req_icache.apb_psel;
  assign new_icache_apb_penable_o  = new_req_icache.apb_penable;
  assign new_icache_apb_pwdata_o   = new_req_icache.apb_pwdata;

  assign orig_tbox_apb_paddr_o   = orig_req_tbox.apb_paddr;
  assign orig_tbox_apb_pwrite_o  = orig_req_tbox.apb_pwrite;
  assign orig_tbox_apb_psel_o    = orig_req_tbox.apb_psel;
  assign orig_tbox_apb_penable_o = orig_req_tbox.apb_penable;
  assign orig_tbox_apb_pwdata_o  = orig_req_tbox.apb_pwdata;
  assign new_tbox_apb_paddr_o    = new_req_tbox.apb_paddr;
  assign new_tbox_apb_pwrite_o   = new_req_tbox.apb_pwrite;
  assign new_tbox_apb_psel_o     = new_req_tbox.apb_psel;
  assign new_tbox_apb_penable_o  = new_req_tbox.apb_penable;
  assign new_tbox_apb_pwdata_o   = new_req_tbox.apb_pwdata;

  neigh_ch_apb_mux_orig u_orig (
    .clock                 (clk_i),
    .reset                 (!rst_ni),
    .APB_req_from_shire    (orig_req_from_shire),
    .APB_rsp_to_shire      (orig_rsp_to_shire),
    .minion_apb_prdata     (minion_apb_prdata),
    .minion_apb_pready     (minion_apb_pready_i),
    .minion_apb_pslverr    (minion_apb_pslverr_i),
    .minion_apb_penable    (orig_minion_apb_penable_o),
    .minion_apb_psel       (orig_minion_apb_psel_o),
    .minion_apb_pwrite     (orig_minion_apb_pwrite_o),
    .minion_apb_paddr      (orig_minion_apb_paddr_o),
    .minion_apb_pwdata     (orig_minion_apb_pwdata_o),
    .APB_req_struct_ESR    (orig_req_esr),
    .APB_req_struct_icache (orig_req_icache),
    .APB_req_struct_tbox   (orig_req_tbox),
    .APB_rsp_struct_ESR    (orig_rsp_esr),
    .APB_rsp_struct_icache (orig_rsp_icache),
    .APB_rsp_struct_tbox   (orig_rsp_tbox)
  );

  neigh_ch_apb_mux u_new (
    .clk_i,
    .rst_ni,
    .apb_req_from_shire_i    (new_req_from_shire),
    .apb_rsp_to_shire_o      (new_rsp_to_shire),
    .minion_apb_prdata_i     (minion_apb_prdata),
    .minion_apb_pready_i,
    .minion_apb_pslverr_i,
    .minion_apb_penable_o    (new_minion_apb_penable_o),
    .minion_apb_psel_o       (new_minion_apb_psel_o),
    .minion_apb_pwrite_o     (new_minion_apb_pwrite_o),
    .minion_apb_paddr_o      (new_minion_apb_paddr_o),
    .minion_apb_pwdata_o     (new_minion_apb_pwdata_o),
    .apb_req_struct_esr_o    (new_req_esr),
    .apb_req_struct_icache_o (new_req_icache),
    .apb_req_struct_tbox_o   (new_req_tbox),
    .apb_rsp_struct_esr_i    (new_rsp_esr),
    .apb_rsp_struct_icache_i (new_rsp_icache),
    .apb_rsp_struct_tbox_i   (new_rsp_tbox)
  );

endmodule : cosim_neigh_ch_apb_mux_tb
