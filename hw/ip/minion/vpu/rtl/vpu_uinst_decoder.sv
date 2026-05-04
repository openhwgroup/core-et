// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off CASEX */  // Faithful translation of the original decoder table, which matches wildcarded internal micro-op encodings.
/* verilator lint_off CASEOVERLAP */  // `casex` patterns intentionally overlap on don't-care register fields exactly as in the original.
module vpu_uinst_decoder
  import minion_pkg::*;
  import vpu_pkg::*;
  #(
    parameter bit EnableExtraTrans = EnableExtraTransDefault
  )
(
  input  logic [31:0]     id_inst_i,
  output vpu_ctrl_sigs_t  id_sigs_o
);

  localparam logic Y = 1'b1;
  localparam logic N = 1'b0;

  always_comb begin
    id_sigs_o = '0;

    casex (id_inst_i)
      // Packed-single
      32'bxxxxxxxxxxxxxxxxx110xxxxx0001011: id_sigs_o = '{VpuCmdMvXf,       N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,N,Y,N,N,  Y,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,N}; // FSW_PS
      32'b0000000xxxxxxxxxxxxxxxxxx1111011: id_sigs_o = '{VpuCmdAdd,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,VpuDtypeF32,     N,N,N,N,Y,  N,N,Y,Y}; // FADD_PS
      32'b0000100xxxxxxxxxxxxxxxxxx1111011: id_sigs_o = '{VpuCmdSub,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,VpuDtypeF32,     N,N,N,N,N,  Y,N,Y,Y}; // FSUB_PS
      32'b0001000xxxxxxxxxxxxxxxxxx1111011: id_sigs_o = '{VpuCmdMul,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,Y,Y,Y}; // FMUL_PS
      32'b0010100xxxxxxxxxx000xxxxx1111011: id_sigs_o = '{VpuCmdMin,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,Y}; // FMIN_PS
      32'b0010100xxxxxxxxxx001xxxxx1111011: id_sigs_o = '{VpuCmdMax,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,Y}; // FMAX_PS
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1011011: id_sigs_o = '{VpuCmdMadd,       Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  Y,VpuDtypeF32,     N,N,Y,N,N,  N,N,Y,Y}; // FMADD_PS
      32'b110000000000xxxxxxxxxxxxx1111011: id_sigs_o = '{VpuCmdCvtIntf32,  Y,N,N,  N,Y,N,N,  N,N,Y,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,Y,Y}; // FCVT_PW_PS
      32'b110100000000xxxxxxxxxxxxx1111011: id_sigs_o = '{VpuCmdCvtF32int,  Y,N,N,  N,Y,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,VpuDtypeInt,     N,N,N,N,N,  N,N,Y,Y}; // FCVT_PS_PW
      32'b0000000xxxxxxxxxx000xxxxx1110111: id_sigs_o = '{VpuCmdFcmovm,     N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,Y,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,N}; // FCMOVM_PS

      // Trans
      32'b0000000xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransRcpFma1, Y,N,Y,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,N}; // TR_FMA1_RCP
      32'b0000001xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransRcpFma2, Y,N,N,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  Y,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,Y}; // TR_FMA2_RCP
      32'b0000010xxxxxxxxxxxxxxxxxx0110011: if (EnableExtraTrans) id_sigs_o = '{VpuTransRsqrtFma1,Y,N,Y, N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,N}; // TR_FMA1_RSQRT
      32'b0000011xxxxxxxxxxxxxxxxxx0110011: if (EnableExtraTrans) id_sigs_o = '{VpuTransRsqrtFma2,Y,N,N, N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  Y,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,Y}; // TR_FMA2_RSQRT
      32'b0000111xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransLogFma1, Y,Y,Y,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,N}; // TR_FMA1_LOG
      32'b0001000xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransLogFma2, Y,Y,N,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,N}; // TR_FMA2_LOG
      32'b0001001xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransLogMul,  Y,N,N,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  Y,N,Y,N,N,  N,Y,N,  Y,VpuDtypeF32,     N,N,Y,N,N,  N,Y,N,Y}; // TR_MUL_LOG
      32'b0001010xxxxxxxxxxxxxxxxxx0110011: if (EnableExtraTrans) id_sigs_o = '{VpuTransSinTrans,Y,N,N, N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,Y,N}; // TR_TR_SIN
      32'b0001011xxxxxxxxxxxxxxxxxx0110011: if (EnableExtraTrans) id_sigs_o = '{VpuTransSinRr,   N,N,Y, N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,N,N,N,  N,N,N,N}; // TR_RR_SIN
      32'b0001100xxxxxxxxxxxxxxxxxx0110011: if (EnableExtraTrans) id_sigs_o = '{VpuTransSinP1,   Y,N,Y, N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,Y,Y,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,Y,Y,N}; // TR_P1_SIN
      32'b0001101xxxxxxxxxxxxxxxxxx0110011: if (EnableExtraTrans) id_sigs_o = '{VpuTransSinP2,   Y,N,Y, N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,Y,N,N,  N,N,Y,N}; // TR_P2_SIN
      32'b0001110xxxxxxxxxxxxxxxxxx0110011: if (EnableExtraTrans) id_sigs_o = '{VpuTransSinP3,   Y,N,N, N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  Y,N,Y,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,Y,Y,Y}; // TR_P3_SIN
      32'b0001111xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransExpRr,   N,N,Y,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,N}; // TR_RR_EXP
      32'b0010000xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransExpFma1, Y,N,Y,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  N,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,N}; // TR_FMA1_EXP
      32'b0010001xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuTransExpFma2, Y,N,N,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  Y,N,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,Y}; // TR_FMA2_EXP
      32'b0000100xxxxxxxxxxxxxxxxxx0110011: id_sigs_o = '{VpuCmdNr2Frcpfxp,Y,N,N,  N,N,Y,N,  N,N,N,N,  N,N,N,N,N,  Y,N,Y,Y,N,  N,N,N,  Y,VpuDtypeInt,     N,N,Y,N,N,  N,N,N,Y}; // TR_NR

      // Packed-integer
      32'b0000011xxxxxxxxxx000xxxxx1111011: id_sigs_o = '{VpuCmdAdd,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,VpuDtypeInt,     N,N,N,N,Y,  N,N,N,N}; // FADD_PI
      32'b0000111xxxxxxxxxx000xxxxx1111011: id_sigs_o = '{VpuCmdSub,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,VpuDtypeInt,     N,N,N,N,N,  Y,N,N,N}; // FSUB_PI
      32'b0010111xxxxxxxxxx000xxxxx1111011: id_sigs_o = '{VpuCmdMin,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,N,N,N,  N,N,N,N}; // FMIN_PI
      32'b0010111xxxxxxxxxx001xxxxx1111011: id_sigs_o = '{VpuCmdMax,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,N,N,N,  N,N,N,N}; // FMAX_PI
      32'b0000011xxxxxxxxxx100xxxxx1111011: id_sigs_o = '{VpuCmdXor,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,VpuDtypeInt,     N,N,N,N,N,  N,N,N,N}; // FXOR_PI
      32'b000001100000xxxxx011xxxxx1111011: id_sigs_o = '{VpuCmdFsat8,     N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,N}; // FSAT8_PI
      32'b000001100001xxxxx011xxxxx1111011: id_sigs_o = '{VpuCmdFsatu8,    N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,N}; // FSATU8_PI
      32'b001001100000xxxxx000xxxxx1111011: id_sigs_o = '{VpuCmdPackrepb,  N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,VpuDtypeF32,     N,N,N,N,N,  N,N,N,N}; // FPACKREPB_PI

      // Tensor
      32'bxxx0101xxxxxxxxxxxxxxxxxx1000011: id_sigs_o = '{VpuCmdMul,       Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,VpuDtypeF16f32,  N,N,N,N,N,  N,Y,Y,Y}; // TMUL_PH_PS
      32'bxxxxx01xxxxxxxxxxxxxxxxxx1001011: id_sigs_o = '{VpuCmdMadd,      Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  Y,VpuDtypeF16f32,  N,N,Y,N,N,  N,N,Y,Y}; // TFMA_PH_PS

      default: begin
      end
    endcase
  end

endmodule
/* verilator lint_on CASEOVERLAP */
/* verilator lint_on CASEX */
