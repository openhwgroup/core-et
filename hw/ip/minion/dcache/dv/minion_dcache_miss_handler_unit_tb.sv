// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_miss_handler_unit.

/* verilator lint_off UNOPTFLAT */  // The wrapper exposes preserved handler ready/inflight paths that Verilator flattens into false loops.
/* verilator lint_off SYNCASYNCNET */  // The wrapper intentionally fans one reset into async-reset child arbiters and the preserved synchronous reset flop inside the miss-handler unit.
module minion_dcache_miss_handler_unit_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0] neigh_min_id_i,
  input  logic                         req_valid_i,
  output logic                         req_ready_o,
  input  logic [PaSize-1:0]            req_addr_i,
  input  logic [DcacheSetIdxWidth-1:0] req_set_i,
  input  logic [4:0]                   req_cmd_i,
  input  logic [3:0]                   req_typ_i,
  input  logic [DcacheBufferIdxWidth-1:0] req_buf_id_i,
  input  logic [DcacheWays-1:0]        req_way_en_i,
  input  logic                         req_tag_match_i,
  input  logic [1:0]                   req_old_meta_state_i,
  input  logic [PaTagExtSize-1:0]      req_old_meta_tag_i,
  input  logic                         req_cacheable_i,
  input  logic                         req_thread_id_i,
  input  logic                         req_mem_g_i,
  input  logic                         req_is_amo_i,
  input  logic                         req_is_msg_i,
  input  logic                         req_is_bypass_i,
  output logic [DcacheMhFileSize-1:0]  req_assigned_o,
  output logic [DcacheMhFileSize-1:0]  req_accepted_o,
  output logic                         can_accept_uc_o,
  output logic                         wait_l2_to_ba_resp_o,
  input  logic                         inflight_reduce_i,
  output logic                         going_to_uc_store_o,
  input  logic                         l2_evict_ready_i,
  output logic                         l2_evict_valid_o,
  output logic [4:0]                   l2_evict_id_o,
  output logic [4:0]                   l2_evict_opcode_o,
  output logic [PaSize-1:0]            l2_evict_addr_o,
  output logic [2:0]                   l2_evict_size_o,
  input  logic                         l2_miss_ready_i,
  output logic                         l2_miss_valid_o,
  output logic [4:0]                   l2_miss_id_o,
  output logic [4:0]                   l2_miss_opcode_o,
  output logic [PaSize-1:0]            l2_miss_addr_o,
  output logic [ShireCoopMinMaskSize-1:0] l2_miss_data_o,
  output logic [2:0]                   l2_miss_size_o,
  input  logic                         l2_resp_valid_i,
  input  logic [7:0]                   l2_resp_id_i,
  input  logic [1:0]                   l2_resp_opcode_i,
  input  logic                         l2_resp_slot_avail_i,
  output logic [DcacheMhFileSize-1:0]  l2_resp_stall_o,
  output logic [DcacheWayIdxWidth-1:0] refill_way_o,
  output logic [DcacheLramAddrWidth-1:0] refill_addr_o,
  input  logic                         meta_write_ready_i,
  output logic                         meta_write_valid_next_o,
  output logic                         meta_write_valid_o,
  output logic                         meta_write_clear_slock_o,
  output logic [DcacheSetIdxWidth-1:0] meta_write_set_o,
  output logic [DcacheWays-1:0]        meta_write_way_en_o,
  output logic [1:0]                   meta_write_state_o,
  output logic [PaTagExtSize-1:0]      meta_write_tag_o,
  input  logic                         writeback_ready_i,
  output logic                         writeback_valid_o,
  output logic [PaTagExtSize-1:0]      writeback_tag_o,
  output logic [DcacheSetIdxWidth-1:0] writeback_set_o,
  output logic [DcacheWays-1:0]        writeback_way_en_o,
  input  logic                         writeback_done_i,
  output logic                         force_ba_rptr_o,
  output logic                         force_ba_rptr_qual_o,
  output logic [DcacheBufferIdxWidth-1:0] ba_rptr_o,
  output logic                         ba_is_amo_o,
  output logic [DcacheBufferIdxWidth-1:0] ba_wptr_o,
  output logic                         idle_o,
  output logic [DcacheMhFileSize-1:0]  probe_rdy_o,
  output logic [DcacheMhFileSize-1:0]  metaw_rdy_o,
  output logic [NrThreads-1:0]         fence_rdy_o,
  output logic [DcacheMhFileSize-1:0]  handler_ready_o,
  output logic [DcacheMhFileSize-1:0]  handler_fill_bus_error_o,
  output logic [DcacheMhFileSize-1:0]  handler_uc_load_bus_error_o,
  output logic                         match_miss_line_o,
  output logic [NrThreads-1:0]         bus_err_o,
  output logic [PaSize-1:0]            bus_err_addr_o
);
  dcache_mh_req_t req_info;
  et_link_minion_rsp_info_t l2_resp_data;
  /* verilator lint_off UNUSEDSIGNAL */  // The wrapper only exposes the miss-handler-unit fields this test checks.
  et_link_minion_evict_req_info_t l2_evict_req;
  et_link_minion_miss_req_info_t l2_miss_req;
  dcache_meta_write_req_t meta_write_req;
  dcache_wb_req_t writeback_req;
  logic [DcacheMhFileSize*DcacheDbgCsrMhSize-1:0] csr_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0] sm_match_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0] sm_data_debug_signals;
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    req_info = '0;
    req_info.addr = req_addr_i;
    req_info.set = req_set_i;
    req_info.cmd = dcache_cmd_e'(req_cmd_i);
    req_info.typ = dcache_type_e'(req_typ_i);
    req_info.buf_id = req_buf_id_i;
    req_info.way_en = req_way_en_i;
    req_info.tag_match = req_tag_match_i;
    req_info.old_meta.coh_state = dcache_line_state_e'(req_old_meta_state_i);
    req_info.old_meta.tag = req_old_meta_tag_i;
    req_info.cacheable = req_cacheable_i;
    req_info.thread_id = req_thread_id_i;
    req_info.mem_g = req_mem_g_i;
    req_info.is_amo = req_is_amo_i;
    req_info.is_msg = req_is_msg_i;
    req_info.is_bypass = req_is_bypass_i;

    l2_resp_data = '0;
    l2_resp_data.id = l2_resp_id_i;
    l2_resp_data.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);

    l2_evict_id_o = l2_evict_req.id;
    l2_evict_opcode_o = l2_evict_req.opcode;
    l2_evict_addr_o = l2_evict_req.address;
    l2_evict_size_o = l2_evict_req.size;

    l2_miss_id_o = l2_miss_req.id;
    l2_miss_opcode_o = l2_miss_req.opcode;
    l2_miss_addr_o = l2_miss_req.address;
    l2_miss_data_o = l2_miss_req.data[ShireCoopMinMaskSize-1:0];
    l2_miss_size_o = l2_miss_req.size;

    meta_write_set_o = meta_write_req.set_idx;
    meta_write_way_en_o = meta_write_req.way_en;
    meta_write_state_o = meta_write_req.data.coh_state;
    meta_write_tag_o = meta_write_req.data.tag;

    writeback_tag_o = writeback_req.tag;
    writeback_set_o = writeback_req.set;
    writeback_way_en_o = writeback_req.way_en;
  end

  minion_dcache_miss_handler_unit u_dut (
    .clk_i,
    .rst_ni,
    .neigh_min_id_i,
    .req_ready_o,
    .req_valid_i,
    .req_info_i(req_info),
    .req_assigned_o,
    .req_accepted_o,
    .can_accept_uc_o,
    .wait_l2_to_ba_resp_o,
    .inflight_reduce_i,
    .going_to_uc_store_o,
    .l2_evict_ready_i,
    .l2_evict_valid_o,
    .l2_evict_req_o(l2_evict_req),
    .l2_miss_ready_i,
    .l2_miss_valid_o,
    .l2_miss_req_o(l2_miss_req),
    .l2_resp_valid_i,
    .l2_resp_data_i(l2_resp_data),
    .l2_resp_slot_avail_i,
    .l2_resp_stall_o,
    .refill_way_o,
    .refill_addr_o,
    .meta_write_ready_i,
    .meta_write_valid_next_o,
    .meta_write_valid_o,
    .meta_write_clear_slock_o,
    .meta_write_req_o(meta_write_req),
    .writeback_ready_i,
    .writeback_valid_o,
    .writeback_req_o(writeback_req),
    .writeback_done_i,
    .force_ba_rptr_o,
    .force_ba_rptr_qual_o,
    .ba_rptr_o,
    .ba_is_amo_o,
    .ba_wptr_o,
    .idle_o,
    .probe_rdy_o,
    .metaw_rdy_o,
    .fence_rdy_o,
    .handler_ready_o,
    .handler_fill_bus_error_o,
    .handler_uc_load_bus_error_o,
    .match_miss_line_o,
    .bus_err_o,
    .bus_err_addr_o,
    .csr_debug_signals_o(csr_debug_signals),
    .sm_match_debug_signals_o(sm_match_debug_signals),
    .sm_filter_debug_signals_o(sm_filter_debug_signals),
    .sm_data_debug_signals_o(sm_data_debug_signals)
  );

endmodule : minion_dcache_miss_handler_unit_tb
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on SYNCASYNCNET */
