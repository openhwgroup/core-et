// Co-simulation testbench: intpipe_top (new) vs original.
//
// Instantiates both modules with identical stimulus and exposes all
// key outputs for cycle-by-cycle comparison in the C++ test driver.

`include "soc.vh"

module cosim_intpipe_top_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // ── Additional resets ──
  input  logic        rst_w_ni,
  input  logic        rst_d_ni,

  // ── DFT ──
  input  logic        stim_dft_reset_byp,
  input  logic        stim_dft_reset,

  // ── TE enable ──
  input  logic        stim_te_enable,

  // ── Interrupts (flat) ──
  input  logic [$bits(minion_interrupts)-1:0] stim_interrupts,

  // ── Thread / shire id ──
  input  logic [7:0]  stim_shire_id,
  input  logic [4:0]  stim_shire_min_id,

  // ── Chicken bits ──
  input  logic        stim_chicken_bit_intpipe,

  // ── Frontend response ──
  input  logic        stim_id_fe_resp_valid,
  input  logic        stim_id_fe_resp_thread_id,
  input  logic [$bits(frontend_core_resp)-1:0] stim_id_fe_resp,

  // ── DCache interface inputs ──
  input  logic [$bits(dcache_core_ctrl)-1:0] stim_dcache_ctrl_resp,
  input  logic        stim_id_dcache_ready,
  input  logic        stim_mem_dcache_resp_int_valid,
  input  logic        stim_wb_dcache_resp_valid,
  input  logic [$bits(dcache_minion_resp)-1:0] stim_wb_dcache_resp,
  input  logic [$bits(dcache_core_bp_t)-1:0] stim_tag_dcache_bp,
  input  logic        stim_tag_dcache_bp_valid,

  // ── Frontend decode control ──
  input  logic        stim_use_decoded_id_ctrl,
  input  logic [$bits(minion_control_t)-1:0] stim_id_ctrl,

  // ── DCache control inputs ──
  input  logic [1:0]  stim_id_dcache_ordered,
  input  logic        stim_id_dcache_idle,
  input  logic [$bits(dcache_scoreboard_t)-1:0] stim_id_dcache_scoreboard,
  input  logic [1:0]  stim_id_dcache_sb_int_dealloc,
  input  logic [1:0]  stim_id_dcache_sb_fp_dealloc,
  input  logic [$bits(dcache_tag_xcpt)-1:0] stim_tag_dcache_xcpt,
  input  logic        stim_tag_dcache_replay_next,
  input  logic        stim_mem_dcache_flush_pipeline,
  input  logic [1:0]  stim_dcache_bus_error,
  input  logic [1:0][39:0] stim_dcache_bus_error_addr,

  // ── L2 resp ──
  input  logic        stim_l2_resp_valid,
  input  logic        stim_l2_resp_ready,
  input  logic [$bits(et_link_minion_rsp_info_t)-1:0] stim_l2_resp,

  // ── Thread enables ──
  input  logic        stim_thread0_enable,
  input  logic        stim_thread1_enable,

  // ── VPU control inputs ──
  input  logic [$bits(vpu_ctrl_sigs_t)-1:0] stim_id_frontend_vpu_ctrl,
  input  logic [$bits(vpu_id_ctrl_t)-1:0] stim_id_vpu_ctrl,
  input  logic [$bits(vpu_minion_ex_ctrl)-1:0] stim_ex_vpu_ctrl,
  input  logic [$bits(vpu_minion_tag_ctrl)-1:0] stim_tag_vpu_ctrl,
  input  logic [$bits(vpu_minion_mem_ctrl)-1:0] stim_mem_vpu_ctrl,
  input  logic [$bits(vpu_minion_wb_ctrl)-1:0] stim_wb_vpu_ctrl,
  input  logic        stim_vpu_tfma_tenb_working,

  // ── FLB ──
  input  logic        stim_flb_neigh_resp_valid,
  input  logic        stim_flb_neigh_resp_data,

  // ── TE ──
  input  logic        stim_te_thread_sel,

  // ── Debug ──
  input  logic [1:0][63:0] stim_read_ddata0,
  input  logic [1:0]  stim_debug_ex_program_buffer,
  input  logic [1:0]  stim_halt,
  input  logic [1:0]  stim_resume,

  // ── ESR ──
  input  logic        stim_esr_shire_coop_mode,
  input  logic [$bits(esr_minion_features_t)-1:0] stim_esr_features,

  // ── Error bits ──
  input  logic [2:0]  stim_tensor_load_err_flags,
  input  logic [1:0][1:0] stim_cache_ops_err_flags,
  input  logic        stim_tensor_reduce_err_flags,

  // ── PMU ──
  input  logic [1:0][63:0] stim_pmu_read_data,

  // ── Events ──
  input  logic [22:0] stim_io_events_dcache_vpu,

  // ── DCache debug ──
  input  logic [63:0] stim_dcache_debug_signals,

  // ═══════════════════════════════════════════════════════════════════
  // NEW module outputs (key signals for comparison)
  // ═══════════════════════════════════════════════════════════════════
  output logic [1:0]  new_id_fe_req_valid,
  output logic [1:0][$bits(minion_fe_req)-1:0] new_id_fe_req,
  output logic [48:0] new_id_fe_req_pc0,
  output logic [1:0]  new_id_fe_stall,
  output logic        new_id_fe_resp_ready,

  output logic        new_icache_invalidate,
  output logic [$bits(core_dcache_ctrl)-1:0] new_dcache_ctrl,
  output logic [$bits(core_vpu_ctrl)-1:0] new_vpu_ctrl,

  output logic        new_id_dcache_alloc_rq_pre,
  output logic        new_ex_dcache_alloc_rq_val,
  output logic        new_id_dcache_gsc,
  output logic        new_ex_dcache_req_valid,
  output logic [$bits(minion_dcache_req)-1:0] new_ex_dcache_req,
  output logic        new_ex_dcache_gsc,
  output logic        new_tag_dcache_kill,
  output logic [255:0] new_tag_dcache_store_data,
  output logic        new_mem_dcache_kill,
  output logic [63:0] new_wb_dcache_x31,
  output logic        new_wb_dcache_invalidate_lr,
  output logic        new_wb_dcache_fp_toint,

  output logic [$bits(minion_satp_info)-1:0] new_satp_info,
  output logic [$bits(minion_satp_info)-1:0] new_matp_info,
  output logic        new_satp_info_en,
  output logic        new_matp_info_en,
  output logic [1:0][$bits(vm_status_t)-1:0] new_vm_status,
  output logic        new_tlb_invalidate,

  output logic [$bits(minion_vpu_id_req)-1:0] new_id_vpu_req,
  output logic [$bits(minion_vpu_ex_req)-1:0] new_ex_vpu_req,
  output logic        new_tag_vpu_kill,
  output logic        new_mem_vpu_kill,
  output logic        new_wb_vpu_kill,

  output logic        new_wb_vpu_dcache_resp_valid,
  output logic [$bits(dcache_vpu_resp)-1:0] new_wb_vpu_dcache_resp,

  output logic        new_flb_neigh_req_valid,
  output logic [12:0] new_flb_neigh_req_data,

  output logic [31:0] new_TE_wb_reg_inst,
  output logic [63:0] new_TE_wb_reg_cause_ie,
  output logic        new_TE_wb_reg_context,
  output logic [48:0] new_TE_tval,
  output logic [1:0]  new_TE_prv,
  output logic        new_TE_exception,

  output logic [1:0]  new_update_ddata0,
  output logic [63:0] new_ddata0_wdata,
  output logic [1:0]  new_in_debug_mode,
  output logic [1:0]  new_debug_busy,
  output logic [1:0]  new_debug_exception,
  output logic [$bits(intpipe_dbg_monitor_t)-1:0] new_debug_monitor_out,

  output logic        new_wb_valid,
  output logic        new_wb_thread_id,
  output logic [48:0] new_wb_reg_pc,

  output logic [7:0]  new_pmu_count_up,
  output logic [1:0][3:0] new_pmu_read_sel,
  output logic [11:0] new_pmu_write_en,
  output logic [63:0] new_pmu_write_data,
  output logic [19:0] new_pmu_neigh_event_sel,

  // ═══════════════════════════════════════════════════════════════════
  // ORIGINAL module outputs
  // ═══════════════════════════════════════════════════════════════════
  output logic [1:0]  orig_id_fe_req_valid,
  output logic [1:0][$bits(minion_fe_req)-1:0] orig_id_fe_req,
  output logic [48:0] orig_id_fe_req_pc0,
  output logic [1:0]  orig_id_fe_stall,
  output logic        orig_id_fe_resp_ready,

  output logic        orig_icache_invalidate,
  output logic [$bits(core_dcache_ctrl)-1:0] orig_dcache_ctrl,
  output logic [$bits(core_vpu_ctrl)-1:0] orig_vpu_ctrl,

  output logic        orig_id_dcache_alloc_rq_pre,
  output logic        orig_ex_dcache_alloc_rq_val,
  output logic        orig_id_dcache_gsc,
  output logic        orig_ex_dcache_req_valid,
  output logic [$bits(minion_dcache_req)-1:0] orig_ex_dcache_req,
  output logic        orig_ex_dcache_gsc,
  output logic        orig_tag_dcache_kill,
  output logic [255:0] orig_tag_dcache_store_data,
  output logic        orig_mem_dcache_kill,
  output logic [63:0] orig_wb_dcache_x31,
  output logic        orig_wb_dcache_invalidate_lr,
  output logic        orig_wb_dcache_fp_toint,

  output logic [$bits(minion_satp_info)-1:0] orig_satp_info,
  output logic [$bits(minion_satp_info)-1:0] orig_matp_info,
  output logic        orig_satp_info_en,
  output logic        orig_matp_info_en,
  output logic [1:0][$bits(vm_status_t)-1:0] orig_vm_status,
  output logic        orig_tlb_invalidate,

  output logic [$bits(minion_vpu_id_req)-1:0] orig_id_vpu_req,
  output logic [$bits(minion_vpu_ex_req)-1:0] orig_ex_vpu_req,
  output logic        orig_tag_vpu_kill,
  output logic        orig_mem_vpu_kill,
  output logic        orig_wb_vpu_kill,

  output logic        orig_wb_vpu_dcache_resp_valid,
  output logic [$bits(dcache_vpu_resp)-1:0] orig_wb_vpu_dcache_resp,

  output logic        orig_flb_neigh_req_valid,
  output logic [12:0] orig_flb_neigh_req_data,

  output logic [31:0] orig_TE_wb_reg_inst,
  output logic [63:0] orig_TE_wb_reg_cause_ie,
  output logic        orig_TE_wb_reg_context,
  output logic [48:0] orig_TE_tval,
  output logic [1:0]  orig_TE_prv,
  output logic        orig_TE_exception,

  output logic [1:0]  orig_update_ddata0,
  output logic [63:0] orig_ddata0_wdata,
  output logic [1:0]  orig_in_debug_mode,
  output logic [1:0]  orig_debug_busy,
  output logic [1:0]  orig_debug_exception,
  output logic [$bits(intpipe_dbg_monitor_t)-1:0] orig_debug_monitor_out,

  output logic        orig_wb_valid,
  output logic        orig_wb_thread_id,
  output logic [48:0] orig_wb_reg_pc,

  output logic [7:0]  orig_pmu_count_up,
  output logic [1:0][3:0] orig_pmu_read_sel,
  output logic [11:0] orig_pmu_write_en,
  output logic [63:0] orig_pmu_write_data,
  output logic [19:0] orig_pmu_neigh_event_sel
);

  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import intpipe_csr_pkg::*;
  import dft_pkg::*;

  // ── Cast flat stimulus to typed structs ──
  minion_interrupts        interrupts_s;
  assign interrupts_s      = stim_interrupts;

  frontend_core_resp       id_fe_resp_s;
  assign id_fe_resp_s      = stim_id_fe_resp;

  dcache_core_ctrl         dcache_ctrl_resp_s;
  assign dcache_ctrl_resp_s = stim_dcache_ctrl_resp;

  dcache_minion_resp       wb_dcache_resp_s;
  assign wb_dcache_resp_s  = stim_wb_dcache_resp;

  dcache_core_bp_t         tag_dcache_bp_s;
  assign tag_dcache_bp_s   = stim_tag_dcache_bp;

  minion_control_t         decoded_id_ctrl_s;
  minion_control_t         id_ctrl_s;
  assign id_ctrl_s         = stim_use_decoded_id_ctrl ? decoded_id_ctrl_s : stim_id_ctrl;

  dcache_scoreboard_t      id_dcache_scoreboard_s;
  assign id_dcache_scoreboard_s = stim_id_dcache_scoreboard;

  dcache_tag_xcpt          tag_dcache_xcpt_s;
  assign tag_dcache_xcpt_s = stim_tag_dcache_xcpt;

  et_link_minion_rsp_info_t l2_resp_s;
  assign l2_resp_s         = stim_l2_resp;

  vpu_ctrl_sigs_t          id_frontend_vpu_ctrl_s;
  assign id_frontend_vpu_ctrl_s = stim_id_frontend_vpu_ctrl;

  vpu_id_ctrl_t            id_vpu_ctrl_s;
  assign id_vpu_ctrl_s     = stim_id_vpu_ctrl;

  vpu_minion_ex_ctrl       ex_vpu_ctrl_s;
  assign ex_vpu_ctrl_s     = stim_ex_vpu_ctrl;

  vpu_minion_tag_ctrl      tag_vpu_ctrl_s;
  assign tag_vpu_ctrl_s    = stim_tag_vpu_ctrl;

  vpu_minion_mem_ctrl      mem_vpu_ctrl_s;
  assign mem_vpu_ctrl_s    = stim_mem_vpu_ctrl;

  vpu_minion_wb_ctrl       wb_vpu_ctrl_s;
  assign wb_vpu_ctrl_s     = stim_wb_vpu_ctrl;

  esr_minion_features_t    esr_features_s;
  assign esr_features_s    = stim_esr_features;

  // ── Typed outputs from new module ──
  core_dcache_ctrl         new_dcache_ctrl_s;
  core_vpu_ctrl            new_vpu_ctrl_s;
  minion_fe_req [1:0]      new_id_fe_req_s;
  minion_dcache_req        new_ex_dcache_req_s;
  minion_satp_info         new_satp_info_s;
  minion_satp_info         new_matp_info_s;
  vm_status_t [1:0]        new_vm_status_s;
  minion_vpu_id_req        new_id_vpu_req_s;
  minion_vpu_ex_req        new_ex_vpu_req_s;
  dcache_vpu_resp          new_wb_vpu_dcache_resp_s;
  csr_cause_t              new_TE_wb_reg_cause_ie_s;
  intpipe_dbg_monitor_t    new_debug_monitor_out_s;

  assign new_dcache_ctrl     = new_dcache_ctrl_s;
  assign new_vpu_ctrl        = new_vpu_ctrl_s;
  assign new_id_fe_req       = new_id_fe_req_s;
  assign new_id_fe_req_pc0   = new_id_fe_req_s[0].pc;
  assign new_ex_dcache_req   = new_ex_dcache_req_s;
  assign new_satp_info       = new_satp_info_s;
  assign new_matp_info       = new_matp_info_s;
  assign new_vm_status       = new_vm_status_s;
  assign new_id_vpu_req      = new_id_vpu_req_s;
  assign new_ex_vpu_req      = new_ex_vpu_req_s;
  assign new_wb_vpu_dcache_resp = new_wb_vpu_dcache_resp_s;
  assign new_TE_wb_reg_cause_ie = new_TE_wb_reg_cause_ie_s;
  assign new_debug_monitor_out = new_debug_monitor_out_s;

  // ── Typed outputs from original module ──
  core_dcache_ctrl         orig_dcache_ctrl_s;
  core_vpu_ctrl            orig_vpu_ctrl_s;
  minion_fe_req [1:0]      orig_id_fe_req_s;
  minion_dcache_req        orig_ex_dcache_req_s;
  minion_satp_info         orig_satp_info_s;
  minion_satp_info         orig_matp_info_s;
  vm_status_t [1:0]        orig_vm_status_s;
  minion_vpu_id_req        orig_id_vpu_req_s;
  minion_vpu_ex_req        orig_ex_vpu_req_s;
  dcache_vpu_resp          orig_wb_vpu_dcache_resp_s;
  csr_cause_t              orig_TE_wb_reg_cause_ie_s;
  intpipe_dbg_monitor_t    orig_debug_monitor_out_s;

  assign orig_dcache_ctrl     = orig_dcache_ctrl_s;
  assign orig_vpu_ctrl        = orig_vpu_ctrl_s;
  assign orig_id_fe_req       = orig_id_fe_req_s;
  assign orig_id_fe_req_pc0   = orig_id_fe_req_s[0].pc;
  assign orig_ex_dcache_req   = orig_ex_dcache_req_s;
  assign orig_satp_info       = orig_satp_info_s;
  assign orig_matp_info       = orig_matp_info_s;
  assign orig_vm_status       = orig_vm_status_s;
  assign orig_id_vpu_req      = orig_id_vpu_req_s;
  assign orig_ex_vpu_req      = orig_ex_vpu_req_s;
  assign orig_wb_vpu_dcache_resp = orig_wb_vpu_dcache_resp_s;
  assign orig_TE_wb_reg_cause_ie = orig_TE_wb_reg_cause_ie_s;
  assign orig_debug_monitor_out = orig_debug_monitor_out_s;

  intpipe_decode u_decode (
    .inst_bits (id_fe_resp_s.inst_bits),
    .inst_ctrl (decoded_id_ctrl_s)
  );

  // ──────────────────────────────────────────────────────────────────
  // NEW module (SystemVerilog, active-low async resets)
  // ──────────────────────────────────────────────────────────────────
  intpipe_top u_new (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .rst_w_ni                 (rst_w_ni),
    .rst_d_ni                 (rst_d_ni),
    .te_enable                (stim_te_enable),
    .dft__reset_byp           (stim_dft_reset_byp),
    .dft__reset               (stim_dft_reset),
    .interrupts               (interrupts_s),
    .shire_id                 (stim_shire_id),
    .shire_min_id             (stim_shire_min_id),
    .chicken_bit_intpipe      (stim_chicken_bit_intpipe),

    .id_fe_req_valid          (new_id_fe_req_valid),
    .id_fe_req                (new_id_fe_req_s),
    .id_fe_stall              (new_id_fe_stall),
    .id_fe_resp_ready         (new_id_fe_resp_ready),
    .id_fe_resp_valid         (stim_id_fe_resp_valid),
    .id_fe_resp_thread_id     (stim_id_fe_resp_thread_id),
    .id_fe_resp               (id_fe_resp_s),

    .icache_invalidate        (new_icache_invalidate),
    .dcache_ctrl              (new_dcache_ctrl_s),
    .dcache_ctrl_resp         (dcache_ctrl_resp_s),
    .vpu_ctrl                 (new_vpu_ctrl_s),

    .id_dcache_alloc_rq_pre   (new_id_dcache_alloc_rq_pre),
    .ex_dcache_alloc_rq_val   (new_ex_dcache_alloc_rq_val),
    .id_dcache_gsc            (new_id_dcache_gsc),
    .id_dcache_ready          (stim_id_dcache_ready),
    .ex_dcache_req_valid      (new_ex_dcache_req_valid),
    .ex_dcache_req            (new_ex_dcache_req_s),
    .ex_dcache_gsc            (new_ex_dcache_gsc),
    .tag_dcache_kill          (new_tag_dcache_kill),
    .tag_dcache_store_data    (new_tag_dcache_store_data),
    .mem_dcache_kill          (new_mem_dcache_kill),
    .wb_dcache_x31            (new_wb_dcache_x31),

    .mem_dcache_resp_int_valid(stim_mem_dcache_resp_int_valid),
    .wb_dcache_resp_valid     (stim_wb_dcache_resp_valid),
    .wb_dcache_resp           (wb_dcache_resp_s),
    .tag_dcache_bp            (tag_dcache_bp_s),
    .tag_dcache_bp_valid      (stim_tag_dcache_bp_valid),

    .id_ctrl                  (id_ctrl_s),
    .id_dcache_ordered        (stim_id_dcache_ordered),
    .id_dcache_idle           (stim_id_dcache_idle),
    .id_dcache_scoreboard     (id_dcache_scoreboard_s),
    .id_dcache_sb_int_dealloc (stim_id_dcache_sb_int_dealloc),
    .id_dcache_sb_fp_dealloc  (stim_id_dcache_sb_fp_dealloc),
    .tag_dcache_xcpt          (tag_dcache_xcpt_s),
    .tag_dcache_replay_next   (stim_tag_dcache_replay_next),
    .mem_dcache_flush_pipeline(stim_mem_dcache_flush_pipeline),
    .wb_dcache_invalidate_lr  (new_wb_dcache_invalidate_lr),
    .wb_dcache_fp_toint       (new_wb_dcache_fp_toint),
    .dcache_bus_error         (stim_dcache_bus_error),
    .dcache_bus_error_addr    (stim_dcache_bus_error_addr),

    .satp_info                (new_satp_info_s),
    .matp_info                (new_matp_info_s),
    .satp_info_en             (new_satp_info_en),
    .matp_info_en             (new_matp_info_en),
    .vm_status                (new_vm_status_s),
    .tlb_invalidate           (new_tlb_invalidate),

    .l2_resp_valid            (stim_l2_resp_valid),
    .l2_resp_ready            (stim_l2_resp_ready),
    .l2_resp                  (l2_resp_s),

    .id_vpu_req               (new_id_vpu_req_s),
    .ex_vpu_req               (new_ex_vpu_req_s),
    .tag_vpu_kill             (new_tag_vpu_kill),
    .mem_vpu_kill             (new_mem_vpu_kill),
    .wb_vpu_kill              (new_wb_vpu_kill),

    .wb_vpu_dcache_resp_valid (new_wb_vpu_dcache_resp_valid),
    .wb_vpu_dcache_resp       (new_wb_vpu_dcache_resp_s),

    .thread0_enable           (stim_thread0_enable),
    .thread1_enable           (stim_thread1_enable),

    .id_frontend_vpu_ctrl     (id_frontend_vpu_ctrl_s),
    .id_vpu_ctrl              (id_vpu_ctrl_s),
    .ex_vpu_ctrl              (ex_vpu_ctrl_s),
    .tag_vpu_ctrl             (tag_vpu_ctrl_s),
    .mem_vpu_ctrl             (mem_vpu_ctrl_s),
    .wb_vpu_ctrl              (wb_vpu_ctrl_s),
    .vpu_tfma_tenb_working    (stim_vpu_tfma_tenb_working),

    .flb_neigh_req_valid      (new_flb_neigh_req_valid),
    .flb_neigh_req_data       (new_flb_neigh_req_data),
    .flb_neigh_resp_valid     (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data      (stim_flb_neigh_resp_data),

    .TE_wb_reg_inst           (new_TE_wb_reg_inst),
    .TE_wb_reg_cause_ie       (new_TE_wb_reg_cause_ie_s),
    .TE_wb_reg_context        (new_TE_wb_reg_context),
    .TE_tval                  (new_TE_tval),
    .TE_prv                   (new_TE_prv),
    .TE_exception             (new_TE_exception),
    .te_thread_sel            (stim_te_thread_sel),

    .update_ddata0            (new_update_ddata0),
    .ddata0_wdata             (new_ddata0_wdata),
    .read_ddata0              (stim_read_ddata0),
    .debug_ex_program_buffer  (stim_debug_ex_program_buffer),
    .halt                     (stim_halt),
    .resume                   (stim_resume),
    .in_debug_mode            (new_in_debug_mode),
    .debug_busy               (new_debug_busy),
    .debug_exception          (new_debug_exception),
    .debug_monitor_out        (new_debug_monitor_out_s),

    .wb_valid                 (new_wb_valid),
    .wb_thread_id             (new_wb_thread_id),
    .wb_reg_pc                (new_wb_reg_pc),

    .esr_shire_coop_mode      (stim_esr_shire_coop_mode),
    .esr_features             (esr_features_s),

    .tensor_load_err_flags    (stim_tensor_load_err_flags),
    .cache_ops_err_flags      (stim_cache_ops_err_flags),
    .tensor_reduce_err_flags  (stim_tensor_reduce_err_flags),

    .pmu_count_up             (new_pmu_count_up),
    .pmu_read_data            (stim_pmu_read_data),
    .pmu_read_sel             (new_pmu_read_sel),
    .pmu_write_en             (new_pmu_write_en),
    .pmu_write_data           (new_pmu_write_data),
    .pmu_neigh_event_sel      (new_pmu_neigh_event_sel),

    .io_events_dcache_vpu     (stim_io_events_dcache_vpu),
    .dcache_debug_signals     (stim_dcache_debug_signals)
  );

  // ──────────────────────────────────────────────────────────────────
  // ORIGINAL module (Verilog, active-high synchronous resets)
  // ──────────────────────────────────────────────────────────────────
  logic orig_reset_c, orig_reset_w, orig_reset_d;
  assign orig_reset_c = ~rst_ni;
  assign orig_reset_w = ~rst_w_ni;
  assign orig_reset_d = ~rst_d_ni;

  intpipe_top_orig u_orig (
    .clock                    (clk_i),
    .reset_c                  (orig_reset_c),
    .reset_w                  (orig_reset_w),
    .reset_d                  (orig_reset_d),
    .te_enable                (stim_te_enable),
    .dft__reset_byp           (stim_dft_reset_byp),
    .dft__reset               (stim_dft_reset),
    .interrupts               (interrupts_s),
    .shire_id                 (stim_shire_id),
    .shire_min_id             (stim_shire_min_id),
    .chicken_bit_intpipe      (stim_chicken_bit_intpipe),

    .id_fe_req_valid          (orig_id_fe_req_valid),
    .id_fe_req                (orig_id_fe_req_s),
    .id_fe_stall              (orig_id_fe_stall),
    .id_fe_resp_ready         (orig_id_fe_resp_ready),
    .id_fe_resp_valid         (stim_id_fe_resp_valid),
    .id_fe_resp_thread_id     (stim_id_fe_resp_thread_id),
    .id_fe_resp               (id_fe_resp_s),

    .icache_invalidate        (orig_icache_invalidate),
    .dcache_ctrl              (orig_dcache_ctrl_s),
    .dcache_ctrl_resp         (dcache_ctrl_resp_s),
    .vpu_ctrl                 (orig_vpu_ctrl_s),

    .id_dcache_alloc_rq_pre   (orig_id_dcache_alloc_rq_pre),
    .ex_dcache_alloc_rq_val   (orig_ex_dcache_alloc_rq_val),
    .id_dcache_gsc            (orig_id_dcache_gsc),
    .id_dcache_ready          (stim_id_dcache_ready),
    .ex_dcache_req_valid      (orig_ex_dcache_req_valid),
    .ex_dcache_req            (orig_ex_dcache_req_s),
    .ex_dcache_gsc            (orig_ex_dcache_gsc),
    .tag_dcache_kill          (orig_tag_dcache_kill),
    .tag_dcache_store_data    (orig_tag_dcache_store_data),
    .mem_dcache_kill          (orig_mem_dcache_kill),
    .wb_dcache_x31            (orig_wb_dcache_x31),

    .mem_dcache_resp_int_valid(stim_mem_dcache_resp_int_valid),
    .wb_dcache_resp_valid     (stim_wb_dcache_resp_valid),
    .wb_dcache_resp           (wb_dcache_resp_s),
    .tag_dcache_bp            (tag_dcache_bp_s),
    .tag_dcache_bp_valid      (stim_tag_dcache_bp_valid),

    .id_ctrl                  (id_ctrl_s),
    .id_dcache_ordered        (stim_id_dcache_ordered),
    .id_dcache_idle           (stim_id_dcache_idle),
    .id_dcache_scoreboard     (id_dcache_scoreboard_s),
    .id_dcache_sb_int_dealloc (stim_id_dcache_sb_int_dealloc),
    .id_dcache_sb_fp_dealloc  (stim_id_dcache_sb_fp_dealloc),
    .tag_dcache_xcpt          (tag_dcache_xcpt_s),
    .tag_dcache_replay_next   (stim_tag_dcache_replay_next),
    .mem_dcache_flush_pipeline(stim_mem_dcache_flush_pipeline),
    .wb_dcache_invalidate_lr  (orig_wb_dcache_invalidate_lr),
    .wb_dcache_fp_toint       (orig_wb_dcache_fp_toint),
    .dcache_bus_error         (stim_dcache_bus_error),
    .dcache_bus_error_addr    (stim_dcache_bus_error_addr),

    .satp_info                (orig_satp_info_s),
    .matp_info                (orig_matp_info_s),
    .satp_info_en             (orig_satp_info_en),
    .matp_info_en             (orig_matp_info_en),
    .vm_status                (orig_vm_status_s),
    .tlb_invalidate           (orig_tlb_invalidate),

    .l2_resp_valid            (stim_l2_resp_valid),
    .l2_resp_ready            (stim_l2_resp_ready),
    .l2_resp                  (l2_resp_s),

    .id_vpu_req               (orig_id_vpu_req_s),
    .ex_vpu_req               (orig_ex_vpu_req_s),
    .tag_vpu_kill             (orig_tag_vpu_kill),
    .mem_vpu_kill             (orig_mem_vpu_kill),
    .wb_vpu_kill              (orig_wb_vpu_kill),

    .wb_vpu_dcache_resp_valid (orig_wb_vpu_dcache_resp_valid),
    .wb_vpu_dcache_resp       (orig_wb_vpu_dcache_resp_s),

    .thread0_enable           (stim_thread0_enable),
    .thread1_enable           (stim_thread1_enable),

    .id_frontend_vpu_ctrl     (id_frontend_vpu_ctrl_s),
    .id_vpu_ctrl              (id_vpu_ctrl_s),
    .ex_vpu_ctrl              (ex_vpu_ctrl_s),
    .tag_vpu_ctrl             (tag_vpu_ctrl_s),
    .mem_vpu_ctrl             (mem_vpu_ctrl_s),
    .wb_vpu_ctrl              (wb_vpu_ctrl_s),
    .vpu_tfma_tenb_working    (stim_vpu_tfma_tenb_working),

    .flb_neigh_req_valid      (orig_flb_neigh_req_valid),
    .flb_neigh_req_data       (orig_flb_neigh_req_data),
    .flb_neigh_resp_valid     (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data      (stim_flb_neigh_resp_data),

    .TE_wb_reg_inst           (orig_TE_wb_reg_inst),
    .TE_wb_reg_cause_ie       (orig_TE_wb_reg_cause_ie_s),
    .TE_wb_reg_context        (orig_TE_wb_reg_context),
    .TE_tval                  (orig_TE_tval),
    .TE_prv                   (orig_TE_prv),
    .TE_exception             (orig_TE_exception),
    .te_thread_sel            (stim_te_thread_sel),

    .update_ddata0            (orig_update_ddata0),
    .ddata0_wdata             (orig_ddata0_wdata),
    .read_ddata0              (stim_read_ddata0),
    .debug_ex_program_buffer  (stim_debug_ex_program_buffer),
    .halt                     (stim_halt),
    .resume                   (stim_resume),
    .in_debug_mode            (orig_in_debug_mode),
    .debug_busy               (orig_debug_busy),
    .debug_exception          (orig_debug_exception),
    .debug_monitor_out        (orig_debug_monitor_out_s),

    .wb_valid                 (orig_wb_valid),
    .wb_thread_id             (orig_wb_thread_id),
    .wb_reg_pc                (orig_wb_reg_pc),

    .esr_shire_coop_mode      (stim_esr_shire_coop_mode),
    .esr_features             (esr_features_s),

    .tensor_load_err_flags    (stim_tensor_load_err_flags),
    .cache_ops_err_flags      (stim_cache_ops_err_flags),
    .tensor_reduce_err_flags  (stim_tensor_reduce_err_flags),

    .pmu_count_up             (orig_pmu_count_up),
    .pmu_read_data            (stim_pmu_read_data),
    .pmu_read_sel             (orig_pmu_read_sel),
    .pmu_write_en             (orig_pmu_write_en),
    .pmu_write_data           (orig_pmu_write_data),
    .pmu_neigh_event_sel      (orig_pmu_neigh_event_sel),

    .io_events_dcache_vpu     (stim_io_events_dcache_vpu),
    .dcache_debug_signals     (stim_dcache_debug_signals)
  );

endmodule
