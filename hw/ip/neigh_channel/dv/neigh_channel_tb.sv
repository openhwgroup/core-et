// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // Smoke wrapper declares and ties every DUT port; most observed outputs are intentionally unused by the test.
module neigh_channel_tb (
  input  logic clk_i,
  input  logic rst_ni,
  output logic clk_minion_o,
  output logic clk_tbox_o,
  output logic rst_w_icache_no,
  output logic any_neigh_sc_req_valid_o,
  output logic any_l2_dcache_resp_valid_o,
  output logic tbox_neigh_rsp_valid_o
);

  import dft_pkg::*;
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import icache_pkg::*;
  import neigh_pkg::*;

  logic dut_clk_shire_i;
  assign dut_clk_shire_i = clk_i;
  logic dut_clk_minion_o;
  logic dut_rst_c_shire_ni;
  assign dut_rst_c_shire_ni = rst_ni;
  logic dut_rst_d_shire_ni;
  assign dut_rst_d_shire_ni = rst_ni;
  logic dut_rst_w_shire_ni;
  assign dut_rst_w_shire_ni = rst_ni;
  logic dut_rst_warm_ni;
  assign dut_rst_warm_ni = rst_ni;
  logic dut_rst_w_icache_no;
  dft_t dut_dft_hv_i;
  assign dut_dft_hv_i = '0;
  logic dut_dft_use_reset_cntl_hv_i;
  assign dut_dft_use_reset_cntl_hv_i = '0;
  neigh_pkg::shire_tdr_reset_cntl_t dut_dft_reset_cntl_hv_i;
  assign dut_dft_reset_cntl_hv_i = '0;
  dft_t dut_dft_lv_o;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t dut_chicken_bits_vc_i;
  assign dut_chicken_bits_vc_i = '0;
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t dut_bpam_run_control_i;
  assign dut_bpam_run_control_i = '0;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t dut_bpam_rc_tbox_ack_hi_o;
  logic dut_dll_feedback_shire_o;
  logic dut_dll_feedback_neigh_o;
  logic [minion_pkg::NumShireIdsBits-1:0] dut_shire_id_i;
  assign dut_shire_id_i = '0;
  logic [neigh_pkg::NumNeighWidth-1:0] dut_neigh_id_i;
  assign dut_neigh_id_i = '0;
  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] dut_shire_tbox_id_i;
  assign dut_shire_tbox_id_i = '0;
  logic dut_shire_tbox_en_i;
  assign dut_shire_tbox_en_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_esr_thread0_enable_i;
  assign dut_esr_thread0_enable_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_esr_thread1_enable_i;
  assign dut_esr_thread1_enable_i = '0;
  minion_pkg::esr_minion_features_t dut_esr_minion_features_i;
  assign dut_esr_minion_features_i = '0;
  icache_pkg::icache_prefetch_conf_t dut_esr_icache_prefetch_conf_i;
  assign dut_esr_icache_prefetch_conf_i = '0;
  logic dut_esr_icache_prefetch_start_i;
  assign dut_esr_icache_prefetch_start_i = '0;
  logic dut_esr_icache_prefetch_done_o;
  logic dut_esr_icache_err_detected_o;
  logic dut_esr_icache_err_logged_o;
  esr_pkg::esr_ms_dmctrl_t dut_dmctrl_i;
  assign dut_dmctrl_i = '0;
  esr_pkg::esr_and_or_tree_l0_t dut_esr_and_or_tree_l0_o;
  logic dut_esr_shire_coop_mode_i;
  assign dut_esr_shire_coop_mode_i = '0;
  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] dut_esr_minion_mem_override_i;
  assign dut_esr_minion_mem_override_i = '0;
  esr_pkg::esr_dll_dly_est_ctl_t dut_esr_dll_dly_est_ctl_i;
  assign dut_esr_dll_dly_est_ctl_i = '0;
  esr_pkg::esr_dll_dly_est_sts_t dut_esr_dll_dly_est_sts_o;
  logic [shirecache_pkg::Banks:0] dut_neigh_sc_req_ready_i;
  assign dut_neigh_sc_req_ready_i = '1;
  logic [shirecache_pkg::Banks:0] dut_neigh_sc_req_valid_o;
  etlink_pkg::req_t dut_neigh_sc_req_info_o;
  logic dut_neigh_sc_rsp_ready_o;
  logic dut_neigh_sc_rsp_valid_i;
  assign dut_neigh_sc_rsp_valid_i = '0;
  etlink_pkg::rsp_t dut_neigh_sc_rsp_info_i;
  assign dut_neigh_sc_rsp_info_i = '0;
  logic dut_hw_dbg_sm_monitor_enabled_i;
  assign dut_hw_dbg_sm_monitor_enabled_i = '0;
  logic [neigh_voltage_cross_pkg::NeighSmGpioWidth-1:0] dut_neigh_sm_gpio_i;
  assign dut_neigh_sm_gpio_i = '0;
  minion_pkg::neigh_sm_dbg_monitor_t dut_neigh_sm_signals_o;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t dut_apb_req_from_shire_i;
  assign dut_apb_req_from_shire_i = '0;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t dut_apb_rsp_to_shire_o;
  logic [neigh_pkg::MinPerN-1:0] dut_int_mtip_i;
  assign dut_int_mtip_i = '0;
  logic dut_int_meip_i;
  assign dut_int_meip_i = '0;
  logic dut_int_seip_i;
  assign dut_int_seip_i = '0;
  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] dut_ipi_msip_i;
  assign dut_ipi_msip_i = '0;
  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] dut_ipi_redirect_trigger_i;
  assign dut_ipi_redirect_trigger_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_uc_to_neigh_fcc_i;
  assign dut_uc_to_neigh_fcc_i = '0;
  logic [neigh_voltage_cross_pkg::UcFccTargetSize-1:0] dut_uc_to_neigh_fcc_target_i;
  assign dut_uc_to_neigh_fcc_target_i = '0;
  logic dut_flb_neigh_l2_req_valid_o;
  logic [neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] dut_flb_neigh_l2_req_data_o;
  logic dut_flb_l2_neigh_resp_valid_i;
  assign dut_flb_l2_neigh_resp_valid_i = '0;
  logic dut_flb_l2_neigh_resp_data_i;
  assign dut_flb_l2_neigh_resp_data_i = '0;
  logic dut_icache_f2_sram_req_write_o;
  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] dut_icache_f2_sram_req_addr_o;
  logic dut_icache_f2_sram_req_valid_o;
  logic dut_icache_f2_sram_req_ready_i;
  assign dut_icache_f2_sram_req_ready_i = '1;
  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] dut_icache_f0_sram_resp_dout_i;
  assign dut_icache_f0_sram_resp_dout_i = '0;
  logic dut_icache_f0_sram_resp_valid_i;
  assign dut_icache_f0_sram_resp_valid_i = '0;
  logic dut_icache_f0_sram_resp_ready_o;
  neigh_pkg::coop_tload_slv_rdy_req_t dut_coop_tload_slv_rdy_out_data_o;
  logic dut_coop_tload_slv_rdy_out_valid_o;
  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] dut_coop_tload_slv_rdy_in_data_i;
  assign dut_coop_tload_slv_rdy_in_data_i = '0;
  logic [neigh_pkg::NumOtherNeigh-1:0] dut_coop_tload_slv_rdy_in_valid_i;
  assign dut_coop_tload_slv_rdy_in_valid_i = '0;
  logic [neigh_pkg::ShireCoopIdSize-1:0] dut_coop_tload_mst_done_out_coop_id_o;
  logic [neigh_pkg::NumOtherNeigh-1:0] dut_coop_tload_mst_done_out_valid_o;
  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] dut_coop_tload_mst_done_in_coop_id_i;
  assign dut_coop_tload_mst_done_in_coop_id_i = '0;
  logic [neigh_pkg::NumOtherNeigh-1:0] dut_coop_tload_mst_done_in_valid_i;
  assign dut_coop_tload_mst_done_in_valid_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_rst_c_per_minion_no;
  logic [neigh_pkg::MinPerN-1:0] dut_rst_d_per_minion_no;
  logic [neigh_pkg::MinPerN-1:0] dut_rst_w_per_minion_no;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t dut_chicken_bits_vc_lo_o;
  logic [minion_pkg::NumShireIdsBits-1:0] dut_shire_id_lo_o;
  logic [neigh_pkg::NumNeighWidth-1:0] dut_neigh_id_lo_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::MinPerSBits-1:0] dut_shire_min_id_o;
  logic [neigh_pkg::MinPerN-1:0] dut_thread0_enable_o;
  logic [neigh_pkg::MinPerN-1:0] dut_thread1_enable_o;
  minion_pkg::esr_minion_features_t dut_esr_minion_features_lo_o;
  logic dut_esr_shire_coop_mode_lo_o;
  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] dut_esr_minion_mem_override_lo_o;
  logic dut_esr_bypass_dcache_o;
  logic [minion_pkg::VaSize-1:0] dut_minion_boot_addr_o;
  esr_pkg::esr_mprot_t dut_mprot_o;
  minion_pkg::tlb_entry_type dut_vmspagesize_o;
  minion_pkg::minion_interrupts [neigh_pkg::MinPerN-1:0] dut_interrupts_per_minion_o;
  logic [neigh_pkg::MinPerN-1:0] dut_flb_core_neigh_req_valid_i;
  assign dut_flb_core_neigh_req_valid_i = '0;
  logic [neigh_pkg::MinPerN-1:0][neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] dut_flb_core_neigh_req_data_i;
  assign dut_flb_core_neigh_req_data_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_flb_neigh_core_resp_valid_o;
  logic dut_flb_neigh_core_resp_data_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] dut_l2_dcache_evict_req_ready_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] dut_l2_dcache_evict_req_valid_i;
  assign dut_l2_dcache_evict_req_valid_i = '0;
  minion_dcache_pkg::et_link_minion_evict_req_info_t [neigh_pkg::MinPerN-1:0] dut_l2_dcache_evict_req_i;
  assign dut_l2_dcache_evict_req_i = '0;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] dut_l2_dcache_miss_req_ready_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] dut_l2_dcache_miss_req_valid_i;
  assign dut_l2_dcache_miss_req_valid_i = '0;
  minion_dcache_pkg::et_link_minion_miss_req_info_t [neigh_pkg::MinPerN-1:0] dut_l2_dcache_miss_req_i;
  assign dut_l2_dcache_miss_req_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_l2_dcache_resp_ready_i;
  assign dut_l2_dcache_resp_ready_i = '1;
  logic [neigh_pkg::MinPerN-1:0] dut_l2_dcache_resp_valid_o;
  minion_pkg::et_link_minion_rsp_info_t [neigh_pkg::MinPerN-1:0] dut_l2_dcache_resp_o;
  logic [neigh_pkg::MinPerN-1:0] dut_icache_req_ready_o;
  logic [neigh_pkg::MinPerN-1:0] dut_icache_req_valid_i;
  assign dut_icache_req_valid_i = '0;
  icache_pkg::frontend_icache_req_t [neigh_pkg::MinPerN-1:0] dut_icache_req_i;
  assign dut_icache_req_i = '0;
  logic [icache_pkg::IcacheNrReqs-1:0] dut_icache_resp_valid_o;
  logic [icache_pkg::IcacheNrReqs-1:0] dut_icache_resp_miss_o;
  icache_pkg::icache_frontend_resp_t [icache_pkg::IcacheNrReqs-1:0] dut_icache_resp_o;
  logic [icache_pkg::IcacheNrReqs-1:0] dut_icache_fill_done_o;
  logic [neigh_pkg::MinPerN-1:0] dut_icache_flush_data_i;
  assign dut_icache_flush_data_i = '0;
  minion_pkg::minion_satp_info [neigh_pkg::MinPerN-1:0] dut_satp_info_i;
  assign dut_satp_info_i = '0;
  minion_pkg::minion_satp_info [neigh_pkg::MinPerN-1:0] dut_matp_info_i;
  assign dut_matp_info_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_tlb_invalidate_i;
  assign dut_tlb_invalidate_i = '0;
  minion_pkg::minion_ptw_req [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dut_dc_ptw_req_data_i;
  assign dut_dc_ptw_req_data_i = '0;
  logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dut_dc_ptw_req_valid_i;
  assign dut_dc_ptw_req_valid_i = '0;
  logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dut_dc_ptw_req_ready_o;
  minion_pkg::minion_ptw_pte [neigh_pkg::PtwPerN-1:0] dut_ptw_dc_resp_data_o;
  logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dut_ptw_dc_resp_valid_o;
  logic [neigh_pkg::MinPerN-1:0][esr_pkg::EsrApbDataWidth-1:0] dut_minion_apb_prdata_i;
  assign dut_minion_apb_prdata_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_minion_apb_pready_i;
  assign dut_minion_apb_pready_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_minion_apb_pslverr_i;
  assign dut_minion_apb_pslverr_i = '0;
  logic dut_minion_apb_penable_o;
  logic [neigh_pkg::MinPerN-1:0] dut_minion_apb_psel_o;
  logic dut_minion_apb_pwrite_o;
  logic [minion_pkg::MinDmApbAddrWidth-1:0] dut_minion_apb_paddr_o;
  logic [esr_pkg::EsrApbDataWidth-1:0] dut_minion_apb_pwdata_o;
  minion_pkg::minion_debug_in_t [neigh_pkg::MinPerN-1:0] dut_debug_minion_in_o;
  minion_pkg::minion_debug_out_t [neigh_pkg::MinPerN-1:0] dut_debug_minion_out_i;
  assign dut_debug_minion_out_i = '0;
  logic [neigh_pkg::MinPerN-1:0] dut_minion_dbg_signals_en_o;
  minion_pkg::neigh_sm_dbg_monitor_t [neigh_pkg::MinPerN-1:0] dut_minion_dbg_signals_i;
  assign dut_minion_dbg_signals_i = '0;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::NeighDebugSmMuxWidth-1:0] dut_minion_dbg_signals_mux_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuMinionCountersNum-1:0] dut_pmu_count_up_i;
  assign dut_pmu_count_up_i = '0;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::XregSize-1:0] dut_pmu_read_data_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::PmuCountersSelectBits-1:0] dut_pmu_read_sel_i;
  assign dut_pmu_read_sel_i = '0;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuTotalCountersNum-1:0] dut_pmu_write_en_i;
  assign dut_pmu_write_en_i = '0;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::XregSize-1:0] dut_pmu_write_data_i;
  assign dut_pmu_write_data_i = '0;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuNeighEventCntSelBits-1:0] dut_pmu_neigh_event_sel_i;
  assign dut_pmu_neigh_event_sel_i = '0;
  logic dut_clk_tbox_o;
  logic dut_tbox_rst_c_no;
  logic dut_tbox_rst_d_no;
  logic dut_tbox_rst_w_no;
  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] dut_tbox_id_lo_o;
  logic [15:0] dut_tbox_control_esr_o;
  logic [15:0] dut_tbox_status_esr_i;
  assign dut_tbox_status_esr_i = '0;
  logic [minion_pkg::VaSize-1:0] dut_tbox_image_table_ptr_esr_o;
  logic dut_tbox_neigh_req_ready_o;
  logic dut_tbox_neigh_req_valid_i;
  assign dut_tbox_neigh_req_valid_i = '0;
  etlink_pkg::neigh_req_t dut_tbox_neigh_req_info_i;
  assign dut_tbox_neigh_req_info_i = '0;
  logic dut_tbox_neigh_rsp_ready_i;
  assign dut_tbox_neigh_rsp_ready_i = '1;
  logic dut_tbox_neigh_rsp_valid_o;
  etlink_pkg::rsp_t dut_tbox_neigh_rsp_info_o;
  logic dut_tbox_dbg_signals_en_o;
  minion_pkg::neigh_sm_dbg_monitor_t dut_tbox_dbg_signals_i;
  assign dut_tbox_dbg_signals_i = '0;
  logic [minion_pkg::NeighDebugSmMuxWidth-1:0] dut_tbox_dbg_signals_mux_o;
  neigh_ch_apb_mux_pkg::apb_to_neigh_tbox_t dut_tbox_apb_req_o;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t dut_tbox_apb_rsp_i;
  assign dut_tbox_apb_rsp_i = '0;
  neigh_ch_dbg_pkg::tbox_bpam_run_control_t dut_tbox_bpam_run_control_o;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t dut_tbox_bpam_run_control_ack_i;
  assign dut_tbox_bpam_run_control_ack_i = '0;

  assign clk_minion_o = dut_clk_minion_o;
  assign clk_tbox_o = dut_clk_tbox_o;
  assign rst_w_icache_no = dut_rst_w_icache_no;
  assign any_neigh_sc_req_valid_o = |dut_neigh_sc_req_valid_o;
  assign any_l2_dcache_resp_valid_o = |dut_l2_dcache_resp_valid_o;
  assign tbox_neigh_rsp_valid_o = dut_tbox_neigh_rsp_valid_o;

  neigh_channel dut (
    .clk_i(clk_i),
    .clk_shire_i(dut_clk_shire_i),
    .clk_minion_o(dut_clk_minion_o),
    .rst_c_shire_ni(dut_rst_c_shire_ni),
    .rst_d_shire_ni(dut_rst_d_shire_ni),
    .rst_w_shire_ni(dut_rst_w_shire_ni),
    .rst_warm_ni(dut_rst_warm_ni),
    .rst_w_icache_no(dut_rst_w_icache_no),
    .dft_hv_i(dut_dft_hv_i),
    .dft_use_reset_cntl_hv_i(dut_dft_use_reset_cntl_hv_i),
    .dft_reset_cntl_hv_i(dut_dft_reset_cntl_hv_i),
    .dft_lv_o(dut_dft_lv_o),
    .chicken_bits_vc_i(dut_chicken_bits_vc_i),
    .bpam_run_control_i(dut_bpam_run_control_i),
    .bpam_rc_tbox_ack_hi_o(dut_bpam_rc_tbox_ack_hi_o),
    .dll_feedback_shire_o(dut_dll_feedback_shire_o),
    .dll_feedback_neigh_o(dut_dll_feedback_neigh_o),
    .shire_id_i(dut_shire_id_i),
    .neigh_id_i(dut_neigh_id_i),
    .shire_tbox_id_i(dut_shire_tbox_id_i),
    .shire_tbox_en_i(dut_shire_tbox_en_i),
    .esr_thread0_enable_i(dut_esr_thread0_enable_i),
    .esr_thread1_enable_i(dut_esr_thread1_enable_i),
    .esr_minion_features_i(dut_esr_minion_features_i),
    .esr_icache_prefetch_conf_i(dut_esr_icache_prefetch_conf_i),
    .esr_icache_prefetch_start_i(dut_esr_icache_prefetch_start_i),
    .esr_icache_prefetch_done_o(dut_esr_icache_prefetch_done_o),
    .esr_icache_err_detected_o(dut_esr_icache_err_detected_o),
    .esr_icache_err_logged_o(dut_esr_icache_err_logged_o),
    .dmctrl_i(dut_dmctrl_i),
    .esr_and_or_tree_l0_o(dut_esr_and_or_tree_l0_o),
    .esr_shire_coop_mode_i(dut_esr_shire_coop_mode_i),
    .esr_minion_mem_override_i(dut_esr_minion_mem_override_i),
    .esr_dll_dly_est_ctl_i(dut_esr_dll_dly_est_ctl_i),
    .esr_dll_dly_est_sts_o(dut_esr_dll_dly_est_sts_o),
    .neigh_sc_req_ready_i(dut_neigh_sc_req_ready_i),
    .neigh_sc_req_valid_o(dut_neigh_sc_req_valid_o),
    .neigh_sc_req_info_o(dut_neigh_sc_req_info_o),
    .neigh_sc_rsp_ready_o(dut_neigh_sc_rsp_ready_o),
    .neigh_sc_rsp_valid_i(dut_neigh_sc_rsp_valid_i),
    .neigh_sc_rsp_info_i(dut_neigh_sc_rsp_info_i),
    .hw_dbg_sm_monitor_enabled_i(dut_hw_dbg_sm_monitor_enabled_i),
    .neigh_sm_gpio_i(dut_neigh_sm_gpio_i),
    .neigh_sm_signals_o(dut_neigh_sm_signals_o),
    .apb_req_from_shire_i(dut_apb_req_from_shire_i),
    .apb_rsp_to_shire_o(dut_apb_rsp_to_shire_o),
    .int_mtip_i(dut_int_mtip_i),
    .int_meip_i(dut_int_meip_i),
    .int_seip_i(dut_int_seip_i),
    .ipi_msip_i(dut_ipi_msip_i),
    .ipi_redirect_trigger_i(dut_ipi_redirect_trigger_i),
    .uc_to_neigh_fcc_i(dut_uc_to_neigh_fcc_i),
    .uc_to_neigh_fcc_target_i(dut_uc_to_neigh_fcc_target_i),
    .flb_neigh_l2_req_valid_o(dut_flb_neigh_l2_req_valid_o),
    .flb_neigh_l2_req_data_o(dut_flb_neigh_l2_req_data_o),
    .flb_l2_neigh_resp_valid_i(dut_flb_l2_neigh_resp_valid_i),
    .flb_l2_neigh_resp_data_i(dut_flb_l2_neigh_resp_data_i),
    .icache_f2_sram_req_write_o(dut_icache_f2_sram_req_write_o),
    .icache_f2_sram_req_addr_o(dut_icache_f2_sram_req_addr_o),
    .icache_f2_sram_req_valid_o(dut_icache_f2_sram_req_valid_o),
    .icache_f2_sram_req_ready_i(dut_icache_f2_sram_req_ready_i),
    .icache_f0_sram_resp_dout_i(dut_icache_f0_sram_resp_dout_i),
    .icache_f0_sram_resp_valid_i(dut_icache_f0_sram_resp_valid_i),
    .icache_f0_sram_resp_ready_o(dut_icache_f0_sram_resp_ready_o),
    .coop_tload_slv_rdy_out_data_o(dut_coop_tload_slv_rdy_out_data_o),
    .coop_tload_slv_rdy_out_valid_o(dut_coop_tload_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data_i(dut_coop_tload_slv_rdy_in_data_i),
    .coop_tload_slv_rdy_in_valid_i(dut_coop_tload_slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id_o(dut_coop_tload_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid_o(dut_coop_tload_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id_i(dut_coop_tload_mst_done_in_coop_id_i),
    .coop_tload_mst_done_in_valid_i(dut_coop_tload_mst_done_in_valid_i),
    .rst_c_per_minion_no(dut_rst_c_per_minion_no),
    .rst_d_per_minion_no(dut_rst_d_per_minion_no),
    .rst_w_per_minion_no(dut_rst_w_per_minion_no),
    .chicken_bits_vc_lo_o(dut_chicken_bits_vc_lo_o),
    .shire_id_lo_o(dut_shire_id_lo_o),
    .neigh_id_lo_o(dut_neigh_id_lo_o),
    .shire_min_id_o(dut_shire_min_id_o),
    .thread0_enable_o(dut_thread0_enable_o),
    .thread1_enable_o(dut_thread1_enable_o),
    .esr_minion_features_lo_o(dut_esr_minion_features_lo_o),
    .esr_shire_coop_mode_lo_o(dut_esr_shire_coop_mode_lo_o),
    .esr_minion_mem_override_lo_o(dut_esr_minion_mem_override_lo_o),
    .esr_bypass_dcache_o(dut_esr_bypass_dcache_o),
    .minion_boot_addr_o(dut_minion_boot_addr_o),
    .mprot_o(dut_mprot_o),
    .vmspagesize_o(dut_vmspagesize_o),
    .interrupts_per_minion_o(dut_interrupts_per_minion_o),
    .flb_core_neigh_req_valid_i(dut_flb_core_neigh_req_valid_i),
    .flb_core_neigh_req_data_i(dut_flb_core_neigh_req_data_i),
    .flb_neigh_core_resp_valid_o(dut_flb_neigh_core_resp_valid_o),
    .flb_neigh_core_resp_data_o(dut_flb_neigh_core_resp_data_o),
    .l2_dcache_evict_req_ready_o(dut_l2_dcache_evict_req_ready_o),
    .l2_dcache_evict_req_valid_i(dut_l2_dcache_evict_req_valid_i),
    .l2_dcache_evict_req_i(dut_l2_dcache_evict_req_i),
    .l2_dcache_miss_req_ready_o(dut_l2_dcache_miss_req_ready_o),
    .l2_dcache_miss_req_valid_i(dut_l2_dcache_miss_req_valid_i),
    .l2_dcache_miss_req_i(dut_l2_dcache_miss_req_i),
    .l2_dcache_resp_ready_i(dut_l2_dcache_resp_ready_i),
    .l2_dcache_resp_valid_o(dut_l2_dcache_resp_valid_o),
    .l2_dcache_resp_o(dut_l2_dcache_resp_o),
    .icache_req_ready_o(dut_icache_req_ready_o),
    .icache_req_valid_i(dut_icache_req_valid_i),
    .icache_req_i(dut_icache_req_i),
    .icache_resp_valid_o(dut_icache_resp_valid_o),
    .icache_resp_miss_o(dut_icache_resp_miss_o),
    .icache_resp_o(dut_icache_resp_o),
    .icache_fill_done_o(dut_icache_fill_done_o),
    .icache_flush_data_i(dut_icache_flush_data_i),
    .satp_info_i(dut_satp_info_i),
    .matp_info_i(dut_matp_info_i),
    .tlb_invalidate_i(dut_tlb_invalidate_i),
    .dc_ptw_req_data_i(dut_dc_ptw_req_data_i),
    .dc_ptw_req_valid_i(dut_dc_ptw_req_valid_i),
    .dc_ptw_req_ready_o(dut_dc_ptw_req_ready_o),
    .ptw_dc_resp_data_o(dut_ptw_dc_resp_data_o),
    .ptw_dc_resp_valid_o(dut_ptw_dc_resp_valid_o),
    .minion_apb_prdata_i(dut_minion_apb_prdata_i),
    .minion_apb_pready_i(dut_minion_apb_pready_i),
    .minion_apb_pslverr_i(dut_minion_apb_pslverr_i),
    .minion_apb_penable_o(dut_minion_apb_penable_o),
    .minion_apb_psel_o(dut_minion_apb_psel_o),
    .minion_apb_pwrite_o(dut_minion_apb_pwrite_o),
    .minion_apb_paddr_o(dut_minion_apb_paddr_o),
    .minion_apb_pwdata_o(dut_minion_apb_pwdata_o),
    .debug_minion_in_o(dut_debug_minion_in_o),
    .debug_minion_out_i(dut_debug_minion_out_i),
    .minion_dbg_signals_en_o(dut_minion_dbg_signals_en_o),
    .minion_dbg_signals_i(dut_minion_dbg_signals_i),
    .minion_dbg_signals_mux_o(dut_minion_dbg_signals_mux_o),
    .pmu_count_up_i(dut_pmu_count_up_i),
    .pmu_read_data_o(dut_pmu_read_data_o),
    .pmu_read_sel_i(dut_pmu_read_sel_i),
    .pmu_write_en_i(dut_pmu_write_en_i),
    .pmu_write_data_i(dut_pmu_write_data_i),
    .pmu_neigh_event_sel_i(dut_pmu_neigh_event_sel_i),
    .clk_tbox_o(dut_clk_tbox_o),
    .tbox_rst_c_no(dut_tbox_rst_c_no),
    .tbox_rst_d_no(dut_tbox_rst_d_no),
    .tbox_rst_w_no(dut_tbox_rst_w_no),
    .tbox_id_lo_o(dut_tbox_id_lo_o),
    .tbox_control_esr_o(dut_tbox_control_esr_o),
    .tbox_status_esr_i(dut_tbox_status_esr_i),
    .tbox_image_table_ptr_esr_o(dut_tbox_image_table_ptr_esr_o),
    .tbox_neigh_req_ready_o(dut_tbox_neigh_req_ready_o),
    .tbox_neigh_req_valid_i(dut_tbox_neigh_req_valid_i),
    .tbox_neigh_req_info_i(dut_tbox_neigh_req_info_i),
    .tbox_neigh_rsp_ready_i(dut_tbox_neigh_rsp_ready_i),
    .tbox_neigh_rsp_valid_o(dut_tbox_neigh_rsp_valid_o),
    .tbox_neigh_rsp_info_o(dut_tbox_neigh_rsp_info_o),
    .tbox_dbg_signals_en_o(dut_tbox_dbg_signals_en_o),
    .tbox_dbg_signals_i(dut_tbox_dbg_signals_i),
    .tbox_dbg_signals_mux_o(dut_tbox_dbg_signals_mux_o),
    .tbox_apb_req_o(dut_tbox_apb_req_o),
    .tbox_apb_rsp_i(dut_tbox_apb_rsp_i),
    .tbox_bpam_run_control_o(dut_tbox_bpam_run_control_o),
    .tbox_bpam_run_control_ack_i(dut_tbox_bpam_run_control_ack_i)
  );

endmodule : neigh_channel_tb
/* verilator lint_on UNUSEDSIGNAL */
