// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// core_top — Minion core integration wrapper.
//
// Mechanical translation of rtl/shire/minion/core_top.v. This block wires the
// translated frontend, integer pipeline, dcache, debug APB slave, and trace
// encoder staging together while preserving the original cycle-level glue.

/* verilator lint_off UNUSEDPARAM */  // `UseMmi` is a preserved original top-level parameter kept for interface compatibility.
module core_top
  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import intpipe_csr_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned UseMmi = 1,  // Preserved for interface compatibility.
  parameter bit          DebugApbEn = 1'b1,
  parameter bit          DebugMonEn = 1'b1,
  parameter bit          TraceEn = 1'b1,
  parameter bit          VpuEn = 1'b1
) (
  input  logic                                               clk_i,
  input  logic                                               rst_c_ni,
  input  logic                                               rst_w_ni,
  input  logic                                               rst_d_ni,
  input  dft_t                                               dft_i,

  input  logic                                               ioshire_i,
  input  logic                                               chicken_bit_dcache_i,
  input  logic                                               chicken_bit_intpipe_i,
  input  logic                                               chicken_bit_frontend_i,

  input  logic [DcacheL2EvictReqPorts-1:0]                   l2_dcache_evict_req_ready_i,
  output logic [DcacheL2EvictReqPorts-1:0]                   l2_dcache_evict_req_valid_o,
  output minion_dcache_pkg::et_link_minion_evict_req_info_t  l2_dcache_evict_req_o,

  input  logic [DcacheL2MissReqPorts-1:0]                    l2_dcache_miss_req_ready_i,
  output logic [DcacheL2MissReqPorts-1:0]                    l2_dcache_miss_req_valid_o,
  output minion_dcache_pkg::et_link_minion_miss_req_info_t   l2_dcache_miss_req_o,

  output logic                                               l2_dcache_resp_ready_o,
  input  logic                                               l2_dcache_resp_valid_i,
  input  et_link_minion_rsp_info_t                           l2_dcache_resp_i,

  input  logic                                               icache_req_ready_i,
  output logic                                               icache_req_valid_o,
  output fe_icache_req_t                                     icache_req_o,

  input  logic                                               icache_resp_valid_i,
  input  logic                                               icache_resp_miss_i,
  input  icache_fe_resp_t                                    icache_resp_i,
  input  logic                                               icache_fill_done_i,

  output logic                                               icache_flush_data_o,

  output minion_satp_info                                    satp_info_o,
  output minion_satp_info                                    matp_info_o,
  output logic                                               tlb_invalidate_o,

  output minion_ptw_req                                      dc_ptw_req_data_o,
  output logic                                               dc_ptw_req_valid_o,
  input  logic                                               dc_ptw_req_ready_i,
  input  minion_ptw_pte                                      ptw_dc_resp_data_i,
  input  logic                                               ptw_dc_resp_valid_i,

  input  minion_interrupts                                   interrupts_i,
  input  logic [NumShireIdsBits-1:0]                         shire_id_i,
  input  logic [MinPerSBits-1:0]                             shire_min_id_i,
  input  logic [NrThreads-1:0]                               enabled_i,
  input  logic [VaSize-1:0]                                  reset_vector_i,
  input  minion_dcache_pkg::esr_mprot_t                      mprot_i,
  input  tlb_entry_type                                      vmspagesize_i,

  output minion_vpu_id_req                                   id_vpu_req_o,
  output vpu_ctrl_sigs_t                                     id_vpu_decoder_sigs_o,
  output minion_vpu_ex_req                                   ex_vpu_req_o,
  output logic                                               tag_vpu_kill_o,
  /* verilator lint_off UNOPTFLAT */  // Full-core lint flattens the preserved VPU kill path through dcache replay feedback and reports a false integrated cycle.
  output logic                                               mem_vpu_kill_o,
  /* verilator lint_on UNOPTFLAT */
  output logic                                               wb_vpu_kill_o,
  output core_vpu_ctrl                                       f0_vpu_ctrl_o,

  output logic                                               wb_dcache_vpu_resp_valid_o,
  output dcache_vpu_resp                                     wb_dcache_vpu_resp_o,

  input  vpu_dcache_ctrl                                     vpu_dcache_ctl_i,
  output dcache_vpu_scp_resp                                 dcache_vpu_scp_res_o,
  output logic [VpuDataSz-1:0]                               dcache_vpu_scp_data_o,
  output logic [VpuDataSz-1:0]                               dcache_vpu_tenb_data_o,
  output dcache_vpu_reduce_ctrl                              vpu_reduce_ctrl_o,

  input  vpu_id_ctrl_t                                       id_vpu_ctrl_i,
  input  vpu_minion_ex_ctrl                                  ex_vpu_ctrl_i,
  input  vpu_minion_tag_ctrl                                 f2_vpu_ctrl_i,
  input  vpu_minion_mem_ctrl                                 f3_vpu_ctrl_i,
  input  vpu_minion_wb_ctrl                                  wb_vpu_ctrl_i,

  output logic                                               flb_neigh_req_valid_o,
  output logic [CsrFlBarrierSize-1:0]                        flb_neigh_req_data_o,
  input  logic                                               flb_neigh_resp_valid_i,
  input  logic                                               flb_neigh_resp_data_i,

  input  logic                                               te_thread_sel_i,
  output trace_encoder_signals_t                             trace_encoder_o,
  input  logic                                               te_enable_i,

  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` structurally removes the external APB debug transport, so the preserved APB ingress remains intentionally unused in stripped builds.
  input  logic [MinDmApbAddrWidth-1:0]                       apb_paddr_i,
  input  logic                                               apb_pwrite_i,
  input  logic                                               apb_psel_i,
  input  logic                                               apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]                   apb_pwdata_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic                                               apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]                   apb_prdata_o,
  output logic                                               apb_pslverr_o,

  input  minion_debug_in_t                                   debug_in_i,
  output minion_debug_out_t                                  debug_out_o,

  input  esr_minion_features_t                               esr_features_i,
  input  logic                                               esr_bypass_dcache_i,
  input  logic                                               esr_shire_coop_mode_i,
  input  logic [MinionMemCtrlBits-1:0]                       mem_ctrl_override_i,

  output logic [PmuMinionCountersNum-1:0]                    pmu_count_up_o,
  input  logic [NrThreads-1:0][XregSize-1:0]                 pmu_read_data_i,
  output logic [NrThreads-1:0][PmuCountersSelectBits-1:0]    pmu_read_sel_o,
  output logic [PmuTotalCountersNum-1:0]                     pmu_write_en_o,
  output logic [XregSize-1:0]                                pmu_write_data_o,
  output logic [PmuNeighEventCntSelBits-1:0]                 pmu_neigh_event_sel_o,
  input  logic [CsrNrEventsVpu-1:0]                          io_events_vpu_i,

  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_tl0_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_tl0_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0]  sm_data_debug_signals_tl0_o,
  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_tl1_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_tl1_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0]  sm_data_debug_signals_tl1_o,
  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_ts_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_ts_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0]  sm_data_debug_signals_ts_o,
  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_co_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_co_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0]  sm_data_debug_signals_co_o,
  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_col2_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_col2_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0]  sm_data_debug_signals_col2_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_mh_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_mh_o,
  output logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmDataBits-1:0] sm_data_debug_signals_mh_o,
  output logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_dc_o,
  output logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_dc_o,
  output logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0]  sm_data_debug_signals_dc_o,

  output intpipe_dbg_monitor_t                               intpipe_debug_monitor_o
);
  /* verilator lint_on UNUSEDPARAM */

  logic reset_c, reset_w, reset_d;
  logic dft_reset_byp, dft_reset;

  logic                                                 wb_valid;
  logic                                                 wb_thread_id;
  logic [PcSizeExt-1:0]                                 wb_reg_pc_ext;

  logic [NrThreads-1:0]                                 id_core_fe_req_valid;
  minion_fe_req [NrThreads-1:0]                         id_core_fe_req_intpipe;
  fe_req_t [NrThreads-1:0]                              id_core_fe_req_frontend;
  /* verilator lint_off UNOPTFLAT */  // Frontend ready is the original intpipe backpressure return path; the loop reported in full-core lint is cut by stage state in the translated pipeline.
  logic                                                 id_core_fe_resp_ready;
  /* verilator lint_on UNOPTFLAT */
  logic                                                 id_fe_core_resp_valid;
  logic                                                 id_fe_core_resp_thread_id;
  fe_core_resp_t                                        id_fe_core_resp_frontend;
  frontend_core_resp                                    id_fe_core_resp_intpipe;
  logic [NrThreads-1:0]                                 id_core_fe_stall;
  vpu_id_ctrl_t                                         id_frontend_vpu_ctrl;
  minion_control_t                                      id_intpipe_ctrl;

  core_dcache_ctrl                                      s0_core_dcache_ctrl;
  dcache_core_ctrl                                      s0_dcache_core_ctrl_resp;
  minion_dcache_req                                     ex_core_dcache_req;
  logic                                                 ex_core_dcache_gsc;
  logic                                                 id_core_dcache_alloc_rq_pre;
  logic                                                 ex_core_dcache_alloc_rq_val;
  logic                                                 id_core_dcache_gsc;
  logic                                                 id_dcache_core_ready;
  logic                                                 ex_core_dcache_req_valid;
  /* verilator lint_off UNOPTFLAT */  // Core-top kill/replay wires preserve the original intpipe<->dcache feedback path, which Verilator reports as a false integrated cycle.
  logic                                                 tag_core_dcache_kill;
  /* verilator lint_on UNOPTFLAT */
  logic [VpuDataSz-1:0]                                 tag_core_dcache_store_data;
  logic                                                 mem_core_dcache_kill;
  logic [XregSize-1:0]                                  wb_core_dcache_x31;
  logic                                                 mem_dcache_core_resp_int_valid;
  logic                                                 wb_dcache_core_resp_valid;
  dcache_minion_resp                                    wb_dcache_core_resp;
  dcache_minion_scoreboard                              id_dcache_core_scoreboard_dcache;
  dcache_scoreboard_t                                   id_dcache_core_scoreboard_intpipe;
  logic [NrThreads-1:0]                                 id_dcache_core_sb_fp_dealloc;
  logic [NrThreads-1:0]                                 id_dcache_core_sb_int_dealloc;
  dcache_tag_xcpt                                       tag_dcache_core_xcpt;
  logic                                                 tag_dcache_core_replay_next;
  logic                                                 mem_dcache_core_flush_pipeline;
  logic [NrThreads-1:0]                                 id_dcache_core_ordered;
  logic                                                 dcache_idle_excl_mode;
  logic                                                 wb_core_dcache_invalidate_lr;
  logic                                                 wb_dcache_fp_toint;
  logic [NrThreads-1:0]                                 dcache_bus_error;
  logic [NrThreads-1:0][PaSize-1:0]                     dcache_bus_error_addr;
  dcache_core_bp_t                                      tag_dcache_bp;
  logic                                                 tag_dcache_bp_valid;
  logic [DcacheTlErrorBits-3:0]                         tensor_load_err_flags;
  logic [DcacheErrFlagBits-1:0][NrThreads-1:0]          cache_ops_err_flags;
  logic                                                 tensor_reduce_err_flags;
  logic                                                 satp_info_en;
  logic                                                 matp_info_en;
  vm_status_t [NrThreads-1:0]                           vm_status;
  logic                                                 l2_dcache_resp_ready_int;
  logic                                                 l2_dcache_resp_valid_int;
  et_link_minion_rsp_info_t                             l2_dcache_resp_int;
  logic [CsrNrEventsDcache-1:0]                         io_events_dcache;
  logic [CsrNrEventsDcacheVpu-1:0]                      io_events_dcache_vpu;
  dcache_debug_sigs                                     dcache_debug_signals;

  logic [63:0]                                          debug_ffb_wdata;
  logic [MinionDebugFfbWords-1:0]                       debug_ffb_en;
  logic                                                 debug_ffb_thread_sel;
  logic [NrThreads-1:0]                                 debug_ex_program_buffer;
  logic [NrThreads-1:0][63:0]                           read_ddata0;
  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` removes the debug APB slave, leaving the architectural debug CSR writeback nets intentionally unused in the wrapper.
  logic [NrThreads-1:0]                                 update_ddata0;
  logic [63:0]                                          ddata0_wdata;
  /* verilator lint_on UNUSEDSIGNAL */

  /* verilator lint_off UNUSEDSIGNAL */  // Keep the full packed debug-control bundle at the core-top boundary even though Verilator only sees a subset of its bits consumed in this integration wrapper.
  minion_debug_in_t                                     debug_in_core;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                                 reset_c_ff_q;
  logic                                                 reset_w_ff_q;
  logic [NrThreads-1:0]                                 debug_out_have_reset;
  logic [NrThreads-1:0]                                 debug_out_have_reset_q;
  logic [NrThreads-1:0]                                 debug_out_error_q;
  logic [NrThreads-1:0]                                 debug_busy;
  logic [NrThreads-1:0]                                 debug_exception;
  logic [NrThreads-1:0]                                 in_debug_mode;
  logic [NrThreads-1:0]                                 pending_halt_q;
  logic [NrThreads-1:0]                                 pending_halt;

  logic [InstSize-1:0]                                  te_wb_reg_inst;
  /* verilator lint_off UNUSEDSIGNAL */  // Trace staging consumes only the interrupt bit and architectural cause field from the preserved CSR cause struct.
  csr_cause_t                                           te_wb_reg_cause_ie;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                                 te_wb_reg_context;
  logic                                                 te_exception;
  logic [VaSizeExt-1:0]                                 te_tval;
  logic [1:0]                                           te_prv;
  logic [ShireTeRetiresPerCycle-1:0]                    te_instr_valid_reg_q;
  logic [ShireTeRetiresPerCycle-1:0]                    te_exception_reg_q;
  logic [ShireTeRetiresPerCycle-1:0]                    te_interrupt_reg_q;
  logic [(ShireTeRetiresPerCycle * ShireTeInstrWidth)-1:0] te_instr_bus_reg_q;
  logic [(ShireTeRetiresPerCycle * ShireTeInstrAddrWidth)-1:0] te_instr_addr_reg_q;
  logic [(ShireTeRetiresPerCycle * ShireTeContextWidth)-1:0] te_instr_context_reg_q;
  logic [(ShireTeRetiresPerCycle * ShireTeEcauseWidth)-1:0] te_exception_cause_reg_q;
  logic [(ShireTeRetiresPerCycle * ShireTeInstrAddrWidth)-1:0] te_exception_trap_value_reg_q;
  logic [(ShireTeRetiresPerCycle * ShireTePrivilegeWidth)-1:0] te_privilege_level_reg_q;
  logic                                                 te_cpu_halted_reg_q;
  logic [ShireTeRetiresPerCycle-1:0]                    te_instr_valid_reginput;

  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` ties off the debug APB transport, so these registered bus slices are only used in feature-on builds.
  logic [MinRegApbAddrWidth-1:0]                        debug_apb_paddr_q;
  logic                                                 debug_apb_pwrite_q;
  logic                                                 debug_apb_psel_q;
  logic                                                 debug_apb_penable_q;
  logic [BpamShireApbDataWidth-1:0]                     debug_apb_pwdata;
  logic                                                 debug_apb_pready;
  logic [BpamShireApbDataWidth-1:0]                     debug_apb_prdata;
  logic                                                 debug_apb_pslverr;
  /* verilator lint_on UNUSEDSIGNAL */

  /* verilator lint_off UNUSEDSIGNAL */  // `DebugApbEn=0` compiles out the APB debug transport flops and selects, leaving the preserved bus slices intentionally unused in stripped builds.
  logic [DcacheDbgAddrBits-1:0]                         dcache_apb_paddr_q;
  logic                                                 dcache_apb_pwrite_q;
  logic                                                 dcache_apb_psel_q;
  logic                                                 dcache_apb_penable_q;
  logic [BpamShireApbDataWidth-1:0]                     dcache_apb_pwdata;
  logic                                                 dcache_apb_pready;
  logic [BpamShireApbDataWidth-1:0]                     dcache_apb_prdata;
  logic                                                 dcache_apb_pslverr;

  logic                                                 apb_target_dcache;
  logic                                                 apb_target_debug;
  logic                                                 apb_target_debug_reg_q;
  logic [BpamShireApbDataWidth-1:0]                     apb_pwdata_reg_q;
  logic                                                 dcache_apb_en;
  logic                                                 debug_apb_en;
  /* verilator lint_on UNUSEDSIGNAL */

  assign reset_c = !rst_c_ni;
  assign reset_w = !rst_w_ni;
  assign reset_d = !rst_d_ni;
  assign dft_reset_byp = dft_i.scanmode;
  assign dft_reset = !dft_i.scan_reset_n;

  always_comb begin
    for (int i = 0; i < NrThreads; i++) begin
      id_core_fe_req_frontend[i] = fe_req_t'(id_core_fe_req_intpipe[i]);
    end

    id_fe_core_resp_intpipe = frontend_core_resp'(id_fe_core_resp_frontend);
    id_dcache_core_scoreboard_intpipe = dcache_scoreboard_t'(id_dcache_core_scoreboard_dcache);
  end

  always_ff @(posedge clk_i) begin
    reset_c_ff_q <= reset_c;
    reset_w_ff_q <= reset_w;
    debug_out_error_q <= debug_busy & {NrThreads{reset_c || reset_d || reset_w}};
  end

  for (genvar i = 0; i < NrThreads; i++) begin : gen_pending_halts
    logic minion_out_reset;
    logic thread_halt_reset;
    logic thread_halted;
    logic pending_halt_d;

    assign minion_out_reset = (reset_c_ff_q && !reset_c) || (reset_w_ff_q && !reset_w);
    assign thread_halt_reset = minion_out_reset && debug_in_i.resethalt[i];
    assign thread_halted = debug_out_o.halted[i];
    assign pending_halt_d = thread_halt_reset && !thread_halted;
    assign pending_halt[i] = pending_halt_q[i] || pending_halt_d;
    assign debug_out_have_reset[i] = debug_in_core.ackhavereset[i] ? 1'b0 : minion_out_reset;

    /* verilator lint_off SYNCASYNCNET */  // rst_c_ni is the preserved async-assert top reset; this per-thread debug-reset bookkeeping also observes synchronized reset-release pulses, which Verilator flags as mixed sync/async use at the wrapper boundary.
    always_ff @(posedge clk_i or negedge rst_c_ni) begin
      if (!rst_c_ni) begin
        debug_out_have_reset_q[i] <= 1'b0;
        pending_halt_q[i] <= 1'b0;
      end else begin
        if (debug_out_have_reset[i] || debug_in_core.ackhavereset[i])
          debug_out_have_reset_q[i] <= debug_out_have_reset[i];
        if (thread_halt_reset || thread_halted)
          pending_halt_q[i] <= pending_halt_d;
      end
    end
    /* verilator lint_on SYNCASYNCNET */
  end

  always_comb begin
    debug_in_core = debug_in_i;
    debug_in_core.resume = debug_in_i.resume & enabled_i;
    debug_in_core.resethalt = debug_in_i.resethalt & enabled_i;
    debug_in_core.ackhavereset = debug_in_i.ackhavereset & enabled_i;

    for (int i = 0; i < NrThreads; i++) begin
      debug_in_core.halt[i] = (debug_in_i.halt[i] || pending_halt[i]) && enabled_i[i];
    end

    debug_out_o.halted = in_debug_mode & enabled_i;
    debug_out_o.running = (~in_debug_mode) & (~debug_out_have_reset) & enabled_i &
                          {NrThreads{!reset_w && !reset_c}};
    debug_out_o.have_reset = debug_out_have_reset_q;
    debug_out_o.error = debug_out_error_q;
    debug_out_o.exception = debug_exception;
    debug_out_o.busy = debug_busy;
  end

  /* verilator lint_off PINCONNECTEMPTY */  // Core-top intentionally drops the frontend-only debug visibility ports; they are consumed by dedicated bring-up harnesses instead.
  minion_frontend #(
    .VpuEn(VpuEn)
  ) u_frontend (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_w_ni),
    .reset_debug_i          (reset_d),
    .chicken_bit_i          (chicken_bit_frontend_i),
    .dft_i                  (dft_i),
    .f0_thread_enabled_i    (enabled_i),
    .f0_reset_vector_i      (reset_vector_i),
    .vm_status_i            (vm_status),
    .f1_icache_req_ready_i  (icache_req_ready_i),
    .f1_icache_req_valid_o  (icache_req_valid_o),
    .f1_icache_req_o        (icache_req_o),
    .f5_icache_resp_valid_i (icache_resp_valid_i),
    .f5_icache_resp_miss_i  (icache_resp_miss_i),
    .f5_icache_resp_i       (icache_resp_i),
    .f6_icache_fill_done_i  (icache_fill_done_i),
    .f0_core_req_valid_i    (id_core_fe_req_valid),
    .f0_core_req_i          (id_core_fe_req_frontend),
    .id_core_stall_i        (id_core_fe_stall),
    .id_inst_ready_i        (id_core_fe_resp_ready),
    .id_inst_valid_o        (id_fe_core_resp_valid),
    .id_inst_thread_id_o    (id_fe_core_resp_thread_id),
    .id_inst_data_o         (id_fe_core_resp_frontend),
    .id_intpipe_ctrl_o      (id_intpipe_ctrl),
    .id_vpu_decoder_sigs_o  (id_vpu_decoder_sigs_o),
    .id_vpu_core_ctrl_o     (id_frontend_vpu_ctrl),
    .debug_thread_o         (),
    .debug_inst_fifo_empty_o(),
    .debug_inst_fifo_full_o (),
    .debug_tb_inst_valid_o  (),
    .debug_tb_inst_ready_o  (),
    .debug_ffb_wdata_i      (debug_ffb_wdata),
    .debug_ffb_en_i         (debug_ffb_en),
    .debug_ffb_thread_sel_i (debug_ffb_thread_sel),
    .debug_ffb_exec_i       (debug_ex_program_buffer),
    .halt_i                 (debug_in_core.halt),
    .halted_i               (debug_out_o.halted)
  );
  /* verilator lint_on PINCONNECTEMPTY */

  intpipe_top #(
    .VpuEn(VpuEn)
  ) u_intpipe (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_c_ni),
    .rst_w_ni                 (rst_w_ni),
    .rst_d_ni                 (rst_d_ni),
    .te_enable                (te_enable_i),
    .dft__reset_byp           (dft_reset_byp),
    .dft__reset               (dft_reset),
    .interrupts               (interrupts_i),
    .shire_id                 (shire_id_i),
    .shire_min_id             (shire_min_id_i),
    .chicken_bit_intpipe      (chicken_bit_intpipe_i),
    .id_fe_req_valid          (id_core_fe_req_valid),
    .id_fe_req                (id_core_fe_req_intpipe),
    .id_fe_stall              (id_core_fe_stall),
    .id_fe_resp_ready         (id_core_fe_resp_ready),
    .id_fe_resp_valid         (id_fe_core_resp_valid),
    .id_fe_resp_thread_id     (id_fe_core_resp_thread_id),
    .id_fe_resp               (id_fe_core_resp_intpipe),
    .icache_invalidate        (icache_flush_data_o),
    .dcache_ctrl              (s0_core_dcache_ctrl),
    .dcache_ctrl_resp         (s0_dcache_core_ctrl_resp),
    .vpu_ctrl                 (f0_vpu_ctrl_o),
    .id_dcache_alloc_rq_pre   (id_core_dcache_alloc_rq_pre),
    .ex_dcache_alloc_rq_val   (ex_core_dcache_alloc_rq_val),
    .id_dcache_gsc            (id_core_dcache_gsc),
    .id_dcache_ready          (id_dcache_core_ready),
    .ex_dcache_req_valid      (ex_core_dcache_req_valid),
    .ex_dcache_req            (ex_core_dcache_req),
    .ex_dcache_gsc            (ex_core_dcache_gsc),
    .tag_dcache_kill          (tag_core_dcache_kill),
    .tag_dcache_store_data    (tag_core_dcache_store_data),
    .mem_dcache_kill          (mem_core_dcache_kill),
    .wb_dcache_x31            (wb_core_dcache_x31),
    .mem_dcache_resp_int_valid(mem_dcache_core_resp_int_valid),
    .wb_dcache_resp_valid     (wb_dcache_core_resp_valid),
    .wb_dcache_resp           (wb_dcache_core_resp),
    .tag_dcache_bp            (tag_dcache_bp),
    .tag_dcache_bp_valid      (tag_dcache_bp_valid),
    .id_ctrl                  (id_intpipe_ctrl),
    .id_dcache_ordered        (id_dcache_core_ordered),
    .id_dcache_idle           (dcache_idle_excl_mode),
    .id_dcache_scoreboard     (id_dcache_core_scoreboard_intpipe),
    .id_dcache_sb_int_dealloc (id_dcache_core_sb_int_dealloc),
    .id_dcache_sb_fp_dealloc  (id_dcache_core_sb_fp_dealloc),
    .tag_dcache_xcpt          (tag_dcache_core_xcpt),
    .tag_dcache_replay_next   (tag_dcache_core_replay_next),
    .mem_dcache_flush_pipeline(mem_dcache_core_flush_pipeline),
    .wb_dcache_invalidate_lr  (wb_core_dcache_invalidate_lr),
    .wb_dcache_fp_toint       (wb_dcache_fp_toint),
    .dcache_bus_error         (dcache_bus_error),
    .dcache_bus_error_addr    (dcache_bus_error_addr),
    .satp_info                (satp_info_o),
    .matp_info                (matp_info_o),
    .satp_info_en             (satp_info_en),
    .matp_info_en             (matp_info_en),
    .vm_status                (vm_status),
    .tlb_invalidate           (tlb_invalidate_o),
    .l2_resp_valid            (l2_dcache_resp_valid_int),
    .l2_resp_ready            (l2_dcache_resp_ready_int),
    .l2_resp                  (l2_dcache_resp_int),
    .id_vpu_req               (id_vpu_req_o),
    .ex_vpu_req               (ex_vpu_req_o),
    .tag_vpu_kill             (tag_vpu_kill_o),
    .mem_vpu_kill             (mem_vpu_kill_o),
    .wb_vpu_kill              (wb_vpu_kill_o),
    .wb_vpu_dcache_resp_valid (wb_dcache_vpu_resp_valid_o),
    .wb_vpu_dcache_resp       (wb_dcache_vpu_resp_o),
    .thread0_enable           (enabled_i[0]),
    .thread1_enable           (enabled_i[1]),
    .id_frontend_vpu_ctrl     (id_frontend_vpu_ctrl),
    .id_vpu_ctrl              (id_vpu_ctrl_i),
    .ex_vpu_ctrl              (ex_vpu_ctrl_i),
    .tag_vpu_ctrl             (f2_vpu_ctrl_i),
    .mem_vpu_ctrl             (f3_vpu_ctrl_i),
    .wb_vpu_ctrl              (wb_vpu_ctrl_i),
    .vpu_tfma_tenb_working    (vpu_dcache_ctl_i.tfma_enabled),
    .flb_neigh_req_valid      (flb_neigh_req_valid_o),
    .flb_neigh_req_data       (flb_neigh_req_data_o),
    .flb_neigh_resp_valid     (flb_neigh_resp_valid_i),
    .flb_neigh_resp_data      (flb_neigh_resp_data_i),
    .TE_wb_reg_inst           (te_wb_reg_inst),
    .TE_wb_reg_cause_ie       (te_wb_reg_cause_ie),
    .TE_wb_reg_context        (te_wb_reg_context),
    .TE_tval                  (te_tval),
    .TE_prv                   (te_prv),
    .TE_exception             (te_exception),
    .te_thread_sel            (te_thread_sel_i),
    .update_ddata0            (update_ddata0),
    .ddata0_wdata             (ddata0_wdata),
    .read_ddata0              (read_ddata0),
    .debug_ex_program_buffer  (debug_ex_program_buffer),
    .halt                     (debug_in_core.halt),
    .resume                   (debug_in_core.resume),
    .in_debug_mode            (in_debug_mode),
    .debug_busy               (debug_busy),
    .debug_exception          (debug_exception),
    .debug_monitor_out        (intpipe_debug_monitor_o),
    .wb_valid                 (wb_valid),
    .wb_thread_id             (wb_thread_id),
    .wb_reg_pc                (wb_reg_pc_ext),
    .esr_shire_coop_mode      (esr_shire_coop_mode_i),
    .esr_features             (esr_features_i),
    .tensor_load_err_flags    (tensor_load_err_flags),
    .cache_ops_err_flags      (cache_ops_err_flags),
    .tensor_reduce_err_flags  (tensor_reduce_err_flags),
    .pmu_count_up             (pmu_count_up_o),
    .pmu_read_data            (pmu_read_data_i),
    .pmu_read_sel             (pmu_read_sel_o),
    .pmu_write_en             (pmu_write_en_o),
    .pmu_write_data           (pmu_write_data_o),
    .pmu_neigh_event_sel      (pmu_neigh_event_sel_o),
    .io_events_dcache_vpu     (io_events_dcache_vpu),
    .dcache_debug_signals     (dcache_debug_signals)
  );

  assign te_instr_valid_reginput = wb_valid && (wb_thread_id == te_thread_sel_i);

  always_ff @(posedge clk_i or negedge rst_w_ni) begin
    if (!rst_w_ni) begin
      te_instr_valid_reg_q <= '0;
      te_instr_bus_reg_q <= '0;
      te_instr_addr_reg_q <= '0;
      te_instr_context_reg_q <= '0;
      te_exception_reg_q <= '0;
      te_exception_cause_reg_q <= '0;
      te_interrupt_reg_q <= '0;
      te_exception_trap_value_reg_q <= '0;
      te_privilege_level_reg_q <= '0;
      te_cpu_halted_reg_q <= 1'b0;
    end else if (TraceEn && te_enable_i) begin
      te_instr_valid_reg_q <= te_instr_valid_reginput;
      te_instr_bus_reg_q <= te_wb_reg_inst;
      te_instr_addr_reg_q <= wb_reg_pc_ext;
      te_instr_context_reg_q <= te_wb_reg_context;
      te_exception_reg_q <= te_exception && (wb_thread_id == te_thread_sel_i);
      te_exception_cause_reg_q <= te_wb_reg_cause_ie[ShireTeEcauseWidth-1:0];
      te_interrupt_reg_q <= te_wb_reg_cause_ie[63];
      te_exception_trap_value_reg_q <= te_tval;
      te_privilege_level_reg_q <= te_prv;
      te_cpu_halted_reg_q <= debug_out_o.halted[te_thread_sel_i];
    end
  end

  always_comb begin
    trace_encoder_o = '0;
    if (TraceEn) begin
      trace_encoder_o.instr_valid = te_instr_valid_reg_q;
      trace_encoder_o.instr_bus = te_instr_bus_reg_q;
      trace_encoder_o.instr_addr = te_instr_addr_reg_q;
      trace_encoder_o.instr_context = te_instr_context_reg_q;
      trace_encoder_o.exception = te_exception_reg_q;
      trace_encoder_o.exception_cause = te_exception_cause_reg_q;
      trace_encoder_o.interrupt = te_interrupt_reg_q;
      trace_encoder_o.exception_trap_value = te_exception_trap_value_reg_q;
      trace_encoder_o.privilege_level = te_privilege_level_reg_q;
      trace_encoder_o.status_flags = '0;
      trace_encoder_o.cpu_halted = te_cpu_halted_reg_q;
      trace_encoder_o.min_reset = reset_w || reset_c || reset_d;
    end
  end

  assign io_events_dcache_vpu = {io_events_vpu_i, io_events_dcache};

  minion_dcache_top #(
    .DebugApbEn(DebugApbEn),
    .DebugMonEn(DebugMonEn),
    .VpuEn(VpuEn)
  ) u_dcache (
    .clk_i                       (clk_i),
    .rst_ni                      (rst_w_ni),
    .shire_id_i                  (shire_id_i),
    .shire_min_id_i              (shire_min_id_i),
    .ioshire_i                   (ioshire_i),
    .chicken_bit_dcache_i        (chicken_bit_dcache_i),
    .core_ctrl_i                 (s0_core_dcache_ctrl),
    .core_ctrl_resp_o            (s0_dcache_core_ctrl_resp),
    .id_core_alloc_rq_pre_i      (id_core_dcache_alloc_rq_pre),
    .s0_core_alloc_rq_val_i      (ex_core_dcache_alloc_rq_val),
    .id_core_gsc_i               (id_core_dcache_gsc),
    .id_core_ready_o             (id_dcache_core_ready),
    .s0_core_req_valid_i         (ex_core_dcache_req_valid),
    .s0_core_req_i               (ex_core_dcache_req),
    .s0_core_gsc_i               (ex_core_dcache_gsc),
    .s1_core_kill_i              (tag_core_dcache_kill),
    .s1_core_store_data_i        (tag_core_dcache_store_data),
    .s1_mprot_i                  (mprot_i),
    .s2_core_kill_i              (mem_core_dcache_kill),
    .s3_core_x31_i               (wb_core_dcache_x31),
    .s1_vpu_ctrl_i               (vpu_dcache_ctl_i),
    .s3_vpu_scp_resp_o           (dcache_vpu_scp_res_o),
    .s3_vpu_scp_data_o           (dcache_vpu_scp_data_o),
    .s3_vpu_tenb_data_o          (dcache_vpu_tenb_data_o),
    .vpu_reduce_ctrl_o           (vpu_reduce_ctrl_o),
    .s2_core_resp_int_valid_o    (mem_dcache_core_resp_int_valid),
    .s3_core_resp_valid_o        (wb_dcache_core_resp_valid),
    .s3_core_resp_o              (wb_dcache_core_resp),
    .id_core_scoreboard_o        (id_dcache_core_scoreboard_dcache),
    .id_core_sb_fp_dealloc_o     (id_dcache_core_sb_fp_dealloc),
    .id_core_sb_int_dealloc_o    (id_dcache_core_sb_int_dealloc),
    .s1_core_replay_next_o       (tag_dcache_core_replay_next),
    .s1_core_xcpt_o              (tag_dcache_core_xcpt),
    .s2_core_flush_pipeline_o    (mem_dcache_core_flush_pipeline),
    .s3_ordered_o                (id_dcache_core_ordered),
    .s3_invalidate_lr_i          (wb_core_dcache_invalidate_lr),
    .wb_dcache_fp_toint_i        (wb_dcache_fp_toint),
    .vmspagesize_i               (vmspagesize_i),
    .bypass_dcache_i             (esr_bypass_dcache_i),
    .mem_ctrl_override_i         (mem_ctrl_override_i),
    .satp_info_i                 (satp_info_o),
    .matp_info_i                 (matp_info_o),
    .satp_info_en_i              (satp_info_en),
    .matp_info_en_i              (matp_info_en),
    .vm_status_i                 (vm_status),
    .tlb_invalidate_i            (tlb_invalidate_o),
    .ptw_req_data_o              (dc_ptw_req_data_o),
    .ptw_req_valid_o             (dc_ptw_req_valid_o),
    .ptw_req_ready_i             (dc_ptw_req_ready_i),
    .ptw_resp_data_i             (ptw_dc_resp_data_i),
    .ptw_resp_valid_i            (ptw_dc_resp_valid_i),
    .l2_evict_req_ready_i        (l2_dcache_evict_req_ready_i),
    .l2_evict_req_valid_o        (l2_dcache_evict_req_valid_o),
    .l2_evict_req_o              (l2_dcache_evict_req_o),
    .l2_miss_req_ready_i         (l2_dcache_miss_req_ready_i),
    .l2_miss_req_valid_o         (l2_dcache_miss_req_valid_o),
    .l2_miss_req_o               (l2_dcache_miss_req_o),
    .l2_resp_ready_o             (l2_dcache_resp_ready_o),
    .l2_resp_valid_i             (l2_dcache_resp_valid_i),
    .l2_resp_i                   (l2_dcache_resp_i),
    .l2_resp_ready_int_o         (l2_dcache_resp_ready_int),
    .l2_resp_valid_int_o         (l2_dcache_resp_valid_int),
    .l2_resp_int_o               (l2_dcache_resp_int),
    .s1_bp_conf_o                (tag_dcache_bp),
    .s1_bp_conf_valid_o          (tag_dcache_bp_valid),
    .dcache_idle_excl_mode_o     (dcache_idle_excl_mode),
    .tensor_load_err_flags_o     (tensor_load_err_flags),
    .cache_ops_err_flags_o       (cache_ops_err_flags),
    .tensor_reduce_err_flags_o   (tensor_reduce_err_flags),
    .apb_paddr_i                 (dcache_apb_paddr_q),
    .apb_pwrite_i                (dcache_apb_pwrite_q),
    .apb_psel_i                  (dcache_apb_psel_q),
    .apb_penable_i               (dcache_apb_penable_q),
    .apb_pwdata_i                (dcache_apb_pwdata),
    .apb_pready_o                (dcache_apb_pready),
    .apb_prdata_o                (dcache_apb_prdata),
    .apb_pslverr_o               (dcache_apb_pslverr),
    .io_events_o                 (io_events_dcache),
    .bus_err_o                   (dcache_bus_error),
    .bus_err_addr_o              (dcache_bus_error_addr),
    .csr_debug_sigs_o            (dcache_debug_signals),
    .sm_match_debug_signals_tl0_o(sm_match_debug_signals_tl0_o),
    .sm_filter_debug_signals_tl0_o(sm_filter_debug_signals_tl0_o),
    .sm_data_debug_signals_tl0_o (sm_data_debug_signals_tl0_o),
    .sm_match_debug_signals_tl1_o(sm_match_debug_signals_tl1_o),
    .sm_filter_debug_signals_tl1_o(sm_filter_debug_signals_tl1_o),
    .sm_data_debug_signals_tl1_o (sm_data_debug_signals_tl1_o),
    .sm_match_debug_signals_ts_o (sm_match_debug_signals_ts_o),
    .sm_filter_debug_signals_ts_o(sm_filter_debug_signals_ts_o),
    .sm_data_debug_signals_ts_o  (sm_data_debug_signals_ts_o),
    .sm_match_debug_signals_co_o (sm_match_debug_signals_co_o),
    .sm_filter_debug_signals_co_o(sm_filter_debug_signals_co_o),
    .sm_data_debug_signals_co_o  (sm_data_debug_signals_co_o),
    .sm_match_debug_signals_col2_o(sm_match_debug_signals_col2_o),
    .sm_filter_debug_signals_col2_o(sm_filter_debug_signals_col2_o),
    .sm_data_debug_signals_col2_o(sm_data_debug_signals_col2_o),
    .sm_match_debug_signals_mh_o (sm_match_debug_signals_mh_o),
    .sm_filter_debug_signals_mh_o(sm_filter_debug_signals_mh_o),
    .sm_data_debug_signals_mh_o  (sm_data_debug_signals_mh_o),
    .sm_match_debug_signals_dc_o (sm_match_debug_signals_dc_o),
    .sm_filter_debug_signals_dc_o(sm_filter_debug_signals_dc_o),
    .sm_data_debug_signals_dc_o  (sm_data_debug_signals_dc_o)
  );

  if (DebugApbEn) begin : gen_debug_apb_transport
    assign apb_target_dcache =
        apb_paddr_i[MinDmApbAddrWidth-1 -: MinionDebugApbAddrUpperBits] == MinionDebugApbAddrDcache;
    assign apb_target_debug =
        apb_paddr_i[MinDmApbAddrWidth-1 -: MinionDebugApbAddrUpperBits] == MinionDebugApbAddrDebug;
    assign dcache_apb_en = apb_psel_i || dcache_apb_psel_q;
    assign debug_apb_en = apb_psel_i || debug_apb_psel_q;
    assign dcache_apb_pwdata = apb_pwdata_reg_q;
    assign debug_apb_pwdata = apb_pwdata_reg_q;

    always_ff @(posedge clk_i or negedge rst_w_ni) begin
      if (!rst_w_ni) begin
        dcache_apb_psel_q <= 1'b0;
        dcache_apb_penable_q <= 1'b0;
        debug_apb_psel_q <= 1'b0;
        debug_apb_penable_q <= 1'b0;
      end else begin
        if (dcache_apb_en) begin
          dcache_apb_psel_q <= apb_psel_i && apb_target_dcache;
          dcache_apb_penable_q <= apb_penable_i && apb_target_dcache;
        end
        if (debug_apb_en) begin
          debug_apb_psel_q <= apb_psel_i && apb_target_debug;
          debug_apb_penable_q <= apb_penable_i && apb_target_debug;
        end
      end
    end

    always_ff @(posedge clk_i) begin
      if (dcache_apb_en) begin
        dcache_apb_pwrite_q <= apb_pwrite_i;
        dcache_apb_paddr_q <= apb_paddr_i[DcacheDbgAddrBits-1:0];
      end

      if (debug_apb_en) begin
        debug_apb_pwrite_q <= apb_pwrite_i;
        debug_apb_paddr_q <= apb_paddr_i[MinRegApbAddrWidth-1:0];
        apb_target_debug_reg_q <= apb_target_debug;
      end

      if (apb_psel_i)
        apb_pwdata_reg_q <= apb_pwdata_i;
    end

    assign apb_pready_o = apb_target_debug_reg_q ? debug_apb_pready : dcache_apb_pready;
    assign apb_prdata_o = apb_target_debug_reg_q ? debug_apb_prdata : dcache_apb_prdata;
    assign apb_pslverr_o = apb_target_debug_reg_q ? debug_apb_pslverr : dcache_apb_pslverr;

    minion_debug_apb_slv u_debug_apb_slv (
      .clk_i               (clk_i),
      .rst_ni              (rst_d_ni),
      .apb_paddr_i         (debug_apb_paddr_q),
      .apb_pwrite_i        (debug_apb_pwrite_q),
      .apb_psel_i          (debug_apb_psel_q),
      .apb_penable_i       (debug_apb_penable_q),
      .apb_pwdata_i        (debug_apb_pwdata),
      .apb_pready_o        (debug_apb_pready),
      .apb_prdata_o        (debug_apb_prdata),
      .apb_pslverr_o       (debug_apb_pslverr),
      .csr_wdata_i         (ddata0_wdata),
      .update_ddata0_i     (update_ddata0),
      .read_ddata0_o       (read_ddata0),
      .ffb_wdata_o         (debug_ffb_wdata),
      .ffb_en_o            (debug_ffb_en),
      .ffb_thread_sel_o    (debug_ffb_thread_sel),
      .ex_program_buffer_o (debug_ex_program_buffer),
      .in_debug_mode_i     (debug_out_o.halted)
    );
  end else begin : gen_no_debug_apb_transport
    assign apb_target_dcache = 1'b0;
    assign apb_target_debug = 1'b0;
    assign dcache_apb_en = 1'b0;
    assign debug_apb_en = 1'b0;
    assign dcache_apb_pwdata = '0;
    assign debug_apb_pwdata = '0;
    assign dcache_apb_paddr_q = '0;
    assign dcache_apb_pwrite_q = 1'b0;
    assign dcache_apb_psel_q = 1'b0;
    assign dcache_apb_penable_q = 1'b0;
    assign debug_apb_paddr_q = '0;
    assign debug_apb_pwrite_q = 1'b0;
    assign debug_apb_psel_q = 1'b0;
    assign debug_apb_penable_q = 1'b0;
    assign debug_apb_pready = 1'b0;
    assign debug_apb_prdata = '0;
    assign debug_apb_pslverr = 1'b0;
    assign apb_target_debug_reg_q = 1'b0;
    assign apb_pwdata_reg_q = '0;
    assign apb_pready_o = apb_psel_i && apb_penable_i;
    assign apb_prdata_o = '0;
    assign apb_pslverr_o = apb_psel_i && apb_penable_i;
    assign read_ddata0 = '0;
    assign debug_ffb_wdata = '0;
    assign debug_ffb_en = '0;
    assign debug_ffb_thread_sel = 1'b0;
    assign debug_ex_program_buffer = '0;
  end

endmodule
