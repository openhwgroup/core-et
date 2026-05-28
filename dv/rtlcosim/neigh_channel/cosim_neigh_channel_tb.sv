// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */  // Integration cosim intentionally drives and observes the full channel surface through aggregate chunks.
/* verilator lint_off WIDTHEXPAND */  // Packed original vectors and translated structs are width-equivalent by construction.
/* verilator lint_off WIDTHTRUNC */  // Deterministic stimulus truncates the cycle/LFSR pattern to each port width.
/* verilator lint_off ENUMVALUE */  // Randomized protocol stress can place reserved enum encodings on inputs; both DUTs see identical bits.
module cosim_neigh_channel_tb
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
  output logic [63:0] orig_chunks_o [512],
  output logic [63:0] new_chunks_o [512],
  output logic input_toggle_ok_o
);

  localparam int unsigned OutChunks = 512;
  // Exercise reset/DFT control inputs before the compared window; the translated
  // RTL uses active-low async resets while the original uses active-high sync resets.
  localparam int unsigned ResetExerciseCycles = 2048;
  localparam int unsigned DriveMinSalt = 8;
  localparam int unsigned DriveMaxSalt = 182;
  localparam int unsigned DriveSaltCount = DriveMaxSalt + 1;

  logic clk_shire_i;
  logic new_clk_minion_o;
  logic rst_c_shire_ni;
  logic rst_d_shire_ni;
  logic rst_w_shire_ni;
  logic rst_warm_ni;
  logic new_rst_w_icache_no;
  dft_t dft_hv_i;
  logic dft_use_reset_cntl_hv_i;
  neigh_pkg::shire_tdr_reset_cntl_t dft_reset_cntl_hv_i;
  dft_t new_dft_lv_o;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t chicken_bits_vc_i;
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control_i;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_bpam_rc_tbox_ack_hi_o;
  logic new_dll_feedback_shire_o;
  logic new_dll_feedback_neigh_o;
  logic [minion_pkg::NumShireIdsBits-1:0] shire_id_i;
  logic [neigh_pkg::NumNeighWidth-1:0] neigh_id_i;
  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] shire_tbox_id_i;
  logic shire_tbox_en_i;
  logic [neigh_pkg::MinPerN-1:0] esr_thread0_enable_i;
  logic [neigh_pkg::MinPerN-1:0] esr_thread1_enable_i;
  minion_pkg::esr_minion_features_t esr_minion_features_i;
  icache_pkg::icache_prefetch_conf_t esr_icache_prefetch_conf_i;
  logic esr_icache_prefetch_start_i;
  logic new_esr_icache_prefetch_done_o;
  logic new_esr_icache_err_detected_o;
  logic new_esr_icache_err_logged_o;
  esr_pkg::esr_ms_dmctrl_t dmctrl_i;
  esr_pkg::esr_and_or_tree_l0_t new_esr_and_or_tree_l0_o;
  logic esr_shire_coop_mode_i;
  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_i;
  esr_pkg::esr_dll_dly_est_ctl_t esr_dll_dly_est_ctl_i;
  esr_pkg::esr_dll_dly_est_sts_t new_esr_dll_dly_est_sts_o;
  logic [shirecache_pkg::Banks:0] neigh_sc_req_ready_i;
  logic [shirecache_pkg::Banks:0] new_neigh_sc_req_valid_o;
  etlink_pkg::req_t new_neigh_sc_req_info_o;
  logic new_neigh_sc_rsp_ready_o;
  logic neigh_sc_rsp_valid_i;
  etlink_pkg::rsp_t neigh_sc_rsp_info_i;
  logic hw_dbg_sm_monitor_enabled_i;
  logic [neigh_voltage_cross_pkg::NeighSmGpioWidth-1:0] neigh_sm_gpio_i;
  minion_pkg::neigh_sm_dbg_monitor_t new_neigh_sm_signals_o;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_req_from_shire_i;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t new_apb_rsp_to_shire_o;
  logic [neigh_pkg::MinPerN-1:0] int_mtip_i;
  logic int_meip_i;
  logic int_seip_i;
  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] ipi_msip_i;
  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] ipi_redirect_trigger_i;
  logic [neigh_pkg::MinPerN-1:0] uc_to_neigh_fcc_i;
  logic [neigh_voltage_cross_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target_i;
  logic new_flb_neigh_l2_req_valid_o;
  logic [neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] new_flb_neigh_l2_req_data_o;
  logic flb_l2_neigh_resp_valid_i;
  logic flb_l2_neigh_resp_data_i;
  logic new_icache_f2_sram_req_write_o;
  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] new_icache_f2_sram_req_addr_o;
  logic new_icache_f2_sram_req_valid_o;
  logic icache_f2_sram_req_ready_i;
  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] icache_f0_sram_resp_dout_i;
  logic icache_f0_sram_resp_valid_i;
  logic new_icache_f0_sram_resp_ready_o;
  neigh_pkg::coop_tload_slv_rdy_req_t new_coop_tload_slv_rdy_out_data_o;
  logic new_coop_tload_slv_rdy_out_valid_o;
  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_data_i;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_valid_i;
  logic [neigh_pkg::ShireCoopIdSize-1:0] new_coop_tload_mst_done_out_coop_id_o;
  logic [neigh_pkg::NumOtherNeigh-1:0] new_coop_tload_mst_done_out_valid_o;
  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_i;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_mst_done_in_valid_i;
  logic [neigh_pkg::MinPerN-1:0] new_rst_c_per_minion_no;
  logic [neigh_pkg::MinPerN-1:0] new_rst_d_per_minion_no;
  logic [neigh_pkg::MinPerN-1:0] new_rst_w_per_minion_no;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t new_chicken_bits_vc_lo_o;
  logic [minion_pkg::NumShireIdsBits-1:0] new_shire_id_lo_o;
  logic [neigh_pkg::NumNeighWidth-1:0] new_neigh_id_lo_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::MinPerSBits-1:0] new_shire_min_id_o;
  logic [neigh_pkg::MinPerN-1:0] new_thread0_enable_o;
  logic [neigh_pkg::MinPerN-1:0] new_thread1_enable_o;
  minion_pkg::esr_minion_features_t new_esr_minion_features_lo_o;
  logic new_esr_shire_coop_mode_lo_o;
  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] new_esr_minion_mem_override_lo_o;
  logic new_esr_bypass_dcache_o;
  logic [minion_pkg::VaSize-1:0] new_minion_boot_addr_o;
  esr_pkg::esr_mprot_t new_mprot_o;
  minion_pkg::tlb_entry_type new_vmspagesize_o;
  minion_pkg::minion_interrupts [neigh_pkg::MinPerN-1:0] new_interrupts_per_minion_o;
  logic [neigh_pkg::MinPerN-1:0] flb_core_neigh_req_valid_i;
  logic [neigh_pkg::MinPerN-1:0][neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] flb_core_neigh_req_data_i;
  logic [neigh_pkg::MinPerN-1:0] new_flb_neigh_core_resp_valid_o;
  logic new_flb_neigh_core_resp_data_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] new_l2_dcache_evict_req_ready_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_valid_i;
  minion_dcache_pkg::et_link_minion_evict_req_info_t [neigh_pkg::MinPerN-1:0] l2_dcache_evict_req_i;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] new_l2_dcache_miss_req_ready_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] l2_dcache_miss_req_valid_i;
  minion_dcache_pkg::et_link_minion_miss_req_info_t [neigh_pkg::MinPerN-1:0] l2_dcache_miss_req_i;
  logic [neigh_pkg::MinPerN-1:0] l2_dcache_resp_ready_i;
  logic [neigh_pkg::MinPerN-1:0] new_l2_dcache_resp_valid_o;
  minion_pkg::et_link_minion_rsp_info_t [neigh_pkg::MinPerN-1:0] new_l2_dcache_resp_o;
  logic [neigh_pkg::MinPerN-1:0] new_icache_req_ready_o;
  logic [neigh_pkg::MinPerN-1:0] icache_req_valid_i;
  icache_pkg::frontend_icache_req_t [neigh_pkg::MinPerN-1:0] icache_req_i;
  logic [icache_pkg::IcacheNrReqs-1:0] new_icache_resp_valid_o;
  logic [icache_pkg::IcacheNrReqs-1:0] new_icache_resp_miss_o;
  icache_pkg::icache_frontend_resp_t [icache_pkg::IcacheNrReqs-1:0] new_icache_resp_o;
  logic [icache_pkg::IcacheNrReqs-1:0] new_icache_fill_done_o;
  logic [neigh_pkg::MinPerN-1:0] icache_flush_data_i;
  minion_pkg::minion_satp_info [neigh_pkg::MinPerN-1:0] satp_info_i;
  minion_pkg::minion_satp_info [neigh_pkg::MinPerN-1:0] matp_info_i;
  logic [neigh_pkg::MinPerN-1:0] tlb_invalidate_i;
  minion_pkg::minion_ptw_req [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dc_ptw_req_data_i;
  logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dc_ptw_req_valid_i;
  logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] new_dc_ptw_req_ready_o;
  minion_pkg::minion_ptw_pte [neigh_pkg::PtwPerN-1:0] new_ptw_dc_resp_data_o;
  logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] new_ptw_dc_resp_valid_o;
  logic [neigh_pkg::MinPerN-1:0][esr_pkg::EsrApbDataWidth-1:0] minion_apb_prdata_i;
  logic [neigh_pkg::MinPerN-1:0] minion_apb_pready_i;
  logic [neigh_pkg::MinPerN-1:0] minion_apb_pslverr_i;
  logic new_minion_apb_penable_o;
  logic [neigh_pkg::MinPerN-1:0] new_minion_apb_psel_o;
  logic new_minion_apb_pwrite_o;
  logic [minion_pkg::MinDmApbAddrWidth-1:0] new_minion_apb_paddr_o;
  logic [esr_pkg::EsrApbDataWidth-1:0] new_minion_apb_pwdata_o;
  minion_pkg::minion_debug_in_t [neigh_pkg::MinPerN-1:0] new_debug_minion_in_o;
  minion_pkg::minion_debug_out_t [neigh_pkg::MinPerN-1:0] debug_minion_out_i;
  logic [neigh_pkg::MinPerN-1:0] new_minion_dbg_signals_en_o;
  minion_pkg::neigh_sm_dbg_monitor_t [neigh_pkg::MinPerN-1:0] minion_dbg_signals_i;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::NeighDebugSmMuxWidth-1:0] new_minion_dbg_signals_mux_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuMinionCountersNum-1:0] pmu_count_up_i;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::XregSize-1:0] new_pmu_read_data_o;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::PmuCountersSelectBits-1:0] pmu_read_sel_i;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuTotalCountersNum-1:0] pmu_write_en_i;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::XregSize-1:0] pmu_write_data_i;
  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuNeighEventCntSelBits-1:0] pmu_neigh_event_sel_i;
  logic new_clk_tbox_o;
  logic new_tbox_rst_c_no;
  logic new_tbox_rst_d_no;
  logic new_tbox_rst_w_no;
  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] new_tbox_id_lo_o;
  logic [15:0] new_tbox_control_esr_o;
  logic [15:0] tbox_status_esr_i;
  logic [minion_pkg::VaSize-1:0] new_tbox_image_table_ptr_esr_o;
  logic new_tbox_neigh_req_ready_o;
  logic tbox_neigh_req_valid_i;
  etlink_pkg::neigh_req_t tbox_neigh_req_info_i;
  logic tbox_neigh_rsp_ready_i;
  logic new_tbox_neigh_rsp_valid_o;
  etlink_pkg::rsp_t new_tbox_neigh_rsp_info_o;
  logic new_tbox_dbg_signals_en_o;
  minion_pkg::neigh_sm_dbg_monitor_t tbox_dbg_signals_i;
  logic [minion_pkg::NeighDebugSmMuxWidth-1:0] new_tbox_dbg_signals_mux_o;
  neigh_ch_apb_mux_pkg::apb_to_neigh_tbox_t new_tbox_apb_req_o;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t tbox_apb_rsp_i;
  neigh_ch_dbg_pkg::tbox_bpam_run_control_t new_tbox_bpam_run_control_o;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t tbox_bpam_run_control_ack_i;

  logic [$bits(chicken_bits_vc_i)-1:0] chicken_bits_vc_i_flat;
  assign chicken_bits_vc_i_flat = chicken_bits_vc_i;
  logic [$bits(esr_minion_features_i)-1:0] esr_minion_features_i_flat;
  assign esr_minion_features_i_flat = esr_minion_features_i;
  logic [$bits(esr_icache_prefetch_conf_i)-1:0] esr_icache_prefetch_conf_i_flat;
  assign esr_icache_prefetch_conf_i_flat = esr_icache_prefetch_conf_i;
  logic [$bits(dmctrl_i)-1:0] dmctrl_i_flat;
  assign dmctrl_i_flat = dmctrl_i;
  logic [$bits(esr_dll_dly_est_ctl_i)-1:0] esr_dll_dly_est_ctl_i_flat;
  assign esr_dll_dly_est_ctl_i_flat = esr_dll_dly_est_ctl_i;
  logic [$bits(neigh_sc_rsp_info_i)-1:0] neigh_sc_rsp_info_i_flat;
  assign neigh_sc_rsp_info_i_flat = neigh_sc_rsp_info_i;
  logic [$bits(apb_req_from_shire_i)-1:0] apb_req_from_shire_i_flat;
  assign apb_req_from_shire_i_flat = apb_req_from_shire_i;
  logic [$bits(l2_dcache_evict_req_i)-1:0] l2_dcache_evict_req_i_flat;
  assign l2_dcache_evict_req_i_flat = l2_dcache_evict_req_i;
  logic [$bits(l2_dcache_miss_req_i)-1:0] l2_dcache_miss_req_i_flat;
  assign l2_dcache_miss_req_i_flat = l2_dcache_miss_req_i;
  logic [$bits(icache_req_i)-1:0] icache_req_i_flat;
  assign icache_req_i_flat = icache_req_i;
  logic [$bits(dc_ptw_req_data_i)-1:0] dc_ptw_req_data_i_flat;
  assign dc_ptw_req_data_i_flat = dc_ptw_req_data_i;
  logic [$bits(debug_minion_out_i)-1:0] debug_minion_out_i_flat;
  assign debug_minion_out_i_flat = debug_minion_out_i;
  logic [$bits(minion_dbg_signals_i)-1:0] minion_dbg_signals_i_flat;
  assign minion_dbg_signals_i_flat = minion_dbg_signals_i;
  logic [$bits(tbox_neigh_req_info_i)-1:0] tbox_neigh_req_info_i_flat;
  assign tbox_neigh_req_info_i_flat = tbox_neigh_req_info_i;
  logic [$bits(tbox_apb_rsp_i)-1:0] tbox_apb_rsp_i_flat;
  assign tbox_apb_rsp_i_flat = tbox_apb_rsp_i;
  logic [$bits(tbox_bpam_run_control_ack_i)-1:0] tbox_bpam_run_control_ack_i_flat;
  assign tbox_bpam_run_control_ack_i_flat = tbox_bpam_run_control_ack_i;
  logic [$bits(new_apb_rsp_to_shire_o)-1:0] new_apb_rsp_to_shire_o_flat;
  assign new_apb_rsp_to_shire_o_flat = new_apb_rsp_to_shire_o;
  logic [$bits(new_bpam_rc_tbox_ack_hi_o)-1:0] new_bpam_rc_tbox_ack_hi_o_flat;
  assign new_bpam_rc_tbox_ack_hi_o_flat = new_bpam_rc_tbox_ack_hi_o;
  logic [$bits(new_chicken_bits_vc_lo_o)-1:0] new_chicken_bits_vc_lo_o_flat;
  assign new_chicken_bits_vc_lo_o_flat = new_chicken_bits_vc_lo_o;
  logic [$bits(new_coop_tload_slv_rdy_out_data_o)-1:0] new_coop_tload_slv_rdy_out_data_o_flat;
  assign new_coop_tload_slv_rdy_out_data_o_flat = new_coop_tload_slv_rdy_out_data_o;
  logic [$bits(new_debug_minion_in_o)-1:0] new_debug_minion_in_o_flat;
  assign new_debug_minion_in_o_flat = new_debug_minion_in_o;
  logic [$bits(new_esr_and_or_tree_l0_o)-1:0] new_esr_and_or_tree_l0_o_flat;
  assign new_esr_and_or_tree_l0_o_flat = new_esr_and_or_tree_l0_o;
  logic [$bits(new_esr_dll_dly_est_sts_o)-1:0] new_esr_dll_dly_est_sts_o_flat;
  assign new_esr_dll_dly_est_sts_o_flat = new_esr_dll_dly_est_sts_o;
  logic [$bits(new_esr_minion_features_lo_o)-1:0] new_esr_minion_features_lo_o_flat;
  assign new_esr_minion_features_lo_o_flat = new_esr_minion_features_lo_o;
  logic [$bits(new_icache_resp_o)-1:0] new_icache_resp_o_flat;
  assign new_icache_resp_o_flat = new_icache_resp_o;
  logic [$bits(new_interrupts_per_minion_o)-1:0] new_interrupts_per_minion_o_flat;
  assign new_interrupts_per_minion_o_flat = new_interrupts_per_minion_o;
  logic [$bits(new_l2_dcache_resp_o)-1:0] new_l2_dcache_resp_o_flat;
  assign new_l2_dcache_resp_o_flat = new_l2_dcache_resp_o;
  logic [$bits(new_mprot_o)-1:0] new_mprot_o_flat;
  assign new_mprot_o_flat = new_mprot_o;
  logic [$bits(new_neigh_sc_req_info_o)-1:0] new_neigh_sc_req_info_o_flat;
  assign new_neigh_sc_req_info_o_flat = new_neigh_sc_req_info_o;
  logic [$bits(new_neigh_sm_signals_o)-1:0] new_neigh_sm_signals_o_flat;
  assign new_neigh_sm_signals_o_flat = new_neigh_sm_signals_o;
  logic [$bits(new_ptw_dc_resp_data_o)-1:0] new_ptw_dc_resp_data_o_flat;
  assign new_ptw_dc_resp_data_o_flat = new_ptw_dc_resp_data_o;
  logic [$bits(new_tbox_apb_req_o)-1:0] new_tbox_apb_req_o_flat;
  assign new_tbox_apb_req_o_flat = new_tbox_apb_req_o;
  logic [$bits(new_tbox_bpam_run_control_o)-1:0] new_tbox_bpam_run_control_o_flat;
  assign new_tbox_bpam_run_control_o_flat = new_tbox_bpam_run_control_o;
  logic [$bits(new_tbox_neigh_rsp_info_o)-1:0] new_tbox_neigh_rsp_info_o_flat;
  assign new_tbox_neigh_rsp_info_o_flat = new_tbox_neigh_rsp_info_o;
  logic [$bits(new_vmspagesize_o)-1:0] new_vmspagesize_o_flat;
  assign new_vmspagesize_o_flat = new_vmspagesize_o;

  logic [$bits(new_clk_minion_o)-1:0] orig_clock_minion;
  logic [$bits(!new_rst_w_icache_no)-1:0] orig_reset_w_icache;
  logic [$bits(new_bpam_rc_tbox_ack_hi_o_flat)-1:0] orig_bpam_rc_tbox_ack_hi;
  logic [$bits(new_dll_feedback_shire_o)-1:0] orig_dll_feedback_shire;
  logic [$bits(new_dll_feedback_neigh_o)-1:0] orig_dll_feedback_neigh;
  logic [$bits(new_esr_icache_prefetch_done_o)-1:0] orig_esr_icache_prefetch_done;
  logic [$bits(new_esr_icache_err_detected_o)-1:0] orig_esr_icache_err_detected_op;
  logic [$bits(new_esr_icache_err_logged_o)-1:0] orig_esr_icache_err_logged_op;
  logic [$bits(new_esr_and_or_tree_l0_o_flat)-1:0] orig_esr_and_or_tree_L0_op;
  logic [$bits(new_esr_dll_dly_est_sts_o_flat)-1:0] orig_esr_dll_dly_est_sts;
  logic [$bits(new_neigh_sc_req_valid_o)-1:0] orig_neigh_sc_req_valid;
  logic [$bits(new_neigh_sc_req_info_o_flat)-1:0] orig_neigh_sc_req_info;
  logic [$bits(new_neigh_sc_rsp_ready_o)-1:0] orig_neigh_sc_rsp_ready;
  logic [$bits(new_neigh_sm_signals_o_flat)-1:0] orig_neigh_sm_signals;
  logic [$bits(new_apb_rsp_to_shire_o_flat)-1:0] orig_APB_rsp_to_shire;
  logic [$bits(new_flb_neigh_l2_req_valid_o)-1:0] orig_flb_neigh_l2_req_valid;
  logic [$bits(new_flb_neigh_l2_req_data_o)-1:0] orig_flb_neigh_l2_req_data;
  logic [$bits(new_icache_f2_sram_req_write_o)-1:0] orig_icache_f2_sram_req_write;
  logic [$bits(new_icache_f2_sram_req_addr_o)-1:0] orig_icache_f2_sram_req_addr;
  logic [$bits(new_icache_f2_sram_req_valid_o)-1:0] orig_icache_f2_sram_req_valid;
  logic [$bits(new_icache_f0_sram_resp_ready_o)-1:0] orig_icache_f0_sram_resp_ready;
  logic [$bits(new_coop_tload_slv_rdy_out_data_o_flat)-1:0] orig_coop_tload_slv_rdy_out_data;
  logic [$bits(new_coop_tload_slv_rdy_out_valid_o)-1:0] orig_coop_tload_slv_rdy_out_valid;
  logic [$bits(new_coop_tload_mst_done_out_coop_id_o)-1:0] orig_coop_tload_mst_done_out_coop_id;
  logic [$bits(new_coop_tload_mst_done_out_valid_o)-1:0] orig_coop_tload_mst_done_out_valid;
  logic [$bits(~new_rst_c_per_minion_no)-1:0] orig_reset_c_per_minion;
  logic [$bits(~new_rst_d_per_minion_no)-1:0] orig_reset_d_per_minion;
  logic [$bits(~new_rst_w_per_minion_no)-1:0] orig_reset_w_per_minion;
  logic [$bits(new_dft_lv_o.scanmode)-1:0] orig_dft__reset_byp_lv;
  logic [$bits(!new_dft_lv_o.scan_reset_n)-1:0] orig_dft__reset_lv;
  // The original LV DFT output surface exposes only reset bypass/reset. The
  // translated dft_t also carries SRAM/RAM DFT controls, which are documented
  // passthroughs from the HV DFT input and must be compared every cycle.
  logic [$bits(new_dft_lv_o.sram_clk_override)-1:0] exp_dft_lv_sram_clk_override;
  logic [$bits(new_dft_lv_o.ram_rei)-1:0] exp_dft_lv_ram_rei;
  logic [$bits(new_dft_lv_o.ram_wei)-1:0] exp_dft_lv_ram_wei;
  assign exp_dft_lv_sram_clk_override = dft_hv_i.sram_clk_override;
  assign exp_dft_lv_ram_rei = dft_hv_i.ram_rei;
  assign exp_dft_lv_ram_wei = dft_hv_i.ram_wei;
  logic [$bits(new_chicken_bits_vc_lo_o_flat)-1:0] orig_chicken_bits_vc_lo;
  logic [$bits(new_shire_id_lo_o)-1:0] orig_shire_id_lo;
  logic [$bits(new_neigh_id_lo_o)-1:0] orig_neigh_id_lo;
  logic [$bits(new_shire_min_id_o)-1:0] orig_shire_min_id;
  logic [$bits(new_thread0_enable_o)-1:0] orig_thread0_enable;
  logic [$bits(new_thread1_enable_o)-1:0] orig_thread1_enable;
  logic [$bits(new_esr_minion_features_lo_o_flat)-1:0] orig_esr_minion_features_lo;
  logic [$bits(new_esr_shire_coop_mode_lo_o)-1:0] orig_esr_shire_coop_mode_lo;
  logic [$bits(new_esr_minion_mem_override_lo_o)-1:0] orig_esr_minion_mem_override_lo;
  logic [$bits(new_esr_bypass_dcache_o)-1:0] orig_esr_bypass_dcache;
  logic [$bits(new_minion_boot_addr_o)-1:0] orig_minion_boot_addr;
  logic [$bits(new_mprot_o_flat)-1:0] orig_mprot;
  logic [$bits(new_vmspagesize_o_flat)-1:0] orig_vmspagesize;
  logic [$bits(new_interrupts_per_minion_o_flat)-1:0] orig_interrupts_per_minion;
  logic [$bits(new_flb_neigh_core_resp_valid_o)-1:0] orig_flb_neigh_core_resp_valid;
  logic [$bits(new_flb_neigh_core_resp_data_o)-1:0] orig_flb_neigh_core_resp_data;
  logic [$bits(new_l2_dcache_evict_req_ready_o)-1:0] orig_l2_dcache_evict_req_ready;
  logic [$bits(new_l2_dcache_miss_req_ready_o)-1:0] orig_l2_dcache_miss_req_ready;
  logic [$bits(new_l2_dcache_resp_valid_o)-1:0] orig_l2_dcache_resp_valid;
  logic [$bits(new_l2_dcache_resp_o_flat)-1:0] orig_l2_dcache_resp;
  logic [$bits(new_icache_req_ready_o)-1:0] orig_icache_req_ready;
  logic [$bits(new_icache_resp_valid_o)-1:0] orig_icache_resp_valid;
  logic [$bits(new_icache_resp_miss_o)-1:0] orig_icache_resp_miss;
  logic [$bits(new_icache_resp_o_flat)-1:0] orig_icache_resp;
  logic [$bits(new_icache_fill_done_o)-1:0] orig_icache_fill_done;
  logic [$bits(new_dc_ptw_req_ready_o)-1:0] orig_dc_ptw_req_ready;
  logic [$bits(new_ptw_dc_resp_data_o_flat)-1:0] orig_ptw_dc_resp_data;
  logic [$bits(new_ptw_dc_resp_valid_o)-1:0] orig_ptw_dc_resp_valid;
  logic [$bits(new_minion_apb_penable_o)-1:0] orig_minion_apb_penable;
  logic [$bits(new_minion_apb_psel_o)-1:0] orig_minion_apb_psel;
  logic [$bits(new_minion_apb_pwrite_o)-1:0] orig_minion_apb_pwrite;
  logic [$bits(new_minion_apb_paddr_o)-1:0] orig_minion_apb_paddr;
  logic [$bits(new_minion_apb_pwdata_o)-1:0] orig_minion_apb_pwdata;
  logic [$bits(new_debug_minion_in_o_flat)-1:0] orig_debug_minion_in;
  logic [$bits(new_minion_dbg_signals_en_o)-1:0] orig_minion_dbg_signals_en;
  logic [$bits(new_minion_dbg_signals_mux_o)-1:0] orig_minion_dbg_signals_mux;
  logic [$bits(new_pmu_read_data_o)-1:0] orig_pmu_read_data;
  logic [$bits(new_clk_tbox_o)-1:0] orig_clock_tbox;
  logic [$bits(!new_tbox_rst_c_no)-1:0] orig_tbox_reset_c;
  logic [$bits(!new_tbox_rst_d_no)-1:0] orig_tbox_reset_d;
  logic [$bits(!new_tbox_rst_w_no)-1:0] orig_tbox_reset_w;
  logic [$bits(new_tbox_id_lo_o)-1:0] orig_tbox_id_lo;
  logic [$bits(new_tbox_control_esr_o)-1:0] orig_tbox_control_esr;
  logic [$bits(new_tbox_image_table_ptr_esr_o)-1:0] orig_tbox_image_table_ptr_esr;
  logic [$bits(new_tbox_neigh_req_ready_o)-1:0] orig_tbox_neigh_req_ready;
  logic [$bits(new_tbox_neigh_rsp_valid_o)-1:0] orig_tbox_neigh_rsp_valid;
  logic [$bits(new_tbox_neigh_rsp_info_o_flat)-1:0] orig_tbox_neigh_rsp_info;
  logic [$bits(new_tbox_dbg_signals_en_o)-1:0] orig_tbox_dbg_signals_en;
  logic [$bits(new_tbox_dbg_signals_mux_o)-1:0] orig_tbox_dbg_signals_mux;
  logic [$bits(new_tbox_apb_req_o_flat)-1:0] orig_tbox_APB_req;
  logic [$bits(new_tbox_bpam_run_control_o_flat)-1:0] orig_tbox_bpam_run_control;

  logic [31:0] cycle_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) cycle_q <= '0;
    else         cycle_q <= cycle_q + 32'd1;
  end

  function automatic logic drive_bit(input int unsigned salt);
    logic [31:0] x;
    x = cycle_q ^ (32'h9e3779b9 * (salt + 1));
    drive_bit = x[0] ^ x[3] ^ x[7] ^ x[11] ^ x[16];
  endfunction

  logic reset_exercise_phase;
  logic [DriveSaltCount-1:0] drive_seen_zero_q;
  logic [DriveSaltCount-1:0] drive_seen_one_q;

  assign reset_exercise_phase = (cycle_q < ResetExerciseCycles);

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      drive_seen_zero_q <= '0;
      drive_seen_one_q  <= '0;
    end else begin
      for (int unsigned salt = DriveMinSalt; salt <= DriveMaxSalt; salt++) begin
        if (drive_bit(salt)) begin
          drive_seen_one_q[salt] <= 1'b1;
        end else begin
          drive_seen_zero_q[salt] <= 1'b1;
        end
      end
    end
  end

  always_comb begin
    clk_shire_i = clk_i;
    rst_c_shire_ni = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd120));
    rst_d_shire_ni = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd180));
    rst_w_shire_ni = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd240));
    rst_warm_ni = rst_ni && !(reset_exercise_phase && (cycle_q[8:0] == 9'd300));
    dft_hv_i.scanmode = reset_exercise_phase && (cycle_q[9:0] >= 10'd64) && (cycle_q[9:0] < 10'd72);
    dft_hv_i.scan_reset_n = !(reset_exercise_phase && (cycle_q[9:0] >= 10'd66) && (cycle_q[9:0] < 10'd70));
    dft_hv_i.sram_clk_override = drive_bit(8);
    dft_hv_i.ram_rei = drive_bit(9);
    dft_hv_i.ram_wei = drive_bit(10);
    dft_use_reset_cntl_hv_i = reset_exercise_phase ? (drive_bit(112) ? '1 : '0) : 1'b0;
    dft_reset_cntl_hv_i = reset_exercise_phase ? (drive_bit(113) ? '1 : '0) : '0;
    chicken_bits_vc_i = drive_bit(114) ? '1 : '0;
    bpam_run_control_i = drive_bit(115) ? '1 : '0;
    if (!reset_exercise_phase) begin
      bpam_run_control_i.gpio.ndmreset = 1'b0;
    end
    shire_id_i = drive_bit(116) ? '1 : '0;
    neigh_id_i = drive_bit(117) ? '1 : '0;
    shire_tbox_id_i = drive_bit(118) ? '1 : '0;
    shire_tbox_en_i = drive_bit(119) ? '1 : '0;
    esr_thread0_enable_i = drive_bit(120) ? '1 : '0;
    esr_thread1_enable_i = drive_bit(121) ? '1 : '0;
    esr_minion_features_i = drive_bit(122) ? '1 : '0;
    esr_icache_prefetch_conf_i = drive_bit(123) ? '1 : '0;
    esr_icache_prefetch_start_i = drive_bit(124) ? '1 : '0;
    dmctrl_i = drive_bit(125) ? '1 : '0;
    if (!reset_exercise_phase) begin
      dmctrl_i.ndmreset = 1'b0;
      dmctrl_i.dmactive = 1'b1;
    end
    esr_shire_coop_mode_i = drive_bit(126) ? '1 : '0;
    esr_minion_mem_override_i = drive_bit(127) ? '1 : '0;
    esr_dll_dly_est_ctl_i = drive_bit(128) ? '1 : '0;
    neigh_sc_req_ready_i = drive_bit(129) ? '1 : '0;
    neigh_sc_rsp_valid_i = drive_bit(130) ? '1 : '0;
    neigh_sc_rsp_info_i = drive_bit(131) ? '1 : '0;
    hw_dbg_sm_monitor_enabled_i = drive_bit(132) ? '1 : '0;
    neigh_sm_gpio_i = drive_bit(133) ? '1 : '0;
    apb_req_from_shire_i = drive_bit(134) ? '1 : '0;
    int_mtip_i = drive_bit(135) ? '1 : '0;
    int_meip_i = drive_bit(136) ? '1 : '0;
    int_seip_i = drive_bit(137) ? '1 : '0;
    ipi_msip_i = drive_bit(138) ? '1 : '0;
    ipi_redirect_trigger_i = drive_bit(139) ? '1 : '0;
    uc_to_neigh_fcc_i = drive_bit(140) ? '1 : '0;
    uc_to_neigh_fcc_target_i = drive_bit(141) ? '1 : '0;
    flb_l2_neigh_resp_valid_i = drive_bit(142) ? '1 : '0;
    flb_l2_neigh_resp_data_i = drive_bit(143) ? '1 : '0;
    icache_f2_sram_req_ready_i = drive_bit(144) ? '1 : '0;
    icache_f0_sram_resp_dout_i = drive_bit(145) ? '1 : '0;
    icache_f0_sram_resp_valid_i = drive_bit(146) ? '1 : '0;
    coop_tload_slv_rdy_in_data_i = drive_bit(147) ? '1 : '0;
    coop_tload_slv_rdy_in_valid_i = drive_bit(148) ? '1 : '0;
    coop_tload_mst_done_in_coop_id_i = drive_bit(149) ? '1 : '0;
    coop_tload_mst_done_in_valid_i = drive_bit(150) ? '1 : '0;
    flb_core_neigh_req_valid_i = drive_bit(151) ? '1 : '0;
    flb_core_neigh_req_data_i = drive_bit(152) ? '1 : '0;
    l2_dcache_evict_req_valid_i = drive_bit(153) ? '1 : '0;
    l2_dcache_evict_req_i = drive_bit(154) ? '1 : '0;
    l2_dcache_miss_req_valid_i = drive_bit(155) ? '1 : '0;
    l2_dcache_miss_req_i = drive_bit(156) ? '1 : '0;
    l2_dcache_resp_ready_i = drive_bit(157) ? '1 : '0;
    icache_req_valid_i = drive_bit(158) ? '1 : '0;
    icache_req_i = drive_bit(159) ? '1 : '0;
    icache_flush_data_i = drive_bit(160) ? '1 : '0;
    satp_info_i = drive_bit(161) ? '1 : '0;
    matp_info_i = drive_bit(162) ? '1 : '0;
    tlb_invalidate_i = drive_bit(163) ? '1 : '0;
    dc_ptw_req_data_i = drive_bit(164) ? '1 : '0;
    dc_ptw_req_valid_i = drive_bit(165) ? '1 : '0;
    minion_apb_prdata_i = drive_bit(166) ? '1 : '0;
    minion_apb_pready_i = drive_bit(167) ? '1 : '0;
    minion_apb_pslverr_i = drive_bit(168) ? '1 : '0;
    debug_minion_out_i = drive_bit(169) ? '1 : '0;
    minion_dbg_signals_i = drive_bit(170) ? '1 : '0;
    pmu_count_up_i = drive_bit(171) ? '1 : '0;
    pmu_read_sel_i = drive_bit(172) ? '1 : '0;
    pmu_write_en_i = drive_bit(173) ? '1 : '0;
    pmu_write_data_i = drive_bit(174) ? '1 : '0;
    pmu_neigh_event_sel_i = drive_bit(175) ? '1 : '0;
    tbox_status_esr_i = drive_bit(176) ? '1 : '0;
    tbox_neigh_req_valid_i = drive_bit(177) ? '1 : '0;
    tbox_neigh_req_info_i = drive_bit(178) ? '1 : '0;
    tbox_neigh_rsp_ready_i = drive_bit(179) ? '1 : '0;
    tbox_dbg_signals_i = drive_bit(180) ? '1 : '0;
    tbox_apb_rsp_i = drive_bit(181) ? '1 : '0;
    tbox_bpam_run_control_ack_i = drive_bit(182) ? '1 : '0;
  end

  assign input_toggle_ok_o = (cycle_q > ResetExerciseCycles) &&
                             (&drive_seen_zero_q[DriveMaxSalt:DriveMinSalt]) &&
                             (&drive_seen_one_q[DriveMaxSalt:DriveMinSalt]);

  neigh_channel u_new (
    .clk_i(clk_i),
    .clk_shire_i(clk_shire_i),
    .clk_minion_o(new_clk_minion_o),
    .rst_c_shire_ni(rst_c_shire_ni),
    .rst_d_shire_ni(rst_d_shire_ni),
    .rst_w_shire_ni(rst_w_shire_ni),
    .rst_warm_ni(rst_warm_ni),
    .rst_w_icache_no(new_rst_w_icache_no),
    .dft_hv_i(dft_hv_i),
    .dft_use_reset_cntl_hv_i(dft_use_reset_cntl_hv_i),
    .dft_reset_cntl_hv_i(dft_reset_cntl_hv_i),
    .dft_lv_o(new_dft_lv_o),
    .chicken_bits_vc_i(chicken_bits_vc_i),
    .bpam_run_control_i(bpam_run_control_i),
    .bpam_rc_tbox_ack_hi_o(new_bpam_rc_tbox_ack_hi_o),
    .dll_feedback_shire_o(new_dll_feedback_shire_o),
    .dll_feedback_neigh_o(new_dll_feedback_neigh_o),
    .shire_id_i(shire_id_i),
    .neigh_id_i(neigh_id_i),
    .shire_tbox_id_i(shire_tbox_id_i),
    .shire_tbox_en_i(shire_tbox_en_i),
    .esr_thread0_enable_i(esr_thread0_enable_i),
    .esr_thread1_enable_i(esr_thread1_enable_i),
    .esr_minion_features_i(esr_minion_features_i),
    .esr_icache_prefetch_conf_i(esr_icache_prefetch_conf_i),
    .esr_icache_prefetch_start_i(esr_icache_prefetch_start_i),
    .esr_icache_prefetch_done_o(new_esr_icache_prefetch_done_o),
    .esr_icache_err_detected_o(new_esr_icache_err_detected_o),
    .esr_icache_err_logged_o(new_esr_icache_err_logged_o),
    .dmctrl_i(dmctrl_i),
    .esr_and_or_tree_l0_o(new_esr_and_or_tree_l0_o),
    .esr_shire_coop_mode_i(esr_shire_coop_mode_i),
    .esr_minion_mem_override_i(esr_minion_mem_override_i),
    .esr_dll_dly_est_ctl_i(esr_dll_dly_est_ctl_i),
    .esr_dll_dly_est_sts_o(new_esr_dll_dly_est_sts_o),
    .neigh_sc_req_ready_i(neigh_sc_req_ready_i),
    .neigh_sc_req_valid_o(new_neigh_sc_req_valid_o),
    .neigh_sc_req_info_o(new_neigh_sc_req_info_o),
    .neigh_sc_rsp_ready_o(new_neigh_sc_rsp_ready_o),
    .neigh_sc_rsp_valid_i(neigh_sc_rsp_valid_i),
    .neigh_sc_rsp_info_i(neigh_sc_rsp_info_i),
    .hw_dbg_sm_monitor_enabled_i(hw_dbg_sm_monitor_enabled_i),
    .neigh_sm_gpio_i(neigh_sm_gpio_i),
    .neigh_sm_signals_o(new_neigh_sm_signals_o),
    .apb_req_from_shire_i(apb_req_from_shire_i),
    .apb_rsp_to_shire_o(new_apb_rsp_to_shire_o),
    .int_mtip_i(int_mtip_i),
    .int_meip_i(int_meip_i),
    .int_seip_i(int_seip_i),
    .ipi_msip_i(ipi_msip_i),
    .ipi_redirect_trigger_i(ipi_redirect_trigger_i),
    .uc_to_neigh_fcc_i(uc_to_neigh_fcc_i),
    .uc_to_neigh_fcc_target_i(uc_to_neigh_fcc_target_i),
    .flb_neigh_l2_req_valid_o(new_flb_neigh_l2_req_valid_o),
    .flb_neigh_l2_req_data_o(new_flb_neigh_l2_req_data_o),
    .flb_l2_neigh_resp_valid_i(flb_l2_neigh_resp_valid_i),
    .flb_l2_neigh_resp_data_i(flb_l2_neigh_resp_data_i),
    .icache_f2_sram_req_write_o(new_icache_f2_sram_req_write_o),
    .icache_f2_sram_req_addr_o(new_icache_f2_sram_req_addr_o),
    .icache_f2_sram_req_valid_o(new_icache_f2_sram_req_valid_o),
    .icache_f2_sram_req_ready_i(icache_f2_sram_req_ready_i),
    .icache_f0_sram_resp_dout_i(icache_f0_sram_resp_dout_i),
    .icache_f0_sram_resp_valid_i(icache_f0_sram_resp_valid_i),
    .icache_f0_sram_resp_ready_o(new_icache_f0_sram_resp_ready_o),
    .coop_tload_slv_rdy_out_data_o(new_coop_tload_slv_rdy_out_data_o),
    .coop_tload_slv_rdy_out_valid_o(new_coop_tload_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data_i(coop_tload_slv_rdy_in_data_i),
    .coop_tload_slv_rdy_in_valid_i(coop_tload_slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id_o(new_coop_tload_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid_o(new_coop_tload_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id_i(coop_tload_mst_done_in_coop_id_i),
    .coop_tload_mst_done_in_valid_i(coop_tload_mst_done_in_valid_i),
    .rst_c_per_minion_no(new_rst_c_per_minion_no),
    .rst_d_per_minion_no(new_rst_d_per_minion_no),
    .rst_w_per_minion_no(new_rst_w_per_minion_no),
    .chicken_bits_vc_lo_o(new_chicken_bits_vc_lo_o),
    .shire_id_lo_o(new_shire_id_lo_o),
    .neigh_id_lo_o(new_neigh_id_lo_o),
    .shire_min_id_o(new_shire_min_id_o),
    .thread0_enable_o(new_thread0_enable_o),
    .thread1_enable_o(new_thread1_enable_o),
    .esr_minion_features_lo_o(new_esr_minion_features_lo_o),
    .esr_shire_coop_mode_lo_o(new_esr_shire_coop_mode_lo_o),
    .esr_minion_mem_override_lo_o(new_esr_minion_mem_override_lo_o),
    .esr_bypass_dcache_o(new_esr_bypass_dcache_o),
    .minion_boot_addr_o(new_minion_boot_addr_o),
    .mprot_o(new_mprot_o),
    .vmspagesize_o(new_vmspagesize_o),
    .interrupts_per_minion_o(new_interrupts_per_minion_o),
    .flb_core_neigh_req_valid_i(flb_core_neigh_req_valid_i),
    .flb_core_neigh_req_data_i(flb_core_neigh_req_data_i),
    .flb_neigh_core_resp_valid_o(new_flb_neigh_core_resp_valid_o),
    .flb_neigh_core_resp_data_o(new_flb_neigh_core_resp_data_o),
    .l2_dcache_evict_req_ready_o(new_l2_dcache_evict_req_ready_o),
    .l2_dcache_evict_req_valid_i(l2_dcache_evict_req_valid_i),
    .l2_dcache_evict_req_i(l2_dcache_evict_req_i),
    .l2_dcache_miss_req_ready_o(new_l2_dcache_miss_req_ready_o),
    .l2_dcache_miss_req_valid_i(l2_dcache_miss_req_valid_i),
    .l2_dcache_miss_req_i(l2_dcache_miss_req_i),
    .l2_dcache_resp_ready_i(l2_dcache_resp_ready_i),
    .l2_dcache_resp_valid_o(new_l2_dcache_resp_valid_o),
    .l2_dcache_resp_o(new_l2_dcache_resp_o),
    .icache_req_ready_o(new_icache_req_ready_o),
    .icache_req_valid_i(icache_req_valid_i),
    .icache_req_i(icache_req_i),
    .icache_resp_valid_o(new_icache_resp_valid_o),
    .icache_resp_miss_o(new_icache_resp_miss_o),
    .icache_resp_o(new_icache_resp_o),
    .icache_fill_done_o(new_icache_fill_done_o),
    .icache_flush_data_i(icache_flush_data_i),
    .satp_info_i(satp_info_i),
    .matp_info_i(matp_info_i),
    .tlb_invalidate_i(tlb_invalidate_i),
    .dc_ptw_req_data_i(dc_ptw_req_data_i),
    .dc_ptw_req_valid_i(dc_ptw_req_valid_i),
    .dc_ptw_req_ready_o(new_dc_ptw_req_ready_o),
    .ptw_dc_resp_data_o(new_ptw_dc_resp_data_o),
    .ptw_dc_resp_valid_o(new_ptw_dc_resp_valid_o),
    .minion_apb_prdata_i(minion_apb_prdata_i),
    .minion_apb_pready_i(minion_apb_pready_i),
    .minion_apb_pslverr_i(minion_apb_pslverr_i),
    .minion_apb_penable_o(new_minion_apb_penable_o),
    .minion_apb_psel_o(new_minion_apb_psel_o),
    .minion_apb_pwrite_o(new_minion_apb_pwrite_o),
    .minion_apb_paddr_o(new_minion_apb_paddr_o),
    .minion_apb_pwdata_o(new_minion_apb_pwdata_o),
    .debug_minion_in_o(new_debug_minion_in_o),
    .debug_minion_out_i(debug_minion_out_i),
    .minion_dbg_signals_en_o(new_minion_dbg_signals_en_o),
    .minion_dbg_signals_i(minion_dbg_signals_i),
    .minion_dbg_signals_mux_o(new_minion_dbg_signals_mux_o),
    .pmu_count_up_i(pmu_count_up_i),
    .pmu_read_data_o(new_pmu_read_data_o),
    .pmu_read_sel_i(pmu_read_sel_i),
    .pmu_write_en_i(pmu_write_en_i),
    .pmu_write_data_i(pmu_write_data_i),
    .pmu_neigh_event_sel_i(pmu_neigh_event_sel_i),
    .clk_tbox_o(new_clk_tbox_o),
    .tbox_rst_c_no(new_tbox_rst_c_no),
    .tbox_rst_d_no(new_tbox_rst_d_no),
    .tbox_rst_w_no(new_tbox_rst_w_no),
    .tbox_id_lo_o(new_tbox_id_lo_o),
    .tbox_control_esr_o(new_tbox_control_esr_o),
    .tbox_status_esr_i(tbox_status_esr_i),
    .tbox_image_table_ptr_esr_o(new_tbox_image_table_ptr_esr_o),
    .tbox_neigh_req_ready_o(new_tbox_neigh_req_ready_o),
    .tbox_neigh_req_valid_i(tbox_neigh_req_valid_i),
    .tbox_neigh_req_info_i(tbox_neigh_req_info_i),
    .tbox_neigh_rsp_ready_i(tbox_neigh_rsp_ready_i),
    .tbox_neigh_rsp_valid_o(new_tbox_neigh_rsp_valid_o),
    .tbox_neigh_rsp_info_o(new_tbox_neigh_rsp_info_o),
    .tbox_dbg_signals_en_o(new_tbox_dbg_signals_en_o),
    .tbox_dbg_signals_i(tbox_dbg_signals_i),
    .tbox_dbg_signals_mux_o(new_tbox_dbg_signals_mux_o),
    .tbox_apb_req_o(new_tbox_apb_req_o),
    .tbox_apb_rsp_i(tbox_apb_rsp_i),
    .tbox_bpam_run_control_o(new_tbox_bpam_run_control_o),
    .tbox_bpam_run_control_ack_i(tbox_bpam_run_control_ack_i)
  );

  neigh_channel_orig u_orig (
    .clock(clk_i),
    .clock_shire(clk_shire_i),
    .clock_minion(orig_clock_minion),
    .reset_c_shire(!rst_c_shire_ni),
    .reset_d_shire(!rst_d_shire_ni),
    .reset_w_shire(!rst_w_shire_ni),
    .reset_warm(!rst_warm_ni),
    .reset_w_icache(orig_reset_w_icache),
    .dft__reset_byp_hv(dft_hv_i.scanmode),
    .dft__reset_hv(!dft_hv_i.scan_reset_n),
    .dft__use_reset_cntl_hv(dft_use_reset_cntl_hv_i),
    .dft__reset_cntl_hv(dft_reset_cntl_hv_i),
    .chicken_bits_vc(chicken_bits_vc_i_flat),
    .bpam_run_control(bpam_run_control_i),
    .bpam_rc_tbox_ack_hi(orig_bpam_rc_tbox_ack_hi),
    .dll_feedback_shire(orig_dll_feedback_shire),
    .dll_feedback_neigh(orig_dll_feedback_neigh),
    .shire_id(shire_id_i),
    .neigh_id(neigh_id_i),
    .shire_tbox_id(shire_tbox_id_i),
    .shire_tbox_en(shire_tbox_en_i),
    .esr_thread0_enable(esr_thread0_enable_i),
    .esr_thread1_enable(esr_thread1_enable_i),
    .esr_minion_features(esr_minion_features_i_flat),
    .esr_icache_prefetch_conf(esr_icache_prefetch_conf_i_flat),
    .esr_icache_prefetch_start(esr_icache_prefetch_start_i),
    .esr_icache_prefetch_done(orig_esr_icache_prefetch_done),
    .esr_icache_err_detected_op(orig_esr_icache_err_detected_op),
    .esr_icache_err_logged_op(orig_esr_icache_err_logged_op),
    .dmctrl(dmctrl_i_flat),
    .esr_and_or_tree_L0_op(orig_esr_and_or_tree_L0_op),
    .esr_shire_coop_mode(esr_shire_coop_mode_i),
    .esr_minion_mem_override(esr_minion_mem_override_i),
    .esr_dll_dly_est_ctl(esr_dll_dly_est_ctl_i_flat),
    .esr_dll_dly_est_sts(orig_esr_dll_dly_est_sts),
    .neigh_sc_req_ready(neigh_sc_req_ready_i),
    .neigh_sc_req_valid(orig_neigh_sc_req_valid),
    .neigh_sc_req_info(orig_neigh_sc_req_info),
    .neigh_sc_rsp_ready(orig_neigh_sc_rsp_ready),
    .neigh_sc_rsp_valid(neigh_sc_rsp_valid_i),
    .neigh_sc_rsp_info(neigh_sc_rsp_info_i_flat),
    .hw_dbg_sm_monitor_enabled(hw_dbg_sm_monitor_enabled_i),
    .neigh_sm_gpio(neigh_sm_gpio_i),
    .neigh_sm_signals(orig_neigh_sm_signals),
    .APB_req_from_shire(apb_req_from_shire_i_flat),
    .APB_rsp_to_shire(orig_APB_rsp_to_shire),
    .int_mtip(int_mtip_i),
    .int_meip(int_meip_i),
    .int_seip(int_seip_i),
    .ipi_msip(ipi_msip_i),
    .ipi_redirect_trigger(ipi_redirect_trigger_i),
    .uc_to_neigh_fcc(uc_to_neigh_fcc_i),
    .uc_to_neigh_fcc_target(uc_to_neigh_fcc_target_i),
    .flb_neigh_l2_req_valid(orig_flb_neigh_l2_req_valid),
    .flb_neigh_l2_req_data(orig_flb_neigh_l2_req_data),
    .flb_l2_neigh_resp_valid(flb_l2_neigh_resp_valid_i),
    .flb_l2_neigh_resp_data(flb_l2_neigh_resp_data_i),
    .icache_f2_sram_req_write(orig_icache_f2_sram_req_write),
    .icache_f2_sram_req_addr(orig_icache_f2_sram_req_addr),
    .icache_f2_sram_req_valid(orig_icache_f2_sram_req_valid),
    .icache_f2_sram_req_ready(icache_f2_sram_req_ready_i),
    .icache_f0_sram_resp_dout(icache_f0_sram_resp_dout_i),
    .icache_f0_sram_resp_valid(icache_f0_sram_resp_valid_i),
    .icache_f0_sram_resp_ready(orig_icache_f0_sram_resp_ready),
    .coop_tload_slv_rdy_out_data(orig_coop_tload_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid(orig_coop_tload_slv_rdy_out_valid),
    .coop_tload_slv_rdy_in_data(coop_tload_slv_rdy_in_data_i),
    .coop_tload_slv_rdy_in_valid(coop_tload_slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id(orig_coop_tload_mst_done_out_coop_id),
    .coop_tload_mst_done_out_valid(orig_coop_tload_mst_done_out_valid),
    .coop_tload_mst_done_in_coop_id(coop_tload_mst_done_in_coop_id_i),
    .coop_tload_mst_done_in_valid(coop_tload_mst_done_in_valid_i),
    .reset_c_per_minion(orig_reset_c_per_minion),
    .reset_d_per_minion(orig_reset_d_per_minion),
    .reset_w_per_minion(orig_reset_w_per_minion),
    .dft__reset_byp_lv(orig_dft__reset_byp_lv),
    .dft__reset_lv(orig_dft__reset_lv),
    .chicken_bits_vc_lo(orig_chicken_bits_vc_lo),
    .shire_id_lo(orig_shire_id_lo),
    .neigh_id_lo(orig_neigh_id_lo),
    .shire_min_id(orig_shire_min_id),
    .thread0_enable(orig_thread0_enable),
    .thread1_enable(orig_thread1_enable),
    .esr_minion_features_lo(orig_esr_minion_features_lo),
    .esr_shire_coop_mode_lo(orig_esr_shire_coop_mode_lo),
    .esr_minion_mem_override_lo(orig_esr_minion_mem_override_lo),
    .esr_bypass_dcache(orig_esr_bypass_dcache),
    .minion_boot_addr(orig_minion_boot_addr),
    .mprot(orig_mprot),
    .vmspagesize(orig_vmspagesize),
    .interrupts_per_minion(orig_interrupts_per_minion),
    .flb_core_neigh_req_valid(flb_core_neigh_req_valid_i),
    .flb_core_neigh_req_data(flb_core_neigh_req_data_i),
    .flb_neigh_core_resp_valid(orig_flb_neigh_core_resp_valid),
    .flb_neigh_core_resp_data(orig_flb_neigh_core_resp_data),
    .l2_dcache_evict_req_ready(orig_l2_dcache_evict_req_ready),
    .l2_dcache_evict_req_valid(l2_dcache_evict_req_valid_i),
    .l2_dcache_evict_req(l2_dcache_evict_req_i_flat),
    .l2_dcache_miss_req_ready(orig_l2_dcache_miss_req_ready),
    .l2_dcache_miss_req_valid(l2_dcache_miss_req_valid_i),
    .l2_dcache_miss_req(l2_dcache_miss_req_i_flat),
    .l2_dcache_resp_ready(l2_dcache_resp_ready_i),
    .l2_dcache_resp_valid(orig_l2_dcache_resp_valid),
    .l2_dcache_resp(orig_l2_dcache_resp),
    .icache_req_ready(orig_icache_req_ready),
    .icache_req_valid(icache_req_valid_i),
    .icache_req(icache_req_i_flat),
    .icache_resp_valid(orig_icache_resp_valid),
    .icache_resp_miss(orig_icache_resp_miss),
    .icache_resp(orig_icache_resp),
    .icache_fill_done(orig_icache_fill_done),
    .icache_flush_data(icache_flush_data_i),
    .satp_info(satp_info_i),
    .matp_info(matp_info_i),
    .tlb_invalidate(tlb_invalidate_i),
    .dc_ptw_req_data(dc_ptw_req_data_i_flat),
    .dc_ptw_req_valid(dc_ptw_req_valid_i),
    .dc_ptw_req_ready(orig_dc_ptw_req_ready),
    .ptw_dc_resp_data(orig_ptw_dc_resp_data),
    .ptw_dc_resp_valid(orig_ptw_dc_resp_valid),
    .minion_apb_prdata(minion_apb_prdata_i),
    .minion_apb_pready(minion_apb_pready_i),
    .minion_apb_pslverr(minion_apb_pslverr_i),
    .minion_apb_penable(orig_minion_apb_penable),
    .minion_apb_psel(orig_minion_apb_psel),
    .minion_apb_pwrite(orig_minion_apb_pwrite),
    .minion_apb_paddr(orig_minion_apb_paddr),
    .minion_apb_pwdata(orig_minion_apb_pwdata),
    .debug_minion_in(orig_debug_minion_in),
    .debug_minion_out(debug_minion_out_i_flat),
    .minion_dbg_signals_en(orig_minion_dbg_signals_en),
    .minion_dbg_signals(minion_dbg_signals_i_flat),
    .minion_dbg_signals_mux(orig_minion_dbg_signals_mux),
    .pmu_count_up(pmu_count_up_i),
    .pmu_read_data(orig_pmu_read_data),
    .pmu_read_sel(pmu_read_sel_i),
    .pmu_write_en(pmu_write_en_i),
    .pmu_write_data(pmu_write_data_i),
    .pmu_neigh_event_sel(pmu_neigh_event_sel_i),
    .clock_tbox(orig_clock_tbox),
    .tbox_reset_c(orig_tbox_reset_c),
    .tbox_reset_d(orig_tbox_reset_d),
    .tbox_reset_w(orig_tbox_reset_w),
    .tbox_id_lo(orig_tbox_id_lo),
    .tbox_control_esr(orig_tbox_control_esr),
    .tbox_status_esr(tbox_status_esr_i),
    .tbox_image_table_ptr_esr(orig_tbox_image_table_ptr_esr),
    .tbox_neigh_req_ready(orig_tbox_neigh_req_ready),
    .tbox_neigh_req_valid(tbox_neigh_req_valid_i),
    .tbox_neigh_req_info(tbox_neigh_req_info_i_flat),
    .tbox_neigh_rsp_ready(tbox_neigh_rsp_ready_i),
    .tbox_neigh_rsp_valid(orig_tbox_neigh_rsp_valid),
    .tbox_neigh_rsp_info(orig_tbox_neigh_rsp_info),
    .tbox_dbg_signals_en(orig_tbox_dbg_signals_en),
    .tbox_dbg_signals(tbox_dbg_signals_i),
    .tbox_dbg_signals_mux(orig_tbox_dbg_signals_mux),
    .tbox_APB_req(orig_tbox_APB_req),
    .tbox_APB_rsp(tbox_apb_rsp_i_flat),
    .tbox_bpam_run_control(orig_tbox_bpam_run_control),
    .tbox_bpam_run_control_ack(tbox_bpam_run_control_ack_i_flat)
  );


  localparam int unsigned OutFlatWidth = $bits({new_clk_minion_o, !new_rst_w_icache_no, new_bpam_rc_tbox_ack_hi_o_flat, new_dll_feedback_shire_o, new_dll_feedback_neigh_o, new_esr_icache_prefetch_done_o, new_esr_icache_err_detected_o, new_esr_icache_err_logged_o, new_esr_and_or_tree_l0_o_flat, new_esr_dll_dly_est_sts_o_flat, new_neigh_sc_req_valid_o, new_neigh_sc_req_info_o_flat, new_neigh_sc_rsp_ready_o, new_neigh_sm_signals_o_flat, new_apb_rsp_to_shire_o_flat, new_flb_neigh_l2_req_valid_o, new_flb_neigh_l2_req_data_o, new_icache_f2_sram_req_write_o, new_icache_f2_sram_req_addr_o, new_icache_f2_sram_req_valid_o, new_icache_f0_sram_resp_ready_o, new_coop_tload_slv_rdy_out_data_o_flat, new_coop_tload_slv_rdy_out_valid_o, new_coop_tload_mst_done_out_coop_id_o, new_coop_tload_mst_done_out_valid_o, ~new_rst_c_per_minion_no, ~new_rst_d_per_minion_no, ~new_rst_w_per_minion_no, new_dft_lv_o.scanmode, !new_dft_lv_o.scan_reset_n, new_dft_lv_o.sram_clk_override, new_dft_lv_o.ram_rei, new_dft_lv_o.ram_wei, new_chicken_bits_vc_lo_o_flat, new_shire_id_lo_o, new_neigh_id_lo_o, new_shire_min_id_o, new_thread0_enable_o, new_thread1_enable_o, new_esr_minion_features_lo_o_flat, new_esr_shire_coop_mode_lo_o, new_esr_minion_mem_override_lo_o, new_esr_bypass_dcache_o, new_minion_boot_addr_o, new_mprot_o_flat, new_vmspagesize_o_flat, new_interrupts_per_minion_o_flat, new_flb_neigh_core_resp_valid_o, new_flb_neigh_core_resp_data_o, new_l2_dcache_evict_req_ready_o, new_l2_dcache_miss_req_ready_o, new_l2_dcache_resp_valid_o, new_l2_dcache_resp_o_flat, new_icache_req_ready_o, new_icache_resp_valid_o, new_icache_resp_miss_o, new_icache_resp_o_flat, new_icache_fill_done_o, new_dc_ptw_req_ready_o, new_ptw_dc_resp_data_o_flat, new_ptw_dc_resp_valid_o, new_minion_apb_penable_o, new_minion_apb_psel_o, new_minion_apb_pwrite_o, new_minion_apb_paddr_o, new_minion_apb_pwdata_o, new_debug_minion_in_o_flat, new_minion_dbg_signals_en_o, new_minion_dbg_signals_mux_o, new_pmu_read_data_o, new_clk_tbox_o, !new_tbox_rst_c_no, !new_tbox_rst_d_no, !new_tbox_rst_w_no, new_tbox_id_lo_o, new_tbox_control_esr_o, new_tbox_image_table_ptr_esr_o, new_tbox_neigh_req_ready_o, new_tbox_neigh_rsp_valid_o, new_tbox_neigh_rsp_info_o_flat, new_tbox_dbg_signals_en_o, new_tbox_dbg_signals_mux_o, new_tbox_apb_req_o_flat, new_tbox_bpam_run_control_o_flat});
  logic [OutFlatWidth-1:0] new_outputs_flat;
  logic [OutFlatWidth-1:0] orig_outputs_flat;
  logic [OutChunks*64-1:0] new_outputs_padded;
  logic [OutChunks*64-1:0] orig_outputs_padded;
  assign new_outputs_flat = {new_clk_minion_o, !new_rst_w_icache_no, new_bpam_rc_tbox_ack_hi_o_flat, new_dll_feedback_shire_o, new_dll_feedback_neigh_o, new_esr_icache_prefetch_done_o, new_esr_icache_err_detected_o, new_esr_icache_err_logged_o, new_esr_and_or_tree_l0_o_flat, new_esr_dll_dly_est_sts_o_flat, new_neigh_sc_req_valid_o, new_neigh_sc_req_info_o_flat, new_neigh_sc_rsp_ready_o, new_neigh_sm_signals_o_flat, new_apb_rsp_to_shire_o_flat, new_flb_neigh_l2_req_valid_o, new_flb_neigh_l2_req_data_o, new_icache_f2_sram_req_write_o, new_icache_f2_sram_req_addr_o, new_icache_f2_sram_req_valid_o, new_icache_f0_sram_resp_ready_o, new_coop_tload_slv_rdy_out_data_o_flat, new_coop_tload_slv_rdy_out_valid_o, new_coop_tload_mst_done_out_coop_id_o, new_coop_tload_mst_done_out_valid_o, ~new_rst_c_per_minion_no, ~new_rst_d_per_minion_no, ~new_rst_w_per_minion_no, new_dft_lv_o.scanmode, !new_dft_lv_o.scan_reset_n, new_dft_lv_o.sram_clk_override, new_dft_lv_o.ram_rei, new_dft_lv_o.ram_wei, new_chicken_bits_vc_lo_o_flat, new_shire_id_lo_o, new_neigh_id_lo_o, new_shire_min_id_o, new_thread0_enable_o, new_thread1_enable_o, new_esr_minion_features_lo_o_flat, new_esr_shire_coop_mode_lo_o, new_esr_minion_mem_override_lo_o, new_esr_bypass_dcache_o, new_minion_boot_addr_o, new_mprot_o_flat, new_vmspagesize_o_flat, new_interrupts_per_minion_o_flat, new_flb_neigh_core_resp_valid_o, new_flb_neigh_core_resp_data_o, new_l2_dcache_evict_req_ready_o, new_l2_dcache_miss_req_ready_o, new_l2_dcache_resp_valid_o, new_l2_dcache_resp_o_flat, new_icache_req_ready_o, new_icache_resp_valid_o, new_icache_resp_miss_o, new_icache_resp_o_flat, new_icache_fill_done_o, new_dc_ptw_req_ready_o, new_ptw_dc_resp_data_o_flat, new_ptw_dc_resp_valid_o, new_minion_apb_penable_o, new_minion_apb_psel_o, new_minion_apb_pwrite_o, new_minion_apb_paddr_o, new_minion_apb_pwdata_o, new_debug_minion_in_o_flat, new_minion_dbg_signals_en_o, new_minion_dbg_signals_mux_o, new_pmu_read_data_o, new_clk_tbox_o, !new_tbox_rst_c_no, !new_tbox_rst_d_no, !new_tbox_rst_w_no, new_tbox_id_lo_o, new_tbox_control_esr_o, new_tbox_image_table_ptr_esr_o, new_tbox_neigh_req_ready_o, new_tbox_neigh_rsp_valid_o, new_tbox_neigh_rsp_info_o_flat, new_tbox_dbg_signals_en_o, new_tbox_dbg_signals_mux_o, new_tbox_apb_req_o_flat, new_tbox_bpam_run_control_o_flat};
  assign orig_outputs_flat = {orig_clock_minion, orig_reset_w_icache, orig_bpam_rc_tbox_ack_hi, orig_dll_feedback_shire, orig_dll_feedback_neigh, orig_esr_icache_prefetch_done, orig_esr_icache_err_detected_op, orig_esr_icache_err_logged_op, orig_esr_and_or_tree_L0_op, orig_esr_dll_dly_est_sts, orig_neigh_sc_req_valid, orig_neigh_sc_req_info, orig_neigh_sc_rsp_ready, orig_neigh_sm_signals, orig_APB_rsp_to_shire, orig_flb_neigh_l2_req_valid, orig_flb_neigh_l2_req_data, orig_icache_f2_sram_req_write, orig_icache_f2_sram_req_addr, orig_icache_f2_sram_req_valid, orig_icache_f0_sram_resp_ready, orig_coop_tload_slv_rdy_out_data, orig_coop_tload_slv_rdy_out_valid, orig_coop_tload_mst_done_out_coop_id, orig_coop_tload_mst_done_out_valid, orig_reset_c_per_minion, orig_reset_d_per_minion, orig_reset_w_per_minion, orig_dft__reset_byp_lv, orig_dft__reset_lv, exp_dft_lv_sram_clk_override, exp_dft_lv_ram_rei, exp_dft_lv_ram_wei, orig_chicken_bits_vc_lo, orig_shire_id_lo, orig_neigh_id_lo, orig_shire_min_id, orig_thread0_enable, orig_thread1_enable, orig_esr_minion_features_lo, orig_esr_shire_coop_mode_lo, orig_esr_minion_mem_override_lo, orig_esr_bypass_dcache, orig_minion_boot_addr, orig_mprot, orig_vmspagesize, orig_interrupts_per_minion, orig_flb_neigh_core_resp_valid, orig_flb_neigh_core_resp_data, orig_l2_dcache_evict_req_ready, orig_l2_dcache_miss_req_ready, orig_l2_dcache_resp_valid, orig_l2_dcache_resp, orig_icache_req_ready, orig_icache_resp_valid, orig_icache_resp_miss, orig_icache_resp, orig_icache_fill_done, orig_dc_ptw_req_ready, orig_ptw_dc_resp_data, orig_ptw_dc_resp_valid, orig_minion_apb_penable, orig_minion_apb_psel, orig_minion_apb_pwrite, orig_minion_apb_paddr, orig_minion_apb_pwdata, orig_debug_minion_in, orig_minion_dbg_signals_en, orig_minion_dbg_signals_mux, orig_pmu_read_data, orig_clock_tbox, orig_tbox_reset_c, orig_tbox_reset_d, orig_tbox_reset_w, orig_tbox_id_lo, orig_tbox_control_esr, orig_tbox_image_table_ptr_esr, orig_tbox_neigh_req_ready, orig_tbox_neigh_rsp_valid, orig_tbox_neigh_rsp_info, orig_tbox_dbg_signals_en, orig_tbox_dbg_signals_mux, orig_tbox_APB_req, orig_tbox_bpam_run_control};
  assign new_outputs_padded = {{(OutChunks*64-OutFlatWidth){1'b0}}, new_outputs_flat};
  assign orig_outputs_padded = {{(OutChunks*64-OutFlatWidth){1'b0}}, orig_outputs_flat};

  for (genvar chunk = 0; chunk < OutChunks; chunk++) begin : gen_chunks
    assign new_chunks_o[chunk] = new_outputs_padded[chunk*64 +: 64];
    assign orig_chunks_o[chunk] = orig_outputs_padded[chunk*64 +: 64];
  end

endmodule : cosim_neigh_channel_tb
/* verilator lint_on ENUMVALUE */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNUSEDSIGNAL */
