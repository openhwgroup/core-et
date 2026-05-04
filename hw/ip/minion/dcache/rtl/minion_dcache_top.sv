// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache top-level integration.
//
// Replaces: dcache_top

module minion_dcache_top
  import minion_pkg::*;
  import minion_dcache_pkg::*;
#(
  parameter bit DebugApbEn = 1'b1,
  parameter bit DebugMonEn = 1'b1,
  parameter bit VpuEn = 1'b1
)
(
  input  logic                                               clk_i,
  input  logic                                               rst_ni,
  input  logic [NumShireIdsBits-1:0]                         shire_id_i,
  /* verilator lint_off UNUSEDSIGNAL */  // `VpuEn=0` removes the tensor/offload fabric, leaving some control fields and upper bits intentionally unused in scalar-only builds.
  input  logic [MinPerSBits-1:0]                             shire_min_id_i,
  input  logic                                               ioshire_i,
  input  logic                                               chicken_bit_dcache_i,
  input  core_dcache_ctrl                                    core_ctrl_i,
  output dcache_core_ctrl                                    core_ctrl_resp_o,
  input  logic                                               id_core_alloc_rq_pre_i,
  input  logic                                               s0_core_alloc_rq_val_i,
  input  logic                                               id_core_gsc_i,
  output logic                                               id_core_ready_o,
  input  logic                                               s0_core_req_valid_i,
  input  minion_dcache_req                                   s0_core_req_i,
  input  logic                                               s0_core_gsc_i,
  input  logic                                               s1_core_kill_i,
  input  logic [VpuDataSz-1:0]                               s1_core_store_data_i,
  input  esr_mprot_t                                         s1_mprot_i,
  input  logic                                               s2_core_kill_i,
  input  logic [minion_dcache_pkg::XregSize-1:0]                                s3_core_x31_i,
  input  vpu_dcache_ctrl                                     s1_vpu_ctrl_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output dcache_vpu_scp_resp                                 s3_vpu_scp_resp_o,
  output logic [VpuDataSz-1:0]                               s3_vpu_scp_data_o,
  output logic [VpuDataSz-1:0]                               s3_vpu_tenb_data_o,
  output dcache_vpu_reduce_ctrl                              vpu_reduce_ctrl_o,
  output logic                                               s2_core_resp_int_valid_o,
  output logic                                               s3_core_resp_valid_o,
  output dcache_minion_resp                                  s3_core_resp_o,
  output dcache_minion_scoreboard                            id_core_scoreboard_o,
  output logic [NrThreads-1:0]                               id_core_sb_fp_dealloc_o,
  output logic [NrThreads-1:0]                               id_core_sb_int_dealloc_o,
  output logic                                               s1_core_replay_next_o,
  output dcache_tag_xcpt                                     s1_core_xcpt_o,
  output logic                                               s2_core_flush_pipeline_o,
  output logic [NrThreads-1:0]                               s3_ordered_o,
  input  logic                                               s3_invalidate_lr_i,
  input  logic                                               wb_dcache_fp_toint_i,
  input  tlb_entry_type                                      vmspagesize_i,
  input  logic                                               bypass_dcache_i,
  input  logic [MinionMemCtrlBits-1:0]                       mem_ctrl_override_i,
  input  minion_satp_info                                    satp_info_i,
  input  minion_satp_info                                    matp_info_i,
  input  logic                                               satp_info_en_i,
  input  logic                                               matp_info_en_i,
  input  minion_dcache_pkg::vm_status_t [NrThreads-1:0] vm_status_i,
  input  logic                                               tlb_invalidate_i,
  output minion_ptw_req                                      ptw_req_data_o,
  output logic                                               ptw_req_valid_o,
  input  logic                                               ptw_req_ready_i,
  input  minion_ptw_pte                                      ptw_resp_data_i,
  input  logic                                               ptw_resp_valid_i,
  input  logic [DcacheL2EvictReqPorts-1:0]                   l2_evict_req_ready_i,
  output logic [DcacheL2EvictReqPorts-1:0]                   l2_evict_req_valid_o,
  output et_link_minion_evict_req_info_t                     l2_evict_req_o,
  input  logic [DcacheL2MissReqPorts-1:0]                    l2_miss_req_ready_i,
  output logic [DcacheL2MissReqPorts-1:0]                    l2_miss_req_valid_o,
  output et_link_minion_miss_req_info_t                      l2_miss_req_o,
  output logic                                               l2_resp_ready_o,
  input  logic                                               l2_resp_valid_i,
  input  et_link_minion_rsp_info_t                           l2_resp_i,
  output logic                                               l2_resp_ready_int_o,
  output logic                                               l2_resp_valid_int_o,
  output et_link_minion_rsp_info_t                           l2_resp_int_o,
  output dcache_core_bp_t                                    s1_bp_conf_o,
  output logic                                               s1_bp_conf_valid_o,
  output logic                                               dcache_idle_excl_mode_o,
  output logic [DcacheTlErrorBits-3:0]                       tensor_load_err_flags_o,
  output logic [DcacheErrFlagBits-1:0][NrThreads-1:0]        cache_ops_err_flags_o,
  output logic                                               tensor_reduce_err_flags_o,
  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` compiles out the dcache debug window while preserving the shared APB/debug port contract for feature-on builds.
  input  logic [DcacheDbgAddrBits-1:0]                       apb_paddr_i,
  input  logic                                               apb_pwrite_i,
  input  logic                                               apb_psel_i,
  input  logic                                               apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]                   apb_pwdata_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic                                               apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]                   apb_prdata_o,
  output logic                                               apb_pslverr_o,
  output logic [CsrNrEventsDcache-1:0]                       io_events_o,
  output logic [NrThreads-1:0]                               bus_err_o,
  output logic [NrThreads-1:0][minion_dcache_pkg::PaSize-1:0]                   bus_err_addr_o,
  output dcache_debug_sigs                                   csr_debug_sigs_o,
  output logic [DcacheDbgSmMatchBits-1:0]                    sm_match_debug_signals_tl0_o,
  output logic [DcacheDbgSmFilterBits-1:0]                   sm_filter_debug_signals_tl0_o,
  output logic [DcacheDbgSmDataBits-1:0]                     sm_data_debug_signals_tl0_o,
  output logic [DcacheDbgSmMatchBits-1:0]                    sm_match_debug_signals_tl1_o,
  output logic [DcacheDbgSmFilterBits-1:0]                   sm_filter_debug_signals_tl1_o,
  output logic [DcacheDbgSmDataBits-1:0]                     sm_data_debug_signals_tl1_o,
  output logic [DcacheDbgSmMatchBits-1:0]                    sm_match_debug_signals_ts_o,
  output logic [DcacheDbgSmFilterBits-1:0]                   sm_filter_debug_signals_ts_o,
  output logic [DcacheDbgSmDataBits-1:0]                     sm_data_debug_signals_ts_o,
  output logic [DcacheDbgSmMatchBits-1:0]                    sm_match_debug_signals_co_o,
  output logic [DcacheDbgSmFilterBits-1:0]                   sm_filter_debug_signals_co_o,
  output logic [DcacheDbgSmDataBits-1:0]                     sm_data_debug_signals_co_o,
  output logic [DcacheDbgSmMatchBits-1:0]                    sm_match_debug_signals_col2_o,
  output logic [DcacheDbgSmFilterBits-1:0]                   sm_filter_debug_signals_col2_o,
  output logic [DcacheDbgSmDataBits-1:0]                     sm_data_debug_signals_col2_o,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_mh_o,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_mh_o,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]   sm_data_debug_signals_mh_o,
  output logic [DcacheDbgSmMatchBits-1:0]                    sm_match_debug_signals_dc_o,
  output logic [DcacheDbgSmFilterBits-1:0]                   sm_filter_debug_signals_dc_o,
  output logic [DcacheDbgSmDataBits-1:0]                     sm_data_debug_signals_dc_o
);

  typedef enum logic [2:0] {
    S1DaMuxSelCore = 3'd0,
    S1DaMuxSelWb   = 3'd1,
    S1DaMuxSelVpu  = 3'd2,
    S1DaMuxSelTs   = 3'd3,
    S1DaMuxSelDbg  = 3'd4
  } s1_da_mux_sel_e;

  localparam int unsigned DcacheL2EvictArbPorts = 4;
  localparam int unsigned DcacheL2EvictWbPort = 0;
  localparam int unsigned DcacheL2EvictMhPort = 1;
  localparam int unsigned DcacheL2EvictRePort = 2;
  localparam int unsigned DcacheL2EvictPfPort = 3;

  localparam int unsigned DcacheL2MissArbPorts = 4;
  localparam int unsigned DcacheL2MissTxPort = 0;
  localparam int unsigned DcacheL2MissMhPort = 1;
  localparam int unsigned DcacheL2MissTl0Port = 2;
  localparam int unsigned DcacheL2MissTl1Port = 3;

  localparam int unsigned DcacheL2EvictMuxPorts = 2;
  localparam int unsigned DcacheL2MissMuxPorts = 3;
  localparam int unsigned DcacheL2EvictIdxTstore = 0;
  localparam int unsigned DcacheL2EvictIdxComm = 1;
  localparam int unsigned DcacheL2MissIdxScp = 0;
  localparam int unsigned DcacheL2MissIdxTenb = 1;
  localparam int unsigned DcacheL2MissIdxComm = 2;
  localparam int unsigned LrscCycles = 32;
  localparam int unsigned LrscCountWidth = $clog2(LrscCycles);
  localparam int unsigned GscLaneBits = DcacheDataSize / VpuLaneNum;

  et_link_minion_evict_req_info_t [DcacheL2EvictArbPorts-1:0] l2_evict_arb_data_in;
  logic [DcacheL2EvictArbPorts-1:0]                           l2_evict_arb_bid;
  logic [DcacheL2EvictArbPorts-1:0]                           l2_evict_arb_grant;

  et_link_minion_evict_req_info_t [DcacheL2EvictMuxPorts-1:0] l2_evict_pipe_data_in;
  logic [DcacheL2EvictMuxPorts-1:0]                           l2_evict_pipe_valid;
  logic [DcacheL2EvictMuxPorts-1:0]                           l2_evict_pipe_ready;
  logic [DcacheL2EvictMuxPorts-1:0]                           l2_evict_pipe_valid_en;
  logic [DcacheL2EvictMuxPorts-1:0]                           l2_evict_pipe_data_en;

  et_link_minion_evict_req_info_t [DcacheL2EvictMuxPorts-1:0] l2_evict_mux_data_in;
  logic [DcacheL2EvictMuxPorts-1:0]                           l2_evict_mux_valid;
  logic [DcacheL2EvictMuxPorts-1:0]                           l2_evict_mux_ready;

  et_link_minion_miss_req_info_t [DcacheL2MissArbPorts-1:0]   l2_miss_arb_data_in;
  logic [DcacheL2MissArbPorts-1:0]                            l2_miss_arb_bid;
  logic [DcacheL2MissArbPorts-1:0]                            l2_miss_arb_grant;

  et_link_minion_miss_req_info_t [DcacheL2MissMuxPorts-1:0]   l2_miss_pipe_data_in;
  logic [DcacheL2MissMuxPorts-1:0]                            l2_miss_pipe_valid;
  logic [DcacheL2MissMuxPorts-1:0]                            l2_miss_pipe_ready;
  logic [DcacheL2MissMuxPorts-1:0]                            l2_miss_pipe_valid_en;
  logic [DcacheL2MissMuxPorts-1:0]                            l2_miss_pipe_data_en;

  et_link_minion_miss_req_info_t [DcacheL2MissMuxPorts-1:0]   l2_miss_mux_data_in;
  logic [DcacheL2MissMuxPorts-1:0]                            l2_miss_mux_valid;
  logic [DcacheL2MissMuxPorts-1:0]                            l2_miss_mux_ready;

  et_link_minion_evict_req_info_t                             wb_l2_evict_req;
  et_link_minion_evict_req_info_t                             mh_l2_evict_req;
  et_link_minion_evict_req_info_t                             re_l2_evict_req;
  et_link_minion_evict_req_info_t                             ts_l2_evict_req;
  et_link_minion_evict_req_info_t                             pf_l2_evict_req;
  et_link_minion_evict_req_info_t                             arb_l2_evict_req;

  et_link_minion_miss_req_info_t                              tx_l2_miss_req;
  et_link_minion_miss_req_info_t                              mh_l2_miss_req;
  et_link_minion_miss_req_info_t                              tl0_l2_miss_req;
  et_link_minion_miss_req_info_t                              tl1_l2_miss_req;
  et_link_minion_miss_req_info_t                              arb_l2_miss_req;

  /* verilator lint_off UNOPTFLAT */  // The preserved multi-source L2 evict arbitration crosses submodule ready/valid feedback that Verilator over-flattens into false loops.
  logic                                                       wb_l2_evict_valid;
  logic                                                       wb_l2_evict_ready;
  logic                                                       mh_l2_evict_valid;
  logic                                                       mh_l2_evict_ready;
  logic                                                       re_l2_evict_valid;
  logic                                                       re_l2_evict_ready;
  logic                                                       ts_l2_evict_valid;
  logic                                                       ts_l2_evict_ready;
  logic                                                       re_ts_l2_evict_inflight;
  logic                                                       pf_l2_evict_valid;
  logic                                                       pf_l2_evict_ready;
  logic                                                       arb_l2_evict_valid;
  logic                                                       arb_l2_evict_ready;
  /* verilator lint_on UNOPTFLAT */
  logic                                                       arb_l2_miss_valid;
  logic                                                       arb_l2_miss_ready;
  /* verilator lint_off UNUSEDSIGNAL */  // `VpuEn=0` statically removes the offload request generators, so these ready channels are intentionally left unused.
  logic                                                       tx_l2_miss_valid;
  logic                                                       tx_l2_miss_ready;
  logic                                                       mh_l2_miss_valid;
  logic                                                       mh_l2_miss_ready;
  logic                                                       tl0_l2_miss_valid;
  logic                                                       tl0_l2_miss_ready;
  logic                                                       tl0_l2_miss_valid_no_coop;
  logic                                                       tl0_l2_miss_ready_no_coop;
  logic                                                       tl1_l2_miss_valid;
  logic                                                       tl1_l2_miss_ready;
  logic                                                       tl1_l2_miss_valid_no_coop;
  logic                                                       tl1_l2_miss_ready_no_coop;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                                       wb_allow_evict_bid;
  logic                                                       l2_evict_handshake;
  logic                                                       l2_miss_handshake;
  logic [$clog2(DcacheL2EvictArbPorts)-1:0]                   l2_evict_arb_winner;
  logic [$clog2(DcacheL2MissArbPorts)-1:0]                    l2_miss_arb_winner;
  logic [$clog2(DcacheL2EvictMuxPorts)-1:0]                   l2_evict_mux_select;
  logic [$clog2(DcacheL2MissMuxPorts)-1:0]                    l2_miss_mux_select;

  logic                            dcache_cfg_stall;
  logic                            dcache_cfg_clear_all;
  logic                            dcache_cfg_clear_low;
  logic                            dcache_cfg_alloc_scp;
  dcache_mode_t                    dcache_cfg_mode;
  dcache_mode_t                    dcache_core_ctrl_mode;
  logic                            s3_cfg_da_clear_en;
  logic                            s4_cfg_da_clear_val;
  logic [DcacheSetIdxWidth-1:0]    s4_cfg_da_clear_set;
  logic [DcacheWayIdxWidth-1:0]    s4_cfg_da_clear_way;
  logic                            s4_cfg_da_clear_idx;
  logic                            s4_cfg_da_clear_rdy;
  logic                            dcache_idle;
  /* verilator lint_off UNUSEDSIGNAL */  // Preserved original config helper; the current translated top does not consume the predecoded flag directly.
  logic                            dcache_cfg_sets_red;
  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_off UNUSEDSIGNAL */  // These helpers only feed the optional tensor/offload pipeline and are intentionally idle when `VpuEn=0`.
  logic                            scp_ctrl_on;

  dcache_wb_req_t [2:0]            wb_arb_data_in;
  dcache_wb_req_t                  wb_arb_data_winner;
  dcache_da_read_req_t             s0_wb_da_req;
  logic [DcacheDataSize-1:0]       s2_data;
  dcache_da_data_t                 s3_data;
  logic [2:0]                      wb_arb_grant;
  logic [2:0]                      wb_arb_bid;
  logic                            co_wb_valid;
  logic                            co_wb_ready;
  dcache_wb_req_t                  co_wb_req;
  /* verilator lint_off UNOPTFLAT */  // Writeback/miss-handler arbitration preserves the original cross-unit feedback and only appears cyclic after flattening.
  logic                            mh_wb_valid;
  /* verilator lint_on UNOPTFLAT */
  logic                            mh_wb_ready;
  dcache_wb_req_t                  mh_wb_req;
  logic                            tx_wb_valid;
  logic                            tx_wb_ready;
  dcache_wb_req_t                  tx_wb_req;
  /* verilator lint_off UNOPTFLAT */  // The writeback-data bid participates in the original arbitration fabric across registered units; Verilator reports a false combinational cycle.
  logic                            id_wb_da_bid;
  /* verilator lint_on UNOPTFLAT */
  logic                            id_wb_da_grant;
  logic                            s0_wb_da_valid;
  dcache_da_read_req_t             s1_wb_da_req;
  logic                            s1_wb_da_nack;
  logic                            wb_ready;
  logic                            wb_valid;
  logic                            wb_arb_stall;
  /* verilator lint_off UNOPTFLAT */  // Replay-queue fullness feeds the preserved issue/writeback/L2-evict arbitration cone, but the apparent cycle is cut by replay-queue occupancy flops and writeback-unit fired/active flops; Verilator only sees the flattened resource network.
  logic                            id_rq_full;
  /* verilator lint_on UNOPTFLAT */
  logic                            id_rq_half_full;
  logic                            id_rq_grant;
  logic                            id_core_pipe_available;
  logic                            id_rq_replay_bid;
  logic                            id_rq_replay_bid_qual;
  logic                            id_dbg_bid;
  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` structurally removes the APB debug window, so the preserved debug grant and helper nets are intentionally unused in stripped builds.
  logic                            id_dbg_grant;
  logic                            id_dbg_nvpu_bid;
  logic                            id_wb_da_nvpu_bid;
  logic                            id_ba_full;
  logic                            id_core_ready;
  logic                            vpu_scp_bid_1;
  logic                            vpu_scp_bid_2;
  logic                            s0_core_req_is_gsc32;
  logic                            s0_core_req_is_valid_gsc;
  /* verilator lint_off UNOPTFLAT */  // Top-level request-kill qualification flows through preserved packed request helpers and TLB flush state; Verilator flattens a false loop here.
  logic                            s0_core_req_valid_killed;
  /* verilator lint_on UNOPTFLAT */
  logic                            s0_valid_to_s1;
  logic                            s0_valid_to_s1_ordered;
  logic                            s0_req_thread_id_ordered;
  logic                            s0_md_read_en;
  logic                            s0_da_read_en;
  logic                            s0_available_to_tl;
  logic                            s0_req_addr_load;
  logic [minion_dcache_pkg::VaSize-1:0]               s0_req_addr;
  dcache_req_t                     s0_req;
  logic                            s0_bp_conf_valid;
  debug_bp_e                       s0_bp_conf_cmd;
  logic                            s0_rq_replay_valid;
  dcache_replay_req_t              s0_rq_replay;
  logic [DcacheReplayqAddrWidth-1:0] s0_rq_replay_entry;
  logic [DcacheReplayqAgeWidth-1:0]  s0_rq_replay_age;
  logic                            s0_rq_replay_uc_load_ok;
  logic                            s0_rq_replay_miss_fill_ok;
  logic                            s1_req_thread_id;
  logic                            s1_req_force_set;
  logic [minion_dcache_pkg::VaSize-1:0]               s1_req_addr;
  dcache_req_t                     s1_req;
  dcache_req_t                     s1_req_pre_addr;
  logic                            s1_md_read_en;
  logic                            s1_da_read_en;
  logic                            s1_wb_da_valid;
  logic                            s1_bp_conf_valid;
  debug_bp_e                       s1_bp_conf_cmd;
  dcache_core_bp_t                 s1_bp_conf;
  /* verilator lint_off UNOPTFLAT */  // TAG-stage exception packing feeds breakpoint/kill helpers through a packed bus; the individual exception fields do not form a real combinational loop.
  dcache_tag_xcpt                  s1_core_xcpt;
  /* verilator lint_on UNOPTFLAT */
  dcache_tag_xcpt                  s1_core_xcpt_no_kill;
  logic                            s1_has_xcpt;
  /* verilator lint_off UNOPTFLAT */  // Exception bookkeeping reuses packed request/state fields across staged helpers; the field-level graph is acyclic.
  logic                            s1_has_xcpt_no_kill;
  /* verilator lint_on UNOPTFLAT */
  logic                            core_cache_op_ready;
  logic                            core_cache_op_valid;
  logic [minion_dcache_pkg::PaSize-1:0]               s1_phys_addr;
  dcache_tlb_req_t                 s1_tlb_req_data;
  logic [minion_dcache_pkg::VaSize-1:minion_dcache_pkg::VaUntransSize]   s1_tlb_req_va;
  logic                            s1_tlb_req_valid;
  logic                            s1_is_write;
  logic                            s1_is_store;
  logic                            s1_readwrite;
  logic [NrThreads-1:0]            s1_tlb_flush_pipeline;
  logic                            s1_valid_to_tlb;
  logic                            s1_translate_only_addr_load;
  minion_dcache_pkg::vm_status_t s1_vm_status;
  dcache_pma_req_t                 s1_pma_req_data;
  dcache_pma_resp_t                s1_pma_resp_data;
  logic                            s1_pma_ts_tl_co;
  logic                            s1_pma_access_fault;
  logic                            tlb_valid_addr;
  logic                            af_skip;
  logic                            vm_enabled;
  logic [PaTagExtSize-1:0]         s1_phys_addr_tag;
  logic [DcacheSetIdxWidth-1:0]    s1_phys_addr_set;
  dcache_meta_write_req_t          s1_co_mw_req;
  dcache_da_read_req_t             s0_da_read_req;
  dcache_da_read_req_t             s1_da_read_req;
  dcache_da_read_req_t             s1_da_read_req_hit[DcacheLramNumBanks];
  dcache_da_read_req_t             s1_da_read_req_mux[DcacheLramNumBanks];
  dcache_da_data_t                 s2_da_read_resp;
  dcache_da_write_req_t            s4_da_write_data[DcacheLramNumBanks];
  s1_da_mux_sel_e                  s1_da_read_mux_sel;
  s1_da_mux_sel_e                  s1_da_read_mux_sel_prev;
  logic                            s1_da_read_en_final;
  logic                            s1_da_read_en_ovr;
  logic                            s4_da_write_en;
  logic                            s4_da_write_en_ovr;
  logic [DcacheLramNumBanks-1:0]   s1_da_load_data_en;
  logic [(DcacheDataSize/32)-1:0]  s1_vpu_scp_read_en;
  logic                            s1_vpu_en;
  logic                            s3_da_write_en;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s1_tag_match;
  logic [DcacheWayIdxWidth-1:0]    s1_read_way;
  logic [VpuLaneNum-1:0][4:0]      s1_to_s2_gsc32_indices;
  logic [4:0]                      s1_block_offset;
  logic [DcacheLramNumBanks-1:0]   s1_gather32_en;
  logic                            s1_misaligned_case_1;
  logic                            s1_misaligned_case_2;
  logic                            s1_misaligned_case_3;
  logic                            s1_misaligned_case_4;
  logic                            s1_misaligned_case_5;
  logic                            s1_misaligned_block;
  logic                            s1_misaligned;
  logic                            s1_access_is_page_split;
  logic                            s1_mf_xcpt;
  logic                            s1_req_size_128b;
  logic                            s1_req_size_256b;
  logic [6:0]                      s1_access_size;
  /* verilator lint_off UNUSEDSIGNAL */  // Lower bits are intentionally dropped once the split/align cases have been decoded.
  logic [6:0]                      s1_access_last_byte;
  /* verilator lint_on UNUSEDSIGNAL */
  dcache_req_t                     s1_req_to_s2;
  logic [minion_dcache_pkg::VaSize-1:0]               s1_req_addr_to_s2;
  logic                            s1_valid_masked;
  logic                            s1_valid_to_s2;
  logic                            s1_core_flush_pipeline;
  logic                            s1_core_flush_en;
  logic                            s0_clear_progress;
  logic                            s1_clear_progress;
  logic                            s2_clear_progress;
  logic                            s1_save_progress;
  logic                            s2_save_progress;
  logic                            s1_is_read;
  logic                            s1_is_read_no_amo;
  logic                            s1_nack;
  logic                            s1_nack_co_collide;
  logic                            s1_nack_vpu_rf_write;
  logic                            s1_is_amo;
  logic                            s1_is_msg;
  logic                            s1_is_gsc32;
  logic                            s1_pma_bus_err;
  logic                            s1_addr_clk_en;
  logic                            s1_is_bypass;
  logic                            s1_is_to_null;
  logic [DcacheLruSetSize-1:0]     s1_lru_data;
  logic [DcacheLruSetSize-1:0]     s1_lru_data_byp;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s1_hlocked_ways;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s1_slocked_ways;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s1_replace_way_en;
  dcache_meta_data_t               s1_mr_read_resp[minion_dcache_pkg::DcacheWays];
  logic [minion_dcache_pkg::DcacheWays-1:0]           s1_mr_read_valid;
  dcache_meta_write_req_t          s1_md_write_data;
  logic                            s1_md_write_en;
  logic                            s1_md_write_early_en;
  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` strips the APB debug window, so the preserved metadata debug helpers become intentionally unused in stripped builds.
  logic                            dbg_md_read_en;
  logic [DcacheSetIdxWidth+DcacheWayIdxWidth-1:0] dbg_md_read_addr;
  logic                            dbg_md_read_accept;
  logic                            dbg_md_read_accept_next;
  logic                            dbg_md_read_valid;
  dcache_meta_data_t               dbg_md_read_data;
  logic                            s0_dbg_valid;
  logic                            s1_dbg_valid;
  logic                            dbg_md_write_en_early;
  logic                            dbg_md_write_en;
  logic                            dbg_md_write_valid;
  logic [DcacheSetIdxWidth+DcacheWayIdxWidth-1:0] dbg_md_write_addr;
  dcache_meta_data_t               dbg_md_write_data;
  dcache_meta_write_req_t          dbg_md_write_req;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [minion_dcache_pkg::DcacheWays-1:0]           s2_tag_match;
  logic [minion_dcache_pkg::VaSize-1:0]               s0_co_mr_addr;
  logic                            s0_co_mr_addr_msb_err;
  logic                            s0_co_mr_phys;
  logic                            s0_co_mr_force_set;
  logic                            s0_req_force_set;
  logic                            s0_co_mr_thread_id;
  logic                            s1_co_mr_skip_pma;
  logic                            s1_co_mw_grant;
  logic [DcacheMhFileSize-1:0]     mh_probe_rdy;
  logic [DcacheMhFileSize-1:0]     mh_metaw_rdy;
  logic [NrThreads-1:0]            s2_rq_empty;
  logic                            s0_co_mr_valid;
  logic                            id_co_mr_bid;
  logic                            id_co_mr_grant;
  logic                            s1_co_mw_valid;
  logic                            s1_co_mw_valid_early;
  logic                            s1_co_nack_set_valid;
  logic [DcacheSetIdxWidth-1:0]    s1_co_nack_set_value;
  dcache_meta_data_t               s2_meta_read [minion_dcache_pkg::DcacheWays-1:0];
  logic                            s1_tlb_wait;
  logic                            s1_tlb_fail;
  logic                            id_co_pf_bid;
  logic                            id_co_pf_grant;
  logic                            s0_co_pf_valid;
  dcache_req_t                     s0_co_pf_req;
  logic                            cache_op_ready_byp;
  logic                            cache_op_valid_byp;
  core_dcache_op_req_t             cache_op_req_byp;
  logic                            mh_clear_slock_valid;
  logic [DcacheSetIdxWidth-1:0]    mh_clear_slock_set;
  logic [DcacheWayIdxWidth-1:0]    mh_clear_slock_way;
  /* verilator lint_off UNOPTFLAT */  // Replay-queue alloc-pre bookkeeping shares a status cone with conflict detection, but requests still enter from registered stage state rather than a true combinational loop.
  logic                            id_rq_alloc_req_pre;
  /* verilator lint_on UNOPTFLAT */
  logic                            s0_rq_alloc_req_val;
  logic                            id_rq_alloc_gsc;
  logic [minion_dcache_pkg::DcacheSets-1:0][minion_dcache_pkg::DcacheWays-1:0] hlock_state;
  logic [minion_dcache_pkg::DcacheSets-1:0][minion_dcache_pkg::DcacheWays-1:0] slock_state;
  logic                            s3_co_da_clear_en;
  /* verilator lint_off UNOPTFLAT */  // Data-array clear and debug write arbitration is staged, but Verilator flattens the helper nets into a spurious loop.
  logic                            s4_co_da_clear_val;
  /* verilator lint_on UNOPTFLAT */
  logic [DcacheSetIdxWidth-1:0]    s4_co_da_clear_set;
  logic [DcacheWayIdxWidth-1:0]    s4_co_da_clear_way;
  logic                            s4_co_da_clear_idx;
  logic                            s4_co_da_clear_rdy;
  /* verilator lint_off UNOPTFLAT */  // Cache-op/L2/tensor-load/miss-handler helpers below live in preserved multi-stage replay cones that Verilator flattens into false cycles.
  logic                            rq_conflict_to_co;
  minion_dcache_pkg::vm_status_t vm_status_co;
  logic [DcacheErrFlagRange:0][NrThreads-1:0] cache_ops_err_flags_l1;
  logic [minion_dcache_pkg::DcacheDbgCsrCouSize-1:0]   co_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0]  co_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] co_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0]   co_sm_data_debug_signals;
  logic                            cacheop_busy;
  logic [NrThreads-1:0][1:0]       co_l1_th_id_complete;
  logic                            core_co_l2_pref_ready;
  dcache_tlb_resp_t                s1_tlb_resp_data;
  logic                            tensorstore_busy;
  logic                            reduce_ready;
  logic                            l2_resp_valid_to_co_l2;
  logic                            core_co_l2_pref_valid;
  logic                            cacheop_l2_busy;
  logic [NrThreads-1:0][1:0]       co_l2_th_id_complete;
  logic                            s0_available_to_co_l2;
  logic                            s0_co_l2_addr_load;
  logic [minion_dcache_pkg::VaSize-1:0]               s0_co_l2_addr;
  logic                            s0_co_l2_addr_msb_err;
  logic                            s0_co_l2_thread_id;
  logic                            s1_available_to_co_l2;
  logic                            s1_co_l2_addr_load;
  logic [NrThreads-1:0]            cache_ops_err_flags_l2;
  logic [NrThreads-1:0]            bus_err_co_l2;
  logic [minion_dcache_pkg::DcacheDbgCsrCoul2Size-1:0] l2_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0]  l2_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] l2_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0]   l2_sm_data_debug_signals;
  minion_dcache_pkg::vm_status_t vm_status_l2;
  logic                            s1_clk_en;
  logic                            s1_available_to_tl;
  logic                            s3_lrsc_valid;
  logic                            l2_resp_valid_ready_int;
  logic                            tl0_working;
  logic                            tl1_working;
  logic [1:0]                      tl0_id_complete;
  /* verilator lint_off UNUSEDSIGNAL */  // The translated core control surface only exposes TL0 completion, matching the current public interface.
  logic [1:0]                      tl1_id_complete;
  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_off UNUSEDSIGNAL */  // These availability helpers only feed the optional tensor-load workers and are intentionally idle when `VpuEn=0`.
  logic                            s0_available_to_tl0;
  logic                            s1_available_to_tl0;
  logic                            s0_available_to_tl1;
  logic                            s1_available_to_tl1;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                            s0_tl0_addr_load;
  logic [minion_dcache_pkg::VaSize-1:0]               s0_tl0_addr;
  logic                            s0_tl0_addr_msb_err;
  logic                            s0_tl0_thread_id;
  logic                            s1_tl0_addr_load;
  logic                            s0_tl1_addr_load;
  logic [minion_dcache_pkg::VaSize-1:0]               s0_tl1_addr;
  logic                            s0_tl1_addr_msb_err;
  logic                            s0_tl1_thread_id;
  logic                            s1_tl1_addr_load;
  logic                            s0_tl_addr_load;
  logic                            s1_tl_addr_load;
  logic [minion_dcache_pkg::VaSize-1:0]               s0_tl_addr;
  logic                            s0_tl_addr_msb_err;
  logic                            s0_tl_thread_id;
  logic [DcacheVmSelWidth-1:0]     s0_tl_vm_sel;
  logic [minion_dcache_pkg::VaSize-1:0]               s2_req_addr;
  logic [DcacheWayIdxWidth-1:0]    s4_tl0_load_way;
  logic [DcacheLramAddrWidth-1:0]  s4_tl0_load_addr;
  logic [minion_dcache_pkg::CoreL2BlockExtSize-1:0]   s4_tl0_load_data;
  logic [DcacheLramNumBanks-1:0]   s4_tl0_load_valid;
  logic                            s3_tl0_load_valid_pre;
  logic                            s4_tl0_load_ready;
  logic                            s4_tl0_load_capture;
  /* verilator lint_off UNUSEDSIGNAL */  // Only the low set bits feed the TENB return bookkeeping; the full original address is preserved for timing/interface fidelity.
  logic [DcacheLramAddrWidth-1:0]  s4_tl1_load_addr;
  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_off UNUSEDSIGNAL */  // These helpers are only consumed by offload-side TENB/tensor bookkeeping.
  logic                            tenb_tstore_busy;
  logic                            s4_tenb_written;
  logic                            s4_tl1_tenb_ready;
  logic                            tenb_flush;
  logic                            vpu_scp_return_cycle_1;
  dcache_vpu_scp_resp              s3_vpu_scp_resp_tmp;
  minion_dcache_pkg::vm_status_t vm_status_tl0;
  minion_dcache_pkg::vm_status_t vm_status_tl1;
  logic [DcacheTlErrorBits-1:0]    tensor_load_err_flags_0;
  logic [DcacheTlErrorBits-1:0]    tensor_load_err_flags_1;
  logic                            tl0_start;
  logic                            tl0_op;
  logic                            tl1_start;
  logic                            tl1_op;
  logic [NrThreads-1:0]            bus_err_tl0;
  logic [NrThreads-1:0]            bus_err_tl1;
  logic [DcacheDbgCsrTlSize-1:0]   tl0_csr_debug_signals;
  logic [DcacheDbgCsrTlSize-1:0]   tl1_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0]  tl0_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] tl0_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0]   tl0_sm_data_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0]  tl1_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] tl1_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0]   tl1_sm_data_debug_signals;
  /* verilator lint_on UNUSEDSIGNAL */
  dcache_mh_req_t                  s2_mh_req_info;
  logic                            s2_mh_req_valid;
  logic [DcacheMhFileSize-1:0]     s2_mh_req_assigned;
  logic [DcacheMhFileSize-1:0]     s2_mh_req_accepted;
  logic [DcacheMhFileSize-1:0]     s2_mh_handler_ready;
  logic [DcacheMhFileSize-1:0]     mh_fill_bus_error;
  logic [DcacheMhFileSize-1:0]     mh_uc_load_bus_error;
  logic [NrThreads-1:0]            s2_mh_fence_rdy;
  logic [DcacheWayIdxWidth-1:0]    s4_mh_refill_way;
  logic [DcacheLramAddrWidth-1:0]  s4_mh_refill_addr;
  logic                            s2_mh_can_accept_uc;
  logic                            mh_wait_l2_to_ba_resp;
  logic [DcacheBufferIdxWidth-1:0] s3_mh_ba_wptr;
  logic                            s1_mh_force_ba_rptr_qual;
  logic [DcacheBufferIdxWidth-1:0] s1_mh_ba_rptr;
  logic                            s1_mh_ba_is_amo;
  logic                            mh_idle;
  logic [NrThreads-1:0]            bus_err_mh;
  logic [minion_dcache_pkg::PaSize-1:0]               bus_err_addr_mh;
  logic                            s1_mh_mw_grant;
  logic                            s1_mh_mw_valid_early;
  logic                            s1_mh_mw_valid;
  logic                            s1_mh_mw_clear_slock;
  dcache_meta_write_req_t          s1_mh_mw_req;
  logic                            s2_mh_match_miss_line;
  /* verilator lint_on UNOPTFLAT */
  logic [DcacheMhFileSize*minion_dcache_pkg::DcacheDbgCsrMhSize-1:0] mh_csr_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0] mh_sm_match_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] mh_sm_filter_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]   mh_sm_data_debug_signals;
  /* verilator lint_off UNOPTFLAT */  // Buffer-array/replay coordination spans several preserved stage helpers; the actual cycle is broken by flops inside the leaf blocks.
  logic [DcacheBufferIdxWidth-1:0] s1_re_ba_read_addr;
  logic [DcacheBufferIdxWidth-1:0] s2_ba_write_entry;
  logic [DcacheBufferIdxWidth-1:0] s3_re_ba_write_addr;
  logic [DcacheBufferIdxWidth-1:0] s1_ba_rptr;
  logic                            s1_reduce_send_inst;
  logic                            s1_reduce_op_inst;
  logic                            s1_re_ba_read_en;
  logic                            s1_re_ba_read_en_spec;
  logic                            s1_mh_force_ba_rptr;
  logic                            s1_ba_dealloc_uc;
  logic                            s1_valid;
  logic                            s2_valid;
  logic                            s2_reduce_send_inst;
  logic                            s2_reduce_op_inst;
  logic                            s2_re_ba_write_needed;
  logic                            s3_reduce_send_inst;
  logic                            s3_reduce_op_inst;
  logic                            s3_valid_to_s4;
  logic                            s3_to_s4_en;
  dcache_req_t                     s3_req_to_s4;
  logic                            s3_sc_fail;
  logic [minion_dcache_pkg::PaSize-7:0]               s3_lrsc_addr;
  logic [LrscCountWidth-1:0]       s3_lrsc_count;
  logic [LrscCountWidth-1:0]       s3_lrsc_count_next;
  logic                            s3_lr;
  logic                            s3_sc;
  logic                            s3_lrsc_addr_match;
  logic [DcacheDataSize-1:0]       s3_load_data_mux2;
  logic [DcacheDataSize-1:0]       s3_data_gather32;
  logic                            s3_is_gather32;
  logic [VpuLaneNum-1:0][31:0]     s3_data_word;
  logic [VpuLaneNum-1:0][15:0]     s3_data_half;
  logic [VpuLaneNum-1:0][7:0]      s3_data_byte;
  logic                            re_pipe_used;
  logic                            re_ba_alloc;
  logic                            re_ba_dealloc;
  /* verilator lint_on UNOPTFLAT */
  logic                            re_buffer_avail;
  /* verilator lint_off UNUSEDSIGNAL */  // These reduce/texsend helpers are only consumed by the optional offload-side pipeline.
  logic                            re_ba_full;
  logic                            re_wait_l2_to_ba_resp;
  logic                            re_ba_entries_required;
  logic [DcacheDataSize-1:0]       s1_ba_data;
  logic                            ts_scp_read;
  logic [DcacheWayIdxWidth-1:0]    ts_scp_read_way;
  logic [DcacheLramAddrWidth-1:0]  ts_scp_read_addr;
  logic                            ts_col_size_is_128b;
  logic                            s0_available_to_ts;
  logic                            s0_ts_addr_load;
  logic [minion_dcache_pkg::VaSize-1:0]               s0_ts_addr;
  logic                            s0_ts_addr_msb_err;
  logic                            s0_ts_thread_id;
  logic                            s1_available_to_ts;
  logic                            s1_ts_addr_load;
  logic                            mh_going_to_uc_store;
  logic                            reduce_start;
  logic                            tensor_store_start;
  minion_dcache_pkg::vm_status_t vm_status_ts;
  dcache_vpu_reduce_ctrl_t         vpu_reduce_ctrl;
  logic [DcacheErrFlagRange:0][NrThreads-1:0] tensor_reduce_err_all_flags;
  logic [NrThreads-1:0]            bus_err_re;
  logic [minion_dcache_pkg::DcacheDbgCsrTsSize-1:0]   ts_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0] ts_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] ts_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0]  ts_sm_data_debug_signals;
  logic                            ts_start;
  logic                            ts_op;
  logic                            tx_idle;
  logic                            tex_send_valid;
  logic                            tex_send_ready;
  logic [minion_dcache_pkg::DcacheDbgCsrTxsndSize-1:0] tx_csr_debug_signals;
  /* verilator lint_on UNUSEDSIGNAL */
  dcache_core_ctrl_t               core_ctrl_resp;
  logic [NrThreads-1:0]            cache_op_busy_changed;
  logic [NrThreads-1:0]            cache_op_busy_changed_z1;
  logic [NrThreads-1:0]            tensor_reduce_mem_err_flag;
  logic [NrThreads-1:0]            tensor_load_mem_err_flags;
  logic [DcacheErrFlagRange:0][NrThreads-1:0] cache_ops_err_flags;
  logic                            cacheop_busy_prev;
  logic                            cacheop_l2_busy_prev;
  logic                            dcache_cfg_stall_prev;
  logic                            l2_resp_valid_int;
  logic                            l2_resp_valid_prev_int;
  // The original dcache enables the "full perf" L2 response pipe, which is a
  // 2-entry FIFO in front of the internal response consumers.
  logic [1:0]                      l2_resp_fifo_count_q;
  logic [1:0]                      l2_resp_fifo_count_d;
  logic                            l2_resp_fifo_push;
  logic                            l2_resp_fifo_pop;
  logic                            l2_resp_fifo_full;
  logic                            l2_resp_fifo_rd_ptr_q;
  logic                            l2_resp_fifo_rd_ptr_d;
  logic                            l2_resp_fifo_wr_ptr_q;
  logic                            l2_resp_fifo_wr_ptr_d;
  et_link_minion_rsp_info_t        l2_resp_fifo_data[2];
  logic                            l2_resp_ready_int;
  et_link_minion_rsp_info_t        l2_resp_int;
  logic                            l2_resp_is_port_data;
  logic                            l2_resp_is_for_tensor_0;
  /* verilator lint_off UNOPTFLAT */  // The scalar-only/APB-debug elaboration keeps this original response-classification helper in the same preserved fill/ready cone; Verilator flattens the staged logic and invents a loop.
  logic                            l2_resp_is_for_tensor_1;
  /* verilator lint_on UNOPTFLAT */
  logic                            l2_resp_is_for_mh_fill;
  logic                            l2_resp_is_for_mh_uc;
  logic                            l2_resp_is_for_mh_uc_ok;
  logic                            l2_resp_is_for_reduce;
  logic                            l2_resp_is_reduce_data;
  logic                            l2_resp_is_reduce_ready;
  logic                            l2_fill_ready;
  logic                            l2_da_write;
  /* verilator lint_off UNOPTFLAT */  // L2-response blocking is derived from staged tensor-load/fill state; Verilator flattens the helpers into a false cycle on this ready gate.
  logic                            l2_resp_ready_blocked;
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_off UNOPTFLAT */  // This helper remains in the same preserved fill/ready cone as the tensor-1 classification above and only appears cyclic after Verilator flattening.
  logic                            l2_resp_ready_block_op;
  /* verilator lint_on UNOPTFLAT */
  logic [1:0]                      l2_resp_count_q;
  logic [1:0]                      l2_resp_count_d;
  logic [1:0]                      l2_resp_count_limit;
  logic                            l2_resp_count_en;
  logic [DcacheMhFileSize-1:0]     mh_l2_resp_stall;
  logic                            l2_fill_slot_avail;
  logic                            s3_ba_write_en;
  logic                            s3_ba_write_en_ext;
  logic                            s3_ba_write_en_uc;
  logic                            s3_ba_write_en_msg;
  logic [DcacheBufferIdxWidth-1:0] s3_ba_write_addr_ext;
  logic [DcacheDataSize-1:0]       s3_ba_write_data;
  logic [DcacheDataSize-1:0]       s3_ba_write_data_ext;
  logic                            s2_ba_write_needed_core;
  logic                            s2_ba_write_needed;
  logic                            s2_ba_write_en;
  logic                            s2_ba_replay;
  /* verilator lint_off UNOPTFLAT */  // Buffer-array dealloc feeds the translated original replay/BA bookkeeping; Verilator flattens the path through buffer_array/miss-handler and reports a false loop.
  logic                            s2_ba_dealloc;
  /* verilator lint_on UNOPTFLAT */
  logic                            s2_ba_write_en_ext_prev;
  logic                            s4_tl1_tenb_ready_reg;
  logic                            s4_valid;
  dcache_req_t                     s4_req;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s4_way;
  logic                            s4_misaligned_case_3;
  /* verilator lint_off UNOPTFLAT */  // Debug data-array writes arbitrate against the original staged S4 write helpers; Verilator flattens the helper cone and reports a false loop on this pre-debug gate.
  logic                            s4_da_write_en_but_dbg;
  /* verilator lint_on UNOPTFLAT */
  logic [DcacheLramNumBanks-1:0][DcacheLramAddrWidth-1:0] s4_req_da_write_addr;
  logic [DcacheLramNumBanks-1:0][DcacheBlockAddrWidth-1:0] s2_block_addr;
  logic [DcacheLramNumBanks-1:0][DcacheBlockAddrWidth-1:0] s3_block_addr;
  logic [DcacheLramNumBanks-1:0][DcacheBlockAddrWidth-1:0] s4_block_addr;
  logic [DcacheLramNumBanks-1:0][DcacheBlockAddrWidth-1:0] s5_block_addr;
  logic                            s4_clk_en;
  /* verilator lint_off UNUSEDSIGNAL */  // S5 store-bypass only consumes the original address/data/chunk subset; the full request is preserved to match the source timing.
  dcache_req_t                     s5_req;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                            s5_valid;
  logic                            s5_cl_addr_match_s3_s2;
  logic                            s5_cl_addr_match_s4_s2;
  logic                            s5_cl_addr_match_s5_s2;
  logic                            dbg_da_write_en_early;
  /* verilator lint_off UNOPTFLAT */  // APB debug writes share the S4 write arbiter; Verilator's flattening loses the stage boundary and invents a loop.
  logic                            dbg_da_write_en;
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` structurally removes the APB debug window, so the preserved data-array debug helpers are intentionally unused in stripped builds.
  logic                            dbg_da_read_en;
  logic [DcacheDbgAddrBits-2:0]    dbg_da_read_addr;
  logic                            dbg_da_read_valid;
  logic                            dbg_da_read_valid_next;
  logic                            dbg_da_read_accept;
  logic                            dbg_da_read_accept_next;
  logic [BpamShireApbDataWidth-1:0] dbg_da_read_data;
  logic [DcacheDbgAddrBits-2:0]    dbg_da_write_addr;
  logic                            dbg_da_write_valid;
  logic                            dbg_da_write_valid_next;
  logic [DcacheLramDataSize-1:0]   dbg_da_write_data;
  logic [DcacheDbgAddrBits-2:0]    dbg_apb_paddr;
  logic                            dbg_block_sel_md;
  logic                            dbg_block_sel_da;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                            dcache_idle_next;
  logic                            dcache_idle_excl_mode_next;
  logic [minion_dcache_pkg::PaSize-1:0]               bus_err_addr_next;
  logic [NrThreads-1:0]            bus_err_next;
  logic [NrThreads-1:0]            s1_pma_bus_err_per_thread;
  logic [CsrNrEventsDcache-1:0]    io_events_next;
  logic                            io_events_en;
  dcache_req_t                     s2_req;
  dcache_req_t                     s2_req_pre_addr;
  dcache_replay_conf_t             s2_rq_conf_type;
  logic                            s2_nack;
  logic                            s2_nack_misaligned;
  // Packed s2_req field dependencies across the translated
  // S2 combinational blocks and reports a false loop on the nack signals.
  /* verilator lint_off UNOPTFLAT */
  logic                            s2_nack_pre_misaligned;
  logic                            s2_nack_pre_misaligned_miss;
  logic                            s2_nack_inflight;
  logic                            s2_nack_miss;
  logic                            s2_nack_rq_conflict;
  logic                            s2_valid_to_s3;
  logic                            s2_has_data;
  logic                            s2_to_s3_en;
  logic                            s2_to_s2_req_en;
  logic                            s2_to_s3_en_store_data;
  logic                            s2_to_s3_en_store_byp;
  logic                            s2_rq_push_cancel;
  logic                            s2_store_misaligned_rearm;
  logic                            s2_ba_nack_needed;
  logic                            s2_uc_load_1st;
  logic                            s2_uc_load_ok;
  logic                            s2_miss_fill_ok;
  logic                            s2_rq_push;
  logic                            s1_rq_push_conservative;
  logic                            s2_rq_renack;
  logic                            s1_rq_renack_conservative;
  logic                            s2_rq_dealloc;
  logic                            rq_uc_load_fill;
  logic [DcacheBufferIdxWidth-1:0] rq_uc_load_fill_id;
  logic                            rq_uc_load_fill_ok;
  logic [DcacheLruSetSize-1:0]     s2_lru_data;
  logic [DcacheLruSetSize-1:0]     s2_lru_data_up;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s2_replace_way_en;
  logic                            s2_reg_nack;
  logic                            s2_nack_vpu_rf_write;
  logic                            s2_misaligned;
  logic                            s2_misaligned_case_3;
  logic                            s2_is_read;
  logic                            s2_is_write;
  logic                            s2_is_msg;
  logic                            s2_is_bypass;
  logic                            s2_is_amo;
  logic                            s2_valid_qual;
  logic                            s2_valid_masked;
  logic                            s2_has_xcpt_addr;
  logic [DcacheLramNumBanks-1:0]   s2_to_s3_en_load_data;
  logic [DcacheLramNumBanks-1:0]   s2_da_load_data_en;
  logic [DcacheDataSize-1:0]       s2_to_s3_store_data;
  logic [VpuLaneNum-1:0][4:0]      s2_gsc32_indices;
  logic [(DcacheDataSize/32)-1:0]  s2_vpu_scp_read_en;
  logic                            s2_wb_da_valid;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s2_tag_match_qual;
  logic                            s2_tag_match_any;
  logic                            s2_state_hit;
  logic                            s2_hit;
  dcache_line_state_e              s2_way_hit_state;
  dcache_meta_data_t               s2_repl_meta;
  logic                            s2_lru_update;
  logic                            s2_mh_req_valid_reg;
  logic                            s2_mh_req_ready;
  /* verilator lint_on UNOPTFLAT */
  dcache_req_t                     s3_req;
  logic [DcacheDataSize-1:0]       s3_store_data;
  logic [DcacheDataSize-1:0]       s3_store_amo_ext_data;
  logic [DcacheDataSize-1:0]       s3_store_bypass_data;
  logic [DcacheDataSize-1:0]       s5_store_bypass_data;
  logic [minion_dcache_pkg::DcacheWays-1:0]           s3_way;
  logic [(DcacheDataSize/64)-1:0]  s3_store_bypass;
  logic [(DcacheDataSize/64)-1:0]  s5_store_bypass;
  logic                            s3_valid;
  logic                            s3_has_data;
  logic                            s3_is_write;
  logic                            s3_misaligned_case_3;
  logic [VpuLaneNum-1:0][4:0]      s3_gsc32_indices;
  logic [DcacheDataSize-1:0]       s3_data_bypassed;
  logic [DcacheDataSize-1:0]       s3_data_aligned;
  /* verilator lint_off UNUSEDSIGNAL */  // The wide duplicate line is shifted first and only the lower aligned half is consumed.
  logic [(2*DcacheDataSize)-1:0]   s3_data_aligned_wide;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [DcacheDataSize-1:0]       s3_data_merged;
  logic [DcacheDataSize-1:0]       s3_load_data_mux1;
  logic [(DcacheDataSize/8)-1:0]   s3_data_merge_sel;
  logic [minion_dcache_pkg::XregSize-1:0]             s3_data_extended;
  logic [DcacheDataSize-1:0]       s3_store_merge_out;
  logic                            s3_store_merge_enable_lo;
  dcache_da_data_t                 s3_scatter32_data;

  assign dcache_core_ctrl_mode = dcache_mode_t'(core_ctrl_i.cache_ctrl[1:0]);
  assign scp_ctrl_on = (dcache_cfg_mode == DcacheModeScratchpad);
  assign dcache_cfg_sets_red = (dcache_cfg_mode != DcacheModeShared);
  assign core_cache_op_valid = core_ctrl_i.cache_op_valid && !dcache_cfg_stall;
  assign l2_resp_fifo_full = (l2_resp_fifo_count_q == 2'd2);
  assign l2_resp_fifo_push = l2_resp_valid_i && !l2_resp_fifo_full;
  assign l2_resp_fifo_pop = l2_resp_valid_int && l2_resp_ready_int;
  assign l2_resp_valid_int = (l2_resp_fifo_count_q != 2'b0);
  assign l2_resp_int = l2_resp_fifo_data[l2_resp_fifo_rd_ptr_q];
  assign l2_resp_valid_prev_int = (l2_resp_fifo_count_d != 2'b0);
  assign l2_resp_valid_ready_int = l2_resp_valid_int && l2_resp_ready_int;
  assign l2_resp_valid_to_co_l2 = l2_resp_valid_int && !s3_lrsc_valid;
  assign core_co_l2_pref_valid = core_ctrl_i.cache_op_valid && !dcache_cfg_stall;
  assign reduce_start = VpuEn && core_ctrl_i.reduce_start && !dcache_cfg_stall;
  assign tensor_store_start = VpuEn && core_ctrl_i.tensorstore_start && !dcache_cfg_stall;
  assign tex_send_valid = VpuEn && core_ctrl_i.tex_send_valid && !dcache_cfg_stall;
  assign s4_tl0_load_ready = l2_fill_slot_avail && !l2_resp_valid_int;

  always_comb begin
    l2_resp_fifo_count_d = l2_resp_fifo_count_q;
    unique case ({l2_resp_fifo_push, l2_resp_fifo_pop})
      2'b10: l2_resp_fifo_count_d = l2_resp_fifo_count_q + 2'd1;
      2'b01: l2_resp_fifo_count_d = l2_resp_fifo_count_q - 2'd1;
      default: begin
      end
    endcase
  end

  always_comb begin
    l2_resp_fifo_rd_ptr_d = l2_resp_fifo_rd_ptr_q;
    if (l2_resp_fifo_pop) begin
      l2_resp_fifo_rd_ptr_d = ~l2_resp_fifo_rd_ptr_q;
    end
  end

  always_comb begin
    l2_resp_fifo_wr_ptr_d = l2_resp_fifo_wr_ptr_q;
    if (l2_resp_fifo_push) begin
      l2_resp_fifo_wr_ptr_d = ~l2_resp_fifo_wr_ptr_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      l2_resp_fifo_count_q <= '0;
      l2_resp_fifo_rd_ptr_q <= 1'b0;
      l2_resp_fifo_wr_ptr_q <= 1'b0;
    end else begin
      l2_resp_fifo_count_q <= l2_resp_fifo_count_d;
      l2_resp_fifo_rd_ptr_q <= l2_resp_fifo_rd_ptr_d;
      l2_resp_fifo_wr_ptr_q <= l2_resp_fifo_wr_ptr_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (l2_resp_fifo_push) begin
      l2_resp_fifo_data[l2_resp_fifo_wr_ptr_q] <= l2_resp_i;
    end
  end

  assign s0_core_req_is_gsc32 = minion_pkg::dcache_cmd_is_gsc32(s0_core_req_i.cmd);
  assign s0_core_req_is_valid_gsc =
      s0_core_req_valid_i
      && minion_dcache_pkg::dcache_type_is_gsc(minion_dcache_pkg::dcache_type_e'(s0_core_req_i.typ))
      && !s0_core_req_is_gsc32;
  assign s0_core_req_valid_killed =
      (s0_core_req_is_valid_gsc && !s0_core_req_i.ps_mask[s0_core_req_i.gsc_cnt])
      || (s0_core_req_valid_i
          && minion_dcache_pkg::dcache_type_is_ps(minion_dcache_pkg::dcache_type_e'(s0_core_req_i.typ))
          && (s0_core_req_i.ps_mask == '0))
      || s1_tlb_flush_pipeline[s0_core_req_i.dest.thread_id];
  assign s0_valid_to_s1 =
      (s0_core_req_valid_i && !s0_core_req_valid_killed) || s0_rq_replay_valid || s0_co_pf_valid;
  assign s0_valid_to_s1_ordered = s0_core_req_valid_i || s0_rq_replay_valid;
  assign s0_req_thread_id_ordered =
      s0_core_req_valid_i ? s0_core_req_i.dest.thread_id : s0_rq_replay.dest.thread_id;
  assign s0_md_read_en =
      s0_co_mr_valid || s0_rq_replay_valid || s0_core_req_valid_i || s0_co_pf_valid
      || (DebugApbEn && s0_dbg_valid);
  assign s0_available_to_tl = !(s0_valid_to_s1 || s0_md_read_en);
  assign s1_available_to_tl = !(s1_clk_en || s1_ts_addr_load || s1_co_l2_addr_load);
  assign s0_available_to_ts = !(s0_valid_to_s1 || s0_md_read_en || s0_tl_addr_load);
  assign s1_available_to_ts = !(s1_clk_en || s1_tl_addr_load || s1_co_l2_addr_load);
  assign s0_available_to_co_l2 = !(s0_valid_to_s1 || s0_md_read_en || s0_tl_addr_load || s0_ts_addr_load);
  assign s1_available_to_co_l2 = !(s1_clk_en || s1_tl_addr_load || s1_ts_addr_load);
  assign s0_available_to_tl0 = s0_available_to_tl;
  assign s0_available_to_tl1 = s0_available_to_tl && !s0_tl0_addr_load;
  assign s1_available_to_tl0 = s1_available_to_tl && !s1_tl1_addr_load;
  assign s1_available_to_tl1 = s1_available_to_tl && !s1_tl0_addr_load;
  assign s0_tl_addr_load = s0_tl0_addr_load || s0_tl1_addr_load;
  assign s1_tl_addr_load = s1_tl0_addr_load || s1_tl1_addr_load;
  assign s0_tl_addr = s0_tl0_addr_load ? s0_tl0_addr : s0_tl1_addr;
  assign s0_tl_addr_msb_err = s0_tl0_addr_load ? s0_tl0_addr_msb_err : s0_tl1_addr_msb_err;
  assign s0_tl_thread_id = s0_tl0_addr_load ? s0_tl0_thread_id : s0_tl1_thread_id;
  assign s0_tl_vm_sel = s0_tl0_addr_load ? DcacheVmSelTl0 : DcacheVmSelTl1;
  assign tenb_tstore_busy = VpuEn && (tensorstore_busy || !reduce_ready);
  assign s0_req_addr_load =
      s0_valid_to_s1 || s0_md_read_en || s0_tl_addr_load || s0_ts_addr_load || s0_co_l2_addr_load;

  always_comb begin
    s0_da_read_en = s0_valid_to_s1;

    if (s0_rq_replay_valid) begin
      s0_da_read_en &= !minion_dcache_pkg::dcache_cmd_is_prefetch(s0_rq_replay.cmd);
    end else begin
      s0_da_read_en &=
          !minion_dcache_pkg::dcache_cmd_is_prefetch(
              minion_dcache_pkg::dcache_cmd_e'(s0_core_req_i.cmd));
    end
  end

  always_comb begin
    s0_da_read_req = '0;

    if (s0_rq_replay_valid) begin
      s0_da_read_req.addr = s0_rq_replay.addr[DcacheLramAddrWidth-1:0];
    end else begin
      s0_da_read_req.addr = s0_core_req_i.addr[DcacheLramAddrWidth-1:0];
    end

    s0_da_read_req.addr[DcacheLramAddrWidth-1:6] =
        dcache_set_correct(s0_da_read_req.addr[DcacheLramAddrWidth-1:6], dcache_cfg_mode,
                           s0_req.dest.thread_id);
  end

  always_comb begin
    s0_req = '0;
    s0_req_addr = s0_rq_replay.addr[minion_dcache_pkg::VaSize-1:0];
    s0_req.addr_msb_err = 1'b0;
    s0_req.phys = s0_rq_replay.phys;
    s0_req.dest = s0_rq_replay.dest;
    s0_req.cmd = s0_rq_replay.cmd;
    s0_req.typ = s0_rq_replay.typ;
    s0_req.gsc_cnt = s0_rq_replay.gsc_cnt;
    s0_req.gsc32_idx = s0_rq_replay.gsc32_idx;
    s0_req.ps_mask = s0_rq_replay.ps_mask;
    s0_req.replay = 1'b1;
    s0_req.replay_entry = s0_rq_replay_entry;
    s0_req.replay_age = s0_rq_replay_age;
    s0_req.cache_op = s0_rq_replay.cache_op;
    s0_req.misaligned = s0_rq_replay.misaligned;
    s0_req.misaligned_2nd = s0_rq_replay.misaligned_2nd;
    s0_req.buf_alloc = s0_rq_replay.buf_alloc;
    s0_req.buf_id = s0_rq_replay.buf_id;
    s0_req.cacheable = s0_rq_replay.cacheable;
    s0_req.uc_load_2nd = s0_rq_replay.uc_load_2nd;
    s0_req.uc_load_ok = s0_rq_replay_uc_load_ok;
    s0_req.miss_fill_ok = s0_rq_replay_miss_fill_ok;
    s0_req.vm_sel = s0_rq_replay.vm_sel;
    s0_req.chunk_read = '0;
    s0_req.mem_global = s0_rq_replay.mem_global;
    s0_req_force_set = 1'b0;
    s0_bp_conf_valid = 1'b0;
    s0_bp_conf_cmd = s1_bp_conf.cmd;

    if (s0_core_req_valid_i) begin
      s0_req_addr = s0_core_req_i.addr;
      s0_req.addr_msb_err = s0_core_req_i.addr_msb_err;
      s0_req.phys = s0_core_req_i.phys;
      s0_req.dest = s0_core_req_i.dest;
      s0_req.cmd = minion_dcache_pkg::dcache_cmd_e'(s0_core_req_i.cmd);
      s0_req.typ = minion_dcache_pkg::dcache_type_e'(s0_core_req_i.typ);
      s0_req.gsc_cnt = s0_core_req_i.gsc_cnt;
      s0_req.ps_mask = s0_core_req_i.ps_mask;
      s0_req.gsc32_idx = s0_core_req_i.gsc32_idx;
      s0_req.replay = 1'b0;
      s0_req.replay_entry = '0;
      s0_req.replay_age = '0;
      s0_req.cache_op = 1'b0;
      s0_req.misaligned = 1'b0;
      s0_req.misaligned_2nd = 1'b0;
      s0_req.buf_alloc = 1'b0;
      s0_req.buf_id = '0;
      s0_req.cacheable = 1'b1;
      s0_req.uc_load_2nd = 1'b0;
      s0_req.uc_load_ok = 1'b0;
      s0_req.miss_fill_ok = 1'b0;
      s0_req.vm_sel = DcacheVmSelIp;
      s0_req.mem_global = s0_core_req_i.mem_global;
      s0_bp_conf_valid = !s0_core_req_valid_killed;
      s0_bp_conf_cmd =
          minion_dcache_pkg::dcache_cmd_is_amo(minion_dcache_pkg::dcache_cmd_e'(s0_core_req_i.cmd))
              ? DbgBpAmo
              : minion_dcache_pkg::dcache_cmd_is_read(minion_dcache_pkg::dcache_cmd_e'(s0_core_req_i.cmd))
                  ? DbgBpLoad
                  : DbgBpStore;
    end else if (s0_co_mr_valid) begin
      s0_req_addr = s0_co_mr_addr;
      s0_req.addr_msb_err = s0_co_mr_addr_msb_err;
      s0_req.set = s0_co_mr_addr[DcacheLramAddrWidth-1:6];
      s0_req.cmd = minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdXrd);
      s0_req.phys = s0_co_mr_phys;
      s0_req.replay = 1'b0;
      s0_req.vm_sel = DcacheVmSelCo;
      s0_req.dest.thread_id = s0_co_mr_thread_id;
      s0_req_force_set = s0_co_mr_force_set;
      s0_bp_conf_valid = 1'b0;
      s0_bp_conf_cmd = DbgBpCo;
    end else if (DebugApbEn && s0_dbg_valid) begin
      s0_req.set = dbg_md_read_addr[DcacheSetIdxWidth-1:0];
      s0_req.phys = 1'b1;
      s0_req.replay = 1'b0;
      s0_req_force_set = 1'b1;
      s0_bp_conf_valid = 1'b0;
    end else if (s0_co_pf_valid) begin
      s0_req_addr = s0_co_pf_req.addr[minion_dcache_pkg::VaSize-1:0];
      s0_req = s0_co_pf_req;
      s0_bp_conf_valid = 1'b1;
      s0_bp_conf_cmd = DbgBpCo;
    end else if (s0_tl_addr_load) begin
      s0_req_addr = s0_tl_addr;
      s0_req.addr_msb_err = s0_tl_addr_msb_err;
      s0_req.vm_sel = s0_tl_vm_sel;
      s0_req.dest = '{fp: 1'b0, addr: '0, thread_id: s0_tl_thread_id};
      s0_req.phys = 1'b0;
      s0_req.replay = 1'b0;
      s0_req.cmd = minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdXrd);
      s0_req.typ = minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypeB);
      s0_bp_conf_valid = 1'b1;
      s0_bp_conf_cmd = DbgBpTensorLoad;
    end else if (s0_ts_addr_load) begin
      s0_req_addr = s0_ts_addr;
      s0_req.addr_msb_err = s0_ts_addr_msb_err;
      s0_req.vm_sel = DcacheVmSelTs;
      s0_req.dest = '{fp: 1'b0, addr: '0, thread_id: s0_ts_thread_id};
      s0_req.phys = 1'b0;
      s0_req.replay = 1'b0;
      s0_req.cmd = minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdXrd);
      s0_req.typ = minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypeB);
      s0_bp_conf_valid = 1'b1;
      s0_bp_conf_cmd = DbgBpTensorStore;
    end else if (s0_co_l2_addr_load) begin
      s0_req_addr = s0_co_l2_addr;
      s0_req.addr_msb_err = s0_co_l2_addr_msb_err;
      s0_req.vm_sel = DcacheVmSelL2;
      s0_req.dest = '{fp: 1'b0, addr: '0, thread_id: s0_co_l2_thread_id};
      s0_req.phys = 1'b0;
      s0_req.replay = 1'b0;
      s0_req.cmd = minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdXrd);
      s0_req.typ = minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypeB);
      s0_bp_conf_valid = 1'b1;
      s0_bp_conf_cmd = DbgBpCo;
    end

    if (!s0_req_force_set) begin
      s0_req.set = dcache_set_correct(s0_req_addr[DcacheLramAddrWidth-1:6], dcache_cfg_mode,
                                      s0_req.dest.thread_id);
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s1_valid <= 1'b0;
      s1_bp_conf_valid <= 1'b0;
    end else begin
      s1_valid <= s0_valid_to_s1;
      s1_bp_conf_valid <= s0_bp_conf_valid;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s0_valid_to_s1 || s0_md_read_en) begin
      s1_req_pre_addr <= s0_req;
      s1_req_force_set <= s0_req_force_set;
    end
    if (s0_req_addr_load) begin
      s1_req_thread_id <= s0_req.dest.thread_id;
      s1_req_addr <= s0_req_addr;
    end
    s1_md_read_en <= s0_md_read_en;
    s1_da_read_en <= s0_da_read_en;
    if (s0_md_read_en || s0_da_read_en) begin
      s1_da_read_req <= s0_da_read_req;
    end
    s1_wb_da_valid <= s0_wb_da_valid;
    if (s0_wb_da_valid) begin
      s1_wb_da_req <= s0_wb_da_req;
    end
    s1_bp_conf_cmd <= s0_bp_conf_cmd;
  end

  always_comb begin
    s1_req = s1_req_pre_addr;
    s1_req.addr = {1'b0, s1_req_addr};
    s1_req.dest.thread_id = s1_req_thread_id;

    s1_bp_conf = '0;
    s1_bp_conf.address[minion_dcache_pkg::VaSize-1:0] = s1_req_addr;
    s1_bp_conf.address[minion_pkg::VaSizeExt-1] = s1_req.addr_msb_err ^ s1_req_addr[minion_dcache_pkg::VaSize-1];
    s1_bp_conf.thread_id = s1_req_thread_id;
    s1_bp_conf.cmd = s1_bp_conf_cmd;
  end

  always_comb begin
    s1_md_write_early_en =
        s1_co_mw_valid_early || s1_mh_mw_valid_early || (DebugApbEn && dbg_md_write_en_early);
    s1_md_write_en = s1_co_mw_valid || s1_mh_mw_valid || (DebugApbEn && dbg_md_write_en);
    s1_co_mw_grant = 1'b1;
    s1_mh_mw_grant = !s1_co_mw_valid;
    dbg_md_write_valid = DebugApbEn && !s1_co_mw_valid && !s1_mh_mw_valid;

    dbg_md_write_req = '0;
    dbg_md_write_req.set_idx = dbg_md_write_addr[DcacheSetIdxWidth-1:0];
    dbg_md_write_req.way_en =
        dcache_way_decode(dbg_md_write_addr[DcacheSetIdxWidth +: DcacheWayIdxWidth]);
    dbg_md_write_req.data = dbg_md_write_data;

    s1_md_write_data = s1_co_mw_valid ? s1_co_mw_req
                     : s1_mh_mw_valid ? s1_mh_mw_req
                     :                  dbg_md_write_req;
  end

  minion_dcache_metadata_array u_metadata_array (
    .clk_i                (clk_i),
    .rst_ni               (rst_ni),
    .s1_read_set_i        (s1_req.set),
    .s1_write_valid_i     (s1_md_write_en),
    .s1_write_valid_prev_i(s1_md_write_early_en),
    .s1_write_req_i       (s1_md_write_data),
    .s1_read_resp_valid_o (s1_mr_read_valid),
    .s1_read_resp_o       (s1_mr_read_resp),
    .cfg_clear_all_i      (dcache_cfg_clear_all),
    .cfg_clear_low_i      (dcache_cfg_clear_low)
  );

  assign s1_vm_status = (s1_req.vm_sel == DcacheVmSelCo)  ? vm_status_co
                      : (s1_req.vm_sel == DcacheVmSelTl0) ? vm_status_tl0
                      : (s1_req.vm_sel == DcacheVmSelTl1) ? vm_status_tl1
                      : (s1_req.vm_sel == DcacheVmSelTs)  ? vm_status_ts
                      : (s1_req.vm_sel == DcacheVmSelL2)  ? vm_status_l2
                      : (s1_req.dest.thread_id == 1'b1)   ? vm_status_i[1]
                      :                                    vm_status_i[0];

  assign s1_translate_only_addr_load = s1_tl_addr_load || s1_ts_addr_load || s1_co_l2_addr_load;
  assign s1_is_write = minion_dcache_pkg::dcache_cmd_is_write(s1_req.cmd) && !s1_req.uc_load_2nd
                    && !s1_translate_only_addr_load && !s1_reduce_op_inst;
  assign s1_is_store =
      (minion_dcache_pkg::dcache_cmd_is_write(s1_req.cmd) && !s1_req.uc_load_2nd) || s1_ts_addr_load;
  assign s1_readwrite = minion_dcache_pkg::dcache_cmd_is_read(s1_req.cmd)
                     || s1_is_write
                     || minion_dcache_pkg::dcache_cmd_is_prefetch(s1_req.cmd);
  assign s1_valid_to_tlb = s1_valid
                        && (!s1_core_kill_i || s1_req.replay || s1_req.cache_op);
  assign s1_tlb_req_va = s1_req.addr[minion_dcache_pkg::VaSize-1:minion_dcache_pkg::VaUntransSize];

  always_comb begin
    s1_tlb_req_data = '0;
    s1_tlb_req_data.status = s1_vm_status;
    s1_tlb_req_data.vpn = s1_tlb_req_va;
    s1_tlb_req_data.passthrough = s1_req.phys;
    s1_tlb_req_data.store = s1_is_store;
    s1_tlb_req_data.msb_err = s1_req.addr_msb_err;
  end

  assign s1_tlb_req_valid = (s1_valid_to_tlb && s1_readwrite) || s1_translate_only_addr_load;

  minion_dcache_tlb_array u_tlb_array (
    .clk_i             (clk_i),
    .rst_ni            (rst_ni),
    .dft_i             (dft_pkg::dft_t'('0)),
    .esr_vmspagesize_i (vmspagesize_i),
    .req_data_i        (s1_tlb_req_data),
    .req_valid_i       (s1_tlb_req_valid),
    .resp_data_o       (s1_tlb_resp_data),
    .satp_info_i       (satp_info_i),
    .matp_info_i       (matp_info_i),
    .satp_info_en_i    (satp_info_en_i),
    .matp_info_en_i    (matp_info_en_i),
    .tlb_invalidate_i  (tlb_invalidate_i),
    .ptw_req_data_o    (ptw_req_data_o),
    .ptw_req_valid_o   (ptw_req_valid_o),
    .ptw_req_ready_i   (ptw_req_ready_i),
    .ptw_resp_data_i   (ptw_resp_data_i),
    .ptw_resp_valid_i  (ptw_resp_valid_i),
    .vm_enabled_o      (vm_enabled)
  );

  always_comb begin
    s1_phys_addr = '0;
    s1_phys_addr[minion_dcache_pkg::PaSize-1:minion_dcache_pkg::PaUntransSize] = s1_tlb_resp_data.ppn;
    s1_phys_addr[minion_dcache_pkg::PaUntransSize-1:0] = s1_req.addr[minion_dcache_pkg::PaUntransSize-1:0];
    s1_phys_addr_tag = s1_phys_addr[minion_dcache_pkg::PaSize-1 -: PaTagExtSize];

    tlb_valid_addr = !s1_tlb_wait && !s1_tlb_resp_data.xcpt_ld && !s1_tlb_resp_data.xcpt_st;
    s1_pma_ts_tl_co =
        s1_translate_only_addr_load || ((s1_req.vm_sel == DcacheVmSelCo) && (s1_valid || s1_md_read_en));

    s1_pma_req_data = '0;
    s1_pma_req_data.paddr = s1_phys_addr;
    s1_pma_req_data.typ = s1_req.typ;
    s1_pma_req_data.amo = minion_dcache_pkg::dcache_cmd_is_amo(s1_req.cmd)
                       && !s1_translate_only_addr_load;
    s1_pma_req_data.write = s1_is_write;
    s1_pma_req_data.mem_global = s1_req.mem_global;
    s1_pma_req_data.ts_tl_co = s1_pma_ts_tl_co;
  end

  assign af_skip = (s1_req.replay && !s1_req.misaligned_2nd && s1_valid) || s1_co_mr_skip_pma;

  minion_dcache_pma_unit u_pma_unit (
    .ioshire_i    (ioshire_i),
    .af_skip_i    (af_skip),
    .req_data_i   (s1_pma_req_data),
    .mprot_i      (s1_mprot_i),
    .valid_addr_i (tlb_valid_addr),
    .vm_status_i  (s1_vm_status),
    .resp_data_o  (s1_pma_resp_data)
  );

  assign s1_tlb_wait = s1_tlb_resp_data.miss || s1_tlb_resp_data.fill_pending;
  assign s1_tlb_fail = s1_tlb_resp_data.xcpt_ld || s1_tlb_resp_data.xcpt_st
                    || s1_tlb_resp_data.access_fault || s1_pma_resp_data.access_fault;
  assign s1_clk_en = s1_md_read_en || s1_da_read_en || s1_reduce_send_inst
                  || s1_reduce_op_inst || s1_wb_da_valid;

  always_comb begin
    s1_block_offset = '0;
    unique case (s1_req.typ)
      minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePsGs8):  s1_block_offset = s1_req.addr[4:0];
      minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePsGs16): s1_block_offset = {s1_req.addr[4:1], 1'b0};
      minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePsGs32): s1_block_offset = {s1_req.addr[4:2], 2'b0};
      default: ;
    endcase

    s1_gather32_en = '0;
    s1_to_s2_gsc32_indices = '0;
    for (int unsigned i = 0; i < VpuLaneNum; i++) begin
      if (s1_req.ps_mask[i]) begin
        unique case (s1_req.typ)
          minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePsGs8):
              s1_to_s2_gsc32_indices[i] = s1_req.gsc32_idx[i*5 +: 5];
          minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePsGs16):
              s1_to_s2_gsc32_indices[i] = {s1_req.gsc32_idx[i*4 +: 4], 1'b0};
          minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePsGs32):
              s1_to_s2_gsc32_indices[i] = {s1_req.gsc32_idx[i*3 +: 3], 2'b0};
          default: ;
        endcase

        s1_to_s2_gsc32_indices[i] = s1_to_s2_gsc32_indices[i] + s1_block_offset;
        s1_gather32_en[s1_to_s2_gsc32_indices[i][DcacheBankSelLsb +: DcacheBankSelWidth]] =
            s1_valid
            && ((s1_req.cmd == minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdG32))
                || (s1_req.cmd == minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdSc32)));
      end
    end
  end

  always_comb begin
    s1_phys_addr_set =
        dcache_set_correct(s1_phys_addr[DcacheLramAddrWidth-1:6], dcache_cfg_mode, s1_req.dest.thread_id);

    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
      s1_tag_match[i] = (s1_phys_addr_tag == s1_mr_read_resp[i].tag) && s1_mr_read_valid[i];
    end

    s1_read_way = dcache_way_encode(s1_tag_match);
    s1_req_size_128b = s1_misaligned_case_4 || s1_req.misaligned_2nd;
    s1_req_size_256b = (s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs));

    for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
      s1_da_read_req_hit[i] = s1_da_read_req;

      if ((s1_req.cmd == minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdG32))
          || (s1_req.cmd == minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdSc32))) begin
        s1_da_read_req_hit[i].valid_l = s1_da_read_en && s1_gather32_en[i];
      end else begin
        s1_da_read_req_hit[i].valid_l =
            s1_da_read_en
            && ((s1_da_read_req_hit[i].addr[DcacheBankSelLsb +: DcacheBankSelWidth] == i[1:0])
                || ((s1_da_read_req_hit[i].addr[DcacheBankSelLsb +: DcacheBankSelWidth]
                     == (i[1:0] - 2'b1)) && s1_req_size_128b)
                || s1_req_size_256b);
      end
      s1_da_read_req_hit[i].valid_h = s1_da_read_req_hit[i].valid_l;

      if ((i[1:0] < s1_da_read_req.addr[DcacheBankSelLsb +: DcacheBankSelWidth])
          && s1_misaligned_case_3 && s1_valid) begin
        s1_da_read_req_hit[i].addr[DcacheBlockSelBit] =
            s1_da_read_req_hit[i].addr[DcacheBlockSelBit] + 1'b1;
        s1_da_read_req_hit[i].valid_l = 1'b1;
        s1_da_read_req_hit[i].valid_h = 1'b1;
      end

      s1_da_read_req_hit[i].way = s1_read_way;
    end
  end

  always_comb begin
    s1_access_size = 7'd3;
    if (minion_dcache_pkg::dcache_type_is_byte(s1_req.typ)) begin
      s1_access_size = 7'd0;
    end else if (minion_dcache_pkg::dcache_type_is_hword(s1_req.typ)) begin
      s1_access_size = 7'd1;
    end else if (minion_dcache_pkg::dcache_type_is_word(s1_req.typ)) begin
      s1_access_size = 7'd3;
    end else if (minion_dcache_pkg::dcache_type_is_dword(s1_req.typ)) begin
      s1_access_size = 7'd7;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[7]) begin
      s1_access_size = 7'd31;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[6]) begin
      s1_access_size = 7'd27;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[5]) begin
      s1_access_size = 7'd23;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[4]) begin
      s1_access_size = 7'd19;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[3]) begin
      s1_access_size = 7'd15;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[2]) begin
      s1_access_size = 7'd11;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[1]) begin
      s1_access_size = 7'd7;
    end else if ((s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs)) && s1_req.ps_mask[0]) begin
      s1_access_size = 7'd3;
    end

    s1_access_last_byte = {1'b0, s1_req.addr[5:0]} + s1_access_size;

    s1_misaligned_case_1 = 1'b0;
    s1_misaligned_case_2 = 1'b0;
    s1_misaligned_case_3 = 1'b0;
    s1_misaligned_case_4 = 1'b0;
    s1_misaligned_case_5 = 1'b0;
    if (!s1_req.misaligned_2nd && !s1_is_msg && !s1_is_gsc32) begin
      if (s1_access_last_byte[6]) begin
        s1_misaligned_case_1 = 1'b1;
      end else if ((s1_req.addr[DcacheBlockSelBit]
                    != s1_access_last_byte[DcacheBlockSelBit])
                   && (s1_req.typ == minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs))
                   && (|s1_req.addr[2:0])) begin
        s1_misaligned_case_2 = 1'b1;
      end else if (s1_req.addr[DcacheBlockSelBit]
                   != s1_access_last_byte[DcacheBlockSelBit]) begin
        s1_misaligned_case_3 = 1'b1;
      end else if (((s1_access_size[3:0] + {1'b0, s1_req.addr[2:0]}) > 4'd7)
                   && (s1_req.typ != minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypePs))) begin
        s1_misaligned_case_4 = 1'b1;
      end else if (((s1_access_size == 7'd1) && s1_req.addr[0])
                   || ((s1_access_size == 7'd3) && (|s1_req.addr[1:0]))
                   || ((s1_access_size == 7'd7) && (|s1_req.addr[2:0]))
                   || ((s1_access_size == 7'd15) && (|s1_req.addr[3:0]))
                   || ((s1_access_size == 7'd31) && (|s1_req.addr[4:0]))) begin
        s1_misaligned_case_5 = 1'b1;
      end
    end

    s1_misaligned_block = s1_misaligned_case_1 || s1_misaligned_case_2 || s1_misaligned_case_3;
    s1_misaligned = s1_misaligned_case_1 || s1_misaligned_case_2;
  end

  always_comb begin
    s1_valid_masked =
        s1_valid && (!s1_core_kill_i || s1_req.replay || s1_req.cache_op) && !(|s1_core_xcpt)
        && !s1_pma_bus_err;
    s1_valid_to_s2 = s1_valid_masked && !s1_tlb_resp_data.miss && (~s1_req.cache_op || ~s1_tlb_fail);
    s1_rq_push_conservative = s1_valid_to_s2 && !s1_req.replay;
    s1_rq_renack_conservative = s1_valid_to_s2 && s1_req.replay;

    s1_is_read = minion_dcache_pkg::dcache_cmd_is_read(s1_req.cmd);
    s1_is_amo = minion_dcache_pkg::dcache_cmd_is_amo(s1_req.cmd) && !s1_translate_only_addr_load;
    s1_is_read_no_amo = minion_dcache_pkg::dcache_cmd_is_read(s1_req.cmd) && !s1_is_amo;
    s1_is_msg = 1'b0;
    s1_is_bypass = minion_dcache_pkg::dcache_cmd_is_bypass(s1_req.cmd) && !s1_translate_only_addr_load;
    s1_is_gsc32 = (s1_req.cmd == minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdG32))
               || (s1_req.cmd == minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdSc32));
    s1_is_to_null = s1_is_read && !s1_req.dest.fp && (s1_req.dest.addr == '0);

    s1_nack_co_collide = (s1_req.set == s1_co_nack_set_value) && s1_co_nack_set_valid;
    s1_nack_vpu_rf_write = VpuEn && s1_is_read && s1_req.dest.fp && s1_vpu_ctrl_i.tfma_rf_write;
    s1_nack =
        (s1_valid_to_s2 && s1_req.buf_alloc && (s1_mh_force_ba_rptr || s1_re_ba_read_en_spec))
        || (VpuEn && s1_valid_to_s2 && s1_vpu_ctrl_i.scp_req.read_en && !s1_is_msg && !s1_is_amo
            && !s1_is_bypass)
        || (s1_valid_to_s2 && s1_nack_co_collide && !s1_is_msg && !s1_is_amo && !s1_is_bypass)
        || (s1_valid_to_s2 && s1_nack_vpu_rf_write);
    s1_addr_clk_en = s1_clk_en || s1_translate_only_addr_load || s1_has_xcpt_no_kill;
  end

  always_comb begin
    s1_tlb_flush_pipeline = '0;
    if (s1_valid_to_tlb && s1_tlb_resp_data.miss && (s1_req.vm_sel == DcacheVmSelIp)) begin
      s1_tlb_flush_pipeline[s1_req.dest.thread_id] = 1'b1;
    end

    if ((s1_valid_to_tlb && s1_tlb_resp_data.miss
         && minion_dcache_pkg::dcache_type_is_gsc(s1_req.typ)
         && !minion_pkg::dcache_cmd_is_gsc32(minion_pkg::dcache_cmd_e'(s1_req.cmd))
         && (s1_req.vm_sel == DcacheVmSelIp))
        || (s1_has_xcpt
            && minion_dcache_pkg::dcache_type_is_gsc(s1_req.typ)
            && !minion_pkg::dcache_cmd_is_gsc32(minion_pkg::dcache_cmd_e'(s1_req.cmd))
            && (s1_req.vm_sel == DcacheVmSelIp))) begin
      s1_save_progress = 1'b1;
    end else begin
      s1_save_progress = 1'b0;
    end

    if (s0_core_req_is_valid_gsc && (&s0_req.gsc_cnt)) begin
      s0_clear_progress = 1'b1;
    end else begin
      s0_clear_progress = 1'b0;
    end
  end

  always_comb begin
    s1_pma_access_fault = s1_pma_resp_data.access_fault && !s1_req.misaligned_2nd && s1_valid_to_tlb;
    s1_pma_bus_err = s1_pma_resp_data.access_fault && s1_req.misaligned_2nd && s1_valid_to_tlb;

    s1_core_xcpt = '0;
    s1_core_xcpt.pf_ld =
        (s1_is_read_no_amo || minion_dcache_pkg::dcache_cmd_is_prefetch(s1_req.cmd))
        && s1_tlb_resp_data.xcpt_ld && !s1_req.cache_op;
    s1_core_xcpt.pf_st = s1_is_store && s1_tlb_resp_data.xcpt_st && !s1_req.cache_op;
    s1_core_xcpt.af_ld =
        (s1_is_read_no_amo || minion_dcache_pkg::dcache_cmd_is_prefetch(s1_req.cmd))
        && (s1_pma_access_fault || s1_tlb_resp_data.access_fault) && !s1_req.cache_op;
    s1_core_xcpt.af_st = s1_is_store
                      && (s1_pma_access_fault || s1_tlb_resp_data.access_fault)
                      && !s1_req.cache_op;

    s1_mf_xcpt =
        !(s1_pma_resp_data.cacheable && !s1_is_amo && !s1_is_msg && !s1_is_bypass && !bypass_dcache_i)
        && !minion_dcache_pkg::dcache_address_is_size_aligned(s1_req.typ, s1_req.addr[4:0]);
    s1_core_xcpt.mf_ld = 1'b0;
    s1_core_xcpt.mf_st = 1'b0;
    s1_core_xcpt.af_ld |= s1_is_read_no_amo && s1_mf_xcpt;
    s1_core_xcpt.af_st |= s1_is_write && s1_mf_xcpt;

    s1_access_is_page_split =
        minion_dcache_pkg::dcache_access_is_page_split(s1_req.addr, vmspagesize_i,
                                                       s1_misaligned_case_1);
    s1_core_xcpt.ps_ld = s1_is_read_no_amo && s1_access_is_page_split && vm_enabled;
    s1_core_xcpt.ps_st = s1_is_write && s1_access_is_page_split && vm_enabled && !s1_mf_xcpt;

    if (s1_tlb_wait) begin
      s1_core_xcpt = '0;
    end

    s1_core_xcpt_no_kill = s1_valid ? s1_core_xcpt : dcache_tag_xcpt'('0);
    s1_has_xcpt_no_kill = |s1_core_xcpt_no_kill;

    if (!(s1_valid && !s1_core_kill_i)) begin
      s1_core_xcpt = '0;
    end
    s1_has_xcpt = |s1_core_xcpt;
  end

  assign s1_core_flush_pipeline = |s1_tlb_flush_pipeline;
  assign s1_core_flush_en = s1_valid_to_tlb || s2_core_flush_pipeline_o;

  always_comb begin
    s1_req_to_s2 = s1_req;
    s1_req_to_s2.addr = '0;
    s1_req_to_s2.phys = s1_req.phys || !s1_tlb_resp_data.miss;
    if (s1_has_xcpt) begin
      s1_req_addr_to_s2 = s1_req.addr[minion_dcache_pkg::VaSize-1:0];
    end else if (s1_req.phys) begin
      s1_req_addr_to_s2 = s1_req.addr[minion_dcache_pkg::VaSize-1:0];
    end else begin
      s1_req_addr_to_s2 = {{(minion_dcache_pkg::VaSize-minion_dcache_pkg::PaSize){1'b0}}, s1_phys_addr};
    end

    if (!s1_req_force_set) begin
      s1_req_to_s2.set =
          dcache_set_correct(s1_req_addr_to_s2[DcacheLramAddrWidth-1:6], dcache_cfg_mode,
                             s1_req.dest.thread_id);
    end

    if ((!s1_req.replay || s1_reduce_send_inst) && !s1_reduce_op_inst) begin
      s1_req_to_s2.data = s1_core_store_data_i;
    end else begin
      s1_req_to_s2.data = s1_ba_data;
    end

    s1_req_to_s2.misaligned = s1_misaligned_block && !s1_req.misaligned_2nd;
    for (int unsigned i = 0; i < (DcacheDataSize / 64); i++) begin
      s1_req_to_s2.chunk_read[i] = s1_da_read_req_hit[i].valid_l || s1_da_read_req_hit[i].valid_h;
    end
    s1_req_to_s2.cacheable =
        s1_pma_resp_data.cacheable && !s1_is_amo && !s1_is_msg && !s1_is_bypass && !bypass_dcache_i;
  end

  assign s1_da_read_en_ovr = s1_da_read_en_final && !mem_ctrl_override_i[MinionMemCtrlRe];
  assign s4_da_write_en_ovr = s4_da_write_en && !mem_ctrl_override_i[MinionMemCtrlWe];

  always_ff @(posedge clk_i) begin
    if (s1_da_read_en_final) begin
      s1_da_read_mux_sel_prev <= s1_da_read_mux_sel;
    end
  end

  always_comb begin
    s1_da_read_en_final = 1'b1;
    if (VpuEn && s1_vpu_ctrl_i.scp_req.read_en) begin
      s1_da_read_mux_sel = S1DaMuxSelVpu;
    end else if (VpuEn && ts_scp_read && s1_reduce_send_inst) begin
      s1_da_read_mux_sel = S1DaMuxSelTs;
    end else if (s1_wb_da_valid) begin
      s1_da_read_mux_sel = S1DaMuxSelWb;
    end else if (s1_da_read_en) begin
      s1_da_read_mux_sel = S1DaMuxSelCore;
    end else if (DebugApbEn && dbg_da_read_en) begin
      s1_da_read_mux_sel = S1DaMuxSelDbg;
    end else begin
      s1_da_read_mux_sel = s1_da_read_mux_sel_prev;
      s1_da_read_en_final = 1'b0;
    end

    s1_da_read_req_mux = s1_da_read_req_hit;
    dbg_da_read_accept_next = 1'b0;
    s1_wb_da_nack = s1_wb_da_valid && VpuEn && s1_vpu_ctrl_i.scp_req.read_en;
    if (s1_da_read_mux_sel == S1DaMuxSelWb) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s1_da_read_req_mux[i] = s1_wb_da_req;
      end
    end
    if (s1_da_read_mux_sel == S1DaMuxSelTs) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s1_da_read_req_mux[i].valid_l = 1'b1;
        s1_da_read_req_mux[i].valid_h = 1'b1;
        s1_da_read_req_mux[i].way = ts_scp_read_way;
        s1_da_read_req_mux[i].addr = ts_scp_read_addr;
      end
    end
    if (s1_da_read_mux_sel == S1DaMuxSelVpu) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s1_da_read_req_mux[i].valid_l = s1_vpu_ctrl_i.scp_req.size256
                                     || (s1_vpu_ctrl_i.scp_req.size64
                                         && (s1_vpu_ctrl_i.scp_req.addr[DcacheBankSelLsb +: DcacheBankSelWidth]
                                             == i[1:0]))
                                     || (s1_vpu_ctrl_i.scp_req.addr[4:2] == {i[1:0], 1'b0});
        s1_da_read_req_mux[i].valid_h = s1_vpu_ctrl_i.scp_req.size256
                                     || (s1_vpu_ctrl_i.scp_req.size64
                                         && (s1_vpu_ctrl_i.scp_req.addr[DcacheBankSelLsb +: DcacheBankSelWidth]
                                             == i[1:0]))
                                     || (s1_vpu_ctrl_i.scp_req.addr[4:2] == {i[1:0], 1'b1});
        s1_da_read_req_mux[i].way = s1_vpu_ctrl_i.scp_req.way;
        s1_da_read_req_mux[i].addr = s1_vpu_ctrl_i.scp_req.addr;
      end
    end
    if (s1_da_read_mux_sel == S1DaMuxSelDbg) begin
      dbg_da_read_accept_next = 1'b1;
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s1_da_read_req_mux[i].valid_l = 1'b1;
        s1_da_read_req_mux[i].valid_h = 1'b1;
        s1_da_read_req_mux[i].addr = {dbg_da_read_addr[DcacheDbgAddrBits-4:0], 3'b0};
        s1_da_read_req_mux[i].way = dbg_da_read_addr[DcacheDbgAddrBits-2:DcacheDbgAddrBits-3];
      end
    end

    for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
      s1_da_load_data_en[i] = s1_da_read_req_mux[i].valid_l || s1_da_read_req_mux[i].valid_h;
    end
    for (int unsigned i = 0; i < (DcacheDataSize / 32); i++) begin
      s1_vpu_scp_read_en[i] =
          VpuEn && s1_vpu_ctrl_i.scp_req.read_en
          && (s1_vpu_ctrl_i.scp_req.size256
              || (s1_vpu_ctrl_i.scp_req.size64 && (s1_vpu_ctrl_i.scp_req.addr[3] == i[1]))
              || (s1_vpu_ctrl_i.scp_req.addr[4:2] == i[2:0]));
    end
    s1_vpu_en = VpuEn && s1_vpu_ctrl_i.scp_req.read_en;
  end

  minion_dcache_data_array u_data_array (
    .clk_i          (clk_i),
    .s1_read_valid_i(s1_da_read_en_ovr),
    .s1_read_req_i  (s1_da_read_req_mux),
    .s2_read_resp_o (s2_da_read_resp),
    .s3_write_valid_i(s3_da_write_en),
    .s4_write_valid_i(s4_da_write_en_ovr),
    .s4_write_req_i (s4_da_write_data),
    .ram_cfg_i      (ram_cfg_pkg::ram_cfg_t'('0))
  );

  minion_dcache_lru_array u_lru_array (
    .clk_i      (clk_i),
    .rst_ni     (rst_ni),
    .read_set_i (s1_phys_addr_set),
    .read_data_o(s1_lru_data),
    .write_en_i (s2_lru_update),
    .write_set_i(s2_req.set),
    .write_data_i(s2_lru_data_up)
  );

  always_comb begin
    if (s2_lru_update && (s2_req.set == s1_phys_addr_set)) begin
      s1_lru_data_byp = s2_lru_data_up;
    end else begin
      s1_lru_data_byp = s1_lru_data;
    end
    s1_hlocked_ways = hlock_state[s1_req_to_s2.set];
    s1_slocked_ways = slock_state[s1_req_to_s2.set];
    s1_replace_way_en = dcache_lru_victim(s1_lru_data_byp, s1_hlocked_ways, s1_slocked_ways);
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s2_valid <= 1'b0;
      s2_reg_nack <= 1'b0;
      s2_has_xcpt_addr <= 1'b0;
    end else begin
      s2_valid <= s1_valid_to_s2;
      if (s1_valid) begin
        s2_reg_nack <= s1_nack;
        s2_has_xcpt_addr <= s1_has_xcpt;
        s2_nack_vpu_rf_write <= s1_nack_vpu_rf_write;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s1_clear_progress <= 1'b0;
      s2_clear_progress <= 1'b0;
      s2_save_progress <= 1'b0;
      s2_core_flush_pipeline_o <= 1'b0;
    end else begin
      s1_clear_progress <= s0_clear_progress && !s1_save_progress;
      s2_clear_progress <= s1_clear_progress && !s1_core_kill_i;
      s2_save_progress <= s1_save_progress && !s1_core_kill_i && !s2_save_progress;

      if (s1_core_flush_en) begin
        s2_core_flush_pipeline_o <= s1_core_flush_pipeline;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (s1_clk_en) begin
      s2_tag_match <= s1_tag_match;
      s2_req_pre_addr <= s1_req_to_s2;
      s2_lru_data <= s1_lru_data_byp;
      s2_misaligned <= s1_misaligned;
      s2_misaligned_case_3 <= s1_misaligned_case_3;
      s2_da_load_data_en <= s1_da_load_data_en;
      s2_replace_way_en <= s1_replace_way_en;
      s2_is_read <= s1_is_read;
      s2_is_write <= s1_is_write;
      s2_is_amo <= s1_is_amo;
      s2_gsc32_indices <= s1_to_s2_gsc32_indices;
    end
    if (s1_addr_clk_en) begin
      s2_req_addr <= s1_req_addr_to_s2;
      s2_is_msg <= s1_is_msg;
      s2_is_bypass <= s1_is_bypass;
    end
    if (s1_vpu_en) begin
      s2_vpu_scp_read_en <= s1_vpu_scp_read_en;
    end
    s2_wb_da_valid <= s1_wb_da_valid;
  end

  for (genvar mrw = 0; mrw < minion_dcache_pkg::DcacheWays; mrw++) begin : gen_s2_meta_read
    localparam int unsigned SetMsbWidth = DcacheSetIdxWidth - $clog2(DcacheSetRed);
    localparam int unsigned PaTagRegSize = PaTagExtSize - SetMsbWidth;
    logic [SetMsbWidth-1:0] set_msb;

    assign set_msb = dcache_cfg_sets_red ? '0 : s1_req.set[DcacheSetIdxWidth-1:$clog2(DcacheSetRed)];

    always_ff @(posedge clk_i) begin
      if (s1_clk_en) begin
        if (s1_mr_read_valid[mrw]) begin
          s2_meta_read[mrw] <= s1_mr_read_resp[mrw];
        end else begin
          s2_meta_read[mrw].coh_state <= StateInvalid;
          s2_meta_read[mrw].tag <= {{PaTagRegSize{1'b0}}, set_msb};
        end
      end
    end
  end

  /* verilator lint_off UNOPTFLAT */  // The translated original S2 request/BA/miss-handler path spans packed structs and combinational helpers; field-wise dataflow is acyclic and only depends on flopped state, but Verilator flattens it and reports false loops.
  always_comb begin
    s2_req = s2_req_pre_addr;
    s2_req.addr = {1'b0, s2_req_addr};

    s2_valid_qual = s2_valid && (!s2_core_kill_i || s2_req.replay || s2_req.cache_op);
    s2_valid_masked = s2_valid_qual && !s2_nack_pre_misaligned;

    s2_tag_match_qual = s2_tag_match & {minion_dcache_pkg::DcacheWays{!s2_is_msg}} & {minion_dcache_pkg::DcacheWays{!s2_is_amo}}
                      & {minion_dcache_pkg::DcacheWays{!s2_is_bypass}} & {minion_dcache_pkg::DcacheWays{!bypass_dcache_i}};
    s2_tag_match_any = |s2_tag_match_qual;

    s2_way_hit_state = StateInvalid;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
      if (s2_tag_match_qual[i]) begin
        s2_way_hit_state = s2_meta_read[i].coh_state;
      end
    end

    s2_state_hit = (s2_is_read && (s2_way_hit_state >= StateShared))
                || ((s2_req.cmd == minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdPfr))
                    && (s2_way_hit_state >= StateShared))
                || (minion_dcache_pkg::dcache_cmd_is_write_intent(s2_req.cmd)
                    && (s2_way_hit_state >= StateExclusive));
    s2_state_hit &= !s2_is_write || (s2_way_hit_state == StateModified);
    s2_hit = s2_tag_match_any && s2_state_hit;
  end

  always_comb begin
    s2_nack_inflight = s2_hit && s2_mh_match_miss_line;
    s2_nack_miss = !s2_hit && !s2_mh_req_ready && !s2_req.uc_load_2nd;
    s2_nack_pre_misaligned_miss = s2_reg_nack || s2_nack_inflight || s2_nack_rq_conflict;
    s2_nack_pre_misaligned = s2_nack_pre_misaligned_miss || s2_nack_miss;
    s2_nack_misaligned =
        !s2_nack_pre_misaligned && s2_misaligned && !s2_req.misaligned_2nd && s2_hit;
    s2_nack = s2_nack_pre_misaligned || s2_nack_misaligned;

    s2_repl_meta = '0;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
      if (s2_replace_way_en[i]) begin
        s2_repl_meta = s2_meta_read[i];
      end
    end

    s2_mh_req_valid_reg =
        s2_valid_qual && !s2_nack_pre_misaligned_miss && !s2_hit
        && (minion_dcache_pkg::dcache_cmd_is_prefetch(s2_req.cmd) || s2_is_read || s2_is_write)
        && !s2_is_msg && !s2_req.uc_load_2nd && !(s2_req.replay && !s2_req.miss_fill_ok);
    s2_mh_req_valid =
        s2_mh_req_valid_reg
        || (s2_valid_qual && s2_is_msg && !s2_nack_rq_conflict && !s2_req.uc_load_2nd);

    s2_mh_req_info = '0;
    s2_mh_req_info.addr = s2_req.addr[minion_dcache_pkg::PaSize-1:0];
    s2_mh_req_info.set = s2_req.set;
    s2_mh_req_info.cmd = s2_req.cmd;
    s2_mh_req_info.typ = s2_req.typ;
    s2_mh_req_info.mem_g = s2_req.mem_global;
    s2_mh_req_info.buf_id = !s2_req.replay ? s2_ba_write_entry : s2_req.buf_id;
    s2_mh_req_info.tag_match = s2_tag_match_any;
    s2_mh_req_info.cacheable = s2_req.cacheable;
    s2_mh_req_info.thread_id = s2_req.dest.thread_id;
    s2_mh_req_info.is_amo = s2_is_amo;
    s2_mh_req_info.is_msg = s2_is_msg;
    s2_mh_req_info.is_bypass = s2_is_bypass;
    if (s2_tag_match_any) begin
      s2_mh_req_info.old_meta.tag = s2_repl_meta.tag;
      s2_mh_req_info.old_meta.coh_state = s2_way_hit_state;
      s2_mh_req_info.way_en = s2_tag_match_qual;
    end else begin
      s2_mh_req_info.old_meta = s2_repl_meta;
      s2_mh_req_info.way_en = s2_replace_way_en;
    end

    s2_lru_data_up = dcache_lru_update(s2_lru_data, s2_mh_req_info.way_en);
    s2_lru_update = s2_valid_masked && (s2_hit || ~s2_mh_match_miss_line) && s2_req.cacheable;
  end

  assign id_rq_replay_bid_qual =
      id_rq_replay_bid && !wb_dcache_fp_toint_i && !(VpuEn && vpu_scp_bid_2);

  assign s1_ba_rptr = s1_mh_force_ba_rptr ? s1_mh_ba_rptr
                                          : (s1_re_ba_read_en_spec ? s1_re_ba_read_addr
                                                                   : s1_req.buf_id);
  assign s1_ba_dealloc_uc = s1_mh_force_ba_rptr_qual && !s1_mh_ba_is_amo;
  assign re_ba_dealloc =
      VpuEn
      && ((re_l2_evict_ready && re_l2_evict_valid && s1_re_ba_read_en)
          || (ts_l2_evict_ready && ts_l2_evict_valid && s1_re_ba_read_en)
          || (s1_reduce_op_inst && s1_re_ba_read_en));

  always_comb begin
    s2_store_misaligned_rearm = s2_valid_to_s3 && s2_is_write && s2_nack_misaligned;

    s2_rq_push_cancel = s2_valid_to_s3 && !s2_store_misaligned_rearm
                     || s2_mh_can_accept_uc && s2_mh_req_valid && s2_is_write && !s2_is_amo
                     || minion_dcache_pkg::dcache_cmd_is_prefetch(s2_req.cmd)
                        && !s2_nack_pre_misaligned
                     || s2_is_msg && s2_is_write && !s2_nack_miss && !s2_nack_rq_conflict
                     || s2_valid_masked && s2_is_bypass && s2_is_write
                     || s2_valid_masked && s2_req.uc_load_2nd && !s2_req.uc_load_ok
                     || s2_valid_masked && s2_req.replay && !s2_req.miss_fill_ok;

    s2_rq_push = s2_valid_qual && !s2_req.replay && !s2_rq_push_cancel;
    s2_rq_renack = s2_valid_qual && s2_req.replay && !s2_rq_push_cancel;
    s2_rq_dealloc = s2_valid_qual && s2_req.replay && s2_rq_push_cancel;

    s2_rq_conf_type = '0;
    s2_rq_conf_type.miss_alloc = s2_nack_miss && !s2_nack_vpu_rf_write;
    s2_rq_conf_type.miss_assigned =
        s2_mh_req_assigned & {DcacheMhFileSize{!s2_nack_vpu_rf_write}};
    s2_rq_conf_type.miss_accepted =
        s2_mh_req_accepted & {DcacheMhFileSize{!s2_nack_vpu_rf_write}};
    s2_rq_conf_type.vpu_rf_write = s2_nack_vpu_rf_write;

    s2_uc_load_1st = s2_mh_req_valid && s2_is_read && s2_mh_can_accept_uc && !s2_req.uc_load_2nd;
    s2_uc_load_ok = s2_req.replay ? s2_req.uc_load_ok : 1'b0;
    s2_miss_fill_ok = s2_req.replay ? s2_req.miss_fill_ok : 1'b1;
  end

  always_comb begin
    s2_ba_nack_needed = s2_is_write;
    s2_ba_write_needed_core =
        s2_valid_qual && !s2_req.replay && s2_nack && s2_ba_nack_needed
        || s2_valid_qual && !s2_req.replay && s2_misaligned
        || s2_valid_qual && !s2_req.replay && s2_is_read && !s2_req.cacheable
        || s2_mh_req_valid && !s2_req.replay && s2_is_write
        || s2_valid_qual && !s2_req.replay && s2_is_msg;
    s2_ba_write_needed = s2_ba_write_needed_core || s2_re_ba_write_needed;

    s2_ba_write_en =
        s2_valid_qual && !s2_req.replay && s2_nack && s2_ba_nack_needed
        || s2_valid_qual && s2_nack_misaligned
        || s2_mh_req_valid && !s2_req.replay && s2_is_write
        || s2_valid_qual && !s2_req.replay && s2_is_write && s2_is_msg
        || s2_reduce_send_inst;

    s2_ba_dealloc =
        ((s2_valid_to_s3)
         || (s2_valid_masked && s2_req.uc_load_2nd)
         || (s2_valid_masked && s2_req.replay && !s2_req.miss_fill_ok))
        && !s2_store_misaligned_rearm && s2_req.buf_alloc;

    s2_ba_replay = s2_req.replay && !s2_re_ba_write_needed;

    s3_ba_write_en_uc = l2_resp_valid_ready_int && l2_resp_is_for_mh_uc_ok;
    s3_ba_write_en_msg = l2_resp_valid_ready_int && l2_resp_is_reduce_data;
    s3_ba_write_en_ext = s3_ba_write_en_uc || s3_ba_write_en_msg;
    s3_ba_write_addr_ext = s3_ba_write_en_uc ? s3_mh_ba_wptr : s3_re_ba_write_addr;
    s2_ba_write_en_ext_prev = mh_wait_l2_to_ba_resp || re_wait_l2_to_ba_resp;
    s3_ba_write_data_ext = l2_resp_int.data;
  end

  always_comb begin
    for (int unsigned i = 0; i < (DcacheDataSize / 64); i++) begin
      s3_data_bypassed[i*64 +: 64] =
          s3_store_bypass[i] ? s3_store_bypass_data[i*64 +: 64] : s3_data[i];
    end

    s3_data_aligned_wide = {s3_data_bypassed, s3_data_bypassed} >> {s3_req.addr[4:0], 3'b0};
    s3_data_aligned = s3_data_aligned_wide[DcacheDataSize-1:0];

    if (s3_req.uc_load_2nd || s3_reduce_op_inst) begin
      s3_data_merge_sel = {(DcacheDataSize/8){1'b1}};
    end else if (s3_req.misaligned_2nd) begin
      s3_data_merge_sel = {(DcacheDataSize/8){1'b1}} >> s3_req.addr[4:0];
    end else begin
      s3_data_merge_sel = '0;
    end

    for (int unsigned i = 0; i < (DcacheDataSize / 8); i++) begin
      s3_data_merged[i*8 +: 8] =
          s3_data_merge_sel[i] ? s3_store_data[i*8 +: 8] : s3_data_aligned[i*8 +: 8];
    end

    if (s3_reduce_op_inst) begin
      s3_data_extended = s3_data_merged[minion_dcache_pkg::XregSize-1:0];
    end else if (s3_req.typ == minion_dcache_pkg::DcTypeB || s3_req.typ == minion_dcache_pkg::DcTypePsGs8) begin
      s3_data_extended = {{56{s3_data_merged[7]}}, s3_data_merged[7:0]};
    end else if (s3_req.typ == minion_dcache_pkg::DcTypeH || s3_req.typ == minion_dcache_pkg::DcTypePsGs16) begin
      s3_data_extended = {{48{s3_data_merged[15]}}, s3_data_merged[15:0]};
    end else if (s3_req.typ == minion_dcache_pkg::DcTypeW || s3_req.typ == minion_dcache_pkg::DcTypePsGs32) begin
      s3_data_extended = {{32{s3_data_merged[31]}}, s3_data_merged[31:0]};
    end else if (s3_req.typ == minion_dcache_pkg::DcTypeBu) begin
      s3_data_extended = {{56{1'b0}}, s3_data_merged[7:0]};
    end else if (s3_req.typ == minion_dcache_pkg::DcTypeHu) begin
      s3_data_extended = {{48{1'b0}}, s3_data_merged[15:0]};
    end else if (s3_req.typ == minion_dcache_pkg::DcTypeWu) begin
      s3_data_extended = {{32{1'b0}}, s3_data_merged[31:0]};
    end else begin
      s3_data_extended = s3_data_merged[minion_dcache_pkg::XregSize-1:0];
    end

    s3_is_gather32 = (s3_req.cmd == minion_dcache_pkg::DcCmdG32) && s3_valid;
    for (int unsigned i = 0; i < VpuLaneNum; i++) begin
      s3_data_word[i] = s3_data_bypassed[s3_gsc32_indices[i][4:2] * 32 +: 32];
      s3_data_half[i] = s3_data_bypassed[s3_gsc32_indices[i][4:1] * 16 +: 16];
      s3_data_byte[i] = s3_data_bypassed[s3_gsc32_indices[i] * 8 +: 8];

      if (s3_req.typ == minion_dcache_pkg::DcTypePsGs8) begin
        s3_data_gather32[i*GscLaneBits +: GscLaneBits] = {{24{s3_data_byte[i][7]}}, s3_data_byte[i]};
      end else if (s3_req.typ == minion_dcache_pkg::DcTypePsGs16) begin
        s3_data_gather32[i*GscLaneBits +: GscLaneBits] = {{16{s3_data_half[i][15]}}, s3_data_half[i]};
      end else if (s3_req.typ == minion_dcache_pkg::DcTypePsGs32) begin
        s3_data_gather32[i*GscLaneBits +: GscLaneBits] = s3_data_word[i];
      end else begin
        s3_data_gather32[i*GscLaneBits +: GscLaneBits] = '0;
      end
    end

    s3_load_data_mux1 = {s3_data_merged[DcacheDataSize-1:minion_dcache_pkg::XregSize], s3_data_extended};
    s3_load_data_mux2 = s3_is_gather32 ? s3_data_gather32 : s3_load_data_mux1;

    s3_store_amo_ext_data = s3_store_data;
    if (s3_req.cmd == minion_dcache_pkg::DcCmdXaCmpswp) begin
      if (minion_dcache_pkg::dcache_type_is_word(s3_req.typ)) begin
        s3_store_amo_ext_data[64 +: 32] = s3_core_x31_i[31:0];
      end else begin
        s3_store_amo_ext_data[64 +: 64] = s3_core_x31_i;
      end
    end

    s3_ba_write_data = s3_reduce_send_inst ? s3_store_data
                                           : (s3_is_write ? s3_store_amo_ext_data
                                                          : s3_load_data_mux1);
  end

  always_comb begin
    rq_uc_load_fill = l2_resp_valid_ready_int && l2_resp_is_for_mh_uc;
    rq_uc_load_fill_id = s3_mh_ba_wptr;
    rq_uc_load_fill_ok = !(|mh_uc_load_bus_error);
  end

  always_comb begin
    s2_valid_to_s3 =
        s2_valid_masked && (s2_hit || (s2_req.uc_load_2nd && s2_req.uc_load_ok))
        && (s2_is_write || !s2_nack_misaligned);
    s2_has_data = s2_valid_to_s3 && s2_is_read && !(!s2_req.dest.fp && (s2_req.dest.addr == '0));
    s2_to_s3_en = (s2_valid_masked && (s2_hit || s2_req.uc_load_2nd)) || s2_ba_write_en;
    s2_to_s2_req_en = s2_to_s3_en || s2_has_xcpt_addr;
    s2_to_s3_en_store_data =
        (s2_valid_masked && s2_hit && s2_is_write)
        || (s2_valid_masked && s2_req.uc_load_2nd)
        || (s2_ba_write_en && s2_is_write)
        || (s2_valid_masked && s2_hit && s2_is_read && s2_req.misaligned_2nd)
        || s2_reduce_send_inst || s2_reduce_op_inst;
    s2_to_s3_en_store_byp = s2_valid_masked && s2_hit && (|s5_store_bypass);
  end
  /* verilator lint_on UNOPTFLAT */

  always_comb begin
    for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
      s2_to_s3_en_load_data[i] =
          (((s2_valid_masked && s2_hit) || s2_wb_da_valid) && s2_da_load_data_en[i])
          || ((dbg_da_read_addr[1:0] == i[1:0]) && dbg_da_read_accept);
      s2_data[i*DcacheLramDataSize +: DcacheLramDataSize] = s2_da_read_resp[i];
    end
  end

  always_comb begin
    if (!s2_reduce_send_inst) begin
      if (s2_reduce_op_inst) begin
        s2_to_s3_store_data = s2_req.data;
      end else if (s2_req.uc_load_2nd) begin
        s2_to_s3_store_data = s2_req.data >> {s2_req.addr[4:0], 3'b0};
      end else if (!s2_req.cacheable && s2_is_write && !s2_is_msg && !s2_is_amo) begin
        s2_to_s3_store_data = s2_req.data << {s2_req.addr[4:0], 3'b0};
      end else if (s2_is_amo) begin
        if (minion_dcache_pkg::dcache_type_is_byte(s2_req.typ)) begin
          s2_to_s3_store_data = {{(DcacheDataSize-8){1'b0}}, s2_req.data[7:0]};
        end else if (minion_dcache_pkg::dcache_type_is_hword(s2_req.typ)) begin
          s2_to_s3_store_data = {{(DcacheDataSize-16){1'b0}}, s2_req.data[15:0]};
        end else if (minion_dcache_pkg::dcache_type_is_word(s2_req.typ)) begin
          s2_to_s3_store_data = {{(DcacheDataSize-32){1'b0}}, s2_req.data[31:0]};
        end else if (minion_dcache_pkg::dcache_type_is_dword(s2_req.typ)) begin
          s2_to_s3_store_data = {{(DcacheDataSize-64){1'b0}}, s2_req.data[63:0]};
        end else begin
          s2_to_s3_store_data = s2_req.data;
        end
      end else begin
        s2_to_s3_store_data = s2_req.data;
      end
    end else begin
      if (ts_scp_read) begin
        if (!ts_col_size_is_128b) begin
          s2_to_s3_store_data = s2_data;
        end else begin
          s2_to_s3_store_data = {s2_data[(DcacheDataSize/2)-1:0], s2_data[(DcacheDataSize/2)-1:0]};
        end
      end else begin
        if (!ts_col_size_is_128b) begin
          s2_to_s3_store_data = s2_req.data;
        end else begin
          s2_to_s3_store_data = {s2_req.data[(DcacheDataSize/2)-1:0], s2_req.data[(DcacheDataSize/2)-1:0]};
        end
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s3_valid <= 1'b0;
      s3_has_data <= 1'b0;
    end else begin
      s3_valid <= s2_valid_to_s3;
      s3_has_data <= s2_has_data;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s2_to_s2_req_en) begin
      s3_req <= s2_req;
    end
    if (s2_to_s3_en) begin
      s3_way <= s2_tag_match_qual;
      s3_store_bypass <= s5_store_bypass;
      s3_misaligned_case_3 <= s2_misaligned_case_3;
      s3_is_write <= s2_is_write;
      s3_gsc32_indices <= s2_gsc32_indices;
    end
    if (s2_to_s3_en_store_data) begin
      s3_store_data <= s2_to_s3_store_data;
    end
    if (s2_to_s3_en_store_byp) begin
      s3_store_bypass_data <= s5_store_bypass_data;
    end
  end

  for (genvar num_word = 0; num_word < (DcacheDataSize / 32); num_word++) begin : gen_vpu_scp_data
    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        s3_vpu_scp_data_o[num_word*32 +: 32] <= '0;
      end else if (VpuEn && s2_vpu_scp_read_en[num_word]) begin
        s3_vpu_scp_data_o[num_word*32 +: 32] <= s2_data[num_word*32 +: 32];
      end
    end
  end

  for (genvar lram_bank = 0; lram_bank < DcacheLramNumBanks; lram_bank++) begin : gen_s3_data
    always_ff @(posedge clk_i) begin
      if (s2_to_s3_en_load_data[lram_bank]) begin
        s3_data[lram_bank] <= s2_da_read_resp[lram_bank];
      end
    end
  end

  assign s3_vpu_scp_resp_tmp.fill_is_tenb_early = 1'b0;
  assign s3_vpu_scp_resp_tmp.tenb_flush = 1'b0;

  always_ff @(posedge clk_i) begin
    s3_vpu_scp_resp_tmp.fill_is_tenb <= s3_vpu_scp_resp_o.fill_is_tenb_early;
    if (VpuEn && s3_vpu_scp_resp_o.fill_is_tenb_early) begin
      s3_vpu_tenb_data_o <= l2_resp_int.data;
      s3_vpu_scp_resp_tmp.tenb_line <= s4_tl1_load_addr[DcacheSetIdxWidth-1:0];
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      vpu_scp_return_cycle_1 <= 1'b0;
    end else if (VpuEn && s3_vpu_scp_resp_o.fill_is_tenb_early) begin
      vpu_scp_return_cycle_1 <= ~vpu_scp_return_cycle_1;
    end
  end

  always_comb begin
    s3_vpu_scp_resp_o = '0;
    s4_tenb_written = 1'b0;
    if (VpuEn) begin
      s3_vpu_scp_resp_o = s3_vpu_scp_resp_tmp;
      s3_vpu_scp_resp_o.fill_is_tenb_early =
          l2_resp_valid_int && l2_resp_is_for_tensor_1 && s4_tl1_tenb_ready_reg
          && !l2_resp_ready_blocked;
      s4_tenb_written = s3_vpu_scp_resp_o.fill_is_tenb_early && vpu_scp_return_cycle_1;
      s3_vpu_scp_resp_o.tenb_flush = tenb_flush;
    end
  end

  minion_dcache_store_merge_unit u_store_merge_unit (
    .addr_i      (s3_req.addr[4:0]),
    .typ_i       (s3_req.typ),
    .ps_mask_i   (s3_req.ps_mask),
    .orig_data_i (s3_data_bypassed),
    .store_data_i(s3_store_data),
    .enable_lo_i (s3_store_merge_enable_lo),
    .enable_hi_i (!s3_req.misaligned_2nd),
    .merge_out_o (s3_store_merge_out)
  );

  assign s3_store_merge_enable_lo = s3_misaligned_case_3 || s3_req.misaligned_2nd;

  always_comb begin
    for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
      s3_scatter32_data[i] = s3_data_bypassed[i*DcacheLramDataSize +: DcacheLramDataSize];
    end

    for (int unsigned i = 0; i < VpuLaneNum; i++) begin
      if (s3_req.ps_mask[i]) begin
        unique case (s3_req.typ)
          minion_dcache_pkg::DcTypePsGs8: begin
            s3_scatter32_data[s3_gsc32_indices[i][DcacheBankSelLsb +: DcacheBankSelWidth]][s3_gsc32_indices[i][2:0]*8 +: 8] =
                s3_store_data[i*32 +: 8];
          end
          minion_dcache_pkg::DcTypePsGs16: begin
            s3_scatter32_data[s3_gsc32_indices[i][DcacheBankSelLsb +: DcacheBankSelWidth]][s3_gsc32_indices[i][2:1]*16 +: 16] =
                s3_store_data[i*32 +: 16];
          end
          minion_dcache_pkg::DcTypePsGs32: begin
            s3_scatter32_data[s3_gsc32_indices[i][DcacheBankSelLsb +: DcacheBankSelWidth]][s3_gsc32_indices[i][2]*32 +: 32] =
                s3_store_data[i*32 +: 32];
          end
          default: begin
          end
        endcase
      end
    end
  end

  always_comb begin
    s3_lr = (s3_req.cmd == minion_dcache_pkg::DcCmdXlr);
    s3_sc = (s3_req.cmd == minion_dcache_pkg::DcCmdXsc);
    s3_lrsc_valid = |s3_lrsc_count;
    s3_lrsc_addr_match = s3_lrsc_valid && (s3_lrsc_addr == s3_req.addr[minion_dcache_pkg::PaSize-1:6]);
    s3_sc_fail = s3_sc && s3_valid && !s3_lrsc_addr_match;

    s3_lrsc_count_next = s3_lrsc_count;
    if (s3_lrsc_valid) begin
      s3_lrsc_count_next = s3_lrsc_count - 1'b1;
    end
    if (s3_valid) begin
      if (s3_lr) begin
        s3_lrsc_count_next = LrscCycles[LrscCountWidth-1:0] - 1'b1;
      end else if (s3_lrsc_valid) begin
        s3_lrsc_count_next = '0;
      end
    end
    if (s3_invalidate_lr_i) begin
      s3_lrsc_count_next = '0;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s3_lrsc_count <= '0;
    end else begin
      s3_lrsc_count <= s3_lrsc_count_next;
      if (s3_valid && s3_lr) begin
        s3_lrsc_addr <= s3_req.addr[minion_dcache_pkg::PaSize-1:6];
      end
    end
  end

  always_comb begin
    s3_valid_to_s4 = s3_valid && !s3_sc_fail && s3_is_write;
    s3_to_s4_en = s3_valid && s3_is_write;

    s3_req_to_s4 = s3_req;
    s3_req_to_s4.data = s3_store_merge_out;
    if (s3_req.cmd == minion_dcache_pkg::DcCmdSc32) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s3_req_to_s4.data[i*DcacheLramDataSize +: DcacheLramDataSize] = s3_scatter32_data[i];
      end
    end
  end

  always_comb begin
    s3_da_write_en = (s3_valid && s3_is_write) || l2_resp_valid_prev_int || l2_resp_valid_int
                  || s3_co_da_clear_en || s3_cfg_da_clear_en || s3_tl0_load_valid_pre
                  || (DebugApbEn && dbg_da_write_en_early);
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s4_valid <= 1'b0;
    end else begin
      s4_valid <= s3_valid_to_s4;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s3_to_s4_en) begin
      s4_req <= s3_req_to_s4;
      s4_way <= s3_way;
      s4_misaligned_case_3 <= s3_misaligned_case_3;
    end
  end

  always_comb begin
    s4_da_write_en_but_dbg =
        s4_valid || (l2_resp_valid_int && l2_da_write)
        || ((|s4_tl0_load_valid) && s4_tl0_load_ready)
        || (s4_co_da_clear_val || s4_cfg_da_clear_val);
    s4_da_write_en =
        s4_da_write_en_but_dbg || (DebugApbEn && dbg_da_write_en && !dbg_da_write_valid);

    for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
      s4_req_da_write_addr[i] = {s4_req.set, s4_req.addr[5:0]};
      if ((i[1:0] < s4_req.addr[DcacheBankSelLsb +: DcacheBankSelWidth])
          && s4_misaligned_case_3) begin
        s4_req_da_write_addr[i][DcacheBlockSelBit] =
            s4_req_da_write_addr[i][DcacheBlockSelBit] + 1'b1;
      end

      s4_da_write_data[i] = '0;
    end

    s4_co_da_clear_rdy = 1'b0;
    s4_cfg_da_clear_rdy = 1'b0;

    if (s4_valid) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s4_da_write_data[i].valid_l = s4_req.chunk_read[i];
        s4_da_write_data[i].valid_h = s4_da_write_data[i].valid_l;
        s4_da_write_data[i].way = dcache_way_encode(s4_way);
        s4_da_write_data[i].addr = s4_req_da_write_addr[i];
        s4_da_write_data[i].data = s4_req.data[i*DcacheLramDataSize +: DcacheLramDataSize];
      end
    end else if ((l2_resp_int.opcode == EtLinkRspMsgRcvData) && l2_resp_is_port_data
               && l2_resp_valid_int) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s4_da_write_data[i].valid_l =
            (l2_resp_int.size == EtLinkLine)
            || (l2_resp_int.size == EtLinkHLine)
            || ((l2_resp_int.size == EtLinkQWord) && (core_ctrl_i.msg_port_wr_addr[4] == i[1]))
            || (core_ctrl_i.msg_port_wr_addr[DcacheBankSelLsb +: DcacheBankSelWidth] == i[1:0]);
        s4_da_write_data[i].valid_h = s4_da_write_data[i].valid_l;
        if (l2_resp_int.size == EtLinkDWord) begin
          s4_da_write_data[i].data = l2_resp_int.data[0*DcacheLramDataSize +: DcacheLramDataSize];
        end else if (l2_resp_int.size == EtLinkQWord) begin
          s4_da_write_data[i].data =
              l2_resp_int.data[i[0]*DcacheLramDataSize +: DcacheLramDataSize];
        end else begin
          s4_da_write_data[i].data = l2_resp_int.data[i*DcacheLramDataSize +: DcacheLramDataSize];
        end
        s4_da_write_data[i].way = core_ctrl_i.msg_port_wr_way;
        s4_da_write_data[i].addr = core_ctrl_i.msg_port_wr_addr;
      end
    end else if (l2_resp_valid_int && l2_da_write) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s4_da_write_data[i].valid_l = 1'b1;
        s4_da_write_data[i].valid_h = 1'b1;
        s4_da_write_data[i].data = l2_resp_int.data[i*DcacheLramDataSize +: DcacheLramDataSize];
        s4_da_write_data[i].way = l2_resp_is_for_tensor_0 ? s4_tl0_load_way : s4_mh_refill_way;
        s4_da_write_data[i].addr = l2_resp_is_for_tensor_0 ? s4_tl0_load_addr : s4_mh_refill_addr;
      end
    end else if (|s4_tl0_load_valid) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s4_da_write_data[i].valid_l = s4_tl0_load_valid[i];
        s4_da_write_data[i].valid_h = s4_da_write_data[i].valid_l;
        s4_da_write_data[i].data = s4_tl0_load_data[i*DcacheLramDataSize +: DcacheLramDataSize];
        s4_da_write_data[i].way = s4_tl0_load_way;
        s4_da_write_data[i].addr = s4_tl0_load_addr;
      end
    end else if (s4_cfg_da_clear_val) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s4_da_write_data[i].valid_l = 1'b1;
        s4_da_write_data[i].valid_h = 1'b1;
        s4_da_write_data[i].data = '0;
        s4_da_write_data[i].way = s4_cfg_da_clear_way;
        s4_da_write_data[i].addr = {s4_cfg_da_clear_set, s4_cfg_da_clear_idx, 5'b0};
      end
      s4_cfg_da_clear_rdy = 1'b1;
    end else if (s4_co_da_clear_val) begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s4_da_write_data[i].valid_l = 1'b1;
        s4_da_write_data[i].valid_h = 1'b1;
        s4_da_write_data[i].data = '0;
        s4_da_write_data[i].way = s4_co_da_clear_way;
        s4_da_write_data[i].addr = {s4_co_da_clear_set, s4_co_da_clear_idx, 5'b0};
      end
      s4_co_da_clear_rdy = 1'b1;
    end else begin
      for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
        s4_da_write_data[i].valid_l = dbg_da_write_en && (dbg_da_write_addr[1:0] == i[1:0]);
        s4_da_write_data[i].valid_h = s4_da_write_data[i].valid_l;
        s4_da_write_data[i].data = dbg_da_write_data;
        s4_da_write_data[i].addr = {dbg_da_write_addr[DcacheDbgAddrBits-4:0], 3'b0};
        s4_da_write_data[i].way = dbg_da_write_addr[DcacheDbgAddrBits-2:DcacheDbgAddrBits-3];
      end
    end

    for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
      s4_block_addr[i] = s4_req_da_write_addr[i][DcacheBlockSelBit];
    end
  end

  always_comb begin
    s4_clk_en = s4_valid && s1_valid;
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s5_valid <= 1'b0;
    end else begin
      s5_valid <= s4_valid;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s4_clk_en) begin
      s5_req <= s4_req;
      s5_block_addr <= s4_block_addr;
    end
  end

  always_comb begin
    s5_store_bypass = '0;
    s5_store_bypass_data = '0;

    s5_cl_addr_match_s3_s2 = (s3_req.addr[minion_dcache_pkg::PaSize-1:6] == s2_req.addr[minion_dcache_pkg::PaSize-1:6]);
    s5_cl_addr_match_s4_s2 = (s4_req.addr[minion_dcache_pkg::PaSize-1:6] == s2_req.addr[minion_dcache_pkg::PaSize-1:6]);
    s5_cl_addr_match_s5_s2 = (s5_req.addr[minion_dcache_pkg::PaSize-1:6] == s2_req.addr[minion_dcache_pkg::PaSize-1:6]);

    for (int unsigned i = 0; i < DcacheLramNumBanks; i++) begin
      s2_block_addr[i] = s2_req.addr[DcacheBlockSelBit];
      if ((i[1:0] < s2_req.addr[DcacheBankSelLsb +: DcacheBankSelWidth])
          && s2_misaligned_case_3) begin
        s2_block_addr[i] = s2_block_addr[i] + 'b1;
      end

      s5_store_bypass_data[i*DcacheLramDataSize +: DcacheLramDataSize] =
          s5_req.data[i*DcacheLramDataSize +: DcacheLramDataSize];
      if (s5_valid && s5_cl_addr_match_s5_s2 && s5_req.chunk_read[i]
          && (s5_block_addr[i] == s2_block_addr[i])) begin
        s5_store_bypass[i] = 1'b1;
      end

      if (s4_valid && s5_cl_addr_match_s4_s2 && s4_req.chunk_read[i]
          && (s4_block_addr[i] == s2_block_addr[i])) begin
        s5_store_bypass[i] = 1'b1;
        s5_store_bypass_data[i*DcacheLramDataSize +: DcacheLramDataSize] =
            s4_req.data[i*DcacheLramDataSize +: DcacheLramDataSize];
      end

      s3_block_addr[i] = s3_req_to_s4.addr[DcacheBlockSelBit];
      if ((i[1:0] < s3_req_to_s4.addr[DcacheBankSelLsb +: DcacheBankSelWidth])
          && s3_misaligned_case_3) begin
        s3_block_addr[i] = s3_block_addr[i] + 'b1;
      end

      if (s3_valid && s3_is_write && !s3_sc_fail && s5_cl_addr_match_s3_s2 && s3_req.chunk_read[i]
          && (s3_block_addr[i] == s2_block_addr[i])) begin
        s5_store_bypass[i] = 1'b1;
        s5_store_bypass_data[i*DcacheLramDataSize +: DcacheLramDataSize] =
            s3_req_to_s4.data[i*DcacheLramDataSize +: DcacheLramDataSize];
      end
    end
  end

  always_comb begin
    s3_core_resp_valid_o = s3_has_data || s3_reduce_op_inst;
    s3_core_resp_o = '0;
    s3_core_resp_o.dest = s3_req.dest;
    s3_core_resp_o.dest.fp = s3_core_resp_o.dest.fp || s3_reduce_op_inst;
    s3_core_resp_o.typ =
        (s3_reduce_op_inst || s3_is_gather32) ? minion_pkg::DcTypePs
                                              : minion_pkg::dcache_type_e'(s3_req.typ);
    s3_core_resp_o.gsc_cnt = s3_req.gsc_cnt;
    s3_core_resp_o.ps_mask = s3_req.ps_mask | {VpuLaneNum{s3_reduce_op_inst}};
    s3_core_resp_o.data = s3_load_data_mux2;
    s3_core_resp_o.data[0] |= s3_sc_fail;
    s3_core_resp_o.addr[minion_dcache_pkg::VaSize-1:0] = s3_req.addr[minion_dcache_pkg::VaSize-1:0];
    s3_core_resp_o.addr[minion_dcache_pkg::VaSize] = s3_req.addr_msb_err ^ s3_req.addr[minion_dcache_pkg::VaSize-1];

    for (int unsigned i = 0; i < NrThreads; i++) begin
      s3_ordered_o[i] =
          !(s0_valid_to_s1_ordered && (s0_req_thread_id_ordered == i[0]))
          && !(s1_valid && (s1_req.dest.thread_id == i[0]))
          && !(s2_valid && (s2_req.dest.thread_id == i[0]))
          && !(s3_valid && (s3_req.dest.thread_id == i[0]))
          && !(s4_valid && (s4_req.dest.thread_id == i[0]))
          && s2_mh_fence_rdy[i]
          && s2_rq_empty[i]
          && !dcache_cfg_stall;
    end

    dcache_idle_next =
        !s0_valid_to_s1_ordered && !s1_valid && !s2_valid && !s3_valid && !s4_valid
        && (&s2_rq_empty) && !tl0_working && !tl1_working && !cacheop_busy && !cacheop_l2_busy
        && mh_idle && wb_ready && reduce_ready && tx_idle;
    dcache_idle_excl_mode_next =
        !s0_valid_to_s1_ordered && !s1_valid && !s2_valid && !s3_valid && !s4_valid
        && (&s2_rq_empty) && !tl0_working && !cacheop_busy && !cacheop_l2_busy
        && mh_idle && wb_ready && reduce_ready && tx_idle;
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      dcache_idle <= 1'b0;
      dcache_idle_excl_mode_o <= 1'b0;
    end else begin
      dcache_idle <= dcache_idle_next;
      dcache_idle_excl_mode_o <= dcache_idle_excl_mode_next;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      vpu_scp_bid_1 <= 1'b0;
      vpu_scp_bid_2 <= 1'b0;
    end else begin
      vpu_scp_bid_1 <= VpuEn && s1_vpu_ctrl_i.scp_req.bid;
      vpu_scp_bid_2 <= vpu_scp_bid_1;
    end
  end

  always_comb begin
    id_dbg_nvpu_bid = id_dbg_bid && !vpu_scp_bid_2;
    id_wb_da_nvpu_bid = id_wb_da_bid && !vpu_scp_bid_2;

    id_co_mr_grant = !s0_core_gsc_i && !re_pipe_used && id_co_mr_bid;
    id_dbg_grant = !s0_core_gsc_i && !re_pipe_used && !id_co_mr_bid && id_dbg_nvpu_bid;
    id_wb_da_grant =
        !s0_core_gsc_i && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid && id_wb_da_nvpu_bid;
    id_rq_grant =
        !s0_core_gsc_i && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid
        && !id_wb_da_nvpu_bid && id_rq_replay_bid_qual;
    id_co_pf_grant =
        !s0_core_gsc_i && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid
        && !id_wb_da_nvpu_bid && !id_rq_replay_bid_qual && id_co_pf_bid;
    id_core_pipe_available =
        !dcache_cfg_stall && !re_pipe_used && !id_co_mr_bid && !id_dbg_nvpu_bid
        && !id_wb_da_nvpu_bid && !id_rq_replay_bid_qual && !id_co_pf_bid;

    id_core_ready =
        (re_ba_entries_required ? ~id_rq_half_full : ~id_rq_full)
        && ~id_ba_full && id_core_pipe_available && !re_ba_alloc && !s0_co_pf_valid;
  end

  minion_dcache_scratchpad_ctrl u_scratchpad_ctrl (
    .clk_i                (clk_i),
    .rst_ni               (rst_ni),
    .scratchpad_ctrl_i    (dcache_core_ctrl_mode),
    .dcache_idle_i        (dcache_idle),
    .dcache_cfg_stall_o   (dcache_cfg_stall),
    .dcache_cfg_clear_all_o(dcache_cfg_clear_all),
    .dcache_cfg_clear_low_o(dcache_cfg_clear_low),
    .dcache_cfg_alloc_scp_o(dcache_cfg_alloc_scp),
    .da_clear_en_o        (s3_cfg_da_clear_en),
    .da_clear_val_o       (s4_cfg_da_clear_val),
    .da_clear_set_o       (s4_cfg_da_clear_set),
    .da_clear_way_o       (s4_cfg_da_clear_way),
    .da_clear_idx_o       (s4_cfg_da_clear_idx),
    .da_clear_rdy_i       (s4_cfg_da_clear_rdy),
    .dcache_cfg_mode_o    (dcache_cfg_mode)
  );

  prim_arb_lru #(
    .NumClients(DcacheL2EvictArbPorts)
  ) u_l2_arb_to_evict (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (l2_evict_arb_bid),
    .stall_i  (!arb_l2_evict_ready),
    .grant_o  (l2_evict_arb_grant),
    .winner_o (l2_evict_arb_winner)
  );

  assign arb_l2_evict_valid = |l2_evict_arb_bid;
  assign arb_l2_evict_req = l2_evict_arb_data_in[l2_evict_arb_winner];

  for (genvar evict_it = 0; evict_it < DcacheL2EvictMuxPorts; evict_it++) begin : gen_l2_evict_pipe
    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        l2_evict_mux_valid[evict_it] <= 1'b0;
      end else if (l2_evict_pipe_valid_en[evict_it]) begin
        l2_evict_mux_valid[evict_it] <= l2_evict_pipe_valid[evict_it];
      end
    end

    always_ff @(posedge clk_i) begin
      if (l2_evict_pipe_data_en[evict_it]) begin
        l2_evict_mux_data_in[evict_it] <= l2_evict_pipe_data_in[evict_it];
      end
    end

    assign l2_evict_pipe_ready[evict_it] = l2_evict_mux_ready[evict_it] | ~l2_evict_mux_valid[evict_it];
    assign l2_evict_pipe_valid_en[evict_it] =
        l2_evict_pipe_ready[evict_it] & (l2_evict_mux_valid[evict_it] | l2_evict_pipe_valid[evict_it]);
    assign l2_evict_pipe_data_en[evict_it] = l2_evict_pipe_ready[evict_it] & l2_evict_pipe_valid[evict_it];
  end

  prim_arb_lru #(
    .NumClients(DcacheL2MissArbPorts)
  ) u_l2_arb_to_miss (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (l2_miss_arb_bid),
    .stall_i  (!arb_l2_miss_ready),
    .grant_o  (l2_miss_arb_grant),
    .winner_o (l2_miss_arb_winner)
  );

  assign arb_l2_miss_valid = |l2_miss_arb_bid;
  assign arb_l2_miss_req = l2_miss_arb_data_in[l2_miss_arb_winner];

  for (genvar miss_it = 0; miss_it < DcacheL2MissMuxPorts; miss_it++) begin : gen_l2_miss_pipe
    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        l2_miss_mux_valid[miss_it] <= 1'b0;
      end else if (l2_miss_pipe_valid_en[miss_it]) begin
        l2_miss_mux_valid[miss_it] <= l2_miss_pipe_valid[miss_it];
      end
    end

    always_ff @(posedge clk_i) begin
      if (l2_miss_pipe_data_en[miss_it]) begin
        l2_miss_mux_data_in[miss_it] <= l2_miss_pipe_data_in[miss_it];
      end
    end

    assign l2_miss_pipe_ready[miss_it] = l2_miss_mux_ready[miss_it] | ~l2_miss_mux_valid[miss_it];
    assign l2_miss_pipe_valid_en[miss_it] =
        l2_miss_pipe_ready[miss_it] & (l2_miss_mux_valid[miss_it] | l2_miss_pipe_valid[miss_it]);
    assign l2_miss_pipe_data_en[miss_it] = l2_miss_pipe_ready[miss_it] & l2_miss_pipe_valid[miss_it];
  end

  always_comb begin
    wb_arb_grant = '0;
    wb_arb_data_winner = wb_arb_data_in[2];
    if (!wb_arb_stall) begin
      if (wb_arb_bid[2]) begin
        wb_arb_grant[2] = 1'b1;
        wb_arb_data_winner = wb_arb_data_in[2];
      end else if (wb_arb_bid[1]) begin
        wb_arb_grant[1] = 1'b1;
        wb_arb_data_winner = wb_arb_data_in[1];
      end else if (wb_arb_bid[0]) begin
        wb_arb_grant[0] = 1'b1;
        wb_arb_data_winner = wb_arb_data_in[0];
      end else begin
        wb_arb_grant[2] = 1'b1;
      end
    end
  end

  always_comb begin
    wb_arb_bid[0] = co_wb_valid;
    wb_arb_data_in[0] = co_wb_req;
    co_wb_ready = wb_arb_grant[0];

    wb_arb_bid[1] = tx_wb_valid;
    wb_arb_data_in[1] = tx_wb_req;
    tx_wb_ready = wb_arb_grant[1];

    wb_arb_bid[2] = mh_wb_valid;
    wb_arb_data_in[2] = mh_wb_req;
    mh_wb_ready = wb_arb_grant[2];
  end

  always_comb begin
    wb_allow_evict_bid = wb_ready && !wb_valid;

    l2_evict_arb_bid = '0;
    l2_evict_arb_data_in = '{default: '0};
    wb_l2_evict_ready = l2_evict_arb_grant[DcacheL2EvictWbPort];
    mh_l2_evict_ready = l2_evict_arb_grant[DcacheL2EvictMhPort];
    re_l2_evict_ready = l2_evict_arb_grant[DcacheL2EvictRePort];
    pf_l2_evict_ready = l2_evict_arb_grant[DcacheL2EvictPfPort];

    l2_evict_arb_bid[DcacheL2EvictWbPort] = wb_l2_evict_valid;
    l2_evict_arb_data_in[DcacheL2EvictWbPort] = wb_l2_evict_req;

    l2_evict_arb_bid[DcacheL2EvictMhPort] =
        mh_l2_evict_valid && wb_allow_evict_bid && !re_ts_l2_evict_inflight;
    l2_evict_arb_data_in[DcacheL2EvictMhPort] = mh_l2_evict_req;
    l2_evict_arb_data_in[DcacheL2EvictMhPort].data = s1_ba_data;

    l2_evict_arb_bid[DcacheL2EvictRePort] = re_l2_evict_valid && wb_allow_evict_bid;
    l2_evict_arb_data_in[DcacheL2EvictRePort] = re_l2_evict_req;

    l2_evict_arb_bid[DcacheL2EvictPfPort] =
        pf_l2_evict_valid && wb_allow_evict_bid && !re_ts_l2_evict_inflight;
    l2_evict_arb_data_in[DcacheL2EvictPfPort] = pf_l2_evict_req;

    l2_evict_pipe_valid = '0;
    l2_evict_pipe_data_in = '{default: '0};
    ts_l2_evict_ready = l2_evict_pipe_ready[DcacheL2EvictIdxTstore];
    arb_l2_evict_ready = l2_evict_pipe_ready[DcacheL2EvictIdxComm];

    l2_evict_pipe_valid[DcacheL2EvictIdxTstore] = ts_l2_evict_valid;
    l2_evict_pipe_data_in[DcacheL2EvictIdxTstore] = ts_l2_evict_req;
    l2_evict_pipe_valid[DcacheL2EvictIdxComm] = arb_l2_evict_valid;
    l2_evict_pipe_data_in[DcacheL2EvictIdxComm] = arb_l2_evict_req;

    l2_miss_arb_bid = '0;
    l2_miss_arb_data_in = '{default: '0};
    tx_l2_miss_ready = l2_miss_arb_grant[DcacheL2MissTxPort];
    mh_l2_miss_ready = l2_miss_arb_grant[DcacheL2MissMhPort];
    tl0_l2_miss_ready_no_coop = l2_miss_arb_grant[DcacheL2MissTl0Port];
    tl1_l2_miss_ready_no_coop = l2_miss_arb_grant[DcacheL2MissTl1Port];

    l2_miss_arb_bid[DcacheL2MissTxPort] = tx_l2_miss_valid;
    l2_miss_arb_data_in[DcacheL2MissTxPort] = tx_l2_miss_req;
    l2_miss_arb_bid[DcacheL2MissMhPort] = mh_l2_miss_valid;
    l2_miss_arb_data_in[DcacheL2MissMhPort] = mh_l2_miss_req;
    l2_miss_arb_bid[DcacheL2MissTl0Port] = tl0_l2_miss_valid_no_coop;
    l2_miss_arb_data_in[DcacheL2MissTl0Port] = tl0_l2_miss_req;
    l2_miss_arb_bid[DcacheL2MissTl1Port] = tl1_l2_miss_valid_no_coop;
    l2_miss_arb_data_in[DcacheL2MissTl1Port] = tl1_l2_miss_req;

    l2_miss_pipe_valid = '0;
    l2_miss_pipe_data_in = '{default: '0};
    tl0_l2_miss_ready = l2_miss_pipe_ready[DcacheL2MissIdxScp];
    tl1_l2_miss_ready = l2_miss_pipe_ready[DcacheL2MissIdxTenb];
    arb_l2_miss_ready = l2_miss_pipe_ready[DcacheL2MissIdxComm];

    l2_miss_pipe_valid[DcacheL2MissIdxScp] = tl0_l2_miss_valid;
    l2_miss_pipe_data_in[DcacheL2MissIdxScp] = tl0_l2_miss_req;
    l2_miss_pipe_valid[DcacheL2MissIdxTenb] = tl1_l2_miss_valid;
    l2_miss_pipe_data_in[DcacheL2MissIdxTenb] = tl1_l2_miss_req;
    l2_miss_pipe_valid[DcacheL2MissIdxComm] = arb_l2_miss_valid;
    l2_miss_pipe_data_in[DcacheL2MissIdxComm] = arb_l2_miss_req;
  end

  always_comb begin
    l2_evict_mux_ready = '0;
    l2_evict_req_valid_o = '0;
    l2_evict_handshake = 1'b0;
    l2_evict_mux_select = '1;
    for (int unsigned i = 0; i < DcacheL2EvictMuxPorts; i++) begin
      if (!l2_evict_handshake) begin
        l2_evict_req_valid_o[i] = l2_evict_mux_valid[i] && l2_evict_req_ready_i[i];
        l2_evict_handshake = l2_evict_mux_valid[i] && l2_evict_req_ready_i[i];
        l2_evict_mux_ready[i] = l2_evict_req_ready_i[i] && l2_evict_mux_valid[i];
        l2_evict_mux_select = i[$clog2(DcacheL2EvictMuxPorts)-1:0];
      end
    end
    l2_evict_req_o = l2_evict_mux_data_in[l2_evict_mux_select];
  end

  always_comb begin
    l2_miss_mux_ready = '0;
    l2_miss_req_valid_o = '0;
    l2_miss_handshake = 1'b0;
    l2_miss_mux_select = '1;
    for (int unsigned i = 0; i < DcacheL2MissMuxPorts; i++) begin
      if (!l2_miss_handshake) begin
        l2_miss_req_valid_o[i] = l2_miss_mux_valid[i] && l2_miss_req_ready_i[i];
        l2_miss_handshake = l2_miss_mux_valid[i] && l2_miss_req_ready_i[i];
        l2_miss_mux_ready[i] = l2_miss_req_ready_i[i] && l2_miss_mux_valid[i];
        l2_miss_mux_select = i[$clog2(DcacheL2MissMuxPorts)-1:0];
      end
    end
    l2_miss_req_o = l2_miss_mux_data_in[l2_miss_mux_select];
  end

  always_comb begin
    l2_resp_is_port_data = 1'b0;
    l2_fill_slot_avail =
        !s4_valid && !mh_l2_resp_stall[l2_resp_int.id[$clog2(DcacheMhFileSize)-1:0]];

    l2_resp_is_for_tensor_0 = VpuEn && (l2_resp_int.opcode == EtLinkRspAckData)
        && ((et_link_core_miss_id_e'(l2_resp_int.id) >= EtLinkCoreMissIdTensorLoadExt0)
            && (et_link_core_miss_id_e'(l2_resp_int.id) <= EtLinkCoreMissIdTensorLoadExt3));
    l2_resp_is_for_tensor_1 = VpuEn && (l2_resp_int.opcode == EtLinkRspAckData)
        && ((et_link_core_miss_id_e'(l2_resp_int.id) >= EtLinkCoreMissIdTensorLoadExt4)
            && (et_link_core_miss_id_e'(l2_resp_int.id) <= EtLinkCoreMissIdTensorLoadExt7));
    l2_resp_is_for_mh_fill = (l2_resp_int.opcode == EtLinkRspAckData)
        && ((et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandler0)
            || (et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandler1));
    l2_resp_is_for_mh_uc = ((l2_resp_int.opcode == EtLinkRspAckData) || (l2_resp_int.opcode == EtLinkRspErr))
        && ((et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcLd0)
            || (et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcLd1)
            || (et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcSt0)
            || (et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcSt1));
    l2_resp_is_for_mh_uc_ok = (l2_resp_int.opcode == EtLinkRspAckData)
        && ((et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcLd0)
            || (et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcLd1)
            || (et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcSt0)
            || (et_link_core_miss_id_e'(l2_resp_int.id) == EtLinkCoreMissIdMissHandlerUcSt1));
    l2_resp_is_reduce_data = VpuEn && (l2_resp_int.opcode == EtLinkRspMsgRcvData)
        && (et_link_min_msg_id_e'(l2_resp_int.id) == EtLinkMinMsgIdReduceData);
    l2_resp_is_reduce_ready = VpuEn && (l2_resp_int.opcode == EtLinkRspMsgRcvData)
        && (et_link_min_msg_id_e'(l2_resp_int.id) == EtLinkMinMsgIdReduceReady);
    l2_resp_is_for_reduce = l2_resp_is_reduce_data || l2_resp_is_reduce_ready;

    l2_da_write = 1'b0;
    if (l2_resp_is_for_mh_fill) begin
      l2_fill_ready = l2_fill_slot_avail;
      l2_da_write = l2_fill_ready;
    end else if (l2_resp_is_reduce_data) begin
      l2_fill_ready = !s3_ba_write_en && re_buffer_avail && !l2_resp_ready_blocked;
    end else if (l2_resp_is_reduce_ready) begin
      l2_fill_ready = !l2_resp_ready_blocked;
    end else if (l2_resp_is_for_mh_uc) begin
      l2_fill_ready = !s3_ba_write_en;
    end else if (l2_resp_int.opcode == EtLinkRspMsgRcvData) begin
      l2_fill_ready = !s4_valid && !l2_resp_ready_blocked;
      l2_da_write = l2_fill_ready && core_ctrl_i.msg_port_wr_en;
      l2_resp_is_port_data = 1'b1;
    end else if (l2_resp_is_for_tensor_0) begin
      l2_fill_ready = !s4_valid;
      l2_da_write = !s4_tl0_load_capture;
    end else if (l2_resp_is_for_tensor_1) begin
      l2_fill_ready = s4_tl1_tenb_ready_reg && !l2_resp_ready_blocked;
    end else begin
      l2_fill_ready = 1'b1;
    end
  end

  assign l2_resp_count_limit = (l2_resp_int.size == EtLinkLine) ? 2'd2 : 2'd1;
  assign l2_resp_count_d = (l2_resp_count_q == l2_resp_count_limit) ? 2'd0 : (l2_resp_count_q + 2'd1);
  assign l2_resp_ready_block_op =
      l2_resp_valid_int && (((l2_resp_int.opcode == EtLinkRspMsgRcvData) && !l2_resp_is_for_reduce)
                            || l2_resp_is_for_tensor_1);
  assign l2_resp_ready_blocked = l2_resp_ready_block_op && (l2_resp_count_q == 2'b0);
  assign l2_resp_count_en =
      l2_resp_ready_blocked || (l2_resp_valid_int && l2_resp_ready_int && (l2_resp_count_q != 2'd0));
  assign l2_resp_ready_int = l2_fill_ready;

  always_comb begin
    core_ctrl_resp = '0;
    cache_op_busy_changed = '0;
    tensor_reduce_mem_err_flag =
        VpuEn ? tensor_reduce_err_all_flags[DcacheErrFlagMemFault] : '0;
    tensor_load_mem_err_flags[1] =
        VpuEn
            ? (tensor_load_err_flags_0[DcacheTlErrorAccessFault1]
               | tensor_load_err_flags_1[DcacheTlErrorAccessFault1])
            : 1'b0;
    tensor_load_mem_err_flags[0] =
        VpuEn
            ? (tensor_load_err_flags_0[DcacheTlErrorAccessFault0]
               | tensor_load_err_flags_1[DcacheTlErrorAccessFault0])
            : 1'b0;
    cache_ops_err_flags = cache_ops_err_flags_l1;

    core_ctrl_resp.cache_op_ready = (core_co_l2_pref_ready || core_cache_op_ready) & !dcache_cfg_stall;

    for (int unsigned i = 0; i < NrThreads; i++) begin
      cache_op_busy_changed[i] =
          (!cacheop_busy && cacheop_busy_prev && (i[0] == s0_co_mr_thread_id))
          || (!cacheop_l2_busy && cacheop_l2_busy_prev && (i[0] == s0_co_l2_thread_id))
          || (!dcache_cfg_stall && dcache_cfg_stall_prev);
    end

    core_ctrl_resp.cache_op_busy_changed =
        {NrThreads{|cache_op_busy_changed}} & ~cache_op_busy_changed
        | {NrThreads{|cache_op_busy_changed_z1}} & cache_op_busy_changed_z1;
    core_ctrl_resp.cache_op_complete_l1 = co_l1_th_id_complete;
    core_ctrl_resp.cache_op_complete_l2 = co_l2_th_id_complete;
    core_ctrl_resp.tl_ready_0 = VpuEn ? (!tl0_working & !dcache_cfg_stall) : 1'b1;
    core_ctrl_resp.tl_ready_1 = VpuEn ? (!tl1_working & !dcache_cfg_stall) : 1'b1;
    core_ctrl_resp.tl_complete_0 = VpuEn ? tl0_id_complete : '1;
    core_ctrl_resp.ts_ready =
        VpuEn ? (reduce_ready ? (!tensorstore_busy & !dcache_cfg_stall) : 1'b0) : 1'b1;
    core_ctrl_resp.reduce_ready =
        VpuEn ? (tensorstore_busy ? 1'b0 : (reduce_ready & !dcache_cfg_stall)) : 1'b1;
    core_ctrl_resp.tex_send_ready = VpuEn ? (tex_send_ready & !dcache_cfg_stall) : 1'b1;
    core_ctrl_resp.gsc_progress_save = '0;
    core_ctrl_resp.gsc_progress_save[s2_req.dest.thread_id] = s2_save_progress || s2_clear_progress;
    core_ctrl_resp.gsc_progress = (s2_clear_progress && !s2_save_progress) ? '0 : s2_req.gsc_cnt;

    for (int unsigned i = 0; i < NrThreads; i++) begin
      cache_ops_err_flags[DcacheErrFlagMemFault][i] |=
          cache_ops_err_flags_l2[i] | tensor_reduce_mem_err_flag[i] | tensor_load_mem_err_flags[i];
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      cacheop_busy_prev <= 1'b0;
      cacheop_l2_busy_prev <= 1'b0;
      dcache_cfg_stall_prev <= 1'b0;
      cache_op_busy_changed_z1 <= '0;
      l2_resp_count_q <= '0;
    end else begin
      cacheop_busy_prev <= cacheop_busy;
      cacheop_l2_busy_prev <= cacheop_l2_busy;
      dcache_cfg_stall_prev <= dcache_cfg_stall;
      cache_op_busy_changed_z1 <= cache_op_busy_changed;
      if (l2_resp_count_en) begin
        l2_resp_count_q <= l2_resp_count_d;
      end
      if (l2_resp_valid_int) begin
        s4_tl1_tenb_ready_reg <= s4_tl1_tenb_ready;
      end
    end
  end

  assign wb_arb_stall = !wb_ready || re_ts_l2_evict_inflight;
  assign wb_valid = (|wb_arb_bid) && !re_ts_l2_evict_inflight;

  minion_dcache_writeback_unit u_writeback_unit (
    .clk_i          (clk_i),
    .rst_ni         (rst_ni),
    .req_ready_o    (wb_ready),
    .req_valid_i    (wb_valid),
    .req_data_i     (wb_arb_data_winner),
    .data_req_bid_o (id_wb_da_bid),
    .data_req_grant_i(id_wb_da_grant),
    .data_req_valid_o(s0_wb_da_valid),
    .data_req_o     (s0_wb_da_req),
    .data_req_nack_i(s1_wb_da_nack),
    .data_resp_i    (s3_data),
    .l2_req_ready_i (wb_l2_evict_ready),
    .l2_req_valid_o (wb_l2_evict_valid),
    .l2_req_data_o  (wb_l2_evict_req)
  );

  minion_dcache_cache_op_unit u_cache_op_unit (
    .clk_i                 (clk_i),
    .rst_ni                (rst_ni),
    .chicken_bit_dcache_i  (chicken_bit_dcache_i),
    .busy_o                (cacheop_busy),
    .cache_op_ready_o      (core_cache_op_ready),
    .cache_op_valid_i      (core_cache_op_valid),
    .cache_op_req_i        (core_ctrl_i.cache_op_req),
    .cache_op_req_next_i   (core_ctrl_i.cache_op_req_next),
    .th_id_complete_o      (co_l1_th_id_complete),
    .meta_read_bid_o       (id_co_mr_bid),
    .meta_read_grant_i     (id_co_mr_grant),
    .meta_read_valid_o     (s0_co_mr_valid),
    .meta_read_phys_o      (s0_co_mr_phys),
    .meta_read_force_set_o (s0_co_mr_force_set),
    .meta_read_thread_id_o (s0_co_mr_thread_id),
    .meta_read_skip_pma_o  (s1_co_mr_skip_pma),
    .meta_read_addr_o      (s0_co_mr_addr),
    .meta_read_addr_msb_err_o(s0_co_mr_addr_msb_err),
    .meta_read_resp_way_en_i(s2_tag_match),
    .meta_read_resp_i      (s2_meta_read),
    .s1_phys_addr_tag_i    (s1_phys_addr_tag),
    .s1_tlb_wait_i         (s1_tlb_wait),
    .s1_tlb_fail_i         (s1_tlb_fail),
    .meta_write_ready_i    (s1_co_mw_grant),
    .meta_write_valid_next_o(s1_co_mw_valid_early),
    .meta_write_valid_o    (s1_co_mw_valid),
    .meta_write_req_o      (s1_co_mw_req),
    .nack_set_valid_o      (s1_co_nack_set_valid),
    .nack_set_value_o      (s1_co_nack_set_value),
    .writeback_ready_i     (co_wb_ready),
    .writeback_valid_o     (co_wb_valid),
    .writeback_req_o       (co_wb_req),
    .writeback_done_i      (wb_ready),
    .id_rq_full_i          (id_rq_full),
    .id_co_pf_bid_o        (id_co_pf_bid),
    .id_co_pf_grant_i      (id_co_pf_grant),
    .s0_co_pf_valid_o      (s0_co_pf_valid),
    .s0_co_pf_req_o        (s0_co_pf_req),
    .cache_op_ready_byp_i  (cache_op_ready_byp),
    .cache_op_valid_byp_o  (cache_op_valid_byp),
    .cache_op_req_byp_o    (cache_op_req_byp),
    .mh_clear_slock_valid_i(mh_clear_slock_valid),
    .mh_clear_slock_set_i  (mh_clear_slock_set),
    .mh_clear_slock_way_i  (mh_clear_slock_way),
    .hlock_state_o         (hlock_state),
    .slock_state_o         (slock_state),
    .da_clear_en_o         (s3_co_da_clear_en),
    .da_clear_val_o        (s4_co_da_clear_val),
    .da_clear_set_o        (s4_co_da_clear_set),
    .da_clear_way_o        (s4_co_da_clear_way),
    .da_clear_idx_o        (s4_co_da_clear_idx),
    .da_clear_rdy_i        (s4_co_da_clear_rdy),
    .mh_probe_rdy_i        (mh_probe_rdy),
    .mh_metaw_rdy_i        (mh_metaw_rdy),
    .rq_conflict_i         (rq_conflict_to_co),
    .cfg_clear_all_i       (dcache_cfg_clear_all),
    .cfg_clear_low_i       (dcache_cfg_clear_low),
    .cfg_alloc_scp_i       (dcache_cfg_alloc_scp),
    .cfg_cache_mode_i      (dcache_cfg_mode),
    .vm_status_i           (vm_status_i),
    .vm_status_co_o        (vm_status_co),
    .err_flags_o           (cache_ops_err_flags_l1),
    .csr_debug_signals_o   (co_csr_debug_signals),
    .sm_match_debug_signals_o(co_sm_match_debug_signals),
    .sm_filter_debug_signals_o(co_sm_filter_debug_signals),
    .sm_data_debug_signals_o(co_sm_data_debug_signals)
  );

  minion_dcache_cache_op_unit_l2 u_cache_op_unit_l2 (
    .clk_i                 (clk_i),
    .rst_ni                (rst_ni),
    .shire_id_i            (shire_id_i),
    .cacheop_max_i         (core_ctrl_i.cache_ctrl.cacheop_max),
    .cacheop_reprate_i     (core_ctrl_i.cache_ctrl.cacheop_reprate),
    .chicken_bit_dcache_i  (chicken_bit_dcache_i),
    .busy_o                (cacheop_l2_busy),
    .th_id_complete_o      (co_l2_th_id_complete),
    .l2_resp_valid_i       (l2_resp_valid_to_co_l2),
    .l2_resp_i             (l2_resp_int),
    .cache_op_ready_o      (core_co_l2_pref_ready),
    .cache_op_valid_i      (core_co_l2_pref_valid),
    .cache_op_req_i        (core_ctrl_i.cache_op_req),
    .cache_op_req_next_i   (core_ctrl_i.cache_op_req_next),
    .l2_req_ready_i        (pf_l2_evict_ready),
    .l2_req_valid_o        (pf_l2_evict_valid),
    .l2_req_data_o         (pf_l2_evict_req),
    .cache_op_ready_byp_o  (cache_op_ready_byp),
    .cache_op_valid_byp_i  (cache_op_valid_byp),
    .cache_op_req_byp_i    (cache_op_req_byp),
    .s0_available_i        (s0_available_to_co_l2),
    .s0_addr_load_o        (s0_co_l2_addr_load),
    .s0_addr_o             (s0_co_l2_addr),
    .s0_addr_msb_err_o     (s0_co_l2_addr_msb_err),
    .s0_thread_id_o        (s0_co_l2_thread_id),
    .s1_available_i        (s1_available_to_co_l2),
    .s1_addr_load_o        (s1_co_l2_addr_load),
    .s1_tlb_wait_i         (s1_tlb_wait),
    .s1_tlb_fail_i         (s1_tlb_fail),
    .s1_tlb_addr_i         (s1_tlb_resp_data.ppn),
    .vm_status_i           (vm_status_i),
    .vm_status_co_i        (vm_status_co),
    .vm_status_l2_o        (vm_status_l2),
    .err_flags_o           (cache_ops_err_flags_l2),
    .bus_err_o             (bus_err_co_l2),
    .csr_debug_signals_o   (l2_csr_debug_signals),
    .sm_match_debug_signals_o(l2_sm_match_debug_signals),
    .sm_filter_debug_signals_o(l2_sm_filter_debug_signals),
    .sm_data_debug_signals_o(l2_sm_data_debug_signals)
  );

  if (VpuEn) begin : gen_offload
    /* verilator lint_off PINCONNECTEMPTY */  // These TL ports are intentionally unused at top level in the same places as the original integration.
    minion_dcache_tensor_load #(
      .ModuleIdx(0)
    ) u_tensor_load_0 (
      .clk_i                 (clk_i),
      .rst_ni                (rst_ni),
      .shire_min_id_i        (shire_min_id_i),
      .chicken_bit_dcache_i  (chicken_bit_dcache_i),
      .busy_o                (tl0_working),
      .id_complete_o         (tl0_id_complete),
      .tensor_ctrl_i         (core_ctrl_i),
      .s0_available_i        (s0_available_to_tl0),
      .s0_addr_load_o        (s0_tl0_addr_load),
      .s0_addr_o             (s0_tl0_addr),
      .s0_addr_msb_err_o     (s0_tl0_addr_msb_err),
      .s0_thread_id_o        (s0_tl0_thread_id),
      .s1_available_i        (s1_available_to_tl0),
      .s1_addr_load_o        (s1_tl0_addr_load),
      .s1_tlb_wait_i         (s1_tlb_wait),
      .s1_tlb_fail_i         (s1_tlb_fail),
      .s2_addr_i             (s2_req_addr[minion_dcache_pkg::PaSize-1:0]),
      .l2_req_ready_coop_i   (tl0_l2_miss_ready),
      .l2_req_valid_coop_o   (tl0_l2_miss_valid),
      .l2_req_ready_no_coop_i(tl0_l2_miss_ready_no_coop),
      .l2_req_valid_no_coop_o(tl0_l2_miss_valid_no_coop),
      .l2_req_o              (tl0_l2_miss_req),
      .l2_resp_valid_i       (l2_resp_valid_ready_int),
      .l2_resp_i             (l2_resp_int),
      .load_way_o            (s4_tl0_load_way),
      .load_addr_o           (s4_tl0_load_addr),
      .load_data_o           (s4_tl0_load_data),
      .load_valid_o          (s4_tl0_load_valid),
      .load_valid_pre_o      (s3_tl0_load_valid_pre),
      .load_ready_i          (s4_tl0_load_ready),
      .load_capture_o        (s4_tl0_load_capture),
      .tfma_enabled_i        (1'b0),
      .tenb_tstore_busy_i    (1'b0),
      .tenb_credit_i         (1'b0),
      .tenb_credit_entry_i   ('0),
      .tenb_written_i        (1'b0),
      .tenb_ready_o          (),
      .tenb_flush_o          (),
      .vm_status_i           (vm_status_i),
      .vm_status_tl_o        (vm_status_tl0),
      .err_flags_o           (tensor_load_err_flags_0),
      .tl_start_o            (tl0_start),
      .tl_op_o               (tl0_op),
      .bus_err_o             (bus_err_tl0),
      .csr_debug_signals_o   (tl0_csr_debug_signals),
      .sm_match_debug_signals_o(tl0_sm_match_debug_signals),
      .sm_filter_debug_signals_o(tl0_sm_filter_debug_signals),
      .sm_data_debug_signals_o(tl0_sm_data_debug_signals)
    );

    minion_dcache_tensor_load #(
      .ModuleIdx(1)
    ) u_tensor_load_1 (
      .clk_i                 (clk_i),
      .rst_ni                (rst_ni),
      .shire_min_id_i        (shire_min_id_i),
      .chicken_bit_dcache_i  (chicken_bit_dcache_i),
      .busy_o                (tl1_working),
      .id_complete_o         (tl1_id_complete),
      .tensor_ctrl_i         (core_ctrl_i),
      .s0_available_i        (s0_available_to_tl1),
      .s0_addr_load_o        (s0_tl1_addr_load),
      .s0_addr_o             (s0_tl1_addr),
      .s0_addr_msb_err_o     (s0_tl1_addr_msb_err),
      .s0_thread_id_o        (s0_tl1_thread_id),
      .s1_available_i        (s1_available_to_tl1),
      .s1_addr_load_o        (s1_tl1_addr_load),
      .s1_tlb_wait_i         (s1_tlb_wait),
      .s1_tlb_fail_i         (s1_tlb_fail),
      .s2_addr_i             (s2_req_addr[minion_dcache_pkg::PaSize-1:0]),
      .l2_req_ready_coop_i   (tl1_l2_miss_ready),
      .l2_req_valid_coop_o   (tl1_l2_miss_valid),
      .l2_req_ready_no_coop_i(tl1_l2_miss_ready_no_coop),
      .l2_req_valid_no_coop_o(tl1_l2_miss_valid_no_coop),
      .l2_req_o              (tl1_l2_miss_req),
      .l2_resp_valid_i       (l2_resp_valid_ready_int),
      .l2_resp_i             (l2_resp_int),
      .load_way_o            (),
      .load_addr_o           (s4_tl1_load_addr),
      .load_data_o           (),
      .load_valid_o          (),
      .load_valid_pre_o      (),
      .load_ready_i          (1'b0),
      .load_capture_o        (),
      .tfma_enabled_i        (s1_vpu_ctrl_i.tfma_enabled),
      .tenb_tstore_busy_i    (tenb_tstore_busy),
      .tenb_credit_i         (s1_vpu_ctrl_i.tenb_credit),
      .tenb_credit_entry_i   (s1_vpu_ctrl_i.tenb_credit_entry),
      .tenb_written_i        (s4_tenb_written),
      .tenb_ready_o          (s4_tl1_tenb_ready),
      .tenb_flush_o          (tenb_flush),
      .vm_status_i           (vm_status_i),
      .vm_status_tl_o        (vm_status_tl1),
      .err_flags_o           (tensor_load_err_flags_1),
      .tl_start_o            (tl1_start),
      .tl_op_o               (tl1_op),
      .bus_err_o             (bus_err_tl1),
      .csr_debug_signals_o   (tl1_csr_debug_signals),
      .sm_match_debug_signals_o(tl1_sm_match_debug_signals),
      .sm_filter_debug_signals_o(tl1_sm_filter_debug_signals),
      .sm_data_debug_signals_o(tl1_sm_data_debug_signals)
    );
    /* verilator lint_on PINCONNECTEMPTY */
  end else begin : gen_no_offload_tl
    assign tl0_working = 1'b0;
    assign tl1_working = 1'b0;
    assign tl0_id_complete = '1;
    assign tl1_id_complete = '1;
    assign s0_tl0_addr_load = 1'b0;
    assign s0_tl0_addr = '0;
    assign s0_tl0_addr_msb_err = 1'b0;
    assign s0_tl0_thread_id = 1'b0;
    assign s1_tl0_addr_load = 1'b0;
    assign s0_tl1_addr_load = 1'b0;
    assign s0_tl1_addr = '0;
    assign s0_tl1_addr_msb_err = 1'b0;
    assign s0_tl1_thread_id = 1'b0;
    assign s1_tl1_addr_load = 1'b0;
    assign tl0_l2_miss_valid = 1'b0;
    assign tl0_l2_miss_valid_no_coop = 1'b0;
    assign tl1_l2_miss_valid = 1'b0;
    assign tl1_l2_miss_valid_no_coop = 1'b0;
    assign tl0_l2_miss_req = '0;
    assign tl1_l2_miss_req = '0;
    assign s4_tl0_load_way = '0;
    assign s4_tl0_load_addr = '0;
    assign s4_tl0_load_data = '0;
    assign s4_tl0_load_valid = '0;
    assign s3_tl0_load_valid_pre = 1'b0;
    assign s4_tl0_load_capture = 1'b0;
    assign s4_tl1_load_addr = '0;
    assign s4_tl1_tenb_ready = 1'b0;
    assign tenb_flush = 1'b0;
    assign vm_status_tl0 = '0;
    assign vm_status_tl1 = '0;
    assign tensor_load_err_flags_0 = '0;
    assign tensor_load_err_flags_1 = '0;
    assign tl0_start = 1'b0;
    assign tl0_op = 1'b0;
    assign tl1_start = 1'b0;
    assign tl1_op = 1'b0;
    assign bus_err_tl0 = '0;
    assign bus_err_tl1 = '0;
    assign tl0_csr_debug_signals = '0;
    assign tl1_csr_debug_signals = '0;
    assign tl0_sm_match_debug_signals = '0;
    assign tl0_sm_filter_debug_signals = '0;
    assign tl0_sm_data_debug_signals = '0;
    assign tl1_sm_match_debug_signals = '0;
    assign tl1_sm_filter_debug_signals = '0;
    assign tl1_sm_data_debug_signals = '0;
  end

  minion_dcache_miss_handler_unit u_miss_handler_unit (
    .clk_i                     (clk_i),
    .rst_ni                    (rst_ni),
    .neigh_min_id_i            (shire_min_id_i[$clog2(ShireCoopMinMaskSize)-1:0]),
    .req_ready_o               (s2_mh_req_ready),
    .req_valid_i               (s2_mh_req_valid),
    .req_info_i                (s2_mh_req_info),
    .req_assigned_o            (s2_mh_req_assigned),
    .req_accepted_o            (s2_mh_req_accepted),
    .can_accept_uc_o           (s2_mh_can_accept_uc),
    .wait_l2_to_ba_resp_o      (mh_wait_l2_to_ba_resp),
    .inflight_reduce_i         (re_ts_l2_evict_inflight),
    .going_to_uc_store_o       (mh_going_to_uc_store),
    .l2_evict_ready_i          (mh_l2_evict_ready),
    .l2_evict_valid_o          (mh_l2_evict_valid),
    .l2_evict_req_o            (mh_l2_evict_req),
    .l2_miss_ready_i           (mh_l2_miss_ready),
    .l2_miss_valid_o           (mh_l2_miss_valid),
    .l2_miss_req_o             (mh_l2_miss_req),
    .l2_resp_valid_i           (l2_resp_valid_ready_int),
    .l2_resp_data_i            (l2_resp_int),
    .l2_resp_slot_avail_i      (l2_fill_slot_avail),
    .l2_resp_stall_o           (mh_l2_resp_stall),
    .refill_way_o              (s4_mh_refill_way),
    .refill_addr_o             (s4_mh_refill_addr),
    .meta_write_ready_i        (s1_mh_mw_grant),
    .meta_write_valid_next_o   (s1_mh_mw_valid_early),
    .meta_write_valid_o        (s1_mh_mw_valid),
    .meta_write_clear_slock_o  (s1_mh_mw_clear_slock),
    .meta_write_req_o          (s1_mh_mw_req),
    .writeback_ready_i         (mh_wb_ready),
    .writeback_valid_o         (mh_wb_valid),
    .writeback_req_o           (mh_wb_req),
    .writeback_done_i          (wb_ready),
    .force_ba_rptr_o           (s1_mh_force_ba_rptr),
    .force_ba_rptr_qual_o      (s1_mh_force_ba_rptr_qual),
    .ba_rptr_o                 (s1_mh_ba_rptr),
    .ba_is_amo_o               (s1_mh_ba_is_amo),
    .ba_wptr_o                 (s3_mh_ba_wptr),
    .idle_o                    (mh_idle),
    .probe_rdy_o               (mh_probe_rdy),
    .metaw_rdy_o               (mh_metaw_rdy),
    .fence_rdy_o               (s2_mh_fence_rdy),
    .handler_ready_o           (s2_mh_handler_ready),
    .handler_fill_bus_error_o  (mh_fill_bus_error),
    .handler_uc_load_bus_error_o(mh_uc_load_bus_error),
    .match_miss_line_o         (s2_mh_match_miss_line),
    .bus_err_o                 (bus_err_mh),
    .bus_err_addr_o            (bus_err_addr_mh),
    .csr_debug_signals_o       (mh_csr_debug_signals),
    .sm_match_debug_signals_o  (mh_sm_match_debug_signals),
    .sm_filter_debug_signals_o (mh_sm_filter_debug_signals),
    .sm_data_debug_signals_o   (mh_sm_data_debug_signals)
  );

  assign mh_clear_slock_valid = s1_mh_mw_valid && s1_mh_mw_clear_slock;
  assign mh_clear_slock_set = s1_mh_mw_req.set_idx;
  assign mh_clear_slock_way = dcache_way_encode(s1_mh_mw_req.way_en);
  assign id_rq_alloc_req_pre = id_core_alloc_rq_pre_i || id_co_pf_bid;
  assign s0_rq_alloc_req_val = s0_core_alloc_rq_val_i || s0_co_pf_valid;
  assign id_rq_alloc_gsc = id_co_pf_bid ? 1'b0 : id_core_gsc_i;

  minion_dcache_replay_queue u_replay_queue (
    .clk_i                (clk_i),
    .rst_ni               (rst_ni),
    .full_o               (id_rq_full),
    .half_full_o          (id_rq_half_full),
    .empty_o              (s2_rq_empty),
    .alloc_req_pre_i      (id_rq_alloc_req_pre),
    .alloc_req_val_i      (s0_rq_alloc_req_val),
    .alloc_gsc_i          (id_rq_alloc_gsc),
    .push_req_i           (s2_rq_push),
    .push_req_prev_i      (s1_rq_push_conservative),
    .req_data_i           (s2_req),
    .cacheable_i          (s2_req.cacheable),
    .req_buf_alloc_i      (s2_ba_write_needed_core),
    .req_buf_entry_i      (s2_ba_write_entry),
    .req_conf_type_i      (s2_rq_conf_type),
    .misaligned_1st_i     (s2_nack_misaligned),
    .uc_load_1st_i        (s2_uc_load_1st),
    .uc_load_ok_i         (s2_uc_load_ok),
    .miss_fill_ok_i       (s2_miss_fill_ok),
    .rq_conflict_o        (s2_nack_rq_conflict),
    .rq_conflict_to_co_o  (rq_conflict_to_co),
    .rearm_i              (s2_rq_renack),
    .rearm_prev_i         (s1_rq_renack_conservative),
    .rearm_entry_i        (s2_req.replay_entry),
    .dealloc_i            (s2_rq_dealloc),
    .dealloc_entry_i      (s2_req.replay_entry),
    .miss_available_i     (s2_mh_handler_ready),
    .miss_fill_error_i    (mh_fill_bus_error),
    .uc_load_fill_i       (rq_uc_load_fill),
    .uc_load_fill_id_i    (rq_uc_load_fill_id),
    .uc_load_fill_ok_i    (rq_uc_load_fill_ok),
    .vpu_rf_in_use_i      (s1_vpu_ctrl_i.tfma_rf_write),
    .replay_bid_o         (id_rq_replay_bid),
    .replay_grant_i       (id_rq_grant),
    .replay_valid_o       (s0_rq_replay_valid),
    .replay_req_o         (s0_rq_replay),
    .replay_entry_o       (s0_rq_replay_entry),
    .replay_age_o         (s0_rq_replay_age),
    .replay_uc_ok_o       (s0_rq_replay_uc_load_ok),
    .replay_miss_ok_o     (s0_rq_replay_miss_fill_ok),
    .scoreboard_o         (id_core_scoreboard_o),
    .fp_sb_dealloc_o      (id_core_sb_fp_dealloc_o),
    .int_sb_dealloc_o     (id_core_sb_int_dealloc_o)
  );

  minion_dcache_buffer_array u_buffer_array (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .full_o                 (id_ba_full),
    .reduce_full_o          (re_ba_full),
    .alloc_req_pre_i        (id_core_alloc_rq_pre_i),
    .alloc_req_val_i        (s0_core_alloc_rq_val_i),
    .alloc_gsc_i            (id_core_gsc_i),
    .reduce_alloc_req_i     (re_ba_alloc),
    .s2_write_needed_core_i (s2_ba_write_needed_core),
    .s2_write_needed_i      (s2_ba_write_needed),
    .s2_write_en_i          (s2_ba_write_en),
    .s2_write_replay_i      (s2_ba_replay),
    .s2_write_replay_entry_i(s2_req.buf_id),
    .s3_write_data_i        (s3_ba_write_data),
    .s2_write_entry_o       (s2_ba_write_entry),
    .s3_write_en_o          (s3_ba_write_en),
    .s3_write_en_ext_i      (s3_ba_write_en_ext),
    .s2_write_en_ext_prev_i (s2_ba_write_en_ext_prev),
    .s3_write_entry_ext_i   (s3_ba_write_addr_ext),
    .s3_write_data_ext_i    (s3_ba_write_data_ext),
    .s2_dealloc_i           (s2_ba_dealloc),
    .s2_dealloc_entry_i     (s2_req.buf_id),
    .s1_dealloc_uc_i        (s1_ba_dealloc_uc),
    .s1_dealloc_uc_entry_i  (s1_ba_rptr),
    .reduce_dealloc_req_i   (re_ba_dealloc),
    .reduce_dealloc_entry_i (s1_ba_rptr),
    .s1_read_entry_i        (s1_ba_rptr),
    .s1_read_data_o         (s1_ba_data)
  );

  if (VpuEn) begin : gen_offload_reduce
    minion_dcache_reduce u_reduce (
      .clk_i                 (clk_i),
      .rst_ni                (rst_ni),
      .global_min_id_i       ({shire_id_i, shire_min_id_i}),
      .chicken_bit_dcache_i  (chicken_bit_dcache_i),
      .reduce_ready_o        (reduce_ready),
      .reduce_start_i        (reduce_start),
      .tensorstore_busy_o    (tensorstore_busy),
      .tensorstore_start_i   (tensor_store_start),
      .tensorstore_stride_i  (core_ctrl_i.tensorstore_stride),
      .reduce_tensorstore_ctrl_i(core_ctrl_i.reduce_tensorstore_ctrl),
      .thread_id_i           (core_ctrl_i.thread_id),
      .l2_resp_valid_i       (l2_resp_valid_ready_int),
      .l2_resp_i             (l2_resp_int),
      .buffer_avail_o        (re_buffer_avail),
      .re_l2_req_ready_i     (re_l2_evict_ready),
      .re_l2_req_valid_o     (re_l2_evict_valid),
      .re_l2_req_o           (re_l2_evict_req),
      .ts_l2_req_ready_i     (ts_l2_evict_ready),
      .ts_l2_req_valid_o     (ts_l2_evict_valid),
      .ts_l2_req_o           (ts_l2_evict_req),
      .re_ts_l2_req_inflight_o(re_ts_l2_evict_inflight),
      .pipe_used_o           (re_pipe_used),
      .s1_inst_send_val_o    (s1_reduce_send_inst),
      .s2_inst_send_val_o    (s2_reduce_send_inst),
      .s3_inst_send_val_o    (s3_reduce_send_inst),
      .s1_inst_op_val_o      (s1_reduce_op_inst),
      .s2_inst_op_val_o      (s2_reduce_op_inst),
      .s3_inst_op_val_o      (s3_reduce_op_inst),
      .ts_scp_read_o         (ts_scp_read),
      .ts_scp_read_way_o     (ts_scp_read_way),
      .ts_scp_read_addr_o    (ts_scp_read_addr),
      .ts_col_size_is_128b_o (ts_col_size_is_128b),
      .s0_available_i        (s0_available_to_ts),
      .s0_addr_load_o        (s0_ts_addr_load),
      .s0_addr_o             (s0_ts_addr),
      .s0_addr_msb_err_o     (s0_ts_addr_msb_err),
      .s0_thread_id_o        (s0_ts_thread_id),
      .s1_available_i        (s1_available_to_ts),
      .s1_addr_load_o        (s1_ts_addr_load),
      .s1_tlb_wait_i         (s1_tlb_wait),
      .s1_tlb_fail_i         (s1_tlb_fail),
      .s2_addr_i             (s2_req_addr[minion_dcache_pkg::PaSize-1:0]),
      .ba_full_i             (re_ba_full),
      .ba_alloc_o            (re_ba_alloc),
      .ba_write_needed_o     (s2_re_ba_write_needed),
      .s2_valid_i            (s2_valid),
      .s2_ba_written_entry_i (s2_ba_write_entry),
      .s3_ba_write_addr_o    (s3_re_ba_write_addr),
      .wait_l2_to_ba_resp_o  (re_wait_l2_to_ba_resp),
      .ba_entries_required_o (re_ba_entries_required),
      .s1_valid_i            (s1_valid),
      .ba_read_used_i        (s1_mh_force_ba_rptr),
      .s1_ba_data_i          (s1_ba_data),
      .ba_read_en_o          (s1_re_ba_read_en),
      .ba_read_en_spec_o     (s1_re_ba_read_en_spec),
      .ba_read_addr_o        (s1_re_ba_read_addr),
      .scp_ctrl_on_i         (scp_ctrl_on),
      .vpu_reduce_wait_i     (s1_vpu_ctrl_i.reduce_wait),
      .vpu_ctrl_o            (vpu_reduce_ctrl),
      .vm_status_i           (vm_status_i),
      .vm_status_ts_o        (vm_status_ts),
      .ts_start_o            (ts_start),
      .ts_op_o               (ts_op),
      .mh_going_to_uc_store_i(mh_going_to_uc_store),
      .err_flags_o           (tensor_reduce_err_all_flags),
      .bus_err_o             (bus_err_re),
      .csr_debug_signals_o   (ts_csr_debug_signals),
      .sm_match_debug_signals_o(ts_sm_match_debug_signals),
      .sm_filter_debug_signals_o(ts_sm_filter_debug_signals),
      .sm_data_debug_signals_o(ts_sm_data_debug_signals)
    );

    minion_dcache_texsend u_texsend (
      .clk_i                (clk_i),
      .rst_ni               (rst_ni),
      .neigh_min_id_i       (shire_min_id_i[$clog2(ShireCoopMinMaskSize)-1:0]),
      .chicken_bit_dcache_i (chicken_bit_dcache_i),
      .idle_o               (tx_idle),
      .tex_send_ready_o     (tex_send_ready),
      .tex_send_valid_i     (tex_send_valid),
      .tex_send_ctrl_i      (core_ctrl_i.tex_send_ctrl),
      .l2_resp_valid_i      (l2_resp_valid_ready_int),
      .l2_resp_i            (l2_resp_int),
      .l2_req_ready_i       (tx_l2_miss_ready),
      .l2_req_valid_o       (tx_l2_miss_valid),
      .l2_req_o             (tx_l2_miss_req),
      .writeback_ready_i    (tx_wb_ready),
      .writeback_valid_o    (tx_wb_valid),
      .writeback_req_o      (tx_wb_req),
      .writeback_done_i     (wb_ready),
      .csr_debug_signals_o  (tx_csr_debug_signals)
    );
  end else begin : gen_no_offload_reduce
    assign reduce_ready = 1'b1;
    assign tensorstore_busy = 1'b0;
    assign re_buffer_avail = 1'b1;
    assign re_l2_evict_valid = 1'b0;
    assign re_l2_evict_req = '0;
    assign ts_l2_evict_valid = 1'b0;
    assign ts_l2_evict_req = '0;
    assign re_ts_l2_evict_inflight = 1'b0;
    assign re_pipe_used = 1'b0;
    assign s1_reduce_send_inst = 1'b0;
    assign s2_reduce_send_inst = 1'b0;
    assign s3_reduce_send_inst = 1'b0;
    assign s1_reduce_op_inst = 1'b0;
    assign s2_reduce_op_inst = 1'b0;
    assign s3_reduce_op_inst = 1'b0;
    assign ts_scp_read = 1'b0;
    assign ts_scp_read_way = '0;
    assign ts_scp_read_addr = '0;
    assign ts_col_size_is_128b = 1'b0;
    assign s0_ts_addr_load = 1'b0;
    assign s0_ts_addr = '0;
    assign s0_ts_addr_msb_err = 1'b0;
    assign s0_ts_thread_id = 1'b0;
    assign s1_ts_addr_load = 1'b0;
    assign re_ba_alloc = 1'b0;
    assign s2_re_ba_write_needed = 1'b0;
    assign s3_re_ba_write_addr = '0;
    assign re_wait_l2_to_ba_resp = 1'b0;
    assign re_ba_entries_required = 1'b0;
    assign s1_re_ba_read_en = 1'b0;
    assign s1_re_ba_read_en_spec = 1'b0;
    assign s1_re_ba_read_addr = '0;
    assign vpu_reduce_ctrl = '0;
    assign vm_status_ts = '0;
    assign ts_start = 1'b0;
    assign ts_op = 1'b0;
    assign tensor_reduce_err_all_flags = '0;
    assign bus_err_re = '0;
    assign ts_csr_debug_signals = '0;
    assign ts_sm_match_debug_signals = '0;
    assign ts_sm_filter_debug_signals = '0;
    assign ts_sm_data_debug_signals = '0;
    assign tx_idle = 1'b1;
    assign tex_send_ready = 1'b1;
    assign tx_l2_miss_valid = 1'b0;
    assign tx_l2_miss_req = '0;
    assign tx_wb_valid = 1'b0;
    assign tx_wb_req = '0;
    assign tx_csr_debug_signals = '0;
  end

  if (DebugApbEn) begin : gen_debug_apb_window
    always_comb begin
      dbg_md_read_en = 1'b0;
      dbg_da_read_en = 1'b0;

      dbg_md_read_addr = dbg_apb_paddr[DcacheSetIdxWidth + DcacheWayIdxWidth - 1:0];
      dbg_da_read_addr = dbg_apb_paddr;
      apb_prdata_o = '0;

      if (apb_psel_i && apb_penable_i && !apb_pwrite_i) begin
        dbg_md_read_en = dbg_block_sel_md;
        dbg_da_read_en = dbg_block_sel_da;
      end

      apb_prdata_o =
          dbg_block_sel_md
              ? {{(BpamShireApbDataWidth - $bits(dcache_meta_data_t)){1'b0}}, dbg_md_read_data}
              : dbg_da_read_data;
    end

    always_comb begin
      dbg_da_read_valid_next = 1'b0;
      if (apb_psel_i && apb_penable_i && dbg_da_read_en) begin
        dbg_da_read_valid_next = dbg_da_read_accept || dbg_da_read_valid;
      end
    end

    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        dbg_da_read_accept <= 1'b0;
        dbg_da_read_valid <= 1'b0;
      end else if (apb_psel_i) begin
        dbg_da_read_accept <= dbg_da_read_accept_next;
        dbg_da_read_valid <= dbg_da_read_valid_next;
      end
    end

    assign dbg_da_read_data = s3_data[dbg_da_read_addr[1:0]];

    always_comb begin
      id_dbg_bid = dbg_md_read_en && !dbg_md_read_accept;
      dbg_md_read_accept_next = apb_psel_i && (id_dbg_grant || dbg_md_read_accept);
    end

    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        dbg_md_read_accept <= 1'b0;
        s0_dbg_valid <= 1'b0;
        s1_dbg_valid <= 1'b0;
        dbg_md_read_valid <= 1'b0;
      end else begin
        if (apb_psel_i || dbg_md_read_accept) begin
          dbg_md_read_accept <= dbg_md_read_accept_next;
        end
        if (apb_psel_i) begin
          s0_dbg_valid <= id_dbg_grant;
        end
        if (dbg_md_read_accept) begin
          s1_dbg_valid <= s0_dbg_valid;
          dbg_md_read_valid <= s1_dbg_valid;
        end
      end
    end

    assign dbg_md_read_data = s2_meta_read[dbg_md_read_addr[DcacheSetIdxWidth +: DcacheWayIdxWidth]];

    always_comb begin
      dbg_da_write_en = 1'b0;
      dbg_md_write_en = 1'b0;
      dbg_md_write_en_early = 1'b0;
      dbg_da_write_en_early = 1'b0;

      if (apb_psel_i && apb_penable_i && apb_pwrite_i) begin
        dbg_da_write_en = dbg_block_sel_da;
        dbg_md_write_en = dbg_block_sel_md;
      end

      if (apb_psel_i && !apb_penable_i && apb_pwrite_i) begin
        dbg_md_write_en_early = dbg_block_sel_md;
        dbg_da_write_en_early = dbg_block_sel_da;
      end

      dbg_da_write_valid_next = dbg_da_write_en && (!s4_da_write_en_but_dbg || dbg_da_write_valid);
      dbg_md_write_addr = dbg_apb_paddr[DcacheSetIdxWidth + DcacheWayIdxWidth - 1:0];
      dbg_da_write_addr = dbg_apb_paddr;
    end

    assign dbg_md_write_data = dcache_meta_data_t'(apb_pwdata_i[$bits(dcache_meta_data_t)-1:0]);
    assign dbg_da_write_data = apb_pwdata_i[DcacheLramDataSize-1:0];

    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        dbg_da_write_valid <= 1'b0;
      end else if (dbg_da_write_en || dbg_da_write_valid) begin
        dbg_da_write_valid <= dbg_da_write_valid_next;
      end
    end

    always_comb begin
      dbg_block_sel_md = 1'b0;
      dbg_block_sel_da = 1'b0;
      apb_pready_o = 1'b0;

      dbg_block_sel_md = (apb_paddr_i[DcacheDbgAddrBits-1] == 1'b0);
      dbg_block_sel_da = (apb_paddr_i[DcacheDbgAddrBits-1] == 1'b1);

      apb_pready_o =
          dbg_block_sel_md ? (apb_pwrite_i ? dbg_md_write_valid : dbg_md_read_valid)
                           : (apb_pwrite_i ? dbg_da_write_valid : dbg_da_read_valid);
      apb_pready_o &= apb_psel_i;
    end

    assign dbg_apb_paddr = apb_paddr_i[DcacheDbgAddrBits-2:0];
    assign apb_pslverr_o = 1'b0;
  end else begin : gen_no_debug_apb_window
    assign dbg_md_read_en = 1'b0;
    assign dbg_da_read_en = 1'b0;
    assign dbg_md_read_addr = '0;
    assign dbg_da_read_addr = '0;
    assign dbg_da_read_valid_next = 1'b0;
    assign dbg_da_read_accept = 1'b0;
    assign dbg_da_read_valid = 1'b0;
    assign dbg_da_read_data = '0;
    assign id_dbg_bid = 1'b0;
    assign dbg_md_read_accept_next = 1'b0;
    assign dbg_md_read_accept = 1'b0;
    assign s0_dbg_valid = 1'b0;
    assign s1_dbg_valid = 1'b0;
    assign dbg_md_read_valid = 1'b0;
    assign dbg_md_read_data = '0;
    assign dbg_da_write_en = 1'b0;
    assign dbg_md_write_en = 1'b0;
    assign dbg_md_write_en_early = 1'b0;
    assign dbg_da_write_en_early = 1'b0;
    assign dbg_da_write_valid_next = 1'b0;
    assign dbg_md_write_addr = '0;
    assign dbg_da_write_addr = '0;
    assign dbg_md_write_data = '0;
    assign dbg_da_write_data = '0;
    assign dbg_da_write_valid = 1'b0;
    assign dbg_block_sel_md = 1'b0;
    assign dbg_block_sel_da = 1'b0;
    assign apb_pready_o = apb_psel_i && apb_penable_i;
    assign apb_prdata_o = '0;
    assign dbg_apb_paddr = '0;
    assign apb_pslverr_o = apb_psel_i && apb_penable_i;
  end

  always_comb begin
    s1_pma_bus_err_per_thread = '0;
    s1_pma_bus_err_per_thread[s1_req.dest.thread_id] = s1_pma_bus_err;

    bus_err_next = bus_err_co_l2 | bus_err_mh | bus_err_re | bus_err_tl0 | bus_err_tl1
                | s1_pma_bus_err_per_thread;
    if (|bus_err_mh) begin
      bus_err_addr_next = bus_err_addr_mh;
    end else if (s1_pma_bus_err) begin
      bus_err_addr_next = s1_req.addr[minion_dcache_pkg::PaSize-1:0];
    end else begin
      bus_err_addr_next = '0;
    end

    io_events_next = '0;
    io_events_next[0] =
        s0_core_req_valid_i && !s0_core_req_valid_killed && (s0_core_req_i.dest.thread_id == 1'b0);
    io_events_next[1] =
        s0_core_req_valid_i && !s0_core_req_valid_killed && (s0_core_req_i.dest.thread_id == 1'b1);
    io_events_next[2] = s2_mh_req_valid && s2_req.cacheable && (s2_req.dest.thread_id == 1'b0);
    io_events_next[3] = s2_mh_req_valid && s2_req.cacheable && (s2_req.dest.thread_id == 1'b1);
    io_events_next[4] = |(l2_miss_req_valid_o & l2_miss_req_ready_i);
    io_events_next[5] = |(l2_miss_req_valid_o & ~l2_miss_req_ready_i) && !io_events_next[4];
    io_events_next[6] = |(l2_evict_req_valid_o & l2_evict_req_ready_i);
    io_events_next[7] = |(l2_evict_req_valid_o & ~l2_evict_req_ready_i) && !io_events_next[6];
    io_events_next[8] = tl0_start || tl1_start;
    io_events_next[9] = tl0_op || tl1_op;
    io_events_next[10] = ts_start;
    io_events_next[11] = ts_op;

    io_events_en = (|io_events_next) || (|io_events_o);
  end

  for (genvar bus_err_thread = 0; bus_err_thread < NrThreads; bus_err_thread++) begin : gen_bus_err_addr
    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        bus_err_addr_o[bus_err_thread] <= '0;
      end else if (bus_err_next[bus_err_thread]) begin
        bus_err_addr_o[bus_err_thread] <= bus_err_addr_next;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      bus_err_o <= '0;
      io_events_o <= '0;
    end else begin
      bus_err_o <= bus_err_next;
      if (io_events_en) begin
        io_events_o <= io_events_next;
      end
    end
  end

  assign core_ctrl_resp_o = core_ctrl_resp;
  assign id_core_ready_o = id_core_ready;
  assign vpu_reduce_ctrl_o = VpuEn ? vpu_reduce_ctrl : '0;
  assign s2_core_resp_int_valid_o =
      s2_valid_to_s3 && s2_is_read && !(s2_req.dest.fp || (s2_req.dest.addr == '0));
  assign s1_core_replay_next_o =
      (s1_valid && s1_req.replay) && s1_is_read && !s1_req.dest.fp && !s1_is_to_null;
  assign s1_core_xcpt_o = s1_core_xcpt;
  assign l2_resp_ready_o = !l2_resp_fifo_full;
  assign l2_resp_ready_int_o = l2_resp_ready_int;
  assign l2_resp_valid_int_o = l2_resp_valid_int;
  assign l2_resp_int_o = l2_resp_int;
  assign s1_bp_conf_o = s1_bp_conf;
  assign s1_bp_conf_valid_o = s1_bp_conf_valid;
  assign tensor_load_err_flags_o =
      VpuEn
          ? (tensor_load_err_flags_0[DcacheTlErrorBits-3:0]
             | tensor_load_err_flags_1[DcacheTlErrorBits-3:0])
          : '0;
  assign cache_ops_err_flags_o = cache_ops_err_flags;
  assign tensor_reduce_err_flags_o =
      VpuEn ? tensor_reduce_err_all_flags[DcacheErrFlagCoLock][0] : 1'b0;

  always_comb begin
    logic [DcacheDbgSmDataGroupBits-1:0] sm_data_group0;
    logic [DcacheDbgSmDataGroupBits-1:0] sm_data_group1;
    logic [DcacheDbgSmDataGroupBits-1:0] sm_data_group2;
    logic [DcacheDbgSmDataGroupBits-1:0] sm_data_group3;

    csr_debug_sigs_o = '0;
    sm_match_debug_signals_dc_o = '0;
    sm_filter_debug_signals_dc_o = '0;
    sm_data_debug_signals_dc_o = '0;
    sm_data_group3 = '0;
    sm_data_group2 = '0;
    sm_data_group1 = '0;
    sm_data_group0 = '0;

    if (DebugMonEn) begin
      csr_debug_sigs_o = {
        tl0_csr_debug_signals,
        tl1_csr_debug_signals,
        ts_csr_debug_signals,
        co_csr_debug_signals,
        l2_csr_debug_signals,
        tx_csr_debug_signals,
        mh_csr_debug_signals[minion_dcache_pkg::DcacheDbgCsrMhSize-1:0],
        mh_csr_debug_signals[2*minion_dcache_pkg::DcacheDbgCsrMhSize-1:minion_dcache_pkg::DcacheDbgCsrMhSize]
      };

      sm_filter_debug_signals_dc_o[minion_dcache_pkg::PaSize + minion_dcache_pkg::VaSize
                                   + minion_dcache_pkg::VaSize + 24 - 1:0] =
          {dcache_idle, s3_valid, s2_valid_to_s3, s2_rq_dealloc, s2_rq_renack, s2_rq_push,
           s2_valid_masked, s2_valid_qual, s2_valid, s1_tlb_req_valid, s1_valid_to_s2, s1_valid_masked,
           s1_valid, s0_co_l2_addr_load, s0_ts_addr_load, s0_tl_addr_load, s0_co_mr_valid,
           s0_rq_replay_valid, s0_core_req_valid_i, s0_co_pf_valid, s0_dbg_valid, s0_da_read_en,
           s0_wb_da_valid, s0_valid_to_s1, s2_req.addr[minion_dcache_pkg::PaSize-1:0],
           s1_req.addr[minion_dcache_pkg::VaSize-1:0], s0_req_addr};

      sm_data_group3[$bits({
          s3_req.addr_msb_err, s3_req.phys, s3_req.dest, s3_req.cmd, s3_req.typ, s3_req.gsc_cnt,
          s3_req.gsc32_idx, s3_req.ps_mask, s3_req.replay, s3_req.replay_entry, s3_req.replay_age,
          s3_req.cache_op, s3_req.misaligned, s3_req.misaligned_2nd, s3_req.buf_alloc, s3_req.buf_id,
          s3_req.cacheable, s3_req.uc_load_2nd, s3_req.vm_sel, s3_req.chunk_read, s3_req.mem_global
        })-1:0] =
          {s3_req.addr_msb_err, s3_req.phys, s3_req.dest, s3_req.cmd, s3_req.typ, s3_req.gsc_cnt,
           s3_req.gsc32_idx, s3_req.ps_mask, s3_req.replay, s3_req.replay_entry, s3_req.replay_age,
           s3_req.cache_op, s3_req.misaligned, s3_req.misaligned_2nd, s3_req.buf_alloc, s3_req.buf_id,
           s3_req.cacheable, s3_req.uc_load_2nd, s3_req.vm_sel, s3_req.chunk_read, s3_req.mem_global};

      sm_data_group2[$bits({
          s2_mh_req_valid, s2_tag_match_qual, s2_hit, s2_nack_inflight, s2_nack_miss,
          s2_nack_misaligned, s2_nack, s2_req.addr_msb_err, s2_req.phys, s2_req.dest, s2_req.cmd,
          s2_req.typ, s2_req.gsc_cnt, s2_req.gsc32_idx, s2_req.ps_mask, s2_req.replay,
          s2_req.replay_entry, s2_req.replay_age, s2_req.cache_op, s2_req.misaligned,
          s2_req.misaligned_2nd, s2_req.buf_alloc, s2_req.buf_id, s2_req.cacheable,
          s2_req.uc_load_2nd, s2_req.vm_sel, s2_req.chunk_read, s2_req.mem_global
        })-1:0] =
          {s2_mh_req_valid, s2_tag_match_qual, s2_hit, s2_nack_inflight, s2_nack_miss,
           s2_nack_misaligned, s2_nack, s2_req.addr_msb_err, s2_req.phys, s2_req.dest, s2_req.cmd,
           s2_req.typ, s2_req.gsc_cnt, s2_req.gsc32_idx, s2_req.ps_mask, s2_req.replay,
           s2_req.replay_entry, s2_req.replay_age, s2_req.cache_op, s2_req.misaligned,
           s2_req.misaligned_2nd, s2_req.buf_alloc, s2_req.buf_id, s2_req.cacheable,
           s2_req.uc_load_2nd, s2_req.vm_sel, s2_req.chunk_read, s2_req.mem_global};

      sm_data_group1[$bits({
          {s1_misaligned_case_5, s1_misaligned_case_4, s1_misaligned_case_3, s1_misaligned_case_2,
           s1_misaligned_case_1}, s1_tag_match, s1_translate_only_addr_load, s1_pma_resp_data.access_fault,
          s1_pma_resp_data.cacheable, s1_tlb_resp_data.xcpt_ld, s1_tlb_resp_data.xcpt_st,
          s1_tlb_resp_data.miss, s1_tlb_resp_data.fill_pending, s1_tlb_resp_data.access_fault,
          s1_req.addr_msb_err, s1_req.phys, s1_req.dest, s1_req.cmd, s1_req.typ, s1_req.gsc_cnt,
          s1_req.gsc32_idx, s1_req.ps_mask, s1_req.replay, s1_req.replay_entry, s1_req.replay_age,
          s1_req.cache_op, s1_req.misaligned, s1_req.misaligned_2nd, s1_req.buf_alloc, s1_req.buf_id,
          s1_req.cacheable, s1_req.uc_load_2nd, s1_req.vm_sel, s1_req.chunk_read, s1_req.mem_global
        })-1:0] =
          {{s1_misaligned_case_5, s1_misaligned_case_4, s1_misaligned_case_3, s1_misaligned_case_2,
            s1_misaligned_case_1}, s1_tag_match, s1_translate_only_addr_load,
           s1_pma_resp_data.access_fault, s1_pma_resp_data.cacheable, s1_tlb_resp_data.xcpt_ld,
           s1_tlb_resp_data.xcpt_st, s1_tlb_resp_data.miss, s1_tlb_resp_data.fill_pending,
           s1_tlb_resp_data.access_fault, s1_req.addr_msb_err, s1_req.phys, s1_req.dest, s1_req.cmd,
           s1_req.typ, s1_req.gsc_cnt, s1_req.gsc32_idx, s1_req.ps_mask, s1_req.replay,
           s1_req.replay_entry, s1_req.replay_age, s1_req.cache_op, s1_req.misaligned,
           s1_req.misaligned_2nd, s1_req.buf_alloc, s1_req.buf_id, s1_req.cacheable,
           s1_req.uc_load_2nd, s1_req.vm_sel, s1_req.chunk_read, s1_req.mem_global};

      sm_data_group0[$bits({
          s0_req.addr_msb_err, s0_req.phys, s0_req.dest, s0_req.cmd, s0_req.typ, s0_req.gsc_cnt,
          s0_req.gsc32_idx, s0_req.ps_mask, s0_req.replay, s0_req.replay_entry, s0_req.replay_age,
          s0_req.cache_op, s0_req.misaligned, s0_req.misaligned_2nd, s0_req.buf_alloc, s0_req.buf_id,
          s0_req.cacheable, s0_req.uc_load_2nd, s0_req.vm_sel, s0_req.chunk_read, s0_req.mem_global
        })-1:0] =
          {s0_req.addr_msb_err, s0_req.phys, s0_req.dest, s0_req.cmd, s0_req.typ, s0_req.gsc_cnt,
           s0_req.gsc32_idx, s0_req.ps_mask, s0_req.replay, s0_req.replay_entry, s0_req.replay_age,
           s0_req.cache_op, s0_req.misaligned, s0_req.misaligned_2nd, s0_req.buf_alloc, s0_req.buf_id,
           s0_req.cacheable, s0_req.uc_load_2nd, s0_req.vm_sel, s0_req.chunk_read, s0_req.mem_global};

      sm_data_debug_signals_dc_o = {sm_data_group3, sm_data_group2, sm_data_group1, sm_data_group0};
    end
  end

  assign sm_match_debug_signals_ts_o = DebugMonEn ? ts_sm_match_debug_signals : '0;
  assign sm_filter_debug_signals_ts_o = DebugMonEn ? ts_sm_filter_debug_signals : '0;
  assign sm_data_debug_signals_ts_o = DebugMonEn ? ts_sm_data_debug_signals : '0;
  assign sm_match_debug_signals_co_o = DebugMonEn ? co_sm_match_debug_signals : '0;
  assign sm_filter_debug_signals_co_o = DebugMonEn ? co_sm_filter_debug_signals : '0;
  assign sm_data_debug_signals_co_o = DebugMonEn ? co_sm_data_debug_signals : '0;
  assign sm_match_debug_signals_col2_o = DebugMonEn ? l2_sm_match_debug_signals : '0;
  assign sm_filter_debug_signals_col2_o = DebugMonEn ? l2_sm_filter_debug_signals : '0;
  assign sm_data_debug_signals_col2_o = DebugMonEn ? l2_sm_data_debug_signals : '0;
  // Keep monitor observability available, but make DebugMonEn=0 a hard boundary
  // so FPGA-only harnesses can prune the raw monitor buses.
  assign sm_match_debug_signals_tl0_o = DebugMonEn ? tl0_sm_match_debug_signals : '0;
  assign sm_filter_debug_signals_tl0_o = DebugMonEn ? tl0_sm_filter_debug_signals : '0;
  assign sm_data_debug_signals_tl0_o = DebugMonEn ? tl0_sm_data_debug_signals : '0;
  assign sm_match_debug_signals_tl1_o = DebugMonEn ? tl1_sm_match_debug_signals : '0;
  assign sm_filter_debug_signals_tl1_o = DebugMonEn ? tl1_sm_filter_debug_signals : '0;
  assign sm_data_debug_signals_tl1_o = DebugMonEn ? tl1_sm_data_debug_signals : '0;
  assign sm_match_debug_signals_mh_o = DebugMonEn ? mh_sm_match_debug_signals : '0;
  assign sm_filter_debug_signals_mh_o = DebugMonEn ? mh_sm_filter_debug_signals : '0;
  assign sm_data_debug_signals_mh_o = DebugMonEn ? mh_sm_data_debug_signals : '0;

endmodule : minion_dcache_top
