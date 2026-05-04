// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c2
(
  input [`VPU_DTYPE_SZ-1:0]                     op_dtype_f2a_h,
  input [`VPU_FCMD_SZ-1:0]                      cmd_f2a_h,
  input [`TXFMA_SIGS_SZ-1:0]                    sigs_f2a_h,
  input [`TXFMA_RM_SZ-1:0]                      rm_f2a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]             ediff1_f2a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]             ediff1_abs_f2a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]             ediff2_abs_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           ediff1_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           ediff2_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           align1_mux_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           align2_mux_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           opd3_mux_sel_f2a_h,
  input [`TXFMA_EXP_PS_SZ-1:0]                  ec_f2a_h,
  input                                         ec_zero_f2a_h,
  input                                         eprod_hi_zero_f2a_h,
  input                                         fc_b0_f2a_h,
  input                                         e_sticky1_f2a_h,
  input                                         e_sticky2_f2a_h,
  input                                         sc_f2a_h,
  input                                         sel_fcmask_f2a_h,
  input                                         prod_sign_hi_f2a_h, 
  input                                         prod_sign_lo_f2a_h,
  input                                         fc_zero30_8_f2a_h,
  input                                         minmax_op_f2a_h,
  output                                        eff_sub1_f2a_h,
  output                                        eff_sub2_f2a_h,
  output                                        comp_op_f2a_h,
  output                                        ediff1_0_f2a_h,
  output                                        fa_ge_fc_comp_sign_hi_res_f2a_h,
  output                                        fa_eq_fc_comp_sign_hi_res_f2a_h,
  output                                        ediff1_lt_31_f2a_h,
  output                                        cvt_fui_pos_ovf_f2a_h,
  output                                        cvt_fi_pos_ovf_f2a_h,  
  output                                        cvt_fi_neg_ovf_f2a_h,
  output                                        cvt_fi_min_neg_f2a_h,
  output                                        fc_sel_f2a_h,
  output                                        op3p1c_sel_p_f2a_h,
  output                                        op3p1c_sel_r_f2a_h,
  output                                        op3g_sel_f2a_h,
  output                                        rm_rne_f2a_h,
  output                                        rm_rmm_f2a_h,
  output                                        frnd_rupnsc_f2a_h,
  output                                        frnd_rdnsc_f2a_h,
  output                                        frnd_t1_f2a_h,
  output                                        frnd_t2_f2a_h,
  output                                        al1c_zap_f2a_h,
  output                                        op1c_log_mul_clr_f2a_h,
  output                                        negop1c_int_logmul_clr_f2a_h,
  output                                        psc_op1_clr_f2a_h,
  output                                        align2_zap_f2a_h,
  output                                        align2c_neg_f2a_h,
  output                                        trans_log_rr_neg_f2a_h,
  output                                        sin_eff_sub1_f2a_h,
  output                                        al2c3_neg_cin_ps_f2a_h,
  output                                        al1c3_neg_cin_f2a_h,
  output                                        op2_sxtto_g_f2a_h,
  output [1:0]                                  op3p3spc_sel_f2a_h,
  output [2:0]                                  op3p2c_sel_f2a_h,
  output [1:0]                                  ugfxrplc_sel_f2a_h,
  output [1:0]                                  sgfxrplc_sel_f2a_h,
  output [1:0]                                  icgfxc_sel_f2a_h,
  output [1:0]                                  al1znp2c_sel_f2a_h,
  output [1:0]                                  al2c_sel_f2a_h,
  output [1:0]                                  al1p2c_sel_f2a_h,
  output logic                                  sin_ediff2_f2a_h,
  output logic                                  al1p1c_sel_f2a_h,
  output logic                                  sin_align1_neg_f2a_h,
  output logic                                  sin_align2_neg_f2a_h,
  output logic                                  sin_check_fc_sign_f2a_h,
  output logic                                  int_minmax_mux_sel_fc_f2a_h,
  output logic                                  eff_sign1_f2a_h
);


///////////////////////////////////////////////////////////////////////////////
// Internal Logic
///////////////////////////////////////////////////////////////////////////////
logic      fp_sub_f2a_h;
logic      ediff1_gt_m1_f2a_h;
logic      ediff1_gt_0_f2a_h;
logic      ffrc_op_f2a_h;
logic      eff_sign2_f2a_h;
logic      negated_op1_a2a1_clr_f2a_h;
logic      negated_op1_log_mul_clr_f2a_h;
logic      negated_op1_log_mul_a0b5_clr_f2a_h;
logic      align1_neg_f2a_h;
logic      align2_neg_f2a_h;
logic      use_align1_f2a_h;
logic      use_align2_f2a_h;
logic      use_align1_fma_add_f2a_h;
logic      use_align1_trans_f2a_h;
logic      ec_hi_gte_127_f2a_h;
logic      out_of_range_1_f2a_h;
logic      out_of_range_2_f2a_h;
logic      fc_lt_0_f2a_h;
logic      fc_lte_m05_f2a_h;
logic      fc_lte_m075_f2a_h;
logic      fc_lte_m025_f2a_h;
logic      fc_gte_05_f2a_h;
logic      fc_gte_075_f2a_h;
logic      fc_gte_025_f2a_h;
logic      fc_gt_0_f2a_h;
logic      ediff1_gte_m1_f2a_h;
logic      ediff1_gte_m2_f2a_h;
logic      ediff1_lte_0_f2a_h;
logic      eprod_ge_ec_1_f2a_h;
logic      trans_log_mul_align1_neg_f2a_h;
logic      align1_neg_cin_f2a_h;
logic      align2_neg_cin_f2a_h;
logic      align2_neg_cin_ps_f2a_h;

///////////////////////////////////////////////////////////////////////////////
//  FP and INT instructions that contains addition and subtraction
///////////////////////////////////////////////////////////////////////////////

assign fp_sub_f2a_h  = (sigs_f2a_h[`TXFMA_SUB] | sigs_f2a_h[`TXFMA_FMS] ) & (op_dtype_f2a_h == `VPU_DTYPE_F32);

  //ffrc operation
