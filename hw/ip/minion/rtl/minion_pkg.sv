// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion shared package.
//
// Types, constants, and enums shared across the minion core —
// used by both the instruction frontend and the integer pipeline.

/* verilator lint_off UNUSED */  // Shared package exports many constants/types that are intentionally consumed by other minion sub-IPs, not every symbol in every build.
package minion_pkg;

  // ════════════════════════════════════════════════════════
  // Architectural constants
  // ════════════════════════════════════════════════════════

  parameter int unsigned NrThreadsDefault = 2;
  parameter int unsigned NrThreads       = 2;

  parameter int unsigned CsrRegAddrSize  = 12;  // CSR address width (12-bit RISC-V)
  parameter int unsigned NrMsgPorts      = 4;
  parameter int unsigned FccPerMin       = 2;   // FCC ports per minion

  // ── DCache constants ──────────────────────────────────────────────
  parameter int unsigned DcacheSets          = 16;
  parameter int unsigned DcacheWays          = 4;
  parameter int unsigned DcacheLineSize      = 64;  // bytes
  parameter int unsigned DcacheLineAddrLsb   = 6;   // $clog2(64/8*8) = 6
  parameter int unsigned DcacheWayBits       = 2;    // $clog2(4)
  parameter int unsigned DcacheSetBits       = 4;    // $clog2(16)
  parameter int unsigned DcacheLramAddrBits  = 10;   // set_bits + line_addr_lsb
  parameter int unsigned DcacheTensorCtrlNlinesBits = 4;
  parameter int unsigned DcacheL2ScpDstBits  = 17;
  parameter int unsigned DcacheCoL2MaxOpsBits  = 5;
  parameter int unsigned DcacheCoL2ReprateBits = 3;
  parameter int unsigned TensorMaskBits      = 16;

  // ── Message port constants ────────────────────────────────────────
  parameter int unsigned CoreMsgPortMaxMsgsSz = 4;

  // ── ET link constants ─────────────────────────────────────────────
  parameter int unsigned EtLinkIdSize        = 8;
  parameter int unsigned EtMsgPortIdStart    = 3;
  parameter int unsigned EtMsgPortIdSize     = 2;
  parameter int unsigned EtMsgValidBit       = 2;
  parameter int unsigned CoreL2BlockExtSize  = 256;

  parameter int unsigned VaSize    = 48;
  parameter int unsigned VaSizeExt = 49;  // +1 for sign extension
  parameter int unsigned PaSize    = 40;
  parameter int unsigned PcSize    = 48;
  parameter int unsigned PcSizeExt = 49;
  parameter int unsigned InstSize  = 32;
  parameter int unsigned VaUntransSize = 12;
  parameter int unsigned PaUntransSize = 12;
  parameter int unsigned VaTransSize = VaSize - VaUntransSize;
  parameter int unsigned Va39Msb = 38;
  parameter int unsigned PtwPgIdxSz = 9;

  parameter int unsigned XregSize     = 64;
  parameter int unsigned XregAddrSize = 5;
  parameter int unsigned FregAddrSize = 5;
  parameter int unsigned MregAddrSize = 3;

  // ════════════════════════════════════════════════════════
  // Privilege levels
  // ════════════════════════════════════════════════════════

  parameter logic [1:0] PrvU = 2'd0;
  parameter logic [1:0] PrvS = 2'd1;
  parameter logic [1:0] PrvM = 2'd3;

  // ════════════════════════════════════════════════════════
  // Debug breakpoint types
  // ════════════════════════════════════════════════════════

  typedef enum logic [2:0] {
    DbgBpLoad,
    DbgBpStore,
    DbgBpTensorLoad,
    DbgBpTensorStore,
    DbgBpAmo,
    DbgBpCo
  } debug_bp_e;

  typedef struct packed {
    logic dmode;
    logic action;   // 0=raise exception, 1=enter debug mode
    logic match;
    logic m;
    logic s;
    logic u;
    logic execute;
    logic store;
    logic load;
  } minion_bp_ctrl_t;

  // ════════════════════════════════════════════════════════
  // VM status (passed to ICache for TLB lookup)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [1:0] prv;       // privilege level
    logic       mprv;      // modify privilege
    logic [1:0] mpp;       // machine previous privilege
    logic       sum;       // supervisor user memory access
    logic       mxr;       // make executable readable
    logic       debug;     // debug mode
  } vm_status_t;

  // ════════════════════════════════════════════════════════
  // ALU function codes
  // ════════════════════════════════════════════════════════

  typedef enum logic [4:0] {
    AluAdd     = 5'b00000,
    AluSl      = 5'b00001,
    AluSeq     = 5'b00010,
    AluSne     = 5'b00011,
    AluXor     = 5'b00100,
    AluSr      = 5'b00101,
    AluOr      = 5'b00110,
    AluAnd     = 5'b00111,
    AluPackb   = 5'b01000,
    AluBitmixb = 5'b01001,
    AluSub     = 5'b01010,
    AluSra     = 5'b01011,
    AluSlt     = 5'b01100,
    AluSge     = 5'b01101,
    AluSltu    = 5'b01110,
    AluSgeu    = 5'b01111,
    AluMul     = 5'b10000,
    AluMulh    = 5'b10001,
    AluDiv     = 5'b10100,
    AluDivu    = 5'b10101,
    AluRem     = 5'b10110,
    AluRemu    = 5'b10111,
    AluMulhsu  = 5'b11100,
    AluMulhu   = 5'b11110
  } alu_func_e;

  // ════════════════════════════════════════════════════════
  // Decode control enums
  // ════════════════════════════════════════════════════════

  typedef enum logic [1:0] {
    A1Zero = 2'b00,
    A1Rs1  = 2'b01,
    A1Pc   = 2'b10,
    A1Fs1  = 2'b11
  } a1_ctrl_e;

  typedef enum logic [1:0] {
    A2Zero = 2'b00,
    A2Size = 2'b01,
    A2Rs2  = 2'b10,
    A2Imm  = 2'b11
  } a2_ctrl_e;

  typedef enum logic [2:0] {
    ImmS  = 3'b000,
    ImmSb = 3'b001,
    ImmU  = 3'b010,
    ImmUj = 3'b011,
    ImmI  = 3'b100,
    ImmZ  = 3'b101
  } imm_ctrl_e;

  typedef enum logic {
    Dw32 = 1'b0,
    Dw64 = 1'b1
  } dw_ctrl_e;

  typedef enum logic [2:0] {
    CsrN = 3'b000,
    CsrW = 3'b001,
    CsrS = 3'b010,
    CsrC = 3'b011,
    CsrI = 3'b100,
    CsrR = 3'b101
  } csr_ctrl_e;

  // ════════════════════════════════════════════════════════
  // DCache command (mem_cmd field in decode output)
  // ════════════════════════════════════════════════════════

  typedef enum logic [4:0] {
    DcCmdXrd      = 5'b00000,
    DcCmdXwr      = 5'b00001,
    DcCmdPfr      = 5'b00010,
    DcCmdPfw      = 5'b00011,
    DcCmdXaSwap   = 5'b00100,
    DcCmdFlushAll = 5'b00101,
    DcCmdXlr      = 5'b00110,
    DcCmdXsc      = 5'b00111,
    DcCmdXaAdd    = 5'b01000,
    DcCmdXaXor    = 5'b01001,
    DcCmdXaOr     = 5'b01010,
    DcCmdXaAnd    = 5'b01011,
    DcCmdXaMin    = 5'b01100,
    DcCmdXaMax    = 5'b01101,
    DcCmdXaMinu   = 5'b01110,
    DcCmdXaMaxu   = 5'b01111,
    DcCmdFlush    = 5'b10000,
    DcCmdProduce  = 5'b10001,
    DcCmdClean    = 5'b10011,
    DcCmdXaCmpswp = 5'b11000,
    DcCmdG32      = 5'b11001,
    DcCmdSc32     = 5'b11010,
    DcCmdXwrb     = 5'b11011,
    DcCmdXrdb     = 5'b11100,
    DcCmdXaMinPs  = 5'b11101,
    DcCmdXaMaxPs  = 5'b11111
  } dcache_cmd_e;

  // ════════════════════════════════════════════════════════
  // DCache type (mem_type field in decode output)
  // ════════════════════════════════════════════════════════

  typedef enum logic [3:0] {
    DcTypeB      = 4'b0000,
    DcTypeH      = 4'b0001,
    DcTypeW      = 4'b0010,
    DcTypeD      = 4'b0011,
    DcTypeBu     = 4'b0100,
    DcTypeHu     = 4'b0101,
    DcTypeWu     = 4'b0110,
    DcTypePs     = 4'b1011,
    DcTypePsBr   = 4'b1010,
    DcTypePsGs8  = 4'b1100,
    DcTypePsGs16 = 4'b1101,
    DcTypePsGs32 = 4'b1110
  } dcache_type_e;

  // ════════════════════════════════════════════════════════
  // Integer pipeline control (frontend decode output)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic          legal;
    logic          mcode;
    logic          fp;
    logic          br;
    logic          jal;
    logic          jalr;
    logic          rxs2;
    logic          rxs1;
    a2_ctrl_e      sel_alu2;
    a1_ctrl_e      sel_alu1;
    imm_ctrl_e     sel_imm;
    dw_ctrl_e      alu_dw;
    alu_func_e     alu_fn;
    logic          mem;
    dcache_cmd_e   mem_cmd;
    dcache_type_e  mem_type;
    logic          rma;
    logic          wmd;
    logic          wmad;
    logic          wfd;
    logic          div;
    logic          wxd;
    csr_ctrl_e     csr;
    logic          fence;
    logic          mem_g;
    logic          gsc;
    logic          gfx;
    logic          x31;
  } minion_control_t;

  // ════════════════════════════════════════════════════════
  // VPU command codes (cmd field in vpu_ctrl_sigs_t)
  // ════════════════════════════════════════════════════════

  typedef enum logic [6:0] {
    VpuCmdAdd          = 7'b0000000,
    VpuCmdSub          = 7'b0000001,
    VpuCmdMul          = 7'b0000010,
    VpuCmdMull         = 7'b0000011,
    VpuCmdMullu        = 7'b0000100,
    VpuCmdMulhu        = 7'b0000101,
    VpuCmdMulh         = 7'b0000110,
    VpuCmdMadd         = 7'b0000111,
    VpuCmdMsub         = 7'b0001000,
    VpuCmdNmsub        = 7'b0001001,
    VpuCmdNmadd        = 7'b0001010,
    VpuCmdDiv          = 7'b0001011,
    VpuCmdSqrt         = 7'b0001100,
    VpuCmdNr2Frcpfxp   = 7'b0001101,
    VpuCmdFsgnj        = 7'b0001110,
    VpuCmdMin          = 7'b0001111,
    VpuCmdMax          = 7'b0010000,
    VpuCmdCvtFf        = 7'b0010001,
    VpuCmdCvtF32int    = 7'b0010010,
    VpuCmdCmp          = 7'b0010011,
    VpuCmdMvXf         = 7'b0010100,
    VpuCmdCvtIntf32    = 7'b0010101,
    VpuCmdMvFx         = 7'b0010110,
    VpuCmdX            = 7'b0010111,
    VpuCmdNot          = 7'b0011000,
    VpuCmdAnd          = 7'b0011001,
    VpuCmdOr           = 7'b0011010,
    VpuCmdXor          = 7'b0011011,
    VpuCmdSll          = 7'b0011100,
    VpuCmdSrl          = 7'b0011101,
    VpuCmdSra          = 7'b0011110,
    VpuCmdFeq          = 7'b0011111,
    VpuCmdFle          = 7'b0100000,
    VpuCmdFlt          = 7'b0100001,
    VpuCmdFltu         = 7'b0100010,
    VpuCmdMand         = 7'b0100011,
    VpuCmdMor          = 7'b0100100,
    VpuCmdMxor         = 7'b0100101,
    VpuCmdMnot         = 7'b0100110,
    VpuCmdFsetm        = 7'b0100111,
    VpuCmdMovaXM       = 7'b0101000,
    VpuCmdMovaMX       = 7'b0101001,
    VpuCmdMovMX        = 7'b0101010,
    VpuCmdMpopc        = 7'b0101011,
    VpuCmdMpopcz       = 7'b0101100,
    VpuCmdFcmov        = 7'b0101101,
    VpuCmdFcmovm       = 7'b0101110,
    VpuCmdFswizz       = 7'b0101111,
    VpuCmdMvzFx        = 7'b0110000,
    VpuCmdMvsFx        = 7'b0110001,
    VpuCmdCvtF32f16    = 7'b0110010,
    VpuCmdCvtF32f11    = 7'b0110011,
    VpuCmdCvtF32f10    = 7'b0110100,
    VpuCmdCvtF32un24   = 7'b0110101,
    VpuCmdCvtF32un16   = 7'b0110110,
    VpuCmdCvtF32un8    = 7'b0110111,
    VpuCmdCvtF32sn24   = 7'b0111000,
    VpuCmdCvtF32sn16   = 7'b0111001,
    VpuCmdCvtF32sn8    = 7'b0111010,
    VpuCmdCvtF16f32    = 7'b0111011,
    VpuCmdCvtF11f32    = 7'b0111100,
    VpuCmdCvtF10f32    = 7'b0111101,
    VpuCmdCvtUn24f32   = 7'b0111110,
    VpuCmdCvtUn16f32   = 7'b0111111,
    VpuCmdCvtUn10f32   = 7'b1000000,
    VpuCmdCvtUn8f32    = 7'b1000001,
    VpuCmdCvtUn2f32    = 7'b1000010,
    VpuCmdCvtSn24f32   = 7'b1000011,
    VpuCmdCvtSn16f32   = 7'b1000100,
    VpuCmdCvtSn8f32    = 7'b1000101,
    VpuCmdCvtF32rast   = 7'b1000110,
    VpuCmdCvtRastf32   = 7'b1001000,
    VpuCmdNr1Frcpfxp   = 7'b1001001,
    VpuCmdDivu         = 7'b1001010,
    VpuCmdFremu        = 7'b1001011,
    VpuCmdFfrc         = 7'b1001100,
    VpuCmdFround       = 7'b1001101,
    VpuCmdAddi         = 7'b1001110,
    VpuCmdAndi         = 7'b1001111,
    VpuCmdSlli         = 7'b1010000,
    VpuCmdFfrcSin      = 7'b1010001,
    VpuCmdFbci         = 7'b1010010,
    VpuCmdSrli         = 7'b1010011,
    VpuCmdSrai         = 7'b1010100,
    VpuCmdFbc          = 7'b1010101,
    VpuCmdClass        = 7'b1010110,
    VpuCmdCvtF32uint   = 7'b1010111,
    VpuCmdCvtUintf32   = 7'b1011000,
    VpuCmdFsgnjn       = 7'b1011001,
    VpuCmdFsgnjx       = 7'b1011010,
    VpuCmdCvtF32un2    = 7'b1011011,
    VpuCmdCvtF32sn2    = 7'b1011100,
    VpuCmdCvtF32un10   = 7'b1011101,
    VpuCmdCvtF32sn10   = 7'b1011110,
    VpuCmdMinu         = 7'b1011111,
    VpuCmdMaxu         = 7'b1100000,
    VpuCmdFsatu8       = 7'b1100001,
    VpuTransRcpRr      = 7'b1100010,
    VpuTransRcpFma1    = 7'b1100011,
    VpuTransRcpFma2    = 7'b1100100,
    VpuTransRsqrtRr    = 7'b1100101,
    VpuTransRsqrtFma1  = 7'b1100110,
    VpuTransRsqrtFma2  = 7'b1100111,
    VpuCmdMpocRast     = 7'b1101000,
    VpuCmdCubeface     = 7'b1101001,
    VpuCmdCubefaceIdx  = 7'b1101010,
    VpuCmdCubesgnsc    = 7'b1101011,
    VpuCmdCubesgntc    = 7'b1101100,
    VpuCmdPackrepb     = 7'b1101101,
    VpuCmdPackreph     = 7'b1101110,
    VpuTransLogRr      = 7'b1110001,
    VpuTransLogFma1    = 7'b1110010,
    VpuTransLogFma2    = 7'b1110011,
    VpuTransLogMul     = 7'b1110100,
    VpuTransSinTrans   = 7'b1110101,
    VpuTransSinRr      = 7'b1110110,
    VpuTransSinP1      = 7'b1110111,
    VpuTransSinP2      = 7'b1111000,
    VpuTransSinP3      = 7'b1111001,
    VpuTransExpFrac    = 7'b1111010,
    VpuTransExpRr      = 7'b1111011,
    VpuTransExpFma1    = 7'b1111100,
    VpuTransExpFma2    = 7'b1111101,
    VpuCmdFsat8        = 7'b1111110
  } vpu_cmd_e;

  // ════════════════════════════════════════════════════════
  // VPU data type codes (dtype field in vpu_ctrl_sigs_t)
  // ════════════════════════════════════════════════════════

  typedef enum logic [3:0] {
    VpuDtypeInt   = 4'd0,
    VpuDtypeF32   = 4'd1,
    VpuDtypeF16   = 4'd2,
    VpuDtypeF11   = 4'd3,
    VpuDtypeF10   = 4'd4,
    VpuDtypeUn24  = 4'd5,
    VpuDtypeUn16  = 4'd6,
    VpuDtypeSn16  = 4'd7,
    VpuDtypeUn8   = 4'd8,
    VpuDtypeSn8   = 4'd9,
    VpuDtypeUn10  = 4'd10,
    VpuDtypeUn2   = 4'd11,
    VpuDtypeSn10  = 4'd12,
    VpuDtypeSn2   = 4'd13,
    VpuDtypeF16f32 = 4'd14
  } vpu_dtype_e;

  // ════════════════════════════════════════════════════════
  // VPU control signals (vpu_decoder output)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    vpu_cmd_e      cmd;
    logic          txfma;
    logic          shsw;
    logic          rom;
    logic          scatter;
    logic          cvt;
    logic          trans;
    logic          gcvt;
    logic          fromintf;
    logic          tointm;
    logic          tointf;
    logic          maskop;
    logic          ldst;
    logic          m0ren;
    logic          mallren;
    logic          mren1;
    logic          mren2;
    logic          wen;
    logic          ren1;
    logic          ren2;
    logic          ren3;
    logic          rend;
    logic          swap12;
    logic          swap23;
    logic          swap13;
    /* verilator lint_off SYMRSVDWORD */  // Matches the original VPU control field naming.
    logic          vector;
    /* verilator lint_on SYMRSVDWORD */
    vpu_dtype_e    dtype;
    logic          fromint;
    logic          toint;
    logic          fma;
    logic          fms;
    logic          add;
    logic          sub;
    logic          mul;
    logic          round;
    logic          wflags;
  } vpu_ctrl_sigs_t;

  // ════════════════════════════════════════════════════════
  // VPU → Core ID-stage control (vpu_minion_id_ctrl)
  // ════════════════════════════════════════════════════════
  //
  // This is the struct output by the frontend to the intpipe for
  // VPU dependency checking. Most fields are filled by the VPU at
  // runtime; the frontend only sets the decode-derived fields.

  // Sub-types needed for the scoreboard
  typedef struct packed {
    logic                    fp;
    logic [XregAddrSize-1:0] addr;
    logic                    thread_id;
  } minion_reg_dest_t;

  typedef struct packed {
    logic [MregAddrSize-1:0] addr;
    logic                    thread_id;
  } minion_mreg_dest_t;

  // VPU mask register count
  parameter int unsigned VpuRegmaskNum = 8;

  // DCache scoreboard (replayQ entries visible to intpipe for hazard checking)
  parameter int unsigned DcacheReplayqSize = 8;

  typedef struct packed {
    logic              [DcacheReplayqSize-1:0] valid;
    minion_reg_dest_t  [DcacheReplayqSize-1:0] dest;
  } dcache_scoreboard_t;

  parameter int unsigned VpuTransScoreboardSize = 12;
  parameter int unsigned VpuFpScoreboardSize    = VpuTransScoreboardSize + 2; // 14
  parameter int unsigned VpuTointScoreboardSize = 4;
  parameter int unsigned VpuMaskScoreboardSize  = 4;

  typedef struct packed {
    logic              [VpuFpScoreboardSize-1:0]    fp_valid;
    minion_reg_dest_t  [VpuFpScoreboardSize-1:0]    fp_dest;
    logic              [VpuTointScoreboardSize-1:0]  toint_valid;
    minion_reg_dest_t  [VpuTointScoreboardSize-1:0]  toint_dest;
    logic              [VpuMaskScoreboardSize-1:0]   mask_valid;
    minion_mreg_dest_t [VpuMaskScoreboardSize-1:0]   mask_dest;
  } vpu_scoreboard_t;

  typedef struct packed {
    logic              m0ren;
    logic              mallren;
    logic              mren1;
    logic              mren2;
    logic              wen;
    logic              ren1;
    logic              ren2;
    logic              ren3;
    logic              rend;
    logic              is_trans;
    logic              trans_busy;
    logic              id_trans_insert;
    vpu_scoreboard_t   scoreboard;
    logic              tfma_enabled;
    logic              tfma_wrrf_enabled;
    logic              tquant_enabled;
    logic              reduce_enabled;
    logic              fflags_stall;
    logic              fromint;
  } vpu_id_ctrl_t;

  // ════════════════════════════════════════════════════════
  // Exception causes
  // ════════════════════════════════════════════════════════

  typedef enum logic [4:0] {
    XcptMisalignedFetch     = 5'h0,
    XcptFetchAccess         = 5'h1,
    XcptIllegalInstruction  = 5'h2,
    XcptBreakpoint          = 5'h3,
    XcptMisalignedLoad      = 5'h4,
    XcptLoadAccess          = 5'h5,
    XcptMisalignedStore     = 5'h6,
    XcptStoreAccess         = 5'h7,
    XcptUserEcall           = 5'h8,
    XcptSupervisorEcall     = 5'h9,
    XcptMachineEcall        = 5'hb,
    XcptFetchPageFault      = 5'hc,
    XcptLoadPageFault       = 5'hd,
    XcptDebug               = 5'he,
    XcptStorePageFault      = 5'hf,
    XcptFetchBusError       = 5'h19,
    XcptFetchEccError       = 5'h1a
  } xcpt_cause_e;

  // ── CSR command type ────────────────────────────────────────────────
  typedef enum logic [2:0] {
    CsrCmdN = 3'd0,
    CsrCmdW = 3'd1,
    CsrCmdS = 3'd2,
    CsrCmdC = 3'd3,
    CsrCmdI = 3'd4,
    CsrCmdR = 3'd5
  } csr_cmd_e;

  // ── Message port types ────────────────────────────────────────────
  typedef enum logic [2:0] {
    MsgPort32  = 3'd2,
    MsgPort64  = 3'd3,
    MsgPort128 = 3'd4,
    MsgPort256 = 3'd5
  } core_msg_port_logsize_e;

  typedef struct packed {
    logic                              umode;
    core_msg_port_logsize_e            logsize;
    logic [CoreMsgPortMaxMsgsSz-1:0]   max_msgs;
    logic [DcacheSetBits-1:0]          scp_set;
    logic [DcacheWayBits-1:0]          scp_way;
    logic                              enable_oob;
  } core_msg_port_conf_t;

  // ── ET link types ─────────────────────────────────────────────────
  typedef enum logic [1:0] {
    EtLinkRspAck         = 2'd0,
    EtLinkRspAckData     = 2'd1,
    EtLinkRspMsgRcvData  = 2'd2,
    EtLinkRspErr         = 2'd3
  } et_link_rsp_opcode_e;

  typedef enum logic [2:0] {
    EtLinkSz1   = 3'd0,
    EtLinkSz2   = 3'd1,
    EtLinkSz4   = 3'd2,
    EtLinkSz8   = 3'd3,
    EtLinkSz16  = 3'd4,
    EtLinkSz32  = 3'd5,
    EtLinkSz64  = 3'd6
  } et_link_size_e;

  typedef enum logic [7:0] {
    EtLinkMinMsgIdRegularPort   = 8'b00000000,
    EtLinkMinMsgIdTboxReplyPort = 8'b10000000,
    EtLinkMinMsgIdReduceReady   = 8'b11000000,
    EtLinkMinMsgIdReduceData    = 8'b11000100,
    EtLinkMinMsgIdTboxPull      = 8'b11001000
  } et_link_min_msg_id_e;

  typedef struct packed {
    logic [EtLinkIdSize-1:0]          id;
    logic                             dest;
    logic                             wdata;
    et_link_rsp_opcode_e              opcode;
    logic [CoreL2BlockExtSize-1:0]    data;
    et_link_size_e                    size;
    logic [3:0]                       qwen;
  } et_link_minion_rsp_info_t;

  typedef struct packed {
    logic [EtLinkIdSize-1:0]          id;
    logic                             dest;
    logic                             wdata;
    et_link_rsp_opcode_e              opcode;
    et_link_size_e                    size;
    logic [3:0]                       qwen;
  } et_link_minion_nodata_rsp_info_t;

  typedef struct packed {
    logic [etlink_pkg::SourceSize-2:0] min_id;
    logic                               thread_id;
  } et_link_neigh_msg_dest_t;

  // ── DCache interface types ────────────────────────────────────────
  // GSC progress
  localparam int CoreGscCntBits = 3;

  // dcache → core control response
  typedef struct packed {
    logic                             cache_op_ready;
    logic [NrThreads-1:0]             cache_op_busy_changed;
    logic [NrThreads-1:0][1:0]        cache_op_complete_l1;
    logic [NrThreads-1:0][1:0]        cache_op_complete_l2;
    logic                             tl_ready_0;
    logic                             tl_ready_1;
    logic                             ts_ready;
    logic [1:0]                       tl_complete_0;
    logic                             reduce_ready;
    logic                             tex_send_ready;
    logic [NrThreads-1:0]             gsc_progress_save;
    logic [CoreGscCntBits-1:0]        gsc_progress;
  } dcache_core_ctrl_t;

  // ── Tensor convolution types ────────────────────────────────────────
  typedef struct packed {
    logic [7:0]  row_step_offset;
    logic [7:0]  padding2;
    logic [15:0] row_size;
    logic [7:0]  col_step_offset;
    logic [7:0]  padding1;
    logic [15:0] col_size;
  } tensorconv_size;

  typedef struct packed {
    logic [15:0] row_pos;
    logic [15:0] padding1;
    logic [15:0] col_pos;
  } tensorconv_control;

  // ── CSR fast local barrier ──────────────────────────────────────────
  localparam int CsrFlBarrierSize  = 13;

  // ── PMU counters ──────────────────────────────────────────────────
  localparam int PmuCountersSelectBits = 4;   // $clog2(12)
  localparam int PmuCountersReadDelay  = 3;

  // ── CSR replay ────────────────────────────────────────────────────
  typedef enum logic [4:0] {
    CSR_REPLAY_CACHE_OP       = 0,
    CSR_REPLAY_FLB            = 1,
    CSR_REPLAY_TL0            = 2,
    CSR_REPLAY_TL1            = 3,
    CSR_REPLAY_TFMA           = 4,
    CSR_REPLAY_TQUANT         = 5,
    CSR_REPLAY_TREDUCE        = 6,
    CSR_REPLAY_TS             = 7,
    CSR_REPLAY_CONV           = 8,
    CSR_REPLAY_PGET           = 9,
    CSR_REPLAY_FCC0           = 10,
    CSR_REPLAY_FCC1           = 11,
    CSR_REPLAY_TMASK          = 12,
    CSR_REPLAY_PMU_READ       = 13,
    CSR_REPLAY_TEX            = 14,
    CSR_REPLAY_TW_TL0         = 15,
    CSR_REPLAY_TW_TL1         = 16,
    CSR_REPLAY_TW_PREFETCH0   = 17,
    CSR_REPLAY_TW_PREFETCH1   = 18,
    CSR_REPLAY_TW_CACHE_OP    = 19,
    CSR_REPLAY_TW_TENSOR_FMA  = 20,
    CSR_REPLAY_TW_TENSOR_STORE  = 21,
    CSR_REPLAY_TW_TENSOR_REDUCE = 22,
    CSR_REPLAY_TW_TENSOR_QUANT  = 23,
    CSR_REPLAY_SZ             = 24
  } csr_replay_e;

  localparam int CsrReplaySz = 24;  // CSR_REPLAY_SZ

  // ── CSR constants (from csr_defines.vh) ─────────────────────────────
  localparam int CsrNrEvents        = 32;
  localparam int CsrNrEventsDcache  = 12;
  localparam int CsrNrEventsVpu     = 11;
  localparam int CsrNrEventsBranch  = 2;
  localparam int CsrNrEventsExt     = CsrNrEventsDcache + CsrNrEventsVpu + CsrNrEventsBranch;
  localparam int CsrNrEventsL       = $clog2(CsrNrEvents);
  localparam int CsrFirstHpm        = 3;
  localparam int CsrLastCoreHpm     = 6;
  localparam int CsrLastHpm         = 8;
  localparam int CsrNrCounters      = CsrLastHpm + 1;
  localparam int CsrMaxNrCounters   = CsrNrCounters;
  localparam int CsrMaxNrCountersL  = $clog2(CsrMaxNrCounters);
  localparam int CsrFirstCounter    = 32'd3072;  // csr_ad_CYCLE

  localparam logic [63:0] CsrVendorId = 64'h5e5;
  localparam logic [63:0] CsrArchId   = 64'h8000_0000_0000_0001;
  localparam logic [63:0] CsrImpId    = 64'h0;
  localparam logic [63:0] CsrIsaMax   = ((64'h1 << 2)  |
                                          (64'h1 << 5)  |
                                          (64'h1 << 8)  |
                                          (64'h1 << 12) |
                                          (64'h1 << 18) |
                                          (64'h1 << 20) |
                                          (64'h1 << 23) |
                                          (64'h2 << 62));

  localparam int TensorErrorSz     = 10;
  localparam int CsrUcacheCtrlSz   = 11;
  localparam int CsrMcacheCtrlSz   = 2;
  localparam int unsigned CsrUcacheCtrlResetVal = 32'd512;

  // Privilege levels
  parameter logic [1:0] PrvH = 2'd2;

  // Debug address space
  localparam logic [7:0] CsrDebugOnlyAdMsb = 8'h7B;

  // ── SATP constants ────────────────────────────────────────────────────
  localparam int CsrSatpModeSz = 4;
  localparam int CsrSatpAsidSz = 4;
  localparam int CsrSatpPpnSz  = 44;
  localparam logic [3:0] CsrSatpModeBare = 4'd0;
  localparam logic [3:0] CsrSatpModeSv39 = 4'd8;
  localparam logic [3:0] CsrSatpModeSv48 = 4'd9;

  // ── DCache error constants ────────────────────────────────────────────
  localparam int DcacheTlErrorBits       = 5;
  localparam int DcacheDbgCsrTlSize      = 3;
  localparam int DcacheDbgCsrTsSize      = 4;
  localparam int DcacheDbgCsrCouSize     = 4;
  localparam int DcacheDbgCsrCoul2Size   = 3;
  localparam int DcacheDbgCsrTxsndSize   = 2;
  localparam int DcacheDbgCsrMhSize      = 4;
  localparam int DcacheDbgAddrBits       = 10;
  localparam int DcacheTlErrorUnlockLine = 0;
  localparam int DcacheTlErrorTransValid = 1;
  localparam int DcacheTlErrorWrongOffst = 2;
  localparam int DcacheTlErrorAccessFault0 = 3;
  localparam int DcacheTlErrorAccessFault1 = 4;
  localparam int DcacheTxErrorScpbtUnset = 4;
  localparam int DcacheCoErrorLockVa     = 5;
  localparam int DcacheErrFlagCoLock     = 0;
  localparam int DcacheErrFlagMemFault   = 1;
  localparam int DcacheErrFlagRange      = 1;  // 1:0
  localparam int DcacheTlL2Transfers     = 4;
  localparam int DcacheTlL2TransfersSelBits = $clog2(DcacheTlL2Transfers);
  localparam int DcacheTlTenbLookAhead   = 2;
  localparam int DcacheTlScpMaxIdx       = 48;
  localparam int DcacheTenbBits          = 2;
  localparam int DcacheTlTenbCredits     = 6;
  localparam int VpuTenbSize             = 8;
  localparam int VpuTenbCreditBits       = $clog2(VpuTenbSize);
  localparam int VpuTenbRcvEntryBits     = VpuTenbSize / 2;
  localparam int VpuTenbRcvEntryIdxBits  = $clog2(VpuTenbSize / 2);
  localparam int DcacheL2MissReqPorts    = 3;
  localparam int DcacheL2EvictReqPorts   = 2;
  localparam int BpamShireApbDataWidth   = 64;
  localparam int MinionDebugApbDataWidth = 64;
  localparam int MinionDebugApbAddrWidth = 10;
  localparam int MinionDebugFfbWords     = 4;
  localparam int MinRegApbAddrWidth      = 12;
  localparam int MinDcacheApbAddrWidth   = 10;
  localparam int MinDmApbAddrWidth       =
      ((MinRegApbAddrWidth > MinDcacheApbAddrWidth) ? MinRegApbAddrWidth : MinDcacheApbAddrWidth) + 1;
  localparam int MinionDebugApbAddrUpperBits = 1;
  localparam logic [MinionDebugApbAddrUpperBits-1:0] MinionDebugApbAddrDebug  = '0;
  localparam logic [MinionDebugApbAddrUpperBits-1:0] MinionDebugApbAddrDcache = 1'b1;
  localparam int AdEsrProtBitsSize       = 2;
  localparam logic [AdEsrProtBitsSize-1:0] AdEsrProtBitsMsg = 2'd2;
  localparam int MinionMemCtrlBits       = 2;
  localparam int MinionMemCtrlWe         = 1;
  localparam int MinionMemCtrlRe         = 0;
  localparam int ShireMinionMemOverrideBits = 3;
  localparam int ShireMinionMemOverrideSel  = 2;
  localparam int ShireMinionMemOverrideWe   = 1;
  localparam int ShireMinionMemOverrideRe   = 0;
  localparam int TmaskCountBits          = 4;
  localparam int ShireCoopSeqIdSize      = 3;
  localparam int ShireTeRetiresPerCycle  = 1;
  localparam int ShireTeInstrWidth       = InstSize;
  localparam int ShireTeContextWidth     = 1;
  localparam int ShireTeEcauseWidth      = 5;
  localparam int ShireTePrivilegeWidth   = 2;
  localparam int ShireTeStatusWidth      = 1;
  localparam int NeighDebugFilterWidth   = 200;
  localparam int NeighDebugMatchWidth    = 64;
  localparam int NeighDebugDataWidth     = 128;
  localparam int NeighDebugSmMuxDataWidth = 2;
  localparam int NeighDebugSmMuxModWidth  = 5;
  localparam int NeighDebugSmMuxWidth     = NeighDebugSmMuxDataWidth + NeighDebugSmMuxModWidth;
  localparam int EtEcoMinionInputWidth   = 10;
  localparam int EtEcoMinionOutputWidth  = 10;

  // ── DCache reduce constants ───────────────────────────────────────────
  localparam logic [1:0] DcacheReduceSend = 2'b00;
  localparam logic [1:0] DcacheReduceRecv = 2'b01;
  localparam logic [1:0] DcacheReduceDoNothing = 2'b10;
  localparam int DcacheReduceActionBits = 2;
  localparam int DcacheReduceLevelBits = 4;
  localparam int DcacheReduceMsgDataBits = 5;
  localparam int DcacheReduceMsgDataMsb = DcacheReduceMsgDataBits - 1;
  localparam int DcacheTsColsBits = 3;
  localparam int DcacheTsRowsBits = 4;
  localparam int DcacheReCountBits = 8;
  localparam int DcacheReActionBits = 2;
  localparam int DcacheTsRateBits = 4;
  localparam int DcacheTsRateCounterBits = 6;
  localparam int DcacheTsBackoffCounterBits = 4;
  localparam int DcacheTsTransAddrCntBits = 2;
  localparam int DcacheTsPhysAddrNum = 2;

  // ── PMU constants ─────────────────────────────────────────────────────
  localparam int PmuMinionCountersNum    = 8;
  localparam int PmuNeighCountersNum     = 4;
  localparam int PmuTotalCountersNum     = PmuMinionCountersNum + PmuNeighCountersNum;
  localparam int PmuNeighEvents          = 25;
  localparam int PmuNeighEventsSelBits   = $clog2(PmuNeighEvents);
  localparam int PmuNeighEventCntSelBits = PmuNeighEventsSelBits * PmuNeighCountersNum;

  // ── DCache mode enum ──────────────────────────────────────────────────
  typedef enum logic [1:0] {
    DcacheMode_Shared     = 2'b00,
    DcacheMode_Split      = 2'b01,
    DcacheMode_Invalid    = 2'b10,
    DcacheMode_Scratchpad = 2'b11
  } dcache_mode_e;

  localparam logic [1:0] DcacheMode_Shared_v     = 2'b00;
  localparam logic [1:0] DcacheMode_Split_v      = 2'b01;
  localparam logic [1:0] DcacheMode_Invalid_v    = 2'b10;
  localparam logic [1:0] DcacheMode_Scratchpad_v = 2'b11;

  // ── Tensor quant constants ────────────────────────────────────────────
  localparam int TquantBitsPerTransform  = 4;
  localparam int TquantNumCfgTransforms  = 10;
  localparam int TquantTransBits         = TquantBitsPerTransform * TquantNumCfgTransforms;
  localparam logic [3:0] TquantLast             = 4'b0000;
  localparam logic [3:0] TquantInt32ToFp32Cvt   = 4'b0001;
  localparam logic [3:0] TquantFp32ToInt32Cvt   = 4'b0010;
  localparam logic [3:0] TquantRelu             = 4'b0011;
  localparam logic [3:0] TquantInt32AddRow      = 4'b0100;
  localparam logic [3:0] TquantInt32AddCol      = 4'b0101;
  localparam logic [3:0] TquantFp32MulRow       = 4'b0110;
  localparam logic [3:0] TquantFp32MulCol       = 4'b0111;

  // ── Tensor error struct types ─────────────────────────────────────────
  typedef struct packed {
    logic [DcacheTlErrorBits-3:0] tload;
    logic                         fcc;
    logic                         scp_unset;
    logic                         co;
    logic                         tenb;
    logic                         mem_fault;
    logic                         tstore;
    logic                         treduce;
  } terror_t0_t;

  typedef struct packed {
    logic                         fcc;
    logic                         scp_unset;
    logic                         co;
    logic                         mem_fault;
  } terror_t1_t;

  // ── ESR minion features ───────────────────────────────────────────────
  typedef struct packed {
    logic disable_lock_unlock;
    logic disable_multithreading;
    logic trap_on_u_scp;
    logic trap_on_u_cacheops;
    logic trap_on_ml;
    logic trap_on_gfx;
  } esr_minion_features_t;

  // ── Exception cause enum (alias for xcpt_cause_e with mcode) ──────────
  localparam logic [4:0] XcptMcodeInstruction = 5'h1e;

  // ── intpipe_status_t ──────────────────────────────────────────────────
  typedef struct packed {
    logic       mprv;
    logic [1:0] mpp;
    logic       sum;
    logic       mxr;
    logic       fs;
  } intpipe_status_t;

  // ══════════════════════════════════════════════════════════════════════
  // Placeholder types for dcache/vpu interfaces
  // (These are complex structs that will be properly defined later
  //  when dcache and vpu modules are translated. For now they are
  //  opaque so this file can be parsed.)
  // ══════════════════════════════════════════════════════════════════════

  // Cache-op command enum
  typedef enum logic [3:0] {
    cache_op_Lock_VA   = 4'b0000,
    cache_op_Unlock_VA = 4'b0001,
    cache_op_Flush_SW  = 4'b0010,
    cache_op_Evict_SW  = 4'b0011,
    cache_op_Pref      = 4'b0100,
    cache_op_Snoop     = 4'b0101,
    cache_op_Flush_VA  = 4'b0110,
    cache_op_Evict_VA  = 4'b0111,
    cache_op_Lock_SW   = 4'b1000,
    cache_op_Unlock_SW = 4'b1001
  } cache_op_cmd_e;

  // Cache-op level enum
  typedef enum logic [1:0] {
    cache_op_L1  = 2'b00,
    cache_op_L2  = 2'b01,
    cache_op_L3  = 2'b10,
    cache_op_MEM = 2'b11
  } CacheOpLevel_t;

  // Tensor load transform enum
  typedef enum logic [2:0] {
    tensorload_transform_none   = 3'b000,
    tensorload_transform_t1     = 3'b001,
    tensorload_transform_t2     = 3'b010,
    tensorload_transform_t3     = 3'b011,
    tensorload_transform_t4     = 3'b100,
    tensorload_transform_t5     = 3'b101,
    tensorload_transform_t6     = 3'b110,
    tensorload_transform_t7     = 3'b111
  } tensorload_transform;

  // Tensor load offset enum
  typedef enum logic [1:0] {
    tensorload_offset_0 = 2'b00,
    tensorload_offset_1 = 2'b01,
    tensorload_offset_2 = 2'b10,
    tensorload_offset_3 = 2'b11
  } tensorload_offset;

  // ── Shire cooperation constants ─────────────────────────────────────
  parameter int unsigned ShireCoopNeighMaskSize = 4;   // NUM_NEIGH
  parameter int unsigned ShireCoopMinMaskSize   = 8;   // MIN_PER_N
  parameter int unsigned ShireCoopIdSize        = 5;   // $clog2(32)
  parameter int unsigned DcacheTensorCtrlDestBits = 6;  // $clog2(WAYS*SETS)
  parameter int unsigned VaCaBits               = 42;   // VA_SIZE-1 downto 6 = [47:6] = 42 bits

  typedef CacheOpLevel_t cache_op_level_e;

  typedef enum logic [2:0] {
    TensorTransNone  = 3'b000,
    TensorTransInt8  = 3'b001,
    TensorTransInt16 = 3'b010,
    TensorTransTrp8  = 3'b101,
    TensorTransTrp16 = 3'b110,
    TensorTransTrp32 = 3'b111
  } tensorload_transform_e;

  // ── Tensor load control ───────────────────────────────────────────
  typedef struct packed {
    logic                                     use_tmask;
    logic                                     use_coop;
    tensorload_transform_e                    transform;
    logic [DcacheTensorCtrlDestBits-1:0]       scp_dest;
    logic                                     tl_sel;
    logic [VaCaBits-1:0]                       addr;
    tensorload_offset                          offset;
    logic [DcacheTensorCtrlNlinesBits-1:0]     num_lines;
  } tensorload_control_t;

  // ── Tensor cooperation control ────────────────────────────────────
  typedef struct packed {
    logic [ShireCoopNeighMaskSize-1:0] neigh_mask;
    logic [ShireCoopMinMaskSize-1:0]   minion_mask;
    logic [2:0]                        padding;
    logic [ShireCoopIdSize-1:0]        id;
  } tensorcoop_control;

  typedef struct packed {
    logic                         read_en;
    logic [DcacheWayBits-1:0]     way;
    logic [DcacheLramAddrBits-1:0] addr;
    logic                         size64;
    logic                         size256;
    logic                         bid;
  } vpu_dcache_scp_req;

  typedef struct packed {
    vpu_dcache_scp_req            scp_req;
    logic                         tenb_credit;
    logic [VpuTenbRcvEntryIdxBits-1:0] tenb_credit_entry;
    logic                         tfma_enabled;
    logic                         reduce_wait;
    logic                         tfma_rf_write;
  } vpu_dcache_ctrl;

  typedef struct packed {
    logic send_reg;
    logic exec_op;
    logic nothing;
  } dcache_vpu_reduce_ctrl_t;

  typedef dcache_vpu_reduce_ctrl_t dcache_vpu_reduce_ctrl;

  typedef struct packed {
    logic       fill_is_tenb_early;
    logic       fill_is_tenb;
    logic       tenb_flush;
    logic [3:0] tenb_line;
  } dcache_vpu_scp_resp;

  // ── Cache op request ──────────────────────────────────────────────
  typedef struct packed {
    cache_op_cmd_e                    cmd;
    cache_op_level_e                   start_level;
    cache_op_level_e                   dest_level;
    logic [3:0]                        count;
    logic [VaSize-1:0]                 addr;
    logic [DcacheWayBits-1:0]          way;
    logic [VaSize-1:0]                 stride;
    logic                              thread_id;
    logic                              use_tmask;
    logic                              tmask_ready;
    logic [TensorMaskBits-1:0]         tmask_bits;
    logic [DcacheL2ScpDstBits-1:0]     l2_scp_dest;
    logic                              l2_scp_fill;
  } core_dcache_op_req_t;

  typedef struct packed {
    cache_op_cmd_e                    cmd;
    cache_op_level_e                   start_level;
    cache_op_level_e                   dest_level;
  } core_dcache_op_req_next_t;

  // ── Tex send control ──────────────────────────────────────────────
  typedef struct packed {
    logic [$clog2(NrMsgPorts)-1:0] port_id;
    logic [2:0]                    size;
    logic [DcacheWayBits-1:0]      way;
    /* verilator lint_off SYMRSVDWORD */  // Matches the original cache-op field naming.
    logic [DcacheSetBits-1:0]      set;
    /* verilator lint_on SYMRSVDWORD */
    logic                          port_en;
    logic                          thread_id;
  } texsend_control_t;

  // ── Cache control ─────────────────────────────────────────────────
  typedef struct packed {
    logic [DcacheCoL2MaxOpsBits-1:0]   cacheop_max;
    logic                              padding;
    logic [DcacheCoL2ReprateBits-1:0]  cacheop_reprate;
    logic                              scpenable;
    logic                              d1split;
  } cache_control_t;

  // ── Reduce control ────────────────────────────────────────────────
  typedef struct packed {
    logic [1:0]  padding3;
    logic [4:0]  start_reg;
    logic [28:0] padding2;
    logic [3:0]  op;
    logic        padding1;
    logic [6:0]  num_regs;
    logic [12:0] partner;
    logic        padding0;
    logic [1:0]  action;
  } reduce_control_t;

  // ── Tensor store control ──────────────────────────────────────────
  typedef struct packed {
    logic [1:0]          src_inc;
    logic [4:0]          start_reg;
    logic [1:0]          cols;
    logic [3:0]          rows;
    logic [1:0]          coop;
    logic                scp;
    logic [VaSize-1:4]   addr;
    logic [3:0]          rate;
  } tensorstore_control;

  typedef struct packed {
    logic [1:0]          stride_entry;
    logic [5:0]          start_entry;
    logic                padding2;
    logic [3:0]          rows;
    logic [1:0]          padding1;
    logic                scp;
    logic [VaSize-1:6]   addr;
    logic [1:0]          padding0;
    logic [3:0]          rate;
  } tensorstore_scp_control_t;

  typedef union packed {
    reduce_control_t          reduce;
    tensorstore_control       tensorstore;
    tensorstore_scp_control_t tensorstore_scp;
  } reduce_tensorstore_control_t;

  // ── Tensor FMA control ────────────────────────────────────────────
  typedef struct packed {
    logic        is_conv;
    logic [5:0]  padding3;
    logic [1:0]  cols_b;
    logic [3:0]  rows_a;
    logic [3:0]  cols_a;
    logic [3:0]  start_a;
    logic [18:0] padding2;
    logic        to_vrf;
    logic        u_b;
    logic        u_a;
    logic        ten_b;
    logic [1:0]  padding1;
    logic [5:0]  scp_b;
    logic [1:0]  padding0;
    logic [5:0]  scp_a;
    logic [2:0]  mode;
    logic        first_pass;
  } tensorfma_control;

  // ── Tensor quant control ──────────────────────────────────────────
  typedef struct packed {
    logic [1:0]  padding2;
    logic [4:0]  start_reg;
    logic [1:0]  cols;
    logic [3:0]  rows;
    logic [5:0]  scp_src;
    logic [4:0]  padding;
    logic [39:0] trans;
  } tensorquant_control;

  // ── Core → DCache control ─────────────────────────────────────────
  typedef struct packed {
    tensorload_control_t          tensorload_ctrl;
    tensorcoop_control            tensorcoop_ctrl;
    logic [VaSize-1:0]            tensorload_stride;
    logic                         tensorload_start;
    logic                         tensorload_restart_b;
    logic                         tensorload_tmask_ready;
    logic [TensorMaskBits-1:0]    tensorload_tmask_bits;
    logic                         cache_op_valid;
    core_dcache_op_req_t          cache_op_req;
    core_dcache_op_req_next_t     cache_op_req_next;
    logic [DcacheWayBits-1:0]     msg_port_wr_way;
    logic [DcacheLramAddrBits-1:0] msg_port_wr_addr;
    logic                         msg_port_wr_en;
    logic                         reduce_start;
    logic                         tensorstore_start;
    reduce_tensorstore_control_t  reduce_tensorstore_ctrl;
    logic [VaSize-1:0]            tensorstore_stride;
    logic                         tex_send_valid;
    texsend_control_t             tex_send_ctrl;
    cache_control_t               cache_ctrl;
    logic                         thread_id;
  } core_dcache_ctrl;

  // ── Core → VPU control ────────────────────────────────────────────
  typedef struct packed {
    logic                         tensorfma_start;
    tensorfma_control             tensorfma_ctrl;
    logic                         tensorfma_conv_bits_ready;
    logic [TensorMaskBits-1:0]    tensorfma_conv_bits;
    logic                         reduce_start;
    logic                         tensorstore_start;
    reduce_tensorstore_control_t  reduce_ctrl;
    logic                         tensorquant_start;
    tensorquant_control           tensorquant_ctrl;
    logic                         pending_scoreboard_t0;
  } core_vpu_ctrl;

  typedef enum logic [3:0] {
    minion_debug_nxdata0    = 4'h0,
    minion_debug_nxdata1    = 4'h1,
    minion_debug_axdata0    = 4'h2,
    minion_debug_axdata1    = 4'h3,
    minion_debug_axprogbuf0 = 4'h4,
    minion_debug_axprogbuf1 = 4'h5,
    minion_debug_nxprogbuf0 = 4'h6,
    minion_debug_nxprogbuf1 = 4'h7,
    minion_debug_abscmd     = 4'h8,
    minion_debug_invalid    = 4'h9
  } minion_debug_apb_regs_t;

  typedef struct packed {
    logic [NrThreads-1:0] halt;
    logic [NrThreads-1:0] resume;
    logic [NrThreads-1:0] resethalt;
    logic [NrThreads-1:0] ackhavereset;
  } minion_debug_in_t;

  typedef struct packed {
    logic [NrThreads-1:0] halted;
    logic [NrThreads-1:0] running;
    logic [NrThreads-1:0] have_reset;
    logic [NrThreads-1:0] busy;
    logic [NrThreads-1:0] exception;
    logic [NrThreads-1:0] error;
  } minion_debug_out_t;

  typedef struct packed {
    logic rsv_1;
    logic rsv_0;
    logic min_frontend_clock_gate_disable;
    logic min_dcache_clock_gate_disable;
    logic min_vputrans_clock_gate_disable;
    logic min_vputima_clock_gate_disable;
    logic min_vpulane_clock_gate_disable;
    logic min_intpipe_clock_gate_disable;
  } minion_chicken_bits_t;

  typedef struct packed {
    logic [NeighDebugFilterWidth-1:0] filter_signals;
    logic                             filter_valid;
    logic [NeighDebugMatchWidth-1:0]  match_signals;
    logic                             match_valid;
    logic [NeighDebugDataWidth-1:0]   data_signals;
    logic                             data_valid;
  } neigh_sm_dbg_monitor_t;

  // ── DCache debug signals ──────────────────────────────────────────
  typedef struct packed {
    logic [DcacheDbgCsrTlSize-1:0]    csr_signals_tl0;
    logic [DcacheDbgCsrTlSize-1:0]    csr_signals_tl1;
    logic [DcacheDbgCsrTsSize-1:0]    csr_signals_ts;
    logic [DcacheDbgCsrCouSize-1:0]   csr_signals_cou;
    logic [DcacheDbgCsrCoul2Size-1:0] csr_signals_coul2;
    logic [DcacheDbgCsrTxsndSize-1:0] csr_signals_txsnd;
    logic [DcacheDbgCsrMhSize-1:0]    csr_signals_mh0;
    logic [DcacheDbgCsrMhSize-1:0]    csr_signals_mh1;
  } dcache_debug_sigs;

  typedef struct packed {
    logic local_message_ready;
    logic tbox_min_rsp_ready;
    logic fill_ff_ready_for_no_msg;
  } fill_ff_dbg_sm_t;

  // ── CSR command type (original name alias) ────────────────────────
  typedef csr_cmd_e csr_cmd_t;

  // ── DCache core ctrl (original name alias) ────────────────────────
  typedef dcache_core_ctrl_t dcache_core_ctrl;

  // ══════════════════════════════════════════════════════════════════════
  // Additional constants for intpipe / dcache / vpu interfaces
  // ══════════════════════════════════════════════════════════════════════

  parameter int unsigned VpuDataSz             = 256;
  parameter int unsigned VpuLaneNum            = 8;
  parameter int unsigned VpuDataSSz            = VpuDataSz / VpuLaneNum;  // 32-bit per-lane VPU data
  parameter int unsigned CsrNrEventsDcacheVpu  = 23;
  parameter int unsigned DcacheErrFlagBits     = 2;
  parameter int unsigned ShireTeInstrAddrWidth = 49;  // VaSizeExt
  parameter int unsigned NumShireIdsBits       = 8;   // $clog2(256)
  parameter int unsigned MaxMinionIdBits       = 13;  // $clog2(NUM_SHIRE_IDS*NUM_NEIGH*MIN_PER_N)
  parameter int unsigned MinPerSBits           = 5;   // $clog2(32)
  parameter int unsigned AdEsrShireHartIdSize  = 8;
  // CoreGscCntBits already defined above (= 3)
  parameter int unsigned CoreGsc32IdxVSize     = 40;  // 5*8
  parameter int unsigned DmemReqPsMaskSz       = 8;
  parameter int unsigned DcacheDataBits        = 64;
  // DcacheReplayqSize already defined above (= 8)
  parameter int unsigned CoreVpuStoreDataSz    = 256; // VpuDataSz
  // CsrSatpModeSz already defined above (= 4)
  parameter int unsigned PaTransSize           = 28;  // PPN bits

  typedef struct packed {
    logic [ShireTeRetiresPerCycle-1:0]                           instr_valid;
    logic [(ShireTeRetiresPerCycle * ShireTeInstrWidth)-1:0]     instr_bus;
    logic [(ShireTeRetiresPerCycle * ShireTeInstrAddrWidth)-1:0] instr_addr;
    logic [(ShireTeRetiresPerCycle * ShireTeContextWidth)-1:0]   instr_context;
    logic [ShireTeRetiresPerCycle-1:0]                           exception;
    logic [(ShireTeRetiresPerCycle * ShireTeEcauseWidth)-1:0]    exception_cause;
    /* verilator lint_off SYMRSVDWORD */  // Matches the original trace encoder field name.
    logic [ShireTeRetiresPerCycle-1:0]                           interrupt;
    /* verilator lint_on SYMRSVDWORD */
    logic [(ShireTeRetiresPerCycle * ShireTeInstrAddrWidth)-1:0] exception_trap_value;
    logic [(ShireTeRetiresPerCycle * ShireTePrivilegeWidth)-1:0] privilege_level;
    logic [ShireTeStatusWidth-1:0]                               status_flags;
    logic                                                        cpu_halted;
    logic                                                        min_reset;
  } trace_encoder_signals_t;

  typedef trace_encoder_signals_t TraceEncoder_signals;

  typedef struct packed {
    logic                id_csr_interrupt;
    logic                excl_mode_stall;
    logic [NrThreads-1:0] ipi_with_pc;
    logic                id_xcpt;
    logic                wb_xcpt;
    logic                csr_fe_stall;
    logic                id_do_fence;
  } intpipe_dbg_match_t;

  // ══════════════════════════════════════════════════════════════════════
  // Frontend request debug info
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic halt;
    logic resume;
  } minion_fe_req_debug;

  // ══════════════════════════════════════════════════════════════════════
  // Frontend request (intpipe → frontend)
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [PcSizeExt-1:0] pc;
    logic                 speculative;
    minion_fe_req_debug   debug_info;
  } minion_fe_req;

  // ══════════════════════════════════════════════════════════════════════
  // Frontend → core response
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [PcSizeExt-1:0] pc;
    logic [InstSize-1:0]  inst_bits;
    logic                 pf0;
    logic                 pf1;
    logic                 af0;
    logic                 af1;
    logic                 replay;
    logic                 rvc;
    logic                 bus_error;
    logic                 ecc_error;
  } frontend_core_resp;

  // ══════════════════════════════════════════════════════════════════════
  // DCache tag exceptions
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic pf_ld;
    logic pf_st;
    logic mf_ld;
    logic mf_st;
    logic af_ld;
    logic af_st;
    logic ps_ld;
    logic ps_st;
  } dcache_tag_xcpt;

  // ══════════════════════════════════════════════════════════════════════
  // VM status for dcache / TLB (alias — vm_status_t already defined)
  // ══════════════════════════════════════════════════════════════════════
  // vm_status_t (defined above) serves as minion_vm_status.

  // ══════════════════════════════════════════════════════════════════════
  // SATP info (mode + ppn for TLB / PTW)
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [CsrSatpModeSz-1:0] mode;
    logic [PaTransSize-1:0]   ppn;
  } minion_satp_info;

  // ══════════════════════════════════════════════════════════════════════
  // TLB/PTW shared types
  // ══════════════════════════════════════════════════════════════════════

  typedef enum logic [1:0] {
    tlb_entry_type_4K   = 2'h0,
    tlb_entry_type_RSVD = 2'h1,
    tlb_entry_type_2M   = 2'h2,
    tlb_entry_type_1G   = 2'h3
  } tlb_entry_type;

  typedef struct packed {
    logic [CsrSatpModeSz-1:0]     satp_mode;
    logic [PaTransSize-1:0]       satp_ppn;
    logic [1:0]                   prv;
    logic [VaSize-1:VaUntransSize] addr;
  } minion_ptw_req;

  typedef struct packed {
    logic                         canceled_req;
    logic                         access_fault;
    logic [7:0]                   rsvd_for_hw;
    logic [CsrSatpPpnSz-1:0]      ppn;
    logic [1:0]                   rsvd_for_sw;
    logic                         d;
    logic                         a;
    logic                         g;
    logic                         u;
    logic                         x;
    logic                         w;
    logic                         r;
    logic                         v;
  } minion_ptw_pte;

  typedef struct packed {
    vm_status_t                    status;
    logic [VaSize-1:VaUntransSize] vpn;
    logic                          passthrough;
    logic                          instruction;
    logic                          store;
    logic                          msb_err;
  } tlb_req;

  typedef struct packed {
    logic                         fill_pending;
    logic                         miss;
    logic [PaSize-1:PaUntransSize] ppn;
    logic                         xcpt_ld;
    logic                         xcpt_st;
    logic                         xcpt_if;
    logic                         access_fault;
  } tlb_resp;

  // ══════════════════════════════════════════════════════════════════════
  // Interrupts from platform to intpipe
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [NrThreads-1:0]              mieco;
    logic [NrThreads-1:0]              mtip;
    logic [NrThreads-1:0]              msip;
    logic [NrThreads-1:0]              meip;
    logic [NrThreads-1:0]              seip;
    logic [(NrThreads*FccPerMin)-1:0]  fcc;
    logic [NrThreads-1:0]              ipi_with_pc;
    logic [PcSize-2:0]                 ipi_pc;
  } minion_interrupts;

  // ══════════════════════════════════════════════════════════════════════
  // Minion → DCache request
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [VaSize-1:0]                 addr;
    logic                              addr_msb_err;
    minion_reg_dest_t                  dest;
    dcache_cmd_e                       cmd;
    dcache_type_e                      typ;
    logic [CoreGscCntBits-1:0]         gsc_cnt;
    logic [DmemReqPsMaskSz-1:0]       ps_mask;
    logic                              phys;
    logic [CoreGsc32IdxVSize-1:0]      gsc32_idx;
    logic                              mem_global;
`ifdef DCACHE_REPORT_PC
    logic [PcSizeExt-1:0]             pc;
`endif
  } minion_dcache_req;

  // ══════════════════════════════════════════════════════════════════════
  // DCache → minion response
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    minion_reg_dest_t                  dest;
    dcache_type_e                      typ;
    logic [CoreGscCntBits-1:0]         gsc_cnt;
    logic [DmemReqPsMaskSz-1:0]       ps_mask;
    logic [VpuDataSz-1:0]             data;
    logic [VaSizeExt-1:0]             addr;
  } dcache_minion_resp;

  // ══════════════════════════════════════════════════════════════════════
  // DCache → minion scoreboard (replay queue)
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [DcacheReplayqSize-1:0]      valid;
    minion_reg_dest_t [DcacheReplayqSize-1:0] dest;
  } dcache_minion_scoreboard;

  // ══════════════════════════════════════════════════════════════════════
  // DCache breakpoint info
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              thread_id;
    logic [VaSizeExt-1:0]             address;
    debug_bp_e                         cmd;
  } dcache_core_bp_t;

  // ══════════════════════════════════════════════════════════════════════
  // Minion → VPU ID-stage request
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              valid;
    logic                              thread_id;
    logic [2:0]                        fcsr_rm;
    logic [InstSize-1:0]              inst_bits;
  } minion_vpu_id_req;

  // ══════════════════════════════════════════════════════════════════════
  // Minion → VPU EX-stage request
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              gscing;
    logic [CoreGscCntBits-1:0]         gsc_src;
    logic                              kill;
    logic [XregSize-1:0]              fromint_data;
    logic                              thread_id;
  } minion_vpu_ex_req;

  // ══════════════════════════════════════════════════════════════════════
  // DCache → VPU response
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              thread_id;
    logic [CoreGscCntBits-1:0]         gdst;
    dcache_type_e                      typ;
    logic [4:0]                        addr;
    logic [VpuDataSz-1:0]             data;
    logic [DmemReqPsMaskSz-1:0]       ps_mask;
  } dcache_vpu_resp;

  // ══════════════════════════════════════════════════════════════════════
  // VPU → minion EX-stage control
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              tointm;
    logic                              illegal_rm;
    logic [DmemReqPsMaskSz-1:0]       ps_mask;
    logic [31:0]                       gsc_fs;
  } vpu_minion_ex_ctrl;

  // ══════════════════════════════════════════════════════════════════════
  // VPU → minion TAG-stage control
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              fma;
    logic [CoreVpuStoreDataSz-1:0]    store_data;
    logic [VpuDataSSz-1:0]            scatter_data;
    logic                              tointm;
  } vpu_minion_tag_ctrl;

  // ══════════════════════════════════════════════════════════════════════
  // VPU → minion MEM-stage control
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              fma;
    logic                              tointm;
  } vpu_minion_mem_ctrl;

  // ══════════════════════════════════════════════════════════════════════
  // VPU → minion WB-stage control
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              fma;
    logic [XregSize-1:0]              toint_data;
    logic                              mova_mx;
    logic                              fcsr_flags_valid;
    logic [5:0]                        fcsr_flags;
    logic                              thread_id;
    logic                              tointm;
  } vpu_minion_wb_ctrl;

  // ══════════════════════════════════════════════════════════════════════
  // Debug monitor output from intpipe
  // (concatenation of 31 signals — packed struct for the port type)
  // ══════════════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                              id_thread_id;
    logic [PcSizeExt-1:0]            id_pc;
    logic                              id_valid;
    logic                              id_valid_qual;
    logic                              id_inst_replay;
    logic                              id_take_pc;
    logic                              id_csr_interrupt;
    logic                              excl_mode_stall;
    logic                              ex_valid_qual;
    logic                              tag_valid_qual;
    logic                              mem_valid_qual;
    logic                              wb_valid;
    logic [CoreGscCntBits-1:0]        gsc_reg_gsc_cnt;
    logic                              tag_take_pc;
    logic [NrThreads-1:0]              ipi_with_pc;
    logic                              wb_take_pc;
    logic                              id_xcpt;
    logic                              wb_xcpt;
    logic                              id_stall_fp2int;
    logic                              ex_gsc_busy;
    logic                              csr_fe_stall;
    logic                              id_ctrl_stall_csr;
    logic                              id_ctrl_stall_dcache;
    logic                              id_do_fence;
    logic                              id_ctrl_stall_trans;
    logic                              id_ctrl_stall_div;
    logic                              id_int_sboard_hazard;
    logic                              id_ctrl_stall_hazard_vpu;
    logic                              id_ctrl_stall_hazard;
  } intpipe_dbg_monitor_t;

  // ══════════════════════════════════════════════════════════════════════
  // Helper functions for dcache commands
  // ══════════════════════════════════════════════════════════════════════

  function automatic logic dcache_cmd_is_amo(dcache_cmd_e cmd);
    return (cmd == DcCmdXaSwap) || (cmd == DcCmdXaAdd) ||
           (cmd == DcCmdXaXor)  || (cmd == DcCmdXaOr)  ||
           (cmd == DcCmdXaAnd)  || (cmd == DcCmdXaMin) ||
           (cmd == DcCmdXaMax)  || (cmd == DcCmdXaMinu) ||
           (cmd == DcCmdXaMaxu) || (cmd == DcCmdXaCmpswp) ||
           (cmd == DcCmdXaMinPs) || (cmd == DcCmdXaMaxPs);
  endfunction

  function automatic logic dcache_cmd_is_gsc32(dcache_cmd_e cmd);
    return (cmd == DcCmdG32) || (cmd == DcCmdSc32);
  endfunction

  function automatic logic dcache_cmd_is_write(dcache_cmd_e cmd);
    return (cmd == DcCmdXwr) || (cmd == DcCmdXsc) ||
           dcache_cmd_is_amo(cmd) ||
           (cmd == DcCmdSc32) || (cmd == DcCmdXwrb);
  endfunction

  // ══════════════════════════════════════════════════════════════════════
  // VA sign-extension function
  // ══════════════════════════════════════════════════════════════════════

  function automatic logic [VaSizeExt-1:0] extend_VA(input logic [XregSize-1:0] in);
    begin
      extend_VA[VaSize-1:0] = in[VaSize-1:0];
      // if bits 48..63 are all the same as bit 47, then sign-extend
      if (&in[63:VaSize-1] || ~|in[63:VaSize-1])
        extend_VA[VaSizeExt-1] = extend_VA[VaSize-1];  // extra bit is sign extension
      else
        extend_VA[VaSizeExt-1] = !extend_VA[VaSize-1]; // extra bit is negating
    end
  endfunction

endpackage
/* verilator lint_on UNUSED */
