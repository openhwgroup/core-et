// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_pipe_atomic.
// Breaks packed struct ports into scalars for easy C++ driving.

// TB constructs structs internally; unused intermediate signals expected.
/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields intentionally unused
module shirecache_atomic_tb
  import shirecache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // Simplified pipe inputs
  input  logic                        pipe_valid_i,
  input  logic                        tag_hit_i,
  input  logic                        victim_i,
  input  logic [SetIdSize-1:0]        pipe_set_i,
  input  logic [WayIdSize-1:0]        hit_way_i,
  input  logic                        data_rsp_zero_i,

  // Simplified request
  input  logic [5:0]                  opcode_i,     // reqq_opcode_e
  input  logic [LineQwSize-1:0]       qwen_i,
  input  logic [PaSize-1:0]           address_i,

  // Full-width data
  input  logic [LineDataSize-1:0]     pipe_data_i,
  input  logic [LineDataSize-1:0]     atomic_operands_i,
  input  logic [LineByteSize-1:0]     partial_wr_data_ben_i,

  // Outputs
  output logic [LineDataSize-1:0]     atomic_data_rsp_data_o,
  output logic                        result_wr_en_o,
  output logic [DataRamAddrSize-1:0]  result_wr_addr_o,
  output logic [LineDataSize-1:0]     result_wr_data_o,
  output logic [LineQwSize-1:0]       result_wr_qwen_o
);

  pipe_info_t pipe_info;
  assign pipe_info = '0;

  // Override specific fields via a second always_comb
  pipe_info_t pipe_info_patched;
  always_comb begin
    pipe_info_patched                     = '0;
    pipe_info_patched.pipe_set            = pipe_set_i;
    pipe_info_patched.tag_hit             = tag_hit_i;
    pipe_info_patched.tag_ram_hit_way     = hit_way_i;
    pipe_info_patched.victim              = victim_i;
    pipe_info_patched.data_rsp_zero       = data_rsp_zero_i;
  end

  reqq_pipe_req_t pipe_req_info;
  always_comb begin
    pipe_req_info             = '0;
    pipe_req_info.opcode      = reqq_opcode_e'(opcode_i);
    pipe_req_info.qwen        = qwen_i;
    pipe_req_info.address     = address_i;
  end

  shirecache_pipe_atomic u_dut (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .pipe_valid_i           (pipe_valid_i),
    .pipe_info_i            (pipe_info_patched),
    .pipe_req_info_i        (pipe_req_info),
    .pipe_data_i            (pipe_data_i),
    .atomic_operands_i      (atomic_operands_i),
    .partial_wr_data_ben_i  (partial_wr_data_ben_i),
    .atomic_data_rsp_data_o (atomic_data_rsp_data_o),
    .atomic_result_wr_en_o  (result_wr_en_o),
    .atomic_result_wr_addr_o(result_wr_addr_o),
    .atomic_result_wr_data_o(result_wr_data_o),
    .atomic_result_wr_qwen_o(result_wr_qwen_o)
  );

endmodule : shirecache_atomic_tb
