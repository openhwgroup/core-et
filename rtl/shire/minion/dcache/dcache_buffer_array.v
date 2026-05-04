// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_buffer_array (
  // System signals
  input  logic                           clock,
  input  logic                           reset,
  // Control
  output logic                           full,
  output logic                           reduce_full,
  // Alloc port
  input  logic                           alloc_req_pre,
  input  logic                           alloc_req_val,
  input  logic                           alloc_gsc,
  input  logic                           reduce_alloc_req,
  // Write port for dcache operations
  input  logic                           s2_write_needed_core,
  input  logic                           s2_write_needed,
  input  logic                           s2_write_en,
  input  logic                           s2_write_replay,
  input  logic [`DCACHE_BUFFER_ID_RANGE] s2_write_replay_entry,
  input  logic [`DCACHE_DATA_RANGE]      s3_write_data,
  output logic [`DCACHE_BUFFER_ID_RANGE] s2_write_entry,
  output logic                           s3_write_en,
  // Write port for UC loads
  input  logic                           s3_write_en_ext,
  input  logic                           s2_write_en_ext_prev,
  input  logic [`DCACHE_BUFFER_ID_RANGE] s3_write_entry_ext,
  input  logic [`DCACHE_DATA_RANGE]      s3_write_data_ext,
  // Dealloc port
  input  logic                           s2_dealloc,
  input  logic [`DCACHE_BUFFER_ID_RANGE] s2_dealloc_entry,
  // Dealloc port for UC stores
  input  logic                           s1_dealloc_uc,
  input  logic [`DCACHE_BUFFER_ID_RANGE] s1_dealloc_uc_entry,
  // Dealloc port for Reduce
  input  logic                           reduce_dealloc_req,
  input  logic [`DCACHE_BUFFER_ID_RANGE] reduce_dealloc_entry,
  // Read port
  input  logic [`DCACHE_BUFFER_ID_RANGE] s1_read_entry,
  output logic [`DCACHE_DATA_RANGE]      s1_read_data
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// This module stores outstanding data for different transactions in the
// dcache. It can store the data for stores that missed and UC stores. It also
// can store the data result for UC loads as well as partial data for
// misaligned loads. An entry is allocated at same time as a replayQ entry is
// allocated. Deallocation depends on what happens with the transaction. Each
// entry has the same width as the dcache working size.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  logic s0_alloc_req_pre; // Request was allocated for S0 transaction
  logic s0_alloc_req;     // Request was allocated for S0 transaction
  logic s1_alloc_req;     // Request was allocated for S1 transaction
  logic s2_alloc_req;     // Request was allocated for S2 transaction
  logic s3_alloc_req;     // Request was allocated for S3 transaction
  logic s0_alloc_gsc;     // Request allocated for S0 transaction is Gather/Scatter
  logic s1_alloc_gsc;     // Request allocated for S1 transaction is Gather/Scatter
  logic s2_alloc_gsc;     // Request allocated for S2 transaction is Gather/Scatter
  logic alloc_clk_en;     // Clock enable for the alloc registers

  // Validated request allocation
  assign s0_alloc_req = s0_alloc_req_pre && alloc_req_val;

  //         CLK    RST    EN            DOUT              DIN                           DEF
  `RST_EN_FF(clock, reset, alloc_clk_en, s0_alloc_req_pre, alloc_req_pre,                1'b0)
  `EN_FF    (clock,        alloc_clk_en, s1_alloc_req,     s0_alloc_req                      )
  `EN_FF    (clock,        alloc_clk_en, s2_alloc_req,     s1_alloc_req                      )
  `EN_FF    (clock,        alloc_clk_en, s3_alloc_req,     s2_alloc_req && s2_alloc_gsc      )
  `RST_EN_FF(clock, reset, alloc_clk_en, s0_alloc_gsc,     alloc_gsc,                    1'b0)
  `EN_FF    (clock,        alloc_clk_en, s1_alloc_gsc,     s0_alloc_gsc                      )
  `EN_FF    (clock,        alloc_clk_en, s2_alloc_gsc,     s1_alloc_gsc                      )

////////////////////////////////////////////////////////////////////////////////
// We need to know in ID core stage if there is an entry available to alloc
// and reserve it. So we have a counter for this purpose. When an entry is
// actually written, a free list of available entries is used to know which
// one to use (because deallocation is out of order).
////////////////////////////////////////////////////////////////////////////////

  logic [$clog2(`DCACHE_BUFFER_SIZE):0] entry_count_available, entry_count_available_next; // Counter of available entries at alloc time
  logic [`DCACHE_BUFFER_RANGE]          entry_mask_available,  entry_mask_available_next;  // Mask of available entries at write time
  logic                                 alloc_req_pre_applied, alloc_req_pre_applied_next; // Flag indicating that a resources was pre-allocated
  logic [$clog2(`DCACHE_BUFFER_SIZE):0] reduce_count_available, reduce_count_available_next; // Counter of available entries for reduce at alloc time


  //         CLK    RST    EN            DOUT                   DIN                         DEF
  `RST_EN_FF(clock, reset, alloc_clk_en, entry_count_available, entry_count_available_next,  `DCACHE_BUFFER_SIZE)
  `RST_EN_FF(clock, reset, alloc_clk_en, entry_mask_available,  entry_mask_available_next,  {`DCACHE_BUFFER_SIZE{1'b1}})
  `EN_FF    (clock,        alloc_clk_en, alloc_req_pre_applied, alloc_req_pre_applied_next)
  `RST_EN_FF(clock, reset, alloc_clk_en, reduce_count_available,reduce_count_available_next, `DCACHE_BUFFER_SIZE/2)

  always_comb begin
    // Sends full and gets which entry to write to
    s2_write_entry = 0;
    for (integer unsigned i = 1; i < `DCACHE_BUFFER_SIZE; i++) begin
      if (entry_mask_available[i]) begin
        s2_write_entry = i;
      end
    end

    // In case of replay, the write entry comes from the instruction itself
    if (s2_write_replay) begin
      s2_write_entry = s2_write_replay_entry;
    end

    full = (entry_count_available == 0);

    // Updates counter
    entry_count_available_next = entry_count_available;

    // Pre-allocate resource if possible
    alloc_req_pre_applied_next = 1'b0;
    if (alloc_req_pre || reduce_alloc_req) begin
      if (entry_count_available_next != 0) begin
        if (!reduce_alloc_req) begin
          alloc_req_pre_applied_next = 1'b1;
        end
        entry_count_available_next = entry_count_available_next - 'b1;
      end
    end

    // If allocated entry but not used, free it
    // Check if a pre-allocated resource has to be de-allocated
    if (alloc_req_pre_applied && !s0_alloc_req) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end

    // Gathers/Scatters use the entry one cycle later than other requests
    // Only one instruction can be pushed at a time, one of the counted entries must be freed
    if ((s2_alloc_req && !s2_alloc_gsc) && !s2_write_needed_core) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end
    if (s3_alloc_req && !s2_write_needed_core) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end
    if ((s2_alloc_req && !s2_alloc_gsc) && s3_alloc_req && s2_write_needed_core) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end

    // Used entry is deallocated
    if (s2_dealloc) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end
    if (s1_dealloc_uc) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end
    if (reduce_dealloc_req) begin
      entry_count_available_next = entry_count_available_next + 'b1;
    end

    // Updates mask
    entry_mask_available_next = entry_mask_available;
    if (s2_write_needed) begin
      entry_mask_available_next[s2_write_entry] = 1'b0;
    end
    if (s2_dealloc) begin
      entry_mask_available_next[s2_dealloc_entry] = 1'b1;
    end
    if (s1_dealloc_uc) begin
      entry_mask_available_next[s1_dealloc_uc_entry] = 1'b1;
    end
    if (reduce_dealloc_req) begin
      entry_mask_available_next[reduce_dealloc_entry] = 1'b1;
    end

    // Count entries allocated by reduce
    reduce_count_available_next = reduce_count_available;
    if (reduce_alloc_req && (reduce_count_available != 0)) begin
      reduce_count_available_next = reduce_count_available - 'b1;
    end
    if (reduce_dealloc_req) begin
      reduce_count_available_next = reduce_count_available_next + 'b1;
    end

    reduce_full = (reduce_count_available == 0) || full;

    // Clock enable
    alloc_clk_en = reset || alloc_req_pre || reduce_alloc_req || s0_alloc_req_pre || s1_alloc_req || s2_alloc_req || s3_alloc_req || s2_dealloc || s1_dealloc_uc || s2_write_needed || reduce_dealloc_req;
  end

////////////////////////////////////////////////////////////////////////////////
// This is the array with the actual data storage
////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_DATA_RANGE]                       s3_write_data_mux;   // Write data muxed between dcache and external
  logic [`DCACHE_BUFFER_ID_RANGE]                  s3_write_entry;      // Write entry from regular dcache pipeline
  logic [`DCACHE_BUFFER_ID_RANGE]                  s3_write_entry_mux;  // Muxed write entry between dcache pipeline and external pipeline
  logic                                            s3_write_en_1p;      // Write enable one cycle before latched in phase 2

  //  FF     DOUT            DIN
  `FF(clock, s3_write_en,    s2_write_en)
  `FF(clock, s3_write_entry, s2_write_entry)

  // Write enable one cycle before latched in phase 2
  `LATCH_P2(clock, s3_write_en_1p, s2_write_en_ext_prev || s2_write_en)

  rf_latch_1r_1w #(
    .WIDTH           ( `DCACHE_DATA_SIZE   ),
    .ENTRIES         ( `DCACHE_BUFFER_SIZE ),
    .LEVEL2_CLK_GATE ( 1                   )
  ) rf_buffer_array (
    // System signals
    .clk             ( clock                          ),
    .test_en         ( 1'b0                           ),
    // Read port
    .rd_data_a       ( s1_read_data                   ),
    .rd_addr_a       ( s1_read_entry                  ),
    // Write port
    .wr_addr_a       ( s3_write_entry_mux             ),
    .wr_data_a       ( s3_write_data_mux              ),
    .wr_en_a         ( s3_write_en || s3_write_en_ext ),
    .wr_data_a_en_1p ( s3_write_en_1p                 )
  );

  always_comb begin
    // Selects which is the entry written
    if (s3_write_en) begin
      s3_write_entry_mux = s3_write_entry;
    end else begin
      s3_write_entry_mux = s3_write_entry_ext;
    end

    // Selects the data to be written
    if (s3_write_en) begin
      s3_write_data_mux = s3_write_data;
    end else begin
      s3_write_data_mux = s3_write_data_ext;
    end

  end

endmodule
