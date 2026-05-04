// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_micro_tag_array (
  // System signals
  input  logic                                   clock,
  input  logic                                   reset,
  // Read request
  input  logic [`ICACHE_MICRO_TAG_RANGE]         f2_rd_tag,
  output logic [`ICACHE_MICRO_CACHE_SIZE-1:0]    f2_rd_hit_array,
  // Write request
  input  logic                                   f0_wr_early_valid,
  input  logic                                   f0_wr_valid,
  input  logic [`ICACHE_MICRO_CACHE_SIZE-1:0]    f0_wr_victim,
  input  logic [`ICACHE_MICRO_TAG_RANGE]         f0_wr_tag,
  // Miss indication
  input  logic                                   f2_miss,
  input  logic [`ICACHE_MICRO_CACHE_SIZE-1:0]    f2_miss_victim,
  input  icache_miss_state                       f0_miss_state,
  // Flush control
  input  logic                                   f0_flush_data,
  // APB access
  input  logic [`ICACHE_DBG_UTAG_ADDR_WIDTH-1:0] apb_paddr,
  input  logic                                   apb_pwrite,
  input  logic                                   apb_psel,
  input  logic                                   apb_penable,
  input  logic [`bpam_shire_apb_data_width-1:0]  apb_pwdata,
  output logic                                   apb_pready,
  output logic [`bpam_shire_apb_data_width-1:0]  apb_prdata,
  output logic                                   apb_pslverr
  );

  // INTERNAL DECLARATIONS
  logic [`ICACHE_DBG_UTAG_ADDR_WIDTH-1:0] dbg_addr;
  logic                                   dbg_write_en, dbg_write_en_next;
  logic [`bpam_shire_apb_data_width-1:0]  dbg_write_data;
  logic                                   dbg_write_ready;
  logic                                   dbg_read_en, dbg_read_en_next;
  logic [`bpam_shire_apb_data_width-1:0]  dbg_read_data;

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F2 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_MICRO_CACHE_SIZE-1:0][`ICACHE_MICRO_TAG_RANGE] f2_tags; // Tags of entries in micro array
  logic [`ICACHE_MICRO_CACHE_SIZE-1:0]                          f2_valid_entries; // Valid entries in micro array
  logic [`ICACHE_MICRO_CACHE_SIZE-1:0]                          f2_valid_entries_next; // Next valid entries in micro array
  logic                                                         f2_invalidated; // Entries invalidated in micro array
  logic                                                         f2_invalidated_next; // Next entries invalidated in micro array

  // CLK    RST    DOUT              DIN                    RST
  `RST_FF(clock, reset, f2_valid_entries, f2_valid_entries_next, '0)
  `FF    (clock,        f2_invalidated,   f2_invalidated_next)

  // //////////////////////////////////////////////////////////////////////////////
  // TAGS
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_MICRO_CACHE_ADWIDTH-1:0] f0_wr_entry;
  logic                                   f0_tag_wr_en;
  logic                                   f0_tag_wr_en_early;
  logic                                   f0_tag_wr_en_early_p2;
  logic [`ICACHE_MICRO_CACHE_ADWIDTH-1:0] f0_tag_wr_addr;
  logic [`ICACHE_MICRO_TAG_RANGE]         f0_tag_wr_data;

  // Encoded victim entry
  always_comb begin
    f0_wr_entry = '0;

    for (integer i = 0; i < `ICACHE_MICRO_CACHE_SIZE; i++) begin
      if (f0_wr_victim[i])
      f0_wr_entry |= i[`ICACHE_MICRO_CACHE_ADWIDTH-1:0];
    end
  end

  assign f0_tag_wr_en       = dbg_write_ready | f0_wr_valid;
  assign f0_tag_wr_en_early = (apb_psel & apb_pwrite) | f0_wr_early_valid;
  assign f0_tag_wr_addr     = dbg_write_ready ? dbg_addr
      : f0_wr_entry;
  assign f0_tag_wr_data     = dbg_write_ready ? dbg_write_data[`ICACHE_MICRO_TAG_SIZE-1:0]
      : f0_wr_tag;

  `LATCH_P2(clock, f0_tag_wr_en_early_p2, f0_tag_wr_en_early)

  rf_latch_1r_1w_par #(
    .WIDTH           ( `ICACHE_MICRO_TAG_SIZE   ),
    .ENTRIES         ( `ICACHE_MICRO_CACHE_SIZE )
  ) rf_tag_data_array (
    // System signals
    .clk             ( clock                    ),
    .test_en         ( 1'b0                     ),
    // Output port
    .rd_data_a       ( f2_tags                  ),
    // Writ port
    .wr_en_a         ( f0_tag_wr_en             ),
    .wr_data_a_en_1p ( f0_tag_wr_en_early_p2    ),
    .wr_addr_a       ( f0_tag_wr_addr           ),
    .wr_data_a       ( f0_tag_wr_data           )
  );

  // //////////////////////////////////////////////////////////////////////////////
  // Valid entries
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    f2_valid_entries_next = f2_valid_entries;
    f2_invalidated_next = f2_invalidated;

    for (integer i = 0; i < `ICACHE_MICRO_CACHE_SIZE; i++) begin
      // Set valid entry upon fill
      // (unless a flush happened while fill was pending)
      if (f0_wr_valid) begin
        if (f0_wr_victim[i] & !f2_invalidated)
          f2_valid_entries_next[i] = 1'b1;
      end else if (dbg_write_ready)
        f2_valid_entries_next[dbg_addr] = dbg_write_data[`ICACHE_MICRO_TAG_SIZE];

      // Clear the valid bit of the selected victim on a miss
      if (f2_miss & f2_miss_victim[i])
        f2_valid_entries_next[i] = 1'b0;

      // Clear valid entries in case of an invalidation
      if (f0_flush_data)
        f2_valid_entries_next[i] = 1'b0;
    end

    // Keep entries invalidated while a fill is pending
    if (f0_miss_state == icache_miss_state_Ready)
      f2_invalidated_next = 1'b0;
    if (f0_flush_data)
      f2_invalidated_next = 1'b1;
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Compares the input PA with the entries and generate hit array
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_MICRO_CACHE_SIZE-1:0] f2_hit_array; // Hit array with the valid entries in F2
  logic [`ICACHE_MICRO_CACHE_SIZE-1:0] f2_hit_array_from_fill; // Hit array with entry that is being filled right now

  always_comb begin
    // Compare for all entries
    for (integer i = 0; i < `ICACHE_MICRO_CACHE_SIZE; i++) begin
      f2_hit_array[i] = f2_valid_entries[i]
          && (f2_tags[i] == f2_rd_tag)
          && !(f0_tag_wr_en && (f0_tag_wr_addr == i[`ICACHE_MICRO_CACHE_ADWIDTH-1:0])) // If being filled, do not hit on old value
          && !f0_flush_data;

      // Compare with entry that is being filled
      f2_hit_array_from_fill[i] = f0_tag_wr_en
          && (f0_tag_wr_data == f2_rd_tag)
          && (f0_tag_wr_addr ==  i[`ICACHE_MICRO_CACHE_ADWIDTH-1:0]);
    end

    // Generate final hit array including hit from fill
    f2_rd_hit_array = f2_hit_array | f2_hit_array_from_fill;
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Debug
  // //////////////////////////////////////////////////////////////////////////////

  // CLK    RST    EN                       DOUT            DIN                            DEF
  `EN_FF  (clock,        apb_psel & ~apb_penable, dbg_addr,       apb_paddr)
  `EN_FF  (clock,        apb_psel & ~apb_penable, dbg_write_data, apb_pwdata)
  `RST_FF (clock, reset,                          dbg_read_en,    dbg_read_en_next,              1'b0)
  `RST_FF (clock, reset,                          dbg_write_en,   dbg_write_en_next,             1'b0)
  `EN_FF  (clock,        dbg_read_en,             apb_prdata,     dbg_read_data)
  `RST_FF (clock, reset,                          apb_pready,     dbg_read_en | dbg_write_ready, 1'b0)

  // Read access
  always_comb begin
    dbg_read_en_next = dbg_read_en;
    if (apb_psel & ~apb_penable & ~apb_pwrite)
      dbg_read_en_next = 1'b1;
    else if (dbg_read_en)
      dbg_read_en_next = 1'b0;
  end

  assign dbg_read_data = (`bpam_shire_apb_data_width)'({f2_valid_entries[dbg_addr],f2_tags[dbg_addr]});

  // Write access
  always_comb begin
    dbg_write_en_next = dbg_write_en;
    if (apb_psel & ~apb_penable & apb_pwrite)
      dbg_write_en_next = 1'b1;
    else if (dbg_write_ready)
      dbg_write_en_next = 1'b0;
  end

  assign dbg_write_ready = dbg_write_en & ~f0_wr_valid;

  assign apb_pslverr = 1'b0;

endmodule
