// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX top-level integration.
//
// Instantiates all RBOX sub-modules:
//   - rbox_esr       — APB ESR register block
//   - rbox_dbg_ram   — APB debug/ESR address decode mux
//   - rbox_ctrl      — master controller (input/output buffers, SC arb)
//   - rbox_traverse  — triangle rasterization pipeline
//   - rbox_test      — depth/stencil test pipeline
//   - prim_clk_gate  — gated RBOX clock
//
// Also implements the debug run control FSM (halt/resume/ack).
//
// Reset domains (matching original rbox_top.v):
//   - rst_d_ni (debug reset): halt/resume FSM
//   - rst_w_ni (warm reset): rbox_ctrl, rbox_traverse, rbox_test, ESR,
//     clock gate enable
//
// Differences from original (rbox_top.v):
//   - Reset: active-low async-assert/sync-deassert (rst_d_ni, rst_w_ni)
//     instead of active-high synchronous (reset_d, reset_w)
//   - Naming: lowRISC _i/_o/_d/_q conventions
//   - Types: rbox_pkg/etlink_pkg/apb_pkg instead of `include "soc.vh"
//   - UltraSoC status monitor (rbox_sm_*) dropped entirely
//   - DFT via dft_pkg::dft_t, RAM config via ram_cfg_pkg::ram_cfg_t
//   - Flip-flop macros replaced with explicit always_ff

/* verilator lint_off UNUSEDSIGNAL */  // shire_id, sm_ctrl_t fields, dbg_db_meta_read_valid — unused per original
/* verilator lint_off WIDTHEXPAND */   // Zero-extension in run control logic
/* verilator lint_off UNOPTFLAT */     // rc FSM: independent fields in same always_comb — acyclic

