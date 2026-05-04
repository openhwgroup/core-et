// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module tima_top_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                  clk_i,
  input  logic                  f1_mul_en_i,
  input  logic [VpuDataSSz-1:0] f1_in1_i,
  input  logic [VpuDataSSz-1:0] f1_in2_i,
  input  logic                  f1_in1_signed_i,
  input  logic                  f1_in2_signed_i,
  input  logic [VpuDataSSz-1:0] f2_in3_i,
  input  logic                  f2_zap_opd3_i,
  input  logic                  f2_tima_en_i,
  input  logic                  f2_in3_en_i,
  output logic [VpuDataSSz-1:0] f3_out_o
);

  tima_top u_dut (
    .clk_i,
    .f1_mul_en_i,
    .f1_in1_i,
    .f1_in2_i,
    .f1_in1_signed_i,
    .f1_in2_signed_i,
    .f2_in3_i,
    .f2_zap_opd3_i,
    .f2_tima_en_i,
    .f2_in3_en_i,
    .f3_out_o
  );

endmodule : tima_top_tb
