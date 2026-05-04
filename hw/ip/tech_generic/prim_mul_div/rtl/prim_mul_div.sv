// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_mul_div -- Default mul/div primitive implementation.
//
// This is the technology seam for the Minion integer mul/div unit. The
// generic/default body preserves the current translated latch-based
// implementation and its external request/response timing contract so
// technology-specific trees can replace only this block later.

module prim_mul_div
  import minion_pkg::*;
  import dft_pkg::*;
(
  input  logic                  clk_i,
  input  logic                  rst_ni,
  input  dft_t                  dft_i,
  output logic                  req_ready,
  input  logic                  req_valid,
  input  logic                  req_valid_early,
  input  alu_func_e             req_fn,
  input  alu_func_e             req_fn_early,
  input  logic                  req_dw,
  input  logic [XregSize-1:0]   req_in1,
  input  logic [XregSize-1:0]   req_in2,
  input  minion_reg_dest_t      req_dest,
  input  logic                  kill,
  input  logic                  chicken_bit_mul_div,
  input  logic                  resp_ready,
  output logic                  resp_valid,
  output logic                  resp_valid_early,
  output logic [XregSize-1:0]   resp_data,
  output minion_reg_dest_t      resp_dest
);

  logic                 md_div_by_zero_1p;
  logic [7:0]           md_mplr_2d;
  logic                 md_nxt_quot_1q;
  logic                 md_rem_sgn_1d;
  logic                 md_zero_rem_1d;
  logic [1:0]           mdctl_acc_v1_sl_1d;
  logic [1:0]           mdctl_acc_v2_sl_1d;
  logic                 mdctl_add_ain_sl_1p;
  logic [1:0]           mdctl_add_bin_sl_1p;
  logic [2:0]           mdctl_besl_0_1p;
  logic [2:0]           mdctl_besl_1_1p;
  logic [2:0]           mdctl_besl_2_1p;
  logic [2:0]           mdctl_besl_3_1p;
  logic                 mdctl_clken_acc_v1_1p;
  logic                 mdctl_clken_acc_v1_2p;
  logic                 mdctl_clken_acc_v2_1p;
  logic                 mdctl_clken_acc_v2_2p;
  logic                 mdctl_clken_mpcnd_1d;
  logic                 mdctl_clken_mul_ph1_2d;
  logic                 mdctl_clken_muldvsr_ph2_1d;
  logic                 mdctl_div_cin_1p;
  logic                 mdctl_dvsr_sgn_1p;
  logic                 mdctl_dw_1q;
  logic                 mdctl_dw_2q;
  logic                 mdctl_fc_zr_acc_1p;
  logic                 mdctl_fnl_mul_cyc_1p;
  logic                 mdctl_fnl_pp_1p;
  logic [1:0]           mdctl_resp_sl_2p;
  logic                 mdctl_se_zero_1p;
  logic                 mdctl_start_div_2p;
  logic                 mdctl_start_op_2p;
  logic                 op1_sgnd_2p;
  logic                 op2_sgnd_2p;
  logic                 scan_shift_en;
  logic                 clock;
  logic                 clock_enable;
  logic                 reset;
  /* verilator lint_off UNUSEDSIGNAL */  // Optional debug ports on subblocks are tied off in the architectural top and consumed only by project-local debug wrappers.
  logic                 unused_debug_start_mul_1p;
  logic                 unused_debug_start_mul_2p;
  logic                 unused_debug_mul_op_1p;
  logic                 unused_debug_mul_op_2p;
  logic [6:0]           unused_debug_loop_cnt_1p;
  logic [6:0]           unused_debug_loop_cnt_2p;
  logic [XregSize:0]    unused_debug_mpcnd_1p;
  logic [XregSize:0]    unused_debug_mul_lo_dvsr_1p;
  logic [XregSize-1:0]  unused_debug_mul_lo_2p;
  logic [XregSize+1:0]  unused_debug_acc_v1_1q;
  logic [XregSize+1:0]  unused_debug_acc_v2_1q;
  /* verilator lint_on UNUSEDSIGNAL */

  assign reset = !rst_ni;
  assign clock_enable = chicken_bit_mul_div | req_valid | req_valid_early | !req_ready | reset;

  prim_clk_gate clk_gate_wb (
    .clk_i  (clk_i),
    .en_i   (clock_enable),
    .dft_i  (dft_i),
    .clk_o  (clock)
  );

  assign scan_shift_en = 1'b0;  // DFT_SCAN_INSERT

  intpipe_mul_div_dp mul_div_dp (
    .md_resp_data_2p                (resp_data[XregSize-1:0]),
    .md_mplr_2d                     (md_mplr_2d[7:0]),
    .md_rem_sgn_1d                  (md_rem_sgn_1d),
    .md_zero_rem_1d                 (md_zero_rem_1d),
    .md_nxt_quot_1q                 (md_nxt_quot_1q),
    .md_div_by_zero_1p              (md_div_by_zero_1p),
    .debug_mpcnd_1p_o               (unused_debug_mpcnd_1p),
    .debug_mul_lo_dvsr_1p_o         (unused_debug_mul_lo_dvsr_1p),
    .debug_mul_lo_2p_o              (unused_debug_mul_lo_2p),
    .debug_acc_v1_1q_o              (unused_debug_acc_v1_1q),
    .debug_acc_v2_1q_o              (unused_debug_acc_v2_1q),
    .ex_op1_2p                      (req_in1[XregSize-1:0]),
    .ex_op2_2p                      (req_in2[XregSize-1:0]),
    .op1_sgnd_2p                    (op1_sgnd_2p),
    .op2_sgnd_2p                    (op2_sgnd_2p),
    .ex_dw_2p                       (req_dw),
    .mdctl_besl_0_1p                (mdctl_besl_0_1p[2:0]),
    .mdctl_besl_1_1p                (mdctl_besl_1_1p[2:0]),
    .mdctl_besl_2_1p                (mdctl_besl_2_1p[2:0]),
    .mdctl_besl_3_1p                (mdctl_besl_3_1p[2:0]),
    .mdctl_start_op_2p              (mdctl_start_op_2p),
    .mdctl_fnl_pp_1p                (mdctl_fnl_pp_1p),
    .mdctl_dw_1q                    (mdctl_dw_1q),
    .mdctl_dw_2q                    (mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d             (mdctl_acc_v1_sl_1d[1:0]),
    .mdctl_acc_v2_sl_1d             (mdctl_acc_v2_sl_1d[1:0]),
    .mdctl_add_ain_sl_1p            (mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p            (mdctl_add_bin_sl_1p[1:0]),
    .mdctl_se_zero_1p               (mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p           (mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p             (mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p             (mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p              (mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p               (mdctl_div_cin_1p),
    .mdctl_resp_sl_2p               (mdctl_resp_sl_2p[1:0]),
    .mdctl_clken_mpcnd_1d           (mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d         (mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d     (mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p          (mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p          (mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p          (mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p          (mdctl_clken_acc_v2_1p),
    .clock                          (clock),
    .scan_shift_en                  (scan_shift_en)
  );

  intpipe_mul_div_ctl mul_div_ctl (
    .resp_dest                      (resp_dest),
    .req_ready                      (req_ready),
    .resp_valid                     (resp_valid),
    .resp_valid_early               (resp_valid_early),
    .debug_start_mul_1p_o           (unused_debug_start_mul_1p),
    .debug_start_mul_2p_o           (unused_debug_start_mul_2p),
    .debug_mul_op_1p_o              (unused_debug_mul_op_1p),
    .debug_mul_op_2p_o              (unused_debug_mul_op_2p),
    .debug_loop_cnt_1p_o            (unused_debug_loop_cnt_1p),
    .debug_loop_cnt_2p_o            (unused_debug_loop_cnt_2p),
    .op1_sgnd_2p                    (op1_sgnd_2p),
    .op2_sgnd_2p                    (op2_sgnd_2p),
    .mdctl_besl_0_1p                (mdctl_besl_0_1p[2:0]),
    .mdctl_besl_1_1p                (mdctl_besl_1_1p[2:0]),
    .mdctl_besl_2_1p                (mdctl_besl_2_1p[2:0]),
    .mdctl_besl_3_1p                (mdctl_besl_3_1p[2:0]),
    .mdctl_start_op_2p              (mdctl_start_op_2p),
    .mdctl_fnl_pp_1p                (mdctl_fnl_pp_1p),
    .mdctl_dw_1q                    (mdctl_dw_1q),
    .mdctl_dw_2q                    (mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d             (mdctl_acc_v1_sl_1d[1:0]),
    .mdctl_acc_v2_sl_1d             (mdctl_acc_v2_sl_1d[1:0]),
    .mdctl_add_ain_sl_1p            (mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p            (mdctl_add_bin_sl_1p[1:0]),
    .mdctl_se_zero_1p               (mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p           (mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p             (mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p             (mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p              (mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p               (mdctl_div_cin_1p),
    .mdctl_resp_sl_2p               (mdctl_resp_sl_2p[1:0]),
    .mdctl_clken_mpcnd_1d           (mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d         (mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d     (mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p          (mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p          (mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p          (mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p          (mdctl_clken_acc_v2_1p),
    .ex_op1_msb_2p                  (req_dw ? req_in1[63] : req_in1[31]),
    .ex_op2_msb_2p                  (req_dw ? req_in2[63] : req_in2[31]),
    .md_mplr_2d                     (md_mplr_2d[7:0]),
    .md_rem_sgn_1d                  (md_rem_sgn_1d),
    .md_zero_rem_1d                 (md_zero_rem_1d),
    .md_nxt_quot_1q                 (md_nxt_quot_1q),
    .md_div_by_zero_1p              (md_div_by_zero_1p),
    .req_fn                         (req_fn),
    .req_fn_early                   (req_fn_early),
    .req_valid_early                (req_valid_early),
    .resp_ready                     (resp_ready),
    .req_dest                       (req_dest),
    .kill                           (kill),
    .ex_dw_2p                       (req_dw),
    .reset                          (reset),
    .clock                          (clock)
  );

endmodule // prim_mul_div
