// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module core_top (
  // System signals
  input  logic                                          clock,

  input  logic                                          reset_c,
  input  logic                                          reset_w,
  input  logic                                          reset_d,

  // DFT signals
  input  logic                                          dft__reset_byp,     //Reset byp
  input  logic                                          dft__reset,         //DFT mode reset

  // Is this in the IOShire
  input  logic                                          ioshire,
  // Chicken bit clock gate enable
  input  logic                                          chicken_bit_dcache,
  input  logic                                          chicken_bit_intpipe,
  input  logic                                          chicken_bit_frontend,
  // DCache request to L2 evict port
  input  logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]         l2_dcache_evict_req_ready,
  output logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]         l2_dcache_evict_req_valid,
  output et_link_minion_evict_req_info_t                l2_dcache_evict_req,
  // DCache request to L2 miss port
  input  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]          l2_dcache_miss_req_ready,
  output logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]          l2_dcache_miss_req_valid,
  output et_link_minion_miss_req_info_t                 l2_dcache_miss_req,
  // L2 response to DCache
  output logic                                          l2_dcache_resp_ready,
  input  logic                                          l2_dcache_resp_valid,
  input  et_link_minion_rsp_info_t                      l2_dcache_resp,
  // ICache requests
  input  logic                                          icache_req_ready,
  output logic                                          icache_req_valid,
  output frontend_icache_req                            icache_req,
  // ICache response
  input  logic                                          icache_resp_valid,
  input  logic                                          icache_resp_miss,
  input  icache_frontend_resp                           icache_resp,
  input  logic                                          icache_fill_done,
  // ICache control
  output logic                                          icache_flush_data,
  // TLB/PTW control
  output minion_satp_info                               satp_info,
  output minion_satp_info                               matp_info,
  output logic                                          tlb_invalidate,
  // PTW request
  output minion_ptw_req                                 dc_ptw_req_data,
  output logic                                          dc_ptw_req_valid,
  input  logic                                          dc_ptw_req_ready,
  input  minion_ptw_pte                                 ptw_dc_resp_data,
  input  logic                                          ptw_dc_resp_valid,
  // Interrupts
  input  minion_interrupts                              interrupts,
  // Thread ID
  input  logic [`NUM_SHIRE_IDS_R]                       shire_id,
  input  logic [`MIN_PER_S_R]                           shire_min_id,
  // Reset PC
  input  logic [`CORE_NR_THREADS-1:0]                   enabled,
  input  logic [`VA_RANGE]                              reset_vector,
  // PMA control
  input  esr_mprot_t                                    mprot,
  // Minimum page size ESR
  input  tlb_entry_type                                 vmspagesize,
  // Control signals going to VPU
  output minion_vpu_id_req                              id_vpu_req,
  output vpu_ctrl_sigs_t                                id_vpu_decoder_sigs,
  output minion_vpu_ex_req                              ex_vpu_req,
  output logic                                          tag_vpu_kill,
  output logic                                          mem_vpu_kill,
  output logic                                          wb_vpu_kill,
  output core_vpu_ctrl                                  f0_vpu_ctrl,
  // WB Dcache response to VPU
  output logic                                          wb_dcache_vpu_resp_valid,
  output dcache_vpu_resp                                wb_dcache_vpu_resp,
  // DCache SRAM direct port for VPU
  input  vpu_dcache_ctrl                                vpu_dcache_ctl,
  output dcache_vpu_scp_resp                            dcache_vpu_scp_res,
  output logic [`DCACHE_DATA_RANGE]                     dcache_vpu_scp_data,
  output logic [`DCACHE_DATA_RANGE]                     dcache_vpu_tenb_data,
  // VPU reduce control
  output dcache_vpu_reduce_ctrl                         vpu_reduce_ctrl,
  // Control signals from VPU
  input  vpu_minion_id_ctrl                             id_vpu_ctrl,
  input  vpu_minion_ex_ctrl                             ex_vpu_ctrl,
  input  vpu_minion_tag_ctrl                            f2_vpu_ctrl,
  input  vpu_minion_mem_ctrl                            f3_vpu_ctrl,
  input  vpu_minion_wb_ctrl                             wb_vpu_ctrl,
  // Fast Local Barrier request interface with the neigh
  output logic                                          flb_neigh_req_valid,
  output logic [`CSR_FL_BARRIER_RANGE]                  flb_neigh_req_data,
  // Fast Local Barrier response interface with the neigh
  input  logic                                          flb_neigh_resp_valid,
  input  logic                                          flb_neigh_resp_data,
  //Trace Encoder signals
  input  logic                                          te_thread_sel,
  output TraceEncoder_signals                           traceEncoder,
  input  logic                                          te_enable,
  // APB signals from BPAM for debug
  input  logic [`MIN_DM_APB_ADDR_WIDTH-1:0]             apb_paddr,
  input  logic                                          apb_pwrite,
  input  logic                                          apb_psel,
  input  logic                                          apb_penable,
  input  logic [`ESR_APB_D_WIDTH-1:0]                   apb_pwdata,
  output logic                                          apb_pready,
  output logic [`ESR_APB_D_WIDTH-1:0]                   apb_prdata,
  output logic                                          apb_pslverr,
  // Debug signals
  input  minion_debug_in_t                              debug_in,
  output minion_debug_out_t                             debug_out,
  // ESR configuration
  input  esr_minion_features_t                          esr_features,
  input  logic                                          esr_bypass_dcache,
  input  logic                                          esr_shire_coop_mode,
  // Memory control override
  input  logic [`MINION_MEM_CTRL_BITS-1:0]              mem_ctrl_override,
  // PMU signals
  output logic [`PMU_MINION_COUNTERS_RANGE]                       pmu_count_up,
  input  logic [`CORE_NR_THREADS-1:0][`XREG_RANGE]                pmu_read_data,
  output logic [`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE] pmu_read_sel,
  output logic [`PMU_TOTAL_COUNTERS_RANGE]                        pmu_write_en,
  output logic [`XREG_RANGE]                                      pmu_write_data,
  output logic [`PMU_NEIGH_EVENT_CNT_SEL_RANGE]                   pmu_neigh_event_sel,
  // Events for PMU counters
  input  logic [`CSR_NR_EVENTS_VPU-1:0]                 io_events_vpu,
  // Signals for Debug Status Monitor
  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_tl0,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_tl0,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_tl0,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_tl1,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_tl1,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_tl1,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_ts,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_ts,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_ts,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_co,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_co,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_co,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_col2,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_col2,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_col2,

  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_MATCH_BITS-1:0]  sm_match_debug_signals_mh,
  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_FILTER_BITS-1:0] sm_filter_debug_signals_mh,
  output logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_DATA_BITS-1:0]   sm_data_debug_signals_mh,

  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_dc,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_dc,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_dc,

  output intpipe_dbg_monitor_t                          intpipe_debug_monitor_out
);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Front End module. It fetches one instruction per cycle and sends it to the
// core. The core can redirect the PC in case of mispredict or recycle.
// The front end has a branch predictor to predict branch targets and also an
// icache to reduce accesses to the L2.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
logic                                     wb_valid;
logic                                     wb_thread_id;
logic [`CORE_NR_THREADS-1:0]              id_core_fe_req_valid;
minion_fe_req [`CORE_NR_THREADS-1:0]      id_core_fe_req;
logic                                     id_core_fe_resp_ready;
logic                                     id_fe_core_resp_valid;
logic                                     id_fe_core_resp_thread_id;
frontend_core_resp                        id_fe_core_resp;
logic [`CORE_NR_THREADS-1:0]              id_core_fe_stall;
core_dcache_ctrl                          s0_core_dcache_ctrl;
dcache_core_ctrl                          s0_dcache_core_ctrl_resp;
minion_vm_status [`CORE_NR_THREADS-1:0]   vm_status;
logic [`MINION_DEBUG_APB_D_WIDTH-1:0]     debug_ffb_wdata;
logic [`MINION_DEBUG_FFB_WORDS-1:0]       debug_ffb_en;
logic                                     debug_ffb_thread_sel;
logic [`CORE_NR_THREADS-1:0]              debug_ex_program_buffer;
vpu_minion_id_ctrl                        id_frontend_vpu_ctrl;
minion_control                            id_intpipe_ctrl;
minion_debug_in_t                         debug_in_core;

