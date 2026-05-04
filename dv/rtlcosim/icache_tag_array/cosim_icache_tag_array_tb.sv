// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_icache_tag_array_tb
  import minion_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic                             read_valid_i,
  input  logic [IcacheSetAddrWidth-1:0]    read_set_i,
  input  logic [IcachePaTagSize-1:0]       read_tag_i,
  input  logic                             write_early_valid_i,
  input  logic                             write_valid_i,
  input  logic                             write_req_valid_i,
  input  logic [IcacheSetAddrWidth-1:0]    write_req_set_i,
  input  logic [IcacheWayAddrWidth-1:0]    write_req_way_i,
  input  logic [IcachePaTagSize-1:0]       write_req_tag_i,
  input  logic                             f1_miss_i,
  input  logic [IcacheSetAddrWidth-1:0]    f1_miss_idx_i,
  input  logic [IcacheWayAddrWidth-1:0]    f1_miss_way_i,
  input  icache_miss_state_e               miss_state_i,
  input  logic                             flush_data_i,
  input  logic [$clog2(IcacheWays/2)-1:0]  apb_pair_i,
  input  logic [IcacheSetAddrWidth-1:0]    apb_set_i,
  input  logic                             apb_pwrite_i,
  input  logic                             apb_psel_i,
  input  logic                             apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic [IcacheWays-1:0]            new_f1_read_tag_hit_o,
  output logic [IcacheWays-1:0]            orig_f1_read_tag_hit_o,
  output logic                             new_apb_pready_o,
  output logic                             orig_apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] new_apb_prdata_o,
  output logic [BpamShireApbDataWidth-1:0] orig_apb_prdata_o,
  output logic                             new_apb_pslverr_o,
  output logic                             orig_apb_pslverr_o
);

  logic [PaSize-1:0]                 f0_read_paddr;
  logic [IcacheDbgTagAddrWidth-1:0]  apb_paddr;
  icache_tag_write_req_t             f0_write_req;
  logic                              reset;

  assign reset = ~rst_ni;

  always_comb begin
    f0_read_paddr = '0;
    f0_read_paddr[IcacheBlockAddrSize +: IcacheSetAddrWidth] = read_set_i;
    f0_read_paddr[PaSize-1:IcacheDataAddrSize] = read_tag_i;
  end

  assign apb_paddr = {apb_set_i, apb_pair_i};

  always_comb begin
    f0_write_req       = '0;
    f0_write_req.valid = write_req_valid_i;
    f0_write_req.idx   = write_req_set_i;
    f0_write_req.way   = write_req_way_i;
    f0_write_req.data  = write_req_tag_i;
  end

  icache_tag_array u_new (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .f0_read_valid_i(read_valid_i),
    .f0_read_paddr_i(f0_read_paddr),
    .f1_read_tag_hit_o(new_f1_read_tag_hit_o),
    .f0_write_early_valid_i(write_early_valid_i),
    .f0_write_valid_i(write_valid_i),
    .f0_write_req_i(f0_write_req),
    .f1_miss_i(f1_miss_i),
    .f1_miss_idx_i(f1_miss_idx_i),
    .f1_miss_way_i(f1_miss_way_i),
    .f0_miss_state_i(miss_state_i),
    .f0_flush_data_i(flush_data_i),
    .apb_paddr_i(apb_paddr),
    .apb_pwrite_i(apb_pwrite_i),
    .apb_psel_i(apb_psel_i),
    .apb_penable_i(apb_penable_i),
    .apb_pwdata_i(apb_pwdata_i),
    .apb_pready_o(new_apb_pready_o),
    .apb_prdata_o(new_apb_prdata_o),
    .apb_pslverr_o(new_apb_pslverr_o)
  );

  icache_tag_array_orig u_orig (
    .clock(clk_i),
    .reset(reset),
    .f0_read_valid(read_valid_i),
    .f0_read_paddr(f0_read_paddr),
    .f1_read_tag_hit(orig_f1_read_tag_hit_o),
    .f0_write_early_valid(write_early_valid_i),
    .f0_write_valid(write_valid_i),
    .f0_write_req(f0_write_req),
    .f1_miss(f1_miss_i),
    .f1_miss_idx(f1_miss_idx_i),
    .f1_miss_way(f1_miss_way_i),
    .f0_miss_state(miss_state_i),
    .f0_flush_data(flush_data_i),
    .apb_paddr(apb_paddr),
    .apb_pwrite(apb_pwrite_i),
    .apb_psel(apb_psel_i),
    .apb_penable(apb_penable_i),
    .apb_pwdata(apb_pwdata_i),
    .apb_pready(orig_apb_pready_o),
    .apb_prdata(orig_apb_prdata_o),
    .apb_pslverr(orig_apb_pslverr_o)
  );

endmodule
