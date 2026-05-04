// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_wallace2_tb
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0]   sxt_i,
  input  logic [VpuDataSSz-1:0]   cin_i,
  input  logic [VpuDataSSz:0]     pp17_i,
  input  logic [2*VpuDataSSz-1:0] c21_i,
  input  logic [2*VpuDataSSz-1:0] c22_i,
  input  logic [2*VpuDataSSz-1:0] s21_i,
  input  logic [2*VpuDataSSz-1:0] s22_i,
  output logic [2*VpuDataSSz-1:0] orig_c41_o,
  output logic [2*VpuDataSSz-1:0] orig_s41_o,
  output logic [2*VpuDataSSz-1:0] new_c41_o,
  output logic [2*VpuDataSSz-1:0] new_s41_o
);

  txfma_wallace2_orig u_orig (
    .Sxt  (sxt_i),
    .Cin  (cin_i),
    .pp17 (pp17_i),
    .C21  (c21_i),
    .C22  (c22_i),
    .S21  (s21_i),
    .S22  (s22_i),
    .C41  (orig_c41_o),
    .S41  (orig_s41_o)
  );

  txfma_wallace2 u_new (
    .sxt_i,
    .cin_i,
    .pp17_i,
    .c21_i,
    .c22_i,
    .s21_i,
    .s22_i,
    .c41_o (new_c41_o),
    .s41_o (new_s41_o)
  );

endmodule : cosim_txfma_wallace2_tb
