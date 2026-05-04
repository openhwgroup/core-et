// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench for shirecache_bist_mbx.
// Instantiates two DUTs: RamDelay=1 (no blocking) and RamDelay=3 (shift-register blocking).
// Uses small DataSize/AddrSize for easy C++ driving.

module shirecache_bist_mbx_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        mbx_impl_i,

  // Shared MBIST inputs
  input  logic        mbist_sel_i,
  input  logic        mbist_rd_en_i,
  input  logic        mbist_wr_en_i,
  input  logic [3:0]  mbist_addr_i,
  input  logic [15:0] mbist_wdata_i,

  // DUT1: RamDelay=1 (no blocking)
  output logic [15:0] d1_mbist_rdata_o,
  output logic        d1_mbx_sel_o,
  output logic        d1_mbx_rd_en_o,
  output logic        d1_mbx_wr_en_o,
  output logic [3:0]  d1_mbx_addr_o,
  output logic [15:0] d1_mbx_wdata_o,
  input  logic [15:0] d1_mbx_rdata_i,

  // DUT3: RamDelay=3 (2-stage shift-register blocking)
  output logic [15:0] d3_mbist_rdata_o,
  output logic        d3_mbx_sel_o,
  output logic        d3_mbx_rd_en_o,
  output logic        d3_mbx_wr_en_o,
  output logic [3:0]  d3_mbx_addr_o,
  output logic [15:0] d3_mbx_wdata_o,
  input  logic [15:0] d3_mbx_rdata_i
);

  shirecache_bist_mbx #(
    .DataSize(16), .AddrSize(4), .RamDelay(1)
  ) u_d1 (
    .clk_i, .rst_ni, .mbx_impl_i,
    .mbist_sel_i, .mbist_rd_en_i, .mbist_wr_en_i,
    .mbist_addr_i, .mbist_wdata_i,
    .mbist_rdata_o(d1_mbist_rdata_o),
    .mbx_sel_o    (d1_mbx_sel_o),
    .mbx_rd_en_o  (d1_mbx_rd_en_o),
    .mbx_wr_en_o  (d1_mbx_wr_en_o),
    .mbx_addr_o   (d1_mbx_addr_o),
    .mbx_wdata_o  (d1_mbx_wdata_o),
    .mbx_rdata_i  (d1_mbx_rdata_i)
  );

  shirecache_bist_mbx #(
    .DataSize(16), .AddrSize(4), .RamDelay(3)
  ) u_d3 (
    .clk_i, .rst_ni, .mbx_impl_i,
    .mbist_sel_i, .mbist_rd_en_i, .mbist_wr_en_i,
    .mbist_addr_i, .mbist_wdata_i,
    .mbist_rdata_o(d3_mbist_rdata_o),
    .mbx_sel_o    (d3_mbx_sel_o),
    .mbx_rd_en_o  (d3_mbx_rd_en_o),
    .mbx_wr_en_o  (d3_mbx_wr_en_o),
    .mbx_addr_o   (d3_mbx_addr_o),
    .mbx_wdata_o  (d3_mbx_wdata_o),
    .mbx_rdata_i  (d3_mbx_rdata_i)
  );

endmodule
