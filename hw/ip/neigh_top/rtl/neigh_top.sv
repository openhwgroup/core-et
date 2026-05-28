// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // Top shell preserves original DFT/TBOX/minion surfaces; some fields are intentionally tied off in the non-GFX/all-stub configurations used by scoped DV.
/* verilator lint_off PINCONNECTEMPTY */  // DFT/ECO/trace outputs from instantiated minions are intentionally unconnected, matching the original top shell.
/* verilator lint_off UNUSEDPARAM */  // Simulation ID parameters are preserved for original ET_SIMULATION compatibility.
module neigh_top
  import dft_pkg::*;
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
  import esr_pkg::*;
  import neigh_ch_dbg_pkg::*;
#(
  parameter int unsigned ShireId = 0,
  parameter int unsigned NeighId = 0,
  parameter bit          EnableGfx = 1'b0,
  parameter logic [neigh_pkg::MinPerN-1:0] DisableMinions = '0,
  parameter logic [neigh_pkg::MinPerN-1:0] StubMinions = '0
) (
  input  logic                                             clk_i,
  input  logic                                             clk_shire_i,
  input  logic                                             rst_c_shire_ni,
  input  logic                                             rst_d_shire_ni,
  input  logic                                             rst_w_shire_ni,
  input  logic                                             rst_warm_ni,
  output logic                                             rst_w_icache_no,

  input  logic [neigh_pkg::ShireDftScaninNeighWidth-1:0]   dft_scanin_hv_i,
  output logic [neigh_pkg::ShireDftScanoutNeighWidth-1:0]  dft_scanout_hv_o,
  input  dft_t                                             dft_hv_i,
  input  logic                                             dft_scan_enable_hv_i,
  input  logic [neigh_pkg::ShireDftModeWidth-1:0]          dft_test_mode_hv_i,
  input  logic                                             dft_clock_gate_en_hv_i,
  input  logic [neigh_pkg::ShireDftCntlWidth-1:0]          dft_cntl_hv_i,
  input  logic                                             dft_occ_scanin_hv_i,
  output logic                                             dft_occ_scanout_hv_o,
  input  logic                                             dft_occ_reset_hv_i,
  input  logic                                             dft_occ_testmode_hv_i,
  input  logic                                             dft_scan_ate_clk_hv_i,
  input  logic                                             dft_occ_bypass_hv_i,
  input  logic                                             dft_use_reset_cntl_hv_i,
  input  neigh_pkg::shire_tdr_reset_cntl_t                 dft_reset_cntl_hv_i,

  input  logic [neigh_pkg::ShireDftScaninTboxWidth-1:0]    dft_scanin_tbox_hv_i,
  output logic [neigh_pkg::ShireDftScanoutTboxWidth-1:0]   dft_scanout_tbox_hv_o,
  input  logic                                             dft_occ_scanin_tbox_hv_i,
  output logic                                             dft_occ_scanout_tbox_hv_o,

  input  logic [neigh_pkg::EtEcoNeighInputWidth-1:0]       eco_i,
  output logic [neigh_pkg::EtEcoNeighOutputWidth-1:0]      eco_o,

  input  logic                                             pwr_ctrl_glb_nsleepin_i,
  output logic                                             pwr_ctrl_glb_nsleepout_o,
  input  logic                                             pwr_ctrl_glb_isolate_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    pwr_ctrl_min_nsleepin_i,
  output logic [neigh_pkg::MinPerN-1:0]                    pwr_ctrl_min_nsleepout_o,
  input  logic [neigh_pkg::MinPerN-1:0]                    pwr_ctrl_min_isolate_i,

  input  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t esr_clk_gate_ctrl_i,
  input  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_i,

  output neigh_hv_logic_pkg::bpam_rc_tbox_ack_t            bpam_rc_tbox_ack_hi_o,
  input  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control_i,

  input  logic [minion_pkg::NumShireIdsBits-1:0]           shire_id_i,
  input  logic [neigh_pkg::NumNeighWidth-1:0]              neigh_id_i,
  input  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] shire_tbox_id_i,
  input  logic                                             shire_tbox_en_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    esr_thread0_enable_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    esr_thread1_enable_i,
  input  minion_pkg::esr_minion_features_t                 esr_minion_features_i,
  input  icache_pkg::icache_prefetch_conf_t                esr_icache_prefetch_conf_i,
  input  logic                                             esr_icache_prefetch_start_i,
  output logic                                             esr_icache_prefetch_done_o,
  output logic                                             esr_icache_err_detected_o,
  output logic                                             esr_icache_err_logged_o,
  input  esr_pkg::esr_ms_dmctrl_t                          dmctrl_i,
  output esr_pkg::esr_and_or_tree_l0_t                     esr_and_or_tree_l0_o,
  input  logic                                             esr_shire_coop_mode_i,

  output logic                                             dll_feedback_shire_o,
  output logic                                             dll_feedback_neigh_o,
  input  esr_pkg::esr_dll_dly_est_ctl_t                    esr_dll_dly_est_ctl_i,
  output esr_pkg::esr_dll_dly_est_sts_t                    esr_dll_dly_est_sts_o,

  input  logic [shirecache_pkg::Banks:0]                   neigh_sc_req_ready_i,
  output logic [shirecache_pkg::Banks:0]                   neigh_sc_req_valid_o,
  output etlink_pkg::req_t                                 neigh_sc_req_info_o,
  output logic                                             neigh_sc_rsp_ready_o,
  input  logic                                             neigh_sc_rsp_valid_i,
  input  etlink_pkg::rsp_t                                 neigh_sc_rsp_info_i,

  input  logic                                             hw_dbg_sm_monitor_enabled_i,
  input  logic [neigh_voltage_cross_pkg::NeighSmGpioWidth-1:0] neigh_sm_gpio_i,
  output minion_pkg::neigh_sm_dbg_monitor_t                neigh_sm_signals_o,

  input  neigh_ch_apb_mux_pkg::apb_to_neigh_t              apb_esr_req_i,
  output neigh_ch_apb_mux_pkg::apb_from_neigh_t            apb_esr_rsp_o,

  input  logic [neigh_pkg::MinPerN-1:0]                    int_mtip_i,
  input  logic                                             int_meip_i,
  input  logic                                             int_seip_i,
  input  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0]       ipi_msip_i,
  input  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0]       ipi_redirect_trigger_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    uc_to_neigh_fcc_i,
  input  logic [neigh_voltage_cross_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target_i,

  output logic                                             flb_neigh_l2_req_valid_o,
  output logic [neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] flb_neigh_l2_req_data_o,
  input  logic                                             flb_l2_neigh_resp_valid_i,
  input  logic                                             flb_l2_neigh_resp_data_i,

  output logic                                             icache_f2_sram_req_write_o,
  output logic [icache_geom_pkg::IcacheSramAddrWidth-1:0]  icache_f2_sram_req_addr_o,
  output logic                                             icache_f2_sram_req_valid_o,
  input  logic                                             icache_f2_sram_req_ready_i,
  input  logic [icache_geom_pkg::IcacheSramDataWidth-1:0]  icache_f0_sram_resp_dout_i,
  input  logic                                             icache_f0_sram_resp_valid_i,
  output logic                                             icache_f0_sram_resp_ready_o,

  output logic                                             voltage_monitor_vdd_o,
  output logic                                             voltage_monitor_vss_o,

  output logic                                             clk_tbox_o,
  output logic                                             tbox_rst_c_no,
  output logic                                             tbox_rst_d_no,
  output logic                                             tbox_rst_w_no,
  output dft_t                                             dft_tbox_lv_o,
  output logic [neigh_pkg::ShireDftScaninTboxWidth-1:0]    dft_scanin_to_tbox_lv_o,
  input  logic [neigh_pkg::ShireDftScanoutTboxWidth-1:0]   dft_scanout_to_tbox_lv_i,
  output logic                                             dft_occ_scanin_to_tbox_lv_o,
  input  logic                                             dft_occ_scanout_to_tbox_lv_i,
  output logic                                             dft_scan_enable_to_tbox_lv_o,
  output logic [neigh_pkg::ShireDftModeWidth-1:0]          dft_test_mode_to_tbox_lv_o,
  output logic                                             dft_clock_gate_en_to_tbox_lv_o,
  output logic [neigh_pkg::ShireDftCntlWidth-1:0]          dft_cntl_to_tbox_lv_o,
  output logic                                             dft_occ_reset_to_tbox_lv_o,
  output logic                                             dft_occ_testmode_to_tbox_lv_o,
  output logic                                             dft_scan_ate_clk_to_tbox_lv_o,
  output logic                                             dft_occ_bypass_to_tbox_lv_o,
  output logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0] tbox_id_lo_o,
  output logic [15:0]                                      tbox_control_esr_o,
  input  logic [15:0]                                      tbox_status_esr_i,
  output logic [minion_pkg::VaSize-1:0]                    tbox_image_table_ptr_esr_o,
  output logic                                             tbox_neigh_req_ready_o,
  input  logic                                             tbox_neigh_req_valid_i,
  input  etlink_pkg::neigh_req_t                           tbox_neigh_req_info_i,
  input  logic                                             tbox_neigh_rsp_ready_i,
  output logic                                             tbox_neigh_rsp_valid_o,
  output etlink_pkg::rsp_t                                 tbox_neigh_rsp_info_o,
  output logic                                             tbox_dbg_signals_en_o,
  input  minion_pkg::neigh_sm_dbg_monitor_t                tbox_dbg_signals_i,
  output logic [minion_pkg::NeighDebugSmMuxWidth-1:0]      tbox_dbg_signals_mux_o,
  output neigh_ch_apb_mux_pkg::apb_to_neigh_tbox_t         tbox_apb_req_o,
  input  neigh_ch_apb_mux_pkg::apb_from_neigh_t            tbox_apb_rsp_i,
  output neigh_ch_dbg_pkg::tbox_bpam_run_control_t         tbox_bpam_run_control_o,
  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t            tbox_bpam_run_control_ack_i,

  output neigh_pkg::coop_tload_slv_rdy_req_t               coop_tload_slv_rdy_out_data_o,
  output logic                                             coop_tload_slv_rdy_out_valid_o,
  input  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_data_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]              coop_tload_slv_rdy_in_valid_i,
  output logic [neigh_pkg::ShireCoopIdSize-1:0]            coop_tload_mst_done_out_coop_id_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0]              coop_tload_mst_done_out_valid_o,
  input  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]              coop_tload_mst_done_in_valid_i
);

  localparam int unsigned MinPerN = neigh_pkg::MinPerN;
  localparam int unsigned MinPerPtw = neigh_pkg::MinPerPtw;
  localparam int unsigned PtwPerN = neigh_pkg::PtwPerN;
  localparam int unsigned IcacheReqCount = icache_pkg::IcacheNrReqs;
  localparam int unsigned IcacheMinPerReq = MinPerN / IcacheReqCount;

  logic [MinPerN-1:0] reset_c_minion_n;
  logic [MinPerN-1:0] reset_d_minion_n;
  logic [MinPerN-1:0] reset_w_minion_n;
  logic [minion_pkg::NumShireIdsBits-1:0] shire_id_lo;
  logic [neigh_pkg::NumNeighWidth-1:0] neigh_id_lo;
  logic [MinPerN-1:0][minion_pkg::MinPerSBits-1:0] shire_min_id;
  logic [MinPerN-1:0] thread0_enable;
  logic [MinPerN-1:0] thread1_enable;
  minion_pkg::esr_minion_features_t esr_minion_features_lo;
  logic esr_shire_coop_mode_lo;
  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_lo;
  logic esr_bypass_dcache;
  logic [minion_pkg::VaSize-1:0] minion_boot_addr;
  esr_pkg::esr_mprot_t mprot;
  minion_pkg::tlb_entry_type vmspagesize;

  minion_pkg::minion_interrupts [MinPerN-1:0] interrupts_per_minion;
  logic [MinPerN-1:0] flb_neigh_req_valid;
  logic [MinPerN-1:0][neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] flb_neigh_req_data;
  logic [MinPerN-1:0] flb_neigh_resp_valid;
  logic flb_neigh_resp_data;

  logic [MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_ready;
  logic [MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_valid;
  minion_dcache_pkg::et_link_minion_evict_req_info_t [MinPerN-1:0] l2_dcache_evict_req;
  logic [MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] l2_dcache_miss_req_ready;
  logic [MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] l2_dcache_miss_req_valid;
  minion_dcache_pkg::et_link_minion_miss_req_info_t [MinPerN-1:0] l2_dcache_miss_req;
  logic [MinPerN-1:0] l2_dcache_resp_ready;
  logic [MinPerN-1:0] l2_dcache_resp_valid;
  minion_pkg::et_link_minion_rsp_info_t [MinPerN-1:0] l2_dcache_resp;

  logic [MinPerN-1:0] icache_req_ready;
  logic [MinPerN-1:0] icache_req_valid;
  icache_pkg::frontend_icache_req_t [MinPerN-1:0] icache_req;
  logic [IcacheReqCount-1:0] icache_resp_valid;
  logic [IcacheReqCount-1:0] icache_resp_miss;
  icache_pkg::icache_frontend_resp_t [IcacheReqCount-1:0] icache_resp;
  logic [IcacheReqCount-1:0] icache_fill_done;
  logic [MinPerN-1:0] icache_flush_data;

  minion_pkg::minion_satp_info [MinPerN-1:0] satp_info;
  minion_pkg::minion_satp_info [MinPerN-1:0] matp_info;
  logic [MinPerN-1:0] tlb_invalidate;
  minion_pkg::minion_ptw_req [PtwPerN-1:0][MinPerPtw-1:0] dc_ptw_req_data;
  logic [PtwPerN-1:0][MinPerPtw-1:0] dc_ptw_req_valid;
  logic [PtwPerN-1:0][MinPerPtw-1:0] dc_ptw_req_ready;
  minion_pkg::minion_ptw_pte [PtwPerN-1:0] ptw_dc_resp_data;
  logic [PtwPerN-1:0][MinPerPtw-1:0] ptw_dc_resp_valid;

  logic [MinPerN-1:0][esr_pkg::EsrApbDataWidth-1:0] minion_apb_prdata;
  logic [MinPerN-1:0] minion_apb_pready;
  logic [MinPerN-1:0] minion_apb_pslverr;
  logic minion_apb_penable;
  logic [MinPerN-1:0] minion_apb_psel;
  logic minion_apb_pwrite;
  logic [minion_pkg::MinDmApbAddrWidth-1:0] minion_apb_paddr;
  logic [esr_pkg::EsrApbDataWidth-1:0] minion_apb_pwdata;

  minion_pkg::minion_debug_in_t [MinPerN-1:0] debug_minion_in;
  minion_pkg::minion_debug_out_t [MinPerN-1:0] debug_minion_out;
  logic [MinPerN-1:0] minion_dbg_signals_en;
  minion_pkg::neigh_sm_dbg_monitor_t [MinPerN-1:0] minion_dbg_signals;
  logic [MinPerN-1:0][minion_pkg::NeighDebugSmMuxWidth-1:0] minion_dbg_signals_mux;
  logic [minion_pkg::NeighDebugSmMuxWidth-1:0] tbox_dbg_signals_mux_channel;

  logic [MinPerN-1:0][minion_pkg::PmuMinionCountersNum-1:0] pmu_count_up;
  logic [MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::XregSize-1:0] pmu_read_data;
  logic [MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::PmuCountersSelectBits-1:0] pmu_read_sel;
  logic [MinPerN-1:0][minion_pkg::PmuTotalCountersNum-1:0] pmu_write_en;
  logic [MinPerN-1:0][minion_pkg::XregSize-1:0] pmu_write_data;
  logic [MinPerN-1:0][minion_pkg::PmuNeighEventCntSelBits-1:0] pmu_neigh_event_sel;

  dft_t dft_lv;
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t chicken_bits_vc_lo;
  minion_pkg::minion_chicken_bits_t min_chicken_bits;

  logic clk_minion;
  logic pwr_ctrl_glb_isolate_int;
  logic pwr_ctrl_glb_nsleepin_lv;
  logic pwr_ctrl_glb_isolate_lv;
  logic [MinPerN-1:0] pwr_ctrl_min_nsleepin_lv;
  logic [MinPerN-1:0] pwr_ctrl_min_isolate_lv;
  logic pwr_ctrl_glb_nsleepout_lv;
  logic [MinPerN-1:0] pwr_ctrl_min_nsleepout_lv;
  logic [MinPerN-1:0] minion_nsleepin;
  logic [MinPerN-1:0] minion_nsleepout;
  logic [MinPerN-1:0] minion_iso_enable;

  logic coop_tload_slv_rdy_out_valid_pwrstub;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_mst_done_out_valid_pwrstub;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_esr_req_pwrstub;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_esr_rsp_pwrstub;
  logic [shirecache_pkg::Banks:0] neigh_sc_req_ready_pwrstub;
  logic [shirecache_pkg::Banks:0] neigh_sc_req_valid_pwrstub;
  logic neigh_sc_rsp_ready_pwrstub;
  logic neigh_sc_rsp_valid_pwrstub;
  logic esr_icache_prefetch_done_pwrstub;
  logic esr_icache_err_detected_pwrstub;
  logic esr_icache_err_logged_pwrstub;
  logic icache_f0_sram_resp_valid_pwrstub;
  logic icache_f0_sram_resp_ready_pwrstub;
  esr_pkg::esr_and_or_tree_l0_t esr_and_or_tree_l0_pwrstub;
  logic flb_neigh_l2_req_valid_pwrstub;
  logic flb_l2_neigh_resp_valid_pwrstub;
  logic icache_f2_sram_req_write_pwrstub;
  logic icache_f2_sram_req_valid_pwrstub;
  logic icache_f2_sram_req_ready_pwrstub;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_hi_pwrstub;
  logic reset_w_icache_pwrstub;
  logic reset_w_icache_pwrstub_out;
  logic [neigh_pkg::EtEcoNeighOutputWidth-1:0] eco_o_pwrstub;

  logic [MinPerN-1:0] int_mtip_sync_q [2];
  logic int_meip_sync_q [2];
  logic int_seip_sync_q [2];

  always_ff @(posedge clk_shire_i or negedge rst_c_shire_ni) begin
    if (!rst_c_shire_ni) begin
      int_mtip_sync_q[0] <= '0;
      int_mtip_sync_q[1] <= '0;
      int_meip_sync_q[0] <= 1'b0;
      int_meip_sync_q[1] <= 1'b0;
      int_seip_sync_q[0] <= 1'b0;
      int_seip_sync_q[1] <= 1'b0;
    end else begin
      int_mtip_sync_q[0] <= int_mtip_i;
      int_mtip_sync_q[1] <= int_mtip_sync_q[0];
      int_meip_sync_q[0] <= int_meip_i;
      int_meip_sync_q[1] <= int_meip_sync_q[0];
      int_seip_sync_q[0] <= int_seip_i;
      int_seip_sync_q[1] <= int_seip_sync_q[0];
    end
  end

  assign min_chicken_bits.rsv_1 = 1'b0;
  assign min_chicken_bits.rsv_0 = 1'b0;
  assign min_chicken_bits.min_frontend_clock_gate_disable =
      chicken_bits_vc_lo.min_frontend_clock_gate_disable;
  assign min_chicken_bits.min_dcache_clock_gate_disable =
      chicken_bits_vc_lo.min_dcache_clock_gate_disable;
  assign min_chicken_bits.min_vputrans_clock_gate_disable =
      chicken_bits_vc_lo.min_vputrans_clock_gate_disable;
  assign min_chicken_bits.min_vputima_clock_gate_disable =
      chicken_bits_vc_lo.min_vputima_clock_gate_disable;
  assign min_chicken_bits.min_vpulane_clock_gate_disable =
      chicken_bits_vc_lo.min_vpulane_clock_gate_disable;
  assign min_chicken_bits.min_intpipe_clock_gate_disable =
      chicken_bits_vc_lo.min_intpipe_clock_gate_disable;

  neigh_channel #(
    .EnableGfx(EnableGfx),
    .StubMinions('0)
  ) u_channel (
    .clk_i(clk_i),
    .clk_shire_i(clk_shire_i),
    .clk_minion_o(clk_minion),
    .rst_c_shire_ni(rst_c_shire_ni),
    .rst_d_shire_ni(rst_d_shire_ni),
    .rst_w_shire_ni(rst_w_shire_ni),
    .rst_warm_ni(rst_warm_ni),
    .rst_w_icache_no(reset_w_icache_pwrstub),
    .dft_hv_i(dft_hv_i),
    .dft_use_reset_cntl_hv_i(dft_use_reset_cntl_hv_i),
    .dft_reset_cntl_hv_i(dft_reset_cntl_hv_i),
    .dft_lv_o(dft_lv),
    .chicken_bits_vc_i(esr_clk_gate_ctrl_i),
    .bpam_run_control_i(bpam_run_control_i),
    .bpam_rc_tbox_ack_hi_o(bpam_rc_tbox_ack_hi_pwrstub),
    .dll_feedback_shire_o(dll_feedback_shire_o),
    .dll_feedback_neigh_o(dll_feedback_neigh_o),
    .shire_id_i(shire_id_i),
    .neigh_id_i(neigh_id_i),
    .shire_tbox_id_i(shire_tbox_id_i),
    .shire_tbox_en_i(shire_tbox_en_i),
    .esr_thread0_enable_i(esr_thread0_enable_i),
    .esr_thread1_enable_i(esr_thread1_enable_i),
    .esr_minion_features_i(esr_minion_features_i),
    .esr_icache_prefetch_conf_i(esr_icache_prefetch_conf_i),
    .esr_icache_prefetch_start_i(esr_icache_prefetch_start_i),
    .esr_icache_prefetch_done_o(esr_icache_prefetch_done_pwrstub),
    .esr_icache_err_detected_o(esr_icache_err_detected_pwrstub),
    .esr_icache_err_logged_o(esr_icache_err_logged_pwrstub),
    .dmctrl_i(dmctrl_i),
    .esr_and_or_tree_l0_o(esr_and_or_tree_l0_pwrstub),
    .esr_shire_coop_mode_i(esr_shire_coop_mode_i),
    .esr_minion_mem_override_i(esr_minion_mem_override_i),
    .esr_dll_dly_est_ctl_i(esr_dll_dly_est_ctl_i),
    .esr_dll_dly_est_sts_o(esr_dll_dly_est_sts_o),
    .neigh_sc_req_ready_i(neigh_sc_req_ready_pwrstub),
    .neigh_sc_req_valid_o(neigh_sc_req_valid_pwrstub),
    .neigh_sc_req_info_o(neigh_sc_req_info_o),
    .neigh_sc_rsp_ready_o(neigh_sc_rsp_ready_pwrstub),
    .neigh_sc_rsp_valid_i(neigh_sc_rsp_valid_pwrstub),
    .neigh_sc_rsp_info_i(neigh_sc_rsp_info_i),
    .hw_dbg_sm_monitor_enabled_i(hw_dbg_sm_monitor_enabled_i),
    .neigh_sm_gpio_i(neigh_sm_gpio_i),
    .neigh_sm_signals_o(neigh_sm_signals_o),
    .apb_req_from_shire_i(apb_esr_req_pwrstub),
    .apb_rsp_to_shire_o(apb_esr_rsp_pwrstub),
    .int_mtip_i(int_mtip_sync_q[1]),
    .int_meip_i(int_meip_sync_q[1]),
    .int_seip_i(int_seip_sync_q[1]),
    .ipi_msip_i(ipi_msip_i),
    .ipi_redirect_trigger_i(ipi_redirect_trigger_i),
    .uc_to_neigh_fcc_i(uc_to_neigh_fcc_i),
    .uc_to_neigh_fcc_target_i(uc_to_neigh_fcc_target_i),
    .flb_neigh_l2_req_valid_o(flb_neigh_l2_req_valid_pwrstub),
    .flb_neigh_l2_req_data_o(flb_neigh_l2_req_data_o),
    .flb_l2_neigh_resp_valid_i(flb_l2_neigh_resp_valid_pwrstub),
    .flb_l2_neigh_resp_data_i(flb_l2_neigh_resp_data_i),
    .icache_f2_sram_req_write_o(icache_f2_sram_req_write_pwrstub),
    .icache_f2_sram_req_addr_o(icache_f2_sram_req_addr_o),
    .icache_f2_sram_req_valid_o(icache_f2_sram_req_valid_pwrstub),
    .icache_f2_sram_req_ready_i(icache_f2_sram_req_ready_pwrstub),
    .icache_f0_sram_resp_dout_i(icache_f0_sram_resp_dout_i),
    .icache_f0_sram_resp_valid_i(icache_f0_sram_resp_valid_pwrstub),
    .icache_f0_sram_resp_ready_o(icache_f0_sram_resp_ready_pwrstub),
    .coop_tload_slv_rdy_out_data_o(coop_tload_slv_rdy_out_data_o),
    .coop_tload_slv_rdy_out_valid_o(coop_tload_slv_rdy_out_valid_pwrstub),
    .coop_tload_slv_rdy_in_data_i(coop_tload_slv_rdy_in_data_i),
    .coop_tload_slv_rdy_in_valid_i(coop_tload_slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id_o(coop_tload_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid_o(coop_tload_mst_done_out_valid_pwrstub),
    .coop_tload_mst_done_in_coop_id_i(coop_tload_mst_done_in_coop_id_i),
    .coop_tload_mst_done_in_valid_i(coop_tload_mst_done_in_valid_i),
    .rst_c_per_minion_no(reset_c_minion_n),
    .rst_d_per_minion_no(reset_d_minion_n),
    .rst_w_per_minion_no(reset_w_minion_n),
    .chicken_bits_vc_lo_o(chicken_bits_vc_lo),
    .shire_id_lo_o(shire_id_lo),
    .neigh_id_lo_o(neigh_id_lo),
    .shire_min_id_o(shire_min_id),
    .thread0_enable_o(thread0_enable),
    .thread1_enable_o(thread1_enable),
    .esr_minion_features_lo_o(esr_minion_features_lo),
    .esr_shire_coop_mode_lo_o(esr_shire_coop_mode_lo),
    .esr_minion_mem_override_lo_o(esr_minion_mem_override_lo),
    .esr_bypass_dcache_o(esr_bypass_dcache),
    .minion_boot_addr_o(minion_boot_addr),
    .mprot_o(mprot),
    .vmspagesize_o(vmspagesize),
    .interrupts_per_minion_o(interrupts_per_minion),
    .flb_core_neigh_req_valid_i(flb_neigh_req_valid),
    .flb_core_neigh_req_data_i(flb_neigh_req_data),
    .flb_neigh_core_resp_valid_o(flb_neigh_resp_valid),
    .flb_neigh_core_resp_data_o(flb_neigh_resp_data),
    .l2_dcache_evict_req_ready_o(l2_dcache_evict_req_ready),
    .l2_dcache_evict_req_valid_i(l2_dcache_evict_req_valid),
    .l2_dcache_evict_req_i(l2_dcache_evict_req),
    .l2_dcache_miss_req_ready_o(l2_dcache_miss_req_ready),
    .l2_dcache_miss_req_valid_i(l2_dcache_miss_req_valid),
    .l2_dcache_miss_req_i(l2_dcache_miss_req),
    .l2_dcache_resp_ready_i(l2_dcache_resp_ready),
    .l2_dcache_resp_valid_o(l2_dcache_resp_valid),
    .l2_dcache_resp_o(l2_dcache_resp),
    .icache_req_ready_o(icache_req_ready),
    .icache_req_valid_i(icache_req_valid),
    .icache_req_i(icache_req),
    .icache_resp_valid_o(icache_resp_valid),
    .icache_resp_miss_o(icache_resp_miss),
    .icache_resp_o(icache_resp),
    .icache_fill_done_o(icache_fill_done),
    .icache_flush_data_i(icache_flush_data),
    .satp_info_i(satp_info),
    .matp_info_i(matp_info),
    .tlb_invalidate_i(tlb_invalidate),
    .dc_ptw_req_data_i(dc_ptw_req_data),
    .dc_ptw_req_valid_i(dc_ptw_req_valid),
    .dc_ptw_req_ready_o(dc_ptw_req_ready),
    .ptw_dc_resp_data_o(ptw_dc_resp_data),
    .ptw_dc_resp_valid_o(ptw_dc_resp_valid),
    .minion_apb_prdata_i(minion_apb_prdata),
    .minion_apb_pready_i(minion_apb_pready),
    .minion_apb_pslverr_i(minion_apb_pslverr),
    .minion_apb_penable_o(minion_apb_penable),
    .minion_apb_psel_o(minion_apb_psel),
    .minion_apb_pwrite_o(minion_apb_pwrite),
    .minion_apb_paddr_o(minion_apb_paddr),
    .minion_apb_pwdata_o(minion_apb_pwdata),
    .debug_minion_in_o(debug_minion_in),
    .debug_minion_out_i(debug_minion_out),
    .minion_dbg_signals_en_o(minion_dbg_signals_en),
    .minion_dbg_signals_i(minion_dbg_signals),
    .minion_dbg_signals_mux_o(minion_dbg_signals_mux),
    .pmu_count_up_i(pmu_count_up),
    .pmu_read_data_o(pmu_read_data),
    .pmu_read_sel_i(pmu_read_sel),
    .pmu_write_en_i(pmu_write_en),
    .pmu_write_data_i(pmu_write_data),
    .pmu_neigh_event_sel_i(pmu_neigh_event_sel),
    .clk_tbox_o(clk_tbox_o),
    .tbox_rst_c_no(tbox_rst_c_no),
    .tbox_rst_d_no(tbox_rst_d_no),
    .tbox_rst_w_no(tbox_rst_w_no),
    .tbox_id_lo_o(tbox_id_lo_o),
    .tbox_control_esr_o(tbox_control_esr_o),
    .tbox_status_esr_i(EnableGfx ? tbox_status_esr_i : '0),
    .tbox_image_table_ptr_esr_o(tbox_image_table_ptr_esr_o),
    .tbox_neigh_req_ready_o(tbox_neigh_req_ready_o),
    .tbox_neigh_req_valid_i(EnableGfx ? tbox_neigh_req_valid_i : 1'b0),
    .tbox_neigh_req_info_i(EnableGfx ? tbox_neigh_req_info_i : '0),
    .tbox_neigh_rsp_ready_i(EnableGfx ? tbox_neigh_rsp_ready_i : 1'b0),
    .tbox_neigh_rsp_valid_o(tbox_neigh_rsp_valid_o),
    .tbox_neigh_rsp_info_o(tbox_neigh_rsp_info_o),
    .tbox_dbg_signals_en_o(tbox_dbg_signals_en_o),
    .tbox_dbg_signals_i(EnableGfx ? tbox_dbg_signals_i : '0),
    .tbox_dbg_signals_mux_o(tbox_dbg_signals_mux_channel),
    .tbox_apb_req_o(tbox_apb_req_o),
    .tbox_apb_rsp_i(EnableGfx ? tbox_apb_rsp_i : '1),
    .tbox_bpam_run_control_o(tbox_bpam_run_control_o),
    .tbox_bpam_run_control_ack_i(EnableGfx ? tbox_bpam_run_control_ack_i : '0)
  );

  prim_eco_ports #(
    .InputWidth(neigh_pkg::EtEcoNeighInputWidth),
    .OutputWidth(neigh_pkg::EtEcoNeighOutputWidth)
  ) u_eco_ports (
    .eco_i(eco_i),
    .eco_o(eco_o_pwrstub)
  );

  neigh_hv_logic_pwr_ctrl_tdr_isolate u_pwr_ctrl_tdr_isolate (
    .dft_tdr_ctrl_i(dft_use_reset_cntl_hv_i),
    .pwr_ctrl_glb_isolate_i(pwr_ctrl_glb_isolate_i),
    .pwr_ctrl_glb_isolate_o(pwr_ctrl_glb_isolate_int)
  );

  vclevel_shft_h2l #(
    .WIDTH(2 + 2 * MinPerN)
  ) u_ls_h2l_pwr_ctrl (
    .inp_hv({pwr_ctrl_glb_nsleepin_i, pwr_ctrl_glb_isolate_int,
             pwr_ctrl_min_nsleepin_i, pwr_ctrl_min_isolate_i}),
    .out_lv({pwr_ctrl_glb_nsleepin_lv, pwr_ctrl_glb_isolate_lv,
             pwr_ctrl_min_nsleepin_lv, pwr_ctrl_min_isolate_lv})
  );

  assign pwr_ctrl_glb_nsleepout_lv = pwr_ctrl_glb_nsleepin_lv & (&pwr_ctrl_min_nsleepout_lv);

  neigh_top_pwrstub u_pwrstub (
    .pwr_ctrl_glb_isolate_i(pwr_ctrl_glb_isolate_int),
    .coop_tload_slv_rdy_out_valid_i(coop_tload_slv_rdy_out_valid_pwrstub),
    .coop_tload_slv_rdy_out_valid_o(coop_tload_slv_rdy_out_valid_o),
    .coop_tload_mst_done_out_valid_i(coop_tload_mst_done_out_valid_pwrstub),
    .coop_tload_mst_done_out_valid_o(coop_tload_mst_done_out_valid_o),
    .apb_esr_req_i(apb_esr_req_i),
    .apb_esr_req_o(apb_esr_req_pwrstub),
    .apb_esr_rsp_i(apb_esr_rsp_pwrstub),
    .apb_esr_rsp_o(apb_esr_rsp_o),
    .neigh_sc_req_ready_i(neigh_sc_req_ready_i),
    .neigh_sc_req_ready_o(neigh_sc_req_ready_pwrstub),
    .neigh_sc_req_valid_i(neigh_sc_req_valid_pwrstub),
    .neigh_sc_req_valid_o(neigh_sc_req_valid_o),
    .neigh_sc_rsp_ready_i(neigh_sc_rsp_ready_pwrstub),
    .neigh_sc_rsp_valid_i(neigh_sc_rsp_valid_i),
    .neigh_sc_rsp_ready_o(neigh_sc_rsp_ready_o),
    .neigh_sc_rsp_valid_o(neigh_sc_rsp_valid_pwrstub),
    .esr_icache_prefetch_done_i(esr_icache_prefetch_done_pwrstub),
    .esr_icache_err_detected_i(esr_icache_err_detected_pwrstub),
    .esr_icache_err_logged_i(esr_icache_err_logged_pwrstub),
    .esr_icache_prefetch_done_o(esr_icache_prefetch_done_o),
    .esr_icache_err_detected_o(esr_icache_err_detected_o),
    .esr_icache_err_logged_o(esr_icache_err_logged_o),
    .icache_f0_sram_resp_valid_i(icache_f0_sram_resp_valid_i),
    .icache_f0_sram_resp_ready_i(icache_f0_sram_resp_ready_pwrstub),
    .icache_f0_sram_resp_valid_o(icache_f0_sram_resp_valid_pwrstub),
    .icache_f0_sram_resp_ready_o(icache_f0_sram_resp_ready_o),
    .esr_and_or_tree_l0_i(esr_and_or_tree_l0_pwrstub),
    .esr_and_or_tree_l0_o(esr_and_or_tree_l0_o),
    .flb_neigh_l2_req_valid_i(flb_neigh_l2_req_valid_pwrstub),
    .flb_l2_neigh_resp_valid_i(flb_l2_neigh_resp_valid_i),
    .flb_neigh_l2_req_valid_o(flb_neigh_l2_req_valid_o),
    .flb_l2_neigh_resp_valid_o(flb_l2_neigh_resp_valid_pwrstub),
    .icache_f2_sram_req_write_i(icache_f2_sram_req_write_pwrstub),
    .icache_f2_sram_req_valid_i(icache_f2_sram_req_valid_pwrstub),
    .icache_f2_sram_req_ready_i(icache_f2_sram_req_ready_i),
    .icache_f2_sram_req_write_o(icache_f2_sram_req_write_o),
    .icache_f2_sram_req_valid_o(icache_f2_sram_req_valid_o),
    .icache_f2_sram_req_ready_o(icache_f2_sram_req_ready_pwrstub),
    .bpam_rc_tbox_ack_hi_i(bpam_rc_tbox_ack_hi_pwrstub),
    .bpam_rc_tbox_ack_hi_o(bpam_rc_tbox_ack_hi_o),
    .reset_w_icache_i(!reset_w_icache_pwrstub),
    .reset_w_icache_o(reset_w_icache_pwrstub_out),
    .eco_o_i(eco_o_pwrstub),
    .eco_o_o(eco_o),
    .pwr_ctrl_glb_nsleepout_i(pwr_ctrl_glb_nsleepout_lv),
    .pwr_ctrl_min_nsleepout_i(pwr_ctrl_min_nsleepout_lv),
    .pwr_ctrl_glb_nsleepout_o(pwr_ctrl_glb_nsleepout_o),
    .pwr_ctrl_min_nsleepout_o(pwr_ctrl_min_nsleepout_o)
  );

  assign rst_w_icache_no = !reset_w_icache_pwrstub_out;
  assign voltage_monitor_vdd_o = 1'b1;
  assign voltage_monitor_vss_o = 1'b0;

  assign dft_scanout_hv_o = '0;
  assign dft_occ_scanout_hv_o = 1'b0;

  if (EnableGfx) begin : gen_gfx_dft
    assign tbox_dbg_signals_mux_o = tbox_dbg_signals_mux_channel;
    assign dft_scanin_to_tbox_lv_o = dft_scanin_tbox_hv_i;
    assign dft_scanout_tbox_hv_o = dft_scanout_to_tbox_lv_i;
    assign dft_tbox_lv_o.scanmode = dft_lv.scanmode;
    assign dft_tbox_lv_o.scan_reset_n = dft_lv.scan_reset_n;
    assign dft_tbox_lv_o.sram_clk_override = dft_hv_i.sram_clk_override;
    assign dft_tbox_lv_o.ram_rei = dft_hv_i.ram_rei;
    assign dft_tbox_lv_o.ram_wei = dft_hv_i.ram_wei;
    assign dft_scan_enable_to_tbox_lv_o = dft_scan_enable_hv_i;
    assign dft_test_mode_to_tbox_lv_o = dft_test_mode_hv_i;
    assign dft_clock_gate_en_to_tbox_lv_o = dft_clock_gate_en_hv_i;
    assign dft_cntl_to_tbox_lv_o = dft_cntl_hv_i;
    assign dft_occ_scanin_to_tbox_lv_o = dft_occ_scanin_tbox_hv_i;
    assign dft_occ_scanout_tbox_hv_o = dft_occ_scanout_to_tbox_lv_i;
    assign dft_occ_reset_to_tbox_lv_o = dft_occ_reset_hv_i;
    assign dft_occ_testmode_to_tbox_lv_o = dft_occ_testmode_hv_i;
    assign dft_scan_ate_clk_to_tbox_lv_o = dft_scan_ate_clk_hv_i;
    assign dft_occ_bypass_to_tbox_lv_o = dft_occ_bypass_hv_i;
  end else begin : gen_no_gfx_dft
    assign tbox_dbg_signals_mux_o = '0;
    assign dft_scanin_to_tbox_lv_o = '0;
    assign dft_scanout_tbox_hv_o = '0;
    assign dft_tbox_lv_o = '0;
    assign dft_scan_enable_to_tbox_lv_o = 1'b0;
    assign dft_test_mode_to_tbox_lv_o = '0;
    assign dft_clock_gate_en_to_tbox_lv_o = 1'b0;
    assign dft_cntl_to_tbox_lv_o = '0;
    assign dft_occ_scanin_to_tbox_lv_o = 1'b0;
    assign dft_occ_scanout_tbox_hv_o = 1'b0;
    assign dft_occ_reset_to_tbox_lv_o = 1'b0;
    assign dft_occ_testmode_to_tbox_lv_o = 1'b0;
    assign dft_scan_ate_clk_to_tbox_lv_o = 1'b0;
    assign dft_occ_bypass_to_tbox_lv_o = 1'b0;
  end

  genvar gen_it;
  generate
    for (gen_it = 0; gen_it < MinPerN; gen_it++) begin : gen_minions
      localparam int unsigned IcacheReqIdx = gen_it / IcacheMinPerReq;
      localparam int unsigned MinPerPtwIdx = gen_it % MinPerPtw;
      localparam int unsigned PtwIdx = gen_it / MinPerPtw;

      if ((StubMinions[gen_it] == 1'b0) && (DisableMinions[gen_it] == 1'b0)) begin : gen_real_minion
        logic [minion_pkg::NrThreads-1:0] thread_enables;
        minion_pkg::trace_encoder_signals_t trace_encoder_unused;
        logic [minion_pkg::EtEcoMinionOutputWidth-1:0] minion_eco_o_unused;

        assign thread_enables = {thread1_enable[gen_it], thread0_enable[gen_it]};
        assign minion_nsleepin[gen_it] = pwr_ctrl_glb_nsleepin_lv & pwr_ctrl_min_nsleepin_lv[gen_it];
        assign minion_iso_enable[gen_it] = pwr_ctrl_glb_isolate_lv | pwr_ctrl_min_isolate_lv[gen_it];
        assign pwr_ctrl_min_nsleepout_lv[gen_it] = minion_nsleepout[gen_it];

        minion_top u_minion (
          .clk_i(clk_minion),
          .rst_c_ni(reset_c_minion_n[gen_it]),
          .rst_d_ni(reset_d_minion_n[gen_it]),
          .rst_w_ni(reset_w_minion_n[gen_it]),
          .dft_i(dft_lv),
          .eco_i('0),
          .eco_o(minion_eco_o_unused),
          .ioshire_i(1'b0),
          .nsleepin_i(minion_nsleepin[gen_it]),
          .iso_enable_i(minion_iso_enable[gen_it]),
          .nsleepout_o(minion_nsleepout[gen_it]),
          .chicken_bits_i(min_chicken_bits),
          .l2_dcache_evict_req_ready_i(l2_dcache_evict_req_ready[gen_it]),
          .l2_dcache_evict_req_valid_o(l2_dcache_evict_req_valid[gen_it]),
          .l2_dcache_evict_req_o(l2_dcache_evict_req[gen_it]),
          .l2_dcache_miss_req_ready_i(l2_dcache_miss_req_ready[gen_it]),
          .l2_dcache_miss_req_valid_o(l2_dcache_miss_req_valid[gen_it]),
          .l2_dcache_miss_req_o(l2_dcache_miss_req[gen_it]),
          .l2_dcache_resp_ready_o(l2_dcache_resp_ready[gen_it]),
          .l2_dcache_resp_valid_i(l2_dcache_resp_valid[gen_it]),
          .l2_dcache_resp_i(l2_dcache_resp[gen_it]),
          .icache_req_ready_i(icache_req_ready[gen_it]),
          .icache_req_valid_o(icache_req_valid[gen_it]),
          .icache_req_o(icache_req[gen_it]),
          .icache_resp_valid_i(icache_resp_valid[IcacheReqIdx]),
          .icache_resp_miss_i(icache_resp_miss[IcacheReqIdx]),
          .icache_resp_i(icache_resp[IcacheReqIdx]),
          .icache_fill_done_i(icache_fill_done[IcacheReqIdx]),
          .icache_flush_data_o(icache_flush_data[gen_it]),
          .satp_info_o(satp_info[gen_it]),
          .matp_info_o(matp_info[gen_it]),
          .tlb_invalidate_o(tlb_invalidate[gen_it]),
          .dc_ptw_req_data_o(dc_ptw_req_data[PtwIdx][MinPerPtwIdx]),
          .dc_ptw_req_valid_o(dc_ptw_req_valid[PtwIdx][MinPerPtwIdx]),
          .dc_ptw_req_ready_i(dc_ptw_req_ready[PtwIdx][MinPerPtwIdx]),
          .ptw_dc_resp_data_i(ptw_dc_resp_data[PtwIdx]),
          .ptw_dc_resp_valid_i(ptw_dc_resp_valid[PtwIdx][MinPerPtwIdx]),
          .interrupts_i(interrupts_per_minion[gen_it]),
          .shire_id_i(shire_id_lo),
          .shire_min_id_i(shire_min_id[gen_it]),
          .enabled_i(thread_enables),
          .reset_vector_i(minion_boot_addr),
          .mprot_i(mprot),
          .vmspagesize_i(vmspagesize),
          .flb_neigh_req_valid_o(flb_neigh_req_valid[gen_it]),
          .flb_neigh_req_data_o(flb_neigh_req_data[gen_it]),
          .flb_neigh_resp_valid_i(flb_neigh_resp_valid[gen_it]),
          .flb_neigh_resp_data_i(flb_neigh_resp_data),
          .te_thread_sel_i(1'b0),
          .trace_encoder_o(trace_encoder_unused),
          .te_enable_i(1'b0),
          .apb_paddr_i(minion_apb_paddr),
          .apb_penable_i(minion_apb_penable),
          .apb_prdata_o(minion_apb_prdata[gen_it]),
          .apb_pready_o(minion_apb_pready[gen_it]),
          .apb_psel_i(minion_apb_psel[gen_it]),
          .apb_pslverr_o(minion_apb_pslverr[gen_it]),
          .apb_pwdata_i(minion_apb_pwdata),
          .apb_pwrite_i(minion_apb_pwrite),
          .debug_in_i(debug_minion_in[gen_it]),
          .debug_out_o(debug_minion_out[gen_it]),
          .minion_dbg_signals_o(minion_dbg_signals[gen_it]),
          .minion_dbg_signals_mux_i(minion_dbg_signals_mux[gen_it]),
          .minion_dbg_sig_enable_i(minion_dbg_signals_en[gen_it]),
          .esr_features_i(esr_minion_features_lo),
          .esr_bypass_dcache_i(esr_bypass_dcache),
          .esr_shire_coop_mode_i(esr_shire_coop_mode_lo),
          .esr_minion_mem_override_i(esr_minion_mem_override_lo),
          .pmu_count_up_o(pmu_count_up[gen_it]),
          .pmu_read_data_i(pmu_read_data[gen_it]),
          .pmu_read_sel_o(pmu_read_sel[gen_it]),
          .pmu_write_en_o(pmu_write_en[gen_it]),
          .pmu_write_data_o(pmu_write_data[gen_it]),
          .pmu_neigh_event_sel_o(pmu_neigh_event_sel[gen_it])
        );
      end else begin : gen_stub_minion
        assign minion_nsleepin[gen_it] = pwr_ctrl_glb_nsleepin_lv & pwr_ctrl_min_nsleepin_lv[gen_it];
        assign minion_iso_enable[gen_it] = pwr_ctrl_glb_isolate_lv | pwr_ctrl_min_isolate_lv[gen_it];
        assign minion_nsleepout[gen_it] = 1'b0;
        assign pwr_ctrl_min_nsleepout_lv[gen_it] = minion_nsleepout[gen_it];
        assign l2_dcache_evict_req_valid[gen_it] = '0;
        assign l2_dcache_miss_req_valid[gen_it] = '0;
        assign l2_dcache_evict_req[gen_it] = '0;
        assign l2_dcache_miss_req[gen_it] = '0;
        assign l2_dcache_resp_ready[gen_it] = 1'b0;
        assign icache_req_valid[gen_it] = '0;
        assign icache_req[gen_it] = '0;
        assign icache_flush_data[gen_it] = 1'b0;
        assign satp_info[gen_it] = '0;
        assign matp_info[gen_it] = '0;
        assign tlb_invalidate[gen_it] = 1'b0;
        assign dc_ptw_req_data[PtwIdx][MinPerPtwIdx] = '0;
        assign dc_ptw_req_valid[PtwIdx][MinPerPtwIdx] = 1'b0;
        assign minion_apb_prdata[gen_it] = '0;
        assign minion_apb_pready[gen_it] = 1'b0;
        assign minion_apb_pslverr[gen_it] = 1'b0;
        assign debug_minion_out[gen_it] = '0;
        assign minion_dbg_signals[gen_it] = '0;
        assign flb_neigh_req_valid[gen_it] = 1'b0;
        assign flb_neigh_req_data[gen_it] = '0;
        assign pmu_count_up[gen_it] = '0;
        assign pmu_read_sel[gen_it] = '0;
        assign pmu_write_en[gen_it] = '0;
        assign pmu_write_data[gen_it] = '0;
        assign pmu_neigh_event_sel[gen_it] = '0;
      end
    end
  endgenerate

endmodule : neigh_top
/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on UNUSEDSIGNAL */
