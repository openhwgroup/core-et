// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

/* verilator lint_off WIDTHEXPAND */  // Imported original TXFMA exponent stage relies on macro-sized sign-extension idioms and narrow constant literals.
/* verilator lint_off WIDTHTRUNC */  // The preserved original datapath truncates back into packed exponent slices after wider helper expressions.
/* verilator lint_off UNUSEDSIGNAL */  // Helper carry/signal bits are intentionally kept even when not all packed bits feed later logic in this build.
module txfma_e1
(
  input  [VPU_DTYPE_SZ-1:0]                op_dtype_f1a_h,
  input  [TXFMA_SIGS_SZ-1:0]               sigs_f1a_h,
  input  [VPU_FCMD_SZ-1:0]                 cmd_f1a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]             ea_hi_f1a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]             eb_hi_f1a_h,
  input  [TXFMA_EXP_PH_SZ-1:0]             ea_lo_f1a_h,
  input  [TXFMA_EXP_PH_SZ-1:0]             eb_lo_f1a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]             ec_f1a_h,
  input                                     ec_zero_f1a_h,
  input                                     ec_all1_f1a_h,
  input                                     ea_hi_zero_f1a_h,
  input  [3:0]                              etrz_fa_hi_ph_f1a_h,
  input  [4:0]                              etrz_fa_lo_ps_f1a_h,
  input  [3:0]                              etrz_fb_hi_ph_f1a_h,
  input  [4:0]                              etrz_fb_lo_ps_f1a_h,
  input                                     fc_zero30_8_f1a_h,
  output logic [TXFMA_ETRZ_PROD_PH_SZ-1:0] etrz_prod_hi_ph_f1a_h,
  output logic [TXFMA_ETRZ_PROD_PS_SZ-1:0] etrz_prod_lo_ps_f1a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  ediff_ec_eph_f1a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  ediff_ec_epl_f1a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  ediff_eph_epl_f1a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  eprod_hi_f1a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  eprod_lo_f1a_h,
  output logic [TXFMA_EXP_PS_SZ-1:0]       ec_out_f1a_h
);


///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  ea_hi_mux_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  eb_hi_mux_f1a_h;
logic [TXFMA_EXP_PS_SZ-1:0]       ec_mux_f1a_h;
logic [TXFMA_EXP_PS_SZ-1:0]       ea_lo_mux_f1a_h;
logic [TXFMA_EXP_PS_SZ-1:0]       eb_lo_mux_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  cin_hi_mux_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  cin_lo_mux_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  carry32_hi_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  carry32_lo_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  sum32_hi_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  sum32_lo_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  trans_log_mul_inc_dec_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  trans_log_mul_ec_f1a_h;
logic                              ec_gt127_f1a_h;
logic                              comp_op_f1a_h;
logic                              minmax_op_f1a_h;
logic                              cin_hi_bias_sel_f1a_h;
logic                              cin_hi_trans_zero_sel_f1a_h;
logic                              cin_hi_zero_sel_f1a_h;

///////////////////////////////////////////////////////////////////////////////
// exp hi/lo mux
//for ph/pl, convert exponent from f16 to f32:
//  -copy e16 b3:0 to e32 b3:0
//  -replicate e16 b4(negated) to e32 bit b6:4
//  -copy e16 b4 to e32 b7
///////////////////////////////////////////////////////////////////////////////

  //exp hi
