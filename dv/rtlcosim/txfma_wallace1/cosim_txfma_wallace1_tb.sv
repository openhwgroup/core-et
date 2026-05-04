// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_wallace1_tb
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
  output logic [2*VpuDataSSz-1:0] orig_c21_o,
  output logic [2*VpuDataSSz-1:0] orig_c22_o,
  output logic [2*VpuDataSSz-1:0] orig_s21_o,
  output logic [2*VpuDataSSz-1:0] orig_s22_o,
  output logic [2*VpuDataSSz-1:0] new_c21_o,
  output logic [2*VpuDataSSz-1:0] new_c22_o,
  output logic [2*VpuDataSSz-1:0] new_s21_o,
  output logic [2*VpuDataSSz-1:0] new_s22_o
);

  txfma_wallace1_orig u_orig (
    .pp1  (pp1_i),
    .pp2  (pp2_i),
    .pp3  (pp3_i),
    .pp4  (pp4_i),
    .pp5  (pp5_i),
    .pp6  (pp6_i),
    .pp7  (pp7_i),
    .pp8  (pp8_i),
    .pp9  (pp9_i),
    .pp10 (pp10_i),
    .pp11 (pp11_i),
    .pp12 (pp12_i),
    .pp13 (pp13_i),
    .pp14 (pp14_i),
    .pp15 (pp15_i),
    .pp16 (pp16_i),
    .mode (mode_i),
    .C21  (orig_c21_o),
    .C22  (orig_c22_o),
    .S21  (orig_s21_o),
    .S22  (orig_s22_o)
  );

  txfma_wallace1 u_new (
    .pp1_i  (pp1_i),
    .pp2_i  (pp2_i),
    .pp3_i  (pp3_i),
    .pp4_i  (pp4_i),
    .pp5_i  (pp5_i),
    .pp6_i  (pp6_i),
    .pp7_i  (pp7_i),
    .pp8_i  (pp8_i),
    .pp9_i  (pp9_i),
    .pp10_i (pp10_i),
    .pp11_i (pp11_i),
    .pp12_i (pp12_i),
    .pp13_i (pp13_i),
    .pp14_i (pp14_i),
    .pp15_i (pp15_i),
    .pp16_i (pp16_i),
    .mode_i (mode_i),
    .c21_o  (new_c21_o),
    .c22_o  (new_c22_o),
    .s21_o  (new_s21_o),
    .s22_o  (new_s22_o)
  );

endmodule : cosim_txfma_wallace1_tb
