// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNOPTFLAT */  // Large translated integration fabric has packed-struct muxes and handshake paths that are acyclic before flattening.
/* verilator lint_off VARHIDDEN */  // Original CORE-ET names are preserved as local aliases and intentionally shadow package constants.
/* verilator lint_off PINCONNECTEMPTY */  // Legacy FIFO/debug helper outputs are intentionally unused at these integration sites.
/* verilator lint_off WIDTHEXPAND */  // Debug-mux packing intentionally zero-extends narrow status vectors into fixed-width debug buses.
/* verilator lint_off UNUSEDSIGNAL */  // Translated debug/status structs carry many fields that this integration layer only partially observes.
/* verilator lint_off UNUSEDPARAM */  // Stub/geometry parameters are preserved for top-level compatibility even when this shell does not consume them directly.
module neigh_channel
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
  parameter bit EnableGfx = 1'b0,
  parameter logic [neigh_pkg::MinPerN-1:0] StubMinions = '0
) (
  input  logic                                             clk_i,
  input  logic                                             clk_shire_i,
  output logic                                             clk_minion_o,

  input  logic                                             rst_c_shire_ni,
  input  logic                                             rst_d_shire_ni,
  input  logic                                             rst_w_shire_ni,
  input  logic                                             rst_warm_ni,
  output logic                                             rst_w_icache_no,

  input  dft_t                                             dft_hv_i,
  input  logic                                             dft_use_reset_cntl_hv_i,
  input  neigh_pkg::shire_tdr_reset_cntl_t                 dft_reset_cntl_hv_i,
  output dft_t                                             dft_lv_o,

  input  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t chicken_bits_vc_i,

  input  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control_i,
  output neigh_hv_logic_pkg::bpam_rc_tbox_ack_t            bpam_rc_tbox_ack_hi_o,

  output logic                                             dll_feedback_shire_o,
  output logic                                             dll_feedback_neigh_o,

  input  logic [minion_pkg::NumShireIdsBits-1:0]           shire_id_i,
  input  logic [neigh_pkg::NumNeighWidth-1:0]              neigh_id_i,
  input  logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0]          shire_tbox_id_i,
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
  input  logic [minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_i,

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

  input  neigh_ch_apb_mux_pkg::apb_to_neigh_t              apb_req_from_shire_i,
  output neigh_ch_apb_mux_pkg::apb_from_neigh_t            apb_rsp_to_shire_o,

  input  logic [neigh_pkg::MinPerN-1:0]                    int_mtip_i,
  input  logic                                             int_meip_i,
  input  logic                                             int_seip_i,
  input  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0]                ipi_msip_i,
  input  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0]                ipi_redirect_trigger_i,
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

  output neigh_pkg::coop_tload_slv_rdy_req_t               coop_tload_slv_rdy_out_data_o,
  output logic                                             coop_tload_slv_rdy_out_valid_o,
  input  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_data_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]              coop_tload_slv_rdy_in_valid_i,
  output logic [neigh_pkg::ShireCoopIdSize-1:0]            coop_tload_mst_done_out_coop_id_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0]              coop_tload_mst_done_out_valid_o,
  input  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]              coop_tload_mst_done_in_valid_i,

  output logic [neigh_pkg::MinPerN-1:0]                    rst_c_per_minion_no,
  output logic [neigh_pkg::MinPerN-1:0]                    rst_d_per_minion_no,
  output logic [neigh_pkg::MinPerN-1:0]                    rst_w_per_minion_no,
  output neigh_voltage_cross_pkg::minion_chicken_bits_vc_t chicken_bits_vc_lo_o,
  output logic [minion_pkg::NumShireIdsBits-1:0]           shire_id_lo_o,
  output logic [neigh_pkg::NumNeighWidth-1:0]              neigh_id_lo_o,
  output logic [neigh_pkg::MinPerN-1:0][minion_pkg::MinPerSBits-1:0] shire_min_id_o,
  output logic [neigh_pkg::MinPerN-1:0]                    thread0_enable_o,
  output logic [neigh_pkg::MinPerN-1:0]                    thread1_enable_o,
  output minion_pkg::esr_minion_features_t                 esr_minion_features_lo_o,
  output logic                                             esr_shire_coop_mode_lo_o,
  output logic [minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_lo_o,
  output logic                                             esr_bypass_dcache_o,
  output logic [minion_pkg::VaSize-1:0]                    minion_boot_addr_o,
  output esr_pkg::esr_mprot_t                              mprot_o,
  output minion_pkg::tlb_entry_type                        vmspagesize_o,
  output minion_pkg::minion_interrupts [neigh_pkg::MinPerN-1:0] interrupts_per_minion_o,

  input  logic [neigh_pkg::MinPerN-1:0]                    flb_core_neigh_req_valid_i,
  input  logic [neigh_pkg::MinPerN-1:0][neigh_voltage_cross_pkg::NvcCsrFlBarrierSize-1:0] flb_core_neigh_req_data_i,
  output logic [neigh_pkg::MinPerN-1:0]                    flb_neigh_core_resp_valid_o,
  output logic                                             flb_neigh_core_resp_data_o,

  output logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_ready_o,
  input  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_valid_i,
  input  minion_dcache_pkg::et_link_minion_evict_req_info_t [neigh_pkg::MinPerN-1:0] l2_dcache_evict_req_i,
  output logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] l2_dcache_miss_req_ready_o,
  input  logic [neigh_pkg::MinPerN-1:0][minion_pkg::DcacheL2MissReqPorts-1:0] l2_dcache_miss_req_valid_i,
  input  minion_dcache_pkg::et_link_minion_miss_req_info_t [neigh_pkg::MinPerN-1:0] l2_dcache_miss_req_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    l2_dcache_resp_ready_i,
  output logic [neigh_pkg::MinPerN-1:0]                    l2_dcache_resp_valid_o,
  output minion_pkg::et_link_minion_rsp_info_t [neigh_pkg::MinPerN-1:0] l2_dcache_resp_o,

  output logic [neigh_pkg::MinPerN-1:0]                    icache_req_ready_o,
  input  logic [neigh_pkg::MinPerN-1:0]                    icache_req_valid_i,
  input  icache_pkg::frontend_icache_req_t [neigh_pkg::MinPerN-1:0] icache_req_i,
  output logic [icache_pkg::IcacheNrReqs-1:0]              icache_resp_valid_o,
  output logic [icache_pkg::IcacheNrReqs-1:0]              icache_resp_miss_o,
  output icache_pkg::icache_frontend_resp_t [icache_pkg::IcacheNrReqs-1:0] icache_resp_o,
  output logic [icache_pkg::IcacheNrReqs-1:0]              icache_fill_done_o,
  input  logic [neigh_pkg::MinPerN-1:0]                    icache_flush_data_i,

  input  minion_pkg::minion_satp_info [neigh_pkg::MinPerN-1:0] satp_info_i,
  input  minion_pkg::minion_satp_info [neigh_pkg::MinPerN-1:0] matp_info_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    tlb_invalidate_i,
  input  minion_pkg::minion_ptw_req [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dc_ptw_req_data_i,
  input  logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dc_ptw_req_valid_i,
  output logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] dc_ptw_req_ready_o,
  output minion_pkg::minion_ptw_pte [neigh_pkg::PtwPerN-1:0] ptw_dc_resp_data_o,
  output logic [neigh_pkg::PtwPerN-1:0][neigh_pkg::MinPerPtw-1:0] ptw_dc_resp_valid_o,

  input  logic [neigh_pkg::MinPerN-1:0][esr_pkg::EsrApbDataWidth-1:0] minion_apb_prdata_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    minion_apb_pready_i,
  input  logic [neigh_pkg::MinPerN-1:0]                    minion_apb_pslverr_i,
  output logic                                             minion_apb_penable_o,
  output logic [neigh_pkg::MinPerN-1:0]                    minion_apb_psel_o,
  output logic                                             minion_apb_pwrite_o,
  output logic [minion_pkg::MinDmApbAddrWidth-1:0]         minion_apb_paddr_o,
  output logic [esr_pkg::EsrApbDataWidth-1:0]              minion_apb_pwdata_o,

  output minion_pkg::minion_debug_in_t [neigh_pkg::MinPerN-1:0] debug_minion_in_o,
  input  minion_pkg::minion_debug_out_t [neigh_pkg::MinPerN-1:0] debug_minion_out_i,
  output logic [neigh_pkg::MinPerN-1:0]                    minion_dbg_signals_en_o,
  input  minion_pkg::neigh_sm_dbg_monitor_t [neigh_pkg::MinPerN-1:0] minion_dbg_signals_i,
  output logic [neigh_pkg::MinPerN-1:0][minion_pkg::NeighDebugSmMuxWidth-1:0] minion_dbg_signals_mux_o,

  input  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuMinionCountersNum-1:0] pmu_count_up_i,
  output logic [neigh_pkg::MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::XregSize-1:0] pmu_read_data_o,
  input  logic [neigh_pkg::MinPerN-1:0][minion_pkg::NrThreads-1:0][minion_pkg::PmuCountersSelectBits-1:0] pmu_read_sel_i,
  input  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuTotalCountersNum-1:0] pmu_write_en_i,
  input  logic [neigh_pkg::MinPerN-1:0][minion_pkg::XregSize-1:0] pmu_write_data_i,
  input  logic [neigh_pkg::MinPerN-1:0][minion_pkg::PmuNeighEventCntSelBits-1:0] pmu_neigh_event_sel_i,

  output logic                                             clk_tbox_o,
  output logic                                             tbox_rst_c_no,
  output logic                                             tbox_rst_d_no,
  output logic                                             tbox_rst_w_no,
  output logic [neigh_voltage_cross_pkg::TboxPerShireBits-1:0]          tbox_id_lo_o,
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
  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t            tbox_bpam_run_control_ack_i
);

  typedef etlink_pkg::neigh_req_t             et_link_neigh_req_info_t;
  typedef etlink_pkg::req_nodata_t            et_link_nodata_req_info_t;
  typedef etlink_pkg::req_t                   et_link_req_info_t;
  typedef etlink_pkg::rsp_t                   et_link_rsp_info_t;
  typedef etlink_pkg::neigh_rsp_t             et_link_neigh_rsp_info_t;
  typedef etlink_pkg::rsp_opcode_e            et_link_rsp_opcode_t;
  typedef icache_pkg::frontend_icache_req_t frontend_icache_req;
  typedef icache_pkg::icache_frontend_resp_t icache_frontend_resp;
  typedef neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_req_t;
  typedef neigh_pkg::coop_tload_tag_table_data_t coop_tload_tag_table_data_t;
  typedef neigh_pkg::ptw_dbg_sm_t ptw_dbg_sm_t;
  typedef neigh_pkg::coop_tload_dbg_sm_t coop_tload_dbg_sm_t;
  typedef neigh_pkg::tstore_dbg_sm_t tstore_dbg_sm_t;
  typedef neigh_pkg::fln_dbg_sm_t fln_dbg_sm_t;
  typedef neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control_neigh_t;
  typedef neigh_voltage_cross_pkg::minion_chicken_bits_vc_t minion_chicken_bits_vc_t;
  typedef neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_t;
  typedef neigh_ch_apb_mux_pkg::apb_to_neigh_t APB_to_neigh_t;
  typedef neigh_ch_apb_mux_pkg::apb_from_neigh_t APB_from_neigh_t;
  typedef neigh_ch_apb_mux_pkg::apb_to_neigh_esrs_t APB_to_neigh_esrs_t;
  typedef neigh_ch_apb_mux_pkg::apb_to_neigh_icache_t APB_to_neigh_icache_t;
  typedef neigh_ch_apb_mux_pkg::apb_from_neigh_channel_t APB_from_neigh_channel_t;
  typedef esr_pkg::esr_and_or_tree_l0_t esr_and_or_tree_L0_t;

  localparam int unsigned ScBanks = shirecache_pkg::Banks;
  localparam int unsigned MinPerN = neigh_pkg::MinPerN;
  localparam int unsigned MinPerNW = (MinPerN <= 1) ? 1 : $clog2(MinPerN);
  localparam int unsigned MinPerSW = minion_pkg::MinPerSBits;
  localparam int unsigned NumShireIdsW = minion_pkg::NumShireIdsBits;
  localparam int unsigned NumNeigh = neigh_pkg::NumNeigh;
  localparam int unsigned NumNeighW = neigh_pkg::NumNeighWidth;
  localparam int unsigned TboxPerShireW = neigh_voltage_cross_pkg::TboxPerShireBits;
  localparam int unsigned MinPerIcache = MinPerN;
  localparam int unsigned IcachePerN = neigh_pkg::IcachePerN;
  localparam int unsigned MinPerPtw = neigh_pkg::MinPerPtw;
  localparam int unsigned PtwPerN = neigh_pkg::PtwPerN;
  localparam int unsigned PtwPerNW = (PtwPerN <= 1) ? 1 : $clog2(PtwPerN);
  localparam int unsigned NeighNumRespAgents = neigh_pkg::NeighNumRespAgents;
  localparam int unsigned NeighNumReqAgents = MinPerN + IcachePerN + PtwPerN + 3;
  localparam int unsigned NeighAgentMinion = neigh_pkg::NeighAgentMinion;
  localparam int unsigned NeighAgentIcache = neigh_pkg::NeighAgentIcache;
  localparam int unsigned NeighAgentPtw = neigh_pkg::NeighAgentPtw;
  localparam int unsigned NeighAgentTbox = neigh_pkg::NeighAgentTbox;
  localparam int unsigned NeighAgentTload = MinPerN + IcachePerN + PtwPerN + 1;
  localparam int unsigned NeighAgentTstore = MinPerN + IcachePerN + PtwPerN + 2;
  localparam int unsigned NeighIntFifoSize = 2;
  localparam int unsigned NeighIntFifoSizeW = $clog2(NeighIntFifoSize);
  localparam int unsigned NeighBankFifoSize = 3;
  localparam int unsigned NeighMaxLocalConnect = neigh_pkg::NeighMaxLocalConnect;
  localparam int unsigned ShireCoopMissReqs = neigh_pkg::ShireCoopMissReqs;
  localparam int unsigned ShireCoopIdSize = neigh_pkg::ShireCoopIdSize;
  localparam int unsigned ShireCoopLoadIdSize = neigh_pkg::ShireCoopLoadIdSize;
  localparam int unsigned ThreadsPerN = neigh_hv_logic_pkg::ThreadsPerN;
  localparam int unsigned UcFccTargetSize = neigh_voltage_cross_pkg::UcFccTargetSize;
  localparam int unsigned ShireHiVoltageFifoSize = 3;
  localparam int unsigned ShireLoVoltageFifoSize = 3;
  localparam int unsigned NeighDataSize = etlink_pkg::NeighDataSize;
  localparam int unsigned UcDataSize = etlink_pkg::NeighDataSize;
  localparam int unsigned CsrFlBarrierSize = neigh_voltage_cross_pkg::NvcCsrFlBarrierSize;
  localparam int unsigned IcacheSramAddrWidth = icache_geom_pkg::IcacheSramAddrWidth;
  localparam int unsigned IcacheSramDataWidth = icache_geom_pkg::IcacheSramDataWidth;
  localparam int unsigned IcacheNrReqs = icache_pkg::IcacheNrReqs;
  localparam int unsigned IcacheNrReqsW = (IcacheNrReqs <= 1) ? 1 : $clog2(IcacheNrReqs);
  localparam int unsigned DcacheL2EvictReqPorts = minion_pkg::DcacheL2EvictReqPorts;
  localparam int unsigned DcacheL2MissReqPorts = minion_pkg::DcacheL2MissReqPorts;
  localparam int unsigned EsrApbDataWidth = esr_pkg::EsrApbDataWidth;
  localparam int unsigned EsrNeighApbPpAddrWidth = esr_pkg::EsrNeighApbPpAddrWidth;
  localparam int unsigned EsrMinionFeatureSize = $bits(minion_pkg::esr_minion_features_t);
  localparam int unsigned MinDmApbAddrWidth = minion_pkg::MinDmApbAddrWidth;
  localparam int unsigned ShireMinionMemOverrideBits = minion_pkg::ShireMinionMemOverrideBits;
  localparam int unsigned PaSize = etlink_pkg::PaSize;
  localparam int unsigned VaSize = minion_pkg::VaSize;
  localparam int unsigned PcSize = minion_pkg::PcSize;
  localparam int unsigned XregSize = minion_pkg::XregSize;
  localparam int unsigned IdSize = etlink_pkg::IdSize;
  localparam int unsigned PmuNeighEvents = minion_pkg::PmuNeighEvents;
  localparam int unsigned PmuMinionCountersNum = minion_pkg::PmuMinionCountersNum;
  localparam int unsigned PmuTotalCountersNum = minion_pkg::PmuTotalCountersNum;
  localparam int unsigned PmuCountersSelectBits = minion_pkg::PmuCountersSelectBits;
  localparam int unsigned PmuNeighEventCntSelBits = minion_pkg::PmuNeighEventCntSelBits;
  localparam int unsigned NeighDebugMatchWidth = minion_pkg::NeighDebugMatchWidth;
  localparam int unsigned NeighDebugFilterWidth = minion_pkg::NeighDebugFilterWidth;
  localparam int unsigned NeighDebugDataWidth = minion_pkg::NeighDebugDataWidth;
  localparam int unsigned NeighDebugSmMuxWidth = minion_pkg::NeighDebugSmMuxWidth;
  localparam int unsigned NeighDebugSmMuxModWidth = minion_pkg::NeighDebugSmMuxModWidth;
  localparam int unsigned NeighDebugSmMuxDataWidth = minion_pkg::NeighDebugSmMuxDataWidth;
  localparam logic [NeighDebugSmMuxModWidth-1:0] NeighChDebugSmModMin4 = 5'd4;
  localparam logic [NeighDebugSmMuxModWidth-1:0] NeighChDebugSmModIcache = 5'd8;
  localparam logic [NeighDebugSmMuxModWidth-1:0] NeighChDebugSmModTload = 5'd10;
  localparam logic [NeighDebugSmMuxModWidth-1:0] NeighChDebugSmModFln = 5'd12;
  localparam logic [NeighDebugSmMuxModWidth-1:0] NeighChDebugSmModTbox = 5'd13;
  localparam logic [NeighDebugSmMuxModWidth-1:0] NeighChDebugSmModDpath = 5'd14;
  localparam logic NeighChDebugSmTloadDataExt = 1'b0;
  localparam logic [1:0] NeighForceAgentDcache = 2'd0;
  localparam logic [1:0] NeighForceAgentIcache = 2'd1;
  localparam logic [1:0] NeighForceAgentTbox = 2'd2;
  localparam logic [1:0] NeighForceAgentPtw = 2'd3;
  localparam logic [1:0] NeighForceDestFifoBanks = 2'd0;
  localparam logic [1:0] NeighForceDestFifoUc = 2'd1;
  localparam int unsigned ScBankIdSize = shirecache_pkg::BankIdSize;
  localparam int unsigned ScBankLsb = shirecache_pkg::BankLsb;
  localparam int unsigned NeighSmGpioWidth = neigh_voltage_cross_pkg::NeighSmGpioWidth;
  localparam int unsigned OutFifos = ScBanks + 1;
  localparam int unsigned OutFifosW = $clog2(OutFifos);
  localparam int unsigned UcFifo = ScBanks;
  localparam int unsigned DestFifos = OutFifos + 1;
  localparam int unsigned DestFifosW = $clog2(DestFifos);
  localparam int unsigned TboxFifoInt = OutFifos;
  localparam logic [DestFifosW-1:0] TboxFifo = TboxFifoInt[DestFifosW-1:0];
  localparam logic [DestFifosW-1:0] UcFifoDest = UcFifo[DestFifosW-1:0];
  localparam logic [4:0] EtLinkCoreMissIdTensorStoreCoop = 5'b00001;
  localparam int unsigned EtAtomicDestStart = 6;
  localparam logic EtAtomicDestGlobal = 1'b1;
  localparam int unsigned AdEsrRegionStart = 32;
  localparam int unsigned AdEsrRegionSize = PaSize - AdEsrRegionStart;
  localparam logic [AdEsrRegionSize-1:0] AdEsrRegion = 8'h01;
  localparam int unsigned AdEsrMessageIdStart = 3;
  localparam int unsigned AdEsrMessageIdSize = 8;
  localparam int unsigned AdEsrAgentTypeStart = 20;
  localparam int unsigned AdEsrAgentTypeSize = 2;
  localparam logic [AdEsrAgentTypeSize-1:0] AdEsrAgentTypeTbox = 2'd2;
  localparam int unsigned AdEsrShireIdStart = 22;
  localparam int unsigned AdEsrShireIdSize = 8;
  localparam int unsigned AdEsrNeighIdStart = 16;
  localparam logic [AdEsrShireIdSize-1:0] LocalShireId = 8'hff;
  localparam int unsigned AdDdrRegionStart = 39;
  localparam int unsigned AdDdrRegionSize = PaSize - AdDdrRegionStart;
  localparam logic [AdDdrRegionSize-1:0] AdDdrRegion = 1'b1;
  localparam int unsigned AdScpRegionStart = 31;
  localparam int unsigned AdScpRegionSize = PaSize - AdScpRegionStart;
  localparam logic [AdScpRegionSize-1:0] AdScpRegion = {{(AdScpRegionSize-1){1'b0}}, 1'b1};
  localparam int unsigned AdScpFormatStart = 30;
  localparam int unsigned AdScpFormatSize = 1;
  localparam logic AdScpFormat0 = 1'b0;
  localparam int unsigned AdScpF0ShireIdStart = 23;
  localparam int unsigned AdScpF0ShireIdSize = 7;
  localparam int unsigned AdScpF0ShireIdMsb = AdScpF0ShireIdStart + AdScpF0ShireIdSize - 1;
  localparam int unsigned AdScpF1OffsetStart0 = 0;
  localparam int unsigned AdScpF1OffsetSize0 = 6;
  localparam int unsigned AdScpF1ShireIdStart0 = AdScpF1OffsetStart0 + AdScpF1OffsetSize0;
  localparam int unsigned AdScpF1ShireIdSize0 = 5;
  localparam int unsigned AdScpF1OffsetStart1 = AdScpF1ShireIdStart0 + AdScpF1ShireIdSize0;
  localparam int unsigned AdScpF1OffsetSize1 = 17;
  localparam int unsigned AdScpF1ShireIdStart1 = AdScpF1OffsetStart1 + AdScpF1OffsetSize1;
  localparam int unsigned AdScpF1ShireIdSize1 = 2;
  localparam int unsigned AdScpF1ShireIdMsb = AdScpF1ShireIdStart1 + AdScpF1ShireIdSize1 - 1;
  localparam int unsigned AdScpFl2OffsetStart = 0;
  localparam int unsigned AdScpFl2OffsetSize = 23;
  localparam int unsigned AdScpFl2ShireIdStart = AdScpFl2OffsetStart + AdScpFl2OffsetSize;
  localparam int unsigned AdScpFl2ShireIdSize = 8;
  localparam int unsigned AdScpFl2ShireIdMsb = AdScpFl2ShireIdStart + AdScpFl2ShireIdSize - 1;

  function automatic logic [NeighDebugMatchWidth-1:0] fit_dbg_match(input logic [1023:0] value);
    fit_dbg_match = value[NeighDebugMatchWidth-1:0];
  endfunction
  function automatic logic [NeighDebugFilterWidth-1:0] fit_dbg_filter(input logic [1023:0] value);
    fit_dbg_filter = value[NeighDebugFilterWidth-1:0];
  endfunction
  function automatic logic [NeighDebugDataWidth-1:0] fit_dbg_data(input logic [1023:0] value);
    fit_dbg_data = value[NeighDebugDataWidth-1:0];
  endfunction
  function automatic logic [DestFifosW-1:0] dest_fifo_from_bank(input logic [ScBankIdSize-1:0] bank);
    dest_fifo_from_bank = {{(DestFifosW-ScBankIdSize){1'b0}}, bank};
  endfunction
  function automatic etlink_pkg::neigh_source_e source_from_ptw(input logic [PtwPerNW-1:0] idx);
    source_from_ptw = etlink_pkg::neigh_source_e'(etlink_pkg::SrcPtw0 + idx);
  endfunction
  function automatic etlink_pkg::neigh_source_e source_from_minion(input logic [MinPerNW-1:0] idx);
    source_from_minion = etlink_pkg::neigh_source_e'(etlink_pkg::SrcMinion0 + idx);
  endfunction


  logic clock;
  logic clock_shire;
  logic clock_minion;
  logic clock_tbox;
  logic reset_c_shire;
  logic reset_d_shire;
  logic reset_w_shire;
  logic reset_warm;
  logic rst_w_icache_n;
  logic dft_reset_byp_hv;
  logic dft_reset_hv;
  logic dft_reset_byp_lv;
  logic dft_reset_lv;
  logic dft__reset_byp_hv;
  logic dft__reset_hv;
  logic dft__reset_byp_lv;
  logic dft__reset_lv;
  logic dft__use_reset_cntl_hv;
  neigh_pkg::shire_tdr_reset_cntl_t dft__reset_cntl_hv;
  logic [5:0] chicken_bits_vc;
  logic [5:0] chicken_bits_vc_lo;
  bpam_run_control_neigh_t bpam_run_control;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_hi;
  logic dll_feedback_shire;
  logic dll_feedback_neigh;
  logic [NumShireIdsW-1:0] shire_id;
  logic [NumNeighW-1:0] neigh_id;
  logic [TboxPerShireW-1:0] shire_tbox_id;
  logic shire_tbox_en;
  logic [MinPerN-1:0] esr_thread0_enable;
  logic [MinPerN-1:0] esr_thread1_enable;
  esr_minion_features_t esr_minion_features;
  icache_prefetch_conf_t esr_icache_prefetch_conf;
  logic esr_icache_prefetch_start;
  logic esr_icache_prefetch_done;
  logic esr_icache_err_detected_op;
  logic esr_icache_err_logged_op;
  esr_ms_dmctrl_t dmctrl;
  esr_and_or_tree_l0_t esr_and_or_tree_L0_op;
  logic esr_shire_coop_mode;
  logic [ShireMinionMemOverrideBits-1:0] esr_minion_mem_override;
  esr_dll_dly_est_ctl_t esr_dll_dly_est_ctl;
  esr_dll_dly_est_sts_t esr_dll_dly_est_sts;
  logic [ScBanks:0] neigh_sc_req_ready;
  logic [ScBanks:0] neigh_sc_req_valid;
  et_link_req_info_t neigh_sc_req_info;
  logic neigh_sc_rsp_ready;
  logic neigh_sc_rsp_valid;
  et_link_rsp_info_t neigh_sc_rsp_info;
  logic hw_dbg_sm_monitor_enabled;
  logic [NeighSmGpioWidth-1:0] neigh_sm_gpio;
  neigh_sm_dbg_monitor_t neigh_sm_signals;
  APB_to_neigh_t APB_req_from_shire;
  APB_from_neigh_t APB_rsp_to_shire;
  logic [MinPerN-1:0] int_mtip;
  logic int_meip;
  logic int_seip;
  logic [ThreadsPerN-1:0] ipi_msip;
  logic [ThreadsPerN-1:0] ipi_redirect_trigger;
  logic [MinPerN-1:0] uc_to_neigh_fcc;
  logic [UcFccTargetSize-1:0] uc_to_neigh_fcc_target;
  logic flb_neigh_l2_req_valid;
  logic [CsrFlBarrierSize-1:0] flb_neigh_l2_req_data;
  logic flb_l2_neigh_resp_valid;
  logic flb_l2_neigh_resp_data;
  logic icache_f2_sram_req_write;
  logic [IcacheSramAddrWidth-1:0] icache_f2_sram_req_addr;
  logic icache_f2_sram_req_valid;
  logic icache_f2_sram_req_ready;
  logic [IcacheSramDataWidth-1:0] icache_f0_sram_resp_dout;
  logic icache_f0_sram_resp_valid;
  logic icache_f0_sram_resp_ready;
  coop_tload_slv_rdy_req_t coop_tload_slv_rdy_out_data;
  logic coop_tload_slv_rdy_out_valid;
  coop_tload_slv_rdy_req_t [NumNeigh-2:0] coop_tload_slv_rdy_in_data;
  logic [NumNeigh-2:0] coop_tload_slv_rdy_in_valid;
  logic [ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id;
  logic [NumNeigh-2:0] coop_tload_mst_done_out_valid;
  logic [NumNeigh-2:0][ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id;
  logic [NumNeigh-2:0] coop_tload_mst_done_in_valid;
  logic [MinPerN-1:0] reset_c_per_minion;
  logic [MinPerN-1:0] reset_d_per_minion;
  logic [MinPerN-1:0] reset_w_per_minion;
  logic [NumShireIdsW-1:0] shire_id_lo;
  logic [NumNeighW-1:0] neigh_id_lo;
  logic [MinPerN-1:0][MinPerSW-1:0] shire_min_id;
  logic [MinPerN-1:0] thread0_enable;
  logic [MinPerN-1:0] thread1_enable;
  esr_minion_features_t esr_minion_features_lo;
  logic esr_shire_coop_mode_lo;
  logic [ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_lo;
  logic esr_bypass_dcache;
  logic [VaSize-1:0] minion_boot_addr;
  esr_mprot_t mprot;
  tlb_entry_type vmspagesize;
  minion_interrupts [MinPerN-1:0] interrupts_per_minion;
  logic [MinPerN-1:0] flb_core_neigh_req_valid;
  logic [MinPerN-1:0][CsrFlBarrierSize-1:0] flb_core_neigh_req_data;
  logic [MinPerN-1:0] flb_neigh_core_resp_valid;
  logic flb_neigh_core_resp_data;
  logic [MinPerN-1:0][DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_ready;
  logic [MinPerN-1:0][DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_valid;
  et_link_minion_evict_req_info_t [MinPerN-1:0] l2_dcache_evict_req;
  logic [MinPerN-1:0][DcacheL2MissReqPorts-1:0] l2_dcache_miss_req_ready;
  logic [MinPerN-1:0][DcacheL2MissReqPorts-1:0] l2_dcache_miss_req_valid;
  et_link_minion_miss_req_info_t [MinPerN-1:0] l2_dcache_miss_req;
  logic [MinPerN-1:0] l2_dcache_resp_ready;
  logic [MinPerN-1:0] l2_dcache_resp_valid;
  et_link_minion_rsp_info_t [MinPerN-1:0] l2_dcache_resp;
  logic [MinPerN-1:0] icache_req_ready;
  logic [MinPerN-1:0] icache_req_valid;
  frontend_icache_req [MinPerN-1:0] icache_req;
  logic [IcacheNrReqs-1:0] icache_resp_valid;
  logic [IcacheNrReqs-1:0] icache_resp_miss;
  icache_frontend_resp [IcacheNrReqs-1:0] icache_resp;
  logic [IcacheNrReqs-1:0] icache_fill_done;
  logic [MinPerN-1:0] icache_flush_data;
  minion_satp_info [MinPerN-1:0] satp_info;
  minion_satp_info [MinPerN-1:0] matp_info;
  logic [MinPerN-1:0] tlb_invalidate;
  minion_ptw_req [PtwPerN-1:0][MinPerPtw-1:0] dc_ptw_req_data;
  logic [PtwPerN-1:0][MinPerPtw-1:0] dc_ptw_req_valid;
  logic [PtwPerN-1:0][MinPerPtw-1:0] dc_ptw_req_ready;
  minion_ptw_pte [PtwPerN-1:0] ptw_dc_resp_data;
  logic [PtwPerN-1:0][MinPerPtw-1:0] ptw_dc_resp_valid;
  logic [MinPerN-1:0][EsrApbDataWidth-1:0] minion_apb_prdata;
  logic [MinPerN-1:0] minion_apb_pready;
  logic [MinPerN-1:0] minion_apb_pslverr;
  logic minion_apb_penable;
  logic [MinPerN-1:0] minion_apb_psel;
  logic minion_apb_pwrite;
  logic [MinDmApbAddrWidth-1:0] minion_apb_paddr;
  logic [EsrApbDataWidth-1:0] minion_apb_pwdata;
  minion_debug_in_t [MinPerN-1:0] debug_minion_in;
  minion_debug_out_t [MinPerN-1:0] debug_minion_out;
  logic [MinPerN-1:0] minion_dbg_signals_en;
  neigh_sm_dbg_monitor_t [MinPerN-1:0] minion_dbg_signals;
  logic [MinPerN-1:0][NeighDebugSmMuxWidth-1:0] minion_dbg_signals_mux;
  logic [MinPerN-1:0][PmuMinionCountersNum-1:0] pmu_count_up;
  logic [MinPerN-1:0][minion_pkg::NrThreads-1:0][XregSize-1:0] pmu_read_data;
  logic [MinPerN-1:0][minion_pkg::NrThreads-1:0][PmuCountersSelectBits-1:0] pmu_read_sel;
  logic [MinPerN-1:0][PmuTotalCountersNum-1:0] pmu_write_en;
  logic [MinPerN-1:0][XregSize-1:0] pmu_write_data;
  logic [MinPerN-1:0][PmuNeighEventCntSelBits-1:0] pmu_neigh_event_sel;
  logic tbox_reset_c;
  logic tbox_reset_d;
  logic tbox_reset_w;
  logic [TboxPerShireW-1:0] tbox_id_lo;
  logic [15:0] tbox_control_esr;
  logic [15:0] tbox_status_esr;
  logic [VaSize-1:0] tbox_image_table_ptr_esr;
  logic tbox_neigh_req_ready;
  logic tbox_neigh_req_valid;
  et_link_neigh_req_info_t tbox_neigh_req_info;
  logic tbox_neigh_rsp_ready;
  logic tbox_neigh_rsp_valid;
  et_link_rsp_info_t tbox_neigh_rsp_info;
  logic tbox_dbg_signals_en;
  neigh_sm_dbg_monitor_t tbox_dbg_signals;
  logic [NeighDebugSmMuxWidth-1:0] tbox_dbg_signals_mux;
  neigh_ch_apb_mux_pkg::apb_to_neigh_tbox_t tbox_APB_req;
  APB_from_neigh_t tbox_APB_rsp;
  tbox_bpam_run_control_t tbox_bpam_run_control;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t tbox_bpam_run_control_ack;

  assign clock = clk_i;
  assign clock_shire = clk_shire_i;
  assign clk_minion_o = clock_minion;
  assign clk_tbox_o = clock_tbox;
  assign reset_c_shire = !rst_c_shire_ni;
  assign reset_d_shire = !rst_d_shire_ni;
  assign reset_w_shire = !rst_w_shire_ni;
  assign reset_warm = !rst_warm_ni;
  assign rst_w_icache_no = rst_w_icache_n;
  assign dft_reset_byp_hv = dft_hv_i.scanmode;
  assign dft_reset_hv = !dft_hv_i.scan_reset_n;
  assign dft__reset_byp_hv = dft_reset_byp_hv;
  assign dft__reset_hv = dft_reset_hv;
  assign dft_reset_byp_lv = dft__reset_byp_lv;
  assign dft_reset_lv = dft__reset_lv;
  assign dft__use_reset_cntl_hv = dft_use_reset_cntl_hv_i;
  assign dft__reset_cntl_hv = dft_reset_cntl_hv_i;
  assign dft_lv_o.scanmode = dft_reset_byp_lv;
  assign dft_lv_o.scan_reset_n = !dft_reset_lv;
  assign dft_lv_o.sram_clk_override = dft_hv_i.sram_clk_override;
  assign dft_lv_o.ram_rei = dft_hv_i.ram_rei;
  assign dft_lv_o.ram_wei = dft_hv_i.ram_wei;
  assign chicken_bits_vc = {chicken_bits_vc_i.min_frontend_clock_gate_disable,
                            chicken_bits_vc_i.min_dcache_clock_gate_disable,
                            chicken_bits_vc_i.min_vputrans_clock_gate_disable,
                            chicken_bits_vc_i.min_vputima_clock_gate_disable,
                            chicken_bits_vc_i.min_vpulane_clock_gate_disable,
                            chicken_bits_vc_i.min_intpipe_clock_gate_disable};
  assign chicken_bits_vc_lo_o.min_frontend_clock_gate_disable = chicken_bits_vc_lo[5];
  assign chicken_bits_vc_lo_o.min_dcache_clock_gate_disable   = chicken_bits_vc_lo[4];
  assign chicken_bits_vc_lo_o.min_vputrans_clock_gate_disable = chicken_bits_vc_lo[3];
  assign chicken_bits_vc_lo_o.min_vputima_clock_gate_disable  = chicken_bits_vc_lo[2];
  assign chicken_bits_vc_lo_o.min_vpulane_clock_gate_disable  = chicken_bits_vc_lo[1];
  assign chicken_bits_vc_lo_o.min_intpipe_clock_gate_disable  = chicken_bits_vc_lo[0];
  assign bpam_run_control = bpam_run_control_i;
  assign bpam_rc_tbox_ack_hi_o = bpam_rc_tbox_ack_hi;
  assign dll_feedback_shire_o = dll_feedback_shire;
  assign dll_feedback_neigh_o = dll_feedback_neigh;
  assign shire_id = shire_id_i;
  assign neigh_id = neigh_id_i;
  assign shire_tbox_id = shire_tbox_id_i;
  assign shire_tbox_en = shire_tbox_en_i;
  assign esr_thread0_enable = esr_thread0_enable_i;
  assign esr_thread1_enable = esr_thread1_enable_i;
  assign esr_minion_features = esr_minion_features_i;
  assign esr_icache_prefetch_conf = esr_icache_prefetch_conf_i;
  assign esr_icache_prefetch_start = esr_icache_prefetch_start_i;
  assign esr_icache_prefetch_done_o = esr_icache_prefetch_done;
  assign esr_icache_err_detected_o = esr_icache_err_detected_op;
  assign esr_icache_err_logged_o = esr_icache_err_logged_op;
  assign dmctrl = dmctrl_i;
  assign esr_and_or_tree_l0_o = esr_and_or_tree_L0_op;
  assign esr_shire_coop_mode = esr_shire_coop_mode_i;
  assign esr_minion_mem_override = esr_minion_mem_override_i;
  assign esr_dll_dly_est_ctl = esr_dll_dly_est_ctl_i;
  assign esr_dll_dly_est_sts_o = esr_dll_dly_est_sts;
  assign neigh_sc_req_ready = neigh_sc_req_ready_i;
  assign neigh_sc_req_valid_o = neigh_sc_req_valid;
  assign neigh_sc_req_info_o = neigh_sc_req_info;
  assign neigh_sc_rsp_ready_o = neigh_sc_rsp_ready;
  assign neigh_sc_rsp_valid = neigh_sc_rsp_valid_i;
  assign neigh_sc_rsp_info = neigh_sc_rsp_info_i;
  assign hw_dbg_sm_monitor_enabled = hw_dbg_sm_monitor_enabled_i;
  assign neigh_sm_gpio = neigh_sm_gpio_i;
  assign neigh_sm_signals_o = neigh_sm_signals;
  assign APB_req_from_shire = apb_req_from_shire_i;
  assign apb_rsp_to_shire_o = APB_rsp_to_shire;
  assign int_mtip = int_mtip_i;
  assign int_meip = int_meip_i;
  assign int_seip = int_seip_i;
  assign ipi_msip = ipi_msip_i;
  assign ipi_redirect_trigger = ipi_redirect_trigger_i;
  assign uc_to_neigh_fcc = uc_to_neigh_fcc_i;
  assign uc_to_neigh_fcc_target = uc_to_neigh_fcc_target_i;
  assign flb_neigh_l2_req_valid_o = flb_neigh_l2_req_valid;
  assign flb_neigh_l2_req_data_o = flb_neigh_l2_req_data;
  assign flb_l2_neigh_resp_valid = flb_l2_neigh_resp_valid_i;
  assign flb_l2_neigh_resp_data = flb_l2_neigh_resp_data_i;
  assign icache_f2_sram_req_write_o = icache_f2_sram_req_write;
  assign icache_f2_sram_req_addr_o = icache_f2_sram_req_addr;
  assign icache_f2_sram_req_valid_o = icache_f2_sram_req_valid;
  assign icache_f2_sram_req_ready = icache_f2_sram_req_ready_i;
  assign icache_f0_sram_resp_dout = icache_f0_sram_resp_dout_i;
  assign icache_f0_sram_resp_valid = icache_f0_sram_resp_valid_i;
  assign icache_f0_sram_resp_ready_o = icache_f0_sram_resp_ready;
  assign coop_tload_slv_rdy_out_data_o = coop_tload_slv_rdy_out_data;
  assign coop_tload_slv_rdy_out_valid_o = coop_tload_slv_rdy_out_valid;
  assign coop_tload_slv_rdy_in_data = coop_tload_slv_rdy_in_data_i;
  assign coop_tload_slv_rdy_in_valid = coop_tload_slv_rdy_in_valid_i;
  assign coop_tload_mst_done_out_coop_id_o = coop_tload_mst_done_out_coop_id;
  assign coop_tload_mst_done_out_valid_o = coop_tload_mst_done_out_valid;
  assign coop_tload_mst_done_in_coop_id = coop_tload_mst_done_in_coop_id_i;
  assign coop_tload_mst_done_in_valid = coop_tload_mst_done_in_valid_i;
  assign rst_c_per_minion_no = ~reset_c_per_minion;
  assign rst_d_per_minion_no = ~reset_d_per_minion;
  assign rst_w_per_minion_no = ~reset_w_per_minion;
  assign shire_id_lo_o = shire_id_lo;
  assign neigh_id_lo_o = neigh_id_lo;
  assign shire_min_id_o = shire_min_id;
  assign thread0_enable_o = thread0_enable;
  assign thread1_enable_o = thread1_enable;
  assign esr_minion_features_lo_o = esr_minion_features_lo;
  assign esr_shire_coop_mode_lo_o = esr_shire_coop_mode_lo;
  assign esr_minion_mem_override_lo_o = esr_minion_mem_override_lo;
  assign esr_bypass_dcache_o = esr_bypass_dcache;
  assign minion_boot_addr_o = minion_boot_addr;
  assign mprot_o = mprot;
  assign vmspagesize_o = vmspagesize;
  assign interrupts_per_minion_o = interrupts_per_minion;
  assign flb_core_neigh_req_valid = flb_core_neigh_req_valid_i;
  assign flb_core_neigh_req_data = flb_core_neigh_req_data_i;
  assign flb_neigh_core_resp_valid_o = flb_neigh_core_resp_valid;
  assign flb_neigh_core_resp_data_o = flb_neigh_core_resp_data;
  assign l2_dcache_evict_req_ready_o = l2_dcache_evict_req_ready;
  assign l2_dcache_evict_req_valid = l2_dcache_evict_req_valid_i;
  assign l2_dcache_evict_req = l2_dcache_evict_req_i;
  assign l2_dcache_miss_req_ready_o = l2_dcache_miss_req_ready;
  assign l2_dcache_miss_req_valid = l2_dcache_miss_req_valid_i;
  assign l2_dcache_miss_req = l2_dcache_miss_req_i;
  assign l2_dcache_resp_ready = l2_dcache_resp_ready_i;
  assign l2_dcache_resp_valid_o = l2_dcache_resp_valid;
  assign l2_dcache_resp_o = l2_dcache_resp;
  assign icache_req_ready_o = icache_req_ready;
  assign icache_req_valid = icache_req_valid_i;
  assign icache_req = icache_req_i;
  assign icache_resp_valid_o = icache_resp_valid;
  assign icache_resp_miss_o = icache_resp_miss;
  assign icache_resp_o = icache_resp;
  assign icache_fill_done_o = icache_fill_done;
  assign icache_flush_data = icache_flush_data_i;
  assign satp_info = satp_info_i;
  assign matp_info = matp_info_i;
  assign tlb_invalidate = tlb_invalidate_i;
  assign dc_ptw_req_data = dc_ptw_req_data_i;
  assign dc_ptw_req_valid = dc_ptw_req_valid_i;
  assign dc_ptw_req_ready_o = dc_ptw_req_ready;
  assign ptw_dc_resp_data_o = ptw_dc_resp_data;
  assign ptw_dc_resp_valid_o = ptw_dc_resp_valid;
  assign minion_apb_prdata = minion_apb_prdata_i;
  assign minion_apb_pready = minion_apb_pready_i;
  assign minion_apb_pslverr = minion_apb_pslverr_i;
  assign minion_apb_penable_o = minion_apb_penable;
  assign minion_apb_psel_o = minion_apb_psel;
  assign minion_apb_pwrite_o = minion_apb_pwrite;
  assign minion_apb_paddr_o = minion_apb_paddr;
  assign minion_apb_pwdata_o = minion_apb_pwdata;
  assign debug_minion_in_o = debug_minion_in;
  assign debug_minion_out = debug_minion_out_i;
  assign minion_dbg_signals_en_o = minion_dbg_signals_en;
  assign minion_dbg_signals = minion_dbg_signals_i;
  assign minion_dbg_signals_mux_o = minion_dbg_signals_mux;
  assign pmu_count_up = pmu_count_up_i;
  assign pmu_read_data_o = pmu_read_data;
  assign pmu_read_sel = pmu_read_sel_i;
  assign pmu_write_en = pmu_write_en_i;
  assign pmu_write_data = pmu_write_data_i;
  assign pmu_neigh_event_sel = pmu_neigh_event_sel_i;
  assign tbox_rst_c_no = ~tbox_reset_c;
  assign tbox_rst_d_no = ~tbox_reset_d;
  assign tbox_rst_w_no = ~tbox_reset_w;
  assign tbox_id_lo_o = tbox_id_lo;
  assign tbox_control_esr_o = tbox_control_esr;
  assign tbox_status_esr = tbox_status_esr_i;
  assign tbox_image_table_ptr_esr_o = tbox_image_table_ptr_esr;
  assign tbox_neigh_req_ready_o = tbox_neigh_req_ready;
  assign tbox_neigh_req_valid = tbox_neigh_req_valid_i;
  assign tbox_neigh_req_info = tbox_neigh_req_info_i;
  assign tbox_neigh_rsp_ready = tbox_neigh_rsp_ready_i;
  assign tbox_neigh_rsp_valid_o = tbox_neigh_rsp_valid;
  assign tbox_neigh_rsp_info_o = tbox_neigh_rsp_info;
  assign tbox_dbg_signals_en_o = tbox_dbg_signals_en;
  assign tbox_dbg_signals = tbox_dbg_signals_i;
  assign tbox_dbg_signals_mux_o = tbox_dbg_signals_mux;
  assign tbox_apb_req_o = tbox_APB_req;
  assign tbox_APB_rsp = tbox_apb_rsp_i;
  assign tbox_bpam_run_control_o = tbox_bpam_run_control;
  assign tbox_bpam_run_control_ack = tbox_bpam_run_control_ack_i;
// INTERNAL DECLARATIONS
// Minion miss FF request
et_link_neigh_req_info_t [MinPerN-1:0]                                  min_miss_req;
logic [MinPerN-1:0]                                                     min_miss_req_valid;
logic [MinPerN-1:0]                                                     min_miss_req_ready;
et_link_minion_miss_req_info_t [MinPerN-1:0][ShireCoopMissReqs-1:0] min_coop_miss_req;
logic [MinPerN-1:0][ShireCoopMissReqs-1:0]                          min_coop_miss_req_valid;
logic [MinPerN-1:0][ShireCoopMissReqs-1:0]                          min_coop_miss_req_ready;
// Minion evict FF request
et_link_neigh_req_info_t [MinPerN-1:0]           min_evict_req;
logic [MinPerN-1:0]                              min_evict_req_valid;
logic [MinPerN-1:0]                              min_evict_req_ready;
et_link_neigh_req_info_t [MinPerN-1:0]           coop_evict_req;
logic [MinPerN-1:0]                              coop_evict_valid;
logic [MinPerN-1:0]                              coop_evict_ready;
// Common minion request
et_link_nodata_req_info_t [MinPerN-1:0]          min_l2_req_info;
logic [MinPerN-1:0][NeighDataSize-1:0]           min_l2_req_data;
logic [MinPerN-1:0]                              min_l2_req_is_evict;
logic [MinPerN-1:0]                              min_l2_req_valid;
logic [MinPerN-1:0]                              min_l2_req_bid;
logic [MinPerN-1:0]                              min_l2_req_ready;
// Cooperative TLoad request
et_link_neigh_req_info_t                            coop_tload_miss_req_info;
logic                                               coop_tload_miss_req_valid;
logic                                               coop_tload_miss_req_ready;
// Cooperative TLoad tag table CAM port
logic                                               coop_tload_tag_table_cam_pop;
logic [ShireCoopLoadIdSize-1:0]                   coop_tload_tag_table_cam_tag;
coop_tload_tag_table_data_t                         coop_tload_tag_table_cam_data;
// TStore evict request
logic                                               tstore_evict_valid;
logic                                               tstore_miss_req_ready;
et_link_neigh_req_info_t                            tstore_evict_req;
// TStore response to minions
logic [MinPerN-1:0]                              tstore_min_resp_ready;
logic [MinPerN-1:0]                              tstore_min_resp_valid;
et_link_minion_rsp_info_t [MinPerN-1:0]          tstore_min_resp;
// Icache request
et_link_nodata_req_info_t                           icache_l2_miss_req_info;
logic                                               icache_l2_miss_req_valid;
logic                                               icache_l2_miss_req_ready;
// PTW request
et_link_neigh_req_info_t [PtwPerN-1:0]           ptw_l2_miss_req_info;
logic [PtwPerN-1:0]                              ptw_l2_miss_req_valid;
logic [PtwPerN-1:0]                              ptw_l2_miss_req_ready;
// TBOX request FIFO
et_link_nodata_req_info_t                           tbox_miss_req_info;
logic                                               tbox_miss_req_valid;
logic                                               tbox_miss_req_ready;
// Miss request Round-Robin arbiter signals
logic [NeighNumReqAgents-1:0]                   miss_req_arb_bid;
et_link_nodata_req_info_t [3:0]                     miss_req_info_xx;
et_link_nodata_req_info_t [3:0]                     miss_req_info_xx_next;
logic [3:0][NeighDataSize-1:0]                      miss_req_data_xx;
logic [3:0][NeighDataSize-1:0]                      miss_req_data_xx_next;
et_link_nodata_req_info_t                           miss_req_info;
logic [NeighDataSize-1:0]                           miss_req_data;
logic [NeighNumReqAgents-1:0]                   miss_req_arb_grant;
logic [MinPerN-1:0]                              min_l2_req_arb_grant;
logic                                               icache_l2_miss_req_arb_grant;
logic [PtwPerN-1:0]                              ptw_l2_miss_req_arb_grant;
logic                                               tbox_l2_miss_req_arb_grant;
logic                                               coop_tload_l2_miss_req_arb_grant;
logic                                               tstore_l2_miss_req_arb_grant;
logic [3:0]                                         miss_req_valid_xx;
logic [3:0]                                         miss_req_valid_xx_next;
logic [3:0]                                         miss_req_accepted_xx;
logic                                               miss_req_valid;
logic                                               miss_req_accepted;
logic                                               miss_req_ready;
// Flopped and address processed miss request
et_link_nodata_req_info_t                           miss_req_info_ff;
logic [NeighDataSize-1:0]                           miss_req_data_ff;
logic                                               miss_req_valid_ff;
logic                                               miss_req_accepted_ff;
et_link_nodata_req_info_t                           miss_req_info_ff_proc;
et_link_nodata_req_info_t                           miss_req_info_ff_proc_next;
logic [NeighDataSize-1:0]                           miss_req_data_ff_proc;
logic [NeighDataSize-1:0]                           miss_req_data_ff_proc_next;
logic                                               miss_req_valid_ff_proc;
logic [DestFifosW-1:0]                            miss_req_info_dest_fifo;
logic [DestFifosW-1:0]                            miss_req_info_dest_fifo_next;
logic                                               miss_req_goes_to_tbox;
logic                                               miss_req_goes_to_tbox_next;
logic                                               miss_req_goes_to_int_fifo;
// Intermediate FIFO
logic [OutFifos-1:0][NeighIntFifoSizeW:0]     int_req_fifo_counter;
logic [OutFifos-1:0][NeighIntFifoSizeW:0]     int_req_fifo_counter_next;
et_link_nodata_req_info_t                           int_req_info_in;
logic [NeighDataSize-1:0]                           int_req_data_in;
logic [OutFifosW-1:0]                             int_req_info_push_out_fifo;
logic                                               int_req_push_early;
logic                                               int_req_push;
logic [OutFifos-1:0]                               int_req_push_per_out_fifo;
logic                                               int_req_fifo_full;
logic                                               int_req_info_pop;
logic                                               int_req_data_pop;
logic [OutFifos-1:0]                               int_req_pop_per_out_fifo;
et_link_nodata_req_info_t                           int_req_info_out;
logic [NeighDataSize-1:0]                           int_req_data_out;
logic [OutFifosW-1:0]                             int_req_info_pop_out_fifo;
logic                                               int_req_info_valid;
// Output FIFOs
et_link_nodata_req_info_t                           out_req_info_in;
logic                                               out_req_is_line_evict;
logic [NeighDataSize-1:0]                           out_req_data_in;
logic [OutFifos-1:0]                               out_req_push;
logic [OutFifos-1:0]                               out_req_fifo_ready;
logic [OutFifos-1:0]                               out_req_fifo_full;
// Miss request per-bank info to the L2
et_link_req_info_t [ScBanks-1:0]                  miss_req_bank_info;
logic [ScBanks-1:0]                               miss_req_bank_valid;
logic [ScBanks-1:0]                               miss_req_bank_ready;
// Minion to TBOX requests
et_link_neigh_req_info_t                            min_tbox_req_info;
logic                                               min_tbox_req_valid_early;
logic                                               min_tbox_req_valid;
logic                                               min_tbox_req_ready;
// TBOX to minion replies
et_link_neigh_rsp_info_t                            tbox_min_rsp_info;
logic                                               tbox_min_rsp_valid;
logic                                               tbox_min_rsp_ready;
et_link_neigh_rsp_info_t [MinPerN-1:0]           tbox_min_rsp_info_in;
logic [MinPerN-1:0]                              tbox_min_rsp_valid_in;
logic [MinPerN-1:0]                              tbox_min_rsp_ready_in;
// Fill FIFO
et_link_rsp_info_t                                  fill_fifo_in_rsp_info;
logic                                               fill_fifo_in_rsp_valid;
logic                                               fill_fifo_in_rsp_ready;
logic                                               fill_fifo_push;
logic                                               fill_fifo_full;
et_link_rsp_info_t [NeighNumRespAgents-1:0]     fill_fifo_out_rsp_info;
logic [NeighNumRespAgents-1:0]                  fill_fifo_out_rsp_valid;
logic [NeighNumRespAgents-1:0]                  fill_ff_available;
// Minion Fill FF response
et_link_rsp_info_t [MinPerN-1:0]                 fill_fifo_min_resp;
logic [MinPerN-1:0]                              fill_fifo_min_valid;
logic [MinPerN-1:0]                              fill_fifo_min_ready;
// ESRs
esr_neigh_chicken_t                                 esr_neigh_chicken;
minion_chicken_bits_vc_t                            chicken_bits_vc_in;
minion_chicken_bits_vc_t                            chicken_bits_vc_lo_out;
// PMU Unit
esr_pmu_ctrl_t                                      esr_pmu_ctrl;
logic  [PmuNeighEvents:1]                        pmu_neigh_events;
logic  [PmuNeighEvents:1]                        pmu_neigh_events_next;
// FCCs
logic [MinPerN-1:0]                              uc_to_neigh_fcc_lo;
logic [UcFccTargetSize-1:0]                     uc_to_neigh_fcc_target_lo;
// Interrupts (low voltage)
logic [MinPerN-1:0]                              int_mtip_lo;
logic                                               int_meip_lo;
logic                                               int_seip_lo;
logic [ThreadsPerN-1:0]                          ipi_msip_lo;
logic [ThreadsPerN-1:0]                          ipi_redirect_trigger_lo;
//IPI program counter
logic [PcSize-1:0]                                   esr_ipi_redirect_pc;
// Fast local barrier
logic                                               flb_neigh_l2_req_valid_lo;
logic [CsrFlBarrierSize-1:0]                       flb_neigh_l2_req_data_lo;
logic                                               flb_l2_neigh_resp_valid_lo;
logic                                               flb_l2_neigh_resp_data_lo;
// Request to L1 SRAM blocks (low voltage)
logic                                               icache_f2_sram_req_write_lo;
logic [IcacheSramAddrWidth-1:0]                  icache_f2_sram_req_addr_lo;
logic                                               icache_f2_sram_req_valid_lo;
logic                                               icache_f2_sram_req_ready_lo;
// Response from L1 SRAM blocks (low voltage)
logic [IcacheSramDataWidth-1:0]                 icache_f0_sram_resp_dout_lo;
logic                                               icache_f0_sram_resp_valid_lo;
logic                                               icache_f0_sram_resp_ready_lo;
// Icache ESRs (low voltage)
icache_prefetch_conf_t                              esr_icache_prefetch_conf_lo;
logic                                               esr_icache_prefetch_start_lo;
logic                                               esr_icache_prefetch_done_lo;
logic                                               esr_icache_err_detected_lo;
logic                                               esr_icache_err_logged_lo;
// TBOX ESRs
logic [15:0]                                        esr_texture_control_op;
logic [15:0]                                        esr_texture_status_ip;
logic [VaSize-1:0]                                esr_texture_image_table_ptr_op;
logic [15:0]                                        esr_texture_control_op_ff;
logic [15:0]                                        esr_texture_status_ip_ff;
logic [VaSize-1:0]                                esr_texture_image_table_ptr_op_ff;
// L2 Response bus (low voltage)
et_link_rsp_info_t                                  neigh_sc_rsp_info_lo;
logic                                               neigh_sc_rsp_valid_lo;
logic                                               neigh_sc_rsp_ready_lo;
// Debug
esr_ms_dmctrl_t                                     dmctrl_struct;
esr_ms_dmctrl_t                                     dmctrl_lo;
logic                                               hw_dbg_sm_monitor_enabled_lo;
bpam_run_control_neigh_t                            bpam_run_control_lo;
// Debug signals going to Status Monitor
logic [MinPerN-1:0][DcacheL2MissReqPorts-1:0] miss_ff_dbg_sm_signals;
logic [MinPerN-1:0][DcacheL2EvictReqPorts-1:0] evict_ff_dbg_sm_signals;
fill_ff_dbg_sm_t [MinPerN-1:0]                   fill_ff_dbg_sm_signals;
icache_dbg_sm_t                                     icache_dbg_sm_signals;
ptw_dbg_sm_t [PtwPerN-1:0]                       ptw_dbg_sm_signals;
coop_tload_dbg_sm_t                                 coop_tload_dbg_sm_signals;
tstore_dbg_sm_t                                     tstore_dbg_sm_signals;
fln_dbg_sm_t                                        fln_dbg_sm_signals;

// Low voltage (internal) signals for Cooperative Tensor Load
coop_tload_slv_rdy_req_t                          coop_tload_slv_rdy_out_data_lv;
logic                                             coop_tload_slv_rdy_out_valid_lv;
coop_tload_slv_rdy_req_t [(NumNeigh-1)-1:0]     coop_tload_slv_rdy_in_data_lv;
logic [(NumNeigh-1)-1:0]                        coop_tload_slv_rdy_in_valid_lv;
logic [ShireCoopIdSize-1:0]                      coop_tload_mst_done_out_coop_id_lv;
logic [(NumNeigh-1)-1:0]                        coop_tload_mst_done_out_valid_lv;
logic [(NumNeigh-1)-1:0][ShireCoopIdSize-1:0]  coop_tload_mst_done_in_coop_id_lv;
logic [(NumNeigh-1)-1:0]                        coop_tload_mst_done_in_valid_lv;

// Reset pipeline signals
logic reset_w;
logic reset_w_dbg;
logic reset_w_neigh;
logic reset_w_neigh_ff;
logic reset_d_neigh_ff;
logic reset_c_neigh_ff;
logic reset_w_neigh_pmu;
logic tbox_en_lo;


assign chicken_bits_vc_in.min_frontend_clock_gate_disable  = chicken_bits_vc[5];
assign chicken_bits_vc_in.min_dcache_clock_gate_disable    = chicken_bits_vc[4];
assign chicken_bits_vc_in.min_vputrans_clock_gate_disable  = chicken_bits_vc[3];
assign chicken_bits_vc_in.min_vputima_clock_gate_disable   = chicken_bits_vc[2];
assign chicken_bits_vc_in.min_vpulane_clock_gate_disable   = chicken_bits_vc[1];
assign chicken_bits_vc_in.min_intpipe_clock_gate_disable   = chicken_bits_vc[0];

assign chicken_bits_vc_lo[5]  = chicken_bits_vc_lo_out.min_frontend_clock_gate_disable;
assign chicken_bits_vc_lo[4]  = chicken_bits_vc_lo_out.min_dcache_clock_gate_disable;
assign chicken_bits_vc_lo[3]  = chicken_bits_vc_lo_out.min_vputrans_clock_gate_disable;
assign chicken_bits_vc_lo[2]  = chicken_bits_vc_lo_out.min_vputima_clock_gate_disable;
assign chicken_bits_vc_lo[1]  = chicken_bits_vc_lo_out.min_vpulane_clock_gate_disable;
assign chicken_bits_vc_lo[0]  = chicken_bits_vc_lo_out.min_intpipe_clock_gate_disable;

//////////////////////////////////////////////////
// Generate resets
//////////////////////////////////////////////////
assign dmctrl_struct = dmctrl;

//
wire reset_c_shire_rsync;
wire reset_d_shire_rsync;
wire reset_w_shire_rsync;
//Resynchronization of shire resets with clock_shire
rst_repeat
rst_repeat_reset_c_shire_pd_hv (
  .dft__reset_byp( dft__reset_byp_hv  ),
  .dft__reset    ( dft__reset_hv      ),
  .clock         ( clock_shire        ),
  .reset_in      ( reset_c_shire      ),
  .reset_out     ( reset_c_shire_rsync)
);
rst_repeat
rst_repeat_reset_d_shire_pd_hv  (
  .dft__reset_byp( dft__reset_byp_hv  ),
  .dft__reset    ( dft__reset_hv      ),
  .clock         ( clock_shire        ),
  .reset_in      ( reset_d_shire      ),
  .reset_out     ( reset_d_shire_rsync)
);
rst_repeat
rst_repeat_reset_w_shire_pd_hv  (
  .dft__reset_byp( dft__reset_byp_hv  ),
  .dft__reset    ( dft__reset_hv      ),
  .clock         ( clock_shire        ),
  .reset_in      ( reset_w_shire      ),
  .reset_out     ( reset_w_shire_rsync)
);

// Change voltage of Neigh clock
wire clock_lv;
assign clock_minion = clock_lv;
assign clock_tbox   = clock_lv;

// Clock feedback connections
neigh_hv_logic_clock_feedback
neigh_hv_logic_clock_feedback_pd_hv
(
    .clk_shire_i ( clock_shire ),
    .clk_neigh_i ( clock_lv ), //a level shifter h2l will be added inside the *_pd_hv instance
    .dll_feedback_shire_o ( dll_feedback_shire ),
    .dll_feedback_neigh_o ( dll_feedback_neigh )
);


logic reset_c_neigh;
logic reset_d_neigh;

sys_gasket_lov
sys_gasket_rst_mngr (
   //inputs
   .clock                ( clock                          ), // high voltage clock
   .clock_fb_ref         ( dll_feedback_neigh             ), // high voltage clock
   .reset_warm_in        ( reset_warm                     ), // high voltage input
   .gpio_ndmreset_in     ( bpam_run_control.gpio.ndmreset ), // high voltage input
   .ndmreset_in          ( dmctrl_struct.ndmreset         ), // high voltage input
   .dmactive_in          ( dmctrl_struct.dmactive         ), // high voltage input
   .reset_cold_in        ( reset_c_shire_rsync            ), // high voltage input
   .dft__reset_byp       ( dft__reset_byp_hv              ), // high voltage input
   .dft__reset           ( dft__reset_hv                  ), // high voltage input
   .use_shire_tdr_reset_cntl (dft__use_reset_cntl_hv      ), // high voltage input
   .shire_tdr_reset_cntl ( dft__reset_cntl_hv             ), // high voltage input
   .clock_lv             ( clock_lv                       ), // low voltage clock output
   .reset_w              ( reset_w                        ), // LoV reset for non-debug blocks (includes global reset and ndm_reset)
   .reset_c              ( reset_c_neigh                  ), // LoV Global reset
   .reset_d              ( reset_d_neigh                  ), // LoV reset for debug blocks (includes global reset and ndm_reset)
   .dft__reset_byp_lv    ( dft__reset_byp_lv              ), // low voltage output
   .dft__reset_lv        ( dft__reset_lv                  )  // low voltage output
);

// Local Pipeline stage of reset_w signal to neigh_ch_dbg
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
rst_repeat rst_repeat_reset_w_dbg_stg  (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_w), .reset_out(reset_w_dbg));

// Local Pipeline stage of reset signals to Minions which have a pipeline stage in the Minions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
rst_repeat rst_repeat_reset_w_stg      (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_w_neigh), .reset_out(reset_w_neigh_ff));
rst_repeat rst_repeat_reset_w_pmu_stg  (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_w_neigh), .reset_out(reset_w_neigh_pmu));
rst_repeat rst_repeat_reset_d_stg      (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_d_neigh), .reset_out(reset_d_neigh_ff));
rst_repeat rst_repeat_reset_c_stg      (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_c_neigh), .reset_out(reset_c_neigh_ff));

// Pipeline stage of reset signals to Tbox
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
rst_repeat rst_repeat_reset_w_tbox_stg (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in( reset_w_neigh_ff | !tbox_en_lo ), .reset_out(tbox_reset_w));
rst_repeat rst_repeat_reset_d_tbox_stg (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in( reset_d_neigh_ff | !tbox_en_lo ), .reset_out(tbox_reset_d));
rst_repeat rst_repeat_reset_c_tbox_stg (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in( reset_c_neigh_ff | !tbox_en_lo ), .reset_out(tbox_reset_c));

//////////////////////////////////////////////////
// Memory override signals from TDR
/////////////////////////////////////////////////

// Just a level shifter instantiated
vclevel_shft_h2l #(
  .WIDTH   ( ShireMinionMemOverrideBits ))
ls_h2l_mem_override_pd_hv (
  .inp_hv  ( esr_minion_mem_override    ),
  .out_lv  ( esr_minion_mem_override_lo )
);

//////////////////////////////////////////////////
//APB MUX
/////////////////////////////////////////////////
APB_to_neigh_t                                      APB_req_from_shire_lo;
APB_from_neigh_t                                    APB_rsp_to_shire_lo;
APB_from_neigh_t                                    APB_rsp_to_shire_hi;
assign APB_rsp_to_shire = APB_rsp_to_shire_hi;

APB_to_neigh_esrs_t                                 APB_req_struct_ESR   ;
APB_to_neigh_icache_t                               APB_req_struct_icache;
APB_from_neigh_channel_t                            APB_rsp_struct_ESR   ;
APB_from_neigh_channel_t                            APB_rsp_struct_icache;

neigh_ch_apb_mux
neigh_ch_apb_mux
(
    // System signals
    .clk_i ( clock_lv ),
    .rst_ni ( ~(reset_c_neigh_ff) ),

    // APB connection with the shire
    .apb_req_from_shire_i ( APB_req_from_shire_lo ),
    .apb_rsp_to_shire_o ( APB_rsp_to_shire_lo ),

    // APB connection with the minions
    .minion_apb_prdata_i ( minion_apb_prdata ),
    .minion_apb_pready_i ( minion_apb_pready ),
    .minion_apb_pslverr_i ( minion_apb_pslverr ),
    .minion_apb_penable_o ( minion_apb_penable ),
    .minion_apb_psel_o ( minion_apb_psel ),
    .minion_apb_pwrite_o ( minion_apb_pwrite ),
    .minion_apb_paddr_o ( minion_apb_paddr ),
    .minion_apb_pwdata_o ( minion_apb_pwdata ),

    // APB connection with ESR
    .apb_req_struct_esr_o ( APB_req_struct_ESR ),
    .apb_req_struct_icache_o ( APB_req_struct_icache ),
    // APB connection with icache
    .apb_rsp_struct_esr_i ( APB_rsp_struct_ESR ),
    .apb_rsp_struct_icache_i ( APB_rsp_struct_icache ),

    // APB connection with TBOX
    .apb_req_struct_tbox_o ( tbox_APB_req ),
    .apb_rsp_struct_tbox_i ( tbox_APB_rsp )
);

////////////////////////////////////////////////////////////////////////////////
// Miss request Round-Robin arbiter to the intermediate/per-bank FIFOs
////////////////////////////////////////////////////////////////////////////////
logic [MinPerN-1:0] min_evict_second_access;
logic [MinPerN-1:0] min_evict_second_access_next;
logic                  min_evict_second_access_pending;
logic                  tstore_second_access;
logic                  tstore_second_access_next;
logic                  evict_second_access_pending;

// If evict requests are to write 512 bits, they must be assigned two consecutive grants,
// so that they are stored in two consecutive 256-bit FIFO entries
// Thus, we disable other bids during the second evict access
always_comb
begin
    min_evict_second_access_next = min_evict_second_access;

    for (integer unsigned i = 0; i < MinPerN; i++)
    begin
        if (min_l2_req_ready[i] && min_l2_req_is_evict[i] && (min_l2_req_info[i].size == etlink_pkg::SizeLine) && !min_evict_second_access[i])
            min_evict_second_access_next[i] = 1'b1;
        else if (min_l2_req_ready[i] && min_evict_second_access[i])
            min_evict_second_access_next[i] = 1'b0;
    end

    tstore_second_access_next = tstore_second_access;

    if (tstore_miss_req_ready && (tstore_evict_req.size == etlink_pkg::SizeLine) && !tstore_second_access)
        tstore_second_access_next = 1'b1;
    else if (tstore_miss_req_ready && tstore_second_access)
        tstore_second_access_next = 1'b0;
end

assign min_evict_second_access_pending = |min_evict_second_access_next;
assign evict_second_access_pending     = |{tstore_second_access_next,min_evict_second_access_next};

//       CLK       RST               DOUT                      DIN                           DEF
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    min_evict_second_access <= '0;
  end else begin
    min_evict_second_access <= min_evict_second_access_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    tstore_second_access <= 1'b0;
  end else begin
    tstore_second_access <= tstore_second_access_next;
  end
end

// TensorStore requests to arbiter
assign miss_req_arb_bid[NeighAgentTstore +: 1]             = tstore_evict_valid;
// Cooperative TensorLoad requests to arbiter
assign miss_req_arb_bid[NeighAgentTload +: 1]              = coop_tload_miss_req_valid;
// TBOX requests to arbiter
assign miss_req_arb_bid[NeighAgentTbox +: 1]               = tbox_miss_req_valid;
// PTW requests to arbiter
assign miss_req_arb_bid[NeighAgentPtw +: PtwPerN]       = ptw_l2_miss_req_valid;
// Icache requests to arbiter
assign miss_req_arb_bid[NeighAgentIcache +: IcachePerN] = icache_l2_miss_req_valid;
// Minion requests to arbiter
assign miss_req_arb_bid[NeighAgentMinion +: MinPerN]    = min_l2_req_bid;

// Round-robin arbiter between the different requestors
arb_rr
#(
    .NUM_REQS ( NeighNumReqAgents )
)
miss_req_arbiter
(
    // System signals
    .clock    ( clock_lv              ),
    .reset    ( reset_w_neigh_ff      ),
    // Bidding
    .reqs     ( miss_req_arb_bid      ),
    .pop      ( miss_req_ready        ),
    // Grant
    .grants   ( miss_req_arb_grant    )
);

// Grant signals for requestors
// They indicate next granted requestor but do not necessarily grant
// current cycle (only if miss_req_ready is enabled)
assign min_l2_req_arb_grant             = miss_req_arb_grant[NeighAgentMinion +: MinPerN];
assign icache_l2_miss_req_arb_grant     = miss_req_arb_grant[NeighAgentIcache +: IcachePerN];
assign ptw_l2_miss_req_arb_grant        = miss_req_arb_grant[NeighAgentPtw    +: PtwPerN];
assign tbox_l2_miss_req_arb_grant       = miss_req_arb_grant[NeighAgentTbox   +: 1];
assign coop_tload_l2_miss_req_arb_grant = miss_req_arb_grant[NeighAgentTload  +: 1];
assign tstore_l2_miss_req_arb_grant     = miss_req_arb_grant[NeighAgentTstore +: 1];

// Ready signals for requestors
// They do grant this cycle to the indicated requestor
assign min_l2_req_ready          = miss_req_ready ? min_l2_req_arb_grant             : '0;
assign icache_l2_miss_req_ready  = miss_req_ready ? icache_l2_miss_req_arb_grant     : '0;
assign ptw_l2_miss_req_ready     = miss_req_ready ? ptw_l2_miss_req_arb_grant        : '0;
assign tbox_miss_req_ready       = miss_req_ready ? tbox_l2_miss_req_arb_grant       : '0;
assign coop_tload_miss_req_ready = miss_req_ready ? coop_tload_l2_miss_req_arb_grant : '0;
assign tstore_miss_req_ready     = miss_req_ready ? tstore_l2_miss_req_arb_grant     : '0;

// Miss arbiter request muxing
// Multiplexing is done in two stages, as a 16:1 is too big for timing closure
always_comb
begin
    // Stage 1
    // -------
    // Multiplexer 0 (4:1 for minions 0-3)
    // -----------------------------------
    // Default to minion 0
    miss_req_info_xx_next[0] = min_l2_req_info[0];
    miss_req_data_xx_next[0] = min_l2_req_data[0];

    // Requests from other minions
    for(integer unsigned i = 1; i < MinPerN/2; i++)
    begin : miss_req_info_min0_3_for
        if(min_l2_req_arb_grant[i])
        begin
            miss_req_info_xx_next[0] = min_l2_req_info[i];
            miss_req_data_xx_next[0] = min_l2_req_data[i];
        end
    end

    // Valid for FIFOs
    miss_req_valid_xx_next[0] = |miss_req_arb_grant[NeighAgentMinion +: MinPerN/2];

    // Multiplexer 1 (4:1 for minions 4-7)
    // -----------------------------------
    // Default to minion 4
    miss_req_info_xx_next[1] = min_l2_req_info[MinPerN/2];
    miss_req_data_xx_next[1] = min_l2_req_data[MinPerN/2];

    // Requests from other minions
    for(integer unsigned i = MinPerN/2+1; i < MinPerN; i++)
    begin : miss_req_info_min4_7_for
        if(min_l2_req_arb_grant[i])
        begin
            miss_req_info_xx_next[1] = min_l2_req_info[i];
            miss_req_data_xx_next[1] = min_l2_req_data[i];
        end
    end

    // Valid for FIFOs
    miss_req_valid_xx_next[1] = |miss_req_arb_grant[NeighAgentMinion+MinPerN/2 +: MinPerN/2];

    // Multiplexer 2 (4:1 for Icache, PTWs and TBOX)
    // ---------------------------------------------
    // Default to miss request from Icache
    //if (icache_l2_miss_req_arb_grant)
    miss_req_info_xx_next[2].id        = icache_l2_miss_req_info.id;
    miss_req_info_xx_next[2].source    = etlink_pkg::SrcIcache;
    miss_req_info_xx_next[2].wdata     = icache_l2_miss_req_info.wdata;
    miss_req_info_xx_next[2].opcode    = icache_l2_miss_req_info.opcode;
    miss_req_info_xx_next[2].subopcode = icache_l2_miss_req_info.subopcode;
    miss_req_info_xx_next[2].address   = icache_l2_miss_req_info.address;
    miss_req_info_xx_next[2].size      = icache_l2_miss_req_info.size;
    miss_req_info_xx_next[2].qwen      = icache_l2_miss_req_info.qwen;
    // Data not used in this multiplexer
    miss_req_data_xx_next[2]           = '0;

    // Miss request from PTWs
    for (integer unsigned i = 0; i < PtwPerN; i++)
    begin : miss_req_info_ptw_for
        if (ptw_l2_miss_req_arb_grant[i])
        begin
            miss_req_info_xx_next[2].id        = ptw_l2_miss_req_info[i].id;
            miss_req_info_xx_next[2].source    = source_from_ptw(i[PtwPerNW-1:0]);
            miss_req_info_xx_next[2].wdata     = ptw_l2_miss_req_info[i].wdata;
            miss_req_info_xx_next[2].opcode    = ptw_l2_miss_req_info[i].opcode;
            miss_req_info_xx_next[2].subopcode = ptw_l2_miss_req_info[i].subopcode;
            miss_req_info_xx_next[2].address   = ptw_l2_miss_req_info[i].address;
            miss_req_info_xx_next[2].size      = ptw_l2_miss_req_info[i].size;
            miss_req_info_xx_next[2].qwen      = ptw_l2_miss_req_info[i].qwen;
        end
    end

    // Miss request from TBOX
    if (tbox_l2_miss_req_arb_grant)
    begin
        miss_req_info_xx_next[2].id        = tbox_miss_req_info.id;
        miss_req_info_xx_next[2].source    = etlink_pkg::SrcTbox;
        miss_req_info_xx_next[2].wdata     = tbox_miss_req_info.wdata;
        miss_req_info_xx_next[2].opcode    = tbox_miss_req_info.opcode;
        miss_req_info_xx_next[2].subopcode = tbox_miss_req_info.subopcode;
        miss_req_info_xx_next[2].address   = tbox_miss_req_info.address;
        miss_req_info_xx_next[2].size      = tbox_miss_req_info.size;
        miss_req_info_xx_next[2].qwen      = tbox_miss_req_info.qwen;
    end

    // Valid for FIFOs
    miss_req_valid_xx_next[2] = |miss_req_arb_grant[NeighAgentIcache +: IcachePerN + PtwPerN + 1];

    // Multiplexer 3 (2:1 for cooperative TLoad and TStore)
    // ----------------------------------------------------
    // Default to miss request from cooperative TLoad
    //if (coop_tload_l2_miss_req_arb_grant)
    miss_req_info_xx_next[3].id        = coop_tload_miss_req_info.id;
    miss_req_info_xx_next[3].source    = etlink_pkg::SrcCoop;
    miss_req_info_xx_next[3].wdata     = coop_tload_miss_req_info.wdata;
    miss_req_info_xx_next[3].opcode    = coop_tload_miss_req_info.opcode;
    miss_req_info_xx_next[3].subopcode = coop_tload_miss_req_info.subopcode;
    miss_req_info_xx_next[3].address   = coop_tload_miss_req_info.address;
    miss_req_data_xx_next[3]           = coop_tload_miss_req_info.data;
    miss_req_info_xx_next[3].size      = coop_tload_miss_req_info.size;
    miss_req_info_xx_next[3].qwen      = coop_tload_miss_req_info.qwen;

    // Miss request from TStore
    if (tstore_l2_miss_req_arb_grant)
    begin
        miss_req_info_xx_next[3].id        = tstore_evict_req.id;
        miss_req_info_xx_next[3].source    = tstore_evict_req.source;
        miss_req_info_xx_next[3].wdata     = tstore_evict_req.wdata;
        miss_req_info_xx_next[3].opcode    = tstore_evict_req.opcode;
        miss_req_info_xx_next[3].subopcode = tstore_evict_req.subopcode;
        miss_req_info_xx_next[3].address   = tstore_evict_req.address;
        miss_req_data_xx_next[3]           = tstore_evict_req.data;
        miss_req_info_xx_next[3].size      = tstore_evict_req.size;
        miss_req_info_xx_next[3].qwen      = tstore_evict_req.qwen;
    end

    // Valid for FIFOs
    miss_req_valid_xx_next[3] = |miss_req_arb_grant[NeighAgentTload +: 2];

    // Stage 2: Multiplexer 4:1
    // ------------------------
    miss_req_info = miss_req_info_xx[0];
    miss_req_data = miss_req_data_xx[0];

    for (integer unsigned i = 1; i < 4; i++)
    begin
        if (miss_req_valid_xx[i])
        begin : miss_req_info_for
            miss_req_info = miss_req_info_xx[i];
            miss_req_data = miss_req_data_xx[i];
        end
    end

    // Valid for FIFOs
    miss_req_valid = |miss_req_valid_xx;

    // Miss request is not accepted:
    // - while intermediate FIFO is full if going to output FIFOs
    // - while TBOX FIFO is full if going to TBOX
    miss_req_ready = miss_req_goes_to_tbox ? min_tbox_req_ready
                                           : ~int_req_fifo_full;
end

assign miss_req_accepted_xx = miss_req_valid_xx_next & {4{miss_req_ready}};
assign miss_req_accepted    = miss_req_valid & miss_req_ready;

genvar miss_req_xx_idx;
generate for (miss_req_xx_idx = 0; miss_req_xx_idx < 4; miss_req_xx_idx++)
begin : MISS_REQ_INFO_XX_FF
    //          CLK       RST               EN                                                                                    DOUT                                DIN                                      DEF
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_info_xx[miss_req_xx_idx] <= '0;
  end else if (miss_req_accepted_xx[miss_req_xx_idx]) begin
    miss_req_info_xx[miss_req_xx_idx] <= miss_req_info_xx_next[miss_req_xx_idx];
  end
end
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_data_xx[miss_req_xx_idx] <= '0;
  end else if (miss_req_accepted_xx[miss_req_xx_idx] & miss_req_info_xx_next[miss_req_xx_idx].wdata) begin
    miss_req_data_xx[miss_req_xx_idx] <= miss_req_data_xx_next[miss_req_xx_idx];
  end
end
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_valid_xx[miss_req_xx_idx] <= 1'b0;
  end else if (miss_req_ready) begin
    miss_req_valid_xx[miss_req_xx_idx] <= miss_req_valid_xx_next[miss_req_xx_idx];
  end
end
end
endgenerate

//          CLK    RST               EN                                       DOUT                DIN             DEF
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_info_ff <= '0;
  end else if (miss_req_accepted) begin
    miss_req_info_ff <= miss_req_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_data_ff <= '0;
  end else if (miss_req_accepted & miss_req_info.wdata) begin
    miss_req_data_ff <= miss_req_data;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_valid_ff <= 1'b0;
  end else if (miss_req_ready) begin
    miss_req_valid_ff <= miss_req_valid;
  end
end

assign miss_req_accepted_ff = miss_req_valid_ff & miss_req_ready;

// Process request to adapt it to output format before being sent
always_comb
begin
    miss_req_info_ff_proc_next = miss_req_info_ff;
    miss_req_data_ff_proc_next = miss_req_data_ff;

    // Process SCP address from minion formats into common L2 format
    miss_req_info_ff_proc_next.address = commonL2AddrFormat(miss_req_info_ff.address);

    // If it is a ScpFill, address in the data field must be also converted
    // into common L2 format
    if (miss_req_info_ff.opcode == etlink_pkg::ReqScpFill)
        miss_req_data_ff_proc_next[PaSize-1:0] = commonL2AddrFormat(miss_req_data_ff[PaSize-1:0]);

    // If it is a WriteAround going to local scratchpad, convert it into
    // regular write to avoid being sent to the shire cache coalescing buffer
    if (isWriteAroundtoLocalSCP(miss_req_info_ff_proc_next,shire_id_lo))
        miss_req_info_ff_proc_next.opcode = etlink_pkg::ReqWrite;
end

// Miss request destination FIFO
assign miss_req_info_dest_fifo_next = destFifo(miss_req_info_ff_proc_next,shire_id_lo,esr_neigh_chicken);

// Check whether the miss request goes into the TBOX FIFO
assign miss_req_goes_to_tbox_next = miss_req_info_dest_fifo_next == TboxFifo;

//          CLK       RST               EN                                                       DOUT                     DIN                           DEF
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_info_ff_proc <= '0;
  end else if (miss_req_accepted_ff) begin
    miss_req_info_ff_proc <= miss_req_info_ff_proc_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_data_ff_proc <= '0;
  end else if (miss_req_accepted_ff & miss_req_info_ff_proc_next.wdata) begin
    miss_req_data_ff_proc <= miss_req_data_ff_proc_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_info_dest_fifo <= '0;
  end else if (miss_req_accepted_ff) begin
    miss_req_info_dest_fifo <= miss_req_info_dest_fifo_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_goes_to_tbox <= 1'b0;
  end else if (miss_req_accepted_ff) begin
    miss_req_goes_to_tbox <= miss_req_goes_to_tbox_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    miss_req_valid_ff_proc <= 1'b0;
  end else if (miss_req_ready) begin
    miss_req_valid_ff_proc <= miss_req_valid_ff;
  end
end

////////////////////////////////////////////////////////////////////////////////
// Intermediate FIFO before going to output (per-bank/UC) FIFOs
////////////////////////////////////////////////////////////////////////////////

// The miss request goes into the intermediate FIFO if:
// - It does not go to TBOX FIFO and
// - There is not room in the corresponding output FIFO or
// - There are pending requests to the same output FIFO in the intermediate FIFO or
// - The intermediate FIFO is writing to any output FIFO
assign miss_req_goes_to_int_fifo = !miss_req_goes_to_tbox &&
                                   (out_req_fifo_full[miss_req_info_dest_fifo[OutFifosW-1:0]] ||
                                    (int_req_fifo_counter[miss_req_info_dest_fifo[OutFifosW-1:0]] != '0) ||
                                    int_req_info_pop);

// Miss request is pushed into the intermediate FIFO if it goes there
assign int_req_push       = miss_req_valid_ff_proc & miss_req_goes_to_int_fifo & ~int_req_fifo_full;
// Generate early valid for intermediate FIFO
assign int_req_push_early = miss_req_accepted_ff | (miss_req_valid_ff_proc & miss_req_goes_to_int_fifo & int_req_fifo_full);
// FIFO push data
assign int_req_info_in = miss_req_info_ff_proc;
assign int_req_data_in = miss_req_data_ff_proc;

// FIFO push data output FIFO
assign int_req_info_push_out_fifo = miss_req_info_dest_fifo[OutFifosW-1:0];

always_comb
begin
    for (integer fifo_idx = 0; fifo_idx < OutFifos; fifo_idx++)
    begin: int_fifo_per_out_fifo_push_pop_for
        // FIFO push per output FIFO
        int_req_push_per_out_fifo[fifo_idx] = int_req_push && (int_req_info_push_out_fifo == fifo_idx[OutFifosW-1:0]);

        // FIFO pop per output FIFO
        int_req_pop_per_out_fifo[fifo_idx] = int_req_info_pop && (int_req_info_pop_out_fifo == fifo_idx[OutFifosW-1:0]);
    end
end

// Destination FIFO counters
//       CLK       RST               DOUT                  DIN                        DEF
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    int_req_fifo_counter <= '0;
  end else begin
    int_req_fifo_counter <= int_req_fifo_counter_next;
  end
end

always_comb
begin
    int_req_fifo_counter_next = int_req_fifo_counter;
    for (integer fifo_idx = 0; fifo_idx < OutFifos; fifo_idx++)
    begin: int_fifo_counter
        // Incremented when pushing an element for this FIFO
        if (int_req_push_per_out_fifo[fifo_idx] && !int_req_pop_per_out_fifo[fifo_idx])
            int_req_fifo_counter_next[fifo_idx] = int_req_fifo_counter[fifo_idx] + 1'b1;
        // Decremented when popping an element for this FIFO
        else if (!int_req_push_per_out_fifo[fifo_idx] && int_req_pop_per_out_fifo[fifo_idx])
            int_req_fifo_counter_next[fifo_idx] = int_req_fifo_counter[fifo_idx] - 1'b1;

    end
end

gen_fifo
#(
    .ELEM_SIZE   ( OutFifosW +
                   $bits(et_link_nodata_req_info_t) ),
    .NUM_ELEMS   ( NeighIntFifoSize             ),
    .USE_LATCHES ( 1                                )
)
int_req_info_fifo
(
    // System signals
    .clock       ( clock_lv                         ),
    .reset       ( reset_w_neigh_ff                 ),
    // Push data
    .push_early  ( int_req_push_early               ),
    .push        ( int_req_push                     ),
    .push_data   ( {int_req_info_push_out_fifo,
                    int_req_info_in}                ),
    .full        ( int_req_fifo_full                ),
    .full_next   (                                  ),
    // Pop data
    .pop         ( int_req_info_pop                 ),
    .pop_data    ( {int_req_info_pop_out_fifo,
                    int_req_info_out}               ),
    .valid       ( int_req_info_valid               ),
    .valid_next  (                                  )
);

gen_fifo
#(
    .ELEM_SIZE   ( NeighDataSize      ),
    .NUM_ELEMS   ( NeighIntFifoSize  ),
    .USE_LATCHES ( 1                     )
)
int_req_data_fifo
(
    // System signals
    .clock       ( clock_lv              ),
    .reset       ( reset_w_neigh_ff      ),
    // Push data
    .push_early  ( int_req_push_early    ),
    .push        ( int_req_push &
                   int_req_info_in.wdata ),
    .push_data   ( int_req_data_in       ),
    .full        (                       ),
    .full_next   (                       ),
    // Pop data
    .pop         ( int_req_data_pop      ),
    .pop_data    ( int_req_data_out      ),
    .valid       (                       ),
    .valid_next  (                       )
);


// Miss request is accepted into the destination FIFO if it is not full
assign int_req_info_pop = int_req_info_valid && !out_req_fifo_full[int_req_info_pop_out_fifo];
assign int_req_data_pop = int_req_info_pop & int_req_info_out.wdata;

// Output FIFO push
always_comb
begin
    for (integer unsigned fifo_idx = 0; fifo_idx < OutFifos; fifo_idx++)
        // Miss request is accepted into this FIFO if it is accepted from either
        // the intermediate FIFO or the miss request arbiter
        out_req_push[fifo_idx] = (int_req_info_pop && (int_req_info_pop_out_fifo == fifo_idx[OutFifosW-1:0])) ||
                                 (!int_req_info_pop && miss_req_valid_ff_proc && miss_req_ready && !miss_req_goes_to_int_fifo && (miss_req_info_dest_fifo == fifo_idx[DestFifosW-1:0]));
end

// Output FIFO push data
assign out_req_info_in = int_req_info_pop ? int_req_info_out : miss_req_info_ff_proc;
assign out_req_data_in = int_req_data_pop ? int_req_data_out : miss_req_data_ff_proc;

// Check whether this is one half of a full line evict
// ReadCoop operation carries configuration in the data field but specified size is for read data returned in the response
assign out_req_is_line_evict = out_req_info_in.wdata && (out_req_info_in.size == etlink_pkg::SizeLine) && (out_req_info_in.opcode != etlink_pkg::ReqReadCoop);

////////////////////////////////////////////////////////////////////////////////////
// Per-bank FIFOs that store the requests to the L2 and do the voltage crossing sync
////////////////////////////////////////////////////////////////////////////////////
genvar l2_bank;
generate for (l2_bank = 0; l2_bank < ScBanks; l2_bank++)
begin: BANK_FIFOS
    logic                     bank_req_info_push;
    logic                     bank_req_in_is_first_half;
    logic                     bank_req_is_second_half;
    logic                     bank_req_data_push;
    logic                     bank_req_info_pop;
    logic                     bank_req_data_pop;
    logic                     bank_req_out_is_first_half;
    et_link_nodata_req_info_t bank_req_info_out;
    logic [NeighDataSize-1:0] bank_req_data_out;

    logic                     bank_req_info_valid;

    // Push logic for LV
    // -------------------------------------------------------------------------
    //          CLK       RST               EN                                          DOUT                     DIN                       DEF
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    bank_req_is_second_half <= 1'b0;
  end else if (bank_req_info_push & out_req_is_line_evict) begin
    bank_req_is_second_half <= ~bank_req_is_second_half;
  end
end

    assign bank_req_info_push         = out_req_push[l2_bank];
    assign bank_req_data_push         = out_req_push[l2_bank] & out_req_info_in.wdata;
    // Obtain and store whether this is the first half of a full line evict
    assign bank_req_in_is_first_half  = out_req_is_line_evict & ~bank_req_is_second_half;
    assign out_req_fifo_full[l2_bank] = ~out_req_fifo_ready[l2_bank];

    // -------------------------------------------------------------------------
    // ET-Link request info FIFO
    vcfifo_wr_lov_ss
    #(
       .ELEM_SIZE          ( $bits(et_link_nodata_req_info_t) + 1           ),
       .NUM_ELEMS          ( NeighBankFifoSize                          )
    )
    bank_req_info_vcfifo_wr_lov
    (
      .clock_push          ( clock_lv                                       ),
      .reset_push          ( reset_w_neigh_ff                               ),
      .clock_pop           ( clock_shire                                    ),
      .reset_pop           ( reset_w_shire_rsync                            ),
      .push                ( bank_req_info_push                             ),
      .push_data           ( {bank_req_in_is_first_half,out_req_info_in}    ),
      .pop                 ( bank_req_info_pop                              ),
      .pop_data            ( {bank_req_out_is_first_half,bank_req_info_out} ),
      .ready               ( out_req_fifo_ready[l2_bank]                    ),
      .valid               ( bank_req_info_valid                            ),
      .dft__reset_byp_push ( dft__reset_byp_lv                              ),
      .dft__reset_push     ( dft__reset_lv                                  ),
      .dft__reset_byp_pop  ( dft__reset_byp_hv                              ),
      .dft__reset_pop      ( dft__reset_hv                                  )
    );

    // ET-Link request data FIFO
    vcfifo_wr_lov_ss
    #(
       .ELEM_SIZE          ( NeighDataSize      ),
       .NUM_ELEMS          ( NeighBankFifoSize )
    )
    bank_req_data_bank_vcfifo_wr_lov
    (
      .clock_push          ( clock_lv              ),
      .reset_push          ( reset_w_neigh_ff      ),
      .clock_pop           ( clock_shire           ),
      .reset_pop           ( reset_w_shire_rsync   ),
      .push                ( bank_req_data_push    ),
      .push_data           ( out_req_data_in       ),
      .pop                 ( bank_req_data_pop     ),
      .pop_data            ( bank_req_data_out     ),
      .ready               (                       ),
      .valid               (                       ),
      .dft__reset_byp_push ( dft__reset_byp_lv     ),
      .dft__reset_push     ( dft__reset_lv         ),
      .dft__reset_byp_pop  ( dft__reset_byp_hv     ),
      .dft__reset_pop      ( dft__reset_hv         )
    );

    // Pop logic for HV
    // -------------------------------------------------------------------------
    neigh_hv_logic_l2_bank
    neigh_hv_logic_l2_bank_pd_hv
    (
      .clk_shire_i ( clock_shire ),
      .rst_shire_ndm_ni ( ~(reset_w_shire_rsync) ),
      // BANK FIFO output (256 bits)
      .bank_req_out_is_first_half_i ( bank_req_out_is_first_half ),
      .bank_req_info_out_i ( bank_req_info_out ),
      .bank_req_data_out_i ( bank_req_data_out ),
      .bank_req_info_valid_i ( bank_req_info_valid ),
      .bank_req_info_pop_o ( bank_req_info_pop ),
      .bank_req_data_pop_o ( bank_req_data_pop ),
      // BANK FIFO output (512 bits)
      .miss_req_bank_info_o ( miss_req_bank_info[l2_bank] ),
      .miss_req_bank_valid_o ( miss_req_bank_valid[l2_bank] ),
      .miss_req_bank_ready_i ( miss_req_bank_ready[l2_bank] )
    );

