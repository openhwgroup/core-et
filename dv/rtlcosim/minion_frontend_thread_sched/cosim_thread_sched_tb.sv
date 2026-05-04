// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: minion_frontend_thread_sched vs frontend_thread_sched.

module cosim_thread_sched_tb (
  input  logic       clk_i,
  input  logic       rst_ni,

  input  logic [1:0] f7_valid_tid_i,
  input  logic [1:0] f0_thread_enabled_i,
  input  logic [1:0] id_core_stall_i,

  output logic [1:0] new_f7_thread_awake_o,
  output logic       new_f7_thread_id_o,

  output logic [1:0] orig_f7_thread_awake_o,
  output logic       orig_f7_thread_id_o
);

  minion_frontend_thread_sched u_new (
    .clk_i,
    .rst_ni,
    .f7_valid_tid_i,
    .f0_thread_enabled_i,
    .id_core_stall_i,
    .f7_thread_awake_o (new_f7_thread_awake_o),
    .f7_thread_id_o    (new_f7_thread_id_o)
  );

  // Original uses active-high synchronous reset.
  logic reset_orig;
  assign reset_orig = ~rst_ni;

  frontend_thread_sched u_orig (
    .clock              (clk_i),
    .reset              (reset_orig),
    .f7_valid_tid       (f7_valid_tid_i),
    .f0_thread_enabled  (f0_thread_enabled_i),
    .id_core_stall      (id_core_stall_i),
    .f7_thread_awake    (orig_f7_thread_awake_o),
    .f7_thread_id       (orig_f7_thread_id_o)
  );

endmodule
