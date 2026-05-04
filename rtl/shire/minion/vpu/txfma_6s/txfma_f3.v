// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f3 
(
  input         [`VPU_DTYPE_SZ-1:0]            op_dtype_f3a_h, 
  input         [`VPU_FCMD_SZ-1:0]             cmd_f3a_h,
  input                                        align1_zap_f3a_h,
  input                                        align2_zap_f3a_h,
  input                                        align1_neg_f3a_h,
  input                                        align2_neg_f3a_h,
  input                                        op2_sxtto_g_f3a_h,
  input         [`TXFMA_MINMAX_OPD_SEL_SZ-1:0] int_minmax_mux_sel_f3a_h,
  input         [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  opd3_mux_sel_f3a_h,
  input                                        lxd_sub_f3a_h,
  input         [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  align1_mux_sel_f3a_h,
  input         [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]  align2_mux_sel_f3a_h,
  input         [`TXFMA_EXP_SEXT_PS_SZ-1:0]    ediff1_abs_f3a_h,
  input         [`TXFMA_EXP_SEXT_PH_SZ-1:0]    ediff2_abs_f3a_h,
  input         [`TXFMA_A0:`TXFMA_B22]         fa_f3a_h,
  input         [`TXFMA_A0:`TXFMA_B30]         fc_f3a_h,
  input         [`TXFMA_A3:0]                  prodsum_f3a_h,
  input         [`TXFMA_SIGS_SZ-1:0]           sigs_f3a_h,
  input                                        align1_neg_cin_f3a_h,
  input                                        align2_neg_cin_f3a_h,
  input         [`TXFMA_EXP_SEXT_PS_SZ-1:0]    e_res_f3a_h,  
  input                                        fround_l_f3a_h,
  input                                        eprod_hi_zero_f3a_h,
  output logic  [`TXFMA_LXD_PS_SZ-1:0]         lxd_f3a_h,
  output logic  [`TXFMA_WSPH_ETRZ_SZ-1:0]      comb_add_hi_ph_trz_f3a_h,
  output logic  [`TXFMA_WSPS_ETRZ_SZ-1:0]      comb_add_lo_ps_trz_f3a_h,
  output logic  [31:0]                         int_minmax_res_f3a_h,
  output logic  [`TXFMA_A3:`TXFMA_B47]         comb_add_f3a_h,
  output logic                                 aligned_48_int_r_f3a_h,
  output logic                                 aligned_48_int_l_f3a_h
);


////////////////////////
// Internal Signals 
////////////////////////
logic [`TXFMA_A1:`TXFMA_B45]      align_in_48_f3a_h;
logic [`TXFMA_A1:`TXFMA_B22]      align_in_25_f3a_h;
logic [`TXFMA_A1:`TXFMA_B47]      aligned_50_f3a_h;
logic [`TXFMA_A1:`TXFMA_B24]      aligned_27_f3a_h;
logic [`TXFMA_A0:`TXFMA_B22]      fc_mask_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      csa_in1_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      csa_in2_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      csa_in3_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      S_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      C_f3a_h;
logic [`TXFMA_A1:`TXFMA_B47]      zapped_op1_f3a_h;
logic [`TXFMA_A1:`TXFMA_B24]      zapped_op2_f3a_h;
logic [`TXFMA_A1:`TXFMA_B47]      ps_op1_clear_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      opd3_f3a_h;
logic [`TXFMA_A2:`TXFMA_B47]      negated_op1_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      negated_op1_ext_f3a_h;
logic [`TXFMA_A3:`TXFMA_B45]      gfx_rpl_out_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      align1_gfx_f3a_h;
logic [`TXFMA_A2:`TXFMA_B24]      negated_op2_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]      negated_op2_ext_f3a_h;
logic                             negated_op1_a2a1_clr_f3a_h;
logic [`TXFMA_A2:`TXFMA_B47]      negated_op1_clr_f3a_h;
logic [`TXFMA_A2:`TXFMA_B47]      negated_op1_log_mul_clr_f3a_h;

  //clear A2-A1 during CVT IF + neg
assign negated_op1_a2a1_clr_f3a_h = ((cmd_f3a_h == `VPU_FCMD_CVT_F32INT) | (cmd_f3a_h == `VPU_FCMD_CVT_F32RAST)) & align1_neg_f3a_h;

  //datapath clear for log mul op
always_comb begin
  negated_op1_log_mul_clr_f3a_h = {{(`TXFMA_A2 - `TXFMA_B6){(cmd_f3a_h != `VPU_TRANS_LOG_MUL) || ((cmd_f3a_h == `VPU_TRANS_LOG_MUL) & ((ediff1_abs_f3a_h==0) || eprod_hi_zero_f3a_h))}}, {{(`TXFMA_B6 - `TXFMA_B47 + 1){1'b1}}}};
  negated_op1_log_mul_clr_f3a_h[`TXFMA_A2:`TXFMA_A1] &= {2{(~((cmd_f3a_h == `VPU_TRANS_LOG_MUL) & (ediff1_abs_f3a_h==0)))}};
