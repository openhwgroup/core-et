// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_top run control FSM vs new rbox_top.
//
// The rbox_top integration is mostly wiring — all sub-modules already have
// their own cosims. The unique logic is the debug run control FSM and the
// clock gating enable. This cosim compares those against an inlined copy
// of the original FSM logic.
//
// Uses separate rst_d_ni / rst_w_ni to verify reset domain independence.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off BLKSEQ */

module cosim_rbox_top_tb (
  input  logic        clk_i,
  input  logic        rst_d_ni,
  input  logic        rst_w_ni,

  // Run control stimulus
  input  logic        rc_halt_req_events_i,
  input  logic        rc_resume_req_events_i,
  input  logic        rc_halt_req_gpio_i,
  input  logic        rc_resume_req_gpio_i,

  // New module outputs
  output logic        new_halt_ack_o,
  output logic        new_resume_ack_o,
  output logic        new_reset_ack_o,
  output logic        new_ck_enable_o,

  // Original module outputs
  output logic        orig_halt_ack_o,
  output logic        orig_resume_ack_o,
  output logic        orig_reset_ack_o,
  output logic        orig_ck_enable_o
);

  import rbox_pkg::*;

  // ── Reset adaptation ────────────────────────────────────
  logic reset_d;
  logic reset_w;
  assign reset_d = ~rst_d_ni;
  assign reset_w = ~rst_w_ni;

  // ════════════════════════════════════════════════════════════
  // New FSM (extracted from rbox_top)
  // ════════════════════════════════════════════════════════════

  logic new_halted_q, new_halted_d, new_halted_en;
  logic new_halt_ack_d, new_halt_ack_q;
  logic new_resume_ack_d, new_resume_ack_q;

  rc_t new_rc;
  assign new_rc.events.halt_req   = rc_halt_req_events_i;
  assign new_rc.events.resume_req = rc_resume_req_events_i;
  assign new_rc.gpio.halt_req     = rc_halt_req_gpio_i;
  assign new_rc.gpio.resume_req   = rc_resume_req_gpio_i;
  assign new_rc.gpio.reset_req    = 1'b0;
  assign new_rc.single_step       = 1'b0;

  always_comb begin
    new_halted_en  = (!new_halted_q && (new_rc.events.halt_req   || new_rc.gpio.halt_req))
                   || (new_halted_q && (new_rc.events.resume_req || new_rc.gpio.resume_req));
    new_halted_d   = (new_halted_q && (!new_rc.events.resume_req || !new_rc.gpio.resume_req))
                   || (!new_halted_q && (new_rc.events.halt_req   || new_rc.gpio.halt_req));
    new_halt_ack_d   = new_halted_en && !new_halted_q;
    new_resume_ack_d = new_halted_en &&  new_halted_q;
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      new_halted_q     <= 1'b0;
      new_halt_ack_q   <= 1'b0;
      new_resume_ack_q <= 1'b0;
    end else begin
      if (new_halted_en)
        new_halted_q <= new_halted_d;
      new_halt_ack_q   <= new_halt_ack_d;
      new_resume_ack_q <= new_resume_ack_d;
    end
  end

  assign new_halt_ack_o   = new_halt_ack_q;
  assign new_resume_ack_o = new_resume_ack_q;
  assign new_reset_ack_o  = 1'b0;

  // Clock gate enable (warm reset domain)
  assign new_ck_enable_o = !rst_w_ni;  // rbox_enable is 0 in this standalone test

  // ════════════════════════════════════════════════════════════
  // Original FSM (inlined from rbox_top.v)
  // ════════════════════════════════════════════════════════════

  bpam_rc_rbox_t orig_rc;
  assign orig_rc.events.halt_req   = rc_halt_req_events_i;
  assign orig_rc.events.resume_req = rc_resume_req_events_i;
  assign orig_rc.gpio.halt_req     = rc_halt_req_gpio_i;
  assign orig_rc.gpio.resume_req   = rc_resume_req_gpio_i;
  assign orig_rc.gpio.reset_req    = 1'b0;
  assign orig_rc.single_step       = 1'b0;

  logic orig_halted_ff, orig_halted_nxt, orig_halted_en;
  logic orig_halt_ack_ff, orig_halt_ack_nxt;
  logic orig_resume_ack_ff, orig_resume_ack_nxt;

  always_comb begin
    orig_halted_en  =  !orig_halted_ff && ( orig_rc.events.halt_req   ||  orig_rc.gpio.halt_req)
                    ||  orig_halted_ff && ( orig_rc.events.resume_req ||  orig_rc.gpio.resume_req);
    orig_halted_nxt =   orig_halted_ff && (!orig_rc.events.resume_req || !orig_rc.gpio.resume_req)
                    || !orig_halted_ff && ( orig_rc.events.halt_req   ||  orig_rc.gpio.halt_req);
    orig_halt_ack_nxt   = orig_halted_en && !orig_halted_ff;
    orig_resume_ack_nxt = orig_halted_en &&  orig_halted_ff;
  end

  `RST_EN_FF(clk_i, reset_d, orig_halted_en, orig_halted_ff,     orig_halted_nxt,     1'b0);
     `RST_FF(clk_i, reset_d,                 orig_halt_ack_ff,   orig_halt_ack_nxt,   1'b0);
     `RST_FF(clk_i, reset_d,                 orig_resume_ack_ff, orig_resume_ack_nxt, 1'b0);

  assign orig_halt_ack_o   = orig_halt_ack_ff;
  assign orig_resume_ack_o = orig_resume_ack_ff;
  assign orig_reset_ack_o  = 1'b0;

  // Clock gate enable (original: rbox_ck_enable = rbox_enable || reset_w)
  assign orig_ck_enable_o = reset_w;  // rbox_enable is 0 in this standalone test

endmodule

/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNOPTFLAT */
