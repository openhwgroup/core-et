// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache pipe — top-level pipeline module.
//
// Instantiates:
//   - sub_bank control     (shirecache_pipe_sub_bank)
//   - coalesce buffer      (shirecache_pipe_cbuf)
//   - read buffer          (shirecache_pipe_rbuf)
//   - memory wrappers      (shirecache_pipe_sub_bank_mem × SubBanks)
//   - idx_cop state machine(shirecache_pipe_idx_cop_sm)
//   - set calculators      (shirecache_pipe_set_calc × 3)
//   - L3 swizzle get       (shirecache_pipe_l3_swizzle_get)
//
// Pipeline valid tracking (rqa_d0 .. rqa_d22) drives sub-bank busy logic,
// bubble insertion for writes/atomics, and clock gating.
//
// Replaces: shire_cache_pipe (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni / rst_c_ni)
//   - lowRISC naming conventions (_i/_o)
//   - Package types instead of `define macros
//   - prim_rst_sync instead of rst_repeat
//   - prim_clk_gate instead of et_clk_gate
//   - DFT consolidated into dft_pkg::dft_t struct + separate dft_sram_clk_i
//   - Per-sub-bank mem instantiation inlined (replaces shire_cache_pipe_mem wrapper)
//   - ram_cfg_pkg::ram_cfg_t + dft_t replace esr_shire_cache_ram_cfg_t

module shirecache_pipe
  import shirecache_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
