// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module minion_dcache_atomic_alu_unit_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [2:0]  addr_i,
  input  logic [4:0]  cmd_i,
  input  logic [1:0]  typ_i,
  input  logic [63:0] lhs_i,
  input  logic [63:0] rhs_i,
  output logic [63:0] out_o
);

  minion_dcache_atomic_alu_unit u_dut (
    .clk_i,
    .rst_ni,
    .addr_i,
    .cmd_i (minion_dcache_pkg::dcache_cmd_e'(cmd_i)),
    .typ_i,
    .lhs_i,
    .rhs_i,
    .out_o
  );

endmodule