assign ffrc_op_f2a_h = (cmd_f2a_h==`VPU_FCMD_FFRC) | (cmd_f2a_h==`VPU_FCMD_FFRC_SIN);

  // compare operation
assign comp_op_f2a_h   = (cmd_f2a_h==`VPU_FCMD_FLE) | (cmd_f2a_h==`VPU_FCMD_FLT) | (cmd_f2a_h==`VPU_FCMD_FEQ) | (cmd_f2a_h == `VPU_FCMD_CLASS);


///////////////////////////////////////////////////////////////////////////////
// Effective sign
///////////////////////////////////////////////////////////////////////////////

always_comb 
begin
  eff_sign2_f2a_h = 1'b0;
  eff_sign1_f2a_h = 1'b0;  

    // 1
  case(ediff1_sel_f2a_h) 
    `TXFMA_SEL_EDIFF_EC_EPH,`TXFMA_SEL_EDIFF_N_EC_EPH,
    `TXFMA_SEL_EDIFF_NR2_FRCPFXP                        : eff_sign1_f2a_h = prod_sign_hi_f2a_h ^ sc_f2a_h;
    `TXFMA_SEL_EDIFF_EC_EPL,`TXFMA_SEL_EDIFF_N_EC_EPL   : eff_sign1_f2a_h = prod_sign_lo_f2a_h ^ sc_f2a_h;
  endcase
  
    // 2 
  case(ediff2_sel_f2a_h) 
    `TXFMA_SEL_EDIFF_EC_EPL,`TXFMA_SEL_EDIFF_N_EC_EPL   : eff_sign2_f2a_h = prod_sign_lo_f2a_h ^ sc_f2a_h;   
    `TXFMA_SEL_EDIFF_ECH_EPH,`TXFMA_SEL_EDIFF_N_ECH_EPH : eff_sign2_f2a_h = prod_sign_hi_f2a_h ^ sc_f2a_h; 
    `TXFMA_SEL_EDIFF_EPH_EPL,`TXFMA_SEL_EDIFF_N_EPH_EPL : eff_sign2_f2a_h = prod_sign_lo_f2a_h ^ prod_sign_hi_f2a_h;
  endcase
end

///////////////////////////////////////////////////////////////////////////////
// Effective sub
///////////////////////////////////////////////////////////////////////////////

  //1
assign eff_sub1_f2a_h = (op_dtype_f2a_h == `VPU_DTYPE_F32) ? (((eff_sign1_f2a_h ^ fp_sub_f2a_h) & ~(sigs_f2a_h[`TXFMA_CVT] | sigs_f2a_h[`TXFMA_NORM_F32]) & !ffrc_op_f2a_h) | minmax_op_f2a_h | comp_op_f2a_h)
                      :                                      eff_sign1_f2a_h;
  //2
