// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f4
#(
    parameter left_bit_in_hi    = `TXFMA_A3,
    parameter left_bit_out_hi   = `TXFMA_A0,
    parameter right_bit_in_hi   = `TXFMA_B47,
    parameter right_bit_out_hi  = `TXFMA_B31
)
(
  input [`VPU_DTYPE_SZ-1:0]                     op_dtype_f4a_h,
  input [`VPU_FCMD_SZ-1:0]                      cmd_f4a_h,
  input [`TXFMA_MERGE_MUX_SEL_SZ-1:0]           mergd_mux_sel_f4a_h,
  input                                         neg_norm_f4a_h,
  input [31:0]                                  int_minmax_res_f4a_h,
  input [`TXFMA_A3:`TXFMA_B47]                  comb_add_f4a_h,
  input                                         sign_hi_f4a_h,
  input [`TXFMA_SIGS_SZ-1:0]                    sigs_f4a_h,      
  input [`TXFMA_RM_SZ-1:0]                      rm_f4a_h,  
  input                                         rm_clr_f4a_h,
  input                                         rm_rne_f4a_h,
  input                                         rm_rmm_f4a_h,
  input                                         e_sticky1_until_r_f4a_h,
  input                                         e_sticky1_until_g_f4a_h,
  input                                         fround_sticky_until_l_f4a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]             norm_shfamnt_f4a_h,
  output logic [`TXFMA_RND_CTL_SZ-1:0]          rnd_ctl_f4a_h,      
  output logic [`TXFMA_A0:`TXFMA_B31]           mergd_frac_res_f4a_h,
  output logic [`TXFMA_A0:`TXFMA_B0]            norm_shift_out_neg_a0b0_f4a_h,
  output logic [`TXFMA_FTZ_SZ-1:0]              ftz_f4a_h,
  output logic                                  sticky_until_l_f4a_h
);
 

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [`TXFMA_FTZ_HI_SZ-1:0]      ftz_hi_f4a_h;
logic [`TXFMA_RND_CTL_HI_SZ-1:0]  rnd_ctl_hi_f4a_h;      
logic                             sticky_until_r_f4a_h;
logic [`TXFMA_A3:`TXFMA_B47]      comb_add_cleared_f4a_h;
logic [`TXFMA_A0:`TXFMA_B31]      norm_shift_out_f4a_h;
logic [`TXFMA_A0:`TXFMA_B31]      norm_shift_out_neg_f4a_h;
logic                             ph_ps_clear_f4a_l;
logic                             ph_ps_sxt_f4a_h;


///////////////////////////////////////////////////////////////////////////////
// Comb add negate
///////////////////////////////////////////////////////////////////////////////
assign ph_ps_clear_f4a_l = (op_dtype_f4a_h != `VPU_DTYPE_F16_F32);
assign ph_ps_sxt_f4a_h = (op_dtype_f4a_h== `VPU_DTYPE_F16_F32) & e_sticky1_until_g_f4a_h & neg_norm_f4a_h;


// Clear low part for norm shifter fround and fma ph ps (not norm)
assign comb_add_cleared_f4a_h[`TXFMA_A3:`TXFMA_B24] = comb_add_f4a_h[`TXFMA_A3:`TXFMA_B24];
assign comb_add_cleared_f4a_h[`TXFMA_B25:`TXFMA_B30] = {(`TXFMA_B25-`TXFMA_B30+1){ph_ps_clear_f4a_l}} & comb_add_f4a_h[`TXFMA_B25:`TXFMA_B30] 
                                                     | {(`TXFMA_B25-`TXFMA_B30+1){ph_ps_sxt_f4a_h}}; 
assign comb_add_cleared_f4a_h[`TXFMA_B31:`TXFMA_B47] = {(`TXFMA_B31-`TXFMA_B47+1){((cmd_f4a_h != `VPU_FCMD_FROUND) && ph_ps_clear_f4a_l )}} & comb_add_f4a_h[`TXFMA_B31:`TXFMA_B47]
                                                     | {(`TXFMA_B31-`TXFMA_B47+1){ph_ps_sxt_f4a_h}};


///////////////////////////////////////////////////////////////////////////////
// normalization shifter high
///////////////////////////////////////////////////////////////////////////////

