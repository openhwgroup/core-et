// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench for shirecache_bank_perfmon_qual (IsP0=1).
// Decomposes perfmon structs to scalars for C++ driving.
// Also instantiates a second DUT with IsP0=0 for P1 resource mode testing.

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_perfmon_qual_tb
  import shirecache_pkg::*;
#(
  parameter int unsigned PerfmonCovfSize = 41,
  parameter int unsigned PerfmonModeSize = 8,
  parameter int unsigned PerfmonQualLsb  = 0,
  parameter int unsigned PerfmonQualSize = 51
) (
  input  logic                             clk_i,  // for testbench eval only
  input  logic                             rst_ni, // unused by DUT

  // ESR control inputs
  input  logic                             perfmon_enable_i,
  input  logic                             perfmon_event_i,
  input  logic [PerfmonModeSize-1:0]       perfmon_mode_i,
  input  logic [PerfmonQualSize-1:0]       perfmon_qual_i,

  // Counter input
  input  logic [PerfmonCovfSize-1:0]       esr_sc_perfmon_cntr_i,

  // Event response inputs (decomposed)
  input  logic                             ev_rbuf_l2_hit_i,
  input  logic                             ev_rbuf_scp_hit_i,
  input  logic                             ev_msgsenddata_i,
  input  logic                             ev_tag_bubble_i,
  input  logic                             ev_tag_hit_i,
  input  logic                             ev_tag_miss_i,
  input  logic                             ev_tag_evict_dirty_i,
  input  logic                             ev_tag_evict_write_around_i,
  input  logic                             ev_tag_evict_none_i,
  input  logic [3:0]                       ev_tag_victim_qwen_i,
  input  logic [5:0]                       ev_tag_opcode_i,

  // Resource response input
  input  logic [PerfmonIncrSize-1:0]       rsrc_cntr_i,

  // ── P0 DUT outputs ───────────────────────────────────
  output logic                             p0_update_cntr_o,
  output logic [PerfmonCovfSize-1:0]       p0_update_cntr_val_o,
  output logic                             p0_req_cgater_o,
  output logic [2:0]                       p0_req_ev_mode_o,
  output logic [4:0]                       p0_req_rsrc_mode_o,

  // ── P1 DUT outputs ───────────────────────────────────
  output logic                             p1_update_cntr_o,
  output logic [PerfmonCovfSize-1:0]       p1_update_cntr_val_o,
  output logic                             p1_req_cgater_o,
  output logic [2:0]                       p1_req_ev_mode_o,
  output logic [4:0]                       p1_req_rsrc_mode_o
);

  // Build perfmon_rsp_info_t from scalars
  perfmon_rsp_info_t rsp_info;
  always_comb begin
    rsp_info.ev_rsp.ev_rbuf_l2_hit         = ev_rbuf_l2_hit_i;
    rsp_info.ev_rsp.ev_rbuf_scp_hit        = ev_rbuf_scp_hit_i;
    rsp_info.ev_rsp.ev_msgsenddata         = ev_msgsenddata_i;
    rsp_info.ev_rsp.ev_tag_bubble          = ev_tag_bubble_i;
    rsp_info.ev_rsp.ev_tag_hit             = ev_tag_hit_i;
    rsp_info.ev_rsp.ev_tag_miss            = ev_tag_miss_i;
    rsp_info.ev_rsp.ev_tag_evict_dirty     = ev_tag_evict_dirty_i;
    rsp_info.ev_rsp.ev_tag_evict_write_around = ev_tag_evict_write_around_i;
    rsp_info.ev_rsp.ev_tag_evict_none      = ev_tag_evict_none_i;
    rsp_info.ev_rsp.ev_tag_victim_qwen     = ev_tag_victim_qwen_i;
    rsp_info.ev_rsp.ev_tag_opcode          = reqq_opcode_e'(ev_tag_opcode_i);
    rsp_info.rsrc_rsp.rsrc_cntr            = rsrc_cntr_i;
  end

  // ── P0 DUT (IsP0=1) ──────────────────────────────────
  perfmon_req_info_t p0_req_info;

  shirecache_bank_perfmon_qual #(
    .IsP0            (1),
    .PerfmonCovfSize (PerfmonCovfSize),
    .PerfmonModeSize (PerfmonModeSize),
    .PerfmonQualLsb  (PerfmonQualLsb),
    .PerfmonQualSize (PerfmonQualSize)
  ) u_p0 (
    .perfmon_req_info_o         (p0_req_info),
    .perfmon_rsp_info_i         (rsp_info),
    .perfmon_update_cntr_o      (p0_update_cntr_o),
    .esr_sc_perfmon_update_cntr_o(p0_update_cntr_val_o),
    .esr_sc_perfmon_cntr_i      (esr_sc_perfmon_cntr_i),
    .perfmon_enable_i,
    .perfmon_event_i,
    .perfmon_mode_i,
    .perfmon_qual_i
  );

  assign p0_req_cgater_o    = p0_req_info.cgater;
  assign p0_req_ev_mode_o   = p0_req_info.ev_mode;
  assign p0_req_rsrc_mode_o = p0_req_info.rsrc_mode;

  // ── P1 DUT (IsP0=0) ──────────────────────────────────
  perfmon_req_info_t p1_req_info;

  shirecache_bank_perfmon_qual #(
    .IsP0            (0),
    .PerfmonCovfSize (PerfmonCovfSize),
    .PerfmonModeSize (PerfmonModeSize),
    .PerfmonQualLsb  (PerfmonQualLsb),
    .PerfmonQualSize (PerfmonQualSize)
  ) u_p1 (
    .perfmon_req_info_o         (p1_req_info),
    .perfmon_rsp_info_i         (rsp_info),
    .perfmon_update_cntr_o      (p1_update_cntr_o),
    .esr_sc_perfmon_update_cntr_o(p1_update_cntr_val_o),
    .esr_sc_perfmon_cntr_i      (esr_sc_perfmon_cntr_i),
    .perfmon_enable_i,
    .perfmon_event_i,
    .perfmon_mode_i,
    .perfmon_qual_i
  );

  assign p1_req_cgater_o    = p1_req_info.cgater;
  assign p1_req_ev_mode_o   = p1_req_info.ev_mode;
  assign p1_req_rsrc_mode_o = p1_req_info.rsrc_mode;

  logic unused_ok;
  assign unused_ok = &{1'b0, clk_i, rst_ni, p0_req_info, p1_req_info, unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */
endmodule
