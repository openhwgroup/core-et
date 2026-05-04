// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_tag_ram_wrap vs shirecache_pipe_tag_ram_wrap.

`include "soc.vh"

module cosim_tag_ram_tb #(
  parameter int unsigned SETS = `SC_SETS_PER_SUB_BANK
) (
  input  logic                               clk_i,
  input  logic                               rst_ni,
  input  logic                               rd_en_i,
  input  logic                               wr_en_i,
  input  logic [$clog2(SETS)-1:0]            addr_i,
  input  logic [`SC_TAG_RAM_DATA_SIZE-1:0]   wr_data_i,
  output logic [`SC_TAG_RAM_DATA_SIZE-1:0]   new_rd_data_o,
  output logic [`SC_TAG_RAM_DATA_SIZE-1:0]   orig_rd_data_o
);

  logic reset;
  assign reset = ~rst_ni;

  esr_shire_cache_ram_cfg_t ram_cfg;
  assign ram_cfg = '0;

  // ── New ───────────────────────────────────────────────

  shirecache_pipe_tag_ram_wrap #(.SetsPerSubBank(SETS)) u_new (
    .clk_i,
    .rst_ni,
    .dft_sram_clk_i (1'b0),
    .clock_en_i (rd_en_i | wr_en_i),
    .ram_delay_i (shirecache_pkg::RamDelaySize'(2)),
    .rd_en_i,
    .wr_en_i,
    .addr_i,
    .wr_data_i,
    .rd_data_o  (new_rd_data_o),
    .ram_cfg_i  (ram_cfg_pkg::ram_cfg_t'('0)),
    .dft_i      (dft_pkg::dft_t'('0))
  );

  // ── Original ──────────────────────────────────────────

  shire_cache_pipe_tag_ram_wrap #(.SETS_PER_SUB_BANK(SETS)) u_orig (
    .clock                  (clk_i),
    .reset                  (reset),
    .dft__sram_clock        (1'b0),
    .dft__clk_override      (1'b0),
    .clock_en               (rd_en_i | wr_en_i),
    .rd_en                  (rd_en_i),
    .wr_en                  (wr_en_i),
    .addr                   (addr_i),
    .wr_data                (wr_data_i),
    .rd_data                (orig_rd_data_o),
    .esr_sc_ram_deep_sleep  (1'b0),
    .esr_sc_ram_shut_down   (1'b0),
    .esr_shire_cache_ram_cfg(ram_cfg)
  );

endmodule
