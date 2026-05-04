// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_e2 
(
  input  [`VPU_FCMD_SZ-1:0]                   cmd_f2a_h, 
  input  [`VPU_DTYPE_SZ-1:0]                  op_dtype_f2a_h,
  input  [`TXFMA_SIGS_SZ-1:0]                 sigs_f2a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]          ediff_ec_eph_f2a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]          ediff_ec_epl_f2a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]          ediff_eph_epl_f2a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]          eprod_hi_f2a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]          eprod_lo_f2a_h,
  input  [`TXFMA_EXP_PS_SZ-1:0]               ec_f2a_h,
  input  [`TXFMA_EXP_PS_SZ-1:0]               eb_hi_f2a_h,
  input  [`TXFMA_ETRZ_PS_SZ-1:0]              etrz_fa_lo_ps_f2a_h,
  input  [`TXFMA_ETRZ_PS_SZ-1:0]              etrz_fc_lo_ps_f2a_h,
  input  [`TXFMA_ETRZ_PROD_PH_SZ-1:0]         etrz_prod_hi_ph_f2a_h,
  input  [`TXFMA_ETRZ_PROD_PS_SZ-1:0]         etrz_prod_lo_ps_f2a_h,
  input                                       eprod_hi_zero_f2a_h,
  input                                       eprod_lo_zero_f2a_h,
  input                                       ec_zero_f2a_h,
  input                                       exp_fma2_exc_f2a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]    e_res_f2a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]    exp_frac_e_shift_f2a_h,  
  output logic                                e_sticky1_f2a_h,
  output logic                                e_sticky2_f2a_h,
  output logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  align1_mux_sel_f2a_h,
  output logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  align2_mux_sel_f2a_h,
  output logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  opd3_mux_sel_f2a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]    ediff1_f2a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]    ediff2_f2a_h,
  output logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  ediff1_sel_f2a_h,
  output logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  ediff2_sel_f2a_h,
  output logic                                ediff1_zap_f2a_h
);


logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0] res_exp_hi_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0] res_exp_lo_sel_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   etrz_1_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   etrz_2_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   e_1_tk_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   e_2_tk_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   exp_sticky1_csa_carry_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   exp_sticky1_csa_sum_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   exp_sticky2_csa_carry_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   exp_sticky2_csa_sum_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   e_sticky1_add_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   e_sticky2_add_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   ediff1_mux_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   ediff2_mux_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   ediff1_abs_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   ediff2_abs_f2a_h;
logic                               ediff_gt_31_f2a_h;
logic                               ediff_gte_0_f2a_h;
logic                               ediff_oporder_sign_c_ph_f2a_h;
logic                               ediff_oporder_sign_c_pl_f2a_h;
logic                               ediff_oporder_sign_ph_pl_f2a_h;
logic                               sticky1_clear_f2a_h;
logic                               sticky2_clear_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   exp_sticky2_csa_inb_mux_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   exp_sticky2_csa_cin_mux_f2a_h;


///////////////////////////////////////////////////////////////////////////////
// ediff operand logic 
///////////////////////////////////////////////////////////////////////////////

  //sign of ediff substraction tells about which exponent is greater
  //in case of zero operand, force the sign
assign ediff_oporder_sign_c_ph_f2a_h  = (ediff_ec_eph_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-1]  & !eprod_hi_zero_f2a_h) | ec_zero_f2a_h;
assign ediff_oporder_sign_c_pl_f2a_h  = (ediff_ec_epl_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-1]  & !eprod_lo_zero_f2a_h) | 
                                  (ec_zero_f2a_h & (op_dtype_f2a_h == `VPU_DTYPE_F16_F32));
assign ediff_oporder_sign_ph_pl_f2a_h = (ediff_eph_epl_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-1] & !eprod_lo_zero_f2a_h) | eprod_hi_zero_f2a_h;

