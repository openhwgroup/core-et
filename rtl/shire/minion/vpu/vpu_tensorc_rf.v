// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_tensorc_rf (
  // System signal
  input  logic                      clock,
  // Read ports
  input  logic [`VPU_TENC_ADDR]     rd_addr,
  output logic [`VPU_DATA_S_SZ-1:0] rd_data,
  input  logic                      rd_en,
  // Write ports
  input  logic                      wr_en,
  input  logic [`VPU_TENC_ADDR]     wr_addr,
  input  logic [`VPU_DATA_S_SZ-1:0] wr_data
);

////////////////////////////////////////////////////////////////////////////////
// Data storage
////////////////////////////////////////////////////////////////////////////////

vpu_tensorc_rf_buffer_array rf_buffer_array (
  // System signals
  .clk             ( clock          ),
  // Read port
  .rd_data         ( rd_data        ),
  .rd_addr         ( rd_addr        ),
  .rd_enable       ( rd_en          ),
  // Write port
  .wr_addr         ( wr_addr        ),
  .wr_data         ( wr_data        ),
  .wr_enable       ( wr_en          )
);

endmodule

