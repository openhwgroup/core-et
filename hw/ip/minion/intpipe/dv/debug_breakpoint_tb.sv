// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for the standard sim_ctrl harness; debug_breakpoint is combinational.
module debug_breakpoint_tb
  import minion_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [8:0]  control0_i,
  input  logic [8:0]  control1_i,
  input  logic [1:0]  in_debug_i,
  input  logic [1:0]  prv0_i,
  input  logic [1:0]  prv1_i,
  input  logic [47:0] tdata2_0_i,
  input  logic [47:0] tdata2_1_i,
  input  logic [48:0] address_i,
  input  logic [2:0]  cmd_i,
  input  logic        valid_i,
  input  logic        thread_id_i,

  output logic        insn_timing_o,
  output logic        insn_xcpt_o,
  output logic        insn_enter_debug_o,
  output logic        insn_update_o,
  output logic        data_timing_o,
  output logic        data_xcpt_o,
  output logic        data_enter_debug_o,
  output logic        data_update_o
);
/* verilator lint_on UNUSED */

  minion_bp_ctrl_t [NrThreadsDefault-1:0]  control;
  logic [NrThreadsDefault-1:0][1:0]        prv;
  logic [NrThreadsDefault-1:0][VaSize-1:0] tdata2;

  always_comb begin
    control[0] = minion_bp_ctrl_t'(control0_i);
    control[1] = minion_bp_ctrl_t'(control1_i);
    prv[0]     = prv0_i;
    prv[1]     = prv1_i;
    tdata2[0]  = tdata2_0_i;
    tdata2[1]  = tdata2_1_i;
  end

  debug_breakpoint #(.TriggerType(0)) u_insn (
    .control_ip     (control),
    .in_debug_ip    (in_debug_i),
    .prv_ip         (prv),
    .tdata2_ip      (tdata2),
    .address_ip     (address_i),
    .cmd_ip         (debug_bp_e'(cmd_i)),
    .valid_ip       (valid_i),
    .thread_id_ip   (thread_id_i),
    .timing_op      (insn_timing_o),
    .xcpt_op        (insn_xcpt_o),
    .enter_debug_op (insn_enter_debug_o),
    .update_op      (insn_update_o)
  );

  debug_breakpoint #(.TriggerType(1)) u_data (
    .control_ip     (control),
    .in_debug_ip    (in_debug_i),
    .prv_ip         (prv),
    .tdata2_ip      (tdata2),
    .address_ip     (address_i),
    .cmd_ip         (debug_bp_e'(cmd_i)),
    .valid_ip       (valid_i),
    .thread_id_ip   (thread_id_i),
    .timing_op      (data_timing_o),
    .xcpt_op        (data_xcpt_o),
    .enter_debug_op (data_enter_debug_o),
    .update_op      (data_update_o)
  );

endmodule
