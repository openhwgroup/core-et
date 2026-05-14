// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_sub_bank_mem vs
// shirecache_pipe_sub_bank_mem.

`include "soc.vh"

module cosim_shirecache_pipe_sub_bank_mem_tb
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
(
  input  logic                              clk_i,
  input  logic                              rst_ni,

  input  logic [RamDelaySize-1:0]           esr_ram_delay_i,

  input  logic                              ts_rd_en_i,
  input  logic                              ts_wr_en_i,
  input  logic [TagStateRamAddrSize-1:0]    ts_rd_addr_i,
  input  logic [TagStateRamAddrSize-1:0]    ts_wr_addr_i,
  input  logic [TagStateRamDataSize-1:0]    ts_wr_data_i,
  input  logic [DvTransIdSize-1:0]          ts_rd_trans_id_i,
  input  logic [ReqqIdSize-1:0]             ts_rd_reqq_id_i,

  input  logic                              t_rd_en_i,
  input  logic                              t_wr_en_i,
  input  logic [TagRamAddrSize-1:0]         t_addr_i,
  input  logic [TagRamDataSize-1:0]         t_wr_data_i,
  input  logic [DvTransIdSize-1:0]          t_rd_trans_id_i,
  input  logic [ReqqIdSize-1:0]             t_rd_reqq_id_i,

  input  logic                              d_rd_en_i,
  input  logic                              d_wr_en_i,
  input  logic                              d_amo_wr_en_i,
  input  logic [DataRamAddrSize-1:0]        d_addr_i,
  input  logic [DataRamDataSize-1:0]        d_wr_data_i,
  input  logic [LineQwSize-1:0]             d_wr_qwen_i,
  input  logic [DvTransIdSize-1:0]          d_rd_trans_id_i,
  input  logic [ReqqIdSize-1:0]             d_rd_reqq_id_i,

  input  logic                              dft_sram_clk_i,
  input  logic                              dft_scanmode_i,
  input  logic                              dft_scan_reset_n_i,
  input  logic                              dft_sram_clk_override_i,
  input  logic                              dft_ram_rei_i,
  input  logic                              dft_ram_wei_i,

  input  logic                              ram_cfg_test_en_i,
  input  logic [3:0]                        ram_cfg_rm_i,
  input  logic                              ram_cfg_rme_i,
  input  logic [1:0]                        ram_cfg_ra_i,
  input  logic [1:0]                        ram_cfg_wa_i,
  input  logic [2:0]                        ram_cfg_wpulse_i,
  input  logic                              ram_cfg_deep_sleep_i,
  input  logic                              ram_cfg_shut_down_i,

  output logic                              orig_ts_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          orig_ts_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             orig_ts_rsp_reqq_id_o,
  output logic [TagStateRamDataSize-1:0]    orig_ts_rsp_data_o,
  output logic                              new_ts_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          new_ts_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             new_ts_rsp_reqq_id_o,
  output logic [TagStateRamDataSize-1:0]    new_ts_rsp_data_o,

  output logic                              orig_t_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          orig_t_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             orig_t_rsp_reqq_id_o,
  output logic [TagRamDataSize-1:0]         orig_t_rsp_data_o,
  output logic                              new_t_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          new_t_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             new_t_rsp_reqq_id_o,
  output logic [TagRamDataSize-1:0]         new_t_rsp_data_o,

  output logic                              orig_d_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          orig_d_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             orig_d_rsp_reqq_id_o,
  output logic [DataRamDataSize-1:0]        orig_d_rsp_data_o,
  output logic                              new_d_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          new_d_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             new_d_rsp_reqq_id_o,
  output logic [DataRamDataSize-1:0]        new_d_rsp_data_o
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

    orig_ram_cfg.cfg1.sc_mbs_test1 = ram_cfg_test_en_i;
    orig_ram_cfg.cfg1.sc_mbs_rm = ram_cfg_rm_i;
    orig_ram_cfg.cfg1.sc_mbs_rme = ram_cfg_rme_i;
    orig_ram_cfg.cfg1.sc_mbs_ra = ram_cfg_ra_i;
    orig_ram_cfg.cfg1.sc_mbs_wa = {1'b0, ram_cfg_wa_i};
    orig_ram_cfg.cfg1.sc_mbs_wpulse = ram_cfg_wpulse_i;

    orig_ram_cfg.cfg1.sc_mbt_test1 = ram_cfg_test_en_i;
    orig_ram_cfg.cfg1.sc_mbt_rm = ram_cfg_rm_i;
    orig_ram_cfg.cfg1.sc_mbt_rme = ram_cfg_rme_i;
    orig_ram_cfg.cfg1.sc_mbt_ra = ram_cfg_ra_i;
    orig_ram_cfg.cfg1.sc_mbt_wa = {1'b0, ram_cfg_wa_i};
    orig_ram_cfg.cfg1.sc_mbt_wpulse = ram_cfg_wpulse_i;

    orig_ram_cfg.cfg2.sc_mbd_test1 = ram_cfg_test_en_i;
    orig_ram_cfg.cfg2.sc_mbd_rm = ram_cfg_rm_i;
    orig_ram_cfg.cfg2.sc_mbd_rme = ram_cfg_rme_i;
    orig_ram_cfg.cfg2.sc_mbd_ra = ram_cfg_ra_i;
    orig_ram_cfg.cfg2.sc_mbd_wa = {1'b0, ram_cfg_wa_i};
    orig_ram_cfg.cfg2.sc_mbd_wpulse = ram_cfg_wpulse_i;
  end

  sc_pipe_tag_state_ram_req_t orig_ts_req;
  always_comb begin
    orig_ts_req = '0;
    orig_ts_req.rd_trans_id = ts_rd_trans_id_i;
    orig_ts_req.rd_reqq_id = ts_rd_reqq_id_i;
    orig_ts_req.rd_addr = ts_rd_addr_i;
    // The translated wrapper exposes one transaction-id input per RAM.  Drive
    // the original write IDs from the same source so simultaneous read/write
    // cycles (dual-port tag-state RAM) have a single comparable ID contract.
    orig_ts_req.wr_trans_id = ts_rd_trans_id_i;
    orig_ts_req.wr_reqq_id = ts_rd_reqq_id_i;
    orig_ts_req.wr_addr = ts_wr_addr_i;
    orig_ts_req.wr_data = ts_wr_data_i;
  end

  sc_pipe_tag_ram_req_t orig_t_req;
  always_comb begin
    orig_t_req = '0;
    orig_t_req.rd_trans_id = t_rd_trans_id_i;
    orig_t_req.rd_reqq_id = t_rd_reqq_id_i;
    orig_t_req.rd_addr = t_addr_i;
    orig_t_req.wr_trans_id = t_rd_trans_id_i;
    orig_t_req.wr_reqq_id = t_rd_reqq_id_i;
    orig_t_req.wr_addr = t_addr_i;
    orig_t_req.wr_data = t_wr_data_i;
  end

  sc_pipe_data_ram_req_t orig_d_req;
  always_comb begin
    orig_d_req = '0;
    orig_d_req.rd_trans_id = d_rd_trans_id_i;
    orig_d_req.rd_reqq_id = d_rd_reqq_id_i;
    orig_d_req.rd_addr = d_addr_i;
    orig_d_req.wr_trans_id = d_rd_trans_id_i;
    orig_d_req.wr_reqq_id = d_rd_reqq_id_i;
    orig_d_req.wr_addr = d_addr_i;
    orig_d_req.wr_data = d_wr_data_i;
    orig_d_req.wr_qwen = d_wr_qwen_i;
    orig_d_req.wr_xcheck_disable = 1'b0;
    orig_d_req.amo_wr_trans_id = d_rd_trans_id_i;
    orig_d_req.amo_wr_reqq_id = d_rd_reqq_id_i;
    orig_d_req.amo_wr_addr = d_addr_i;
    orig_d_req.amo_wr_data = d_wr_data_i;
    orig_d_req.amo_wr_qwen = d_wr_qwen_i;
  end

  sc_pipe_tag_state_ram_rsp_t orig_ts_rsp;
  sc_pipe_tag_ram_rsp_t       orig_t_rsp;
  sc_pipe_data_ram_rsp_t      orig_d_rsp;

  logic d_any_wr_en;
  assign d_any_wr_en = d_wr_en_i | d_amo_wr_en_i;

  assign orig_ts_rsp_trans_id_o = orig_ts_rsp.trans_id;
  assign orig_ts_rsp_reqq_id_o  = orig_ts_rsp.reqq_id;
  assign orig_ts_rsp_data_o     = orig_ts_rsp.rd_data;
  assign orig_t_rsp_trans_id_o  = orig_t_rsp.trans_id;
  assign orig_t_rsp_reqq_id_o   = orig_t_rsp.reqq_id;
  assign orig_t_rsp_data_o      = orig_t_rsp.rd_data;
  assign orig_d_rsp_trans_id_o  = orig_d_rsp.trans_id;
  assign orig_d_rsp_reqq_id_o   = orig_d_rsp.reqq_id;
  assign orig_d_rsp_data_o      = orig_d_rsp.rd_data;

  shire_cache_pipe_sub_bank_mem orig_sub_bank_mem (
    .clock                       (clk_i),
    .reset                       (reset),
    .dft__sram_clock             (dft_sram_clk_i),
    .dft__clk_override           (dft_sram_clk_override_i),
    .pipe_tag_state_ram_req_rd_en(ts_rd_en_i),
    .pipe_tag_state_ram_req_wr_en(ts_wr_en_i),
    .pipe_tag_state_ram_req_info (orig_ts_req),
    .pipe_tag_ram_req_rd_en      (t_rd_en_i),
    .pipe_tag_ram_req_wr_en      (t_wr_en_i),
    .pipe_tag_ram_req_info       (orig_t_req),
    .pipe_data_ram_req_rd_en     (d_rd_en_i),
    .pipe_data_ram_req_wr_en     (d_wr_en_i),
    .pipe_data_ram_req_amo_wr_en (d_amo_wr_en_i),
    .pipe_data_ram_req_info      (orig_d_req),
    .pipe_tag_state_ram_rsp_valid(orig_ts_rsp_valid_o),
    .pipe_tag_state_ram_rsp_info (orig_ts_rsp),
    .pipe_tag_ram_rsp_valid      (orig_t_rsp_valid_o),
    .pipe_tag_ram_rsp_info       (orig_t_rsp),
    .pipe_data_ram_rsp_valid     (orig_d_rsp_valid_o),
    .pipe_data_ram_rsp_info      (orig_d_rsp),
    .esr_sc_ram_delay            (esr_ram_delay_i),
    .esr_sc_ram_deep_sleep       (ram_cfg_deep_sleep_i),
    .esr_sc_ram_shut_down        (ram_cfg_shut_down_i),
    .esr_shire_cache_ram_cfg     (orig_ram_cfg)
  );

  shirecache_pipe_sub_bank_mem #(
    .SetsPerSubBank   (SetsPerSubBank),
    .TagStateDualPort (1'b1)
  ) new_sub_bank_mem (
    .clk_i,
    .rst_ni,
    .esr_ram_delay_i,
    .ts_rd_en_i,
    .ts_wr_en_i,
    .ts_rd_addr_i,
    .ts_wr_addr_i,
    .ts_wr_data_i,
    .ts_rd_trans_id_i,
    .ts_rd_reqq_id_i,
    .ts_rsp_valid_o    (new_ts_rsp_valid_o),
    .ts_rsp_trans_id_o (new_ts_rsp_trans_id_o),
    .ts_rsp_reqq_id_o  (new_ts_rsp_reqq_id_o),
    .ts_rsp_data_o     (new_ts_rsp_data_o),
    .t_rd_en_i,
    .t_wr_en_i,
    .t_addr_i,
    .t_wr_data_i,
    .t_rd_trans_id_i,
    .t_rd_reqq_id_i,
    .t_rsp_valid_o     (new_t_rsp_valid_o),
    .t_rsp_trans_id_o  (new_t_rsp_trans_id_o),
    .t_rsp_reqq_id_o   (new_t_rsp_reqq_id_o),
    .t_rsp_data_o      (new_t_rsp_data_o),
    .d_rd_en_i,
    .d_wr_en_i         (d_any_wr_en),
    .d_addr_i,
    .d_wr_data_i,
    .d_wr_qwen_i,
    .d_rd_trans_id_i,
    .d_rd_reqq_id_i,
    .d_rsp_valid_o     (new_d_rsp_valid_o),
    .d_rsp_trans_id_o  (new_d_rsp_trans_id_o),
    .d_rsp_reqq_id_o   (new_d_rsp_reqq_id_o),
    .d_rsp_data_o      (new_d_rsp_data_o),
    .dft_sram_clk_i,
    .ram_cfg_i         (new_ram_cfg),
    .dft_i             (new_dft)
  );

endmodule
