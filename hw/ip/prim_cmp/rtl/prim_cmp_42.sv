// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_cmp_42 — 4:2 compressor (carry-save adder).
//
// Pure combinational. Replaces: r42cmp (etcore-soc)

/* verilator lint_off UNOPTFLAT */
module prim_cmp_42 #(
  parameter int unsigned Width = 1
) (
  input  logic [Width-1:0] a,
  input  logic [Width-1:0] b,
  input  logic [Width-1:0] c,
  input  logic [Width-1:0] d,
  input  logic             cin,
  output logic [Width-1:0] sv,
  output logic [Width-1:0] cv
);

  /* verilator lint_off UNUSEDSIGNAL */
  logic [Width-1:0] cout;
  /* verilator lint_on UNUSEDSIGNAL */

  assign cout = (a ^ b) & c | ~(a ^ b) & a;
  assign sv = (a ^ b ^ c ^ d) ^ {cout[Width-2:0], cin};
  assign cv = (a ^ b ^ c ^ d) & {cout[Width-2:0], cin} | ~(a ^ b ^ c ^ d) & d;

/* verilator lint_on UNOPTFLAT */
endmodule
