// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_mul_div_ctl -- Multiplier / Divider Control.
//
// Booth-encoder, loop counters, divide state machine, clock enables,
// adder-input mux selects, and response steering.
//
// Latch-based dual-phase pipeline preserved 1:1 from the original.
//
// Line-by-line translation of: intpipe_mul_div_ctl (etcore-soc)

// Latch-based design has intentional combinational loops through the
// counter and state-machine paths.  Suppress Verilator's UNOPTFLAT.
/* verilator lint_off UNOPTFLAT */

module intpipe_mul_div_ctl
  import minion_pkg::*;
(
  // To bypass, stall  control
  output minion_reg_dest_t      resp_dest,               // response destination
  output logic                  req_ready,               // Not Busy
  output logic                  resp_valid,              // Multiply, Divide Result valid
  output logic                  resp_valid_early,
  output logic                  debug_start_mul_1p_o,
  output logic                  debug_start_mul_2p_o,
  output logic                  debug_mul_op_1p_o,
  output logic                  debug_mul_op_2p_o,
  output logic [6:0]            debug_loop_cnt_1p_o,
  output logic [6:0]            debug_loop_cnt_2p_o,

  // to mul/div datapath
  output logic                  op1_sgnd_2p,             // sign extend multiplicand/dividend
  output logic                  op2_sgnd_2p,             // sign extend multiplier/divisor

  output logic [2:0]            mdctl_besl_0_1p,         // booth encode select pp_0
  output logic [2:0]            mdctl_besl_1_1p,         // booth encode select pp_1
  output logic [2:0]            mdctl_besl_2_1p,         // booth encode select pp_2
  output logic [2:0]            mdctl_besl_3_1p,         // booth encode select pp_3
  output logic                  mdctl_start_op_2p,       // start multiply or divide
  output logic                  mdctl_fnl_pp_1p,         // select final partial product
  output logic                  mdctl_dw_1q,             // sign extend result for 32b ops
  output logic                  mdctl_dw_2q,             // sign extend result for 32b ops
  output logic [1:0]            mdctl_acc_v1_sl_1d,      // accumulator v1, remainder, mhi input select
  output logic [1:0]            mdctl_acc_v2_sl_1d,      // accumulator v2, quotient input select
  output logic                  mdctl_add_ain_sl_1p,     // select A-input of adder
  output logic [1:0]            mdctl_add_bin_sl_1p,     // select B-input of adder
  output logic                  mdctl_se_zero_1p,        // sign extend or zero Ain to adder
  output logic                  mdctl_fnl_mul_cyc_1p,    // final multiply cycle (sv + cv)
  output logic                  mdctl_start_div_2p,      // start new divide
  output logic                  mdctl_fc_zr_acc_1p,      // zero out acc vectors (first of multiply or divides)
  output logic                  mdctl_dvsr_sgn_1p,       // divisor sign
  output logic                  mdctl_div_cin_1p,        // divide carry in
  output logic [1:0]            mdctl_resp_sl_2p,        // response select

  output logic                  mdctl_clken_mpcnd_1d,    // Multiplicand clock enable
  output logic                  mdctl_clken_mul_ph1_2d,  // Multiplier clock enable
  output logic                  mdctl_clken_muldvsr_ph2_1d, // Multiplier / Divisor clock enable
  output logic                  mdctl_clken_acc_v1_2p,   // acc_v1 register clock enable
  output logic                  mdctl_clken_acc_v1_1p,   // acc_v1 register clock enable
  output logic                  mdctl_clken_acc_v2_2p,   // acc_v2 register clock enable
  output logic                  mdctl_clken_acc_v2_1p,   // acc_v2 register clock enable

  // from ex datapath
  input  logic                  ex_op1_msb_2p,           // MSB of dividend
  input  logic                  ex_op2_msb_2p,           // MSB of divisor

  input  logic [7:0]            md_mplr_2d,              // lower 8 bits of the multiplier
  input  logic                  md_rem_sgn_1d,           // remainder sign
  input  logic                  md_zero_rem_1d,          // remainder is zero
  input  logic                  md_nxt_quot_1q,          // next quotient bit (lsb of remainder)
  input  logic                  md_div_by_zero_1p,       // Divide By zero

  // from pipeline control
  input  alu_func_e             req_fn,                  // New command request
  input  alu_func_e             req_fn_early,            // New command request 1-cycle before start
  input  logic                  req_valid_early,         // New Mul/Div 1-cycle before start
  input  logic                  resp_ready,
  input  minion_reg_dest_t      req_dest,                // response destination (during EX stage)
  input  logic                  kill,
  input  logic                  ex_dw_2p,                // 64b op

  // global signals
  input  logic                  reset,
  input  logic                  clock
);

  //-----------------------------------------------------------------------------------------//
  //
  //-----------------------------------------------------------------------------------------//
  logic           start_mul_1p;                       // start_mul_1p new multiply
  logic           start_div_1p;                       // start new divide
  logic           id_new_mul;
  logic           id_new_div;


  // ID stage
  assign id_new_mul = req_valid_early &&  ((req_fn_early == AluMul)   ||
                                           (req_fn_early == AluMulh)  ||
                                           (req_fn_early == AluMulhu) ||
                                           (req_fn_early == AluMulhsu));

  assign id_new_div = req_valid_early && !((req_fn_early == AluMul)   ||
                                           (req_fn_early == AluMulh)  ||
                                           (req_fn_early == AluMulhu) ||
                                           (req_fn_early == AluMulhsu));

  prim_phase_pair_lo_hi u_start_mul_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (id_new_mul),
    .q_lo_o  (start_mul_1p),
    .q_hi_o  (start_mul_2p)
  );

  prim_phase_pair_lo_hi u_start_div_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (id_new_div),
    .q_lo_o  (start_div_1p),
    .q_hi_o  (start_div_2p)
  );

  // ExStage
  assign op1_sgnd_2p = (req_fn == AluDiv)  ||
                       (req_fn == AluRem)  ||
                       (req_fn == AluMulh) ||
                       (req_fn == AluMulhsu);

  assign op2_sgnd_2p = (req_fn == AluDiv) ||
                       (req_fn == AluRem) ||
                       (req_fn == AluMulh);

  //-----------------------------------------------------------------------------------------//
  // Loop Counters
  //-----------------------------------------------------------------------------------------//
  logic           start_mul_2p;
  logic           start_div_2p;
  logic [6:0]     loop_cnt_2p;                        // Loop counter
  logic [6:0]     loop_cnt_1p;
  logic [6:0]     init_cnt_2p;                        // initial count
  logic           mul_op_1p;                          // multiply in progress
  logic           mul_op_2p;
  logic           early_out_2d;
  logic [4:0]     muldiv_fn_2q;                       // the function in progress

  assign init_cnt_2p = start_div_2p ? (ex_dw_2p ? 7'd65 : 7'd33) :
                                      (ex_dw_2p ? 7'd8  : 7'd4);


  assign early_out_2d = kill ||
                        (muldiv_fn_2q == AluMul) && (loop_cnt_2p == 7'd2);

  prim_phase_pair_lo_hi #(
    .Width (7)
  ) u_loop_cnt_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     ((reset || early_out_2d) ? 7'b0 :
              (start_mul_2p || start_div_2p) ? init_cnt_2p :
              (|loop_cnt_2p) ? loop_cnt_2p - 7'd1 :
                               loop_cnt_2p),
    .q_lo_o  (loop_cnt_1p),
    .q_hi_o  (loop_cnt_2p)
  );

  prim_phase_pair_lo_hi u_mdctl_dw_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (reset ? 1'b0 : ((start_div_2p || start_mul_2p) ? ex_dw_2p : mdctl_dw_2q)),
    .q_lo_o  (mdctl_dw_1q),
    .q_hi_o  (mdctl_dw_2q)
  );

  assign mdctl_start_div_2p = start_div_2p;

  // Destination Register
  // en_ff #(.width(5)) muldiv_fn_rg(...)
  always_ff @(posedge clock)
    if (start_div_2p || start_mul_2p)
      muldiv_fn_2q <= req_fn;

  // en_ff #(.width(7)) resp_dest_rg(...)
  always_ff @(posedge clock)
    if (start_mul_2p || start_div_2p)
      resp_dest <= req_dest;

  //--------------------------------------------------------------------------
  logic           md_op2_sgn_1q;
  logic           md_op1_sgn_2q;
  logic           md_op2_sgn_2q;
  logic           md_op1_sgn_2d;
  logic           md_op2_sgn_2d;

  logic           dvnd_sgn_2p;                        // dividend sign
  logic           dvsr_sgn_1p;                        // divisor sign
  logic           dvsr_sgn_2p;                        // divisor sign
  logic           dvnd_sgn_2d;                        // dividend sign
  logic           dvsr_sgn_2d;                        // divisor sign

  logic           mplr_sgn_1p;                        // multiplier sign

  // rregs #(.WIDTH(1)) md_op1_sgn_rg(...)
  always_ff @(posedge clock)
    md_op1_sgn_2q <= md_op1_sgn_2d;

  prim_phase_pair_lo_hi u_md_op2_sgn_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (md_op2_sgn_2d),
    .q_lo_o  (md_op2_sgn_1q),
    .q_hi_o  (md_op2_sgn_2q)
  );

  //--------------------------------------------------------------------------
  // Multiplier Control
  //--------------------------------------------------------------------------
  logic [2:0]     mdctl_besl_0_2d;                    // booth encode select pp_0
  logic [2:0]     mdctl_besl_1_2d;                    // booth encode select pp_1
  logic [2:0]     mdctl_besl_2_2d;                    // booth encode select pp_2
  logic [2:0]     mdctl_besl_3_2d;                    // booth encode select pp_3
  logic           mplr_1p_7;                          // LSB for booth encoding
  logic           mul_lo_lsb_2p;                      // LSB for booth encoding


  assign mdctl_besl_0_2d = benc({md_mplr_2d[1:0], !start_mul_2p && mul_lo_lsb_2p}) & {3{start_mul_2p || mul_op_2p}};
  assign mdctl_besl_1_2d = benc( md_mplr_2d[3:1]) & {3{start_mul_2p || mul_op_2p}};
  assign mdctl_besl_2_2d = benc( md_mplr_2d[5:3]) & {3{start_mul_2p || mul_op_2p}};
  assign mdctl_besl_3_2d = benc( md_mplr_2d[7:5]) & {3{start_mul_2p || mul_op_2p}};


  // Preserve the original low-phase booth-select latches locally instead of
  // depending on raw leaf latch primitives.
  always_latch begin
    if (!clock) mdctl_besl_0_1p = ~mdctl_besl_0_2d;
  end

  always_latch begin
    if (!clock) mdctl_besl_1_1p = ~mdctl_besl_1_2d;
  end

  always_latch begin
    if (!clock) mdctl_besl_2_1p = ~mdctl_besl_2_2d;
  end

  always_latch begin
    if (!clock) mdctl_besl_3_1p = ~mdctl_besl_3_2d;
  end

  prim_phase_pair_lo_hi u_mul_lo_lsb_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (md_mplr_2d[7]),
    .q_lo_o  (mplr_1p_7),
    .q_hi_o  (mul_lo_lsb_2p)
  );

  prim_phase_pair_lo_hi u_mul_op_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (start_mul_2p || mul_op_2p && |loop_cnt_2p),
    .q_lo_o  (mul_op_1p),
    .q_hi_o  (mul_op_2p)
  );

  //-----------------------------------------------------------------------------------------//
  // Divide controls
  //-----------------------------------------------------------------------------------------//
  logic           div_op_1p;                          // divide in progress
  logic           div_op_2p;
  logic           rem_adj_ph_1p;                      // remainder adjust phase
  logic           rem_adj_ph_2p;
  logic           quot_adj_ph_1p;                     // quotient adjust phase
  logic           quot_adj_ph_2p;
  logic           zero_rem_1q;                        // zero remainder
  logic           zero_rem_2q;                        // zero remainder
  logic           div_by_zero_2p;                     // Divide By zero
  logic           final_zero_rem_2q;                  // final remainder is zero
  logic           nxt_zero_rem_1d;                    // next zero remainder
  logic           rq_rem_adj_2d;                      // successful remainder adjust
  logic           rq_rem_adj_1q;
  logic           rq_quot_cr_2d;                      // successful quotient correction
  logic           rq_quot_cr_1q;
  logic           rem_sgn_2p;                         // sign of remainder


  prim_phase_pair_lo_hi u_div_op_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (start_div_2p ||
              !early_out_2d && div_op_2p && (|loop_cnt_2p || rem_adj_ph_2p || quot_adj_ph_2p)),
    .q_lo_o  (div_op_1p),
    .q_hi_o  (div_op_2p)
  );

  prim_phase_pair_lo_hi u_rem_adj_ph_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (!early_out_2d && div_op_2p && (loop_cnt_2p == 7'd1)),
    .q_lo_o  (rem_adj_ph_1p),
    .q_hi_o  (rem_adj_ph_2p)
  );

  prim_phase_pair_lo_hi u_quot_adj_ph_pair (
    .clk_i   (clock),
    .lo_en_i (1'b1),
    .hi_en_i (1'b1),
    .d_i     (!early_out_2d && rem_adj_ph_2p),
    .q_lo_o  (quot_adj_ph_1p),
    .q_hi_o  (quot_adj_ph_2p)
  );


  assign nxt_zero_rem_1d =  (zero_rem_1q ||  md_zero_rem_1d) && !md_nxt_quot_1q;

  // rregsnce_sr_m #(.WIDTH(1)) zero_rem_rg(...)
  // rlatch: qm <= en ? d : q
  always_latch begin
    if (clock) zero_rem_2q = (|loop_cnt_1p[6:1]) ? nxt_zero_rem_1d : zero_rem_1q;
  end
  // rlatchn: q <= rst ? 0 : qm
  always_latch begin
    if (!clock) zero_rem_1q = (reset || start_div_2p || early_out_2d) ? 1'b0 : zero_rem_2q;
  end

  // rregsnce_sr_m #(.WIDTH(1)) final_zero_rem_rg(...)
  // q output is unconnected in the original but feeds back internally
  logic final_zero_rem_q_internal;
  // rlatch: qm <= en ? d : q
  always_latch begin
    if (clock) final_zero_rem_2q = (loop_cnt_1p == 7'd1) ? md_zero_rem_1d : final_zero_rem_q_internal;
  end
  // rlatchn: q <= rst ? 0 : qm
  always_latch begin
    if (!clock) final_zero_rem_q_internal = (reset || early_out_2d) ? 1'b0 : final_zero_rem_2q;
  end

  // rlatch #(.WIDTH(1)) rem_sgn_1lt(.q(rem_sgn_2p), .d(md_rem_sgn_1d), .clk(clock));
  always_latch begin
    if (clock) rem_sgn_2p = md_rem_sgn_1d;
  end

  // Successful remainder adjust (enable remainder flop to accept adjustment)
  assign rq_rem_adj_2d = !dvnd_sgn_2p &&  rem_sgn_2p ||
                          dvnd_sgn_2p && !rem_sgn_2p && !final_zero_rem_2q ||
                          dvnd_sgn_2p && zero_rem_2q && !final_zero_rem_2q;

  // Successful quotient adjust (enable quotient flop to accept adjustment)
  assign rq_quot_cr_2d =  dvsr_sgn_2p && !dvnd_sgn_2p ||
                         !dvsr_sgn_2p &&  dvnd_sgn_2p && !(zero_rem_2q || final_zero_rem_2q) ||
                          dvsr_sgn_2p &&  dvnd_sgn_2p &&  (zero_rem_2q || final_zero_rem_2q);

  // rlatchn #(.WIDTH(1)) rq_rem_adj_2lt(.q(rq_rem_adj_1q), .d(rq_rem_adj_2d), .clk(clock));
  always_latch begin
    if (!clock) rq_rem_adj_1q = rq_rem_adj_2d;
  end

  // rlatchn #(.WIDTH(1)) rq_quot_cr_2lt(.q(rq_quot_cr_1q), .d(rq_quot_cr_2d), .clk(clock));
  always_latch begin
    if (!clock) rq_quot_cr_1q = rq_quot_cr_2d;
  end

  // rlatch #(.WIDTH(1)) div_by_zero_1lt(.q(div_by_zero_2p), .d(md_div_by_zero_1p), .clk(clock));
  always_latch begin
    if (clock) div_by_zero_2p = md_div_by_zero_1p;
  end

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

  assign mplr_sgn_1p  = md_op2_sgn_1q;

  // To Datapath
  assign mdctl_dvsr_sgn_1p = dvsr_sgn_1p;


  //-----------------------------------------------------------------------------------------//
  // Datapath controls
  //-----------------------------------------------------------------------------------------//
  logic              add_ain_sl_2d;                   // Adder A input select
  logic [1:0]        add_bin_sl_2d;                   // Adder B input select
  logic              div_cin_2d;                      // Adder carry-in for divides
  logic              se_zero_2d;                      // Sign Extend (or Zero) A input
  logic              fnl_mul_cyc_1p;                  // final multiply cycle (sv + cv)
  logic              fnl_pp_1p;                       // select final partial product


  assign mdctl_start_op_2p = start_div_2p || start_mul_2p;

  assign mdctl_clken_mpcnd_1d = start_mul_1p;

  assign mdctl_clken_muldvsr_ph2_1d = start_div_1p || start_mul_1p || mul_op_1p;

  assign mdctl_clken_mul_ph1_2d = start_mul_2p || mul_op_2p && |loop_cnt_2p[6:1];

  // Zero out PPA's in first cycle of multiply (and during divides)
  // rlatchn #(.WIDTH(1)) fc_zr_acc_lt(.q(mdctl_fc_zr_acc_1p), .d(!(start_mul_2p || div_op_2p)), .clk(clock));
  always_latch begin
    if (!clock) mdctl_fc_zr_acc_1p = !(start_mul_2p || div_op_2p);
  end

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

  // rlatchn #(.WIDTH(1)) add_ain_sl_2lt(.q(mdctl_add_ain_sl_1p), .d(add_ain_sl_2d), .clk(clock));
  always_latch begin
    if (!clock) mdctl_add_ain_sl_1p = add_ain_sl_2d;
  end

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

  // rlatchn #(.WIDTH(1)) se_ze_2lt(.q(mdctl_se_zero_1p),.d(se_zero_2d), .clk(clock));
  always_latch begin
    if (!clock) mdctl_se_zero_1p = se_zero_2d;
  end

  // rlatchn #(.WIDTH(2)) add_bin_sl_2lt(.q(mdctl_add_bin_sl_1p), .d(add_bin_sl_2d),  .clk(clock));
  always_latch begin
    if (!clock) mdctl_add_bin_sl_1p = add_bin_sl_2d;
  end

  /*
   divide carry in
   set to 1 if quotient adjustment is required OR
   during divide loop and remainder adjust, for subtraction
   */
  assign div_cin_2d = rem_adj_ph_2p && rq_quot_cr_2d || // quotient correction
                      start_div_2p                && (!dvnd_sgn_2d ^ dvsr_sgn_2d) ||
                      div_op_2p && (|loop_cnt_2p) && (!rem_sgn_2p  ^ dvsr_sgn_2d);

  // rlatchn #(.WIDTH(1)) div_cin_2lt(.q(mdctl_div_cin_1p), .d(div_cin_2d), .clk(clock));
  always_latch begin
    if (!clock) mdctl_div_cin_1p = div_cin_2d;
  end

  //-----------------------------------------------------------------------------------------//
  // Enable final partial product
  assign fnl_pp_1p = mul_op_1p && (loop_cnt_1p == 7'd1) && (mplr_1p_7 && !mplr_sgn_1p);


  assign fnl_mul_cyc_1p = mul_op_1p && (loop_cnt_1p == 7'd0);

  // To Datapath
  assign mdctl_fnl_pp_1p = fnl_pp_1p;

  // rlatchn #(.WIDTH(1)) mdctl_fnl_mul_cyc_2lt(.q(mdctl_fnl_mul_cyc_1p), .d(mul_op_2p && (loop_cnt_2p == 7'd1)), .clk(clock));
  always_latch begin
    if (!clock) mdctl_fnl_mul_cyc_1p = mul_op_2p && (loop_cnt_2p == 7'd1);
  end

  //-----------------------------------------------------------------------------------------//
  // Response
  logic            resp_valid_2q;

  assign resp_valid_early = mul_op_2p && ((muldiv_fn_2q == AluMul) ? (loop_cnt_2p == 7'd2) : (loop_cnt_2p == 7'd1)) ||
                            rem_adj_ph_2p || resp_valid_2q && !resp_ready;

  assign resp_valid = resp_valid_2q;

  assign req_ready = !(mul_op_2p || div_op_2p) && !resp_valid_2q;


  // rst_ff #(.width(1)) resp_valid_rg(...)
  always_ff @(posedge clock)
    if (reset) resp_valid_2q <= 1'b0;
    else       resp_valid_2q <= resp_valid_early;

  // [1]: picks mull
  // [0]: picks quot
  assign mdctl_resp_sl_2p[1] = (muldiv_fn_2q == AluMul);
  assign mdctl_resp_sl_2p[0] = (muldiv_fn_2q == AluDiv)    ||
                                (muldiv_fn_2q == AluDivu);

  assign debug_start_mul_1p_o = start_mul_1p;
  assign debug_start_mul_2p_o = start_mul_2p;
  assign debug_mul_op_1p_o    = mul_op_1p;
  assign debug_mul_op_2p_o    = mul_op_2p;
  assign debug_loop_cnt_1p_o  = loop_cnt_1p;
  assign debug_loop_cnt_2p_o  = loop_cnt_2p;


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
