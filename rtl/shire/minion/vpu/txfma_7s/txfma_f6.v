// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f6 
(
  input                                    ctrl_f5_clk,    
  input [`TXFMA_A0:`TXFMA_B31]             mergd_frac_res_hi_f6a_h,
  input [`FP_CLASS_BITS-1:0]               classify_f6a_h,
  input                                    sel_classify_out_f6a_h,
  input                                    sel_nr1_frcpres_f6a_h,
  input                                    rnd_l_ph_f6a_h,
  input                                    rnd_r_ph_f6a_h,
  input                                    rnd_l_ps_f6a_h,
  input                                    rnd_r_ps_f6a_h,
  input                                    rnd_l_int_f6a_h,
  input                                    rnd_r_int_f6a_h,
  input [`TXFMA_FTZ_SZ-1:0]                ftz_f6a_l,
  input                                    rc6_ph_f5a_h,
  input                                    rc6_ps_f5a_h,
  input                                    rc_int32_f5a_h,
  input                                    normmask1_f6a_h,
  input                                    normmask2_f6a_h,
  input                                    trans_sinp2m_f6a_h,
  input                                    s_res_f6a_h,
  input [`TXFMA_EXP_PS_SZ-1:0]             exp_res_f6a_h,
  input                                    trans2_ps_roundup_r_f5a_h,
  input                                    ps_rndovf_f6a_h,
  input                                    ph_rndovf_f6a_h,
  input                                    sel_set_bit0_one_f6a_h,
  input                                    effsub1_set_sign1_f6a_h,
  input [1:0]                              rrmux32_sel_f6a_h,
  input                                    rrmux16l_sel_f6a_l,
  input                                    f10f11mux_sel_f6a_h,
  input                                    sel_psexc_sr_f6a_h,
  input                                    sel_bit31_one_f6a_h,
  input                                    sel_b30to0_ones_f6a_h,
  input                                    sel_zerores_f6a_h,
  input                                    sel_ps_unf_f6a_h,
  input                                    sel_ps_normone_f6a_h,
  input                                    sel_ps_inf_f6a_h,
  input                                    sel_ps_cqn_f6a_h,
  input                                    sel_ps_large_f6a_h,
  input                                    sel_phsp_f6a_h,
  input                                    sel_phhi_sr_f6a_h,
  input                                    sel_phhi_large_f6a_h,
  input                                    sel_phhi_unf_f6a_h,
  input                                    sel_phhi_infres_f6a_h,
  input                                    sel_f11inf_f6a_h,
  input                                    sel_f10cqnan_f6a_h,
  input                                    sel_f11sp_f6a_h,
  input                                    sel_f10sp_f6a_h,
  input                                    sel_f11large_f6a_h,
  input                                    sel_f10large_f6a_h,
  input                                    sel_gmaxu24_f6a_h,
  input                                    sel_gmaxu16_f6a_h,
  input                                    sel_gmaxs16_f6a_h,
  input                                    sel_gmins16_f6a_h,
  input                                    sel_gmaxu2_f6a_h,
  input                                    sel_gmaxu8_f6a_h,
  input                                    sel_gmaxs8_f6a_h,
  input                                    sel_gmins8_f6a_h,
  input                                    sel_gfxmax_f6a_h,
  input [1:0]                              sel_excreshi_f6a_h,
  input [1:0]                              sel_excreslo_f6a_h,
  input [1:0]                              sel_fmares_hi_f6a_h,
  input [1:0]                              sel_fmares_lo_f6a_h,
  output                                   int_res_l_bit_f6a_h,
  output logic [`VPU_DATA_S_SZ-1:0]        fma_res_f6a_h
);



