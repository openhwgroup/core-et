// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache miss handler.
//
// Replaces: dcache_miss_handler

/* verilator lint_off UNUSEDSIGNAL */  // The preserved ET-link response struct carries fields this leaf does not inspect.
/* verilator lint_off UNOPTFLAT */  // Request-ready/state muxing is structurally acyclic, but flattening reports false loops on the preserved nets.
module minion_dcache_miss_handler
  import minion_pkg::*;
  import minion_dcache_pkg::*;
#(
  parameter int unsigned Id = 0
) (
  input  logic                                       clk_i,
  input  logic                                       rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0]    neigh_min_id_i,
  input  logic                                       req_pri_val_i,
  output logic                                       req_pri_rdy_o,
  input  logic                                       req_sec_val_i,
  output logic                                       req_sec_rdy_o,
  output logic                                       can_accept_uc_o,
  input  dcache_mh_req_t                             req_info_i,
  output logic                                       tag_match_o,
  output logic                                       evict_match_o,
  output logic [DcacheSetIdxWidth-1:0]               set_o,
  input  dcache_miss_info_t                          inflight_misses_i [DcacheMhFileSize],
  input  logic                                       inflight_reduce_i,
  output logic                                       going_to_uc_store_o,
  output logic                                       idle_o,
  output logic                                       wait_l2_to_ba_resp_o,
  output logic                                       have_l2_to_ba_resp_o,
  input  logic                                       l2_evict_ready_i,
  output logic                                       l2_evict_valid_o,
  output et_link_minion_evict_req_info_t             l2_evict_req_o,
  input  logic                                       l2_miss_ready_i,
  output logic                                       l2_miss_valid_o,
  output et_link_minion_miss_req_info_t              l2_miss_req_o,
  input  logic                                       l2_resp_valid_i,
  input  et_link_minion_rsp_info_t                   l2_resp_data_i,
  input  logic                                       l2_resp_slot_avail_i,
  output logic                                       l2_resp_stall_o,
  output logic [DcacheWayIdxWidth-1:0]               refill_way_o,
  output logic [DcacheLramAddrWidth-1:0]             refill_addr_o,
  output logic                                       force_ba_rptr_next_o,
  output logic                                       force_ba_rptr_qual_o,
  output logic [DcacheBufferIdxWidth-1:0]            ba_rptr_o,
  output logic                                       ba_is_amo_o,
  input  logic                                       meta_write_ready_i,
  output logic                                       meta_write_valid_next_o,
  output logic                                       meta_write_valid_o,
  output logic                                       meta_write_clear_slock_o,
  output dcache_meta_write_req_t                     meta_write_req_o,
  input  logic                                       writeback_ready_i,
  output logic                                       writeback_valid_o,
  output dcache_wb_req_t                             writeback_req_o,
  input  logic                                       writeback_done_i,
  output logic [NrThreads-1:0]                       bus_err_o,
  output logic [minion_dcache_pkg::PaSize-1:0]                          bus_err_addr_o,
