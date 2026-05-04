// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_data_ram_wrap_tb
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
  output logic                        icache_req_ready_o,
  output logic                        icache_resp_valid_o,
  input  logic                        icache_resp_ready_i,
  output logic [63:0]                 icache_resp_qw0_o,
  output logic [63:0]                 icache_resp_qw1_o,
  output logic [63:0]                 icache_resp_qw2_o,
  output logic [63:0]                 icache_resp_qw3_o,
  output logic [63:0]                 icache_resp_qw4_o,
  output logic [63:0]                 icache_resp_qw5_o,
  output logic [63:0]                 icache_resp_qw6_o,
  output logic [63:0]                 icache_resp_qw7_o,
  output logic [63:0]                 icache_resp_qw8_o,
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
  /* verilator lint_off UNUSEDSIGNAL */  // Wrapper exposes a uniform 64-bit chunk; only the low 16 bits feed the 144-bit MBIST word.
  input  logic [63:0]                 bist_req_mbi_wdata_2_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic [63:0]                 bist_rsp_mbi_rdata_0_o,
  output logic [63:0]                 bist_rsp_mbi_rdata_1_o,
  output logic [63:0]                 bist_rsp_mbi_rdata_2_o,
  input  logic [IcacheDataApbAddrWidth-1:0] apb_paddr_i,
  input  logic                        apb_pwrite_i,
  input  logic                        apb_psel_i,
  input  logic                        apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic                        apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] apb_prdata_o,
  output logic                        apb_pslverr_o
);

  dft_t              dft;
  ram_cfg_t          ram_cfg;
  rsp_t              neigh_sc_rsp_info;
  icache_bist_req_t  bist_req_info;
  icache_bist_rsp_t  bist_rsp_info;
  logic [IcacheSramDataWidth-1:0] icache_resp_dout;

  always_comb begin
    dft = '0;
    dft.scanmode          = dft_scanmode_i;
    dft.scan_reset_n      = dft_scan_reset_n_i;
    dft.sram_clk_override = dft_sram_clk_override_i;
    dft.ram_rei           = dft_ram_rei_i;
    dft.ram_wei           = dft_ram_wei_i;

    ram_cfg = '0;
    ram_cfg.test_en    = ram_cfg_test_en_i;
    ram_cfg.rm         = ram_cfg_rm_i;
    ram_cfg.rme        = ram_cfg_rme_i;
    ram_cfg.ra         = ram_cfg_ra_i;
    ram_cfg.wa         = ram_cfg_wa_i;
    ram_cfg.wpulse     = ram_cfg_wpulse_i;
    ram_cfg.deep_sleep = ram_cfg_deep_sleep_i;
    ram_cfg.shut_down  = ram_cfg_shut_down_i;

    neigh_sc_rsp_info = '0;
    neigh_sc_rsp_info.id     = neigh_sc_rsp_id_i;
    neigh_sc_rsp_info.dest   = neigh_sc_rsp_dest_i;
    neigh_sc_rsp_info.wdata  = neigh_sc_rsp_wdata_i;
    neigh_sc_rsp_info.opcode = rsp_opcode_e'(neigh_sc_rsp_opcode_i);
    neigh_sc_rsp_info.data   = {
      neigh_sc_rsp_data_7_i,
      neigh_sc_rsp_data_6_i,
      neigh_sc_rsp_data_5_i,
      neigh_sc_rsp_data_4_i,
      neigh_sc_rsp_data_3_i,
      neigh_sc_rsp_data_2_i,
      neigh_sc_rsp_data_1_i,
      neigh_sc_rsp_data_0_i
    };
    neigh_sc_rsp_info.size   = size_e'(neigh_sc_rsp_size_i);
    neigh_sc_rsp_info.qwen   = neigh_sc_rsp_qwen_i;

    bist_req_info = '0;
    bist_req_info.mbist_on = bist_req_mbist_on_i;
    bist_req_info.mbi_sel  = bist_req_mbi_sel_i;
    bist_req_info.mbi_rd_en = bist_req_mbi_rd_en_i;
    bist_req_info.mbi_wr_en = bist_req_mbi_wr_en_i;
    bist_req_info.mbi_addr  = bist_req_mbi_addr_i;
    bist_req_info.mbi_wdata = {
      bist_req_mbi_wdata_2_i[15:0],
      bist_req_mbi_wdata_1_i,
      bist_req_mbi_wdata_0_i
    };
  end

  assign icache_resp_qw0_o = icache_resp_dout[0 +: 64];
  assign icache_resp_qw1_o = icache_resp_dout[64 +: 64];
  assign icache_resp_qw2_o = icache_resp_dout[128 +: 64];
  assign icache_resp_qw3_o = icache_resp_dout[192 +: 64];
  assign icache_resp_qw4_o = icache_resp_dout[256 +: 64];
  assign icache_resp_qw5_o = icache_resp_dout[320 +: 64];
  assign icache_resp_qw6_o = icache_resp_dout[384 +: 64];
  assign icache_resp_qw7_o = icache_resp_dout[448 +: 64];
  assign icache_resp_qw8_o = icache_resp_dout[512 +: 64];

  assign bist_rsp_mbi_rdata_0_o = bist_rsp_info.mbi_rdata[63:0];
  assign bist_rsp_mbi_rdata_1_o = bist_rsp_info.mbi_rdata[127:64];
  assign bist_rsp_mbi_rdata_2_o = {{48{1'b0}}, bist_rsp_info.mbi_rdata[143:128]};

  icache_data_ram_wrap u_dut (
    .clk_i,
    .rst_ni,
    .dft_sram_clk_i,
    .dft_i(dft),
    .icache_req_write_i,
    .icache_req_addr_i,
    .icache_req_valid_i,
    .icache_req_ready_o,
    .icache_resp_dout_o(icache_resp_dout),
    .icache_resp_valid_o,
    .icache_resp_ready_i,
    .neigh_sc_rsp_info_i(neigh_sc_rsp_info),
    .neigh_sc_rsp_valid_i,
    .neigh_sc_rsp_ready_i,
    .ram_cfg_i(ram_cfg),
    .bist_req_info_i(bist_req_info),
    .bist_rsp_info_o(bist_rsp_info),
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
