// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_perfmon
// vs new shirecache_bank_perfmon.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
module cosim_perfmon_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  // ── ESR control scalars ───────────────────────────────────
  input  logic        wr_en_perfmon_ctl_status_i,
  input  logic        wr_en_perfmon_cyc_cntr_i,
  input  logic        wr_en_perfmon_p0_cntr_i,
  input  logic        wr_en_perfmon_p1_cntr_i,
  input  logic        wr_en_perfmon_p0_qual_i,
  input  logic        wr_en_perfmon_p1_qual_i,
  input  logic [63:0] wr_data_i,

  // ── P0 rsp scalars ───────────────────────────────────────
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

  // ── P1 rsp scalars ───────────────────────────────────────
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

  // ── Original outputs ──────────────────────────────────────
  output logic        orig_p0_req_cgater_o,
  output logic [2:0]  orig_p0_req_ev_mode_o,
  output logic [4:0]  orig_p0_req_rsrc_mode_o,
  output logic        orig_p1_req_cgater_o,
  output logic [2:0]  orig_p1_req_ev_mode_o,
  output logic [4:0]  orig_p1_req_rsrc_mode_o,
  output logic        orig_cyc_overflow_o,
  output logic        orig_p0_overflow_o,
  output logic        orig_p1_overflow_o,
  output logic [63:0] orig_esr_ctl_status_o,
  output logic [63:0] orig_esr_cyc_cntr_o,
  output logic [63:0] orig_esr_p0_cntr_o,
  output logic [63:0] orig_esr_p1_cntr_o,
  output logic [63:0] orig_esr_p0_qual_o,
  output logic [63:0] orig_esr_p1_qual_o,

  // ── New outputs ───────────────────────────────────────────
  output logic        new_p0_req_cgater_o,
  output logic [2:0]  new_p0_req_ev_mode_o,
  output logic [4:0]  new_p0_req_rsrc_mode_o,
  output logic        new_p1_req_cgater_o,
  output logic [2:0]  new_p1_req_ev_mode_o,
  output logic [4:0]  new_p1_req_rsrc_mode_o,
  output logic        new_cyc_overflow_o,
  output logic        new_p0_overflow_o,
  output logic        new_p1_overflow_o,
  output logic [63:0] new_esr_ctl_status_o,
  output logic [63:0] new_esr_cyc_cntr_o,
  output logic [63:0] new_esr_p0_cntr_o,
  output logic [63:0] new_esr_p1_cntr_o,
  output logic [63:0] new_esr_p0_qual_o,
  output logic [63:0] new_esr_p1_qual_o
);

  // ══════════════════════════════════════════════════════════
  // Build original rsp structs from scalars
  // ══════════════════════════════════════════════════════════

  sc_perfmon_rsp_info_t orig_p0_rsp, orig_p1_rsp;
  always_comb begin
    orig_p0_rsp = '0;
    orig_p0_rsp.ev_rsp.ev_rbuf_l2_hit         = p0_ev_rbuf_l2_hit_i;
    orig_p0_rsp.ev_rsp.ev_rbuf_scp_hit        = p0_ev_rbuf_scp_hit_i;
    orig_p0_rsp.ev_rsp.ev_msgsenddata         = p0_ev_msgsenddata_i;
    orig_p0_rsp.ev_rsp.ev_tag_bubble          = p0_ev_tag_bubble_i;
    orig_p0_rsp.ev_rsp.ev_tag_hit             = p0_ev_tag_hit_i;
    orig_p0_rsp.ev_rsp.ev_tag_miss            = p0_ev_tag_miss_i;
    orig_p0_rsp.ev_rsp.ev_tag_evict_dirty     = p0_ev_tag_evict_dirty_i;
    orig_p0_rsp.ev_rsp.ev_tag_evict_write_around = p0_ev_tag_evict_write_around_i;
    orig_p0_rsp.ev_rsp.ev_tag_evict_none      = p0_ev_tag_evict_none_i;
    orig_p0_rsp.ev_rsp.ev_tag_victim_qwen     = p0_ev_tag_victim_qwen_i;
    orig_p0_rsp.ev_rsp.ev_tag_opcode          = sc_reqq_opcode_t'(p0_ev_tag_opcode_i);
    orig_p0_rsp.rsrc_rsp.rsrc_cntr            = p0_rsrc_cntr_i;

    orig_p1_rsp = '0;
    orig_p1_rsp.ev_rsp.ev_rbuf_l2_hit         = p1_ev_rbuf_l2_hit_i;
    orig_p1_rsp.ev_rsp.ev_rbuf_scp_hit        = p1_ev_rbuf_scp_hit_i;
    orig_p1_rsp.ev_rsp.ev_msgsenddata         = p1_ev_msgsenddata_i;
    orig_p1_rsp.ev_rsp.ev_tag_bubble          = p1_ev_tag_bubble_i;
    orig_p1_rsp.ev_rsp.ev_tag_hit             = p1_ev_tag_hit_i;
    orig_p1_rsp.ev_rsp.ev_tag_miss            = p1_ev_tag_miss_i;
    orig_p1_rsp.ev_rsp.ev_tag_evict_dirty     = p1_ev_tag_evict_dirty_i;
    orig_p1_rsp.ev_rsp.ev_tag_evict_write_around = p1_ev_tag_evict_write_around_i;
    orig_p1_rsp.ev_rsp.ev_tag_evict_none      = p1_ev_tag_evict_none_i;
    orig_p1_rsp.ev_rsp.ev_tag_victim_qwen     = p1_ev_tag_victim_qwen_i;
    orig_p1_rsp.ev_rsp.ev_tag_opcode          = sc_reqq_opcode_t'(p1_ev_tag_opcode_i);
    orig_p1_rsp.rsrc_rsp.rsrc_cntr            = p1_rsrc_cntr_i;
  end

  // Build new rsp structs
  perfmon_rsp_info_t new_p0_rsp, new_p1_rsp;
  always_comb begin
    new_p0_rsp.ev_rsp.ev_rbuf_l2_hit         = p0_ev_rbuf_l2_hit_i;
    new_p0_rsp.ev_rsp.ev_rbuf_scp_hit        = p0_ev_rbuf_scp_hit_i;
    new_p0_rsp.ev_rsp.ev_msgsenddata         = p0_ev_msgsenddata_i;
    new_p0_rsp.ev_rsp.ev_tag_bubble          = p0_ev_tag_bubble_i;
    new_p0_rsp.ev_rsp.ev_tag_hit             = p0_ev_tag_hit_i;
    new_p0_rsp.ev_rsp.ev_tag_miss            = p0_ev_tag_miss_i;
    new_p0_rsp.ev_rsp.ev_tag_evict_dirty     = p0_ev_tag_evict_dirty_i;
    new_p0_rsp.ev_rsp.ev_tag_evict_write_around = p0_ev_tag_evict_write_around_i;
    new_p0_rsp.ev_rsp.ev_tag_evict_none      = p0_ev_tag_evict_none_i;
    new_p0_rsp.ev_rsp.ev_tag_victim_qwen     = p0_ev_tag_victim_qwen_i;
    new_p0_rsp.ev_rsp.ev_tag_opcode          = reqq_opcode_e'(p0_ev_tag_opcode_i);
    new_p0_rsp.rsrc_rsp.rsrc_cntr            = p0_rsrc_cntr_i;

    new_p1_rsp.ev_rsp.ev_rbuf_l2_hit         = p1_ev_rbuf_l2_hit_i;
    new_p1_rsp.ev_rsp.ev_rbuf_scp_hit        = p1_ev_rbuf_scp_hit_i;
    new_p1_rsp.ev_rsp.ev_msgsenddata         = p1_ev_msgsenddata_i;
    new_p1_rsp.ev_rsp.ev_tag_bubble          = p1_ev_tag_bubble_i;
    new_p1_rsp.ev_rsp.ev_tag_hit             = p1_ev_tag_hit_i;
    new_p1_rsp.ev_rsp.ev_tag_miss            = p1_ev_tag_miss_i;
    new_p1_rsp.ev_rsp.ev_tag_evict_dirty     = p1_ev_tag_evict_dirty_i;
    new_p1_rsp.ev_rsp.ev_tag_evict_write_around = p1_ev_tag_evict_write_around_i;
    new_p1_rsp.ev_rsp.ev_tag_evict_none      = p1_ev_tag_evict_none_i;
    new_p1_rsp.ev_rsp.ev_tag_victim_qwen     = p1_ev_tag_victim_qwen_i;
    new_p1_rsp.ev_rsp.ev_tag_opcode          = reqq_opcode_e'(p1_ev_tag_opcode_i);
    new_p1_rsp.rsrc_rsp.rsrc_cntr            = p1_rsrc_cntr_i;
  end

  // ══════════════════════════════════════════════════════════
  // Build original esr_ctl struct
  // ══════════════════════════════════════════════════════════

  sc_bank_esr_ctl_t orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.wr_en_perfmon_ctl_status = wr_en_perfmon_ctl_status_i;
    orig_esr_ctl.wr_en_perfmon_cyc_cntr   = wr_en_perfmon_cyc_cntr_i;
    orig_esr_ctl.wr_en_perfmon_p0_cntr    = wr_en_perfmon_p0_cntr_i;
    orig_esr_ctl.wr_en_perfmon_p1_cntr    = wr_en_perfmon_p1_cntr_i;
    orig_esr_ctl.wr_en_perfmon_p0_qual    = wr_en_perfmon_p0_qual_i;
    orig_esr_ctl.wr_en_perfmon_p1_qual    = wr_en_perfmon_p1_qual_i;
    orig_esr_ctl.wr_data                  = wr_data_i;
  end

  // Build new esr_ctl struct
  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.wr_en_perfmon_ctl_status = wr_en_perfmon_ctl_status_i;
    new_esr_ctl.wr_en_perfmon_cyc_cntr   = wr_en_perfmon_cyc_cntr_i;
    new_esr_ctl.wr_en_perfmon_p0_cntr    = wr_en_perfmon_p0_cntr_i;
    new_esr_ctl.wr_en_perfmon_p1_cntr    = wr_en_perfmon_p1_cntr_i;
    new_esr_ctl.wr_en_perfmon_p0_qual    = wr_en_perfmon_p0_qual_i;
    new_esr_ctl.wr_en_perfmon_p1_qual    = wr_en_perfmon_p1_qual_i;
    new_esr_ctl.wr_data                  = wr_data_i;
  end

  // ══════════════════════════════════════════════════════════
  // Original DUT
  // ══════════════════════════════════════════════════════════

  sc_perfmon_req_info_t  orig_p0_req_info, orig_p1_req_info;
  sc_perfmon_cntr_overflow_t orig_overflow;
  sc_perfmon_esr_status_t    orig_esr_status;

  shire_cache_bank_perfmon u_orig (
    .clock                  (clk_i),
    .reset                  (~rst_ni),

    .perfmon_p0_req_info    (orig_p0_req_info),
    .perfmon_p0_rsp_info    (orig_p0_rsp),
    .perfmon_p1_req_info    (orig_p1_req_info),
    .perfmon_p1_rsp_info    (orig_p1_rsp),

    .perfmon_cntr_overflow  (orig_overflow),

    .esr_ctl                (orig_esr_ctl),
    .perfmon_esr_status     (orig_esr_status)
  );

  assign orig_p0_req_cgater_o    = orig_p0_req_info.cgater;
  assign orig_p0_req_ev_mode_o   = orig_p0_req_info.ev_mode;
  assign orig_p0_req_rsrc_mode_o = orig_p0_req_info.rsrc_mode;
  assign orig_p1_req_cgater_o    = orig_p1_req_info.cgater;
  assign orig_p1_req_ev_mode_o   = orig_p1_req_info.ev_mode;
  assign orig_p1_req_rsrc_mode_o = orig_p1_req_info.rsrc_mode;

  assign orig_cyc_overflow_o = orig_overflow.cyc_cntr_overflow;
  assign orig_p0_overflow_o  = orig_overflow.p0_cntr_overflow;
  assign orig_p1_overflow_o  = orig_overflow.p1_cntr_overflow;

  assign orig_esr_ctl_status_o = orig_esr_status.ctl_status;
  assign orig_esr_cyc_cntr_o   = orig_esr_status.cyc_cntr;
  assign orig_esr_p0_cntr_o    = orig_esr_status.p0_cntr;
  assign orig_esr_p1_cntr_o    = orig_esr_status.p1_cntr;
  assign orig_esr_p0_qual_o    = orig_esr_status.p0_qual;
  assign orig_esr_p1_qual_o    = orig_esr_status.p1_qual;

  // ══════════════════════════════════════════════════════════
  // New DUT
  // ══════════════════════════════════════════════════════════

  perfmon_req_info_t      new_p0_req_info, new_p1_req_info;
  perfmon_cntr_overflow_t new_overflow;
  perfmon_esr_status_t    new_esr_status;

  shirecache_bank_perfmon u_new (
    .clk_i                   (clk_i),
    .clk_free_i              (clk_i),
    .rst_ni                  (rst_ni),
    .dft_i                   ('0),

    .perfmon_p0_req_info_o   (new_p0_req_info),
    .perfmon_p0_rsp_info_i   (new_p0_rsp),
    .perfmon_p1_req_info_o   (new_p1_req_info),
    .perfmon_p1_rsp_info_i   (new_p1_rsp),

    .perfmon_cntr_overflow_o (new_overflow),

    .esr_ctl_i               (new_esr_ctl),
    .perfmon_esr_status_o    (new_esr_status)
  );

  assign new_p0_req_cgater_o    = new_p0_req_info.cgater;
  assign new_p0_req_ev_mode_o   = new_p0_req_info.ev_mode;
  assign new_p0_req_rsrc_mode_o = new_p0_req_info.rsrc_mode;
  assign new_p1_req_cgater_o    = new_p1_req_info.cgater;
  assign new_p1_req_ev_mode_o   = new_p1_req_info.ev_mode;
  assign new_p1_req_rsrc_mode_o = new_p1_req_info.rsrc_mode;

  assign new_cyc_overflow_o = new_overflow.cyc_cntr_overflow;
  assign new_p0_overflow_o  = new_overflow.p0_cntr_overflow;
  assign new_p1_overflow_o  = new_overflow.p1_cntr_overflow;

  assign new_esr_ctl_status_o = new_esr_status.ctl_status;
  assign new_esr_cyc_cntr_o   = new_esr_status.cyc_cntr;
  assign new_esr_p0_cntr_o    = new_esr_status.p0_cntr;
  assign new_esr_p1_cntr_o    = new_esr_status.p1_cntr;
  assign new_esr_p0_qual_o    = new_esr_status.p0_qual;
  assign new_esr_p1_qual_o    = new_esr_status.p1_qual;

  // ══════════════════════════════════════════════════════════
  // Unused
  // ══════════════════════════════════════════════════════════
  logic unused_ok;
  assign unused_ok = &{1'b0,
    orig_p0_req_info, orig_p1_req_info, orig_overflow, orig_esr_status,
    new_p0_req_info, new_p1_req_info, new_overflow, new_esr_status,
    unused_ok};

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on VARHIDDEN */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
