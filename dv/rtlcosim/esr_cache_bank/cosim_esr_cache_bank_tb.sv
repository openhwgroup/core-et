// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_esr_cache_bank_tb (
  input  logic        clk_i,
  input  logic        rst_c_ni,
  input  logic        rst_w_ni,
  input  logic        rst_d_ni,
  input  logic        test_en_i,
  input  logic [11:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,
  input  logic [63:0] status_seed_i,

  output logic        orig_apb_pready_o,
  output logic [63:0] orig_apb_prdata_o,
  output logic        orig_apb_pslverr_o,
  output logic        new_apb_pready_o,
  output logic [63:0] new_apb_prdata_o,
  output logic        new_apb_pslverr_o,

  output logic [63:0] orig_ctl0_o,
  output logic [63:0] orig_ctl1_o,
  output logic [63:0] orig_ctl2_o,
  output logic [63:0] orig_ctl3_o,
  output logic [63:0] orig_ctl4_o,
  output logic [63:0] orig_ctl5_o,
  output logic [63:0] orig_ctl6_o,
  output logic [63:0] orig_ctl7_o,
  output logic [63:0] new_ctl0_o,
  output logic [63:0] new_ctl1_o,
  output logic [63:0] new_ctl2_o,
  output logic [63:0] new_ctl3_o,
  output logic [63:0] new_ctl4_o,
  output logic [63:0] new_ctl5_o,
  output logic [63:0] new_ctl6_o,
  output logic [63:0] new_ctl7_o,
  output logic [63:0] orig_swizzle_o,
  output logic [63:0] new_swizzle_o,
  output logic [7:0]  orig_eco_o,
  output logic [7:0]  new_eco_o,
  output logic        ctl_fields_match_o
);
  sc_bank_esr_ctl_t orig_ctl;
  sc_bank_esr_status_t orig_status;
  esr_sc_l3_shire_swizzle_ctl_t orig_swizzle;
  logic [7:0] orig_eco;

  shirecache_pkg::bank_esr_ctl_t new_ctl;
  shirecache_pkg::bank_esr_status_t new_status;
  shire_esr_pkg::esr_sc_l3_shire_swizzle_ctl_t new_swizzle;
  logic [7:0] new_eco;

  always_comb begin
    orig_status = '0;
    orig_status.pipe_idx_cop_sm.ctl = status_seed_i ^ 64'h0001_0001_0001_0001;
    orig_status.pipe_idx_cop_sm.physical_index = status_seed_i ^ 64'h0002_0002_0002_0002;
    orig_status.pipe_idx_cop_sm.data0 = status_seed_i ^ 64'h0003_0003_0003_0003;
    orig_status.pipe_idx_cop_sm.data1 = status_seed_i ^ 64'h0004_0004_0004_0004;
    orig_status.pipe_idx_cop_sm.ecc = status_seed_i ^ 64'h0005_0005_0005_0005;
    orig_status.err_log.info = status_seed_i ^ 64'h1000_0000_0000_0000;
    orig_status.err_log.address = status_seed_i ^ 64'h2000_0000_0000_0000;
    orig_status.err_log.reqq_debug0 = status_seed_i ^ 64'h3000_0000_0000_0000;
    orig_status.err_log.reqq_debug1 = status_seed_i ^ 64'h4000_0000_0000_0000;
    orig_status.err_log.reqq_debug2 = status_seed_i ^ 64'h5000_0000_0000_0000;
    orig_status.err_log.reqq_debug3 = status_seed_i ^ 64'h6000_0000_0000_0000;
    orig_status.sbe_dbe_counts = status_seed_i ^ 64'h7000_0000_0000_0000;
    orig_status.perfmon.ctl_status = status_seed_i ^ 64'h8000_0000_0000_0000;
    orig_status.perfmon.cyc_cntr = status_seed_i ^ 64'h9000_0000_0000_0000;
    orig_status.perfmon.p0_cntr = status_seed_i ^ 64'ha000_0000_0000_0000;
    orig_status.perfmon.p1_cntr = status_seed_i ^ 64'hb000_0000_0000_0000;
    orig_status.perfmon.p0_qual = status_seed_i ^ 64'hc000_0000_0000_0000;
    orig_status.perfmon.p1_qual = status_seed_i ^ 64'hd000_0000_0000_0000;

    new_status = '0;
    new_status.pipe_idx_cop_sm.ctl = orig_status.pipe_idx_cop_sm.ctl;
    new_status.pipe_idx_cop_sm.physical_index = orig_status.pipe_idx_cop_sm.physical_index;
    new_status.pipe_idx_cop_sm.data0 = orig_status.pipe_idx_cop_sm.data0;
    new_status.pipe_idx_cop_sm.data1 = orig_status.pipe_idx_cop_sm.data1;
    new_status.pipe_idx_cop_sm.ecc = orig_status.pipe_idx_cop_sm.ecc;
    new_status.err_log.info = orig_status.err_log.info;
    new_status.err_log.address = orig_status.err_log.address;
    new_status.err_log.reqq_debug0 = orig_status.err_log.reqq_debug0;
    new_status.err_log.reqq_debug1 = orig_status.err_log.reqq_debug1;
    new_status.err_log.reqq_debug2 = orig_status.err_log.reqq_debug2;
    new_status.err_log.reqq_debug3 = orig_status.err_log.reqq_debug3;
    new_status.sbe_dbe_counts = orig_status.sbe_dbe_counts;
    new_status.perfmon.ctl_status = orig_status.perfmon.ctl_status;
    new_status.perfmon.cyc_cntr = orig_status.perfmon.cyc_cntr;
    new_status.perfmon.p0_cntr = orig_status.perfmon.p0_cntr;
    new_status.perfmon.p1_cntr = orig_status.perfmon.p1_cntr;
    new_status.perfmon.p0_qual = orig_status.perfmon.p0_qual;
    new_status.perfmon.p1_qual = orig_status.perfmon.p1_qual;
  end

  esr_cache_bank_orig #(
    .AD_WIDTH(12),
    .SETS_PER_SUB_BANK(shirecache_pkg::SetsPerSubBank),
    .NUM_REQQ_ENTRIES(shirecache_pkg::ReqqDepth)
  ) u_orig (
    .clock                         (clk_i),
    .reset_c                       (!rst_c_ni),
    .reset_w                       (!rst_w_ni),
    .reset_d                       (!rst_d_ni),
    .test_en                       (test_en_i),
    .esr_ctl                       (orig_ctl),
    .esr_status                    (orig_status),
    .esr_sc_l3_shire_swizzle_ctl   (orig_swizzle),
    .esr_sc_eco_ctl                (orig_eco),
    .apb_paddr                     (apb_paddr_i),
    .apb_pwrite                    (apb_pwrite_i),
    .apb_psel                      (apb_psel_i),
    .apb_penable                   (apb_penable_i),
    .apb_pwdata                    (apb_pwdata_i),
    .apb_pready                    (orig_apb_pready_o),
    .apb_prdata                    (orig_apb_prdata_o),
    .apb_pslverr                   (orig_apb_pslverr_o)
  );

  esr_cache_bank u_new (
    .clk_i                         (clk_i),
    .rst_c_ni                      (rst_c_ni),
    .rst_w_ni                      (rst_w_ni),
    .rst_d_ni                      (rst_d_ni),
    .test_en_i                     (test_en_i),
    .esr_ctl_o                     (new_ctl),
    .esr_status_i                  (new_status),
    .esr_sc_l3_shire_swizzle_ctl_o (new_swizzle),
    .esr_sc_eco_ctl_o              (new_eco),
    .apb_paddr_i                   (apb_paddr_i),
    .apb_pwrite_i                  (apb_pwrite_i),
    .apb_psel_i                    (apb_psel_i),
    .apb_penable_i                 (apb_penable_i),
    .apb_pwdata_i                  (apb_pwdata_i),
    .apb_pready_o                  (new_apb_pready_o),
    .apb_prdata_o                  (new_apb_prdata_o),
    .apb_pslverr_o                 (new_apb_pslverr_o)
  );

  logic [511:0] orig_ctl_pad;
  logic [511:0] new_ctl_pad;
  logic [$bits(sc_bank_esr_ctl_t)-1:0] orig_ctl_canon;
  logic [$bits(shirecache_pkg::bank_esr_ctl_t)-1:0] new_ctl_canon;

  assign orig_ctl_canon = {
      orig_ctl.esr_sc_clk_gate_disable,
      orig_ctl.esr_sc_axi_qos,
      orig_ctl.esr_sc_cbuf_enable,
      orig_ctl.esr_sc_remote_l3_enable,
      orig_ctl.esr_sc_remote_scp_enable,
      orig_ctl.esr_sc_l2_bypass,
      orig_ctl.esr_sc_l3_bypass,
      orig_ctl.esr_sc_num_l3_reqq_entries,
      orig_ctl.esr_sc_reqq_no_link_list,
      orig_ctl.esr_sc_ecc_scrub_enable,
      orig_ctl.esr_sc_l3_yield_priority,
      orig_ctl.esr_sc_idx_cop_sm_ctl_user_en,
      orig_ctl.esr_sc_ram_deep_sleep,
      orig_ctl.esr_sc_ram_shut_down,
      orig_ctl.esr_sc_ram_delay,
      orig_ctl.esr_sc_l2_rbuf_enable,
      orig_ctl.esr_sc_scp_rbuf_enable,
      orig_ctl.esr_sc_zero_state_enable,
      orig_ctl.esr_sc_allow_only_1_req_per_sub_bank,
      orig_ctl.esr_sc_allow_only_1_req_per_bank,
      orig_ctl.esr_sc_cbuf_entry_enable,
      orig_ctl.esr_sc_two_shire_aliasing_use_shire_lsb,
      orig_ctl.esr_sc_all_shire_aliasing,
      orig_ctl.esr_sc_sub_bank_sel_b2,
      orig_ctl.esr_sc_sub_bank_sel_b1,
      orig_ctl.esr_sc_sub_bank_sel_b0,
      orig_ctl.esr_sc_bank_sel_b2,
      orig_ctl.esr_sc_bank_sel_b1,
      orig_ctl.esr_sc_bank_sel_b0,
      orig_ctl.esr_sc_shire_sel_b5,
      orig_ctl.esr_sc_shire_sel_b4,
      orig_ctl.esr_sc_shire_sel_b3,
      orig_ctl.esr_sc_shire_sel_b2,
      orig_ctl.esr_sc_shire_sel_b1,
      orig_ctl.esr_sc_shire_sel_b0,
      orig_ctl.esr_sc_l2_set_base,
      orig_ctl.esr_sc_l2_set_size,
      orig_ctl.esr_sc_l2_set_mask,
      orig_ctl.esr_sc_l2_tag_mask,
      orig_ctl.esr_sc_l3_set_base,
      orig_ctl.esr_sc_l3_set_size,
      orig_ctl.esr_sc_l3_set_mask,
      orig_ctl.esr_sc_l3_tag_mask,
      orig_ctl.esr_sc_scp_set_base,
      orig_ctl.esr_sc_scp_set_size,
      orig_ctl.esr_sc_scp_set_mask,
      orig_ctl.esr_sc_scp_tag_mask,
      orig_ctl.esr_sc_err_rsp_enable,
      orig_ctl.esr_sc_err_interrupt_enable,
      orig_ctl.esr_sc_trace_filter_address_enable,
      orig_ctl.esr_sc_trace_filter_address_value,
      orig_ctl.esr_sc_trace_filter_reqq_id_enable,
      orig_ctl.esr_sc_trace_filter_reqq_id,
      orig_ctl.esr_sc_trace_filter_port_enable,
      orig_ctl.esr_sc_trace_filter_port,
      orig_ctl.esr_sc_trace_filter_source_enable,
      orig_ctl.esr_sc_trace_filter_source,
      orig_ctl.esr_sc_trace_filter_l2_enable,
      orig_ctl.esr_sc_trace_filter_l3_enable,
      orig_ctl.esr_sc_trace_filter_fsm_enable,
      orig_ctl.esr_sc_trace_type_hot_enable,
      orig_ctl.wr_en_reqq_ctl,
      orig_ctl.wr_en_idx_cop_sm_ctl,
      orig_ctl.wr_en_idx_cop_sm_physical_index,
      orig_ctl.wr_en_idx_cop_sm_data0,
      orig_ctl.wr_en_idx_cop_sm_data1,
      orig_ctl.wr_en_idx_cop_sm_ecc,
      orig_ctl.wr_en_err_log_info,
      orig_ctl.wr_en_sbe_dbe_counts,
      orig_ctl.wr_en_reqq_debug_ctl,
      orig_ctl.wr_en_perfmon_ctl_status,
      orig_ctl.wr_en_perfmon_cyc_cntr,
      orig_ctl.wr_en_perfmon_p0_cntr,
      orig_ctl.wr_en_perfmon_p1_cntr,
      orig_ctl.wr_en_perfmon_p0_qual,
      orig_ctl.wr_en_perfmon_p1_qual,
      orig_ctl.wr_data,
      orig_ctl.access_in_flight
  };
  assign new_ctl_canon = {
      new_ctl.esr_sc_clk_gate_disable,
      new_ctl.esr_sc_axi_qos,
      new_ctl.esr_sc_cbuf_enable,
      new_ctl.esr_sc_remote_l3_enable,
      new_ctl.esr_sc_remote_scp_enable,
      new_ctl.esr_sc_l2_bypass,
      new_ctl.esr_sc_l3_bypass,
      new_ctl.esr_sc_num_l3_reqq_entries,
      new_ctl.esr_sc_reqq_no_link_list,
      new_ctl.esr_sc_ecc_scrub_enable,
      new_ctl.esr_sc_l3_yield_priority,
      new_ctl.esr_sc_idx_cop_sm_ctl_user_en,
      new_ctl.esr_sc_ram_deep_sleep,
      new_ctl.esr_sc_ram_shut_down,
      new_ctl.esr_sc_ram_delay,
      new_ctl.esr_sc_l2_rbuf_enable,
      new_ctl.esr_sc_scp_rbuf_enable,
      new_ctl.esr_sc_zero_state_enable,
      new_ctl.esr_sc_allow_only_1_req_per_sub_bank,
      new_ctl.esr_sc_allow_only_1_req_per_bank,
      new_ctl.esr_sc_cbuf_entry_enable,
      new_ctl.esr_sc_two_shire_aliasing_use_shire_lsb,
      new_ctl.esr_sc_all_shire_aliasing,
      new_ctl.esr_sc_sub_bank_sel_b2,
      new_ctl.esr_sc_sub_bank_sel_b1,
      new_ctl.esr_sc_sub_bank_sel_b0,
      new_ctl.esr_sc_bank_sel_b2,
      new_ctl.esr_sc_bank_sel_b1,
      new_ctl.esr_sc_bank_sel_b0,
      new_ctl.esr_sc_shire_sel_b5,
      new_ctl.esr_sc_shire_sel_b4,
      new_ctl.esr_sc_shire_sel_b3,
      new_ctl.esr_sc_shire_sel_b2,
      new_ctl.esr_sc_shire_sel_b1,
      new_ctl.esr_sc_shire_sel_b0,
      new_ctl.esr_sc_l2_set_base,
      new_ctl.esr_sc_l2_set_size,
      new_ctl.esr_sc_l2_set_mask,
      new_ctl.esr_sc_l2_tag_mask,
      new_ctl.esr_sc_l3_set_base,
      new_ctl.esr_sc_l3_set_size,
      new_ctl.esr_sc_l3_set_mask,
      new_ctl.esr_sc_l3_tag_mask,
      new_ctl.esr_sc_scp_set_base,
      new_ctl.esr_sc_scp_set_size,
      new_ctl.esr_sc_scp_set_mask,
      new_ctl.esr_sc_scp_tag_mask,
      new_ctl.esr_sc_err_rsp_enable,
      new_ctl.esr_sc_err_interrupt_enable,
      new_ctl.esr_sc_trace_filter_address_enable,
      new_ctl.esr_sc_trace_filter_address_value,
      new_ctl.esr_sc_trace_filter_reqq_id_enable,
      new_ctl.esr_sc_trace_filter_reqq_id,
      new_ctl.esr_sc_trace_filter_port_enable,
      new_ctl.esr_sc_trace_filter_port,
      new_ctl.esr_sc_trace_filter_source_enable,
      new_ctl.esr_sc_trace_filter_source,
      new_ctl.esr_sc_trace_filter_l2_enable,
      new_ctl.esr_sc_trace_filter_l3_enable,
      new_ctl.esr_sc_trace_filter_fsm_enable,
      new_ctl.esr_sc_trace_type_hot_enable,
      new_ctl.wr_en_reqq_ctl,
      new_ctl.wr_en_idx_cop_sm_ctl,
      new_ctl.wr_en_idx_cop_sm_physical_index,
      new_ctl.wr_en_idx_cop_sm_data0,
      new_ctl.wr_en_idx_cop_sm_data1,
      new_ctl.wr_en_idx_cop_sm_ecc,
      new_ctl.wr_en_err_log_info,
      new_ctl.wr_en_sbe_dbe_counts,
      new_ctl.wr_en_reqq_debug_ctl,
      new_ctl.wr_en_perfmon_ctl_status,
      new_ctl.wr_en_perfmon_cyc_cntr,
      new_ctl.wr_en_perfmon_p0_cntr,
      new_ctl.wr_en_perfmon_p1_cntr,
      new_ctl.wr_en_perfmon_p0_qual,
      new_ctl.wr_en_perfmon_p1_qual,
      new_ctl.wr_data,
      new_ctl.access_in_flight
  };
  assign orig_ctl_pad = {{(512-$bits(orig_ctl_canon)){1'b0}}, orig_ctl_canon};
  assign new_ctl_pad = {{(512-$bits(new_ctl_canon)){1'b0}}, new_ctl_canon};
  assign orig_ctl0_o = orig_ctl_pad[0 +: 64];
  assign orig_ctl1_o = orig_ctl_pad[64 +: 64];
  assign orig_ctl2_o = orig_ctl_pad[128 +: 64];
  assign orig_ctl3_o = orig_ctl_pad[192 +: 64];
  assign orig_ctl4_o = orig_ctl_pad[256 +: 64];
  assign orig_ctl5_o = orig_ctl_pad[320 +: 64];
  assign orig_ctl6_o = orig_ctl_pad[384 +: 64];
  assign orig_ctl7_o = orig_ctl_pad[448 +: 64];
  assign new_ctl0_o = new_ctl_pad[0 +: 64];
  assign new_ctl1_o = new_ctl_pad[64 +: 64];
  assign new_ctl2_o = new_ctl_pad[128 +: 64];
  assign new_ctl3_o = new_ctl_pad[192 +: 64];
  assign new_ctl4_o = new_ctl_pad[256 +: 64];
  assign new_ctl5_o = new_ctl_pad[320 +: 64];
  assign new_ctl6_o = new_ctl_pad[384 +: 64];
  assign new_ctl7_o = new_ctl_pad[448 +: 64];

  assign orig_swizzle_o = {{(64-$bits(esr_sc_l3_shire_swizzle_ctl_t)){1'b0}}, orig_swizzle};
  assign new_swizzle_o = {{(64-$bits(shire_esr_pkg::esr_sc_l3_shire_swizzle_ctl_t)){1'b0}}, new_swizzle};
  assign orig_eco_o = orig_eco;
  assign new_eco_o = new_eco;

  assign ctl_fields_match_o =
      (orig_ctl.esr_sc_clk_gate_disable == new_ctl.esr_sc_clk_gate_disable) &
      (orig_ctl.esr_sc_axi_qos == new_ctl.esr_sc_axi_qos) &
      (orig_ctl.esr_sc_cbuf_enable == new_ctl.esr_sc_cbuf_enable) &
      (orig_ctl.esr_sc_remote_l3_enable == new_ctl.esr_sc_remote_l3_enable) &
      (orig_ctl.esr_sc_remote_scp_enable == new_ctl.esr_sc_remote_scp_enable) &
      (orig_ctl.esr_sc_l2_bypass == new_ctl.esr_sc_l2_bypass) &
      (orig_ctl.esr_sc_l3_bypass == new_ctl.esr_sc_l3_bypass) &
      (orig_ctl.esr_sc_num_l3_reqq_entries == new_ctl.esr_sc_num_l3_reqq_entries) &
      (orig_ctl.esr_sc_reqq_no_link_list == new_ctl.esr_sc_reqq_no_link_list) &
      (orig_ctl.esr_sc_ecc_scrub_enable == new_ctl.esr_sc_ecc_scrub_enable) &
      (orig_ctl.esr_sc_l3_yield_priority == new_ctl.esr_sc_l3_yield_priority) &
      (orig_ctl.esr_sc_idx_cop_sm_ctl_user_en == new_ctl.esr_sc_idx_cop_sm_ctl_user_en) &
      (orig_ctl.esr_sc_ram_deep_sleep == new_ctl.esr_sc_ram_deep_sleep) &
      (orig_ctl.esr_sc_ram_shut_down == new_ctl.esr_sc_ram_shut_down) &
      (orig_ctl.esr_sc_ram_delay == new_ctl.esr_sc_ram_delay) &
      (orig_ctl.esr_sc_l2_rbuf_enable == new_ctl.esr_sc_l2_rbuf_enable) &
      (orig_ctl.esr_sc_scp_rbuf_enable == new_ctl.esr_sc_scp_rbuf_enable) &
      (orig_ctl.esr_sc_zero_state_enable == new_ctl.esr_sc_zero_state_enable) &
      (orig_ctl.esr_sc_allow_only_1_req_per_sub_bank == new_ctl.esr_sc_allow_only_1_req_per_sub_bank) &
      (orig_ctl.esr_sc_allow_only_1_req_per_bank == new_ctl.esr_sc_allow_only_1_req_per_bank) &
      (orig_ctl.esr_sc_cbuf_entry_enable == new_ctl.esr_sc_cbuf_entry_enable) &
      (orig_ctl.esr_sc_two_shire_aliasing_use_shire_lsb == new_ctl.esr_sc_two_shire_aliasing_use_shire_lsb) &
      (orig_ctl.esr_sc_all_shire_aliasing == new_ctl.esr_sc_all_shire_aliasing) &
      (orig_ctl.esr_sc_sub_bank_sel_b2 == new_ctl.esr_sc_sub_bank_sel_b2) &
      (orig_ctl.esr_sc_sub_bank_sel_b1 == new_ctl.esr_sc_sub_bank_sel_b1) &
      (orig_ctl.esr_sc_sub_bank_sel_b0 == new_ctl.esr_sc_sub_bank_sel_b0) &
      (orig_ctl.esr_sc_bank_sel_b2 == new_ctl.esr_sc_bank_sel_b2) &
      (orig_ctl.esr_sc_bank_sel_b1 == new_ctl.esr_sc_bank_sel_b1) &
      (orig_ctl.esr_sc_bank_sel_b0 == new_ctl.esr_sc_bank_sel_b0) &
      (orig_ctl.esr_sc_shire_sel_b5 == new_ctl.esr_sc_shire_sel_b5) &
      (orig_ctl.esr_sc_shire_sel_b4 == new_ctl.esr_sc_shire_sel_b4) &
      (orig_ctl.esr_sc_shire_sel_b3 == new_ctl.esr_sc_shire_sel_b3) &
      (orig_ctl.esr_sc_shire_sel_b2 == new_ctl.esr_sc_shire_sel_b2) &
      (orig_ctl.esr_sc_shire_sel_b1 == new_ctl.esr_sc_shire_sel_b1) &
      (orig_ctl.esr_sc_shire_sel_b0 == new_ctl.esr_sc_shire_sel_b0) &
      (orig_ctl.esr_sc_l2_set_base == new_ctl.esr_sc_l2_set_base) &
      (orig_ctl.esr_sc_l2_set_size == new_ctl.esr_sc_l2_set_size) &
      (orig_ctl.esr_sc_l2_set_mask == new_ctl.esr_sc_l2_set_mask) &
      (orig_ctl.esr_sc_l2_tag_mask == new_ctl.esr_sc_l2_tag_mask) &
      (orig_ctl.esr_sc_l3_set_base == new_ctl.esr_sc_l3_set_base) &
      (orig_ctl.esr_sc_l3_set_size == new_ctl.esr_sc_l3_set_size) &
      (orig_ctl.esr_sc_l3_set_mask == new_ctl.esr_sc_l3_set_mask) &
      (orig_ctl.esr_sc_l3_tag_mask == new_ctl.esr_sc_l3_tag_mask) &
      (orig_ctl.esr_sc_scp_set_base == new_ctl.esr_sc_scp_set_base) &
      (orig_ctl.esr_sc_scp_set_size == new_ctl.esr_sc_scp_set_size) &
      (orig_ctl.esr_sc_scp_set_mask == new_ctl.esr_sc_scp_set_mask) &
      (orig_ctl.esr_sc_scp_tag_mask == new_ctl.esr_sc_scp_tag_mask) &
      (orig_ctl.esr_sc_err_rsp_enable == new_ctl.esr_sc_err_rsp_enable) &
      (orig_ctl.esr_sc_err_interrupt_enable == new_ctl.esr_sc_err_interrupt_enable) &
      (orig_ctl.esr_sc_trace_filter_address_enable == new_ctl.esr_sc_trace_filter_address_enable) &
      (orig_ctl.esr_sc_trace_filter_address_value == new_ctl.esr_sc_trace_filter_address_value) &
      (orig_ctl.esr_sc_trace_filter_reqq_id_enable == new_ctl.esr_sc_trace_filter_reqq_id_enable) &
      (orig_ctl.esr_sc_trace_filter_reqq_id == new_ctl.esr_sc_trace_filter_reqq_id) &
      (orig_ctl.esr_sc_trace_filter_port_enable == new_ctl.esr_sc_trace_filter_port_enable) &
      (orig_ctl.esr_sc_trace_filter_port == new_ctl.esr_sc_trace_filter_port) &
      (orig_ctl.esr_sc_trace_filter_source_enable == new_ctl.esr_sc_trace_filter_source_enable) &
      (orig_ctl.esr_sc_trace_filter_source[$bits(new_ctl.esr_sc_trace_filter_source)-1:0] ==
       new_ctl.esr_sc_trace_filter_source) &
      (orig_ctl.esr_sc_trace_filter_l2_enable == new_ctl.esr_sc_trace_filter_l2_enable) &
      (orig_ctl.esr_sc_trace_filter_l3_enable == new_ctl.esr_sc_trace_filter_l3_enable) &
      (orig_ctl.esr_sc_trace_filter_fsm_enable == new_ctl.esr_sc_trace_filter_fsm_enable) &
      (orig_ctl.esr_sc_trace_type_hot_enable == new_ctl.esr_sc_trace_type_hot_enable) &
      (orig_ctl.wr_en_reqq_ctl == new_ctl.wr_en_reqq_ctl) &
      (orig_ctl.wr_en_idx_cop_sm_ctl == new_ctl.wr_en_idx_cop_sm_ctl) &
      (orig_ctl.wr_en_idx_cop_sm_physical_index == new_ctl.wr_en_idx_cop_sm_physical_index) &
      (orig_ctl.wr_en_idx_cop_sm_data0 == new_ctl.wr_en_idx_cop_sm_data0) &
      (orig_ctl.wr_en_idx_cop_sm_data1 == new_ctl.wr_en_idx_cop_sm_data1) &
      (orig_ctl.wr_en_idx_cop_sm_ecc == new_ctl.wr_en_idx_cop_sm_ecc) &
      (orig_ctl.wr_en_err_log_info == new_ctl.wr_en_err_log_info) &
      (orig_ctl.wr_en_sbe_dbe_counts == new_ctl.wr_en_sbe_dbe_counts) &
      (orig_ctl.wr_en_reqq_debug_ctl == new_ctl.wr_en_reqq_debug_ctl) &
      (orig_ctl.wr_en_perfmon_ctl_status == new_ctl.wr_en_perfmon_ctl_status) &
      (orig_ctl.wr_en_perfmon_cyc_cntr == new_ctl.wr_en_perfmon_cyc_cntr) &
      (orig_ctl.wr_en_perfmon_p0_cntr == new_ctl.wr_en_perfmon_p0_cntr) &
      (orig_ctl.wr_en_perfmon_p1_cntr == new_ctl.wr_en_perfmon_p1_cntr) &
      (orig_ctl.wr_en_perfmon_p0_qual == new_ctl.wr_en_perfmon_p0_qual) &
      (orig_ctl.wr_en_perfmon_p1_qual == new_ctl.wr_en_perfmon_p1_qual) &
      (orig_ctl.wr_data == new_ctl.wr_data) &
      (orig_ctl.access_in_flight == new_ctl.access_in_flight);
endmodule