assign eff_sub2_f2a_h = eff_sign2_f2a_h;

  // special cases detection
assign ediff1_gt_m1_f2a_h = $signed(ediff1_f2a_h) > -1;
assign ediff1_0_f2a_h     = $signed(ediff1_f2a_h) == 0;
assign ediff1_gt_0_f2a_h  = $signed(ediff1_f2a_h) > 0;
assign ediff1_lt_31_f2a_h = $signed(ediff1_f2a_h) < $signed(-`TXFMA_CVT_INTF32_OFFSET);

  // special cases detection used at result mux  
assign cvt_fi_pos_ovf_f2a_h  = ~sc_f2a_h & ediff1_gt_m1_f2a_h & (cmd_f2a_h == `VPU_FCMD_CVT_INTF32);
assign cvt_fui_pos_ovf_f2a_h = ~sc_f2a_h & ediff1_gt_0_f2a_h & (cmd_f2a_h == `VPU_FCMD_CVT_UINTF32);
assign cvt_fi_min_neg_f2a_h  = sc_f2a_h & ediff1_gt_m1_f2a_h & (cmd_f2a_h == `VPU_FCMD_CVT_INTF32);
assign cvt_fi_neg_ovf_f2a_h  = cvt_fi_min_neg_f2a_h & ~(ediff1_0_f2a_h & fc_zero30_8_f2a_h) & (cmd_f2a_h == `VPU_FCMD_CVT_INTF32);


///////////////////////////////////////////////////////////////////////////////
// Integer MIN/MAX MUX selector - the remainder of the logic is in txfma_c3 
///////////////////////////////////////////////////////////////////////////////

  //fa greater than fc compare by sign
assign fa_ge_fc_comp_sign_hi_res_f2a_h = (eff_sign1_f2a_h & !prod_sign_hi_f2a_h) & (cmd_f2a_h!=`VPU_TRANS_EXP_FRAC);

  //fa equal to fc compared by sign
assign fa_eq_fc_comp_sign_hi_res_f2a_h = !(prod_sign_hi_f2a_h ^ sc_f2a_h);


///////////////////////////////////////////////////////////////////////////////
//clear A2-A1 during CVT IF + neg (note this logic has been moved from
//txfma_f4 and txfma_c3
///////////////////////////////////////////////////////////////////////////////

assign ediff1_gte_m1_f2a_h  = ($signed(ediff1_f2a_h)>=-1);
assign ediff1_gte_m2_f2a_h  = ($signed(ediff1_f2a_h)>=-2);
assign ediff1_lte_0_f2a_h   = ($signed(ediff1_f2a_h)<=0);


///////////////////////////////////////////////////////////////////////////////
// sin transform compare logic
///////////////////////////////////////////////////////////////////////////////

  //fc<0
assign fc_lt_0_f2a_h = sc_f2a_h & !ec_zero_f2a_h;
  //fc<=-0.5
assign fc_lte_m05_f2a_h = sc_f2a_h & ediff1_gte_m1_f2a_h;
  //fc<=-0.75
