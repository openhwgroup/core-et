// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncacheable_noc_tb #(
  parameter int unsigned ReqqSize = shire_uncached_pkg::UcReqqSize,
  parameter int unsigned NumPortsP = shire_uncached_pkg::NumNeigh,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [shire_uncached_pkg::NumShireIdsBits-1:0] shire_id_i,
  input  logic uc_config_bits_i,
  input  logic neigh_uc_req_valid_i,
  input  logic [$bits(shirecache_pkg::xbar_req_t)-1:0] neigh_uc_req_info_bits_i,
  input  logic [NumPortsP-1:0] neigh_uc_min_rsp_ready_i,
  input  logic credit_i,
  input  logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] from_sys_aw_bits_i,
  input  logic from_sys_aw_valid_i,
  input  logic [$bits(axi_pkg::sys_slave_w_t)-1:0] from_sys_w_bits_i,
  input  logic from_sys_w_valid_i,
  input  logic from_sys_b_ready_i,
  input  logic l3_enabled_i,
  input  logic remote_scp_enabled_i,
  input  logic to_l3_ar_ready_i,
  input  logic to_l3_aw_ready_i,
  input  logic to_l3_w_ready_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_l3_b_bits_i,
  input  logic to_l3_b_valid_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_l3_r_bits_i,
  input  logic to_l3_r_valid_i,
  input  logic to_sys_ar_ready_i,
  input  logic to_sys_aw_ready_i,
  input  logic to_sys_w_ready_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_sys_b_bits_i,
  input  logic to_sys_b_valid_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_sys_r_bits_i,
  input  logic to_sys_r_valid_i,
  output logic orig_neigh_uc_req_ready_o,
  output logic new_neigh_uc_req_ready_o,
  output logic [NumPortsP-1:0] orig_neigh_uc_min_rsp_valid_o,
  output logic [NumPortsP-1:0] new_neigh_uc_min_rsp_valid_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] orig_neigh_uc_min_rsp_info_bits_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] new_neigh_uc_min_rsp_info_bits_o,
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
  output logic new_to_l3_w_valid_o,
  output logic orig_to_l3_b_ready_o,
  output logic new_to_l3_b_ready_o,
  output logic orig_to_l3_r_ready_o,
  output logic new_to_l3_r_ready_o,
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
  output logic orig_to_sys_b_ready_o,
  output logic new_to_sys_b_ready_o,
  output logic orig_to_sys_r_ready_o,
  output logic new_to_sys_r_ready_o
);

  esr_uc_config_t orig_uc_config;
  shire_esr_pkg::esr_uc_config_t new_uc_config;
  sc_xbar_req_t orig_neigh_uc_req_info;
  shirecache_pkg::xbar_req_t new_neigh_uc_req_info;
  sc_xbar_rsp_t orig_neigh_uc_min_rsp_info;
  shirecache_pkg::xbar_rsp_t new_neigh_uc_min_rsp_info;
  sys_slave_axi_AW_channel_t orig_from_sys_aw;
  axi_pkg::sys_slave_aw_t new_from_sys_aw;
  sys_slave_axi_W_channel_t orig_from_sys_w;
  axi_pkg::sys_slave_w_t new_from_sys_w;
  sys_slave_axi_B_channel_t orig_from_sys_b;
  axi_pkg::sys_slave_b_t new_from_sys_b;
  sc_master_axi_AR_channel_t orig_to_l3_ar;
  axi_pkg::sc_master_ar_t new_to_l3_ar;
  sc_master_axi_AW_channel_t orig_to_l3_aw;
  axi_pkg::sc_master_aw_t new_to_l3_aw;
  sc_master_axi_W_channel_t orig_to_l3_w;
  axi_pkg::sc_master_w_t new_to_l3_w;
  sc_master_axi_B_channel_t orig_to_l3_b;
  axi_pkg::sc_master_b_t new_to_l3_b;
  sc_master_axi_R_channel_t orig_to_l3_r;
  axi_pkg::sc_master_r_t new_to_l3_r;
  sc_master_axi_AR_channel_t orig_to_sys_ar;
  axi_pkg::sc_master_ar_t new_to_sys_ar;
  sc_master_axi_AW_channel_t orig_to_sys_aw;
  axi_pkg::sc_master_aw_t new_to_sys_aw;
  sc_master_axi_W_channel_t orig_to_sys_w;
  axi_pkg::sc_master_w_t new_to_sys_w;
  sc_master_axi_B_channel_t orig_to_sys_b;
  axi_pkg::sc_master_b_t new_to_sys_b;
  sc_master_axi_R_channel_t orig_to_sys_r;
  axi_pkg::sc_master_r_t new_to_sys_r;

  assign orig_uc_config = uc_config_bits_i;
  assign new_uc_config = uc_config_bits_i;
  assign orig_neigh_uc_req_info = neigh_uc_req_info_bits_i[$bits(sc_xbar_req_t)-1:0];
  assign new_neigh_uc_req_info = neigh_uc_req_info_bits_i;
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

  assign orig_neigh_uc_min_rsp_info_bits_o = orig_neigh_uc_min_rsp_info;
  assign new_neigh_uc_min_rsp_info_bits_o = new_neigh_uc_min_rsp_info;
  assign orig_from_sys_b_bits_o = orig_from_sys_b;
  assign new_from_sys_b_bits_o = new_from_sys_b;
  assign orig_to_l3_ar_bits_o = orig_to_l3_ar;
  assign new_to_l3_ar_bits_o = new_to_l3_ar;
  assign orig_to_l3_aw_bits_o = orig_to_l3_aw;
  assign new_to_l3_aw_bits_o = new_to_l3_aw;
  assign orig_to_l3_w_bits_o = orig_to_l3_w;
  assign new_to_l3_w_bits_o = new_to_l3_w;
  assign orig_to_sys_ar_bits_o = orig_to_sys_ar;
  assign new_to_sys_ar_bits_o = new_to_sys_ar;
  assign orig_to_sys_aw_bits_o = orig_to_sys_aw;
  assign new_to_sys_aw_bits_o = new_to_sys_aw;
  assign orig_to_sys_w_bits_o = orig_to_sys_w;
  assign new_to_sys_w_bits_o = new_to_sys_w;

  uncacheable_noc_orig #(.REQQ_SIZE(ReqqSize), .NUM_PORTS(NumPortsP)) u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .shire_id(shire_id_i),
    .uc_config(orig_uc_config),
    .neigh_uc_req_ready(orig_neigh_uc_req_ready_o),
    .neigh_uc_req_valid(neigh_uc_req_valid_i),
    .neigh_uc_req_info(orig_neigh_uc_req_info),
    .neigh_uc_min_rsp_ready(neigh_uc_min_rsp_ready_i),
    .neigh_uc_min_rsp_valid(orig_neigh_uc_min_rsp_valid_o),
    .neigh_uc_min_rsp_info(orig_neigh_uc_min_rsp_info),
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
    .l3_enabled(l3_enabled_i),
    .remote_scp_enabled(remote_scp_enabled_i),
    .to_l3_AR(orig_to_l3_ar),
    .to_l3_ARVALID(orig_to_l3_ar_valid_o),
    .to_l3_ARREADY(to_l3_ar_ready_i),
    .to_l3_AW(orig_to_l3_aw),
    .to_l3_AWVALID(orig_to_l3_aw_valid_o),
    .to_l3_AWREADY(to_l3_aw_ready_i),
    .to_l3_W(orig_to_l3_w),
    .to_l3_WVALID(orig_to_l3_w_valid_o),
    .to_l3_WREADY(to_l3_w_ready_i),
    .to_l3_B(orig_to_l3_b),
    .to_l3_BVALID(to_l3_b_valid_i),
    .to_l3_BREADY(orig_to_l3_b_ready_o),
    .to_l3_R(orig_to_l3_r),
    .to_l3_RVALID(to_l3_r_valid_i),
    .to_l3_RREADY(orig_to_l3_r_ready_o),
    .to_sys_AR(orig_to_sys_ar),
    .to_sys_ARVALID(orig_to_sys_ar_valid_o),
    .to_sys_ARREADY(to_sys_ar_ready_i),
    .to_sys_AW(orig_to_sys_aw),
    .to_sys_AWVALID(orig_to_sys_aw_valid_o),
    .to_sys_AWREADY(to_sys_aw_ready_i),
    .to_sys_W(orig_to_sys_w),
    .to_sys_WVALID(orig_to_sys_w_valid_o),
    .to_sys_WREADY(to_sys_w_ready_i),
    .to_sys_B(orig_to_sys_b),
    .to_sys_BVALID(to_sys_b_valid_i),
    .to_sys_BREADY(orig_to_sys_b_ready_o),
    .to_sys_R(orig_to_sys_r),
    .to_sys_RVALID(to_sys_r_valid_i),
    .to_sys_RREADY(orig_to_sys_r_ready_o)
  );

  uncacheable_noc #(.ReqqSize(ReqqSize), .NumPortsP(NumPortsP)) u_new (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .shire_id_i(shire_id_i),
    .uc_config_i(new_uc_config),
    .neigh_uc_req_ready_o(new_neigh_uc_req_ready_o),
    .neigh_uc_req_valid_i(neigh_uc_req_valid_i),
    .neigh_uc_req_info_i(new_neigh_uc_req_info),
    .neigh_uc_min_rsp_ready_i(neigh_uc_min_rsp_ready_i),
    .neigh_uc_min_rsp_valid_o(new_neigh_uc_min_rsp_valid_o),
    .neigh_uc_min_rsp_info_o(new_neigh_uc_min_rsp_info),
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
    .l3_enabled_i(l3_enabled_i),
    .remote_scp_enabled_i(remote_scp_enabled_i),
    .to_l3_ar_o(new_to_l3_ar),
    .to_l3_ar_valid_o(new_to_l3_ar_valid_o),
    .to_l3_ar_ready_i(to_l3_ar_ready_i),
    .to_l3_aw_o(new_to_l3_aw),
    .to_l3_aw_valid_o(new_to_l3_aw_valid_o),
    .to_l3_aw_ready_i(to_l3_aw_ready_i),
    .to_l3_w_o(new_to_l3_w),
    .to_l3_w_valid_o(new_to_l3_w_valid_o),
    .to_l3_w_ready_i(to_l3_w_ready_i),
    .to_l3_b_i(new_to_l3_b),
    .to_l3_b_valid_i(to_l3_b_valid_i),
    .to_l3_b_ready_o(new_to_l3_b_ready_o),
    .to_l3_r_i(new_to_l3_r),
    .to_l3_r_valid_i(to_l3_r_valid_i),
    .to_l3_r_ready_o(new_to_l3_r_ready_o),
    .to_sys_ar_o(new_to_sys_ar),
    .to_sys_ar_valid_o(new_to_sys_ar_valid_o),
    .to_sys_ar_ready_i(to_sys_ar_ready_i),
    .to_sys_aw_o(new_to_sys_aw),
    .to_sys_aw_valid_o(new_to_sys_aw_valid_o),
    .to_sys_aw_ready_i(to_sys_aw_ready_i),
    .to_sys_w_o(new_to_sys_w),
    .to_sys_w_valid_o(new_to_sys_w_valid_o),
    .to_sys_w_ready_i(to_sys_w_ready_i),
    .to_sys_b_i(new_to_sys_b),
    .to_sys_b_valid_i(to_sys_b_valid_i),
    .to_sys_b_ready_o(new_to_sys_b_ready_o),
    .to_sys_r_i(new_to_sys_r),
    .to_sys_r_valid_i(to_sys_r_valid_i),
    .to_sys_r_ready_o(new_to_sys_r_ready_o)
  );

endmodule : cosim_uncacheable_noc_tb
