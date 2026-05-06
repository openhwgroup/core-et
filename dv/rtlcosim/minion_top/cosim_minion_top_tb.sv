// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original minion_top vs new minion_top.
//
// The default translated top instantiates the real translated vpu_top. The
// paired C++ test includes VPU fetch/debug stimulus while still comparing every
// minion_top output cycle-by-cycle against the original real-VPU top.

`include "soc.vh"

/* verilator lint_off SYNCASYNCNET */  // The wrapper intentionally fans the same async reset stimulus into both the translated active-low resets and the legacy active-high resets.
module cosim_minion_top_tb
  import minion_pkg::*;
  import minion_frontend_pkg::*;
(
  input  logic                              clk_i,
  input  logic                              rst_c_ni,
  input  logic                              rst_w_ni,
  input  logic                              rst_d_ni,

  input  logic [`SHIRE_DFT_SCANIN_MINION_WIDTH-1:0] stim_dft_scanin,
  input  logic                              stim_dft_scan_mode,
  input  logic                              stim_dft_reset_byp,
  input  logic                              stim_dft_scan_enable,
  input  logic [`SHIRE_DFT_MODE_WIDTH-1:0]  stim_dft_test_mode,
  input  logic                              stim_dft_reset,
  input  logic                              stim_dft_clock_gate_en,
  input  logic [`SHIRE_DFT_CNTL_WIDTH-1:0]  stim_dft_cntl,
  input  logic                              stim_dft_occ_scanin,
  input  logic                              stim_dft_occ_reset,
  input  logic                              stim_dft_occ_testmode,
  input  logic                              stim_dft_scan_ate_clk,
  input  logic                              stim_dft_occ_bypass,

  input  logic [NrThreads-1:0]              stim_enabled,
  input  logic [VaSize-1:0]                 stim_reset_vector,
  input  logic [EtEcoMinionInputWidth-1:0]  stim_eco_i,
  input  logic                              stim_ioshire,
  input  logic                              stim_nsleepin,
  input  logic                              stim_iso_enable,
  input  logic [$bits(minion_chicken_bits_t)-1:0] stim_chicken_bits,

  input  logic [DcacheL2EvictReqPorts-1:0]  stim_l2_dcache_evict_req_ready,
  input  logic [DcacheL2MissReqPorts-1:0]   stim_l2_dcache_miss_req_ready,
  input  logic                              stim_l2_dcache_resp_valid,
  input  logic [$bits(et_link_minion_rsp_info_t)-1:0] stim_l2_dcache_resp,

  input  logic                              stim_icache_req_ready,
  input  logic                              stim_icache_resp_valid,
  input  logic                              stim_icache_resp_miss,
  input  logic [FeFetchReadSize-1:0]        stim_icache_resp_data,
  input  logic [4:0]                        stim_icache_resp_status,
  input  logic                              stim_icache_fill_done,

  input  logic                              stim_dc_ptw_req_ready,
  input  logic [$bits(minion_ptw_pte)-1:0]  stim_ptw_dc_resp_data,
  input  logic                              stim_ptw_dc_resp_valid,

  input  logic [$bits(minion_interrupts)-1:0] stim_interrupts,
  input  logic [NumShireIdsBits-1:0]        stim_shire_id,
  input  logic [MinPerSBits-1:0]            stim_shire_min_id,
  input  logic [$bits(minion_dcache_pkg::esr_mprot_t)-1:0] stim_mprot,
  input  logic [$bits(tlb_entry_type)-1:0]  stim_vmspagesize,

  input  logic                              stim_flb_neigh_resp_valid,
  input  logic                              stim_flb_neigh_resp_data,
  input  logic                              stim_te_thread_sel,
  input  logic                              stim_te_enable,

  input  logic [NrThreads-1:0]              stim_debug_halt,
  input  logic [NrThreads-1:0]              stim_debug_resume,
  input  logic [NrThreads-1:0]              stim_debug_resethalt,
  input  logic [NrThreads-1:0]              stim_debug_ackhavereset,
  input  logic [MinDmApbAddrWidth-1:0]      stim_apb_paddr,
  input  logic                              stim_apb_pwrite,
  input  logic                              stim_apb_psel,
  input  logic                              stim_apb_penable,
  input  logic [BpamShireApbDataWidth-1:0]  stim_apb_pwdata,
  input  logic [NeighDebugSmMuxWidth-1:0]   stim_dbg_mux,
  input  logic                              stim_dbg_enable,

  input  logic [$bits(esr_minion_features_t)-1:0] stim_esr_features,
  input  logic                              stim_esr_bypass_dcache,
  input  logic                              stim_esr_shire_coop_mode,
  input  logic [ShireMinionMemOverrideBits-1:0] stim_esr_minion_mem_override,
  input  logic [NrThreads*XregSize-1:0]     stim_pmu_read_data,

  output logic [EtEcoMinionOutputWidth-1:0] orig_eco_o,
  output logic [EtEcoMinionOutputWidth-1:0] new_eco_o,
  output logic                              orig_nsleepout,
  output logic                              new_nsleepout,
  output logic [DcacheL2EvictReqPorts-1:0]  orig_l2_dcache_evict_req_valid,
  output logic [DcacheL2EvictReqPorts-1:0]  new_l2_dcache_evict_req_valid,
  output logic [$bits(et_link_minion_evict_req_info_t)-1:0] orig_l2_dcache_evict_req,
  output logic [$bits(et_link_minion_evict_req_info_t)-1:0] new_l2_dcache_evict_req,
  output logic [DcacheL2MissReqPorts-1:0]   orig_l2_dcache_miss_req_valid,
  output logic [DcacheL2MissReqPorts-1:0]   new_l2_dcache_miss_req_valid,
  output logic [$bits(et_link_minion_miss_req_info_t)-1:0]  orig_l2_dcache_miss_req,
  output logic [$bits(et_link_minion_miss_req_info_t)-1:0]  new_l2_dcache_miss_req,
  output logic                              orig_l2_dcache_resp_ready,
  output logic                              new_l2_dcache_resp_ready,
  output logic                              orig_icache_req_valid,
  output logic                              new_icache_req_valid,
  output logic [$bits(fe_icache_req_t)-1:0] orig_icache_req,
  output logic [$bits(fe_icache_req_t)-1:0] new_icache_req,
  output logic                              orig_icache_flush_data,
  output logic                              new_icache_flush_data,
  output logic [$bits(minion_satp_info)-1:0] orig_satp_info,
  output logic [$bits(minion_satp_info)-1:0] new_satp_info,
  output logic [$bits(minion_satp_info)-1:0] orig_matp_info,
  output logic [$bits(minion_satp_info)-1:0] new_matp_info,
  output logic                              orig_tlb_invalidate,
  output logic                              new_tlb_invalidate,
  output logic [$bits(minion_ptw_req)-1:0]  orig_dc_ptw_req_data,
  output logic [$bits(minion_ptw_req)-1:0]  new_dc_ptw_req_data,
  output logic                              orig_dc_ptw_req_valid,
  output logic                              new_dc_ptw_req_valid,
  output logic                              orig_flb_neigh_req_valid,
  output logic                              new_flb_neigh_req_valid,
  output logic [CsrFlBarrierSize-1:0]       orig_flb_neigh_req_data,
  output logic [CsrFlBarrierSize-1:0]       new_flb_neigh_req_data,
  output logic [$bits(trace_encoder_signals_t)-1:0] orig_trace_encoder,
  output logic [$bits(trace_encoder_signals_t)-1:0] new_trace_encoder,
  output logic [BpamShireApbDataWidth-1:0]  orig_apb_prdata,
  output logic [BpamShireApbDataWidth-1:0]  new_apb_prdata,
  output logic                              orig_apb_pready,
  output logic                              new_apb_pready,
  output logic                              orig_apb_pslverr,
  output logic                              new_apb_pslverr,
  output logic [$bits(minion_debug_out_t)-1:0] orig_debug_out,
  output logic [$bits(minion_debug_out_t)-1:0] new_debug_out,
  output logic [$bits(neigh_sm_dbg_monitor_t)-1:0] orig_minion_dbg_signals,
  output logic [$bits(neigh_sm_dbg_monitor_t)-1:0] new_minion_dbg_signals,
  output logic [PmuMinionCountersNum-1:0]   orig_pmu_count_up,
  output logic [PmuMinionCountersNum-1:0]   new_pmu_count_up,
  output logic [NrThreads*PmuCountersSelectBits-1:0] orig_pmu_read_sel,
  output logic [NrThreads*PmuCountersSelectBits-1:0] new_pmu_read_sel,
  output logic [PmuTotalCountersNum-1:0]    orig_pmu_write_en,
  output logic [PmuTotalCountersNum-1:0]    new_pmu_write_en,
  output logic [XregSize-1:0]               orig_pmu_write_data,
  output logic [XregSize-1:0]               new_pmu_write_data,
  output logic [PmuNeighEventCntSelBits-1:0] orig_pmu_neigh_event_sel,
  output logic [PmuNeighEventCntSelBits-1:0] new_pmu_neigh_event_sel
);

  dft_pkg::dft_t          stim_dft_new_t;
  icache_fe_resp_t        stim_icache_resp_t;
  minion_debug_in_t       stim_debug_in_t;
  esr_minion_features_t   stim_esr_features_t;
  minion_chicken_bits_t   stim_chicken_bits_t;
  minion_interrupts       stim_interrupts_t;
  et_link_minion_rsp_info_t stim_l2_dcache_resp_t;
  minion_ptw_pte          stim_ptw_dc_resp_data_t;
  minion_dcache_pkg::esr_mprot_t stim_mprot_new_t;
  esr_mprot_t             stim_mprot_orig_t;
  tlb_entry_type          stim_vmspagesize_t;
  logic [NrThreads-1:0][XregSize-1:0] stim_pmu_read_data_t;

  fe_icache_req_t          new_icache_req_t;
  minion_satp_info         new_satp_info_t;
  minion_satp_info         new_matp_info_t;
  minion_ptw_req           new_dc_ptw_req_data_t;
  trace_encoder_signals_t  new_trace_encoder_t;
  minion_debug_out_t       new_debug_out_t;
  neigh_sm_dbg_monitor_t   new_minion_dbg_signals_t;
  et_link_minion_evict_req_info_t new_l2_dcache_evict_req_t;
  et_link_minion_miss_req_info_t  new_l2_dcache_miss_req_t;
  logic [NrThreads-1:0][PmuCountersSelectBits-1:0] new_pmu_read_sel_t;

  always_comb begin
    // Project dft_t has one scanmode bit used by reset synchronizers and clock
    // gates. The original exposes reset-bypass and clock-gate test enables as
    // separate DFT controls, so the cosim only asserts these during safe
    // reset/idle phases and maps either asserted control to dft_t.scanmode.
    stim_dft_new_t = '{
      scanmode:          (stim_dft_reset_byp | stim_dft_clock_gate_en),
      scan_reset_n:      ~stim_dft_reset,
      sram_clk_override: 1'b0,
      ram_rei:           stim_dft_cntl[9],
      ram_wei:           stim_dft_cntl[9]
    };

    stim_icache_resp_t = '0;
    stim_icache_resp_t.data = stim_icache_resp_data;
    stim_icache_resp_t.page_fault = stim_icache_resp_status[0];
    stim_icache_resp_t.access_fault = stim_icache_resp_status[1];
    stim_icache_resp_t.cacheable = stim_icache_resp_status[2];
    stim_icache_resp_t.bus_err = stim_icache_resp_status[3];
    stim_icache_resp_t.ecc_err = stim_icache_resp_status[4];

    stim_debug_in_t = '0;
    stim_debug_in_t.halt = stim_debug_halt;
    stim_debug_in_t.resume = stim_debug_resume;
    stim_debug_in_t.resethalt = stim_debug_resethalt;
    stim_debug_in_t.ackhavereset = stim_debug_ackhavereset;

    stim_esr_features_t = esr_minion_features_t'(stim_esr_features);
    stim_chicken_bits_t = minion_chicken_bits_t'(stim_chicken_bits);
    stim_interrupts_t = minion_interrupts'(stim_interrupts);
    stim_l2_dcache_resp_t = et_link_minion_rsp_info_t'(stim_l2_dcache_resp);
    stim_ptw_dc_resp_data_t = minion_ptw_pte'(stim_ptw_dc_resp_data);
    stim_mprot_new_t = minion_dcache_pkg::esr_mprot_t'(stim_mprot);
    stim_mprot_orig_t = esr_mprot_t'(stim_mprot);
    stim_vmspagesize_t = tlb_entry_type'(stim_vmspagesize);
    stim_pmu_read_data_t = stim_pmu_read_data;
  end

  minion_top_orig u_orig (
    .clock                  (clk_i),
    .reset                  (!rst_c_ni),
    .reset_debug            (!rst_d_ni),
    .reset_non_debug        (!rst_w_ni),
    .dft__scanin            (stim_dft_scanin),
    .dft__scanout           (),
    .dft__scan_mode         (stim_dft_scan_mode),
    .dft__reset_byp         (stim_dft_reset_byp),
    .dft__scan_enable       (stim_dft_scan_enable),
    .dft__test_mode         (stim_dft_test_mode),
    .dft__reset             (stim_dft_reset),
    .dft__clock_gate_en     (stim_dft_clock_gate_en),
    .dft__cntl              (stim_dft_cntl),
    .dft__occ_scanin        (stim_dft_occ_scanin),
    .dft__occ_scanout       (),
    .dft__occ_reset         (stim_dft_occ_reset),
    .dft__occ_testmode      (stim_dft_occ_testmode),
    .dft__scan_ate_clk      (stim_dft_scan_ate_clk),
    .dft__occ_bypass        (stim_dft_occ_bypass),
    .eco_i                  (stim_eco_i),
    .eco_o                  (orig_eco_o),
    .ioshire                (stim_ioshire),
    .nsleepin               (stim_nsleepin),
    .iso_enable             (stim_iso_enable),
    .nsleepout              (orig_nsleepout),
    .chicken_bits           (stim_chicken_bits_t),
    .l2_dcache_evict_req_ready(stim_l2_dcache_evict_req_ready),
    .l2_dcache_evict_req_valid(orig_l2_dcache_evict_req_valid),
    .l2_dcache_evict_req    (orig_l2_dcache_evict_req),
    .l2_dcache_miss_req_ready(stim_l2_dcache_miss_req_ready),
    .l2_dcache_miss_req_valid(orig_l2_dcache_miss_req_valid),
    .l2_dcache_miss_req     (orig_l2_dcache_miss_req),
    .l2_dcache_resp_ready   (orig_l2_dcache_resp_ready),
    .l2_dcache_resp_valid   (stim_l2_dcache_resp_valid),
    .l2_dcache_resp         (stim_l2_dcache_resp_t),
    .icache_req_ready       (stim_icache_req_ready),
    .icache_req_valid       (orig_icache_req_valid),
    .icache_req             (orig_icache_req),
    .icache_resp_valid      (stim_icache_resp_valid),
    .icache_resp_miss       (stim_icache_resp_miss),
    .icache_resp            (stim_icache_resp_t),
    .icache_fill_done       (stim_icache_fill_done),
    .icache_flush_data      (orig_icache_flush_data),
    .satp_info              (orig_satp_info),
    .matp_info              (orig_matp_info),
    .tlb_invalidate         (orig_tlb_invalidate),
    .dc_ptw_req_data        (orig_dc_ptw_req_data),
    .dc_ptw_req_valid       (orig_dc_ptw_req_valid),
    .dc_ptw_req_ready       (stim_dc_ptw_req_ready),
    .ptw_dc_resp_data       (stim_ptw_dc_resp_data_t),
    .ptw_dc_resp_valid      (stim_ptw_dc_resp_valid),
    .interrupts             (stim_interrupts_t),
    .shire_id               (stim_shire_id),
    .shire_min_id           (stim_shire_min_id),
    .enabled                (stim_enabled),
    .reset_vector           (stim_reset_vector),
    .mprot                  (stim_mprot_orig_t),
    .vmspagesize            (stim_vmspagesize_t),
    .flb_neigh_req_valid    (orig_flb_neigh_req_valid),
    .flb_neigh_req_data     (orig_flb_neigh_req_data),
    .flb_neigh_resp_valid   (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data    (stim_flb_neigh_resp_data),
    .te_thread_sel          (stim_te_thread_sel),
    .traceEncoder           (orig_trace_encoder),
    .te_enable              (stim_te_enable),
    .apb_paddr              (stim_apb_paddr),
    .apb_penable            (stim_apb_penable),
    .apb_prdata             (orig_apb_prdata),
    .apb_pready             (orig_apb_pready),
    .apb_psel               (stim_apb_psel),
    .apb_pslverr            (orig_apb_pslverr),
    .apb_pwdata             (stim_apb_pwdata),
    .apb_pwrite             (stim_apb_pwrite),
    .debug_in               (stim_debug_in_t),
    .debug_out              (orig_debug_out),
    .minion_dbg_signals     (orig_minion_dbg_signals),
    .minion_dbg_signals_mux (stim_dbg_mux),
    .minion_dbg_sig_enable  (stim_dbg_enable),
    .esr_features           (stim_esr_features_t),
    .esr_bypass_dcache      (stim_esr_bypass_dcache),
    .esr_shire_coop_mode    (stim_esr_shire_coop_mode),
    .esr_minion_mem_override(stim_esr_minion_mem_override),
    .pmu_count_up           (orig_pmu_count_up),
    .pmu_read_data          (stim_pmu_read_data_t),
    .pmu_read_sel           (orig_pmu_read_sel),
    .pmu_write_en           (orig_pmu_write_en),
    .pmu_write_data         (orig_pmu_write_data),
    .pmu_neigh_event_sel    (orig_pmu_neigh_event_sel)
  );

  minion_top u_new (
    .clk_i                      (clk_i),
    .rst_c_ni                   (rst_c_ni),
    .rst_d_ni                   (rst_d_ni),
    .rst_w_ni                   (rst_w_ni),
    .dft_i                      (stim_dft_new_t),
    .eco_i                      (stim_eco_i),
    .eco_o                      (new_eco_o),
    .ioshire_i                  (stim_ioshire),
    .nsleepin_i                 (stim_nsleepin),
    .iso_enable_i               (stim_iso_enable),
    .nsleepout_o                (new_nsleepout),
    .chicken_bits_i             (stim_chicken_bits_t),
    .l2_dcache_evict_req_ready_i(stim_l2_dcache_evict_req_ready),
    .l2_dcache_evict_req_valid_o(new_l2_dcache_evict_req_valid),
    .l2_dcache_evict_req_o      (new_l2_dcache_evict_req_t),
    .l2_dcache_miss_req_ready_i (stim_l2_dcache_miss_req_ready),
    .l2_dcache_miss_req_valid_o (new_l2_dcache_miss_req_valid),
    .l2_dcache_miss_req_o       (new_l2_dcache_miss_req_t),
    .l2_dcache_resp_ready_o     (new_l2_dcache_resp_ready),
    .l2_dcache_resp_valid_i     (stim_l2_dcache_resp_valid),
    .l2_dcache_resp_i           (stim_l2_dcache_resp_t),
    .icache_req_ready_i         (stim_icache_req_ready),
    .icache_req_valid_o         (new_icache_req_valid),
    .icache_req_o               (new_icache_req_t),
    .icache_resp_valid_i        (stim_icache_resp_valid),
    .icache_resp_miss_i         (stim_icache_resp_miss),
    .icache_resp_i              (stim_icache_resp_t),
    .icache_fill_done_i         (stim_icache_fill_done),
    .icache_flush_data_o        (new_icache_flush_data),
    .satp_info_o                (new_satp_info_t),
    .matp_info_o                (new_matp_info_t),
    .tlb_invalidate_o           (new_tlb_invalidate),
    .dc_ptw_req_data_o          (new_dc_ptw_req_data_t),
    .dc_ptw_req_valid_o         (new_dc_ptw_req_valid),
    .dc_ptw_req_ready_i         (stim_dc_ptw_req_ready),
    .ptw_dc_resp_data_i         (stim_ptw_dc_resp_data_t),
    .ptw_dc_resp_valid_i        (stim_ptw_dc_resp_valid),
    .interrupts_i               (stim_interrupts_t),
    .shire_id_i                 (stim_shire_id),
    .shire_min_id_i             (stim_shire_min_id),
    .enabled_i                  (stim_enabled),
    .reset_vector_i             (stim_reset_vector),
    .mprot_i                    (stim_mprot_new_t),
    .vmspagesize_i              (stim_vmspagesize_t),
    .flb_neigh_req_valid_o      (new_flb_neigh_req_valid),
    .flb_neigh_req_data_o       (new_flb_neigh_req_data),
    .flb_neigh_resp_valid_i     (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data_i      (stim_flb_neigh_resp_data),
    .te_thread_sel_i            (stim_te_thread_sel),
    .trace_encoder_o            (new_trace_encoder_t),
    .te_enable_i                (stim_te_enable),
    .apb_paddr_i                (stim_apb_paddr),
    .apb_penable_i              (stim_apb_penable),
    .apb_prdata_o               (new_apb_prdata),
    .apb_pready_o               (new_apb_pready),
    .apb_psel_i                 (stim_apb_psel),
    .apb_pslverr_o              (new_apb_pslverr),
    .apb_pwdata_i               (stim_apb_pwdata),
    .apb_pwrite_i               (stim_apb_pwrite),
    .debug_in_i                 (stim_debug_in_t),
    .debug_out_o                (new_debug_out_t),
    .minion_dbg_signals_o       (new_minion_dbg_signals_t),
    .minion_dbg_signals_mux_i   (stim_dbg_mux),
    .minion_dbg_sig_enable_i    (stim_dbg_enable),
    .esr_features_i             (stim_esr_features_t),
    .esr_bypass_dcache_i        (stim_esr_bypass_dcache),
    .esr_shire_coop_mode_i      (stim_esr_shire_coop_mode),
    .esr_minion_mem_override_i  (stim_esr_minion_mem_override),
    .pmu_count_up_o             (new_pmu_count_up),
    .pmu_read_data_i            (stim_pmu_read_data_t),
    .pmu_read_sel_o             (new_pmu_read_sel_t),
    .pmu_write_en_o             (new_pmu_write_en),
    .pmu_write_data_o           (new_pmu_write_data),
    .pmu_neigh_event_sel_o      (new_pmu_neigh_event_sel)
  );

  assign new_l2_dcache_evict_req = new_l2_dcache_evict_req_t;
  assign new_l2_dcache_miss_req = new_l2_dcache_miss_req_t;
  assign new_icache_req = new_icache_req_t;
  assign new_satp_info = new_satp_info_t;
  assign new_matp_info = new_matp_info_t;
  assign new_dc_ptw_req_data = new_dc_ptw_req_data_t;
  assign new_trace_encoder = new_trace_encoder_t;
  assign new_debug_out = new_debug_out_t;
  assign new_minion_dbg_signals = new_minion_dbg_signals_t;
  assign new_pmu_read_sel = new_pmu_read_sel_t;

endmodule
/* verilator lint_on SYNCASYNCNET */