end
endgenerate

///////////////////////////////////////////////////////////////////////////////////
// UC FIFO that stores requests to uncached blocks and do the voltage crossing sync
///////////////////////////////////////////////////////////////////////////////////
logic                     uc_req_info_push;
logic                     uc_req_data_push;
et_link_nodata_req_info_t uc_req_info_in;
logic [UcDataSize-1:0] uc_req_data_in;
logic                     uc_req_info_pop;
logic                     uc_req_data_pop;
et_link_nodata_req_info_t uc_req_info_out;
logic [UcDataSize-1:0] uc_req_data_out;
logic                     uc_req_info_pop_valid;

// UC FIFO push data
assign uc_req_info_push = out_req_push[UcFifo];
assign uc_req_data_push = out_req_push[UcFifo] & uc_req_info_in.wdata;
assign uc_req_info_in   = out_req_info_in;
assign uc_req_data_in   = out_req_data_in;
assign out_req_fifo_full[UcFifo] = ~out_req_fifo_ready[UcFifo];

// UC request info FIFO
vcfifo_wr_lov_ss
#(
   .ELEM_SIZE          ( $bits(et_link_nodata_req_info_t) ),
   .NUM_ELEMS          ( NeighBankFifoSize            )
)
uc_req_info_vcfifo_wr_lov
(
  .clock_push          ( clock_lv                         ),
  .reset_push          ( reset_w_neigh_ff                 ),
  .clock_pop           ( clock_shire                      ),
  .reset_pop           ( reset_w_shire_rsync              ),
  .push                ( uc_req_info_push                 ),
  .push_data           ( uc_req_info_in                   ),
  .pop                 ( uc_req_info_pop                  ),
  .pop_data            ( uc_req_info_out                  ),
  .ready               ( out_req_fifo_ready[UcFifo]      ),
  .valid               ( uc_req_info_pop_valid            ),
  .dft__reset_byp_push ( dft__reset_byp_lv                ),
  .dft__reset_push     ( dft__reset_lv                    ),
  .dft__reset_byp_pop  ( dft__reset_byp_hv                ),
  .dft__reset_pop      ( dft__reset_hv                    )
);

