// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_replay_queue vs new
// minion_dcache_replay_queue.

`include "soc.vh"

module cosim_minion_dcache_replay_queue_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  logic alloc_req_pre_i,
  input  logic alloc_req_val_i,
  input  logic alloc_gsc_i,
  input  logic push_req_i,
  input  logic push_req_prev_i,
  input  logic [VaSize:0] req_addr_i,
  input  logic req_phys_i,
  input  logic req_misaligned_i,
  input  logic req_misaligned_2nd_i,
  input  logic req_dest_fp_i,
  input  logic [XregAddrSize-1:0] req_dest_addr_i,
  input  logic req_dest_thread_i,
  input  logic [4:0] req_cmd_i,
  input  logic [3:0] req_typ_i,
  input  logic [CoreGscCntBits-1:0] req_gsc_cnt_i,
  input  logic [DmemReqPsMaskSz-1:0] req_ps_mask_i,
  input  logic req_replay_i,
  input  logic [DcacheReplayqAddrWidth-1:0] req_replay_entry_i,
  input  logic [DcacheReplayqAgeWidth-1:0] req_replay_age_i,
  input  logic req_cache_op_i,
  input  logic req_buf_alloc_i,
  input  logic [DcacheBufferIdxWidth-1:0] req_buf_id_i,
  input  logic req_cacheable_i,
  input  logic req_uc_load_2nd_i,
  input  logic req_uc_load_ok_i,
  input  logic req_miss_fill_ok_i,
  input  logic [CoreGsc32IdxVSize-1:0] req_gsc32_idx_i,
  input  logic [DcacheVmSelWidth-1:0] req_vm_sel_i,
  input  logic req_mem_global_i,
  input  logic cacheable_i,
  input  logic req_conf_vpu_rf_write_i,
  input  logic req_conf_miss_alloc_i,
  input  logic [DcacheMhFileSize-1:0] req_conf_miss_accepted_i,
  input  logic [DcacheMhFileSize-1:0] req_conf_miss_assigned_i,
  input  logic misaligned_1st_i,
  input  logic uc_load_1st_i,
  input  logic uc_load_ok_i,
  input  logic miss_fill_ok_i,
  input  logic rearm_i,
  input  logic rearm_prev_i,
  input  logic [DcacheReplayqAddrWidth-1:0] rearm_entry_i,
  input  logic dealloc_i,
  input  logic [DcacheReplayqAddrWidth-1:0] dealloc_entry_i,
  input  logic [DcacheMhFileSize-1:0] miss_available_i,
  input  logic [DcacheMhFileSize-1:0] miss_fill_error_i,
  input  logic uc_load_fill_i,
  input  logic [DcacheBufferIdxWidth-1:0] uc_load_fill_id_i,
  input  logic uc_load_fill_ok_i,
  input  logic vpu_rf_in_use_i,
  input  logic replay_grant_i,
  output logic orig_full_o,
  output logic new_full_o,
  output logic orig_half_full_o,
  output logic new_half_full_o,
  output logic [NrThreads-1:0] orig_empty_o,
  output logic [NrThreads-1:0] new_empty_o,
  output logic orig_rq_conflict_o,
  output logic new_rq_conflict_o,
  output logic orig_rq_conflict_to_co_o,
  output logic new_rq_conflict_to_co_o,
  output logic orig_replay_bid_o,
  output logic new_replay_bid_o,
  output logic orig_replay_valid_o,
  output logic new_replay_valid_o,
  output logic [DcacheReplayqAddrWidth-1:0] orig_replay_entry_o,
  output logic [DcacheReplayqAddrWidth-1:0] new_replay_entry_o,
  output logic [DcacheReplayqAgeWidth-1:0] orig_replay_age_o,
  output logic [DcacheReplayqAgeWidth-1:0] new_replay_age_o,
  output logic orig_replay_uc_ok_o,
  output logic new_replay_uc_ok_o,
  output logic orig_replay_miss_ok_o,
  output logic new_replay_miss_ok_o,
  output logic [$bits(dcache_replay_req_t)-1:0] orig_replay_req_o,
  output logic [$bits(dcache_replay_req_t)-1:0] new_replay_req_o,
  output logic [$bits(dcache_minion_scoreboard)-1:0] orig_scoreboard_o,
  output logic [$bits(dcache_minion_scoreboard)-1:0] new_scoreboard_o,
  output logic [NrThreads-1:0] orig_fp_sb_dealloc_o,
  output logic [NrThreads-1:0] new_fp_sb_dealloc_o,
  output logic [NrThreads-1:0] orig_int_sb_dealloc_o,
  output logic [NrThreads-1:0] new_int_sb_dealloc_o
);

  dcache_req_t              req_data_new;
  dcache_replay_conf_t      req_conf_new;
  dcache_replay_req_t       replay_req_new;
  dcache_minion_scoreboard  scoreboard_new;

  dcache_req                req_data_orig;
  dcache_replay_conf        req_conf_orig;
  dcache_replay_req         replay_req_orig;
  dcache_minion_scoreboard  scoreboard_orig;

  assign req_data_new = '{
    addr: req_addr_i,
    addr_msb_err: 1'b0,
    set: '0,
    phys: req_phys_i,
    misaligned: req_misaligned_i,
    misaligned_2nd: req_misaligned_2nd_i,
    dest: '{fp: req_dest_fp_i, addr: req_dest_addr_i, thread_id: req_dest_thread_i},
    cmd: dcache_cmd_e'(req_cmd_i),
    typ: dcache_type_e'(req_typ_i),
    gsc_cnt: req_gsc_cnt_i,
    ps_mask: req_ps_mask_i,
    data: '0,
    replay: req_replay_i,
    replay_entry: req_replay_entry_i,
    replay_age: req_replay_age_i,
    cache_op: req_cache_op_i,
    buf_alloc: req_buf_alloc_i,
    buf_id: req_buf_id_i,
    cacheable: req_cacheable_i,
    chunk_read: '0,
    uc_load_2nd: req_uc_load_2nd_i,
    uc_load_ok: req_uc_load_ok_i,
    miss_fill_ok: req_miss_fill_ok_i,
    gsc32_idx: req_gsc32_idx_i,
    vm_sel: req_vm_sel_i,
    mem_global: req_mem_global_i
  };

  assign req_data_orig = '{
    addr: req_addr_i,
    addr_msb_err: 1'b0,
    set: '0,
    phys: req_phys_i,
    misaligned: req_misaligned_i,
    misaligned_2nd: req_misaligned_2nd_i,
    dest: '{fp: req_dest_fp_i, addr: req_dest_addr_i, thread_id: req_dest_thread_i},
    cmd: dcache_cmd'(req_cmd_i),
    typ: dcache_type'(req_typ_i),
    gsc_cnt: req_gsc_cnt_i,
    ps_mask: req_ps_mask_i,
    data: '0,
    replay: req_replay_i,
    replay_entry: req_replay_entry_i,
    replay_age: req_replay_age_i,
    cache_op: req_cache_op_i,
    buf_alloc: req_buf_alloc_i,
    buf_id: req_buf_id_i,
    cacheable: req_cacheable_i,
    chunk_read: '0,
    uc_load_2nd: req_uc_load_2nd_i,
    uc_load_ok: req_uc_load_ok_i,
    miss_fill_ok: req_miss_fill_ok_i,
    gsc32_idx: req_gsc32_idx_i,
    vm_sel: req_vm_sel_i,
    mem_global: req_mem_global_i
  };

  assign req_conf_new = '{
    vpu_rf_write: req_conf_vpu_rf_write_i,
    miss_alloc: req_conf_miss_alloc_i,
    miss_accepted: req_conf_miss_accepted_i,
    miss_assigned: req_conf_miss_assigned_i
  };

  assign req_conf_orig = '{
    vpu_rf_write: req_conf_vpu_rf_write_i,
    miss_alloc: req_conf_miss_alloc_i,
    miss_accepted: req_conf_miss_accepted_i,
    miss_assigned: req_conf_miss_assigned_i
  };

  assign orig_replay_req_o = replay_req_orig;
  assign new_replay_req_o = replay_req_new;
  assign orig_scoreboard_o = scoreboard_orig;
  assign new_scoreboard_o = scoreboard_new;

  minion_dcache_replay_queue u_new (
    .clk_i,
    .rst_ni,
    .full_o(new_full_o),
    .half_full_o(new_half_full_o),
    .empty_o(new_empty_o),
    .alloc_req_pre_i,
    .alloc_req_val_i,
    .alloc_gsc_i,
    .push_req_i,
    .push_req_prev_i,
    .req_data_i(req_data_new),
    .cacheable_i,
    .req_buf_alloc_i,
    .req_buf_entry_i(req_buf_id_i),
    .req_conf_type_i(req_conf_new),
    .misaligned_1st_i,
    .uc_load_1st_i,
    .uc_load_ok_i,
    .miss_fill_ok_i,
    .rq_conflict_o(new_rq_conflict_o),
    .rq_conflict_to_co_o(new_rq_conflict_to_co_o),
    .rearm_i,
    .rearm_prev_i,
    .rearm_entry_i,
    .dealloc_i,
    .dealloc_entry_i,
    .miss_available_i,
    .miss_fill_error_i,
    .uc_load_fill_i,
    .uc_load_fill_id_i,
    .uc_load_fill_ok_i,
    .vpu_rf_in_use_i,
    .replay_bid_o(new_replay_bid_o),
    .replay_grant_i,
    .replay_valid_o(new_replay_valid_o),
    .replay_req_o(replay_req_new),
    .replay_entry_o(new_replay_entry_o),
    .replay_age_o(new_replay_age_o),
    .replay_uc_ok_o(new_replay_uc_ok_o),
    .replay_miss_ok_o(new_replay_miss_ok_o),
    .scoreboard_o(scoreboard_new),
    .fp_sb_dealloc_o(new_fp_sb_dealloc_o),
    .int_sb_dealloc_o(new_int_sb_dealloc_o)
  );

  dcache_replay_queue u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .full(orig_full_o),
    .half_full(orig_half_full_o),
    .empty(orig_empty_o),
    .alloc_req_pre(alloc_req_pre_i),
    .alloc_req_val(alloc_req_val_i),
    .alloc_gsc(alloc_gsc_i),
    .push_req(push_req_i),
    .push_req_prev(push_req_prev_i),
    .req(req_data_orig),
    .cacheable(cacheable_i),
    .req_buf_alloc(req_buf_alloc_i),
    .req_buf_entry(req_buf_id_i),
    .req_conf_type(req_conf_orig),
    .misaligned_1st(misaligned_1st_i),
    .uc_load_1st(uc_load_1st_i),
    .uc_load_ok(uc_load_ok_i),
    .miss_fill_ok(miss_fill_ok_i),
    .rq_conflict(orig_rq_conflict_o),
    .rq_conflict_to_co(orig_rq_conflict_to_co_o),
    .rearm(rearm_i),
    .rearm_prev(rearm_prev_i),
    .rearm_entry(rearm_entry_i),
    .dealloc(dealloc_i),
    .dealloc_entry(dealloc_entry_i),
    .miss_available(miss_available_i),
    .miss_fill_error(miss_fill_error_i),
    .uc_load_fill(uc_load_fill_i),
    .uc_load_fill_id(uc_load_fill_id_i),
    .uc_load_fill_ok(uc_load_fill_ok_i),
    .vpu_rf_in_use(vpu_rf_in_use_i),
    .replay_bid(orig_replay_bid_o),
    .replay_grant(replay_grant_i),
    .replay_valid(orig_replay_valid_o),
    .replay_req(replay_req_orig),
    .replay_entry(orig_replay_entry_o),
    .replay_age(orig_replay_age_o),
    .replay_uc_ok(orig_replay_uc_ok_o),
    .replay_miss_ok(orig_replay_miss_ok_o),
    .scoreboard(scoreboard_orig),
    .fp_sb_dealloc(orig_fp_sb_dealloc_o),
    .int_sb_dealloc(orig_int_sb_dealloc_o)
  );

endmodule : cosim_minion_dcache_replay_queue_tb
