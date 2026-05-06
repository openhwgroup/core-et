// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for standalone_minion.

/* verilator lint_off SYNCASYNCNET */  // The wrapper forwards independently asserted active-low reset domains to the DUT.
module standalone_minion_tb
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import etlink_pkg::*;
  import esr_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import shirecache_pkg::*;
(
  input  logic                                  clk_i,
  input  logic                                  rst_ni,
  input  logic                                  rst_debug_ni,
  input  logic                                  rst_non_debug_ni,
  input  logic                                  dft_scanmode_i,
  input  logic                                  dft_scan_reset_n_i,
  input  logic                                  dft_sram_clk_override_i,
  input  logic                                  dft_ram_rei_i,
  input  logic                                  dft_ram_wei_i,
  input  logic                                  dft_sram_clk_i,
  input  logic [EtEcoMinionInputWidth-1:0]      eco_i,
  input  logic                                  ioshire_i,
  input  logic                                  int_mieco_i,
  input  logic                                  int_mtip_i,
  input  logic                                  int_meip_i,
  input  logic                                  int_seip_i,
  input  logic [NrThreads-1:0]                  int_msip_i,
  input  logic [NumShireIdsBits-1:0]            shire_id_i,
  input  logic [MinPerSBits-1:0]                shire_min_id_i,
  input  logic [NrThreads-1:0]                  thread_enable_i,
  input  logic [VaSize-1:0]                     minion_boot_addr_i,
  input  logic [$bits(esr_mprot_t)-1:0]         mprot_i,
  input  logic [$bits(tlb_entry_type)-1:0]      vmspagesize_i,
  input  logic [$bits(esr_minion_features_t)-1:0] minion_features_i,
  input  logic                                  shire_coop_mode_i,
  input  logic [ShireMinionMemOverrideBits-1:0] minion_mem_override_i,
  input  logic [$bits(minion_debug_in_t)-1:0]   debug_in_i,
  input  logic                                  spio_rc_resumeack_i,
  input  logic                                  minion_dbg_sig_enable_i,
  input  logic [NeighDebugSmMuxWidth-1:0]       minion_dbg_signals_mux_i,
  input  logic                                  esr_bypass_icache_i,
  input  logic                                  esr_bypass_dcache_i,
  input  logic [$bits(ram_cfg_t)-1:0]           ram_cfg_i,
  input  logic [$bits(esr_icache_err_log_ctl_t)-1:0] esr_icache_err_log_ctl_i,
  input  logic [$bits(icache_bist_req_t)-1:0]   icache_bist_req_info_i,
  input  logic                                  etl_req_ready_i,
  input  logic                                  etl_rsp_valid_i,
  input  logic [$bits(rsp_t)-1:0]               etl_rsp_info_i,
  input  logic                                  te_enable_i,
  input  logic [15:0]                           apb_paddr_i,
  input  logic                                  apb_penable_i,
  input  logic                                  apb_psel_i,
  input  logic [BpamShireApbDataWidth-1:0]      apb_pwdata_i,
  input  logic                                  apb_pwrite_i,

  output logic [11:0]                           dft_scanout_o,
  output logic                                  dft_occ_scanout_o,
  output logic [EtEcoMinionOutputWidth-1:0]     eco_o,
  output logic                                  dbg_filter_valid_o,
  output logic                                  dbg_match_valid_o,
  output logic                                  dbg_data_valid_o,
  output logic [NeighDebugDataWidth-1:0]        dbg_data_o,
  output logic                                  esr_icache_err_log_sbe_o,
  output logic                                  esr_icache_err_log_dbe_o,
  output logic [$bits(icache_err_log_info_t)-1:0] esr_icache_err_log_info_o,
  output logic [$bits(icache_bist_rsp_t)-1:0]   icache_bist_rsp_info_o,
  output logic                                  etl_req_valid_o,
  output logic [$bits(req_t)-1:0]               etl_req_info_o,
  output logic                                  etl_rsp_ready_o,
  output logic                                  icache_req_ready_dbg_o,
  output logic                                  icache_req_valid_dbg_o,
  output logic                                  icache_miss_req_ready_dbg_o,
  output logic                                  icache_miss_req_valid_dbg_o,
  output logic                                  miss_req_pop_valid_dbg_o,
  output logic                                  trace_min_reset_o,
  output logic                                  trace_instr_valid_o,
  output logic [$bits(esr_spdmctrl_t)-1:0]      esr_spdmctrl_o,
  output logic [$bits(esr_sphastatus_t)-1:0]    esr_sphastatus_o,
  output logic [BpamShireApbDataWidth-1:0]      apb_prdata_o,
  output logic                                  apb_pready_o,
  output logic                                  apb_pslverr_o
);

  dft_t dft;
  esr_mprot_t mprot;
  tlb_entry_type vmspagesize;
  esr_minion_features_t minion_features;
  minion_debug_in_t debug_in;
  ram_cfg_t ram_cfg;
  esr_icache_err_log_ctl_t err_log_ctl;
  icache_bist_req_t bist_req;
  rsp_t etl_rsp_info;
  neigh_sm_dbg_monitor_t minion_dbg_signals;
  icache_err_log_info_t err_log_info;
  icache_bist_rsp_t bist_rsp;
  req_t etl_req_info;
  trace_encoder_signals_t trace_encoder;
  logic tb_unused_ok;
  esr_spdmctrl_t esr_spdmctrl;
  esr_sphastatus_t esr_sphastatus;

  always_comb begin
    dft = '{
      scanmode: dft_scanmode_i,
      scan_reset_n: dft_scan_reset_n_i,
      sram_clk_override: dft_sram_clk_override_i,
      ram_rei: dft_ram_rei_i,
      ram_wei: dft_ram_wei_i
    };
    mprot = esr_mprot_t'(mprot_i);
    vmspagesize = tlb_entry_type'(vmspagesize_i);
    minion_features = esr_minion_features_t'(minion_features_i);
    debug_in = minion_debug_in_t'(debug_in_i);
    ram_cfg = ram_cfg_t'(ram_cfg_i);
    err_log_ctl = esr_icache_err_log_ctl_t'(esr_icache_err_log_ctl_i);
    bist_req = icache_bist_req_t'(icache_bist_req_info_i);
    etl_rsp_info = rsp_t'(etl_rsp_info_i);
  end

  assign dbg_filter_valid_o = minion_dbg_signals.filter_valid;
  assign dbg_match_valid_o = minion_dbg_signals.match_valid;
  assign dbg_data_valid_o = minion_dbg_signals.data_valid;
  assign dbg_data_o = minion_dbg_signals.data_signals;
  assign esr_icache_err_log_info_o = err_log_info;
  assign icache_bist_rsp_info_o = bist_rsp;
  assign etl_req_info_o = etl_req_info;
  assign icache_req_ready_dbg_o = u_dut.icache_req_ready;
  assign icache_req_valid_dbg_o = u_dut.icache_req_valid;
  assign icache_miss_req_ready_dbg_o = u_dut.icache_miss_req_ready;
  assign icache_miss_req_valid_dbg_o = u_dut.icache_miss_req_valid;
  assign miss_req_pop_valid_dbg_o = u_dut.miss_req_pop_valid;
  assign trace_min_reset_o = trace_encoder.min_reset;
  assign tb_unused_ok = ^{minion_dbg_signals[394:195], minion_dbg_signals[193:130], trace_encoder[142:1]};
  assign trace_instr_valid_o = trace_encoder.instr_valid[0] ^ (tb_unused_ok & 1'b0);
  assign esr_spdmctrl_o = esr_spdmctrl;
  assign esr_sphastatus_o = esr_sphastatus;

  standalone_minion u_dut (
    .clk_i,
    .rst_ni,
    .rst_debug_ni,
    .rst_non_debug_ni,
    .dft_i                  (dft),
    .dft_sram_clk_i,
    .dft_scanout_o,
    .dft_occ_scanout_o,
    .eco_i,
    .eco_o,
    .ioshire_i,
    .int_mieco_i,
    .int_mtip_i,
    .int_meip_i,
    .int_seip_i,
    .int_msip_i,
    .shire_id_i,
    .shire_min_id_i,
    .thread_enable_i,
    .minion_boot_addr_i,
    .mprot_i                (mprot),
    .vmspagesize_i          (vmspagesize),
    .minion_features_i      (minion_features),
    .shire_coop_mode_i,
    .minion_mem_override_i,
    .debug_in_i             (debug_in),
    .spio_rc_resumeack_i,
    .minion_dbg_sig_enable_i,
    .minion_dbg_signals_o   (minion_dbg_signals),
    .minion_dbg_signals_mux_i,
    .esr_bypass_icache_i,
    .esr_bypass_dcache_i,
    .ram_cfg_i              (ram_cfg),
    .esr_icache_err_log_ctl_i(err_log_ctl),
    .esr_icache_err_log_sbe_o,
    .esr_icache_err_log_dbe_o,
    .esr_icache_err_log_info_o(err_log_info),
    .icache_bist_req_info_i (bist_req),
    .icache_bist_rsp_info_o (bist_rsp),
    .etl_req_ready_i,
    .etl_req_valid_o,
    .etl_req_info_o         (etl_req_info),
    .etl_rsp_ready_o,
    .etl_rsp_valid_i,
    .etl_rsp_info_i         (etl_rsp_info),
    .te_enable_i,
    .trace_encoder_o        (trace_encoder),
    .esr_spdmctrl_o         (esr_spdmctrl),
    .esr_sphastatus_o       (esr_sphastatus),
    .apb_paddr_i,
    .apb_penable_i,
    .apb_prdata_o,
    .apb_pready_o,
    .apb_psel_i,
    .apb_pslverr_o,
    .apb_pwdata_i,
    .apb_pwrite_i
  );

endmodule : standalone_minion_tb
/* verilator lint_on SYNCASYNCNET */
