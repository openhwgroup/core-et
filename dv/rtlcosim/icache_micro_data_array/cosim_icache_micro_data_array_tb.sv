// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_icache_micro_data_array_tb #(
  parameter int unsigned Width = 18,
  parameter int unsigned Entries = 4
) (
  input  logic                clk_i,
  input  logic                f3_rd_en_i,
  input  logic [Entries-1:0]  f3_rd_addr_dec_i,
  output logic [Width-1:0]    new_f3_rd_data_o,
  output logic [Width-1:0]    orig_f3_rd_data_o,
  input  logic                f0_wr_en_i,
  input  logic                f0_wr_en_early_i,
  input  logic [Entries-1:0]  f0_wr_addr_dec_i,
  input  logic [Width-1:0]    f0_wr_data_i,
  input  logic [Entries-1:0]  dbg_addr_dec_i,
  input  logic                dbg_write_en_i,
  input  logic                dbg_write_en_early_i,
  input  logic [Width-1:0]    dbg_write_data_i,
  output logic                new_dbg_write_ready_o,
  output logic                orig_dbg_write_ready_o,
  input  logic                dbg_read_en_i,
  output logic [Width-1:0]    new_dbg_read_data_o,
  output logic [Width-1:0]    orig_dbg_read_data_o,
  output logic                new_dbg_read_ready_o,
  output logic                orig_dbg_read_ready_o
);

  icache_micro_data_array #(
    .Width   (Width),
    .Entries (Entries)
  ) u_new (
    .clk_i,
    .f3_rd_en_i,
    .f3_rd_addr_dec_i,
    .f3_rd_data_o        (new_f3_rd_data_o),
    .f0_wr_en_i,
    .f0_wr_en_early_i,
    .f0_wr_addr_dec_i,
    .f0_wr_data_i,
    .dbg_addr_dec_i,
    .dbg_write_en_i,
    .dbg_write_en_early_i,
    .dbg_write_data_i,
    .dbg_write_ready_o   (new_dbg_write_ready_o),
    .dbg_read_en_i,
    .dbg_read_data_o     (new_dbg_read_data_o),
    .dbg_read_ready_o    (new_dbg_read_ready_o)
  );

  icache_micro_data_array_orig #(
    .WIDTH   (Width),
    .ENTRIES (Entries)
  ) u_orig (
    .clock              (clk_i),
    .f3_rd_en           (f3_rd_en_i),
    .f3_rd_addr_dec     (f3_rd_addr_dec_i),
    .f3_rd_data         (orig_f3_rd_data_o),
    .f0_wr_en           (f0_wr_en_i),
    .f0_wr_en_early     (f0_wr_en_early_i),
    .f0_wr_addr_dec     (f0_wr_addr_dec_i),
    .f0_wr_data         (f0_wr_data_i),
    .dbg_addr_dec       (dbg_addr_dec_i),
    .dbg_write_en       (dbg_write_en_i),
    .dbg_write_en_early (dbg_write_en_early_i),
    .dbg_write_data     (dbg_write_data_i),
    .dbg_write_ready    (orig_dbg_write_ready_o),
    .dbg_read_en        (dbg_read_en_i),
    .dbg_read_data      (orig_dbg_read_data_o),
    .dbg_read_ready     (orig_dbg_read_ready_o)
  );

endmodule : cosim_icache_micro_data_array_tb