// UC request data FIFO
vcfifo_wr_lov_ss
#(
   .ELEM_SIZE ( NeighDataSize      ),
   .NUM_ELEMS ( NeighBankFifoSize )
)
uc_req_data_vcfifo_wr_lov
(
  .clock_push          ( clock_lv          ),
  .reset_push          ( reset_w_neigh_ff  ),
  .clock_pop           ( clock_shire       ),
  .reset_pop           ( reset_w_shire_rsync    ),
  .push                ( uc_req_data_push  ),
  .push_data           ( uc_req_data_in    ),
  .pop                 ( uc_req_data_pop   ),
  .pop_data            ( uc_req_data_out   ),
  .ready               (                   ),
  .valid               (                   ),
  .dft__reset_byp_push ( dft__reset_byp_lv ),
  .dft__reset_push     ( dft__reset_lv     ),
  .dft__reset_byp_pop  ( dft__reset_byp_hv ),
  .dft__reset_pop      ( dft__reset_hv     )
);

///////////////////////////////////////////////////////////////////////////////////
// Logic for HV interface:
// - UC FIFO pop logic
// - BANK/UC FIFO output request arbiter to the L2
///////////////////////////////////////////////////////////////////////////////////
et_link_req_info_t neigh_sc_req_info_struct;

neigh_hv_logic_neigh_sc
neigh_hv_logic_neigh_sc_pd_hv
(
  .clk_shire_i ( clock_shire ),
  .rst_shire_ndm_ni ( ~(reset_w_shire_rsync) ),
   // BANK FIFO outputs (512 bits)
  .miss_req_bank_info_i ( miss_req_bank_info ),
  .miss_req_bank_valid_i ( miss_req_bank_valid ),
  .miss_req_bank_ready_o ( miss_req_bank_ready ),
   // UC FIFO output
  .uc_req_info_out_i ( uc_req_info_out ),
  .uc_req_data_out_i ( uc_req_data_out ),
  .uc_req_info_pop_valid_i ( uc_req_info_pop_valid ),
  .uc_req_info_pop_o ( uc_req_info_pop ),
  .uc_req_data_pop_o ( uc_req_data_pop ),
  // Output to Shire Cache
  .neigh_sc_req_info_o ( neigh_sc_req_info_struct ),
  .neigh_sc_req_valid_o ( neigh_sc_req_valid ),
  .neigh_sc_req_ready_i ( neigh_sc_req_ready )
);