txfma_ediff_opdorder_logic
ediff_opdorder_logic
(
  .cmd            ( cmd_f2a_h                      ),
  .op_dtype       ( op_dtype_f2a_h                 ),
  .sigs           ( sigs_f2a_h                     ),
  .sign_c_ph      ( ediff_oporder_sign_c_ph_f2a_h  ),
  .sign_c_pl      ( ediff_oporder_sign_c_pl_f2a_h  ),
  .sign_ph_pl     ( ediff_oporder_sign_ph_pl_f2a_h ),
  .exp_fma2_exc   ( exp_fma2_exc_f2a_h             ),
  .e_res_f2a_h    ( e_res_f2a_h                    ),
  .align1_mux_sel ( align1_mux_sel_f2a_h           ),
  .align2_mux_sel ( align2_mux_sel_f2a_h           ),
  .opd3_mux_sel   ( opd3_mux_sel_f2a_h             ),
  .ediff1_sel     ( ediff1_sel_f2a_h               ),
  .ediff2_sel     ( ediff2_sel_f2a_h               ),
  .res_exp_hi_sel ( res_exp_hi_sel_f2a_h           ),
  .res_exp_lo_sel ( res_exp_lo_sel_f2a_h           )
);



///////////////////////////////////////////////////////////////////////////////
// Exp ediff 1 mux
///////////////////////////////////////////////////////////////////////////////
always_comb begin
  case(ediff1_sel_f2a_h) 
    `TXFMA_SEL_EDIFF_EC_EPH,`TXFMA_SEL_EDIFF_N_EC_EPH:    ediff1_mux_f2a_h = ediff_ec_eph_f2a_h;
    `TXFMA_SEL_EDIFF_N_EC_EPL,`TXFMA_SEL_EDIFF_EC_EPL:    ediff1_mux_f2a_h = ediff_ec_epl_f2a_h;
    `TXFMA_SEL_EDIFF_ZERO:                                ediff1_mux_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h0;
    `TXFMA_SEL_EDIFF_LOG_MUL:                             ediff1_mux_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h7;
    `TXFMA_SEL_EDIFF_LOG_MUL2:                            ediff1_mux_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h0;
    `TXFMA_SEL_EDIFF_NR2_FRCPFXP:                         ediff1_mux_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h0;
    default:                                              ediff1_mux_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h0;
  endcase
end

// For FROUND Instruction, zero out ediff1_f2a_h (Alignment Shift Amount) if the exponent difference
// causes the entire OpC fraction to shift beyond the PS LSB 
//assign ediff_gt_31_f2a_h = $signed(ediff1_mux_f2a_h) > $signed(31);
assign ediff_gt_31_f2a_h = $signed(ediff1_mux_f2a_h) > $signed(31);
assign ediff_gte_0_f2a_h = $signed(ediff1_mux_f2a_h) >= 0;

assign ediff1_zap_f2a_h = ((ediff_gt_31_f2a_h | ediff_gte_0_f2a_h) & (cmd_f2a_h==`VPU_FCMD_FROUND));
assign ediff1_f2a_h     = ediff1_zap_f2a_h ? `TXFMA_EXP_SEXT_PS_SZ'h0 : ediff1_mux_f2a_h;
assign ediff1_abs_f2a_h = ediff1_mux_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-1] ? -ediff1_mux_f2a_h : ediff1_mux_f2a_h;


///////////////////////////////////////////////////////////////////////////////
// Exp ediff 2 mux
///////////////////////////////////////////////////////////////////////////////
always_comb begin
  case(ediff2_sel_f2a_h) 
    `TXFMA_SEL_EDIFF_ECH_EPH,`TXFMA_SEL_EDIFF_N_ECH_EPH:    ediff2_mux_f2a_h = ediff_ec_eph_f2a_h; 
    `TXFMA_SEL_EDIFF_EC_EPL,`TXFMA_SEL_EDIFF_N_EC_EPL:      ediff2_mux_f2a_h = ediff_ec_epl_f2a_h;
    `TXFMA_SEL_EDIFF_EPH_EPL,`TXFMA_SEL_EDIFF_N_EPH_EPL:    ediff2_mux_f2a_h = ediff_eph_epl_f2a_h;
    default:                                                ediff2_mux_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h0;
  endcase
end
assign ediff2_f2a_h     = ediff2_mux_f2a_h;
assign ediff2_abs_f2a_h = ediff2_mux_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-1] ? -ediff2_mux_f2a_h : ediff2_mux_f2a_h;


