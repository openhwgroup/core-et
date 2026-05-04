// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_e5 
(
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_f5a_h,
  input                               e_inc_sel_f5a_h,
  input  [`VPU_FCMD_SZ-1:0]           cmd_f5a_h,  
  output [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_res_f5a_h
);


logic [`TXFMA_EXP_SEXT_PS_SZ-1:0] e_inc_dec_f5a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0] e_res_adder_in2_mux_f5a_h;

  //adder in mux
assign e_res_adder_in2_mux_f5a_h = (cmd_f5a_h==`VPU_TRANS_RSQRT_FMA2) ? `TXFMA_EXP_SEXT_PS_SZ'h3fe //substract 2 to compensate cin 
                                    :                                      `TXFMA_EXP_SEXT_PS_SZ'b0;

                                    

///////////////////////////////////////////////////////////////////////////////
// exp hi res add hi
///////////////////////////////////////////////////////////////////////////////
  // Do incrementing anyway in order to shorten the timing
txfma_adder
#(
  .WIDTH(`TXFMA_EXP_SEXT_PS_SZ)
)
exp_res_adder_hi
(
  .a    ( e_f5a_h                   ),
  .b    ( e_res_adder_in2_mux_f5a_h ),
  .cin  ( 1'b1                         ),
  .sum  ( e_inc_dec_f5a_h           ) 
);

///////////////////////////////////////////////////////////////////////////////
// exp hi res mux 
///////////////////////////////////////////////////////////////////////////////
assign e_res_f5a_h = (e_inc_sel_f5a_h || (cmd_f5a_h==`VPU_TRANS_RSQRT_FMA2)) ?  e_inc_dec_f5a_h 
                      :                                                               e_f5a_h;
endmodule