assign neigh_sc_req_info = neigh_sc_req_info_struct;

////////////////////////////////////////////////////////////////////////////////
// Arbiter and FIFO for L2/TBOX responses
////////////////////////////////////////////////////////////////////////////////
assign neigh_sc_rsp_ready_lo = fill_fifo_in_rsp_ready;

//         CLK    RST               EN                                              DOUT                    DIN                    DEF
always_ff @(posedge clock_lv) begin
  if (fill_fifo_in_rsp_ready & neigh_sc_rsp_valid_lo) begin
    fill_fifo_in_rsp_info <= neigh_sc_rsp_info_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    fill_fifo_in_rsp_valid <= 1'b0;
  end else if (fill_fifo_in_rsp_ready) begin
    fill_fifo_in_rsp_valid <= neigh_sc_rsp_valid_lo;
  end
end

// FIFO
assign fill_fifo_in_rsp_ready = ~fill_fifo_full;
assign fill_fifo_push         = fill_fifo_in_rsp_valid & ~fill_fifo_full;

// Obtain cooperative information from cooperative TLoad tag table
assign coop_tload_tag_table_cam_pop = fill_fifo_push
                                    & (fill_fifo_in_rsp_info.opcode != etlink_pkg::RspMsgRcvData)
                                    & (etlink_pkg::neigh_source_e'(fill_fifo_in_rsp_info.dest) == etlink_pkg::SrcCoop);
// Take tag to CAM from fill response id field
assign coop_tload_tag_table_cam_tag = fill_fifo_in_rsp_info.id;

neigh_fill_fifo
fill_fifo
(
    // System signals
    .clk_i ( clock_lv ),
    .rst_ni ( ~(reset_w_neigh_ff) ),
    // Push data
    .push_i ( fill_fifo_push ),
    .push_data_i ( fill_fifo_in_rsp_info ),
    .coop_data_i ( coop_tload_tag_table_cam_data ),
    .full_o ( fill_fifo_full ),
    // Pop data
    .out_rsp_ready_i ( fill_ff_available ),
    .out_rsp_valid_o ( fill_fifo_out_rsp_valid ),
    .out_rsp_info_o ( fill_fifo_out_rsp_info )
);

////////////////////////////////////////////////////////////////////////////////
// This is the icache, shared between all the minions in the neigh
////////////////////////////////////////////////////////////////////////////////
frontend_icache_req icache_req_struct [MinPerN];
icache_frontend_resp icache_resp_struct [IcacheNrReqs];
esr_icache_err_log_ctl_t                   esr_icache_err_log_ctl;
logic                                      esr_icache_err_log_sbe;
logic                                      esr_icache_err_log_dbe;
icache_err_log_info_t                      esr_icache_err_log_info;
logic                                      esr_icache_ecc_count_ov;
minion_ptw_req ic_ptw_req_data [PtwPerN];
logic [PtwPerN-1:0]                     ic_ptw_req_valid;
logic [PtwPerN-1:0]                     ic_ptw_req_ready;
logic [PtwPerN-1:0]                     ic_ptw_invalidate;
minion_ptw_pte ptw_ic_resp_data [PtwPerN];
logic [PtwPerN-1:0]                     ptw_ic_resp_valid;
logic                                      icache_l2_miss_resp_ready;
logic                                      icache_l2_miss_resp_valid;
et_link_rsp_info_t                         icache_l2_miss_resp_info;

minion_satp_info satp_info_unpacked [MinPerN];
minion_satp_info matp_info_unpacked [MinPerN];

always_comb begin
    for (integer i = 0; i < MinPerN; i++) begin
        icache_req_struct[i] = frontend_icache_req'(icache_req[i]);
        satp_info_unpacked[i] = minion_satp_info'(satp_info[i]);
        matp_info_unpacked[i] = minion_satp_info'(matp_info[i]);
    end
    for (integer i = 0; i < IcacheNrReqs; i++) begin
        icache_resp[i] = icache_resp_struct[i];
    end
end

neigh_shared_icache
#(
    .NrMinions                  ( MinPerIcache                   ),
    .NrRequestors               ( IcacheNrReqs                   )
)
shared_icache
(
    // System signals
    .clk_i ( clock_lv ),
    .rst_ni ( ~(reset_w_neigh_ff) ),
    // ESRs
    .esr_prefetch_conf_i ( icache_prefetch_conf_t'(esr_icache_prefetch_conf_lo ) ),
    .esr_prefetch_start_i ( esr_icache_prefetch_start_lo ),
    .esr_prefetch_done_o ( esr_icache_prefetch_done_lo ),
    .esr_err_log_ctl_i ( esr_icache_err_log_ctl ),
    .esr_err_log_sbe_o ( esr_icache_err_log_sbe ),
    .esr_err_log_dbe_o ( esr_icache_err_log_dbe ),
    .esr_err_log_info_o ( esr_icache_err_log_info ),
    .esr_mprot_i ( esr_mprot_t'(mprot )               ),
    .esr_vmspagesize_i ( tlb_entry_type'(vmspagesize )      ),
    .esr_bypass_icache_i ( esr_neigh_chicken.bypass_icache ),
    .esr_shire_coop_mode_i ( esr_shire_coop_mode_lo ),
    // Request port
    .f0_req_ready_o ( icache_req_ready ),
    .f0_req_valid_i ( icache_req_valid ),
    .f0_req_i ( icache_req_struct ),
    // Response
    .f4_resp_valid_o ( icache_resp_valid ),
    .f4_resp_miss_o ( icache_resp_miss ),
    .f4_resp_o ( icache_resp_struct ),
    .f5_resp_fill_done_o ( icache_fill_done ),
    // Flush control
    .f0_flush_data_i ( icache_flush_data ),
    // Request to L2 port
    .f0_l2_miss_req_disable_next_i ( evict_second_access_pending ),
    .f0_l2_miss_req_ready_i ( icache_l2_miss_req_ready ),
    .f0_l2_miss_req_valid_o ( icache_l2_miss_req_valid ),
    .f0_l2_miss_req_o ( icache_l2_miss_req_info ),
    // Fill from L2 port
    .f0_l2_miss_resp_ready_o ( icache_l2_miss_resp_ready ),
    .f0_l2_miss_resp_valid_i ( icache_l2_miss_resp_valid ),
    .f0_l2_miss_resp_i ( icache_l2_miss_resp_info ),
    .satp_info_i ( satp_info_unpacked ),
    .matp_info_i ( matp_info_unpacked ),
    .tlb_invalidate_i ( tlb_invalidate ),
    // PTW request
    .ptw_req_data_o ( ic_ptw_req_data ),
    .ptw_req_valid_o ( ic_ptw_req_valid ),
    .ptw_req_ready_i ( ic_ptw_req_ready ),
    .ptw_invalidate_o ( ic_ptw_invalidate ),
    // PTW response
    .ptw_resp_valid_i ( ptw_ic_resp_valid ),
    .ptw_resp_data_i ( ptw_ic_resp_data ),
    // Request to L1 SRAM blocks
    .icache_f2_sram_req_write_o ( icache_f2_sram_req_write_lo ),
    .icache_f2_sram_req_addr_o ( icache_f2_sram_req_addr_lo ),
    .icache_f2_sram_req_valid_o ( icache_f2_sram_req_valid_lo ),
    .icache_f2_sram_req_ready_i ( icache_f2_sram_req_ready_lo ),
    // Response from L1 SRAM blocks
    .icache_f0_sram_resp_dout_i ( icache_f0_sram_resp_dout_lo ),
    .icache_f0_sram_resp_valid_i ( icache_f0_sram_resp_valid_lo ),
    .icache_f0_sram_resp_ready_o ( icache_f0_sram_resp_ready_lo ),
    // APB signals from BPAM for debug
    .apb_paddr_i ( APB_req_struct_icache.apb_paddr ),
    .apb_pwrite_i ( APB_req_struct_icache.apb_pwrite ),
    .apb_psel_i ( APB_req_struct_icache.apb_psel ),
    .apb_penable_i ( APB_req_struct_icache.apb_penable ),
    .apb_pwdata_i ( APB_req_struct_icache.apb_pwdata ),
    .apb_pready_o ( APB_rsp_struct_icache.apb_pready ),
    .apb_prdata_o ( APB_rsp_struct_icache.apb_prdata ),
    .apb_pslverr_o ( APB_rsp_struct_icache.apb_pslverr ),
    // Output debug signals going to Status Monitor
    .dbg_sm_signals_o ( icache_dbg_sm_signals )
); //shared_icache

assign icache_l2_miss_resp_info = fill_fifo_out_rsp_info[NeighAgentIcache];
assign icache_l2_miss_resp_valid = fill_fifo_out_rsp_valid[NeighAgentIcache];
assign fill_ff_available[NeighAgentIcache] = icache_l2_miss_resp_ready;

////////////////////////////////////////////////////////////////////////////////
// This is the PTW, shared between all the minions in the neigh
////////////////////////////////////////////////////////////////////////////////
et_link_rsp_info_t [PtwPerN-1:0] ptw_l2_miss_resp_info;
logic [PtwPerN-1:0]              ptw_l2_miss_resp_valid;
logic [PtwPerN-1:0]              ptw_l2_miss_resp_ready;
minion_ptw_pte [PtwPerN-1:0]     ptw_resp_data;

genvar ptw_it;
generate for (ptw_it = 0 ; ptw_it < PtwPerN ; ptw_it ++ )
begin : PTW
    minion_ptw_req ptw_req_data [MinPerPtw + 1];
    always_comb begin
        ptw_req_data[MinPerPtw] = ic_ptw_req_data[ptw_it];
        for (int unsigned i = 0; i < MinPerPtw; i++) begin
            ptw_req_data[i] = minion_ptw_req'(dc_ptw_req_data[ptw_it][i]);
        end
    end

    neigh_shared_ptw
    #(
        .NReqs                    ( MinPerPtw + 1                                       )
    )
    shared_ptw
    (
        // System signals
        .clk_i ( clock_lv ),
        .rst_ni ( ~(reset_w_neigh_ff) ),
        // Is this in the IOShire
        .ioshire_i ( 1'b0 ),
        // Requests
        .req_data_i ( ptw_req_data ),
        .req_valid_i ( {ic_ptw_req_valid[ptw_it], dc_ptw_req_valid[ptw_it]} ),
        .req_ready_o ( {ic_ptw_req_ready[ptw_it], dc_ptw_req_ready[ptw_it]} ),
        // Invalidates from dcache are not necessary,
        // as its origin is the same one as that from icache
        .invalidate_i ( {ic_ptw_invalidate[ptw_it], {MinPerPtw{1'b0}}} ),
        // Neighborhood ESRs
        .mprot_i ( esr_mprot_t'(mprot )                                    ),
        .vmspagesize_i ( tlb_entry_type'(vmspagesize )                           ),
        // Responses
        .resp_data_o ( ptw_resp_data[ptw_it] ),
        .resp_valid_o ( {ptw_ic_resp_valid[ptw_it], ptw_dc_resp_valid[ptw_it]} ),
        // Memory interface
        // L2 Request bus
        .mem_miss_req_disable_next_i ( evict_second_access_pending ),
        .mem_miss_req_info_o ( ptw_l2_miss_req_info[ptw_it] ),
        .mem_miss_req_valid_o ( ptw_l2_miss_req_valid[ptw_it] ),
        .mem_miss_req_ready_i ( ptw_l2_miss_req_ready[ptw_it] ),
        // L2 Fill/Ack bus
        .mem_fill_req_info_i ( ptw_l2_miss_resp_info[ptw_it] ),
        .mem_fill_req_valid_i ( ptw_l2_miss_resp_valid[ptw_it] ),
        .mem_fill_req_ready_o ( ptw_l2_miss_resp_ready[ptw_it] ),
        // Output debug signals going to Status Monitor
        .dbg_sm_signals_o ( ptw_dbg_sm_signals[ptw_it] )
    );

    assign ptw_dc_resp_data[ptw_it] = ptw_resp_data[ptw_it];
    assign ptw_ic_resp_data[ptw_it] = ptw_resp_data[ptw_it];

    assign ptw_l2_miss_resp_info[ptw_it] = fill_fifo_out_rsp_info[NeighAgentPtw + ptw_it];
    assign ptw_l2_miss_resp_valid[ptw_it] = fill_fifo_out_rsp_valid[NeighAgentPtw + ptw_it];
    assign fill_ff_available[NeighAgentPtw + ptw_it] = ptw_l2_miss_resp_ready[ptw_it];

end // block: PTW
endgenerate

////////////////////////////////////////////////////////////////////////////////
// Fast local message minion interconnection
////////////////////////////////////////////////////////////////////////////////
logic [MinPerN-1:0][NeighMaxLocalConnect-1:0]               local_channel_mask;
logic [MinPerN-1:0][NeighMaxLocalConnect-1:0][MinPerNW-1:0] local_dest_id;
logic [MinPerN-1:0]                                             local_message_ready_out;
logic [MinPerN-1:0][NeighMaxLocalConnect-1:0]               local_message_valid_out;
et_link_neigh_rsp_info_t [MinPerN-1:0]                          local_message_info_out;
logic [MinPerN-1:0]                                             local_message_ready_in;
logic [MinPerN-1:0]                                             local_message_valid_in;
et_link_neigh_rsp_info_t [MinPerN-1:0]                          local_message_info_in;

neigh_local_message_network
local_message_network
(
    // System signals
    .clk_i ( clock_lv ),
    .rst_ni ( ~(reset_w_neigh_ff) ),
    // Configuration signals
    .local_channel_mask_o ( local_channel_mask ),
    .local_dest_id_o ( local_dest_id ),
    // Local message outputs from evict FFs
    .local_message_ready_out_o ( local_message_ready_out ),
    .local_message_valid_out_i ( local_message_valid_out ),
    .local_message_info_out_i ( local_message_info_out ),
    // Local message inputs to fill FFs
    .local_message_ready_in_i ( local_message_ready_in ),
    .local_message_valid_in_o ( local_message_valid_in ),
    .local_message_info_in_o ( local_message_info_in ),
    // Output debug signals going to Status Monitor
    .dbg_sm_signals_o ( fln_dbg_sm_signals )
);

////////////////////////////////////////////////////////////////////////////////
// Generates the minions
////////////////////////////////////////////////////////////////////////////////
et_link_minion_miss_req_info_t [MinPerN-1:0]  l2_dcache_miss_req_struct;
et_link_minion_evict_req_info_t [MinPerN-1:0] l2_dcache_evict_req_struct;
et_link_minion_rsp_info_t [MinPerN-1:0]       l2_dcache_resp_struct;
esr_hactrl_t                                     esr_hactrl;
esr_hastatus0_t                                  esr_hastatus0;
esr_hastatus1_t                                  esr_hastatus1_min;
logic [MinPerN-1:0]                           l2_dcache_resp_tstore_ack;
logic [MinPerN-1:0]                           esr_thread0_enable_lo;
logic [MinPerN-1:0]                           esr_thread1_enable_lo;

genvar min_it;
generate for(min_it = 0; min_it < MinPerN; min_it++)
begin : MINION_FF
    logic [MinPerNW-1:0]            neigh_min_id;

    // Minion ID within the neighborhood
    assign neigh_min_id = min_it[MinPerNW-1:0];

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has its own FF to store a miss request. It waits until the
    // request is stored before going to arbiter to get access to the L2.
    // A given minion can't request a new cacheline until the FF is emptied
    ////////////////////////////////////////////////////////////////////////////

    assign l2_dcache_miss_req_struct[min_it] = et_link_minion_miss_req_info_t'(l2_dcache_miss_req[min_it]);

    neigh_miss_ff
    miss_ff
    (
        // System signals
        .clk_i ( clock_lv ),
        .rst_ni ( ~(reset_w_neigh_ff) ),
        // Miss request from minion
        .l2_dcache_req_i ( l2_dcache_miss_req_struct[min_it] ),
        .l2_dcache_req_valid_i ( l2_dcache_miss_req_valid[min_it] ),
        .l2_dcache_req_ready_o ( l2_dcache_miss_req_ready[min_it] ),
        // Data to L2
        .miss_req_o ( min_miss_req[min_it] ),
        .miss_valid_o ( min_miss_req_valid[min_it] ),
        .miss_ready_i ( min_miss_req_ready[min_it] ),
        // Data to cooperative tload module
        .coop_miss_req_o ( min_coop_miss_req[min_it] ),
        .coop_miss_valid_o ( min_coop_miss_req_valid[min_it] ),
        .coop_miss_ready_i ( min_coop_miss_req_ready[min_it] ),
        // Output debug signals going to Status Monitor
        .dbg_sm_signals_o ( miss_ff_dbg_sm_signals[min_it] )
    );

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has its own FF to store the evict. It waits until the 512b
    // of data are available before going to arbiter to get access to the L2.
    // A given minion can't evict a new cacheline until the FF is emptied
    ////////////////////////////////////////////////////////////////////////////

    assign l2_dcache_evict_req_struct[min_it] = et_link_minion_evict_req_info_t'(l2_dcache_evict_req[min_it]);

    neigh_evict_ff
    evict_ff
    (
        // System signals
        .clk_i ( clock_lv ),
        .rst_ni ( ~(reset_w_neigh_ff) ),
        .shire_id_i ( shire_id_lo ),
        .neigh_id_i ( neigh_id_lo ),
        // Evict from minion
        .l2_dcache_req_i ( l2_dcache_evict_req_struct[min_it] ),
        .l2_dcache_req_valid_i ( l2_dcache_evict_req_valid[min_it] ),
        .l2_dcache_req_ready_o ( l2_dcache_evict_req_ready[min_it] ),
        // Reduce local interconnect
        .local_channel_mask_i ( local_channel_mask[min_it] ),
        .local_dest_id_i ( local_dest_id[min_it] ),
        .local_message_ready_i ( local_message_ready_out[min_it] ),
        .local_message_valid_o ( local_message_valid_out[min_it] ),
        .local_message_info_o ( local_message_info_out[min_it] ),
        // Data to L2
        .evict_req_o ( min_evict_req[min_it] ),
        .evict_valid_o ( min_evict_req_valid[min_it] ),
        .evict_ready_i ( min_evict_req_ready[min_it] ),
        // Data to coopertaive TStore module
        .coop_evict_req_o ( coop_evict_req[min_it] ),
        .coop_evict_valid_o ( coop_evict_valid[min_it] ),
        .coop_evict_ready_i ( coop_evict_ready[min_it] ),
        // Output debug signals going to Status Monitor
        .dbg_sm_signals_o ( evict_ff_dbg_sm_signals[min_it] )
    );

    ////////////////////////////////////////////////////////////////////////////
    // Combine and flop miss and evict requests from minions
    ////////////////////////////////////////////////////////////////////////////
    logic                     min_miss_req_bid;
    logic                     min_evict_req_bid;
    logic                     min_evict_second_access_xx;
    logic                     min_evict_second_access_xx_next;
    et_link_nodata_req_info_t min_l2_req_info_xx;
    logic [NeighDataSize-1:0] min_l2_req_data_xx;
    logic                     min_l2_req_is_evict_xx;
    logic                     min_l2_req_valid_xx;
    logic                     min_l2_req_bid_xx;
    logic                     min_l2_req_ready_xx;

    //         CLK       RST               EN                                         DOUT                         DIN                              DEF
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    min_evict_second_access_xx <= '0;
  end else begin
    min_evict_second_access_xx <= min_evict_second_access_xx_next;
  end
end
    always_ff @(posedge clock_lv) begin
  if (min_l2_req_ready_xx & min_l2_req_valid_xx) begin
    min_l2_req_info[min_it] <= min_l2_req_info_xx;
  end
end
    always_ff @(posedge clock_lv) begin
  if (min_l2_req_ready_xx & min_l2_req_valid_xx) begin
    min_l2_req_data[min_it] <= min_l2_req_data_xx;
  end
end
    always_ff @(posedge clock_lv) begin
  if (min_l2_req_ready_xx & min_l2_req_valid_xx) begin
    min_l2_req_is_evict[min_it] <= min_l2_req_is_evict_xx;
  end
end
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    min_l2_req_valid[min_it] <= 1'b0;
  end else if (min_l2_req_ready_xx) begin
    min_l2_req_valid[min_it] <= min_l2_req_valid_xx;
  end
end
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    min_l2_req_bid[min_it] <= 1'b0;
  end else begin
    min_l2_req_bid[min_it] <= min_l2_req_bid_xx;
  end
end

    // If evict requests are to write 512 bits, they must be assigned two consecutive grants,
    // so that they are stored in two consecutive 256-bit FIFO entries
    // Thus, we disable miss request bids during the second evict access
    always_comb
    begin
        min_evict_second_access_xx_next = min_evict_second_access_xx;

        if (min_evict_req_ready[min_it] && (min_evict_req[min_it].size == etlink_pkg::SizeLine) && !min_evict_second_access_xx)
            min_evict_second_access_xx_next = 1'b1;
        else if (min_evict_req_ready[min_it] && min_evict_second_access_xx)
            min_evict_second_access_xx_next = 1'b0;
    end

    assign min_miss_req_bid  = min_miss_req_valid[min_it] & ~min_evict_second_access_xx;
    assign min_evict_req_bid = min_evict_req_valid[min_it];

    // Priority arbiter between evict and miss requests
    // with priority updated only if ready is given
    arb_prio_dly_rdy
    #(
        .NUM_CLIENTS ( 2                            )
    )
    min_req_arbiter
    (
        // System signals
        .clock      ( clock_lv                      ),
        .reset      ( reset_w_neigh_ff              ),

        // client side
        .c_valid    ( {min_evict_req_bid,
                       min_miss_req_bid}            ),
        .c_ready    ( {min_evict_req_ready[min_it],
                       min_miss_req_ready[min_it]}  ),
        .c_winner   (                               ),

        // destination side
        .valid      ( min_l2_req_valid_xx           ),
        .ready      ( min_l2_req_ready_xx           )
    );

    // minion miss/evict arbiter request muxing
    always_comb
    begin
        min_l2_req_info_xx = '0;
        min_l2_req_info_xx.source = source_from_minion(min_it[MinPerNW-1:0]);

        // Evict request from minions
        if (min_evict_req_ready[min_it])
        begin
            min_l2_req_info_xx.id        = min_evict_req[min_it].id;
            min_l2_req_info_xx.wdata     = min_evict_req[min_it].wdata;
            min_l2_req_info_xx.opcode    = min_evict_req[min_it].opcode;
            min_l2_req_info_xx.subopcode = min_evict_req[min_it].subopcode;
            min_l2_req_info_xx.address   = min_evict_req[min_it].address;
            min_l2_req_data_xx           = min_evict_req[min_it].data;
            min_l2_req_info_xx.size      = min_evict_req[min_it].size;
            min_l2_req_info_xx.qwen      = min_evict_req[min_it].qwen;
        end
        // Miss request from minions
        else
        begin
            min_l2_req_info_xx.id        = min_miss_req[min_it].id;
            min_l2_req_info_xx.wdata     = min_miss_req[min_it].wdata;
            min_l2_req_info_xx.opcode    = min_miss_req[min_it].opcode;
            min_l2_req_info_xx.subopcode = min_miss_req[min_it].subopcode;
            min_l2_req_info_xx.address   = min_miss_req[min_it].address;
            min_l2_req_data_xx           = min_miss_req[min_it].data;
            min_l2_req_info_xx.size      = min_miss_req[min_it].size;
            min_l2_req_info_xx.qwen      = min_miss_req[min_it].qwen;
        end
    end

    // Bids from miss and evict FF to miss request arbiter
    assign min_l2_req_is_evict_xx = min_evict_req_ready[min_it];
    assign min_l2_req_bid_xx      = (min_l2_req_ready_xx ? min_l2_req_valid_xx : min_l2_req_valid[min_it])
                                  & (~evict_second_access_pending | min_evict_second_access_next[min_it]);

    // Grant from miss request arbiter
    assign min_l2_req_ready_xx = min_l2_req_ready[min_it] | ~min_l2_req_valid[min_it];

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has a FF to store a fill coming from the L2. When a fill is
    // received (512b) it sequences the data to the minion in a 256b basis. Once
    // the data is drained, a new fill can be accepted for this minion
    ////////////////////////////////////////////////////////////////////////////

    // L2 response ( and Tbox requests) from fill FIFO to fill FF
    assign fill_fifo_min_resp[min_it]  = fill_fifo_out_rsp_info[NeighAgentMinion + min_it];
    assign fill_fifo_min_valid[min_it] = fill_fifo_out_rsp_valid[NeighAgentMinion + min_it];
    assign fill_ff_available[NeighAgentMinion + min_it] = fill_fifo_min_ready[min_it];

    neigh_fill_ff
    fill_ff
    (
        // System signals
        .clk_i ( clock_lv ),
        .rst_ni ( ~(reset_w_neigh_ff) ),
        // Fills from L2
        .fill_fifo_resp_i ( fill_fifo_min_resp[min_it] ),
        .fill_fifo_valid_i ( fill_fifo_min_valid[min_it] ),
        .fill_fifo_ready_o ( fill_fifo_min_ready[min_it] ),
        // Response from cooperative TensorStore
        .tstore_min_resp_i ( tstore_min_resp[min_it] ),
        .tstore_min_resp_valid_i ( tstore_min_resp_valid[min_it] ),
        .tstore_min_resp_ready_o ( tstore_min_resp_ready[min_it] ),
        // Reduce local interconnect
        .local_message_info_i ( local_message_info_in[min_it] ),
        .local_message_valid_i ( local_message_valid_in[min_it] ),
        .local_message_ready_o ( local_message_ready_in[min_it] ),
        // Messages from TBOX
        .tbox_min_rsp_info_i ( tbox_min_rsp_info_in[min_it] ),
        .tbox_min_rsp_valid_i ( tbox_min_rsp_valid_in[min_it] ),
        .tbox_min_rsp_ready_o ( tbox_min_rsp_ready_in[min_it] ),
        // Data to minion
        .l2_resp_o ( l2_dcache_resp_struct[min_it] ),
        .l2_resp_valid_o ( l2_dcache_resp_valid[min_it] ),
        .l2_resp_ready_i ( l2_dcache_resp_ready[min_it] ),
        // Indication to tensor store buffer of the Ack
        .tstore_min_ack_o ( l2_dcache_resp_tstore_ack[min_it] ),
        // Output debug signals going to Status Monitor
        .dbg_sm_signals_o ( fill_ff_dbg_sm_signals[min_it] )
    );

    assign l2_dcache_resp[min_it] = l2_dcache_resp_struct[min_it];

    // Minion ID within the shire
    assign shire_min_id[min_it] = { neigh_id_lo, neigh_min_id };

    // FF for thread enables
    //       CLK       RST               DOUT                    DIN                            DEF
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    thread0_enable[min_it] <= '0;
  end else begin
    thread0_enable[min_it] <= esr_thread0_enable_lo[min_it];
  end
end
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    thread1_enable[min_it] <= '0;
  end else begin
    thread1_enable[min_it] <= esr_thread1_enable_lo[min_it];
  end
end

    ////////////////////////////////////////////////////////////////////////////
    // Each minion has a FF for interrupt lines
    ////////////////////////////////////////////////////////////////////////////
    minion_interrupts interrupts_next;

    //       CLK       RST               DOUT                           DIN              DEF
    always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    interrupts_per_minion[min_it] <= '0;
  end else begin
    interrupts_per_minion[min_it] <= interrupts_next;
  end
end

    always_comb
    begin

        interrupts_next.fcc = '0;
        interrupts_next.fcc[uc_to_neigh_fcc_target_lo] = uc_to_neigh_fcc_lo[min_it];
        interrupts_next.mtip           = {minion_pkg::NrThreads{int_mtip_lo[min_it]}};
        interrupts_next.meip           = {minion_pkg::NrThreads{int_meip_lo}};
        interrupts_next.seip           = {minion_pkg::NrThreads{int_seip_lo}};
        interrupts_next.msip           = ipi_msip_lo[min_it*minion_pkg::NrThreads +: minion_pkg::NrThreads];

        for(integer i = 0; i < minion_pkg::NrThreads; i++)
        begin
            interrupts_next.ipi_with_pc[i] = ipi_redirect_trigger_lo[2*min_it+i];
            // IECO only connected to thread 0 of minion 0
            interrupts_next.mieco[i] = (min_it == 0) && (i == 0) && esr_icache_ecc_count_ov;
        end
        interrupts_next.ipi_pc         = esr_ipi_redirect_pc[PcSize-1:1]; // not connecting LSB, which is always 0
    end

end
endgenerate // minions

////////////////////////////////////////////////////////////////////////////////
// TBOX router
// Routes TBOX-L2 requests and replies and minion-TBOX messages back and forth
// through the TBOX buses
////////////////////////////////////////////////////////////////////////////////
et_link_neigh_req_info_t tbox_neigh_req_info_struct;
et_link_rsp_info_t       tbox_neigh_rsp_info_struct;
et_link_rsp_info_t       sc_tbox_rsp_info;
logic                    sc_tbox_rsp_valid;
logic                    sc_tbox_rsp_ready;
// TBOX message response intermediate north and sought stages
et_link_neigh_rsp_info_t tbox_min_north_ff_rsp;
et_link_neigh_msg_dest_t tbox_min_north_ff_rsp_msg_dest;
logic                    tbox_min_north_ff_available;
logic                    tbox_min_north_ff_available_next;
logic                    tbox_min_north_ff_in_accepted;
logic                    tbox_min_north_ff_out_accepted;
et_link_neigh_rsp_info_t tbox_min_south_ff_rsp;
et_link_neigh_msg_dest_t tbox_min_south_ff_rsp_msg_dest;
logic                    tbox_min_south_dest_is_north;
logic                    tbox_min_south2north_valid;
logic                    tbox_min_south_ff_available;
logic                    tbox_min_south_ff_available_next;
logic                    tbox_min_south_ff_in_accepted;
logic                    tbox_min_south_ff_out_accepted;

assign tbox_neigh_req_info_struct = et_link_neigh_req_info_t'(tbox_neigh_req_info);
assign tbox_neigh_rsp_info = tbox_neigh_rsp_info_struct;

// TBOX FIFO push data
always_comb
begin
  min_tbox_req_info.id        = miss_req_info_ff_proc.id;
  min_tbox_req_info.source    = miss_req_info_ff_proc.source;
  min_tbox_req_info.wdata     = miss_req_info_ff_proc.wdata;
  min_tbox_req_info.opcode    = miss_req_info_ff_proc.opcode;
  min_tbox_req_info.subopcode = miss_req_info_ff_proc.subopcode;
  min_tbox_req_info.address   = miss_req_info_ff_proc.address;
  min_tbox_req_info.size      = miss_req_info_ff_proc.size;
  min_tbox_req_info.qwen      = miss_req_info_ff_proc.qwen;
  min_tbox_req_info.data      = miss_req_data_ff_proc;
end
assign min_tbox_req_valid_early = miss_req_accepted_ff | (miss_req_valid_ff_proc & miss_req_goes_to_tbox & ~min_tbox_req_ready);
assign min_tbox_req_valid       = miss_req_valid_ff_proc & miss_req_goes_to_tbox;

// TBOX L2 replies
assign sc_tbox_rsp_info = fill_fifo_out_rsp_info[NeighAgentTbox];
assign sc_tbox_rsp_valid = fill_fifo_out_rsp_valid[NeighAgentTbox];
assign fill_ff_available[NeighAgentTbox] = sc_tbox_rsp_ready;

if (EnableGfx) begin : gen_gfx_enabled
    neigh_tbox_router
    tbox_router
    (
        // System signals
        .clk_i ( clock_lv ),
        .rst_ni ( ~(tbox_reset_w) ),
        // Minion requests
        .min_tbox_req_info_i ( min_tbox_req_info ),
        .min_tbox_req_valid_early_i ( min_tbox_req_valid_early ),
        .min_tbox_req_valid_i ( min_tbox_req_valid ),
        .min_tbox_req_ready_o ( min_tbox_req_ready ),
        // Minion TBOX reply bus
        .tbox_min_rsp_info_o ( tbox_min_rsp_info ),
        .tbox_min_rsp_valid_o ( tbox_min_rsp_valid ),
        .tbox_min_rsp_ready_i ( tbox_min_rsp_ready ),
        // TBOX L2 request
        .tbox_sc_req_disable_next_i ( evict_second_access_pending ),
        .tbox_sc_req_info_o ( tbox_miss_req_info ),
        .tbox_sc_req_valid_o ( tbox_miss_req_valid ),
        .tbox_sc_req_ready_i ( tbox_miss_req_ready ),
        // TBOX L2 reply
        .sc_tbox_rsp_info_i ( sc_tbox_rsp_info ),
        .sc_tbox_rsp_valid_i ( sc_tbox_rsp_valid ),
        .sc_tbox_rsp_ready_o ( sc_tbox_rsp_ready ),
        // TBOX neighborhood request bus
        .tbox_neigh_req_info_i ( tbox_neigh_req_info_struct ),
        .tbox_neigh_req_valid_i ( tbox_neigh_req_valid ),
        .tbox_neigh_req_ready_o ( tbox_neigh_req_ready ),
        // TBOX neighborhood response bus
        .tbox_neigh_rsp_info_o ( tbox_neigh_rsp_info_struct ),
        .tbox_neigh_rsp_valid_o ( tbox_neigh_rsp_valid ),
        .tbox_neigh_rsp_ready_i ( tbox_neigh_rsp_ready )
    );
end else begin : gen_gfx_disabled
    assign min_tbox_req_ready   = 1'b0;
    assign tbox_min_rsp_info    = '0;
    assign tbox_min_rsp_valid   = 1'b0;
    assign tbox_miss_req_info   = '0;
    assign tbox_miss_req_valid  = 1'b0;
    assign sc_tbox_rsp_ready    = 1'b0;
    assign tbox_neigh_req_ready = 1'b0;
    assign tbox_neigh_rsp_info_struct = '0;
    assign tbox_neigh_rsp_valid = 1'b0;
end

// Flop TBOX response in south and north intermediate stages
// connected in daisy chain from south to north

//      CLK       RST               EN                             DOUT                         DIN                               DEF
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    tbox_min_north_ff_available <= 1'b1;
  end else begin
    tbox_min_north_ff_available <= tbox_min_north_ff_available_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (tbox_min_north_ff_in_accepted) begin
    tbox_min_north_ff_rsp <= tbox_min_south_ff_rsp;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    tbox_min_south_ff_available <= 1'b1;
  end else begin
    tbox_min_south_ff_available <= tbox_min_south_ff_available_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (tbox_min_south_ff_in_accepted) begin
    tbox_min_south_ff_rsp <= tbox_min_rsp_info;
  end
end

assign tbox_min_north_ff_rsp_msg_dest = et_link_neigh_msg_dest_t'(tbox_min_north_ff_rsp.dest);
assign tbox_min_north_ff_in_accepted  = tbox_min_south2north_valid & tbox_min_north_ff_available;
assign tbox_min_south_ff_rsp_msg_dest = et_link_neigh_msg_dest_t'(tbox_min_south_ff_rsp.dest);
assign tbox_min_south_ff_in_accepted  = tbox_min_rsp_valid & tbox_min_south_ff_available;
assign tbox_min_south_dest_is_north   = tbox_min_south_ff_rsp_msg_dest.min_id < MinPerN/2;
assign tbox_min_south2north_valid     = ~tbox_min_south_ff_available & tbox_min_south_dest_is_north;

assign tbox_min_rsp_ready = tbox_min_south_ff_available;

always_comb
begin
    // North FF
    // --------
    // If FF still available
    if (tbox_min_north_ff_available)
        // Not available anymore when input is valid
        tbox_min_north_ff_available_next = ~tbox_min_south2north_valid;
    // Not available, wait for output to be accepted
    else
        // Grant received, FF available again
        tbox_min_north_ff_available_next = tbox_min_north_ff_out_accepted;

    // South FF
    // --------
    // If FF still available
    if (tbox_min_south_ff_available)
        // Not available anymore when input is valid
        tbox_min_south_ff_available_next = ~tbox_min_rsp_valid;
    // Not available, wait for output to be accepted
    else
        // Grant received, FF available again
        tbox_min_south_ff_available_next = tbox_min_south_ff_out_accepted;
end

assign tbox_min_north_ff_out_accepted = tbox_min_rsp_ready_in[tbox_min_north_ff_rsp_msg_dest.min_id];
assign tbox_min_south_ff_out_accepted = tbox_min_south_dest_is_north ? tbox_min_north_ff_available
                                                                     : tbox_min_rsp_ready_in[tbox_min_south_ff_rsp_msg_dest.min_id];

// Connect to minion inputs
always_comb
begin
    // Nort minions
    for (integer i = 0; i < MinPerN/2; i++)
    begin
        tbox_min_rsp_info_in[i]  = tbox_min_north_ff_rsp;
        tbox_min_rsp_valid_in[i] = ~tbox_min_north_ff_available & (tbox_min_north_ff_rsp_msg_dest.min_id == i[MinPerNW-1:0]);
    end

    // South minions
    for (integer i = MinPerN/2; i < MinPerN; i++)
    begin
        tbox_min_rsp_info_in[i]  = tbox_min_south_ff_rsp;
        tbox_min_rsp_valid_in[i] = ~tbox_min_south_ff_available & (tbox_min_south_ff_rsp_msg_dest.min_id == i[MinPerNW-1:0]);
    end
end

////////////////////////////////////////////////////////////////////////////
// Fast Local Barrier unit
////////////////////////////////////////////////////////////////////////////

neigh_fl_barrier
fl_barrier
(
    // System signals
    .clk_i ( clock_lv ),
    .rst_ni ( ~(reset_w_neigh_ff) ),
    // Fast Local Barrier request interface between cores and neigh
    .flb_core_neigh_req_valid_i ( flb_core_neigh_req_valid ),
    .flb_core_neigh_req_data_i ( flb_core_neigh_req_data ),
    // Fast Local Barrier response interface between cores and neigh
    .flb_neigh_core_resp_valid_o ( flb_neigh_core_resp_valid ),
    .flb_neigh_core_resp_data_o ( flb_neigh_core_resp_data ),
    // Fast Local Barrier request interface between neigh and L2
    .flb_neigh_l2_req_valid_o ( flb_neigh_l2_req_valid_lo ),
    .flb_neigh_l2_req_data_o ( flb_neigh_l2_req_data_lo ),
    // Fast Local Barrier response interface between neigh and L2
    .flb_l2_neigh_resp_valid_i ( flb_l2_neigh_resp_valid_lo ),
    .flb_l2_neigh_resp_data_i ( flb_l2_neigh_resp_data_lo )
);

////////////////////////////////////////////////////////////////////////////
// Cooperative Tensor Load
////////////////////////////////////////////////////////////////////////////

neigh_coop_tload
coop_tload
(
    // System signals
    .clk_i ( clock_lv ),
    .rst_ni ( ~(reset_w_neigh_ff) ),
    .neigh_id_i ( neigh_id_lo ),
    // Cooperative requests from minions
    .min_coop_miss_req_i ( min_coop_miss_req ),
    .min_coop_miss_req_valid_i ( min_coop_miss_req_valid ),
    .min_coop_miss_req_ready_o ( min_coop_miss_req_ready ),
    // Combined cooperative request to L2
    .coop_tload_disable_next_i ( evict_second_access_pending ),
    .coop_tload_req_o ( coop_tload_miss_req_info ),
    .coop_tload_req_bid_o ( coop_tload_miss_req_valid ),
    .coop_tload_req_ready_i ( coop_tload_miss_req_ready ),
    // Cooperative TLoad slave ready output bus
    .coop_tload_slv_rdy_out_data_o ( coop_tload_slv_rdy_out_data_lv ),
    .coop_tload_slv_rdy_out_valid_o ( coop_tload_slv_rdy_out_valid_lv ),
    // Cooperative TLoad slave ready input buses
    .coop_tload_slv_rdy_in_data_i ( coop_tload_slv_rdy_in_data_lv ),
    .coop_tload_slv_rdy_in_valid_i ( coop_tload_slv_rdy_in_valid_lv ),
    // Cooperative TLoad master done output bus
    .coop_tload_mst_done_out_coop_id_o ( coop_tload_mst_done_out_coop_id_lv ),
    .coop_tload_mst_done_out_valid_o ( coop_tload_mst_done_out_valid_lv ),
    // Cooperative TLoad master done input buses
    .coop_tload_mst_done_in_coop_id_i ( coop_tload_mst_done_in_coop_id_lv ),
    .coop_tload_mst_done_in_valid_i ( coop_tload_mst_done_in_valid_lv ),
    // Cooperative TLoad tag table CAM port
    .tag_table_cam_pop_i ( coop_tload_tag_table_cam_pop ),
    .tag_table_cam_tag_i ( coop_tload_tag_table_cam_tag ),
    .tag_table_cam_data_o ( coop_tload_tag_table_cam_data ),
    // Output debug signals going to Status Monitor
    .dbg_sm_signals_o ( coop_tload_dbg_sm_signals )
);

// HV version of neigh reset_w
logic reset_w_neigh_ff_hv;

vclevel_shft_l2h #(.WIDTH(1)) ls_l2h_reset_w_neigh (.inp_lv(reset_w_neigh_ff), .out_hv(reset_w_neigh_ff_hv));

neigh_coop_tload_ports
coop_tload_ports_pd_hv
(
  // System signals
  .clk_i ( dll_feedback_neigh ),
  .rst_ni ( ~(reset_w_neigh_ff_hv) ),

  // Cooperative TLoad ports on HV part
  .coop_tload_slv_rdy_out_data_o ( coop_tload_slv_rdy_out_data ),
  .coop_tload_slv_rdy_out_valid_o ( coop_tload_slv_rdy_out_valid ),

  .coop_tload_slv_rdy_in_data_i ( coop_tload_slv_rdy_in_data ),
  .coop_tload_slv_rdy_in_valid_i ( coop_tload_slv_rdy_in_valid ),

  .coop_tload_mst_done_out_coop_id_o ( coop_tload_mst_done_out_coop_id ),
  .coop_tload_mst_done_out_valid_o ( coop_tload_mst_done_out_valid ),

  .coop_tload_mst_done_in_coop_id_i ( coop_tload_mst_done_in_coop_id ),
  .coop_tload_mst_done_in_valid_i ( coop_tload_mst_done_in_valid ),

  // Cooperative TLoad ports on LV part
  .coop_tload_slv_rdy_out_data_lv_i ( coop_tload_slv_rdy_out_data_lv ),
  .coop_tload_slv_rdy_out_valid_lv_i ( coop_tload_slv_rdy_out_valid_lv ),

  .coop_tload_slv_rdy_in_data_lv_o ( coop_tload_slv_rdy_in_data_lv ),
  .coop_tload_slv_rdy_in_valid_lv_o ( coop_tload_slv_rdy_in_valid_lv ),

  .coop_tload_mst_done_out_coop_id_lv_i ( coop_tload_mst_done_out_coop_id_lv ),
  .coop_tload_mst_done_out_valid_lv_i ( coop_tload_mst_done_out_valid_lv ),

  .coop_tload_mst_done_in_coop_id_lv_o ( coop_tload_mst_done_in_coop_id_lv ),
  .coop_tload_mst_done_in_valid_lv_o ( coop_tload_mst_done_in_valid_lv )
);

////////////////////////////////////////////////////////////////////////////
// Tensor Store cooperative
////////////////////////////////////////////////////////////////////////////

neigh_tensor_store_buffer
tensor_store_buffer
(
    // System signals
    .clk_i ( clock_lv ),
    .rst_ni ( ~(reset_w_neigh_ff) ),
    // Evict request from minion
    .evict_req_i ( coop_evict_req ),
    .evict_valid_i ( coop_evict_valid ),
    .evict_ready_o ( coop_evict_ready ),
    // Request going out
    .evict_out_req_o ( tstore_evict_req ),
    .evict_out_valid_o ( tstore_evict_valid ),
    .evict_out_ready_i ( tstore_miss_req_ready ),
    .evict_out_disable_next_i ( min_evict_second_access_pending ),
    // Gets Acks going back to minions
    .tstore_min_ack_i ( l2_dcache_resp_tstore_ack ),
    // Sends Acks to slave minions
    .tstore_min_resp_ready_i ( tstore_min_resp_ready ),
    .tstore_min_resp_valid_o ( tstore_min_resp_valid ),
    .tstore_min_resp_o ( tstore_min_resp ),
    // Output debug signals going to Status Monitor
    .dbg_sm_signals_o ( tstore_dbg_sm_signals )
);

////////////////////////////////////////////////////////////////////////////////
// PMU Unit
////////////////////////////////////////////////////////////////////////////////

//          CLK       RST               EN                           DOUT              DIN                    DEF
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    pmu_neigh_events <= '0;
  end else if (~esr_pmu_ctrl.disable_clock) begin
    pmu_neigh_events <= pmu_neigh_events_next;
  end
end

assign pmu_neigh_events_next = {tbox_min_rsp_valid & tbox_min_rsp_ready,                     // The TBOX sends a message to any minion
                                min_tbox_req_valid & min_tbox_req_ready,                     // Any minion sends a message to the TBOX
                                fill_fifo_push,                                              // An ET Link response is received from the SC/UC input
                                |out_req_push,                                               // An ET Link request is pushed into any of the BANK/UC FIFOs
                                int_req_push,                                                // An ET Link request is pushed into the intermediate FIFO
                                sc_tbox_rsp_valid & sc_tbox_rsp_ready,                       // The TBOX receives an ET Link response from SC/UC
                                tbox_miss_req_valid & tbox_miss_req_ready,                   // The TBOX sends an ET Link request to SC/UC
                                |(ptw_l2_miss_resp_valid & ptw_l2_miss_resp_ready),          // Any PTW receives an ET Link response
                                |(ptw_l2_miss_req_valid & ptw_l2_miss_req_ready),            // Any PTW sends an ET Link request
                                icache_f0_sram_resp_valid_lo & icache_f0_sram_resp_ready_lo, // The Icache receives a response from the L1 data SRAM
                                icache_f2_sram_req_valid_lo & icache_f2_sram_req_ready_lo,   // The Icache sends a request to the L1 data SRAM
                                icache_l2_miss_resp_valid & icache_l2_miss_resp_ready,       // The Icache receives an ET Link response
                                icache_l2_miss_req_valid & icache_l2_miss_req_ready,         // The Icache sends an ET Link request
                                |(local_message_valid_in & local_message_ready_in),          // A message is sent between minions through the FLN
                                |ptw_dc_resp_valid,                                          // Any minion receives a response from the PTW
                                |(dc_ptw_req_valid & dc_ptw_req_ready),                      // Any minion sends a request to the PTW
                                |icache_resp_valid,                                          // Any minion receives a response from the Icache
                                |(icache_req_valid & icache_req_ready),                      // Any minion sends a request to the Icache
                                fill_fifo_push &                                             // A cooperative store response is received
                                    (fill_fifo_in_rsp_info.opcode == etlink_pkg::RspAck) &
                                    (fill_fifo_in_rsp_info.id[4:0] == EtLinkCoreMissIdTensorStoreCoop),
                                tstore_evict_valid & tstore_miss_req_ready,                  // A cooperative store request is sent
                                fill_fifo_push &                                             // A cooperative load response is received
                                    (fill_fifo_in_rsp_info.opcode == etlink_pkg::RspAckData) &
                                    (etlink_pkg::neigh_source_e'(fill_fifo_in_rsp_info.dest) == etlink_pkg::SrcCoop),
                                coop_tload_miss_req_valid & coop_tload_miss_req_ready &      // An inter-neighborhood cooperative load request is sent
                                    (coop_tload_miss_req_info.opcode == etlink_pkg::ReqReadCoop),
                                coop_tload_miss_req_valid & coop_tload_miss_req_ready,       // A cooperative load request is sent
                                |(fill_fifo_min_valid & fill_fifo_min_ready),                // Any minion receives an ET Link response
                                |(min_l2_req_bid & min_l2_req_ready)                         // Any minion sends an ET Link request
                               };

// PMU module
neigh_pmu
pmu
(
  // System signals
  .clk_i ( clock_lv ),
  .rst_ni ( ~(reset_w_neigh_pmu) ),
  // Minion interface
  .pmu_count_up_i ( pmu_count_up ),
  .pmu_read_data_o ( pmu_read_data ),
  .pmu_read_sel_i ( pmu_read_sel ),
  .pmu_write_en_i ( pmu_write_en ),
  .pmu_write_data_i ( pmu_write_data ),
  .pmu_neigh_event_sel_i ( pmu_neigh_event_sel ),
  // Neigh interface
  .pmu_neigh_events_i ( pmu_neigh_events ),
  // Module control
  .pmu_ctrl_i ( esr_pmu_ctrl )
);

////////////////////////////////////////////////////////////////////////////
// ESR file
////////////////////////////////////////////////////////////////////////////
esr_and_or_tree_L0_t esr_and_or_tree_L0_2_esr;
esr_and_or_tree_L0_t esr_and_or_tree_L0;
esr_and_or_tree_L0_t esr_and_or_tree_L0_lo;

always_comb begin
  esr_and_or_tree_L0_2_esr = 'h0;
  esr_and_or_tree_L0_2_esr.anyselected    = esr_and_or_tree_L0.anyselected   ;
  esr_and_or_tree_L0_2_esr.anyunavailable = esr_and_or_tree_L0.anyunavailable;
  esr_and_or_tree_L0_2_esr.allhavereset   = esr_and_or_tree_L0.allhavereset  ;
  esr_and_or_tree_L0_2_esr.anyhavereset   = esr_and_or_tree_L0.anyhavereset  ;
  esr_and_or_tree_L0_2_esr.allresumeack   = esr_and_or_tree_L0.allresumeack  ;
  esr_and_or_tree_L0_2_esr.anyresumeack   = esr_and_or_tree_L0.anyresumeack  ;
  esr_and_or_tree_L0_2_esr.allrunning     = esr_and_or_tree_L0.allrunning    ;
  esr_and_or_tree_L0_2_esr.anyrunning     = esr_and_or_tree_L0.anyrunning    ;
  esr_and_or_tree_L0_2_esr.allhalted      = esr_and_or_tree_L0.allhalted     ;
  esr_and_or_tree_L0_2_esr.anyhalted      = esr_and_or_tree_L0.anyhalted     ;
end

  //   CLK     DOUT                   DIN
always_ff @(posedge clock_lv) begin
  esr_and_or_tree_L0_lo <= esr_and_or_tree_L0;
end

esr_neigh
#(
    .AdWidth                   ( EsrNeighApbPpAddrWidth  )
)
esrs
(
    .clk_i ( clock_lv ),
    .rst_c_ni ( ~(reset_c_neigh_ff) ),
    .rst_w_ni ( ~(reset_w_neigh_ff) ),
    .rst_d_ni ( ~(reset_d_neigh_ff) ),
    .test_en_i ( 1'b0 ), // DFT_SCAN_INSERT
    .dft_i ( dft_lv_o ), // unused
    .esr_minion_boot_o ( minion_boot_addr ),
    .esr_mprot_o ( mprot ),
    .esr_vmspagesize_o ( vmspagesize ),
    .esr_ipi_redirect_pc_o ( esr_ipi_redirect_pc ),
    .esr_hactrl_o ( esr_hactrl ),
    .esr_hastatus0_i ( esr_hastatus0 ),
    .esr_hastatus1_min_i ( esr_hastatus1_min ),
    .esr_dmctrl_i ( dmctrl_lo ),
    .esr_icache_err_log_ctl_o ( esr_icache_err_log_ctl ),
    .esr_icache_err_log_sbe_i ( esr_icache_err_log_sbe ),
    .esr_icache_err_log_dbe_i ( esr_icache_err_log_dbe ),
    .esr_icache_err_log_info_i ( esr_icache_err_log_info ),
    .esr_icache_err_detected_o ( esr_icache_err_detected_lo ),
    .esr_icache_err_logged_o ( esr_icache_err_logged_lo ),
    .esr_icache_ecc_count_ov_o ( esr_icache_ecc_count_ov ),
    .esr_texture_control_o ( esr_texture_control_op ),
    .esr_texture_status_i ( esr_texture_status_ip ),
    .esr_texture_image_table_ptr_o ( esr_texture_image_table_ptr_op ),
    .apb_paddr_i ( APB_req_struct_ESR.apb_paddr ),
    .apb_pwrite_i ( APB_req_struct_ESR.apb_pwrite ),
    .apb_psel_i ( APB_req_struct_ESR.apb_psel ),
    .apb_penable_i ( APB_req_struct_ESR.apb_penable ),
    .apb_pwdata_i ( APB_req_struct_ESR.apb_pwdata ),
    .apb_pready_o ( APB_rsp_struct_ESR.apb_pready ),
    .apb_prdata_o ( APB_rsp_struct_ESR.apb_prdata ),
    .apb_pslverr_o ( APB_rsp_struct_ESR.apb_pslverr ),
    .esr_and_or_tree_l0_i ( esr_and_or_tree_L0_2_esr ),
    .esr_pmu_ctrl_o ( esr_pmu_ctrl ),
    .esr_neigh_chicken_o ( esr_neigh_chicken )
);

// Bypass Dcache bits
assign esr_bypass_dcache = esr_neigh_chicken.bypass_dcache;

// Flop ESRs from/to TBOX
//      CLK       RST               DOUT                               DIN                             DEF
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    esr_texture_control_op_ff <= '0;
  end else begin
    esr_texture_control_op_ff <= esr_texture_control_op;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    esr_texture_status_ip_ff <= '0;
  end else begin
    esr_texture_status_ip_ff <= tbox_status_esr;
  end
end
always_ff @(posedge clock_lv) begin
  if (reset_w_neigh_ff) begin
    esr_texture_image_table_ptr_op_ff <= '0;
  end else begin
    esr_texture_image_table_ptr_op_ff <= esr_texture_image_table_ptr_op;
  end
end

assign esr_texture_status_ip    = esr_texture_status_ip_ff;
assign tbox_control_esr         = esr_texture_control_op_ff;
assign tbox_image_table_ptr_esr = esr_texture_image_table_ptr_op_ff;

////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////
// Debug signals from/to Voltage crossings
logic [NeighSmGpioWidth-1:0]      neigh_sm_gpio_lo;
neigh_sm_dbg_monitor_t                neigh_sm_signals_lo;
// Debug signals from neigh_channel to Status Monitor 0
logic                                 neigh_ch_dbg_signals_en;
logic [NeighDebugSmMuxWidth-1:0] neigh_ch_dbg_signals_mux;
neigh_sm_dbg_monitor_t                neigh_ch_dbg_signals_next;
neigh_sm_dbg_monitor_t                neigh_ch_dbg_signals;
// Debug signals from/to minions
minion_debug_out_t [MinPerN-1:0]   debug_minion_out_ff;

//      CLK       RST               EN              DOUT                   DIN               DEF
always_ff @(posedge clock_lv) begin
  if (reset_d_neigh_ff) begin
    debug_minion_out_ff <= '0;
  end else begin
    debug_minion_out_ff <= debug_minion_out;
  end
end


logic  [MinPerN-1:0] reset_w_per_minion_pre;
genvar min_i;
generate for (min_i= 0; min_i<MinPerN; min_i++)
begin : SYNC_RESETS_PER_MINION
    rst_repeat rst_repeat_reset_w_per_minion(.dft__reset_byp(dft__reset_byp_lv) , .dft__reset(dft__reset_lv) , .clock(clock_lv) , .reset_in(reset_w_per_minion_pre[min_i]) , .reset_out(reset_w_per_minion[min_i]));
    rst_repeat rst_repeat_reset_c_per_minion(.dft__reset_byp(dft__reset_byp_lv) , .dft__reset(dft__reset_lv) , .clock(clock_lv) , .reset_in(reset_c_neigh)                 , .reset_out(reset_c_per_minion[min_i]));
    rst_repeat rst_repeat_reset_d_per_minion(.dft__reset_byp(dft__reset_byp_lv) , .dft__reset(dft__reset_lv) , .clock(clock_lv) , .reset_in(reset_d_neigh)                 , .reset_out(reset_d_per_minion[min_i]));
end
endgenerate

neigh_ch_dbg
neigh_ch_dbg
(
    .clk_i ( clock_lv ),
    // Input resets
    .rst_w_ni ( ~(reset_w_dbg) ),
    .rst_d_ni ( ~(reset_d_neigh_ff) ),
    // Output resets
    .reset_w_per_minion_o ( reset_w_per_minion_pre ),
    .reset_w_neigh_o ( reset_w_neigh ),

    // Run-control for minions and tbox
    .bpam_run_control_i ( bpam_run_control_lo ),
    .tbox_bpam_run_control_o ( tbox_bpam_run_control ),
    .tbox_bpam_run_control_ack_i ( tbox_bpam_run_control_ack ),
    .debug_minion_req_o ( debug_minion_in ),
    .debug_minion_status_i ( debug_minion_out_ff ),
    .dmctrl_i ( dmctrl_lo ),
    .esr_hastatus0_o ( esr_hastatus0 ),
    .esr_hastatus1_min_o ( esr_hastatus1_min ),
    .esr_hactrl_i ( esr_hactrl ),
    .esr_and_or_tree_l0_o ( esr_and_or_tree_L0 ),

    // Trace Encoder
    /*
    .te_enable_module           ( '0                           ),
    .te_enable                  (                              ),
    .te_minion_sel              ( '0                           ),
    .te_thread_sel              ( '0                           ),
    .traceEncoder_minions       (                              ),
    .traceEncoder_neigh         (                              ),
    */

    // Status Monitor
    .status_monitor_enable_i ( hw_dbg_sm_monitor_enabled_lo ),
    .neigh_sm_gpio_i ( neigh_sm_gpio_lo ),
    .minion_dbg_signals_en_o ( minion_dbg_signals_en ),
    .minion_dbg_signals_mux_o ( minion_dbg_signals_mux ),
    .minion_dbg_signals_i ( minion_dbg_signals ),
    .tbox_dbg_signals_en_o ( tbox_dbg_signals_en ),
    .tbox_dbg_signals_mux_o ( tbox_dbg_signals_mux ),
    .tbox_dbg_signals_i ( tbox_dbg_signals ),
    .neigh_ch_dbg_signals_en_o ( neigh_ch_dbg_signals_en ),
    .neigh_ch_dbg_signals_mux_o ( neigh_ch_dbg_signals_mux ),
    .neigh_ch_dbg_signals_i ( neigh_ch_dbg_signals ),
    .neigh_sm_signals_o ( neigh_sm_signals_lo )
);

// Connect debug signals from neigh_channel
logic [NeighDebugSmMuxModWidth-1:0]    neigh_ch_dbg_mod_idx;
logic [NeighDebugSmMuxDataWidth-1:0]   neigh_ch_dbg_data_idx;
logic [(MinPerNW-1)-1:0]                 neigh_ch_dbg_min_idx;
logic [MinPerNW-1:0]                         neigh_ch_dbg_north_min_idx;
logic [MinPerNW-1:0]                         neigh_ch_dbg_south_min_idx;
logic [IcacheNrReqsW-1:0]                neigh_ch_dbg_icache_req_idx;
logic [PtwPerNW-1:0]                         neigh_ch_dbg_ptw_idx;
logic                                        neigh_ch_dbg_tload_data_idx;
logic [MinPerN-1:0][1:0]                 tstore_min_resp_opcode;
logic [MinPerN-1:0][IdSize-1:0] local_message_info_out_id;
logic [MinPerN-1:0][8-1:0]                local_message_info_out_data;
logic [NeighDebugMatchWidth-1:0]         min_north_dbg_match_signals_next;
logic [NeighDebugFilterWidth-1:0]        min_north_dbg_filter_signals_next;
logic [NeighDebugDataWidth-1:0]          min_north_dbg_data_signals_next;
logic [NeighDebugMatchWidth-1:0]         min_north_dbg_match_signals;
logic [NeighDebugFilterWidth-1:0]        min_north_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          min_north_dbg_data_signals;
logic [NeighDebugMatchWidth-1:0]         min_south_dbg_match_signals_next;
logic [NeighDebugFilterWidth-1:0]        min_south_dbg_filter_signals_next;
logic [NeighDebugDataWidth-1:0]          min_south_dbg_data_signals_next;
logic [NeighDebugMatchWidth-1:0]         min_south_dbg_match_signals;
logic [NeighDebugFilterWidth-1:0]        min_south_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          min_south_dbg_data_signals;
logic [NeighDebugMatchWidth-1:0]         icache_ptw_dbg_match_signals;
logic [NeighDebugFilterWidth-1:0]        icache_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          icache_dbg_data_signals;
logic [NeighDebugFilterWidth-1:0]        ptw_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          ptw_dbg_data_signals;
logic [NeighDebugMatchWidth-1:0]         tload_tstore_dbg_match_signals;
logic [NeighDebugFilterWidth-1:0]        tload_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          tload_dbg_data_signals;
logic [NeighDebugFilterWidth-1:0]        tstore_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          tstore_dbg_data_signals;
logic [NeighDebugMatchWidth-1:0]         fln_tbox_dpath_flb_dbg_match_signals;
logic [NeighDebugFilterWidth-1:0]        fln_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          fln_dbg_data_signals;
logic [NeighDebugFilterWidth-1:0]        tbox_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          tbox_dbg_data_signals;
logic [NeighDebugFilterWidth-1:0]        dpath_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          dpath_dbg_data_signals;
logic [NeighDebugFilterWidth-1:0]        flb_dbg_filter_signals;
logic [NeighDebugDataWidth-1:0]          flb_dbg_data_signals;

//          CLK       RST               EN                       DOUT                  DIN                        DEF
always_ff @(posedge clock_lv) begin
  if (reset_d_neigh_ff) begin
    neigh_ch_dbg_signals <= '0;
  end else if (neigh_ch_dbg_signals_en) begin
    neigh_ch_dbg_signals <= neigh_ch_dbg_signals_next;
  end
end

// Extract debug multiplexing indexes
assign neigh_ch_dbg_mod_idx  = neigh_ch_dbg_signals_mux[NeighDebugSmMuxDataWidth+:NeighDebugSmMuxModWidth];
assign neigh_ch_dbg_data_idx = neigh_ch_dbg_signals_mux[0+:NeighDebugSmMuxDataWidth];

assign neigh_ch_dbg_min_idx         = neigh_ch_dbg_mod_idx[0+:MinPerNW-1];
assign neigh_ch_dbg_north_min_idx   = {1'b0,neigh_ch_dbg_min_idx};
assign neigh_ch_dbg_south_min_idx   = {1'b1,neigh_ch_dbg_min_idx};
assign neigh_ch_dbg_icache_req_idx  = neigh_ch_dbg_data_idx[0+:IcacheNrReqsW];
assign neigh_ch_dbg_ptw_idx         = neigh_ch_dbg_data_idx[0+:PtwPerNW];
assign neigh_ch_dbg_tload_data_idx  = neigh_ch_dbg_data_idx[0];

// Extract opcodes from responses from TStore to minions
always_comb
begin
    for (integer unsigned i = 0; i < MinPerN; i++)
        tstore_min_resp_opcode[i] = tstore_min_resp[i].opcode;
end

// Extract ids data from requests from evict FFs to FLN
always_comb
begin
    for (integer unsigned i = 0; i < MinPerN; i++)
    begin
        local_message_info_out_id[i]   = local_message_info_out[i].id;
        local_message_info_out_data[i] = local_message_info_out[i].data[7:0];
    end
end

// Connect debug signals from the different neighborhood channel modules before being multiplexed

// Minion debug signals (flopped)
// --------------------
logic [MinPerN-1:0][DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_ready_dbg_reg;
logic [MinPerN-1:0][DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_valid_dbg_reg;
et_link_minion_evict_req_info_t [MinPerN-1:0]       l2_dcache_evict_req_dbg_reg;
logic [MinPerN-1:0][DcacheL2MissReqPorts-1:0]  l2_dcache_miss_req_ready_dbg_reg;
logic [MinPerN-1:0][DcacheL2MissReqPorts-1:0]  l2_dcache_miss_req_valid_dbg_reg;
et_link_minion_miss_req_info_t [MinPerN-1:0]        l2_dcache_miss_req_dbg_reg;
logic [MinPerN-1:0]                                 l2_dcache_resp_ready_dbg_reg;
logic [MinPerN-1:0]                                 l2_dcache_resp_valid_dbg_reg;
et_link_minion_rsp_info_t [MinPerN-1:0]             l2_dcache_resp_dbg_reg;
logic [MinPerN-1:0][DcacheL2MissReqPorts-1:0]  miss_ff_dbg_sm_signals_reg;
logic [MinPerN-1:0][DcacheL2EvictReqPorts-1:0] evict_ff_dbg_sm_signals_reg;
fill_ff_dbg_sm_t [MinPerN-1:0]                      fill_ff_dbg_sm_signals_reg;

//      CLK       EN                       DOUT                               DIN
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_evict_req_ready_dbg_reg <= l2_dcache_evict_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_evict_req_valid_dbg_reg <= l2_dcache_evict_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_evict_req_dbg_reg <= l2_dcache_evict_req;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_miss_req_ready_dbg_reg <= l2_dcache_miss_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_miss_req_valid_dbg_reg <= l2_dcache_miss_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_miss_req_dbg_reg <= l2_dcache_miss_req;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_resp_ready_dbg_reg <= l2_dcache_resp_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_resp_valid_dbg_reg <= l2_dcache_resp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    l2_dcache_resp_dbg_reg <= l2_dcache_resp;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    miss_ff_dbg_sm_signals_reg <= miss_ff_dbg_sm_signals;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    evict_ff_dbg_sm_signals_reg <= evict_ff_dbg_sm_signals;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    fill_ff_dbg_sm_signals_reg <= fill_ff_dbg_sm_signals;
  end
end

// Combine match signals from all minions
// (first, north and south minions are multiplexed independently and flopped)
assign min_north_dbg_match_signals_next = fit_dbg_match({
                                          l2_dcache_resp_valid_dbg_reg[0+:MinPerN/2] & l2_dcache_resp_ready_dbg_reg[0+:MinPerN/2],
                                          l2_dcache_evict_req_valid_dbg_reg[0+:MinPerN/2] & l2_dcache_evict_req_ready_dbg_reg[0+:MinPerN/2],
                                          l2_dcache_miss_req_valid_dbg_reg[0+:MinPerN/2] & l2_dcache_miss_req_ready_dbg_reg[0+:MinPerN/2]});
assign min_south_dbg_match_signals_next = fit_dbg_match({
                                          l2_dcache_resp_valid_dbg_reg[MinPerN/2+:MinPerN/2] & l2_dcache_resp_ready_dbg_reg[MinPerN/2+:MinPerN/2],
                                          l2_dcache_evict_req_valid_dbg_reg[MinPerN/2+:MinPerN/2] & l2_dcache_evict_req_ready_dbg_reg[MinPerN/2+:MinPerN/2],
                                          l2_dcache_miss_req_valid_dbg_reg[MinPerN/2+:MinPerN/2] & l2_dcache_miss_req_ready_dbg_reg[MinPerN/2+:MinPerN/2]});

assign min_north_dbg_filter_signals_next = fit_dbg_filter({
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                           l2_dcache_resp_valid_dbg_reg[neigh_ch_dbg_north_min_idx] & l2_dcache_resp_ready_dbg_reg[neigh_ch_dbg_north_min_idx],
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].address,
                                           l2_dcache_evict_req_valid_dbg_reg[neigh_ch_dbg_north_min_idx] & l2_dcache_evict_req_ready_dbg_reg[neigh_ch_dbg_north_min_idx],
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].address,
                                           l2_dcache_miss_req_valid_dbg_reg[neigh_ch_dbg_north_min_idx] & l2_dcache_miss_req_ready_dbg_reg[neigh_ch_dbg_north_min_idx]});
assign min_south_dbg_filter_signals_next = fit_dbg_filter({
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                           l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                           l2_dcache_resp_valid_dbg_reg[neigh_ch_dbg_south_min_idx] & l2_dcache_resp_ready_dbg_reg[neigh_ch_dbg_south_min_idx],
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                           l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].address,
                                           l2_dcache_evict_req_valid_dbg_reg[neigh_ch_dbg_south_min_idx] & l2_dcache_evict_req_ready_dbg_reg[neigh_ch_dbg_south_min_idx],
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                           l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].address,
                                           l2_dcache_miss_req_valid_dbg_reg[neigh_ch_dbg_south_min_idx] & l2_dcache_miss_req_ready_dbg_reg[neigh_ch_dbg_south_min_idx]});

assign min_north_dbg_data_signals_next = fit_dbg_data({
                                         fill_ff_dbg_sm_signals_reg[neigh_ch_dbg_north_min_idx],
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                         evict_ff_dbg_sm_signals_reg[neigh_ch_dbg_north_min_idx],
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_north_min_idx].address,
                                         miss_ff_dbg_sm_signals_reg[neigh_ch_dbg_north_min_idx],
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].id,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].opcode,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_north_min_idx].address});
assign min_south_dbg_data_signals_next = fit_dbg_data({
                                         fill_ff_dbg_sm_signals_reg[neigh_ch_dbg_south_min_idx],
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                         l2_dcache_resp_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                         evict_ff_dbg_sm_signals_reg[neigh_ch_dbg_south_min_idx],
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                         l2_dcache_evict_req_dbg_reg[neigh_ch_dbg_south_min_idx].address,
                                         miss_ff_dbg_sm_signals_reg[neigh_ch_dbg_south_min_idx],
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].id,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].opcode,
                                         l2_dcache_miss_req_dbg_reg[neigh_ch_dbg_south_min_idx].address});

//      CLK       EN                       DOUT                          DIN
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_north_dbg_match_signals <= min_north_dbg_match_signals_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_north_dbg_filter_signals <= min_north_dbg_filter_signals_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_north_dbg_data_signals <= min_north_dbg_data_signals_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_south_dbg_match_signals <= min_south_dbg_match_signals_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_south_dbg_filter_signals <= min_south_dbg_filter_signals_next;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_south_dbg_data_signals <= min_south_dbg_data_signals_next;
  end
end

// Icache and PTW debug signals (flopped)
// ----------------------------
logic [MinPerN-1:0]                     icache_req_valid_dbg_reg;
logic [MinPerN-1:0]                     icache_req_ready_dbg_reg;
icache_frontend_resp [IcacheNrReqs-1:0] icache_resp_dbg_reg;
logic [IcacheNrReqs-1:0]                icache_resp_miss_dbg_reg;
logic [IcacheNrReqs-1:0]                icache_resp_valid_dbg_reg;
logic [IcacheNrReqs-1:0]                icache_fill_done_dbg_reg;
logic                                      icache_f2_sram_req_write_dbg_reg;
logic [IcacheSramAddrWidth-1:0]         icache_f2_sram_req_addr_dbg_reg;
logic                                      icache_f2_sram_req_valid_dbg_reg;
logic                                      icache_f2_sram_req_ready_dbg_reg;
logic [IcacheSramDataWidth-1:0]        icache_f0_sram_resp_dout_dbg_reg;
logic                                      icache_f0_sram_resp_valid_dbg_reg;
logic                                      icache_f0_sram_resp_ready_dbg_reg;
et_link_nodata_req_info_t                  icache_l2_miss_req_info_dbg_reg;
logic                                      icache_l2_miss_req_valid_dbg_reg;
logic                                      icache_l2_miss_req_ready_dbg_reg;
et_link_rsp_info_t                         icache_l2_miss_resp_info_dbg_reg;
logic                                      icache_l2_miss_resp_valid_dbg_reg;
logic                                      icache_l2_miss_resp_ready_dbg_reg;
icache_dbg_sm_t                            icache_dbg_sm_signals_reg;
logic [PtwPerN-1:0][MinPerPtw-1:0]   dc_ptw_req_valid_dbg_reg;
logic [PtwPerN-1:0][MinPerPtw-1:0]   dc_ptw_req_ready_dbg_reg;
logic [PtwPerN-1:0]                     ic_ptw_req_valid_dbg_reg;
logic [PtwPerN-1:0]                     ic_ptw_req_ready_dbg_reg;
minion_ptw_pte [PtwPerN-1:0]            ptw_resp_data_dbg_reg;
logic [PtwPerN-1:0][MinPerPtw-1:0]   ptw_dc_resp_valid_dbg_reg;
logic [PtwPerN-1:0]                     ptw_ic_resp_valid_dbg_reg;
et_link_neigh_req_info_t [PtwPerN-1:0]  ptw_l2_miss_req_info_dbg_reg;
logic [PtwPerN-1:0]                     ptw_l2_miss_req_valid_dbg_reg;
logic [PtwPerN-1:0]                     ptw_l2_miss_req_ready_dbg_reg;
et_link_rsp_info_t [PtwPerN-1:0]        ptw_l2_miss_resp_info_dbg_reg;
logic [PtwPerN-1:0]                     ptw_l2_miss_resp_valid_dbg_reg;
logic [PtwPerN-1:0]                     ptw_l2_miss_resp_ready_dbg_reg;
ptw_dbg_sm_t [PtwPerN-1:0]              ptw_dbg_sm_signals_reg;

//      CLK       EN                       DOUT                               DIN
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_req_valid_dbg_reg <= icache_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_req_ready_dbg_reg <= icache_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_resp_dbg_reg <= icache_resp;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_resp_miss_dbg_reg <= icache_resp_miss;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_resp_valid_dbg_reg <= icache_resp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_fill_done_dbg_reg <= icache_fill_done;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_f2_sram_req_write_dbg_reg <= icache_f2_sram_req_write_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_f2_sram_req_addr_dbg_reg <= icache_f2_sram_req_addr_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_f2_sram_req_valid_dbg_reg <= icache_f2_sram_req_valid_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_f2_sram_req_ready_dbg_reg <= icache_f2_sram_req_ready_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_f0_sram_resp_dout_dbg_reg <= icache_f0_sram_resp_dout_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_f0_sram_resp_valid_dbg_reg <= icache_f0_sram_resp_valid_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_f0_sram_resp_ready_dbg_reg <= icache_f0_sram_resp_ready_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_l2_miss_req_info_dbg_reg <= icache_l2_miss_req_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_l2_miss_req_valid_dbg_reg <= icache_l2_miss_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_l2_miss_req_ready_dbg_reg <= icache_l2_miss_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_l2_miss_resp_info_dbg_reg <= icache_l2_miss_resp_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_l2_miss_resp_valid_dbg_reg <= icache_l2_miss_resp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_l2_miss_resp_ready_dbg_reg <= icache_l2_miss_resp_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    icache_dbg_sm_signals_reg <= icache_dbg_sm_signals;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    dc_ptw_req_valid_dbg_reg <= dc_ptw_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    dc_ptw_req_ready_dbg_reg <= dc_ptw_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ic_ptw_req_valid_dbg_reg <= ic_ptw_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ic_ptw_req_ready_dbg_reg <= ic_ptw_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_resp_data_dbg_reg <= ptw_resp_data;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_dc_resp_valid_dbg_reg <= ptw_dc_resp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_ic_resp_valid_dbg_reg <= ptw_ic_resp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_l2_miss_req_info_dbg_reg <= ptw_l2_miss_req_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_l2_miss_req_valid_dbg_reg <= ptw_l2_miss_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_l2_miss_req_ready_dbg_reg <= ptw_l2_miss_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_l2_miss_resp_info_dbg_reg <= ptw_l2_miss_resp_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_l2_miss_resp_valid_dbg_reg <= ptw_l2_miss_resp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_l2_miss_resp_ready_dbg_reg <= ptw_l2_miss_resp_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    ptw_dbg_sm_signals_reg <= ptw_dbg_sm_signals;
  end
end

// Combine match signals from Icache and PTW
assign icache_ptw_dbg_match_signals = fit_dbg_match({
                                         // PTW signals
                                         ptw_l2_miss_resp_valid_dbg_reg & ptw_l2_miss_resp_ready_dbg_reg,
                                         ptw_l2_miss_req_valid_dbg_reg & ptw_l2_miss_req_ready_dbg_reg,
                                         ptw_ic_resp_valid_dbg_reg,
                                         ic_ptw_req_valid_dbg_reg & ic_ptw_req_ready_dbg_reg,
                                         ptw_dc_resp_valid_dbg_reg,
                                         dc_ptw_req_valid_dbg_reg & dc_ptw_req_ready_dbg_reg,
                                         // Icache signals
                                         icache_dbg_sm_signals_reg.ucache_f2_valid,
                                         icache_l2_miss_resp_valid_dbg_reg & icache_l2_miss_resp_ready_dbg_reg,
                                         icache_l2_miss_req_valid_dbg_reg & icache_l2_miss_req_ready_dbg_reg,
                                         icache_f0_sram_resp_valid_dbg_reg & icache_f0_sram_resp_ready_dbg_reg,
                                         icache_f2_sram_req_valid_dbg_reg & icache_f2_sram_req_ready_dbg_reg,
                                         icache_dbg_sm_signals_reg.f0_l1_miss_req_valid,
                                         icache_fill_done_dbg_reg,
                                         icache_resp_valid_dbg_reg,
                                         icache_req_valid_dbg_reg & icache_req_ready_dbg_reg});

// Icache debug signals
assign icache_dbg_filter_signals = fit_dbg_filter({
                                     // Signals always connected
                                     icache_l2_miss_resp_valid_dbg_reg & icache_l2_miss_resp_ready_dbg_reg,
                                     icache_l2_miss_resp_info_dbg_reg.opcode,
                                     icache_l2_miss_req_valid_dbg_reg & icache_l2_miss_req_ready_dbg_reg,
                                     icache_l2_miss_req_info_dbg_reg.address[PaSize-1:6],
                                     icache_f0_sram_resp_valid_dbg_reg & icache_f0_sram_resp_ready_dbg_reg,
                                     icache_f2_sram_req_valid_dbg_reg & icache_f2_sram_req_ready_dbg_reg,
                                     icache_f2_sram_req_addr_dbg_reg,
                                     icache_f2_sram_req_write_dbg_reg,
                                     icache_dbg_sm_signals_reg.f0_l1_miss_req_valid,
                                     icache_dbg_sm_signals_reg.f0_l1_miss_req_addr,
                                     // Signals selected per ucache
                                     icache_fill_done_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                     icache_resp_valid_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                     icache_resp_miss_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].page_fault,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].access_fault,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].cacheable,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].bus_err,
                                     icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].ecc_err,
                                     icache_dbg_sm_signals_reg.ucache_f2_valid[neigh_ch_dbg_icache_req_idx],
                                     icache_dbg_sm_signals_reg.ucache_f2_paddr[neigh_ch_dbg_icache_req_idx],
                                     icache_dbg_sm_signals_reg.f1_req_valid[neigh_ch_dbg_icache_req_idx],
                                     icache_dbg_sm_signals_reg.f1_req_addr[neigh_ch_dbg_icache_req_idx]});

