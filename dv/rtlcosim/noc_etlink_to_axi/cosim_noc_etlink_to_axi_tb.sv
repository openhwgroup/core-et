// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_noc_etlink_to_axi_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic uc_config_qos_dram_i,
  output logic [3:0] orig_ready_o,
  output logic [3:0] new_ready_o,
  input  logic wr_req_axi_valid_i,
  input  logic [$bits(shire_uncached_pkg::uc_noc_master_req_t)-1:0] wr_req_axi_info_bits_i,
  input  logic rd_req_axi_valid_i,
  input  logic [$bits(shire_uncached_pkg::uc_noc_master_req_nodata_t)-1:0] rd_req_axi_info_bits_i,
  input  logic to_sys_ar_ready_i,
  input  logic to_sys_aw_ready_i,
  input  logic to_sys_w_ready_i,
  input  logic to_l3_ar_ready_i,
  input  logic to_l3_aw_ready_i,
  input  logic to_l3_w_ready_i,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_sys_ar_bits_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_sys_ar_bits_o,
  output logic orig_to_sys_ar_valid_o,
  output logic new_to_sys_ar_valid_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_sys_aw_bits_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_sys_aw_bits_o,
  output logic orig_to_sys_aw_valid_o,
  output logic new_to_sys_aw_valid_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_sys_w_bits_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_to_sys_w_bits_o,
  output logic orig_to_sys_w_valid_o,
  output logic new_to_sys_w_valid_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_l3_ar_bits_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_l3_ar_bits_o,
  output logic orig_to_l3_ar_valid_o,
  output logic new_to_l3_ar_valid_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_l3_aw_bits_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_l3_aw_bits_o,
  output logic orig_to_l3_aw_valid_o,
  output logic new_to_l3_aw_valid_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_l3_w_bits_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_to_l3_w_bits_o,
  output logic orig_to_l3_w_valid_o,
  output logic new_to_l3_w_valid_o
);

  esr_uc_config_t orig_uc_config;
  shire_esr_pkg::esr_uc_config_t new_uc_config;
  uc_noc_master_req_t orig_wr_req_axi_info;
  shire_uncached_pkg::uc_noc_master_req_t new_wr_req_axi_info;
  uc_noc_master_req_nodata_t orig_rd_req_axi_info;
  shire_uncached_pkg::uc_noc_master_req_nodata_t new_rd_req_axi_info;

  sc_master_axi_AR_channel_t orig_to_sys_ar;
  sc_master_axi_AW_channel_t orig_to_sys_aw;
  sc_master_axi_W_channel_t  orig_to_sys_w;
  sc_master_axi_AR_channel_t orig_to_l3_ar;
  sc_master_axi_AW_channel_t orig_to_l3_aw;
  sc_master_axi_W_channel_t  orig_to_l3_w;

  axi_pkg::sc_master_ar_t new_to_sys_ar;
  axi_pkg::sc_master_aw_t new_to_sys_aw;
  axi_pkg::sc_master_w_t  new_to_sys_w;
  axi_pkg::sc_master_ar_t new_to_l3_ar;
  axi_pkg::sc_master_aw_t new_to_l3_aw;
  axi_pkg::sc_master_w_t  new_to_l3_w;

  assign orig_uc_config.qos_dram = uc_config_qos_dram_i;
  assign new_uc_config.qos_dram = uc_config_qos_dram_i;
  assign orig_wr_req_axi_info = wr_req_axi_info_bits_i[$bits(uc_noc_master_req_t)-1:0];
  assign new_wr_req_axi_info = wr_req_axi_info_bits_i;
  assign orig_rd_req_axi_info = rd_req_axi_info_bits_i[$bits(uc_noc_master_req_nodata_t)-1:0];
  assign new_rd_req_axi_info = rd_req_axi_info_bits_i;

  assign orig_to_sys_ar_bits_o = orig_to_sys_ar;
  assign orig_to_sys_aw_bits_o = orig_to_sys_aw;
  assign orig_to_sys_w_bits_o = orig_to_sys_w;
  assign orig_to_l3_ar_bits_o = orig_to_l3_ar;
  assign orig_to_l3_aw_bits_o = orig_to_l3_aw;
  assign orig_to_l3_w_bits_o = orig_to_l3_w;
  assign new_to_sys_ar_bits_o = new_to_sys_ar;
  assign new_to_sys_aw_bits_o = new_to_sys_aw;
  assign new_to_sys_w_bits_o = new_to_sys_w;
  assign new_to_l3_ar_bits_o = new_to_l3_ar;
  assign new_to_l3_aw_bits_o = new_to_l3_aw;
  assign new_to_l3_w_bits_o = new_to_l3_w;

  noc_etlink_to_axi_orig u_orig (
    .uc_config(orig_uc_config),
    .wr_req_axi_ready_tol3(orig_ready_o[0]),
    .wr_req_axi_ready_tosys(orig_ready_o[1]),
    .wr_req_axi_valid(wr_req_axi_valid_i),
    .wr_req_axi_info(orig_wr_req_axi_info),
    .rd_req_axi_ready_tol3(orig_ready_o[2]),
    .rd_req_axi_ready_tosys(orig_ready_o[3]),
    .rd_req_axi_valid(rd_req_axi_valid_i),
    .rd_req_axi_info(orig_rd_req_axi_info),
    .to_sys_AR(orig_to_sys_ar),
    .to_sys_ARVALID(orig_to_sys_ar_valid_o),
    .to_sys_ARREADY(to_sys_ar_ready_i),
    .to_sys_AW(orig_to_sys_aw),
    .to_sys_AWVALID(orig_to_sys_aw_valid_o),
    .to_sys_AWREADY(to_sys_aw_ready_i),
    .to_sys_W(orig_to_sys_w),
    .to_sys_WVALID(orig_to_sys_w_valid_o),
    .to_sys_WREADY(to_sys_w_ready_i),
    .to_l3_AR(orig_to_l3_ar),
    .to_l3_ARVALID(orig_to_l3_ar_valid_o),
    .to_l3_ARREADY(to_l3_ar_ready_i),
    .to_l3_AW(orig_to_l3_aw),
    .to_l3_AWVALID(orig_to_l3_aw_valid_o),
    .to_l3_AWREADY(to_l3_aw_ready_i),
    .to_l3_W(orig_to_l3_w),
    .to_l3_WVALID(orig_to_l3_w_valid_o),
    .to_l3_WREADY(to_l3_w_ready_i)
  );

  noc_etlink_to_axi u_new (
    .uc_config_i(new_uc_config),
    .wr_req_axi_ready_tol3_o(new_ready_o[0]),
    .wr_req_axi_ready_tosys_o(new_ready_o[1]),
    .wr_req_axi_valid_i(wr_req_axi_valid_i),
    .wr_req_axi_info_i(new_wr_req_axi_info),
    .rd_req_axi_ready_tol3_o(new_ready_o[2]),
    .rd_req_axi_ready_tosys_o(new_ready_o[3]),
    .rd_req_axi_valid_i(rd_req_axi_valid_i),
    .rd_req_axi_info_i(new_rd_req_axi_info),
    .to_sys_ar_o(new_to_sys_ar),
    .to_sys_ar_valid_o(new_to_sys_ar_valid_o),
    .to_sys_ar_ready_i(to_sys_ar_ready_i),
    .to_sys_aw_o(new_to_sys_aw),
    .to_sys_aw_valid_o(new_to_sys_aw_valid_o),
    .to_sys_aw_ready_i(to_sys_aw_ready_i),
    .to_sys_w_o(new_to_sys_w),
    .to_sys_w_valid_o(new_to_sys_w_valid_o),
    .to_sys_w_ready_i(to_sys_w_ready_i),
    .to_l3_ar_o(new_to_l3_ar),
    .to_l3_ar_valid_o(new_to_l3_ar_valid_o),
    .to_l3_ar_ready_i(to_l3_ar_ready_i),
    .to_l3_aw_o(new_to_l3_aw),
    .to_l3_aw_valid_o(new_to_l3_aw_valid_o),
    .to_l3_aw_ready_i(to_l3_aw_ready_i),
    .to_l3_w_o(new_to_l3_w),
    .to_l3_w_valid_o(new_to_l3_w_valid_o),
    .to_l3_w_ready_i(to_l3_w_ready_i)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_clock_reset;
  assign unused_clock_reset = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : cosim_noc_etlink_to_axi_tb
