// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_mul_div_dp (new) vs original.
//
// Both modules are latch-based dual-phase datapaths. The testbench
// drives identical control and data inputs and compares all 6 outputs
// every cycle.

`include "soc.vh"

module cosim_mul_div_dp_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus — data operands
  input  logic [63:0] stim_ex_op1_2p,
  input  logic [63:0] stim_ex_op2_2p,

  // Stimulus — control from ctl
  input  logic        stim_op1_sgnd_2p,
  input  logic        stim_op2_sgnd_2p,
  input  logic        stim_ex_dw_2p,

  input  logic [2:0]  stim_mdctl_besl_0_1p,
  input  logic [2:0]  stim_mdctl_besl_1_1p,
  input  logic [2:0]  stim_mdctl_besl_2_1p,
  input  logic [2:0]  stim_mdctl_besl_3_1p,
  input  logic        stim_mdctl_start_op_2p,
  input  logic        stim_mdctl_fnl_pp_1p,
  input  logic        stim_mdctl_dw_1q,
  input  logic        stim_mdctl_dw_2q,
  input  logic [1:0]  stim_mdctl_acc_v1_sl_1d,
  input  logic [1:0]  stim_mdctl_acc_v2_sl_1d,
  input  logic        stim_mdctl_add_ain_sl_1p,
  input  logic [1:0]  stim_mdctl_add_bin_sl_1p,
  input  logic        stim_mdctl_se_zero_1p,
  input  logic        stim_mdctl_fnl_mul_cyc_1p,
  input  logic        stim_mdctl_start_div_2p,
  input  logic        stim_mdctl_fc_zr_acc_1p,
  input  logic        stim_mdctl_dvsr_sgn_1p,
  input  logic        stim_mdctl_div_cin_1p,
  input  logic [1:0]  stim_mdctl_resp_sl_2p,

  input  logic        stim_mdctl_clken_mpcnd_1d,
  input  logic        stim_mdctl_clken_mul_ph1_2d,
  input  logic        stim_mdctl_clken_muldvsr_ph2_1d,
  input  logic        stim_mdctl_clken_acc_v1_2p,
  input  logic        stim_mdctl_clken_acc_v1_1p,
  input  logic        stim_mdctl_clken_acc_v2_2p,
  input  logic        stim_mdctl_clken_acc_v2_1p,

  // New module outputs
  output logic [63:0] new_md_resp_data_2p,
  output logic [7:0]  new_md_mplr_2d,
  output logic        new_md_rem_sgn_1d,
  output logic        new_md_zero_rem_1d,
  output logic        new_md_nxt_quot_1q,
  output logic        new_md_div_by_zero_1p,

  // Original module outputs
  output logic [63:0] orig_md_resp_data_2p,
  output logic [7:0]  orig_md_mplr_2d,
  output logic        orig_md_rem_sgn_1d,
  output logic        orig_md_zero_rem_1d,
  output logic        orig_md_nxt_quot_1q,
  output logic        orig_md_div_by_zero_1p
);

  import minion_pkg::*;
  import dft_pkg::*;

  // ---- New module (SystemVerilog) ----
  intpipe_mul_div_dp u_new (
    .md_resp_data_2p             (new_md_resp_data_2p),
    .md_mplr_2d                  (new_md_mplr_2d),
    .md_rem_sgn_1d               (new_md_rem_sgn_1d),
    .md_zero_rem_1d              (new_md_zero_rem_1d),
    .md_nxt_quot_1q              (new_md_nxt_quot_1q),
    .md_div_by_zero_1p           (new_md_div_by_zero_1p),
    .ex_op1_2p                   (stim_ex_op1_2p),
    .ex_op2_2p                   (stim_ex_op2_2p),
    .op1_sgnd_2p                 (stim_op1_sgnd_2p),
    .op2_sgnd_2p                 (stim_op2_sgnd_2p),
    .ex_dw_2p                    (stim_ex_dw_2p),
    .mdctl_besl_0_1p             (stim_mdctl_besl_0_1p),
    .mdctl_besl_1_1p             (stim_mdctl_besl_1_1p),
    .mdctl_besl_2_1p             (stim_mdctl_besl_2_1p),
    .mdctl_besl_3_1p             (stim_mdctl_besl_3_1p),
    .mdctl_start_op_2p           (stim_mdctl_start_op_2p),
    .mdctl_fnl_pp_1p             (stim_mdctl_fnl_pp_1p),
    .mdctl_dw_1q                 (stim_mdctl_dw_1q),
    .mdctl_dw_2q                 (stim_mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d          (stim_mdctl_acc_v1_sl_1d),
    .mdctl_acc_v2_sl_1d          (stim_mdctl_acc_v2_sl_1d),
    .mdctl_add_ain_sl_1p         (stim_mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p         (stim_mdctl_add_bin_sl_1p),
    .mdctl_se_zero_1p            (stim_mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p        (stim_mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p          (stim_mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p          (stim_mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p           (stim_mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p            (stim_mdctl_div_cin_1p),
    .mdctl_resp_sl_2p            (stim_mdctl_resp_sl_2p),
    .mdctl_clken_mpcnd_1d        (stim_mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d      (stim_mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d  (stim_mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p       (stim_mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p       (stim_mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p       (stim_mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p       (stim_mdctl_clken_acc_v2_1p),
    .clock                       (clk_i),
    .scan_shift_en               (1'b0)
  );

  // ---- Original module (Verilog) ----
  intpipe_mul_div_dp_orig u_orig (
    .md_resp_data_2p             (orig_md_resp_data_2p),
    .md_mplr_2d                  (orig_md_mplr_2d),
    .md_rem_sgn_1d               (orig_md_rem_sgn_1d),
    .md_zero_rem_1d              (orig_md_zero_rem_1d),
    .md_nxt_quot_1q              (orig_md_nxt_quot_1q),
    .md_div_by_zero_1p           (orig_md_div_by_zero_1p),
    .ex_op1_2p                   (stim_ex_op1_2p),
    .ex_op2_2p                   (stim_ex_op2_2p),
    .op1_sgnd_2p                 (stim_op1_sgnd_2p),
    .op2_sgnd_2p                 (stim_op2_sgnd_2p),
    .ex_dw_2p                    (stim_ex_dw_2p),
    .mdctl_besl_0_1p             (stim_mdctl_besl_0_1p),
    .mdctl_besl_1_1p             (stim_mdctl_besl_1_1p),
    .mdctl_besl_2_1p             (stim_mdctl_besl_2_1p),
    .mdctl_besl_3_1p             (stim_mdctl_besl_3_1p),
    .mdctl_start_op_2p           (stim_mdctl_start_op_2p),
    .mdctl_fnl_pp_1p             (stim_mdctl_fnl_pp_1p),
    .mdctl_dw_1q                 (stim_mdctl_dw_1q),
    .mdctl_dw_2q                 (stim_mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d          (stim_mdctl_acc_v1_sl_1d),
    .mdctl_acc_v2_sl_1d          (stim_mdctl_acc_v2_sl_1d),
    .mdctl_add_ain_sl_1p         (stim_mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p         (stim_mdctl_add_bin_sl_1p),
    .mdctl_se_zero_1p            (stim_mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p        (stim_mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p          (stim_mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p          (stim_mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p           (stim_mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p            (stim_mdctl_div_cin_1p),
    .mdctl_resp_sl_2p            (stim_mdctl_resp_sl_2p),
    .mdctl_clken_mpcnd_1d        (stim_mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d      (stim_mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d  (stim_mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p       (stim_mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p       (stim_mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p       (stim_mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p       (stim_mdctl_clken_acc_v2_1p),
    .clock                       (clk_i),
    .scan_shift_en               (1'b0)
  );

endmodule
