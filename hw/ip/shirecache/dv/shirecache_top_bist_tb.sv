// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // This directed wrapper ties off the full cache datapath and observes only the top-level icache BIST request.
module shirecache_top_bist_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  logic dft_mbist_en_i,

  output logic                     icache_bist_mbist_on_o,
  output logic                     icache_bist_mbi_sel_o,
  output logic                     icache_bist_mbi_rd_en_o,
  output logic                     icache_bist_mbi_wr_en_o,
  output logic [MbiAddrSize-1:0]   icache_bist_mbi_addr_o,
  output logic [63:0]              icache_bist_mbi_wdata_lo_o,
  output logic [63:0]              icache_bist_mbi_wdata_mid_o,
  output logic [MbiDataSize-129:0] icache_bist_mbi_wdata_hi_o
);

  dft_pkg::dft_t dft;
  assign dft = '0;

  ram_cfg_pkg::ram_cfg_t ram_cfg;
  assign ram_cfg = '0;

  logic [Ports-1:0][Banks:0] neigh_sc_req_ready;
  logic [Ports-1:0][Banks:0] neigh_sc_req_valid;
  etlink_pkg::req_t [Ports-1:0] neigh_sc_req_info;
  logic [Ports-1:0] neigh_sc_rsp_ready;
  logic [Ports-1:0] neigh_sc_rsp_valid;
  etlink_pkg::rsp_t [Ports-1:0] neigh_sc_rsp_info;

  assign neigh_sc_req_valid = '0;
  assign neigh_sc_req_info = '0;
  assign neigh_sc_rsp_ready = '1;

  xbar_req_t neigh_uc_req_info;
  logic neigh_uc_req_valid;
  logic [Ports-1:0] neigh_uc_rsp_ready;
  xbar_rsp_t neigh_uc_rsp_info;

  assign neigh_uc_rsp_info = '0;

  bank_esr_ctl_t [Banks-1:0] bank_esr_ctl;
  bank_esr_status_t [Banks-1:0] bank_esr_status;
  esr_shire_config_t esr_shire_config;

  assign bank_esr_ctl = '0;
  assign esr_shire_config = '0;

  logic esr_sc_remote_l3_enable;
  logic esr_sc_remote_scp_enable;
  logic [Banks-1:0] sc_bank_err_detected;
  logic [Banks-1:0] sc_bank_err_logged;

  logic [Banks-1:0] sc_neigh_l2hpf_req_valid;
  neigh_l2hpf_req_t [Banks-1:0] sc_neigh_l2hpf_req_info;

  trace_packet_t sc_trace_data;
  logic sc_trace_valid;

  sc_master_ar_t [L3MasterPorts-1:0] to_l3_axi_ar;
  sc_master_aw_t [L3MasterPorts-1:0] to_l3_axi_aw;
  sc_master_w_t [L3MasterPorts-1:0] to_l3_axi_w;
  sc_master_r_t [L3MasterPorts-1:0] to_l3_axi_r;
  sc_master_b_t [L3MasterPorts-1:0] to_l3_axi_b;
  logic [L3MasterPorts-1:0] to_l3_axi_ar_valid;
  logic [L3MasterPorts-1:0] to_l3_axi_aw_valid;
  logic [L3MasterPorts-1:0] to_l3_axi_w_valid;
  logic [L3MasterPorts-1:0] to_l3_axi_r_ready;
  logic [L3MasterPorts-1:0] to_l3_axi_b_ready;

  assign to_l3_axi_r = '0;
  assign to_l3_axi_b = '0;

  sc_master_ar_t [SysPorts-1:0] to_sys_axi_ar;
  sc_master_aw_t [SysPorts-1:0] to_sys_axi_aw;
  sc_master_w_t [SysPorts-1:0] to_sys_axi_w;
  sc_master_r_t [SysPorts-1:0] to_sys_axi_r;
  sc_master_b_t [SysPorts-1:0] to_sys_axi_b;
  logic [SysPorts-1:0] to_sys_axi_ar_valid;
  logic [SysPorts-1:0] to_sys_axi_aw_valid;
  logic [SysPorts-1:0] to_sys_axi_w_valid;
  logic [SysPorts-1:0] to_sys_axi_r_ready;
  logic [SysPorts-1:0] to_sys_axi_b_ready;

  assign to_sys_axi_r = '0;
  assign to_sys_axi_b = '0;

  sc_slave_ar_t [L3SlavePorts-1:0] l3_axi_ar;
  sc_slave_aw_t [L3SlavePorts-1:0] l3_axi_aw;
  sc_master_w_t [L3SlavePorts-1:0] l3_axi_w;
  sc_slave_r_t [L3SlavePorts-1:0] l3_axi_r;
  sc_slave_b_t [L3SlavePorts-1:0] l3_axi_b;
  logic [L3SlavePorts-1:0] l3_axi_ar_ready;
  logic [L3SlavePorts-1:0] l3_axi_aw_ready;
  logic [L3SlavePorts-1:0] l3_axi_w_ready;
  logic [L3SlavePorts-1:0] l3_axi_r_valid;
  logic [L3SlavePorts-1:0] l3_axi_b_valid;

  assign l3_axi_ar = '0;
  assign l3_axi_aw = '0;
  assign l3_axi_w = '0;

  icache_bist_req_t icache_bist_req;
  icache_bist_rsp_t icache_bist_rsp;

  assign icache_bist_rsp = '0;
  assign icache_bist_mbist_on_o = icache_bist_req.mbist_on;
  assign icache_bist_mbi_sel_o = icache_bist_req.mbi_sel;
  assign icache_bist_mbi_rd_en_o = icache_bist_req.mbi_rd_en;
  assign icache_bist_mbi_wr_en_o = icache_bist_req.mbi_wr_en;
  assign icache_bist_mbi_addr_o = icache_bist_req.mbi_addr;
  assign icache_bist_mbi_wdata_lo_o = icache_bist_req.mbi_wdata[63:0];
  assign icache_bist_mbi_wdata_mid_o = icache_bist_req.mbi_wdata[127:64];
  assign icache_bist_mbi_wdata_hi_o = icache_bist_req.mbi_wdata[MbiDataSize-1:128];

  shirecache_top u_dut (
    .clk_i                         (clk_i),
    .clk_free_i                    (clk_i),
    .rst_cold_ni                   (rst_ni),
    .rst_ni                        (rst_ni),
    .rst_debug_ni                  (rst_ni),
    .noc_clk_i                     (clk_i),
    .noc_rst_ni                    (rst_ni),
    .dft_i                         (dft),
    .dft_sram_clk_i                (1'b0),
    .dft_mbist_en_i                (dft_mbist_en_i),
    .ram_cfg_i                     (ram_cfg),
    .neigh_sc_req_ready_o          (neigh_sc_req_ready),
    .neigh_sc_req_valid_i          (neigh_sc_req_valid),
    .neigh_sc_req_info_i           (neigh_sc_req_info),
    .neigh_sc_rsp_ready_i          (neigh_sc_rsp_ready),
    .neigh_sc_rsp_valid_o          (neigh_sc_rsp_valid),
    .neigh_sc_rsp_info_o           (neigh_sc_rsp_info),
    .neigh_uc_req_ready_i          (1'b1),
    .neigh_uc_req_valid_o          (neigh_uc_req_valid),
    .neigh_uc_req_info_o           (neigh_uc_req_info),
    .neigh_uc_rsp_ready_o          (neigh_uc_rsp_ready),
    .neigh_uc_rsp_valid_i          ('0),
    .neigh_uc_rsp_info_i           (neigh_uc_rsp_info),
    .bank_esr_ctl_i                (bank_esr_ctl),
    .bank_esr_status_o             (bank_esr_status),
    .esr_sc_remote_l3_enable_o     (esr_sc_remote_l3_enable),
    .esr_sc_remote_scp_enable_o    (esr_sc_remote_scp_enable),
    .esr_shire_config_i            (esr_shire_config),
    .sc_bank_err_detected_o        (sc_bank_err_detected),
    .sc_bank_err_logged_o          (sc_bank_err_logged),
    .sc_neigh_l2hpf_req_valid_o    (sc_neigh_l2hpf_req_valid),
    .sc_neigh_l2hpf_req_info_o     (sc_neigh_l2hpf_req_info),
    .status_monitor_bank_sel_i     ('0),
    .sc_trace_data_o               (sc_trace_data),
    .sc_trace_valid_o              (sc_trace_valid),
    .to_l3_axi_ar_ready_i          ('1),
    .to_l3_axi_ar_valid_o          (to_l3_axi_ar_valid),
    .to_l3_axi_ar_o                (to_l3_axi_ar),
    .to_l3_axi_aw_ready_i          ('1),
    .to_l3_axi_aw_valid_o          (to_l3_axi_aw_valid),
    .to_l3_axi_aw_o                (to_l3_axi_aw),
    .to_l3_axi_w_ready_i           ('1),
    .to_l3_axi_w_valid_o           (to_l3_axi_w_valid),
    .to_l3_axi_w_o                 (to_l3_axi_w),
    .to_l3_axi_r_ready_o           (to_l3_axi_r_ready),
    .to_l3_axi_r_valid_i           ('0),
    .to_l3_axi_r_i                 (to_l3_axi_r),
    .to_l3_axi_b_ready_o           (to_l3_axi_b_ready),
    .to_l3_axi_b_valid_i           ('0),
    .to_l3_axi_b_i                 (to_l3_axi_b),
    .to_sys_axi_ar_ready_i         ('1),
    .to_sys_axi_ar_valid_o         (to_sys_axi_ar_valid),
    .to_sys_axi_ar_o               (to_sys_axi_ar),
    .to_sys_axi_aw_ready_i         ('1),
    .to_sys_axi_aw_valid_o         (to_sys_axi_aw_valid),
    .to_sys_axi_aw_o               (to_sys_axi_aw),
    .to_sys_axi_w_ready_i          ('1),
    .to_sys_axi_w_valid_o          (to_sys_axi_w_valid),
    .to_sys_axi_w_o                (to_sys_axi_w),
    .to_sys_axi_r_ready_o          (to_sys_axi_r_ready),
    .to_sys_axi_r_valid_i          ('0),
    .to_sys_axi_r_i                (to_sys_axi_r),
    .to_sys_axi_b_ready_o          (to_sys_axi_b_ready),
    .to_sys_axi_b_valid_i          ('0),
    .to_sys_axi_b_i                (to_sys_axi_b),
    .l3_axi_ar_ready_o             (l3_axi_ar_ready),
    .l3_axi_ar_valid_i             ('0),
    .l3_axi_ar_i                   (l3_axi_ar),
    .l3_axi_aw_ready_o             (l3_axi_aw_ready),
    .l3_axi_aw_valid_i             ('0),
    .l3_axi_aw_i                   (l3_axi_aw),
    .l3_axi_w_ready_o              (l3_axi_w_ready),
    .l3_axi_w_valid_i              ('0),
    .l3_axi_w_i                    (l3_axi_w),
    .l3_axi_r_ready_i              ('1),
    .l3_axi_r_valid_o              (l3_axi_r_valid),
    .l3_axi_r_o                    (l3_axi_r),
    .l3_axi_b_ready_i              ('1),
    .l3_axi_b_valid_o              (l3_axi_b_valid),
    .l3_axi_b_o                    (l3_axi_b),
    .icache_bist_req_info_o        (icache_bist_req),
    .icache_bist_rsp_info_i        (icache_bist_rsp)
  );

  logic unused_ok;
  assign unused_ok = &{1'b0,
    neigh_sc_req_ready,
    neigh_sc_rsp_valid,
    neigh_sc_rsp_info,
    neigh_uc_req_valid,
    neigh_uc_req_info,
    neigh_uc_rsp_ready,
    bank_esr_status,
    esr_sc_remote_l3_enable,
    esr_sc_remote_scp_enable,
    sc_bank_err_detected,
    sc_bank_err_logged,
    sc_neigh_l2hpf_req_valid,
    sc_neigh_l2hpf_req_info,
    sc_trace_data,
    sc_trace_valid,
    to_l3_axi_ar_valid,
    to_l3_axi_ar,
    to_l3_axi_aw_valid,
    to_l3_axi_aw,
    to_l3_axi_w_valid,
    to_l3_axi_w,
    to_l3_axi_r_ready,
    to_l3_axi_b_ready,
    to_sys_axi_ar_valid,
    to_sys_axi_ar,
    to_sys_axi_aw_valid,
    to_sys_axi_aw,
    to_sys_axi_w_valid,
    to_sys_axi_w,
    to_sys_axi_r_ready,
    to_sys_axi_b_ready,
    l3_axi_ar_ready,
    l3_axi_aw_ready,
    l3_axi_w_ready,
    l3_axi_r_valid,
    l3_axi_r,
    l3_axi_b_valid,
    l3_axi_b,
    unused_ok};

endmodule : shirecache_top_bist_tb
/* verilator lint_on UNUSEDSIGNAL */
