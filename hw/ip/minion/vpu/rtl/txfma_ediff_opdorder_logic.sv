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

/* verilator lint_off WIDTHEXPAND */  // Imported original operand-order helper mixes narrow packed selectors with macro-sized literals.
/* verilator lint_off CASEINCOMPLETE */  // The original sign-combination decode intentionally leaves unused combinations uncovered.
/* verilator lint_off UNUSEDSIGNAL */  // Packed TXFMA control bits are preserved even when not every bit is consumed by this helper.
module txfma_ediff_opdorder_logic
(
  input  [VPU_FCMD_SZ-1:0]                   cmd,
  input  [VPU_DTYPE_SZ-1:0]                  op_dtype,
  input  [TXFMA_SIGS_SZ-1:0]                 sigs,
  input                                       sign_c_ph,
  input                                       sign_c_pl,
  input                                       sign_ph_pl,
  input                                       exp_fma2_exc,
  input  [6:0]                                exp_res_2f3_f2a_h,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]  align1_mux_sel,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]  align2_mux_sel,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]  opd3_mux_sel,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]  ediff1_sel,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]  ediff2_sel,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]  res_exp_hi_sel,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]  res_exp_lo_sel
);


  //look at the sign of ediff exponent c-ph, c-pl and ph-pl to select operands
