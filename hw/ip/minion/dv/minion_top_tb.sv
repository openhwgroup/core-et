// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_top.
//
// Stubs the SoC-facing interfaces and exposes a small set of top-level
// observables for bring-up smoke coverage: reset propagation, the wrapped core
// fetch path, the debug APB path, the top-level debug monitor mux, power
// control passthrough, and the ECO tie-off behavior.

/* verilator lint_off SYNCASYNCNET */  // The wrapper forwards async top-level resets into the translated reset repeaters and only observes their effects at the DUT boundary.
module minion_top_tb
  import minion_pkg::*;
  import minion_frontend_pkg::*;
#(
  parameter bit DebugApbEn = 1'b1,
  parameter bit DebugMonEn = 1'b1,
  parameter bit TraceEn = 1'b1,
  parameter bit VpuEn = 1'b1
) (
  input  logic                              clk_i,
  input  logic                              rst_c_ni,
  input  logic                              rst_w_ni,
  input  logic                              rst_d_ni,
  input  logic [NrThreads-1:0]              enabled_i,
  input  logic [VaSize-1:0]                 reset_vector_i,
  input  logic                              icache_req_ready_i,
  input  logic                              icache_resp_valid_i,
  input  logic [FeFetchReadSize-1:0]        icache_resp_data_i,
  input  logic [NrThreads-1:0]              debug_halt_i,
  input  logic [NrThreads-1:0]              debug_resume_i,
  input  logic [NrThreads-1:0]              debug_resethalt_i,
  input  logic [NrThreads-1:0]              debug_ackhavereset_i,
  input  logic [MinDmApbAddrWidth-1:0]      apb_paddr_i,
  input  logic                              apb_pwrite_i,
  input  logic                              apb_psel_i,
  input  logic                              apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]  apb_pwdata_i,
  input  logic [NeighDebugSmMuxWidth-1:0]   minion_dbg_signals_mux_i,
  input  logic                              minion_dbg_sig_enable_i,
  input  logic                              te_enable_i,
  input  logic                              nsleepin_i,
  output logic                              icache_req_valid_o,
  output logic [0:0]                        icache_req_thread_id_o,
  output logic [VaSizeExt-1:0]              icache_req_addr_o,
  output logic [NrThreads-1:0]              debug_halted_o,
  output logic [NrThreads-1:0]              debug_running_o,
  output logic [NrThreads-1:0]              debug_have_reset_o,
  output logic                              apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]  apb_prdata_o,
  output logic                              apb_pslverr_o,
  output logic                              trace_min_reset_o,
  output logic                              nsleepout_o,
  output logic [EtEcoMinionOutputWidth-1:0] eco_o,
  output logic                              dbg_filter_valid_o,
  output logic                              dbg_match_valid_o,
  output logic                              dbg_data_valid_o,
  output logic [NeighDebugDataWidth-1:0]    dbg_data_o,
  output logic                              trace_instr_valid_o,
  output logic [InstSize-1:0]               trace_instr_bus_o,
  output logic [ShireTeInstrAddrWidth-1:0]  trace_instr_addr_o,
  output logic                              trace_exception_o
);

  icache_fe_resp_t        icache_resp;
  minion_debug_in_t       debug_in;
  esr_minion_features_t   esr_features;
  /* verilator lint_off UNUSEDSIGNAL */  // The smoke wrapper intentionally observes only a subset of the packed top-level interface bundles.
  fe_icache_req_t         icache_req;
  minion_debug_out_t      debug_out;
  trace_encoder_signals_t trace_encoder;
  neigh_sm_dbg_monitor_t  minion_dbg_signals;
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    icache_resp = '0;
    icache_resp.data = icache_resp_data_i;

    debug_in = '0;
    debug_in.halt = debug_halt_i;
    debug_in.resume = debug_resume_i;
    debug_in.resethalt = debug_resethalt_i;
    debug_in.ackhavereset = debug_ackhavereset_i;

    esr_features = '0;
    esr_features.trap_on_ml = 1'b1;
    esr_features.trap_on_gfx = 1'b1;
    esr_features.trap_on_u_scp = 1'b1;
    esr_features.trap_on_u_cacheops = 1'b1;
  end

  assign icache_req_valid_o = icache_req_valid;
  assign icache_req_thread_id_o = icache_req.thread_id;
  assign icache_req_addr_o = icache_req.addr;
  assign debug_halted_o = debug_out.halted;
  assign debug_running_o = debug_out.running;
  assign debug_have_reset_o = debug_out.have_reset;
  assign trace_min_reset_o = trace_encoder.min_reset;
  assign dbg_filter_valid_o = minion_dbg_signals.filter_valid;
  assign dbg_match_valid_o = minion_dbg_signals.match_valid;
  assign dbg_data_valid_o = minion_dbg_signals.data_valid;
  assign dbg_data_o = minion_dbg_signals.data_signals;
  assign trace_instr_valid_o = trace_encoder.instr_valid[0];
  assign trace_instr_bus_o = trace_encoder.instr_bus[InstSize-1:0];
  assign trace_instr_addr_o = trace_encoder.instr_addr[ShireTeInstrAddrWidth-1:0];
  assign trace_exception_o = trace_encoder.exception[0];

  logic icache_req_valid;

  /* verilator lint_off PINCONNECTEMPTY */  // The smoke wrapper intentionally leaves unrelated SoC-facing outputs open while checking the top-level integration path.
  minion_top #(
    .DebugApbEn(DebugApbEn),
    .DebugMonEn(DebugMonEn),
    .TraceEn(TraceEn),
    .VpuEn(VpuEn)
  ) u_dut (
    .clk_i                      (clk_i),
    .rst_c_ni                   (rst_c_ni),
    .rst_d_ni                   (rst_d_ni),
    .rst_w_ni                   (rst_w_ni),
    .dft_i                      ('0),
    .eco_i                      (10'h3a5),
    .eco_o                      (eco_o),
    .ioshire_i                  (1'b0),
    .nsleepin_i                 (nsleepin_i),
    .iso_enable_i               (1'b0),
    .nsleepout_o                (nsleepout_o),
    .chicken_bits_i             ('0),
    .l2_dcache_evict_req_ready_i('1),
    .l2_dcache_evict_req_valid_o(),
    .l2_dcache_evict_req_o      (),
    .l2_dcache_miss_req_ready_i ('1),
    .l2_dcache_miss_req_valid_o (),
    .l2_dcache_miss_req_o       (),
    .l2_dcache_resp_ready_o     (),
    .l2_dcache_resp_valid_i     (1'b0),
    .l2_dcache_resp_i           ('0),
    .icache_req_ready_i         (icache_req_ready_i),
    .icache_req_valid_o         (icache_req_valid),
    .icache_req_o               (icache_req),
    .icache_resp_valid_i        (icache_resp_valid_i),
    .icache_resp_miss_i         (1'b0),
    .icache_resp_i              (icache_resp),
    .icache_fill_done_i         (1'b0),
    .icache_flush_data_o        (),
    .satp_info_o                (),
    .matp_info_o                (),
    .tlb_invalidate_o           (),
    .dc_ptw_req_data_o          (),
    .dc_ptw_req_valid_o         (),
    .dc_ptw_req_ready_i         (1'b1),
    .ptw_dc_resp_data_i         ('0),
    .ptw_dc_resp_valid_i        (1'b0),
    .interrupts_i               ('0),
    .shire_id_i                 ('0),
    .shire_min_id_i             ('0),
    .enabled_i                  (enabled_i),
    .reset_vector_i             (reset_vector_i),
    .mprot_i                    ('0),
    .vmspagesize_i              ('0),
    .flb_neigh_req_valid_o      (),
    .flb_neigh_req_data_o       (),
    .flb_neigh_resp_valid_i     (1'b0),
    .flb_neigh_resp_data_i      (1'b0),
    .te_thread_sel_i            (1'b0),
    .trace_encoder_o            (trace_encoder),
    .te_enable_i                (te_enable_i),
    .apb_paddr_i                (apb_paddr_i),
    .apb_penable_i              (apb_penable_i),
    .apb_prdata_o               (apb_prdata_o),
    .apb_pready_o               (apb_pready_o),
    .apb_psel_i                 (apb_psel_i),
    .apb_pslverr_o              (apb_pslverr_o),
    .apb_pwdata_i               (apb_pwdata_i),
    .apb_pwrite_i               (apb_pwrite_i),
    .debug_in_i                 (debug_in),
    .debug_out_o                (debug_out),
    .minion_dbg_signals_o       (minion_dbg_signals),
    .minion_dbg_signals_mux_i   (minion_dbg_signals_mux_i),
    .minion_dbg_sig_enable_i    (minion_dbg_sig_enable_i),
    .esr_features_i             (esr_features),
    .esr_bypass_dcache_i        (1'b0),
    .esr_shire_coop_mode_i      (1'b0),
    .esr_minion_mem_override_i  ('0),
    .pmu_count_up_o             (),
    .pmu_read_data_i            ('0),
    .pmu_read_sel_o             (),
    .pmu_write_en_o             (),
    .pmu_write_data_o           (),
    .pmu_neigh_event_sel_o      ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

endmodule
/* verilator lint_on SYNCASYNCNET */