#(
  parameter int unsigned SETS_PER_SUB_BANK = SetsPerSubBank
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic                        rst_c_ni,

  // DFT
  input  dft_t                        dft_i,
  input  logic                        dft_sram_clk_i,

  // RAM configuration
  input  ram_cfg_t                    ram_cfg_i,

  // Request interface from reqq
  input  logic                        pipe_start_i,
  input  reqq_pipe_req_t              pipe_req_info_i,
  output logic                        pipe_active_o,
  output pipe_busy_t                  pipe_busy_o,
  output logic                        pipe_squash_o,

  // Response interfaces to reqq
  output logic                        pipe_tag_rsp_valid_o,
  output tag_rsp_t                    pipe_tag_rsp_info_o,

  output logic                        pipe_data_rsp_pre_valid_o,
  output data_rsp_pre_t               pipe_data_rsp_pre_info_o,

  output logic                        pipe_data_rsp_valid_o,
  output data_rsp_t                   pipe_data_rsp_info_o,

  output logic                        pipe_cbuf_rsp_valid_o,
  output cbuf_rsp_t                   pipe_cbuf_rsp_info_o,

  // Read buffer clear request from reqq
  input  rbuf_clear_req_t             pipe_rbuf_clear_req_info_i,  // valids within struct

  // Read buffer request from reqq
  output logic                        pipe_rbuf_rd_req_ready_o,
  input  logic                        pipe_rbuf_rd_req_valid_i,
  input  rbuf_rd_req_t                pipe_rbuf_rd_req_info_i,

  // Read buffer data response to reqq/mesh
  input  logic                        pipe_rbuf_rd_rsp_ready_i,
  output logic                        pipe_rbuf_rd_rsp_valid_o,
  output rbuf_rd_rsp_t                pipe_rbuf_rd_rsp_info_o,

  // Read buffer status to reqq
  output rbuf_state_t                 pipe_rbuf_state_o,

  // Data interface to dataq
  output logic                        pipe_dataq_rd_req_valid_o,
  output dataq_rd_req_t               pipe_dataq_rd_req_info_o,
  input  dataq_rd_rsp_t               pipe_dataq_rd_rsp_info_i,

  // Request interface to reqq (idx_cop)
  output logic                        pipe_idx_cop_aborted_o,
  output logic                        pipe_idx_cop_l2_busy_o,
  output logic                        pipe_idx_cop_l3_busy_o,
  input  logic                        pipe_idx_cop_req_ready_i,
  output logic                        pipe_idx_cop_req_valid_o,
  output idx_cop_req_t                pipe_idx_cop_req_info_o,
  output logic                        pipe_idx_cop_is_active_o,

  // BIST
  input  logic                        mbist_on_i,
  input  bist_req_t                   pipe_bist_req_info_i,
  output bist_rsp_t                   pipe_bist_rsp_info_o,

  // Performance monitor
  input  perfmon_req_info_t           perfmon_req_info_i,
  output perfmon_rsp_info_t           perfmon_rsp_info_o,

  // ESRs and constants
  input  bank_esr_ctl_t               esr_ctl_i,
  output pipe_esr_status_t            pipe_esr_status_o,
  output sbe_dbe_overflow_t           pipe_sbe_dbe_overflow_o,
  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [L3ShireIdSize-1:0]    my_l3_virtual_shire_id_i,
  input  logic [ScpShireIdSize-1:0]   my_scp_virtual_shire_id_i
);

  // data_ram_req_info xcheck_disable bit unused at pipe level; struct fields partially used.
  /* verilator lint_off UNUSEDSIGNAL */  // xcheck_disable and partial struct fields unused at pipe interconnect level

  // ════════════════════════════════════════════════════════
  // Local constants
  // ════════════════════════════════════════════════════════

  // SCP set address range for out-of-range checking.
  // SC_SCP_SET_LSB  = ScpSetLsb = ScpWayLsb + WayIdSize = 12
  // SC_SCP_SET_MAX_MSB = ScpShireLsb - 1 = 22
  // SC_SCP_SET_ID_MAX_SIZE = ScpShireLsb - ScpSetLsb = 11
  localparam int unsigned ScpSetIdMaxSize = ScpShireLsb - ScpSetLsb;

  // ════════════════════════════════════════════════════════
  // Reset synchronizers
  // ════════════════════════════════════════════════════════

  logic rst_n;
  logic rst_c_n;

  prim_rst_sync u_rst_sync_reset (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_n)
  );

  prim_rst_sync u_rst_sync_reset_c (
    .clk_i  (clk_i),
    .rst_ni (rst_c_ni),
    .dft_i  (dft_i),
    .rst_no (rst_c_n)
  );

  // ═════════════════════════════════════════════════════��══
  // Extract ESR fields for local use
  // ════════════════════════════════════════════════════════

  logic                       esr_sc_ram_deep_sleep;
  logic                       esr_sc_ram_shut_down;
  logic [RamDelaySize-1:0]    esr_sc_ram_delay;
  logic                       esr_sc_l2_rbuf_enable;
  logic                       esr_sc_scp_rbuf_enable;
  logic                       esr_sc_zero_state_enable;
  logic                       esr_sc_allow_only_1_req_per_sub_bank;
  logic                       esr_sc_allow_only_1_req_per_bank;
  logic [CbufSize-1:0]        esr_sc_cbuf_entry_enable;
  logic                       esr_sc_all_shire_aliasing;
  logic                       esr_sc_two_shire_aliasing_use_shire_lsb;
  logic [SetIdSize-1:0]       esr_sc_l2_set_base;
  logic [SetAllocSize-1:0]    esr_sc_l2_set_size;
  logic [SetIdSize-1:0]       esr_sc_l2_set_mask;
  logic [SetIdSize-1:0]       esr_sc_l2_tag_mask;
  logic [SetIdSize-1:0]       esr_sc_l3_set_base;
  logic [SetAllocSize-1:0]    esr_sc_l3_set_size;
  logic [SetIdSize-1:0]       esr_sc_l3_set_mask;
  logic [SetIdSize-1:0]       esr_sc_l3_tag_mask;
  logic [SetIdSize-1:0]       esr_sc_scp_set_base;
  logic [SetAllocSize-1:0]    esr_sc_scp_set_size;
  logic [SetIdSize-1:0]       esr_sc_scp_set_mask;
  logic [SetIdSize-1:0]       esr_sc_scp_tag_mask;

  assign esr_sc_ram_deep_sleep                   = esr_ctl_i.esr_sc_ram_deep_sleep;
  assign esr_sc_ram_shut_down                    = esr_ctl_i.esr_sc_ram_shut_down;
  assign esr_sc_ram_delay                        = esr_ctl_i.esr_sc_ram_delay;
  assign esr_sc_l2_rbuf_enable                   = esr_ctl_i.esr_sc_l2_rbuf_enable;
  assign esr_sc_scp_rbuf_enable                  = esr_ctl_i.esr_sc_scp_rbuf_enable;
  assign esr_sc_zero_state_enable                = esr_ctl_i.esr_sc_zero_state_enable;
  assign esr_sc_allow_only_1_req_per_sub_bank    = esr_ctl_i.esr_sc_allow_only_1_req_per_sub_bank;
  assign esr_sc_allow_only_1_req_per_bank        = esr_ctl_i.esr_sc_allow_only_1_req_per_bank;
  assign esr_sc_cbuf_entry_enable                = esr_ctl_i.esr_sc_cbuf_entry_enable;
  assign esr_sc_all_shire_aliasing               = esr_ctl_i.esr_sc_all_shire_aliasing;
  assign esr_sc_two_shire_aliasing_use_shire_lsb = esr_ctl_i.esr_sc_two_shire_aliasing_use_shire_lsb;
  assign esr_sc_l2_set_base                      = esr_ctl_i.esr_sc_l2_set_base;
  assign esr_sc_l2_set_size                      = esr_ctl_i.esr_sc_l2_set_size;
  assign esr_sc_l2_set_mask                      = esr_ctl_i.esr_sc_l2_set_mask;
  assign esr_sc_l2_tag_mask                      = esr_ctl_i.esr_sc_l2_tag_mask;
  assign esr_sc_l3_set_base                      = esr_ctl_i.esr_sc_l3_set_base;
  assign esr_sc_l3_set_size                      = esr_ctl_i.esr_sc_l3_set_size;
  assign esr_sc_l3_set_mask                      = esr_ctl_i.esr_sc_l3_set_mask;
  assign esr_sc_l3_tag_mask                      = esr_ctl_i.esr_sc_l3_tag_mask;
  assign esr_sc_scp_set_base                     = esr_ctl_i.esr_sc_scp_set_base;
  assign esr_sc_scp_set_size                     = esr_ctl_i.esr_sc_scp_set_size;
  assign esr_sc_scp_set_mask                     = esr_ctl_i.esr_sc_scp_set_mask;
  assign esr_sc_scp_tag_mask                     = esr_ctl_i.esr_sc_scp_tag_mask;

  // Construct l3_swizzle_ctl from esr_ctl_i fields
  l3_swizzle_ctl_t l3_swizzle_ctl;
  assign l3_swizzle_ctl.all_shire_aliasing               = esr_ctl_i.esr_sc_all_shire_aliasing;
  assign l3_swizzle_ctl.two_shire_aliasing_use_shire_lsb = esr_ctl_i.esr_sc_two_shire_aliasing_use_shire_lsb;
  assign l3_swizzle_ctl.sub_bank_sel_b2                  = esr_ctl_i.esr_sc_sub_bank_sel_b2;
  assign l3_swizzle_ctl.sub_bank_sel_b1                  = esr_ctl_i.esr_sc_sub_bank_sel_b1;
  assign l3_swizzle_ctl.sub_bank_sel_b0                  = esr_ctl_i.esr_sc_sub_bank_sel_b0;
  assign l3_swizzle_ctl.bank_sel_b2                      = esr_ctl_i.esr_sc_bank_sel_b2;
  assign l3_swizzle_ctl.bank_sel_b1                      = esr_ctl_i.esr_sc_bank_sel_b1;
  assign l3_swizzle_ctl.bank_sel_b0                      = esr_ctl_i.esr_sc_bank_sel_b0;
  assign l3_swizzle_ctl.shire_sel_b5                     = esr_ctl_i.esr_sc_shire_sel_b5;
  assign l3_swizzle_ctl.shire_sel_b4                     = esr_ctl_i.esr_sc_shire_sel_b4;
  assign l3_swizzle_ctl.shire_sel_b3                     = esr_ctl_i.esr_sc_shire_sel_b3;
  assign l3_swizzle_ctl.shire_sel_b2                     = esr_ctl_i.esr_sc_shire_sel_b2;
  assign l3_swizzle_ctl.shire_sel_b1                     = esr_ctl_i.esr_sc_shire_sel_b1;
  assign l3_swizzle_ctl.shire_sel_b0                     = esr_ctl_i.esr_sc_shire_sel_b0;

  // ════════════════════════════════════════════════════════
  // _rqa (request arbitration) stage
  // ════════════════════════════════════════════════════════
  //   - incoming signals at _rqa are at end of long combinational path
  //   - calculate sub_bank_busy for immediate feedback to arbiter
  //   - flop incoming request for read buffer on next stage

  // pipe_valid_rqa_d*
  wire  pipe_valid_rqa_d0 = pipe_start_i;
  logic pipe_valid_rqa_d1;
  logic pipe_valid_rqa_d2;
  logic pipe_valid_rqa_d3;
  logic pipe_valid_rqa_d4;
  logic pipe_valid_rqa_d5;
  logic pipe_valid_rqa_d6;
  logic pipe_valid_rqa_d7;
  logic pipe_valid_rqa_d8;
  logic pipe_valid_rqa_d9;
  logic pipe_valid_rqa_d10;
  logic pipe_valid_rqa_d11;
  logic pipe_valid_rqa_d12;

  wire any_pipe_valid_rqa_d1_d12 =
    pipe_valid_rqa_d1  ||
    pipe_valid_rqa_d2  ||
    pipe_valid_rqa_d3  ||
    pipe_valid_rqa_d4  ||
    pipe_valid_rqa_d5  ||
    pipe_valid_rqa_d6  ||
    pipe_valid_rqa_d7  ||
    pipe_valid_rqa_d8  ||
    pipe_valid_rqa_d9  ||
    pipe_valid_rqa_d10 ||
    pipe_valid_rqa_d11 ||
    pipe_valid_rqa_d12;

  // pipe_valid_rqa_d1 — keep separate for timing closure of incoming pipe_start_i
  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                                        pipe_valid_rqa_d1 <= '0;
    else if (pipe_valid_rqa_d1 || pipe_valid_rqa_d0)   pipe_valid_rqa_d1 <= pipe_valid_rqa_d0;
  end

  // pipe_valid_rqa_d2..d12
  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      pipe_valid_rqa_d2  <= '0;
      pipe_valid_rqa_d3  <= '0;
      pipe_valid_rqa_d4  <= '0;
      pipe_valid_rqa_d5  <= '0;
      pipe_valid_rqa_d6  <= '0;
      pipe_valid_rqa_d7  <= '0;
      pipe_valid_rqa_d8  <= '0;
      pipe_valid_rqa_d9  <= '0;
      pipe_valid_rqa_d10 <= '0;
      pipe_valid_rqa_d11 <= '0;
      pipe_valid_rqa_d12 <= '0;
    end else if (any_pipe_valid_rqa_d1_d12) begin
      pipe_valid_rqa_d2  <= pipe_valid_rqa_d1;
      pipe_valid_rqa_d3  <= pipe_valid_rqa_d2;
      pipe_valid_rqa_d4  <= pipe_valid_rqa_d3;
      pipe_valid_rqa_d5  <= pipe_valid_rqa_d4;
      pipe_valid_rqa_d6  <= pipe_valid_rqa_d5;
      pipe_valid_rqa_d7  <= pipe_valid_rqa_d6;
      pipe_valid_rqa_d8  <= pipe_valid_rqa_d7;
      pipe_valid_rqa_d9  <= pipe_valid_rqa_d8;
      pipe_valid_rqa_d10 <= pipe_valid_rqa_d9;
      pipe_valid_rqa_d11 <= pipe_valid_rqa_d10;
      pipe_valid_rqa_d12 <= pipe_valid_rqa_d11;
    end
  end

  wire esr_sc_ram_delay_2 = (esr_sc_ram_delay == RamDelaySize'(2));
  wire esr_sc_ram_delay_3 = (esr_sc_ram_delay == RamDelaySize'(3));
  wire esr_sc_ram_delay_4 = (esr_sc_ram_delay == RamDelaySize'(4));

  logic pipe_valid_rqa_d13;
  logic pipe_valid_rqa_d14;
  logic pipe_valid_rqa_d15;
  logic pipe_valid_rqa_d16;
  logic pipe_valid_rqa_d17;
  logic pipe_valid_rqa_d18;
  logic pipe_valid_rqa_d19;
  logic pipe_valid_rqa_d20;
  logic pipe_valid_rqa_d21;
  logic pipe_valid_rqa_d22;

  wire any_pipe_valid_rqa_d12_d22 =
    pipe_valid_rqa_d12 ||
    pipe_valid_rqa_d13 ||
    pipe_valid_rqa_d14 ||
    pipe_valid_rqa_d15 ||
    pipe_valid_rqa_d16 ||
    pipe_valid_rqa_d17 ||
    pipe_valid_rqa_d18 ||
    pipe_valid_rqa_d19 ||
    pipe_valid_rqa_d20 ||
    pipe_valid_rqa_d21 ||
    pipe_valid_rqa_d22;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      pipe_valid_rqa_d13 <= '0;
      pipe_valid_rqa_d14 <= '0;
      pipe_valid_rqa_d15 <= '0;
      pipe_valid_rqa_d16 <= '0;
      pipe_valid_rqa_d17 <= '0;
      pipe_valid_rqa_d18 <= '0;
      pipe_valid_rqa_d19 <= '0;
      pipe_valid_rqa_d20 <= '0;
      pipe_valid_rqa_d21 <= '0;
      pipe_valid_rqa_d22 <= '0;
    end else if (any_pipe_valid_rqa_d12_d22) begin
      pipe_valid_rqa_d13 <= pipe_valid_rqa_d12;
      pipe_valid_rqa_d14 <= pipe_valid_rqa_d13;
      pipe_valid_rqa_d15 <= pipe_valid_rqa_d14;
      pipe_valid_rqa_d16 <= pipe_valid_rqa_d15 && !esr_sc_ram_delay_2;
      pipe_valid_rqa_d17 <= pipe_valid_rqa_d16;
      pipe_valid_rqa_d18 <= pipe_valid_rqa_d17;
      pipe_valid_rqa_d19 <= pipe_valid_rqa_d18;
      pipe_valid_rqa_d20 <= pipe_valid_rqa_d19;
      pipe_valid_rqa_d21 <= pipe_valid_rqa_d20;
      pipe_valid_rqa_d22 <= pipe_valid_rqa_d21 && !esr_sc_ram_delay_4;
    end
  end

  wire pipe_active_next = pipe_valid_rqa_d0 || any_pipe_valid_rqa_d1_d12 || any_pipe_valid_rqa_d12_d22;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                                   pipe_active_o <= '0;
    else if (pipe_active_o || pipe_active_next)    pipe_active_o <= pipe_active_next;
  end

  // ── Get sub_bank from L3 swizzle ────────────────────────
  logic [SubBankIdSize-1:0]  l3_sub_bank_id_rqa;
  logic [BankIdSize-1:0]     l3_bank_id_rqa;
  logic [L3ShireIdSize-1:0]  l3_shire_id_rqa;

  shirecache_pipe_l3_swizzle_get u_l3_swizzle_get_rqa (
    .address_i      (pipe_req_info_i.address),
    .swizzle_ctl_i  (l3_swizzle_ctl),
    .sub_bank_id_o  (l3_sub_bank_id_rqa),
    .bank_id_o      (l3_bank_id_rqa),
    .shire_id_o     (l3_shire_id_rqa)
  );

  wire                       l3_operation_rqa        = pipe_req_info_i.address_space == AddressL3;
  wire [SubBankIdSize-1:0]   sub_bank_id_check_rqa   = l3_operation_rqa ? l3_sub_bank_id_rqa :
                                                                           pipe_req_info_i.address[SubBankLsb +: SubBankIdSize];
  wire [SubBankIdSize-1:0]   sub_bank_id_rqa         = pipe_req_info_i.sub_bank_id;
  wire [SubBanks-1:0]        pipe_sub_bank_rqa       = SubBanks'(1'b1) << sub_bank_id_rqa;

  // pipe_sub_bank_rqa_d0 / pipe_sub_bank_busy_rqa_d0
  wire [SubBanks-1:0] pipe_sub_bank_rqa_d0      = {SubBanks{pipe_valid_rqa_d0}} & pipe_sub_bank_rqa;
  wire [SubBanks-1:0] pipe_sub_bank_busy_rqa_d0 = {SubBanks{pipe_valid_rqa_d0}} & (pipe_sub_bank_rqa | {SubBanks{esr_sc_allow_only_1_req_per_bank}});

  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d1;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d2;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d3;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d4;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d5;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d6;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d7;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d8;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d9;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d10;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d11;
  logic [SubBanks-1:0] pipe_sub_bank_busy_rqa_d12;

  // pipe_sub_bank_busy_rqa_d1 — separate for timing closure of incoming pipe_start_i
  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                                        pipe_sub_bank_busy_rqa_d1 <= '0;
    else if (pipe_valid_rqa_d1 || pipe_valid_rqa_d0)   pipe_sub_bank_busy_rqa_d1 <= pipe_sub_bank_busy_rqa_d0;
  end

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      pipe_sub_bank_busy_rqa_d2  <= '0;
      pipe_sub_bank_busy_rqa_d3  <= '0;
      pipe_sub_bank_busy_rqa_d4  <= '0;
      pipe_sub_bank_busy_rqa_d5  <= '0;
      pipe_sub_bank_busy_rqa_d6  <= '0;
      pipe_sub_bank_busy_rqa_d7  <= '0;
      pipe_sub_bank_busy_rqa_d8  <= '0;
      pipe_sub_bank_busy_rqa_d9  <= '0;
      pipe_sub_bank_busy_rqa_d10 <= '0;
      pipe_sub_bank_busy_rqa_d11 <= '0;
      pipe_sub_bank_busy_rqa_d12 <= '0;
    end else begin
      if (pipe_valid_rqa_d2  || pipe_valid_rqa_d1)  pipe_sub_bank_busy_rqa_d2  <= pipe_sub_bank_busy_rqa_d1;
      if (pipe_valid_rqa_d3  || pipe_valid_rqa_d2)  pipe_sub_bank_busy_rqa_d3  <= pipe_sub_bank_busy_rqa_d2;
      if (pipe_valid_rqa_d4  || pipe_valid_rqa_d3)  pipe_sub_bank_busy_rqa_d4  <= pipe_sub_bank_busy_rqa_d3;
      if (pipe_valid_rqa_d5  || pipe_valid_rqa_d4)  pipe_sub_bank_busy_rqa_d5  <= pipe_sub_bank_busy_rqa_d4;
      if (pipe_valid_rqa_d6  || pipe_valid_rqa_d5)  pipe_sub_bank_busy_rqa_d6  <= pipe_sub_bank_busy_rqa_d5;
      if (pipe_valid_rqa_d7  || pipe_valid_rqa_d6)  pipe_sub_bank_busy_rqa_d7  <= pipe_sub_bank_busy_rqa_d6;
      if (pipe_valid_rqa_d8  || pipe_valid_rqa_d7)  pipe_sub_bank_busy_rqa_d8  <= pipe_sub_bank_busy_rqa_d7;
      if (pipe_valid_rqa_d9  || pipe_valid_rqa_d8)  pipe_sub_bank_busy_rqa_d9  <= pipe_sub_bank_busy_rqa_d8;
      if (pipe_valid_rqa_d10 || pipe_valid_rqa_d9)  pipe_sub_bank_busy_rqa_d10 <= pipe_sub_bank_busy_rqa_d9;
      if (pipe_valid_rqa_d11 || pipe_valid_rqa_d10) pipe_sub_bank_busy_rqa_d11 <= pipe_sub_bank_busy_rqa_d10;
      if (pipe_valid_rqa_d12 || pipe_valid_rqa_d11) pipe_sub_bank_busy_rqa_d12 <= pipe_sub_bank_busy_rqa_d11;
    end
  end

  reqq_pipe_req_t pipe_req_info_tap;  // need access for bubble/atomic detection

  // ── Bubble / atomic detection ───────────────────────────
  // sub_bank_busy: rqa_d1 is same cycle as _tap

  wire bubble_operation_rqa_d1 = pipe_valid_rqa_d1 && (
    esr_sc_allow_only_1_req_per_sub_bank ||
    esr_sc_allow_only_1_req_per_bank ||
    (pipe_req_info_tap.opcode == L2Write) ||
    (pipe_req_info_tap.opcode == L2WritePartial) ||
    (pipe_req_info_tap.opcode == L2WriteAround) ||
    (pipe_req_info_tap.opcode == L2Lock) ||
    (pipe_req_info_tap.opcode == L2Atomic) ||
    (pipe_req_info_tap.opcode == L2Fill) ||
    (pipe_req_info_tap.opcode == L2Scrub) ||
    (pipe_req_info_tap.opcode == L3Write) ||
    (pipe_req_info_tap.opcode == L3WritePartial) ||
    (pipe_req_info_tap.opcode == L3FlushWData) ||
    (pipe_req_info_tap.opcode == L3EvictWData) ||
    (pipe_req_info_tap.opcode == L3Atomic) ||
    (pipe_req_info_tap.opcode == L3Fill) ||
    (pipe_req_info_tap.opcode == L3Scrub) ||
    (pipe_req_info_tap.opcode == ScpWrite) ||
    (pipe_req_info_tap.opcode == ScpWritePartial) ||
    (pipe_req_info_tap.opcode == ScpAtomic) ||
    (pipe_req_info_tap.opcode == ScpFill) ||
    (pipe_req_info_tap.opcode == ScpScrub) ||
    (pipe_req_info_tap.opcode == ScpZero) ||
    (pipe_req_info_tap.opcode == IdxWrite)
  );

  wire atomic_operation_rqa_d1 = pipe_valid_rqa_d1 && (
    esr_sc_allow_only_1_req_per_sub_bank ||
    esr_sc_allow_only_1_req_per_bank ||
    (pipe_req_info_tap.opcode == L2WritePartial) ||
    (pipe_req_info_tap.opcode == L2Atomic) ||
    (pipe_req_info_tap.opcode == L3WritePartial) ||
    (pipe_req_info_tap.opcode == L3Atomic) ||
    (pipe_req_info_tap.opcode == ScpWritePartial) ||
    (pipe_req_info_tap.opcode == ScpAtomic)
  );

  wire  pipe_bubble_rqa_d1 = bubble_operation_rqa_d1;
  logic pipe_bubble_rqa_d2;
  logic pipe_bubble_rqa_d3;
  logic pipe_bubble_rqa_d4;
  logic pipe_bubble_rqa_d5;
  logic pipe_bubble_rqa_d6;
  logic pipe_bubble_rqa_d7;
  logic pipe_bubble_rqa_d8;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      pipe_bubble_rqa_d2 <= '0;
      pipe_bubble_rqa_d3 <= '0;
      pipe_bubble_rqa_d4 <= '0;
      pipe_bubble_rqa_d5 <= '0;
      pipe_bubble_rqa_d6 <= '0;
      pipe_bubble_rqa_d7 <= '0;
      pipe_bubble_rqa_d8 <= '0;
    end else if (any_pipe_valid_rqa_d1_d12) begin
      pipe_bubble_rqa_d2 <= pipe_bubble_rqa_d1;
      pipe_bubble_rqa_d3 <= pipe_bubble_rqa_d2;
      pipe_bubble_rqa_d4 <= pipe_bubble_rqa_d3;
      pipe_bubble_rqa_d5 <= pipe_bubble_rqa_d4;
      pipe_bubble_rqa_d6 <= pipe_bubble_rqa_d5;
      pipe_bubble_rqa_d7 <= pipe_bubble_rqa_d6;
      pipe_bubble_rqa_d8 <= pipe_bubble_rqa_d7;
    end
  end

  wire  pipe_atomic_rqa_d1 = atomic_operation_rqa_d1;
  logic pipe_atomic_rqa_d2;
  logic pipe_atomic_rqa_d3;
  logic pipe_atomic_rqa_d4;
  logic pipe_atomic_rqa_d5;
  logic pipe_atomic_rqa_d6;
  logic pipe_atomic_rqa_d7;
  logic pipe_atomic_rqa_d8;
  logic pipe_atomic_rqa_d9;
  logic pipe_atomic_rqa_d10;
  logic pipe_atomic_rqa_d11;
  logic pipe_atomic_rqa_d12;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      pipe_atomic_rqa_d2  <= '0;
      pipe_atomic_rqa_d3  <= '0;
      pipe_atomic_rqa_d4  <= '0;
      pipe_atomic_rqa_d5  <= '0;
      pipe_atomic_rqa_d6  <= '0;
      pipe_atomic_rqa_d7  <= '0;
      pipe_atomic_rqa_d8  <= '0;
      pipe_atomic_rqa_d9  <= '0;
      pipe_atomic_rqa_d10 <= '0;
      pipe_atomic_rqa_d11 <= '0;
      pipe_atomic_rqa_d12 <= '0;
    end else if (any_pipe_valid_rqa_d1_d12) begin
      pipe_atomic_rqa_d2  <= pipe_atomic_rqa_d1;
      pipe_atomic_rqa_d3  <= pipe_atomic_rqa_d2;
      pipe_atomic_rqa_d4  <= pipe_atomic_rqa_d3;
      pipe_atomic_rqa_d5  <= pipe_atomic_rqa_d4;
      pipe_atomic_rqa_d6  <= pipe_atomic_rqa_d5;
      pipe_atomic_rqa_d7  <= pipe_atomic_rqa_d6;
      pipe_atomic_rqa_d8  <= pipe_atomic_rqa_d7;
      pipe_atomic_rqa_d9  <= pipe_atomic_rqa_d8;
      pipe_atomic_rqa_d10 <= pipe_atomic_rqa_d9;
      pipe_atomic_rqa_d11 <= pipe_atomic_rqa_d10;
      pipe_atomic_rqa_d12 <= pipe_atomic_rqa_d11;
    end
  end

  // ── sub_bank_busy calculation ───────────────────────────
  logic [SubBanks-1:0] sub_bank_busy_rqa_next;

  always_comb begin : sub_busy_block
    if (esr_sc_ram_delay_2) begin
      sub_bank_busy_rqa_next =
         pipe_sub_bank_busy_rqa_d0                                          |
        (pipe_sub_bank_busy_rqa_d1  & {SubBanks{pipe_atomic_rqa_d1}})       |
         pipe_sub_bank_busy_rqa_d2                                          |
        (pipe_sub_bank_busy_rqa_d3  & {SubBanks{pipe_atomic_rqa_d3}})       |
         pipe_sub_bank_busy_rqa_d4                                          |
        (pipe_sub_bank_busy_rqa_d5  & {SubBanks{pipe_bubble_rqa_d5}})       |
         pipe_sub_bank_busy_rqa_d6                                          |
        (pipe_sub_bank_busy_rqa_d7  & {SubBanks{pipe_atomic_rqa_d7}})       |
        (pipe_sub_bank_busy_rqa_d8  & {SubBanks{pipe_atomic_rqa_d8}});
    end
    else if (esr_sc_ram_delay_3) begin
      sub_bank_busy_rqa_next =
         pipe_sub_bank_busy_rqa_d0                                          |
         pipe_sub_bank_busy_rqa_d1                                          |
        (pipe_sub_bank_busy_rqa_d2  & {SubBanks{pipe_atomic_rqa_d2}})       |
         pipe_sub_bank_busy_rqa_d3                                          |
         pipe_sub_bank_busy_rqa_d4                                          |
        (pipe_sub_bank_busy_rqa_d5  & {SubBanks{pipe_atomic_rqa_d5}})       |
         pipe_sub_bank_busy_rqa_d6                                          |
         pipe_sub_bank_busy_rqa_d7                                          |
        (pipe_sub_bank_busy_rqa_d8  & {SubBanks{pipe_bubble_rqa_d8}})       |
         pipe_sub_bank_busy_rqa_d9                                          |
         pipe_sub_bank_busy_rqa_d10                                         |
        (pipe_sub_bank_busy_rqa_d11 & {SubBanks{pipe_atomic_rqa_d11}})      |
        (pipe_sub_bank_busy_rqa_d12 & {SubBanks{pipe_atomic_rqa_d12}});
    end
    else begin
      // esr_sc_ram_delay_4
      sub_bank_busy_rqa_next =
         pipe_sub_bank_busy_rqa_d0                                          |
         pipe_sub_bank_busy_rqa_d1                                          |
         pipe_sub_bank_busy_rqa_d2                                          |
        (pipe_sub_bank_busy_rqa_d3  & {SubBanks{pipe_atomic_rqa_d3}})       |
         pipe_sub_bank_busy_rqa_d4                                          |
         pipe_sub_bank_busy_rqa_d5                                          |
         pipe_sub_bank_busy_rqa_d6                                          |
        (pipe_sub_bank_busy_rqa_d7  & {SubBanks{pipe_bubble_rqa_d7}})       |
         pipe_sub_bank_busy_rqa_d8                                          |
         pipe_sub_bank_busy_rqa_d9                                          |
         pipe_sub_bank_busy_rqa_d10                                         |
        (pipe_sub_bank_busy_rqa_d11 & {SubBanks{pipe_atomic_rqa_d11}})      |
        (pipe_sub_bank_busy_rqa_d12 & {SubBanks{pipe_atomic_rqa_d12}});
    end
  end

  logic [SubBanks-1:0] sub_bank_busy_rqa;
  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                                                sub_bank_busy_rqa <= '0;
    else if (|sub_bank_busy_rqa || |sub_bank_busy_rqa_next)    sub_bank_busy_rqa <= sub_bank_busy_rqa_next;
  end

  assign pipe_busy_o.sub_bank_busy = sub_bank_busy_rqa;

  // ════════════════════════════════════════════════════════
  // _tap (pre-stage before _ta) — same as _rqa_d1
  // ════════════════════════════════════════════════════════

  logic                       pipe_valid_tap;
  logic [SubBanks-1:0]        pipe_sub_bank_tap;
  // pipe_req_info_tap declared above for bubble detection
  logic [BankIdSize-1:0]      l3_bank_id_tap;
  logic [L3ShireIdSize-1:0]   l3_shire_id_tap;

  wire                        pipe_bubble_tap   = bubble_operation_rqa_d1;
  wire                        pipe_scp_read_tap = (pipe_req_info_tap.opcode == ScpRead);

  // pipe_valid_tap — replicating pipe_valid_rqa_d1 for timing
  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                                        pipe_valid_tap <= '0;
    else if (pipe_valid_tap || pipe_valid_rqa_d0)      pipe_valid_tap <= pipe_valid_rqa_d0;
  end

  always_ff @(posedge clk_i) begin
    if (pipe_valid_rqa_d0) begin
      pipe_sub_bank_tap <= pipe_sub_bank_rqa_d0;
      pipe_req_info_tap <= pipe_req_info_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (l3_operation_rqa && pipe_valid_rqa_d0) begin
      l3_bank_id_tap  <= l3_bank_id_rqa;
      l3_shire_id_tap <= l3_shire_id_rqa;
    end
  end

  // ── Address space detection ─────────────────────────────
  wire l2_operation_tap  = (pipe_req_info_tap.address_space == AddressL2);
  wire l3_operation_tap  = (pipe_req_info_tap.address_space == AddressL3);
  wire scp_operation_tap = (pipe_req_info_tap.address_space == AddressScp);
  wire idx_operation_tap = (pipe_req_info_tap.address_space == AddressIdx);

  // ── Region enabled logic (multi-cycled ESRs) ───────────
  wire esr_sc_l2_region_enabled  = (esr_sc_l2_set_size  != '0);
  wire esr_sc_l3_region_enabled  = (esr_sc_l3_set_size  != '0);
  wire esr_sc_scp_region_enabled = (esr_sc_scp_set_size != '0);

  wire [SetIdSize-1:0] l2_region_base  = esr_sc_l2_set_base;
  wire [SetIdSize-1:0] l3_region_base  = esr_sc_l3_set_base;
  wire [SetIdSize-1:0] scp_region_base = esr_sc_scp_set_base;

  wire [SetIdSize:0] l2_region_top_full  = esr_sc_l2_set_base  + esr_sc_l2_set_size  - 1'b1;
  wire [SetIdSize:0] l3_region_top_full  = esr_sc_l3_set_base  + esr_sc_l3_set_size  - 1'b1;
  wire [SetIdSize:0] scp_region_top_full = esr_sc_scp_set_base + esr_sc_scp_set_size - 1'b1;

  wire [SetIdSize-1:0] l2_region_top  = l2_region_top_full [SetIdSize-1:0];
  wire [SetIdSize-1:0] l3_region_top  = l3_region_top_full [SetIdSize-1:0];
  wire [SetIdSize-1:0] scp_region_top = scp_region_top_full[SetIdSize-1:0];

  // ── IdxRead address space determination ─────────────────
  // SC_IDX_SET_SEL_ADDR_RANGE = ScpSetLsb +: SetIdSize
  wire [SetIdSize-1:0] idx_set_tap = pipe_req_info_tap.address[ScpSetLsb +: SetIdSize];
  wire l2_idx_read_address_space_tap  = esr_sc_l2_region_enabled  && (idx_set_tap >= l2_region_base ) && (idx_set_tap <= l2_region_top);
  wire l3_idx_read_address_space_tap  = esr_sc_l3_region_enabled  && (idx_set_tap >= l3_region_base ) && (idx_set_tap <= l3_region_top);
  wire scp_idx_read_address_space_tap = esr_sc_scp_region_enabled && (idx_set_tap >= scp_region_base) && (idx_set_tap <= scp_region_top);

  address_space_e pipe_idx_address_space_tap;
  assign pipe_idx_address_space_tap =
    l2_idx_read_address_space_tap  ? AddressL2  :
    l3_idx_read_address_space_tap  ? AddressL3  :
    scp_idx_read_address_space_tap ? AddressScp :
                                     AddressIdx;  // did not hit any of the normal spaces

  // ── Decode error detection ──────────────────────────��───
  wire l2_op_to_non_en_region_tap  = l2_operation_tap  && !esr_sc_l2_region_enabled;
  wire l3_op_to_non_en_region_tap  = l3_operation_tap  && !esr_sc_l3_region_enabled;
  wire scp_op_to_non_en_region_tap = scp_operation_tap && !esr_sc_scp_region_enabled;

  // SCP set range check
  // SC_SCP_SET_MAX_SEL_ADDR_RANGE = ScpSetLsb +: ScpSetIdMaxSize
  wire [ScpSetIdMaxSize-1:0] scp_relative_index =
    pipe_req_info_tap.address[ScpSetLsb +: ScpSetIdMaxSize] - ScpSetIdMaxSize'(esr_sc_scp_set_base);
  wire scp_set_out_of_range_tap = scp_operation_tap && ({1'b0, scp_relative_index} >= (ScpSetIdMaxSize + 1)'(esr_sc_scp_set_size));

  // L3 shire address check
  logic l3_addr_shire_check_tap;
  generate
    if (L3ShireIdSize > 1) begin : gen_ac_shire_gt_2
      assign l3_addr_shire_check_tap =
        esr_sc_all_shire_aliasing                ? 1'b1 :
        esr_sc_two_shire_aliasing_use_shire_lsb  ? l3_shire_id_tap[L3ShireIdSize-1:1] == my_l3_virtual_shire_id_i[L3ShireIdSize-1:1] :
                                                    l3_shire_id_tap[L3ShireIdSize-2:0] == my_l3_virtual_shire_id_i[L3ShireIdSize-2:0];
    end else begin : gen_ac_shire2
      assign l3_addr_shire_check_tap = 1'b1;
    end
  endgenerate

  wire l3_shire_dec_err_tap   = l3_operation_tap  && !l3_addr_shire_check_tap;
  wire scp_shire_dec_err_tap  = scp_operation_tap && pipe_req_info_tap.address[ScpShireLsb +: ScpShireIdSize] != my_scp_virtual_shire_id_i;
  wire scp_region_dec_err_tap = scp_operation_tap && pipe_req_info_tap.address[ScpRegionLsb +: ScpRegionIdSize] != ScpRegionId;

  // ── Set calculators ─────────────────────────────────────
  logic [SetIdSize-1:0] l2_pipe_set_tap;
  shirecache_pipe_set_calc u_l2_pipe_set_calc (
    .region_op_i    (l2_operation_tap),
    .address_set_i  (pipe_req_info_tap.address[SetLsb +: SetIdSize]),
    .esr_set_base_i (esr_sc_l2_set_base),
    .esr_set_size_i (esr_sc_l2_set_size),
    .esr_set_mask_i (esr_sc_l2_set_mask),
    .esr_tag_mask_i (esr_sc_l2_tag_mask),
    .pipe_set_o     (l2_pipe_set_tap)
  );

  logic [SetIdSize-1:0] l3_pipe_set_tap;
  shirecache_pipe_set_calc u_l3_pipe_set_calc (
    .region_op_i    (l3_operation_tap),
    .address_set_i  (pipe_req_info_tap.address[L3SetLsb +: SetIdSize]),
    .esr_set_base_i (esr_sc_l3_set_base),
    .esr_set_size_i (esr_sc_l3_set_size),
    .esr_set_mask_i (esr_sc_l3_set_mask),
    .esr_tag_mask_i (esr_sc_l3_tag_mask),
    .pipe_set_o     (l3_pipe_set_tap)
  );

  logic [SetIdSize-1:0] scp_pipe_set_tap;
  shirecache_pipe_set_calc u_scp_pipe_set_calc (
    .region_op_i    (scp_operation_tap),
    .address_set_i  (pipe_req_info_tap.address[ScpSetLsb +: SetIdSize]),
    .esr_set_base_i (esr_sc_scp_set_base),
    .esr_set_size_i (esr_sc_scp_set_size),
    .esr_set_mask_i (esr_sc_scp_set_mask),
    .esr_tag_mask_i (esr_sc_scp_tag_mask),
    .pipe_set_o     (scp_pipe_set_tap)
  );

  wire [SetIdSize-1:0] idx_pipe_set_tap = {SetIdSize{idx_operation_tap}} & idx_set_tap;

  wire [SetIdSize-1:0] pipe_set_tap = l2_pipe_set_tap | l3_pipe_set_tap | scp_pipe_set_tap | idx_pipe_set_tap;

  // ── Assemble pipe_info_tap ──────────────────────────────
  pipe_info_t pipe_info_tap;

  assign pipe_info_tap.pipe_set               = pipe_set_tap;
  assign pipe_info_tap.pipe_sub_bank          = pipe_sub_bank_tap;
  assign pipe_info_tap.pipe_idx_address_space = pipe_idx_address_space_tap;
  assign pipe_info_tap.pipe_dec_err           =
    l3_shire_dec_err_tap        |
    scp_shire_dec_err_tap       |
    scp_region_dec_err_tap      |
    l2_op_to_non_en_region_tap  |
    l3_op_to_non_en_region_tap  |
    scp_op_to_non_en_region_tap |
    scp_set_out_of_range_tap;

  assign pipe_info_tap.pipe_dec_err_type      =
    l3_shire_dec_err_tap         ? PipeErrL3ShireDec    :
    scp_shire_dec_err_tap        ? PipeErrScpShireDec   :
    scp_region_dec_err_tap       ? PipeErrScpRegionDec  :
    l2_op_to_non_en_region_tap   ? PipeErrL2OpNonEn     :
    l3_op_to_non_en_region_tap   ? PipeErrL3OpNonEn     :
    scp_op_to_non_en_region_tap  ? PipeErrScpOpNonEn    :
    scp_set_out_of_range_tap     ? PipeErrScpSetRange   :
                                   ErrNone;

  assign pipe_info_tap.pipe_scp_read           = pipe_scp_read_tap;
  assign pipe_info_tap.pipe_bubble             = pipe_bubble_tap;

  // following are only pipelined from _tc to _dc
  assign pipe_info_tap.idx_write               = '0;
  assign pipe_info_tap.tag_hit                 = '0;
  assign pipe_info_tap.tag_ram_hit_way         = '0;
  assign pipe_info_tap.tag_sbe                 = '0;
  assign pipe_info_tap.victim                  = '0;
  assign pipe_info_tap.data_rd_req             = '0;
  assign pipe_info_tap.data_wr_req             = '0;
  assign pipe_info_tap.data_wr_zero            = '0;
  assign pipe_info_tap.data_wr_qwen            = '0;
  assign pipe_info_tap.dataq_rd_req_en         = '0;
  assign pipe_info_tap.dataq_rd_req_partial    = '0;
  assign pipe_info_tap.dataq_sel_qwen          = '0;
  assign pipe_info_tap.merge_wr_data_in_rd_rsp = '0;
  assign pipe_info_tap.merge_rd_data_in_wr_req = '0;
  assign pipe_info_tap.data_rsp_req            = '0;
  assign pipe_info_tap.data_rsp_zero           = '0;
  assign pipe_info_tap.data_rsp_qwen           = '0;

  // ════════════════════════════════════════════════════════
  // Block-level clock gaters
  // ════════════════════════════════════════════════════════

  logic clk_rbuf;
  wire  clk_rbuf_en = !rst_n || esr_ctl_i.esr_sc_l2_rbuf_enable || esr_ctl_i.esr_sc_scp_rbuf_enable;
  prim_clk_gate u_clk_gate_rbuf (
    .clk_i  (clk_i),
    .en_i   (clk_rbuf_en),
    .dft_i  (dft_i),
    .clk_o  (clk_rbuf)
  );

  // ════════════════════════════════════════════════════════
  // _ta through _dc (sub_bank logic)
  // ════════════════════════════════════════════════════════

  // Internal outputs from sub_bank
  logic                        pipe_cbuf_invalidate_req_valid;
  logic                        pipe_cbuf_req_valid;
  cbuf_req_t                   pipe_cbuf_req_info;
  logic                        pipe_rbuf_invalidate_req_valid;
  logic                        pipe_rbuf_install_req_valid;
  rbuf_install_req_t           pipe_rbuf_install_req_info;
  logic                        pipe_rbuf_evict_req_valid;
  rbuf_evict_req_t             pipe_rbuf_evict_req_info;
  logic                        pipe_rbuf_data_req_valid;
  rbuf_data_req_t              pipe_rbuf_data_req_info;
  sbe_dbe_esr_status_t         pipe_sbe_dbe_esr_status;

  // pipe_mem req interface
  logic [SubBanks-1:0]         pipe_tag_state_ram_req_rd_en;
  logic [SubBanks-1:0]         pipe_tag_state_ram_req_wr_en;
  tag_state_ram_req_t          pipe_tag_state_ram_req_info;
  logic [SubBanks-1:0]         pipe_tag_ram_req_rd_en;
  logic [SubBanks-1:0]         pipe_tag_ram_req_wr_en;
  tag_ram_req_t                pipe_tag_ram_req_info;
  logic [SubBanks-1:0]         pipe_data_ram_req_rd_en;
  logic [SubBanks-1:0]         pipe_data_ram_req_wr_en;
  logic [SubBanks-1:0]         pipe_data_ram_req_amo_wr_en;
  data_ram_req_t               pipe_data_ram_req_info;

  // pipe_mem rsp interface
  logic                        pipe_tag_state_ram_rsp_valid;
  tag_state_ram_rsp_t          pipe_tag_state_ram_rsp_info;
  logic                        pipe_tag_ram_rsp_valid;
  tag_ram_rsp_t                pipe_tag_ram_rsp_info;
  logic                        pipe_data_ram_rsp_valid;
  data_ram_rsp_t               pipe_data_ram_rsp_info;

  // idx cop sm
  cbuf_state_t                 pipe_cbuf_state_info;
  idx_cop_sm_dbg_write_t       esr_dbg_wr_info;
  idx_cop_sm_dbg_read_t        dbg_rd_info;
  idx_cop_sm_esr_status_t      pipe_idx_cop_sm_esr_status;

  // assign pipe_esr_status outputs
  assign pipe_esr_status_o.pipe_idx_cop_sm_esr_status = pipe_idx_cop_sm_esr_status;
  assign pipe_esr_status_o.pipe_sbe_dbe_esr_status    = pipe_sbe_dbe_esr_status;

  // performance monitor
  perfmon_rsp_info_t           perfmon_subbank_rsp_info;
  perfmon_rsp_info_t           perfmon_cbuf_rsp_info;
  perfmon_rsp_info_t           perfmon_rbuf_rsp_info;

  assign perfmon_rsp_info_o =
    perfmon_subbank_rsp_info |
    perfmon_cbuf_rsp_info    |
    perfmon_rbuf_rsp_info;

  // pipe_squash
  logic pipe_squash_tap;
  assign pipe_squash_o = pipe_squash_tap;

  // ════════════════════════════════════════════════��═══════
  // Sub-bank control instantiation
  // ════════════════════════════════════════════════════════

  shirecache_pipe_sub_bank u_sub_bank (
    .clk_i                                    (clk_i),
    .rst_ni                                   (rst_n),

    .pipe_valid_tap_i                         (pipe_valid_tap),
    .pipe_info_tap_i                          (pipe_info_tap),
    .pipe_req_info_tap_i                      (pipe_req_info_tap),
    .pipe_squash_tap_o                        (pipe_squash_tap),

    .pipe_tag_rsp_valid_o                     (pipe_tag_rsp_valid_o),
    .pipe_tag_rsp_info_o                      (pipe_tag_rsp_info_o),

    .pipe_data_rsp_pre_valid_o                (pipe_data_rsp_pre_valid_o),
    .pipe_data_rsp_pre_info_o                 (pipe_data_rsp_pre_info_o),

    .pipe_data_rsp_valid_o                    (pipe_data_rsp_valid_o),
    .pipe_data_rsp_info_o                     (pipe_data_rsp_info_o),

    .pipe_dataq_rd_req_valid_o                (pipe_dataq_rd_req_valid_o),
    .pipe_dataq_rd_req_info_o                 (pipe_dataq_rd_req_info_o),
    .pipe_dataq_rd_rsp_info_i                 (pipe_dataq_rd_rsp_info_i),

    .pipe_cbuf_invalidate_req_valid_o         (pipe_cbuf_invalidate_req_valid),
    .pipe_cbuf_req_valid_o                    (pipe_cbuf_req_valid),
    .pipe_cbuf_req_info_o                     (pipe_cbuf_req_info),

    .pipe_rbuf_invalidate_req_valid_o         (pipe_rbuf_invalidate_req_valid),
    .pipe_rbuf_install_req_valid_o            (pipe_rbuf_install_req_valid),
    .pipe_rbuf_install_req_info_o             (pipe_rbuf_install_req_info),
    .pipe_rbuf_evict_req_valid_o              (pipe_rbuf_evict_req_valid),
    .pipe_rbuf_evict_req_info_o               (pipe_rbuf_evict_req_info),
    .pipe_rbuf_data_req_valid_o               (pipe_rbuf_data_req_valid),
    .pipe_rbuf_data_req_info_o                (pipe_rbuf_data_req_info),

    .pipe_tag_state_ram_req_rd_en_o           (pipe_tag_state_ram_req_rd_en),
    .pipe_tag_state_ram_req_wr_en_o           (pipe_tag_state_ram_req_wr_en),
    .pipe_tag_state_ram_req_info_o            (pipe_tag_state_ram_req_info),
    .pipe_tag_ram_req_rd_en_o                 (pipe_tag_ram_req_rd_en),
    .pipe_tag_ram_req_wr_en_o                 (pipe_tag_ram_req_wr_en),
    .pipe_tag_ram_req_info_o                  (pipe_tag_ram_req_info),
    .pipe_data_ram_req_rd_en_o                (pipe_data_ram_req_rd_en),
    .pipe_data_ram_req_wr_en_o                (pipe_data_ram_req_wr_en),
    .pipe_data_ram_req_amo_wr_en_o            (pipe_data_ram_req_amo_wr_en),
    .pipe_data_ram_req_info_o                 (pipe_data_ram_req_info),

    .pipe_tag_state_ram_rsp_valid_i           (pipe_tag_state_ram_rsp_valid),
    .pipe_tag_state_ram_rsp_info_i            (pipe_tag_state_ram_rsp_info),
    .pipe_tag_ram_rsp_valid_i                 (pipe_tag_ram_rsp_valid),
    .pipe_tag_ram_rsp_info_i                  (pipe_tag_ram_rsp_info),
    .pipe_data_ram_rsp_valid_i                (pipe_data_ram_rsp_valid),
    .pipe_data_ram_rsp_info_i                 (pipe_data_ram_rsp_info),

    .esr_dbg_wr_info_i                        (esr_dbg_wr_info),
    .dbg_rd_info_o                            (dbg_rd_info),

    .pipe_bist_req_info_i                     (pipe_bist_req_info_i),
    .pipe_bist_rsp_info_o                     (pipe_bist_rsp_info_o),

    .perfmon_req_info_i                       (perfmon_req_info_i),
    .perfmon_rsp_info_o                       (perfmon_subbank_rsp_info),

    .esr_ctl_i                                (esr_ctl_i),
    .l3_swizzle_ctl_i                         (l3_swizzle_ctl),
    .esr_sc_l2_tag_mask_i                     (esr_sc_l2_tag_mask),
    .esr_sc_l3_tag_mask_i                     (esr_sc_l3_tag_mask),
    .esr_sc_ram_delay_i                       (esr_sc_ram_delay),
    .esr_sc_l2_rbuf_enable_i                  (esr_sc_l2_rbuf_enable),
    .esr_sc_scp_rbuf_enable_i                 (esr_sc_scp_rbuf_enable),
    .esr_sc_zero_state_enable_i               (esr_sc_zero_state_enable),
    .esr_sc_all_shire_aliasing_i              (esr_sc_all_shire_aliasing),
    .esr_sc_two_shire_aliasing_use_shire_lsb_i(esr_sc_two_shire_aliasing_use_shire_lsb),
    .pipe_sbe_dbe_esr_status_o                (pipe_sbe_dbe_esr_status),
    .pipe_sbe_dbe_overflow_o                  (pipe_sbe_dbe_overflow_o),
    .my_bank_id_i                             (my_bank_id_i),
    .my_l3_virtual_shire_id_i                 (my_l3_virtual_shire_id_i)
  );

  // ════════════════════════════════════════════════════════
  // Coalesce buffer (cbuf)
  // ════════════════════════════════════════════════════════

  shirecache_pipe_cbuf u_cbuf (
    .clk_i                      (clk_i),
    .rst_ni                     (rst_n),

    .invalidate_req_valid_i     (pipe_cbuf_invalidate_req_valid),
    .req_valid_i                (pipe_cbuf_req_valid),
    .req_info_i                 (pipe_cbuf_req_info),
    .rsp_valid_o                (pipe_cbuf_rsp_valid_o),
    .rsp_info_o                 (pipe_cbuf_rsp_info_o),
    .state_o                    (pipe_cbuf_state_info),
    .perfmon_req_info_i         (perfmon_req_info_i),
    .perfmon_rsp_info_o         (perfmon_cbuf_rsp_info),
    .esr_cbuf_entry_enable_i    (esr_sc_cbuf_entry_enable),
    .my_bank_id_i               (my_bank_id_i)
  );

  // ════════════════════════════════════════════════════════
  // Read buffer (rbuf)
  // ════════════════════════════════════════════════════════

  shirecache_pipe_rbuf u_rbuf (
    .clk_i                      (clk_rbuf),  // block-level gated clock
    .rst_ni                     (rst_n),

    .invalidate_req_valid_i     (pipe_rbuf_invalidate_req_valid),
    .install_req_valid_i        (pipe_rbuf_install_req_valid),
    .install_req_info_i         (pipe_rbuf_install_req_info),
    .evict_req_valid_i          (pipe_rbuf_evict_req_valid),
    .evict_req_info_i           (pipe_rbuf_evict_req_info),
    .data_req_valid_i           (pipe_rbuf_data_req_valid),
    .data_req_info_i            (pipe_rbuf_data_req_info),
    .clear_req_info_i           (pipe_rbuf_clear_req_info_i),
    .rd_req_ready_o             (pipe_rbuf_rd_req_ready_o),
    .rd_req_valid_i             (pipe_rbuf_rd_req_valid_i),
    .rd_req_info_i              (pipe_rbuf_rd_req_info_i),
    .rd_rsp_ready_i             (pipe_rbuf_rd_rsp_ready_i),
    .rd_rsp_valid_o             (pipe_rbuf_rd_rsp_valid_o),
    .rd_rsp_info_o              (pipe_rbuf_rd_rsp_info_o),
    .state_o                    (pipe_rbuf_state_o),
    .perfmon_req_info_i         (perfmon_req_info_i),
    .perfmon_rsp_info_o         (perfmon_rbuf_rsp_info),
    .esr_ram_delay_i            (esr_sc_ram_delay),
    .esr_l2_rbuf_enable_i       (esr_sc_l2_rbuf_enable),
    .esr_scp_rbuf_enable_i      (esr_sc_scp_rbuf_enable),
    .my_bank_id_i               (my_bank_id_i)
  );

  // ════════════════════════════════════════════════════════
  // Memory — tag_state, tag, and data RAMs (per sub-bank)
  // ════════════════════════════════════════════════════════
  // Replaces shire_cache_pipe_mem wrapper: instantiate SubBanks copies
  // of shirecache_pipe_sub_bank_mem and OR the responses.

  logic                    [SubBanks-1:0] sb_ts_rsp_valid;
  logic [SubBanks-1:0][DvTransIdSize-1:0] sb_ts_rsp_trans_id;
  logic [SubBanks-1:0][ReqqIdSize-1:0]    sb_ts_rsp_reqq_id;
  logic [SubBanks-1:0][TagStateRamDataSize-1:0] sb_ts_rsp_data;

  logic                    [SubBanks-1:0] sb_t_rsp_valid;
  logic [SubBanks-1:0][DvTransIdSize-1:0] sb_t_rsp_trans_id;
  logic [SubBanks-1:0][ReqqIdSize-1:0]    sb_t_rsp_reqq_id;
  logic [SubBanks-1:0][TagRamDataSize-1:0] sb_t_rsp_data;

  logic                    [SubBanks-1:0] sb_d_rsp_valid;
  logic [SubBanks-1:0][DvTransIdSize-1:0] sb_d_rsp_trans_id;
  logic [SubBanks-1:0][ReqqIdSize-1:0]    sb_d_rsp_reqq_id;
  logic [SubBanks-1:0][DataRamDataSize-1:0] sb_d_rsp_data;

  // Build the ram_cfg for sub_bank_mem: deep_sleep/shut_down come from ESR
  ram_cfg_t mem_ram_cfg;
  always_comb begin
    mem_ram_cfg             = ram_cfg_i;
    mem_ram_cfg.deep_sleep  = esr_sc_ram_deep_sleep;
    mem_ram_cfg.shut_down   = esr_sc_ram_shut_down;
  end

  for (genvar sb = 0; sb < SubBanks; sb++) begin : gen_sb_mem
    // Decompose struct fields to per-signal interface.
    // Tag state RAM: dual-port, rd/wr addresses separate.
    wire ts_rd_en = pipe_tag_state_ram_req_rd_en[sb];
    wire ts_wr_en = pipe_tag_state_ram_req_wr_en[sb];

    // Tag RAM: single-port (rd/wr address muxed inside sub_bank_mem).
    wire t_rd_en = pipe_tag_ram_req_rd_en[sb];
    wire t_wr_en = pipe_tag_ram_req_wr_en[sb];

    // Data RAM
    wire d_rd_en     = pipe_data_ram_req_rd_en[sb];
    wire d_wr_en     = pipe_data_ram_req_wr_en[sb];
    wire d_amo_wr_en = pipe_data_ram_req_amo_wr_en[sb];

    // Mux data RAM signals for amo_wr vs wr vs rd (same as original sub_bank_mem)
    wire [DvTransIdSize-1:0]  d_trans_id = d_amo_wr_en ? pipe_data_ram_req_info.amo_wr_trans_id :
                                           d_wr_en     ? pipe_data_ram_req_info.wr_trans_id     :
                                                          pipe_data_ram_req_info.rd_trans_id;
    wire [ReqqIdSize-1:0]     d_reqq_id  = d_amo_wr_en ? pipe_data_ram_req_info.amo_wr_reqq_id :
                                           d_wr_en     ? pipe_data_ram_req_info.wr_reqq_id     :
                                                          pipe_data_ram_req_info.rd_reqq_id;
    wire [DataRamAddrSize-1:0] d_addr    = d_amo_wr_en ? pipe_data_ram_req_info.amo_wr_addr :
                                           d_wr_en     ? pipe_data_ram_req_info.wr_addr     :
                                                          pipe_data_ram_req_info.rd_addr;
    wire [DataRamDataSize-1:0] d_wr_data = d_amo_wr_en ? pipe_data_ram_req_info.amo_wr_data :
                                                          pipe_data_ram_req_info.wr_data;
    wire [LineQwSize-1:0]      d_wr_qwen = d_amo_wr_en ? pipe_data_ram_req_info.amo_wr_qwen :
                                                          pipe_data_ram_req_info.wr_qwen;
    wire any_d_wr_en = d_amo_wr_en || d_wr_en;

    // Tag state RAM: select trans_id/reqq_id based on wr priority
    wire [DvTransIdSize-1:0] ts_trans_id = ts_wr_en ? pipe_tag_state_ram_req_info.wr_trans_id :
                                                       pipe_tag_state_ram_req_info.rd_trans_id;
    wire [ReqqIdSize-1:0]    ts_reqq_id  = ts_wr_en ? pipe_tag_state_ram_req_info.wr_reqq_id :
                                                       pipe_tag_state_ram_req_info.rd_reqq_id;

    // Tag RAM: select trans_id/reqq_id and address based on wr priority
    wire [DvTransIdSize-1:0] t_trans_id = t_wr_en ? pipe_tag_ram_req_info.wr_trans_id :
                                                     pipe_tag_ram_req_info.rd_trans_id;
    wire [ReqqIdSize-1:0]    t_reqq_id  = t_wr_en ? pipe_tag_ram_req_info.wr_reqq_id :
                                                     pipe_tag_ram_req_info.rd_reqq_id;
    wire [$clog2(SETS_PER_SUB_BANK)-1:0] t_addr = t_wr_en ? pipe_tag_ram_req_info.wr_addr[$clog2(SETS_PER_SUB_BANK)-1:0] :
                                                              pipe_tag_ram_req_info.rd_addr[$clog2(SETS_PER_SUB_BANK)-1:0];

    shirecache_pipe_sub_bank_mem #(
      .SetsPerSubBank   (SETS_PER_SUB_BANK),
      .TagStateDualPort (1'b1)
    ) u_sb_mem (
      .clk_i              (clk_i),
      .rst_ni             (rst_n),

      .esr_ram_delay_i    (esr_sc_ram_delay),

      // Tag state RAM
      .ts_rd_en_i         (ts_rd_en),
      .ts_wr_en_i         (ts_wr_en),
      .ts_rd_addr_i       (pipe_tag_state_ram_req_info.rd_addr[$clog2(SETS_PER_SUB_BANK)-1:0]),
      .ts_wr_addr_i       (pipe_tag_state_ram_req_info.wr_addr[$clog2(SETS_PER_SUB_BANK)-1:0]),
      .ts_wr_data_i       (pipe_tag_state_ram_req_info.wr_data),
      .ts_rd_trans_id_i   (ts_trans_id),
      .ts_rd_reqq_id_i    (ts_reqq_id),
      .ts_rsp_valid_o     (sb_ts_rsp_valid[sb]),
      .ts_rsp_trans_id_o  (sb_ts_rsp_trans_id[sb]),
      .ts_rsp_reqq_id_o   (sb_ts_rsp_reqq_id[sb]),
      .ts_rsp_data_o       (sb_ts_rsp_data[sb]),

      // Tag RAM
      .t_rd_en_i          (t_rd_en),
      .t_wr_en_i          (t_wr_en),
      .t_addr_i           (t_addr),
      .t_wr_data_i        (pipe_tag_ram_req_info.wr_data),
      .t_rd_trans_id_i    (t_trans_id),
      .t_rd_reqq_id_i     (t_reqq_id),
      .t_rsp_valid_o      (sb_t_rsp_valid[sb]),
      .t_rsp_trans_id_o   (sb_t_rsp_trans_id[sb]),
      .t_rsp_reqq_id_o    (sb_t_rsp_reqq_id[sb]),
      .t_rsp_data_o       (sb_t_rsp_data[sb]),

      // Data RAM
      .d_rd_en_i          (d_rd_en),
      .d_wr_en_i          (any_d_wr_en),
      .d_addr_i           (d_addr),
      .d_wr_data_i        (d_wr_data),
      .d_wr_qwen_i        (d_wr_qwen),
      .d_rd_trans_id_i    (d_trans_id),
      .d_rd_reqq_id_i     (d_reqq_id),
      .d_rsp_valid_o      (sb_d_rsp_valid[sb]),
      .d_rsp_trans_id_o   (sb_d_rsp_trans_id[sb]),
      .d_rsp_reqq_id_o    (sb_d_rsp_reqq_id[sb]),
      .d_rsp_data_o       (sb_d_rsp_data[sb]),

      // Technology hooks
      .dft_sram_clk_i     (dft_sram_clk_i),
      .ram_cfg_i           (mem_ram_cfg),
      .dft_i               (dft_i)
    );
  end : gen_sb_mem

  // OR sub-bank mem responses (only one valid per clock across sub-banks).
  // Mask data with valid to allow safe OR reduction (inactive sub-banks
  // contribute zeros, matching original shire_cache_pipe_sub_bank_mem behavior).
  always_comb begin
    pipe_tag_state_ram_rsp_valid     = '0;
    pipe_tag_state_ram_rsp_info      = '0;
    pipe_tag_ram_rsp_valid           = '0;
    pipe_tag_ram_rsp_info            = '0;
    pipe_data_ram_rsp_valid          = '0;
    pipe_data_ram_rsp_info           = '0;
    for (int unsigned ii = 0; ii < SubBanks; ii++) begin
      pipe_tag_state_ram_rsp_valid           |= sb_ts_rsp_valid[ii];
      pipe_tag_state_ram_rsp_info.trans_id   |= {DvTransIdSize{sb_ts_rsp_valid[ii]}}       & sb_ts_rsp_trans_id[ii];
      pipe_tag_state_ram_rsp_info.reqq_id    |= {ReqqIdSize{sb_ts_rsp_valid[ii]}}          & sb_ts_rsp_reqq_id[ii];
      pipe_tag_state_ram_rsp_info.rd_data    |= {TagStateRamDataSize{sb_ts_rsp_valid[ii]}} & sb_ts_rsp_data[ii];

      pipe_tag_ram_rsp_valid                 |= sb_t_rsp_valid[ii];
      pipe_tag_ram_rsp_info.trans_id         |= {DvTransIdSize{sb_t_rsp_valid[ii]}}        & sb_t_rsp_trans_id[ii];
      pipe_tag_ram_rsp_info.reqq_id          |= {ReqqIdSize{sb_t_rsp_valid[ii]}}           & sb_t_rsp_reqq_id[ii];
      pipe_tag_ram_rsp_info.rd_data          |= {TagRamDataSize{sb_t_rsp_valid[ii]}}       & sb_t_rsp_data[ii];

      pipe_data_ram_rsp_valid                |= sb_d_rsp_valid[ii];
      pipe_data_ram_rsp_info.trans_id        |= {DvTransIdSize{sb_d_rsp_valid[ii]}}        & sb_d_rsp_trans_id[ii];
      pipe_data_ram_rsp_info.reqq_id         |= {ReqqIdSize{sb_d_rsp_valid[ii]}}           & sb_d_rsp_reqq_id[ii];
      pipe_data_ram_rsp_info.rd_data         |= {DataRamDataSize{sb_d_rsp_valid[ii]}}      & sb_d_rsp_data[ii];
    end
  end

  // ════════════════════════════════════════════════════════
  // Index cache-op state machine
  // ════════════════════════════════════════════════════════

  shirecache_pipe_idx_cop_sm u_idx_cop_sm (
    .clk_i                         (clk_i),
    .rst_ni                        (rst_n),
    .rst_c_ni                      (rst_c_n),

    .dft_i                         (dft_i),

    .mbist_on_i                    (mbist_on_i),

    .pipe_idx_cop_aborted_o        (pipe_idx_cop_aborted_o),
    .pipe_idx_cop_l2_busy_o        (pipe_idx_cop_l2_busy_o),
    .pipe_idx_cop_l3_busy_o        (pipe_idx_cop_l3_busy_o),
    .pipe_idx_cop_req_ready_i      (pipe_idx_cop_req_ready_i),
    .pipe_idx_cop_req_valid_o      (pipe_idx_cop_req_valid_o),
    .pipe_idx_cop_req_info_o       (pipe_idx_cop_req_info_o),
    .pipe_idx_cop_is_active_o      (pipe_idx_cop_is_active_o),

    .pipe_cbuf_state_info_i        (pipe_cbuf_state_info),
    .esr_dbg_wr_info_o             (esr_dbg_wr_info),
    .dbg_rd_info_i                 (dbg_rd_info),

    .esr_ctl_i                     (esr_ctl_i),
    .pipe_idx_cop_sm_esr_status_o  (pipe_idx_cop_sm_esr_status),
    .my_bank_id_i                  (my_bank_id_i),
    .my_scp_virtual_shire_id_i     (my_scp_virtual_shire_id_i)
  );

  // ════════════════════════════════════════════════════════
  // Assertions
  // ════════════════════════════════════════════════════════

`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    if (rst_n && pipe_start_i) begin
      if (sub_bank_id_rqa !== sub_bank_id_check_rqa)
        $error("ERROR_PIPE_sub_bank_check: sub_bank_id mismatch");
    end
    if (rst_n && !$onehot0(sb_ts_rsp_valid))
      $error("ERROR_PIPE_multi_tag_state_rsp_valid");
    if (rst_n && !$onehot0(sb_t_rsp_valid))
      $error("ERROR_PIPE_multi_tag_rsp_valid");
    if (rst_n && !$onehot0(sb_d_rsp_valid))
      $error("ERROR_PIPE_multi_data_rsp_valid");
  end
`else
  // SVA
  // Sub-bank ID check
  // synthesis translate_off
  assert property (@(posedge clk_i) disable iff (!rst_n)
    pipe_start_i |-> (sub_bank_id_rqa === sub_bank_id_check_rqa)
  ) else $error("ERROR_PIPE_sub_bank_check: sub_bank_id mismatch");

  // Only one sub-bank responds per clock
  assert property (@(posedge clk_i) disable iff (!rst_n)
    $onehot0(sb_ts_rsp_valid)
  ) else $error("ERROR_PIPE_multi_tag_state_rsp_valid");

  assert property (@(posedge clk_i) disable iff (!rst_n)
    $onehot0(sb_t_rsp_valid)
  ) else $error("ERROR_PIPE_multi_tag_rsp_valid");

  assert property (@(posedge clk_i) disable iff (!rst_n)
    $onehot0(sb_d_rsp_valid)
  ) else $error("ERROR_PIPE_multi_data_rsp_valid");

  // Region top no-wrap checks
  assert property (@(posedge clk_i) disable iff (!rst_n)
    (esr_sc_l2_region_enabled && pipe_valid_tap) |-> !l2_region_top_full[SetIdSize]
  ) else $error("ERROR_PIPE_l2_region_top_full_no_wrap_check");

  assert property (@(posedge clk_i) disable iff (!rst_n)
    (esr_sc_l3_region_enabled && pipe_valid_tap) |-> !l3_region_top_full[SetIdSize]
  ) else $error("ERROR_PIPE_l3_region_top_full_no_wrap_check");

  assert property (@(posedge clk_i) disable iff (!rst_n)
    (esr_sc_scp_region_enabled && pipe_valid_tap) |-> !scp_region_top_full[SetIdSize]
  ) else $error("ERROR_PIPE_scp_region_top_full_no_wrap_check");

  // Bank address check
  assert property (@(posedge clk_i) disable iff (!rst_n)
    pipe_valid_tap |-> (
      (l2_operation_tap  && (pipe_req_info_tap.address[BankLsb +: BankIdSize] == my_bank_id_i)) ||
      (l3_operation_tap  && (l3_bank_id_tap == my_bank_id_i)) ||
      (scp_operation_tap && (pipe_req_info_tap.address[BankLsb +: BankIdSize] == my_bank_id_i)) ||
      (idx_operation_tap && (pipe_req_info_tap.address[BankLsb +: BankIdSize] == my_bank_id_i))
    )
  ) else $error("ERROR_PIPE_SUB_PIPE_addr_bank_check_tap");

  // RAM delay legal range
  assert property (@(posedge clk_i) disable iff (!rst_n)
    pipe_start_i |-> (esr_sc_ram_delay inside {[2:4]})
  ) else $error("ERROR_PIPE_legal_esr_sc_ram_delay_check");
  // synthesis translate_on
`endif

  // ════════════════════════════════════════════════════════
  // Lint
  // ════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{
    1'b0,
    l3_bank_id_tap,               // only used for assertion
    l2_region_top_full[SetIdSize],
    l3_region_top_full[SetIdSize],
    scp_region_top_full[SetIdSize],
    sub_bank_id_check_rqa,        // only used for assertion
    esr_ctl_i                     // not all esr values are used in this module
  };

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
