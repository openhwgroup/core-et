// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module esr_cache_bank_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        rst_c_ni,
  input  logic        rst_w_ni,
  input  logic        rst_d_ni,

  input  logic [11:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,
  output logic        apb_pready_o,
  output logic [63:0] apb_prdata_o,
  output logic        apb_pslverr_o,

  input  logic [63:0] status_idx_ctl_i,
  input  logic [63:0] status_idx_index_i,
  input  logic [63:0] status_idx_data0_i,
  input  logic [63:0] status_err_info_i,
  input  logic [63:0] status_err_addr_i,
  input  logic [63:0] status_counts_i,
  input  logic [63:0] status_perf_cyc_i,

  output logic [63:0] ctl_wr_data_o,
  output logic        wr_en_reqq_ctl_o,
  output logic        wr_en_idx_ctl_o,
  output logic        wr_en_err_info_o,
  output logic        access_in_flight_o,
  output logic [9:0]  l2_set_base_o,
  output logic [9:0]  l3_set_base_o,
  output logic [9:0]  scp_set_base_o,
  output logic [5:0]  num_l3_reqq_entries_o,
  output logic        cbuf_enable_o,
  output logic        idx_user_enable_o
);
  /* verilator lint_off UNUSEDSIGNAL */  // Unit-test wrapper exposes only the control fields checked by C++.
  shirecache_pkg::bank_esr_ctl_t ctl;
  /* verilator lint_on UNUSEDSIGNAL */
  shirecache_pkg::bank_esr_status_t status;
  shire_esr_pkg::esr_sc_l3_shire_swizzle_ctl_t swizzle_unused;
  logic [7:0] eco_unused;

  always_comb begin
    status = '0;
    status.pipe_idx_cop_sm.ctl = status_idx_ctl_i;
    status.pipe_idx_cop_sm.physical_index = status_idx_index_i;
    status.pipe_idx_cop_sm.data0 = status_idx_data0_i;
    status.err_log.info = status_err_info_i;
    status.err_log.address = status_err_addr_i;
    status.sbe_dbe_counts = status_counts_i;
    status.perfmon.cyc_cntr = status_perf_cyc_i;
  end

  esr_cache_bank u_dut (
    .clk_i          (clk_i),
    .rst_c_ni       (rst_ni & rst_c_ni),
    .rst_w_ni       (rst_ni & rst_w_ni),
    .rst_d_ni       (rst_ni & rst_d_ni),
    .test_en_i      (1'b0),
    .esr_ctl_o                       (ctl),
    .esr_status_i                    (status),
    .esr_sc_l3_shire_swizzle_ctl_o   (swizzle_unused),
    .esr_sc_eco_ctl_o                (eco_unused),
    .apb_paddr_i                     (apb_paddr_i),
    .apb_pwrite_i   (apb_pwrite_i),
    .apb_psel_i     (apb_psel_i),
    .apb_penable_i  (apb_penable_i),
    .apb_pwdata_i   (apb_pwdata_i),
    .apb_pready_o   (apb_pready_o),
    .apb_prdata_o   (apb_prdata_o),
    .apb_pslverr_o  (apb_pslverr_o)
  );

  assign ctl_wr_data_o = ctl.wr_data;
  assign wr_en_reqq_ctl_o = ctl.wr_en_reqq_ctl;
  assign wr_en_idx_ctl_o = ctl.wr_en_idx_cop_sm_ctl;
  assign wr_en_err_info_o = ctl.wr_en_err_log_info;
  assign access_in_flight_o = ctl.access_in_flight;
  assign l2_set_base_o = ctl.esr_sc_l2_set_base;
  assign l3_set_base_o = ctl.esr_sc_l3_set_base;
  assign scp_set_base_o = ctl.esr_sc_scp_set_base;
  assign num_l3_reqq_entries_o = ctl.esr_sc_num_l3_reqq_entries;
  assign cbuf_enable_o = ctl.esr_sc_cbuf_enable;
  assign idx_user_enable_o = ctl.esr_sc_idx_cop_sm_ctl_user_en;

  logic unused_wrapper_outputs;
  assign unused_wrapper_outputs = &{1'b0, swizzle_unused, eco_unused};
endmodule
