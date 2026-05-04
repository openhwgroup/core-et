// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_decode — Integer pipeline instruction decoder.
//
// Pure combinational decoder: maps 32-bit instruction to minion_control_t.
// One-to-one reimplementation of etcore-soc intpipe_decode.v.
// Module name preserved for interface compatibility.

/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off CASEOVERLAP */
/* verilator lint_off CASEX */  // casex required for instruction decoder X-pattern matching

module intpipe_decode
  import minion_pkg::*;
  import minion_frontend_pkg::*;
#(
  parameter bit EnableExtraTrans = 1'b0
) (
  input  logic [31:0]     inst_bits,
  output minion_control_t inst_ctrl
);

  // ──────────────────────────────────────────────────────
  // Don't-care constants for unused fields.
  // Use X values matching the original for identical Verilator results.
  // ──────────────────────────────────────────────────────
  localparam logic [1:0] A1_X   = 2'bx;
  localparam logic [1:0] A2_X   = 2'bx;
  localparam logic [2:0] IMM_X  = 3'bx;
  localparam logic       DW_X   = 1'bx;
  localparam logic [4:0] FN_X   = 5'b1xxxx;  // core_alu_func_NONE
  localparam logic [4:0] M_X    = 5'bx;
  localparam logic [3:0] MT_X   = 4'bx;

  // Enum aliases
  localparam logic [1:0] A1_ZERO = 2'(A1Zero);
  localparam logic [1:0] A1_RS1  = 2'(A1Rs1);
  localparam logic [1:0] A1_PC   = 2'(A1Pc);
  localparam logic [1:0] A1_FS1  = 2'(A1Fs1);
  localparam logic [1:0] A2_ZERO = 2'(A2Zero);
  localparam logic [1:0] A2_SIZE = 2'(A2Size);
  localparam logic [1:0] A2_RS2  = 2'(A2Rs2);
  localparam logic [1:0] A2_IMM  = 2'(A2Imm);
  localparam logic [2:0] IMM_S   = 3'(ImmS);
  localparam logic [2:0] IMM_SB  = 3'(ImmSb);
  localparam logic [2:0] IMM_U   = 3'(ImmU);
  localparam logic [2:0] IMM_UJ  = 3'(ImmUj);
  localparam logic [2:0] IMM_I   = 3'(ImmI);
  localparam logic [2:0] IMM_Z   = 3'(ImmZ);
  localparam logic       DW_32   = 1'(Dw32);
  localparam logic       DW_XPR  = 1'(Dw64);
  localparam logic [4:0] FN_ADD     = 5'(AluAdd);
  localparam logic [4:0] FN_SL      = 5'(AluSl);
  localparam logic [4:0] FN_SEQ     = 5'(AluSeq);
  localparam logic [4:0] FN_SNE     = 5'(AluSne);
  localparam logic [4:0] FN_XOR     = 5'(AluXor);
  localparam logic [4:0] FN_SR      = 5'(AluSr);
  localparam logic [4:0] FN_OR      = 5'(AluOr);
  localparam logic [4:0] FN_AND     = 5'(AluAnd);
  localparam logic [4:0] FN_PACKB   = 5'(AluPackb);
  localparam logic [4:0] FN_BITMIXB = 5'(AluBitmixb);
  localparam logic [4:0] FN_SUB     = 5'(AluSub);
  localparam logic [4:0] FN_SRA     = 5'(AluSra);
  localparam logic [4:0] FN_SLT     = 5'(AluSlt);
  localparam logic [4:0] FN_SGE     = 5'(AluSge);
  localparam logic [4:0] FN_SLTU    = 5'(AluSltu);
  localparam logic [4:0] FN_SGEU    = 5'(AluSgeu);
  localparam logic [4:0] FN_MUL     = 5'(AluMul);
  localparam logic [4:0] FN_MULH    = 5'(AluMulh);
  localparam logic [4:0] FN_MULHU   = 5'(AluMulhu);
  localparam logic [4:0] FN_MULHSU  = 5'(AluMulhsu);
  localparam logic [4:0] FN_DIV     = 5'(AluDiv);
  localparam logic [4:0] FN_DIVU    = 5'(AluDivu);
  localparam logic [4:0] FN_REM     = 5'(AluRem);
  localparam logic [4:0] FN_REMU    = 5'(AluRemu);
  localparam logic [4:0] M_XRD       = 5'(DcCmdXrd);
  localparam logic [4:0] M_XWR       = 5'(DcCmdXwr);
  localparam logic [4:0] M_XRDB      = 5'(DcCmdXrdb);
  localparam logic [4:0] M_XWRB      = 5'(DcCmdXwrb);
  localparam logic [4:0] M_XA_SWAP   = 5'(DcCmdXaSwap);
  localparam logic [4:0] M_XA_ADD    = 5'(DcCmdXaAdd);
  localparam logic [4:0] M_XA_XOR    = 5'(DcCmdXaXor);
  localparam logic [4:0] M_XA_OR     = 5'(DcCmdXaOr);
  localparam logic [4:0] M_XA_AND    = 5'(DcCmdXaAnd);
  localparam logic [4:0] M_XA_MIN    = 5'(DcCmdXaMin);
  localparam logic [4:0] M_XA_MAX    = 5'(DcCmdXaMax);
  localparam logic [4:0] M_XA_MINU   = 5'(DcCmdXaMinu);
  localparam logic [4:0] M_XA_MAXU   = 5'(DcCmdXaMaxu);
  localparam logic [4:0] M_XA_CMPSWP = 5'(DcCmdXaCmpswp);
  localparam logic [4:0] M_G32       = 5'(DcCmdG32);
  localparam logic [4:0] M_SC32      = 5'(DcCmdSc32);
  localparam logic [4:0] M_XA_MIN_PS = 5'(DcCmdXaMinPs);
  localparam logic [4:0] M_XA_MAX_PS = 5'(DcCmdXaMaxPs);
  localparam logic [3:0] MT_B       = 4'(DcTypeB);
  localparam logic [3:0] MT_H       = 4'(DcTypeH);
  localparam logic [3:0] MT_W       = 4'(DcTypeW);
  localparam logic [3:0] MT_D       = 4'(DcTypeD);
  localparam logic [3:0] MT_BU      = 4'(DcTypeBu);
  localparam logic [3:0] MT_HU      = 4'(DcTypeHu);
  localparam logic [3:0] MT_WU      = 4'(DcTypeWu);
  localparam logic [3:0] MT_PS      = 4'(DcTypePs);
  localparam logic [3:0] MT_PS_BR   = 4'(DcTypePsBr);
  localparam logic [3:0] MT_PS_GS8  = 4'(DcTypePsGs8);
  localparam logic [3:0] MT_PS_GS16 = 4'(DcTypePsGs16);
  localparam logic [3:0] MT_PS_GS32 = 4'(DcTypePsGs32);
  localparam logic [2:0] CSR_N = 3'(CsrN);
  localparam logic [2:0] CSR_W = 3'(CsrW);
  localparam logic [2:0] CSR_S = 3'(CsrS);
  localparam logic [2:0] CSR_C = 3'(CsrC);
  localparam logic [2:0] CSR_I = 3'(CsrI);

  localparam logic Y = 1'b1;
  localparam logic N = 1'b0;

  // ──────────────────────────────────────────────────────
  // Decode table: raw bit concat → struct cast
  // ──────────────────────────────────────────────────────
  //
  // Layout (MSB→LSB, 46 bits):
  //   legal mcode fp br jal jalr rxs2 rxs1
  //   sel_alu2[1:0] sel_alu1[1:0] sel_imm[2:0] alu_dw alu_fn[4:0]
  //   mem mem_cmd[4:0] mem_type[3:0]
  //   rma wmd wmad wfd div wxd csr[2:0] fence mem_g gsc gfx x31

  logic [$bits(minion_control_t)-1:0] raw_inst_ctrl;

  always_comb begin
    casex (inst_bits)
      // ── Basic set ──
      //                                                       val mc fp br ja jr x2 x1 a2      a1      imm    dw     fn         m  cmd       typ        rm wm wa wf dv wx csr   fe mg gs gx x3
      32'bxxxxxxxxxxxxxxxxx001xxxxx1100011: raw_inst_ctrl = {Y,N,N,Y,N,N,Y,Y,A2_RS2, A1_RS1, IMM_SB,DW_X,  FN_SNE,  N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // BNE
      32'bxxxxxxxxxxxxxxxxx000xxxxx1100011: raw_inst_ctrl = {Y,N,N,Y,N,N,Y,Y,A2_RS2, A1_RS1, IMM_SB,DW_X,  FN_SEQ,  N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // BEQ
      32'bxxxxxxxxxxxxxxxxx100xxxxx1100011: raw_inst_ctrl = {Y,N,N,Y,N,N,Y,Y,A2_RS2, A1_RS1, IMM_SB,DW_X,  FN_SLT,  N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // BLT
      32'bxxxxxxxxxxxxxxxxx110xxxxx1100011: raw_inst_ctrl = {Y,N,N,Y,N,N,Y,Y,A2_RS2, A1_RS1, IMM_SB,DW_X,  FN_SLTU, N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // BLTU
      32'bxxxxxxxxxxxxxxxxx101xxxxx1100011: raw_inst_ctrl = {Y,N,N,Y,N,N,Y,Y,A2_RS2, A1_RS1, IMM_SB,DW_X,  FN_SGE,  N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // BGE
      32'bxxxxxxxxxxxxxxxxx111xxxxx1100011: raw_inst_ctrl = {Y,N,N,Y,N,N,Y,Y,A2_RS2, A1_RS1, IMM_SB,DW_X,  FN_SGEU, N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // BGEU
      32'bxxxxxxxxxxxxxxxxxxxxxxxxx1101111: raw_inst_ctrl = {Y,N,N,N,Y,N,N,N,A2_SIZE,A1_PC,  IMM_UJ,DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // JAL
      32'bxxxxxxxxxxxxxxxxx000xxxxx1100111: raw_inst_ctrl = {Y,N,N,N,N,Y,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // JALR
      32'bxxxxxxxxxxxxxxxxxxxxxxxxx0010111: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_IMM, A1_PC,  IMM_U, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AUIPC

      // ── Loads ──
      32'bxxxxxxxxxxxxxxxxx000xxxxx0000011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_B,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LB
      32'bxxxxxxxxxxxxxxxxx001xxxxx0000011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_H,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LH
      32'bxxxxxxxxxxxxxxxxx010xxxxx0000011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LW
      32'bxxxxxxxxxxxxxxxxx100xxxxx0000011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_BU,   N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LBU
      32'bxxxxxxxxxxxxxxxxx101xxxxx0000011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_HU,   N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LHU

      // ── Stores ──
      32'bxxxxxxxxxxxxxxxxx000xxxxx0100011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_IMM, A1_RS1, IMM_S, DW_XPR,FN_ADD,  Y,M_XWR,  MT_B,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // SB
      32'bxxxxxxxxxxxxxxxxx001xxxxx0100011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_IMM, A1_RS1, IMM_S, DW_XPR,FN_ADD,  Y,M_XWR,  MT_H,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // SH
      32'bxxxxxxxxxxxxxxxxx010xxxxx0100011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_IMM, A1_RS1, IMM_S, DW_XPR,FN_ADD,  Y,M_XWR,  MT_W,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // SW

      // ── LUI ──
      32'bxxxxxxxxxxxxxxxxxxxxxxxxx0110111: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_IMM, A1_ZERO,IMM_U, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LUI

      // ── ALU I-type ──
      32'bxxxxxxxxxxxxxxxxx000xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // ADDI
      32'bxxxxxxxxxxxxxxxxx010xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_SLT,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLTI
      32'bxxxxxxxxxxxxxxxxx011xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_SLTU, N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLTIU
      32'bxxxxxxxxxxxxxxxxx111xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_AND,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // ANDI
      32'bxxxxxxxxxxxxxxxxx110xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_OR,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // ORI
      32'bxxxxxxxxxxxxxxxxx100xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_XOR,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // XORI

      // ── Shifts I-type ──
      32'b000000xxxxxxxxxxx001xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_SL,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLLI
      32'b000000xxxxxxxxxxx101xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_SR,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRLI
      32'b010000xxxxxxxxxxx101xxxxx0010011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_SRA,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRAI

      // ── ALU R-type ──
      32'b0000000xxxxxxxxxx000xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // ADD
      32'b0100000xxxxxxxxxx000xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_SUB,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SUB
      32'b0000000xxxxxxxxxx010xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_SLT,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLT
      32'b0000000xxxxxxxxxx011xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_SLTU, N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLTU
      32'b0000000xxxxxxxxxx111xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_AND,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AND
      32'b0000000xxxxxxxxxx110xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_OR,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // OR
      32'b0000000xxxxxxxxxx100xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_XOR,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // XOR
      32'b0000000xxxxxxxxxx001xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_SL,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLL
      32'b0000000xxxxxxxxxx101xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_SR,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRL
      32'b0100000xxxxxxxxxx101xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_SRA,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRA

      // ── Misc ──
      32'bxxxxxxxxxxxxxxxxx000xxxxx0001111: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,Y,N,N,N,N}; // FENCE
      32'b00000000000000000000000001110011: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_I,N,N,N,N,N}; // ECALL
      32'b00000000000100000000000001110011: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_ZERO,A1_PC,  IMM_X, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,N,CSR_I,N,N,N,N,N}; // EBREAK
      32'b00110000001000000000000001110011: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_I,N,N,N,N,N}; // MRET
      32'b00010000010100000000000001110011: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_I,N,N,N,N,N}; // WFI

      // ── CSR ──
      32'bxxxxxxxxxxxxxxxxx001xxxxx1110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_W,N,N,N,N,N}; // CSRRW
      32'bxxxxxxxxxxxxxxxxx010xxxxx1110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_S,N,N,N,N,N}; // CSRRS
      32'bxxxxxxxxxxxxxxxxx011xxxxx1110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_C,N,N,N,N,N}; // CSRRC
      32'bxxxxxxxxxxxxxxxxx101xxxxx1110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,N,A2_IMM, A1_ZERO,IMM_Z, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_W,N,N,N,N,N}; // CSRRWI
      32'bxxxxxxxxxxxxxxxxx110xxxxx1110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,N,A2_IMM, A1_ZERO,IMM_Z, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_S,N,N,N,N,N}; // CSRRSI
      32'bxxxxxxxxxxxxxxxxx111xxxxx1110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,N,A2_IMM, A1_ZERO,IMM_Z, DW_XPR,FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_C,N,N,N,N,N}; // CSRRCI

      // ── Rets ──
      32'b00010000001000000000000001110011: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_I,N,N,N,N,N}; // SRET
      32'b01111011001000000000000001110011: raw_inst_ctrl = {Y,N,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_I,N,N,N,N,N}; // DRET

      // ── 64-bit loads/stores ──
      32'bxxxxxxxxxxxxxxxxx011xxxxx0000011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LD
      32'bxxxxxxxxxxxxxxxxx110xxxxx0000011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_WU,   N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // LWU
      32'bxxxxxxxxxxxxxxxxx011xxxxx0100011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_IMM, A1_RS1, IMM_S, DW_XPR,FN_ADD,  Y,M_XWR,  MT_D,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // SD

      // ── 32-bit W-suffix ops ──
      32'bxxxxxxxxxxxxxxxxx000xxxxx0011011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_32, FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // ADDIW
      32'b0000000xxxxxxxxxx001xxxxx0011011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_32, FN_SL,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLLIW
      32'b0000000xxxxxxxxxx101xxxxx0011011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_32, FN_SR,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRLIW
      32'b0100000xxxxxxxxxx101xxxxx0011011: raw_inst_ctrl = {Y,N,N,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_32, FN_SRA,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRAIW
      32'b0000000xxxxxxxxxx000xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_ADD,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // ADDW
      32'b0100000xxxxxxxxxx000xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_SUB,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SUBW
      32'b0000000xxxxxxxxxx001xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_SL,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SLLW
      32'b0000000xxxxxxxxxx101xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_SR,   N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRLW
      32'b0100000xxxxxxxxxx101xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_SRA,  N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // SRAW

      // ── Mul/Div ──
      32'b0000001xxxxxxxxxx000xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_MUL,    N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // MUL
      32'b0000001xxxxxxxxxx001xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_MULH,   N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // MULH
      32'b0000001xxxxxxxxxx011xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_MULHU,  N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // MULHU
      32'b0000001xxxxxxxxxx010xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_MULHSU, N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // MULHSU
      32'b0000001xxxxxxxxxx100xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_DIV,    N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // DIV
      32'b0000001xxxxxxxxxx101xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_DIVU,   N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // DIVU
      32'b0000001xxxxxxxxxx110xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_REM,    N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // REM
      32'b0000001xxxxxxxxxx111xxxxx0110011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_REMU,   N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // REMU

      // ── Mul/Div 64b W-suffix ──
      32'b0000001xxxxxxxxxx000xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_MUL,    N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // MULW
      32'b0000001xxxxxxxxxx100xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_DIV,    N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // DIVW
      32'b0000001xxxxxxxxxx101xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_DIVU,   N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // DIVUW
      32'b0000001xxxxxxxxxx110xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_REM,    N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // REMW
      32'b0000001xxxxxxxxxx111xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_32, FN_REMU,   N,M_X,  MT_X,    N,N,N,N,Y,Y,CSR_N,N,N,N,N,N}; // REMUW

      // ── FP atomics (local PI) ──
      32'b0000011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_ADD,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOADDL_PI
      32'b0000111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_SWAP,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOSWAPL_PI
      32'b0001011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_AND,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOANDL_PI
      32'b0001111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_OR,    MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOORL_PI
      32'b0010011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_XOR,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOXORL_PI
      32'b0010111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOMINL_PI
      32'b0011011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOMAXL_PI
      32'b0011111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MINU,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOMINUL_PI
      32'b0100011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAXU,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOMAXUL_PI
      32'b0010100xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX_PS,MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOMAXL_PS
      32'b0011000xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN_PS,MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FAMOMINL_PS

      // ── FP atomics (global PI) ──
      32'b1000011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_ADD,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOADDG_PI
      32'b1000111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_SWAP,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOSWAPG_PI
      32'b1001011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_AND,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOANDG_PI
      32'b1001111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_OR,    MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOORG_PI
      32'b1010011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_XOR,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOXORG_PI
      32'b1010111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOMING_PI
      32'b1011011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX,   MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOMAXG_PI
      32'b1011111xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MINU,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOMINUG_PI
      32'b1100011xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAXU,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOMAXUG_PI
      32'b1010100xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX_PS,MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOMAXG_PS
      32'b1011000xxxxxxxxxx100xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN_PS,MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FAMOMING_PS

      // ── Scalar AMO W (local) ──
      32'b0000100xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_SWAP,MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOSWAPL_W
      32'b0000000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_ADD, MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOADDL_W
      32'b0010000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_XOR, MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOXORL_W
      32'b0110000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_AND, MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOANDL_W
      32'b0100000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_OR,  MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOORL_W
      32'b1000000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN, MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMINL_W
      32'b1010000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX, MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMAXL_W
      32'b1100000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MINU,MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMINUL_W
      32'b1110000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAXU,MT_W,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMAXUL_W

      // ── Scalar AMO W (global) ──
      32'b0000101xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_SWAP,MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOSWAPG_W
      32'b0000001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_ADD, MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOADDG_W
      32'b0010001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_XOR, MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOXORG_W
      32'b0110001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_AND, MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOANDG_W
      32'b0100001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_OR,  MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOORG_W
      32'b1000001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN, MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMING_W
      32'b1010001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX, MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMAXG_W
      32'b1100001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MINU,MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMINUG_W
      32'b1110001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAXU,MT_W,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMAXUG_W

      // ── Scalar AMO D (local) ──
      32'b0000100xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_SWAP,MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOSWAPL_D
      32'b0000000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_ADD, MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOADDL_D
      32'b0010000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_XOR, MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOXORL_D
      32'b0110000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_AND, MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOANDL_D
      32'b0100000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_OR,  MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOORL_D
      32'b1000000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN, MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMINL_D
      32'b1010000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX, MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMAXL_D
      32'b1100000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MINU,MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMINUL_D
      32'b1110000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAXU,MT_D,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // AMOMAXUL_D

      // ── Scalar AMO D (global) ──
      32'b0000101xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_SWAP,MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOSWAPG_D
      32'b0000001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_ADD, MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOADDG_D
      32'b0010001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_XOR, MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOXORG_D
      32'b0110001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_AND, MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOANDG_D
      32'b0100001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_OR,  MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOORG_D
      32'b1000001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MIN, MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMING_D
      32'b1010001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAX, MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMAXG_D
      32'b1100001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MINU,MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMINUG_D
      32'b1110001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_MAXU,MT_D,    N,N,N,N,N,Y,CSR_N,N,Y,N,N,N}; // AMOMAXUG_D

      // ── AMO compare-swap ──
      32'b1111000xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_CMPSWP,MT_W,  N,N,N,N,N,Y,CSR_N,N,N,N,N,Y}; // AMOCMPSWPL_W
      32'b1111001xxxxxxxxxx010xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_CMPSWP,MT_W,  N,N,N,N,N,Y,CSR_N,N,Y,N,N,Y}; // AMOCMPSWPG_W
      32'b1111000xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_CMPSWP,MT_D,  N,N,N,N,N,Y,CSR_N,N,N,N,N,Y}; // AMOCMPSWPL_D
      32'b1111001xxxxxxxxxx011xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XA_CMPSWP,MT_D,  N,N,N,N,N,Y,CSR_N,N,Y,N,N,Y}; // AMOCMPSWPG_D

      // ── CUDA byte/half stores ──
      32'b0001000xxxxxxxxxx011000000111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_S, DW_XPR,FN_ADD, Y,M_XWRB,  MT_B,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // SBL
      32'b0001001xxxxxxxxxx011000000111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_S, DW_XPR,FN_ADD, Y,M_XWRB,  MT_B,    N,N,N,N,N,N,CSR_N,N,Y,N,N,N}; // SBG
      32'b0001100xxxxxxxxxx011000000111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_S, DW_XPR,FN_ADD, Y,M_XWRB,  MT_H,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // SHL
      32'b0001101xxxxxxxxxx011000000111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_ZERO,A1_RS1, IMM_S, DW_XPR,FN_ADD, Y,M_XWRB,  MT_H,    N,N,N,N,N,N,CSR_N,N,Y,N,N,N}; // SHG

      // ── CUDA PS load/store/gather/scatter bypass (local, funct3=111) ──
      32'b000100000000xxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS,   N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLWL_PS
      32'b010100000000xxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS,   N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSWL_PS
      32'b1000000xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS_GS8, N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FGBL_PS (instr name FGBL_PS)
      32'b1100000xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS_GS8, N,N,N,N,N,N,CSR_N,N,N,Y,N,N}; // FSCBL_PS
      32'b1000100xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS_GS16,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FGHL_PS
      32'b1100100xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS_GS16,N,N,N,N,N,N,CSR_N,N,N,Y,N,N}; // FSCHL_PS
      32'b1001000xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FGWL_PS
      32'b1101000xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS_GS32,N,N,N,N,N,N,CSR_N,N,N,Y,N,N}; // FSCWL_PS

      // ── CUDA PS gather/scatter bypass (global) ──
      32'b000100100000xxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS,   N,N,N,Y,N,N,CSR_N,N,Y,N,N,N}; // FLWG_PS
      32'b010100100000xxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_ZERO,A1_RS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS,   N,N,N,N,N,N,CSR_N,N,Y,N,N,N}; // FSWG_PS
      32'b1000001xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS_GS8, N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FGBG_PS
      32'b1100001xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS_GS8, N,N,N,N,N,N,CSR_N,N,Y,Y,N,N}; // FSCBG_PS
      32'b1000101xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS_GS16,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FGHG_PS
      32'b1100101xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS_GS16,N,N,N,N,N,N,CSR_N,N,Y,Y,N,N}; // FSCHG_PS
      32'b1001001xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XRDB,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,Y,Y,N,N}; // FGWG_PS
      32'b1101001xxxxxxxxxx111xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD, Y,M_XWRB,  MT_PS_GS32,N,N,N,N,N,N,CSR_N,N,Y,Y,N,N}; // FSCWG_PS

      // ── Float single precision ──
      32'b0010000xxxxxxxxxx000xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSGNJ_S
      32'b0010000xxxxxxxxxx010xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSGNJX_S
      32'b0010000xxxxxxxxxx001xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSGNJN_S
      32'b0010100xxxxxxxxxx000xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMIN_S
      32'b0010100xxxxxxxxxx001xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMAX_S
      32'b0000000xxxxxxxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FADD_S
      32'b0000100xxxxxxxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSUB_S
      32'b0001000xxxxxxxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMUL_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1000011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMADD_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1000111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMSUB_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1001111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FNMADD_S
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FNMSUB_S
      32'b111000000000xxxxx001xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FCLASS_S
      32'b111000000000xxxxx000xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FMV_X_S
      32'b110000000000xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FCVT_W_S
      32'b110000000001xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FCVT_WU_S
      32'b1010000xxxxxxxxxx010xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FEQ_S
      32'b1010000xxxxxxxxxx001xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FLT_S
      32'b1010000xxxxxxxxxx000xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FLE_S
      32'b111100000000xxxxx000xxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMV_S_X
      32'b110100000000xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_S_W
      32'b110100000001xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_S_WU

      // ── FP loads/stores ──
      32'bxxxxxxxxxxxxxxxxx010xxxxx0000111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_WU,   N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLW
      32'bxxxxxxxxxxxxxxxxx010xxxxx0100111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_S, DW_XPR,FN_ADD,  Y,M_XWR,  MT_W,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSW

      // NOTE: FP double-precision operations (FSGNJ_D, FADD_D, FMADD_D, FLD, FSD, etc.)
      // are NOT decoded in intpipe_decode — they are handled by vpu_decoder.

      // ── Moscow / FLQ2 / FSQ2 ──
      32'bxxxxxxxxxxxxxxxxx101xxxxx0000111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_PS,   N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLQ2
      32'bxxxxxxxxxxxxxxxxx101xxxxx0100111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_S, DW_XPR,FN_ADD,  Y,M_XWR,  MT_PS,   N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSQ2

      // ── Packed single loads/stores ──
      32'bxxxxxxxxxxxxxxxxx010xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_PS,   N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLW_PS
      32'bxxxxxxxxxxxxxxxxx110xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_S, DW_XPR,FN_ADD,  Y,M_XWR,  MT_PS,   N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSW_PS
      32'bxxxxxxxxxxxxxxxxx000xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_IMM, A1_RS1, IMM_I, DW_XPR,FN_ADD,  Y,M_XRD,  MT_PS_BR,N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FBC_PS
      32'bxxxxxxxxxxxxxxxxx011xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FBCX_PS
      32'bxxxxxxxxxxxxxxxxxxxxxxxxx0011111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FBCI_PS

      // ── PS gather/scatter (non-bypass, local) ──
      32'b0110000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD,  Y,M_XRD,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FGW_PS
      32'b0101000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD,  Y,M_XRD,  MT_PS_GS16,N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FGH_PS
      32'b0100100xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD,  Y,M_XRD,  MT_PS_GS8, N,N,N,Y,N,N,CSR_N,N,N,Y,N,N}; // FGB_PS
      32'b1110000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD,  Y,M_XWR,  MT_PS_GS32,N,N,N,N,N,N,CSR_N,N,N,Y,N,N}; // FSCW_PS
      32'b1101000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD,  Y,M_XWR,  MT_PS_GS16,N,N,N,N,N,N,CSR_N,N,N,Y,N,N}; // FSCH_PS
      32'b1100100xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,N,A2_RS2, A1_FS1, IMM_X, DW_XPR,FN_ADD,  Y,M_XWR,  MT_PS_GS8, N,N,N,N,N,N,CSR_N,N,N,Y,N,N}; // FSCB_PS

      // ── PS G32 gather/scatter ──
      32'b0010000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,Y,A2_RS2, A1_ZERO,IMM_X, DW_XPR,FN_ADD,  Y,M_G32,  MT_PS_GS32,N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FG32W_PS
      32'b0001000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,Y,A2_RS2, A1_ZERO,IMM_X, DW_XPR,FN_ADD,  Y,M_G32,  MT_PS_GS16,N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FG32H_PS
      32'b0000100xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,Y,A2_RS2, A1_ZERO,IMM_X, DW_XPR,FN_ADD,  Y,M_G32,  MT_PS_GS8, N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FG32B_PS
      32'b1010000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,Y,A2_RS2, A1_ZERO,IMM_X, DW_XPR,FN_ADD,  Y,M_SC32, MT_PS_GS32,N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSC32W_PS
      32'b1001000xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,Y,A2_RS2, A1_ZERO,IMM_X, DW_XPR,FN_ADD,  Y,M_SC32, MT_PS_GS16,N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSC32H_PS
      32'b1000100xxxxxxxxxx001xxxxx0001011: raw_inst_ctrl = {Y,N,Y,N,N,N,Y,Y,A2_RS2, A1_ZERO,IMM_X, DW_XPR,FN_ADD,  Y,M_SC32, MT_PS_GS8, N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSC32B_PS

      // ── PS arithmetic ──
      32'b0000000xxxxxxxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FADD_PS
      32'b0000100xxxxxxxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSUB_PS
      32'b0001000xxxxxxxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMUL_PS
      32'b0010100xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMIN_PS
      32'b0010100xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMAX_PS
      32'b010110001000xxxxx000xxxxx1111011: raw_inst_ctrl = EnableExtraTrans
                                                          ? {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N} // FRSQ_PS
                                                          : {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FRSQ_PS (mcode)
      32'b010110000111xxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FRCP_PS
      32'bxxxxx00xxxxxxxxxxxxxxxxxx1011011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMADD_PS
      32'bxxxxx01xxxxxxxxxxxxxxxxxx1011011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMSUB_PS
      32'bxxxxx10xxxxxxxxxxxxxxxxxx1011011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FNMSUB_PS
      32'bxxxxx11xxxxxxxxxxxxxxxxxx1011011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FNMADD_PS
      32'b1010000xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FEQ_PS
      32'b1010000xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLT_PS
      32'b1010000xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLE_PS
      32'b1010000xxxxxxxxxx110xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // FEQM_PS
      32'b1010000xxxxxxxxxx101xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // FLTM_PS
      32'b1010000xxxxxxxxxx100xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // FLEM_PS
      32'b111000000000xxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCLASS_PS
      32'b110000000000xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_PW_PS
      32'b110000000001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_PWU_PS
      32'b110100000000xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_PS_PW
      32'b110100000001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_PS_PWU
      32'b111000000xxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FMVZ_X_PS
      32'b111000000xxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // FMVS_X_PS
      32'b0010000xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSGNJ_PS
      32'b0010000xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSGNJX_PS
      32'b0010000xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSGNJN_PS
      32'bxxxxx10xxxxxxxxxx010xxxxx0111111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCMOV_PS
      32'b0000000xxxxxxxxxx000xxxxx1110111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCMOVM_PS
      32'b1110011xxxxxxxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSWIZZ_PS
      32'b010110000110xxxxx000xxxxx1111011: raw_inst_ctrl = EnableExtraTrans
                                                          ? {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N} // FSIN_PS
                                                          : {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSIN_PS (mcode)
      32'b010110000100xxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FEXP_PS
      32'b010110000011xxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLOG_PS
      32'b0011000xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FRCP_FIX_RAST
      32'b010110000010xxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FFRC_PS
      32'b010110000001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FROUND_PS

      // ── PS conversions (gfx=1) ──
      32'b110100001010xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_PS_F16
      32'b110100001001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_F11
      32'b110100001000xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_F10
      32'b110100010000xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_UN24
      32'b110100010001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_UN16
      32'b110100010010xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_UN10
      32'b110100010011xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_UN8
      32'b110100010111xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_UN2
      32'b110100011001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_SN16
      32'b110100011011xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_SN8
      32'b110110001001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FCVT_F16_PS
      32'b110110001000xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_F11_PS
      32'b110110001011xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_F10_PS
      32'b110110010000xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_UN24_PS
      32'b110110010001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_UN16_PS
      32'b110110010010xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_UN10_PS
      32'b110110010011xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_UN8_PS
      32'b110110010111xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_UN2_PS
      32'b110110011001xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_SN16_PS
      32'b110110011011xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_SN8_PS
      32'b110100000010xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_PS_RAST
      32'b110000000010xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // FCVT_RAST_PS

      // ── Mask operations ──
      32'b0110011xxxxxxxxxx111xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // MAND
      32'b0110011xxxxxxxxxx110xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // MOR
      32'b011001100000xxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // MNOT
      32'b0110011xxxxxxxxxx100xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // MXOR
      32'b11010110000000000000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    Y,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // MOVA_X_M
      32'b110101100000xxxxx001000001111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,Y,N,N,N,CSR_N,N,N,N,N,N}; // MOVA_M_X
      32'b0101011xxxxxxxxxxxxx00xxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,Y,A2_X,   A1_RS1, IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // MOV_M_X
      32'b010100100000xxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // MASKPOPC
      32'b010101000000xxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // MASKPOPCZ
      32'b0101111xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,Y,N}; // MASKPOPCR

      // ── Packed Integer ──
      32'bxxxxxxxxxxxxxxxxxxxxxxxxx1011111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FBCI_PI
      32'b0000011xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FADD_PI
      32'b0000111xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSUB_PI
      32'b0001011xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMUL_PI
      32'b0001011xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMULH_PI
      32'b0001011xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMULHU_PI
      32'b0010111xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMIN_PI
      32'b0010111xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMAX_PI
      32'b0010111xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMINU_PI
      32'b0010111xxxxxxxxxx011xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FMAXU_PI
      32'b0000011xxxxxxxxxx111xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FAND_PI
      32'b0011111xxxxxxxxxx00000xxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // FLTM_PI
      32'b000001100000xxxxx011xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSAT8_PI
      32'b000001100001xxxxx011xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSATU8_PI
      32'b0000011xxxxxxxxxx110xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FOR_PI
      32'b0000011xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FNOT_PI
      32'b0000011xxxxxxxxxx100xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FXOR_PI
      32'b0000011xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSLL_PI
      32'b0100111xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSLLI_PI
      32'b0000011xxxxxxxxxx101xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSRL_PI
      32'b0000111xxxxxxxxxx101xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSRA_PI
      32'b0100111xxxxxxxxxx101xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSRLI_PI
      32'b0100111xxxxxxxxxx111xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FSRAI_PI
      32'b1010011xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FEQ_PI
      32'b1010011xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLT_PI
      32'b1010011xxxxxxxxxx011xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLTU_PI
      32'b1010011xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FLE_PI
      32'b101001100000xxxxx10000xxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,Y,N,N,N,N,CSR_N,N,N,N,N,N}; // FSETM_PI
      32'bxxxxx10xxxxxxxxxx000xxxxx0111111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FADDI_PI
      32'bxxxxx10xxxxxxxxxx001xxxxx0111111: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FANDI_PI
      32'b001001100000xxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FPACKREPB_PI
      32'b001001100000xxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,N,N}; // FPACKREPH_PI

      // ── Cube / GFX ──
      32'b1000100xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // CUBEF_PS
      32'b1000100xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // CUBEFIDX_PS
      32'b1000100xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // CUBESGNSC_PS
      32'b1000100xxxxxxxxxx011xxxxx1111011: raw_inst_ctrl = {Y,N,Y,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,Y,N,N,CSR_N,N,N,N,Y,N}; // CUBESGNTC_PS

      // ── PACKB / BITMIXB ──
      32'b1000000xxxxxxxxxx110xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_PACKB,  N,M_X,  MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,N,N}; // PACKB
      32'b1000000xxxxxxxxxx111xxxxx0111011: raw_inst_ctrl = {Y,N,N,N,N,N,Y,Y,A2_RS2, A1_RS1, IMM_X, DW_XPR,FN_BITMIXB,N,M_X,  MT_X,    N,N,N,N,N,Y,CSR_N,N,N,N,Y,N}; // BITMIXB

      // ── M-CODE instructions (mcode=1) ──
      32'b110000000010xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FCVT_L_S
      32'b110000000011xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FCVT_LU_S
      32'b110100000010xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FCVT_S_L
      32'b110100000011xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FCVT_S_LU
      32'b0001111xxxxxxxxxx000xxxxx1111011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FDIV_PI
      32'b0001111xxxxxxxxxx001xxxxx1111011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FDIVU_PI
      32'b0001111xxxxxxxxxx011xxxxx1111011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FREMU_PI
      32'b0001111xxxxxxxxxx010xxxxx1111011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FREM_PI
      32'b0001100xxxxxxxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FDIV_S
      32'b010110000000xxxxxxxxxxxxx1010011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSQRT_S
      32'b010110000000xxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FSQRT_PS
      32'b0001100xxxxxxxxxxxxxxxxxx1111011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FDIV_PS
      32'bxxxxxxxxxxxxxxxxx001xxxxx0001111: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // FENCE_I
      32'b0001001xxxxxxxxxx000000001110011: raw_inst_ctrl = {Y,Y,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N}; // SFENCE_VMA

      // ── Default: illegal instruction ──
      default:                              raw_inst_ctrl = {N,N,N,N,N,N,N,N,A2_X,   A1_X,   IMM_X, DW_X,  FN_X,    N,M_X,    MT_X,    N,N,N,N,N,N,CSR_N,N,N,N,N,N};
    endcase

    inst_ctrl = minion_control_t'(raw_inst_ctrl);
  end

endmodule

/* verilator lint_on CASEX */
/* verilator lint_on CASEOVERLAP */
/* verilator lint_on UNUSEDPARAM */