assign icache_dbg_data_signals = fit_dbg_data({
                                   // Signals always connected
                                   icache_l2_miss_resp_info_dbg_reg.opcode,
                                   icache_l2_miss_resp_info_dbg_reg.data[7:0],
                                   icache_f0_sram_resp_dout_dbg_reg[7:0],
                                   icache_f2_sram_req_write_dbg_reg,
                                   icache_dbg_sm_signals_reg.f1_tag_hit,
                                   icache_dbg_sm_signals_reg.f0_miss_state,
                                   icache_dbg_sm_signals_reg.ucache_f0_prefetch_state,
                                   // Signals selected per ucache
                                   icache_dbg_sm_signals_reg.ucache_f0_miss_state[neigh_ch_dbg_icache_req_idx],
                                   icache_resp_miss_dbg_reg[neigh_ch_dbg_icache_req_idx],
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].page_fault,
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].access_fault,
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].cacheable,
                                   icache_resp_dbg_reg[neigh_ch_dbg_icache_req_idx].data[7:0],
                                   icache_dbg_sm_signals_reg.ucache_f2_hit_array[neigh_ch_dbg_icache_req_idx],
                                   icache_dbg_sm_signals_reg.ucache_f2_paddr[neigh_ch_dbg_icache_req_idx],
                                   icache_dbg_sm_signals_reg.f1_req_addr[neigh_ch_dbg_icache_req_idx][42:6]});

