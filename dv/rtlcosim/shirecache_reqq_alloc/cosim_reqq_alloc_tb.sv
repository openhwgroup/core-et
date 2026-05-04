// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_reqq_alloc vs new
// shirecache_bank_reqq_alloc.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.
//
// Struct inputs are built from flat scalars so that C++ can drive them.
// Struct outputs are decomposed to scalars so C++ can compare field-by-field.
//
// Simplification: reqq_entry_valid and reqq_state are driven from C++.
// Most tests use all-zero state (no valid entries). Specific entries can
// be set valid with particular state to test dependency matching.

`include "soc.vh"
`include "macros.vh"

/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields intentionally unused
/* verilator lint_off UNOPTFLAT */     // alloc_info struct fields written by independent always_comb blocks
module cosim_reqq_alloc_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Bank request (flat scalars) ─────────────────────────────
  input  logic                        bank_valid_i,
  input  logic [4:0]                  bank_opcode_i,
  input  logic [6:0]                  bank_subopcode_i,
  input  logic [`PA_SIZE-1:0]         bank_address_i,
  input  logic [`SC_LINE_DATA_SIZE-1:0] bank_data_i,
  input  logic [`SC_LINE_QW_SIZE-1:0] bank_qwen_i,
  input  logic [2:0]                  bank_size_i,
  input  logic [`SC_PORT_ID_SIZE-1:0] bank_port_id_i,
  input  logic [`SC_REQQ_SOURCE_SIZE-1:0] bank_source_i,
  input  logic [`DV_TRANS_ID_SIZE-1:0] bank_trans_id_i,
  input  logic                        bank_write_is_partial_i,
  input  logic [`AXI_AXQOS_SIZE-1:0]  bank_qos_i,
  input  logic [`SC_LINE_BYTE_SIZE-1:0] bank_ben_i,
  input  logic [`SC_REQQ_TAG_ID_SIZE-1:0] bank_id_i,
  input  logic                        bank_wdata_i,

  // ── Dataq ───────────────────────────────────────────────────
  input  logic                        dataq_bank_ready_i,

  // ── Idx CacheOp FSM ─────────────────────────────────────────
  input  logic                        pipe_idx_cop_req_valid_i,
  input  logic [5:0]                  pipe_idx_cop_req_opcode_i,
  input  logic [`PA_SIZE-1:0]         pipe_idx_cop_req_address_i,
  input  logic [`DV_TRANS_ID_SIZE-1:0] pipe_idx_cop_req_trans_id_i,
  input  logic                        pipe_idx_cop_req_sync_flush_victims_i,
  input  logic                        pipe_idx_cop_l2_busy_i,
  input  logic                        pipe_idx_cop_l3_busy_i,
  input  logic                        pipe_idx_cop_aborted_i,

  // ── Controls ────────────────────────────────────────────────
  input  logic                        mbist_on_i,
  input  logic                        esr_wr_reqq_entries_i,

  // ── reqq state (simplified: entry_valid as 64-bit, state all zeros) ──
  input  logic [`SC_REQQ_DEPTH-1:0]   reqq_entry_valid_i,
  input  logic [`SC_REQQ_DEPTH-1:0]   reqq_allowed_entries_i,

  // ── Tag response (flat scalars for victim dep testing) ──────
  input  logic                        pipe_tag_rsp_valid_i,
  input  logic                        pipe_tag_rsp_victim_i,
  input  logic [`PA_SIZE-1:0]         pipe_tag_rsp_victim_address_i,
  input  logic [2:0]                  pipe_tag_rsp_victim_address_space_i,
  input  logic [`SC_REQQ_ID_SIZE-1:0] pipe_tag_rsp_reqq_id_i,

  // ── ESR controls ────────────────────────────────────────────
  input  logic                        esr_sc_l2_bypass_i,
  input  logic                        esr_sc_remote_scp_enable_i,

  // ── New module outputs ──────────────────────────────────────
  output logic                        new_bank_ready_o,
  output logic                        new_reqq_full_o,
  output logic                        new_alloc_valid_ag_o,
  output logic [4:0]                  new_alloc_orig_opcode_o,
  output logic [5:0]                  new_alloc_opcode_o,
  output logic [2:0]                  new_alloc_address_space_o,
  output logic                        new_alloc_err_valid_o,
  output logic [4:0]                  new_alloc_err_type_o,
  output logic                        new_alloc_dep_valid_o,
  output logic [`SC_REQQ_ID_SIZE-1:0] new_alloc_dep_id_o,
  output logic [`SC_REQQ_ID_SIZE-1:0] new_reqq_alloc_id_ag_o,
  output logic                        new_pipe_idx_cop_req_ready_o,
  output logic                        new_alloc_valid_ad_o,
  output logic [`SC_REQQ_DEPTH-1:0]   new_alloc_1hot_ag_o,
  output logic [`SC_REQQ_DEPTH-1:0]   new_alloc_1hot_ad_o,
  output logic [1:0]                  new_alloc_reqq_source_o,
  output logic [`PA_SIZE-1:0]         new_alloc_address_o,
  output logic [`SC_LINE_QW_SIZE-1:0] new_alloc_qwen_o,
  output logic [2:0]                  new_alloc_size_o,
  output logic                        new_alloc_dep_non_meshll_head_o,
  output logic                        new_alloc_match_tag_victim_ag_o,
  output logic                        new_alloc_err_imprecise_o,
  output logic [`SC_REQQ_ID_SIZE-1:0] new_alloc_paired_id_o,
  output logic [`SC_REQQ_DEPTH-1:0]   new_alloc_1hot_paired_ag_o,
  output logic [4:0]                  new_alloc_paired_orig_opcode_o,

  // ── Original module outputs ─────────────────────────────────
  output logic                        orig_bank_ready_o,
  output logic                        orig_reqq_full_o,
  output logic                        orig_alloc_valid_ag_o,
  output logic [4:0]                  orig_alloc_orig_opcode_o,
  output logic [5:0]                  orig_alloc_opcode_o,
  output logic [2:0]                  orig_alloc_address_space_o,
  output logic                        orig_alloc_err_valid_o,
  output logic [4:0]                  orig_alloc_err_type_o,
  output logic                        orig_alloc_dep_valid_o,
  output logic [`SC_REQQ_ID_SIZE-1:0] orig_alloc_dep_id_o,
  output logic [`SC_REQQ_ID_SIZE-1:0] orig_reqq_alloc_id_ag_o,
  output logic                        orig_pipe_idx_cop_req_ready_o,
  output logic                        orig_alloc_valid_ad_o,
  output logic [`SC_REQQ_DEPTH-1:0]   orig_alloc_1hot_ag_o,
  output logic [`SC_REQQ_DEPTH-1:0]   orig_alloc_1hot_ad_o,
  output logic [1:0]                  orig_alloc_reqq_source_o,
  output logic [`PA_SIZE-1:0]         orig_alloc_address_o,
  output logic [`SC_LINE_QW_SIZE-1:0] orig_alloc_qwen_o,
  output logic [2:0]                  orig_alloc_size_o,
  output logic                        orig_alloc_dep_non_meshll_head_o,
  output logic                        orig_alloc_match_tag_victim_ag_o,
  output logic                        orig_alloc_err_imprecise_o,
  output logic [`SC_REQQ_ID_SIZE-1:0] orig_alloc_paired_id_o,
  output logic [`SC_REQQ_DEPTH-1:0]   orig_alloc_1hot_paired_ag_o,
  output logic [4:0]                  orig_alloc_paired_orig_opcode_o
);

  // ════════════════════════════════════════════════════════════
  // Build struct inputs from flat scalars
  // ════════════════════════════════════════════════════════════

  // -- New bank request --
  reqq_bank_req_t new_bank_info;
  always_comb begin
    new_bank_info                    = '0;
    new_bank_info.opcode             = etlink_pkg::req_opcode_e'(bank_opcode_i);
    new_bank_info.subopcode          = bank_subopcode_i;
    new_bank_info.address            = bank_address_i;
    new_bank_info.data               = bank_data_i;
    new_bank_info.qwen               = bank_qwen_i;
    new_bank_info.size               = sc_size_t'(bank_size_i);
    new_bank_info.port_id            = bank_port_id_i;
    new_bank_info.source             = bank_source_i;
    new_bank_info.trans_id           = bank_trans_id_i;
    new_bank_info.write_is_partial   = bank_write_is_partial_i;
    new_bank_info.qos                = bank_qos_i;
    new_bank_info.ben                = bank_ben_i;
    new_bank_info.id                 = bank_id_i;
    new_bank_info.wdata              = bank_wdata_i;
  end

  // -- New idx CacheOp request --
  idx_cop_req_t new_idx_cop_req_info;
  always_comb begin
    new_idx_cop_req_info                       = '0;
    new_idx_cop_req_info.opcode                = reqq_opcode_e'(pipe_idx_cop_req_opcode_i);
    new_idx_cop_req_info.address               = pipe_idx_cop_req_address_i;
    new_idx_cop_req_info.trans_id              = pipe_idx_cop_req_trans_id_i;
    new_idx_cop_req_info.sync_flush_victims    = pipe_idx_cop_req_sync_flush_victims_i;
  end

  // -- Original bank request --
  sc_reqq_bank_req_t orig_bank_info;
  always_comb begin
    orig_bank_info                    = '0;
    orig_bank_info.opcode             = et_link_req_opcode_t'(bank_opcode_i);
    orig_bank_info.subopcode          = bank_subopcode_i;
    orig_bank_info.address            = bank_address_i;
    orig_bank_info.data               = bank_data_i;
    orig_bank_info.qwen               = bank_qwen_i;
    orig_bank_info.size               = et_link_size_t'(bank_size_i);
    orig_bank_info.port_id            = bank_port_id_i;
    orig_bank_info.source             = bank_source_i;
    orig_bank_info.trans_id           = bank_trans_id_i;
    orig_bank_info.write_is_partial   = bank_write_is_partial_i;
    orig_bank_info.qos                = bank_qos_i[0];
    orig_bank_info.ben                = bank_ben_i;
    orig_bank_info.id                 = bank_id_i;
    orig_bank_info.wdata              = bank_wdata_i;
  end

  // -- Original idx CacheOp request --
  sc_pipe_idx_cop_req_t orig_idx_cop_req_info;
  always_comb begin
    orig_idx_cop_req_info                       = '0;
    orig_idx_cop_req_info.opcode                = sc_reqq_opcode_t'(pipe_idx_cop_req_opcode_i);
    orig_idx_cop_req_info.address               = pipe_idx_cop_req_address_i;
    orig_idx_cop_req_info.trans_id              = pipe_idx_cop_req_trans_id_i;
    orig_idx_cop_req_info.sync_flush_victims    = pipe_idx_cop_req_sync_flush_victims_i;
  end

  // -- Tag response --
  tag_rsp_t new_tag_rsp_info;
  always_comb begin
    new_tag_rsp_info                       = '0;
    new_tag_rsp_info.victim                = pipe_tag_rsp_victim_i;
    new_tag_rsp_info.victim_address        = pipe_tag_rsp_victim_address_i;
    new_tag_rsp_info.victim_address_space  = address_space_e'(pipe_tag_rsp_victim_address_space_i);
    new_tag_rsp_info.reqq_id               = pipe_tag_rsp_reqq_id_i;
  end

  sc_pipe_tag_rsp_t orig_tag_rsp_info;
  always_comb begin
    orig_tag_rsp_info                       = '0;
    orig_tag_rsp_info.victim                = pipe_tag_rsp_victim_i;
    orig_tag_rsp_info.victim_address        = pipe_tag_rsp_victim_address_i;
    orig_tag_rsp_info.victim_address_space  = sc_address_space_t'(pipe_tag_rsp_victim_address_space_i);
    orig_tag_rsp_info.reqq_id               = pipe_tag_rsp_reqq_id_i;
  end

  // -- All-zero reqq state --
  /* verilator lint_off WIDTHCONCAT */  // 64-entry state arrays zeroed at cosim TB level
  reqq_entry_state_t [ReqqDepth-1:0] new_reqq_state;
  assign new_reqq_state = '0;

  sc_reqq_entry_state_t [`SC_REQQ_DEPTH-1:0] orig_reqq_state;
  assign orig_reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */

  // -- rbuf state / clear req: all inactive --
  rbuf_state_t new_rbuf_state;
  assign new_rbuf_state = '0;

  rbuf_clear_req_t new_rbuf_clear_req;
  assign new_rbuf_clear_req = '0;

  sc_pipe_rbuf_state_t orig_rbuf_state;
  assign orig_rbuf_state = '0;

  sc_pipe_rbuf_clear_req_t orig_rbuf_clear_req;
  assign orig_rbuf_clear_req = '0;

  // -- ESR --
  esr_shire_config_t new_esr_shire_config;
  assign new_esr_shire_config = '0;

  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_l2_bypass = esr_sc_l2_bypass_i;
    new_esr_ctl.esr_sc_remote_scp_enable = esr_sc_remote_scp_enable_i;
  end

  esr_shire_config_t orig_esr_shire_config;
  assign orig_esr_shire_config = '0;

  sc_bank_esr_ctl_t orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.esr_sc_l2_bypass = esr_sc_l2_bypass_i;
    orig_esr_ctl.esr_sc_remote_scp_enable = esr_sc_remote_scp_enable_i;
  end

  // DFT
  dft_t dft;
  assign dft = '0;

  // ════════════════════════════════════════════════════════════
  // New module
  // ════════════════════════════════════════════════════════════

  reqq_alloc_t new_alloc_info_ag;
  reqq_alloc_t new_alloc_info_ad;
  reqq_alloc_t new_alloc_info_paired_ag;

  shirecache_bank_reqq_alloc #(.IsL2(1)) u_new (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .dft_i                    (dft),
    .reqq_allowed_entries_i   (reqq_allowed_entries_i),
    .reqq_full_o              (new_reqq_full_o),
    .bank_ready_o             (new_bank_ready_o),
    .bank_valid_i             (bank_valid_i),
    .bank_info_i              (new_bank_info),
    .reqq_alloc_id_ag_o       (new_reqq_alloc_id_ag_o),
    .dataq_bank_ready_i       (dataq_bank_ready_i),
    .pipe_idx_cop_aborted_i   (pipe_idx_cop_aborted_i),
    .pipe_idx_cop_l2_busy_i   (pipe_idx_cop_l2_busy_i),
    .pipe_idx_cop_l3_busy_i   (pipe_idx_cop_l3_busy_i),
    .pipe_idx_cop_req_ready_o (new_pipe_idx_cop_req_ready_o),
    .pipe_idx_cop_req_valid_i (pipe_idx_cop_req_valid_i),
    .pipe_idx_cop_req_info_i  (new_idx_cop_req_info),
    .pipe_tag_rsp_valid_i     (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info_i      (new_tag_rsp_info),
    .pipe_rbuf_clear_req_info_i (new_rbuf_clear_req),
    .pipe_rbuf_state_i        (new_rbuf_state),
    .alloc_match_tag_victim_ag_o (new_alloc_match_tag_victim_ag_o),
    .reqq_entry_valid_i       (reqq_entry_valid_i),
    .upgrade_to_meshll_hot_i  ('0),
    .entry_is_non_meshll_i    ('0),
    .clear_dep_hot_i          ('0),
    .reqq_state_i             (new_reqq_state),
    .alloc_valid_ag_o         (new_alloc_valid_ag_o),
    .alloc_1hot_ag_o          (new_alloc_1hot_ag_o),
    .alloc_info_ag_o          (new_alloc_info_ag),
    .alloc_valid_ad_o         (new_alloc_valid_ad_o),
    .alloc_1hot_ad_o          (new_alloc_1hot_ad_o),
    .alloc_info_ad_o          (new_alloc_info_ad),
    .alloc_1hot_paired_ag_o   (new_alloc_1hot_paired_ag_o),
    .alloc_info_paired_ag_o   (new_alloc_info_paired_ag),
    .mbist_on_i               (mbist_on_i),
    .esr_shire_config_i       (new_esr_shire_config),
    .esr_wr_reqq_entries_i    (esr_wr_reqq_entries_i),
    .esr_ctl_i                (new_esr_ctl)
  );

  // Decompose new outputs
  assign new_alloc_orig_opcode_o         = new_alloc_info_ag.orig_opcode;
  assign new_alloc_opcode_o              = new_alloc_info_ag.opcode;
  assign new_alloc_address_space_o       = new_alloc_info_ag.address_space;
  assign new_alloc_err_valid_o           = new_alloc_info_ag.err_valid;
  assign new_alloc_err_type_o            = new_alloc_info_ag.err_type;
  assign new_alloc_dep_valid_o           = new_alloc_info_ag.dep_valid;
  assign new_alloc_dep_id_o              = new_alloc_info_ag.dep_id;
  assign new_alloc_reqq_source_o         = new_alloc_info_ag.reqq_source;
  assign new_alloc_address_o             = new_alloc_info_ag.address;
  assign new_alloc_qwen_o               = new_alloc_info_ag.qwen;
  assign new_alloc_size_o                = new_alloc_info_ag.size;
  assign new_alloc_dep_non_meshll_head_o = new_alloc_info_ag.dep_non_meshll_head;
  assign new_alloc_err_imprecise_o       = new_alloc_info_ag.err_imprecise;
  assign new_alloc_paired_id_o           = new_alloc_info_ag.paired_id;
  assign new_alloc_paired_orig_opcode_o  = new_alloc_info_paired_ag.orig_opcode;

  // ════════════════════════════════════════════════════════════
  // Original module
  // ════════════════════════════════════════════════════════════

  // Original uses active-high synchronous reset
  wire orig_reset = ~rst_ni;

  sc_reqq_alloc_t orig_alloc_info_ag;
  sc_reqq_alloc_t orig_alloc_info_ad;
  sc_reqq_alloc_t orig_alloc_info_paired_ag;

  shire_cache_bank_reqq_alloc #(.IS_L2(1)) u_orig (
    .fclock                   (clk_i),
    .reset                    (orig_reset),
    .reqq_allowed_entries     (reqq_allowed_entries_i),
    .reqq_full                (orig_reqq_full_o),
    .bank_ready               (orig_bank_ready_o),
    .bank_valid               (bank_valid_i),
    .bank_info                (orig_bank_info),
    .reqq_alloc_id_ag         (orig_reqq_alloc_id_ag_o),
    .dataq_bank_ready         (dataq_bank_ready_i),
    .pipe_idx_cop_aborted     (pipe_idx_cop_aborted_i),
    .pipe_idx_cop_l2_busy     (pipe_idx_cop_l2_busy_i),
    .pipe_idx_cop_l3_busy     (pipe_idx_cop_l3_busy_i),
    .pipe_idx_cop_req_ready   (orig_pipe_idx_cop_req_ready_o),
    .pipe_idx_cop_req_valid   (pipe_idx_cop_req_valid_i),
    .pipe_idx_cop_req_info    (orig_idx_cop_req_info),
    .pipe_tag_rsp_valid       (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info        (orig_tag_rsp_info),
    .pipe_rbuf_clear_req_info (orig_rbuf_clear_req),
    .pipe_rbuf_state          (orig_rbuf_state),
    .alloc_match_tag_victim_ag (orig_alloc_match_tag_victim_ag_o),
    .reqq_entry_valid         (reqq_entry_valid_i),
    .upgrade_to_meshll_hot    ('0),
    .entry_is_non_meshll      ('0),
    .clear_dep_hot            ('0),
    .reqq_state               (orig_reqq_state),
    .alloc_valid_ag           (orig_alloc_valid_ag_o),
    .alloc_1hot_ag            (orig_alloc_1hot_ag_o),
    .alloc_info_ag            (orig_alloc_info_ag),
    .alloc_valid_ad           (orig_alloc_valid_ad_o),
    .alloc_1hot_ad            (orig_alloc_1hot_ad_o),
    .alloc_info_ad            (orig_alloc_info_ad),
    .alloc_1hot_paired_ag     (orig_alloc_1hot_paired_ag_o),
    .alloc_info_paired_ag     (orig_alloc_info_paired_ag),
    .mbist_on                 (mbist_on_i),
    .esr_shire_config         (orig_esr_shire_config),
    .esr_wr_reqq_entries      (esr_wr_reqq_entries_i),
    .esr_ctl                  (orig_esr_ctl)
  );

  // Decompose orig outputs
  assign orig_alloc_orig_opcode_o         = orig_alloc_info_ag.orig_opcode;
  assign orig_alloc_opcode_o              = orig_alloc_info_ag.opcode;
  assign orig_alloc_address_space_o       = orig_alloc_info_ag.address_space;
  assign orig_alloc_err_valid_o           = orig_alloc_info_ag.err_valid;
  assign orig_alloc_err_type_o            = orig_alloc_info_ag.err_type;
  assign orig_alloc_dep_valid_o           = orig_alloc_info_ag.dep_valid;
  assign orig_alloc_dep_id_o              = orig_alloc_info_ag.dep_id;
  assign orig_alloc_reqq_source_o         = orig_alloc_info_ag.reqq_source;
  assign orig_alloc_address_o             = orig_alloc_info_ag.address;
  assign orig_alloc_qwen_o               = orig_alloc_info_ag.qwen;
  assign orig_alloc_size_o                = orig_alloc_info_ag.size;
  assign orig_alloc_dep_non_meshll_head_o = orig_alloc_info_ag.dep_non_meshll_head;
  assign orig_alloc_err_imprecise_o       = orig_alloc_info_ag.err_imprecise;
  assign orig_alloc_paired_id_o           = orig_alloc_info_ag.paired_id;
  assign orig_alloc_paired_orig_opcode_o  = orig_alloc_info_paired_ag.orig_opcode;

endmodule : cosim_reqq_alloc_tb
/* verilator lint_on UNOPTFLAT */