assign fc_lte_m075_f2a_h = sc_f2a_h & ediff1_gte_m1_f2a_h & fc_b0_f2a_h;
  //fc<=-0.25
assign fc_lte_m025_f2a_h = sc_f2a_h & ediff1_gte_m2_f2a_h;
  //fc>=0.5
assign fc_gte_05_f2a_h = !sc_f2a_h & ediff1_gte_m1_f2a_h;
  //fc>=0.75
assign fc_gte_075_f2a_h = !sc_f2a_h & ediff1_gte_m1_f2a_h & fc_b0_f2a_h;
  //fc>=0.25
assign fc_gte_025_f2a_h = !sc_f2a_h & ediff1_gte_m2_f2a_h;
  //fc>0
assign fc_gt_0_f2a_h = !sc_f2a_h & ediff1_lte_0_f2a_h & !ec_zero_f2a_h;

  //sin eff sub
assign sin_eff_sub1_f2a_h = sin_align1_neg_f2a_h ^ sin_align2_neg_f2a_h;

always_comb 
begin
    //I Quartile 
  sin_align1_neg_f2a_h   = 1'b0;
  sin_align2_neg_f2a_h   = 1'b0;
  sin_ediff2_f2a_h       = 1'b0;
  int_minmax_mux_sel_fc_f2a_h = 1'b0;
  sin_check_fc_sign_f2a_h = 1'b0;

    //trans sin transform
  if (cmd_f2a_h==`VPU_TRANS_SIN_TRANSFORM) begin
      //IV Quartile: fc < 0 && fc <= -0.5 && fc <= -0.75; res = fc+1  
    if (fc_lt_0_f2a_h && fc_lte_m05_f2a_h && fc_lte_m075_f2a_h) begin
      sin_align1_neg_f2a_h = sc_f2a_h;
      //III Quartile: fc < 0 && fc <= -0.5; res = -0.5-fc     
    end else if (fc_lt_0_f2a_h && fc_lte_m05_f2a_h) begin
      sin_align1_neg_f2a_h = !sc_f2a_h;
      sin_align2_neg_f2a_h = 1'b1;
      sin_check_fc_sign_f2a_h = 1'b1;
      sin_ediff2_f2a_h = 1'b1;      
      //II Quartile: fc < 0 && fc <= -0.25; res = -0.5-fc
    end else if (fc_lt_0_f2a_h && fc_lte_m025_f2a_h) begin
      sin_align1_neg_f2a_h = !sc_f2a_h;
      sin_align2_neg_f2a_h = 1'b1;      
      sin_ediff2_f2a_h = 1'b1;            
    end else if (!fc_lt_0_f2a_h) begin
        //IV Quartile: fc > 0 && fc >= 0.5 && fc >= 0.75; res = fc-1
      if (fc_gt_0_f2a_h && fc_gte_05_f2a_h && fc_gte_075_f2a_h) begin
        sin_align1_neg_f2a_h = sc_f2a_h;
        sin_align2_neg_f2a_h = 1'b1;        
        //III Quartile: fc > 0 && fc >= 0.5; res = 0.5-fc
      end else if (fc_gt_0_f2a_h && fc_gte_05_f2a_h) begin
        sin_check_fc_sign_f2a_h = 1'b1;
        sin_align1_neg_f2a_h = !sc_f2a_h; 
        sin_ediff2_f2a_h = 1'b1;              
      end
        //II Quartile: fc > 0 && fc >= 0.25; res = 0.5-fc
      else if (fc_gt_0_f2a_h && fc_gte_025_f2a_h) begin
        sin_align1_neg_f2a_h = !sc_f2a_h;   
        sin_ediff2_f2a_h = 1'b1;      
        //I Quartile   
      end 
      else begin
        int_minmax_mux_sel_fc_f2a_h = 1'b1;
      end
      //I Quartile    
    end else begin
      int_minmax_mux_sel_fc_f2a_h = 1'b1;
    end
  end
end

assign ec_hi_gte_127_f2a_h = (ec_f2a_h >= 127);
assign eprod_ge_ec_1_f2a_h  = ($signed(ediff1_f2a_h)<0);

  //fc mask selection
assign fc_sel_f2a_h = ffrc_op_f2a_h & eprod_ge_ec_1_f2a_h | int_minmax_mux_sel_fc_f2a_h;
 
  //trans log rr negate
assign trans_log_rr_neg_f2a_h = fc_b0_f2a_h & (cmd_f2a_h==`VPU_TRANS_LOG_RR);

  //trans log mul, neg if ec>127 ^ fc!=0
