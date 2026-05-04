// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_depth_buffer vs new rbox_depth_buffer.

`include "soc.vh"

module cosim_rbox_depth_buffer_tb (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  // Shared stimulus: request ready
  input  logic                    db_sc_req_ready_i,

  // Shared stimulus: response from Shire Cache (584-bit packed rsp_t / et_link_rsp_info_t)
  input  logic [533:0]            sc_db_rsp_i,       // etlink_pkg::rsp_t width = 534 bits
  input  logic                    sc_db_rsp_valid_i,

  // Shared stimulus: fetch interface
  input  logic [39:0]             db_fetch_address_i,
  input  logic [6:0]              db_fetch_quad_i,
  input  logic                    db_fetch_valid_i,

  // Shared stimulus: read interface
  input  logic [5:0]              db_read_ptr_i,
  input  logic                    db_read_valid_i,

  // Shared stimulus: write interface
  input  logic [5:0]              db_write_ptr_i,
  input  logic [511:0]            db_write_data_i,
  input  logic                    db_write_valid_i,

  // Shared stimulus: dealloc
  input  logic [5:0]              db_dealloc_ptr_i,
  input  logic [6:0]              db_dealloc_quad_i,
  input  logic                    db_dealloc_valid_i,

  // Shared stimulus: flush
  input  logic                    db_flush_i,

  // Shared stimulus: debug
  input  logic [6:0]              dbg_ram_ptr_i,
  input  logic                    dbg_db_read_valid_i,
  input  logic [511:0]            dbg_db_write_data_i,
  input  logic                    dbg_db_write_valid_i,
  input  logic [42:0]             dbg_db_meta_write_data_i,  // DbgDbMetaDataSz = 43
  input  logic                    dbg_db_meta_write_valid_i,

  // ── New module outputs ────────────────────────────────
  output logic [583:0]            new_db_sc_req_o,
  output logic [4:0]              new_db_sc_req_valid_o,
  output logic [5:0]              new_db_fetch_ptr_o,
  output logic                    new_db_fetch_done_o,
  output logic [511:0]            new_db_read_data_o,
  output logic                    new_db_read_data_valid_o,
  output logic                    new_db_write_done_o,
  output logic                    new_db_flush_done_o,
  output logic [511:0]            new_dbg_db_read_data_o,
  output logic [42:0]             new_dbg_db_meta_read_data_o,

  // ── Original module outputs ───────────────────────────
  output logic [583:0]            orig_db_sc_req_o,
  output logic [4:0]              orig_db_sc_req_valid_o,
  output logic [5:0]              orig_db_fetch_ptr_o,
  output logic                    orig_db_fetch_done_o,
  output logic [511:0]            orig_db_read_data_o,
  output logic                    orig_db_read_data_valid_o,
  output logic                    orig_db_write_done_o,
  output logic                    orig_db_flush_done_o,
  output logic [511:0]            orig_dbg_db_read_data_o,
  output logic [42:0]             orig_dbg_db_meta_read_data_o
);

  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  // Adapt reset
  logic reset;
  assign reset = ~rst_ni;

  // DFT/config = disabled (all zeros)
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // ── Original response type wiring ─────────────────────
  et_link_rsp_info_t orig_sc_db_rsp;
  assign orig_sc_db_rsp = et_link_rsp_info_t'(sc_db_rsp_i);

  // ── Original request output ───────────────────────────
  et_link_req_info_t               orig_db_sc_req;
  logic [(`SC_BANKS+1)-1:0]        orig_db_sc_req_valid;
  assign orig_db_sc_req_o       = orig_db_sc_req;
  assign orig_db_sc_req_valid_o = orig_db_sc_req_valid;

  // ── Original debug metadata output (38 bits → zero-extend to 43) ──
  logic [`RBOX_DBG_DB_META_DATA_R] orig_dbg_db_meta_read_data_raw;
  assign orig_dbg_db_meta_read_data_o = 43'(orig_dbg_db_meta_read_data_raw);

  // ── New module response type wiring ───────────────────
  etlink_pkg::rsp_t new_sc_db_rsp;
  assign new_sc_db_rsp = etlink_pkg::rsp_t'(sc_db_rsp_i);

  // ── New request output ────────────────────────────────
  etlink_pkg::req_t new_db_sc_req;
  logic [ScBanks:0]  new_db_sc_req_valid;
  assign new_db_sc_req_o       = new_db_sc_req;
  assign new_db_sc_req_valid_o = new_db_sc_req_valid;

  // ── New module ────────────────────────────────────────

  rbox_depth_buffer u_new (
    .clk_i                    (clk_i),
    .clk_rbox_i               (clk_i),
    .rst_ni                   (rst_ni),
    .ram_cfg_i                (ram_cfg_t'('0)),
    .dft_i                    (dft_t'('0)),
    .db_sc_req_o              (new_db_sc_req),
    .db_sc_req_valid_o        (new_db_sc_req_valid),
    .db_sc_req_ready_i        (db_sc_req_ready_i),
    .sc_db_rsp_i              (new_sc_db_rsp),
    .sc_db_rsp_valid_i        (sc_db_rsp_valid_i),
    .db_fetch_address_i       (db_fetch_address_i),
    .db_fetch_quad_i          (db_fetch_quad_i),
    .db_fetch_valid_i         (db_fetch_valid_i),
    .db_fetch_ptr_o           (new_db_fetch_ptr_o),
    .db_fetch_done_o          (new_db_fetch_done_o),
    .db_read_ptr_i            (db_read_ptr_i),
    .db_read_valid_i          (db_read_valid_i),
    .db_read_data_o           (new_db_read_data_o),
    .db_read_data_valid_o     (new_db_read_data_valid_o),
    .db_write_ptr_i           (db_write_ptr_i),
    .db_write_data_i          (db_write_data_i),
    .db_write_valid_i         (db_write_valid_i),
    .db_write_done_o          (new_db_write_done_o),
    .db_dealloc_ptr_i         (db_dealloc_ptr_i),
    .db_dealloc_quad_i        (db_dealloc_quad_i),
    .db_dealloc_valid_i       (db_dealloc_valid_i),
    .db_flush_i               (db_flush_i),
    .db_flush_done_o          (new_db_flush_done_o),
    .dbg_ram_ptr_i            (dbg_ram_ptr_i),
    .dbg_db_read_valid_i      (dbg_db_read_valid_i),
    .dbg_db_read_data_o       (new_dbg_db_read_data_o),
    .dbg_db_write_data_i      (dbg_db_write_data_i),
    .dbg_db_write_valid_i     (dbg_db_write_valid_i),
    .dbg_db_meta_read_data_o  (new_dbg_db_meta_read_data_o),
    .dbg_db_meta_write_data_i (dbg_db_meta_write_data_i),
    .dbg_db_meta_write_valid_i(dbg_db_meta_write_valid_i)
  );

  // ── Original module ───────────────────────────────────

  // Original debug metadata write input (truncated to 38 bits)
  logic [`RBOX_DBG_DB_META_DATA_R] orig_dbg_db_meta_write_data;
  assign orig_dbg_db_meta_write_data = dbg_db_meta_write_data_i[`RBOX_DBG_DB_META_DATA_R];

  rbox_depth_buffer_orig u_orig (
    .clock                   (clk_i),
    .clock_rbox              (clk_i),
    .reset                   (reset),
    .esr_shire_cache_ram_cfg (orig_ram_cfg),
    .db_sc_req               (orig_db_sc_req),
    .db_sc_req_valid         (orig_db_sc_req_valid),
    .db_sc_req_ready         (db_sc_req_ready_i),
    .sc_db_rsp               (orig_sc_db_rsp),
    .sc_db_rsp_valid         (sc_db_rsp_valid_i),
    .db_fetch_address        (db_fetch_address_i),
    .db_fetch_quad           (db_fetch_quad_i),
    .db_fetch_valid          (db_fetch_valid_i),
    .db_fetch_ptr            (orig_db_fetch_ptr_o),
    .db_fetch_done           (orig_db_fetch_done_o),
    .db_read_ptr             (db_read_ptr_i),
    .db_read_valid           (db_read_valid_i),
    .db_read_data            (orig_db_read_data_o),
    .db_read_data_valid      (orig_db_read_data_valid_o),
    .db_write_ptr            (db_write_ptr_i),
    .db_write_data           (db_write_data_i),
    .db_write_valid          (db_write_valid_i),
    .db_write_done           (orig_db_write_done_o),
    .db_dealloc_ptr          (db_dealloc_ptr_i),
    .db_dealloc_quad         (db_dealloc_quad_i),
    .db_dealloc_valid        (db_dealloc_valid_i),
    .db_flush                (db_flush_i),
    .db_flush_done           (orig_db_flush_done_o),
    .dbg_ram_ptr             (dbg_ram_ptr_i),
    .dbg_db_read_valid       (dbg_db_read_valid_i),
    .dbg_db_read_data        (orig_dbg_db_read_data_o),
    .dbg_db_write_data       (dbg_db_write_data_i),
    .dbg_db_write_valid      (dbg_db_write_valid_i),
    .dbg_db_meta_read_data   (orig_dbg_db_meta_read_data_raw),
    .dbg_db_meta_write_data  (orig_dbg_db_meta_write_data),
    .dbg_db_meta_write_valid (dbg_db_meta_write_valid_i)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Renamed copy of original rbox_depth_buffer (behavioral path only)
// ════════════════════════════════════════════════════════════

module rbox_depth_buffer_orig
(
    // System
    input  logic                            clock,
    input  logic                            clock_rbox,
    input  logic                            reset,

    // Configuration
    input  esr_shire_cache_ram_cfg_t        esr_shire_cache_ram_cfg,

    // Request to Shire Cache
    output et_link_req_info_t               db_sc_req,
    output logic [(`SC_BANKS+1)-1:0]        db_sc_req_valid,
    input  logic                            db_sc_req_ready,

    // Response from Shire Cache
    input  et_link_rsp_info_t               sc_db_rsp,
    input  logic                            sc_db_rsp_valid,

    // Fetch interface with test
    input  logic [`PA_RANGE]                db_fetch_address,
    input  logic [`RBOX_QUAD_BUF_I_R]       db_fetch_quad,
    input  logic                            db_fetch_valid,
    output logic [`RBOX_DEPTH_BUF_I_R]      db_fetch_ptr,
    output logic                            db_fetch_done,

    // Read interface with test
    input  logic [`RBOX_DEPTH_BUF_I_R]      db_read_ptr,
    input  logic                            db_read_valid,
    output logic [`RBOX_DEPTH_LINE_R]       db_read_data,
    output logic                            db_read_data_valid,

    input  logic [`RBOX_DEPTH_BUF_I_R]      db_write_ptr,
    input  logic [`RBOX_DEPTH_LINE_R]       db_write_data,
    input  logic                            db_write_valid,
    output logic                            db_write_done,

    input  logic [`RBOX_DEPTH_BUF_I_R]      db_dealloc_ptr,
    input  logic [`RBOX_QUAD_BUF_I_R]       db_dealloc_quad,
    input  logic                            db_dealloc_valid,

    input  logic                            db_flush,
    output logic                            db_flush_done,

    // Debug interface for Depth Buffer
    input  logic [`RBOX_DBG_PTR_R]          dbg_ram_ptr,

    input  logic                            dbg_db_read_valid,
    output logic [`RBOX_DEPTH_LINE_R]       dbg_db_read_data,

    input  logic [`RBOX_DEPTH_LINE_R]       dbg_db_write_data,
    input  logic                            dbg_db_write_valid,

    output logic [`RBOX_DBG_DB_META_DATA_R] dbg_db_meta_read_data,

    input  logic [`RBOX_DBG_DB_META_DATA_R] dbg_db_meta_write_data,
    input  logic                            dbg_db_meta_write_valid

);

