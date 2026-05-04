// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_cache_op_unit.

module minion_dcache_cache_op_unit_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic                             chicken_bit_dcache_i,
  output logic                             busy_o,
  output logic                             cache_op_ready_o,
  input  logic                             cache_op_valid_i,
  input  logic [3:0]                       cache_op_cmd_i,
  input  logic [1:0]                       cache_op_start_level_i,
  input  logic [1:0]                       cache_op_dest_level_i,
  input  logic [3:0]                       cache_op_count_i,
  input  logic [VaSize-1:0]                cache_op_addr_i,
  input  logic [DcacheWayIdxWidth-1:0]     cache_op_way_i,
  input  logic [VaSize-1:0]                cache_op_stride_i,
  input  logic                             cache_op_thread_id_i,
  input  logic                             cache_op_use_tmask_i,
  input  logic                             cache_op_tmask_ready_i,
  input  logic [TensorMaskBits-1:0]        cache_op_tmask_bits_i,
  input  logic [DcacheL2ScpDstBits-1:0]    cache_op_l2_scp_dest_i,
  input  logic                             cache_op_l2_scp_fill_i,
  output logic [NrThreads-1:0][1:0]        th_id_complete_o,
  output logic                             meta_read_bid_o,
  input  logic                             meta_read_grant_i,
  output logic                             meta_read_valid_o,
  output logic                             meta_read_phys_o,
  output logic                             meta_read_force_set_o,
  output logic                             meta_read_thread_id_o,
  output logic                             meta_read_skip_pma_o,
  output logic [VaSize-1:0]                meta_read_addr_o,
  output logic                             meta_read_addr_msb_err_o,
  input  logic [DcacheWays-1:0]            meta_read_resp_way_en_i,
  input  logic [PaTagExtSize-1:0]          meta_read_resp_tag0_i,
  input  logic [PaTagExtSize-1:0]          meta_read_resp_tag1_i,
  input  logic [PaTagExtSize-1:0]          meta_read_resp_tag2_i,
  input  logic [PaTagExtSize-1:0]          meta_read_resp_tag3_i,
  input  logic [1:0]                       meta_read_resp_state0_i,
  input  logic [1:0]                       meta_read_resp_state1_i,
  input  logic [1:0]                       meta_read_resp_state2_i,
  input  logic [1:0]                       meta_read_resp_state3_i,
  input  logic [PaTagExtSize-1:0]          s1_phys_addr_tag_i,
  input  logic                             s1_tlb_wait_i,
  input  logic                             s1_tlb_fail_i,
  input  logic                             meta_write_ready_i,
  output logic                             meta_write_valid_next_o,
  output logic                             meta_write_valid_o,
  output logic [DcacheSetIdxWidth-1:0]     meta_write_set_o,
  output logic [DcacheWays-1:0]            meta_write_way_en_o,
  output logic [PaTagExtSize-1:0]          meta_write_tag_o,
  output logic [1:0]                       meta_write_coh_state_o,
  output logic                             nack_set_valid_o,
  output logic [DcacheSetIdxWidth-1:0]     nack_set_value_o,
  input  logic                             writeback_ready_i,
  output logic                             writeback_valid_o,
  output logic [PaTagExtSize-1:0]          writeback_tag_o,
  output logic [DcacheSetIdxWidth-1:0]     writeback_set_o,
  output logic [DcacheWays-1:0]            writeback_way_en_o,
  input  logic                             writeback_done_i,
  input  logic                             id_rq_full_i,
  output logic                             id_co_pf_bid_o,
  input  logic                             id_co_pf_grant_i,
  output logic                             s0_co_pf_valid_o,
  output logic [VaSize:0]                  s0_co_pf_addr_o,
  output logic [4:0]                       s0_co_pf_cmd_o,
  output logic [3:0]                       s0_co_pf_typ_o,
  output logic                             s0_co_pf_thread_id_o,
  output logic                             s0_co_pf_cache_op_o,
  output logic [DcacheVmSelWidth-1:0]      s0_co_pf_vm_sel_o,
  input  logic                             cache_op_ready_byp_i,
  output logic                             cache_op_valid_byp_o,
  output logic [3:0]                       cache_op_byp_cmd_o,
  output logic [1:0]                       cache_op_byp_start_level_o,
  output logic [1:0]                       cache_op_byp_dest_level_o,
  output logic [VaSize-1:0]                cache_op_byp_addr_o,
  output logic [VaSize-1:0]                cache_op_byp_stride_o,
  output logic                             cache_op_byp_thread_id_o,
  input  logic                             mh_clear_slock_valid_i,
  input  logic [DcacheSetIdxWidth-1:0]     mh_clear_slock_set_i,
  input  logic [DcacheWayIdxWidth-1:0]     mh_clear_slock_way_i,
  input  logic [DcacheSetIdxWidth-1:0]     probe_set_i,
  input  logic [DcacheWayIdxWidth-1:0]     probe_way_i,
  output logic                             hlock_probe_o,
  output logic                             slock_probe_o,
  output logic                             da_clear_en_o,
  output logic                             da_clear_val_o,
  output logic [DcacheSetIdxWidth-1:0]     da_clear_set_o,
  output logic [DcacheWayIdxWidth-1:0]     da_clear_way_o,
  output logic                             da_clear_idx_o,
  input  logic                             da_clear_rdy_i,
  input  logic [DcacheMhFileSize-1:0]      mh_probe_rdy_i,
  input  logic [DcacheMhFileSize-1:0]      mh_metaw_rdy_i,
  input  logic                             rq_conflict_i,
  input  logic                             cfg_clear_all_i,
  input  logic                             cfg_clear_low_i,
  input  logic                             cfg_alloc_scp_i,
  input  logic [1:0]                       cfg_cache_mode_i,
  input  logic [NrThreads-1:0]             vm_status_debug_i,
  input  logic [NrThreads-1:0]             vm_status_sum_i,
  input  logic [NrThreads-1:0]             vm_status_mxr_i,
  input  logic [NrThreads-1:0][1:0]        vm_status_prv_i,
  input  logic [NrThreads-1:0][1:0]        vm_status_mpp_i,
  input  logic [NrThreads-1:0]             vm_status_mprv_i,
  output logic [1:0]                       vm_status_co_prv_o,
  output logic [1:0]                       vm_status_co_mpp_o,
  output logic                             vm_status_co_mprv_o,
  output logic                             vm_status_co_sum_o,
  output logic                             vm_status_co_mxr_o,
  output logic                             vm_status_co_debug_o,
  output logic [DcacheErrFlagRange:0][NrThreads-1:0] err_flags_o,
  output logic [DcacheDbgCsrCouSize-1:0]   csr_debug_signals_o
);

  core_dcache_op_req_t      cache_op_req;
  core_dcache_op_req_next_t cache_op_req_next;
  dcache_meta_data_t        meta_read_resp [DcacheWays];
  /* verilator lint_off UNUSEDSIGNAL */  // The wrapper scalarizes only the fields this directed test checks.
  dcache_meta_write_req_t   meta_write_req;
  dcache_wb_req_t           writeback_req;
  dcache_req_t              s0_co_pf_req;
  core_dcache_op_req_t      cache_op_req_byp;
  vm_status_t [NrThreads-1:0] vm_status;
  vm_status_t               vm_status_co;
  logic [DcacheDbgSmMatchBits-1:0] sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] sm_data_debug_signals;
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    cache_op_req = '0;
    cache_op_req.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    cache_op_req.start_level = cache_op_level_e'(cache_op_start_level_i);
    cache_op_req.dest_level = cache_op_level_e'(cache_op_dest_level_i);
    cache_op_req.count = cache_op_count_i;
    cache_op_req.addr = cache_op_addr_i;
    cache_op_req.way = cache_op_way_i;
    cache_op_req.stride = cache_op_stride_i;
    cache_op_req.thread_id = cache_op_thread_id_i;
    cache_op_req.use_tmask = cache_op_use_tmask_i;
    cache_op_req.tmask_ready = cache_op_tmask_ready_i;
    cache_op_req.tmask_bits = cache_op_tmask_bits_i;
    cache_op_req.l2_scp_dest = cache_op_l2_scp_dest_i;
    cache_op_req.l2_scp_fill = cache_op_l2_scp_fill_i;

    cache_op_req_next = '0;
    cache_op_req_next.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    cache_op_req_next.start_level = cache_op_level_e'(cache_op_start_level_i);
    cache_op_req_next.dest_level = cache_op_level_e'(cache_op_dest_level_i);

    meta_read_resp[0] = '{coh_state: dcache_line_state_e'(meta_read_resp_state0_i), tag: meta_read_resp_tag0_i};
    meta_read_resp[1] = '{coh_state: dcache_line_state_e'(meta_read_resp_state1_i), tag: meta_read_resp_tag1_i};
    meta_read_resp[2] = '{coh_state: dcache_line_state_e'(meta_read_resp_state2_i), tag: meta_read_resp_tag2_i};
    meta_read_resp[3] = '{coh_state: dcache_line_state_e'(meta_read_resp_state3_i), tag: meta_read_resp_tag3_i};

    for (int unsigned i = 0; i < NrThreads; i++) begin
      vm_status[i] = '0;
      vm_status[i].debug = vm_status_debug_i[i];
      vm_status[i].sum = vm_status_sum_i[i];
      vm_status[i].mxr = vm_status_mxr_i[i];
      vm_status[i].prv = vm_status_prv_i[i];
      vm_status[i].mpp = vm_status_mpp_i[i];
      vm_status[i].mprv = vm_status_mprv_i[i];
    end

    meta_write_set_o = meta_write_req.set_idx;
    meta_write_way_en_o = meta_write_req.way_en;
    meta_write_tag_o = meta_write_req.data.tag;
    meta_write_coh_state_o = meta_write_req.data.coh_state;

    writeback_tag_o = writeback_req.tag;
    writeback_set_o = writeback_req.set;
    writeback_way_en_o = writeback_req.way_en;

    s0_co_pf_addr_o = s0_co_pf_req.addr;
    s0_co_pf_cmd_o = s0_co_pf_req.cmd;
    s0_co_pf_typ_o = s0_co_pf_req.typ;
    s0_co_pf_thread_id_o = s0_co_pf_req.dest.thread_id;
    s0_co_pf_cache_op_o = s0_co_pf_req.cache_op;
    s0_co_pf_vm_sel_o = s0_co_pf_req.vm_sel;

    cache_op_byp_cmd_o = cache_op_req_byp.cmd;
    cache_op_byp_start_level_o = cache_op_req_byp.start_level;
    cache_op_byp_dest_level_o = cache_op_req_byp.dest_level;
    cache_op_byp_addr_o = cache_op_req_byp.addr;
    cache_op_byp_stride_o = cache_op_req_byp.stride;
    cache_op_byp_thread_id_o = cache_op_req_byp.thread_id;

    hlock_probe_o = hlock_state[probe_set_i][probe_way_i];
    slock_probe_o = slock_state[probe_set_i][probe_way_i];

    vm_status_co_prv_o = vm_status_co.prv;
    vm_status_co_mpp_o = vm_status_co.mpp;
    vm_status_co_mprv_o = vm_status_co.mprv;
    vm_status_co_sum_o = vm_status_co.sum;
    vm_status_co_mxr_o = vm_status_co.mxr;
    vm_status_co_debug_o = vm_status_co.debug;
  end

  logic [DcacheSets-1:0][DcacheWays-1:0] hlock_state;
  logic [DcacheSets-1:0][DcacheWays-1:0] slock_state;

  minion_dcache_cache_op_unit u_dut (
    .clk_i,
    .rst_ni,
    .chicken_bit_dcache_i,
    .busy_o,
    .cache_op_ready_o,
    .cache_op_valid_i,
    .cache_op_req_i            (cache_op_req),
    .cache_op_req_next_i       (cache_op_req_next),
    .th_id_complete_o,
    .meta_read_bid_o,
    .meta_read_grant_i,
    .meta_read_valid_o,
    .meta_read_phys_o,
    .meta_read_force_set_o,
    .meta_read_thread_id_o,
    .meta_read_skip_pma_o,
    .meta_read_addr_o,
    .meta_read_addr_msb_err_o,
    .meta_read_resp_way_en_i,
    .meta_read_resp_i          (meta_read_resp),
    .s1_phys_addr_tag_i,
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .meta_write_ready_i,
    .meta_write_valid_next_o,
    .meta_write_valid_o,
    .meta_write_req_o          (meta_write_req),
    .nack_set_valid_o,
    .nack_set_value_o,
    .writeback_ready_i,
    .writeback_valid_o,
    .writeback_req_o           (writeback_req),
    .writeback_done_i,
    .id_rq_full_i,
    .id_co_pf_bid_o,
    .id_co_pf_grant_i,
    .s0_co_pf_valid_o,
    .s0_co_pf_req_o            (s0_co_pf_req),
    .cache_op_ready_byp_i,
    .cache_op_valid_byp_o,
    .cache_op_req_byp_o        (cache_op_req_byp),
    .mh_clear_slock_valid_i,
    .mh_clear_slock_set_i,
    .mh_clear_slock_way_i,
    .hlock_state_o             (hlock_state),
    .slock_state_o             (slock_state),
    .da_clear_en_o,
    .da_clear_val_o,
    .da_clear_set_o,
    .da_clear_way_o,
    .da_clear_idx_o,
    .da_clear_rdy_i,
    .mh_probe_rdy_i,
    .mh_metaw_rdy_i,
    .rq_conflict_i,
    .cfg_clear_all_i,
    .cfg_clear_low_i,
    .cfg_alloc_scp_i,
    .cfg_cache_mode_i          (dcache_mode_t'(cfg_cache_mode_i)),
    .vm_status_i               (vm_status),
    .vm_status_co_o            (vm_status_co),
    .err_flags_o,
    .csr_debug_signals_o,
    .sm_match_debug_signals_o  (sm_match_debug_signals),
    .sm_filter_debug_signals_o (sm_filter_debug_signals),
    .sm_data_debug_signals_o   (sm_data_debug_signals)
  );

endmodule : minion_dcache_cache_op_unit_tb
