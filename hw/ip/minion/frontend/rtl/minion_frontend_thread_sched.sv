// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Frontend thread scheduler.
//
// Round-robin arbitration between two hardware threads for the ID stage.
// When both threads have valid instructions, alternates between them.
// When only one has a valid instruction, selects that one.
//
// Replaces: frontend_thread_sched (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions
//   - Explicit always_ff with _d/_q instead of RST_FF/RST_EN_FF macros

module minion_frontend_thread_sched #(
  /* verilator lint_off VARHIDDEN */  // Preserve public NrThreads; minion_pkg exports the default too.
  parameter int unsigned NrThreads = 2
  /* verilator lint_on VARHIDDEN */
) (
  input  logic                  clk_i,
  input  logic                  rst_ni,

  input  logic [NrThreads-1:0]  f7_valid_tid_i,
  input  logic [NrThreads-1:0]  f0_thread_enabled_i,
  input  logic [NrThreads-1:0]  id_core_stall_i,

  output logic [NrThreads-1:0]  f7_thread_awake_o,
  output logic                  f7_thread_id_o
);

  // ── Internal signals ──────────────────────────────────

  logic                  thread_id_prev_d, thread_id_prev_q;
  logic [NrThreads-1:0] thread_awake_d,   thread_awake_q;
  logic [NrThreads-1:0] valid_tid_qual;

  // ── Combinational ─────────────────────────────────────

  always_comb begin
    // Thread can issue if enabled and not stalled by core.
    thread_awake_d = f0_thread_enabled_i & ~id_core_stall_i;

    // Qualified valid: instruction available AND thread awake.
    valid_tid_qual = f7_valid_tid_i & thread_awake_q;

    // Thread ID picker: round-robin when both valid, otherwise
    // select whichever has a valid instruction.
    if      (!valid_tid_qual[1]) f7_thread_id_o = 1'b0;
    else if (!valid_tid_qual[0]) f7_thread_id_o = 1'b1;
    else                         f7_thread_id_o = ~thread_id_prev_q;

    // Update previous selection only when a valid instruction issued.
    thread_id_prev_d = f7_thread_id_o;
  end

  assign f7_thread_awake_o = thread_awake_q;

  // ── Sequential ────────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      thread_id_prev_q <= 1'b0;
      thread_awake_q   <= {NrThreads{1'b1}};
    end else begin
      // Only update previous thread ID when a valid thread was picked.
      if (|valid_tid_qual) begin
        thread_id_prev_q <= thread_id_prev_d;
      end
      thread_awake_q <= thread_awake_d;
    end
  end

endmodule
