// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_sub_bank vs new shirecache_pipe_sub_bank.
//
// Both modules receive identical stimulus.  Key outputs are compared
// cycle-by-cycle from C++.  Behavioral RAM models (simple arrays) live
// inside this wrapper; the NEW module's RAM-request outputs drive them
// and the same responses are fed to both modules.
//
// Original: active-high synchronous reset, sc_* types, ecc_gen/ecc_corr.
// New:      active-low async reset, shirecache_pkg types, prim_ecc_enc/dec.

`include "soc.vh"

module cosim_sub_bank_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Pipe request (scalar decomposition of pipe_info_tap + pipe_req_info_tap) ──
  input  logic                        pipe_valid_tap_i,

  // pipe_info_tap fields
  input  logic [SetIdSize-1:0]        pi_pipe_set_i,
  input  logic [SubBanks-1:0]         pi_pipe_sub_bank_i,
  input  logic [2:0]                  pi_pipe_idx_address_space_i,
  input  logic                        pi_pipe_dec_err_i,
  input  logic [4:0]                  pi_pipe_dec_err_type_i,
  input  logic                        pi_pipe_scp_read_i,
  input  logic                        pi_pipe_bubble_i,
  input  logic                        pi_idx_write_i,
  input  logic                        pi_tag_hit_i,
  input  logic [WayIdSize-1:0]        pi_tag_ram_hit_way_i,
  input  logic                        pi_tag_sbe_i,
  input  logic                        pi_victim_i,
  input  logic                        pi_data_rd_req_i,
  input  logic                        pi_data_wr_req_i,
  input  logic                        pi_data_wr_zero_i,
  input  logic [LineQwSize-1:0]       pi_data_wr_qwen_i,
  input  logic                        pi_dataq_rd_req_en_i,
  input  logic                        pi_dataq_rd_req_partial_i,
  input  logic [LineQwSize-1:0]       pi_dataq_sel_qwen_i,
  input  logic                        pi_merge_wr_data_in_rd_rsp_i,
  input  logic                        pi_merge_rd_data_in_wr_req_i,
  input  logic                        pi_data_rsp_req_i,
  input  logic                        pi_data_rsp_zero_i,
  input  logic [LineQwSize-1:0]       pi_data_rsp_qwen_i,

  // pipe_req_info_tap fields
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

  // ESR config scalars
  input  logic [SetIdSize-1:0]        esr_sc_l2_tag_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_tag_mask_i,
  input  logic [RamDelaySize-1:0]     esr_sc_ram_delay_i,
  input  logic                        esr_sc_l2_rbuf_enable_i,
  input  logic                        esr_sc_scp_rbuf_enable_i,
  input  logic                        esr_sc_zero_state_enable_i,
  input  logic                        esr_sc_all_shire_aliasing_i,
  input  logic                        esr_sc_two_shire_aliasing_use_shire_lsb_i,
  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [L3ShireIdSize-1:0]    my_l3_virtual_shire_id_i,

  // esr_ctl sub-fields needed for sbe_dbe clear
  input  logic                        esr_wr_en_sbe_dbe_counts_i,
  input  logic [DwDataSize-1:0]       esr_wr_data_i,

  // ESR set allocation
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l2_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l3_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_scp_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_tag_mask_i,

  // ── Comparison outputs: NEW module ──
  output logic                        new_pipe_squash_tap_o,
  output logic                        new_tag_rsp_valid_o,
  output logic [$bits(tag_rsp_t)-1:0]       new_tag_rsp_info_o,
  output logic                        new_data_rsp_pre_valid_o,
  output logic [$bits(data_rsp_pre_t)-1:0]  new_data_rsp_pre_info_o,
  output logic                        new_data_rsp_valid_o,
  output logic [$bits(data_rsp_t)-1:0]      new_data_rsp_info_o,
  output logic                        new_dataq_rd_req_valid_o,
  output logic                        new_cbuf_invalidate_req_valid_o,
  output logic                        new_cbuf_req_valid_o,
  output logic                        new_rbuf_invalidate_req_valid_o,
  output logic                        new_rbuf_install_req_valid_o,
  output logic                        new_rbuf_evict_req_valid_o,
  output logic                        new_rbuf_data_req_valid_o,
  output logic [SubBanks-1:0]         new_ts_ram_rd_en_o,
  output logic [SubBanks-1:0]         new_ts_ram_wr_en_o,
  output logic [SubBanks-1:0]         new_tag_ram_rd_en_o,
  output logic [SubBanks-1:0]         new_tag_ram_wr_en_o,
  output logic [SubBanks-1:0]         new_data_ram_rd_en_o,
  output logic [SubBanks-1:0]         new_data_ram_wr_en_o,
  output logic [SubBanks-1:0]         new_data_ram_amo_wr_en_o,

  // ── Comparison outputs: ORIGINAL module ──
  output logic                        orig_pipe_squash_tap_o,
  output logic                        orig_tag_rsp_valid_o,
  output logic [$bits(tag_rsp_t)-1:0]       orig_tag_rsp_info_o,
  output logic                        orig_data_rsp_pre_valid_o,
  output logic [$bits(data_rsp_pre_t)-1:0]  orig_data_rsp_pre_info_o,
  output logic                        orig_data_rsp_valid_o,
  output logic [$bits(data_rsp_t)-1:0]      orig_data_rsp_info_o,
  output logic                        orig_dataq_rd_req_valid_o,
  output logic                        orig_cbuf_invalidate_req_valid_o,
  output logic                        orig_cbuf_req_valid_o,
  output logic                        orig_rbuf_invalidate_req_valid_o,
  output logic                        orig_rbuf_install_req_valid_o,
  output logic                        orig_rbuf_evict_req_valid_o,
  output logic                        orig_rbuf_data_req_valid_o,
  output logic [SubBanks-1:0]         orig_ts_ram_rd_en_o,
  output logic [SubBanks-1:0]         orig_ts_ram_wr_en_o,
  output logic [SubBanks-1:0]         orig_tag_ram_rd_en_o,
  output logic [SubBanks-1:0]         orig_tag_ram_wr_en_o,
  output logic [SubBanks-1:0]         orig_data_ram_rd_en_o,
  output logic [SubBanks-1:0]         orig_data_ram_wr_en_o,
  output logic [SubBanks-1:0]         orig_data_ram_amo_wr_en_o
);

  // ════════════════════════════════════════════════════════
  // Reset
  // ════════════════════════════════════════════════════════
  wire reset = !rst_ni;

  // ════════════════════════════════════════════════════════
  // Assemble struct inputs
  // ════════════════════════════════════════════════════════

  // ── pipe_info_tap ──
  pipe_info_t new_pipe_info_tap;
  always_comb begin
    new_pipe_info_tap.pipe_set               = pi_pipe_set_i;
    new_pipe_info_tap.pipe_sub_bank          = pi_pipe_sub_bank_i;
    new_pipe_info_tap.pipe_idx_address_space = address_space_e'(pi_pipe_idx_address_space_i);
    new_pipe_info_tap.pipe_dec_err           = pi_pipe_dec_err_i;
    new_pipe_info_tap.pipe_dec_err_type      = err_rsp_e'(pi_pipe_dec_err_type_i);
    new_pipe_info_tap.pipe_scp_read          = pi_pipe_scp_read_i;
    new_pipe_info_tap.pipe_bubble            = pi_pipe_bubble_i;
    new_pipe_info_tap.idx_write              = pi_idx_write_i;
    new_pipe_info_tap.tag_hit                = pi_tag_hit_i;
    new_pipe_info_tap.tag_ram_hit_way        = pi_tag_ram_hit_way_i;
    new_pipe_info_tap.tag_sbe                = pi_tag_sbe_i;
    new_pipe_info_tap.victim                 = pi_victim_i;
    new_pipe_info_tap.data_rd_req            = pi_data_rd_req_i;
    new_pipe_info_tap.data_wr_req            = pi_data_wr_req_i;
    new_pipe_info_tap.data_wr_zero           = pi_data_wr_zero_i;
    new_pipe_info_tap.data_wr_qwen           = pi_data_wr_qwen_i;
    new_pipe_info_tap.dataq_rd_req_en        = pi_dataq_rd_req_en_i;
    new_pipe_info_tap.dataq_rd_req_partial   = pi_dataq_rd_req_partial_i;
    new_pipe_info_tap.dataq_sel_qwen         = pi_dataq_sel_qwen_i;
    new_pipe_info_tap.merge_wr_data_in_rd_rsp = pi_merge_wr_data_in_rd_rsp_i;
    new_pipe_info_tap.merge_rd_data_in_wr_req = pi_merge_rd_data_in_wr_req_i;
    new_pipe_info_tap.data_rsp_req           = pi_data_rsp_req_i;
    new_pipe_info_tap.data_rsp_zero          = pi_data_rsp_zero_i;
    new_pipe_info_tap.data_rsp_qwen          = pi_data_rsp_qwen_i;
  end

  sc_pipe_info_t orig_pipe_info_tap;
  assign orig_pipe_info_tap = new_pipe_info_tap;

  // ── reqq_pipe_req ──
  reqq_pipe_req_t new_pipe_req_info_tap;
  always_comb begin
    new_pipe_req_info_tap.trans_id      = pr_trans_id_i;
    new_pipe_req_info_tap.reqq_id       = pr_reqq_id_i;
    new_pipe_req_info_tap.opcode        = reqq_opcode_e'(pr_opcode_i);
    new_pipe_req_info_tap.address       = pr_address_i;
    new_pipe_req_info_tap.address_space = address_space_e'(pr_address_space_i);
    new_pipe_req_info_tap.zero_data     = pr_zero_data_i;
    new_pipe_req_info_tap.qwen          = pr_qwen_i;
    new_pipe_req_info_tap.size          = sc_size_t'(pr_size_i);
    new_pipe_req_info_tap.sub_bank_id   = pr_sub_bank_id_i;
  end

  sc_reqq_pipe_req_t orig_pipe_req_info_tap;
  assign orig_pipe_req_info_tap = new_pipe_req_info_tap;

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

  // ── ESR ctl (zero all fields then fill what sub_bank uses) ──
  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_l2_set_base  = esr_sc_l2_set_base_i;
    new_esr_ctl.esr_sc_l2_set_size  = esr_sc_l2_set_size_i;
    new_esr_ctl.esr_sc_l2_set_mask  = esr_sc_l2_set_mask_i;
    new_esr_ctl.esr_sc_l2_tag_mask  = esr_sc_l2_tag_mask_i;
    new_esr_ctl.esr_sc_l3_set_base  = esr_sc_l3_set_base_i;
    new_esr_ctl.esr_sc_l3_set_size  = esr_sc_l3_set_size_i;
    new_esr_ctl.esr_sc_l3_set_mask  = esr_sc_l3_set_mask_i;
    new_esr_ctl.esr_sc_l3_tag_mask  = esr_sc_l3_tag_mask_i;
    new_esr_ctl.esr_sc_scp_set_base = esr_sc_scp_set_base_i;
    new_esr_ctl.esr_sc_scp_set_size = esr_sc_scp_set_size_i;
    new_esr_ctl.esr_sc_scp_set_mask = esr_sc_scp_set_mask_i;
    new_esr_ctl.esr_sc_scp_tag_mask = esr_sc_scp_tag_mask_i;
    new_esr_ctl.wr_en_sbe_dbe_counts = esr_wr_en_sbe_dbe_counts_i;
    new_esr_ctl.wr_data              = esr_wr_data_i;
  end

  sc_bank_esr_ctl_t orig_esr_ctl;
  assign orig_esr_ctl = new_esr_ctl;

  // ── l3_swizzle_ctl ──
  l3_swizzle_ctl_t new_l3_swizzle_ctl;
  always_comb begin
    new_l3_swizzle_ctl = '0;  // default zero — no swizzling for cosim
  end

  esr_sc_l3_shire_swizzle_ctl_t orig_l3_swizzle_ctl;
  always_comb begin
    orig_l3_swizzle_ctl.esr_sc_two_shire_aliasing_use_shire_lsb = new_l3_swizzle_ctl.two_shire_aliasing_use_shire_lsb;
    orig_l3_swizzle_ctl.esr_sc_all_shire_aliasing                = new_l3_swizzle_ctl.all_shire_aliasing;
    orig_l3_swizzle_ctl.esr_sc_sub_bank_sel_b2 = new_l3_swizzle_ctl.sub_bank_sel_b2;
    orig_l3_swizzle_ctl.esr_sc_sub_bank_sel_b1 = new_l3_swizzle_ctl.sub_bank_sel_b1;
    orig_l3_swizzle_ctl.esr_sc_sub_bank_sel_b0 = new_l3_swizzle_ctl.sub_bank_sel_b0;
    orig_l3_swizzle_ctl.esr_sc_bank_sel_b2     = new_l3_swizzle_ctl.bank_sel_b2;
    orig_l3_swizzle_ctl.esr_sc_bank_sel_b1     = new_l3_swizzle_ctl.bank_sel_b1;
    orig_l3_swizzle_ctl.esr_sc_bank_sel_b0     = new_l3_swizzle_ctl.bank_sel_b0;
    orig_l3_swizzle_ctl.esr_sc_shire_sel_b5    = new_l3_swizzle_ctl.shire_sel_b5;
    orig_l3_swizzle_ctl.esr_sc_shire_sel_b4    = new_l3_swizzle_ctl.shire_sel_b4;
    orig_l3_swizzle_ctl.esr_sc_shire_sel_b3    = new_l3_swizzle_ctl.shire_sel_b3;
    orig_l3_swizzle_ctl.esr_sc_shire_sel_b2    = new_l3_swizzle_ctl.shire_sel_b2;
    orig_l3_swizzle_ctl.esr_sc_shire_sel_b1    = new_l3_swizzle_ctl.shire_sel_b1;
    orig_l3_swizzle_ctl.esr_sc_shire_sel_b0    = new_l3_swizzle_ctl.shire_sel_b0;
  end

  // ── dbg_wr — tie to zero (no idx_cop_sm in cosim) ──
  idx_cop_sm_dbg_write_t new_dbg_wr;
  assign new_dbg_wr = '0;

  sc_idx_cop_sm_dbg_write_t orig_dbg_wr;
  assign orig_dbg_wr = '0;

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
  // Behavioral RAM models
  // ════════════════════════════════════════════════════════
  // Driven by the NEW module's RAM request ports.
  // Responses fed identically to both modules.

  // ── Tag-state RAM  (per sub-bank) ──
  localparam int TS_DEPTH = 1 << TagStateRamAddrSize;
  logic [TagStateRamDataSize-1:0] ts_ram [SubBanks-1:0][TS_DEPTH-1:0];

  // ── Tag RAM (per sub-bank) ──
  localparam int TAG_DEPTH = 1 << TagRamAddrSize;
  logic [TagRamDataSize-1:0] tag_ram [SubBanks-1:0][TAG_DEPTH-1:0];

  // ── Data RAM (per sub-bank) ──
  localparam int DATA_DEPTH = 1 << DataRamAddrSize;
  localparam int QW_WIDTH   = DataRamDataSize / LineQwSize;  // per-QW width
  logic [DataRamDataSize-1:0] data_ram [SubBanks-1:0][DATA_DEPTH-1:0];

  // NEW module signals
  logic [SubBanks-1:0]        new_ts_ram_rd_en, new_ts_ram_wr_en;
  tag_state_ram_req_t         new_ts_ram_req;
  logic                       new_ts_ram_rsp_valid;
  tag_state_ram_rsp_t         new_ts_ram_rsp;

  logic [SubBanks-1:0]        new_tag_ram_rd_en, new_tag_ram_wr_en;
  tag_ram_req_t               new_tag_ram_req;
  logic                       new_tag_ram_rsp_valid;
  tag_ram_rsp_t               new_tag_ram_rsp;

  logic [SubBanks-1:0]        new_data_ram_rd_en, new_data_ram_wr_en, new_data_ram_amo_wr_en;
  data_ram_req_t              new_data_ram_req;
  logic                       new_data_ram_rsp_valid;
  data_ram_rsp_t              new_data_ram_rsp;

  // ORIG module signals
  logic [`SC_SUB_BANKS-1:0]   orig_ts_ram_rd_en, orig_ts_ram_wr_en;
  sc_pipe_tag_state_ram_req_t orig_ts_ram_req;
  logic                       orig_ts_ram_rsp_valid;
  sc_pipe_tag_state_ram_rsp_t orig_ts_ram_rsp;

  logic [`SC_SUB_BANKS-1:0]   orig_tag_ram_rd_en, orig_tag_ram_wr_en;
  sc_pipe_tag_ram_req_t       orig_tag_ram_req;
  logic                       orig_tag_ram_rsp_valid;
  sc_pipe_tag_ram_rsp_t       orig_tag_ram_rsp;

  logic [`SC_SUB_BANKS-1:0]   orig_data_ram_rd_en, orig_data_ram_wr_en, orig_data_ram_amo_wr_en;
  sc_pipe_data_ram_req_t      orig_data_ram_req;
  logic                       orig_data_ram_rsp_valid;
  sc_pipe_data_ram_rsp_t      orig_data_ram_rsp;

  // ── RAM model: tag-state ──
  // Pipeline the read to produce 1-cycle latency (the _ta flop is inside sub_bank;
  // the RAM itself has registered output).
  logic [TagStateRamDataSize-1:0] ts_rd_data_d;
  logic [DvTransIdSize-1:0]       ts_rd_trans_id_d;
  logic [ReqqIdSize-1:0]          ts_rd_reqq_id_d;
  logic                           ts_rd_valid_d;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ts_rd_valid_d <= 1'b0;
    end else begin
      ts_rd_valid_d    <= |new_ts_ram_rd_en;
      ts_rd_trans_id_d <= new_ts_ram_req.rd_trans_id;
      ts_rd_reqq_id_d  <= new_ts_ram_req.rd_reqq_id;
      // Read from first active sub-bank
      ts_rd_data_d     <= '0;
      for (int sb = 0; sb < SubBanks; sb++) begin
        if (new_ts_ram_rd_en[sb])
          ts_rd_data_d <= ts_ram[sb][new_ts_ram_req.rd_addr];
      end
      // Writes
      for (int sb = 0; sb < SubBanks; sb++) begin
        if (new_ts_ram_wr_en[sb])
          ts_ram[sb][new_ts_ram_req.wr_addr] <= new_ts_ram_req.wr_data;
      end
    end
  end

  always_comb begin
    new_ts_ram_rsp.trans_id = ts_rd_trans_id_d;
    new_ts_ram_rsp.reqq_id  = ts_rd_reqq_id_d;
    new_ts_ram_rsp.rd_data  = ts_rd_data_d;
  end
  assign new_ts_ram_rsp_valid = ts_rd_valid_d;

  // Feed same response to orig
  assign orig_ts_ram_rsp_valid = new_ts_ram_rsp_valid;
  assign orig_ts_ram_rsp       = new_ts_ram_rsp;

  // ── RAM model: tag ──
  logic [TagRamDataSize-1:0]  tag_rd_data_d;
  logic [DvTransIdSize-1:0]   tag_rd_trans_id_d;
  logic [ReqqIdSize-1:0]      tag_rd_reqq_id_d;
  logic                       tag_rd_valid_d;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tag_rd_valid_d <= 1'b0;
    end else begin
      tag_rd_valid_d    <= |new_tag_ram_rd_en;
      tag_rd_trans_id_d <= new_tag_ram_req.rd_trans_id;
      tag_rd_reqq_id_d  <= new_tag_ram_req.rd_reqq_id;
      tag_rd_data_d     <= '0;
      for (int sb = 0; sb < SubBanks; sb++) begin
        if (new_tag_ram_rd_en[sb])
          tag_rd_data_d <= tag_ram[sb][new_tag_ram_req.rd_addr];
      end
      for (int sb = 0; sb < SubBanks; sb++) begin
        if (new_tag_ram_wr_en[sb])
          tag_ram[sb][new_tag_ram_req.wr_addr] <= new_tag_ram_req.wr_data;
      end
    end
  end

  always_comb begin
    new_tag_ram_rsp.trans_id = tag_rd_trans_id_d;
    new_tag_ram_rsp.reqq_id  = tag_rd_reqq_id_d;
    new_tag_ram_rsp.rd_data  = tag_rd_data_d;
  end
  assign new_tag_ram_rsp_valid = tag_rd_valid_d;

  assign orig_tag_ram_rsp_valid = new_tag_ram_rsp_valid;
  assign orig_tag_ram_rsp       = new_tag_ram_rsp;

  // ── RAM model: data ──
  logic [DataRamDataSize-1:0] data_rd_data_d;
  logic [DvTransIdSize-1:0]   data_rd_trans_id_d;
  logic [ReqqIdSize-1:0]      data_rd_reqq_id_d;
  logic                       data_rd_valid_d;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      data_rd_valid_d <= 1'b0;
    end else begin
      data_rd_valid_d    <= |new_data_ram_rd_en;
      data_rd_trans_id_d <= new_data_ram_req.rd_trans_id;
      data_rd_reqq_id_d  <= new_data_ram_req.rd_reqq_id;
      data_rd_data_d     <= '0;
      for (int sb = 0; sb < SubBanks; sb++) begin
        if (new_data_ram_rd_en[sb])
          data_rd_data_d <= data_ram[sb][new_data_ram_req.rd_addr];
      end
      // Normal writes (per-QW enable)
      for (int sb = 0; sb < SubBanks; sb++) begin
        if (new_data_ram_wr_en[sb]) begin
          for (int qw = 0; qw < LineQwSize; qw++) begin
            if (new_data_ram_req.wr_qwen[qw])
              data_ram[sb][new_data_ram_req.wr_addr][qw*QW_WIDTH +: QW_WIDTH] <= new_data_ram_req.wr_data[qw*QW_WIDTH +: QW_WIDTH];
          end
        end
      end
      // Atomic writes (per-QW enable)
      for (int sb = 0; sb < SubBanks; sb++) begin
        if (new_data_ram_amo_wr_en[sb]) begin
          for (int qw = 0; qw < LineQwSize; qw++) begin
            if (new_data_ram_req.amo_wr_qwen[qw])
              data_ram[sb][new_data_ram_req.amo_wr_addr][qw*QW_WIDTH +: QW_WIDTH] <= new_data_ram_req.amo_wr_data[qw*QW_WIDTH +: QW_WIDTH];
          end
        end
      end
    end
  end

  always_comb begin
    new_data_ram_rsp.trans_id = data_rd_trans_id_d;
    new_data_ram_rsp.reqq_id  = data_rd_reqq_id_d;
    new_data_ram_rsp.rd_data  = data_rd_data_d;
  end
  assign new_data_ram_rsp_valid = data_rd_valid_d;

  assign orig_data_ram_rsp_valid = new_data_ram_rsp_valid;
  assign orig_data_ram_rsp       = new_data_ram_rsp;

  // Initialize RAMs to zero
  initial begin
    for (int sb = 0; sb < SubBanks; sb++) begin
      for (int i = 0; i < TS_DEPTH; i++)   ts_ram[sb][i]   = '0;
      for (int i = 0; i < TAG_DEPTH; i++)  tag_ram[sb][i]  = '0;
      for (int i = 0; i < DATA_DEPTH; i++) data_ram[sb][i] = '0;
    end
  end

  // ════════════════════════════════════════════════════════
  // NEW module instance
  // ════════════════════════════════════════════════════════
  tag_rsp_t         new_tag_rsp;
  data_rsp_pre_t    new_data_rsp_pre;
  data_rsp_t        new_data_rsp;
  dataq_rd_req_t    new_dataq_rd_req;
  cbuf_req_t        new_cbuf_req;
  rbuf_install_req_t new_rbuf_install_req;
  rbuf_evict_req_t  new_rbuf_evict_req;
  rbuf_data_req_t   new_rbuf_data_req;
  idx_cop_sm_dbg_read_t new_dbg_rd;
  bist_rsp_t        new_bist_rsp;
  perfmon_rsp_info_t new_perfmon_rsp;
  sbe_dbe_esr_status_t new_sbe_dbe_status;
  sbe_dbe_overflow_t   new_sbe_dbe_overflow;

  shirecache_pipe_sub_bank u_new (
    .clk_i                              (clk_i),
    .rst_ni                             (rst_ni),
    .pipe_valid_tap_i                   (pipe_valid_tap_i),
    .pipe_info_tap_i                    (new_pipe_info_tap),
    .pipe_req_info_tap_i                (new_pipe_req_info_tap),
    .pipe_squash_tap_o                  (new_pipe_squash_tap_o),
    .pipe_tag_rsp_valid_o               (new_tag_rsp_valid_o),
    .pipe_tag_rsp_info_o                (new_tag_rsp),
    .pipe_data_rsp_pre_valid_o          (new_data_rsp_pre_valid_o),
    .pipe_data_rsp_pre_info_o           (new_data_rsp_pre),
    .pipe_data_rsp_valid_o              (new_data_rsp_valid_o),
    .pipe_data_rsp_info_o               (new_data_rsp),
    .pipe_dataq_rd_req_valid_o          (new_dataq_rd_req_valid_o),
    .pipe_dataq_rd_req_info_o           (new_dataq_rd_req),
    .pipe_dataq_rd_rsp_info_i           (new_dataq_rd_rsp),
    .pipe_cbuf_invalidate_req_valid_o   (new_cbuf_invalidate_req_valid_o),
    .pipe_cbuf_req_valid_o              (new_cbuf_req_valid_o),
    .pipe_cbuf_req_info_o               (new_cbuf_req),
    .pipe_rbuf_invalidate_req_valid_o   (new_rbuf_invalidate_req_valid_o),
    .pipe_rbuf_install_req_valid_o      (new_rbuf_install_req_valid_o),
    .pipe_rbuf_install_req_info_o       (new_rbuf_install_req),
    .pipe_rbuf_evict_req_valid_o        (new_rbuf_evict_req_valid_o),
    .pipe_rbuf_evict_req_info_o         (new_rbuf_evict_req),
    .pipe_rbuf_data_req_valid_o         (new_rbuf_data_req_valid_o),
    .pipe_rbuf_data_req_info_o          (new_rbuf_data_req),
    .pipe_tag_state_ram_req_rd_en_o     (new_ts_ram_rd_en),
    .pipe_tag_state_ram_req_wr_en_o     (new_ts_ram_wr_en),
    .pipe_tag_state_ram_req_info_o      (new_ts_ram_req),
    .pipe_tag_ram_req_rd_en_o           (new_tag_ram_rd_en),
    .pipe_tag_ram_req_wr_en_o           (new_tag_ram_wr_en),
    .pipe_tag_ram_req_info_o            (new_tag_ram_req),
    .pipe_data_ram_req_rd_en_o          (new_data_ram_rd_en),
    .pipe_data_ram_req_wr_en_o          (new_data_ram_wr_en),
    .pipe_data_ram_req_amo_wr_en_o      (new_data_ram_amo_wr_en),
    .pipe_data_ram_req_info_o           (new_data_ram_req),
    .pipe_tag_state_ram_rsp_valid_i     (new_ts_ram_rsp_valid),
    .pipe_tag_state_ram_rsp_info_i      (new_ts_ram_rsp),
    .pipe_tag_ram_rsp_valid_i           (new_tag_ram_rsp_valid),
    .pipe_tag_ram_rsp_info_i            (new_tag_ram_rsp),
    .pipe_data_ram_rsp_valid_i          (new_data_ram_rsp_valid),
    .pipe_data_ram_rsp_info_i           (new_data_ram_rsp),
    .esr_dbg_wr_info_i                  (new_dbg_wr),
    .dbg_rd_info_o                      (new_dbg_rd),
    .pipe_bist_req_info_i               (new_bist_req),
    .pipe_bist_rsp_info_o               (new_bist_rsp),
    .perfmon_req_info_i                 (new_perfmon_req),
    .perfmon_rsp_info_o                 (new_perfmon_rsp),
    .esr_ctl_i                          (new_esr_ctl),
    .l3_swizzle_ctl_i                   (new_l3_swizzle_ctl),
    .esr_sc_l2_tag_mask_i               (esr_sc_l2_tag_mask_i),
    .esr_sc_l3_tag_mask_i               (esr_sc_l3_tag_mask_i),
    .esr_sc_ram_delay_i                 (esr_sc_ram_delay_i),
    .esr_sc_l2_rbuf_enable_i            (esr_sc_l2_rbuf_enable_i),
    .esr_sc_scp_rbuf_enable_i           (esr_sc_scp_rbuf_enable_i),
    .esr_sc_zero_state_enable_i         (esr_sc_zero_state_enable_i),
    .esr_sc_all_shire_aliasing_i        (esr_sc_all_shire_aliasing_i),
    .esr_sc_two_shire_aliasing_use_shire_lsb_i (esr_sc_two_shire_aliasing_use_shire_lsb_i),
    .pipe_sbe_dbe_esr_status_o          (new_sbe_dbe_status),
    .pipe_sbe_dbe_overflow_o            (new_sbe_dbe_overflow),
    .my_bank_id_i                       (my_bank_id_i),
    .my_l3_virtual_shire_id_i           (my_l3_virtual_shire_id_i)
  );

  assign new_tag_rsp_info_o       = new_tag_rsp;
  assign new_data_rsp_pre_info_o  = new_data_rsp_pre;
  assign new_data_rsp_info_o      = new_data_rsp;
  assign new_ts_ram_rd_en_o       = new_ts_ram_rd_en;
  assign new_ts_ram_wr_en_o       = new_ts_ram_wr_en;
  assign new_tag_ram_rd_en_o      = new_tag_ram_rd_en;
  assign new_tag_ram_wr_en_o      = new_tag_ram_wr_en;
  assign new_data_ram_rd_en_o     = new_data_ram_rd_en;
  assign new_data_ram_wr_en_o     = new_data_ram_wr_en;
  assign new_data_ram_amo_wr_en_o = new_data_ram_amo_wr_en;

  // ════════════════════════════════════════════════════════
  // ORIGINAL module instance
  // ════════════════════════════════════════════════════════
  sc_pipe_tag_rsp_t         orig_tag_rsp;
  sc_pipe_data_rsp_pre_t    orig_data_rsp_pre;
  sc_pipe_data_rsp_t        orig_data_rsp;
  sc_pipe_dataq_rd_req_t    orig_dataq_rd_req;
  sc_pipe_cbuf_req_t        orig_cbuf_req;
  sc_pipe_rbuf_install_req_t orig_rbuf_install_req;
  sc_pipe_rbuf_evict_req_t  orig_rbuf_evict_req;
  sc_pipe_rbuf_data_req_t   orig_rbuf_data_req;
  sc_idx_cop_sm_dbg_read_t  orig_dbg_rd;
  sc_pipe_bist_rsp_t        orig_bist_rsp;
  sc_perfmon_rsp_info_t     orig_perfmon_rsp;
  sc_pipe_sbe_dbe_esr_status_t orig_sbe_dbe_status;
  sc_pipe_sbe_dbe_overflow_t   orig_sbe_dbe_overflow;

  shire_cache_pipe_sub_bank u_orig (
    .clock                              (clk_i),
    .reset                              (reset),
    .pipe_valid_tap                     (pipe_valid_tap_i),
    .pipe_info_tap                      (orig_pipe_info_tap),
    .pipe_req_info_tap                  (orig_pipe_req_info_tap),
    .pipe_squash_tap                    (orig_pipe_squash_tap_o),
    .pipe_tag_rsp_valid                 (orig_tag_rsp_valid_o),
    .pipe_tag_rsp_info                  (orig_tag_rsp),
    .pipe_data_rsp_pre_valid            (orig_data_rsp_pre_valid_o),
    .pipe_data_rsp_pre_info             (orig_data_rsp_pre),
    .pipe_data_rsp_valid                (orig_data_rsp_valid_o),
    .pipe_data_rsp_info                 (orig_data_rsp),
    .pipe_dataq_rd_req_valid            (orig_dataq_rd_req_valid_o),
    .pipe_dataq_rd_req_info             (orig_dataq_rd_req),
    .pipe_dataq_rd_rsp_info             (orig_dataq_rd_rsp),
    .pipe_cbuf_invalidate_req_valid     (orig_cbuf_invalidate_req_valid_o),
    .pipe_cbuf_req_valid                (orig_cbuf_req_valid_o),
    .pipe_cbuf_req_info                 (orig_cbuf_req),
    .pipe_rbuf_invalidate_req_valid     (orig_rbuf_invalidate_req_valid_o),
    .pipe_rbuf_install_req_valid        (orig_rbuf_install_req_valid_o),
    .pipe_rbuf_install_req_info         (orig_rbuf_install_req),
    .pipe_rbuf_evict_req_valid          (orig_rbuf_evict_req_valid_o),
    .pipe_rbuf_evict_req_info           (orig_rbuf_evict_req),
    .pipe_rbuf_data_req_valid           (orig_rbuf_data_req_valid_o),
    .pipe_rbuf_data_req_info            (orig_rbuf_data_req),
    .pipe_tag_state_ram_req_rd_en       (orig_ts_ram_rd_en),
    .pipe_tag_state_ram_req_wr_en       (orig_ts_ram_wr_en),
    .pipe_tag_state_ram_req_info        (orig_ts_ram_req),
    .pipe_tag_ram_req_rd_en             (orig_tag_ram_rd_en),
    .pipe_tag_ram_req_wr_en             (orig_tag_ram_wr_en),
    .pipe_tag_ram_req_info              (orig_tag_ram_req),
    .pipe_data_ram_req_rd_en            (orig_data_ram_rd_en),
    .pipe_data_ram_req_wr_en            (orig_data_ram_wr_en),
    .pipe_data_ram_req_amo_wr_en        (orig_data_ram_amo_wr_en),
    .pipe_data_ram_req_info             (orig_data_ram_req),
    .pipe_tag_state_ram_rsp_valid       (orig_ts_ram_rsp_valid),
    .pipe_tag_state_ram_rsp_info        (orig_ts_ram_rsp),
    .pipe_tag_ram_rsp_valid             (orig_tag_ram_rsp_valid),
    .pipe_tag_ram_rsp_info              (orig_tag_ram_rsp),
    .pipe_data_ram_rsp_valid            (orig_data_ram_rsp_valid),
    .pipe_data_ram_rsp_info             (orig_data_ram_rsp),
    .esr_dbg_wr_info                    (orig_dbg_wr),
    .dbg_rd_info                        (orig_dbg_rd),
    .pipe_bist_req_info                 (orig_bist_req),
    .pipe_bist_rsp_info                 (orig_bist_rsp),
    .perfmon_req_info                   (orig_perfmon_req),
    .perfmon_rsp_info                   (orig_perfmon_rsp),
    .esr_ctl                            (orig_esr_ctl),
    .esr_sc_l3_shire_swizzle_ctl        (orig_l3_swizzle_ctl),
    .esr_sc_l2_tag_mask                 (esr_sc_l2_tag_mask_i),
    .esr_sc_l3_tag_mask                 (esr_sc_l3_tag_mask_i),
    .esr_sc_ram_delay                   (esr_sc_ram_delay_i),
    .esr_sc_l2_rbuf_enable              (esr_sc_l2_rbuf_enable_i),
    .esr_sc_scp_rbuf_enable             (esr_sc_scp_rbuf_enable_i),
    .esr_sc_zero_state_enable           (esr_sc_zero_state_enable_i),
    .esr_sc_all_shire_aliasing          (esr_sc_all_shire_aliasing_i),
    .esr_sc_two_shire_aliasing_use_shire_lsb (esr_sc_two_shire_aliasing_use_shire_lsb_i),
    .pipe_sbe_dbe_esr_status            (orig_sbe_dbe_status),
    .pipe_sbe_dbe_overflow              (orig_sbe_dbe_overflow),
    .my_bank_id                         (my_bank_id_i),
    .my_l3_virtual_shire_id             (my_l3_virtual_shire_id_i)
  );

  assign orig_tag_rsp_info_o       = orig_tag_rsp;
  assign orig_data_rsp_pre_info_o  = orig_data_rsp_pre;
  assign orig_data_rsp_info_o      = orig_data_rsp;
  assign orig_ts_ram_rd_en_o       = orig_ts_ram_rd_en;
  assign orig_ts_ram_wr_en_o       = orig_ts_ram_wr_en;
  assign orig_tag_ram_rd_en_o      = orig_tag_ram_rd_en;
  assign orig_tag_ram_wr_en_o      = orig_tag_ram_wr_en;
  assign orig_data_ram_rd_en_o     = orig_data_ram_rd_en;
  assign orig_data_ram_wr_en_o     = orig_data_ram_wr_en;
  assign orig_data_ram_amo_wr_en_o = orig_data_ram_amo_wr_en;

endmodule : cosim_sub_bank_tb
