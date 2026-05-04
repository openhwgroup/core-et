// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Integer Multiplier Divide Top
// Project       : Minion
//-----------------------------------------------------------------------------
// File          : intpipe_mul_div_top.v
// Author        : Jay Patel  <jay@konstruisto.esperantotech.com>
// Created       : 20.04.2018
// Last modified : 20.04.2018
//-----------------------------------------------------------------------------
// Description :
// 
// Modification history :
// 20.04.2018 : created
//-----------------------------------------------------------------------------

`include "soc.vh"


module intpipe_mul_div_top (/*AUTOARG*/
  // Outputs
  req_ready, resp_valid, resp_valid_early, resp_data, resp_dest,
  // Inputs
  clock_aon, reset, req_valid, req_valid_early, req_fn, req_fn_early,
  req_dw, req_in1, req_in2, req_dest, kill, chicken_bit_mul_div, resp_ready
);


  // System signals
  input  logic               clock_aon;
  input  logic               reset;
  // Request port
  output logic               req_ready;
  input  logic               req_valid;
  input  logic               req_valid_early;
  input  core_alu_func       req_fn;
  input  core_alu_func       req_fn_early;
  input  logic               req_dw;
  input  logic [`XREG_RANGE] req_in1;
  input  logic [`XREG_RANGE] req_in2;
  //input [`XREG_ADDR_RANGE]   req_dest_addr;           // destination register
  //input                      req_dest_thread_id;      // destination register
  input  minion_reg_dest     req_dest;
  input  logic               kill;
  // Chicken bits
  input  logic               chicken_bit_mul_div;
  // Response port
  input  logic               resp_ready;
  output logic               resp_valid;
  output logic               resp_valid_early;
  output logic [`XREG_RANGE] resp_data;
  //output [`XREG_ADDR_RANGE]  resp_dest_addr;          // response destination address
  //output                     resp_dest_thread_id;     // response destination thread id
  output minion_reg_dest     resp_dest;

   
  /*AUTOOUTPUT*/
  /*AUTOINPUT*/

  /*AUTOWIRE*/
  // Beginning of automatic wires (for undeclared instantiated-module outputs)
  wire                 md_div_by_zero_1p;      // From mul_div_dp of intpipe_mul_div_dp.v
  wire [7:0]           md_mplr_2d;             // From mul_div_dp of intpipe_mul_div_dp.v
  wire                 md_nxt_quot_1q;         // From mul_div_dp of intpipe_mul_div_dp.v
  wire                 md_rem_sgn_1d;          // From mul_div_dp of intpipe_mul_div_dp.v
  wire                 md_zero_rem_1d;         // From mul_div_dp of intpipe_mul_div_dp.v
  wire [1:0]           mdctl_acc_v1_sl_1d;     // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire [1:0]           mdctl_acc_v2_sl_1d;     // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_add_ain_sl_1p;    // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire [1:0]           mdctl_add_bin_sl_1p;    // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire [2:0]           mdctl_besl_0_1p;        // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire [2:0]           mdctl_besl_1_1p;        // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire [2:0]           mdctl_besl_2_1p;        // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire [2:0]           mdctl_besl_3_1p;        // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_clken_acc_v1_1p;  // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_clken_acc_v1_2p;  // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_clken_acc_v2_1p;  // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_clken_acc_v2_2p;  // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_clken_mpcnd_1d;   // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_clken_mul_ph1_2d; // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_clken_muldvsr_ph2_1d;// From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_div_cin_1p;       // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_dvsr_sgn_1p;      // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_dw_1q;            // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_dw_2q;            // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_fc_zr_acc_1p;     // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_fnl_mul_cyc_1p;   // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_fnl_pp_1p;        // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire [1:0]           mdctl_resp_sl_2p;       // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_se_zero_1p;       // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_start_div_2p;     // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 mdctl_start_op_2p;      // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 op1_sgnd_2p;            // From mul_div_ctl of intpipe_mul_div_ctl.v
  wire                 op2_sgnd_2p;            // From mul_div_ctl of intpipe_mul_div_ctl.v
  // End of automatics

  wire                 scan_shift_en;

  logic                clock;
  logic                clock_enable;

  assign clock_enable = chicken_bit_mul_div | req_valid | req_valid_early | !req_ready | reset;

  et_clk_gate clk_gate_wb (.enclk(clock), .en(clock_enable), .clk(clock_aon), .te(1'b0));
   
   assign scan_shift_en = 1'b0;  // DFT_SCAN_INSERT

   // ri lint_check_off FULL_RANGE
   /* 
    intpipe_mul_div_dp AUTO_TEMPLATE(
        .md_resp_data_2p  (resp_data[]),
        .ex_op1_2p      (req_in1[]),
        .ex_op2_2p      (req_in2[]),
        // debug outputs
        .mul_hi_1q            (),
        .mul_lo_1q            (),
        .rem_1q               (),
        .quot_1q              (),
        .ex_dw_2p        (req_dw),
    );

    intpipe_mul_div_ctl AUTO_TEMPLATE(
        .ex_op1_msb_2p   (req_dw ? req_in1[63] : req_in1[31]),
        .ex_op2_msb_2p   (req_dw ? req_in2[63] : req_in2[31]),
        .ex_dw_2p        (req_dw),
    );

    */

  intpipe_mul_div_dp mul_div_dp (/*AUTOINST*/
    // Outputs
    .md_resp_data_2p            (resp_data[`XREG_RANGE]), // Templated
    .md_mplr_2d                 (md_mplr_2d[7:0]),
    .md_rem_sgn_1d              (md_rem_sgn_1d),
    .md_zero_rem_1d             (md_zero_rem_1d),
    .md_nxt_quot_1q             (md_nxt_quot_1q),
    .md_div_by_zero_1p          (md_div_by_zero_1p),
    // Inputs
    .ex_op1_2p                  (req_in1[`XREG_RANGE]), // Templated
    .ex_op2_2p                  (req_in2[`XREG_RANGE]), // Templated
    .op1_sgnd_2p                (op1_sgnd_2p),
    .op2_sgnd_2p                (op2_sgnd_2p),
    .ex_dw_2p                   (req_dw),        // Templated
    .mdctl_besl_0_1p            (mdctl_besl_0_1p[2:0]),
    .mdctl_besl_1_1p            (mdctl_besl_1_1p[2:0]),
    .mdctl_besl_2_1p            (mdctl_besl_2_1p[2:0]),
    .mdctl_besl_3_1p            (mdctl_besl_3_1p[2:0]),
    .mdctl_start_op_2p          (mdctl_start_op_2p),
    .mdctl_fnl_pp_1p            (mdctl_fnl_pp_1p),
    .mdctl_dw_1q                (mdctl_dw_1q),
    .mdctl_dw_2q                (mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d         (mdctl_acc_v1_sl_1d[1:0]),
    .mdctl_acc_v2_sl_1d         (mdctl_acc_v2_sl_1d[1:0]),
    .mdctl_add_ain_sl_1p        (mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p        (mdctl_add_bin_sl_1p[1:0]),
    .mdctl_se_zero_1p           (mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p       (mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p         (mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p         (mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p          (mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p           (mdctl_div_cin_1p),
    .mdctl_resp_sl_2p           (mdctl_resp_sl_2p[1:0]),
    .mdctl_clken_mpcnd_1d       (mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d     (mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d (mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p      (mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p      (mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p      (mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p      (mdctl_clken_acc_v2_1p),
    .clock                      (clock),
    .scan_shift_en              (scan_shift_en)
  );

  intpipe_mul_div_ctl mul_div_ctl (/*AUTOINST*/
    // Outputs
    .resp_dest                  (resp_dest),
    .req_ready                  (req_ready),
    .resp_valid                 (resp_valid),
    .resp_valid_early           (resp_valid_early),
    .op1_sgnd_2p                (op1_sgnd_2p),
    .op2_sgnd_2p                (op2_sgnd_2p),
    .mdctl_besl_0_1p            (mdctl_besl_0_1p[2:0]),
    .mdctl_besl_1_1p            (mdctl_besl_1_1p[2:0]),
    .mdctl_besl_2_1p            (mdctl_besl_2_1p[2:0]),
    .mdctl_besl_3_1p            (mdctl_besl_3_1p[2:0]),
    .mdctl_start_op_2p          (mdctl_start_op_2p),
    .mdctl_fnl_pp_1p            (mdctl_fnl_pp_1p),
    .mdctl_dw_1q                (mdctl_dw_1q),
    .mdctl_dw_2q                (mdctl_dw_2q),
    .mdctl_acc_v1_sl_1d         (mdctl_acc_v1_sl_1d[1:0]),
    .mdctl_acc_v2_sl_1d         (mdctl_acc_v2_sl_1d[1:0]),
    .mdctl_add_ain_sl_1p        (mdctl_add_ain_sl_1p),
    .mdctl_add_bin_sl_1p        (mdctl_add_bin_sl_1p[1:0]),
    .mdctl_se_zero_1p           (mdctl_se_zero_1p),
    .mdctl_fnl_mul_cyc_1p       (mdctl_fnl_mul_cyc_1p),
    .mdctl_start_div_2p         (mdctl_start_div_2p),
    .mdctl_fc_zr_acc_1p         (mdctl_fc_zr_acc_1p),
    .mdctl_dvsr_sgn_1p          (mdctl_dvsr_sgn_1p),
    .mdctl_div_cin_1p           (mdctl_div_cin_1p),
    .mdctl_resp_sl_2p           (mdctl_resp_sl_2p[1:0]),
    .mdctl_clken_mpcnd_1d       (mdctl_clken_mpcnd_1d),
    .mdctl_clken_mul_ph1_2d     (mdctl_clken_mul_ph1_2d),
    .mdctl_clken_muldvsr_ph2_1d (mdctl_clken_muldvsr_ph2_1d),
    .mdctl_clken_acc_v1_2p      (mdctl_clken_acc_v1_2p),
    .mdctl_clken_acc_v1_1p      (mdctl_clken_acc_v1_1p),
    .mdctl_clken_acc_v2_2p      (mdctl_clken_acc_v2_2p),
    .mdctl_clken_acc_v2_1p      (mdctl_clken_acc_v2_1p),
    // Inputs
    .ex_op1_msb_2p              (req_dw ? req_in1[63] : req_in1[31]), // Templated
    .ex_op2_msb_2p              (req_dw ? req_in2[63] : req_in2[31]), // Templated
    .md_mplr_2d                 (md_mplr_2d[7:0]),
    .md_rem_sgn_1d              (md_rem_sgn_1d),
    .md_zero_rem_1d             (md_zero_rem_1d),
    .md_nxt_quot_1q             (md_nxt_quot_1q),
    .md_div_by_zero_1p          (md_div_by_zero_1p),
    .req_fn                     (req_fn),
    .req_fn_early               (req_fn_early),
    .req_valid_early            (req_valid_early),
    .resp_ready                 (resp_ready),
    .req_dest                   (req_dest),
    .kill                       (kill),
    .ex_dw_2p                   (req_dw),        // Templated
    .reset                      (reset),
    .clock                      (clock)
  );

   // ri lint_check_on FULL_RANGE

endmodule // intpipe_mul_div_top
