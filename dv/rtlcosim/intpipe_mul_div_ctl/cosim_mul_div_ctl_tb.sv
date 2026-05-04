// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_mul_div_ctl (new) vs original.
//
// Both modules are latch-based dual-phase controllers. The testbench
// drives identical inputs (req interface + dp feedback) and compares
// all outputs every cycle.

`include "soc.vh"

module cosim_mul_div_ctl_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus — from pipeline control
  input  logic [4:0]  stim_req_fn,
  input  logic [4:0]  stim_req_fn_early,
  input  logic        stim_req_valid_early,
  input  logic        stim_resp_ready,
  input  logic [6:0]  stim_req_dest,
  input  logic        stim_kill,
  input  logic        stim_ex_dw_2p,

  // Stimulus — from ex datapath
  input  logic        stim_ex_op1_msb_2p,
  input  logic        stim_ex_op2_msb_2p,

  // Stimulus — from dp feedback
  input  logic [7:0]  stim_md_mplr_2d,
  input  logic        stim_md_rem_sgn_1d,
  input  logic        stim_md_zero_rem_1d,
  input  logic        stim_md_nxt_quot_1q,
  input  logic        stim_md_div_by_zero_1p,

  // New module outputs
  output logic [6:0]  new_resp_dest,
  output logic        new_req_ready,
  output logic        new_resp_valid,
  output logic        new_resp_valid_early,
  output logic        new_op1_sgnd_2p,
  output logic        new_op2_sgnd_2p,
  output logic [2:0]  new_mdctl_besl_0_1p,
  output logic [2:0]  new_mdctl_besl_1_1p,
  output logic [2:0]  new_mdctl_besl_2_1p,
  output logic [2:0]  new_mdctl_besl_3_1p,
  output logic        new_mdctl_start_op_2p,
  output logic        new_mdctl_fnl_pp_1p,
  output logic        new_mdctl_dw_1q,
  output logic        new_mdctl_dw_2q,
  output logic [1:0]  new_mdctl_acc_v1_sl_1d,
  output logic [1:0]  new_mdctl_acc_v2_sl_1d,
  output logic        new_mdctl_add_ain_sl_1p,
  output logic [1:0]  new_mdctl_add_bin_sl_1p,
  output logic        new_mdctl_se_zero_1p,
  output logic        new_mdctl_fnl_mul_cyc_1p,
  output logic        new_mdctl_start_div_2p,
  output logic        new_mdctl_fc_zr_acc_1p,
  output logic        new_mdctl_dvsr_sgn_1p,
  output logic        new_mdctl_div_cin_1p,
  output logic [1:0]  new_mdctl_resp_sl_2p,
  output logic        new_mdctl_clken_mpcnd_1d,
  output logic        new_mdctl_clken_mul_ph1_2d,
  output logic        new_mdctl_clken_muldvsr_ph2_1d,
  output logic        new_mdctl_clken_acc_v1_2p,
  output logic        new_mdctl_clken_acc_v1_1p,
  output logic        new_mdctl_clken_acc_v2_2p,
  output logic        new_mdctl_clken_acc_v2_1p,

  // Original module outputs
  output logic [6:0]  orig_resp_dest,
  output logic        orig_req_ready,
  output logic        orig_resp_valid,
  output logic        orig_resp_valid_early,
  output logic        orig_op1_sgnd_2p,
  output logic        orig_op2_sgnd_2p,
  output logic [2:0]  orig_mdctl_besl_0_1p,
  output logic [2:0]  orig_mdctl_besl_1_1p,
  output logic [2:0]  orig_mdctl_besl_2_1p,
  output logic [2:0]  orig_mdctl_besl_3_1p,
  output logic        orig_mdctl_start_op_2p,
  output logic        orig_mdctl_fnl_pp_1p,
  output logic        orig_mdctl_dw_1q,
  output logic        orig_mdctl_dw_2q,
  output logic [1:0]  orig_mdctl_acc_v1_sl_1d,
  output logic [1:0]  orig_mdctl_acc_v2_sl_1d,
  output logic        orig_mdctl_add_ain_sl_1p,
  output logic [1:0]  orig_mdctl_add_bin_sl_1p,
  output logic        orig_mdctl_se_zero_1p,
  output logic        orig_mdctl_fnl_mul_cyc_1p,
  output logic        orig_mdctl_start_div_2p,
  output logic        orig_mdctl_fc_zr_acc_1p,
  output logic        orig_mdctl_dvsr_sgn_1p,
  output logic        orig_mdctl_div_cin_1p,
  output logic [1:0]  orig_mdctl_resp_sl_2p,
  output logic        orig_mdctl_clken_mpcnd_1d,
  output logic        orig_mdctl_clken_mul_ph1_2d,
  output logic        orig_mdctl_clken_muldvsr_ph2_1d,
  output logic        orig_mdctl_clken_acc_v1_2p,
  output logic        orig_mdctl_clken_acc_v1_1p,
  output logic        orig_mdctl_clken_acc_v2_2p,
  output logic        orig_mdctl_clken_acc_v2_1p
);

  import minion_pkg::*;

  // Both modules use clock (gated) and reset (active-high synchronous).
  logic clock_sig;
  logic reset_sig;
  assign clock_sig = clk_i;
  assign reset_sig = !rst_ni;

  // ---- New module (SystemVerilog) ----
  minion_reg_dest_t new_resp_dest_s;

  intpipe_mul_div_ctl u_new (
    .resp_dest                   (new_resp_dest_s),
    .req_ready                   (new_req_ready),
    .resp_valid                  (new_resp_valid),
    .resp_valid_early            (new_resp_valid_early),
    .op1_sgnd_2p                 (new_op1_sgnd_2p),
    .op2_sgnd_2p                 (new_op2_sgnd_2p),
    .mdctl_besl_0_1p             (new_mdctl_besl_0_1p),
    .mdctl_besl_1_1p             (new_mdctl_besl_1_1p),
    .mdctl_besl_2_1p             (new_mdctl_besl_2_1p),
    .mdctl_besl_3_1p             (new_mdctl_besl_3_1p),
    .mdctl_start_op_2p           (new_mdctl_start_op_2p),
    .mdctl_fnl_pp_1p             (new_mdctl_fnl_pp_1p),
    .mdctl_dw_1q                 (new_mdctl_dw_1q),
    .mdctl_dw_2q                 (new_mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d          (new_mdctl_acc_v1_sl_1d),
    .mdctl_acc_v2_sl_1d          (new_mdctl_acc_v2_sl_1d),
    .mdctl_add_ain_sl_1p         (new_mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p         (new_mdctl_add_bin_sl_1p),
    .mdctl_se_zero_1p            (new_mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p        (new_mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p          (new_mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p          (new_mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p           (new_mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p            (new_mdctl_div_cin_1p),
    .mdctl_resp_sl_2p            (new_mdctl_resp_sl_2p),
    .mdctl_clken_mpcnd_1d        (new_mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d      (new_mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d  (new_mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p       (new_mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p       (new_mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p       (new_mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p       (new_mdctl_clken_acc_v2_1p),
    .ex_op1_msb_2p               (stim_ex_op1_msb_2p),
    .ex_op2_msb_2p               (stim_ex_op2_msb_2p),
    .md_mplr_2d                  (stim_md_mplr_2d),
    .md_rem_sgn_1d               (stim_md_rem_sgn_1d),
    .md_zero_rem_1d              (stim_md_zero_rem_1d),
    .md_nxt_quot_1q              (stim_md_nxt_quot_1q),
    .md_div_by_zero_1p           (stim_md_div_by_zero_1p),
    .req_fn                      (alu_func_e'(stim_req_fn)),
    .req_fn_early                (alu_func_e'(stim_req_fn_early)),
    .req_valid_early             (stim_req_valid_early),
    .resp_ready                  (stim_resp_ready),
    .req_dest                    (minion_reg_dest_t'(stim_req_dest)),
    .kill                        (stim_kill),
    .ex_dw_2p                    (stim_ex_dw_2p),
    .reset                       (reset_sig),
    .clock                       (clock_sig)
  );
  assign new_resp_dest = new_resp_dest_s;

  // ---- Original module (Verilog) ----
  minion_reg_dest orig_resp_dest_s;

  intpipe_mul_div_ctl_orig u_orig (
    .resp_dest                   (orig_resp_dest_s),
    .req_ready                   (orig_req_ready),
    .resp_valid                  (orig_resp_valid),
    .resp_valid_early            (orig_resp_valid_early),
    .op1_sgnd_2p                 (orig_op1_sgnd_2p),
    .op2_sgnd_2p                 (orig_op2_sgnd_2p),
    .mdctl_besl_0_1p             (orig_mdctl_besl_0_1p),
    .mdctl_besl_1_1p             (orig_mdctl_besl_1_1p),
    .mdctl_besl_2_1p             (orig_mdctl_besl_2_1p),
    .mdctl_besl_3_1p             (orig_mdctl_besl_3_1p),
    .mdctl_start_op_2p           (orig_mdctl_start_op_2p),
    .mdctl_fnl_pp_1p             (orig_mdctl_fnl_pp_1p),
    .mdctl_dw_1q                 (orig_mdctl_dw_1q),
    .mdctl_dw_2q                 (orig_mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d          (orig_mdctl_acc_v1_sl_1d),
    .mdctl_acc_v2_sl_1d          (orig_mdctl_acc_v2_sl_1d),
    .mdctl_add_ain_sl_1p         (orig_mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p         (orig_mdctl_add_bin_sl_1p),
    .mdctl_se_zero_1p            (orig_mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p        (orig_mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p          (orig_mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p          (orig_mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p           (orig_mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p            (orig_mdctl_div_cin_1p),
    .mdctl_resp_sl_2p            (orig_mdctl_resp_sl_2p),
    .mdctl_clken_mpcnd_1d        (orig_mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d      (orig_mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d  (orig_mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p       (orig_mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p       (orig_mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p       (orig_mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p       (orig_mdctl_clken_acc_v2_1p),
    .ex_op1_msb_2p               (stim_ex_op1_msb_2p),
    .ex_op2_msb_2p               (stim_ex_op2_msb_2p),
    .md_mplr_2d                  (stim_md_mplr_2d),
    .md_rem_sgn_1d               (stim_md_rem_sgn_1d),
    .md_zero_rem_1d              (stim_md_zero_rem_1d),
    .md_nxt_quot_1q              (stim_md_nxt_quot_1q),
    .md_div_by_zero_1p           (stim_md_div_by_zero_1p),
    .req_fn                      (core_alu_func'(stim_req_fn)),
    .req_fn_early                (core_alu_func'(stim_req_fn_early)),
    .req_valid_early             (stim_req_valid_early),
    .resp_ready                  (stim_resp_ready),
    .req_dest                    (minion_reg_dest'(stim_req_dest)),
    .kill                        (stim_kill),
    .ex_dw_2p                    (stim_ex_dw_2p),
    .reset                       (reset_sig),
    .clock                       (clock_sig)
  );
  assign orig_resp_dest = orig_resp_dest_s;

endmodule
