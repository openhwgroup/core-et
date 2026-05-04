// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_replay_queue.

module minion_dcache_replay_queue_tb
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
  output logic rq_conflict_o,
  output logic rq_conflict_to_co_o,
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
  output logic full_o,
  output logic half_full_o,
  output logic [NrThreads-1:0] empty_o,
  output logic replay_bid_o,
  input  logic replay_grant_i,
  output logic replay_valid_o,
  output logic [DcacheReplayqAddrWidth-1:0] replay_entry_o,
  output logic [DcacheReplayqAgeWidth-1:0] replay_age_o,
  output logic replay_uc_ok_o,
  output logic replay_miss_ok_o,
  output logic [4:0] replay_req_cmd_o,
  output logic [3:0] replay_req_typ_o,
  output logic replay_req_dest_fp_o,
  output logic [XregAddrSize-1:0] replay_req_dest_addr_o,
  output logic replay_req_dest_thread_o,
  output logic [VaSize:0] replay_req_addr_o,
  output logic replay_req_cacheable_o,
  output logic [DcacheReplayqSize-1:0] scoreboard_valid_o,
  output logic [NrThreads-1:0] fp_sb_dealloc_o,
  output logic [NrThreads-1:0] int_sb_dealloc_o
);
  /* verilator lint_off UNUSEDSIGNAL */  // The unit-test wrapper intentionally exposes only the replay-queue fields the directed test checks.

  dcache_req_t            req_data;
  dcache_replay_conf_t    req_conf_type;
  dcache_replay_req_t     replay_req;
  dcache_minion_scoreboard scoreboard;

  assign req_data = '{
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

  assign req_conf_type = '{
    vpu_rf_write: req_conf_vpu_rf_write_i,
    miss_alloc: req_conf_miss_alloc_i,
    miss_accepted: req_conf_miss_accepted_i,
    miss_assigned: req_conf_miss_assigned_i
  };

  assign replay_req_cmd_o = replay_req.cmd;
  assign replay_req_typ_o = replay_req.typ;
  assign replay_req_dest_fp_o = replay_req.dest.fp;
  assign replay_req_dest_addr_o = replay_req.dest.addr;
  assign replay_req_dest_thread_o = replay_req.dest.thread_id;
  assign replay_req_addr_o = replay_req.addr;
  assign replay_req_cacheable_o = replay_req.cacheable;
  assign scoreboard_valid_o = scoreboard.valid;

  minion_dcache_replay_queue u_dut (
    .clk_i,
    .rst_ni,
    .full_o,
    .half_full_o,
    .empty_o,
    .alloc_req_pre_i,
    .alloc_req_val_i,
    .alloc_gsc_i,
    .push_req_i,
    .push_req_prev_i,
    .req_data_i(req_data),
    .cacheable_i,
    .req_buf_alloc_i,
    .req_buf_entry_i(req_buf_id_i),
    .req_conf_type_i(req_conf_type),
    .misaligned_1st_i,
    .uc_load_1st_i,
    .uc_load_ok_i,
    .miss_fill_ok_i,
    .rq_conflict_o,
    .rq_conflict_to_co_o,
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
    .replay_bid_o,
    .replay_grant_i,
    .replay_valid_o,
    .replay_req_o(replay_req),
    .replay_entry_o,
    .replay_age_o,
    .replay_uc_ok_o,
    .replay_miss_ok_o,
    .scoreboard_o(scoreboard),
    .fp_sb_dealloc_o,
    .int_sb_dealloc_o
  );

  /* verilator lint_on UNUSEDSIGNAL */

endmodule : minion_dcache_replay_queue_tb