end

  //datapath clear for tensor ops
assign negated_op1_clr_f3a_h      = {{2{!negated_op1_a2a1_clr_f3a_h}}, {{(`TXFMA_A0 - `TXFMA_B47 + 1){1'b1}}}};
assign ps_op1_clear_f3a_h = {{(`TXFMA_A1-`TXFMA_B25){1'b1}}, {(`TXFMA_B25-`TXFMA_B47+1){(op_dtype_f3a_h != `VPU_DTYPE_F16_F32)}}};


///////////////////////////////////////////////////////////////////////////////
// 48bit Align High Mux 1
///////////////////////////////////////////////////////////////////////////////
always_comb
begin  
  case (align1_mux_sel_f3a_h)
    `TXFMA_SEL_PH:    align_in_48_f3a_h = {prodsum_f3a_h[`TXFMA_A1:`TXFMA_B19], 26'b0}; 
    `TXFMA_SEL_C:     align_in_48_f3a_h = {1'b0, fc_f3a_h[`TXFMA_A0:`TXFMA_B22], 23'b0};
    `TXFMA_SEL_C_INT: align_in_48_f3a_h = {1'b0, fc_f3a_h[`TXFMA_A0:`TXFMA_B30], 15'b0};
    `TXFMA_SEL_A:     align_in_48_f3a_h = {1'b0, fa_f3a_h[`TXFMA_A0:`TXFMA_B22], 23'b0};
    `TXFMA_SEL_P:     align_in_48_f3a_h = prodsum_f3a_h[`TXFMA_A1:`TXFMA_B45]; 
    default:          align_in_48_f3a_h = 48'b0;
  endcase     
end


///////////////////////////////////////////////////////////////////////////////
// 48 bit Alignment shifter
///////////////////////////////////////////////////////////////////////////////
txfma_align_shf
#(.left_bit  (`TXFMA_A1               ), 
  .right_bit (`TXFMA_B47              ), 
  .shft_size ($bits(ediff1_abs_f3a_h))
)
align_shf_50
(
  .op       ( {align_in_48_f3a_h,2'b0} ),
  .shfamnt  ( ediff1_abs_f3a_h         ),
  .op_align ( aligned_50_f3a_h         )
);

assign aligned_48_int_r_f3a_h = aligned_50_f3a_h[`TXFMA_INT_R];
assign aligned_48_int_l_f3a_h = aligned_50_f3a_h[`TXFMA_INT_L];


///////////////////////////////////////////////////////////////////////////////
// 50 bit ZAP
///////////////////////////////////////////////////////////////////////////////
assign zapped_op1_f3a_h = {($bits(aligned_50_f3a_h)){!align1_zap_f3a_h}} & aligned_50_f3a_h & ps_op1_clear_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// 50 bit Negation + clearing
///////////////////////////////////////////////////////////////////////////////
assign negated_op1_f3a_h = ({($bits({1'b0, zapped_op1_f3a_h})){align1_neg_f3a_h}} ^ {1'b0, zapped_op1_f3a_h}) 
                                & negated_op1_clr_f3a_h & negated_op1_log_mul_clr_f3a_h;
                
                         
//extend 
assign negated_op1_ext_f3a_h = {negated_op1_f3a_h[`TXFMA_A2], negated_op1_f3a_h};


//////////////////////////////////////////////////////////////////////////////
// GFX Replicator
//////////////////////////////////////////////////////////////////////////////
txfma_gfx_replicator gfx_replicator
(
  .cmd  ( cmd_f3a_h         ),
  .in   ( align_in_48_f3a_h ),
  .out  ( gfx_rpl_out_f3a_h )
);


//////////////////////////////////////////////////////////////////////////////
// Mux between Aligner48 and gfx
//////////////////////////////////////////////////////////////////////////////
assign align1_gfx_f3a_h = ~sigs_f3a_h[`TXFMA_F32_NORM] ? negated_op1_ext_f3a_h : {gfx_rpl_out_f3a_h,2'b0};


///////////////////////////////////////////////////////////////////////////////
// 25bit Align Low Mux 2
///////////////////////////////////////////////////////////////////////////////
always_comb
begin
  case (align2_mux_sel_f3a_h)
    `TXFMA_SEL_PH:      align_in_25_f3a_h = {prodsum_f3a_h[`TXFMA_A1:`TXFMA_B19], 3'b0};
    `TXFMA_SEL_PL:      align_in_25_f3a_h = {prodsum_f3a_h[`TXFMA_B36:`TXFMA_B57], 3'b0};
    `TXFMA_SEL_A:       align_in_25_f3a_h = {1'b0, fa_f3a_h[`TXFMA_A0:`TXFMA_B22]};
    default:            align_in_25_f3a_h = 25'b0;
  endcase   
end


///////////////////////////////////////////////////////////////////////////////
// 27 bit Alignment shifter
///////////////////////////////////////////////////////////////////////////////
txfma_align_shf 
#(.left_bit  (`TXFMA_A1), 
  .right_bit (`TXFMA_B24), 
  .shft_size ($bits(ediff2_abs_f3a_h))
)
align_shf_27
(
  .op       ( {align_in_25_f3a_h, 2'b0}),
  .shfamnt  ( ediff2_abs_f3a_h         ),
  .op_align ( aligned_27_f3a_h         )
);


///////////////////////////////////////////////////////////////////////////////
// 26 bit ZAP
///////////////////////////////////////////////////////////////////////////////
assign zapped_op2_f3a_h = {($bits(aligned_27_f3a_h)){!align2_zap_f3a_h}} & aligned_27_f3a_h;

///////////////////////////////////////////////////////////////////////////////
// 27 bit Negation
///////////////////////////////////////////////////////////////////////////////
assign negated_op2_f3a_h[`TXFMA_A2:`TXFMA_B24]  = {($bits({1'b0, zapped_op2_f3a_h[`TXFMA_A1:`TXFMA_B24]})){align2_neg_f3a_h}} ^ {1'b0, zapped_op2_f3a_h[`TXFMA_A1:`TXFMA_B24]};
assign negated_op2_ext_f3a_h = {negated_op2_f3a_h[`TXFMA_A2], negated_op2_f3a_h, {(`TXFMA_B25-`TXFMA_B47+1){op2_sxtto_g_f3a_h}}};


////////////////////////////////////////////////////////////////////////////////
// Oprand Mux 3
////////////////////////////////////////////////////////////////////////////////
//fc mask for ffrc or fc
assign fc_mask_f3a_h = ((cmd_f3a_h == `VPU_FCMD_FFRC) | (cmd_f3a_h==`VPU_FCMD_FFRC_SIN)) ? (fc_f3a_h[`TXFMA_A0:`TXFMA_B22] & aligned_50_f3a_h[`TXFMA_A0:`TXFMA_B22]) : fc_f3a_h[`TXFMA_A0:`TXFMA_B22];

always_comb
begin  
  case (opd3_mux_sel_f3a_h)
    `TXFMA_SEL_PH:       opd3_f3a_h = {2'b0,  prodsum_f3a_h[`TXFMA_A1:`TXFMA_B19], 4'b0, align2_neg_cin_f3a_h, 23'b0};
    `TXFMA_SEL_PL:       opd3_f3a_h = {2'b0,  prodsum_f3a_h[`TXFMA_B35:`TXFMA_B57], 4'b0, align2_neg_cin_f3a_h, 23'b0};
    `TXFMA_SEL_C:        opd3_f3a_h = {3'b0,  fc_mask_f3a_h, 1'b0, align2_neg_cin_f3a_h, 23'b0};
    `TXFMA_SEL_A:        opd3_f3a_h = {3'b0,  fa_f3a_h[`TXFMA_A0:`TXFMA_B22], 25'b0};
    `TXFMA_SEL_P:        opd3_f3a_h = {2'b0,  prodsum_f3a_h[`TXFMA_A1:`TXFMA_B47]};
    `TXFMA_SEL_P_INT:    opd3_f3a_h = {3'b0,  prodsum_f3a_h[63:17],2'b0};
    `TXFMA_SEL_EXP_RES:  opd3_f3a_h = {3'b0,  e_res_f3a_h[6:0],42'b0};
    `TXFMA_SEL_ROUND:    opd3_f3a_h = {34'b0, fround_l_f3a_h, 17'b0};
    default:             opd3_f3a_h = 52'b0;    
  endcase     
end



////////////////////////////////////////////////////////////////////////////////
// CSA ADDER
////////////////////////////////////////////////////////////////////////////////
assign csa_in1_f3a_h = align1_gfx_f3a_h;
assign csa_in2_f3a_h = negated_op2_ext_f3a_h;
assign csa_in3_f3a_h = opd3_f3a_h;

txfma_csa 
#(
  .bits(`TXFMA_A3-`TXFMA_B47+1)
)
f3_32csa
 (
  .a    ( csa_in1_f3a_h ),
  .b    ( csa_in2_f3a_h ),
  .cin  ( csa_in3_f3a_h ),
  .cout ( C_f3a_h       ),
  .sum  ( S_f3a_h       )
);

////////////////////////////////////////////////////////////////////////////////
// Combining Adder
////////////////////////////////////////////////////////////////////////////////

txfma_adder
#(
  .WIDTH( `TXFMA_A3-`TXFMA_B47+1 )
)
comb_adder_51
(
  .a    ( {C_f3a_h[`TXFMA_A2:`TXFMA_B47], op2_sxtto_g_f3a_h} ),
  .b    ( S_f3a_h[`TXFMA_A3:`TXFMA_B47]                      ),
  .cin  ( align1_neg_cin_f3a_h                               ),
  .sum  ( comb_add_f3a_h                                     )
);


///////////////////////////////////////////////////////////////////////////////
//  51b Leading 1/0 Anticipator
///////////////////////////////////////////////////////////////////////////////
txfma_lxd 
#(
  .in_width        ( `TXFMA_PS_DPATH_SZ    ),
  .aft_a0_bits_num ( `TXFMA_A3-`TXFMA_A0+1 ),   
  .out_width       ( `TXFMA_LXD_PS_SZ      )
) 
lxd_ps_hi
(
  .in1    ( {C_f3a_h[`TXFMA_A2:`TXFMA_B47], 1'b0} ),
  .in2    ( S_f3a_h[`TXFMA_A3:`TXFMA_B47]         ),   
  .sub_op ( lxd_sub_f3a_h                         ),
  .lxd    ( lxd_f3a_h                             )
);


assign comb_add_hi_ph_trz_f3a_h = 0;

///////////////////////////////////////////////////////////////////////////////
//  50b TRZ Low PS
///////////////////////////////////////////////////////////////////////////////
txfma_trz2 
#(
  .in_width  ( `TXFMA_PS_DPATH_SZ  ),
  .out_width ( `TXFMA_WSPS_ETRZ_SZ )
)
addnd_trz_ps
(
  .in1      ( S_f3a_h[`TXFMA_A3:`TXFMA_B47]         ),
  .in2      ( {C_f3a_h[`TXFMA_A2:`TXFMA_B47], 1'b0} ),
  .add_cin  ( align1_neg_cin_f3a_h                  ),
  .trz      ( comb_add_lo_ps_trz_f3a_h              )
);


//////////////////////////////////////////////////////////////////////////////
// Integer MinMax Mux
//////////////////////////////////////////////////////////////////////////////
always_comb
begin
  case (int_minmax_mux_sel_f3a_h)
    `TXFMA_INT_MINMAX_SEL_PH:     int_minmax_res_f3a_h = prodsum_f3a_h[63:32];
    `TXFMA_INT_MINMAX_SEL_S:      int_minmax_res_f3a_h = prodsum_f3a_h[60:29];    
    `TXFMA_INT_MINMAX_SEL_PL:     int_minmax_res_f3a_h = prodsum_f3a_h[31:0];
    `TXFMA_INT_MINMAX_SEL_C:      int_minmax_res_f3a_h = {fc_f3a_h[`TXFMA_A0:`TXFMA_B22], 8'b0};
    `TXFMA_INT_MINMAX_SEL_A:      int_minmax_res_f3a_h = {fa_f3a_h[`TXFMA_A0:`TXFMA_B22], 8'b0};
    default:                      int_minmax_res_f3a_h = 32'b0; 
  endcase   
end




endmodule
