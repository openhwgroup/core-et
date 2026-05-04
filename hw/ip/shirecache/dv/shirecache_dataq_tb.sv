// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_dataq.
// Breaks packed struct ports into scalars for easy C++ driving.

/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields intentionally unused
module shirecache_dataq_tb
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

  //// Scalar outputs ////

  // Write-side ready
  output logic                        dataq_bank_l2_ready_o,
  output logic                        dataq_bank_l3_ready_o,
  output logic                        to_l3_mesh_rsp_ready_o,
  output logic                        to_sys_mesh_rsp_ready_o,

  // Write/read status
  output logic                        dataq_write_status_any_o,
  output logic [ReqqDepth-1:0]        dataq_write_status_hot_o,
  output logic [ReqqIdSize-1:0]       dataq_write_status_id_o,
  output logic [ReqqDepth-1:0]        dataq_read_status_hot_o,

  // Pipeline read response
  output logic [DvTransIdSize-1:0]    pipe_rd_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]       pipe_rd_rsp_reqq_id_o,
  output logic [LineDataSize-1:0]     pipe_rd_rsp_data_o,
  output logic [LineByteSize-1:0]     pipe_rd_rsp_ben_o,
  output logic                        pipe_rd_rsp_err_o,

  // to_l3_mesh read output
  output logic                        dataq_to_l3_mesh_req_valid_o,
  output logic [DvTransIdSize-1:0]    dataq_to_l3_mesh_req_trans_id_o,
  output logic [ReqqIdSize-1:0]       dataq_to_l3_mesh_req_reqq_id_o,
  output logic [LineDataSize-1:0]     dataq_to_l3_mesh_req_data_o,
  output logic [LineByteSize-1:0]     dataq_to_l3_mesh_req_ben_o,
  output logic                        reqq_to_l3_mesh_req_ready_wdata_o,

  // rspmux_l2
  output logic                        dataq_rspmux_l2_valid_o,
  output logic [DvTransIdSize-1:0]    dataq_rspmux_l2_trans_id_o,
  output logic [ReqqIdSize-1:0]       dataq_rspmux_l2_reqq_id_o,
  output logic [LineDataSize-1:0]     dataq_rspmux_l2_data_o,
  output logic                        reqq_rspmux_l2_ready_wdata_o,

  // to_sys_mesh read output
  output logic                        dataq_to_sys_mesh_req_valid_o,
  output logic [DvTransIdSize-1:0]    dataq_to_sys_mesh_req_trans_id_o,
  output logic [ReqqIdSize-1:0]       dataq_to_sys_mesh_req_reqq_id_o,
  output logic [LineDataSize-1:0]     dataq_to_sys_mesh_req_data_o,
  output logic [LineByteSize-1:0]     dataq_to_sys_mesh_req_ben_o,
  output logic                        reqq_to_sys_mesh_req_ready_wdata_o,

  // rspmux_l3
  output logic                        dataq_rspmux_l3_valid_o,
  output logic [DvTransIdSize-1:0]    dataq_rspmux_l3_trans_id_o,
  output logic [ReqqIdSize-1:0]       dataq_rspmux_l3_reqq_id_o,
  output logic [LineDataSize-1:0]     dataq_rspmux_l3_data_o,
  output logic                        reqq_rspmux_l3_ready_wdata_o
);

  // ── Build structured types from scalar inputs ────────────────────

  data_rsp_t pipe_data_rsp_info;
  always_comb begin
    pipe_data_rsp_info            = '0;
    pipe_data_rsp_info.trans_id   = pipe_data_rsp_trans_id_i;
    pipe_data_rsp_info.reqq_id    = pipe_data_rsp_reqq_id_i;
    pipe_data_rsp_info.qwen       = pipe_data_rsp_qwen_i;
    pipe_data_rsp_info.data       = pipe_data_rsp_data_i;
  end

  data_rsp_modifier_t pipe_data_rsp_modifier;
  always_comb begin
    pipe_data_rsp_modifier           = '0;
    pipe_data_rsp_modifier.dataq_id  = pipe_data_rsp_mod_dataq_id_i;
  end

  // Bank L2
  reqq_bank_req_t bank_l2_info;
  always_comb begin
    bank_l2_info                    = '0;
    bank_l2_info.trans_id           = bank_l2_trans_id_i;
    bank_l2_info.opcode             = etlink_pkg::req_opcode_e'(bank_l2_opcode_i);
    bank_l2_info.subopcode          = bank_l2_subopcode_i;
    bank_l2_info.address            = bank_l2_address_i;
    bank_l2_info.wdata              = bank_l2_wdata_i;
    bank_l2_info.data               = bank_l2_data_i;
    bank_l2_info.ben                = bank_l2_ben_i;
    bank_l2_info.qwen               = bank_l2_qwen_i;
    bank_l2_info.size               = sc_size_t'(bank_l2_size_i);
    bank_l2_info.write_is_partial   = bank_l2_write_is_partial_i;
  end

  // to_l3 mesh response
  reqq_mesh_rsp_t to_l3_mesh_rsp_info;
  always_comb begin
    to_l3_mesh_rsp_info             = '0;
    to_l3_mesh_rsp_info.trans_id    = to_l3_mesh_rsp_trans_id_i;
    to_l3_mesh_rsp_info.reqq_id     = to_l3_mesh_rsp_reqq_id_i;
    to_l3_mesh_rsp_info.dataq_id    = to_l3_mesh_rsp_dataq_id_i;
    to_l3_mesh_rsp_info.data        = to_l3_mesh_rsp_data_i;
  end

  // Bank L3
  reqq_bank_req_t bank_l3_info;
  always_comb begin
    bank_l3_info                    = '0;
    bank_l3_info.trans_id           = bank_l3_trans_id_i;
    bank_l3_info.opcode             = etlink_pkg::req_opcode_e'(bank_l3_opcode_i);
    bank_l3_info.wdata              = bank_l3_wdata_i;
    bank_l3_info.data               = bank_l3_data_i;
    bank_l3_info.ben                = bank_l3_ben_i;
    bank_l3_info.qwen               = bank_l3_qwen_i;
    bank_l3_info.size               = sc_size_t'(bank_l3_size_i);
    bank_l3_info.write_is_partial   = bank_l3_write_is_partial_i;
  end

  // to_sys mesh response
  reqq_mesh_rsp_t to_sys_mesh_rsp_info;
  always_comb begin
    to_sys_mesh_rsp_info             = '0;
    to_sys_mesh_rsp_info.trans_id    = to_sys_mesh_rsp_trans_id_i;
    to_sys_mesh_rsp_info.reqq_id     = to_sys_mesh_rsp_reqq_id_i;
    to_sys_mesh_rsp_info.dataq_id    = to_sys_mesh_rsp_dataq_id_i;
    to_sys_mesh_rsp_info.data        = to_sys_mesh_rsp_data_i;
  end

  // Pipeline read request
  dataq_rd_req_t pipe_dataq_rd_req_info;
  always_comb begin
    pipe_dataq_rd_req_info.trans_id = pipe_dataq_rd_req_trans_id_i;
    pipe_dataq_rd_req_info.reqq_id  = pipe_dataq_rd_req_reqq_id_i;
    pipe_dataq_rd_req_info.partial  = pipe_dataq_rd_req_partial_i;
    pipe_dataq_rd_req_info.qwen     = pipe_dataq_rd_req_qwen_i;
  end

  // to_l3_mesh read request
  reqq_mesh_req_t reqq_to_l3_mesh_req_info;
  always_comb begin
    reqq_to_l3_mesh_req_info          = '0;
    reqq_to_l3_mesh_req_info.trans_id = reqq_to_l3_mesh_req_trans_id_i;
    reqq_to_l3_mesh_req_info.reqq_id  = reqq_to_l3_mesh_req_reqq_id_i;
    reqq_to_l3_mesh_req_info.dataq_id = reqq_to_l3_mesh_req_dataq_id_i;
    reqq_to_l3_mesh_req_info.qwen     = reqq_to_l3_mesh_req_qwen_i;
    reqq_to_l3_mesh_req_info.rd_ben   = reqq_to_l3_mesh_req_rd_ben_i;
  end

  // rspmux_l2 read request
  rspmux_t reqq_rspmux_l2_info;
  always_comb begin
    reqq_rspmux_l2_info             = '0;
    reqq_rspmux_l2_info.trans_id    = reqq_rspmux_l2_trans_id_i;
    reqq_rspmux_l2_info.reqq_id     = reqq_rspmux_l2_reqq_id_i;
    reqq_rspmux_l2_info.qwen        = reqq_rspmux_l2_qwen_i;
  end

  // to_sys_mesh read request
  reqq_mesh_req_t reqq_to_sys_mesh_req_info;
  always_comb begin
    reqq_to_sys_mesh_req_info          = '0;
    reqq_to_sys_mesh_req_info.trans_id = reqq_to_sys_mesh_req_trans_id_i;
    reqq_to_sys_mesh_req_info.reqq_id  = reqq_to_sys_mesh_req_reqq_id_i;
    reqq_to_sys_mesh_req_info.dataq_id = reqq_to_sys_mesh_req_dataq_id_i;
    reqq_to_sys_mesh_req_info.qwen     = reqq_to_sys_mesh_req_qwen_i;
    reqq_to_sys_mesh_req_info.rd_ben   = reqq_to_sys_mesh_req_rd_ben_i;
  end

  // rspmux_l3 read request
  rspmux_t reqq_rspmux_l3_info;
  always_comb begin
    reqq_rspmux_l3_info             = '0;
    reqq_rspmux_l3_info.trans_id    = reqq_rspmux_l3_trans_id_i;
    reqq_rspmux_l3_info.reqq_id     = reqq_rspmux_l3_reqq_id_i;
    reqq_rspmux_l3_info.qwen        = reqq_rspmux_l3_qwen_i;
  end

  // ── DUT output structs ──────────────────────────────────────────

  dataq_rd_rsp_t  pipe_dataq_rd_rsp_info;
  reqq_mesh_req_t dataq_to_l3_mesh_req_info;
  rspmux_t        dataq_rspmux_l2_info;
  reqq_mesh_req_t dataq_to_sys_mesh_req_info;
  rspmux_t        dataq_rspmux_l3_info;

  // ── Decompose struct outputs to scalars ─────────────────────────

  // Pipeline read response
  assign pipe_rd_rsp_trans_id_o = pipe_dataq_rd_rsp_info.trans_id;
  assign pipe_rd_rsp_reqq_id_o  = pipe_dataq_rd_rsp_info.reqq_id;
  assign pipe_rd_rsp_data_o     = pipe_dataq_rd_rsp_info.data;
  assign pipe_rd_rsp_ben_o      = pipe_dataq_rd_rsp_info.ben;
  assign pipe_rd_rsp_err_o      = pipe_dataq_rd_rsp_info.err;

  // to_l3_mesh read output
  assign dataq_to_l3_mesh_req_trans_id_o = dataq_to_l3_mesh_req_info.trans_id;
  assign dataq_to_l3_mesh_req_reqq_id_o  = dataq_to_l3_mesh_req_info.reqq_id;
  assign dataq_to_l3_mesh_req_data_o     = dataq_to_l3_mesh_req_info.data;
  assign dataq_to_l3_mesh_req_ben_o      = dataq_to_l3_mesh_req_info.ben;

  // rspmux_l2
  assign dataq_rspmux_l2_trans_id_o = dataq_rspmux_l2_info.trans_id;
  assign dataq_rspmux_l2_reqq_id_o  = dataq_rspmux_l2_info.reqq_id;
  assign dataq_rspmux_l2_data_o     = dataq_rspmux_l2_info.data;

  // to_sys_mesh read output
  assign dataq_to_sys_mesh_req_trans_id_o = dataq_to_sys_mesh_req_info.trans_id;
  assign dataq_to_sys_mesh_req_reqq_id_o  = dataq_to_sys_mesh_req_info.reqq_id;
  assign dataq_to_sys_mesh_req_data_o     = dataq_to_sys_mesh_req_info.data;
  assign dataq_to_sys_mesh_req_ben_o      = dataq_to_sys_mesh_req_info.ben;

  // rspmux_l3
  assign dataq_rspmux_l3_trans_id_o = dataq_rspmux_l3_info.trans_id;
  assign dataq_rspmux_l3_reqq_id_o  = dataq_rspmux_l3_info.reqq_id;
  assign dataq_rspmux_l3_data_o     = dataq_rspmux_l3_info.data;

  // ── ESR / DFT tie-offs ──────────────────────────────────────────

  bank_esr_ctl_t esr_ctl;
  assign esr_ctl = '0;

  dft_t dft;
  assign dft = '0;

  // ── DUT instantiation ───────────────────────────────────────────

  shirecache_bank_dataq u_dut (
    .clk_i                              (clk_i),
    .rst_ni                             (rst_ni),

    // Pipeline write
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i      (pipe_data_rsp_modifier),
    .mesh_bypassed_victim_i             (mesh_bypassed_victim_i),
    .rspmux_bypassed_data_rsp_i         (rspmux_bypassed_data_rsp_i),

    // Bank L2
    .dataq_bank_l2_ready_o              (dataq_bank_l2_ready_o),
    .bank_l2_valid_i                    (bank_l2_valid_i),
    .bank_l2_ready_i                    (bank_l2_ready_i),
    .bank_l2_info_i                     (bank_l2_info),
    .reqq_alloc_l2_id_ag_i              (reqq_alloc_l2_id_ag_i),

    // to_l3 mesh write
    .to_l3_mesh_rsp_valid_i             (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info_i              (to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready_o             (to_l3_mesh_rsp_ready_o),

    // Bank L3
    .dataq_bank_l3_ready_o              (dataq_bank_l3_ready_o),
    .bank_l3_valid_i                    (bank_l3_valid_i),
    .bank_l3_ready_i                    (bank_l3_ready_i),
    .bank_l3_info_i                     (bank_l3_info),
    .reqq_alloc_l3_id_ag_i              (reqq_alloc_l3_id_ag_i),

    // to_sys mesh write
    .to_sys_mesh_rsp_valid_i            (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info_i             (to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_o            (to_sys_mesh_rsp_ready_o),

    // Pipeline read
    .pipe_dataq_rd_req_valid_i          (pipe_dataq_rd_req_valid_i),
    .pipe_dataq_rd_req_info_i           (pipe_dataq_rd_req_info),
    .pipe_dataq_rd_rsp_info_o           (pipe_dataq_rd_rsp_info),

    // to_l3_mesh read
    .reqq_to_l3_mesh_req_valid_wdata_i  (reqq_to_l3_mesh_req_valid_wdata_i),
    .reqq_to_l3_mesh_req_info_i         (reqq_to_l3_mesh_req_info),
    .reqq_to_l3_mesh_req_ready_wdata_o  (reqq_to_l3_mesh_req_ready_wdata_o),
    .dataq_to_l3_mesh_req_valid_o       (dataq_to_l3_mesh_req_valid_o),
    .dataq_to_l3_mesh_req_info_o        (dataq_to_l3_mesh_req_info),
    .dataq_to_l3_mesh_req_ready_i       (dataq_to_l3_mesh_req_ready_i),

    // rspmux_l2
    .reqq_rspmux_l2_valid_wdata_i       (reqq_rspmux_l2_valid_wdata_i),
    .reqq_rspmux_l2_info_i              (reqq_rspmux_l2_info),
    .reqq_rspmux_l2_ready_wdata_o       (reqq_rspmux_l2_ready_wdata_o),
    .dataq_rspmux_l2_valid_o            (dataq_rspmux_l2_valid_o),
    .dataq_rspmux_l2_info_o             (dataq_rspmux_l2_info),
    .dataq_rspmux_l2_ready_i            (dataq_rspmux_l2_ready_i),

    // to_sys_mesh read
    .reqq_to_sys_mesh_req_valid_wdata_i (reqq_to_sys_mesh_req_valid_wdata_i),
    .reqq_to_sys_mesh_req_info_i        (reqq_to_sys_mesh_req_info),
    .reqq_to_sys_mesh_req_ready_wdata_o (reqq_to_sys_mesh_req_ready_wdata_o),
    .dataq_to_sys_mesh_req_valid_o      (dataq_to_sys_mesh_req_valid_o),
    .dataq_to_sys_mesh_req_info_o       (dataq_to_sys_mesh_req_info),
    .dataq_to_sys_mesh_req_ready_i      (dataq_to_sys_mesh_req_ready_i),

    // rspmux_l3
    .reqq_rspmux_l3_valid_wdata_i       (reqq_rspmux_l3_valid_wdata_i),
    .reqq_rspmux_l3_info_i              (reqq_rspmux_l3_info),
    .reqq_rspmux_l3_ready_wdata_o       (reqq_rspmux_l3_ready_wdata_o),
    .dataq_rspmux_l3_valid_o            (dataq_rspmux_l3_valid_o),
    .dataq_rspmux_l3_info_o             (dataq_rspmux_l3_info),
    .dataq_rspmux_l3_ready_i            (dataq_rspmux_l3_ready_i),

    // Write/read status
    .dataq_write_status_any_o           (dataq_write_status_any_o),
    .dataq_write_status_hot_o           (dataq_write_status_hot_o),
    .dataq_write_status_id_o            (dataq_write_status_id_o),
    .dataq_read_status_hot_o            (dataq_read_status_hot_o),

    // ESR / DFT
    .esr_ctl_i                          (esr_ctl),
    .dft_i                              (dft)
  );

endmodule : shirecache_dataq_tb
