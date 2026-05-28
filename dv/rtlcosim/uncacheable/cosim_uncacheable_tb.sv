// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncacheable_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic dft_hv_scanmode_i,
  input  logic dft_hv_scan_reset_ni,
  input  logic dft_lv_scanmode_i,
  input  logic dft_lv_scan_reset_ni,

  input  logic [shire_uncached_pkg::NumShireIdsBits-1:0] shire_id_i,
  input  logic [$bits(shire_uncached_pkg::uc_esr_enable_sigs_t)-1:0] esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] esr_wdata_i,
  input  logic [$bits(shire_esr_pkg::esr_uc_config_t)-1:0] esr_uc_config_i,

  input  logic [1:0] flb_neigh_l2_req_valid_i,
  input  logic [2*shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,
  input  logic neigh_uc_req_valid_i,
  input  logic [$bits(shirecache_pkg::xbar_req_t)-1:0] neigh_uc_req_info_i,
  input  logic [2:0] neigh_uc_min_rsp_ready_i,

  input  logic sbm_write_credit_return_i,
  input  logic sbm_sys_axi_ar_ready_i,
  input  logic sbm_sys_axi_aw_ready_i,
  input  logic sbm_sys_axi_w_ready_i,
  input  logic [$bits(axi_pkg::sys_slave_b_t)-1:0] sbm_sys_axi_b_i,
  input  logic sbm_sys_axi_b_valid_i,
  input  logic [$bits(axi_pkg::sys_slave_r_t)-1:0] sbm_sys_axi_r_i,
  input  logic sbm_sys_axi_r_valid_i,

  input  logic l3_enabled_i,
  input  logic remote_scp_enabled_i,
  input  logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] sys_axi_ar_i,
  input  logic sys_axi_ar_valid_i,
  input  logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] sys_axi_aw_i,
  input  logic sys_axi_aw_valid_i,
  input  logic [$bits(axi_pkg::sys_slave_w_t)-1:0] sys_axi_w_i,
  input  logic sys_axi_w_valid_i,
  input  logic sys_axi_b_ready_i,
  input  logic sys_axi_r_ready_i,
  input  logic [1:0] sys_axi_aw_vcvalid_i,
  input  logic [1:0] sys_axi_w_vcvalid_i,

  input  logic to_l3_ar_ready_i,
  input  logic to_l3_aw_ready_i,
  input  logic to_l3_w_ready_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_l3_b_i,
  input  logic to_l3_b_valid_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_l3_r_i,
  input  logic to_l3_r_valid_i,
  input  logic to_sys_ar_ready_i,
  input  logic to_sys_aw_ready_i,
  input  logic to_sys_w_ready_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_sys_b_i,
  input  logic to_sys_b_valid_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_sys_r_i,
  input  logic to_sys_r_valid_i,

  output logic [$bits(shire_uncached_pkg::uc_esr_values_t)-1:0] orig_esr_values_o,
  output logic [$bits(shire_uncached_pkg::uc_esr_values_t)-1:0] new_esr_values_o,
  output logic [1:0] orig_flb_resp_valid_o,
  output logic [1:0] new_flb_resp_valid_o,
  output logic [1:0] orig_flb_resp_data_o,
  output logic [1:0] new_flb_resp_data_o,
  output logic [2*shire_uncached_pkg::MinPerNeigh-1:0] orig_fcc_o,
  output logic [2*shire_uncached_pkg::MinPerNeigh-1:0] new_fcc_o,
  output logic [shire_uncached_pkg::UcFccTargetSize-1:0] orig_fcc_target_o,
  output logic [shire_uncached_pkg::UcFccTargetSize-1:0] new_fcc_target_o,
  output logic orig_neigh_ready_o,
  output logic new_neigh_ready_o,
  output logic [2:0] orig_neigh_rsp_valid_o,
  output logic [2:0] new_neigh_rsp_valid_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] orig_neigh_rsp_info_o,
  output logic [$bits(shirecache_pkg::xbar_rsp_t)-1:0] new_neigh_rsp_info_o,

  output logic orig_sbm_enable_read_o,
  output logic new_sbm_enable_read_o,
  output logic orig_sbm_enable_write_o,
  output logic new_sbm_enable_write_o,
  output logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] orig_sbm_ar_o,
  output logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] new_sbm_ar_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] orig_sbm_aw_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] new_sbm_aw_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] orig_sbm_w_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] new_sbm_w_o,
  output logic orig_sbm_b_ready_o,
  output logic new_sbm_b_ready_o,
  output logic orig_sbm_r_ready_o,
  output logic new_sbm_r_ready_o,

  output logic orig_sys_ar_ready_o,
  output logic new_sys_ar_ready_o,
  output logic orig_sys_aw_ready_o,
  output logic new_sys_aw_ready_o,
  output logic orig_sys_w_ready_o,
  output logic new_sys_w_ready_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] orig_sys_b_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] new_sys_b_o,
  output logic orig_sys_b_valid_o,
  output logic new_sys_b_valid_o,
  output logic [$bits(axi_pkg::sys_slave_r_t)-1:0] orig_sys_r_o,
  output logic [$bits(axi_pkg::sys_slave_r_t)-1:0] new_sys_r_o,
  output logic orig_sys_r_valid_o,
  output logic new_sys_r_valid_o,
  output logic [1:0] orig_sys_aw_credit_o,
  output logic [1:0] new_sys_aw_credit_o,

  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_l3_ar_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_l3_ar_o,
  output logic orig_to_l3_ar_valid_o,
  output logic new_to_l3_ar_valid_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_l3_aw_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_l3_aw_o,
  output logic orig_to_l3_aw_valid_o,
  output logic new_to_l3_aw_valid_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_l3_w_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_to_l3_w_o,
  output logic orig_to_l3_w_valid_o,
  output logic new_to_l3_w_valid_o,
  output logic orig_to_l3_b_ready_o,
  output logic new_to_l3_b_ready_o,
  output logic orig_to_l3_r_ready_o,
  output logic new_to_l3_r_ready_o,

  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_sys_ar_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_sys_ar_o,
  output logic orig_to_sys_ar_valid_o,
  output logic new_to_sys_ar_valid_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_sys_aw_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_sys_aw_o,
  output logic orig_to_sys_aw_valid_o,
  output logic new_to_sys_aw_valid_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_sys_w_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_to_sys_w_o,
  output logic orig_to_sys_w_valid_o,
  output logic new_to_sys_w_valid_o,
  output logic orig_to_sys_b_ready_o,
  output logic new_to_sys_b_ready_o,
  output logic orig_to_sys_r_ready_o,
  output logic new_to_sys_r_ready_o
);
  localparam int unsigned NumNeighLocal = 2;
  localparam int unsigned NumPortsLocal = 3;

  // Original-side typed nets.
  uc_esr_enable_sigs orig_esr_enables;
  uc_esr_values_t orig_esr_values;
  esr_uc_config_t orig_esr_config;
  logic [NumNeighLocal-1:0][`CSR_FL_BARRIER_RANGE] orig_flb_data;
  logic [NumNeighLocal-1:0][`MIN_PER_N-1:0] orig_fcc;
  sc_xbar_req_t orig_neigh_req;
  sc_xbar_rsp_t orig_neigh_rsp;
  sys_slave_axi_AR_channel_t orig_sbm_ar, orig_sys_ar_in, orig_sbm_ar_out;
  sys_slave_axi_AW_channel_t orig_sbm_aw, orig_sys_aw_in, orig_sbm_aw_out;
  sys_slave_axi_W_channel_t  orig_sbm_w, orig_sys_w_in, orig_sbm_w_out;
  sys_slave_axi_B_channel_t  orig_sbm_b_in, orig_sys_b;
  sys_slave_axi_R_channel_t  orig_sbm_r_in, orig_sys_r;
  sc_master_axi_AR_channel_t orig_to_l3_ar, orig_to_sys_ar;
  sc_master_axi_AW_channel_t orig_to_l3_aw, orig_to_sys_aw;
  sc_master_axi_W_channel_t  orig_to_l3_w, orig_to_sys_w;
  sc_master_axi_B_channel_t  orig_to_l3_b_in, orig_to_sys_b_in;
  sc_master_axi_R_channel_t  orig_to_l3_r_in, orig_to_sys_r_in;
  debug_axi_port_ctl_t orig_debug_ctl;
  debug_axi_port_t orig_debug_port;

  // New-side typed nets.
  dft_pkg::dft_t dft_hv, dft_lv;
  shire_uncached_pkg::uc_esr_enable_sigs_t new_esr_enables;
  shire_uncached_pkg::uc_esr_values_t new_esr_values;
  shire_esr_pkg::esr_uc_config_t new_esr_config;
  logic [NumNeighLocal-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] new_flb_data;
  logic [NumNeighLocal-1:0][shire_uncached_pkg::MinPerNeigh-1:0] new_fcc;
  shirecache_pkg::xbar_req_t new_neigh_req;
  shirecache_pkg::xbar_rsp_t new_neigh_rsp;
  axi_pkg::sys_slave_ar_t new_sbm_ar, new_sys_ar_in, new_sbm_ar_out;
  axi_pkg::sys_slave_aw_t new_sbm_aw, new_sys_aw_in, new_sbm_aw_out;
  axi_pkg::sys_slave_w_t  new_sbm_w, new_sys_w_in, new_sbm_w_out;
  axi_pkg::sys_slave_b_t  new_sbm_b_in, new_sys_b;
  axi_pkg::sys_slave_r_t  new_sbm_r_in, new_sys_r;
  axi_pkg::sc_master_ar_t new_to_l3_ar, new_to_sys_ar;
  axi_pkg::sc_master_aw_t new_to_l3_aw, new_to_sys_aw;
  axi_pkg::sc_master_w_t  new_to_l3_w, new_to_sys_w;
  axi_pkg::sc_master_b_t  new_to_l3_b_in, new_to_sys_b_in;
  axi_pkg::sc_master_r_t  new_to_l3_r_in, new_to_sys_r_in;

  assign orig_esr_enables = esr_enables_i;
  assign new_esr_enables = esr_enables_i;
  assign orig_esr_config = esr_uc_config_i;
  assign new_esr_config = esr_uc_config_i;
  assign orig_flb_data = flb_neigh_l2_req_data_i;
  assign new_flb_data = flb_neigh_l2_req_data_i;
  assign orig_neigh_req = neigh_uc_req_info_i;
  assign new_neigh_req = neigh_uc_req_info_i;
  assign orig_sbm_b_in = sbm_sys_axi_b_i;
  assign new_sbm_b_in = sbm_sys_axi_b_i;
  assign orig_sbm_r_in = sbm_sys_axi_r_i;
  assign new_sbm_r_in = sbm_sys_axi_r_i;
  assign orig_sys_ar_in = sys_axi_ar_i;
  assign new_sys_ar_in = sys_axi_ar_i;
  assign orig_sys_aw_in = sys_axi_aw_i;
  assign new_sys_aw_in = sys_axi_aw_i;
  assign orig_sys_w_in = sys_axi_w_i;
  assign new_sys_w_in = sys_axi_w_i;
  assign orig_to_l3_b_in = to_l3_b_i;
  assign new_to_l3_b_in = to_l3_b_i;
  assign orig_to_l3_r_in = to_l3_r_i;
  assign new_to_l3_r_in = to_l3_r_i;
  assign orig_to_sys_b_in = to_sys_b_i;
  assign new_to_sys_b_in = to_sys_b_i;
  assign orig_to_sys_r_in = to_sys_r_i;
  assign new_to_sys_r_in = to_sys_r_i;
  assign orig_debug_ctl = '0;
  assign dft_hv = '{scanmode: dft_hv_scanmode_i, scan_reset_n: dft_hv_scan_reset_ni,
                    sram_clk_override: 1'b0, ram_rei: 1'b0, ram_wei: 1'b0};
  assign dft_lv = '{scanmode: dft_lv_scanmode_i, scan_reset_n: dft_lv_scan_reset_ni,
                    sram_clk_override: 1'b0, ram_rei: 1'b0, ram_wei: 1'b0};

  uncacheable_orig #(
    .NUM_NEIGH(NumNeighLocal),
    .NUM_PORTS(NumPortsLocal)
  ) orig (
    .clock(clk_i), .reset(!rst_ni), .noc_clock(clk_i), .noc_reset(!rst_ni), .shire_id(shire_id_i),
    .esr_enables(orig_esr_enables), .esr_wdata(esr_wdata_i), .esr_values(orig_esr_values), .esr_uc_config(orig_esr_config),
    .flb_neigh_l2_req_valid(flb_neigh_l2_req_valid_i), .flb_neigh_l2_req_data(orig_flb_data),
    .flb_l2_neigh_resp_valid(orig_flb_resp_valid_o), .flb_l2_neigh_resp_data(orig_flb_resp_data_o),
    .uc_to_neigh_fcc(orig_fcc), .uc_to_neigh_fcc_target(orig_fcc_target_o),
    .neigh_uc_req_ready(orig_neigh_ready_o), .neigh_uc_req_valid(neigh_uc_req_valid_i), .neigh_uc_req_info(orig_neigh_req),
    .neigh_uc_min_rsp_ready(neigh_uc_min_rsp_ready_i), .neigh_uc_min_rsp_valid(orig_neigh_rsp_valid_o), .neigh_uc_min_rsp_info(orig_neigh_rsp),
    .debug_axi_port_ctl(orig_debug_ctl), .uc_debug_axi_port(orig_debug_port),
    .sbm_enable_read(orig_sbm_enable_read_o), .sbm_enable_write(orig_sbm_enable_write_o), .sbm_write_credit_return(sbm_write_credit_return_i),
    .sbm_sys_mesh_slave_axi_AR(orig_sbm_ar_out), .sbm_sys_mesh_slave_axi_ARREADY(sbm_sys_axi_ar_ready_i),
    .sbm_sys_mesh_slave_axi_AW(orig_sbm_aw_out), .sbm_sys_mesh_slave_axi_AWREADY(sbm_sys_axi_aw_ready_i),
    .sbm_sys_mesh_slave_axi_W(orig_sbm_w_out), .sbm_sys_mesh_slave_axi_WREADY(sbm_sys_axi_w_ready_i),
    .sbm_sys_mesh_slave_axi_B(orig_sbm_b_in), .sbm_sys_mesh_slave_axi_BVALID(sbm_sys_axi_b_valid_i), .sbm_sys_mesh_slave_axi_BREADY(orig_sbm_b_ready_o),
    .sbm_sys_mesh_slave_axi_R(orig_sbm_r_in), .sbm_sys_mesh_slave_axi_RVALID(sbm_sys_axi_r_valid_i), .sbm_sys_mesh_slave_axi_RREADY(orig_sbm_r_ready_o),
    .l3_enabled(l3_enabled_i), .remote_scp_enabled(remote_scp_enabled_i),
    .sys_mesh_slave_axi_AR(orig_sys_ar_in), .sys_mesh_slave_axi_ARVALID(sys_axi_ar_valid_i), .sys_mesh_slave_axi_ARREADY(orig_sys_ar_ready_o),
    .sys_mesh_slave_axi_AW(orig_sys_aw_in), .sys_mesh_slave_axi_AWVALID(sys_axi_aw_valid_i), .sys_mesh_slave_axi_AWREADY(orig_sys_aw_ready_o),
    .sys_mesh_slave_axi_W(orig_sys_w_in), .sys_mesh_slave_axi_WVALID(sys_axi_w_valid_i), .sys_mesh_slave_axi_WREADY(orig_sys_w_ready_o),
    .sys_mesh_slave_axi_B(orig_sys_b), .sys_mesh_slave_axi_BVALID(orig_sys_b_valid_o), .sys_mesh_slave_axi_BREADY(sys_axi_b_ready_i),
    .sys_mesh_slave_axi_R(orig_sys_r), .sys_mesh_slave_axi_RVALID(orig_sys_r_valid_o), .sys_mesh_slave_axi_RREADY(sys_axi_r_ready_i),
    .sys_mesh_slave_axi_AWvcvalid(sys_axi_aw_vcvalid_i), .sys_mesh_slave_axi_AWcredit(orig_sys_aw_credit_o), .sys_mesh_slave_axi_Wvcvalid(sys_axi_w_vcvalid_i),
    .to_l3_AR(orig_to_l3_ar), .to_l3_ARVALID(orig_to_l3_ar_valid_o), .to_l3_ARREADY(to_l3_ar_ready_i),
    .to_l3_AW(orig_to_l3_aw), .to_l3_AWVALID(orig_to_l3_aw_valid_o), .to_l3_AWREADY(to_l3_aw_ready_i),
    .to_l3_W(orig_to_l3_w), .to_l3_WVALID(orig_to_l3_w_valid_o), .to_l3_WREADY(to_l3_w_ready_i),
    .to_l3_B(orig_to_l3_b_in), .to_l3_BVALID(to_l3_b_valid_i), .to_l3_BREADY(orig_to_l3_b_ready_o),
    .to_l3_R(orig_to_l3_r_in), .to_l3_RVALID(to_l3_r_valid_i), .to_l3_RREADY(orig_to_l3_r_ready_o),
    .to_sys_AR(orig_to_sys_ar), .to_sys_ARVALID(orig_to_sys_ar_valid_o), .to_sys_ARREADY(to_sys_ar_ready_i),
    .to_sys_AW(orig_to_sys_aw), .to_sys_AWVALID(orig_to_sys_aw_valid_o), .to_sys_AWREADY(to_sys_aw_ready_i),
    .to_sys_W(orig_to_sys_w), .to_sys_WVALID(orig_to_sys_w_valid_o), .to_sys_WREADY(to_sys_w_ready_i),
    .to_sys_B(orig_to_sys_b_in), .to_sys_BVALID(to_sys_b_valid_i), .to_sys_BREADY(orig_to_sys_b_ready_o),
    .to_sys_R(orig_to_sys_r_in), .to_sys_RVALID(to_sys_r_valid_i), .to_sys_RREADY(orig_to_sys_r_ready_o),
    .dft__reset_byp_hv(dft_hv_scanmode_i), .dft__reset_hv(!dft_hv_scan_reset_ni),
    .dft__reset_byp_lv(dft_lv_scanmode_i), .dft__reset_lv(!dft_lv_scan_reset_ni)
  );

  uncacheable #(
    .NumNeighP(NumNeighLocal),
    .NumPortsP(NumPortsLocal)
  ) new_dut (
    .clk_i(clk_i), .rst_ni(rst_ni), .noc_clk_i(clk_i), .noc_rst_ni(rst_ni), .shire_id_i(shire_id_i),
    .esr_enables_i(new_esr_enables), .esr_wdata_i(esr_wdata_i), .esr_values_o(new_esr_values), .esr_uc_config_i(new_esr_config),
    .flb_neigh_l2_req_valid_i(flb_neigh_l2_req_valid_i), .flb_neigh_l2_req_data_i(new_flb_data),
    .flb_l2_neigh_resp_valid_o(new_flb_resp_valid_o), .flb_l2_neigh_resp_data_o(new_flb_resp_data_o),
    .uc_to_neigh_fcc_o(new_fcc), .uc_to_neigh_fcc_target_o(new_fcc_target_o),
    .neigh_uc_req_ready_o(new_neigh_ready_o), .neigh_uc_req_valid_i(neigh_uc_req_valid_i), .neigh_uc_req_info_i(new_neigh_req),
    .neigh_uc_min_rsp_ready_i(neigh_uc_min_rsp_ready_i), .neigh_uc_min_rsp_valid_o(new_neigh_rsp_valid_o), .neigh_uc_min_rsp_info_o(new_neigh_rsp),
    .sbm_enable_read_o(new_sbm_enable_read_o), .sbm_enable_write_o(new_sbm_enable_write_o), .sbm_write_credit_return_i(sbm_write_credit_return_i),
    .sbm_sys_axi_ar_o(new_sbm_ar_out), .sbm_sys_axi_ar_ready_i(sbm_sys_axi_ar_ready_i),
    .sbm_sys_axi_aw_o(new_sbm_aw_out), .sbm_sys_axi_aw_ready_i(sbm_sys_axi_aw_ready_i),
    .sbm_sys_axi_w_o(new_sbm_w_out), .sbm_sys_axi_w_ready_i(sbm_sys_axi_w_ready_i),
    .sbm_sys_axi_b_i(new_sbm_b_in), .sbm_sys_axi_b_valid_i(sbm_sys_axi_b_valid_i), .sbm_sys_axi_b_ready_o(new_sbm_b_ready_o),
    .sbm_sys_axi_r_i(new_sbm_r_in), .sbm_sys_axi_r_valid_i(sbm_sys_axi_r_valid_i), .sbm_sys_axi_r_ready_o(new_sbm_r_ready_o),
    .l3_enabled_i(l3_enabled_i), .remote_scp_enabled_i(remote_scp_enabled_i),
    .sys_axi_ar_i(new_sys_ar_in), .sys_axi_ar_valid_i(sys_axi_ar_valid_i), .sys_axi_ar_ready_o(new_sys_ar_ready_o),
    .sys_axi_aw_i(new_sys_aw_in), .sys_axi_aw_valid_i(sys_axi_aw_valid_i), .sys_axi_aw_ready_o(new_sys_aw_ready_o),
    .sys_axi_w_i(new_sys_w_in), .sys_axi_w_valid_i(sys_axi_w_valid_i), .sys_axi_w_ready_o(new_sys_w_ready_o),
    .sys_axi_b_o(new_sys_b), .sys_axi_b_valid_o(new_sys_b_valid_o), .sys_axi_b_ready_i(sys_axi_b_ready_i),
    .sys_axi_r_o(new_sys_r), .sys_axi_r_valid_o(new_sys_r_valid_o), .sys_axi_r_ready_i(sys_axi_r_ready_i),
    .sys_axi_aw_vcvalid_i(sys_axi_aw_vcvalid_i), .sys_axi_aw_credit_o(new_sys_aw_credit_o), .sys_axi_w_vcvalid_i(sys_axi_w_vcvalid_i),
    .to_l3_ar_o(new_to_l3_ar), .to_l3_ar_valid_o(new_to_l3_ar_valid_o), .to_l3_ar_ready_i(to_l3_ar_ready_i),
    .to_l3_aw_o(new_to_l3_aw), .to_l3_aw_valid_o(new_to_l3_aw_valid_o), .to_l3_aw_ready_i(to_l3_aw_ready_i),
    .to_l3_w_o(new_to_l3_w), .to_l3_w_valid_o(new_to_l3_w_valid_o), .to_l3_w_ready_i(to_l3_w_ready_i),
    .to_l3_b_i(new_to_l3_b_in), .to_l3_b_valid_i(to_l3_b_valid_i), .to_l3_b_ready_o(new_to_l3_b_ready_o),
    .to_l3_r_i(new_to_l3_r_in), .to_l3_r_valid_i(to_l3_r_valid_i), .to_l3_r_ready_o(new_to_l3_r_ready_o),
    .to_sys_ar_o(new_to_sys_ar), .to_sys_ar_valid_o(new_to_sys_ar_valid_o), .to_sys_ar_ready_i(to_sys_ar_ready_i),
    .to_sys_aw_o(new_to_sys_aw), .to_sys_aw_valid_o(new_to_sys_aw_valid_o), .to_sys_aw_ready_i(to_sys_aw_ready_i),
    .to_sys_w_o(new_to_sys_w), .to_sys_w_valid_o(new_to_sys_w_valid_o), .to_sys_w_ready_i(to_sys_w_ready_i),
    .to_sys_b_i(new_to_sys_b_in), .to_sys_b_valid_i(to_sys_b_valid_i), .to_sys_b_ready_o(new_to_sys_b_ready_o),
    .to_sys_r_i(new_to_sys_r_in), .to_sys_r_valid_i(to_sys_r_valid_i), .to_sys_r_ready_o(new_to_sys_r_ready_o),
    .dft_hv_i(dft_hv), .dft_lv_i(dft_lv)
  );

  assign orig_esr_values_o = orig_esr_values;
  assign new_esr_values_o = new_esr_values;
  assign orig_fcc_o = orig_fcc;
  assign new_fcc_o = new_fcc;
  assign orig_neigh_rsp_info_o = orig_neigh_rsp;
  assign new_neigh_rsp_info_o = new_neigh_rsp;
  assign orig_sbm_ar_o = orig_sbm_ar_out;
  assign new_sbm_ar_o = new_sbm_ar_out;
  assign orig_sbm_aw_o = orig_sbm_aw_out;
  assign new_sbm_aw_o = new_sbm_aw_out;
  assign orig_sbm_w_o = orig_sbm_w_out;
  assign new_sbm_w_o = new_sbm_w_out;
  assign orig_sys_b_o = orig_sys_b;
  assign new_sys_b_o = new_sys_b;
  assign orig_sys_r_o = orig_sys_r;
  assign new_sys_r_o = new_sys_r;
  assign orig_to_l3_ar_o = orig_to_l3_ar;
  assign new_to_l3_ar_o = new_to_l3_ar;
  assign orig_to_l3_aw_o = orig_to_l3_aw;
  assign new_to_l3_aw_o = new_to_l3_aw;
  assign orig_to_l3_w_o = orig_to_l3_w;
  assign new_to_l3_w_o = new_to_l3_w;
  assign orig_to_sys_ar_o = orig_to_sys_ar;
  assign new_to_sys_ar_o = new_to_sys_ar;
  assign orig_to_sys_aw_o = orig_to_sys_aw;
  assign new_to_sys_aw_o = new_to_sys_aw;
  assign orig_to_sys_w_o = orig_to_sys_w;
  assign new_to_sys_w_o = new_to_sys_w;
endmodule
