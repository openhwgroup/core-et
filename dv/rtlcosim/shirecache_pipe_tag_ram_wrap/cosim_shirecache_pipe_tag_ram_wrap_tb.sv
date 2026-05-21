// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_tag_ram_wrap vs
// shirecache_pipe_tag_ram_wrap.

`include "soc.vh"

module cosim_shirecache_pipe_tag_ram_wrap_tb
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,

  input  logic                         dft_sram_clk_i,
  input  logic                         clock_en_i,
  input  logic [RamDelaySize-1:0]      ram_delay_i,
  input  logic                         rd_en_i,
  input  logic                         wr_en_i,
  input  logic [TagRamAddrSize-1:0]    addr_i,
  input  logic [TagRamDataSize-1:0]    wr_data_i,

  input  logic                         dft_scanmode_i,
  input  logic                         dft_scan_reset_n_i,
  input  logic                         dft_sram_clk_override_i,
  input  logic                         dft_ram_rei_i,
  input  logic                         dft_ram_wei_i,

  input  logic                         ram_cfg_test_en_i,
  input  logic [3:0]                   ram_cfg_rm_i,
  input  logic                         ram_cfg_rme_i,
  input  logic [1:0]                   ram_cfg_ra_i,
  input  logic [1:0]                   ram_cfg_wa_i,
  input  logic [2:0]                   ram_cfg_wpulse_i,
  input  logic                         ram_cfg_deep_sleep_i,
  input  logic                         ram_cfg_shut_down_i,

  output logic [TagRamDataSize-1:0]    orig_rd_data_o,
  output logic [TagRamDataSize-1:0]    new_rd_data_o
);

  wire reset = !rst_ni;

  dft_t new_dft;
  always_comb begin
    new_dft.scanmode = dft_scanmode_i;
    new_dft.scan_reset_n = dft_scan_reset_n_i;
    new_dft.sram_clk_override = dft_sram_clk_override_i;
    new_dft.ram_rei = dft_ram_rei_i;
    new_dft.ram_wei = dft_ram_wei_i;
  end

  ram_cfg_t new_ram_cfg;
  always_comb begin
    new_ram_cfg.test_en = ram_cfg_test_en_i;
    new_ram_cfg.rm = ram_cfg_rm_i;
    new_ram_cfg.rme = ram_cfg_rme_i;
    new_ram_cfg.ra = ram_cfg_ra_i;
    new_ram_cfg.wa = ram_cfg_wa_i;
    new_ram_cfg.wpulse = ram_cfg_wpulse_i;
    new_ram_cfg.deep_sleep = ram_cfg_deep_sleep_i;
    new_ram_cfg.shut_down = ram_cfg_shut_down_i;
  end

  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  always_comb begin
    orig_ram_cfg = '0;
    orig_ram_cfg.dft__ram_rei = dft_ram_rei_i;
    orig_ram_cfg.dft__ram_wei = dft_ram_wei_i;
    orig_ram_cfg.cfg1.sc_mbt_test1 = ram_cfg_test_en_i;
    orig_ram_cfg.cfg1.sc_mbt_rm = ram_cfg_rm_i;
    orig_ram_cfg.cfg1.sc_mbt_rme = ram_cfg_rme_i;
    orig_ram_cfg.cfg1.sc_mbt_ra = ram_cfg_ra_i;
    orig_ram_cfg.cfg1.sc_mbt_wa = {1'b0, ram_cfg_wa_i};
    orig_ram_cfg.cfg1.sc_mbt_wpulse = ram_cfg_wpulse_i;
  end

  shire_cache_pipe_tag_ram_wrap orig_tag_ram_wrap (
    .clock                   (clk_i),
    .reset                   (reset),
    .dft__sram_clock         (dft_sram_clk_i),
    .dft__clk_override       (dft_sram_clk_override_i),
    .clock_en                (clock_en_i),
    .rd_en                   (rd_en_i),
    .wr_en                   (wr_en_i),
    .addr                    (addr_i),
    .wr_data                 (wr_data_i),
    .rd_data                 (orig_rd_data_o),
    .esr_sc_ram_deep_sleep   (ram_cfg_deep_sleep_i),
    .esr_sc_ram_shut_down    (ram_cfg_shut_down_i),
    .esr_shire_cache_ram_cfg (orig_ram_cfg)
  );

  shirecache_pipe_tag_ram_wrap #(
    .SetsPerSubBank (SetsPerSubBank)
  ) new_tag_ram_wrap (
    .clk_i,
    .rst_ni,
    .dft_sram_clk_i,
    .clock_en_i,
    .ram_delay_i,
    .rd_en_i,
    .wr_en_i,
    .addr_i,
    .wr_data_i,
    .rd_data_o               (new_rd_data_o),
    .ram_cfg_i               (new_ram_cfg),
    .dft_i                   (new_dft)
  );

endmodule
