// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_ctrl vs new rbox_ctrl.
//
// Both clock domains (clock, clock_rbox) are driven from clk_i for cosim.
// The original module and its sub-modules (rbox_input_fifo, rbox_output_buffer,
// rbox_fifo, gen_mem1p) are inlined with _orig suffix to avoid name collision
// with the new SystemVerilog implementations.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off MULTIDRIVEN */
/* verilator lint_off BLKSEQ */

module cosim_rbox_ctrl_tb (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  // ── Shared stimulus: SC request ready ────────────────
  input  logic [4:0]              rbox_sc_req_ready_i,

  // ── Shared stimulus: SC response ─────────────────────
  input  logic [533:0]            sc_rbox_rsp_i,         // etlink_pkg::rsp_t = 534 bits
  input  logic                    sc_rbox_rsp_valid_i,

  // ── Shared stimulus: Depth Buffer SC request ─────────
  input  logic [583:0]            db_sc_req_i,           // etlink_pkg::req_t = 584 bits
  input  logic [4:0]              db_sc_req_valid_i,

  // ── Shared stimulus: ESR registers ───────────────────
  input  logic [63:0]             esr_config_i,
  input  logic                    esr_config_en_i,
  input  logic [63:0]             esr_in_buf_pg_i,
  input  logic                    esr_in_buf_pg_en_i,
  input  logic [63:0]             esr_in_buf_cfg_i,
  input  logic                    esr_in_buf_cfg_en_i,
  input  logic [31:0]             esr_out_buf_pg_i,
  input  logic                    esr_out_buf_pg_en_i,
  input  logic [63:0]             esr_out_buf_cfg_i,
  input  logic                    esr_out_buf_cfg_en_i,
  input  logic [31:0]             esr_start_i,
  input  logic                    esr_start_en_i,
  input  logic [31:0]             esr_consume_i,
  input  logic                    esr_consume_en_i,

  // ── Shared stimulus: traverse accept/empty ───────────
  input  logic                    tr_accept_i,
  input  logic                    tr_empty_i,

  // ── Shared stimulus: traverse quad output ────────────
  input  logic [779:0]            tr_quad_i,             // rbox_pkg::quad_data_t
  input  logic                    tr_quad_valid_i,

  // ── Shared stimulus: test empty ──────────────────────
  input  logic                    ts_empty_i,

  // ── Shared stimulus: test result ─────────────────────
  input  logic [3:0]              ts_result_mask_i,
  input  logic                    ts_result_valid_i,

  // ── Shared stimulus: flush done ──────────────────────
  input  logic                    db_flush_done_i,

  // ── Shared stimulus: debug halted ────────────────────
  input  logic                    rbox_halted_i,

  // ── New module outputs ───────────────────────────────
  output logic                    new_rbox_enable_o,
  output logic [583:0]            new_rbox_sc_req_o,
  output logic [4:0]              new_rbox_sc_req_valid_o,
  output logic                    new_sc_rbox_rsp_ready_o,
  output logic                    new_db_sc_req_ready_o,
  output logic [533:0]            new_sc_db_rsp_o,
  output logic                    new_sc_db_rsp_valid_o,
  output logic [31:0]             new_esr_status_o,
  output logic                    new_tr_msaa_enable_o,
  output logic [1:0]              new_tr_msaa_mode_o,
  output logic                    new_tr_depth_is_fp32_o,
  output logic                    new_tr_depth_is_un16_o,
  output logic                    new_tr_depth_is_un24_o,
  output logic [14:0]             new_tr_scissor_min_x_o,
  output logic [14:0]             new_tr_scissor_max_x_o,
  output logic [14:0]             new_tr_scissor_min_y_o,
  output logic [14:0]             new_tr_scissor_max_y_o,
  output logic                    new_tr_depth_clamp_enable_o,
  output logic [31:0]             new_tr_depth_min_o,
  output logic [31:0]             new_tr_depth_max_o,
  output logic                    new_tr_depth_bound_enable_o,
  output logic [31:0]             new_tr_depth_bound_min_o,
  output logic [31:0]             new_tr_depth_bound_max_o,
  output logic                    new_tr_tile_valid_o,
  output logic                    new_ct_quad_accept_o,
  output logic                    new_ct_result_accept_o,
  output logic                    new_ts_front_triangle_o,
  output logic                    new_db_flush_o,

  // ── Original module outputs ──────────────────────────
  output logic                    orig_rbox_enable_o,
  output logic [583:0]            orig_rbox_sc_req_o,
  output logic [4:0]              orig_rbox_sc_req_valid_o,
  output logic                    orig_sc_rbox_rsp_ready_o,
  output logic                    orig_db_sc_req_ready_o,
  output logic [533:0]            orig_sc_db_rsp_o,
  output logic                    orig_sc_db_rsp_valid_o,
  output logic [31:0]             orig_esr_status_o,
  output logic                    orig_tr_msaa_enable_o,
  output logic [1:0]              orig_tr_msaa_mode_o,
  output logic                    orig_tr_depth_is_fp32_o,
  output logic                    orig_tr_depth_is_un16_o,
  output logic                    orig_tr_depth_is_un24_o,
  output logic [14:0]             orig_tr_scissor_min_x_o,
  output logic [14:0]             orig_tr_scissor_max_x_o,
  output logic [14:0]             orig_tr_scissor_min_y_o,
  output logic [14:0]             orig_tr_scissor_max_y_o,
  output logic                    orig_tr_depth_clamp_enable_o,
  output logic [31:0]             orig_tr_depth_min_o,
  output logic [31:0]             orig_tr_depth_max_o,
  output logic                    orig_tr_depth_bound_enable_o,
  output logic [31:0]             orig_tr_depth_bound_min_o,
  output logic [31:0]             orig_tr_depth_bound_max_o,
  output logic                    orig_tr_tile_valid_o,
  output logic                    orig_ct_quad_accept_o,
  output logic                    orig_ct_result_accept_o,
  output logic                    orig_ts_front_triangle_o,
  output logic                    orig_db_flush_o
);

  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  // ── Adapt reset ──────────────────────────────────────
  logic reset;
  assign reset = ~rst_ni;

  // ── Adapt ET-Link types ──────────────────────────────
  // New types
  etlink_pkg::rsp_t new_sc_rbox_rsp;
  assign new_sc_rbox_rsp = etlink_pkg::rsp_t'(sc_rbox_rsp_i);

  etlink_pkg::req_t new_db_sc_req;
  assign new_db_sc_req = etlink_pkg::req_t'(db_sc_req_i);

  etlink_pkg::req_t new_rbox_sc_req;
  assign new_rbox_sc_req_o = new_rbox_sc_req;

  etlink_pkg::rsp_t new_sc_db_rsp;
  assign new_sc_db_rsp_o = new_sc_db_rsp;

  // Original types
  et_link_rsp_info_t orig_sc_rbox_rsp;
  assign orig_sc_rbox_rsp = et_link_rsp_info_t'(sc_rbox_rsp_i);

  et_link_req_info_t orig_db_sc_req;
  assign orig_db_sc_req = et_link_req_info_t'(db_sc_req_i);

  et_link_req_info_t orig_rbox_sc_req;
  assign orig_rbox_sc_req_o = orig_rbox_sc_req;

  et_link_rsp_info_t orig_sc_db_rsp;
  assign orig_sc_db_rsp_o = orig_sc_db_rsp;

  // ── Adapt quad_data_t ────────────────────────────────
  rbox_pkg::quad_data_t new_tr_quad;
  assign new_tr_quad = rbox_pkg::quad_data_t'(tr_quad_i);

  rbox_quad_data_t orig_tr_quad;
  assign orig_tr_quad = rbox_quad_data_t'(tr_quad_i);

  // ── Unused outputs from new (packed structs) ─────────
  rbox_pkg::triangle_state_t new_tr_triangle;
  rbox_pkg::tile_state_t     new_tr_tile;
  rbox_pkg::state_t          new_ts_state;
  rbox_pkg::sm_ctrl_t        new_ct_sm_data;
  rbox_pkg::msaa_mode_e      new_tr_msaa_mode;
  assign new_tr_msaa_mode_o = new_tr_msaa_mode;

  // ── Unused outputs from original (packed structs) ────
  rbox_triangle_state_t      orig_tr_triangle;
  rbox_tile_state_t          orig_tr_tile;
  rbox_state_t               orig_ts_state;
  rbox_sm_ctrl_t             orig_ct_sm_data;
  rbox_msaa_mode_t           orig_tr_msaa_mode;
  assign orig_tr_msaa_mode_o = orig_tr_msaa_mode;

  // ── RAM config (disabled) ────────────────────────────
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // ════════════════════════════════════════════════════════════
  // New module instance
  // ════════════════════════════════════════════════════════════

  rbox_ctrl u_new (
    .clk_i                   (clk_i),
    .clk_rbox_i              (clk_i),
    .rst_ni                  (rst_ni),
    .ram_cfg_i               (ram_cfg_t'('0)),
    .dft_i                   (dft_t'('0)),

    .rbox_enable_o           (new_rbox_enable_o),

    .rbox_sc_req_o           (new_rbox_sc_req),
    .rbox_sc_req_valid_o     (new_rbox_sc_req_valid_o),
    .rbox_sc_req_ready_i     (rbox_sc_req_ready_i),

    .sc_rbox_rsp_i           (new_sc_rbox_rsp),
    .sc_rbox_rsp_valid_i     (sc_rbox_rsp_valid_i),
    .sc_rbox_rsp_ready_o     (new_sc_rbox_rsp_ready_o),

    .db_sc_req_i             (new_db_sc_req),
    .db_sc_req_valid_i       (db_sc_req_valid_i),
    .db_sc_req_ready_o       (new_db_sc_req_ready_o),

    .sc_db_rsp_o             (new_sc_db_rsp),
    .sc_db_rsp_valid_o       (new_sc_db_rsp_valid_o),

    .esr_config_i            (esr_config_i),
    .esr_config_en_i         (esr_config_en_i),
    .esr_in_buf_pg_i         (esr_in_buf_pg_i),
    .esr_in_buf_pg_en_i      (esr_in_buf_pg_en_i),
    .esr_in_buf_cfg_i        (esr_in_buf_cfg_i),
    .esr_in_buf_cfg_en_i     (esr_in_buf_cfg_en_i),
    .esr_out_buf_pg_i        (esr_out_buf_pg_i),
    .esr_out_buf_pg_en_i     (esr_out_buf_pg_en_i),
    .esr_out_buf_cfg_i       (esr_out_buf_cfg_i),
    .esr_out_buf_cfg_en_i    (esr_out_buf_cfg_en_i),
    .esr_start_i             (esr_start_i),
    .esr_start_en_i          (esr_start_en_i),
    .esr_consume_i           (esr_consume_i),
    .esr_consume_en_i        (esr_consume_en_i),
    .esr_status_o            (new_esr_status_o),

    .tr_msaa_enable_o        (new_tr_msaa_enable_o),
    .tr_msaa_mode_o          (new_tr_msaa_mode),
    .tr_depth_is_fp32_o      (new_tr_depth_is_fp32_o),
    .tr_depth_is_un16_o      (new_tr_depth_is_un16_o),
    .tr_depth_is_un24_o      (new_tr_depth_is_un24_o),
    .tr_scissor_min_x_o      (new_tr_scissor_min_x_o),
    .tr_scissor_max_x_o      (new_tr_scissor_max_x_o),
    .tr_scissor_min_y_o      (new_tr_scissor_min_y_o),
    .tr_scissor_max_y_o      (new_tr_scissor_max_y_o),
    .tr_depth_clamp_enable_o (new_tr_depth_clamp_enable_o),
    .tr_depth_min_o          (new_tr_depth_min_o),
    .tr_depth_max_o          (new_tr_depth_max_o),
    .tr_depth_bound_enable_o (new_tr_depth_bound_enable_o),
    .tr_depth_bound_min_o    (new_tr_depth_bound_min_o),
    .tr_depth_bound_max_o    (new_tr_depth_bound_max_o),

    .tr_triangle_o           (new_tr_triangle),
    .tr_tile_o               (new_tr_tile),
    .tr_tile_valid_o         (new_tr_tile_valid_o),
    .tr_accept_i             (tr_accept_i),
    .tr_empty_i              (tr_empty_i),

    .tr_quad_i               (new_tr_quad),
    .tr_quad_valid_i         (tr_quad_valid_i),
    .ct_quad_accept_o        (new_ct_quad_accept_o),

    .ts_state_o              (new_ts_state),
    .ts_front_triangle_o     (new_ts_front_triangle_o),

    .ts_empty_i              (ts_empty_i),

    .ts_result_mask_i        (ts_result_mask_i),
    .ts_result_valid_i       (ts_result_valid_i),
    .ct_result_accept_o      (new_ct_result_accept_o),

    .db_flush_o              (new_db_flush_o),
    .db_flush_done_i         (db_flush_done_i),

    .ct_sm_data_o            (new_ct_sm_data),

    .rbox_halted_i           (rbox_halted_i)
  );

  // ════════════════════════════════════════════════════════════
  // Original module instance (renamed copy inlined below)
  // ════════════════════════════════════════════════════════════

  rbox_ctrl_orig u_orig (
    .clock                   (clk_i),
    .clock_rbox              (clk_i),
    .reset                   (reset),
    .esr_shire_cache_ram_cfg (orig_ram_cfg),

    .rbox_enable             (orig_rbox_enable_o),

    .rbox_sc_req             (orig_rbox_sc_req),
    .rbox_sc_req_valid       (orig_rbox_sc_req_valid_o),
    .rbox_sc_req_ready       (rbox_sc_req_ready_i),

    .sc_rbox_rsp             (orig_sc_rbox_rsp),
    .sc_rbox_rsp_valid       (sc_rbox_rsp_valid_i),
    .sc_rbox_rsp_ready       (orig_sc_rbox_rsp_ready_o),

    .db_sc_req               (orig_db_sc_req),
    .db_sc_req_valid         (db_sc_req_valid_i),
    .db_sc_req_ready         (orig_db_sc_req_ready_o),

    .sc_db_rsp               (orig_sc_db_rsp),
    .sc_db_rsp_valid         (orig_sc_db_rsp_valid_o),

    .rbox_esr_config         (esr_config_i),
    .rbox_esr_config_en      (esr_config_en_i),
    .rbox_esr_in_buf_pg      (esr_in_buf_pg_i),
    .rbox_esr_in_buf_pg_en   (esr_in_buf_pg_en_i),
    .rbox_esr_in_buf_cfg     (esr_in_buf_cfg_i),
    .rbox_esr_in_buf_cfg_en  (esr_in_buf_cfg_en_i),
    .rbox_esr_out_buf_pg     (esr_out_buf_pg_i),
    .rbox_esr_out_buf_pg_en  (esr_out_buf_pg_en_i),
    .rbox_esr_out_buf_cfg    (esr_out_buf_cfg_i),
    .rbox_esr_out_buf_cfg_en (esr_out_buf_cfg_en_i),
    .rbox_esr_start          (esr_start_i),
    .rbox_esr_start_en       (esr_start_en_i),
    .rbox_esr_consume        (esr_consume_i),
    .rbox_esr_consume_en     (esr_consume_en_i),
    .rbox_esr_status         (orig_esr_status_o),

    .tr_msaa_enable          (orig_tr_msaa_enable_o),
    .tr_msaa_mode            (orig_tr_msaa_mode),
    .tr_depth_is_fp32        (orig_tr_depth_is_fp32_o),
    .tr_depth_is_un16        (orig_tr_depth_is_un16_o),
    .tr_depth_is_un24        (orig_tr_depth_is_un24_o),
    .tr_scissor_min_x        (orig_tr_scissor_min_x_o),
    .tr_scissor_max_x        (orig_tr_scissor_max_x_o),
    .tr_scissor_min_y        (orig_tr_scissor_min_y_o),
    .tr_scissor_max_y        (orig_tr_scissor_max_y_o),
    .tr_depth_clamp_enable   (orig_tr_depth_clamp_enable_o),
    .tr_depth_min            (orig_tr_depth_min_o),
    .tr_depth_max            (orig_tr_depth_max_o),
    .tr_depth_bound_enable   (orig_tr_depth_bound_enable_o),
    .tr_depth_bound_min      (orig_tr_depth_bound_min_o),
    .tr_depth_bound_max      (orig_tr_depth_bound_max_o),

    .tr_triangle             (orig_tr_triangle),
    .tr_tile                 (orig_tr_tile),
    .tr_tile_valid           (orig_tr_tile_valid_o),
    .tr_accept               (tr_accept_i),
    .tr_empty                (tr_empty_i),

    .tr_quad                 (orig_tr_quad),
    .tr_quad_valid           (tr_quad_valid_i),
    .ct_quad_accept          (orig_ct_quad_accept_o),

    .ts_state                (orig_ts_state),
    .ts_front_triangle       (orig_ts_front_triangle_o),

    .ts_empty                (ts_empty_i),

    .ts_result_mask          (ts_result_mask_i),
    .ts_result_valid         (ts_result_valid_i),
    .ct_result_accept        (orig_ct_result_accept_o),

    .db_flush                (orig_db_flush_o),
    .db_flush_done           (db_flush_done_i),

    .ct_sm_data              (orig_ct_sm_data),

    .rbox_halted             (rbox_halted_i)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Renamed copy of original rbox_ctrl (from rbox_ctrl.v)
// Sub-module instantiations renamed to _orig variants which
// are resolved via -y library paths.
// ════════════════════════════════════════════════════════════

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off MULTIDRIVEN */
/* verilator lint_off BLKSEQ */
/* verilator lint_off CASEINCOMPLETE */

module rbox_ctrl_orig
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
    input logic                           clock,
    input logic                           clock_rbox,
    input logic                           reset,

    // Configuration
    input esr_shire_cache_ram_cfg_t       esr_shire_cache_ram_cfg,

    // Global enable for RBOX clock, ESR driven
    output logic                          rbox_enable,

    // Request to Shire Cache
    output et_link_req_info_t             rbox_sc_req,
    output logic [(`SC_BANKS+1)-1:0]      rbox_sc_req_valid,
    input  logic [(`SC_BANKS+1)-1:0]      rbox_sc_req_ready,

    // Response from Shire Cache
    input  et_link_rsp_info_t             sc_rbox_rsp,
    input  logic                          sc_rbox_rsp_valid,
    output logic                          sc_rbox_rsp_ready,

    // Request from Depth Buffer to Shire Cache
    input  et_link_req_info_t             db_sc_req,
    input  logic [(`SC_BANKS+1)-1:0]      db_sc_req_valid,
    output logic                          db_sc_req_ready,

    // Response from Shire Cache to Depth Buffer
    output et_link_rsp_info_t             sc_db_rsp,
    output logic                          sc_db_rsp_valid,

    // RBOX ESRs
    input  logic [63:0]                   rbox_esr_config,
    input  logic                          rbox_esr_config_en,
    input  logic [63:0]                   rbox_esr_in_buf_pg,
    input  logic                          rbox_esr_in_buf_pg_en,
    input  logic [63:0]                   rbox_esr_in_buf_cfg,
    input  logic                          rbox_esr_in_buf_cfg_en,
    input  logic [31:0]                   rbox_esr_out_buf_pg,
    input  logic                          rbox_esr_out_buf_pg_en,
    input  logic [63:0]                   rbox_esr_out_buf_cfg,
    input  logic                          rbox_esr_out_buf_cfg_en,
    input  logic [31:0]                   rbox_esr_start,
    input  logic                          rbox_esr_start_en,
    input  logic [31:0]                   rbox_esr_consume,
    input  logic                          rbox_esr_consume_en,
    output logic [31:0]                   rbox_esr_status,

    // Control to traverse
    output logic                          tr_msaa_enable,
    output rbox_msaa_mode_t               tr_msaa_mode,
    output logic                          tr_depth_is_fp32,
    output logic                          tr_depth_is_un16,
    output logic                          tr_depth_is_un24,
    output logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_min_x,
    output logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_max_x,
    output logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_min_y,
    output logic [`RBOX_MAX_RENDER_DIM_R] tr_scissor_max_y,
    output logic                          tr_depth_clamp_enable,
    output logic [31:0]                   tr_depth_min,
    output logic [31:0]                   tr_depth_max,
    output logic                          tr_depth_bound_enable,
    output logic [31:0]                   tr_depth_bound_min,
    output logic [31:0]                   tr_depth_bound_max,

    // Work to Traverse
    output rbox_triangle_state_t          tr_triangle,
    output rbox_tile_state_t              tr_tile,
    output logic                          tr_tile_valid,
    input  logic                          tr_accept,
    input  logic                          tr_empty,

    // Output from Traverse
    input  rbox_quad_data_t               tr_quad,
    input  logic                          tr_quad_valid,
    output logic                          ct_quad_accept,

    // Control to Test
    output rbox_state_t                   ts_state,
    output logic                          ts_front_triangle,

    // Test to Control
    input logic                           ts_empty,

    // Output from Test
    input  logic [3:0]                    ts_result_mask,
    input  logic                          ts_result_valid,
    output logic                          ct_result_accept,

    // Flush Depth Buffer to/from Test
    output logic                          db_flush,
    input  logic                          db_flush_done,

    // Signals to Status Monitor
    output rbox_sm_ctrl_t                 ct_sm_data,

    // Debug run control
    input  logic                          rbox_halted

);

rbox_esr_config_t      s0_rbox_esr_config_ff;
rbox_esr_in_buf_pg_t   s0_rbox_esr_in_buf_pg_ff;
rbox_esr_in_buf_cfg_t  s0_rbox_esr_in_buf_cfg_ff;
rbox_esr_out_buf_pg_t  s0_rbox_esr_out_buf_pg_ff;
rbox_esr_out_buf_cfg_t s0_rbox_esr_out_buf_cfg_ff;
rbox_esr_consume_t     s0_rbox_esr_consume_ff;

rbox_esr_start_t       s0_rbox_esr_start_ff;

rbox_esr_status_t      s0_rbox_esr_status_ff;
rbox_esr_status_t      s0_rbox_esr_status_nxt;
logic                  s0_rbox_esr_status_en;


`RST_EN_FF(clock,      reset, rbox_esr_config_en,      s0_rbox_esr_config_ff,      rbox_esr_config,        `RBOX_ESR_CONFIG_RESET     )
`RST_EN_FF(clock_rbox, reset, rbox_esr_in_buf_pg_en,   s0_rbox_esr_in_buf_pg_ff,   rbox_esr_in_buf_pg,     `RBOX_ESR_IN_BUF_PG_RESET  )
`RST_EN_FF(clock_rbox, reset, rbox_esr_in_buf_cfg_en,  s0_rbox_esr_in_buf_cfg_ff,  rbox_esr_in_buf_cfg,    `RBOX_ESR_IN_BUF_CFG_RESET )
`RST_EN_FF(clock_rbox, reset, rbox_esr_out_buf_pg_en,  s0_rbox_esr_out_buf_pg_ff,  rbox_esr_out_buf_pg,    `RBOX_ESR_OUT_BUF_PG_RESET )
`RST_EN_FF(clock_rbox, reset, rbox_esr_out_buf_cfg_en, s0_rbox_esr_out_buf_cfg_ff, rbox_esr_out_buf_cfg,   `RBOX_ESR_OUT_BUF_CFG_RESET)
`RST_EN_FF(clock_rbox, reset, rbox_esr_start_en,       s0_rbox_esr_start_ff,       rbox_esr_start,         `RBOX_ESR_START_RESET      )
`RST_EN_FF(clock_rbox, reset, s0_rbox_esr_status_en,   s0_rbox_esr_status_ff,      s0_rbox_esr_status_nxt, `RBOX_ESR_STATUS_RESET     )
`RST_EN_FF(clock_rbox, reset, rbox_esr_consume_en,     s0_rbox_esr_consume_ff,     rbox_esr_consume,       `RBOX_ESR_CONSUME_RESET    )

