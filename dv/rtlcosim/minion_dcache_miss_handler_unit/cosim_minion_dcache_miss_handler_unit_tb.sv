// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_miss_handler_unit vs new
// minion_dcache_miss_handler_unit.

`include "soc.vh"

module cosim_minion_dcache_miss_handler_unit_tb
  import minion_pkg::*;
(
  input  logic                            clk_i,
  input  logic                            rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0] neigh_min_id_i,
  input  logic                            req_valid_i,
  input  logic [minion_dcache_pkg::PaSize-1:0] req_addr_i,
  input  logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] req_set_i,
  input  logic [4:0]                      req_cmd_i,
  input  logic [3:0]                      req_typ_i,
  input  logic [minion_dcache_pkg::DcacheBufferIdxWidth-1:0] req_buf_id_i,
  input  logic [minion_dcache_pkg::DcacheWays-1:0] req_way_en_i,
  input  logic                            req_tag_match_i,
  input  logic [1:0]                      req_old_meta_state_i,
  input  logic [minion_dcache_pkg::PaTagExtSize-1:0] req_old_meta_tag_i,
  input  logic                            req_cacheable_i,
  input  logic                            req_thread_id_i,
  input  logic                            req_mem_g_i,
  input  logic                            req_is_amo_i,
  input  logic                            req_is_msg_i,
  input  logic                            req_is_bypass_i,
  input  logic                            inflight_reduce_i,
  input  logic                            l2_evict_ready_i,
  input  logic                            l2_miss_ready_i,
  input  logic                            l2_resp_valid_i,
  input  logic [7:0]                      l2_resp_id_i,
  input  logic [1:0]                      l2_resp_opcode_i,
  input  logic                            l2_resp_slot_avail_i,
  input  logic                            meta_write_ready_i,
  input  logic                            writeback_ready_i,
  input  logic                            writeback_done_i,
  output logic                            orig_req_ready_o,
  output logic                            new_req_ready_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_req_assigned_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_req_assigned_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_req_accepted_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_req_accepted_o,
  output logic                            orig_can_accept_uc_o,
  output logic                            new_can_accept_uc_o,
  output logic                            orig_wait_l2_to_ba_resp_o,
  output logic                            new_wait_l2_to_ba_resp_o,
  output logic                            orig_going_to_uc_store_o,
  output logic                            new_going_to_uc_store_o,
  output logic                            orig_l2_evict_valid_o,
  output logic                            new_l2_evict_valid_o,
  output logic [4:0]                      orig_l2_evict_id_o,
  output logic [4:0]                      new_l2_evict_id_o,
  output logic [4:0]                      orig_l2_evict_opcode_o,
  output logic [4:0]                      new_l2_evict_opcode_o,
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] orig_l2_evict_subopcode_o,
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] new_l2_evict_subopcode_o,
  output logic [minion_dcache_pkg::PaSize-1:0] orig_l2_evict_addr_o,
  output logic [minion_dcache_pkg::PaSize-1:0] new_l2_evict_addr_o,
  output logic [63:0]                     orig_l2_evict_data0_o,
  output logic [63:0]                     orig_l2_evict_data1_o,
  output logic [63:0]                     orig_l2_evict_data2_o,
  output logic [63:0]                     orig_l2_evict_data3_o,
  output logic [63:0]                     new_l2_evict_data0_o,
  output logic [63:0]                     new_l2_evict_data1_o,
  output logic [63:0]                     new_l2_evict_data2_o,
  output logic [63:0]                     new_l2_evict_data3_o,
  output logic [2:0]                      orig_l2_evict_size_o,
  output logic [2:0]                      new_l2_evict_size_o,
  output logic [minion_dcache_pkg::EtLinkQwenSize-1:0] orig_l2_evict_qwen_o,
  output logic [minion_dcache_pkg::EtLinkQwenSize-1:0] new_l2_evict_qwen_o,
  output logic                            orig_l2_miss_valid_o,
  output logic                            new_l2_miss_valid_o,
  output logic [4:0]                      orig_l2_miss_id_o,
  output logic [4:0]                      new_l2_miss_id_o,
  output logic [4:0]                      orig_l2_miss_opcode_o,
  output logic [4:0]                      new_l2_miss_opcode_o,
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] orig_l2_miss_subopcode_o,
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] new_l2_miss_subopcode_o,
  output logic [minion_dcache_pkg::PaSize-1:0] orig_l2_miss_addr_o,
  output logic [minion_dcache_pkg::PaSize-1:0] new_l2_miss_addr_o,
  output logic [63:0]                     orig_l2_miss_data0_o,
  output logic [63:0]                     orig_l2_miss_data1_o,
  output logic [63:0]                     orig_l2_miss_data2_o,
  output logic [63:0]                     orig_l2_miss_data3_o,
  output logic [63:0]                     new_l2_miss_data0_o,
  output logic [63:0]                     new_l2_miss_data1_o,
  output logic [63:0]                     new_l2_miss_data2_o,
  output logic [63:0]                     new_l2_miss_data3_o,
  output logic [2:0]                      orig_l2_miss_size_o,
  output logic [2:0]                      new_l2_miss_size_o,
  output logic [minion_dcache_pkg::EtLinkQwenSize-1:0] orig_l2_miss_qwen_o,
  output logic [minion_dcache_pkg::EtLinkQwenSize-1:0] new_l2_miss_qwen_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_l2_resp_stall_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_l2_resp_stall_o,
  output logic [minion_dcache_pkg::DcacheWayIdxWidth-1:0] orig_refill_way_o,
  output logic [minion_dcache_pkg::DcacheWayIdxWidth-1:0] new_refill_way_o,
  output logic [minion_dcache_pkg::DcacheLramAddrWidth-1:0] orig_refill_addr_o,
  output logic [minion_dcache_pkg::DcacheLramAddrWidth-1:0] new_refill_addr_o,
  output logic                            orig_meta_write_valid_next_o,
  output logic                            new_meta_write_valid_next_o,
  output logic                            orig_meta_write_valid_o,
  output logic                            new_meta_write_valid_o,
  output logic                            orig_meta_write_clear_slock_o,
  output logic                            new_meta_write_clear_slock_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] orig_meta_write_set_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] new_meta_write_set_o,
  output logic [minion_dcache_pkg::DcacheWays-1:0] orig_meta_write_way_en_o,
  output logic [minion_dcache_pkg::DcacheWays-1:0] new_meta_write_way_en_o,
  output logic [1:0]                      orig_meta_write_state_o,
  output logic [1:0]                      new_meta_write_state_o,
  output logic [minion_dcache_pkg::PaTagExtSize-1:0] orig_meta_write_tag_o,
  output logic [minion_dcache_pkg::PaTagExtSize-1:0] new_meta_write_tag_o,
  output logic                            orig_writeback_valid_o,
  output logic                            new_writeback_valid_o,
  output logic [minion_dcache_pkg::PaTagExtSize-1:0] orig_writeback_tag_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] orig_writeback_set_o,
  output logic [minion_dcache_pkg::DcacheWays-1:0] orig_writeback_way_en_o,
  output logic                            orig_writeback_is_tbox_o,
  output logic                            orig_writeback_mem_idx_o,
  output logic [minion_dcache_pkg::PaNoTagExtSize-1:0] orig_writeback_addr_low_o,
  output logic [minion_dcache_pkg::PaTagExtSize-1:0] new_writeback_tag_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] new_writeback_set_o,
  output logic [minion_dcache_pkg::DcacheWays-1:0] new_writeback_way_en_o,
  output logic                            new_writeback_is_tbox_o,
  output logic                            new_writeback_mem_idx_o,
  output logic [minion_dcache_pkg::PaNoTagExtSize-1:0] new_writeback_addr_low_o,
  output logic                            orig_force_ba_rptr_o,
  output logic                            new_force_ba_rptr_o,
  output logic                            orig_force_ba_rptr_qual_o,
  output logic                            new_force_ba_rptr_qual_o,
  output logic [minion_dcache_pkg::DcacheBufferIdxWidth-1:0] orig_ba_rptr_o,
  output logic [minion_dcache_pkg::DcacheBufferIdxWidth-1:0] new_ba_rptr_o,
  output logic                            orig_ba_is_amo_o,
  output logic                            new_ba_is_amo_o,
  output logic [minion_dcache_pkg::DcacheBufferIdxWidth-1:0] orig_ba_wptr_o,
  output logic [minion_dcache_pkg::DcacheBufferIdxWidth-1:0] new_ba_wptr_o,
  output logic                            orig_idle_o,
  output logic                            new_idle_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_probe_rdy_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_probe_rdy_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_metaw_rdy_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_metaw_rdy_o,
  output logic [NrThreads-1:0]            orig_fence_rdy_o,
  output logic [NrThreads-1:0]            new_fence_rdy_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_handler_ready_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_handler_ready_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_handler_fill_bus_error_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_handler_fill_bus_error_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] orig_handler_uc_load_bus_error_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0] new_handler_uc_load_bus_error_o,
  output logic                            orig_match_miss_line_o,
  output logic                            new_match_miss_line_o,
  output logic [NrThreads-1:0]            orig_bus_err_o,
  output logic [NrThreads-1:0]            new_bus_err_o,
  output logic [minion_dcache_pkg::PaSize-1:0] orig_bus_err_addr_o,
  output logic [minion_dcache_pkg::PaSize-1:0] new_bus_err_addr_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize*minion_dcache_pkg::DcacheDbgCsrMhSize-1:0] orig_csr_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize*minion_dcache_pkg::DcacheDbgCsrMhSize-1:0] new_csr_debug_signals_o,
  output logic [63:0]                     orig_sm_match_h0_o,
  output logic [63:0]                     orig_sm_match_h1_o,
  output logic [63:0]                     new_sm_match_h0_o,
  output logic [63:0]                     new_sm_match_h1_o,
  output logic [63:0]                     orig_sm_filter_h0_chunk0_o,
  output logic [63:0]                     orig_sm_filter_h0_chunk1_o,
  output logic [63:0]                     orig_sm_filter_h0_chunk2_o,
  output logic [63:0]                     orig_sm_filter_h0_chunk3_o,
  output logic [63:0]                     orig_sm_filter_h1_chunk0_o,
  output logic [63:0]                     orig_sm_filter_h1_chunk1_o,
  output logic [63:0]                     orig_sm_filter_h1_chunk2_o,
  output logic [63:0]                     orig_sm_filter_h1_chunk3_o,
  output logic [63:0]                     new_sm_filter_h0_chunk0_o,
  output logic [63:0]                     new_sm_filter_h0_chunk1_o,
  output logic [63:0]                     new_sm_filter_h0_chunk2_o,
  output logic [63:0]                     new_sm_filter_h0_chunk3_o,
  output logic [63:0]                     new_sm_filter_h1_chunk0_o,
  output logic [63:0]                     new_sm_filter_h1_chunk1_o,
  output logic [63:0]                     new_sm_filter_h1_chunk2_o,
  output logic [63:0]                     new_sm_filter_h1_chunk3_o,
  output logic [63:0]                     orig_sm_data_h0_chunk0_o,
  output logic [63:0]                     orig_sm_data_h0_chunk1_o,
  output logic [63:0]                     orig_sm_data_h0_chunk2_o,
  output logic [63:0]                     orig_sm_data_h0_chunk3_o,
  output logic [63:0]                     orig_sm_data_h0_chunk4_o,
  output logic [63:0]                     orig_sm_data_h0_chunk5_o,
  output logic [63:0]                     orig_sm_data_h0_chunk6_o,
  output logic [63:0]                     orig_sm_data_h0_chunk7_o,
  output logic [63:0]                     orig_sm_data_h1_chunk0_o,
  output logic [63:0]                     orig_sm_data_h1_chunk1_o,
  output logic [63:0]                     orig_sm_data_h1_chunk2_o,
  output logic [63:0]                     orig_sm_data_h1_chunk3_o,
  output logic [63:0]                     orig_sm_data_h1_chunk4_o,
  output logic [63:0]                     orig_sm_data_h1_chunk5_o,
  output logic [63:0]                     orig_sm_data_h1_chunk6_o,
  output logic [63:0]                     orig_sm_data_h1_chunk7_o,
  output logic [63:0]                     new_sm_data_h0_chunk0_o,
  output logic [63:0]                     new_sm_data_h0_chunk1_o,
  output logic [63:0]                     new_sm_data_h0_chunk2_o,
  output logic [63:0]                     new_sm_data_h0_chunk3_o,
  output logic [63:0]                     new_sm_data_h0_chunk4_o,
  output logic [63:0]                     new_sm_data_h0_chunk5_o,
  output logic [63:0]                     new_sm_data_h0_chunk6_o,
  output logic [63:0]                     new_sm_data_h0_chunk7_o,
  output logic [63:0]                     new_sm_data_h1_chunk0_o,
  output logic [63:0]                     new_sm_data_h1_chunk1_o,
  output logic [63:0]                     new_sm_data_h1_chunk2_o,
  output logic [63:0]                     new_sm_data_h1_chunk3_o,
  output logic [63:0]                     new_sm_data_h1_chunk4_o,
  output logic [63:0]                     new_sm_data_h1_chunk5_o,
  output logic [63:0]                     new_sm_data_h1_chunk6_o,
  output logic [63:0]                     new_sm_data_h1_chunk7_o
);
  minion_dcache_pkg::dcache_mh_req_t req_info_new;
  dcache_mh_req req_info_orig;
  et_link_minion_rsp_info_t l2_resp_data_new;
  et_link_minion_rsp_info_t l2_resp_data_orig;
  minion_dcache_pkg::et_link_minion_evict_req_info_t l2_evict_req_new;
  et_link_minion_evict_req_info_t l2_evict_req_orig;
  minion_dcache_pkg::et_link_minion_miss_req_info_t l2_miss_req_new;
  et_link_minion_miss_req_info_t l2_miss_req_orig;
  minion_dcache_pkg::dcache_meta_write_req_t meta_write_req_new;
  dcache_meta_write_req meta_write_req_orig;
  minion_dcache_pkg::dcache_wb_req_t writeback_req_new;
  dcache_wb_req writeback_req_orig;
  logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] new_sm_match_debug_signals_o;
  logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] new_sm_filter_debug_signals_o;
  logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmDataBits-1:0] new_sm_data_debug_signals_o;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_MATCH_BITS-1:0] orig_sm_match_debug_signals_o;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_FILTER_BITS-1:0] orig_sm_filter_debug_signals_o;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_DATA_BITS-1:0] orig_sm_data_debug_signals_o;

  always_comb begin
    req_info_new = '0;
    req_info_new.addr = req_addr_i;
    req_info_new.set = req_set_i;
    req_info_new.cmd = minion_dcache_pkg::dcache_cmd_e'(req_cmd_i);
    req_info_new.typ = minion_dcache_pkg::dcache_type_e'(req_typ_i);
    req_info_new.buf_id = req_buf_id_i;
    req_info_new.way_en = req_way_en_i;
    req_info_new.tag_match = req_tag_match_i;
    req_info_new.old_meta.coh_state = minion_dcache_pkg::dcache_line_state_e'(req_old_meta_state_i);
    req_info_new.old_meta.tag = req_old_meta_tag_i;
    req_info_new.cacheable = req_cacheable_i;
    req_info_new.thread_id = req_thread_id_i;
    req_info_new.mem_g = req_mem_g_i;
    req_info_new.is_amo = req_is_amo_i;
    req_info_new.is_msg = req_is_msg_i;
    req_info_new.is_bypass = req_is_bypass_i;

    req_info_orig = '0;
    req_info_orig.addr = req_addr_i;
    req_info_orig.set = req_set_i;
    req_info_orig.cmd = dcache_cmd'(req_cmd_i);
    req_info_orig.typ = dcache_type'(req_typ_i);
    req_info_orig.buf_id = req_buf_id_i;
    req_info_orig.way_en = req_way_en_i;
    req_info_orig.tag_match = req_tag_match_i;
    req_info_orig.old_meta.coh_state = dcache_line_state'(req_old_meta_state_i);
    req_info_orig.old_meta.tag = req_old_meta_tag_i;
    req_info_orig.cacheable = req_cacheable_i;
    req_info_orig.thread_id = req_thread_id_i;
    req_info_orig.mem_g = req_mem_g_i;
    req_info_orig.is_amo = req_is_amo_i;
    req_info_orig.is_msg = req_is_msg_i;
    req_info_orig.is_bypass = req_is_bypass_i;

    l2_resp_data_new = '0;
    l2_resp_data_new.id = minion_dcache_pkg::et_link_core_miss_id_e'(l2_resp_id_i[4:0]);
    l2_resp_data_new.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);

    l2_resp_data_orig = '0;
    l2_resp_data_orig.id = et_link_core_miss_id_t'(l2_resp_id_i[4:0]);
    l2_resp_data_orig.opcode = et_link_rsp_opcode_t'(l2_resp_opcode_i);
  end

  assign orig_l2_evict_id_o = l2_evict_req_orig.id;
  assign new_l2_evict_id_o = l2_evict_req_new.id;
  assign orig_l2_evict_opcode_o = l2_evict_req_orig.opcode;
  assign new_l2_evict_opcode_o = l2_evict_req_new.opcode;
  assign orig_l2_evict_subopcode_o = l2_evict_req_orig.subopcode;
  assign new_l2_evict_subopcode_o = l2_evict_req_new.subopcode;
  assign orig_l2_evict_addr_o = l2_evict_req_orig.address;
  assign new_l2_evict_addr_o = l2_evict_req_new.address;
  assign {orig_l2_evict_data3_o, orig_l2_evict_data2_o, orig_l2_evict_data1_o, orig_l2_evict_data0_o} = l2_evict_req_orig.data;
  assign {new_l2_evict_data3_o, new_l2_evict_data2_o, new_l2_evict_data1_o, new_l2_evict_data0_o} = l2_evict_req_new.data;
  assign orig_l2_evict_size_o = l2_evict_req_orig.size;
  assign new_l2_evict_size_o = l2_evict_req_new.size;
  assign orig_l2_evict_qwen_o = l2_evict_req_orig.qwen;
  assign new_l2_evict_qwen_o = l2_evict_req_new.qwen;

  assign orig_l2_miss_id_o = l2_miss_req_orig.id;
  assign new_l2_miss_id_o = l2_miss_req_new.id;
  assign orig_l2_miss_opcode_o = l2_miss_req_orig.opcode;
  assign new_l2_miss_opcode_o = l2_miss_req_new.opcode;
  assign orig_l2_miss_subopcode_o = l2_miss_req_orig.subopcode;
  assign new_l2_miss_subopcode_o = l2_miss_req_new.subopcode;
  assign orig_l2_miss_addr_o = l2_miss_req_orig.address;
  assign new_l2_miss_addr_o = l2_miss_req_new.address;
  assign {orig_l2_miss_data3_o, orig_l2_miss_data2_o, orig_l2_miss_data1_o, orig_l2_miss_data0_o} = l2_miss_req_orig.data;
  assign {new_l2_miss_data3_o, new_l2_miss_data2_o, new_l2_miss_data1_o, new_l2_miss_data0_o} = l2_miss_req_new.data;
  assign orig_l2_miss_size_o = l2_miss_req_orig.size;
  assign new_l2_miss_size_o = l2_miss_req_new.size;
  assign orig_l2_miss_qwen_o = l2_miss_req_orig.qwen;
  assign new_l2_miss_qwen_o = l2_miss_req_new.qwen;

  assign orig_meta_write_set_o = meta_write_req_orig.set;
  assign new_meta_write_set_o = meta_write_req_new.set_idx;
  assign orig_meta_write_way_en_o = meta_write_req_orig.way_en;
  assign new_meta_write_way_en_o = meta_write_req_new.way_en;
  assign orig_meta_write_state_o = meta_write_req_orig.data.coh_state;
  assign new_meta_write_state_o = meta_write_req_new.data.coh_state;
  assign orig_meta_write_tag_o = meta_write_req_orig.data.tag;
  assign new_meta_write_tag_o = meta_write_req_new.data.tag;

  assign orig_writeback_tag_o = writeback_req_orig.tag;
  assign orig_writeback_set_o = writeback_req_orig.set;
  assign orig_writeback_way_en_o = writeback_req_orig.way_en;
  assign orig_writeback_is_tbox_o = writeback_req_orig.is_tbox;
  assign orig_writeback_mem_idx_o = writeback_req_orig.mem_idx;
  assign orig_writeback_addr_low_o = writeback_req_orig.addr_low;
  assign new_writeback_tag_o = writeback_req_new.tag;
  assign new_writeback_set_o = writeback_req_new.set;
  assign new_writeback_way_en_o = writeback_req_new.way_en;
  assign new_writeback_is_tbox_o = writeback_req_new.is_tbox;
  assign new_writeback_mem_idx_o = writeback_req_new.mem_idx;
  assign new_writeback_addr_low_o = writeback_req_new.addr_low;

  assign orig_sm_match_h0_o = orig_sm_match_debug_signals_o[0];
  assign orig_sm_match_h1_o = orig_sm_match_debug_signals_o[1];
  assign new_sm_match_h0_o = new_sm_match_debug_signals_o[0];
  assign new_sm_match_h1_o = new_sm_match_debug_signals_o[1];

  assign orig_sm_filter_h0_chunk0_o = orig_sm_filter_debug_signals_o[0][63:0];
  assign orig_sm_filter_h0_chunk1_o = orig_sm_filter_debug_signals_o[0][127:64];
  assign orig_sm_filter_h0_chunk2_o = orig_sm_filter_debug_signals_o[0][191:128];
  assign orig_sm_filter_h0_chunk3_o = {56'b0, orig_sm_filter_debug_signals_o[0][199:192]};
  assign orig_sm_filter_h1_chunk0_o = orig_sm_filter_debug_signals_o[1][63:0];
  assign orig_sm_filter_h1_chunk1_o = orig_sm_filter_debug_signals_o[1][127:64];
  assign orig_sm_filter_h1_chunk2_o = orig_sm_filter_debug_signals_o[1][191:128];
  assign orig_sm_filter_h1_chunk3_o = {56'b0, orig_sm_filter_debug_signals_o[1][199:192]};
  assign new_sm_filter_h0_chunk0_o = new_sm_filter_debug_signals_o[0][63:0];
  assign new_sm_filter_h0_chunk1_o = new_sm_filter_debug_signals_o[0][127:64];
  assign new_sm_filter_h0_chunk2_o = new_sm_filter_debug_signals_o[0][191:128];
  assign new_sm_filter_h0_chunk3_o = {56'b0, new_sm_filter_debug_signals_o[0][199:192]};
  assign new_sm_filter_h1_chunk0_o = new_sm_filter_debug_signals_o[1][63:0];
  assign new_sm_filter_h1_chunk1_o = new_sm_filter_debug_signals_o[1][127:64];
  assign new_sm_filter_h1_chunk2_o = new_sm_filter_debug_signals_o[1][191:128];
  assign new_sm_filter_h1_chunk3_o = {56'b0, new_sm_filter_debug_signals_o[1][199:192]};

  assign orig_sm_data_h0_chunk0_o = orig_sm_data_debug_signals_o[0][63:0];
  assign orig_sm_data_h0_chunk1_o = orig_sm_data_debug_signals_o[0][127:64];
  assign orig_sm_data_h0_chunk2_o = orig_sm_data_debug_signals_o[0][191:128];
  assign orig_sm_data_h0_chunk3_o = orig_sm_data_debug_signals_o[0][255:192];
  assign orig_sm_data_h0_chunk4_o = orig_sm_data_debug_signals_o[0][319:256];
  assign orig_sm_data_h0_chunk5_o = orig_sm_data_debug_signals_o[0][383:320];
  assign orig_sm_data_h0_chunk6_o = orig_sm_data_debug_signals_o[0][447:384];
  assign orig_sm_data_h0_chunk7_o = orig_sm_data_debug_signals_o[0][511:448];
  assign orig_sm_data_h1_chunk0_o = orig_sm_data_debug_signals_o[1][63:0];
  assign orig_sm_data_h1_chunk1_o = orig_sm_data_debug_signals_o[1][127:64];
  assign orig_sm_data_h1_chunk2_o = orig_sm_data_debug_signals_o[1][191:128];
  assign orig_sm_data_h1_chunk3_o = orig_sm_data_debug_signals_o[1][255:192];
  assign orig_sm_data_h1_chunk4_o = orig_sm_data_debug_signals_o[1][319:256];
  assign orig_sm_data_h1_chunk5_o = orig_sm_data_debug_signals_o[1][383:320];
  assign orig_sm_data_h1_chunk6_o = orig_sm_data_debug_signals_o[1][447:384];
  assign orig_sm_data_h1_chunk7_o = orig_sm_data_debug_signals_o[1][511:448];
  assign new_sm_data_h0_chunk0_o = new_sm_data_debug_signals_o[0][63:0];
  assign new_sm_data_h0_chunk1_o = new_sm_data_debug_signals_o[0][127:64];
  assign new_sm_data_h0_chunk2_o = new_sm_data_debug_signals_o[0][191:128];
  assign new_sm_data_h0_chunk3_o = new_sm_data_debug_signals_o[0][255:192];
  assign new_sm_data_h0_chunk4_o = new_sm_data_debug_signals_o[0][319:256];
  assign new_sm_data_h0_chunk5_o = new_sm_data_debug_signals_o[0][383:320];
  assign new_sm_data_h0_chunk6_o = new_sm_data_debug_signals_o[0][447:384];
  assign new_sm_data_h0_chunk7_o = new_sm_data_debug_signals_o[0][511:448];
  assign new_sm_data_h1_chunk0_o = new_sm_data_debug_signals_o[1][63:0];
  assign new_sm_data_h1_chunk1_o = new_sm_data_debug_signals_o[1][127:64];
  assign new_sm_data_h1_chunk2_o = new_sm_data_debug_signals_o[1][191:128];
  assign new_sm_data_h1_chunk3_o = new_sm_data_debug_signals_o[1][255:192];
  assign new_sm_data_h1_chunk4_o = new_sm_data_debug_signals_o[1][319:256];
  assign new_sm_data_h1_chunk5_o = new_sm_data_debug_signals_o[1][383:320];
  assign new_sm_data_h1_chunk6_o = new_sm_data_debug_signals_o[1][447:384];
  assign new_sm_data_h1_chunk7_o = new_sm_data_debug_signals_o[1][511:448];

  minion_dcache_miss_handler_unit u_new (
    .clk_i,
    .rst_ni,
    .neigh_min_id_i,
    .req_ready_o(new_req_ready_o),
    .req_valid_i,
    .req_info_i(req_info_new),
    .req_assigned_o(new_req_assigned_o),
    .req_accepted_o(new_req_accepted_o),
    .can_accept_uc_o(new_can_accept_uc_o),
    .wait_l2_to_ba_resp_o(new_wait_l2_to_ba_resp_o),
    .inflight_reduce_i,
    .going_to_uc_store_o(new_going_to_uc_store_o),
    .l2_evict_ready_i,
    .l2_evict_valid_o(new_l2_evict_valid_o),
    .l2_evict_req_o(l2_evict_req_new),
    .l2_miss_ready_i,
    .l2_miss_valid_o(new_l2_miss_valid_o),
    .l2_miss_req_o(l2_miss_req_new),
    .l2_resp_valid_i,
    .l2_resp_data_i(l2_resp_data_new),
    .l2_resp_slot_avail_i,
    .l2_resp_stall_o(new_l2_resp_stall_o),
    .refill_way_o(new_refill_way_o),
    .refill_addr_o(new_refill_addr_o),
    .meta_write_ready_i,
    .meta_write_valid_next_o(new_meta_write_valid_next_o),
    .meta_write_valid_o(new_meta_write_valid_o),
    .meta_write_clear_slock_o(new_meta_write_clear_slock_o),
    .meta_write_req_o(meta_write_req_new),
    .writeback_ready_i,
    .writeback_valid_o(new_writeback_valid_o),
    .writeback_req_o(writeback_req_new),
    .writeback_done_i,
    .force_ba_rptr_o(new_force_ba_rptr_o),
    .force_ba_rptr_qual_o(new_force_ba_rptr_qual_o),
    .ba_rptr_o(new_ba_rptr_o),
    .ba_is_amo_o(new_ba_is_amo_o),
    .ba_wptr_o(new_ba_wptr_o),
    .idle_o(new_idle_o),
    .probe_rdy_o(new_probe_rdy_o),
    .metaw_rdy_o(new_metaw_rdy_o),
    .fence_rdy_o(new_fence_rdy_o),
    .handler_ready_o(new_handler_ready_o),
    .handler_fill_bus_error_o(new_handler_fill_bus_error_o),
    .handler_uc_load_bus_error_o(new_handler_uc_load_bus_error_o),
    .match_miss_line_o(new_match_miss_line_o),
    .bus_err_o(new_bus_err_o),
    .bus_err_addr_o(new_bus_err_addr_o),
    .csr_debug_signals_o(new_csr_debug_signals_o),
    .sm_match_debug_signals_o(new_sm_match_debug_signals_o),
    .sm_filter_debug_signals_o(new_sm_filter_debug_signals_o),
    .sm_data_debug_signals_o(new_sm_data_debug_signals_o)
  );

  dcache_miss_handler_unit u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .neigh_min_id(neigh_min_id_i),
    .req_ready(orig_req_ready_o),
    .req_valid(req_valid_i),
    .req_info(req_info_orig),
    .req_assigned(orig_req_assigned_o),
    .req_accepted(orig_req_accepted_o),
    .can_accept_uc(orig_can_accept_uc_o),
    .wait_l2_to_ba_resp(orig_wait_l2_to_ba_resp_o),
    .inflight_reduce(inflight_reduce_i),
    .going_to_uc_store(orig_going_to_uc_store_o),
    .l2_evict_ready(l2_evict_ready_i),
    .l2_evict_valid(orig_l2_evict_valid_o),
    .l2_evict_req(l2_evict_req_orig),
    .l2_miss_ready(l2_miss_ready_i),
    .l2_miss_valid(orig_l2_miss_valid_o),
    .l2_miss_req(l2_miss_req_orig),
    .l2_resp_valid(l2_resp_valid_i),
    .l2_resp_data(l2_resp_data_orig),
    .l2_resp_slot_avail(l2_resp_slot_avail_i),
    .l2_resp_stall(orig_l2_resp_stall_o),
    .refill_way(orig_refill_way_o),
    .refill_addr(orig_refill_addr_o),
    .meta_write_ready(meta_write_ready_i),
    .meta_write_valid_next(orig_meta_write_valid_next_o),
    .meta_write_valid(orig_meta_write_valid_o),
    .meta_write_clear_slock(orig_meta_write_clear_slock_o),
    .meta_write_req(meta_write_req_orig),
    .writeback_ready(writeback_ready_i),
    .writeback_valid(orig_writeback_valid_o),
    .writeback_req(writeback_req_orig),
    .writeback_done(writeback_done_i),
    .force_ba_rptr(orig_force_ba_rptr_o),
    .force_ba_rptr_qual(orig_force_ba_rptr_qual_o),
    .ba_rptr(orig_ba_rptr_o),
    .ba_is_amo(orig_ba_is_amo_o),
    .ba_wptr(orig_ba_wptr_o),
    .idle(orig_idle_o),
    .probe_rdy(orig_probe_rdy_o),
    .metaw_rdy(orig_metaw_rdy_o),
    .fence_rdy(orig_fence_rdy_o),
    .handler_ready(orig_handler_ready_o),
    .handler_fill_bus_error(orig_handler_fill_bus_error_o),
    .handler_uc_load_bus_error(orig_handler_uc_load_bus_error_o),
    .match_miss_line(orig_match_miss_line_o),
    .bus_err(orig_bus_err_o),
    .bus_err_addr(orig_bus_err_addr_o),
    .csr_debug_signals(orig_csr_debug_signals_o),
    .sm_match_debug_signals(orig_sm_match_debug_signals_o),
    .sm_filter_debug_signals(orig_sm_filter_debug_signals_o),
    .sm_data_debug_signals(orig_sm_data_debug_signals_o)
  );
endmodule : cosim_minion_dcache_miss_handler_unit_tb