`ifdef DCACHE_REPORT_PC
  output logic [47:0]                                bus_err_pc_o,
`endif
  output logic                                       fill_bus_error_o,
  output logic                                       uc_load_bus_error_o,
  output logic                                       probe_rdy_o,
  output logic [NrThreads-1:0]                       fence_rdy_o,
  output logic                                       metaw_rdy_o,
  output logic                                       ready_o,
  output logic [minion_dcache_pkg::DcacheDbgCsrMhSize-1:0]              csr_debug_signals_o,
  output logic [DcacheDbgSmMatchBits-1:0]            sm_match_debug_signals_o,
  output logic [DcacheDbgSmFilterBits-1:0]           sm_filter_debug_signals_o,
  output logic [DcacheDbgSmDataBits-1:0]             sm_data_debug_signals_o
);
  localparam logic [4:0] IdVal = {4'b0000, Id[0]};
  localparam logic       IdThread = Id[0];
  localparam logic [0:0] RefillLast = 1'b1;

  logic [4:0] my_l2_miss_id;
  logic [4:0] my_l2_miss_id_uc_st;
  logic [4:0] my_l2_miss_id_uc_ld;
  logic       l2_resp_is_my_id;
  logic       l2_resp_is_my_id_uc;
  logic       l2_resp_is_my_id_uc_st;
  logic       l2_resp_is_my_id_uc_ld;

  logic                 req_up;
  logic                 req_info_is_write;
  dcache_mh_req_t       req_q;
  dcache_mh_req_t       req_d;

  dcache_mh_state_e     state_q;
  dcache_mh_state_e     state_d;
  dcache_line_state_e   new_coh_q;
  dcache_line_state_e   new_coh_d;
  dcache_line_state_e   coh_on_grant;
  logic                 refill_done;
  logic                 uc_store_ack;
  logic                 writeback_pending_q;
  logic                 writeback_pending_d;
  logic                 metawrite_pending_q;
  logic                 metawrite_pending_d;
  logic                 writeback_alloc_nack;
  logic                 state_was_fill_resp_q;
  logic                 response_err;
  logic                 response_err_fill;
  logic                 response_err_uc;
  logic                 clear_soft_lock_q;
  logic                 clear_soft_lock_d;

  logic [PaTagExtSize-1:0]            tag;
  logic                               set_match;
  dcache_access_type_e                acc_type;
  logic                               sec_rdy;
  logic                               state_before_refill;
  logic                               cmd_requires_second_acquire;
  logic [0:0]                         refill_address_inc_q;
  logic [0:0]                         refill_address_inc_d;
  logic                               refill_address_inc_en;
  logic                               refill_inc_is_last;
  logic                               set_way_conf;
  logic                               force_ba_rptr;
  logic                               l2_evict_valid_d;
  logic                               l2_miss_valid_d;
  logic [1:0]                         l2_resp_opcode;
  logic [ShireCoopMinMaskSize-1:0]    neigh_mask;
  logic [13:0]                        sm_filter_raw;
  logic [127:0]                       sm_data_group0_raw;
  logic [127:0]                       sm_data_group1_raw;

  assign my_l2_miss_id = EtLinkCoreMissIdMissHandler0 + IdVal;
  assign my_l2_miss_id_uc_st = EtLinkCoreMissIdMissHandlerUcSt0 + IdVal;
  assign my_l2_miss_id_uc_ld = EtLinkCoreMissIdMissHandlerUcLd0 + IdVal;

  assign l2_resp_is_my_id = l2_resp_data_i.id[4:0] == my_l2_miss_id;
  assign l2_resp_is_my_id_uc_st = l2_resp_data_i.id[4:0] == my_l2_miss_id_uc_st;
  assign l2_resp_is_my_id_uc_ld = l2_resp_data_i.id[4:0] == my_l2_miss_id_uc_ld;
  assign l2_resp_is_my_id_uc = l2_resp_is_my_id_uc_st || l2_resp_is_my_id_uc_ld;

  assign l2_resp_opcode = l2_resp_data_i.opcode;

  always_comb begin
    req_info_is_write = minion_dcache_pkg::dcache_cmd_is_write(req_info_i.cmd);

    req_d = req_q;
    req_up = 1'b0;

    if (req_pri_val_i && req_pri_rdy_o) begin
      req_d = req_info_i;
      req_up = 1'b1;
    end else if (req_sec_val_i && req_sec_rdy_o
        && (dcache_cmd_to_type(req_q.cmd) < dcache_cmd_to_type(req_info_i.cmd))) begin
      req_d.cmd = req_info_i.cmd;
      req_up = 1'b1;
    end
  end

  always_comb begin
    state_d = state_q;
    new_coh_d = new_coh_q;
    writeback_pending_d = writeback_pending_q;
    metawrite_pending_d = metawrite_pending_q;
    writeback_valid_o = 1'b0;
    clear_soft_lock_d = clear_soft_lock_q;

    writeback_alloc_nack = !req_info_i.tag_match && req_info_i.cacheable
        && (req_info_i.old_meta.coh_state == StateModified)
        && !writeback_ready_i && !req_info_i.is_bypass;

    if (req_pri_val_i && req_pri_rdy_o) begin
      if (req_info_i.tag_match) begin
        if (req_info_is_write && (req_info_i.old_meta.coh_state == StateExclusive)) begin
          new_coh_d = StateModified;
          state_d = MhMetaWriteReq;
          metawrite_pending_d = 1'b1;
        end else begin
          new_coh_d = coh_on_grant;
          state_d = MhFillReq;
          metawrite_pending_d = 1'b1;
        end
      end else if (!req_info_i.cacheable) begin
        state_d = MhUcWaitIdle;
      end else begin
        new_coh_d = coh_on_grant;
        clear_soft_lock_d = 1'b1;
        if (req_info_i.old_meta.coh_state == StateModified) begin
          state_d = MhAcquireWb;
        end else begin
          state_d = MhFillReq;
          metawrite_pending_d = 1'b1;
        end
      end
    end

    if (state_q == MhAcquireWb) begin
      writeback_valid_o = 1'b1;
      if (writeback_ready_i) begin
        state_d = MhFillReq;
        writeback_pending_d = 1'b1;
        metawrite_pending_d = 1'b1;
      end
    end

    if ((state_q == MhFillReq) && l2_miss_ready_i) begin
      state_d = MhFillResp;
    end
    if ((state_q == MhFillResp) && refill_done) begin
      if (!metawrite_pending_q) begin
        state_d = MhInvalid;
      end else if (meta_write_valid_o && meta_write_ready_i) begin
        state_d = MhMetaHazard;
      end else begin
        state_d = MhMetaWriteReq;
      end
    end
    if ((state_q == MhFillResp) && response_err_fill) begin
      if (meta_write_valid_o && meta_write_ready_i) begin
        state_d = MhFillClean;
      end else begin
        metawrite_pending_d = 1'b1;
        new_coh_d = StateInvalid;
        state_d = MhMetaWriteReq;
      end
    end
    if (state_q == MhFillClean) begin
      metawrite_pending_d = 1'b1;
      new_coh_d = StateInvalid;
      state_d = MhMetaWriteReq;
    end
    if ((state_q == MhMetaWriteReq) && meta_write_ready_i) begin
      state_d = MhMetaHazard;
    end
    if (state_q == MhMetaHazard) begin
      state_d = MhInvalid;
    end

    if (state_q == MhUcWaitIdle) begin
      state_d = minion_dcache_pkg::dcache_cmd_is_write(req_q.cmd) ? MhUcStoreWait : MhUcLoadReq;
    end
    if ((state_q == MhUcLoadReq) && l2_miss_ready_i) begin
      state_d = MhUcLoadResp;
    end
    if ((state_q == MhUcLoadResp) && l2_resp_valid_i
        && (l2_resp_opcode == EtLinkRspAckData)
        && l2_resp_is_my_id_uc) begin
      state_d = MhInvalid;
    end
    if ((state_q == MhUcLoadResp) && l2_resp_valid_i
        && (l2_resp_opcode == EtLinkRspErr)
        && l2_resp_is_my_id_uc) begin
      state_d = MhInvalid;
    end
    if ((state_q == MhUcStoreWait) && !inflight_reduce_i) begin
      state_d = MhUcStoreReq;
    end
    if ((state_q == MhUcStoreReq) && l2_evict_ready_i) begin
      if (req_q.is_amo) begin
        state_d = MhUcLoadResp;
      end else if (req_q.is_msg) begin
        state_d = MhInvalid;
      end else begin
        state_d = MhUcStoreAck;
      end
    end
    if ((state_q == MhUcStoreAck) && uc_store_ack) begin
      state_d = MhInvalid;
    end

    if (writeback_pending_q && writeback_done_i) begin
      writeback_pending_d = 1'b0;
    end

    if (metawrite_pending_q && meta_write_ready_i) begin
      metawrite_pending_d = 1'b0;
      clear_soft_lock_d = 1'b0;
    end
  end

  assign metaw_rdy_o = !metawrite_pending_q && (state_q != MhAcquireWb)
                    && (state_q != MhFillReq) && (state_q != MhFillResp)
                    && (state_q != MhFillClean);
  assign meta_write_clear_slock_o = clear_soft_lock_q;

  always_comb begin
    tag = req_q.addr[minion_dcache_pkg::PaSize-1:PaNoTagExtSize];
    set_o = req_q.set;

    set_match = (set_o == req_info_i.set) && ((state_q != MhInvalid) || state_was_fill_resp_q);
    tag_match_o = (tag == req_info_i.addr[minion_dcache_pkg::PaSize-1:PaNoTagExtSize]) && set_match
               && !req_info_i.is_msg && !req_info_i.is_bypass && req_info_i.cacheable;
    evict_match_o = (state_q != MhInvalid) && (set_o == req_info_i.set)
                 && (req_q.way_en == req_info_i.way_en) && !req_info_i.is_msg
                 && !req_info_i.is_bypass && req_info_i.cacheable;
  end

  always_comb begin
    acc_type = minion_dcache_pkg::dcache_cmd_to_type(req_info_i.cmd);
    if (acc_type == DcacheAccessRd) begin
      coh_on_grant = StateExclusive;
    end else begin
      coh_on_grant = StateModified;
    end
  end

  always_comb begin
    cmd_requires_second_acquire = minion_dcache_pkg::dcache_cmd_is_write_intent(req_info_i.cmd)
                               && !minion_dcache_pkg::dcache_cmd_is_write_intent(req_q.cmd);
    state_before_refill = 1'b0;

    sec_rdy = tag_match_o && req_info_i.cacheable && !req_info_i.is_bypass;
    if (!state_before_refill) begin
      sec_rdy &= !cmd_requires_second_acquire;
    end
  end

  assign refill_address_inc_en = l2_resp_valid_i && l2_resp_is_my_id
                              && (l2_resp_opcode == EtLinkRspAckData);
  assign refill_done = refill_address_inc_en && refill_inc_is_last;
  assign refill_inc_is_last = refill_address_inc_q == RefillLast;

  always_comb begin
    if (refill_inc_is_last || (state_q == MhInvalid)) begin
      refill_address_inc_d = '0;
    end else begin
      refill_address_inc_d = refill_address_inc_q + 1'b1;
    end
  end

  assign response_err_fill = l2_resp_valid_i && l2_resp_is_my_id
                          && (l2_resp_opcode == EtLinkRspErr);
  assign response_err_uc = l2_resp_valid_i && l2_resp_is_my_id_uc
                        && (l2_resp_opcode == EtLinkRspErr);
  assign response_err = response_err_fill || response_err_uc;

  assign bus_err_o = {response_err && req_q.thread_id, response_err && !req_q.thread_id};
  assign bus_err_addr_o = req_q.addr;
`ifdef DCACHE_REPORT_PC
  assign bus_err_pc_o = req_q.pc;
`endif

  assign fill_bus_error_o = response_err_fill && (state_q == MhFillResp);
  assign uc_load_bus_error_o = response_err_uc && (state_q == MhUcLoadResp);

  always_comb begin
    set_way_conf = 1'b0;
    for (int unsigned i = 0; i < DcacheMhFileSize; i++) begin
      if ((i != Id) && inflight_misses_i[i].valid
          && (inflight_misses_i[i].set == req_info_i.set)
          && (inflight_misses_i[i].way_en == req_info_i.way_en)) begin
        set_way_conf = 1'b1;
      end
    end

    req_pri_rdy_o = (state_q == MhInvalid)
                 && !writeback_alloc_nack
                 && !(set_way_conf && !req_info_i.is_msg && req_info_i.cacheable)
                 && ((req_info_i.thread_id == IdThread) || req_info_i.cacheable);
    req_sec_rdy_o = sec_rdy;
    can_accept_uc_o = req_pri_rdy_o && !req_info_i.cacheable;

    refill_way_o = dcache_way_encode(req_q.way_en);
    refill_addr_o = {set_o, refill_address_inc_q[0], {CoreL2BlockExtAddrOffsetBits{1'b0}}};

    writeback_req_o.tag = req_q.old_meta.tag;
    writeback_req_o.set = req_q.set;
    writeback_req_o.way_en = req_q.way_en;
    writeback_req_o.is_tbox = 1'b0;
    writeback_req_o.mem_idx = 1'b0;
    writeback_req_o.addr_low = '0;

    l2_evict_valid_d = state_d == MhUcStoreReq;
    l2_evict_req_o.id = et_link_core_miss_id_e'(my_l2_miss_id_uc_st);
    l2_evict_req_o.source = 1'b0;
    l2_evict_req_o.wdata = 1'b1;
    if (req_q.is_amo) begin
      l2_evict_req_o.opcode = EtLinkReqAtomic;
      l2_evict_req_o.subopcode = minion_dcache_pkg::dcache_get_et_link_amo_config(req_q.cmd, req_q.typ, req_q.mem_g);
    end else if (req_q.is_msg) begin
      l2_evict_req_o.opcode = EtLinkReqMsgSendData;
      l2_evict_req_o.subopcode = '0;
    end else begin
      l2_evict_req_o.opcode = EtLinkReqWrite;
      l2_evict_req_o.subopcode = {{EtLinkSubopcodeSize-1{1'b0}}, req_q.mem_g};
    end
    l2_evict_req_o.address = req_q.addr;
    l2_evict_req_o.data = '0;
    l2_evict_req_o.size = minion_dcache_pkg::dcache_uc_size(req_q.typ);
    if (req_q.is_amo || req_q.is_msg) begin
      l2_evict_req_o.qwen = minion_dcache_pkg::dcache_typ2qwen(req_q.typ, 2'b0);
    end else begin
      l2_evict_req_o.qwen = minion_dcache_pkg::dcache_typ2qwen(req_q.typ, req_q.addr[5:4]);
    end

    l2_miss_valid_d = (state_d == MhFillReq) || (state_d == MhUcLoadReq);
    l2_miss_req_o.id = (state_q == MhFillReq)
        ? et_link_core_miss_id_e'(my_l2_miss_id)
        : et_link_core_miss_id_e'(my_l2_miss_id_uc_ld);
    l2_miss_req_o.source = 1'b0;
    l2_miss_req_o.wdata = 1'b0;
    l2_miss_req_o.opcode = EtLinkReqRead;
    if (state_q == MhFillReq) begin
      l2_miss_req_o.subopcode = '0;
    end else begin
      l2_miss_req_o.subopcode = {{EtLinkSubopcodeSize-1{1'b0}}, req_q.mem_g};
    end
    if (state_q == MhFillReq) begin
      l2_miss_req_o.address = {req_q.addr[minion_dcache_pkg::PaSize-1:6], 6'b0};
      l2_miss_req_o.size = EtLinkLine;
    end else begin
      l2_miss_req_o.address = req_q.addr;
      l2_miss_req_o.size = minion_dcache_pkg::dcache_uc_size(req_q.typ);
    end
    l2_miss_req_o.data = '0;
    neigh_mask = '0;
    neigh_mask[neigh_min_id_i] = 1'b1;
    l2_miss_req_o.data[ShireCoopMinMaskSize-1:0] = neigh_mask;
    l2_miss_req_o.qwen = 4'b0000;

    meta_write_valid_next_o = metawrite_pending_d && !writeback_pending_d;
    meta_write_valid_o = metawrite_pending_q && !writeback_pending_q;
    meta_write_req_o.set_idx = set_o;
    meta_write_req_o.way_en = req_q.way_en;
    meta_write_req_o.data.coh_state = new_coh_q;
    meta_write_req_o.data.tag = tag;

    force_ba_rptr_next_o = state_d == MhUcStoreReq;
    force_ba_rptr = state_q == MhUcStoreReq;
    force_ba_rptr_qual_o = force_ba_rptr && l2_evict_ready_i;
    ba_rptr_o = req_q.buf_id;
    ba_is_amo_o = req_q.is_amo;

    idle_o = state_q == MhInvalid;
    going_to_uc_store_o = (state_q == MhUcStoreWait) || (state_q == MhUcStoreReq);
    wait_l2_to_ba_resp_o = state_q == MhUcLoadResp;
    have_l2_to_ba_resp_o = (state_q == MhUcLoadResp) && l2_resp_valid_i && l2_resp_is_my_id_uc;
    ready_o = (state_q == MhInvalid)
           || (state_q == MhMetaHazard)
           || ((state_q == MhFillResp) && refill_inc_is_last && l2_resp_slot_avail_i);
    l2_resp_stall_o = metawrite_pending_q || (writeback_pending_q && !writeback_done_i);
  end

  always_comb begin
    probe_rdy_o = !set_match || (state_q == MhInvalid);
    for (int unsigned i = 0; i < NrThreads; i++) begin
      fence_rdy_o[i] = (state_q == MhInvalid) || (req_q.thread_id != i[0]);
    end
  end

  always_comb begin
    uc_store_ack = l2_resp_valid_i && l2_resp_is_my_id_uc
                && ((l2_resp_opcode == EtLinkRspAck)
                    || (l2_resp_opcode == EtLinkRspErr));
  end

  assign csr_debug_signals_o = state_q;
  assign sm_match_debug_signals_o = '0;
  assign sm_filter_raw = {
    meta_write_valid_o,
    writeback_valid_o,
    bus_err_o,
    tag_match_o,
    evict_match_o,
    can_accept_uc_o,
    req_sec_val_i,
    req_pri_val_i,
    req_up,
    state_q
  };
  assign sm_filter_debug_signals_o = {
    {(DcacheDbgSmFilterBits - $bits(sm_filter_raw)){1'b0}},
    sm_filter_raw
  };
  assign sm_data_group1_raw = {
    8'b0,
    req_pri_val_i,
    req_pri_rdy_o,
    req_sec_val_i,
    req_sec_rdy_o,
    can_accept_uc_o,
    tag_match_o,
    evict_match_o,
    inflight_misses_i[1],
    inflight_misses_i[0],
    inflight_reduce_i,
    going_to_uc_store_o,
    meta_write_req_o,
    writeback_req_o
  };
  assign sm_data_group0_raw = {
    22'b0,
    writeback_pending_q,
    metawrite_pending_q,
    new_coh_q,
    req_q.addr,
    req_q.set,
    req_q.cmd,
    req_q.typ,
    req_q.buf_id,
    req_q.way_en,
    req_q.tag_match,
    req_q.old_meta,
    req_q.cacheable,
    req_q.thread_id,
    req_q.mem_g,
    req_q.is_amo,
    req_q.is_msg,
    req_q.is_bypass
  };
  assign sm_data_debug_signals_o[(3 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = '0;
  assign sm_data_debug_signals_o[(2 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = '0;
  assign sm_data_debug_signals_o[(1 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = sm_data_group1_raw;
  assign sm_data_debug_signals_o[(0 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] = sm_data_group0_raw;

  always_ff @(posedge clk_i) begin
    if (req_up) begin
      req_q <= req_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      state_q <= MhInvalid;
      writeback_pending_q <= 1'b0;
      metawrite_pending_q <= 1'b0;
      clear_soft_lock_q <= 1'b0;
    end else begin
      state_q <= state_d;
      writeback_pending_q <= writeback_pending_d;
      metawrite_pending_q <= metawrite_pending_d;
      clear_soft_lock_q <= clear_soft_lock_d;
    end
  end

  always_ff @(posedge clk_i) begin
    new_coh_q <= new_coh_d;
    state_was_fill_resp_q <= state_q == MhFillResp;
    l2_evict_valid_o <= l2_evict_valid_d;
    l2_miss_valid_o <= l2_miss_valid_d;
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      refill_address_inc_q <= '0;
    end else if (refill_address_inc_en) begin
      refill_address_inc_q <= refill_address_inc_d;
    end
  end
endmodule
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