assign trans_log_mul_align1_neg_f2a_h = (cmd_f2a_h==`VPU_TRANS_LOG_MUL) & !(!ec_hi_gte_127_f2a_h ^ !fc_b0_f2a_h);


///////////////////////////////////////////////////////////////////////////////
// Alignment Shifter control logic
///////////////////////////////////////////////////////////////////////////////

assign use_align1_fma_add_f2a_h  = sigs_f2a_h[`TXFMA_FMA] | sigs_f2a_h[`TXFMA_FMS] | sigs_f2a_h[`TXFMA_ADD] | sigs_f2a_h[`TXFMA_SUB];
assign use_align1_trans_f2a_h    = sigs_f2a_h[`TXFMA_TRANS1] | sigs_f2a_h[`TXFMA_TRANS2] | (cmd_f2a_h==`VPU_TRANS_SIN_TRANSFORM)  
                                 | (cmd_f2a_h==`VPU_TRANS_EXP_FRAC) | (cmd_f2a_h==`VPU_TRANS_LOG_RR);

assign use_align1_f2a_h = use_align1_fma_add_f2a_h | use_align1_trans_f2a_h | sigs_f2a_h[`TXFMA_CVT] | sigs_f2a_h[`TXFMA_NORM_F32] | ffrc_op_f2a_h;

assign use_align2_f2a_h = op_dtype_f2a_h == `VPU_DTYPE_F16_F32 | (cmd_f2a_h==`VPU_TRANS_SIN_TRANSFORM);                        


//  Negate in case of effective SUB and integer SUB (if prod is not zero), negative int to float convert or min/max operation
assign align1_neg_f2a_h = ((eff_sub1_f2a_h | (sc_f2a_h & (cmd_f2a_h==`VPU_FCMD_CVT_F32INT | cmd_f2a_h==`VPU_FCMD_CVT_F32RAST))) & use_align1_f2a_h
                           | trans_log_rr_neg_f2a_h | sin_align1_neg_f2a_h | trans_log_mul_align1_neg_f2a_h) & (cmd_f2a_h!=`VPU_TRANS_EXP_FRAC);

assign align2_neg_f2a_h = eff_sub2_f2a_h & use_align2_f2a_h | sin_align2_neg_f2a_h;

  // Aligner Shifter Cin
assign align1_neg_cin_f2a_h = align1_neg_f2a_h & ~e_sticky1_f2a_h | trans_log_mul_align1_neg_f2a_h;
assign align2_neg_cin_f2a_h = align2_neg_f2a_h & ~e_sticky2_f2a_h;
assign align2_neg_cin_ps_f2a_h = align2_neg_cin_f2a_h  & (op_dtype_f2a_h == `VPU_DTYPE_F16_F32);

assign al2c3_neg_cin_ps_f2a_h = align2_neg_cin_ps_f2a_h;
assign al1c3_neg_cin_f2a_h    = align1_neg_cin_f2a_h;

