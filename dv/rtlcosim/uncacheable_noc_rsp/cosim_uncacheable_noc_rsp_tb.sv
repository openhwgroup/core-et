// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncacheable_noc_rsp_tb #(
  parameter int unsigned ReqqSize = shire_uncached_pkg::UcReqqSize,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic rsp_send_i,
  input  logic [$bits(shire_uncached_pkg::uc_reqq_entry_t)-1:0] reqq_entry_bits_i,
  input  logic reqq_entry_valid_i,
  input  logic broadcast_rsp_is_error_reg_i,
  input  logic [shire_uncached_pkg::BcastSendsSize-1:0] broadcast_acks_i,
  input  logic [ReqqIndexWidth-1:0] broadcast_req_index_i,
  input  logic credit_i,
  input  logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] from_sys_aw_bits_i,
  input  logic from_sys_aw_valid_i,
  input  logic [$bits(axi_pkg::sys_slave_w_t)-1:0] from_sys_w_bits_i,
  input  logic from_sys_w_valid_i,
  input  logic from_sys_b_ready_i,
  input  logic remote_scp_enabled_i,
  input  logic l3_enabled_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_l3_b_bits_i,
  input  logic to_l3_b_valid_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_l3_r_bits_i,
  input  logic to_l3_r_valid_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_sys_b_bits_i,
  input  logic to_sys_b_valid_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_sys_r_bits_i,
  input  logic to_sys_r_valid_i,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] orig_rsp_data_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] new_rsp_data_bits_o,
  output logic orig_rsp_valid_o,
  output logic new_rsp_valid_o,
  output logic [ReqqIndexWidth-1:0] orig_reqq_index_o,
  output logic [ReqqIndexWidth-1:0] new_reqq_index_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] orig_reqq_entry_clear_bits_o,
  output logic [$bits(shire_uncached_pkg::uc_reqq_valid_t)-1:0] new_reqq_entry_clear_bits_o,
  output logic orig_broadcast_new_ack_o,
  output logic new_broadcast_new_ack_o,
  output logic orig_broadcast_req_done_o,
  output logic new_broadcast_req_done_o,
  output logic orig_broadcast_rsp_is_error_o,
  output logic new_broadcast_rsp_is_error_o,
  output logic orig_credit_return_o,
  output logic new_credit_return_o,
  output logic orig_from_sys_aw_ready_o,
  output logic new_from_sys_aw_ready_o,
  output logic orig_from_sys_w_ready_o,
  output logic new_from_sys_w_ready_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] orig_from_sys_b_bits_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] new_from_sys_b_bits_o,
  output logic orig_from_sys_b_valid_o,
  output logic new_from_sys_b_valid_o,
  output logic orig_to_l3_b_ready_o,
  output logic new_to_l3_b_ready_o,
  output logic orig_to_l3_r_ready_o,
  output logic new_to_l3_r_ready_o,
  output logic orig_to_sys_b_ready_o,
  output logic new_to_sys_b_ready_o,
  output logic orig_to_sys_r_ready_o,
  output logic new_to_sys_r_ready_o
);

  sc_xbar_rsp_t orig_rsp_data;
  shirecache_pkg::xbar_rsp_t new_rsp_data;
  uc_reqq_entry_t orig_reqq_entry;
  shire_uncached_pkg::uc_reqq_entry_t new_reqq_entry;
  uc_reqq_valid_t orig_reqq_entry_clear;
  shire_uncached_pkg::uc_reqq_valid_t new_reqq_entry_clear;
  sys_slave_axi_AW_channel_t orig_from_sys_aw;
  axi_pkg::sys_slave_aw_t new_from_sys_aw;
  sys_slave_axi_W_channel_t orig_from_sys_w;
  axi_pkg::sys_slave_w_t new_from_sys_w;
  sys_slave_axi_B_channel_t orig_from_sys_b;
  axi_pkg::sys_slave_b_t new_from_sys_b;
  sc_master_axi_B_channel_t orig_to_l3_b;
  axi_pkg::sc_master_b_t new_to_l3_b;
  sc_master_axi_R_channel_t orig_to_l3_r;
  axi_pkg::sc_master_r_t new_to_l3_r;
  sc_master_axi_B_channel_t orig_to_sys_b;
  axi_pkg::sc_master_b_t new_to_sys_b;
  sc_master_axi_R_channel_t orig_to_sys_r;
  axi_pkg::sc_master_r_t new_to_sys_r;

  assign orig_reqq_entry = reqq_entry_bits_i[$bits(uc_reqq_entry_t)-1:0];
  assign new_reqq_entry = reqq_entry_bits_i;
  assign orig_from_sys_aw = from_sys_aw_bits_i[$bits(sys_slave_axi_AW_channel_t)-1:0];
  assign new_from_sys_aw = from_sys_aw_bits_i;
  assign orig_from_sys_w = from_sys_w_bits_i[$bits(sys_slave_axi_W_channel_t)-1:0];
  assign new_from_sys_w = from_sys_w_bits_i;
  assign orig_to_l3_b = to_l3_b_bits_i[$bits(sc_master_axi_B_channel_t)-1:0];
  assign new_to_l3_b = to_l3_b_bits_i;
  assign orig_to_l3_r = to_l3_r_bits_i[$bits(sc_master_axi_R_channel_t)-1:0];
  assign new_to_l3_r = to_l3_r_bits_i;
  assign orig_to_sys_b = to_sys_b_bits_i[$bits(sc_master_axi_B_channel_t)-1:0];
  assign new_to_sys_b = to_sys_b_bits_i;
  assign orig_to_sys_r = to_sys_r_bits_i[$bits(sc_master_axi_R_channel_t)-1:0];
  assign new_to_sys_r = to_sys_r_bits_i;

  assign orig_rsp_data_bits_o = orig_rsp_data;
  assign new_rsp_data_bits_o = new_rsp_data;
  assign orig_reqq_entry_clear_bits_o = orig_reqq_entry_clear;
  assign new_reqq_entry_clear_bits_o = new_reqq_entry_clear;
  assign orig_from_sys_b_bits_o = orig_from_sys_b;
  assign new_from_sys_b_bits_o = new_from_sys_b;

  uncacheable_noc_rsp_orig #(.REQQ_SIZE(ReqqSize)) u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .rsp_data(orig_rsp_data),
    .rsp_valid(orig_rsp_valid_o),
    .rsp_send(rsp_send_i),
    .reqq_entry(orig_reqq_entry),
    .reqq_index(orig_reqq_index_o),
    .reqq_entry_valid(reqq_entry_valid_i),
    .reqq_entry_clear(orig_reqq_entry_clear),
    .broadcast_new_ack(orig_broadcast_new_ack_o),
    .broadcast_req_done(orig_broadcast_req_done_o),
    .broadcast_rsp_is_error(orig_broadcast_rsp_is_error_o),
    .broadcast_rsp_is_error_reg(broadcast_rsp_is_error_reg_i),
    .broadcast_acks(broadcast_acks_i),
    .broadcast_req_index(broadcast_req_index_i),
    .credit(credit_i),
    .credit_return(orig_credit_return_o),
    .from_sys_AW(orig_from_sys_aw),
    .from_sys_AWVALID(from_sys_aw_valid_i),
    .from_sys_AWREADY(orig_from_sys_aw_ready_o),
    .from_sys_W(orig_from_sys_w),
    .from_sys_WVALID(from_sys_w_valid_i),
    .from_sys_WREADY(orig_from_sys_w_ready_o),
    .from_sys_B(orig_from_sys_b),
    .from_sys_BVALID(orig_from_sys_b_valid_o),
    .from_sys_BREADY(from_sys_b_ready_i),
    .remote_scp_enabled(remote_scp_enabled_i),
    .l3_enabled(l3_enabled_i),
    .to_l3_B(orig_to_l3_b),
    .to_l3_BVALID(to_l3_b_valid_i),
    .to_l3_BREADY(orig_to_l3_b_ready_o),
    .to_l3_R(orig_to_l3_r),
    .to_l3_RVALID(to_l3_r_valid_i),
    .to_l3_RREADY(orig_to_l3_r_ready_o),
    .to_sys_B(orig_to_sys_b),
    .to_sys_BVALID(to_sys_b_valid_i),
    .to_sys_BREADY(orig_to_sys_b_ready_o),
    .to_sys_R(orig_to_sys_r),
    .to_sys_RVALID(to_sys_r_valid_i),
    .to_sys_RREADY(orig_to_sys_r_ready_o)
  );

  uncacheable_noc_rsp #(.ReqqSize(ReqqSize)) u_new (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .rsp_data_o(new_rsp_data),
    .rsp_valid_o(new_rsp_valid_o),
    .rsp_send_i(rsp_send_i),
    .reqq_entry_i(new_reqq_entry),
    .reqq_index_o(new_reqq_index_o),
    .reqq_entry_valid_i(reqq_entry_valid_i),
    .reqq_entry_clear_o(new_reqq_entry_clear),
    .broadcast_new_ack_o(new_broadcast_new_ack_o),
    .broadcast_req_done_o(new_broadcast_req_done_o),
    .broadcast_rsp_is_error_o(new_broadcast_rsp_is_error_o),
    .broadcast_rsp_is_error_reg_i(broadcast_rsp_is_error_reg_i),
    .broadcast_acks_i(broadcast_acks_i),
    .broadcast_req_index_i(broadcast_req_index_i),
    .credit_i(credit_i),
    .credit_return_o(new_credit_return_o),
    .from_sys_aw_i(new_from_sys_aw),
    .from_sys_aw_valid_i(from_sys_aw_valid_i),
    .from_sys_aw_ready_o(new_from_sys_aw_ready_o),
    .from_sys_w_i(new_from_sys_w),
    .from_sys_w_valid_i(from_sys_w_valid_i),
    .from_sys_w_ready_o(new_from_sys_w_ready_o),
    .from_sys_b_o(new_from_sys_b),
    .from_sys_b_valid_o(new_from_sys_b_valid_o),
    .from_sys_b_ready_i(from_sys_b_ready_i),
    .remote_scp_enabled_i(remote_scp_enabled_i),
    .l3_enabled_i(l3_enabled_i),
    .to_l3_b_i(new_to_l3_b),
    .to_l3_b_valid_i(to_l3_b_valid_i),
    .to_l3_b_ready_o(new_to_l3_b_ready_o),
    .to_l3_r_i(new_to_l3_r),
    .to_l3_r_valid_i(to_l3_r_valid_i),
    .to_l3_r_ready_o(new_to_l3_r_ready_o),
    .to_sys_b_i(new_to_sys_b),
    .to_sys_b_valid_i(to_sys_b_valid_i),
    .to_sys_b_ready_o(new_to_sys_b_ready_o),
    .to_sys_r_i(new_to_sys_r),
    .to_sys_r_valid_i(to_sys_r_valid_i),
    .to_sys_r_ready_o(new_to_sys_r_ready_o)
  );

endmodule : cosim_uncacheable_noc_rsp_tb
