// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_icache_data_ram_wrap_tb
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import etlink_pkg::*;
  import minion_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
  import shirecache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic                        dft_sram_clk_i,
  input  logic                        dft_scanmode_i,
  input  logic                        dft_scan_reset_n_i,
  input  logic                        dft_sram_clk_override_i,
  input  logic                        dft_ram_rei_i,
  input  logic                        dft_ram_wei_i,
  input  logic                        ram_cfg_test_en_i,
  input  logic [3:0]                  ram_cfg_rm_i,
  input  logic                        ram_cfg_rme_i,
  input  logic [1:0]                  ram_cfg_ra_i,
  input  logic [1:0]                  ram_cfg_wa_i,
  input  logic [2:0]                  ram_cfg_wpulse_i,
  input  logic                        ram_cfg_deep_sleep_i,
  input  logic                        ram_cfg_shut_down_i,
  input  logic                        icache_req_write_i,
  input  logic [IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic                        icache_req_valid_i,
  input  logic                        icache_resp_ready_i,
  input  logic [IdSize-1:0]           neigh_sc_rsp_id_i,
  input  logic [SourceSize-1:0]       neigh_sc_rsp_dest_i,
  input  logic                        neigh_sc_rsp_wdata_i,
  input  logic [$bits(rsp_opcode_e)-1:0] neigh_sc_rsp_opcode_i,
  input  logic [63:0]                 neigh_sc_rsp_data_0_i,
  input  logic [63:0]                 neigh_sc_rsp_data_1_i,
  input  logic [63:0]                 neigh_sc_rsp_data_2_i,
  input  logic [63:0]                 neigh_sc_rsp_data_3_i,
  input  logic [63:0]                 neigh_sc_rsp_data_4_i,
  input  logic [63:0]                 neigh_sc_rsp_data_5_i,
  input  logic [63:0]                 neigh_sc_rsp_data_6_i,
  input  logic [63:0]                 neigh_sc_rsp_data_7_i,
  input  logic [$bits(size_e)-1:0]    neigh_sc_rsp_size_i,
  input  logic [QwenSize-1:0]         neigh_sc_rsp_qwen_i,
  input  logic                        neigh_sc_rsp_valid_i,
  input  logic                        neigh_sc_rsp_ready_i,
  input  logic                        bist_req_mbist_on_i,
  input  logic                        bist_req_mbi_sel_i,
  input  logic                        bist_req_mbi_rd_en_i,
  input  logic                        bist_req_mbi_wr_en_i,
  input  logic [MbiAddrSize-1:0]      bist_req_mbi_addr_i,
  input  logic [63:0]                 bist_req_mbi_wdata_0_i,
  input  logic [63:0]                 bist_req_mbi_wdata_1_i,
  input  logic [63:0]                 bist_req_mbi_wdata_2_i,
  input  logic [IcacheDataApbAddrWidth-1:0] apb_paddr_i,
  input  logic                        apb_pwrite_i,
  input  logic                        apb_psel_i,
  input  logic                        apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,

  output logic                        orig_icache_req_ready_o,
  output logic                        orig_icache_resp_valid_o,
  output logic [63:0]                 orig_icache_resp_qw0_o,
  output logic [63:0]                 orig_icache_resp_qw1_o,
  output logic [63:0]                 orig_icache_resp_qw2_o,
  output logic [63:0]                 orig_icache_resp_qw3_o,
  output logic [63:0]                 orig_icache_resp_qw4_o,
  output logic [63:0]                 orig_icache_resp_qw5_o,
  output logic [63:0]                 orig_icache_resp_qw6_o,
  output logic [63:0]                 orig_icache_resp_qw7_o,
  output logic [63:0]                 orig_icache_resp_qw8_o,
  output logic [63:0]                 orig_bist_rsp_mbi_rdata_0_o,
  output logic [63:0]                 orig_bist_rsp_mbi_rdata_1_o,
  output logic [63:0]                 orig_bist_rsp_mbi_rdata_2_o,
  output logic                        orig_apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] orig_apb_prdata_o,
  output logic                        orig_apb_pslverr_o,

  output logic                        new_icache_req_ready_o,
  output logic                        new_icache_resp_valid_o,
  output logic [63:0]                 new_icache_resp_qw0_o,
  output logic [63:0]                 new_icache_resp_qw1_o,
  output logic [63:0]                 new_icache_resp_qw2_o,
  output logic [63:0]                 new_icache_resp_qw3_o,
  output logic [63:0]                 new_icache_resp_qw4_o,
  output logic [63:0]                 new_icache_resp_qw5_o,
  output logic [63:0]                 new_icache_resp_qw6_o,
  output logic [63:0]                 new_icache_resp_qw7_o,
  output logic [63:0]                 new_icache_resp_qw8_o,
  output logic [63:0]                 new_bist_rsp_mbi_rdata_0_o,
  output logic [63:0]                 new_bist_rsp_mbi_rdata_1_o,
  output logic [63:0]                 new_bist_rsp_mbi_rdata_2_o,
  output logic                        new_apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] new_apb_prdata_o,
  output logic                        new_apb_pslverr_o
);

  logic                      reset;
  dft_t                      new_dft;
  ram_cfg_t                  new_ram_cfg;
  rsp_t                      new_neigh_sc_rsp_info;
  icache_bist_req_t          new_bist_req_info;
  icache_bist_rsp_t          new_bist_rsp_info;
  logic [IcacheSramDataWidth-1:0] new_icache_resp_dout;

  esr_shire_cache_ram_cfg_t  orig_ram_cfg;
  et_link_rsp_info_t         orig_neigh_sc_rsp_info;
  sc_icache_bist_req_t       orig_bist_req_info;
  sc_icache_bist_rsp_t       orig_bist_rsp_info;
  logic [`ICACHE_SRAM_DATA_WIDTH-1:0] orig_icache_resp_dout;

  assign reset = !rst_ni;

  always_comb begin
    new_dft = '0;
    new_dft.scanmode          = dft_scanmode_i;
    new_dft.scan_reset_n      = dft_scan_reset_n_i;
    new_dft.sram_clk_override = dft_sram_clk_override_i;
    new_dft.ram_rei           = dft_ram_rei_i;
    new_dft.ram_wei           = dft_ram_wei_i;

    new_ram_cfg = '0;
    new_ram_cfg.test_en    = ram_cfg_test_en_i;
    new_ram_cfg.rm         = ram_cfg_rm_i;
    new_ram_cfg.rme        = ram_cfg_rme_i;
    new_ram_cfg.ra         = ram_cfg_ra_i;
    new_ram_cfg.wa         = ram_cfg_wa_i;
    new_ram_cfg.wpulse     = ram_cfg_wpulse_i;
    new_ram_cfg.deep_sleep = ram_cfg_deep_sleep_i;
    new_ram_cfg.shut_down  = ram_cfg_shut_down_i;

    new_neigh_sc_rsp_info = '0;
    new_neigh_sc_rsp_info.id     = neigh_sc_rsp_id_i;
    new_neigh_sc_rsp_info.dest   = neigh_sc_rsp_dest_i;
    new_neigh_sc_rsp_info.wdata  = neigh_sc_rsp_wdata_i;
    new_neigh_sc_rsp_info.opcode = rsp_opcode_e'(neigh_sc_rsp_opcode_i);
    new_neigh_sc_rsp_info.data   = {
      neigh_sc_rsp_data_7_i,
      neigh_sc_rsp_data_6_i,
      neigh_sc_rsp_data_5_i,
      neigh_sc_rsp_data_4_i,
      neigh_sc_rsp_data_3_i,
      neigh_sc_rsp_data_2_i,
      neigh_sc_rsp_data_1_i,
      neigh_sc_rsp_data_0_i
    };
    new_neigh_sc_rsp_info.size   = size_e'(neigh_sc_rsp_size_i);
    new_neigh_sc_rsp_info.qwen   = neigh_sc_rsp_qwen_i;

    new_bist_req_info = '0;
    new_bist_req_info.mbist_on = bist_req_mbist_on_i;
    new_bist_req_info.mbi_sel  = bist_req_mbi_sel_i;
    new_bist_req_info.mbi_rd_en = bist_req_mbi_rd_en_i;
    new_bist_req_info.mbi_wr_en = bist_req_mbi_wr_en_i;
    new_bist_req_info.mbi_addr  = bist_req_mbi_addr_i;
    new_bist_req_info.mbi_wdata = {
      bist_req_mbi_wdata_2_i[15:0],
      bist_req_mbi_wdata_1_i,
      bist_req_mbi_wdata_0_i
    };

    orig_ram_cfg = '0;
    orig_ram_cfg.dft__ram_rei     = dft_ram_rei_i;
    orig_ram_cfg.dft__ram_wei     = dft_ram_wei_i;
    orig_ram_cfg.cfg4.sc_mbi_test1 = ram_cfg_test_en_i;
    orig_ram_cfg.cfg4.sc_mbi_rme   = ram_cfg_rme_i;
    orig_ram_cfg.cfg4.sc_mbi_rm    = ram_cfg_rm_i;
    orig_ram_cfg.cfg4.sc_mbi_ra    = ram_cfg_ra_i;
    orig_ram_cfg.cfg4.sc_mbi_wa    = {1'b0, ram_cfg_wa_i};
    orig_ram_cfg.cfg4.sc_mbi_wpulse = ram_cfg_wpulse_i;

    orig_neigh_sc_rsp_info = '0;
    orig_neigh_sc_rsp_info.id     = neigh_sc_rsp_id_i;
    orig_neigh_sc_rsp_info.dest   = neigh_sc_rsp_dest_i;
    orig_neigh_sc_rsp_info.wdata  = neigh_sc_rsp_wdata_i;
    orig_neigh_sc_rsp_info.opcode = et_link_rsp_opcode_t'(neigh_sc_rsp_opcode_i);
    orig_neigh_sc_rsp_info.data   = {
      neigh_sc_rsp_data_7_i,
      neigh_sc_rsp_data_6_i,
      neigh_sc_rsp_data_5_i,
      neigh_sc_rsp_data_4_i,
      neigh_sc_rsp_data_3_i,
      neigh_sc_rsp_data_2_i,
      neigh_sc_rsp_data_1_i,
      neigh_sc_rsp_data_0_i
    };
    orig_neigh_sc_rsp_info.size   = et_link_size_t'(neigh_sc_rsp_size_i);
    orig_neigh_sc_rsp_info.qwen   = neigh_sc_rsp_qwen_i;

    orig_bist_req_info = '0;
    orig_bist_req_info.mbist_on = bist_req_mbist_on_i;
    orig_bist_req_info.mbi_sel  = bist_req_mbi_sel_i;
    orig_bist_req_info.mbi_rd_en = bist_req_mbi_rd_en_i;
    orig_bist_req_info.mbi_wr_en = bist_req_mbi_wr_en_i;
    orig_bist_req_info.mbi_addr  = bist_req_mbi_addr_i;
    orig_bist_req_info.mbi_wdata = {
      bist_req_mbi_wdata_2_i[15:0],
      bist_req_mbi_wdata_1_i,
      bist_req_mbi_wdata_0_i
    };
  end

  assign orig_icache_resp_qw0_o = orig_icache_resp_dout[0 +: 64];
  assign orig_icache_resp_qw1_o = orig_icache_resp_dout[64 +: 64];
  assign orig_icache_resp_qw2_o = orig_icache_resp_dout[128 +: 64];
  assign orig_icache_resp_qw3_o = orig_icache_resp_dout[192 +: 64];
  assign orig_icache_resp_qw4_o = orig_icache_resp_dout[256 +: 64];
  assign orig_icache_resp_qw5_o = orig_icache_resp_dout[320 +: 64];
  assign orig_icache_resp_qw6_o = orig_icache_resp_dout[384 +: 64];
  assign orig_icache_resp_qw7_o = orig_icache_resp_dout[448 +: 64];
  assign orig_icache_resp_qw8_o = orig_icache_resp_dout[512 +: 64];
  assign orig_bist_rsp_mbi_rdata_0_o = orig_bist_rsp_info.mbi_rdata[63:0];
  assign orig_bist_rsp_mbi_rdata_1_o = orig_bist_rsp_info.mbi_rdata[127:64];
  assign orig_bist_rsp_mbi_rdata_2_o = {{48{1'b0}}, orig_bist_rsp_info.mbi_rdata[143:128]};

  assign new_icache_resp_qw0_o = new_icache_resp_dout[0 +: 64];
  assign new_icache_resp_qw1_o = new_icache_resp_dout[64 +: 64];
  assign new_icache_resp_qw2_o = new_icache_resp_dout[128 +: 64];
  assign new_icache_resp_qw3_o = new_icache_resp_dout[192 +: 64];
  assign new_icache_resp_qw4_o = new_icache_resp_dout[256 +: 64];
  assign new_icache_resp_qw5_o = new_icache_resp_dout[320 +: 64];
  assign new_icache_resp_qw6_o = new_icache_resp_dout[384 +: 64];
  assign new_icache_resp_qw7_o = new_icache_resp_dout[448 +: 64];
  assign new_icache_resp_qw8_o = new_icache_resp_dout[512 +: 64];
  assign new_bist_rsp_mbi_rdata_0_o = new_bist_rsp_info.mbi_rdata[63:0];
  assign new_bist_rsp_mbi_rdata_1_o = new_bist_rsp_info.mbi_rdata[127:64];
  assign new_bist_rsp_mbi_rdata_2_o = {{48{1'b0}}, new_bist_rsp_info.mbi_rdata[143:128]};

  icache_data_ram_wrap_orig u_orig (
    .clock(clk_i),
    .reset(reset),
    .dft__sram_clock(dft_sram_clk_i),
    .dft__clk_override(dft_sram_clk_override_i),
    .icache_req_write(icache_req_write_i),
    .icache_req_addr(icache_req_addr_i),
    .icache_req_valid(icache_req_valid_i),
    .icache_req_ready(orig_icache_req_ready_o),
    .icache_resp_dout(orig_icache_resp_dout),
    .icache_resp_valid(orig_icache_resp_valid_o),
    .icache_resp_ready(icache_resp_ready_i),
    .neigh_sc_rsp_info(orig_neigh_sc_rsp_info),
    .neigh_sc_rsp_valid(neigh_sc_rsp_valid_i),
    .neigh_sc_rsp_ready(neigh_sc_rsp_ready_i),
    .esr_shire_cache_ram_cfg(orig_ram_cfg),
    .bist_req_info(orig_bist_req_info),
    .bist_rsp_info(orig_bist_rsp_info),
    .apb_paddr(apb_paddr_i),
    .apb_pwrite(apb_pwrite_i),
    .apb_psel(apb_psel_i),
    .apb_penable(apb_penable_i),
    .apb_pwdata(apb_pwdata_i),
    .apb_pready(orig_apb_pready_o),
    .apb_prdata(orig_apb_prdata_o),
    .apb_pslverr(orig_apb_pslverr_o)
  );

  icache_data_ram_wrap u_new (
    .clk_i,
    .rst_ni,
    .dft_sram_clk_i,
    .dft_i(new_dft),
    .icache_req_write_i,
    .icache_req_addr_i,
    .icache_req_valid_i,
    .icache_req_ready_o(new_icache_req_ready_o),
    .icache_resp_dout_o(new_icache_resp_dout),
    .icache_resp_valid_o(new_icache_resp_valid_o),
    .icache_resp_ready_i,
    .neigh_sc_rsp_info_i(new_neigh_sc_rsp_info),
    .neigh_sc_rsp_valid_i,
    .neigh_sc_rsp_ready_i,
    .ram_cfg_i(new_ram_cfg),
    .bist_req_info_i(new_bist_req_info),
    .bist_rsp_info_o(new_bist_rsp_info),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o(new_apb_pready_o),
    .apb_prdata_o(new_apb_prdata_o),
    .apb_pslverr_o(new_apb_pslverr_o)
  );

endmodule
