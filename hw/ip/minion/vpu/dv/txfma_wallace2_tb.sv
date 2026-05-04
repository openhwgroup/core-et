// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_wallace2_tb
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0]   sxt_i,
  input  logic [VpuDataSSz-1:0]   cin_i,
  input  logic [VpuDataSSz:0]     pp17_i,
  input  logic [2*VpuDataSSz-1:0] c21_i,
  input  logic [2*VpuDataSSz-1:0] c22_i,
  input  logic [2*VpuDataSSz-1:0] s21_i,
  input  logic [2*VpuDataSSz-1:0] s22_i,
  output logic [2*VpuDataSSz-1:0] c41_o,
  output logic [2*VpuDataSSz-1:0] s41_o
);

  txfma_wallace2 u_dut (
    .sxt_i,
    .cin_i,
    .pp17_i,
    .c21_i,
    .c22_i,
    .s21_i,
    .s22_i,
    .c41_o,
    .s41_o
  );

endmodule : txfma_wallace2_tb
