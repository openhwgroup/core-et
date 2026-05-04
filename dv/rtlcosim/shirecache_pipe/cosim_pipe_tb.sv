// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe vs new shirecache_pipe.
//
// Both modules are self-contained with internal RAMs and sub-modules.
// Identical stimulus is driven; key outputs are compared cycle-by-cycle
// from C++.
//
// Original: active-high synchronous reset, sc_* types, individual DFT signals.
// New:      active-low async reset, shirecache_pkg types, consolidated dft_t.

`include "soc.vh"

module cosim_pipe_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Pipe request inputs (scalar) ──
  input  logic                        pipe_start_i,

  // pipe_req_info fields
  input  logic [DvTransIdSize-1:0]    pr_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pr_reqq_id_i,
  input  logic [5:0]                  pr_opcode_i,
  input  logic [PaSize-1:0]           pr_address_i,
  input  logic [2:0]                  pr_address_space_i,
  input  logic                        pr_zero_data_i,
  input  logic [LineQwSize-1:0]       pr_qwen_i,
  input  logic [2:0]                  pr_size_i,
  input  logic [SubBankIdSize-1:0]    pr_sub_bank_id_i,

  // dataq rd rsp (from C++)
  input  logic [DvTransIdSize-1:0]    dataq_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       dataq_rsp_reqq_id_i,
  input  logic [LineDataSize-1:0]     dataq_rsp_data_i,
  input  logic [LineByteSize-1:0]     dataq_rsp_ben_i,
  input  logic                        dataq_rsp_err_i,

  // idx_cop ready from reqq
  input  logic                        pipe_idx_cop_req_ready_i,

  // rbuf clear (tie to zero normally)
  input  logic [RbufSize-1:0]         rbuf_clear_mem_valid_hot_i,
  // rbuf rd req (tie to zero normally)
  input  logic                        pipe_rbuf_rd_req_valid_i,
  input  logic                        pipe_rbuf_rd_rsp_ready_i,

  // ESR config scalars
  input  logic [RamDelaySize-1:0]     esr_sc_ram_delay_i,
  input  logic                        esr_sc_l2_rbuf_enable_i,
  input  logic                        esr_sc_scp_rbuf_enable_i,
  input  logic                        esr_sc_zero_state_enable_i,
  input  logic                        esr_sc_all_shire_aliasing_i,
  input  logic                        esr_sc_two_shire_aliasing_use_shire_lsb_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l2_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_tag_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l3_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_tag_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_scp_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_tag_mask_i,
  input  logic                        esr_sc_cbuf_enable_i,
  input  logic [CbufSize-1:0]         esr_sc_cbuf_entry_enable_i,
  input  logic                        esr_sc_allow_only_1_req_per_sub_bank_i,
  input  logic                        esr_sc_allow_only_1_req_per_bank_i,

  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [L3ShireIdSize-1:0]    my_l3_virtual_shire_id_i,
  input  logic [ScpShireIdSize-1:0]   my_scp_virtual_shire_id_i,

  // ── Comparison outputs: NEW module ──
  output logic                        new_pipe_active_o,
  output logic [SubBanks-1:0]         new_pipe_busy_sub_bank_o,
  output logic                        new_pipe_squash_o,
  output logic                        new_tag_rsp_valid_o,
  output logic [$bits(tag_rsp_t)-1:0]        new_tag_rsp_info_o,
  output logic                        new_data_rsp_pre_valid_o,
  output logic [$bits(data_rsp_pre_t)-1:0]   new_data_rsp_pre_info_o,
  output logic                        new_data_rsp_valid_o,
  output logic [$bits(data_rsp_t)-1:0]       new_data_rsp_info_o,
  output logic                        new_cbuf_rsp_valid_o,
  output logic [$bits(cbuf_rsp_t)-1:0]       new_cbuf_rsp_info_o,
  output logic                        new_idx_cop_aborted_o,
  output logic                        new_idx_cop_l2_busy_o,
  output logic                        new_idx_cop_l3_busy_o,
  output logic                        new_idx_cop_req_valid_o,
  output logic [$bits(idx_cop_req_t)-1:0]    new_idx_cop_req_info_o,
  output logic                        new_idx_cop_is_active_o,
  output logic                        new_dataq_rd_req_valid_o,
  output logic                        new_rbuf_rd_req_ready_o,

  // ── Comparison outputs: ORIGINAL module ──
  output logic                        orig_pipe_active_o,
  output logic [SubBanks-1:0]         orig_pipe_busy_sub_bank_o,
  output logic                        orig_pipe_squash_o,
  output logic                        orig_tag_rsp_valid_o,
  output logic [$bits(tag_rsp_t)-1:0]        orig_tag_rsp_info_o,
  output logic                        orig_data_rsp_pre_valid_o,
  output logic [$bits(data_rsp_pre_t)-1:0]   orig_data_rsp_pre_info_o,
  output logic                        orig_data_rsp_valid_o,
  output logic [$bits(data_rsp_t)-1:0]       orig_data_rsp_info_o,
  output logic                        orig_cbuf_rsp_valid_o,
  output logic [$bits(cbuf_rsp_t)-1:0]       orig_cbuf_rsp_info_o,
  output logic                        orig_idx_cop_aborted_o,
  output logic                        orig_idx_cop_l2_busy_o,
  output logic                        orig_idx_cop_l3_busy_o,
  output logic                        orig_idx_cop_req_valid_o,
  output logic [$bits(idx_cop_req_t)-1:0]    orig_idx_cop_req_info_o,
  output logic                        orig_idx_cop_is_active_o,
  output logic                        orig_dataq_rd_req_valid_o,
  output logic                        orig_rbuf_rd_req_ready_o
);

  // ════════════════════════════════════════════════════════
  // Reset
  // ════════════════════════════════════════════════════════
  wire reset = !rst_ni;

  // ════════════════════════════════════════════════════════
  // Assemble struct inputs
  // ════════════════════════════════════════════════════════

  // ── reqq_pipe_req ──
  reqq_pipe_req_t new_pipe_req_info;
  always_comb begin
    new_pipe_req_info.trans_id      = pr_trans_id_i;
    new_pipe_req_info.reqq_id       = pr_reqq_id_i;
    new_pipe_req_info.opcode        = reqq_opcode_e'(pr_opcode_i);
    new_pipe_req_info.address       = pr_address_i;
    new_pipe_req_info.address_space = address_space_e'(pr_address_space_i);
    new_pipe_req_info.zero_data     = pr_zero_data_i;
    new_pipe_req_info.qwen          = pr_qwen_i;
    new_pipe_req_info.size          = sc_size_t'(pr_size_i);
    new_pipe_req_info.sub_bank_id   = pr_sub_bank_id_i;
  end

  sc_reqq_pipe_req_t orig_pipe_req_info;
  assign orig_pipe_req_info = new_pipe_req_info;

  // ── dataq_rd_rsp ──
  dataq_rd_rsp_t new_dataq_rd_rsp;
  always_comb begin
    new_dataq_rd_rsp.trans_id = dataq_rsp_trans_id_i;
    new_dataq_rd_rsp.reqq_id  = dataq_rsp_reqq_id_i;
    new_dataq_rd_rsp.data     = dataq_rsp_data_i;
    new_dataq_rd_rsp.ben      = dataq_rsp_ben_i;
    new_dataq_rd_rsp.err      = dataq_rsp_err_i;
  end

  sc_pipe_dataq_rd_rsp_t orig_dataq_rd_rsp;
  assign orig_dataq_rd_rsp = new_dataq_rd_rsp;

  // ── rbuf_clear_req (tie to zero for basic cosim) ──
  rbuf_clear_req_t new_rbuf_clear_req;
  assign new_rbuf_clear_req = '0;

  sc_pipe_rbuf_clear_req_t orig_rbuf_clear_req;
  assign orig_rbuf_clear_req = '0;

  // ── rbuf_rd_req ──
  rbuf_rd_req_t new_rbuf_rd_req_info;
  assign new_rbuf_rd_req_info = '0;

  sc_pipe_rbuf_rd_req_t orig_rbuf_rd_req_info;
  assign orig_rbuf_rd_req_info = '0;

  // ── ESR ctl ──
  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_ram_delay                      = esr_sc_ram_delay_i;
    new_esr_ctl.esr_sc_l2_rbuf_enable                 = esr_sc_l2_rbuf_enable_i;
    new_esr_ctl.esr_sc_scp_rbuf_enable                = esr_sc_scp_rbuf_enable_i;
    new_esr_ctl.esr_sc_zero_state_enable              = esr_sc_zero_state_enable_i;
    new_esr_ctl.esr_sc_all_shire_aliasing             = esr_sc_all_shire_aliasing_i;
    new_esr_ctl.esr_sc_two_shire_aliasing_use_shire_lsb = esr_sc_two_shire_aliasing_use_shire_lsb_i;
    new_esr_ctl.esr_sc_l2_set_base                    = esr_sc_l2_set_base_i;
    new_esr_ctl.esr_sc_l2_set_size                    = esr_sc_l2_set_size_i;
    new_esr_ctl.esr_sc_l2_set_mask                    = esr_sc_l2_set_mask_i;
    new_esr_ctl.esr_sc_l2_tag_mask                    = esr_sc_l2_tag_mask_i;
    new_esr_ctl.esr_sc_l3_set_base                    = esr_sc_l3_set_base_i;
    new_esr_ctl.esr_sc_l3_set_size                    = esr_sc_l3_set_size_i;
    new_esr_ctl.esr_sc_l3_set_mask                    = esr_sc_l3_set_mask_i;
    new_esr_ctl.esr_sc_l3_tag_mask                    = esr_sc_l3_tag_mask_i;
    new_esr_ctl.esr_sc_scp_set_base                   = esr_sc_scp_set_base_i;
    new_esr_ctl.esr_sc_scp_set_size                   = esr_sc_scp_set_size_i;
    new_esr_ctl.esr_sc_scp_set_mask                   = esr_sc_scp_set_mask_i;
    new_esr_ctl.esr_sc_scp_tag_mask                   = esr_sc_scp_tag_mask_i;
    new_esr_ctl.esr_sc_cbuf_enable                    = esr_sc_cbuf_enable_i;
    new_esr_ctl.esr_sc_cbuf_entry_enable              = esr_sc_cbuf_entry_enable_i;
    new_esr_ctl.esr_sc_allow_only_1_req_per_sub_bank  = esr_sc_allow_only_1_req_per_sub_bank_i;
    new_esr_ctl.esr_sc_allow_only_1_req_per_bank      = esr_sc_allow_only_1_req_per_bank_i;
  end

  sc_bank_esr_ctl_t orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.esr_sc_ram_delay                      = esr_sc_ram_delay_i;
    orig_esr_ctl.esr_sc_l2_rbuf_enable                 = esr_sc_l2_rbuf_enable_i;
    orig_esr_ctl.esr_sc_scp_rbuf_enable                = esr_sc_scp_rbuf_enable_i;
    orig_esr_ctl.esr_sc_zero_state_enable              = esr_sc_zero_state_enable_i;
    orig_esr_ctl.esr_sc_all_shire_aliasing             = esr_sc_all_shire_aliasing_i;
    orig_esr_ctl.esr_sc_two_shire_aliasing_use_shire_lsb = esr_sc_two_shire_aliasing_use_shire_lsb_i;
    orig_esr_ctl.esr_sc_l2_set_base                    = esr_sc_l2_set_base_i;
    orig_esr_ctl.esr_sc_l2_set_size                    = esr_sc_l2_set_size_i;
    orig_esr_ctl.esr_sc_l2_set_mask                    = esr_sc_l2_set_mask_i;
    orig_esr_ctl.esr_sc_l2_tag_mask                    = esr_sc_l2_tag_mask_i;
    orig_esr_ctl.esr_sc_l3_set_base                    = esr_sc_l3_set_base_i;
    orig_esr_ctl.esr_sc_l3_set_size                    = esr_sc_l3_set_size_i;
    orig_esr_ctl.esr_sc_l3_set_mask                    = esr_sc_l3_set_mask_i;
    orig_esr_ctl.esr_sc_l3_tag_mask                    = esr_sc_l3_tag_mask_i;
    orig_esr_ctl.esr_sc_scp_set_base                   = esr_sc_scp_set_base_i;
    orig_esr_ctl.esr_sc_scp_set_size                   = esr_sc_scp_set_size_i;
    orig_esr_ctl.esr_sc_scp_set_mask                   = esr_sc_scp_set_mask_i;
    orig_esr_ctl.esr_sc_scp_tag_mask                   = esr_sc_scp_tag_mask_i;
    orig_esr_ctl.esr_sc_cbuf_enable                    = esr_sc_cbuf_enable_i;
    orig_esr_ctl.esr_sc_cbuf_entry_enable              = esr_sc_cbuf_entry_enable_i;
    orig_esr_ctl.esr_sc_allow_only_1_req_per_sub_bank  = esr_sc_allow_only_1_req_per_sub_bank_i;
    orig_esr_ctl.esr_sc_allow_only_1_req_per_bank      = esr_sc_allow_only_1_req_per_bank_i;
  end

  // ── l3_swizzle_ctl (zeros for cosim — no swizzle) ──
  esr_sc_l3_shire_swizzle_ctl_t orig_l3_swizzle_ctl;
  always_comb begin
    orig_l3_swizzle_ctl = '0;
  end

  // ── esr_shire_cache_ram_cfg — tie all fields to zero ──
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // ── DFT — tie to zero for both modules ──
  dft_t new_dft;
  assign new_dft = '0;

  ram_cfg_t new_ram_cfg;
  assign new_ram_cfg = '0;

  // ── BIST — tie to zero ──
  bist_req_t new_bist_req;
  assign new_bist_req = '0;

  sc_pipe_bist_req_t orig_bist_req;
  assign orig_bist_req = '0;

  // ── Perfmon — tie to zero ──
  perfmon_req_info_t new_perfmon_req;
  assign new_perfmon_req = '0;

  sc_perfmon_req_info_t orig_perfmon_req;
  assign orig_perfmon_req = '0;

  // ════════════════════════════════════════════════════════
  // NEW module instance
  // ════════════════════════════════════════════════════════

  pipe_busy_t     new_pipe_busy;
  tag_rsp_t       new_tag_rsp;
  data_rsp_pre_t  new_data_rsp_pre;
  data_rsp_t      new_data_rsp;
  cbuf_rsp_t      new_cbuf_rsp;
  dataq_rd_req_t  new_dataq_rd_req;
  idx_cop_req_t   new_idx_cop_req;
  rbuf_state_t    new_rbuf_state;
  rbuf_rd_rsp_t   new_rbuf_rd_rsp;
  bist_rsp_t      new_bist_rsp;
  perfmon_rsp_info_t new_perfmon_rsp;
  pipe_esr_status_t  new_esr_status;
  sbe_dbe_overflow_t new_sbe_dbe_overflow;

  shirecache_pipe u_new (
    .clk_i                              (clk_i),
    .rst_ni                             (rst_ni),
    .rst_c_ni                           (rst_ni),

    .dft_i                              (new_dft),
    .dft_sram_clk_i                     (1'b0),

    .ram_cfg_i                          (new_ram_cfg),

    .pipe_start_i                       (pipe_start_i),
    .pipe_req_info_i                    (new_pipe_req_info),
    .pipe_active_o                      (new_pipe_active_o),
    .pipe_busy_o                        (new_pipe_busy),
    .pipe_squash_o                      (new_pipe_squash_o),

    .pipe_tag_rsp_valid_o               (new_tag_rsp_valid_o),
    .pipe_tag_rsp_info_o                (new_tag_rsp),

    .pipe_data_rsp_pre_valid_o          (new_data_rsp_pre_valid_o),
    .pipe_data_rsp_pre_info_o           (new_data_rsp_pre),

    .pipe_data_rsp_valid_o              (new_data_rsp_valid_o),
    .pipe_data_rsp_info_o               (new_data_rsp),

    .pipe_cbuf_rsp_valid_o              (new_cbuf_rsp_valid_o),
    .pipe_cbuf_rsp_info_o               (new_cbuf_rsp),

    .pipe_rbuf_clear_req_info_i         (new_rbuf_clear_req),

    .pipe_rbuf_rd_req_ready_o           (new_rbuf_rd_req_ready_o),
    .pipe_rbuf_rd_req_valid_i           (pipe_rbuf_rd_req_valid_i),
    .pipe_rbuf_rd_req_info_i            (new_rbuf_rd_req_info),

    .pipe_rbuf_rd_rsp_ready_i           (pipe_rbuf_rd_rsp_ready_i),
    .pipe_rbuf_rd_rsp_valid_o           (),
    .pipe_rbuf_rd_rsp_info_o            (new_rbuf_rd_rsp),

    .pipe_rbuf_state_o                  (new_rbuf_state),

    .pipe_dataq_rd_req_valid_o          (new_dataq_rd_req_valid_o),
    .pipe_dataq_rd_req_info_o           (new_dataq_rd_req),
    .pipe_dataq_rd_rsp_info_i           (new_dataq_rd_rsp),

    .pipe_idx_cop_aborted_o             (new_idx_cop_aborted_o),
    .pipe_idx_cop_l2_busy_o             (new_idx_cop_l2_busy_o),
    .pipe_idx_cop_l3_busy_o             (new_idx_cop_l3_busy_o),
    .pipe_idx_cop_req_ready_i           (pipe_idx_cop_req_ready_i),
    .pipe_idx_cop_req_valid_o           (new_idx_cop_req_valid_o),
    .pipe_idx_cop_req_info_o            (new_idx_cop_req),
    .pipe_idx_cop_is_active_o           (new_idx_cop_is_active_o),

    .mbist_on_i                         (1'b0),
    .pipe_bist_req_info_i               (new_bist_req),
    .pipe_bist_rsp_info_o               (new_bist_rsp),

    .perfmon_req_info_i                 (new_perfmon_req),
    .perfmon_rsp_info_o                 (new_perfmon_rsp),

    .esr_ctl_i                          (new_esr_ctl),
    .pipe_esr_status_o                  (new_esr_status),
    .pipe_sbe_dbe_overflow_o            (new_sbe_dbe_overflow),
    .my_bank_id_i                       (my_bank_id_i),
    .my_l3_virtual_shire_id_i           (my_l3_virtual_shire_id_i),
    .my_scp_virtual_shire_id_i          (my_scp_virtual_shire_id_i)
  );

  assign new_pipe_busy_sub_bank_o = new_pipe_busy.sub_bank_busy;
  assign new_tag_rsp_info_o       = new_tag_rsp;
  assign new_data_rsp_pre_info_o  = new_data_rsp_pre;
  assign new_data_rsp_info_o      = new_data_rsp;
  assign new_cbuf_rsp_info_o      = new_cbuf_rsp;
  assign new_idx_cop_req_info_o   = new_idx_cop_req;

  // ════════════════════════════════════════════════════════
  // ORIGINAL module instance
  // ════════════════════════════════════════════════════════

  sc_pipe_busy_t      orig_pipe_busy;
  sc_pipe_tag_rsp_t   orig_tag_rsp;
  sc_pipe_data_rsp_pre_t orig_data_rsp_pre;
  sc_pipe_data_rsp_t  orig_data_rsp;
  sc_pipe_cbuf_rsp_t  orig_cbuf_rsp;
  sc_pipe_dataq_rd_req_t orig_dataq_rd_req;
  sc_pipe_idx_cop_req_t  orig_idx_cop_req;
  sc_pipe_rbuf_state_t   orig_rbuf_state;
  sc_pipe_rbuf_rd_rsp_t  orig_rbuf_rd_rsp;
  sc_pipe_bist_rsp_t     orig_bist_rsp;
  sc_perfmon_rsp_info_t  orig_perfmon_rsp;
  sc_pipe_esr_status_t   orig_esr_status;
  sc_pipe_sbe_dbe_overflow_t orig_sbe_dbe_overflow;

  shire_cache_pipe u_orig (
    .clock                              (clk_i),
    .reset_in                           (reset),
    .reset_c_in                         (reset),

    .dft__reset_byp_hv                  (1'b0),
    .dft__reset_hv                      (1'b0),
    .dft__sram_clock                    (1'b0),
    .dft__clk_override                  (1'b0),

    .pipe_start                         (pipe_start_i),
    .pipe_req_info                      (orig_pipe_req_info),
    .pipe_active                        (orig_pipe_active_o),
    .pipe_busy                          (orig_pipe_busy),
    .pipe_squash                        (orig_pipe_squash_o),

    .pipe_tag_rsp_valid                 (orig_tag_rsp_valid_o),
    .pipe_tag_rsp_info                  (orig_tag_rsp),

    .pipe_data_rsp_pre_valid            (orig_data_rsp_pre_valid_o),
    .pipe_data_rsp_pre_info             (orig_data_rsp_pre),

    .pipe_data_rsp_valid                (orig_data_rsp_valid_o),
    .pipe_data_rsp_info                 (orig_data_rsp),

    .pipe_cbuf_rsp_valid                (orig_cbuf_rsp_valid_o),
    .pipe_cbuf_rsp_info                 (orig_cbuf_rsp),

    .pipe_rbuf_clear_req_info           (orig_rbuf_clear_req),

    .pipe_rbuf_rd_req_ready             (orig_rbuf_rd_req_ready_o),
    .pipe_rbuf_rd_req_valid             (pipe_rbuf_rd_req_valid_i),
    .pipe_rbuf_rd_req_info              (orig_rbuf_rd_req_info),

    .pipe_rbuf_rd_rsp_ready             (pipe_rbuf_rd_rsp_ready_i),
    .pipe_rbuf_rd_rsp_valid             (),
    .pipe_rbuf_rd_rsp_info              (orig_rbuf_rd_rsp),

    .pipe_rbuf_state                    (orig_rbuf_state),

    .pipe_dataq_rd_req_valid            (orig_dataq_rd_req_valid_o),
    .pipe_dataq_rd_req_info             (orig_dataq_rd_req),
    .pipe_dataq_rd_rsp_info             (orig_dataq_rd_rsp),

    .pipe_idx_cop_aborted               (orig_idx_cop_aborted_o),
    .pipe_idx_cop_l2_busy               (orig_idx_cop_l2_busy_o),
    .pipe_idx_cop_l3_busy               (orig_idx_cop_l3_busy_o),
    .pipe_idx_cop_req_ready             (pipe_idx_cop_req_ready_i),
    .pipe_idx_cop_req_valid             (orig_idx_cop_req_valid_o),
    .pipe_idx_cop_req_info              (orig_idx_cop_req),
    .pipe_idx_cop_is_active             (orig_idx_cop_is_active_o),

    .mbist_on                           (1'b0),
    .pipe_bist_req_info                 (orig_bist_req),
    .pipe_bist_rsp_info                 (orig_bist_rsp),

    .perfmon_req_info                   (orig_perfmon_req),
    .perfmon_rsp_info                   (orig_perfmon_rsp),

    .esr_ctl                            (orig_esr_ctl),
    .esr_sc_l3_shire_swizzle_ctl        (orig_l3_swizzle_ctl),
    .esr_shire_cache_ram_cfg            (orig_ram_cfg),
    .pipe_esr_status                    (orig_esr_status),
    .pipe_sbe_dbe_overflow              (orig_sbe_dbe_overflow),
    .my_bank_id                         (my_bank_id_i),
    .my_l3_virtual_shire_id             (my_l3_virtual_shire_id_i),
    .my_scp_virtual_shire_id            (my_scp_virtual_shire_id_i)
  );

  assign orig_pipe_busy_sub_bank_o = orig_pipe_busy.sub_bank_busy;
  assign orig_tag_rsp_info_o       = orig_tag_rsp;
  assign orig_data_rsp_pre_info_o  = orig_data_rsp_pre;
  assign orig_data_rsp_info_o      = orig_data_rsp;
  assign orig_cbuf_rsp_info_o      = orig_cbuf_rsp;
  assign orig_idx_cop_req_info_o   = orig_idx_cop_req;

endmodule