// PTW debug signals
assign ptw_dbg_filter_signals = fit_dbg_filter({
                                  // Signals selected per PTW
                                  ptw_l2_miss_resp_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & ptw_l2_miss_resp_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_l2_miss_resp_info_dbg_reg[neigh_ch_dbg_ptw_idx].opcode,
                                  ptw_l2_miss_req_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & ptw_l2_miss_req_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_l2_miss_req_info_dbg_reg[neigh_ch_dbg_ptw_idx].address[PaSize-1:6],
                                  ptw_ic_resp_valid_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_dc_resp_valid_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].canceled_req,
                                  ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].access_fault,
                                  ic_ptw_req_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & ic_ptw_req_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  dc_ptw_req_valid_dbg_reg[neigh_ch_dbg_ptw_idx] & dc_ptw_req_ready_dbg_reg[neigh_ch_dbg_ptw_idx],
                                  ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_satp_mode,
                                  ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_prv,
                                  ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_addr});

assign ptw_dbg_data_signals = fit_dbg_data({
                                // Signals selected per PTW
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].state,
                                ptw_l2_miss_resp_info_dbg_reg[neigh_ch_dbg_ptw_idx].opcode,
                                ptw_l2_miss_resp_info_dbg_reg[neigh_ch_dbg_ptw_idx].data[7:0],
                                ptw_l2_miss_req_info_dbg_reg[neigh_ch_dbg_ptw_idx].address[PaSize-1:6],
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].canceled_req,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].access_fault,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].d,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].a,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].g,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].u,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].x,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].w,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].r,
                                ptw_resp_data_dbg_reg[neigh_ch_dbg_ptw_idx].v,
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_satp_mode,
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_prv,
                                ptw_dbg_sm_signals_reg[neigh_ch_dbg_ptw_idx].req_addr});

