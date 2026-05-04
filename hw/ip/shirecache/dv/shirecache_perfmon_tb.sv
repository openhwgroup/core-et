// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench for shirecache_bank_perfmon.
// Decomposes structs to scalars for C++ driving.

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_perfmon_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  // ── ESR control inputs (decomposed from bank_esr_ctl_t) ──
  input  logic        wr_en_perfmon_ctl_status_i,
  input  logic        wr_en_perfmon_cyc_cntr_i,
  input  logic        wr_en_perfmon_p0_cntr_i,
  input  logic        wr_en_perfmon_p1_cntr_i,
  input  logic        wr_en_perfmon_p0_qual_i,
  input  logic        wr_en_perfmon_p1_qual_i,
  input  logic [63:0] wr_data_i,

  // ── P0 rsp event inputs (decomposed) ─────────────────────
  input  logic        p0_ev_rbuf_l2_hit_i,
  input  logic        p0_ev_rbuf_scp_hit_i,
  input  logic        p0_ev_msgsenddata_i,
  input  logic        p0_ev_tag_bubble_i,
  input  logic        p0_ev_tag_hit_i,
  input  logic        p0_ev_tag_miss_i,
  input  logic        p0_ev_tag_evict_dirty_i,
  input  logic        p0_ev_tag_evict_write_around_i,
  input  logic        p0_ev_tag_evict_none_i,
  input  logic [3:0]  p0_ev_tag_victim_qwen_i,
  input  logic [5:0]  p0_ev_tag_opcode_i,
  input  logic [6:0]  p0_rsrc_cntr_i,

  // ── P1 rsp event inputs (decomposed) ─────────────────────
  input  logic        p1_ev_rbuf_l2_hit_i,
  input  logic        p1_ev_rbuf_scp_hit_i,
  input  logic        p1_ev_msgsenddata_i,
  input  logic        p1_ev_tag_bubble_i,
  input  logic        p1_ev_tag_hit_i,
  input  logic        p1_ev_tag_miss_i,
  input  logic        p1_ev_tag_evict_dirty_i,
  input  logic        p1_ev_tag_evict_write_around_i,
  input  logic        p1_ev_tag_evict_none_i,
  input  logic [3:0]  p1_ev_tag_victim_qwen_i,
  input  logic [5:0]  p1_ev_tag_opcode_i,
  input  logic [6:0]  p1_rsrc_cntr_i,

  // ── Outputs ──────────────────────────────────────────────
  // P0 req info
  output logic        p0_req_cgater_o,
  output logic [2:0]  p0_req_ev_mode_o,
  output logic [4:0]  p0_req_rsrc_mode_o,

  // P1 req info
  output logic        p1_req_cgater_o,
  output logic [2:0]  p1_req_ev_mode_o,
  output logic [4:0]  p1_req_rsrc_mode_o,

  // Overflow
  output logic        cyc_cntr_overflow_o,
  output logic        p0_cntr_overflow_o,
  output logic        p1_cntr_overflow_o,

  // ESR status readback
  output logic [63:0] esr_ctl_status_o,
  output logic [63:0] esr_cyc_cntr_o,
  output logic [63:0] esr_p0_cntr_o,
  output logic [63:0] esr_p1_cntr_o,
  output logic [63:0] esr_p0_qual_o,
  output logic [63:0] esr_p1_qual_o
);

  // Build bank_esr_ctl_t from scalars
  bank_esr_ctl_t esr_ctl;
  always_comb begin
    esr_ctl = '0;
    esr_ctl.wr_en_perfmon_ctl_status = wr_en_perfmon_ctl_status_i;
    esr_ctl.wr_en_perfmon_cyc_cntr   = wr_en_perfmon_cyc_cntr_i;
    esr_ctl.wr_en_perfmon_p0_cntr    = wr_en_perfmon_p0_cntr_i;
    esr_ctl.wr_en_perfmon_p1_cntr    = wr_en_perfmon_p1_cntr_i;
    esr_ctl.wr_en_perfmon_p0_qual    = wr_en_perfmon_p0_qual_i;
    esr_ctl.wr_en_perfmon_p1_qual    = wr_en_perfmon_p1_qual_i;
    esr_ctl.wr_data                  = wr_data_i;
  end

  // Build perfmon_rsp_info_t from scalars
  perfmon_rsp_info_t p0_rsp_info, p1_rsp_info;
  always_comb begin
    p0_rsp_info.ev_rsp.ev_rbuf_l2_hit         = p0_ev_rbuf_l2_hit_i;
    p0_rsp_info.ev_rsp.ev_rbuf_scp_hit        = p0_ev_rbuf_scp_hit_i;
    p0_rsp_info.ev_rsp.ev_msgsenddata         = p0_ev_msgsenddata_i;
    p0_rsp_info.ev_rsp.ev_tag_bubble          = p0_ev_tag_bubble_i;
    p0_rsp_info.ev_rsp.ev_tag_hit             = p0_ev_tag_hit_i;
    p0_rsp_info.ev_rsp.ev_tag_miss            = p0_ev_tag_miss_i;
    p0_rsp_info.ev_rsp.ev_tag_evict_dirty     = p0_ev_tag_evict_dirty_i;
    p0_rsp_info.ev_rsp.ev_tag_evict_write_around = p0_ev_tag_evict_write_around_i;
    p0_rsp_info.ev_rsp.ev_tag_evict_none      = p0_ev_tag_evict_none_i;
    p0_rsp_info.ev_rsp.ev_tag_victim_qwen     = p0_ev_tag_victim_qwen_i;
    p0_rsp_info.ev_rsp.ev_tag_opcode          = reqq_opcode_e'(p0_ev_tag_opcode_i);
    p0_rsp_info.rsrc_rsp.rsrc_cntr            = p0_rsrc_cntr_i;

    p1_rsp_info.ev_rsp.ev_rbuf_l2_hit         = p1_ev_rbuf_l2_hit_i;
    p1_rsp_info.ev_rsp.ev_rbuf_scp_hit        = p1_ev_rbuf_scp_hit_i;
    p1_rsp_info.ev_rsp.ev_msgsenddata         = p1_ev_msgsenddata_i;
    p1_rsp_info.ev_rsp.ev_tag_bubble          = p1_ev_tag_bubble_i;
    p1_rsp_info.ev_rsp.ev_tag_hit             = p1_ev_tag_hit_i;
    p1_rsp_info.ev_rsp.ev_tag_miss            = p1_ev_tag_miss_i;
    p1_rsp_info.ev_rsp.ev_tag_evict_dirty     = p1_ev_tag_evict_dirty_i;
    p1_rsp_info.ev_rsp.ev_tag_evict_write_around = p1_ev_tag_evict_write_around_i;
    p1_rsp_info.ev_rsp.ev_tag_evict_none      = p1_ev_tag_evict_none_i;
    p1_rsp_info.ev_rsp.ev_tag_victim_qwen     = p1_ev_tag_victim_qwen_i;
    p1_rsp_info.ev_rsp.ev_tag_opcode          = reqq_opcode_e'(p1_ev_tag_opcode_i);
    p1_rsp_info.rsrc_rsp.rsrc_cntr            = p1_rsrc_cntr_i;
  end

  // DUT outputs
  perfmon_req_info_t      p0_req_info, p1_req_info;
  perfmon_cntr_overflow_t overflow;
  perfmon_esr_status_t    esr_status;

  shirecache_bank_perfmon u_dut (
    .clk_i,
    .clk_free_i         (clk_i),   // use same clock for test
    .rst_ni,
    .dft_i              ('0),

    .perfmon_p0_req_info_o  (p0_req_info),
    .perfmon_p0_rsp_info_i  (p0_rsp_info),
    .perfmon_p1_req_info_o  (p1_req_info),
    .perfmon_p1_rsp_info_i  (p1_rsp_info),

    .perfmon_cntr_overflow_o (overflow),

    .esr_ctl_i               (esr_ctl),
    .perfmon_esr_status_o    (esr_status)
  );

  // Decompose outputs for C++
  assign p0_req_cgater_o    = p0_req_info.cgater;
  assign p0_req_ev_mode_o   = p0_req_info.ev_mode;
  assign p0_req_rsrc_mode_o = p0_req_info.rsrc_mode;

  assign p1_req_cgater_o    = p1_req_info.cgater;
  assign p1_req_ev_mode_o   = p1_req_info.ev_mode;
  assign p1_req_rsrc_mode_o = p1_req_info.rsrc_mode;

  assign cyc_cntr_overflow_o = overflow.cyc_cntr_overflow;
  assign p0_cntr_overflow_o  = overflow.p0_cntr_overflow;
  assign p1_cntr_overflow_o  = overflow.p1_cntr_overflow;

  assign esr_ctl_status_o = esr_status.ctl_status;
  assign esr_cyc_cntr_o   = esr_status.cyc_cntr;
  assign esr_p0_cntr_o    = esr_status.p0_cntr;
  assign esr_p1_cntr_o    = esr_status.p1_cntr;
  assign esr_p0_qual_o    = esr_status.p0_qual;
  assign esr_p1_qual_o    = esr_status.p1_qual;

  logic unused_ok;
  assign unused_ok = &{1'b0, p0_req_info, p1_req_info, overflow, esr_status, unused_ok};

/* verilator lint_on UNUSEDSIGNAL */
endmodule
