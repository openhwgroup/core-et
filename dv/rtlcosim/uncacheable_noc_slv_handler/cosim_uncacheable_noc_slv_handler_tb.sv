// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncacheable_noc_slv_handler_tb #(
  parameter int unsigned ReqqSize = shire_uncached_pkg::UcReqqSize,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(shire_uncached_pkg::uc_reqq_entry_t)-1:0] reqq_entry_bits_i,
  input  logic reqq_entry_valid_i,
  input  logic credit_i,
  input  logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] from_sys_aw_bits_i,
  input  logic from_sys_aw_valid_i,
  input  logic [$bits(axi_pkg::sys_slave_w_t)-1:0] from_sys_w_bits_i,
  input  logic from_sys_w_valid_i,
  output logic [ReqqIndexWidth-1:0] orig_reqq_index_o,
  output logic [ReqqIndexWidth-1:0] new_reqq_index_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] orig_reqq_entry_clear_bits_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] new_reqq_entry_clear_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] orig_rsp_info_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] new_rsp_info_bits_o,
  output logic orig_new_rsp_o,
  output logic new_new_rsp_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] orig_from_sys_b_bits_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] new_from_sys_b_bits_o,
  output logic orig_from_sys_b_valid_o,
  output logic new_from_sys_b_valid_o
);

  uc_reqq_entry_t orig_reqq_entry;
  shire_uncached_pkg::uc_reqq_entry_t new_reqq_entry;
  uc_reqq_valid_t orig_reqq_entry_clear;
  shire_uncached_pkg::uc_reqq_valid_t new_reqq_entry_clear;
  sc_xbar_rsp_t orig_rsp_info;
  shirecache_pkg::xbar_rsp_t new_rsp_info;
  sys_slave_axi_AW_channel_t orig_from_sys_aw;
  axi_pkg::sys_slave_aw_t new_from_sys_aw;
  sys_slave_axi_W_channel_t orig_from_sys_w;
  axi_pkg::sys_slave_w_t new_from_sys_w;
  sys_slave_axi_B_channel_t orig_from_sys_b;
  axi_pkg::sys_slave_b_t new_from_sys_b;

  assign orig_reqq_entry = reqq_entry_bits_i[$bits(uc_reqq_entry_t)-1:0];
  assign new_reqq_entry = reqq_entry_bits_i;
  assign orig_from_sys_aw = from_sys_aw_bits_i[$bits(sys_slave_axi_AW_channel_t)-1:0];
  assign new_from_sys_aw = from_sys_aw_bits_i;
  assign orig_from_sys_w = from_sys_w_bits_i[$bits(sys_slave_axi_W_channel_t)-1:0];
  assign new_from_sys_w = from_sys_w_bits_i;
  assign orig_reqq_entry_clear_bits_o = orig_reqq_entry_clear;
  assign new_reqq_entry_clear_bits_o = new_reqq_entry_clear;
  assign orig_rsp_info_bits_o = orig_rsp_info;
  assign new_rsp_info_bits_o = new_rsp_info;
  assign orig_from_sys_b_bits_o = orig_from_sys_b;
  assign new_from_sys_b_bits_o = new_from_sys_b;

  uncacheable_noc_slv_handler_orig #(.REQQ_SIZE(ReqqSize)) u_orig (
    .reqq_entry(orig_reqq_entry),
    .reqq_index(orig_reqq_index_o),
    .reqq_entry_valid(reqq_entry_valid_i),
    .reqq_entry_clear(orig_reqq_entry_clear),
    .rsp_info(orig_rsp_info),
    .new_rsp(orig_new_rsp_o),
    .credit(credit_i),
    .from_sys_AW(orig_from_sys_aw),
    .from_sys_AWVALID(from_sys_aw_valid_i),
    .from_sys_W(orig_from_sys_w),
    .from_sys_WVALID(from_sys_w_valid_i),
    .from_sys_B(orig_from_sys_b),
    .from_sys_BVALID(orig_from_sys_b_valid_o)
  );

  uncacheable_noc_slv_handler #(.ReqqSize(ReqqSize)) u_new (
    .reqq_entry_i(new_reqq_entry),
    .reqq_index_o(new_reqq_index_o),
    .reqq_entry_valid_i(reqq_entry_valid_i),
    .reqq_entry_clear_o(new_reqq_entry_clear),
    .rsp_info_o(new_rsp_info),
    .new_rsp_o(new_new_rsp_o),
    .credit_i(credit_i),
    .from_sys_aw_i(new_from_sys_aw),
    .from_sys_aw_valid_i(from_sys_aw_valid_i),
    .from_sys_w_i(new_from_sys_w),
    .from_sys_w_valid_i(from_sys_w_valid_i),
    .from_sys_b_o(new_from_sys_b),
    .from_sys_b_valid_o(new_from_sys_b_valid_o)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_clock_reset;
  assign unused_clock_reset = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : cosim_uncacheable_noc_slv_handler_tb