frontend_top frontend (
  // System signals
  .clock                (clock),
  .reset                (reset_w),
  .reset_debug          (reset_d),
  .chicken_bit          (chicken_bit_frontend),
  // Enable and Reset PC
  .f0_thread_enabled    (enabled),
  .f0_reset_vector      (reset_vector),
  // TLB control
  .vm_status            (vm_status),
  // Icache requests
  .f1_icache_req_ready  (icache_req_ready),
  .f1_icache_req_valid  (icache_req_valid),
  .f1_icache_req        (icache_req),
  // Icache response
  .f5_icache_resp_valid (icache_resp_valid),
  .f5_icache_resp_miss  (icache_resp_miss),
  .f5_icache_resp       (icache_resp),
  .f6_icache_fill_done  (icache_fill_done),
  // Mispredict from core
  .f0_core_req_valid    (id_core_fe_req_valid),
  .f0_core_req          (id_core_fe_req),
  .id_core_stall        (id_core_fe_stall),
  // Fetch resp
  .id_inst_ready        (id_core_fe_resp_ready),
  .id_inst_valid        (id_fe_core_resp_valid),
  .id_inst_thread_id    (id_fe_core_resp_thread_id),
  .id_inst_data         (id_fe_core_resp),
  // intpipe decode control
  .id_intpipe_ctrl      (id_intpipe_ctrl),
  // VPU decode signals
  .id_vpu_decoder_sigs  (id_vpu_decoder_sigs),
  .id_vpu_core_ctrl     (id_frontend_vpu_ctrl),
  // interface to write program buffer
  .debug_ffb_wdata      (debug_ffb_wdata),
  .debug_ffb_en         (debug_ffb_en),
  .debug_ffb_thread_sel (debug_ffb_thread_sel),
  .debug_ffb_exec       (debug_ex_program_buffer),
  .halt                 (debug_in_core.halt),
  .halted               (debug_out.halted)
);

  
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Core module. It controls the execution of the instruction, sends requests
// to the other units (dcache, frontend and VPU) and executes the integer
// operations.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

