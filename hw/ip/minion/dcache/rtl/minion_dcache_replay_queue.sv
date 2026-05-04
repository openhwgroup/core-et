// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache replay queue.
//
// Replaces: dcache_replay_queue

/* verilator lint_off UNUSEDSIGNAL */  // The replay queue intentionally consumes only the subset of the full dcache request bundle needed to preserve the original local behavior.
module minion_dcache_replay_queue
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                              clk_i,
  input  logic                              rst_ni,
  output logic                              full_o,
  output logic                              half_full_o,
  output logic [NrThreads-1:0]              empty_o,
  input  logic                              alloc_req_pre_i,
  input  logic                              alloc_req_val_i,
  input  logic                              alloc_gsc_i,
  input  logic                              push_req_i,
  input  logic                              push_req_prev_i,
  input  dcache_req_t                       req_data_i,
  input  logic                              cacheable_i,
  input  logic                              req_buf_alloc_i,
  input  logic [DcacheBufferIdxWidth-1:0]   req_buf_entry_i,
  input  dcache_replay_conf_t               req_conf_type_i,
  input  logic                              misaligned_1st_i,
  input  logic                              uc_load_1st_i,
  input  logic                              uc_load_ok_i,
  input  logic                              miss_fill_ok_i,
  output logic                              rq_conflict_o,
  output logic                              rq_conflict_to_co_o,
  input  logic                              rearm_i,
  input  logic                              rearm_prev_i,
  input  logic [DcacheReplayqAddrWidth-1:0] rearm_entry_i,
  input  logic                              dealloc_i,
  input  logic [DcacheReplayqAddrWidth-1:0] dealloc_entry_i,
  input  logic [DcacheMhFileSize-1:0]       miss_available_i,
  input  logic [DcacheMhFileSize-1:0]       miss_fill_error_i,
  input  logic                              uc_load_fill_i,
  input  logic [DcacheBufferIdxWidth-1:0]   uc_load_fill_id_i,
  input  logic                              uc_load_fill_ok_i,
  input  logic                              vpu_rf_in_use_i,
  output logic                              replay_bid_o,
  input  logic                              replay_grant_i,
  output logic                              replay_valid_o,
  output dcache_replay_req_t                replay_req_o,
  output logic [DcacheReplayqAddrWidth-1:0] replay_entry_o,
  output logic [DcacheReplayqAgeWidth-1:0]  replay_age_o,
  output logic                              replay_uc_ok_o,
  output logic                              replay_miss_ok_o,
  output dcache_minion_scoreboard           scoreboard_o,
  output logic [NrThreads-1:0]              fp_sb_dealloc_o,
  output logic [NrThreads-1:0]              int_sb_dealloc_o
);
  localparam logic [$clog2(minion_dcache_pkg::DcacheReplayqSize):0] HalfFullThreshold = 'd4;

  typedef struct packed {
    logic                              pending;
    logic                              store;
    logic [DcacheReplayqAgeWidth-1:0]  age;
    dcache_replay_conf_t               reason;
    logic                              wait_uc_load;
    logic                              uc_load_ok;
    logic                              miss_fill_ok;
  } replayq_ready_info_t;

  localparam int unsigned ReplayPushWidth = $bits(dcache_replay_req_push_t);
  localparam int unsigned ReplayRearmWidth = $bits(dcache_replay_req_rearm_t);

  logic                                s0_alloc_req_pre_q;
  logic                                s1_alloc_req_q;
  logic                                s2_alloc_req_q;
  logic                                s3_alloc_req_q;
  logic                                s0_alloc_gsc_q;
  logic                                s1_alloc_gsc_q;
  logic                                s2_alloc_gsc_q;
  logic                                s0_alloc_req;
  logic                                alloc_clk_en;

  logic [$clog2(minion_dcache_pkg::DcacheReplayqSize):0]  entry_count_available_q;
  logic [$clog2(minion_dcache_pkg::DcacheReplayqSize):0]  entry_count_available_d;
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0]        entry_mask_available_q;
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0]        entry_mask_available_d;
  logic                                alloc_req_pre_applied_q;
  logic                                alloc_req_pre_applied_d;
  logic [DcacheReplayqAgeMaskSize-1:0] age_id_mask_available_q;
  logic [DcacheReplayqAgeMaskSize-1:0] age_id_mask_available_d;
  logic [DcacheReplayqAddrWidth-1:0]   push_entry;
  logic [DcacheReplayqAgeWidth-1:0]    push_age_q;
  logic [DcacheReplayqAgeWidth-1:0]    push_age_d;
  logic [DcacheReplayqAgeCmpWidth-1:0] push_age_check_available;

  dcache_replay_req_push_t             replayq_push_q [minion_dcache_pkg::DcacheReplayqSize];
  dcache_replay_req_rearm_t            replayq_rearm_q [minion_dcache_pkg::DcacheReplayqSize];
  dcache_replay_req_t                  replayq         [minion_dcache_pkg::DcacheReplayqSize];
  dcache_replay_req_push_t             req_written_push;
  dcache_replay_req_rearm_t            req_written_rearm;
  logic [ReplayPushWidth*minion_dcache_pkg::DcacheReplayqSize-1:0] replayq_push_flat_q;
  logic [ReplayRearmWidth*minion_dcache_pkg::DcacheReplayqSize-1:0] replayq_rearm_flat_q;

  replayq_ready_info_t                 replayq_ready_info_q [minion_dcache_pkg::DcacheReplayqSize];
  replayq_ready_info_t                 replayq_ready_info_d [minion_dcache_pkg::DcacheReplayqSize];
  logic [DcacheReplayqAddrWidth-1:0]   uc_load_id_q [DcacheBufferSize];
  logic [DcacheReplayqAddrWidth-1:0]   uc_load_id_d [DcacheBufferSize];
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0][minion_dcache_pkg::DcacheReplayqSize-1:0] rq_conflict_matrix_q;
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0][minion_dcache_pkg::DcacheReplayqSize-1:0] rq_conflict_matrix_d;
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0]        entry_ready;
  logic [DcacheReplayqAddrWidth-1:0]   replay_entry_d;
  logic [DcacheReplayqAddrWidth-1:0]   replay_entry_prev_q;
  logic [NrThreads-1:0]                empty_d;
  logic                                replay_valid_prev_q;
  logic                                req_is_write;
  logic                                arb_clk_en;
  logic                                push_req_spec_q;

  logic [minion_dcache_pkg::DcacheReplayqSize-1:0]        rq_to_s2_conflict_array;
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0]        s2_to_rq_conflict_array;
  logic [NrThreads-1:0]                fp_sb_dealloc_d;
  logic [NrThreads-1:0]                int_sb_dealloc_d;

  logic [minion_dcache_pkg::DcacheReplayqSize-1:0][minion_dcache_pkg::DcacheReplayqSize-1:0] prio_bits_q;
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0][minion_dcache_pkg::DcacheReplayqSize-1:0] prio_bits_d;
  logic [minion_dcache_pkg::DcacheReplayqSize-1:0]        win_per_client;

  prim_rf_1r1w_par_preview #(
    .Width (ReplayPushWidth),
    .Depth (minion_dcache_pkg::DcacheReplayqSize)
  ) u_replayq_push (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (push_req_prev_i),
    .rd_data_o            (replayq_push_flat_q),
    .wr_data_i            (req_written_push),
    .wr_addr_i            (push_entry),
    .wr_en_i              (push_req_i)
  );

  prim_rf_1r1w_par_preview #(
    .Width (ReplayRearmWidth),
    .Depth (minion_dcache_pkg::DcacheReplayqSize)
  ) u_replayq_rearm (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (push_req_prev_i || rearm_prev_i),
    .rd_data_o            (replayq_rearm_flat_q),
    .wr_data_i            (req_written_rearm),
    .wr_addr_i            (push_entry),
    .wr_en_i              (push_req_i || rearm_i)
  );

  assign s0_alloc_req = s0_alloc_req_pre_q && alloc_req_val_i;

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s0_alloc_req_pre_q <= 1'b0;
      s1_alloc_req_q <= 1'b0;
      s2_alloc_req_q <= 1'b0;
      s3_alloc_req_q <= 1'b0;
      s0_alloc_gsc_q <= 1'b0;
      s1_alloc_gsc_q <= 1'b0;
      s2_alloc_gsc_q <= 1'b0;
      entry_count_available_q <= minion_dcache_pkg::DcacheReplayqSize[$clog2(minion_dcache_pkg::DcacheReplayqSize):0];
      entry_mask_available_q <= '1;
      age_id_mask_available_q <= '1;
      alloc_req_pre_applied_q <= 1'b0;
    end else if (alloc_clk_en) begin
      s0_alloc_req_pre_q <= alloc_req_pre_i;
      s1_alloc_req_q <= s0_alloc_req;
      s2_alloc_req_q <= s1_alloc_req_q;
      s3_alloc_req_q <= s2_alloc_req_q && s2_alloc_gsc_q;
      s0_alloc_gsc_q <= alloc_gsc_i;
      s1_alloc_gsc_q <= s0_alloc_gsc_q;
      s2_alloc_gsc_q <= s1_alloc_gsc_q;
      entry_count_available_q <= entry_count_available_d;
      entry_mask_available_q <= entry_mask_available_d;
      age_id_mask_available_q <= age_id_mask_available_d;
      alloc_req_pre_applied_q <= alloc_req_pre_applied_d;
    end
  end

  always_comb begin
    push_age_check_available = push_age_q[DcacheReplayqAgeCmpWidth-1:0]
        + s0_alloc_req + s1_alloc_req_q + s2_alloc_req_q + s3_alloc_req_q;

    push_entry = '0;
    for (int unsigned i = 1; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      if (entry_mask_available_q[i]) begin
        push_entry = DcacheReplayqAddrWidth'(i);
      end
    end
    if (req_data_i.replay) begin
      push_entry = rearm_entry_i;
    end

    full_o = (entry_count_available_q == '0)
          || !age_id_mask_available_q[push_age_check_available];
    half_full_o = (entry_count_available_q <= HalfFullThreshold)
               || !age_id_mask_available_q[push_age_check_available];

    entry_count_available_d = entry_count_available_q;
    alloc_req_pre_applied_d = 1'b0;
    if (alloc_req_pre_i && (entry_count_available_d != '0)) begin
      alloc_req_pre_applied_d = 1'b1;
      entry_count_available_d = entry_count_available_d - 1'b1;
    end
    if (alloc_req_pre_applied_q && !s0_alloc_req) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if ((s2_alloc_req_q && !s2_alloc_gsc_q) && !push_req_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if (s3_alloc_req_q && !push_req_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if ((s2_alloc_req_q && !s2_alloc_gsc_q) && s3_alloc_req_q && push_req_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if (dealloc_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end

    entry_mask_available_d = entry_mask_available_q;
    if (push_req_i) begin
      entry_mask_available_d[push_entry] = 1'b0;
    end
    if (dealloc_i) begin
      entry_mask_available_d[dealloc_entry_i] = 1'b1;
    end

    age_id_mask_available_d = age_id_mask_available_q;
    if (push_req_i) begin
      age_id_mask_available_d[push_age_q[DcacheReplayqAgeCmpWidth-1:0]] = 1'b0;
    end
    if (dealloc_i) begin
      age_id_mask_available_d[req_data_i.replay_age[DcacheReplayqAgeCmpWidth-1:0]] = 1'b1;
    end

    alloc_clk_en = !rst_ni || alloc_req_pre_i || s0_alloc_req_pre_q || s1_alloc_req_q
                || s2_alloc_req_q || s3_alloc_req_q || dealloc_i || push_req_i;
  end

  always_comb begin
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      replayq_push_q[i] = replayq_push_flat_q[ReplayPushWidth*i +: ReplayPushWidth];
      replayq_rearm_q[i] = replayq_rearm_flat_q[ReplayRearmWidth*i +: ReplayRearmWidth];
      replayq[i].dest = replayq_push_q[i].dest;
      replayq[i].cmd = replayq_push_q[i].cmd;
      replayq[i].typ = replayq_push_q[i].typ;
      replayq[i].gsc_cnt = replayq_push_q[i].gsc_cnt;
      replayq[i].ps_mask = replayq_push_q[i].ps_mask;
      replayq[i].buf_alloc = replayq_push_q[i].buf_alloc;
      replayq[i].buf_id = replayq_push_q[i].buf_id;
      replayq[i].cache_op = replayq_push_q[i].cache_op;
      replayq[i].gsc32_idx = replayq_push_q[i].gsc32_idx;
      replayq[i].vm_sel = replayq_push_q[i].vm_sel;
      replayq[i].mem_global = replayq_push_q[i].mem_global;
      replayq[i].addr = replayq_rearm_q[i].addr;
      replayq[i].misaligned = replayq_rearm_q[i].misaligned;
      replayq[i].misaligned_2nd = replayq_rearm_q[i].misaligned_2nd;
      replayq[i].uc_load_2nd = replayq_rearm_q[i].uc_load_2nd;
      replayq[i].phys = replayq_rearm_q[i].phys;
      replayq[i].cacheable = replayq_rearm_q[i].cacheable;
    end
  end

  always_comb begin
    req_written_push = '0;
    req_written_push.dest = req_data_i.dest;
    req_written_push.cmd = req_data_i.cmd;
    req_written_push.typ = req_data_i.typ;
    req_written_push.gsc_cnt = req_data_i.gsc_cnt;
    req_written_push.ps_mask = req_data_i.ps_mask;
    req_written_push.buf_alloc = req_buf_alloc_i;
    req_written_push.buf_id = req_buf_entry_i;
    req_written_push.cache_op = req_data_i.cache_op;
    req_written_push.gsc32_idx = req_data_i.gsc32_idx;
    req_written_push.vm_sel = req_data_i.vm_sel;
    req_written_push.mem_global = req_data_i.mem_global;

    req_written_rearm = '0;
    req_written_rearm.addr = req_data_i.addr;
    req_written_rearm.misaligned = req_data_i.misaligned;
    req_written_rearm.misaligned_2nd = req_data_i.misaligned_2nd;
    req_written_rearm.uc_load_2nd = req_data_i.uc_load_2nd;
    req_written_rearm.phys = req_data_i.phys;
    req_written_rearm.cacheable = cacheable_i;

    if (misaligned_1st_i) begin
      req_written_rearm.misaligned = 1'b0;
      req_written_rearm.misaligned_2nd = 1'b1;
      req_written_rearm.addr[minion_dcache_pkg::VaSize:5] = req_data_i.addr[minion_dcache_pkg::VaSize:5] + 1'b1;
    end
    if (uc_load_1st_i) begin
      req_written_rearm.uc_load_2nd = 1'b1;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      replay_valid_o <= 1'b0;
      replay_valid_prev_q <= 1'b0;
      push_age_q <= '0;
      empty_o <= '1;
    end else if (arb_clk_en) begin
      replay_valid_o <= replay_grant_i;
      replay_valid_prev_q <= replay_valid_o;
      push_age_q <= push_age_d;
      empty_o <= empty_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (arb_clk_en) begin
      replayq_ready_info_q <= replayq_ready_info_d;
      replay_entry_o <= replay_entry_d;
      replay_entry_prev_q <= replay_entry_o;
      uc_load_id_q <= uc_load_id_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      push_req_spec_q <= 1'b0;
      prio_bits_q <= '0;
      fp_sb_dealloc_o <= '0;
      int_sb_dealloc_o <= '0;
    end else begin
      push_req_spec_q <= push_req_prev_i;
      if (push_req_i) begin
        prio_bits_q <= prio_bits_d;
      end
      fp_sb_dealloc_o <= fp_sb_dealloc_d;
      int_sb_dealloc_o <= int_sb_dealloc_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (push_req_i || rearm_i || replay_grant_i) begin
      rq_conflict_matrix_q <= rq_conflict_matrix_d;
    end
  end

  always_comb begin
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      entry_ready[i] = replayq_ready_info_q[i].pending && !entry_mask_available_q[i]
                    && !(|rq_conflict_matrix_q[i])
                    && !replayq_ready_info_q[i].wait_uc_load
                    && !replayq_ready_info_q[i].reason.miss_alloc
                    && (&(~replayq_ready_info_q[i].reason.miss_assigned))
                    && !replayq_ready_info_q[i].reason.vpu_rf_write;
    end

    for (int unsigned i = 0; i < NrThreads; i++) begin
      empty_d[i] = 1'b1;
      for (int unsigned j = 0; j < minion_dcache_pkg::DcacheReplayqSize; j++) begin
        empty_d[i] &= entry_mask_available_q[j] || (replayq[j].dest.thread_id != i[0]);
      end
      empty_d[i] &= !(push_req_i && (req_data_i.dest.thread_id == i[0]));
    end

    replayq_ready_info_d = replayq_ready_info_q;
    push_age_d = push_age_q;

    for (int unsigned i = 0; i < DcacheMhFileSize; i++) begin
      if (miss_available_i[i]) begin
        for (int unsigned j = 0; j < minion_dcache_pkg::DcacheReplayqSize; j++) begin
          if (replayq_ready_info_d[j].reason.miss_assigned[i]
              || (replayq_ready_info_d[j].reason.miss_assigned == '0)) begin
            replayq_ready_info_d[j].reason.miss_alloc = 1'b0;
          end
          replayq_ready_info_d[j].reason.miss_assigned[i] = 1'b0;
        end
      end
    end

    for (int unsigned i = 0; i < DcacheMhFileSize; i++) begin
      if (miss_fill_error_i[i]) begin
        for (int unsigned j = 0; j < minion_dcache_pkg::DcacheReplayqSize; j++) begin
          if (replayq_ready_info_d[j].reason.miss_accepted[i]) begin
            replayq_ready_info_d[j].miss_fill_ok = 1'b0;
          end
        end
      end
    end

    if (!vpu_rf_in_use_i) begin
      for (int unsigned j = 0; j < minion_dcache_pkg::DcacheReplayqSize; j++) begin
        replayq_ready_info_d[j].reason.vpu_rf_write = 1'b0;
      end
    end

    if (uc_load_fill_i) begin
      replayq_ready_info_d[uc_load_id_q[uc_load_fill_id_i]].wait_uc_load = 1'b0;
      replayq_ready_info_d[uc_load_id_q[uc_load_fill_id_i]].uc_load_ok = uc_load_fill_ok_i;
    end

    req_is_write = minion_dcache_pkg::dcache_cmd_is_write(req_data_i.cmd);

    if (push_req_i) begin
      replayq_ready_info_d[push_entry].store = req_is_write;
      replayq_ready_info_d[push_entry].age = push_age_q;
      push_age_d = push_age_q + 1'b1;
    end

    if (push_req_i || rearm_i) begin
      replayq_ready_info_d[push_entry].pending = 1'b1;
      replayq_ready_info_d[push_entry].reason = req_conf_type_i;
      replayq_ready_info_d[push_entry].wait_uc_load = uc_load_1st_i;
      replayq_ready_info_d[push_entry].uc_load_ok = uc_load_ok_i;
      replayq_ready_info_d[push_entry].miss_fill_ok = miss_fill_ok_i;
    end

    if (replay_grant_i) begin
      replayq_ready_info_d[replay_entry_d].pending = 1'b0;
    end

    uc_load_id_d = uc_load_id_q;
    if (push_req_i && req_buf_alloc_i) begin
      uc_load_id_d[req_buf_entry_i] = push_entry;
    end

    arb_clk_en = !rst_ni || !(&empty_o) || push_req_spec_q || replay_valid_o || replay_valid_prev_q;
  end

  always_comb begin
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      win_per_client[i] = entry_ready[i];
      for (int unsigned j = 0; j < i; j++) begin
        win_per_client[i] = win_per_client[i] && (~entry_ready[j] || prio_bits_q[i][j]);
      end
      for (int unsigned j = i + 1; j < minion_dcache_pkg::DcacheReplayqSize; j++) begin
        win_per_client[i] = win_per_client[i] && (~entry_ready[j] || ~prio_bits_q[j][i]);
      end
    end

    replay_entry_d = '0;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      if (win_per_client[i]) begin
        replay_entry_d = DcacheReplayqAddrWidth'(i);
      end
    end

    prio_bits_d = prio_bits_q;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      if (push_req_i && (push_entry == DcacheReplayqAddrWidth'(i))) begin
        for (int unsigned j = 0; j < i; j++) begin
          prio_bits_d[i][j] = 1'b0;
        end
        for (int unsigned j = i + 1; j < minion_dcache_pkg::DcacheReplayqSize; j++) begin
          prio_bits_d[j][i] = 1'b1;
        end
      end
    end
  end

  always_comb begin
    replay_bid_o = |entry_ready;
    replay_req_o = replayq[replay_entry_o];
    replay_age_o = replayq_ready_info_q[replay_entry_o].age;
    replay_uc_ok_o = replayq_ready_info_q[replay_entry_o].uc_load_ok;
    replay_miss_ok_o = replayq_ready_info_q[replay_entry_o].miss_fill_ok;
  end

  always_comb begin
    rq_to_s2_conflict_array = '0;
    s2_to_rq_conflict_array = '0;
    rq_conflict_o = 1'b0;
    rq_conflict_to_co_o = 1'b0;

    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      logic rq_entry_is_older;
      logic rq_entry_pending;

      rq_entry_is_older = !req_data_i.replay
                       || dcache_replayq_entry_is_older(replayq_ready_info_q[i].age, req_data_i.replay_age);
      rq_entry_pending = !entry_mask_available_q[i] && replayq_ready_info_q[i].pending;

      if (rq_entry_pending
          || (replay_valid_o && (replay_entry_o == DcacheReplayqAddrWidth'(i)))
          || (replay_valid_prev_q && (replay_entry_prev_q == DcacheReplayqAddrWidth'(i)))) begin
        if (req_is_write || replayq_ready_info_q[i].store) begin
          if ((req_data_i.addr[DcacheReplayqAddrConflictLsb +: DcacheReplayqAddrConflictWidth]
                == replayq[i].addr[DcacheReplayqAddrConflictLsb +: DcacheReplayqAddrConflictWidth])
              || req_data_i.misaligned || replayq[i].misaligned
              || replayq[i].misaligned_2nd || !replayq[i].cacheable) begin
            if (rq_entry_is_older) begin
              rq_to_s2_conflict_array[i] = rq_entry_pending;
              rq_conflict_o = 1'b1;
            end else begin
              s2_to_rq_conflict_array[i] = 1'b1;
            end
          end
        end
        rq_conflict_to_co_o |= replayq[i].misaligned_2nd;
      end
    end
  end

  always_comb begin
    rq_conflict_matrix_d = rq_conflict_matrix_q;
    if (push_req_i || rearm_i) begin
      rq_conflict_matrix_d[push_entry] = rq_to_s2_conflict_array;
      for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
        rq_conflict_matrix_d[i][push_entry] = s2_to_rq_conflict_array[i];
      end
    end
    if (replay_grant_i) begin
      for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
        rq_conflict_matrix_d[i][replay_entry_d] = 1'b0;
      end
    end
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      rq_conflict_matrix_d[i][i] = 1'b0;
    end
  end

  always_comb begin
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++) begin
      scoreboard_o.valid[i] = !entry_mask_available_q[i]
                           && minion_dcache_pkg::dcache_cmd_is_read(replayq[i].cmd);
      scoreboard_o.dest[i] = replayq[i].dest;
    end
    for (int unsigned i = 0; i < NrThreads; i++) begin
      fp_sb_dealloc_d[i] = dealloc_i && replayq[dealloc_entry_i].dest.fp
                        && (replayq[dealloc_entry_i].dest.thread_id == i[0]);
      int_sb_dealloc_d[i] = dealloc_i && !replayq[dealloc_entry_i].dest.fp
                         && (replayq[dealloc_entry_i].dest.thread_id == i[0]);
    end
  end

endmodule : minion_dcache_replay_queue
/* verilator lint_on UNUSEDSIGNAL */
