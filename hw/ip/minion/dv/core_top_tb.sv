// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion core_top.
//
// Stubs the external SoC-facing interfaces and exposes a few integration-level
// observables for a smoke test: reset tracking, debug handshakes, the frontend
// icache request, and the minion-local debug APB window.

/* verilator lint_off SYNCASYNCNET */  // The wrapper only forwards DUT reset nets, which are intentionally async in the core and appear synchronous at the harness boundary.
module core_top_tb
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
  output logic                              icache_req_valid_o,
  output logic [0:0]                        icache_req_thread_id_o,
  output logic [VaSizeExt-1:0]              icache_req_addr_o,
  output logic [NrThreads-1:0]              debug_halted_o,
  output logic [NrThreads-1:0]              debug_running_o,
  output logic [NrThreads-1:0]              debug_have_reset_o,
  output logic                              apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]  apb_prdata_o,
  output logic                              apb_pslverr_o,
  output logic                              trace_min_reset_o
);

  icache_fe_resp_t         icache_resp;
  minion_debug_in_t        debug_in;
  /* verilator lint_off UNUSEDSIGNAL */  // The smoke wrapper intentionally exposes only a small observable subset of these packed DUT-facing structs.
  fe_icache_req_t          icache_req;
  minion_debug_out_t       debug_out;
  trace_encoder_signals_t  trace_encoder;
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    icache_resp = '0;
    icache_resp.data = icache_resp_data_i;

    debug_in = '0;
    debug_in.halt = debug_halt_i;
    debug_in.resume = debug_resume_i;
    debug_in.resethalt = debug_resethalt_i;
    debug_in.ackhavereset = debug_ackhavereset_i;
  end

  assign icache_req_valid_o = icache_req_valid;
  assign icache_req_thread_id_o = icache_req.thread_id;
  assign icache_req_addr_o = icache_req.addr;
  assign debug_halted_o = debug_out.halted;
  assign debug_running_o = debug_out.running;
  assign debug_have_reset_o = debug_out.have_reset;
  assign trace_min_reset_o = trace_encoder.min_reset;

  logic icache_req_valid;

  /* verilator lint_off PINCONNECTEMPTY */  // The smoke wrapper intentionally leaves unrelated subsystem outputs open.
  core_top #(
    .DebugApbEn(DebugApbEn),
    .DebugMonEn(DebugMonEn),
    .TraceEn(TraceEn),
    .VpuEn(VpuEn)
  ) u_dut (
    .clk_i                      (clk_i),
    .rst_c_ni                   (rst_c_ni),
    .rst_w_ni                   (rst_w_ni),
    .rst_d_ni                   (rst_d_ni),
    .dft_i                      ('0),
    .ioshire_i                  (1'b0),
    .chicken_bit_dcache_i       (1'b0),
    .chicken_bit_intpipe_i      (1'b0),
    .chicken_bit_frontend_i     (1'b0),
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
    .id_vpu_req_o               (),
    .id_vpu_decoder_sigs_o      (),
    .ex_vpu_req_o               (),
    .tag_vpu_kill_o             (),
    .mem_vpu_kill_o             (),
    .wb_vpu_kill_o              (),
    .f0_vpu_ctrl_o              (),
    .wb_dcache_vpu_resp_valid_o (),
    .wb_dcache_vpu_resp_o       (),
    .vpu_dcache_ctl_i           ('0),
    .dcache_vpu_scp_res_o       (),
    .dcache_vpu_scp_data_o      (),
    .dcache_vpu_tenb_data_o     (),
    .vpu_reduce_ctrl_o          (),
    .id_vpu_ctrl_i              ('0),
    .ex_vpu_ctrl_i              ('0),
    .f2_vpu_ctrl_i              ('0),
    .f3_vpu_ctrl_i              ('0),
    .wb_vpu_ctrl_i              ('0),
    .flb_neigh_req_valid_o      (),
    .flb_neigh_req_data_o       (),
    .flb_neigh_resp_valid_i     (1'b0),
    .flb_neigh_resp_data_i      (1'b0),
    .te_thread_sel_i            (1'b0),
    .trace_encoder_o            (trace_encoder),
    .te_enable_i                (1'b0),
    .apb_paddr_i                (apb_paddr_i),
    .apb_pwrite_i               (apb_pwrite_i),
    .apb_psel_i                 (apb_psel_i),
    .apb_penable_i              (apb_penable_i),
    .apb_pwdata_i               (apb_pwdata_i),
    .apb_pready_o               (apb_pready_o),
    .apb_prdata_o               (apb_prdata_o),
    .apb_pslverr_o              (apb_pslverr_o),
    .debug_in_i                 (debug_in),
    .debug_out_o                (debug_out),
    .esr_features_i             ('0),
    .esr_bypass_dcache_i        (1'b0),
    .esr_shire_coop_mode_i      (1'b0),
    .mem_ctrl_override_i        ('0),
    .pmu_count_up_o             (),
    .pmu_read_data_i            ('0),
    .pmu_read_sel_o             (),
    .pmu_write_en_o             (),
    .pmu_write_data_o           (),
    .pmu_neigh_event_sel_o      (),
    .io_events_vpu_i            ('0),
    .sm_match_debug_signals_tl0_o(),
    .sm_filter_debug_signals_tl0_o(),
    .sm_data_debug_signals_tl0_o(),
    .sm_match_debug_signals_tl1_o(),
    .sm_filter_debug_signals_tl1_o(),
    .sm_data_debug_signals_tl1_o(),
    .sm_match_debug_signals_ts_o(),
    .sm_filter_debug_signals_ts_o(),
    .sm_data_debug_signals_ts_o (),
    .sm_match_debug_signals_co_o(),
    .sm_filter_debug_signals_co_o(),
    .sm_data_debug_signals_co_o (),
    .sm_match_debug_signals_col2_o(),
    .sm_filter_debug_signals_col2_o(),
    .sm_data_debug_signals_col2_o(),
    .sm_match_debug_signals_mh_o(),
    .sm_filter_debug_signals_mh_o(),
    .sm_data_debug_signals_mh_o (),
    .sm_match_debug_signals_dc_o(),
    .sm_filter_debug_signals_dc_o(),
    .sm_data_debug_signals_dc_o (),
    .intpipe_debug_monitor_o    ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

endmodule
/* verilator lint_on SYNCASYNCNET */