dcache_tag_xcpt              tag_dcache_core_xcpt;
logic                        mem_dcache_core_flush_pipeline;
minion_dcache_req            ex_core_dcache_req;
logic                        ex_core_dcache_gsc;
dcache_minion_resp           wb_dcache_core_resp;
dcache_minion_scoreboard     id_dcache_core_scoreboard;
logic [`CORE_NR_THREADS-1:0] id_dcache_core_sb_fp_dealloc;
logic [`CORE_NR_THREADS-1:0] id_dcache_core_sb_int_dealloc;
logic [`VPU_DATA_RANGE]      tag_core_dcache_store_data;
logic [`XREG_RANGE]          wb_core_dcache_x31;
logic [`CORE_NR_THREADS-1:0] id_dcache_core_ordered;
logic                        dcache_idle_excl_mode;
logic                        id_core_dcache_alloc_rq_pre;
logic                        ex_core_dcache_alloc_rq_val;
logic                        id_core_dcache_gsc;
logic                        id_dcache_core_ready;
logic                        ex_core_dcache_req_valid;
logic                        tag_core_dcache_kill;
logic                        mem_core_dcache_kill;
logic                        mem_dcache_core_resp_int_valid;
logic                        wb_dcache_core_resp_valid;
logic                        tag_dcache_core_replay_next;
logic                        wb_core_dcache_invalidate_lr;
logic                        wb_dcache_fp_toint;
logic [`CORE_NR_THREADS-1:0] dcache_bus_error;
logic [`CORE_NR_THREADS-1:0]
      [`PA_RANGE]            dcache_bus_error_addr;
