// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_wallace1_tb
  import minion_pkg::*;
(
  input  logic [VpuDataSSz:0]     pp1_i,
  input  logic [VpuDataSSz:0]     pp2_i,
  input  logic [VpuDataSSz:0]     pp3_i,
  input  logic [VpuDataSSz:0]     pp4_i,
  input  logic [VpuDataSSz:0]     pp5_i,
  input  logic [VpuDataSSz:0]     pp6_i,
  input  logic [VpuDataSSz:0]     pp7_i,
  input  logic [VpuDataSSz:0]     pp8_i,
  input  logic [VpuDataSSz:0]     pp9_i,
  input  logic [VpuDataSSz:0]     pp10_i,
  input  logic [VpuDataSSz:0]     pp11_i,
  input  logic [VpuDataSSz:0]     pp12_i,
  input  logic [VpuDataSSz:0]     pp13_i,
  input  logic [VpuDataSSz:0]     pp14_i,
  input  logic [VpuDataSSz:0]     pp15_i,
  input  logic [VpuDataSSz:0]     pp16_i,
  input  logic                    mode_i,
  output logic [2*VpuDataSSz-1:0] c21_o,
  output logic [2*VpuDataSSz-1:0] c22_o,
  output logic [2*VpuDataSSz-1:0] s21_o,
  output logic [2*VpuDataSSz-1:0] s22_o
);

  txfma_wallace1 u_dut (
    .pp1_i,
    .pp2_i,
    .pp3_i,
    .pp4_i,
    .pp5_i,
    .pp6_i,
    .pp7_i,
    .pp8_i,
    .pp9_i,
    .pp10_i,
    .pp11_i,
    .pp12_i,
    .pp13_i,
    .pp14_i,
    .pp15_i,
    .pp16_i,
    .mode_i,
    .c21_o,
    .c22_o,
    .s21_o,
    .s22_o
  );

endmodule : txfma_wallace1_tb
