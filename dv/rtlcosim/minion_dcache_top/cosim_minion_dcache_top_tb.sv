// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_top vs new minion_dcache_top.

`include "soc.vh"

module cosim_minion_dcache_top_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                   clk_i,
  input  logic                                   rst_ni,
  input  logic [NumShireIdsBits-1:0]             stim_shire_id_i,
  input  logic [MinPerSBits-1:0]                 stim_shire_min_id_i,
  input  logic                                   stim_ioshire_i,
  input  logic                                   stim_chicken_bit_dcache_i,
  input  core_dcache_ctrl                        stim_core_ctrl_i,
  input  logic                                   stim_force_ts_scp_start_i,
  input  logic                                   stim_id_core_alloc_rq_pre_i,
  input  logic                                   stim_s0_core_alloc_rq_val_i,
  input  logic                                   stim_id_core_gsc_i,
  input  logic                                   stim_s0_core_req_valid_i,
  input  logic [VaSize-1:0]                      stim_req_addr_i,
  input  logic                                   stim_req_addr_msb_err_i,
  input  logic                                   stim_req_dest_fp_i,
  input  logic [XregAddrSize-1:0]                stim_req_dest_addr_i,
  input  logic                                   stim_req_thread_id_i,
  input  logic [4:0]                             stim_req_cmd_i,
  input  logic [3:0]                             stim_req_typ_i,
  input  logic [CoreGscCntBits-1:0]              stim_req_gsc_cnt_i,
  input  logic [DmemReqPsMaskSz-1:0]             stim_req_ps_mask_i,
  input  logic                                   stim_req_phys_i,
  input  logic                                   stim_req_mem_global_i,
  input  logic [CoreGsc32IdxVSize-1:0]           stim_req_gsc32_idx_i,
  input  logic                                   stim_s0_core_gsc_i,
  input  logic                                   stim_s1_core_kill_i,
  input  logic [VpuDataSz-1:0]                   stim_s1_core_store_data_i,
  input  esr_mprot_t                             stim_s1_mprot_i,
  input  logic                                   stim_s2_core_kill_i,
  input  logic [XregSize-1:0]                    stim_s3_core_x31_i,
  input  vpu_dcache_ctrl                         stim_s1_vpu_ctrl_i,
  input  logic                                   stim_s3_invalidate_lr_i,
  input  logic                                   stim_wb_dcache_fp_toint_i,
  input  logic [$bits(tlb_entry_type)-1:0]       stim_vmspagesize_i,
  input  logic                                   stim_bypass_dcache_i,
  input  logic [MinionMemCtrlBits-1:0]           stim_mem_ctrl_override_i,
  input  logic [3:0]                             stim_satp_mode_i,
  input  logic [PaTransSize-1:0]                 stim_satp_ppn_i,
  input  logic [3:0]                             stim_matp_mode_i,
  input  logic [PaTransSize-1:0]                 stim_matp_ppn_i,
  input  logic                                   stim_satp_info_en_i,
  input  logic                                   stim_matp_info_en_i,
  input  logic [1:0]                             stim_vm_prv_t0_i,
  input  logic                                   stim_vm_mprv_t0_i,
  input  logic [1:0]                             stim_vm_mpp_t0_i,
  input  logic                                   stim_vm_sum_t0_i,
  input  logic                                   stim_vm_mxr_t0_i,
  input  logic                                   stim_vm_debug_t0_i,
  input  logic [1:0]                             stim_vm_prv_t1_i,
  input  logic                                   stim_vm_mprv_t1_i,
  input  logic [1:0]                             stim_vm_mpp_t1_i,
  input  logic                                   stim_vm_sum_t1_i,
  input  logic                                   stim_vm_mxr_t1_i,
  input  logic                                   stim_vm_debug_t1_i,
  input  logic                                   stim_tlb_invalidate_i,
  input  logic                                   stim_ptw_req_ready_i,
  input  logic                                   stim_ptw_resp_canceled_req_i,
  input  logic                                   stim_ptw_resp_access_fault_i,
  input  logic [7:0]                             stim_ptw_resp_rsvd_for_hw_i,
  input  logic [CsrSatpPpnSz-1:0]                stim_ptw_resp_ppn_i,
  input  logic [1:0]                             stim_ptw_resp_rsvd_for_sw_i,
  input  logic                                   stim_ptw_resp_d_i,
  input  logic                                   stim_ptw_resp_a_i,
  input  logic                                   stim_ptw_resp_g_i,
  input  logic                                   stim_ptw_resp_u_i,
  input  logic                                   stim_ptw_resp_x_i,
  input  logic                                   stim_ptw_resp_w_i,
  input  logic                                   stim_ptw_resp_r_i,
  input  logic                                   stim_ptw_resp_v_i,
  input  logic                                   stim_ptw_resp_valid_i,
  input  logic [DcacheL2EvictReqPorts-1:0]       stim_l2_evict_req_ready_i,
  input  logic [DcacheL2MissReqPorts-1:0]        stim_l2_miss_req_ready_i,
  input  logic                                   stim_l2_resp_valid_i,
  input  et_link_minion_rsp_info_t               stim_l2_resp_i,
  input  logic [DcacheDbgAddrBits-1:0]           stim_apb_paddr_i,
  input  logic                                   stim_apb_pwrite_i,
  input  logic                                   stim_apb_psel_i,
  input  logic                                   stim_apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]       stim_apb_pwdata_i,

  output dcache_core_ctrl                        orig_core_ctrl_resp,
  output dcache_core_ctrl                        new_core_ctrl_resp,
  output logic                                   orig_id_core_ready,
  output logic                                   new_id_core_ready,
  output dcache_vpu_scp_resp                     orig_s3_vpu_scp_resp,
  output dcache_vpu_scp_resp                     new_s3_vpu_scp_resp,
  output logic [VpuDataSz-1:0]                   orig_s3_vpu_scp_data,
  output logic [VpuDataSz-1:0]                   new_s3_vpu_scp_data,
  output logic [VpuDataSz-1:0]                   orig_s3_vpu_tenb_data,
  output logic [VpuDataSz-1:0]                   new_s3_vpu_tenb_data,
  output dcache_vpu_reduce_ctrl                  orig_vpu_reduce_ctrl,
  output dcache_vpu_reduce_ctrl                  new_vpu_reduce_ctrl,
  output logic                                   orig_s2_core_resp_int_valid,
  output logic                                   new_s2_core_resp_int_valid,
  output logic                                   orig_s3_core_resp_valid,
  output logic                                   new_s3_core_resp_valid,
  output dcache_minion_resp                      orig_s3_core_resp,
  output dcache_minion_resp                      new_s3_core_resp,
  output dcache_minion_scoreboard                orig_id_core_scoreboard,
  output dcache_minion_scoreboard                new_id_core_scoreboard,
  output logic [NrThreads-1:0]                   orig_id_core_sb_fp_dealloc,
  output logic [NrThreads-1:0]                   new_id_core_sb_fp_dealloc,
  output logic [NrThreads-1:0]                   orig_id_core_sb_int_dealloc,
  output logic [NrThreads-1:0]                   new_id_core_sb_int_dealloc,
  output logic                                   orig_s1_core_replay_next,
  output logic                                   new_s1_core_replay_next,
  output dcache_tag_xcpt                         orig_s1_core_xcpt,
  output dcache_tag_xcpt                         new_s1_core_xcpt,
  output logic                                   orig_s2_core_flush_pipeline,
  output logic                                   new_s2_core_flush_pipeline,
  output logic [NrThreads-1:0]                   orig_s3_ordered,
  output logic [NrThreads-1:0]                   new_s3_ordered,
  output minion_ptw_req                          orig_ptw_req_data,
  output minion_ptw_req                          new_ptw_req_data,
  output logic                                   orig_ptw_req_valid,
  output logic                                   new_ptw_req_valid,
  output logic [DcacheL2EvictReqPorts-1:0]       orig_l2_evict_req_valid,
  output logic [DcacheL2EvictReqPorts-1:0]       new_l2_evict_req_valid,
  output et_link_minion_evict_req_info_t         orig_l2_evict_req,
  output et_link_minion_evict_req_info_t         new_l2_evict_req,
  output logic [DcacheL2MissReqPorts-1:0]        orig_l2_miss_req_valid,
  output logic [DcacheL2MissReqPorts-1:0]        new_l2_miss_req_valid,
  output et_link_minion_miss_req_info_t          orig_l2_miss_req,
  output et_link_minion_miss_req_info_t          new_l2_miss_req,
  output logic                                   orig_l2_resp_ready,
  output logic                                   new_l2_resp_ready,
  output logic                                   orig_l2_resp_ready_int,
  output logic                                   new_l2_resp_ready_int,
  output logic                                   orig_l2_resp_valid_int,
  output logic                                   new_l2_resp_valid_int,
  output et_link_minion_rsp_info_t               orig_l2_resp_int,
  output et_link_minion_rsp_info_t               new_l2_resp_int,
  output dcache_core_bp_t                        orig_s1_bp_conf,
  output dcache_core_bp_t                        new_s1_bp_conf,
  output logic                                   orig_s1_bp_conf_valid,
  output logic                                   new_s1_bp_conf_valid,
  output logic                                   orig_dcache_idle_excl_mode,
  output logic                                   new_dcache_idle_excl_mode,
  output logic [DcacheTlErrorBits-3:0]           orig_tensor_load_err_flags,
  output logic [DcacheTlErrorBits-3:0]           new_tensor_load_err_flags,
  output logic [DcacheErrFlagBits-1:0][NrThreads-1:0] orig_cache_ops_err_flags,
  output logic [DcacheErrFlagBits-1:0][NrThreads-1:0] new_cache_ops_err_flags,
  output logic                                   orig_tensor_reduce_err_flags,
  output logic                                   new_tensor_reduce_err_flags,
  output logic                                   orig_apb_pready,
  output logic                                   new_apb_pready,
  output logic [BpamShireApbDataWidth-1:0]       orig_apb_prdata,
  output logic [BpamShireApbDataWidth-1:0]       new_apb_prdata,
  output logic                                   orig_apb_pslverr,
  output logic                                   new_apb_pslverr,
  output logic [CsrNrEventsDcache-1:0]           orig_io_events,
  output logic [CsrNrEventsDcache-1:0]           new_io_events,
  output logic [NrThreads-1:0]                   orig_bus_err,
  output logic [NrThreads-1:0]                   new_bus_err,
  output logic [NrThreads-1:0][PaSize-1:0]       orig_bus_err_addr,
  output logic [NrThreads-1:0][PaSize-1:0]       new_bus_err_addr,
  output dcache_debug_sigs                       orig_csr_debug_sigs,
  output dcache_debug_sigs                       new_csr_debug_sigs,
  output logic [DcacheDbgSmMatchBits-1:0]        orig_sm_match_debug_signals_tl0,
  output logic [DcacheDbgSmMatchBits-1:0]        new_sm_match_debug_signals_tl0,
  output logic [DcacheDbgSmFilterBits-1:0]       orig_sm_filter_debug_signals_tl0,
  output logic [DcacheDbgSmFilterBits-1:0]       new_sm_filter_debug_signals_tl0,
  output logic [DcacheDbgSmDataBits-1:0]         orig_sm_data_debug_signals_tl0,
  output logic [DcacheDbgSmDataBits-1:0]         new_sm_data_debug_signals_tl0,
  output logic [DcacheDbgSmMatchBits-1:0]        orig_sm_match_debug_signals_tl1,
  output logic [DcacheDbgSmMatchBits-1:0]        new_sm_match_debug_signals_tl1,
  output logic [DcacheDbgSmFilterBits-1:0]       orig_sm_filter_debug_signals_tl1,
  output logic [DcacheDbgSmFilterBits-1:0]       new_sm_filter_debug_signals_tl1,
  output logic [DcacheDbgSmDataBits-1:0]         orig_sm_data_debug_signals_tl1,
  output logic [DcacheDbgSmDataBits-1:0]         new_sm_data_debug_signals_tl1,
  output logic [DcacheDbgSmMatchBits-1:0]        orig_sm_match_debug_signals_ts,
  output logic [DcacheDbgSmMatchBits-1:0]        new_sm_match_debug_signals_ts,
  output logic [DcacheDbgSmFilterBits-1:0]       orig_sm_filter_debug_signals_ts,
  output logic [DcacheDbgSmFilterBits-1:0]       new_sm_filter_debug_signals_ts,
  output logic [DcacheDbgSmDataBits-1:0]         orig_sm_data_debug_signals_ts,
  output logic [DcacheDbgSmDataBits-1:0]         new_sm_data_debug_signals_ts,
  output logic [DcacheDbgSmMatchBits-1:0]        orig_sm_match_debug_signals_co,
  output logic [DcacheDbgSmMatchBits-1:0]        new_sm_match_debug_signals_co,
  output logic [DcacheDbgSmFilterBits-1:0]       orig_sm_filter_debug_signals_co,
  output logic [DcacheDbgSmFilterBits-1:0]       new_sm_filter_debug_signals_co,
  output logic [DcacheDbgSmDataBits-1:0]         orig_sm_data_debug_signals_co,
  output logic [DcacheDbgSmDataBits-1:0]         new_sm_data_debug_signals_co,
  output logic [DcacheDbgSmMatchBits-1:0]        orig_sm_match_debug_signals_col2,
  output logic [DcacheDbgSmMatchBits-1:0]        new_sm_match_debug_signals_col2,
  output logic [DcacheDbgSmFilterBits-1:0]       orig_sm_filter_debug_signals_col2,
  output logic [DcacheDbgSmFilterBits-1:0]       new_sm_filter_debug_signals_col2,
  output logic [DcacheDbgSmDataBits-1:0]         orig_sm_data_debug_signals_col2,
  output logic [DcacheDbgSmDataBits-1:0]         new_sm_data_debug_signals_col2,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0]  orig_sm_match_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0]  new_sm_match_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] orig_sm_filter_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] new_sm_filter_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]   orig_sm_data_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]   new_sm_data_debug_signals_mh,
  output logic [DcacheDbgSmMatchBits-1:0]        orig_sm_match_debug_signals_dc,
  output logic [DcacheDbgSmMatchBits-1:0]        new_sm_match_debug_signals_dc,
  output logic [DcacheDbgSmFilterBits-1:0]       orig_sm_filter_debug_signals_dc,
  output logic [DcacheDbgSmFilterBits-1:0]       new_sm_filter_debug_signals_dc,
  output logic [DcacheDbgSmDataBits-1:0]         orig_sm_data_debug_signals_dc,
  output logic [DcacheDbgSmDataBits-1:0]         new_sm_data_debug_signals_dc,
  output logic                                   orig_mh_idle,
  output logic                                   new_mh_idle,
  output logic                                   orig_mh_l2_evict_valid,
  output logic                                   new_mh_l2_evict_valid,
  output logic [DcacheMhFileSize-1:0]            orig_s2_mh_handler_ready,
  output logic [DcacheMhFileSize-1:0]            new_s2_mh_handler_ready,
  output logic                                   orig_id_rq_full,
  output logic                                   new_id_rq_full,
  output logic                                   orig_id_rq_half_full,
  output logic                                   new_id_rq_half_full,
  output logic [NrThreads-1:0]                   orig_s2_rq_empty,
  output logic [NrThreads-1:0]                   new_s2_rq_empty,
  output logic [DcacheReplayqSize-1:0]           orig_rq_entry_mask_available,
  output logic [DcacheReplayqSize-1:0]           new_rq_entry_mask_available,
  output logic [DcacheReplayqAgeWidth-1:0]       orig_rq_push_age,
  output logic [DcacheReplayqAgeWidth-1:0]       new_rq_push_age,
  output logic [DcacheReplayqSize-1:0]           orig_rq_pending,
  output logic [DcacheReplayqSize-1:0]           new_rq_pending,
  output logic [DcacheReplayqSize-1:0]           orig_rq_wait_uc_load,
  output logic [DcacheReplayqSize-1:0]           new_rq_wait_uc_load,
  output logic                                   orig_rq_replay_valid_prev,
  output logic                                   new_rq_replay_valid_prev,
  output logic [DcacheReplayqAddrWidth-1:0]      orig_rq_replay_entry_prev,
  output logic [DcacheReplayqAddrWidth-1:0]      new_rq_replay_entry_prev,
  output logic                                   orig_s1_valid,
  output logic                                   new_s1_valid,
  output logic                                   orig_s1_valid_masked,
  output logic                                   new_s1_valid_masked,
  output logic                                   orig_s1_valid_to_tlb,
  output logic                                   new_s1_valid_to_tlb,
  output logic                                   orig_s1_valid_to_s2,
  output logic                                   new_s1_valid_to_s2,
  output logic [PaSize-1:0]                      orig_s1_phys_addr,
  output logic [PaSize-1:0]                      new_s1_phys_addr,
  output logic [VaSize-1:0]                      orig_s1_req_addr,
  output logic [VaSize-1:0]                      new_s1_req_addr,
  output logic [4:0]                             orig_s1_req_cmd,
  output logic [4:0]                             new_s1_req_cmd,
  output logic [3:0]                             orig_s1_req_typ,
  output logic [3:0]                             new_s1_req_typ,
  output logic [DcacheWays-1:0]                  orig_s1_mr_read_valid,
  output logic [DcacheWays-1:0]                  new_s1_mr_read_valid,
  output logic [DcacheWays-1:0]                  orig_s1_tag_match,
  output logic [DcacheWays-1:0]                  new_s1_tag_match,
  output logic                                   orig_s1_req_replay,
  output logic                                   new_s1_req_replay,
  output logic                                   orig_s1_req_cache_op,
  output logic                                   new_s1_req_cache_op,
  output logic                                   orig_s1_tlb_miss,
  output logic                                   new_s1_tlb_miss,
  output logic                                   orig_s1_pma_bus_err,
  output logic                                   new_s1_pma_bus_err,
  output logic                                   orig_s1_rq_push_conservative,
  output logic                                   new_s1_rq_push_conservative,
  output logic                                   orig_s2_valid,
  output logic                                   new_s2_valid,
  output logic                                   orig_s2_valid_qual,
  output logic                                   new_s2_valid_qual,
  output logic                                   orig_s2_rq_push_cancel,
  output logic                                   new_s2_rq_push_cancel,
  output logic                                   orig_s2_rq_push,
  output logic                                   new_s2_rq_push,
  output logic                                   orig_s2_rq_renack,
  output logic                                   new_s2_rq_renack,
  output logic                                   orig_s2_rq_dealloc,
  output logic                                   new_s2_rq_dealloc,
  output logic [3:0]                             orig_mh_state_0,
  output logic [3:0]                             new_mh_state_0,
  output logic [3:0]                             orig_mh_state_1,
  output logic [3:0]                             new_mh_state_1,
  output logic                                   orig_l2_resp_is_for_mh_fill,
  output logic                                   new_l2_resp_is_for_mh_fill,
  output logic                                   orig_l2_da_write,
  output logic                                   new_l2_da_write,
  output logic                                   orig_l2_fill_slot_avail,
  output logic                                   new_l2_fill_slot_avail,
  output logic [DcacheWayIdxWidth-1:0]           orig_s4_mh_refill_way,
  output logic [DcacheWayIdxWidth-1:0]           new_s4_mh_refill_way,
  output logic [DcacheLramAddrWidth-1:0]         orig_s4_mh_refill_addr,
  output logic [DcacheLramAddrWidth-1:0]         new_s4_mh_refill_addr,
  output logic [DcacheWays-1:0]                  orig_s2_tag_match,
  output logic [DcacheWays-1:0]                  new_s2_tag_match,
  output logic [DcacheWays-1:0]                  orig_s2_tag_match_qual,
  output logic [DcacheWays-1:0]                  new_s2_tag_match_qual,
  output logic                                   orig_s2_tag_match_any,
  output logic                                   new_s2_tag_match_any,
  output logic                                   orig_s2_state_hit,
  output logic                                   new_s2_state_hit,
  output logic [1:0]                             orig_s2_way_hit_state,
  output logic [1:0]                             new_s2_way_hit_state,
  output logic                                   orig_s2_hit,
  output logic                                   new_s2_hit,
  output logic                                   orig_s2_mh_req_valid,
  output logic                                   new_s2_mh_req_valid,
  output logic                                   orig_s2_mh_req_ready,
  output logic                                   new_s2_mh_req_ready,
  output logic                                   orig_s2_req_replay,
  output logic                                   new_s2_req_replay,
  output logic                                   orig_s2_req_cacheable,
  output logic                                   new_s2_req_cacheable,
  output logic                                   orig_s2_req_uc_load_2nd,
  output logic                                   new_s2_req_uc_load_2nd,
  output logic [PaSize-1:0]                      orig_s2_req_addr,
  output logic [PaSize-1:0]                      new_s2_req_addr,
  output logic [4:0]                             orig_s2_req_cmd,
  output logic [4:0]                             new_s2_req_cmd,
  output logic [3:0]                             orig_s2_req_typ,
  output logic [3:0]                             new_s2_req_typ,
  output logic                                   orig_s4_valid,
  output logic                                   new_s4_valid,
  output logic                                   orig_s4_da_write_en,
  output logic                                   new_s4_da_write_en,
  output logic                                   orig_s4_da_write_en_but_dbg,
  output logic                                   new_s4_da_write_en_but_dbg,
  output logic [PaSize-1:0]                      orig_s4_req_addr,
  output logic [PaSize-1:0]                      new_s4_req_addr,
  output logic [4:0]                             orig_s4_req_cmd,
  output logic [4:0]                             new_s4_req_cmd,
  output logic [3:0]                             orig_s4_req_typ,
  output logic [3:0]                             new_s4_req_typ,
  output logic [DcacheLramDataSize-1:0]          orig_s4_req_data_lo,
  output logic [DcacheLramDataSize-1:0]          new_s4_req_data_lo,
  output logic [DcacheLramDataSize-1:0]          orig_s4_da_write_data_0,
  output logic [DcacheLramDataSize-1:0]          new_s4_da_write_data_0
);

  logic                        reset_orig;
  minion_dcache_req            stim_s0_core_req;
  minion_satp_info             stim_satp_info;
  minion_satp_info             stim_matp_info;
  vm_status_t [NrThreads-1:0]  stim_vm_status_new;
  minion_vm_status [NrThreads-1:0] stim_vm_status_orig;
  minion_ptw_pte               stim_ptw_resp_data;
  core_dcache_ctrl             stim_core_ctrl;

  always_comb begin
    reset_orig = ~rst_ni;

    stim_s0_core_req = '0;
    stim_s0_core_req.addr = stim_req_addr_i;
    stim_s0_core_req.addr_msb_err = stim_req_addr_msb_err_i;
    stim_s0_core_req.dest.fp = stim_req_dest_fp_i;
    stim_s0_core_req.dest.addr = stim_req_dest_addr_i;
    stim_s0_core_req.dest.thread_id = stim_req_thread_id_i;
    stim_s0_core_req.cmd = dcache_cmd_e'(stim_req_cmd_i);
    stim_s0_core_req.typ = dcache_type_e'(stim_req_typ_i);
    stim_s0_core_req.gsc_cnt = stim_req_gsc_cnt_i;
    stim_s0_core_req.ps_mask = stim_req_ps_mask_i;
    stim_s0_core_req.phys = stim_req_phys_i;
    stim_s0_core_req.mem_global = stim_req_mem_global_i;
    stim_s0_core_req.gsc32_idx = stim_req_gsc32_idx_i;

    stim_satp_info = '{mode: stim_satp_mode_i, ppn: stim_satp_ppn_i};
    stim_matp_info = '{mode: stim_matp_mode_i, ppn: stim_matp_ppn_i};

    stim_vm_status_new = '{default: '0};
    stim_vm_status_new[0].prv = stim_vm_prv_t0_i;
    stim_vm_status_new[0].mprv = stim_vm_mprv_t0_i;
    stim_vm_status_new[0].mpp = stim_vm_mpp_t0_i;
    stim_vm_status_new[0].sum = stim_vm_sum_t0_i;
    stim_vm_status_new[0].mxr = stim_vm_mxr_t0_i;
    stim_vm_status_new[0].debug = stim_vm_debug_t0_i;
    stim_vm_status_new[1].prv = stim_vm_prv_t1_i;
    stim_vm_status_new[1].mprv = stim_vm_mprv_t1_i;
    stim_vm_status_new[1].mpp = stim_vm_mpp_t1_i;
    stim_vm_status_new[1].sum = stim_vm_sum_t1_i;
    stim_vm_status_new[1].mxr = stim_vm_mxr_t1_i;
    stim_vm_status_new[1].debug = stim_vm_debug_t1_i;

    stim_vm_status_orig[0] = minion_vm_status'(stim_vm_status_new[0]);
    stim_vm_status_orig[1] = minion_vm_status'(stim_vm_status_new[1]);

    stim_ptw_resp_data = '0;
    stim_ptw_resp_data.canceled_req = stim_ptw_resp_canceled_req_i;
    stim_ptw_resp_data.access_fault = stim_ptw_resp_access_fault_i;
    stim_ptw_resp_data.rsvd_for_hw = stim_ptw_resp_rsvd_for_hw_i;
    stim_ptw_resp_data.ppn = stim_ptw_resp_ppn_i;
    stim_ptw_resp_data.rsvd_for_sw = stim_ptw_resp_rsvd_for_sw_i;
    stim_ptw_resp_data.d = stim_ptw_resp_d_i;
    stim_ptw_resp_data.a = stim_ptw_resp_a_i;
    stim_ptw_resp_data.g = stim_ptw_resp_g_i;
    stim_ptw_resp_data.u = stim_ptw_resp_u_i;
    stim_ptw_resp_data.x = stim_ptw_resp_x_i;
    stim_ptw_resp_data.w = stim_ptw_resp_w_i;
    stim_ptw_resp_data.r = stim_ptw_resp_r_i;
    stim_ptw_resp_data.v = stim_ptw_resp_v_i;

    stim_core_ctrl = stim_core_ctrl_i;
    if (stim_force_ts_scp_start_i) begin
      stim_core_ctrl.reduce_start = 1'b0;
      stim_core_ctrl.tensorstore_start = 1'b1;
      stim_core_ctrl.thread_id = 1'b0;
      stim_core_ctrl.tensorstore_stride = '0;
      stim_core_ctrl.tensorstore_stride[6] = 1'b1;
      stim_core_ctrl.reduce_tensorstore_ctrl = '0;
      stim_core_ctrl.reduce_tensorstore_ctrl.tensorstore_scp.scp = 1'b1;
      stim_core_ctrl.reduce_tensorstore_ctrl.tensorstore_scp.rows = 4'h1;
      stim_core_ctrl.reduce_tensorstore_ctrl.tensorstore_scp.rate = 4'h1;
      stim_core_ctrl.reduce_tensorstore_ctrl.tensorstore_scp.start_entry = 6'h1;
    end
  end

  minion_dcache_top u_new (
    .clk_i,
    .rst_ni,
    .shire_id_i               (stim_shire_id_i),
    .shire_min_id_i           (stim_shire_min_id_i),
    .ioshire_i                (stim_ioshire_i),
    .chicken_bit_dcache_i     (stim_chicken_bit_dcache_i),
    .core_ctrl_i              (stim_core_ctrl),
    .core_ctrl_resp_o         (new_core_ctrl_resp),
    .id_core_alloc_rq_pre_i   (stim_id_core_alloc_rq_pre_i),
    .s0_core_alloc_rq_val_i   (stim_s0_core_alloc_rq_val_i),
    .id_core_gsc_i            (stim_id_core_gsc_i),
    .id_core_ready_o          (new_id_core_ready),
    .s0_core_req_valid_i      (stim_s0_core_req_valid_i),
    .s0_core_req_i            (stim_s0_core_req),
    .s0_core_gsc_i            (stim_s0_core_gsc_i),
    .s1_core_kill_i           (stim_s1_core_kill_i),
    .s1_core_store_data_i     (stim_s1_core_store_data_i),
    .s1_mprot_i               (stim_s1_mprot_i),
    .s2_core_kill_i           (stim_s2_core_kill_i),
    .s3_core_x31_i            (stim_s3_core_x31_i),
    .s1_vpu_ctrl_i            (stim_s1_vpu_ctrl_i),
    .s3_vpu_scp_resp_o        (new_s3_vpu_scp_resp),
    .s3_vpu_scp_data_o        (new_s3_vpu_scp_data),
    .s3_vpu_tenb_data_o       (new_s3_vpu_tenb_data),
    .vpu_reduce_ctrl_o        (new_vpu_reduce_ctrl),
    .s2_core_resp_int_valid_o (new_s2_core_resp_int_valid),
    .s3_core_resp_valid_o     (new_s3_core_resp_valid),
    .s3_core_resp_o           (new_s3_core_resp),
    .id_core_scoreboard_o     (new_id_core_scoreboard),
    .id_core_sb_fp_dealloc_o  (new_id_core_sb_fp_dealloc),
    .id_core_sb_int_dealloc_o (new_id_core_sb_int_dealloc),
    .s1_core_replay_next_o    (new_s1_core_replay_next),
    .s1_core_xcpt_o           (new_s1_core_xcpt),
    .s2_core_flush_pipeline_o (new_s2_core_flush_pipeline),
    .s3_ordered_o             (new_s3_ordered),
    .s3_invalidate_lr_i       (stim_s3_invalidate_lr_i),
    .wb_dcache_fp_toint_i     (stim_wb_dcache_fp_toint_i),
    .vmspagesize_i            (tlb_entry_type'(stim_vmspagesize_i)),
    .bypass_dcache_i          (stim_bypass_dcache_i),
    .mem_ctrl_override_i      (stim_mem_ctrl_override_i),
    .satp_info_i              (stim_satp_info),
    .matp_info_i              (stim_matp_info),
    .satp_info_en_i           (stim_satp_info_en_i),
    .matp_info_en_i           (stim_matp_info_en_i),
    .vm_status_i              (stim_vm_status_new),
    .tlb_invalidate_i         (stim_tlb_invalidate_i),
    .ptw_req_data_o           (new_ptw_req_data),
    .ptw_req_valid_o          (new_ptw_req_valid),
    .ptw_req_ready_i          (stim_ptw_req_ready_i),
    .ptw_resp_data_i          (stim_ptw_resp_data),
    .ptw_resp_valid_i         (stim_ptw_resp_valid_i),
    .l2_evict_req_ready_i     (stim_l2_evict_req_ready_i),
    .l2_evict_req_valid_o     (new_l2_evict_req_valid),
    .l2_evict_req_o           (new_l2_evict_req),
    .l2_miss_req_ready_i      (stim_l2_miss_req_ready_i),
    .l2_miss_req_valid_o      (new_l2_miss_req_valid),
    .l2_miss_req_o            (new_l2_miss_req),
    .l2_resp_ready_o          (new_l2_resp_ready),
    .l2_resp_valid_i          (stim_l2_resp_valid_i),
    .l2_resp_i                (stim_l2_resp_i),
    .l2_resp_ready_int_o      (new_l2_resp_ready_int),
    .l2_resp_valid_int_o      (new_l2_resp_valid_int),
    .l2_resp_int_o            (new_l2_resp_int),
    .s1_bp_conf_o             (new_s1_bp_conf),
    .s1_bp_conf_valid_o       (new_s1_bp_conf_valid),
    .dcache_idle_excl_mode_o  (new_dcache_idle_excl_mode),
    .tensor_load_err_flags_o  (new_tensor_load_err_flags),
    .cache_ops_err_flags_o    (new_cache_ops_err_flags),
    .tensor_reduce_err_flags_o(new_tensor_reduce_err_flags),
    .apb_paddr_i              (stim_apb_paddr_i),
    .apb_pwrite_i             (stim_apb_pwrite_i),
    .apb_psel_i               (stim_apb_psel_i),
    .apb_penable_i            (stim_apb_penable_i),
    .apb_pwdata_i             (stim_apb_pwdata_i),
    .apb_pready_o             (new_apb_pready),
    .apb_prdata_o             (new_apb_prdata),
    .apb_pslverr_o            (new_apb_pslverr),
    .io_events_o              (new_io_events),
    .bus_err_o                (new_bus_err),
    .bus_err_addr_o           (new_bus_err_addr),
    .csr_debug_sigs_o         (new_csr_debug_sigs),
    .sm_match_debug_signals_tl0_o  (new_sm_match_debug_signals_tl0),
    .sm_filter_debug_signals_tl0_o (new_sm_filter_debug_signals_tl0),
    .sm_data_debug_signals_tl0_o   (new_sm_data_debug_signals_tl0),
    .sm_match_debug_signals_tl1_o  (new_sm_match_debug_signals_tl1),
    .sm_filter_debug_signals_tl1_o (new_sm_filter_debug_signals_tl1),
    .sm_data_debug_signals_tl1_o   (new_sm_data_debug_signals_tl1),
    .sm_match_debug_signals_ts_o   (new_sm_match_debug_signals_ts),
    .sm_filter_debug_signals_ts_o  (new_sm_filter_debug_signals_ts),
    .sm_data_debug_signals_ts_o    (new_sm_data_debug_signals_ts),
    .sm_match_debug_signals_co_o   (new_sm_match_debug_signals_co),
    .sm_filter_debug_signals_co_o  (new_sm_filter_debug_signals_co),
    .sm_data_debug_signals_co_o    (new_sm_data_debug_signals_co),
    .sm_match_debug_signals_col2_o (new_sm_match_debug_signals_col2),
    .sm_filter_debug_signals_col2_o(new_sm_filter_debug_signals_col2),
    .sm_data_debug_signals_col2_o  (new_sm_data_debug_signals_col2),
    .sm_match_debug_signals_mh_o   (new_sm_match_debug_signals_mh),
    .sm_filter_debug_signals_mh_o  (new_sm_filter_debug_signals_mh),
    .sm_data_debug_signals_mh_o    (new_sm_data_debug_signals_mh),
    .sm_match_debug_signals_dc_o   (new_sm_match_debug_signals_dc),
    .sm_filter_debug_signals_dc_o  (new_sm_filter_debug_signals_dc),
    .sm_data_debug_signals_dc_o    (new_sm_data_debug_signals_dc)
  );

  dcache_top u_orig (
    .clock                    (clk_i),
    .reset                    (reset_orig),
    .shire_id                 (stim_shire_id_i),
    .shire_min_id             (stim_shire_min_id_i),
    .ioshire                  (stim_ioshire_i),
    .chicken_bit_dcache       (stim_chicken_bit_dcache_i),
    .core_ctrl                (stim_core_ctrl),
    .core_ctrl_resp           (orig_core_ctrl_resp),
    .id_core_alloc_rq_pre     (stim_id_core_alloc_rq_pre_i),
    .s0_core_alloc_rq_val     (stim_s0_core_alloc_rq_val_i),
    .id_core_gsc              (stim_id_core_gsc_i),
    .id_core_ready            (orig_id_core_ready),
    .s0_core_req_valid        (stim_s0_core_req_valid_i),
    .s0_core_req              (stim_s0_core_req),
    .s0_core_gsc              (stim_s0_core_gsc_i),
    .s1_core_kill             (stim_s1_core_kill_i),
    .s1_core_store_data       (stim_s1_core_store_data_i),
    .s1_mprot                 (stim_s1_mprot_i),
    .s2_core_kill             (stim_s2_core_kill_i),
    .s3_core_x31              (stim_s3_core_x31_i),
    .s1_vpu_ctrl              (stim_s1_vpu_ctrl_i),
    .s3_vpu_scp_resp          (orig_s3_vpu_scp_resp),
    .s3_vpu_scp_data          (orig_s3_vpu_scp_data),
    .s3_vpu_tenb_data         (orig_s3_vpu_tenb_data),
    .vpu_reduce_ctrl          (orig_vpu_reduce_ctrl),
    .s2_core_resp_int_valid   (orig_s2_core_resp_int_valid),
    .s3_core_resp_valid       (orig_s3_core_resp_valid),
    .s3_core_resp             (orig_s3_core_resp),
    .id_core_scoreboard       (orig_id_core_scoreboard),
    .id_core_sb_fp_dealloc    (orig_id_core_sb_fp_dealloc),
    .id_core_sb_int_dealloc   (orig_id_core_sb_int_dealloc),
    .s1_core_replay_next      (orig_s1_core_replay_next),
    .s1_core_xcpt             (orig_s1_core_xcpt),
    .s2_core_flush_pipeline   (orig_s2_core_flush_pipeline),
    .s3_ordered               (orig_s3_ordered),
    .s3_invalidate_lr         (stim_s3_invalidate_lr_i),
    .wb_dcache_fp_toint       (stim_wb_dcache_fp_toint_i),
    .vmspagesize              (tlb_entry_type'(stim_vmspagesize_i)),
    .bypass_dcache            (stim_bypass_dcache_i),
    .mem_ctrl_override        (stim_mem_ctrl_override_i),
    .satp_info                (stim_satp_info),
    .matp_info                (stim_matp_info),
    .satp_info_en             (stim_satp_info_en_i),
    .matp_info_en             (stim_matp_info_en_i),
    .vm_status                (stim_vm_status_orig),
    .tlb_invalidate           (stim_tlb_invalidate_i),
    .ptw_req_data             (orig_ptw_req_data),
    .ptw_req_valid            (orig_ptw_req_valid),
    .ptw_req_ready            (stim_ptw_req_ready_i),
    .ptw_resp_data            (stim_ptw_resp_data),
    .ptw_resp_valid           (stim_ptw_resp_valid_i),
    .l2_evict_req_ready       (stim_l2_evict_req_ready_i),
    .l2_evict_req_valid       (orig_l2_evict_req_valid),
    .l2_evict_req             (orig_l2_evict_req),
    .l2_miss_req_ready        (stim_l2_miss_req_ready_i),
    .l2_miss_req_valid        (orig_l2_miss_req_valid),
    .l2_miss_req              (orig_l2_miss_req),
    .l2_resp_ready            (orig_l2_resp_ready),
    .l2_resp_valid            (stim_l2_resp_valid_i),
    .l2_resp                  (stim_l2_resp_i),
    .l2_resp_ready_int        (orig_l2_resp_ready_int),
    .l2_resp_valid_int        (orig_l2_resp_valid_int),
    .l2_resp_int              (orig_l2_resp_int),
    .s1_bp_conf               (orig_s1_bp_conf),
    .s1_bp_conf_valid         (orig_s1_bp_conf_valid),
    .dcache_idle_excl_mode    (orig_dcache_idle_excl_mode),
    .tensor_load_err_flags    (orig_tensor_load_err_flags),
    .cache_ops_err_flags      (orig_cache_ops_err_flags),
    .tensor_reduce_err_flags  (orig_tensor_reduce_err_flags),
    .apb_paddr                (stim_apb_paddr_i),
    .apb_pwrite               (stim_apb_pwrite_i),
    .apb_psel                 (stim_apb_psel_i),
    .apb_penable              (stim_apb_penable_i),
    .apb_pwdata               (stim_apb_pwdata_i),
    .apb_pready               (orig_apb_pready),
    .apb_prdata               (orig_apb_prdata),
    .apb_pslverr              (orig_apb_pslverr),
    .io_events                (orig_io_events),
    .bus_err                  (orig_bus_err),
    .bus_err_addr             (orig_bus_err_addr),
    .csr_debug_sigs           (orig_csr_debug_sigs),
    .sm_match_debug_signals_tl0  (orig_sm_match_debug_signals_tl0),
    .sm_filter_debug_signals_tl0 (orig_sm_filter_debug_signals_tl0),
    .sm_data_debug_signals_tl0   (orig_sm_data_debug_signals_tl0),
    .sm_match_debug_signals_tl1  (orig_sm_match_debug_signals_tl1),
    .sm_filter_debug_signals_tl1 (orig_sm_filter_debug_signals_tl1),
    .sm_data_debug_signals_tl1   (orig_sm_data_debug_signals_tl1),
    .sm_match_debug_signals_ts   (orig_sm_match_debug_signals_ts),
    .sm_filter_debug_signals_ts  (orig_sm_filter_debug_signals_ts),
    .sm_data_debug_signals_ts    (orig_sm_data_debug_signals_ts),
    .sm_match_debug_signals_co   (orig_sm_match_debug_signals_co),
    .sm_filter_debug_signals_co  (orig_sm_filter_debug_signals_co),
    .sm_data_debug_signals_co    (orig_sm_data_debug_signals_co),
    .sm_match_debug_signals_col2 (orig_sm_match_debug_signals_col2),
    .sm_filter_debug_signals_col2(orig_sm_filter_debug_signals_col2),
    .sm_data_debug_signals_col2  (orig_sm_data_debug_signals_col2),
    .sm_match_debug_signals_mh   (orig_sm_match_debug_signals_mh),
    .sm_filter_debug_signals_mh  (orig_sm_filter_debug_signals_mh),
    .sm_data_debug_signals_mh    (orig_sm_data_debug_signals_mh),
    .sm_match_debug_signals_dc   (orig_sm_match_debug_signals_dc),
    .sm_filter_debug_signals_dc  (orig_sm_filter_debug_signals_dc),
    .sm_data_debug_signals_dc    (orig_sm_data_debug_signals_dc)
  );

  assign new_mh_idle = u_new.mh_idle;
  assign orig_mh_idle = u_orig.mh_idle;
  assign new_mh_l2_evict_valid = u_new.mh_l2_evict_valid;
  assign orig_mh_l2_evict_valid = u_orig.mh_l2_evict_valid;
  assign new_s2_mh_handler_ready = u_new.s2_mh_handler_ready;
  assign orig_s2_mh_handler_ready = u_orig.s2_mh_handler_ready;
  assign new_id_rq_full = u_new.id_rq_full;
  assign orig_id_rq_full = u_orig.id_rq_full;
  assign new_id_rq_half_full = u_new.id_rq_half_full;
  assign orig_id_rq_half_full = u_orig.id_rq_half_full;
  assign new_s2_rq_empty = u_new.s2_rq_empty;
  assign orig_s2_rq_empty = u_orig.s2_rq_empty;
  assign new_rq_entry_mask_available = u_new.u_replay_queue.entry_mask_available_q;
  assign orig_rq_entry_mask_available = u_orig.replay_queue.entry_mask_available;
  assign new_rq_push_age = u_new.u_replay_queue.push_age_q;
  assign orig_rq_push_age = u_orig.replay_queue.push_age;
  assign new_rq_replay_valid_prev = u_new.u_replay_queue.replay_valid_prev_q;
  assign orig_rq_replay_valid_prev = u_orig.replay_queue.replay_valid_prev;
  assign new_rq_replay_entry_prev = u_new.u_replay_queue.replay_entry_prev_q;
  assign orig_rq_replay_entry_prev = u_orig.replay_queue.replay_entry_prev;
  assign new_s1_valid = u_new.s1_valid;
  assign orig_s1_valid = u_orig.s1_valid;
  assign new_s1_valid_masked = u_new.s1_valid_masked;
  assign orig_s1_valid_masked = u_orig.s1_valid_masked;
  assign new_s1_valid_to_tlb = u_new.s1_valid_to_tlb;
  assign orig_s1_valid_to_tlb = u_orig.s1_valid_to_tlb;
  assign new_s1_valid_to_s2 = u_new.s1_valid_to_s2;
  assign orig_s1_valid_to_s2 = u_orig.s1_valid_to_s2;
  assign new_s1_phys_addr = u_new.s1_phys_addr;
  assign orig_s1_phys_addr = u_orig.s1_phys_addr;
  assign new_s1_req_addr = u_new.s1_req.addr[VaSize-1:0];
  assign orig_s1_req_addr = u_orig.s1_req.addr[VaSize-1:0];
  assign new_s1_req_cmd = u_new.s1_req.cmd;
  assign orig_s1_req_cmd = u_orig.s1_req.cmd;
  assign new_s1_req_typ = u_new.s1_req.typ;
  assign orig_s1_req_typ = u_orig.s1_req.typ;
  assign new_s1_mr_read_valid = u_new.s1_mr_read_valid;
  assign orig_s1_mr_read_valid = u_orig.s1_mr_read_valid;
  assign new_s1_tag_match = u_new.s1_tag_match;
  assign orig_s1_tag_match = u_orig.s1_tag_match;
  assign new_s1_req_replay = u_new.s1_req.replay;
  assign orig_s1_req_replay = u_orig.s1_req.replay;
  assign new_s1_req_cache_op = u_new.s1_req.cache_op;
  assign orig_s1_req_cache_op = u_orig.s1_req.cache_op;
  assign new_s1_tlb_miss = u_new.s1_tlb_resp_data.miss;
  assign orig_s1_tlb_miss = u_orig.s1_tlb_resp_data.miss;
  assign new_s1_pma_bus_err = u_new.s1_pma_bus_err;
  assign orig_s1_pma_bus_err = u_orig.s1_pma_bus_err;
  assign new_s1_rq_push_conservative = u_new.s1_rq_push_conservative;
  assign orig_s1_rq_push_conservative = u_orig.s1_rq_push_conservative;
  assign new_s2_valid = u_new.s2_valid;
  assign orig_s2_valid = u_orig.s2_valid;
  assign new_s2_valid_qual = u_new.s2_valid_qual;
  assign orig_s2_valid_qual = u_orig.s2_valid_qual;
  assign new_s2_rq_push_cancel = u_new.s2_rq_push_cancel;
  assign orig_s2_rq_push_cancel = u_orig.s2_rq_push_cancel;
  assign new_s2_rq_push = u_new.s2_rq_push;
  assign orig_s2_rq_push = u_orig.s2_rq_push;
  assign new_s2_rq_renack = u_new.s2_rq_renack;
  assign orig_s2_rq_renack = u_orig.s2_rq_renack;
  assign new_s2_rq_dealloc = u_new.s2_rq_dealloc;
  assign orig_s2_rq_dealloc = u_orig.s2_rq_dealloc;
  assign new_mh_state_0 = u_new.u_miss_handler_unit.gen_handlers[0].u_miss_handler.state_q;
  assign orig_mh_state_0 = u_orig.miss_handler_unit.handlers[0].miss_handler.state;
  assign new_mh_state_1 = u_new.u_miss_handler_unit.gen_handlers[1].u_miss_handler.state_q;
  assign orig_mh_state_1 = u_orig.miss_handler_unit.handlers[1].miss_handler.state;
  assign new_l2_resp_is_for_mh_fill = u_new.l2_resp_is_for_mh_fill;
  assign orig_l2_resp_is_for_mh_fill = u_orig.l2_resp_is_for_mh_fill;
  assign new_l2_da_write = u_new.l2_da_write;
  assign orig_l2_da_write = u_orig.l2_da_write;
  assign new_l2_fill_slot_avail = u_new.l2_fill_slot_avail;
  assign orig_l2_fill_slot_avail = u_orig.l2_fill_slot_avail;
  assign new_s4_mh_refill_way = u_new.s4_mh_refill_way;
  assign orig_s4_mh_refill_way = u_orig.s4_mh_refill_way;
  assign new_s4_mh_refill_addr = u_new.s4_mh_refill_addr;
  assign orig_s4_mh_refill_addr = u_orig.s4_mh_refill_addr;
  assign new_s2_tag_match = u_new.s2_tag_match;
  assign orig_s2_tag_match = u_orig.s2_tag_match;
  assign new_s2_tag_match_qual = u_new.s2_tag_match_qual;
  assign orig_s2_tag_match_qual = u_orig.s2_tag_match_qual;
  assign new_s2_tag_match_any = u_new.s2_tag_match_any;
  assign orig_s2_tag_match_any = u_orig.s2_tag_match_any;
  assign new_s2_state_hit = u_new.s2_state_hit;
  assign orig_s2_state_hit = u_orig.s2_state_hit;
  assign new_s2_way_hit_state = u_new.s2_way_hit_state;
  assign orig_s2_way_hit_state = u_orig.s2_way_hit_state;
  assign new_s2_hit = u_new.s2_hit;
  assign orig_s2_hit = u_orig.s2_hit;
  assign new_s2_mh_req_valid = u_new.s2_mh_req_valid;
  assign orig_s2_mh_req_valid = u_orig.s2_mh_req_valid;
  assign new_s2_mh_req_ready = u_new.s2_mh_req_ready;
  assign orig_s2_mh_req_ready = u_orig.s2_mh_req_ready;
  assign new_s2_req_replay = u_new.s2_req.replay;
  assign orig_s2_req_replay = u_orig.s2_req.replay;
  assign new_s2_req_cacheable = u_new.s2_req.cacheable;
  assign orig_s2_req_cacheable = u_orig.s2_req.cacheable;
  assign new_s2_req_uc_load_2nd = u_new.s2_req.uc_load_2nd;
  assign orig_s2_req_uc_load_2nd = u_orig.s2_req.uc_load_2nd;
  assign new_s2_req_addr = u_new.s2_req.addr[PaSize-1:0];
  assign orig_s2_req_addr = u_orig.s2_req.addr[PaSize-1:0];
  assign new_s2_req_cmd = u_new.s2_req.cmd;
  assign orig_s2_req_cmd = u_orig.s2_req.cmd;
  assign new_s2_req_typ = u_new.s2_req.typ;
  assign orig_s2_req_typ = u_orig.s2_req.typ;
  assign new_s4_valid = u_new.s4_valid;
  assign orig_s4_valid = u_orig.s4_valid;
  assign new_s4_da_write_en = u_new.s4_da_write_en;
  assign orig_s4_da_write_en = u_orig.s4_da_write_en;
  assign new_s4_da_write_en_but_dbg = u_new.s4_da_write_en_but_dbg;
  assign orig_s4_da_write_en_but_dbg = u_orig.s4_da_write_en_but_dbg;
  assign new_s4_req_addr = u_new.s4_req.addr[PaSize-1:0];
  assign orig_s4_req_addr = u_orig.s4_req.addr[PaSize-1:0];
  assign new_s4_req_cmd = u_new.s4_req.cmd;
  assign orig_s4_req_cmd = u_orig.s4_req.cmd;
  assign new_s4_req_typ = u_new.s4_req.typ;
  assign orig_s4_req_typ = u_orig.s4_req.typ;
  assign new_s4_req_data_lo = u_new.s4_req.data[DcacheLramDataSize-1:0];
  assign orig_s4_req_data_lo = u_orig.s4_req.data[DcacheLramDataSize-1:0];
  assign new_s4_da_write_data_0 = u_new.s4_da_write_data[0].data;
  assign orig_s4_da_write_data_0 = u_orig.s4_da_write_data[0].data;

  always_comb begin
    for (int unsigned i = 0; i < DcacheReplayqSize; i++) begin
      new_rq_pending[i] = u_new.u_replay_queue.replayq_ready_info_q[i].pending;
      orig_rq_pending[i] = u_orig.replay_queue.replayq_ready_info[i].pending;
      new_rq_wait_uc_load[i] = u_new.u_replay_queue.replayq_ready_info_q[i].wait_uc_load;
      orig_rq_wait_uc_load[i] = u_orig.replay_queue.replayq_ready_info[i].wait_uc_load;
    end
  end

endmodule
