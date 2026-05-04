// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f4 
(
  input                                        ctrl_f3_clk,     
  input                                        ctrl_f4_clk,    
  input  [`TXFMA_A3:`TXFMA_B47]                comb_s_f4a_h,
  input  [`TXFMA_A3:`TXFMA_B47]                comb_c_f4a_h,
  input  [`VPU_DATA_S_SZ-1:0]                  int_minmax_res_f4a_h,
  input                                        neg_norm_a3_f4a_h,
  input                                        neg_norm_na3_f4a_h,
  input                                        clearphps_en_f4a_h,
  input                                        clear16frnd_nshin_f4a_l,
  input                                        ph_ps_clear_f4a_l,
  input                                        al1c3_neg_cin_f3a_h,
  input                                        fma_subopc3_f3a_h,
  input                                        op2_sxtto_g_f3a_h,
  output logic                                 neg_norm_f5a_h,
  output logic [`VPU_DATA_S_SZ-1:0]            int_minmax_res_f5a_h,
  output logic [`TXFMA_A3:`TXFMA_B47]          comb_add_f5a_h,
  output [`TXFMA_LXD_PS_SZ-1:0]                elxd_f4a_h,
  output [`TXFMA_WSPS_ETRZ_SZ-1:0]             etrz50_f4a_h
);

////////////////////////
// Internal Signals 
////////////////////////
logic                             al1c3_neg_cin_f4a_h;
logic                             fma_subopc3_f4a_h;
logic                             neg_norm_f4a_h;
logic                             ph_ps_clearbuf_f4a_l;
logic                             clr_frnd_phps_f4a_l;
logic                             ph_ps_sxt_f4a_h;
logic                             op2_sxtto_g_f4a_h;
logic [`TXFMA_A3:`TXFMA_B47]      comb_add_f4a_h;
logic [`TXFMA_A3:`TXFMA_B47]      comb_add_cleared_f4a_h;


///////////////////////////////////////////////////////////////////////////////
// F5 flops; Moved from txfma_top 
///////////////////////////////////////////////////////////////////////////////

 //    CLK               DOUT                        DIN
  `FF (ctrl_f4_clk, comb_add_f5a_h,             comb_add_cleared_f4a_h  )
  `FF (ctrl_f4_clk, int_minmax_res_f5a_h,       int_minmax_res_f4a_h    )
  `FF (ctrl_f4_clk, neg_norm_f5a_h,             neg_norm_f4a_h          )

// Locally flopped control signals I/P from txfma_c3
  `FF (ctrl_f3_clk, al1c3_neg_cin_f4a_h,        al1c3_neg_cin_f3a_h     )
  `FF (ctrl_f3_clk, fma_subopc3_f4a_h,          fma_subopc3_f3a_h       )
  `FF (ctrl_f3_clk, op2_sxtto_g_f4a_h,          op2_sxtto_g_f3a_h       )


////////////////////////////////////////////////////////////////////////////////
// Combining Adder
////////////////////////////////////////////////////////////////////////////////

txfma_adder
#(
  .WIDTH( `TXFMA_A3-`TXFMA_B47+1 )
)
comb_adder_52
(
  .a    ( {comb_c_f4a_h[`TXFMA_A2:`TXFMA_B47], op2_sxtto_g_f4a_h}  ),
  .b    ( comb_s_f4a_h[`TXFMA_A3:`TXFMA_B47]                       ),
  .cin  ( al1c3_neg_cin_f4a_h                                      ),
  .sum  ( comb_add_f4a_h                                           )
);


assign neg_norm_f4a_h = neg_norm_na3_f4a_h | (comb_add_f4a_h[`TXFMA_A3] && neg_norm_a3_f4a_h);
assign ph_ps_sxt_f4a_h = clearphps_en_f4a_h & neg_norm_f4a_h;
assign ph_ps_clearbuf_f4a_l = ph_ps_clear_f4a_l;

assign clr_frnd_phps_f4a_l = clear16frnd_nshin_f4a_l & ph_ps_clearbuf_f4a_l;

// Clear lower-bits for the Norm Shifter
assign comb_add_cleared_f4a_h[`TXFMA_A3:`TXFMA_B24] = comb_add_f4a_h[`TXFMA_A3:`TXFMA_B24];
assign comb_add_cleared_f4a_h[`TXFMA_B25:`TXFMA_B30] = {(`TXFMA_B25-`TXFMA_B30+1){ph_ps_clearbuf_f4a_l}} & comb_add_f4a_h[`TXFMA_B25:`TXFMA_B30] | 
                                                       {(`TXFMA_B25-`TXFMA_B30+1){ph_ps_sxt_f4a_h}}; 

assign comb_add_cleared_f4a_h[`TXFMA_B31:`TXFMA_B47] = {(`TXFMA_B31-`TXFMA_B47+1){(clr_frnd_phps_f4a_l)}} & comb_add_f4a_h[`TXFMA_B31:`TXFMA_B47] |
                                                       {(`TXFMA_B31-`TXFMA_B47+1){ph_ps_sxt_f4a_h}};

///////////////////////////////////////////////////////////////////////////////
//  50-bit Leading 1/0 Anticipator (ELXD)
///////////////////////////////////////////////////////////////////////////////
txfma_lxd 
#(
  .in_width        ( `TXFMA_PS_DPATH_SZ    ),
  .aft_a0_bits_num ( `TXFMA_A3-`TXFMA_A0+1 ),   
  .out_width       ( `TXFMA_LXD_PS_SZ      )
) 
lxd_ps_hi
(
  .in1    ( {comb_c_f4a_h[`TXFMA_A2:`TXFMA_B47], 1'b0} ),
  .in2    ( comb_s_f4a_h[`TXFMA_A3:`TXFMA_B47]         ),   
  .sub_op ( fma_subopc3_f4a_h                          ),
  .lxd    ( elxd_f4a_h                                 )
);

///////////////////////////////////////////////////////////////////////////////
//  49-bit TRZ 
///////////////////////////////////////////////////////////////////////////////
txfma_trz2 
#(
  .in_width  ( `TXFMA_PS_DPATH_SZ  ),
  .out_width ( `TXFMA_WSPS_ETRZ_SZ )
)
addnd_trz_ps
(
  .in1      ( comb_s_f4a_h[`TXFMA_A3:`TXFMA_B47]           ),
  .in2      ( {comb_c_f4a_h[`TXFMA_A2:`TXFMA_B47], 1'b0}   ),
  .add_cin  ( al1c3_neg_cin_f4a_h                          ),
  .trz      ( etrz50_f4a_h                                 )
);

endmodule