logic                         s1_start_trigger;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_buf_off_ff;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_buf_off_nxt;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_pending_lines_ff;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_pending_lines_nxt;
logic                         s1_in_req_line_en;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_received_lines_ff;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_received_lines_nxt;
logic                         s1_in_received_lines_en;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_process_pckts_ff;
logic [`RBOX_IN_BUF_LINE_RNG] s1_in_process_pckts_nxt;
logic                         s1_in_process_pckts_en;
logic                         s1_started_ff;
logic                         s1_started_nxt;
logic                         s1_done_ff;
logic                         s1_done_nxt;
logic                         s1_db_flush_pending_en;
logic                         s1_db_flush_pending_ff;
logic                         s1_db_flush_pending_nxt;
logic                         s1_db_flush_done_en;
logic                         s1_db_flush_done_ff;
logic                         s1_db_flush_done_nxt;
logic                         s1_in_update;
logic                         s1_config_error;
logic                         s1_in_new_req;
logic                         s1_in_buffer_end;
logic                         s1_sc_rbox_rsp_is_read_data;
logic                         s1_in_sc_req_ready;
logic [(`SC_BANKS+1)-1:0]     s1_in_sc_req_valid_ff;
logic                         s2_in_fifo_pop_done;
logic                         s3_end_of_buffer_ff;
logic                         s4_all_sent_end_buf_ff;
logic                         s4_all_sent_msg_ff;

