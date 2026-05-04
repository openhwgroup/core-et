// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Trace packet generation — looks for activity on traced reqq entries and
// creates trace packets.  Mechanical translation of shire_cache_bank_trace.v.
//
// Differences from original:
//   - Reset: active-low async-assert/sync-deassert (rst_ni) vs active-high sync
//   - Clock: clk_free_i (free-running) + prim_clk_gate vs fclock + et_clk_gate
//   - DFT: dft_pkg::dft_t dft_i added
//   - Types: shirecache_pkg trace types vs sc_trace_* defines
//   - Macros: RST_FF/EN_FF/RST_EN_FF → explicit always_ff
//   - Port suffixes: _i/_o

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
module shirecache_bank_trace
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                                clk_free_i,
  input  logic                                rst_ni,
  input  dft_t                                dft_i,

  // Status monitor interface
  output trace_packet_t                       bank_trace_data_o,
  output logic                                bank_trace_valid_o,

  // Allocation
  input  logic                                alloc_l2_valid_ag_i,
  input  reqq_alloc_t                         alloc_l2_info_ag_i,
  input  logic                                alloc_l3_valid_ag_i,
  input  reqq_alloc_t                         alloc_l3_info_ag_i,

  // Tag TC
  input  logic                                pipe_tag_rsp_valid_i,
  input  tag_rsp_t                            pipe_tag_rsp_info_i,

  // Rbuf
  input  logic                                pipe_rbuf_rd_req_ready_i,
  input  logic                                pipe_rbuf_rd_req_valid_i,
  input  rbuf_rd_req_t                        pipe_rbuf_rd_req_info_i,

  // Mesh requests / responses
  input  logic                                to_l3_mesh_master_bank_req_valid_i,
  input  mesh_master_req_t                    to_l3_mesh_master_bank_req_info_i,
  input  logic                                to_l3_mesh_master_bank_req_ready_i,
  input  logic                                to_l3_mesh_master_bank_rsp_valid_i,
  input  mesh_master_rsp_t                    to_l3_mesh_master_bank_rsp_info_i,
  input  logic                                to_l3_mesh_master_bank_rsp_ready_i,
  input  logic                                to_sys_mesh_master_bank_req_valid_i,
  input  mesh_master_req_t                    to_sys_mesh_master_bank_req_info_i,
  input  logic                                to_sys_mesh_master_bank_req_ready_i,
  input  logic                                to_sys_mesh_master_bank_rsp_valid_i,
  input  mesh_master_rsp_t                    to_sys_mesh_master_bank_rsp_info_i,
  input  logic                                to_sys_mesh_master_bank_rsp_ready_i,

  // l3_slave response
  input  logic                                rspmux_l3_sent_valid_i,
  input  logic [NumEntriesId-1:0]             rspmux_l3_sent_id_i,

  // l2 neigh response
  input  rspmux_sent_t                        rspmux_l2_trace_sent_i,

  // traced reqq state
  input  reqq_entry_state_t                   traced_reqq_state_i,

  // reqq state
  input  reqq_entry_state_t [NumEntries-1:0]  reqq_state_i,

  // ESRs
  input  bank_esr_ctl_t                       esr_ctl_i
);

  // ════════════════════════════════════════════════════════════
  // Clock gating
  // ════════════════════════════════════════════════════════════

  logic clock_en;
  assign clock_en = ~rst_ni | (|esr_ctl_i.esr_sc_trace_type_hot_enable);

  logic clk_gated;
  prim_clk_gate u_clk_gate (
    .clk_i  (clk_free_i),
    .en_i   (clock_en),
    .dft_i  (dft_i),
    .clk_o  (clk_gated)
  );

  // ════════════════════════════════════════════════════════════
  // Build up alloc l2 trace snippet
  // ════════════════════════════════════════════════════════════

  trace_alloc_t alloc_l2;
  logic         alloc_l2_valid;

  always_comb begin
    alloc_l2       = '0;
    alloc_l2_valid = 1'b0;
    if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnAllocL2]
        & alloc_l2_valid_ag_i
        & alloc_l2_info_ag_i.traced) begin

      alloc_l2_valid       = 1'b1;
      alloc_l2.trace_type  = TraceAllocL2;
      alloc_l2.reqq_id     = alloc_l2_info_ag_i.reqq_id;
      alloc_l2.port_id     = alloc_l2_info_ag_i.port_id;
      alloc_l2.orig_opcode = alloc_l2_info_ag_i.orig_opcode;
      alloc_l2.address     = alloc_l2_info_ag_i.address[PaSize-1:LineOffsetIdSize];
      alloc_l2.qwen        = alloc_l2_info_ag_i.qwen;

      // remap Flush, Evict, Prefetch to unused orig_opcode when start level
      // is beyond this level — saves bits.
      if ((alloc_l2_info_ag_i.orig_opcode == OrigCopFlush) &
          (alloc_l2_info_ag_i.cache_level_start inside {CacheL3, CacheMem})) begin
        alloc_l2.orig_opcode = OrigCopFlushNext;
      end
      if ((alloc_l2_info_ag_i.orig_opcode == OrigCopEvict) &
          (alloc_l2_info_ag_i.cache_level_start inside {CacheL3, CacheMem})) begin
        alloc_l2.orig_opcode = OrigCopEvictNext;
      end
      if ((alloc_l2_info_ag_i.orig_opcode == OrigCopPrefetch) &
          (alloc_l2_info_ag_i.cache_level_dest inside {CacheL3, CacheMem})) begin
        alloc_l2.orig_opcode = OrigCopPrefetchNext;
      end
    end
  end

  // ════════════════════════════════════════════════════════════
  // Build up alloc l3 trace snippet
  // ════════════════════════════════════════════════════════════

  trace_alloc_t alloc_l3;
  logic         alloc_l3_valid;

  always_comb begin
    alloc_l3       = '0;
    alloc_l3_valid = 1'b0;
    if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnAllocL3]
        & alloc_l3_valid_ag_i
        & alloc_l3_info_ag_i.traced) begin

      alloc_l3_valid       = 1'b1;
      alloc_l3.trace_type  = TraceAllocL3;
      alloc_l3.reqq_id     = alloc_l3_info_ag_i.reqq_id;
      alloc_l3.port_id     = alloc_l3_info_ag_i.port_id;
      alloc_l3.orig_opcode = alloc_l3_info_ag_i.orig_opcode;
      alloc_l3.address     = alloc_l3_info_ag_i.address[PaSize-1:LineOffsetIdSize];
      alloc_l3.qwen        = alloc_l3_info_ag_i.qwen;
    end
  end

  // ════════════════════════════════════════════════════════════
  // Build up TC trace snippet
  // ════════════════════════════════════════════════════════════

  trace_tc_t tc_status;
  logic      tc_status_valid;

  always_comb begin
    tc_status       = '0;
    tc_status_valid = 1'b0;
    if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnTc]
        & (~(pipe_tag_rsp_info_i.opcode inside {L2Fill, L3Fill})
           | esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnFill])
        & pipe_tag_rsp_valid_i
        & reqq_state_i[pipe_tag_rsp_info_i.reqq_id].traced) begin

      tc_status_valid      = 1'b1;
      tc_status.trace_type = TraceTC;
      tc_status.hit        = pipe_tag_rsp_info_i.hit;
      tc_status.way        = pipe_tag_rsp_info_i.tag_ram_hit_way;
      tc_status.victim     = pipe_tag_rsp_info_i.victim;

      if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnTcReqqId]) begin
        tc_status.reqq_id = pipe_tag_rsp_info_i.reqq_id;
      end else begin
        tc_status.reqq_id = pipe_tag_rsp_info_i.opcode[NumEntriesId-1:0];
      end

      if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnTcVictim]) begin
        tc_status.address = pipe_tag_rsp_info_i.victim_address[PaSize-1-LineOffsetIdSize:0]
                            & {(PaSize-LineOffsetIdSize){pipe_tag_rsp_info_i.victim}};
        tc_status.qwen    = pipe_tag_rsp_info_i.victim_qwen
                            & {LineQwSize{pipe_tag_rsp_info_i.victim}};
      end else begin
        tc_status.address = pipe_tag_rsp_info_i.orig_address[PaSize-LineOffsetIdSize-1:0];
        tc_status.qwen    = pipe_tag_rsp_info_i.tag_ram_hit_qwen;
      end
    end
  end

  // ════════════════════════════════════════════════════════════
  // Build up rbuf trace snippet
  // ════════════════════════════════════════════════════════════

  // Register rbuf before trace
  logic        pipe_rbuf_rd_req_valid_q;
  rbuf_rd_req_t pipe_rbuf_rd_req_info_q;

  logic rbuf_en;
  assign rbuf_en = esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnRbuf];

  // RST_EN_FF: pipe_rbuf_rd_req_valid_q
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) begin
      pipe_rbuf_rd_req_valid_q <= '0;
    end else if (rbuf_en) begin
      pipe_rbuf_rd_req_valid_q <= pipe_rbuf_rd_req_valid_i & pipe_rbuf_rd_req_ready_i;
    end
  end

  // EN_FF: pipe_rbuf_rd_req_info_q
  always_ff @(posedge clk_gated) begin
    if (rbuf_en) begin
      pipe_rbuf_rd_req_info_q <= pipe_rbuf_rd_req_info_i;
    end
  end

  trace_rbuf_t rbuf;
  logic        rbuf_valid;

  always_comb begin
    rbuf       = '0;
    rbuf_valid = 1'b0;
    if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnRbuf]
        & pipe_rbuf_rd_req_valid_q
        & reqq_state_i[pipe_rbuf_rd_req_info_q.reqq_id].traced) begin

      rbuf_valid       = 1'b1;
      rbuf.trace_type  = TraceRbuf;
      rbuf.reqq_id     = pipe_rbuf_rd_req_info_q.reqq_id;
      rbuf.address     = reqq_state_i[pipe_rbuf_rd_req_info_q.reqq_id].address[PaSize-LineOffsetIdSize-1:0];
    end
  end

  // ════════════════════════════════════════════════════════════
  // Build up mesh/rsp trace snippet
  // ════════════════════════════════════════════════════════════

  logic                      to_l3_req_valid_q;
  logic [NumEntriesId-1:0]   to_l3_req_reqq_id_q;
  logic                      to_l3_rsp_valid_q;
  logic [NumEntriesId-1:0]   to_l3_rsp_reqq_id_q;
  logic                      to_sys_req_valid_q;
  logic [NumEntriesId-1:0]   to_sys_req_reqq_id_q;
  logic                      to_sys_rsp_valid_q;
  logic [NumEntriesId-1:0]   to_sys_rsp_reqq_id_q;
  logic                      l3_slave_rsp_valid_q;
  logic [NumEntriesId-1:0]   l3_slave_rsp_reqq_id_q;
  logic                      l2_neigh_rsp_valid_q;
  logic [NumEntriesId-1:0]   l2_neigh_rsp_reqq_id_q;

  logic mesh_en;
  assign mesh_en = esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnMesh];

  // ── to_l3_req ──
  logic to_l3_req_valid_d;
  assign to_l3_req_valid_d = to_l3_mesh_master_bank_req_valid_i
                           & to_l3_mesh_master_bank_req_ready_i
                           & reqq_state_i[to_l3_mesh_master_bank_req_info_i.id].traced
                           & mesh_en;

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)          to_l3_req_valid_q <= '0;
    else if (mesh_en)     to_l3_req_valid_q <= to_l3_req_valid_d;
  end
  always_ff @(posedge clk_gated) begin
    if (to_l3_req_valid_d) to_l3_req_reqq_id_q <= to_l3_mesh_master_bank_req_info_i.id;
  end

  // ── to_l3_rsp ──
  logic to_l3_rsp_valid_d;
  assign to_l3_rsp_valid_d = to_l3_mesh_master_bank_rsp_valid_i
                           & to_l3_mesh_master_bank_rsp_ready_i
                           & reqq_state_i[to_l3_mesh_master_bank_rsp_info_i.id].traced
                           & mesh_en;

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)          to_l3_rsp_valid_q <= '0;
    else if (mesh_en)     to_l3_rsp_valid_q <= to_l3_rsp_valid_d;
  end
  always_ff @(posedge clk_gated) begin
    if (to_l3_rsp_valid_d) to_l3_rsp_reqq_id_q <= to_l3_mesh_master_bank_rsp_info_i.id;
  end

  // ── to_sys_req ──
  logic to_sys_req_valid_d;
  assign to_sys_req_valid_d = to_sys_mesh_master_bank_req_valid_i
                            & to_sys_mesh_master_bank_req_ready_i
                            & reqq_state_i[to_sys_mesh_master_bank_req_info_i.id].traced
                            & mesh_en;

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)          to_sys_req_valid_q <= '0;
    else if (mesh_en)     to_sys_req_valid_q <= to_sys_req_valid_d;
  end
  always_ff @(posedge clk_gated) begin
    if (to_sys_req_valid_d) to_sys_req_reqq_id_q <= to_sys_mesh_master_bank_req_info_i.id;
  end

  // ── to_sys_rsp ──
  logic to_sys_rsp_valid_d;
  assign to_sys_rsp_valid_d = to_sys_mesh_master_bank_rsp_valid_i
                            & to_sys_mesh_master_bank_rsp_ready_i
                            & reqq_state_i[to_sys_mesh_master_bank_rsp_info_i.id].traced
                            & mesh_en;

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)          to_sys_rsp_valid_q <= '0;
    else if (mesh_en)     to_sys_rsp_valid_q <= to_sys_rsp_valid_d;
  end
  always_ff @(posedge clk_gated) begin
    if (to_sys_rsp_valid_d) to_sys_rsp_reqq_id_q <= to_sys_mesh_master_bank_rsp_info_i.id;
  end

  // ── l3_slave_rsp ──
  logic l3_slave_rsp_valid_d;
  assign l3_slave_rsp_valid_d = rspmux_l3_sent_valid_i
                              & reqq_state_i[rspmux_l3_sent_id_i].traced
                              & mesh_en;

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)          l3_slave_rsp_valid_q <= '0;
    else if (mesh_en)     l3_slave_rsp_valid_q <= l3_slave_rsp_valid_d;
  end
  always_ff @(posedge clk_gated) begin
    if (l3_slave_rsp_valid_d) l3_slave_rsp_reqq_id_q <= rspmux_l3_sent_id_i;
  end

  // ── l2_neigh_rsp ──
  logic l2_neigh_rsp_valid_d;
  assign l2_neigh_rsp_valid_d = rspmux_l2_trace_sent_i.valid
                              & rspmux_l2_trace_sent_i.traced
                              & mesh_en;

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)          l2_neigh_rsp_valid_q <= '0;
    else if (mesh_en)     l2_neigh_rsp_valid_q <= l2_neigh_rsp_valid_d;
  end
  always_ff @(posedge clk_gated) begin
    if (l2_neigh_rsp_valid_d) l2_neigh_rsp_reqq_id_q <= rspmux_l2_trace_sent_i.reqq_id;
  end

  // Compose mesh snippet
  trace_mesh_t mesh_rsp;
  logic        mesh_rsp_valid;

  always_comb begin
    mesh_rsp = '0;
    if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnMesh]) begin
      mesh_rsp.trace_type = TraceMesh;
      if (to_l3_req_valid_q) begin
        mesh_rsp.to_l3_req_valid   = 1'b1;
        mesh_rsp.to_l3_req_reqq_id = to_l3_req_reqq_id_q;
      end
      if (to_l3_rsp_valid_q) begin
        mesh_rsp.to_l3_rsp_valid   = 1'b1;
        mesh_rsp.to_l3_rsp_reqq_id = to_l3_rsp_reqq_id_q;
      end
      if (to_sys_req_valid_q) begin
        mesh_rsp.to_sys_req_valid   = 1'b1;
        mesh_rsp.to_sys_req_reqq_id = to_sys_req_reqq_id_q;
      end
      if (to_sys_rsp_valid_q) begin
        mesh_rsp.to_sys_rsp_valid   = 1'b1;
        mesh_rsp.to_sys_rsp_reqq_id = to_sys_rsp_reqq_id_q;
      end
      if (l3_slave_rsp_valid_q) begin
        mesh_rsp.l3_slave_rsp_valid   = 1'b1;
        mesh_rsp.l3_slave_rsp_reqq_id = l3_slave_rsp_reqq_id_q;
      end
      if (l2_neigh_rsp_valid_q) begin
        mesh_rsp.l2_neigh_rsp_valid   = 1'b1;
        mesh_rsp.l2_neigh_rsp_reqq_id = l2_neigh_rsp_reqq_id_q;
      end
    end
  end

  assign mesh_rsp_valid =
      mesh_rsp.to_l3_req_valid
    | mesh_rsp.to_l3_rsp_valid
    | mesh_rsp.to_sys_req_valid
    | mesh_rsp.to_sys_rsp_valid
    | mesh_rsp.l3_slave_rsp_valid
    | mesh_rsp.l2_neigh_rsp_valid;

  // ════════════════════════════════════════════════════════════
  // Build up reqq entry trace snippet
  // ════════════════════════════════════════════════════════════

  trace_reqq_t reqq;
  trace_reqq_t reqq_prev_q;
  trace_reqq_t reqq_next_if_changed;
  logic        reqq_valid;

  always_comb begin
    reqq               = '0;
    reqq_valid          = 1'b0;
    reqq_next_if_changed = '0;

    if (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnReqq]) begin
      reqq_next_if_changed.trace_type              = TraceReqq;
      reqq_next_if_changed.padding                 = '0;
      reqq_next_if_changed.reqq_id                 = traced_reqq_state_i.reqq_id;
      reqq_next_if_changed.opcode                  = traced_reqq_state_i.opcode;
      reqq_next_if_changed.rspmux_l2_eligible      = traced_reqq_state_i.rspmux_l2_eligible;
      reqq_next_if_changed.rspmux_l3_eligible      = traced_reqq_state_i.rspmux_l3_eligible;
      reqq_next_if_changed.rspmux_sent             = traced_reqq_state_i.rspmux_sent;
      reqq_next_if_changed.pipe_req_eligible       = traced_reqq_state_i.pipe_req_eligible;
      reqq_next_if_changed.to_l3_mesh_req_eligible = traced_reqq_state_i.to_l3_mesh_req_eligible;
      reqq_next_if_changed.to_sys_mesh_req_eligible = traced_reqq_state_i.to_sys_mesh_req_eligible;
      reqq_next_if_changed.rbuf_req_eligible       = traced_reqq_state_i.rbuf_req_eligible;
      reqq_next_if_changed.inflight                = traced_reqq_state_i.inflight;
      reqq_next_if_changed.data_ready              = traced_reqq_state_i.data_ready;
      reqq_next_if_changed.wait_for_dataq          = traced_reqq_state_i.wait_for_dataq;
      reqq_next_if_changed.dep_valid               = traced_reqq_state_i.dep_valid;
      reqq_next_if_changed.dep_tail                = traced_reqq_state_i.dep_tail;
      reqq_next_if_changed.dep_meshll_tail         = traced_reqq_state_i.dep_meshll_tail;
      reqq_next_if_changed.dep_non_meshll_head     = traced_reqq_state_i.dep_non_meshll_head;
      reqq_next_if_changed.dont_depend_on_me       = traced_reqq_state_i.dont_depend_on_me;
      reqq_next_if_changed.rbuf_valid              = traced_reqq_state_i.rbuf_valid;
      reqq_next_if_changed.rbuf_pending_valid      = traced_reqq_state_i.rbuf_pending_valid;
      reqq_next_if_changed.err_detected            = traced_reqq_state_i.err_detected;

      reqq_valid = (reqq_next_if_changed != reqq_prev_q)
                 & traced_reqq_state_i.valid
                 & traced_reqq_state_i.traced;
      if (reqq_valid) begin
        reqq = reqq_next_if_changed;
      end
    end
  end

  // RST_EN_FF: reqq_prev_q
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) begin
      reqq_prev_q <= '0;
    end else if (reqq_valid) begin
      reqq_prev_q <= reqq;
    end
  end

  // ════════════════════════════════════════════════════════════
  // Pack the snippets together into a trace packet
  // ════════════════════════════════════════════════════════════

  trace_packet_t  trace_packet;
  trace_packet_t  trace_packet_r1_q;
  trace_segment_t trace_segment    [3];
  logic           trace_segment_filled [3];
  trace_segment_t trace_segment_prev   [3];
  logic alloc_l2_placed;
  logic alloc_l3_placed;
  logic tc_status_placed;
  logic rbuf_placed;
  logic mesh_placed;
  logic reqq_placed;

  assign trace_segment_prev[0] = trace_packet_r1_q.segment0;
  assign trace_segment_prev[1] = trace_packet_r1_q.segment1;
  assign trace_segment_prev[2] = trace_packet_r1_q.segment2;

  always_comb begin
    alloc_l2_placed  = 1'b0;
    alloc_l3_placed  = 1'b0;
    tc_status_placed = 1'b0;
    rbuf_placed      = 1'b0;
    mesh_placed      = 1'b0;
    reqq_placed      = 1'b0;

    for (int ii = 0; ii < 3; ii++) begin
      // Default to previous with type set to none
      trace_segment[ii]        = trace_segment_prev[ii];
      trace_segment_filled[ii] = 1'b0;
      trace_segment[ii].tc.trace_type = TraceNone;

      if (alloc_l2_valid & ~trace_segment_filled[ii] & ~alloc_l2_placed) begin
        trace_segment_filled[ii] = 1'b1;
        trace_segment[ii]        = alloc_l2;
        alloc_l2_placed          = 1'b1;
      end
      if (alloc_l3_valid & ~trace_segment_filled[ii] & ~alloc_l3_placed) begin
        trace_segment_filled[ii] = 1'b1;
        trace_segment[ii]        = alloc_l3;
        alloc_l3_placed          = 1'b1;
      end
      if (tc_status_valid & ~trace_segment_filled[ii] & ~tc_status_placed) begin
        trace_segment_filled[ii] = 1'b1;
        trace_segment[ii]        = tc_status;
        tc_status_placed         = 1'b1;
      end
      if (rbuf_valid & ~trace_segment_filled[ii] & ~rbuf_placed) begin
        trace_segment_filled[ii] = 1'b1;
        trace_segment[ii]        = rbuf;
        rbuf_placed              = 1'b1;
      end
      if (mesh_rsp_valid & ~trace_segment_filled[ii] & ~mesh_placed) begin
        trace_segment_filled[ii] = 1'b1;
        trace_segment[ii]        = mesh_rsp;
        mesh_placed              = 1'b1;
      end
      if (reqq_valid & ~trace_segment_filled[ii] & ~reqq_placed) begin
        trace_segment_filled[ii] = 1'b1;
        trace_segment[ii]        = reqq;
        reqq_placed              = 1'b1;
      end
    end
  end

  assign trace_packet.segment0 = trace_segment[0];
  assign trace_packet.segment1 = trace_segment[1];
  assign trace_packet.segment2 = trace_segment[2];

  // ════════════════════════════════════════════════════════════
  // Gather all trace valid signals and register the packet
  // ════════════════════════════════════════════════════════════

  logic trace_valid;
  logic trace_valid_r1_q;

  assign trace_valid =
      alloc_l2_valid
    | alloc_l3_valid
    | tc_status_valid
    | rbuf_valid
    | mesh_rsp_valid
    | reqq_valid;

  // RST_FF: trace_valid_r1_q
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) trace_valid_r1_q <= '0;
    else         trace_valid_r1_q <= trace_valid;
  end

  // RST_EN_FF: trace_packet_r1_q
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)        trace_packet_r1_q <= '0;
    else if (trace_valid) trace_packet_r1_q <= trace_packet;
  end

  assign bank_trace_data_o  = trace_packet_r1_q;
  assign bank_trace_valid_o = trace_valid_r1_q;

  // ════════════════════════════════════════════════════════════
  // Unused signal tie-off
  // ════════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{
    1'b0,
    pipe_tag_rsp_info_i,
    pipe_rbuf_rd_req_info_i,
    pipe_rbuf_rd_req_info_q,
    to_l3_mesh_master_bank_req_info_i,
    to_l3_mesh_master_bank_rsp_info_i,
    to_sys_mesh_master_bank_req_info_i,
    to_sys_mesh_master_bank_rsp_info_i,
    alloc_l2_info_ag_i,
    alloc_l3_info_ag_i,
    traced_reqq_state_i,
    esr_ctl_i,
    reqq_state_i,
    dft_i,
    unused_ok
  };

/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */

endmodule : shirecache_bank_trace
