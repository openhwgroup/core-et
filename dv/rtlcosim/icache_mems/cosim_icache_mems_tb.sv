// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_icache_mems_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [3:0] rst_vec_ni_i,
  input  logic dft_sram_clk_i,
  input  logic dft_clk_override_i,
  input  logic [3:0] icache_req_write_i,
  input  logic [3:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic [3:0] icache_req_valid_i,
  input  logic [3:0] icache_resp_ready_i,
  input  logic [(4*$bits(etlink_pkg::rsp_t))-1:0] neigh_rsp_info_bits_i,
  input  logic [3:0] neigh_rsp_valid_i,
  input  logic [3:0] neigh_rsp_ready_i,
  input  logic [$bits(shirecache_pkg::icache_bist_req_t)-1:0] bist_req_bits_i,
  input  logic [icache_pkg::IcacheMemsApbAddrWidth-1:0] apb_paddr_i,
  input  logic apb_pwrite_i,
  input  logic apb_psel_i,
  input  logic apb_penable_i,
  input  logic [minion_pkg::BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic [3:0] orig_req_ready_o,
  output logic [3:0] new_req_ready_o,
  output logic [(4*icache_geom_pkg::IcacheSramDataWidth)-1:0] orig_resp_dout_o,
  output logic [(4*icache_geom_pkg::IcacheSramDataWidth)-1:0] new_resp_dout_o,
  output logic [3:0] orig_resp_valid_o,
  output logic [3:0] new_resp_valid_o,
  output logic [$bits(shirecache_pkg::icache_bist_rsp_t)-1:0] orig_bist_rsp_o,
  output logic [$bits(shirecache_pkg::icache_bist_rsp_t)-1:0] new_bist_rsp_o,
  output logic orig_apb_pready_o,
  output logic new_apb_pready_o,
  output logic [minion_pkg::BpamShireApbDataWidth-1:0] orig_apb_prdata_o,
  output logic [minion_pkg::BpamShireApbDataWidth-1:0] new_apb_prdata_o,
  output logic orig_apb_pslverr_o,
  output logic new_apb_pslverr_o
);
  localparam int unsigned NumNeigh = 4;
  localparam int unsigned RspWidth = $bits(etlink_pkg::rsp_t);
  localparam int unsigned DataWidth = icache_geom_pkg::IcacheSramDataWidth;

  logic [NumNeigh-1:0] orig_reset;
  et_link_rsp_info_t [NumNeigh-1:0] orig_neigh_rsp_info;
  etlink_pkg::rsp_t new_neigh_rsp_info [NumNeigh-1:0];
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  ram_cfg_pkg::ram_cfg_t new_ram_cfg;
  sc_icache_bist_req_t orig_bist_req;
  shirecache_pkg::icache_bist_req_t new_bist_req;
  sc_icache_bist_rsp_t orig_bist_rsp;
  shirecache_pkg::icache_bist_rsp_t new_bist_rsp;
  logic [NumNeigh-1:0][DataWidth-1:0] orig_resp_dout;
  logic [NumNeigh-1:0][DataWidth-1:0] new_resp_dout;
  dft_pkg::dft_t new_dft;

  assign orig_reset = ~({NumNeigh{rst_ni}} & rst_vec_ni_i);
  assign orig_ram_cfg = '0;
  assign new_ram_cfg = '0;
  assign orig_bist_req = bist_req_bits_i;
  assign new_bist_req = bist_req_bits_i;
  assign orig_bist_rsp_o = orig_bist_rsp;
  assign new_bist_rsp_o = new_bist_rsp;
  assign new_dft = '{sram_clk_override: dft_clk_override_i, default: '0};

  always_comb begin
    for (int unsigned n = 0; n < NumNeigh; n++) begin
      orig_neigh_rsp_info[n] = neigh_rsp_info_bits_i[(n*RspWidth) +: RspWidth];
      new_neigh_rsp_info[n] = neigh_rsp_info_bits_i[(n*RspWidth) +: RspWidth];
      orig_resp_dout_o[(n*DataWidth) +: DataWidth] = orig_resp_dout[n];
      new_resp_dout_o[(n*DataWidth) +: DataWidth] = new_resp_dout[n];
    end
  end

  icache_mems_orig u_orig (
    .clock                    (clk_i),
    .reset                    (orig_reset),
    .dft__sram_clock          (dft_sram_clk_i),
    .dft__clk_override        (dft_clk_override_i),
    .icache_req_write         (icache_req_write_i),
    .icache_req_addr          (icache_req_addr_i),
    .icache_req_valid         (icache_req_valid_i),
    .icache_req_ready         (orig_req_ready_o),
    .icache_resp_dout         (orig_resp_dout),
    .icache_resp_valid        (orig_resp_valid_o),
    .icache_resp_ready        (icache_resp_ready_i),
    .neigh_sc_rsp_info        (orig_neigh_rsp_info),
    .neigh_sc_rsp_valid       (neigh_rsp_valid_i),
    .neigh_sc_rsp_ready       (neigh_rsp_ready_i),
    .esr_shire_cache_ram_cfg  (orig_ram_cfg),
    .bist_req_info            (orig_bist_req),
    .bist_rsp_info            (orig_bist_rsp),
    .apb_paddr                (apb_paddr_i),
    .apb_pwrite               (apb_pwrite_i),
    .apb_psel                 (apb_psel_i),
    .apb_penable              (apb_penable_i),
    .apb_pwdata               (apb_pwdata_i),
    .apb_pready               (orig_apb_pready_o),
    .apb_prdata               (orig_apb_prdata_o),
    .apb_pslverr              (orig_apb_pslverr_o)
  );

  icache_mems u_new (
    .clk_i                    (clk_i),
    .rst_ni                   ({NumNeigh{rst_ni}} & rst_vec_ni_i),
    .dft_sram_clk_i           (dft_sram_clk_i),
    .dft_i                    (new_dft),
    .icache_req_write_i       (icache_req_write_i),
    .icache_req_addr_i        (icache_req_addr_i),
    .icache_req_valid_i       (icache_req_valid_i),
    .icache_req_ready_o       (new_req_ready_o),
    .icache_resp_dout_o       (new_resp_dout),
    .icache_resp_valid_o      (new_resp_valid_o),
    .icache_resp_ready_i      (icache_resp_ready_i),
    .neigh_sc_rsp_info_i      (new_neigh_rsp_info),
    .neigh_sc_rsp_valid_i     (neigh_rsp_valid_i),
    .neigh_sc_rsp_ready_i     (neigh_rsp_ready_i),
    .ram_cfg_i                (new_ram_cfg),
    .bist_req_info_i          (new_bist_req),
    .bist_rsp_info_o          (new_bist_rsp),
    .apb_paddr_i              (apb_paddr_i),
    .apb_pwrite_i             (apb_pwrite_i),
    .apb_psel_i               (apb_psel_i),
    .apb_penable_i            (apb_penable_i),
    .apb_pwdata_i             (apb_pwdata_i),
    .apb_pready_o             (new_apb_pready_o),
    .apb_prdata_o             (new_apb_prdata_o),
    .apb_pslverr_o            (new_apb_pslverr_o)
  );
endmodule