always_comb
begin
    s1_start_trigger = rbox_esr_start_en && rbox_esr_start;

    s1_config_error =   !s0_rbox_esr_in_buf_pg_ff.page_active0 && !s0_rbox_esr_in_buf_pg_ff.page_active1
                    ||  !s0_rbox_esr_in_buf_pg_ff.page_active0 &&  s0_rbox_esr_in_buf_cfg_ff.start_offset[`RBOX_OUT_BUF_LINE_SZ-1]
                    ||  !s0_rbox_esr_in_buf_pg_ff.page_active1 &&  s0_rbox_esr_in_buf_cfg_ff.start_offset[`RBOX_OUT_BUF_LINE_SZ-1]
                    || !(s0_rbox_esr_in_buf_pg_ff.page_active1 &&  s0_rbox_esr_in_buf_pg_ff.page_active0) && s0_rbox_esr_in_buf_cfg_ff.line_count[`RBOX_OUT_BUF_LINE_SZ-1]
                    ||  !s0_rbox_esr_out_buf_pg_ff.page_active;

    s1_started_nxt = (s1_started_ff && ~s1_done_ff) || s1_start_trigger;

    s1_done_nxt = s1_started_ff
                && (   s1_config_error
                    || (   s3_end_of_buffer_ff
                        && s4_all_sent_end_buf_ff
                        && s4_all_sent_msg_ff
                        && (   !s0_rbox_esr_start_ff.flush
                            ||  s1_db_flush_done_ff)));

    s1_db_flush_pending_en  =  reset
                            || db_flush_done
                            || (    s1_started_ff
                                && !s1_config_error
                                &&  s0_rbox_esr_start_ff.flush
                                &&  s3_end_of_buffer_ff
                                &&  tr_empty
                                &&  ts_empty
                                && !s1_db_flush_done_ff);

    s1_db_flush_pending_nxt =  !reset
                            &&  s1_started_ff
                            && !s1_config_error
                            &&  s0_rbox_esr_start_ff.flush
                            &&  s3_end_of_buffer_ff
                            &&  tr_empty
                            &&  ts_empty
                            && !db_flush_done;

    db_flush = s1_db_flush_pending_ff;

    s1_db_flush_done_en =  reset
                        || s1_done_ff
                        || db_flush_done;

    s1_db_flush_done_nxt = !reset && !s1_done_ff && s1_db_flush_pending_ff && db_flush_done;

    s1_in_req_line_en       = 1'b0;
    s1_in_received_lines_en = 1'b0;
    s1_in_process_pckts_en  = 1'b0;

    s1_in_pending_lines_nxt  = s1_in_pending_lines_ff;
    s1_in_received_lines_nxt = s1_in_received_lines_ff;
    s1_in_process_pckts_nxt  = s1_in_process_pckts_ff;
    s1_in_buf_off_nxt        = s1_in_buf_off_ff;

    if (s1_start_trigger && ~s1_started_ff && !s1_config_error)
    begin
        s1_in_req_line_en       = 1'b1;
        s1_in_received_lines_en = 1'b1;
        s1_in_process_pckts_en  = 1'b1;

        s1_in_pending_lines_nxt  = s0_rbox_esr_in_buf_cfg_ff.line_count;
        s1_in_received_lines_nxt = '0;
        s1_in_process_pckts_nxt  = '0;
        s1_in_buf_off_nxt        = '0;
    end

    if (s1_in_new_req && (!(|s1_in_sc_req_valid_ff) || s1_in_sc_req_ready))
    begin
        s1_in_req_line_en = 1'b1;
        s1_in_pending_lines_nxt--;
        s1_in_buf_off_nxt++;
    end

    if (s2_in_fifo_pop_done)
    begin
        s1_in_process_pckts_en = 1'b1;
        s1_in_process_pckts_nxt++;
    end

    if (s1_sc_rbox_rsp_is_read_data)
    begin
        s1_in_received_lines_en = 1'b1;
        s1_in_received_lines_nxt++;
    end

    s1_in_buffer_end = (s1_in_received_lines_ff == s0_rbox_esr_in_buf_cfg_ff.line_count);

    s0_rbox_esr_status_nxt = '0;
    if (s1_start_trigger && s1_config_error)
        s0_rbox_esr_status_nxt.status = rbox_status_t'(rbox_pkg::StatusError);
    else
        if (s1_started_nxt)
            s0_rbox_esr_status_nxt.status = rbox_status_t'(rbox_pkg::StatusWorking);
        else
            if (s1_done_ff)
                s0_rbox_esr_status_nxt.status = rbox_status_t'(rbox_pkg::StatusFinished);
            else
                s0_rbox_esr_status_nxt.status = rbox_status_t'(rbox_pkg::StatusReady);

    s0_rbox_esr_status_en = reset || s1_start_trigger || (s1_started_ff && s1_done_ff);

    rbox_esr_status = s0_rbox_esr_status_ff;

    rbox_enable = s0_rbox_esr_config_ff.enable;
end

logic [`RBOX_FIFO_PTR_RNG]    s1_sc_rbox_rsp_buf_ptr;
logic [511:0]                 s1_sc_rbox_rsp_data;
logic                         s4_hart_ack_received;
logic [`RBOX_HART_I_R]        s4_hart_id_for_ack;

always_comb
begin
    sc_rbox_rsp_ready = sc_rbox_rsp_valid;

    s1_sc_rbox_rsp_is_read_data =    sc_rbox_rsp_valid
                                && ( sc_rbox_rsp.opcode == ET_LINK_RSP_AckData)
                                &&  !sc_rbox_rsp.id[`ET_LINK_ID_SIZE-1];
    s1_sc_rbox_rsp_buf_ptr = sc_rbox_rsp.id[`RBOX_FIFO_PTR_RNG];
    s1_sc_rbox_rsp_data = sc_rbox_rsp.data;

    s4_hart_ack_received =    sc_rbox_rsp_valid
                         && ( sc_rbox_rsp.opcode == ET_LINK_RSP_Ack)
                         &&  !sc_rbox_rsp.id[`ET_LINK_ID_SIZE-1];
    s4_hart_id_for_ack = sc_rbox_rsp.id[`RBOX_HART_I_R];

    sc_db_rsp_valid =  sc_rbox_rsp_valid
                    && sc_rbox_rsp.id[`ET_LINK_ID_SIZE-1];

    sc_db_rsp = sc_rbox_rsp;
