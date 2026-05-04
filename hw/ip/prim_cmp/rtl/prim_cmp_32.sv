// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_cmp_32 — 3:2 compressor (full adder array).
//
// Pure combinational. Replaces: r32cmp (etcore-soc)

module prim_cmp_32 #(
  parameter int unsigned Width = 1
) (
  input  logic [Width-1:0] a,
  input  logic [Width-1:0] b,
  input  logic [Width-1:0] c,
  output logic [Width-1:0] sv,
  output logic [Width-1:0] cv
);

  assign sv = a ^ b ^ c;
  assign cv = (a & b) | (a & c) | (b & c);

endmodule
