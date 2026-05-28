// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncacheable_noc_tol3_handler_tb #(
  parameter int unsigned ReqqSize = shire_uncached_pkg::UcReqqSize,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(shire_uncached_pkg::uc_reqq_entry_t)-1:0] reqq_entry_bits_i,
  input  logic wr_reqq_entry_valid_i,
  input  logic rd_reqq_entry_valid_i,
  input  logic l3_enabled_i,
  input  logic remote_scp_enabled_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_l3_b_bits_i,
  input  logic to_l3_b_valid_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_l3_r_bits_i,
  input  logic to_l3_r_valid_i,
  output logic [ReqqIndexWidth-1:0] orig_wr_reqq_index_o,
  output logic [ReqqIndexWidth-1:0] new_wr_reqq_index_o,
  output logic [ReqqIndexWidth-1:0] orig_rd_reqq_index_o,
  output logic [ReqqIndexWidth-1:0] new_rd_reqq_index_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] orig_wr_reqq_entry_clear_bits_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] new_wr_reqq_entry_clear_bits_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] orig_rd_reqq_entry_clear_bits_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] new_rd_reqq_entry_clear_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] orig_wr_rsp_info_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] new_wr_rsp_info_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] orig_rd_rsp_info_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] new_rd_rsp_info_bits_o,
  output logic orig_wr_new_rsp_o,
  output logic new_wr_new_rsp_o,
  output logic orig_rd_new_rsp_o,
  output logic new_rd_new_rsp_o
);

  uc_reqq_entry_t orig_reqq_entry;
  shire_uncached_pkg::uc_reqq_entry_t new_reqq_entry;
  uc_reqq_valid_t orig_wr_reqq_entry_clear;
  shire_uncached_pkg::uc_reqq_valid_t new_wr_reqq_entry_clear;
  uc_reqq_valid_t orig_rd_reqq_entry_clear;
  shire_uncached_pkg::uc_reqq_valid_t new_rd_reqq_entry_clear;
  sc_xbar_rsp_t orig_wr_rsp_info;
  shirecache_pkg::xbar_rsp_t new_wr_rsp_info;
  sc_xbar_rsp_t orig_rd_rsp_info;
  shirecache_pkg::xbar_rsp_t new_rd_rsp_info;
  sc_master_axi_B_channel_t orig_to_l3_b;
  axi_pkg::sc_master_b_t new_to_l3_b;
  sc_master_axi_R_channel_t orig_to_l3_r;
  axi_pkg::sc_master_r_t new_to_l3_r;

  assign orig_reqq_entry = reqq_entry_bits_i[$bits(uc_reqq_entry_t)-1:0];
  assign new_reqq_entry = reqq_entry_bits_i;
  assign orig_to_l3_b = to_l3_b_bits_i[$bits(sc_master_axi_B_channel_t)-1:0];
  assign new_to_l3_b = to_l3_b_bits_i;
  assign orig_to_l3_r = to_l3_r_bits_i[$bits(sc_master_axi_R_channel_t)-1:0];
  assign new_to_l3_r = to_l3_r_bits_i;
  assign orig_wr_reqq_entry_clear_bits_o = orig_wr_reqq_entry_clear;
  assign new_wr_reqq_entry_clear_bits_o = new_wr_reqq_entry_clear;
  assign orig_rd_reqq_entry_clear_bits_o = orig_rd_reqq_entry_clear;
  assign new_rd_reqq_entry_clear_bits_o = new_rd_reqq_entry_clear;
  assign orig_wr_rsp_info_bits_o = orig_wr_rsp_info;
  assign new_wr_rsp_info_bits_o = new_wr_rsp_info;
  assign orig_rd_rsp_info_bits_o = orig_rd_rsp_info;
  assign new_rd_rsp_info_bits_o = new_rd_rsp_info;

  uncacheable_noc_tol3_handler_orig #(.REQQ_SIZE(ReqqSize)) u_orig (
    .reqq_entry(orig_reqq_entry),
    .wr_reqq_index(orig_wr_reqq_index_o),
    .wr_reqq_entry_valid(wr_reqq_entry_valid_i),
    .wr_reqq_entry_clear(orig_wr_reqq_entry_clear),
    .rd_reqq_index(orig_rd_reqq_index_o),
    .rd_reqq_entry_valid(rd_reqq_entry_valid_i),
    .rd_reqq_entry_clear(orig_rd_reqq_entry_clear),
    .wr_rsp_info(orig_wr_rsp_info),
    .wr_new_rsp(orig_wr_new_rsp_o),
    .rd_rsp_info(orig_rd_rsp_info),
    .rd_new_rsp(orig_rd_new_rsp_o),
    .l3_enabled(l3_enabled_i),
    .remote_scp_enabled(remote_scp_enabled_i),
    .to_l3_B(orig_to_l3_b),
    .to_l3_BVALID(to_l3_b_valid_i),
    .to_l3_R(orig_to_l3_r),
    .to_l3_RVALID(to_l3_r_valid_i)
  );

  uncacheable_noc_tol3_handler #(.ReqqSize(ReqqSize)) u_new (
    .reqq_entry_i(new_reqq_entry),
    .wr_reqq_index_o(new_wr_reqq_index_o),
    .wr_reqq_entry_valid_i(wr_reqq_entry_valid_i),
    .wr_reqq_entry_clear_o(new_wr_reqq_entry_clear),
    .rd_reqq_index_o(new_rd_reqq_index_o),
    .rd_reqq_entry_valid_i(rd_reqq_entry_valid_i),
    .rd_reqq_entry_clear_o(new_rd_reqq_entry_clear),
    .wr_rsp_info_o(new_wr_rsp_info),
    .wr_new_rsp_o(new_wr_new_rsp_o),
    .rd_rsp_info_o(new_rd_rsp_info),
    .rd_new_rsp_o(new_rd_new_rsp_o),
    .l3_enabled_i(l3_enabled_i),
    .remote_scp_enabled_i(remote_scp_enabled_i),
    .to_l3_b_i(new_to_l3_b),
    .to_l3_b_valid_i(to_l3_b_valid_i),
    .to_l3_r_i(new_to_l3_r),
    .to_l3_r_valid_i(to_l3_r_valid_i)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_clock_reset;
  assign unused_clock_reset = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : cosim_uncacheable_noc_tol3_handler_tb