///////////////////////////////////////////////////////////////////////////////
// Exp trz/align1 mux
///////////////////////////////////////////////////////////////////////////////
always_comb begin
  case(align1_mux_sel_f2a_h)
    `TXFMA_SEL_PH:                etrz_1_f2a_h = {($size(etrz_1_f2a_h) - $size(etrz_prod_hi_ph_f2a_h))'(0), etrz_prod_hi_ph_f2a_h};  
    `TXFMA_SEL_P, `TXFMA_SEL_PL:  etrz_1_f2a_h = {($size(etrz_1_f2a_h) - $size(etrz_prod_hi_ph_f2a_h))'(0), etrz_prod_lo_ps_f2a_h};
    `TXFMA_SEL_C:                 etrz_1_f2a_h = {($size(etrz_1_f2a_h) - $size(etrz_fc_lo_ps_f2a_h))'(0), etrz_fc_lo_ps_f2a_h};
    `TXFMA_SEL_A:                 etrz_1_f2a_h = {($size(etrz_1_f2a_h) - $size(etrz_fa_lo_ps_f2a_h))'(0), etrz_fa_lo_ps_f2a_h};
    default:                      etrz_1_f2a_h = `TXFMA_ETRZ_PS_SZ'h0;
  endcase
end

///////////////////////////////////////////////////////////////////////////////
// Exp trz/align2 mux
///////////////////////////////////////////////////////////////////////////////
always_comb begin
  case(align2_mux_sel_f2a_h)
    `TXFMA_SEL_PH:                etrz_2_f2a_h = {($size(etrz_2_f2a_h) - $size(etrz_prod_hi_ph_f2a_h))'(0), etrz_prod_hi_ph_f2a_h};  
    `TXFMA_SEL_P, `TXFMA_SEL_PL:  etrz_2_f2a_h = {($size(etrz_2_f2a_h) - $size(etrz_prod_lo_ps_f2a_h))'(0), etrz_prod_lo_ps_f2a_h};
    `TXFMA_SEL_C:                 etrz_2_f2a_h = {($size(etrz_2_f2a_h) - $size(etrz_fc_lo_ps_f2a_h))'(0), etrz_fc_lo_ps_f2a_h};
    `TXFMA_SEL_A:                 etrz_2_f2a_h = {($size(etrz_2_f2a_h) - $size(etrz_prod_hi_ph_f2a_h))'(0), etrz_prod_hi_ph_f2a_h};
    default:                      etrz_2_f2a_h = `TXFMA_ETRZ_PS_SZ'h0;
  endcase
end

///////////////////////////////////////////////////////////////////////////////
// exp 1 tk mux
///////////////////////////////////////////////////////////////////////////////
always_comb begin

  case (op_dtype_f2a_h)
    `VPU_DTYPE_INT:               e_1_tk_f2a_h = $unsigned(`TXFMA_F32_A_C_STICKY_DISTANCE);

    `VPU_DTYPE_F32:
                                  if ((align1_mux_sel_f2a_h==`TXFMA_SEL_P) & ~sigs_f2a_h[`TXFMA_NORM_F32])
                                    e_1_tk_f2a_h = $unsigned(`TXFMA_F32_PROD_STICKY_DISTANCE); 
                                  else
                                    e_1_tk_f2a_h = `TXFMA_F32_A_C_STICKY_DISTANCE; 

    `VPU_DTYPE_F16_F32:                
                                  if (align1_mux_sel_f2a_h==`TXFMA_SEL_C) 
                                    e_1_tk_f2a_h = {7'h0, $unsigned(`TXFMA_F16_F32_C_STICKY_DISTANCE)};
                                  else
                                    e_1_tk_f2a_h = {9'h0, $unsigned(`TXFMA_F16_F32_PROD_STICKY_DISTANCE)};
    default :                     e_1_tk_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'b0;
  endcase

  if (cmd_f2a_h == `VPU_FCMD_CVT_F16F32) 
    e_1_tk_f2a_h = `TXFMA_F16_F32_MAN_DIFF;

  if (cmd_f2a_h == `VPU_FCMD_CVT_F11F32)
    e_1_tk_f2a_h = `TXFMA_F11_F32_MAN_DIFF;

  if (cmd_f2a_h == `VPU_FCMD_CVT_F10F32)
    e_1_tk_f2a_h = `TXFMA_F10_F32_MAN_DIFF;

  if (cmd_f2a_h==`VPU_FCMD_FROUND)
    e_1_tk_f2a_h = $unsigned(`TXFMA_INT_STICKY_DISTANCE); 
end

///////////////////////////////////////////////////////////////////////////////
// exp 2 tk mux
///////////////////////////////////////////////////////////////////////////////
always_comb begin
    //tensor phps 
  e_2_tk_f2a_h = `TXFMA_F16_F32_PROD_STICKY_DISTANCE;
 
    //exp frac
  if (cmd_f2a_h==`VPU_TRANS_EXP_FRAC)
    e_2_tk_f2a_h = `TXFMA_TRANS_EXP_STICKY_DISTANCE;
end

///////////////////////////////////////////////////////////////////////////////
// sticky1 csa
///////////////////////////////////////////////////////////////////////////////
txfma_csa #`TXFMA_EXP_SEXT_PS_SZ exp_sticky1_csa
(
 .a    ( e_1_tk_f2a_h                ),
 .b    ( ~ediff1_abs_f2a_h           ),
 .cin  ( etrz_1_f2a_h                ),
 .cout ( exp_sticky1_csa_carry_f2a_h ),
 .sum  ( exp_sticky1_csa_sum_f2a_h   )
);



///////////////////////////////////////////////////////////////////////////////
// sticky2 csa
// adder also used for exp frac 
///////////////////////////////////////////////////////////////////////////////
assign exp_sticky2_csa_inb_mux_f2a_h = (cmd_f2a_h==`VPU_TRANS_EXP_FRAC) ? ~ec_f2a_h
                                     :                                    ~ediff2_abs_f2a_h;
assign exp_sticky2_csa_cin_mux_f2a_h = (cmd_f2a_h==`VPU_TRANS_EXP_FRAC) ? `TXFMA_EXP_SEXT_PS_SZ'b0 
                                     :                                    etrz_2_f2a_h;

txfma_csa #`TXFMA_EXP_SEXT_PS_SZ exp_sticky2_csa
(
 .a    ( e_2_tk_f2a_h                  ),
 .b    ( exp_sticky2_csa_inb_mux_f2a_h ),
 .cin  ( exp_sticky2_csa_cin_mux_f2a_h ),
 .cout ( exp_sticky2_csa_carry_f2a_h   ),
 .sum  ( exp_sticky2_csa_sum_f2a_h     )
);


///////////////////////////////////////////////////////////////////////////////
// sticky1 adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH(`TXFMA_EXP_SEXT_PS_SZ)
)
exp_sticky1_adder
(
  .a    ( exp_sticky1_csa_sum_f2a_h                                      ),
  .b    ( {exp_sticky1_csa_carry_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-2:0], 1'b0} ),
  .cin  ( 1'b1                                                           ),
  .sum  ( e_sticky1_add_f2a_h                                            ) 
);

///////////////////////////////////////////////////////////////////////////////
// sticky2 adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH(`TXFMA_EXP_SEXT_PS_SZ)
)
exp_sticky2_adder
(
  .a    ( exp_sticky2_csa_sum_f2a_h                                      ),
  .b    ( {exp_sticky2_csa_carry_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-2:0], 1'b0} ),
  .cin  ( 1'b1                                                           ),
  .sum  ( e_sticky2_add_f2a_h                                            )  
);


///////////////////////////////////////////////////////////////////////////////
// exp hi resmux
///////////////////////////////////////////////////////////////////////////////
always_comb begin
  case(res_exp_hi_sel_f2a_h)
    `TXFMA_SEL_PH,`TXFMA_SEL_P,`TXFMA_SEL_A:  e_res_f2a_h = eprod_hi_f2a_h;
    `TXFMA_SEL_PL:                            e_res_f2a_h = eprod_lo_f2a_h;
    `TXFMA_SEL_C:                             e_res_f2a_h = ec_f2a_h;
    `TXFMA_SEL_B:                             e_res_f2a_h = eb_hi_f2a_h;
    `TXFMA_SEL_CVT_F32INT:                    e_res_f2a_h = (`FLOAT32_E_BIAS_HEX+`TXFMA_CVT_F32INT_OFFSET);
    `TXFMA_SEL_CVT_F32_FXP1615:               e_res_f2a_h = (`FLOAT32_E_BIAS_HEX+`TXFMA_CVT_F32_FXP1615_OFFSET);
    `TXFMA_SEL_GCVT_F32_NORM:                 e_res_f2a_h = (`FLOAT32_E_BIAS_HEX-`TXFMA_GCVT_F32_NORM_OFFSET);
    `TXFMA_SEL_EXP_FRAC:                      e_res_f2a_h = (`FLOAT32_E_BIAS_HEX+`TXFMA_EXP_FRAC_OFFSET);
    default:                                  e_res_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h0;
  endcase
end


///////////////////////////////////////////////////////////////////////////////
// exp lo resmux
///////////////////////////////////////////////////////////////////////////////
always_comb begin
  case(res_exp_lo_sel_f2a_h)
    `TXFMA_SEL_EXP_FRAC: exp_frac_e_shift_f2a_h = e_sticky2_add_f2a_h;
    default:             exp_frac_e_shift_f2a_h = `TXFMA_EXP_SEXT_PS_SZ'h0;
  endcase
end


///////////////////////////////////////////////////////////////////////////////
// sticky clear select
///////////////////////////////////////////////////////////////////////////////
always_comb begin
  sticky1_clear_f2a_h = 1'b1;
  sticky2_clear_f2a_h = 1'b1;

  if (op_dtype_f2a_h == `VPU_DTYPE_F32)  
    sticky1_clear_f2a_h = eprod_hi_zero_f2a_h | ec_zero_f2a_h & ~(sigs_f2a_h[`TXFMA_NORM_F32] | sigs_f2a_h[`TXFMA_FX_F32]) | ediff1_zap_f2a_h;

  if (op_dtype_f2a_h == `VPU_DTYPE_F16_F32) begin  
      sticky1_clear_f2a_h = (eprod_hi_zero_f2a_h & (align1_mux_sel_f2a_h==`TXFMA_SEL_PH)) | (ec_zero_f2a_h & (align1_mux_sel_f2a_h==`TXFMA_SEL_C));
      sticky2_clear_f2a_h = (eprod_lo_zero_f2a_h & (align2_mux_sel_f2a_h==`TXFMA_SEL_PL)) | (eprod_hi_zero_f2a_h & (align2_mux_sel_f2a_h==`TXFMA_SEL_PH));
  end
end


///////////////////////////////////////////////////////////////////////////////
// sticky clearing in case that al addends are zero
///////////////////////////////////////////////////////////////////////////////
assign e_sticky1_f2a_h = e_sticky1_add_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-1] & !sticky1_clear_f2a_h;
assign e_sticky2_f2a_h = e_sticky2_add_f2a_h[`TXFMA_EXP_SEXT_PS_SZ-1] & !sticky2_clear_f2a_h;



endmodule
