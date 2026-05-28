// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */  // Full-top cosim drives and observes the broad integration surface through aggregate chunks.
/* verilator lint_off WIDTHTRUNC */  // Deterministic salt patterns intentionally truncate to each translated field width.
/* verilator lint_off WIDTHEXPAND */  // Packed original vectors and translated structs are width-equivalent by construction.
/* verilator lint_off ENUMVALUE */  // Reserved enum encodings may be driven during identical random stimulus on both DUTs.
module cosim_neigh_top_tb
  import dft_pkg::*;
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
  import esr_pkg::*;
  import neigh_ch_dbg_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  output logic [63:0] orig_chunks_o [128],
  output logic [63:0] new_chunks_o [128],
  output logic input_toggle_ok_o
);

  localparam int unsigned OutBits = 8192;
  localparam int unsigned OutChunks = 128;
  localparam int unsigned ResetExerciseCycles = 2048;
  localparam int unsigned DriveMinSalt = 0;
  localparam int unsigned DriveMaxSalt = 68;
  localparam int unsigned DriveSaltCount = DriveMaxSalt + 1;

  logic [31:0] cycle_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) cycle_q <= '0;
    else         cycle_q <= cycle_q + 32'd1;
  end

  function automatic logic drive_bit(input int unsigned salt);
    logic [31:0] x;
    x = cycle_q ^ (32'h9e3779b9 * (salt + 1));
    drive_bit = x[0] ^ x[3] ^ x[7] ^ x[13] ^ x[21];
  endfunction

  logic [DriveSaltCount-1:0] drive_seen_zero_q;
  logic [DriveSaltCount-1:0] drive_seen_one_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      drive_seen_zero_q <= '0;
      drive_seen_one_q <= '0;
    end else begin
      for (int unsigned salt = DriveMinSalt; salt <= DriveMaxSalt; salt++) begin
        if (drive_bit(salt)) drive_seen_one_q[salt] <= 1'b1;
        else                 drive_seen_zero_q[salt] <= 1'b1;
      end
    end
  end

  logic reset_exercise_phase;
  assign reset_exercise_phase = (cycle_q < ResetExerciseCycles);

  logic clk_shire;
  logic rst_c_shire_n;
  logic rst_d_shire_n;
  logic rst_w_shire_n;
  logic rst_warm_n;
  assign clk_shire = clk_i;
  always_comb begin
    rst_c_shire_n = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd120));
    rst_d_shire_n = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd180));
    rst_w_shire_n = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd240));
    rst_warm_n = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd300));
  end

  dft_t dft_hv;
  logic dft_scan_mode_hv;
  logic dft_scan_enable_hv;
  logic [neigh_pkg::ShireDftModeWidth-1:0] dft_test_mode_hv;
  logic dft_clock_gate_en_hv;
  logic [neigh_pkg::ShireDftCntlWidth-1:0] dft_cntl_hv;
  logic dft_occ_scanin_hv;
  logic dft_occ_reset_hv;
  logic dft_occ_testmode_hv;
  logic dft_scan_ate_clk_hv;
  logic dft_occ_bypass_hv;
  logic dft_use_reset_cntl_hv;
  shire_tdr_reset_cntl_t orig_reset_cntl_hv;
  neigh_pkg::shire_tdr_reset_cntl_t new_reset_cntl_hv;

  logic [neigh_pkg::EtEcoNeighInputWidth-1:0] eco_i;
  logic pwr_ctrl_glb_nsleepin;
  logic pwr_ctrl_glb_isolate;
  logic [neigh_pkg::MinPerN-1:0] pwr_ctrl_min_nsleepin;
  logic [neigh_pkg::MinPerN-1:0] pwr_ctrl_min_isolate;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t new_chicken_bits;
  minion_chicken_bits_vc_t orig_chicken_bits;
  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override;
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t new_bpam_run_control;
  bpam_run_control_neigh_t orig_bpam_run_control;
  logic [minion_pkg::NumShireIdsBits-1:0] shire_id;
  logic [neigh_pkg::NumNeighWidth-1:0] neigh_id;
  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] shire_tbox_id;
  logic shire_tbox_en;
  logic [neigh_pkg::MinPerN-1:0] esr_thread0_enable;
  logic [neigh_pkg::MinPerN-1:0] esr_thread1_enable;
  minion_pkg::esr_minion_features_t new_esr_minion_features;
  esr_minion_features_t orig_esr_minion_features;
  icache_pkg::icache_prefetch_conf_t new_icache_prefetch_conf;
  icache_prefetch_conf_t orig_icache_prefetch_conf;
  logic esr_icache_prefetch_start;
  esr_pkg::esr_ms_dmctrl_t new_dmctrl;
  esr_ms_dmctrl_t orig_dmctrl;
  logic esr_shire_coop_mode;
  esr_pkg::esr_dll_dly_est_ctl_t new_dll_ctl;
  esr_dll_dly_est_ctl_t orig_dll_ctl;
  logic [shirecache_pkg::Banks:0] neigh_sc_req_ready;
  logic neigh_sc_rsp_valid;
  etlink_pkg::rsp_t new_neigh_sc_rsp_info;
  et_link_rsp_info_t orig_neigh_sc_rsp_info;
  logic hw_dbg_sm_monitor_enabled;
  logic [neigh_voltage_cross_pkg::NeighSmGpioWidth-1:0] neigh_sm_gpio;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t new_apb_req;
  APB_to_neigh_t orig_apb_req;
  logic [neigh_pkg::MinPerN-1:0] int_mtip;
  logic int_meip;
  logic int_seip;
  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] ipi_msip;
  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] ipi_redirect_trigger;
  logic [neigh_pkg::MinPerN-1:0] uc_to_neigh_fcc;
  logic [neigh_voltage_cross_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target;
  logic flb_l2_neigh_resp_valid;
  logic flb_l2_neigh_resp_data;
  logic icache_f2_sram_req_ready;
  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] icache_f0_sram_resp_dout;
  logic icache_f0_sram_resp_valid;
  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] new_coop_slv_rdy_in_data;
  coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] orig_coop_slv_rdy_in_data;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_slv_rdy_in_valid;
  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_mst_done_in_coop_id;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_mst_done_in_valid;

  // Extra always-present translated TBOX ports are driven in the non-GFX build
  // to prove they are ignored/tied off when EnableGfx=0.
  logic [15:0] tbox_status_esr;
  logic tbox_neigh_req_valid;
  etlink_pkg::neigh_req_t tbox_neigh_req_info;
  logic tbox_neigh_rsp_ready;
  minion_pkg::neigh_sm_dbg_monitor_t tbox_dbg_signals;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t tbox_apb_rsp;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t tbox_bpam_ack;
  logic [neigh_pkg::ShireDftScanoutTboxWidth-1:0] dft_scanout_to_tbox_lv;
  logic dft_occ_scanout_to_tbox_lv;

  // Reset-control, isolation, and protocol-valid fields are randomized only
  // during the reset exercise window, then held benign for the strict compare
  // phase. This still toggles the practical input pins both ways without
  // continuously resetting the hierarchy or popping idle FIFOs while comparing.
  always_comb begin
    dft_hv.scanmode = reset_exercise_phase ? drive_bit(0) : drive_bit(1);
    dft_hv.scan_reset_n = !(reset_exercise_phase && drive_bit(2));
    dft_hv.sram_clk_override = drive_bit(3);
    dft_hv.ram_rei = drive_bit(4);
    dft_hv.ram_wei = drive_bit(5);
    dft_scan_mode_hv = drive_bit(6);
    dft_scan_enable_hv = drive_bit(7);
    dft_test_mode_hv = drive_bit(8) ? '1 : '0;
    dft_clock_gate_en_hv = drive_bit(9);
    dft_cntl_hv = drive_bit(10) ? '1 : '0;
    dft_occ_scanin_hv = drive_bit(11);
    dft_occ_reset_hv = drive_bit(12);
    dft_occ_testmode_hv = drive_bit(13);
    dft_scan_ate_clk_hv = drive_bit(14);
    dft_occ_bypass_hv = drive_bit(15);
    dft_use_reset_cntl_hv = reset_exercise_phase ? drive_bit(16) : 1'b0;
    orig_reset_cntl_hv = drive_bit(17) ? '1 : '0;
    new_reset_cntl_hv = orig_reset_cntl_hv;
    eco_i = drive_bit(18) ? '1 : '0;
    pwr_ctrl_glb_nsleepin = drive_bit(19);
    pwr_ctrl_glb_isolate = reset_exercise_phase ? drive_bit(20) : 1'b0;
    pwr_ctrl_min_nsleepin = drive_bit(21) ? '1 : '0;
    pwr_ctrl_min_isolate = reset_exercise_phase && drive_bit(22) ? '1 : '0;
    orig_chicken_bits = drive_bit(23) ? '1 : '0;
    new_chicken_bits = orig_chicken_bits;
    esr_minion_mem_override = drive_bit(24) ? '1 : '0;
    orig_bpam_run_control = drive_bit(25) ? '1 : '0;
    if (!reset_exercise_phase) orig_bpam_run_control.gpio.ndmreset = 1'b0;
    new_bpam_run_control = orig_bpam_run_control;
    shire_id = drive_bit(26) ? '1 : '0;
    neigh_id = drive_bit(27) ? '1 : '0;
    shire_tbox_id = drive_bit(28) ? '1 : '0;
    shire_tbox_en = drive_bit(29);
    esr_thread0_enable = drive_bit(30) ? '1 : '0;
    esr_thread1_enable = drive_bit(31) ? '1 : '0;
    orig_esr_minion_features = drive_bit(32) ? '1 : '0;
    new_esr_minion_features = orig_esr_minion_features;
    orig_icache_prefetch_conf = drive_bit(33) ? '1 : '0;
    new_icache_prefetch_conf = orig_icache_prefetch_conf;
    esr_icache_prefetch_start = drive_bit(34);
    orig_dmctrl = drive_bit(35) ? '1 : '0;
    if (!reset_exercise_phase) begin
      orig_dmctrl.ndmreset = 1'b0;
      orig_dmctrl.dmactive = 1'b1;
    end
    new_dmctrl = orig_dmctrl;
    esr_shire_coop_mode = drive_bit(36);
    orig_dll_ctl = drive_bit(37) ? '1 : '0;
    new_dll_ctl = orig_dll_ctl;
    neigh_sc_req_ready = drive_bit(38) ? '1 : '0;
    neigh_sc_rsp_valid = drive_bit(39);
    orig_neigh_sc_rsp_info = drive_bit(40) ? '1 : '0;
    new_neigh_sc_rsp_info = orig_neigh_sc_rsp_info;
    hw_dbg_sm_monitor_enabled = drive_bit(41);
    neigh_sm_gpio = drive_bit(42) ? '1 : '0;
    orig_apb_req = drive_bit(43) ? '1 : '0;
    new_apb_req = orig_apb_req;
    int_mtip = drive_bit(44) ? '1 : '0;
    int_meip = drive_bit(45);
    int_seip = drive_bit(46);
    ipi_msip = drive_bit(47) ? '1 : '0;
    ipi_redirect_trigger = drive_bit(48) ? '1 : '0;
    uc_to_neigh_fcc = drive_bit(49) ? '1 : '0;
    uc_to_neigh_fcc_target = drive_bit(50) ? '1 : '0;
    flb_l2_neigh_resp_valid = reset_exercise_phase ? drive_bit(51) : 1'b0;
    flb_l2_neigh_resp_data = drive_bit(52);
    icache_f2_sram_req_ready = drive_bit(53);
    icache_f0_sram_resp_dout = drive_bit(54) ? '1 : '0;
    icache_f0_sram_resp_valid = drive_bit(55);
    orig_coop_slv_rdy_in_data = drive_bit(56) ? '1 : '0;
    new_coop_slv_rdy_in_data = orig_coop_slv_rdy_in_data;
    coop_slv_rdy_in_valid = drive_bit(57) ? '1 : '0;
    coop_mst_done_in_coop_id = drive_bit(58) ? '1 : '0;
    coop_mst_done_in_valid = drive_bit(59) ? '1 : '0;
    tbox_status_esr = drive_bit(60) ? '1 : '0;
    tbox_neigh_req_valid = drive_bit(61);
    tbox_neigh_req_info = drive_bit(62) ? '1 : '0;
    tbox_neigh_rsp_ready = drive_bit(63);
    tbox_dbg_signals = drive_bit(64) ? '1 : '0;
    tbox_apb_rsp = drive_bit(65) ? '1 : '0;
    tbox_bpam_ack = drive_bit(66) ? '1 : '0;
    dft_scanout_to_tbox_lv = drive_bit(67) ? '1 : '0;
    dft_occ_scanout_to_tbox_lv = drive_bit(68);
  end

  // Original outputs.
  logic orig_reset_w_icache;
  logic [neigh_pkg::ShireDftScanoutNeighWidth-1:0] orig_dft_scanout_hv;
  logic orig_dft_occ_scanout_hv;
  logic [neigh_pkg::EtEcoNeighOutputWidth-1:0] orig_eco_o;
  logic orig_pwr_ctrl_glb_nsleepout;
  logic [neigh_pkg::MinPerN-1:0] orig_pwr_ctrl_min_nsleepout;
  bpam_rc_tbox_ack_t orig_bpam_ack;
  logic orig_prefetch_done;
  logic orig_icache_err_detected;
  logic orig_icache_err_logged;
  esr_and_or_tree_L0_t orig_esr_tree;
  logic orig_dll_feedback_shire;
  logic orig_dll_feedback_neigh;
  esr_dll_dly_est_sts_t orig_dll_sts;
  logic [shirecache_pkg::Banks:0] orig_neigh_sc_req_valid;
  et_link_req_info_t orig_neigh_sc_req_info;
  logic orig_neigh_sc_rsp_ready;
  neigh_sm_dbg_monitor_t orig_neigh_sm_signals;
  APB_from_neigh_t orig_apb_rsp;
  logic orig_flb_req_valid;
  logic [neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] orig_flb_req_data;
  logic orig_icache_req_write;
  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] orig_icache_req_addr;
  logic orig_icache_req_valid;
  logic orig_icache_resp_ready;
  logic orig_voltage_vdd;
  logic orig_voltage_vss;
  coop_tload_slv_rdy_req_t orig_coop_slv_rdy_out_data;
  logic orig_coop_slv_rdy_out_valid;
  logic [neigh_pkg::ShireCoopIdSize-1:0] orig_coop_mst_done_out_coop_id;
  logic [neigh_pkg::NumOtherNeigh-1:0] orig_coop_mst_done_out_valid;

  neigh_top_orig #(
    .DISABLE_MINIONS({neigh_pkg::MinPerN{1'b0}}),
    .STUB_MINIONS({neigh_pkg::MinPerN{1'b1}})
  ) u_orig (
    .clock(clk_i),
    .clock_shire(clk_shire),
    .reset_c_shire(!rst_c_shire_n),
    .reset_d_shire(!rst_d_shire_n),
    .reset_w_shire(!rst_w_shire_n),
    .reset_warm(!rst_warm_n),
    .reset_w_icache(orig_reset_w_icache),
    .dft__scanin_hv('0),
    .dft__scanout_hv(orig_dft_scanout_hv),
    .dft__scan_mode_hv(dft_scan_mode_hv),
    .dft__reset_byp_hv(dft_hv.scanmode),
    .dft__scan_enable_hv(dft_scan_enable_hv),
    .dft__test_mode_hv(dft_test_mode_hv),
    .dft__reset_hv(!dft_hv.scan_reset_n),
    .dft__clock_gate_en_hv(dft_clock_gate_en_hv),
    .dft__cntl_hv(dft_cntl_hv),
    .dft__occ_scanin_hv(dft_occ_scanin_hv),
    .dft__occ_scanout_hv(orig_dft_occ_scanout_hv),
    .dft__occ_reset_hv(dft_occ_reset_hv),
    .dft__occ_testmode_hv(dft_occ_testmode_hv),
    .dft__scan_ate_clk_hv(dft_scan_ate_clk_hv),
    .dft__occ_bypass_hv(dft_occ_bypass_hv),
    .dft__use_reset_cntl_hv(dft_use_reset_cntl_hv),
    .dft__reset_cntl_hv(orig_reset_cntl_hv),
    .eco_i(eco_i),
    .eco_o(orig_eco_o),
    .pwr_ctrl_glb_nsleepin(pwr_ctrl_glb_nsleepin),
    .pwr_ctrl_glb_nsleepout(orig_pwr_ctrl_glb_nsleepout),
    .pwr_ctrl_glb_isolate(pwr_ctrl_glb_isolate),
    .pwr_ctrl_min_nsleepin(pwr_ctrl_min_nsleepin),
    .pwr_ctrl_min_nsleepout(orig_pwr_ctrl_min_nsleepout),
    .pwr_ctrl_min_isolate(pwr_ctrl_min_isolate),
    .esr_clk_gate_ctrl(orig_chicken_bits),
    .esr_minion_mem_override(esr_minion_mem_override),
    .bpam_rc_tbox_ack_hi(orig_bpam_ack),
    .bpam_run_control(orig_bpam_run_control),
    .shire_id(shire_id),
    .neigh_id(neigh_id),
    .shire_tbox_id(shire_tbox_id),
    .shire_tbox_en(shire_tbox_en),
    .esr_thread0_enable(esr_thread0_enable),
    .esr_thread1_enable(esr_thread1_enable),
    .esr_minion_features(orig_esr_minion_features),
    .esr_icache_prefetch_conf(orig_icache_prefetch_conf),
    .esr_icache_prefetch_start(esr_icache_prefetch_start),
    .esr_icache_prefetch_done(orig_prefetch_done),
    .esr_icache_err_detected(orig_icache_err_detected),
    .esr_icache_err_logged(orig_icache_err_logged),
    .dmctrl(orig_dmctrl),
    .esr_and_or_tree_L0(orig_esr_tree),
    .esr_shire_coop_mode(esr_shire_coop_mode),
    .dll_feedback_shire(orig_dll_feedback_shire),
    .dll_feedback_neigh(orig_dll_feedback_neigh),
    .esr_dll_dly_est_ctl(orig_dll_ctl),
    .esr_dll_dly_est_sts(orig_dll_sts),
    .neigh_sc_req_ready(neigh_sc_req_ready),
    .neigh_sc_req_valid(orig_neigh_sc_req_valid),
    .neigh_sc_req_info(orig_neigh_sc_req_info),
    .neigh_sc_rsp_ready(orig_neigh_sc_rsp_ready),
    .neigh_sc_rsp_valid(neigh_sc_rsp_valid),
    .neigh_sc_rsp_info(orig_neigh_sc_rsp_info),
    .hw_dbg_sm_monitor_enabled(hw_dbg_sm_monitor_enabled),
    .neigh_sm_gpio(neigh_sm_gpio),
    .neigh_sm_signals(orig_neigh_sm_signals),
    .APB_ESR_req(orig_apb_req),
    .APB_ESR_rsp(orig_apb_rsp),
    .int_mtip(int_mtip),
    .int_meip(int_meip),
    .int_seip(int_seip),
    .ipi_msip(ipi_msip),
    .ipi_redirect_trigger(ipi_redirect_trigger),
    .uc_to_neigh_fcc(uc_to_neigh_fcc),
    .uc_to_neigh_fcc_target(uc_to_neigh_fcc_target),
    .flb_neigh_l2_req_valid(orig_flb_req_valid),
    .flb_neigh_l2_req_data(orig_flb_req_data),
    .flb_l2_neigh_resp_valid(flb_l2_neigh_resp_valid),
    .flb_l2_neigh_resp_data(flb_l2_neigh_resp_data),
    .icache_f2_sram_req_write(orig_icache_req_write),
    .icache_f2_sram_req_addr(orig_icache_req_addr),
    .icache_f2_sram_req_valid(orig_icache_req_valid),
    .icache_f2_sram_req_ready(icache_f2_sram_req_ready),
    .icache_f0_sram_resp_dout(icache_f0_sram_resp_dout),
    .icache_f0_sram_resp_valid(icache_f0_sram_resp_valid),
    .icache_f0_sram_resp_ready(orig_icache_resp_ready),
    .voltage_monitor_vdd(orig_voltage_vdd),
    .voltage_monitor_vss(orig_voltage_vss),
    .coop_tload_slv_rdy_out_data(orig_coop_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid(orig_coop_slv_rdy_out_valid),
    .coop_tload_slv_rdy_in_data(orig_coop_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid(coop_slv_rdy_in_valid),
    .coop_tload_mst_done_out_coop_id(orig_coop_mst_done_out_coop_id),
    .coop_tload_mst_done_out_valid(orig_coop_mst_done_out_valid),
    .coop_tload_mst_done_in_coop_id(coop_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid(coop_mst_done_in_valid)
  );

  // Translated outputs.
  logic new_rst_w_icache_n;
  logic [neigh_pkg::ShireDftScanoutNeighWidth-1:0] new_dft_scanout_hv;
  logic new_dft_occ_scanout_hv;
  logic [neigh_pkg::ShireDftScanoutTboxWidth-1:0] new_dft_scanout_tbox_hv;
  logic new_dft_occ_scanout_tbox_hv;
  logic [neigh_pkg::EtEcoNeighOutputWidth-1:0] new_eco_o;
  logic new_pwr_ctrl_glb_nsleepout;
  logic [neigh_pkg::MinPerN-1:0] new_pwr_ctrl_min_nsleepout;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_bpam_ack;
  logic new_prefetch_done;
  logic new_icache_err_detected;
  logic new_icache_err_logged;
  esr_pkg::esr_and_or_tree_l0_t new_esr_tree;
  logic new_dll_feedback_shire;
  logic new_dll_feedback_neigh;
  esr_pkg::esr_dll_dly_est_sts_t new_dll_sts;
  logic [shirecache_pkg::Banks:0] new_neigh_sc_req_valid;
  etlink_pkg::req_t new_neigh_sc_req_info;
  logic new_neigh_sc_rsp_ready;
  minion_pkg::neigh_sm_dbg_monitor_t new_neigh_sm_signals;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t new_apb_rsp;
  logic new_flb_req_valid;
  logic [neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] new_flb_req_data;
  logic new_icache_req_write;
  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] new_icache_req_addr;
  logic new_icache_req_valid;
  logic new_icache_resp_ready;
  logic new_voltage_vdd;
  logic new_voltage_vss;
  neigh_pkg::coop_tload_slv_rdy_req_t new_coop_slv_rdy_out_data;
  logic new_coop_slv_rdy_out_valid;
  logic [neigh_pkg::ShireCoopIdSize-1:0] new_coop_mst_done_out_coop_id;
  logic [neigh_pkg::NumOtherNeigh-1:0] new_coop_mst_done_out_valid;
  logic new_clk_tbox;
  logic new_tbox_rst_c_n;
  logic new_tbox_rst_d_n;
  logic new_tbox_rst_w_n;
  dft_t new_dft_tbox_lv;
  logic [neigh_pkg::ShireDftScaninTboxWidth-1:0] new_dft_scanin_to_tbox_lv;
  logic new_dft_occ_scanin_to_tbox_lv;
  logic new_dft_scan_enable_to_tbox_lv;
  logic [neigh_pkg::ShireDftModeWidth-1:0] new_dft_test_mode_to_tbox_lv;
  logic new_dft_clock_gate_en_to_tbox_lv;
  logic [neigh_pkg::ShireDftCntlWidth-1:0] new_dft_cntl_to_tbox_lv;
  logic new_dft_occ_reset_to_tbox_lv;
  logic new_dft_occ_testmode_to_tbox_lv;
  logic new_dft_scan_ate_clk_to_tbox_lv;
  logic new_dft_occ_bypass_to_tbox_lv;
  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] new_tbox_id_lo;
  logic [15:0] new_tbox_control_esr;
  logic [minion_pkg::VaSize-1:0] new_tbox_image_table_ptr_esr;
  logic new_tbox_neigh_req_ready;
  logic new_tbox_neigh_rsp_valid;
  etlink_pkg::rsp_t new_tbox_neigh_rsp_info;
  logic new_tbox_dbg_signals_en;
  logic [minion_pkg::NeighDebugSmMuxWidth-1:0] new_tbox_dbg_signals_mux;
  neigh_ch_apb_mux_pkg::apb_to_neigh_tbox_t new_tbox_apb_req;
  neigh_ch_dbg_pkg::tbox_bpam_run_control_t new_tbox_bpam_run_control;

  neigh_top #(
    .EnableGfx(1'b0),
    .DisableMinions('0),
    .StubMinions({neigh_pkg::MinPerN{1'b1}})
  ) u_new (
    .clk_i(clk_i),
    .clk_shire_i(clk_shire),
    .rst_c_shire_ni(rst_c_shire_n),
    .rst_d_shire_ni(rst_d_shire_n),
    .rst_w_shire_ni(rst_w_shire_n),
    .rst_warm_ni(rst_warm_n),
    .rst_w_icache_no(new_rst_w_icache_n),
    .dft_scanin_hv_i('0),
    .dft_scanout_hv_o(new_dft_scanout_hv),
    .dft_hv_i(dft_hv),
    .dft_scan_enable_hv_i(dft_scan_enable_hv),
    .dft_test_mode_hv_i(dft_test_mode_hv),
    .dft_clock_gate_en_hv_i(dft_clock_gate_en_hv),
    .dft_cntl_hv_i(dft_cntl_hv),
    .dft_occ_scanin_hv_i(dft_occ_scanin_hv),
    .dft_occ_scanout_hv_o(new_dft_occ_scanout_hv),
    .dft_occ_reset_hv_i(dft_occ_reset_hv),
    .dft_occ_testmode_hv_i(dft_occ_testmode_hv),
    .dft_scan_ate_clk_hv_i(dft_scan_ate_clk_hv),
    .dft_occ_bypass_hv_i(dft_occ_bypass_hv),
    .dft_use_reset_cntl_hv_i(dft_use_reset_cntl_hv),
    .dft_reset_cntl_hv_i(new_reset_cntl_hv),
    .dft_scanin_tbox_hv_i('0),
    .dft_scanout_tbox_hv_o(new_dft_scanout_tbox_hv),
    .dft_occ_scanin_tbox_hv_i(dft_occ_scanin_hv),
    .dft_occ_scanout_tbox_hv_o(new_dft_occ_scanout_tbox_hv),
    .eco_i(eco_i),
    .eco_o(new_eco_o),
    .pwr_ctrl_glb_nsleepin_i(pwr_ctrl_glb_nsleepin),
    .pwr_ctrl_glb_nsleepout_o(new_pwr_ctrl_glb_nsleepout),
    .pwr_ctrl_glb_isolate_i(pwr_ctrl_glb_isolate),
    .pwr_ctrl_min_nsleepin_i(pwr_ctrl_min_nsleepin),
    .pwr_ctrl_min_nsleepout_o(new_pwr_ctrl_min_nsleepout),
    .pwr_ctrl_min_isolate_i(pwr_ctrl_min_isolate),
    .esr_clk_gate_ctrl_i(new_chicken_bits),
    .esr_minion_mem_override_i(esr_minion_mem_override),
    .bpam_rc_tbox_ack_hi_o(new_bpam_ack),
    .bpam_run_control_i(new_bpam_run_control),
    .shire_id_i(shire_id),
    .neigh_id_i(neigh_id),
    .shire_tbox_id_i(shire_tbox_id),
    .shire_tbox_en_i(shire_tbox_en),
    .esr_thread0_enable_i(esr_thread0_enable),
    .esr_thread1_enable_i(esr_thread1_enable),
    .esr_minion_features_i(new_esr_minion_features),
    .esr_icache_prefetch_conf_i(new_icache_prefetch_conf),
    .esr_icache_prefetch_start_i(esr_icache_prefetch_start),
    .esr_icache_prefetch_done_o(new_prefetch_done),
    .esr_icache_err_detected_o(new_icache_err_detected),
    .esr_icache_err_logged_o(new_icache_err_logged),
    .dmctrl_i(new_dmctrl),
    .esr_and_or_tree_l0_o(new_esr_tree),
    .esr_shire_coop_mode_i(esr_shire_coop_mode),
    .dll_feedback_shire_o(new_dll_feedback_shire),
    .dll_feedback_neigh_o(new_dll_feedback_neigh),
    .esr_dll_dly_est_ctl_i(new_dll_ctl),
    .esr_dll_dly_est_sts_o(new_dll_sts),
    .neigh_sc_req_ready_i(neigh_sc_req_ready),
    .neigh_sc_req_valid_o(new_neigh_sc_req_valid),
    .neigh_sc_req_info_o(new_neigh_sc_req_info),
    .neigh_sc_rsp_ready_o(new_neigh_sc_rsp_ready),
    .neigh_sc_rsp_valid_i(neigh_sc_rsp_valid),
    .neigh_sc_rsp_info_i(new_neigh_sc_rsp_info),
    .hw_dbg_sm_monitor_enabled_i(hw_dbg_sm_monitor_enabled),
    .neigh_sm_gpio_i(neigh_sm_gpio),
    .neigh_sm_signals_o(new_neigh_sm_signals),
    .apb_esr_req_i(new_apb_req),
    .apb_esr_rsp_o(new_apb_rsp),
    .int_mtip_i(int_mtip),
    .int_meip_i(int_meip),
    .int_seip_i(int_seip),
    .ipi_msip_i(ipi_msip),
    .ipi_redirect_trigger_i(ipi_redirect_trigger),
    .uc_to_neigh_fcc_i(uc_to_neigh_fcc),
    .uc_to_neigh_fcc_target_i(uc_to_neigh_fcc_target),
    .flb_neigh_l2_req_valid_o(new_flb_req_valid),
    .flb_neigh_l2_req_data_o(new_flb_req_data),
    .flb_l2_neigh_resp_valid_i(flb_l2_neigh_resp_valid),
    .flb_l2_neigh_resp_data_i(flb_l2_neigh_resp_data),
    .icache_f2_sram_req_write_o(new_icache_req_write),
    .icache_f2_sram_req_addr_o(new_icache_req_addr),
    .icache_f2_sram_req_valid_o(new_icache_req_valid),
    .icache_f2_sram_req_ready_i(icache_f2_sram_req_ready),
    .icache_f0_sram_resp_dout_i(icache_f0_sram_resp_dout),
    .icache_f0_sram_resp_valid_i(icache_f0_sram_resp_valid),
    .icache_f0_sram_resp_ready_o(new_icache_resp_ready),
    .voltage_monitor_vdd_o(new_voltage_vdd),
    .voltage_monitor_vss_o(new_voltage_vss),
    .clk_tbox_o(new_clk_tbox),
    .tbox_rst_c_no(new_tbox_rst_c_n),
    .tbox_rst_d_no(new_tbox_rst_d_n),
    .tbox_rst_w_no(new_tbox_rst_w_n),
    .dft_tbox_lv_o(new_dft_tbox_lv),
    .dft_scanin_to_tbox_lv_o(new_dft_scanin_to_tbox_lv),
    .dft_scanout_to_tbox_lv_i(dft_scanout_to_tbox_lv),
    .dft_occ_scanin_to_tbox_lv_o(new_dft_occ_scanin_to_tbox_lv),
    .dft_occ_scanout_to_tbox_lv_i(dft_occ_scanout_to_tbox_lv),
    .dft_scan_enable_to_tbox_lv_o(new_dft_scan_enable_to_tbox_lv),
    .dft_test_mode_to_tbox_lv_o(new_dft_test_mode_to_tbox_lv),
    .dft_clock_gate_en_to_tbox_lv_o(new_dft_clock_gate_en_to_tbox_lv),
    .dft_cntl_to_tbox_lv_o(new_dft_cntl_to_tbox_lv),
    .dft_occ_reset_to_tbox_lv_o(new_dft_occ_reset_to_tbox_lv),
    .dft_occ_testmode_to_tbox_lv_o(new_dft_occ_testmode_to_tbox_lv),
    .dft_scan_ate_clk_to_tbox_lv_o(new_dft_scan_ate_clk_to_tbox_lv),
    .dft_occ_bypass_to_tbox_lv_o(new_dft_occ_bypass_to_tbox_lv),
    .tbox_id_lo_o(new_tbox_id_lo),
    .tbox_control_esr_o(new_tbox_control_esr),
    .tbox_status_esr_i(tbox_status_esr),
    .tbox_image_table_ptr_esr_o(new_tbox_image_table_ptr_esr),
    .tbox_neigh_req_ready_o(new_tbox_neigh_req_ready),
    .tbox_neigh_req_valid_i(tbox_neigh_req_valid),
    .tbox_neigh_req_info_i(tbox_neigh_req_info),
    .tbox_neigh_rsp_ready_i(tbox_neigh_rsp_ready),
    .tbox_neigh_rsp_valid_o(new_tbox_neigh_rsp_valid),
    .tbox_neigh_rsp_info_o(new_tbox_neigh_rsp_info),
    .tbox_dbg_signals_en_o(new_tbox_dbg_signals_en),
    .tbox_dbg_signals_i(tbox_dbg_signals),
    .tbox_dbg_signals_mux_o(new_tbox_dbg_signals_mux),
    .tbox_apb_req_o(new_tbox_apb_req),
    .tbox_apb_rsp_i(tbox_apb_rsp),
    .tbox_bpam_run_control_o(new_tbox_bpam_run_control),
    .tbox_bpam_run_control_ack_i(tbox_bpam_ack),
    .coop_tload_slv_rdy_out_data_o(new_coop_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_o(new_coop_slv_rdy_out_valid),
    .coop_tload_slv_rdy_in_data_i(new_coop_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_i(coop_slv_rdy_in_valid),
    .coop_tload_mst_done_out_coop_id_o(new_coop_mst_done_out_coop_id),
    .coop_tload_mst_done_out_valid_o(new_coop_mst_done_out_valid),
    .coop_tload_mst_done_in_coop_id_i(coop_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_i(coop_mst_done_in_valid)
  );

  logic [OutBits-1:0] orig_flat;
  logic [OutBits-1:0] new_flat;
  always_comb begin
    orig_flat = '0;
    new_flat = '0;

    orig_flat[$bits({
      orig_reset_w_icache, orig_dft_scanout_hv, orig_dft_occ_scanout_hv,
      orig_eco_o, orig_pwr_ctrl_glb_nsleepout, orig_pwr_ctrl_min_nsleepout,
      orig_bpam_ack, orig_prefetch_done, orig_icache_err_detected,
      orig_icache_err_logged, orig_esr_tree, orig_dll_feedback_shire,
      orig_dll_feedback_neigh, orig_dll_sts, orig_neigh_sc_req_valid,
      orig_neigh_sc_req_info, orig_neigh_sc_rsp_ready, orig_neigh_sm_signals,
      orig_apb_rsp, orig_flb_req_valid, orig_flb_req_data, orig_icache_req_write,
      orig_icache_req_addr, orig_icache_req_valid, orig_icache_resp_ready,
      orig_voltage_vdd, orig_voltage_vss, orig_coop_slv_rdy_out_data,
      orig_coop_slv_rdy_out_valid, orig_coop_mst_done_out_coop_id,
      orig_coop_mst_done_out_valid, u_orig.tbox_clock, u_orig.tbox_reset_c,
      u_orig.tbox_reset_d, u_orig.tbox_reset_w, u_orig.tbox_id_lo,
      u_orig.tbox_control_esr, u_orig.tbox_image_table_ptr_esr,
      u_orig.tbox_neigh_req_ready_lo, u_orig.tbox_neigh_rsp_valid_lo,
      u_orig.tbox_neigh_rsp_info_lo, u_orig.tbox_dbg_signals_en,
      {minion_pkg::NeighDebugSmMuxWidth{1'b0}}, u_orig.tbox_APB_req,
      u_orig.tbox_bpam_run_control,
      {neigh_pkg::ShireDftScanoutTboxWidth{1'b0}}, 1'b0,
      {5{1'b0}}, {neigh_pkg::ShireDftScaninTboxWidth{1'b0}}, 1'b0,
      1'b0, {neigh_pkg::ShireDftModeWidth{1'b0}}, 1'b0,
      {neigh_pkg::ShireDftCntlWidth{1'b0}}, 1'b0, 1'b0, 1'b0, 1'b0
    })-1:0] = {
      orig_reset_w_icache, orig_dft_scanout_hv, orig_dft_occ_scanout_hv,
      orig_eco_o, orig_pwr_ctrl_glb_nsleepout, orig_pwr_ctrl_min_nsleepout,
      orig_bpam_ack, orig_prefetch_done, orig_icache_err_detected,
      orig_icache_err_logged, orig_esr_tree, orig_dll_feedback_shire,
      orig_dll_feedback_neigh, orig_dll_sts, orig_neigh_sc_req_valid,
      orig_neigh_sc_req_info, orig_neigh_sc_rsp_ready, orig_neigh_sm_signals,
      orig_apb_rsp, orig_flb_req_valid, orig_flb_req_data, orig_icache_req_write,
      orig_icache_req_addr, orig_icache_req_valid, orig_icache_resp_ready,
      orig_voltage_vdd, orig_voltage_vss, orig_coop_slv_rdy_out_data,
      orig_coop_slv_rdy_out_valid, orig_coop_mst_done_out_coop_id,
      orig_coop_mst_done_out_valid, u_orig.tbox_clock, u_orig.tbox_reset_c,
      u_orig.tbox_reset_d, u_orig.tbox_reset_w, u_orig.tbox_id_lo,
      u_orig.tbox_control_esr, u_orig.tbox_image_table_ptr_esr,
      u_orig.tbox_neigh_req_ready_lo, u_orig.tbox_neigh_rsp_valid_lo,
      u_orig.tbox_neigh_rsp_info_lo, u_orig.tbox_dbg_signals_en,
      {minion_pkg::NeighDebugSmMuxWidth{1'b0}}, u_orig.tbox_APB_req,
      u_orig.tbox_bpam_run_control,
      {neigh_pkg::ShireDftScanoutTboxWidth{1'b0}}, 1'b0,
      {5{1'b0}}, {neigh_pkg::ShireDftScaninTboxWidth{1'b0}}, 1'b0,
      1'b0, {neigh_pkg::ShireDftModeWidth{1'b0}}, 1'b0,
      {neigh_pkg::ShireDftCntlWidth{1'b0}}, 1'b0, 1'b0, 1'b0, 1'b0
    };

    new_flat[$bits({
      !new_rst_w_icache_n, new_dft_scanout_hv, new_dft_occ_scanout_hv,
      new_eco_o, new_pwr_ctrl_glb_nsleepout, new_pwr_ctrl_min_nsleepout,
      new_bpam_ack, new_prefetch_done, new_icache_err_detected,
      new_icache_err_logged, new_esr_tree, new_dll_feedback_shire,
      new_dll_feedback_neigh, new_dll_sts, new_neigh_sc_req_valid,
      new_neigh_sc_req_info, new_neigh_sc_rsp_ready, new_neigh_sm_signals,
      new_apb_rsp, new_flb_req_valid, new_flb_req_data, new_icache_req_write,
      new_icache_req_addr, new_icache_req_valid, new_icache_resp_ready,
      new_voltage_vdd, new_voltage_vss, new_coop_slv_rdy_out_data,
      new_coop_slv_rdy_out_valid, new_coop_mst_done_out_coop_id,
      new_coop_mst_done_out_valid, new_clk_tbox, !new_tbox_rst_c_n,
      !new_tbox_rst_d_n, !new_tbox_rst_w_n, new_tbox_id_lo,
      new_tbox_control_esr, new_tbox_image_table_ptr_esr,
      new_tbox_neigh_req_ready, new_tbox_neigh_rsp_valid,
      new_tbox_neigh_rsp_info, new_tbox_dbg_signals_en,
      new_tbox_dbg_signals_mux, new_tbox_apb_req,
      new_tbox_bpam_run_control,
      new_dft_scanout_tbox_hv, new_dft_occ_scanout_tbox_hv,
      new_dft_tbox_lv, new_dft_scanin_to_tbox_lv,
      new_dft_occ_scanin_to_tbox_lv, new_dft_scan_enable_to_tbox_lv,
      new_dft_test_mode_to_tbox_lv, new_dft_clock_gate_en_to_tbox_lv,
      new_dft_cntl_to_tbox_lv, new_dft_occ_reset_to_tbox_lv,
      new_dft_occ_testmode_to_tbox_lv, new_dft_scan_ate_clk_to_tbox_lv,
      new_dft_occ_bypass_to_tbox_lv
    })-1:0] = {
      !new_rst_w_icache_n, new_dft_scanout_hv, new_dft_occ_scanout_hv,
      new_eco_o, new_pwr_ctrl_glb_nsleepout, new_pwr_ctrl_min_nsleepout,
      new_bpam_ack, new_prefetch_done, new_icache_err_detected,
      new_icache_err_logged, new_esr_tree, new_dll_feedback_shire,
      new_dll_feedback_neigh, new_dll_sts, new_neigh_sc_req_valid,
      new_neigh_sc_req_info, new_neigh_sc_rsp_ready, new_neigh_sm_signals,
      new_apb_rsp, new_flb_req_valid, new_flb_req_data, new_icache_req_write,
      new_icache_req_addr, new_icache_req_valid, new_icache_resp_ready,
      new_voltage_vdd, new_voltage_vss, new_coop_slv_rdy_out_data,
      new_coop_slv_rdy_out_valid, new_coop_mst_done_out_coop_id,
      new_coop_mst_done_out_valid, new_clk_tbox, !new_tbox_rst_c_n,
      !new_tbox_rst_d_n, !new_tbox_rst_w_n, new_tbox_id_lo,
      new_tbox_control_esr, new_tbox_image_table_ptr_esr,
      new_tbox_neigh_req_ready, new_tbox_neigh_rsp_valid,
      new_tbox_neigh_rsp_info, new_tbox_dbg_signals_en,
      new_tbox_dbg_signals_mux, new_tbox_apb_req,
      new_tbox_bpam_run_control,
      new_dft_scanout_tbox_hv, new_dft_occ_scanout_tbox_hv,
      new_dft_tbox_lv, new_dft_scanin_to_tbox_lv,
      new_dft_occ_scanin_to_tbox_lv, new_dft_scan_enable_to_tbox_lv,
      new_dft_test_mode_to_tbox_lv, new_dft_clock_gate_en_to_tbox_lv,
      new_dft_cntl_to_tbox_lv, new_dft_occ_reset_to_tbox_lv,
      new_dft_occ_testmode_to_tbox_lv, new_dft_scan_ate_clk_to_tbox_lv,
      new_dft_occ_bypass_to_tbox_lv
    };
  end

  for (genvar chunk = 0; chunk < OutChunks; chunk++) begin : gen_chunks
    assign orig_chunks_o[chunk] = orig_flat[chunk*64 +: 64];
    assign new_chunks_o[chunk] = new_flat[chunk*64 +: 64];
  end

  assign input_toggle_ok_o = (cycle_q > (ResetExerciseCycles + 32'd128)) &&
                             (&drive_seen_zero_q[DriveMaxSalt:DriveMinSalt]) &&
                             (&drive_seen_one_q[DriveMaxSalt:DriveMinSalt]);

endmodule : cosim_neigh_top_tb
/* verilator lint_on ENUMVALUE */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNUSEDSIGNAL */
