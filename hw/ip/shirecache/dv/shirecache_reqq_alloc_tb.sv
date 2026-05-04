// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_reqq_alloc.
// Breaks packed struct ports into scalars for easy C++ driving.
// reqq_state and rbuf/tag_rsp interfaces are tied off to inactive
// (no valid entries, no rbuf/tag match) to simplify unit testing.

/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields intentionally unused
/* verilator lint_off UNOPTFLAT */     // alloc_info struct fields written by independent always_comb blocks
module shirecache_reqq_alloc_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Bank request (flat scalars -> reqq_bank_req_t) ──────────
  input  logic                        bank_valid_i,
  input  logic [4:0]                  bank_opcode_i,
  input  logic [6:0]                  bank_subopcode_i,
  input  logic [PaSize-1:0]          bank_address_i,
  input  logic [LineDataSize-1:0]    bank_data_i,
  input  logic [LineQwSize-1:0]      bank_qwen_i,
  input  logic [2:0]                  bank_size_i,
  input  logic [PortIdSize-1:0]      bank_port_id_i,
  input  logic [ReqqSourceSize-1:0]  bank_source_i,
  input  logic [DvTransIdSize-1:0]   bank_trans_id_i,
  input  logic                        bank_write_is_partial_i,
  input  logic [AxiAxqosSize-1:0]    bank_qos_i,
  input  logic [LineByteSize-1:0]    bank_ben_i,
  input  logic [ReqqTagIdSize-1:0]   bank_id_i,
  input  logic                        bank_wdata_i,

  // ── Dataq ready ─────────────────────────────────────────────
  input  logic                        dataq_bank_ready_i,

  // ── Idx CacheOp FSM ─────────────────────────────────────────
  input  logic                        pipe_idx_cop_req_valid_i,
  input  logic [5:0]                  pipe_idx_cop_req_opcode_i,
  input  logic [PaSize-1:0]          pipe_idx_cop_req_address_i,
  input  logic [DvTransIdSize-1:0]   pipe_idx_cop_req_trans_id_i,
  input  logic                        pipe_idx_cop_req_sync_flush_victims_i,
  input  logic                        pipe_idx_cop_l2_busy_i,
  input  logic                        pipe_idx_cop_l3_busy_i,
  input  logic                        pipe_idx_cop_aborted_i,

  // ── Controls ────────────────────────────────────────────────
  input  logic                        mbist_on_i,
  input  logic                        esr_wr_reqq_entries_i,

  // ── Scalar outputs ──────────────────────────────────────────
  output logic                        bank_ready_o,
  output logic                        reqq_full_o,
  output logic                        alloc_valid_ag_o,
  output logic [4:0]                  alloc_orig_opcode_o,
  output logic [5:0]                  alloc_opcode_o,
  output logic [2:0]                  alloc_address_space_o,
  output logic                        alloc_err_valid_o,
  output logic [4:0]                  alloc_err_type_o,
  output logic                        alloc_dep_valid_o,
  output logic [ReqqIdSize-1:0]      alloc_dep_id_o,
  output logic [ReqqIdSize-1:0]      reqq_alloc_id_ag_o,
  output logic                        pipe_idx_cop_req_ready_o,
  output logic                        alloc_valid_ad_o,
  output logic [ReqqDepth-1:0]       alloc_1hot_ag_o,
  output logic [1:0]                  alloc_reqq_source_o,
  output logic [PaSize-1:0]          alloc_address_o,
  output logic [LineQwSize-1:0]      alloc_qwen_o
);

  // ── Build bank_info from scalars ────────────────────────────
  reqq_bank_req_t bank_info;
  always_comb begin
    bank_info                    = '0;
    bank_info.opcode             = etlink_pkg::req_opcode_e'(bank_opcode_i);
    bank_info.subopcode          = bank_subopcode_i;
    bank_info.address            = bank_address_i;
    bank_info.data               = bank_data_i;
    bank_info.qwen               = bank_qwen_i;
    bank_info.size               = sc_size_t'(bank_size_i);
    bank_info.port_id            = bank_port_id_i;
    bank_info.source             = bank_source_i;
    bank_info.trans_id           = bank_trans_id_i;
    bank_info.write_is_partial   = bank_write_is_partial_i;
    bank_info.qos                = bank_qos_i;
    bank_info.ben                = bank_ben_i;
    bank_info.id                 = bank_id_i;
    bank_info.wdata              = bank_wdata_i;
  end

  // ── Build idx_cop_req_info from scalars ─────────────────────
  idx_cop_req_t idx_cop_req_info;
  always_comb begin
    idx_cop_req_info                       = '0;
    idx_cop_req_info.opcode                = reqq_opcode_e'(pipe_idx_cop_req_opcode_i);
    idx_cop_req_info.address               = pipe_idx_cop_req_address_i;
    idx_cop_req_info.trans_id              = pipe_idx_cop_req_trans_id_i;
    idx_cop_req_info.sync_flush_victims    = pipe_idx_cop_req_sync_flush_victims_i;
  end

  // ── Tie off state arrays to inactive ────────────────────────
  // No valid entries -> no dependencies, no write-around tracking
  logic [ReqqDepth-1:0] reqq_entry_valid;
  assign reqq_entry_valid = '0;

  logic [ReqqDepth-1:0] reqq_allowed_entries;
  assign reqq_allowed_entries = '1;  // all entries allowed

  /* verilator lint_off WIDTHCONCAT */  // 64-entry state array zeroed at TB level
  reqq_entry_state_t [ReqqDepth-1:0] reqq_state;
  assign reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */

  logic [ReqqDepth-1:0] upgrade_to_meshll_hot;
  assign upgrade_to_meshll_hot = '0;

  logic [ReqqDepth-1:0] entry_is_non_meshll;
  assign entry_is_non_meshll = '0;

  logic [ReqqDepth-1:0] clear_dep_hot;
  assign clear_dep_hot = '0;

  // Tie off tag_rsp, rbuf_state, rbuf_clear_req to inactive
  tag_rsp_t pipe_tag_rsp_info;
  assign pipe_tag_rsp_info = '0;

  rbuf_state_t pipe_rbuf_state;
  assign pipe_rbuf_state = '0;

  rbuf_clear_req_t pipe_rbuf_clear_req_info;
  assign pipe_rbuf_clear_req_info = '0;

  // ESR tie-offs
  esr_shire_config_t esr_shire_config;
  assign esr_shire_config = '0;

  bank_esr_ctl_t esr_ctl;
  assign esr_ctl = '0;

  dft_t dft;
  assign dft = '0;

  // ── DUT output structs ──────────────────────────────────────
  reqq_alloc_t alloc_info_ag;
  reqq_alloc_t alloc_info_ad;
  reqq_alloc_t alloc_info_paired_ag;
  logic [ReqqDepth-1:0] alloc_1hot_paired_ag;
  logic [ReqqDepth-1:0] alloc_1hot_ad;
  logic alloc_match_tag_victim_ag;

  // ── Decompose outputs to scalars ────────────────────────────
  assign alloc_orig_opcode_o   = alloc_info_ag.orig_opcode;
  assign alloc_opcode_o        = alloc_info_ag.opcode;
  assign alloc_address_space_o = alloc_info_ag.address_space;
  assign alloc_err_valid_o     = alloc_info_ag.err_valid;
  assign alloc_err_type_o      = alloc_info_ag.err_type;
  assign alloc_dep_valid_o     = alloc_info_ag.dep_valid;
  assign alloc_dep_id_o        = alloc_info_ag.dep_id;
  assign alloc_reqq_source_o   = alloc_info_ag.reqq_source;
  assign alloc_address_o       = alloc_info_ag.address;
  assign alloc_qwen_o          = alloc_info_ag.qwen;

  // ── DUT ─────────────────────────────────────────────────────
  shirecache_bank_reqq_alloc #(
    .IsL2(1)
  ) u_dut (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .dft_i                    (dft),
    .reqq_allowed_entries_i   (reqq_allowed_entries),
    .reqq_full_o              (reqq_full_o),
    .bank_ready_o             (bank_ready_o),
    .bank_valid_i             (bank_valid_i),
    .bank_info_i              (bank_info),
    .reqq_alloc_id_ag_o       (reqq_alloc_id_ag_o),
    .dataq_bank_ready_i       (dataq_bank_ready_i),
    .pipe_idx_cop_aborted_i   (pipe_idx_cop_aborted_i),
    .pipe_idx_cop_l2_busy_i   (pipe_idx_cop_l2_busy_i),
    .pipe_idx_cop_l3_busy_i   (pipe_idx_cop_l3_busy_i),
    .pipe_idx_cop_req_ready_o (pipe_idx_cop_req_ready_o),
    .pipe_idx_cop_req_valid_i (pipe_idx_cop_req_valid_i),
    .pipe_idx_cop_req_info_i  (idx_cop_req_info),
    .pipe_tag_rsp_valid_i     (1'b0),
    .pipe_tag_rsp_info_i      (pipe_tag_rsp_info),
    .pipe_rbuf_clear_req_info_i (pipe_rbuf_clear_req_info),
    .pipe_rbuf_state_i        (pipe_rbuf_state),
    .alloc_match_tag_victim_ag_o (alloc_match_tag_victim_ag),
    .reqq_entry_valid_i       (reqq_entry_valid),
    .upgrade_to_meshll_hot_i  (upgrade_to_meshll_hot),
    .entry_is_non_meshll_i    (entry_is_non_meshll),
    .clear_dep_hot_i          (clear_dep_hot),
    .reqq_state_i             (reqq_state),
    .alloc_valid_ag_o         (alloc_valid_ag_o),
    .alloc_1hot_ag_o          (alloc_1hot_ag_o),
    .alloc_info_ag_o          (alloc_info_ag),
    .alloc_valid_ad_o         (alloc_valid_ad_o),
    .alloc_1hot_ad_o          (alloc_1hot_ad),
    .alloc_info_ad_o          (alloc_info_ad),
    .alloc_1hot_paired_ag_o   (alloc_1hot_paired_ag),
    .alloc_info_paired_ag_o   (alloc_info_paired_ag),
    .mbist_on_i               (mbist_on_i),
    .esr_shire_config_i       (esr_shire_config),
    .esr_wr_reqq_entries_i    (esr_wr_reqq_entries_i),
    .esr_ctl_i                (esr_ctl)
  );

endmodule : shirecache_reqq_alloc_tb
/* verilator lint_on UNOPTFLAT */
