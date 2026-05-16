// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module bpam2minions_tb (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] req_apb_paddr_i,
  input  logic                                                 req_apb_pwrite_i,
  input  logic                                                 req_apb_psel_i,
  input  logic                                                 req_apb_penable_i,
  input  logic [neigh_ch_apb_mux_pkg::DataWidth-1:0]           req_apb_pwdata_i,

  output logic                                       rsp_apb_pready_o,
  output logic [neigh_ch_apb_mux_pkg::DataWidth-1:0] rsp_apb_prdata_o,
  output logic                                       rsp_apb_pslverr_o,

  output logic [neigh_ch_apb_mux_pkg::MinDmApbAddrWidth-1:0] minion_apb_paddr_o,
  output logic                                               minion_apb_pwrite_o,
  output logic [neigh_ch_apb_mux_pkg::MinPerN-1:0]           minion_apb_psel_o,
  output logic                                               minion_apb_penable_o,
  output logic [neigh_ch_apb_mux_pkg::DataWidth-1:0]         minion_apb_pwdata_o,

  input  logic [neigh_ch_apb_mux_pkg::MinPerN-1:0] minion_apb_pready_i,
  input  logic [neigh_ch_apb_mux_pkg::MinPerN-1:0] minion_apb_pslverr_i,
  input  logic [63:0]                              minion_apb_prdata0_i,
  input  logic [63:0]                              minion_apb_prdata1_i,
  input  logic [63:0]                              minion_apb_prdata2_i,
  input  logic [63:0]                              minion_apb_prdata3_i,
  input  logic [63:0]                              minion_apb_prdata4_i,
  input  logic [63:0]                              minion_apb_prdata5_i,
  input  logic [63:0]                              minion_apb_prdata6_i,
  input  logic [63:0]                              minion_apb_prdata7_i
);

  logic [neigh_ch_apb_mux_pkg::MinPerN-1:0][neigh_ch_apb_mux_pkg::DataWidth-1:0]
      minion_apb_prdata;

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

  bpam2minions #(
    .AdInWidth  (neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth),
    .AdOutWidth (neigh_ch_apb_mux_pkg::MinDmApbAddrWidth),
    .DataWidth  (neigh_ch_apb_mux_pkg::DataWidth),
    .NrMinions  (neigh_ch_apb_mux_pkg::MinPerN)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .req_apb_paddr_i,
    .req_apb_pwrite_i,
    .req_apb_psel_i,
    .req_apb_penable_i,
    .req_apb_pwdata_i,
    .rsp_apb_pready_o,
    .rsp_apb_prdata_o,
    .rsp_apb_pslverr_o,
    .minion_apb_paddr_o,
    .minion_apb_pwrite_o,
    .minion_apb_psel_o,
    .minion_apb_penable_o,
    .minion_apb_pwdata_o,
    .minion_apb_pready_i,
    .minion_apb_prdata_i(minion_apb_prdata),
    .minion_apb_pslverr_i
  );

endmodule : bpam2minions_tb
