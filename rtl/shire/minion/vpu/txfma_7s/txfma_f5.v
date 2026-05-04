// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f5
#(
    parameter left_bit_in   = `TXFMA_A3,
    parameter left_bit_out  = `TXFMA_A0,
    parameter right_bit_in  = `TXFMA_B47,
    parameter right_bit_out = `TXFMA_B32
)
(
  input                                       ctrl_f4_clk,
  input                                       ctrl_f5_clk,
  input  [`TXFMA_A3:`TXFMA_B47]               comb_add_f5a_h,
  input  [`TXFMA_A3:`TXFMA_B27]               int_minmax_res_f5a_h,
  input  [`TXFMA_NORM_SHF_AMNT_SZ-1:0]        enc_nsamt_f4a_h,
  input                                       neg_norm_f5a_h,
  input                                       en_logrrsel1b_f5a_l,
  input                                       mmx_sel_immres_f5a_h,
  input                                       nshcen_sel1b_f5a_h,
  input                                       rc_ps_f5a_h,
  input                                       rc_ph_f5a_h,
  input                                       rc_int32_f5a_h,
  input                                       int32_stky2r_f5a_h,
  input                                       stky2g_f5a_h,
  input                                       rc_3rmxr_f5a_h,
  input                                       rc_rl01_f5a_h,
  input                                       rc_rmm_f5a_h,
  input                                       rc_rup_f5a_h,
  input                                       rc_rne_f5a_h,
  input                                       rc_rdn_f5a_h,
  input                                       rc_res_sign_f5a_l,
  input                                       rc_transrnd_f5a_h,
  input                                       rc_transrnd_log_fma2_f5a_h,
  output                                      ph_allone2r_f5a_h,
  output                                      ph_allone2l_f5a_h,
  output                                      ps_allone2r_f5a_h,
  output                                      ps_allone2l_f5a_h,
  output                                      nshc_f5a_h,
  output                                      ph_stky2l_f5a_h,
  output                                      ps_stky2l_f5a_h,
  output                                      int32_stky2l_f5a_h,
  output                                      comb_add_a3_f5a_h,
  output logic                                frac_zero_f6a_h,
  output logic                                rnd_l_ph_f6a_h,
  output logic                                rnd_r_ph_f6a_h,
  output logic                                rnd_l_ps_f6a_h,
  output logic                                rnd_r_ps_f6a_h,
  output logic                                rnd_l_int_f6a_h,
  output logic                                rnd_r_int_f6a_h,
  output logic [`TXFMA_FTZ_SZ-1:0]            ftz_f6a_l,
  output logic [`TXFMA_A0:`TXFMA_B31]         mergd_frac_res_hi_f6a_h
);

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////