///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [31:0]                  rr32_f6a_h;
logic [31:0]                  rr16_f6a_h;
logic [31:0]                  fma_exc_res_f6a_h;
logic [23:0]                  gfx_max_res_f6a_h;
logic [23:0]                  gfx_maxmin_res_f6a_h;
logic [31:0]                  ps_exc_res_f6a_h;
logic [32:1]                  rndadd32_ina_f6a_h;
logic [31:0]                  rndres_hi_f6a_h;
logic [31:0]                  rndres32_f6a_h;
logic [31:8]                  intres32_f6a_h;
logic [15:0]                  ph_exc_res_f6a_h;
logic [15:0]                  f10f11r_f6a_h; 
logic [5:0]                   f10f11mr_f6a_h;
logic [4:0]                   f10f11exp_f6a_h;
logic [1:0]                   ps_rnd_f6a_h;
logic [1:0]                   ph_rnd_f6a_h;
logic                         int_rnd_l_f6a_h;
logic                         int_rnd_r_f6a_h;
logic                         sinp2a0_f6a_h; 
logic                         rndinc_cinb_f6a_h;
logic                         trans2_ps_roundup_r_f6a_h;
logic                         rc_ph_f6a_h;
logic                         rc_ps_f6a_h;
logic                         rc_int32_f6a_h;
logic                         rrmux16h_sel_psrovf_f6a_h;
logic                         ro_rrmux16l_sel_f6a_h;
logic                         sel_f10rf11_large_f6a_h;
logic [31:0]                  class_logfma2_nr1_frcp_f6a_h;
logic                         sf_res_f6a_h;			// Sign bit for eff_sub1
logic                         rctl0_f6a_h;
logic                         gfx_min_res_f6a_h;

///////////////////////////////////////////////////////////////////////////////
// Local flops - needed to meet timing 
///////////////////////////////////////////////////////////////////////////////

 //    CLK               DOUT                       DIN
  `FF (ctrl_f5_clk, trans2_ps_roundup_r_f6a_h, trans2_ps_roundup_r_f5a_h  )
  `FF (ctrl_f5_clk, rc_ph_f6a_h,               rc6_ph_f5a_h               )
  `FF (ctrl_f5_clk, rc_ps_f6a_h,               rc6_ps_f5a_h               )
  `FF (ctrl_f5_clk, rc_int32_f6a_h,            rc_int32_f5a_h             )


///////////////////////////////////////////////////////////////////////////////
// Round Control Logic
///////////////////////////////////////////////////////////////////////////////

