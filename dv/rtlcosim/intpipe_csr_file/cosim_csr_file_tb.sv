// Co-simulation testbench: intpipe_csr_file (new) vs original.
//
// Instantiates both modules with identical stimulus and exposes all
// key outputs for cycle-by-cycle comparison in the C++ test driver.

`include "soc.vh"

module cosim_csr_file_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // ── Additional resets ──
  input  logic        rst_w_ni,
  input  logic        rst_d_ni,
  input  logic        stim_test_en,
  input  logic        stim_dft_reset_byp,
  input  logic        stim_dft_reset,

  // ── Interrupts and thread id ──
  input  logic [1:0]  stim_int_mieco,
  input  logic [1:0]  stim_int_mtip,
  input  logic [1:0]  stim_int_msip,
  input  logic [1:0]  stim_int_meip,
  input  logic [1:0]  stim_int_seip,
  input  logic [3:0]  stim_int_fcc,     // NrThreads*FccPerMin = 4
  input  logic [7:0]  stim_shire_id,
  input  logic [4:0]  stim_shire_min_id,
  input  logic        stim_wb_thread_id,
  input  logic        stim_mem_thread_id,

  // ── FrontEnd, Dcache and VPU control inputs ──
  input  logic        stim_pending_scoreboard_t0,
  input  logic        stim_vpu_tfma_enabled,
  input  logic        stim_vpu_tfma_tenb_working,
  input  logic        stim_vpu_tquant_enabled,
  input  logic        stim_vpu_treduce_enabled,

  // ── dcache_ctrl_resp (dcache_core_ctrl / dcache_core_ctrl_t) as flat bits ──
  input  logic [$bits(dcache_core_ctrl)-1:0] stim_dcache_ctrl_resp,

  // ── Read/Write port ──
  input  logic [11:0] stim_io_rw_mem_addr,
  input  logic [2:0]  stim_io_rw_wb_cmd,
  input  logic [2:0]  stim_io_rw_mem_cmd,
  input  logic [2:0]  stim_io_rw_wb_inst,
  input  logic [63:0] stim_io_rw_mem_wdata,
  input  logic [63:0] stim_io_rw_wb_wdata,
  input  logic [4:0]  stim_io_rw_wb_waddr,
  input  logic [63:0] stim_wb_x31_reg,

  // ── Exception and retire from core ──
  input  logic        stim_io_exception,
  input  logic        stim_io_retire,
  input  logic [63:0] stim_io_cause,     // csr_cause_t
  input  logic [48:0] stim_io_pc,
  input  logic [48:0] stim_io_badaddr,   // VaSizeExt = 49
  input  logic [31:0] stim_io_inst_bits,
  input  logic        stim_io_inst_rvc,
  input  logic [1:0]  stim_io_bad_redirect,
  input  logic [1:0]  stim_io_redirect,

  // ── VPU / FCSR ──
  input  logic        stim_io_fcsr_flags_valid,
  input  logic [5:0]  stim_io_fcsr_flags_bits,
  input  logic        stim_io_fcsr_thread_id,

  // ── Bus errors ──
  input  logic [1:0]  stim_dcache_bus_error,
  input  logic [1:0][39:0] stim_dcache_bus_error_addr,

  // ── Breakpoint and debug ──
  input  logic [1:0]  stim_debug_timing,
  input  logic        stim_bp_progress_save,
  input  logic [1:0][63:0] stim_read_ddata0,
  input  logic [1:0]  stim_halt,
  input  logic [1:0]  stim_resume,
  input  logic [1:0]  stim_ex_progbuf,

  // ── FLB ──
  input  logic        stim_flb_neigh_resp_valid,
  input  logic        stim_flb_neigh_resp_data,
  input  logic        stim_flb_rf_wen_ready,

  // ── L2 resp ──
  input  logic        stim_l2_resp_valid,
  input  logic        stim_l2_resp_ready,
  input  logic [$bits(et_link_minion_rsp_info_t)-1:0] stim_l2_resp,

  // ── Events ──
  input  logic [24:0] stim_io_events_ext, // CsrNrEventsExt = 25

  // ── TE ──
  input  logic        stim_te_thread_sel,

  // ── ESR configuration ──
  input  logic        stim_esr_shire_coop_mode,
  input  logic [5:0]  stim_esr_features,  // esr_minion_features_t = 6 bits

  // ── Tensor/cache error bits ──
  input  logic [2:0]  stim_tensor_load_err_flags, // DcacheTlErrorBits-3+1 = 5-3+1 = 3
  input  logic [1:0][1:0] stim_cache_ops_err_flags, // [DcacheErrFlagRange:0][NrThreads-1:0]
  input  logic        stim_tensor_reduce_err_flags,

  // ── PMU ──
  input  logic [1:0][63:0] stim_pmu_read_data,

  // ── Debug signals ──
  input  logic [63:0] stim_dcache_debug_signals,

  // ═══════════════════════════════════════════════════════════════════
  // NEW module outputs
  // ═══════════════════════════════════════════════════════════════════
  output logic [63:0] new_io_rw_wb_rdata,
  output logic [1:0]  new_io_csr_stall,
  output logic        new_io_csr_xcpt,
  output logic        new_io_eret,
  output logic        new_io_replay,
  output logic [1:0]  new_io_resume,
  output logic [1:0]  new_io_singleStep,
  output logic        new_io_excl_mode,
  output logic        new_io_excl_mode_sel,
  output logic        new_io_excl_mode_transition,

  output logic [1:0][$bits(intpipe_status_t)-1:0] new_io_status,
  output logic [1:0][1:0] new_io_prv,
  output logic [1:0]  new_io_debug,

  output logic [$bits(csr_satp_t)-1:0]  new_io_satp,
  output logic        new_io_satp_en,
  output logic [$bits(csr_matp_t)-1:0]  new_io_matp,
  output logic        new_io_matp_en,

  output logic [48:0] new_io_evec,
  output logic [48:0] new_io_evec_resume,

  output logic [1:0]  new_fe_ctrl_stall,
  output logic        new_icache_invalidate,
  output logic        new_tlb_invalidate,

  output logic [2:0]  new_io_fcsr_rm_thread0,
  output logic [2:0]  new_io_fcsr_rm_thread1,

  output logic [1:0]  new_io_interrupt,
  output logic [1:0][4:0] new_io_interrupt_cause,

  output logic [$bits(core_dcache_ctrl)-1:0] new_dcache_ctrl,
  output logic [$bits(core_vpu_ctrl)-1:0]    new_vpu_ctrl,

  output logic [1:0][$bits(minion_bp_ctrl_t)-1:0] new_bp_control,
  output logic [1:0][47:0] new_bp_address,   // VaSize = 48

  output logic [1:0]  new_update_ddata0,
  output logic [63:0] new_ddata0_wdata,
  output logic [1:0]  new_busy,
  output logic [1:0]  new_debug_out_exception,
  output logic        new_xcpt_traps_to_debug,

  output logic        new_flb_neigh_req_valid,
  output logic [12:0] new_flb_neigh_req_data,
  output logic        new_flb_rf_wen_valid,
  output logic        new_flb_rf_wen_valid_early,
  output logic        new_flb_rf_wen_thread_id,
  output logic [4:0]  new_flb_rf_wen_addr,
  output logic [63:0] new_flb_rf_wen_data,
  output logic        new_flb_scoreboard_valid,
  output logic [$bits(minion_reg_dest_t)-1:0] new_flb_scoreboard_addr,

  output logic [1:0]  new_TE_prv,
  output logic        new_TE_exception,
  output logic [48:0] new_TE_tval,      // VaSizeExt = 49
  output logic [63:0] new_TE_wb_reg_cause_ie,

  output logic [1:0][$bits(minst_mask_t)-1:0] new_io_minstmask,
  output logic [1:0][31:0] new_io_minstmatch,

  output logic [1:0][2:0] new_gsc_progress,

  output logic [7:0]  new_pmu_count_up,  // PmuMinionCountersNum = 8
  output logic [1:0][3:0] new_pmu_read_sel, // PmuCountersSelectBits = 4
  output logic [11:0] new_pmu_write_en,  // PmuTotalCountersNum = 12
  output logic [63:0] new_pmu_write_data,
  output logic [19:0] new_pmu_neigh_event_sel, // PmuNeighEventCntSelBits = 20

  // ═══════════════════════════════════════════════════════════════════
  // ORIGINAL module outputs
  // ═══════════════════════════════════════════════════════════════════
  output logic [63:0] orig_io_rw_wb_rdata,
  output logic [1:0]  orig_io_csr_stall,
  output logic        orig_io_csr_xcpt,
  output logic        orig_io_eret,
  output logic        orig_io_replay,
  output logic [1:0]  orig_io_resume,
  output logic [1:0]  orig_io_singleStep,
  output logic        orig_io_excl_mode,
  output logic        orig_io_excl_mode_sel,
  output logic        orig_io_excl_mode_transition,

  output logic [1:0][$bits(intpipe_status_t)-1:0] orig_io_status,
  output logic [1:0][1:0] orig_io_prv,
  output logic [1:0]  orig_io_debug,

  output logic [$bits(csr_satp_t)-1:0]  orig_io_satp,
  output logic        orig_io_satp_en,
  output logic [$bits(csr_matp_t)-1:0]  orig_io_matp,
  output logic        orig_io_matp_en,

  output logic [48:0] orig_io_evec,
  output logic [48:0] orig_io_evec_resume,

  output logic [1:0]  orig_fe_ctrl_stall,
  output logic        orig_icache_invalidate,
  output logic        orig_tlb_invalidate,

  output logic [2:0]  orig_io_fcsr_rm_thread0,
  output logic [2:0]  orig_io_fcsr_rm_thread1,

  output logic [1:0]  orig_io_interrupt,
  output logic [1:0][4:0] orig_io_interrupt_cause,

  output logic [$bits(core_dcache_ctrl)-1:0] orig_dcache_ctrl,
  output logic [$bits(core_vpu_ctrl)-1:0]    orig_vpu_ctrl,

  output logic [1:0][$bits(minion_bp_ctrl_t)-1:0] orig_bp_control,
  output logic [1:0][47:0] orig_bp_address,

  output logic [1:0]  orig_update_ddata0,
  output logic [63:0] orig_ddata0_wdata,
  output logic [1:0]  orig_busy,
  output logic [1:0]  orig_debug_out_exception,
  output logic        orig_xcpt_traps_to_debug,

  output logic        orig_flb_neigh_req_valid,
  output logic [12:0] orig_flb_neigh_req_data,
  output logic        orig_flb_rf_wen_valid,
  output logic        orig_flb_rf_wen_valid_early,
  output logic        orig_flb_rf_wen_thread_id,
  output logic [4:0]  orig_flb_rf_wen_addr,
  output logic [63:0] orig_flb_rf_wen_data,
  output logic        orig_flb_scoreboard_valid,
  output logic [$bits(minion_reg_dest_t)-1:0] orig_flb_scoreboard_addr,

  output logic [1:0]  orig_TE_prv,
  output logic        orig_TE_exception,
  output logic [48:0] orig_TE_tval,
  output logic [63:0] orig_TE_wb_reg_cause_ie,

  output logic [1:0][$bits(minst_mask_t)-1:0] orig_io_minstmask,
  output logic [1:0][31:0] orig_io_minstmatch,

  output logic [1:0][2:0] orig_gsc_progress,

  output logic [7:0]  orig_pmu_count_up,
  output logic [1:0][3:0] orig_pmu_read_sel,
  output logic [11:0] orig_pmu_write_en,
  output logic [63:0] orig_pmu_write_data,
  output logic [19:0] orig_pmu_neigh_event_sel
);

  import minion_pkg::*;
  import intpipe_csr_pkg::*;
  import dft_pkg::*;

  // Cast flat stimulus to typed structs
  dcache_core_ctrl   dcache_ctrl_resp_s;
  assign dcache_ctrl_resp_s = stim_dcache_ctrl_resp;

  csr_cause_t        io_cause_s;
  assign io_cause_s = stim_io_cause;

  et_link_minion_rsp_info_t l2_resp_s;
  assign l2_resp_s = stim_l2_resp;

  esr_minion_features_t esr_features_s;
  assign esr_features_s = stim_esr_features;

  // ── Typed outputs from new module ──
  intpipe_status_t [1:0]     new_io_status_s;
  csr_satp_t                 new_io_satp_s;
  csr_matp_t                 new_io_matp_s;
  core_dcache_ctrl           new_dcache_ctrl_s;
  core_vpu_ctrl              new_vpu_ctrl_s;
  minion_bp_ctrl_t [1:0]     new_bp_control_s;
  minion_reg_dest_t          new_flb_scoreboard_addr_s;
  csr_cause_t                new_TE_wb_reg_cause_ie_s;
  minst_mask_t [1:0]         new_io_minstmask_s;

  assign new_io_status      = new_io_status_s;
  assign new_io_satp        = new_io_satp_s;
  assign new_io_matp        = new_io_matp_s;
  assign new_dcache_ctrl    = new_dcache_ctrl_s;
  assign new_vpu_ctrl       = new_vpu_ctrl_s;
  assign new_bp_control     = new_bp_control_s;
  assign new_flb_scoreboard_addr = new_flb_scoreboard_addr_s;
  assign new_TE_wb_reg_cause_ie  = new_TE_wb_reg_cause_ie_s;
  assign new_io_minstmask   = new_io_minstmask_s;

  // ── Typed outputs from original module ──
  intpipe_status_t [1:0]     orig_io_status_s;
  csr_satp_t                 orig_io_satp_s;
  csr_matp_t                 orig_io_matp_s;
  core_dcache_ctrl           orig_dcache_ctrl_s;
  core_vpu_ctrl              orig_vpu_ctrl_s;
  minion_bp_ctrl   [1:0]     orig_bp_control_s;
  minion_reg_dest            orig_flb_scoreboard_addr_s;
  csr_cause_t                orig_TE_wb_reg_cause_ie_s;
  minst_mask_t [1:0]         orig_io_minstmask_s;

  assign orig_io_status      = orig_io_status_s;
  assign orig_io_satp        = orig_io_satp_s;
  assign orig_io_matp        = orig_io_matp_s;
  assign orig_dcache_ctrl    = orig_dcache_ctrl_s;
  assign orig_vpu_ctrl       = orig_vpu_ctrl_s;
  assign orig_bp_control     = orig_bp_control_s;
  assign orig_flb_scoreboard_addr = orig_flb_scoreboard_addr_s;
  assign orig_TE_wb_reg_cause_ie  = orig_TE_wb_reg_cause_ie_s;
  assign orig_io_minstmask   = orig_io_minstmask_s;

  // ──────────────────────────────────────────────────────────────────
  // NEW module (SystemVerilog, active-low async resets)
  // ──────────────────────────────────────────────────────────────────
  intpipe_csr_file u_new (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .rst_w_ni                 (rst_w_ni),
    .rst_d_ni                 (rst_d_ni),
    .test_en                  (stim_test_en),
    .dft__reset_byp           (stim_dft_reset_byp),
    .dft__reset               (stim_dft_reset),

    .int_mieco                (stim_int_mieco),
    .int_mtip                 (stim_int_mtip),
    .int_msip                 (stim_int_msip),
    .int_meip                 (stim_int_meip),
    .int_seip                 (stim_int_seip),
    .int_fcc                  (stim_int_fcc),
    .shire_id                 (stim_shire_id),
    .shire_min_id             (stim_shire_min_id),
    .wb_thread_id             (stim_wb_thread_id),
    .mem_thread_id            (stim_mem_thread_id),

    .fe_ctrl_stall            (new_fe_ctrl_stall),
    .dcache_ctrl              (new_dcache_ctrl_s),
    .dcache_ctrl_resp         (dcache_ctrl_resp_s),
    .vpu_ctrl                 (new_vpu_ctrl_s),
    .pending_scoreboard_t0    (stim_pending_scoreboard_t0),
    .vpu_tfma_enabled         (stim_vpu_tfma_enabled),
    .vpu_tfma_tenb_working    (stim_vpu_tfma_tenb_working),
    .vpu_tquant_enabled       (stim_vpu_tquant_enabled),
    .vpu_treduce_enabled      (stim_vpu_treduce_enabled),
    .icache_invalidate        (new_icache_invalidate),
    .tlb_invalidate           (new_tlb_invalidate),

    .io_rw_mem_addr           (stim_io_rw_mem_addr),
    .io_rw_wb_cmd             (csr_cmd_t'(stim_io_rw_wb_cmd)),
    .io_rw_mem_cmd            (csr_cmd_t'(stim_io_rw_mem_cmd)),
    .io_rw_wb_inst            (stim_io_rw_wb_inst),
    .io_rw_wb_rdata           (new_io_rw_wb_rdata),
    .io_rw_mem_wdata          (stim_io_rw_mem_wdata),
    .io_rw_wb_wdata           (stim_io_rw_wb_wdata),
    .io_rw_wb_waddr           (stim_io_rw_wb_waddr),
    .wb_x31_reg               (stim_wb_x31_reg),

    .io_csr_stall             (new_io_csr_stall),
    .io_csr_xcpt              (new_io_csr_xcpt),
    .io_eret                  (new_io_eret),
    .io_replay                (new_io_replay),
    .io_resume                (new_io_resume),
    .io_singleStep            (new_io_singleStep),
    .io_excl_mode             (new_io_excl_mode),
    .io_excl_mode_sel         (new_io_excl_mode_sel),
    .io_excl_mode_transition  (new_io_excl_mode_transition),

    .io_status                (new_io_status_s),
    .io_prv                   (new_io_prv),
    .io_debug                 (new_io_debug),

    .io_satp                  (new_io_satp_s),
    .io_satp_en               (new_io_satp_en),
    .io_matp                  (new_io_matp_s),
    .io_matp_en               (new_io_matp_en),

    .l2_resp_valid            (stim_l2_resp_valid),
    .l2_resp_ready            (stim_l2_resp_ready),
    .l2_resp                  (l2_resp_s),

    .io_evec                  (new_io_evec),
    .io_evec_resume           (new_io_evec_resume),

    .io_events_ext            (stim_io_events_ext),
    .io_exception             (stim_io_exception),
    .io_retire                (stim_io_retire),
    .io_cause                 (io_cause_s),
    .io_pc                    (stim_io_pc),
    .io_badaddr               (stim_io_badaddr),
    .io_inst_bits             (stim_io_inst_bits),
    .io_inst_rvc              (stim_io_inst_rvc),
    .io_bad_redirect          (stim_io_bad_redirect),
    .io_redirect              (stim_io_redirect),

    .io_fcsr_rm_thread0       (new_io_fcsr_rm_thread0),
    .io_fcsr_rm_thread1       (new_io_fcsr_rm_thread1),
    .io_fcsr_flags_valid      (stim_io_fcsr_flags_valid),
    .io_fcsr_flags_bits       (stim_io_fcsr_flags_bits),
    .io_fcsr_thread_id        (stim_io_fcsr_thread_id),

    .io_interrupt             (new_io_interrupt),
    .io_interrupt_cause       (new_io_interrupt_cause),

    .dcache_bus_error         (stim_dcache_bus_error),
    .dcache_bus_error_addr    (stim_dcache_bus_error_addr),

    .bp_control               (new_bp_control_s),
    .bp_address               (new_bp_address),
    .debug_timing             (stim_debug_timing),
    .update_ddata0            (new_update_ddata0),
    .ddata0_wdata             (new_ddata0_wdata),
    .bp_progress_save         (stim_bp_progress_save),
    .read_ddata0              (stim_read_ddata0),
    .halt                     (stim_halt),
    .resume                   (stim_resume),
    .ex_progbuf               (stim_ex_progbuf),
    .busy                     (new_busy),
    .debug_out_exception      (new_debug_out_exception),
    .xcpt_traps_to_debug      (new_xcpt_traps_to_debug),

    .flb_neigh_req_valid      (new_flb_neigh_req_valid),
    .flb_neigh_req_data       (new_flb_neigh_req_data),
    .flb_neigh_resp_valid     (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data      (stim_flb_neigh_resp_data),
    .flb_rf_wen_ready         (stim_flb_rf_wen_ready),
    .flb_rf_wen_valid         (new_flb_rf_wen_valid),
    .flb_rf_wen_valid_early   (new_flb_rf_wen_valid_early),
    .flb_rf_wen_thread_id     (new_flb_rf_wen_thread_id),
    .flb_rf_wen_addr          (new_flb_rf_wen_addr),
    .flb_rf_wen_data          (new_flb_rf_wen_data),
    .flb_scoreboard_valid     (new_flb_scoreboard_valid),
    .flb_scoreboard_addr      (new_flb_scoreboard_addr_s),

    .TE_prv                   (new_TE_prv),
    .TE_exception             (new_TE_exception),
    .TE_tval                  (new_TE_tval),
    .TE_wb_reg_cause_ie       (new_TE_wb_reg_cause_ie_s),
    .te_thread_sel            (stim_te_thread_sel),

    .io_minstmask             (new_io_minstmask_s),
    .io_minstmatch            (new_io_minstmatch),

    .esr_shire_coop_mode      (stim_esr_shire_coop_mode),
    .esr_features             (esr_features_s),

    .tensor_load_err_flags    (stim_tensor_load_err_flags),
    .cache_ops_err_flags      (stim_cache_ops_err_flags),
    .tensor_reduce_err_flags  (stim_tensor_reduce_err_flags),

    .gsc_progress             (new_gsc_progress),

    .pmu_count_up             (new_pmu_count_up),
    .pmu_read_data            (stim_pmu_read_data),
    .pmu_read_sel             (new_pmu_read_sel),
    .pmu_write_en             (new_pmu_write_en),
    .pmu_write_data           (new_pmu_write_data),
    .pmu_neigh_event_sel      (new_pmu_neigh_event_sel),

    .dcache_debug_signals     (stim_dcache_debug_signals)
  );

  // ──────────────────────────────────────────────────────────────────
  // ORIGINAL module (Verilog, active-high synchronous resets)
  // ──────────────────────────────────────────────────────────────────
  logic orig_reset_c, orig_reset_w, orig_reset_d;
  assign orig_reset_c = ~rst_ni;
  assign orig_reset_w = ~rst_w_ni;
  assign orig_reset_d = ~rst_d_ni;

  // The original flb_rf_wen_valid_early is declared as plain `logic`
  // (not `output logic`) in some versions.  Connect to a wire.
  logic orig_flb_rf_wen_valid_early_w;
  assign orig_flb_rf_wen_valid_early = orig_flb_rf_wen_valid_early_w;

  intpipe_csr_file_orig u_orig (
    .clock                    (clk_i),
    .reset_c                  (orig_reset_c),
    .reset_w                  (orig_reset_w),
    .reset_d                  (orig_reset_d),
    .test_en                  (stim_test_en),
    .dft__reset_byp           (stim_dft_reset_byp),
    .dft__reset               (stim_dft_reset),

    .int_mieco                (stim_int_mieco),
    .int_mtip                 (stim_int_mtip),
    .int_msip                 (stim_int_msip),
    .int_meip                 (stim_int_meip),
    .int_seip                 (stim_int_seip),
    .int_fcc                  (stim_int_fcc),
    .shire_id                 (stim_shire_id),
    .shire_min_id             (stim_shire_min_id),
    .wb_thread_id             (stim_wb_thread_id),
    .mem_thread_id            (stim_mem_thread_id),

    .fe_ctrl_stall            (orig_fe_ctrl_stall),
    .dcache_ctrl              (orig_dcache_ctrl_s),
    .dcache_ctrl_resp         (dcache_ctrl_resp_s),
    .vpu_ctrl                 (orig_vpu_ctrl_s),
    .pending_scoreboard_t0    (stim_pending_scoreboard_t0),
    .vpu_tfma_enabled         (stim_vpu_tfma_enabled),
    .vpu_tfma_tenb_working    (stim_vpu_tfma_tenb_working),
    .vpu_tquant_enabled       (stim_vpu_tquant_enabled),
    .vpu_treduce_enabled      (stim_vpu_treduce_enabled),
    .icache_invalidate        (orig_icache_invalidate),
    .tlb_invalidate           (orig_tlb_invalidate),

    .io_rw_mem_addr           (stim_io_rw_mem_addr),
    .io_rw_wb_cmd             (csr_cmd_t'(stim_io_rw_wb_cmd)),
    .io_rw_mem_cmd            (csr_cmd_t'(stim_io_rw_mem_cmd)),
    .io_rw_wb_inst            (stim_io_rw_wb_inst),
    .io_rw_wb_rdata           (orig_io_rw_wb_rdata),
    .io_rw_mem_wdata          (stim_io_rw_mem_wdata),
    .io_rw_wb_wdata           (stim_io_rw_wb_wdata),
    .io_rw_wb_waddr           (stim_io_rw_wb_waddr),
    .wb_x31_reg               (stim_wb_x31_reg),

    .io_csr_stall             (orig_io_csr_stall),
    .io_csr_xcpt              (orig_io_csr_xcpt),
    .io_eret                  (orig_io_eret),
    .io_replay                (orig_io_replay),
    .io_resume                (orig_io_resume),
    .io_singleStep            (orig_io_singleStep),
    .io_excl_mode             (orig_io_excl_mode),
    .io_excl_mode_sel         (orig_io_excl_mode_sel),
    .io_excl_mode_transition  (orig_io_excl_mode_transition),

    .io_status                (orig_io_status_s),
    .io_prv                   (orig_io_prv),
    .io_debug                 (orig_io_debug),

    .io_satp                  (orig_io_satp_s),
    .io_satp_en               (orig_io_satp_en),
    .io_matp                  (orig_io_matp_s),
    .io_matp_en               (orig_io_matp_en),

    .l2_resp_valid            (stim_l2_resp_valid),
    .l2_resp_ready            (stim_l2_resp_ready),
    .l2_resp                  (l2_resp_s),

    .io_evec                  (orig_io_evec),
    .io_evec_resume           (orig_io_evec_resume),

    .io_events_ext            (stim_io_events_ext),
    .io_exception             (stim_io_exception),
    .io_retire                (stim_io_retire),
    .io_cause                 (io_cause_s),
    .io_pc                    (stim_io_pc),
    .io_badaddr               (stim_io_badaddr),
    .io_inst_bits             (stim_io_inst_bits),
    .io_inst_rvc              (stim_io_inst_rvc),
    .io_bad_redirect          (stim_io_bad_redirect),
    .io_redirect              (stim_io_redirect),

    .io_fcsr_rm_thread0       (orig_io_fcsr_rm_thread0),
    .io_fcsr_rm_thread1       (orig_io_fcsr_rm_thread1),
    .io_fcsr_flags_valid      (stim_io_fcsr_flags_valid),
    .io_fcsr_flags_bits       (stim_io_fcsr_flags_bits),
    .io_fcsr_thread_id        (stim_io_fcsr_thread_id),

    .io_interrupt             (orig_io_interrupt),
    .io_interrupt_cause       (orig_io_interrupt_cause),

    .dcache_bus_error         (stim_dcache_bus_error),
    .dcache_bus_error_addr    (stim_dcache_bus_error_addr),

    .bp_control               (orig_bp_control_s),
    .bp_address               (orig_bp_address),
    .debug_timing             (stim_debug_timing),
    .update_ddata0            (orig_update_ddata0),
    .ddata0_wdata             (orig_ddata0_wdata),
    .bp_progress_save         (stim_bp_progress_save),
    .read_ddata0              (stim_read_ddata0),
    .halt                     (stim_halt),
    .resume                   (stim_resume),
    .ex_progbuf               (stim_ex_progbuf),
    .busy                     (orig_busy),
    .debug_out_exception      (orig_debug_out_exception),
    .xcpt_traps_to_debug      (orig_xcpt_traps_to_debug),

    .flb_neigh_req_valid      (orig_flb_neigh_req_valid),
    .flb_neigh_req_data       (orig_flb_neigh_req_data),
    .flb_neigh_resp_valid     (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data      (stim_flb_neigh_resp_data),
    .flb_rf_wen_ready         (stim_flb_rf_wen_ready),
    .flb_rf_wen_valid         (orig_flb_rf_wen_valid),
    .flb_rf_wen_valid_early   (orig_flb_rf_wen_valid_early_w),
    .flb_rf_wen_thread_id     (orig_flb_rf_wen_thread_id),
    .flb_rf_wen_addr          (orig_flb_rf_wen_addr),
    .flb_rf_wen_data          (orig_flb_rf_wen_data),
    .flb_scoreboard_valid     (orig_flb_scoreboard_valid),
    .flb_scoreboard_addr      (orig_flb_scoreboard_addr_s),

    .TE_prv                   (orig_TE_prv),
    .TE_exception             (orig_TE_exception),
    .TE_tval                  (orig_TE_tval),
    .TE_wb_reg_cause_ie       (orig_TE_wb_reg_cause_ie_s),
    .te_thread_sel            (stim_te_thread_sel),

    .io_minstmask             (orig_io_minstmask_s),
    .io_minstmatch            (orig_io_minstmatch),

    .esr_shire_coop_mode      (stim_esr_shire_coop_mode),
    .esr_features             (esr_features_s),

    .tensor_load_err_flags    (stim_tensor_load_err_flags),
    .cache_ops_err_flags      (stim_cache_ops_err_flags),
    .tensor_reduce_err_flags  (stim_tensor_reduce_err_flags),

    .gsc_progress             (orig_gsc_progress),

    .pmu_count_up             (orig_pmu_count_up),
    .pmu_read_data            (stim_pmu_read_data),
    .pmu_read_sel             (orig_pmu_read_sel),
    .pmu_write_en             (orig_pmu_write_en),
    .pmu_write_data           (orig_pmu_write_data),
    .pmu_neigh_event_sel      (orig_pmu_neigh_event_sel),

    .dcache_debug_signals     (stim_dcache_debug_signals)
  );

endmodule