logic                                    rnd_l_ph_f5a_h;
logic                                    rnd_r_ph_f5a_h;
logic                                    rnd_l_ps_f5a_h;
logic                                    rnd_r_ps_f5a_h;
logic                                    rnd_l_int_f5a_h;
logic                                    rnd_r_int_f5a_h;
logic                                    frac_zero_f5a_h;
logic                                    frac_zero_1_f5a_h;
logic                                    frac_zero_2_f5a_h;
logic [`TXFMA_A0:`TXFMA_B32]             norm_shift_out_f5a_h;
logic [`TXFMA_A0:`TXFMA_B32]             norm_shift_out_neg_f5a_h;
logic [`TXFMA_A0:`TXFMA_B31]             mergd_frac_res_hi_f5a_h;
logic [`TXFMA_FTZ_SZ-1:0]                ftz_f5a_l;
logic [`TXFMA_NORM_SHF_AMNT_SZ-1:0]      enc_nsamt_f5a_h;
logic [1:0]                              mergd_mux_sel_f5a_h;
logic                                    ph_stky2r_f5a_h;
logic                                    ps_stky2r_f5a_h;
logic                                    ps_neg_inc_f5a_h;
logic                                    int32_neg_inc_f5a_h;
logic                                    ps_rc_xr_f5a_l;
logic                                    ps_rc_rr0_f5a_h;
logic                                    ps_rc_rr1_f5a_h;
logic                                    ps_rc_rr2_f5a_h;
logic                                    ps_rc_rl2_f5a_l;
logic                                    ps_rc_rl3_f5a_l;
logic                                    ph_rc001_f5a_l;  
logic                                    ph_rc3_f5a_h;
logic                                    ph_rc4_f5a_h;
logic                                    ph_irr1_f5a_l; 
logic                                    ph_irr2_f5a_l;
logic                                    ps_rc001_f5a_l;  
logic                                    ps_rc0_f5a_l; 
logic                                    ps_rc3_f5a_h;
logic                                    ps_rc4_f5a_h;
logic                                    ps_irr1_f5a_l; 
logic                                    ps_irr2_f5a_l;
logic                                    ph_t1_f5a_l;
logic                                    ps_lxr_f5a_h;
logic                                    ps_x2_f5a_l;
logic                                    ps_t1_f5a_l;
logic                                    ps_t2_f5a_l;
logic                                    int32_crdn_f5a_h;
logic                                    int32_crup_f5a_h;
logic                                    int32_crmm_f5a_h;
logic                                    int32_cr1_f5a_h;
logic                                    int32_cr2_f5a_h;
logic                                    int32_cr3_f5a_h;
logic                                    int32_cr1c_f5a_h;
logic                                    int32_cr2c_f5a_h;
logic                                    int32_cr3c_f5a_h;
logic                                    int32_cvtrnel_f5a_h;
logic                                    int32_nr3_f5a_h;
logic                                    int32_b1_f5a_h;  
logic                                    int32_b2_f5a_h; 
logic                                    int32_b3_f5a_h;
logic                                    ph_lbit_f5a_h;
logic                                    ph_rbit_f5a_h; 
logic                                    ps_rbit_f5a_h;
logic                                    int32_lbit_f5a_h; 
logic                                    int32_rbit_f5a_h; 
logic                                    nsh_ls_a0bb0_h;
logic                                    nsh_ls_a0bb0_l;
logic                                    nshc_sel_f5a_h;
logic                                    ps_lbit_f5a_h;


///////////////////////////////////////////////////////////////////////////////
// Local Flops - Needed to guarantee timing 
///////////////////////////////////////////////////////////////////////////////

 //    CLK               DOUT                     DIN
  `FF (ctrl_f5_clk, mergd_frac_res_hi_f6a_h, mergd_frac_res_hi_f5a_h  )
  `FF (ctrl_f5_clk, ftz_f6a_l,               ftz_f5a_l                )
  `FF (ctrl_f5_clk, frac_zero_f6a_h,         frac_zero_f5a_h          )
  `FF (ctrl_f5_clk, rnd_l_ph_f6a_h,          rnd_l_ph_f5a_h           ) 
  `FF (ctrl_f5_clk, rnd_r_ph_f6a_h,          rnd_r_ph_f5a_h           ) 
  `FF (ctrl_f5_clk, rnd_l_ps_f6a_h,          rnd_l_ps_f5a_h           ) 
  `FF (ctrl_f5_clk, rnd_r_ps_f6a_h,          rnd_r_ps_f5a_h           ) 
  `FF (ctrl_f5_clk, rnd_l_int_f6a_h,         rnd_l_int_f5a_h          ) 
  `FF (ctrl_f5_clk, rnd_r_int_f6a_h,         rnd_r_int_f5a_h          ) 


// Locally flopped control signals I/P from txfma_c4
  `FF (ctrl_f4_clk, enc_nsamt_f5a_h,         enc_nsamt_f4a_h   )


///////////////////////////////////////////////////////////////////////////////
// Normalization Shifter 
///////////////////////////////////////////////////////////////////////////////

//assign final_norm_shftamt_f5a_h = sel_lsout_f5a_h ? ns_lsamt_hi_f5a_h : {4'b1111, -ns_rsamt_hi_f5a_h};

txfma_norm_shf 
#(
  .left_bit_in    (left_bit_in            ),
  .left_bit_out   (left_bit_out           ),  
  .right_bit_in   (right_bit_in           ),
  .right_bit_out  (right_bit_out          ),
  .shift_size     (`TXFMA_NORM_SHF_AMNT_SZ)
)
norm_shf
(
  .op      ( comb_add_f5a_h       ),
  .shfamnt ( enc_nsamt_f5a_h      ),
  .op_norm ( norm_shift_out_f5a_h )
);

assign comb_add_a3_f5a_h = comb_add_f5a_h[`TXFMA_A3];

