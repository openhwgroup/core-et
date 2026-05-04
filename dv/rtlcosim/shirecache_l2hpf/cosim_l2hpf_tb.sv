// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_l2hpf vs new shirecache_bank_l2hpf.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.
//
// The tag_rsp and reqq_state structs are decomposed into scalars at the
// TB boundary to avoid exposing packed-struct layout to C++.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
module cosim_l2hpf_tb
  import shirecache_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries      = ReqqDepth,
  parameter bit          L2HpfImplemented = 1
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Tag response scalars ──────────────────────────────────
  input  logic                        pipe_tag_rsp_valid_i,
  input  logic [5:0]                  pipe_tag_rsp_opcode_i,
  input  logic [ReqqIdSize-1:0]       pipe_tag_rsp_reqq_id_i,
  input  logic [PaSize-1:0]           pipe_tag_rsp_orig_address_i,
  input  logic                        pipe_tag_rsp_hit_i,
  input  logic                        pipe_tag_rsp_victim_i,
  input  logic [4:0]                  pipe_tag_rsp_err_rsp_i,

  // ── Reqq state entry 0 ────────────────────────────────────
  input  logic [ReqqIdSize-1:0]       rs_idx0_i,
  input  logic [ReqqSourceSize-1:0]   rs_source0_i,
  input  logic [ReqqTagIdSize-1:0]    rs_tag_id0_i,
  input  logic [4:0]                  rs_orig_opcode0_i,

  // ── Reqq state entry 1 ────────────────────────────────────
  input  logic [ReqqIdSize-1:0]       rs_idx1_i,
  input  logic [ReqqSourceSize-1:0]   rs_source1_i,
  input  logic [ReqqTagIdSize-1:0]    rs_tag_id1_i,
  input  logic [4:0]                  rs_orig_opcode1_i,

  // ── Outputs (original) ────────────────────────────────────
  output logic                        orig_l2hpf_req_valid_o,
  output logic [etlink_pkg::SourceSize-1:0] orig_source_o,
  output logic [etlink_pkg::IdSize-1:0]     orig_id_o,
  output logic [PaSize-1:0]                 orig_address_o,
  output logic [4:0]                        orig_opcode_o,
  output logic                              orig_hit_o,

  // ── Outputs (new) ─────────────────────────────────────────
  output logic                        new_l2hpf_req_valid_o,
  output logic [etlink_pkg::SourceSize-1:0] new_source_o,
  output logic [etlink_pkg::IdSize-1:0]     new_id_o,
  output logic [PaSize-1:0]                 new_address_o,
  output logic [4:0]                        new_opcode_o,
  output logic                              new_hit_o
);

  // ════════════════════════════════════════════════════════════
  // Build tag_rsp structs from scalars
  // ════════════════════════════════════════════════════════════

  tag_rsp_t            new_tag_rsp;
  sc_pipe_tag_rsp_t    orig_tag_rsp;

  always_comb begin
    new_tag_rsp = '0;
    new_tag_rsp.opcode       = reqq_opcode_e'(pipe_tag_rsp_opcode_i);
    new_tag_rsp.reqq_id      = pipe_tag_rsp_reqq_id_i;
    new_tag_rsp.orig_address = pipe_tag_rsp_orig_address_i;
    new_tag_rsp.hit          = pipe_tag_rsp_hit_i;
    new_tag_rsp.victim       = pipe_tag_rsp_victim_i;
    new_tag_rsp.err_rsp      = err_rsp_e'(pipe_tag_rsp_err_rsp_i);

    orig_tag_rsp = '0;
    orig_tag_rsp.opcode       = sc_reqq_opcode_t'(pipe_tag_rsp_opcode_i);
    orig_tag_rsp.reqq_id      = pipe_tag_rsp_reqq_id_i;
    orig_tag_rsp.orig_address = pipe_tag_rsp_orig_address_i;
    orig_tag_rsp.hit          = pipe_tag_rsp_hit_i;
    orig_tag_rsp.victim       = pipe_tag_rsp_victim_i;
    orig_tag_rsp.err_rsp      = sc_err_rsp_t'(pipe_tag_rsp_err_rsp_i);
  end

  // ════════════════════════════════════════════════════════════
  // Build reqq_state arrays from scalars
  // ════════════════════════════════════════════════════════════

  reqq_entry_state_t [NumEntries-1:0]       reqq_state;
  sc_reqq_entry_state_t [NumEntries-1:0]    orig_reqq_state;

  always_comb begin
    reqq_state = '0;
    orig_reqq_state = '0;

    // Entry 0
    reqq_state[rs_idx0_i].source      = rs_source0_i;
    reqq_state[rs_idx0_i].tag_id      = rs_tag_id0_i;
    reqq_state[rs_idx0_i].orig_opcode = reqq_orig_opcode_e'(rs_orig_opcode0_i);

    orig_reqq_state[rs_idx0_i].source      = rs_source0_i;
    orig_reqq_state[rs_idx0_i].tag_id      = rs_tag_id0_i;
    orig_reqq_state[rs_idx0_i].orig_opcode = sc_reqq_orig_opcode_t'(rs_orig_opcode0_i);

    // Entry 1
    reqq_state[rs_idx1_i].source      = rs_source1_i;
    reqq_state[rs_idx1_i].tag_id      = rs_tag_id1_i;
    reqq_state[rs_idx1_i].orig_opcode = reqq_orig_opcode_e'(rs_orig_opcode1_i);

    orig_reqq_state[rs_idx1_i].source      = rs_source1_i;
    orig_reqq_state[rs_idx1_i].tag_id      = rs_tag_id1_i;
    orig_reqq_state[rs_idx1_i].orig_opcode = sc_reqq_orig_opcode_t'(rs_orig_opcode1_i);
  end

  // ════════════════════════════════════════════════════════════
  // Original module instantiation
  // ════════════════════════════════════════════════════════════

  logic                       orig_l2hpf_req_valid;
  sc_neigh_l2hpf_req_t        orig_l2hpf_req_info;

  shire_cache_bank_l2hpf #(
    .NUM_ENTRIES       (NumEntries),
    .L2HPF_IMPLEMENTED (L2HpfImplemented)
  ) u_orig (
    .clock                    (clk_i),
    .reset                    (~rst_ni),

    .pipe_tag_rsp_valid       (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info        (orig_tag_rsp),

    .sc_neigh_l2hpf_req_valid (orig_l2hpf_req_valid),
    .sc_neigh_l2hpf_req_info  (orig_l2hpf_req_info),

    .reqq_state               (orig_reqq_state)
  );

  // ════════════════════════════════════════════════════════════
  // New module instantiation
  // ════════════════════════════════════════════════════════════

  logic                       new_l2hpf_req_valid;
  neigh_l2hpf_req_t           new_l2hpf_req_info;

  shirecache_bank_l2hpf #(
    .NumEntries       (NumEntries),
    .L2HpfImplemented (L2HpfImplemented)
  ) u_new (
    .clk_i,
    .rst_ni,

    .pipe_tag_rsp_valid_i     (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info_i      (new_tag_rsp),

    .l2hpf_req_valid_o        (new_l2hpf_req_valid),
    .l2hpf_req_info_o         (new_l2hpf_req_info),

    .reqq_state_i             (reqq_state)
  );

  // ════════════════════════════════════════════════════════════
  // Decompose outputs to scalars for C++
  // ════════════════════════════════════════════════════════════

  // Original
  assign orig_l2hpf_req_valid_o = orig_l2hpf_req_valid;
  assign orig_source_o          = orig_l2hpf_req_info.source;
  assign orig_id_o              = orig_l2hpf_req_info.id;
  assign orig_address_o         = orig_l2hpf_req_info.address;
  assign orig_opcode_o          = orig_l2hpf_req_info.opcode;
  assign orig_hit_o             = orig_l2hpf_req_info.hit;

  // New
  assign new_l2hpf_req_valid_o  = new_l2hpf_req_valid;
  assign new_source_o           = new_l2hpf_req_info.source;
  assign new_id_o               = new_l2hpf_req_info.id;
  assign new_address_o          = new_l2hpf_req_info.address;
  assign new_opcode_o           = new_l2hpf_req_info.opcode;
  assign new_hit_o              = new_l2hpf_req_info.hit;

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDPARAM */
  /* verilator lint_on VARHIDDEN */

endmodule : cosim_l2hpf_tb
