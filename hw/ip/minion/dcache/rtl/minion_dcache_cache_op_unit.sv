// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module minion_dcache_cache_op_unit
  import dft_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                       clk_i,
  input  logic                                       rst_ni,
  input  logic                                       chicken_bit_dcache_i,
  output logic                                       busy_o,
  output logic                                       cache_op_ready_o,
  input  logic                                       cache_op_valid_i,
  input  core_dcache_op_req_t                        cache_op_req_i,
  input  core_dcache_op_req_next_t                   cache_op_req_next_i,
  output logic [NrThreads-1:0][1:0]                  th_id_complete_o,
  output logic                                       meta_read_bid_o,
  input  logic                                       meta_read_grant_i,
  output logic                                       meta_read_valid_o,
  output logic                                       meta_read_phys_o,
  output logic                                       meta_read_force_set_o,
  output logic                                       meta_read_thread_id_o,
  output logic                                       meta_read_skip_pma_o,
  output logic [minion_dcache_pkg::VaSize-1:0]      meta_read_addr_o,
  output logic                                       meta_read_addr_msb_err_o,
  input  logic [minion_dcache_pkg::DcacheWays-1:0]  meta_read_resp_way_en_i,
  input  dcache_meta_data_t                          meta_read_resp_i [minion_dcache_pkg::DcacheWays-1:0],
  input  logic [PaTagExtSize-1:0]                    s1_phys_addr_tag_i,
  input  logic                                       s1_tlb_wait_i,
  input  logic                                       s1_tlb_fail_i,
  input  logic                                       meta_write_ready_i,
  output logic                                       meta_write_valid_next_o,
  output logic                                       meta_write_valid_o,
  output dcache_meta_write_req_t                     meta_write_req_o,
  output logic                                       nack_set_valid_o,
  output logic [DcacheSetIdxWidth-1:0]               nack_set_value_o,
  input  logic                                       writeback_ready_i,
  output logic                                       writeback_valid_o,
  output dcache_wb_req_t                             writeback_req_o,
  input  logic                                       writeback_done_i,
  input  logic                                       id_rq_full_i,
  output logic                                       id_co_pf_bid_o,
  input  logic                                       id_co_pf_grant_i,
  output logic                                       s0_co_pf_valid_o,
  output dcache_req_t                                s0_co_pf_req_o,
  input  logic                                       cache_op_ready_byp_i,
  output logic                                       cache_op_valid_byp_o,
  output core_dcache_op_req_t                        cache_op_req_byp_o,
  input  logic                                       mh_clear_slock_valid_i,
  input  logic [DcacheSetIdxWidth-1:0]               mh_clear_slock_set_i,
  input  logic [DcacheWayIdxWidth-1:0]               mh_clear_slock_way_i,
  output logic [minion_dcache_pkg::DcacheSets-1:0][minion_dcache_pkg::DcacheWays-1:0] hlock_state_o,
  output logic [minion_dcache_pkg::DcacheSets-1:0][minion_dcache_pkg::DcacheWays-1:0] slock_state_o,
  output logic                                       da_clear_en_o,
  output logic                                       da_clear_val_o,
  output logic [DcacheSetIdxWidth-1:0]               da_clear_set_o,
  output logic [DcacheWayIdxWidth-1:0]               da_clear_way_o,
  output logic                                       da_clear_idx_o,
  input  logic                                       da_clear_rdy_i,
  input  logic [DcacheMhFileSize-1:0]                mh_probe_rdy_i,
  input  logic [DcacheMhFileSize-1:0]                mh_metaw_rdy_i,
  input  logic                                       rq_conflict_i,
  input  logic                                       cfg_clear_all_i,
  input  logic                                       cfg_clear_low_i,
  input  logic                                       cfg_alloc_scp_i,
  input  dcache_mode_t                               cfg_cache_mode_i,
  input  minion_dcache_pkg::vm_status_t [NrThreads-1:0] vm_status_i,
  output minion_dcache_pkg::vm_status_t             vm_status_co_o,
  output logic [DcacheErrFlagRange:0][NrThreads-1:0] err_flags_o,
  output logic [minion_dcache_pkg::DcacheDbgCsrCouSize-1:0] csr_debug_signals_o,
  output logic [DcacheDbgSmMatchBits-1:0]            sm_match_debug_signals_o,
  output logic [DcacheDbgSmFilterBits-1:0]           sm_filter_debug_signals_o,
  output logic [DcacheDbgSmDataBits-1:0]             sm_data_debug_signals_o
);
  /* verilator lint_off UNUSEDSIGNAL */  // Preserved original leaf interface carries fields not all consumed by this subcontroller.

  localparam int unsigned ReqSetWayWidth = DcacheWayIdxWidth + DcacheSetIdxWidth;
  localparam int unsigned SmFilterPayloadBits = 6 + minion_dcache_pkg::VaSize
      + $bits(cache_op_req_t) + $bits(cache_op_state_e);
  localparam int unsigned SmDataGroup0PayloadBits = 2 + 4 + (minion_dcache_pkg::VaSize - 6)
      + ReqSetWayWidth + minion_dcache_pkg::VaSize + $bits(cache_op_req_t);
  localparam int unsigned SmDataGroup1PayloadBits = 4 + PaTagExtSize + DcacheWayIdxWidth
      + $bits(dcache_line_state_e) + minion_dcache_pkg::VaSize
      + minion_dcache_pkg::DcacheWays + PaTagExtSize;

  /* verilator lint_off UNOPTFLAT */  // The translated cache-op FSM reuses packed helper predicates across preserved original stages; Verilator flattens a false loop onto state_d.
  cache_op_state_e                               state_q, state_d;
  /* verilator lint_on UNOPTFLAT */
  cache_op_req_t                                 req_q, req_d;
  logic                                          req_up;
  logic                                          req_up_id;
  logic [minion_dcache_pkg::VaSize-1:0]         req_addr_q, req_addr_d;
  logic                                          req_addr_msb_err_q, req_addr_msb_err_d;
  logic [TensorMaskBits-1:0]                     req_tmask_bits_q;
  logic                                          req_tmask_up;
  logic [3:0]                                    req_count_q, req_count_d;
  logic                                          req_count_up;
  logic [minion_dcache_pkg::VaSize-1:6]         req_stride_q;
  logic [DcacheWayIdxWidth+DcacheSetIdxWidth-1:0] req_set_way_q, req_set_way_d;
  logic                                          meta_state_up;
  /* verilator lint_off UNOPTFLAT */  // Preserved cache-op combinational muxing shares selectors across blocks; field-level paths are acyclic.
  logic [DcacheSetIdxWidth-1:0]                  meta_read_set;
  logic                                          meta_read_resp_match;
  logic [PaTagExtSize-1:0]                       meta_read_mux_tag;
  dcache_line_state_e                            meta_read_mux_state;
  logic                                          meta_read_mux_match;
  logic [DcacheWayIdxWidth-1:0]                  meta_read_mux_way;
  logic [minion_dcache_pkg::DcacheWays-1:0]     meta_force_way_en;
  logic [minion_dcache_pkg::DcacheWays-1:0]     meta_data_way_en_q, meta_data_way_en_d;
  logic [minion_dcache_pkg::PaSize-1:0]         meta_data_req_addr_q, meta_data_req_addr_d;
  logic [PaTagExtSize-1:0]                       phys_addr_tag_q;
  logic                                          phys_addr_up;
  logic                                          is_set_way_op;
  logic                                          is_lock_xx_op;
  dcache_line_state_e                            new_coh_q, new_coh_d;
  logic                                          abort_operation_q, abort_operation_d;
  logic                                          err_lock_q, err_lock_d;
  logic                                          err_mem_q, err_mem_d;
  logic                                          term_loop_q, term_loop_d;
  logic                                          cancel_send_to_l2;
  logic                                          cache_op_accepted;
  logic                                          cache_op_is_l1_q, cache_op_is_l1_d;
  logic                                          cache_op_is_nop_q, cache_op_is_nop_d;
  logic                                          cache_op_start_l1_d;
  logic                                          cache_op_dest_l1_d;
  logic                                          cache_op_prefetch;
  logic                                          send_op_to_l2_q, send_op_to_l2_d;
  logic                                          clear_line;
  logic [DcacheSetIdxWidth-1:0]                  req_set;
  logic [DcacheWayIdxWidth-1:0]                  req_way;
  logic                                          id_running_q;
  logic [DcacheMhFileSize-1:0]                   mh_probe_rdy_reg_q;
  /* verilator lint_on UNOPTFLAT */
  cache_op_conflict_e                            conflict_reason_q, conflict_reason_d;
  logic                                          conflict_reason_up;
  logic                                          da_clear_idx_d;
  logic                                          clear_line_on_q, clear_line_on_d;
  logic                                          state_up;
  logic                                          mh_probe_rdy_up;
  logic                                          state_meta_resp_qual;
  logic                                          meta_write_valid_lock;
  logic                                          found_free_way;
  logic                                          abort_up;
  logic [2:0]                                    count_hlock_zero;
  logic                                          err_ffs;
  logic                                          lock_en;
  logic                                          lock_zero;
  logic [DcacheSetIdxWidth-1:0]                  lock_set;
  logic [DcacheWayIdxWidth-1:0]                  lock_way;
  logic [minion_dcache_pkg::DcacheSets-1:0][minion_dcache_pkg::DcacheWays-1:0] hlock_state_d;
  logic [minion_dcache_pkg::DcacheSets-1:0][minion_dcache_pkg::DcacheWays-1:0] slock_state_d;
  logic                                          hlock_up;
  logic                                          slock_up;
  logic                                          unused_inputs;
  logic                                          clkgt;
  logic                                          clkgt_en;
  dft_t                                          dft_zero;

  /* verilator lint_off UNUSEDSIGNAL */  // This preserved leaf exports full original state bundles and debug paths.
  logic [SmFilterPayloadBits-1:0]      sm_filter_debug_raw;
  logic [SmDataGroup0PayloadBits-1:0]  debug_group0_raw;
  logic [SmDataGroup1PayloadBits-1:0]  debug_group1_raw;
  /* verilator lint_on UNUSEDSIGNAL */

  assign cache_op_start_l1_d = cache_op_req_next_i.start_level == cache_op_L1;
  assign cache_op_dest_l1_d = cache_op_req_next_i.dest_level == cache_op_L1;
  assign cache_op_prefetch = (cache_op_req_i.cmd == cache_op_Pref)
      && (cache_op_req_i.dest_level == cache_op_L1);
  assign unused_inputs = chicken_bit_dcache_i ^ cache_op_req_i.l2_scp_fill ^ ^cache_op_req_i.l2_scp_dest;

  assign cache_op_is_l1_d =
      (cache_op_req_next_i.cmd == cache_op_Lock_VA) ||
      (cache_op_req_next_i.cmd == cache_op_Unlock_VA) ||
      ((cache_op_req_next_i.cmd == cache_op_Flush_SW) && !cache_op_dest_l1_d && cache_op_start_l1_d) ||
      ((cache_op_req_next_i.cmd == cache_op_Evict_SW) && !cache_op_dest_l1_d && cache_op_start_l1_d) ||
      (cache_op_req_next_i.cmd == cache_op_Lock_SW) ||
      (cache_op_req_next_i.cmd == cache_op_Unlock_SW) ||
      ((cache_op_req_next_i.cmd == cache_op_Flush_VA) && !cache_op_dest_l1_d && cache_op_start_l1_d) ||
      ((cache_op_req_next_i.cmd == cache_op_Evict_VA) && !cache_op_dest_l1_d && cache_op_start_l1_d) ||
      ((cache_op_req_next_i.cmd == cache_op_Pref) && cache_op_dest_l1_d);

  assign cache_op_is_nop_d =
      ((cache_op_req_next_i.cmd == cache_op_Flush_SW) && (cache_op_dest_l1_d || !cache_op_start_l1_d)) ||
      ((cache_op_req_next_i.cmd == cache_op_Evict_SW) && (cache_op_dest_l1_d || !cache_op_start_l1_d)) ||
      ((cache_op_req_next_i.cmd == cache_op_Flush_VA) && cache_op_dest_l1_d) ||
      ((cache_op_req_next_i.cmd == cache_op_Evict_VA) && cache_op_dest_l1_d);

  assign cache_op_accepted = cache_op_valid_i && !send_op_to_l2_q && cache_op_is_l1_q;

  always_comb begin
    state_d = state_q;
    cancel_send_to_l2 = 1'b0;
    err_mem_d = 1'b0;
    conflict_reason_d = conflict_reason_q;

    unique case (state_q)
      CacheOpInvalid: begin
        if (cache_op_accepted) begin
          if (!cache_op_req_i.use_tmask) begin
            if (cache_op_prefetch) begin
              state_d = CacheOpL1Prefetch;
            end else begin
              state_d = CacheOpMetaRead;
            end
          end else begin
            state_d = CacheOpWaitTmask;
          end
        end
      end
      CacheOpWaitTmask: begin
        if (cache_op_req_i.tmask_ready) begin
          if (cache_op_req_i.tmask_bits[0]) begin
            if (req_q.prefetch) begin
              state_d = CacheOpL1Prefetch;
            end else begin
              state_d = CacheOpMetaRead;
            end
          end else if (req_count_q != req_q.count) begin
            state_d = CacheOpNextOperation;
          end else begin
            state_d = CacheOpInvalid;
          end
        end
      end
      CacheOpMetaRead: begin
        if (meta_read_valid_o) begin
          state_d = CacheOpMetaRespWait;
        end
      end
      CacheOpMetaRespWait: begin
        state_d = CacheOpMetaResp;
        if (s1_tlb_wait_i) begin
          state_d = CacheOpWaitTlb;
        end else if (s1_tlb_fail_i) begin
          state_d = CacheOpInvalid;
          err_mem_d = 1'b1;
        end
      end
      CacheOpMetaResp: begin
        if (!(&mh_probe_rdy_i)) begin
          state_d = CacheOpWaitConflict;
          conflict_reason_d = CoConflictMh;
          cancel_send_to_l2 = 1'b1;
        end else if (rq_conflict_i) begin
          state_d = CacheOpWaitConflict;
          conflict_reason_d = CoConflictRq;
          cancel_send_to_l2 = 1'b1;
        end else if (req_q.cmd == cache_op_Evict_SW) begin
          if (meta_read_mux_state == StateModified) begin
            state_d = CacheOpWritebackReq;
          end else begin
            if (meta_read_mux_state == StateInvalid) cancel_send_to_l2 = 1'b1;
            state_d = CacheOpMetaWrite;
          end
        end else if (req_q.cmd == cache_op_Flush_SW) begin
          if (meta_read_mux_state == StateModified) begin
            state_d = CacheOpWritebackReq;
          end else begin
            if (meta_read_mux_state == StateInvalid) cancel_send_to_l2 = 1'b1;
            state_d = CacheOpMetaWrite;
          end
        end else if (req_q.cmd == cache_op_Lock_SW) begin
          if (!meta_read_resp_match && (meta_read_mux_state == StateModified)) begin
            state_d = CacheOpWritebackReq;
          end else begin
            state_d = CacheOpMetaWrite;
          end
        end else if (req_q.cmd == cache_op_Unlock_SW) begin
          state_d = CacheOpMetaWriteLock;
        end else if (req_q.cmd == cache_op_Evict_VA) begin
          if (meta_read_mux_match && (meta_read_mux_state == StateModified)) begin
            state_d = CacheOpWritebackReq;
          end else if (meta_read_mux_match) begin
            state_d = CacheOpMetaWrite;
          end else if (req_q.send_to_l2) begin
            state_d = CacheOpWaitNext;
          end else if (req_count_q != req_q.count) begin
            state_d = CacheOpNextOperation;
          end else begin
            state_d = CacheOpInvalid;
          end
        end else if (req_q.cmd == cache_op_Flush_VA) begin
          if (meta_read_mux_match && (meta_read_mux_state == StateModified)) begin
            state_d = CacheOpWritebackReq;
          end else if (meta_read_mux_match) begin
            state_d = CacheOpMetaWrite;
          end else if (req_q.send_to_l2) begin
            state_d = CacheOpWaitNext;
          end else if (req_count_q != req_q.count) begin
            state_d = CacheOpNextOperation;
          end else begin
            state_d = CacheOpInvalid;
          end
        end else if (req_q.cmd == cache_op_Lock_VA) begin
          if (!meta_read_resp_match && (meta_read_mux_state == StateModified)) begin
            state_d = CacheOpWritebackReq;
          end else begin
            state_d = CacheOpMetaWrite;
          end
        end else if (req_q.cmd == cache_op_Unlock_VA) begin
          if (meta_read_mux_match) begin
            state_d = CacheOpMetaWriteLock;
          end else if (req_count_q != req_q.count) begin
            state_d = CacheOpNextOperation;
          end else begin
            state_d = CacheOpInvalid;
          end
        end else begin
          state_d = CacheOpInvalid;
        end

        if (abort_operation_d) begin
          cancel_send_to_l2 = 1'b1;
        end
      end
      CacheOpWaitConflict: begin
        if (conflict_reason_q == CoConflictMh) begin
          if (&(mh_probe_rdy_reg_q | mh_metaw_rdy_i)) begin
            state_d = CacheOpMetaRead;
          end
        end else if (!rq_conflict_i) begin
          state_d = CacheOpMetaRead;
        end
      end
      CacheOpWritebackReq: begin
        if (abort_operation_q) begin
          state_d = CacheOpMetaWrite;
        end else if (writeback_ready_i) begin
          state_d = CacheOpWritebackResp;
        end
      end
      CacheOpWritebackResp: begin
        if (writeback_done_i) begin
          state_d = CacheOpMetaWrite;
        end
      end
      CacheOpMetaWrite: begin
        if (meta_write_ready_i || abort_operation_q) begin
          if (send_op_to_l2_q || clear_line) begin
            state_d = CacheOpWaitNext;
          end else if (req_count_q != req_q.count) begin
            state_d = CacheOpNextOperation;
          end else begin
            state_d = CacheOpInvalid;
          end
        end
      end
      CacheOpMetaWriteLock: begin
        if (clear_line) begin
          state_d = CacheOpWaitNext;
        end else if (req_count_q != req_q.count) begin
          state_d = CacheOpNextOperation;
        end else begin
          state_d = CacheOpInvalid;
        end
      end
      CacheOpL1Prefetch: begin
        if (s0_co_pf_valid_o) begin
          state_d = CacheOpNextOpOrInvalid;
        end
      end
      CacheOpNextOpOrInvalid: begin
        if (!s1_tlb_fail_i) begin
          if (req_count_q != req_q.count) begin
            state_d = CacheOpNextOperation;
          end else begin
            state_d = CacheOpInvalid;
          end
        end else begin
          state_d = CacheOpInvalid;
          err_mem_d = 1'b1;
        end
      end
      CacheOpWaitNext: begin
        if (!send_op_to_l2_q && !clear_line) begin
          if (req_count_q != req_q.count) begin
            state_d = CacheOpNextOperation;
          end else begin
            state_d = CacheOpInvalid;
          end
        end
      end
      CacheOpWaitTlb: begin
        if (!s1_tlb_wait_i) begin
          state_d = CacheOpMetaRead;
        end
      end
      CacheOpNextOperation: begin
        if (term_loop_q) begin
          state_d = CacheOpInvalid;
        end else if (!send_op_to_l2_q && !clear_line) begin
          if (!req_q.use_tmask) begin
            if (req_q.prefetch) begin
              state_d = CacheOpL1Prefetch;
            end else begin
              state_d = CacheOpMetaRead;
            end
          end else if (req_tmask_bits_q[req_count_q]) begin
            if (req_q.prefetch) begin
              state_d = CacheOpL1Prefetch;
            end else begin
              state_d = CacheOpMetaRead;
            end
          end else if (req_count_q == req_q.count) begin
            state_d = CacheOpInvalid;
          end
        end
      end
      default: begin
        state_d = CacheOpInvalid;
      end
    endcase
  end

  assign state_up = (state_q != CacheOpInvalid) || cache_op_valid_i;
  assign mh_probe_rdy_up = (state_q == CacheOpMetaResp);
  assign conflict_reason_up = (state_q == CacheOpMetaResp);
  assign state_meta_resp_qual = (state_q == CacheOpMetaResp) && (state_d != CacheOpWaitConflict);
  assign meta_state_up = state_meta_resp_qual;

  always_comb begin
    send_op_to_l2_d = send_op_to_l2_q;
    if (!send_op_to_l2_q) begin
      send_op_to_l2_d = req_q.send_to_l2 && !abort_operation_q && !cancel_send_to_l2
          && (((state_meta_resp_qual) && (state_d != CacheOpWritebackReq))
          || ((state_q == CacheOpWritebackResp) && (state_d != CacheOpWritebackResp)));
    end else begin
      send_op_to_l2_d = !cache_op_ready_byp_i;
    end

    cache_op_valid_byp_o = send_op_to_l2_q;
    cache_op_req_byp_o = '0;
    cache_op_req_byp_o.cmd = req_q.cmd;
    cache_op_req_byp_o.start_level = cache_op_level_e'(req_q.start_level);
    cache_op_req_byp_o.dest_level = cache_op_level_e'(req_q.dest_level);
    cache_op_req_byp_o.addr = {{(minion_dcache_pkg::VaSize-minion_dcache_pkg::PaSize){1'b0}},
        meta_data_req_addr_q[minion_dcache_pkg::PaSize-1:0]};
    cache_op_req_byp_o.stride = {{(minion_dcache_pkg::VaSize-1){1'b0}}, id_running_q};
    cache_op_req_byp_o.thread_id = req_q.thread_id;
  end

  assign req_up = (state_q == CacheOpInvalid) && (state_d != CacheOpInvalid);
  assign req_tmask_up = (state_q == CacheOpWaitTmask);
  assign req_up_id = (state_q == CacheOpInvalid) && cache_op_ready_o && cache_op_valid_i;
  assign req_count_up = req_up || ((state_d == CacheOpNextOperation) && !send_op_to_l2_q);

  always_comb begin
    req_d = '0;
    req_d.cmd = cache_op_req_i.cmd;
    req_d.snoop_type = (cache_op_req_i.cmd == cache_op_Evict_VA) ? SnoopToInvalid :
        (cache_op_req_i.cmd == cache_op_Evict_SW) ? SnoopToInvalid :
        (cache_op_req_i.cmd == cache_op_Flush_VA) ? SnoopToExclusive :
        (cache_op_req_i.cmd == cache_op_Flush_SW) ? SnoopToExclusive : SnoopToInvalid;
    req_d.thread_id = cache_op_req_i.thread_id;
    req_d.use_tmask = cache_op_req_i.use_tmask;
    req_d.count = cache_op_req_i.count;
    req_d.start_level = cache_op_req_i.start_level;
    req_d.dest_level = cache_op_req_i.dest_level;
    req_d.prefetch = cache_op_prefetch;
    req_d.send_to_l2 = (cache_op_req_i.dest_level > cache_op_L2)
        && ((cache_op_req_i.cmd == cache_op_Flush_SW)
        || (cache_op_req_i.cmd == cache_op_Evict_SW)
        || (cache_op_req_i.cmd == cache_op_Flush_VA)
        || (cache_op_req_i.cmd == cache_op_Evict_VA));

    if (state_q == CacheOpInvalid) begin
      req_addr_d = cache_op_req_i.addr;
      req_addr_msb_err_d = 1'b0;
      req_set_way_d = {cache_op_req_i.way, cache_op_req_i.addr[DcacheSetIdxWidth+5:6]};
      req_count_d = 4'b0;
    end else begin
      req_addr_d = {req_addr_q[minion_dcache_pkg::VaSize-1:6], 6'b0} + {req_stride_q, 6'b0};
      req_addr_msb_err_d = req_addr_q[minion_dcache_pkg::VaSize-1]
          && !req_addr_d[minion_dcache_pkg::VaSize-1];
      req_set_way_d = req_set_way_q + {{(ReqSetWayWidth-1){1'b0}}, 1'b1};
      req_count_d = req_count_q + 4'd1;
    end
  end

  assign req_way = req_set_way_q[ReqSetWayWidth-1 -: DcacheWayIdxWidth];
  assign req_set = req_set_way_q[DcacheSetIdxWidth-1:0];

  always_comb begin
    meta_read_resp_match = 1'b0;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
      if (meta_read_resp_way_en_i[i]) begin
        meta_read_resp_match |= (meta_read_resp_i[i].coh_state >= StateShared);
      end
    end
  end

  always_comb begin
    meta_force_way_en = '0;
    found_free_way = 1'b0;

    if (is_set_way_op || (req_q.cmd == cache_op_Lock_SW)) begin
      meta_read_mux_tag = meta_read_resp_i[req_way].tag;
      meta_read_mux_state = meta_read_resp_i[req_way].coh_state;
      meta_force_way_en = dcache_way_decode(req_way);
      meta_read_mux_way = req_way;
      meta_read_mux_match = 1'b1;
    end else begin
      meta_read_mux_match = |meta_read_resp_way_en_i;
      meta_read_mux_tag = meta_read_resp_i[0].tag;
      meta_read_mux_state = meta_read_resp_i[0].coh_state;
      meta_read_mux_way = dcache_way_encode(meta_read_resp_way_en_i);

      for (int unsigned i = 1; i < minion_dcache_pkg::DcacheWays; i++) begin
        if (meta_read_resp_way_en_i[i]) begin
          meta_read_mux_tag = meta_read_resp_i[i].tag;
          meta_read_mux_state = meta_read_resp_i[i].coh_state;
        end
      end

      if (!meta_read_mux_match && (req_q.cmd == cache_op_Lock_VA)) begin
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
          if ((meta_read_resp_i[i].coh_state == StateInvalid) && !found_free_way) begin
            found_free_way = 1'b1;
            meta_read_mux_way = i[DcacheWayIdxWidth-1:0];
            meta_read_mux_tag = meta_read_resp_i[i].tag;
            meta_read_mux_state = meta_read_resp_i[i].coh_state;
          end
        end
        if (!found_free_way) begin
          for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
            if (!hlock_state_o[meta_read_set][i] && !slock_state_o[meta_read_set][i] && !found_free_way) begin
              found_free_way = 1'b1;
              meta_read_mux_way = i[DcacheWayIdxWidth-1:0];
              meta_read_mux_tag = meta_read_resp_i[i].tag;
              meta_read_mux_state = meta_read_resp_i[i].coh_state;
            end
          end
        end
        if (!found_free_way) begin
          for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
            if (!hlock_state_o[meta_read_set][i] && !found_free_way) begin
              found_free_way = 1'b1;
              meta_read_mux_way = i[DcacheWayIdxWidth-1:0];
              meta_read_mux_tag = meta_read_resp_i[i].tag;
              meta_read_mux_state = meta_read_resp_i[i].coh_state;
            end
          end
        end
      end

      if (!meta_read_mux_match
          && ((req_q.cmd == cache_op_Evict_VA) || (req_q.cmd == cache_op_Flush_VA))) begin
        meta_read_mux_tag = phys_addr_tag_q;
      end
    end
  end

  always_comb begin
    if (is_set_way_op) begin
      meta_data_req_addr_d = {meta_read_mux_tag, req_set[$clog2(DcacheSetRed)-1:0], 6'b0};
    end else begin
      meta_data_req_addr_d = {meta_read_mux_tag, req_addr_q[$clog2(DcacheSetRed)+5:6], 6'b0};
    end
  end

  assign meta_data_way_en_d = is_set_way_op ? meta_force_way_en : dcache_way_decode(meta_read_mux_way);
  assign phys_addr_up = (state_q == CacheOpMetaRespWait);

  always_comb begin
    abort_operation_d = abort_operation_q;
    err_lock_d = 1'b0;
    term_loop_d = term_loop_q;
    count_hlock_zero = '0;
    abort_up = (cache_op_valid_i && (state_q == CacheOpInvalid))
        || (state_q == CacheOpNextOperation) || state_meta_resp_qual;

    if ((cache_op_valid_i && (state_q == CacheOpInvalid)) || (state_q == CacheOpNextOperation)) begin
      abort_operation_d = 1'b0;
    end else if (state_meta_resp_qual) begin
      if (meta_read_mux_match && hlock_state_o[meta_read_set][meta_read_mux_way]) begin
        if ((req_q.cmd != cache_op_Unlock_VA) && (req_q.cmd != cache_op_Unlock_SW)
            && (req_q.cmd != cache_op_Flush_VA) && (req_q.cmd != cache_op_Flush_SW)
            && (req_q.cmd != cache_op_Lock_VA)) begin
          abort_operation_d = 1'b1;
        end
        if (req_q.cmd == cache_op_Lock_SW) begin
          abort_operation_d = 1'b1;
          err_lock_d = 1'b1;
        end
      end
      if ((req_q.cmd == cache_op_Evict_SW) && hlock_state_o[meta_read_set][meta_read_mux_way]) begin
        abort_operation_d = 1'b1;
      end
      if (req_q.cmd == cache_op_Lock_SW) begin
        if ((|meta_read_resp_way_en_i) && (dcache_way_encode(meta_read_resp_way_en_i) != meta_read_mux_way)) begin
          abort_operation_d = 1'b1;
          err_lock_d = 1'b1;
        end
        count_hlock_zero = '0;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
          if (hlock_state_o[meta_read_set][i] == 1'b0) begin
            count_hlock_zero = count_hlock_zero + 3'd1;
          end
        end
        if (count_hlock_zero <= 3'd1) begin
          abort_operation_d = 1'b1;
          err_lock_d = 1'b1;
        end
      end
    end

    if (state_q == CacheOpInvalid) begin
      term_loop_d = 1'b0;
    end else begin
      term_loop_d = err_lock_d | term_loop_q;
    end
  end

  assign err_ffs = err_lock_q | err_mem_q | term_loop_q;
  assign err_flags_o[DcacheErrFlagCoLock] =
      {(err_lock_q && req_q.thread_id), (err_lock_q && !req_q.thread_id)};
  assign err_flags_o[DcacheErrFlagMemFault] =
      {(err_mem_q && req_q.thread_id), (err_mem_q && !req_q.thread_id)};
  assign clkgt_en = cache_op_accepted | busy_o | !(&th_id_complete_o) | !rst_ni | err_ffs;
  assign dft_zero = '0;

  prim_clk_gate u_cgate (
    .clk_i (clk_i),
    .en_i  (clkgt_en | chicken_bit_dcache_i),
    .dft_i (dft_zero),
    .clk_o (clkgt)
  );

  always_ff @(posedge clkgt or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q <= CacheOpInvalid;
    end else if (state_up) begin
      state_q <= state_d;
    end
  end

  always_ff @(posedge clkgt) begin
    meta_read_valid_o <= meta_read_grant_i;
    if (mh_probe_rdy_up) mh_probe_rdy_reg_q <= mh_probe_rdy_i;
    if (conflict_reason_up) conflict_reason_q <= conflict_reason_d;
    if (req_up) req_q <= req_d;
    if (req_up) req_stride_q <= cache_op_req_i.stride[minion_dcache_pkg::VaSize-1:6];
    if (req_tmask_up) req_tmask_bits_q <= cache_op_req_i.tmask_bits;
    if (req_up_id) id_running_q <= cache_op_req_i.stride[0];
    if (req_up) begin
      if (cache_op_req_i.thread_id) begin
        vm_status_co_o <= vm_status_i[1];
      end else begin
        vm_status_co_o <= vm_status_i[0];
      end
    end
    if (req_count_up) begin
      req_addr_q <= req_addr_d;
      req_addr_msb_err_q <= req_addr_msb_err_d;
      req_count_q <= req_count_d;
      req_set_way_q <= req_set_way_d;
    end
    if (meta_state_up) begin
      meta_data_way_en_q <= meta_data_way_en_d;
      meta_data_req_addr_q <= meta_data_req_addr_d;
      new_coh_q <= new_coh_d;
    end
    if (phys_addr_up) phys_addr_tag_q <= s1_phys_addr_tag_i;
    if (abort_up) abort_operation_q <= abort_operation_d;
    s0_co_pf_valid_o <= id_co_pf_grant_i;
  end

  always_ff @(posedge clk_i) begin
    cache_op_is_l1_q <= cache_op_is_l1_d;
    cache_op_is_nop_q <= cache_op_is_nop_d;
  end

  always_ff @(posedge clkgt or negedge rst_ni) begin
    if (!rst_ni) begin
      send_op_to_l2_q <= 1'b0;
      err_lock_q <= 1'b0;
      err_mem_q <= 1'b0;
      term_loop_q <= 1'b0;
      clear_line_on_q <= 1'b0;
      da_clear_idx_o <= 1'b0;
    end else begin
      send_op_to_l2_q <= send_op_to_l2_d;
      err_lock_q <= err_lock_d;
      err_mem_q <= err_mem_d;
      term_loop_q <= term_loop_d;
      clear_line_on_q <= clear_line_on_d;
      da_clear_idx_o <= da_clear_idx_d;
    end
  end

  always_comb begin
    cache_op_ready_o = (state_q == CacheOpInvalid) && !send_op_to_l2_q
        && (cache_op_is_l1_q || cache_op_is_nop_q);
    is_set_way_op = (req_q.cmd == cache_op_Flush_SW) || (req_q.cmd == cache_op_Evict_SW)
        || (req_q.cmd == cache_op_Unlock_SW);
    is_lock_xx_op = (req_q.cmd == cache_op_Lock_SW) || (req_q.cmd == cache_op_Lock_VA);

    meta_read_bid_o = (state_q == CacheOpMetaRead) && !meta_read_valid_o;
    meta_read_set = is_set_way_op ? req_set
        : dcache_set_correct(req_addr_q[DcacheSetIdxWidth+5:6], cfg_cache_mode_i, req_q.thread_id);
    meta_read_addr_o = is_set_way_op
        ? {req_addr_q[minion_dcache_pkg::VaSize-1:DcacheSetIdxWidth+6], req_set, 6'b0}
        : req_addr_q;
    meta_read_addr_msb_err_o = is_set_way_op ? 1'b0 : req_addr_msb_err_q;
    meta_read_phys_o = (req_q.cmd == cache_op_Lock_SW);
    meta_read_force_set_o = is_set_way_op;
    meta_read_thread_id_o = req_q.thread_id;
    meta_read_skip_pma_o = (state_q == CacheOpMetaRespWait) && is_set_way_op;

    writeback_valid_o = (state_q == CacheOpWritebackReq) && !abort_operation_q;
    writeback_req_o = '0;
    writeback_req_o.tag = meta_data_req_addr_q[minion_dcache_pkg::PaSize-1 -: PaTagExtSize];
    writeback_req_o.set = is_set_way_op ? req_set
        : dcache_set_correct(meta_data_req_addr_q[DcacheSetIdxWidth+5:6], cfg_cache_mode_i, req_q.thread_id);
    writeback_req_o.way_en = meta_data_way_en_q;

    meta_write_valid_next_o = (state_d == CacheOpMetaWrite) && !abort_operation_q;
    meta_write_valid_o = (state_q == CacheOpMetaWrite) && !abort_operation_q;
    meta_write_valid_lock = (state_q == CacheOpMetaWriteLock) && !abort_operation_q;
    meta_write_req_o.set_idx = is_set_way_op ? req_set
        : dcache_set_correct(req_addr_q[DcacheSetIdxWidth+5:6], cfg_cache_mode_i, req_q.thread_id);
    meta_write_req_o.way_en = meta_data_way_en_q;
    meta_write_req_o.data.tag = is_lock_xx_op ? phys_addr_tag_q
        : meta_data_req_addr_q[minion_dcache_pkg::PaSize-1 -: PaTagExtSize];
    meta_write_req_o.data.coh_state = is_lock_xx_op ? StateModified : new_coh_q;

    busy_o = (state_q != CacheOpInvalid);
    for (int unsigned th = 0; th < NrThreads; th++) begin
      for (int unsigned id = 0; id <= 1; id++) begin
        if ((id_running_q == id[0]) && (req_q.thread_id == th[0])) begin
          th_id_complete_o[th][id] = !busy_o;
        end else begin
          th_id_complete_o[th][id] = 1'b1;
        end
      end
    end
  end

  always_comb begin
    if (meta_read_mux_state == StateInvalid) begin
      new_coh_d = StateInvalid;
    end else if (meta_read_mux_state == StateShared) begin
      if (req_q.snoop_type == SnoopToInvalid) begin
        new_coh_d = StateInvalid;
      end else begin
        new_coh_d = StateShared;
      end
    end else if (req_q.snoop_type == SnoopToInvalid) begin
      new_coh_d = StateInvalid;
    end else begin
      new_coh_d = StateExclusive;
    end
  end

  always_comb begin
    hlock_state_d = hlock_state_o;
    slock_state_d = slock_state_o;

    lock_en = (meta_write_valid_o || meta_write_valid_lock)
        && ((req_q.cmd == cache_op_Lock_SW) || (req_q.cmd == cache_op_Unlock_SW)
        || (req_q.cmd == cache_op_Lock_VA) || (req_q.cmd == cache_op_Unlock_VA)
        || (req_q.cmd == cache_op_Evict_VA) || (req_q.cmd == cache_op_Evict_SW));
    lock_zero = (meta_write_valid_o || meta_write_valid_lock)
        && ((req_q.cmd == cache_op_Lock_SW) || (req_q.cmd == cache_op_Lock_VA));
    lock_en |= (cfg_clear_all_i || cfg_clear_low_i || cfg_alloc_scp_i);

    if (lock_en) begin
      if (cfg_alloc_scp_i) begin
        for (int unsigned i = 0; i < DcacheSetsForScp; i++) begin
          hlock_state_d[i] = {minion_dcache_pkg::DcacheWays{1'b1}};
          slock_state_d[i] = {minion_dcache_pkg::DcacheWays{1'b1}};
        end
      end else if (cfg_clear_low_i) begin
        for (int unsigned i = 0; i < DcacheSetsScpSplit; i++) begin
          hlock_state_d[i] = '0;
          slock_state_d[i] = '0;
        end
      end else if (cfg_clear_all_i) begin
        hlock_state_d = '0;
        slock_state_d = '0;
      end else if (req_q.cmd == cache_op_Lock_SW) begin
        hlock_state_d[lock_set][lock_way] = 1'b1;
        slock_state_d[lock_set][lock_way] = 1'b0;
      end else if (req_q.cmd == cache_op_Unlock_SW) begin
        hlock_state_d[lock_set][lock_way] = 1'b0;
        slock_state_d[lock_set][lock_way] = 1'b0;
      end else if (req_q.cmd == cache_op_Evict_SW) begin
        slock_state_d[lock_set][lock_way] = 1'b0;
      end else if (req_q.cmd == cache_op_Lock_VA) begin
        slock_state_d[lock_set][lock_way] = !hlock_state_o[lock_set][lock_way];
      end else if ((req_q.cmd == cache_op_Unlock_VA) || (req_q.cmd == cache_op_Evict_VA)) begin
        slock_state_d[lock_set][lock_way] = 1'b0;
      end
    end

    if (mh_clear_slock_valid_i) begin
      slock_state_d[mh_clear_slock_set_i][mh_clear_slock_way_i] = 1'b0;
    end
  end

  always_comb begin
    lock_way = dcache_way_encode(meta_data_way_en_q);
    lock_set = meta_write_req_o.set_idx;
  end

  always_comb begin
    da_clear_en_o = lock_zero || clear_line_on_q;
    da_clear_set_o = lock_set;
    da_clear_way_o = lock_way;
    da_clear_val_o = clear_line_on_q;
    da_clear_idx_d = (da_clear_val_o && da_clear_rdy_i) ? ~da_clear_idx_o : da_clear_idx_o;
    clear_line_on_d = lock_zero || (clear_line_on_q && !(da_clear_val_o && da_clear_rdy_i && da_clear_idx_o));
  end

  assign clear_line = da_clear_en_o;
  assign hlock_up = lock_en;
  assign slock_up = lock_en || mh_clear_slock_valid_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      hlock_state_o <= '0;
      slock_state_o <= '0;
    end else begin
      if (hlock_up) hlock_state_o <= hlock_state_d;
      if (slock_up) slock_state_o <= slock_state_d;
    end
  end

  always_comb begin
    id_co_pf_bid_o = (state_q == CacheOpL1Prefetch) && !id_rq_full_i && !s0_co_pf_valid_o;
    s0_co_pf_req_o = '0;
    s0_co_pf_req_o.addr = {1'b0, req_addr_q[minion_dcache_pkg::VaSize-1:6], 6'b0};
    s0_co_pf_req_o.addr_msb_err = req_addr_msb_err_q;
    s0_co_pf_req_o.cmd = minion_dcache_pkg::dcache_cmd_e'(minion_dcache_pkg::DcCmdPfr);
    s0_co_pf_req_o.typ = minion_dcache_pkg::dcache_type_e'(minion_dcache_pkg::DcTypeD);
    s0_co_pf_req_o.cacheable = 1'b1;
    s0_co_pf_req_o.cache_op = 1'b1;
    s0_co_pf_req_o.dest.thread_id = req_q.thread_id;
    s0_co_pf_req_o.vm_sel = DcacheVmSelCo;
  end

  assign nack_set_valid_o = state_meta_resp_qual
      || (state_q == CacheOpWritebackReq)
      || (state_q == CacheOpWritebackResp)
      || (state_q == CacheOpMetaWrite)
      || (state_q == CacheOpMetaWriteLock)
      || clear_line_on_q;
  assign nack_set_value_o = meta_read_set;

  assign csr_debug_signals_o = state_q[minion_dcache_pkg::DcacheDbgCsrCouSize-1:0];

  assign sm_match_debug_signals_o = {{(DcacheDbgSmMatchBits - 7){1'b0}},
      meta_read_valid_o,
      err_lock_q,
      err_mem_q,
      term_loop_q,
      state_up,
      abort_operation_q,
      cache_op_accepted};

  assign sm_filter_debug_raw = {
      err_lock_q,
      err_mem_q,
      term_loop_q,
      state_up,
      abort_operation_q,
      cache_op_accepted,
      req_addr_q,
      req_q,
      state_q
  };
  assign sm_filter_debug_signals_o = {
      {(DcacheDbgSmFilterBits - SmFilterPayloadBits){1'b0}},
      sm_filter_debug_raw
  };

  assign debug_group0_raw = {
      cache_op_is_l1_q,
      cache_op_is_nop_q,
      req_count_q,
      req_stride_q,
      req_set_way_q,
      req_addr_q,
      req_q
  };
  assign sm_data_debug_signals_o[(3 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = '0;
  assign sm_data_debug_signals_o[(2 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = '0;
  assign sm_data_debug_signals_o[(0 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = {
      {(DcacheDbgSmDataGroupBits - SmDataGroup0PayloadBits){1'b0}},
      debug_group0_raw
  };
  assign debug_group1_raw = {
      err_lock_q,
      err_mem_q,
      term_loop_q,
      meta_read_mux_match,
      meta_read_mux_tag,
      meta_read_mux_way,
      meta_read_mux_state,
      meta_read_addr_o,
      meta_read_resp_way_en_i,
      phys_addr_tag_q
  };
  assign sm_data_debug_signals_o[(1 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = {
      {(DcacheDbgSmDataGroupBits - SmDataGroup1PayloadBits){1'b0}},
      debug_group1_raw
  };
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : minion_dcache_cache_op_unit