///////////////////////////////////////////////////////////////////////////////
// Round Control - Sticky Bit Calculations 
///////////////////////////////////////////////////////////////////////////////

// Negate the O/P of the Normalization Shifter if necessary
assign norm_shift_out_neg_f5a_h = norm_shift_out_f5a_h ^ {34{neg_norm_f5a_h}};

assign nsh_ls_a0bb0_h = (nshcen_sel1b_f5a_h & norm_shift_out_neg_f5a_h[`TXFMA_B0] & ~norm_shift_out_neg_f5a_h[`TXFMA_A0]);
assign nsh_ls_a0bb0_l = ~nsh_ls_a0bb0_h; 

assign nshc_sel_f5a_h = nsh_ls_a0bb0_h | ~en_logrrsel1b_f5a_l;


// Norm Shifter Correction to txfma_c5
assign nshc_f5a_h = nsh_ls_a0bb0_h; 

assign ph_stky2r_f5a_h     = stky2g_f5a_h; // | (norm_shift_out_f5a_h[`TXFMA_B11] & nsh_ls_a0bb0_l);
assign ps_stky2r_f5a_h     = stky2g_f5a_h | (norm_shift_out_f5a_h[`TXFMA_B24] & nsh_ls_a0bb0_l);

assign ph_stky2l_f5a_h     = stky2g_f5a_h | int_minmax_res_f5a_h[52];// (((nsh_ls_a0bb0_l & norm_shift_out_f5a_h[`TXFMA_B10]) | stky2g_f5a_h) | norm_shift_out_f5a_h[`TXFMA_B11]);
assign ps_stky2l_f5a_h     = (((nsh_ls_a0bb0_l & norm_shift_out_f5a_h[`TXFMA_B23]) | stky2g_f5a_h) | norm_shift_out_f5a_h[`TXFMA_B24]);

assign ps_neg_inc_f5a_h    = (rc_ps_f5a_h & neg_norm_f5a_h) & ~ps_stky2r_f5a_h;

assign ps_rc_xr_f5a_l      = ~(rc_3rmxr_f5a_h & ps_neg_inc_f5a_h); 

assign ps_rc_rr0_f5a_h     = ps_neg_inc_f5a_h & rc_rne_f5a_h;
assign ps_rc_rr1_f5a_h     = ~(ps_neg_inc_f5a_h | ~rc_rne_f5a_h);
assign ps_rc_rr2_f5a_h     = ~(ps_neg_inc_f5a_h | ~rc_rmm_f5a_h);
assign ps_rc_rl2_f5a_l     = ~(ps_neg_inc_f5a_h & rc_rne_f5a_h);
assign ps_rc_rl3_f5a_l     = ~(ps_neg_inc_f5a_h & rc_rmm_f5a_h);

assign ph_lbit_f5a_h    =  int_minmax_res_f5a_h[53]; //(nsh_ls_a0bb0_l & norm_shift_out_neg_f5a_h[`TXFMA_B9])  | (nsh_ls_a0bb0_h & norm_shift_out_neg_f5a_h[`TXFMA_B10]); 
assign ph_rbit_f5a_h    =  int_minmax_res_f5a_h[52]; // (nsh_ls_a0bb0_l & norm_shift_out_neg_f5a_h[`TXFMA_B10]) | (nsh_ls_a0bb0_h & norm_shift_out_neg_f5a_h[`TXFMA_B11]); 
assign ps_lbit_f5a_h    = (nsh_ls_a0bb0_l & norm_shift_out_neg_f5a_h[`TXFMA_B22]) | (nsh_ls_a0bb0_h & norm_shift_out_neg_f5a_h[`TXFMA_B23]); 
assign ps_rbit_f5a_h    = (nsh_ls_a0bb0_l & norm_shift_out_neg_f5a_h[`TXFMA_B23]) | (nsh_ls_a0bb0_h & norm_shift_out_neg_f5a_h[`TXFMA_B24]);
assign int32_lbit_f5a_h = norm_shift_out_neg_f5a_h[`TXFMA_B30]; 
assign int32_rbit_f5a_h = norm_shift_out_neg_f5a_h[`TXFMA_B31]; 


// Using all of the above, determine the rounding bits based on data type (PH, PS, INT32)

  // PH R-Bit Calculations
assign ph_rc001_f5a_l = ~(ph_lbit_f5a_h | ~ph_rbit_f5a_h | ~ph_stky2r_f5a_h);  
assign ph_rc3_f5a_h   = ph_rbit_f5a_h & ~ph_lbit_f5a_h;
assign ph_rc4_f5a_h   = ph_rbit_f5a_h & ph_lbit_f5a_h;

assign ph_irr1_f5a_l  = ~((ph_rc3_f5a_h & rc_rne_f5a_h) | (rc_rne_f5a_h & ph_rc4_f5a_h)); 
assign ph_irr2_f5a_l  = ~((ph_rc001_f5a_l & rc_rne_f5a_h) | (ph_rbit_f5a_h & rc_rmm_f5a_h)); 

assign rnd_r_ph_f5a_h  = ~(ph_irr1_f5a_l & ph_irr2_f5a_l); 


  // PH L-Bit Calculations
assign ph_t1_f5a_l          = ~(rc_rl01_f5a_h & ph_stky2l_f5a_h);

assign rnd_l_ph_f5a_h       = ~(ph_t1_f5a_l); 


  // PS R-Bit Calculations
assign ps_rc001_f5a_l = ~(ps_lbit_f5a_h | ~ps_rbit_f5a_h | ~ps_stky2r_f5a_h);  
assign ps_rc0_f5a_l   = ~(ps_rbit_f5a_h | ps_lbit_f5a_h | ps_stky2r_f5a_h); 
assign ps_rc3_f5a_h   = ps_rbit_f5a_h & ~ps_lbit_f5a_h;
assign ps_rc4_f5a_h   = ps_rbit_f5a_h & ps_lbit_f5a_h;

assign ps_irr1_f5a_l  = ~((ps_rc3_f5a_h & ps_rc_rr1_f5a_h) | (ps_rc0_f5a_l & ps_rc_rr0_f5a_h) | (rc_rne_f5a_h & ps_rc4_f5a_h)); 
assign ps_irr2_f5a_l  = ~((ps_rc001_f5a_l & rc_rne_f5a_h) | (ps_rbit_f5a_h & ps_rc_rr2_f5a_h)); 

assign rnd_r_ps_f5a_h  = ~(ps_rc_xr_f5a_l & ps_irr1_f5a_l & ps_irr2_f5a_l); 


  // PS L-Bit Calculations
assign ps_lxr_f5a_h         = (ps_lbit_f5a_h ^ ps_rbit_f5a_h);
assign ps_x2_f5a_l          = ~(ps_stky2r_f5a_h | ps_rc_rl2_f5a_l);

assign ps_t1_f5a_l          = ~(rc_rl01_f5a_h & ps_stky2l_f5a_h);
assign ps_t2_f5a_l          = ~(ps_lxr_f5a_h & ps_x2_f5a_l);

assign rnd_l_ps_f5a_h       = ~(ps_t1_f5a_l & ps_t2_f5a_l & ps_rc_rl3_f5a_l); 


  // INT32 R-Bit Calculations
assign int32_stky2l_f5a_h  = int32_stky2r_f5a_h | norm_shift_out_f5a_h[`TXFMA_B31];
assign int32_neg_inc_f5a_h = rc_int32_f5a_h & neg_norm_f5a_h;

assign int32_crdn_f5a_h = rc_rdn_f5a_h & int32_stky2l_f5a_h;
assign int32_crup_f5a_h = rc_rup_f5a_h & int32_stky2l_f5a_h;
assign int32_crmm_f5a_h = rc_rmm_f5a_h & int32_cr2_f5a_h;

assign int32_cr1_f5a_h = ~(int32_rbit_f5a_h | int32_lbit_f5a_h);
assign int32_cr2_f5a_h = (neg_norm_f5a_h ^ int32_rbit_f5a_h);
assign int32_cr3_f5a_h = ~(rc_rne_f5a_h & int32_rbit_f5a_h & int32_lbit_f5a_h);

assign int32_cr1c_f5a_h = ~(rc_rne_f5a_h & neg_norm_f5a_h & int32_cr1_f5a_h);
assign int32_cr2c_f5a_h = ~(rc_rne_f5a_h & int32_cr2_f5a_h & int32_stky2r_f5a_h);
assign int32_cr3c_f5a_h = (neg_norm_f5a_h | int32_cr3_f5a_h);

assign int32_cvtrnel_f5a_h = ~(int32_cr1c_f5a_h & int32_cr2c_f5a_h & int32_cr3c_f5a_h);
assign int32_nr3_f5a_h = ~(int32_crdn_f5a_h | int32_crmm_f5a_h | int32_cvtrnel_f5a_h);

assign int32_b1_f5a_h = ~( rc_res_sign_f5a_l & (int32_cvtrnel_f5a_h | int32_crmm_f5a_h | int32_crup_f5a_h));  
assign int32_b2_f5a_h = ~(int32_neg_inc_f5a_h & int32_nr3_f5a_h); 
assign int32_b3_f5a_h = ~(int32_b1_f5a_h & int32_b2_f5a_h);

assign rnd_l_int_f5a_h = rc_transrnd_f5a_h | (rc_int32_f5a_h & int32_b3_f5a_h);
assign rnd_r_int_f5a_h = rc_transrnd_log_fma2_f5a_h;

assign ftz_f5a_l[1] = ~((ph_rbit_f5a_h) & ~ph_stky2r_f5a_h & rc_rne_f5a_h & rc_ph_f5a_h);
assign ftz_f5a_l[0] = ~((ps_rbit_f5a_h ^ neg_norm_f5a_h) & ~ps_stky2r_f5a_h & rc_rne_f5a_h & rc_ps_f5a_h);


///////////////////////////////////////////////////////////////////////////////
// Round Overflow Control 
//////////////////////////////////////////////////////////////////////////////

// Result reduction for the purpose of determining round overflow. 3 gate delay
assign ph_allone2l_f5a_h = &mergd_frac_res_hi_f5a_h[`TXFMA_A0:`TXFMA_B9];
assign ph_allone2r_f5a_h = ph_allone2l_f5a_h & (mergd_frac_res_hi_f5a_h[`TXFMA_B10]);
assign ps_allone2l_f5a_h = ph_allone2r_f5a_h & (&mergd_frac_res_hi_f5a_h[`TXFMA_B11:`TXFMA_B22]);
assign ps_allone2r_f5a_h = ps_allone2l_f5a_h & mergd_frac_res_hi_f5a_h[`TXFMA_B23];


///////////////////////////////////////////////////////////////////////////////
// Merging MUX 
///////////////////////////////////////////////////////////////////////////////

assign mergd_mux_sel_f5a_h = {mmx_sel_immres_f5a_h, ~nshc_sel_f5a_h};

always_comb
begin

  casez (mergd_mux_sel_f5a_h)
    2'b00:      mergd_frac_res_hi_f5a_h = norm_shift_out_neg_f5a_h[`TXFMA_B0:`TXFMA_B32];
    2'b01:      mergd_frac_res_hi_f5a_h = norm_shift_out_neg_f5a_h[`TXFMA_A0:`TXFMA_B31];
    2'b1?:      mergd_frac_res_hi_f5a_h = {int_minmax_res_f5a_h, 1'b0};
  endcase
end



///////////////////////////////////////////////////////////////////////////////
// Zero frac detect
//////////////////////////////////////////////////////////////////////////////

txfma_frac_zero_detect
#(
  .left_bit     ( `TXFMA_A0  ),
  .right_bit    ( `TXFMA_B10 )
)
frac_zero_detect_hi_1
(
  .in_data ( mergd_frac_res_hi_f5a_h[`TXFMA_A0:`TXFMA_B10] ),
  .zero    ( frac_zero_1_f5a_h                          )
);

txfma_frac_zero_detect
#(
  .left_bit     ( `TXFMA_B11 ),
  .right_bit    ( `TXFMA_B31 )
)
frac_zero_detect_hi_2
(
  .in_data ( mergd_frac_res_hi_f5a_h[`TXFMA_B11:`TXFMA_B31] ),
  .zero    ( frac_zero_2_f5a_h                           )
);

assign  frac_zero_f5a_h = frac_zero_1_f5a_h & frac_zero_2_f5a_h;   

endmodule
