// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

/* verilator lint_off WIDTHEXPAND */  // Imported original TXFMA exponent post-processing uses macro-sized constants and sign-extension idioms.
/* verilator lint_off CASEINCOMPLETE */  // Sparse decode cases are preserved from the original TXFMA control/exponent logic.
/* verilator lint_off UNUSEDSIGNAL */  // Helper sticky/carry nets are kept verbatim even when some packed bits are unused in this build.
module txfma_e5
(
  input [VPU_FCMD_SZ-1:0]                 cmd_f5a_h,
  input [FP_RM_BITS-1:0]                  rm_f5a_h,
  input [VPU_DTYPE_SZ-1:0]                op_dtype_f5a_h,
  input [TXFMA_SIGS_SZ-1:0]               sigs_f5a_h,
  input [TXFMA_EXP_SEXT_PS_SZ-1:0]        exp_res_in_f5a_h,
  input [TXFMA_EXP_PS_SZ-1:0]             ea_hi_f5a_h,
  input [TXFMA_EXP_PS_SZ-1:0]             ec_f5a_h,
  input                                    s_res_f5a_h,
  input                                    comp_op_f5a_h,
  input                                    comp_op_true_f5a_h,
  input                                    nshc_f5a_h,
  input                                    int_minmax_mux_sel_a_f5a_h,
  input                                    int_minmax_mux_sel_c_f5a_h,
  input                                    e_force_to_zero_f5a_h,
  input                                    ediff1_zap_f5a_h,
  input                                    e_sticky1_f5a_h,
  input                                    e_sticky2_f5a_h,
  input  [TXFMA_LXD_PS_SZ-1:0]            elxd_f5a_h,
  input  [TXFMA_WSPS_ETRZ_SZ-1:0]         etrz50_f5a_h,
  input                                    use_elxd_f5a_h,
  input                                    elxd_add_in2_mux_sel_f5a_h,
  input                                    fb_hi_zero_f5a_h,
  input                                    eb_hi_lsb_f5a_h,
  output logic                             stky2g_f5a_h,
  output logic                             rmode_f5a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0] exp_res_f5a_h
);

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  elxd_res_f5a_h;
logic                              rne_f5a_h;
logic                              rup_f5a_h;
logic                              rdn_f5a_h;
logic                              rmm_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  elxd_add_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  elxd_add_m1_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  sticky1_carry_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  sticky1_sum_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_add_in1_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_add_in2_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  elxd_add_in1_mux_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  elxd_add_in2_mux_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_add_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_in1_mux_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]  e_sticky1_in2_mux_f5a_h;
logic                              sticky1_cin2_f5a_h;
logic                              elxd_add_cin_f5a_h;
logic                              exp_res_zero_sel_f5a_h;
logic                              exp_res_ea_sel_f5a_h;
logic                              exp_res_ec_sel_f5a_h;
logic                              exp_res_m1_sel_f5a_h;
logic                              exp_res_in_sel_f5a_h;


///////////////////////////////////////////////////////////////////////////////
// sticky1 in1 mux
///////////////////////////////////////////////////////////////////////////////
  //constants
always_comb
begin
    e_sticky1_in1_mux_f5a_h = 10'b0;

    case (op_dtype_f5a_h)
      VPU_DTYPE_F32:
         if (sigs_f5a_h[TXFMA_CVT] | sigs_f5a_h[TXFMA_NORM_F32]) begin
           e_sticky1_in1_mux_f5a_h = TXFMA_STICKY_K_FI;
         end else if (use_elxd_f5a_h) begin
           e_sticky1_in1_mux_f5a_h = TXFMA_STICKY_K_F32 ;
         end

      VPU_DTYPE_F16_F32:
         e_sticky1_in1_mux_f5a_h = TXFMA_STICKY_TO_G_K_F16_L;

      VPU_DTYPE_INT:
         e_sticky1_in1_mux_f5a_h = TXFMA_STICKY_TO_R_K_IF;
  endcase
end

///////////////////////////////////////////////////////////////////////////////
// sticky1 in2 mux
///////////////////////////////////////////////////////////////////////////////
always_comb
begin
  e_sticky1_in2_mux_f5a_h = 10'b0;

  if (use_elxd_f5a_h)
    e_sticky1_in2_mux_f5a_h = {{(TXFMA_EXP_SEXT_PS_SZ-$bits(elxd_f5a_h)){elxd_f5a_h[$bits(elxd_f5a_h)-1]}}, elxd_f5a_h};
