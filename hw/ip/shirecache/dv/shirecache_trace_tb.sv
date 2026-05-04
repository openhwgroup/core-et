// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_trace.
// Decomposes struct inputs/outputs to scalar ports for C++ driving.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off WIDTHEXPAND */

module shirecache_trace_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,

  // ── Outputs ───────────────────────────────────────────────
  output logic                              bank_trace_valid_o,
  output logic [TraceSegmentSize-1:0]       bank_trace_seg0_o,
  output logic [TraceSegmentSize-1:0]       bank_trace_seg1_o,
  output logic [TraceSegmentSize-1:0]       bank_trace_seg2_o,

  // ── Alloc L2 ──────────────────────────────────────────────
  input  logic                              alloc_l2_valid_i,
  input  logic [ReqqIdSize-1:0]             alloc_l2_reqq_id_i,
  input  logic [PortIdSize-1:0]             alloc_l2_port_id_i,
  input  logic [4:0]                        alloc_l2_orig_opcode_i,
  input  logic [PaSize-1:0]                 alloc_l2_address_i,
  input  logic [LineQwSize-1:0]             alloc_l2_qwen_i,
  input  logic                              alloc_l2_traced_i,
  input  logic [1:0]                        alloc_l2_cache_level_start_i,
  input  logic [1:0]                        alloc_l2_cache_level_dest_i,

  // ── Alloc L3 ──────────────────────────────────────────────
  input  logic                              alloc_l3_valid_i,
  input  logic [ReqqIdSize-1:0]             alloc_l3_reqq_id_i,
  input  logic [PortIdSize-1:0]             alloc_l3_port_id_i,
  input  logic [4:0]                        alloc_l3_orig_opcode_i,
  input  logic [PaSize-1:0]                 alloc_l3_address_i,
  input  logic [LineQwSize-1:0]             alloc_l3_qwen_i,
  input  logic                              alloc_l3_traced_i,

  // ── Tag TC ────────────────────────────────────────────────
  input  logic                              pipe_tag_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             pipe_tag_rsp_reqq_id_i,
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
  input  logic [ReqqIdSize-1:0]             pipe_rbuf_rd_req_reqq_id_i,

  // ── Mesh master to_l3 req ─────────────────────────────────
  input  logic                              to_l3_mm_req_valid_i,
  input  logic [ReqqIdSize-1:0]             to_l3_mm_req_id_i,
  input  logic                              to_l3_mm_req_ready_i,
  // ── Mesh master to_l3 rsp ─────────────────────────────────
  input  logic                              to_l3_mm_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             to_l3_mm_rsp_id_i,
  input  logic                              to_l3_mm_rsp_ready_i,
  // ── Mesh master to_sys req ────────────────────────────────
  input  logic                              to_sys_mm_req_valid_i,
  input  logic [ReqqIdSize-1:0]             to_sys_mm_req_id_i,
  input  logic                              to_sys_mm_req_ready_i,
  // ── Mesh master to_sys rsp ────────────────────────────────
  input  logic                              to_sys_mm_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             to_sys_mm_rsp_id_i,
  input  logic                              to_sys_mm_rsp_ready_i,

  // ── L3 slave rsp ──────────────────────────────────────────
  input  logic                              rspmux_l3_sent_valid_i,
  input  logic [ReqqIdSize-1:0]             rspmux_l3_sent_id_i,

  // ── L2 neigh rsp (rspmux_sent_t) ─────────────────────────
  input  logic                              rspmux_l2_sent_valid_i,
  input  logic [ReqqIdSize-1:0]             rspmux_l2_sent_reqq_id_i,
  input  logic                              rspmux_l2_sent_traced_i,

  // ── traced_reqq_state ─────────────────────────────────────
  input  logic                              trs_valid_i,
  input  logic [ReqqIdSize-1:0]             trs_reqq_id_i,
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

  // ── reqq_state array (2 indexed entries) ──────────────────
  input  logic [ReqqIdSize-1:0]             rs0_idx_i,
  input  logic                              rs0_valid_i,
  input  logic                              rs0_traced_i,
  input  logic [PaSize-1:0]                 rs0_address_i,

  input  logic [ReqqIdSize-1:0]             rs1_idx_i,
  input  logic                              rs1_valid_i,
  input  logic                              rs1_traced_i,
  input  logic [PaSize-1:0]                 rs1_address_i,

  // ── ESR control (trace-related fields) ────────────────────
  input  logic [TraceEnSize-1:0]            esr_trace_type_hot_enable_i
);

  // Build alloc structs
  reqq_alloc_t alloc_l2_info, alloc_l3_info;
  always_comb begin
    alloc_l2_info = '0;
    alloc_l2_info.reqq_id           = alloc_l2_reqq_id_i;
    alloc_l2_info.port_id           = alloc_l2_port_id_i;
    alloc_l2_info.orig_opcode       = reqq_orig_opcode_e'(alloc_l2_orig_opcode_i);
    alloc_l2_info.address           = alloc_l2_address_i;
    alloc_l2_info.qwen              = alloc_l2_qwen_i;
    alloc_l2_info.traced            = alloc_l2_traced_i;
    alloc_l2_info.cache_level_start = cache_level_e'(alloc_l2_cache_level_start_i);
    alloc_l2_info.cache_level_dest  = cache_level_e'(alloc_l2_cache_level_dest_i);

    alloc_l3_info = '0;
    alloc_l3_info.reqq_id           = alloc_l3_reqq_id_i;
    alloc_l3_info.port_id           = alloc_l3_port_id_i;
    alloc_l3_info.orig_opcode       = reqq_orig_opcode_e'(alloc_l3_orig_opcode_i);
    alloc_l3_info.address           = alloc_l3_address_i;
    alloc_l3_info.qwen              = alloc_l3_qwen_i;
    alloc_l3_info.traced            = alloc_l3_traced_i;
  end

  // Build tag_rsp struct
  tag_rsp_t pipe_tag_rsp_info;
  always_comb begin
    pipe_tag_rsp_info = '0;
    pipe_tag_rsp_info.reqq_id          = pipe_tag_rsp_reqq_id_i;
    pipe_tag_rsp_info.hit              = pipe_tag_rsp_hit_i;
    pipe_tag_rsp_info.tag_ram_hit_way  = pipe_tag_rsp_way_i;
    pipe_tag_rsp_info.victim           = pipe_tag_rsp_victim_i;
    pipe_tag_rsp_info.orig_address     = pipe_tag_rsp_orig_address_i;
    pipe_tag_rsp_info.victim_address   = pipe_tag_rsp_victim_address_i;
    pipe_tag_rsp_info.victim_qwen      = pipe_tag_rsp_victim_qwen_i;
    pipe_tag_rsp_info.tag_ram_hit_qwen = pipe_tag_rsp_tag_ram_hit_qwen_i;
    pipe_tag_rsp_info.opcode           = reqq_opcode_e'(pipe_tag_rsp_opcode_i);
  end

  // Build rbuf_rd_req struct
  rbuf_rd_req_t pipe_rbuf_rd_req_info;
  always_comb begin
    pipe_rbuf_rd_req_info = '0;
    pipe_rbuf_rd_req_info.reqq_id = pipe_rbuf_rd_req_reqq_id_i;
  end

  // Build mesh master structs
  mesh_master_req_t to_l3_mm_req_info, to_sys_mm_req_info;
  mesh_master_rsp_t to_l3_mm_rsp_info, to_sys_mm_rsp_info;
  always_comb begin
    to_l3_mm_req_info    = '0;
    to_l3_mm_req_info.id = to_l3_mm_req_id_i;

    to_l3_mm_rsp_info    = '0;
    to_l3_mm_rsp_info.id = to_l3_mm_rsp_id_i;

    to_sys_mm_req_info    = '0;
    to_sys_mm_req_info.id = to_sys_mm_req_id_i;

    to_sys_mm_rsp_info    = '0;
    to_sys_mm_rsp_info.id = to_sys_mm_rsp_id_i;
  end

  // Build rspmux_sent_t
  rspmux_sent_t rspmux_l2_trace_sent;
  always_comb begin
    rspmux_l2_trace_sent.valid   = rspmux_l2_sent_valid_i;
    rspmux_l2_trace_sent.reqq_id = rspmux_l2_sent_reqq_id_i;
    rspmux_l2_trace_sent.traced  = rspmux_l2_sent_traced_i;
  end

  // Build traced_reqq_state
  reqq_entry_state_t traced_reqq_state;
  always_comb begin
    traced_reqq_state = '0;
    traced_reqq_state.valid                   = trs_valid_i;
    traced_reqq_state.reqq_id                 = trs_reqq_id_i;
    traced_reqq_state.opcode                  = reqq_opcode_e'(trs_opcode_i);
    traced_reqq_state.rspmux_l2_eligible      = trs_rspmux_l2_eligible_i;
    traced_reqq_state.rspmux_l3_eligible      = trs_rspmux_l3_eligible_i;
    traced_reqq_state.rspmux_sent             = trs_rspmux_sent_i;
    traced_reqq_state.pipe_req_eligible       = trs_pipe_req_eligible_i;
    traced_reqq_state.to_l3_mesh_req_eligible = trs_to_l3_mesh_req_eligible_i;
    traced_reqq_state.to_sys_mesh_req_eligible = trs_to_sys_mesh_req_eligible_i;
    traced_reqq_state.rbuf_req_eligible       = trs_rbuf_req_eligible_i;
    traced_reqq_state.inflight                = trs_inflight_i;
    traced_reqq_state.data_ready              = trs_data_ready_i;
    traced_reqq_state.wait_for_dataq          = trs_wait_for_dataq_i;
    traced_reqq_state.dep_valid               = trs_dep_valid_i;
    traced_reqq_state.dep_tail                = trs_dep_tail_i;
    traced_reqq_state.dep_meshll_tail         = trs_dep_meshll_tail_i;
    traced_reqq_state.dep_non_meshll_head     = trs_dep_non_meshll_head_i;
    traced_reqq_state.dont_depend_on_me       = trs_dont_depend_on_me_i;
    traced_reqq_state.rbuf_valid              = trs_rbuf_valid_i;
    traced_reqq_state.rbuf_pending_valid      = trs_rbuf_pending_valid_i;
    traced_reqq_state.err_detected            = trs_err_detected_i;
    traced_reqq_state.traced                  = trs_traced_i;
  end

  // Build reqq_state array
  /* verilator lint_off WIDTHCONCAT */
  reqq_entry_state_t [NumEntries-1:0] reqq_state;
  always_comb begin
    reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */
    reqq_state[rs0_idx_i].valid   = rs0_valid_i;
    reqq_state[rs0_idx_i].traced  = rs0_traced_i;
    reqq_state[rs0_idx_i].address = rs0_address_i;

    if (rs1_idx_i != rs0_idx_i) begin
      reqq_state[rs1_idx_i].valid   = rs1_valid_i;
      reqq_state[rs1_idx_i].traced  = rs1_traced_i;
      reqq_state[rs1_idx_i].address = rs1_address_i;
    end
  end

  // Build ESR control
  bank_esr_ctl_t esr_ctl;
  always_comb begin
    esr_ctl = '0;
    esr_ctl.esr_sc_trace_type_hot_enable = esr_trace_type_hot_enable_i;
  end

  // DUT outputs
  trace_packet_t bank_trace_data;
  logic          bank_trace_valid;

  shirecache_bank_trace #(
    .NumEntries   (NumEntries),
    .NumEntriesId (NumEntriesId)
  ) u_dut (
    .clk_free_i                           (clk_i),
    .rst_ni                               (rst_ni),
    .dft_i                                ('0),

    .bank_trace_data_o                    (bank_trace_data),
    .bank_trace_valid_o                   (bank_trace_valid),

    .alloc_l2_valid_ag_i                  (alloc_l2_valid_i),
    .alloc_l2_info_ag_i                   (alloc_l2_info),
    .alloc_l3_valid_ag_i                  (alloc_l3_valid_i),
    .alloc_l3_info_ag_i                   (alloc_l3_info),

    .pipe_tag_rsp_valid_i                 (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info_i                  (pipe_tag_rsp_info),

    .pipe_rbuf_rd_req_ready_i             (pipe_rbuf_rd_req_ready_i),
    .pipe_rbuf_rd_req_valid_i             (pipe_rbuf_rd_req_valid_i),
    .pipe_rbuf_rd_req_info_i              (pipe_rbuf_rd_req_info),

    .to_l3_mesh_master_bank_req_valid_i   (to_l3_mm_req_valid_i),
    .to_l3_mesh_master_bank_req_info_i    (to_l3_mm_req_info),
    .to_l3_mesh_master_bank_req_ready_i   (to_l3_mm_req_ready_i),
    .to_l3_mesh_master_bank_rsp_valid_i   (to_l3_mm_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_info_i    (to_l3_mm_rsp_info),
    .to_l3_mesh_master_bank_rsp_ready_i   (to_l3_mm_rsp_ready_i),
    .to_sys_mesh_master_bank_req_valid_i  (to_sys_mm_req_valid_i),
    .to_sys_mesh_master_bank_req_info_i   (to_sys_mm_req_info),
    .to_sys_mesh_master_bank_req_ready_i  (to_sys_mm_req_ready_i),
    .to_sys_mesh_master_bank_rsp_valid_i  (to_sys_mm_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_info_i   (to_sys_mm_rsp_info),
    .to_sys_mesh_master_bank_rsp_ready_i  (to_sys_mm_rsp_ready_i),

    .rspmux_l3_sent_valid_i               (rspmux_l3_sent_valid_i),
    .rspmux_l3_sent_id_i                  (rspmux_l3_sent_id_i),

    .rspmux_l2_trace_sent_i               (rspmux_l2_trace_sent),

    .traced_reqq_state_i                  (traced_reqq_state),

    .reqq_state_i                         (reqq_state),

    .esr_ctl_i                            (esr_ctl)
  );

  // Decompose outputs for C++
  assign bank_trace_valid_o = bank_trace_valid;
  assign bank_trace_seg0_o  = bank_trace_data.segment0;
  assign bank_trace_seg1_o  = bank_trace_data.segment1;
  assign bank_trace_seg2_o  = bank_trace_data.segment2;

  logic unused_ok;
  assign unused_ok = &{1'b0, bank_trace_data, unused_ok};

/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
endmodule
