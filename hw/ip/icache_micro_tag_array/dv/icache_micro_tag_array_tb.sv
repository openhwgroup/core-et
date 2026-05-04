// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_micro_tag_array_tb
  import minion_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic [IcacheMicroTagSize-1:0]    rd_tag_i,
  output logic [IcacheMicroCacheSize-1:0]  rd_hit_array_o,
  input  logic                             wr_early_valid_i,
  input  logic                             wr_valid_i,
  input  logic [IcacheMicroCacheAdWidth-1:0] wr_entry_i,
  input  logic [IcacheMicroTagSize-1:0]    wr_tag_i,
  input  logic                             miss_i,
  input  logic [IcacheMicroCacheAdWidth-1:0] miss_entry_i,
  input  icache_miss_state_e               miss_state_i,
  input  logic                             flush_data_i,
  input  logic [IcacheDbgUtagAddrWidth-1:0] apb_paddr_i,
  input  logic                             apb_pwrite_i,
  input  logic                             apb_psel_i,
  input  logic                             apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic                             apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] apb_prdata_o,
  output logic                             apb_pslverr_o
);

  logic [IcacheMicroCacheSize-1:0] wr_victim;
  logic [IcacheMicroCacheSize-1:0] miss_victim;

  always_comb begin
    wr_victim = '0;
    wr_victim[wr_entry_i] = 1'b1;
    miss_victim = '0;
    miss_victim[miss_entry_i] = 1'b1;
  end

  icache_micro_tag_array u_dut (
    .clk_i,
    .rst_ni,
    .f2_rd_tag_i       (rd_tag_i),
    .f2_rd_hit_array_o (rd_hit_array_o),
    .f0_wr_early_valid_i(wr_early_valid_i),
    .f0_wr_valid_i     (wr_valid_i),
    .f0_wr_victim_i    (wr_victim),
    .f0_wr_tag_i       (wr_tag_i),
    .f2_miss_i         (miss_i),
    .f2_miss_victim_i  (miss_victim),
    .f0_miss_state_i   (miss_state_i),
    .f0_flush_data_i   (flush_data_i),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o,
    .apb_prdata_o,
    .apb_pslverr_o
  );

endmodule
