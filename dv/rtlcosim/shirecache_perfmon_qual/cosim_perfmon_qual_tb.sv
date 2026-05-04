// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_perfmon_qual
// vs new shirecache_bank_perfmon_qual.
//
// Both modules are purely combinational (no clock or reset).
// clk_i/rst_ni exist only for the Verilator testbench framework.
// C++ sets inputs, evaluates, and compares outputs.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
module cosim_perfmon_qual_tb
  import shirecache_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  // ── Scalar inputs ─────────────────────────────────────
  input  logic        perfmon_enable_i,
  input  logic        perfmon_event_i,
  input  logic [7:0]  perfmon_mode_i,
  input  logic [50:0] perfmon_qual_i,
  input  logic [40:0] esr_sc_perfmon_cntr_i,

  // ── Event rsp scalars ─────────────────────────────────
  input  logic        ev_rbuf_l2_hit_i,
  input  logic        ev_rbuf_scp_hit_i,
  input  logic        ev_msgsenddata_i,
  input  logic        ev_tag_bubble_i,
  input  logic        ev_tag_hit_i,
  input  logic        ev_tag_miss_i,
  input  logic        ev_tag_evict_dirty_i,
  input  logic        ev_tag_evict_write_around_i,
  input  logic        ev_tag_evict_none_i,
  input  logic [3:0]  ev_tag_victim_qwen_i,
  input  logic [5:0]  ev_tag_opcode_i,

  // ── Resource rsp scalar ───────────────────────────────
  input  logic [6:0]  rsrc_cntr_i,

  // ── P0 original outputs ───────────────────────────────
  output logic        orig_p0_req_cgater_o,
  output logic [2:0]  orig_p0_req_ev_mode_o,
  output logic [4:0]  orig_p0_req_rsrc_mode_o,
  output logic        orig_p0_update_cntr_o,
  output logic [40:0] orig_p0_update_cntr_val_o,

  // ── P0 new outputs ────────────────────────────────────
  output logic        new_p0_req_cgater_o,
  output logic [2:0]  new_p0_req_ev_mode_o,
  output logic [4:0]  new_p0_req_rsrc_mode_o,
  output logic        new_p0_update_cntr_o,
  output logic [40:0] new_p0_update_cntr_val_o,

  // ── P1 original outputs ───────────────────────────────
  output logic        orig_p1_req_cgater_o,
  output logic [2:0]  orig_p1_req_ev_mode_o,
  output logic [4:0]  orig_p1_req_rsrc_mode_o,
  output logic        orig_p1_update_cntr_o,
  output logic [40:0] orig_p1_update_cntr_val_o,

  // ── P1 new outputs ────────────────────────────────────
  output logic        new_p1_req_cgater_o,
  output logic [2:0]  new_p1_req_ev_mode_o,
  output logic [4:0]  new_p1_req_rsrc_mode_o,
  output logic        new_p1_update_cntr_o,
  output logic [40:0] new_p1_update_cntr_val_o
);

  // ══════════════════════════════════════════════════════════
  // Build rsp structs from scalars
  // ══════════════════════════════════════════════════════════

  // Original struct
  sc_perfmon_rsp_info_t orig_rsp_info;
  assign orig_rsp_info.ev_rsp.ev_rbuf_l2_hit            = ev_rbuf_l2_hit_i;
  assign orig_rsp_info.ev_rsp.ev_rbuf_scp_hit           = ev_rbuf_scp_hit_i;
  assign orig_rsp_info.ev_rsp.ev_msgsenddata             = ev_msgsenddata_i;
  assign orig_rsp_info.ev_rsp.ev_tag_bubble              = ev_tag_bubble_i;
  assign orig_rsp_info.ev_rsp.ev_tag_hit                 = ev_tag_hit_i;
  assign orig_rsp_info.ev_rsp.ev_tag_miss                = ev_tag_miss_i;
  assign orig_rsp_info.ev_rsp.ev_tag_evict_dirty         = ev_tag_evict_dirty_i;
  assign orig_rsp_info.ev_rsp.ev_tag_evict_write_around  = ev_tag_evict_write_around_i;
  assign orig_rsp_info.ev_rsp.ev_tag_evict_none          = ev_tag_evict_none_i;
  assign orig_rsp_info.ev_rsp.ev_tag_victim_qwen         = ev_tag_victim_qwen_i;
  assign orig_rsp_info.ev_rsp.ev_tag_opcode              = sc_reqq_opcode_t'(ev_tag_opcode_i);
  assign orig_rsp_info.rsrc_rsp.rsrc_cntr                = rsrc_cntr_i;

  // New struct
  perfmon_rsp_info_t new_rsp_info;
  assign new_rsp_info.ev_rsp.ev_rbuf_l2_hit            = ev_rbuf_l2_hit_i;
  assign new_rsp_info.ev_rsp.ev_rbuf_scp_hit           = ev_rbuf_scp_hit_i;
  assign new_rsp_info.ev_rsp.ev_msgsenddata             = ev_msgsenddata_i;
  assign new_rsp_info.ev_rsp.ev_tag_bubble              = ev_tag_bubble_i;
  assign new_rsp_info.ev_rsp.ev_tag_hit                 = ev_tag_hit_i;
  assign new_rsp_info.ev_rsp.ev_tag_miss                = ev_tag_miss_i;
  assign new_rsp_info.ev_rsp.ev_tag_evict_dirty         = ev_tag_evict_dirty_i;
  assign new_rsp_info.ev_rsp.ev_tag_evict_write_around  = ev_tag_evict_write_around_i;
  assign new_rsp_info.ev_rsp.ev_tag_evict_none          = ev_tag_evict_none_i;
  assign new_rsp_info.ev_rsp.ev_tag_victim_qwen         = ev_tag_victim_qwen_i;
  assign new_rsp_info.ev_rsp.ev_tag_opcode              = reqq_opcode_e'(ev_tag_opcode_i);
  assign new_rsp_info.rsrc_rsp.rsrc_cntr                = rsrc_cntr_i;

  // ══════════════════════════════════════════════════════════
  // P0 instances (IsP0 = 1)
  // ══════════════════════════════════════════════════════════

  // Original P0
  sc_perfmon_req_info_t  orig_p0_req_info;
  logic                  orig_p0_update_cntr;
  logic [40:0]           orig_p0_update_cntr_val;

  shire_cache_bank_perfmon_qual #(
    .IS_P0             (1),
    .PERFMON_INCR_SIZE (7),
    .PERFMON_COVF_SIZE (41),
    .PERFMON_MODE_SIZE (8),
    .PERFMON_QUAL_LSB  (0),
    .PERFMON_QUAL_SIZE (51)
  ) u_orig_p0 (
    .perfmon_req_info           (orig_p0_req_info),
    .perfmon_rsp_info           (orig_rsp_info),
    .perfmon_update_cntr        (orig_p0_update_cntr),
    .esr_sc_perfmon_update_cntr (orig_p0_update_cntr_val),
    .esr_sc_perfmon_cntr        (esr_sc_perfmon_cntr_i),
    .perfmon_enable             (perfmon_enable_i),
    .perfmon_event              (perfmon_event_i),
    .perfmon_mode               (perfmon_mode_i),
    .perfmon_qual               (perfmon_qual_i)
  );

  assign orig_p0_req_cgater_o    = orig_p0_req_info.cgater;
  assign orig_p0_req_ev_mode_o   = orig_p0_req_info.ev_mode;
  assign orig_p0_req_rsrc_mode_o = orig_p0_req_info.rsrc_mode;
  assign orig_p0_update_cntr_o     = orig_p0_update_cntr;
  assign orig_p0_update_cntr_val_o = orig_p0_update_cntr_val;

  // New P0
  perfmon_req_info_t     new_p0_req_info;
  logic                  new_p0_update_cntr;
  logic [40:0]           new_p0_update_cntr_val;

  shirecache_bank_perfmon_qual #(
    .IsP0            (1),
    .PerfmonCovfSize (41),
    .PerfmonModeSize (8),
    .PerfmonQualLsb  (0),
    .PerfmonQualSize (51)
  ) u_new_p0 (
    .perfmon_req_info_o           (new_p0_req_info),
    .perfmon_rsp_info_i           (new_rsp_info),
    .perfmon_update_cntr_o        (new_p0_update_cntr),
    .esr_sc_perfmon_update_cntr_o (new_p0_update_cntr_val),
    .esr_sc_perfmon_cntr_i        (esr_sc_perfmon_cntr_i),
    .perfmon_enable_i             (perfmon_enable_i),
    .perfmon_event_i              (perfmon_event_i),
    .perfmon_mode_i               (perfmon_mode_i),
    .perfmon_qual_i               (perfmon_qual_i)
  );

  assign new_p0_req_cgater_o    = new_p0_req_info.cgater;
  assign new_p0_req_ev_mode_o   = new_p0_req_info.ev_mode;
  assign new_p0_req_rsrc_mode_o = new_p0_req_info.rsrc_mode;
  assign new_p0_update_cntr_o     = new_p0_update_cntr;
  assign new_p0_update_cntr_val_o = new_p0_update_cntr_val;

  // ══════════════════════════════════════════════════════════
  // P1 instances (IsP0 = 0)
  // ══════════════════════════════════════════════════════════

  // Original P1
  sc_perfmon_req_info_t  orig_p1_req_info;
  logic                  orig_p1_update_cntr;
  logic [40:0]           orig_p1_update_cntr_val;

  shire_cache_bank_perfmon_qual #(
    .IS_P0             (0),
    .PERFMON_INCR_SIZE (7),
    .PERFMON_COVF_SIZE (41),
    .PERFMON_MODE_SIZE (8),
    .PERFMON_QUAL_LSB  (0),
    .PERFMON_QUAL_SIZE (51)
  ) u_orig_p1 (
    .perfmon_req_info           (orig_p1_req_info),
    .perfmon_rsp_info           (orig_rsp_info),
    .perfmon_update_cntr        (orig_p1_update_cntr),
    .esr_sc_perfmon_update_cntr (orig_p1_update_cntr_val),
    .esr_sc_perfmon_cntr        (esr_sc_perfmon_cntr_i),
    .perfmon_enable             (perfmon_enable_i),
    .perfmon_event              (perfmon_event_i),
    .perfmon_mode               (perfmon_mode_i),
    .perfmon_qual               (perfmon_qual_i)
  );

  assign orig_p1_req_cgater_o    = orig_p1_req_info.cgater;
  assign orig_p1_req_ev_mode_o   = orig_p1_req_info.ev_mode;
  assign orig_p1_req_rsrc_mode_o = orig_p1_req_info.rsrc_mode;
  assign orig_p1_update_cntr_o     = orig_p1_update_cntr;
  assign orig_p1_update_cntr_val_o = orig_p1_update_cntr_val;

  // New P1
  perfmon_req_info_t     new_p1_req_info;
  logic                  new_p1_update_cntr;
  logic [40:0]           new_p1_update_cntr_val;

  shirecache_bank_perfmon_qual #(
    .IsP0            (0),
    .PerfmonCovfSize (41),
    .PerfmonModeSize (8),
    .PerfmonQualLsb  (0),
    .PerfmonQualSize (51)
  ) u_new_p1 (
    .perfmon_req_info_o           (new_p1_req_info),
    .perfmon_rsp_info_i           (new_rsp_info),
    .perfmon_update_cntr_o        (new_p1_update_cntr),
    .esr_sc_perfmon_update_cntr_o (new_p1_update_cntr_val),
    .esr_sc_perfmon_cntr_i        (esr_sc_perfmon_cntr_i),
    .perfmon_enable_i             (perfmon_enable_i),
    .perfmon_event_i              (perfmon_event_i),
    .perfmon_mode_i               (perfmon_mode_i),
    .perfmon_qual_i               (perfmon_qual_i)
  );

  assign new_p1_req_cgater_o    = new_p1_req_info.cgater;
  assign new_p1_req_ev_mode_o   = new_p1_req_info.ev_mode;
  assign new_p1_req_rsrc_mode_o = new_p1_req_info.rsrc_mode;
  assign new_p1_update_cntr_o     = new_p1_update_cntr;
  assign new_p1_update_cntr_val_o = new_p1_update_cntr_val;

  // ══════════════════════════════════════════════════════════
  // Unused
  // ══════════════════════════════════════════════════════════
  logic unused_ok;
  assign unused_ok = &{1'b0, clk_i, rst_ni, unused_ok};

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on VARHIDDEN */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
