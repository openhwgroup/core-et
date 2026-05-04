// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_trace
// vs new shirecache_bank_trace.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
module cosim_trace_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,

  // ── Alloc L2 ──────────────────────────────────────────────
  input  logic                              alloc_l2_valid_i,
  input  logic [NumEntriesId-1:0]           alloc_l2_reqq_id_i,
  input  logic [PortIdSize-1:0]             alloc_l2_port_id_i,
  input  logic [4:0]                        alloc_l2_orig_opcode_i,
  input  logic [PaSize-1:0]                 alloc_l2_address_i,
  input  logic [LineQwSize-1:0]             alloc_l2_qwen_i,
  input  logic                              alloc_l2_traced_i,
  input  logic [1:0]                        alloc_l2_cache_level_start_i,
  input  logic [1:0]                        alloc_l2_cache_level_dest_i,

  // ── Alloc L3 ──────────────────────────────────────────────
  input  logic                              alloc_l3_valid_i,
  input  logic [NumEntriesId-1:0]           alloc_l3_reqq_id_i,
  input  logic [PortIdSize-1:0]             alloc_l3_port_id_i,
  input  logic [4:0]                        alloc_l3_orig_opcode_i,
  input  logic [PaSize-1:0]                 alloc_l3_address_i,
  input  logic [LineQwSize-1:0]             alloc_l3_qwen_i,
  input  logic                              alloc_l3_traced_i,

  // ── Tag TC ────────────────────────────────────────────────
  input  logic                              pipe_tag_rsp_valid_i,
  input  logic [NumEntriesId-1:0]           pipe_tag_rsp_reqq_id_i,
  input  logic                              pipe_tag_rsp_hit_i,
  input  logic [WayIdSize-1:0]             pipe_tag_rsp_way_i,
  input  logic                              pipe_tag_rsp_victim_i,
  input  logic [PaSize-1:0]                 pipe_tag_rsp_orig_address_i,
  input  logic [PaSize-1:0]                 pipe_tag_rsp_victim_address_i,
  input  logic [LineQwSize-1:0]             pipe_tag_rsp_victim_qwen_i,
  input  logic [LineQwSize-1:0]             pipe_tag_rsp_tag_ram_hit_qwen_i,
  input  logic [5:0]                        pipe_tag_rsp_opcode_i,

  // ── Rbuf ──────────────────────────────────────────────────
  input  logic                              pipe_rbuf_rd_req_ready_i,
  input  logic                              pipe_rbuf_rd_req_valid_i,
  input  logic [NumEntriesId-1:0]           pipe_rbuf_rd_req_reqq_id_i,

  // ── Mesh master ───────────────────────────────────────────
  input  logic                              to_l3_mm_req_valid_i,
  input  logic [NumEntriesId-1:0]           to_l3_mm_req_id_i,
  input  logic                              to_l3_mm_req_ready_i,
  input  logic                              to_l3_mm_rsp_valid_i,
  input  logic [NumEntriesId-1:0]           to_l3_mm_rsp_id_i,
  input  logic                              to_l3_mm_rsp_ready_i,
  input  logic                              to_sys_mm_req_valid_i,
  input  logic [NumEntriesId-1:0]           to_sys_mm_req_id_i,
  input  logic                              to_sys_mm_req_ready_i,
  input  logic                              to_sys_mm_rsp_valid_i,
  input  logic [NumEntriesId-1:0]           to_sys_mm_rsp_id_i,
  input  logic                              to_sys_mm_rsp_ready_i,

  // ── L3 slave rsp ──────────────────────────────────────────
  input  logic                              rspmux_l3_sent_valid_i,
  input  logic [NumEntriesId-1:0]           rspmux_l3_sent_id_i,

  // ── L2 neigh rsp ─────────────────────────────────────────
  input  logic                              rspmux_l2_sent_valid_i,
  input  logic [NumEntriesId-1:0]           rspmux_l2_sent_reqq_id_i,
  input  logic                              rspmux_l2_sent_traced_i,

  // ── traced_reqq_state ─────────────────────────────────────
  input  logic                              trs_valid_i,
  input  logic [NumEntriesId-1:0]           trs_reqq_id_i,
  input  logic [5:0]                        trs_opcode_i,
  input  logic                              trs_rspmux_l2_eligible_i,
  input  logic                              trs_rspmux_l3_eligible_i,
  input  logic                              trs_rspmux_sent_i,
  input  logic                              trs_pipe_req_eligible_i,
  input  logic                              trs_to_l3_mesh_req_eligible_i,
  input  logic                              trs_to_sys_mesh_req_eligible_i,
  input  logic                              trs_rbuf_req_eligible_i,
  input  logic                              trs_inflight_i,
  input  logic                              trs_data_ready_i,
  input  logic                              trs_wait_for_dataq_i,
  input  logic                              trs_dep_valid_i,
  input  logic                              trs_dep_tail_i,
  input  logic                              trs_dep_meshll_tail_i,
  input  logic                              trs_dep_non_meshll_head_i,
  input  logic                              trs_dont_depend_on_me_i,
  input  logic                              trs_rbuf_valid_i,
  input  logic                              trs_rbuf_pending_valid_i,
  input  logic                              trs_err_detected_i,
  input  logic                              trs_traced_i,

  // ── reqq_state (2 indexed entries) ────────────────────────
  input  logic [NumEntriesId-1:0]           rs0_idx_i,
  input  logic                              rs0_valid_i,
  input  logic                              rs0_traced_i,
  input  logic [PaSize-1:0]                 rs0_address_i,
  input  logic [NumEntriesId-1:0]           rs1_idx_i,
  input  logic                              rs1_valid_i,
  input  logic                              rs1_traced_i,
  input  logic [PaSize-1:0]                 rs1_address_i,

  // ── ESR ───────────────────────────────────────────────────
  input  logic [TraceEnSize-1:0]            esr_trace_type_hot_enable_i,

  // ── Original outputs ──────────────────────────────────────
  output logic                              orig_trace_valid_o,
  output logic [TraceSegmentSize-1:0]       orig_trace_seg0_o,
  output logic [TraceSegmentSize-1:0]       orig_trace_seg1_o,
  output logic [TraceSegmentSize-1:0]       orig_trace_seg2_o,

  // ── New outputs ───────────────────────────────────────────
  output logic                              new_trace_valid_o,
  output logic [TraceSegmentSize-1:0]       new_trace_seg0_o,
  output logic [TraceSegmentSize-1:0]       new_trace_seg1_o,
  output logic [TraceSegmentSize-1:0]       new_trace_seg2_o
);

  // ══════════════════════════════════════════════════════════
  // Build original struct inputs
  // ══════════════════════════════════════════════════════════

  sc_reqq_alloc_t orig_alloc_l2, orig_alloc_l3;
  always_comb begin
    orig_alloc_l2 = '0;
    orig_alloc_l2.reqq_id           = alloc_l2_reqq_id_i;
    orig_alloc_l2.port_id           = alloc_l2_port_id_i;
    orig_alloc_l2.orig_opcode       = sc_reqq_orig_opcode_t'(alloc_l2_orig_opcode_i);
    orig_alloc_l2.address           = alloc_l2_address_i;
    orig_alloc_l2.qwen              = alloc_l2_qwen_i;
    orig_alloc_l2.traced            = alloc_l2_traced_i;
    orig_alloc_l2.cache_level_start = sc_cache_level_t'(alloc_l2_cache_level_start_i);
    orig_alloc_l2.cache_level_dest  = sc_cache_level_t'(alloc_l2_cache_level_dest_i);

    orig_alloc_l3 = '0;
    orig_alloc_l3.reqq_id           = alloc_l3_reqq_id_i;
    orig_alloc_l3.port_id           = alloc_l3_port_id_i;
    orig_alloc_l3.orig_opcode       = sc_reqq_orig_opcode_t'(alloc_l3_orig_opcode_i);
    orig_alloc_l3.address           = alloc_l3_address_i;
    orig_alloc_l3.qwen              = alloc_l3_qwen_i;
    orig_alloc_l3.traced            = alloc_l3_traced_i;
  end

  sc_pipe_tag_rsp_t orig_tag_rsp;
  always_comb begin
    orig_tag_rsp = '0;
    orig_tag_rsp.reqq_id          = pipe_tag_rsp_reqq_id_i;
    orig_tag_rsp.hit              = pipe_tag_rsp_hit_i;
    orig_tag_rsp.tag_ram_hit_way  = pipe_tag_rsp_way_i;
    orig_tag_rsp.victim           = pipe_tag_rsp_victim_i;
    orig_tag_rsp.orig_address     = pipe_tag_rsp_orig_address_i;
    orig_tag_rsp.victim_address   = pipe_tag_rsp_victim_address_i;
    orig_tag_rsp.victim_qwen      = pipe_tag_rsp_victim_qwen_i;
    orig_tag_rsp.tag_ram_hit_qwen = pipe_tag_rsp_tag_ram_hit_qwen_i;
    orig_tag_rsp.opcode           = sc_reqq_opcode_t'(pipe_tag_rsp_opcode_i);
  end

  sc_pipe_rbuf_rd_req_t orig_rbuf_req;
  always_comb begin
    orig_rbuf_req = '0;
    orig_rbuf_req.reqq_id = pipe_rbuf_rd_req_reqq_id_i;
  end

  sc_mesh_master_req_t orig_to_l3_mm_req, orig_to_sys_mm_req;
  sc_mesh_master_rsp_t orig_to_l3_mm_rsp, orig_to_sys_mm_rsp;
  always_comb begin
    orig_to_l3_mm_req    = '0;
    orig_to_l3_mm_req.id = to_l3_mm_req_id_i;
    orig_to_l3_mm_rsp    = '0;
    orig_to_l3_mm_rsp.id = to_l3_mm_rsp_id_i;
    orig_to_sys_mm_req    = '0;
    orig_to_sys_mm_req.id = to_sys_mm_req_id_i;
    orig_to_sys_mm_rsp    = '0;
    orig_to_sys_mm_rsp.id = to_sys_mm_rsp_id_i;
  end

  sc_rspmux_sent_t orig_rspmux_l2;
  always_comb begin
    orig_rspmux_l2.valid   = rspmux_l2_sent_valid_i;
    orig_rspmux_l2.reqq_id = rspmux_l2_sent_reqq_id_i;
    orig_rspmux_l2.traced  = rspmux_l2_sent_traced_i;
  end

  sc_reqq_entry_state_t orig_traced_reqq_state;
  always_comb begin
    orig_traced_reqq_state = '0;
    orig_traced_reqq_state.valid                   = trs_valid_i;
    orig_traced_reqq_state.reqq_id                 = trs_reqq_id_i;
    orig_traced_reqq_state.opcode                  = sc_reqq_opcode_t'(trs_opcode_i);
    orig_traced_reqq_state.rspmux_l2_eligible      = trs_rspmux_l2_eligible_i;
    orig_traced_reqq_state.rspmux_l3_eligible      = trs_rspmux_l3_eligible_i;
    orig_traced_reqq_state.rspmux_sent             = trs_rspmux_sent_i;
    orig_traced_reqq_state.pipe_req_eligible       = trs_pipe_req_eligible_i;
    orig_traced_reqq_state.to_l3_mesh_req_eligible = trs_to_l3_mesh_req_eligible_i;
    orig_traced_reqq_state.to_sys_mesh_req_eligible = trs_to_sys_mesh_req_eligible_i;
    orig_traced_reqq_state.rbuf_req_eligible       = trs_rbuf_req_eligible_i;
    orig_traced_reqq_state.inflight                = trs_inflight_i;
    orig_traced_reqq_state.data_ready              = trs_data_ready_i;
    orig_traced_reqq_state.wait_for_dataq          = trs_wait_for_dataq_i;
    orig_traced_reqq_state.dep_valid               = trs_dep_valid_i;
    orig_traced_reqq_state.dep_tail                = trs_dep_tail_i;
    orig_traced_reqq_state.dep_meshll_tail         = trs_dep_meshll_tail_i;
    orig_traced_reqq_state.dep_non_meshll_head     = trs_dep_non_meshll_head_i;
    orig_traced_reqq_state.dont_depend_on_me       = trs_dont_depend_on_me_i;
    orig_traced_reqq_state.rbuf_valid              = trs_rbuf_valid_i;
    orig_traced_reqq_state.rbuf_pending_valid      = trs_rbuf_pending_valid_i;
    orig_traced_reqq_state.err_detected            = trs_err_detected_i;
    orig_traced_reqq_state.traced                  = trs_traced_i;
  end

  /* verilator lint_off WIDTHCONCAT */
  sc_reqq_entry_state_t [NumEntries-1:0] orig_reqq_state;
  always_comb begin
    orig_reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */
    orig_reqq_state[rs0_idx_i].valid   = rs0_valid_i;
    orig_reqq_state[rs0_idx_i].traced  = rs0_traced_i;
    orig_reqq_state[rs0_idx_i].address = rs0_address_i;
    if (rs1_idx_i != rs0_idx_i) begin
      orig_reqq_state[rs1_idx_i].valid   = rs1_valid_i;
      orig_reqq_state[rs1_idx_i].traced  = rs1_traced_i;
      orig_reqq_state[rs1_idx_i].address = rs1_address_i;
    end
  end

  sc_bank_esr_ctl_t orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.esr_sc_trace_type_hot_enable = esr_trace_type_hot_enable_i;
  end

  // ══════════════════════════════════════════════════════════
  // Build new struct inputs
  // ══════════════════════════════════════════════════════════

  reqq_alloc_t new_alloc_l2, new_alloc_l3;
  always_comb begin
    new_alloc_l2 = '0;
    new_alloc_l2.reqq_id           = alloc_l2_reqq_id_i;
    new_alloc_l2.port_id           = alloc_l2_port_id_i;
    new_alloc_l2.orig_opcode       = reqq_orig_opcode_e'(alloc_l2_orig_opcode_i);
    new_alloc_l2.address           = alloc_l2_address_i;
    new_alloc_l2.qwen              = alloc_l2_qwen_i;
    new_alloc_l2.traced            = alloc_l2_traced_i;
    new_alloc_l2.cache_level_start = cache_level_e'(alloc_l2_cache_level_start_i);
    new_alloc_l2.cache_level_dest  = cache_level_e'(alloc_l2_cache_level_dest_i);

    new_alloc_l3 = '0;
    new_alloc_l3.reqq_id           = alloc_l3_reqq_id_i;
    new_alloc_l3.port_id           = alloc_l3_port_id_i;
    new_alloc_l3.orig_opcode       = reqq_orig_opcode_e'(alloc_l3_orig_opcode_i);
    new_alloc_l3.address           = alloc_l3_address_i;
    new_alloc_l3.qwen              = alloc_l3_qwen_i;
    new_alloc_l3.traced            = alloc_l3_traced_i;
  end

  tag_rsp_t new_tag_rsp;
  always_comb begin
    new_tag_rsp = '0;
    new_tag_rsp.reqq_id          = pipe_tag_rsp_reqq_id_i;
    new_tag_rsp.hit              = pipe_tag_rsp_hit_i;
    new_tag_rsp.tag_ram_hit_way  = pipe_tag_rsp_way_i;
    new_tag_rsp.victim           = pipe_tag_rsp_victim_i;
    new_tag_rsp.orig_address     = pipe_tag_rsp_orig_address_i;
    new_tag_rsp.victim_address   = pipe_tag_rsp_victim_address_i;
    new_tag_rsp.victim_qwen      = pipe_tag_rsp_victim_qwen_i;
    new_tag_rsp.tag_ram_hit_qwen = pipe_tag_rsp_tag_ram_hit_qwen_i;
    new_tag_rsp.opcode           = reqq_opcode_e'(pipe_tag_rsp_opcode_i);
  end

  rbuf_rd_req_t new_rbuf_req;
  always_comb begin
    new_rbuf_req = '0;
    new_rbuf_req.reqq_id = pipe_rbuf_rd_req_reqq_id_i;
  end

  mesh_master_req_t new_to_l3_mm_req, new_to_sys_mm_req;
  mesh_master_rsp_t new_to_l3_mm_rsp, new_to_sys_mm_rsp;
  always_comb begin
    new_to_l3_mm_req    = '0;
    new_to_l3_mm_req.id = to_l3_mm_req_id_i;
    new_to_l3_mm_rsp    = '0;
    new_to_l3_mm_rsp.id = to_l3_mm_rsp_id_i;
    new_to_sys_mm_req    = '0;
    new_to_sys_mm_req.id = to_sys_mm_req_id_i;
    new_to_sys_mm_rsp    = '0;
    new_to_sys_mm_rsp.id = to_sys_mm_rsp_id_i;
  end

  rspmux_sent_t new_rspmux_l2;
  always_comb begin
    new_rspmux_l2.valid   = rspmux_l2_sent_valid_i;
    new_rspmux_l2.reqq_id = rspmux_l2_sent_reqq_id_i;
    new_rspmux_l2.traced  = rspmux_l2_sent_traced_i;
  end

  reqq_entry_state_t new_traced_reqq_state;
  always_comb begin
    new_traced_reqq_state = '0;
    new_traced_reqq_state.valid                   = trs_valid_i;
    new_traced_reqq_state.reqq_id                 = trs_reqq_id_i;
    new_traced_reqq_state.opcode                  = reqq_opcode_e'(trs_opcode_i);
    new_traced_reqq_state.rspmux_l2_eligible      = trs_rspmux_l2_eligible_i;
    new_traced_reqq_state.rspmux_l3_eligible      = trs_rspmux_l3_eligible_i;
    new_traced_reqq_state.rspmux_sent             = trs_rspmux_sent_i;
    new_traced_reqq_state.pipe_req_eligible       = trs_pipe_req_eligible_i;
    new_traced_reqq_state.to_l3_mesh_req_eligible = trs_to_l3_mesh_req_eligible_i;
    new_traced_reqq_state.to_sys_mesh_req_eligible = trs_to_sys_mesh_req_eligible_i;
    new_traced_reqq_state.rbuf_req_eligible       = trs_rbuf_req_eligible_i;
    new_traced_reqq_state.inflight                = trs_inflight_i;
    new_traced_reqq_state.data_ready              = trs_data_ready_i;
    new_traced_reqq_state.wait_for_dataq          = trs_wait_for_dataq_i;
    new_traced_reqq_state.dep_valid               = trs_dep_valid_i;
    new_traced_reqq_state.dep_tail                = trs_dep_tail_i;
    new_traced_reqq_state.dep_meshll_tail         = trs_dep_meshll_tail_i;
    new_traced_reqq_state.dep_non_meshll_head     = trs_dep_non_meshll_head_i;
    new_traced_reqq_state.dont_depend_on_me       = trs_dont_depend_on_me_i;
    new_traced_reqq_state.rbuf_valid              = trs_rbuf_valid_i;
    new_traced_reqq_state.rbuf_pending_valid      = trs_rbuf_pending_valid_i;
    new_traced_reqq_state.err_detected            = trs_err_detected_i;
    new_traced_reqq_state.traced                  = trs_traced_i;
  end

  /* verilator lint_off WIDTHCONCAT */
  reqq_entry_state_t [NumEntries-1:0] new_reqq_state;
  always_comb begin
    new_reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */
    new_reqq_state[rs0_idx_i].valid   = rs0_valid_i;
    new_reqq_state[rs0_idx_i].traced  = rs0_traced_i;
    new_reqq_state[rs0_idx_i].address = rs0_address_i;
    if (rs1_idx_i != rs0_idx_i) begin
      new_reqq_state[rs1_idx_i].valid   = rs1_valid_i;
      new_reqq_state[rs1_idx_i].traced  = rs1_traced_i;
      new_reqq_state[rs1_idx_i].address = rs1_address_i;
    end
  end

  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_trace_type_hot_enable = esr_trace_type_hot_enable_i;
  end

  // ══════════════════════════════════════════════════════════
  // Original DUT
  // ══════════════════════════════════════════════════════════

  sc_trace_packet_t orig_trace_data;
  logic             orig_trace_valid;

  shire_cache_bank_trace #(
    .NUM_ENTRIES    (NumEntries),
    .NUM_ENTRIES_ID (NumEntriesId)
  ) u_orig (
    .fclock                             (clk_i),
    .reset                              (~rst_ni),

    .bank_trace_data                    (orig_trace_data),
    .bank_trace_valid                   (orig_trace_valid),

    .alloc_l2_valid_ag                  (alloc_l2_valid_i),
    .alloc_l2_info_ag                   (orig_alloc_l2),
    .alloc_l3_valid_ag                  (alloc_l3_valid_i),
    .alloc_l3_info_ag                   (orig_alloc_l3),

    .pipe_tag_rsp_valid                 (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info                  (orig_tag_rsp),

    .pipe_rbuf_rd_req_ready             (pipe_rbuf_rd_req_ready_i),
    .pipe_rbuf_rd_req_valid             (pipe_rbuf_rd_req_valid_i),
    .pipe_rbuf_rd_req_info              (orig_rbuf_req),

    .to_l3_mesh_master_bank_req_valid   (to_l3_mm_req_valid_i),
    .to_l3_mesh_master_bank_req_info    (orig_to_l3_mm_req),
    .to_l3_mesh_master_bank_req_ready   (to_l3_mm_req_ready_i),
    .to_l3_mesh_master_bank_rsp_valid   (to_l3_mm_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_info    (orig_to_l3_mm_rsp),
    .to_l3_mesh_master_bank_rsp_ready   (to_l3_mm_rsp_ready_i),
    .to_sys_mesh_master_bank_req_valid  (to_sys_mm_req_valid_i),
    .to_sys_mesh_master_bank_req_info   (orig_to_sys_mm_req),
    .to_sys_mesh_master_bank_req_ready  (to_sys_mm_req_ready_i),
    .to_sys_mesh_master_bank_rsp_valid  (to_sys_mm_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_info   (orig_to_sys_mm_rsp),
    .to_sys_mesh_master_bank_rsp_ready  (to_sys_mm_rsp_ready_i),

    .rspmux_l3_sent_valid               (rspmux_l3_sent_valid_i),
    .rspmux_l3_sent_id                  (rspmux_l3_sent_id_i),

    .rspmux_l2_trace_sent               (orig_rspmux_l2),

    .traced_reqq_state                  (orig_traced_reqq_state),

    .reqq_state                         (orig_reqq_state),

    .esr_ctl                            (orig_esr_ctl)
  );

  assign orig_trace_valid_o = orig_trace_valid;
  assign orig_trace_seg0_o  = orig_trace_data.segment0;
  assign orig_trace_seg1_o  = orig_trace_data.segment1;
  assign orig_trace_seg2_o  = orig_trace_data.segment2;

  // ══════════════════════════════════════════════════════════
  // New DUT
  // ══════════════════════════════════════════════════════════

  trace_packet_t new_trace_data;
  logic          new_trace_valid;

  shirecache_bank_trace #(
    .NumEntries   (NumEntries),
    .NumEntriesId (NumEntriesId)
  ) u_new (
    .clk_free_i                           (clk_i),
    .rst_ni                               (rst_ni),
    .dft_i                                ('0),

    .bank_trace_data_o                    (new_trace_data),
    .bank_trace_valid_o                   (new_trace_valid),

    .alloc_l2_valid_ag_i                  (alloc_l2_valid_i),
    .alloc_l2_info_ag_i                   (new_alloc_l2),
    .alloc_l3_valid_ag_i                  (alloc_l3_valid_i),
    .alloc_l3_info_ag_i                   (new_alloc_l3),

    .pipe_tag_rsp_valid_i                 (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info_i                  (new_tag_rsp),

    .pipe_rbuf_rd_req_ready_i             (pipe_rbuf_rd_req_ready_i),
    .pipe_rbuf_rd_req_valid_i             (pipe_rbuf_rd_req_valid_i),
    .pipe_rbuf_rd_req_info_i              (new_rbuf_req),

    .to_l3_mesh_master_bank_req_valid_i   (to_l3_mm_req_valid_i),
    .to_l3_mesh_master_bank_req_info_i    (new_to_l3_mm_req),
    .to_l3_mesh_master_bank_req_ready_i   (to_l3_mm_req_ready_i),
    .to_l3_mesh_master_bank_rsp_valid_i   (to_l3_mm_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_info_i    (new_to_l3_mm_rsp),
    .to_l3_mesh_master_bank_rsp_ready_i   (to_l3_mm_rsp_ready_i),
    .to_sys_mesh_master_bank_req_valid_i  (to_sys_mm_req_valid_i),
    .to_sys_mesh_master_bank_req_info_i   (new_to_sys_mm_req),
    .to_sys_mesh_master_bank_req_ready_i  (to_sys_mm_req_ready_i),
    .to_sys_mesh_master_bank_rsp_valid_i  (to_sys_mm_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_info_i   (new_to_sys_mm_rsp),
    .to_sys_mesh_master_bank_rsp_ready_i  (to_sys_mm_rsp_ready_i),

    .rspmux_l3_sent_valid_i               (rspmux_l3_sent_valid_i),
    .rspmux_l3_sent_id_i                  (rspmux_l3_sent_id_i),

    .rspmux_l2_trace_sent_i               (new_rspmux_l2),

    .traced_reqq_state_i                  (new_traced_reqq_state),

    .reqq_state_i                         (new_reqq_state),

    .esr_ctl_i                            (new_esr_ctl)
  );

  assign new_trace_valid_o = new_trace_valid;
  assign new_trace_seg0_o  = new_trace_data.segment0;
  assign new_trace_seg1_o  = new_trace_data.segment1;
  assign new_trace_seg2_o  = new_trace_data.segment2;

  // ══════════════════════════════════════════════════════════
  // Unused
  // ══════════════════════════════════════════════════════════
  logic unused_ok;
  assign unused_ok = &{1'b0,
    orig_trace_data, new_trace_data,
    unused_ok};

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on VARHIDDEN */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
