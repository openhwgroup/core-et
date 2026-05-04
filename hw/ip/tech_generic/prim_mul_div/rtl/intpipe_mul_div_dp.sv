// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_mul_div_dp -- Integer Multiplier / Divider Datapath.
//
// Booth-encoded radix-16 multiplier with 4:2 and 3:2 compressor
// tree.  Restoring divider sharing the same accumulator and adder.
//
// Latch-based dual-phase pipeline preserved 1:1 from the original.
//
// Line-by-line translation of: intpipe_mul_div_dp (etcore-soc)

// Latch-based design has intentional combinational loops through the
// accumulator and clock-gate paths.  Suppress Verilator's UNOPTFLAT.
/* verilator lint_off UNOPTFLAT */

module intpipe_mul_div_dp
  import minion_pkg::*;
(
  // Outputs
  output logic [XregSize-1:0]  md_resp_data_2p,             // Multiply Hi result

  output logic [7:0]           md_mplr_2d,                  // multiplier bits for booth encoding
  output logic                 md_rem_sgn_1d,               // remainder sign
  output logic                 md_zero_rem_1d,              // remainder is zero
  output logic                 md_nxt_quot_1q,              // next quotient bit (lsb of remainder)
  output logic                 md_div_by_zero_1p,           // Divide By zero
  output logic [XregSize:0]    debug_mpcnd_1p_o,
  output logic [XregSize:0]    debug_mul_lo_dvsr_1p_o,
  output logic [XregSize-1:0]  debug_mul_lo_2p_o,
  output logic [XregSize+1:0]  debug_acc_v1_1q_o,
  output logic [XregSize+1:0]  debug_acc_v2_1q_o,

  // From ALU
  input  logic [XregSize-1:0]  ex_op1_2p,                   // Multiplicand / Dividend
  input  logic [XregSize-1:0]  ex_op2_2p,                   // Multiplier / Divisor

  // From control
  input  logic                 op1_sgnd_2p,                 // Signed Multiplicand / Dividend
  input  logic                 op2_sgnd_2p,                 // Signed Multiplier / Divisor
  input  logic                 ex_dw_2p,

  // From mul/div control
  input  logic [2:0]           mdctl_besl_0_1p,             // booth encode select pp_0
  input  logic [2:0]           mdctl_besl_1_1p,             // booth encode select pp_1
  input  logic [2:0]           mdctl_besl_2_1p,             // booth encode select pp_2
  input  logic [2:0]           mdctl_besl_3_1p,             // booth encode select pp_3
  input  logic                 mdctl_start_op_2p,           // start multiply or divide
  input  logic                 mdctl_fnl_pp_1p,             // select final partial product
  input  logic                 mdctl_dw_1q,                 // sign extend result for 32b ops
  input  logic                 mdctl_dw_2q,                 // sign extend result for 32b ops
  input  logic [1:0]           mdctl_acc_v1_sl_1d,          // accumulator v1, remainder, mhi input select
  input  logic [1:0]           mdctl_acc_v2_sl_1d,          // accumulator v2, quotient input select
  input  logic                 mdctl_add_ain_sl_1p,         // select A-input of adder
  input  logic [1:0]           mdctl_add_bin_sl_1p,         // select B-input of adder
  input  logic                 mdctl_se_zero_1p,            // sign extend or zero Ain to adder
  input  logic                 mdctl_fnl_mul_cyc_1p,        // final multiply cycle (sv + cv)
  input  logic                 mdctl_start_div_2p,          // start new divide
  input  logic                 mdctl_fc_zr_acc_1p,          // zero out acc vectors (first of multiply or divides)
  input  logic                 mdctl_dvsr_sgn_1p,           // divisor sign
  input  logic                 mdctl_div_cin_1p,            // divide carry in
  input  logic [1:0]           mdctl_resp_sl_2p,            // response select

  input  logic                 mdctl_clken_mpcnd_1d,        // Multiplicand clock enable
  input  logic                 mdctl_clken_mul_ph1_2d,      // Multiplier clock enable
  input  logic                 mdctl_clken_muldvsr_ph2_1d,  // Multiplier / Divisor clock enable
  input  logic                 mdctl_clken_acc_v1_2p,       // acc_v1 register clock enable
  input  logic                 mdctl_clken_acc_v1_1p,       // acc_v1 register clock enable
  input  logic                 mdctl_clken_acc_v2_2p,       // acc_v2 register clock enable
  input  logic                 mdctl_clken_acc_v2_1p,       // acc_v2 register clock enable

  input  logic                 clock,
  input  logic                 scan_shift_en                // DFT — tied to 0
);

  localparam int unsigned WregSize = 32;
  localparam int unsigned WregMsb  = WregSize - 1;

  // Unused DFT signal
  logic unused_scan;
  assign unused_scan = scan_shift_en;

  //--------------------------------------------------------------------------
  // acc_v1 & acc_v2_1p have the same weight
  logic [XregSize+1:0]  acc_v1_1d;                   // Accumulated vector-1
  logic [XregSize+1:0]  acc_v2_1d;                   // Accumulated vector-2

  logic                 fa_cout_1d;                  // carry out from the Full Adder
  /* verilator lint_off UNUSEDSIGNAL */  // The hi-phase half of the preserved phase-pair exists to keep the original two-phase state shape even though only q_lo is consumed locally.
  logic                 fa_cout_2q;                  // carry out from the Full Adder
  /* verilator lint_on UNUSEDSIGNAL */
  logic                 fa_cout_1q;                  // carry out from the Full Adder

  logic [XregSize+1:0]  pp_0_1p;                     // Partial Product-0
  logic [XregSize+1:0]  pp_1_1p;                     // Partial Product-1
  logic [XregSize+1:0]  pp_2_1p;                     // Partial Product-2
  logic [XregSize+1:0]  pp_3_1p;                     // Partial Product-3
  logic [XregSize-1:0]  pp_l_1p;                     // Final Adjustment

  logic [XregSize+3:0]  sv_0_0_1p;                   // Level-0 sum vector from 4-2 compressor
  logic [XregSize+3:0]  cv_0_0_1p;                   // Level-0 carry vector from 4-2 compressor
  logic [XregSize+3:0]  sv_0_1_1p;                   // Level-0 sum vector from 3-2 compressor
  logic [XregSize+3:0]  cv_0_1_1p;                   // Level-0 carry vector from 3-2 compressor

  logic [XregSize+6:0]  sv_1_0_1p;                   // Level-1 sum vector from 4-2 compressor
  logic [XregSize+6:0]  cv_1_0_1p;                   // Level-1 carry vector from 4-2 compressor

  logic [7:0]           l2_mul_res_1p;               // Level-2
  logic                 l2_mul_co_1d;


  /*
   According to the RISCV spec, 32b operands must already be appropriately sign extended
  */
  //-----------------------------------------------------------------------------------//
  // Multiplier, Divisor latches
  //-----------------------------------------------------------------------------------//
  logic [XregSize:0]    mpcnd_1p;                    // Multiplicand (held for duration of multiply)
  logic [XregSize:0]    mul_lo_dvsr_1p;              // Ph2 latched mul_lo OR divisor
  logic [XregSize-1:0]  mul_lo_2p;                   // Ph1 latched multiplier Lo result
  logic [XregSize:0]    mul_lo_dvsr_2d;              // next mul_lo_dvsr
  logic [XregSize-1:0]  mul_lo_1d;                   // next mul_lo
  logic [XregSize:0]    mpcnd_1d;
  logic [XregSize:0]    dvsr_1d;
  logic                 mpcnd_en_2p;
  logic                 mul_lo_dvsr_en_2p;
  logic                 mul_lo_en_1p;
  logic                 acc_v2_2q_en_1p;
  logic                 acc_v2_1q_en_2p;


  // Multiplicand, Dividend
  assign mpcnd_1d = ex_dw_2p ? {    op1_sgnd_2p && ex_op1_2p[63],   ex_op1_2p} :
                                {{33{op1_sgnd_2p && ex_op1_2p[31]}}, ex_op1_2p[31:0]};

  // rlatchn #(65) mpcnd_se_rg — low-phase transparent with high-phase sampled enable
  always_latch begin
    if (clock) begin
      mpcnd_en_2p = mdctl_clken_mpcnd_1d;
    end
  end

  always_latch begin
    if (!clock && mpcnd_en_2p) begin
      mpcnd_1p = mpcnd_1d;
    end
  end


  // Multiplier, Divisor
  assign dvsr_1d = ex_dw_2p ? {    op2_sgnd_2p && ex_op2_2p[63],   ex_op2_2p} :
                               {{33{op2_sgnd_2p && ex_op2_2p[31]}}, ex_op2_2p[31:0]};

  assign mul_lo_dvsr_2d = mdctl_start_op_2p ? dvsr_1d : {1'b0, mul_lo_2p};
  // rlatchn #(65) mul_lo_dvsr_1lt — low-phase transparent with high-phase sampled enable
  always_latch begin
    if (clock) begin
      mul_lo_dvsr_en_2p = mdctl_clken_muldvsr_ph2_1d;
    end
  end

  always_latch begin
    if (!clock && mul_lo_dvsr_en_2p) begin
      mul_lo_dvsr_1p = mul_lo_dvsr_2d;
    end
  end

  assign md_div_by_zero_1p  = mul_lo_dvsr_1p[63:0] == 64'b0;

  // only for multiplies, shift in the 8b of result while shifting out the multiplier bits
  assign mul_lo_1d = mdctl_dw_1q ? {l2_mul_res_1p, mul_lo_dvsr_1p[63:8]}:
                                    {32'b0, l2_mul_res_1p, mul_lo_dvsr_1p[31:8]};
  // rlatch #(64) mplr_lo_1lt — high-phase transparent with low-phase sampled enable
  always_latch begin
    if (!clock) begin
      mul_lo_en_1p = mdctl_clken_mul_ph1_2d;
    end
  end

  always_latch begin
    if (clock && mul_lo_en_1p) begin
      mul_lo_2p = mul_lo_1d;
    end
  end

  assign md_mplr_2d = mul_lo_dvsr_2d[7:0];

  // Final multiply lo result is in mul_lo_dvsr_1p
  //-----------------------------------------------------------------------------------//
  // Multiplier
  //-----------------------------------------------------------------------------------//

  assign pp_0_1p = !mdctl_besl_0_1p[1] ? ( {66{!mdctl_besl_0_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_0_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                          ( {66{!mdctl_besl_0_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_0_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_1_1p = !mdctl_besl_1_1p[1] ? ( {66{!mdctl_besl_1_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_1_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                          ( {66{!mdctl_besl_1_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_1_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_2_1p = !mdctl_besl_2_1p[1] ? ( {66{!mdctl_besl_2_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_2_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                          ( {66{!mdctl_besl_2_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_2_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_3_1p = !mdctl_besl_3_1p[1] ? ( {66{!mdctl_besl_3_1p[0]}} & {mpcnd_1p, 1'b0}         | {66{!mdctl_besl_3_1p[2]}} & ~{mpcnd_1p, 1'b0}) :
                                          ( {66{!mdctl_besl_3_1p[0]}} & {mpcnd_1p[64], mpcnd_1p} | {66{!mdctl_besl_3_1p[2]}} & ~{mpcnd_1p[64], mpcnd_1p});


  assign pp_l_1p = mdctl_fnl_pp_1p ? mpcnd_1p[XregSize-1:0] : 64'b0;


  // Level-0 has a 4-2 and a 3-2 compressor
  prim_cmp_42 #(.Width(68)) l0_42_i(
    .sv  (sv_0_0_1p),
    .cv  (cv_0_0_1p),
    .a   ({ {2{acc_v1_1d[65]}}, acc_v1_1d    }),
    .b   ({ {2{acc_v2_1d[65]}}, acc_v2_1d    }),
    .c   ({ {2{pp_0_1p[65]}},   pp_0_1p      }),
    .d   ({  pp_1_1p, 1'b0,     !mdctl_besl_0_1p[2] }),
    .cin (fa_cout_1d)
  );

  prim_cmp_32 #(.Width(68)) l0_32_i(
    .sv  (sv_0_1_1p),
    .cv  (cv_0_1_1p),
    .a   ({ {2{pp_2_1p[65]}}, pp_2_1p }),
    .b   ({  pp_3_1p, 1'b0,   !mdctl_besl_2_1p[2]}),
    .c   ({  pp_l_1p, 1'b0,   !mdctl_besl_3_1p[2], 2'b0})
  );

  // Level-1 has 4-2  compressor
  prim_cmp_42 #(.Width(71)) l1_42_i(
    .sv  (sv_1_0_1p),
    .cv  (cv_1_0_1p),
    .a   ({ {5{sv_0_0_1p[67]}}, sv_0_0_1p[67:2] }),
    .b   ({ {4{cv_0_0_1p[67]}}, cv_0_0_1p[67:1] }),
    .c   ({ {1{sv_0_1_1p[67]}}, sv_0_1_1p, 1'b0, !mdctl_besl_1_1p[2] }),
    .d   ({  cv_0_1_1p, 3'b0 }),
    .cin (1'b0)
  );

  // Level-2 has a 6-b full adder that generates a  carry-out for the next iteration's fa_cout_1q
  // while the remaining 64 bits of result are held in a sv,cv register.
  assign                l2_mul_res_1p[1:0]  = {sv_0_0_1p[1] ^ cv_0_0_1p[0], sv_0_0_1p[0]};
  assign {l2_mul_co_1d, l2_mul_res_1p[7:2]} =  sv_1_0_1p[5:0] + {cv_1_0_1p[4:0], sv_0_0_1p[1] && cv_0_0_1p[0]};

  // Setup for next multiply iteration
  logic [XregSize+1:0]  nxt_acc_v1_1p;     // next accumulated vector-1
  logic [XregSize+1:0]  nxt_acc_v2_1p;     // next accumulated vector-2
  logic                 nxt_fa_cout_1p;    // next carry-in from previous iteration

  assign nxt_acc_v1_1p  = {sv_1_0_1p[70], sv_1_0_1p[70:6]};
  assign nxt_acc_v2_1p  =                 cv_1_0_1p[70:5] ;
  assign nxt_fa_cout_1p = l2_mul_co_1d;

  //---------------------------------------------------------------------------------------
  // For Divide operations, the remainder is held in acc_v1_1q while the dividend, quotient
  // is  in acc_v2_1q.
  // For Multiply the final result is in acc_v1_1q
  logic [XregSize+1:0]  acc_v1_1q;         // accumulated v1 vector to PPA
  /* verilator lint_off UNUSEDSIGNAL */  // The phase-pair keeps the full translated width; only the architectural Xreg payload bits are consumed downstream.
  logic [XregSize+1:0]  acc_v1_2p;         // accumulated v1 vector to PPA
  /* verilator lint_on UNUSEDSIGNAL */
  logic [XregSize+1:0]  acc_v2_1q;         // accumulated v2 vector to PPA
  logic [XregSize+1:0]  acc_v2_2d;
  logic [XregSize+1:0]  acc_v2_2q;

  logic [XregSize+1:0]  nxt_acc_v1_1d;     // next acc_v1
  logic [XregSize+1:0]  nxt_acc_v2_1d;     // next acc_v2


  logic [XregSize:0]    sum_1d;
  logic                 cout_1d;            // carry out of adder

  assign md_nxt_quot_1q = mdctl_dw_1q ? acc_v2_1q[63] : acc_v2_1q[31];

  assign nxt_acc_v1_1d = (mdctl_acc_v1_sl_1d[1] == 1'b1) ? {1'b0, sum_1d} :  // remainder adjust, final multiply
                         (mdctl_acc_v1_sl_1d == 2'b01)   ? nxt_acc_v1_1p  :  // multiply loop as accumulator
                                                           {sum_1d, md_nxt_quot_1q}; // divide loop

  assign nxt_acc_v2_1d = (mdctl_acc_v2_sl_1d[1] == 1'b1) ? {1'b0, sum_1d} : // quotient adjust
                         (mdctl_acc_v2_sl_1d == 2'b01)   ? nxt_acc_v2_1p  : // multiply loop as accumulator
                                                           {acc_v2_1q[XregSize:0], cout_1d ^ mdctl_dvsr_sgn_1p || md_div_by_zero_1p}; // divide loop

  prim_phase_pair_hi_lo #(
    .Width (XregSize + 2)
  ) u_acc_v1_pair (
    .clk_i   (clock),
    .hi_en_i (mdctl_clken_acc_v1_2p),
    .lo_en_i (mdctl_clken_acc_v1_1p),
    .d_i     (nxt_acc_v1_1d),
    .q_hi_o  (acc_v1_2p),
    .q_lo_o  (acc_v1_1q)
  );

  // CV, QUOT register
  assign  acc_v2_2d = mdctl_start_div_2p ? {1'b0, mpcnd_1d} : acc_v2_2q;
  // rlatch  #(.WIDTH(66)) acc_v2_1lt — high-phase transparent with low-phase sampled enable
  always_latch begin
    if (!clock) begin
      acc_v2_2q_en_1p = mdctl_clken_acc_v2_2p;
    end
  end

  always_latch begin
    if (clock && acc_v2_2q_en_1p) begin
      acc_v2_2q = nxt_acc_v2_1d;
    end
  end

  // rlatchn #(.WIDTH(66)) acc_v2_2lt — low-phase transparent with high-phase sampled enable
  always_latch begin
    if (clock) begin
      acc_v2_1q_en_2p = mdctl_clken_acc_v2_1p;
    end
  end

  always_latch begin
    if (!clock && acc_v2_1q_en_2p) begin
      acc_v2_1q = acc_v2_2d;
    end
  end


  prim_phase_pair_hi_lo #(
    .Width (1)
  ) u_fa_cout_pair (
    .clk_i   (clock),
    .hi_en_i (mdctl_clken_acc_v2_2p),
    .lo_en_i (mdctl_clken_acc_v2_1p),
    .d_i     (nxt_fa_cout_1p),
    .q_hi_o  (fa_cout_2q),
    .q_lo_o  (fa_cout_1q)
  );

  assign md_zero_rem_1d = sum_1d[XregSize:0] == 65'd0; // late. If needed can be moved to next cycle


  assign acc_v1_1d  = acc_v1_1q  & {66{mdctl_fc_zr_acc_1p}};
  assign acc_v2_1d  = acc_v2_1q  & {66{mdctl_fc_zr_acc_1p}};
  assign fa_cout_1d = fa_cout_1q && mdctl_fc_zr_acc_1p;


  //------------------------------------------------------------------------------
  // Adder and its inputs
  //------------------------------------------------------------------------------
  logic [XregSize:0]    ain_1p;              // Adder A input
  logic [XregSize:0]    bin_1p;              // Adder B input
  logic                 cin_1p;              // Adder Cin input

  assign ain_1p = {65{mdctl_add_ain_sl_1p}} & acc_v1_1q[XregSize:0] | {65{mdctl_se_zero_1p}};


  // For divides, select divisor or ~divisor
  assign bin_1p = (mdctl_add_bin_sl_1p == 2'b11) ? ~mul_lo_dvsr_1p :
                  (mdctl_add_bin_sl_1p == 2'b10) ?  mul_lo_dvsr_1p :
                  (mdctl_add_bin_sl_1p == 2'b01) ? acc_v2_1q[XregSize:0]      :
                                                   65'd0; // zero during multiply loop to conserve power

  assign cin_1p = mdctl_fnl_mul_cyc_1p ? fa_cout_1q : mdctl_div_cin_1p;

  assign {cout_1d, sum_1d} = ain_1p + bin_1p + {64'b0, cin_1p};

  assign md_rem_sgn_1d = sum_1d[64];

  //------------------------------------------------------------------------------
  // Named outputs, replace with 'rd'
  //------------------------------------------------------------------------------

  // response destination
  logic [XregSize-1:0]  resp_data_2d;

  assign resp_data_2d = mdctl_resp_sl_2p[1] ? mul_lo_2p[XregSize-1:0] :
                        mdctl_resp_sl_2p[0] ? acc_v2_2q[XregSize-1:0] :
                                              acc_v1_2p[XregSize-1:0];

  assign md_resp_data_2p   = mdctl_dw_2q ? resp_data_2d :
                                           {{WregSize{resp_data_2d[WregMsb]}}, resp_data_2d[WregMsb:0]};

  assign debug_mpcnd_1p_o       = mpcnd_1p;
  assign debug_mul_lo_dvsr_1p_o = mul_lo_dvsr_1p;
  assign debug_mul_lo_2p_o      = mul_lo_2p;
  assign debug_acc_v1_1q_o      = acc_v1_1q;
  assign debug_acc_v2_1q_o      = acc_v2_1q;

  /* verilator lint_on UNOPTFLAT */

endmodule // intpipe_mul_div_dp
