// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_icache_micro_tag_array_tb
  import minion_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic [IcacheMicroTagSize-1:0]    rd_tag_i,
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
  output logic [IcacheMicroCacheSize-1:0]  new_rd_hit_array_o,
  output logic [IcacheMicroCacheSize-1:0]  orig_rd_hit_array_o,
  output logic                             new_apb_pready_o,
  output logic                             orig_apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] new_apb_prdata_o,
  output logic [BpamShireApbDataWidth-1:0] orig_apb_prdata_o,
  output logic                             new_apb_pslverr_o,
  output logic                             orig_apb_pslverr_o
);

  logic [IcacheMicroCacheSize-1:0] wr_victim;
  logic [IcacheMicroCacheSize-1:0] miss_victim;
  logic                            reset;

  assign reset = ~rst_ni;

  always_comb begin
    wr_victim = '0;
    wr_victim[wr_entry_i] = 1'b1;
    miss_victim = '0;
    miss_victim[miss_entry_i] = 1'b1;
  end

  icache_micro_tag_array u_new (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .f2_rd_tag_i(rd_tag_i),
    .f2_rd_hit_array_o(new_rd_hit_array_o),
    .f0_wr_early_valid_i(wr_early_valid_i),
    .f0_wr_valid_i(wr_valid_i),
    .f0_wr_victim_i(wr_victim),
    .f0_wr_tag_i(wr_tag_i),
    .f2_miss_i(miss_i),
    .f2_miss_victim_i(miss_victim),
    .f0_miss_state_i(miss_state_i),
    .f0_flush_data_i(flush_data_i),
    .apb_paddr_i(apb_paddr_i),
    .apb_pwrite_i(apb_pwrite_i),
    .apb_psel_i(apb_psel_i),
    .apb_penable_i(apb_penable_i),
    .apb_pwdata_i(apb_pwdata_i),
    .apb_pready_o(new_apb_pready_o),
    .apb_prdata_o(new_apb_prdata_o),
    .apb_pslverr_o(new_apb_pslverr_o)
  );

  icache_micro_tag_array_orig u_orig (
    .clock(clk_i),
    .reset(reset),
    .f2_rd_tag(rd_tag_i),
    .f2_rd_hit_array(orig_rd_hit_array_o),
    .f0_wr_early_valid(wr_early_valid_i),
    .f0_wr_valid(wr_valid_i),
    .f0_wr_victim(wr_victim),
    .f0_wr_tag(wr_tag_i),
    .f2_miss(miss_i),
    .f2_miss_victim(miss_victim),
    .f0_miss_state(miss_state_i),
    .f0_flush_data(flush_data_i),
    .apb_paddr(apb_paddr_i),
    .apb_pwrite(apb_pwrite_i),
    .apb_psel(apb_psel_i),
    .apb_penable(apb_penable_i),
    .apb_pwdata(apb_pwdata_i),
    .apb_pready(orig_apb_pready_o),
    .apb_prdata(orig_apb_prdata_o),
    .apb_pslverr(orig_apb_pslverr_o)
  );

endmodule
