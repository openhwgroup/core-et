// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// FIFO example project top.
//
// Instantiates prim_fifo_sync with fixed parameters.
// Serves as a template for projects that use IPs.

module fifo_example #(
  parameter int unsigned Width = 8,
  parameter int unsigned Depth = 4,
  // Derived — do not override.
  localparam int unsigned DepthW = $clog2(Depth + 1)
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  input  logic              clr_i,

  // Write port
  input  logic              wvalid_i,
  output logic              wready_o,
  input  logic [Width-1:0]  wdata_i,

  // Read port
  output logic              rvalid_o,
  input  logic              rready_i,
  output logic [Width-1:0]  rdata_o,

  // Status
  output logic [DepthW-1:0] depth_o,
  output logic              full_o,
  output logic              empty_o
);

  prim_fifo_sync #(
    .Width(Width),
    .Depth(Depth)
  ) u_fifo (
    .clk_i,
    .rst_ni,
    .clr_i,
    .wvalid_i,
    .wready_o,
    .wdata_i,
    .rvalid_o,
    .rready_i,
    .rdata_o,
    .depth_o,
    .full_o,
    .empty_o
  );

endmodule
