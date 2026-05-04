// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Integer Multiplier / Divider
// Project       : Minion
//-----------------------------------------------------------------------------
// File          : intpipe_mul_div_dp.v
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

module intpipe_mul_div_dp (/*AUTOARG*/
  // Outputs
  md_resp_data_2p, md_mplr_2d, md_rem_sgn_1d, md_zero_rem_1d,
  md_nxt_quot_1q, md_div_by_zero_1p,
  // Inputs
  ex_op1_2p, ex_op2_2p, op1_sgnd_2p, op2_sgnd_2p, ex_dw_2p,
  mdctl_besl_0_1p, mdctl_besl_1_1p, mdctl_besl_2_1p, mdctl_besl_3_1p,
  mdctl_start_op_2p, mdctl_fnl_pp_1p, mdctl_dw_1q, mdctl_dw_2q,
  mdctl_acc_v1_sl_1d, mdctl_acc_v2_sl_1d, mdctl_add_ain_sl_1p,
  mdctl_add_bin_sl_1p, mdctl_se_zero_1p, mdctl_fnl_mul_cyc_1p,
  mdctl_start_div_2p, mdctl_fc_zr_acc_1p, mdctl_dvsr_sgn_1p,
  mdctl_div_cin_1p, mdctl_resp_sl_2p, mdctl_clken_mpcnd_1d,
  mdctl_clken_mul_ph1_2d, mdctl_clken_muldvsr_ph2_1d,
  mdctl_clken_acc_v1_2p, mdctl_clken_acc_v1_1p,
  mdctl_clken_acc_v2_2p, mdctl_clken_acc_v2_1p, clock, scan_shift_en
);


  output [`XREG_RANGE]  md_resp_data_2p;             // Multiply Hi result

  output [7:0]          md_mplr_2d;                  // multiplier bits for booth encoding
  output                md_rem_sgn_1d;               // remainder sign
  output                md_zero_rem_1d;              // remainder is zero
  output                md_nxt_quot_1q;              // next quotient bit (lsb of remainder)
  output                md_div_by_zero_1p;           // Divide By zero

  // From ALU
  input [`XREG_RANGE]   ex_op1_2p;                   // Multiplicand / Dividend
  input [`XREG_RANGE]   ex_op2_2p;                   // Multiplier / Divisor

  // From control
  input                 op1_sgnd_2p;                 // Signed Multiplicand / Dividend
  input                 op2_sgnd_2p;                 // Signed Multiplier / Divisor
  input                 ex_dw_2p;

  // From mul/div control
  input [2:0]           mdctl_besl_0_1p;             // booth encode select pp_0
  input [2:0]           mdctl_besl_1_1p;             // booth encode select pp_1
  input [2:0]           mdctl_besl_2_1p;             // booth encode select pp_2
  input [2:0]           mdctl_besl_3_1p;             // booth encode select pp_3
  input                 mdctl_start_op_2p;           // start multiply or divide
  input                 mdctl_fnl_pp_1p;             // select final partial product
  input                 mdctl_dw_1q;                 // sign extend result for 32b ops
  input                 mdctl_dw_2q;                 // sign extend result for 32b ops
  input [1:0]           mdctl_acc_v1_sl_1d;          // accumulator v1, remainder, mhi input select
  input [1:0]           mdctl_acc_v2_sl_1d;          // accumulator v2, quotient input select
  input                 mdctl_add_ain_sl_1p;         // select A-input of adder
  input [1:0]           mdctl_add_bin_sl_1p;         // select B-input of adder
  input                 mdctl_se_zero_1p;            // sign extend or zero Ain to adder
  input                 mdctl_fnl_mul_cyc_1p;        // final multiply cycle (sv + cv)
  input                 mdctl_start_div_2p;          // start new divide
  input                 mdctl_fc_zr_acc_1p;          // zero out acc vectors (first of multiply or divides)
  input                 mdctl_dvsr_sgn_1p;           // divisor sign
  input                 mdctl_div_cin_1p;            // divide carry in
  input [1:0]           mdctl_resp_sl_2p;            // response select

  input                 mdctl_clken_mpcnd_1d;        // Multiplicand clock enable
  input                 mdctl_clken_mul_ph1_2d;      // Multiplier clock enable
  input                 mdctl_clken_muldvsr_ph2_1d;  // Multiplier / Divisor clock enable
  input                 mdctl_clken_acc_v1_2p;       // acc_v1 register clock enable
  input                 mdctl_clken_acc_v1_1p;       // acc_v1 register clock enable
  input                 mdctl_clken_acc_v2_2p;       // acc_v2 register clock enable
  input                 mdctl_clken_acc_v2_1p;       // acc_v2 register clock enable

  input                 clock;
  input                 scan_shift_en;

  //--------------------------------------------------------------------------
  // acc_v1 & acc_v2_1p have the same weight
  wire                  clk_op_ph2;                  // ph2 clock for operands
  wire                  clk_muldvsr_ph2;             // ph2 clock for multiplier and divisor
  wire                  clk_mul_ph1;                 // ph1 clock for Mul lo
  wire                  clk_acc_v1_ph1;              // accumulator clock
  wire                  clk_acc_v1_ph2;              // accumulator clock
  wire                  clk_acc_v2_ph1;              // accumulator clock
  wire                  clk_acc_v2_ph2;              // accumulator clock

  wire [`XREG_SIZE+1:0] acc_v1_1d;                   // Accumulated vector-1
  wire [`XREG_SIZE+1:0] acc_v2_1d;                   // Accumulated vector-2

  wire                  fa_cout_1d;                  // carry out from the Full Adder
  wire                  fa_cout_2q;                  // carry out from the Full Adder
  wire                  fa_cout_1q;                  // carry out from the Full Adder

  wire [`XREG_SIZE+1:0] pp_0_1p;                     // Partial Product-0
  wire [`XREG_SIZE+1:0] pp_1_1p;                     // Partial Product-1
  wire [`XREG_SIZE+1:0] pp_2_1p;                     // Partial Product-2
  wire [`XREG_SIZE+1:0] pp_3_1p;                     // Partial Product-3
  wire [`XREG_RANGE]    pp_l_1p;                     // Final Adjustment

  wire [`XREG_SIZE+3:0] sv_0_0_1p;                   // Level-0 sum vector from 4-2 compressor
  wire [`XREG_SIZE+3:0] cv_0_0_1p;                   // Level-0 carry vector from 4-2 compressor
  wire [`XREG_SIZE+3:0] sv_0_1_1p;                   // Level-0 sum vector from 3-2 compressor
  wire [`XREG_SIZE+3:0] cv_0_1_1p;                   // Level-0 carry vector from 3-2 compressor

  wire [`XREG_SIZE+6:0] sv_1_0_1p;                   // Level-1 sum vector from 4-2 compressor
  wire [`XREG_SIZE+6:0] cv_1_0_1p;                   // Level-1 carry vector from 4-2 compressor

  wire [7:0]            l2_mul_res_1p;               // Level-2
  wire                  l2_mul_co_1d;


  /*
   According to the RISCV spec, 32b operands must already be appropriately sign extended
  */
  //-----------------------------------------------------------------------------------//
  // Multiplier, Divisor latches
  //-----------------------------------------------------------------------------------//
  wire [`XREG_SIZE:0]   mpcnd_1p;                    // Multiplicand (held for duration of multiply)
  wire [`XREG_SIZE:0]   mul_lo_dvsr_1p;              // Ph2 latched mul_lo OR divisor
  wire [`XREG_RANGE]    mul_lo_2p;                   // Ph1 latched multiplier Lo result
  wire [`XREG_SIZE:0]   mul_lo_dvsr_2d;              // next mul_lo_dvsr
  wire [`XREG_RANGE]    mul_lo_1d;                   // next mul_lo
  wire                  md_div_by_zero_1p;           // Divide By zero
  wire [`XREG_SIZE:0]   mpcnd_1d;
  wire [`XREG_SIZE:0]   dvsr_1d;


  // Multiplicand, Dividend
  assign mpcnd_1d = ex_dw_2p ? {    op1_sgnd_2p && ex_op1_2p[63],   ex_op1_2p} :
                               {{33{op1_sgnd_2p && ex_op1_2p[31]}}, ex_op1_2p[31:0]};


  rlatchn #(65) mpcnd_se_rg(.q(mpcnd_1p), .d(mpcnd_1d), .clk(clk_op_ph2));


  // Multiplier, Divisor
  assign dvsr_1d = ex_dw_2p ? {    op2_sgnd_2p && ex_op2_2p[63],   ex_op2_2p} :
                              {{33{op2_sgnd_2p && ex_op2_2p[31]}}, ex_op2_2p[31:0]};

  assign mul_lo_dvsr_2d = mdctl_start_op_2p ? dvsr_1d : {1'b0, mul_lo_2p};

  rlatchn #(65)  mul_lo_dvsr_1lt(.q(mul_lo_dvsr_1p), .d(mul_lo_dvsr_2d), .clk(clk_muldvsr_ph2));

  assign md_div_by_zero_1p  = mul_lo_dvsr_1p[63:0] == 64'b0;

  // only for multiplies, shift in the 8b of result while shifting out the multiplier bits
  assign mul_lo_1d = mdctl_dw_1q ? {l2_mul_res_1p, mul_lo_dvsr_1p[63:8]}:
                                   {32'b0, l2_mul_res_1p, mul_lo_dvsr_1p[31:8]};

  rlatch #(64)  mplr_lo_1lt(.q(mul_lo_2p), .d(mul_lo_1d), .clk(clk_mul_ph1));

  assign md_mplr_2d = mul_lo_dvsr_2d[7:0];
  // Final multiply lo result is in mul_lo_dvsr_1p
  //-----------------------------------------------------------------------------------//
  // Multiplier
  //-----------------------------------------------------------------------------------//
  /*
   X = m  ? s  : a
   Y = m1 ? s : a
   Z = x2 ? Y  : X

   */

/* -----\/----- EXCLUDED -----\/-----
  assign pp_0_1p = mdctl_besl_0_1p[2] ? ~( mdctl_besl_0_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_0_1p[0]}}} ) :
                                         ( mdctl_besl_0_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_0_1p[0]}}} );

  assign pp_1_1p = mdctl_besl_1_1p[2] ? ~( mdctl_besl_1_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_1_1p[0]}}} ) :
                                         ( mdctl_besl_1_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_1_1p[0]}}} );

  assign pp_2_1p = mdctl_besl_2_1p[2] ? ~( mdctl_besl_2_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_2_1p[0]}}} ) :
                                         ( mdctl_besl_2_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_2_1p[0]}}} );

  assign pp_3_1p = mdctl_besl_3_1p[2] ? ~( mdctl_besl_3_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_3_1p[0]}}} ) :
                                         ( mdctl_besl_3_1p[1] ? {mpcnd_1p, 1'b0} :
                                                                {{mpcnd_1p[64], mpcnd_1p} & {66{mdctl_besl_3_1p[0]}}} );
 -----/\----- EXCLUDED -----/\----- */

  // ri lint_check_off SIGN_EXT
  assign pp_0_1p = !mdctl_besl_0_1p[1] ? ( {66{!mdctl_besl_0_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_0_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                         ( {66{!mdctl_besl_0_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_0_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_1_1p = !mdctl_besl_1_1p[1] ? ( {66{!mdctl_besl_1_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_1_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                         ( {66{!mdctl_besl_1_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_1_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_2_1p = !mdctl_besl_2_1p[1] ? ( {66{!mdctl_besl_2_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_2_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                         ( {66{!mdctl_besl_2_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_2_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_3_1p = !mdctl_besl_3_1p[1] ? ( {66{!mdctl_besl_3_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_3_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                         ( {66{!mdctl_besl_3_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_3_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_l_1p = mdctl_fnl_pp_1p ? mpcnd_1p[`XREG_RANGE] : 64'b0;


  // Level-0 has a 4-2 and a 3-2 compressor
  r42cmp #(.WIDTH(68)) l0_42_i(.sv(sv_0_0_1p), .cv(cv_0_0_1p),
                               .a({ {2{acc_v1_1d[65]}}, acc_v1_1d    }),
                               .b({ {2{acc_v2_1d[65]}}, acc_v2_1d    }),
                               .c({ {2{pp_0_1p[65]}},   pp_0_1p      }),
                               .d({  pp_1_1p, 1'b0,     !mdctl_besl_0_1p[2] }),
                               .cin(fa_cout_1d));

  r32cmp #(.WIDTH(68)) l0_32_i(.sv(sv_0_1_1p), .cv(cv_0_1_1p),
                               .a({ {2{pp_2_1p[65]}}, pp_2_1p }),
                               .b({  pp_3_1p, 1'b0,   !mdctl_besl_2_1p[2]}),
                               .c({  pp_l_1p, 1'b0,   !mdctl_besl_3_1p[2], 2'b0}));

  // Level-1 has 4-2  compressor
  r42cmp #(.WIDTH(71)) l1_42_i(.sv(sv_1_0_1p), .cv(cv_1_0_1p),
                               .a({ {5{sv_0_0_1p[67]}}, sv_0_0_1p[67:2] }),
                               .b({ {4{cv_0_0_1p[67]}}, cv_0_0_1p[67:1] }),
                               .c({ {1{sv_0_1_1p[67]}}, sv_0_1_1p, 1'b0, !mdctl_besl_1_1p[2] }),
                               .d({  cv_0_1_1p, 3'b0 }),
                               .cin(1'b0));
  // ri lint_check_on SIGN_EXT

  // Level-2 has a 6-b full adder that generates a  carry-out for the next iteration's fa_cout_1q
  // while the remaining 64 bits of result are held in a sv,cv register.
  assign                l2_mul_res_1p[1:0]  = {sv_0_0_1p[1] ^ cv_0_0_1p[0], sv_0_0_1p[0]};
  assign {l2_mul_co_1d, l2_mul_res_1p[7:2]} =  sv_1_0_1p[5:0] + {cv_1_0_1p[4:0], sv_0_0_1p[1] && cv_0_0_1p[0]};

  // Setup for next multiply iteration
  wire [`XREG_SIZE+1:0]           nxt_acc_v1_1p;     // next accumulated vector-1
  wire [`XREG_SIZE+1:0]           nxt_acc_v2_1p;     // next accumulated vector-2
  wire                  nxt_fa_cout_1p;              // next carry-in from previous iteration

  assign nxt_acc_v1_1p  = {sv_1_0_1p[70], sv_1_0_1p[70:6]};
  assign nxt_acc_v2_1p  =                 cv_1_0_1p[70:5] ;
  assign nxt_fa_cout_1p = l2_mul_co_1d;

  //---------------------------------------------------------------------------------------
  // For Divide operations, the remainder is held in acc_v1_1q while the dividend, quotient
  // is  in acc_v2_1q.
  // For Multiply the final result is in acc_v1_1q
  wire [`XREG_SIZE+1:0]           acc_v1_1q;         // accumulated v1 vector to PPA
  wire [`XREG_SIZE+1:0]           acc_v1_2p;         // accumulated v1 vector to PPA
  wire [`XREG_SIZE+1:0]           acc_v2_1q;         // accumulated v2 vector to PPA
  wire [`XREG_SIZE+1:0]           acc_v2_2d;
  wire [`XREG_SIZE+1:0]           acc_v2_2q;

  wire [`XREG_SIZE+1:0]           nxt_acc_v1_1d;     // next acc_v1
  wire [`XREG_SIZE+1:0]           nxt_acc_v2_1d;     // next acc_v2


  wire [`XREG_SIZE:0]           sum_1d;
  wire                          md_nxt_quot_1q;     // next qutient bit in divide loop
  wire                          cout_1d;            // carry out of adder

  assign md_nxt_quot_1q = mdctl_dw_1q ? acc_v2_1q[63] : acc_v2_1q[31];

  assign nxt_acc_v1_1d = (mdctl_acc_v1_sl_1d[1] == 1'b1) ? {1'b0, sum_1d} :  // remainder adjust, final multiply
                         (mdctl_acc_v1_sl_1d == 2'b01)   ? nxt_acc_v1_1p  :  // multiply loop as accumulator
                                                           {sum_1d, md_nxt_quot_1q}; // divide loop

  assign nxt_acc_v2_1d = (mdctl_acc_v2_sl_1d[1] == 1'b1) ? {1'b0, sum_1d} : // quotient adjust
                         (mdctl_acc_v2_sl_1d == 2'b01)   ? nxt_acc_v2_1p  : // multiply loop as accumulator
                                                           {acc_v2_1q[`XREG_SIZE:0], cout_1d ^ mdctl_dvsr_sgn_1p || md_div_by_zero_1p}; // divide loop

  //rregsn_m #(.WIDTH(66)) acc_v1_rg(.q(acc_v1_1q), .qm(acc_v1_2p), .d(nxt_acc_v1_1d),  .clk(clk_acc_v1_ph2));
  // SV, REM register
  rlatch  #(.WIDTH(66)) acc_v1_1lt(.q(acc_v1_2p), .d(nxt_acc_v1_1d),  .clk(clk_acc_v1_ph1));
  rlatchn #(.WIDTH(66)) acc_v1_2lt(.q(acc_v1_1q), .d(acc_v1_2p),      .clk(clk_acc_v1_ph2));

  // CV, QUOT register
  assign  acc_v2_2d = mdctl_start_div_2p ? {1'b0, mpcnd_1d} : acc_v2_2q;

  rlatch  #(.WIDTH(66)) acc_v2_1lt(.q(acc_v2_2q), .d(nxt_acc_v2_1d),  .clk(clk_acc_v2_ph1));
  rlatchn #(.WIDTH(66)) acc_v2_2lt(.q(acc_v2_1q), .d(acc_v2_2d),      .clk(clk_acc_v2_ph2));


  //rregsn #(.WIDTH(1)) fa_cout_rg(.q(fa_cout_1q),  .d(nxt_fa_cout_1p), .clk(clk_acc_v2_ph2));
  rlatch  #(.WIDTH(1)) fa_cout_1lt(.q(fa_cout_2q),  .d(nxt_fa_cout_1p), .clk(clk_acc_v2_ph1));
  rlatchn #(.WIDTH(1)) fa_cout_2lt(.q(fa_cout_1q),  .d(fa_cout_2q),     .clk(clk_acc_v2_ph2));

  // ri lint_check_off FULL_RANGE
  assign md_zero_rem_1d = sum_1d[`XREG_SIZE:0] == 65'd0; // late. If needed can be moved to next cycle
  // ri lint_check_on FULL_RANGE


  assign acc_v1_1d  = acc_v1_1q  & {66{mdctl_fc_zr_acc_1p}};
  assign acc_v2_1d  = acc_v2_1q  & {66{mdctl_fc_zr_acc_1p}};
  assign fa_cout_1d = fa_cout_1q && mdctl_fc_zr_acc_1p;


  //------------------------------------------------------------------------------
  // Adder and its inputs
  //------------------------------------------------------------------------------
  wire [`XREG_SIZE:0]           ain_1p;              // Adder A input
  wire [`XREG_SIZE:0]           bin_1p;              // Adder B input
  wire                  cin_1p;                      // Adder Cin input

/* -----\/----- EXCLUDED -----\/-----
  assign ain_1p = (mdctl_add_ain_sl_1p == 1'b1) ? acc_v1_1q[`XREG_SIZE:0] :
                                                  {65{mdctl_se_zero_1p}}; // zero to conserve power
 -----/\----- EXCLUDED -----/\----- */
  assign ain_1p = {65{mdctl_add_ain_sl_1p}} & acc_v1_1q[`XREG_SIZE:0] | {65{mdctl_se_zero_1p}};


  // For divides, select divisor or ~divisor
  assign bin_1p = (mdctl_add_bin_sl_1p == 2'b11) ? ~mul_lo_dvsr_1p :
                  (mdctl_add_bin_sl_1p == 2'b10) ?  mul_lo_dvsr_1p :
                  (mdctl_add_bin_sl_1p == 2'b01) ? acc_v2_1q[`XREG_SIZE:0]      :
                                                   65'd0; // zero during multiply loop to conserve power (or quot correction??)

  assign cin_1p = mdctl_fnl_mul_cyc_1p ? fa_cout_1q : mdctl_div_cin_1p;

  assign {cout_1d, sum_1d} = ain_1p + bin_1p + {64'b0, cin_1p};

  assign md_rem_sgn_1d = sum_1d[64];

  //------------------------------------------------------------------------------
  // Named outputs, replace with 'rd'
  //------------------------------------------------------------------------------
  // Debug ouputs
  //assign mul_hi_1q = 1'b1        ? acc_v1_1q[`XREG_RANGE]      : {{32{acc_v1_1q[31]}}, acc_v1_1q[31:0]};
  //assign mul_lo_1q = mdctl_dw_1q ? mul_lo_dvsr_1p[`XREG_RANGE] : {{32{mul_lo_dvsr_1p[31]}}, mul_lo_dvsr_1p[31:0]};
  //assign rem_1q    = mdctl_dw_1q ? acc_v1_1q[`XREG_RANGE]      : {{32{mdctl_dvnd_sgn_1p && acc_v1_1q[31]}}, acc_v1_1q[31:0]};
  //assign quot_1q   = mdctl_dw_1q ? acc_v2_1q[`XREG_RANGE]      : {{32{mdctl_quot_se_1p  && acc_v2_1q[31]}}, acc_v2_1q[31:0]};;

  // response destination
  wire [`XREG_RANGE]    resp_data_2d;

  // ri lint_check_off FULL_RANGE
  assign resp_data_2d = mdctl_resp_sl_2p[1] ? mul_lo_2p[`XREG_RANGE] :
                        mdctl_resp_sl_2p[0] ? acc_v2_2q[`XREG_RANGE] :
                                              acc_v1_2p[`XREG_RANGE];

  assign md_resp_data_2p   = mdctl_dw_2q ? resp_data_2d :
                                           {{`WREG_SIZE{resp_data_2d[`WREG_MSB]}}, resp_data_2d[`WREG_RANGE]};

  // ri lint_check_on FULL_RANGE

  //------------------------------------------------------------------------------
  // Buffers & Clocks
  //------------------------------------------------------------------------------

  et_clk_gate_n cbuf_mpcnd_ph2   ( .enclk_n ( clk_op_ph2      ), .en_1p ( mdctl_clken_mpcnd_1d       ), .clk ( clock ), .te ( 1'b0 ));
  et_clk_gate_n cbuf_muldvsr_ph2 ( .enclk_n ( clk_muldvsr_ph2 ), .en_1p ( mdctl_clken_muldvsr_ph2_1d ), .clk ( clock ), .te ( 1'b0 ));
  et_clk_gate   cbuf_mul_ph1     ( .enclk   ( clk_mul_ph1     ), .en    ( mdctl_clken_mul_ph1_2d     ), .clk ( clock ), .te ( 1'b0 ));

  // SV, REM reg
  et_clk_gate   cbuf_acc_v1_ph1  ( .enclk   ( clk_acc_v1_ph1  ), .en    ( mdctl_clken_acc_v1_2p      ), .clk ( clock ), .te ( 1'b0 ));
  et_clk_gate_n cbuf_acc_v1_ph2  ( .enclk_n ( clk_acc_v1_ph2  ), .en_1p ( mdctl_clken_acc_v1_1p      ), .clk ( clock ), .te ( 1'b0 ));

  // CV, Quot reg
  et_clk_gate   cbuf_acc_v2_ph1  ( .enclk   ( clk_acc_v2_ph1  ), .en    ( mdctl_clken_acc_v2_2p      ), .clk ( clock ), .te ( 1'b0 ));
  et_clk_gate_n cbuf_acc_v2_ph2  ( .enclk_n ( clk_acc_v2_ph2  ), .en_1p ( mdctl_clken_acc_v2_1p      ), .clk ( clock ), .te ( 1'b0 ));


endmodule // intpipe_mul_div_dp
