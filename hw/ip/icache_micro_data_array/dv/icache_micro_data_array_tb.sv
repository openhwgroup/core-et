// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_micro_data_array_tb #(
  parameter int unsigned Width = 18,
  parameter int unsigned Entries = 4
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

  icache_micro_data_array #(
    .Width   (Width),
    .Entries (Entries)
  ) u_dut (
    .clk_i,
    .f3_rd_en_i,
    .f3_rd_addr_dec_i,
    .f3_rd_data_o,
    .f0_wr_en_i,
    .f0_wr_en_early_i,
    .f0_wr_addr_dec_i,
    .f0_wr_data_i,
    .dbg_addr_dec_i,
    .dbg_write_en_i,
    .dbg_write_en_early_i,
    .dbg_write_data_i,
    .dbg_write_ready_o,
    .dbg_read_en_i,
    .dbg_read_data_o,
    .dbg_read_ready_o
  );

endmodule : icache_micro_data_array_tb
