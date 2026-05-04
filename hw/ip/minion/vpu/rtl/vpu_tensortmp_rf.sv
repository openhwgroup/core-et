// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_tensortmp_rf
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                      clk_i,
  input  logic [VpuTentmpAddrW-1:0] rd_addr_i,
  output logic [VpuDataSSz-1:0]     rd_data_o,
  input  logic                      wr_en_i,
  input  logic                      wr_en_early_i,
  input  logic [VpuTentmpAddrW-1:0] wr_addr_i,
  input  logic [VpuDataSSz-1:0]     wr_data_i
);

  prim_rf_1r1w_preview #(
    .Width (VpuDataSSz),
    .Depth (VpuTentmpSize)
  ) u_rf (
    .preview_clk_i       (clk_i),
    .rf_clk_i            (clk_i),
    .wr_data_en_1p_next_i(wr_en_early_i),
    .wr_en_i             (wr_en_i),
    .wr_addr_i           (wr_addr_i),
    .wr_data_i           (wr_data_i),
    .rd_addr_i           (rd_addr_i),
    .rd_data_o           (rd_data_o)
  );

endmodule : vpu_tensortmp_rf
