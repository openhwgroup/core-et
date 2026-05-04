// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache pipe coalesce buffer (cbuf).
//
// Manages a coalesce buffer of CbufSize (32) entries for write coalescing.
// Supports Install, MRU update, Clear, Replace, and Bypass operations.
// Uses LRU replacement. When an install evicts a valid entry, generates
// a victim response to the request queue.
//
// Replaces: shire_cache_pipe_cbuf (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions (_i/_o, _d/_q)
//   - prim_arb_lru instead of arb_lru
//   - Package types (cbuf_req_t etc.) instead of `define types
//   - Explicit always_ff/always_comb instead of RST_EN_FF/EN_FF macros

/* verilator lint_off UNUSEDSIGNAL */  // cbuf_req_t/perfmon structs passed whole; cbuf only reads a subset of fields
module shirecache_pipe_cbuf
  import shirecache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // Invalidate request from sub_bank
  input  logic                        invalidate_req_valid_i,

  // Request interface from sub_banks
  input  logic                        req_valid_i,
  input  cbuf_req_t                   req_info_i,

  // Response interface to reqq
  output logic                        rsp_valid_o,
  output cbuf_rsp_t                   rsp_info_o,

  // State output to idx_cop_sm
  output cbuf_state_t                 state_o,

  // Performance monitor
  input  perfmon_req_info_t           perfmon_req_info_i,
  output perfmon_rsp_info_t           perfmon_rsp_info_o,

  // Configuration
  input  logic [CbufSize-1:0]         esr_cbuf_entry_enable_i,
  input  logic [BankIdSize-1:0]       my_bank_id_i
);

  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert, used in both always_ff and assertions

  // ── Helper: reconstruct line-aligned address ──────────
  function automatic logic [PaSize-1:0] get_line_addr(
    logic [PaSize-1:0]      addr,
    logic [BankIdSize-1:0]  bank_id
  );
    return {addr[PaSize-1:SubBankLsb], bank_id, {BankLsb{1'b0}}};
  endfunction

  // ── Helper: count set bits ────────────────────────────
  function automatic logic [PerfmonRsrcRspSize-1:0] count_valid(
    logic [CbufSize-1:0] entries
  );
    automatic logic [PerfmonRsrcRspSize-1:0] cnt = '0;
    for (int unsigned ii = 0; ii < CbufSize; ii++) begin
      cnt = cnt + PerfmonRsrcRspSize'(entries[ii]);
    end
    return cnt;
  endfunction

  // ── Buffer storage ────────────────────────────────────
  logic [CbufSize-1:0]              cbuf_mem_valid_q;
  logic [PaSize-1:0]                cbuf_mem_q [CbufSize];
  logic                             cbuf_full_q;

  assign state_o.cbuf_mem_valid = cbuf_mem_valid_q;
  // Pack mem into state output
  always_comb begin
    for (int unsigned i = 0; i < CbufSize; i++) begin
      state_o.cbuf_mem[i] = cbuf_mem_q[i];
    end
  end

  // ── Match logic ───────────────────────────────────────
  logic                             match_install_valid;
  logic [CbufIdSize-1:0]            match_install_id;
  logic [CbufSize-1:0]              match_install;
  logic                             match_clear_valid;
  logic [CbufIdSize-1:0]            match_clear_id;
  logic [CbufSize-1:0]              match_clear;

  always_comb begin
    match_install_valid = '0;
    match_install_id    = '0;
    match_install       = '0;
    match_clear_valid   = '0;
    match_clear_id      = '0;
    match_clear         = '0;
    for (int unsigned entry = 0; entry < CbufSize; entry++) begin
      if (req_valid_i && cbuf_mem_valid_q[entry] &&
          (cbuf_mem_q[entry][PaSize-1:SubBankLsb] ==
           req_info_i.install_address[PaSize-1:SubBankLsb])) begin
        match_install_valid      = 1'b1;
        match_install_id         = CbufIdSize'(entry);
        match_install[entry]     = 1'b1;
      end
      if (req_valid_i && cbuf_mem_valid_q[entry] &&
          (cbuf_mem_q[entry][PaSize-1:SubBankLsb] ==
           req_info_i.clear_address[PaSize-1:SubBankLsb])) begin
        match_clear_valid      = 1'b1;
        match_clear_id         = CbufIdSize'(entry);
        match_clear[entry]     = 1'b1;
      end
    end
  end

  // ── LRU arbiter ───────────────────────────────────────
  logic [CbufSize-1:0]              lru_grant;
  logic [CbufIdSize-1:0]            lru_entry_id;

  // Replace-miss: opcode is Replace but clear_address not found in cbuf
  // (entry must be in pending_cbuf_evict — convert to install into LRU way)
  logic replace_miss;
  assign replace_miss = req_valid_i &&
                        (req_info_i.opcode == CBufReplace) &&
                        !match_clear_valid;

  // LRU bid vector selection
  logic [CbufSize-1:0] lru_bid;
  logic [CbufSize-1:0] lru_install_bid;
  assign lru_install_bid = ~(cbuf_mem_valid_q | ~esr_cbuf_entry_enable_i) |
                           ({CbufSize{cbuf_full_q}} & esr_cbuf_entry_enable_i);

  always_comb begin
    unique case (req_info_i.opcode)
      CBufInstall:   lru_bid = lru_install_bid;
      CBufReplace:   lru_bid = replace_miss ? lru_install_bid : match_clear;
      default:       lru_bid = match_install;  // MruUpdate, Clear, Bypass
    endcase
  end

  logic lru_en;
  assign lru_en = req_valid_i && (
    (req_info_i.opcode == CBufInstall) ||
    (req_info_i.opcode == CBufReplace) ||
    ((req_info_i.opcode == CBufMruUpdate) && match_install_valid)
  );

  prim_arb_lru #(
    .NumClients (CbufSize)
  ) u_lru (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (lru_bid),
    .stall_i  (!lru_en),
    .grant_o  (lru_grant),
    .winner_o (lru_entry_id)
  );

  // ── Valid / full logic ────────────────────────────────
  logic [CbufSize-1:0] cbuf_mem_valid_d;
  logic                cbuf_full_d;

  assign cbuf_mem_valid_d =
    (cbuf_mem_valid_q
      | ({CbufSize{(req_info_i.opcode == CBufInstall)}}                   & lru_grant)
      | ({CbufSize{(req_info_i.opcode == CBufReplace) && replace_miss}}   & lru_grant))
    & ~({CbufSize{(req_info_i.opcode == CBufClear)}} & match_clear)
    & ~({CbufSize{invalidate_req_valid_i}});

  assign cbuf_full_d = &(cbuf_mem_valid_d | ~esr_cbuf_entry_enable_i);

  logic update_valid;
  assign update_valid =
    (req_valid_i && ((req_info_i.opcode == CBufInstall) ||
                     ((req_info_i.opcode == CBufReplace) && replace_miss) ||
                     (req_info_i.opcode == CBufClear))) ||
    invalidate_req_valid_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      cbuf_mem_valid_q <= '0;
      cbuf_full_q      <= 1'b0;
    end else if (update_valid) begin
      cbuf_mem_valid_q <= cbuf_mem_valid_d;
      cbuf_full_q      <= cbuf_full_d;
    end
  end

  // ── Victim logic ──────────────────────────────────────
  logic                victim;
  logic [PaSize-1:0]   victim_address;

  assign victim = req_valid_i && cbuf_full_q &&
                  ((req_info_i.opcode == CBufInstall) ||
                   ((req_info_i.opcode == CBufReplace) && replace_miss));

  logic [PaSize-1:0] install_victim_line_addr;
  assign install_victim_line_addr = get_line_addr(cbuf_mem_q[lru_entry_id], my_bank_id_i);

  assign victim_address = victim ? install_victim_line_addr : '0;

  // ── Cbuf memory write ─────────────────────────────────
  logic                install_entry;
  logic                replace_miss_entry;
  logic                replace_entry;
  logic [CbufSize-1:0] mem_entry_update;

  assign install_entry      = req_valid_i && (req_info_i.opcode == CBufInstall);
  assign replace_miss_entry = req_valid_i && (req_info_i.opcode == CBufReplace) &&  replace_miss;
  assign replace_entry      = req_valid_i && (req_info_i.opcode == CBufReplace) && !replace_miss;

  assign mem_entry_update =
    ({CbufSize{install_entry}}      & lru_grant) |
    ({CbufSize{replace_miss_entry}} & lru_grant) |
    ({CbufSize{replace_entry}}      & match_clear);

  logic [PaSize-1:0] install_line_addr;
  assign install_line_addr = get_line_addr(req_info_i.install_address, my_bank_id_i);

  for (genvar i = 0; i < int'(CbufSize); i++) begin : gen_cbuf_mem
    always_ff @(posedge clk_i) begin
      if (mem_entry_update[i]) begin
        cbuf_mem_q[i] <= install_line_addr;
      end
    end
  end

  // ── Response pipeline (_d0 → _d1) ────────────────────
  logic      rsp_valid_d0;
  cbuf_rsp_t rsp_info_d0;

  assign rsp_valid_d0              = req_valid_i & req_info_i.rsp_required;
  assign rsp_info_d0.trans_id      = req_info_i.trans_id;
  assign rsp_info_d0.reqq_id       = req_info_i.reqq_id;
  assign rsp_info_d0.victim        = victim;
  assign rsp_info_d0.victim_address = victim_address;

  logic      rsp_valid_q;
  cbuf_rsp_t rsp_info_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rsp_valid_q <= 1'b0;
    end else if (rsp_valid_q || rsp_valid_d0) begin
      rsp_valid_q <= rsp_valid_d0;
    end
  end

  always_ff @(posedge clk_i) begin
    if (rsp_valid_d0) begin
      rsp_info_q <= rsp_info_d0;
    end
  end

  assign rsp_valid_o = rsp_valid_q;
  assign rsp_info_o  = rsp_info_q;

  // ── Performance monitor ───────────────────────────────
  perfmon_req_info_t                  perfmon_req_q;
  logic [PerfmonRsrcRspSize-1:0]     perfmon_cntr_d;
  logic [PerfmonRsrcRspSize-1:0]     perfmon_cntr_q;

  assign perfmon_cntr_d = perfmon_req_q.rsrc_mode[PerfmonRsrcCbuf]
    ? count_valid(esr_cbuf_entry_enable_i & cbuf_mem_valid_q)
    : '0;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      perfmon_cntr_q <= '0;
      perfmon_req_q  <= '0;
    end else if (perfmon_req_info_i.cgater) begin
      perfmon_cntr_q <= perfmon_cntr_d;
      perfmon_req_q  <= perfmon_req_info_i;
    end
  end

  always_comb begin
    perfmon_rsp_info_o               = '0;
    perfmon_rsp_info_o.rsrc_rsp.rsrc_cntr = perfmon_cntr_q;
  end

  // ── Assertions ────────────────────────────────────────
  // synthesis translate_off
`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    if (rst_ni && req_valid_i && (req_info_i.opcode > CBufBypass))
      $error("ERROR_PIPE_CBUF_no_reserved_opcode: opcode=%0d", req_info_i.opcode);

    if (rst_ni && (match_install & (match_install - CbufSize'(1))) != '0)
      $error("ERROR_PIPE_CBUF_multi_match_install: match_install=0x%0x", match_install);

    if (rst_ni && (match_clear & (match_clear - CbufSize'(1))) != '0)
      $error("ERROR_PIPE_CBUF_multi_match_clear: match_clear=0x%0x", match_clear);

    if (rst_ni && req_valid_i && (req_info_i.opcode == CBufInstall) && match_install_valid)
      $error("ERROR_PIPE_CBUF_install_on_match_install");

    if (rst_ni && (lru_grant & (lru_grant - CbufSize'(1))) != '0)
      $error("ERROR_PIPE_CBUF_multi_lru_grant: lru_grant=0x%0x", lru_grant);

    if (rst_ni && lru_en && (lru_grant == '0))
      $error("ERROR_PIPE_CBUF_lru_update_must_have_grant");
  end
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni)
    req_valid_i |-> (req_info_i.opcode <= CBufBypass))
  else $error("ERROR_PIPE_CBUF_no_reserved_opcode");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    $onehot0(match_install))
  else $error("ERROR_PIPE_CBUF_multi_match_install");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    $onehot0(match_clear))
  else $error("ERROR_PIPE_CBUF_multi_match_clear");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    (req_valid_i && (req_info_i.opcode == CBufInstall)) |-> !match_install_valid)
  else $error("ERROR_PIPE_CBUF_install_on_match_install");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    $onehot0(lru_grant))
  else $error("ERROR_PIPE_CBUF_multi_lru_grant");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    lru_en |-> (lru_grant != '0))
  else $error("ERROR_PIPE_CBUF_lru_update_must_have_grant");
`endif
  // synthesis translate_on

  /* verilator lint_on SYNCASYNCNET */

endmodule : shirecache_pipe_cbuf
/* verilator lint_on UNUSEDSIGNAL */
