// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_tensorb_rf (
  // System signal
  input  logic                                clock,
  // Read ports
  input  logic [$clog2(`VPU_TENB_SIZE/2)-1:0] rd_addr,
  output logic [`VPU_DATA_S_SZ-1:0]           rd_data,
  // Write ports
  input  logic                                wr_en,
  input  logic                                wr_en_early,
  input  logic [$clog2(`VPU_TENB_SIZE/2)-1:0] wr_addr,
  input  logic [`VPU_DATA_S_SZ-1:0]           wr_data
);

////////////////////////////////////////////////////////////////////////////////
// Data storage
////////////////////////////////////////////////////////////////////////////////

logic wr_en_early_p2;

`LATCH_P2(clock, wr_en_early_p2, wr_en_early)

rf_latch_1r_1w #(
  .WIDTH           ( `VPU_DATA_S_SZ   ),
  .ENTRIES         ( `VPU_TENB_SIZE/2 ),
  .LEVEL2_CLK_GATE ( 1                )
) rf_buffer_array (
  // System signals
  .clk             ( clock            ),
  .test_en         ( 1'b0             ),
  // Read port
  .rd_addr_a       ( rd_addr          ),
  .rd_data_a       ( rd_data          ),
  // Write port
  .wr_en_a         ( wr_en            ),
  .wr_data_a_en_1p ( wr_en_early_p2   ),
  .wr_addr_a       ( wr_addr          ),
  .wr_data_a       ( wr_data          )
);

endmodule

