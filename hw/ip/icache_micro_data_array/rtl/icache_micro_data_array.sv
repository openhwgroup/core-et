// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_micro_data_array #(
  parameter int unsigned Width = icache_geom_pkg::IcacheBlockBits + 2,
  parameter int unsigned Entries = icache_geom_pkg::IcacheMicroCacheSize
) (
  input  logic                clk_i,
  input  logic                f3_rd_en_i,
  input  logic [Entries-1:0]  f3_rd_addr_dec_i,
  output logic [Width-1:0]    f3_rd_data_o,
  input  logic                f0_wr_en_i,
  input  logic                f0_wr_en_early_i,
  input  logic [Entries-1:0]  f0_wr_addr_dec_i,
  input  logic [Width-1:0]    f0_wr_data_i,
  input  logic [Entries-1:0]  dbg_addr_dec_i,
  input  logic                dbg_write_en_i,
  input  logic                dbg_write_en_early_i,
  input  logic [Width-1:0]    dbg_write_data_i,
  output logic                dbg_write_ready_o,
  input  logic                dbg_read_en_i,
  output logic [Width-1:0]    dbg_read_data_o,
  output logic                dbg_read_ready_o
);

  logic [Entries-1:0] rd_addr_dec;
  logic [Width-1:0]   rd_data;
  logic               wr_en;
  logic               wr_en_early;
  logic [Entries-1:0] wr_addr_dec;
  logic [Width-1:0]   wr_data;

  assign rd_addr_dec = f3_rd_en_i ? f3_rd_addr_dec_i : dbg_addr_dec_i;
  assign f3_rd_data_o = rd_data;
  assign wr_en = f0_wr_en_i || dbg_write_en_i;
  assign wr_en_early = f0_wr_en_early_i || dbg_write_en_early_i;
  assign wr_addr_dec = f0_wr_en_i ? f0_wr_addr_dec_i : dbg_addr_dec_i;
  assign wr_data = f0_wr_en_i ? f0_wr_data_i : dbg_write_data_i;

  prim_rf_1r1w_dec_preview #(
    .Width   (Width),
    .Entries (Entries)
  ) u_line_storage (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (wr_en_early),
    .rd_addr_i            (rd_addr_dec),
    .rd_data_o            (rd_data),
    .wr_data_i            (wr_data),
    .wr_addr_i            (wr_addr_dec),
    .wr_en_i              (wr_en)
  );

  assign dbg_read_data_o = rd_data;
  assign dbg_read_ready_o = dbg_read_en_i && !f3_rd_en_i;
  assign dbg_write_ready_o = dbg_write_en_i && !f0_wr_en_i;

endmodule : icache_micro_data_array
