// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_atomic_alu_unit vs new
// minion_dcache_atomic_alu_unit.

`include "soc.vh"

module cosim_minion_dcache_atomic_alu_unit_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [2:0]  addr_i,
  input  logic [4:0]  cmd_i,
  input  logic [1:0]  typ_i,
  input  logic [63:0] lhs_i,
  input  logic [63:0] rhs_i,
  output logic [63:0] new_out_o,
  output logic [63:0] orig_out_o
);

  logic reset;
  assign reset = ~rst_ni;

  minion_dcache_atomic_alu_unit u_new (
    .clk_i,
    .rst_ni,
    .addr_i,
    .cmd_i (minion_dcache_pkg::dcache_cmd_e'(cmd_i)),
    .typ_i,
    .lhs_i,
    .rhs_i,
    .out_o (new_out_o)
  );

  dcache_atomic_alu_unit u_orig (
    .clock (clk_i),
    .reset (reset),
    .addr  (addr_i),
    .cmd   (dcache_cmd'(cmd_i)),
    .typ   (typ_i),
    .lhs   (lhs_i),
    .rhs   (rhs_i),
    .out   (orig_out_o)
  );

endmodule
