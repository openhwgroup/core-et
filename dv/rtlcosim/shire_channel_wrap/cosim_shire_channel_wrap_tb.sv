// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */  // Full container cosim ties off many third-party-omitted sidebands.
/* verilator lint_off WIDTHEXPAND */    // Original macro widths are adapted to native package widths.
/* verilator lint_off WIDTHTRUNC */     // Original macro widths are adapted to native package widths.
/* verilator lint_off PINCONNECTEMPTY */  // Removed third-party/TDR/SMS outputs are intentionally unobserved.
module cosim_shire_channel_wrap_tb
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
  import neigh_voltage_cross_pkg::*;
  import ram_cfg_pkg::*;
  import shire_channel_leaves_pkg::*;
  import shire_esr_pkg::*;
  import shire_sbm_pkg::*;
  import shire_uncached_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic rst_c_ext_ni,
  input  logic rst_w_ext_ni,
  input  logic rst_d_ext_ni,
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

  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_c_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_c_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_w_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_w_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_d_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_d_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_c_shire_scs_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_c_shire_scs_no_o,
  output logic orig_rst_sc_no_o,
  output logic new_rst_sc_no_o,
  output logic orig_rst_rbox_no_o,
  output logic new_rst_rbox_no_o,
  output logic orig_rst_system_lv_no_o,
  output logic new_rst_system_lv_no_o,
  output logic orig_rst_system_debug_lv_no_o,
  output logic new_rst_system_debug_lv_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_warm_to_neigh_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_warm_to_neigh_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_clk_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_clk_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_clk_shire_to_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_clk_shire_to_neigh_o,
  output logic orig_clk_shire_o,
  output logic new_clk_shire_o,
  output logic orig_clk_shire_debug_o,
  output logic new_clk_shire_debug_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_ms_dmctrl_t)-1:0] orig_dmctrl_to_neigh_flat_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_ms_dmctrl_t)-1:0] new_dmctrl_to_neigh_flat_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_plic_meip_to_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_plic_meip_to_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_plic_seip_to_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_plic_seip_to_neigh_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_apb_neigh_psel_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_apb_neigh_psel_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_apb_neigh_pwrite_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_apb_neigh_pwrite_o,
  output logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] orig_apb_neigh0_paddr_o,
  output logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] new_apb_neigh0_paddr_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] orig_apb_neigh0_pwdata_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] new_apb_neigh0_pwdata_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] orig_coop_slv_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] new_coop_slv_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0] orig_coop_done_id_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0] new_coop_done_id_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] orig_coop_done_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] new_coop_done_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] orig_shire_id_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] new_shire_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0][1:0] orig_tbox_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0][1:0] new_tbox_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0] orig_tbox_en_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0] new_tbox_en_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_thread0_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_thread0_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_thread1_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_thread1_enable_o,
  output logic [$bits(minion_pkg::esr_minion_features_t)-1:0] orig_esr_minion_features_o [shire_esr_pkg::NumNeigh],
  output logic [$bits(minion_pkg::esr_minion_features_t)-1:0] new_esr_minion_features_o [shire_esr_pkg::NumNeigh],
  output logic [$bits(icache_pkg::icache_prefetch_conf_t)-1:0] orig_icache_prefetch_conf_o [shire_esr_pkg::NumNeigh],
  output logic [$bits(icache_pkg::icache_prefetch_conf_t)-1:0] new_icache_prefetch_conf_o [shire_esr_pkg::NumNeigh],
  output logic [shire_esr_pkg::IcachePerShire-1:0] orig_icache_prefetch_start_o,
  output logic [shire_esr_pkg::IcachePerShire-1:0] new_icache_prefetch_start_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_ipi_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_ipi_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_ipi_redirect_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_ipi_redirect_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_mtime_local_target_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_mtime_local_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_esr_shire_coop_mode_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_esr_shire_coop_mode_o,
  output logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0] orig_debug_and_or_tree_l2_o,
  output logic [$bits(shire_channel_leaves_pkg::esr_and_or_tree_l2_t)-1:0] new_debug_and_or_tree_l2_o,
  output logic [$bits(neigh_voltage_cross_pkg::bpam_run_control_neigh_t)-1:0] orig_bpam_run_control_neigh_o [shire_esr_pkg::NumNeigh],
  output logic [$bits(neigh_voltage_cross_pkg::bpam_run_control_neigh_t)-1:0] new_bpam_run_control_neigh_o [shire_esr_pkg::NumNeigh],
  output logic orig_ioshire_log_err_int_o,
  output logic new_ioshire_log_err_int_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] orig_uc_to_neigh_fcc_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] new_uc_to_neigh_fcc_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] orig_uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] new_uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_flb_l2_neigh_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_flb_l2_neigh_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_flb_l2_neigh_resp_data_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_flb_l2_neigh_resp_data_o,
  output logic orig_apb_pready_o,
  output logic new_apb_pready_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] orig_apb_prdata_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] new_apb_prdata_o,
  output logic orig_apb_pslverr_o,
  output logic new_apb_pslverr_o,
  output logic orig_sbm_enable_read_o,
  output logic new_sbm_enable_read_o,
  output logic orig_sbm_enable_write_o,
  output logic new_sbm_enable_write_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shirecache_pkg::Banks:0] orig_neigh_sc_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shirecache_pkg::Banks:0] new_neigh_sc_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_neigh_sc_rsp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_neigh_sc_rsp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_icache_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_icache_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_icache_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_icache_resp_valid_o,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0] orig_noc_all_err_int_srcs_o,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0] new_noc_all_err_int_srcs_o,
  output logic orig_ioshire_noc_err_int_o,
  output logic new_ioshire_noc_err_int_o,
  output logic [shirecache_pkg::Banks-1:0] orig_l2hpf_req_valid_o,
  output logic [shirecache_pkg::Banks-1:0] new_l2hpf_req_valid_o,
  output logic orig_sc_trace_valid_o,
  output logic new_sc_trace_valid_o,
  output logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0] new_ram_cfg_flat_o,
  output logic [$bits(shire_esr_pkg::esr_clk_gate_ctrl_t)-1:0] orig_clk_gate_ctrl_flat_o,
  output logic [$bits(shire_esr_pkg::esr_clk_gate_ctrl_t)-1:0] new_clk_gate_ctrl_flat_o,
  output logic orig_debug_clk_gate_ctrl_o,
  output logic new_debug_clk_gate_ctrl_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(etlink_pkg::rsp_t)-1:0] orig_neigh_sc_rsp_info_flat_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(etlink_pkg::rsp_t)-1:0] new_neigh_sc_rsp_info_flat_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] orig_apb_pready_all_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] new_apb_pready_all_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] orig_apb_pslverr_all_o,
  output logic [shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2-1:0] new_apb_pslverr_all_o,
  output logic [(shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2)*shire_sbm_pkg::ApbDataWidth-1:0] orig_apb_prdata_all_flat_o,
  output logic [(shirecache_pkg::Banks+shirecache_pkg::RboxPerShire+2)*shire_sbm_pkg::ApbDataWidth-1:0] new_apb_prdata_all_flat_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_pwr_ctrl_glb_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_pwr_ctrl_glb_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_pwr_ctrl_glb_iso_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_pwr_ctrl_glb_iso_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] orig_pwr_ctrl_neigh_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] new_pwr_ctrl_neigh_nsleepin_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] orig_pwr_ctrl_neigh_iso_o,
  output logic [shire_esr_pkg::NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] new_pwr_ctrl_neigh_iso_o,
  output logic [$bits(shire_esr_pkg::esr_pll_auto_config_t)-1:0] orig_pll_ctrl_flat_o,
  output logic [$bits(shire_esr_pkg::esr_pll_auto_config_t)-1:0] new_pll_ctrl_flat_o,
  output logic [shire_esr_pkg::ShirePllConfBits-1:0] orig_pll_conf_o,
  output logic [shire_esr_pkg::ShirePllConfBits-1:0] new_pll_conf_o,
  output logic [$bits(shire_esr_pkg::esr_clk_dly_ctl_t)-1:0] orig_clk_dly_ctl_flat_o,
  output logic [$bits(shire_esr_pkg::esr_clk_dly_ctl_t)-1:0] new_clk_dly_ctl_flat_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_dll_dly_est_ctl_t)-1:0]
      orig_dll_dly_est_ctl_flat_o,
  output logic [shire_esr_pkg::NumNeigh*$bits(esr_pkg::esr_dll_dly_est_ctl_t)-1:0]
      new_dll_dly_est_ctl_flat_o,
  output logic [$bits(shire_esr_pkg::esr_clkmux_ctl_t)-1:0] orig_shire_ctrl_clockmux_flat_o,
  output logic [$bits(shire_esr_pkg::esr_clkmux_ctl_t)-1:0] new_shire_ctrl_clockmux_flat_o,
  output logic [$bits(shire_esr_pkg::esr_dll_auto_config_t)-1:0] orig_dll_ctrl_flat_o,
  output logic [$bits(shire_esr_pkg::esr_dll_auto_config_t)-1:0] new_dll_ctrl_flat_o,
  output logic [shire_esr_pkg::ShireDllConfBits-1:0] orig_dll_conf_o,
  output logic [shire_esr_pkg::ShireDllConfBits-1:0] new_dll_conf_o,
  output logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0] orig_ram_cfg_flat_o,
  output logic [shirecache_pkg::L3MasterPorts*$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_l3_axi_ar_flat_o,
  output logic [shirecache_pkg::L3MasterPorts*$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_l3_axi_ar_flat_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] orig_to_l3_axi_ar_valid_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] new_to_l3_axi_ar_valid_o,
  output logic [shirecache_pkg::L3MasterPorts*$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_l3_axi_aw_flat_o,
  output logic [shirecache_pkg::L3MasterPorts*$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_l3_axi_aw_flat_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] orig_to_l3_axi_aw_valid_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] new_to_l3_axi_aw_valid_o,
  output logic [shirecache_pkg::L3MasterPorts*$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_l3_axi_w_flat_o,
  output logic [shirecache_pkg::L3MasterPorts*$bits(axi_pkg::sc_master_w_t)-1:0] new_to_l3_axi_w_flat_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] orig_to_l3_axi_w_valid_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] new_to_l3_axi_w_valid_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] orig_to_l3_axi_b_ready_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] new_to_l3_axi_b_ready_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] orig_to_l3_axi_r_ready_o,
  output logic [shirecache_pkg::L3MasterPorts-1:0] new_to_l3_axi_r_ready_o,
  output logic [shirecache_pkg::SysPorts*$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_sys_axi_ar_flat_o,
  output logic [shirecache_pkg::SysPorts*$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_sys_axi_ar_flat_o,
  output logic [shirecache_pkg::SysPorts-1:0] orig_to_sys_axi_ar_valid_o,
  output logic [shirecache_pkg::SysPorts-1:0] new_to_sys_axi_ar_valid_o,
  output logic [shirecache_pkg::SysPorts*$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_sys_axi_aw_flat_o,
  output logic [shirecache_pkg::SysPorts*$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_sys_axi_aw_flat_o,
  output logic [shirecache_pkg::SysPorts-1:0] orig_to_sys_axi_aw_valid_o,
  output logic [shirecache_pkg::SysPorts-1:0] new_to_sys_axi_aw_valid_o,
  output logic [shirecache_pkg::SysPorts*$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_sys_axi_w_flat_o,
  output logic [shirecache_pkg::SysPorts*$bits(axi_pkg::sc_master_w_t)-1:0] new_to_sys_axi_w_flat_o,
  output logic [shirecache_pkg::SysPorts-1:0] orig_to_sys_axi_w_valid_o,
  output logic [shirecache_pkg::SysPorts-1:0] new_to_sys_axi_w_valid_o,
  output logic [shirecache_pkg::SysPorts-1:0] orig_to_sys_axi_b_ready_o,
  output logic [shirecache_pkg::SysPorts-1:0] new_to_sys_axi_b_ready_o,
  output logic [shirecache_pkg::SysPorts-1:0] orig_to_sys_axi_r_ready_o,
  output logic [shirecache_pkg::SysPorts-1:0] new_to_sys_axi_r_ready_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] orig_l3_axi_ar_ready_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] new_l3_axi_ar_ready_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] orig_l3_axi_aw_ready_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] new_l3_axi_aw_ready_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] orig_l3_axi_w_ready_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] new_l3_axi_w_ready_o,
  output logic [shirecache_pkg::L3SlavePorts*$bits(axi_pkg::sc_slave_b_t)-1:0] orig_l3_axi_b_flat_o,
  output logic [shirecache_pkg::L3SlavePorts*$bits(axi_pkg::sc_slave_b_t)-1:0] new_l3_axi_b_flat_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] orig_l3_axi_b_valid_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] new_l3_axi_b_valid_o,
  output logic [shirecache_pkg::L3SlavePorts*$bits(axi_pkg::sc_slave_r_t)-1:0] orig_l3_axi_r_flat_o,
  output logic [shirecache_pkg::L3SlavePorts*$bits(axi_pkg::sc_slave_r_t)-1:0] new_l3_axi_r_flat_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] orig_l3_axi_r_valid_o,
  output logic [shirecache_pkg::L3SlavePorts-1:0] new_l3_axi_r_valid_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_uc_to_l3_axi_ar_flat_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_uc_to_l3_axi_ar_flat_o,
  output logic orig_uc_to_l3_axi_ar_valid_o,
  output logic new_uc_to_l3_axi_ar_valid_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_uc_to_l3_axi_aw_flat_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_uc_to_l3_axi_aw_flat_o,
  output logic orig_uc_to_l3_axi_aw_valid_o,
  output logic new_uc_to_l3_axi_aw_valid_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_uc_to_l3_axi_w_flat_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_uc_to_l3_axi_w_flat_o,
  output logic orig_uc_to_l3_axi_w_valid_o,
  output logic new_uc_to_l3_axi_w_valid_o,
  output logic orig_uc_to_l3_axi_b_ready_o,
  output logic new_uc_to_l3_axi_b_ready_o,
  output logic orig_uc_to_l3_axi_r_ready_o,
  output logic new_uc_to_l3_axi_r_ready_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_uc_to_sys_axi_ar_flat_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_uc_to_sys_axi_ar_flat_o,
  output logic orig_uc_to_sys_axi_ar_valid_o,
  output logic new_uc_to_sys_axi_ar_valid_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_uc_to_sys_axi_aw_flat_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_uc_to_sys_axi_aw_flat_o,
  output logic orig_uc_to_sys_axi_aw_valid_o,
  output logic new_uc_to_sys_axi_aw_valid_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_uc_to_sys_axi_w_flat_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_uc_to_sys_axi_w_flat_o,
  output logic orig_uc_to_sys_axi_w_valid_o,
  output logic new_uc_to_sys_axi_w_valid_o,
  output logic orig_uc_to_sys_axi_b_ready_o,
  output logic new_uc_to_sys_axi_b_ready_o,
  output logic orig_uc_to_sys_axi_r_ready_o,
  output logic new_uc_to_sys_axi_r_ready_o,
  output logic orig_sys_axi_ar_ready_o,
  output logic new_sys_axi_ar_ready_o,
  output logic orig_sys_axi_aw_ready_o,
  output logic new_sys_axi_aw_ready_o,
  output logic orig_sys_axi_w_ready_o,
  output logic new_sys_axi_w_ready_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] orig_sys_axi_b_flat_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] new_sys_axi_b_flat_o,
  output logic orig_sys_axi_b_valid_o,
  output logic new_sys_axi_b_valid_o,
  output logic [$bits(axi_pkg::sys_slave_r_t)-1:0] orig_sys_axi_r_flat_o,
  output logic [$bits(axi_pkg::sys_slave_r_t)-1:0] new_sys_axi_r_flat_o,
  output logic orig_sys_axi_r_valid_o,
  output logic new_sys_axi_r_valid_o,
  output logic [1:0] orig_sys_axi_aw_credit_o,
  output logic [1:0] new_sys_axi_aw_credit_o,
  output logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] orig_sbm_sys_axi_ar_flat_o,
  output logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] new_sbm_sys_axi_ar_flat_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] orig_sbm_sys_axi_aw_flat_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] new_sbm_sys_axi_aw_flat_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] orig_sbm_sys_axi_w_flat_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] new_sbm_sys_axi_w_flat_o,
  output logic orig_sbm_sys_axi_b_ready_o,
  output logic new_sbm_sys_axi_b_ready_o,
  output logic orig_sbm_sys_axi_r_ready_o,
  output logic new_sbm_sys_axi_r_ready_o,
  output logic [shire_esr_pkg::NumNeigh*icache_geom_pkg::IcacheSramDataWidth-1:0] orig_icache_resp_dout_flat_o,
  output logic [shire_esr_pkg::NumNeigh*icache_geom_pkg::IcacheSramDataWidth-1:0] new_icache_resp_dout_flat_o,
  output logic [shirecache_pkg::Banks*$bits(shirecache_pkg::neigh_l2hpf_req_t)-1:0] orig_l2hpf_req_info_flat_o,
  output logic [shirecache_pkg::Banks*$bits(shirecache_pkg::neigh_l2hpf_req_t)-1:0] new_l2hpf_req_info_flat_o,
  output logic [$bits(shirecache_pkg::trace_packet_t)-1:0] orig_sc_trace_data_flat_o,
  output logic [$bits(shirecache_pkg::trace_packet_t)-1:0] new_sc_trace_data_flat_o
);

  localparam int unsigned NUM_NEIGH = shire_esr_pkg::NumNeigh;
  localparam int unsigned NUM_RBOX = shirecache_pkg::RboxPerShire;
  localparam int unsigned NUM_BANKS = shirecache_pkg::Banks;
  localparam int unsigned NUM_UC = 1;
  localparam int unsigned NUM_L3_MASTER_PORTS = shirecache_pkg::L3MasterPorts;
  localparam int unsigned NUM_L3_SLAVE_PORTS = shirecache_pkg::L3SlavePorts;
  localparam int unsigned NUM_SYS_PORTS = shirecache_pkg::SysPorts;
  localparam int unsigned APB_SLAVES = NUM_BANKS + NUM_RBOX + 2;
  localparam int unsigned EtRspBits = $bits(etlink_pkg::rsp_t);
  localparam int unsigned ApbDataBits = shire_sbm_pkg::ApbDataWidth;
  localparam int unsigned ScMasterArBits = $bits(axi_pkg::sc_master_ar_t);
  localparam int unsigned ScMasterAwBits = $bits(axi_pkg::sc_master_aw_t);
  localparam int unsigned ScMasterWBits = $bits(axi_pkg::sc_master_w_t);
  localparam int unsigned ScSlaveBBits = $bits(axi_pkg::sc_slave_b_t);
  localparam int unsigned ScSlaveRBits = $bits(axi_pkg::sc_slave_r_t);
  localparam int unsigned SysSlaveArBits = $bits(axi_pkg::sys_slave_ar_t);
  localparam int unsigned SysSlaveAwBits = $bits(axi_pkg::sys_slave_aw_t);
  localparam int unsigned SysSlaveWBits = $bits(axi_pkg::sys_slave_w_t);
  localparam int unsigned SysSlaveBBits = $bits(axi_pkg::sys_slave_b_t);
  localparam int unsigned SysSlaveRBits = $bits(axi_pkg::sys_slave_r_t);
  localparam int unsigned IcacheRespBits = icache_geom_pkg::IcacheSramDataWidth;
  localparam int unsigned L2hpfReqBits = $bits(shirecache_pkg::neigh_l2hpf_req_t);
  localparam int unsigned EsrAndOrTreeL0Bits = $bits(esr_pkg::esr_and_or_tree_l0_t);
  localparam int unsigned BpamRcTboxAckBits = $bits(neigh_hv_logic_pkg::bpam_rc_tbox_ack_t);
  localparam int unsigned DllDlyEstStsBits = $bits(esr_pkg::esr_dll_dly_est_sts_t);
  localparam int unsigned DllDlyEstCtlBits = $bits(esr_pkg::esr_dll_dly_est_ctl_t);
  localparam int unsigned DmctrlBits = $bits(esr_pkg::esr_ms_dmctrl_t);

  // Original CORE-ET port signals. The retained cache, ICache, uncached,
  // ESR/reset, RBOX, APB, AXI, and run-control paths remain live and are
  // compared against the project-native implementation below.
   logic                                                 clock;
   logic                                                 reset_c;
   logic                                                 reset_w;
   logic                                                 reset_d;
   logic [NUM_NEIGH-1:0]                                 reset_w_icache;
   logic                                                 noc_clock;
   logic                                                 noc_reset;
  logic [NUM_NEIGH-1:0]                                 reset_c_shire;
  logic [NUM_NEIGH-1:0]                                 reset_d_shire;
  logic [NUM_NEIGH-1:0]                                 reset_w_shire;
  logic [NUM_NEIGH-1:0]                                 reset_c_shire_scs;
  logic                                                 reset_sc;
  logic                                                 reset_rbox;
   debug_axi_port_ctl_t                                  debug_axi_port_ctl;
  debug_axi_port_t                                      shire_channel_debug_axi_port;
   esr_and_or_tree_L2_t                                  debug_and_or_tree_L2_in;
  esr_and_or_tree_L2_t                                  debug_and_or_tree_L2_out;
   bpam_run_control_t                                    bpam_run_control;
  bpam_run_control_neigh_t [NUM_NEIGH-1:0]              bpam_run_control_neigh_op;
   logic [`NUM_SHIRES_R]                                 shire_phy_id;
   logic [`NUM_SHIRE_IDS_R]                              shire_id_reset_val;
  logic [NUM_NEIGH-1:0][`NUM_SHIRE_IDS_R]               shire_id;
  logic [`TBOX_PER_SHIRE-1:0][`TBOX_PER_SHIRE_R]        tbox_id;
  logic [`TBOX_PER_SHIRE-1:0]                           tbox_en;
  logic [`MIN_PER_S-1:0]                                esr_thread0_enable;
  logic [`MIN_PER_S-1:0]                                esr_thread1_enable;
  logic [NUM_NEIGH-1:0][`ESR_MINION_FEATURE_SZ-1:0]     esr_minion_features;
  icache_prefetch_conf_t  [NUM_NEIGH-1:0]               esr_icache_prefetch_conf;
  logic [`ICACHE_PER_SHIRE-1:0]                         esr_icache_prefetch_start;
   logic [`ICACHE_PER_SHIRE-1:0]                         esr_icache_prefetch_done;
   logic [NUM_NEIGH-1:0]                                 neigh_sc_err_detected;
   logic [NUM_NEIGH-1:0]                                 neigh_sc_err_logged;
  logic                                                 ioshire_log_err_int;
   esr_and_or_tree_L0_t  [NUM_NEIGH-1:0]                 esr_and_or_tree_L0;
  logic [NUM_NEIGH-1:0]                                 esr_shire_coop_mode;
   logic [`NOC_INT_NUM+`NOC_DBG_INT_NUM-1:0]             esr_noc_interrupt_status_ip;
  esr_clk_gate_ctrl_t                                   esr_clk_gate_ctrl_op;
  logic                                                 esr_debug_clk_gate_ctrl_op;
  logic [NUM_NEIGH-1:0][(NUM_BANKS+NUM_UC)-1:0]         neigh_sc_req_ready;
   logic [NUM_NEIGH-1:0][(NUM_BANKS+NUM_UC)-1:0]         neigh_sc_req_valid;
   et_link_req_info_t  [NUM_NEIGH-1:0]                   neigh_sc_req_info;
   logic [NUM_NEIGH-1:0]                                 neigh_sc_rsp_ready;
  logic [NUM_NEIGH-1:0]                                 neigh_sc_rsp_valid;
  et_link_rsp_info_t  [NUM_NEIGH-1:0]                   neigh_sc_rsp_info;
  logic [NUM_NEIGH-1:0][`MIN_PER_N-1:0]                 uc_to_neigh_fcc;
  logic [NUM_NEIGH-1:0][`UC_FCC_TARGET_SIZE-1:0]        uc_to_neigh_fcc_target;
  logic [`THREADS_PER_S-1:0]                            esr_ipi_trigger;
  logic [`THREADS_PER_S-1:0]                            esr_ipi_redirect_trigger;
   logic [NUM_NEIGH-1:0]                                 flb_neigh_l2_req_valid;
   logic [NUM_NEIGH-1:0][`CSR_FL_BARRIER_RANGE]          flb_neigh_l2_req_data;
  logic [NUM_NEIGH-1:0]                                 flb_l2_neigh_resp_valid;
  logic [NUM_NEIGH-1:0]                                 flb_l2_neigh_resp_data;
   logic [`shire_cache_sm_gpio_width-1:0]                status_monitor_gpio;
  Neigh_ET_Link_us_monitor_t  [NUM_NEIGH+NUM_RBOX-1:0]  neigh_et_link_monitor;
   logic                                                 etlink_rbox_sm_enabled;
  rbox_dbg_us_monitor_t  [NUM_RBOX-1:0]                 rbox_dbg_sig;
   logic [`RBOX_SM_GPIO_WIDTH-1:0]                       rbox_sm_gpio;
   bpam_rc_tbox_ack_t  [NUM_NEIGH-1:0]                   bpam_rc_tbox_ack;
  sc_trace_packet_t                                     sc_trace_data;
  logic                                                 sc_trace_valid;
   APB_to_all_ESRs_t   [APB_SLAVES-1:0]                  APB_ESR_req;
  APB_from_all_ESRs_t [APB_SLAVES-1:0]                  APB_ESR_rsp;
  logic [`MIN_PER_S-1:0]                                esr_mtime_local_target_op;
  logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_nsleepin_op;
  logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_iso_op;
   logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_nsleepout_ip;
  logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_nsleepin_op;
  logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_iso_op;
   logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_nsleepout_ip;
  logic [`SHIRE_PLL_ESR_CTRL_BITS-1:0]                  esr_pll_ctrl_op;
  logic [`SHIRE_PLL_ESR_CONF_BITS-1:0]                  esp_pll_conf_op;
   logic                                                 esr_pll_busy_ip;
   logic [`SHIRE_PLL_DATA_BITS-1:0]                      esr_pll_rdata_ip;
   logic                                                 esr_pll_rrdy_ip;
   logic                                                 esr_pll_lock_ip;
  esr_clk_dly_ctl_t                                     esr_clk_dly_ctl_op;
  esr_dll_dly_est_ctl_t                                 esr_dll_dly_est_ctl_op;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_0_ip;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_1_ip;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_2_ip;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_3_ip;
  esr_clkmux_ctl_t                                      esr_shire_ctrl_clockmux_op;
  logic [`SHIRE_DLL_ESR_CTRL_BITS-1:0]                  esr_dll_ctrl_op;
  logic [`SHIRE_DLL_ESR_CONF_BITS-1:0]                  esp_dll_conf_op;
   logic                                                 esr_dll_busy_ip;
   logic [`SHIRE_DLL_DATA_BITS-1:0]                      esr_dll_rdata_ip;
   logic                                                 esr_dll_rrdy_ip;
   logic                                                 esr_dll_lock_ip;
  logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0]           esr_minion_mem_override;
  sc_master_axi_AR_channel_t [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AR;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_ARVALID;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AW;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AWVALID;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t  [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_W;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_WVALID;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t  [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_B;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_BVALID;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t  [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_R;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_RVALID;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_RREADY;
  sc_master_axi_AR_channel_t                            to_sys_mesh_master_axi_AR;
  logic                                                 to_sys_mesh_master_axi_ARVALID;
   logic                                                 to_sys_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t                            to_sys_mesh_master_axi_AW;
  logic                                                 to_sys_mesh_master_axi_AWVALID;
   logic                                                 to_sys_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t                             to_sys_mesh_master_axi_W;
  logic                                                 to_sys_mesh_master_axi_WVALID;
   logic                                                 to_sys_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t                             to_sys_mesh_master_axi_B;
   logic                                                 to_sys_mesh_master_axi_BVALID;
  logic                                                 to_sys_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t                             to_sys_mesh_master_axi_R;
   logic                                                 to_sys_mesh_master_axi_RVALID;
  logic                                                 to_sys_mesh_master_axi_RREADY;
   sc_slave_axi_AR_channel_t [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AR;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_ARVALID;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_ARREADY;
   sc_slave_axi_AW_channel_t [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AW;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AWVALID;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AWREADY;
   sc_slave_axi_W_channel_t  [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_W;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_WVALID;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_WREADY;
  sc_slave_axi_B_channel_t  [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_B;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_BVALID;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_BREADY;
  sc_slave_axi_R_channel_t  [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_R;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_RVALID;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_RREADY;
  sc_master_axi_AR_channel_t                            uc_to_l3_mesh_master_axi_AR;
  logic                                                 uc_to_l3_mesh_master_axi_ARVALID;
   logic                                                 uc_to_l3_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t                            uc_to_l3_mesh_master_axi_AW;
  logic                                                 uc_to_l3_mesh_master_axi_AWVALID;
   logic                                                 uc_to_l3_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t                             uc_to_l3_mesh_master_axi_W;
  logic                                                 uc_to_l3_mesh_master_axi_WVALID;
   logic                                                 uc_to_l3_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t                             uc_to_l3_mesh_master_axi_B;
   logic                                                 uc_to_l3_mesh_master_axi_BVALID;
  logic                                                 uc_to_l3_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t                             uc_to_l3_mesh_master_axi_R;
   logic                                                 uc_to_l3_mesh_master_axi_RVALID;
  logic                                                 uc_to_l3_mesh_master_axi_RREADY;
  sc_master_axi_AR_channel_t                            uc_to_sys_mesh_master_axi_AR;
  logic                                                 uc_to_sys_mesh_master_axi_ARVALID;
   logic                                                 uc_to_sys_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t                            uc_to_sys_mesh_master_axi_AW;
  logic                                                 uc_to_sys_mesh_master_axi_AWVALID;
   logic                                                 uc_to_sys_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t                             uc_to_sys_mesh_master_axi_W;
  logic                                                 uc_to_sys_mesh_master_axi_WVALID;
   logic                                                 uc_to_sys_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t                             uc_to_sys_mesh_master_axi_B;
   logic                                                 uc_to_sys_mesh_master_axi_BVALID;
  logic                                                 uc_to_sys_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t                             uc_to_sys_mesh_master_axi_R;
   logic                                                 uc_to_sys_mesh_master_axi_RVALID;
  logic                                                 uc_to_sys_mesh_master_axi_RREADY;
   sys_slave_axi_AR_channel_t                            sys_mesh_slave_axi_AR;
   logic                                                 sys_mesh_slave_axi_ARVALID;
  logic                                                 sys_mesh_slave_axi_ARREADY;
   sys_slave_axi_AW_channel_t                            sys_mesh_slave_axi_AW;
   logic                                                 sys_mesh_slave_axi_AWVALID;
  logic                                                 sys_mesh_slave_axi_AWREADY;
   sys_slave_axi_W_channel_t                             sys_mesh_slave_axi_W;
   logic                                                 sys_mesh_slave_axi_WVALID;
  logic                                                 sys_mesh_slave_axi_WREADY;
  sys_slave_axi_B_channel_t                             sys_mesh_slave_axi_B;
  logic                                                 sys_mesh_slave_axi_BVALID;
   logic                                                 sys_mesh_slave_axi_BREADY;
  sys_slave_axi_R_channel_t                             sys_mesh_slave_axi_R;
  logic                                                 sys_mesh_slave_axi_RVALID;
   logic                                                 sys_mesh_slave_axi_RREADY;
   logic [1:0]                                           sys_mesh_slave_axi_AWvcvalid;
  logic [1:0]                                           sys_mesh_slave_axi_AWcredit;
   logic [1:0]                                           sys_mesh_slave_axi_Wvcvalid;
  logic                                                 sbm_enable_read;
  logic                                                 sbm_enable_write;
   logic                                                 sbm_write_credit_return;
  sys_slave_axi_AR_channel_t                            sbm_sys_mesh_slave_axi_AR;
   logic                                                 sbm_sys_mesh_slave_axi_ARREADY;
  sys_slave_axi_AW_channel_t                            sbm_sys_mesh_slave_axi_AW;
   logic                                                 sbm_sys_mesh_slave_axi_AWREADY;
  sys_slave_axi_W_channel_t                             sbm_sys_mesh_slave_axi_W;
   logic                                                 sbm_sys_mesh_slave_axi_WREADY;
   sys_slave_axi_B_channel_t                             sbm_sys_mesh_slave_axi_B;
   logic                                                 sbm_sys_mesh_slave_axi_BVALID;
  logic                                                 sbm_sys_mesh_slave_axi_BREADY;
   sys_slave_axi_R_channel_t                             sbm_sys_mesh_slave_axi_R;
   logic                                                 sbm_sys_mesh_slave_axi_RVALID;
  logic                                                 sbm_sys_mesh_slave_axi_RREADY;
   logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_write;
   logic [NUM_NEIGH-1:0][`ICACHE_SRAM_ADD_WIDTH-1:0]     icache_f2_sram_req_addr;
   logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_valid;
  logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_ready;
  logic [NUM_NEIGH-1:0][`ICACHE_SRAM_DATA_WIDTH-1:0]    icache_f0_sram_resp_dout;
  logic [NUM_NEIGH-1:0]                                 icache_f0_sram_resp_valid;
   logic [NUM_NEIGH-1:0]                                 icache_f0_sram_resp_ready;
   logic                                                 dft__reset_byp_hv;
   logic                                                 dft__reset_hv;
   logic                                                 dft__reset_byp_lv;
   logic                                                 dft__reset_lv;
   logic                                                 dft__sram_clock;
   logic                                                 dft__clk_override;
   logic                                                 dft__mbist_en;
   logic                                                 dft__bist_insertion;
   et_tdr_cntl_t                                         dft__tdr_cntl;
   logic                                                 dft__tdr_tck;
   logic                                                 dft__tdr_tdi_sc;
  logic                                                 dft__tdr_tdo_sc;
  logic                                                 use_shire_tdr_clk_cntl;
  logic                                                 use_shire_tdr_reset_cntl;
  shire_tdr_clk_cntl_t                                  shire_tdr_clk_cntl;
  shire_tdr_reset_cntl_t                                shire_tdr_reset_cntl;
  logic                [NUM_BANKS-1:0]                  sc_neigh_l2hpf_req_valid;
  sc_neigh_l2hpf_req_t [NUM_BANKS-1:0]                  sc_neigh_l2hpf_req_info;
   logic                                                 vl_sms_shire_channel_wrck;
   logic                                                 vl_sms_shire_channel_wrstn;
   logic                                                 vl_sms_shire_channel_rst;
   logic                                                 vl_sms_shire_channel_wsi;
   logic                                                 vl_sms_shire_channel_selectwir;
   logic                                                 vl_sms_shire_channel_capturewr;
   logic                                                 vl_sms_shire_channel_shiftwr;
   logic                                                 vl_sms_shire_channel_updatewr;
  logic                                                 vl_sms_shire_channel_wso;
   logic                                                 dm0_shire_channel;
   logic                                                 dm1_shire_channel;
   logic                                                 dm2_shire_channel;

  logic [2:0] apb_sel_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) apb_sel_q <= '0;
    else if (apb_psel_i && !apb_penable_i) apb_sel_q <= apb_sel_i;
  end

  always_comb begin
    clock = clk_i;
    reset_w_icache = ~rst_w_icache_ext_ni;
    noc_clock = clk_i;

    debug_axi_port_ctl = '0;
    debug_and_or_tree_L2_in = esr_and_or_tree_L2_t'(debug_and_or_tree_l2_stim_i);
    bpam_run_control = '0;
    bpam_run_control.gpio.ndmreset = dft_scanmode_i;
    bpam_run_control.gpio.halt_req = dft_ram_rei_i;
    bpam_run_control.gpio.resume_req = dft_ram_wei_i;
    bpam_run_control.events.halt_req = dft_sram_clk_override_i;
    bpam_run_control.events.resume_req = dft_mbist_en_i;

    shire_phy_id = shire_phy_id_i;
    shire_id_reset_val = shire_id_reset_val_i;
    esr_icache_prefetch_done = esr_icache_prefetch_done_stim_i;
    neigh_sc_err_detected = neigh_sc_err_detected_i;
    neigh_sc_err_logged = neigh_sc_err_logged_i;
    for (int unsigned neigh_idx = 0; neigh_idx < NUM_NEIGH; neigh_idx++) begin
      esr_and_or_tree_L0[neigh_idx] = esr_and_or_tree_L0_t'(
          esr_and_or_tree_l0_flat_stim_i[neigh_idx*EsrAndOrTreeL0Bits +: EsrAndOrTreeL0Bits]);
    end
    esr_noc_interrupt_status_ip = {{`NOC_DBG_INT_NUM{1'b0}}, noc_err_int_srcs_i};

    neigh_sc_req_valid = '0;
    neigh_sc_req_valid[0] = neigh0_req_valid_i ? neigh0_req_dest_i : '0;
    neigh_sc_req_info = '{default: '0};
    neigh_sc_req_info[0].opcode = et_link_req_opcode_t'(neigh0_req_opcode_i);
    neigh_sc_req_info[0].size = et_link_size_t'(neigh0_req_size_i);
    neigh_sc_req_info[0].address = neigh0_req_addr_i;
    neigh_sc_req_info[0].qwen = neigh0_req_qwen_i;
    neigh_sc_rsp_ready = neigh_sc_rsp_ready_stim_i;

    flb_neigh_l2_req_valid = flb_neigh_l2_req_valid_i;
    flb_neigh_l2_req_data = flb_neigh_l2_req_data_i;
    status_monitor_gpio = '0;
    etlink_rbox_sm_enabled = 1'b0;
    rbox_sm_gpio = '0;
    for (int unsigned neigh_idx = 0; neigh_idx < NUM_NEIGH; neigh_idx++) begin
      bpam_rc_tbox_ack[neigh_idx] = bpam_rc_tbox_ack_t'(
          bpam_rc_tbox_ack_flat_stim_i[neigh_idx*BpamRcTboxAckBits +: BpamRcTboxAckBits]);
    end

    // APB_ESR_req is driven below by the original sbm_top slice.

    esr_pwr_ctrl_glb_nsleepout_ip = esr_pwr_ctrl_glb_nsleepout_stim_i;
    esr_pwr_ctrl_neigh_nsleepout_ip = esr_pwr_ctrl_neigh_nsleepout_stim_i;
    esr_pll_busy_ip = esr_pll_busy_stim_i;
    esr_pll_rdata_ip = esr_pll_rdata_stim_i;
    esr_pll_rrdy_ip = esr_pll_rrdy_stim_i;
    esr_pll_lock_ip = esr_pll_lock_stim_i;
    esr_dll_dly_est_sts_0_ip = esr_dll_dly_est_sts_t'(
        esr_dll_dly_est_sts_flat_stim_i[0*DllDlyEstStsBits +: DllDlyEstStsBits]);
    esr_dll_dly_est_sts_1_ip = esr_dll_dly_est_sts_t'(
        esr_dll_dly_est_sts_flat_stim_i[1*DllDlyEstStsBits +: DllDlyEstStsBits]);
    esr_dll_dly_est_sts_2_ip = esr_dll_dly_est_sts_t'(
        esr_dll_dly_est_sts_flat_stim_i[2*DllDlyEstStsBits +: DllDlyEstStsBits]);
    esr_dll_dly_est_sts_3_ip = esr_dll_dly_est_sts_t'(
        esr_dll_dly_est_sts_flat_stim_i[3*DllDlyEstStsBits +: DllDlyEstStsBits]);
    esr_dll_busy_ip = esr_dll_busy_stim_i;
    esr_dll_rdata_ip = esr_dll_rdata_stim_i;
    esr_dll_rrdy_ip = esr_dll_rrdy_stim_i;
    esr_dll_lock_ip = esr_dll_lock_stim_i;

    to_l3_mesh_master_axi_ARREADY = to_l3_axi_ar_ready_stim_i;
    to_l3_mesh_master_axi_AWREADY = to_l3_axi_aw_ready_stim_i;
    to_l3_mesh_master_axi_WREADY = to_l3_axi_w_ready_stim_i;
    to_l3_mesh_master_axi_B = '{default: '0};
    to_l3_mesh_master_axi_BVALID = to_l3_axi_b_valid_stim_i;
    to_l3_mesh_master_axi_R = '{default: '0};
    to_l3_mesh_master_axi_RVALID = to_l3_axi_r_valid_stim_i;
    for (int unsigned axi_idx = 0; axi_idx < NUM_L3_MASTER_PORTS; axi_idx++) begin
      to_l3_mesh_master_axi_B[axi_idx].ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
      to_l3_mesh_master_axi_B[axi_idx].RESP = axi_stim_i[1:0];
      to_l3_mesh_master_axi_R[axi_idx].ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
      to_l3_mesh_master_axi_R[axi_idx].RESP = axi_stim_i[3:2];
      to_l3_mesh_master_axi_R[axi_idx].LAST = axi_stim_i[4];
      to_l3_mesh_master_axi_R[axi_idx].DATA = {16{axi_stim_i}};
    end
    to_sys_mesh_master_axi_ARREADY = to_sys_axi_ar_ready_stim_i[0];
    to_sys_mesh_master_axi_AWREADY = to_sys_axi_aw_ready_stim_i[0];
    to_sys_mesh_master_axi_WREADY = to_sys_axi_w_ready_stim_i[0];
    to_sys_mesh_master_axi_B = '0;
    to_sys_mesh_master_axi_B.ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
    to_sys_mesh_master_axi_B.RESP = axi_stim_i[5:4];
    to_sys_mesh_master_axi_BVALID = to_sys_axi_b_valid_stim_i[0];
    to_sys_mesh_master_axi_R = '0;
    to_sys_mesh_master_axi_R.ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
    to_sys_mesh_master_axi_R.RESP = axi_stim_i[7:6];
    to_sys_mesh_master_axi_R.LAST = axi_stim_i[8];
    to_sys_mesh_master_axi_R.DATA = {16{axi_stim_i}};
    to_sys_mesh_master_axi_RVALID = to_sys_axi_r_valid_stim_i[0];
    l3_mesh_slave_axi_AR = '{default: '0};
    l3_mesh_slave_axi_ARVALID = l3_axi_ar_valid_stim_i;
    l3_mesh_slave_axi_AW = '{default: '0};
    l3_mesh_slave_axi_AWVALID = l3_axi_aw_valid_stim_i;
    l3_mesh_slave_axi_W = '{default: '0};
    l3_mesh_slave_axi_WVALID = l3_axi_w_valid_stim_i;
    l3_mesh_slave_axi_BREADY = l3_axi_b_ready_stim_i;
    l3_mesh_slave_axi_RREADY = l3_axi_r_ready_stim_i;
    for (int unsigned l3_idx = 0; l3_idx < NUM_L3_SLAVE_PORTS; l3_idx++) begin
      l3_mesh_slave_axi_AR[l3_idx].ID = axi_stim_i[`SC_MESH_SLAVE_AXI_ID_SIZE-1:0];
      l3_mesh_slave_axi_AR[l3_idx].ADDR = {8'h00, axi_stim_i};
      l3_mesh_slave_axi_AR[l3_idx].LEN = axi_stim_i[15:8];
      l3_mesh_slave_axi_AR[l3_idx].SIZE = axi_stim_i[10:8];
      l3_mesh_slave_axi_AR[l3_idx].BURST = 2'b01;
      l3_mesh_slave_axi_AR[l3_idx].LOCK = axi_stim_i[11];
      l3_mesh_slave_axi_AR[l3_idx].CACHE = axi_stim_i[15:12];
      l3_mesh_slave_axi_AR[l3_idx].PROT = axi_stim_i[18:16];
      l3_mesh_slave_axi_AR[l3_idx].QOS = axi_stim_i[22:19];
      l3_mesh_slave_axi_AR[l3_idx].USER = axi_stim_i[23];
      l3_mesh_slave_axi_AW[l3_idx].ID = axi_stim_i[`SC_MESH_SLAVE_AXI_ID_SIZE-1:0];
      l3_mesh_slave_axi_AW[l3_idx].ADDR = {8'h00, axi_stim_i};
      l3_mesh_slave_axi_AW[l3_idx].LEN = axi_stim_i[31:24];
      l3_mesh_slave_axi_AW[l3_idx].SIZE = axi_stim_i[26:24];
      l3_mesh_slave_axi_AW[l3_idx].BURST = 2'b01;
      l3_mesh_slave_axi_AW[l3_idx].LOCK = axi_stim_i[27];
      l3_mesh_slave_axi_AW[l3_idx].CACHE = axi_stim_i[7:4];
      l3_mesh_slave_axi_AW[l3_idx].PROT = axi_stim_i[10:8];
      l3_mesh_slave_axi_AW[l3_idx].QOS = axi_stim_i[14:11];
      l3_mesh_slave_axi_AW[l3_idx].USER = axi_stim_i[19:15];
      l3_mesh_slave_axi_W[l3_idx].DATA = {16{axi_stim_i}};
      l3_mesh_slave_axi_W[l3_idx].STRB = {64{axi_stim_i[0]}};
      l3_mesh_slave_axi_W[l3_idx].LAST = axi_stim_i[1];
    end
    uc_to_l3_mesh_master_axi_ARREADY = uc_to_l3_axi_ar_ready_stim_i;
    uc_to_l3_mesh_master_axi_AWREADY = uc_to_l3_axi_aw_ready_stim_i;
    uc_to_l3_mesh_master_axi_WREADY = uc_to_l3_axi_w_ready_stim_i;
    uc_to_l3_mesh_master_axi_B = '0;
    uc_to_l3_mesh_master_axi_B.ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
    uc_to_l3_mesh_master_axi_B.RESP = axi_stim_i[9:8];
    uc_to_l3_mesh_master_axi_BVALID = uc_to_l3_axi_b_valid_stim_i;
    uc_to_l3_mesh_master_axi_R = '0;
    uc_to_l3_mesh_master_axi_R.ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
    uc_to_l3_mesh_master_axi_R.RESP = axi_stim_i[11:10];
    uc_to_l3_mesh_master_axi_R.LAST = axi_stim_i[12];
    uc_to_l3_mesh_master_axi_R.DATA = {16{axi_stim_i}};
    uc_to_l3_mesh_master_axi_RVALID = uc_to_l3_axi_r_valid_stim_i;
    uc_to_sys_mesh_master_axi_ARREADY = uc_to_sys_axi_ar_ready_stim_i;
    uc_to_sys_mesh_master_axi_AWREADY = uc_to_sys_axi_aw_ready_stim_i;
    uc_to_sys_mesh_master_axi_WREADY = uc_to_sys_axi_w_ready_stim_i;
    uc_to_sys_mesh_master_axi_B = '0;
    uc_to_sys_mesh_master_axi_B.ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
    uc_to_sys_mesh_master_axi_B.RESP = axi_stim_i[13:12];
    uc_to_sys_mesh_master_axi_BVALID = uc_to_sys_axi_b_valid_stim_i;
    uc_to_sys_mesh_master_axi_R = '0;
    uc_to_sys_mesh_master_axi_R.ID = axi_stim_i[`SC_MESH_MASTER_AXI_ID_SIZE-1:0];
    uc_to_sys_mesh_master_axi_R.RESP = axi_stim_i[15:14];
    uc_to_sys_mesh_master_axi_R.LAST = axi_stim_i[16];
    uc_to_sys_mesh_master_axi_R.DATA = {16{axi_stim_i}};
    uc_to_sys_mesh_master_axi_RVALID = uc_to_sys_axi_r_valid_stim_i;
    sys_mesh_slave_axi_AR = '0;
    sys_mesh_slave_axi_AR.ID = sys_id_stim_i[`SC_MESH_SLAVE_AXI_ID_SIZE-1:0];
    sys_mesh_slave_axi_AR.ADDR = sys_addr_stim_i;
    sys_mesh_slave_axi_AR.LEN = sys_len_stim_i;
    sys_mesh_slave_axi_AR.SIZE = sys_size_stim_i;
    sys_mesh_slave_axi_AR.BURST = 2'b01;
    sys_mesh_slave_axi_AR.LOCK = axi_stim_i[19];
    sys_mesh_slave_axi_AR.CACHE = axi_stim_i[23:20];
    sys_mesh_slave_axi_AR.PROT = axi_stim_i[26:24];
    sys_mesh_slave_axi_AR.QOS = axi_stim_i[30:27];
    sys_mesh_slave_axi_AR.USER = axi_stim_i[31];
    sys_mesh_slave_axi_ARVALID = sys_axi_ar_valid_stim_i;
    sys_mesh_slave_axi_AW = '0;
    sys_mesh_slave_axi_AW.ID = sys_id_stim_i[`SC_MESH_SLAVE_AXI_ID_SIZE-1:0];
    sys_mesh_slave_axi_AW.ADDR = sys_addr_stim_i;
    sys_mesh_slave_axi_AW.LEN = sys_len_stim_i;
    sys_mesh_slave_axi_AW.SIZE = sys_size_stim_i;
    sys_mesh_slave_axi_AW.BURST = 2'b01;
    sys_mesh_slave_axi_AW.LOCK = axi_stim_i[19];
    sys_mesh_slave_axi_AW.CACHE = axi_stim_i[23:20];
    sys_mesh_slave_axi_AW.PROT = axi_stim_i[26:24];
    sys_mesh_slave_axi_AW.QOS = axi_stim_i[30:27];
    sys_mesh_slave_axi_AW.USER = axi_stim_i[4:0];
    sys_mesh_slave_axi_AWVALID = sys_axi_aw_valid_stim_i;
    sys_mesh_slave_axi_W = '0;
    sys_mesh_slave_axi_W.DATA = sys_wdata_stim_i;
    sys_mesh_slave_axi_W.STRB = '1;
    sys_mesh_slave_axi_W.LAST = 1'b1;
    sys_mesh_slave_axi_WVALID = sys_axi_w_valid_stim_i;
    sys_mesh_slave_axi_BREADY = sys_axi_b_ready_stim_i;
    sys_mesh_slave_axi_RREADY = sys_axi_r_ready_stim_i;
    sys_mesh_slave_axi_AWvcvalid = sys_axi_aw_vcvalid_stim_i;
    sys_mesh_slave_axi_Wvcvalid = sys_axi_w_vcvalid_stim_i;
    // The original sbm_top slice drives the SBM response/ready channels back into shire_channel_orig.

    icache_f2_sram_req_write = icache_req_write_i;
    icache_f2_sram_req_addr = icache_req_addr_i;
    icache_f2_sram_req_valid = icache_req_valid_i;
    icache_f0_sram_resp_ready = icache_resp_ready_i;
    dft__reset_byp_hv = dft_scanmode_i;
    dft__reset_hv = !dft_scan_reset_ni;
    dft__reset_byp_lv = dft_scanmode_i;
    dft__reset_lv = !dft_scan_reset_ni;
    dft__sram_clock = dft_sram_clk_i;
    dft__clk_override = dft_sram_clk_override_i;
    dft__mbist_en = dft_mbist_en_i;
    dft__bist_insertion = 1'b0;
    dft__tdr_cntl = '0;
    dft__tdr_tck = clk_i;
    dft__tdr_tdi_sc = 1'b0;
    vl_sms_shire_channel_wrck = 1'b0;
    vl_sms_shire_channel_wrstn = 1'b1;
    vl_sms_shire_channel_rst = 1'b0;
    vl_sms_shire_channel_wsi = 1'b0;
    vl_sms_shire_channel_selectwir = 1'b0;
    vl_sms_shire_channel_capturewr = 1'b0;
    vl_sms_shire_channel_shiftwr = 1'b0;
    vl_sms_shire_channel_updatewr = 1'b0;
    dm0_shire_channel = 1'b0;
    dm1_shire_channel = 1'b0;
    dm2_shire_channel = 1'b0;
  end



  logic reset_cold_seed;
  logic reset_warm_seed;
  logic reset_system_seed_ni;
  logic reset_system_debug_seed_ni;
  logic orig_rst_system_lv_no;
  logic orig_rst_system_debug_lv_no;
  logic orig_reset_noc_lv;
  logic orig_reset_noc_debug_lv;

  assign reset_cold_seed = !rst_ni | !rst_c_ext_ni;
  assign reset_warm_seed = !rst_ni | !rst_w_ext_ni;
  assign reset_system_seed_ni = rst_ni & rst_noc_ext_ni;
  assign reset_system_debug_seed_ni = rst_ni & rst_d_ext_ni;

  esr_pkg::esr_ms_dmctrl_t orig_dmctrl_in;
  esr_pkg::esr_ms_dmctrl_t orig_dmctrl_q;
  always_comb begin
    orig_dmctrl_in = '0;
    orig_dmctrl_in.dmactive = dft_ram_rei_i;
    orig_dmctrl_in.ndmreset = dft_ram_wei_i;
  end

  shire_dmctrl u_orig_dmctrl_adapter (
    .clk_i    (clock),
    .rst_ni   (~reset_c),
    .dmctrl_i (orig_dmctrl_in),
    .dmctrl_o (orig_dmctrl_q)
  );

  sys_gasket_reset u_orig_reset (
    .clock            (clock),
    .reset_warm_in    (reset_warm_seed),
    .gpio_ndmreset_in (bpam_run_control.gpio.ndmreset),
    .ndmreset_in      (orig_dmctrl_q.ndmreset),
    .dmactive_in      (orig_dmctrl_q.dmactive),
    .reset_cold_in    (reset_cold_seed),
    .dft__reset_byp   (dft__reset_byp_hv),
    .dft__reset       (dft__reset_hv),
    .reset_w          (reset_w),
    .reset_c          (reset_c),
    .reset_d          (reset_d)
  );

  logic reset_n_system_sync;
  logic reset_n_system_debug_sync;
  sys_gasket_noc_reset_sync u_orig_noc_reset_sync (
    .noc_clock                 (noc_clock),
    .reset_n_system            (reset_system_seed_ni),
    .reset_n_system_debug      (reset_system_debug_seed_ni),
    .dft__reset_byp            (dft__reset_byp_hv),
    .dft__reset                (dft__reset_hv),
    .reset_n_system_sync       (reset_n_system_sync),
    .reset_n_system_debug_sync (reset_n_system_debug_sync)
  );

  sys_gasket_noc_reset u_orig_noc_reset (
    .clock                (noc_clock),
    .reset_n_system       (reset_n_system_sync),
    .reset_n_system_debug (reset_n_system_debug_sync),
    .dft__reset_byp       (dft__reset_byp_lv),
    .dft__reset           (dft__reset_lv),
    .reset_n_noc          (orig_rst_system_lv_no),
    .reset_n_noc_debug    (orig_rst_system_debug_lv_no),
    .reset_noc            (orig_reset_noc_lv),
    .reset_noc_debug      (orig_reset_noc_debug_lv)
  );
  assign noc_reset = orig_reset_noc_lv;

  APB_to_all_ESRs_t [shire_sbm_pkg::ShireApbInterfaces-1:0] orig_sbm_apb_req;
  APB_from_all_ESRs_t [shire_sbm_pkg::ShireApbInterfaces-1:0] orig_sbm_apb_rsp;
  logic orig_bpam_pready;
  logic orig_bpam_pslverr;
  logic [shire_sbm_pkg::ApbDataWidth-1:0] orig_bpam_prdata;

  always_comb begin
    APB_ESR_req = '{default: '0};
    orig_sbm_apb_rsp = '{default: '0};
    for (int unsigned bank = 0; bank < NUM_BANKS; bank++) begin
      APB_ESR_req[bank] = orig_sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank];
      orig_sbm_apb_rsp[shire_sbm_pkg::BankRangeLo + bank] = APB_ESR_rsp[bank];
    end
    APB_ESR_req[NUM_BANKS] = orig_sbm_apb_req[shire_sbm_pkg::ShireRangeLo];
    orig_sbm_apb_rsp[shire_sbm_pkg::ShireRangeLo] = APB_ESR_rsp[NUM_BANKS];
    APB_ESR_req[NUM_BANKS + 1] = orig_sbm_apb_req[shire_sbm_pkg::RboxRangeLo];
    orig_sbm_apb_rsp[shire_sbm_pkg::RboxRangeLo] = APB_ESR_rsp[NUM_BANKS + 1];
    APB_ESR_req[NUM_BANKS + 2] = orig_sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId];
    orig_sbm_apb_rsp[shire_sbm_pkg::IcacheApbInterfaceId] = APB_ESR_rsp[NUM_BANKS + 2];
    for (int unsigned neigh = 0; neigh < NUM_NEIGH; neigh++) begin
      orig_sbm_apb_rsp[neigh].apb_pready = 1'b1;
      orig_sbm_apb_rsp[neigh].apb_prdata = 64'h5a5a_0000_0000_0000 | {62'h0, neigh[1:0]};
      orig_sbm_apb_rsp[neigh].apb_pslverr = 1'b0;
    end
  end

  sbm_top_orig #(
    .sbm_interfaces   (shire_sbm_pkg::SbmInterfaces),
    .shire_interfaces (shire_sbm_pkg::ShireApbInterfaces),
    .sbm_apb_addr     (shire_sbm_pkg::SbmApbAddrWidth),
    .sbm_apb_data     (shire_sbm_pkg::ApbDataWidth)
  ) u_orig_sbm (
    .clock (clock),
    .reset_c (reset_c),
    .reset_w (reset_w),
    .reset_d (reset_d),
    .reset_neigh (reset_c_shire_scs),
    .reset_rbox (reset_rbox),
    .reset_sc (reset_sc),
    .shire_virtual_id (shire_id[0]),
    .sbm_enable_read (sbm_enable_read),
    .sbm_enable_write (sbm_enable_write),
    .sbm_write_credit_return (sbm_write_credit_return),
    .msg_lock_en_esperanto (1'b0),
    .bpam_paddr ('0),
    .bpam_penable (1'b0),
    .bpam_prdata (orig_bpam_prdata),
    .bpam_pready (orig_bpam_pready),
    .bpam_pslverr (orig_bpam_pslverr),
    .bpam_psel (1'b0),
    .bpam_pwdata ('0),
    .bpam_pwrite (1'b0),
    .sbm_mesh_slave_axi_AR (sbm_sys_mesh_slave_axi_AR),
    .sbm_mesh_slave_axi_ARREADY (sbm_sys_mesh_slave_axi_ARREADY),
    .sbm_mesh_slave_axi_AW (sbm_sys_mesh_slave_axi_AW),
    .sbm_mesh_slave_axi_AWREADY (sbm_sys_mesh_slave_axi_AWREADY),
    .sbm_mesh_slave_axi_W (sbm_sys_mesh_slave_axi_W),
    .sbm_mesh_slave_axi_WREADY (sbm_sys_mesh_slave_axi_WREADY),
    .sbm_mesh_slave_axi_B (sbm_sys_mesh_slave_axi_B),
    .sbm_mesh_slave_axi_BVALID (sbm_sys_mesh_slave_axi_BVALID),
    .sbm_mesh_slave_axi_BREADY (sbm_sys_mesh_slave_axi_BREADY),
    .sbm_mesh_slave_axi_R (sbm_sys_mesh_slave_axi_R),
    .sbm_mesh_slave_axi_RVALID (sbm_sys_mesh_slave_axi_RVALID),
    .sbm_mesh_slave_axi_RREADY (sbm_sys_mesh_slave_axi_RREADY),
    .APB_ESR_req (orig_sbm_apb_req),
    .APB_ESR_rsp (orig_sbm_apb_rsp)
  );

  shire_ioshire_noc_ints_orig u_orig_ioshire_noc_ints (
    .noc_clock               (clk_i),
    .reset_noc               (~orig_rst_system_lv_no),
    .noc_err_int_srcs_lo     (noc_err_int_srcs_i[`NOC_INT_NUM-1:0]),
    .noc_dbg_err_int_srcs_lo ('0),
    .noc_all_err_int_srcs_lo (orig_noc_all_err_int_srcs_o),
    .ioshire_noc_err_int     (orig_ioshire_noc_err_int_o)
  );

  logic [NUM_NEIGH-1:0][NUM_NEIGH-2:0] orig_coop_tload_slv_rdy_in_valid;
  logic [NUM_NEIGH-1:0][NUM_NEIGH-2:0][`SHIRE_COOP_ID_RANGE] orig_coop_tload_mst_done_in_coop_id;
  logic [NUM_NEIGH-1:0][NUM_NEIGH-2:0] orig_coop_tload_mst_done_in_valid;

  shire_coop_tload_bus_orig u_orig_coop (
    .coop_tload_slv_rdy_out_data     ('0),
    .coop_tload_slv_rdy_out_valid    (coop_slv_valid_i),
    .coop_tload_slv_rdy_in_data      (),
    .coop_tload_slv_rdy_in_valid     (orig_coop_tload_slv_rdy_in_valid),
    .coop_tload_mst_done_out_coop_id (coop_done_id_i),
    .coop_tload_mst_done_out_valid   (coop_done_valid_i),
    .coop_tload_mst_done_in_coop_id  (orig_coop_tload_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid    (orig_coop_tload_mst_done_in_valid)
  );

  shire_channel_orig #(
    .DV_SC_STUB              (1'b0),
    .NUM_RBOX                (NUM_RBOX),
    .NUM_BANKS               (NUM_BANKS),
    .NUM_L3_MASTER_PORTS     (NUM_L3_MASTER_PORTS),
    .NUM_L3_SLAVE_PORTS      (NUM_L3_SLAVE_PORTS),
    .ICACHE_MEMS_IMPLEMENTED (1),
    .L2HPF_IMPLEMENTED       (0),
    .APB_SLAVES              (APB_SLAVES)
  ) u_orig (.*);

  /* verilator lint_off PINMISSING */  // This cosim observes many wrapper TB internals directly; unused observation ports stay unconnected.
  shire_channel_wrap_tb u_new (
    .clk_i,
    .rst_ni,
    .rst_c_ext_ni,
    .rst_w_ext_ni,
    .rst_d_ext_ni,
    .rst_noc_ext_ni,
    .rst_w_icache_ext_ni,
    .dft_scanmode_i,
    .dft_scan_reset_ni,
    .dft_sram_clk_i,
    .dft_sram_clk_override_i,
    .dft_ram_rei_i,
    .dft_ram_wei_i,
    .dft_mbist_en_i,
    .shire_id_reset_val_i,
    .shire_phy_id_i,
    .apb_sel_i,
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o(new_apb_pready_o),
    .apb_prdata_o(new_apb_prdata_o),
    .apb_pslverr_o(new_apb_pslverr_o),
    .noc_err_int_srcs_i,
    .neigh_sc_err_detected_i,
    .neigh_sc_err_logged_i,
    .flb_neigh_l2_req_valid_i,
    .flb_neigh_l2_req_data_i,
    .neigh0_req_valid_i,
    .neigh0_req_dest_i,
    .neigh0_req_addr_i,
    .neigh0_req_opcode_i,
    .neigh0_req_size_i,
    .neigh0_req_qwen_i,
    .icache_req_valid_i,
    .icache_req_write_i,
    .icache_req_addr_i,
    .icache_resp_ready_i,
    .coop_slv_valid_i,
    .coop_done_id_i,
    .coop_done_valid_i,
    .esr_icache_prefetch_done_stim_i,
    .esr_and_or_tree_l0_flat_stim_i,
    .debug_and_or_tree_l2_stim_i,
    .bpam_rc_tbox_ack_flat_stim_i,
    .esr_pwr_ctrl_glb_nsleepout_stim_i,
    .esr_pwr_ctrl_neigh_nsleepout_stim_i,
    .esr_pll_busy_stim_i,
    .esr_pll_rdata_stim_i,
    .esr_pll_rrdy_stim_i,
    .esr_pll_lock_stim_i,
    .esr_dll_dly_est_sts_flat_stim_i,
    .esr_dll_busy_stim_i,
    .esr_dll_rdata_stim_i,
    .esr_dll_rrdy_stim_i,
    .esr_dll_lock_stim_i,
    .status_monitor_bank_sel_i,
    .neigh_sc_rsp_ready_stim_i,
    .to_l3_axi_ar_ready_stim_i,
    .to_l3_axi_aw_ready_stim_i,
    .to_l3_axi_w_ready_stim_i,
    .to_l3_axi_b_valid_stim_i,
    .to_l3_axi_r_valid_stim_i,
    .to_sys_axi_ar_ready_stim_i,
    .to_sys_axi_aw_ready_stim_i,
    .to_sys_axi_w_ready_stim_i,
    .to_sys_axi_b_valid_stim_i,
    .to_sys_axi_r_valid_stim_i,
    .l3_axi_ar_valid_stim_i,
    .l3_axi_aw_valid_stim_i,
    .l3_axi_w_valid_stim_i,
    .l3_axi_b_ready_stim_i,
    .l3_axi_r_ready_stim_i,
    .uc_to_l3_axi_ar_ready_stim_i,
    .uc_to_l3_axi_aw_ready_stim_i,
    .uc_to_l3_axi_w_ready_stim_i,
    .uc_to_l3_axi_b_valid_stim_i,
    .uc_to_l3_axi_r_valid_stim_i,
    .uc_to_sys_axi_ar_ready_stim_i,
    .uc_to_sys_axi_aw_ready_stim_i,
    .uc_to_sys_axi_w_ready_stim_i,
    .uc_to_sys_axi_b_valid_stim_i,
    .uc_to_sys_axi_r_valid_stim_i,
    .sys_axi_ar_valid_stim_i,
    .sys_axi_aw_valid_stim_i,
    .sys_axi_w_valid_stim_i,
    .sys_axi_b_ready_stim_i,
    .sys_axi_r_ready_stim_i,
    .sys_axi_aw_vcvalid_stim_i,
    .sys_axi_w_vcvalid_stim_i,
    .sbm_write_credit_return_stim_i,
    .sbm_sys_axi_ar_ready_stim_i,
    .sbm_sys_axi_aw_ready_stim_i,
    .sbm_sys_axi_w_ready_stim_i,
    .sbm_sys_axi_b_valid_stim_i,
    .sbm_sys_axi_r_valid_stim_i,
    .axi_stim_i,
    .sys_id_stim_i,
    .sys_addr_stim_i,
    .sys_wdata_stim_i,
    .sys_len_stim_i,
    .sys_size_stim_i,
    .rst_c_shire_no_o(new_rst_c_shire_no_o),
    .rst_w_shire_no_o(new_rst_w_shire_no_o),
    .rst_d_shire_no_o(new_rst_d_shire_no_o),
    .rst_c_shire_scs_no_o(new_rst_c_shire_scs_no_o),
    .rst_sc_no_o(new_rst_sc_no_o),
    .rst_rbox_no_o(new_rst_rbox_no_o),
    .rst_warm_to_neigh_no_o(new_rst_warm_to_neigh_no_o),
    .dmctrl_to_neigh_flat_o(new_dmctrl_to_neigh_flat_o),
    .plic_meip_obs_o(new_plic_meip_to_neigh_o),
    .plic_seip_obs_o(new_plic_seip_to_neigh_o),
    .shire_id_o(new_shire_id_o),
    .tbox_id_flat_o(new_tbox_id_o),
    .tbox_en_o(new_tbox_en_o),
    .esr_thread0_enable_o(new_esr_thread0_enable_o),
    .esr_thread1_enable_o(new_esr_thread1_enable_o),
    .esr_minion_features_flat_o(new_esr_minion_features_o),
    .esr_icache_prefetch_conf_flat_o(new_icache_prefetch_conf_o),
    .esr_icache_prefetch_start_flat_o(new_icache_prefetch_start_o),
    .esr_ipi_trigger_o(new_esr_ipi_trigger_o),
    .esr_ipi_redirect_trigger_o(new_esr_ipi_redirect_trigger_o),
    .esr_mtime_local_target_o(new_esr_mtime_local_target_o),
    .esr_shire_coop_mode_o(new_esr_shire_coop_mode_o),
    .debug_and_or_tree_l2_flat_o(new_debug_and_or_tree_l2_o),
    .bpam_run_control_neigh_flat_o(new_bpam_run_control_neigh_o),
    .ioshire_log_err_int_o(new_ioshire_log_err_int_o),
    .ioshire_noc_err_int_o(new_ioshire_noc_err_int_o),
    .noc_all_err_int_srcs_o(new_noc_all_err_int_srcs_o),
    .uc_to_neigh_fcc_o(new_uc_to_neigh_fcc_o),
    .uc_to_neigh_fcc_target_o(new_uc_to_neigh_fcc_target_o),
    .flb_l2_neigh_resp_valid_o(new_flb_l2_neigh_resp_valid_o),
    .flb_l2_neigh_resp_data_o(new_flb_l2_neigh_resp_data_o),
    .neigh_sc_req_ready_o(new_neigh_sc_req_ready_o),
    .neigh_sc_rsp_valid_o(new_neigh_sc_rsp_valid_o),
    .icache_req_ready_o(new_icache_req_ready_o),
    .icache_resp_valid_o(new_icache_resp_valid_o),
    .sc_neigh_l2hpf_req_valid_flat_o(new_l2hpf_req_valid_o),
    .sc_trace_valid_flat_o(new_sc_trace_valid_o),
    .dft_hv_flat_o(),
    .pwr_ctrl_glb_nsleepin_o(),
    .pwr_ctrl_glb_iso_o(),
    .pwr_ctrl_neigh_nsleepin_o(),
    .pwr_ctrl_neigh_iso_o(),
    .ram_cfg_flat_o(new_ram_cfg_flat_o),
    .clk_gate_ctrl_flat_o(new_clk_gate_ctrl_flat_o),
    .debug_clk_gate_ctrl_o(new_debug_clk_gate_ctrl_o),
    .coop_slv_valid_o(new_coop_slv_valid_o),
    .coop_done_id_o(new_coop_done_id_o),
    .coop_done_valid_o(new_coop_done_valid_o),
    .sbm_enable_read_o(new_sbm_enable_read_o),
    .sbm_enable_write_o(new_sbm_enable_write_o)
  );
  /* verilator lint_on PINMISSING */

  assign orig_rst_c_shire_no_o = ~reset_c_shire;
  assign orig_rst_w_shire_no_o = ~reset_w_shire;
  assign orig_rst_d_shire_no_o = ~reset_d_shire;
  assign orig_rst_c_shire_scs_no_o = ~reset_c_shire_scs;
  assign orig_rst_sc_no_o = ~reset_sc;
  assign orig_rst_rbox_no_o = ~reset_rbox;
  assign orig_rst_system_lv_no_o = orig_rst_system_lv_no;
  assign new_rst_system_lv_no_o = u_new.rst_system_lv_no_o;
  assign orig_rst_system_debug_lv_no_o = orig_rst_system_debug_lv_no;
  assign new_rst_system_debug_lv_no_o = u_new.rst_system_debug_lv_no_o;
  assign orig_rst_warm_to_neigh_no_o = {NUM_NEIGH{~reset_warm_seed}};
  assign orig_clk_neigh_o = u_new.clk_neigh_o;
  assign new_clk_neigh_o = u_new.clk_neigh_o;
  assign orig_clk_shire_to_neigh_o = {NUM_NEIGH{u_new.clk_shire_o}};
  assign new_clk_shire_to_neigh_o = u_new.clk_shire_to_neigh_o;
  assign orig_clk_shire_o = u_new.clk_shire_o;
  assign new_clk_shire_o = u_new.clk_shire_o;
  assign orig_clk_shire_debug_o = u_new.clk_shire_debug_o;
  assign new_clk_shire_debug_o = u_new.clk_shire_debug_o;
  assign orig_plic_meip_to_neigh_o = {NUM_NEIGH{1'b1}};
  assign orig_plic_seip_to_neigh_o = '0;
  assign orig_apb_neigh_psel_o = {orig_sbm_apb_req[3].apb_psel, orig_sbm_apb_req[2].apb_psel,
                                  orig_sbm_apb_req[1].apb_psel, orig_sbm_apb_req[0].apb_psel};
  assign new_apb_neigh_psel_o = u_new.apb_neigh_psel_o;
  assign orig_apb_neigh_pwrite_o = {orig_sbm_apb_req[3].apb_pwrite, orig_sbm_apb_req[2].apb_pwrite,
                                    orig_sbm_apb_req[1].apb_pwrite, orig_sbm_apb_req[0].apb_pwrite};
  assign new_apb_neigh_pwrite_o = u_new.apb_neigh_pwrite_o;
  assign orig_apb_neigh0_paddr_o = orig_sbm_apb_req[0].apb_paddr[neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0];
  assign new_apb_neigh0_paddr_o = u_new.apb_neigh0_paddr_o;
  assign orig_apb_neigh0_pwdata_o = orig_sbm_apb_req[0].apb_pwdata;
  assign new_apb_neigh0_pwdata_o = u_new.apb_neigh0_pwdata_o;
  assign orig_coop_slv_valid_o = orig_coop_tload_slv_rdy_in_valid;
  assign orig_coop_done_id_o = orig_coop_tload_mst_done_in_coop_id;
  assign orig_coop_done_valid_o = orig_coop_tload_mst_done_in_valid;
  assign orig_shire_id_o = shire_id;
  assign orig_tbox_id_o = tbox_id;
  assign orig_tbox_en_o = tbox_en;
  assign orig_esr_thread0_enable_o = esr_thread0_enable;
  assign orig_esr_thread1_enable_o = esr_thread1_enable;
  assign orig_icache_prefetch_start_o = esr_icache_prefetch_start;
  assign orig_esr_ipi_trigger_o = esr_ipi_trigger;
  assign orig_esr_ipi_redirect_trigger_o = esr_ipi_redirect_trigger;
  assign orig_esr_mtime_local_target_o = esr_mtime_local_target_op;
  assign orig_esr_shire_coop_mode_o = esr_shire_coop_mode;
  assign orig_debug_and_or_tree_l2_o = debug_and_or_tree_L2_out;
  assign orig_ioshire_log_err_int_o = ioshire_log_err_int;
  assign orig_uc_to_neigh_fcc_o = uc_to_neigh_fcc;
  assign orig_uc_to_neigh_fcc_target_o = uc_to_neigh_fcc_target;
  assign orig_flb_l2_neigh_resp_valid_o = flb_l2_neigh_resp_valid;
  assign orig_flb_l2_neigh_resp_data_o = flb_l2_neigh_resp_data;
  assign orig_apb_pready_o = APB_ESR_rsp[apb_sel_q].apb_pready;
  assign orig_apb_prdata_o = APB_ESR_rsp[apb_sel_q].apb_prdata;
  assign orig_apb_pslverr_o = APB_ESR_rsp[apb_sel_q].apb_pslverr;
  assign orig_sbm_enable_read_o = sbm_enable_read;
  assign orig_sbm_enable_write_o = sbm_enable_write;
  assign orig_neigh_sc_req_ready_o = neigh_sc_req_ready;
  assign orig_neigh_sc_rsp_valid_o = neigh_sc_rsp_valid;
  assign orig_icache_req_ready_o = icache_f2_sram_req_ready;
  assign orig_icache_resp_valid_o = icache_f0_sram_resp_valid;
  assign orig_l2hpf_req_valid_o = sc_neigh_l2hpf_req_valid;
  assign orig_sc_trace_valid_o = sc_trace_valid;
  assign orig_clk_gate_ctrl_flat_o = esr_clk_gate_ctrl_op;
  assign orig_debug_clk_gate_ctrl_o = esr_debug_clk_gate_ctrl_op;

  for (genvar out_idx = 0; out_idx < NUM_NEIGH; out_idx++) begin : gen_orig_flat_outputs
    assign orig_dmctrl_to_neigh_flat_o[out_idx*DmctrlBits +: DmctrlBits] = orig_dmctrl_q;
    assign orig_esr_minion_features_o[out_idx] = esr_minion_features[out_idx];
    assign orig_icache_prefetch_conf_o[out_idx] = esr_icache_prefetch_conf[out_idx];
    assign orig_bpam_run_control_neigh_o[out_idx] = bpam_run_control_neigh_op[out_idx];
    assign orig_neigh_sc_rsp_info_flat_o[out_idx*EtRspBits +: EtRspBits] =
        neigh_sc_rsp_info[out_idx];
    assign new_neigh_sc_rsp_info_flat_o[out_idx*EtRspBits +: EtRspBits] =
        u_new.neigh_sc_rsp_info_o[out_idx];
    assign orig_icache_resp_dout_flat_o[out_idx*IcacheRespBits +: IcacheRespBits] =
        icache_f0_sram_resp_dout[out_idx];
    assign new_icache_resp_dout_flat_o[out_idx*IcacheRespBits +: IcacheRespBits] =
        u_new.icache_resp_dout_o[out_idx];
  end

  for (genvar apb_out_idx = 0; apb_out_idx < APB_SLAVES; apb_out_idx++) begin : gen_apb_outputs
    assign orig_apb_pready_all_o[apb_out_idx] = APB_ESR_rsp[apb_out_idx].apb_pready;
    assign new_apb_pready_all_o[apb_out_idx] = u_new.apb_rsp_o[apb_out_idx].pready;
    assign orig_apb_pslverr_all_o[apb_out_idx] = APB_ESR_rsp[apb_out_idx].apb_pslverr;
    assign new_apb_pslverr_all_o[apb_out_idx] = u_new.apb_rsp_o[apb_out_idx].pslverr;
    assign orig_apb_prdata_all_flat_o[apb_out_idx*ApbDataBits +: ApbDataBits] =
        APB_ESR_rsp[apb_out_idx].apb_prdata;
    assign new_apb_prdata_all_flat_o[apb_out_idx*ApbDataBits +: ApbDataBits] =
        u_new.apb_rsp_o[apb_out_idx].prdata;
  end

  assign orig_pwr_ctrl_glb_nsleepin_o = esr_pwr_ctrl_glb_nsleepin_op;
  assign new_pwr_ctrl_glb_nsleepin_o = u_new.pwr_ctrl_glb_nsleepin_o;
  assign orig_pwr_ctrl_glb_iso_o = esr_pwr_ctrl_glb_iso_op;
  assign new_pwr_ctrl_glb_iso_o = u_new.pwr_ctrl_glb_iso_o;
  assign orig_pwr_ctrl_neigh_nsleepin_o = esr_pwr_ctrl_neigh_nsleepin_op;
  assign new_pwr_ctrl_neigh_nsleepin_o = u_new.pwr_ctrl_neigh_nsleepin_o;
  assign orig_pwr_ctrl_neigh_iso_o = esr_pwr_ctrl_neigh_iso_op;
  assign new_pwr_ctrl_neigh_iso_o = u_new.pwr_ctrl_neigh_iso_o;

  assign orig_pll_ctrl_flat_o = esr_pll_ctrl_op;
  assign new_pll_ctrl_flat_o = u_new.esr_pll_ctrl_o;
  assign orig_pll_conf_o = esp_pll_conf_op;
  assign new_pll_conf_o = u_new.esr_pll_conf_o;
  assign orig_clk_dly_ctl_flat_o = esr_clk_dly_ctl_op;
  assign new_clk_dly_ctl_flat_o = esr_clk_dly_ctl_op;
  for (genvar dll_ctl_idx = 0; dll_ctl_idx < NUM_NEIGH; dll_ctl_idx++) begin : gen_dll_ctl_flat
    assign orig_dll_dly_est_ctl_flat_o[dll_ctl_idx*DllDlyEstCtlBits +: DllDlyEstCtlBits] =
        esr_dll_dly_est_ctl_op;
    assign new_dll_dly_est_ctl_flat_o[dll_ctl_idx*DllDlyEstCtlBits +: DllDlyEstCtlBits] =
        u_new.esr_dll_dly_est_ctl_o[dll_ctl_idx];
  end
  assign orig_shire_ctrl_clockmux_flat_o = esr_shire_ctrl_clockmux_op;
  assign new_shire_ctrl_clockmux_flat_o = u_new.esr_shire_ctrl_clockmux_o;
  assign orig_dll_ctrl_flat_o = esr_dll_ctrl_op;
  assign new_dll_ctrl_flat_o = u_new.esr_dll_ctrl_o;
  assign orig_dll_conf_o = esp_dll_conf_op;
  assign new_dll_conf_o = u_new.esr_dll_conf_o;

  ram_cfg_pkg::ram_cfg_t orig_ram_cfg;
  always_comb begin
    orig_ram_cfg = '0;
    orig_ram_cfg.test_en = u_orig.esr_shire_cache_ram_cfg.cfg2.sc_mbd_test1 |
                           u_orig.esr_shire_cache_ram_cfg.cfg2.sc_mbd_test_rnm;
    orig_ram_cfg.rm = u_orig.esr_shire_cache_ram_cfg.cfg2.sc_mbd_rm;
    orig_ram_cfg.rme = u_orig.esr_shire_cache_ram_cfg.cfg2.sc_mbd_rme;
    orig_ram_cfg.ra = u_orig.esr_shire_cache_ram_cfg.cfg2.sc_mbd_ra;
    orig_ram_cfg.wa = u_orig.esr_shire_cache_ram_cfg.cfg2.sc_mbd_wa[1:0];
    orig_ram_cfg.wpulse = u_orig.esr_shire_cache_ram_cfg.cfg2.sc_mbd_wpulse;
  end
  assign orig_ram_cfg_flat_o = orig_ram_cfg;

  for (genvar axi_m_idx = 0; axi_m_idx < NUM_L3_MASTER_PORTS; axi_m_idx++) begin : gen_l3_master_outputs
    assign orig_to_l3_axi_ar_flat_o[axi_m_idx*ScMasterArBits +: ScMasterArBits] =
        to_l3_mesh_master_axi_AR[axi_m_idx];
    assign new_to_l3_axi_ar_flat_o[axi_m_idx*ScMasterArBits +: ScMasterArBits] =
        u_new.to_l3_axi_ar_o[axi_m_idx];
    assign orig_to_l3_axi_ar_valid_o[axi_m_idx] = to_l3_mesh_master_axi_ARVALID[axi_m_idx];
    assign new_to_l3_axi_ar_valid_o[axi_m_idx] = u_new.to_l3_axi_ar_valid_o[axi_m_idx];
    assign orig_to_l3_axi_aw_flat_o[axi_m_idx*ScMasterAwBits +: ScMasterAwBits] =
        to_l3_mesh_master_axi_AW[axi_m_idx];
    assign new_to_l3_axi_aw_flat_o[axi_m_idx*ScMasterAwBits +: ScMasterAwBits] =
        u_new.to_l3_axi_aw_o[axi_m_idx];
    assign orig_to_l3_axi_aw_valid_o[axi_m_idx] = to_l3_mesh_master_axi_AWVALID[axi_m_idx];
    assign new_to_l3_axi_aw_valid_o[axi_m_idx] = u_new.to_l3_axi_aw_valid_o[axi_m_idx];
    assign orig_to_l3_axi_w_flat_o[axi_m_idx*ScMasterWBits +: ScMasterWBits] =
        to_l3_mesh_master_axi_W[axi_m_idx];
    assign new_to_l3_axi_w_flat_o[axi_m_idx*ScMasterWBits +: ScMasterWBits] =
        u_new.to_l3_axi_w_o[axi_m_idx];
    assign orig_to_l3_axi_w_valid_o[axi_m_idx] = to_l3_mesh_master_axi_WVALID[axi_m_idx];
    assign new_to_l3_axi_w_valid_o[axi_m_idx] = u_new.to_l3_axi_w_valid_o[axi_m_idx];
    assign orig_to_l3_axi_b_ready_o[axi_m_idx] = to_l3_mesh_master_axi_BREADY[axi_m_idx];
    assign new_to_l3_axi_b_ready_o[axi_m_idx] = u_new.to_l3_axi_b_ready_o[axi_m_idx];
    assign orig_to_l3_axi_r_ready_o[axi_m_idx] = to_l3_mesh_master_axi_RREADY[axi_m_idx];
    assign new_to_l3_axi_r_ready_o[axi_m_idx] = u_new.to_l3_axi_r_ready_o[axi_m_idx];
  end

  for (genvar axi_sys_idx = 0; axi_sys_idx < NUM_SYS_PORTS; axi_sys_idx++) begin : gen_sys_master_outputs
    if (axi_sys_idx == 0) begin : gen_orig_sys_master
      assign orig_to_sys_axi_ar_flat_o[axi_sys_idx*ScMasterArBits +: ScMasterArBits] =
          to_sys_mesh_master_axi_AR;
      assign orig_to_sys_axi_ar_valid_o[axi_sys_idx] = to_sys_mesh_master_axi_ARVALID;
      assign orig_to_sys_axi_aw_flat_o[axi_sys_idx*ScMasterAwBits +: ScMasterAwBits] =
          to_sys_mesh_master_axi_AW;
      assign orig_to_sys_axi_aw_valid_o[axi_sys_idx] = to_sys_mesh_master_axi_AWVALID;
      assign orig_to_sys_axi_w_flat_o[axi_sys_idx*ScMasterWBits +: ScMasterWBits] =
          to_sys_mesh_master_axi_W;
      assign orig_to_sys_axi_w_valid_o[axi_sys_idx] = to_sys_mesh_master_axi_WVALID;
      assign orig_to_sys_axi_b_ready_o[axi_sys_idx] = to_sys_mesh_master_axi_BREADY;
      assign orig_to_sys_axi_r_ready_o[axi_sys_idx] = to_sys_mesh_master_axi_RREADY;
    end else begin : gen_unused_orig_sys_master
      assign orig_to_sys_axi_ar_flat_o[axi_sys_idx*ScMasterArBits +: ScMasterArBits] = '0;
      assign orig_to_sys_axi_ar_valid_o[axi_sys_idx] = 1'b0;
      assign orig_to_sys_axi_aw_flat_o[axi_sys_idx*ScMasterAwBits +: ScMasterAwBits] = '0;
      assign orig_to_sys_axi_aw_valid_o[axi_sys_idx] = 1'b0;
      assign orig_to_sys_axi_w_flat_o[axi_sys_idx*ScMasterWBits +: ScMasterWBits] = '0;
      assign orig_to_sys_axi_w_valid_o[axi_sys_idx] = 1'b0;
      assign orig_to_sys_axi_b_ready_o[axi_sys_idx] = 1'b0;
      assign orig_to_sys_axi_r_ready_o[axi_sys_idx] = 1'b0;
    end
    assign new_to_sys_axi_ar_flat_o[axi_sys_idx*ScMasterArBits +: ScMasterArBits] =
        u_new.to_sys_axi_ar_o[axi_sys_idx];
    assign new_to_sys_axi_ar_valid_o[axi_sys_idx] = u_new.to_sys_axi_ar_valid_o[axi_sys_idx];
    assign new_to_sys_axi_aw_flat_o[axi_sys_idx*ScMasterAwBits +: ScMasterAwBits] =
        u_new.to_sys_axi_aw_o[axi_sys_idx];
    assign new_to_sys_axi_aw_valid_o[axi_sys_idx] = u_new.to_sys_axi_aw_valid_o[axi_sys_idx];
    assign new_to_sys_axi_w_flat_o[axi_sys_idx*ScMasterWBits +: ScMasterWBits] =
        u_new.to_sys_axi_w_o[axi_sys_idx];
    assign new_to_sys_axi_w_valid_o[axi_sys_idx] = u_new.to_sys_axi_w_valid_o[axi_sys_idx];
    assign new_to_sys_axi_b_ready_o[axi_sys_idx] = u_new.to_sys_axi_b_ready_o[axi_sys_idx];
    assign new_to_sys_axi_r_ready_o[axi_sys_idx] = u_new.to_sys_axi_r_ready_o[axi_sys_idx];
  end

  for (genvar axi_s_idx = 0; axi_s_idx < NUM_L3_SLAVE_PORTS; axi_s_idx++) begin : gen_l3_slave_outputs
    assign orig_l3_axi_ar_ready_o[axi_s_idx] = l3_mesh_slave_axi_ARREADY[axi_s_idx];
    assign new_l3_axi_ar_ready_o[axi_s_idx] = u_new.l3_axi_ar_ready_o[axi_s_idx];
    assign orig_l3_axi_aw_ready_o[axi_s_idx] = l3_mesh_slave_axi_AWREADY[axi_s_idx];
    assign new_l3_axi_aw_ready_o[axi_s_idx] = u_new.l3_axi_aw_ready_o[axi_s_idx];
    assign orig_l3_axi_w_ready_o[axi_s_idx] = l3_mesh_slave_axi_WREADY[axi_s_idx];
    assign new_l3_axi_w_ready_o[axi_s_idx] = u_new.l3_axi_w_ready_o[axi_s_idx];
    assign orig_l3_axi_b_flat_o[axi_s_idx*ScSlaveBBits +: ScSlaveBBits] =
        l3_mesh_slave_axi_B[axi_s_idx];
    assign new_l3_axi_b_flat_o[axi_s_idx*ScSlaveBBits +: ScSlaveBBits] =
        u_new.l3_axi_b_o[axi_s_idx];
    assign orig_l3_axi_b_valid_o[axi_s_idx] = l3_mesh_slave_axi_BVALID[axi_s_idx];
    assign new_l3_axi_b_valid_o[axi_s_idx] = u_new.l3_axi_b_valid_o[axi_s_idx];
    assign orig_l3_axi_r_flat_o[axi_s_idx*ScSlaveRBits +: ScSlaveRBits] =
        l3_mesh_slave_axi_R[axi_s_idx];
    assign new_l3_axi_r_flat_o[axi_s_idx*ScSlaveRBits +: ScSlaveRBits] =
        u_new.l3_axi_r_o[axi_s_idx];
    assign orig_l3_axi_r_valid_o[axi_s_idx] = l3_mesh_slave_axi_RVALID[axi_s_idx];
    assign new_l3_axi_r_valid_o[axi_s_idx] = u_new.l3_axi_r_valid_o[axi_s_idx];
  end

  assign orig_uc_to_l3_axi_ar_flat_o = uc_to_l3_mesh_master_axi_AR;
  assign new_uc_to_l3_axi_ar_flat_o = u_new.uc_to_l3_axi_ar_o;
  assign orig_uc_to_l3_axi_ar_valid_o = uc_to_l3_mesh_master_axi_ARVALID;
  assign new_uc_to_l3_axi_ar_valid_o = u_new.uc_to_l3_axi_ar_valid_o;
  assign orig_uc_to_l3_axi_aw_flat_o = uc_to_l3_mesh_master_axi_AW;
  assign new_uc_to_l3_axi_aw_flat_o = u_new.uc_to_l3_axi_aw_o;
  assign orig_uc_to_l3_axi_aw_valid_o = uc_to_l3_mesh_master_axi_AWVALID;
  assign new_uc_to_l3_axi_aw_valid_o = u_new.uc_to_l3_axi_aw_valid_o;
  assign orig_uc_to_l3_axi_w_flat_o = uc_to_l3_mesh_master_axi_W;
  assign new_uc_to_l3_axi_w_flat_o = u_new.uc_to_l3_axi_w_o;
  assign orig_uc_to_l3_axi_w_valid_o = uc_to_l3_mesh_master_axi_WVALID;
  assign new_uc_to_l3_axi_w_valid_o = u_new.uc_to_l3_axi_w_valid_o;
  assign orig_uc_to_l3_axi_b_ready_o = uc_to_l3_mesh_master_axi_BREADY;
  assign new_uc_to_l3_axi_b_ready_o = u_new.uc_to_l3_axi_b_ready_o;
  assign orig_uc_to_l3_axi_r_ready_o = uc_to_l3_mesh_master_axi_RREADY;
  assign new_uc_to_l3_axi_r_ready_o = u_new.uc_to_l3_axi_r_ready_o;

  assign orig_uc_to_sys_axi_ar_flat_o = uc_to_sys_mesh_master_axi_AR;
  assign new_uc_to_sys_axi_ar_flat_o = u_new.uc_to_sys_axi_ar_o;
  assign orig_uc_to_sys_axi_ar_valid_o = uc_to_sys_mesh_master_axi_ARVALID;
  assign new_uc_to_sys_axi_ar_valid_o = u_new.uc_to_sys_axi_ar_valid_o;
  assign orig_uc_to_sys_axi_aw_flat_o = uc_to_sys_mesh_master_axi_AW;
  assign new_uc_to_sys_axi_aw_flat_o = u_new.uc_to_sys_axi_aw_o;
  assign orig_uc_to_sys_axi_aw_valid_o = uc_to_sys_mesh_master_axi_AWVALID;
  assign new_uc_to_sys_axi_aw_valid_o = u_new.uc_to_sys_axi_aw_valid_o;
  assign orig_uc_to_sys_axi_w_flat_o = uc_to_sys_mesh_master_axi_W;
  assign new_uc_to_sys_axi_w_flat_o = u_new.uc_to_sys_axi_w_o;
  assign orig_uc_to_sys_axi_w_valid_o = uc_to_sys_mesh_master_axi_WVALID;
  assign new_uc_to_sys_axi_w_valid_o = u_new.uc_to_sys_axi_w_valid_o;
  assign orig_uc_to_sys_axi_b_ready_o = uc_to_sys_mesh_master_axi_BREADY;
  assign new_uc_to_sys_axi_b_ready_o = u_new.uc_to_sys_axi_b_ready_o;
  assign orig_uc_to_sys_axi_r_ready_o = uc_to_sys_mesh_master_axi_RREADY;
  assign new_uc_to_sys_axi_r_ready_o = u_new.uc_to_sys_axi_r_ready_o;

  assign orig_sys_axi_ar_ready_o = sys_mesh_slave_axi_ARREADY;
  assign new_sys_axi_ar_ready_o = u_new.sys_axi_ar_ready_o;
  assign orig_sys_axi_aw_ready_o = sys_mesh_slave_axi_AWREADY;
  assign new_sys_axi_aw_ready_o = u_new.sys_axi_aw_ready_o;
  assign orig_sys_axi_w_ready_o = sys_mesh_slave_axi_WREADY;
  assign new_sys_axi_w_ready_o = u_new.sys_axi_w_ready_o;
  assign orig_sys_axi_b_flat_o = sys_mesh_slave_axi_B;
  assign new_sys_axi_b_flat_o = u_new.sys_axi_b_o;
  assign orig_sys_axi_b_valid_o = sys_mesh_slave_axi_BVALID;
  assign new_sys_axi_b_valid_o = u_new.sys_axi_b_valid_o;
  assign orig_sys_axi_r_flat_o = sys_mesh_slave_axi_R;
  assign new_sys_axi_r_flat_o = u_new.sys_axi_r_o;
  assign orig_sys_axi_r_valid_o = sys_mesh_slave_axi_RVALID;
  assign new_sys_axi_r_valid_o = u_new.sys_axi_r_valid_o;
  assign orig_sys_axi_aw_credit_o = sys_mesh_slave_axi_AWcredit;
  assign new_sys_axi_aw_credit_o = u_new.sys_axi_aw_credit_o;

  assign orig_sbm_sys_axi_ar_flat_o = sbm_sys_mesh_slave_axi_AR;
  assign new_sbm_sys_axi_ar_flat_o = u_new.sbm_sys_axi_ar_o;
  assign orig_sbm_sys_axi_aw_flat_o = sbm_sys_mesh_slave_axi_AW;
  assign new_sbm_sys_axi_aw_flat_o = u_new.sbm_sys_axi_aw_o;
  assign orig_sbm_sys_axi_w_flat_o = sbm_sys_mesh_slave_axi_W;
  assign new_sbm_sys_axi_w_flat_o = u_new.sbm_sys_axi_w_o;
  assign orig_sbm_sys_axi_b_ready_o = sbm_sys_mesh_slave_axi_BREADY;
  assign new_sbm_sys_axi_b_ready_o = u_new.sbm_sys_axi_b_ready_o;
  assign orig_sbm_sys_axi_r_ready_o = sbm_sys_mesh_slave_axi_RREADY;
  assign new_sbm_sys_axi_r_ready_o = u_new.sbm_sys_axi_r_ready_o;

  for (genvar l2hpf_idx = 0; l2hpf_idx < NUM_BANKS; l2hpf_idx++) begin : gen_l2hpf_outputs
    assign orig_l2hpf_req_info_flat_o[l2hpf_idx*L2hpfReqBits +: L2hpfReqBits] =
        sc_neigh_l2hpf_req_info[l2hpf_idx];
    assign new_l2hpf_req_info_flat_o[l2hpf_idx*L2hpfReqBits +: L2hpfReqBits] =
        u_new.sc_neigh_l2hpf_req_info_o[l2hpf_idx];
  end
  assign orig_sc_trace_data_flat_o = sc_trace_data;
  assign new_sc_trace_data_flat_o = u_new.sc_trace_data_o;

endmodule
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNUSEDSIGNAL */
