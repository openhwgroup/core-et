// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache pipe read buffer (rbuf).
//
// Manages a read buffer of RbufSize (8) entries for read data caching.
// Features:
//   - Install on read hits (silently evict LRU way if full)
//   - Check for rbuf hits and return data through credit-based FIFO
//   - Invalidate entry on cache capacity eviction
//   - 7-stage pending-install pipeline matching RAM read delay (2/3/4 cycles)
//   - Pending installs can be squashed by evictions or cleared by reqq
//
// Replaces: shire_cache_pipe_rbuf (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions
//   - prim_arb_lru instead of arb_lru, prim_fifo_reg instead of gen_fifo_reg
//   - Package types instead of `define types
//   - Explicit always_ff/always_comb instead of RST_EN_FF/EN_FF macros

/* verilator lint_off UNUSEDSIGNAL */  // struct ports carry full pipeline context; rbuf only reads address/scp/data subfields
module shirecache_pipe_rbuf
  import shirecache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // Invalidate request from sub_bank
  input  logic                        invalidate_req_valid_i,

  // Install interface from sub_bank
  input  logic                        install_req_valid_i,
  input  rbuf_install_req_t           install_req_info_i,

  // Evict interface from sub_bank (delayed clear)
  input  logic                        evict_req_valid_i,
  input  rbuf_evict_req_t             evict_req_info_i,

  // Install delayed-data info
  input  logic                        data_req_valid_i,
  input  rbuf_data_req_t              data_req_info_i,

  // Clear request from reqq
  input  rbuf_clear_req_t             clear_req_info_i,

  // Read request interface
  output logic                        rd_req_ready_o,
  input  logic                        rd_req_valid_i,
  input  rbuf_rd_req_t                rd_req_info_i,

  // Read response interface
  input  logic                        rd_rsp_ready_i,
  output logic                        rd_rsp_valid_o,
  output rbuf_rd_rsp_t                rd_rsp_info_o,

  // State output to reqq
  output rbuf_state_t                 state_o,

  // Performance monitor
  input  perfmon_req_info_t           perfmon_req_info_i,
  output perfmon_rsp_info_t           perfmon_rsp_info_o,

  // ESRs and constants
  input  logic [RamDelaySize-1:0]     esr_ram_delay_i,
  input  logic                        esr_l2_rbuf_enable_i,
  input  logic                        esr_scp_rbuf_enable_i,
  input  logic [BankIdSize-1:0]       my_bank_id_i
);

  // Struct ports carry trans_id/reqq_id fields used only for assertion/trace;
  // rbuf reads address/scp/data subfields only.
  /* verilator lint_off UNUSEDSIGNAL */  // trans_id/reqq_id struct fields unused by rbuf logic
  // rst_ni used in both async reset sensitivity and assertion guards
  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert, used in both always_ff and assertions

  // ── Helpers ───────────────────────────────────────────
  function automatic logic [PaSize-1:0] get_line_addr(
    logic [PaSize-1:0]      addr,
    logic [BankIdSize-1:0]  bank_id
  );
    return {addr[PaSize-1:SubBankLsb], bank_id, {BankLsb{1'b0}}};
  endfunction

  logic esr_rbuf_enable;
  assign esr_rbuf_enable = esr_l2_rbuf_enable_i | esr_scp_rbuf_enable_i;

  logic esr_ram_delay_2, esr_ram_delay_3, esr_ram_delay_4;
  assign esr_ram_delay_2 = (esr_ram_delay_i == RamDelaySize'(2));
  assign esr_ram_delay_3 = (esr_ram_delay_i == RamDelaySize'(3));
  assign esr_ram_delay_4 = (esr_ram_delay_i == RamDelaySize'(4));

  // ── Gated inputs ──────────────────────────────────────
  logic invalidate_valid_d0;
  logic install_valid_d0;
  logic evict_valid_d0;
  assign invalidate_valid_d0 = invalidate_req_valid_i && esr_rbuf_enable;
  assign install_valid_d0    = install_req_valid_i    && esr_rbuf_enable;
  assign evict_valid_d0      = evict_req_valid_i      && esr_rbuf_enable;

  // ── Buffer storage ────────────────────────────────────
  logic [RbufSize-1:0]                rbuf_mem_valid_q;
  logic [PaSize-1:0]                  rbuf_addr_mem_q  [RbufSize];
  logic [LineDataSize-1:0]            rbuf_data_mem_q  [RbufSize];
  logic [RbufSize-1:0]               rbuf_scp_mem_q;

  // ── Match logic ───────────────────────────────────────
  logic                    match_install_valid;
  logic [RbufIdSize-1:0]   match_install_id;
  logic [RbufSize-1:0]     match_install;
  logic                    match_evict_valid;
  logic [RbufIdSize-1:0]   match_evict_id;
  logic [RbufSize-1:0]     match_evict;

  always_comb begin
    match_install_valid = '0;
    match_install_id    = '0;
    match_install       = '0;
    match_evict_valid   = '0;
    match_evict_id      = '0;
    match_evict         = '0;
    for (int unsigned entry = 0; entry < RbufSize; entry++) begin
      if (install_valid_d0 && rbuf_mem_valid_q[entry] &&
          (install_req_info_i.scp == rbuf_scp_mem_q[entry]) &&
          (rbuf_addr_mem_q[entry][PaSize-1:SubBankLsb] ==
           install_req_info_i.address[PaSize-1:SubBankLsb])) begin
        match_install_valid      = 1'b1;
        match_install_id         = RbufIdSize'(entry);
        match_install[entry]     = 1'b1;
      end
      if (evict_valid_d0 && rbuf_mem_valid_q[entry] &&
          (evict_req_info_i.scp == rbuf_scp_mem_q[entry]) &&
          (rbuf_addr_mem_q[entry][PaSize-1:SubBankLsb] ==
           evict_req_info_i.address[PaSize-1:SubBankLsb])) begin
        match_evict_valid      = 1'b1;
        match_evict_id         = RbufIdSize'(entry);
        match_evict[entry]     = 1'b1;
      end
    end
  end

  // ── Actions ───────────────────────────────────────────
  logic rbuf_full_q;

  logic rbuf_install_d0;
  logic rbuf_clear_evict_d0;
  assign rbuf_install_d0     = install_valid_d0 && !match_install_valid;
  assign rbuf_clear_evict_d0 = match_evict_valid;

  // ── LRU arbiter ───────────────────────────────────────
  logic [RbufSize-1:0]   lru_grant;
  logic [RbufIdSize-1:0] lru_entry_id;

  // Forward-declare for pending install tracking
  logic [RbufSize-1:0] pending_install_xx;

  logic                  rbuf_mru_update_d0;
  logic [RbufSize-1:0]   match_fast_rd_req_d1;
  logic                  match_fast_rd_req_valid_d1;
  assign rbuf_mru_update_d0 = match_fast_rd_req_valid_d1;

  logic [RbufSize-1:0] lru_install_bid;
  assign lru_install_bid = ~(rbuf_mem_valid_q | pending_install_xx) |
                           ({RbufSize{rbuf_full_q}} & ~pending_install_xx);

  logic [RbufSize-1:0] lru_bid;
  assign lru_bid = rbuf_install_d0 ? lru_install_bid : match_fast_rd_req_d1;

  logic lru_en;
  assign lru_en = rbuf_install_d0 || rbuf_mru_update_d0;

  prim_arb_lru #(
    .NumClients (RbufSize)
  ) u_lru (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (lru_bid),
    .stall_i  (!lru_en),
    .grant_o  (lru_grant),
    .winner_o (lru_entry_id)
  );

  // ── Pending install pipeline (d0 → d7) ────────────────
  // Tracks install in-flight through the RAM read pipeline.
  // Each stage can be squashed by eviction or cleared by reqq.

  logic squash_d1, squash_d2, squash_d3, squash_d4, squash_d5, squash_d6, squash_d7;

  logic [RbufSize-1:0] pend_d0;
  logic [RbufSize-1:0] pend_q  [7]; // pend_q[0]=d1 .. pend_q[6]=d7
  logic                pend_scp_d0;
  logic                pend_scp_q [7];
  logic [PaSize-1:0]   pend_addr_d0;
  logic [PaSize-1:0]   pend_addr_q [7];

  assign pend_d0      = {RbufSize{rbuf_install_d0}} & lru_grant;
  assign pend_scp_d0  = install_req_info_i.scp;
  assign pend_addr_d0 = install_req_info_i.address;

  // Pipeline stages with clear and squash
  // d1 has no clear (see comments in original)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) pend_q[0] <= '0;
    else if (|pend_q[0] || |pend_d0) pend_q[0] <= pend_d0;
  end
  always_ff @(posedge clk_i) begin
    if (|pend_q[0] || |pend_d0) begin
      pend_scp_q[0]  <= pend_scp_d0;
      pend_addr_q[0] <= pend_addr_d0;
    end
  end

  // d2..d7: clear by reqq clear_pending_install_hot, squash by eviction
  // Squash at source stage: d(N+1) <= d(N) & ~squash_d(N)
  for (genvar s = 1; s < 7; s++) begin : gen_pend
    logic squash_this;
    assign squash_this = (s == 1) ? squash_d1 : (s == 2) ? squash_d2 :
                         (s == 3) ? squash_d3 : (s == 4) ? squash_d4 :
                         (s == 5) ? squash_d5 : squash_d6;

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) pend_q[s] <= '0;
      else if (|pend_q[s] || |pend_q[s-1])
        pend_q[s] <= pend_q[s-1] &
                     ~clear_req_info_i.rbuf_clear_pending_install_hot &
                     {RbufSize{~squash_this}};
    end
    always_ff @(posedge clk_i) begin
      if (|pend_q[s] || |pend_q[s-1]) begin
        pend_scp_q[s]  <= pend_scp_q[s-1];
        pend_addr_q[s] <= pend_addr_q[s-1];
      end
    end
  end

  // ── Pending install pipe arrays for state output ──────
  logic [RbufPendingInstallSize-1:0][RbufSize-1:0] pend_pipe_valid;
  logic [RbufPendingInstallSize-1:0]               pend_pipe_scp;
  logic [RbufPendingInstallSize-1:0][PaSize-1:0]   pend_pipe_address;

  // Per-stage explicit assigns (no generate loop — matches original exactly)
  assign pend_pipe_valid[0]   = '0; // squash d0 to reqq
  assign pend_pipe_valid[1]   = pend_q[0];
  assign pend_pipe_valid[2]   = pend_q[1];
  assign pend_pipe_valid[3]   = pend_q[2];
  assign pend_pipe_valid[4]   = pend_q[3];
  assign pend_pipe_valid[5]   = pend_q[4];
  assign pend_pipe_valid[6]   = pend_q[5] & {RbufSize{!esr_ram_delay_2}};
  assign pend_pipe_valid[7]   = pend_q[6] & {RbufSize{!esr_ram_delay_2}};

  assign pend_pipe_scp[0]     = '0;
  assign pend_pipe_scp[1]     = pend_scp_q[0];
  assign pend_pipe_scp[2]     = pend_scp_q[1];
  assign pend_pipe_scp[3]     = pend_scp_q[2];
  assign pend_pipe_scp[4]     = pend_scp_q[3];
  assign pend_pipe_scp[5]     = pend_scp_q[4];
  assign pend_pipe_scp[6]     = pend_scp_q[5];
  assign pend_pipe_scp[7]     = pend_scp_q[6];

  assign pend_pipe_address[0] = '0;
  assign pend_pipe_address[1] = get_line_addr(pend_addr_q[0], my_bank_id_i);
  assign pend_pipe_address[2] = get_line_addr(pend_addr_q[1], my_bank_id_i);
  assign pend_pipe_address[3] = get_line_addr(pend_addr_q[2], my_bank_id_i);
  assign pend_pipe_address[4] = get_line_addr(pend_addr_q[3], my_bank_id_i);
  assign pend_pipe_address[5] = get_line_addr(pend_addr_q[4], my_bank_id_i);
  assign pend_pipe_address[6] = get_line_addr(pend_addr_q[5], my_bank_id_i);
  assign pend_pipe_address[7] = get_line_addr(pend_addr_q[6], my_bank_id_i);

  // ── Squash logic ──────────────────────────────────────
  assign squash_d1 = evict_valid_d0 && |pend_q[0] &&
    (evict_req_info_i.scp == pend_scp_q[0]) &&
    (evict_req_info_i.address[PaSize-1:SubBankLsb] == pend_addr_q[0][PaSize-1:SubBankLsb]);
  assign squash_d2 = evict_valid_d0 && |pend_q[1] &&
    (evict_req_info_i.scp == pend_scp_q[1]) &&
    (evict_req_info_i.address[PaSize-1:SubBankLsb] == pend_addr_q[1][PaSize-1:SubBankLsb]);
  assign squash_d3 = evict_valid_d0 && |pend_q[2] &&
    (evict_req_info_i.scp == pend_scp_q[2]) &&
    (evict_req_info_i.address[PaSize-1:SubBankLsb] == pend_addr_q[2][PaSize-1:SubBankLsb]);
  assign squash_d4 = evict_valid_d0 && |pend_q[3] &&
    (evict_req_info_i.scp == pend_scp_q[3]) &&
    (evict_req_info_i.address[PaSize-1:SubBankLsb] == pend_addr_q[3][PaSize-1:SubBankLsb]);
  assign squash_d5 = evict_valid_d0 && |pend_q[4] &&
    (evict_req_info_i.scp == pend_scp_q[4]) &&
    (evict_req_info_i.address[PaSize-1:SubBankLsb] == pend_addr_q[4][PaSize-1:SubBankLsb]);
  assign squash_d6 = evict_valid_d0 && |pend_q[5] &&
    (evict_req_info_i.scp == pend_scp_q[5]) &&
    (evict_req_info_i.address[PaSize-1:SubBankLsb] == pend_addr_q[5][PaSize-1:SubBankLsb]);
  assign squash_d7 = evict_valid_d0 && |pend_q[6] &&
    (evict_req_info_i.scp == pend_scp_q[6]) &&
    (evict_req_info_i.address[PaSize-1:SubBankLsb] == pend_addr_q[6][PaSize-1:SubBankLsb]);

  // ── Pending install evict/clear aggregation ───────────
  logic [RbufSize-1:0] pend_evict_ram2, pend_evict_ram3, pend_evict_ram4;
  assign pend_evict_ram2 =
    ({RbufSize{squash_d1}} & pend_q[0]) |
    ({RbufSize{squash_d2}} & pend_q[1]) |
    ({RbufSize{squash_d3}} & pend_q[2]) |
    ({RbufSize{squash_d4}} & pend_q[3]) |
    ({RbufSize{squash_d5}} & pend_q[4]);
  assign pend_evict_ram3 = pend_evict_ram2 |
    ({RbufSize{squash_d6}} & pend_q[5]);
  assign pend_evict_ram4 = pend_evict_ram3 |
    ({RbufSize{squash_d7}} & pend_q[6]);

  logic [RbufSize-1:0] pend_evict_xx;
  assign pend_evict_xx =
    ({RbufSize{esr_ram_delay_2}} & pend_evict_ram2) |
    ({RbufSize{esr_ram_delay_3}} & pend_evict_ram4) |
    ({RbufSize{esr_ram_delay_4}} & pend_evict_ram4);

  logic [RbufSize-1:0] pend_clear_xx;
  assign pend_clear_xx = clear_req_info_i.rbuf_clear_pending_install_hot | pend_evict_xx;

  // pending_install_dc: the install that completes this cycle
  logic [RbufSize-1:0] pending_install_dc;
  assign pending_install_dc =
    ~clear_req_info_i.rbuf_clear_pending_install_hot &
    (({RbufSize{esr_ram_delay_2 && !squash_d5}} & pend_q[4]) |
     ({RbufSize{esr_ram_delay_3 && !squash_d7}} & pend_q[6]) |
     ({RbufSize{esr_ram_delay_4 && !squash_d7}} & pend_q[6]));

  logic any_pend_evict_xx;
  assign any_pend_evict_xx = |pend_evict_xx;

  // One-hot to ID for pending_install_evict
  logic [RbufIdSize-1:0] pend_evict_id_xx;
  always_comb begin
    pend_evict_id_xx = '0;
    for (int unsigned entry = 0; entry < RbufSize; entry++) begin
      if (pend_evict_xx[entry]) pend_evict_id_xx = RbufIdSize'(entry);
    end
  end

  // pending_install_xx accumulator
  logic [RbufSize-1:0] pending_install_next_xx;
  assign pending_install_next_xx = (pending_install_xx | pend_d0) &
                                   ~pending_install_dc & ~pend_clear_xx;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) pending_install_xx <= '0;
    else if (|pend_d0 || |pending_install_dc || |pend_clear_xx)
      pending_install_xx <= pending_install_next_xx;
  end

  // ── Victim logic ──────────────────────────────────────
  logic rbuf_capacity_evict_d0;
  logic rbuf_evict_d0;
  logic victim_d0;

  assign rbuf_capacity_evict_d0 = rbuf_install_d0 && rbuf_full_q;
  assign rbuf_evict_d0          = rbuf_clear_evict_d0 || rbuf_capacity_evict_d0;
  assign victim_d0              = any_pend_evict_xx || rbuf_evict_d0;

  logic [RbufIdSize-1:0] victim_id_d0;
  logic [PaSize-1:0]     victim_address_d0;
  logic                  victim_scp_d0;

  always_comb begin
    if (any_pend_evict_xx) begin
      victim_id_d0      = pend_evict_id_xx;
      victim_address_d0 = get_line_addr(evict_req_info_i.address, my_bank_id_i);
      victim_scp_d0     = evict_req_info_i.scp;
    end else if (rbuf_capacity_evict_d0) begin
      victim_id_d0      = lru_entry_id;
      victim_address_d0 = get_line_addr(rbuf_addr_mem_q[lru_entry_id], my_bank_id_i);
      victim_scp_d0     = rbuf_scp_mem_q[lru_entry_id];
    end else if (rbuf_clear_evict_d0) begin
      victim_id_d0      = match_evict_id;
      victim_address_d0 = get_line_addr(rbuf_addr_mem_q[match_evict_id], my_bank_id_i);
      victim_scp_d0     = rbuf_scp_mem_q[match_evict_id];
    end else begin
      victim_id_d0      = '0;
      victim_address_d0 = '0;
      victim_scp_d0     = '0;
    end
  end

  // ── Valid / full ──────────────────────────────────────
  logic [RbufSize-1:0] rbuf_mem_valid_d;
  logic                rbuf_full_d;

  assign rbuf_mem_valid_d =
    (rbuf_mem_valid_q | pending_install_dc)
    & ~clear_req_info_i.rbuf_clear_mem_valid_hot
    & ~({RbufSize{rbuf_clear_evict_d0}}    & match_evict)
    & ~({RbufSize{rbuf_capacity_evict_d0}} & lru_grant)
    & ~({RbufSize{invalidate_valid_d0}});

  assign rbuf_full_d = &(rbuf_mem_valid_d | pending_install_next_xx);

  logic update_valid;
  assign update_valid = rbuf_install_d0 || (|pend_clear_xx) || (|pending_install_dc) ||
                        rbuf_clear_evict_d0 ||
                        (|clear_req_info_i.rbuf_clear_mem_valid_hot) ||
                        invalidate_valid_d0;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rbuf_mem_valid_q <= '0;
      rbuf_full_q      <= 1'b0;
    end else if (update_valid) begin
      rbuf_mem_valid_q <= rbuf_mem_valid_d;
      rbuf_full_q      <= rbuf_full_d;
    end
  end

  // ── Rbuf memory write (at _dc stage) ──────────────────
  logic              install_dc;
  logic [PaSize-1:0] install_line_addr_dc;

  assign install_dc           = |pending_install_dc;
  assign install_line_addr_dc = get_line_addr(data_req_info_i.address, my_bank_id_i);

  for (genvar i = 0; i < int'(RbufSize); i++) begin : gen_rbuf_mem
    always_ff @(posedge clk_i) begin
      if (pending_install_dc[i]) begin
        rbuf_addr_mem_q[i] <= install_line_addr_dc;
        rbuf_data_mem_q[i] <= data_req_info_i.data;
        rbuf_scp_mem_q[i]  <= data_req_info_i.scp;
      end
    end
  end

  // ── State output ──────────────────────────────────────
  always_comb begin
    for (int unsigned i = 0; i < RbufSize; i++) begin
      state_o.rbuf_mem[i] = rbuf_addr_mem_q[i];
    end
  end
  assign state_o.rbuf_mem_valid = rbuf_mem_valid_q;
  assign state_o.rbuf_mem_scp   = rbuf_scp_mem_q;

  assign state_o.rbuf_evict         = victim_d0;
  assign state_o.rbuf_evict_id      = victim_id_d0;
  assign state_o.rbuf_evict_scp     = victim_scp_d0;
  assign state_o.rbuf_evict_address = victim_address_d0;

  // One-hot to ID for pending_install_dc
  logic [RbufIdSize-1:0] install_id_dc;
  always_comb begin
    install_id_dc = '0;
    for (int unsigned ii = 0; ii < RbufSize; ii++) begin
      if (pending_install_dc[ii]) install_id_dc = RbufIdSize'(ii);
    end
  end

  assign state_o.rbuf_install         = install_dc;
  assign state_o.rbuf_install_id      = install_id_dc;
  assign state_o.rbuf_install_scp     = data_req_info_i.scp;
  assign state_o.rbuf_install_address = install_line_addr_dc;

  assign state_o.rbuf_pending_install         = rbuf_install_d0;
  assign state_o.rbuf_pending_install_scp     = install_req_info_i.scp;
  assign state_o.rbuf_pending_install_id      = lru_entry_id;
  assign state_o.rbuf_pending_install_address = get_line_addr(install_req_info_i.address, my_bank_id_i);

  assign state_o.rbuf_pending_install_pipe_valid   = pend_pipe_valid;
  assign state_o.rbuf_pending_install_pipe_scp     = pend_pipe_scp;
  assign state_o.rbuf_pending_install_pipe_address = pend_pipe_address;

  // ── Rd req fast match (for MRU update) ────────────────
  logic                  match_fast_rd_req_valid;
  logic [RbufSize-1:0]   match_fast_rd_req;

  always_comb begin
    match_fast_rd_req_valid = '0;
    match_fast_rd_req       = '0;
    for (int unsigned entry = 0; entry < RbufSize; entry++) begin
      if (esr_rbuf_enable && rd_req_valid_i &&
          (entry[RbufIdSize-1:0] == rd_req_info_i.rbuf_id)) begin
        match_fast_rd_req_valid  = rbuf_mem_valid_q[entry];
        match_fast_rd_req[entry] = 1'b1;
      end
    end
  end

  // Delay MRU update by 1 cycle to cut timing path
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) match_fast_rd_req_valid_d1 <= 1'b0;
    else if (match_fast_rd_req_valid_d1 || match_fast_rd_req_valid)
      match_fast_rd_req_valid_d1 <= match_fast_rd_req_valid;
  end
  always_ff @(posedge clk_i) begin
    if (match_fast_rd_req_valid)
      match_fast_rd_req_d1 <= match_fast_rd_req;
  end

  // ── Rd rsp FIFO ──────────────────────────────────────
  localparam int unsigned FifoWidth = $bits(rbuf_rd_rsp_t);

  rbuf_rd_rsp_t  fifo_wdata_packed;
  assign fifo_wdata_packed.trans_id  = rd_req_info_i.trans_id;
  assign fifo_wdata_packed.reqq_id   = rd_req_info_i.reqq_id;
  assign fifo_wdata_packed.l3_source = rd_req_info_i.l3_source;
  assign fifo_wdata_packed.data      = rbuf_data_mem_q[rd_req_info_i.rbuf_id];

  logic                fifo_push, fifo_full, fifo_pop, fifo_valid;
  logic [FifoWidth-1:0] fifo_wdata, fifo_rdata;

  assign fifo_push  = rd_req_valid_i && rd_req_ready_o && esr_rbuf_enable;
  assign fifo_wdata = fifo_wdata_packed;
  assign fifo_pop   = rd_rsp_valid_o && rd_rsp_ready_i;

  logic [RbufFifoSize:0] fifo_level_unused;

  prim_fifo_reg #(
    .Width (FifoWidth),
    .Depth (RbufFifoSize)
  ) u_fifo (
    .clk_i   (clk_i),
    .rst_ni  (rst_ni),
    .push_i  (fifo_push),
    .wdata_i (fifo_wdata),
    .full_o  (fifo_full),
    .pop_i   (fifo_pop),
    .valid_o (fifo_valid),
    .rdata_o (fifo_rdata),
    .level_o (fifo_level_unused)
  );

  assign rd_req_ready_o = !fifo_full && esr_rbuf_enable;
  assign rd_rsp_valid_o = fifo_valid;
  assign rd_rsp_info_o  = fifo_rdata;

  // ── Performance monitor ───────────────────────────────
  logic perfmon_rbuf_valid;
  assign perfmon_rbuf_valid = rd_req_valid_i & rd_req_ready_o;

  logic [1:0] perfmon_rbuf_d, perfmon_rbuf_q;
  assign perfmon_rbuf_d = perfmon_req_info_i.ev_mode[PerfmonEvRbuf]
    ? {perfmon_rbuf_valid & esr_l2_rbuf_enable_i  & ~rd_req_info_i.scp,
       perfmon_rbuf_valid & esr_scp_rbuf_enable_i &  rd_req_info_i.scp}
    : '0;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) perfmon_rbuf_q <= '0;
    else if (perfmon_req_info_i.cgater) perfmon_rbuf_q <= perfmon_rbuf_d;
  end

  always_comb begin
    perfmon_rsp_info_o = '0;
    {perfmon_rsp_info_o.ev_rsp.ev_rbuf_l2_hit,
     perfmon_rsp_info_o.ev_rsp.ev_rbuf_scp_hit} = perfmon_rbuf_q;
  end

  // ── Assertions ────────────────────────────────────────
  // synthesis translate_off
`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    if (rst_ni && (match_install & (match_install - RbufSize'(1))) != '0)
      $error("ERROR_RBUF_multi_match_install");
    if (rst_ni && (match_evict & (match_evict - RbufSize'(1))) != '0)
      $error("ERROR_RBUF_multi_match_evict");
    if (rst_ni && install_valid_d0 && evict_valid_d0)
      $error("ERROR_RBUF_if_install_no_evict");
    if (rst_ni && (lru_grant & (lru_grant - RbufSize'(1))) != '0)
      $error("ERROR_RBUF_multi_lru_grant");
    if (rst_ni && lru_en && (lru_grant == '0))
      $error("ERROR_RBUF_install_must_have_grant");
    if (rst_ni && (pending_install_dc & (pending_install_dc - RbufSize'(1))) != '0)
      $error("ERROR_RBUF_multi_pending_install_dc");
    if (rst_ni && |pending_install_dc && !data_req_valid_i)
      $error("ERROR_RBUF_mem_update_valid_check_dc");
    if (rst_ni && fifo_push && fifo_full)
      $error("ERROR_RBUF_fifo_overflow");
  end
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni) $onehot0(match_install))
  else $error("ERROR_RBUF_multi_match_install");
  assert property (@(posedge clk_i) disable iff (!rst_ni) $onehot0(match_evict))
  else $error("ERROR_RBUF_multi_match_evict");
  assert property (@(posedge clk_i) disable iff (!rst_ni) install_valid_d0 |-> !evict_valid_d0)
  else $error("ERROR_RBUF_if_install_no_evict");
  assert property (@(posedge clk_i) disable iff (!rst_ni) $onehot0(lru_grant))
  else $error("ERROR_RBUF_multi_lru_grant");
  assert property (@(posedge clk_i) disable iff (!rst_ni) lru_en |-> (lru_grant != '0))
  else $error("ERROR_RBUF_install_must_have_grant");
  assert property (@(posedge clk_i) disable iff (!rst_ni) $onehot0(pending_install_dc))
  else $error("ERROR_RBUF_multi_pending_install_dc");
  assert property (@(posedge clk_i) disable iff (!rst_ni) |pending_install_dc |-> data_req_valid_i)
  else $error("ERROR_RBUF_mem_update_valid_check_dc");
  assert property (@(posedge clk_i) disable iff (!rst_ni) fifo_push |-> !fifo_full)
  else $error("ERROR_RBUF_fifo_overflow");
`endif
  // synthesis translate_on

  /* verilator lint_on SYNCASYNCNET */

endmodule : shirecache_pipe_rbuf
/* verilator lint_on UNUSEDSIGNAL */
