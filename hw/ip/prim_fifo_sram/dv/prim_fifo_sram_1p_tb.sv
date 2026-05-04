// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Wrapper to test prim_fifo_sram with Ports=1 (single-port mode).

module prim_fifo_sram_1p_tb
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic         clk_i,
  input  logic         rst_ni,
  input  logic         wr_en_i,
  input  logic [63:0]  wr_data_i,
  output logic         wr_done_o,
  input  logic         rd_en_i,
  output logic         rd_valid_o,
  output logic [63:0]  rd_data_o,
  input  ram_cfg_t     ram_cfg_i,
  input  dft_t         dft_i
);

  prim_fifo_sram #(
    .Width (64),
    .Depth (64),
    .Ports (1)
  ) u_fifo (
    .clk_i,
    .rst_ni,
    .wr_en_i,
    .wr_data_i,
    .wr_done_o,
    .rd_en_i,
    .rd_valid_o,
    .rd_data_o,
    .ram_cfg_i,
    .dft_i
  );

endmodule
