// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_e1 
(
  input  [`TXFMA_EXP_PS_SZ-1:0]             ea_hi_f1a_h,
  input  [`TXFMA_EXP_PS_SZ-1:0]             eb_hi_f1a_h,
  input  [`TXFMA_EXP_PS_SZ-1:0]             ec_f1a_h,
  input  [`TXFMA_EXP_PH_SZ-1:0]             ea_lo_f1a_h,
  input  [`TXFMA_EXP_PH_SZ-1:0]             eb_lo_f1a_h,
  input                                     ea_hi_zero_f1a_h,
  input  [`VPU_DTYPE_SZ-1:0]                op_dtype_f1a_h,
  input  [`TXFMA_SIGS_SZ-1:0]               sigs_f1a_h, 
  input  [`VPU_FCMD_SZ-1:0]                 cmd_f1a_h,
  input  [`TXFMA_ETRZ_PH_SZ-1:0]            etrz_fa_hi_ph_f1a_h,
  input  [`TXFMA_ETRZ_PS_SZ-1:0]            etrz_fa_lo_ps_f1a_h,
  input  [`TXFMA_ETRZ_PH_SZ-1:0]            etrz_fb_hi_ph_f1a_h,
  input  [`TXFMA_ETRZ_PS_SZ-1:0]            etrz_fb_lo_ps_f1a_h,
  input                                     fc_zero30_8_f1a_h,
  output logic [`TXFMA_ETRZ_PROD_PH_SZ-1:0] etrz_prod_hi_ph_f1a_h,
  output logic [`TXFMA_ETRZ_PROD_PS_SZ-1:0] etrz_prod_lo_ps_f1a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  ediff_ec_eph_f1a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  ediff_ec_epl_f1a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  ediff_eph_epl_f1a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  eprod_hi_f1a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  eprod_lo_f1a_h,
  output logic [`TXFMA_EXP_PS_SZ-1:0]       ec_out_f1a_h
);


///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  ea_hi_mux_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  eb_hi_mux_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]       ec_mux_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]       ea_lo_mux_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]       eb_lo_mux_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  cin_hi_mux_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  cin_lo_mux_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  carry32_hi_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  carry32_lo_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  sum32_hi_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  sum32_lo_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  trans_log_mul_inc_dec_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  trans_log_mul_ec_f1a_h;
logic                              ec_gt127_f1a_h;


///////////////////////////////////////////////////////////////////////////////
// exp hi/lo mux
//for ph/pl, convert exponent from f16 to f32:
//  -copy e16 b3:0 to e32 b3:0
//  -replicate e16 b4(negated) to e32 bit b6:4
//  -copy e16 b4 to e32 b7
///////////////////////////////////////////////////////////////////////////////

  //exp hi
always_comb begin

  ea_hi_mux_f1a_h = (cmd_f1a_h == `VPU_FCMD_CVT_INTF32 | 
                     cmd_f1a_h == `VPU_FCMD_CVT_UINTF32 | 
                     cmd_f1a_h == `VPU_FCMD_FROUND)           ? {2'h0, `TXFMA_CVT_INTF32_OFFSET}
                  : (cmd_f1a_h == `VPU_FCMD_CVT_RASTF32)      ? {2'h0,`TXFMA_CVT_FXP1714_F32_OFFSET}                  
                  : (cmd_f1a_h == `VPU_TRANS_RCP_FMA2)        ? {2'h0,`TXFMA_TRANS_RCP_OFFSET}
                  : (cmd_f1a_h == `VPU_TRANS_RSQRT_FMA2)      ? {`TXFMA_EXP_SEXT_PS_SZ{1'b1}}
                  : ((cmd_f1a_h == `VPU_TRANS_EXP_FMA2) | 
                     (cmd_f1a_h == `VPU_TRANS_SIN_TRANSFORM)) ? '0
                  : (cmd_f1a_h == `VPU_TRANS_LOG_MUL)         ? trans_log_mul_ec_f1a_h  
                  : (op_dtype_f1a_h == `VPU_DTYPE_F32)        ? {2'h0, ea_hi_f1a_h}
                  :                                             {2'h0, ea_hi_f1a_h[`TXFMA_EXP_PH_SZ-1], 
                                                                 {3{~ea_hi_f1a_h[`TXFMA_EXP_PH_SZ-1] & ~ea_hi_zero_f1a_h}}, 
                                                                 ea_hi_f1a_h[`TXFMA_EXP_PH_SZ-2:0]};

  eb_hi_mux_f1a_h = (cmd_f1a_h==`VPU_TRANS_RCP_FMA2)   ? {2'h3,~eb_hi_f1a_h}
                  : (cmd_f1a_h==`VPU_TRANS_LOG_MUL)    ? trans_log_mul_inc_dec_f1a_h  
                  : (cmd_f1a_h==`VPU_TRANS_EXP_FMA2)   ? {{2{eb_hi_f1a_h[`TXFMA_EXP_PS_SZ-1]}},eb_hi_f1a_h}                       
                  : (op_dtype_f1a_h==`VPU_DTYPE_F32 | 
                     sigs_f1a_h[`TXFMA_F32_FX] | 
                     cmd_f1a_h==`VPU_TRANS_RSQRT_FMA2) 
                                                       ? {2'h0, eb_hi_f1a_h}
                  :                                      {2'h0, eb_hi_f1a_h[`TXFMA_EXP_PH_SZ-1], 
                                                          {3{~eb_hi_f1a_h[`TXFMA_EXP_PH_SZ-1]}}, 
                                                          eb_hi_f1a_h[`TXFMA_EXP_PH_SZ-2:0]};
  
  ec_mux_f1a_h = (op_dtype_f1a_h == `VPU_DTYPE_F32 || 
                     op_dtype_f1a_h == `VPU_DTYPE_F16_F32) ? ec_f1a_h
                       :                                     {ec_f1a_h[`TXFMA_EXP_PH_SZ-1] , {3{~ec_f1a_h[`TXFMA_EXP_PH_SZ-1]}}, ec_f1a_h[`TXFMA_EXP_PH_SZ-2:0]};

  if (sigs_f1a_h[`TXFMA_NORM_F32])
    ec_mux_f1a_h = `TXFMA_NORM_F32_BIAS;

end

  //exp lo
assign ea_lo_mux_f1a_h = (cmd_f1a_h==`VPU_TRANS_EXP_FRAC) ? `TXFMA_EXP_PS_SZ'h86 //compare fc>=128 (exp is 0x86)
                       :                                    {ea_lo_f1a_h[`TXFMA_EXP_PH_SZ-1],{3{~ea_lo_f1a_h[`TXFMA_EXP_PH_SZ-1]}},ea_lo_f1a_h[`TXFMA_EXP_PH_SZ-2:0]};
                    
assign eb_lo_mux_f1a_h = (cmd_f1a_h==`VPU_TRANS_EXP_FRAC) ? `TXFMA_EXP_PS_SZ'h0
                       :                                    {eb_lo_f1a_h[`TXFMA_EXP_PH_SZ-1],{3{~eb_lo_f1a_h[`TXFMA_EXP_PH_SZ-1]}},eb_lo_f1a_h[`TXFMA_EXP_PH_SZ-2:0]};

  //ci
assign cin_hi_mux_f1a_h = (cmd_f1a_h==`VPU_FCMD_CVT_INTF32  ||
                           cmd_f1a_h==`VPU_FCMD_CVT_UINTF32 ||
                           cmd_f1a_h==`VPU_FCMD_CVT_RASTF32 ||                           
                           cmd_f1a_h==`VPU_FCMD_FROUND      ||
                           cmd_f1a_h==`VPU_FCMD_FFRC        ||
                           cmd_f1a_h==`VPU_FCMD_FFRC_SIN    ||
                           cmd_f1a_h==`VPU_TRANS_EXP_FMA2)         ? {2'h0, `FLOAT32_E_BIAS_HEX}
                        : (sigs_f1a_h[`TXFMA_TRANS2] ||
                           cmd_f1a_h==`VPU_TRANS_SIN_TRANSFORM ||
                           cmd_f1a_h==`VPU_TRANS_LOG_MUL)          ? `TXFMA_EXP_SEXT_PS_SZ'h0
                        :                                            {2'h3, `FLOAT32_NEG_E_BIAS_HEX};

assign cin_lo_mux_f1a_h = (cmd_f1a_h==`VPU_TRANS_EXP_FRAC) ? `TXFMA_EXP_SEXT_PS_SZ'b0
                        :                                    {2'h3, `FLOAT32_NEG_E_BIAS_HEX};

//output
assign ec_out_f1a_h = ec_mux_f1a_h;


///////////////////////////////////////////////////////////////////////////////
// exp hi eprod csa
///////////////////////////////////////////////////////////////////////////////
txfma_csa 
#(
  .bits( `TXFMA_EXP_SEXT_PS_SZ )
)
exp_hi_prod_csa
(
 .a    ( ea_hi_mux_f1a_h  ), 
 .b    ( eb_hi_mux_f1a_h  ),
 .cin  ( cin_hi_mux_f1a_h ),
 .cout ( carry32_hi_f1a_h ),
 .sum  ( sum32_hi_f1a_h   )
);


///////////////////////////////////////////////////////////////////////////////
// exp hi eprod adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH( `TXFMA_EXP_SEXT_PS_SZ )
)
exp_hi_ep_adder
(
  .a    ( sum32_hi_f1a_h                                     ),
  .b    ( {carry32_hi_f1a_h[`TXFMA_EXP_SEXT_PS_SZ-2:0],1'b0} ),
  .cin  ( 1'b0                                               ),
  .sum  ( eprod_hi_f1a_h                                     )
);


///////////////////////////////////////////////////////////////////////////////
// exp lo eprod csa
///////////////////////////////////////////////////////////////////////////////
txfma_csa 
#(
  .bits( `TXFMA_EXP_SEXT_PS_SZ )
)
exp_lo_prod_csa
(
 .a    ( {2'h0, ea_lo_mux_f1a_h}         ), 
 .b    ( {2'h0, eb_lo_mux_f1a_h}         ),
 .cin  ( cin_lo_mux_f1a_h                ),
 .cout ( carry32_lo_f1a_h                ),
 .sum  ( sum32_lo_f1a_h                  )
);


///////////////////////////////////////////////////////////////////////////////
// exp lo eprod adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH( `TXFMA_EXP_SEXT_PS_SZ )
)
exp_lo_ep_adder
(
  .a    ( sum32_lo_f1a_h                                     ),
  .b    ( {carry32_lo_f1a_h[`TXFMA_EXP_SEXT_PS_SZ-2:0],1'b0} ),
  .cin  ( 1'b0                                               ),
  .sum  ( eprod_lo_f1a_h                                     ) 
);

///////////////////////////////////////////////////////////////////////////////
// ec-eph ediff adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH( `TXFMA_EXP_SEXT_PS_SZ )
)
ec_eph_ediff_adder
(
  .a    ( {2'b0, ec_mux_f1a_h} ),
  .b    ( ~eprod_hi_f1a_h         ),
  .cin  ( 1'b1                    ), 
  .sum  ( ediff_ec_eph_f1a_h      )
);


///////////////////////////////////////////////////////////////////////////////
// ec-epl ediff adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH( `TXFMA_EXP_SEXT_PS_SZ )
)
ec_epl_ediff_adder
(
  .a    ( {2'b0, ec_f1a_h}     ),
  .b    ( ~eprod_lo_f1a_h         ),  
  .cin  ( 1'b1                    ),
  .sum  ( ediff_ec_epl_f1a_h      )
);


///////////////////////////////////////////////////////////////////////////////
// eph-epl ediff adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH( `TXFMA_EXP_SEXT_PS_SZ )
)
eph_epl_ediff_adder
(
  .a    ( eprod_hi_f1a_h      ),
  .b    ( ~eprod_lo_f1a_h     ),
  .cin  ( 1'b1                ),
  .sum  ( ediff_eph_epl_f1a_h ) 
);


///////////////////////////////////////////////////////////////////////////////
// exp hi ph tp adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH( `TXFMA_ETRZ_PROD_PH_SZ )
)
exp_hi_tp_adder
(
  .a    ( {1'b0, etrz_fa_hi_ph_f1a_h} ),
  .b    ( {1'b0, etrz_fb_hi_ph_f1a_h} ),
  .cin  ( 1'b0                        ),
  .sum  ( etrz_prod_hi_ph_f1a_h       )
);


///////////////////////////////////////////////////////////////////////////////
// exp lo ps tp adder
///////////////////////////////////////////////////////////////////////////////

txfma_adder
#(
  .WIDTH( `TXFMA_ETRZ_PROD_PS_SZ )
)
exp_lo_tp_adder
(
  .a    ( {1'b0, etrz_fa_lo_ps_f1a_h} ),
  .b    ( {1'b0, etrz_fb_lo_ps_f1a_h} ),
  .cin  ( 1'b0                        ),
  .sum  ( etrz_prod_lo_ps_f1a_h       )
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
    trans_log_mul_inc_dec_f1a_h = `TXFMA_EXP_SEXT_PS_SZ'h1;
    //ec<127 and fc!=0
  else if (!fc_zero30_8_f1a_h)
    trans_log_mul_inc_dec_f1a_h = {`TXFMA_EXP_SEXT_PS_SZ{1'b1}}; 
end
  // neg ec if ec<127
assign trans_log_mul_ec_f1a_h = ec_gt127_f1a_h ? {2'h0,ec_f1a_h} 
                              :                  {3'h7, ~ec_f1a_h[`TXFMA_EXP_PS_SZ-2:0]};


endmodule
