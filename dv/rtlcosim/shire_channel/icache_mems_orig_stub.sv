// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

// Cosim-only original-compatible ICache-memory stub. The shire_channel cosim
// runs the original channel with ICACHE_MEMS_IMPLEMENTED=0, but Verilator still
// resolves module port names in disabled generate branches.
module icache_mems_orig (
  input  logic                                               clock,
  input  logic [`NUM_NEIGH-1:0]                              reset,
  input  logic                                               dft__sram_clock,
  input  logic                                               dft__clk_override,
  input  logic [`NUM_NEIGH-1:0]                              icache_req_write,
  input  logic [`NUM_NEIGH-1:0][`ICACHE_SRAM_ADD_WIDTH-1:0]  icache_req_addr,
  input  logic [`NUM_NEIGH-1:0]                              icache_req_valid,
  output logic [`NUM_NEIGH-1:0]                              icache_req_ready,
  output logic [`NUM_NEIGH-1:0][`ICACHE_SRAM_DATA_WIDTH-1:0] icache_resp_dout,
  output logic [`NUM_NEIGH-1:0]                              icache_resp_valid,
  input  logic [`NUM_NEIGH-1:0]                              icache_resp_ready,
  input  et_link_rsp_info_t [`NUM_NEIGH-1:0]                 neigh_sc_rsp_info,
  input  logic [`NUM_NEIGH-1:0]                              neigh_sc_rsp_valid,
  input  logic [`NUM_NEIGH-1:0]                              neigh_sc_rsp_ready,
  input  esr_shire_cache_ram_cfg_t                           esr_shire_cache_ram_cfg,
  input  sc_icache_bist_req_t                                bist_req_info,
  output sc_icache_bist_rsp_t                                bist_rsp_info,
  input  logic [`ICACHE_MEMS_APB_ADDR_WIDTH-1:0]             apb_paddr,
  input  logic                                               apb_pwrite,
  input  logic                                               apb_psel,
  input  logic                                               apb_penable,
  input  logic [`bpam_shire_apb_data_width-1:0]              apb_pwdata,
  output logic                                               apb_pready,
  output logic [`bpam_shire_apb_data_width-1:0]              apb_prdata,
  output logic                                               apb_pslverr
);
  always_comb begin
    icache_req_ready = '0;
    icache_resp_dout = '0;
    icache_resp_valid = '0;
    bist_rsp_info = '0;
    apb_pready = 1'b1;
    apb_prdata = '0;
    apb_pslverr = 1'b1;
  end

  wire unused_inputs = &{
    1'b0,
    clock,
    reset,
    dft__sram_clock,
    dft__clk_override,
    icache_req_write,
    icache_req_addr,
    icache_req_valid,
    icache_resp_ready,
    neigh_sc_rsp_info,
    neigh_sc_rsp_valid,
    neigh_sc_rsp_ready,
    esr_shire_cache_ram_cfg,
    bist_req_info,
    apb_paddr,
    apb_pwrite,
    apb_psel,
    apb_penable,
    apb_pwdata
  };
endmodule