logic spill_pending_ff;
logic spill_pending_nxt;
logic spill_pending_en;

logic fetch_accept;
logic fetch_fill_done;
logic fetch_fill_pending_ff;
logic fetch_fill_pending_nxt;
logic fetch_fill_pending_en;

`RST_EN_FF(clock_rbox, reset, spill_pending_en,      spill_pending_ff,      spill_pending_nxt,      1'b0)
`RST_EN_FF(clock_rbox, reset, fetch_fill_pending_en, fetch_fill_pending_ff, fetch_fill_pending_nxt, 1'b0)

logic [`RBOX_DEPTH_BUF_E_R][`RBOX_DEPTH_BUF_I_R]   lru_array_ff;
logic [`RBOX_DEPTH_BUF_E_R][`RBOX_DEPTH_BUF_I_R]   lru_array_nxt;
logic [`RBOX_DEPTH_BUF_E_R]                        lru_array_en;

logic [`RBOX_DEPTH_BUF_I_R]                        lru_ptr;
logic [`RBOX_DEPTH_BUF_I_R]                        update_ptr;
logic                                              allocate;

logic [`RBOX_DEPTH_BUF_E_R]                        lru_match;
logic [`RBOX_DEPTH_BUF_E_R]                        update_lru;

logic [`RBOX_DEPTH_BUF_E_R]                        tag_match;
logic [`RBOX_DEPTH_BUF_I_R]                        match_ptr;

logic [`RBOX_DEPTH_BUF_E_R][`RBOX_DEPTH_BUF_TAG_R] tags_ff;
logic [`RBOX_DEPTH_BUF_E_R][`RBOX_DEPTH_BUF_TAG_R] tags_nxt;
logic [`RBOX_DEPTH_BUF_E_R]                        tags_en;
logic [`RBOX_DEPTH_BUF_E_R]                        valid_ff;
logic [`RBOX_DEPTH_BUF_E_R]                        valid_nxt;
logic [`RBOX_DEPTH_BUF_E_R]                        valid_en;
logic [`RBOX_DEPTH_BUF_E_R]                        dirty_ff;
logic [`RBOX_DEPTH_BUF_E_R]                        dirty_nxt;
logic [`RBOX_DEPTH_BUF_E_R]                        dirty_en;
logic [`RBOX_DEPTH_BUF_E_R]                        reserved_ff;
logic [`RBOX_DEPTH_BUF_E_R]                        reserved_nxt;
logic [`RBOX_DEPTH_BUF_E_R]                        reserved_en;
logic [`RBOX_DEPTH_BUF_E_R][`RBOX_QUAD_BUF_I_R]    dealloc_quad_ff;
logic [`RBOX_DEPTH_BUF_E_R][`RBOX_QUAD_BUF_I_R]    dealloc_quad_nxt;
logic [`RBOX_DEPTH_BUF_E_R]                        dealloc_quad_en;

logic [`RBOX_DEPTH_BUF_I_R]                        spill_ptr_ff;
logic [`RBOX_DEPTH_BUF_I_R]                        spill_ptr_nxt;
logic                                              spill_ptr_en;

logic [`RBOX_DEPTH_BUF_I_R]                        flush_ptr_ff;
logic [`RBOX_DEPTH_BUF_I_R]                        flush_ptr_nxt;
logic                                              flush_ptr_en;
logic                                              flush_done_ff;
logic                                              flush_done_nxt;
logic                                              flush_done_en;
logic                                              flush_spill_done_ff;
logic                                              flush_spill_done_nxt;
logic                                              flush_spill_done_en;

logic [`RBOX_DEPTH_BUF_I_R]                        spill_pending_acks_ff;
logic [`RBOX_DEPTH_BUF_I_R]                        spill_pending_acks_nxt;
logic                                              spill_pending_acks_en;

    `EN_FF(clock,             tags_en,               tags_ff,               tags_nxt                             )
`RST_EN_FF(clock,      reset, valid_en,              valid_ff,              valid_nxt       ,       '0           )
`RST_EN_FF(clock,      reset, dirty_en,              dirty_ff,              dirty_nxt       ,       '0           )
`RST_EN_FF(clock,      reset, reserved_en,           reserved_ff,           reserved_nxt    ,       '0           )
    `EN_FF(clock,             dealloc_quad_en,       dealloc_quad_ff,       dealloc_quad_nxt                     )
`RST_EN_FF(clock_rbox, reset, lru_array_en,          lru_array_ff,          lru_array_nxt,          lru_array_nxt)
    `EN_FF(clock_rbox,        spill_ptr_en,          spill_ptr_ff,          spill_ptr_nxt                        )
    `EN_FF(clock_rbox,        flush_ptr_en,          flush_ptr_ff,          flush_ptr_nxt                        )
    `EN_FF(clock_rbox,        spill_pending_acks_en, spill_pending_acks_ff, spill_pending_acks_nxt               )
    `EN_FF(clock_rbox,        flush_done_en,         flush_done_ff,         flush_done_nxt                       )
    `EN_FF(clock_rbox,        flush_spill_done_en,   flush_spill_done_ff,   flush_spill_done_nxt                 )

always_comb
begin
    lru_ptr = lru_array_ff[`RBOX_DEPTH_BUF_SZ - 1];

    db_fetch_ptr = (|tag_match) ? match_ptr : lru_ptr;

    fetch_accept =  db_fetch_valid
                 && ((|tag_match) || (!reserved_ff[lru_ptr] && fetch_fill_done));

    for (integer i = 0; i < `RBOX_DEPTH_BUF_SZ; i++)
        lru_match[i] = (lru_array_ff[i] == db_fetch_ptr);
end

genvar entry;

generate

    for (entry = 0; entry < `RBOX_DEPTH_BUF_SZ; entry++)
        always_comb
            update_lru[entry] = (|lru_match[(`RBOX_DEPTH_BUF_SZ-1):entry]) && fetch_accept;

endgenerate

always_comb
begin
    lru_array_en = '0;
    lru_array_nxt = lru_array_ff;

    for (integer i = (`RBOX_DEPTH_BUF_SZ - 1); i >= 0; i--)
    begin
        if (reset)
        begin
            lru_array_en[i]  = 1'b1;
            lru_array_nxt[i] = i;
        end
        else
        begin
            if (update_lru[i])
            begin
                lru_array_en[i]  = 1'b1;
                lru_array_nxt[i] = (i == 0) ? db_fetch_ptr : lru_array_ff[i - 1];
            end
        end
    end
end

always_comb
begin
    match_ptr = '0;
    for (integer i = 0; i < `RBOX_DEPTH_BUF_SZ; i++)
    begin
        tag_match[i] = (reserved_ff[i] || valid_ff[i]) && (db_fetch_address[`RBOX_DEPTH_BUF_TAG_R] == tags_ff[i]);
        match_ptr    = tag_match[i] ? i : match_ptr;
    end

end


logic                       fetch_fill_received;
logic [`RBOX_DEPTH_BUF_I_R] fetch_fill_ptr;
logic                       spill_ack_received;
logic                       spill_done;

always_comb
begin
    tags_en               = '0;
    valid_en              = '0;
    dirty_en              = '0;
    reserved_en           = '0;
    dealloc_quad_en       = '0;
    spill_pending_acks_en = 1'b0;

    db_fetch_done         = 1'b0;

    tags_nxt               = tags_ff;
    valid_nxt              = valid_ff;
    dirty_nxt              = dirty_ff;
    reserved_nxt           = reserved_ff;
    dealloc_quad_nxt       = dealloc_quad_ff;
    spill_pending_acks_nxt = spill_pending_acks_ff;

    if (reset)
    begin
        spill_pending_acks_en  = 1'b1;
        spill_pending_acks_nxt = '0;
    end

    if (fetch_fill_done)
    begin
         tags_en[db_fetch_ptr] = 1'b1;
        tags_nxt[db_fetch_ptr] = db_fetch_address[`RBOX_DEPTH_BUF_TAG_R];
    end

    if (fetch_accept)
    begin
            reserved_en[db_fetch_ptr] = 1'b1;
        dealloc_quad_en[db_fetch_ptr] = 1'b1;

            reserved_nxt[db_fetch_ptr] = 1'b1;
        dealloc_quad_nxt[db_fetch_ptr] = db_fetch_quad;

        db_fetch_done = 1'b1;
    end

    if (db_dealloc_valid && (dealloc_quad_ff[db_dealloc_ptr] == db_dealloc_quad))
    begin
         reserved_en[db_dealloc_ptr] = 1'b1;
        reserved_nxt[db_dealloc_ptr] = 1'b0;
    end

    if (db_write_done)
    begin
         dirty_en[db_write_ptr] = 1'b1;
        dirty_nxt[db_write_ptr] = 1'b1;
    end

    if (fetch_fill_received)
    begin
         valid_en[fetch_fill_ptr] = 1'b1;
        valid_nxt[fetch_fill_ptr] = 1'b1;
         dirty_en[fetch_fill_ptr] = 1'b1;
        dirty_nxt[fetch_fill_ptr] = 1'b0;
    end

    if (spill_done)
    begin
         valid_en[spill_ptr_ff] = 1'b1;
        valid_nxt[spill_ptr_ff] = 1'b0;
         dirty_en[spill_ptr_ff] = 1'b0;
        dirty_nxt[spill_ptr_ff] = 1'b0;
        spill_pending_acks_en   = 1'b1;
        spill_pending_acks_nxt++;
    end

    if (spill_ack_received)
    begin
        spill_pending_acks_en = 1'b1;
        spill_pending_acks_nxt--;
    end

    if (dbg_db_meta_write_valid)
    begin
                 tags_en[dbg_ram_ptr] = 1'b1;
                dirty_en[dbg_ram_ptr] = 1'b1;
                valid_en[dbg_ram_ptr] = 1'b1;
             reserved_en[dbg_ram_ptr] = 1'b1;
         dealloc_quad_en[dbg_ram_ptr] = 1'b1;

        {        tags_nxt[dbg_ram_ptr],
                dirty_nxt[dbg_ram_ptr],
                valid_nxt[dbg_ram_ptr],
             reserved_nxt[dbg_ram_ptr],
         dealloc_quad_nxt[dbg_ram_ptr]} = dbg_db_meta_write_data;

    end

end

et_link_req_info_t        db_sc_req_ff;
et_link_req_info_t        db_sc_req_nxt;
logic [(`SC_BANKS+1)-1:0] db_sc_req_valid_ff;
logic [(`SC_BANKS+1)-1:0] db_sc_req_valid_nxt;
logic                     db_sc_req_en;

`RST_EN_FF(clock_rbox, reset, db_sc_req_en, db_sc_req_valid_ff, db_sc_req_valid_nxt, '0);
    `EN_FF(clock_rbox,        db_sc_req_en, db_sc_req_ff      , db_sc_req_nxt          );

logic                       fetch_spill_read;
logic                       flush_spill_read;
logic                       spill_read;
logic [`PA_RANGE]           spill_address;
logic [`PA_RANGE]           flush_address;
logic                       fetch_fill_write;
logic [`RBOX_DEPTH_LINE_R]  fetch_fill_data;
logic [`RBOX_DEPTH_LINE_R]  write_data;
logic [`RBOX_DEPTH_BUF_I_R] write_ptr;
logic                       write_valid;
logic [`RBOX_DEPTH_BUF_I_R] read_ptr;
logic [`RBOX_DEPTH_LINE_R]  read_data;
logic                       read_valid;
logic                       db_sc_req_accepted;

always_comb
begin
    db_sc_req_accepted = db_sc_req_ready || !(|db_sc_req_valid_ff);

    fetch_spill_read = db_fetch_valid && !dbg_db_read_valid && !dbg_db_write_valid
                     && !(|tag_match) && valid_ff[db_fetch_ptr] && dirty_ff[db_fetch_ptr];

    flush_spill_read = db_flush && !dbg_db_read_valid && !dbg_db_write_valid
                     && valid_ff[flush_ptr_ff] && dirty_ff[flush_ptr_ff];

    spill_read = fetch_spill_read || flush_spill_read;

    spill_ptr_en  = spill_read;
    spill_ptr_nxt = db_flush ? flush_ptr_ff : db_fetch_ptr;

    spill_done = spill_pending_ff && db_sc_req_accepted;

    fetch_fill_write = db_fetch_valid && !dbg_db_read_valid && !dbg_db_write_valid
                     && !(|tag_match) && (spill_done || !fetch_spill_read);

    fetch_fill_done =  fetch_fill_pending_ff && db_sc_req_accepted
                    || fetch_fill_write      && db_sc_req_accepted;

    spill_pending_en  = !spill_pending_ff && spill_read
                      || spill_done;

    spill_pending_nxt = !spill_pending_ff && spill_read;

    fetch_fill_pending_en =  !fetch_fill_pending_ff && !fetch_fill_done
                          ||  fetch_fill_pending_ff &&  db_sc_req_accepted;

    fetch_fill_pending_nxt =  !fetch_fill_pending_ff && fetch_fill_write && !fetch_fill_done;

    db_sc_req_en        = db_sc_req_ready;
    db_sc_req_valid_nxt = db_sc_req_ready ? '0 : db_sc_req_valid_ff;
    db_sc_req_nxt       = '0;

    spill_address = '0;

    if (spill_pending_ff && db_sc_req_accepted)
    begin
        spill_address = {tags_ff[spill_ptr_ff], 6'b0};

        db_sc_req_nxt.source    = 0;
        db_sc_req_nxt.wdata     = 1;
        db_sc_req_nxt.size      = ET_LINK_Line;
        db_sc_req_nxt.qwen      = 4'b1111;
        db_sc_req_nxt.opcode    = ET_LINK_REQ_Write;
        db_sc_req_nxt.subopcode = '0;
        db_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, spill_ptr_ff) + 8'h80;
        db_sc_req_nxt.address   = spill_address;
        db_sc_req_nxt.data      = read_data;

        db_sc_req_valid_nxt[spill_address[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;

        db_sc_req_en = 1'b1;
    end

    if (!fetch_fill_pending_ff && fetch_fill_write)
    begin
        db_sc_req_nxt.source    = 0;
        db_sc_req_nxt.wdata     = 0;
        db_sc_req_nxt.size      = ET_LINK_Line;
        db_sc_req_nxt.qwen      = 4'b0000;
        db_sc_req_nxt.opcode    = ET_LINK_REQ_Read;
        db_sc_req_nxt.subopcode = '0;
        db_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, db_fetch_ptr) + 8'h80;
        db_sc_req_nxt.address   = {db_fetch_address[`RBOX_DEPTH_BUF_TAG_R], 6'b0};

        db_sc_req_valid_nxt[db_fetch_address[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;

        db_sc_req_en = 1'b1;
    end

    db_sc_req       = db_sc_req_ff;
    db_sc_req_valid = db_sc_req_valid_ff;
end

always_comb
begin
    read_valid =  spill_read
               || db_read_valid && valid_ff[db_read_ptr]
               || dbg_db_read_valid;
    read_ptr   = dbg_db_read_valid ? dbg_ram_ptr
                                   : (spill_read ? spill_ptr_nxt : db_read_ptr);

    fetch_fill_received =   sc_db_rsp_valid
                        && (sc_db_rsp.opcode == ET_LINK_RSP_AckData)
                        &&  sc_db_rsp.id[`ET_LINK_ID_SIZE-1];
    fetch_fill_ptr      = sc_db_rsp.id[`RBOX_DEPTH_BUF_I_R];
    fetch_fill_data     = sc_db_rsp.data;

    spill_ack_received =    sc_db_rsp_valid
                        && (sc_db_rsp.opcode == ET_LINK_RSP_Ack)
                        &&  sc_db_rsp.id[`ET_LINK_ID_SIZE-1];

    write_valid = fetch_fill_received || db_write_valid;
    write_ptr   = dbg_db_write_valid ? dbg_ram_ptr
                                     : (fetch_fill_received ? fetch_fill_ptr : db_write_ptr);
    write_data  = dbg_db_write_valid ? dbg_db_write_data
                                     : (fetch_fill_received ? fetch_fill_data : db_write_data);

    db_write_done = db_write_valid && !fetch_fill_received;

    db_read_data = read_data;

    db_read_data_valid = db_read_valid && !dbg_db_read_valid && !spill_read && valid_ff[db_read_ptr];

end

// Flush machine

always_comb
begin
    flush_ptr_en        = 1'b0;
    flush_done_en       = 1'b0;
    flush_spill_done_en = 1'b0;

    flush_ptr_nxt        = '0;
    flush_spill_done_nxt = 1'b0;
    flush_done_nxt       = 1'b0;

    if (reset)
    begin
        flush_ptr_en         = 1'b1;
        flush_ptr_nxt        = '0;
    end

    if (  (db_flush && !flush_spill_done_ff)
        && ((     valid_ff[flush_ptr_ff] &&  dirty_ff[flush_ptr_ff] && spill_done)
             || (!valid_ff[flush_ptr_ff] || !dirty_ff[flush_ptr_ff])))
    begin
        flush_ptr_en  = 1'b1;
        flush_ptr_nxt = flush_ptr_ff + 'd1;

        flush_spill_done_en  = (flush_ptr_ff == '1);
        flush_spill_done_nxt = (flush_ptr_ff == '1);
    end

    flush_spill_done_en =  reset
                        || flush_spill_done_en
                        || flush_done_ff;

    flush_spill_done_nxt = !reset && flush_spill_done_nxt;

    flush_done_en  =   reset
                   || !flush_done_ff && flush_spill_done_ff && (spill_pending_acks_ff == '0)
                   ||  flush_done_ff;
    flush_done_nxt = !reset && flush_spill_done_ff && (spill_pending_acks_ff == '0) && !flush_done_ff;

    db_flush_done = flush_done_ff;
end


// Debug RAM
always_comb
begin
    dbg_db_read_data      = read_data;
    dbg_db_meta_read_data = {       valid_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]],
                                    dirty_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]],
                                 reserved_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]],
                             dealloc_quad_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]],
                                     tags_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]]};
end

// Assertions commented out for cosim
// `ASSERT_ALWAYS_MSG(ERROR_rbox_dbuf_sram_sz, ...)
// `ASSERT_ALWAYS_MSG(ERROR_rbox_dbuf_sz_2n, ...)
// `ASSERT_ALWAYS_MSG(ERROR_rbox_dbuf_tag_match_1h, ...)
// `ASSERT_ALWAYS_MSG(ERROR_rbox_dbuf_no_reserve_flush, ...)

    gen_mem2p #(.WIDTH(`RBOX_DEPTH_LINE_SZ), .WORDS(`RBOX_DEPTH_BUF_SZ))
    depth_buffer_data (
                       .clock    (clock         ),
                       .addressA (read_ptr      ),
                       .enA      (read_valid    ),
                       .weA      (1'b0          ),
                       .dinA     ('0            ),
                       .doutA    (read_data     ),
                       .addressB (write_ptr     ),
                       .enB      (write_valid   ),
                       .weB      (1'b1 && !esr_shire_cache_ram_cfg.dft__ram_wei ),
                       .dinB     (write_data    ),
                       .doutB    (/*unused*/    )
                      );


wire unused_ok = &{
   1'b0,
   esr_shire_cache_ram_cfg,
   esr_shire_cache_ram_cfg
};


endmodule
