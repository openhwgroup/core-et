// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // Unit wrapper observes selected smoke outputs while tying the full top-level surface deterministically.
module neigh_top_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic isolate_i,
  input  logic dft_use_reset_cntl_i,
  input  logic dft_scanmode_i,
  input  logic dft_scan_reset_n_i,
  input  logic apb_psel_i,
  input  logic apb_penable_i,
  input  logic neigh_rsp_valid_i,
  input  logic icache_resp_valid_i,
  input  logic icache_req_ready_i,
  output logic effective_isolate_o,
  output logic rst_w_icache_no,
  output logic dft_scanout_zero_o,
  output logic dft_occ_scanout_o,
  output logic voltage_monitor_vdd_o,
  output logic voltage_monitor_vss_o,
  output logic apb_pready_o,
  output logic apb_pslverr_o,
  output logic [shirecache_pkg::Banks:0] neigh_sc_req_valid_o,
  output logic neigh_sc_rsp_ready_o,
  output logic icache_f2_sram_req_valid_o,
  output logic icache_f2_sram_req_write_o,
  output logic icache_f0_sram_resp_ready_o,
  output logic coop_tload_slv_rdy_out_valid_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_mst_done_out_valid_o,
  output logic bpam_halt_ack_o,
  output logic bpam_resume_ack_o,
  output logic esr_icache_prefetch_done_o,
  output logic esr_icache_err_detected_o,
  output logic esr_icache_err_logged_o,
  output logic [neigh_pkg::EtEcoNeighOutputWidth-1:0] eco_o,
  output logic pwr_ctrl_glb_nsleepout_o,
  output logic [neigh_pkg::MinPerN-1:0] pwr_ctrl_min_nsleepout_o,
  output logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] tbox_id_lo_o
);

  dft_pkg::dft_t dft_hv;
  neigh_pkg::shire_tdr_reset_cntl_t reset_cntl;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t chicken_bits;
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control;
  minion_pkg::esr_minion_features_t esr_minion_features;
  icache_pkg::icache_prefetch_conf_t icache_prefetch_conf;
  esr_pkg::esr_ms_dmctrl_t dmctrl;
  esr_pkg::esr_and_or_tree_l0_t esr_and_or_tree_l0;
  esr_pkg::esr_dll_dly_est_ctl_t dll_ctl;
  esr_pkg::esr_dll_dly_est_sts_t dll_sts;
  etlink_pkg::req_t neigh_sc_req_info;
  etlink_pkg::rsp_t neigh_sc_rsp_info;
  minion_pkg::neigh_sm_dbg_monitor_t neigh_sm_signals;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_req;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_rsp;
  neigh_pkg::coop_tload_slv_rdy_req_t coop_slv_rdy_out_data;
  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_slv_rdy_in_data;
  logic [neigh_pkg::ShireCoopIdSize-1:0] coop_mst_done_out_coop_id;
  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_mst_done_in_coop_id;
  dft_pkg::dft_t dft_tbox_lv;
  logic [neigh_pkg::ShireDftScaninTboxWidth-1:0] dft_scanin_to_tbox_lv;
  logic dft_occ_scanin_to_tbox_lv;
  logic dft_scan_enable_to_tbox_lv;
  logic [neigh_pkg::ShireDftModeWidth-1:0] dft_test_mode_to_tbox_lv;
  logic dft_clock_gate_en_to_tbox_lv;
  logic [neigh_pkg::ShireDftCntlWidth-1:0] dft_cntl_to_tbox_lv;
  logic dft_occ_reset_to_tbox_lv;
  logic dft_occ_testmode_to_tbox_lv;
  logic dft_scan_ate_clk_to_tbox_lv;
  logic dft_occ_bypass_to_tbox_lv;
  logic clk_tbox;
  logic tbox_rst_c_n;
  logic tbox_rst_d_n;
  logic tbox_rst_w_n;
  logic [15:0] tbox_control_esr;
  logic [minion_pkg::VaSize-1:0] tbox_image_table_ptr_esr;
  logic tbox_neigh_req_ready;
  etlink_pkg::neigh_req_t tbox_neigh_req_info;
  logic tbox_neigh_rsp_valid;
  etlink_pkg::rsp_t tbox_neigh_rsp_info;
  logic tbox_dbg_signals_en;
  minion_pkg::neigh_sm_dbg_monitor_t tbox_dbg_signals;
  logic [minion_pkg::NeighDebugSmMuxWidth-1:0] tbox_dbg_signals_mux;
  neigh_ch_apb_mux_pkg::apb_to_neigh_tbox_t tbox_apb_req;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t tbox_apb_rsp;
  neigh_ch_dbg_pkg::tbox_bpam_run_control_t tbox_bpam_run_control;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t tbox_bpam_ack;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_ack;
  logic [neigh_pkg::ShireDftScanoutNeighWidth-1:0] dft_scanout_hv;
  logic dft_occ_scanout_hv;
  logic [neigh_pkg::ShireDftScanoutTboxWidth-1:0] dft_scanout_tbox_hv;
  logic dft_occ_scanout_tbox_hv;
  logic dll_feedback_shire;
  logic dll_feedback_neigh;
  logic flb_neigh_l2_req_valid;
  logic [neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] flb_neigh_l2_req_data;
  logic icache_f2_sram_req_write;
  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_f2_sram_req_addr;

  assign dft_hv.scanmode = dft_scanmode_i;
  assign dft_hv.scan_reset_n = dft_scan_reset_n_i;
  assign dft_hv.sram_clk_override = 1'b0;
  assign dft_hv.ram_rei = 1'b0;
  assign dft_hv.ram_wei = 1'b0;
  assign reset_cntl = '0;
  assign chicken_bits = '0;
  assign bpam_run_control = '0;
  assign esr_minion_features = '0;
  assign icache_prefetch_conf = '0;
  assign dmctrl = '0;
  assign dll_ctl = '0;
  assign neigh_sc_rsp_info = '0;
  assign apb_req.apb_paddr = '0;
  assign apb_req.apb_penable = apb_penable_i;
  assign apb_req.apb_psel = apb_psel_i;
  assign apb_req.apb_pwdata = 64'h5a5a_a5a5_1122_3344;
  assign apb_req.apb_pwrite = 1'b1;
  assign coop_slv_rdy_in_data = '0;
  assign coop_mst_done_in_coop_id = '0;
  assign tbox_neigh_req_info = '0;
  assign tbox_dbg_signals = '0;
  assign tbox_apb_rsp = '1;
  assign tbox_bpam_ack = '0;

  assign dft_scanout_zero_o = (dft_scanout_hv == '0);
  assign dft_occ_scanout_o = dft_occ_scanout_hv;
  assign bpam_halt_ack_o = bpam_ack.halt_ack;
  assign bpam_resume_ack_o = bpam_ack.resume_ack;
  assign esr_icache_err_detected_o = esr_icache_err_detected;
  assign esr_icache_err_logged_o = esr_icache_err_logged;

  logic esr_icache_err_detected;
  logic esr_icache_err_logged;

  neigh_top #(
    .EnableGfx(1'b0),
    .DisableMinions('0),
    .StubMinions({neigh_pkg::MinPerN{1'b1}})
  ) u_dut (
    .clk_i(clk_i),
    .clk_shire_i(clk_i),
    .rst_c_shire_ni(rst_ni),
    .rst_d_shire_ni(rst_ni),
    .rst_w_shire_ni(rst_ni),
    .rst_warm_ni(rst_ni),
    .rst_w_icache_no(rst_w_icache_no),
    .dft_scanin_hv_i('0),
    .dft_scanout_hv_o(dft_scanout_hv),
    .dft_hv_i(dft_hv),
    .dft_scan_enable_hv_i(1'b0),
    .dft_test_mode_hv_i('0),
    .dft_clock_gate_en_hv_i(1'b0),
    .dft_cntl_hv_i('0),
    .dft_occ_scanin_hv_i(1'b0),
    .dft_occ_scanout_hv_o(dft_occ_scanout_hv),
    .dft_occ_reset_hv_i(1'b0),
    .dft_occ_testmode_hv_i(1'b0),
    .dft_scan_ate_clk_hv_i(1'b0),
    .dft_occ_bypass_hv_i(1'b0),
    .dft_use_reset_cntl_hv_i(dft_use_reset_cntl_i),
    .dft_reset_cntl_hv_i(reset_cntl),
    .dft_scanin_tbox_hv_i('0),
    .dft_scanout_tbox_hv_o(dft_scanout_tbox_hv),
    .dft_occ_scanin_tbox_hv_i(1'b0),
    .dft_occ_scanout_tbox_hv_o(dft_occ_scanout_tbox_hv),
    .eco_i(10'h3ff),
    .eco_o(eco_o),
    .pwr_ctrl_glb_nsleepin_i(1'b1),
    .pwr_ctrl_glb_nsleepout_o(pwr_ctrl_glb_nsleepout_o),
    .pwr_ctrl_glb_isolate_i(isolate_i),
    .pwr_ctrl_min_nsleepin_i('1),
    .pwr_ctrl_min_nsleepout_o(pwr_ctrl_min_nsleepout_o),
    .pwr_ctrl_min_isolate_i('0),
    .esr_clk_gate_ctrl_i(chicken_bits),
    .esr_minion_mem_override_i('0),
    .bpam_rc_tbox_ack_hi_o(bpam_ack),
    .bpam_run_control_i(bpam_run_control),
    .shire_id_i('0),
    .neigh_id_i('0),
    .shire_tbox_id_i('0),
    .shire_tbox_en_i(1'b0),
    .esr_thread0_enable_i('0),
    .esr_thread1_enable_i('0),
    .esr_minion_features_i(esr_minion_features),
    .esr_icache_prefetch_conf_i(icache_prefetch_conf),
    .esr_icache_prefetch_start_i(1'b0),
    .esr_icache_prefetch_done_o(esr_icache_prefetch_done_o),
    .esr_icache_err_detected_o(esr_icache_err_detected),
    .esr_icache_err_logged_o(esr_icache_err_logged),
    .dmctrl_i(dmctrl),
    .esr_and_or_tree_l0_o(esr_and_or_tree_l0),
    .esr_shire_coop_mode_i(1'b0),
    .dll_feedback_shire_o(dll_feedback_shire),
    .dll_feedback_neigh_o(dll_feedback_neigh),
    .esr_dll_dly_est_ctl_i(dll_ctl),
    .esr_dll_dly_est_sts_o(dll_sts),
    .neigh_sc_req_ready_i('1),
    .neigh_sc_req_valid_o(neigh_sc_req_valid_o),
    .neigh_sc_req_info_o(neigh_sc_req_info),
    .neigh_sc_rsp_ready_o(neigh_sc_rsp_ready_o),
    .neigh_sc_rsp_valid_i(neigh_rsp_valid_i),
    .neigh_sc_rsp_info_i(neigh_sc_rsp_info),
    .hw_dbg_sm_monitor_enabled_i(1'b0),
    .neigh_sm_gpio_i('0),
    .neigh_sm_signals_o(neigh_sm_signals),
    .apb_esr_req_i(apb_req),
    .apb_esr_rsp_o(apb_rsp),
    .int_mtip_i('0),
    .int_meip_i(1'b0),
    .int_seip_i(1'b0),
    .ipi_msip_i('0),
    .ipi_redirect_trigger_i('0),
    .uc_to_neigh_fcc_i('0),
    .uc_to_neigh_fcc_target_i('0),
    .flb_neigh_l2_req_valid_o(flb_neigh_l2_req_valid),
    .flb_neigh_l2_req_data_o(flb_neigh_l2_req_data),
    .flb_l2_neigh_resp_valid_i(1'b0),
    .flb_l2_neigh_resp_data_i(1'b0),
    .icache_f2_sram_req_write_o(icache_f2_sram_req_write),
    .icache_f2_sram_req_addr_o(icache_f2_sram_req_addr),
    .icache_f2_sram_req_valid_o(icache_f2_sram_req_valid_o),
    .icache_f2_sram_req_ready_i(icache_req_ready_i),
    .icache_f0_sram_resp_dout_i('0),
    .icache_f0_sram_resp_valid_i(icache_resp_valid_i),
    .icache_f0_sram_resp_ready_o(icache_f0_sram_resp_ready_o),
    .voltage_monitor_vdd_o(voltage_monitor_vdd_o),
    .voltage_monitor_vss_o(voltage_monitor_vss_o),
    .clk_tbox_o(clk_tbox),
    .tbox_rst_c_no(tbox_rst_c_n),
    .tbox_rst_d_no(tbox_rst_d_n),
    .tbox_rst_w_no(tbox_rst_w_n),
    .dft_tbox_lv_o(dft_tbox_lv),
    .dft_scanin_to_tbox_lv_o(dft_scanin_to_tbox_lv),
    .dft_scanout_to_tbox_lv_i('0),
    .dft_occ_scanin_to_tbox_lv_o(dft_occ_scanin_to_tbox_lv),
    .dft_occ_scanout_to_tbox_lv_i(1'b0),
    .dft_scan_enable_to_tbox_lv_o(dft_scan_enable_to_tbox_lv),
    .dft_test_mode_to_tbox_lv_o(dft_test_mode_to_tbox_lv),
    .dft_clock_gate_en_to_tbox_lv_o(dft_clock_gate_en_to_tbox_lv),
    .dft_cntl_to_tbox_lv_o(dft_cntl_to_tbox_lv),
    .dft_occ_reset_to_tbox_lv_o(dft_occ_reset_to_tbox_lv),
    .dft_occ_testmode_to_tbox_lv_o(dft_occ_testmode_to_tbox_lv),
    .dft_scan_ate_clk_to_tbox_lv_o(dft_scan_ate_clk_to_tbox_lv),
    .dft_occ_bypass_to_tbox_lv_o(dft_occ_bypass_to_tbox_lv),
    .tbox_id_lo_o(tbox_id_lo_o),
    .tbox_control_esr_o(tbox_control_esr),
    .tbox_status_esr_i('0),
    .tbox_image_table_ptr_esr_o(tbox_image_table_ptr_esr),
    .tbox_neigh_req_ready_o(tbox_neigh_req_ready),
    .tbox_neigh_req_valid_i(1'b0),
    .tbox_neigh_req_info_i(tbox_neigh_req_info),
    .tbox_neigh_rsp_ready_i(1'b0),
    .tbox_neigh_rsp_valid_o(tbox_neigh_rsp_valid),
    .tbox_neigh_rsp_info_o(tbox_neigh_rsp_info),
    .tbox_dbg_signals_en_o(tbox_dbg_signals_en),
    .tbox_dbg_signals_i(tbox_dbg_signals),
    .tbox_dbg_signals_mux_o(tbox_dbg_signals_mux),
    .tbox_apb_req_o(tbox_apb_req),
    .tbox_apb_rsp_i(tbox_apb_rsp),
    .tbox_bpam_run_control_o(tbox_bpam_run_control),
    .tbox_bpam_run_control_ack_i(tbox_bpam_ack),
    .coop_tload_slv_rdy_out_data_o(coop_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_o(coop_tload_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data_i(coop_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_i('0),
    .coop_tload_mst_done_out_coop_id_o(coop_mst_done_out_coop_id),
    .coop_tload_mst_done_out_valid_o(coop_tload_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id_i(coop_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_i('0)
  );

  assign effective_isolate_o = u_dut.pwr_ctrl_glb_isolate_int;
  assign apb_pready_o = apb_rsp.apb_pready;
  assign apb_pslverr_o = apb_rsp.apb_pslverr;
  assign icache_f2_sram_req_write_o = icache_f2_sram_req_write;

endmodule : neigh_top_tb
/* verilator lint_on UNUSEDSIGNAL */
