// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache sub-bank data pipeline.
//
// Instantiates sub-bank tag-state, tag-RAM, and data-cache pipelines.
// Performs tag compare, LRU management, ECC encode/decode, data replace
// logic, atomic operations, and coalesce/read-buffer request generation.
//
// Pipeline stages:
//   _tap → _ta → _ta_dN → _te → _tc → _tn → _dap → _da → _da_dN → _de → _dc
//
// Replaces: shire_cache_pipe_sub_bank (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions (_i/_o, _d/_q)
//   - Package types instead of `define macros
//   - prim_ecc_enc/prim_ecc_dec instead of ecc_gen/ecc_corr
//   - shirecache_pipe_sbe_insert with InsertEnable=0
//   - Dual-port tag-state RAM path only (SC_TAG_STATE_RAM_SINGLE_PORT not defined)
//   - Explicit always_ff/always_comb instead of RST_EN_FF/EN_FF macros

module shirecache_pipe_sub_bank
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,

  // Request interface from reqq
  input  logic                             pipe_valid_tap_i,
  input  pipe_info_t                       pipe_info_tap_i,
  input  reqq_pipe_req_t                   pipe_req_info_tap_i,
  output logic                             pipe_squash_tap_o,

  // Response interfaces to reqq
  output logic                             pipe_tag_rsp_valid_o,
  output tag_rsp_t                         pipe_tag_rsp_info_o,

  output logic                             pipe_data_rsp_pre_valid_o,
  output data_rsp_pre_t                    pipe_data_rsp_pre_info_o,

  output logic                             pipe_data_rsp_valid_o,
  output data_rsp_t                        pipe_data_rsp_info_o,

  // Data interface to dataq
  output logic                             pipe_dataq_rd_req_valid_o,
  output dataq_rd_req_t                    pipe_dataq_rd_req_info_o,
  input  dataq_rd_rsp_t                    pipe_dataq_rd_rsp_info_i,

  // Request interface to cbuf (coalesce buffer)
  output logic                             pipe_cbuf_invalidate_req_valid_o,
  output logic                             pipe_cbuf_req_valid_o,
  output cbuf_req_t                        pipe_cbuf_req_info_o,

  // Request interfaces to rbuf (read buffer)
  output logic                             pipe_rbuf_invalidate_req_valid_o,
  output logic                             pipe_rbuf_install_req_valid_o,
  output rbuf_install_req_t                pipe_rbuf_install_req_info_o,
  output logic                             pipe_rbuf_evict_req_valid_o,
  output rbuf_evict_req_t                  pipe_rbuf_evict_req_info_o,
  output logic                             pipe_rbuf_data_req_valid_o,
  output rbuf_data_req_t                   pipe_rbuf_data_req_info_o,

  // pipe_mem req interface
  output logic [SubBanks-1:0]              pipe_tag_state_ram_req_rd_en_o,
  output logic [SubBanks-1:0]              pipe_tag_state_ram_req_wr_en_o,
  output tag_state_ram_req_t               pipe_tag_state_ram_req_info_o,
  output logic [SubBanks-1:0]              pipe_tag_ram_req_rd_en_o,
  output logic [SubBanks-1:0]              pipe_tag_ram_req_wr_en_o,
  output tag_ram_req_t                     pipe_tag_ram_req_info_o,
  output logic [SubBanks-1:0]              pipe_data_ram_req_rd_en_o,
  output logic [SubBanks-1:0]              pipe_data_ram_req_wr_en_o,
  output logic [SubBanks-1:0]              pipe_data_ram_req_amo_wr_en_o,
  output data_ram_req_t                    pipe_data_ram_req_info_o,

  // pipe_mem rsp interface
  input  logic                             pipe_tag_state_ram_rsp_valid_i,
  input  tag_state_ram_rsp_t               pipe_tag_state_ram_rsp_info_i,
  input  logic                             pipe_tag_ram_rsp_valid_i,
  input  tag_ram_rsp_t                     pipe_tag_ram_rsp_info_i,
  input  logic                             pipe_data_ram_rsp_valid_i,
  input  data_ram_rsp_t                    pipe_data_ram_rsp_info_i,

  // Dbg_Write interface
  input  idx_cop_sm_dbg_write_t            esr_dbg_wr_info_i,

  // Dbg_Read interface
  output idx_cop_sm_dbg_read_t             dbg_rd_info_o,

  // BIST
  input  bist_req_t                        pipe_bist_req_info_i,
  output bist_rsp_t                        pipe_bist_rsp_info_o,

  // Performance monitor for resource and event
  input  perfmon_req_info_t                perfmon_req_info_i,
  output perfmon_rsp_info_t                perfmon_rsp_info_o,

  // ESRs and constants
  input  bank_esr_ctl_t                    esr_ctl_i,
  input  l3_swizzle_ctl_t                  l3_swizzle_ctl_i,
  input  logic [SetIdSize-1:0]             esr_sc_l2_tag_mask_i,
  input  logic [SetIdSize-1:0]             esr_sc_l3_tag_mask_i,
  input  logic [RamDelaySize-1:0]          esr_sc_ram_delay_i,
  input  logic                             esr_sc_l2_rbuf_enable_i,
  input  logic                             esr_sc_scp_rbuf_enable_i,
  input  logic                             esr_sc_zero_state_enable_i,
  input  logic                             esr_sc_all_shire_aliasing_i,
  input  logic                             esr_sc_two_shire_aliasing_use_shire_lsb_i,
  output sbe_dbe_esr_status_t              pipe_sbe_dbe_esr_status_o,
  output sbe_dbe_overflow_t                pipe_sbe_dbe_overflow_o,
  input  logic [BankIdSize-1:0]            my_bank_id_i,
  input  logic [L3ShireIdSize-1:0]         my_l3_virtual_shire_id_i
);

  // Line-offset bits of address unused (cache operates on full lines);
  // shire_id shift widens implicitly; struct fields partially used.
  /* verilator lint_off UNUSEDSIGNAL */  // address line-offset bits and partial struct fields intentionally unused
  /* verilator lint_off WIDTHEXPAND */  // intentional zero-extend when shifting shire_id into wider address field
  // pipe_info_tc is a packed struct where independent fields (tag_hit, tag_sbe,
  // victim) are assigned from separate combinational paths. Verilator treats the
  // whole struct as one signal and reports a false circular dependency. The
  // original shire_cache_pipe_sub_bank has the identical pattern (lines 1705-1729).
  /* verilator lint_off UNOPTFLAT */

  // ════════════════════════════════════════════════════════
  // Local constants
  // ════════════════════════════════════════════════════════
  localparam int unsigned QWEN_SIZE = LineQwSize;
  localparam int unsigned PerfmonEvTag = 2;

  // ════════════════════════════════════════════════════════
  // Helper function
  // ════════════════════════════════════════════════════════
  function automatic logic [PaSize-1:0] get_line_addr(
    input logic [PaSize-1:0] int_address
  );
    return {int_address[(PaSize-1):LineOffsetIdSize], {LineOffsetIdSize{1'b0}}};
  endfunction

  // ════════════════════════════════════════════════════════
  // _tap (inputs to this module are at _tap)
  // ════════════════════════════════════════════════════════
  //   - flop signals driving tag ram to save power and help timing closure

  // assigned below
  logic                              tag_ram_wr_en_tn;
  wire  [SubBanks-1:0]              tag_ram_wr_sub_bank_tn;
  wire  [DvTransIdSize-1:0]         tag_ram_wr_trans_id_tn;
  wire  [ReqqIdSize-1:0]            tag_ram_wr_reqq_id_tn;
  logic [TagRamAddrSize-1:0]        tag_ram_wr_addr_tn;
  logic [TagRamDataSize-1:0]        tag_ram_wr_data_tn;

  // NOTE: read tag_state and tag ram on first pass (_tap stage) of all operations except scratchpad, All_Inv, Idx_Write operations
  wire scp_operation_tap       = (pipe_req_info_tap_i.address_space == AddressScp);
  wire all_inv_operation_tap   = (pipe_req_info_tap_i.opcode == IdxAllInv);
  wire dbg_wr_operation_tap    = (pipe_req_info_tap_i.opcode == IdxWrite);
  wire squash_tag_state_and_tag_rd_tap = scp_operation_tap || all_inv_operation_tap || dbg_wr_operation_tap || pipe_info_tap_i.pipe_dec_err;

  // tag ram muxing
  wire                             tag_ram_rd_en_tap       = pipe_valid_tap_i && !squash_tag_state_and_tag_rd_tap;
  wire [SubBanks-1:0]             tag_ram_rd_sub_bank_tap = pipe_info_tap_i.pipe_sub_bank;
  wire [DvTransIdSize-1:0]        tag_ram_rd_trans_id_tap = pipe_req_info_tap_i.trans_id;
  wire [ReqqIdSize-1:0]           tag_ram_rd_reqq_id_tap  = pipe_req_info_tap_i.reqq_id;
  wire [TagRamAddrSize-1:0]       tag_ram_rd_addr_tap     = pipe_info_tap_i.pipe_set;
  wire                             tag_ram_wr_en_tap       = tag_ram_wr_en_tn;
  wire [SubBanks-1:0]             tag_ram_wr_sub_bank_tap = tag_ram_wr_sub_bank_tn;
  wire [DvTransIdSize-1:0]        tag_ram_wr_trans_id_tap = tag_ram_wr_trans_id_tn;
  wire [ReqqIdSize-1:0]           tag_ram_wr_reqq_id_tap  = tag_ram_wr_reqq_id_tn;
  wire [TagRamAddrSize-1:0]       tag_ram_wr_addr_tap     = tag_ram_wr_addr_tn;
  wire [TagRamDataSize-1:0]       tag_ram_wr_data_tap     = tag_ram_wr_data_tn;

  // bist muxing (control signals)
  wire                             mbt_rd_en_xx       = pipe_bist_req_info_i.mbt_sel && pipe_bist_req_info_i.mbt_rd_en;
  wire                             mbt_wr_en_xx       = pipe_bist_req_info_i.mbt_sel && pipe_bist_req_info_i.mbt_wr_en;
  wire                             mbt_access_xx      = (mbt_rd_en_xx | mbt_wr_en_xx);
  wire [TagRamAddrSize-1:0]       mbt_addr_xx        = pipe_bist_req_info_i.mbt_addr[0 +: SetIdSize];
  wire [SubBankIdSize-1:0]        mbt_sub_bank_id_xx = pipe_bist_req_info_i.mbt_addr[SetIdSize +: SubBankIdSize];
  wire [SubBanks-1:0]             mbt_sub_bank_xx    = (SubBanks'(1'b1) << mbt_sub_bank_id_xx);
  wire [TagRamDataSize-1:0]       mbt_data_xx        = pipe_bist_req_info_i.mbt_wdata;

  // assign to tag ram_req ports
  assign pipe_tag_ram_req_rd_en_o            = mbt_access_xx ? {SubBanks{mbt_rd_en_xx}} & mbt_sub_bank_xx : {SubBanks{tag_ram_rd_en_tap}} & tag_ram_rd_sub_bank_tap;
  assign pipe_tag_ram_req_info_o.rd_trans_id = mbt_access_xx ? '0                                         : tag_ram_rd_trans_id_tap;
  assign pipe_tag_ram_req_info_o.rd_reqq_id  = mbt_access_xx ? '0                                         : tag_ram_rd_reqq_id_tap;
  assign pipe_tag_ram_req_info_o.rd_addr     = mbt_access_xx ? mbt_addr_xx                                : tag_ram_rd_addr_tap;
  assign pipe_tag_ram_req_wr_en_o            = mbt_access_xx ? {SubBanks{mbt_wr_en_xx}} & mbt_sub_bank_xx : {SubBanks{tag_ram_wr_en_tap}} & tag_ram_wr_sub_bank_tap;
  assign pipe_tag_ram_req_info_o.wr_trans_id = mbt_access_xx ? '0                                         : tag_ram_wr_trans_id_tap;
  assign pipe_tag_ram_req_info_o.wr_reqq_id  = mbt_access_xx ? '0                                         : tag_ram_wr_reqq_id_tap;
  assign pipe_tag_ram_req_info_o.wr_addr     = mbt_access_xx ? mbt_addr_xx                                : tag_ram_wr_addr_tap;
  assign pipe_tag_ram_req_info_o.wr_data     = mbt_access_xx ? mbt_data_xx                                : tag_ram_wr_data_tap;

  // ════════════════════════════════════════════════════════
  // _ta (tag ram access) — resides in shire_cache_mem
  // ════════════════════════════════════════════════════════
  logic                             pipe_valid_ta_q;
  pipe_info_t                       pipe_info_ta_q;
  reqq_pipe_req_t                   pipe_req_info_ta_q;

  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) pipe_valid_ta_q <= '0;
    else if (pipe_valid_ta_q || pipe_valid_tap_i) pipe_valid_ta_q <= pipe_valid_tap_i && !pipe_squash_tap_o;

  always_ff @(posedge clk_i) if (pipe_valid_tap_i) pipe_info_ta_q <= pipe_info_tap_i;
  always_ff @(posedge clk_i) if (pipe_valid_tap_i) pipe_req_info_ta_q <= pipe_req_info_tap_i;


  localparam int unsigned RAM_DLY = 4; // max number of ram cycles of delay

  wire pipe_valid_ta_d0 = pipe_valid_ta_q;
  logic [RAM_DLY-1:0] vec_pipe_valid_ta_dN_q;
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) vec_pipe_valid_ta_dN_q <= '0;
    else if (|{vec_pipe_valid_ta_dN_q, pipe_valid_ta_d0}) vec_pipe_valid_ta_dN_q <= {vec_pipe_valid_ta_dN_q[0 +: RAM_DLY-1], pipe_valid_ta_d0};

  wire pipe_valid_ta_d1 = vec_pipe_valid_ta_dN_q[0];
  wire pipe_valid_ta_d2 = vec_pipe_valid_ta_dN_q[1];
  wire pipe_valid_ta_d3 = vec_pipe_valid_ta_dN_q[2];
  wire pipe_valid_ta_d4 = vec_pipe_valid_ta_dN_q[3];

  wire esr_sc_ram_delay_2 = (esr_sc_ram_delay_i == RamDelaySize'(2));
  wire esr_sc_ram_delay_3 = (esr_sc_ram_delay_i == RamDelaySize'(3));
  wire esr_sc_ram_delay_4 = (esr_sc_ram_delay_i == RamDelaySize'(4));

  // capture read data after ram_delay cycles
  wire pipe_valid_ta_dN =
    (esr_sc_ram_delay_2 && pipe_valid_ta_d2) ||
    (esr_sc_ram_delay_3 && pipe_valid_ta_d3) ||
    (esr_sc_ram_delay_4 && pipe_valid_ta_d4);


  // ════════════════════════════════════════════════════════
  // Tag state RAM — dual-port path
  // ════════════════════════════════════════════════════════
  // tag state ram write signals assigned below
  logic                                   tag_state_ram_wr_en_tn;
  logic [SubBanks-1:0]                   tag_state_ram_wr_sub_bank_tn;
  logic [DvTransIdSize-1:0]              tag_state_ram_wr_trans_id_tn;
  logic [ReqqIdSize-1:0]                 tag_state_ram_wr_reqq_id_tn;
  logic [TagStateRamAddrSize-1:0]        tag_state_ram_wr_addr_tn;
  logic [TagStateRamDataSize-1:0]        tag_state_ram_wr_data_tn;

  // dual-port: read uses _tap signals
  wire                                    tag_state_ram_rd_en_xx_m1       = pipe_valid_tap_i && !squash_tag_state_and_tag_rd_tap;
  wire [SubBanks-1:0]                    tag_state_ram_rd_sub_bank_xx_m1 = pipe_info_tap_i.pipe_sub_bank;
  wire [DvTransIdSize-1:0]               tag_state_ram_rd_trans_id_xx_m1 = pipe_req_info_tap_i.trans_id;
  wire [ReqqIdSize-1:0]                  tag_state_ram_rd_reqq_id_xx_m1  = pipe_req_info_tap_i.reqq_id;
  wire [TagStateRamAddrSize-1:0]         tag_state_ram_rd_addr_xx_m1     = pipe_info_tap_i.pipe_set;

  wire                                    tag_state_ram_wr_en_xx_m1       = tag_state_ram_wr_en_tn;
  wire [SubBanks-1:0]                    tag_state_ram_wr_sub_bank_xx_m1 = tag_state_ram_wr_sub_bank_tn;
  wire [DvTransIdSize-1:0]               tag_state_ram_wr_trans_id_xx_m1 = tag_state_ram_wr_trans_id_tn;
  wire [ReqqIdSize-1:0]                  tag_state_ram_wr_reqq_id_xx_m1  = tag_state_ram_wr_reqq_id_tn;
  wire [TagStateRamAddrSize-1:0]         tag_state_ram_wr_addr_xx_m1     = tag_state_ram_wr_addr_tn;
  wire [TagStateRamDataSize-1:0]         tag_state_ram_wr_data_xx_m1     = tag_state_ram_wr_data_tn;

  // bist muxing (control signals)
  wire                                   mbs_rd_en_xx       = pipe_bist_req_info_i.mbs_sel && pipe_bist_req_info_i.mbs_rd_en;
  wire                                   mbs_wr_en_xx       = pipe_bist_req_info_i.mbs_sel && pipe_bist_req_info_i.mbs_wr_en;
  wire                                   mbs_access_xx      = (mbs_rd_en_xx | mbs_wr_en_xx);
  wire [TagStateRamAddrSize-1:0]         mbs_addr_xx        = pipe_bist_req_info_i.mbs_addr[0 +: SetIdSize];
  wire [SubBankIdSize-1:0]               mbs_sub_bank_id_xx = pipe_bist_req_info_i.mbs_addr[SetIdSize +: SubBankIdSize];
  wire [SubBanks-1:0]                   mbs_sub_bank_xx    = (SubBanks'(1'b1) << mbs_sub_bank_id_xx);
  wire [TagStateRamDataSize-1:0]         mbs_data_xx        = pipe_bist_req_info_i.mbs_wdata;

  // assign to tag_state ram_req ports
  assign pipe_tag_state_ram_req_rd_en_o            = mbs_access_xx ? {SubBanks{mbs_rd_en_xx}} & mbs_sub_bank_xx : {SubBanks{tag_state_ram_rd_en_xx_m1}} & tag_state_ram_rd_sub_bank_xx_m1;
  assign pipe_tag_state_ram_req_info_o.rd_trans_id = mbs_access_xx ? '0                                         : tag_state_ram_rd_trans_id_xx_m1;
  assign pipe_tag_state_ram_req_info_o.rd_reqq_id  = mbs_access_xx ? '0                                         : tag_state_ram_rd_reqq_id_xx_m1;
  assign pipe_tag_state_ram_req_info_o.rd_addr     = mbs_access_xx ? mbs_addr_xx                                : tag_state_ram_rd_addr_xx_m1;
  assign pipe_tag_state_ram_req_wr_en_o            = mbs_access_xx ? {SubBanks{mbs_wr_en_xx}} & mbs_sub_bank_xx : {SubBanks{tag_state_ram_wr_en_xx_m1}} & tag_state_ram_wr_sub_bank_xx_m1;
  assign pipe_tag_state_ram_req_info_o.wr_trans_id = mbs_access_xx ? '0                                         : tag_state_ram_wr_trans_id_xx_m1;
  assign pipe_tag_state_ram_req_info_o.wr_reqq_id  = mbs_access_xx ? '0                                         : tag_state_ram_wr_reqq_id_xx_m1;
  assign pipe_tag_state_ram_req_info_o.wr_addr     = mbs_access_xx ? mbs_addr_xx                                : tag_state_ram_wr_addr_xx_m1;
  assign pipe_tag_state_ram_req_info_o.wr_data     = mbs_access_xx ? mbs_data_xx                                : tag_state_ram_wr_data_xx_m1;


  // ════════════════════════════════════════════════════════
  // Pipeline control signals from _ta to _ta_dN
  // ════════════════════════════════════════════════════════
  pipe_info_t                    pipe_info_ta_d0;
  reqq_pipe_req_t                pipe_req_info_ta_d0;
  assign                         pipe_info_ta_d0     = pipe_info_ta_q;
  assign                         pipe_req_info_ta_d0 = pipe_req_info_ta_q;

  // _ta_d1
  pipe_info_t                    pipe_info_ta_d1_q;
  reqq_pipe_req_t                pipe_req_info_ta_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d0) pipe_info_ta_d1_q     <= pipe_info_ta_d0;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d0) pipe_req_info_ta_d1_q <= pipe_req_info_ta_d0;

  // _ta_d2
  pipe_info_t                    pipe_info_ta_d2_q;
  reqq_pipe_req_t                pipe_req_info_ta_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d1) pipe_info_ta_d2_q     <= pipe_info_ta_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d1) pipe_req_info_ta_d2_q <= pipe_req_info_ta_d1_q;

  // _ta_d3
  pipe_info_t                    pipe_info_ta_d3_q;
  reqq_pipe_req_t                pipe_req_info_ta_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d2) pipe_info_ta_d3_q     <= pipe_info_ta_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d2) pipe_req_info_ta_d3_q <= pipe_req_info_ta_d2_q;

  // _ta_d4
  pipe_info_t                    pipe_info_ta_d4_q;
  reqq_pipe_req_t                pipe_req_info_ta_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d3) pipe_info_ta_d4_q     <= pipe_info_ta_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_ta_d3) pipe_req_info_ta_d4_q <= pipe_req_info_ta_d3_q;


  // assign tag read data signals at _ta_dN
  pipe_info_t                    pipe_info_ta_dN;
  assign pipe_info_ta_dN =
    ({$bits(pipe_info_t){esr_sc_ram_delay_2}} & pipe_info_ta_d2_q) |
    ({$bits(pipe_info_t){esr_sc_ram_delay_3}} & pipe_info_ta_d3_q) |
    ({$bits(pipe_info_t){esr_sc_ram_delay_4}} & pipe_info_ta_d4_q);

  reqq_pipe_req_t                pipe_req_info_ta_dN;
  assign pipe_req_info_ta_dN =
    ({$bits(reqq_pipe_req_t){esr_sc_ram_delay_2}} & pipe_req_info_ta_d2_q) |
    ({$bits(reqq_pipe_req_t){esr_sc_ram_delay_3}} & pipe_req_info_ta_d3_q) |
    ({$bits(reqq_pipe_req_t){esr_sc_ram_delay_4}} & pipe_req_info_ta_d4_q);


  // ════════════════════════════════════════════════════════
  // _te (tag ram ecc stage)
  // ════════════════════════════════════════════════════════
  logic                                   pipe_valid_te_q;
  pipe_info_t                             pipe_info_te_q;
  reqq_pipe_req_t                         pipe_req_info_te_q;
  logic                                   tag_state_ram_rd_data_valid_te_q;
  logic [TagStateRamDataSize-1:0]         tag_state_ram_rd_data_te_q;
  logic                                   tag_ram_rd_data_valid_te_q;
  logic [TagRamDataSize-1:0]              tag_ram_rd_data_te_q;

  wire                                    tag_state_ram_rd_data_valid_ta_dN = pipe_tag_state_ram_rsp_valid_i;
  wire [TagStateRamDataSize-1:0]          tag_state_ram_rd_data_ta_dN       = pipe_tag_state_ram_rsp_info_i.rd_data;
  wire                                    tag_ram_rd_data_valid_ta_dN       = pipe_tag_ram_rsp_valid_i;
  wire [TagRamDataSize-1:0]               tag_ram_rd_data_ta_dN             = pipe_tag_ram_rsp_info_i.rd_data;

  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) pipe_valid_te_q <= '0;
    else if (pipe_valid_te_q || pipe_valid_ta_dN) pipe_valid_te_q <= pipe_valid_ta_dN;
  always_ff @(posedge clk_i) if (pipe_valid_ta_dN)                   pipe_info_te_q     <= pipe_info_ta_dN;
  always_ff @(posedge clk_i) if (pipe_valid_ta_dN)                   pipe_req_info_te_q <= pipe_req_info_ta_dN;

  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) tag_state_ram_rd_data_valid_te_q <= '0;
    else if (tag_state_ram_rd_data_valid_te_q || tag_state_ram_rd_data_valid_ta_dN) tag_state_ram_rd_data_valid_te_q <= tag_state_ram_rd_data_valid_ta_dN;
  always_ff @(posedge clk_i) if (tag_state_ram_rd_data_valid_ta_dN) tag_state_ram_rd_data_te_q <= tag_state_ram_rd_data_ta_dN;
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) tag_ram_rd_data_valid_te_q <= '0;
    else if (tag_ram_rd_data_valid_te_q || tag_ram_rd_data_valid_ta_dN) tag_ram_rd_data_valid_te_q <= tag_ram_rd_data_valid_ta_dN;
  always_ff @(posedge clk_i) if (tag_ram_rd_data_valid_ta_dN) tag_ram_rd_data_te_q <= tag_ram_rd_data_ta_dN;

  // SC_IdxRead muxing
  wire idx_read_valid_te = pipe_valid_te_q && (pipe_req_info_te_q.opcode == IdxRead);
  assign dbg_rd_info_o.tag_state_and_ecc_valid = idx_read_valid_te && esr_dbg_wr_info_i.tag_state_en;
  assign dbg_rd_info_o.tag_state_and_ecc       = tag_state_ram_rd_data_te_q;
  assign dbg_rd_info_o.tag_state_space         = pipe_info_te_q.pipe_idx_address_space;
  assign dbg_rd_info_o.tags_and_ecc_valid      = idx_read_valid_te && esr_dbg_wr_info_i.tags_en;
  assign dbg_rd_info_o.tags_and_ecc            = tag_ram_rd_data_te_q;

  // bist muxing - read rsp muxing
  assign pipe_bist_rsp_info_o.mbs_rdata = tag_state_ram_rd_data_te_q;
  assign pipe_bist_rsp_info_o.mbt_rdata = tag_ram_rd_data_te_q;

  // ecc correction — tag state
  logic [TagStateDataSize-1:0] int_tag_state_ram_rd_data_corr_te;
  logic                        tag_state_ram_sbe_te;
  logic                        tag_state_ram_dbe_te;
  prim_ecc_dec #(
    .DataWidth(TagStateDataSize)
  ) tag_state_ram_ecc_corr (
    .data_and_ecc_i(tag_state_ram_rd_data_te_q),
    .data_o        (int_tag_state_ram_rd_data_corr_te),
    .sbe_o         (tag_state_ram_sbe_te),
    .dbe_o         (tag_state_ram_dbe_te)
  );

  // ecc correction — tags (per way)
  logic [TagsDataSize-1:0]     int_tag_ram_rd_data_corr_te;
  logic [Ways-1:0]             tag_ram_sbe_te;
  logic [Ways-1:0]             tag_ram_dbe_te;
  prim_ecc_dec #(
    .DataWidth(TagDataSize)
  ) tag_ram_ecc_corr [Ways-1:0] (
    .data_and_ecc_i(tag_ram_rd_data_te_q),
    .data_o        (int_tag_ram_rd_data_corr_te),
    .sbe_o         (tag_ram_sbe_te),
    .dbe_o         (tag_ram_dbe_te)
  );

  wire [TagStateDataSize-1:0] tag_state_replace_te;
  wire                        tag_replace_te;
  wire [TagsDataSize-1:0]     tags_replace_te;

  wire [TagStateDataSize-1:0] tag_state_ram_rd_data_corr_te = tag_replace_te ? tag_state_replace_te : int_tag_state_ram_rd_data_corr_te;
  wire [TagsDataSize-1:0]     tag_ram_rd_data_corr_te       = tag_replace_te ? tags_replace_te      : int_tag_ram_rd_data_corr_te;


  // ════════════════════════════════════════════════════════
  // _tc (tag compare stage)
  // ════════════════════════════════════════════════════════
  logic                                      pipe_valid_tc_q;
  pipe_info_t                                int_pipe_info_tc_q;
  pipe_info_t                                pipe_info_tc;
  reqq_pipe_req_t                            pipe_req_info_tc_q;
  logic                                      tag_replace_tc_q;
  logic [TagStateDataSize-1:0]               tag_state_ram_rd_data_corr_tc_q;
  logic [TagsDataSize-1:0]                   tag_ram_rd_data_corr_tc_q;
  logic                                      tag_state_ram_sbe_tc_q;
  logic                                      tag_state_ram_dbe_tc_q;
  logic [Ways-1:0]                           tag_ram_sbe_tc_q;
  logic [Ways-1:0]                           tag_ram_dbe_tc_q;

  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) pipe_valid_tc_q <= '0;
    else if (pipe_valid_tc_q || pipe_valid_te_q) pipe_valid_tc_q <= pipe_valid_te_q;
  always_ff @(posedge clk_i) if (pipe_valid_te_q)                   int_pipe_info_tc_q <= pipe_info_te_q;
  always_ff @(posedge clk_i) if (pipe_valid_te_q)                   pipe_req_info_tc_q <= pipe_req_info_te_q;
  always_ff @(posedge clk_i) if (pipe_valid_te_q)                   tag_replace_tc_q   <= tag_replace_te;
  always_ff @(posedge clk_i) if (tag_state_ram_rd_data_valid_te_q)  tag_state_ram_rd_data_corr_tc_q <= tag_state_ram_rd_data_corr_te;
  always_ff @(posedge clk_i) if (tag_ram_rd_data_valid_te_q)        tag_ram_rd_data_corr_tc_q      <= tag_ram_rd_data_corr_te;
  always_ff @(posedge clk_i) if (pipe_valid_te_q)                   tag_state_ram_sbe_tc_q <= tag_state_ram_sbe_te && !idx_read_valid_te && tag_state_ram_rd_data_valid_te_q;
  always_ff @(posedge clk_i) if (pipe_valid_te_q)                   tag_state_ram_dbe_tc_q <= tag_state_ram_dbe_te && !idx_read_valid_te && tag_state_ram_rd_data_valid_te_q;
  always_ff @(posedge clk_i) if (pipe_valid_te_q)                   tag_ram_sbe_tc_q <= tag_ram_sbe_te & {Ways{!idx_read_valid_te && tag_ram_rd_data_valid_te_q}};
  always_ff @(posedge clk_i) if (pipe_valid_te_q)                   tag_ram_dbe_tc_q <= tag_ram_dbe_te & {Ways{!idx_read_valid_te && tag_ram_rd_data_valid_te_q}};


  // ── Tag equations ──
  tag_state_t tag_state_tc;

  wire l2_operation_tc        = pipe_req_info_tc_q.address_space == AddressL2;
  wire l3_operation_tc        = pipe_req_info_tc_q.address_space == AddressL3;
  wire scp_operation_tc       = pipe_req_info_tc_q.address_space == AddressScp;
  wire idx_operation_tc       = pipe_req_info_tc_q.address_space == AddressIdx;
  wire idx_all_inv_tc         = pipe_req_info_tc_q.opcode == IdxAllInv;
  wire idx_read_tc            = pipe_req_info_tc_q.opcode == IdxRead;
  wire idx_write_tc           = pipe_req_info_tc_q.opcode == IdxWrite;
  wire idx_write_tag_state_tc = idx_write_tc && esr_dbg_wr_info_i.tag_state_en;
  wire idx_write_tags_tc      = idx_write_tc && esr_dbg_wr_info_i.tags_en;
  wire idx_write_data_tc      = idx_write_tc && esr_dbg_wr_info_i.data_en;

  assign tag_state_tc = tag_state_t'(tag_state_ram_rd_data_corr_tc_q);
  wire [Ways-1:0][TagMaxSize-1:0] tags_tc = tag_ram_rd_data_corr_tc_q;

  // l2_addr_tag_set_masked from incoming addr
  wire [TagSetSelAddrSize-1:0] esr_l2_tag_set_mask_tc = ~{{(TagSetSelAddrSize - SetIdSize){1'b0}}, esr_sc_l2_tag_mask_i};
  wire [TagSetSelAddrSize-1:0] l2_addr_tag_set_tc = pipe_req_info_tc_q.address[SetLsb +: TagSetSelAddrSize];
  wire [TagSetSelAddrSize-1:0] l2_addr_tag_set_masked_tc = l2_addr_tag_set_tc & esr_l2_tag_set_mask_tc;
  wire [TagMaxSize-1:0]        l2_addr_tag_masked_tc     = l2_addr_tag_set_masked_tc[TagOfTagSetLsb +: TagIdSizeL2];


  // l3_addr_tag_set_and_shire_masked_tc from incoming addr
  logic [SubBankIdSize-1:0] l3_sub_bank_id_tc;
  logic [BankIdSize-1:0]    l3_bank_id_tc;
  logic [L3ShireIdSize-1:0] l3_shire_id_tc;
  shirecache_pipe_l3_swizzle_get l3_swizzle_get_tc (
    .address_i     (pipe_req_info_tc_q.address),
    .swizzle_ctl_i (l3_swizzle_ctl_i),
    .sub_bank_id_o (l3_sub_bank_id_tc),
    .bank_id_o     (l3_bank_id_tc),
    .shire_id_o    (l3_shire_id_tc)
  );

  wire [L3ShireIdSize-1:0] l3_shire_id_tag_bits_tc =
    esr_sc_all_shire_aliasing_i                ?  l3_shire_id_tc :
    esr_sc_two_shire_aliasing_use_shire_lsb_i  ?  L3ShireIdSize'(l3_shire_id_tc[0]) :
                                                   L3ShireIdSize'(l3_shire_id_tc[L3ShireIdSize-1]);

  localparam int unsigned L3_ADDR_SHIRE_ID_SHIFTED_W = L3ShireIdSize + (PaSize - TagMaxSize);
  wire [L3_ADDR_SHIRE_ID_SHIFTED_W-1:0] l3_shire_id_tag_bits_shifted_tc = l3_shire_id_tag_bits_tc << (PaSize - TagMaxSize);
  wire [PaSize-1:0]                     l3_addr_shire_tc                 = PaSize'(l3_shire_id_tag_bits_shifted_tc);

  wire [PaSize-1:0]                     esr_l3_tag_set_mask_full_addr_tc = ~(PaSize'({esr_sc_l3_tag_mask_i, {L3SetLsb{1'b0}}}));
  wire [PaSize-1:0]                     l3_addr_masked_tc                = pipe_req_info_tc_q.address & esr_l3_tag_set_mask_full_addr_tc;
  wire [PaSize-1:0]                     l3_addr_and_shire_masked_tc      = l3_addr_masked_tc | l3_addr_shire_tc;

  wire [L3TagSetSelAddrSize-1:0]        l3_addr_tag_set_and_shire_masked_tc = l3_addr_and_shire_masked_tc[L3SetLsb +: L3TagSetSelAddrSize];

  // NOTE: TagMaxSize can be larger than the l3_tag_set combined field
  logic [TagMaxSize-1:0] l3_addr_tag_masked_tc;
  generate
    if (TagMaxSize > L3TagSetSelAddrSize) begin : g_tag_max_gt_tag_set
      assign l3_addr_tag_masked_tc = {{(TagMaxSize - L3TagSetSelAddrSize){1'b0}}, l3_addr_tag_set_and_shire_masked_tc[L3TagOfTagSetLsb +: L3TagIdSize]};
    end else begin : g_tag_max_lteq_tag_set
      assign l3_addr_tag_masked_tc = l3_addr_tag_set_and_shire_masked_tc[L3TagOfTagSetLsb +: L3TagIdSize];
    end
  endgenerate

  // LRU and idx tag read
  wire [TagStateLruSize-1:0] lru_state_tc       = tag_state_tc.lru_state;
  wire [WayIdSize-1:0]       lru_way_tc         = lru_state_tc[WayIdSize-1:0];
  wire                       lru_way_valid_tc   = tag_state_tc.way_state[lru_way_tc].valid;
  wire                       lru_way_dirty_tc   = |tag_state_tc.way_state[lru_way_tc].qwen;
  wire                       lru_way_partial_tc = lru_way_dirty_tc && !(&tag_state_tc.way_state[lru_way_tc].qwen);
  wire                       lru_way_locked_tc  = tag_state_tc.way_state[lru_way_tc].locked;
  wire                       lru_way_zero_tc    = tag_state_tc.way_state[lru_way_tc].zero;
  wire [QWEN_SIZE-1:0]       lru_way_qwen_tc    = tag_state_tc.way_state[lru_way_tc].qwen;
  wire [TagMaxSize-1:0]      lru_way_tag_tc     = tags_tc[lru_way_tc];

  wire [WayIdSize-1:0]       idx_way_tc         = pipe_req_info_tc_q.address[ScpWayLsb +: WayIdSize];
  wire [TagMaxSize-1:0]      idx_tag_tc         = tags_tc[idx_way_tc];


  // create l2/l3 fields from tag read
  wire [TagMaxSize-1:0] l2_tag_tc = idx_operation_tc ? idx_tag_tc : lru_way_tag_tc;

  logic [TagMaxSize-1:0] l3_tag_and_shire_id_tc;
  generate
    if (TagMaxSize > L3TagSetSelAddrSize) begin : g_l3_tag_max_gt
      assign l3_tag_and_shire_id_tc = l2_tag_tc[0 +: L3TagSetSelAddrSize];
    end else begin : g_l3_tag_max_lteq
      assign l3_tag_and_shire_id_tc = l2_tag_tc;
    end
  endgenerate


  // l3_tag_tc/l3_shire_tc from l3_tag_and_shire_id_tc
  wire                        one_const = 1'b1;
  wire [L3ShireIdSize-1:0]   l3_shire_id_mask_const = esr_sc_all_shire_aliasing_i ? '1 : L3ShireIdSize'(one_const);
  wire [TagMaxSize-1:0]      l3_tag_tc = l3_tag_and_shire_id_tc & ~(TagMaxSize'(l3_shire_id_mask_const));
  wire [SubBankIdSize-1:0]   mux_l3_sub_bank_id_tc = idx_operation_tc ? pipe_req_info_tc_q.address[SubBankLsb +: SubBankIdSize] : l3_sub_bank_id_tc;

  logic [L3ShireIdSize-1:0] mux_l3_shire_id_tc;
  generate
    if (L3ShireMsb > L3ShireLsb) begin : g_shire_gt_2
      assign mux_l3_shire_id_tc =
        esr_sc_all_shire_aliasing_i               ?  l3_tag_and_shire_id_tc[L3ShireIdSize-1:0] :
        esr_sc_two_shire_aliasing_use_shire_lsb_i ? {my_l3_virtual_shire_id_i[L3ShireIdSize-1:1], l3_tag_and_shire_id_tc[0]} :
                                                     {l3_tag_and_shire_id_tc[0], my_l3_virtual_shire_id_i[L3ShireIdSize-2:0]};
    end else begin : g_shire2
      assign mux_l3_shire_id_tc = l3_tag_and_shire_id_tc[0];
    end
  endgenerate

  // get l3_swizzle_addr from (mux_l3_sub_bank_id_tc, my_bank_id, mux_l3_shire_id_tc)
  logic [PaSize-1:0] l3_swizzle_address_put_tc;
  shirecache_pipe_l3_swizzle_put l3_swizzle_put_tc (
    .sub_bank_id_i (mux_l3_sub_bank_id_tc),
    .bank_id_i     (my_bank_id_i),
    .shire_id_i    (mux_l3_shire_id_tc),
    .swizzle_ctl_i (l3_swizzle_ctl_i),
    .address_put_o (l3_swizzle_address_put_tc)
  );

  // lru_way_l2_address_tc
  wire [PaSize-1:0] l2_tag_address_tc =
    {                                                          l2_tag_tc,                                             {TagLsb{1'b0}}} |
    {{PaSize-SetIdSize-SetLsb{1'b0}},                         pipe_info_tc.pipe_set & esr_sc_l2_tag_mask_i,          {SetLsb{1'b0}}} |
    {{PaSize-SubBankIdSize-SubBankLsb{1'b0}},                 pipe_req_info_tc_q.address[SubBankLsb +: SubBankIdSize],{SubBankLsb{1'b0}}} |
    {{PaSize-BankIdSize-BankLsb{1'b0}},                       my_bank_id_i,                                          {BankLsb{1'b0}}};

  // lru_way_l3_address_tc
  wire [PaSize-1:0] l3_tag_address_tc =
    {                                                          l3_tag_tc,                                             {L3TagLsb{1'b0}}} |
    {{PaSize-SetIdSize-L3SetLsb{1'b0}},                       pipe_info_tc.pipe_set & esr_sc_l3_tag_mask_i,          {L3SetLsb{1'b0}}} |
    l3_swizzle_address_put_tc;


  wire l2_idx_hit_tc = (pipe_req_info_tc_q.opcode == IdxL2Inv) || (pipe_req_info_tc_q.opcode == IdxL2Flush) || (pipe_req_info_tc_q.opcode == IdxL2Evict);

  wire [PaSize-1:0] eviction_address_tc = (l2_operation_tc || l2_idx_hit_tc) ? l2_tag_address_tc : l3_tag_address_tc;

  wire [PaSize-1:0] pipe_line_address_tc = get_line_addr(pipe_req_info_tc_q.address);
  wire              pipe_dec_err_tc      = pipe_info_tc.pipe_dec_err;

  logic                        tag_ram_hit_tc;
  logic                        tag_ram_hit_dirty_tc;
  logic                        tag_ram_hit_partial_tc;
  logic                        tag_ram_hit_locked_tc;
  logic                        tag_ram_hit_zero_tc;
  logic [QWEN_SIZE-1:0]        tag_ram_hit_qwen_tc;
  logic [WayIdSize-1:0]        tag_ram_hit_way_tc;
  logic [Ways-1:0]             tag_ram_valid_bits_tc;
  logic [Ways-1:0]             tag_ram_locked_bits_tc;
  logic [Ways-1:0]             tag_ram_hit_bits_tc;
  always_comb begin
    tag_ram_hit_tc             = '0;
    tag_ram_hit_dirty_tc       = '0;
    tag_ram_hit_partial_tc     = '0;
    tag_ram_hit_locked_tc      = '0;
    tag_ram_hit_zero_tc        = '0;
    tag_ram_hit_qwen_tc        = '0;
    tag_ram_hit_way_tc         = '0;
    tag_ram_valid_bits_tc      = '0;
    tag_ram_locked_bits_tc     = '0;
    tag_ram_hit_bits_tc        = '0;
    if (scp_operation_tc) begin
      tag_ram_hit_tc         = !pipe_dec_err_tc;
      tag_ram_hit_way_tc     = pipe_req_info_tc_q.address[ScpWayLsb +: WayIdSize];
    end else begin
      for (int unsigned ii = 0; ii < Ways; ii++) begin
        tag_ram_valid_bits_tc[ii]  = tag_state_tc.way_state[ii].valid;
        tag_ram_locked_bits_tc[ii] = tag_state_tc.way_state[ii].valid && tag_state_tc.way_state[ii].locked;
        if (
          (tag_state_tc.way_state[ii].valid || idx_read_tc || idx_write_tc) &&
          !pipe_dec_err_tc && (
            (l2_operation_tc && (tags_tc[ii] == l2_addr_tag_masked_tc)) ||
            (l3_operation_tc && (tags_tc[ii] == l3_addr_tag_masked_tc)) ||
            (idx_operation_tc && (idx_way_tc == ii[WayIdSize-1:0]) && !idx_all_inv_tc)
        )) begin
          tag_ram_hit_tc             = 1'b1;
          tag_ram_hit_dirty_tc       = (|tag_state_tc.way_state[ii].qwen);
          tag_ram_hit_partial_tc     = tag_ram_hit_dirty_tc && !(&tag_state_tc.way_state[ii].qwen);
          tag_ram_hit_locked_tc      = tag_state_tc.way_state[ii].locked;
          tag_ram_hit_zero_tc        = tag_state_tc.way_state[ii].zero;
          tag_ram_hit_qwen_tc        = tag_state_tc.way_state[ii].qwen;
          tag_ram_hit_way_tc         = ii[WayIdSize-1:0];
          tag_ram_hit_bits_tc[ii]    = 1'b1;
        end
      end
      if ((tag_ram_hit_tc == 1'b0) && !pipe_dec_err_tc && !idx_all_inv_tc) begin
        tag_ram_hit_way_tc = lru_way_tc;
      end
    end
  end
  wire tag_ram_miss_tc = !tag_ram_hit_tc && !pipe_dec_err_tc;

  logic lru_lock_err_tc;
  wire tag_ram_read_partial_tc        = tag_ram_hit_partial_tc &&
    (pipe_req_info_tc_q.opcode == L2Read     ||
     pipe_req_info_tc_q.opcode == L2Prefetch ||
     pipe_req_info_tc_q.opcode == L2Atomic   ||
     pipe_req_info_tc_q.opcode == L3Read     ||
     pipe_req_info_tc_q.opcode == L3Prefetch ||
     pipe_req_info_tc_q.opcode == L3Atomic);
  wire tag_ram_write_partial_tc                = tag_ram_hit_partial_tc && (pipe_req_info_tc_q.opcode == L2Write);
  wire tag_ram_l2_write_partial_hit_partial_tc = tag_ram_hit_partial_tc && (pipe_req_info_tc_q.opcode == L2WritePartial);
  wire tag_ram_l3_write_partial_hit_partial_tc = tag_ram_hit_partial_tc && (pipe_req_info_tc_q.opcode == L3WritePartial);
  wire tag_ram_lock_partial_tc                 = tag_ram_hit_partial_tc && (pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc;


  logic [QWEN_SIZE-1:0] input_qwen_tc;
  logic [QWEN_SIZE-1:0] merged_qwen_tc;
  wire write_around_hit_tc               = tag_ram_hit_tc  && (pipe_req_info_tc_q.opcode == L2WriteAround);
  wire write_around_miss_tc              = tag_ram_miss_tc && (pipe_req_info_tc_q.opcode == L2WriteAround);
  wire write_around_self_evict_tc        = write_around_hit_tc  && (&merged_qwen_tc);
  wire write_around_self_evict_bypass_tc = write_around_miss_tc && (&input_qwen_tc);
  wire write_around_install_tc           = write_around_miss_tc && !(&input_qwen_tc);


  // tag_ram_victim_tc
  wire tag_ram_victim_tc =
    tag_ram_read_partial_tc ||
    tag_ram_write_partial_tc ||
    tag_ram_l2_write_partial_hit_partial_tc ||
    tag_ram_l3_write_partial_hit_partial_tc ||
    tag_ram_lock_partial_tc ||
    write_around_self_evict_tc ||
    write_around_self_evict_bypass_tc ||
    (tag_ram_miss_tc && lru_way_valid_tc && lru_way_dirty_tc && (
       (pipe_req_info_tc_q.opcode == L2Write) ||
       (write_around_install_tc) ||
       ((pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc) ||
       (pipe_req_info_tc_q.opcode == L2Fill) ||
       (pipe_req_info_tc_q.opcode == L3Write) ||
       (pipe_req_info_tc_q.opcode == L3Fill)
    )) ||
    (tag_ram_hit_dirty_tc && !tag_ram_hit_locked_tc && (
       (pipe_req_info_tc_q.opcode == L2Flush) ||
       (pipe_req_info_tc_q.opcode == L2Evict)
    )) ||
    (tag_ram_hit_tc && (
       (pipe_req_info_tc_q.opcode == L3FlushWData) ||
       (pipe_req_info_tc_q.opcode == L3EvictWData)
    )) ||
    (tag_ram_hit_dirty_tc && (
       (pipe_req_info_tc_q.opcode == L3Flush) ||
       (pipe_req_info_tc_q.opcode == L3Evict) ||
       (pipe_req_info_tc_q.opcode == IdxL2Flush) ||
       (pipe_req_info_tc_q.opcode == IdxL2Evict) ||
       (pipe_req_info_tc_q.opcode == IdxL3Flush) ||
       (pipe_req_info_tc_q.opcode == IdxL3Evict)
    ));

  wire tag_ram_victim_silent_tc =
    (tag_ram_miss_tc && lru_way_valid_tc && !lru_way_dirty_tc && (
       (pipe_req_info_tc_q.opcode == L2Write) ||
       (write_around_install_tc) ||
       ((pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc) ||
       (pipe_req_info_tc_q.opcode == L2Fill) ||
       (pipe_req_info_tc_q.opcode == L3Write) ||
       (pipe_req_info_tc_q.opcode == L3Fill)
    )) ||
    (tag_ram_hit_tc && !tag_ram_hit_dirty_tc && !tag_ram_hit_locked_tc && (
       pipe_req_info_tc_q.opcode == L2Evict
    )) ||
    (tag_ram_hit_tc && !tag_ram_hit_dirty_tc && (
       (pipe_req_info_tc_q.opcode == L3Evict) ||
       (pipe_req_info_tc_q.opcode == IdxL2Evict) ||
       (pipe_req_info_tc_q.opcode == IdxL3Evict)
    ));

  wire tag_ram_rbuf_dirty_victim_tc =
    tag_ram_read_partial_tc ||
    tag_ram_l2_write_partial_hit_partial_tc ||
    write_around_self_evict_tc ||
    (tag_ram_miss_tc && lru_way_valid_tc && lru_way_dirty_tc && (
       (pipe_req_info_tc_q.opcode == L2Write) ||
       (write_around_install_tc) ||
       ((pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc) ||
       (pipe_req_info_tc_q.opcode == L2Fill)
    )) ||
    (tag_ram_hit_dirty_tc && !tag_ram_hit_locked_tc && (
       pipe_req_info_tc_q.opcode == L2Evict
    )) ||
    (tag_ram_hit_dirty_tc && (
       (pipe_req_info_tc_q.opcode == IdxL2Inv) ||
       (pipe_req_info_tc_q.opcode == IdxL2Evict)
    ));

  wire tag_ram_rbuf_victim_silent_tc =
    (tag_ram_miss_tc && lru_way_valid_tc && !lru_way_dirty_tc && (
       (pipe_req_info_tc_q.opcode == L2Write) ||
       (write_around_install_tc) ||
       ((pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc) ||
       (pipe_req_info_tc_q.opcode == L2Fill)
    )) ||
    (tag_ram_hit_tc && !tag_ram_hit_dirty_tc && !tag_ram_hit_locked_tc && (
       pipe_req_info_tc_q.opcode == L2Evict
    )) |
    (tag_ram_hit_tc && !tag_ram_hit_dirty_tc && (
       (pipe_req_info_tc_q.opcode == IdxL2Inv) ||
       (pipe_req_info_tc_q.opcode == IdxL2Evict)
    ));

  wire tag_ram_victim_write_around_tc =
    (tag_ram_hit_tc && (pipe_req_info_tc_q.opcode == IdxL2Inv) && tag_ram_hit_partial_tc) ||
    (tag_ram_victim_tc && (l2_operation_tc || l2_idx_hit_tc) && (
       (tag_ram_hit_partial_tc) ||
       (tag_ram_miss_tc && lru_way_partial_tc))) ||
    write_around_self_evict_tc ||
    write_around_self_evict_bypass_tc;

  wire [PaSize-1:0] victim_address_tc =
    (((tag_ram_victim_tc || tag_ram_victim_silent_tc) && tag_ram_hit_tc && !idx_operation_tc) ||
      write_around_self_evict_bypass_tc)                                                       ? pipe_line_address_tc :
    ((tag_ram_victim_tc || tag_ram_victim_silent_tc) && (tag_ram_miss_tc || idx_operation_tc))  ? eviction_address_tc :
    (tag_ram_hit_tc && (pipe_req_info_tc_q.opcode == IdxL2Inv) && tag_ram_hit_partial_tc)      ? eviction_address_tc :
                                                                                                  '0;

  address_space_e victim_address_space_tc;
  assign victim_address_space_tc =
    idx_operation_tc ? AddressIdx :
    scp_operation_tc ? AddressScp :
    l3_operation_tc  ? AddressL3 :
                       AddressL2;


  // rbuf
  wire rbuf_cache_victim_tc = tag_ram_rbuf_dirty_victim_tc || tag_ram_rbuf_victim_silent_tc;

  wire [PaSize-1:0] rbuf_cache_victim_address_tc =
    (tag_ram_hit_tc && (pipe_req_info_tc_q.opcode == IdxL2Inv)) ? eviction_address_tc : victim_address_tc;


  // cbuf (coalesce buffer) request signals
  wire cbuf_req_valid_tc = pipe_valid_tc_q && !pipe_dec_err_tc && (l2_operation_tc || l2_idx_hit_tc) &&
    (tag_ram_victim_write_around_tc || (pipe_req_info_tc_q.opcode == L2WriteAround));

  wire cbuf_rsp_required_tc = (pipe_req_info_tc_q.opcode == L2WriteAround);

  cbuf_opcode_e cbuf_req_opcode_tc;
  logic [PaSize-1:0] cbuf_req_install_address_tc;
  logic [PaSize-1:0] cbuf_req_clear_address_tc;
  always_comb begin
    cbuf_req_opcode_tc          = cbuf_opcode_e'(3'd5); // Reserved
    cbuf_req_install_address_tc = '0;
    cbuf_req_clear_address_tc   = '0;

    if (write_around_self_evict_bypass_tc) begin
      cbuf_req_opcode_tc = CBufBypass;
    end else if (write_around_install_tc) begin
      if (tag_ram_victim_write_around_tc) begin
        cbuf_req_opcode_tc          = CBufReplace;
        cbuf_req_install_address_tc = pipe_line_address_tc;
        cbuf_req_clear_address_tc   = victim_address_tc;
      end else begin
        cbuf_req_opcode_tc          = CBufInstall;
        cbuf_req_install_address_tc = pipe_line_address_tc;
      end
    end else if (tag_ram_read_partial_tc || tag_ram_write_partial_tc || tag_ram_l2_write_partial_hit_partial_tc || tag_ram_lock_partial_tc) begin
      cbuf_req_opcode_tc        = CBufClear;
      cbuf_req_clear_address_tc = pipe_line_address_tc;
    end else if (write_around_self_evict_tc || tag_ram_victim_write_around_tc) begin
      cbuf_req_opcode_tc = CBufClear;
      if (write_around_self_evict_tc)
        cbuf_req_clear_address_tc = pipe_line_address_tc;
      else
        cbuf_req_clear_address_tc = victim_address_tc;
    end else if (write_around_hit_tc) begin
      if (tag_ram_hit_partial_tc) begin
        cbuf_req_opcode_tc          = CBufMruUpdate;
        cbuf_req_install_address_tc = pipe_line_address_tc;
      end else begin
        cbuf_req_opcode_tc          = CBufInstall;
        cbuf_req_install_address_tc = pipe_line_address_tc;
      end
    end
  end

  // int_data_rsp_req_tc
  logic int_data_rsp_req_tc;
  wire idx_read_data_ram_tc   = (pipe_req_info_tc_q.opcode == IdxRead) && esr_dbg_wr_info_i.data_en;
  wire write_partial_hit_tc   = tag_ram_hit_tc && (pipe_req_info_tc_q.opcode inside {L2WritePartial, L3WritePartial, ScpWritePartial});
  wire scrub_hit_tc           = tag_ram_hit_tc && (pipe_req_info_tc_q.opcode inside {L2Scrub, L3Scrub, ScpScrub});
  wire int_data_ram_rd_en_tc  = (int_data_rsp_req_tc && !tag_ram_hit_zero_tc && !write_around_self_evict_bypass_tc) ||
                                (write_partial_hit_tc && !tag_ram_hit_zero_tc && !tag_ram_hit_partial_tc) ||
                                (scrub_hit_tc && !tag_ram_hit_zero_tc) ||
                                idx_read_data_ram_tc;

  assign int_data_rsp_req_tc =
    !pipe_dec_err_tc && (
      tag_ram_victim_tc ||
      (tag_ram_hit_tc && (
        (pipe_req_info_tc_q.opcode == L2Read) ||
        (pipe_req_info_tc_q.opcode == L2Atomic) ||
        (pipe_req_info_tc_q.opcode == L3Read) ||
        (pipe_req_info_tc_q.opcode == L3Atomic) ||
        (pipe_req_info_tc_q.opcode == ScpRead) ||
        (pipe_req_info_tc_q.opcode == ScpAtomic)
      ))
    );


  // lock/unlock control
  wire locking_way_tc = (pipe_req_info_tc_q.opcode == L2Lock);

  wire unlocking_way_tc =
    (tag_ram_hit_locked_tc && (
      (pipe_req_info_tc_q.opcode == L2WriteAround) ||
      (pipe_req_info_tc_q.opcode == L2Unlock) ||
      (pipe_req_info_tc_q.opcode == L2UnlockInv) ||
      (pipe_req_info_tc_q.opcode == IdxL2Inv) ||
      (pipe_req_info_tc_q.opcode == IdxL2Evict)
    ));


  // MRU/LRU handling
  wire update_mru_tc =
    (tag_ram_hit_tc && (
      (pipe_req_info_tc_q.opcode == L2Read && !tag_ram_hit_partial_tc) ||
      (pipe_req_info_tc_q.opcode == L2WritePartial && !tag_ram_hit_partial_tc) ||
      (pipe_req_info_tc_q.opcode == L2Prefetch && !tag_ram_hit_partial_tc) ||
      (pipe_req_info_tc_q.opcode == L2Atomic && !tag_ram_hit_partial_tc) ||
      (pipe_req_info_tc_q.opcode == L3Read && !tag_ram_hit_partial_tc) ||
      (pipe_req_info_tc_q.opcode == L3WritePartial && !tag_ram_hit_partial_tc) ||
      (pipe_req_info_tc_q.opcode == L3Prefetch && !tag_ram_hit_partial_tc) ||
      (pipe_req_info_tc_q.opcode == L3Atomic && !tag_ram_hit_partial_tc)
    )) ||
    (pipe_req_info_tc_q.opcode == L2Write) ||
    (pipe_req_info_tc_q.opcode == L2WriteAround && !tag_ram_hit_locked_tc && !write_around_self_evict_tc && !write_around_self_evict_bypass_tc) ||
    (pipe_req_info_tc_q.opcode == L2Lock && !lru_lock_err_tc) ||
    (pipe_req_info_tc_q.opcode == L2Fill) ||
    (pipe_req_info_tc_q.opcode == L3Write) ||
    (pipe_req_info_tc_q.opcode == L3Fill);

  wire update_lru_tc =
    tag_ram_read_partial_tc ||
    tag_ram_l2_write_partial_hit_partial_tc ||
    tag_ram_l3_write_partial_hit_partial_tc ||
    (tag_ram_hit_tc & (
      (pipe_req_info_tc_q.opcode == L2WriteAround && write_around_self_evict_tc) ||
      (pipe_req_info_tc_q.opcode == L2WriteAround && tag_ram_hit_locked_tc) ||
      ((pipe_req_info_tc_q.opcode == L2Unlock) && tag_ram_hit_locked_tc) ||
      ((pipe_req_info_tc_q.opcode == L2UnlockInv) && tag_ram_hit_locked_tc) ||
      ((pipe_req_info_tc_q.opcode == L2Flush) && !tag_ram_hit_locked_tc && tag_ram_hit_partial_tc) ||
      ((pipe_req_info_tc_q.opcode == L3Flush) && tag_ram_hit_partial_tc) ||
      ((pipe_req_info_tc_q.opcode == IdxL2Flush) && tag_ram_hit_partial_tc) ||
      ((pipe_req_info_tc_q.opcode == IdxL3Flush) && tag_ram_hit_partial_tc) ||
      ((pipe_req_info_tc_q.opcode == L2Evict) && !tag_ram_hit_locked_tc) ||
      (pipe_req_info_tc_q.opcode == L3Evict) ||
      (pipe_req_info_tc_q.opcode == L3EvictWData) ||
      (pipe_req_info_tc_q.opcode == IdxL2Inv) ||
      (pipe_req_info_tc_q.opcode == IdxL2Evict) ||
      (pipe_req_info_tc_q.opcode == IdxL3Inv) ||
      (pipe_req_info_tc_q.opcode == IdxL3Evict)
    ));


  wire tag_state_ram_wr_en_tc =
    idx_all_inv_tc ||
    idx_write_tag_state_tc ||
    (!pipe_dec_err_tc && (
      (update_mru_tc) ||
      (update_lru_tc) ||
      (tag_ram_hit_tc && (pipe_req_info_tc_q.opcode == L2Scrub)) ||
      (tag_ram_hit_tc && (pipe_req_info_tc_q.opcode == L3Scrub))
    )) ||
    (tag_ram_hit_tc && !tag_ram_hit_locked_tc && (pipe_req_info_tc_q.opcode == L2Flush)) ||
    (tag_ram_hit_tc && (
      (pipe_req_info_tc_q.opcode == L3Flush) ||
      (pipe_req_info_tc_q.opcode == L3FlushWData) ||
      (pipe_req_info_tc_q.opcode == IdxL2Flush) ||
      (pipe_req_info_tc_q.opcode == IdxL3Flush)
    ));

  wire tag_ram_wr_en_tc =
    idx_write_tags_tc ||
    (!pipe_dec_err_tc && !tag_ram_hit_tc && (
      (pipe_req_info_tc_q.opcode == L2Write) ||
      (pipe_req_info_tc_q.opcode == L2Fill) ||
      ((pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc) ||
      ((pipe_req_info_tc_q.opcode == L2WriteAround) && !write_around_self_evict_tc && !write_around_self_evict_bypass_tc) ||
      (pipe_req_info_tc_q.opcode == L3Write) ||
      (pipe_req_info_tc_q.opcode == L3Fill)
    )) ||
    (!pipe_dec_err_tc && tag_ram_hit_tc && (
      (pipe_req_info_tc_q.opcode == L2Scrub) ||
      (pipe_req_info_tc_q.opcode == L3Scrub)
    ));


  // ── LRU calculation ──
  logic [TagStateLruSize-1:0] lru_state_next_tc;
  logic int_lru_lock_err_tc;
  assign lru_lock_err_tc = !pipe_info_tc.pipe_dec_err && int_lru_lock_err_tc;
  shirecache_pipe_lru_4w_calc lru_calc (
    .lru_state_i    (lru_state_tc),
    .hit_way_i      (tag_ram_hit_way_tc),
    .hit_locked_i   (tag_ram_hit_locked_tc),
    .valid_bits_i   (tag_ram_valid_bits_tc),
    .locked_bits_i  (tag_ram_locked_bits_tc),
    .locking_way_i  (locking_way_tc),
    .unlocking_way_i(unlocking_way_tc),
    .update_mru_i   (update_mru_tc),
    .update_lru_i   (update_lru_tc),
    .lru_state_o    (lru_state_next_tc),
    .lru_lock_err_o (int_lru_lock_err_tc)
  );


  // ── err_log ──
  err_rsp_e err_rsp_tc;
  assign err_rsp_tc = pipe_info_tc.pipe_dec_err           ? pipe_info_tc.pipe_dec_err_type :
                      (locking_way_tc && lru_lock_err_tc) ? PipeErrLock :
                                                            ErrNone;

  // ── calculate next flags ──
  logic tag_ram_hit_partial_flush_wdata_still_partial_tc;

  wire any_write_around_self_evict_tc = write_around_self_evict_tc || write_around_self_evict_bypass_tc;

  wire flags_clear =
    tag_ram_read_partial_tc ||
    write_around_self_evict_tc ||
    tag_ram_l2_write_partial_hit_partial_tc ||
    tag_ram_l3_write_partial_hit_partial_tc ||
    (tag_ram_hit_locked_tc && (pipe_req_info_tc_q.opcode == L2UnlockInv)) ||
    (!tag_ram_hit_locked_tc && ((pipe_req_info_tc_q.opcode == L2Flush) || (pipe_req_info_tc_q.opcode == L3Flush) || (pipe_req_info_tc_q.opcode == IdxL2Flush) || (pipe_req_info_tc_q.opcode == IdxL3Flush)) && tag_ram_hit_partial_tc) ||
    ((pipe_req_info_tc_q.opcode == L3FlushWData) && tag_ram_hit_partial_flush_wdata_still_partial_tc) ||
    ((pipe_req_info_tc_q.opcode == L2Evict) && !tag_ram_hit_locked_tc) ||
    (pipe_req_info_tc_q.opcode == L3Evict) ||
    (pipe_req_info_tc_q.opcode == L3EvictWData) ||
    (pipe_req_info_tc_q.opcode == IdxAllInv) ||
    (pipe_req_info_tc_q.opcode == IdxL2Inv) ||
    (pipe_req_info_tc_q.opcode == IdxL2Evict) ||
    (pipe_req_info_tc_q.opcode == IdxL3Inv) ||
    (pipe_req_info_tc_q.opcode == IdxL3Evict);

  wire write_next_tc =
    (pipe_req_info_tc_q.opcode == L2Fill) ||
    (pipe_req_info_tc_q.opcode == L2Write) ||
    ((pipe_req_info_tc_q.opcode == L2WriteAround) && !any_write_around_self_evict_tc) ||
    (pipe_req_info_tc_q.opcode == L3Fill) ||
    (pipe_req_info_tc_q.opcode == L3Write) ||
    ((pipe_req_info_tc_q.opcode == L3FlushWData) && tag_ram_hit_tc && !tag_ram_hit_partial_flush_wdata_still_partial_tc) ||
    (pipe_req_info_tc_q.opcode == ScpFill) ||
    (pipe_req_info_tc_q.opcode == ScpWrite) ||
    (pipe_req_info_tc_q.opcode == ScpZero);

  wire lock_zero_write_next_tc = ((pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc);

  wire valid_next_tc = !flags_clear && (tag_ram_hit_tc || write_next_tc || lock_zero_write_next_tc);

  wire locked_next_tc = !flags_clear && !unlocking_way_tc && (tag_ram_hit_locked_tc || lock_zero_write_next_tc);

  wire all_input_qwens_set_tc = &pipe_req_info_tc_q.qwen;

  wire zero_next_tc = !flags_clear && esr_sc_zero_state_enable_i && (
    (!(write_next_tc || lock_zero_write_next_tc || any_write_around_self_evict_tc) && tag_ram_hit_zero_tc) ||
    (((pipe_req_info_tc_q.opcode == L2Fill) ||
      (pipe_req_info_tc_q.opcode == L2Write) ||
      (pipe_req_info_tc_q.opcode == L3Fill) ||
      (pipe_req_info_tc_q.opcode == ScpFill) ||
      ((pipe_req_info_tc_q.opcode == L3Write) && all_input_qwens_set_tc) ||
      ((pipe_req_info_tc_q.opcode == L3FlushWData) && all_input_qwens_set_tc) ||
      ((pipe_req_info_tc_q.opcode == ScpWrite) && all_input_qwens_set_tc)
     ) && pipe_req_info_tc_q.zero_data) ||
    (pipe_req_info_tc_q.opcode == ScpZero) ||
    (((pipe_req_info_tc_q.opcode == L2WriteAround) ||
      (pipe_req_info_tc_q.opcode == L3Write) ||
      ((pipe_req_info_tc_q.opcode == L3FlushWData) && tag_ram_hit_tc) ||
      (pipe_req_info_tc_q.opcode == ScpWrite)) &&
     pipe_req_info_tc_q.zero_data &&
     (!tag_ram_hit_tc || tag_ram_hit_zero_tc)) ||
    lock_zero_write_next_tc
  ) &&
  !(tag_ram_hit_tc && ((pipe_req_info_tc_q.opcode == L2Atomic) || (pipe_req_info_tc_q.opcode == L3Atomic) || (pipe_req_info_tc_q.opcode == ScpAtomic))) &&
  !(tag_ram_hit_tc && !tag_ram_hit_partial_tc && ((pipe_req_info_tc_q.opcode == L2WritePartial) || (pipe_req_info_tc_q.opcode == L3WritePartial) || (pipe_req_info_tc_q.opcode == ScpWritePartial)));

  wire clear_qwen_tc =
    ((pipe_req_info_tc_q.opcode == L2Flush) && !tag_ram_hit_locked_tc) ||
    (pipe_req_info_tc_q.opcode == L3Flush) ||
    (pipe_req_info_tc_q.opcode == L3FlushWData) ||
    (pipe_req_info_tc_q.opcode == IdxL2Flush) ||
    (pipe_req_info_tc_q.opcode == IdxL3Flush);

  wire full_line_qwens_tc =
    (tag_ram_hit_tc & (pipe_req_info_tc_q.opcode inside {L2Atomic, L3Atomic, ScpAtomic})) ||
    ((pipe_req_info_tc_q.opcode == L2Lock) && !lru_lock_err_tc) ||
    (pipe_req_info_tc_q.opcode == L2Write) ||
    (pipe_req_info_tc_q.opcode == ScpZero);

  assign input_qwen_tc =
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == L2WritePartial)}}  & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == L2WriteAround)}}   & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == L3Write)}}         & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == L3WritePartial)}}  & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == L3FlushWData)}}    & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == L3EvictWData)}}    & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == ScpWrite)}}        & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{(pipe_req_info_tc_q.opcode == ScpWritePartial)}} & pipe_req_info_tc_q.qwen) |
    ({QWEN_SIZE{full_line_qwens_tc}});

  assign merged_qwen_tc =
    ({QWEN_SIZE{tag_ram_hit_tc}} & tag_ram_hit_qwen_tc) |
    ({QWEN_SIZE{tag_ram_hit_tc && (tag_ram_hit_qwen_tc == '0) &&
       (pipe_req_info_tc_q.opcode inside {L2WritePartial, L3Write, L3WritePartial, ScpWritePartial})}}) |
    (input_qwen_tc);

  wire [QWEN_SIZE-1:0] qwen_next_tc =
    ({QWEN_SIZE{!(flags_clear || clear_qwen_tc)}}) & merged_qwen_tc;

  assign tag_ram_hit_partial_flush_wdata_still_partial_tc = tag_ram_hit_partial_tc && !(merged_qwen_tc == '1);


  wire atomic_hit_partial_tc = ((pipe_req_info_tc_q.opcode == L2Atomic) || (pipe_req_info_tc_q.opcode == L3Atomic)) && tag_ram_hit_partial_tc;

  wire data_wr_zero_tc = !pipe_dec_err_tc && (
    ((scrub_hit_tc || write_next_tc) && zero_next_tc) ||
    (pipe_req_info_tc_q.opcode == ScpZero) ||
    lock_zero_write_next_tc
  );

  wire merge_wr_data_in_rd_rsp_tc =
    any_write_around_self_evict_tc ||
    (tag_ram_hit_tc && ((pipe_req_info_tc_q.opcode == L3FlushWData) || (pipe_req_info_tc_q.opcode == L3EvictWData)));

  wire merge_rd_data_in_wr_req_tc = scrub_hit_tc || write_partial_hit_tc;

  wire data_wr_req_tc = !pipe_dec_err_tc && (
    idx_write_data_tc ||
    scrub_hit_tc ||
    (lock_zero_write_next_tc && !esr_sc_zero_state_enable_i) ||
    (pipe_req_info_tc_q.opcode == ScpZero) ||
    (pipe_req_info_tc_q.opcode == ScpFill) ||
    (pipe_req_info_tc_q.opcode == ScpWrite) ||
    ((!data_wr_zero_tc || !esr_sc_zero_state_enable_i) &&
      ((pipe_req_info_tc_q.opcode == L2Fill) ||
       (pipe_req_info_tc_q.opcode == L2Write) ||
       (pipe_req_info_tc_q.opcode == L3Fill) ||
       (pipe_req_info_tc_q.opcode == L3Write) ||
       ((pipe_req_info_tc_q.opcode == L3FlushWData) && tag_ram_hit_tc && !tag_ram_hit_partial_flush_wdata_still_partial_tc) ||
       ((pipe_req_info_tc_q.opcode == L2WriteAround) && !any_write_around_self_evict_tc)))
  );

  wire scrub_hit_partial_tc = scrub_hit_tc && tag_ram_hit_partial_tc;

  wire dataq_rd_req_en_tc = ~pipe_dec_err_tc && (
    (pipe_req_info_tc_q.opcode == L2Fill) ||
    (pipe_req_info_tc_q.opcode == L2Write) ||
    ((pipe_req_info_tc_q.opcode == L2WritePartial) && tag_ram_hit_tc && !tag_ram_hit_partial_tc) ||
    (pipe_req_info_tc_q.opcode == L2WriteAround) ||
    (pipe_req_info_tc_q.opcode == L2Atomic) ||
    (pipe_req_info_tc_q.opcode == L3Fill) ||
    (pipe_req_info_tc_q.opcode == L3Write) ||
    ((pipe_req_info_tc_q.opcode == L3WritePartial) && tag_ram_hit_tc && !tag_ram_hit_partial_tc) ||
    ((pipe_req_info_tc_q.opcode == L3FlushWData) && tag_ram_hit_tc) ||
    ((pipe_req_info_tc_q.opcode == L3EvictWData) && tag_ram_hit_tc) ||
    (pipe_req_info_tc_q.opcode == L3Atomic) ||
    (pipe_req_info_tc_q.opcode == ScpFill) ||
    (pipe_req_info_tc_q.opcode == ScpWrite) ||
    (pipe_req_info_tc_q.opcode == ScpWritePartial) ||
    (pipe_req_info_tc_q.opcode == ScpAtomic));

  wire dataq_rd_req_partial_tc =
    (pipe_req_info_tc_q.opcode == L2WritePartial) ||
    (pipe_req_info_tc_q.opcode == L3WritePartial) ||
    (pipe_req_info_tc_q.opcode == ScpWritePartial);

  wire backfill_zero_data_tc =
    tag_ram_hit_zero_tc && !zero_next_tc &&
    ((pipe_req_info_tc_q.opcode == L2WritePartial) ||
     (pipe_req_info_tc_q.opcode == L2WriteAround) ||
     (pipe_req_info_tc_q.opcode == L3Write) ||
     (pipe_req_info_tc_q.opcode == L3WritePartial) ||
     (pipe_req_info_tc_q.opcode == L3FlushWData));

  wire [QWEN_SIZE-1:0] dataq_sel_qwen_tc = backfill_zero_data_tc ? input_qwen_tc : '1;

  wire [QWEN_SIZE-1:0] data_wr_qwen_tc =
    backfill_zero_data_tc                              ? '1 :
    (pipe_req_info_tc_q.opcode == L2WritePartial)      ? input_qwen_tc :
    (pipe_req_info_tc_q.opcode == L2WriteAround)       ? input_qwen_tc :
    (pipe_req_info_tc_q.opcode == L3Write)             ? input_qwen_tc :
    (pipe_req_info_tc_q.opcode == L3WritePartial)      ? input_qwen_tc :
    (pipe_req_info_tc_q.opcode == L3FlushWData)        ? input_qwen_tc :
    (pipe_req_info_tc_q.opcode == L3EvictWData)        ? input_qwen_tc :
    (pipe_req_info_tc_q.opcode == ScpWrite)            ? input_qwen_tc :
    (pipe_req_info_tc_q.opcode == ScpWritePartial)     ? input_qwen_tc :
    idx_write_data_tc                                  ? esr_dbg_wr_info_i.data_qwen :
    scrub_hit_partial_tc                               ? merged_qwen_tc :
                                                         '1;

  wire data_rsp_req_tc  = int_data_rsp_req_tc;
  wire data_rsp_zero_tc = tag_ram_hit_tc    ? tag_ram_hit_zero_tc :
                           tag_ram_victim_tc ? lru_way_zero_tc :
                                               '0;

  wire [QWEN_SIZE-1:0] data_rsp_qwen_tc =
    (atomic_hit_partial_tc || tag_ram_write_partial_tc || tag_ram_l2_write_partial_hit_partial_tc || tag_ram_l3_write_partial_hit_partial_tc || tag_ram_lock_partial_tc) ? tag_ram_hit_qwen_tc :
    (scp_operation_tc || idx_read_tc)                     ? '0 :
    (tag_ram_hit_tc || write_around_self_evict_bypass_tc) ? merged_qwen_tc :
    tag_ram_victim_tc                                     ? lru_way_qwen_tc :
                                                            '0;

  // ── calculate next tag_state ──
  tag_state_t                                tag_state_next_tc;
  logic [Ways-1:0][TagMaxSize-1:0]           tags_next_tc;
  always_comb begin
    tag_state_next_tc = tag_state_tc;
    tags_next_tc      = tags_tc;
    tag_state_next_tc.way_state[tag_ram_hit_way_tc].valid  = valid_next_tc;
    tag_state_next_tc.way_state[tag_ram_hit_way_tc].zero   = zero_next_tc;
    tag_state_next_tc.way_state[tag_ram_hit_way_tc].locked = locked_next_tc;
    tag_state_next_tc.way_state[tag_ram_hit_way_tc].qwen   = qwen_next_tc;
    tag_state_next_tc.lru_state                            = lru_state_next_tc;
    tags_next_tc[tag_ram_hit_way_tc] = idx_operation_tc ? tags_tc[tag_ram_hit_way_tc] :
                                        l3_operation_tc  ? l3_addr_tag_masked_tc :
                                                           l2_addr_tag_masked_tc;
    if (idx_write_tc && !esr_dbg_wr_info_i.ecc_wr_en) begin
      tag_state_next_tc = esr_dbg_wr_info_i.tag_state;
    end
    if (idx_write_tc && !esr_dbg_wr_info_i.ecc_wr_en) begin
      tags_next_tc = esr_dbg_wr_info_i.tags;
    end
    if (idx_all_inv_tc) begin
      tag_state_next_tc = '0;
    end
  end

  tag_rsp_t pipe_tag_rsp_info_tc;

  // create pipe_info
  assign pipe_info_tc.pipe_set                = int_pipe_info_tc_q.pipe_set;
  assign pipe_info_tc.pipe_sub_bank           = int_pipe_info_tc_q.pipe_sub_bank;
  assign pipe_info_tc.pipe_idx_address_space  = int_pipe_info_tc_q.pipe_idx_address_space;
  assign pipe_info_tc.pipe_dec_err            = int_pipe_info_tc_q.pipe_dec_err;
  assign pipe_info_tc.pipe_dec_err_type       = int_pipe_info_tc_q.pipe_dec_err_type;
  assign pipe_info_tc.pipe_scp_read           = int_pipe_info_tc_q.pipe_scp_read;
  assign pipe_info_tc.pipe_bubble             = int_pipe_info_tc_q.pipe_bubble;
  assign pipe_info_tc.idx_write               = idx_write_tc;
  assign pipe_info_tc.tag_hit                 = tag_ram_hit_tc;
  assign pipe_info_tc.tag_ram_hit_way         = tag_ram_hit_way_tc;
  assign pipe_info_tc.tag_sbe                 = pipe_tag_rsp_info_tc.tag_state_ram_sbe || (|pipe_tag_rsp_info_tc.tag_ram_sbe);
  assign pipe_info_tc.victim                  = tag_ram_victim_tc;
  assign pipe_info_tc.data_rd_req             = int_data_ram_rd_en_tc;
  assign pipe_info_tc.data_wr_req             = data_wr_req_tc;
  assign pipe_info_tc.data_wr_zero            = data_wr_zero_tc;
  assign pipe_info_tc.data_wr_qwen            = data_wr_qwen_tc;
  assign pipe_info_tc.dataq_rd_req_en         = dataq_rd_req_en_tc;
  assign pipe_info_tc.dataq_rd_req_partial    = dataq_rd_req_partial_tc;
  assign pipe_info_tc.dataq_sel_qwen          = dataq_sel_qwen_tc;
  assign pipe_info_tc.merge_wr_data_in_rd_rsp = merge_wr_data_in_rd_rsp_tc;
  assign pipe_info_tc.merge_rd_data_in_wr_req = merge_rd_data_in_wr_req_tc;
  assign pipe_info_tc.data_rsp_req            = data_rsp_req_tc;
  assign pipe_info_tc.data_rsp_zero           = data_rsp_zero_tc;
  assign pipe_info_tc.data_rsp_qwen           = data_rsp_qwen_tc;

  // assign tag_rsp fields
  wire pipe_tag_rsp_valid_tc = pipe_valid_tc_q;
  assign pipe_tag_rsp_info_tc.trans_id             = pipe_req_info_tc_q.trans_id;
  assign pipe_tag_rsp_info_tc.reqq_id              = pipe_req_info_tc_q.reqq_id;
  assign pipe_tag_rsp_info_tc.hit                  = pipe_info_tc.tag_hit;
  assign pipe_tag_rsp_info_tc.orig_address         = pipe_req_info_tc_q.address;
  assign pipe_tag_rsp_info_tc.victim               = pipe_info_tc.victim;
  assign pipe_tag_rsp_info_tc.victim_silent        = tag_ram_victim_silent_tc;
  assign pipe_tag_rsp_info_tc.victim_address       = victim_address_tc;
  assign pipe_tag_rsp_info_tc.victim_address_space = victim_address_space_tc;
  assign pipe_tag_rsp_info_tc.victim_write_around  = tag_ram_victim_write_around_tc;
  assign pipe_tag_rsp_info_tc.victim_qwen          = (idx_read_tc || idx_write_tc) ? '0 : data_rsp_qwen_tc;
  assign pipe_tag_rsp_info_tc.err_rsp              = err_rsp_tc;
  assign pipe_tag_rsp_info_tc.tag_index            = pipe_dec_err_tc ? '0 : pipe_info_tc.pipe_set;
  assign pipe_tag_rsp_info_tc.bubble               = pipe_info_tc.pipe_bubble;
  assign pipe_tag_rsp_info_tc.opcode               = pipe_req_info_tc_q.opcode;
  assign pipe_tag_rsp_info_tc.tag_ram_hit_way      = tag_ram_hit_way_tc;
  assign pipe_tag_rsp_info_tc.tag_ram_hit_qwen     = (idx_read_tc || idx_write_tc) ? '0 : tag_ram_hit_qwen_tc;
  assign pipe_tag_rsp_info_tc.tag_state_ram_sbe    = tag_state_ram_sbe_tc_q && !tag_replace_tc_q;
  assign pipe_tag_rsp_info_tc.tag_state_ram_dbe    = tag_state_ram_dbe_tc_q && !tag_replace_tc_q;
  assign pipe_tag_rsp_info_tc.tag_ram_sbe          = tag_ram_sbe_tc_q & tag_ram_valid_bits_tc & ~{Ways{tag_replace_tc_q}};
  assign pipe_tag_rsp_info_tc.tag_ram_dbe          = tag_ram_dbe_tc_q & tag_ram_valid_bits_tc & ~{Ways{tag_replace_tc_q}};


  wire pipe_cbuf_invalidate_req_valid_tc = pipe_valid_tc_q && (pipe_req_info_tc_q.opcode == IdxAllInv);
  wire pipe_cbuf_req_valid_tc = cbuf_req_valid_tc;
  cbuf_req_t pipe_cbuf_req_info_tc;
  assign pipe_cbuf_req_info_tc.trans_id        = pipe_req_info_tc_q.trans_id;
  assign pipe_cbuf_req_info_tc.reqq_id         = pipe_req_info_tc_q.reqq_id;
  assign pipe_cbuf_req_info_tc.rsp_required    = cbuf_rsp_required_tc;
  assign pipe_cbuf_req_info_tc.opcode          = cbuf_req_opcode_tc;
  assign pipe_cbuf_req_info_tc.install_address = cbuf_req_install_address_tc;
  assign pipe_cbuf_req_info_tc.clear_address   = cbuf_req_clear_address_tc;


  wire pipe_rbuf_invalidate_req_valid_tc = pipe_valid_tc_q && (pipe_req_info_tc_q.opcode == IdxAllInv);
  wire pipe_rbuf_install_req_valid_tc = pipe_valid_tc_q && tag_ram_hit_tc && !tag_ram_hit_partial_tc &&
    (((pipe_req_info_tc_q.opcode == L2Read)  && esr_sc_l2_rbuf_enable_i) ||
     ((pipe_req_info_tc_q.opcode == ScpRead) && esr_sc_scp_rbuf_enable_i));
  rbuf_install_req_t pipe_rbuf_install_req_info_tc;
  assign pipe_rbuf_install_req_info_tc.trans_id = pipe_req_info_tc_q.trans_id;
  assign pipe_rbuf_install_req_info_tc.reqq_id  = pipe_req_info_tc_q.reqq_id;
  assign pipe_rbuf_install_req_info_tc.scp      = (pipe_req_info_tc_q.opcode == ScpRead);
  assign pipe_rbuf_install_req_info_tc.address  = pipe_line_address_tc;

  wire                pipe_rbuf_evict_req_valid_tc = pipe_valid_tc_q && rbuf_cache_victim_tc;
  rbuf_install_req_t  pipe_rbuf_evict_req_info_tc;
  assign pipe_rbuf_evict_req_info_tc.trans_id = pipe_req_info_tc_q.trans_id;
  assign pipe_rbuf_evict_req_info_tc.reqq_id  = pipe_req_info_tc_q.reqq_id;
  assign pipe_rbuf_evict_req_info_tc.scp      = '0;
  assign pipe_rbuf_evict_req_info_tc.address  = rbuf_cache_victim_address_tc;


  // ════════════════════════════════════════════════════════
  // _tn (data pipeline)
  // ════════════════════════════════════════════════════════
  logic                               pipe_valid_tn_q;
  pipe_info_t                         pipe_info_tn_q;
  reqq_pipe_req_t                     pipe_req_info_tn_q;

  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) pipe_valid_tn_q <= '0;
    else if (pipe_valid_tn_q || pipe_valid_tc_q) pipe_valid_tn_q <= pipe_valid_tc_q;
  always_ff @(posedge clk_i) if (pipe_valid_tc_q) pipe_info_tn_q     <= pipe_info_tc;
  always_ff @(posedge clk_i) if (pipe_valid_tc_q) pipe_req_info_tn_q <= pipe_req_info_tc_q;

  // tag state/tag ram write signal assignments
  logic                               int_tag_state_ram_wr_en_tn_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_q;
  logic                               int_tag_ram_wr_en_tn_q;
  logic [TagsDataSize-1:0]            tags_tn_q;
  always_ff @(posedge clk_i) if (pipe_valid_tc_q) int_tag_state_ram_wr_en_tn_q <= tag_state_ram_wr_en_tc;
  always_ff @(posedge clk_i) if (pipe_valid_tc_q) tag_state_tn_q               <= tag_state_next_tc;
  always_ff @(posedge clk_i) if (pipe_valid_tc_q) int_tag_ram_wr_en_tn_q       <= tag_ram_wr_en_tc;
  always_ff @(posedge clk_i) if (pipe_valid_tc_q) tags_tn_q                    <= tags_next_tc;


  // assign outputs to registered out signals
  logic              pipe_tag_rsp_valid_tn_q;
  tag_rsp_t          pipe_tag_rsp_info_tn_q;
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) pipe_tag_rsp_valid_tn_q <= '0;
    else if (pipe_valid_tn_q || (pipe_valid_tc_q && esr_sc_ram_delay_3)) pipe_tag_rsp_valid_tn_q <= pipe_tag_rsp_valid_tc;
  always_ff @(posedge clk_i) if (pipe_valid_tc_q && esr_sc_ram_delay_3) pipe_tag_rsp_info_tn_q <= pipe_tag_rsp_info_tc;

  // mux between _tc/_tn for ram_delay_3
  logic              pipe_tag_rsp_valid_m1;
  tag_rsp_t          pipe_tag_rsp_info_m1;
  assign pipe_tag_rsp_valid_m1 = esr_sc_ram_delay_3 ? pipe_tag_rsp_valid_tn_q : pipe_tag_rsp_valid_tc;
  assign pipe_tag_rsp_info_m1  = esr_sc_ram_delay_3 ? pipe_tag_rsp_info_tn_q  : pipe_tag_rsp_info_tc;

  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) pipe_tag_rsp_valid_o <= '0;
    else if (pipe_tag_rsp_valid_o || pipe_tag_rsp_valid_m1) pipe_tag_rsp_valid_o <= pipe_tag_rsp_valid_m1;
  always_ff @(posedge clk_i)
    if (pipe_tag_rsp_valid_o || pipe_tag_rsp_valid_m1) pipe_tag_rsp_info_o <= pipe_tag_rsp_info_m1 & {$bits(pipe_tag_rsp_info_m1){pipe_tag_rsp_valid_m1}};


  // cbuf
  logic              pipe_cbuf_invalidate_req_valid_tn_q;
  logic              pipe_cbuf_req_valid_tn_q;
  cbuf_req_t         pipe_cbuf_req_info_tn_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_cbuf_invalidate_req_valid_tn_q <= '0; else if (pipe_cbuf_invalidate_req_valid_tn_q || pipe_cbuf_invalidate_req_valid_tc) pipe_cbuf_invalidate_req_valid_tn_q <= pipe_cbuf_invalidate_req_valid_tc;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_cbuf_req_valid_tn_q <= '0; else if (pipe_cbuf_req_valid_tn_q || pipe_cbuf_req_valid_tc) pipe_cbuf_req_valid_tn_q <= pipe_cbuf_req_valid_tc;
  always_ff @(posedge clk_i) if (pipe_cbuf_req_valid_tc) pipe_cbuf_req_info_tn_q <= pipe_cbuf_req_info_tc;
  assign pipe_cbuf_invalidate_req_valid_o = pipe_cbuf_invalidate_req_valid_tn_q;
  assign pipe_cbuf_req_valid_o            = pipe_cbuf_req_valid_tn_q;
  assign pipe_cbuf_req_info_o             = pipe_cbuf_req_info_tn_q & {$bits(pipe_cbuf_req_info_tn_q){pipe_cbuf_req_valid_tn_q}};


  // rbuf _tn
  logic                      pipe_rbuf_invalidate_req_valid_tn_q;
  logic                      pipe_rbuf_install_req_valid_tn_q;
  rbuf_install_req_t         pipe_rbuf_install_req_info_tn_q;
  logic                      pipe_rbuf_evict_req_valid_tn_q;
  rbuf_install_req_t         pipe_rbuf_evict_req_info_tn_q;

  wire pipe_rbuf_valid_tn_tc_en = pipe_valid_tn_q || pipe_valid_tc_q;

  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_rbuf_invalidate_req_valid_tn_q <= '0; else if (pipe_rbuf_valid_tn_tc_en) pipe_rbuf_invalidate_req_valid_tn_q <= pipe_rbuf_invalidate_req_valid_tc;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_rbuf_install_req_valid_tn_q <= '0; else if (pipe_rbuf_valid_tn_tc_en) pipe_rbuf_install_req_valid_tn_q <= pipe_rbuf_install_req_valid_tc;
  always_ff @(posedge clk_i) if (pipe_rbuf_install_req_valid_tc) pipe_rbuf_install_req_info_tn_q <= pipe_rbuf_install_req_info_tc;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_rbuf_evict_req_valid_tn_q <= '0; else if (pipe_rbuf_valid_tn_tc_en) pipe_rbuf_evict_req_valid_tn_q <= pipe_rbuf_evict_req_valid_tc;
  always_ff @(posedge clk_i) if (pipe_rbuf_evict_req_valid_tc) pipe_rbuf_evict_req_info_tn_q <= pipe_rbuf_evict_req_info_tc;

  // d0 aliases
  wire                      pipe_rbuf_invalidate_req_valid_tn_d0 = pipe_rbuf_invalidate_req_valid_tn_q;
  wire                      pipe_rbuf_install_req_valid_tn_d0    = pipe_rbuf_install_req_valid_tn_q;
  wire rbuf_install_req_t   pipe_rbuf_install_req_info_tn_d0     = pipe_rbuf_install_req_info_tn_q;
  wire                      pipe_rbuf_evict_req_valid_tn_d0      = pipe_rbuf_evict_req_valid_tn_q;
  wire rbuf_install_req_t   pipe_rbuf_evict_req_info_tn_d0       = pipe_rbuf_evict_req_info_tn_q;

  // d1
  logic                      pipe_rbuf_invalidate_req_valid_tn_d1_q;
  logic                      pipe_rbuf_install_req_valid_tn_d1_q;
  rbuf_install_req_t         pipe_rbuf_install_req_info_tn_d1_q;
  logic                      pipe_rbuf_evict_req_valid_tn_d1_q;
  rbuf_install_req_t         pipe_rbuf_evict_req_info_tn_d1_q;

  logic pipe_valid_tn_d1_q;
  wire  pipe_rbuf_valid_tn_d1_tn_d0_en = pipe_valid_tn_d1_q || (pipe_valid_tn_q && esr_sc_ram_delay_3);

  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_rbuf_invalidate_req_valid_tn_d1_q <= '0; else if (pipe_rbuf_valid_tn_d1_tn_d0_en) pipe_rbuf_invalidate_req_valid_tn_d1_q <= pipe_rbuf_invalidate_req_valid_tn_d0;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_rbuf_install_req_valid_tn_d1_q <= '0; else if (pipe_rbuf_valid_tn_d1_tn_d0_en) pipe_rbuf_install_req_valid_tn_d1_q <= pipe_rbuf_install_req_valid_tn_d0;
  always_ff @(posedge clk_i) if (pipe_rbuf_install_req_valid_tn_d0 && esr_sc_ram_delay_3) pipe_rbuf_install_req_info_tn_d1_q <= pipe_rbuf_install_req_info_tn_d0;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_rbuf_evict_req_valid_tn_d1_q <= '0; else if (pipe_rbuf_valid_tn_d1_tn_d0_en) pipe_rbuf_evict_req_valid_tn_d1_q <= pipe_rbuf_evict_req_valid_tn_d0;
  always_ff @(posedge clk_i) if (pipe_rbuf_evict_req_valid_tn_d0 && esr_sc_ram_delay_3) pipe_rbuf_evict_req_info_tn_d1_q <= pipe_rbuf_evict_req_info_tn_d0;

  assign pipe_rbuf_invalidate_req_valid_o = esr_sc_ram_delay_3 ? pipe_rbuf_invalidate_req_valid_tn_d1_q : pipe_rbuf_invalidate_req_valid_tn_d0;
  assign pipe_rbuf_install_req_valid_o    = esr_sc_ram_delay_3 ? pipe_rbuf_install_req_valid_tn_d1_q    : pipe_rbuf_install_req_valid_tn_d0;
  assign pipe_rbuf_install_req_info_o     = esr_sc_ram_delay_3 ? pipe_rbuf_install_req_info_tn_d1_q & {$bits(rbuf_install_req_t){pipe_rbuf_install_req_valid_tn_d1_q}} : pipe_rbuf_install_req_info_tn_d0 & {$bits(rbuf_install_req_t){pipe_rbuf_install_req_valid_tn_d0}};
  assign pipe_rbuf_evict_req_valid_o      = esr_sc_ram_delay_3 ? pipe_rbuf_evict_req_valid_tn_d1_q     : pipe_rbuf_evict_req_valid_tn_d0;
  assign pipe_rbuf_evict_req_info_o       = esr_sc_ram_delay_3 ? pipe_rbuf_evict_req_info_tn_d1_q & {$bits(rbuf_install_req_t){pipe_rbuf_evict_req_valid_tn_d1_q}} : pipe_rbuf_evict_req_info_tn_d0 & {$bits(rbuf_install_req_t){pipe_rbuf_evict_req_valid_tn_d0}};


  // ════════════════════════════════════════════════════════
  // tag_replace_te — set checking across tn_d0..tn_d7
  // ════════════════════════════════════════════════════════
  wire tag_state_update_tn = int_tag_state_ram_wr_en_tn_q;

  wire                                pipe_valid_tn_d0       = pipe_valid_tn_q;
  pipe_info_t                         pipe_info_tn_d0;
  assign                              pipe_info_tn_d0        = pipe_info_tn_q;
  wire                                tag_state_update_tn_d0 = tag_state_update_tn;
  wire [TagStateDataSize-1:0]         tag_state_tn_d0        = tag_state_tn_q;
  wire [TagsDataSize-1:0]             tags_tn_d0             = tags_tn_q;
  logic                               any_pipe_valid_tn_d0_tn_d7;

  // tn_d1
  pipe_info_t                         pipe_info_tn_d1_q;
  logic                               tag_state_update_tn_d1_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_d1_q;
  logic [TagsDataSize-1:0]            tags_tn_d1_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_tn_d1_q <= '0; else if (any_pipe_valid_tn_d0_tn_d7) pipe_valid_tn_d1_q <= pipe_valid_tn_d0;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d0) pipe_info_tn_d1_q          <= pipe_info_tn_d0;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d0) tag_state_update_tn_d1_q   <= tag_state_update_tn_d0;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d0) tag_state_tn_d1_q          <= tag_state_tn_d0;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d0) tags_tn_d1_q               <= tags_tn_d0;

  // tn_d2
  logic                               pipe_valid_tn_d2_q;
  pipe_info_t                         pipe_info_tn_d2_q;
  logic                               tag_state_update_tn_d2_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_d2_q;
  logic [TagsDataSize-1:0]            tags_tn_d2_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_tn_d2_q <= '0; else if (any_pipe_valid_tn_d0_tn_d7) pipe_valid_tn_d2_q <= pipe_valid_tn_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d1_q) pipe_info_tn_d2_q        <= pipe_info_tn_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d1_q) tag_state_update_tn_d2_q <= tag_state_update_tn_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d1_q) tag_state_tn_d2_q        <= tag_state_tn_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d1_q) tags_tn_d2_q             <= tags_tn_d1_q;

  // tn_d3
  logic                               pipe_valid_tn_d3_q;
  pipe_info_t                         pipe_info_tn_d3_q;
  logic                               tag_state_update_tn_d3_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_d3_q;
  logic [TagsDataSize-1:0]            tags_tn_d3_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_tn_d3_q <= '0; else if (any_pipe_valid_tn_d0_tn_d7) pipe_valid_tn_d3_q <= pipe_valid_tn_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d2_q) pipe_info_tn_d3_q        <= pipe_info_tn_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d2_q) tag_state_update_tn_d3_q <= tag_state_update_tn_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d2_q) tag_state_tn_d3_q        <= tag_state_tn_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d2_q) tags_tn_d3_q             <= tags_tn_d2_q;

  // tn_d4
  logic                               pipe_valid_tn_d4_q;
  pipe_info_t                         pipe_info_tn_d4_q;
  logic                               tag_state_update_tn_d4_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_d4_q;
  logic [TagsDataSize-1:0]            tags_tn_d4_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_tn_d4_q <= '0; else if (any_pipe_valid_tn_d0_tn_d7) pipe_valid_tn_d4_q <= pipe_valid_tn_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d3_q) pipe_info_tn_d4_q        <= pipe_info_tn_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d3_q) tag_state_update_tn_d4_q <= tag_state_update_tn_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d3_q) tag_state_tn_d4_q        <= tag_state_tn_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d3_q) tags_tn_d4_q             <= tags_tn_d3_q;

  // dual port: tn_d5..tn_d7
  logic                               pipe_valid_tn_d5_q;
  pipe_info_t                         pipe_info_tn_d5_q;
  logic                               tag_state_update_tn_d5_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_d5_q;
  logic [TagsDataSize-1:0]            tags_tn_d5_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_tn_d5_q <= '0; else if (any_pipe_valid_tn_d0_tn_d7) pipe_valid_tn_d5_q <= pipe_valid_tn_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d4_q) pipe_info_tn_d5_q        <= pipe_info_tn_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d4_q) tag_state_update_tn_d5_q <= tag_state_update_tn_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d4_q) tag_state_tn_d5_q        <= tag_state_tn_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d4_q) tags_tn_d5_q             <= tags_tn_d4_q;

  logic                               pipe_valid_tn_d6_q;
  pipe_info_t                         pipe_info_tn_d6_q;
  logic                               tag_state_update_tn_d6_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_d6_q;
  logic [TagsDataSize-1:0]            tags_tn_d6_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_tn_d6_q <= '0; else if (any_pipe_valid_tn_d0_tn_d7) pipe_valid_tn_d6_q <= pipe_valid_tn_d5_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d5_q) pipe_info_tn_d6_q        <= pipe_info_tn_d5_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d5_q) tag_state_update_tn_d6_q <= tag_state_update_tn_d5_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d5_q) tag_state_tn_d6_q        <= tag_state_tn_d5_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d5_q) tags_tn_d6_q             <= tags_tn_d5_q;

  logic                               pipe_valid_tn_d7_q;
  pipe_info_t                         pipe_info_tn_d7_q;
  logic                               tag_state_update_tn_d7_q;
  logic [TagStateDataSize-1:0]        tag_state_tn_d7_q;
  logic [TagsDataSize-1:0]            tags_tn_d7_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_tn_d7_q <= '0; else if (any_pipe_valid_tn_d0_tn_d7) pipe_valid_tn_d7_q <= pipe_valid_tn_d6_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d6_q) pipe_info_tn_d7_q        <= pipe_info_tn_d6_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d6_q) tag_state_update_tn_d7_q <= tag_state_update_tn_d6_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d6_q) tag_state_tn_d7_q        <= tag_state_tn_d6_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d6_q) tags_tn_d7_q             <= tags_tn_d6_q;

  assign any_pipe_valid_tn_d0_tn_d7 =
    pipe_valid_tn_d0 || pipe_valid_tn_d1_q || pipe_valid_tn_d2_q || pipe_valid_tn_d3_q ||
    pipe_valid_tn_d4_q || pipe_valid_tn_d5_q || pipe_valid_tn_d6_q || pipe_valid_tn_d7_q;

  // match (sub_bank/set) and replace if match
  wire set_match_te_tn_d0 = pipe_valid_te_q && pipe_valid_tn_d0 && tag_state_update_tn_d0 && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d0.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d0.pipe_set);
  wire set_match_te_tn_d1 = pipe_valid_te_q && pipe_valid_tn_d1_q && tag_state_update_tn_d1_q && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d1_q.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d1_q.pipe_set);
  wire set_match_te_tn_d2 = pipe_valid_te_q && pipe_valid_tn_d2_q && tag_state_update_tn_d2_q && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d2_q.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d2_q.pipe_set);
  wire set_match_te_tn_d3 = pipe_valid_te_q && pipe_valid_tn_d3_q && tag_state_update_tn_d3_q && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d3_q.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d3_q.pipe_set);
  wire set_match_te_tn_d4 = pipe_valid_te_q && pipe_valid_tn_d4_q && tag_state_update_tn_d4_q && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d4_q.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d4_q.pipe_set);
  wire set_match_te_tn_d5 = pipe_valid_te_q && pipe_valid_tn_d5_q && tag_state_update_tn_d5_q && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d5_q.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d5_q.pipe_set);
  wire set_match_te_tn_d6 = pipe_valid_te_q && pipe_valid_tn_d6_q && tag_state_update_tn_d6_q && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d6_q.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d6_q.pipe_set);
  wire set_match_te_tn_d7 = pipe_valid_te_q && pipe_valid_tn_d7_q && tag_state_update_tn_d7_q && (pipe_info_te_q.pipe_sub_bank == pipe_info_tn_d7_q.pipe_sub_bank) && (pipe_info_te_q.pipe_set == pipe_info_tn_d7_q.pipe_set);

  // dual port tag state ram
  assign tag_replace_te =
    set_match_te_tn_d0 || set_match_te_tn_d1 || set_match_te_tn_d2 ||
    set_match_te_tn_d4 || set_match_te_tn_d6 || set_match_te_tn_d7;

  assign tag_state_replace_te =
    set_match_te_tn_d0 ? tag_state_tn_d0 :
    set_match_te_tn_d1 ? tag_state_tn_d1_q :
    set_match_te_tn_d2 ? tag_state_tn_d2_q :
    set_match_te_tn_d4 ? tag_state_tn_d4_q :
    set_match_te_tn_d6 ? tag_state_tn_d6_q :
                          tag_state_tn_d7_q;

  assign tags_replace_te =
    set_match_te_tn_d0 ? tags_tn_d0 :
    set_match_te_tn_d1 ? tags_tn_d1_q :
    set_match_te_tn_d2 ? tags_tn_d2_q :
    set_match_te_tn_d4 ? tags_tn_d4_q :
    set_match_te_tn_d6 ? tags_tn_d6_q :
                          tags_tn_d7_q;


  // ── wr_en delay for ram_delay_3 ──
  reqq_pipe_req_t                  pipe_req_info_tn_d0;
  wire                             int_tag_state_ram_wr_en_tn_d0 = int_tag_state_ram_wr_en_tn_q;
  wire                             int_tag_ram_wr_en_tn_d0       = int_tag_ram_wr_en_tn_q;
  assign                           pipe_req_info_tn_d0           = pipe_req_info_tn_q;

  logic                            int_tag_state_ram_wr_en_tn_d1_q;
  logic                            int_tag_ram_wr_en_tn_d1_q;
  reqq_pipe_req_t                  pipe_req_info_tn_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d0) int_tag_state_ram_wr_en_tn_d1_q <= int_tag_state_ram_wr_en_tn_d0;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d0) int_tag_ram_wr_en_tn_d1_q       <= int_tag_ram_wr_en_tn_d0;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d0) pipe_req_info_tn_d1_q           <= pipe_req_info_tn_d0;

  logic                            int_tag_state_ram_wr_en_tn_d2_q;
  logic                            int_tag_ram_wr_en_tn_d2_q;
  reqq_pipe_req_t                  pipe_req_info_tn_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d1_q && esr_sc_ram_delay_3) int_tag_state_ram_wr_en_tn_d2_q <= int_tag_state_ram_wr_en_tn_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d1_q && esr_sc_ram_delay_3) int_tag_ram_wr_en_tn_d2_q       <= int_tag_ram_wr_en_tn_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_tn_d1_q && esr_sc_ram_delay_3) pipe_req_info_tn_d2_q           <= pipe_req_info_tn_d1_q;


  // tag state ram write signal assignments
  assign tag_state_ram_wr_en_tn       = esr_sc_ram_delay_3 ? pipe_valid_tn_d2_q & int_tag_state_ram_wr_en_tn_d2_q : pipe_valid_tn_d0 & int_tag_state_ram_wr_en_tn_d0;
  assign tag_state_ram_wr_sub_bank_tn = esr_sc_ram_delay_3 ? pipe_info_tn_d2_q.pipe_sub_bank                      : pipe_info_tn_d0.pipe_sub_bank;
  assign tag_state_ram_wr_trans_id_tn = esr_sc_ram_delay_3 ? pipe_req_info_tn_d2_q.trans_id                       : pipe_req_info_tn_d0.trans_id;
  assign tag_state_ram_wr_reqq_id_tn  = esr_sc_ram_delay_3 ? pipe_req_info_tn_d2_q.reqq_id                        : pipe_req_info_tn_d0.reqq_id;
  assign tag_state_ram_wr_addr_tn     = esr_sc_ram_delay_3 ? pipe_info_tn_d2_q.pipe_set                           : pipe_info_tn_d0.pipe_set;
  wire   idx_write_post_ecc_mux_tn    = (esr_sc_ram_delay_3 ? pipe_info_tn_d2_q.idx_write : pipe_info_tn_d0.idx_write) && esr_dbg_wr_info_i.ecc_wr_en;

  // ecc generation — tag state
  wire [TagStateDataSize-1:0]      tag_state_ram_wr_data_pre_ecc_tn = esr_sc_ram_delay_3 ? tag_state_tn_d2_q : tag_state_tn_d0;
  logic [TagStateRamDataSize-1:0]  tag_state_ram_wr_data_ecc_tn;
  prim_ecc_enc #(
    .DataWidth(TagStateDataSize)
  ) tag_state_ram_ecc_gen (
    .data_i         (tag_state_ram_wr_data_pre_ecc_tn),
    .data_and_ecc_o (tag_state_ram_wr_data_ecc_tn)
  );

  // sbe insert — tag state
  wire [TagStateRamDataSize-1:0] tag_state_ram_wr_data_bit_flip_tn;
  shirecache_pipe_sbe_insert #(
    .DataWidth    (TagStateDataSize),
    .EccWidth     (TagStateEccSize),
    .InsertEnable (1'b0)
  ) tag_state_ram_sbe_insert (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .wr_en_i            (tag_state_ram_wr_en_tn),
    .data_ecc_bit_flip_o(tag_state_ram_wr_data_bit_flip_tn)
  );

  assign tag_state_ram_wr_data_tn =
    idx_write_post_ecc_mux_tn ? esr_dbg_wr_info_i.tag_state_and_ecc :
                                (tag_state_ram_wr_data_ecc_tn ^ tag_state_ram_wr_data_bit_flip_tn);


  // tag ram write signal assignments
  assign tag_ram_wr_en_tn       = esr_sc_ram_delay_3 ? pipe_valid_tn_d2_q & int_tag_ram_wr_en_tn_d2_q : pipe_valid_tn_d0 & int_tag_ram_wr_en_tn_d0;
  assign tag_ram_wr_sub_bank_tn = esr_sc_ram_delay_3 ? pipe_info_tn_d2_q.pipe_sub_bank               : pipe_info_tn_d0.pipe_sub_bank;
  assign tag_ram_wr_trans_id_tn = esr_sc_ram_delay_3 ? pipe_req_info_tn_d2_q.trans_id                : pipe_req_info_tn_d0.trans_id;
  assign tag_ram_wr_reqq_id_tn  = esr_sc_ram_delay_3 ? pipe_req_info_tn_d2_q.reqq_id                 : pipe_req_info_tn_d0.reqq_id;
  assign tag_ram_wr_addr_tn     = esr_sc_ram_delay_3 ? pipe_info_tn_d2_q.pipe_set                    : pipe_info_tn_d0.pipe_set;

  // ecc generation — tags
  wire [TagsDataSize-1:0]       tag_ram_wr_data_pre_ecc_tn = esr_sc_ram_delay_3 ? tags_tn_d2_q : tags_tn_d0;
  logic [TagRamDataSize-1:0]    tag_ram_wr_data_ecc_tn;
  prim_ecc_enc #(
    .DataWidth(TagDataSize)
  ) tag_ram_ecc_gen [Ways-1:0] (
    .data_i         (tag_ram_wr_data_pre_ecc_tn),
    .data_and_ecc_o (tag_ram_wr_data_ecc_tn)
  );

  // sbe insert — tags
  wire [TagRamDataSize-1:0] tag_ram_wr_data_bit_flip_tn;
  shirecache_pipe_sbe_insert #(
    .DataWidth    (TagDataSize),
    .EccWidth     (TagEccSize),
    .InsertEnable (1'b0)
  ) tag_ram_sbe_insert [Ways-1:0] (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .wr_en_i            (tag_ram_wr_en_tn),
    .data_ecc_bit_flip_o(tag_ram_wr_data_bit_flip_tn)
  );

  assign tag_ram_wr_data_tn =
    idx_write_post_ecc_mux_tn ? esr_dbg_wr_info_i.tags_and_ecc : (tag_ram_wr_data_ecc_tn ^ tag_ram_wr_data_bit_flip_tn);


  // ════════════════════════════════════════════════════════
  // pipe_squash_tap
  // ════════════════════════════════════════════════════════
  assign pipe_squash_tap_o =
    pipe_valid_tap_i && (pipe_req_info_tap_i.address_space == AddressScp) && (
      (pipe_valid_ta_d1 && pipe_info_ta_d1_q.pipe_scp_read && (get_line_addr(pipe_req_info_ta_d1_q.address) == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_ta_d2 && pipe_info_ta_d2_q.pipe_scp_read && (get_line_addr(pipe_req_info_ta_d2_q.address) == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_ta_d3 && pipe_info_ta_d3_q.pipe_scp_read && (get_line_addr(pipe_req_info_ta_d3_q.address) == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_ta_d4 && pipe_info_ta_d4_q.pipe_scp_read && (get_line_addr(pipe_req_info_ta_d4_q.address) == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_te_q  && pipe_info_te_q.pipe_scp_read    && (get_line_addr(pipe_req_info_te_q.address)    == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_tc_q  && pipe_info_tc.pipe_scp_read      && (get_line_addr(pipe_req_info_tc_q.address)    == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_tn_d0 && pipe_info_tn_d0.pipe_scp_read   && (get_line_addr(pipe_req_info_tn_d0.address)   == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_tn_d1_q && pipe_info_tn_d1_q.pipe_scp_read && (get_line_addr(pipe_req_info_tn_d1_q.address) == get_line_addr(pipe_req_info_tap_i.address))) ||
      (pipe_valid_tn_d2_q && pipe_info_tn_d2_q.pipe_scp_read && (get_line_addr(pipe_req_info_tn_d2_q.address) == get_line_addr(pipe_req_info_tap_i.address)) && esr_sc_ram_delay_3)
    );


  // ════════════════════════════════════════════════════════
  // Data RAM muxing
  // ════════════════════════════════════════════════════════
  // normal writes
  logic                              data_ram_wr_en_dc;
  logic [SubBanks-1:0]              data_ram_wr_sub_bank_dc;
  logic [DvTransIdSize-1:0]         data_ram_wr_trans_id_dc;
  logic [ReqqIdSize-1:0]            data_ram_wr_reqq_id_dc;
  logic [DataRamAddrSize-1:0]       data_ram_wr_addr_dc;
  logic [DataRamDataSize-1:0]       data_ram_wr_data_dc;
  logic [QWEN_SIZE-1:0]             data_ram_wr_qwen_dc;
  logic                              data_ram_wr_xcheck_disable_dc;

  // atomic
  pipe_info_t                        pipe_info_dc_dAMO;
  reqq_pipe_req_t                    pipe_req_info_dc_dAMO;
  logic                              atomic_result_wr_en_dc_dAMO;
  logic [DataRamAddrSize-1:0]       atomic_result_wr_addr_dc_dAMO;
  logic [LineDataSize-1:0]          atomic_result_wr_data_dc_dAMO;
  logic [QWEN_SIZE-1:0]             atomic_result_wr_qwen_dc_dAMO;

  // bist muxing
  wire                              mbd_rd_en_xx       = pipe_bist_req_info_i.mbd_sel && pipe_bist_req_info_i.mbd_rd_en;
  wire                              mbd_wr_en_xx       = pipe_bist_req_info_i.mbd_sel && pipe_bist_req_info_i.mbd_wr_en;
  wire                              mbd_access_xx      = (mbd_rd_en_xx | mbd_wr_en_xx);
  wire [DataRamAddrSize-1:0]       mbd_addr_xx        = pipe_bist_req_info_i.mbd_addr[0 +: DataRamAddrSize];
  wire [LineQwIdSize-1:0]          mbd_qwen_id_xx     = pipe_bist_req_info_i.mbd_addr[DataRamAddrSize +: LineQwIdSize];
  wire [SubBankIdSize-1:0]         mbd_sub_bank_id_xx = pipe_bist_req_info_i.mbd_addr[(DataRamAddrSize + LineQwIdSize) +: SubBankIdSize];
  wire [SubBanks-1:0]             mbd_sub_bank_xx    = (SubBanks'(1'b1) << mbd_sub_bank_id_xx);
  wire [DataRamDataSize-1:0]       mbd_data_xx        = {LineQwSize{pipe_bist_req_info_i.mbd_wdata}};
  wire [QWEN_SIZE-1:0]             mbd_qwen_xx        = (LineQwSize'(1'b1) << mbd_qwen_id_xx);

  wire data_ram_rd_en_tc             = mbd_access_xx ? mbd_rd_en_xx    : (esr_sc_ram_delay_3 ? pipe_valid_tn_d1_q & pipe_info_tn_d1_q.data_rd_req              : pipe_valid_tc_q & pipe_info_tc.data_rd_req);
  wire [SubBanks-1:0] data_ram_rd_sub_bank_tc = mbd_access_xx ? mbd_sub_bank_xx : (esr_sc_ram_delay_3 ? pipe_info_tn_d1_q.pipe_sub_bank               : pipe_info_tc.pipe_sub_bank);
  wire [DvTransIdSize-1:0] data_ram_rd_trans_id_tc = mbd_access_xx ? '0 : (esr_sc_ram_delay_3 ? pipe_req_info_tn_d1_q.trans_id : pipe_req_info_tc_q.trans_id);
  wire [ReqqIdSize-1:0] data_ram_rd_reqq_id_tc = mbd_access_xx ? '0 : (esr_sc_ram_delay_3 ? pipe_req_info_tn_d1_q.reqq_id : pipe_req_info_tc_q.reqq_id);
  wire [DataRamAddrSize-1:0] data_ram_rd_addr_tc = mbd_access_xx ? mbd_addr_xx : (esr_sc_ram_delay_3 ? {pipe_info_tn_d1_q.pipe_set, pipe_info_tn_d1_q.tag_ram_hit_way} : {pipe_info_tc.pipe_set, pipe_info_tc.tag_ram_hit_way});

  wire data_ram_wr_en_tc             = mbd_access_xx ? mbd_wr_en_xx     : data_ram_wr_en_dc;
  wire [SubBanks-1:0] data_ram_wr_sub_bank_tc = mbd_access_xx ? mbd_sub_bank_xx  : data_ram_wr_sub_bank_dc;
  wire [DvTransIdSize-1:0] data_ram_wr_trans_id_tc = mbd_access_xx ? '0 : data_ram_wr_trans_id_dc;
  wire [ReqqIdSize-1:0] data_ram_wr_reqq_id_tc = mbd_access_xx ? '0 : data_ram_wr_reqq_id_dc;
  wire [DataRamAddrSize-1:0] data_ram_wr_addr_tc = mbd_access_xx ? mbd_addr_xx : data_ram_wr_addr_dc;
  wire [DataRamDataSize-1:0] data_ram_wr_data_tc = mbd_access_xx ? mbd_data_xx : data_ram_wr_data_dc;
  wire [QWEN_SIZE-1:0] data_ram_wr_qwen_tc = mbd_access_xx ? mbd_qwen_xx : data_ram_wr_qwen_dc;
  wire data_ram_wr_xcheck_disable_tc = mbd_access_xx ? '0 : data_ram_wr_xcheck_disable_dc;

  // atomic ecc
  wire [LineDataSize-1:0] atomic_result_data_ram_wr_data_pre_ecc_dc_dAMO = atomic_result_wr_data_dc_dAMO;
  logic [DataRamDataSize-1:0] atomic_result_data_ram_wr_data_ecc_dc_dAMO;
  prim_ecc_enc #(.DataWidth(DwDataSize)) atomic_result_data_ram_ecc_gen [LineDwSize-1:0] (
    .data_i         (atomic_result_data_ram_wr_data_pre_ecc_dc_dAMO),
    .data_and_ecc_o (atomic_result_data_ram_wr_data_ecc_dc_dAMO)
  );
  wire [DataRamDataSize-1:0] atomic_result_data_ram_wr_data_bit_flip_dc_dAMO;
  shirecache_pipe_sbe_insert #(.DataWidth(DwDataSize), .EccWidth(DwEccSize), .InsertEnable(1'b0)) atomic_result_data_ram_sbe_insert [LineDwSize-1:0] (
    .clk_i(clk_i), .rst_ni(rst_ni), .wr_en_i(atomic_result_wr_en_dc_dAMO), .data_ecc_bit_flip_o(atomic_result_data_ram_wr_data_bit_flip_dc_dAMO)
  );
  wire [DataRamDataSize-1:0] atomic_result_data_ram_wr_data_dc = atomic_result_data_ram_wr_data_ecc_dc_dAMO ^ atomic_result_data_ram_wr_data_bit_flip_dc_dAMO;

  wire                         data_ram_amo_wr_en_tc       = atomic_result_wr_en_dc_dAMO;
  wire [SubBanks-1:0]         data_ram_amo_wr_sub_bank_tc = pipe_info_dc_dAMO.pipe_sub_bank;
  wire [DvTransIdSize-1:0]    data_ram_amo_wr_trans_id_tc = pipe_req_info_dc_dAMO.trans_id;
  wire [ReqqIdSize-1:0]       data_ram_amo_wr_reqq_id_tc  = pipe_req_info_dc_dAMO.reqq_id;
  wire [DataRamAddrSize-1:0]  data_ram_amo_wr_addr_tc     = atomic_result_wr_addr_dc_dAMO;
  wire [DataRamDataSize-1:0]  data_ram_amo_wr_data_tc     = atomic_result_data_ram_wr_data_dc;
  wire [QWEN_SIZE-1:0]        data_ram_amo_wr_qwen_tc     = atomic_result_wr_qwen_dc_dAMO;


  // ════════════════════════════════════════════════════════
  // _dap (data pipeline)
  // ════════════════════════════════════════════════════════
  logic                              pipe_valid_dap;
  pipe_info_t                        pipe_info_dap;
  reqq_pipe_req_t                    pipe_req_info_dap;
  assign pipe_valid_dap    = esr_sc_ram_delay_3 ? pipe_valid_tn_d2_q    : pipe_valid_tn_q;
  assign pipe_info_dap     = esr_sc_ram_delay_3 ? pipe_info_tn_d2_q     : pipe_info_tn_q;
  assign pipe_req_info_dap = esr_sc_ram_delay_3 ? pipe_req_info_tn_d2_q : pipe_req_info_tn_q;

  logic data_ram_rd_en_dap_q;
  logic [SubBanks-1:0] data_ram_rd_sub_bank_dap_q;
  logic [DvTransIdSize-1:0] data_ram_rd_trans_id_dap_q;
  logic [ReqqIdSize-1:0] data_ram_rd_reqq_id_dap_q;
  logic [DataRamAddrSize-1:0] data_ram_rd_addr_dap_q;
  logic data_ram_wr_en_dap_q;
  logic [SubBanks-1:0] data_ram_wr_sub_bank_dap_q;
  logic [DvTransIdSize-1:0] data_ram_wr_trans_id_dap_q;
  logic [ReqqIdSize-1:0] data_ram_wr_reqq_id_dap_q;
  logic [DataRamAddrSize-1:0] data_ram_wr_addr_dap_q;
  logic [DataRamDataSize-1:0] data_ram_wr_data_dap_q;
  logic [QWEN_SIZE-1:0] data_ram_wr_qwen_dap_q;
  logic data_ram_wr_xcheck_disable_dap_q;
  logic data_ram_amo_wr_en_dap_q;
  logic [SubBanks-1:0] data_ram_amo_wr_sub_bank_dap_q;
  logic [DvTransIdSize-1:0] data_ram_amo_wr_trans_id_dap_q;
  logic [ReqqIdSize-1:0] data_ram_amo_wr_reqq_id_dap_q;
  logic [DataRamAddrSize-1:0] data_ram_amo_wr_addr_dap_q;
  logic [DataRamDataSize-1:0] data_ram_amo_wr_data_dap_q;
  logic [QWEN_SIZE-1:0] data_ram_amo_wr_qwen_dap_q;

  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) data_ram_rd_en_dap_q <= '0;  else if (data_ram_rd_en_dap_q || data_ram_rd_en_tc) data_ram_rd_en_dap_q <= data_ram_rd_en_tc;
  always_ff @(posedge clk_i) if (data_ram_rd_en_tc) begin data_ram_rd_sub_bank_dap_q <= data_ram_rd_sub_bank_tc; data_ram_rd_trans_id_dap_q <= data_ram_rd_trans_id_tc; data_ram_rd_reqq_id_dap_q <= data_ram_rd_reqq_id_tc; data_ram_rd_addr_dap_q <= data_ram_rd_addr_tc; end
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) data_ram_wr_en_dap_q <= '0;  else if (data_ram_wr_en_dap_q || data_ram_wr_en_tc) data_ram_wr_en_dap_q <= data_ram_wr_en_tc;
  always_ff @(posedge clk_i) if (data_ram_wr_en_tc) begin data_ram_wr_sub_bank_dap_q <= data_ram_wr_sub_bank_tc; data_ram_wr_trans_id_dap_q <= data_ram_wr_trans_id_tc; data_ram_wr_reqq_id_dap_q <= data_ram_wr_reqq_id_tc; data_ram_wr_addr_dap_q <= data_ram_wr_addr_tc; data_ram_wr_data_dap_q <= data_ram_wr_data_tc; data_ram_wr_qwen_dap_q <= data_ram_wr_qwen_tc; data_ram_wr_xcheck_disable_dap_q <= data_ram_wr_xcheck_disable_tc; end
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) data_ram_amo_wr_en_dap_q <= '0; else if (data_ram_amo_wr_en_dap_q || data_ram_amo_wr_en_tc) data_ram_amo_wr_en_dap_q <= data_ram_amo_wr_en_tc;
  always_ff @(posedge clk_i) if (data_ram_amo_wr_en_tc) begin data_ram_amo_wr_sub_bank_dap_q <= data_ram_amo_wr_sub_bank_tc; data_ram_amo_wr_trans_id_dap_q <= data_ram_amo_wr_trans_id_tc; data_ram_amo_wr_reqq_id_dap_q <= data_ram_amo_wr_reqq_id_tc; data_ram_amo_wr_addr_dap_q <= data_ram_amo_wr_addr_tc; data_ram_amo_wr_data_dap_q <= data_ram_amo_wr_data_tc; data_ram_amo_wr_qwen_dap_q <= data_ram_amo_wr_qwen_tc; end

  // assign to data ram_req ports
  assign pipe_data_ram_req_rd_en_o                  = {SubBanks{data_ram_rd_en_dap_q}} & data_ram_rd_sub_bank_dap_q;
  assign pipe_data_ram_req_info_o.rd_trans_id       = data_ram_rd_trans_id_dap_q;
  assign pipe_data_ram_req_info_o.rd_reqq_id        = data_ram_rd_reqq_id_dap_q;
  assign pipe_data_ram_req_info_o.rd_addr           = data_ram_rd_addr_dap_q;
  assign pipe_data_ram_req_wr_en_o                  = {SubBanks{data_ram_wr_en_dap_q}} & data_ram_wr_sub_bank_dap_q;
  assign pipe_data_ram_req_info_o.wr_trans_id       = data_ram_wr_trans_id_dap_q;
  assign pipe_data_ram_req_info_o.wr_reqq_id        = data_ram_wr_reqq_id_dap_q;
  assign pipe_data_ram_req_info_o.wr_addr           = data_ram_wr_addr_dap_q;
  assign pipe_data_ram_req_info_o.wr_data           = data_ram_wr_data_dap_q;
  assign pipe_data_ram_req_info_o.wr_qwen           = data_ram_wr_qwen_dap_q;
  assign pipe_data_ram_req_info_o.wr_xcheck_disable = data_ram_wr_xcheck_disable_dap_q;
  assign pipe_data_ram_req_amo_wr_en_o              = {SubBanks{data_ram_amo_wr_en_dap_q}} & data_ram_amo_wr_sub_bank_dap_q;
  assign pipe_data_ram_req_info_o.amo_wr_trans_id   = data_ram_amo_wr_trans_id_dap_q;
  assign pipe_data_ram_req_info_o.amo_wr_reqq_id    = data_ram_amo_wr_reqq_id_dap_q;
  assign pipe_data_ram_req_info_o.amo_wr_addr       = data_ram_amo_wr_addr_dap_q;
  assign pipe_data_ram_req_info_o.amo_wr_data       = data_ram_amo_wr_data_dap_q;
  assign pipe_data_ram_req_info_o.amo_wr_qwen       = data_ram_amo_wr_qwen_dap_q;


  // ════════════════════════════════════════════════════════
  // _da (data ram access)
  // ════════════════════════════════════════════════════════
  logic pipe_valid_da_q;
  pipe_info_t pipe_info_da_q;
  reqq_pipe_req_t pipe_req_info_da_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_da_q <= '0; else if (pipe_valid_da_q || pipe_valid_dap) pipe_valid_da_q <= pipe_valid_dap;
  always_ff @(posedge clk_i) if (pipe_valid_dap) pipe_info_da_q <= pipe_info_dap;
  always_ff @(posedge clk_i) if (pipe_valid_dap) pipe_req_info_da_q <= pipe_req_info_dap;

  wire pipe_valid_da_d0 = pipe_valid_da_q;
  logic [RAM_DLY-1:0] vec_pipe_valid_da_dN_q;
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) vec_pipe_valid_da_dN_q <= '0;
    else if (|{vec_pipe_valid_da_dN_q, pipe_valid_da_d0}) vec_pipe_valid_da_dN_q <= {vec_pipe_valid_da_dN_q[0 +: RAM_DLY-1], pipe_valid_da_d0};

  wire pipe_valid_da_d1 = vec_pipe_valid_da_dN_q[0];
  wire pipe_valid_da_d2 = vec_pipe_valid_da_dN_q[1];
  wire pipe_valid_da_d3 = vec_pipe_valid_da_dN_q[2];
  wire pipe_valid_da_d4 = vec_pipe_valid_da_dN_q[3];
  wire pipe_valid_da_dN = (esr_sc_ram_delay_2 & pipe_valid_da_d2) || (esr_sc_ram_delay_3 & pipe_valid_da_d3) || (esr_sc_ram_delay_4 & pipe_valid_da_d4);

  // pipeline info through _da stages
  pipe_info_t pipe_info_da_d0; reqq_pipe_req_t pipe_req_info_da_d0;
  assign pipe_info_da_d0 = pipe_info_da_q; assign pipe_req_info_da_d0 = pipe_req_info_da_q;
  pipe_info_t pipe_info_da_d1_q; reqq_pipe_req_t pipe_req_info_da_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_da_d0) begin pipe_info_da_d1_q <= pipe_info_da_d0; pipe_req_info_da_d1_q <= pipe_req_info_da_d0; end
  pipe_info_t pipe_info_da_d2_q; reqq_pipe_req_t pipe_req_info_da_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_da_d1) begin pipe_info_da_d2_q <= pipe_info_da_d1_q; pipe_req_info_da_d2_q <= pipe_req_info_da_d1_q; end
  pipe_info_t pipe_info_da_d3_q; reqq_pipe_req_t pipe_req_info_da_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_da_d2 && (esr_sc_ram_delay_3 || esr_sc_ram_delay_4)) begin pipe_info_da_d3_q <= pipe_info_da_d2_q; pipe_req_info_da_d3_q <= pipe_req_info_da_d2_q; end
  pipe_info_t pipe_info_da_d4_q; reqq_pipe_req_t pipe_req_info_da_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_da_d3 && esr_sc_ram_delay_4) begin pipe_info_da_d4_q <= pipe_info_da_d3_q; pipe_req_info_da_d4_q <= pipe_req_info_da_d3_q; end

  pipe_info_t pipe_info_da_dN;
  assign pipe_info_da_dN = ({$bits(pipe_info_t){esr_sc_ram_delay_2}} & pipe_info_da_d2_q) | ({$bits(pipe_info_t){esr_sc_ram_delay_3}} & pipe_info_da_d3_q) | ({$bits(pipe_info_t){esr_sc_ram_delay_4}} & pipe_info_da_d4_q);
  reqq_pipe_req_t pipe_req_info_da_dN;
  assign pipe_req_info_da_dN = ({$bits(reqq_pipe_req_t){esr_sc_ram_delay_2}} & pipe_req_info_da_d2_q) | ({$bits(reqq_pipe_req_t){esr_sc_ram_delay_3}} & pipe_req_info_da_d3_q) | ({$bits(reqq_pipe_req_t){esr_sc_ram_delay_4}} & pipe_req_info_da_d4_q);


  // ════════════════════════════════════════════════════════
  // _de (data ram ecc stage)
  // ════════════════════════════════════════════════════════
  logic pipe_valid_de_q;
  pipe_info_t pipe_info_de_q;
  reqq_pipe_req_t pipe_req_info_de_q;
  logic data_ram_rd_data_valid_de_q;
  logic [DataRamDataSize-1:0] data_ram_rd_data_de_q;

  wire data_ram_rd_data_valid_da_dN = pipe_data_ram_rsp_valid_i;
  wire [DataRamDataSize-1:0] data_ram_rd_data_da_dN = pipe_data_ram_rsp_info_i.rd_data;

  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_de_q <= '0; else if (pipe_valid_de_q || pipe_valid_da_dN) pipe_valid_de_q <= pipe_valid_da_dN;
  always_ff @(posedge clk_i) if (pipe_valid_da_dN) begin pipe_info_de_q <= pipe_info_da_dN; pipe_req_info_de_q <= pipe_req_info_da_dN; end
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) data_ram_rd_data_valid_de_q <= '0; else if (data_ram_rd_data_valid_de_q || data_ram_rd_data_valid_da_dN) data_ram_rd_data_valid_de_q <= data_ram_rd_data_valid_da_dN;
  always_ff @(posedge clk_i) if (data_ram_rd_data_valid_da_dN) data_ram_rd_data_de_q <= data_ram_rd_data_da_dN;

  // IdxRead muxing
  wire idx_read_valid_de = pipe_valid_de_q && (pipe_req_info_de_q.opcode == IdxRead);
  assign dbg_rd_info_o.data_and_ecc_valid = idx_read_valid_de && esr_dbg_wr_info_i.data_en;
  assign dbg_rd_info_o.data_and_ecc       = data_ram_rd_data_de_q;

  // bist — data ram rsp
  localparam int unsigned MBD_QWEN_DELAY = 5;
  wire [LineQwIdSize-1:0] mbd_qwen_id_xx_d0 = mbd_qwen_id_xx;
  logic [LineQwIdSize-1:0] mbd_qwen_id_xx_d1_q, mbd_qwen_id_xx_d2_q, mbd_qwen_id_xx_d3_q, mbd_qwen_id_xx_d4_q, mbd_qwen_id_xx_d5_q;
  logic [MBD_QWEN_DELAY-1:0] vec_mbd_rd_dN_q;
  wire mbd_rd_dN_enable = |{vec_mbd_rd_dN_q, mbd_rd_en_xx};
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) vec_mbd_rd_dN_q <= '0; else if (mbd_rd_dN_enable) vec_mbd_rd_dN_q <= {vec_mbd_rd_dN_q[0 +: MBD_QWEN_DELAY-1], mbd_rd_en_xx};
  always_ff @(posedge clk_i) if (mbd_rd_dN_enable) begin mbd_qwen_id_xx_d1_q <= mbd_qwen_id_xx_d0; mbd_qwen_id_xx_d2_q <= mbd_qwen_id_xx_d1_q; mbd_qwen_id_xx_d3_q <= mbd_qwen_id_xx_d2_q; mbd_qwen_id_xx_d4_q <= mbd_qwen_id_xx_d3_q; mbd_qwen_id_xx_d5_q <= mbd_qwen_id_xx_d4_q; end
  assign pipe_bist_rsp_info_o.mbd_rdata = data_ram_rd_data_de_q[mbd_qwen_id_xx_d5_q*(DataRamDataSize/LineQwSize) +: (DataRamDataSize/LineQwSize)];

  // ecc correction — data
  logic [LineDataSize-1:0] data_ram_rd_data_corr_de;
  logic [LineDwSize-1:0] data_ram_sbe_de;
  logic [LineDwSize-1:0] data_ram_dbe_de;
  prim_ecc_dec #(.DataWidth(DwDataSize)) data_ram_ecc_corr [LineDwSize-1:0] (
    .data_and_ecc_i(data_ram_rd_data_de_q), .data_o(data_ram_rd_data_corr_de), .sbe_o(data_ram_sbe_de), .dbe_o(data_ram_dbe_de)
  );


  // ════════════════════════════════════════════════════════
  // _dc (data complete)
  // ════════════════════════════════════════════════════════
  logic pipe_valid_dc_q;
  pipe_info_t pipe_info_dc_q;
  reqq_pipe_req_t pipe_req_info_dc_q;
  logic [LineDataSize-1:0] data_ram_rd_data_corr_dc_q;
  logic [LineDwSize-1:0] data_ram_sbe_dc_q;
  logic [LineDwSize-1:0] data_ram_dbe_dc_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_valid_dc_q <= '0; else if (pipe_valid_dc_q || pipe_valid_de_q) pipe_valid_dc_q <= pipe_valid_de_q;
  always_ff @(posedge clk_i) if (pipe_valid_de_q) begin pipe_info_dc_q <= pipe_info_de_q; pipe_req_info_dc_q <= pipe_req_info_de_q; end
  always_ff @(posedge clk_i) if (data_ram_rd_data_valid_de_q) begin data_ram_rd_data_corr_dc_q <= data_ram_rd_data_corr_de; data_ram_sbe_dc_q <= data_ram_sbe_de & {LineDwSize{!idx_read_valid_de}}; data_ram_dbe_dc_q <= data_ram_dbe_de & {LineDwSize{!idx_read_valid_de}}; end

  // data rsp final results (assigned below)
  logic [LineDataSize-1:0] data_rsp_data_dc;
  logic [QWEN_SIZE-1:0]   data_rsp_qwen_dc;

  // sbe/dbe valid per qw
  logic [2*QWEN_SIZE-1:0] data_rsp_sbe_dbe_valid_dc;
  logic [2*QWEN_SIZE-1:0] data_rsp_use_data_ram_rd_data_dc;
  genvar gen_it;
  for (gen_it = 0; gen_it < LineQwSize; gen_it++) begin : g_drsp_sbe_dbe
    assign data_rsp_sbe_dbe_valid_dc[gen_it*2 +: 2] =
      (data_rsp_qwen_dc == '0) ? '1 : {2{data_rsp_qwen_dc[gen_it]}};
  end

  // data_rsp
  data_rsp_t pipe_data_rsp_info_dc;
  wire pipe_data_rsp_valid_dc              = pipe_valid_dc_q && pipe_info_dc_q.data_rsp_req;
  assign pipe_data_rsp_info_dc.trans_id    = pipe_req_info_dc_q.trans_id;
  assign pipe_data_rsp_info_dc.reqq_id     = pipe_req_info_dc_q.reqq_id;
  assign pipe_data_rsp_info_dc.rsp_type    = pipe_info_dc_q.victim ? VictimData : NormalRsp;
  assign pipe_data_rsp_info_dc.tag_sbe     = pipe_info_dc_q.tag_sbe;
  assign pipe_data_rsp_info_dc.data        = data_rsp_data_dc;
  assign pipe_data_rsp_info_dc.qwen        = data_rsp_qwen_dc;
  assign pipe_data_rsp_info_dc.data_index  = {pipe_info_dc_q.pipe_set, pipe_info_dc_q.tag_ram_hit_way};
  assign pipe_data_rsp_info_dc.data_ram_sbe = data_ram_sbe_dc_q & data_rsp_sbe_dbe_valid_dc & data_rsp_use_data_ram_rd_data_dc;
  assign pipe_data_rsp_info_dc.data_ram_dbe = data_ram_dbe_dc_q & data_rsp_sbe_dbe_valid_dc & data_rsp_use_data_ram_rd_data_dc;

  // ── sbe/dbe counters ──
  localparam int unsigned SBE_COUNT_SIZE = SbeCountSize;
  localparam int unsigned DBE_COUNT_SIZE = DbeCountSize;
  logic [SBE_COUNT_SIZE-1:0] tag_state_ram_sbe_count_q, tag_ram_sbe_count_q, data_ram_sbe_count_q;
  logic [DBE_COUNT_SIZE-1:0] tag_state_ram_dbe_count_q, tag_ram_dbe_count_q, data_ram_dbe_count_q;

  assign pipe_sbe_dbe_esr_status_o.tag_state_ram_sbe_count = tag_state_ram_sbe_count_q;
  assign pipe_sbe_dbe_esr_status_o.tag_state_ram_dbe_count = tag_state_ram_dbe_count_q;
  assign pipe_sbe_dbe_esr_status_o.tag_ram_sbe_count       = tag_ram_sbe_count_q;
  assign pipe_sbe_dbe_esr_status_o.tag_ram_dbe_count       = tag_ram_dbe_count_q;
  assign pipe_sbe_dbe_esr_status_o.data_ram_sbe_count      = data_ram_sbe_count_q;
  assign pipe_sbe_dbe_esr_status_o.data_ram_dbe_count      = data_ram_dbe_count_q;
  // dataq and ben RAM ECC counts are combined at bank level, not in pipe
  assign pipe_sbe_dbe_esr_status_o.dataq_ram_sbe_count     = '0;
  assign pipe_sbe_dbe_esr_status_o.dataq_ram_dbe_count     = '0;
  assign pipe_sbe_dbe_esr_status_o.ben_ram_sbe_count       = '0;
  assign pipe_sbe_dbe_esr_status_o.ben_ram_dbe_count       = '0;

  sbe_dbe_esr_status_t wr_sbe_dbe_data;
  assign wr_sbe_dbe_data = sbe_dbe_esr_status_t'(esr_ctl_i.wr_data[0 +: $bits(sbe_dbe_esr_status_t)]);
  wire clr_tag_state_ram_sbe_count = esr_ctl_i.wr_en_sbe_dbe_counts && ((wr_sbe_dbe_data.tag_state_ram_sbe_count == '1) || (wr_sbe_dbe_data.tag_state_ram_sbe_count == tag_state_ram_sbe_count_q));
  wire clr_tag_state_ram_dbe_count = esr_ctl_i.wr_en_sbe_dbe_counts && ((wr_sbe_dbe_data.tag_state_ram_dbe_count == '1) || (wr_sbe_dbe_data.tag_state_ram_dbe_count == tag_state_ram_dbe_count_q));
  wire clr_tag_ram_sbe_count       = esr_ctl_i.wr_en_sbe_dbe_counts && ((wr_sbe_dbe_data.tag_ram_sbe_count == '1) || (wr_sbe_dbe_data.tag_ram_sbe_count == tag_ram_sbe_count_q));
  wire clr_tag_ram_dbe_count       = esr_ctl_i.wr_en_sbe_dbe_counts && ((wr_sbe_dbe_data.tag_ram_dbe_count == '1) || (wr_sbe_dbe_data.tag_ram_dbe_count == tag_ram_dbe_count_q));
  wire clr_data_ram_sbe_count      = esr_ctl_i.wr_en_sbe_dbe_counts && ((wr_sbe_dbe_data.data_ram_sbe_count == '1) || (wr_sbe_dbe_data.data_ram_sbe_count == data_ram_sbe_count_q));
  wire clr_data_ram_dbe_count      = esr_ctl_i.wr_en_sbe_dbe_counts && ((wr_sbe_dbe_data.data_ram_dbe_count == '1) || (wr_sbe_dbe_data.data_ram_dbe_count == data_ram_dbe_count_q));

  wire tag_state_ram_sbe_overflow = &tag_state_ram_sbe_count_q;
  wire tag_state_ram_dbe_overflow = &tag_state_ram_dbe_count_q;
  wire tag_ram_sbe_overflow       = &tag_ram_sbe_count_q;
  wire tag_ram_dbe_overflow       = &tag_ram_dbe_count_q;
  wire data_ram_sbe_overflow      = &data_ram_sbe_count_q;
  wire data_ram_dbe_overflow      = &data_ram_dbe_count_q;
  assign pipe_sbe_dbe_overflow_o.tag_state_ram_sbe_overflow = tag_state_ram_sbe_overflow;
  assign pipe_sbe_dbe_overflow_o.tag_state_ram_dbe_overflow = tag_state_ram_dbe_overflow;
  assign pipe_sbe_dbe_overflow_o.tag_ram_sbe_overflow       = tag_ram_sbe_overflow;
  assign pipe_sbe_dbe_overflow_o.tag_ram_dbe_overflow       = tag_ram_dbe_overflow;
  assign pipe_sbe_dbe_overflow_o.data_ram_sbe_overflow      = data_ram_sbe_overflow;
  assign pipe_sbe_dbe_overflow_o.data_ram_dbe_overflow      = data_ram_dbe_overflow;

  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) tag_state_ram_sbe_count_q <= '0; else if (clr_tag_state_ram_sbe_count || (pipe_valid_tc_q && !tag_state_ram_sbe_overflow && pipe_tag_rsp_info_tc.tag_state_ram_sbe)) tag_state_ram_sbe_count_q <= clr_tag_state_ram_sbe_count ? '0 : (tag_state_ram_sbe_count_q + 1'b1);
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) tag_state_ram_dbe_count_q <= '0; else if (clr_tag_state_ram_dbe_count || (pipe_valid_tc_q && !tag_state_ram_dbe_overflow && pipe_tag_rsp_info_tc.tag_state_ram_dbe)) tag_state_ram_dbe_count_q <= clr_tag_state_ram_dbe_count ? '0 : (tag_state_ram_dbe_count_q + 1'b1);
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) tag_ram_sbe_count_q <= '0; else if (clr_tag_ram_sbe_count || (pipe_valid_tc_q && !tag_ram_sbe_overflow && (|pipe_tag_rsp_info_tc.tag_ram_sbe))) tag_ram_sbe_count_q <= clr_tag_ram_sbe_count ? '0 : (tag_ram_sbe_count_q + 1'b1);
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) tag_ram_dbe_count_q <= '0; else if (clr_tag_ram_dbe_count || (pipe_valid_tc_q && !tag_ram_dbe_overflow && (|pipe_tag_rsp_info_tc.tag_ram_dbe))) tag_ram_dbe_count_q <= clr_tag_ram_dbe_count ? '0 : (tag_ram_dbe_count_q + 1'b1);
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) data_ram_sbe_count_q <= '0; else if (clr_data_ram_sbe_count || (pipe_data_rsp_valid_dc && !data_ram_sbe_overflow && (|pipe_data_rsp_info_dc.data_ram_sbe))) data_ram_sbe_count_q <= clr_data_ram_sbe_count ? '0 : (data_ram_sbe_count_q + 1'b1);
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) data_ram_dbe_count_q <= '0; else if (clr_data_ram_dbe_count || (pipe_data_rsp_valid_dc && !data_ram_dbe_overflow && (|pipe_data_rsp_info_dc.data_ram_dbe))) data_ram_dbe_count_q <= clr_data_ram_dbe_count ? '0 : (data_ram_dbe_count_q + 1'b1);


  // data_rsp
  assign pipe_data_rsp_valid_o = pipe_data_rsp_valid_dc;
  assign pipe_data_rsp_info_o  = pipe_data_rsp_info_dc & {$bits(pipe_data_rsp_info_dc){pipe_data_rsp_valid_dc}};

  // data_rsp_pre
  data_rsp_pre_t pipe_data_rsp_pre_info_de;
  wire pipe_data_rsp_valid_de          = pipe_valid_de_q && pipe_info_de_q.data_rsp_req;
  assign pipe_data_rsp_pre_info_de.trans_id = pipe_req_info_de_q.trans_id;
  assign pipe_data_rsp_pre_info_de.reqq_id  = pipe_req_info_de_q.reqq_id;
  assign pipe_data_rsp_pre_info_de.rsp_type = pipe_info_de_q.victim ? VictimData : NormalRsp;
  assign pipe_data_rsp_pre_valid_o = pipe_data_rsp_valid_de;
  assign pipe_data_rsp_pre_info_o  = pipe_data_rsp_pre_info_de & {$bits(pipe_data_rsp_pre_info_de){pipe_data_rsp_valid_de}};

  wire [PaSize-1:0] pipe_line_address_dc = get_line_addr(pipe_req_info_dc_q.address);

  // pipe_rbuf_data_req
  rbuf_data_req_t pipe_rbuf_data_req_info_dc;
  wire pipe_rbuf_data_req_valid_dc          = pipe_valid_dc_q && pipe_info_dc_q.data_rsp_req;
  assign pipe_rbuf_data_req_info_dc.trans_id = pipe_req_info_dc_q.trans_id;
  assign pipe_rbuf_data_req_info_dc.reqq_id  = pipe_req_info_dc_q.reqq_id;
  assign pipe_rbuf_data_req_info_dc.scp      = (pipe_req_info_dc_q.opcode == ScpRead);
  assign pipe_rbuf_data_req_info_dc.opcode   = pipe_req_info_dc_q.opcode;
  assign pipe_rbuf_data_req_info_dc.address  = pipe_line_address_dc;
  assign pipe_rbuf_data_req_info_dc.data     = data_rsp_data_dc;
  assign pipe_rbuf_data_req_valid_o = pipe_rbuf_data_req_valid_dc;
  assign pipe_rbuf_data_req_info_o  = pipe_rbuf_data_req_info_dc & {$bits(pipe_rbuf_data_req_info_dc){pipe_rbuf_data_req_valid_dc}};


  // ════════════════════════════════════════════════════════
  // dataq read interface
  // ════════════════════════════════════════════════════════
  dataq_rd_req_t pipe_dataq_rd_req_info_dap;
  wire pipe_dataq_rd_req_valid_dap         = pipe_valid_dap & pipe_info_dap.dataq_rd_req_en;
  assign pipe_dataq_rd_req_info_dap.trans_id = pipe_req_info_dap.trans_id;
  assign pipe_dataq_rd_req_info_dap.reqq_id  = pipe_req_info_dap.reqq_id;
  assign pipe_dataq_rd_req_info_dap.partial  = pipe_info_dap.dataq_rd_req_partial;
  assign pipe_dataq_rd_req_info_dap.qwen     = pipe_req_info_dap.qwen;

  dataq_rd_req_t pipe_dataq_rd_req_info_da;
  wire pipe_dataq_rd_req_valid_da          = pipe_valid_da_q & pipe_info_da_q.dataq_rd_req_en;
  assign pipe_dataq_rd_req_info_da.trans_id = pipe_req_info_da_q.trans_id;
  assign pipe_dataq_rd_req_info_da.reqq_id  = pipe_req_info_da_q.reqq_id;
  assign pipe_dataq_rd_req_info_da.partial  = pipe_info_da_q.dataq_rd_req_partial;
  assign pipe_dataq_rd_req_info_da.qwen     = pipe_req_info_da_q.qwen;

  dataq_rd_req_t pipe_dataq_rd_req_info_da_d1;
  wire pipe_dataq_rd_req_valid_da_d1       = pipe_valid_da_d1 & pipe_info_da_d1_q.dataq_rd_req_en;
  assign pipe_dataq_rd_req_info_da_d1.trans_id = pipe_req_info_da_d1_q.trans_id;
  assign pipe_dataq_rd_req_info_da_d1.reqq_id  = pipe_req_info_da_d1_q.reqq_id;
  assign pipe_dataq_rd_req_info_da_d1.partial  = pipe_info_da_d1_q.dataq_rd_req_partial;
  assign pipe_dataq_rd_req_info_da_d1.qwen     = pipe_req_info_da_d1_q.qwen;

  wire pipe_dataq_rd_req_valid_m1 =
    (esr_sc_ram_delay_2 && pipe_dataq_rd_req_valid_dap) ||
    (esr_sc_ram_delay_3 && pipe_dataq_rd_req_valid_da) ||
    (esr_sc_ram_delay_4 && pipe_dataq_rd_req_valid_da_d1);

  dataq_rd_req_t pipe_dataq_rd_req_info_m1;
  assign pipe_dataq_rd_req_info_m1 =
    ({$bits(dataq_rd_req_t){esr_sc_ram_delay_2 & pipe_dataq_rd_req_valid_dap}}   & pipe_dataq_rd_req_info_dap) |
    ({$bits(dataq_rd_req_t){esr_sc_ram_delay_3 & pipe_dataq_rd_req_valid_da}}    & pipe_dataq_rd_req_info_da) |
    ({$bits(dataq_rd_req_t){esr_sc_ram_delay_4 & pipe_dataq_rd_req_valid_da_d1}} & pipe_dataq_rd_req_info_da_d1);

  dataq_rd_req_t int_pipe_dataq_rd_req_info_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) pipe_dataq_rd_req_valid_o <= '0; else if (pipe_dataq_rd_req_valid_o | pipe_dataq_rd_req_valid_m1) pipe_dataq_rd_req_valid_o <= pipe_dataq_rd_req_valid_m1;
  always_ff @(posedge clk_i) if (pipe_dataq_rd_req_valid_o | pipe_dataq_rd_req_valid_m1) int_pipe_dataq_rd_req_info_q <= pipe_dataq_rd_req_info_m1;
  assign pipe_dataq_rd_req_info_o = {$bits(dataq_rd_req_t){pipe_dataq_rd_req_valid_o}} & int_pipe_dataq_rd_req_info_q;

  // dataq rsp delay
  localparam int unsigned DATAQ_RD_DLY = 3;
  wire dataq_rd_req_valid_d0 = pipe_dataq_rd_req_valid_o;
  logic [DATAQ_RD_DLY-1:0] vec_dataq_rd_req_valid_dN_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) vec_dataq_rd_req_valid_dN_q <= '0; else if (|{vec_dataq_rd_req_valid_dN_q, dataq_rd_req_valid_d0}) vec_dataq_rd_req_valid_dN_q <= {vec_dataq_rd_req_valid_dN_q[0 +: DATAQ_RD_DLY-1], dataq_rd_req_valid_d0};

  dataq_rd_rsp_t pipe_dataq_rd_rsp_info_de;
  dataq_rd_rsp_t pipe_dataq_rd_rsp_info_dc_q;
  wire pipe_dataq_rd_rsp_valid_de = vec_dataq_rd_req_valid_dN_q[DATAQ_RD_DLY-1];
  assign pipe_dataq_rd_rsp_info_de = pipe_dataq_rd_rsp_info_i;
  always_ff @(posedge clk_i) if (pipe_dataq_rd_rsp_valid_de) pipe_dataq_rd_rsp_info_dc_q <= pipe_dataq_rd_rsp_info_de;
  wire [LineDataSize-1:0] dataq_rd_data_dc     = pipe_dataq_rd_rsp_info_dc_q.data;
  wire [LineByteSize-1:0] dataq_rd_data_ben_dc = pipe_dataq_rd_rsp_info_dc_q.ben;


  // ════════════════════════════════════════════════════════
  // data_replace_dc
  // ════════════════════════════════════════════════════════
  wire  pipe_valid_dc_d0 = pipe_valid_dc_q;
  logic pipe_valid_dc_d1_q, pipe_valid_dc_d2_q, pipe_valid_dc_d3_q, pipe_valid_dc_d4_q, pipe_valid_dc_d5_q, pipe_valid_dc_d6_q;
  wire any_pipe_valid_dc_d0_dc_d6 = pipe_valid_dc_d0 || pipe_valid_dc_d1_q || pipe_valid_dc_d2_q || pipe_valid_dc_d3_q || pipe_valid_dc_d4_q || pipe_valid_dc_d5_q || pipe_valid_dc_d6_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) begin pipe_valid_dc_d1_q <= '0; pipe_valid_dc_d2_q <= '0; pipe_valid_dc_d3_q <= '0; pipe_valid_dc_d4_q <= '0; pipe_valid_dc_d5_q <= '0; pipe_valid_dc_d6_q <= '0; end else if (any_pipe_valid_dc_d0_dc_d6) begin pipe_valid_dc_d1_q <= pipe_valid_dc_d0; pipe_valid_dc_d2_q <= pipe_valid_dc_d1_q; pipe_valid_dc_d3_q <= pipe_valid_dc_d2_q; pipe_valid_dc_d4_q <= pipe_valid_dc_d3_q; pipe_valid_dc_d5_q <= pipe_valid_dc_d4_q; pipe_valid_dc_d6_q <= pipe_valid_dc_d5_q; end

  wire  pipe_data_ram_wr_en_dc_d0 = pipe_info_dc_q.data_wr_req || pipe_info_dc_q.data_wr_zero;
  logic pipe_data_ram_wr_en_dc_d1_q, pipe_data_ram_wr_en_dc_d2_q, pipe_data_ram_wr_en_dc_d3_q, pipe_data_ram_wr_en_dc_d4_q, pipe_data_ram_wr_en_dc_d5_q, pipe_data_ram_wr_en_dc_d6_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d0) pipe_data_ram_wr_en_dc_d1_q <= pipe_data_ram_wr_en_dc_d0;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d1_q) pipe_data_ram_wr_en_dc_d2_q <= pipe_data_ram_wr_en_dc_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d2_q) pipe_data_ram_wr_en_dc_d3_q <= pipe_data_ram_wr_en_dc_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d3_q) pipe_data_ram_wr_en_dc_d4_q <= pipe_data_ram_wr_en_dc_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d4_q) pipe_data_ram_wr_en_dc_d5_q <= pipe_data_ram_wr_en_dc_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d5_q) pipe_data_ram_wr_en_dc_d6_q <= pipe_data_ram_wr_en_dc_d5_q;

  pipe_info_t pipe_info_dc_d0; reqq_pipe_req_t pipe_req_info_dc_d0;
  assign pipe_info_dc_d0 = pipe_info_dc_q; assign pipe_req_info_dc_d0 = pipe_req_info_dc_q;
  pipe_info_t pipe_info_dc_d1_q; reqq_pipe_req_t pipe_req_info_dc_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d0) begin pipe_info_dc_d1_q <= pipe_info_dc_d0; pipe_req_info_dc_d1_q <= pipe_req_info_dc_d0; end
  pipe_info_t pipe_info_dc_d2_q; reqq_pipe_req_t pipe_req_info_dc_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d1_q) begin pipe_info_dc_d2_q <= pipe_info_dc_d1_q; pipe_req_info_dc_d2_q <= pipe_req_info_dc_d1_q; end
  pipe_info_t pipe_info_dc_d3_q; reqq_pipe_req_t pipe_req_info_dc_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d2_q) begin pipe_info_dc_d3_q <= pipe_info_dc_d2_q; pipe_req_info_dc_d3_q <= pipe_req_info_dc_d2_q; end
  pipe_info_t pipe_info_dc_d4_q; reqq_pipe_req_t pipe_req_info_dc_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d3_q) begin pipe_info_dc_d4_q <= pipe_info_dc_d3_q; pipe_req_info_dc_d4_q <= pipe_req_info_dc_d3_q; end
  pipe_info_t pipe_info_dc_d5_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d4_q) pipe_info_dc_d5_q <= pipe_info_dc_d4_q;
  pipe_info_t pipe_info_dc_d6_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d5_q) pipe_info_dc_d6_q <= pipe_info_dc_d5_q;


  // ── write data ──
  wire                       bubble_slot_wr_en_dc   = pipe_valid_dc_q & pipe_info_dc_q.data_wr_req;
  wire [DataRamAddrSize-1:0] bubble_slot_wr_addr_dc = {pipe_info_dc_q.pipe_set, pipe_info_dc_q.tag_ram_hit_way};

  logic [LineDataSize-1:0] int_dataq_rd_data_dc;
  for (gen_it = 0; gen_it < LineQwSize; gen_it++) begin : g_wr_data_sel
    assign int_dataq_rd_data_dc[gen_it*QwDataSize +: QwDataSize] =
      pipe_info_dc_d0.dataq_sel_qwen[gen_it] ? dataq_rd_data_dc[gen_it*QwDataSize +: QwDataSize] : '0;
  end

  logic [LineDataSize-1:0] int_newest_rd_data_dc;

  wire [LineDataSize-1:0] bubble_slot_wr_data_dc =
    (pipe_info_dc_q.idx_write && !esr_dbg_wr_info_i.ecc_wr_en) ? esr_dbg_wr_info_i.data :
    pipe_info_dc_q.merge_rd_data_in_wr_req                     ? int_newest_rd_data_dc :
    pipe_info_dc_q.data_wr_zero                                ? '0 :
                                                                  int_dataq_rd_data_dc;

  // dc_d2 for ram_delay_3
  wire                       bubble_slot_wr_en_dc_d2   = pipe_valid_dc_d2_q & pipe_info_dc_d2_q.data_wr_req;
  wire [DataRamAddrSize-1:0] bubble_slot_wr_addr_dc_d2 = {pipe_info_dc_d2_q.pipe_set, pipe_info_dc_d2_q.tag_ram_hit_way};
  logic [LineDataSize-1:0]   bubble_slot_wr_data_dc_d2_q;

  assign data_ram_wr_en_dc             = esr_sc_ram_delay_3 ? bubble_slot_wr_en_dc_d2         : bubble_slot_wr_en_dc;
  assign data_ram_wr_sub_bank_dc       = esr_sc_ram_delay_3 ? pipe_info_dc_d2_q.pipe_sub_bank : pipe_info_dc_q.pipe_sub_bank;
  assign data_ram_wr_trans_id_dc       = esr_sc_ram_delay_3 ? pipe_req_info_dc_d2_q.trans_id  : pipe_req_info_dc_q.trans_id;
  assign data_ram_wr_reqq_id_dc        = esr_sc_ram_delay_3 ? pipe_req_info_dc_d2_q.reqq_id   : pipe_req_info_dc_q.reqq_id;
  assign data_ram_wr_addr_dc           = esr_sc_ram_delay_3 ? bubble_slot_wr_addr_dc_d2       : bubble_slot_wr_addr_dc;
  assign data_ram_wr_qwen_dc           = esr_sc_ram_delay_3 ? pipe_info_dc_d2_q.data_wr_qwen  : pipe_info_dc_q.data_wr_qwen;
  assign data_ram_wr_xcheck_disable_dc = esr_sc_ram_delay_3 ? (pipe_req_info_dc_d2_q.opcode == ScpScrub) : (pipe_req_info_dc_q.opcode == ScpScrub);

  // ecc generation — data write
  wire [LineDataSize-1:0] data_ram_wr_data_pre_ecc_dc = esr_sc_ram_delay_3 ? bubble_slot_wr_data_dc_d2_q : bubble_slot_wr_data_dc;
  logic [DataRamDataSize-1:0] data_ram_wr_data_ecc_dc;
  prim_ecc_enc #(.DataWidth(DwDataSize)) data_ram_ecc_gen [LineDwSize-1:0] (
    .data_i(data_ram_wr_data_pre_ecc_dc), .data_and_ecc_o(data_ram_wr_data_ecc_dc)
  );
  wire [DataRamDataSize-1:0] data_ram_wr_data_bit_flip_dc;
  shirecache_pipe_sbe_insert #(.DataWidth(DwDataSize), .EccWidth(DwEccSize), .InsertEnable(1'b0)) data_ram_sbe_insert [LineDwSize-1:0] (
    .clk_i(clk_i), .rst_ni(rst_ni), .wr_en_i(data_ram_wr_en_dc), .data_ecc_bit_flip_o(data_ram_wr_data_bit_flip_dc)
  );

  wire idx_write_post_ecc_mux_dc = pipe_info_dc_q.idx_write && esr_dbg_wr_info_i.ecc_wr_en;
  assign data_ram_wr_data_dc =
    idx_write_post_ecc_mux_dc ? esr_dbg_wr_info_i.data_and_ecc : (data_ram_wr_data_ecc_dc ^ data_ram_wr_data_bit_flip_dc);


  // data replace logic
  wire [LineDataSize-1:0] bubble_slot_wr_data_dc_d0 = bubble_slot_wr_data_dc;
  logic [LineDataSize-1:0] bubble_slot_wr_data_dc_d1_q;
  // bubble_slot_wr_data_dc_d2_q defined above
  logic [LineDataSize-1:0] bubble_slot_wr_data_dc_d3_q, bubble_slot_wr_data_dc_d4_q, bubble_slot_wr_data_dc_d5_q, bubble_slot_wr_data_dc_d6_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d0 && pipe_data_ram_wr_en_dc_d0) bubble_slot_wr_data_dc_d1_q <= bubble_slot_wr_data_dc_d0;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d1_q && pipe_data_ram_wr_en_dc_d1_q) bubble_slot_wr_data_dc_d2_q <= bubble_slot_wr_data_dc_d1_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d2_q && pipe_data_ram_wr_en_dc_d2_q) bubble_slot_wr_data_dc_d3_q <= bubble_slot_wr_data_dc_d2_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d3_q && pipe_data_ram_wr_en_dc_d3_q) bubble_slot_wr_data_dc_d4_q <= bubble_slot_wr_data_dc_d3_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d4_q && pipe_data_ram_wr_en_dc_d4_q) bubble_slot_wr_data_dc_d5_q <= bubble_slot_wr_data_dc_d4_q;
  always_ff @(posedge clk_i) if (pipe_valid_dc_d5_q && pipe_data_ram_wr_en_dc_d5_q) bubble_slot_wr_data_dc_d6_q <= bubble_slot_wr_data_dc_d5_q;

  // match and replace
  wire check_for_set_way_match_dc = pipe_valid_dc_q && (pipe_info_dc_q.data_rsp_req || pipe_info_dc_q.merge_rd_data_in_wr_req);
  wire set_way_match_dc_dc_d0 = check_for_set_way_match_dc && pipe_info_dc_q.merge_wr_data_in_rd_rsp;
  wire set_way_match_dc_dc_d2 = check_for_set_way_match_dc && pipe_valid_dc_d2_q && pipe_data_ram_wr_en_dc_d2_q && (pipe_info_dc_d0.pipe_sub_bank == pipe_info_dc_d2_q.pipe_sub_bank) && (pipe_info_dc_d0.pipe_set == pipe_info_dc_d2_q.pipe_set) && (pipe_info_dc_d0.tag_ram_hit_way == pipe_info_dc_d2_q.tag_ram_hit_way);
  wire set_way_match_dc_dc_d3 = check_for_set_way_match_dc && pipe_valid_dc_d3_q && pipe_data_ram_wr_en_dc_d3_q && (pipe_info_dc_d0.pipe_sub_bank == pipe_info_dc_d3_q.pipe_sub_bank) && (pipe_info_dc_d0.pipe_set == pipe_info_dc_d3_q.pipe_set) && (pipe_info_dc_d0.tag_ram_hit_way == pipe_info_dc_d3_q.tag_ram_hit_way);
  wire set_way_match_dc_dc_d4 = check_for_set_way_match_dc && pipe_valid_dc_d4_q && pipe_data_ram_wr_en_dc_d4_q && (pipe_info_dc_d0.pipe_sub_bank == pipe_info_dc_d4_q.pipe_sub_bank) && (pipe_info_dc_d0.pipe_set == pipe_info_dc_d4_q.pipe_set) && (pipe_info_dc_d0.tag_ram_hit_way == pipe_info_dc_d4_q.tag_ram_hit_way);
  wire set_way_match_dc_dc_d6 = check_for_set_way_match_dc && pipe_valid_dc_d6_q && pipe_data_ram_wr_en_dc_d6_q && (pipe_info_dc_d0.pipe_sub_bank == pipe_info_dc_d6_q.pipe_sub_bank) && (pipe_info_dc_d0.pipe_set == pipe_info_dc_d6_q.pipe_set) && (pipe_info_dc_d0.tag_ram_hit_way == pipe_info_dc_d6_q.tag_ram_hit_way);


  // mux data_rsp_data_dc
  logic [LineDataSize-1:0] int_data_rsp_data_dc;
  for (gen_it = 0; gen_it < LineQwSize; gen_it++) begin : g_drsp
    assign int_data_rsp_data_dc[gen_it*QwDataSize +: QwDataSize] =
      (set_way_match_dc_dc_d0 & pipe_info_dc_d0.data_wr_qwen[gen_it]) ? bubble_slot_wr_data_dc_d0[gen_it*QwDataSize +: QwDataSize] :
                                                                          int_newest_rd_data_dc[gen_it*QwDataSize +: QwDataSize];
    assign int_newest_rd_data_dc[gen_it*QwDataSize +: QwDataSize] =
      (set_way_match_dc_dc_d2 & pipe_info_dc_d2_q.data_wr_qwen[gen_it]) ? bubble_slot_wr_data_dc_d2_q[gen_it*QwDataSize +: QwDataSize] :
      (set_way_match_dc_dc_d3 & pipe_info_dc_d3_q.data_wr_qwen[gen_it]) ? bubble_slot_wr_data_dc_d3_q[gen_it*QwDataSize +: QwDataSize] :
      (set_way_match_dc_dc_d4 & pipe_info_dc_d4_q.data_wr_qwen[gen_it]) ? bubble_slot_wr_data_dc_d4_q[gen_it*QwDataSize +: QwDataSize] :
      (set_way_match_dc_dc_d6 & pipe_info_dc_d6_q.data_wr_qwen[gen_it]) ? bubble_slot_wr_data_dc_d6_q[gen_it*QwDataSize +: QwDataSize] :
      (pipe_info_dc_q.data_rsp_zero)                                     ? {QwDataSize{1'b0}} :
                                                                            data_ram_rd_data_corr_dc_q[gen_it*QwDataSize +: QwDataSize];
    assign data_rsp_use_data_ram_rd_data_dc[gen_it*2 +: 2] =
      (set_way_match_dc_dc_d0 && !pipe_info_dc_d0.merge_rd_data_in_wr_req && pipe_info_dc_d0.data_wr_qwen[gen_it]) ? '0 :
      (set_way_match_dc_dc_d2 && !pipe_info_dc_d2_q.merge_rd_data_in_wr_req && pipe_info_dc_d2_q.data_wr_qwen[gen_it]) ? '0 :
      (set_way_match_dc_dc_d3 && !pipe_info_dc_d3_q.merge_rd_data_in_wr_req && pipe_info_dc_d3_q.data_wr_qwen[gen_it]) ? '0 :
      (set_way_match_dc_dc_d4 && !pipe_info_dc_d4_q.merge_rd_data_in_wr_req && pipe_info_dc_d4_q.data_wr_qwen[gen_it]) ? '0 :
      (set_way_match_dc_dc_d6 && !pipe_info_dc_d6_q.merge_rd_data_in_wr_req && pipe_info_dc_d6_q.data_wr_qwen[gen_it]) ? '0 :
      (pipe_info_dc_q.data_rsp_zero)                                                                                   ? '0 :
                                                                                                                          '1;
  end

  // atomic
  wire atomic_operation_dc = !pipe_info_dc_q.victim && (pipe_req_info_dc_q.opcode inside {L2Atomic, L3Atomic, ScpAtomic});
  logic [LineDataSize-1:0] atomic_data_rsp_data_dc;
  assign data_rsp_data_dc = atomic_operation_dc ? atomic_data_rsp_data_dc : int_data_rsp_data_dc;
  assign data_rsp_qwen_dc = pipe_info_dc_q.data_rsp_qwen;


  // ── atomic block ──
  logic                       atomic_result_wr_en_dc_d2;
  logic [DataRamAddrSize-1:0] atomic_result_wr_addr_dc_d2;
  logic [LineDataSize-1:0]    atomic_result_wr_data_dc_d2;
  logic [QWEN_SIZE-1:0]       atomic_result_wr_qwen_dc_d2;
  shirecache_pipe_atomic atomic_inst (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .pipe_valid_i           (pipe_valid_dc_q),
    .pipe_info_i            (pipe_info_dc_q),
    .pipe_req_info_i        (pipe_req_info_dc_q),
    .pipe_data_i            (int_data_rsp_data_dc),
    .atomic_operands_i      (dataq_rd_data_dc),
    .partial_wr_data_ben_i  (dataq_rd_data_ben_dc),
    .atomic_data_rsp_data_o (atomic_data_rsp_data_dc),
    .atomic_result_wr_en_o  (atomic_result_wr_en_dc_d2),
    .atomic_result_wr_addr_o(atomic_result_wr_addr_dc_d2),
    .atomic_result_wr_data_o(atomic_result_wr_data_dc_d2),
    .atomic_result_wr_qwen_o(atomic_result_wr_qwen_dc_d2)
  );

  logic                       atomic_result_wr_en_dc_d3_q;
  logic [DataRamAddrSize-1:0] atomic_result_wr_addr_dc_d3_q;
  logic [LineDataSize-1:0]    atomic_result_wr_data_dc_d3_q;
  logic [QWEN_SIZE-1:0]       atomic_result_wr_qwen_dc_d3_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) atomic_result_wr_en_dc_d3_q <= '0; else if (atomic_result_wr_en_dc_d3_q || (atomic_result_wr_en_dc_d2 && esr_sc_ram_delay_3)) atomic_result_wr_en_dc_d3_q <= atomic_result_wr_en_dc_d2;
  always_ff @(posedge clk_i) if (atomic_result_wr_en_dc_d2 && esr_sc_ram_delay_3) begin atomic_result_wr_addr_dc_d3_q <= atomic_result_wr_addr_dc_d2; atomic_result_wr_data_dc_d3_q <= atomic_result_wr_data_dc_d2; atomic_result_wr_qwen_dc_d3_q <= atomic_result_wr_qwen_dc_d2; end

  logic                       atomic_result_wr_en_dc_d4_q;
  logic [DataRamAddrSize-1:0] atomic_result_wr_addr_dc_d4_q;
  logic [LineDataSize-1:0]    atomic_result_wr_data_dc_d4_q;
  logic [QWEN_SIZE-1:0]       atomic_result_wr_qwen_dc_d4_q;
  always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) atomic_result_wr_en_dc_d4_q <= '0; else if (atomic_result_wr_en_dc_d4_q || atomic_result_wr_en_dc_d3_q) atomic_result_wr_en_dc_d4_q <= atomic_result_wr_en_dc_d3_q;
  always_ff @(posedge clk_i) if (atomic_result_wr_en_dc_d3_q) begin atomic_result_wr_addr_dc_d4_q <= atomic_result_wr_addr_dc_d3_q; atomic_result_wr_data_dc_d4_q <= atomic_result_wr_data_dc_d3_q; atomic_result_wr_qwen_dc_d4_q <= atomic_result_wr_qwen_dc_d3_q; end

  assign pipe_info_dc_dAMO             = esr_sc_ram_delay_3 ? pipe_info_dc_d4_q             : pipe_info_dc_d2_q;
  assign pipe_req_info_dc_dAMO         = esr_sc_ram_delay_3 ? pipe_req_info_dc_d4_q         : pipe_req_info_dc_d2_q;
  assign atomic_result_wr_en_dc_dAMO   = esr_sc_ram_delay_3 ? atomic_result_wr_en_dc_d4_q   : atomic_result_wr_en_dc_d2;
  assign atomic_result_wr_addr_dc_dAMO = esr_sc_ram_delay_3 ? atomic_result_wr_addr_dc_d4_q : atomic_result_wr_addr_dc_d2;
  assign atomic_result_wr_data_dc_dAMO = esr_sc_ram_delay_3 ? atomic_result_wr_data_dc_d4_q : atomic_result_wr_data_dc_d2;
  assign atomic_result_wr_qwen_dc_dAMO = esr_sc_ram_delay_3 ? atomic_result_wr_qwen_dc_d4_q : atomic_result_wr_qwen_dc_d2;


  // ════════════════════════════════════════════════════════
  // Performance monitor
  // ════════════════════════════════════════════════════════
  localparam int unsigned PERFMON_RSP_SIZE = 16;
  logic [PERFMON_RSP_SIZE-1:0] perfmon_rsp_tag_next;
  logic [PERFMON_RSP_SIZE-1:0] perfmon_rsp_tag_q;

  assign perfmon_rsp_tag_next = perfmon_req_info_i.ev_mode[PerfmonEvTag]
    ? { pipe_tag_rsp_valid_o & pipe_tag_rsp_info_o.bubble,
        pipe_tag_rsp_valid_o & pipe_tag_rsp_info_o.hit,
        pipe_tag_rsp_valid_o & ~pipe_tag_rsp_info_o.hit,
        pipe_tag_rsp_valid_o & pipe_tag_rsp_info_o.victim,
        pipe_tag_rsp_valid_o & pipe_tag_rsp_info_o.victim_write_around,
        pipe_tag_rsp_valid_o & ~(pipe_tag_rsp_info_o.victim | pipe_tag_rsp_info_o.victim_write_around),
        pipe_tag_rsp_valid_o ? pipe_tag_rsp_info_o.victim_qwen : {QWEN_SIZE{1'b0}},
        pipe_tag_rsp_valid_o ? pipe_tag_rsp_info_o.opcode : reqq_opcode_e'(0) }
    : '0;

  always_comb begin
    perfmon_rsp_info_o = '0;
    {perfmon_rsp_info_o.ev_rsp.ev_tag_bubble,
     perfmon_rsp_info_o.ev_rsp.ev_tag_hit,
     perfmon_rsp_info_o.ev_rsp.ev_tag_miss,
     perfmon_rsp_info_o.ev_rsp.ev_tag_evict_dirty,
     perfmon_rsp_info_o.ev_rsp.ev_tag_evict_write_around,
     perfmon_rsp_info_o.ev_rsp.ev_tag_evict_none,
     perfmon_rsp_info_o.ev_rsp.ev_tag_victim_qwen,
     perfmon_rsp_info_o.ev_rsp.ev_tag_opcode} = perfmon_rsp_tag_q;
  end
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) perfmon_rsp_tag_q <= '0;
    else if (perfmon_req_info_i.cgater) perfmon_rsp_tag_q <= perfmon_rsp_tag_next;


  // ════════════════════════════════════════════════════════
  // Assertions (Verilator-compatible)
  // ════════════════════════════════════════════════════════
  // synthesis translate_off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert, used here only as assertion guard
  always_ff @(posedge clk_i) begin
    if (rst_ni && pipe_valid_tc_q && (pipe_req_info_tc_q.opcode == L2Fill) && tag_ram_hit_tc)
      $error("ERROR_PIPE_SUB_PIPE_l2_fill_on_hit_tc");
    if (rst_ni && pipe_valid_tc_q && (pipe_req_info_tc_q.opcode == L3Fill) && tag_ram_hit_tc)
      $error("ERROR_PIPE_SUB_PIPE_l3_fill_on_hit_tc");
    if (rst_ni && !$onehot0(tag_ram_hit_bits_tc))
      $error("ERROR_PIPE_SUB_PIPE_multi_tag_hit_tc");
    if (rst_ni && pipe_valid_tc_q && tag_ram_victim_tc && tag_ram_victim_silent_tc)
      $error("ERROR_PIPE_SUB_PIPE_victim_victim_silent_overlap_tc");
    if (rst_ni && pipe_valid_tc_q && tag_ram_victim_write_around_tc && tag_ram_victim_silent_tc)
      $error("ERROR_PIPE_SUB_PIPE_victim_write_around_victim_silent_overlap_tc");
  end
  /* verilator lint_on SYNCASYNCNET */
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni)
    (pipe_valid_tc_q && (pipe_req_info_tc_q.opcode == L2Fill)) |-> !tag_ram_hit_tc)
  else $error("ERROR_PIPE_SUB_PIPE_l2_fill_on_hit_tc");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    (pipe_valid_tc_q && (pipe_req_info_tc_q.opcode == L3Fill)) |-> !tag_ram_hit_tc)
  else $error("ERROR_PIPE_SUB_PIPE_l3_fill_on_hit_tc");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    $onehot0(tag_ram_hit_bits_tc))
  else $error("ERROR_PIPE_SUB_PIPE_multi_tag_hit_tc");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    pipe_valid_tc_q |-> !(tag_ram_victim_tc && tag_ram_victim_silent_tc))
  else $error("ERROR_PIPE_SUB_PIPE_victim_victim_silent_overlap_tc");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    pipe_valid_tc_q |-> !(tag_ram_victim_write_around_tc && tag_ram_victim_silent_tc))
  else $error("ERROR_PIPE_SUB_PIPE_victim_write_around_victim_silent_overlap_tc");
