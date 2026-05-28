// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_bpam2minions_tb
  import neigh_ch_apb_mux_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [NeighDmApbAddrWidth-1:0] req_apb_paddr_i,
  input  logic                           req_apb_pwrite_i,
  input  logic                           req_apb_psel_i,
  input  logic                           req_apb_penable_i,
  input  logic [DataWidth-1:0]           req_apb_pwdata_i,

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

  output logic                         orig_rsp_apb_pready_o,
  output logic                         new_rsp_apb_pready_o,
  output logic [DataWidth-1:0]         orig_rsp_apb_prdata_o,
  output logic [DataWidth-1:0]         new_rsp_apb_prdata_o,
  output logic                         orig_rsp_apb_pslverr_o,
  output logic                         new_rsp_apb_pslverr_o,
  output logic [MinDmApbAddrWidth-1:0] orig_minion_apb_paddr_o,
  output logic [MinDmApbAddrWidth-1:0] new_minion_apb_paddr_o,
  output logic                         orig_minion_apb_pwrite_o,
  output logic                         new_minion_apb_pwrite_o,
  output logic [MinPerN-1:0]           orig_minion_apb_psel_o,
  output logic [MinPerN-1:0]           new_minion_apb_psel_o,
  output logic                         orig_minion_apb_penable_o,
  output logic                         new_minion_apb_penable_o,
  output logic [DataWidth-1:0]         orig_minion_apb_pwdata_o,
  output logic [DataWidth-1:0]         new_minion_apb_pwdata_o
);

  logic [MinPerN-1:0][DataWidth-1:0] minion_apb_prdata;

  always_comb begin
    minion_apb_prdata[0] = minion_apb_prdata0_i;
    minion_apb_prdata[1] = minion_apb_prdata1_i;
    minion_apb_prdata[2] = minion_apb_prdata2_i;
    minion_apb_prdata[3] = minion_apb_prdata3_i;
    minion_apb_prdata[4] = minion_apb_prdata4_i;
    minion_apb_prdata[5] = minion_apb_prdata5_i;
    minion_apb_prdata[6] = minion_apb_prdata6_i;
    minion_apb_prdata[7] = minion_apb_prdata7_i;
  end

  bpam2minions_orig #(
    .AD_IN_WIDTH  (NeighDmApbAddrWidth),
    .AD_OUT_WIDTH (MinDmApbAddrWidth),
    .DATA_WIDTH   (DataWidth),
    .NR_MINIONS   (MinPerN)
  ) u_orig (
    .clock              (clk_i),
    .reset              (!rst_ni),
    .req_apb_paddr      (req_apb_paddr_i),
    .req_apb_pwrite     (req_apb_pwrite_i),
    .req_apb_psel       (req_apb_psel_i),
    .req_apb_penable    (req_apb_penable_i),
    .req_apb_pwdata     (req_apb_pwdata_i),
    .rsp_apb_pready     (orig_rsp_apb_pready_o),
    .rsp_apb_prdata     (orig_rsp_apb_prdata_o),
    .rsp_apb_pslverr    (orig_rsp_apb_pslverr_o),
    .minion_apb_paddr   (orig_minion_apb_paddr_o),
    .minion_apb_pwrite  (orig_minion_apb_pwrite_o),
    .minion_apb_psel    (orig_minion_apb_psel_o),
    .minion_apb_penable (orig_minion_apb_penable_o),
    .minion_apb_pwdata  (orig_minion_apb_pwdata_o),
    .minion_apb_pready  (minion_apb_pready_i),
    .minion_apb_prdata  (minion_apb_prdata),
    .minion_apb_pslverr (minion_apb_pslverr_i)
  );

  bpam2minions #(
    .AdInWidth  (NeighDmApbAddrWidth),
    .AdOutWidth (MinDmApbAddrWidth),
    .DataWidth  (DataWidth),
    .NrMinions  (MinPerN)
  ) u_new (
    .clk_i,
    .rst_ni,
    .req_apb_paddr_i,
    .req_apb_pwrite_i,
    .req_apb_psel_i,
    .req_apb_penable_i,
    .req_apb_pwdata_i,
    .rsp_apb_pready_o  (new_rsp_apb_pready_o),
    .rsp_apb_prdata_o  (new_rsp_apb_prdata_o),
    .rsp_apb_pslverr_o (new_rsp_apb_pslverr_o),
    .minion_apb_paddr_o   (new_minion_apb_paddr_o),
    .minion_apb_pwrite_o  (new_minion_apb_pwrite_o),
    .minion_apb_psel_o    (new_minion_apb_psel_o),
    .minion_apb_penable_o (new_minion_apb_penable_o),
    .minion_apb_pwdata_o  (new_minion_apb_pwdata_o),
    .minion_apb_pready_i,
    .minion_apb_prdata_i  (minion_apb_prdata),
    .minion_apb_pslverr_i
  );

endmodule : cosim_bpam2minions_tb
