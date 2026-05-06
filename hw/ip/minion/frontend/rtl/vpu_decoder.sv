// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// VPU instruction decoder.
//
// Pure combinational casex decode table mapping instruction bit-patterns
// to VPU control signals. Reimplementation of the original
// etcore-soc/rtl/shire/minion/vpu/vpu_decoder.v

/* verilator lint_off CASEX */
/* verilator lint_off CASEOVERLAP */
/* verilator lint_off UNUSEDPARAM */

module vpu_decoder
  import minion_pkg::*;
  import minion_frontend_pkg::*;
#(
  parameter bit EnableExtraTrans = 1'b0
) (
  input  logic [31:0]      id_inst,
  output vpu_ctrl_sigs_t   id_sigs,
  output logic             id_vpu_insn
);

  // -----------------------------------------------------------
  // Y / N shorthand
  // -----------------------------------------------------------
  /* verilator lint_off VARHIDDEN */  // Decoder table aliases intentionally mirror package Y/N constants.
  localparam logic Y = 1'b1;
  localparam logic N = 1'b0;
  /* verilator lint_on VARHIDDEN */

  // -----------------------------------------------------------
  // VPU command aliases (7-bit)
  // -----------------------------------------------------------
  localparam logic [6:0] CMD_ADD          = 7'(VpuCmdAdd);
  localparam logic [6:0] CMD_SUB          = 7'(VpuCmdSub);
  localparam logic [6:0] CMD_MUL          = 7'(VpuCmdMul);
  localparam logic [6:0] CMD_MULL         = 7'(VpuCmdMull);
  localparam logic [6:0] CMD_MULHU        = 7'(VpuCmdMulhu);
  localparam logic [6:0] CMD_MULH         = 7'(VpuCmdMulh);
  localparam logic [6:0] CMD_MADD         = 7'(VpuCmdMadd);
  localparam logic [6:0] CMD_MSUB         = 7'(VpuCmdMsub);
  localparam logic [6:0] CMD_NMSUB        = 7'(VpuCmdNmsub);
  localparam logic [6:0] CMD_NMADD        = 7'(VpuCmdNmadd);
  localparam logic [6:0] CMD_FSGNJ        = 7'(VpuCmdFsgnj);
  localparam logic [6:0] CMD_MIN          = 7'(VpuCmdMin);
  localparam logic [6:0] CMD_MAX          = 7'(VpuCmdMax);
  localparam logic [6:0] CMD_CVT_F32INT   = 7'(VpuCmdCvtF32int);
  localparam logic [6:0] CMD_MV_XF        = 7'(VpuCmdMvXf);
  localparam logic [6:0] CMD_CVT_INTF32   = 7'(VpuCmdCvtIntf32);
  localparam logic [6:0] CMD_X            = 7'(VpuCmdX);
  localparam logic [6:0] CMD_NOT          = 7'(VpuCmdNot);
  localparam logic [6:0] CMD_AND          = 7'(VpuCmdAnd);
  localparam logic [6:0] CMD_OR           = 7'(VpuCmdOr);
  localparam logic [6:0] CMD_XOR          = 7'(VpuCmdXor);
  localparam logic [6:0] CMD_SLL          = 7'(VpuCmdSll);
  localparam logic [6:0] CMD_SRL          = 7'(VpuCmdSrl);
  localparam logic [6:0] CMD_SRA          = 7'(VpuCmdSra);
  localparam logic [6:0] CMD_FEQ          = 7'(VpuCmdFeq);
  localparam logic [6:0] CMD_FLE          = 7'(VpuCmdFle);
  localparam logic [6:0] CMD_FLT          = 7'(VpuCmdFlt);
  localparam logic [6:0] CMD_FLTU         = 7'(VpuCmdFltu);
  localparam logic [6:0] CMD_MAND         = 7'(VpuCmdMand);
  localparam logic [6:0] CMD_MOR          = 7'(VpuCmdMor);
  localparam logic [6:0] CMD_MXOR         = 7'(VpuCmdMxor);
  localparam logic [6:0] CMD_MNOT         = 7'(VpuCmdMnot);
  localparam logic [6:0] CMD_FSETM        = 7'(VpuCmdFsetm);
  localparam logic [6:0] CMD_MOVA_X_M     = 7'(VpuCmdMovaXM);
  localparam logic [6:0] CMD_MOVA_M_X     = 7'(VpuCmdMovaMX);
  localparam logic [6:0] CMD_MOV_M_X      = 7'(VpuCmdMovMX);
  localparam logic [6:0] CMD_MPOPC        = 7'(VpuCmdMpopc);
  localparam logic [6:0] CMD_MPOPCZ       = 7'(VpuCmdMpopcz);
  localparam logic [6:0] CMD_FCMOV        = 7'(VpuCmdFcmov);
  localparam logic [6:0] CMD_FCMOVM       = 7'(VpuCmdFcmovm);
  localparam logic [6:0] CMD_FSWIZZ       = 7'(VpuCmdFswizz);
  localparam logic [6:0] CMD_MVZ_FX       = 7'(VpuCmdMvzFx);
  localparam logic [6:0] CMD_MVS_FX       = 7'(VpuCmdMvsFx);
  localparam logic [6:0] CMD_CVT_F32F16   = 7'(VpuCmdCvtF32f16);
  localparam logic [6:0] CMD_CVT_F32F11   = 7'(VpuCmdCvtF32f11);
  localparam logic [6:0] CMD_CVT_F32F10   = 7'(VpuCmdCvtF32f10);
  localparam logic [6:0] CMD_CVT_F32UN24  = 7'(VpuCmdCvtF32un24);
  localparam logic [6:0] CMD_CVT_F32UN16  = 7'(VpuCmdCvtF32un16);
  localparam logic [6:0] CMD_CVT_F32UN8   = 7'(VpuCmdCvtF32un8);
  localparam logic [6:0] CMD_CVT_F32UN2   = 7'(VpuCmdCvtF32un2);
  localparam logic [6:0] CMD_CVT_F32SN16  = 7'(VpuCmdCvtF32sn16);
  localparam logic [6:0] CMD_CVT_F32SN8   = 7'(VpuCmdCvtF32sn8);
  localparam logic [6:0] CMD_CVT_F32UN10  = 7'(VpuCmdCvtF32un10);
  localparam logic [6:0] CMD_CVT_F16F32   = 7'(VpuCmdCvtF16f32);
  localparam logic [6:0] CMD_CVT_F11F32   = 7'(VpuCmdCvtF11f32);
  localparam logic [6:0] CMD_CVT_F10F32   = 7'(VpuCmdCvtF10f32);
  localparam logic [6:0] CMD_CVT_UN24F32  = 7'(VpuCmdCvtUn24f32);
  localparam logic [6:0] CMD_CVT_UN16F32  = 7'(VpuCmdCvtUn16f32);
  localparam logic [6:0] CMD_CVT_UN10F32  = 7'(VpuCmdCvtUn10f32);
  localparam logic [6:0] CMD_CVT_UN8F32   = 7'(VpuCmdCvtUn8f32);
  localparam logic [6:0] CMD_CVT_UN2F32   = 7'(VpuCmdCvtUn2f32);
  localparam logic [6:0] CMD_CVT_SN16F32  = 7'(VpuCmdCvtSn16f32);
  localparam logic [6:0] CMD_CVT_SN8F32   = 7'(VpuCmdCvtSn8f32);
  localparam logic [6:0] CMD_CVT_F32UINT  = 7'(VpuCmdCvtF32uint);
  localparam logic [6:0] CMD_CVT_UINTF32  = 7'(VpuCmdCvtUintf32);
  localparam logic [6:0] CMD_FSGNJN       = 7'(VpuCmdFsgnjn);
  localparam logic [6:0] CMD_FSGNJX       = 7'(VpuCmdFsgnjx);
  localparam logic [6:0] CMD_MINU         = 7'(VpuCmdMinu);
  localparam logic [6:0] CMD_MAXU         = 7'(VpuCmdMaxu);
  localparam logic [6:0] CMD_FSATU8       = 7'(VpuCmdFsatu8);
  localparam logic [6:0] CMD_FSAT8        = 7'(VpuCmdFsat8);
  localparam logic [6:0] CMD_CVT_F32RAST  = 7'(VpuCmdCvtF32rast);
  localparam logic [6:0] CMD_CVT_RASTF32  = 7'(VpuCmdCvtRastf32);
  localparam logic [6:0] CMD_NR1_FRCPFXP  = 7'(VpuCmdNr1Frcpfxp);
  localparam logic [6:0] CMD_FFRC         = 7'(VpuCmdFfrc);
  localparam logic [6:0] CMD_FROUND       = 7'(VpuCmdFround);
  localparam logic [6:0] CMD_ADDI         = 7'(VpuCmdAddi);
  localparam logic [6:0] CMD_ANDI         = 7'(VpuCmdAndi);
  localparam logic [6:0] CMD_SLLI         = 7'(VpuCmdSlli);
  localparam logic [6:0] CMD_FBCI         = 7'(VpuCmdFbci);
  localparam logic [6:0] CMD_SRLI         = 7'(VpuCmdSrli);
  localparam logic [6:0] CMD_SRAI         = 7'(VpuCmdSrai);
  localparam logic [6:0] CMD_FBC          = 7'(VpuCmdFbc);
  localparam logic [6:0] CMD_CLASS        = 7'(VpuCmdClass);
  localparam logic [6:0] CMD_MPOPC_RAST   = 7'(VpuCmdMpocRast);
  localparam logic [6:0] CMD_CUBEFACE     = 7'(VpuCmdCubeface);
  localparam logic [6:0] CMD_CUBEFACE_IDX = 7'(VpuCmdCubefaceIdx);
  localparam logic [6:0] CMD_CUBESGNSC    = 7'(VpuCmdCubesgnsc);
  localparam logic [6:0] CMD_CUBESGNTC    = 7'(VpuCmdCubesgntc);
  localparam logic [6:0] CMD_PACKREPB     = 7'(VpuCmdPackrepb);
  localparam logic [6:0] CMD_PACKREPH     = 7'(VpuCmdPackreph);
  localparam logic [6:0] CMD_TRANS_RCP_RR = 7'(VpuTransRcpRr);
  localparam logic [6:0] CMD_TRANS_RSQRT_RR = 7'(VpuTransRsqrtRr);
  localparam logic [6:0] CMD_TRANS_LOG_RR = 7'(VpuTransLogRr);
  localparam logic [6:0] CMD_TRANS_EXP_FRAC = 7'(VpuTransExpFrac);
  localparam logic [6:0] CMD_FFRC_SIN    = 7'(VpuCmdFfrcSin);

  // -----------------------------------------------------------
  // VPU dtype aliases (4-bit)
  // -----------------------------------------------------------
  localparam logic [3:0] DT_INT  = 4'(VpuDtypeInt);
  localparam logic [3:0] DT_F32  = 4'(VpuDtypeF32);
  localparam logic [3:0] DT_F16  = 4'(VpuDtypeF16);
  localparam logic [3:0] DT_F11  = 4'(VpuDtypeF11);
  localparam logic [3:0] DT_F10  = 4'(VpuDtypeF10);
  localparam logic [3:0] DT_UN24 = 4'(VpuDtypeUn24);
  localparam logic [3:0] DT_UN16 = 4'(VpuDtypeUn16);
  localparam logic [3:0] DT_SN16 = 4'(VpuDtypeSn16);
  localparam logic [3:0] DT_UN8  = 4'(VpuDtypeUn8);
  localparam logic [3:0] DT_SN8  = 4'(VpuDtypeSn8);
  localparam logic [3:0] DT_UN10 = 4'(VpuDtypeUn10);
  localparam logic [3:0] DT_UN2  = 4'(VpuDtypeUn2);

  // -----------------------------------------------------------
  // Raw bit-vector for the decode table
  // -----------------------------------------------------------
  logic [$bits(vpu_ctrl_sigs_t)-1:0] raw_sigs;

  always_comb begin
    id_vpu_insn = 1'b1;

    //                                cmd              txfma shsw rom   scatter cvt trans gcvt  fromintf tointm tointf maskop  ldst m0ren mallren mren1 mren2  wen ren1 ren2 ren3 rend  swap12 swap23 swap13  vector dtype           fromint toint fma fms add  sub mul round wflags
    casex (id_inst)
      // ── FP Single ──────────────────────────────────────────────────────
      32'bxxxxxxxxxxxxxxxxx010xxxxx0000111: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,N,N,N,N,  Y,N,N,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FLW
      32'bxxxxxxxxxxxxxxxxx010xxxxx0100111: raw_sigs = {CMD_MVS_FX,       N,Y,N,  N,N,N,N,  N,N,N,N,  Y,N,N,N,N,  N,Y,Y,N,N,  Y,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSW
      32'b111100000000xxxxx000xxxxx1010011: raw_sigs = {CMD_MV_XF,        N,Y,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,N,N,N,N,  N,N,N,  N,DT_F32,  Y,N,N,N,N,  N,N,Y,N}; // FMV_S_X
      32'b110100000000xxxxxxxxxxxxx1010011: raw_sigs = {CMD_CVT_F32INT,   Y,N,N,  N,Y,N,N,  N,N,N,N,  N,N,N,N,N,  Y,N,N,N,N,  N,N,Y,  N,DT_INT,  Y,N,N,N,N,  N,N,Y,Y}; // FCVT_S_W
      32'b110100000001xxxxxxxxxxxxx1010011: raw_sigs = {CMD_CVT_F32UINT,  Y,N,N,  N,Y,N,N,  N,N,N,N,  N,N,N,N,N,  Y,N,N,N,N,  N,N,Y,  N,DT_INT,  Y,N,N,N,N,  N,N,Y,Y}; // FCVT_S_WU
      32'b111000000000xxxxx000xxxxx1010011: raw_sigs = {CMD_MVS_FX,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,N,N,N,  N,N,N,  N,DT_F32,  N,Y,N,N,N,  N,N,Y,N}; // FMV_X_S
      32'b111000000000xxxxx001xxxxx1010011: raw_sigs = {CMD_CLASS,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,N,N,N,  N,N,Y,  N,DT_F32,  N,Y,N,N,N,  N,N,N,N}; // FCLASS_S
      32'b110000000000xxxxxxxxxxxxx1010011: raw_sigs = {CMD_CVT_INTF32,   Y,N,N,  N,Y,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,N,N,N,  N,N,Y,  N,DT_F32,  N,Y,N,N,N,  N,N,Y,Y}; // FCVT_W_S
      32'b110000000001xxxxxxxxxxxxx1010011: raw_sigs = {CMD_CVT_UINTF32,  Y,N,N,  N,Y,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,N,N,N,  N,N,Y,  N,DT_F32,  N,Y,N,N,N,  N,N,Y,Y}; // FCVT_WU_S
      32'b1010000xxxxxxxxxx010xxxxx1010011: raw_sigs = {CMD_FEQ,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  N,DT_F32,  N,Y,N,N,N,  N,N,N,Y}; // FEQ_S
      32'b1010000xxxxxxxxxx001xxxxx1010011: raw_sigs = {CMD_FLT,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  N,DT_F32,  N,Y,N,N,N,  N,N,N,Y}; // FLT_S
      32'b1010000xxxxxxxxxx000xxxxx1010011: raw_sigs = {CMD_FLE,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  N,DT_F32,  N,Y,N,N,N,  N,N,N,Y}; // FLE_S
      32'b0010000xxxxxxxxxx000xxxxx1010011: raw_sigs = {CMD_FSGNJ,        N,Y,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSGNJ_S
      32'b0010000xxxxxxxxxx001xxxxx1010011: raw_sigs = {CMD_FSGNJN,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSGNJN_S
      32'b0010000xxxxxxxxxx010xxxxx1010011: raw_sigs = {CMD_FSGNJX,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSGNJX_S
      32'b0010100xxxxxxxxxx000xxxxx1010011: raw_sigs = {CMD_MIN,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FMIN_S
      32'b0010100xxxxxxxxxx001xxxxx1010011: raw_sigs = {CMD_MAX,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FMAX_S
      32'b0000000xxxxxxxxxxxxxxxxxx1010011: raw_sigs = {CMD_ADD,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  N,DT_F32,  N,N,N,N,Y,  N,N,Y,Y}; // FADD_S
      32'b0000100xxxxxxxxxxxxxxxxxx1010011: raw_sigs = {CMD_SUB,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  N,DT_F32,  N,N,N,N,N,  Y,N,Y,Y}; // FSUB_S
      32'b0001000xxxxxxxxxxxxxxxxxx1010011: raw_sigs = {CMD_MUL,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,Y,Y,Y}; // FMUL_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1000011: raw_sigs = {CMD_MADD,         Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  N,DT_F32,  N,N,Y,N,N,  N,N,Y,Y}; // FMADD_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1000111: raw_sigs = {CMD_MSUB,         Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  N,DT_F32,  N,N,N,Y,N,  N,N,Y,Y}; // FMSUB_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1001111: raw_sigs = {CMD_NMADD,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  N,DT_F32,  N,N,Y,N,N,  N,N,Y,Y}; // FNMADD_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1001011: raw_sigs = {CMD_NMSUB,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  N,DT_F32,  N,N,N,Y,N,  N,N,Y,Y}; // FNMSUB_S

      // ── Moscow ─────────────────────────────────────────────────────────
      32'bxxxxxxxxxxxxxxxxx101xxxxx0000111: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,N,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FLQ2
      32'bxxxxxxxxxxxxxxxxx101xxxxx0100111: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,N,N,N,N,  N,N,Y,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSQ2

      // ── Packed-Single: Load & Store ────────────────────────────────────
      32'bxxxxxxxxxxxxxxxxx010xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FLW_PS
      32'bxxxxxxxxxxxxxxxxx110xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,N,Y,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSW_PS

      // ── Packed-Single: Broadcast ───────────────────────────────────────
      32'bxxxxxxxxxxxxxxxxx000xxxxx0001011: raw_sigs = {CMD_FBC,          N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FBC_PS
      32'bxxxxxxxxxxxxxxxxx011xxxxx0001011: raw_sigs = {CMD_FBC,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  Y,N,N,N,N,  N,N,N,N}; // FBCX_PS
      32'bxxxxxxxxxxxxxxxxxxxxxxxxx0011111: raw_sigs = {CMD_FBCI,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FBCI_PS

      // ── Packed-Single: Gather and Scatter ──────────────────────────────
      32'b0110000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGW_PS
      32'b0101000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGH_PS
      32'b0100100xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGB_PS
      32'b1110000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCW_PS
      32'b1101000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCH_PS
      32'b1100100xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCB_PS
      32'b0010000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FG32W_PS
      32'b0001000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FG32H_PS
      32'b0000100xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FG32B_PS
      32'b1010000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,N,N,N,Y,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSC32W_PS
      32'b1001000xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,N,N,N,Y,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSC32H_PS
      32'b1000100xxxxxxxxxx001xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,N,N,N,Y,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSC32B_PS

      // ── Packed-Single: Computational ───────────────────────────────────
      32'b0000000xxxxxxxxxxxxxxxxxx1111011: raw_sigs = {CMD_ADD,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,Y,  N,N,Y,Y}; // FADD_PS
      32'b0000100xxxxxxxxxxxxxxxxxx1111011: raw_sigs = {CMD_SUB,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  Y,N,Y,Y}; // FSUB_PS
      32'b0001000xxxxxxxxxxxxxxxxxx1111011: raw_sigs = {CMD_MUL,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,Y,Y,Y}; // FMUL_PS
      32'b0010100xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_MIN,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FMIN_PS
      32'b0010100xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_MAX,          Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FMAX_PS
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1011011: raw_sigs = {CMD_MADD,         Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  Y,DT_F32,  N,N,Y,N,N,  N,N,Y,Y}; // FMADD_PS
      32'bxxxxx01xxxxxxxxxxxxxxxxxx1011011: raw_sigs = {CMD_MSUB,         Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  Y,DT_F32,  N,N,N,Y,N,  N,N,Y,Y}; // FMSUB_PS
      32'bxxxxx10xxxxxxxxxxxxxxxxxx1011011: raw_sigs = {CMD_NMSUB,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  Y,DT_F32,  N,N,N,Y,N,  N,N,Y,Y}; // FNMSUB_PS
      32'bxxxxx11xxxxxxxxxxxxxxxxxx1011011: raw_sigs = {CMD_NMADD,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  Y,DT_F32,  N,N,Y,N,N,  N,N,Y,Y}; // FNMADD_PS

      // ── Packed-Single: Compare ─────────────────────────────────────────
      32'b1010000xxxxxxxxxx010xxxxx1111011: raw_sigs = {CMD_FEQ,          Y,N,N,  N,N,N,N,  N,N,Y,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FEQ_PS
      32'b1010000xxxxxxxxxx110xxxxx1111011: raw_sigs = {CMD_FEQ,          Y,N,N,  N,N,N,N,  N,Y,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FEQM_PS
      32'b1010000xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_FLT,          Y,N,N,  N,N,N,N,  N,N,Y,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FLT_PS
      32'b1010000xxxxxxxxxx101xxxxx1111011: raw_sigs = {CMD_FLT,          Y,N,N,  N,N,N,N,  N,Y,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FLTM_PS
      32'b1010000xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_FLE,          Y,N,N,  N,N,N,N,  N,N,Y,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FLE_PS
      32'b1010000xxxxxxxxxx100xxxxx1111011: raw_sigs = {CMD_FLE,          Y,N,N,  N,N,N,N,  N,Y,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FLEM_PS
      32'b111000000000xxxxx001xxxxx1111011: raw_sigs = {CMD_CLASS,        Y,N,N,  N,N,N,N,  N,N,Y,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCLASS_PS

      // ── Packed-Single: Conversion ──────────────────────────────────────
      32'b110000000000xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_INTF32,   Y,N,N,  N,Y,N,N,  N,N,Y,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,Y,Y}; // FCVT_PW_PS
      32'b110000000001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_UINTF32,  Y,N,N,  N,Y,N,N,  N,N,Y,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,Y,Y}; // FCVT_PWU_PS
      32'b110100000000xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32INT,   Y,N,N,  N,Y,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_INT,  N,N,N,N,N,  N,N,Y,Y}; // FCVT_PS_PW
      32'b110100000001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32UINT,  Y,N,N,  N,Y,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_INT,  N,N,N,N,N,  N,N,Y,Y}; // FCVT_PS_PWU
      32'b110100000010xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32RAST,  Y,N,N,  N,Y,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FCVT_PS_RAST
      32'b110000000010xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_RASTF32,  Y,N,N,  N,Y,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_RAST_PS

      // ── Packed-Single: Sign / Move ─────────────────────────────────────
      32'b0010000xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_FSGNJ,        N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSGNJ_PS
      32'b0010000xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_FSGNJN,       N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSGNJN_PS
      32'b0010000xxxxxxxxxx010xxxxx1111011: raw_sigs = {CMD_FSGNJX,       N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSGNJX_PS
      32'b111000000xxxxxxxx000xxxxx1111011: raw_sigs = {CMD_MVZ_FX,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,Y,N,N,N,  N,N,N,N}; // FMVZ_X_PS
      32'b111000000xxxxxxxx010xxxxx1111011: raw_sigs = {CMD_MVS_FX,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,N,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,Y,N,N,N,  N,N,N,N}; // FMVS_X_PS
      32'bxxxxx10xxxxxxxxxx010xxxxx0111111: raw_sigs = {CMD_FCMOV,        N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCMOV_PS
      32'b0000000xxxxxxxxxx000xxxxx1110111: raw_sigs = {CMD_FCMOVM,       N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCMOVM_PS
      32'b1110011xxxxxxxxxxxxxxxxxx1111011: raw_sigs = {CMD_FSWIZZ,       N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSWIZZ_PS

      // ── Packed-Single: Misc ────────────────────────────────────────────
      32'b010110000010xxxxx000xxxxx1111011: raw_sigs = {CMD_FFRC,         Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,N,Y}; // FFRC_PS
      32'b010110000001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_FROUND,       Y,N,N,  N,Y,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,Y,Y}; // FROUND_PS

      // ── Transcendental ────────────────────────────────────────────────
      32'b010110000111xxxxx000xxxxx1111011: raw_sigs = {CMD_TRANS_RCP_RR,  N,N,Y,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FRCP_PS
      32'b010110001000xxxxx000xxxxx1111011: begin
        if (EnableExtraTrans) begin
          raw_sigs = {CMD_TRANS_RSQRT_RR, N,N,Y,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FRSQ_PS
        end else begin
          raw_sigs    = '0;
          id_vpu_insn = 1'b0;
        end
      end
      32'b010110000011xxxxx000xxxxx1111011: raw_sigs = {CMD_TRANS_LOG_RR,  Y,N,Y,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  Y,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FLOG_PS
      32'b010110000100xxxxx000xxxxx1111011: raw_sigs = {CMD_TRANS_EXP_FRAC,Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FEXP_PS
      32'b010110000110xxxxx000xxxxx1111011: begin
        if (EnableExtraTrans) begin
          raw_sigs = {CMD_FFRC_SIN,      Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSIN_PS
        end else begin
          raw_sigs    = '0;
          id_vpu_insn = 1'b0;
        end
      end
      32'b0011000xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_NR1_FRCPFXP,  Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,Y,N,N}; // FRCP_FIX_RAST

      // ── Graphics: Upconvert ────────────────────────────────────────────
      32'b110100001010xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32F16,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F16,  N,N,N,N,N,  N,N,N,Y}; // FCVT_PS_F16
      32'b110100001001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32F11,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F11,  N,N,N,N,N,  N,N,N,N}; // FCVT_PS_F11
      32'b110100001000xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32F10,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_F10,  N,N,N,N,N,  N,N,N,N}; // FCVT_PS_F10
      32'b110100010000xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32UN24,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_UN24, N,N,N,N,N,  N,N,Y,N}; // FCVT_PS_UN24
      32'b110100010001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32UN16,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_UN16, N,N,N,N,N,  N,N,Y,N}; // FCVT_PS_UN16
      32'b110100010010xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32UN10,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_UN10, N,N,N,N,N,  N,N,Y,N}; // FCVT_PS_UN10
      32'b110100010011xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32UN8,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_UN8,  N,N,N,N,N,  N,N,Y,N}; // FCVT_PS_UN8
      32'b110100010111xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32UN2,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_UN2,  N,N,N,N,N,  N,N,Y,N}; // FCVT_PS_UN2
      32'b110100011001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32SN16,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_SN16, N,N,N,N,N,  N,N,Y,N}; // FCVT_PS_SN16
      32'b110100011011xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F32SN8,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,Y,  Y,DT_SN8,  N,N,N,N,N,  N,N,Y,N}; // FCVT_PS_SN8

      // ── Graphics: Downconvert ──────────────────────────────────────────
      32'b110110001001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F16F32,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,Y,Y}; // FCVT_F16_PS
      32'b110110001000xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F11F32,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_F11_PS
      32'b110110001011xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_F10F32,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_F10_PS
      32'b110110010000xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_UN24F32,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_UN24_PS
      32'b110110010001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_UN16F32,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_UN16_PS
      32'b110110010010xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_UN10F32,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_UN10_PS
      32'b110110010011xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_UN8F32,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_UN8_PS
      32'b110110010111xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_UN2F32,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_UN2_PS
      32'b110110011001xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_SN16F32,  Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_SN16_PS
      32'b110110011011xxxxxxxxxxxxx1111011: raw_sigs = {CMD_CVT_SN8F32,   Y,N,N,  N,N,N,Y,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FCVT_SN8_PS

      // ── Packed-Integer (TXFMA): Arithmetic ─────────────────────────────
      32'b0001011xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_MULL,         Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,Y,N,N}; // FMUL_PI
      32'b0001011xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_MULH,         Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,Y,N,N}; // FMULH_PI
      32'b0001011xxxxxxxxxx010xxxxx1111011: raw_sigs = {CMD_MULHU,        Y,N,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,Y,N,N}; // FMULHU_PI

      // ── Packed-Integer (Short): Arithmetic ─────────────────────────────
      32'b0000011xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_ADD,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_INT,  N,N,N,N,Y,  N,N,N,N}; // FADD_PI
      32'bxxxxx10xxxxxxxxxx000xxxxx0111111: raw_sigs = {CMD_ADDI,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_INT,  N,N,N,N,Y,  N,N,N,N}; // FADDI_PI
      32'b0000111xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_SUB,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,Y,N,  Y,DT_INT,  N,N,N,N,N,  Y,N,N,N}; // FSUB_PI
      32'b0010111xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_MIN,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FMIN_PI
      32'b0010111xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_MAX,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FMAX_PI
      32'b0010111xxxxxxxxxx010xxxxx1111011: raw_sigs = {CMD_MINU,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FMINU_PI
      32'b0010111xxxxxxxxxx011xxxxx1111011: raw_sigs = {CMD_MAXU,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FMAXU_PI

      // ── Packed-Integer (Short): Logic ──────────────────────────────────
      32'b0000011xxxxxxxxxx111xxxxx1111011: raw_sigs = {CMD_AND,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FAND_PI
      32'b0000011xxxxxxxxxx110xxxxx1111011: raw_sigs = {CMD_OR,           N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FOR_PI
      32'b0000011xxxxxxxxxx010xxxxx1111011: raw_sigs = {CMD_NOT,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FNOT_PI
      32'b0000011xxxxxxxxxx100xxxxx1111011: raw_sigs = {CMD_XOR,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FXOR_PI
      32'b0000011xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_SLL,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FSLL_PI
      32'b0000011xxxxxxxxxx101xxxxx1111011: raw_sigs = {CMD_SRL,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FSRL_PI
      32'b0000111xxxxxxxxxx101xxxxx1111011: raw_sigs = {CMD_SRA,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FSRA_PI

      // ── Packed-Integer (Short): Compare ────────────────────────────────
      32'b1010011xxxxxxxxxx010xxxxx1111011: raw_sigs = {CMD_FEQ,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FEQ_PI
      32'b1010011xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_FLT,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FLT_PI
      32'b1010011xxxxxxxxxx011xxxxx1111011: raw_sigs = {CMD_FLTU,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FLTU_PI
      32'b1010011xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_FLE,          N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FLE_PI
      32'b0011111xxxxxxxxxx00000xxx1111011: raw_sigs = {CMD_FLT,          N,Y,N,  N,N,N,N,  N,Y,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FLTM_PI
      32'b101001100000xxxxx10000xxx1111011: raw_sigs = {CMD_FSETM,        N,Y,N,  N,N,N,N,  N,Y,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSETM_PI
      32'b000001100000xxxxx011xxxxx1111011: raw_sigs = {CMD_FSAT8,        N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSAT8_PI
      32'b000001100001xxxxx011xxxxx1111011: raw_sigs = {CMD_FSATU8,       N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSATU8_PI

      // ── Packed-Integer (Short): Immediate ──────────────────────────────
      32'bxxxxx10xxxxxxxxxx001xxxxx0111111: raw_sigs = {CMD_ANDI,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FANDI_PI
      32'b0100111xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_SLLI,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FSLLI_PI
      32'bxxxxxxxxxxxxxxxxxxxxxxxxx1011111: raw_sigs = {CMD_FBCI,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FBCI_PI
      32'b0100111xxxxxxxxxx101xxxxx1111011: raw_sigs = {CMD_SRLI,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FSRLI_PI
      32'b0100111xxxxxxxxxx111xxxxx1111011: raw_sigs = {CMD_SRAI,         N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // FSRAI_PI

      // ── Pack and Replicate ─────────────────────────────────────────────
      32'b001001100000xxxxx000xxxxx1111011: raw_sigs = {CMD_PACKREPB,     N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FPACKREPB_PI
      32'b001001100000xxxxx001xxxxx1111011: raw_sigs = {CMD_PACKREPH,     N,Y,N,  N,N,N,N,  Y,N,N,N,  N,Y,N,N,N,  Y,Y,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FPACKREPH_PI

      // ── Cubeface ───────────────────────────────────────────────────────
      32'b1000100xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_CUBEFACE,     N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,Y,Y,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // CUBEF_PS
      32'b1000100xxxxxxxxxx001xxxxx1111011: raw_sigs = {CMD_CUBEFACE_IDX, N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_INT,  N,N,N,N,N,  N,N,N,N}; // CUBEFIDX_PS
      32'b1000100xxxxxxxxxx010xxxxx1111011: raw_sigs = {CMD_CUBESGNSC,    N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // CUBESGNSC_PS
      32'b1000100xxxxxxxxxx011xxxxx1111011: raw_sigs = {CMD_CUBESGNTC,    N,Y,N,  N,N,N,N,  N,N,N,N,  N,Y,N,N,N,  Y,Y,Y,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // CUBESGNTC_PS

      // ── Mask Extension ─────────────────────────────────────────────────
      32'b0110011xxxxxxxxxx111xxxxx1111011: raw_sigs = {CMD_MAND,         N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,Y,Y,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // MAND
      32'b0110011xxxxxxxxxx110xxxxx1111011: raw_sigs = {CMD_MOR,          N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,Y,Y,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // MOR
      32'b0110011xxxxxxxxxx100xxxxx1111011: raw_sigs = {CMD_MXOR,         N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,Y,Y,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // MXOR
      32'b011001100000xxxxx010xxxxx1111011: raw_sigs = {CMD_MNOT,         N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,Y,N,  Y,Y,N,N,N,  N,N,N,  N,DT_F32,  N,N,N,N,N,  N,N,N,N}; // MNOT
      32'b110101100000xxxxx001000001111011: raw_sigs = {CMD_MOVA_M_X,     N,N,N,  N,N,N,N,  N,Y,N,Y,  N,N,N,N,N,  Y,N,N,N,N,  N,N,N,  N,DT_F32,  Y,N,N,N,N,  N,N,N,N}; // MOVA_M_X
      32'b11010110000000000000xxxxx1111011: raw_sigs = {CMD_MOVA_X_M,     N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,Y,N,N,  Y,Y,N,N,N,  N,N,N,  N,DT_F32,  N,Y,N,N,N,  N,N,N,N}; // MOVA_X_M
      32'b0101011xxxxxxxxxxxxx00xxx1111011: raw_sigs = {CMD_MOV_M_X,      N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,N,N,  Y,N,N,N,N,  N,N,N,  N,DT_F32,  Y,N,N,N,N,  N,N,N,N}; // MOV_M_X
      32'b010100100000xxxxx000xxxxx1111011: raw_sigs = {CMD_MPOPC,        N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,Y,N,  Y,Y,N,N,N,  N,N,N,  N,DT_F32,  N,Y,N,N,N,  N,N,N,N}; // MASKPOPC
      32'b010101000000xxxxx000xxxxx1111011: raw_sigs = {CMD_MPOPCZ,       N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,Y,N,  Y,Y,N,N,N,  N,N,N,  N,DT_F32,  N,Y,N,N,N,  N,N,N,N}; // MASKPOPCZ
      32'b0101111xxxxxxxxxx000xxxxx1111011: raw_sigs = {CMD_MPOPC_RAST,   N,N,N,  N,N,N,N,  N,N,N,Y,  N,N,N,Y,Y,  Y,Y,Y,N,N,  N,N,N,  N,DT_F32,  N,Y,N,N,N,  N,N,N,N}; // MASKPOPCR

      // ── FP Atomics: Local ──────────────────────────────────────────────
      32'b0000011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOADDL_PI
      32'b0000111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOSWAPL_PI
      32'b0001011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOANDL_PI
      32'b0001111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOORL_PI
      32'b0010011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOXORL_PI
      32'b0010111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMINL_PI
      32'b0011011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMAXL_PI
      32'b0011111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMINUL_PI
      32'b0100011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMAXUL_PI
      32'b0010100xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMAXL_PS
      32'b0011000xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMINL_PS

      // ── FP Atomics: Global ─────────────────────────────────────────────
      32'b1000011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOADDG_PI
      32'b1000111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOSWAPG_PI
      32'b1001011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOANDG_PI
      32'b1001111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOORG_PI
      32'b1010011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOXORG_PI
      32'b1010111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMING_PI
      32'b1011011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMAXG_PI
      32'b1011111xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMINUG_PI
      32'b1100011xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMAXUG_PI
      32'b1010100xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMAXG_PS
      32'b1011000xxxxxxxxxx100xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FAMOMING_PS

      // ── CUDA Bypass ────────────────────────────────────────────────────
      32'b000100000000xxxxx111xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FLWL_PS
      32'b010100000000xxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,N,N,N,Y,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSWL_PS
      32'b1001000xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGWL_PS
      32'b1101000xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCWL_PS
      32'b000100100000xxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,N,N,N,N,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FLWG_PS
      32'b010100100000xxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,N,N,N,Y,  N,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSWG_PS
      32'b1001001xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGWG_PS
      32'b1101001xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCWG_PS
      32'b1000001xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGBG_PS
      32'b1100001xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCBG_PS
      32'b1000100xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGHL_PS
      32'b1100100xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCHL_PS
      32'b1000101xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGHG_PS
      32'b1100101xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCHG_PS
      32'b1000000xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_X,            N,N,N,  N,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  Y,Y,N,N,N,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FGBL_PS
      32'b1100000xxxxxxxxxx111xxxxx0001011: raw_sigs = {CMD_MV_XF,        N,N,N,  Y,N,N,N,  N,N,N,N,  Y,Y,N,N,N,  N,Y,N,N,Y,  Y,N,N,  Y,DT_F32,  N,N,N,N,N,  N,N,N,N}; // FSCBL_PS

      default: begin
        raw_sigs   = '0;
        id_vpu_insn = 1'b0;
      end
    endcase
  end

  assign id_sigs = vpu_ctrl_sigs_t'(raw_sigs);

endmodule
