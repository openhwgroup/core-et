// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // TB drives a smoke subset of the native top while observing internal fanout seams.
/* verilator lint_off PINCONNECTEMPTY */  // AXI payload outputs not checked directly in this smoke are intentionally ignored.
/* verilator lint_off VARHIDDEN */  // TB local constants intentionally mirror imported package topology names.
module shire_top_tb
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_pkg::*;
  import ram_cfg_pkg::*;
  import shire_channel_leaves_pkg::*;
  import shire_esr_pkg::*;
  import shire_sbm_pkg::*;
  import shire_uncached_pkg::*;
  import neigh_ch_apb_mux_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic rst_cold_ext_ni,
  input  logic rst_warm_ext_ni,
  input  logic rst_system_ext_ni,
  input  logic rst_system_debug_ext_ni,
  input  logic rst_noc_ext_ni,

  input  logic dft_scanmode_i,
  input  logic dft_scan_reset_ni,
  input  logic dft_sram_clk_i,
  input  logic dft_sram_clk_override_i,
  input  logic dft_ram_rei_i,
  input  logic dft_ram_wei_i,
  input  logic dft_mbist_en_i,

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_id_reset_val_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_phy_id_i,
  input  logic plic_mtip_i,
  input  logic plic_meip_i,
  input  logic plic_seip_i,
  input  logic [shire_channel_leaves_pkg::NocIntNum-1:0] noc_err_int_srcs_i,
  input  logic [shirecache_pkg::BankIdSize-1:0] status_monitor_bank_sel_stim_i,
  input  logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0]
      debug_and_or_tree_l2_stim_i,

  input  logic sys_axi_ar_valid_stim_i,
  input  logic sys_axi_aw_valid_stim_i,
  input  logic sys_axi_w_valid_stim_i,
  input  logic sys_axi_b_ready_stim_i,
  input  logic sys_axi_r_ready_stim_i,
  input  logic [1:0] sys_axi_aw_vcvalid_stim_i,
  input  logic [1:0] sys_axi_w_vcvalid_stim_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] sys_id_stim_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] sys_addr_stim_i,
  input  logic [axi_pkg::SysSlaveDataSize-1:0] sys_wdata_stim_i,
  input  logic [7:0] sys_len_stim_i,
  input  logic [2:0] sys_size_stim_i,

  output logic [shire_esr_pkg::NumNeigh-1:0] rst_c_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_w_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_d_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_warm_to_neigh_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_w_icache_no_o,
  output logic rst_system_lv_no_o,
  output logic rst_system_debug_lv_no_o,
  output logic rst_noc_lv_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] clk_neigh_obs_o,
  output logic clk_shire_obs_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] shire_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0][1:0] tbox_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0] tbox_en_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] plic_meip_to_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] plic_seip_to_neigh_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] int_mtip_flat_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] apb_neigh_psel_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] apb_neigh_pwrite_o,
  output logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] apb_neigh0_paddr_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] apb_neigh0_pwdata_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_psel_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_pwrite_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shirecache_pkg::Banks:0] neigh_sc_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_rsp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] icache_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] icache_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] flb_l2_neigh_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] flb_l2_neigh_resp_data_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] uc_to_neigh_fcc_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_iso_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] pwr_ctrl_neigh_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] pwr_ctrl_neigh_iso_o,
  output logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0] ram_cfg_flat_o,
  output logic [$bits(dft_pkg::dft_t)-1:0] dft_hv_flat_o,
  output logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0] debug_and_or_tree_l2_flat_o,
  output logic ioshire_combined_err_int_o,
  output logic ioshire_noc_err_int_o,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0] noc_all_err_int_srcs_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_slv_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_done_valid_o,
  output logic sys_axi_ar_ready_obs_o,
  output logic sys_axi_aw_ready_obs_o,
  output logic sys_axi_w_ready_obs_o,
  output logic sys_axi_b_valid_obs_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] sys_axi_b_id_obs_o,
  output logic [1:0] sys_axi_b_resp_obs_o,
  output logic sys_axi_r_valid_obs_o,
  output logic [63:0] sys_axi_r_data_lo_obs_o,
  output logic [1:0] sys_axi_aw_credit_obs_o
);

  localparam int unsigned NumNeigh = shire_esr_pkg::NumNeigh;
  localparam int unsigned NumBanks = shirecache_pkg::Banks;
  localparam int unsigned NumRbox = shirecache_pkg::RboxPerShire;
  localparam int unsigned NumL3MasterPorts = shirecache_pkg::L3MasterPorts;
  localparam int unsigned NumL3SlavePorts = shirecache_pkg::L3SlavePorts;
  localparam int unsigned NumSysPorts = shirecache_pkg::SysPorts;
  localparam int unsigned ApbSlaves = NumBanks + NumRbox + 2;

  dft_pkg::dft_t dft_hv;
  dft_pkg::dft_t dft_lv;
  always_comb begin
    dft_hv.scanmode = dft_scanmode_i;
    dft_hv.scan_reset_n = dft_scan_reset_ni;
    dft_hv.sram_clk_override = dft_sram_clk_override_i;
    dft_hv.ram_rei = dft_ram_rei_i;
    dft_hv.ram_wei = dft_ram_wei_i;
    dft_lv = dft_hv;
  end

  shire_channel_leaves_pkg::bpam_run_control_t bpam_run_control;
  always_comb begin
    bpam_run_control = '0;
    bpam_run_control.gpio.ndmreset = dft_scanmode_i;
    bpam_run_control.gpio.halt_req = dft_ram_rei_i;
    bpam_run_control.gpio.resume_req = dft_ram_wei_i;
    bpam_run_control.events.halt_req = dft_sram_clk_override_i;
    bpam_run_control.events.resume_req = dft_mbist_en_i;
  end

  shire_channel_leaves_pkg::esr_and_or_tree_l2_t debug_and_or_tree_l2_i;
  assign debug_and_or_tree_l2_i = debug_and_or_tree_l2_stim_i;
  shire_channel_leaves_pkg::esr_and_or_tree_l2_t debug_and_or_tree_l2_o;

  axi_pkg::sc_master_b_t [NumL3MasterPorts-1:0] to_l3_axi_b;
  axi_pkg::sc_master_r_t [NumL3MasterPorts-1:0] to_l3_axi_r;
  axi_pkg::sc_master_b_t [NumSysPorts-1:0] to_sys_axi_b;
  axi_pkg::sc_master_r_t [NumSysPorts-1:0] to_sys_axi_r;
  axi_pkg::sc_slave_ar_t [NumL3SlavePorts-1:0] l3_axi_ar;
  axi_pkg::sc_slave_aw_t [NumL3SlavePorts-1:0] l3_axi_aw;
  axi_pkg::sc_master_w_t [NumL3SlavePorts-1:0] l3_axi_w;
  axi_pkg::sc_master_b_t uc_to_l3_axi_b;
  axi_pkg::sc_master_r_t uc_to_l3_axi_r;
  axi_pkg::sc_master_b_t uc_to_sys_axi_b;
  axi_pkg::sc_master_r_t uc_to_sys_axi_r;
  axi_pkg::sys_slave_ar_t sys_axi_ar;
  axi_pkg::sys_slave_aw_t sys_axi_aw;
  axi_pkg::sys_slave_w_t sys_axi_w;
  axi_pkg::sys_slave_b_t sys_axi_b;
  axi_pkg::sys_slave_r_t sys_axi_r;
  logic [1:0] sys_axi_aw_credit;

  always_comb begin
    to_l3_axi_b = '0;
    to_l3_axi_r = '0;
    to_sys_axi_b = '0;
    to_sys_axi_r = '0;
    l3_axi_ar = '0;
    l3_axi_aw = '0;
    l3_axi_w = '0;
    uc_to_l3_axi_b = '0;
    uc_to_l3_axi_r = '0;
    uc_to_sys_axi_b = '0;
    uc_to_sys_axi_r = '0;
    sys_axi_ar = '0;
    sys_axi_ar.id = sys_id_stim_i;
    sys_axi_ar.addr = sys_addr_stim_i;
    sys_axi_ar.len = sys_len_stim_i;
    sys_axi_ar.size = sys_size_stim_i;
    sys_axi_ar.burst = 2'b01;
    sys_axi_aw = '0;
    sys_axi_aw.id = sys_id_stim_i;
    sys_axi_aw.addr = sys_addr_stim_i;
    sys_axi_aw.len = sys_len_stim_i;
    sys_axi_aw.size = sys_size_stim_i;
    sys_axi_aw.burst = 2'b01;
    sys_axi_w = '0;
    sys_axi_w.data = sys_wdata_stim_i;
    sys_axi_w.strb = '1;
    sys_axi_w.last = 1'b1;
  end

  shire_top #(
    .DisableMinions              ('1),
    .StubMinions                 ('1)
  ) u_dut (
    .clk_shire_i                 (clk_i),
    .clk_debug_i                 (clk_i),
    .clk_neigh_i                 ({NumNeigh{clk_i}}),
    .clk_noc_i                   (clk_i),
    .rst_cold_ni                 (rst_ni & rst_cold_ext_ni),
    .rst_warm_ni                 (rst_ni & rst_warm_ext_ni),
    .rst_system_ni               (rst_ni & rst_system_ext_ni),
    .rst_system_debug_ni         (rst_ni & rst_system_debug_ext_ni),
    .rst_noc_ni                  (rst_ni & rst_noc_ext_ni),
    .rst_system_lv_no            (rst_system_lv_no_o),
    .rst_system_debug_lv_no      (rst_system_debug_lv_no_o),
    .dft_hv_i                    (dft_hv),
    .dft_lv_i                    (dft_lv),
    .dft_sram_clk_i,
    .dft_mbist_en_i,
    .dmctrl_i                    ('0),
    .bpam_run_control_i          (bpam_run_control),
    .shire_phy_id_i,
    .shire_id_reset_val_i,
    .plic_mtip_i,
    .plic_meip_i,
    .plic_seip_i,
    .debug_and_or_tree_l2_i,
    .debug_and_or_tree_l2_o,
    .ioshire_combined_err_int_o,
    .ioshire_noc_err_int_o,
    .noc_err_int_srcs_i,
    .noc_all_err_int_srcs_o,
    .status_monitor_bank_sel_i   (status_monitor_bank_sel_stim_i),
    .to_l3_axi_ar_o              (),
    .to_l3_axi_ar_valid_o        (),
    .to_l3_axi_ar_ready_i        ('1),
    .to_l3_axi_aw_o              (),
    .to_l3_axi_aw_valid_o        (),
    .to_l3_axi_aw_ready_i        ('1),
    .to_l3_axi_w_o               (),
    .to_l3_axi_w_valid_o         (),
    .to_l3_axi_w_ready_i         ('1),
    .to_l3_axi_b_i               (to_l3_axi_b),
    .to_l3_axi_b_valid_i         ('0),
    .to_l3_axi_b_ready_o         (),
    .to_l3_axi_r_i               (to_l3_axi_r),
    .to_l3_axi_r_valid_i         ('0),
    .to_l3_axi_r_ready_o         (),
    .to_sys_axi_ar_o             (),
    .to_sys_axi_ar_valid_o       (),
    .to_sys_axi_ar_ready_i       ('1),
    .to_sys_axi_aw_o             (),
    .to_sys_axi_aw_valid_o       (),
    .to_sys_axi_aw_ready_i       ('1),
    .to_sys_axi_w_o              (),
    .to_sys_axi_w_valid_o        (),
    .to_sys_axi_w_ready_i        ('1),
    .to_sys_axi_b_i              (to_sys_axi_b),
    .to_sys_axi_b_valid_i        ('0),
    .to_sys_axi_b_ready_o        (),
    .to_sys_axi_r_i              (to_sys_axi_r),
    .to_sys_axi_r_valid_i        ('0),
    .to_sys_axi_r_ready_o        (),
    .l3_axi_ar_i                 (l3_axi_ar),
    .l3_axi_ar_valid_i           ('0),
    .l3_axi_ar_ready_o           (),
    .l3_axi_aw_i                 (l3_axi_aw),
    .l3_axi_aw_valid_i           ('0),
    .l3_axi_aw_ready_o           (),
    .l3_axi_w_i                  (l3_axi_w),
    .l3_axi_w_valid_i            ('0),
    .l3_axi_w_ready_o            (),
    .l3_axi_b_o                  (),
    .l3_axi_b_valid_o            (),
    .l3_axi_b_ready_i            ('1),
    .l3_axi_r_o                  (),
    .l3_axi_r_valid_o            (),
    .l3_axi_r_ready_i            ('1),
    .uc_to_l3_axi_ar_o           (),
    .uc_to_l3_axi_ar_valid_o     (),
    .uc_to_l3_axi_ar_ready_i     (1'b1),
    .uc_to_l3_axi_aw_o           (),
    .uc_to_l3_axi_aw_valid_o     (),
    .uc_to_l3_axi_aw_ready_i     (1'b1),
    .uc_to_l3_axi_w_o            (),
    .uc_to_l3_axi_w_valid_o      (),
    .uc_to_l3_axi_w_ready_i      (1'b1),
    .uc_to_l3_axi_b_i            (uc_to_l3_axi_b),
    .uc_to_l3_axi_b_valid_i      (1'b0),
    .uc_to_l3_axi_b_ready_o      (),
    .uc_to_l3_axi_r_i            (uc_to_l3_axi_r),
    .uc_to_l3_axi_r_valid_i      (1'b0),
    .uc_to_l3_axi_r_ready_o      (),
    .uc_to_sys_axi_ar_o          (),
    .uc_to_sys_axi_ar_valid_o    (),
    .uc_to_sys_axi_ar_ready_i    (1'b1),
    .uc_to_sys_axi_aw_o          (),
    .uc_to_sys_axi_aw_valid_o    (),
    .uc_to_sys_axi_aw_ready_i    (1'b1),
    .uc_to_sys_axi_w_o           (),
    .uc_to_sys_axi_w_valid_o     (),
    .uc_to_sys_axi_w_ready_i     (1'b1),
    .uc_to_sys_axi_b_i           (uc_to_sys_axi_b),
    .uc_to_sys_axi_b_valid_i     (1'b0),
    .uc_to_sys_axi_b_ready_o     (),
    .uc_to_sys_axi_r_i           (uc_to_sys_axi_r),
    .uc_to_sys_axi_r_valid_i     (1'b0),
    .uc_to_sys_axi_r_ready_o     (),
    .sys_axi_ar_i                (sys_axi_ar),
    .sys_axi_ar_valid_i          (sys_axi_ar_valid_stim_i),
    .sys_axi_ar_ready_o          (sys_axi_ar_ready_obs_o),
    .sys_axi_aw_i                (sys_axi_aw),
    .sys_axi_aw_valid_i          (sys_axi_aw_valid_stim_i),
    .sys_axi_aw_ready_o          (sys_axi_aw_ready_obs_o),
    .sys_axi_w_i                 (sys_axi_w),
    .sys_axi_w_valid_i           (sys_axi_w_valid_stim_i),
    .sys_axi_w_ready_o           (sys_axi_w_ready_obs_o),
    .sys_axi_b_o                 (sys_axi_b),
    .sys_axi_b_valid_o           (sys_axi_b_valid_obs_o),
    .sys_axi_b_ready_i           (sys_axi_b_ready_stim_i),
    .sys_axi_r_o                 (sys_axi_r),
    .sys_axi_r_valid_o           (sys_axi_r_valid_obs_o),
    .sys_axi_r_ready_i           (sys_axi_r_ready_stim_i),
    .sys_axi_aw_vcvalid_i        (sys_axi_aw_vcvalid_stim_i),
    .sys_axi_aw_credit_o         (sys_axi_aw_credit),
    .sys_axi_w_vcvalid_i         (sys_axi_w_vcvalid_stim_i),
    .sc_neigh_l2hpf_req_valid_o  (),
    .sc_neigh_l2hpf_req_info_o   (),
    .sc_trace_data_o             (),
    .sc_trace_valid_o            (),
    .ram_cfg_o                   (),
    .esr_clk_gate_ctrl_o         (),
    .esr_debug_clk_gate_ctrl_o   (),
    .esr_pll_ctrl_o              (),
    .esr_pll_conf_o              (),
    .esr_dll_ctrl_o              (),
    .esr_dll_conf_o              (),
    .esr_shire_ctrl_clockmux_o   ()
  );

  assign rst_c_shire_no_o = u_dut.rst_c_shire_n;
  assign rst_w_shire_no_o = u_dut.rst_w_shire_n;
  assign rst_d_shire_no_o = u_dut.rst_d_shire_n;
  assign rst_warm_to_neigh_no_o = u_dut.rst_warm_to_neigh_n;
  assign rst_w_icache_no_o = u_dut.rst_w_icache_n;
  assign rst_noc_lv_no_o = u_dut.rst_noc_lv_n;
  assign clk_neigh_obs_o = u_dut.clk_neigh;
  assign clk_shire_obs_o = u_dut.clk_shire;
  assign shire_id_o = u_dut.shire_id;
  assign tbox_id_o = u_dut.tbox_id;
  assign tbox_en_o = u_dut.tbox_en;
  assign plic_meip_to_neigh_o = u_dut.plic_meip_to_neigh;
  assign plic_seip_to_neigh_o = u_dut.plic_seip_to_neigh;
  assign int_mtip_flat_o = u_dut.int_mtip_to_neigh;
  assign neigh_sc_req_ready_o = u_dut.neigh_sc_req_ready;
  assign neigh_sc_rsp_valid_o = u_dut.neigh_sc_rsp_valid;
  assign icache_req_ready_o = u_dut.icache_req_ready;
  assign icache_resp_valid_o = u_dut.icache_resp_valid;
  assign flb_l2_neigh_resp_valid_o = u_dut.flb_l2_neigh_resp_valid;
  assign flb_l2_neigh_resp_data_o = u_dut.flb_l2_neigh_resp_data;
  assign uc_to_neigh_fcc_o = u_dut.uc_to_neigh_fcc;
  assign uc_to_neigh_fcc_target_o = u_dut.uc_to_neigh_fcc_target;
  assign pwr_ctrl_glb_nsleepin_o = u_dut.esr_pwr_ctrl_glb_nsleepin;
  assign pwr_ctrl_glb_iso_o = u_dut.esr_pwr_ctrl_glb_iso;
  assign pwr_ctrl_neigh_nsleepin_o = u_dut.esr_pwr_ctrl_neigh_nsleepin;
  assign pwr_ctrl_neigh_iso_o = u_dut.esr_pwr_ctrl_neigh_iso;
  assign ram_cfg_flat_o = u_dut.u_channel_wrap.ram_cfg_o;
  assign dft_hv_flat_o = u_dut.gen_neigh_top[0].u_neigh_top.dft_hv_i;
  assign debug_and_or_tree_l2_flat_o = debug_and_or_tree_l2_o;
  assign sys_axi_b_id_obs_o = sys_axi_b.id;
  assign sys_axi_b_resp_obs_o = sys_axi_b.resp;
  assign sys_axi_r_data_lo_obs_o = sys_axi_r.data[63:0];
  assign sys_axi_aw_credit_obs_o = sys_axi_aw_credit;

  for (genvar apb_i = 0; apb_i < NumNeigh; apb_i++) begin : gen_apb_obs
    assign apb_neigh_psel_o[apb_i] = u_dut.apb_esr_req_neigh[apb_i].apb_psel;
    assign apb_neigh_pwrite_o[apb_i] = u_dut.apb_esr_req_neigh[apb_i].apb_pwrite;
  end
  assign apb_neigh0_paddr_o = u_dut.apb_esr_req_neigh[0].apb_paddr;
  assign apb_neigh0_pwdata_o = u_dut.apb_esr_req_neigh[0].apb_pwdata;

  for (genvar apb_i = 0; apb_i < ApbSlaves; apb_i++) begin : gen_channel_apb_obs
    assign apb_channel_psel_o[apb_i] = u_dut.u_channel_wrap.channel_apb_req[apb_i].psel;
    assign apb_channel_pwrite_o[apb_i] = u_dut.u_channel_wrap.channel_apb_req[apb_i].pwrite;
  end

  assign coop_slv_valid_o = u_dut.coop_tload_slv_rdy_in_valid;
  assign coop_done_valid_o = u_dut.coop_tload_mst_done_in_valid;

endmodule : shire_top_tb