end


///////////////////////////////////////////////////////////////////////////////
// sticky1 csa
///////////////////////////////////////////////////////////////////////////////
txfma_csa #(
  .Bits(TXFMA_EXP_SEXT_PS_SZ)
) exp_sticky1_csa
(
 .a_i  (e_sticky1_in1_mux_f5a_h),
 .b_i  (e_sticky1_in2_mux_f5a_h),
 .cin_i({4'b0, etrz50_f5a_h}),
 .cout_o(sticky1_carry_f5a_h),
 .sum_o(sticky1_sum_f5a_h)
);

///////////////////////////////////////////////////////////////////////////////
// sticky1 adder carry mux
///////////////////////////////////////////////////////////////////////////////
always_comb
begin
  sticky1_cin2_f5a_h = 1'b0;

  if (use_elxd_f5a_h)
  begin
    sticky1_cin2_f5a_h = 1'b1; // to compensate K2 - 1
  end
end


///////////////////////////////////////////////////////////////////////////////
// exp sticky1 adder
///////////////////////////////////////////////////////////////////////////////
assign e_sticky1_add_in1_f5a_h = sticky1_sum_f5a_h;
assign e_sticky1_add_in2_f5a_h = {sticky1_carry_f5a_h[TXFMA_EXP_SEXT_PS_SZ-2:0], 1'b0};

txfma_adder
#(
  .Width(TXFMA_EXP_SEXT_PS_SZ)
)
exp_sticky1_adder
(
  .a_i  (e_sticky1_add_in1_f5a_h),
  .b_i  (e_sticky1_add_in2_f5a_h),
  .cin_i(sticky1_cin2_f5a_h),
  .sum_o(e_sticky1_add_f5a_h)
);

////////////////////////////////////////////////////////////////////////////////
// Sticky1 bit detection
// Sticky1 and Sticky2 bits ORed when computing tensor F16F32 operations
////////////////////////////////////////////////////////////////////////////////
assign stky2g_f5a_h = e_sticky1_add_f5a_h[TXFMA_EXP_SEXT_PS_SZ-1] | e_sticky1_f5a_h | (e_sticky2_f5a_h & (op_dtype_f5a_h == VPU_DTYPE_F16_F32)) | sigs_f5a_h[TXFMA_F32_NORM];

///////////////////////////////////////////////////////////////////////////////
// exp hi lxd in1 mux
///////////////////////////////////////////////////////////////////////////////
assign elxd_add_in1_mux_f5a_h = (cmd_f5a_h==VPU_TRANS_RSQRT_FMA2) ? ~{2'b0,exp_res_in_f5a_h[7],~exp_res_in_f5a_h[7],exp_res_in_f5a_h[6:1]}
                                                                         // bias-1 + -1 (mul op1) + 8 (exp op3)
                              : (cmd_f5a_h==VPU_TRANS_LOG_MUL)    ? 10'd133 - (10'd7 & {TXFMA_EXP_SEXT_PS_SZ{exp_res_in_f5a_h[6:0]=='0}})
                              :                                    exp_res_in_f5a_h;


///////////////////////////////////////////////////////////////////////////////
// exp hi lxd in2 mux
///////////////////////////////////////////////////////////////////////////////
assign elxd_add_in2_mux_f5a_h = (elxd_add_in2_mux_sel_f5a_h | sigs_f5a_h[TXFMA_F32_FX] | sigs_f5a_h[TXFMA_FX_F32]) ? 7'b0
                                  : (cmd_f5a_h==VPU_TRANS_RCP_FMA2)   ? {{(TXFMA_EXP_SEXT_PS_SZ-2){1'b0}},(fb_hi_zero_f5a_h)} //+1 if mantissa!=0
                                  : (cmd_f5a_h==VPU_TRANS_RSQRT_FMA2) ? {(TXFMA_EXP_SEXT_PS_SZ){!fb_hi_zero_f5a_h || !eb_hi_lsb_f5a_h}} //-1 if mant!=0 or exp b[0]
                                  : (cmd_f5a_h==VPU_TRANS_EXP_FRAC)   ? ~{2'b0,ec_f5a_h}
                                  :                                      ~({{(TXFMA_EXP_SEXT_PS_SZ-$bits(elxd_f5a_h)){elxd_f5a_h[$bits(elxd_f5a_h)-1]}}, elxd_f5a_h});

///////////////////////////////////////////////////////////////////////////////
// exp hi ELXD adder - (e - elxd)
///////////////////////////////////////////////////////////////////////////////

  // elxd_adder_cin: this is always one because we always calculating exp-1 & exp
  // and pick the correct one later once Norm Shifter corrections is available
assign elxd_add_cin_f5a_h  = ~(sigs_f5a_h[TXFMA_TRANS2] | (cmd_f5a_h==VPU_TRANS_EXP_FRAC) | sigs_f5a_h[TXFMA_F32_FX]);

txfma_adder
#(
  .Width(TXFMA_EXP_SEXT_PS_SZ)
)
exp_lxd_adder
(
  .a_i  (elxd_add_in1_mux_f5a_h),
  .b_i  (elxd_add_in2_mux_f5a_h),
  .cin_i(elxd_add_cin_f5a_h    ),
  .sum_o(elxd_add_f5a_h        )
);


///////////////////////////////////////////////////////////////////////////////
// exp hi ELXD adder - (e - elxd - 1)
///////////////////////////////////////////////////////////////////////////////
//
txfma_adder
#(
  .Width(TXFMA_EXP_SEXT_PS_SZ)
)
exp_lxd_m1_adder
(
  .a_i  (elxd_add_in1_mux_f5a_h),
  .b_i  (elxd_add_in2_mux_f5a_h),
  .cin_i(1'b0),
  .sum_o(elxd_add_m1_f5a_h)
);



///////////////////////////////////////////////////////////////////////////////
// exp hi selection
///////////////////////////////////////////////////////////////////////////////

always_comb
begin
  case (nshc_f5a_h)
    1'b0:      elxd_res_f5a_h = elxd_add_f5a_h;
    1'b1:      elxd_res_f5a_h = elxd_add_m1_f5a_h;
  endcase
end

assign exp_res_zero_sel_f5a_h = ((e_force_to_zero_f5a_h & !sigs_f5a_h[TXFMA_TRANS2]) || (comp_op_f5a_h & ~comp_op_true_f5a_h));
assign exp_res_ea_sel_f5a_h = (int_minmax_mux_sel_a_f5a_h & ~sigs_f5a_h[TXFMA_FX_F32]);
assign exp_res_ec_sel_f5a_h = ((int_minmax_mux_sel_c_f5a_h & ~sigs_f5a_h[TXFMA_F32_FX]) | ediff1_zap_f5a_h);

assign exp_res_m1_sel_f5a_h = (comp_op_f5a_h & comp_op_true_f5a_h);

assign exp_res_in_sel_f5a_h = (sigs_f5a_h[TXFMA_FX_F32] | sigs_f5a_h[TXFMA_F32_FX] | (cmd_f5a_h==VPU_TRANS_EXP_FMA2));


assign exp_res_f5a_h = exp_res_zero_sel_f5a_h ? 10'h0
                     : exp_res_ea_sel_f5a_h   ? {2'b0,ea_hi_f5a_h}
                     : exp_res_ec_sel_f5a_h   ? {2'b0,ec_f5a_h}
                     : exp_res_m1_sel_f5a_h   ? 10'h3ff
                     : exp_res_in_sel_f5a_h   ? exp_res_in_f5a_h
                     :                          elxd_res_f5a_h;


///////////////////////////////////////////////////////////////////////////////
// Calculations for ovf/unf/inf/0
///////////////////////////////////////////////////////////////////////////////
assign rne_f5a_h = (rm_f5a_h == RNE);
assign rup_f5a_h = (rm_f5a_h == RUP);
assign rdn_f5a_h = (rm_f5a_h == RDN);
assign rmm_f5a_h = (rm_f5a_h == RMM);

assign rmode_f5a_h = rne_f5a_h | rmm_f5a_h | (rup_f5a_h & ~s_res_f5a_h) | (rdn_f5a_h & s_res_f5a_h);
endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on CASEINCOMPLETE */
/* verilator lint_on WIDTHEXPAND */
