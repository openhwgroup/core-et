// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Retained original-wiring slice for the project-native shire_top cosim.
//
// The full CORE-ET public shire_top at ORIG_ROOT/rtl/shire/shiretop/shire_top.v
// depends on generated mesh stops, third-party debug fabrics, sensor, hard
// PLL/DLL, TDR/OCC, and scan-hub surfaces that the native compute-shire
// contract intentionally omits.  This slice keeps the retained Ainekko-owned
// top-shell wiring shape: one shire channel shell, quiet/stub neighborhood
// feedback, and the native top observations that remain meaningful with the
// removed surfaces held idle.  It is intentionally a slice, not the new
// shire_top DUT: the new side instantiates `shire_top`, while this side
// instantiates `shire_channel_wrap` directly and models the all-stub
// neighborhood feedback used by the native smoke/cosim configuration.

/* verilator lint_off UNUSEDSIGNAL */  // Original slice keeps observation-only fanout signals for cosim comparison.
/* verilator lint_off PINCONNECTEMPTY */  // Retained slice intentionally leaves unobserved native payloads open.
/* verilator lint_off VARHIDDEN */  // Local constants intentionally mirror package topology constants.
module orig_shire_top_retained_slice
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
  import neigh_ch_apb_mux_pkg::*;
  import neigh_voltage_cross_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
  import shire_channel_leaves_pkg::*;
  import shire_esr_pkg::*;
  import shire_sbm_pkg::*;
  import shire_uncached_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic rst_cold_ext_ni,
  input  logic rst_warm_ext_ni,
  input  logic rst_system_ext_ni,
  input  logic rst_system_debug_ext_ni,

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
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0]
      uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_iso_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]
      pwr_ctrl_neigh_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]
      pwr_ctrl_neigh_iso_o,
  output logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0] ram_cfg_flat_o,
  output logic [$bits(dft_pkg::dft_t)-1:0] dft_hv_flat_o,
  output logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0]
      debug_and_or_tree_l2_flat_o,
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
  localparam int unsigned NumUc = 1;
  localparam int unsigned NumPorts = NumNeigh + NumRbox;
  localparam int unsigned NumL3MasterPorts = shirecache_pkg::L3MasterPorts;
  localparam int unsigned NumL3SlavePorts = shirecache_pkg::L3SlavePorts;
  localparam int unsigned NumSysPorts = shirecache_pkg::SysPorts;
  localparam int unsigned ThreadsPerNeigh = shire_esr_pkg::ThreadsPerShire / NumNeigh;
  localparam int unsigned NumMinionsPerShire = NumNeigh * shire_esr_pkg::MinPerNeigh;
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
  shire_channel_leaves_pkg::esr_and_or_tree_l2_t debug_and_or_tree_l2_o;
  esr_pkg::esr_and_or_tree_l0_t esr_and_or_tree_l0 [NumNeigh-1:0];
  esr_pkg::esr_dll_dly_est_sts_t esr_dll_dly_est_sts [NumNeigh-1:0];
  assign debug_and_or_tree_l2_i = debug_and_or_tree_l2_stim_i;
  assign esr_and_or_tree_l0 = '{default: '0};
  assign esr_dll_dly_est_sts = '{default: '0};

  axi_pkg::sc_master_ar_t [NumL3MasterPorts-1:0] to_l3_axi_ar;
  axi_pkg::sc_master_aw_t [NumL3MasterPorts-1:0] to_l3_axi_aw;
  axi_pkg::sc_master_w_t [NumL3MasterPorts-1:0] to_l3_axi_w;
  axi_pkg::sc_master_b_t [NumL3MasterPorts-1:0] to_l3_axi_b;
  axi_pkg::sc_master_r_t [NumL3MasterPorts-1:0] to_l3_axi_r;
  axi_pkg::sc_master_ar_t [NumSysPorts-1:0] to_sys_axi_ar;
  axi_pkg::sc_master_aw_t [NumSysPorts-1:0] to_sys_axi_aw;
  axi_pkg::sc_master_w_t [NumSysPorts-1:0] to_sys_axi_w;
  axi_pkg::sc_master_b_t [NumSysPorts-1:0] to_sys_axi_b;
  axi_pkg::sc_master_r_t [NumSysPorts-1:0] to_sys_axi_r;
  axi_pkg::sc_slave_ar_t [NumL3SlavePorts-1:0] l3_axi_ar;
  axi_pkg::sc_slave_aw_t [NumL3SlavePorts-1:0] l3_axi_aw;
  axi_pkg::sc_master_w_t [NumL3SlavePorts-1:0] l3_axi_w;
  axi_pkg::sc_slave_b_t [NumL3SlavePorts-1:0] l3_axi_b;
  axi_pkg::sc_slave_r_t [NumL3SlavePorts-1:0] l3_axi_r;
  axi_pkg::sc_master_ar_t uc_to_l3_axi_ar;
  axi_pkg::sc_master_aw_t uc_to_l3_axi_aw;
  axi_pkg::sc_master_w_t uc_to_l3_axi_w;
  axi_pkg::sc_master_b_t uc_to_l3_axi_b;
  axi_pkg::sc_master_r_t uc_to_l3_axi_r;
  axi_pkg::sc_master_ar_t uc_to_sys_axi_ar;
  axi_pkg::sc_master_aw_t uc_to_sys_axi_aw;
  axi_pkg::sc_master_w_t uc_to_sys_axi_w;
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

  logic [NumNeigh-1:0] clk_neigh;
  logic [NumNeigh-1:0] clk_shire_to_neigh;
  logic clk_shire;
  logic clk_shire_debug;
  logic [NumNeigh-1:0] rst_c_shire_scs_n;
  logic rst_sc_n;
  logic rst_rbox_n;
  esr_pkg::esr_ms_dmctrl_t dmctrl_to_neigh [NumNeigh-1:0];
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control_neigh [NumNeigh-1:0];
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] int_mtip_to_neigh;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread0_enable;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread1_enable;
  logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_msip;
  logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_redirect_trigger;
  minion_pkg::esr_minion_features_t esr_minion_features [NumNeigh-1:0];
  icache_pkg::icache_prefetch_conf_t esr_icache_prefetch_conf [NumNeigh-1:0];
  logic [shire_esr_pkg::IcachePerShire-1:0] esr_icache_prefetch_start;
  logic [NumNeigh-1:0] esr_shire_coop_mode;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t esr_neigh_clk_gate_ctrl [NumNeigh-1:0];
  logic [NumNeigh-1:0][minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override;
  esr_pkg::esr_dll_dly_est_ctl_t esr_dll_dly_est_ctl [NumNeigh-1:0];
  shire_esr_pkg::esr_clk_gate_ctrl_t esr_clk_gate_ctrl;
  logic esr_debug_clk_gate_ctrl;
  shire_esr_pkg::esr_pll_auto_config_t esr_pll_ctrl;
  logic [shire_esr_pkg::ShirePllConfBits-1:0] esr_pll_conf;
  shire_esr_pkg::esr_dll_auto_config_t esr_dll_ctrl;
  logic [shire_esr_pkg::ShireDllConfBits-1:0] esr_dll_conf;
  shire_esr_pkg::esr_clkmux_ctl_t esr_shire_ctrl_clockmux;
  shirecache_pkg::neigh_l2hpf_req_t [NumBanks-1:0] sc_neigh_l2hpf_req_info;
  shirecache_pkg::trace_packet_t sc_trace_data;
  logic [NumBanks-1:0] sc_neigh_l2hpf_req_valid;
  logic sc_trace_valid;

  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_esr_req_neigh [NumNeigh-1:0];
  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_esr_rsp_neigh [NumNeigh-1:0];
  etlink_pkg::rsp_t [NumNeigh-1:0] neigh_sc_rsp_info;
  logic [NumNeigh-1:0][NumBanks+NumUc-1:0] neigh_sc_req_valid;
  etlink_pkg::req_t [NumNeigh-1:0] neigh_sc_req_info;
  logic [NumNeigh-1:0] neigh_sc_rsp_ready;
  logic [NumNeigh-1:0] flb_neigh_l2_req_valid;
  logic [NumNeigh-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack [NumNeigh-1:0];
  logic [NumNeigh-1:0] icache_req_write;
  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr;
  logic [NumNeigh-1:0] icache_req_valid;
  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout;
  logic [NumNeigh-1:0] icache_resp_ready;
  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_out_data [NumNeigh-1:0];
  logic [NumNeigh-1:0] coop_tload_slv_rdy_out_valid;
  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_in_data [NumNeigh-1:0][NumNeigh-2:0];
  logic [NumNeigh-1:0][NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0]
      coop_tload_mst_done_in_coop_id;
  logic [NumNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id;
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_mst_done_out_valid;
  ram_cfg_pkg::ram_cfg_t ram_cfg;

  always_comb begin
    apb_esr_rsp_neigh = '{default: '0};
    neigh_sc_req_valid = '0;
    neigh_sc_req_info = '0;
    neigh_sc_rsp_ready = '0;
    flb_neigh_l2_req_valid = '0;
    flb_neigh_l2_req_data = '0;
    bpam_rc_tbox_ack = '{default: '0};
    icache_req_write = '0;
    icache_req_addr = '0;
    icache_req_valid = '0;
    icache_resp_ready = '0;
    coop_tload_slv_rdy_out_data = '{default: '0};
    coop_tload_slv_rdy_out_valid = '0;
    coop_tload_mst_done_out_coop_id = '0;
    coop_tload_mst_done_out_valid = '0;
  end

  shire_channel_wrap #(
    .NumNeigh             (NumNeigh),
    .NumRbox              (NumRbox),
    .NumBanks             (NumBanks),
    .NumUc                (NumUc),
    .NumPorts             (NumPorts),
    .NumL3MasterPorts     (NumL3MasterPorts),
    .NumL3SlavePorts      (NumL3SlavePorts),
    .NumSysPorts          (NumSysPorts),
    .ThreadsPerNeigh      (ThreadsPerNeigh),
    .NumMinionsPerShire   (NumMinionsPerShire),
    .NumGlobPwrDomains    (NumNeigh),
    .NumBankReqqEntries   (shirecache_pkg::ReqqDepth),
    .SetsPerSubBankParam  (shirecache_pkg::SetsPerSubBank),
    .UseNocClkGating      (1'b1),
    .IcacheMemsImplemented(1'b1),
    .L2hpfImplemented     (1'b0),
    .ChannelApbSlaves     (ApbSlaves),
    .SbmApbSlaves         (shire_sbm_pkg::ShireApbInterfaces),
    .DebugResetPulseCycles(32)
  ) u_orig_channel_wrap (
    .clk_ctrl_i                  (clk_i),
    .clk_ref_i                   (clk_i),
    .clk_step_i                  (clk_i),
    .clk_pll_i                   ({4{clk_i}}),
    .clk_dll_i                   (clk_i),
    .clk_noc_i                   (clk_i),
    .rst_cold_ni                 (rst_ni & rst_cold_ext_ni),
    .rst_warm_ni                 (rst_ni & rst_warm_ext_ni),
    .rst_system_ni               (rst_ni & rst_system_ext_ni),
    .rst_system_debug_ni         (rst_ni & rst_system_debug_ext_ni),
    .rst_system_lv_no              (rst_system_lv_no_o),
    .rst_system_debug_lv_no        (rst_system_debug_lv_no_o),
    .clk_neigh_o                 (clk_neigh),
    .clk_shire_to_neigh_o        (clk_shire_to_neigh),
    .clk_shire_o                 (clk_shire),
    .clk_shire_debug_o           (clk_shire_debug),
    .rst_c_shire_no              (rst_c_shire_no_o),
    .rst_d_shire_no              (rst_d_shire_no_o),
    .rst_w_shire_no              (rst_w_shire_no_o),
    .rst_c_shire_scs_no          (rst_c_shire_scs_n),
    .rst_warm_to_neigh_no        (rst_warm_to_neigh_no_o),
    .rst_w_icache_ni             ('0),
    .rst_sc_no                   (rst_sc_n),
    .rst_rbox_no                 (rst_rbox_n),
    .dft_hv_i                    (dft_hv),
    .dft_lv_i                    (dft_lv),
    .dft_sram_clk_i,
    .dft_mbist_en_i,
    .dmctrl_i                    ('0),
    .dmctrl_to_neigh_o           (dmctrl_to_neigh),
    .bpam_run_control_i          (bpam_run_control),
    .bpam_run_control_neigh_o    (bpam_run_control_neigh),
    .shire_phy_id_i,
    .shire_id_reset_val_i,
    .shire_id_o,
    .tbox_id_o,
    .tbox_en_o,
    .plic_mtip_i,
    .plic_meip_i,
    .plic_seip_i,
    .int_mtip_to_neigh_o         (int_mtip_to_neigh),
    .plic_meip_to_neigh_o,
    .plic_seip_to_neigh_o,
    .esr_thread0_enable_o        (esr_thread0_enable),
    .esr_thread1_enable_o        (esr_thread1_enable),
    .esr_neigh_ipi_msip_o        (esr_neigh_ipi_msip),
    .esr_neigh_ipi_redirect_trigger_o(esr_neigh_ipi_redirect_trigger),
    .esr_minion_features_o       (esr_minion_features),
    .esr_icache_prefetch_conf_o  (esr_icache_prefetch_conf),
    .esr_icache_prefetch_start_o (esr_icache_prefetch_start),
    .esr_icache_prefetch_done_i  ('0),
    .esr_neigh_icache_err_detected_i('0),
    .esr_neigh_icache_err_logged_i('0),
    .esr_shire_coop_mode_o       (esr_shire_coop_mode),
    .esr_and_or_tree_l0_i        (esr_and_or_tree_l0),
    .debug_and_or_tree_l2_i,
    .debug_and_or_tree_l2_o,
    .esr_neigh_clk_gate_ctrl_o   (esr_neigh_clk_gate_ctrl),
    .esr_minion_mem_override_o   (esr_minion_mem_override),
    .esr_pwr_ctrl_glb_nsleepin_o (pwr_ctrl_glb_nsleepin_o),
    .esr_pwr_ctrl_glb_iso_o      (pwr_ctrl_glb_iso_o),
    .esr_pwr_ctrl_glb_nsleepout_i('0),
    .esr_pwr_ctrl_neigh_nsleepin_o(pwr_ctrl_neigh_nsleepin_o),
    .esr_pwr_ctrl_neigh_iso_o    (pwr_ctrl_neigh_iso_o),
    .esr_pwr_ctrl_neigh_nsleepout_i('0),
    .esr_dll_dly_est_ctl_o       (esr_dll_dly_est_ctl),
    .esr_dll_dly_est_sts_i       (esr_dll_dly_est_sts),
    .status_monitor_bank_sel_i   ('0),
    .apb_esr_req_neigh_o         (apb_esr_req_neigh),
    .apb_esr_rsp_neigh_i         (apb_esr_rsp_neigh),
    .neigh_sc_req_ready_o        (neigh_sc_req_ready_o),
    .neigh_sc_req_valid_i        (neigh_sc_req_valid),
    .neigh_sc_req_info_i         (neigh_sc_req_info),
    .neigh_sc_rsp_ready_i        (neigh_sc_rsp_ready),
    .neigh_sc_rsp_valid_o        (neigh_sc_rsp_valid_o),
    .neigh_sc_rsp_info_o         (neigh_sc_rsp_info),
    .uc_to_neigh_fcc_o,
    .uc_to_neigh_fcc_target_o,
    .flb_neigh_l2_req_valid_i    (flb_neigh_l2_req_valid),
    .flb_neigh_l2_req_data_i     (flb_neigh_l2_req_data),
    .flb_l2_neigh_resp_valid_o,
    .flb_l2_neigh_resp_data_o,
    .bpam_rc_tbox_ack_i          (bpam_rc_tbox_ack),
    .ioshire_combined_err_int_o,
    .ioshire_noc_err_int_o,
    .noc_err_int_srcs_i,
    .noc_all_err_int_srcs_o,
    .to_l3_axi_ar_o              (to_l3_axi_ar),
    .to_l3_axi_ar_valid_o        (),
    .to_l3_axi_ar_ready_i        ('1),
    .to_l3_axi_aw_o              (to_l3_axi_aw),
    .to_l3_axi_aw_valid_o        (),
    .to_l3_axi_aw_ready_i        ('1),
    .to_l3_axi_w_o               (to_l3_axi_w),
    .to_l3_axi_w_valid_o         (),
    .to_l3_axi_w_ready_i         ('1),
    .to_l3_axi_b_i               (to_l3_axi_b),
    .to_l3_axi_b_valid_i         ('0),
    .to_l3_axi_b_ready_o         (),
    .to_l3_axi_r_i               (to_l3_axi_r),
    .to_l3_axi_r_valid_i         ('0),
    .to_l3_axi_r_ready_o         (),
    .to_sys_axi_ar_o             (to_sys_axi_ar),
    .to_sys_axi_ar_valid_o       (),
    .to_sys_axi_ar_ready_i       ('1),
    .to_sys_axi_aw_o             (to_sys_axi_aw),
    .to_sys_axi_aw_valid_o       (),
    .to_sys_axi_aw_ready_i       ('1),
    .to_sys_axi_w_o              (to_sys_axi_w),
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
    .l3_axi_b_o                  (l3_axi_b),
    .l3_axi_b_valid_o            (),
    .l3_axi_b_ready_i            ('1),
    .l3_axi_r_o                  (l3_axi_r),
    .l3_axi_r_valid_o            (),
    .l3_axi_r_ready_i            ('1),
    .uc_to_l3_axi_ar_o           (uc_to_l3_axi_ar),
    .uc_to_l3_axi_ar_valid_o     (),
    .uc_to_l3_axi_ar_ready_i     (1'b1),
    .uc_to_l3_axi_aw_o           (uc_to_l3_axi_aw),
    .uc_to_l3_axi_aw_valid_o     (),
    .uc_to_l3_axi_aw_ready_i     (1'b1),
    .uc_to_l3_axi_w_o            (uc_to_l3_axi_w),
    .uc_to_l3_axi_w_valid_o      (),
    .uc_to_l3_axi_w_ready_i      (1'b1),
    .uc_to_l3_axi_b_i            (uc_to_l3_axi_b),
    .uc_to_l3_axi_b_valid_i      (1'b0),
    .uc_to_l3_axi_b_ready_o      (),
    .uc_to_l3_axi_r_i            (uc_to_l3_axi_r),
    .uc_to_l3_axi_r_valid_i      (1'b0),
    .uc_to_l3_axi_r_ready_o      (),
    .uc_to_sys_axi_ar_o          (uc_to_sys_axi_ar),
    .uc_to_sys_axi_ar_valid_o    (),
    .uc_to_sys_axi_ar_ready_i    (1'b1),
    .uc_to_sys_axi_aw_o          (uc_to_sys_axi_aw),
    .uc_to_sys_axi_aw_valid_o    (),
    .uc_to_sys_axi_aw_ready_i    (1'b1),
    .uc_to_sys_axi_w_o           (uc_to_sys_axi_w),
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
    .icache_req_write_i          (icache_req_write),
    .icache_req_addr_i           (icache_req_addr),
    .icache_req_valid_i          (icache_req_valid),
    .icache_req_ready_o          (icache_req_ready_o),
    .icache_resp_dout_o          (icache_resp_dout),
    .icache_resp_valid_o         (icache_resp_valid_o),
    .icache_resp_ready_i         (icache_resp_ready),
    .sc_neigh_l2hpf_req_valid_o  (sc_neigh_l2hpf_req_valid),
    .sc_neigh_l2hpf_req_info_o   (sc_neigh_l2hpf_req_info),
    .sc_trace_data_o             (sc_trace_data),
    .sc_trace_valid_o            (sc_trace_valid),
    .coop_tload_slv_rdy_out_data_i(coop_tload_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_i(coop_tload_slv_rdy_out_valid),
    .coop_tload_slv_rdy_in_data_o(coop_tload_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_o(coop_slv_valid_o),
    .coop_tload_mst_done_out_coop_id_i(coop_tload_mst_done_out_coop_id),
    .coop_tload_mst_done_out_valid_i(coop_tload_mst_done_out_valid),
    .coop_tload_mst_done_in_coop_id_o(coop_tload_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_o(coop_done_valid_o),
    .ram_cfg_o                   (ram_cfg),
    .esr_clk_gate_ctrl_o         (esr_clk_gate_ctrl),
    .esr_debug_clk_gate_ctrl_o   (esr_debug_clk_gate_ctrl),
    .esr_pll_ctrl_o              (esr_pll_ctrl),
    .esr_pll_conf_o              (esr_pll_conf),
    .esr_dll_ctrl_o              (esr_dll_ctrl),
    .esr_dll_conf_o              (esr_dll_conf),
    .esr_shire_ctrl_clockmux_o   (esr_shire_ctrl_clockmux)
  );

  assign rst_w_icache_no_o = '0;
  assign clk_neigh_obs_o = clk_neigh;
  assign clk_shire_obs_o = clk_shire;
  assign int_mtip_flat_o = int_mtip_to_neigh;
  assign ram_cfg_flat_o = ram_cfg;
  assign dft_hv_flat_o = dft_hv;
  assign debug_and_or_tree_l2_flat_o = debug_and_or_tree_l2_o;
  assign sys_axi_b_id_obs_o = sys_axi_b.id;
  assign sys_axi_b_resp_obs_o = sys_axi_b.resp;
  assign sys_axi_r_data_lo_obs_o = sys_axi_r.data[63:0];
  assign sys_axi_aw_credit_obs_o = sys_axi_aw_credit;

  for (genvar apb_i = 0; apb_i < NumNeigh; apb_i++) begin : gen_apb_obs
    assign apb_neigh_psel_o[apb_i] = apb_esr_req_neigh[apb_i].apb_psel;
    assign apb_neigh_pwrite_o[apb_i] = apb_esr_req_neigh[apb_i].apb_pwrite;
  end
  assign apb_neigh0_paddr_o = apb_esr_req_neigh[0].apb_paddr;
  assign apb_neigh0_pwdata_o = apb_esr_req_neigh[0].apb_pwdata;

  for (genvar apb_i = 0; apb_i < ApbSlaves; apb_i++) begin : gen_channel_apb_obs
    assign apb_channel_psel_o[apb_i] = u_orig_channel_wrap.channel_apb_req[apb_i].psel;
    assign apb_channel_pwrite_o[apb_i] = u_orig_channel_wrap.channel_apb_req[apb_i].pwrite;
  end

endmodule : orig_shire_top_retained_slice
/* verilator lint_on VARHIDDEN */
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on UNUSEDSIGNAL */