`ifdef DCACHE_REPORT_PC
logic [`CORE_NR_THREADS-1:0]
      [`PC_RANGE_EXT]        dcache_bus_error_pc;
`endif
logic                        satp_info_en;
logic                        matp_info_en;
logic                        TE_exception;
dcache_core_bp_t             tag_dcache_bp; // data from dcache for breakpoing unit
logic                        tag_dcache_bp_valid;

logic [`DCACHE_TL_ERROR_BITS-3:0]                    tensor_load_err_flags;
logic [`DCACHE_ERR_FLAG_RANGE][`CORE_NR_THREADS-1:0] cache_ops_err_flags;
logic                                                tensor_reduce_err_flags;

logic [`INST_RANGE]                     TE_wb_reg_inst;
csr_cause_t                             TE_wb_reg_cause_ie;
logic                                   TE_wb_reg_context;
logic [`shire_te_instr_addr_width-1:0]  TE_tval;
logic [1:0]                             TE_prv;

logic [`CORE_NR_THREADS-1:0]        update_ddata0;
logic [63:0]                        ddata0_wdata;
logic [`CORE_NR_THREADS-1:0][63:0]  read_ddata0;

logic [`shire_te_retires_per_cycle - 1:0]                                       te_instr_valid_reg;
logic [`shire_te_retires_per_cycle - 1:0]                                       te_instr_valid_reginput;
logic [(`shire_te_retires_per_cycle * `shire_te_instr_width)- 1:0]              te_instr_bus_reg;
logic [(`shire_te_retires_per_cycle * `shire_te_instr_addr_width) - 1:0]        te_instr_addr_reg;
logic [(`shire_te_retires_per_cycle * `shire_te_context_width) - 1:0]           te_instr_context_reg;
logic [`shire_te_retires_per_cycle - 1:0]                                       te_exception_reg;
logic [(`shire_te_retires_per_cycle * `shire_te_ecause_width) - 1:0]            te_exception_cause_reg;
logic [`shire_te_retires_per_cycle - 1:0]                                       te_interrupt_reg;
logic [(`shire_te_retires_per_cycle * `shire_te_instr_addr_width) - 1:0]        te_exception_trap_value_reg;
logic [(`shire_te_retires_per_cycle * `shire_te_privilege_width) - 1:0]         te_privilege_level_reg;
//logic [`shire_te_retires_per_cycle - 1:0]                                       te_fetch_valid_reg;
//logic [`clog2min1(`shire_te_fetch_depth + 1) - 1:0]                             te_instr_retract_count_reg;
//logic [`shire_te_user_width - 1:0]                                              te_user_def_filter_qual_reg;
logic                                                                           te_cpu_halted_reg;

logic [`MIN_REG_APB_ADDR_WIDTH-1:0]    debug_apb_paddr;
logic [`MIN_REG_APB_ADDR_WIDTH-1:0]    debug_apb_paddr_next;
logic                                  debug_apb_pwrite;
logic                                  debug_apb_pwrite_next;
logic                                  debug_apb_psel;
logic                                  debug_apb_psel_next;
logic                                  debug_apb_penable;
logic                                  debug_apb_penable_next;
logic [`MINION_DEBUG_APB_D_WIDTH-1:0]  debug_apb_pwdata;

logic                                  debug_apb_pready;
logic [`MINION_DEBUG_APB_D_WIDTH-1:0]  debug_apb_prdata;
logic                                  debug_apb_pslverr;

logic [`DCACHE_DBG_ADDR_RANGE]         dcache_apb_paddr;
logic [`DCACHE_DBG_ADDR_RANGE]         dcache_apb_paddr_next;
logic                                  dcache_apb_pwrite;
logic                                  dcache_apb_pwrite_next;
logic                                  dcache_apb_psel;
logic                                  dcache_apb_psel_next;
logic                                  dcache_apb_penable;
logic                                  dcache_apb_penable_next;
logic [`ESR_APB_D_WIDTH-1:0]           dcache_apb_pwdata;

logic                                  dcache_apb_pready;
logic [`ESR_APB_D_WIDTH-1:0]           dcache_apb_prdata;
logic                                  dcache_apb_pslverr;

logic [`CSR_NR_EVENTS_DCACHE_VPU-1:0]  io_events_dcache_vpu;
logic [`CSR_NR_EVENTS_DCACHE-1:0]      io_events_dcache;

dcache_debug_sigs   dcache_debug_signals;
logic [`PC_RANGE_EXT]                  wb_reg_pc_ext;

logic                                  l2_dcache_resp_ready_int;
logic                                  l2_dcache_resp_valid_int;
et_link_minion_rsp_info_t              l2_dcache_resp_int;


//////////////////////
// DEBUG RUNCONTROL
/////////////////////
logic                               reset_c_ff, reset_w_ff;
logic [`CORE_NR_THREADS-1:0]        debug_out_have_reset,  debug_out_have_reset_ff;
logic [`CORE_NR_THREADS-1:0]        debug_out_error;
logic [`CORE_NR_THREADS-1:0]        debug_busy;
logic [`CORE_NR_THREADS-1:0]        debug_exception;
logic [`CORE_NR_THREADS-1:0]        in_debug_mode;
logic [`CORE_NR_THREADS-1:0]        pending_halt;

`FF(clock, reset_c_ff, reset_c)
`FF(clock, reset_w_ff, reset_w)

`FF(clock, debug_out_error, {debug_busy[1] & (reset_c | reset_d | reset_w),
                             debug_busy[0] & (reset_c | reset_d | reset_w)})

for (genvar gen_it = 0; gen_it < `CORE_NR_THREADS; gen_it++) begin : PENDING_HALTS
  logic thread_halt_reset;
  logic thread_halted;
  logic pending_halt_reg, pending_halt_reg_next;
  logic minion_out_reset;

  assign minion_out_reset = (reset_c_ff && !reset_c) || (reset_w_ff && !reset_w);
  assign thread_halt_reset = minion_out_reset && debug_in.resethalt[gen_it]; // Thread needs to halt
  assign thread_halted = debug_out.halted[gen_it];
  assign pending_halt_reg_next = thread_halt_reset && !thread_halted;            // Halt if not already halted

  assign debug_out_have_reset[gen_it] = debug_in_core.ackhavereset[gen_it] ? 1'b0 : minion_out_reset;

  // Update debug_have_reset when we get out of reset or when we receive an ackhavereset
  //          CLK  | RST    |      EN                                                        |  REG                            |   NEXT                     | RST_ VAL |
  `RST_EN_FF(clock, reset_c, debug_out_have_reset[gen_it] | debug_in_core.ackhavereset[gen_it], debug_out_have_reset_ff[gen_it], debug_out_have_reset[gen_it], 1'b0)
  `RST_EN_FF(clock, reset_c, thread_halt_reset | thread_halted, pending_halt_reg, pending_halt_reg_next, '0)

  assign pending_halt[gen_it] = pending_halt_reg || pending_halt_reg_next;

end



always_comb begin
  debug_out.halted     = in_debug_mode & enabled;
  debug_out.running    = ~in_debug_mode & ~debug_out_have_reset & enabled & {!reset_w && !reset_c, !reset_w && !reset_c};
  debug_out.have_reset = debug_out_have_reset_ff;
  debug_out.error      = debug_out_error;
  debug_out.exception  = debug_exception;
  debug_out.busy       = debug_busy;
end

always_comb begin
  debug_in_core                = debug_in;
  debug_in_core.resume         &= enabled;
  debug_in_core.resethalt      &= enabled;
  debug_in_core.ackhavereset   &= enabled;

  for (int i = 0; i < `CORE_NR_THREADS; ++i) begin
    debug_in_core.halt[i] = (debug_in.halt[i] || pending_halt[i]) && enabled[i];
  end
end


intpipe_top intpipe (
  // System signals
  .clock                        (clock),
  .reset_c                      (reset_c),
  .reset_w                      (reset_w),
  .reset_d                      (reset_d),
  // DFT signals
  .dft__reset_byp               (dft__reset_byp),
  .dft__reset                   (dft__reset),
  // Interrupts
  .interrupts                   (interrupts),
  // Thread id
  .shire_id                     (shire_id),
  .shire_min_id                 (shire_min_id),
  .chicken_bit_intpipe          (chicken_bit_intpipe),
  // Request to the front end
  .id_fe_req_valid              (id_core_fe_req_valid),
  .id_fe_req                    (id_core_fe_req),
  .id_fe_stall                  (id_core_fe_stall),
  // Front end response
  .id_fe_resp_ready             (id_core_fe_resp_ready),
  .id_fe_resp_valid             (id_fe_core_resp_valid),
  .id_fe_resp_thread_id         (id_fe_core_resp_thread_id),
  .id_fe_resp                   (id_fe_core_resp),
  // inpipe decode control coming from frontend
  .id_ctrl                      (id_intpipe_ctrl),
  // Icache flush
  .icache_invalidate            (icache_flush_data),
  // FrontEnd, Dcache and VPU control
  .dcache_ctrl                  (s0_core_dcache_ctrl),
  .dcache_ctrl_resp             (s0_dcache_core_ctrl_resp),
  .vpu_ctrl                     (f0_vpu_ctrl),
  // Request to the dcache
  .id_dcache_alloc_rq_pre       (id_core_dcache_alloc_rq_pre),
  .ex_dcache_alloc_rq_val       (ex_core_dcache_alloc_rq_val),
  .id_dcache_gsc                (id_core_dcache_gsc),
  .id_dcache_ready              (id_dcache_core_ready),
  .ex_dcache_req_valid          (ex_core_dcache_req_valid),
  .ex_dcache_req                (ex_core_dcache_req),
  .ex_dcache_gsc                (ex_core_dcache_gsc),
  .tag_dcache_kill              (tag_core_dcache_kill),
  .tag_dcache_store_data        (tag_core_dcache_store_data),
  .mem_dcache_kill              (mem_core_dcache_kill),
  .wb_dcache_x31                (wb_core_dcache_x31),
  // Dcache response
  .mem_dcache_resp_int_valid    (mem_dcache_core_resp_int_valid),
  .wb_dcache_resp_valid         (wb_dcache_core_resp_valid),
  .wb_dcache_resp               (wb_dcache_core_resp),
  .tag_dcache_bp                (tag_dcache_bp),
  .tag_dcache_bp_valid          (tag_dcache_bp_valid),
  // Dcache control
  .id_dcache_ordered            (id_dcache_core_ordered),
  .id_dcache_idle               (dcache_idle_excl_mode),
  .id_dcache_scoreboard         (id_dcache_core_scoreboard),
  .id_dcache_sb_fp_dealloc      (id_dcache_core_sb_fp_dealloc),
  .id_dcache_sb_int_dealloc     (id_dcache_core_sb_int_dealloc),
  .tag_dcache_xcpt              (tag_dcache_core_xcpt),
  .tag_dcache_replay_next       (tag_dcache_core_replay_next),
  .mem_dcache_flush_pipeline    (mem_dcache_core_flush_pipeline),
  .wb_dcache_invalidate_lr      (wb_core_dcache_invalidate_lr),
  .wb_dcache_fp_toint           (wb_dcache_fp_toint),
  .dcache_bus_error             (dcache_bus_error),
  .dcache_bus_error_addr        (dcache_bus_error_addr),
  `ifdef DCACHE_REPORT_PC
  .dcache_bus_error_pc          (dcache_bus_error_pc),
  `endif
  // TLB/PTW control
  .satp_info                    (satp_info),
  .satp_info_en                 (satp_info_en),
  .matp_info                    (matp_info),
  .matp_info_en                 (matp_info_en),
  .vm_status                    (vm_status),
  .tlb_invalidate               (tlb_invalidate),
  // L2 fills (used for RBOX)
  .l2_resp_valid                (l2_dcache_resp_valid_int),
  .l2_resp_ready                (l2_dcache_resp_ready_int),
  .l2_resp                      (l2_dcache_resp_int),
  // Control signals going to VPU
  .id_vpu_req                   (id_vpu_req),
  .ex_vpu_req                   (ex_vpu_req),
  .tag_vpu_kill                 (tag_vpu_kill),
  .mem_vpu_kill                 (mem_vpu_kill),
  .wb_vpu_kill                  (wb_vpu_kill),
  // Dcache response to VPU
  .wb_vpu_dcache_resp_valid     (wb_dcache_vpu_resp_valid),
  .wb_vpu_dcache_resp           (wb_dcache_vpu_resp),
  // enable for thread1
  .thread0_enable               (enabled[0]),
  .thread1_enable               (enabled[1]),
  // Control signals coming from frontend
  .id_frontend_vpu_ctrl         (id_frontend_vpu_ctrl),
  // Control signals coming from VPU
  .id_vpu_ctrl                  (id_vpu_ctrl),
  .ex_vpu_ctrl                  (ex_vpu_ctrl),
  .tag_vpu_ctrl                 (f2_vpu_ctrl),
  .mem_vpu_ctrl                 (f3_vpu_ctrl),
  .wb_vpu_ctrl                  (wb_vpu_ctrl),
  // Control signals from VPU to pair with TenB operations in Dcache
  .vpu_tfma_tenb_working        (vpu_dcache_ctl.tfma_enabled),
  // Fast Local Barrier request interface with the neigh
  .flb_neigh_req_valid          (flb_neigh_req_valid),
  .flb_neigh_req_data           (flb_neigh_req_data),
  // Fast Local Barrier response interface with the neigh
  .flb_neigh_resp_valid         (flb_neigh_resp_valid),
  .flb_neigh_resp_data          (flb_neigh_resp_data),
  .te_enable                    (te_enable),
  .TE_wb_reg_inst               (TE_wb_reg_inst),
  .TE_wb_reg_cause_ie           (TE_wb_reg_cause_ie),
  .TE_wb_reg_context            (TE_wb_reg_context),
  .TE_exception                 (TE_exception),
  .TE_tval                      (TE_tval),
  .TE_prv                       (TE_prv),
  .te_thread_sel                (te_thread_sel),
  // signals for debug
  .update_ddata0                (update_ddata0),
  .ddata0_wdata                 (ddata0_wdata),
  .read_ddata0                  (read_ddata0),
  .debug_ex_program_buffer      (debug_ex_program_buffer),
  .halt                         (debug_in_core.halt),
  .resume                       (debug_in_core.resume),
  .in_debug_mode                (in_debug_mode),
  .debug_busy                   (debug_busy),
  .debug_exception              (debug_exception),
  .debug_monitor_out            (intpipe_debug_monitor_out),
  // Monitor signals for netlist debug
  .wb_valid                     (wb_valid),
  .wb_thread_id                 (wb_thread_id),
  .wb_reg_pc                    (wb_reg_pc_ext),
  // ESR configuration
  .esr_features                 (esr_features),
  .esr_shire_coop_mode          (esr_shire_coop_mode),
  // Error bits
  .tensor_load_err_flags        (tensor_load_err_flags),
  .cache_ops_err_flags          (cache_ops_err_flags),
  .tensor_reduce_err_flags      (tensor_reduce_err_flags),
  // PMU performance counters
  .pmu_count_up                 (pmu_count_up),
  .pmu_read_data                (pmu_read_data),
  .pmu_read_sel                 (pmu_read_sel),
  .pmu_write_en                 (pmu_write_en),
  .pmu_write_data               (pmu_write_data),
  .pmu_neigh_event_sel          (pmu_neigh_event_sel),
  // Events for performance counters from Dcache and VPU
  .io_events_dcache_vpu         (io_events_dcache_vpu),
  .dcache_debug_signals         (dcache_debug_signals)
);


`RST_EN_FF(clock, reset_w, te_enable, te_instr_valid_reg, te_instr_valid_reginput, '0)
`RST_EN_FF(clock, reset_w, te_enable, te_instr_bus_reg, TE_wb_reg_inst , '0)
`RST_EN_FF(clock, reset_w, te_enable, te_instr_addr_reg, wb_reg_pc_ext , '0)
`RST_EN_FF(clock, reset_w, te_enable, te_instr_context_reg, TE_wb_reg_context, '0)
`RST_EN_FF(clock, reset_w, te_enable, te_exception_reg, TE_exception && (wb_thread_id == te_thread_sel) , '0)
`RST_EN_FF(clock, reset_w, te_enable, te_exception_cause_reg, TE_wb_reg_cause_ie[4:0], '0)
`RST_EN_FF(clock, reset_w, te_enable, te_interrupt_reg, TE_wb_reg_cause_ie[63], '0)
`RST_EN_FF(clock, reset_w, te_enable, te_exception_trap_value_reg, TE_tval, '0)
`RST_EN_FF(clock, reset_w, te_enable, te_privilege_level_reg, TE_prv, '0)
`RST_EN_FF(clock, reset_w, te_enable, te_cpu_halted_reg, debug_out.halted[te_thread_sel], '0)  //should be connected to minion debug interface

// TE registers assigned to the output
assign te_instr_valid_reginput = wb_valid && (wb_thread_id == te_thread_sel);       
assign traceEncoder.instr_valid          = te_instr_valid_reg;
assign traceEncoder.instr_bus            = te_instr_bus_reg;
assign traceEncoder.instr_addr           = te_instr_addr_reg;
assign traceEncoder.instr_context        = te_instr_context_reg;
assign traceEncoder.exception            = te_exception_reg;
assign traceEncoder.exception_cause      = te_exception_cause_reg;
assign traceEncoder.interrupt            = te_interrupt_reg;
assign traceEncoder.exception_trap_value = te_exception_trap_value_reg;
assign traceEncoder.privilege_level      = te_privilege_level_reg;
assign traceEncoder.status_flags         = 1'b0; // no counters in TE, this signals is not used
assign traceEncoder.cpu_halted           = te_cpu_halted_reg;
assign traceEncoder.min_reset            = reset_w | reset_c | reset_d;

// Events for performance registers from Dcache and VPU
assign io_events_dcache_vpu = {io_events_vpu,io_events_dcache};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Dcache module. It handles the load/store operations, the atomic operations
// and the floating point operations.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

dcache_top  dcache (
  // System signals
  .clock                        (clock),
  .reset                        (reset_w),
  // Is this in the IOShire
  .ioshire                      (ioshire),
  // Chicken bit clock gate enable
  .chicken_bit_dcache           (chicken_bit_dcache),
  // Thread ID
  .shire_id                     (shire_id),
  .shire_min_id                 (shire_min_id),

  // Setting control
  .core_ctrl                    (s0_core_dcache_ctrl),
  .core_ctrl_resp               (s0_dcache_core_ctrl_resp),
  // Core request to the DCache (aligns to EXE stage)
  .id_core_alloc_rq_pre         (id_core_dcache_alloc_rq_pre),
  .s0_core_alloc_rq_val         (ex_core_dcache_alloc_rq_val),
  .id_core_gsc                  (id_core_dcache_gsc),
  .id_core_ready                (id_dcache_core_ready),
  .s0_core_req_valid            (ex_core_dcache_req_valid),
  .s0_core_req                  (ex_core_dcache_req),
  .s0_core_gsc                  (ex_core_dcache_gsc),
  .s1_core_kill                 (tag_core_dcache_kill),
  .s1_core_store_data           (tag_core_dcache_store_data),
  .s1_mprot                     (mprot),
  .s2_core_kill                 (mem_core_dcache_kill),
  .s3_core_x31                  (wb_core_dcache_x31),
  // VPU scratchpad direct port
  .s1_vpu_ctrl                  (vpu_dcache_ctl),
  .s3_vpu_scp_resp              (dcache_vpu_scp_res),
  .s3_vpu_scp_data              (dcache_vpu_scp_data),
  .s3_vpu_tenb_data             (dcache_vpu_tenb_data),
  // VPU reduce control
  .vpu_reduce_ctrl              (vpu_reduce_ctrl),
  // DCache response to the Core
  .s2_core_resp_int_valid       (mem_dcache_core_resp_int_valid),
  .s3_core_resp_valid           (wb_dcache_core_resp_valid),
  .s3_core_resp                 (wb_dcache_core_resp),
  // Dcache control
  .id_core_scoreboard           (id_dcache_core_scoreboard),
  .id_core_sb_fp_dealloc        (id_dcache_core_sb_fp_dealloc),
  .id_core_sb_int_dealloc       (id_dcache_core_sb_int_dealloc),
  .s1_core_replay_next          (tag_dcache_core_replay_next),
  .s1_core_xcpt                 (tag_dcache_core_xcpt),
  .s2_core_flush_pipeline       (mem_dcache_core_flush_pipeline),
  .s3_ordered                   (id_dcache_core_ordered),
  .s3_invalidate_lr             (wb_core_dcache_invalidate_lr),
  .wb_dcache_fp_toint           (wb_dcache_fp_toint),
  // Minimum page size ESR
  .vmspagesize                  (vmspagesize),
  // Other ESR settings
  .bypass_dcache                (esr_bypass_dcache),
  // Memory control override
  .mem_ctrl_override            (mem_ctrl_override),
  // TLB/PTW control
  .satp_info                    (satp_info),
  .matp_info                    (matp_info),
  .satp_info_en                 (satp_info_en),
  .matp_info_en                 (matp_info_en),
  .vm_status                    (vm_status),
  .tlb_invalidate               (tlb_invalidate),
  // PTW request
  .ptw_req_data                 (dc_ptw_req_data),
  .ptw_req_valid                (dc_ptw_req_valid),
  .ptw_req_ready                (dc_ptw_req_ready),
  // PTW response
  .ptw_resp_data                (ptw_dc_resp_data),
  .ptw_resp_valid               (ptw_dc_resp_valid),
  // DCache requests to L2 evict
  .l2_evict_req_ready           (l2_dcache_evict_req_ready),
  .l2_evict_req_valid           (l2_dcache_evict_req_valid),
  .l2_evict_req                 (l2_dcache_evict_req),
  // DCache requests to L2 miss
  .l2_miss_req_ready            (l2_dcache_miss_req_ready),
  .l2_miss_req_valid            (l2_dcache_miss_req_valid),
  .l2_miss_req                  (l2_dcache_miss_req),
  // Response L2 port
  .l2_resp_ready                (l2_dcache_resp_ready),
  .l2_resp_valid                (l2_dcache_resp_valid),
  .l2_resp                      (l2_dcache_resp),
  // Internal response L2 port
  .l2_resp_ready_int            (l2_dcache_resp_ready_int),
  .l2_resp_valid_int            (l2_dcache_resp_valid_int),
  .l2_resp_int                  (l2_dcache_resp_int),
  // breakpoing data
  .s1_bp_conf                   (tag_dcache_bp),
  .s1_bp_conf_valid             (tag_dcache_bp_valid),
  // Idle flag
  .dcache_idle_excl_mode        (dcache_idle_excl_mode),
  // Error bits
  .tensor_load_err_flags        (tensor_load_err_flags),
  .cache_ops_err_flags          (cache_ops_err_flags),
  .tensor_reduce_err_flags      (tensor_reduce_err_flags),
  // APB signals from BPAM for debug
  .apb_paddr                    (dcache_apb_paddr),
  .apb_pwrite                   (dcache_apb_pwrite),
  .apb_psel                     (dcache_apb_psel),
  .apb_penable                  (dcache_apb_penable),
  .apb_pwdata                   (dcache_apb_pwdata),
  .apb_pready                   (dcache_apb_pready),
  .apb_prdata                   (dcache_apb_prdata),
  .apb_pslverr                  (dcache_apb_pslverr),
  // Events for performance counters
  .io_events                    (io_events_dcache),
  // Bus error
  .bus_err                      (dcache_bus_error),
  .bus_err_addr                 (dcache_bus_error_addr),
  `ifdef DCACHE_REPORT_PC
  .bus_err_pc                   (dcache_bus_error_pc),
  `endif
  // Debug signals to CSR
  .csr_debug_sigs               (dcache_debug_signals),
  // Signals for Debug Status Monitor
  .sm_match_debug_signals_tl0   (sm_match_debug_signals_tl0),
  .sm_filter_debug_signals_tl0  (sm_filter_debug_signals_tl0),
  .sm_data_debug_signals_tl0    (sm_data_debug_signals_tl0),

  .sm_match_debug_signals_tl1   (sm_match_debug_signals_tl1),
  .sm_filter_debug_signals_tl1  (sm_filter_debug_signals_tl1),
  .sm_data_debug_signals_tl1    (sm_data_debug_signals_tl1),

  .sm_match_debug_signals_ts    (sm_match_debug_signals_ts),
  .sm_filter_debug_signals_ts   (sm_filter_debug_signals_ts),
  .sm_data_debug_signals_ts     (sm_data_debug_signals_ts),

  .sm_match_debug_signals_co    (sm_match_debug_signals_co),
  .sm_filter_debug_signals_co   (sm_filter_debug_signals_co),
  .sm_data_debug_signals_co     (sm_data_debug_signals_co),

  .sm_match_debug_signals_col2  (sm_match_debug_signals_col2),
  .sm_filter_debug_signals_col2 (sm_filter_debug_signals_col2),
  .sm_data_debug_signals_col2   (sm_data_debug_signals_col2),

  .sm_match_debug_signals_mh    (sm_match_debug_signals_mh),
  .sm_filter_debug_signals_mh   (sm_filter_debug_signals_mh),
  .sm_data_debug_signals_mh     (sm_data_debug_signals_mh),

  .sm_match_debug_signals_dc    (sm_match_debug_signals_dc),
  .sm_filter_debug_signals_dc   (sm_filter_debug_signals_dc),
  .sm_data_debug_signals_dc     (sm_data_debug_signals_dc)
);
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  // debug apb slave connected to BPAM
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  
  // Register decoded address to send enable signal to the different modules
  logic apb_target_dcache;
  logic apb_target_debug;
  logic apb_target_debug_reg;
  logic [`ESR_APB_D_WIDTH-1:0] apb_pwdata_reg; 
  
  assign apb_target_dcache = (apb_paddr[`MINION_DEBUG_APB_ADDR_UPPER_RANGE] == `MINION_DEBUG_APB_ADDR_DCACHE);
  assign apb_target_debug  = (apb_paddr[`MINION_DEBUG_APB_ADDR_UPPER_RANGE] == `MINION_DEBUG_APB_ADDR_DEBUG);
  
  // Register enable to Dcache to ease timing
  logic  dcache_apb_en;

  //          CLR    RESET            EN             DOUT                  DIN                      DEF
  `RST_EN_FF (clock, reset_c,         dcache_apb_en, dcache_apb_psel,      dcache_apb_psel_next,    1'b0)
  `RST_EN_FF (clock, reset_c,         dcache_apb_en, dcache_apb_penable,   dcache_apb_penable_next, 1'b0)
  `EN_FF     (clock,                  dcache_apb_en, dcache_apb_pwrite,    dcache_apb_pwrite_next       )
  `EN_FF     (clock,                  dcache_apb_en, dcache_apb_paddr,     dcache_apb_paddr_next        )

  assign dcache_apb_en           = apb_psel | dcache_apb_psel;
  assign dcache_apb_psel_next    = apb_psel & apb_target_dcache;
  assign dcache_apb_penable_next = apb_penable & apb_target_dcache;
  assign dcache_apb_pwrite_next  = apb_pwrite;
  assign dcache_apb_paddr_next   = apb_paddr[`DCACHE_DBG_ADDR_RANGE];

  assign dcache_apb_pwdata       = apb_pwdata_reg;

  // Register enable to Debug module to ease timing
  logic  debug_apb_en;

  //          CLR    RESET            EN            DOUT                  DIN                     DEF
  `RST_EN_FF (clock, reset_c,         debug_apb_en, debug_apb_psel,       debug_apb_psel_next,    1'b0)
  `RST_EN_FF (clock, reset_c,         debug_apb_en, debug_apb_penable,    debug_apb_penable_next, 1'b0)
  `EN_FF     (clock,                  debug_apb_en, debug_apb_pwrite,     debug_apb_pwrite_next       )
  `EN_FF     (clock,                  debug_apb_en, debug_apb_paddr,      debug_apb_paddr_next        )
  `EN_FF     (clock,                  debug_apb_en, apb_target_debug_reg, apb_target_debug            )

  assign debug_apb_en           = apb_psel | debug_apb_psel;
  assign debug_apb_psel_next    = apb_psel & apb_target_debug;
  assign debug_apb_penable_next = apb_penable & apb_target_debug;
  assign debug_apb_pwrite_next  = apb_pwrite;
  assign debug_apb_paddr_next   = apb_paddr[`MIN_REG_APB_ADDR_WIDTH-1:0];

  assign debug_apb_pwdata       = apb_pwdata_reg[`MINION_DEBUG_APB_D_WIDTH-1:0];

  //          CLR     EN        DOUT             DIN
  `EN_FF     (clock,  apb_psel, apb_pwdata_reg,  apb_pwdata)

  // Mux outputs
  assign apb_pready  = apb_target_debug_reg ? debug_apb_pready  : dcache_apb_pready;
  assign apb_prdata  = apb_target_debug_reg ? debug_apb_prdata  : dcache_apb_prdata;
  assign apb_pslverr = apb_target_debug_reg ? debug_apb_pslverr : dcache_apb_pslverr;

  // Debug module
  minion_debug_apb_slv debug_apb_slv (
    .clock               (clock),
    .reset               (reset_d),

    .apb_paddr           (debug_apb_paddr),
    .apb_pwrite          (debug_apb_pwrite),
    .apb_psel            (debug_apb_psel),
    .apb_penable         (debug_apb_penable),
    .apb_pwdata          (debug_apb_pwdata),

    .apb_pready          (debug_apb_pready),
    .apb_prdata          (debug_apb_prdata),
    .apb_pslverr         (debug_apb_pslverr),

    .csr_wdata_ip        (ddata0_wdata),
    .update_ddata0_ip    (update_ddata0),
    .read_ddata0_op      (read_ddata0),
    .ffb_wdata_op        (debug_ffb_wdata),
    .ffb_en_op           (debug_ffb_en),
    .ffb_thread_sel_op   (debug_ffb_thread_sel),
    .ex_program_buffer   (debug_ex_program_buffer),
    .in_debug_mode       (debug_out.halted)
  );

endmodule

