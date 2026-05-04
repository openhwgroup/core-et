// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_atomic vs new shirecache_pipe_atomic.

`include "soc.vh"

module cosim_atomic_tb
  import shirecache_pkg::*;
(
  input  logic                                clk_i,
  input  logic                                rst_ni,

  // Shared inputs
  input  logic                                pipe_valid_i,
  input  logic                                tag_hit_i,
  input  logic                                victim_i,
  input  logic [`SC_SET_ID_SIZE-1:0]          pipe_set_i,
  input  logic [`SC_WAY_ID_SIZE-1:0]          hit_way_i,
  input  logic                                data_rsp_zero_i,
  input  logic [5:0]                          opcode_i,
  input  logic [`SC_LINE_QW_SIZE-1:0]         qwen_i,
  input  logic [`PA_SIZE-1:0]                 address_i,
  input  logic [`SC_LINE_DATA_SIZE-1:0]       pipe_data_i,
  input  logic [`SC_LINE_DATA_SIZE-1:0]       atomic_operands_i,
  input  logic [`SC_LINE_BYTE_SIZE-1:0]       partial_wr_data_ben_i,

  // New outputs
  output logic [`SC_LINE_DATA_SIZE-1:0]       new_data_rsp_o,
  output logic                                new_wr_en_o,
  output logic [`SC_DATA_RAM_ADDR_SIZE-1:0]   new_wr_addr_o,
  output logic [`SC_LINE_DATA_SIZE-1:0]       new_wr_data_o,
  output logic [`SC_LINE_QW_SIZE-1:0]         new_wr_qwen_o,

  // Original outputs
  output logic [`SC_LINE_DATA_SIZE-1:0]       orig_data_rsp_o,
  output logic                                orig_wr_en_o,
  output logic [`SC_DATA_RAM_ADDR_SIZE-1:0]   orig_wr_addr_o,
  output logic [`SC_LINE_DATA_SIZE-1:0]       orig_wr_data_o,
  output logic [`SC_LINE_QW_SIZE-1:0]         orig_wr_qwen_o
);

  wire reset = !rst_ni;

  // ── Build pipe_info for new module ────────────────────
  pipe_info_t new_pipe_info;
  always_comb begin
    new_pipe_info                     = '0;
    new_pipe_info.pipe_set            = pipe_set_i;
    new_pipe_info.tag_hit             = tag_hit_i;
    new_pipe_info.tag_ram_hit_way     = hit_way_i;
    new_pipe_info.victim              = victim_i;
    new_pipe_info.data_rsp_zero       = data_rsp_zero_i;
  end

  reqq_pipe_req_t new_req;
  always_comb begin
    new_req         = '0;
    new_req.opcode  = reqq_opcode_e'(opcode_i);
    new_req.qwen    = qwen_i;
    new_req.address = address_i;
  end

  shirecache_pipe_atomic u_new (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .pipe_valid_i           (pipe_valid_i),
    .pipe_info_i            (new_pipe_info),
    .pipe_req_info_i        (new_req),
    .pipe_data_i            (pipe_data_i),
    .atomic_operands_i      (atomic_operands_i),
    .partial_wr_data_ben_i  (partial_wr_data_ben_i),
    .atomic_data_rsp_data_o (new_data_rsp_o),
    .atomic_result_wr_en_o  (new_wr_en_o),
    .atomic_result_wr_addr_o(new_wr_addr_o),
    .atomic_result_wr_data_o(new_wr_data_o),
    .atomic_result_wr_qwen_o(new_wr_qwen_o)
  );

  // ── Build pipe_info for original module ───────────────
  sc_pipe_info_t orig_pipe_info;
  always_comb begin
    orig_pipe_info                    = '0;
    orig_pipe_info.pipe_set           = pipe_set_i;
    orig_pipe_info.tag_hit            = tag_hit_i;
    orig_pipe_info.tag_ram_hit_way    = hit_way_i;
    orig_pipe_info.victim             = victim_i;
    orig_pipe_info.data_rsp_zero      = data_rsp_zero_i;
  end

  sc_reqq_pipe_req_t orig_req;
  always_comb begin
    orig_req         = '0;
    orig_req.opcode  = sc_reqq_opcode_t'(opcode_i);
    orig_req.qwen    = qwen_i;
    orig_req.address = address_i;
  end

  shire_cache_pipe_atomic u_orig (
    .clock                  (clk_i),
    .reset                  (reset),
    .pipe_valid             (pipe_valid_i),
    .pipe_info              (orig_pipe_info),
    .pipe_req_info          (orig_req),
    .pipe_data              (pipe_data_i),
    .atomic_operands        (atomic_operands_i),
    .partial_wr_data_ben    (partial_wr_data_ben_i),
    .atomic_data_rsp_data   (orig_data_rsp_o),
    .atomic_result_wr_en    (orig_wr_en_o),
    .atomic_result_wr_addr  (orig_wr_addr_o),
    .atomic_result_wr_data  (orig_wr_data_o),
    .atomic_result_wr_qwen  (orig_wr_qwen_o)
  );

endmodule : cosim_atomic_tb
