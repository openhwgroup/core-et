// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module intpipe_rf #(
  parameter READ_PORTS  = 2,
  parameter WRITE_PORTS = 1
) (
  // System signals
  input logic                                      clock,
  // Read ports
  input logic [READ_PORTS-1:0]                     rd_en,
  input logic                                      rd_thread_id,
  input logic [READ_PORTS-1:0][`XREG_ADDR_RANGE]   rd_addr,
  output logic [READ_PORTS-1:0][`XREG_RANGE]       rd_data,
  output logic [`CORE_NR_THREADS-1:0][`XREG_RANGE] wb_x31_reg, // X31 reg value in WB stage for each thread
  // Write ports
  input logic [WRITE_PORTS-1:0]                    wr_en,
  input logic [WRITE_PORTS-1:0]                    wr_en_early,
  input logic                                      wr_thread_id,
  input logic [WRITE_PORTS-1:0][`XREG_ADDR_RANGE]  wr_addr,
  input logic [WRITE_PORTS-1:0][`XREG_RANGE]       wr_data
);

  ////////////////////////////////////////////////////////////////////////////////
  // latch phase 2 for the early write enable signal
  ////////////////////////////////////////////////////////////////////////////////

  logic [WRITE_PORTS-1:0] wr_en_early_p2; // Phase 2 version of the early write enable per write port

  genvar gen_it;

  generate for(gen_it = 0; gen_it < WRITE_PORTS; gen_it++) begin
    `LATCH_P2(clock, wr_en_early_p2[gen_it], wr_en_early[gen_it])
  end
  endgenerate

  ////////////////////////////////////////////////////////////////////////////////
  // Data storage
  // Bootrom should write a 0 to entry 0
  ////////////////////////////////////////////////////////////////////////////////

  rf_latch_2r_1w #(
    .WIDTH           ( `XREG_SIZE                       ),
    .ENTRIES         ( `CORE_NR_THREADS * 32            ),
    .ZERO            ( {`CORE_NR_THREADS{32'b1}}        ),
    .PARALLEL        ( {`CORE_NR_THREADS{32'h80000000}} ),
    .PARALLEL_W      ( `XREG_SIZE*2 ),
    .LEVEL2_CLK_GATE ( 1 )
  ) rf_latches (
    // System signals
    .clk             ( clock                        ),
    .test_en         ( 1'b0                         ),
    // Read port A
    .rd_addr_a       ( { rd_thread_id, rd_addr[0] } ),
    .rd_data_a       ( rd_data[0]                   ),
    // Read port B
    .rd_addr_b       ( { rd_thread_id, rd_addr[1] } ),
    .rd_data_b       ( rd_data[1]                   ),
    // parallel read
    .rd_par          ( wb_x31_reg                   ),
    // Write port
    .wr_en_a         ( wr_en[0]                     ),
    .wr_data_a_en_1p ( wr_en_early_p2[0]            ),
    .wr_addr_a       ( { wr_thread_id, wr_addr[0] } ),
    .wr_data_a       ( wr_data[0]                   )
  );

endmodule