// Cooperative TLoad and TStore debug signals (not flopped)
// ------------------------------------------
// Combine match signals from Cooperative TLoad and TStore
assign tload_tstore_dbg_match_signals = fit_dbg_match({
                                          // TStore signals
                                          tstore_min_resp_valid & tstore_min_resp_ready,
                                          tstore_evict_valid & tstore_miss_req_ready,
                                          coop_evict_valid & coop_evict_ready,
                                          // Cooperative TLoad signals
                                          coop_tload_tag_table_cam_pop,
                                          coop_tload_mst_done_in_valid_lv,
                                          coop_tload_mst_done_out_valid_lv,
                                          coop_tload_slv_rdy_in_valid_lv,
                                          coop_tload_slv_rdy_out_valid_lv,
                                          coop_tload_miss_req_valid & coop_tload_miss_req_ready,
                                          min_coop_miss_req_valid & min_coop_miss_req_ready});

// Cooperative TLoad debug signals
assign tload_dbg_filter_signals = fit_dbg_filter({
                                    coop_tload_tag_table_cam_pop,
                                    coop_tload_tag_table_cam_tag,
                                    coop_tload_mst_done_in_valid_lv,
                                    coop_tload_mst_done_in_coop_id_lv,
                                    coop_tload_mst_done_out_valid_lv,
                                    coop_tload_mst_done_out_coop_id_lv,
                                    coop_tload_slv_rdy_in_valid_lv,
                                    coop_tload_slv_rdy_in_data_lv,
                                    coop_tload_slv_rdy_out_valid_lv,
                                    coop_tload_slv_rdy_out_data_lv,
                                    coop_tload_miss_req_valid & coop_tload_miss_req_ready,
                                    coop_tload_miss_req_info.id,
                                    coop_tload_miss_req_info.address,
                                    coop_tload_miss_req_info.data[19:0]});

assign tload_dbg_data_signals = neigh_ch_dbg_tload_data_idx == NeighChDebugSmTloadDataExt ?
                                // Cooperative TLoad ports
                                fit_dbg_data({
                                 coop_tload_tag_table_cam_tag,
                                 coop_tload_mst_done_in_coop_id_lv,
                                 coop_tload_mst_done_out_coop_id_lv,
                                 coop_tload_slv_rdy_in_data_lv,
                                 coop_tload_slv_rdy_out_data_lv,
                                 coop_tload_miss_req_info.id,
                                 coop_tload_miss_req_info.address,
                                 coop_tload_miss_req_info.data[19:0]})
                                // Cooperative TLoad internal status
                              : fit_dbg_data(
                                 coop_tload_dbg_sm_signals);

// TStore debug signals
assign tstore_dbg_filter_signals = fit_dbg_filter({
                                     tstore_min_resp_valid & tstore_min_resp_ready,
                                     tstore_min_resp_opcode,
                                     tstore_evict_valid & tstore_miss_req_ready,
                                     tstore_evict_req.address});

assign tstore_dbg_data_signals = fit_dbg_data({
                                   tstore_dbg_sm_signals,
                                   tstore_min_resp_opcode,
                                   tstore_evict_req.source,
                                   tstore_evict_req.subopcode,
                                   tstore_evict_req.address,
                                   tstore_evict_req.data[7:0],
                                   tstore_evict_req.size,
                                   tstore_evict_req.qwen});

// Debug signals from FLN, TBOX, datapath and FLB (flopped)
// ----------------------------------------------
fln_dbg_sm_t                                  fln_dbg_sm_signals_reg;
logic [MinPerN-1:0][IdSize-1:0]  local_message_info_out_id_dbg_reg;
logic [MinPerN-1:0][8-1:0]                 local_message_info_out_data_dbg_reg;
logic [MinPerN-1:0]                        local_message_ready_in_dbg_reg;
logic [MinPerN-1:0]                        local_message_valid_in_dbg_reg;
et_link_neigh_req_info_t                      min_tbox_req_info_dbg_reg;
logic                                         min_tbox_req_valid_dbg_reg;
logic                                         min_tbox_req_ready_dbg_reg;
et_link_neigh_rsp_info_t                      tbox_min_rsp_info_dbg_reg;
logic                                         tbox_min_rsp_valid_dbg_reg;
logic                                         tbox_min_rsp_ready_dbg_reg;
et_link_nodata_req_info_t                     tbox_miss_req_info_dbg_reg;
logic                                         tbox_miss_req_valid_dbg_reg;
logic                                         tbox_miss_req_ready_dbg_reg;
et_link_rsp_info_t                            sc_tbox_rsp_info_dbg_reg;
logic                                         sc_tbox_rsp_valid_dbg_reg;
logic                                         sc_tbox_rsp_ready_dbg_reg;
et_link_nodata_req_info_t                     miss_req_info_ff_proc_dbg_reg;
logic [NeighDataSize-1:0]                     miss_req_data_ff_dbg_reg;
logic                                         miss_req_valid_ff_dbg_reg;
logic                                         miss_req_ready_dbg_reg;
logic                                         int_req_push_dbg_reg;
logic                                         int_req_info_pop_dbg_reg;
et_link_nodata_req_info_t                     out_req_info_in_dbg_reg;
logic [NeighDataSize-1:0]                     out_req_data_in_dbg_reg;
logic [OutFifos-1:0]                         out_req_push_dbg_reg;
et_link_rsp_info_t                            neigh_sc_rsp_info_lo_dbg_reg;
logic                                         neigh_sc_rsp_valid_lo_dbg_reg;
logic                                         neigh_sc_rsp_ready_lo_dbg_reg;
logic [MinPerN-1:0]                        flb_core_neigh_req_valid_dbg_reg;
logic [MinPerN-1:0][CsrFlBarrierSize-1:0] flb_core_neigh_req_data_dbg_reg;
logic [MinPerN-1:0]                        flb_neigh_core_resp_valid_dbg_reg;
logic                                         flb_neigh_core_resp_data_dbg_reg;

//      CLK       EN                       DOUT                                 DIN
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    fln_dbg_sm_signals_reg <= fln_dbg_sm_signals;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    local_message_info_out_id_dbg_reg <= local_message_info_out_id;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    local_message_info_out_data_dbg_reg <= local_message_info_out_data;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    local_message_ready_in_dbg_reg <= local_message_ready_in;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    local_message_valid_in_dbg_reg <= local_message_valid_in;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_tbox_req_info_dbg_reg <= min_tbox_req_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_tbox_req_valid_dbg_reg <= min_tbox_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    min_tbox_req_ready_dbg_reg <= min_tbox_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    tbox_min_rsp_info_dbg_reg <= tbox_min_rsp_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    tbox_min_rsp_valid_dbg_reg <= tbox_min_rsp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    tbox_min_rsp_ready_dbg_reg <= tbox_min_rsp_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    tbox_miss_req_info_dbg_reg <= tbox_miss_req_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    tbox_miss_req_valid_dbg_reg <= tbox_miss_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    tbox_miss_req_ready_dbg_reg <= tbox_miss_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    sc_tbox_rsp_info_dbg_reg <= sc_tbox_rsp_info;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    sc_tbox_rsp_valid_dbg_reg <= sc_tbox_rsp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    sc_tbox_rsp_ready_dbg_reg <= sc_tbox_rsp_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    miss_req_info_ff_proc_dbg_reg <= miss_req_info_ff_proc;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    miss_req_data_ff_dbg_reg <= miss_req_data_ff_proc;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    miss_req_valid_ff_dbg_reg <= miss_req_valid_ff_proc;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    miss_req_ready_dbg_reg <= miss_req_ready;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    int_req_push_dbg_reg <= int_req_push;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    int_req_info_pop_dbg_reg <= int_req_info_pop;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    out_req_info_in_dbg_reg <= out_req_info_in;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    out_req_data_in_dbg_reg <= out_req_data_in;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    out_req_push_dbg_reg <= out_req_push;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    neigh_sc_rsp_info_lo_dbg_reg <= neigh_sc_rsp_info_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    neigh_sc_rsp_valid_lo_dbg_reg <= neigh_sc_rsp_valid_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    neigh_sc_rsp_ready_lo_dbg_reg <= neigh_sc_rsp_ready_lo;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    flb_core_neigh_req_valid_dbg_reg <= flb_core_neigh_req_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    flb_core_neigh_req_data_dbg_reg <= flb_core_neigh_req_data;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    flb_neigh_core_resp_valid_dbg_reg <= flb_neigh_core_resp_valid;
  end
end
always_ff @(posedge clock_lv) begin
  if (neigh_ch_dbg_signals_en) begin
    flb_neigh_core_resp_data_dbg_reg <= flb_neigh_core_resp_data;
  end
end

// Combine match signals from FLN, TBOX, datapath and FLB
assign fln_tbox_dpath_flb_dbg_match_signals = fit_dbg_match({
                                                // FLB signals
                                                flb_neigh_core_resp_valid_dbg_reg,
                                                flb_core_neigh_req_valid_dbg_reg,
                                                // Datapath signals
                                                neigh_sc_rsp_valid_lo_dbg_reg & neigh_sc_rsp_ready_lo_dbg_reg,
                                                out_req_push_dbg_reg,
                                                int_req_info_pop_dbg_reg,
                                                int_req_push_dbg_reg,
                                                miss_req_valid_ff_dbg_reg & miss_req_ready_dbg_reg,
                                                // TBOX signals
                                                sc_tbox_rsp_valid_dbg_reg & sc_tbox_rsp_ready_dbg_reg,
                                                tbox_miss_req_valid_dbg_reg & tbox_miss_req_ready_dbg_reg,
                                                tbox_min_rsp_valid_dbg_reg & tbox_min_rsp_ready_dbg_reg,
                                                min_tbox_req_valid_dbg_reg & min_tbox_req_ready_dbg_reg,
                                                // FLN signals
                                                local_message_valid_in_dbg_reg & local_message_ready_in_dbg_reg,
                                                fln_dbg_sm_signals_reg.local_message_valid_out});

// FLN debug signals
assign fln_dbg_filter_signals = fit_dbg_filter({
                                  fln_dbg_sm_signals_reg.local_message_valid_out,
                                  local_message_info_out_id_dbg_reg});

assign fln_dbg_data_signals = fit_dbg_data({
                                local_message_info_out_id_dbg_reg,
                                local_message_info_out_data_dbg_reg});

