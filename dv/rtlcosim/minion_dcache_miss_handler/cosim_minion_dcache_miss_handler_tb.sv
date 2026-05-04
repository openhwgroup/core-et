// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_miss_handler vs new
// minion_dcache_miss_handler.

`include "soc.vh"

module cosim_minion_dcache_miss_handler_tb
  import minion_pkg::*;
(
  input  logic                            clk_i,
  input  logic                            rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0] neigh_min_id_i,
  input  logic                            req_pri_val_i,
  input  logic                            req_sec_val_i,
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
  input  logic                            inflight0_valid_i,
  input  logic [minion_dcache_pkg::DcacheWays-1:0] inflight0_way_en_i,
  input  logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] inflight0_set_i,
  input  logic                            inflight1_valid_i,
  input  logic [minion_dcache_pkg::DcacheWays-1:0] inflight1_way_en_i,
  input  logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] inflight1_set_i,
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
  output logic                            orig_req_pri_rdy_o,
  output logic                            new_req_pri_rdy_o,
  output logic                            orig_req_sec_rdy_o,
  output logic                            new_req_sec_rdy_o,
  output logic                            orig_can_accept_uc_o,
  output logic                            new_can_accept_uc_o,
  output logic                            orig_tag_match_o,
  output logic                            new_tag_match_o,
  output logic                            orig_evict_match_o,
  output logic                            new_evict_match_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] orig_set_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] new_set_o,
  output logic                            orig_going_to_uc_store_o,
  output logic                            new_going_to_uc_store_o,
  output logic                            orig_idle_o,
  output logic                            new_idle_o,
  output logic                            orig_wait_l2_to_ba_resp_o,
  output logic                            new_wait_l2_to_ba_resp_o,
  output logic                            orig_have_l2_to_ba_resp_o,
  output logic                            new_have_l2_to_ba_resp_o,
  output logic                            orig_l2_evict_valid_o,
  output logic                            new_l2_evict_valid_o,
  output logic [$bits(minion_dcache_pkg::et_link_minion_evict_req_info_t)-1:0] orig_l2_evict_req_o,
  output logic [$bits(minion_dcache_pkg::et_link_minion_evict_req_info_t)-1:0] new_l2_evict_req_o,
  output logic [4:0]                      orig_l2_evict_id_o,
  output logic [4:0]                      new_l2_evict_id_o,
  output logic                            orig_l2_evict_source_o,
  output logic                            new_l2_evict_source_o,
  output logic                            orig_l2_evict_wdata_o,
  output logic                            new_l2_evict_wdata_o,
  output logic [4:0]                      orig_l2_evict_opcode_o,
  output logic [4:0]                      new_l2_evict_opcode_o,
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
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] orig_l2_evict_subopcode_o,
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] new_l2_evict_subopcode_o,
  output logic                            orig_l2_miss_valid_o,
  output logic                            new_l2_miss_valid_o,
  output logic [$bits(minion_dcache_pkg::et_link_minion_miss_req_info_t)-1:0] orig_l2_miss_req_o,
  output logic [$bits(minion_dcache_pkg::et_link_minion_miss_req_info_t)-1:0] new_l2_miss_req_o,
  output logic [4:0]                      orig_l2_miss_id_o,
  output logic [4:0]                      new_l2_miss_id_o,
  output logic                            orig_l2_miss_source_o,
  output logic                            new_l2_miss_source_o,
  output logic                            orig_l2_miss_wdata_o,
  output logic                            new_l2_miss_wdata_o,
  output logic [4:0]                      orig_l2_miss_opcode_o,
  output logic [4:0]                      new_l2_miss_opcode_o,
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
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] orig_l2_miss_subopcode_o,
  output logic [minion_dcache_pkg::EtLinkSubopcodeSize-1:0] new_l2_miss_subopcode_o,
  output logic                            orig_l2_resp_stall_o,
  output logic                            new_l2_resp_stall_o,
  output logic [minion_dcache_pkg::DcacheWayIdxWidth-1:0] orig_refill_way_o,
  output logic [minion_dcache_pkg::DcacheWayIdxWidth-1:0] new_refill_way_o,
  output logic [minion_dcache_pkg::DcacheLramAddrWidth-1:0] orig_refill_addr_o,
  output logic [minion_dcache_pkg::DcacheLramAddrWidth-1:0] new_refill_addr_o,
  output logic                            orig_force_ba_rptr_next_o,
  output logic                            new_force_ba_rptr_next_o,
  output logic                            orig_force_ba_rptr_qual_o,
  output logic                            new_force_ba_rptr_qual_o,
  output logic [minion_dcache_pkg::DcacheBufferIdxWidth-1:0] orig_ba_rptr_o,
  output logic [minion_dcache_pkg::DcacheBufferIdxWidth-1:0] new_ba_rptr_o,
  output logic                            orig_ba_is_amo_o,
  output logic                            new_ba_is_amo_o,
  output logic                            orig_meta_write_valid_next_o,
  output logic                            new_meta_write_valid_next_o,
  output logic                            orig_meta_write_valid_o,
  output logic                            new_meta_write_valid_o,
  output logic                            orig_meta_write_clear_slock_o,
  output logic                            new_meta_write_clear_slock_o,
  output logic [$bits(minion_dcache_pkg::dcache_meta_write_req_t)-1:0] orig_meta_write_req_o,
  output logic [$bits(minion_dcache_pkg::dcache_meta_write_req_t)-1:0] new_meta_write_req_o,
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
  output logic [$bits(minion_dcache_pkg::dcache_wb_req_t)-1:0] orig_writeback_req_o,
  output logic [$bits(minion_dcache_pkg::dcache_wb_req_t)-1:0] new_writeback_req_o,
  output logic [minion_dcache_pkg::PaTagExtSize-1:0] orig_writeback_tag_o,
  output logic [minion_dcache_pkg::PaTagExtSize-1:0] new_writeback_tag_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] orig_writeback_set_o,
  output logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] new_writeback_set_o,
  output logic [minion_dcache_pkg::DcacheWays-1:0] orig_writeback_way_en_o,
  output logic [minion_dcache_pkg::DcacheWays-1:0] new_writeback_way_en_o,
  output logic                            orig_writeback_is_tbox_o,
  output logic                            new_writeback_is_tbox_o,
  output logic                            orig_writeback_mem_idx_o,
  output logic                            new_writeback_mem_idx_o,
  output logic [minion_dcache_pkg::PaNoTagExtSize-1:0] orig_writeback_addr_low_o,
  output logic [minion_dcache_pkg::PaNoTagExtSize-1:0] new_writeback_addr_low_o,
  output logic [NrThreads-1:0]            orig_bus_err_o,
  output logic [NrThreads-1:0]            new_bus_err_o,
  output logic [minion_dcache_pkg::PaSize-1:0] orig_bus_err_addr_o,
  output logic [minion_dcache_pkg::PaSize-1:0] new_bus_err_addr_o,
  output logic                            orig_fill_bus_error_o,
  output logic                            new_fill_bus_error_o,
  output logic                            orig_uc_load_bus_error_o,
  output logic                            new_uc_load_bus_error_o,
  output logic                            orig_probe_rdy_o,
  output logic                            new_probe_rdy_o,
  output logic [NrThreads-1:0]            orig_fence_rdy_o,
  output logic [NrThreads-1:0]            new_fence_rdy_o,
  output logic                            orig_metaw_rdy_o,
  output logic                            new_metaw_rdy_o,
  output logic                            orig_ready_o,
  output logic                            new_ready_o,
  output logic [minion_dcache_pkg::DcacheDbgCsrMhSize-1:0] orig_csr_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheDbgCsrMhSize-1:0] new_csr_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] orig_sm_match_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] new_sm_match_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] orig_sm_filter_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] new_sm_filter_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] orig_sm_data_debug_signals_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] new_sm_data_debug_signals_o,
  output logic [63:0]                     orig_sm_filter_chunk0_o,
  output logic [63:0]                     orig_sm_filter_chunk1_o,
  output logic [63:0]                     orig_sm_filter_chunk2_o,
  output logic [63:0]                     orig_sm_filter_chunk3_o,
  output logic [63:0]                     new_sm_filter_chunk0_o,
  output logic [63:0]                     new_sm_filter_chunk1_o,
  output logic [63:0]                     new_sm_filter_chunk2_o,
  output logic [63:0]                     new_sm_filter_chunk3_o,
  output logic [63:0]                     orig_sm_data_chunk0_o,
  output logic [63:0]                     orig_sm_data_chunk1_o,
  output logic [63:0]                     orig_sm_data_chunk2_o,
  output logic [63:0]                     orig_sm_data_chunk3_o,
  output logic [63:0]                     orig_sm_data_chunk4_o,
  output logic [63:0]                     orig_sm_data_chunk5_o,
  output logic [63:0]                     orig_sm_data_chunk6_o,
  output logic [63:0]                     orig_sm_data_chunk7_o,
  output logic [63:0]                     new_sm_data_chunk0_o,
  output logic [63:0]                     new_sm_data_chunk1_o,
  output logic [63:0]                     new_sm_data_chunk2_o,
  output logic [63:0]                     new_sm_data_chunk3_o,
  output logic [63:0]                     new_sm_data_chunk4_o,
  output logic [63:0]                     new_sm_data_chunk5_o,
  output logic [63:0]                     new_sm_data_chunk6_o,
  output logic [63:0]                     new_sm_data_chunk7_o
);
  minion_dcache_pkg::dcache_mh_req_t req_info_new;
  dcache_mh_req req_info_orig;
  minion_dcache_pkg::dcache_miss_info_t inflight_misses_new [minion_dcache_pkg::DcacheMhFileSize];
  dcache_miss_info [`DCACHE_MH_FILE_SIZE-1:0] inflight_misses_orig;
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

    inflight_misses_new[0].valid = inflight0_valid_i;
    inflight_misses_new[0].way_en = inflight0_way_en_i;
    inflight_misses_new[0].set = inflight0_set_i;
    inflight_misses_new[1].valid = inflight1_valid_i;
    inflight_misses_new[1].way_en = inflight1_way_en_i;
    inflight_misses_new[1].set = inflight1_set_i;

    inflight_misses_orig[0].valid = inflight0_valid_i;
    inflight_misses_orig[0].way_en = inflight0_way_en_i;
    inflight_misses_orig[0].set = inflight0_set_i;
    inflight_misses_orig[1].valid = inflight1_valid_i;
    inflight_misses_orig[1].way_en = inflight1_way_en_i;
    inflight_misses_orig[1].set = inflight1_set_i;

    l2_resp_data_new = '0;
    l2_resp_data_new.id = l2_resp_id_i;
    l2_resp_data_new.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);
    l2_resp_data_orig = '0;
    l2_resp_data_orig.id = l2_resp_id_i;
    l2_resp_data_orig.opcode = et_link_rsp_opcode_t'(l2_resp_opcode_i);

    orig_l2_evict_req_o = l2_evict_req_orig;
    new_l2_evict_req_o = l2_evict_req_new;
    orig_l2_miss_req_o = l2_miss_req_orig;
    new_l2_miss_req_o = l2_miss_req_new;
    orig_meta_write_req_o = meta_write_req_orig;
    new_meta_write_req_o = meta_write_req_new;
    orig_writeback_req_o = writeback_req_orig;
    new_writeback_req_o = writeback_req_new;
    orig_l2_evict_id_o = l2_evict_req_orig.id;
    new_l2_evict_id_o = l2_evict_req_new.id;
    orig_l2_evict_source_o = l2_evict_req_orig.source;
    new_l2_evict_source_o = l2_evict_req_new.source;
    orig_l2_evict_wdata_o = l2_evict_req_orig.wdata;
    new_l2_evict_wdata_o = l2_evict_req_new.wdata;
    orig_l2_evict_opcode_o = l2_evict_req_orig.opcode;
    new_l2_evict_opcode_o = l2_evict_req_new.opcode;
    orig_l2_evict_addr_o = l2_evict_req_orig.address;
    new_l2_evict_addr_o = l2_evict_req_new.address;
    orig_l2_evict_data0_o = l2_evict_req_orig.data[63:0];
    orig_l2_evict_data1_o = l2_evict_req_orig.data[127:64];
    orig_l2_evict_data2_o = l2_evict_req_orig.data[191:128];
    orig_l2_evict_data3_o = l2_evict_req_orig.data[255:192];
    new_l2_evict_data0_o = l2_evict_req_new.data[63:0];
    new_l2_evict_data1_o = l2_evict_req_new.data[127:64];
    new_l2_evict_data2_o = l2_evict_req_new.data[191:128];
    new_l2_evict_data3_o = l2_evict_req_new.data[255:192];
    orig_l2_evict_size_o = l2_evict_req_orig.size;
    new_l2_evict_size_o = l2_evict_req_new.size;
    orig_l2_evict_qwen_o = l2_evict_req_orig.qwen;
    new_l2_evict_qwen_o = l2_evict_req_new.qwen;
    orig_l2_evict_subopcode_o = l2_evict_req_orig.subopcode;
    new_l2_evict_subopcode_o = l2_evict_req_new.subopcode;
    orig_l2_miss_id_o = l2_miss_req_orig.id;
    new_l2_miss_id_o = l2_miss_req_new.id;
    orig_l2_miss_source_o = l2_miss_req_orig.source;
    new_l2_miss_source_o = l2_miss_req_new.source;
    orig_l2_miss_wdata_o = l2_miss_req_orig.wdata;
    new_l2_miss_wdata_o = l2_miss_req_new.wdata;
    orig_l2_miss_opcode_o = l2_miss_req_orig.opcode;
    new_l2_miss_opcode_o = l2_miss_req_new.opcode;
    orig_l2_miss_addr_o = l2_miss_req_orig.address;
    new_l2_miss_addr_o = l2_miss_req_new.address;
    orig_l2_miss_data0_o = l2_miss_req_orig.data;
    orig_l2_miss_data1_o = '0;
    orig_l2_miss_data2_o = '0;
    orig_l2_miss_data3_o = '0;
    new_l2_miss_data0_o = l2_miss_req_new.data;
    new_l2_miss_data1_o = '0;
    new_l2_miss_data2_o = '0;
    new_l2_miss_data3_o = '0;
    orig_l2_miss_size_o = l2_miss_req_orig.size;
    new_l2_miss_size_o = l2_miss_req_new.size;
    orig_l2_miss_qwen_o = l2_miss_req_orig.qwen;
    new_l2_miss_qwen_o = l2_miss_req_new.qwen;
    orig_l2_miss_subopcode_o = l2_miss_req_orig.subopcode;
    new_l2_miss_subopcode_o = l2_miss_req_new.subopcode;
    orig_meta_write_set_o = meta_write_req_orig.set;
    new_meta_write_set_o = meta_write_req_new.set_idx;
    orig_meta_write_way_en_o = meta_write_req_orig.way_en;
    new_meta_write_way_en_o = meta_write_req_new.way_en;
    orig_meta_write_state_o = meta_write_req_orig.data.coh_state;
    new_meta_write_state_o = meta_write_req_new.data.coh_state;
    orig_meta_write_tag_o = meta_write_req_orig.data.tag;
    new_meta_write_tag_o = meta_write_req_new.data.tag;
    orig_writeback_tag_o = writeback_req_orig.tag;
    new_writeback_tag_o = writeback_req_new.tag;
    orig_writeback_set_o = writeback_req_orig.set;
    new_writeback_set_o = writeback_req_new.set;
    orig_writeback_way_en_o = writeback_req_orig.way_en;
    new_writeback_way_en_o = writeback_req_new.way_en;
    orig_writeback_is_tbox_o = writeback_req_orig.is_tbox;
    new_writeback_is_tbox_o = writeback_req_new.is_tbox;
    orig_writeback_mem_idx_o = writeback_req_orig.mem_idx;
    new_writeback_mem_idx_o = writeback_req_new.mem_idx;
    orig_writeback_addr_low_o = writeback_req_orig.addr_low;
    new_writeback_addr_low_o = writeback_req_new.addr_low;
    orig_sm_filter_chunk0_o = orig_sm_filter_debug_signals_o[63:0];
    orig_sm_filter_chunk1_o = orig_sm_filter_debug_signals_o[127:64];
    orig_sm_filter_chunk2_o = orig_sm_filter_debug_signals_o[191:128];
    orig_sm_filter_chunk3_o = {56'b0, orig_sm_filter_debug_signals_o[199:192]};
    new_sm_filter_chunk0_o = new_sm_filter_debug_signals_o[63:0];
    new_sm_filter_chunk1_o = new_sm_filter_debug_signals_o[127:64];
    new_sm_filter_chunk2_o = new_sm_filter_debug_signals_o[191:128];
    new_sm_filter_chunk3_o = {56'b0, new_sm_filter_debug_signals_o[199:192]};
    orig_sm_data_chunk0_o = orig_sm_data_debug_signals_o[63:0];
    orig_sm_data_chunk1_o = orig_sm_data_debug_signals_o[127:64];
    orig_sm_data_chunk2_o = orig_sm_data_debug_signals_o[191:128];
    orig_sm_data_chunk3_o = orig_sm_data_debug_signals_o[255:192];
    orig_sm_data_chunk4_o = orig_sm_data_debug_signals_o[319:256];
    orig_sm_data_chunk5_o = orig_sm_data_debug_signals_o[383:320];
    orig_sm_data_chunk6_o = orig_sm_data_debug_signals_o[447:384];
    orig_sm_data_chunk7_o = orig_sm_data_debug_signals_o[511:448];
    new_sm_data_chunk0_o = new_sm_data_debug_signals_o[63:0];
    new_sm_data_chunk1_o = new_sm_data_debug_signals_o[127:64];
    new_sm_data_chunk2_o = new_sm_data_debug_signals_o[191:128];
    new_sm_data_chunk3_o = new_sm_data_debug_signals_o[255:192];
    new_sm_data_chunk4_o = new_sm_data_debug_signals_o[319:256];
    new_sm_data_chunk5_o = new_sm_data_debug_signals_o[383:320];
    new_sm_data_chunk6_o = new_sm_data_debug_signals_o[447:384];
    new_sm_data_chunk7_o = new_sm_data_debug_signals_o[511:448];
  end

  minion_dcache_miss_handler u_new (
    .clk_i,
    .rst_ni,
    .neigh_min_id_i,
    .req_pri_val_i,
    .req_pri_rdy_o(new_req_pri_rdy_o),
    .req_sec_val_i,
    .req_sec_rdy_o(new_req_sec_rdy_o),
    .can_accept_uc_o(new_can_accept_uc_o),
    .req_info_i(req_info_new),
    .tag_match_o(new_tag_match_o),
    .evict_match_o(new_evict_match_o),
    .set_o(new_set_o),
    .inflight_misses_i(inflight_misses_new),
    .inflight_reduce_i,
    .going_to_uc_store_o(new_going_to_uc_store_o),
    .idle_o(new_idle_o),
    .wait_l2_to_ba_resp_o(new_wait_l2_to_ba_resp_o),
    .have_l2_to_ba_resp_o(new_have_l2_to_ba_resp_o),
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
    .force_ba_rptr_next_o(new_force_ba_rptr_next_o),
    .force_ba_rptr_qual_o(new_force_ba_rptr_qual_o),
    .ba_rptr_o(new_ba_rptr_o),
    .ba_is_amo_o(new_ba_is_amo_o),
    .meta_write_ready_i,
    .meta_write_valid_next_o(new_meta_write_valid_next_o),
    .meta_write_valid_o(new_meta_write_valid_o),
    .meta_write_clear_slock_o(new_meta_write_clear_slock_o),
    .meta_write_req_o(meta_write_req_new),
    .writeback_ready_i,
    .writeback_valid_o(new_writeback_valid_o),
    .writeback_req_o(writeback_req_new),
    .writeback_done_i,
    .bus_err_o(new_bus_err_o),
    .bus_err_addr_o(new_bus_err_addr_o),
    .fill_bus_error_o(new_fill_bus_error_o),
    .uc_load_bus_error_o(new_uc_load_bus_error_o),
    .probe_rdy_o(new_probe_rdy_o),
    .fence_rdy_o(new_fence_rdy_o),
    .metaw_rdy_o(new_metaw_rdy_o),
    .ready_o(new_ready_o),
    .csr_debug_signals_o(new_csr_debug_signals_o),
    .sm_match_debug_signals_o(new_sm_match_debug_signals_o),
    .sm_filter_debug_signals_o(new_sm_filter_debug_signals_o),
    .sm_data_debug_signals_o(new_sm_data_debug_signals_o)
  );

  dcache_miss_handler u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .neigh_min_id(neigh_min_id_i),
    .req_pri_val(req_pri_val_i),
    .req_pri_rdy(orig_req_pri_rdy_o),
    .req_sec_val(req_sec_val_i),
    .req_sec_rdy(orig_req_sec_rdy_o),
    .can_accept_uc(orig_can_accept_uc_o),
    .req_info(req_info_orig),
    .tag_match(orig_tag_match_o),
    .evict_match(orig_evict_match_o),
    .set(orig_set_o),
    .inflight_misses(inflight_misses_orig),
    .inflight_reduce(inflight_reduce_i),
    .going_to_uc_store(orig_going_to_uc_store_o),
    .idle(orig_idle_o),
    .wait_l2_to_ba_resp(orig_wait_l2_to_ba_resp_o),
    .have_l2_to_ba_resp(orig_have_l2_to_ba_resp_o),
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
    .force_ba_rptr_next(orig_force_ba_rptr_next_o),
    .force_ba_rptr_qual(orig_force_ba_rptr_qual_o),
    .ba_rptr(orig_ba_rptr_o),
    .ba_is_amo(orig_ba_is_amo_o),
    .meta_write_ready(meta_write_ready_i),
    .meta_write_valid_next(orig_meta_write_valid_next_o),
    .meta_write_valid(orig_meta_write_valid_o),
    .meta_write_clear_slock(orig_meta_write_clear_slock_o),
    .meta_write_req(meta_write_req_orig),
    .writeback_ready(writeback_ready_i),
    .writeback_valid(orig_writeback_valid_o),
    .writeback_req(writeback_req_orig),
    .writeback_done(writeback_done_i),
    .bus_err(orig_bus_err_o),
    .bus_err_addr(orig_bus_err_addr_o),
    .fill_bus_error(orig_fill_bus_error_o),
    .uc_load_bus_error(orig_uc_load_bus_error_o),
    .probe_rdy(orig_probe_rdy_o),
    .fence_rdy(orig_fence_rdy_o),
    .metaw_rdy(orig_metaw_rdy_o),
    .ready(orig_ready_o),
    .csr_debug_signals(orig_csr_debug_signals_o),
    .sm_match_debug_signals(orig_sm_match_debug_signals_o),
    .sm_filter_debug_signals(orig_sm_filter_debug_signals_o),
    .sm_data_debug_signals(orig_sm_data_debug_signals_o)
  );

endmodule : cosim_minion_dcache_miss_handler_tb