module rbox_top
  import rbox_pkg::*;
  import etlink_pkg::*;
  import apb_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_d_ni,   // Debug reset (active-low async)
  input  logic                       rst_w_ni,   // Warm reset (active-low async)

  // Configuration
  input  logic [ShireIdSize-1:0]     shire_id_i,

  // Request to Shire Cache
  output etlink_pkg::req_t           rbox_sc_req_o,
  output logic [ScBanks:0]           rbox_sc_req_valid_o,
  input  logic [ScBanks:0]           rbox_sc_req_ready_i,

  // Response from Shire Cache
  input  etlink_pkg::rsp_t           sc_rbox_rsp_i,
  input  logic                       sc_rbox_rsp_valid_i,
  output logic                       sc_rbox_rsp_ready_o,

  // Debug run control
  input  rc_t                        rc_i,
  output rc_ack_t                    rc_ack_o,

  // APB interface
  input  apb_pkg::req_t              apb_req_i,
  output apb_pkg::rsp_t              apb_rsp_o,

  // RAM configuration and DFT
  input  ram_cfg_t                   ram_cfg_i,
  input  dft_t                       dft_i
);

  // ════════════════════════════════════════════════════════════
  // Internal signals
  // ════════════════════════════════════════════════════════════

  logic                          rbox_enable;
  logic                          tr_msaa_enable;
  msaa_mode_e                    tr_msaa_mode;
  logic                          tr_depth_is_fp32;
  logic                          tr_depth_is_un16;
  logic                          tr_depth_is_un24;
  logic [MaxRenderDimL-1:0]      tr_scissor_min_x;
  logic [MaxRenderDimL-1:0]      tr_scissor_max_x;
  logic [MaxRenderDimL-1:0]      tr_scissor_min_y;
  logic [MaxRenderDimL-1:0]      tr_scissor_max_y;
  logic                          tr_depth_clamp_enable;
  logic [31:0]                   tr_depth_min;
  logic [31:0]                   tr_depth_max;
  logic                          tr_depth_bound_enable;
  logic [31:0]                   tr_depth_bound_min;
  logic [31:0]                   tr_depth_bound_max;
  triangle_state_t               tr_triangle;
  tile_state_t                   tr_tile;
  logic                          tr_tile_valid;
  logic                          tr_accept;
  logic                          tr_empty;
  quad_data_t                    tr_quad_data;
  logic                          tr_quad_data_valid;
  quad_test_t                    tr_quad_test;
  logic                          tr_quad_test_valid;
  logic                          ct_quad_accept;
  logic                          ts_quad_accept;
  logic [3:0]                    ts_result_mask;
  logic                          ts_result_valid;
  logic                          ct_result_accept;
  state_t                        ts_state;
  logic                          ts_front_triangle;
  logic                          ts_empty;
  logic                          db_flush;
  logic                          db_flush_done;
  etlink_pkg::req_t              db_sc_req;
  logic [ScBanks:0]              db_sc_req_valid;
  logic                          db_sc_req_ready;
  etlink_pkg::rsp_t              sc_db_rsp;
  logic                          sc_db_rsp_valid;
  sm_ctrl_t                      ct_sm_data;

  // ── ESR signals ─────────────────────────────────────────

  logic [63:0]              rbox_esr_config;
  logic                     rbox_esr_config_en;
  logic [63:0]              rbox_esr_in_buf_pg;
  logic                     rbox_esr_in_buf_pg_en;
  logic [63:0]              rbox_esr_in_buf_cfg;
  logic                     rbox_esr_in_buf_cfg_en;
  logic [31:0]              rbox_esr_out_buf_pg;
  logic                     rbox_esr_out_buf_pg_en;
  logic [63:0]              rbox_esr_out_buf_cfg;
  logic                     rbox_esr_out_buf_cfg_en;
  logic [31:0]              rbox_esr_status;
  logic [31:0]              rbox_esr_start;
  logic                     rbox_esr_start_en;
  logic [31:0]              rbox_esr_consume;
  logic                     rbox_esr_consume_en;

  // ── Debug RAM signals ───────────────────────────────────

  logic [DbgPtrIdxSz-1:0]          dbg_ram_ptr;

  logic                            dbg_db_read_valid;
  logic [DepthLineSz-1:0]          dbg_db_read_data;

  logic [DepthLineSz-1:0]          dbg_db_write_data;
  logic                            dbg_db_write_valid;

  logic                            dbg_db_meta_read_valid;
  logic [DbgDbMetaDataSz-1:0]     dbg_db_meta_read_data;

  logic [DbgDbMetaDataSz-1:0]     dbg_db_meta_write_data;
  logic                            dbg_db_meta_write_valid;

  // ── APB ESR internal ────────────────────────────────────

  apb_pkg::req_t esr_apb_req;
  apb_pkg::rsp_t esr_apb_rsp;

  // ════════════════════════════════════════════════════════════
  // ESR register block (warm reset domain — original: reset_c=reset_w, reset_w=reset_w)
  // ════════════════════════════════════════════════════════════

  rbox_esr u_esr (
    .clk_i          (clk_i),
    .rst_w_ni       (rst_w_ni),
    .apb_req_i      (esr_apb_req),
    .apb_rsp_o      (esr_apb_rsp),
    .config_o       (rbox_esr_config),
    .config_en_o    (rbox_esr_config_en),
    .in_buf_pg_o    (rbox_esr_in_buf_pg),
    .in_buf_pg_en_o (rbox_esr_in_buf_pg_en),
    .in_buf_cfg_o   (rbox_esr_in_buf_cfg),
    .in_buf_cfg_en_o(rbox_esr_in_buf_cfg_en),
    .out_buf_pg_o   (rbox_esr_out_buf_pg),
    .out_buf_pg_en_o(rbox_esr_out_buf_pg_en),
    .out_buf_cfg_o  (rbox_esr_out_buf_cfg),
    .out_buf_cfg_en_o(rbox_esr_out_buf_cfg_en),
    .start_o        (rbox_esr_start),
    .start_en_o     (rbox_esr_start_en),
    .consume_o      (rbox_esr_consume),
    .consume_en_o   (rbox_esr_consume_en),
    .status_i       (rbox_esr_status)
  );

  // ESR pready qualified by penable && psel (matches original rbox_top.v)
  apb_pkg::rsp_t esr_apb_rsp_qualified;

  always_comb begin
    esr_apb_rsp_qualified = esr_apb_rsp;
    esr_apb_rsp_qualified.pready = esr_apb_rsp.pready &&
                                   esr_apb_req.penable &&
                                   esr_apb_req.psel;
  end

  // ════════════════════════════════════════════════════════════
  // Debug RAM (APB decode mux — uses both rst_d_ni and rst_w_ni)
  // ════════════════════════════════════════════════════════════

  rbox_dbg_ram u_dbg_ram (
    .clk_i                    (clk_i),
    .rst_d_ni                 (rst_d_ni),
    .rst_w_ni                 (rst_w_ni),
    .apb_req_i                (apb_req_i),
    .apb_rsp_o                (apb_rsp_o),
    .esr_apb_req_o            (esr_apb_req),
    .esr_apb_rsp_i            (esr_apb_rsp_qualified),
    .dbg_ram_ptr_o            (dbg_ram_ptr),
    .dbg_db_read_valid_o      (dbg_db_read_valid),
    .dbg_db_read_data_i       (dbg_db_read_data),
    .dbg_db_write_data_o      (dbg_db_write_data),
    .dbg_db_write_valid_o     (dbg_db_write_valid),
    .dbg_db_meta_read_valid_o (dbg_db_meta_read_valid),
    .dbg_db_meta_read_data_i  (dbg_db_meta_read_data),
    .dbg_db_meta_write_data_o (dbg_db_meta_write_data),
    .dbg_db_meta_write_valid_o(dbg_db_meta_write_valid)
  );

  // ════════════════════════════════════════════════════════════
  // Debug Run Control FSM (debug reset domain)
  // ════════════════════════════════════════════════════════════

  logic rbox_halted_q;
  logic rbox_halted_d;
  logic rbox_halted_en;

  logic rbox_halt_ack_d;
  logic rbox_halt_ack_q;
  logic rbox_resume_ack_d;
  logic rbox_resume_ack_q;

  always_comb begin
    rbox_halted_en  = (!rbox_halted_q && (rc_i.events.halt_req   || rc_i.gpio.halt_req))
                    || (rbox_halted_q && (rc_i.events.resume_req || rc_i.gpio.resume_req));
    rbox_halted_d   = (rbox_halted_q && (!rc_i.events.resume_req || !rc_i.gpio.resume_req))
                    || (!rbox_halted_q && (rc_i.events.halt_req   || rc_i.gpio.halt_req));

    rbox_halt_ack_d   = rbox_halted_en && !rbox_halted_q;
    rbox_resume_ack_d = rbox_halted_en &&  rbox_halted_q;

    rc_ack_o.halt_ack   = rbox_halt_ack_q;
    rc_ack_o.resume_ack = rbox_resume_ack_q;
    rc_ack_o.reset_ack  = 1'b0;
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      rbox_halted_q    <= 1'b0;
      rbox_halt_ack_q  <= 1'b0;
      rbox_resume_ack_q <= 1'b0;
    end else begin
      if (rbox_halted_en)
        rbox_halted_q <= rbox_halted_d;
      rbox_halt_ack_q   <= rbox_halt_ack_d;
      rbox_resume_ack_q <= rbox_resume_ack_d;
    end
  end

  // ════════════════════════════════════════════════════════════
  // Clock gating (warm reset domain — original: rbox_ck_enable = rbox_enable || reset_w)
  // ════════════════════════════════════════════════════════════

  logic clk_rbox;
  logic rbox_ck_enable;

  assign rbox_ck_enable = rbox_enable || !rst_w_ni;

  prim_clk_gate u_rbox_cgate (
    .clk_i  (clk_i),
    .en_i   (rbox_ck_enable),
    .dft_i  (dft_i),
    .clk_o  (clk_rbox)
  );

  // ════════════════════════════════════════════════════════════
  // Controller (warm reset domain — original: reset = reset_w)
  // ════════════════════════════════════════════════════════════

  rbox_ctrl u_ctrl (
    .clk_i                (clk_i),
    .clk_rbox_i           (clk_rbox),
    .rst_ni               (rst_w_ni),
    .ram_cfg_i            (ram_cfg_i),
    .dft_i                (dft_i),
    .rbox_enable_o        (rbox_enable),
    .rbox_sc_req_o        (rbox_sc_req_o),
    .rbox_sc_req_valid_o  (rbox_sc_req_valid_o),
    .rbox_sc_req_ready_i  (rbox_sc_req_ready_i),
    .sc_rbox_rsp_i        (sc_rbox_rsp_i),
    .sc_rbox_rsp_valid_i  (sc_rbox_rsp_valid_i),
    .sc_rbox_rsp_ready_o  (sc_rbox_rsp_ready_o),
    .db_sc_req_i          (db_sc_req),
    .db_sc_req_valid_i    (db_sc_req_valid),
    .db_sc_req_ready_o    (db_sc_req_ready),
    .sc_db_rsp_o          (sc_db_rsp),
    .sc_db_rsp_valid_o    (sc_db_rsp_valid),
    .esr_config_i         (rbox_esr_config),
    .esr_config_en_i      (rbox_esr_config_en),
    .esr_in_buf_pg_i      (rbox_esr_in_buf_pg),
    .esr_in_buf_pg_en_i   (rbox_esr_in_buf_pg_en),
    .esr_in_buf_cfg_i     (rbox_esr_in_buf_cfg),
    .esr_in_buf_cfg_en_i  (rbox_esr_in_buf_cfg_en),
    .esr_out_buf_pg_i     (rbox_esr_out_buf_pg),
    .esr_out_buf_pg_en_i  (rbox_esr_out_buf_pg_en),
    .esr_out_buf_cfg_i    (rbox_esr_out_buf_cfg),
    .esr_out_buf_cfg_en_i (rbox_esr_out_buf_cfg_en),
    .esr_start_i          (rbox_esr_start),
    .esr_start_en_i       (rbox_esr_start_en),
    .esr_consume_i        (rbox_esr_consume),
    .esr_consume_en_i     (rbox_esr_consume_en),
    .esr_status_o         (rbox_esr_status),
    .tr_msaa_enable_o     (tr_msaa_enable),
    .tr_msaa_mode_o       (tr_msaa_mode),
    .tr_depth_is_fp32_o   (tr_depth_is_fp32),
    .tr_depth_is_un16_o   (tr_depth_is_un16),
    .tr_depth_is_un24_o   (tr_depth_is_un24),
    .tr_scissor_min_x_o   (tr_scissor_min_x),
    .tr_scissor_max_x_o   (tr_scissor_max_x),
    .tr_scissor_min_y_o   (tr_scissor_min_y),
    .tr_scissor_max_y_o   (tr_scissor_max_y),
    .tr_depth_clamp_enable_o (tr_depth_clamp_enable),
    .tr_depth_min_o       (tr_depth_min),
    .tr_depth_max_o       (tr_depth_max),
    .tr_depth_bound_enable_o (tr_depth_bound_enable),
    .tr_depth_bound_min_o (tr_depth_bound_min),
    .tr_depth_bound_max_o (tr_depth_bound_max),
    .tr_triangle_o        (tr_triangle),
    .tr_tile_o            (tr_tile),
    .tr_tile_valid_o      (tr_tile_valid),
    .tr_accept_i          (tr_accept),
    .tr_empty_i           (tr_empty),
    .tr_quad_i            (tr_quad_data),
    .tr_quad_valid_i      (tr_quad_data_valid),
    .ct_quad_accept_o     (ct_quad_accept),
    .ts_state_o           (ts_state),
    .ts_front_triangle_o  (ts_front_triangle),
    .ts_empty_i           (ts_empty),
    .ts_result_mask_i     (ts_result_mask),
    .ts_result_valid_i    (ts_result_valid),
    .ct_result_accept_o   (ct_result_accept),
    .db_flush_o           (db_flush),
    .db_flush_done_i      (db_flush_done),
    .ct_sm_data_o         (ct_sm_data),
    .rbox_halted_i        (rbox_halted_q)
  );

  // ════════════════════════════════════════════════════════════
  // Traverse (warm reset domain — original: reset = reset_w)
  // ════════════════════════════════════════════════════════════

  rbox_traverse u_traverse (
    .clk_i                (clk_rbox),
    .rst_ni               (rst_w_ni),
    .tr_triangle_i        (tr_triangle),
    .tr_tile_i            (tr_tile),
    .tr_tile_valid_i      (tr_tile_valid),
    .tr_accept_o          (tr_accept),
    .tr_empty_o           (tr_empty),
    .tr_quad_data_o       (tr_quad_data),
    .tr_quad_data_valid_o (tr_quad_data_valid),
    .tr_quad_test_o       (tr_quad_test),
    .tr_quad_test_valid_o (tr_quad_test_valid),
    .ct_quad_accept_i     (ct_quad_accept),
    .ts_quad_accept_i     (ts_quad_accept),
    .tr_msaa_enable_i     (tr_msaa_enable),
    .tr_msaa_mode_i       (tr_msaa_mode),
    .tr_depth_is_fp32_i   (tr_depth_is_fp32),
    .tr_depth_is_un24_i   (tr_depth_is_un24),
    .tr_depth_is_un16_i   (tr_depth_is_un16),
    .tr_scissor_min_x_i   (tr_scissor_min_x),
    .tr_scissor_max_x_i   (tr_scissor_max_x),
    .tr_scissor_min_y_i   (tr_scissor_min_y),
    .tr_scissor_max_y_i   (tr_scissor_max_y),
    .tr_depth_clamp_enable_i (tr_depth_clamp_enable),
    .tr_depth_min_i       (tr_depth_min),
    .tr_depth_max_i       (tr_depth_max),
    .tr_depth_bound_enable_i (tr_depth_bound_enable),
    .tr_depth_bound_min_i (tr_depth_bound_min),
    .tr_depth_bound_max_i (tr_depth_bound_max)
  );

  // ════════════════════════════════════════════════════════════
  // Test (depth/stencil) — warm reset domain (original: reset = reset_w)
  // ════════════════════════════════════════════════════════════

  rbox_test u_test (
    .clk_i                   (clk_i),
    .clk_rbox_i              (clk_rbox),
    .rst_ni                  (rst_w_ni),
    .ram_cfg_i               (ram_cfg_i),
    .dft_i                   (dft_i),
    .db_sc_req_o             (db_sc_req),
    .db_sc_req_valid_o       (db_sc_req_valid),
    .db_sc_req_ready_i       (db_sc_req_ready),
    .sc_db_rsp_i             (sc_db_rsp),
    .sc_db_rsp_valid_i       (sc_db_rsp_valid),
    .ts_quad_test_i          (tr_quad_test),
    .ts_quad_valid_i         (tr_quad_test_valid),
    .ts_quad_accept_o        (ts_quad_accept),
    .ts_result_mask_o        (ts_result_mask),
    .ts_result_valid_o       (ts_result_valid),
    .ct_result_accept_i      (ct_result_accept),
    .ts_state_i              (ts_state),
    .ts_front_triangle_i     (ts_front_triangle),
    .ts_empty_o              (ts_empty),
    .db_flush_i              (db_flush),
    .db_flush_done_o         (db_flush_done),
    .dbg_ram_ptr_i           (dbg_ram_ptr),
    .dbg_db_read_valid_i     (dbg_db_read_valid),
    .dbg_db_read_data_o      (dbg_db_read_data),
    .dbg_db_write_data_i     (dbg_db_write_data),
    .dbg_db_write_valid_i    (dbg_db_write_valid),
    .dbg_db_meta_read_data_o (dbg_db_meta_read_data),
    .dbg_db_meta_write_data_i(dbg_db_meta_write_data),
    .dbg_db_meta_write_valid_i(dbg_db_meta_write_valid)
  );

  // ── Unused signals ─────────────────────────────────────
  logic unused_ok;
  assign unused_ok = &{1'b0,
    shire_id_i,
    dbg_db_meta_read_valid,
    ct_sm_data
  };

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNOPTFLAT */
