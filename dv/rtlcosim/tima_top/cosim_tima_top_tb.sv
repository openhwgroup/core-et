// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_tima_top_tb
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
  output logic [VpuDataSSz-1:0] orig_f3_out_o,
  output logic [VpuDataSSz-1:0] new_f3_out_o
);

  tima_top_orig u_orig (
    .clock         (clk_i),
    .f1_mul_en     (f1_mul_en_i),
    .f1_in1        (f1_in1_i),
    .f1_in2        (f1_in2_i),
    .f1_in1_signed (f1_in1_signed_i),
    .f1_in2_signed (f1_in2_signed_i),
    .f2_in3        (f2_in3_i),
    .f2_zap_opd3   (f2_zap_opd3_i),
    .f2_tima_en    (f2_tima_en_i),
    .f2_in3_en     (f2_in3_en_i),
    .f3_out        (orig_f3_out_o)
  );

  tima_top u_new (
    .clk_i           (clk_i),
    .f1_mul_en_i     (f1_mul_en_i),
    .f1_in1_i        (f1_in1_i),
    .f1_in2_i        (f1_in2_i),
    .f1_in1_signed_i (f1_in1_signed_i),
    .f1_in2_signed_i (f1_in2_signed_i),
    .f2_in3_i        (f2_in3_i),
    .f2_zap_opd3_i   (f2_zap_opd3_i),
    .f2_tima_en_i    (f2_tima_en_i),
    .f2_in3_en_i     (f2_in3_en_i),
    .f3_out_o        (new_f3_out_o)
  );

endmodule : cosim_tima_top_tb
