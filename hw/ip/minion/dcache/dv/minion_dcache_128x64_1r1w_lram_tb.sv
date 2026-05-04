// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_128x64_1r1w_lram — Verilator wrapper with dummy reset for sim_ctrl.

module minion_dcache_128x64_1r1w_lram_tb
  import ram_cfg_pkg::*;
(
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
  input  logic [63:0] wr_data_i,
  output logic [63:0] rd_data_o
);

  logic unused;
  ram_cfg_t ram_cfg;
  assign unused = rst_ni;
  assign ram_cfg = '0;

  minion_dcache_128x64_1r1w_lram u_dut (
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
    .rd_data_o,
    .wr_data_i,
    .ram_cfg_i      (ram_cfg)
  );

endmodule