always_comb begin

    //default values
  align1_mux_sel = TXFMA_SEL_PH;
  align2_mux_sel = TXFMA_SEL_PL;
  opd3_mux_sel   = TXFMA_SEL_P;
  ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
  ediff2_sel     = TXFMA_SEL_EDIFF_EC_EPL;
  res_exp_hi_sel = TXFMA_SEL_C;
  res_exp_lo_sel = TXFMA_SEL_PH;

  if (sigs[TXFMA_F32_NORM]) begin
    align1_mux_sel = TXFMA_SEL_P;
    opd3_mux_sel   = TXFMA_SEL_A;
    ediff1_sel     = TXFMA_SEL_EDIFF_ZERO;
    res_exp_hi_sel = TXFMA_SEL_GCVT_F32_NORM;
  end

    //int
  if (op_dtype == VPU_DTYPE_INT) begin
    if (sigs[TXFMA_CVT]) begin
      align1_mux_sel = TXFMA_SEL_P;
      opd3_mux_sel   = TXFMA_SEL_A;
      ediff1_sel     = TXFMA_SEL_EDIFF_ZERO;
      res_exp_hi_sel = TXFMA_SEL_CVT_F32INT;

      if (cmd==VPU_FCMD_CVT_F32RAST) begin
        res_exp_hi_sel = TXFMA_SEL_CVT_F32_FXP1615;
      end
    end

      //trans
    if (sigs[TXFMA_TRANS1] || sigs[TXFMA_TRANS2] || cmd==VPU_FCMD_NR1_FRCPFXP) begin
      res_exp_hi_sel = TXFMA_SEL_P;
      align1_mux_sel = TXFMA_SEL_C_INT;
      ediff1_sel     = TXFMA_SEL_EDIFF_ZERO;
      opd3_mux_sel   = TXFMA_SEL_P_INT;

        //sel eb in case of exception
      if (cmd==VPU_TRANS_EXP_FMA2 && exp_fma2_exc) begin
        res_exp_hi_sel = TXFMA_SEL_B;
        align1_mux_sel = TXFMA_SEL_ZERO;
        align2_mux_sel = TXFMA_SEL_ZERO;
        opd3_mux_sel   = TXFMA_SEL_ZERO;
      end
    end

    if (cmd==VPU_FCMD_NR2_FRCPFXP) begin
      ediff1_sel     = TXFMA_SEL_EDIFF_NR2_FRCPFXP;
      align1_mux_sel = TXFMA_SEL_C_INT;
      opd3_mux_sel   = TXFMA_SEL_P;
    end
  end

    //ps
  if (op_dtype == VPU_DTYPE_F32) begin
      //cvt
    if (sigs[TXFMA_CVT]) begin
      align1_mux_sel = TXFMA_SEL_C;
      ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
      res_exp_hi_sel = TXFMA_SEL_PH;
      opd3_mux_sel   = TXFMA_SEL_A; //expect to be '0'

      if (cmd==VPU_FCMD_FROUND) begin
        opd3_mux_sel = TXFMA_SEL_ROUND;
        res_exp_hi_sel = TXFMA_SEL_CVT_F32INT;
      end
    end
      //log mul
    else if (cmd==VPU_TRANS_LOG_MUL)  begin
      res_exp_hi_sel = TXFMA_SEL_P;
      align1_mux_sel = TXFMA_SEL_P;

        //do not shift mantissa when exponent is zero
      if (exp_res_2f3_f2a_h==6'b0)
        ediff1_sel     = TXFMA_SEL_EDIFF_LOG_MUL2;
      else
        ediff1_sel     = TXFMA_SEL_EDIFF_LOG_MUL;

      opd3_mux_sel   = TXFMA_SEL_EXP_RES;
    end
      //mul
    else if (sigs[TXFMA_MUL]) begin
      align1_mux_sel = TXFMA_SEL_C;
      opd3_mux_sel   = TXFMA_SEL_P;
      ediff1_sel     = TXFMA_SEL_EDIFF_ZERO;
      res_exp_hi_sel = TXFMA_SEL_PH;
    end
    else if (cmd==VPU_TRANS_LOG_RR) begin
      align1_mux_sel = TXFMA_SEL_C;
      opd3_mux_sel   = TXFMA_SEL_ZERO;
      res_exp_hi_sel = TXFMA_SEL_C;
      ediff1_sel     = TXFMA_SEL_EDIFF_ZERO;
    end
    else if((cmd == VPU_FCMD_FFRC) || (cmd==VPU_FCMD_FFRC_SIN)) begin
        align1_mux_sel = TXFMA_SEL_A;
        opd3_mux_sel   = TXFMA_SEL_C;
        ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
        res_exp_hi_sel = TXFMA_SEL_C;
    end
      //compare
    else if ((cmd==VPU_FCMD_FLE) || (cmd==VPU_FCMD_FLT) || (cmd==VPU_FCMD_FEQ)) begin
      opd3_mux_sel   = TXFMA_SEL_P;
    end
      //trans exp frac
    else if (cmd==VPU_TRANS_EXP_FRAC) begin
      align1_mux_sel = TXFMA_SEL_C;
      align2_mux_sel = TXFMA_SEL_ZERO;
      opd3_mux_sel   = TXFMA_SEL_ZERO;
      res_exp_hi_sel = TXFMA_SEL_EXP_FRAC;
      res_exp_lo_sel = TXFMA_SEL_EXP_FRAC;
      ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
      ediff2_sel     = TXFMA_SEL_EDIFF_EC_EPL;
    end
      //trans sin transform
    else if (cmd==VPU_TRANS_SIN_TRANSFORM) begin
      align1_mux_sel = TXFMA_SEL_C;
      align2_mux_sel = TXFMA_SEL_A;
      ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
      ediff2_sel     = TXFMA_SEL_ZERO;
      opd3_mux_sel   = TXFMA_SEL_ZERO;
      res_exp_hi_sel = TXFMA_SEL_PH;
    end
    else if (sigs[TXFMA_NORM_F32]) begin
      align1_mux_sel = TXFMA_SEL_P;
      align2_mux_sel = TXFMA_SEL_ZERO;
      opd3_mux_sel   = TXFMA_SEL_ZERO;
      ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
      ediff2_sel     = TXFMA_SEL_EDIFF_ZERO;
      res_exp_hi_sel = TXFMA_SEL_ZERO;
      res_exp_lo_sel = TXFMA_SEL_ZERO;
    end
    else if (sigs[TXFMA_FX_F32]) begin
      res_exp_hi_sel = TXFMA_SEL_PH;
      ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
      align1_mux_sel = TXFMA_SEL_A;
    end
      //other float
    else begin
         //p(or a)>c
      if(sign_c_ph) begin
        align1_mux_sel = TXFMA_SEL_C;
        opd3_mux_sel   = (cmd==VPU_FCMD_ADD || cmd==VPU_FCMD_SUB) ? TXFMA_SEL_A : TXFMA_SEL_P;
        ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
        res_exp_hi_sel = (cmd==VPU_FCMD_ADD || cmd==VPU_FCMD_SUB) ? TXFMA_SEL_A : TXFMA_SEL_P;
      end
        //c>p(or a)
      else begin
        align1_mux_sel = (cmd==VPU_FCMD_ADD || cmd==VPU_FCMD_SUB) ? TXFMA_SEL_A : TXFMA_SEL_P;
        opd3_mux_sel   = TXFMA_SEL_C;
        ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
        res_exp_hi_sel = TXFMA_SEL_C;
      end
    end
  end
    //txfma ps
  if (op_dtype == VPU_DTYPE_F16_F32) begin
      //mul
    if (sigs[TXFMA_MUL]) begin
      align1_mux_sel = TXFMA_SEL_C;
      ediff1_sel     = TXFMA_SEL_EDIFF_ZERO;

        //pl>ph
      if (sign_ph_pl) begin
        align2_mux_sel = TXFMA_SEL_PH;
        opd3_mux_sel   = TXFMA_SEL_PL;
        ediff2_sel     = TXFMA_SEL_EDIFF_EPH_EPL;
        res_exp_hi_sel = TXFMA_SEL_PL;
      end
        //ph>pl
      else begin
        align2_mux_sel = TXFMA_SEL_PL;
        opd3_mux_sel   = TXFMA_SEL_PH;
        ediff2_sel     = TXFMA_SEL_EDIFF_EPH_EPL;
        res_exp_hi_sel = TXFMA_SEL_PH;
      end
    end
      //fma
    else begin
      case ({sign_c_ph,sign_c_pl,sign_ph_pl})
          //c>ph>pl
        3'b000: begin
                  align1_mux_sel = TXFMA_SEL_PH;
                  align2_mux_sel = TXFMA_SEL_PL;
                  opd3_mux_sel   = TXFMA_SEL_C;
                  ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
                  ediff2_sel     = TXFMA_SEL_EDIFF_EC_EPL;
                  res_exp_hi_sel = TXFMA_SEL_C;
                end
          //c>pl>ph
        3'b001: begin
                  align1_mux_sel = TXFMA_SEL_PH;
                  align2_mux_sel = TXFMA_SEL_PL;
                  opd3_mux_sel   = TXFMA_SEL_C;
                  ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPH;
                  ediff2_sel     = TXFMA_SEL_EDIFF_EC_EPL;
                  res_exp_hi_sel = TXFMA_SEL_C;
                end
          //pl>c>ph
        3'b011: begin
                  align1_mux_sel = TXFMA_SEL_C;
                  align2_mux_sel = TXFMA_SEL_PH;
                  opd3_mux_sel   = TXFMA_SEL_PL;
                  ediff1_sel     = TXFMA_SEL_EDIFF_EC_EPL;
                  ediff2_sel     = TXFMA_SEL_EDIFF_N_EPH_EPL;
                  res_exp_hi_sel = TXFMA_SEL_PL;
                end
          //ph>c>pl
        3'b100: begin
                  align1_mux_sel = TXFMA_SEL_C;
                  align2_mux_sel = TXFMA_SEL_PL;
                  opd3_mux_sel   = TXFMA_SEL_PH;
                  ediff1_sel     = TXFMA_SEL_EDIFF_N_EC_EPH;
                  ediff2_sel     = TXFMA_SEL_EDIFF_EPH_EPL;
                  res_exp_hi_sel = TXFMA_SEL_PH;
                end
          //ph>pl>c
        3'b110: begin
                  align1_mux_sel = TXFMA_SEL_C;
                  align2_mux_sel = TXFMA_SEL_PL;
                  opd3_mux_sel   = TXFMA_SEL_PH;
                  ediff1_sel     = TXFMA_SEL_EDIFF_N_EC_EPH;
                  ediff2_sel     = TXFMA_SEL_EDIFF_EPH_EPL;
                  res_exp_hi_sel = TXFMA_SEL_PH;
                end
          //pl>ph>c
        3'b111: begin
                  align1_mux_sel = TXFMA_SEL_C;
                  align2_mux_sel = TXFMA_SEL_PH;
                  opd3_mux_sel   = TXFMA_SEL_PL;
                  ediff1_sel     = TXFMA_SEL_EDIFF_N_EC_EPL;
                  ediff2_sel     = TXFMA_SEL_EDIFF_N_EPH_EPL;
                  res_exp_hi_sel = TXFMA_SEL_PL;
                end
       endcase
     end
  end

    //f10/f11/f16 converts
  if ((op_dtype == VPU_DTYPE_F11) || (op_dtype == VPU_DTYPE_F10) || op_dtype == VPU_DTYPE_F16) begin
    res_exp_hi_sel = TXFMA_SEL_C;
  end
end

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on CASEINCOMPLETE */
/* verilator lint_on WIDTHEXPAND */
