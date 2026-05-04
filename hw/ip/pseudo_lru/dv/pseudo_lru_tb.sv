// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module pseudo_lru_tb #(
  parameter int unsigned NWays = 8,
  parameter int unsigned IdxWidth = $clog2(NWays)
) (
  input  logic                clk_i,
  input  logic                rst_ni,
  input  logic                access_i,
  input  logic [IdxWidth-1:0] index_in_i,
  output logic [IdxWidth-1:0] index_out_o
);
  pseudo_lru #(
    .NWays(NWays)
  ) dut (
    .clk_i,
    .rst_ni,
    .access_i,
    .index_in_i,
    .index_out_o
  );
endmodule