//op2 sign extend to g bit
assign op2_sxtto_g_f2a_h = align2_neg_f2a_h & (cmd_f2a_h==`VPU_TRANS_SIN_TRANSFORM);


///////////////////////////////////////////////////////////////////////////////
// Clear A2-A1 during CVT IF + Negation
///////////////////////////////////////////////////////////////////////////////

assign negated_op1_a2a1_clr_f2a_h    = ((cmd_f2a_h == `VPU_FCMD_CVT_F32INT) | ((cmd_f2a_h == `VPU_FCMD_CVT_F32RAST)) & align1_neg_f2a_h);
assign negated_op1_log_mul_clr_f2a_h = ((cmd_f2a_h == `VPU_TRANS_LOG_MUL) & (!eprod_hi_zero_f2a_h | ec_hi_gte_127_f2a_h));
assign negated_op1_log_mul_a0b5_clr_f2a_h = ((cmd_f2a_h == `VPU_TRANS_LOG_MUL) & !eprod_hi_zero_f2a_h & (ediff1_abs_f2a_h!=0));


///////////////////////////////////////////////////////////////////////////////
// The following signals are driven directly to txfma_f3 where they are
// flopped and used locally. This is done to meet critical timing. 
///////////////////////////////////////////////////////////////////////////////

  assign op3p3spc_sel_f2a_h[1] = (opd3_mux_sel_f2a_h == `TXFMA_SEL_A) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_C);
  assign op3p3spc_sel_f2a_h[0] = (opd3_mux_sel_f2a_h == `TXFMA_SEL_EXP_RES) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_C);

  assign op3p2c_sel_f2a_h[2]   = (opd3_mux_sel_f2a_h == `TXFMA_SEL_P_INT);
  assign op3p2c_sel_f2a_h[1]   = (opd3_mux_sel_f2a_h == `TXFMA_SEL_P) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_PH) | sel_fcmask_f2a_h | (opd3_mux_sel_f2a_h == `TXFMA_SEL_P_INT);
  assign op3p2c_sel_f2a_h[0]   = (opd3_mux_sel_f2a_h == `TXFMA_SEL_PL) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_P) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_PH) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_P_INT);

  assign op3p1c_sel_r_f2a_h    = (opd3_mux_sel_f2a_h == `TXFMA_SEL_ROUND) & ~ec_zero_f2a_h;
  assign op3p1c_sel_p_f2a_h    = (opd3_mux_sel_f2a_h == `TXFMA_SEL_P) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_P_INT);
  assign op3g_sel_f2a_h        = (opd3_mux_sel_f2a_h == `TXFMA_SEL_PH) | (opd3_mux_sel_f2a_h == `TXFMA_SEL_PL) | ((opd3_mux_sel_f2a_h == `TXFMA_SEL_C) & (op_dtype_f2a_h==`VPU_DTYPE_F16_F32));

  assign ugfxrplc_sel_f2a_h[1] = ((cmd_f2a_h == `VPU_FCMD_CVT_F32UN16) | (cmd_f2a_h == `VPU_FCMD_CVT_F32UN24));
  assign ugfxrplc_sel_f2a_h[0] = ((cmd_f2a_h == `VPU_FCMD_CVT_F32UN10) | (cmd_f2a_h == `VPU_FCMD_CVT_F32UN24));

  assign sgfxrplc_sel_f2a_h[1] = (cmd_f2a_h == `VPU_FCMD_CVT_F32UN2);
  assign sgfxrplc_sel_f2a_h[0] = (cmd_f2a_h == `VPU_FCMD_CVT_F32SN16);

  assign icgfxc_sel_f2a_h[1]   = ((cmd_f2a_h == `VPU_FCMD_CVT_F32UN16) | (cmd_f2a_h == `VPU_FCMD_CVT_F32UN8) | (cmd_f2a_h == `VPU_FCMD_CVT_F32UN10) | (cmd_f2a_h == `VPU_FCMD_CVT_F32UN24));
  assign icgfxc_sel_f2a_h[0]   = ((cmd_f2a_h == `VPU_FCMD_CVT_F32SN16) | (cmd_f2a_h == `VPU_FCMD_CVT_F32SN8) | (cmd_f2a_h == `VPU_FCMD_CVT_F32UN2));

