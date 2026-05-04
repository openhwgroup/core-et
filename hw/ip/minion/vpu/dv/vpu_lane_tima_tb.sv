// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_lane_tima_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                  te_i,
  input  logic                  clk_i,
  input  logic                  tima_clk_en_i,
  input  logic                  f1_tima_mul_en_i,
  input  logic                  tima_in1_signed_i,
  input  logic                  tima_in2_signed_i,
  input  logic                  f2_tima_en_i,
  input  logic                  f2_tima_in3_en_i,
  input  logic                  f2_tima_zap_opdc_i,
  input  logic [VpuDataSSz-1:0] tena_rd_data_i,
  input  logic                  tenb_wr_en_i,
  input  logic                  tenb_wr_en_early_i,
  input  logic [VpuTenbRfAddrW-1:0] tenb_rd_addr_i,
  input  logic [VpuTenbRfAddrW-1:0] tenb_wr_addr_i,
  input  logic [VpuDataSSz-1:0] tenb_wr_data_i,
  input  logic                  tenc_wr_en_i,
  input  logic                  tenc_wr_en_early_i,
  input  logic [VpuTencAddrW-1:0] tenc_rd_addr_i,
  input  logic [VpuTencAddrW-1:0] tenc_wr_addr_i,
  output logic [VpuDataSSz-1:0] tenb_out_o,
  output logic [VpuDataSSz-1:0] tima_out_o
);

  vpu_lane_tima u_dut (
    .te_i,
    .clk_i,
    .tima_clk_en_i,
    .f1_tima_mul_en_i,
    .tima_in1_signed_i,
    .tima_in2_signed_i,
    .f2_tima_en_i,
    .f2_tima_in3_en_i,
    .f2_tima_zap_opdc_i,
    .tena_rd_data_i,
    .tenb_wr_en_i,
    .tenb_wr_en_early_i,
    .tenb_rd_addr_i,
    .tenb_wr_addr_i,
    .tenb_wr_data_i,
    .tenc_wr_en_i,
    .tenc_wr_en_early_i,
    .tenc_rd_addr_i,
    .tenc_wr_addr_i,
    .tenb_out_o,
    .tima_out_o
  );

endmodule : vpu_lane_tima_tb
