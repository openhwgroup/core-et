// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Multiplier / Divider Control
// Project       : Minion
//-----------------------------------------------------------------------------
// File          : intpipe_mul_div_ctl.v
// Author        : Jay Patel  <jay@konstruisto.esperantotech.com>
// Created       : 04.04.2018
// Last modified : 04.04.2018
//-----------------------------------------------------------------------------
// Description :
// 
// Modification history :
// 04.04.2018 : created
//-----------------------------------------------------------------------------


`include "soc.vh"

module intpipe_mul_div_ctl (/*AUTOARG*/
  // Outputs
  resp_dest, req_ready, resp_valid, resp_valid_early, op1_sgnd_2p,
  op2_sgnd_2p, mdctl_besl_0_1p, mdctl_besl_1_1p, mdctl_besl_2_1p,
  mdctl_besl_3_1p, mdctl_start_op_2p, mdctl_fnl_pp_1p, mdctl_dw_1q,
  mdctl_dw_2q, mdctl_acc_v1_sl_1d, mdctl_acc_v2_sl_1d,
  mdctl_add_ain_sl_1p, mdctl_add_bin_sl_1p, mdctl_se_zero_1p,
  mdctl_fnl_mul_cyc_1p, mdctl_start_div_2p, mdctl_fc_zr_acc_1p,
  mdctl_dvsr_sgn_1p, mdctl_div_cin_1p, mdctl_resp_sl_2p,
  mdctl_clken_mpcnd_1d, mdctl_clken_mul_ph1_2d,
  mdctl_clken_muldvsr_ph2_1d, mdctl_clken_acc_v1_2p,
  mdctl_clken_acc_v1_1p, mdctl_clken_acc_v2_2p,
  mdctl_clken_acc_v2_1p,
  // Inputs
  ex_op1_msb_2p, ex_op2_msb_2p, md_mplr_2d, md_rem_sgn_1d,
  md_zero_rem_1d, md_nxt_quot_1q, md_div_by_zero_1p, req_fn,
  req_fn_early, req_valid_early, resp_ready, req_dest, kill,
  ex_dw_2p, reset, clock
);

   // To bypass, stall  control
  // To bypass, stall  control
  output minion_reg_dest    resp_dest;               // response destination
  output                    req_ready;               // Not Busy
  output                    resp_valid;              // Multiply, Divide Result valid
  output                    resp_valid_early;

  // to mul/div datapath
  output                    op1_sgnd_2p;             // sign extend multiplicand/dividend
  output                    op2_sgnd_2p;             // sign extend multiplier/divisor

  output [2:0]              mdctl_besl_0_1p;         // booth encode select pp_0
  output [2:0]              mdctl_besl_1_1p;         // booth encode select pp_1
  output [2:0]              mdctl_besl_2_1p;         // booth encode select pp_2
  output [2:0]              mdctl_besl_3_1p;         // booth encode select pp_3
  output                    mdctl_start_op_2p;       // start multiply or divide
  output                    mdctl_fnl_pp_1p;         // select final partial product
  output                    mdctl_dw_1q;             // sign extend result for 32b ops
  output                    mdctl_dw_2q;             // sign extend result for 32b ops
  output [1:0]              mdctl_acc_v1_sl_1d;      // accumulator v1, remainder, mhi input select
  output [1:0]              mdctl_acc_v2_sl_1d;      // accumulator v2, quotient input select
  output                    mdctl_add_ain_sl_1p;     // select A-input of adder
  output [1:0]              mdctl_add_bin_sl_1p;     // select B-input of adder
  output                    mdctl_se_zero_1p;        // sign extend or zero Ain to adder
  output                    mdctl_fnl_mul_cyc_1p;    // final multiply cycle (sv + cv)
  output                    mdctl_start_div_2p;      // start new divide
  output                    mdctl_fc_zr_acc_1p;      // zero out acc vectors (first of multiply or divides)
  output                    mdctl_dvsr_sgn_1p;       // divisor sign
  output                    mdctl_div_cin_1p;        // divide carry in
  output [1:0]              mdctl_resp_sl_2p;        // response select

  output                    mdctl_clken_mpcnd_1d;    // Multiplicand clock enable
  output                    mdctl_clken_mul_ph1_2d;  // Multiplier clock enable
  output                    mdctl_clken_muldvsr_ph2_1d; // Multiplier / Divisor clock enable
  output                    mdctl_clken_acc_v1_2p;   // acc_v1 register clock enable
  output                    mdctl_clken_acc_v1_1p;   // acc_v1 register clock enable
  output                    mdctl_clken_acc_v2_2p;   // acc_v2 register clock enable
  output                    mdctl_clken_acc_v2_1p;   // acc_v2 register clock enable

  // from ex datapath
  input                     ex_op1_msb_2p;           // MSB of dividend
  input                     ex_op2_msb_2p;           // MSB of divisor

  input [7:0]               md_mplr_2d;              // lower 8 bits of the multiplier
  input                     md_rem_sgn_1d;           // remainder sign
  input                     md_zero_rem_1d;          // remainder is zero
  input                     md_nxt_quot_1q;          // next quotient bit (lsb of remainder)
  input                     md_div_by_zero_1p;       // Divide By zero

  // from pipeline control
  input core_alu_func       req_fn;                  // New command request
  input core_alu_func       req_fn_early;            // New command request 1-cycle before start
  input                     req_valid_early;         // New Mul/Div 1-cycle before start
  input                     resp_ready;
  input minion_reg_dest     req_dest;                // response destination (during EX stage)
  input                     kill;
  input                     ex_dw_2p;                // 64b op

  // global signals
  input                     reset;
  input                     clock;



  //-----------------------------------------------------------------------------------------//
  //
  //-----------------------------------------------------------------------------------------//
  wire           start_mul_1p;                       // start_mul_1p new multiply
  wire           start_div_1p;                       // start_mul_1p new multiply
  wire           op1_sgnd_2p;                        // sign extend multiplicand/dividend
  wire           op2_sgnd_2p;                        // sign extend multiplier/divisor
  wire           id_new_mul;
  wire           id_new_div;


  // ID stage
  assign id_new_mul = req_valid_early &&  ((req_fn_early == core_alu_func_MUL)   ||
                                           (req_fn_early == core_alu_func_MULH)  ||
                                           (req_fn_early == core_alu_func_MULHU) ||
                                           (req_fn_early == core_alu_func_MULHSU));

  assign id_new_div = req_valid_early && !((req_fn_early == core_alu_func_MUL)   ||
                                           (req_fn_early == core_alu_func_MULH)  ||
                                           (req_fn_early == core_alu_func_MULHU) ||
                                           (req_fn_early == core_alu_func_MULHSU));

  rlatchn #(.WIDTH(1)) start_mul_1lt(.q(start_mul_1p), .d(id_new_mul), .clk(clock));
  rlatchn #(.WIDTH(1)) start_div_1lt(.q(start_div_1p), .d(id_new_div), .clk(clock));

  // ExStage
  assign op1_sgnd_2p = (req_fn == core_alu_func_DIV)  ||
                       (req_fn == core_alu_func_REM)  ||
                       (req_fn == core_alu_func_MULH) ||
                       (req_fn == core_alu_func_MULHSU);

  assign op2_sgnd_2p = (req_fn == core_alu_func_DIV) ||
                       (req_fn == core_alu_func_REM) ||
                       (req_fn == core_alu_func_MULH);

  //-----------------------------------------------------------------------------------------//
  // Loop Counters
  //-----------------------------------------------------------------------------------------//
  wire           start_mul_2p;
  wire           start_div_2p;
  wire           mdctl_start_div_2p;
  wire [6:0]     loop_cnt_2p;                        // Loop counter
  wire [6:0]     loop_cnt_1p;
  wire [6:0]     init_cnt_2p;                        // initial count
  wire           mul_op_1p;                          // multiply in progress
  wire           mul_op_2p;
  wire           mdctl_dw_2q;                        // double-word op held for duration
  wire           mdctl_dw_1q;                        // double-word op held for duration
  wire           early_out_2d;
  wire [4:0]     muldiv_fn_2q;                       // the function in progress

  rlatch #(.WIDTH(1)) start_mul_lt(.q(start_mul_2p), .d(start_mul_1p), .clk(clock));
  rlatch #(.WIDTH(1)) start_div_lt(.q(start_div_2p), .d(start_div_1p), .clk(clock));

  assign init_cnt_2p = start_div_2p ? (ex_dw_2p ? 7'd65 : 7'd33) :
                                      (ex_dw_2p ? 7'd8  : 7'd4);


  assign early_out_2d = kill ||
                        (muldiv_fn_2q == core_alu_func_MUL)                       && (loop_cnt_2p == 7'd2)/*  ||
                        (muldiv_fn_2q == core_alu_func_REM || core_alu_func_REMU) && (loop_cnt_2p == 7'd0)*/;

  rdncounter_m #(.WIDTH(7)) loop_cnt_i(.q_2p(loop_cnt_2p), .q_1p(loop_cnt_1p),
                                       .set_2p(start_mul_2p || start_div_2p),
                                       .init_2p(init_cnt_2p),
                                       .en_2p(|loop_cnt_2p),
                                       .rst_2p(reset || early_out_2d), .clk(clock));

  rregsce_sr_m #(.WIDTH(1)) dw_rg(.q(mdctl_dw_2q), .qm(mdctl_dw_1q),
                                  .d(ex_dw_2p), .en(start_div_2p || start_mul_2p),
                                  .rst(reset), .clk(clock));

  assign mdctl_start_div_2p = start_div_2p;

  // Destination Register
  en_ff #(.width(5))  muldiv_fn_rg(.Q(muldiv_fn_2q), .D(req_fn),
                                   .en(start_div_2p || start_mul_2p),
                                   .clock(clock));

  en_ff #(.width(7)) resp_dest_rg(.Q(resp_dest), .D(req_dest),
                                  .en(start_mul_2p || start_div_2p), .clock(clock));

  //--------------------------------------------------------------------------
  wire           md_op2_sgn_1q;
  wire           md_op1_sgn_2q;
  wire           md_op2_sgn_2q;
  wire           md_op1_sgn_2d;
  wire           md_op2_sgn_2d;

  wire           dvnd_sgn_2p;                        // dividend sign
  wire           dvsr_sgn_1p;                        // divisor sign
  wire           dvsr_sgn_2p;                        // divisor sign
  wire           dvnd_sgn_2d;                        // dividend sign
  wire           dvsr_sgn_2d;                        // divisor sign

  wire           mplr_sgn_1p;                        // multiplier sign

  rregs #(.WIDTH(1)) md_op1_sgn_rg(.q(md_op1_sgn_2q),
                                   .d(md_op1_sgn_2d), .clk(clock));

  rregs_m #(.WIDTH(1)) md_op2_sgn_rg(.q(md_op2_sgn_2q), .qm(md_op2_sgn_1q),
                                     .d(md_op2_sgn_2d), .clk(clock));

   //--------------------------------------------------------------------------
   // Multiplier Control
   //--------------------------------------------------------------------------
  wire [2:0]     mdctl_besl_0_2d;                    // booth encode select pp_0
  wire [2:0]     mdctl_besl_1_2d;                    // booth encode select pp_1
  wire [2:0]     mdctl_besl_2_2d;                    // booth encode select pp_2
  wire [2:0]     mdctl_besl_3_2d;                    // booth encode select pp_3
  wire           mplr_1p_7;                   // LSB for booth encoding
  wire           mul_lo_lsb_2p;               // LSB for booth encoding


  assign mdctl_besl_0_2d = benc({md_mplr_2d[1:0], !start_mul_2p && mul_lo_lsb_2p}) & {3{start_mul_2p || mul_op_2p}};
  assign mdctl_besl_1_2d = benc( md_mplr_2d[3:1]) & {3{start_mul_2p || mul_op_2p}};
  assign mdctl_besl_2_2d = benc( md_mplr_2d[5:3]) & {3{start_mul_2p || mul_op_2p}};
  assign mdctl_besl_3_2d = benc( md_mplr_2d[7:5]) & {3{start_mul_2p || mul_op_2p}};


  rlatchn #(.WIDTH(3)) mdctl_besl_0_lt(.q(mdctl_besl_0_1p), .d(~mdctl_besl_0_2d), .clk(clock));
  rlatchn #(.WIDTH(3)) mdctl_besl_1_lt(.q(mdctl_besl_1_1p), .d(~mdctl_besl_1_2d), .clk(clock));
  rlatchn #(.WIDTH(3)) mdctl_besl_2_lt(.q(mdctl_besl_2_1p), .d(~mdctl_besl_2_2d), .clk(clock));
  rlatchn #(.WIDTH(3)) mdctl_besl_3_lt(.q(mdctl_besl_3_1p), .d(~mdctl_besl_3_2d), .clk(clock));

  rregs_m #(.WIDTH(1))  mul_lo_lsb_rg(.q(mul_lo_lsb_2p), .qm(mplr_1p_7),
                                      .d(md_mplr_2d[7]), .clk(clock));

  rregs_m #(.WIDTH(1)) mul_op_rg(.q(mul_op_2p), .qm(mul_op_1p),
                                 .d(start_mul_2p || mul_op_2p && |loop_cnt_2p),
                                 .clk(clock));

  //-----------------------------------------------------------------------------------------//
  // Divide controls
  //-----------------------------------------------------------------------------------------//
  wire           div_op_1p;                          // divide in progress
  wire           div_op_2p;
  wire           rem_adj_ph_1p;                      // remainder adjust phase
  wire           rem_adj_ph_2p;
  wire           quot_adj_ph_1p;                     // quotient adjust phase
  wire           quot_adj_ph_2p;
  wire           zero_rem_1q;                        // zero remainder
  wire           zero_rem_2q;                        // zero remainder
  wire           div_by_zero_2p;                     // Divide By zero
  //wire         final_zero_rem_1q;                  // ri lint_check_waive NOT_READ
  wire           final_zero_rem_2q;                  // final remainder is zero
  wire           nxt_zero_rem_1d;                    // next zero remainder
  //wire         div_loop_1p;                        // in div loop
  //wire         div_loop_2p;
  wire           rq_rem_adj_2d;                      // successful remainder adjust
  wire           rq_rem_adj_1q;
  wire           rq_quot_cr_2d;                      // successful quotient correction
  wire           rq_quot_cr_1q;
  wire           rem_sgn_2p;                         // sign of remainder
  //wire           quot_se_1p;                       //
  //wire           quot_se_2p;


  rregs_m #(.WIDTH(1)) div_op_rg(.q(div_op_2p), .qm(div_op_1p),
                                 .d(start_div_2p ||
                                    !early_out_2d && div_op_2p && (|loop_cnt_2p || rem_adj_ph_2p || quot_adj_ph_2p)),
                                 .clk(clock));

  rregs_m #(.WIDTH(1)) rem_adj_ph_rg(.q(rem_adj_ph_2p), .qm(rem_adj_ph_1p),
                                     .d(!early_out_2d && div_op_2p && (loop_cnt_2p ==7'd1)),
                                     .clk(clock));

  rregs_m #(.WIDTH(1)) quot_adj_ph_rg(.q(quot_adj_ph_2p), .qm(quot_adj_ph_1p),
                                      .d(!early_out_2d && rem_adj_ph_2p), .clk(clock));


  //assign div_loop_1p = |loop_cnt_1p[6:1];
  //assign div_loop_2p = |loop_cnt_2p[6:1];

  assign nxt_zero_rem_1d =  (zero_rem_1q ||  md_zero_rem_1d) && !md_nxt_quot_1q;

  rregsnce_sr_m #(.WIDTH(1)) zero_rem_rg(.q(zero_rem_1q), .qm(zero_rem_2q),
                                         .d(nxt_zero_rem_1d), .en(|loop_cnt_1p[6:1]),
                                         .rst_2p(reset || start_div_2p || early_out_2d), .clk(clock));

  // ri lint_check_off OPEN_OUTPUT, LATCH
  rregsnce_sr_m #(.WIDTH(1)) final_zero_rem_rg(.q(), .qm(final_zero_rem_2q),
                                               .d(md_zero_rem_1d), .en(loop_cnt_1p == 7'd1),
                                               .rst_2p(reset || early_out_2d), .clk(clock));
  // ri lint_check_on OPEN_OUTPUT, LATCH

  rlatch #(.WIDTH(1)) rem_sgn_1lt(.q(rem_sgn_2p), .d(md_rem_sgn_1d), .clk(clock));

  // Successful remainder adjust (enable remainder flop to accept adjustment)
  assign rq_rem_adj_2d = !dvnd_sgn_2p &&  rem_sgn_2p ||
                         dvnd_sgn_2p && !rem_sgn_2p && !final_zero_rem_2q ||
                         dvnd_sgn_2p && zero_rem_2q && !final_zero_rem_2q;

  // Successful quotient adjust (enable quotient flop to accept adjustment)
  assign rq_quot_cr_2d =  dvsr_sgn_2p && !dvnd_sgn_2p ||
                          !dvsr_sgn_2p &&  dvnd_sgn_2p && !(zero_rem_2q || final_zero_rem_2q) ||
                          dvsr_sgn_2p &&  dvnd_sgn_2p &&  (zero_rem_2q || final_zero_rem_2q);

  rlatchn #(.WIDTH(1)) rq_rem_adj_2lt(.q(rq_rem_adj_1q), .d(rq_rem_adj_2d), .clk(clock));
  rlatchn #(.WIDTH(1)) rq_quot_cr_2lt(.q(rq_quot_cr_1q), .d(rq_quot_cr_2d), .clk(clock));

  rlatch #(.WIDTH(1)) div_by_zero_1lt(.q(div_by_zero_2p), .d(md_div_by_zero_1p), .clk(clock));

   //-----------------------------------------------------------------------------------------//
   // Sign bits
   //-----------------------------------------------------------------------------------------//

  assign md_op1_sgn_2d = (start_div_2p || start_mul_2p) ? ex_op1_msb_2p && op1_sgnd_2p :
                                                          (div_op_2p || mul_op_2p) && md_op1_sgn_2q;

  assign md_op2_sgn_2d = (start_div_2p || start_mul_2p) ? ex_op2_msb_2p && op2_sgnd_2p :
                                                          (div_op_2p || mul_op_2p) && md_op2_sgn_2q;

  assign dvnd_sgn_2p = md_op1_sgn_2q;
  assign dvsr_sgn_2p = md_op2_sgn_2q;
  assign dvnd_sgn_2d = md_op1_sgn_2d;
  assign dvsr_sgn_2d = md_op2_sgn_2d;

  assign dvsr_sgn_1p = md_op2_sgn_1q;
  //assign quot_se_1p  = md_op1_sgn_1q ^ md_op2_sgn_1q;
  //assign quot_se_2p  = md_op1_sgn_2q ^ md_op2_sgn_2q;

  assign mplr_sgn_1p  = md_op2_sgn_1q;

  // To Datapath
  assign mdctl_dvsr_sgn_1p = dvsr_sgn_1p;


  //-----------------------------------------------------------------------------------------//
  // Datapath controls
  //-----------------------------------------------------------------------------------------//
  wire              mdctl_clken_mpcnd_1d;            // Multiplicand clock enable
  wire              mdctl_clken_mul_ph1_2d;          // Multiplier clock enable
  wire              mdctl_clken_muldvsr_ph2_1d;      // Multiplier / Divisor clock enable
  wire              mdctl_clken_acc_v1_2p;           // acc_v1 register clock enable
  wire              mdctl_clken_acc_v1_1p;           // acc_v1 register clock enable
  wire              mdctl_clken_acc_v2_2p;           // acc_v2 register clock enable
  wire              mdctl_clken_acc_v2_1p;           // acc_v2 register clock enable
  wire              add_ain_sl_2d;                   // Adder A input select
  wire              mdctl_add_ain_sl_1p;
  wire [1:0]        add_bin_sl_2d;                   // Adder B input select
  wire [1:0]        mdctl_add_bin_sl_1p;
  wire              div_cin_2d;                      // Adder carry-in for divides
  wire              se_zero_2d;                      // Sign Extend (or Zero) A input
  wire              mdctl_se_zero_1p;
  wire              fnl_mul_cyc_1p;                  // final multiply cycle (sv + cv)
  wire              fnl_pp_1p;                       // select final partial product


  assign mdctl_start_op_2p = start_div_2p || start_mul_2p;

  assign mdctl_clken_mpcnd_1d = start_mul_1p;

  assign mdctl_clken_muldvsr_ph2_1d = start_div_1p || start_mul_1p || mul_op_1p;

  assign mdctl_clken_mul_ph1_2d = start_mul_2p || mul_op_2p && |loop_cnt_2p[6:1];

  // Zero out PPA's in first cycle of multiply (and during divides)
  rlatchn #(.WIDTH(1)) fc_zr_acc_lt(.q(mdctl_fc_zr_acc_1p), .d(!(start_mul_2p || div_op_2p)), .clk(clock));

  /*
   acc_v1_sl
   -> (3,2): final divide cycle, remainder  adjust phase,  final multiply
   -> 1: sum vector during multiply
   -> 0: next remainder in the divide loop
   */
  assign mdctl_acc_v1_sl_1d[1] = div_op_1p && loop_cnt_1p == 7'd1 ||
                                 rem_adj_ph_1p ||
                                 fnl_mul_cyc_1p;
  assign mdctl_acc_v1_sl_1d[0] = mul_op_1p && |loop_cnt_1p;

  // The acc_v1 latch-pair is Ph1 followed by Ph2
  assign mdctl_clken_acc_v1_2p = // decide in last divide cycle if remainder needs adjustment
                                 div_op_2p && (loop_cnt_2p ==7'd1) && rq_rem_adj_2d ||
                                 start_mul_2p || mul_op_2p && |loop_cnt_2p          ||
                                 start_div_2p || div_op_2p && |loop_cnt_2p[6:1];

  assign mdctl_clken_acc_v1_1p = rem_adj_ph_1p && rq_rem_adj_1q              || // requires remainder adjustment
                                 mul_op_1p && |loop_cnt_1p || fnl_mul_cyc_1p ||
                                 div_op_1p && |loop_cnt_1p;

  /*
   acc_v2_sl
   -> 3,2: quotient adjust
   -> 1: carry vector during multiply
   -> 0: divide loop, shift out dividend msb, shift in quotient
   */
  assign mdctl_acc_v2_sl_1d[1] = quot_adj_ph_1p;
  assign mdctl_acc_v2_sl_1d[0] = mul_op_1p && |loop_cnt_1p;

  // acc_v2 reg split in two latches Ph1 followed by Ph2
  // enable for the Ph1 latch clock
  assign mdctl_clken_acc_v2_2p = // decide in remainder adjust cycle if quotient needs correction
                                 rem_adj_ph_2p && rq_quot_cr_2d && !div_by_zero_2p ||
                                 start_mul_2p || mul_op_2p && |loop_cnt_2p       ||
                                 start_div_2p || div_op_2p && |loop_cnt_2p[6:1];

  // enable for the Ph2 latch clock
  assign mdctl_clken_acc_v2_1p = quot_adj_ph_1p && rq_quot_cr_1q  || // require quot correction
                                 start_div_1p    || // load remainder
                                 mul_op_1p && (|loop_cnt_1p[6:1] || loop_cnt_1p == 7'd1) ||
                                 div_op_1p && |loop_cnt_1p;

  // Adder input controls
  /*
   add_ain_sl
   -> 1: divide loop, remainder adjust or final cycle of multiply
   */
  assign add_ain_sl_2d = mul_op_2p && (loop_cnt_2p == 7'd1) ||
                         div_op_2p && (|loop_cnt_2p); // includes remainder adjust

  rlatchn #(.WIDTH(1)) add_ain_sl_2lt(.q(mdctl_add_ain_sl_1p), .d(add_ain_sl_2d), .clk(clock));

  /*
   add_bin_sl
   3: ~divisor - subtract divisor during divide loop
   2:  divisor - add divisor during divide loop and remainder adjust
   1:  quotient or acc for multiply final summation
   0:  zero - save power
   */

  assign add_bin_sl_2d[1] = // select divisor
                            start_div_2p ||
                            div_op_2p && (|loop_cnt_2p); // include remainder adjust

  assign add_bin_sl_2d[0] = // subtract divisor
                            start_div_2p                && (!dvnd_sgn_2d ^ dvsr_sgn_2d) ||
                            div_op_2p && (|loop_cnt_2p) && (!rem_sgn_2p  ^ dvsr_sgn_2d) ||
                            // quot correction or final cycle of multiply
                            rem_adj_ph_2p ||
                            mul_op_2p && (loop_cnt_2p == 7'd1);

  assign se_zero_2d = !add_ain_sl_2d && start_div_2p && dvnd_sgn_2d;

  rlatchn #(.WIDTH(1)) se_ze_2lt(.q(mdctl_se_zero_1p),.d(se_zero_2d), .clk(clock));

  rlatchn #(.WIDTH(2)) add_bin_sl_2lt(.q(mdctl_add_bin_sl_1p), .d(add_bin_sl_2d),  .clk(clock));

  /*
   divide carry in
   set to 1 if quotiend adjustment is required OR
   during divide loop and remainder adjust, for subtraction
   */
  assign div_cin_2d = rem_adj_ph_2p && rq_quot_cr_2d || // quotient correction
                      start_div_2p                && (!dvnd_sgn_2d ^ dvsr_sgn_2d) ||
                      div_op_2p && (|loop_cnt_2p) && (!rem_sgn_2p  ^ dvsr_sgn_2d);

  rlatchn #(.WIDTH(1)) div_cin_2lt(.q(mdctl_div_cin_1p), .d(div_cin_2d), .clk(clock));

  //-----------------------------------------------------------------------------------------//
  // Enable final partial product
  assign fnl_pp_1p = mul_op_1p && (loop_cnt_1p == 7'd1) && (mplr_1p_7 && !mplr_sgn_1p);


  assign fnl_mul_cyc_1p = mul_op_1p && (loop_cnt_1p == 7'd0);

  // To Datapath
  assign mdctl_fnl_pp_1p = fnl_pp_1p;

  rlatchn #(.WIDTH(1)) mdctl_fnl_mul_cyc_2lt(.q(mdctl_fnl_mul_cyc_1p), .d(mul_op_2p && (loop_cnt_2p == 7'd1)), .clk(clock));
  //assign mdctl_fnl_mul_cyc_1p = fnl_mul_cyc_1p;

  //-----------------------------------------------------------------------------------------//
  // Response
  wire [1:0]        mdctl_resp_sl_2p;     // response select
  wire              resp_valid_2q;

  assign resp_valid_early = mul_op_2p && ((muldiv_fn_2q == core_alu_func_MUL) ? (loop_cnt_2p == 7'd2) : (loop_cnt_2p == 7'd1)) ||
                            rem_adj_ph_2p || resp_valid_2q && !resp_ready;

  assign resp_valid = resp_valid_2q;

  assign req_ready = !(mul_op_2p || div_op_2p) && !resp_valid_2q;


  rst_ff #(.width(1)) resp_valid_rg(.Q(resp_valid_2q), .D(resp_valid_early),
                                    .reset(reset), .clock(clock));

  // 1: picks mulh, rem
  // 0: picks quot
/* -----\/----- EXCLUDED -----\/-----
  assign mdctl_resp_sl_2p[1] = (muldiv_fn_2q == core_alu_func_MULH)   ||
                               (muldiv_fn_2q == core_alu_func_MULHSU) ||
                               (muldiv_fn_2q == core_alu_func_MULHU)  ||
                               (muldiv_fn_2q == core_alu_func_REM)    ||
                               (muldiv_fn_2q == core_alu_func_REMU);

  assign mdctl_resp_sl_2p[0] = (muldiv_fn_2q == core_alu_func_DIV)    ||
                               (muldiv_fn_2q == core_alu_func_DIVU);
 -----/\----- EXCLUDED -----/\----- */
  // [1]: picks mull
  // [0]: picks quot
  assign mdctl_resp_sl_2p[1] = (muldiv_fn_2q == core_alu_func_MUL);
  assign mdctl_resp_sl_2p[0] = (muldiv_fn_2q == core_alu_func_DIV)    ||
                               (muldiv_fn_2q == core_alu_func_DIVU);



  //-----------------------------------------------------------------------------------------//
  function automatic [2:0] benc;
    input [2:0] inp;

    // +0, -1, +1, -2, +2
    // [2] -> complement
    // [1] -> 2X
    // [0] -> 1X
    begin
      benc = (inp == 3'b110) ? 3'b100 : // -1
             (inp == 3'b101) ? 3'b100 : // -1
             (inp == 3'b100) ? 3'b110 : // -2
             (inp == 3'b011) ? 3'b011 : // +2
             (inp == 3'b010) ? 3'b001 : // +1
             (inp == 3'b001) ? 3'b001 : // +1
             3'b000;  // +0
    end


  endfunction // benc


endmodule // intpipe_mul_div_ctl

/*
 Local Variables:
 verilog-library-directories:("." "/home/eng/jay/work/lib")
 verilog-library-files:("/home/eng/jay/work/lib/rtllib.v"  "/home/eng/jay/work/lib/cbufs.v")
 verilog-auto-inst-param-value:t
 verilog-indent-level:2
 verilog-indent-declaration:2
 verilog-indent-level-behavioral:2
 verilog-indent-level-module:2
 verilog-typedef-regexp: "\\(minion_\\|core_alu\\)"
 End:
 */