assign ps_rnd_f6a_h[0]   = trans2_ps_roundup_r_f6a_h | ( rnd_r_ps_f6a_h & rc_ps_f6a_h );
assign ps_rnd_f6a_h[1]   = rnd_l_ps_f6a_h & rc_ps_f6a_h; 
assign ph_rnd_f6a_h[0]   = rnd_r_ph_f6a_h & rc_ph_f6a_h; 
assign ph_rnd_f6a_h[1]   = rnd_l_ph_f6a_h & rc_ph_f6a_h; 
assign int_rnd_l_f6a_h   = rnd_l_int_f6a_h & rc_int32_f6a_h;
assign int_rnd_r_f6a_h   = rnd_r_int_f6a_h;
// Currently no rounding in R-bit position for Integers
assign rctl0_f6a_h = int_rnd_r_f6a_h;
assign rndinc_cinb_f6a_h = (mergd_frac_res_hi_f6a_h[`TXFMA_B31] & rctl0_f6a_h); 


///////////////////////////////////////////////////////////////////////////////
// Round Adder
///////////////////////////////////////////////////////////////////////////////
assign rndadd32_ina_f6a_h = mergd_frac_res_hi_f6a_h[`TXFMA_A0:`TXFMA_B30];

txfma_rnd_adder
#(
  .left_bit     ( 32 ),
  .right_bit    ( 0  ),
  .rnd_ctl_sz   ( 5  ),
  .rnd_ps       ( 1  )
)
rnd_adder
(
  .in               ( {rndadd32_ina_f6a_h, 1'b1}                    ),
  .rnd_ctl          ( {ph_rnd_f6a_h, ps_rnd_f6a_h, int_rnd_l_f6a_h} ),
  .cin              ( rndinc_cinb_f6a_h                             ),
  .out              ( rndres_hi_f6a_h                               )
);

///////////////////////////////////////////////////////////////////////////////
// Result MUX 
//////////////////////////////////////////////////////////////////////////////

// Conditioning of TXFMA Round Adder Results

assign rndres32_f6a_h[31:22] = rndres_hi_f6a_h[31:22];
assign rndres32_f6a_h[21]    = rndres_hi_f6a_h[21] & ftz_f6a_l[1];
assign rndres32_f6a_h[20:9]  = rndres_hi_f6a_h[20:9];
assign rndres32_f6a_h[8]     = rndres_hi_f6a_h[8] & ftz_f6a_l[0];
assign rndres32_f6a_h[7:0]   = rndres_hi_f6a_h[7:0];

assign intres32_f6a_h[31:22] = rndres32_f6a_h[31:22] & {($bits(rndres32_f6a_h[31:22])){normmask1_f6a_h}};
assign intres32_f6a_h[21]    = rndres_hi_f6a_h[21] & normmask1_f6a_h; 
assign intres32_f6a_h[20:16] = rndres32_f6a_h[20:16] & {($bits(rndres32_f6a_h[20:16])){normmask1_f6a_h}};
assign intres32_f6a_h[15:9]  = rndres32_f6a_h[15:9] & {($bits(rndres32_f6a_h[15:9])){normmask2_f6a_h}};
assign intres32_f6a_h[8]     = rndres32_f6a_h[8] & normmask2_f6a_h;

assign sinp2a0_f6a_h  = trans_sinp2m_f6a_h & rndres32_f6a_h[31]; 

assign int_res_l_bit_f6a_h = rndres_hi_f6a_h[0];

///////////////////////////////////////////////////////////////////////////////
// Round Result MUX for F32 output Datatypes and the Classify Instruction 
//////////////////////////////////////////////////////////////////////////////

always_comb
begin
  casez ({sel_classify_out_f6a_h, sel_nr1_frcpres_f6a_h})
    2'b00:      class_logfma2_nr1_frcp_f6a_h = {7'b0, rndres_hi_f6a_h[24:0]}; 
    2'b01:      class_logfma2_nr1_frcp_f6a_h = {rndres32_f6a_h[30:0], 1'b0};
    2'b1?:      class_logfma2_nr1_frcp_f6a_h = {22'b0,classify_f6a_h[9:0]};
  endcase   
end

always_comb
begin
  case (rrmux32_sel_f6a_h)
    2'b00:      rr32_f6a_h = {sf_res_f6a_h, exp_res_f6a_h, rndres32_f6a_h[30:8]}; 
    2'b01:      rr32_f6a_h = {intres32_f6a_h[31:8], rndres32_f6a_h[7:0]};
    2'b10:      rr32_f6a_h = class_logfma2_nr1_frcp_f6a_h;
    2'b11:      rr32_f6a_h = {8'b0,sinp2a0_f6a_h,rndres32_f6a_h[30:8]}; 
  endcase   
end


///////////////////////////////////////////////////////////////////////////////
// Round Result MUX for F16/F10/F11 output Datatypes 
//////////////////////////////////////////////////////////////////////////////

assign sf_res_f6a_h = s_res_f6a_h | effsub1_set_sign1_f6a_h;

assign rrmux16h_sel_psrovf_f6a_h = ps_rndovf_f6a_h;
assign ro_rrmux16l_sel_f6a_h     = ~(ps_rndovf_f6a_h | rrmux16l_sel_f6a_l);

assign rr16_f6a_h[31:23] = {($bits(rr16_f6a_h[31:23])){rrmux16h_sel_psrovf_f6a_h}} & {sf_res_f6a_h, exp_res_f6a_h};
assign rr16_f6a_h[22:16] = 7'b0;
assign rr16_f6a_h[15:0]  = {($bits(rr16_f6a_h[15:0])){ro_rrmux16l_sel_f6a_h}} & {s_res_f6a_h, exp_res_f6a_h[7], exp_res_f6a_h[3:0],rndres32_f6a_h[30:21]};  

assign f10f11mr_f6a_h[5:0]  = {($bits(f10f11mr_f6a_h)){~s_res_f6a_h}} & rndadd32_ina_f6a_h[31:26];
assign f10f11exp_f6a_h[4:0] = {($bits(f10f11exp_f6a_h)){~s_res_f6a_h}} & {exp_res_f6a_h[7],exp_res_f6a_h[3:0]};  


///////////////////////////////////////////////////////////////////////////////
// Graphics Min/Max Results 
//////////////////////////////////////////////////////////////////////////////

// generating min/max constants for norms
assign  gfx_max_res_f6a_h[23:16] = {8{sel_gmaxu24_f6a_h}};
assign  gfx_max_res_f6a_h[15]    = sel_gmaxu16_f6a_h | sel_gmaxu24_f6a_h;
assign  gfx_max_res_f6a_h[14:10]  = {5{sel_gmaxu16_f6a_h | sel_gmaxu24_f6a_h | sel_gmaxs16_f6a_h}};
assign  gfx_max_res_f6a_h[9:8]  = {2{ ~(sel_gmaxu8_f6a_h | sel_gmaxu2_f6a_h | sel_gmaxs8_f6a_h)}};
assign  gfx_max_res_f6a_h[7]     = ~(sel_gmaxu2_f6a_h | sel_gmaxs8_f6a_h);
assign  gfx_max_res_f6a_h[6:3]   = {4{~sel_gmaxu2_f6a_h}};
assign  gfx_max_res_f6a_h[2]     = ~sel_gmaxu2_f6a_h; 
assign  gfx_max_res_f6a_h[1:0]   = 2'b11;

assign  gfx_min_res_f6a_h = sel_gmins8_f6a_h | sel_gmins16_f6a_h;

assign gfx_maxmin_res_f6a_h[23:16] = gfx_max_res_f6a_h[23:16] & {8{sel_gfxmax_f6a_h}};

always_comb
begin
  case (sel_gfxmax_f6a_h)
    1'b0:      gfx_maxmin_res_f6a_h[15:0] = {sel_gmins16_f6a_h, 7'b0, sel_gmins8_f6a_h, 6'b0, gfx_min_res_f6a_h}; 
    1'b1:      gfx_maxmin_res_f6a_h[15:0] = gfx_max_res_f6a_h[15:0];
  endcase   
end


///////////////////////////////////////////////////////////////////////////////
// PS/PH Exception Results 
//////////////////////////////////////////////////////////////////////////////

assign  sel_f10rf11_large_f6a_h = sel_f11large_f6a_h | sel_f10large_f6a_h;

assign  ps_exc_res_f6a_h[31]    = (s_res_f6a_h & sel_psexc_sr_f6a_h) | sel_bit31_one_f6a_h;
assign  ps_exc_res_f6a_h[30]    = ~(sel_zerores_f6a_h | sel_ps_unf_f6a_h | sel_ps_normone_f6a_h); 
assign  ps_exc_res_f6a_h[29:24] = {6{~(sel_zerores_f6a_h | sel_ps_unf_f6a_h)}}; 
assign  ps_exc_res_f6a_h[23]    = sel_b30to0_ones_f6a_h | sel_ps_cqn_f6a_h | sel_ps_inf_f6a_h; 
assign  ps_exc_res_f6a_h[22]    = sel_b30to0_ones_f6a_h | sel_ps_large_f6a_h | sel_ps_cqn_f6a_h;
assign  ps_exc_res_f6a_h[21:1]  = {21{sel_b30to0_ones_f6a_h | sel_ps_large_f6a_h}}; 
assign  ps_exc_res_f6a_h[0]     = sel_b30to0_ones_f6a_h | sel_ps_large_f6a_h | sel_set_bit0_one_f6a_h; 

assign  ph_exc_res_f6a_h[15]    = s_res_f6a_h & sel_phhi_sr_f6a_h; 
assign  ph_exc_res_f6a_h[14:11] = {4{~(sel_f11sp_f6a_h | sel_f10sp_f6a_h | sel_phhi_unf_f6a_h)}}; 
assign  ph_exc_res_f6a_h[10]    = ~(sel_phhi_large_f6a_h | sel_f10sp_f6a_h | sel_phhi_unf_f6a_h);
assign  ph_exc_res_f6a_h[9]     = ~(sel_phhi_infres_f6a_h | sel_phhi_unf_f6a_h); 
assign  ph_exc_res_f6a_h[8:7]   = {2{~sel_phsp_f6a_h}}; 
assign  ph_exc_res_f6a_h[6]     = ~(sel_phsp_f6a_h | sel_f11large_f6a_h);
assign  ph_exc_res_f6a_h[5]     = ~(sel_phsp_f6a_h | sel_f10large_f6a_h | sel_f11inf_f6a_h);
assign  ph_exc_res_f6a_h[4]     = (sel_phhi_large_f6a_h | sel_f10rf11_large_f6a_h | sel_f10cqnan_f6a_h);  
assign  ph_exc_res_f6a_h[3:0]   = {4{(sel_phhi_large_f6a_h | sel_f10rf11_large_f6a_h)}};


///////////////////////////////////////////////////////////////////////////////
// F10/F11 Result MUX 
//////////////////////////////////////////////////////////////////////////////

always_comb
begin
  case ({ph_rndovf_f6a_h, f10f11mux_sel_f6a_h})
    2'b00:      f10f11r_f6a_h[15:0] = {6'b0,f10f11exp_f6a_h[4:0],f10f11mr_f6a_h[5:1]}; 
    2'b01:      f10f11r_f6a_h[15:0] = {5'b0,f10f11exp_f6a_h[4:0],f10f11mr_f6a_h[5:0]}; 
    2'b10:      f10f11r_f6a_h[15:0] = {s_res_f6a_h,exp_res_f6a_h[7],exp_res_f6a_h[3:0],10'b0}; 
    default:    f10f11r_f6a_h[15:0] = 16'b0; 
  endcase   
end


///////////////////////////////////////////////////////////////////////////////
// FMA Exception Result MUX 
//////////////////////////////////////////////////////////////////////////////

always_comb
begin
  case (sel_excreshi_f6a_h)
    2'b00:      fma_exc_res_f6a_h[31:16] = 16'b0; 
    2'b01:      fma_exc_res_f6a_h[31:16] = ps_exc_res_f6a_h[31:16];
    2'b10:      fma_exc_res_f6a_h[31:16] = {8'b0, gfx_maxmin_res_f6a_h[23:16]};
    default:    fma_exc_res_f6a_h[31:16] = 16'b0; 
  endcase   
end

always_comb
begin
  case (sel_excreslo_f6a_h)
    2'b00:      fma_exc_res_f6a_h[15:0] = f10f11r_f6a_h; 
    2'b01:      fma_exc_res_f6a_h[15:0] = ps_exc_res_f6a_h[15:0];
    2'b10:      fma_exc_res_f6a_h[15:0] = gfx_maxmin_res_f6a_h[15:0]; 
    2'b11:      fma_exc_res_f6a_h[15:0] = ph_exc_res_f6a_h[15:0]; 
  endcase   
end


///////////////////////////////////////////////////////////////////////////////
// Final Result MUX 
//////////////////////////////////////////////////////////////////////////////

always_comb
begin
  casez (sel_fmares_hi_f6a_h)
    2'b00:      fma_res_f6a_h[31:16] = fma_exc_res_f6a_h[31:16];
    2'b01:      fma_res_f6a_h[31:16] = rr16_f6a_h[31:16];
    2'b1?:      fma_res_f6a_h[31:16] = rr32_f6a_h[31:16];  
  endcase   
end

always_comb
begin
  casez (sel_fmares_lo_f6a_h)
    2'b00:      fma_res_f6a_h[15:0] = fma_exc_res_f6a_h[15:0]; 
    2'b01:      fma_res_f6a_h[15:0] = rr16_f6a_h[15:0];
    2'b1?:      fma_res_f6a_h[15:0] = rr32_f6a_h[15:0]; 
  
  endcase   
end

endmodule
