// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache miss handler unit.
//
// Replaces: dcache_miss_handler_unit

/* verilator lint_off UNOPTFLAT */  // The preserved miss-handler/replay/backpressure cone forms a false cross-module combinational loop only after Verilator flattening.

module minion_dcache_miss_handler_unit
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                                  clk_i,
  input  logic                                                  rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0]               neigh_min_id_i,
  output logic                                                  req_ready_o,
  input  logic                                                  req_valid_i,
  input  dcache_mh_req_t                                        req_info_i,
  output logic [DcacheMhFileSize-1:0]                           req_assigned_o,
  output logic [DcacheMhFileSize-1:0]                           req_accepted_o,
  output logic                                                  can_accept_uc_o,
  output logic                                                  wait_l2_to_ba_resp_o,
  input  logic                                                  inflight_reduce_i,
  output logic                                                  going_to_uc_store_o,
  input  logic                                                  l2_evict_ready_i,
  output logic                                                  l2_evict_valid_o,
  output et_link_minion_evict_req_info_t                        l2_evict_req_o,
  input  logic                                                  l2_miss_ready_i,
  output logic                                                  l2_miss_valid_o,
  output et_link_minion_miss_req_info_t                         l2_miss_req_o,
  input  logic                                                  l2_resp_valid_i,
  input  et_link_minion_rsp_info_t                              l2_resp_data_i,
  input  logic                                                  l2_resp_slot_avail_i,
  output logic [DcacheMhFileSize-1:0]                           l2_resp_stall_o,
  output logic [DcacheWayIdxWidth-1:0]                          refill_way_o,
  output logic [DcacheLramAddrWidth-1:0]                        refill_addr_o,
  input  logic                                                  meta_write_ready_i,
  output logic                                                  meta_write_valid_next_o,
  output logic                                                  meta_write_valid_o,
  output logic                                                  meta_write_clear_slock_o,
  output dcache_meta_write_req_t                                meta_write_req_o,
  input  logic                                                  writeback_ready_i,
  output logic                                                  writeback_valid_o,
  output dcache_wb_req_t                                        writeback_req_o,
  input  logic                                                  writeback_done_i,
  output logic                                                  force_ba_rptr_o,
  output logic                                                  force_ba_rptr_qual_o,
  output logic [DcacheBufferIdxWidth-1:0]                       ba_rptr_o,
  output logic                                                  ba_is_amo_o,
  output logic [DcacheBufferIdxWidth-1:0]                       ba_wptr_o,
  output logic                                                  idle_o,
  output logic [DcacheMhFileSize-1:0]                           probe_rdy_o,
  output logic [DcacheMhFileSize-1:0]                           metaw_rdy_o,
  output logic [NrThreads-1:0]                                  fence_rdy_o,
  output logic [DcacheMhFileSize-1:0]                           handler_ready_o,
  output logic [DcacheMhFileSize-1:0]                           handler_fill_bus_error_o,
  output logic [DcacheMhFileSize-1:0]                           handler_uc_load_bus_error_o,
  output logic                                                  match_miss_line_o,
  output logic [NrThreads-1:0]                                  bus_err_o,
  output logic [minion_dcache_pkg::PaSize-1:0]                                     bus_err_addr_o,
  output logic [DcacheMhFileSize*minion_dcache_pkg::DcacheDbgCsrMhSize-1:0]        csr_debug_signals_o,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_o,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_o,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]  sm_data_debug_signals_o
);
  localparam int unsigned MhIdxWidth = $clog2(DcacheMhFileSize);

  logic [DcacheMhFileSize-1:0][NrThreads-1:0] mh_fence_rdy;
  logic [DcacheMhFileSize-1:0]                mh_probe_rdy;
  logic [DcacheMhFileSize-1:0]                mh_metaw_rdy;
  logic [DcacheMhFileSize-1:0]                mh_req_pri_val;
  logic [DcacheMhFileSize-1:0]                mh_req_pri_rdy;
  logic [DcacheMhFileSize-1:0]                mh_tag_match;
  logic [DcacheMhFileSize-1:0]                mh_evict_match;
  logic [DcacheMhFileSize-1:0]                mh_req_sec_rdy;
  logic [DcacheMhFileSize-1:0]                mh_can_accept_uc;
  logic [DcacheMhFileSize-1:0]                mh_idle;
  logic [DcacheMhFileSize-1:0]                mh_wait_l2_to_ba_resp;
  logic [DcacheMhFileSize-1:0]                mh_have_l2_to_ba_resp;
  logic                                       req_sec_val;

  logic [DcacheMhFileSize-1:0]                mh_l2_evict_valid;
  logic [DcacheMhFileSize-1:0]                mh_l2_evict_ready;
  et_link_minion_evict_req_info_t             mh_l2_evict_req [DcacheMhFileSize];
  logic [DcacheMhFileSize-1:0]                ma_evict_arb_grant;
  logic [MhIdxWidth-1:0]                      ma_evict_arb_winner;

  logic [DcacheMhFileSize-1:0]                mh_l2_miss_valid;
  logic [DcacheMhFileSize-1:0]                mh_l2_miss_ready;
  et_link_minion_miss_req_info_t              mh_l2_miss_req [DcacheMhFileSize];
  logic [DcacheMhFileSize-1:0]                ma_miss_arb_grant;
  logic [MhIdxWidth-1:0]                      ma_miss_arb_winner;

  dcache_meta_write_req_t                     mh_meta_write_req [DcacheMhFileSize];
  logic [DcacheMhFileSize-1:0]                mh_meta_write_valid;
  logic [DcacheMhFileSize-1:0]                mh_meta_write_valid_next;
  logic [DcacheMhFileSize-1:0]                mh_meta_write_clear_slock;
  logic [DcacheMhFileSize-1:0]                mw_arb_grant;
  logic [MhIdxWidth-1:0]                      mw_arb_winner;

  dcache_wb_req_t                             mh_wb_req [DcacheMhFileSize];
  logic [DcacheMhFileSize-1:0]                mh_wb_valid;

  logic [DcacheMhFileSize-1:0][DcacheWayIdxWidth-1:0] mh_refill_way;
  logic [DcacheMhFileSize-1:0][DcacheLramAddrWidth-1:0] mh_refill_addr;
  logic [DcacheMhFileSize-1:0][NrThreads-1:0] mh_bus_err;
  logic [DcacheMhFileSize-1:0][minion_dcache_pkg::PaSize-1:0]    mh_bus_err_addr;
  dcache_miss_info_t                          mh_inflight_misses [DcacheMhFileSize];
  logic [DcacheMhFileSize-1:0][DcacheBufferIdxWidth-1:0] mh_ba_rptr;
  logic [DcacheMhFileSize-1:0]                mh_force_ba_rptr_next;
  logic [DcacheMhFileSize-1:0]                mh_force_ba_rptr_qual;
  logic [DcacheMhFileSize-1:0]                mh_ba_is_amo;
  logic [DcacheMhFileSize-1:0]                mh_req_pri_rdy_sel;
  logic [DcacheMhFileSize-1:0]                mh_going_to_uc_store;
  logic [DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgCsrMhSize-1:0] mh_debug_signals;

  prim_arb_lru #(
    .NumClients(DcacheMhFileSize)
  ) u_l2_evict_arb (
    .clk_i,
    .rst_ni,
    .bid_i   (mh_l2_evict_valid),
    .stall_i (!l2_evict_ready_i),
    .grant_o (ma_evict_arb_grant),
    .winner_o(ma_evict_arb_winner)
  );

  prim_arb_lru #(
    .NumClients(DcacheMhFileSize)
  ) u_l2_miss_arb (
    .clk_i,
    .rst_ni,
    .bid_i   (mh_l2_miss_valid),
    .stall_i (!l2_miss_ready_i),
    .grant_o (ma_miss_arb_grant),
    .winner_o(ma_miss_arb_winner)
  );

  prim_arb_lru #(
    .NumClients(DcacheMhFileSize)
  ) u_meta_write_arb (
    .clk_i,
    .rst_ni,
    .bid_i   (mh_meta_write_valid),
    .stall_i (!meta_write_ready_i),
    .grant_o (mw_arb_grant),
    .winner_o(mw_arb_winner)
  );

  always_comb begin
    req_sec_val = req_valid_i && (|mh_tag_match);

    if (|mh_tag_match) begin
      req_ready_o = |(mh_tag_match & mh_req_sec_rdy);
    end else begin
      req_ready_o = |mh_req_pri_rdy;
    end

    match_miss_line_o = (|mh_tag_match) || (|mh_evict_match);
    probe_rdy_o = mh_probe_rdy;
    metaw_rdy_o = mh_metaw_rdy;
    can_accept_uc_o = |mh_can_accept_uc;
    wait_l2_to_ba_resp_o = |mh_wait_l2_to_ba_resp;

    for (int unsigned i = 0; i < NrThreads; i++) begin
      fence_rdy_o[i] = 1'b1;
      for (int unsigned j = 0; j < DcacheMhFileSize; j++) begin
        fence_rdy_o[i] &= mh_fence_rdy[j][i];
      end
    end

    for (int unsigned i = 0; i < DcacheMhFileSize; i++) begin
      req_assigned_o[i] = (mh_req_pri_val[i] && mh_req_pri_rdy[i])
                       || (req_sec_val && mh_req_sec_rdy[i])
                       || mh_tag_match[i]
                       || mh_evict_match[i]
                       || ((req_info_i.thread_id == i[0]) && !req_info_i.cacheable);

      req_accepted_o[i] = (mh_req_pri_val[i] && mh_req_pri_rdy[i])
                       || (req_sec_val && mh_req_sec_rdy[i]);
    end

    idle_o = &mh_idle;
  end

  always_comb begin
    l2_evict_valid_o = mh_l2_evict_valid[ma_evict_arb_winner];
    l2_evict_req_o = mh_l2_evict_req[ma_evict_arb_winner];
    mh_l2_evict_ready = ma_evict_arb_grant;

    l2_miss_valid_o = mh_l2_miss_valid[ma_miss_arb_winner];
    l2_miss_req_o = mh_l2_miss_req[ma_miss_arb_winner];
    mh_l2_miss_ready = ma_miss_arb_grant;
  end

  always_comb begin
    meta_write_valid_o = |mh_meta_write_valid;
    meta_write_valid_next_o = |mh_meta_write_valid_next;
    meta_write_clear_slock_o = mh_meta_write_clear_slock[mw_arb_winner];
    meta_write_req_o = mh_meta_write_req[mw_arb_winner];
  end

  always_comb begin
    writeback_valid_o = |mh_wb_valid;
    writeback_req_o = '0;
    for (int unsigned i = 0; i < DcacheMhFileSize; i++) begin
      if (mh_wb_valid[i]) begin
        writeback_req_o |= mh_wb_req[i];
      end
    end
  end

  always_comb begin
    refill_way_o = mh_refill_way[l2_resp_data_i.id[MhIdxWidth-1:0]];
    refill_addr_o = mh_refill_addr[l2_resp_data_i.id[MhIdxWidth-1:0]];
  end

  always_comb begin
    bus_err_o = mh_bus_err[0];
    for (int unsigned i = 1; i < DcacheMhFileSize; i++) begin
      bus_err_o |= mh_bus_err[i];
    end

    bus_err_addr_o = mh_bus_err_addr[0];
    for (int unsigned i = 1; i < DcacheMhFileSize; i++) begin
      if (|mh_bus_err[i]) begin
        bus_err_addr_o = mh_bus_err_addr[i];
      end
    end
  end

  always_comb begin
    going_to_uc_store_o = mh_going_to_uc_store[0];
    for (int unsigned i = 1; i < DcacheMhFileSize; i++) begin
      going_to_uc_store_o |= mh_going_to_uc_store[i];
    end
  end

  generate
    for (genvar i = 0; i < DcacheMhFileSize; i++) begin : gen_handlers
      always_comb begin
        mh_req_pri_rdy_sel[i] = mh_req_pri_rdy[i];
        for (int unsigned j = i + 1; j < DcacheMhFileSize; j++) begin
          mh_req_pri_rdy_sel[i] &= !mh_req_pri_rdy[j];
        end

        mh_req_pri_val[i] = req_valid_i && !(|mh_tag_match) && mh_req_pri_rdy_sel[i];
      end

      minion_dcache_miss_handler #(
        .Id(i)
      ) u_miss_handler (
        .clk_i,
        .rst_ni,
        .neigh_min_id_i         (neigh_min_id_i),
        .req_pri_val_i          (mh_req_pri_val[i]),
        .req_pri_rdy_o          (mh_req_pri_rdy[i]),
        .req_sec_val_i          (req_sec_val),
        .req_sec_rdy_o          (mh_req_sec_rdy[i]),
        .can_accept_uc_o        (mh_can_accept_uc[i]),
        .req_info_i             (req_info_i),
        .tag_match_o            (mh_tag_match[i]),
        .evict_match_o          (mh_evict_match[i]),
        .set_o                  (mh_inflight_misses[i].set),
        .inflight_misses_i      (mh_inflight_misses),
        .inflight_reduce_i      (inflight_reduce_i),
        .going_to_uc_store_o    (mh_going_to_uc_store[i]),
        .idle_o                 (mh_idle[i]),
        .wait_l2_to_ba_resp_o   (mh_wait_l2_to_ba_resp[i]),
        .have_l2_to_ba_resp_o   (mh_have_l2_to_ba_resp[i]),
        .l2_evict_ready_i       (mh_l2_evict_ready[i]),
        .l2_evict_valid_o       (mh_l2_evict_valid[i]),
        .l2_evict_req_o         (mh_l2_evict_req[i]),
        .l2_miss_ready_i        (mh_l2_miss_ready[i]),
        .l2_miss_valid_o        (mh_l2_miss_valid[i]),
        .l2_miss_req_o          (mh_l2_miss_req[i]),
        .l2_resp_valid_i        (l2_resp_valid_i),
        .l2_resp_data_i         (l2_resp_data_i),
        .l2_resp_slot_avail_i   (l2_resp_slot_avail_i),
        .l2_resp_stall_o        (l2_resp_stall_o[i]),
        .refill_way_o           (mh_refill_way[i]),
        .refill_addr_o          (mh_refill_addr[i]),
        .force_ba_rptr_next_o   (mh_force_ba_rptr_next[i]),
        .force_ba_rptr_qual_o   (mh_force_ba_rptr_qual[i]),
        .ba_rptr_o              (mh_ba_rptr[i]),
        .ba_is_amo_o            (mh_ba_is_amo[i]),
        .meta_write_ready_i     (mw_arb_grant[i]),
        .meta_write_valid_next_o(mh_meta_write_valid_next[i]),
        .meta_write_valid_o     (mh_meta_write_valid[i]),
        .meta_write_clear_slock_o(mh_meta_write_clear_slock[i]),
        .meta_write_req_o       (mh_meta_write_req[i]),
        .writeback_ready_i      (writeback_ready_i),
        .writeback_valid_o      (mh_wb_valid[i]),
        .writeback_req_o        (mh_wb_req[i]),
        .writeback_done_i       (writeback_done_i),
        .bus_err_o              (mh_bus_err[i]),
        .bus_err_addr_o         (mh_bus_err_addr[i]),
        .fill_bus_error_o       (handler_fill_bus_error_o[i]),
        .uc_load_bus_error_o    (handler_uc_load_bus_error_o[i]),
        .probe_rdy_o            (mh_probe_rdy[i]),
        .fence_rdy_o            (mh_fence_rdy[i]),
        .metaw_rdy_o            (mh_metaw_rdy[i]),
        .ready_o                (handler_ready_o[i]),
        .csr_debug_signals_o    (mh_debug_signals[i]),
        .sm_match_debug_signals_o(sm_match_debug_signals_o[i]),
        .sm_filter_debug_signals_o(sm_filter_debug_signals_o[i]),
        .sm_data_debug_signals_o(sm_data_debug_signals_o[i])
      );

      always_comb begin
        mh_inflight_misses[i].valid = !mh_idle[i];
        mh_inflight_misses[i].way_en = dcache_way_decode(mh_refill_way[i]);
      end
    end
  endgenerate

  /* verilator lint_off SYNCASYNCNET */  // rst_ni intentionally preserves this wrapper flop's original synchronous reset timing while child miss handlers use the same reset asynchronously.
  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      force_ba_rptr_o <= 1'b0;
    end else begin
      force_ba_rptr_o <= |mh_force_ba_rptr_next;
    end
  end
  /* verilator lint_on SYNCASYNCNET */

  always_comb begin
    force_ba_rptr_qual_o = |mh_force_ba_rptr_qual;
    ba_rptr_o = mh_ba_rptr[ma_evict_arb_winner];
    ba_is_amo_o = mh_ba_is_amo[ma_evict_arb_winner];
  end

  always_comb begin
    ba_wptr_o = mh_ba_rptr[0];
    for (int unsigned h = 1; h < DcacheMhFileSize; h++) begin
      if (mh_have_l2_to_ba_resp[h]) begin
        ba_wptr_o = mh_ba_rptr[h];
      end
    end
  end

  generate
    for (genvar i = 0; i < DcacheMhFileSize; i++) begin : gen_csr_pack
      assign csr_debug_signals_o[(i * minion_dcache_pkg::DcacheDbgCsrMhSize) +: minion_dcache_pkg::DcacheDbgCsrMhSize] = mh_debug_signals[i];
    end
  endgenerate
  /* verilator lint_on UNOPTFLAT */

endmodule : minion_dcache_miss_handler_unit
/* verilator lint_on UNOPTFLAT */
