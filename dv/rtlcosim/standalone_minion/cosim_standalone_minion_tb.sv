// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original standalone_minion vs translated shell.

`include "soc.vh"

/* verilator lint_off SYNCASYNCNET */  // Cosim intentionally adapts active-high legacy resets to translated active-low reset domains.
module cosim_standalone_minion_tb (
  input  logic                                                    clk_i,
  input  logic                                                    rst_ni,
  input  logic                                                    rst_debug_ni,
  input  logic                                                    rst_non_debug_ni,

  input  logic [`SHIRE_DFT_SCANIN_SA_MINION_WIDTH-1:0]            stim_dft_scanin,
  input  logic                                                    stim_dft_scan_mode,
  input  logic                                                    stim_dft_reset_byp,
  input  logic                                                    stim_dft_scan_enable,
  input  logic [`SHIRE_DFT_MODE_WIDTH-1:0]                        stim_dft_test_mode,
  input  logic                                                    stim_dft_reset,
  input  logic                                                    stim_dft_clock_gate_en,
  input  logic [`SHIRE_DFT_CNTL_WIDTH-1:0]                        stim_dft_cntl,
  input  logic                                                    stim_dft_occ_scanin,
  input  logic                                                    stim_dft_occ_reset,
  input  logic                                                    stim_dft_occ_testmode,
  input  logic                                                    stim_dft_scan_ate_clk,
  input  logic                                                    stim_dft_occ_bypass,
  input  logic                                                    stim_dft_sram_clk,
  input  logic                                                    stim_dft_clk_override,

  input  logic [minion_pkg::EtEcoMinionInputWidth-1:0]            stim_eco_i,
  input  logic                                                    stim_ioshire,
  input  logic                                                    stim_int_mieco,
  input  logic                                                    stim_int_mtip,
  input  logic                                                    stim_int_meip,
  input  logic                                                    stim_int_seip,
  input  logic [minion_pkg::NrThreads-1:0]                        stim_int_msip,
  input  logic [minion_pkg::NumShireIdsBits-1:0]                  stim_shire_id,
  input  logic [minion_pkg::MinPerSBits-1:0]                      stim_shire_min_id,
  input  logic [minion_pkg::NrThreads-1:0]                        stim_thread_enable,
  input  logic [minion_pkg::VaSize-1:0]                           stim_minion_boot_addr,
  input  logic [$bits(minion_dcache_pkg::esr_mprot_t)-1:0]        stim_mprot,
  input  logic [$bits(minion_pkg::tlb_entry_type)-1:0]            stim_vmspagesize,
  input  logic [$bits(minion_pkg::esr_minion_features_t)-1:0]     stim_minion_features,
  input  logic                                                    stim_shire_coop_mode,
  input  logic [minion_pkg::ShireMinionMemOverrideBits-1:0]       stim_minion_mem_override,
  input  logic [$bits(minion_pkg::minion_debug_in_t)-1:0]         stim_debug_in,
  input  logic                                                    stim_spio_rc_resumeack,
  input  logic                                                    stim_minion_dbg_sig_enable,
  input  logic [minion_pkg::NeighDebugSmMuxWidth-1:0]             stim_minion_dbg_signals_mux,
  input  logic                                                    stim_esr_bypass_icache,
  input  logic                                                    stim_esr_bypass_dcache,
  input  logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0]                stim_ram_cfg,
  input  logic [$bits(icache_pkg::esr_icache_err_log_ctl_t)-1:0]  stim_esr_icache_err_log_ctl,
  input  logic [$bits(shirecache_pkg::icache_bist_req_t)-1:0]     stim_icache_bist_req_info,
  input  logic                                                    stim_etl_req_ready,
  input  logic                                                    stim_etl_rsp_valid,
  input  logic [$bits(etlink_pkg::rsp_t)-1:0]                     stim_etl_rsp_info,
  input  logic                                                    stim_te_enable,
  input  logic [15:0]                                             stim_apb_paddr,
  input  logic                                                    stim_apb_penable,
  input  logic                                                    stim_apb_psel,
  input  logic [minion_pkg::BpamShireApbDataWidth-1:0]            stim_apb_pwdata,
  input  logic                                                    stim_apb_pwrite,

  output logic [`SHIRE_DFT_SCANOUT_SA_MINION_WIDTH-1:0]           orig_dft_scanout,
  output logic [`SHIRE_DFT_SCANOUT_SA_MINION_WIDTH-1:0]           new_dft_scanout,
  output logic                                                    orig_dft_occ_scanout,
  output logic                                                    new_dft_occ_scanout,
  output logic [minion_pkg::EtEcoMinionOutputWidth-1:0]           orig_eco_o,
  output logic [minion_pkg::EtEcoMinionOutputWidth-1:0]           new_eco_o,
  output logic [$bits(minion_pkg::neigh_sm_dbg_monitor_t)-1:0]    orig_minion_dbg_signals,
  output logic [$bits(minion_pkg::neigh_sm_dbg_monitor_t)-1:0]    new_minion_dbg_signals,
  output logic                                                    orig_esr_icache_err_log_sbe,
  output logic                                                    new_esr_icache_err_log_sbe,
  output logic                                                    orig_esr_icache_err_log_dbe,
  output logic                                                    new_esr_icache_err_log_dbe,
  output logic [$bits(icache_pkg::icache_err_log_info_t)-1:0]     orig_esr_icache_err_log_info,
  output logic [$bits(icache_pkg::icache_err_log_info_t)-1:0]     new_esr_icache_err_log_info,
  output logic [$bits(shirecache_pkg::icache_bist_rsp_t)-1:0]     orig_icache_bist_rsp_info,
  output logic [$bits(shirecache_pkg::icache_bist_rsp_t)-1:0]     new_icache_bist_rsp_info,
  output logic                                                    orig_etl_req_valid,
  output logic                                                    new_etl_req_valid,
  output logic [$bits(etlink_pkg::req_t)-1:0]                     orig_etl_req_info,
  output logic [$bits(etlink_pkg::req_t)-1:0]                     new_etl_req_info,
  output logic                                                    orig_etl_rsp_ready,
  output logic                                                    new_etl_rsp_ready,
  output logic [$bits(minion_pkg::trace_encoder_signals_t)-1:0]   orig_trace_encoder,
  output logic [$bits(minion_pkg::trace_encoder_signals_t)-1:0]   new_trace_encoder,
  output logic [$bits(esr_pkg::esr_spdmctrl_t)-1:0]               orig_esr_spdmctrl,
  output logic [$bits(esr_pkg::esr_spdmctrl_t)-1:0]               new_esr_spdmctrl,
  output logic [$bits(esr_pkg::esr_sphastatus_t)-1:0]             orig_esr_sphastatus,
  output logic [$bits(esr_pkg::esr_sphastatus_t)-1:0]             new_esr_sphastatus,
  output logic [minion_pkg::BpamShireApbDataWidth-1:0]            orig_apb_prdata,
  output logic [minion_pkg::BpamShireApbDataWidth-1:0]            new_apb_prdata,
  output logic                                                    orig_apb_pready,
  output logic                                                    new_apb_pready,
  output logic                                                    orig_apb_pslverr,
  output logic                                                    new_apb_pslverr
);

  dft_pkg::dft_t dft_new;
  ram_cfg_pkg::ram_cfg_t ram_cfg_new;
  minion_dcache_pkg::esr_mprot_t mprot_new;
  minion_pkg::esr_minion_features_t minion_features_new;
  minion_pkg::tlb_entry_type vmspagesize_new;
  minion_pkg::minion_debug_in_t debug_in_new;
  icache_pkg::esr_icache_err_log_ctl_t err_log_ctl_new;
  shirecache_pkg::icache_bist_req_t bist_req_new;
  etlink_pkg::rsp_t etl_rsp_new;

  esr_mprot_t mprot_orig;
  tlb_entry_type vmspagesize_orig;
  minion_debug_in_t debug_in_orig;
  esr_shire_cache_ram_cfg_t ram_cfg_orig;
  esr_icache_err_log_ctl_t err_log_ctl_orig;
  sc_icache_bist_req_t bist_req_orig;
  et_link_rsp_info_t etl_rsp_orig;

  minion_pkg::neigh_sm_dbg_monitor_t minion_dbg_new;
  icache_pkg::icache_err_log_info_t err_log_info_new;
  shirecache_pkg::icache_bist_rsp_t bist_rsp_new;
  etlink_pkg::req_t etl_req_new;
  minion_pkg::trace_encoder_signals_t trace_encoder_new;
  esr_pkg::esr_spdmctrl_t esr_spdmctrl_new;
  esr_pkg::esr_sphastatus_t esr_sphastatus_new;

  always_comb begin
    // The project DFT bundle has one scan/reset-bypass mode bit. The original
    // standalone shell's independent reset-bypass port is still stimulated and
    // connected on the original side for port coverage, but translated reset
    // bypass equivalence is constrained to the legal combined scanmode path.
    dft_new = '{
      scanmode: stim_dft_scan_mode,
      scan_reset_n: ~stim_dft_reset,
      sram_clk_override: stim_dft_clk_override,
      ram_rei: stim_dft_cntl[0],
      ram_wei: stim_dft_cntl[1]
    };
    ram_cfg_new = ram_cfg_pkg::ram_cfg_t'(stim_ram_cfg);
    mprot_new = minion_dcache_pkg::esr_mprot_t'(stim_mprot);
    minion_features_new = minion_pkg::esr_minion_features_t'(stim_minion_features);
    vmspagesize_new = minion_pkg::tlb_entry_type'(stim_vmspagesize);
    debug_in_new = minion_pkg::minion_debug_in_t'(stim_debug_in);
    err_log_ctl_new = icache_pkg::esr_icache_err_log_ctl_t'(stim_esr_icache_err_log_ctl);
    bist_req_new = shirecache_pkg::icache_bist_req_t'(stim_icache_bist_req_info);
    etl_rsp_new = etlink_pkg::rsp_t'(stim_etl_rsp_info);

    mprot_orig = esr_mprot_t'(stim_mprot);
    vmspagesize_orig = tlb_entry_type'(stim_vmspagesize);
    debug_in_orig = minion_debug_in_t'(stim_debug_in);
    err_log_ctl_orig = esr_icache_err_log_ctl_t'(stim_esr_icache_err_log_ctl);
    bist_req_orig = sc_icache_bist_req_t'(stim_icache_bist_req_info);
    etl_rsp_orig = et_link_rsp_info_t'(stim_etl_rsp_info);
    ram_cfg_orig = '0;
    ram_cfg_orig.dft__ram_rei = stim_dft_cntl[0];
    ram_cfg_orig.dft__ram_wei = stim_dft_cntl[1];
    ram_cfg_orig.cfg4.sc_mbi_wpulse = ram_cfg_new.wpulse;
    ram_cfg_orig.cfg4.sc_mbi_wa = {1'b0, ram_cfg_new.wa};
    ram_cfg_orig.cfg4.sc_mbi_ra = ram_cfg_new.ra;
    ram_cfg_orig.cfg4.sc_mbi_rm = ram_cfg_new.rm;
    ram_cfg_orig.cfg4.sc_mbi_rme = ram_cfg_new.rme;
  end

  standalone_minion_orig u_orig (
    .clock                    (clk_i),
    .reset                    (!rst_ni),
    .reset_debug              (!rst_debug_ni),
    .reset_non_debug          (!rst_non_debug_ni),
    .dft__scanin              (stim_dft_scanin),
    .dft__scanout             (orig_dft_scanout),
    .dft__scan_mode           (stim_dft_scan_mode),
    .dft__reset_byp           (stim_dft_reset_byp),
    .dft__scan_enable         (stim_dft_scan_enable),
    .dft__test_mode           (stim_dft_test_mode),
    .dft__reset               (stim_dft_reset),
    .dft__clock_gate_en       (stim_dft_clock_gate_en),
    .dft__cntl                (stim_dft_cntl),
    .dft__occ_scanin          (stim_dft_occ_scanin),
    .dft__occ_scanout         (orig_dft_occ_scanout),
    .dft__occ_reset           (stim_dft_occ_reset),
    .dft__occ_testmode        (stim_dft_occ_testmode),
    .dft__scan_ate_clk        (stim_dft_scan_ate_clk),
    .dft__occ_bypass          (stim_dft_occ_bypass),
    .dft__sram_clock          (stim_dft_sram_clk),
    .dft__clk_override        (stim_dft_clk_override),
    .eco_i                    (stim_eco_i),
    .eco_o                    (orig_eco_o),
    .ioshire                  (stim_ioshire),
    .int_mieco                (stim_int_mieco),
    .int_mtip                 (stim_int_mtip),
    .int_meip                 (stim_int_meip),
    .int_seip                 (stim_int_seip),
    .int_msip                 (stim_int_msip),
    .shire_id                 (stim_shire_id),
    .shire_min_id             (stim_shire_min_id),
    .thread_enable            (stim_thread_enable),
    .minion_boot_addr         (stim_minion_boot_addr),
    .mprot                    (mprot_orig),
    .vmspagesize              (vmspagesize_orig),
    .minion_features          (stim_minion_features),
    .shire_coop_mode          (stim_shire_coop_mode),
    .minion_mem_override      (stim_minion_mem_override),
    .debug_in                 (debug_in_orig),
    .spio_rc_resumeack        (stim_spio_rc_resumeack),
    .minion_dbg_sig_enable    (stim_minion_dbg_sig_enable),
    .minion_dbg_signals       (orig_minion_dbg_signals),
    .minion_dbg_signals_mux   (stim_minion_dbg_signals_mux),
    .esr_bypass_icache        (stim_esr_bypass_icache),
    .esr_bypass_dcache        (stim_esr_bypass_dcache),
    .esr_shire_cache_ram_cfg  (ram_cfg_orig),
    .esr_icache_err_log_ctl   (err_log_ctl_orig),
    .esr_icache_err_log_sbe   (orig_esr_icache_err_log_sbe),
    .esr_icache_err_log_dbe   (orig_esr_icache_err_log_dbe),
    .esr_icache_err_log_info  (orig_esr_icache_err_log_info),
    .icache_bist_req_info     (bist_req_orig),
    .icache_bist_rsp_info     (orig_icache_bist_rsp_info),
    .etl_req_ready            (stim_etl_req_ready),
    .etl_req_valid            (orig_etl_req_valid),
    .etl_req_info             (orig_etl_req_info),
    .etl_rsp_ready            (orig_etl_rsp_ready),
    .etl_rsp_valid            (stim_etl_rsp_valid),
    .etl_rsp_info             (etl_rsp_orig),
    .te_enable                (stim_te_enable),
    .traceEncoder             (orig_trace_encoder),
    .esr_spdmctrl             (orig_esr_spdmctrl),
    .esr_sphastatus           (orig_esr_sphastatus),
    .apb_paddr                (stim_apb_paddr),
    .apb_penable              (stim_apb_penable),
    .apb_prdata               (orig_apb_prdata),
    .apb_pready               (orig_apb_pready),
    .apb_psel                 (stim_apb_psel),
    .apb_pslverr              (orig_apb_pslverr),
    .apb_pwdata               (stim_apb_pwdata),
    .apb_pwrite               (stim_apb_pwrite)
  );

  standalone_minion u_new (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .rst_debug_ni             (rst_debug_ni),
    .rst_non_debug_ni         (rst_non_debug_ni),
    .dft_i                    (dft_new),
    .dft_sram_clk_i           (stim_dft_sram_clk),
    .dft_scanout_o            (new_dft_scanout),
    .dft_occ_scanout_o        (new_dft_occ_scanout),
    .eco_i                    (stim_eco_i),
    .eco_o                    (new_eco_o),
    .ioshire_i                (stim_ioshire),
    .int_mieco_i              (stim_int_mieco),
    .int_mtip_i               (stim_int_mtip),
    .int_meip_i               (stim_int_meip),
    .int_seip_i               (stim_int_seip),
    .int_msip_i               (stim_int_msip),
    .shire_id_i               (stim_shire_id),
    .shire_min_id_i           (stim_shire_min_id),
    .thread_enable_i          (stim_thread_enable),
    .minion_boot_addr_i       (stim_minion_boot_addr),
    .mprot_i                  (mprot_new),
    .vmspagesize_i            (vmspagesize_new),
    .minion_features_i        (minion_features_new),
    .shire_coop_mode_i        (stim_shire_coop_mode),
    .minion_mem_override_i    (stim_minion_mem_override),
    .debug_in_i               (debug_in_new),
    .spio_rc_resumeack_i      (stim_spio_rc_resumeack),
    .minion_dbg_sig_enable_i  (stim_minion_dbg_sig_enable),
    .minion_dbg_signals_o     (minion_dbg_new),
    .minion_dbg_signals_mux_i (stim_minion_dbg_signals_mux),
    .esr_bypass_icache_i      (stim_esr_bypass_icache),
    .esr_bypass_dcache_i      (stim_esr_bypass_dcache),
    .ram_cfg_i                (ram_cfg_new),
    .esr_icache_err_log_ctl_i (err_log_ctl_new),
    .esr_icache_err_log_sbe_o (new_esr_icache_err_log_sbe),
    .esr_icache_err_log_dbe_o (new_esr_icache_err_log_dbe),
    .esr_icache_err_log_info_o(err_log_info_new),
    .icache_bist_req_info_i   (bist_req_new),
    .icache_bist_rsp_info_o   (bist_rsp_new),
    .etl_req_ready_i          (stim_etl_req_ready),
    .etl_req_valid_o          (new_etl_req_valid),
    .etl_req_info_o           (etl_req_new),
    .etl_rsp_ready_o          (new_etl_rsp_ready),
    .etl_rsp_valid_i          (stim_etl_rsp_valid),
    .etl_rsp_info_i           (etl_rsp_new),
    .te_enable_i              (stim_te_enable),
    .trace_encoder_o          (trace_encoder_new),
    .esr_spdmctrl_o           (esr_spdmctrl_new),
    .esr_sphastatus_o         (esr_sphastatus_new),
    .apb_paddr_i              (stim_apb_paddr),
    .apb_penable_i            (stim_apb_penable),
    .apb_prdata_o             (new_apb_prdata),
    .apb_pready_o             (new_apb_pready),
    .apb_psel_i               (stim_apb_psel),
    .apb_pslverr_o            (new_apb_pslverr),
    .apb_pwdata_i             (stim_apb_pwdata),
    .apb_pwrite_i             (stim_apb_pwrite)
  );

  assign new_minion_dbg_signals = minion_dbg_new;
  assign new_esr_icache_err_log_info = err_log_info_new;
  assign new_icache_bist_rsp_info = bist_rsp_new;
  assign new_etl_req_info = etl_req_new;
  assign new_trace_encoder = trace_encoder_new;
  assign new_esr_spdmctrl = esr_spdmctrl_new;
  assign new_esr_sphastatus = esr_sphastatus_new;

endmodule : cosim_standalone_minion_tb
/* verilator lint_on SYNCASYNCNET */
