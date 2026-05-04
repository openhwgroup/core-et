// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_tag_array (
  // System signals
  input  logic                                  clock,
  input  logic                                  reset,
  // Read request
  input  logic                                  f0_read_valid,
  input  logic [`PA_RANGE]                      f0_read_paddr,
  input  logic [`PA_RANGE]                      f0_read_paddr_next,
  input  logic                                  f0_read_paddr_en,
  output logic [`ICACHE_WAYS-1:0]               f1_read_tag_hit,
  // Write request
  input  logic                                  f0_write_valid,
  input  icache_tag_write_req                   f0_write_req,
  // Miss indication
  input  logic                                  f1_miss,
  input  logic [`ICACHE_SET_RANGE]              f1_miss_idx,
  input  logic [`ICACHE_WAY_RANGE]              f1_miss_way,
  input  icache_miss_state                      f0_miss_state,
  // Flush control
  input  logic                                  f0_flush_data,
  // APB access
  input  logic [`ICACHE_DBG_TAG_ADDR_WIDTH-1:0] apb_paddr,
  input  logic                                  apb_pwrite,
  input  logic                                  apb_psel,
  input  logic                                  apb_penable,
  input  logic [`bpam_shire_apb_data_width-1:0] apb_pwdata,
  output logic                                  apb_pready,
  output logic [`bpam_shire_apb_data_width-1:0] apb_prdata,
  output logic                                  apb_pslverr
  );

  // INTERNAL DECLARATIONS
  logic [`ICACHE_SET_RANGE]                      dbg_set;
  logic [`ICACHE_SET_RANGE]                      dbg_read_set;
  logic                                          dbg_read_addr_en;
  logic [$clog2(`ICACHE_WAYS/2)-1:0]             dbg_ways;
  logic [`ICACHE_DBG_TAG_ADDR_WIDTH-1:0]         dbg_addr;
  logic                                          dbg_write_en, dbg_write_en_next;
  logic [`bpam_shire_apb_data_width-1:0]         dbg_write_data;
  logic                                          dbg_write_ready;
  logic                                          dbg_read_en, dbg_read_en_next;
  logic [`ICACHE_WAYS-1:0][`ICACHE_PA_TAG_RANGE] dbg_tag_data;
  logic [`bpam_shire_apb_data_width-1:0]         dbg_read_data;

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F0 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_SETS-1:0][`ICACHE_WAYS-1:0] f0_tag_valid, f0_tag_valid_next; // Valid cacheline entries in icache
  logic                                      f0_invalidated, f0_invalidated_next; // Tags have been invalidated

  // CLK    RST    DOUT            DIN                DEF
  `RST_FF(clock, reset, f0_tag_valid,   f0_tag_valid_next, '0)
  `FF    (clock,        f0_invalidated, f0_invalidated_next)

  // //////////////////////////////////////////////////////////////////////////////
  // Gets some info of the read request
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_SET_RANGE] f0_read_idx; // Set index for read request
  logic [`ICACHE_SET_RANGE] f0_read_idx_next; //flop inside the tag array 

  assign f0_read_idx      = f0_read_paddr[`ICACHE_SET_ADDR_RANGE];
  assign f0_read_idx_next = f0_read_paddr_next[`ICACHE_SET_ADDR_RANGE];

  // //////////////////////////////////////////////////////////////////////////////
  // Array with the tag data
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_WAYS-1:0][`ICACHE_PA_TAG_RANGE] f0_tag_data;

 for (genvar i = 0; i < `ICACHE_WAYS; i++) begin : TAG_ARRAY_WAYS
    logic                        f0_tag_wr_en;
    logic                        f0_tag_wr_en_early_p2; // Phase 2 version of the early write enable
    logic [`ICACHE_SET_RANGE]    f0_tag_wr_addr;
    logic [`ICACHE_PA_TAG_RANGE] f0_tag_wr_data;

    assign f0_tag_wr_en       = dbg_write_ready ? dbg_ways == i[`ICACHE_WAY_ADDR_WIDTH-1:1]
        : f0_write_valid & f0_write_req.valid & (f0_write_req.way == i[`ICACHE_WAY_RANGE]);
    assign f0_tag_wr_addr     = dbg_write_ready ? dbg_set
        : f0_write_req.idx;
    assign f0_tag_wr_data     = dbg_write_ready ? dbg_write_data[i[0] * `bpam_shire_apb_data_width/2 +: `ICACHE_PA_TAG_SIZE]
        : f0_write_req.data;

    icache_tag_data_array icache_tag_data_array (
      // System signals
      .clk             ( clock            ),
      // Read port A
      .rd_enable_a     ( f0_read_paddr_en ),
      .rd_addr_a       ( f0_read_idx_next ),
      .rd_data_a       ( f0_tag_data[i]   ),
      // Read port B
      .rd_enable_b     ( dbg_read_addr_en ),
      .rd_addr_b       ( dbg_read_set     ),
      .rd_data_b       ( dbg_tag_data[i]  ),
      // Write port
      .wr_enable       ( f0_tag_wr_en     ),
      .wr_addr         ( f0_tag_wr_addr   ),
      .wr_data         ( f0_tag_wr_data   )
    );
  end
 
  


  // //////////////////////////////////////////////////////////////////////////////
  // Valid entries set upon fill and clear upon invalidate
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    f0_tag_valid_next   = f0_tag_valid;
    f0_invalidated_next = f0_invalidated;

    if (f0_write_valid) begin
      // Set the valid bit upon fill
      // (unless a flush happened while fill was pending)
      if (f0_write_req.valid && !f0_invalidated)
        f0_tag_valid_next[f0_write_req.idx][f0_write_req.way] = 1'b1;
      // Clear the valid bit upon a DBE
      else if (!f0_write_req.valid)
        f0_tag_valid_next[f0_write_req.idx][f0_write_req.way] = 1'b0;
    end else if (dbg_write_ready) begin
    // Debug write access
      f0_tag_valid_next[dbg_set][{dbg_ways,1'b0}] = dbg_write_data[`ICACHE_PA_TAG_SIZE];
      f0_tag_valid_next[dbg_set][{dbg_ways,1'b1}] = dbg_write_data[`bpam_shire_apb_data_width/2 + `ICACHE_PA_TAG_SIZE];
    end

    // Clears the valid bit of the selected victim
    if(f1_miss)
      f0_tag_valid_next[f1_miss_idx][f1_miss_way] = 1'b0;

    // Clears the bits in case of an invalidation
    if (f0_flush_data)
      f0_tag_valid_next = '0;

    // Keep entries invalidated while a fill is pending
    if (f0_miss_state == icache_miss_state_Ready)
      f0_invalidated_next = 1'b0;
    if (f0_flush_data)
      f0_invalidated_next = 1'b1;
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Debug
  // //////////////////////////////////////////////////////////////////////////////

  //       CLK    RST    EN                       DOUT            DIN                            DEF
  `EN_FF  (clock,        apb_psel & ~apb_penable, dbg_addr,       apb_paddr)
  `EN_FF  (clock,        apb_psel & ~apb_penable, dbg_write_data, apb_pwdata)
  `RST_FF (clock, reset,                          dbg_read_en,    dbg_read_en_next,              1'b0)
  `RST_FF (clock, reset,                          dbg_write_en,   dbg_write_en_next,             1'b0)
  `EN_FF  (clock,        dbg_read_en,             apb_prdata,     dbg_read_data)
  `RST_FF (clock, reset,                          apb_pready,     dbg_read_en | dbg_write_ready, 1'b0)

  assign dbg_set  = dbg_addr[$clog2(`ICACHE_WAYS/2) +: `ICACHE_SET_ADDR_WIDTH];
  assign dbg_ways = dbg_addr[$clog2(`ICACHE_WAYS/2)-1:0];
  assign dbg_read_set  = apb_paddr[$clog2(`ICACHE_WAYS/2) +: `ICACHE_SET_ADDR_WIDTH]; // moving flop inside the tag array
  assign dbg_read_addr_en = apb_psel & ~apb_penable ;

  // Read access`
  always_comb begin
    dbg_read_en_next = dbg_read_en;
    if (apb_psel & ~apb_penable & ~apb_pwrite)
      dbg_read_en_next = 1'b1;
    else if (dbg_read_en)
      dbg_read_en_next = 1'b0;
  end

  assign dbg_read_data = { (`bpam_shire_apb_data_width/2)'({f0_tag_valid[dbg_set][{dbg_ways,1'b1}],
      dbg_tag_data[{dbg_ways,1'b1}]}), (`bpam_shire_apb_data_width/2)
      '({f0_tag_valid[dbg_set][{dbg_ways,1'b0}], dbg_tag_data[{dbg_ways,1'b0}]})
  };

  // Write access
  always_comb begin
    dbg_write_en_next = dbg_write_en;
    if (apb_psel & ~apb_penable & apb_pwrite)
      dbg_write_en_next = 1'b1;
    else if (dbg_write_ready)
      dbg_write_en_next = 1'b0;
  end

  assign dbg_write_ready = dbg_write_en & ~f0_write_valid;

  assign apb_pslverr = 1'b0;

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F1 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic [`PA_RANGE]                              f1_read_paddr;
  logic [`ICACHE_WAYS-1:0][`ICACHE_PA_TAG_RANGE] f1_tag_data;
  logic [`ICACHE_WAYS-1:0]                       f1_tag_valid; // Valid tags for the index in F1

  // CLK    EN             DOUT           DIN
  `EN_FF (clock, f0_read_valid, f1_read_paddr, f0_read_paddr)
  `EN_FF (clock, f0_read_valid, f1_tag_valid,  f0_tag_valid[f0_read_idx])

  // Reads the contents to a FF
  for (genvar i = 0; i < `ICACHE_WAYS; i++) begin : out_ff
    `EN_FF(clock, f0_read_valid, f1_tag_data[i], f0_tag_data[i])
  end


  // //////////////////////////////////////////////////////////////////////////////
  // Tag match
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_PA_TAG_RANGE] f1_tag; // Tag for the F1 operation

  always_comb begin
    // Gets the tag
    f1_tag = f1_read_paddr[`ICACHE_PA_TAG_RANGE];

    // Gets all the tag hits
    for (integer i = 0; i < `ICACHE_WAYS; i++)
      f1_read_tag_hit[i] = f1_tag_valid[i] && (f1_tag_data[i] == f1_tag) && !f0_flush_data;
  end

endmodule