txfma_norm_shf 
#(
  .left_bit_in    (left_bit_in_hi),
  .left_bit_out   (left_bit_out_hi),  
  .right_bit_in   (right_bit_in_hi),
  .right_bit_out  (right_bit_out_hi),
  .shift_size     (`TXFMA_EXP_SEXT_PS_SZ)
)
norm_shf_hi
(
  .op      (comb_add_cleared_f4a_h),
  .shfamnt (norm_shfamnt_f4a_h),
  .op_norm (norm_shift_out_f4a_h)
);

///////////////////////////////////////////////////////////////////////////////
// Normalziation out high negate
///////////////////////////////////////////////////////////////////////////////
assign norm_shift_out_neg_f4a_h = norm_shift_out_f4a_h ^ {$bits(norm_shift_out_f4a_h){neg_norm_f4a_h}};

///////////////////////////////////////////////////////////////////////////////
// merging mux
///////////////////////////////////////////////////////////////////////////////
always_comb
begin

  mergd_frac_res_f4a_h = '0;

  case (mergd_mux_sel_f4a_h)
    `TXFMA_MERGE_MUX_SEL_INTH:            mergd_frac_res_f4a_h = {int_minmax_res_f4a_h, 1'b0};
    `TXFMA_MERGE_MUX_SEL_INTL:            mergd_frac_res_f4a_h = {int_minmax_res_f4a_h[31:0], 1'b0};
    `TXFMA_MERGE_MUX_SEL_NORM_TRANS:      mergd_frac_res_f4a_h = norm_shift_out_neg_f4a_h[`TXFMA_A0:`TXFMA_B31];
    `TXFMA_MERGE_MUX_SEL_NORM:            mergd_frac_res_f4a_h = norm_shift_out_neg_f4a_h;
    `TXFMA_MERGE_MUX_SEL_NORM_NSHC,
    `TXFMA_MERGE_MUX_SEL_NORM_LOG_RR:     mergd_frac_res_f4a_h = {norm_shift_out_neg_f4a_h[`TXFMA_B0:`TXFMA_B31], 1'b0};
    `TXFMA_MERGE_MUX_SEL_COMP_TRUE:       mergd_frac_res_f4a_h = {24'hffffff, 9'b0};
    `TXFMA_MERGE_MUX_SEL_COMP_FALSE:      mergd_frac_res_f4a_h = {24'h0, 9'b0};
  endcase
end


///////////////////////////////
// sticky muxing
///////////////////////////////

assign sticky_until_r_f4a_h = e_sticky1_until_r_f4a_h | sigs_f4a_h[`TXFMA_F32_NORM];


/////////////////////////////
// Round Control Generator High
/////////////////////////////
txfma_rnd_ctl_gen_hi rnd_ctl_gen_hi
( 
  .fround_sticky_until_l        ( fround_sticky_until_l_f4a_h          ),
  .round_bit_ps                 ( mergd_frac_res_f4a_h[`TXFMA_PS_R]    ),
  .round_bit_ph                 ( mergd_frac_res_f4a_h[`TXFMA_PH_HI_R] ),  
  .round_bit_int                ( mergd_frac_res_f4a_h[`TXFMA_INT_R]   ),
  .l_bit_ps                     ( mergd_frac_res_f4a_h[`TXFMA_PS_L]    ),  
  .l_bit_ph                     ( mergd_frac_res_f4a_h[`TXFMA_PH_HI_L] ),  
  .l_bit_int                    ( mergd_frac_res_f4a_h[`TXFMA_INT_L]   ),  
  .neg_norm                     ( neg_norm_f4a_h                       ),
  .res_sign_hi                  ( sign_hi_f4a_h                        ),
  .rm                           ( rm_f4a_h                             ),
  .rm_clr                       ( rm_clr_f4a_h                         ),
  .rm_rne                       ( rm_rne_f4a_h                         ),
  .rm_rmm                       ( rm_rmm_f4a_h                         ),
  .sticky_until_r               ( sticky_until_r_f4a_h                 ), 
  .op_dtype                     ( op_dtype_f4a_h                       ),
  .cmd                          ( cmd_f4a_h                            ),
  .sigs                         ( sigs_f4a_h                           ),
  .ftz_hi                       ( ftz_hi_f4a_h                         ),
  .rnd_ctl_hi                   ( rnd_ctl_hi_f4a_h                     ),
  .sticky_until_l               ( sticky_until_l_f4a_h                 )
);

  //round control
assign rnd_ctl_f4a_h = {rnd_ctl_hi_f4a_h[5:4], rnd_ctl_hi_f4a_h[3:0]};
  //force to zero
assign ftz_f4a_h     = {ftz_hi_f4a_h[2],ftz_hi_f4a_h[1:0]};


///////////////////////////////////////////////////////////////////////////////
// output assignation 
///////////////////////////////////////////////////////////////////////////////
assign norm_shift_out_neg_a0b0_f4a_h = norm_shift_out_neg_f4a_h[`TXFMA_A0:`TXFMA_B0];


endmodule