end

   `RST_FF(clock_rbox, reset,                          s1_started_ff,           s1_started_nxt,           1'b0)
   `RST_FF(clock_rbox, reset,                          s1_done_ff,              s1_done_nxt,              1'b0)
    `EN_FF(clock_rbox,        s1_db_flush_pending_en,  s1_db_flush_pending_ff,  s1_db_flush_pending_nxt       )
    `EN_FF(clock_rbox,        s1_db_flush_done_en,     s1_db_flush_done_ff,     s1_db_flush_done_nxt          )
`RST_EN_FF(clock_rbox, reset, s1_in_req_line_en,       s1_in_pending_lines_ff,  s1_in_pending_lines_nxt,   '0 )
`RST_EN_FF(clock_rbox, reset, s1_in_req_line_en,       s1_in_buf_off_ff,        s1_in_buf_off_nxt,         '0 )
`RST_EN_FF(clock_rbox, reset, s1_in_received_lines_en, s1_in_received_lines_ff, s1_in_received_lines_nxt,  '0 )
`RST_EN_FF(clock_rbox, reset, s1_in_process_pckts_en,  s1_in_process_pckts_ff,  s1_in_process_pckts_nxt,   '0 )

logic [`RBOX_FIFO_PTR_RNG] s1_in_fifo_alloc_ptr;
logic                      s1_in_fifo_alloc;

logic [511:0]              s1_in_fifo_in_data;
logic [`RBOX_FIFO_PTR_RNG] s1_in_fifo_push_ptr;
logic                      s1_in_fifo_push;
logic                      s1_in_fifo_push_done;

logic [511:0]              s2_in_fifo_out_data;
logic                      s2_in_fifo_out_valid;
rbox_pkg::inpck_type_e     s2_in_fifo_out_pck_type;
logic                      s2_in_fifo_pop;

logic                      s1_in_fifo_full;
logic                      s1_in_fifo_empty;

rbox_input_fifo input_fifo (
                            .clk_i        (clock_rbox             ),
                            .rst_ni       (~reset                 ),
                            .ram_cfg_i    (ram_cfg_t'('0)         ),
                            .dft_i        (dft_t'('0)             ),

                            .alloc_ptr_o  (s1_in_fifo_alloc_ptr   ),
                            .alloc_i      (s1_in_fifo_alloc       ),

                            .in_data_i    (s1_in_fifo_in_data     ),
                            .push_ptr_i   (s1_in_fifo_push_ptr    ),
                            .push_i       (s1_in_fifo_push        ),
                            .push_done_o  (s1_in_fifo_push_done   ),

                            .out_data_o      (s2_in_fifo_out_data    ),
                            .out_pck_type_o  (s2_in_fifo_out_pck_type),
                            .out_valid_o     (s2_in_fifo_out_valid   ),
                            .pop_i           (s2_in_fifo_pop         ),
                            .pop_done_o      (s2_in_fifo_pop_done    ),

                            .buffer_end_i (s1_in_buffer_end       ),

                            .full_o       (s1_in_fifo_full        ),
                            .empty_o      (s1_in_fifo_empty       )
                           );

logic [(`SC_BANKS+1)-1:0]     s1_in_sc_req_valid_nxt;
logic                         s1_in_sc_req_valid_en;
et_link_req_info_t            s1_in_sc_req_nxt;
et_link_req_info_t            s1_in_sc_req_ff;
logic                         s1_in_sc_req_en;
logic [`PA_RANGE]             s1_req_pa;
logic [`RBOX_IN_BUF_LINE_RNG] s1_req_line;

always_comb
begin
    s1_req_line = s0_rbox_esr_in_buf_cfg_ff.start_offset + s1_in_buf_off_ff;
    s1_req_pa = 0;
    if (s0_rbox_esr_in_buf_pg_ff.page_active0 && s0_rbox_esr_in_buf_pg_ff.page_active1)
        s1_req_pa = {s1_req_line[`RBOX_IN_BUF_LINE_SZ-1] ? s0_rbox_esr_in_buf_pg_ff.page1 : s0_rbox_esr_in_buf_pg_ff.page0, s1_req_line[`RBOX_IN_BUF_LINE_SZ-2:0], 6'b0};
    else if (s0_rbox_esr_in_buf_pg_ff.page_active1)
        s1_req_pa = {s0_rbox_esr_in_buf_pg_ff.page1, s1_req_line[`RBOX_IN_BUF_LINE_SZ-2:0], 6'b0};
    else if (s0_rbox_esr_in_buf_pg_ff.page_active0)
        s1_req_pa = {s0_rbox_esr_in_buf_pg_ff.page0, s1_req_line[`RBOX_IN_BUF_LINE_SZ-2:0], 6'b0};

    s1_in_new_req = s1_started_ff
                  && (s1_in_pending_lines_ff != 'b0)
                  && !s1_in_fifo_full;

    s1_in_sc_req_nxt           = 0;
    s1_in_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, s1_in_fifo_alloc_ptr);
    s1_in_sc_req_nxt.source    = 0;
    s1_in_sc_req_nxt.opcode    = ET_LINK_REQ_Read;
    s1_in_sc_req_nxt.subopcode = '0;
    s1_in_sc_req_nxt.address   = s1_req_pa;
    s1_in_sc_req_nxt.wdata     = 0;
    s1_in_sc_req_nxt.data      = 0;
    s1_in_sc_req_nxt.size      = ET_LINK_Line;
    s1_in_sc_req_nxt.qwen      = 4'b0000;

    s1_in_sc_req_valid_nxt = '0;

    s1_in_sc_req_valid_nxt[s1_req_pa[`SC_BANK_SEL_ADDR_RANGE]] = !reset && s1_in_new_req;

    s1_in_sc_req_valid_en =  reset
                          || (|s1_in_sc_req_valid_nxt) && !(|s1_in_sc_req_valid_ff)
                          ||   s1_in_sc_req_ready;

    s1_in_sc_req_en =     (|s1_in_sc_req_valid_nxt)
                    && ( !(|s1_in_sc_req_valid_ff) || s1_in_sc_req_ready);

    s1_in_fifo_alloc = s1_in_sc_req_en;

    s1_in_fifo_in_data  = s1_sc_rbox_rsp_data;
    s1_in_fifo_push     = s1_sc_rbox_rsp_is_read_data;
    s1_in_fifo_push_ptr = s1_sc_rbox_rsp_buf_ptr;
end

`EN_FF(clock_rbox, s1_in_sc_req_en,       s1_in_sc_req_ff,       s1_in_sc_req_nxt      )
`EN_FF(clock_rbox, s1_in_sc_req_valid_en, s1_in_sc_req_valid_ff, s1_in_sc_req_valid_nxt)

et_link_req_info_t            s4_out_msg_sc_req_ff;
logic [(`SC_BANKS+1)-1:0]     s4_out_msg_sc_req_valid_ff;
logic                         s4_out_msg_sc_req_ready;

et_link_req_info_t            s4_out_pck_sc_req_ff;
logic [(`SC_BANKS+1)-1:0]     s4_out_pck_sc_req_valid_ff;
logic                         s4_out_pck_sc_req_ready;

always_comb
begin

    rbox_sc_req       = '0;
    rbox_sc_req_valid = '0;

    priority case (1'b1)
        (|db_sc_req_valid)            : {rbox_sc_req_valid, rbox_sc_req} = {        db_sc_req_valid,            db_sc_req   };
        (|s1_in_sc_req_valid_ff)      : {rbox_sc_req_valid, rbox_sc_req} = {     s1_in_sc_req_valid_ff,      s1_in_sc_req_ff};
        (|s4_out_msg_sc_req_valid_ff) : {rbox_sc_req_valid, rbox_sc_req} = {s4_out_msg_sc_req_valid_ff, s4_out_msg_sc_req_ff};
        (|s4_out_pck_sc_req_valid_ff) : {rbox_sc_req_valid, rbox_sc_req} = {s4_out_pck_sc_req_valid_ff, s4_out_pck_sc_req_ff};
        default                       : ;
    endcase

    db_sc_req_ready         = 1'b0;
    s1_in_sc_req_ready      = 1'b0;
    s4_out_msg_sc_req_ready = 1'b0;
    s4_out_pck_sc_req_ready = 1'b0;

    priority case (1'b1)
        (|db_sc_req_valid)            : db_sc_req_ready         = |(        db_sc_req_valid    & rbox_sc_req_ready);
        (|s1_in_sc_req_valid_ff)      : s1_in_sc_req_ready      = |(     s1_in_sc_req_valid_ff & rbox_sc_req_ready);
        (|s4_out_msg_sc_req_valid_ff) : s4_out_msg_sc_req_ready = |(s4_out_msg_sc_req_valid_ff & rbox_sc_req_ready);
        (|s4_out_pck_sc_req_valid_ff) : s4_out_pck_sc_req_ready = |(s4_out_pck_sc_req_valid_ff & rbox_sc_req_ready);
        default                       : ;
    endcase

end

logic [rbox_pkg::InpckDataSz-1:0]       s2_read_pck_data;
rbox_pkg::inpck_rbox_state_t                 s2_inpck_rbox_state;
rbox_pkg::inpck_fsh_state_t                  s2_inpck_fsh_state;
rbox_pkg::inpck_large_triangle_t             s2_inpck_rbox_large_triangle;
rbox_pkg::inpck_tri_with_tile_128x128_t s2_inpck_rbox_tri_w_128t;
rbox_pkg::inpck_tri_with_tile_64x64_t   s2_inpck_rbox_tri_w_64t;
rbox_pkg::inpck_large_triangle_tile_t        s2_inpck_rbox_ltt;
rbox_pkg::inpck_fully_covered_tile_t        s2_inpck_rbox_fct;
rbox_pkg::state_t                            s2_rbox_state;
rbox_pkg::fsh_state_t                        s2_fsh_state;
rbox_pkg::triangle_state_t                   s2_triangle_state;
rbox_pkg::tile_state_t                       s2_tile_state;
logic [`RBOX_MAX_RENDER_DIM_R]          s2_scissor_min_x;
logic [`RBOX_MAX_RENDER_DIM_R]          s2_scissor_max_x;
logic [`RBOX_MAX_RENDER_DIM_R]          s2_scissor_min_y;
logic [`RBOX_MAX_RENDER_DIM_R]          s2_scissor_max_y;
logic [`RBOX_HART_I_R]                  s2_last_active_hart;
logic [`RBOX_HART_E_R]                  s2_hart_active_mask;
logic                                   s2_hart_active_en;
logic                                   s2_load_rbox_state;
logic                                   s2_load_fsh_state;
logic                                   s2_load_triangle_state;
logic                                   s2_load_tile_state;
logic                                   s2_input_pck_accept;
logic                                   s2_drawcall_accept;
logic                                   s2_new_tile;
logic                                   s2_new_drawcall;
logic                                   s2_tile_valid_nxt;
logic                                   s2_tile_valid_en;
logic                                   s2_end_of_buffer;
logic                                   s2_end_of_buffer_en;
logic                                   s3_tile_valid_ff;
logic                                   s4_drawcall_done_ff;

always_comb
begin
    s2_input_pck_accept = s2_in_fifo_out_valid && !s3_tile_valid_ff && tr_accept;
    s2_drawcall_accept = s2_in_fifo_out_valid && s4_drawcall_done_ff;

    s2_read_pck_data = s2_in_fifo_out_data;
    s2_inpck_rbox_state = rbox_pkg::inpck_rbox_state_t'(s2_read_pck_data[$bits(rbox_pkg::inpck_rbox_state_t)-1:0]);
    s2_inpck_fsh_state = rbox_pkg::inpck_fsh_state_t'(s2_read_pck_data[$bits(rbox_pkg::inpck_fsh_state_t)-1:0]);
    s2_inpck_rbox_large_triangle = rbox_pkg::inpck_large_triangle_t'(s2_read_pck_data[$bits(rbox_pkg::inpck_large_triangle_t)-1:0]);
    s2_inpck_rbox_tri_w_128t = rbox_pkg::inpck_tri_with_tile_128x128_t'(s2_read_pck_data[$bits(rbox_pkg::inpck_tri_with_tile_128x128_t)-1:0]);
    s2_inpck_rbox_tri_w_64t = rbox_pkg::inpck_tri_with_tile_64x64_t'(s2_read_pck_data[$bits(rbox_pkg::inpck_tri_with_tile_64x64_t)-1:0]);
    s2_inpck_rbox_ltt = rbox_pkg::inpck_large_triangle_tile_t'(s2_read_pck_data[$bits(rbox_pkg::inpck_large_triangle_tile_t)-1:0]);
    s2_inpck_rbox_fct = rbox_pkg::inpck_fully_covered_tile_t'(s2_read_pck_data[$bits(rbox_pkg::inpck_fully_covered_tile_t)-1:0]);

    s2_end_of_buffer_en = s1_start_trigger;
    s2_end_of_buffer    = s3_end_of_buffer_ff && !s1_start_trigger;

    s2_load_triangle_state = 1'b0;
    s2_load_tile_state     = 1'b0;
    s2_load_rbox_state     = 1'b0;
    s2_load_fsh_state      = 1'b0;

    s2_triangle_state      =  '0;
    s2_tile_state          =  '0;
    s2_rbox_state          =  '0;
    s2_fsh_state           =  '0;

    s2_new_drawcall        = 1'b0;
    s2_new_tile            = 1'b0;

    s2_last_active_hart     = 'd63;

    s2_hart_active_mask = '1;

    casez (s2_in_fifo_out_pck_type)
        rbox_pkg::InpckTriWithTile64x64   : begin
                                                    s2_new_tile            = s2_in_fifo_out_valid;
                                                    s2_load_triangle_state = s2_input_pck_accept;
                                                    s2_load_tile_state     = s2_input_pck_accept;
                                                    s2_triangle_state      = rbox_pkg::t64_triangle_state(s2_inpck_rbox_tri_w_64t);
                                                    s2_tile_state          = rbox_pkg::t64_tile_state(s2_inpck_rbox_tri_w_64t);
                                                end
        rbox_pkg::InpckTriWithTile128x128 : begin
                                                    s2_new_tile            = s2_in_fifo_out_valid;
                                                    s2_load_triangle_state = s2_input_pck_accept;
                                                    s2_load_tile_state     = s2_input_pck_accept;
                                                    s2_triangle_state      = rbox_pkg::t128_triangle_state(s2_inpck_rbox_tri_w_128t);
                                                    s2_tile_state          = rbox_pkg::t128_tile_state(s2_inpck_rbox_tri_w_128t);
                                                end
        rbox_pkg::InpckLargeTriangle             : begin
                                                    s2_load_triangle_state = s2_input_pck_accept;
                                                    s2_triangle_state      = rbox_pkg::lt_triangle_state(s2_inpck_rbox_large_triangle);
                                                end
        rbox_pkg::InpckFullyCoveredTile         : begin
                                                    s2_new_tile        = s2_in_fifo_out_valid;
                                                    s2_load_tile_state = s2_input_pck_accept;
                                                    s2_tile_state      = rbox_pkg::fct_tile_state(s2_inpck_rbox_fct);
                                                end
        rbox_pkg::InpckLargeTriangleTile        : begin
                                                    s2_new_tile        = s2_in_fifo_out_valid;
                                                    s2_load_tile_state = s2_input_pck_accept;
                                                    s2_tile_state      = rbox_pkg::fct_tile_state(s2_inpck_rbox_fct);
                                                end
        rbox_pkg::InpckRboxState                 : begin
                                                    s2_load_rbox_state      = s2_input_pck_accept;
                                                    s2_rbox_state           = rbox_pkg::state_from_inpck(s2_inpck_rbox_state.state);
                                                    s2_last_active_hart  = ('d1 << (  `ZX(`RBOX_HARTS_L, s2_rbox_state.shire_layout_height)
                                                                                    + `ZX(`RBOX_HARTS_L, s2_rbox_state.shire_layout_width)))
                                                                         - 'd1;
                                                    for (integer m = 0; m < `RBOX_HARTS_PER_SHIRE; m++)
                                                        s2_hart_active_mask[m] = (m <= s2_last_active_hart);
                                                end
        rbox_pkg::InpckFragShadingState         : begin
                                                    s2_new_drawcall   = s2_in_fifo_out_valid;
                                                    s2_load_fsh_state = s2_drawcall_accept;
                                                    s2_fsh_state      = rbox_pkg::fsh_state_from_inpck(s2_inpck_fsh_state);
                                                end
        rbox_pkg::InpckEndOfBuffer              : begin
                                                    s2_end_of_buffer_en = s2_in_fifo_out_valid;
                                                    s2_end_of_buffer    = s2_in_fifo_out_valid;
                                                end
    endcase

    s2_scissor_min_x = s2_rbox_state.scissor_start_x;
    s2_scissor_min_y = s2_rbox_state.scissor_start_y;
    s2_scissor_max_x = s2_rbox_state.scissor_start_x + s2_rbox_state.scissor_width;
    s2_scissor_max_y = s2_rbox_state.scissor_start_y + s2_rbox_state.scissor_height;

    s2_hart_active_en = reset || s2_load_rbox_state;

    s2_tile_valid_nxt = !reset && s2_new_tile && tr_accept && !s3_tile_valid_ff;
    s2_tile_valid_en  =  reset || s2_new_tile && tr_accept ||  s3_tile_valid_ff;

    s2_in_fifo_pop =    s1_started_ff
                   && (   ( s2_new_drawcall && s2_drawcall_accept)
                       || (!s2_new_drawcall && s2_input_pck_accept)
                       ||   s2_end_of_buffer);
end

rbox_pkg::state_t                   s3_rbox_state_ff;
logic [`RBOX_MAX_RENDER_DIM_R] s3_scissor_min_x_ff;
logic [`RBOX_MAX_RENDER_DIM_R] s3_scissor_max_x_ff;
logic [`RBOX_MAX_RENDER_DIM_R] s3_scissor_min_y_ff;
logic [`RBOX_MAX_RENDER_DIM_R] s3_scissor_max_y_ff;
logic [`RBOX_HART_I_R]         s3_last_active_hart_ff;
logic [`RBOX_HART_E_R]         s3_hart_active_mask_ff;
rbox_pkg::fsh_state_t               s3_fsh_state_ff;
rbox_pkg::triangle_state_t          s3_triangle_state_ff;
rbox_pkg::tile_state_t              s3_tile_state_ff;

`EN_FF(clock_rbox, s2_tile_valid_en,       s3_tile_valid_ff,       s2_tile_valid_nxt  )
`EN_FF(clock_rbox, s2_load_rbox_state,     s3_rbox_state_ff,       s2_rbox_state      )
`EN_FF(clock_rbox, s2_load_rbox_state,     s3_scissor_min_x_ff,    s2_scissor_min_x   )
`EN_FF(clock_rbox, s2_load_rbox_state,     s3_scissor_max_x_ff,    s2_scissor_max_x   )
`EN_FF(clock_rbox, s2_load_rbox_state,     s3_scissor_min_y_ff,    s2_scissor_min_y   )
`EN_FF(clock_rbox, s2_load_rbox_state,     s3_scissor_max_y_ff,    s2_scissor_max_y   )
`EN_FF(clock_rbox, s2_hart_active_en,      s3_last_active_hart_ff, s2_last_active_hart)
`EN_FF(clock_rbox, s2_hart_active_en,      s3_hart_active_mask_ff, s2_hart_active_mask)
`EN_FF(clock_rbox, s2_load_fsh_state,      s3_fsh_state_ff,        s2_fsh_state       )
`EN_FF(clock_rbox, s2_load_triangle_state, s3_triangle_state_ff,   s2_triangle_state  )
`EN_FF(clock_rbox, s2_load_tile_state,     s3_tile_state_ff,       s2_tile_state      )
`EN_FF(clock_rbox, s2_end_of_buffer_en,    s3_end_of_buffer_ff,    s2_end_of_buffer   )


logic [`RBOX_HART_E_R][`RBOX_HART_BUF_OFF_R]     s4_hart_buf_off_ff;
logic [`RBOX_HART_E_R][`RBOX_HART_BUF_OFF_R]     s4_hart_buf_off_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_buf_off_en;
logic [`RBOX_HART_E_R][`RBOX_HART_PCKT_CREDIT_R] s4_hart_pckt_credits_ff;
logic [`RBOX_HART_E_R][`RBOX_HART_PCKT_CREDIT_R] s4_hart_pckt_credits_nxt;
logic [`RBOX_HART_E_R][`RBOX_HART_MSG_CREDIT_R]  s4_hart_msg_credits_ff;
logic [`RBOX_HART_E_R][`RBOX_HART_MSG_CREDIT_R]  s4_hart_msg_credits_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_credits_en;
logic [`RBOX_HART_E_R][`RBOX_HART_PCKT_CREDIT_R] s4_hart_pending_acks_ff;
logic [`RBOX_HART_E_R][`RBOX_HART_PCKT_CREDIT_R] s4_hart_pending_acks_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_pending_acks_en;
logic [`RBOX_HART_E_R][`RBOX_HART_PCKT_CREDIT_R] s4_hart_sent_pckts_ff;
logic [`RBOX_HART_E_R][`RBOX_HART_PCKT_CREDIT_R] s4_hart_sent_pckts_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_sent_pckts_en;
logic [`RBOX_HART_E_R]                           s4_hart_send_fsh_state_ff;
logic [`RBOX_HART_E_R]                           s4_hart_send_fsh_state_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_send_fsh_state_en;
logic [`RBOX_HART_E_R]                           s4_hart_sent_fsh_state_ff;
logic [`RBOX_HART_E_R]                           s4_hart_sent_fsh_state_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_sent_fsh_state_en;
logic [`RBOX_HART_E_R]                           s4_hart_sent_end_buf_ff;
logic [`RBOX_HART_E_R]                           s4_hart_sent_end_buf_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_sent_end_buf_en;
logic                                            s4_all_sent_end_buf_nxt;
logic                                            s4_all_sent_end_buf_en;
logic [`RBOX_HART_E_R]                           s4_hart_send_msg_ff;
logic [`RBOX_HART_E_R]                           s4_hart_send_msg_nxt;
logic [`RBOX_HART_E_R]                           s4_hart_send_msg_en;
logic                                            s4_all_sent_msg_nxt;
logic                                            s4_all_sent_msg_en;

logic [`RBOX_HART_I_R]                           s4_hart_id;
logic [`RBOX_HART_I_R]                           s4_pckt_send_hart_id;
logic [`RBOX_HART_I_R]                           s4_msg_send_hart_id_ff;
logic [`RBOX_HART_I_R]                           s4_msg_send_hart_id_nxt;
logic                                            s4_msg_send_hart_id_en;
logic                                            s4_msg_send_hart_id_sel;
logic [`RBOX_HART_I_R]                           s4_msg_sent_hart_id;
logic [`RBOX_HART_I_R]                           s4_end_buf_hart_id_ff;
logic [`RBOX_HART_I_R]                           s4_end_buf_hart_id_nxt;
logic                                            s4_end_buf_hart_id_en;
logic                                            s4_end_buf_hart_id_sel;

logic                                            s4_hart_packet_sent;
logic                                            s4_hart_fsh_state_sent;
logic                                            s4_hart_msg_sent;
logic                                            s4_hart_msg_send_set;
logic                                            s4_hart_end_buf_set;

logic                                            s4_hart_credits_released_ff;
logic                                            s4_hart_credits_released_nxt;
logic                                            s4_hart_credits_released_en;

always_comb
begin
    s4_hart_buf_off_nxt        = s4_hart_buf_off_ff;
    s4_hart_pckt_credits_nxt   = s4_hart_pckt_credits_ff;
    s4_hart_msg_credits_nxt    = s4_hart_msg_credits_ff;
    s4_hart_sent_pckts_nxt     = s4_hart_sent_pckts_ff;
    s4_hart_pending_acks_nxt   = s4_hart_pending_acks_ff;
    s4_hart_send_fsh_state_nxt = s4_hart_send_fsh_state_ff;
    s4_hart_sent_fsh_state_nxt = s4_hart_sent_fsh_state_ff;
    s4_hart_sent_end_buf_nxt   = s4_hart_sent_end_buf_ff;
    s4_hart_send_msg_nxt       = s4_hart_send_msg_ff;

    s4_hart_buf_off_en        =   '0;
    s4_hart_credits_en        =   '0;
    s4_hart_sent_pckts_en     =   '0;
    s4_hart_pending_acks_en   =   '0;
    s4_hart_send_fsh_state_en =   '0;
    s4_hart_sent_fsh_state_en =   '0;
    s4_hart_sent_end_buf_en   =   '0;
    s4_end_buf_hart_id_en     = 1'b0;
    s4_all_sent_end_buf_en    = 1'b0;
    s4_hart_send_msg_en       =   '0;
    s4_msg_send_hart_id_en    = 1'b0;
    s4_all_sent_msg_en        = 1'b0;

    if (s1_start_trigger)
    begin
        s4_hart_buf_off_en        =   '1;
        s4_hart_credits_en        =   '1;
        s4_hart_sent_pckts_en     =   '1;
        s4_hart_pending_acks_en   =   '1;
        s4_hart_send_fsh_state_en =   '1;
        s4_hart_sent_fsh_state_en =   '1;
        s4_hart_sent_end_buf_en   =   '1;
        s4_end_buf_hart_id_en     = 1'b1;
        s4_all_sent_end_buf_en    = 1'b1;
        s4_hart_send_msg_en       =   '1;
        s4_msg_send_hart_id_en    = 1'b1;
        s4_all_sent_msg_en        = 1'b1;

        s4_hart_buf_off_nxt        = '0;
        s4_hart_sent_pckts_nxt     = '0;
        s4_hart_pending_acks_nxt   = '0;
        s4_hart_sent_fsh_state_nxt = '0;
        s4_hart_sent_end_buf_nxt   = '0;
        s4_hart_send_msg_nxt       = '0;
        for (integer m = 0; m < `RBOX_HARTS_PER_SHIRE; m++)
        begin
            s4_hart_pckt_credits_nxt[m]   = (1 << s0_rbox_esr_out_buf_cfg_ff.hart_buf_lines);
            s4_hart_msg_credits_nxt[m]    = s0_rbox_esr_out_buf_cfg_ff.hart_max_msg + 1;
            s4_hart_send_fsh_state_nxt[m] = 1'b1;
        end
    end

    s4_pckt_send_hart_id = s4_hart_end_buf_set ? s4_end_buf_hart_id_ff : s4_hart_id;

    if (s4_hart_packet_sent)
    begin
        if ( s4_hart_buf_off_nxt[s4_pckt_send_hart_id] == (1 << s0_rbox_esr_out_buf_cfg_ff.hart_buf_lines))
             s4_hart_buf_off_nxt[s4_pckt_send_hart_id] = 0;
        else
             s4_hart_buf_off_nxt[s4_pckt_send_hart_id]++;
              s4_hart_buf_off_en[s4_pckt_send_hart_id] = 1'b1;

        s4_hart_pckt_credits_nxt[s4_pckt_send_hart_id]--;
              s4_hart_credits_en[s4_pckt_send_hart_id] = 1'b1;

        s4_hart_pending_acks_nxt[s4_pckt_send_hart_id]++;
         s4_hart_pending_acks_en[s4_pckt_send_hart_id] = 1'b1;

          s4_hart_sent_pckts_nxt[s4_pckt_send_hart_id]++;
           s4_hart_sent_pckts_en[s4_pckt_send_hart_id] = 1'b1;

    end

    if (s4_hart_fsh_state_sent)
    begin
        s4_hart_send_fsh_state_nxt[s4_hart_id] = 1'b0;
         s4_hart_send_fsh_state_en[s4_hart_id] = 1'b1;

        s4_hart_sent_fsh_state_nxt[s4_hart_id] = 1'b1;
         s4_hart_sent_fsh_state_en[s4_hart_id] = 1'b1;
    end

    if (s4_hart_ack_received)
    begin
        s4_hart_pending_acks_nxt[s4_hart_id_for_ack]--;
         s4_hart_pending_acks_en[s4_hart_id_for_ack] = 1'b1;
    end

    if (s4_hart_credits_released_ff)
    begin
        s4_hart_pckt_credits_nxt[s0_rbox_esr_consume_ff.hart_id] += s0_rbox_esr_consume_ff.pckt_credits;
         s4_hart_msg_credits_nxt[s0_rbox_esr_consume_ff.hart_id] += s0_rbox_esr_consume_ff.msg_credits;
        s4_hart_credits_en[s0_rbox_esr_consume_ff.hart_id] = 1'b1;
    end

    if (s2_load_fsh_state)
    begin
        s4_hart_send_fsh_state_nxt = '1;
        s4_hart_send_fsh_state_en  = '1;

        s4_hart_sent_fsh_state_nxt = '0;
        s4_hart_sent_fsh_state_en  = '1;
    end

    if (s4_hart_end_buf_set)
    begin
         s4_hart_sent_end_buf_en[s4_end_buf_hart_id_ff] = 1'b1;
        s4_hart_sent_end_buf_nxt[s4_end_buf_hart_id_ff] = 1'b1;

        s4_all_sent_end_buf_en = 1'b1;
        s4_end_buf_hart_id_en  = 1'b1;
    end

    s4_msg_sent_hart_id = s4_out_msg_sc_req_ff.id[`RBOX_HART_I_R];

    if (s4_hart_msg_sent)
    begin
           s4_hart_send_msg_nxt[s4_msg_sent_hart_id] = 1'b0;
            s4_hart_send_msg_en[s4_msg_sent_hart_id] = 1'b1;


        s4_hart_msg_credits_nxt[s4_msg_sent_hart_id]--;
             s4_hart_credits_en[s4_msg_sent_hart_id]    = 1'b1;

         s4_hart_sent_pckts_nxt[s4_msg_sent_hart_id] = '0;
          s4_hart_sent_pckts_en[s4_msg_sent_hart_id] = 1'b1;

        s4_msg_send_hart_id_en = 1'b1;
        s4_all_sent_msg_en     = 1'b1;
    end

    if (s4_hart_msg_send_set)
    begin
        s4_hart_send_msg_nxt[s4_pckt_send_hart_id] = 1'b1;
         s4_hart_send_msg_en[s4_pckt_send_hart_id] = 1'b1;

        s4_msg_send_hart_id_en = 1'b1;
        s4_all_sent_msg_en     = 1'b1;
    end

    s4_hart_credits_released_nxt = rbox_esr_consume_en;
    s4_hart_credits_released_en  = rbox_esr_consume_en || s4_hart_credits_released_ff;

    s4_all_sent_msg_nxt     = s1_start_trigger ? 1'b0 : !(|(s4_hart_send_msg_nxt & s3_hart_active_mask_ff));
    s4_all_sent_end_buf_nxt = s1_start_trigger ? 1'b0 : &(s4_hart_sent_end_buf_nxt | (~s3_hart_active_mask_ff));

    s4_msg_send_hart_id_nxt = '0;
    s4_msg_send_hart_id_sel = !reset;

    for (integer m = 0; (m < `RBOX_HARTS_PER_SHIRE) && (m <= s3_last_active_hart_ff) && s4_msg_send_hart_id_sel; m++)
    begin
        s4_msg_send_hart_id_sel = !s4_hart_send_msg_nxt[m];
        s4_msg_send_hart_id_nxt =  s4_hart_send_msg_nxt[m] ? m : '0;
    end

    s4_end_buf_hart_id_nxt = '0;
    s4_end_buf_hart_id_sel = !reset;

    for (integer m = 0; (m < `RBOX_HARTS_PER_SHIRE) && (m <= s3_last_active_hart_ff) && s4_end_buf_hart_id_sel; m++)
    begin
        s4_end_buf_hart_id_sel =  s4_hart_sent_end_buf_nxt[m];
        s4_end_buf_hart_id_nxt = !s4_hart_sent_end_buf_nxt[m] ? m : '0;
    end

end

`RST_EN_FF(clock_rbox, reset, s4_hart_credits_released_en, s4_hart_credits_released_ff, s4_hart_credits_released_nxt, 1'b0);

genvar m;

generate
    for(m = 0; m < `RBOX_HARTS_PER_SHIRE; m++)
    begin
        `EN_FF(clock_rbox, s4_hart_buf_off_en[m],        s4_hart_buf_off_ff[m],        s4_hart_buf_off_nxt[m]       )
        `EN_FF(clock_rbox, s4_hart_credits_en[m],        s4_hart_pckt_credits_ff[m],   s4_hart_pckt_credits_nxt[m]  )
        `EN_FF(clock_rbox, s4_hart_credits_en[m],        s4_hart_msg_credits_ff[m],    s4_hart_msg_credits_nxt[m]   )
        `EN_FF(clock_rbox, s4_hart_sent_pckts_en[m],     s4_hart_sent_pckts_ff[m],     s4_hart_sent_pckts_nxt[m]    )
        `EN_FF(clock_rbox, s4_hart_pending_acks_en[m],   s4_hart_pending_acks_ff[m],   s4_hart_pending_acks_nxt[m]  )
        `EN_FF(clock_rbox, s4_hart_send_fsh_state_en[m], s4_hart_send_fsh_state_ff[m], s4_hart_send_fsh_state_nxt[m])
        `EN_FF(clock_rbox, s4_hart_sent_fsh_state_en[m], s4_hart_sent_fsh_state_ff[m], s4_hart_sent_fsh_state_nxt[m])
        `EN_FF(clock_rbox, s4_hart_sent_end_buf_en[m],   s4_hart_sent_end_buf_ff[m],   s4_hart_sent_end_buf_nxt[m]  )
        `EN_FF(clock_rbox, s4_hart_send_msg_en[m],       s4_hart_send_msg_ff[m],       s4_hart_send_msg_nxt[m]      )
    end
endgenerate

`EN_FF(clock_rbox, s4_all_sent_msg_en,     s4_all_sent_msg_ff,     s4_all_sent_msg_nxt    )
`EN_FF(clock_rbox, s4_all_sent_end_buf_en, s4_all_sent_end_buf_ff, s4_all_sent_end_buf_nxt)
`EN_FF(clock_rbox, s4_msg_send_hart_id_en, s4_msg_send_hart_id_ff, s4_msg_send_hart_id_nxt)
`EN_FF(clock_rbox, s4_end_buf_hart_id_en,  s4_end_buf_hart_id_ff,  s4_end_buf_hart_id_nxt )

logic s4_drawcall_done_nxt;
logic s4_drawcall_done_en;
logic s4_end_drawcall_nxt;
logic s4_end_drawcall_en;
logic s4_end_drawcall_ff;
logic s4_out_buf_empty;

always_comb

begin
    s4_drawcall_done_nxt = !s2_load_fsh_state;
    s4_drawcall_done_en  =  reset
                         || s2_load_fsh_state
                         || (tr_empty && ts_empty && s4_out_buf_empty);

    s4_end_drawcall_nxt =   !reset
                        && ((s2_new_drawcall && !s2_drawcall_accept) || s3_end_of_buffer_ff)
                        &&  tr_empty
                        &&  ts_empty;
    s4_end_drawcall_en =   reset
                       || !s4_end_drawcall_ff && s4_end_drawcall_nxt
                       ||  s2_drawcall_accept;

end

`EN_FF(clock_rbox, s4_drawcall_done_en, s4_drawcall_done_ff, s4_drawcall_done_nxt)
`EN_FF(clock_rbox, s4_end_drawcall_en,  s4_end_drawcall_ff,  s4_end_drawcall_nxt )

logic [`RBOX_MAX_QUAD_PCKS_I_R] s4_num_quad_pcks_ff;
logic [`RBOX_MAX_QUAD_PCKS_I_R] s4_num_quad_pcks_nxt;
logic                           s4_load_rbox_state_ff;
logic                           s4_load_rbox_state_nxt;
logic                           s4_load_rbox_state_en;

always_comb
begin
    s4_load_rbox_state_en  = s2_load_rbox_state || s4_load_rbox_state_ff;
    s4_load_rbox_state_nxt = s2_load_rbox_state;

    s4_num_quad_pcks_nxt = 'd0;

    if (!s3_rbox_state_ff.fragment_shader_disabled)
    begin
        if (s3_rbox_state_ff.fragment_shader_reads_bary)
            s4_num_quad_pcks_nxt += 2;
        if (s3_rbox_state_ff.fragment_shader_reads_depth)
            s4_num_quad_pcks_nxt++;
        if (s3_rbox_state_ff.fragment_shader_reads_coverage)
            s4_num_quad_pcks_nxt++;
    end
end

`EN_FF(clock_rbox, s2_load_rbox_state,    s4_load_rbox_state_ff,  s4_load_rbox_state_nxt)
`EN_FF(clock_rbox, s4_load_rbox_state_ff, s4_num_quad_pcks_ff,     s4_num_quad_pcks_nxt )

rbox_pkg::quad_info_t [1:0]          s4_out_buf_info;
logic                           s4_out_buf_info_valid;
logic [255:0]                   s4_out_buf_packet;
logic                           s4_out_buf_packet_valid;
logic [`RBOX_MAX_QUAD_PCKS_I_R] s4_pending_quad_pcks_ff;
logic                           s4_out_buf_pop;

rbox_output_buffer output_buffer (
                                  .clk_i                (clock_rbox                                     ),
                                  .rst_ni               (~reset                                         ),
                                  .ram_cfg_i            (ram_cfg_t'('0)                                 ),
                                  .dft_i                (dft_t'('0)                                     ),

                                  .tr_quad_i            (tr_quad                                        ),
                                  .tr_quad_valid_i      (tr_quad_valid                                  ),
                                  .ct_quad_accept_o     (ct_quad_accept                                 ),

                                  .ts_result_mask_i     (ts_result_mask                                 ),
                                  .ts_result_valid_i    (ts_result_valid                                ),
                                  .ct_result_accept_o   (ct_result_accept                               ),

                                  .out_info_o           (s4_out_buf_info                                ),
                                  .out_info_valid_o     (s4_out_buf_info_valid                          ),
                                  .out_packet_o         (s4_out_buf_packet                              ),
                                  .out_packet_valid_o   (s4_out_buf_packet_valid                        ),
                                  .out_pop_i            (s4_out_buf_pop                                 ),
                                  .out_pending_pcks_i   (s4_pending_quad_pcks_ff                        ),

                                  .out_end_drawcall_i   (s4_end_drawcall_ff                             ),

                                  .empty_o              (s4_out_buf_empty                               ),

                                  .hart_tile_width_i    (s3_rbox_state_ff.hart_tile_width               ),
                                  .hart_tile_height_i   (s3_rbox_state_ff.hart_tile_height              ),
                                  .shire_layout_width_i (s3_rbox_state_ff.shire_layout_width            ),
                                  .shire_layout_height_i(s3_rbox_state_ff.shire_layout_height           ),
                                  .fsh_reads_coverage_i (s3_rbox_state_ff.fragment_shader_reads_coverage),
                                  .fsh_reads_depth_i    (s3_rbox_state_ff.fragment_shader_reads_depth   ),
                                  .fsh_disabled_i       (s3_rbox_state_ff.fragment_shader_disabled      ),
                                  .early_frag_tests_enable_i(s3_rbox_state_ff.early_frag_tests_enable   ),

                                  .rbox_halted_i        (rbox_halted                                    )
                                 );


logic [`RBOX_MAX_QUAD_PCKS_I_R] s4_pending_quad_pcks_nxt;
logic                           s4_pending_quad_pcks_en;
rbox_pkg::outpck_fsh_state_t         s4_fsh_state_pckt;
rbox_pkg::outpck_quad_info_t         s4_quad_info_pckt;
rbox_pkg::outpck_end_phase_t         s4_end_phase_pckt;
et_link_req_info_t              s4_out_msg_sc_req_nxt;
logic                           s4_out_msg_sc_req_en;
logic [(`SC_BANKS+1)-1:0]       s4_out_msg_sc_req_valid_nxt;
logic                           s4_out_msg_sc_req_valid_en;
et_link_req_info_t              s4_out_pck_sc_req_nxt;
logic                           s4_out_pck_sc_req_en;
logic [(`SC_BANKS+1)-1:0]       s4_out_pck_sc_req_valid_nxt;
logic                           s4_out_pck_sc_req_valid_en;
logic [`PA_RANGE]               s4_out_pck_req_pa;
logic [`PA_RANGE]               s4_out_pck_end_req_pa;
logic [`PA_RANGE]               s4_out_msg_req_pa;
logic [`RBOX_OUT_BUF_LINE_RNG]  s4_hart_buf_ptr;
logic [`RBOX_OUT_BUF_LINE_RNG]  s4_hart_buf_end_ptr;

always_comb
begin
    s4_hart_id = s4_out_buf_info[0].hart_id;

    s4_hart_buf_ptr = {s0_rbox_esr_out_buf_cfg_ff.start_offset, 1'b0}
                    + (1 << s0_rbox_esr_out_buf_cfg_ff.hart_buf_lines) * s4_hart_id
                    + s4_hart_buf_off_ff[s4_hart_id];

    s4_out_pck_req_pa = {s0_rbox_esr_out_buf_pg_ff.page, s4_hart_buf_ptr, 5'b0};

    s4_pending_quad_pcks_nxt = '0;
    s4_pending_quad_pcks_en  = reset;

    s4_out_pck_sc_req_valid_nxt = '0;
    s4_out_pck_sc_req_nxt       = '0;

    s4_out_pck_sc_req_valid_en  = reset || s4_out_pck_sc_req_ready;
    s4_out_pck_sc_req_en        = 1'b0;

    s4_fsh_state_pckt = '0;
    s4_quad_info_pckt = '0;

    s4_hart_packet_sent    = 1'b0;
    s4_hart_fsh_state_sent = 1'b0;
    s4_hart_msg_send_set   = 1'b0;

    s4_out_buf_pop = 1'b0;

    if (    s4_out_buf_info_valid
        && (s4_pending_quad_pcks_ff == '0)
        &&  s4_hart_send_fsh_state_ff[s4_hart_id]
        && (s4_hart_pckt_credits_ff[s4_hart_id] > 'd1)
        && !s4_hart_send_msg_ff[s4_hart_id]
        && (!(|s4_out_pck_sc_req_valid_ff) || s4_out_pck_sc_req_ready))
    begin
        s4_fsh_state_pckt.outpck_type           = rbox_pkg::OutpckStateInfo;
        s4_fsh_state_pckt.fshader_data_addr     = s3_fsh_state_ff.fshader_data_addr;
        s4_fsh_state_pckt.fshader_function_addr = s3_fsh_state_ff.fshader_function_addr;

        s4_out_pck_sc_req_nxt.source    = 0;
        s4_out_pck_sc_req_nxt.wdata     = 1;
        s4_out_pck_sc_req_nxt.size      = ET_LINK_HLine;
        s4_out_pck_sc_req_nxt.qwen      = s4_out_pck_req_pa[5] ? 4'b1100 : 4'b0011;
        s4_out_pck_sc_req_nxt.opcode    = ET_LINK_REQ_Write;
        s4_out_pck_sc_req_nxt.subopcode = '0;
        s4_out_pck_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, s4_hart_id);
        s4_out_pck_sc_req_nxt.address   = s4_out_pck_req_pa;
        s4_out_pck_sc_req_nxt.data      = {s4_fsh_state_pckt, s4_fsh_state_pckt};

        s4_out_pck_sc_req_valid_nxt[s4_out_pck_req_pa[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;

        s4_out_pck_sc_req_valid_en = 1'b1;
        s4_out_pck_sc_req_en       = 1'b1;

        s4_hart_packet_sent    = 1'b1;
        s4_hart_fsh_state_sent = 1'b1;

        s4_hart_msg_send_set = ((s4_hart_sent_pckts_ff[s4_hart_id] + (1 + s4_num_quad_pcks_ff)) >= (1 << s0_rbox_esr_out_buf_cfg_ff.max_pckts_msg));
    end

    if (    s4_out_buf_info_valid
        && (s4_pending_quad_pcks_ff == '0)
        && !s4_hart_send_fsh_state_ff[s4_hart_id]
        && (s4_hart_pckt_credits_ff[s4_hart_id] > s4_num_quad_pcks_ff)
        && !s4_hart_send_msg_ff[s4_hart_id]
        && (!(|s4_out_pck_sc_req_valid_ff) || s4_out_pck_sc_req_ready))
    begin
        s4_pending_quad_pcks_en  = 1'b1;
        s4_pending_quad_pcks_nxt = s4_num_quad_pcks_ff;

        s4_quad_info_pckt.outpck_type = rbox_pkg::OutpckQuadInfo;
        s4_quad_info_pckt.quad0_x     = {s3_tile_state_ff.left[`RBOX_TILE_POS_R], s4_out_buf_info[0].x};
        s4_quad_info_pckt.quad0_y     = {s3_tile_state_ff.top[`RBOX_TILE_POS_R],  s4_out_buf_info[0].y};
        s4_quad_info_pckt.quad1_x     = s4_out_buf_info[1].start_pack ? '0 : {s3_tile_state_ff.left[`RBOX_TILE_POS_R], s4_out_buf_info[1].x};
        s4_quad_info_pckt.quad1_y     = s4_out_buf_info[1].start_pack ? '0 : {s3_tile_state_ff.top[`RBOX_TILE_POS_R],  s4_out_buf_info[1].y};
        s4_quad_info_pckt.sample_idx  =   s4_out_buf_info[0].sample_idx;
        s4_quad_info_pckt.mask        = {(s4_out_buf_info[1].start_pack ? 4'b0 : s4_out_buf_info[1].mask), s4_out_buf_info[0].mask};
        s4_quad_info_pckt.quad0_triangle_ptr = s3_triangle_state_ff.triangle_data_addr;
        s4_quad_info_pckt.quad1_triangle_ptr = s4_out_buf_info[1].start_pack ? '0 : s3_triangle_state_ff.triangle_data_addr;

        s4_out_pck_sc_req_nxt.source    = 0;
        s4_out_pck_sc_req_nxt.wdata     = 1;
        s4_out_pck_sc_req_nxt.size      = ET_LINK_HLine;
        s4_out_pck_sc_req_nxt.qwen      = s4_out_pck_req_pa[5] ? 4'b1100 : 4'b0011;
        s4_out_pck_sc_req_nxt.opcode    = ET_LINK_REQ_Write;
        s4_out_pck_sc_req_nxt.subopcode = '0;
        s4_out_pck_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, s4_hart_id);
        s4_out_pck_sc_req_nxt.address   = s4_out_pck_req_pa;
        s4_out_pck_sc_req_nxt.data      = {s4_quad_info_pckt, s4_quad_info_pckt};

        s4_out_pck_sc_req_valid_nxt[s4_out_pck_req_pa[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;

        s4_out_pck_sc_req_valid_en = 1'b1;
        s4_out_pck_sc_req_en       = 1'b1;

        s4_hart_packet_sent = 1'b1;

        s4_out_buf_pop = (s4_pending_quad_pcks_ff == 'd1);

        s4_hart_msg_send_set = ((s4_hart_sent_pckts_ff[s4_hart_id] + (2 * s4_num_quad_pcks_ff)) >= (1 << s0_rbox_esr_out_buf_cfg_ff.max_pckts_msg));
    end

    if (   s4_out_buf_packet_valid
        && (s4_pending_quad_pcks_ff > '0)
        && (!(|s4_out_pck_sc_req_valid_ff) || s4_out_pck_sc_req_ready))
    begin
        s4_pending_quad_pcks_en  = 1'b1;
        s4_pending_quad_pcks_nxt = s4_pending_quad_pcks_ff - 'd1;

        s4_out_pck_sc_req_nxt.source    = 0;
        s4_out_pck_sc_req_nxt.wdata     = 1;
        s4_out_pck_sc_req_nxt.size      = ET_LINK_HLine;
        s4_out_pck_sc_req_nxt.qwen      = s4_out_pck_req_pa[5] ? 4'b1100 : 4'b0011;
        s4_out_pck_sc_req_nxt.opcode    = ET_LINK_REQ_Write;
        s4_out_pck_sc_req_nxt.subopcode = '0;
        s4_out_pck_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, s4_hart_id);
        s4_out_pck_sc_req_nxt.address   = s4_out_pck_req_pa;
        s4_out_pck_sc_req_nxt.data      = {(s4_out_buf_info[1].start_pack ? 128'b0 : s4_out_buf_packet[255:128]), s4_out_buf_packet[127:0],
                                           (s4_out_buf_info[1].start_pack ? 128'b0 : s4_out_buf_packet[255:128]), s4_out_buf_packet[127:0]};

        s4_out_pck_sc_req_valid_nxt[s4_out_pck_req_pa[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;

        s4_out_pck_sc_req_valid_en = 1'b1;
        s4_out_pck_sc_req_en       = 1'b1;

        s4_hart_packet_sent = 1'b1;

        s4_out_buf_pop = (s4_pending_quad_pcks_ff == 'd1);
    end

    s4_out_msg_sc_req_valid_nxt = '0;
    s4_out_msg_sc_req_nxt       = '0;

    s4_out_msg_sc_req_valid_en  = reset || s4_out_msg_sc_req_ready;
    s4_out_msg_sc_req_en        = 1'b0;

    s4_hart_buf_end_ptr   = '0;
    s4_out_pck_end_req_pa = '0;

    s4_hart_end_buf_set = 1'b0;

    if (    !s4_hart_sent_end_buf_ff[s4_end_buf_hart_id_ff]
        &&  s3_end_of_buffer_ff && tr_empty && ts_empty && s4_out_buf_empty
        && !(|s4_out_pck_sc_req_valid_nxt)
        && (!(|s4_out_pck_sc_req_valid_ff) || s4_out_pck_sc_req_ready))
    begin
        s4_end_phase_pckt = '0;
        s4_end_phase_pckt.outpck_type = rbox_pkg::OutpckEndPhase;

        s4_hart_buf_end_ptr = {s0_rbox_esr_out_buf_cfg_ff.start_offset, 1'b0}
                            + (1 << s0_rbox_esr_out_buf_cfg_ff.hart_buf_lines) * s4_end_buf_hart_id_ff
                            + s4_hart_buf_off_ff[s4_end_buf_hart_id_ff];

        s4_out_pck_end_req_pa = {s0_rbox_esr_out_buf_pg_ff.page, s4_hart_buf_end_ptr, 5'b0};

        s4_out_pck_sc_req_nxt.source    = 0;
        s4_out_pck_sc_req_nxt.wdata     = 1;
        s4_out_pck_sc_req_nxt.size      = ET_LINK_HLine;
        s4_out_pck_sc_req_nxt.qwen      = s4_out_pck_end_req_pa[5] ? 4'b1100 : 4'b0011;
        s4_out_pck_sc_req_nxt.opcode    = ET_LINK_REQ_Write;
        s4_out_pck_sc_req_nxt.subopcode = '0;
        s4_out_pck_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, s4_end_buf_hart_id_ff);
        s4_out_pck_sc_req_nxt.address   = s4_out_pck_end_req_pa;
        s4_out_pck_sc_req_nxt.data      = {s4_end_phase_pckt, s4_end_phase_pckt};

        s4_out_pck_sc_req_valid_nxt[s4_out_pck_end_req_pa[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;

        s4_out_pck_sc_req_valid_en = 1'b1;
        s4_out_pck_sc_req_en       = 1'b1;

        s4_hart_packet_sent = 1'b1;

        s4_hart_end_buf_set = 1'b1;

        s4_hart_msg_send_set = 1'b1;
    end

    s4_out_msg_req_pa = '0;

    if (   s4_hart_send_msg_ff[s4_msg_send_hart_id_ff]
        && (s4_hart_pending_acks_ff[s4_msg_send_hart_id_ff] == '0)
        && (!(|s4_out_msg_sc_req_valid_ff) || s4_out_pck_sc_req_ready))
    begin

        s4_out_msg_req_pa = f_hart_msg_port_pa(s4_msg_send_hart_id_ff, s0_rbox_esr_out_buf_cfg_ff.hart_port_id);

        s4_out_msg_sc_req_nxt.source    = 0;
        s4_out_msg_sc_req_nxt.wdata     = 1;
        s4_out_msg_sc_req_nxt.size      = ET_LINK_QWord;
        s4_out_msg_sc_req_nxt.qwen      = 4'b0001;
        s4_out_msg_sc_req_nxt.opcode    = ET_LINK_REQ_MsgSendData;
        s4_out_msg_sc_req_nxt.subopcode = '0;
        s4_out_msg_sc_req_nxt.id        = `ZX(`ET_LINK_ID_SIZE, s4_msg_send_hart_id_ff) + 'h80;
        s4_out_msg_sc_req_nxt.address   = s4_out_msg_req_pa;
        s4_out_msg_sc_req_nxt.data      = `ZX(512, `ZX(64, s4_hart_sent_pckts_ff[s4_msg_send_hart_id_ff]) | (`ZX(64, s4_msg_send_hart_id_ff) << 32));

        s4_out_msg_sc_req_valid_nxt[s4_out_msg_req_pa[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;

        s4_out_msg_sc_req_valid_en = 1'b1;
        s4_out_msg_sc_req_en       = 1'b1;
    end

    s4_out_msg_sc_req_valid_nxt = reset ? '0 : s4_out_msg_sc_req_valid_nxt;

    s4_hart_msg_sent = (|s4_out_msg_sc_req_valid_ff) && (|s4_out_msg_sc_req_ready);
end

`EN_FF(clock_rbox, s4_pending_quad_pcks_en,    s4_pending_quad_pcks_ff,    s4_pending_quad_pcks_nxt   )
`EN_FF(clock_rbox, s4_out_pck_sc_req_en,       s4_out_pck_sc_req_ff,       s4_out_pck_sc_req_nxt      )
`EN_FF(clock_rbox, s4_out_pck_sc_req_valid_en, s4_out_pck_sc_req_valid_ff, s4_out_pck_sc_req_valid_nxt)
`EN_FF(clock_rbox, s4_out_msg_sc_req_en,       s4_out_msg_sc_req_ff,       s4_out_msg_sc_req_nxt      )
`EN_FF(clock_rbox, s4_out_msg_sc_req_valid_en, s4_out_msg_sc_req_valid_ff, s4_out_msg_sc_req_valid_nxt)


always_comb
begin
    tr_triangle       = rbox_triangle_state_t'(s3_triangle_state_ff);
    tr_tile           = rbox_tile_state_t'(s3_tile_state_ff);
    tr_tile_valid     = s3_tile_valid_ff;

    tr_msaa_enable          = s3_rbox_state_ff.msaa_enable && rbox_pkg::msaa_mode_is_valid(s3_rbox_state_ff.msaa_samples);
    tr_msaa_mode            = rbox_msaa_mode_t'(rbox_pkg::convert_msaa_mode(s3_rbox_state_ff.msaa_samples));
    tr_depth_is_fp32        =  (s3_rbox_state_ff.ds_buffer_format == rbox_pkg::DsFormatD32Sfloat);
    tr_depth_is_un16        =  (s3_rbox_state_ff.ds_buffer_format == rbox_pkg::DsFormatD16Unorm);
    tr_depth_is_un24        =  (s3_rbox_state_ff.ds_buffer_format == rbox_pkg::DsFormatX8D24UnormPack32)
                            || (s3_rbox_state_ff.ds_buffer_format == rbox_pkg::DsFormatD24UnormS8Uint);
    tr_scissor_min_x        = s3_scissor_min_x_ff;
    tr_scissor_max_x        = s3_scissor_max_x_ff;
    tr_scissor_min_y        = s3_scissor_min_y_ff;
    tr_scissor_max_y        = s3_scissor_max_y_ff;
    tr_depth_clamp_enable   = s3_rbox_state_ff.depth_clamp_enable;
    tr_depth_min            = s3_rbox_state_ff.depth_min;
    tr_depth_max            = s3_rbox_state_ff.depth_max;
    tr_depth_bound_enable   = s3_rbox_state_ff.depth_bound_enable;
    tr_depth_bound_min      = s3_rbox_state_ff.depth_bound_min;
    tr_depth_bound_max      = s3_rbox_state_ff.depth_bound_max;

    ts_state = rbox_state_t'(s3_rbox_state_ff);
    ts_front_triangle = (s3_triangle_state_ff.tri_facing == rbox_pkg::TriFacingFront);
end

// Selection of signals to Status Monitor
always_comb
begin
    ct_sm_data.s1_config_error             = s1_config_error;
    ct_sm_data.s1_started                  = s1_started_ff;
    ct_sm_data.s1_done                     = s1_done_ff;
    ct_sm_data.s1_sc_rbox_rsp_is_read_data = s1_sc_rbox_rsp_is_read_data;
    ct_sm_data.s1_in_pending_lines_ff      = s1_in_pending_lines_ff;
    ct_sm_data.s1_in_process_pckts_ff      = s1_in_process_pckts_ff;
    ct_sm_data.s1_in_buf_off               = s1_in_buf_off_ff;
    ct_sm_data.s1_in_sc_req_valid_ff       = s1_in_sc_req_valid_ff;
    ct_sm_data.s1_req_pa                   = s1_req_pa;
    ct_sm_data.s1_in_fifo_full             = s1_in_fifo_full;
    ct_sm_data.s1_in_fifo_empty            = s1_in_fifo_empty;
    ct_sm_data.s1_in_fifo_alloc            = s1_in_fifo_alloc;
    ct_sm_data.s1_in_fifo_alloc_ptr        = s1_in_fifo_alloc_ptr;
    ct_sm_data.s1_in_fifo_push_ptr         = s1_in_fifo_push_ptr;
    ct_sm_data.s1_in_fifo_push             = s1_in_fifo_push;
    ct_sm_data.s1_in_fifo_push_done        = s1_in_fifo_push_done;

    ct_sm_data.s2_load_rbox_state          = s2_load_rbox_state;
    ct_sm_data.s2_load_fsh_state           = s2_load_fsh_state;
    ct_sm_data.s2_load_triangle_state      = s2_load_triangle_state;
    ct_sm_data.s2_load_tile_state          = s2_load_tile_state;
    ct_sm_data.s2_input_pck_accept         = s2_input_pck_accept;

    ct_sm_data.s3_end_of_buffer_ff         = s3_end_of_buffer_ff;
    ct_sm_data.s3_tile_valid_ff            = s3_tile_valid_ff;

    ct_sm_data.s4_out_buf_info_valid       = s4_out_buf_info_valid;
    ct_sm_data.s4_out_buf_packet_valid     = s4_out_buf_packet_valid;
    ct_sm_data.s4_out_buf_info             = s4_out_buf_info;
    ct_sm_data.s4_hart_id                  = s4_hart_id;
    ct_sm_data.s4_out_msg_sc_req_valid_ff  = s4_out_msg_sc_req_valid_ff;
    ct_sm_data.s4_out_pck_sc_req_valid_ff  = s4_out_pck_sc_req_valid_ff;
    ct_sm_data.s4_out_msg_req_pa           = s4_out_msg_req_pa;
    ct_sm_data.s4_out_pck_req_pa           = s4_out_pck_req_pa;
end

endmodule

/* verilator lint_on CASEINCOMPLETE */
/* verilator lint_on BLKSEQ */
/* verilator lint_on MULTIDRIVEN */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNUSEDSIGNAL */
