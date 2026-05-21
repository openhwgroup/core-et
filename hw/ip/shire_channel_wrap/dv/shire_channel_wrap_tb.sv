// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off VARHIDDEN */  // TB local constants mirror package defaults for concise port sizing.
/* verilator lint_off UNUSEDPARAM */  // Some derived TB dimensions document container topology.
/* verilator lint_off UNUSEDSIGNAL */  // TB stubs many container sideband ports not checked directly.
module shire_channel_wrap_tb
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

  input  logic rst_c_ext_ni,
  input  logic rst_w_ext_ni,
  input  logic rst_d_ext_ni,
  input  logic rst_system_ext_ni,
  input  logic rst_noc_ext_ni,
  input  logic [shire_esr_pkg::NumNeigh-1:0] rst_w_icache_ext_ni,

  input  logic dft_scanmode_i,
  input  logic dft_scan_reset_ni,
  input  logic dft_sram_clk_i,
  input  logic dft_sram_clk_override_i,
  input  logic dft_ram_rei_i,
  input  logic dft_ram_wei_i,
  input  logic dft_mbist_en_i,

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_id_reset_val_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_phy_id_i,

  input  logic [2:0] apb_sel_i,
  input  logic [shire_sbm_pkg::ShireApbAddrWidth-1:0] apb_paddr_i,
  input  logic apb_pwrite_i,
  input  logic apb_psel_i,
  input  logic apb_penable_i,
  input  logic [shire_sbm_pkg::ApbDataWidth-1:0] apb_pwdata_i,
  output logic apb_pready_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] apb_prdata_o,
  output logic apb_pslverr_o,

  input  logic [shire_channel_leaves_pkg::NocIntNum-1:0] noc_err_int_srcs_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_err_detected_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_err_logged_i,

  input  logic [shire_esr_pkg::NumNeigh-1:0] flb_neigh_l2_req_valid_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,

  input  logic neigh0_req_valid_i,
  input  logic [shirecache_pkg::Banks:0] neigh0_req_dest_i,
  input  logic [etlink_pkg::PaSize-1:0] neigh0_req_addr_i,
  input  logic [4:0] neigh0_req_opcode_i,
  input  logic [2:0] neigh0_req_size_i,
  input  logic [etlink_pkg::QwenSize-1:0] neigh0_req_qwen_i,

  input  logic [shire_esr_pkg::NumNeigh-1:0] icache_req_valid_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] icache_req_write_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] icache_resp_ready_i,

  input  logic [shire_esr_pkg::NumNeigh-1:0] coop_slv_valid_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_done_id_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_done_valid_i,

  input  logic [shire_esr_pkg::IcachePerShire-1:0] esr_icache_prefetch_done_stim_i,
  input  logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_and_or_tree_l0_t)-1:0]
      esr_and_or_tree_l0_flat_stim_i,
  input  logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0]
      debug_and_or_tree_l2_stim_i,
  input  logic [shire_esr_pkg::NumNeigh*$bits(neigh_hv_logic_pkg::bpam_rc_tbox_ack_t)-1:0]
      bpam_rc_tbox_ack_flat_stim_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] esr_pwr_ctrl_glb_nsleepout_stim_i,
  input  logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]
      esr_pwr_ctrl_neigh_nsleepout_stim_i,
  input  logic esr_pll_busy_stim_i,
  input  logic [shire_esr_pkg::ShirePllDataBits-1:0] esr_pll_rdata_stim_i,
  input  logic esr_pll_rrdy_stim_i,
  input  logic esr_pll_lock_stim_i,
  input  logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_dll_dly_est_sts_t)-1:0]
      esr_dll_dly_est_sts_flat_stim_i,
  input  logic esr_dll_busy_stim_i,
  input  logic [shire_esr_pkg::ShireDllDataBits-1:0] esr_dll_rdata_stim_i,
  input  logic esr_dll_rrdy_stim_i,
  input  logic esr_dll_lock_stim_i,
  input  logic [shirecache_pkg::BankIdSize-1:0] status_monitor_bank_sel_i,

  input  logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_rsp_ready_stim_i,
  input  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_ar_ready_stim_i,
  input  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_aw_ready_stim_i,
  input  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_w_ready_stim_i,
  input  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_b_valid_stim_i,
  input  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_r_valid_stim_i,
  input  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_ar_ready_stim_i,
  input  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_aw_ready_stim_i,
  input  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_w_ready_stim_i,
  input  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_b_valid_stim_i,
  input  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_r_valid_stim_i,
  input  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_ar_valid_stim_i,
  input  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_aw_valid_stim_i,
  input  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_w_valid_stim_i,
  input  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_b_ready_stim_i,
  input  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_r_ready_stim_i,
  input  logic uc_to_l3_axi_ar_ready_stim_i,
  input  logic uc_to_l3_axi_aw_ready_stim_i,
  input  logic uc_to_l3_axi_w_ready_stim_i,
  input  logic uc_to_l3_axi_b_valid_stim_i,
  input  logic uc_to_l3_axi_r_valid_stim_i,
  input  logic uc_to_sys_axi_ar_ready_stim_i,
  input  logic uc_to_sys_axi_aw_ready_stim_i,
  input  logic uc_to_sys_axi_w_ready_stim_i,
  input  logic uc_to_sys_axi_b_valid_stim_i,
  input  logic uc_to_sys_axi_r_valid_stim_i,
  input  logic sys_axi_ar_valid_stim_i,
  input  logic sys_axi_aw_valid_stim_i,
  input  logic sys_axi_w_valid_stim_i,
  input  logic sys_axi_b_ready_stim_i,
  input  logic sys_axi_r_ready_stim_i,
  input  logic [1:0] sys_axi_aw_vcvalid_stim_i,
  input  logic [1:0] sys_axi_w_vcvalid_stim_i,
  input  logic sbm_write_credit_return_stim_i,
  input  logic sbm_sys_axi_ar_ready_stim_i,
  input  logic sbm_sys_axi_aw_ready_stim_i,
  input  logic sbm_sys_axi_w_ready_stim_i,
  input  logic sbm_sys_axi_b_valid_stim_i,
  input  logic sbm_sys_axi_r_valid_stim_i,
  input  logic [31:0] axi_stim_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] sys_id_stim_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] sys_addr_stim_i,
  input  logic [axi_pkg::SysSlaveDataSize-1:0] sys_wdata_stim_i,
  input  logic [7:0] sys_len_stim_i,
  input  logic [2:0] sys_size_stim_i,

  output logic [shire_esr_pkg::NumNeigh-1:0] rst_c_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_w_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_d_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_c_shire_scs_no_o,
  output logic rst_sc_no_o,
  output logic rst_rbox_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] rst_warm_to_neigh_no_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_ms_dmctrl_t)-1:0] dmctrl_to_neigh_flat_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] plic_meip_obs_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] plic_seip_obs_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] shire_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0][1:0] tbox_id_flat_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0] tbox_en_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] esr_thread0_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] esr_thread1_enable_o,
  output logic [$bits(minion_pkg::esr_minion_features_t)-1:0] esr_minion_features_flat_o [shire_esr_pkg::NumNeigh],
  output logic [$bits(icache_pkg::icache_prefetch_conf_t)-1:0] esr_icache_prefetch_conf_flat_o [shire_esr_pkg::NumNeigh],
  output logic [shire_esr_pkg::IcachePerShire-1:0] esr_icache_prefetch_start_flat_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] esr_ipi_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] esr_ipi_redirect_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] esr_mtime_local_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] esr_shire_coop_mode_o,
  output logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0] debug_and_or_tree_l2_flat_o,
  output logic [$bits(neigh_voltage_cross_pkg::bpam_run_control_neigh_t)-1:0] bpam_run_control_neigh_flat_o [shire_esr_pkg::NumNeigh],
  output logic ioshire_log_err_int_o,
  output logic ioshire_noc_err_int_o,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0] noc_all_err_int_srcs_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] uc_to_neigh_fcc_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] flb_l2_neigh_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] flb_l2_neigh_resp_data_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shirecache_pkg::Banks:0] neigh_sc_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_rsp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] icache_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] icache_resp_valid_o,
  output logic [shirecache_pkg::Banks-1:0] sc_neigh_l2hpf_req_valid_flat_o,
  output logic sc_trace_valid_flat_o,
  output logic [$bits(dft_pkg::dft_t)-1:0] dft_hv_flat_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] pwr_ctrl_glb_iso_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] pwr_ctrl_neigh_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] pwr_ctrl_neigh_iso_o,
  output logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0] ram_cfg_flat_o,
  output logic [$bits(shire_esr_pkg::esr_clk_gate_ctrl_t)-1:0] clk_gate_ctrl_flat_o,
  output logic debug_clk_gate_ctrl_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_slv_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0]
      coop_done_id_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_done_valid_o,
  output logic sbm_enable_read_o,
  output logic sbm_enable_write_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] apb_neigh_psel_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] apb_neigh_pwrite_o,
  output logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] apb_neigh0_paddr_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] apb_neigh0_pwdata_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_psel_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_pwrite_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_pready_all_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] apb_channel_pslverr_all_o,
  output logic [(shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2)*shire_sbm_pkg::ApbDataWidth-1:0] apb_channel_prdata_all_flat_o,
  output logic [$bits(shire_esr_pkg::esr_pll_auto_config_t)-1:0] pll_ctrl_flat_o,
  output logic [shire_esr_pkg::ShirePllConfBits-1:0] pll_conf_o,
  output logic [$bits(shire_esr_pkg::esr_dll_auto_config_t)-1:0] dll_ctrl_flat_o,
  output logic [shire_esr_pkg::ShireDllConfBits-1:0] dll_conf_o,
  output logic [$bits(shire_esr_pkg::esr_clkmux_ctl_t)-1:0] shire_ctrl_clockmux_flat_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_dll_dly_est_ctl_t)-1:0] dll_dly_est_ctl_flat_o,
  output logic sys_axi_ar_ready_obs_o,
  output logic sys_axi_aw_ready_obs_o,
  output logic sys_axi_w_ready_obs_o,
  output logic sys_axi_b_valid_obs_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] sys_axi_b_id_obs_o,
  output logic [1:0] sys_axi_b_resp_obs_o,
  output logic sys_axi_r_valid_obs_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] sys_axi_r_id_obs_o,
  output logic [1:0] sys_axi_r_resp_obs_o,
  output logic [63:0] sys_axi_r_data_lo_obs_o,
  output logic rst_system_lv_no_o,
  output logic rst_system_debug_lv_no_o,
  output logic rst_noc_lv_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] clk_neigh_obs_o,
  output logic clk_shire_obs_o
);

  localparam int unsigned NumNeigh = shire_esr_pkg::NumNeigh;
  localparam int unsigned NumRbox = shirecache_pkg::RboxPerShire;
  localparam int unsigned NumPorts = NumNeigh + NumRbox;
  localparam int unsigned NumBanks = shirecache_pkg::Banks;
  localparam int unsigned NumL3MasterPorts = shirecache_pkg::L3MasterPorts;
  localparam int unsigned NumL3SlavePorts = shirecache_pkg::L3SlavePorts;
  localparam int unsigned NumSysPorts = shirecache_pkg::SysPorts;
  localparam int unsigned NumUc = 1;
  localparam int unsigned ThreadsPerNeigh = shire_esr_pkg::ThreadsPerShire / NumNeigh;
  localparam int unsigned ApbSlaves = NumBanks + NumRbox + 2;
  localparam int unsigned EsrAndOrTreeL0Bits = $bits(esr_pkg::esr_and_or_tree_l0_t);
  localparam int unsigned BpamRcTboxAckBits = $bits(neigh_hv_logic_pkg::bpam_rc_tbox_ack_t);
  localparam int unsigned DllDlyEstStsBits = $bits(esr_pkg::esr_dll_dly_est_sts_t);
  localparam int unsigned DllDlyEstCtlBits = $bits(esr_pkg::esr_dll_dly_est_ctl_t);
  localparam int unsigned DmctrlBits = $bits(esr_pkg::esr_ms_dmctrl_t);
  localparam logic [2:0] ApbSlavesSelLimit = ApbSlaves[2:0];

  logic rst_c_ni;
  logic rst_w_ni;
  logic rst_d_ni;
  logic rst_system_ni;
  logic [NumNeigh-1:0] rst_w_icache_ni;
  logic noc_rst_ni;
  logic noc_clk_i;
  assign rst_c_ni = rst_ni & rst_c_ext_ni;
  assign rst_w_ni = rst_ni & rst_w_ext_ni;
  assign rst_d_ni = rst_ni & rst_d_ext_ni;
  assign rst_system_ni = rst_ni & rst_system_ext_ni;
  assign rst_w_icache_ni = rst_w_icache_ext_ni;
  assign noc_rst_ni = rst_ni & rst_noc_ext_ni;
  assign noc_clk_i = clk_i;

  dft_pkg::dft_t dft_hv_i;
  dft_pkg::dft_t dft_lv_i;
  always_comb begin
    dft_hv_i.scanmode = dft_scanmode_i;
    dft_hv_i.scan_reset_n = dft_scan_reset_ni;
    dft_hv_i.sram_clk_override = dft_sram_clk_override_i;
    dft_hv_i.ram_rei = dft_ram_rei_i;
    dft_hv_i.ram_wei = dft_ram_wei_i;
    dft_lv_i = dft_hv_i;
  end

  shire_channel_leaves_pkg::bpam_run_control_t bpam_run_control_i;
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control_neigh_o [NumNeigh-1:0];
  always_comb begin
    bpam_run_control_i = '0;
    bpam_run_control_i.gpio.ndmreset = dft_scanmode_i;
    bpam_run_control_i.gpio.halt_req = dft_ram_rei_i;
    bpam_run_control_i.gpio.resume_req = dft_ram_wei_i;
    bpam_run_control_i.events.halt_req = dft_sram_clk_override_i;
    bpam_run_control_i.events.resume_req = dft_mbist_en_i;
  end

  logic [NumNeigh-1:0] rst_c_shire_no;
  logic [NumNeigh-1:0] rst_d_shire_no;
  logic [NumNeigh-1:0] rst_w_shire_no;
  logic [NumNeigh-1:0] rst_c_shire_scs_no;
  logic rst_sc_no;
  logic rst_rbox_no;
  logic [shire_esr_pkg::TboxPerShire-1:0][1:0] tbox_id_o;
  minion_pkg::esr_minion_features_t esr_minion_features_o [NumNeigh-1:0];
  icache_pkg::icache_prefetch_conf_t esr_icache_prefetch_conf_o [NumNeigh-1:0];
  logic [shire_esr_pkg::IcachePerShire-1:0] esr_icache_prefetch_start_o;
  logic [shire_esr_pkg::IcachePerShire-1:0] esr_icache_prefetch_done_i;
  esr_pkg::esr_and_or_tree_l0_t esr_and_or_tree_l0_i [NumNeigh-1:0];
  shire_channel_leaves_pkg::esr_and_or_tree_l2_t debug_and_or_tree_l2_i;
  shire_channel_leaves_pkg::esr_and_or_tree_l2_t debug_and_or_tree_l2_o;
  shire_esr_pkg::esr_clk_gate_ctrl_t esr_clk_gate_ctrl_o;
  logic esr_debug_clk_gate_ctrl_o;
  logic [NumNeigh-1:0] clk_neigh_o;
  logic [NumNeigh-1:0] clk_shire_to_neigh_o;
  logic clk_shire_o;
  logic clk_shire_debug_o;
  logic rst_system_lv_no;
  logic rst_system_debug_lv_no;
  logic rst_noc_lv_no;
  logic [NumNeigh-1:0] rst_warm_to_neigh_no;
  esr_pkg::esr_ms_dmctrl_t dmctrl_i;
  esr_pkg::esr_ms_dmctrl_t dmctrl_to_neigh_o [NumNeigh-1:0];
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] int_mtip_to_neigh_o;
  logic [NumNeigh-1:0] plic_meip_to_neigh_o;
  logic [NumNeigh-1:0] plic_seip_to_neigh_o;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread0_enable_neigh_o;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread1_enable_neigh_o;
  logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_msip_o;
  logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_redirect_trigger_o;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t esr_neigh_clk_gate_ctrl_o [NumNeigh-1:0];
  logic [NumNeigh-1:0][minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_o;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_esr_req_neigh_o [NumNeigh-1:0];
  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_esr_rsp_neigh_i [NumNeigh-1:0];
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepin_2d_o;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_iso_2d_o;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepout_2d_i;

  logic [NumNeigh-1:0][NumBanks+NumUc-1:0] neigh_sc_req_valid_i;
  etlink_pkg::req_t [NumNeigh-1:0] neigh_sc_req_info_i;
  logic [NumNeigh-1:0] neigh_sc_rsp_ready_i;
  etlink_pkg::rsp_t [NumNeigh-1:0] neigh_sc_rsp_info_o;

  shire_sbm_pkg::shire_apb_req_t [ApbSlaves-1:0] apb_req_i;
  shire_sbm_pkg::shire_apb_rsp_t [ApbSlaves-1:0] apb_rsp_o;
  logic [2:0] apb_sel_q;

  logic [NumNeigh-1:0] esr_pwr_ctrl_glb_nsleepin_o;
  logic [NumNeigh-1:0] esr_pwr_ctrl_glb_iso_o;
  logic [NumNeigh-1:0] esr_pwr_ctrl_glb_nsleepout_i;
  logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepin_o;
  logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_iso_o;
  logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepout_i;

  shire_esr_pkg::esr_pll_auto_config_t esr_pll_ctrl_o;
  logic [shire_esr_pkg::ShirePllConfBits-1:0] esr_pll_conf_o;
  logic esr_pll_busy_i;
  logic [shire_esr_pkg::ShirePllDataBits-1:0] esr_pll_rdata_i;
  logic esr_pll_rrdy_i;
  logic esr_pll_lock_i;
  shire_esr_pkg::esr_clk_dly_ctl_t esr_clk_dly_ctl_o;
  esr_pkg::esr_dll_dly_est_ctl_t esr_dll_dly_est_ctl_o [NumNeigh-1:0];
  esr_pkg::esr_dll_dly_est_sts_t esr_dll_dly_est_sts_i [NumNeigh-1:0];
  shire_esr_pkg::esr_clkmux_ctl_t esr_shire_ctrl_clockmux_o;
  shire_esr_pkg::esr_dll_auto_config_t esr_dll_ctrl_o;
  logic [shire_esr_pkg::ShireDllConfBits-1:0] esr_dll_conf_o;
  logic esr_dll_busy_i;
  logic [shire_esr_pkg::ShireDllDataBits-1:0] esr_dll_rdata_i;
  logic esr_dll_rrdy_i;
  logic esr_dll_lock_i;
  ram_cfg_pkg::ram_cfg_t ram_cfg_o;

  axi_pkg::sc_master_ar_t [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_ar_o;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_ar_valid_o;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_ar_ready_i;
  axi_pkg::sc_master_aw_t [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_aw_o;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_aw_valid_o;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_aw_ready_i;
  axi_pkg::sc_master_w_t [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_w_o;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_w_valid_o;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_w_ready_i;
  axi_pkg::sc_master_b_t [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_b_i;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_b_valid_i;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_b_ready_o;
  axi_pkg::sc_master_r_t [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_r_i;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_r_valid_i;
  logic [shirecache_pkg::L3MasterPorts-1:0] to_l3_axi_r_ready_o;

  axi_pkg::sc_master_ar_t [shirecache_pkg::SysPorts-1:0] to_sys_axi_ar_o;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_ar_valid_o;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_ar_ready_i;
  axi_pkg::sc_master_aw_t [shirecache_pkg::SysPorts-1:0] to_sys_axi_aw_o;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_aw_valid_o;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_aw_ready_i;
  axi_pkg::sc_master_w_t [shirecache_pkg::SysPorts-1:0] to_sys_axi_w_o;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_w_valid_o;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_w_ready_i;
  axi_pkg::sc_master_b_t [shirecache_pkg::SysPorts-1:0] to_sys_axi_b_i;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_b_valid_i;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_b_ready_o;
  axi_pkg::sc_master_r_t [shirecache_pkg::SysPorts-1:0] to_sys_axi_r_i;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_r_valid_i;
  logic [shirecache_pkg::SysPorts-1:0] to_sys_axi_r_ready_o;

  axi_pkg::sc_slave_ar_t [shirecache_pkg::L3SlavePorts-1:0] l3_axi_ar_i;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_ar_valid_i;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_ar_ready_o;
  axi_pkg::sc_slave_aw_t [shirecache_pkg::L3SlavePorts-1:0] l3_axi_aw_i;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_aw_valid_i;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_aw_ready_o;
  axi_pkg::sc_master_w_t [shirecache_pkg::L3SlavePorts-1:0] l3_axi_w_i;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_w_valid_i;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_w_ready_o;
  axi_pkg::sc_slave_b_t [shirecache_pkg::L3SlavePorts-1:0] l3_axi_b_o;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_b_valid_o;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_b_ready_i;
  axi_pkg::sc_slave_r_t [shirecache_pkg::L3SlavePorts-1:0] l3_axi_r_o;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_r_valid_o;
  logic [shirecache_pkg::L3SlavePorts-1:0] l3_axi_r_ready_i;

  axi_pkg::sc_master_ar_t uc_to_l3_axi_ar_o;
  logic uc_to_l3_axi_ar_valid_o;
  logic uc_to_l3_axi_ar_ready_i;
  axi_pkg::sc_master_aw_t uc_to_l3_axi_aw_o;
  logic uc_to_l3_axi_aw_valid_o;
  logic uc_to_l3_axi_aw_ready_i;
  axi_pkg::sc_master_w_t uc_to_l3_axi_w_o;
  logic uc_to_l3_axi_w_valid_o;
  logic uc_to_l3_axi_w_ready_i;
  axi_pkg::sc_master_b_t uc_to_l3_axi_b_i;
  logic uc_to_l3_axi_b_valid_i;
  logic uc_to_l3_axi_b_ready_o;
  axi_pkg::sc_master_r_t uc_to_l3_axi_r_i;
  logic uc_to_l3_axi_r_valid_i;
  logic uc_to_l3_axi_r_ready_o;

  axi_pkg::sc_master_ar_t uc_to_sys_axi_ar_o;
  logic uc_to_sys_axi_ar_valid_o;
  logic uc_to_sys_axi_ar_ready_i;
  axi_pkg::sc_master_aw_t uc_to_sys_axi_aw_o;
  logic uc_to_sys_axi_aw_valid_o;
  logic uc_to_sys_axi_aw_ready_i;
  axi_pkg::sc_master_w_t uc_to_sys_axi_w_o;
  logic uc_to_sys_axi_w_valid_o;
  logic uc_to_sys_axi_w_ready_i;
  axi_pkg::sc_master_b_t uc_to_sys_axi_b_i;
  logic uc_to_sys_axi_b_valid_i;
  logic uc_to_sys_axi_b_ready_o;
  axi_pkg::sc_master_r_t uc_to_sys_axi_r_i;
  logic uc_to_sys_axi_r_valid_i;
  logic uc_to_sys_axi_r_ready_o;

  axi_pkg::sys_slave_ar_t sys_axi_ar_i;
  logic sys_axi_ar_valid_i;
  logic sys_axi_ar_ready_o;
  axi_pkg::sys_slave_aw_t sys_axi_aw_i;
  logic sys_axi_aw_valid_i;
  logic sys_axi_aw_ready_o;
  axi_pkg::sys_slave_w_t sys_axi_w_i;
  logic sys_axi_w_valid_i;
  logic sys_axi_w_ready_o;
  axi_pkg::sys_slave_b_t sys_axi_b_o;
  logic sys_axi_b_valid_o;
  logic sys_axi_b_ready_i;
  axi_pkg::sys_slave_r_t sys_axi_r_o;
  logic sys_axi_r_valid_o;
  logic sys_axi_r_ready_i;
  logic [1:0] sys_axi_aw_vcvalid_i;
  logic [1:0] sys_axi_aw_credit_o;
  logic [1:0] sys_axi_w_vcvalid_i;

  logic sbm_write_credit_return_i;
  axi_pkg::sys_slave_ar_t sbm_sys_axi_ar_o;
  logic sbm_sys_axi_ar_ready_i;
  axi_pkg::sys_slave_aw_t sbm_sys_axi_aw_o;
  logic sbm_sys_axi_aw_ready_i;
  axi_pkg::sys_slave_w_t sbm_sys_axi_w_o;
  logic sbm_sys_axi_w_ready_i;
  axi_pkg::sys_slave_b_t sbm_sys_axi_b_i;
  logic sbm_sys_axi_b_valid_i;
  logic sbm_sys_axi_b_ready_o;
  axi_pkg::sys_slave_r_t sbm_sys_axi_r_i;
  logic sbm_sys_axi_r_valid_i;
  logic sbm_sys_axi_r_ready_o;

  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout_o;
  shirecache_pkg::neigh_l2hpf_req_t [NumBanks-1:0] sc_neigh_l2hpf_req_info_o;
  logic [NumBanks-1:0] sc_neigh_l2hpf_req_valid_o;
  shirecache_pkg::trace_packet_t sc_trace_data_o;
  logic sc_trace_valid_o;

  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_out_data_i [NumNeigh-1:0];
  logic [NumNeigh-1:0] coop_tload_slv_rdy_out_valid_i;
  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_in_data_o [NumNeigh-1:0][NumNeigh-2:0];
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_slv_rdy_in_valid_o;
  logic [NumNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id_i;
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_mst_done_out_valid_i;
  logic [NumNeigh-1:0][NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_o;
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_mst_done_in_valid_o;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      apb_sel_q <= '0;
    end else if (apb_psel_i && !apb_penable_i) begin
      apb_sel_q <= apb_sel_i;
    end
  end

  always_comb begin
    apb_req_i = u_dut.channel_apb_req;
    apb_rsp_o = u_dut.channel_apb_rsp;
    apb_pready_o = apb_rsp_o[apb_sel_q].pready;
    apb_prdata_o = apb_rsp_o[apb_sel_q].prdata;
    apb_pslverr_o = apb_rsp_o[apb_sel_q].pslverr;

    esr_icache_prefetch_done_i = esr_icache_prefetch_done_stim_i;
    debug_and_or_tree_l2_i = shire_channel_leaves_pkg::esr_and_or_tree_l2_t'(
        debug_and_or_tree_l2_stim_i);
    esr_pwr_ctrl_glb_nsleepout_i = esr_pwr_ctrl_glb_nsleepout_stim_i;
    esr_pwr_ctrl_neigh_nsleepout_i = esr_pwr_ctrl_neigh_nsleepout_stim_i;
    dmctrl_i = '0;
    dmctrl_i.dmactive = dft_ram_rei_i;
    dmctrl_i.ndmreset = dft_ram_wei_i;
    apb_esr_rsp_neigh_i = '{default: '0};
    for (int unsigned neigh_rsp_idx = 0; neigh_rsp_idx < NumNeigh; neigh_rsp_idx++) begin
      apb_esr_rsp_neigh_i[neigh_rsp_idx].apb_pready = 1'b1;
      apb_esr_rsp_neigh_i[neigh_rsp_idx].apb_prdata = 64'h5a5a_0000_0000_0000 | {62'h0, neigh_rsp_idx[1:0]};
    end
    for (int unsigned neigh_pwr_idx = 0; neigh_pwr_idx < NumNeigh; neigh_pwr_idx++) begin
      for (int unsigned min_pwr_idx = 0; min_pwr_idx < shire_esr_pkg::MinPerNeigh; min_pwr_idx++) begin
        esr_pwr_ctrl_neigh_nsleepout_2d_i[neigh_pwr_idx][min_pwr_idx] =
            esr_pwr_ctrl_neigh_nsleepout_stim_i[neigh_pwr_idx * shire_esr_pkg::MinPerNeigh + min_pwr_idx];
      end
    end
    for (int unsigned neigh_idx = 0; neigh_idx < NumNeigh; neigh_idx++) begin
      esr_and_or_tree_l0_i[neigh_idx] = esr_pkg::esr_and_or_tree_l0_t'(
          esr_and_or_tree_l0_flat_stim_i[neigh_idx*EsrAndOrTreeL0Bits +: EsrAndOrTreeL0Bits]);
      esr_dll_dly_est_sts_i[neigh_idx] = esr_pkg::esr_dll_dly_est_sts_t'(
          esr_dll_dly_est_sts_flat_stim_i[neigh_idx*DllDlyEstStsBits +: DllDlyEstStsBits]);
    end
    esr_pll_busy_i = esr_pll_busy_stim_i;
    esr_pll_rdata_i = esr_pll_rdata_stim_i;
    esr_pll_rrdy_i = esr_pll_rrdy_stim_i;
    esr_pll_lock_i = esr_pll_lock_stim_i;
    esr_dll_busy_i = esr_dll_busy_stim_i;
    esr_dll_rdata_i = esr_dll_rdata_stim_i;
    esr_dll_rrdy_i = esr_dll_rrdy_stim_i;
    esr_dll_lock_i = esr_dll_lock_stim_i;

    neigh_sc_req_valid_i = '0;
    neigh_sc_req_info_i = '{default: '0};
    neigh_sc_req_valid_i[0] = neigh0_req_valid_i ? neigh0_req_dest_i : '0;
    neigh_sc_req_info_i[0].opcode = etlink_pkg::req_opcode_e'(neigh0_req_opcode_i);
    neigh_sc_req_info_i[0].address = neigh0_req_addr_i;
    neigh_sc_req_info_i[0].size = etlink_pkg::size_e'(neigh0_req_size_i);
    neigh_sc_req_info_i[0].qwen = neigh0_req_qwen_i;
    neigh_sc_rsp_ready_i = neigh_sc_rsp_ready_stim_i;

    to_l3_axi_ar_ready_i = to_l3_axi_ar_ready_stim_i;
    to_l3_axi_aw_ready_i = to_l3_axi_aw_ready_stim_i;
    to_l3_axi_w_ready_i = to_l3_axi_w_ready_stim_i;
    to_l3_axi_b_i = '{default: '0};
    to_l3_axi_b_valid_i = to_l3_axi_b_valid_stim_i;
    to_l3_axi_r_i = '{default: '0};
    to_l3_axi_r_valid_i = to_l3_axi_r_valid_stim_i;
    for (int unsigned axi_idx = 0; axi_idx < NumL3MasterPorts; axi_idx++) begin
      to_l3_axi_b_i[axi_idx].id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
      to_l3_axi_b_i[axi_idx].resp = axi_pkg::resp_e'(axi_stim_i[1:0]);
      to_l3_axi_r_i[axi_idx].id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
      to_l3_axi_r_i[axi_idx].resp = axi_pkg::resp_e'(axi_stim_i[3:2]);
      to_l3_axi_r_i[axi_idx].last = axi_stim_i[4];
      to_l3_axi_r_i[axi_idx].data = {16{axi_stim_i}};
    end
    to_sys_axi_ar_ready_i = to_sys_axi_ar_ready_stim_i;
    to_sys_axi_aw_ready_i = to_sys_axi_aw_ready_stim_i;
    to_sys_axi_w_ready_i = to_sys_axi_w_ready_stim_i;
    to_sys_axi_b_i = '{default: '0};
    to_sys_axi_b_valid_i = to_sys_axi_b_valid_stim_i;
    to_sys_axi_r_i = '{default: '0};
    to_sys_axi_r_valid_i = to_sys_axi_r_valid_stim_i;
    for (int unsigned sys_m_idx = 0; sys_m_idx < NumSysPorts; sys_m_idx++) begin
      to_sys_axi_b_i[sys_m_idx].id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
      to_sys_axi_b_i[sys_m_idx].resp = axi_pkg::resp_e'(axi_stim_i[5:4]);
      to_sys_axi_r_i[sys_m_idx].id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
      to_sys_axi_r_i[sys_m_idx].resp = axi_pkg::resp_e'(axi_stim_i[7:6]);
      to_sys_axi_r_i[sys_m_idx].last = axi_stim_i[8];
      to_sys_axi_r_i[sys_m_idx].data = {16{axi_stim_i}};
    end
    l3_axi_ar_i = '{default: '0};
    l3_axi_ar_valid_i = l3_axi_ar_valid_stim_i;
    l3_axi_aw_i = '{default: '0};
    l3_axi_aw_valid_i = l3_axi_aw_valid_stim_i;
    l3_axi_w_i = '{default: '0};
    l3_axi_w_valid_i = l3_axi_w_valid_stim_i;
    l3_axi_b_ready_i = l3_axi_b_ready_stim_i;
    l3_axi_r_ready_i = l3_axi_r_ready_stim_i;
    for (int unsigned l3_idx = 0; l3_idx < NumL3SlavePorts; l3_idx++) begin
      l3_axi_ar_i[l3_idx].id = axi_stim_i[axi_pkg::ScSlaveIdSize-1:0];
      l3_axi_ar_i[l3_idx].addr = {8'h00, axi_stim_i};
      l3_axi_ar_i[l3_idx].len = axi_stim_i[15:8];
      l3_axi_ar_i[l3_idx].size = axi_stim_i[10:8];
      l3_axi_ar_i[l3_idx].burst = 2'b01;
      l3_axi_ar_i[l3_idx].lock = axi_stim_i[11];
      l3_axi_ar_i[l3_idx].cache = axi_stim_i[15:12];
      l3_axi_ar_i[l3_idx].prot = axi_stim_i[18:16];
      l3_axi_ar_i[l3_idx].qos = axi_stim_i[22:19];
      l3_axi_ar_i[l3_idx].user = axi_stim_i[23];
      l3_axi_aw_i[l3_idx].id = axi_stim_i[axi_pkg::ScSlaveIdSize-1:0];
      l3_axi_aw_i[l3_idx].addr = {8'h00, axi_stim_i};
      l3_axi_aw_i[l3_idx].len = axi_stim_i[31:24];
      l3_axi_aw_i[l3_idx].size = axi_stim_i[26:24];
      l3_axi_aw_i[l3_idx].burst = 2'b01;
      l3_axi_aw_i[l3_idx].lock = axi_stim_i[27];
      l3_axi_aw_i[l3_idx].cache = axi_stim_i[7:4];
      l3_axi_aw_i[l3_idx].prot = axi_stim_i[10:8];
      l3_axi_aw_i[l3_idx].qos = axi_stim_i[14:11];
      l3_axi_aw_i[l3_idx].user = axi_stim_i[19:15];
      l3_axi_w_i[l3_idx].data = {16{axi_stim_i}};
      l3_axi_w_i[l3_idx].strb = {64{axi_stim_i[0]}};
      l3_axi_w_i[l3_idx].last = axi_stim_i[1];
    end
    uc_to_l3_axi_ar_ready_i = uc_to_l3_axi_ar_ready_stim_i;
    uc_to_l3_axi_aw_ready_i = uc_to_l3_axi_aw_ready_stim_i;
    uc_to_l3_axi_w_ready_i = uc_to_l3_axi_w_ready_stim_i;
    uc_to_l3_axi_b_i = '0;
    uc_to_l3_axi_b_i.id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
    uc_to_l3_axi_b_i.resp = axi_pkg::resp_e'(axi_stim_i[9:8]);
    uc_to_l3_axi_b_valid_i = uc_to_l3_axi_b_valid_stim_i;
    uc_to_l3_axi_r_i = '0;
    uc_to_l3_axi_r_i.id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
    uc_to_l3_axi_r_i.resp = axi_pkg::resp_e'(axi_stim_i[11:10]);
    uc_to_l3_axi_r_i.last = axi_stim_i[12];
    uc_to_l3_axi_r_i.data = {16{axi_stim_i}};
    uc_to_l3_axi_r_valid_i = uc_to_l3_axi_r_valid_stim_i;
    uc_to_sys_axi_ar_ready_i = uc_to_sys_axi_ar_ready_stim_i;
    uc_to_sys_axi_aw_ready_i = uc_to_sys_axi_aw_ready_stim_i;
    uc_to_sys_axi_w_ready_i = uc_to_sys_axi_w_ready_stim_i;
    uc_to_sys_axi_b_i = '0;
    uc_to_sys_axi_b_i.id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
    uc_to_sys_axi_b_i.resp = axi_pkg::resp_e'(axi_stim_i[13:12]);
    uc_to_sys_axi_b_valid_i = uc_to_sys_axi_b_valid_stim_i;
    uc_to_sys_axi_r_i = '0;
    uc_to_sys_axi_r_i.id = axi_stim_i[axi_pkg::ScMasterIdSize-1:0];
    uc_to_sys_axi_r_i.resp = axi_pkg::resp_e'(axi_stim_i[15:14]);
    uc_to_sys_axi_r_i.last = axi_stim_i[16];
    uc_to_sys_axi_r_i.data = {16{axi_stim_i}};
    uc_to_sys_axi_r_valid_i = uc_to_sys_axi_r_valid_stim_i;
    sys_axi_ar_i = '0;
    sys_axi_ar_i.id = sys_id_stim_i;
    sys_axi_ar_i.addr = sys_addr_stim_i;
    sys_axi_ar_i.len = sys_len_stim_i;
    sys_axi_ar_i.size = sys_size_stim_i;
    sys_axi_ar_i.burst = 2'b01;
    sys_axi_ar_i.lock = axi_stim_i[19];
    sys_axi_ar_i.cache = axi_stim_i[23:20];
    sys_axi_ar_i.prot = axi_stim_i[26:24];
    sys_axi_ar_i.qos = axi_stim_i[30:27];
    sys_axi_ar_i.user = axi_stim_i[31];
    sys_axi_ar_valid_i = sys_axi_ar_valid_stim_i;
    sys_axi_aw_i = '0;
    sys_axi_aw_i.id = sys_id_stim_i;
    sys_axi_aw_i.addr = sys_addr_stim_i;
    sys_axi_aw_i.len = sys_len_stim_i;
    sys_axi_aw_i.size = sys_size_stim_i;
    sys_axi_aw_i.burst = 2'b01;
    sys_axi_aw_i.lock = axi_stim_i[19];
    sys_axi_aw_i.cache = axi_stim_i[23:20];
    sys_axi_aw_i.prot = axi_stim_i[26:24];
    sys_axi_aw_i.qos = axi_stim_i[30:27];
    sys_axi_aw_i.user = axi_stim_i[4:0];
    sys_axi_aw_valid_i = sys_axi_aw_valid_stim_i;
    sys_axi_w_i = '0;
    sys_axi_w_i.data = sys_wdata_stim_i;
    sys_axi_w_i.strb = '1;
    sys_axi_w_i.last = 1'b1;
    sys_axi_w_valid_i = sys_axi_w_valid_stim_i;
    sys_axi_b_ready_i = sys_axi_b_ready_stim_i;
    sys_axi_r_ready_i = sys_axi_r_ready_stim_i;
    sys_axi_aw_vcvalid_i = sys_axi_aw_vcvalid_stim_i;
    sys_axi_w_vcvalid_i = sys_axi_w_vcvalid_stim_i;
    sbm_write_credit_return_i = sbm_write_credit_return_stim_i;
    sbm_sys_axi_ar_ready_i = sbm_sys_axi_ar_ready_stim_i;
    sbm_sys_axi_aw_ready_i = sbm_sys_axi_aw_ready_stim_i;
    sbm_sys_axi_w_ready_i = sbm_sys_axi_w_ready_stim_i;
    sbm_sys_axi_b_i = '0;
    sbm_sys_axi_b_i.id = axi_stim_i[axi_pkg::ScSlaveIdSize-1:0];
    sbm_sys_axi_b_i.resp = axi_pkg::resp_e'(axi_stim_i[17:16]);
    sbm_sys_axi_b_valid_i = sbm_sys_axi_b_valid_stim_i;
    sbm_sys_axi_r_i = '0;
    sbm_sys_axi_r_i.id = axi_stim_i[axi_pkg::ScSlaveIdSize-1:0];
    sbm_sys_axi_r_i.resp = axi_pkg::resp_e'(axi_stim_i[19:18]);
    sbm_sys_axi_r_i.last = axi_stim_i[20];
    sbm_sys_axi_r_i.data = {8{axi_stim_i}};
    sbm_sys_axi_r_valid_i = sbm_sys_axi_r_valid_stim_i;

    coop_tload_slv_rdy_out_data_i = '{default: '0};
    coop_tload_slv_rdy_out_valid_i = coop_slv_valid_i;
    coop_tload_mst_done_out_coop_id_i = coop_done_id_i;
    coop_tload_mst_done_out_valid_i = coop_done_valid_i;
  end

  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_i [NumNeigh-1:0];
  always_comb begin
    for (int unsigned neigh_idx = 0; neigh_idx < NumNeigh; neigh_idx++) begin
      bpam_rc_tbox_ack_i[neigh_idx] = neigh_hv_logic_pkg::bpam_rc_tbox_ack_t'(
          bpam_rc_tbox_ack_flat_stim_i[neigh_idx*BpamRcTboxAckBits +: BpamRcTboxAckBits]);
    end
  end

  shire_channel_wrap u_dut (
    .clk_ctrl_i                         (clk_i),
    .clk_ref_i                          (clk_i),
    .clk_step_i                         (~clk_i),
    .clk_pll_i                          ({4{~clk_i}}),
    .clk_dll_i                          (clk_i),
    .clk_noc_i                          (noc_clk_i),
    .rst_cold_ni                        (rst_c_ni),
    .rst_warm_ni                        (rst_w_ni),
    .rst_system_ni                      (rst_system_ni),
    .rst_system_debug_ni                (rst_d_ni),
    .rst_noc_ni                         (noc_rst_ni),
    .rst_system_lv_no                   (rst_system_lv_no),
    .rst_system_debug_lv_no             (rst_system_debug_lv_no),
    .rst_noc_lv_no                      (rst_noc_lv_no),
    .clk_neigh_o                        (clk_neigh_o),
    .clk_shire_to_neigh_o               (clk_shire_to_neigh_o),
    .clk_shire_o                        (clk_shire_o),
    .clk_shire_debug_o                  (clk_shire_debug_o),
    .rst_c_shire_no                     (rst_c_shire_no),
    .rst_d_shire_no                     (rst_d_shire_no),
    .rst_w_shire_no                     (rst_w_shire_no),
    .rst_c_shire_scs_no                 (rst_c_shire_scs_no),
    .rst_warm_to_neigh_no               (rst_warm_to_neigh_no),
    .rst_w_icache_ni                    (rst_w_icache_ni),
    .rst_sc_no                          (rst_sc_no),
    .rst_rbox_no                        (rst_rbox_no),
    .dft_hv_i                           (dft_hv_i),
    .dft_lv_i                           (dft_lv_i),
    .dft_sram_clk_i                     (dft_sram_clk_i),
    .dft_mbist_en_i                     (dft_mbist_en_i),
    .dmctrl_i                           (dmctrl_i),
    .dmctrl_to_neigh_o                  (dmctrl_to_neigh_o),
    .bpam_run_control_i                 (bpam_run_control_i),
    .bpam_run_control_neigh_o           (bpam_run_control_neigh_o),
    .shire_phy_id_i                     (shire_phy_id_i),
    .shire_id_reset_val_i               (shire_id_reset_val_i),
    .shire_id_o                         (shire_id_o),
    .tbox_id_o                          (tbox_id_o),
    .tbox_en_o                          (tbox_en_o),
    .plic_mtip_i                        (1'b1),
    .plic_meip_i                        (1'b1),
    .plic_seip_i                        (1'b0),
    .int_mtip_to_neigh_o                (int_mtip_to_neigh_o),
    .plic_meip_to_neigh_o               (plic_meip_to_neigh_o),
    .plic_seip_to_neigh_o               (plic_seip_to_neigh_o),
    .esr_thread0_enable_o               (esr_thread0_enable_neigh_o),
    .esr_thread1_enable_o               (esr_thread1_enable_neigh_o),
    .esr_neigh_ipi_msip_o               (esr_neigh_ipi_msip_o),
    .esr_neigh_ipi_redirect_trigger_o   (esr_neigh_ipi_redirect_trigger_o),
    .esr_minion_features_o              (esr_minion_features_o),
    .esr_icache_prefetch_conf_o         (esr_icache_prefetch_conf_o),
    .esr_icache_prefetch_start_o        (esr_icache_prefetch_start_o),
    .esr_icache_prefetch_done_i         (esr_icache_prefetch_done_i),
    .esr_neigh_icache_err_detected_i    (neigh_sc_err_detected_i),
    .esr_neigh_icache_err_logged_i      (neigh_sc_err_logged_i),
    .esr_shire_coop_mode_o              (esr_shire_coop_mode_o),
    .esr_and_or_tree_l0_i               (esr_and_or_tree_l0_i),
    .debug_and_or_tree_l2_i             (debug_and_or_tree_l2_i),
    .debug_and_or_tree_l2_o             (debug_and_or_tree_l2_o),
    .esr_neigh_clk_gate_ctrl_o          (esr_neigh_clk_gate_ctrl_o),
    .esr_minion_mem_override_o          (esr_minion_mem_override_o),
    .esr_pwr_ctrl_glb_nsleepin_o        (esr_pwr_ctrl_glb_nsleepin_o),
    .esr_pwr_ctrl_glb_iso_o             (esr_pwr_ctrl_glb_iso_o),
    .esr_pwr_ctrl_glb_nsleepout_i       (esr_pwr_ctrl_glb_nsleepout_i),
    .esr_pwr_ctrl_neigh_nsleepin_o      (esr_pwr_ctrl_neigh_nsleepin_2d_o),
    .esr_pwr_ctrl_neigh_iso_o           (esr_pwr_ctrl_neigh_iso_2d_o),
    .esr_pwr_ctrl_neigh_nsleepout_i     (esr_pwr_ctrl_neigh_nsleepout_2d_i),
    .esr_dll_dly_est_ctl_o              (esr_dll_dly_est_ctl_o),
    .esr_dll_dly_est_sts_i              (esr_dll_dly_est_sts_i),
    .status_monitor_bank_sel_i          (status_monitor_bank_sel_i),
    .apb_esr_req_neigh_o                (apb_esr_req_neigh_o),
    .apb_esr_rsp_neigh_i                (apb_esr_rsp_neigh_i),
    .neigh_sc_req_ready_o               (neigh_sc_req_ready_o),
    .neigh_sc_req_valid_i               (neigh_sc_req_valid_i),
    .neigh_sc_req_info_i                (neigh_sc_req_info_i),
    .neigh_sc_rsp_ready_i               (neigh_sc_rsp_ready_i),
    .neigh_sc_rsp_valid_o               (neigh_sc_rsp_valid_o),
    .neigh_sc_rsp_info_o                (neigh_sc_rsp_info_o),
    .uc_to_neigh_fcc_o                  (uc_to_neigh_fcc_o),
    .uc_to_neigh_fcc_target_o           (uc_to_neigh_fcc_target_o),
    .flb_neigh_l2_req_valid_i           (flb_neigh_l2_req_valid_i),
    .flb_neigh_l2_req_data_i            (flb_neigh_l2_req_data_i),
    .flb_l2_neigh_resp_valid_o          (flb_l2_neigh_resp_valid_o),
    .flb_l2_neigh_resp_data_o           (flb_l2_neigh_resp_data_o),
    .bpam_rc_tbox_ack_i                 (bpam_rc_tbox_ack_i),
    .ioshire_combined_err_int_o         (ioshire_log_err_int_o),
    .ioshire_noc_err_int_o              (ioshire_noc_err_int_o),
    .noc_err_int_srcs_i                 (noc_err_int_srcs_i),
    .noc_all_err_int_srcs_o             (noc_all_err_int_srcs_o),
    .to_l3_axi_ar_o                     (to_l3_axi_ar_o),
    .to_l3_axi_ar_valid_o               (to_l3_axi_ar_valid_o),
    .to_l3_axi_ar_ready_i               (to_l3_axi_ar_ready_i),
    .to_l3_axi_aw_o                     (to_l3_axi_aw_o),
    .to_l3_axi_aw_valid_o               (to_l3_axi_aw_valid_o),
    .to_l3_axi_aw_ready_i               (to_l3_axi_aw_ready_i),
    .to_l3_axi_w_o                      (to_l3_axi_w_o),
    .to_l3_axi_w_valid_o                (to_l3_axi_w_valid_o),
    .to_l3_axi_w_ready_i                (to_l3_axi_w_ready_i),
    .to_l3_axi_b_i                      (to_l3_axi_b_i),
    .to_l3_axi_b_valid_i                (to_l3_axi_b_valid_i),
    .to_l3_axi_b_ready_o                (to_l3_axi_b_ready_o),
    .to_l3_axi_r_i                      (to_l3_axi_r_i),
    .to_l3_axi_r_valid_i                (to_l3_axi_r_valid_i),
    .to_l3_axi_r_ready_o                (to_l3_axi_r_ready_o),
    .to_sys_axi_ar_o                    (to_sys_axi_ar_o),
    .to_sys_axi_ar_valid_o              (to_sys_axi_ar_valid_o),
    .to_sys_axi_ar_ready_i              (to_sys_axi_ar_ready_i),
    .to_sys_axi_aw_o                    (to_sys_axi_aw_o),
    .to_sys_axi_aw_valid_o              (to_sys_axi_aw_valid_o),
    .to_sys_axi_aw_ready_i              (to_sys_axi_aw_ready_i),
    .to_sys_axi_w_o                     (to_sys_axi_w_o),
    .to_sys_axi_w_valid_o               (to_sys_axi_w_valid_o),
    .to_sys_axi_w_ready_i               (to_sys_axi_w_ready_i),
    .to_sys_axi_b_i                     (to_sys_axi_b_i),
    .to_sys_axi_b_valid_i               (to_sys_axi_b_valid_i),
    .to_sys_axi_b_ready_o               (to_sys_axi_b_ready_o),
    .to_sys_axi_r_i                     (to_sys_axi_r_i),
    .to_sys_axi_r_valid_i               (to_sys_axi_r_valid_i),
    .to_sys_axi_r_ready_o               (to_sys_axi_r_ready_o),
    .l3_axi_ar_i                        (l3_axi_ar_i),
    .l3_axi_ar_valid_i                  (l3_axi_ar_valid_i),
    .l3_axi_ar_ready_o                  (l3_axi_ar_ready_o),
    .l3_axi_aw_i                        (l3_axi_aw_i),
    .l3_axi_aw_valid_i                  (l3_axi_aw_valid_i),
    .l3_axi_aw_ready_o                  (l3_axi_aw_ready_o),
    .l3_axi_w_i                         (l3_axi_w_i),
    .l3_axi_w_valid_i                   (l3_axi_w_valid_i),
    .l3_axi_w_ready_o                   (l3_axi_w_ready_o),
    .l3_axi_b_o                         (l3_axi_b_o),
    .l3_axi_b_valid_o                   (l3_axi_b_valid_o),
    .l3_axi_b_ready_i                   (l3_axi_b_ready_i),
    .l3_axi_r_o                         (l3_axi_r_o),
    .l3_axi_r_valid_o                   (l3_axi_r_valid_o),
    .l3_axi_r_ready_i                   (l3_axi_r_ready_i),
    .uc_to_l3_axi_ar_o                  (uc_to_l3_axi_ar_o),
    .uc_to_l3_axi_ar_valid_o            (uc_to_l3_axi_ar_valid_o),
    .uc_to_l3_axi_ar_ready_i            (uc_to_l3_axi_ar_ready_i),
    .uc_to_l3_axi_aw_o                  (uc_to_l3_axi_aw_o),
    .uc_to_l3_axi_aw_valid_o            (uc_to_l3_axi_aw_valid_o),
    .uc_to_l3_axi_aw_ready_i            (uc_to_l3_axi_aw_ready_i),
    .uc_to_l3_axi_w_o                   (uc_to_l3_axi_w_o),
    .uc_to_l3_axi_w_valid_o             (uc_to_l3_axi_w_valid_o),
    .uc_to_l3_axi_w_ready_i             (uc_to_l3_axi_w_ready_i),
    .uc_to_l3_axi_b_i                   (uc_to_l3_axi_b_i),
    .uc_to_l3_axi_b_valid_i             (uc_to_l3_axi_b_valid_i),
    .uc_to_l3_axi_b_ready_o             (uc_to_l3_axi_b_ready_o),
    .uc_to_l3_axi_r_i                   (uc_to_l3_axi_r_i),
    .uc_to_l3_axi_r_valid_i             (uc_to_l3_axi_r_valid_i),
    .uc_to_l3_axi_r_ready_o             (uc_to_l3_axi_r_ready_o),
    .uc_to_sys_axi_ar_o                 (uc_to_sys_axi_ar_o),
    .uc_to_sys_axi_ar_valid_o           (uc_to_sys_axi_ar_valid_o),
    .uc_to_sys_axi_ar_ready_i           (uc_to_sys_axi_ar_ready_i),
    .uc_to_sys_axi_aw_o                 (uc_to_sys_axi_aw_o),
    .uc_to_sys_axi_aw_valid_o           (uc_to_sys_axi_aw_valid_o),
    .uc_to_sys_axi_aw_ready_i           (uc_to_sys_axi_aw_ready_i),
    .uc_to_sys_axi_w_o                  (uc_to_sys_axi_w_o),
    .uc_to_sys_axi_w_valid_o            (uc_to_sys_axi_w_valid_o),
    .uc_to_sys_axi_w_ready_i            (uc_to_sys_axi_w_ready_i),
    .uc_to_sys_axi_b_i                  (uc_to_sys_axi_b_i),
    .uc_to_sys_axi_b_valid_i            (uc_to_sys_axi_b_valid_i),
    .uc_to_sys_axi_b_ready_o            (uc_to_sys_axi_b_ready_o),
    .uc_to_sys_axi_r_i                  (uc_to_sys_axi_r_i),
    .uc_to_sys_axi_r_valid_i            (uc_to_sys_axi_r_valid_i),
    .uc_to_sys_axi_r_ready_o            (uc_to_sys_axi_r_ready_o),
    .sys_axi_ar_i                       (sys_axi_ar_i),
    .sys_axi_ar_valid_i                 (sys_axi_ar_valid_i),
    .sys_axi_ar_ready_o                 (sys_axi_ar_ready_o),
    .sys_axi_aw_i                       (sys_axi_aw_i),
    .sys_axi_aw_valid_i                 (sys_axi_aw_valid_i),
    .sys_axi_aw_ready_o                 (sys_axi_aw_ready_o),
    .sys_axi_w_i                        (sys_axi_w_i),
    .sys_axi_w_valid_i                  (sys_axi_w_valid_i),
    .sys_axi_w_ready_o                  (sys_axi_w_ready_o),
    .sys_axi_b_o                        (sys_axi_b_o),
    .sys_axi_b_valid_o                  (sys_axi_b_valid_o),
    .sys_axi_b_ready_i                  (sys_axi_b_ready_i),
    .sys_axi_r_o                        (sys_axi_r_o),
    .sys_axi_r_valid_o                  (sys_axi_r_valid_o),
    .sys_axi_r_ready_i                  (sys_axi_r_ready_i),
    .sys_axi_aw_vcvalid_i               (sys_axi_aw_vcvalid_i),
    .sys_axi_aw_credit_o                (sys_axi_aw_credit_o),
    .sys_axi_w_vcvalid_i                (sys_axi_w_vcvalid_i),
    .icache_req_write_i                 (icache_req_write_i),
    .icache_req_addr_i                  (icache_req_addr_i),
    .icache_req_valid_i                 (icache_req_valid_i),
    .icache_req_ready_o                 (icache_req_ready_o),
    .icache_resp_dout_o                 (icache_resp_dout_o),
    .icache_resp_valid_o                (icache_resp_valid_o),
    .icache_resp_ready_i                (icache_resp_ready_i),
    .sc_neigh_l2hpf_req_valid_o         (sc_neigh_l2hpf_req_valid_o),
    .sc_neigh_l2hpf_req_info_o          (sc_neigh_l2hpf_req_info_o),
    .sc_trace_data_o                    (sc_trace_data_o),
    .sc_trace_valid_o                   (sc_trace_valid_o),
    .coop_tload_slv_rdy_out_data_i      (coop_tload_slv_rdy_out_data_i),
    .coop_tload_slv_rdy_out_valid_i     (coop_tload_slv_rdy_out_valid_i),
    .coop_tload_slv_rdy_in_data_o       (coop_tload_slv_rdy_in_data_o),
    .coop_tload_slv_rdy_in_valid_o      (coop_tload_slv_rdy_in_valid_o),
    .coop_tload_mst_done_out_coop_id_i  (coop_tload_mst_done_out_coop_id_i),
    .coop_tload_mst_done_out_valid_i    (coop_tload_mst_done_out_valid_i),
    .coop_tload_mst_done_in_coop_id_o   (coop_tload_mst_done_in_coop_id_o),
    .coop_tload_mst_done_in_valid_o     (coop_tload_mst_done_in_valid_o),
    .ram_cfg_o                          (ram_cfg_o),
    .esr_clk_gate_ctrl_o                (esr_clk_gate_ctrl_o),
    .esr_debug_clk_gate_ctrl_o          (esr_debug_clk_gate_ctrl_o),
    .esr_pll_ctrl_o                     (esr_pll_ctrl_o),
    .esr_pll_conf_o                     (esr_pll_conf_o),
    .esr_dll_ctrl_o                     (esr_dll_ctrl_o),
    .esr_dll_conf_o                     (esr_dll_conf_o),
    .esr_shire_ctrl_clockmux_o          (esr_shire_ctrl_clockmux_o)
  );

  assign rst_c_shire_no_o = rst_c_shire_no;
  assign rst_w_shire_no_o = rst_w_shire_no;
  assign rst_d_shire_no_o = rst_d_shire_no;
  assign rst_c_shire_scs_no_o = rst_c_shire_scs_no;
  assign rst_sc_no_o = rst_sc_no;
  assign rst_rbox_no_o = rst_rbox_no;
  assign rst_warm_to_neigh_no_o = rst_warm_to_neigh_no;
  assign plic_meip_obs_o = plic_meip_to_neigh_o;
  assign plic_seip_obs_o = plic_seip_to_neigh_o;
  assign tbox_id_flat_o = tbox_id_o;
  assign esr_icache_prefetch_start_flat_o = esr_icache_prefetch_start_o;
  assign debug_and_or_tree_l2_flat_o = debug_and_or_tree_l2_o;
  assign sc_neigh_l2hpf_req_valid_flat_o = sc_neigh_l2hpf_req_valid_o;
  assign sc_trace_valid_flat_o = sc_trace_valid_o;
  assign dft_hv_flat_o = dft_hv_i;
  assign pwr_ctrl_glb_nsleepin_o = esr_pwr_ctrl_glb_nsleepin_o;
  assign pwr_ctrl_glb_iso_o = esr_pwr_ctrl_glb_iso_o;
  assign ram_cfg_flat_o = ram_cfg_o;

  always_comb begin
    esr_thread0_enable_o = '0;
    esr_thread1_enable_o = '0;
    esr_ipi_trigger_o = '0;
    esr_ipi_redirect_trigger_o = '0;
    esr_mtime_local_target_o = '0;
    pwr_ctrl_neigh_nsleepin_o = '0;
    pwr_ctrl_neigh_iso_o = '0;
    apb_neigh_psel_o = '0;
    apb_neigh_pwrite_o = '0;
    dmctrl_to_neigh_flat_o = '0;
    for (int unsigned neigh_idx = 0; neigh_idx < NumNeigh; neigh_idx++) begin
      dmctrl_to_neigh_flat_o[neigh_idx*DmctrlBits +: DmctrlBits] = dmctrl_to_neigh_o[neigh_idx];
      apb_neigh_psel_o[neigh_idx] = apb_esr_req_neigh_o[neigh_idx].apb_psel;
      apb_neigh_pwrite_o[neigh_idx] = apb_esr_req_neigh_o[neigh_idx].apb_pwrite;
      for (int unsigned min_idx = 0; min_idx < shire_esr_pkg::MinPerNeigh; min_idx++) begin
        esr_thread0_enable_o[neigh_idx * shire_esr_pkg::MinPerNeigh + min_idx] =
            esr_thread0_enable_neigh_o[neigh_idx][min_idx];
        esr_thread1_enable_o[neigh_idx * shire_esr_pkg::MinPerNeigh + min_idx] =
            esr_thread1_enable_neigh_o[neigh_idx][min_idx];
        esr_mtime_local_target_o[neigh_idx * shire_esr_pkg::MinPerNeigh + min_idx] =
            int_mtip_to_neigh_o[neigh_idx][min_idx];
        pwr_ctrl_neigh_nsleepin_o[neigh_idx * shire_esr_pkg::MinPerNeigh + min_idx] =
            esr_pwr_ctrl_neigh_nsleepin_2d_o[neigh_idx][min_idx];
        pwr_ctrl_neigh_iso_o[neigh_idx * shire_esr_pkg::MinPerNeigh + min_idx] =
            esr_pwr_ctrl_neigh_iso_2d_o[neigh_idx][min_idx];
      end
      for (int unsigned thread_idx = 0; thread_idx < ThreadsPerNeigh; thread_idx++) begin
        esr_ipi_trigger_o[neigh_idx * ThreadsPerNeigh + thread_idx] =
            esr_neigh_ipi_msip_o[neigh_idx][thread_idx];
        esr_ipi_redirect_trigger_o[neigh_idx * ThreadsPerNeigh + thread_idx] =
            esr_neigh_ipi_redirect_trigger_o[neigh_idx][thread_idx];
      end
    end
  end

  assign apb_neigh0_paddr_o = apb_esr_req_neigh_o[0].apb_paddr;
  assign apb_neigh0_pwdata_o = apb_esr_req_neigh_o[0].apb_pwdata;
  assign sys_axi_ar_ready_obs_o = sys_axi_ar_ready_o;
  assign sys_axi_aw_ready_obs_o = sys_axi_aw_ready_o;
  assign sys_axi_w_ready_obs_o = sys_axi_w_ready_o;
  assign sys_axi_b_valid_obs_o = sys_axi_b_valid_o;
  assign sys_axi_b_id_obs_o = sys_axi_b_o.id;
  assign sys_axi_b_resp_obs_o = sys_axi_b_o.resp;
  assign sys_axi_r_valid_obs_o = sys_axi_r_valid_o;
  assign sys_axi_r_id_obs_o = sys_axi_r_o.id;
  assign sys_axi_r_resp_obs_o = sys_axi_r_o.resp;
  assign sys_axi_r_data_lo_obs_o = sys_axi_r_o.data[63:0];
  assign rst_system_lv_no_o = rst_system_lv_no;
  assign rst_system_debug_lv_no_o = rst_system_debug_lv_no;
  assign rst_noc_lv_no_o = rst_noc_lv_no;
  assign clk_neigh_obs_o = clk_neigh_o;
  assign clk_shire_obs_o = clk_shire_o;
  assign sbm_enable_read_o = u_dut.sbm_enable_read;
  assign sbm_enable_write_o = u_dut.sbm_enable_write;
  assign sbm_sys_axi_ar_o = u_dut.sbm_sys_axi_ar;
  assign sbm_sys_axi_aw_o = u_dut.sbm_sys_axi_aw;
  assign sbm_sys_axi_w_o = u_dut.sbm_sys_axi_w;
  assign sbm_sys_axi_b_ready_o = u_dut.sbm_sys_axi_b_ready;
  assign sbm_sys_axi_r_ready_o = u_dut.sbm_sys_axi_r_ready;
  assign clk_gate_ctrl_flat_o = esr_clk_gate_ctrl_o;
  assign debug_clk_gate_ctrl_o = esr_debug_clk_gate_ctrl_o;
  assign pll_ctrl_flat_o = esr_pll_ctrl_o;
  assign pll_conf_o = esr_pll_conf_o;
  assign dll_ctrl_flat_o = esr_dll_ctrl_o;
  assign dll_conf_o = esr_dll_conf_o;
  assign shire_ctrl_clockmux_flat_o = esr_shire_ctrl_clockmux_o;

  always_comb begin
    apb_channel_psel_o = '0;
    apb_channel_pwrite_o = '0;
    apb_channel_pready_all_o = '0;
    apb_channel_pslverr_all_o = '0;
    apb_channel_prdata_all_flat_o = '0;
    dll_dly_est_ctl_flat_o = '0;
    for (int unsigned apb_idx = 0; apb_idx < ApbSlaves; apb_idx++) begin
      apb_channel_psel_o[apb_idx] = apb_req_i[apb_idx].psel;
      apb_channel_pwrite_o[apb_idx] = apb_req_i[apb_idx].pwrite;
      apb_channel_pready_all_o[apb_idx] = apb_rsp_o[apb_idx].pready;
      apb_channel_pslverr_all_o[apb_idx] = apb_rsp_o[apb_idx].pslverr;
      apb_channel_prdata_all_flat_o[apb_idx*shire_sbm_pkg::ApbDataWidth +: shire_sbm_pkg::ApbDataWidth] =
          apb_rsp_o[apb_idx].prdata;
    end
    for (int unsigned dll_idx = 0; dll_idx < NumNeigh; dll_idx++) begin
      dll_dly_est_ctl_flat_o[dll_idx*DllDlyEstCtlBits +: DllDlyEstCtlBits] =
          esr_dll_dly_est_ctl_o[dll_idx];
    end
  end

  for (genvar flat_idx = 0; flat_idx < NumNeigh; flat_idx++) begin : gen_flat_outputs
    assign esr_minion_features_flat_o[flat_idx] = esr_minion_features_o[flat_idx];
    assign esr_icache_prefetch_conf_flat_o[flat_idx] = esr_icache_prefetch_conf_o[flat_idx];
    assign bpam_run_control_neigh_flat_o[flat_idx] = bpam_run_control_neigh_o[flat_idx];
  end

  assign coop_slv_valid_o = coop_tload_slv_rdy_in_valid_o;
  assign coop_done_id_o = coop_tload_mst_done_in_coop_id_o;
  assign coop_done_valid_o = coop_tload_mst_done_in_valid_o;

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on VARHIDDEN */
