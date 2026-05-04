// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_e4 
(
  input  [`VPU_FCMD_SZ-1:0]                cmd_f4a_h,
  input                                    sign_hi_f4a_h,
  input                                    e_sticky1_f4a_h,
  input                                    e_sticky2_f4a_h,
  input  [`FP_RM_BITS-1:0]                 rm_f4a_h,
  input  [`VPU_DTYPE_SZ-1:0]               op_dtype_f4a_h,
  input  [`TXFMA_SIGS_SZ-1:0]              sigs_f4a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]       e_f4a_h,
  input  [`TXFMA_LXD_PS_SZ-1:0]            lxd_f4a_h,
  input  [`TXFMA_WSPH_ETRZ_SZ-1:0]         comb_add_hi_ph_trz_f4a_h,
  input  [`TXFMA_WSPS_ETRZ_SZ-1:0]         comb_add_lo_ps_trz_f4a_h,
  input                                    use_lxd_f4a_h,
  input                                    e_lxd_add_in2_mux_sel_f4a_h,
  input                                    e_lxd_add_cin_f4a_h,
  input [`TXFMA_EXP_PS_SZ-1:0]             ea_hi_f4a_h,
  input [`TXFMA_EXP_PS_SZ-1:0]             ec_f4a_h,
  input                                    nsh_correction_f4a_h,
  input [`TXFMA_MINMAX_OPD_SEL_SZ-1:0]     int_minmax_mux_sel_f4a_h,
  input [`TXFMA_MERGE_MUX_SEL_SZ-1:0]      mergd_mux_sel_f4a_h,
  input                                    e_force_to_zero_f4a_h,
  input                                    fb_hi_zero_f4a_h,
  input                                    eb_hi_lsb_f4a_h,
  input                                    ediff1_zap_f4a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0] e_res_f4a_h,
  output logic                             e_sticky1_until_r_f4a_h,
  output logic                             e_sticky1_until_g_f4a_h,
  output logic                             rmode_f4a_h,
  output logic                             def_ovf_f4a_h,
  output logic                             pot_ovf_f4a_h,
  output logic                             def_unf_f4a_h,
  output logic                             pot_unf_f4a_h
);

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  sticky1_carry_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  sticky1_sum_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  sticky2_carry_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  sticky2_sum_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_add_in1_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_add_in2_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_lxd_add_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_lxd_add_in1_mux_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_lxd_add_in2_mux_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky_comb_add_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_in1_mux_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky2_in1_mux_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_in2_mux_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky2_in2_mux_f4a_h;
logic                              sticky1_cin1_f4a_h;
logic                              sticky1_cin2_f4a_h;
logic                              sticky2_cin1_f4a_h;
logic                              sticky2_cin2_f4a_h;
logic                              rne_f4a_h;
logic                              rup_f4a_h;
logic                              rdn_f4a_h;
logic                              rmm_f4a_h;


///////////////////////////////////////////////////////////////////////////////
// Calculations for ovf/unf/inf/0
///////////////////////////////////////////////////////////////////////////////
assign rne_f4a_h = (rm_f4a_h == RNE);
assign rup_f4a_h = (rm_f4a_h == RUP);
assign rdn_f4a_h = (rm_f4a_h == RDN);
assign rmm_f4a_h = (rm_f4a_h == RMM);

assign rmode_f4a_h = rne_f4a_h | rmm_f4a_h | rup_f4a_h & ~sign_hi_f4a_h | rdn_f4a_h & sign_hi_f4a_h;

always_comb begin
  if ((op_dtype_f4a_h == `VPU_DTYPE_F32) & ~(sigs_f4a_h[`TXFMA_FX_F32])) begin
    pot_ovf_f4a_h = (e_res_f4a_h == 10'hfe);
    def_ovf_f4a_h = $signed(e_res_f4a_h) > $signed(10'h0fe);
    pot_unf_f4a_h = (e_res_f4a_h == 10'h0);
    def_unf_f4a_h = $signed(e_res_f4a_h) < $signed(10'h0); 
  end else if (op_dtype_f4a_h == `VPU_DTYPE_F16_F32) begin
    pot_ovf_f4a_h = (e_res_f4a_h == 10'hfe) & !sigs_f4a_h[`TXFMA_MUL];
    def_ovf_f4a_h = $signed(e_res_f4a_h) > $signed(10'h0fe) & !sigs_f4a_h[`TXFMA_MUL];
    pot_unf_f4a_h = (e_res_f4a_h == 10'h0)  & !sigs_f4a_h[`TXFMA_MUL];
    def_unf_f4a_h = $signed(e_res_f4a_h) < $signed(10'h0)  & !sigs_f4a_h[`TXFMA_MUL]; 
  end else begin
    pot_ovf_f4a_h = (e_res_f4a_h == `FLOAT16_32_MAX);
    def_ovf_f4a_h = $signed(e_res_f4a_h) > $signed(`FLOAT16_32_MAX);
    pot_unf_f4a_h = (e_res_f4a_h == `TXFMA_BIAS_DIFF);
    def_unf_f4a_h = $signed(e_res_f4a_h) < $signed(`TXFMA_BIAS_DIFF); 
  end
end


///////////////////////////////////////////////////////////////////////////////
// sticky1 in1 mux
///////////////////////////////////////////////////////////////////////////////
  //constants
always_comb begin
    e_sticky1_in1_mux_f4a_h = `TXFMA_EXP_SEXT_PS_SZ'b0;

    case (op_dtype_f4a_h)
      `VPU_DTYPE_F32:
         if (sigs_f4a_h[`TXFMA_CVT] | sigs_f4a_h[`TXFMA_NORM_F32])
                                          e_sticky1_in1_mux_f4a_h = `TXFMA_STICKY_K_FI -1;
         else if (use_lxd_f4a_h)
                                          e_sticky1_in1_mux_f4a_h = `TXFMA_STICKY_K_F32 -1;
      `VPU_DTYPE_F16_F32:  e_sticky1_in1_mux_f4a_h = `TXFMA_STICKY_TO_R_K_F16_L-1; 
      `VPU_DTYPE_INT:                     e_sticky1_in1_mux_f4a_h = `TXFMA_STICKY_TO_R_K_IF -1;
  endcase
end

assign e_sticky2_in1_mux_f4a_h = 0;


///////////////////////////////////////////////////////////////////////////////
// sticky1 in2 mux
///////////////////////////////////////////////////////////////////////////////
always_comb
begin
  e_sticky1_in2_mux_f4a_h=`TXFMA_EXP_SEXT_PS_SZ'b0;

  if (use_lxd_f4a_h) begin
      e_sticky1_in2_mux_f4a_h={{(`TXFMA_EXP_SEXT_PS_SZ-`TXFMA_LXD_PS_SZ){lxd_f4a_h[`TXFMA_LXD_PS_SZ-1]}}, lxd_f4a_h};      
  end
end


///////////////////////////////////////////////////////////////////////////////
// sticky2 in2 mux
///////////////////////////////////////////////////////////////////////////////

assign e_sticky2_in2_mux_f4a_h = 0;   



///////////////////////////////////////////////////////////////////////////////
// sticky1 csa
///////////////////////////////////////////////////////////////////////////////
txfma_csa #`TXFMA_EXP_SEXT_PS_SZ exp_sticky1_csa
(
 .a    (e_sticky1_in1_mux_f4a_h),
 .b    (e_sticky1_in2_mux_f4a_h),
 .cin  ({4'b0, comb_add_lo_ps_trz_f4a_h}),
 .cout (sticky1_carry_f4a_h),
 .sum  (sticky1_sum_f4a_h)
);


assign sticky2_sum_f4a_h = 0;

///////////////////////////////////////////////////////////////////////////////
// sticky1 adder carry mux
///////////////////////////////////////////////////////////////////////////////
always_comb
begin
  sticky1_cin1_f4a_h = 1'b0;
  sticky1_cin2_f4a_h = 1'b0;
  
  if (use_lxd_f4a_h)
  begin
    sticky1_cin1_f4a_h =  nsh_correction_f4a_h;
    sticky1_cin2_f4a_h = 1'b1; // to compensate K2 - 1
  end
end


assign  sticky2_cin1_f4a_h = 0;
assign  sticky2_cin2_f4a_h = 0;


///////////////////////////////////////////////////////////////////////////////
// exp sticky1 adder
///////////////////////////////////////////////////////////////////////////////
assign e_sticky1_add_in1_f4a_h = sticky1_sum_f4a_h;
assign e_sticky1_add_in2_f4a_h = {sticky1_carry_f4a_h[`TXFMA_EXP_SEXT_PS_SZ-2:0], sticky1_cin1_f4a_h};
txfma_adder
#(
  .WIDTH(`TXFMA_EXP_SEXT_PS_SZ)
)
exp_sticky1_adder
(
  .a    (e_sticky1_add_in1_f4a_h),
  .b    (e_sticky1_add_in2_f4a_h),
  .cin  (sticky1_cin2_f4a_h),
  .sum  (e_sticky_comb_add_f4a_h)
);


////////////////////////////////////////////////////////////////////////////////
// Sticky1 bit detection
// Sticky1 and Sticky2 bits ORed when computing tensor F16F32 operations
////////////////////////////////////////////////////////////////////////////////
assign e_sticky1_until_r_f4a_h = e_sticky_comb_add_f4a_h[`TXFMA_EXP_SEXT_PS_SZ-1] | e_sticky1_f4a_h | (e_sticky2_f4a_h & (op_dtype_f4a_h == `VPU_DTYPE_F16_F32));
assign e_sticky1_until_g_f4a_h = e_sticky1_f4a_h | (e_sticky2_f4a_h & (op_dtype_f4a_h == `VPU_DTYPE_F16_F32));

///////////////////////////////////////////////////////////////////////////////
// exp hi lxd in1 mux
///////////////////////////////////////////////////////////////////////////////
assign e_lxd_add_in1_mux_f4a_h = (cmd_f4a_h==`VPU_TRANS_RSQRT_FMA2) ? ~{e_f4a_h[7],~e_f4a_h[7],e_f4a_h[6:1]}
                               : (cmd_f4a_h==`VPU_TRANS_LOG_MUL)    ? `TXFMA_EXP_SEXT_PS_SZ'd133 - (`TXFMA_EXP_SEXT_PS_SZ'd7 & {`TXFMA_EXP_SEXT_PS_SZ{e_f4a_h[6:0]=='0}}) // bias-1 + -1 (mul op1) + 8 (exp op3)
                               :                                      e_f4a_h;


///////////////////////////////////////////////////////////////////////////////
// exp hi lxd in2 mux
///////////////////////////////////////////////////////////////////////////////
assign e_lxd_add_in2_mux_f4a_h = (e_lxd_add_in2_mux_sel_f4a_h | sigs_f4a_h[`TXFMA_F32_FX] | sigs_f4a_h[`TXFMA_FX_F32]) ? `TXFMA_LXD_PS_SZ'b0
                                  : (cmd_f4a_h==`VPU_TRANS_RCP_FMA2)   ? {{(`TXFMA_EXP_SEXT_PS_SZ-2){1'b0}},(fb_hi_zero_f4a_h)} //+1 if mantissa!=0
                                  : (cmd_f4a_h==`VPU_TRANS_RSQRT_FMA2) ? {(`TXFMA_EXP_SEXT_PS_SZ){!fb_hi_zero_f4a_h || !eb_hi_lsb_f4a_h}} //-1 if mant!=0 or exp b[0]
                                  : (cmd_f4a_h==`VPU_TRANS_EXP_FRAC)   ? ~ec_f4a_h
                                  :                                      ~{{(`TXFMA_EXP_SEXT_PS_SZ-`TXFMA_LXD_PS_SZ){lxd_f4a_h[`TXFMA_LXD_PS_SZ-1]}}, lxd_f4a_h};
                               

///////////////////////////////////////////////////////////////////////////////
// exp hi LXD adder
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH(`TXFMA_EXP_SEXT_PS_SZ)
)
exp_hi_lxd_adder
(
  .a    (e_lxd_add_in1_mux_f4a_h),
  .b    (e_lxd_add_in2_mux_f4a_h),
  .cin  (e_lxd_add_cin_f4a_h),
  .sum  (e_lxd_add_f4a_h)
);


///////////////////////////////////////////////////////////////////////////////
// exp hi selection
///////////////////////////////////////////////////////////////////////////////
assign e_res_f4a_h = ((e_force_to_zero_f4a_h & !sigs_f4a_h[`TXFMA_TRANS2]) ||
                        (mergd_mux_sel_f4a_h==`TXFMA_MERGE_MUX_SEL_COMP_FALSE))                                                       ? `TXFMA_EXP_SEXT_PS_SZ'h0
                      : ((int_minmax_mux_sel_f4a_h==`TXFMA_INT_MINMAX_SEL_A) & ~sigs_f4a_h[`TXFMA_FX_F32])                               ? ea_hi_f4a_h
                      : (((int_minmax_mux_sel_f4a_h==`TXFMA_INT_MINMAX_SEL_C) & ~sigs_f4a_h[`TXFMA_F32_FX]) | ediff1_zap_f4a_h)          ? ec_f4a_h
                      : (mergd_mux_sel_f4a_h==`TXFMA_MERGE_MUX_SEL_COMP_TRUE)                                                         ? `TXFMA_EXP_SEXT_PS_SZ'h3ff
                      : (cmd_f4a_h==`VPU_TRANS_EXP_FMA2)                                                                                 ? e_f4a_h
                      :                                                                                                                    e_lxd_add_f4a_h;
                      
endmodule
