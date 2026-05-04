// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_micro_data_array #(
  parameter WIDTH   = `ICACHE_BLOCK_BITS+2,
  parameter ENTRIES = `ICACHE_MICRO_CACHE_SIZE
) (
  // System signals
  input  logic               clock,
  // Read port request
  input  logic               f3_rd_en,
  input  logic [ENTRIES-1:0] f3_rd_addr_dec,
  output logic [WIDTH-1:0]   f3_rd_data,
  // Write port request
  input  logic               f0_wr_en,
  input  logic               f0_wr_en_early,
  input  logic [ENTRIES-1:0] f0_wr_addr_dec,
  input  logic [WIDTH-1:0]   f0_wr_data,
  // Debug access
  input  logic [ENTRIES-1:0] dbg_addr_dec,
  input  logic               dbg_write_en,
  input  logic               dbg_write_en_early,
  input  logic [WIDTH-1:0]   dbg_write_data,
  output logic               dbg_write_ready,
  input  logic               dbg_read_en,
  output logic [WIDTH-1:0]   dbg_read_data,
  output logic               dbg_read_ready
  );

  // INTERNAL DECLARATIONS
  logic [ENTRIES-1:0] rd_addr_dec;
  logic [WIDTH-1:0]   rd_data;
  logic               wr_en;
  logic               wr_en_early;
  logic               wr_en_early_p2;
  logic [ENTRIES-1:0] wr_addr_dec;
  logic [WIDTH-1:0]   wr_data;

  // Multiplex normal operation and debug access signals to memory
  assign rd_addr_dec = f3_rd_en ? f3_rd_addr_dec : dbg_addr_dec;
  assign f3_rd_data  = rd_data;
  assign wr_en       = f0_wr_en | dbg_write_en;
  assign wr_en_early = f0_wr_en_early | dbg_write_en_early;
  assign wr_addr_dec = f0_wr_en ? f0_wr_addr_dec : dbg_addr_dec;
  assign wr_data     = f0_wr_en ? f0_wr_data     : dbg_write_data;

  `LATCH_P2(clock, wr_en_early_p2, wr_en_early)

  rf_latch_1r_1w_dec  #(
    .WIDTH           ( WIDTH          ),
    .ENTRIES         ( ENTRIES        )
  ) line_storage (
    // System signals
    .clk             ( clock          ),
    .test_en         ( 1'b0           ),
    // Read port
    .rd_addr_a       ( rd_addr_dec    ),
    .rd_data_a       ( rd_data        ),
    // Write port
    .wr_en_a         ( wr_en          ),
    .wr_data_a_en_1p ( wr_en_early_p2 ),
    .wr_addr_a       ( wr_addr_dec    ),
    .wr_data_a       ( wr_data        )
  );

  // //////////////////////////////////////////////////////////////////////////////
  // Debug
  // //////////////////////////////////////////////////////////////////////////////

  // Read access
  assign dbg_read_data  = rd_data;
  assign dbg_read_ready = dbg_read_en & ~f3_rd_en;

  // Write access
  assign dbg_write_ready = dbg_write_en & ~f0_wr_en;

endmodule