// TBOX debug signals
assign tbox_dbg_filter_signals = fit_dbg_filter({
                                   sc_tbox_rsp_valid_dbg_reg & sc_tbox_rsp_ready_dbg_reg,
                                   sc_tbox_rsp_info_dbg_reg.opcode,
                                   tbox_miss_req_valid_dbg_reg & tbox_miss_req_ready_dbg_reg,
                                   tbox_miss_req_info_dbg_reg.address[PaSize-1:6],
                                   tbox_min_rsp_valid_dbg_reg & tbox_min_rsp_ready_dbg_reg,
                                   tbox_min_rsp_info_dbg_reg.id,
                                   tbox_min_rsp_info_dbg_reg.dest,
                                   min_tbox_req_valid_dbg_reg & min_tbox_req_ready_dbg_reg,
                                   min_tbox_req_info_dbg_reg.address[AdEsrMessageIdStart +: AdEsrMessageIdSize]});

assign tbox_dbg_data_signals = fit_dbg_data({
                                 sc_tbox_rsp_info_dbg_reg.opcode,
                                 sc_tbox_rsp_info_dbg_reg.data[7:0],
                                 tbox_miss_req_info_dbg_reg.address[PaSize-1:6],
                                 tbox_min_rsp_info_dbg_reg.id,
                                 tbox_min_rsp_info_dbg_reg.dest,
                                 tbox_min_rsp_info_dbg_reg.data[7:0],
                                 tbox_min_rsp_info_dbg_reg.size,
                                 min_tbox_req_info_dbg_reg.address[AdEsrMessageIdStart +: AdEsrMessageIdSize],
                                 min_tbox_req_info_dbg_reg.data[7:0],
                                 min_tbox_req_info_dbg_reg.size});

// Datapath debug signals
assign dpath_dbg_filter_signals = fit_dbg_filter({
                                    neigh_sc_rsp_valid_lo_dbg_reg & neigh_sc_rsp_ready_lo_dbg_reg,
                                    neigh_sc_rsp_info_lo_dbg_reg.id,
                                    neigh_sc_rsp_info_lo_dbg_reg.dest,
                                    neigh_sc_rsp_info_lo_dbg_reg.opcode,
                                    out_req_push_dbg_reg,
                                    out_req_info_in_dbg_reg.id,
                                    out_req_info_in_dbg_reg.source,
                                    out_req_info_in_dbg_reg.opcode,
                                    out_req_info_in_dbg_reg.address,
                                    miss_req_valid_ff_dbg_reg & miss_req_ready_dbg_reg,
                                    miss_req_info_ff_proc_dbg_reg.id,
                                    miss_req_info_ff_proc_dbg_reg.source,
                                    miss_req_info_ff_proc_dbg_reg.opcode,
                                    miss_req_info_ff_proc_dbg_reg.address});

assign dpath_dbg_data_signals = fit_dbg_data({
                                  neigh_sc_rsp_info_lo_dbg_reg.id,
                                  neigh_sc_rsp_info_lo_dbg_reg.dest,
                                  neigh_sc_rsp_info_lo_dbg_reg.opcode,
                                  neigh_sc_rsp_info_lo_dbg_reg.data[7:0],
                                  out_req_info_in_dbg_reg.id,
                                  out_req_info_in_dbg_reg.source,
                                  out_req_info_in_dbg_reg.opcode,
                                  out_req_info_in_dbg_reg.address[15:0],
                                  out_req_data_in_dbg_reg[7:0],
                                  miss_req_info_ff_proc_dbg_reg.id,
                                  miss_req_info_ff_proc_dbg_reg.source,
                                  miss_req_info_ff_proc_dbg_reg.opcode,
                                  miss_req_info_ff_proc_dbg_reg.address,
                                  miss_req_data_ff_dbg_reg[7:0]});

// FLB debug signals
assign flb_dbg_filter_signals = fit_dbg_filter({
                                  flb_neigh_core_resp_valid_dbg_reg,
                                  flb_neigh_core_resp_data_dbg_reg,
                                  flb_core_neigh_req_valid_dbg_reg,
                                  flb_core_neigh_req_data_dbg_reg});

assign flb_dbg_data_signals = fit_dbg_data({
                                flb_neigh_core_resp_data_dbg_reg,
                                flb_core_neigh_req_data_dbg_reg});

// Multiplex debug signals from every neighborhood channel module
always_comb
begin
    // North minion debug signals
    if (neigh_ch_dbg_mod_idx < NeighChDebugSmModMin4)
    begin
        neigh_ch_dbg_signals_next.match_signals  = min_north_dbg_match_signals;
        neigh_ch_dbg_signals_next.match_valid    = 1'b1;
        neigh_ch_dbg_signals_next.filter_signals = min_north_dbg_filter_signals;
        neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
        neigh_ch_dbg_signals_next.data_signals   = min_north_dbg_data_signals;
        neigh_ch_dbg_signals_next.data_valid     = 1'b1;
    end
    // South minion debug signals
    else if (neigh_ch_dbg_mod_idx < NeighChDebugSmModIcache)
    begin
        neigh_ch_dbg_signals_next.match_signals  = min_south_dbg_match_signals;
        neigh_ch_dbg_signals_next.match_valid    = 1'b1;
        neigh_ch_dbg_signals_next.filter_signals = min_south_dbg_filter_signals;
        neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
        neigh_ch_dbg_signals_next.data_signals   = min_south_dbg_data_signals;
        neigh_ch_dbg_signals_next.data_valid     = 1'b1;
    end
    else if (neigh_ch_dbg_mod_idx < NeighChDebugSmModTload)
    begin
        // Match signals combine signals from Icache and PTW
        neigh_ch_dbg_signals_next.match_signals = icache_ptw_dbg_match_signals;
        neigh_ch_dbg_signals_next.match_valid   = 1'b1;

        // Icache debug signals
        if (neigh_ch_dbg_mod_idx == NeighChDebugSmModIcache)
        begin
            neigh_ch_dbg_signals_next.filter_signals = icache_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = icache_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
        // PTW debug signals
        else
        begin
            neigh_ch_dbg_signals_next.filter_signals = ptw_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = ptw_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
    end
    else if (neigh_ch_dbg_mod_idx < NeighChDebugSmModFln)
    begin
        // Match signals combine signals from Cooperative TLoad and TStore
        neigh_ch_dbg_signals_next.match_signals = tload_tstore_dbg_match_signals;
        neigh_ch_dbg_signals_next.match_valid   = 1'b1;

        // Cooperative TLoad debug signals
        if (neigh_ch_dbg_mod_idx == NeighChDebugSmModTload)
        begin
            neigh_ch_dbg_signals_next.filter_signals = tload_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = tload_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
        // TStore debug signals
        else
        begin
            neigh_ch_dbg_signals_next.filter_signals = tstore_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = tstore_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
    end
    else
    begin
        // Match signals combine signals from FLN, TBOX, datapath and FLB
        neigh_ch_dbg_signals_next.match_signals = fln_tbox_dpath_flb_dbg_match_signals;
        neigh_ch_dbg_signals_next.match_valid   = 1'b1;

        // FLN debug signals
        if (neigh_ch_dbg_mod_idx == NeighChDebugSmModFln)
        begin
            neigh_ch_dbg_signals_next.filter_signals = fln_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = fln_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
        // TBOX debug signals
        else if (neigh_ch_dbg_mod_idx == NeighChDebugSmModTbox)
        begin
            neigh_ch_dbg_signals_next.filter_signals = tbox_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = tbox_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
        // Datapath debug signals
        else if (neigh_ch_dbg_mod_idx == NeighChDebugSmModDpath)
        begin
            neigh_ch_dbg_signals_next.filter_signals = dpath_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = dpath_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
        // FLB debug signals
        else
        begin
            neigh_ch_dbg_signals_next.filter_signals = flb_dbg_filter_signals;
            neigh_ch_dbg_signals_next.filter_valid   = 1'b1;
            neigh_ch_dbg_signals_next.data_signals   = flb_dbg_data_signals;
            neigh_ch_dbg_signals_next.data_valid     = 1'b1;
        end
    end
end

esr_dll_dly_est_ctl_t     dll_dly_est_ctl;
esr_dll_dly_est_sts_t     dll_dly_est_sts;

////////////////////////////////////////////////////////////////////////////////
// DLL Delay Estimation
////////////////////////////////////////////////////////////////////////////////
assign dll_dly_est_ctl      = esr_dll_dly_est_ctl;
assign esr_dll_dly_est_sts  = dll_dly_est_sts;
dll_dly_est
dll_dly_est(
  .clk_shire_i ( clock_shire ), //hiv
  .rst_shire_ni ( ~(reset_w_shire_rsync) ), //hiv
  .clk_neigh_i ( clock_lv ), //lov
  .rst_neigh_ni ( ~(reset_w_neigh_ff) ), //lov
  .dll_dly_est_ctl_i ( dll_dly_est_ctl ), //hiv
  .dll_dly_est_sts_o ( dll_dly_est_sts )  //hiv
);


////////////////////////////////////////////////////////////////////////////////
// HI VOLTAGE CROSSING
////////////////////////////////////////////////////////////////////////////////
bpam_rc_tbox_ack_t bpam_rc_tbox_ack_hi_next;

neigh_hv_logic_bpam_rc_tbox_ack
neigh_hv_logic_bpam_rc_tbox_ack_pd_hv (
  .clk_hv_i ( clock_shire ),
  .rst_hv_ni ( ~(reset_d_shire_rsync) ),
  .bpam_rc_tbox_ack_hi_reg_i ( bpam_rc_tbox_ack_hi_next ),
  .bpam_rc_tbox_ack_hi_reg_o ( bpam_rc_tbox_ack_hi )
);

logic reset_c_neigh_hvc ;
rst_repeat rst_repeat_reset_c_hvc (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_c_neigh), .reset_out(reset_c_neigh_hvc));

neigh_hi_voltage_cross
#(
   .FifoDepth (ShireHiVoltageFifoSize)
)
hi_voltage_cross
(
   // Same reset signals are used to reset the block on both sides (push/pop)
   .rst_c_push_ni ( ~(reset_c_shire_rsync) ),
   .rst_c_pop_ni ( ~(reset_c_neigh_hvc) ),
   .rst_push_ni ( ~(reset_w_shire_rsync) ),
   .rst_pop_ni ( ~(reset_w_neigh_ff) ),
   .clk_push_i ( clock_shire ),
   .clk_pop_i ( clock_lv ),

   // Dft
   .dft_hv_i ( dft_hv_i ),
   .dft_lv_i ( dft_lv_o ),

   .neigh_sm_gpio_i ( neigh_sm_gpio ),
   .neigh_sm_gpio_sync_o ( neigh_sm_gpio_lo ),

   .shire_id_i ( shire_id ),
   .shire_id_sync_o ( shire_id_lo ),
   .neigh_id_i ( neigh_id ),
   .neigh_id_sync_o ( neigh_id_lo ),
   .tbox_id_i ( shire_tbox_id ),
   .tbox_id_sync_o ( tbox_id_lo ),
   .tbox_en_i ( shire_tbox_en ),
   .tbox_en_sync_o ( tbox_en_lo ),
   .neigh_t0_en_i ( esr_thread0_enable ),
   .neigh_t0_en_sync_o ( esr_thread0_enable_lo ),
   .neigh_t1_en_i ( esr_thread1_enable ),
   .neigh_t1_en_sync_o ( esr_thread1_enable_lo ),
   .esr_minion_features_i ( esr_minion_features ),
   .esr_minion_features_sync_o ( esr_minion_features_lo ),
   .chicken_bits_i ( chicken_bits_vc_in ),
   .chicken_bits_sync_o ( chicken_bits_vc_lo_out ),

   .bpam_run_control_i ( bpam_run_control ),
   .bpam_run_control_sync_o ( bpam_run_control_lo ),

   .hw_dbg_sm_monitor_enabled_i ( hw_dbg_sm_monitor_enabled ),
   .hw_dbg_sm_monitor_enabled_sync_o ( hw_dbg_sm_monitor_enabled_lo ),

   .esr_icache_prefetch_conf_i ( esr_icache_prefetch_conf ),
   .esr_icache_prefetch_conf_sync_o ( esr_icache_prefetch_conf_lo ),
   .esr_icache_prefetch_start_i ( esr_icache_prefetch_start ),
   .esr_icache_prefetch_start_sync_o ( esr_icache_prefetch_start_lo ),
   .dmctrl_i ( dmctrl_struct ),
   .dmctrl_sync_o ( dmctrl_lo ),

   .esr_shire_coop_mode_i ( esr_shire_coop_mode ),
   .esr_shire_coop_mode_sync_o ( esr_shire_coop_mode_lo ),

   .neigh_sc_rsp_valid_i ( neigh_sc_rsp_valid ),
   .neigh_sc_rsp_valid_sync_o ( neigh_sc_rsp_valid_lo ),
   .neigh_sc_rsp_info_i ( neigh_sc_rsp_info ),
   .neigh_sc_rsp_info_sync_o ( neigh_sc_rsp_info_lo ),
   .neigh_sc_rsp_ready_i ( neigh_sc_rsp_ready_lo ),
   .neigh_sc_rsp_ready_sync_o ( neigh_sc_rsp_ready ),

   .apb_esr_req_i ( APB_req_from_shire ),
   .apb_esr_req_sync_o ( APB_req_from_shire_lo ),
   .apb_pready_neigh_i ( APB_rsp_to_shire_hi.apb_pready ),
   .int_mtip_i ( int_mtip ),
   .plic_meip_i ( int_meip ),
   .plic_seip_i ( int_seip ),
   .int_mtip_sync_o ( int_mtip_lo ),
   .plic_meip_sync_o ( int_meip_lo ),
   .plic_seip_sync_o ( int_seip_lo ),

   .esr_to_neigh_ipi_msip_i ( ipi_msip ),
   .esr_to_neigh_ipi_msip_sync_o ( ipi_msip_lo ),
   .esr_to_neigh_ipi_trigger_i ( ipi_redirect_trigger ),
   .esr_to_neigh_ipi_trigger_sync_o ( ipi_redirect_trigger_lo ),
   .uc_to_neigh_fcc_i ( uc_to_neigh_fcc ),
   .uc_to_neigh_fcc_sync_o ( uc_to_neigh_fcc_lo ),
   .uc_to_neigh_fcc_target_i ( uc_to_neigh_fcc_target ),
   .uc_to_neigh_fcc_target_sync_o ( uc_to_neigh_fcc_target_lo ),

   .flb_l2_neigh_resp_valid_i ( flb_l2_neigh_resp_valid ),
   .flb_l2_neigh_resp_valid_sync_o ( flb_l2_neigh_resp_valid_lo ),
   .flb_l2_neigh_resp_data_i ( flb_l2_neigh_resp_data ),
   .flb_l2_neigh_resp_data_sync_o ( flb_l2_neigh_resp_data_lo ),

   .icache_f0_sram_resp_dout_i ( icache_f0_sram_resp_dout ),
   .icache_f0_sram_resp_dout_sync_o ( icache_f0_sram_resp_dout_lo ),
   .icache_f0_sram_resp_valid_i ( icache_f0_sram_resp_valid ),
   .icache_f0_sram_resp_valid_sync_o ( icache_f0_sram_resp_valid_lo ),
   .icache_f0_sram_resp_ready_i ( icache_f0_sram_resp_ready_lo ),
   .icache_f0_sram_resp_ready_sync_o ( icache_f0_sram_resp_ready )
); //neigh_hi_voltage_cross

logic reset_c_neigh_lvc ;
rst_repeat rst_repeat_reset_c_lvc (.dft__reset_byp(dft__reset_byp_lv), .dft__reset(dft__reset_lv), .clock(clock_lv), .reset_in(reset_c_neigh), .reset_out(reset_c_neigh_lvc));

neigh_lo_voltage_cross
#(
   .FifoDepth (ShireLoVoltageFifoSize),
   .StubMinions (StubMinions)
)
lo_voltage_cross
(
   .rst_c_push_ni ( ~(reset_c_neigh_lvc) ),
   .rst_c_pop_ni ( ~(reset_c_shire_rsync) ),
   .rst_push_ni ( ~(reset_w_neigh_ff) ),
   .rst_pop_ni ( ~(reset_w_shire_rsync) ),
   .clk_push_i ( clock_lv ),
   .clk_pop_i ( clock_shire ),

   // Dft
   .dft_hv_i ( dft_hv_i ),
   .dft_lv_i ( dft_lv_o ),

   .rst_w_icache_ni ( ~(reset_w_neigh_ff) ),
   .rst_w_icache_sync_no ( rst_w_icache_n ),

    //Debug control
   .debug_rst_pop_ni ( ~(reset_d_shire_rsync) ),
   .debug_rst_push_ni ( ~(reset_d_neigh_ff) ),

   .esr_icache_prefetch_done_i ( esr_icache_prefetch_done_lo ),
   .esr_icache_prefetch_done_sync_o ( esr_icache_prefetch_done ),

   .esr_icache_err_detected_i ( esr_icache_err_detected_lo ),
   .esr_icache_err_detected_sync_o ( esr_icache_err_detected_op ),
   .esr_icache_err_logged_i ( esr_icache_err_logged_lo ),
   .esr_icache_err_logged_sync_o ( esr_icache_err_logged_op ),

   .bpam_rc_tbox_ack_i ( tbox_bpam_run_control_ack ),
   .bpam_rc_tbox_ack_sync_o ( bpam_rc_tbox_ack_hi_next ),

   .esr_and_or_tree_l0_i ( esr_and_or_tree_L0_lo ),
   .esr_and_or_tree_l0_sync_o ( esr_and_or_tree_L0_op ),

   .neigh_sm_signals_i ( neigh_sm_signals_lo ),
   .neigh_sm_signals_sync_o ( neigh_sm_signals ),

   .apb_esr_rsp_i ( APB_rsp_to_shire_lo ),
   .apb_esr_rsp_sync_o ( APB_rsp_to_shire_hi ),

   .flb_neigh_l2_req_valid_i ( flb_neigh_l2_req_valid_lo ),
   .flb_neigh_l2_req_valid_sync_o ( flb_neigh_l2_req_valid ),
   .flb_neigh_l2_req_data_i ( flb_neigh_l2_req_data_lo ),
   .flb_neigh_l2_req_data_sync_o ( flb_neigh_l2_req_data ),

   .icache_f2_sram_req_write_i ( icache_f2_sram_req_write_lo ),
   .icache_f2_sram_req_write_sync_o ( icache_f2_sram_req_write ),
   .icache_f2_sram_req_addr_i ( icache_f2_sram_req_addr_lo ),
   .icache_f2_sram_req_addr_sync_o ( icache_f2_sram_req_addr ),
   .icache_f2_sram_req_valid_i ( icache_f2_sram_req_valid_lo ),
   .icache_f2_sram_req_valid_sync_o ( icache_f2_sram_req_valid ),
   .icache_f2_sram_req_ready_i ( icache_f2_sram_req_ready ),
   .icache_f2_sram_req_ready_sync_o ( icache_f2_sram_req_ready_lo )
); //shire_lo_voltage_cross


////////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////////

function automatic logic isGlobalAtomic;
    input et_link_nodata_req_info_t req;
    begin
        isGlobalAtomic = (req.opcode == etlink_pkg::ReqAtomic) &&
                         (req.subopcode[EtAtomicDestStart] == EtAtomicDestGlobal);
    end
endfunction

function automatic logic isMsgToTbox;
    input et_link_nodata_req_info_t req;
    begin
        isMsgToTbox = ((req.opcode == etlink_pkg::ReqMsgSendData)
                    && (req.address[AdEsrRegionStart +: AdEsrRegionSize] == AdEsrRegion)
                    && (req.address[AdEsrAgentTypeStart +: AdEsrAgentTypeSize] == AdEsrAgentTypeTbox));
    end
endfunction

function automatic logic isMsgToSameShire;
    input et_link_nodata_req_info_t req;
    input logic [NumShireIdsW-1:0]  shire_id;
    begin
        isMsgToSameShire = (req.opcode == etlink_pkg::ReqMsgSendData)
                         && (req.address[AdEsrRegionStart +: AdEsrRegionSize] == AdEsrRegion)
                         && ((req.address[AdEsrShireIdStart +: AdEsrShireIdSize] == shire_id) ||
                             (req.address[AdEsrShireIdStart +: AdEsrShireIdSize] == LocalShireId));
    end
endfunction

function automatic logic isMsgToDifferentShire;
    input et_link_nodata_req_info_t req;
    input logic [NumShireIdsW-1:0]  shire_id;
    begin
        isMsgToDifferentShire = (req.opcode == etlink_pkg::ReqMsgSendData)
                              && (req.address[AdEsrRegionStart +: AdEsrRegionSize] == AdEsrRegion)
                              && (req.address[AdEsrShireIdStart +: AdEsrShireIdSize] != shire_id)
                              && (req.address[AdEsrShireIdStart +: AdEsrShireIdSize] != LocalShireId);
    end
endfunction

function automatic logic isUncacheableReadWrite;
    input et_link_nodata_req_info_t req;
    begin
        isUncacheableReadWrite = ((req.opcode == etlink_pkg::ReqRead)  ||
                                  (req.opcode == etlink_pkg::ReqWrite) ||
                                  (req.opcode == etlink_pkg::ReqReadCoop))
                               && !(req.address[AdDdrRegionStart +: AdDdrRegionSize] == AdDdrRegion) // It is not in DDR region
                               && !(req.address[AdScpRegionStart +: AdScpRegionSize] == AdScpRegion); // It is not in SCP region
    end
endfunction

function automatic logic isWriteAroundtoLocalSCP;
    input et_link_nodata_req_info_t req;
    input logic [NumShireIdsW-1:0]  shire_id;
    begin
        isWriteAroundtoLocalSCP = (req.opcode == etlink_pkg::ReqWriteAround)
                                && (req.address[AdScpRegionStart +: AdScpRegionSize] == AdScpRegion)
                                && ((req.address[AdScpFl2ShireIdStart +: AdScpFl2ShireIdSize] == shire_id) ||
                                    (req.address[AdScpFl2ShireIdStart +: AdScpFl2ShireIdSize] == LocalShireId));
    end
endfunction

function automatic logic isAgentForced;
    input etlink_pkg::neigh_source_e source;
    input esr_neigh_chicken_t    chicken;

    begin
        isAgentForced = ((chicken.agent_forced == NeighForceAgentDcache) &&
                          ((source == etlink_pkg::SrcMinion0) ||
                           (source == etlink_pkg::SrcMinion1) ||
                           (source == etlink_pkg::SrcMinion2) ||
                           (source == etlink_pkg::SrcMinion3) ||
                           (source == etlink_pkg::SrcMinion4) ||
                           (source == etlink_pkg::SrcMinion5) ||
                           (source == etlink_pkg::SrcMinion6) ||
                           (source == etlink_pkg::SrcMinion7) ||
                           (source == etlink_pkg::SrcCoop))
                        ) ||
                        ((chicken.agent_forced == NeighForceAgentIcache) &&
                          (source == etlink_pkg::SrcIcache)
                        ) ||
                        ((chicken.agent_forced == NeighForceAgentTbox) &&
                          (source == etlink_pkg::SrcTbox)
                        ) ||
                        ((chicken.agent_forced == NeighForceAgentPtw) &&
                          ((source == etlink_pkg::SrcPtw0) ||
                           (source == etlink_pkg::SrcPtw1))
                        );
    end
endfunction

function automatic [DestFifosW-1:0] destFifoForced;
    input et_link_nodata_req_info_t req;
    input esr_neigh_chicken_t       chicken;

    logic [ScBankIdSize-1:0] bank_id;
    logic [ScBankIdSize-1:0] neigh_to_bank_id;
    begin
        bank_id          = req.address[ScBankLsb +: ScBankIdSize];
        neigh_to_bank_id = (ScBankIdSize)'(req.address[AdEsrNeighIdStart+:NumNeighW]);

        destFifoForced = chicken.dest_fifo == NeighForceDestFifoBanks ?
                         // If forced to bank FIFOs, choose bank according to address
                         ( req.opcode == etlink_pkg::ReqMsgSendData           ? dest_fifo_from_bank(neigh_to_bank_id) // Message goes to bank according to neigh ID LSBs
                         :                                                   dest_fifo_from_bank(bank_id)          // Otherwise, go to corresponding bank FIFO
                         )
                       : chicken.dest_fifo == NeighForceDestFifoUc    ? UcFifoDest          // Go to UC FIFO
                       :                                                     TboxFifo;       // Go to TBOX FIFO
    end
endfunction

function automatic [DestFifosW-1:0] destFifo;
    input et_link_nodata_req_info_t req;
    input logic [NumShireIdsW-1:0]  sid;
    input esr_neigh_chicken_t       chicken;

    logic [ScBankIdSize-1:0] bank_id;
    logic [ScBankIdSize-1:0] neigh_to_bank_id;
    begin
        bank_id          = req.address[ScBankLsb +: ScBankIdSize];
        neigh_to_bank_id = (ScBankIdSize)'(req.address[AdEsrNeighIdStart+:NumNeighW]);

        destFifo = chicken.force_dest_fifo & (chicken.force_all_agents | isAgentForced(etlink_pkg::neigh_source_e'(req.source),chicken)) ? destFifoForced(req,chicken) // If forced, send to forced destination FIFO
                 // Global atomics
                 : isGlobalAtomic(req)            ? UcFifoDest          // Global atomics go directly to L3, so they are handled by the UC
                 // Messages
                 : isMsgToTbox(req)               ? TboxFifo        // Message to TBOX goes to TBOX FIFO
                 : isMsgToSameShire(req,sid)      ? dest_fifo_from_bank(neigh_to_bank_id) // Message to same shire goes to bank FIFOs according to neigh ID LSBs
                 : isMsgToDifferentShire(req,sid) ? UcFifoDest          // Message to different shire goes to UC FIFO
                 // Uncacheable Reads/Writes
                 : isUncacheableReadWrite(req)    ? UcFifoDest          // Regular uncacheable read/write accesses go to UC FIFO (neither in SCP nor in DDR regions)
                 // Other: local Atomics, WriteArounds, CacheOps and
                 //        cacheable Reads, ReadCoops and Writes
                 :                                  dest_fifo_from_bank(bank_id);         // Always go to Shire Cache, so go to corresponding bank FIFO
    end
endfunction

function automatic [PaSize-1:0] commonL2AddrFormat;
    input logic [PaSize-1:0] in_addr;
    begin
        // By default, leave it as is
        commonL2AddrFormat = in_addr;

        // If it is a SCP address, convert it to common L2 format
        if (in_addr[AdScpRegionStart +: AdScpRegionSize] == AdScpRegion)
        begin
            // Format 0
            if (in_addr[AdScpFormatStart +: AdScpFormatSize] == AdScpFormat0)
                // Just duplicate shire_id MSB from format 0 into shire_id MSB of common L2 format
                commonL2AddrFormat[AdScpFl2ShireIdMsb] = in_addr[AdScpF0ShireIdMsb];
            // Format 1
            else
            begin
                // Re-arrange shire_id and L2 SCP offset fields and duplicate shire_id MSB from format 0 into shire_id MSB of common L2 format
                commonL2AddrFormat[AdScpFl2OffsetStart +: AdScpFl2OffsetSize]   = {in_addr[AdScpF1OffsetStart1 +: AdScpF1OffsetSize1],in_addr[AdScpF1OffsetStart0 +: AdScpF1OffsetSize0]};
                commonL2AddrFormat[AdScpFl2ShireIdStart +: AdScpFl2ShireIdSize] = {in_addr[AdScpF1ShireIdMsb],in_addr[AdScpF1ShireIdStart1 +: AdScpF1ShireIdSize1],in_addr[AdScpF1ShireIdStart0 +: AdScpF1ShireIdSize0]};
            end
        end
    end
endfunction

endmodule : neigh_channel
/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on VARHIDDEN */
/* verilator lint_on UNOPTFLAT */