`endif
  // synthesis translate_on


  // ════════════════════════════════════════════════════════
  // LINT
  // ════════════════════════════════════════════════════════
  wire unused_ok = &{1'b0,
    esr_ctl_i,
    pipe_bist_req_info_i.mbist_on,
    l2_addr_tag_set_masked_tc,
    l3_addr_tag_set_and_shire_masked_tc,
    l3_addr_and_shire_masked_tc,
    l3_bank_id_tc,
    my_l3_virtual_shire_id_i,
    tag_ram_hit_bits_tc,
    lru_way_locked_tc,
    pipe_tag_state_ram_rsp_info_i.reqq_id,
    pipe_tag_state_ram_rsp_info_i.trans_id,
    pipe_tag_ram_rsp_info_i.reqq_id,
    pipe_tag_ram_rsp_info_i.trans_id,
    pipe_data_ram_rsp_info_i.reqq_id,
    pipe_data_ram_rsp_info_i.trans_id,
    pipe_dataq_rd_rsp_info_dc_q.trans_id,
    pipe_dataq_rd_rsp_info_dc_q.reqq_id,
    pipe_dataq_rd_rsp_info_dc_q.err,
    pipe_info_tn_d7_q,
    pipe_req_info_dc_d4_q,
    pipe_req_info_dc_dAMO,
    pipe_info_dc_d6_q,
    pipe_info_dc_dAMO,
    wr_sbe_dbe_data,
    int_pipe_info_tc_q,
    perfmon_req_info_i,
    set_match_te_tn_d3,
    set_match_te_tn_d5
  };

  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on WIDTHEXPAND */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
