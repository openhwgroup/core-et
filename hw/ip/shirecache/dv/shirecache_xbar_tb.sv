// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_xbar.
// Small crossbar (3 sources, 2 destinations) for tractable C++ testing.
// Parameters SrcArb and AddOutputRelay driven via TB parameters.

module shirecache_xbar_tb #(
  parameter int unsigned NumSrc         = 3,
  parameter int unsigned NumDest        = 2,
  parameter int unsigned DataWidth      = 16,
  parameter int unsigned FifoDepth      = 3,
  parameter bit [NumSrc-1:0]  SrcArb        = '0,
  parameter bit [NumDest-1:0] AddOutputRelay = '0
) (
  input  logic                                 clk_i,
  input  logic                                 rst_ni,

  // Source side
  output logic [NumSrc-1:0][NumDest-1:0]       src_ready_o,
  output logic [NumSrc-1:0][NumDest-1:0]       src_2_credits_o,
  input  logic [NumSrc-1:0][NumDest-1:0]       src_valid_i,
  input  logic [NumSrc-1:0][DataWidth-1:0]     src_data_i,

  // Destination side
  input  logic [NumDest-1:0]                   dest_ready_i,
  output logic [NumDest-1:0]                   dest_valid_o,
  output logic [NumDest-1:0][DataWidth-1:0]    dest_data_o
);

  shirecache_xbar #(
    .FifoDepth      (FifoDepth),
    .NumSrc         (NumSrc),
    .NumDest        (NumDest),
    .DataWidth      (DataWidth),
    .NumSrcMask     ({NumSrc{1'b1}}),
    .SrcArb         (SrcArb),
    .AddOutputRelay (AddOutputRelay)
  ) u_xbar (
    .clk_i,
    .rst_ni,
    .src_ready_o,
    .src_2_credits_o,
    .src_valid_i,
    .src_data_i,
    .dest_ready_i,
    .dest_valid_o,
    .dest_data_o
  );

endmodule : shirecache_xbar_tb
