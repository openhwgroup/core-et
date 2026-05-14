// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Lightweight shirecache_top free-running-clock structural smoke test.
//
// The real shirecache_top hierarchy is large. This test replaces the bank,
// xbar, and mesh leaves with local stubs so the top-level port hookup can be
// checked directly: the bank stub exposes the clock it receives through the
// trace-valid path. If shirecache_top accidentally ties bank clk_free_i to
// clk_i, sc_trace_valid_o asserts while the external clk_free_i input is held
// low and the C++ test fails.

/* verilator lint_off DECLFILENAME */  // Local leaf stubs intentionally live with this small top-hookup testbench.
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off PINCONNECTEMPTY */

module shirecache_top_freeclk_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                  clk_i,
  input  logic                  rst_ni,
  input  logic                  clk_free_i,
  input  logic [BankIdSize-1:0] status_monitor_bank_sel_i,
  output logic                  sc_trace_valid_o
);

  dft_t     dft;
  ram_cfg_t ram_cfg;
  assign dft     = '0;
  assign ram_cfg = '0;

  logic [Ports-1:0][Banks:0] neigh_sc_req_ready;
  logic [Ports-1:0][Banks:0] neigh_sc_req_valid;
  req_t [Ports-1:0]          neigh_sc_req_info;
  logic [Ports-1:0]          neigh_sc_rsp_ready;
  logic [Ports-1:0]          neigh_sc_rsp_valid;
  rsp_t [Ports-1:0]          neigh_sc_rsp_info;

  logic                      neigh_uc_req_valid;
  xbar_req_t                 neigh_uc_req_info;
  logic [Ports-1:0]          neigh_uc_rsp_ready;
  xbar_rsp_t                 neigh_uc_rsp_info;

  bank_esr_ctl_t [Banks-1:0]    bank_esr_ctl;
  bank_esr_status_t [Banks-1:0] bank_esr_status;
  esr_shire_config_t            esr_shire_config;

  logic                         esr_sc_remote_l3_enable;
  logic                         esr_sc_remote_scp_enable;
  logic [Banks-1:0]             sc_bank_err_detected;
  logic [Banks-1:0]             sc_bank_err_logged;
  logic [Banks-1:0]             sc_neigh_l2hpf_req_valid;
  neigh_l2hpf_req_t [Banks-1:0] sc_neigh_l2hpf_req_info;
  trace_packet_t                sc_trace_data;

  sc_master_ar_t [L3MasterPorts-1:0] to_l3_axi_ar;
  sc_master_aw_t [L3MasterPorts-1:0] to_l3_axi_aw;
  sc_master_w_t  [L3MasterPorts-1:0] to_l3_axi_w;
  sc_master_r_t  [L3MasterPorts-1:0] to_l3_axi_r;
  sc_master_b_t  [L3MasterPorts-1:0] to_l3_axi_b;

  sc_master_ar_t [SysPorts-1:0] to_sys_axi_ar;
  sc_master_aw_t [SysPorts-1:0] to_sys_axi_aw;
  sc_master_w_t  [SysPorts-1:0] to_sys_axi_w;
  sc_master_r_t  [SysPorts-1:0] to_sys_axi_r;
  sc_master_b_t  [SysPorts-1:0] to_sys_axi_b;

  sc_slave_ar_t [L3SlavePorts-1:0] l3_axi_ar;
  sc_slave_aw_t [L3SlavePorts-1:0] l3_axi_aw;
  sc_master_w_t [L3SlavePorts-1:0] l3_axi_w;
  sc_slave_r_t  [L3SlavePorts-1:0] l3_axi_r;
  sc_slave_b_t  [L3SlavePorts-1:0] l3_axi_b;

  icache_bist_req_t icache_bist_req_info;
  icache_bist_rsp_t icache_bist_rsp_info;

  assign neigh_sc_req_valid       = '0;
  assign neigh_sc_req_info        = '0;
  assign neigh_sc_rsp_ready       = '1;
  assign neigh_uc_rsp_info        = '0;
  assign bank_esr_ctl             = '0;
  assign esr_shire_config         = '0;
  assign to_l3_axi_r              = '0;
  assign to_l3_axi_b              = '0;
  assign to_sys_axi_r             = '0;
  assign to_sys_axi_b             = '0;
  assign l3_axi_ar                = '0;
  assign l3_axi_aw                = '0;
  assign l3_axi_w                 = '0;
  assign icache_bist_rsp_info     = '0;

  shirecache_top u_dut (
    .clk_i                       (clk_i),
    .clk_free_i                  (clk_free_i),
    .rst_cold_ni                 (rst_ni),
    .rst_ni                      (rst_ni),
    .rst_debug_ni                (rst_ni),
    .noc_clk_i                   (clk_i),
    .noc_rst_ni                  (rst_ni),
    .dft_i                       (dft),
    .dft_sram_clk_i              (1'b0),
    .ram_cfg_i                   (ram_cfg),
    .neigh_sc_req_ready_o        (neigh_sc_req_ready),
    .neigh_sc_req_valid_i        (neigh_sc_req_valid),
    .neigh_sc_req_info_i         (neigh_sc_req_info),
    .neigh_sc_rsp_ready_i        (neigh_sc_rsp_ready),
    .neigh_sc_rsp_valid_o        (neigh_sc_rsp_valid),
    .neigh_sc_rsp_info_o         (neigh_sc_rsp_info),
    .neigh_uc_req_ready_i        (1'b1),
    .neigh_uc_req_valid_o        (neigh_uc_req_valid),
    .neigh_uc_req_info_o         (neigh_uc_req_info),
    .neigh_uc_rsp_ready_o        (neigh_uc_rsp_ready),
    .neigh_uc_rsp_valid_i        ('0),
    .neigh_uc_rsp_info_i         (neigh_uc_rsp_info),
    .bank_esr_ctl_i              (bank_esr_ctl),
    .bank_esr_status_o           (bank_esr_status),
    .esr_sc_remote_l3_enable_o   (esr_sc_remote_l3_enable),
    .esr_sc_remote_scp_enable_o  (esr_sc_remote_scp_enable),
    .esr_shire_config_i          (esr_shire_config),
    .sc_bank_err_detected_o      (sc_bank_err_detected),
    .sc_bank_err_logged_o        (sc_bank_err_logged),
    .sc_neigh_l2hpf_req_valid_o  (sc_neigh_l2hpf_req_valid),
    .sc_neigh_l2hpf_req_info_o   (sc_neigh_l2hpf_req_info),
    .status_monitor_bank_sel_i   (status_monitor_bank_sel_i),
    .sc_trace_data_o             (sc_trace_data),
    .sc_trace_valid_o            (sc_trace_valid_o),
    .to_l3_axi_ar_ready_i        ('1),
    .to_l3_axi_ar_valid_o        (),
    .to_l3_axi_ar_o              (to_l3_axi_ar),
    .to_l3_axi_aw_ready_i        ('1),
    .to_l3_axi_aw_valid_o        (),
    .to_l3_axi_aw_o              (to_l3_axi_aw),
    .to_l3_axi_w_ready_i         ('1),
    .to_l3_axi_w_valid_o         (),
    .to_l3_axi_w_o               (to_l3_axi_w),
    .to_l3_axi_r_ready_o         (),
    .to_l3_axi_r_valid_i         ('0),
    .to_l3_axi_r_i               (to_l3_axi_r),
    .to_l3_axi_b_ready_o         (),
    .to_l3_axi_b_valid_i         ('0),
    .to_l3_axi_b_i               (to_l3_axi_b),
    .to_sys_axi_ar_ready_i       ('1),
    .to_sys_axi_ar_valid_o       (),
    .to_sys_axi_ar_o             (to_sys_axi_ar),
    .to_sys_axi_aw_ready_i       ('1),
    .to_sys_axi_aw_valid_o       (),
    .to_sys_axi_aw_o             (to_sys_axi_aw),
    .to_sys_axi_w_ready_i        ('1),
    .to_sys_axi_w_valid_o        (),
    .to_sys_axi_w_o              (to_sys_axi_w),
    .to_sys_axi_r_ready_o        (),
    .to_sys_axi_r_valid_i        ('0),
    .to_sys_axi_r_i              (to_sys_axi_r),
    .to_sys_axi_b_ready_o        (),
    .to_sys_axi_b_valid_i        ('0),
    .to_sys_axi_b_i              (to_sys_axi_b),
    .l3_axi_ar_ready_o           (),
    .l3_axi_ar_valid_i           ('0),
    .l3_axi_ar_i                 (l3_axi_ar),
    .l3_axi_aw_ready_o           (),
    .l3_axi_aw_valid_i           ('0),
    .l3_axi_aw_i                 (l3_axi_aw),
    .l3_axi_w_ready_o            (),
    .l3_axi_w_valid_i            ('0),
    .l3_axi_w_i                  (l3_axi_w),
    .l3_axi_r_ready_i            ('1),
    .l3_axi_r_valid_o            (),
    .l3_axi_r_o                  (l3_axi_r),
    .l3_axi_b_ready_i            ('1),
    .l3_axi_b_valid_o            (),
    .l3_axi_b_o                  (l3_axi_b),
    .icache_bist_req_info_o      (icache_bist_req_info),
    .icache_bist_rsp_info_i      (icache_bist_rsp_info)
  );

endmodule : shirecache_top_freeclk_tb

module shirecache_xbar
  import shirecache_pkg::*;
#(
  parameter int unsigned FifoDepth      = 3,
  parameter int unsigned NumSrc         = 8,
  parameter logic [NumSrc-1:0] NumSrcMask = {NumSrc{1'b1}},
  parameter logic [NumSrc-1:0] SrcArb     = '0,
  parameter int unsigned NumDest        = 8,
  parameter logic [NumDest-1:0] AddOutputRelay = '0,
  parameter int unsigned DataWidth      = 128
) (
  input  logic [NumSrc-1:0][NumDest-1:0] src_valid_i,
  output logic [NumSrc-1:0][NumDest-1:0] src_ready_o,
  output logic [NumSrc-1:0][NumDest-1:0] src_2_credits_o,
  input  logic [NumSrc-1:0][DataWidth-1:0] src_data_i,
  input  logic [NumDest-1:0] dest_ready_i,
  output logic [NumDest-1:0] dest_valid_o,
  output logic [NumDest-1:0][DataWidth-1:0] dest_data_o,
  input  logic clk_i,
  input  logic rst_ni
);
  assign src_ready_o     = '0;
  assign src_2_credits_o = '0;
  assign dest_valid_o    = '0;
  assign dest_data_o     = '0;
endmodule : shirecache_xbar

module shirecache_bank
  import shirecache_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
#(
  parameter int unsigned NumPorts         = Ports,
  parameter int unsigned NumEntries       = ReqqDepth,
  parameter int unsigned SetsPerSubBankP  = SetsPerSubBank,
  parameter bit          L2HpfImplemented = 0
) (
  input  logic                        clk_i,
  input  logic                        clk_free_i,
  input  logic                        rst_ni,
  input  logic                        rst_c_ni,
  input  logic                        rst_d_ni,
  input  dft_t                        dft_i,
  input  logic                        dft_sram_clk_i,
  input  logic                        dft_mbist_en_i,
  input  logic [EtEcoShirecacheBankInputWidth-1:0]  eco_i,
  output logic [EtEcoShirecacheBankOutputWidth-1:0] eco_o,
  input  ram_cfg_t                    ram_cfg_i,
  input  logic                        mbi_implemented_const_i,
  output logic                        bank_req_ready_o,
  input  logic                        bank_req_valid_i,
  input  xbar_req_t                   bank_req_info_i,
  output logic [NumPorts-1:0]         bank_rsp_valid_o,
  output xbar_rsp_t                   bank_rsp_info_o,
  input  logic [NumPorts-1:0]         bank_rsp_ready_i,
  input  logic [NumPorts-1:0]         bank_rsp_2_credits_i,
  output logic                        to_l3_mesh_master_bank_clk_en_o,
  input  logic                        to_l3_mesh_master_bank_req_ready_i,
  output logic                        to_l3_mesh_master_bank_req_valid_o,
  output mesh_master_req_t            to_l3_mesh_master_bank_req_info_o,
  output logic                        to_l3_mesh_master_bank_rsp_ready_o,
  input  logic                        to_l3_mesh_master_bank_rsp_valid_i,
  input  mesh_master_rsp_t            to_l3_mesh_master_bank_rsp_info_i,
  output logic                        to_sys_mesh_master_bank_clk_en_o,
  input  logic                        to_sys_mesh_master_bank_req_ready_i,
  output logic                        to_sys_mesh_master_bank_req_valid_o,
  output mesh_master_req_t            to_sys_mesh_master_bank_req_info_o,
  output logic                        to_sys_mesh_master_bank_rsp_ready_o,
  input  logic                        to_sys_mesh_master_bank_rsp_valid_i,
  input  mesh_master_rsp_t            to_sys_mesh_master_bank_rsp_info_i,
  input  logic                        l3_mesh_slave_bank_clk_en_i,
  output logic                        l3_mesh_slave_bank_req_ready_o,
  input  logic                        l3_mesh_slave_bank_req_valid_i,
  input  mesh_slave_req_t             l3_mesh_slave_bank_req_info_i,
  input  logic                        l3_mesh_slave_bank_rsp_ready_i,
  output logic                        l3_mesh_slave_bank_rsp_valid_o,
  output mesh_slave_rsp_t             l3_mesh_slave_bank_rsp_info_o,
  output icache_bist_req_t            icache_bist_req_info_o,
  input  icache_bist_rsp_t            icache_bist_rsp_info_i,
  input  logic                        mbist_en_i,
  input  logic [MbistSelSize-1:0]     mbist_sel_i,
  input  logic                        mbist_rd_en_i,
  input  logic                        mbist_wr_en_i,
  input  logic [MbistAddrSize-1:0]    mbist_addr_i,
  input  logic [MbistDataSize-1:0]    mbist_wdata_i,
  output logic [MbistDataSize-1:0]    mbist_rdata_o,
  input  bank_esr_ctl_t               esr_ctl_i,
  output bank_esr_status_t            esr_status_o,
  input  esr_shire_config_t           esr_shire_config_i,
  output logic                        err_detected_o,
  output logic                        err_logged_o,
  input  logic [BankIdSize-1:0]       my_bank_id_i,
  output trace_packet_t               bank_trace_data_o,
  output logic                        bank_trace_valid_o,
  output logic                        l2hpf_req_valid_o,
  output neigh_l2hpf_req_t            l2hpf_req_info_o,
  output logic                        esr_sc_remote_l3_enable_o,
  output logic                        esr_sc_remote_scp_enable_o,
  output l3_swizzle_ctl_t             esr_sc_l3_shire_swizzle_ctl_o
);
  assign eco_o                              = '0;
  assign bank_req_ready_o                   = 1'b1;
  assign bank_rsp_valid_o                   = '0;
  assign bank_rsp_info_o                    = '0;
  assign to_l3_mesh_master_bank_clk_en_o    = 1'b0;
  assign to_l3_mesh_master_bank_req_valid_o = 1'b0;
  assign to_l3_mesh_master_bank_req_info_o  = '0;
  assign to_l3_mesh_master_bank_rsp_ready_o = 1'b0;
  assign to_sys_mesh_master_bank_clk_en_o   = 1'b0;
  assign to_sys_mesh_master_bank_req_valid_o = 1'b0;
  assign to_sys_mesh_master_bank_req_info_o = '0;
  assign to_sys_mesh_master_bank_rsp_ready_o = 1'b0;
  assign l3_mesh_slave_bank_req_ready_o     = 1'b1;
  assign l3_mesh_slave_bank_rsp_valid_o     = 1'b0;
  assign l3_mesh_slave_bank_rsp_info_o      = '0;
  assign icache_bist_req_info_o             = '0;
  assign mbist_rdata_o                      = '0;
  assign esr_status_o                       = '0;
  assign err_detected_o                     = 1'b0;
  assign err_logged_o                       = 1'b0;
  assign bank_trace_data_o                  = '0;
  assign bank_trace_valid_o                 = clk_free_i;
  assign l2hpf_req_valid_o                  = 1'b0;
  assign l2hpf_req_info_o                   = '0;
  assign esr_sc_remote_l3_enable_o          = 1'b0;
  assign esr_sc_remote_scp_enable_o         = 1'b0;
  assign esr_sc_l3_shire_swizzle_ctl_o      = '0;
endmodule : shirecache_bank

module shirecache_mesh_master
  import shirecache_pkg::*;
  import axi_pkg::*;
  import dft_pkg::*;
#(
  parameter bit          IsToL3          = 1'b1,
  parameter int unsigned NumBanks        = Banks,
  parameter int unsigned NumPorts        = L3MasterPorts,
  parameter bit          UseNocClkGating = 1'b1
) (
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic                         noc_clk_i,
  input  logic                         noc_rst_ni,
  input  dft_t                         dft_i,
  input  l3_swizzle_ctl_t              esr_sc_l3_shire_swizzle_ctl_i,
  input  logic [NumBanks-1:0]          mesh_master_bank_clk_en_i,
  output logic [NumBanks-1:0]          mesh_master_bank_req_ready_o,
  input  logic [NumBanks-1:0]          mesh_master_bank_req_valid_i,
  input  mesh_master_req_t [NumBanks-1:0] mesh_master_bank_req_info_i,
  input  logic [NumBanks-1:0]          mesh_master_bank_rsp_ready_i,
  output logic [NumBanks-1:0]          mesh_master_bank_rsp_valid_o,
  output mesh_master_rsp_t [NumBanks-1:0] mesh_master_bank_rsp_info_o,
  input  logic [NumPorts-1:0]          axi_ar_ready_i,
  output logic [NumPorts-1:0]          axi_ar_valid_o,
  output sc_master_ar_t [NumPorts-1:0] axi_ar_o,
  input  logic [NumPorts-1:0]          axi_aw_ready_i,
  output logic [NumPorts-1:0]          axi_aw_valid_o,
  output sc_master_aw_t [NumPorts-1:0] axi_aw_o,
  input  logic [NumPorts-1:0]          axi_w_ready_i,
  output logic [NumPorts-1:0]          axi_w_valid_o,
  output sc_master_w_t [NumPorts-1:0]  axi_w_o,
  output logic [NumPorts-1:0]          axi_r_ready_o,
  input  logic [NumPorts-1:0]          axi_r_valid_i,
  input  sc_master_r_t [NumPorts-1:0]  axi_r_i,
  output logic [NumPorts-1:0]          axi_b_ready_o,
  input  logic [NumPorts-1:0]          axi_b_valid_i,
  input  sc_master_b_t [NumPorts-1:0]  axi_b_i
);
  assign mesh_master_bank_req_ready_o = '0;
  assign mesh_master_bank_rsp_valid_o = '0;
  assign mesh_master_bank_rsp_info_o  = '0;
  assign axi_ar_valid_o               = '0;
  assign axi_ar_o                     = '0;
  assign axi_aw_valid_o               = '0;
  assign axi_aw_o                     = '0;
  assign axi_w_valid_o                = '0;
  assign axi_w_o                      = '0;
  assign axi_r_ready_o                = '0;
  assign axi_b_ready_o                = '0;
endmodule : shirecache_mesh_master

module shirecache_mesh_slave
  import shirecache_pkg::*;
  import axi_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumBanks = Banks,
  parameter int unsigned NumPorts = L3SlavePorts
) (
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic                         noc_clk_i,
  input  logic                         noc_rst_ni,
  input  dft_t                         dft_i,
  input  l3_swizzle_ctl_t              esr_sc_l3_shire_swizzle_ctl_i,
  output logic [NumBanks-1:0]          mesh_slave_bank_clk_en_o,
  input  logic [NumBanks-1:0]          mesh_slave_bank_req_ready_i,
  output logic [NumBanks-1:0]          mesh_slave_bank_req_valid_o,
  output mesh_slave_req_t [NumBanks-1:0] mesh_slave_bank_req_info_o,
  output logic [NumBanks-1:0]          mesh_slave_bank_rsp_ready_o,
  input  logic [NumBanks-1:0]          mesh_slave_bank_rsp_valid_i,
  input  mesh_slave_rsp_t [NumBanks-1:0] mesh_slave_bank_rsp_info_i,
  output logic [NumPorts-1:0]          axi_ar_ready_o,
  input  logic [NumPorts-1:0]          axi_ar_valid_i,
  input  sc_slave_ar_t [NumPorts-1:0]  axi_ar_i,
  output logic [NumPorts-1:0]          axi_aw_ready_o,
  input  logic [NumPorts-1:0]          axi_aw_valid_i,
  input  sc_slave_aw_t [NumPorts-1:0]  axi_aw_i,
  output logic [NumPorts-1:0]          axi_w_ready_o,
  input  logic [NumPorts-1:0]          axi_w_valid_i,
  input  sc_master_w_t [NumPorts-1:0]  axi_w_i,
  input  logic [NumPorts-1:0]          axi_r_ready_i,
  output logic [NumPorts-1:0]          axi_r_valid_o,
  output sc_slave_r_t [NumPorts-1:0]   axi_r_o,
  input  logic [NumPorts-1:0]          axi_b_ready_i,
  output logic [NumPorts-1:0]          axi_b_valid_o,
  output sc_slave_b_t [NumPorts-1:0]   axi_b_o
);
  assign mesh_slave_bank_clk_en_o     = '0;
  assign mesh_slave_bank_req_valid_o  = '0;
  assign mesh_slave_bank_req_info_o   = '0;
  assign mesh_slave_bank_rsp_ready_o  = '0;
  assign axi_ar_ready_o               = '0;
  assign axi_aw_ready_o               = '0;
  assign axi_w_ready_o                = '0;
  assign axi_r_valid_o                = '0;
  assign axi_r_o                      = '0;
  assign axi_b_valid_o                = '0;
  assign axi_b_o                      = '0;
endmodule : shirecache_mesh_slave

/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on DECLFILENAME */
