// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_cache_op_unit vs new
// minion_dcache_cache_op_unit.

`include "soc.vh"

module cosim_minion_dcache_cache_op_unit_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                clk_i,
  input  logic                                rst_ni,
  input  logic                                chicken_bit_dcache_i,
  input  logic                                cache_op_valid_i,
  input  logic [3:0]                          cache_op_cmd_i,
  input  logic [1:0]                          cache_op_start_level_i,
  input  logic [1:0]                          cache_op_dest_level_i,
  input  logic [3:0]                          cache_op_count_i,
  input  logic [VaSize-1:0]                   cache_op_addr_i,
  input  logic [DcacheWayIdxWidth-1:0]        cache_op_way_i,
  input  logic [VaSize-1:0]                   cache_op_stride_i,
  input  logic                                cache_op_thread_id_i,
  input  logic                                cache_op_use_tmask_i,
  input  logic                                cache_op_tmask_ready_i,
  input  logic [TensorMaskBits-1:0]           cache_op_tmask_bits_i,
  input  logic [DcacheL2ScpDstBits-1:0]       cache_op_l2_scp_dest_i,
  input  logic                                cache_op_l2_scp_fill_i,
  input  logic                                meta_read_grant_i,
  input  logic [DcacheWays-1:0]               meta_read_resp_way_en_i,
  input  logic [PaTagExtSize-1:0]             meta_read_resp_tag0_i,
  input  logic [PaTagExtSize-1:0]             meta_read_resp_tag1_i,
  input  logic [PaTagExtSize-1:0]             meta_read_resp_tag2_i,
  input  logic [PaTagExtSize-1:0]             meta_read_resp_tag3_i,
  input  logic [1:0]                          meta_read_resp_state0_i,
  input  logic [1:0]                          meta_read_resp_state1_i,
  input  logic [1:0]                          meta_read_resp_state2_i,
  input  logic [1:0]                          meta_read_resp_state3_i,
  input  logic [PaTagExtSize-1:0]             s1_phys_addr_tag_i,
  input  logic                                s1_tlb_wait_i,
  input  logic                                s1_tlb_fail_i,
  input  logic                                meta_write_ready_i,
  input  logic                                writeback_ready_i,
  input  logic                                writeback_done_i,
  input  logic                                id_rq_full_i,
  input  logic                                id_co_pf_grant_i,
  input  logic                                cache_op_ready_byp_i,
  input  logic                                mh_clear_slock_valid_i,
  input  logic [DcacheSetIdxWidth-1:0]        mh_clear_slock_set_i,
  input  logic [DcacheWayIdxWidth-1:0]        mh_clear_slock_way_i,
  input  logic                                da_clear_rdy_i,
  input  logic [DcacheMhFileSize-1:0]         mh_probe_rdy_i,
  input  logic [DcacheMhFileSize-1:0]         mh_metaw_rdy_i,
  input  logic                                rq_conflict_i,
  input  logic                                cfg_clear_all_i,
  input  logic                                cfg_clear_low_i,
  input  logic                                cfg_alloc_scp_i,
  input  logic [1:0]                          cfg_cache_mode_i,
  input  logic [NrThreads-1:0]                vm_status_debug_i,
  input  logic [NrThreads-1:0]                vm_status_sum_i,
  input  logic [NrThreads-1:0]                vm_status_mxr_i,
  input  logic [NrThreads-1:0][1:0]           vm_status_prv_i,
  input  logic [NrThreads-1:0][1:0]           vm_status_mpp_i,
  input  logic [NrThreads-1:0]                vm_status_mprv_i,
  output logic                                orig_busy_o,
  output logic                                new_busy_o,
  output logic                                orig_cache_op_ready_o,
  output logic                                new_cache_op_ready_o,
  output logic [NrThreads-1:0][1:0]           orig_th_id_complete_o,
  output logic [NrThreads-1:0][1:0]           new_th_id_complete_o,
  output logic                                orig_meta_read_bid_o,
  output logic                                new_meta_read_bid_o,
  output logic                                orig_meta_read_valid_o,
  output logic                                new_meta_read_valid_o,
  output logic                                orig_meta_read_phys_o,
  output logic                                new_meta_read_phys_o,
  output logic                                orig_meta_read_force_set_o,
  output logic                                new_meta_read_force_set_o,
  output logic                                orig_meta_read_thread_id_o,
  output logic                                new_meta_read_thread_id_o,
  output logic                                orig_meta_read_skip_pma_o,
  output logic                                new_meta_read_skip_pma_o,
  output logic [VaSize-1:0]                   orig_meta_read_addr_o,
  output logic [VaSize-1:0]                   new_meta_read_addr_o,
  output logic                                orig_meta_read_addr_msb_err_o,
  output logic                                new_meta_read_addr_msb_err_o,
  output logic                                orig_meta_write_valid_next_o,
  output logic                                new_meta_write_valid_next_o,
  output logic                                orig_meta_write_valid_o,
  output logic                                new_meta_write_valid_o,
  output logic [DcacheSetIdxWidth-1:0]        orig_meta_write_set_o,
  output logic [DcacheSetIdxWidth-1:0]        new_meta_write_set_o,
  output logic [DcacheWays-1:0]               orig_meta_write_way_en_o,
  output logic [DcacheWays-1:0]               new_meta_write_way_en_o,
  output logic [PaTagExtSize-1:0]             orig_meta_write_tag_o,
  output logic [PaTagExtSize-1:0]             new_meta_write_tag_o,
  output logic [1:0]                          orig_meta_write_coh_state_o,
  output logic [1:0]                          new_meta_write_coh_state_o,
  output logic                                orig_nack_set_valid_o,
  output logic                                new_nack_set_valid_o,
  output logic [DcacheSetIdxWidth-1:0]        orig_nack_set_value_o,
  output logic [DcacheSetIdxWidth-1:0]        new_nack_set_value_o,
  output logic                                orig_writeback_valid_o,
  output logic                                new_writeback_valid_o,
  output logic [PaTagExtSize-1:0]             orig_writeback_tag_o,
  output logic [PaTagExtSize-1:0]             new_writeback_tag_o,
  output logic [DcacheSetIdxWidth-1:0]        orig_writeback_set_o,
  output logic [DcacheSetIdxWidth-1:0]        new_writeback_set_o,
  output logic [DcacheWays-1:0]               orig_writeback_way_en_o,
  output logic [DcacheWays-1:0]               new_writeback_way_en_o,
  output logic                                orig_id_co_pf_bid_o,
  output logic                                new_id_co_pf_bid_o,
  output logic                                orig_s0_co_pf_valid_o,
  output logic                                new_s0_co_pf_valid_o,
  output logic [VaSize:0]                     orig_s0_co_pf_addr_o,
  output logic [VaSize:0]                     new_s0_co_pf_addr_o,
  output logic                                orig_s0_co_pf_addr_msb_err_o,
  output logic                                new_s0_co_pf_addr_msb_err_o,
  output logic [4:0]                          orig_s0_co_pf_cmd_o,
  output logic [4:0]                          new_s0_co_pf_cmd_o,
  output logic [3:0]                          orig_s0_co_pf_typ_o,
  output logic [3:0]                          new_s0_co_pf_typ_o,
  output logic                                orig_s0_co_pf_cacheable_o,
  output logic                                new_s0_co_pf_cacheable_o,
  output logic                                orig_s0_co_pf_thread_id_o,
  output logic                                new_s0_co_pf_thread_id_o,
  output logic                                orig_s0_co_pf_cache_op_o,
  output logic                                new_s0_co_pf_cache_op_o,
  output logic [DcacheVmSelWidth-1:0]         orig_s0_co_pf_vm_sel_o,
  output logic [DcacheVmSelWidth-1:0]         new_s0_co_pf_vm_sel_o,
  output logic                                orig_cache_op_valid_byp_o,
  output logic                                new_cache_op_valid_byp_o,
  output logic [3:0]                          orig_cache_op_byp_cmd_o,
  output logic [3:0]                          new_cache_op_byp_cmd_o,
  output logic [1:0]                          orig_cache_op_byp_start_level_o,
  output logic [1:0]                          new_cache_op_byp_start_level_o,
  output logic [1:0]                          orig_cache_op_byp_dest_level_o,
  output logic [1:0]                          new_cache_op_byp_dest_level_o,
  output logic [3:0]                          orig_cache_op_byp_count_o,
  output logic [3:0]                          new_cache_op_byp_count_o,
  output logic [VaSize-1:0]                   orig_cache_op_byp_addr_o,
  output logic [VaSize-1:0]                   new_cache_op_byp_addr_o,
  output logic [DcacheWayIdxWidth-1:0]        orig_cache_op_byp_way_o,
  output logic [DcacheWayIdxWidth-1:0]        new_cache_op_byp_way_o,
  output logic [VaSize-1:0]                   orig_cache_op_byp_stride_o,
  output logic [VaSize-1:0]                   new_cache_op_byp_stride_o,
  output logic                                orig_cache_op_byp_thread_id_o,
  output logic                                new_cache_op_byp_thread_id_o,
  output logic                                orig_cache_op_byp_use_tmask_o,
  output logic                                new_cache_op_byp_use_tmask_o,
  output logic                                orig_cache_op_byp_tmask_ready_o,
  output logic                                new_cache_op_byp_tmask_ready_o,
  output logic [TensorMaskBits-1:0]           orig_cache_op_byp_tmask_bits_o,
  output logic [TensorMaskBits-1:0]           new_cache_op_byp_tmask_bits_o,
  output logic [DcacheL2ScpDstBits-1:0]       orig_cache_op_byp_l2_scp_dest_o,
  output logic [DcacheL2ScpDstBits-1:0]       new_cache_op_byp_l2_scp_dest_o,
  output logic                                orig_cache_op_byp_l2_scp_fill_o,
  output logic                                new_cache_op_byp_l2_scp_fill_o,
  output logic                                orig_da_clear_en_o,
  output logic                                new_da_clear_en_o,
  output logic                                orig_da_clear_val_o,
  output logic                                new_da_clear_val_o,
  output logic [DcacheSetIdxWidth-1:0]        orig_da_clear_set_o,
  output logic [DcacheSetIdxWidth-1:0]        new_da_clear_set_o,
  output logic [DcacheWayIdxWidth-1:0]        orig_da_clear_way_o,
  output logic [DcacheWayIdxWidth-1:0]        new_da_clear_way_o,
  output logic                                orig_da_clear_idx_o,
  output logic                                new_da_clear_idx_o,
  output logic [1:0]                          orig_vm_status_co_prv_o,
  output logic [1:0]                          new_vm_status_co_prv_o,
  output logic [1:0]                          orig_vm_status_co_mpp_o,
  output logic [1:0]                          new_vm_status_co_mpp_o,
  output logic                                orig_vm_status_co_mprv_o,
  output logic                                new_vm_status_co_mprv_o,
  output logic                                orig_vm_status_co_sum_o,
  output logic                                new_vm_status_co_sum_o,
  output logic                                orig_vm_status_co_mxr_o,
  output logic                                new_vm_status_co_mxr_o,
  output logic                                orig_vm_status_co_debug_o,
  output logic                                new_vm_status_co_debug_o,
  output logic [DcacheErrFlagRange:0][NrThreads-1:0] orig_err_flags_o,
  output logic [DcacheErrFlagRange:0][NrThreads-1:0] new_err_flags_o,
  output logic [DcacheDbgCsrTxsndSize-1:0]    orig_csr_debug_signals_o,
  output logic [DcacheDbgCsrTxsndSize-1:0]    new_csr_debug_signals_o,
  output logic [DcacheDbgSmMatchBits-1:0]     orig_sm_match_debug_signals_o,
  output logic [DcacheDbgSmMatchBits-1:0]     new_sm_match_debug_signals_o,
  output logic [63:0]                         orig_sm_filter_chunks_o [4],
  output logic [63:0]                         new_sm_filter_chunks_o [4],
  output logic [63:0]                         orig_sm_data_chunks_o [8],
  output logic [63:0]                         new_sm_data_chunks_o [8],
  output logic [63:0]                         orig_hlock_chunks_o [8],
  output logic [63:0]                         new_hlock_chunks_o [8],
  output logic [63:0]                         orig_slock_chunks_o [8],
  output logic [63:0]                         new_slock_chunks_o [8]
);
  localparam int unsigned LockBits = DcacheSets * DcacheWays;
  localparam int unsigned LockChunks = (LockBits + 63) / 64;

  core_dcache_op_req_t      req_new;
  core_dcache_op_req_next_t req_next_new;
  dcache_meta_data_t        meta_read_resp_new [DcacheWays-1:0];
  vm_status_t [NrThreads-1:0] vm_status_new;
  dcache_meta_write_req_t   meta_write_req_new;
  dcache_wb_req_t           writeback_req_new;
  dcache_req_t              s0_co_pf_req_new;
  core_dcache_op_req_t      cache_op_req_byp_new;
  vm_status_t               vm_status_co_new;
  logic [DcacheSets-1:0][DcacheWays-1:0] hlock_state_new;
  logic [DcacheSets-1:0][DcacheWays-1:0] slock_state_new;

  core_dcache_op_req        req_orig;
  core_dcache_op_req_next   req_next_orig;
  dcache_meta_data [`DCACHE_WAYS-1:0] meta_read_resp_orig;
  minion_vm_status [`CORE_NR_THREADS-1:0] vm_status_orig;
  dcache_meta_write_req     meta_write_req_orig;
  dcache_wb_req             writeback_req_orig;
  dcache_req                s0_co_pf_req_orig;
  core_dcache_op_req        cache_op_req_byp_orig;
  minion_vm_status          vm_status_co_orig;
  logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] hlock_state_orig;
  logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] slock_state_orig;

  logic [LockBits-1:0] hlock_flat_orig, hlock_flat_new;
  logic [LockBits-1:0] slock_flat_orig, slock_flat_new;
  logic [DcacheDbgSmFilterBits-1:0] orig_sm_filter_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] new_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] orig_sm_data_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] new_sm_data_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] sm_filter_flat_orig, sm_filter_flat_new;
  logic [DcacheDbgSmDataBits-1:0] sm_data_flat_orig, sm_data_flat_new;

  always_comb begin
    req_new = '0;
    req_new.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    req_new.start_level = cache_op_level_e'(cache_op_start_level_i);
    req_new.dest_level = cache_op_level_e'(cache_op_dest_level_i);
    req_new.count = cache_op_count_i;
    req_new.addr = cache_op_addr_i;
    req_new.way = cache_op_way_i;
    req_new.stride = cache_op_stride_i;
    req_new.thread_id = cache_op_thread_id_i;
    req_new.use_tmask = cache_op_use_tmask_i;
    req_new.tmask_ready = cache_op_tmask_ready_i;
    req_new.tmask_bits = cache_op_tmask_bits_i;
    req_new.l2_scp_dest = cache_op_l2_scp_dest_i;
    req_new.l2_scp_fill = cache_op_l2_scp_fill_i;

    req_next_new = '0;
    req_next_new.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    req_next_new.start_level = cache_op_level_e'(cache_op_start_level_i);
    req_next_new.dest_level = cache_op_level_e'(cache_op_dest_level_i);

    req_orig = '0;
    req_orig.cmd = CacheOpControl_t'(cache_op_cmd_i);
    req_orig.start_level = CacheOpLevel_t'(cache_op_start_level_i);
    req_orig.dest_level = CacheOpLevel_t'(cache_op_dest_level_i);
    req_orig.count = cache_op_count_i;
    req_orig.addr = cache_op_addr_i;
    req_orig.way = cache_op_way_i;
    req_orig.stride = cache_op_stride_i;
    req_orig.thread_id = cache_op_thread_id_i;
    req_orig.use_tmask = cache_op_use_tmask_i;
    req_orig.tmask_ready = cache_op_tmask_ready_i;
    req_orig.tmask_bits = cache_op_tmask_bits_i;
    req_orig.l2_scp_dest = cache_op_l2_scp_dest_i;
    req_orig.l2_scp_fill = cache_op_l2_scp_fill_i;

    req_next_orig = '0;
    req_next_orig.cmd = CacheOpControl_t'(cache_op_cmd_i);
    req_next_orig.start_level = CacheOpLevel_t'(cache_op_start_level_i);
    req_next_orig.dest_level = CacheOpLevel_t'(cache_op_dest_level_i);

    meta_read_resp_new[0] = '{coh_state: dcache_line_state_e'(meta_read_resp_state0_i), tag: meta_read_resp_tag0_i};
    meta_read_resp_new[1] = '{coh_state: dcache_line_state_e'(meta_read_resp_state1_i), tag: meta_read_resp_tag1_i};
    meta_read_resp_new[2] = '{coh_state: dcache_line_state_e'(meta_read_resp_state2_i), tag: meta_read_resp_tag2_i};
    meta_read_resp_new[3] = '{coh_state: dcache_line_state_e'(meta_read_resp_state3_i), tag: meta_read_resp_tag3_i};

    meta_read_resp_orig[0] = '{coh_state: dcache_line_state'(meta_read_resp_state0_i), tag: meta_read_resp_tag0_i};
    meta_read_resp_orig[1] = '{coh_state: dcache_line_state'(meta_read_resp_state1_i), tag: meta_read_resp_tag1_i};
    meta_read_resp_orig[2] = '{coh_state: dcache_line_state'(meta_read_resp_state2_i), tag: meta_read_resp_tag2_i};
    meta_read_resp_orig[3] = '{coh_state: dcache_line_state'(meta_read_resp_state3_i), tag: meta_read_resp_tag3_i};

    for (int unsigned i = 0; i < NrThreads; i++) begin
      vm_status_new[i] = '0;
      vm_status_new[i].debug = vm_status_debug_i[i];
      vm_status_new[i].sum = vm_status_sum_i[i];
      vm_status_new[i].mxr = vm_status_mxr_i[i];
      vm_status_new[i].prv = vm_status_prv_i[i];
      vm_status_new[i].mpp = vm_status_mpp_i[i];
      vm_status_new[i].mprv = vm_status_mprv_i[i];

      vm_status_orig[i] = '0;
      vm_status_orig[i].debug = vm_status_debug_i[i];
      vm_status_orig[i].sum = vm_status_sum_i[i];
      vm_status_orig[i].mxr = vm_status_mxr_i[i];
      vm_status_orig[i].prv = vm_status_prv_i[i];
      vm_status_orig[i].mpp = vm_status_mpp_i[i];
      vm_status_orig[i].mprv = vm_status_mprv_i[i];
    end
  end

  assign orig_meta_write_set_o = meta_write_req_orig.set;
  assign new_meta_write_set_o = meta_write_req_new.set_idx;
  assign orig_meta_write_way_en_o = meta_write_req_orig.way_en;
  assign new_meta_write_way_en_o = meta_write_req_new.way_en;
  assign orig_meta_write_tag_o = meta_write_req_orig.data.tag;
  assign new_meta_write_tag_o = meta_write_req_new.data.tag;
  assign orig_meta_write_coh_state_o = meta_write_req_orig.data.coh_state;
  assign new_meta_write_coh_state_o = meta_write_req_new.data.coh_state;

  assign orig_writeback_tag_o = writeback_req_orig.tag;
  assign new_writeback_tag_o = writeback_req_new.tag;
  assign orig_writeback_set_o = writeback_req_orig.set;
  assign new_writeback_set_o = writeback_req_new.set;
  assign orig_writeback_way_en_o = writeback_req_orig.way_en;
  assign new_writeback_way_en_o = writeback_req_new.way_en;

  assign orig_s0_co_pf_addr_o = s0_co_pf_req_orig.addr;
  assign new_s0_co_pf_addr_o = s0_co_pf_req_new.addr;
  assign orig_s0_co_pf_addr_msb_err_o = s0_co_pf_req_orig.addr_msb_err;
  assign new_s0_co_pf_addr_msb_err_o = s0_co_pf_req_new.addr_msb_err;
  assign orig_s0_co_pf_cmd_o = s0_co_pf_req_orig.cmd;
  assign new_s0_co_pf_cmd_o = s0_co_pf_req_new.cmd;
  assign orig_s0_co_pf_typ_o = s0_co_pf_req_orig.typ;
  assign new_s0_co_pf_typ_o = s0_co_pf_req_new.typ;
  assign orig_s0_co_pf_cacheable_o = s0_co_pf_req_orig.cacheable;
  assign new_s0_co_pf_cacheable_o = s0_co_pf_req_new.cacheable;
  assign orig_s0_co_pf_thread_id_o = s0_co_pf_req_orig.dest.thread_id;
  assign new_s0_co_pf_thread_id_o = s0_co_pf_req_new.dest.thread_id;
  assign orig_s0_co_pf_cache_op_o = s0_co_pf_req_orig.cache_op;
  assign new_s0_co_pf_cache_op_o = s0_co_pf_req_new.cache_op;
  assign orig_s0_co_pf_vm_sel_o = s0_co_pf_req_orig.vm_sel;
  assign new_s0_co_pf_vm_sel_o = s0_co_pf_req_new.vm_sel;

  assign orig_cache_op_byp_cmd_o = cache_op_req_byp_orig.cmd;
  assign new_cache_op_byp_cmd_o = cache_op_req_byp_new.cmd;
  assign orig_cache_op_byp_start_level_o = cache_op_req_byp_orig.start_level;
  assign new_cache_op_byp_start_level_o = cache_op_req_byp_new.start_level;
  assign orig_cache_op_byp_dest_level_o = cache_op_req_byp_orig.dest_level;
  assign new_cache_op_byp_dest_level_o = cache_op_req_byp_new.dest_level;
  assign orig_cache_op_byp_count_o = cache_op_req_byp_orig.count;
  assign new_cache_op_byp_count_o = cache_op_req_byp_new.count;
  assign orig_cache_op_byp_addr_o = cache_op_req_byp_orig.addr;
  assign new_cache_op_byp_addr_o = cache_op_req_byp_new.addr;
  assign orig_cache_op_byp_way_o = cache_op_req_byp_orig.way;
  assign new_cache_op_byp_way_o = cache_op_req_byp_new.way;
  assign orig_cache_op_byp_stride_o = cache_op_req_byp_orig.stride;
  assign new_cache_op_byp_stride_o = cache_op_req_byp_new.stride;
  assign orig_cache_op_byp_thread_id_o = cache_op_req_byp_orig.thread_id;
  assign new_cache_op_byp_thread_id_o = cache_op_req_byp_new.thread_id;
  assign orig_cache_op_byp_use_tmask_o = cache_op_req_byp_orig.use_tmask;
  assign new_cache_op_byp_use_tmask_o = cache_op_req_byp_new.use_tmask;
  assign orig_cache_op_byp_tmask_ready_o = cache_op_req_byp_orig.tmask_ready;
  assign new_cache_op_byp_tmask_ready_o = cache_op_req_byp_new.tmask_ready;
  assign orig_cache_op_byp_tmask_bits_o = cache_op_req_byp_orig.tmask_bits;
  assign new_cache_op_byp_tmask_bits_o = cache_op_req_byp_new.tmask_bits;
  assign orig_cache_op_byp_l2_scp_dest_o = cache_op_req_byp_orig.l2_scp_dest;
  assign new_cache_op_byp_l2_scp_dest_o = cache_op_req_byp_new.l2_scp_dest;
  assign orig_cache_op_byp_l2_scp_fill_o = cache_op_req_byp_orig.l2_scp_fill;
  assign new_cache_op_byp_l2_scp_fill_o = cache_op_req_byp_new.l2_scp_fill;

  assign orig_vm_status_co_prv_o = vm_status_co_orig.prv;
  assign new_vm_status_co_prv_o = vm_status_co_new.prv;
  assign orig_vm_status_co_mpp_o = vm_status_co_orig.mpp;
  assign new_vm_status_co_mpp_o = vm_status_co_new.mpp;
  assign orig_vm_status_co_mprv_o = vm_status_co_orig.mprv;
  assign new_vm_status_co_mprv_o = vm_status_co_new.mprv;
  assign orig_vm_status_co_sum_o = vm_status_co_orig.sum;
  assign new_vm_status_co_sum_o = vm_status_co_new.sum;
  assign orig_vm_status_co_mxr_o = vm_status_co_orig.mxr;
  assign new_vm_status_co_mxr_o = vm_status_co_new.mxr;
  assign orig_vm_status_co_debug_o = vm_status_co_orig.debug;
  assign new_vm_status_co_debug_o = vm_status_co_new.debug;

  assign hlock_flat_orig = hlock_state_orig;
  assign hlock_flat_new = hlock_state_new;
  assign slock_flat_orig = slock_state_orig;
  assign slock_flat_new = slock_state_new;
  assign sm_filter_flat_orig = orig_sm_filter_debug_signals;
  assign sm_filter_flat_new = new_sm_filter_debug_signals;
  assign sm_data_flat_orig = orig_sm_data_debug_signals;
  assign sm_data_flat_new = new_sm_data_debug_signals;

  always_comb begin
    for (int unsigned chunk = 0; chunk < 8; chunk++) begin
      orig_hlock_chunks_o[chunk] = '0;
      new_hlock_chunks_o[chunk] = '0;
      orig_slock_chunks_o[chunk] = '0;
      new_slock_chunks_o[chunk] = '0;
      orig_sm_data_chunks_o[chunk] = '0;
      new_sm_data_chunks_o[chunk] = '0;
      if (chunk < LockChunks) begin
        for (int unsigned b = 0; b < 64; b++) begin
          if ((chunk * 64 + b) < LockBits) begin
            orig_hlock_chunks_o[chunk][b] = hlock_flat_orig[chunk*64+b];
            new_hlock_chunks_o[chunk][b] = hlock_flat_new[chunk*64+b];
            orig_slock_chunks_o[chunk][b] = slock_flat_orig[chunk*64+b];
            new_slock_chunks_o[chunk][b] = slock_flat_new[chunk*64+b];
          end
          if ((chunk * 64 + b) < DcacheDbgSmDataBits) begin
            orig_sm_data_chunks_o[chunk][b] = sm_data_flat_orig[chunk*64+b];
            new_sm_data_chunks_o[chunk][b] = sm_data_flat_new[chunk*64+b];
          end
        end
      end
    end
    for (int unsigned chunk = 0; chunk < 4; chunk++) begin
      orig_sm_filter_chunks_o[chunk] = '0;
      new_sm_filter_chunks_o[chunk] = '0;
      for (int unsigned b = 0; b < 64; b++) begin
        if ((chunk * 64 + b) < DcacheDbgSmFilterBits) begin
          orig_sm_filter_chunks_o[chunk][b] = sm_filter_flat_orig[chunk*64+b];
          new_sm_filter_chunks_o[chunk][b] = sm_filter_flat_new[chunk*64+b];
        end
      end
    end
  end

  dcache_cache_op_unit u_orig (
    .clock                 (clk_i),
    .reset                 (~rst_ni),
    .chicken_bit_dcache    (chicken_bit_dcache_i),
    .busy                  (orig_busy_o),
    .cache_op_ready        (orig_cache_op_ready_o),
    .cache_op_valid        (cache_op_valid_i),
    .cache_op_req          (req_orig),
    .cache_op_req_next     (req_next_orig),
    .th_id_complete        (orig_th_id_complete_o),
    .meta_read_bid         (orig_meta_read_bid_o),
    .meta_read_grant       (meta_read_grant_i),
    .meta_read_valid       (orig_meta_read_valid_o),
    .meta_read_phys        (orig_meta_read_phys_o),
    .meta_read_force_set   (orig_meta_read_force_set_o),
    .meta_read_thread_id   (orig_meta_read_thread_id_o),
    .meta_read_skip_pma    (orig_meta_read_skip_pma_o),
    .meta_read_addr        (orig_meta_read_addr_o),
    .meta_read_addr_msb_err(orig_meta_read_addr_msb_err_o),
    .meta_read_resp_way_en (meta_read_resp_way_en_i),
    .meta_read_resp        (meta_read_resp_orig),
    .s1_phys_addr_tag      (s1_phys_addr_tag_i),
    .s1_tlb_wait           (s1_tlb_wait_i),
    .s1_tlb_fail           (s1_tlb_fail_i),
    .meta_write_ready      (meta_write_ready_i),
    .meta_write_valid_next (orig_meta_write_valid_next_o),
    .meta_write_valid      (orig_meta_write_valid_o),
    .meta_write_req        (meta_write_req_orig),
    .nack_set_valid        (orig_nack_set_valid_o),
    .nack_set_value        (orig_nack_set_value_o),
    .writeback_ready       (writeback_ready_i),
    .writeback_valid       (orig_writeback_valid_o),
    .writeback_req         (writeback_req_orig),
    .writeback_done        (writeback_done_i),
    .id_rq_full            (id_rq_full_i),
    .id_co_pf_bid          (orig_id_co_pf_bid_o),
    .id_co_pf_grant        (id_co_pf_grant_i),
    .s0_co_pf_valid        (orig_s0_co_pf_valid_o),
    .s0_co_pf_req          (s0_co_pf_req_orig),
    .cache_op_ready_byp    (cache_op_ready_byp_i),
    .cache_op_valid_byp    (orig_cache_op_valid_byp_o),
    .cache_op_req_byp      (cache_op_req_byp_orig),
    .mh_clear_slock_valid  (mh_clear_slock_valid_i),
    .mh_clear_slock_set    (mh_clear_slock_set_i),
    .mh_clear_slock_way    (mh_clear_slock_way_i),
    .hlock_state           (hlock_state_orig),
    .slock_state           (slock_state_orig),
    .da_clear_en           (orig_da_clear_en_o),
    .da_clear_val          (orig_da_clear_val_o),
    .da_clear_set          (orig_da_clear_set_o),
    .da_clear_way          (orig_da_clear_way_o),
    .da_clear_idx          (orig_da_clear_idx_o),
    .da_clear_rdy          (da_clear_rdy_i),
    .mh_probe_rdy          (mh_probe_rdy_i),
    .mh_metaw_rdy          (mh_metaw_rdy_i),
    .rq_conflict           (rq_conflict_i),
    .cfg_clear_all         (cfg_clear_all_i),
    .cfg_clear_low         (cfg_clear_low_i),
    .cfg_alloc_scp         (cfg_alloc_scp_i),
    .cfg_cache_mode        (dcache_mode_t'(cfg_cache_mode_i)),
    .vm_status             (vm_status_orig),
    .vm_status_co          (vm_status_co_orig),
    .err_flags             (orig_err_flags_o),
    .csr_debug_signals     (orig_csr_debug_signals_o),
    .sm_match_debug_signals(orig_sm_match_debug_signals_o),
    .sm_filter_debug_signals(orig_sm_filter_debug_signals),
    .sm_data_debug_signals (orig_sm_data_debug_signals)
  );

  minion_dcache_cache_op_unit u_new (
    .clk_i,
    .rst_ni,
    .chicken_bit_dcache_i,
    .busy_o                 (new_busy_o),
    .cache_op_ready_o       (new_cache_op_ready_o),
    .cache_op_valid_i,
    .cache_op_req_i         (req_new),
    .cache_op_req_next_i    (req_next_new),
    .th_id_complete_o       (new_th_id_complete_o),
    .meta_read_bid_o        (new_meta_read_bid_o),
    .meta_read_grant_i,
    .meta_read_valid_o      (new_meta_read_valid_o),
    .meta_read_phys_o       (new_meta_read_phys_o),
    .meta_read_force_set_o  (new_meta_read_force_set_o),
    .meta_read_thread_id_o  (new_meta_read_thread_id_o),
    .meta_read_skip_pma_o   (new_meta_read_skip_pma_o),
    .meta_read_addr_o       (new_meta_read_addr_o),
    .meta_read_addr_msb_err_o(new_meta_read_addr_msb_err_o),
    .meta_read_resp_way_en_i,
    .meta_read_resp_i       (meta_read_resp_new),
    .s1_phys_addr_tag_i,
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .meta_write_ready_i,
    .meta_write_valid_next_o(new_meta_write_valid_next_o),
    .meta_write_valid_o     (new_meta_write_valid_o),
    .meta_write_req_o       (meta_write_req_new),
    .nack_set_valid_o       (new_nack_set_valid_o),
    .nack_set_value_o       (new_nack_set_value_o),
    .writeback_ready_i,
    .writeback_valid_o      (new_writeback_valid_o),
    .writeback_req_o        (writeback_req_new),
    .writeback_done_i,
    .id_rq_full_i,
    .id_co_pf_bid_o         (new_id_co_pf_bid_o),
    .id_co_pf_grant_i,
    .s0_co_pf_valid_o       (new_s0_co_pf_valid_o),
    .s0_co_pf_req_o         (s0_co_pf_req_new),
    .cache_op_ready_byp_i,
    .cache_op_valid_byp_o   (new_cache_op_valid_byp_o),
    .cache_op_req_byp_o     (cache_op_req_byp_new),
    .mh_clear_slock_valid_i,
    .mh_clear_slock_set_i,
    .mh_clear_slock_way_i,
    .hlock_state_o          (hlock_state_new),
    .slock_state_o          (slock_state_new),
    .da_clear_en_o          (new_da_clear_en_o),
    .da_clear_val_o         (new_da_clear_val_o),
    .da_clear_set_o         (new_da_clear_set_o),
    .da_clear_way_o         (new_da_clear_way_o),
    .da_clear_idx_o         (new_da_clear_idx_o),
    .da_clear_rdy_i,
    .mh_probe_rdy_i,
    .mh_metaw_rdy_i,
    .rq_conflict_i,
    .cfg_clear_all_i,
    .cfg_clear_low_i,
    .cfg_alloc_scp_i,
    .cfg_cache_mode_i       (dcache_mode_t'(cfg_cache_mode_i)),
    .vm_status_i            (vm_status_new),
    .vm_status_co_o         (vm_status_co_new),
    .err_flags_o            (new_err_flags_o),
    .csr_debug_signals_o    (new_csr_debug_signals_o),
    .sm_match_debug_signals_o(new_sm_match_debug_signals_o),
    .sm_filter_debug_signals_o(new_sm_filter_debug_signals),
    .sm_data_debug_signals_o(new_sm_data_debug_signals)
  );

endmodule : cosim_minion_dcache_cache_op_unit_tb
