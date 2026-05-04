// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_128x64_1r1w_lram — 128x64 registered-read RAM wrapper.

module minion_dcache_128x64_1r1w_lram
  import ram_cfg_pkg::*;
(
  input  logic        clk_i,
  input  logic [6:0]  rd_addr_i,
  input  logic [6:0]  wr_addr_i,
  input  logic        rd_enable_i,
  input  logic        rd_enablelob_i,
  input  logic        rd_enablehib_i,
  input  logic        wr_enablehi_i,
  input  logic        wr_enablehi_pre_i,
  input  logic        wr_enablelo_i,
  input  logic        wr_enablelo_pre_i,
  output logic [63:0] rd_data_o,
  input  logic [63:0] wr_data_i,
  input  ram_cfg_t    ram_cfg_i
);

  logic        lo_rd_en;
  logic        hi_rd_en;
  logic        lo_wr_en;
  logic        hi_wr_en;
  logic        unused_ram_cfg;

  assign lo_rd_en = rd_enable_i && !rd_enablelob_i;
  assign hi_rd_en = rd_enable_i && !rd_enablehib_i;
  assign lo_wr_en = wr_enablelo_i;
  assign hi_wr_en = wr_enablehi_i;

  prim_rf_1r1w_reg_preview #(
    .Width (32),
    .Depth (128)
  ) u_rf_lo (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (wr_enablelo_pre_i),
    .wr_data_i            (wr_data_i[31:0]),
    .wr_addr_i            (wr_addr_i),
    .wr_en_i              (lo_wr_en),
    .rd_addr_i            (rd_addr_i),
    .rd_en_i              (lo_rd_en),
    .rd_data_o            (rd_data_o[31:0])
  );

  prim_rf_1r1w_reg_preview #(
    .Width (32),
    .Depth (128)
  ) u_rf_hi (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (wr_enablehi_pre_i),
    .wr_data_i            (wr_data_i[63:32]),
    .wr_addr_i            (wr_addr_i),
    .wr_en_i              (hi_wr_en),
    .rd_addr_i            (rd_addr_i),
    .rd_en_i              (hi_rd_en),
    .rd_data_o            (rd_data_o[63:32])
  );

  assign unused_ram_cfg = ^ram_cfg_i;

endmodule : minion_dcache_128x64_1r1w_lram
