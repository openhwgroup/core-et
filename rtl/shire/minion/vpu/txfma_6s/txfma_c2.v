// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c2
(
  input  [`VPU_DTYPE_SZ-1:0]          op_dtype_f2a_h,
  input  [`VPU_FCMD_SZ-1:0]           cmd_f2a_h,
  input  [`TXFMA_SIGS_SZ-1:0]         sigs_f2a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]  ediff1_f2a_h,
  input                               sc_f2a_h,
  input                               prod_sign_hi_f2a_h, 
  input                               prod_sign_lo_f2a_h,
  input                               fc_zero30_8_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0] ediff1_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0] ediff2_sel_f2a_h,
  output                              eff_sub1_f2a_h,
  output                              eff_sub2_f2a_h,
  output                              comp_op_f2a_h,
  output                              minmax_op_f2a_h,
  output                              ediff1_lt_31_f2a_h,
  output                              cvt_fui_pos_ovf_f2a_h,
  output                              cvt_fi_pos_ovf_f2a_h,  
  output                              cvt_fi_neg_ovf_f2a_h,
  output                              cvt_fi_min_neg_f2a_h,
  output                              fc_neg_f2a_h,
  output                              use_product_f2a_h,
  output logic                        eff_sign1_f2a_h
);


  //internal logic
logic fp_sub_f2a_h;
logic ediff1_gt_m1_f2a_h;
logic ediff1_0_f2a_h;
logic ediff1_gt_0_f2a_h;
logic ffrc_op_f2a_h;
logic eff_sign2_f2a_h;


//  FP instructions that contains addition and subtraction
assign fp_sub_f2a_h  = (sigs_f2a_h[`TXFMA_SUB] | sigs_f2a_h[`TXFMA_FMS] ) & (op_dtype_f2a_h == `VPU_DTYPE_F32);

  // min/max operation
assign minmax_op_f2a_h = (cmd_f2a_h==`VPU_FCMD_MIN) | (cmd_f2a_h==`VPU_FCMD_MAX);

  //ffrc operation
assign ffrc_op_f2a_h = (cmd_f2a_h==`VPU_FCMD_FFRC) | (cmd_f2a_h==`VPU_FCMD_FFRC_SIN);

  // compare operation
assign comp_op_f2a_h   = (cmd_f2a_h==`VPU_FCMD_FLE) | (cmd_f2a_h==`VPU_FCMD_FLT) | (cmd_f2a_h==`VPU_FCMD_FEQ) | (cmd_f2a_h == `VPU_FCMD_CLASS);

  // Use product CSA
assign use_product_f2a_h = sigs_f2a_h[`TXFMA_FMA] | sigs_f2a_h[`TXFMA_FMS] | sigs_f2a_h[`TXFMA_MUL] | sigs_f2a_h[`TXFMA_NORM_F32];


///////////////////////////////////////////////////////////////////////////////
// Effective sign
///////////////////////////////////////////////////////////////////////////////
always_comb begin
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


  //prod adder cin
assign fc_neg_f2a_h = minmax_op_f2a_h | comp_op_f2a_h | (sigs_f2a_h[`TXFMA_SGCVT] & sigs_f2a_h[`TXFMA_F32_NORM] & sc_f2a_h) | (cmd_f2a_h==`VPU_TRANS_EXP_FRAC);

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

endmodule 
