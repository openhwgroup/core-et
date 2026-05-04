// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module frontend_thread_sched (
  // System signals
  input  logic                                 clock,
  input  logic                                 reset,
  // Thread enable and valid signals
  input  logic [`CORE_NR_THREADS-1:0]          f7_valid_tid,
  input  logic [`CORE_NR_THREADS-1:0]          f0_thread_enabled,
  input  [`CORE_NR_THREADS-1:0]                id_core_stall,
  // Picked thread
  output logic [`CORE_NR_THREADS-1:0]          f7_thread_awake,
  output logic                                 f7_thread_id
);

////////////////////////////////////////////////////////////////////////////////
// Select thread to propagate
// When both threads are valid, access them alternatively
// Otherwise, read the one which is valid
////////////////////////////////////////////////////////////////////////////////
  logic                                         f7_thread_id_prev; // previously selected thread
  logic [`CORE_NR_THREADS-1:0]                  f7_valid_tid_qual;             // Inst valid in F4 for each thread
  logic [`CORE_NR_THREADS-1:0]                  f7_thread_awake_next;

  //         CLK    RST    EN                  DOUT               DIN                   DEF
  `RST_EN_FF(clock, reset, |f7_valid_tid_qual, f7_thread_id_prev, f7_thread_id,         1'b0)
  `RST_FF   (clock, reset,                     f7_thread_awake,   f7_thread_awake_next, 2'b11)

  always_comb begin
    // Computes the qualified valid per thread
    f7_thread_awake_next  = f0_thread_enabled & ~id_core_stall; // Thread can fetch
    f7_valid_tid_qual = f7_valid_tid & f7_thread_awake; // Valid from fetch, thread awake

    // Thread ID picker

    // Selects thread ID for current cycle based on valids and picker
    if      (!f7_valid_tid_qual[1]) f7_thread_id = 1'b0;                // If no inst for thread 1, select 0
    else if (!f7_valid_tid_qual[0]) f7_thread_id = 1'b1;                // If no inst for thread 0, select 1
    else                            f7_thread_id = ~f7_thread_id_prev;  // Alternate threads otherwise
  end // always_comb

endmodule

