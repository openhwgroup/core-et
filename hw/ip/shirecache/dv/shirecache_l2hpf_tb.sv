// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench for shirecache_bank_l2hpf with L2HpfImplemented=1.
// Decomposes tag_rsp_t and neigh_l2hpf_req_t to scalars.
// Reqq state array: set via indexed scalar inputs (1 entry at a time).

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
module shirecache_l2hpf_tb
  import shirecache_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries = ReqqDepth
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // Tag response (decomposed)
  input  logic                        pipe_tag_rsp_valid_i,
  input  logic [5:0]                  pipe_tag_rsp_opcode_i,
  input  logic [ReqqIdSize-1:0]       pipe_tag_rsp_reqq_id_i,
  input  logic [PaSize-1:0]           pipe_tag_rsp_orig_address_i,
  input  logic                        pipe_tag_rsp_hit_i,
  input  logic                        pipe_tag_rsp_victim_i,
  input  logic [4:0]                  pipe_tag_rsp_err_rsp_i,

  // Reqq state config (1 indexed entry)
  input  logic [ReqqIdSize-1:0]       rs_idx_i,
  input  logic [ReqqSourceSize-1:0]   rs_source_i,
  input  logic [ReqqTagIdSize-1:0]    rs_tag_id_i,
  input  logic [4:0]                  rs_orig_opcode_i,

  // L2 HPF req output (decomposed)
  output logic                        l2hpf_req_valid_o,
  output logic [SourceSize-1:0]       l2hpf_req_source_o,
  output logic [IdSize-1:0]           l2hpf_req_id_o,
  output logic [PaSize-1:0]           l2hpf_req_address_o,
  output logic [4:0]                  l2hpf_req_opcode_o,
  output logic                        l2hpf_req_hit_o
);

  // Build tag response struct
  tag_rsp_t pipe_tag_rsp_info;
  always_comb begin
    pipe_tag_rsp_info = '0;
    pipe_tag_rsp_info.opcode       = reqq_opcode_e'(pipe_tag_rsp_opcode_i);
    pipe_tag_rsp_info.reqq_id      = pipe_tag_rsp_reqq_id_i;
    pipe_tag_rsp_info.orig_address = pipe_tag_rsp_orig_address_i;
    pipe_tag_rsp_info.hit          = pipe_tag_rsp_hit_i;
    pipe_tag_rsp_info.victim       = pipe_tag_rsp_victim_i;
    pipe_tag_rsp_info.err_rsp      = err_rsp_e'(pipe_tag_rsp_err_rsp_i);
  end

  // Build reqq state array (set one entry via index)
  /* verilator lint_off WIDTHCONCAT */
  reqq_entry_state_t [NumEntries-1:0] reqq_state;
  always_comb begin
    reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */
    reqq_state[rs_idx_i].source      = rs_source_i;
    reqq_state[rs_idx_i].tag_id      = rs_tag_id_i;
    reqq_state[rs_idx_i].orig_opcode = reqq_orig_opcode_e'(rs_orig_opcode_i);
  end

  neigh_l2hpf_req_t l2hpf_req_info;

  shirecache_bank_l2hpf #(
    .NumEntries     (NumEntries),
    .L2HpfImplemented(1)
  ) u_dut (
    .clk_i, .rst_ni,
    .pipe_tag_rsp_valid_i,
    .pipe_tag_rsp_info_i  (pipe_tag_rsp_info),
    .l2hpf_req_valid_o,
    .l2hpf_req_info_o     (l2hpf_req_info),
    .reqq_state_i         (reqq_state)
  );

  // Decompose output struct
  assign l2hpf_req_source_o  = l2hpf_req_info.source;
  assign l2hpf_req_id_o      = l2hpf_req_info.id;
  assign l2hpf_req_address_o = l2hpf_req_info.address;
  assign l2hpf_req_opcode_o  = l2hpf_req_info.opcode;
  assign l2hpf_req_hit_o     = l2hpf_req_info.hit;

  logic unused_ok;
  assign unused_ok = &{1'b0, pipe_tag_rsp_info, reqq_state, l2hpf_req_info, unused_ok};

  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */
endmodule
