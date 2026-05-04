// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original esr_rbox vs new rbox_esr.
//
// The original esr_rbox uses:
//   - reset_c, reset_w, reset_d (active-high synchronous)
//   - reset_c: APB pipeline state (s1_addr_valid)
//   - reset_w: enable pulse FFs and register storage (ESR_WRITE_SEQ_RST_ND)
//   - reset_d: unused (passed through for test_en logic)
//
// The new rbox_esr uses:
//   - rst_w_ni (active-low async) for ALL state (since reset_c = reset_w in rbox_top)
//
// For the cosim, we drive rst_w_ni and derive reset_c = reset_w = ~rst_w_ni,
// reset_d = ~rst_w_ni (both resets active together).

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off BLKSEQ */

module cosim_rbox_esr_tb (
  input  logic        clk_i,
  input  logic        rst_w_ni,

  // ── Shared stimulus: APB request ────────────────────────
  input  logic [15:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,

  // ── Shared stimulus: status register ────────────────────
  input  logic [31:0] status_i,

  // ── New module outputs ──────────────────────────────────
  output logic        new_pready_o,
  output logic [63:0] new_prdata_o,
  output logic        new_pslverr_o,
  output logic [63:0] new_config_o,
  output logic        new_config_en_o,
  output logic [63:0] new_in_buf_pg_o,
  output logic        new_in_buf_pg_en_o,
  output logic [63:0] new_in_buf_cfg_o,
  output logic        new_in_buf_cfg_en_o,
  output logic [31:0] new_out_buf_pg_o,
  output logic        new_out_buf_pg_en_o,
  output logic [63:0] new_out_buf_cfg_o,
  output logic        new_out_buf_cfg_en_o,
  output logic [31:0] new_start_o,
  output logic        new_start_en_o,
  output logic [31:0] new_consume_o,
  output logic        new_consume_en_o,

  // ── Original module outputs ─────────────────────────────
  output logic        orig_pready_o,
  output logic [63:0] orig_prdata_o,
  output logic        orig_pslverr_o,
  output logic [63:0] orig_config_o,
  output logic        orig_config_en_o,
  output logic [63:0] orig_in_buf_pg_o,
  output logic        orig_in_buf_pg_en_o,
  output logic [63:0] orig_in_buf_cfg_o,
  output logic        orig_in_buf_cfg_en_o,
  output logic [31:0] orig_out_buf_pg_o,
  output logic        orig_out_buf_pg_en_o,
  output logic [63:0] orig_out_buf_cfg_o,
  output logic        orig_out_buf_cfg_en_o,
  output logic [31:0] orig_start_o,
  output logic        orig_start_en_o,
  output logic [31:0] orig_consume_o,
  output logic        orig_consume_en_o
);

  import apb_pkg::*;

  // ── Reset adaptation ────────────────────────────────────
  logic reset_c, reset_w, reset_d;
  assign reset_c = ~rst_w_ni;
  assign reset_w = ~rst_w_ni;
  assign reset_d = ~rst_w_ni;

  // ── New instance ────────────────────────────────────────
  apb_pkg::req_t new_apb_req;
  apb_pkg::rsp_t new_apb_rsp;

  assign new_apb_req.paddr   = apb_paddr_i;
  assign new_apb_req.pwrite  = apb_pwrite_i;
  assign new_apb_req.psel    = apb_psel_i;
  assign new_apb_req.penable = apb_penable_i;
  assign new_apb_req.pwdata  = apb_pwdata_i;

  assign new_pready_o  = new_apb_rsp.pready;
  assign new_prdata_o  = new_apb_rsp.prdata;
  assign new_pslverr_o = new_apb_rsp.pslverr;

  rbox_esr u_new (
    .clk_i          (clk_i),
    .rst_w_ni       (rst_w_ni),
    .apb_req_i      (new_apb_req),
    .apb_rsp_o      (new_apb_rsp),
    .config_o       (new_config_o),
    .config_en_o    (new_config_en_o),
    .in_buf_pg_o    (new_in_buf_pg_o),
    .in_buf_pg_en_o (new_in_buf_pg_en_o),
    .in_buf_cfg_o   (new_in_buf_cfg_o),
    .in_buf_cfg_en_o(new_in_buf_cfg_en_o),
    .out_buf_pg_o   (new_out_buf_pg_o),
    .out_buf_pg_en_o(new_out_buf_pg_en_o),
    .out_buf_cfg_o  (new_out_buf_cfg_o),
    .out_buf_cfg_en_o(new_out_buf_cfg_en_o),
    .start_o        (new_start_o),
    .start_en_o     (new_start_en_o),
    .consume_o      (new_consume_o),
    .consume_en_o   (new_consume_en_o),
    .status_i       (status_i)
  );

  // ── Original instance ───────────────────────────────────
  esr_rbox #(.AD_WIDTH(16)) u_orig (
    .clock                   (clk_i),
    .reset_c                 (reset_c),
    .reset_w                 (reset_w),
    .reset_d                 (reset_d),
    .test_en                 (1'b0),
    .esr_rbox_config_op      (orig_config_o),
    .esr_rbox_in_buf_pg_op   (orig_in_buf_pg_o),
    .esr_rbox_in_buf_cfg_op  (orig_in_buf_cfg_o),
    .esr_rbox_out_buf_pg_op  (orig_out_buf_pg_o),
    .esr_rbox_out_buf_cfg_op (orig_out_buf_cfg_o),
    .esr_rbox_status_ip      (status_i),
    .esr_rbox_start_op       (orig_start_o),
    .esr_rbox_consume_op     (orig_consume_o),
    .esr_rbox_config_en      (orig_config_en_o),
    .esr_rbox_in_buf_pg_en   (orig_in_buf_pg_en_o),
    .esr_rbox_in_buf_cfg_en  (orig_in_buf_cfg_en_o),
    .esr_rbox_out_buf_pg_en  (orig_out_buf_pg_en_o),
    .esr_rbox_out_buf_cfg_en (orig_out_buf_cfg_en_o),
    .esr_rbox_start_en       (orig_start_en_o),
    .esr_rbox_consume_en     (orig_consume_en_o),
    .apb_paddr               (apb_paddr_i),
    .apb_pwrite              (apb_pwrite_i),
    .apb_psel                (apb_psel_i),
    .apb_penable             (apb_penable_i),
    .apb_pwdata              (apb_pwdata_i),
    .apb_pready              (orig_pready_o),
    .apb_prdata              (orig_prdata_o),
    .apb_pslverr             (orig_pslverr_o)
  );

endmodule

/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNOPTFLAT */
