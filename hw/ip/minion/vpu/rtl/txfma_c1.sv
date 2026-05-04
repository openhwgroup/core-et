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

/* verilator lint_off UNUSEDSIGNAL */  // Imported original TXFMA control stage keeps the packed signal bus unchanged.
module txfma_c1
(
  input                               ctrl_valid_f1a_h,
  input  [TXFMA_SIGS_SZ-1:0]         sigs_f1a_h,
  input  [VPU_FCMD_SZ-1:0]           cmd_in_f1a_h,
  input  [VPU_DTYPE_SZ-1:0]          op_dtype_f1a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]       ea_hi_f1a_h,
  input                               fa_hi_zero_f1a_h,
  input                               sa_hi_f1a_h,
  input                               sb_hi_f1a_h,
  input                               sc_f1a_h,
  input                               sa_lo_f1a_h,
  input                               sb_lo_f1a_h,
  input                               ea_hi_zero_f1a_h,
  input                               eb_hi_zero_f1a_h,
  input                               ea_lo_zero_f1a_h,
  input                               eb_lo_zero_f1a_h,
  input                               flog_mul_prod_zero_f1a_h,
  output                              minmax_op_f1a_h,
  output                              prod_sign_hi_f1a_h,
  output                              prod_sign_lo_f1a_h,
  output logic                        eprod_hi_zero_f1a_h,
  output                              eprod_lo_zero_f1a_h,
  output logic                        norm_f32_ovf_f1a_h,
  output logic                        norm_one_out_f1a_h,
  output logic                        paddin1_sels_f1a_h,
  output logic                        paddin2_selc_f1a_h,
  output logic                        paddin2_selfcneg_f1a_h,
  output logic                        padd_cin_f1a_h,
  output logic                        sel_famask_f1a_h,
  output logic                        sel_fcmask_f1a_h,
  output logic                        fa_en_f1a_h,
  output logic [VPU_FCMD_SZ-1:0]     cmd_f1a_h,
  output logic                        use_mul_f1a_h
);

///////////////////////////////////////////////////////////////////////////////
// Local Signals
///////////////////////////////////////////////////////////////////////////////

logic     fc_neg_f1a_h;
logic     comp_op_f1a_h;
logic     use_product_f1a_h;
logic     cvt_norm_trans_valid_f1a_h;

  // product sign

assign prod_sign_hi_f1a_h = (sa_hi_f1a_h ^ (sb_hi_f1a_h & sigs_f1a_h[TXFMA_REN2])) | (cmd_in_f1a_h==VPU_TRANS_EXP_FRAC);
assign prod_sign_lo_f1a_h = sa_lo_f1a_h ^ sb_lo_f1a_h;

  // product zero detection
always_comb begin
  eprod_hi_zero_f1a_h = (ea_hi_zero_f1a_h | eb_hi_zero_f1a_h);

  if (cmd_f1a_h == VPU_TRANS_LOG_MUL)
    eprod_hi_zero_f1a_h = flog_mul_prod_zero_f1a_h;
end


assign eprod_lo_zero_f1a_h = ea_lo_zero_f1a_h | eb_lo_zero_f1a_h;

assign norm_f32_ovf_f1a_h = ~fa_hi_zero_f1a_h & (ea_hi_f1a_h == FLOAT32_E_BIAS) | (ea_hi_f1a_h > FLOAT32_E_BIAS);
assign norm_one_out_f1a_h = fa_hi_zero_f1a_h & (ea_hi_f1a_h == FLOAT32_E_BIAS);


assign fa_en_f1a_h = (op_dtype_f1a_h!=VPU_DTYPE_F16_F32) |
                     !((op_dtype_f1a_h==VPU_DTYPE_F16_F32) & (sigs_f1a_h[TXFMA_FMA] | sigs_f1a_h[TXFMA_MUL]));

assign cmd_f1a_h = cmd_in_f1a_h;

assign cvt_norm_trans_valid_f1a_h = (cmd_f1a_h == VPU_FCMD_CVT_UN24F32) |
                                    (cmd_f1a_h == VPU_FCMD_CVT_UN16F32) |
                                    (cmd_f1a_h == VPU_FCMD_CVT_UN10F32) |
                                    (cmd_f1a_h == VPU_FCMD_CVT_UN8F32)  |
                                    (cmd_f1a_h == VPU_FCMD_CVT_UN2F32)  |
                                    (cmd_f1a_h == VPU_FCMD_CVT_SN16F32) |
                                    (cmd_f1a_h == VPU_FCMD_CVT_SN8F32);

assign use_mul_f1a_h = ctrl_valid_f1a_h & (sigs_f1a_h[TXFMA_FMA] | sigs_f1a_h[TXFMA_MUL] | sigs_f1a_h[TXFMA_FMS] | cvt_norm_trans_valid_f1a_h);


////////////////////////////////////////////////////////////////////////////////
// The following logic was copied or moved here from txfma_c2 to support timing
////////////////////////////////////////////////////////////////////////////////

  // min/max operation
assign minmax_op_f1a_h = (cmd_in_f1a_h==VPU_FCMD_MIN) |(cmd_in_f1a_h==VPU_FCMD_MAX);

  // compare operation
assign comp_op_f1a_h   = (cmd_in_f1a_h==VPU_FCMD_FLE) | (cmd_in_f1a_h==VPU_FCMD_FLT) | (cmd_in_f1a_h==VPU_FCMD_FEQ) | (cmd_in_f1a_h == VPU_FCMD_CLASS);

  //prod adder cin
assign fc_neg_f1a_h =  minmax_op_f1a_h | comp_op_f1a_h | (sigs_f1a_h[TXFMA_SGCVT] & sigs_f1a_h[TXFMA_F32_NORM] & sc_f1a_h) | (cmd_in_f1a_h==VPU_TRANS_EXP_FRAC);

  // Use product CSA
assign use_product_f1a_h = sigs_f1a_h[TXFMA_FMA] | sigs_f1a_h[TXFMA_FMS] | sigs_f1a_h[TXFMA_MUL] | sigs_f1a_h[TXFMA_NORM_F32];


///////////////////////////////////////////////////////////////////////////////
// The following signals are newly created and used in txfma_f2 (they are
// flopped there to meet timing rather than at txfma_top)
///////////////////////////////////////////////////////////////////////////////

assign paddin1_sels_f1a_h     = use_product_f1a_h;
assign paddin2_selc_f1a_h     = use_product_f1a_h;
assign paddin2_selfcneg_f1a_h = fc_neg_f1a_h;
assign padd_cin_f1a_h         = fc_neg_f1a_h;
assign sel_famask_f1a_h       = (cmd_in_f1a_h==VPU_FCMD_FFRC) | (cmd_in_f1a_h==VPU_FCMD_FFRC_SIN);
assign sel_fcmask_f1a_h       = sel_famask_f1a_h;

endmodule
/* verilator lint_on UNUSEDSIGNAL */
