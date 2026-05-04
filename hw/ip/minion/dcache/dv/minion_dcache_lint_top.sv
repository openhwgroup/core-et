// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Lint-only wrapper for minion_dcache leaf modules.

/* verilator lint_off UNUSED */  // Wrapper exists only to provide a single lint top; DUT outputs are intentionally unobserved
module minion_dcache_lint_top
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
;

  logic            clk_i;
  logic            rst_ni;
  logic [2:0]      atomic_addr;
  dcache_cmd_e     atomic_cmd;
  logic [1:0]      atomic_typ;
  logic [63:0]     atomic_lhs;
  logic [63:0]     atomic_rhs;
  logic [63:0]     atomic_out;

  logic [4:0]      merge_addr;
  dcache_type_e    merge_typ;
  logic [7:0]      merge_ps_mask;
  logic [255:0]    merge_orig_data;
  logic [255:0]    merge_store_data;
  logic            merge_enable_lo;
  logic            merge_enable_hi;
  logic [255:0]    merge_out;

  logic            pma_ioshire;
  logic            pma_af_skip;
  dcache_pma_req_t pma_req;
  esr_mprot_t      pma_mprot;
  logic            pma_valid_addr;
  vm_status_t      pma_vm_status;
  dcache_pma_resp_t pma_resp;

  logic [DcacheSetIdxWidth-1:0]   lru_read_set;
  logic [DcacheLruSetSize-1:0]    lru_read_data;
  logic                           lru_write_en;
  logic [DcacheSetIdxWidth-1:0]   lru_write_set;
  logic [DcacheLruSetSize-1:0]    lru_write_data;

  logic [DcacheSetIdxWidth-1:0]   meta_read_set;
  logic                           meta_write_valid;
  logic                           meta_write_valid_prev;
  dcache_meta_write_req_t         meta_write_req;
  logic [DcacheWays-1:0]          meta_read_resp_valid;
  dcache_meta_data_t              meta_read_resp [DcacheWays];
  logic                           meta_clear_all;
  logic                           meta_clear_low;

  logic                           buf_full;
  logic                           buf_reduce_full;
  logic                           buf_alloc_req_pre;
  logic                           buf_alloc_req_val;
  logic                           buf_alloc_gsc;
  logic                           buf_reduce_alloc_req;
  logic                           buf_s2_write_needed_core;
  logic                           buf_s2_write_needed;
  logic                           buf_s2_write_en;
  logic                           buf_s2_write_replay;
  logic [DcacheBufferIdxWidth-1:0] buf_s2_write_replay_entry;
  logic [DcacheDataSize-1:0]      buf_s3_write_data;
  logic [DcacheBufferIdxWidth-1:0] buf_s2_write_entry;
  logic                           buf_s3_write_en;
  logic                           buf_s3_write_en_ext;
  logic                           buf_s2_write_en_ext_prev;
  logic [DcacheBufferIdxWidth-1:0] buf_s3_write_entry_ext;
  logic [DcacheDataSize-1:0]      buf_s3_write_data_ext;
  logic                           buf_s2_dealloc;
  logic [DcacheBufferIdxWidth-1:0] buf_s2_dealloc_entry;
  logic                           buf_s1_dealloc_uc;
  logic [DcacheBufferIdxWidth-1:0] buf_s1_dealloc_uc_entry;
  logic                           buf_reduce_dealloc_req;
  logic [DcacheBufferIdxWidth-1:0] buf_reduce_dealloc_entry;
  logic [DcacheBufferIdxWidth-1:0] buf_s1_read_entry;
  logic [DcacheDataSize-1:0]      buf_s1_read_data;
  logic [6:0]                     lram_rd_addr;
  logic [6:0]                     lram_wr_addr;
  logic                           lram_rd_enable;
  logic                           lram_rd_enablelob;
  logic                           lram_rd_enablehib;
  logic                           lram_wr_enablehi;
  logic                           lram_wr_enablehi_pre;
  logic                           lram_wr_enablelo;
  logic                           lram_wr_enablelo_pre;
  logic [63:0]                    lram64_rd_data;
  logic [63:0]                    lram64_wr_data;
  logic [71:0]                    lram72_rd_data;
  logic [71:0]                    lram72_wr_data;
  dft_t                           dft;
  ram_cfg_t                       ram_cfg;
  logic [7:0]                     prim_ram_2p_a_rdata;
  logic [7:0]                     prim_ram_2p_b_rdata;
  logic                           prim_ram_2p_alert;
  logic                           data_array_read_valid;
  dcache_da_read_req_t            data_array_read_req [DcacheLramNumBanks];
  dcache_da_data_t                data_array_read_resp;
  logic                           data_array_s3_write_valid;
  logic                           data_array_s4_write_valid;
  dcache_da_write_req_t           data_array_write_req [DcacheLramNumBanks];
  logic                           wb_req_ready;
  logic                           wb_req_valid;
  dcache_wb_req_t                 wb_req_data;
  logic                           wb_data_req_bid;
  logic                           wb_data_req_grant;
  logic                           wb_data_req_valid;
  dcache_da_read_req_t            wb_data_req;
  logic                           wb_data_req_nack;
  dcache_da_data_t                wb_data_resp;
  logic                           wb_l2_req_ready;
  logic                           wb_l2_req_valid;
  et_link_minion_evict_req_info_t wb_l2_req_data;
  logic                           rq_full;
  logic                           rq_half_full;
  logic [NrThreads-1:0]           rq_empty;
  logic                           rq_alloc_req_pre;
  logic                           rq_alloc_req_val;
  logic                           rq_alloc_gsc;
  logic                           rq_push_req;
  logic                           rq_push_req_prev;
  dcache_req_t                    rq_req_data;
  logic                           rq_cacheable;
  logic                           rq_req_buf_alloc;
  logic [DcacheBufferIdxWidth-1:0] rq_req_buf_entry;
  dcache_replay_conf_t            rq_req_conf_type;
  logic                           rq_misaligned_1st;
  logic                           rq_uc_load_1st;
  logic                           rq_uc_load_ok;
  logic                           rq_miss_fill_ok;
  logic                           rq_conflict;
  logic                           rq_conflict_to_co;
  logic                           rq_rearm;
  logic                           rq_rearm_prev;
  logic [DcacheReplayqAddrWidth-1:0] rq_rearm_entry;
  logic                           rq_dealloc;
  logic [DcacheReplayqAddrWidth-1:0] rq_dealloc_entry;
  logic [DcacheMhFileSize-1:0]    rq_miss_available;
  logic [DcacheMhFileSize-1:0]    rq_miss_fill_error;
  logic                           rq_uc_load_fill;
  logic [DcacheBufferIdxWidth-1:0] rq_uc_load_fill_id;
  logic                           rq_uc_load_fill_ok;
  logic                           rq_vpu_rf_in_use;
  logic                           rq_replay_bid;
  logic                           rq_replay_grant;
  logic                           rq_replay_valid;
  dcache_replay_req_t             rq_replay_req;
  logic [DcacheReplayqAddrWidth-1:0] rq_replay_entry;
  logic [DcacheReplayqAgeWidth-1:0] rq_replay_age;
  logic                           rq_replay_uc_ok;
  logic                           rq_replay_miss_ok;
  dcache_minion_scoreboard        rq_scoreboard;
  logic [NrThreads-1:0]           rq_fp_sb_dealloc;
  logic [NrThreads-1:0]           rq_int_sb_dealloc;
  logic [$clog2(ShireCoopMinMaskSize)-1:0] mh_neigh_min_id;
  logic                           mh_req_pri_val;
  logic                           mh_req_pri_rdy;
  logic                           mh_req_sec_val;
  logic                           mh_req_sec_rdy;
  logic                           mh_can_accept_uc;
  dcache_mh_req_t                 mh_req_info;
  logic                           mh_tag_match;
  logic                           mh_evict_match;
  logic [DcacheSetIdxWidth-1:0]   mh_set;
  dcache_miss_info_t              mh_inflight_misses [DcacheMhFileSize];
  logic                           mh_inflight_reduce;
  logic                           mh_going_to_uc_store;
  logic                           mh_idle;
  logic                           mh_wait_l2_to_ba_resp;
  logic                           mh_have_l2_to_ba_resp;
  logic                           mh_l2_evict_ready;
  logic                           mh_l2_evict_valid;
  et_link_minion_evict_req_info_t mh_l2_evict_req;
  logic                           mh_l2_miss_ready;
  logic                           mh_l2_miss_valid;
  et_link_minion_miss_req_info_t  mh_l2_miss_req;
  logic                           mh_l2_resp_valid;
  et_link_minion_rsp_info_t       mh_l2_resp_data;
  logic                           mh_l2_resp_slot_avail;
  logic                           mh_l2_resp_stall;
  logic [DcacheWayIdxWidth-1:0]   mh_refill_way;
  logic [DcacheLramAddrWidth-1:0] mh_refill_addr;
  logic                           mh_force_ba_rptr_next;
  logic                           mh_force_ba_rptr_qual;
  logic [DcacheBufferIdxWidth-1:0] mh_ba_rptr;
  logic                           mh_ba_is_amo;
  logic                           mh_meta_write_ready;
  logic                           mh_meta_write_valid_next;
  logic                           mh_meta_write_valid;
  logic                           mh_meta_write_clear_slock;
  dcache_meta_write_req_t         mh_meta_write_req;
  logic                           mh_writeback_ready;
  logic                           mh_writeback_valid;
  dcache_wb_req_t                 mh_writeback_req;
  logic                           mh_writeback_done;
  logic [NrThreads-1:0]           mh_bus_err;
  logic [PaSize-1:0]              mh_bus_err_addr;
  logic                           mh_fill_bus_error;
  logic                           mh_uc_load_bus_error;
  logic                           mh_probe_rdy;
  logic [NrThreads-1:0]           mh_fence_rdy;
  logic                           mh_metaw_rdy;
  logic                           mh_ready;
  logic [DcacheDbgCsrMhSize-1:0]  mh_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0] mh_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] mh_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] mh_sm_data_debug_signals;
  logic                           mhu_req_ready;
  logic                           mhu_req_valid;
  dcache_mh_req_t                 mhu_req_info;
  logic [DcacheMhFileSize-1:0]    mhu_req_assigned;
  logic [DcacheMhFileSize-1:0]    mhu_req_accepted;
  logic                           mhu_can_accept_uc;
  logic                           mhu_wait_l2_to_ba_resp;
  logic                           mhu_inflight_reduce;
  logic                           mhu_going_to_uc_store;
  logic                           mhu_l2_evict_ready;
  logic                           mhu_l2_evict_valid;
  et_link_minion_evict_req_info_t mhu_l2_evict_req;
  logic                           mhu_l2_miss_ready;
  logic                           mhu_l2_miss_valid;
  et_link_minion_miss_req_info_t  mhu_l2_miss_req;
  logic                           mhu_l2_resp_valid;
  et_link_minion_rsp_info_t       mhu_l2_resp_data;
  logic                           mhu_l2_resp_slot_avail;
  logic [DcacheMhFileSize-1:0]    mhu_l2_resp_stall;
  logic [DcacheWayIdxWidth-1:0]   mhu_refill_way;
  logic [DcacheLramAddrWidth-1:0] mhu_refill_addr;
  logic                           mhu_meta_write_ready;
  logic                           mhu_meta_write_valid_next;
  logic                           mhu_meta_write_valid;
  logic                           mhu_meta_write_clear_slock;
  dcache_meta_write_req_t         mhu_meta_write_req;
  logic                           mhu_writeback_ready;
  logic                           mhu_writeback_valid;
  dcache_wb_req_t                 mhu_writeback_req;
  logic                           mhu_writeback_done;
  logic                           mhu_force_ba_rptr;
  logic                           mhu_force_ba_rptr_qual;
  logic [DcacheBufferIdxWidth-1:0] mhu_ba_rptr;
  logic                           mhu_ba_is_amo;
  logic [DcacheBufferIdxWidth-1:0] mhu_ba_wptr;
  logic                           mhu_idle;
  logic [DcacheMhFileSize-1:0]    mhu_probe_rdy;
  logic [DcacheMhFileSize-1:0]    mhu_metaw_rdy;
  logic [NrThreads-1:0]           mhu_fence_rdy;
  logic [DcacheMhFileSize-1:0]    mhu_handler_ready;
  logic [DcacheMhFileSize-1:0]    mhu_handler_fill_bus_error;
  logic [DcacheMhFileSize-1:0]    mhu_handler_uc_load_bus_error;
  logic                           mhu_match_miss_line;
  logic [NrThreads-1:0]           mhu_bus_err;
  logic [PaSize-1:0]              mhu_bus_err_addr;
  logic [DcacheMhFileSize*DcacheDbgCsrMhSize-1:0] mhu_csr_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0] mhu_sm_match_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] mhu_sm_filter_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0] mhu_sm_data_debug_signals;
  logic                           co_chicken_bit_dcache;
  logic                           co_busy;
  logic                           co_cache_op_ready;
  logic                           co_cache_op_valid;
  core_dcache_op_req_t            co_cache_op_req;
  core_dcache_op_req_next_t       co_cache_op_req_next;
  logic [NrThreads-1:0][1:0]      co_th_id_complete;
  logic                           co_meta_read_bid;
  logic                           co_meta_read_grant;
  logic                           co_meta_read_valid;
  logic                           co_meta_read_phys;
  logic                           co_meta_read_force_set;
  logic                           co_meta_read_thread_id;
  logic                           co_meta_read_skip_pma;
  logic [VaSize-1:0]              co_meta_read_addr;
  logic                           co_meta_read_addr_msb_err;
  logic                           co_meta_write_valid_next;
  logic                           co_meta_write_valid;
  dcache_meta_write_req_t         co_meta_write_req;
  logic                           co_nack_set_valid;
  logic [DcacheSetIdxWidth-1:0]   co_nack_set_value;
  logic                           co_writeback_ready;
  logic                           co_writeback_valid;
  dcache_wb_req_t                 co_writeback_req;
  logic                           co_writeback_done;
  logic                           co_id_rq_full;
  logic                           co_id_co_pf_bid;
  logic                           co_id_co_pf_grant;
  logic                           co_s0_co_pf_valid;
  dcache_req_t                    co_s0_co_pf_req;
  logic                           co_cache_op_ready_byp;
  logic                           co_cache_op_valid_byp;
  core_dcache_op_req_t            co_cache_op_req_byp;
  logic                           co_mh_clear_slock_valid;
  logic [DcacheSetIdxWidth-1:0]   co_mh_clear_slock_set;
  logic [DcacheWayIdxWidth-1:0]   co_mh_clear_slock_way;
  logic [DcacheSets-1:0][DcacheWays-1:0] co_hlock_state;
  logic [DcacheSets-1:0][DcacheWays-1:0] co_slock_state;
  logic                           co_da_clear_en;
  logic                           co_da_clear_val;
  logic [DcacheSetIdxWidth-1:0]   co_da_clear_set;
  logic [DcacheWayIdxWidth-1:0]   co_da_clear_way;
  logic                           co_da_clear_idx;
  logic                           co_da_clear_rdy;
  logic [DcacheMhFileSize-1:0]    co_mh_probe_rdy;
  logic [DcacheMhFileSize-1:0]    co_mh_metaw_rdy;
  logic                           co_rq_conflict;
  logic                           co_cfg_clear_all;
  logic                           co_cfg_clear_low;
  logic                           co_cfg_alloc_scp;
  dcache_mode_t                   co_cfg_cache_mode;
  minion_pkg::vm_status_t [NrThreads-1:0] co_vm_status;
  minion_pkg::vm_status_t         co_vm_status_co;
  logic [DcacheErrFlagRange:0][NrThreads-1:0] co_err_flags;
  logic [DcacheDbgCsrCouSize-1:0] co_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0] co_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] co_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] co_sm_data_debug_signals;
  logic [NumShireIdsBits-1:0] coul2_shire_id;
  logic [DcacheCoL2MaxOpsBits-1:0] coul2_cacheop_max;
  logic [DcacheCoL2ReprateBits-1:0] coul2_cacheop_reprate;
  logic                           coul2_chicken_bit_dcache;
  logic                           coul2_busy;
  logic [NrThreads-1:0][1:0]      coul2_th_id_complete;
  logic                           coul2_l2_resp_valid;
  et_link_minion_rsp_info_t       coul2_l2_resp;
  logic                           coul2_cache_op_ready;
  logic                           coul2_cache_op_valid;
  core_dcache_op_req_t            coul2_cache_op_req;
  core_dcache_op_req_next_t       coul2_cache_op_req_next;
  logic                           coul2_l2_req_ready;
  logic                           coul2_l2_req_valid;
  et_link_minion_evict_req_info_t coul2_l2_req_data;
  logic                           coul2_cache_op_ready_byp;
  logic                           coul2_cache_op_valid_byp;
  core_dcache_op_req_t            coul2_cache_op_req_byp;
  logic                           coul2_s0_available;
  logic                           coul2_s0_addr_load;
  logic [VaSize-1:0]              coul2_s0_addr;
  logic                           coul2_s0_addr_msb_err;
  logic                           coul2_s0_thread_id;
  logic                           coul2_s1_available;
  logic                           coul2_s1_addr_load;
  logic                           coul2_s1_tlb_wait;
  logic                           coul2_s1_tlb_fail;
  logic [PaSize-PaUntransSize-1:0] coul2_s1_tlb_addr;
  minion_pkg::vm_status_t [NrThreads-1:0] coul2_vm_status;
  minion_pkg::vm_status_t         coul2_vm_status_co;
  minion_pkg::vm_status_t         coul2_vm_status_l2;
  logic [NrThreads-1:0]           coul2_err_flags;
  logic [NrThreads-1:0]           coul2_bus_err;
  logic [DcacheDbgCsrCoul2Size-1:0] coul2_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0] coul2_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] coul2_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] coul2_sm_data_debug_signals;
  dcache_mode_t                   scp_ctrl_mode;
  logic                           scp_dcache_idle;
  logic                           scp_dcache_cfg_stall;
  logic                           scp_dcache_cfg_clear_all;
  logic                           scp_dcache_cfg_clear_low;
  logic                           scp_dcache_cfg_alloc_scp;
  logic                           scp_da_clear_en;
  logic                           scp_da_clear_val;
  logic [DcacheSetIdxWidth-1:0]   scp_da_clear_set;
  logic [DcacheWayIdxWidth-1:0]   scp_da_clear_way;
  logic                           scp_da_clear_idx;
  logic                           scp_da_clear_rdy;
  dcache_mode_t                   scp_dcache_cfg_mode;
  logic [$clog2(ShireCoopMinMaskSize)-1:0] tex_neigh_min_id;
  logic                           tex_chicken_bit_dcache;
  logic                           tex_idle;
  logic                           tex_send_ready;
  logic                           tex_send_valid;
  texsend_control_t               tex_send_ctrl;
  logic                           tex_l2_resp_valid;
  et_link_minion_rsp_info_t       tex_l2_resp;
  logic                           tex_l2_req_ready;
  logic                           tex_l2_req_valid;
  et_link_minion_miss_req_info_t  tex_l2_req;
  logic                           tex_writeback_ready;
  logic                           tex_writeback_valid;
  dcache_wb_req_t                 tex_writeback_req;
  logic                           tex_writeback_done;
  logic [DcacheDbgCsrTxsndSize-1:0] tex_csr_debug_signals;
  logic [MinPerSBits-1:0]         tl_shire_min_id;
  logic                           tl_chicken_bit_dcache;
  logic                           tl_busy;
  logic [1:0]                     tl_id_complete;
  core_dcache_ctrl                tl_tensor_ctrl;
  logic                           tl_s0_available;
  logic                           tl_s0_addr_load;
  logic [VaSize-1:0]              tl_s0_addr;
  logic                           tl_s0_addr_msb_err;
  logic                           tl_s0_thread_id;
  logic                           tl_s1_available;
  logic                           tl_s1_addr_load;
  logic                           tl_s1_tlb_wait;
  logic                           tl_s1_tlb_fail;
  logic [PaSize-1:0]              tl_s2_addr;
  logic                           tl_l2_req_ready_coop;
  logic                           tl_l2_req_valid_coop;
  logic                           tl_l2_req_ready_no_coop;
  logic                           tl_l2_req_valid_no_coop;
  et_link_minion_miss_req_info_t  tl_l2_req;
  logic                           tl_l2_resp_valid;
  et_link_minion_rsp_info_t       tl_l2_resp;
  logic [DcacheWayIdxWidth-1:0]   tl_load_way;
  logic [DcacheLramAddrWidth-1:0] tl_load_addr;
  logic [CoreL2BlockExtSize-1:0]  tl_load_data;
  logic [DcacheLramNumBanks-1:0]  tl_load_valid;
  logic                           tl_load_valid_pre;
  logic                           tl_load_ready;
  logic                           tl_load_capture;
  logic                           tl_tfma_enabled;
  logic                           tl_tenb_tstore_busy;
  logic                           tl_tenb_credit;
  logic [DcacheTenbBits-1:0]      tl_tenb_credit_entry;
  logic                           tl_tenb_written;
  logic                           tl_tenb_ready;
  logic                           tl_tenb_flush;
  vm_status_t [NrThreads-1:0]     tl_vm_status;
  vm_status_t                     tl_vm_status_tl;
  logic [DcacheTlErrorBits-1:0]   tl_err_flags;
  logic                           tl_start;
  logic                           tl_op;
  logic [NrThreads-1:0]           tl_bus_err;
  logic [DcacheDbgCsrTlSize-1:0]  tl_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0] tl_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] tl_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] tl_sm_data_debug_signals;
  tlb_entry_type                  tlb_vmspagesize;
  dcache_tlb_req_t                tlb_req_data;
  logic                           tlb_req_valid;
  dcache_tlb_resp_t               tlb_resp_data;
  minion_satp_info                tlb_satp_info;
  minion_satp_info                tlb_matp_info;
  logic                           tlb_satp_info_en;
  logic                           tlb_matp_info_en;
  logic                           tlb_invalidate;
  minion_ptw_req                  tlb_ptw_req_data;
  logic                           tlb_ptw_req_valid;
  logic                           tlb_ptw_req_ready;
  minion_ptw_pte                  tlb_ptw_resp_data;
  logic                           tlb_ptw_resp_valid;
  logic                           tlb_vm_enabled;

  assign clk_i = 1'b0;
  assign rst_ni = 1'b1;
  assign atomic_addr = '0;
  assign atomic_cmd = DcCmdXrd;
  assign atomic_typ = '0;
  assign atomic_lhs = '0;
  assign atomic_rhs = '0;

  assign merge_addr = '0;
  assign merge_typ = DcTypeB;
  assign merge_ps_mask = '0;
  assign merge_orig_data = '0;
  assign merge_store_data = '0;
  assign merge_enable_lo = 1'b0;
  assign merge_enable_hi = 1'b0;

  assign pma_ioshire = 1'b0;
  assign pma_af_skip = 1'b0;
  assign pma_req = '0;
  assign pma_mprot = '0;
  assign pma_valid_addr = 1'b0;
  assign pma_vm_status = '0;

  assign lru_read_set = '0;
  assign lru_write_en = 1'b0;
  assign lru_write_set = '0;
  assign lru_write_data = '0;

  assign meta_read_set = '0;
  assign meta_write_valid = 1'b0;
  assign meta_write_valid_prev = 1'b0;
  assign meta_write_req = '0;
  assign meta_clear_all = 1'b0;
  assign meta_clear_low = 1'b0;

  assign buf_alloc_req_pre = 1'b0;
  assign buf_alloc_req_val = 1'b0;
  assign buf_alloc_gsc = 1'b0;
  assign buf_reduce_alloc_req = 1'b0;
  assign buf_s2_write_needed_core = 1'b0;
  assign buf_s2_write_needed = 1'b0;
  assign buf_s2_write_en = 1'b0;
  assign buf_s2_write_replay = 1'b0;
  assign buf_s2_write_replay_entry = '0;
  assign buf_s3_write_data = '0;
  assign buf_s3_write_en_ext = 1'b0;
  assign buf_s2_write_en_ext_prev = 1'b0;
  assign buf_s3_write_entry_ext = '0;
  assign buf_s3_write_data_ext = '0;
  assign buf_s2_dealloc = 1'b0;
  assign buf_s2_dealloc_entry = '0;
  assign buf_s1_dealloc_uc = 1'b0;
  assign buf_s1_dealloc_uc_entry = '0;
  assign buf_reduce_dealloc_req = 1'b0;
  assign buf_reduce_dealloc_entry = '0;
  assign buf_s1_read_entry = '0;
  assign lram_rd_addr = '0;
  assign lram_wr_addr = '0;
  assign lram_rd_enable = 1'b0;
  assign lram_rd_enablelob = 1'b0;
  assign lram_rd_enablehib = 1'b0;
  assign lram_wr_enablehi = 1'b0;
  assign lram_wr_enablehi_pre = 1'b0;
  assign lram_wr_enablelo = 1'b0;
  assign lram_wr_enablelo_pre = 1'b0;
  assign lram64_wr_data = '0;
  assign lram72_wr_data = '0;
  assign dft = '0;
  assign ram_cfg = '0;
  assign data_array_read_valid = 1'b0;
  assign data_array_s3_write_valid = 1'b0;
  assign data_array_s4_write_valid = 1'b0;
  assign wb_req_valid = 1'b0;
  assign wb_req_data = '0;
  assign wb_data_req_grant = 1'b0;
  assign wb_data_req_nack = 1'b0;
  assign wb_data_resp = '0;
  assign wb_l2_req_ready = 1'b0;
  assign rq_alloc_req_pre = 1'b0;
  assign rq_alloc_req_val = 1'b0;
  assign rq_alloc_gsc = 1'b0;
  assign rq_push_req = 1'b0;
  assign rq_push_req_prev = 1'b0;
  assign rq_req_data = '0;
  assign rq_cacheable = 1'b0;
  assign rq_req_buf_alloc = 1'b0;
  assign rq_req_buf_entry = '0;
  assign rq_req_conf_type = '0;
  assign rq_misaligned_1st = 1'b0;
  assign rq_uc_load_1st = 1'b0;
  assign rq_uc_load_ok = 1'b0;
  assign rq_miss_fill_ok = 1'b0;
  assign rq_rearm = 1'b0;
  assign rq_rearm_prev = 1'b0;
  assign rq_rearm_entry = '0;
  assign rq_dealloc = 1'b0;
  assign rq_dealloc_entry = '0;
  assign rq_miss_available = '0;
  assign rq_miss_fill_error = '0;
  assign rq_uc_load_fill = 1'b0;
  assign rq_uc_load_fill_id = '0;
  assign rq_uc_load_fill_ok = 1'b0;
  assign rq_vpu_rf_in_use = 1'b0;
  assign rq_replay_grant = 1'b0;
  assign mh_neigh_min_id = '0;
  assign mh_req_pri_val = 1'b0;
  assign mh_req_sec_val = 1'b0;
  assign mh_req_info = '0;
  assign mh_inflight_misses = '{default: '0};
  assign mh_inflight_reduce = 1'b0;
  assign mh_l2_evict_ready = 1'b0;
  assign mh_l2_miss_ready = 1'b0;
  assign mh_l2_resp_valid = 1'b0;
  assign mh_l2_resp_data = '0;
  assign mh_l2_resp_slot_avail = 1'b0;
  assign mh_meta_write_ready = 1'b0;
  assign mh_writeback_ready = 1'b0;
  assign mh_writeback_done = 1'b0;
  assign mhu_req_valid = 1'b0;
  assign mhu_req_info = '0;
  assign mhu_inflight_reduce = 1'b0;
  assign mhu_l2_evict_ready = 1'b0;
  assign mhu_l2_miss_ready = 1'b0;
  assign mhu_l2_resp_valid = 1'b0;
  assign mhu_l2_resp_data = '0;
  assign mhu_l2_resp_slot_avail = 1'b0;
  assign mhu_meta_write_ready = 1'b0;
  assign mhu_writeback_ready = 1'b0;
  assign mhu_writeback_done = 1'b0;
  assign co_chicken_bit_dcache = 1'b0;
  assign co_cache_op_valid = 1'b0;
  assign co_cache_op_req = '0;
  assign co_cache_op_req_next = '0;
  assign co_meta_read_grant = 1'b0;
  assign co_writeback_ready = 1'b0;
  assign co_writeback_done = 1'b0;
  assign co_id_rq_full = 1'b0;
  assign co_id_co_pf_grant = 1'b0;
  assign co_cache_op_ready_byp = 1'b0;
  assign co_mh_clear_slock_valid = 1'b0;
  assign co_mh_clear_slock_set = '0;
  assign co_mh_clear_slock_way = '0;
  assign co_da_clear_rdy = 1'b0;
  assign co_mh_probe_rdy = '0;
  assign co_mh_metaw_rdy = '0;
  assign co_rq_conflict = 1'b0;
  assign co_cfg_clear_all = 1'b0;
  assign co_cfg_clear_low = 1'b0;
  assign co_cfg_alloc_scp = 1'b0;
  assign co_cfg_cache_mode = DcacheModeShared;
  assign co_vm_status = '0;
  assign coul2_shire_id = '0;
  assign coul2_cacheop_max = '0;
  assign coul2_cacheop_reprate = '0;
  assign coul2_chicken_bit_dcache = 1'b0;
  assign coul2_l2_resp_valid = 1'b0;
  assign coul2_l2_resp = '0;
  assign coul2_cache_op_valid = 1'b0;
  assign coul2_cache_op_req = '0;
  assign coul2_cache_op_req_next = '0;
  assign coul2_l2_req_ready = 1'b0;
  assign coul2_cache_op_valid_byp = 1'b0;
  assign coul2_cache_op_req_byp = '0;
  assign coul2_s0_available = 1'b0;
  assign coul2_s1_available = 1'b0;
  assign coul2_s1_tlb_wait = 1'b0;
  assign coul2_s1_tlb_fail = 1'b0;
  assign coul2_s1_tlb_addr = '0;
  assign coul2_vm_status = '0;
  assign coul2_vm_status_co = '0;
  assign scp_ctrl_mode = DcacheModeShared;
  assign scp_dcache_idle = 1'b0;
  assign scp_da_clear_rdy = 1'b0;
  assign tex_neigh_min_id = '0;
  assign tex_chicken_bit_dcache = 1'b0;
  assign tex_send_valid = 1'b0;
  assign tex_send_ctrl = '0;
  assign tex_l2_resp_valid = 1'b0;
  assign tex_l2_resp = '0;
  assign tex_l2_req_ready = 1'b0;
  assign tex_writeback_ready = 1'b0;
  assign tex_writeback_done = 1'b0;
  assign tl_shire_min_id = '0;
  assign tl_chicken_bit_dcache = 1'b0;
  assign tl_tensor_ctrl = '0;
  assign tl_s0_available = 1'b0;
  assign tl_s1_available = 1'b0;
  assign tl_s1_tlb_wait = 1'b0;
  assign tl_s1_tlb_fail = 1'b0;
  assign tl_s2_addr = '0;
  assign tl_l2_req_ready_coop = 1'b0;
  assign tl_l2_req_ready_no_coop = 1'b0;
  assign tl_l2_resp_valid = 1'b0;
  assign tl_l2_resp = '0;
  assign tl_load_ready = 1'b0;
  assign tl_tfma_enabled = 1'b0;
  assign tl_tenb_tstore_busy = 1'b0;
  assign tl_tenb_credit = 1'b0;
  assign tl_tenb_credit_entry = '0;
  assign tl_tenb_written = 1'b0;
  assign tl_vm_status = '{default: '0};
  assign tlb_vmspagesize = tlb_entry_type_4K;
  assign tlb_req_data = '0;
  assign tlb_req_valid = 1'b0;
  assign tlb_satp_info = '0;
  assign tlb_matp_info = '0;
  assign tlb_satp_info_en = 1'b0;
  assign tlb_matp_info_en = 1'b0;
  assign tlb_invalidate = 1'b0;
  assign tlb_ptw_req_ready = 1'b0;
  assign tlb_ptw_resp_data = '0;
  assign tlb_ptw_resp_valid = 1'b0;

  for (genvar bank = 0; bank < DcacheLramNumBanks; bank++) begin : gen_data_array_zero
    assign data_array_read_req[bank] = '0;
    assign data_array_write_req[bank] = '0;
  end

  minion_dcache_atomic_alu_unit u_atomic (
    .clk_i,
    .rst_ni,
    .addr_i (atomic_addr),
    .cmd_i  (atomic_cmd),
    .typ_i  (atomic_typ),
    .lhs_i  (atomic_lhs),
    .rhs_i  (atomic_rhs),
    .out_o  (atomic_out)
  );

  minion_dcache_store_merge_unit u_store_merge (
    .addr_i      (merge_addr),
    .typ_i       (merge_typ),
    .ps_mask_i   (merge_ps_mask),
    .orig_data_i (merge_orig_data),
    .store_data_i(merge_store_data),
    .enable_lo_i (merge_enable_lo),
    .enable_hi_i (merge_enable_hi),
    .merge_out_o (merge_out)
  );

  minion_dcache_pma_unit u_pma (
    .ioshire_i    (pma_ioshire),
    .af_skip_i    (pma_af_skip),
    .req_data_i   (pma_req),
    .mprot_i      (pma_mprot),
    .valid_addr_i (pma_valid_addr),
    .vm_status_i  (pma_vm_status),
    .resp_data_o  (pma_resp)
  );

  minion_dcache_lru_array u_lru (
    .clk_i,
    .rst_ni,
    .read_set_i  (lru_read_set),
    .read_data_o (lru_read_data),
    .write_en_i  (lru_write_en),
    .write_set_i (lru_write_set),
    .write_data_i(lru_write_data)
  );

  minion_dcache_metadata_array u_metadata (
    .clk_i,
    .rst_ni,
    .s1_read_set_i         (meta_read_set),
    .s1_write_valid_i      (meta_write_valid),
    .s1_write_valid_prev_i (meta_write_valid_prev),
    .s1_write_req_i        (meta_write_req),
    .s1_read_resp_valid_o  (meta_read_resp_valid),
    .s1_read_resp_o        (meta_read_resp),
    .cfg_clear_all_i       (meta_clear_all),
    .cfg_clear_low_i       (meta_clear_low)
  );

  minion_dcache_buffer_array u_buffer (
    .clk_i,
    .rst_ni,
    .full_o                (buf_full),
    .reduce_full_o         (buf_reduce_full),
    .alloc_req_pre_i       (buf_alloc_req_pre),
    .alloc_req_val_i       (buf_alloc_req_val),
    .alloc_gsc_i           (buf_alloc_gsc),
    .reduce_alloc_req_i    (buf_reduce_alloc_req),
    .s2_write_needed_core_i(buf_s2_write_needed_core),
    .s2_write_needed_i     (buf_s2_write_needed),
    .s2_write_en_i         (buf_s2_write_en),
    .s2_write_replay_i     (buf_s2_write_replay),
    .s2_write_replay_entry_i(buf_s2_write_replay_entry),
    .s3_write_data_i       (buf_s3_write_data),
    .s2_write_entry_o      (buf_s2_write_entry),
    .s3_write_en_o         (buf_s3_write_en),
    .s3_write_en_ext_i     (buf_s3_write_en_ext),
    .s2_write_en_ext_prev_i(buf_s2_write_en_ext_prev),
    .s3_write_entry_ext_i  (buf_s3_write_entry_ext),
    .s3_write_data_ext_i   (buf_s3_write_data_ext),
    .s2_dealloc_i          (buf_s2_dealloc),
    .s2_dealloc_entry_i    (buf_s2_dealloc_entry),
    .s1_dealloc_uc_i       (buf_s1_dealloc_uc),
    .s1_dealloc_uc_entry_i (buf_s1_dealloc_uc_entry),
    .reduce_dealloc_req_i  (buf_reduce_dealloc_req),
    .reduce_dealloc_entry_i(buf_reduce_dealloc_entry),
    .s1_read_entry_i       (buf_s1_read_entry),
    .s1_read_data_o        (buf_s1_read_data)
  );

  minion_dcache_128x64_1r1w_lram u_lram64 (
    .clk_i              (clk_i),
    .rd_addr_i          (lram_rd_addr),
    .wr_addr_i          (lram_wr_addr),
    .rd_enable_i        (lram_rd_enable),
    .rd_enablelob_i     (lram_rd_enablelob),
    .rd_enablehib_i     (lram_rd_enablehib),
    .wr_enablehi_i      (lram_wr_enablehi),
    .wr_enablehi_pre_i  (lram_wr_enablehi_pre),
    .wr_enablelo_i      (lram_wr_enablelo),
    .wr_enablelo_pre_i  (lram_wr_enablelo_pre),
    .rd_data_o          (lram64_rd_data),
    .wr_data_i          (lram64_wr_data),
    .ram_cfg_i          (ram_cfg)
  );

  minion_dcache_128x72_1r1w_lram u_lram72 (
    .clk_i              (clk_i),
    .rd_addr_i          (lram_rd_addr),
    .wr_addr_i          (lram_wr_addr),
    .rd_enable_i        (lram_rd_enable),
    .rd_enablelob_i     (lram_rd_enablelob),
    .rd_enablehib_i     (lram_rd_enablehib),
    .wr_enablehi_i      (lram_wr_enablehi),
    .wr_enablehi_pre_i  (lram_wr_enablehi_pre),
    .wr_enablelo_i      (lram_wr_enablelo),
    .wr_enablelo_pre_i  (lram_wr_enablelo_pre),
    .rd_data_o          (lram72_rd_data),
    .wr_data_i          (lram72_wr_data),
    .ram_cfg_i          (ram_cfg)
  );

  prim_ram_2p #(
    .Width(8),
    .Depth(4)
  ) u_prim_ram_2p (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .a_req_i  (1'b0),
    .a_we_i   (1'b0),
    .a_addr_i ('0),
    .a_wdata_i('0),
    .a_rdata_o(prim_ram_2p_a_rdata),
    .b_req_i  (1'b0),
    .b_we_i   (1'b0),
    .b_addr_i ('0),
    .b_wdata_i('0),
    .b_rdata_o(prim_ram_2p_b_rdata),
    .cfg_i    (ram_cfg),
    .dft_i    (dft),
    .alert_o  (prim_ram_2p_alert)
  );

  minion_dcache_data_array u_data_array (
    .clk_i              (clk_i),
    .s1_read_valid_i    (data_array_read_valid),
    .s1_read_req_i      (data_array_read_req),
    .s2_read_resp_o     (data_array_read_resp),
    .s3_write_valid_i   (data_array_s3_write_valid),
    .s4_write_valid_i   (data_array_s4_write_valid),
    .s4_write_req_i     (data_array_write_req),
    .ram_cfg_i          (ram_cfg)
  );

  minion_dcache_writeback_unit u_writeback (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .req_ready_o        (wb_req_ready),
    .req_valid_i        (wb_req_valid),
    .req_data_i         (wb_req_data),
    .data_req_bid_o     (wb_data_req_bid),
    .data_req_grant_i   (wb_data_req_grant),
    .data_req_valid_o   (wb_data_req_valid),
    .data_req_o         (wb_data_req),
    .data_req_nack_i    (wb_data_req_nack),
    .data_resp_i        (wb_data_resp),
    .l2_req_ready_i     (wb_l2_req_ready),
    .l2_req_valid_o     (wb_l2_req_valid),
    .l2_req_data_o      (wb_l2_req_data)
  );

  minion_dcache_replay_queue u_replay_queue (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .full_o             (rq_full),
    .half_full_o        (rq_half_full),
    .empty_o            (rq_empty),
    .alloc_req_pre_i    (rq_alloc_req_pre),
    .alloc_req_val_i    (rq_alloc_req_val),
    .alloc_gsc_i        (rq_alloc_gsc),
    .push_req_i         (rq_push_req),
    .push_req_prev_i    (rq_push_req_prev),
    .req_data_i         (rq_req_data),
    .cacheable_i        (rq_cacheable),
    .req_buf_alloc_i    (rq_req_buf_alloc),
    .req_buf_entry_i    (rq_req_buf_entry),
    .req_conf_type_i    (rq_req_conf_type),
    .misaligned_1st_i   (rq_misaligned_1st),
    .uc_load_1st_i      (rq_uc_load_1st),
    .uc_load_ok_i       (rq_uc_load_ok),
    .miss_fill_ok_i     (rq_miss_fill_ok),
    .rq_conflict_o      (rq_conflict),
    .rq_conflict_to_co_o(rq_conflict_to_co),
    .rearm_i            (rq_rearm),
    .rearm_prev_i       (rq_rearm_prev),
    .rearm_entry_i      (rq_rearm_entry),
    .dealloc_i          (rq_dealloc),
    .dealloc_entry_i    (rq_dealloc_entry),
    .miss_available_i   (rq_miss_available),
    .miss_fill_error_i  (rq_miss_fill_error),
    .uc_load_fill_i     (rq_uc_load_fill),
    .uc_load_fill_id_i  (rq_uc_load_fill_id),
    .uc_load_fill_ok_i  (rq_uc_load_fill_ok),
    .vpu_rf_in_use_i    (rq_vpu_rf_in_use),
    .replay_bid_o       (rq_replay_bid),
    .replay_grant_i     (rq_replay_grant),
    .replay_valid_o     (rq_replay_valid),
    .replay_req_o       (rq_replay_req),
    .replay_entry_o     (rq_replay_entry),
    .replay_age_o       (rq_replay_age),
    .replay_uc_ok_o     (rq_replay_uc_ok),
    .replay_miss_ok_o   (rq_replay_miss_ok),
    .scoreboard_o       (rq_scoreboard),
    .fp_sb_dealloc_o    (rq_fp_sb_dealloc),
    .int_sb_dealloc_o   (rq_int_sb_dealloc)
  );

  minion_dcache_miss_handler u_miss_handler (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .neigh_min_id_i         (mh_neigh_min_id),
    .req_pri_val_i          (mh_req_pri_val),
    .req_pri_rdy_o          (mh_req_pri_rdy),
    .req_sec_val_i          (mh_req_sec_val),
    .req_sec_rdy_o          (mh_req_sec_rdy),
    .can_accept_uc_o        (mh_can_accept_uc),
    .req_info_i             (mh_req_info),
    .tag_match_o            (mh_tag_match),
    .evict_match_o          (mh_evict_match),
    .set_o                  (mh_set),
    .inflight_misses_i      (mh_inflight_misses),
    .inflight_reduce_i      (mh_inflight_reduce),
    .going_to_uc_store_o    (mh_going_to_uc_store),
    .idle_o                 (mh_idle),
    .wait_l2_to_ba_resp_o   (mh_wait_l2_to_ba_resp),
    .have_l2_to_ba_resp_o   (mh_have_l2_to_ba_resp),
    .l2_evict_ready_i       (mh_l2_evict_ready),
    .l2_evict_valid_o       (mh_l2_evict_valid),
    .l2_evict_req_o         (mh_l2_evict_req),
    .l2_miss_ready_i        (mh_l2_miss_ready),
    .l2_miss_valid_o        (mh_l2_miss_valid),
    .l2_miss_req_o          (mh_l2_miss_req),
    .l2_resp_valid_i        (mh_l2_resp_valid),
    .l2_resp_data_i         (mh_l2_resp_data),
    .l2_resp_slot_avail_i   (mh_l2_resp_slot_avail),
    .l2_resp_stall_o        (mh_l2_resp_stall),
    .refill_way_o           (mh_refill_way),
    .refill_addr_o          (mh_refill_addr),
    .force_ba_rptr_next_o   (mh_force_ba_rptr_next),
    .force_ba_rptr_qual_o   (mh_force_ba_rptr_qual),
    .ba_rptr_o              (mh_ba_rptr),
    .ba_is_amo_o            (mh_ba_is_amo),
    .meta_write_ready_i     (mh_meta_write_ready),
    .meta_write_valid_next_o(mh_meta_write_valid_next),
    .meta_write_valid_o     (mh_meta_write_valid),
    .meta_write_clear_slock_o(mh_meta_write_clear_slock),
    .meta_write_req_o       (mh_meta_write_req),
    .writeback_ready_i      (mh_writeback_ready),
    .writeback_valid_o      (mh_writeback_valid),
    .writeback_req_o        (mh_writeback_req),
    .writeback_done_i       (mh_writeback_done),
    .bus_err_o              (mh_bus_err),
    .bus_err_addr_o         (mh_bus_err_addr),
    .fill_bus_error_o       (mh_fill_bus_error),
    .uc_load_bus_error_o    (mh_uc_load_bus_error),
    .probe_rdy_o            (mh_probe_rdy),
    .fence_rdy_o            (mh_fence_rdy),
    .metaw_rdy_o            (mh_metaw_rdy),
    .ready_o                (mh_ready),
    .csr_debug_signals_o    (mh_csr_debug_signals),
    .sm_match_debug_signals_o(mh_sm_match_debug_signals),
    .sm_filter_debug_signals_o(mh_sm_filter_debug_signals),
    .sm_data_debug_signals_o(mh_sm_data_debug_signals)
  );

  minion_dcache_miss_handler_unit u_miss_handler_unit (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .neigh_min_id_i         (mh_neigh_min_id),
    .req_ready_o            (mhu_req_ready),
    .req_valid_i            (mhu_req_valid),
    .req_info_i             (mhu_req_info),
    .req_assigned_o         (mhu_req_assigned),
    .req_accepted_o         (mhu_req_accepted),
    .can_accept_uc_o        (mhu_can_accept_uc),
    .wait_l2_to_ba_resp_o   (mhu_wait_l2_to_ba_resp),
    .inflight_reduce_i      (mhu_inflight_reduce),
    .going_to_uc_store_o    (mhu_going_to_uc_store),
    .l2_evict_ready_i       (mhu_l2_evict_ready),
    .l2_evict_valid_o       (mhu_l2_evict_valid),
    .l2_evict_req_o         (mhu_l2_evict_req),
    .l2_miss_ready_i        (mhu_l2_miss_ready),
    .l2_miss_valid_o        (mhu_l2_miss_valid),
    .l2_miss_req_o          (mhu_l2_miss_req),
    .l2_resp_valid_i        (mhu_l2_resp_valid),
    .l2_resp_data_i         (mhu_l2_resp_data),
    .l2_resp_slot_avail_i   (mhu_l2_resp_slot_avail),
    .l2_resp_stall_o        (mhu_l2_resp_stall),
    .refill_way_o           (mhu_refill_way),
    .refill_addr_o          (mhu_refill_addr),
    .meta_write_ready_i     (mhu_meta_write_ready),
    .meta_write_valid_next_o(mhu_meta_write_valid_next),
    .meta_write_valid_o     (mhu_meta_write_valid),
    .meta_write_clear_slock_o(mhu_meta_write_clear_slock),
    .meta_write_req_o       (mhu_meta_write_req),
    .writeback_ready_i      (mhu_writeback_ready),
    .writeback_valid_o      (mhu_writeback_valid),
    .writeback_req_o        (mhu_writeback_req),
    .writeback_done_i       (mhu_writeback_done),
    .force_ba_rptr_o        (mhu_force_ba_rptr),
    .force_ba_rptr_qual_o   (mhu_force_ba_rptr_qual),
    .ba_rptr_o              (mhu_ba_rptr),
    .ba_is_amo_o            (mhu_ba_is_amo),
    .ba_wptr_o              (mhu_ba_wptr),
    .idle_o                 (mhu_idle),
    .probe_rdy_o            (mhu_probe_rdy),
    .metaw_rdy_o            (mhu_metaw_rdy),
    .fence_rdy_o            (mhu_fence_rdy),
    .handler_ready_o        (mhu_handler_ready),
    .handler_fill_bus_error_o(mhu_handler_fill_bus_error),
    .handler_uc_load_bus_error_o(mhu_handler_uc_load_bus_error),
    .match_miss_line_o      (mhu_match_miss_line),
    .bus_err_o              (mhu_bus_err),
    .bus_err_addr_o         (mhu_bus_err_addr),
    .csr_debug_signals_o    (mhu_csr_debug_signals),
    .sm_match_debug_signals_o(mhu_sm_match_debug_signals),
    .sm_filter_debug_signals_o(mhu_sm_filter_debug_signals),
    .sm_data_debug_signals_o(mhu_sm_data_debug_signals)
  );

  minion_dcache_scratchpad_ctrl u_scratchpad_ctrl (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .scratchpad_ctrl_i      (scp_ctrl_mode),
    .dcache_idle_i          (scp_dcache_idle),
    .dcache_cfg_stall_o     (scp_dcache_cfg_stall),
    .dcache_cfg_clear_all_o (scp_dcache_cfg_clear_all),
    .dcache_cfg_clear_low_o (scp_dcache_cfg_clear_low),
    .dcache_cfg_alloc_scp_o (scp_dcache_cfg_alloc_scp),
    .da_clear_en_o          (scp_da_clear_en),
    .da_clear_val_o         (scp_da_clear_val),
    .da_clear_set_o         (scp_da_clear_set),
    .da_clear_way_o         (scp_da_clear_way),
    .da_clear_idx_o         (scp_da_clear_idx),
    .da_clear_rdy_i         (scp_da_clear_rdy),
    .dcache_cfg_mode_o      (scp_dcache_cfg_mode)
  );

  minion_dcache_cache_op_unit u_cache_op_unit (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .chicken_bit_dcache_i   (co_chicken_bit_dcache),
    .busy_o                 (co_busy),
    .cache_op_ready_o       (co_cache_op_ready),
    .cache_op_valid_i       (co_cache_op_valid),
    .cache_op_req_i         (co_cache_op_req),
    .cache_op_req_next_i    (co_cache_op_req_next),
    .th_id_complete_o       (co_th_id_complete),
    .meta_read_bid_o        (co_meta_read_bid),
    .meta_read_grant_i      (co_meta_read_grant),
    .meta_read_valid_o      (co_meta_read_valid),
    .meta_read_phys_o       (co_meta_read_phys),
    .meta_read_force_set_o  (co_meta_read_force_set),
    .meta_read_thread_id_o  (co_meta_read_thread_id),
    .meta_read_skip_pma_o   (co_meta_read_skip_pma),
    .meta_read_addr_o       (co_meta_read_addr),
    .meta_read_addr_msb_err_o(co_meta_read_addr_msb_err),
    .meta_read_resp_way_en_i(meta_read_resp_valid),
    .meta_read_resp_i       (meta_read_resp),
    .s1_phys_addr_tag_i     ('0),
    .s1_tlb_wait_i          (1'b0),
    .s1_tlb_fail_i          (1'b0),
    .meta_write_ready_i     (1'b0),
    .meta_write_valid_next_o(co_meta_write_valid_next),
    .meta_write_valid_o     (co_meta_write_valid),
    .meta_write_req_o       (co_meta_write_req),
    .nack_set_valid_o       (co_nack_set_valid),
    .nack_set_value_o       (co_nack_set_value),
    .writeback_ready_i      (co_writeback_ready),
    .writeback_valid_o      (co_writeback_valid),
    .writeback_req_o        (co_writeback_req),
    .writeback_done_i       (co_writeback_done),
    .id_rq_full_i           (co_id_rq_full),
    .id_co_pf_bid_o         (co_id_co_pf_bid),
    .id_co_pf_grant_i       (co_id_co_pf_grant),
    .s0_co_pf_valid_o       (co_s0_co_pf_valid),
    .s0_co_pf_req_o         (co_s0_co_pf_req),
    .cache_op_ready_byp_i   (co_cache_op_ready_byp),
    .cache_op_valid_byp_o   (co_cache_op_valid_byp),
    .cache_op_req_byp_o     (co_cache_op_req_byp),
    .mh_clear_slock_valid_i (co_mh_clear_slock_valid),
    .mh_clear_slock_set_i   (co_mh_clear_slock_set),
    .mh_clear_slock_way_i   (co_mh_clear_slock_way),
    .hlock_state_o          (co_hlock_state),
    .slock_state_o          (co_slock_state),
    .da_clear_en_o          (co_da_clear_en),
    .da_clear_val_o         (co_da_clear_val),
    .da_clear_set_o         (co_da_clear_set),
    .da_clear_way_o         (co_da_clear_way),
    .da_clear_idx_o         (co_da_clear_idx),
    .da_clear_rdy_i         (co_da_clear_rdy),
    .mh_probe_rdy_i         (co_mh_probe_rdy),
    .mh_metaw_rdy_i         (co_mh_metaw_rdy),
    .rq_conflict_i          (co_rq_conflict),
    .cfg_clear_all_i        (co_cfg_clear_all),
    .cfg_clear_low_i        (co_cfg_clear_low),
    .cfg_alloc_scp_i        (co_cfg_alloc_scp),
    .cfg_cache_mode_i       (co_cfg_cache_mode),
    .vm_status_i            (co_vm_status),
    .vm_status_co_o         (co_vm_status_co),
    .err_flags_o            (co_err_flags),
    .csr_debug_signals_o    (co_csr_debug_signals),
    .sm_match_debug_signals_o(co_sm_match_debug_signals),
    .sm_filter_debug_signals_o(co_sm_filter_debug_signals),
    .sm_data_debug_signals_o(co_sm_data_debug_signals)
  );

  minion_dcache_cache_op_unit_l2 u_cache_op_unit_l2 (
    .clk_i                 (clk_i),
    .rst_ni                (rst_ni),
    .shire_id_i            (coul2_shire_id),
    .cacheop_max_i         (coul2_cacheop_max),
    .cacheop_reprate_i     (coul2_cacheop_reprate),
    .chicken_bit_dcache_i  (coul2_chicken_bit_dcache),
    .busy_o                (coul2_busy),
    .th_id_complete_o      (coul2_th_id_complete),
    .l2_resp_valid_i       (coul2_l2_resp_valid),
    .l2_resp_i             (coul2_l2_resp),
    .cache_op_ready_o      (coul2_cache_op_ready),
    .cache_op_valid_i      (coul2_cache_op_valid),
    .cache_op_req_i        (coul2_cache_op_req),
    .cache_op_req_next_i   (coul2_cache_op_req_next),
    .l2_req_ready_i        (coul2_l2_req_ready),
    .l2_req_valid_o        (coul2_l2_req_valid),
    .l2_req_data_o         (coul2_l2_req_data),
    .cache_op_ready_byp_o  (coul2_cache_op_ready_byp),
    .cache_op_valid_byp_i  (coul2_cache_op_valid_byp),
    .cache_op_req_byp_i    (coul2_cache_op_req_byp),
    .s0_available_i        (coul2_s0_available),
    .s0_addr_load_o        (coul2_s0_addr_load),
    .s0_addr_o             (coul2_s0_addr),
    .s0_addr_msb_err_o     (coul2_s0_addr_msb_err),
    .s0_thread_id_o        (coul2_s0_thread_id),
    .s1_available_i        (coul2_s1_available),
    .s1_addr_load_o        (coul2_s1_addr_load),
    .s1_tlb_wait_i         (coul2_s1_tlb_wait),
    .s1_tlb_fail_i         (coul2_s1_tlb_fail),
    .s1_tlb_addr_i         (coul2_s1_tlb_addr),
    .vm_status_i           (coul2_vm_status),
    .vm_status_co_i        (coul2_vm_status_co),
    .vm_status_l2_o        (coul2_vm_status_l2),
    .err_flags_o           (coul2_err_flags),
    .bus_err_o             (coul2_bus_err),
    .csr_debug_signals_o   (coul2_csr_debug_signals),
    .sm_match_debug_signals_o(coul2_sm_match_debug_signals),
    .sm_filter_debug_signals_o(coul2_sm_filter_debug_signals),
    .sm_data_debug_signals_o(coul2_sm_data_debug_signals)
  );

  minion_dcache_texsend u_texsend (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .neigh_min_id_i         (tex_neigh_min_id),
    .chicken_bit_dcache_i   (tex_chicken_bit_dcache),
    .idle_o                 (tex_idle),
    .tex_send_ready_o       (tex_send_ready),
    .tex_send_valid_i       (tex_send_valid),
    .tex_send_ctrl_i        (tex_send_ctrl),
    .l2_resp_valid_i        (tex_l2_resp_valid),
    .l2_resp_i              (tex_l2_resp),
    .l2_req_ready_i         (tex_l2_req_ready),
    .l2_req_valid_o         (tex_l2_req_valid),
    .l2_req_o               (tex_l2_req),
    .writeback_ready_i      (tex_writeback_ready),
    .writeback_valid_o      (tex_writeback_valid),
    .writeback_req_o        (tex_writeback_req),
    .writeback_done_i       (tex_writeback_done),
    .csr_debug_signals_o    (tex_csr_debug_signals)
  );

  minion_dcache_tensor_load u_tensor_load (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .shire_min_id_i         (tl_shire_min_id),
    .chicken_bit_dcache_i   (tl_chicken_bit_dcache),
    .busy_o                 (tl_busy),
    .id_complete_o          (tl_id_complete),
    .tensor_ctrl_i          (tl_tensor_ctrl),
    .s0_available_i         (tl_s0_available),
    .s0_addr_load_o         (tl_s0_addr_load),
    .s0_addr_o              (tl_s0_addr),
    .s0_addr_msb_err_o      (tl_s0_addr_msb_err),
    .s0_thread_id_o         (tl_s0_thread_id),
    .s1_available_i         (tl_s1_available),
    .s1_addr_load_o         (tl_s1_addr_load),
    .s1_tlb_wait_i          (tl_s1_tlb_wait),
    .s1_tlb_fail_i          (tl_s1_tlb_fail),
    .s2_addr_i              (tl_s2_addr),
    .l2_req_ready_coop_i    (tl_l2_req_ready_coop),
    .l2_req_valid_coop_o    (tl_l2_req_valid_coop),
    .l2_req_ready_no_coop_i (tl_l2_req_ready_no_coop),
    .l2_req_valid_no_coop_o (tl_l2_req_valid_no_coop),
    .l2_req_o               (tl_l2_req),
    .l2_resp_valid_i        (tl_l2_resp_valid),
    .l2_resp_i              (tl_l2_resp),
    .load_way_o             (tl_load_way),
    .load_addr_o            (tl_load_addr),
    .load_data_o            (tl_load_data),
    .load_valid_o           (tl_load_valid),
    .load_valid_pre_o       (tl_load_valid_pre),
    .load_ready_i           (tl_load_ready),
    .load_capture_o         (tl_load_capture),
    .tfma_enabled_i         (tl_tfma_enabled),
    .tenb_tstore_busy_i     (tl_tenb_tstore_busy),
    .tenb_credit_i          (tl_tenb_credit),
    .tenb_credit_entry_i    (tl_tenb_credit_entry),
    .tenb_written_i         (tl_tenb_written),
    .tenb_ready_o           (tl_tenb_ready),
    .tenb_flush_o           (tl_tenb_flush),
    .vm_status_i            (tl_vm_status),
    .vm_status_tl_o         (tl_vm_status_tl),
    .err_flags_o            (tl_err_flags),
    .tl_start_o             (tl_start),
    .tl_op_o                (tl_op),
    .bus_err_o              (tl_bus_err),
    .csr_debug_signals_o    (tl_csr_debug_signals),
    .sm_match_debug_signals_o(tl_sm_match_debug_signals),
    .sm_filter_debug_signals_o(tl_sm_filter_debug_signals),
    .sm_data_debug_signals_o(tl_sm_data_debug_signals)
  );

  /* verilator lint_off PINCONNECTEMPTY */  // Lint wrapper only checks that reduce integrates and parses cleanly.
  minion_dcache_reduce u_reduce (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .global_min_id_i        ('0),
    .chicken_bit_dcache_i   (1'b0),
    .reduce_ready_o         (),
    .reduce_start_i         (1'b0),
    .tensorstore_busy_o     (),
    .tensorstore_start_i    (1'b0),
    .tensorstore_stride_i   ('0),
    .reduce_tensorstore_ctrl_i('0),
    .thread_id_i            (1'b0),
    .l2_resp_valid_i        (1'b0),
    .l2_resp_i              ('0),
    .buffer_avail_o         (),
    .re_l2_req_ready_i      (1'b0),
    .re_l2_req_valid_o      (),
    .re_l2_req_o            (),
    .ts_l2_req_ready_i      (1'b0),
    .ts_l2_req_valid_o      (),
    .ts_l2_req_o            (),
    .re_ts_l2_req_inflight_o(),
    .pipe_used_o            (),
    .s1_inst_send_val_o     (),
    .s2_inst_send_val_o     (),
    .s3_inst_send_val_o     (),
    .s1_inst_op_val_o       (),
    .s2_inst_op_val_o       (),
    .s3_inst_op_val_o       (),
    .ts_scp_read_o          (),
    .ts_scp_read_way_o      (),
    .ts_scp_read_addr_o     (),
    .ts_col_size_is_128b_o  (),
    .s0_available_i         (1'b0),
    .s0_addr_load_o         (),
    .s0_addr_o              (),
    .s0_addr_msb_err_o      (),
    .s0_thread_id_o         (),
    .s1_available_i         (1'b0),
    .s1_addr_load_o         (),
    .s1_tlb_wait_i          (1'b0),
    .s1_tlb_fail_i          (1'b0),
    .s2_addr_i              ('0),
    .ba_full_i              (1'b0),
    .ba_alloc_o             (),
    .ba_write_needed_o      (),
    .s2_valid_i             (1'b0),
    .s2_ba_written_entry_i  ('0),
    .s3_ba_write_addr_o     (),
    .wait_l2_to_ba_resp_o   (),
    .ba_entries_required_o  (),
    .s1_valid_i             (1'b0),
    .ba_read_used_i         (1'b0),
    .s1_ba_data_i           ('0),
    .ba_read_en_o           (),
    .ba_read_en_spec_o      (),
    .ba_read_addr_o         (),
    .scp_ctrl_on_i          (1'b0),
    .vpu_reduce_wait_i      (1'b0),
    .vpu_ctrl_o             (),
    .vm_status_i            (co_vm_status),
    .vm_status_ts_o         (),
    .ts_start_o             (),
    .ts_op_o                (),
    .mh_going_to_uc_store_i (1'b0),
    .err_flags_o            (),
    .bus_err_o              (),
    .csr_debug_signals_o    (),
    .sm_match_debug_signals_o(),
    .sm_filter_debug_signals_o(),
    .sm_data_debug_signals_o()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  minion_dcache_tlb_array u_tlb_array (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .dft_i              (dft),
    .esr_vmspagesize_i  (tlb_vmspagesize),
    .req_data_i         (tlb_req_data),
    .req_valid_i        (tlb_req_valid),
    .resp_data_o        (tlb_resp_data),
    .satp_info_i        (tlb_satp_info),
    .matp_info_i        (tlb_matp_info),
    .satp_info_en_i     (tlb_satp_info_en),
    .matp_info_en_i     (tlb_matp_info_en),
    .tlb_invalidate_i   (tlb_invalidate),
    .ptw_req_data_o     (tlb_ptw_req_data),
    .ptw_req_valid_o    (tlb_ptw_req_valid),
    .ptw_req_ready_i    (tlb_ptw_req_ready),
    .ptw_resp_data_i    (tlb_ptw_resp_data),
    .ptw_resp_valid_i   (tlb_ptw_resp_valid),
    .vm_enabled_o       (tlb_vm_enabled)
  );

  /* verilator lint_off PINCONNECTEMPTY */  // Lint wrapper only checks that the translated top parses and wires cleanly.
  minion_dcache_top u_top (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .shire_id_i               ('0),
    .shire_min_id_i           ('0),
    .ioshire_i                (1'b0),
    .chicken_bit_dcache_i     (1'b0),
    .core_ctrl_i              (core_dcache_ctrl'('0)),
    .core_ctrl_resp_o         (),
    .id_core_alloc_rq_pre_i   (1'b0),
    .s0_core_alloc_rq_val_i   (1'b0),
    .id_core_gsc_i            (1'b0),
    .id_core_ready_o          (),
    .s0_core_req_valid_i      (1'b0),
    .s0_core_req_i            (minion_dcache_req'('0)),
    .s0_core_gsc_i            (1'b0),
    .s1_core_kill_i           (1'b0),
    .s1_core_store_data_i     ('0),
    .s1_mprot_i               (esr_mprot_t'('0)),
    .s2_core_kill_i           (1'b0),
    .s3_core_x31_i            ('0),
    .s1_vpu_ctrl_i            (vpu_dcache_ctrl'('0)),
    .s3_vpu_scp_resp_o        (),
    .s3_vpu_scp_data_o        (),
    .s3_vpu_tenb_data_o       (),
    .vpu_reduce_ctrl_o        (),
    .s2_core_resp_int_valid_o (),
    .s3_core_resp_valid_o     (),
    .s3_core_resp_o           (),
    .id_core_scoreboard_o     (),
    .id_core_sb_fp_dealloc_o  (),
    .id_core_sb_int_dealloc_o (),
    .s1_core_replay_next_o    (),
    .s1_core_xcpt_o           (),
    .s2_core_flush_pipeline_o (),
    .s3_ordered_o             (),
    .s3_invalidate_lr_i       (1'b0),
    .wb_dcache_fp_toint_i     (1'b0),
    .vmspagesize_i            (tlb_entry_type'(2'b00)),
    .bypass_dcache_i          (1'b0),
    .mem_ctrl_override_i      ('0),
    .satp_info_i              (minion_satp_info'('0)),
    .matp_info_i              (minion_satp_info'('0)),
    .satp_info_en_i           (1'b0),
    .matp_info_en_i           (1'b0),
    .vm_status_i              (co_vm_status),
    .tlb_invalidate_i         (1'b0),
    .ptw_req_data_o           (),
    .ptw_req_valid_o          (),
    .ptw_req_ready_i          (1'b0),
    .ptw_resp_data_i          (minion_ptw_pte'('0)),
    .ptw_resp_valid_i         (1'b0),
    .l2_evict_req_ready_i     ('0),
    .l2_evict_req_valid_o     (),
    .l2_evict_req_o           (),
    .l2_miss_req_ready_i      ('0),
    .l2_miss_req_valid_o      (),
    .l2_miss_req_o            (),
    .l2_resp_ready_o          (),
    .l2_resp_valid_i          (1'b0),
    .l2_resp_i                (et_link_minion_rsp_info_t'('0)),
    .l2_resp_ready_int_o      (),
    .l2_resp_valid_int_o      (),
    .l2_resp_int_o            (),
    .s1_bp_conf_o             (),
    .s1_bp_conf_valid_o       (),
    .dcache_idle_excl_mode_o  (),
    .tensor_load_err_flags_o  (),
    .cache_ops_err_flags_o    (),
    .tensor_reduce_err_flags_o(),
    .apb_paddr_i              ('0),
    .apb_pwrite_i             (1'b0),
    .apb_psel_i               (1'b0),
    .apb_penable_i            (1'b0),
    .apb_pwdata_i             ('0),
    .apb_pready_o             (),
    .apb_prdata_o             (),
    .apb_pslverr_o            (),
    .io_events_o              (),
    .bus_err_o                (),
    .bus_err_addr_o           (),
    .csr_debug_sigs_o         (),
    .sm_match_debug_signals_tl0_o(),
    .sm_filter_debug_signals_tl0_o(),
    .sm_data_debug_signals_tl0_o(),
    .sm_match_debug_signals_tl1_o(),
    .sm_filter_debug_signals_tl1_o(),
    .sm_data_debug_signals_tl1_o(),
    .sm_match_debug_signals_ts_o(),
    .sm_filter_debug_signals_ts_o(),
    .sm_data_debug_signals_ts_o(),
    .sm_match_debug_signals_co_o(),
    .sm_filter_debug_signals_co_o(),
    .sm_data_debug_signals_co_o(),
    .sm_match_debug_signals_col2_o(),
    .sm_filter_debug_signals_col2_o(),
    .sm_data_debug_signals_col2_o(),
    .sm_match_debug_signals_mh_o(),
    .sm_filter_debug_signals_mh_o(),
    .sm_data_debug_signals_mh_o(),
    .sm_match_debug_signals_dc_o(),
    .sm_filter_debug_signals_dc_o(),
    .sm_data_debug_signals_dc_o()
  );
  /* verilator lint_on PINCONNECTEMPTY */

endmodule : minion_dcache_lint_top