always_comb begin

  ea_hi_mux_f1a_h = (cmd_f1a_h == VPU_FCMD_CVT_INTF32 |
                     cmd_f1a_h == VPU_FCMD_CVT_UINTF32 |
                     cmd_f1a_h == VPU_FCMD_FROUND)           ? {2'h0, TXFMA_CVT_INTF32_OFFSET}
                  : (cmd_f1a_h == VPU_FCMD_CVT_RASTF32)      ? {2'h0,TXFMA_CVT_FXP1714_F32_OFFSET}
                  : (cmd_f1a_h == VPU_TRANS_RCP_FMA2)        ? {2'h0,TXFMA_TRANS_RCP_OFFSET}
                  : (cmd_f1a_h == VPU_TRANS_RSQRT_FMA2)      ? {TXFMA_EXP_SEXT_PS_SZ{1'b1}}
                  : ((cmd_f1a_h == VPU_TRANS_EXP_FMA2) |
                     (cmd_f1a_h == VPU_TRANS_SIN_TRANSFORM)) ? '0
                  : (cmd_f1a_h == VPU_TRANS_LOG_MUL)         ? trans_log_mul_ec_f1a_h
                  : (op_dtype_f1a_h == VPU_DTYPE_F32)        ? {2'h0, ea_hi_f1a_h}
                  :                                             {2'h0, ea_hi_f1a_h[TXFMA_EXP_PH_SZ-1],
                                                                 {3{~ea_hi_f1a_h[TXFMA_EXP_PH_SZ-1] & ~ea_hi_zero_f1a_h}},
                                                                 ea_hi_f1a_h[TXFMA_EXP_PH_SZ-2:0]};

  eb_hi_mux_f1a_h = (cmd_f1a_h==VPU_TRANS_RCP_FMA2)      ? {2'h3,~eb_hi_f1a_h}
                  : (cmd_f1a_h==VPU_TRANS_LOG_MUL)       ? trans_log_mul_inc_dec_f1a_h
                  : (cmd_f1a_h==VPU_TRANS_SIN_TRANSFORM) ? {2'h0, FLOAT32_E_BIAS_HEX}
                  : (cmd_f1a_h==VPU_TRANS_EXP_FMA2)      ? {{(TXFMA_EXP_SEXT_PS_SZ-$bits(eb_hi_f1a_h)){eb_hi_f1a_h[$bits(eb_hi_f1a_h)-1]}}, eb_hi_f1a_h}
                  : (op_dtype_f1a_h==VPU_DTYPE_F32 |
                     sigs_f1a_h[TXFMA_F32_FX] |
                     cmd_f1a_h==VPU_TRANS_RSQRT_FMA2)
                                                          ? {2'h0, eb_hi_f1a_h}
                  :                                         {2'h0, eb_hi_f1a_h[TXFMA_EXP_PH_SZ-1],
                                                            {3{~eb_hi_f1a_h[TXFMA_EXP_PH_SZ-1]}},
                                                             eb_hi_f1a_h[TXFMA_EXP_PH_SZ-2:0]};

  ec_mux_f1a_h = (op_dtype_f1a_h == VPU_DTYPE_F32 ||
                  op_dtype_f1a_h == VPU_DTYPE_F16_F32) ? ec_f1a_h
                    :   (ec_zero_f1a_h)                ? 8'h0                      //  5 bit exponent equals 0
                    :   (ec_all1_f1a_h)                 ?   8'hFF                   // if input is all 1's, map it to all 1's
                    :                                     {ec_f1a_h[TXFMA_EXP_PH_SZ-1] , {3{~ec_f1a_h[TXFMA_EXP_PH_SZ-1]}}, ec_f1a_h[TXFMA_EXP_PH_SZ-2:0]};

  if (sigs_f1a_h[TXFMA_NORM_F32])
    ec_mux_f1a_h = TXFMA_NORM_F32_BIAS;

end

  //exp lo
assign ea_lo_mux_f1a_h = (cmd_f1a_h==VPU_TRANS_EXP_FRAC) ? 8'h86 //compare fc>=128 (exp is 0x86)
                       :                                    {ea_lo_f1a_h[TXFMA_EXP_PH_SZ-1],{3{~ea_lo_f1a_h[TXFMA_EXP_PH_SZ-1]}},ea_lo_f1a_h[TXFMA_EXP_PH_SZ-2:0]};

assign eb_lo_mux_f1a_h = (cmd_f1a_h==VPU_TRANS_EXP_FRAC) ? 8'h0
                       :                                    {eb_lo_f1a_h[TXFMA_EXP_PH_SZ-1],{3{~eb_lo_f1a_h[TXFMA_EXP_PH_SZ-1]}},eb_lo_f1a_h[TXFMA_EXP_PH_SZ-2:0]};

assign comp_op_f1a_h   = (cmd_f1a_h==VPU_FCMD_FLE) | (cmd_f1a_h==VPU_FCMD_FLT) | (cmd_f1a_h==VPU_FCMD_FEQ);
assign minmax_op_f1a_h = (cmd_f1a_h==VPU_FCMD_MIN) |(cmd_f1a_h==VPU_FCMD_MAX);

  //cin
assign cin_hi_bias_sel_f1a_h = (cmd_f1a_h==VPU_FCMD_CVT_INTF32)  | (cmd_f1a_h==VPU_FCMD_CVT_UINTF32) | (cmd_f1a_h==VPU_FCMD_CVT_RASTF32)
                             | (cmd_f1a_h==VPU_FCMD_FROUND) | (cmd_f1a_h==VPU_FCMD_FFRC) | (cmd_f1a_h==VPU_FCMD_FFRC_SIN) | (cmd_f1a_h==VPU_TRANS_EXP_FMA2);

assign cin_hi_trans_zero_sel_f1a_h = sigs_f1a_h[TXFMA_TRANS2] |  (cmd_f1a_h==VPU_TRANS_SIN_TRANSFORM) | (cmd_f1a_h==VPU_TRANS_LOG_MUL) | (cmd_f1a_h==VPU_TRANS_EXP_FRAC);
assign cin_hi_zero_sel_f1a_h = cin_hi_trans_zero_sel_f1a_h | (cmd_f1a_h==VPU_FCMD_ADD) | (cmd_f1a_h==VPU_FCMD_SUB) | (cmd_f1a_h==VPU_FCMD_CVT_F16F32)
                             | (cmd_f1a_h==VPU_FCMD_CVT_F10F32) | (cmd_f1a_h==VPU_FCMD_CVT_F11F32) |  comp_op_f1a_h | minmax_op_f1a_h;

assign cin_hi_mux_f1a_h = cin_hi_bias_sel_f1a_h ? {2'h0, FLOAT32_E_BIAS_HEX}
                        : cin_hi_zero_sel_f1a_h ? 10'h0
                        :                         {2'h3, FLOAT32_NEG_E_BIAS_HEX};

assign cin_lo_mux_f1a_h = (cmd_f1a_h==VPU_TRANS_EXP_FRAC) ? 10'b0
                        :                                    {2'h3, FLOAT32_NEG_E_BIAS_HEX};

//output
assign ec_out_f1a_h = ec_mux_f1a_h;


///////////////////////////////////////////////////////////////////////////////
// exp hi eprod csa
///////////////////////////////////////////////////////////////////////////////
txfma_csa
#(
  .Bits( TXFMA_EXP_SEXT_PS_SZ )
)
exp_hi_prod_csa
(
 .a_i  ( ea_hi_mux_f1a_h  ),
 .b_i  ( eb_hi_mux_f1a_h  ),
 .cin_i( cin_hi_mux_f1a_h ),
 .cout_o( carry32_hi_f1a_h ),
 .sum_o( sum32_hi_f1a_h   )
);


///////////////////////////////////////////////////////////////////////////////
// exp hi eprod adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .Width( TXFMA_EXP_SEXT_PS_SZ )
)
exp_hi_ep_adder
(
  .a_i  ( sum32_hi_f1a_h                                     ),
  .b_i  ( {carry32_hi_f1a_h[TXFMA_EXP_SEXT_PS_SZ-2:0],1'b0} ),
  .cin_i( 1'b0                                               ),
  .sum_o( eprod_hi_f1a_h                                     )
);


///////////////////////////////////////////////////////////////////////////////
// exp lo eprod csa
///////////////////////////////////////////////////////////////////////////////
txfma_csa
#(
  .Bits( TXFMA_EXP_SEXT_PS_SZ )
)
exp_lo_prod_csa
(
 .a_i  ( {2'h0, ea_lo_mux_f1a_h}         ),
 .b_i  ( {2'h0, eb_lo_mux_f1a_h}         ),
 .cin_i( cin_lo_mux_f1a_h                ),
 .cout_o( carry32_lo_f1a_h                ),
 .sum_o( sum32_lo_f1a_h                  )
);


///////////////////////////////////////////////////////////////////////////////
// exp lo eprod adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .Width( TXFMA_EXP_SEXT_PS_SZ )
)
exp_lo_ep_adder
(
  .a_i  ( sum32_lo_f1a_h                                     ),
  .b_i  ( {carry32_lo_f1a_h[TXFMA_EXP_SEXT_PS_SZ-2:0],1'b0} ),
  .cin_i( 1'b0                                               ),
  .sum_o( eprod_lo_f1a_h                                     )
);

///////////////////////////////////////////////////////////////////////////////
// ec-eph ediff adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .Width( TXFMA_EXP_SEXT_PS_SZ )
)
ec_eph_ediff_adder
(
  .a_i  ( {2'b0, ec_mux_f1a_h}    ),
  .b_i  ( ~eprod_hi_f1a_h         ),
  .cin_i( 1'b1                    ),
  .sum_o( ediff_ec_eph_f1a_h      )
);


///////////////////////////////////////////////////////////////////////////////
// ec-epl ediff adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .Width( TXFMA_EXP_SEXT_PS_SZ )
)
ec_epl_ediff_adder
(
  .a_i  ( {2'b0, ec_f1a_h}        ),
  .b_i  ( ~eprod_lo_f1a_h         ),
  .cin_i( 1'b1                    ),
  .sum_o( ediff_ec_epl_f1a_h      )
);


///////////////////////////////////////////////////////////////////////////////
// eph-epl ediff adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .Width( TXFMA_EXP_SEXT_PS_SZ )
)
eph_epl_ediff_adder
(
  .a_i  ( eprod_hi_f1a_h      ),
  .b_i  ( ~eprod_lo_f1a_h     ),
  .cin_i( 1'b1                ),
  .sum_o( ediff_eph_epl_f1a_h )
);


///////////////////////////////////////////////////////////////////////////////
// exp hi ph tp adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .Width( TXFMA_ETRZ_PROD_PH_SZ )
)
exp_hi_tp_adder
(
  .a_i  ( {1'b0, etrz_fa_hi_ph_f1a_h} ),
  .b_i  ( {1'b0, etrz_fb_hi_ph_f1a_h} ),
  .cin_i( 1'b0                        ),
  .sum_o( etrz_prod_hi_ph_f1a_h       )
);


///////////////////////////////////////////////////////////////////////////////
// exp lo ps tp adder
///////////////////////////////////////////////////////////////////////////////

txfma_adder
#(
  .Width( TXFMA_ETRZ_PROD_PS_SZ )
)
exp_lo_tp_adder
(
  .a_i  ( {1'b0, etrz_fa_lo_ps_f1a_h}      ),
  .b_i  ( {1'b0, etrz_fb_lo_ps_f1a_h}      ),
  .cin_i( 1'b0                      ),
  .sum_o( etrz_prod_lo_ps_f1a_h     )
);


///////////////////////////////////////////////////////////////////////////////
// trans log mul
///////////////////////////////////////////////////////////////////////////////
  // +1: ec >= 127 && fc[22]=='1'
  // -1: ec < 127 && fc!=0
always_comb begin

  ec_gt127_f1a_h = (ec_f1a_h >= 127);

  trans_log_mul_inc_dec_f1a_h = '0;

    //ec >= 127
  if (ec_gt127_f1a_h)
    trans_log_mul_inc_dec_f1a_h = 10'h1;
    //ec<127 and fc!=0
  else if (!fc_zero30_8_f1a_h)
    trans_log_mul_inc_dec_f1a_h = {TXFMA_EXP_SEXT_PS_SZ{1'b1}};
end
  // neg ec if ec<127
assign trans_log_mul_ec_f1a_h = ec_gt127_f1a_h ? {2'h0,ec_f1a_h}
                              :                  {3'h7, ~ec_f1a_h[TXFMA_EXP_PS_SZ-2:0]};


endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
