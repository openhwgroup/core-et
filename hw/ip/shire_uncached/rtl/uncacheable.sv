// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncacheable
  import axi_pkg::*;
  import dft_pkg::*;
  import shirecache_pkg::*;
  import shire_esr_pkg::*;
  import shire_uncached_pkg::*;
#(
  parameter int unsigned NumNeighP = NumNeigh,
  parameter int unsigned NumPortsP = NumNeighP + RboxPerShire
) (
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic                       noc_clk_i,
  input  logic                       noc_rst_ni,
  input  logic [NumShireIdsBits-1:0] shire_id_i,

  input  uc_esr_enable_sigs_t        esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] esr_wdata_i,
  output uc_esr_values_t             esr_values_o,
  input  esr_uc_config_t             esr_uc_config_i,

  input  logic [NumNeighP-1:0]       flb_neigh_l2_req_valid_i,
  input  logic [NumNeighP-1:0][CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,
  output logic [NumNeighP-1:0]       flb_l2_neigh_resp_valid_o,
  output logic [NumNeighP-1:0]       flb_l2_neigh_resp_data_o,

  output logic [NumNeighP-1:0][MinPerNeigh-1:0] uc_to_neigh_fcc_o,
  output logic [UcFccTargetSize-1:0] uc_to_neigh_fcc_target_o,

  output logic                       neigh_uc_req_ready_o,
  input  logic                       neigh_uc_req_valid_i,
  input  shirecache_pkg::xbar_req_t  neigh_uc_req_info_i,
  input  logic [NumPortsP-1:0]       neigh_uc_min_rsp_ready_i,
  output logic [NumPortsP-1:0]       neigh_uc_min_rsp_valid_o,
  output shirecache_pkg::xbar_rsp_t  neigh_uc_min_rsp_info_o,

  output logic                       sbm_enable_read_o,
  output logic                       sbm_enable_write_o,
  input  logic                       sbm_write_credit_return_i,
  output sys_slave_ar_t              sbm_sys_axi_ar_o,
  input  logic                       sbm_sys_axi_ar_ready_i,
  output sys_slave_aw_t              sbm_sys_axi_aw_o,
  input  logic                       sbm_sys_axi_aw_ready_i,
  output sys_slave_w_t               sbm_sys_axi_w_o,
  input  logic                       sbm_sys_axi_w_ready_i,
  input  sys_slave_b_t               sbm_sys_axi_b_i,
  input  logic                       sbm_sys_axi_b_valid_i,
  output logic                       sbm_sys_axi_b_ready_o,
  input  sys_slave_r_t               sbm_sys_axi_r_i,
  input  logic                       sbm_sys_axi_r_valid_i,
  output logic                       sbm_sys_axi_r_ready_o,

  input  logic                       l3_enabled_i,
  input  logic                       remote_scp_enabled_i,

  input  sys_slave_ar_t              sys_axi_ar_i,
  input  logic                       sys_axi_ar_valid_i,
  output logic                       sys_axi_ar_ready_o,
  input  sys_slave_aw_t              sys_axi_aw_i,
  input  logic                       sys_axi_aw_valid_i,
  output logic                       sys_axi_aw_ready_o,
  input  sys_slave_w_t               sys_axi_w_i,
  input  logic                       sys_axi_w_valid_i,
  output logic                       sys_axi_w_ready_o,
  output sys_slave_b_t               sys_axi_b_o,
  output logic                       sys_axi_b_valid_o,
  input  logic                       sys_axi_b_ready_i,
  output sys_slave_r_t               sys_axi_r_o,
  output logic                       sys_axi_r_valid_o,
  input  logic                       sys_axi_r_ready_i,
  input  logic [1:0]                 sys_axi_aw_vcvalid_i,
  output logic [1:0]                 sys_axi_aw_credit_o,
  input  logic [1:0]                 sys_axi_w_vcvalid_i,

  output sc_master_ar_t              to_l3_ar_o,
  output logic                       to_l3_ar_valid_o,
  input  logic                       to_l3_ar_ready_i,
  output sc_master_aw_t              to_l3_aw_o,
  output logic                       to_l3_aw_valid_o,
  input  logic                       to_l3_aw_ready_i,
  output sc_master_w_t               to_l3_w_o,
  output logic                       to_l3_w_valid_o,
  input  logic                       to_l3_w_ready_i,
  input  sc_master_b_t               to_l3_b_i,
  input  logic                       to_l3_b_valid_i,
  output logic                       to_l3_b_ready_o,
  input  sc_master_r_t               to_l3_r_i,
  input  logic                       to_l3_r_valid_i,
  output logic                       to_l3_r_ready_o,

  output sc_master_ar_t              to_sys_ar_o,
  output logic                       to_sys_ar_valid_o,
  input  logic                       to_sys_ar_ready_i,
  output sc_master_aw_t              to_sys_aw_o,
  output logic                       to_sys_aw_valid_o,
  input  logic                       to_sys_aw_ready_i,
  output sc_master_w_t               to_sys_w_o,
  output logic                       to_sys_w_valid_o,
  input  logic                       to_sys_w_ready_i,
  input  sc_master_b_t               to_sys_b_i,
  input  logic                       to_sys_b_valid_i,
  output logic                       to_sys_b_ready_o,
  input  sc_master_r_t               to_sys_r_i,
  input  logic                       to_sys_r_valid_i,
  output logic                       to_sys_r_ready_o,

  input  dft_t                       dft_hv_i,
  input  dft_t                       dft_lv_i
);

  localparam int unsigned SbmSysSlaveVsiId = 1;
  localparam int unsigned UcSysSlaveVsiId = 0;

  sys_slave_aw_t uc_sys_aw_hi;
  logic uc_sys_aw_valid_hi;
  logic uc_sys_aw_ready_hi;
  sys_slave_w_t uc_sys_w_hi;
  logic uc_sys_w_valid_hi;
  logic uc_sys_w_ready_hi;
  sys_slave_b_t uc_sys_b_hi;
  logic uc_sys_b_valid_hi;
  logic uc_sys_b_ready_hi;
  logic credit_hi_uc;
  logic return_credit_hi_uc;

  sc_master_ar_t uc_to_l3_ar_hi, uc_to_l3_ar_lo;
  logic uc_to_l3_ar_valid_hi, uc_to_l3_ar_ready_hi;
  logic uc_to_l3_ar_valid_lo, uc_to_l3_ar_ready_lo;
  sc_master_aw_t uc_to_l3_aw_hi, uc_to_l3_aw_lo;
  logic uc_to_l3_aw_valid_hi, uc_to_l3_aw_ready_hi;
  logic uc_to_l3_aw_valid_lo, uc_to_l3_aw_ready_lo;
  sc_master_w_t uc_to_l3_w_hi, uc_to_l3_w_lo;
  logic uc_to_l3_w_valid_hi, uc_to_l3_w_ready_hi;
  logic uc_to_l3_w_valid_lo, uc_to_l3_w_ready_lo;
  sc_master_b_t uc_to_l3_b_hi, uc_to_l3_b_lo;
  logic uc_to_l3_b_valid_hi, uc_to_l3_b_ready_hi;
  logic uc_to_l3_b_valid_lo, uc_to_l3_b_ready_lo;
  sc_master_r_t uc_to_l3_r_hi, uc_to_l3_r_lo;
  logic uc_to_l3_r_valid_hi, uc_to_l3_r_ready_hi;
  logic uc_to_l3_r_valid_lo, uc_to_l3_r_ready_lo;

  sc_master_ar_t uc_to_sys_ar_hi, uc_to_sys_ar_lo;
  logic uc_to_sys_ar_valid_hi, uc_to_sys_ar_ready_hi;
  logic uc_to_sys_ar_valid_lo, uc_to_sys_ar_ready_lo;
  sc_master_aw_t uc_to_sys_aw_hi, uc_to_sys_aw_lo;
  logic uc_to_sys_aw_valid_hi, uc_to_sys_aw_ready_hi;
  logic uc_to_sys_aw_valid_lo, uc_to_sys_aw_ready_lo;
  sc_master_w_t uc_to_sys_w_hi, uc_to_sys_w_lo;
  logic uc_to_sys_w_valid_hi, uc_to_sys_w_ready_hi;
  logic uc_to_sys_w_valid_lo, uc_to_sys_w_ready_lo;
  sc_master_b_t uc_to_sys_b_hi, uc_to_sys_b_lo;
  logic uc_to_sys_b_valid_hi, uc_to_sys_b_ready_hi;
  logic uc_to_sys_b_valid_lo, uc_to_sys_b_ready_lo;
  sc_master_r_t uc_to_sys_r_hi, uc_to_sys_r_lo;
  logic uc_to_sys_r_valid_hi, uc_to_sys_r_ready_hi;
  logic uc_to_sys_r_valid_lo, uc_to_sys_r_ready_lo;

  sys_slave_ar_t sys_ar_hi;
  logic sys_ar_valid_hi;
  logic sys_ar_ready_hi;
  sys_slave_aw_t sys_sbm_aw_hi;
  logic sys_sbm_aw_valid_hi;
  logic sys_sbm_aw_ready_hi;
  sys_slave_aw_t sys_uc_aw_hi;
  logic sys_uc_aw_valid_hi;
  logic sys_uc_aw_ready_hi;
  sys_slave_w_t sys_sbm_w_hi;
  logic sys_sbm_w_valid_hi;
  logic sys_sbm_w_ready_hi;
  sys_slave_w_t sys_uc_w_hi;
  logic sys_uc_w_valid_hi;
  logic sys_uc_w_ready_hi;

  sys_slave_b_t sys_b_hi;
  logic sys_b_valid_hi;
  logic sys_b_ready_hi;
  sys_slave_r_t sys_r_hi;
  logic sys_r_valid_hi;
  logic sys_r_ready_hi;
  logic [1:0] return_credit_hi;

  logic [1:0] sys_b_arb_req;
  logic [1:0] sys_b_arb_grant;
  logic sys_b_arb_winner;

  uncacheable_fcc #(.NumNeighP(NumNeighP)) u_fcc (
    .fcc_o(uc_to_neigh_fcc_o),
    .fcc_target_o(uc_to_neigh_fcc_target_o),
    .esr_enables_i(esr_enables_i.fcc),
    .esr_wdata_i(esr_wdata_i)
  );

  uncacheable_flb #(.NumNeighP(NumNeighP)) u_flb (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .esr_enables_i(esr_enables_i.barriers),
    .esr_wdata_i(esr_wdata_i),
    .esr_barrier_values_o(esr_values_o.barriers),
    .flb_neigh_l2_req_valid_i(flb_neigh_l2_req_valid_i),
    .flb_neigh_l2_req_data_i(flb_neigh_l2_req_data_i),
    .flb_l2_neigh_resp_valid_o(flb_l2_neigh_resp_valid_o),
    .flb_l2_neigh_resp_data_o(flb_l2_neigh_resp_data_o)
  );

  uncacheable_noc #(.NumPortsP(NumPortsP)) u_noc (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .shire_id_i(shire_id_i),
    .uc_config_i(esr_uc_config_i),
    .neigh_uc_req_ready_o(neigh_uc_req_ready_o),
    .neigh_uc_req_valid_i(neigh_uc_req_valid_i),
    .neigh_uc_req_info_i(neigh_uc_req_info_i),
    .neigh_uc_min_rsp_ready_i(neigh_uc_min_rsp_ready_i),
    .neigh_uc_min_rsp_valid_o(neigh_uc_min_rsp_valid_o),
    .neigh_uc_min_rsp_info_o(neigh_uc_min_rsp_info_o),
    .credit_i(credit_hi_uc),
    .credit_return_o(return_credit_hi_uc),
    .from_sys_aw_i(uc_sys_aw_hi),
    .from_sys_aw_valid_i(uc_sys_aw_valid_hi),
    .from_sys_aw_ready_o(uc_sys_aw_ready_hi),
    .from_sys_w_i(uc_sys_w_hi),
    .from_sys_w_valid_i(uc_sys_w_valid_hi),
    .from_sys_w_ready_o(uc_sys_w_ready_hi),
    .from_sys_b_o(uc_sys_b_hi),
    .from_sys_b_valid_o(uc_sys_b_valid_hi),
    .from_sys_b_ready_i(uc_sys_b_ready_hi),
    .l3_enabled_i(l3_enabled_i),
    .remote_scp_enabled_i(remote_scp_enabled_i),
    .to_l3_ar_o(uc_to_l3_ar_hi),
    .to_l3_ar_valid_o(uc_to_l3_ar_valid_hi),
    .to_l3_ar_ready_i(uc_to_l3_ar_ready_hi),
    .to_l3_aw_o(uc_to_l3_aw_hi),
    .to_l3_aw_valid_o(uc_to_l3_aw_valid_hi),
    .to_l3_aw_ready_i(uc_to_l3_aw_ready_hi),
    .to_l3_w_o(uc_to_l3_w_hi),
    .to_l3_w_valid_o(uc_to_l3_w_valid_hi),
    .to_l3_w_ready_i(uc_to_l3_w_ready_hi),
    .to_l3_b_i(uc_to_l3_b_hi),
    .to_l3_b_valid_i(uc_to_l3_b_valid_hi),
    .to_l3_b_ready_o(uc_to_l3_b_ready_hi),
    .to_l3_r_i(uc_to_l3_r_hi),
    .to_l3_r_valid_i(uc_to_l3_r_valid_hi),
    .to_l3_r_ready_o(uc_to_l3_r_ready_hi),
    .to_sys_ar_o(uc_to_sys_ar_hi),
    .to_sys_ar_valid_o(uc_to_sys_ar_valid_hi),
    .to_sys_ar_ready_i(uc_to_sys_ar_ready_hi),
    .to_sys_aw_o(uc_to_sys_aw_hi),
    .to_sys_aw_valid_o(uc_to_sys_aw_valid_hi),
    .to_sys_aw_ready_i(uc_to_sys_aw_ready_hi),
    .to_sys_w_o(uc_to_sys_w_hi),
    .to_sys_w_valid_o(uc_to_sys_w_valid_hi),
    .to_sys_w_ready_i(uc_to_sys_w_ready_hi),
    .to_sys_b_i(uc_to_sys_b_hi),
    .to_sys_b_valid_i(uc_to_sys_b_valid_hi),
    .to_sys_b_ready_o(uc_to_sys_b_ready_hi),
    .to_sys_r_i(uc_to_sys_r_hi),
    .to_sys_r_valid_i(uc_to_sys_r_valid_hi),
    .to_sys_r_ready_o(uc_to_sys_r_ready_hi)
  );

  uncached_hi_voltage_cross u_hi_voltage_cross (
    .clk_hv_i(clk_i),
    .rst_hv_ni(rst_ni),
    .clk_lv_i(noc_clk_i),
    .rst_lv_ni(noc_rst_ni),
    .to_axi_l3_ar_i(uc_to_l3_ar_hi),
    .to_axi_l3_ar_valid_i(uc_to_l3_ar_valid_hi),
    .to_axi_l3_ar_ready_o(uc_to_l3_ar_ready_hi),
    .to_axi_l3_aw_i(uc_to_l3_aw_hi),
    .to_axi_l3_aw_valid_i(uc_to_l3_aw_valid_hi),
    .to_axi_l3_aw_ready_o(uc_to_l3_aw_ready_hi),
    .to_axi_l3_w_i(uc_to_l3_w_hi),
    .to_axi_l3_w_valid_i(uc_to_l3_w_valid_hi),
    .to_axi_l3_w_ready_o(uc_to_l3_w_ready_hi),
    .to_axi_l3_ar_lo_o(uc_to_l3_ar_lo),
    .to_axi_l3_ar_valid_lo_o(uc_to_l3_ar_valid_lo),
    .to_axi_l3_ar_ready_lo_i(uc_to_l3_ar_ready_lo),
    .to_axi_l3_aw_lo_o(uc_to_l3_aw_lo),
    .to_axi_l3_aw_valid_lo_o(uc_to_l3_aw_valid_lo),
    .to_axi_l3_aw_ready_lo_i(uc_to_l3_aw_ready_lo),
    .to_axi_l3_w_lo_o(uc_to_l3_w_lo),
    .to_axi_l3_w_valid_lo_o(uc_to_l3_w_valid_lo),
    .to_axi_l3_w_ready_lo_i(uc_to_l3_w_ready_lo),
    .to_axi_sys_ar_i(uc_to_sys_ar_hi),
    .to_axi_sys_ar_valid_i(uc_to_sys_ar_valid_hi),
    .to_axi_sys_ar_ready_o(uc_to_sys_ar_ready_hi),
    .to_axi_sys_aw_i(uc_to_sys_aw_hi),
    .to_axi_sys_aw_valid_i(uc_to_sys_aw_valid_hi),
    .to_axi_sys_aw_ready_o(uc_to_sys_aw_ready_hi),
    .to_axi_sys_w_i(uc_to_sys_w_hi),
    .to_axi_sys_w_valid_i(uc_to_sys_w_valid_hi),
    .to_axi_sys_w_ready_o(uc_to_sys_w_ready_hi),
    .to_axi_sys_ar_lo_o(uc_to_sys_ar_lo),
    .to_axi_sys_ar_valid_lo_o(uc_to_sys_ar_valid_lo),
    .to_axi_sys_ar_ready_lo_i(uc_to_sys_ar_ready_lo),
    .to_axi_sys_aw_lo_o(uc_to_sys_aw_lo),
    .to_axi_sys_aw_valid_lo_o(uc_to_sys_aw_valid_lo),
    .to_axi_sys_aw_ready_lo_i(uc_to_sys_aw_ready_lo),
    .to_axi_sys_w_lo_o(uc_to_sys_w_lo),
    .to_axi_sys_w_valid_lo_o(uc_to_sys_w_valid_lo),
    .to_axi_sys_w_ready_lo_i(uc_to_sys_w_ready_lo),
    .from_axi_sys_r_i(sys_r_hi),
    .from_axi_sys_r_valid_i(sys_r_valid_hi),
    .from_axi_sys_r_ready_o(sys_r_ready_hi),
    .from_axi_sys_r_lo_o(sys_axi_r_o),
    .from_axi_sys_r_valid_lo_o(sys_axi_r_valid_o),
    .from_axi_sys_r_ready_lo_i(sys_axi_r_ready_i),
    .from_axi_sys_b_i(sys_b_hi),
    .from_axi_sys_credit_i(return_credit_hi),
    .from_axi_sys_b_valid_i(sys_b_valid_hi),
    .from_axi_sys_b_ready_o(sys_b_ready_hi),
    .from_axi_sys_b_lo_o(sys_axi_b_o),
    .from_axi_sys_credit_lo_o(sys_axi_aw_credit_o),
    .from_axi_sys_b_valid_lo_o(sys_axi_b_valid_o),
    .from_axi_sys_b_ready_lo_i(sys_axi_b_ready_i),
    .dft_hv_i(dft_hv_i),
    .dft_lv_i(dft_lv_i)
  );

  uncached_lo_voltage_cross u_lo_voltage_cross (
    .clk_lv_i(noc_clk_i),
    .rst_lv_ni(noc_rst_ni),
    .clk_hv_i(clk_i),
    .rst_hv_ni(rst_ni),
    .to_axi_l3_b_o(uc_to_l3_b_hi),
    .to_axi_l3_b_valid_o(uc_to_l3_b_valid_hi),
    .to_axi_l3_b_ready_i(uc_to_l3_b_ready_hi),
    .to_axi_l3_r_o(uc_to_l3_r_hi),
    .to_axi_l3_r_valid_o(uc_to_l3_r_valid_hi),
    .to_axi_l3_r_ready_i(uc_to_l3_r_ready_hi),
    .to_axi_l3_b_lo_i(uc_to_l3_b_lo),
    .to_axi_l3_b_valid_lo_i(uc_to_l3_b_valid_lo),
    .to_axi_l3_b_ready_lo_o(uc_to_l3_b_ready_lo),
    .to_axi_l3_r_lo_i(uc_to_l3_r_lo),
    .to_axi_l3_r_valid_lo_i(uc_to_l3_r_valid_lo),
    .to_axi_l3_r_ready_lo_o(uc_to_l3_r_ready_lo),
    .to_axi_sys_b_o(uc_to_sys_b_hi),
    .to_axi_sys_b_valid_o(uc_to_sys_b_valid_hi),
    .to_axi_sys_b_ready_i(uc_to_sys_b_ready_hi),
    .to_axi_sys_r_o(uc_to_sys_r_hi),
    .to_axi_sys_r_valid_o(uc_to_sys_r_valid_hi),
    .to_axi_sys_r_ready_i(uc_to_sys_r_ready_hi),
    .to_axi_sys_b_lo_i(uc_to_sys_b_lo),
    .to_axi_sys_b_valid_lo_i(uc_to_sys_b_valid_lo),
    .to_axi_sys_b_ready_lo_o(uc_to_sys_b_ready_lo),
    .to_axi_sys_r_lo_i(uc_to_sys_r_lo),
    .to_axi_sys_r_valid_lo_i(uc_to_sys_r_valid_lo),
    .to_axi_sys_r_ready_lo_o(uc_to_sys_r_ready_lo),
    .from_axi_sys_ar_o(sys_ar_hi),
    .from_axi_sys_ar_valid_o(sys_ar_valid_hi),
    .from_axi_sys_ar_ready_i(sys_ar_ready_hi),
    .from_axi_sys_uc_aw_o(sys_uc_aw_hi),
    .from_axi_sys_uc_aw_valid_o(sys_uc_aw_valid_hi),
    .from_axi_sys_uc_aw_ready_i(sys_uc_aw_ready_hi),
    .from_axi_sys_sbm_aw_o(sys_sbm_aw_hi),
    .from_axi_sys_sbm_aw_valid_o(sys_sbm_aw_valid_hi),
    .from_axi_sys_sbm_aw_ready_i(sys_sbm_aw_ready_hi),
    .from_axi_sys_uc_w_o(sys_uc_w_hi),
    .from_axi_sys_uc_w_valid_o(sys_uc_w_valid_hi),
    .from_axi_sys_uc_w_ready_i(sys_uc_w_ready_hi),
    .from_axi_sys_sbm_w_o(sys_sbm_w_hi),
    .from_axi_sys_sbm_w_valid_o(sys_sbm_w_valid_hi),
    .from_axi_sys_sbm_w_ready_i(sys_sbm_w_ready_hi),
    .from_axi_sys_ar_lo_i(sys_axi_ar_i),
    .from_axi_sys_ar_valid_lo_i(sys_axi_ar_valid_i),
    .from_axi_sys_ar_ready_lo_o(sys_axi_ar_ready_o),
    .from_axi_sys_aw_lo_i(sys_axi_aw_i),
    .from_axi_sys_aw_valid_lo_i(sys_axi_aw_valid_i),
    .from_axi_sys_aw_vcvalid_lo_i(sys_axi_aw_vcvalid_i),
    .from_axi_sys_aw_ready_lo_o(sys_axi_aw_ready_o),
    .from_axi_sys_w_lo_i(sys_axi_w_i),
    .from_axi_sys_w_valid_lo_i(sys_axi_w_valid_i),
    .from_axi_sys_w_vcvalid_lo_i(sys_axi_w_vcvalid_i),
    .from_axi_sys_w_ready_lo_o(sys_axi_w_ready_o),
    .dft_lv_i(dft_lv_i),
    .dft_hv_i(dft_hv_i)
  );

  assign to_l3_ar_o = uc_to_l3_ar_lo;
  assign to_l3_ar_valid_o = uc_to_l3_ar_valid_lo;
  assign uc_to_l3_ar_ready_lo = to_l3_ar_ready_i;
  assign to_l3_aw_o = uc_to_l3_aw_lo;
  assign to_l3_aw_valid_o = uc_to_l3_aw_valid_lo;
  assign uc_to_l3_aw_ready_lo = to_l3_aw_ready_i;
  assign to_l3_w_o = uc_to_l3_w_lo;
  assign to_l3_w_valid_o = uc_to_l3_w_valid_lo;
  assign uc_to_l3_w_ready_lo = to_l3_w_ready_i;
  assign uc_to_l3_b_lo = to_l3_b_i;
  assign uc_to_l3_b_valid_lo = to_l3_b_valid_i;
  assign to_l3_b_ready_o = uc_to_l3_b_ready_lo;
  assign uc_to_l3_r_lo = to_l3_r_i;
  assign uc_to_l3_r_valid_lo = to_l3_r_valid_i;
  assign to_l3_r_ready_o = uc_to_l3_r_ready_lo;

  assign to_sys_ar_o = uc_to_sys_ar_lo;
  assign to_sys_ar_valid_o = uc_to_sys_ar_valid_lo;
  assign uc_to_sys_ar_ready_lo = to_sys_ar_ready_i;
  assign to_sys_aw_o = uc_to_sys_aw_lo;
  assign to_sys_aw_valid_o = uc_to_sys_aw_valid_lo;
  assign uc_to_sys_aw_ready_lo = to_sys_aw_ready_i;
  assign to_sys_w_o = uc_to_sys_w_lo;
  assign to_sys_w_valid_o = uc_to_sys_w_valid_lo;
  assign uc_to_sys_w_ready_lo = to_sys_w_ready_i;
  assign uc_to_sys_b_lo = to_sys_b_i;
  assign uc_to_sys_b_valid_lo = to_sys_b_valid_i;
  assign to_sys_b_ready_o = uc_to_sys_b_ready_lo;
  assign uc_to_sys_r_lo = to_sys_r_i;
  assign uc_to_sys_r_valid_lo = to_sys_r_valid_i;
  assign to_sys_r_ready_o = uc_to_sys_r_ready_lo;

  assign sbm_sys_axi_ar_o = sys_ar_hi;
  assign sbm_sys_axi_aw_o = sys_sbm_aw_hi;
  assign sbm_sys_axi_w_o = sys_sbm_w_hi;
  assign sys_ar_ready_hi = sbm_sys_axi_ar_ready_i;
  assign sys_sbm_aw_ready_hi = sbm_sys_axi_aw_ready_i;
  assign sys_sbm_w_ready_hi = sbm_sys_axi_w_ready_i;
  assign sbm_enable_read_o = sys_ar_valid_hi;
  assign sbm_enable_write_o = sys_sbm_aw_valid_hi && sys_sbm_w_valid_hi;

  assign credit_hi_uc = sys_uc_aw_valid_hi;
  assign return_credit_hi[SbmSysSlaveVsiId] = sbm_write_credit_return_i;
  assign return_credit_hi[UcSysSlaveVsiId] = return_credit_hi_uc;

  assign uc_sys_aw_hi = sys_uc_aw_hi;
  assign uc_sys_aw_valid_hi = sys_uc_aw_valid_hi;
  assign sys_uc_aw_ready_hi = uc_sys_aw_ready_hi;
  assign uc_sys_w_hi = sys_uc_w_hi;
  assign uc_sys_w_valid_hi = sys_uc_w_valid_hi;
  assign sys_uc_w_ready_hi = uc_sys_w_ready_hi;

  assign sys_r_hi = sbm_sys_axi_r_i;
  assign sys_r_valid_hi = sbm_sys_axi_r_valid_i;
  assign sbm_sys_axi_r_ready_o = sys_r_ready_hi;

  assign sys_b_arb_req[SbmSysSlaveVsiId] = sbm_sys_axi_b_valid_i;
  assign sys_b_arb_req[UcSysSlaveVsiId] = uc_sys_b_valid_hi;
  assign sys_b_valid_hi = |(sys_b_arb_grant & sys_b_arb_req);
  assign sys_b_hi = sys_b_arb_winner ? uc_sys_b_hi : sbm_sys_axi_b_i;
  assign sbm_sys_axi_b_ready_o = sys_b_arb_grant[SbmSysSlaveVsiId] && sys_b_ready_hi;
  assign uc_sys_b_ready_hi = sys_b_arb_grant[UcSysSlaveVsiId] && sys_b_ready_hi;

  uncached_arb_lru_grant #(.NumClients(2)) u_sys_b_arb (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .bid_i(sys_b_arb_req),
    .stall_i(!sys_b_ready_hi),
    .grant_o(sys_b_arb_grant),
    .winner_o(sys_b_arb_winner)
  );

endmodule : uncacheable
