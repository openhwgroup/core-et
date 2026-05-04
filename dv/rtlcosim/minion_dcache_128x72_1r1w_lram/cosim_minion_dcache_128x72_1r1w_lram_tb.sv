// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_128x72_1r1w_lram vs new minion_dcache_128x72_1r1w_lram.

module cosim_minion_dcache_128x72_1r1w_lram_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [6:0]  rd_addr_i,
  input  logic [6:0]  wr_addr_i,
  input  logic        rd_enable_i,
  input  logic        rd_enablelob_i,
  input  logic        rd_enablehib_i,
  input  logic        wr_enablehi_i,
  input  logic        wr_enablehi_pre_i,
  input  logic        wr_enablelo_i,
  input  logic        wr_enablelo_pre_i,
  input  logic [35:0] wr_data_lo_i,
  input  logic [35:0] wr_data_hi_i,
  output logic [35:0] new_rd_data_lo_o,
  output logic [35:0] new_rd_data_hi_o,
  output logic [35:0] orig_rd_data_lo_o,
  output logic [35:0] orig_rd_data_hi_o
);

  logic unused;
  logic [71:0] new_rd_data;
  logic [71:0] orig_rd_data;
  ram_cfg_pkg::ram_cfg_t ram_cfg;

  assign unused = rst_ni;
  assign ram_cfg = '0;
  assign new_rd_data_lo_o = new_rd_data[35:0];
  assign new_rd_data_hi_o = new_rd_data[71:36];
  assign orig_rd_data_lo_o = orig_rd_data[35:0];
  assign orig_rd_data_hi_o = orig_rd_data[71:36];

  minion_dcache_128x72_1r1w_lram u_new (
    .clk_i,
    .rd_addr_i,
    .wr_addr_i,
    .rd_enable_i,
    .rd_enablelob_i,
    .rd_enablehib_i,
    .wr_enablehi_i,
    .wr_enablehi_pre_i,
    .wr_enablelo_i,
    .wr_enablelo_pre_i,
    .rd_data_o (new_rd_data),
    .wr_data_i ({wr_data_hi_i, wr_data_lo_i}),
    .ram_cfg_i (ram_cfg)
  );

  dcache_128x72_1r1w_lram u_orig (
    .clk            (clk_i),
    .rd_addr        (rd_addr_i),
    .wr_addr        (wr_addr_i),
    .rd_enable      (rd_enable_i),
    .rd_enablelob   (rd_enablelob_i),
    .rd_enablehib   (rd_enablehib_i),
    .wr_enablehi    (wr_enablehi_i),
    .wr_enablehi_pre(wr_enablehi_pre_i),
    .wr_enablelo    (wr_enablelo_i),
    .wr_enablelo_pre(wr_enablelo_pre_i),
    .rd_data        (orig_rd_data),
    .wr_data        ({wr_data_hi_i, wr_data_lo_i})
  );

endmodule
