// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: debug_breakpoint (new) vs (original).
//
// Tests BOTH parameter values in the same TB:
//   TriggerType=0 (insn) and TriggerType=1 (data).
//
// Pure combinational. Drives random addresses, configs, privilege modes.

`include "soc.vh"

module cosim_debug_breakpoint_tb (
  input  logic         clk_i,
  input  logic         rst_ni,

  // Stimulus — breakpoint configuration (per-thread)
  input  logic [17:0]  stim_control,         // 2 x minion_bp_ctrl_t (9 bits each)
  input  logic [1:0]   stim_in_debug,        // per-thread debug mode
  input  logic [3:0]   stim_prv,             // 2 x 2-bit privilege
  input  logic [95:0]  stim_tdata2,          // 2 x 48-bit (VA_SIZE)

  // Stimulus — core signals
  input  logic [48:0]  stim_address,         // VA_SIZE_EXT = 49 bits
  input  logic [2:0]   stim_cmd,             // debug_bp_e = 3 bits
  input  logic         stim_valid,
  input  logic         stim_thread_id,

  // Outputs — TriggerType=0 (insn) — new module
  output logic         new_insn_timing_o,
  output logic         new_insn_xcpt_o,
  output logic         new_insn_enter_debug_o,
  output logic         new_insn_update_o,

  // Outputs — TriggerType=0 (insn) — original module
  output logic         orig_insn_timing_o,
  output logic         orig_insn_xcpt_o,
  output logic         orig_insn_enter_debug_o,
  output logic         orig_insn_update_o,

  // Outputs — TriggerType=1 (data) — new module
  output logic         new_data_timing_o,
  output logic         new_data_xcpt_o,
  output logic         new_data_enter_debug_o,
  output logic         new_data_update_o,

  // Outputs — TriggerType=1 (data) — original module
  output logic         orig_data_timing_o,
  output logic         orig_data_xcpt_o,
  output logic         orig_data_enter_debug_o,
  output logic         orig_data_update_o
);

  import minion_pkg::*;

  // Reconstruct per-thread packed struct arrays
  minion_bp_ctrl_t [NrThreadsDefault-1:0] control;
  assign control = stim_control;

  logic [NrThreadsDefault-1:0][1:0] prv;
  assign prv = stim_prv;

  logic [NrThreadsDefault-1:0][VaSize-1:0] tdata2;
  assign tdata2 = stim_tdata2;

  debug_bp_e cmd;
  assign cmd = debug_bp_e'(stim_cmd);

  // ── TriggerType=0 (insn) — New module ──
  debug_breakpoint #(.TriggerType(0)) u_new_insn (
    .control_ip     (control),
    .in_debug_ip    (stim_in_debug),
    .prv_ip         (prv),
    .tdata2_ip      (tdata2),
    .address_ip     (stim_address),
    .cmd_ip         (cmd),
    .valid_ip       (stim_valid),
    .thread_id_ip   (stim_thread_id),
    .timing_op      (new_insn_timing_o),
    .xcpt_op        (new_insn_xcpt_o),
    .enter_debug_op (new_insn_enter_debug_o),
    .update_op      (new_insn_update_o)
  );

  // ── TriggerType=0 (insn) — Original module ──
  debug_breakpoint_orig #(.TRIGGER_TYPE(0)) u_orig_insn (
    .control_ip     (control),
    .in_debug_ip    (stim_in_debug),
    .prv_ip         (prv),
    .tdata2_ip      (tdata2),
    .address_ip     (stim_address),
    .cmd_ip         (cmd),
    .valid_ip       (stim_valid),
    .thread_id_ip   (stim_thread_id),
    .timing_op      (orig_insn_timing_o),
    .xcpt_op        (orig_insn_xcpt_o),
    .enter_debug_op (orig_insn_enter_debug_o),
    .update_op      (orig_insn_update_o)
  );

  // ── TriggerType=1 (data) — New module ──
  debug_breakpoint #(.TriggerType(1)) u_new_data (
    .control_ip     (control),
    .in_debug_ip    (stim_in_debug),
    .prv_ip         (prv),
    .tdata2_ip      (tdata2),
    .address_ip     (stim_address),
    .cmd_ip         (cmd),
    .valid_ip       (stim_valid),
    .thread_id_ip   (stim_thread_id),
    .timing_op      (new_data_timing_o),
    .xcpt_op        (new_data_xcpt_o),
    .enter_debug_op (new_data_enter_debug_o),
    .update_op      (new_data_update_o)
  );

  // ── TriggerType=1 (data) — Original module ──
  debug_breakpoint_orig #(.TRIGGER_TYPE(1)) u_orig_data (
    .control_ip     (control),
    .in_debug_ip    (stim_in_debug),
    .prv_ip         (prv),
    .tdata2_ip      (tdata2),
    .address_ip     (stim_address),
    .cmd_ip         (cmd),
    .valid_ip       (stim_valid),
    .thread_id_ip   (stim_thread_id),
    .timing_op      (orig_data_timing_o),
    .xcpt_op        (orig_data_xcpt_o),
    .enter_debug_op (orig_data_enter_debug_o),
    .update_op      (orig_data_update_o)
  );

endmodule
