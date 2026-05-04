// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX depth buffer.
//
// 64-entry direct-mapped depth cache with LRU replacement.
// Manages depth line fetch/spill to the shire cache via ET-Link,
// provides read/write access for the depth test unit, and supports
// flush and debug interfaces.
//
// Two clock domains: clk_i (system) and clk_rbox_i (gated rbox clock).
// Uses prim_ram_2p for the 64×512-bit depth data store.

module rbox_depth_buffer
  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  /* verilator lint_off UNUSEDSIGNAL */ // Packed structs carry unused fields (rsp_t.size/qwen, address low bits, dbg_ram_ptr MSB)
  /* verilator lint_off UNOPTFLAT */   // db_fetch_ptr_o feeds back into LRU/tag logic — acyclic per field
  /* verilator lint_off WIDTHEXPAND */  // Bank index narrower than vector; address concat narrower than PaSize
  /* verilator lint_off WIDTHTRUNC */   // Debug metadata concat wider than port (matches original truncation)
  /* verilator lint_off UNUSEDPARAM */  // TagSz defined for clarity even if only TagMsb:TagLsb range used
(
  // System
  input  logic                       clk_i,
  input  logic                       clk_rbox_i,
  input  logic                       rst_ni,

  // Configuration
  input  ram_cfg_t                   ram_cfg_i,
  input  dft_t                       dft_i,

  // Request to Shire Cache
  output etlink_pkg::req_t           db_sc_req_o,
  output logic [ScBanks:0]           db_sc_req_valid_o,
  input  logic                       db_sc_req_ready_i,

  // Response from Shire Cache
  input  etlink_pkg::rsp_t           sc_db_rsp_i,
  input  logic                       sc_db_rsp_valid_i,

  // Fetch interface with test
  input  logic [PaSize-1:0]          db_fetch_address_i,
  input  logic [$clog2(QuadBufSz)-1:0] db_fetch_quad_i,
  input  logic                       db_fetch_valid_i,
  output logic [DepthBufIdxSz-1:0]   db_fetch_ptr_o,
  output logic                       db_fetch_done_o,

  // Read interface with test
  input  logic [DepthBufIdxSz-1:0]   db_read_ptr_i,
  input  logic                       db_read_valid_i,
  output logic [DepthLineSz-1:0]     db_read_data_o,
  output logic                       db_read_data_valid_o,

  input  logic [DepthBufIdxSz-1:0]   db_write_ptr_i,
  input  logic [DepthLineSz-1:0]     db_write_data_i,
  input  logic                       db_write_valid_i,
  output logic                       db_write_done_o,

  input  logic [DepthBufIdxSz-1:0]   db_dealloc_ptr_i,
  input  logic [$clog2(QuadBufSz)-1:0] db_dealloc_quad_i,
  input  logic                       db_dealloc_valid_i,

  input  logic                       db_flush_i,
  output logic                       db_flush_done_o,

  // Debug interface
  input  logic [DbgPtrIdxSz-1:0]    dbg_ram_ptr_i,

  input  logic                       dbg_db_read_valid_i,
  output logic [DepthLineSz-1:0]     dbg_db_read_data_o,

  input  logic [DepthLineSz-1:0]     dbg_db_write_data_i,
  input  logic                       dbg_db_write_valid_i,

  output logic [DbgDbMetaDataSz-1:0] dbg_db_meta_read_data_o,

  input  logic [DbgDbMetaDataSz-1:0] dbg_db_meta_write_data_i,
  input  logic                       dbg_db_meta_write_valid_i
);

  // Cache geometry constants
  localparam int unsigned BufSz       = DepthBufSz;       // 64
  localparam int unsigned BufIdxSz    = DepthBufIdxSz;    // 6
  localparam int unsigned QuadIdxSz   = $clog2(QuadBufSz);

  // Bank select (address bits [7:6] for 4 banks, 64-byte lines)
  localparam int unsigned BankLsb     = 6;   // $clog2(64)
  localparam int unsigned BankIdSize  = $clog2(ScBanks);  // 2

  // Tag range: PA_MSB:($clog2(DEPTH_LINE_SZ)-3) = 39:6
  localparam int unsigned TagLsb      = $clog2(DepthLineSz) - 3; // 6
  localparam int unsigned TagMsb      = PaSize - 1;              // 39
  localparam int unsigned TagSz       = TagMsb - TagLsb + 1;    // 34

  // ── Spill/fill pending state (clk_rbox_i domain) ──────

  logic spill_pending_q;
  logic spill_pending_d;
  logic spill_pending_en;

  logic fetch_accept;
  logic fetch_fill_done;
  logic fetch_fill_pending_q;
  logic fetch_fill_pending_d;
  logic fetch_fill_pending_en;

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni) begin
      spill_pending_q      <= 1'b0;
      fetch_fill_pending_q <= 1'b0;
    end else begin
      if (spill_pending_en)      spill_pending_q      <= spill_pending_d;
      if (fetch_fill_pending_en) fetch_fill_pending_q <= fetch_fill_pending_d;
    end
  end

  // ── LRU array (clk_rbox_i domain) ─────────────────────

  logic [BufSz-1:0][BufIdxSz-1:0] lru_array_q;
  logic [BufSz-1:0][BufIdxSz-1:0] lru_array_d;
  logic [BufSz-1:0]               lru_array_en;

  logic [BufIdxSz-1:0]            lru_ptr;
  logic [BufSz-1:0]               lru_match;
  logic [BufSz-1:0]               update_lru;

  for (genvar g = 0; g < int'(BufSz); g++) begin : gen_lru
    always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
      if (!rst_ni)
        lru_array_q[g] <= BufIdxSz'(g);
      else if (lru_array_en[g])
        lru_array_q[g] <= lru_array_d[g];
    end
  end

  // ── Tag/valid/dirty/reserved arrays (clk_i domain) ────

  logic [BufSz-1:0]               tag_match;
  logic [BufIdxSz-1:0]            match_ptr;

  logic [BufSz-1:0][TagMsb:TagLsb]   tags_q;
  logic [BufSz-1:0][TagMsb:TagLsb]   tags_d;
  logic [BufSz-1:0]               tags_en;
  logic [BufSz-1:0]               valid_q;
  logic [BufSz-1:0]               valid_d;
  logic [BufSz-1:0]               valid_en;
  logic [BufSz-1:0]               dirty_q;
  logic [BufSz-1:0]               dirty_d;
  logic [BufSz-1:0]               dirty_en;
  logic [BufSz-1:0]               reserved_q;
  logic [BufSz-1:0]               reserved_d;
  logic [BufSz-1:0]               reserved_en;
  logic [BufSz-1:0][QuadIdxSz-1:0] dealloc_quad_q;
  logic [BufSz-1:0][QuadIdxSz-1:0] dealloc_quad_d;
  logic [BufSz-1:0]               dealloc_quad_en;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      valid_q    <= '0;
      dirty_q    <= '0;
      reserved_q <= '0;
    end else begin
      for (int i = 0; i < int'(BufSz); i++) begin
        if (valid_en[i])    valid_q[i]    <= valid_d[i];
        if (dirty_en[i])    dirty_q[i]    <= dirty_d[i];
        if (reserved_en[i]) reserved_q[i] <= reserved_d[i];
      end
    end
  end

  for (genvar g = 0; g < int'(BufSz); g++) begin : gen_tag_regs
    always_ff @(posedge clk_i) begin
      if (tags_en[g])         tags_q[g]         <= tags_d[g];
      if (dealloc_quad_en[g]) dealloc_quad_q[g] <= dealloc_quad_d[g];
    end
  end

  // ── Spill/flush pointers (clk_rbox_i domain) ──────────

  logic [BufIdxSz-1:0]            spill_ptr_q;
  logic [BufIdxSz-1:0]            spill_ptr_d;
  logic                            spill_ptr_en;

  logic [BufIdxSz-1:0]            flush_ptr_q;
  logic [BufIdxSz-1:0]            flush_ptr_d;
  logic                            flush_ptr_en;
  logic                            flush_done_q;
  logic                            flush_done_d;
  logic                            flush_done_en;
  logic                            flush_spill_done_q;
  logic                            flush_spill_done_d;
  logic                            flush_spill_done_en;

  logic [BufIdxSz-1:0]            spill_pending_acks_q;
  logic [BufIdxSz-1:0]            spill_pending_acks_d;
  logic                            spill_pending_acks_en;

  always_ff @(posedge clk_rbox_i) begin
    if (spill_ptr_en)          spill_ptr_q          <= spill_ptr_d;
    if (flush_ptr_en)          flush_ptr_q          <= flush_ptr_d;
    if (spill_pending_acks_en) spill_pending_acks_q <= spill_pending_acks_d;
    if (flush_done_en)         flush_done_q         <= flush_done_d;
    if (flush_spill_done_en)   flush_spill_done_q   <= flush_spill_done_d;
  end

  // ── Cache request register (clk_rbox_i domain) ────────

  etlink_pkg::req_t               db_sc_req_q;
  etlink_pkg::req_t               db_sc_req_d;
  logic [ScBanks:0]               db_sc_req_valid_q;
  logic [ScBanks:0]               db_sc_req_valid_d;
  logic                            db_sc_req_en;

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      db_sc_req_valid_q <= '0;
    else if (db_sc_req_en)
      db_sc_req_valid_q <= db_sc_req_valid_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (db_sc_req_en) db_sc_req_q <= db_sc_req_d;
  end

  // ── LRU + tag match logic ─────────────────────────────

  always_comb begin
    lru_ptr = lru_array_q[BufSz - 1];

    db_fetch_ptr_o = (|tag_match) ? match_ptr : lru_ptr;

    fetch_accept = db_fetch_valid_i
                && ((|tag_match) || (!reserved_q[lru_ptr] && fetch_fill_done));

    for (int i = 0; i < int'(BufSz); i++)
      lru_match[i] = (lru_array_q[i] == db_fetch_ptr_o);
  end

  for (genvar g = 0; g < int'(BufSz); g++) begin : gen_update_lru
    always_comb begin
      update_lru[g] = (|lru_match[BufSz-1:g]) && fetch_accept;
    end
  end

  always_comb begin
    lru_array_en = '0;
    lru_array_d  = lru_array_q;

    for (int i = int'(BufSz) - 1; i >= 0; i--) begin
      if (update_lru[i]) begin
        lru_array_en[i] = 1'b1;
        lru_array_d[i]  = (i == 0) ? db_fetch_ptr_o : lru_array_q[i - 1];
      end
    end
  end

  always_comb begin
    match_ptr = '0;
    for (int i = 0; i < int'(BufSz); i++) begin
      tag_match[i] = (reserved_q[i] || valid_q[i]) && (db_fetch_address_i[TagMsb:TagLsb] == tags_q[i]);
      match_ptr     = tag_match[i] ? BufIdxSz'(i) : match_ptr;
    end
  end

  // ── Tag/valid/dirty/reserved update logic ─────────────

  logic                       fetch_fill_received;
  logic [BufIdxSz-1:0]       fetch_fill_ptr;
  logic                       spill_ack_received;
  logic                       spill_done;

  /* verilator lint_off UNUSEDSIGNAL */ // Not all rsp_t/req_t bits consumed

  always_comb begin
    tags_en               = '0;
    valid_en              = '0;
    dirty_en              = '0;
    reserved_en           = '0;
    dealloc_quad_en       = '0;
    spill_pending_acks_en = 1'b0;

    db_fetch_done_o       = 1'b0;

    tags_d               = tags_q;
    valid_d              = valid_q;
    dirty_d              = dirty_q;
    reserved_d           = reserved_q;
    dealloc_quad_d       = dealloc_quad_q;
    spill_pending_acks_d = spill_pending_acks_q;

    if (!rst_ni) begin
      spill_pending_acks_en = 1'b1;
      spill_pending_acks_d  = '0;
    end

    if (fetch_fill_done) begin
       tags_en[db_fetch_ptr_o] = 1'b1;
      tags_d[db_fetch_ptr_o]  = db_fetch_address_i[TagMsb:TagLsb];
    end

    if (fetch_accept) begin
          reserved_en[db_fetch_ptr_o] = 1'b1;
      dealloc_quad_en[db_fetch_ptr_o] = 1'b1;

          reserved_d[db_fetch_ptr_o] = 1'b1;
      dealloc_quad_d[db_fetch_ptr_o] = db_fetch_quad_i;

      db_fetch_done_o = 1'b1;
    end

    if (db_dealloc_valid_i && (dealloc_quad_q[db_dealloc_ptr_i] == db_dealloc_quad_i)) begin
       reserved_en[db_dealloc_ptr_i] = 1'b1;
      reserved_d[db_dealloc_ptr_i]  = 1'b0;
    end

    if (db_write_done_o) begin
       dirty_en[db_write_ptr_i] = 1'b1;
      dirty_d[db_write_ptr_i]  = 1'b1;
    end

    if (fetch_fill_received) begin
       valid_en[fetch_fill_ptr] = 1'b1;
      valid_d[fetch_fill_ptr]  = 1'b1;
       dirty_en[fetch_fill_ptr] = 1'b1;
      dirty_d[fetch_fill_ptr]  = 1'b0;
    end

    if (spill_done) begin
       valid_en[spill_ptr_q] = 1'b1;
      valid_d[spill_ptr_q]  = 1'b0;
       dirty_en[spill_ptr_q] = 1'b0;
      dirty_d[spill_ptr_q]  = 1'b0;
      spill_pending_acks_en  = 1'b1;
      spill_pending_acks_d   = spill_pending_acks_q + BufIdxSz'(1);
    end

    if (spill_ack_received) begin
      spill_pending_acks_en = 1'b1;
      spill_pending_acks_d  = spill_pending_acks_q - BufIdxSz'(1);
    end

    if (dbg_db_meta_write_valid_i) begin
               tags_en[dbg_ram_ptr_i[BufIdxSz-1:0]] = 1'b1;
              dirty_en[dbg_ram_ptr_i[BufIdxSz-1:0]] = 1'b1;
              valid_en[dbg_ram_ptr_i[BufIdxSz-1:0]] = 1'b1;
           reserved_en[dbg_ram_ptr_i[BufIdxSz-1:0]] = 1'b1;
       dealloc_quad_en[dbg_ram_ptr_i[BufIdxSz-1:0]] = 1'b1;

      {          tags_d[dbg_ram_ptr_i[BufIdxSz-1:0]],
                dirty_d[dbg_ram_ptr_i[BufIdxSz-1:0]],
                valid_d[dbg_ram_ptr_i[BufIdxSz-1:0]],
             reserved_d[dbg_ram_ptr_i[BufIdxSz-1:0]],
         dealloc_quad_d[dbg_ram_ptr_i[BufIdxSz-1:0]]} = dbg_db_meta_write_data_i;
    end
  end

  // ── Cache request generation ──────────────────────────

  logic                       fetch_spill_read;
  logic                       flush_spill_read;
  logic                       spill_read;
  logic [PaSize-1:0]          spill_address;
  logic                       fetch_fill_write;
  logic [DepthLineSz-1:0]     write_data;
  logic [BufIdxSz-1:0]        write_ptr;
  logic                        write_valid;
  logic [BufIdxSz-1:0]        read_ptr;
  logic [DepthLineSz-1:0]     read_data;
  logic                        read_valid;
  logic                        db_sc_req_accepted;

  always_comb begin
    db_sc_req_accepted = db_sc_req_ready_i || !(|db_sc_req_valid_q);

    fetch_spill_read = db_fetch_valid_i && !dbg_db_read_valid_i && !dbg_db_write_valid_i
                     && !(|tag_match) && valid_q[db_fetch_ptr_o] && dirty_q[db_fetch_ptr_o];

    flush_spill_read = db_flush_i && !dbg_db_read_valid_i && !dbg_db_write_valid_i
                     && valid_q[flush_ptr_q] && dirty_q[flush_ptr_q];

    spill_read = fetch_spill_read || flush_spill_read;

    spill_ptr_en  = spill_read;
    spill_ptr_d   = db_flush_i ? flush_ptr_q : db_fetch_ptr_o;

    spill_done = spill_pending_q && db_sc_req_accepted;

    fetch_fill_write = db_fetch_valid_i && !dbg_db_read_valid_i && !dbg_db_write_valid_i
                     && !(|tag_match) && (spill_done || !fetch_spill_read);

    fetch_fill_done =  fetch_fill_pending_q && db_sc_req_accepted
                    || fetch_fill_write     && db_sc_req_accepted;

    spill_pending_en  = !spill_pending_q && spill_read
                      || spill_done;
    spill_pending_d   = !spill_pending_q && spill_read;

    fetch_fill_pending_en = !fetch_fill_pending_q && !fetch_fill_done
                          || fetch_fill_pending_q &&  db_sc_req_accepted;
    fetch_fill_pending_d  = !fetch_fill_pending_q && fetch_fill_write && !fetch_fill_done;

    db_sc_req_en        = db_sc_req_ready_i;
    db_sc_req_valid_d   = db_sc_req_ready_i ? '0 : db_sc_req_valid_q;
    db_sc_req_d         = '0;

    spill_address = '0;

    if (spill_pending_q && db_sc_req_accepted) begin
      spill_address = {tags_q[spill_ptr_q], 6'b0};

      db_sc_req_d.source    = '0;
      db_sc_req_d.wdata     = 1'b1;
      db_sc_req_d.size      = etlink_pkg::SizeLine;
      db_sc_req_d.qwen      = 4'b1111;
      db_sc_req_d.opcode    = etlink_pkg::ReqWrite;
      db_sc_req_d.subopcode = '0;
      db_sc_req_d.id        = IdSize'(spill_ptr_q) + 8'h80;
      db_sc_req_d.address   = spill_address;
      db_sc_req_d.data      = DataSize'(read_data);

      db_sc_req_valid_d[spill_address[BankLsb +: BankIdSize]] = 1'b1;

      db_sc_req_en = 1'b1;
    end

    if (!fetch_fill_pending_q && fetch_fill_write) begin
      db_sc_req_d.source    = '0;
      db_sc_req_d.wdata     = 1'b0;
      db_sc_req_d.size      = etlink_pkg::SizeLine;
      db_sc_req_d.qwen      = 4'b0000;
      db_sc_req_d.opcode    = etlink_pkg::ReqRead;
      db_sc_req_d.subopcode = '0;
      db_sc_req_d.id        = IdSize'(db_fetch_ptr_o) + 8'h80;
      db_sc_req_d.address   = {db_fetch_address_i[TagMsb:TagLsb], 6'b0};

      db_sc_req_valid_d[db_fetch_address_i[BankLsb +: BankIdSize]] = 1'b1;

      db_sc_req_en = 1'b1;
    end

    db_sc_req_o       = db_sc_req_q;
    db_sc_req_valid_o = db_sc_req_valid_q;
  end

  // ── Memory read/write arbitration ─────────────────────

  always_comb begin
    read_valid =  spill_read
               || db_read_valid_i && valid_q[db_read_ptr_i]
               || dbg_db_read_valid_i;
    read_ptr   = dbg_db_read_valid_i ? dbg_ram_ptr_i[BufIdxSz-1:0]
                                     : (spill_read ? spill_ptr_d : db_read_ptr_i);

    fetch_fill_received = sc_db_rsp_valid_i
                       && (sc_db_rsp_i.opcode == etlink_pkg::RspAckData)
                       &&  sc_db_rsp_i.id[IdSize-1];
    fetch_fill_ptr      = sc_db_rsp_i.id[BufIdxSz-1:0];

    spill_ack_received  = sc_db_rsp_valid_i
                       && (sc_db_rsp_i.opcode == etlink_pkg::RspAck)
                       &&  sc_db_rsp_i.id[IdSize-1];

    write_valid = fetch_fill_received || db_write_valid_i;
    write_ptr   = dbg_db_write_valid_i ? dbg_ram_ptr_i[BufIdxSz-1:0]
                                       : (fetch_fill_received ? fetch_fill_ptr : db_write_ptr_i);
    write_data  = dbg_db_write_valid_i ? dbg_db_write_data_i
                                       : (fetch_fill_received ? sc_db_rsp_i.data[DepthLineSz-1:0] : db_write_data_i);

    db_write_done_o = db_write_valid_i && !fetch_fill_received;

    db_read_data_o = read_data;

    db_read_data_valid_o = db_read_valid_i && !dbg_db_read_valid_i && !spill_read && valid_q[db_read_ptr_i];
  end

  // ── Flush machine ─────────────────────────────────────

  always_comb begin
    flush_ptr_en        = 1'b0;
    flush_done_en       = 1'b0;
    flush_spill_done_en = 1'b0;

    flush_ptr_d          = '0;
    flush_spill_done_d   = 1'b0;
    flush_done_d         = 1'b0;

    if (!rst_ni) begin
      flush_ptr_en = 1'b1;
      flush_ptr_d  = '0;
    end

    if (  (db_flush_i && !flush_spill_done_q)
        && ((     valid_q[flush_ptr_q] &&  dirty_q[flush_ptr_q] && spill_done)
             || (!valid_q[flush_ptr_q] || !dirty_q[flush_ptr_q])))
    begin
      flush_ptr_en  = 1'b1;
      flush_ptr_d   = flush_ptr_q + BufIdxSz'(1);

      flush_spill_done_en  = (flush_ptr_q == '1);
      flush_spill_done_d   = (flush_ptr_q == '1);
    end

    flush_spill_done_en = !rst_ni
                       || flush_spill_done_en
                       || flush_done_q;

    flush_spill_done_d = rst_ni && flush_spill_done_d;

    flush_done_en =  !rst_ni
                  || !flush_done_q && flush_spill_done_q && (spill_pending_acks_q == '0)
                  ||  flush_done_q;
    flush_done_d  = rst_ni && flush_spill_done_q && (spill_pending_acks_q == '0) && !flush_done_q;

    db_flush_done_o = flush_done_q;
  end

  // ── Debug metadata read ───────────────────────────────

  always_comb begin
    dbg_db_read_data_o      = read_data;
    dbg_db_meta_read_data_o = {       valid_q[dbg_ram_ptr_i[BufIdxSz-1:0]],
                                      dirty_q[dbg_ram_ptr_i[BufIdxSz-1:0]],
                                   reserved_q[dbg_ram_ptr_i[BufIdxSz-1:0]],
                               dealloc_quad_q[dbg_ram_ptr_i[BufIdxSz-1:0]],
                                       tags_q[dbg_ram_ptr_i[BufIdxSz-1:0]]};
  end

  /* verilator lint_on UNUSEDSIGNAL */

  // ── Depth data RAM (prim_ram_2p) ──────────────────────

  /* verilator lint_off PINCONNECTEMPTY */ // Write-port rdata and alert intentionally unconnected

  prim_ram_2p #(
    .Width (DepthLineSz),
    .Depth (BufSz)
  ) u_depth_buffer_data (
    .clk_i     (clk_i),
    .rst_ni    (rst_ni),
    // Port A: write
    .a_req_i   (write_valid && !dft_i.ram_wei),
    .a_we_i    (1'b1),
    .a_addr_i  (write_ptr),
    .a_wdata_i (write_data),
    .a_rdata_o (),
    // Port B: read
    .b_req_i   (read_valid),
    .b_we_i    (1'b0),
    .b_addr_i  (read_ptr),
    .b_wdata_i ('0),
    .b_rdata_o (read_data),
    // Technology hooks
    .cfg_i     (ram_cfg_i),
    .dft_i     (dft_i),
    .alert_o   ()
  );

  /* verilator lint_on PINCONNECTEMPTY */

  // ── Assertions ─────────────────────────────────────────

  // synthesis translate_off
  `ifdef VERILATOR
    /* verilator lint_off SYNCASYNCNET */
    always_ff @(posedge clk_i) begin
      if (rst_ni && ((1 << $clog2(BufSz)) != BufSz))
        $error("RBOX Depth Buffer Size must be a power of 2: %0d", BufSz);
    end
    always_ff @(posedge clk_rbox_i) begin
      if (rst_ni && !$onehot0(tag_match))
        $error("RBOX Depth Buffer tag_match must be one-hot-0: %b", tag_match);
      if (rst_ni && flush_spill_read && reserved_q[flush_ptr_q])
        $error("RBOX Depth Buffer flush found reserved entry: %0d", flush_ptr_q);
    end
    /* verilator lint_on SYNCASYNCNET */
  `else
    assert property (@(posedge clk_i) disable iff (!rst_ni)
      (1 << $clog2(BufSz)) == BufSz)
    else $error("RBOX Depth Buffer Size must be a power of 2: %0d", BufSz);

    assert property (@(posedge clk_rbox_i) disable iff (!rst_ni) $onehot0(tag_match))
    else $error("RBOX Depth Buffer tag_match must be one-hot-0: %b", tag_match);

    assert property (@(posedge clk_rbox_i) disable iff (!rst_ni)
      !(flush_spill_read && reserved_q[flush_ptr_q]))
    else $error("RBOX Depth Buffer flush found reserved entry: %0d", flush_ptr_q);
  `endif
  // synthesis translate_on

  /* verilator lint_on UNUSEDPARAM */
  /* verilator lint_on WIDTHTRUNC */
  /* verilator lint_on WIDTHEXPAND */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
