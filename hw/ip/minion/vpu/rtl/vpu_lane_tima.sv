// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_lane_tima
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

  logic [VpuDataSSz-1:0] tenc_rd_data;
  logic                  tima_clk;
  dft_pkg::dft_t         dft_tieoff;
  /* verilator lint_off UNUSEDSIGNAL */  // The original wrapper exposed TE but hardwired 1'b0 into the internal clock gate. Keep the port for interface fidelity.
  logic te_unused;
  /* verilator lint_on UNUSEDSIGNAL */

  assign dft_tieoff = '0;
  assign te_unused = te_i;

  vpu_tensorb_rf u_tenb_rf (
    .clk_i         (clk_i),
    .rd_addr_i     (tenb_rd_addr_i),
    .rd_data_o     (tenb_out_o),
    .wr_en_i       (tenb_wr_en_i),
    .wr_en_early_i (tenb_wr_en_early_i),
    .wr_addr_i     (tenb_wr_addr_i),
    .wr_data_i     (tenb_wr_data_i)
  );

  vpu_tensorc_rf u_tenc_rf (
    .clk_i         (clk_i),
    .rd_addr_i     (tenc_rd_addr_i),
    .rd_data_o     (tenc_rd_data),
    .wr_en_i       (tenc_wr_en_i),
    .wr_en_early_i (tenc_wr_en_early_i),
    .wr_addr_i     (tenc_wr_addr_i),
    .wr_data_i     (tima_out_o)
  );

  prim_clk_gate u_cgate_tima (
    .clk_i (clk_i),
    .en_i  (tima_clk_en_i),
    .dft_i (dft_tieoff),
    .clk_o (tima_clk)
  );

  tima_top u_tima (
    .clk_i           (tima_clk),
    .f1_mul_en_i     (f1_tima_mul_en_i),
    .f1_in1_i        (tena_rd_data_i),
    .f1_in2_i        (tenb_out_o),
    .f1_in1_signed_i (tima_in1_signed_i),
    .f1_in2_signed_i (tima_in2_signed_i),
    .f2_in3_i        (tenc_rd_data),
    .f2_zap_opd3_i   (!f2_tima_zap_opdc_i),
    .f2_tima_en_i    (f2_tima_en_i),
    .f2_in3_en_i     (f2_tima_in3_en_i),
    .f3_out_o        (tima_out_o)
  );

endmodule : vpu_lane_tima
