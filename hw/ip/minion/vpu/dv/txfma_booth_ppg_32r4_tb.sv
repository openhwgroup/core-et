// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_booth_ppg_32r4_tb
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0] mpcnd_i,
  input  logic [VpuDataSSz-1:0] mplr_i,
  input  logic                  mode_i,
  input  logic                  sign_i,
  output logic [VpuDataSSz-1:0] sxt_o,
  output logic [VpuDataSSz-1:0] cin_o,
  output logic [VpuDataSSz:0]   pp1_o,
  output logic [VpuDataSSz:0]   pp2_o,
  output logic [VpuDataSSz:0]   pp3_o,
  output logic [VpuDataSSz:0]   pp4_o,
  output logic [VpuDataSSz:0]   pp5_o,
  output logic [VpuDataSSz:0]   pp6_o,
  output logic [VpuDataSSz:0]   pp7_o,
  output logic [VpuDataSSz:0]   pp8_o,
  output logic [VpuDataSSz:0]   pp9_o,
  output logic [VpuDataSSz:0]   pp10_o,
  output logic [VpuDataSSz:0]   pp11_o,
  output logic [VpuDataSSz:0]   pp12_o,
  output logic [VpuDataSSz:0]   pp13_o,
  output logic [VpuDataSSz:0]   pp14_o,
  output logic [VpuDataSSz:0]   pp15_o,
  output logic [VpuDataSSz:0]   pp16_o,
  output logic [VpuDataSSz:0]   pp17_o
);

  txfma_booth_ppg_32r4 u_dut (
    .mpcnd_i,
    .mplr_i,
    .mode_i,
    .sign_i,
    .sxt_o,
    .cin_o,
    .pp1_o,
    .pp2_o,
    .pp3_o,
    .pp4_o,
    .pp5_o,
    .pp6_o,
    .pp7_o,
    .pp8_o,
    .pp9_o,
    .pp10_o,
    .pp11_o,
    .pp12_o,
    .pp13_o,
    .pp14_o,
    .pp15_o,
    .pp16_o,
    .pp17_o
  );

endmodule : txfma_booth_ppg_32r4_tb