always_comb
begin
  if (align1_mux_sel_f2a_h == `TXFMA_SEL_P)
    al1p1c_sel_f2a_h = 1'b1;
  else
    al1p1c_sel_f2a_h = 1'b0;
end

  assign al1p2c_sel_f2a_h[1]   = ((align1_mux_sel_f2a_h == `TXFMA_SEL_P) | (align1_mux_sel_f2a_h == `TXFMA_SEL_PH));
  assign al1p2c_sel_f2a_h[0]   = (align1_mux_sel_f2a_h == `TXFMA_SEL_A); 
  assign al1znp2c_sel_f2a_h[1] = align1_neg_f2a_h;
  assign al1znp2c_sel_f2a_h[0] = (~align1_neg_f2a_h & ~sigs_f2a_h[`TXFMA_F32_NORM]) & !ffrc_op_f2a_h;

  assign al2c_sel_f2a_h[1]     = ((align2_mux_sel_f2a_h == `TXFMA_SEL_PH) | (align2_mux_sel_f2a_h == `TXFMA_SEL_PL));
  assign al2c_sel_f2a_h[0]     = ((align2_mux_sel_f2a_h == `TXFMA_SEL_PH) | (align2_mux_sel_f2a_h == `TXFMA_SEL_A));

  assign rm_rne_f2a_h          = (rm_f2a_h == RNE);
  assign rm_rmm_f2a_h          = (rm_f2a_h == RMM);
  assign frnd_rupnsc_f2a_h     = (rm_f2a_h == RUP) & ~sc_f2a_h;
  assign frnd_rdnsc_f2a_h      = (rm_f2a_h == RDN) & sc_f2a_h;
  assign frnd_t1_f2a_h         = (rm_f2a_h == RNE) & e_sticky1_f2a_h;
  assign frnd_t2_f2a_h         = ((rm_f2a_h == RDN) & e_sticky1_f2a_h & sc_f2a_h) | ((rm_f2a_h==RUP) & e_sticky1_f2a_h & ~sc_f2a_h);

  assign al1c_zap_f2a_h               = out_of_range_1_f2a_h | !use_align1_f2a_h; 
  assign op1c_log_mul_clr_f2a_h       = negated_op1_log_mul_a0b5_clr_f2a_h;
  assign negop1c_int_logmul_clr_f2a_h = negated_op1_a2a1_clr_f2a_h | negated_op1_log_mul_clr_f2a_h;
  assign psc_op1_clr_f2a_h            = (op_dtype_f2a_h != `VPU_DTYPE_F16_F32);
  assign align2_zap_f2a_h             = out_of_range_2_f2a_h | !use_align2_f2a_h;
  assign align2c_neg_f2a_h            = (eff_sub2_f2a_h & use_align2_f2a_h) | sin_align2_neg_f2a_h;


  // out of range signals used to zap operands
always_comb 
begin
  if (op_dtype_f2a_h==`VPU_DTYPE_F32) begin
    out_of_range_1_f2a_h = (ediff1_abs_f2a_h>`TXFMA_F32_EDIFF_OUT_OF_RANGE);

    if (cmd_f2a_h == `VPU_FCMD_FROUND)
      out_of_range_1_f2a_h = (ediff1_abs_f2a_h>`TXFMA_EDIFF_OUT_OF_RANGE_CVT_F32INT);
 
  end
  else if (op_dtype_f2a_h==`VPU_DTYPE_F16_F32) begin
    //ph goes to align 1
    if (opd3_mux_sel_f2a_h==`TXFMA_SEL_C)
      out_of_range_1_f2a_h = (ediff1_abs_f2a_h>`TXFMA_PH_PS_P_EDIFF_OUT_OF_RANGE);
      //c goes to align 1
    else
      out_of_range_1_f2a_h = (ediff1_abs_f2a_h>`TXFMA_PH_PS_C_EDIFF_OUT_OF_RANGE);
  end
    //other cases
  else begin
    out_of_range_1_f2a_h = (ediff1_abs_f2a_h>`TXFMA_PH_EDIFF_OUT_OF_RANGE);        
  end
end

assign out_of_range_2_f2a_h = (ediff2_abs_f2a_h>`TXFMA_PH_EDIFF_OUT_OF_RANGE);

endmodule 
