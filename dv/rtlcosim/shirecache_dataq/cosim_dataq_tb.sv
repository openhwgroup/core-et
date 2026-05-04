// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_dataq vs new shirecache_bank_dataq.
//
// Both modules receive identical stimulus. Outputs are compared cycle-by-cycle.
// The original uses active-high synchronous reset; the new uses active-low async.
//
// Struct outputs are decomposed to scalars so C++ can compare them field-by-field.
// Wide data fields (512b) are exported as-is for memcmp comparison.

`include "soc.vh"

module cosim_dataq_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  //// Write-side inputs ////

  // Pipeline write
  input  logic                        pipe_data_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_reqq_id_i,
  input  logic [DvTransIdSize-1:0]    pipe_data_rsp_trans_id_i,
  input  logic [LineQwSize-1:0]       pipe_data_rsp_qwen_i,
  input  logic [LineDataSize-1:0]     pipe_data_rsp_data_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_mod_dataq_id_i,
  input  logic                        mesh_bypassed_victim_i,
  input  logic                        rspmux_bypassed_data_rsp_i,

  // Bank L2
  input  logic                        bank_l2_valid_i,
  input  logic                        bank_l2_ready_i,
  input  logic [ReqqIdSize-1:0]       reqq_alloc_l2_id_ag_i,
  input  logic [DvTransIdSize-1:0]    bank_l2_trans_id_i,
  input  logic [4:0]                  bank_l2_opcode_i,
  input  logic [6:0]                  bank_l2_subopcode_i,
  input  logic [PaSize-1:0]           bank_l2_address_i,
  input  logic                        bank_l2_wdata_i,
  input  logic [LineDataSize-1:0]     bank_l2_data_i,
  input  logic [LineByteSize-1:0]     bank_l2_ben_i,
  input  logic [LineQwSize-1:0]       bank_l2_qwen_i,
  input  logic [2:0]                  bank_l2_size_i,
  input  logic                        bank_l2_write_is_partial_i,

  // to_l3 mesh write
  input  logic                        to_l3_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_l3_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_dataq_id_i,
  input  logic [LineDataSize-1:0]     to_l3_mesh_rsp_data_i,

  // Bank L3
  input  logic                        bank_l3_valid_i,
  input  logic                        bank_l3_ready_i,
  input  logic [ReqqIdSize-1:0]       reqq_alloc_l3_id_ag_i,
  input  logic [DvTransIdSize-1:0]    bank_l3_trans_id_i,
  input  logic [4:0]                  bank_l3_opcode_i,
  input  logic [LineDataSize-1:0]     bank_l3_data_i,
  input  logic [LineByteSize-1:0]     bank_l3_ben_i,
  input  logic [LineQwSize-1:0]       bank_l3_qwen_i,
  input  logic [2:0]                  bank_l3_size_i,
  input  logic                        bank_l3_wdata_i,
  input  logic                        bank_l3_write_is_partial_i,

  // to_sys mesh write
  input  logic                        to_sys_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_sys_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_dataq_id_i,
  input  logic [LineDataSize-1:0]     to_sys_mesh_rsp_data_i,

  //// Read-side inputs ////

  // Pipeline read
  input  logic                        pipe_dataq_rd_req_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_dataq_rd_req_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_dataq_rd_req_reqq_id_i,
  input  logic                        pipe_dataq_rd_req_partial_i,
  input  logic [LineQwSize-1:0]       pipe_dataq_rd_req_qwen_i,

  // to_l3_mesh read
  input  logic                        reqq_to_l3_mesh_req_valid_wdata_i,
  input  logic [DvTransIdSize-1:0]    reqq_to_l3_mesh_req_trans_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_to_l3_mesh_req_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_to_l3_mesh_req_dataq_id_i,
  input  logic [LineQwSize-1:0]       reqq_to_l3_mesh_req_qwen_i,
  input  logic                        reqq_to_l3_mesh_req_rd_ben_i,
  input  logic                        dataq_to_l3_mesh_req_ready_i,

  // rspmux_l2 read
  input  logic                        reqq_rspmux_l2_valid_wdata_i,
  input  logic [DvTransIdSize-1:0]    reqq_rspmux_l2_trans_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_rspmux_l2_reqq_id_i,
  input  logic [LineQwSize-1:0]       reqq_rspmux_l2_qwen_i,
  input  logic                        dataq_rspmux_l2_ready_i,

  // to_sys_mesh read
  input  logic                        reqq_to_sys_mesh_req_valid_wdata_i,
  input  logic [DvTransIdSize-1:0]    reqq_to_sys_mesh_req_trans_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_to_sys_mesh_req_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_to_sys_mesh_req_dataq_id_i,
  input  logic [LineQwSize-1:0]       reqq_to_sys_mesh_req_qwen_i,
  input  logic                        reqq_to_sys_mesh_req_rd_ben_i,
  input  logic                        dataq_to_sys_mesh_req_ready_i,

  // rspmux_l3 read
  input  logic                        reqq_rspmux_l3_valid_wdata_i,
  input  logic [DvTransIdSize-1:0]    reqq_rspmux_l3_trans_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_rspmux_l3_reqq_id_i,
  input  logic [LineQwSize-1:0]       reqq_rspmux_l3_qwen_i,
  input  logic                        dataq_rspmux_l3_ready_i,

  //// Scalar outputs from new module ////
  output logic                        new_dataq_bank_l2_ready_o,
  output logic                        new_dataq_bank_l3_ready_o,
  output logic                        new_to_l3_mesh_rsp_ready_o,
  output logic                        new_to_sys_mesh_rsp_ready_o,

  output logic                        new_dataq_write_status_any_o,
  output logic [ReqqDepth-1:0]        new_dataq_write_status_hot_o,
  output logic [ReqqIdSize-1:0]       new_dataq_write_status_id_o,
  output logic [ReqqDepth-1:0]        new_dataq_read_status_hot_o,

  // pipe rd rsp
  output logic [DvTransIdSize-1:0]    new_pipe_rd_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]       new_pipe_rd_rsp_reqq_id_o,
  output logic [LineDataSize-1:0]     new_pipe_rd_rsp_data_o,
  output logic [LineByteSize-1:0]     new_pipe_rd_rsp_ben_o,
  output logic                        new_pipe_rd_rsp_err_o,

  // to_l3_mesh read output
  output logic                        new_dataq_to_l3_mesh_req_valid_o,
  output logic [DvTransIdSize-1:0]    new_dataq_to_l3_mesh_req_trans_id_o,
  output logic [ReqqIdSize-1:0]       new_dataq_to_l3_mesh_req_reqq_id_o,
  output logic [LineDataSize-1:0]     new_dataq_to_l3_mesh_req_data_o,
  output logic [LineByteSize-1:0]     new_dataq_to_l3_mesh_req_ben_o,
  output logic                        new_reqq_to_l3_mesh_req_ready_wdata_o,

  // rspmux_l2
  output logic                        new_dataq_rspmux_l2_valid_o,
  output logic [DvTransIdSize-1:0]    new_dataq_rspmux_l2_trans_id_o,
  output logic [ReqqIdSize-1:0]       new_dataq_rspmux_l2_reqq_id_o,
  output logic [LineDataSize-1:0]     new_dataq_rspmux_l2_data_o,
  output logic                        new_reqq_rspmux_l2_ready_wdata_o,

  // to_sys_mesh read output
  output logic                        new_dataq_to_sys_mesh_req_valid_o,
  output logic [DvTransIdSize-1:0]    new_dataq_to_sys_mesh_req_trans_id_o,
  output logic [ReqqIdSize-1:0]       new_dataq_to_sys_mesh_req_reqq_id_o,
  output logic [LineDataSize-1:0]     new_dataq_to_sys_mesh_req_data_o,
  output logic [LineByteSize-1:0]     new_dataq_to_sys_mesh_req_ben_o,
  output logic                        new_reqq_to_sys_mesh_req_ready_wdata_o,

  // rspmux_l3
  output logic                        new_dataq_rspmux_l3_valid_o,
  output logic [DvTransIdSize-1:0]    new_dataq_rspmux_l3_trans_id_o,
  output logic [ReqqIdSize-1:0]       new_dataq_rspmux_l3_reqq_id_o,
  output logic [LineDataSize-1:0]     new_dataq_rspmux_l3_data_o,
  output logic                        new_reqq_rspmux_l3_ready_wdata_o,

  //// Scalar outputs from original module ////
  output logic                        orig_dataq_bank_l2_ready_o,
  output logic                        orig_dataq_bank_l3_ready_o,
  output logic                        orig_to_l3_mesh_rsp_ready_o,
  output logic                        orig_to_sys_mesh_rsp_ready_o,

  output logic                        orig_dataq_write_status_any_o,
  output logic [ReqqDepth-1:0]        orig_dataq_write_status_hot_o,
  output logic [ReqqIdSize-1:0]       orig_dataq_write_status_id_o,
  output logic [ReqqDepth-1:0]        orig_dataq_read_status_hot_o,

  // pipe rd rsp
  output logic [DvTransIdSize-1:0]    orig_pipe_rd_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]       orig_pipe_rd_rsp_reqq_id_o,
  output logic [LineDataSize-1:0]     orig_pipe_rd_rsp_data_o,
  output logic [LineByteSize-1:0]     orig_pipe_rd_rsp_ben_o,
  output logic                        orig_pipe_rd_rsp_err_o,

  // to_l3_mesh read output
  output logic                        orig_dataq_to_l3_mesh_req_valid_o,
  output logic [DvTransIdSize-1:0]    orig_dataq_to_l3_mesh_req_trans_id_o,
  output logic [ReqqIdSize-1:0]       orig_dataq_to_l3_mesh_req_reqq_id_o,
  output logic [LineDataSize-1:0]     orig_dataq_to_l3_mesh_req_data_o,
  output logic [LineByteSize-1:0]     orig_dataq_to_l3_mesh_req_ben_o,
  output logic                        orig_reqq_to_l3_mesh_req_ready_wdata_o,

  // rspmux_l2
  output logic                        orig_dataq_rspmux_l2_valid_o,
  output logic [DvTransIdSize-1:0]    orig_dataq_rspmux_l2_trans_id_o,
  output logic [ReqqIdSize-1:0]       orig_dataq_rspmux_l2_reqq_id_o,
  output logic [LineDataSize-1:0]     orig_dataq_rspmux_l2_data_o,
  output logic                        orig_reqq_rspmux_l2_ready_wdata_o,

  // to_sys_mesh read output
  output logic                        orig_dataq_to_sys_mesh_req_valid_o,
  output logic [DvTransIdSize-1:0]    orig_dataq_to_sys_mesh_req_trans_id_o,
  output logic [ReqqIdSize-1:0]       orig_dataq_to_sys_mesh_req_reqq_id_o,
  output logic [LineDataSize-1:0]     orig_dataq_to_sys_mesh_req_data_o,
  output logic [LineByteSize-1:0]     orig_dataq_to_sys_mesh_req_ben_o,
  output logic                        orig_reqq_to_sys_mesh_req_ready_wdata_o,

  // rspmux_l3
  output logic                        orig_dataq_rspmux_l3_valid_o,
  output logic [DvTransIdSize-1:0]    orig_dataq_rspmux_l3_trans_id_o,
  output logic [ReqqIdSize-1:0]       orig_dataq_rspmux_l3_reqq_id_o,
  output logic [LineDataSize-1:0]     orig_dataq_rspmux_l3_data_o,
  output logic                        orig_reqq_rspmux_l3_ready_wdata_o
);

  // Active-high synchronous reset for original
  wire reset = !rst_ni;

  // ── Build structured types from scalar inputs ────────────────────

  // Pipe data response
  data_rsp_t new_pipe_data_rsp_info;
  always_comb begin
    new_pipe_data_rsp_info            = '0;
    new_pipe_data_rsp_info.trans_id   = pipe_data_rsp_trans_id_i;
    new_pipe_data_rsp_info.reqq_id    = pipe_data_rsp_reqq_id_i;
    new_pipe_data_rsp_info.qwen       = pipe_data_rsp_qwen_i;
    new_pipe_data_rsp_info.data       = pipe_data_rsp_data_i;
  end

  sc_pipe_data_rsp_t orig_pipe_data_rsp_info;
  always_comb begin
    orig_pipe_data_rsp_info            = '0;
    orig_pipe_data_rsp_info.trans_id   = pipe_data_rsp_trans_id_i;
    orig_pipe_data_rsp_info.reqq_id    = pipe_data_rsp_reqq_id_i;
    orig_pipe_data_rsp_info.qwen       = pipe_data_rsp_qwen_i;
    orig_pipe_data_rsp_info.data       = pipe_data_rsp_data_i;
  end

  // Pipe data response modifier
  data_rsp_modifier_t new_pipe_data_rsp_modifier;
  always_comb begin
    new_pipe_data_rsp_modifier           = '0;
    new_pipe_data_rsp_modifier.dataq_id  = pipe_data_rsp_mod_dataq_id_i;
  end

  sc_pipe_data_rsp_modifier_t orig_pipe_data_rsp_modifier;
  always_comb begin
    orig_pipe_data_rsp_modifier           = '0;
    orig_pipe_data_rsp_modifier.dataq_id  = pipe_data_rsp_mod_dataq_id_i;
  end

  // Bank L2
  reqq_bank_req_t new_bank_l2_info;
  always_comb begin
    new_bank_l2_info                    = '0;
    new_bank_l2_info.trans_id           = bank_l2_trans_id_i;
    new_bank_l2_info.opcode             = etlink_pkg::req_opcode_e'(bank_l2_opcode_i);
    new_bank_l2_info.subopcode          = bank_l2_subopcode_i;
    new_bank_l2_info.address            = bank_l2_address_i;
    new_bank_l2_info.wdata              = bank_l2_wdata_i;
    new_bank_l2_info.data               = bank_l2_data_i;
    new_bank_l2_info.ben                = bank_l2_ben_i;
    new_bank_l2_info.qwen               = bank_l2_qwen_i;
    new_bank_l2_info.size               = sc_size_t'(bank_l2_size_i);
    new_bank_l2_info.write_is_partial   = bank_l2_write_is_partial_i;
  end

  sc_reqq_bank_req_t orig_bank_l2_info;
  always_comb begin
    orig_bank_l2_info                    = '0;
    orig_bank_l2_info.trans_id           = bank_l2_trans_id_i;
    orig_bank_l2_info.opcode             = et_link_req_opcode_t'(bank_l2_opcode_i);
    orig_bank_l2_info.subopcode          = bank_l2_subopcode_i;
    orig_bank_l2_info.address            = bank_l2_address_i;
    orig_bank_l2_info.wdata              = bank_l2_wdata_i;
    orig_bank_l2_info.data               = bank_l2_data_i;
    orig_bank_l2_info.ben                = bank_l2_ben_i;
    orig_bank_l2_info.qwen               = bank_l2_qwen_i;
    orig_bank_l2_info.size               = et_link_size_t'(bank_l2_size_i);
    orig_bank_l2_info.write_is_partial   = bank_l2_write_is_partial_i;
  end

  // to_l3_mesh response
  reqq_mesh_rsp_t new_to_l3_mesh_rsp_info;
  always_comb begin
    new_to_l3_mesh_rsp_info           = '0;
    new_to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    new_to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    new_to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    new_to_l3_mesh_rsp_info.data      = to_l3_mesh_rsp_data_i;
  end

  sc_reqq_mesh_rsp_t orig_to_l3_mesh_rsp_info;
  always_comb begin
    orig_to_l3_mesh_rsp_info           = '0;
    orig_to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    orig_to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    orig_to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    orig_to_l3_mesh_rsp_info.data      = to_l3_mesh_rsp_data_i;
  end

  // Bank L3
  reqq_bank_req_t new_bank_l3_info;
  always_comb begin
    new_bank_l3_info                    = '0;
    new_bank_l3_info.trans_id           = bank_l3_trans_id_i;
    new_bank_l3_info.opcode             = etlink_pkg::req_opcode_e'(bank_l3_opcode_i);
    new_bank_l3_info.wdata              = bank_l3_wdata_i;
    new_bank_l3_info.data               = bank_l3_data_i;
    new_bank_l3_info.ben                = bank_l3_ben_i;
    new_bank_l3_info.qwen               = bank_l3_qwen_i;
    new_bank_l3_info.size               = sc_size_t'(bank_l3_size_i);
    new_bank_l3_info.write_is_partial   = bank_l3_write_is_partial_i;
  end

  sc_reqq_bank_req_t orig_bank_l3_info;
  always_comb begin
    orig_bank_l3_info                    = '0;
    orig_bank_l3_info.trans_id           = bank_l3_trans_id_i;
    orig_bank_l3_info.opcode             = et_link_req_opcode_t'(bank_l3_opcode_i);
    orig_bank_l3_info.wdata              = bank_l3_wdata_i;
    orig_bank_l3_info.data               = bank_l3_data_i;
    orig_bank_l3_info.ben                = bank_l3_ben_i;
    orig_bank_l3_info.qwen               = bank_l3_qwen_i;
    orig_bank_l3_info.size               = et_link_size_t'(bank_l3_size_i);
    orig_bank_l3_info.write_is_partial   = bank_l3_write_is_partial_i;
  end

  // to_sys_mesh response
  reqq_mesh_rsp_t new_to_sys_mesh_rsp_info;
  always_comb begin
    new_to_sys_mesh_rsp_info           = '0;
    new_to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    new_to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    new_to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    new_to_sys_mesh_rsp_info.data      = to_sys_mesh_rsp_data_i;
  end

  sc_reqq_mesh_rsp_t orig_to_sys_mesh_rsp_info;
  always_comb begin
    orig_to_sys_mesh_rsp_info           = '0;
    orig_to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    orig_to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    orig_to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    orig_to_sys_mesh_rsp_info.data      = to_sys_mesh_rsp_data_i;
  end

  // Pipeline read request
  dataq_rd_req_t new_pipe_dataq_rd_req;
  always_comb begin
    new_pipe_dataq_rd_req.trans_id = pipe_dataq_rd_req_trans_id_i;
    new_pipe_dataq_rd_req.reqq_id  = pipe_dataq_rd_req_reqq_id_i;
    new_pipe_dataq_rd_req.partial  = pipe_dataq_rd_req_partial_i;
    new_pipe_dataq_rd_req.qwen     = pipe_dataq_rd_req_qwen_i;
  end

  sc_pipe_dataq_rd_req_t orig_pipe_dataq_rd_req;
  always_comb begin
    orig_pipe_dataq_rd_req.trans_id = pipe_dataq_rd_req_trans_id_i;
    orig_pipe_dataq_rd_req.reqq_id  = pipe_dataq_rd_req_reqq_id_i;
    orig_pipe_dataq_rd_req.partial  = pipe_dataq_rd_req_partial_i;
    orig_pipe_dataq_rd_req.qwen     = pipe_dataq_rd_req_qwen_i;
  end

  // to_l3_mesh read request
  reqq_mesh_req_t new_reqq_to_l3_mesh_req_info;
  always_comb begin
    new_reqq_to_l3_mesh_req_info          = '0;
    new_reqq_to_l3_mesh_req_info.trans_id = reqq_to_l3_mesh_req_trans_id_i;
    new_reqq_to_l3_mesh_req_info.reqq_id  = reqq_to_l3_mesh_req_reqq_id_i;
    new_reqq_to_l3_mesh_req_info.dataq_id = reqq_to_l3_mesh_req_dataq_id_i;
    new_reqq_to_l3_mesh_req_info.qwen     = reqq_to_l3_mesh_req_qwen_i;
    new_reqq_to_l3_mesh_req_info.rd_ben   = reqq_to_l3_mesh_req_rd_ben_i;
  end

  sc_reqq_mesh_req_t orig_reqq_to_l3_mesh_req_info;
  always_comb begin
    orig_reqq_to_l3_mesh_req_info          = '0;
    orig_reqq_to_l3_mesh_req_info.trans_id = reqq_to_l3_mesh_req_trans_id_i;
    orig_reqq_to_l3_mesh_req_info.reqq_id  = reqq_to_l3_mesh_req_reqq_id_i;
    orig_reqq_to_l3_mesh_req_info.dataq_id = reqq_to_l3_mesh_req_dataq_id_i;
    orig_reqq_to_l3_mesh_req_info.qwen     = reqq_to_l3_mesh_req_qwen_i;
    orig_reqq_to_l3_mesh_req_info.rd_ben   = reqq_to_l3_mesh_req_rd_ben_i;
  end

  // rspmux_l2 read request
  rspmux_t new_reqq_rspmux_l2_info;
  always_comb begin
    new_reqq_rspmux_l2_info          = '0;
    new_reqq_rspmux_l2_info.trans_id = reqq_rspmux_l2_trans_id_i;
    new_reqq_rspmux_l2_info.reqq_id  = reqq_rspmux_l2_reqq_id_i;
    new_reqq_rspmux_l2_info.qwen     = reqq_rspmux_l2_qwen_i;
  end

  sc_rspmux_t orig_reqq_rspmux_l2_info;
  always_comb begin
    orig_reqq_rspmux_l2_info          = '0;
    orig_reqq_rspmux_l2_info.trans_id = reqq_rspmux_l2_trans_id_i;
    orig_reqq_rspmux_l2_info.reqq_id  = reqq_rspmux_l2_reqq_id_i;
    orig_reqq_rspmux_l2_info.qwen     = reqq_rspmux_l2_qwen_i;
  end

  // to_sys_mesh read request
  reqq_mesh_req_t new_reqq_to_sys_mesh_req_info;
  always_comb begin
    new_reqq_to_sys_mesh_req_info          = '0;
    new_reqq_to_sys_mesh_req_info.trans_id = reqq_to_sys_mesh_req_trans_id_i;
    new_reqq_to_sys_mesh_req_info.reqq_id  = reqq_to_sys_mesh_req_reqq_id_i;
    new_reqq_to_sys_mesh_req_info.dataq_id = reqq_to_sys_mesh_req_dataq_id_i;
    new_reqq_to_sys_mesh_req_info.qwen     = reqq_to_sys_mesh_req_qwen_i;
    new_reqq_to_sys_mesh_req_info.rd_ben   = reqq_to_sys_mesh_req_rd_ben_i;
  end

  sc_reqq_mesh_req_t orig_reqq_to_sys_mesh_req_info;
  always_comb begin
    orig_reqq_to_sys_mesh_req_info          = '0;
    orig_reqq_to_sys_mesh_req_info.trans_id = reqq_to_sys_mesh_req_trans_id_i;
    orig_reqq_to_sys_mesh_req_info.reqq_id  = reqq_to_sys_mesh_req_reqq_id_i;
    orig_reqq_to_sys_mesh_req_info.dataq_id = reqq_to_sys_mesh_req_dataq_id_i;
    orig_reqq_to_sys_mesh_req_info.qwen     = reqq_to_sys_mesh_req_qwen_i;
    orig_reqq_to_sys_mesh_req_info.rd_ben   = reqq_to_sys_mesh_req_rd_ben_i;
  end

  // rspmux_l3 read request
  rspmux_t new_reqq_rspmux_l3_info;
  always_comb begin
    new_reqq_rspmux_l3_info          = '0;
    new_reqq_rspmux_l3_info.trans_id = reqq_rspmux_l3_trans_id_i;
    new_reqq_rspmux_l3_info.reqq_id  = reqq_rspmux_l3_reqq_id_i;
    new_reqq_rspmux_l3_info.qwen     = reqq_rspmux_l3_qwen_i;
  end

  sc_rspmux_t orig_reqq_rspmux_l3_info;
  always_comb begin
    orig_reqq_rspmux_l3_info          = '0;
    orig_reqq_rspmux_l3_info.trans_id = reqq_rspmux_l3_trans_id_i;
    orig_reqq_rspmux_l3_info.reqq_id  = reqq_rspmux_l3_reqq_id_i;
    orig_reqq_rspmux_l3_info.qwen     = reqq_rspmux_l3_qwen_i;
  end

  // ESR ctl (tie off unused)
  bank_esr_ctl_t new_esr_ctl;
  assign new_esr_ctl = '0;

  sc_bank_esr_ctl_t orig_esr_ctl;
  assign orig_esr_ctl = '0;

  // ── New module ────────────────────────────────────────────────

  dft_t new_dft;
  assign new_dft = '0;

  dataq_rd_rsp_t new_pipe_dataq_rd_rsp;
  reqq_mesh_req_t new_dataq_to_l3_mesh_req_info;
  rspmux_t new_dataq_rspmux_l2_info;
  reqq_mesh_req_t new_dataq_to_sys_mesh_req_info;
  rspmux_t new_dataq_rspmux_l3_info;

  shirecache_bank_dataq u_new (
    .clk_i                               (clk_i),
    .rst_ni                              (rst_ni),

    // Pipeline write
    .pipe_data_rsp_valid_i               (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info_i                (new_pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i       (new_pipe_data_rsp_modifier),
    .mesh_bypassed_victim_i              (mesh_bypassed_victim_i),
    .rspmux_bypassed_data_rsp_i          (rspmux_bypassed_data_rsp_i),

    // Bank L2
    .dataq_bank_l2_ready_o               (new_dataq_bank_l2_ready_o),
    .bank_l2_valid_i                     (bank_l2_valid_i),
    .bank_l2_ready_i                     (bank_l2_ready_i),
    .bank_l2_info_i                      (new_bank_l2_info),
    .reqq_alloc_l2_id_ag_i               (reqq_alloc_l2_id_ag_i),

    // to_l3 mesh write
    .to_l3_mesh_rsp_valid_i              (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info_i               (new_to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready_o              (new_to_l3_mesh_rsp_ready_o),

    // Bank L3
    .dataq_bank_l3_ready_o               (new_dataq_bank_l3_ready_o),
    .bank_l3_valid_i                     (bank_l3_valid_i),
    .bank_l3_ready_i                     (bank_l3_ready_i),
    .bank_l3_info_i                      (new_bank_l3_info),
    .reqq_alloc_l3_id_ag_i               (reqq_alloc_l3_id_ag_i),

    // to_sys mesh write
    .to_sys_mesh_rsp_valid_i             (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info_i              (new_to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_o             (new_to_sys_mesh_rsp_ready_o),

    // Pipeline read
    .pipe_dataq_rd_req_valid_i           (pipe_dataq_rd_req_valid_i),
    .pipe_dataq_rd_req_info_i            (new_pipe_dataq_rd_req),
    .pipe_dataq_rd_rsp_info_o            (new_pipe_dataq_rd_rsp),

    // to_l3_mesh read
    .reqq_to_l3_mesh_req_valid_wdata_i   (reqq_to_l3_mesh_req_valid_wdata_i),
    .reqq_to_l3_mesh_req_info_i          (new_reqq_to_l3_mesh_req_info),
    .reqq_to_l3_mesh_req_ready_wdata_o   (new_reqq_to_l3_mesh_req_ready_wdata_o),
    .dataq_to_l3_mesh_req_valid_o        (new_dataq_to_l3_mesh_req_valid_o),
    .dataq_to_l3_mesh_req_info_o         (new_dataq_to_l3_mesh_req_info),
    .dataq_to_l3_mesh_req_ready_i        (dataq_to_l3_mesh_req_ready_i),

    // rspmux_l2 read
    .reqq_rspmux_l2_valid_wdata_i        (reqq_rspmux_l2_valid_wdata_i),
    .reqq_rspmux_l2_info_i               (new_reqq_rspmux_l2_info),
    .reqq_rspmux_l2_ready_wdata_o        (new_reqq_rspmux_l2_ready_wdata_o),
    .dataq_rspmux_l2_valid_o             (new_dataq_rspmux_l2_valid_o),
    .dataq_rspmux_l2_info_o              (new_dataq_rspmux_l2_info),
    .dataq_rspmux_l2_ready_i             (dataq_rspmux_l2_ready_i),

    // to_sys_mesh read
    .reqq_to_sys_mesh_req_valid_wdata_i  (reqq_to_sys_mesh_req_valid_wdata_i),
    .reqq_to_sys_mesh_req_info_i         (new_reqq_to_sys_mesh_req_info),
    .reqq_to_sys_mesh_req_ready_wdata_o  (new_reqq_to_sys_mesh_req_ready_wdata_o),
    .dataq_to_sys_mesh_req_valid_o       (new_dataq_to_sys_mesh_req_valid_o),
    .dataq_to_sys_mesh_req_info_o        (new_dataq_to_sys_mesh_req_info),
    .dataq_to_sys_mesh_req_ready_i       (dataq_to_sys_mesh_req_ready_i),

    // rspmux_l3 read
    .reqq_rspmux_l3_valid_wdata_i        (reqq_rspmux_l3_valid_wdata_i),
    .reqq_rspmux_l3_info_i               (new_reqq_rspmux_l3_info),
    .reqq_rspmux_l3_ready_wdata_o        (new_reqq_rspmux_l3_ready_wdata_o),
    .dataq_rspmux_l3_valid_o             (new_dataq_rspmux_l3_valid_o),
    .dataq_rspmux_l3_info_o              (new_dataq_rspmux_l3_info),
    .dataq_rspmux_l3_ready_i             (dataq_rspmux_l3_ready_i),

    // Write/read status
    .dataq_write_status_any_o            (new_dataq_write_status_any_o),
    .dataq_write_status_hot_o            (new_dataq_write_status_hot_o),
    .dataq_write_status_id_o             (new_dataq_write_status_id_o),
    .dataq_read_status_hot_o             (new_dataq_read_status_hot_o),

    .esr_ctl_i                           (new_esr_ctl),
    .dft_i                               (new_dft)
  );

  // Decompose new module struct outputs
  assign new_pipe_rd_rsp_trans_id_o          = new_pipe_dataq_rd_rsp.trans_id;
  assign new_pipe_rd_rsp_reqq_id_o           = new_pipe_dataq_rd_rsp.reqq_id;
  assign new_pipe_rd_rsp_data_o              = new_pipe_dataq_rd_rsp.data;
  assign new_pipe_rd_rsp_ben_o               = new_pipe_dataq_rd_rsp.ben;
  assign new_pipe_rd_rsp_err_o               = new_pipe_dataq_rd_rsp.err;

  assign new_dataq_to_l3_mesh_req_trans_id_o = new_dataq_to_l3_mesh_req_info.trans_id;
  assign new_dataq_to_l3_mesh_req_reqq_id_o  = new_dataq_to_l3_mesh_req_info.reqq_id;
  assign new_dataq_to_l3_mesh_req_data_o     = new_dataq_to_l3_mesh_req_info.data;
  assign new_dataq_to_l3_mesh_req_ben_o      = new_dataq_to_l3_mesh_req_info.ben;

  assign new_dataq_rspmux_l2_trans_id_o      = new_dataq_rspmux_l2_info.trans_id;
  assign new_dataq_rspmux_l2_reqq_id_o       = new_dataq_rspmux_l2_info.reqq_id;
  assign new_dataq_rspmux_l2_data_o          = new_dataq_rspmux_l2_info.data;

  assign new_dataq_to_sys_mesh_req_trans_id_o= new_dataq_to_sys_mesh_req_info.trans_id;
  assign new_dataq_to_sys_mesh_req_reqq_id_o = new_dataq_to_sys_mesh_req_info.reqq_id;
  assign new_dataq_to_sys_mesh_req_data_o    = new_dataq_to_sys_mesh_req_info.data;
  assign new_dataq_to_sys_mesh_req_ben_o     = new_dataq_to_sys_mesh_req_info.ben;

  assign new_dataq_rspmux_l3_trans_id_o      = new_dataq_rspmux_l3_info.trans_id;
  assign new_dataq_rspmux_l3_reqq_id_o       = new_dataq_rspmux_l3_info.reqq_id;
  assign new_dataq_rspmux_l3_data_o          = new_dataq_rspmux_l3_info.data;

  // ── Original module ───────────────────────────────────────────

  sc_pipe_dataq_rd_rsp_t orig_pipe_dataq_rd_rsp;
  sc_reqq_mesh_req_t orig_dataq_to_l3_mesh_req_info;
  sc_rspmux_t orig_dataq_rspmux_l2_info;
  sc_reqq_mesh_req_t orig_dataq_to_sys_mesh_req_info;
  sc_rspmux_t orig_dataq_rspmux_l3_info;

  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  shire_cache_bank_dataq u_orig (
    .clock                               (clk_i),
    .reset_in                            (reset),

    // Pipeline write
    .pipe_data_rsp_valid                 (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info                  (orig_pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info         (orig_pipe_data_rsp_modifier),
    .mesh_bypassed_victim                (mesh_bypassed_victim_i),
    .rspmux_bypassed_data_rsp            (rspmux_bypassed_data_rsp_i),

    // Bank L2
    .dataq_bank_l2_ready                 (orig_dataq_bank_l2_ready_o),
    .bank_l2_valid                       (bank_l2_valid_i),
    .bank_l2_ready                       (bank_l2_ready_i),
    .bank_l2_info                        (orig_bank_l2_info),
    .reqq_alloc_l2_id_ag                 (reqq_alloc_l2_id_ag_i),

    // to_l3 mesh write
    .to_l3_mesh_rsp_valid                (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info                 (orig_to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready                (orig_to_l3_mesh_rsp_ready_o),

    // Bank L3
    .dataq_bank_l3_ready                 (orig_dataq_bank_l3_ready_o),
    .bank_l3_valid                       (bank_l3_valid_i),
    .bank_l3_ready                       (bank_l3_ready_i),
    .bank_l3_info                        (orig_bank_l3_info),
    .reqq_alloc_l3_id_ag                 (reqq_alloc_l3_id_ag_i),

    // to_sys mesh write
    .to_sys_mesh_rsp_valid               (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info                (orig_to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready               (orig_to_sys_mesh_rsp_ready_o),

    // Pipeline read
    .pipe_dataq_rd_req_valid             (pipe_dataq_rd_req_valid_i),
    .pipe_dataq_rd_req_info              (orig_pipe_dataq_rd_req),
    .pipe_dataq_rd_rsp_info              (orig_pipe_dataq_rd_rsp),

    // to_l3_mesh read
    .reqq_to_l3_mesh_req_valid_wdata     (reqq_to_l3_mesh_req_valid_wdata_i),
    .reqq_to_l3_mesh_req_info            (orig_reqq_to_l3_mesh_req_info),
    .reqq_to_l3_mesh_req_ready_wdata     (orig_reqq_to_l3_mesh_req_ready_wdata_o),
    .dataq_to_l3_mesh_req_valid          (orig_dataq_to_l3_mesh_req_valid_o),
    .dataq_to_l3_mesh_req_info           (orig_dataq_to_l3_mesh_req_info),
    .dataq_to_l3_mesh_req_ready          (dataq_to_l3_mesh_req_ready_i),

    // rspmux_l2 read
    .reqq_rspmux_l2_valid_wdata          (reqq_rspmux_l2_valid_wdata_i),
    .reqq_rspmux_l2_info                 (orig_reqq_rspmux_l2_info),
    .reqq_rspmux_l2_ready_wdata          (orig_reqq_rspmux_l2_ready_wdata_o),
    .dataq_rspmux_l2_valid               (orig_dataq_rspmux_l2_valid_o),
    .dataq_rspmux_l2_info                (orig_dataq_rspmux_l2_info),
    .dataq_rspmux_l2_ready               (dataq_rspmux_l2_ready_i),

    // to_sys_mesh read
    .reqq_to_sys_mesh_req_valid_wdata    (reqq_to_sys_mesh_req_valid_wdata_i),
    .reqq_to_sys_mesh_req_info           (orig_reqq_to_sys_mesh_req_info),
    .reqq_to_sys_mesh_req_ready_wdata    (orig_reqq_to_sys_mesh_req_ready_wdata_o),
    .dataq_to_sys_mesh_req_valid         (orig_dataq_to_sys_mesh_req_valid_o),
    .dataq_to_sys_mesh_req_info          (orig_dataq_to_sys_mesh_req_info),
    .dataq_to_sys_mesh_req_ready         (dataq_to_sys_mesh_req_ready_i),

    // rspmux_l3 read
    .reqq_rspmux_l3_valid_wdata          (reqq_rspmux_l3_valid_wdata_i),
    .reqq_rspmux_l3_info                 (orig_reqq_rspmux_l3_info),
    .reqq_rspmux_l3_ready_wdata          (orig_reqq_rspmux_l3_ready_wdata_o),
    .dataq_rspmux_l3_valid               (orig_dataq_rspmux_l3_valid_o),
    .dataq_rspmux_l3_info                (orig_dataq_rspmux_l3_info),
    .dataq_rspmux_l3_ready               (dataq_rspmux_l3_ready_i),

    // Write/read status
    .dataq_write_status_any              (orig_dataq_write_status_any_o),
    .dataq_write_status_hot              (orig_dataq_write_status_hot_o),
    .dataq_write_status_id               (orig_dataq_write_status_id_o),
    .dataq_read_status_hot               (orig_dataq_read_status_hot_o),

    .esr_ctl                             (orig_esr_ctl),
    .esr_shire_cache_ram_cfg             (orig_ram_cfg),
    .dft__reset_byp_hv                   (1'b0),
    .dft__reset_hv                       (1'b0)
  );

  // Decompose original module struct outputs
  assign orig_pipe_rd_rsp_trans_id_o          = orig_pipe_dataq_rd_rsp.trans_id;
  assign orig_pipe_rd_rsp_reqq_id_o           = orig_pipe_dataq_rd_rsp.reqq_id;
  assign orig_pipe_rd_rsp_data_o              = orig_pipe_dataq_rd_rsp.data;
  assign orig_pipe_rd_rsp_ben_o               = orig_pipe_dataq_rd_rsp.ben;
  assign orig_pipe_rd_rsp_err_o               = orig_pipe_dataq_rd_rsp.err;

  assign orig_dataq_to_l3_mesh_req_trans_id_o = orig_dataq_to_l3_mesh_req_info.trans_id;
  assign orig_dataq_to_l3_mesh_req_reqq_id_o  = orig_dataq_to_l3_mesh_req_info.reqq_id;
  assign orig_dataq_to_l3_mesh_req_data_o     = orig_dataq_to_l3_mesh_req_info.data;
  assign orig_dataq_to_l3_mesh_req_ben_o      = orig_dataq_to_l3_mesh_req_info.ben;

  assign orig_dataq_rspmux_l2_trans_id_o      = orig_dataq_rspmux_l2_info.trans_id;
  assign orig_dataq_rspmux_l2_reqq_id_o       = orig_dataq_rspmux_l2_info.reqq_id;
  assign orig_dataq_rspmux_l2_data_o          = orig_dataq_rspmux_l2_info.data;

  assign orig_dataq_to_sys_mesh_req_trans_id_o= orig_dataq_to_sys_mesh_req_info.trans_id;
  assign orig_dataq_to_sys_mesh_req_reqq_id_o = orig_dataq_to_sys_mesh_req_info.reqq_id;
  assign orig_dataq_to_sys_mesh_req_data_o    = orig_dataq_to_sys_mesh_req_info.data;
  assign orig_dataq_to_sys_mesh_req_ben_o     = orig_dataq_to_sys_mesh_req_info.ben;

  assign orig_dataq_rspmux_l3_trans_id_o      = orig_dataq_rspmux_l3_info.trans_id;
  assign orig_dataq_rspmux_l3_reqq_id_o       = orig_dataq_rspmux_l3_info.reqq_id;
  assign orig_dataq_rspmux_l3_data_o          = orig_dataq_rspmux_l3_info.data;

endmodule : cosim_dataq_tb
