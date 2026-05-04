// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Focused FPGA bring-up harness for preview-plus-RF write contracts.

module test_rf_preview #(
  parameter int unsigned ClkHz    = 1_152_000,
  parameter int unsigned BaudRate = 115200
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  typedef enum logic [8:0] {
    StInit,
    StP0Write,
    StP0Check,
    StP1Write,
    StP1Check,
    StP2Arm,
    StP2Commit,
    StP2Check,
    StP3SeedArm,
    StP3SeedCommit,
    StP3Disarm,
    StP3BlockedArm,
    StP3BlockedCommit,
    StP3Check,
    StP4Seed0Arm,
    StP4Seed0Commit,
    StP4Seed1Arm,
    StP4Seed1Commit,
    StP4Seed2Arm,
    StP4Seed2Commit,
    StP4Seed3Arm,
    StP4Seed3Commit,
    StP4PreviewOnlyArm,
    StP4PreviewOnlyCheck,
    StP4CommitOnly,
    StP4CommitOnlyCheck,
    StP4DataArm,
    StP4DataCommit,
    StP4DataCheck,
    StP4DelayArm,
    StP4DelayWait,
    StP4DelayCommit,
    StP4DelayCheck,
    StP4AddrArm,
    StP4AddrCommit,
    StP4AddrCheck,
    StP4HoldArm,
    StP4HoldCommit,
    StP4HoldCheck,
    StP4MultiArmA,
    StP4MultiArmB,
    StP4MultiCommit,
    StP4MultiCheck,
    StP4FinalCheck,
    StD0Write,
    StD0Check,
    StD1Write,
    StD1Check,
    StD2Check,
    StD3Arm,
    StD3Commit,
    StD3Check,
    StF0Arm,
    StF0Commit,
    StF0Check,
    StF1Check,
    StF2Arm,
    StF2Commit,
    StF2Check,
    StF3Arm,
    StF3Commit,
    StF3Check,
    StF4LateArm,
    StF4LateCommit,
    StF4LateRead0,
    StF4LateCheck0,
    StF5LateRead1,
    StF5LateCheck1,
    StR0Write,
    StR0ReadArm,
    StR0Check,
    StR1Write,
    StR1ReadArm,
    StR1Check,
    StR2Arm,
    StR2Commit,
    StR2ReadArm,
    StR2Check,
    StC0Arm,
    StC0Commit,
    StC0Check,
    StC1Write,
    StC1Check,
    StC2Arm,
    StC2Commit,
    StC2Check,
    StA0Arm,
    StA0Commit,
    StA0Check,
    StA1Write,
    StA1Check,
    StA2Arm,
    StA2Commit,
    StA2Check,
    StT0Arm,
    StT0Commit,
    StT0Check,
    StT1Write,
    StT1Check,
    StT2Arm,
    StT2Commit,
    StT2Check,
    StT3Arm,
    StT3Commit,
    StT3Check,
    StV0Arm,
    StV0Commit,
    StV0Check,
    StV1Write,
    StV1Check,
    StV2Arm,
    StV2Commit,
    StV2Check,
    StQ0Arm,
    StQ0Commit,
    StQ0Check,
    StQ1Arm,
    StQ1Commit,
    StQ1Check,
    StQ2Write,
    StQ2Check,
    StS0Arm,
    StS0Commit,
    StS0Check,
    StS1Write,
    StS1Check,
    StS2Arm,
    StS2Commit,
    StS2Check,
    StW0Arm,
    StW0Stage,
    StW0Check,
    StW1Check,
    StW2Arm,
    StW2Stage,
    StW2Check,
    StY0SeedArm,
    StY0SeedCommit,
    StY0SeedReadArm,
    StY0SeedCheck,
    StY1BlockArm,
    StY1BlockCommit,
    StY1BlockReadArm,
    StY1BlockCheck,
    StY2RearmArm,
    StY2RearmCommit,
    StY2RearmReadArm,
    StY2RearmCheck,
    StM0Arm,
    StM0Commit,
    StM0Check,
    StM1Arm,
    StM1Commit,
    StM1Bubble,
    StM1Check,
    StL0SentinelArm,
    StL0SentinelCommit,
    StL0FillArm,
    StL0FillCommit,
    StL0ReadArm,
    StL0Check,
    StL1StoreArm,
    StL1StoreCommit,
    StL1Bubble,
    StL1ReadArm,
    StL1Check,
    StL2ReadArm,
    StL2Check,
    StB0BaseArm,
    StB0BaseCommit,
    StB1BaseArm,
    StB1BaseCommit,
    StB0NewArm,
    StB0RwSame,
    StB0Check,
    StB1NewArm,
    StB1RwDiff,
    StB1Check,
    StB2ReadArm,
    StB2HoldArm,
    StB2HoldCommit,
    StB2Check,
    StB3LowArm,
    StB3LowRwSame,
    StB3Check,
    StB4HighArm,
    StB4HighRwSame,
    StB4Check,
    StX0FillArm,
    StX0FillCommit,
    StX0DbgReadArm,
    StX0DbgReadCheck,
    StX1CoreReadArm,
    StX1CoreReadCheck,
    StZ0MetaArrayArm,
    StZ0MetaArrayCommit,
    StZ0MetaArrayCheck,
    StZ1DataAllArm,
    StZ1DataAllCommit,
    StZ1DataAllReadArm,
    StZ1DataAllCheck,
    StZ2DataLowArm,
    StZ2DataLowCommit,
    StZ2DataLowReadArm,
    StZ2DataLowCheck,
    StZ3DataHighArm,
    StZ3DataHighCommit,
    StZ3DataHighReadArm,
    StZ3DataHighCheck,
    StZ4DataIsoArm,
    StZ4DataIsoCommit,
    StZ4DataIsoReadOrigArm,
    StZ4DataIsoReadOrigCheck,
    StZ4DataIsoReadNewArm,
    StZ4DataIsoReadNewCheck,
    StZ5HitLaunch,
    StZ5HitCheck,
    StZ6HoldArm,
    StZ6HoldBlock,
    StZ6HoldCheck,
    StZ6ResumeArm,
    StZ6ResumeCheck,
    StZ7TimingLaunch,
    StZ7TimingS2Check,
    StZ7TimingS3Check,
    StZ8ArrayHoldLaunch,
    StZ8ArrayHoldDrop0,
    StZ8ArrayHoldCheck0,
    StZ8ArrayResumeLaunch,
    StZ8ArrayResumeCheck,
    StZ8ArrayHoldDrop1,
    StZ8ArrayHoldCheck1,
    StZ9MissLaunch,
    StZ9MissS2Check,
    StZ9MissS3Check,
    StZ10BtbLaunchOrig,
    StZ10BtbSwitchIso,
    StZ10BtbCheckIso,
    StZ11SweepWriteArm,
    StZ11SweepWriteCommit,
    StZ11SweepReadArm,
    StZ11SweepReadCheck,
    StZ11SweepDone,
    StZ12PipeS0Launch,
    StZ12PipeS1Capture,
    StZ12PipeS2Check,
    StZ13QualLaunch,
    StZ13QualS1Capture,
    StZ13QualS2Check,
    StZ13BypassLaunch,
    StZ13BypassS1Capture,
    StZ13BypassS2Check,
    StZ14StressDecoyArm,
    StZ14StressDecoyCommit,
    StZ14StressLaunch,
    StZ14StressS1Capture,
    StZ14StressS2Check,
    StZ14StressS3Check,
    StPrint,
    StDone
  } state_e;

  typedef enum logic [6:0] {
    MsgBoot,
    MsgP0,
    MsgP1,
    MsgP2,
    MsgP3,
    MsgP4,
    MsgD0,
    MsgD1,
    MsgD2,
    MsgD3,
    MsgF0,
    MsgF1,
    MsgF2,
    MsgF3,
    MsgF4,
    MsgF5,
    MsgR0,
    MsgR1,
    MsgR2,
    MsgC0,
    MsgC1,
    MsgC2,
    MsgA0,
    MsgA1,
    MsgA2,
    MsgT0,
    MsgT1,
    MsgT2,
    MsgT3,
    MsgV0,
    MsgV1,
    MsgV2,
    MsgQ0,
    MsgQ1,
    MsgQ2,
    MsgS0,
    MsgS1,
    MsgS2,
    MsgW0,
    MsgW1,
    MsgW2,
    MsgY0,
    MsgY1,
    MsgY2,
    MsgM0,
    MsgM1,
    MsgL0,
    MsgL1,
    MsgL2,
    MsgB0,
    MsgB1,
    MsgB2,
    MsgB3,
    MsgB4,
    MsgX0,
    MsgX1,
    MsgZ0,
    MsgZ1,
    MsgZ2,
    MsgZ3,
    MsgZ4,
    MsgZ5,
    MsgZ6,
    MsgZ7,
    MsgZ8,
    MsgZ9,
    MsgZ10,
    MsgZ11,
    MsgZ12,
    MsgZ13,
    MsgZ14,
    MsgStatus
  } msg_kind_e;

  localparam logic [7:0]  PreviewSeed    = 8'h12;
  localparam logic [7:0]  PreviewNoise   = 8'h56;
  localparam logic [7:0]  PreviewSplit   = 8'h9A;
  localparam logic [7:0]  PreviewBlocked = 8'hE7;
  localparam logic [7:0]  PreviewProtocolExpected = 8'hFF;
  localparam logic [15:0] DiffSeed       = 16'h3412;
  localparam logic [15:0] DiffLowOnly    = 16'hABCD;
  localparam logic [15:0] DiffHighOnly   = 16'hBB00;
  localparam logic [31:0] FrontendWord0  = 32'h0000_0013;
  localparam logic [31:0] FrontendWord1  = 32'h1122_3344;
  localparam logic [31:0] FrontendWord2  = 32'h5566_7788;
  localparam logic [31:0] FrontendWord3  = 32'h99AA_BBCC;
  localparam logic [31:0] FrontendWord4  = 32'hDDEE_F001;
  localparam logic [31:0] FrontendWord5  = 32'h2345_6789;
  localparam logic [31:0] FrontendWord6  = 32'hABCD_EF02;
  localparam logic [31:0] FrontendWord7  = 32'h7788_99AA;
  localparam logic [255:0] FrontendLine0 = {
    FrontendWord7,
    FrontendWord6,
    FrontendWord5,
    FrontendWord4,
    FrontendWord3,
    FrontendWord2,
    FrontendWord1,
    FrontendWord0
  };
  localparam logic [31:0] FrontendLine1Word0 = 32'hCAFE_C001;
  localparam logic [255:0] FrontendLine1 = {
    32'h1111_2222,
    32'h3333_4444,
    32'h5555_6666,
    32'h7777_8888,
    32'h9999_AAAA,
    32'hBBBB_CCCC,
    32'hDDDD_EEEE,
    FrontendLine1Word0
  };
  localparam logic [31:0] FrontendOddReadExpected = {
    FrontendWord1[15:0],
    FrontendWord0[31:16]
  };
  localparam logic [31:0] FrontendWrapReadExpected = {
    FrontendLine1Word0[15:0],
    FrontendWord7[31:16]
  };
  localparam logic [31:0] FrontendPartialWord = 32'hA5A5_F00D;
  localparam logic [31:0] FrontendLateNoBypassExpected = 32'h0000_0000;
  localparam logic [15:0] RegSeed        = 16'h2003;
  localparam logic [15:0] RegNoise       = 16'hAAAA;
  localparam logic [15:0] RegSplit       = 16'h55AA;
  localparam logic [15:0] DecSeed        = 16'h5102;
  localparam logic [15:0] DecRearm       = 16'hA55A;
  localparam logic [31:0] ParSeedPacked  = 32'h0000_0012;
  localparam logic [31:0] ParStalePacked = 32'h0000_1212;
  localparam logic [31:0] ParRearmPacked = 32'h9A00_1212;
  localparam logic [15:0] TwoSeed        = 16'h1234;
  localparam logic [15:0] TwoRearm       = 16'h9A9A;
  localparam logic [15:0] TriSeedA       = 16'h1111;
  localparam logic [15:0] TriSeedB       = 16'h2222;
  localparam logic [15:0] TriRearmA      = 16'h7777;
  localparam logic [7:0]  QueuePushSeed  = 8'h21;
  localparam logic [7:0]  QueueRearmSeed = 8'hAB;
  localparam logic [31:0] QueuePacked0   = 32'h0000_0021;
  localparam logic [31:0] QueuePacked1   = 32'h00AB_0021;
  localparam logic [31:0] QueuePacked2   = 32'h00AB_AB21;
  localparam logic [15:0] SingleSeed     = 16'h2468;
  localparam logic [15:0] SingleRearm    = 16'hBEEF;
  localparam logic [15:0] ChainSeed      = 16'h1357;
  localparam logic [15:0] ChainNoise     = 16'hAAAA;
  localparam logic [15:0] ChainRearm     = 16'hC0DE;
  localparam logic [15:0] DualDiagExpected = 16'h03FF;
  localparam logic [7:0]  DualPreviewSeed  = 8'hA1;
  localparam logic [7:0]  DualPreviewBlock = 8'hB2;
  localparam logic [7:0]  DualPreviewRearm = 8'hC3;
  localparam logic [15:0] DualDiffSeed     = 16'h34A1;
  localparam logic [15:0] DualDiffBlock    = 16'h56B2;
  localparam logic [15:0] DualDiffRearm    = 16'h78C3;
  localparam logic [31:0] DualFrontSeedWord  = 32'h0010_0093;
  localparam logic [31:0] DualFrontBlockWord = 32'h0020_00B7;
  localparam logic [31:0] DualFrontRearmWord = 32'h0030_0193;
  localparam logic [255:0] DualFrontSeedLine = {
    32'h1010_0007,
    32'h1010_0006,
    32'h1010_0005,
    32'h1010_0004,
    32'h1010_0003,
    32'h1010_0002,
    32'h1010_0001,
    DualFrontSeedWord
  };
  localparam logic [255:0] DualFrontBlockLine = {
    32'h2020_0007,
    32'h2020_0006,
    32'h2020_0005,
    32'h2020_0004,
    32'h2020_0003,
    32'h2020_0002,
    32'h2020_0001,
    DualFrontBlockWord
  };
  localparam logic [255:0] DualFrontRearmLine = {
    32'h3030_0007,
    32'h3030_0006,
    32'h3030_0005,
    32'h3030_0004,
    32'h3030_0003,
    32'h3030_0002,
    32'h3030_0001,
    DualFrontRearmWord
  };
  localparam logic [15:0] DualRegSeed     = 16'h7101;
  localparam logic [15:0] DualRegBlock    = 16'h7202;
  localparam logic [15:0] DualRegRearm    = 16'h7303;
  localparam logic [15:0] DualDecSeed     = 16'hD101;
  localparam logic [15:0] DualDecBlock    = 16'hD202;
  localparam logic [15:0] DualDecRearm    = 16'hD303;
  localparam logic [7:0]  DualParSeed     = 8'h5A;
  localparam logic [7:0]  DualParBlock    = 8'h6B;
  localparam logic [7:0]  DualParRearm    = 8'h7C;
  localparam logic [15:0] DualTwoSeed     = 16'h6A5C;
  localparam logic [15:0] DualTwoBlock    = 16'h7B6D;
  localparam logic [15:0] DualTwoRearm    = 16'h8C7E;
  localparam logic [15:0] DualTriSeedA    = 16'h1117;
  localparam logic [15:0] DualTriSeedB    = 16'h2228;
  localparam logic [15:0] DualTriBlockA   = 16'h3339;
  localparam logic [15:0] DualTriBlockB   = 16'h444A;
  localparam logic [15:0] DualTriRearmA   = 16'h555B;
  localparam logic [15:0] DualTriRearmB   = 16'h666C;
  localparam logic [15:0] DualSingleSeed  = 16'h1357;
  localparam logic [15:0] DualSingleBlock = 16'h2468;
  localparam logic [15:0] DualSingleRearm = 16'h369A;
  localparam logic [31:0] CacheMetaFill  = 32'h40D0_2001;
  localparam logic [31:0] CacheMetaStore = 32'h80A0_0002;
  localparam logic [31:0] CacheMetaNoise = 32'hDEAD_BEEF;
  localparam logic [6:0]  CacheLineSeedAddr = 7'h00;
  localparam logic [6:0]  CacheLineTestAddr = 7'h40;
  localparam logic [6:0]  CacheLineCollideAddr = 7'h41;
  localparam logic [6:0]  CacheLineOtherAddr = 7'h42;
  localparam logic [63:0] CacheLineSeed  = 64'h1111_2222_3333_4444;
  localparam logic [63:0] CacheLineFill  = 64'h0123_4567_89AB_CDEF;
  localparam logic [63:0] CacheLineStore = 64'hFEDC_BA98_7654_3210;
  localparam logic [63:0] CacheLineNoise = 64'hCAFE_F00D_DEAD_BEEF;
  localparam logic [63:0] CacheLineCollideOld = 64'h0DDC_0FFE_E0DD_F00D;
  localparam logic [63:0] CacheLineOtherOld = 64'h1357_9BDF_2468_ACE0;
  localparam logic [63:0] CacheLineRwSame = 64'h2222_3333_4444_5555;
  localparam logic [63:0] CacheLineRwDiff = 64'h6666_7777_8888_9999;
  localparam logic [63:0] CacheLineHoldWrite = 64'hAAAA_BBBB_CCCC_DDDD;
  localparam logic [63:0] CacheLineLowOnly = 64'hDEAD_BEEF_0102_0304;
  localparam logic [63:0] CacheLineHighOnly = 64'h5566_7788_CAFE_BABE;
  localparam logic [63:0] CacheLineLowCollision =
      {CacheLineHoldWrite[63:32], CacheLineLowOnly[31:0]};
  localparam logic [63:0] CacheLineHighCollision =
      {CacheLineHighOnly[63:32], CacheLineLowOnly[31:0]};
  localparam logic [minion_dcache_pkg::PaSize-1:0] DcacheLookupAddr = 40'h0000_0012_00;
  localparam logic [minion_dcache_pkg::PaTagExtSize-1:0] DcacheLookupTag =
      DcacheLookupAddr[minion_dcache_pkg::PaSize-1 -: minion_dcache_pkg::PaTagExtSize];
  localparam logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] DcacheLookupSet =
      DcacheLookupAddr[minion_dcache_pkg::DcacheLramAddrWidth-1:6];
  localparam logic [7:0]  DcacheMetaArrayDiagExpected = 8'h3F;
  localparam logic [7:0]  DcacheDataAllDiagExpected = 8'h0F;
  localparam logic [7:0]  DcacheDataHalfDiagExpected = 8'h03;
  localparam logic [7:0]  DcacheDataIsoDiagExpected = 8'h03;
  localparam logic [7:0]  DcacheLeafHitDiagExpected = 8'hFF;
  localparam logic [7:0]  CacheLramReadGateDiagExpected = 8'h03;
  localparam logic [7:0]  DcacheTimingDiagExpected = 8'hFF;
  localparam logic [7:0]  DcacheArrayHoldDiagExpected = 8'h0F;
  localparam logic [7:0]  DcacheMissSuppressDiagExpected = 8'h3F;
  localparam logic [7:0]  DcacheBackToBackDiagExpected = 8'h03;
  localparam logic [7:0]  DcacheSweepDiagExpected = 8'hFF;
  localparam logic [7:0]  DcachePipelineDiagExpected = 8'hFF;
  localparam logic [7:0]  DcacheQualDiagExpected = 8'hFF;
  localparam logic [7:0]  DcacheStressDiagExpected = 8'hFF;
  localparam logic [minion_dcache_pkg::DcacheWayIdxWidth-1:0] DcacheArrayWay = 2'd1;
  localparam logic [minion_dcache_pkg::DcacheWayIdxWidth-1:0] DcacheArrayIsoWay = 2'd2;
  localparam logic [minion_dcache_pkg::DcacheLramAddrWidth-1:0] DcacheArrayAddr = 10'h200;
  localparam logic [minion_dcache_pkg::DcacheLramAddrWidth-1:0] DcacheArrayIsoAddr = 10'h240;
  localparam logic [minion_dcache_pkg::PaTagExtSize-1:0] DcacheDecoyTag =
      DcacheLookupTag ^ {{(minion_dcache_pkg::PaTagExtSize-1){1'b0}}, 1'b1};
  localparam int unsigned DcacheArrayBank = 0;
  localparam logic [63:0] DcacheBank0Line = 64'h1020_3040_5060_7080;
  localparam logic [63:0] DcacheBank1Line = 64'h1122_3344_5566_7788;
  localparam logic [63:0] DcacheBank2Line = 64'h99AA_BBCC_DDEE_F001;
  localparam logic [63:0] DcacheBank3Line = 64'h0BAD_CAFE_1234_5678;
  localparam logic [63:0] DcacheLowHalfLine = 64'hAAAA_BBBB_CCCC_DDDD;
  localparam logic [63:0] DcacheHighHalfLine = 64'hEEEE_FFFF_1111_2222;
  localparam logic [63:0] DcacheHighHalfExpected = {DcacheHighHalfLine[63:32],
                                                    DcacheLowHalfLine[31:0]};
  localparam logic [63:0] DcacheIsoLine = 64'h1357_9BDF_2468_ACE0;

  state_e      state_q;
  msg_kind_e   msg_kind_q;
  logic [5:0]  msg_idx_q;
  logic [7:0]  error_count_q;

  logic        preview_wr_next;
  logic        preview_wr_en;
  logic [1:0]  preview_wr_addr;
  logic [7:0]  preview_wr_data;
  logic [1:0]  preview_rd_addr;
  logic [7:0]  preview_rd_data;

  logic        preview_contract_hold_hi;
  logic        preview_contract_clk;
  logic        preview_contract_wr_next;
  logic        preview_contract_wr_en;
  logic [1:0]  preview_contract_wr_addr;
  logic [7:0]  preview_contract_wr_data;
  logic [1:0]  preview_contract_rd_addr;
  logic [7:0]  preview_contract_rd_data;

  logic [1:0]  diff_wr_next;
  logic [1:0]  diff_wr_en;
  logic        diff_wr_addr;
  logic [15:0] diff_wr_data;
  logic [1:0]  diff_rd_addr;
  logic [7:0]  diff_rd_data;

  logic [7:0]   front_wr_next;
  logic [7:0]   front_wr_en;
  logic         front_wr_addr;
  logic [255:0] front_wr_data;
  logic [4:0]   front_rd_addr;
  logic [31:0]  front_rd_data;

  logic        reg_wr_next;
  logic [15:0] reg_wr_data;
  logic [2:0]  reg_wr_addr;
  logic        reg_wr_en;
  logic [2:0]  reg_rd_addr;
  logic        reg_rd_en;
  logic [15:0] reg_rd_data;

  logic        dec_wr_next;
  logic        dec_wr_en;
  logic [7:0]  dec_rd_addr;
  logic [7:0]  dec_wr_addr;
  logic [15:0] dec_wr_data;
  logic [15:0] dec_rd_data;

  logic        par_wr_next;
  logic        par_wr_en;
  logic [1:0]  par_wr_addr;
  logic [7:0]  par_wr_data;
  logic [31:0] par_rd_data;

  logic        two_wr_next;
  logic        two_wr_en;
  logic [2:0]  two_rd_addr_a;
  logic [2:0]  two_rd_addr_b;
  logic [2:0]  two_wr_addr;
  logic [15:0] two_wr_data;
  logic [15:0] two_rd_data_a;
  logic [15:0] two_rd_data_b;
  logic [15:0] two_rd_par;

  logic        tri_wr_next_a;
  logic        tri_wr_next_b;
  logic        tri_wr_en_a;
  logic        tri_wr_en_b;
  logic [2:0]  tri_rd_addr_a;
  logic [2:0]  tri_rd_addr_b;
  logic [2:0]  tri_rd_addr_c;
  logic [2:0]  tri_wr_addr_a;
  logic [2:0]  tri_wr_addr_b;
  logic [15:0] tri_wr_data_a;
  logic [15:0] tri_wr_data_b;
  logic [15:0] tri_rd_data_a;
  logic [15:0] tri_rd_data_b;
  logic [15:0] tri_rd_data_c;

  logic        rq_wr_next;
  logic        rq_wr_en;
  logic [1:0]  rq_addr;
  logic [7:0]  rq_data;
  logic [31:0] rq_rd_data;

  logic        single_wr_next;
  logic        single_wr_en;
  logic [15:0] single_wr_data;
  logic [15:0] single_rd_data;

  logic        chain_mem_wen;
  logic        chain_mem_wen_p2;
  logic [15:0] chain_wdata;
  logic [15:0] chain_wdata_1p;

  logic        dual_hold_hi;
  logic        dual_preview_clk;
  logic        gated_preview_en;
  logic        gated_preview_clk;
  logic        dual_p_wr_next;
  logic        dual_p_wr_en;
  logic [1:0]  dual_p_wr_addr;
  logic [7:0]  dual_p_wr_data;
  logic [1:0]  dual_p_rd_addr;
  logic [7:0]  dual_p_rd_data;
  logic [1:0]  dual_diff_wr_next;
  logic [1:0]  dual_diff_wr_en;
  logic        dual_diff_wr_addr;
  logic [15:0] dual_diff_wr_data;
  logic [1:0]  dual_diff_rd_addr;
  logic [7:0]  dual_diff_rd_data;
  logic [7:0]   dual_front_wr_next;
  logic [7:0]   dual_front_wr_en;
  logic         dual_front_wr_addr;
  logic [255:0] dual_front_wr_data;
  logic [4:0]   dual_front_rd_addr;
  logic [31:0]  dual_front_rd_data;
  logic [7:0]   gated_front_wr_next;
  logic [7:0]   gated_front_wr_en;
  logic         gated_front_wr_addr;
  logic [255:0] gated_front_wr_data;
  logic [4:0]   gated_front_rd_addr;
  logic [31:0]  gated_front_rd_data;
  logic        dual_reg_wr_next;
  logic [15:0] dual_reg_wr_data;
  logic [2:0]  dual_reg_wr_addr;
  logic        dual_reg_wr_en;
  logic [2:0]  dual_reg_rd_addr;
  logic        dual_reg_rd_en;
  logic [15:0] dual_reg_rd_data;
  logic        dual_dec_wr_next;
  logic        dual_dec_wr_en;
  logic [7:0]  dual_dec_rd_addr;
  logic [7:0]  dual_dec_wr_addr;
  logic [15:0] dual_dec_wr_data;
  logic [15:0] dual_dec_rd_data;
  logic        dual_par_wr_next;
  logic        dual_par_wr_en;
  logic [1:0]  dual_par_wr_addr;
  logic [7:0]  dual_par_wr_data;
  /* verilator lint_off UNUSEDSIGNAL */  // Dual-clock par-RF diagnostic checks initialized entry 2 only.
  logic [31:0] dual_par_rd_data;
  /* verilator lint_on UNUSEDSIGNAL */
  logic        dual_two_wr_next;
  logic        dual_two_wr_en;
  logic [2:0]  dual_two_rd_addr_a;
  logic [2:0]  dual_two_rd_addr_b;
  logic [2:0]  dual_two_wr_addr;
  logic [15:0] dual_two_wr_data;
  logic [15:0] dual_two_rd_data_a;
  logic [15:0] dual_two_rd_data_b;
  logic [15:0] dual_two_rd_par;
  logic        dual_tri_wr_next_a;
  logic        dual_tri_wr_next_b;
  logic        dual_tri_wr_en_a;
  logic        dual_tri_wr_en_b;
  logic [2:0]  dual_tri_rd_addr_a;
  logic [2:0]  dual_tri_rd_addr_b;
  logic [2:0]  dual_tri_rd_addr_c;
  logic [2:0]  dual_tri_wr_addr_a;
  logic [2:0]  dual_tri_wr_addr_b;
  logic [15:0] dual_tri_wr_data_a;
  logic [15:0] dual_tri_wr_data_b;
  logic [15:0] dual_tri_rd_data_a;
  logic [15:0] dual_tri_rd_data_b;
  logic [15:0] dual_tri_rd_data_c;
  logic        dual_single_wr_next;
  logic        dual_single_wr_en;
  logic [15:0] dual_single_wr_data;
  logic [15:0] dual_single_rd_data;
  logic [15:0] dual_seed_diag;
  logic [15:0] dual_block_diag;
  logic [15:0] dual_rearm_diag;

  logic        cache_meta_wr_next;
  logic        cache_meta_wr_en;
  logic [2:0]  cache_meta_wr_addr;
  logic [31:0] cache_meta_wr_data;
  logic [2:0]  cache_meta_rd_addr;
  logic [31:0] cache_meta_rd_data;

  logic        cache_lram_wr_next;
  logic        cache_lram_wr_en;
  logic [1:0]  cache_lram_wr_next_mask;
  logic [1:0]  cache_lram_wr_en_mask;
  logic [6:0]  cache_lram_wr_addr;
  logic [63:0] cache_lram_wr_data;
  logic        cache_lram_rd_en;
  logic [6:0]  cache_lram_rd_addr;
  logic [63:0] cache_lram_rd_data;

  logic                                      dcache_arr_read_valid;
  minion_dcache_pkg::dcache_da_read_req_t   dcache_arr_read_req [minion_dcache_pkg::DcacheLramNumBanks];
  minion_dcache_pkg::dcache_da_data_t       dcache_arr_read_resp;
  logic                                      dcache_arr_s3_write_valid;
  logic                                      dcache_arr_s4_write_valid;
  minion_dcache_pkg::dcache_da_write_req_t  dcache_arr_write_req [minion_dcache_pkg::DcacheLramNumBanks];
  logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] dcache_md_read_set;
  logic                                      dcache_md_write_valid;
  logic                                      dcache_md_write_valid_prev;
  minion_dcache_pkg::dcache_meta_write_req_t dcache_md_write_req;
  logic [minion_dcache_pkg::DcacheWays-1:0]  dcache_md_read_valid;
  minion_dcache_pkg::dcache_meta_data_t      dcache_md_read_resp [minion_dcache_pkg::DcacheWays];
  logic                                      dcache_md_clear_all;
  logic                                      dcache_md_clear_low;
  logic [7:0]                                dcache_md_array_diag;
  logic [7:0]                                dcache_md_array_diag_q;
  logic [7:0]                                dcache_data_all_diag;
  logic [7:0]                                dcache_data_low_diag;
  logic [7:0]                                dcache_data_high_diag;
  logic [7:0]                                dcache_leaf_hit_diag;
  logic [7:0]                                dcache_data_all_diag_q;
  logic [7:0]                                dcache_data_low_diag_q;
  logic [7:0]                                dcache_data_high_diag_q;
  logic [7:0]                                dcache_data_iso_diag_q;
  logic [7:0]                                dcache_leaf_hit_diag_q;
  logic [7:0]                                cache_lram_read_gate_diag_q;
  logic [7:0]                                dcache_timing_diag_q;
  logic [7:0]                                dcache_array_hold_diag_q;
  logic [7:0]                                dcache_miss_suppress_diag_q;
  logic [7:0]                                dcache_back_to_back_diag_q;
  logic [7:0]                                dcache_sweep_diag_q;
  logic [7:0]                                dcache_pipeline_diag_q;
  logic [7:0]                                dcache_qual_diag_q;
  logic [7:0]                                dcache_stress_diag_q;
  logic [7:0]                                dcache_sweep_final_diag;
  logic [7:0]                                dcache_pipeline_final_diag;
  logic [7:0]                                dcache_stress_final_diag;
  logic [6:0]                                dcache_qual_hit_diag;
  logic                                      dcache_qual_bypass_suppressed;
  logic                                      dcache_sweep_read_ok;
  logic [2:0]                                dcache_sweep_slot_q;
  logic [3:0]                                dcache_sweep_seen_bank_q;
  logic [3:0]                                dcache_sweep_seen_way_q;
  logic [3:0]                                dcache_sweep_seen_set_q;
  logic [1:0]                                dcache_sweep_seen_idx_q;
  logic                                      dcache_sweep_fail_q;
  logic                                      dcache_leaf_valid;
  logic                                      dcache_leaf_tag_match;
  logic                                      dcache_leaf_coh_ok;
  logic                                      dcache_leaf_meta_hit;
  logic                                      dcache_leaf_data0_ok;
  logic                                      dcache_leaf_data1_ok;
  logic                                      dcache_leaf_iso_clear;
  logic                                      dcache_timing_s1_en;
  logic                                      dcache_timing_s2_valid_q;
  logic                                      dcache_timing_s2_tag_match_q;
  logic                                      dcache_timing_s2_state_hit_q;
  logic                                      dcache_timing_s2_hit;
  logic                                      dcache_timing_s3_valid_q;
  logic [minion_dcache_pkg::DcacheWays-1:0]  dcache_timing_s3_way_q;
  logic [63:0]                               dcache_timing_s3_data_q;
  logic [7:0]                                dcache_timing_final_diag;
  logic                                      dcache_miss_s1_en;
  logic                                      dcache_miss_s2_valid_q;
  logic                                      dcache_miss_s2_hit_q;
  logic                                      dcache_miss_s3_valid_q;
  logic [63:0]                               dcache_miss_s3_data_q;
  logic                                      dcache_pipeline_s0_valid;
  logic                                      dcache_pipeline_s1_valid_q;
  logic                                      dcache_pipeline_s1_valid_seen_q;
  logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] dcache_pipeline_s1_set_q;
  logic [minion_dcache_pkg::PaTagExtSize-1:0]      dcache_pipeline_s1_tag_q;
  logic                                      dcache_pipeline_s1_clk_en_q;
  logic                                      dcache_pipeline_s1_way_valid;
  logic                                      dcache_pipeline_s1_way_valid_q;
  logic                                      dcache_pipeline_s1_tag_match;
  logic                                      dcache_pipeline_s1_tag_match_q;
  logic                                      dcache_pipeline_s1_state_hit;
  logic                                      dcache_pipeline_s1_state_hit_q;
  logic                                      dcache_pipeline_s2_valid_q;
  logic [minion_dcache_pkg::DcacheWays-1:0]  dcache_pipeline_s2_tag_match_q;
  logic                                      dcache_pipeline_s2_state_hit_q;
  logic                                      dcache_pipeline_s2_hit;
  logic                                      dcache_qual_s0_valid;
  logic                                      dcache_qual_s0_is_bypass;
  logic                                      dcache_qual_s1_valid_q;
  logic                                      dcache_qual_s1_valid_seen_q;
  logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] dcache_qual_s1_set_q;
  logic [minion_dcache_pkg::PaTagExtSize-1:0]      dcache_qual_s1_tag_q;
  logic                                      dcache_qual_s1_clk_en_q;
  logic                                      dcache_qual_s1_is_msg_q;
  logic                                      dcache_qual_s1_is_amo_q;
  logic                                      dcache_qual_s1_is_bypass_q;
  logic                                      dcache_qual_s1_bypass_dcache_q;
  logic                                      dcache_qual_s1_way_valid;
  logic                                      dcache_qual_s1_tag_match;
  logic                                      dcache_qual_s1_tag_match_q;
  logic                                      dcache_qual_s2_valid_q;
  logic [minion_dcache_pkg::DcacheWays-1:0]  dcache_qual_s2_tag_match_q;
  logic [minion_dcache_pkg::DcacheWays-1:0]  dcache_qual_s2_tag_match_qual;
  logic                                      dcache_qual_s2_is_msg_q;
  logic                                      dcache_qual_s2_is_amo_q;
  logic                                      dcache_qual_s2_is_bypass_q;
  logic                                      dcache_qual_s2_bypass_dcache_q;
  minion_dcache_pkg::dcache_line_state_e     dcache_qual_s2_way_hit_state_q;
  logic                                      dcache_qual_s2_state_hit;
  logic                                      dcache_qual_s2_hit;
  logic                                      dcache_stress_s0_valid;
  logic                                      dcache_stress_s1_valid_q;
  logic                                      dcache_stress_s1_valid_seen_q;
  logic [minion_dcache_pkg::DcacheSetIdxWidth-1:0] dcache_stress_s1_set_q;
  logic [minion_dcache_pkg::PaTagExtSize-1:0]      dcache_stress_s1_tag_q;
  logic                                      dcache_stress_s1_clk_en_q;
  logic [minion_dcache_pkg::DcacheWays-1:0] dcache_stress_s1_way_valid;
  logic [minion_dcache_pkg::DcacheWays-1:0] dcache_stress_s1_way_valid_q;
  logic [minion_dcache_pkg::DcacheWays-1:0] dcache_stress_s1_tag_match;
  logic [minion_dcache_pkg::DcacheWays-1:0] dcache_stress_s1_tag_match_q;
  logic [minion_dcache_pkg::DcacheWays-1:0] dcache_stress_s1_state_hit_vec;
  logic [minion_dcache_pkg::DcacheLramNumBanks-1:0] dcache_stress_s1_data_ok;
  logic                                      dcache_stress_s2_valid_q;
  logic [minion_dcache_pkg::DcacheWays-1:0] dcache_stress_s2_tag_match_q;
  logic [minion_dcache_pkg::DcacheWays-1:0] dcache_stress_s2_tag_match_qual;
  logic                                      dcache_stress_s2_state_hit_q;
  logic [minion_dcache_pkg::DcacheLramNumBanks-1:0] dcache_stress_s2_data_ok_q;
  logic                                      dcache_stress_s2_hit;
  logic                                      dcache_stress_s3_valid_q;
  logic [63:0]                               dcache_stress_s3_data_q;

  logic [7:0]  preview_read0_q;
  logic [7:0]  preview_read1_q;
  logic [7:0]  preview_read2_q;
  logic [7:0]  preview_contract_read_q;
  logic [7:0]  preview_protocol_diag_q;
  logic [7:0]  diff_read0_q;
  logic [7:0]  diff_read1_q;
  logic [7:0]  diff_read2_q;
  logic [7:0]  diff_read3_q;
  logic [31:0] front_read0_q;
  logic [31:0] front_read1_q;
  logic [31:0] front_read2_q;
  logic [31:0] front_read3_q;
  logic [31:0] front_read4_q;
  logic [31:0] front_read5_q;
  logic [15:0] reg_read0_q;
  logic [15:0] reg_read1_q;
  logic [15:0] reg_read2_q;
  logic [15:0] dec_read0_q;
  logic [15:0] dec_read1_q;
  logic [15:0] dec_read2_q;
  logic [31:0] par_read0_q;
  logic [31:0] par_read1_q;
  logic [31:0] par_read2_q;
  logic [15:0] two_read0_q;
  logic [15:0] two_read1_q;
  logic [15:0] two_read2_q;
  logic [15:0] two_read3_q;
  logic [15:0] tri_read0_q;
  logic [15:0] tri_read1_q;
  logic [15:0] tri_read2_q;
  logic [31:0] rq_read0_q;
  logic [31:0] rq_read1_q;
  logic [31:0] rq_read2_q;
  logic [15:0] single_read0_q;
  logic [15:0] single_read1_q;
  logic [15:0] single_read2_q;
  logic [15:0] chain_read0_q;
  logic [15:0] chain_read1_q;
  logic [15:0] chain_read2_q;
  logic [15:0] dual_seed_diag_q;
  logic [15:0] dual_block_diag_q;
  logic [15:0] dual_rearm_diag_q;
  logic [31:0] cache_meta_read0_q;
  logic [31:0] cache_meta_read1_q;
  logic [63:0] cache_lram_read0_q;
  logic [63:0] cache_lram_read1_q;
  logic [63:0] cache_lram_read2_q;
  logic [63:0] cache_lram_rw_same_q;
  logic [63:0] cache_lram_rw_diff_q;
  logic [63:0] cache_lram_hold_write_q;
  logic [63:0] cache_lram_low_collision_q;
  logic [63:0] cache_lram_high_collision_q;
  logic [63:0] dcache_arr_dbg_read_q;
  logic [63:0] dcache_arr_core_read_q;

  logic [7:0]  tx_data;
  logic        tx_valid;
  logic        tx_ready;
  /* verilator lint_off UNUSEDSIGNAL */  // The harness does not currently surface the transmitter busy flag.
  logic        tx_busy;
  /* verilator lint_on UNUSEDSIGNAL */
  logic        tx_fire;

  function automatic logic [7:0] hex_ascii(input logic [3:0] nibble);
    unique case (nibble)
      4'h0:    hex_ascii = 8'h30;
      4'h1:    hex_ascii = 8'h31;
      4'h2:    hex_ascii = 8'h32;
      4'h3:    hex_ascii = 8'h33;
      4'h4:    hex_ascii = 8'h34;
      4'h5:    hex_ascii = 8'h35;
      4'h6:    hex_ascii = 8'h36;
      4'h7:    hex_ascii = 8'h37;
      4'h8:    hex_ascii = 8'h38;
      4'h9:    hex_ascii = 8'h39;
      4'hA:    hex_ascii = 8'h41;
      4'hB:    hex_ascii = 8'h42;
      4'hC:    hex_ascii = 8'h43;
      4'hD:    hex_ascii = 8'h44;
      4'hE:    hex_ascii = 8'h45;
      default: hex_ascii = 8'h46;
    endcase
  endfunction

  function automatic logic [7:0] hex8_digit(
    input logic [7:0] value,
    input int unsigned digit_idx
  );
    if (digit_idx == 0) begin
      hex8_digit = hex_ascii(value[7:4]);
    end else begin
      hex8_digit = hex_ascii(value[3:0]);
    end
  endfunction

  function automatic logic [7:0] hex16_digit(
    input logic [15:0] value,
    input int unsigned digit_idx
  );
    hex16_digit = hex_ascii(value[((3 - digit_idx) * 4) +: 4]);
  endfunction

  function automatic logic [7:0] hex32_digit(
    input logic [31:0] value,
    input int unsigned digit_idx
  );
    hex32_digit = hex_ascii(value[((7 - digit_idx) * 4) +: 4]);
  endfunction

  function automatic logic [7:0] hex64_digit(
    input logic [63:0] value,
    input int unsigned digit_idx
  );
    hex64_digit = hex_ascii(value[((15 - digit_idx) * 4) +: 4]);
  endfunction

  function automatic logic [63:0] dcache_bank_line(input int unsigned bank);
    unique case (bank)
      0:       dcache_bank_line = DcacheBank0Line;
      1:       dcache_bank_line = DcacheBank1Line;
      2:       dcache_bank_line = DcacheBank2Line;
      default: dcache_bank_line = DcacheBank3Line;
    endcase
  endfunction

  function automatic logic [minion_dcache_pkg::DcacheWayIdxWidth-1:0] dcache_sweep_way(
    input logic [2:0] slot
  );
    dcache_sweep_way = {slot[2] ^ slot[1], slot[0]};
  endfunction

  function automatic logic [minion_dcache_pkg::DcacheLramAddrWidth-1:0] dcache_sweep_addr(
    input logic [2:0] slot
  );
    dcache_sweep_addr = {{slot, ^slot}, (slot[2] ^ slot[0]), 5'b00000};
  endfunction

  function automatic logic [3:0] dcache_sweep_way_mask(input logic [2:0] slot);
    unique case ({slot[2] ^ slot[1], slot[0]})
      2'd0:    dcache_sweep_way_mask = 4'b0001;
      2'd1:    dcache_sweep_way_mask = 4'b0010;
      2'd2:    dcache_sweep_way_mask = 4'b0100;
      default: dcache_sweep_way_mask = 4'b1000;
    endcase
  endfunction

  function automatic logic [3:0] dcache_sweep_set_mask(input logic [1:0] set_idx);
    unique case (set_idx)
      2'd0:    dcache_sweep_set_mask = 4'b0001;
      2'd1:    dcache_sweep_set_mask = 4'b0010;
      2'd2:    dcache_sweep_set_mask = 4'b0100;
      default: dcache_sweep_set_mask = 4'b1000;
    endcase
  endfunction

  function automatic logic [1:0] dcache_sweep_idx_mask(input logic idx);
    if (idx) begin
      dcache_sweep_idx_mask = 2'b10;
    end else begin
      dcache_sweep_idx_mask = 2'b01;
    end
  endfunction

  function automatic logic [63:0] dcache_sweep_line(
    input logic [2:0] slot,
    input int unsigned bank
  );
    unique case (bank)
      0:       dcache_sweep_line = {16'hA100, 13'h0000, slot, 16'hB100, 13'h0000, slot};
      1:       dcache_sweep_line = {16'hA201, 13'h0000, slot, 16'hB201, 13'h0000, slot};
      2:       dcache_sweep_line = {16'hA302, 13'h0000, slot, 16'hB302, 13'h0000, slot};
      default: dcache_sweep_line = {16'hA403, 13'h0000, slot, 16'hB403, 13'h0000, slot};
    endcase
  endfunction

  function automatic logic [5:0] msg_last_idx(input msg_kind_e kind);
    unique case (kind)
      MsgBoot:   msg_last_idx = 6'd24;
      MsgD0,
      MsgD1,
      MsgD2,
      MsgD3:     msg_last_idx = 6'd15;
      MsgR0,
      MsgR1,
      MsgR2,
      MsgC0,
      MsgC1,
      MsgC2,
      MsgT0,
      MsgT1,
      MsgT2,
      MsgT3,
      MsgV0,
      MsgV1,
      MsgV2,
      MsgS0,
      MsgS1,
      MsgS2,
      MsgW0,
      MsgW1,
      MsgW2,
      MsgY0,
      MsgY1,
      MsgY2:     msg_last_idx = 6'd17;
      MsgA0,
      MsgA1,
      MsgA2,
      MsgF0,
      MsgF1,
      MsgF2,
      MsgF3,
      MsgF4,
      MsgF5,
      MsgM0,
      MsgM1,
      MsgQ0,
      MsgQ1,
      MsgQ2:     msg_last_idx = 6'd25;
      MsgL0,
      MsgL1,
      MsgL2,
      MsgB0,
      MsgB1,
      MsgB2,
      MsgB3,
      MsgB4,
      MsgX0,
      MsgX1:     msg_last_idx = 6'd41;
      MsgZ0,
      MsgZ1,
      MsgZ2,
      MsgZ3,
      MsgZ4,
      MsgZ5,
      MsgZ6,
      MsgZ7,
      MsgZ8,
      MsgZ9,
      MsgZ10,
      MsgZ11,
      MsgZ12,
      MsgZ13,
      MsgZ14:    msg_last_idx = 6'd13;
      MsgStatus: msg_last_idx = 6'd23;
      default:   msg_last_idx = 6'd13;
    endcase
  endfunction

  function automatic logic [7:0] boot_msg_byte(input logic [5:0] idx);
    unique case (idx)
      6'd0:    boot_msg_byte = 8'h41;
      6'd1:    boot_msg_byte = 8'h69;
      6'd2:    boot_msg_byte = 8'h6E;
      6'd3:    boot_msg_byte = 8'h65;
      6'd4:    boot_msg_byte = 8'h6B;
      6'd5:    boot_msg_byte = 8'h6B;
      6'd6:    boot_msg_byte = 8'h6F;
      6'd7:    boot_msg_byte = 8'h20;
      6'd8:    boot_msg_byte = 8'h74;
      6'd9:    boot_msg_byte = 8'h65;
      6'd10:   boot_msg_byte = 8'h73;
      6'd11:   boot_msg_byte = 8'h74;
      6'd12:   boot_msg_byte = 8'h5F;
      6'd13:   boot_msg_byte = 8'h72;
      6'd14:   boot_msg_byte = 8'h66;
      6'd15:   boot_msg_byte = 8'h5F;
      6'd16:   boot_msg_byte = 8'h70;
      6'd17:   boot_msg_byte = 8'h72;
      6'd18:   boot_msg_byte = 8'h65;
      6'd19:   boot_msg_byte = 8'h76;
      6'd20:   boot_msg_byte = 8'h69;
      6'd21:   boot_msg_byte = 8'h65;
      6'd22:   boot_msg_byte = 8'h77;
      6'd23:   boot_msg_byte = 8'h0D;
      default: boot_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] result8_msg_byte(
    input logic [7:0] prefix0,
    input logic [7:0] prefix1,
    input logic [7:0] write_value,
    input logic [7:0] got_value,
    input logic [5:0] idx
  );
    result8_msg_byte = 8'h20;
    unique case (idx)
      6'd0:    result8_msg_byte = prefix0;
      6'd1:    result8_msg_byte = prefix1;
      6'd2:    result8_msg_byte = 8'h20;
      6'd3:    result8_msg_byte = 8'h57;
      6'd4:    result8_msg_byte = 8'h3D;
      6'd5:    result8_msg_byte = hex8_digit(write_value, 0);
      6'd6:    result8_msg_byte = hex8_digit(write_value, 1);
      6'd7:    result8_msg_byte = 8'h20;
      6'd8:    result8_msg_byte = 8'h52;
      6'd9:    result8_msg_byte = 8'h3D;
      6'd10:   result8_msg_byte = hex8_digit(got_value, 0);
      6'd11:   result8_msg_byte = hex8_digit(got_value, 1);
      6'd12:   result8_msg_byte = 8'h0D;
      default: result8_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] result16_msg_byte(
    input logic [7:0] prefix0,
    input logic [7:0] prefix1,
    input logic [15:0] write_value,
    input logic [15:0] got_value,
    input logic [5:0] idx
  );
    result16_msg_byte = 8'h20;
    unique case (idx)
      6'd0:    result16_msg_byte = prefix0;
      6'd1:    result16_msg_byte = prefix1;
      6'd2:    result16_msg_byte = 8'h20;
      6'd3:    result16_msg_byte = 8'h57;
      6'd4:    result16_msg_byte = 8'h3D;
      6'd5:    result16_msg_byte = hex16_digit(write_value, 0);
      6'd6:    result16_msg_byte = hex16_digit(write_value, 1);
      6'd7:    result16_msg_byte = hex16_digit(write_value, 2);
      6'd8:    result16_msg_byte = hex16_digit(write_value, 3);
      6'd9:    result16_msg_byte = 8'h20;
      6'd10:   result16_msg_byte = 8'h52;
      6'd11:   result16_msg_byte = 8'h3D;
      6'd12:   result16_msg_byte = hex16_digit(got_value, 0);
      6'd13:   result16_msg_byte = hex16_digit(got_value, 1);
      6'd14:   result16_msg_byte = hex16_digit(got_value, 2);
      6'd15:   result16_msg_byte = hex16_digit(got_value, 3);
      6'd16:   result16_msg_byte = 8'h0D;
      default: result16_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] result16_8_msg_byte(
    input logic [7:0] prefix0,
    input logic [7:0] prefix1,
    input logic [15:0] write_value,
    input logic [7:0] got_value,
    input logic [5:0] idx
  );
    result16_8_msg_byte = 8'h20;
    unique case (idx)
      6'd0:    result16_8_msg_byte = prefix0;
      6'd1:    result16_8_msg_byte = prefix1;
      6'd2:    result16_8_msg_byte = 8'h20;
      6'd3:    result16_8_msg_byte = 8'h57;
      6'd4:    result16_8_msg_byte = 8'h3D;
      6'd5:    result16_8_msg_byte = hex16_digit(write_value, 0);
      6'd6:    result16_8_msg_byte = hex16_digit(write_value, 1);
      6'd7:    result16_8_msg_byte = hex16_digit(write_value, 2);
      6'd8:    result16_8_msg_byte = hex16_digit(write_value, 3);
      6'd9:    result16_8_msg_byte = 8'h20;
      6'd10:   result16_8_msg_byte = 8'h52;
      6'd11:   result16_8_msg_byte = 8'h3D;
      6'd12:   result16_8_msg_byte = hex8_digit(got_value, 0);
      6'd13:   result16_8_msg_byte = hex8_digit(got_value, 1);
      6'd14:   result16_8_msg_byte = 8'h0D;
      default: result16_8_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] result32_msg_byte(
    input logic [7:0] prefix0,
    input logic [7:0] prefix1,
    input logic [31:0] write_value,
    input logic [31:0] got_value,
    input logic [5:0] idx
  );
    result32_msg_byte = 8'h20;
    unique case (idx)
      6'd0:    result32_msg_byte = prefix0;
      6'd1:    result32_msg_byte = prefix1;
      6'd2:    result32_msg_byte = 8'h20;
      6'd3:    result32_msg_byte = 8'h57;
      6'd4:    result32_msg_byte = 8'h3D;
      6'd5:    result32_msg_byte = hex32_digit(write_value, 0);
      6'd6:    result32_msg_byte = hex32_digit(write_value, 1);
      6'd7:    result32_msg_byte = hex32_digit(write_value, 2);
      6'd8:    result32_msg_byte = hex32_digit(write_value, 3);
      6'd9:    result32_msg_byte = hex32_digit(write_value, 4);
      6'd10:   result32_msg_byte = hex32_digit(write_value, 5);
      6'd11:   result32_msg_byte = hex32_digit(write_value, 6);
      6'd12:   result32_msg_byte = hex32_digit(write_value, 7);
      6'd13:   result32_msg_byte = 8'h20;
      6'd14:   result32_msg_byte = 8'h52;
      6'd15:   result32_msg_byte = 8'h3D;
      6'd16:   result32_msg_byte = hex32_digit(got_value, 0);
      6'd17:   result32_msg_byte = hex32_digit(got_value, 1);
      6'd18:   result32_msg_byte = hex32_digit(got_value, 2);
      6'd19:   result32_msg_byte = hex32_digit(got_value, 3);
      6'd20:   result32_msg_byte = hex32_digit(got_value, 4);
      6'd21:   result32_msg_byte = hex32_digit(got_value, 5);
      6'd22:   result32_msg_byte = hex32_digit(got_value, 6);
      6'd23:   result32_msg_byte = hex32_digit(got_value, 7);
      6'd24:   result32_msg_byte = 8'h0D;
      default: result32_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] result64_msg_byte(
    input logic [7:0] prefix0,
    input logic [7:0] prefix1,
    input logic [63:0] write_value,
    input logic [63:0] got_value,
    input logic [5:0] idx
  );
    result64_msg_byte = 8'h20;
    unique case (idx)
      6'd0:    result64_msg_byte = prefix0;
      6'd1:    result64_msg_byte = prefix1;
      6'd2:    result64_msg_byte = 8'h20;
      6'd3:    result64_msg_byte = 8'h57;
      6'd4:    result64_msg_byte = 8'h3D;
      6'd5:    result64_msg_byte = hex64_digit(write_value, 0);
      6'd6:    result64_msg_byte = hex64_digit(write_value, 1);
      6'd7:    result64_msg_byte = hex64_digit(write_value, 2);
      6'd8:    result64_msg_byte = hex64_digit(write_value, 3);
      6'd9:    result64_msg_byte = hex64_digit(write_value, 4);
      6'd10:   result64_msg_byte = hex64_digit(write_value, 5);
      6'd11:   result64_msg_byte = hex64_digit(write_value, 6);
      6'd12:   result64_msg_byte = hex64_digit(write_value, 7);
      6'd13:   result64_msg_byte = hex64_digit(write_value, 8);
      6'd14:   result64_msg_byte = hex64_digit(write_value, 9);
      6'd15:   result64_msg_byte = hex64_digit(write_value, 10);
      6'd16:   result64_msg_byte = hex64_digit(write_value, 11);
      6'd17:   result64_msg_byte = hex64_digit(write_value, 12);
      6'd18:   result64_msg_byte = hex64_digit(write_value, 13);
      6'd19:   result64_msg_byte = hex64_digit(write_value, 14);
      6'd20:   result64_msg_byte = hex64_digit(write_value, 15);
      6'd21:   result64_msg_byte = 8'h20;
      6'd22:   result64_msg_byte = 8'h52;
      6'd23:   result64_msg_byte = 8'h3D;
      6'd24:   result64_msg_byte = hex64_digit(got_value, 0);
      6'd25:   result64_msg_byte = hex64_digit(got_value, 1);
      6'd26:   result64_msg_byte = hex64_digit(got_value, 2);
      6'd27:   result64_msg_byte = hex64_digit(got_value, 3);
      6'd28:   result64_msg_byte = hex64_digit(got_value, 4);
      6'd29:   result64_msg_byte = hex64_digit(got_value, 5);
      6'd30:   result64_msg_byte = hex64_digit(got_value, 6);
      6'd31:   result64_msg_byte = hex64_digit(got_value, 7);
      6'd32:   result64_msg_byte = hex64_digit(got_value, 8);
      6'd33:   result64_msg_byte = hex64_digit(got_value, 9);
      6'd34:   result64_msg_byte = hex64_digit(got_value, 10);
      6'd35:   result64_msg_byte = hex64_digit(got_value, 11);
      6'd36:   result64_msg_byte = hex64_digit(got_value, 12);
      6'd37:   result64_msg_byte = hex64_digit(got_value, 13);
      6'd38:   result64_msg_byte = hex64_digit(got_value, 14);
      6'd39:   result64_msg_byte = hex64_digit(got_value, 15);
      6'd40:   result64_msg_byte = 8'h0D;
      default: result64_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] status_msg_byte(
    input logic [7:0] errors,
    input logic [5:0] idx
  );
    logic pass;
    pass = (errors == 8'd0);
    unique case (idx)
      6'd0:    status_msg_byte = 8'h54;
      6'd1:    status_msg_byte = 8'h45;
      6'd2:    status_msg_byte = 8'h53;
      6'd3:    status_msg_byte = 8'h54;
      6'd4:    status_msg_byte = 8'h20;
      6'd5:    status_msg_byte = 8'h52;
      6'd6:    status_msg_byte = 8'h46;
      6'd7:    status_msg_byte = 8'h20;
      6'd8:    status_msg_byte = 8'h50;
      6'd9:    status_msg_byte = 8'h52;
      6'd10:   status_msg_byte = 8'h45;
      6'd11:   status_msg_byte = 8'h56;
      6'd12:   status_msg_byte = 8'h49;
      6'd13:   status_msg_byte = 8'h45;
      6'd14:   status_msg_byte = 8'h57;
      6'd15:   status_msg_byte = 8'h20;
      6'd16:   status_msg_byte = pass ? 8'h50 : 8'h46;
      6'd17:   status_msg_byte = 8'h41;
      6'd18:   status_msg_byte = pass ? 8'h53 : 8'h49;
      6'd19:   status_msg_byte = pass ? 8'h53 : 8'h4C;
      6'd20:   status_msg_byte = 8'h45;
      6'd21:   status_msg_byte = 8'h44;
      6'd22:   status_msg_byte = 8'h0D;
      default: status_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] msg_byte(
    input msg_kind_e kind,
    input logic [5:0] idx
  );
    unique case (kind)
      MsgBoot:   msg_byte = boot_msg_byte(idx);
      MsgP0:     msg_byte = result8_msg_byte(8'h50, 8'h30, PreviewSeed, preview_read0_q, idx);
      MsgP1:     msg_byte = result8_msg_byte(8'h50, 8'h31, PreviewNoise, preview_read1_q, idx);
      MsgP2:     msg_byte = result8_msg_byte(8'h50, 8'h32, PreviewSplit, preview_read2_q, idx);
      MsgP3:     msg_byte = result8_msg_byte(8'h50, 8'h33, PreviewBlocked,
                                             preview_contract_read_q, idx);
      MsgP4:     msg_byte = result8_msg_byte(8'h50, 8'h34, PreviewProtocolExpected,
                                             preview_protocol_diag_q, idx);
      MsgD0:     msg_byte = result16_8_msg_byte(8'h44, 8'h30, DiffSeed, diff_read0_q, idx);
      MsgD1:     msg_byte = result16_8_msg_byte(8'h44, 8'h31, DiffLowOnly, diff_read1_q, idx);
      MsgD2:     msg_byte = result16_8_msg_byte(8'h44, 8'h32, DiffLowOnly, diff_read2_q, idx);
      MsgD3:     msg_byte = result16_8_msg_byte(8'h44, 8'h33, DiffHighOnly, diff_read3_q, idx);
      MsgF0:     msg_byte = result32_msg_byte(8'h46, 8'h30, FrontendWord0, front_read0_q, idx);
      MsgF1:     msg_byte = result32_msg_byte(8'h46, 8'h31, FrontendOddReadExpected, front_read1_q, idx);
      MsgF2:     msg_byte = result32_msg_byte(8'h46, 8'h32, FrontendWrapReadExpected, front_read2_q, idx);
      MsgF3:     msg_byte = result32_msg_byte(8'h46, 8'h33, FrontendPartialWord, front_read3_q, idx);
      MsgF4:     msg_byte = result32_msg_byte(8'h46, 8'h34, FrontendLateNoBypassExpected, front_read4_q, idx);
      MsgF5:     msg_byte = result32_msg_byte(8'h46, 8'h35, FrontendLateNoBypassExpected, front_read5_q, idx);
      MsgR0:     msg_byte = result16_msg_byte(8'h52, 8'h30, RegSeed, reg_read0_q, idx);
      MsgR1:     msg_byte = result16_msg_byte(8'h52, 8'h31, RegNoise, reg_read1_q, idx);
      MsgR2:     msg_byte = result16_msg_byte(8'h52, 8'h32, RegSplit, reg_read2_q, idx);
      MsgC0:     msg_byte = result16_msg_byte(8'h43, 8'h30, DecSeed, dec_read0_q, idx);
      MsgC1:     msg_byte = result16_msg_byte(8'h43, 8'h31, DecSeed, dec_read1_q, idx);
      MsgC2:     msg_byte = result16_msg_byte(8'h43, 8'h32, DecRearm, dec_read2_q, idx);
      MsgA0:     msg_byte = result32_msg_byte(8'h41, 8'h30, ParSeedPacked, par_read0_q, idx);
      MsgA1:     msg_byte = result32_msg_byte(8'h41, 8'h31, ParStalePacked, par_read1_q, idx);
      MsgA2:     msg_byte = result32_msg_byte(8'h41, 8'h32, ParRearmPacked, par_read2_q, idx);
      MsgT0:     msg_byte = result16_msg_byte(8'h54, 8'h30, TwoSeed, two_read0_q, idx);
      MsgT1:     msg_byte = result16_msg_byte(8'h54, 8'h31, TwoSeed, two_read1_q, idx);
      MsgT2:     msg_byte = result16_msg_byte(8'h54, 8'h32, 16'h0000, two_read2_q, idx);
      MsgT3:     msg_byte = result16_msg_byte(8'h54, 8'h33, TwoRearm, two_read3_q, idx);
      MsgV0:     msg_byte = result16_msg_byte(8'h56, 8'h30, TriSeedA, tri_read0_q, idx);
      MsgV1:     msg_byte = result16_msg_byte(8'h56, 8'h31, TriSeedA, tri_read1_q, idx);
      MsgV2:     msg_byte = result16_msg_byte(8'h56, 8'h32, TriRearmA, tri_read2_q, idx);
      MsgQ0:     msg_byte = result32_msg_byte(8'h51, 8'h30, QueuePacked0, rq_read0_q, idx);
      MsgQ1:     msg_byte = result32_msg_byte(8'h51, 8'h31, QueuePacked1, rq_read1_q, idx);
      MsgQ2:     msg_byte = result32_msg_byte(8'h51, 8'h32, QueuePacked2, rq_read2_q, idx);
      MsgS0:     msg_byte = result16_msg_byte(8'h53, 8'h30, SingleSeed, single_read0_q, idx);
      MsgS1:     msg_byte = result16_msg_byte(8'h53, 8'h31, ChainNoise, single_read1_q, idx);
      MsgS2:     msg_byte = result16_msg_byte(8'h53, 8'h32, SingleRearm, single_read2_q, idx);
      MsgW0:     msg_byte = result16_msg_byte(8'h57, 8'h30, ChainSeed, chain_read0_q, idx);
      MsgW1:     msg_byte = result16_msg_byte(8'h57, 8'h31, ChainNoise, chain_read1_q, idx);
      MsgW2:     msg_byte = result16_msg_byte(8'h57, 8'h32, ChainRearm, chain_read2_q, idx);
      MsgY0:     msg_byte = result16_msg_byte(8'h59, 8'h30, DualDiagExpected, dual_seed_diag_q, idx);
      MsgY1:     msg_byte = result16_msg_byte(8'h59, 8'h31, DualDiagExpected, dual_block_diag_q, idx);
      MsgY2:     msg_byte = result16_msg_byte(8'h59, 8'h32, DualDiagExpected, dual_rearm_diag_q, idx);
      MsgM0:     msg_byte = result32_msg_byte(8'h4D, 8'h30, CacheMetaFill, cache_meta_read0_q, idx);
      MsgM1:     msg_byte = result32_msg_byte(8'h4D, 8'h31, CacheMetaStore, cache_meta_read1_q, idx);
      MsgL0:     msg_byte = result64_msg_byte(8'h4C, 8'h30, CacheLineFill, cache_lram_read0_q, idx);
      MsgL1:     msg_byte = result64_msg_byte(8'h4C, 8'h31, CacheLineStore, cache_lram_read1_q, idx);
      MsgL2:     msg_byte = result64_msg_byte(8'h4C, 8'h32, CacheLineSeed, cache_lram_read2_q, idx);
      MsgB0:     msg_byte = result64_msg_byte(8'h42, 8'h30, CacheLineRwSame,
                                             cache_lram_rw_same_q, idx);
      MsgB1:     msg_byte = result64_msg_byte(8'h42, 8'h31, CacheLineOtherOld,
                                             cache_lram_rw_diff_q, idx);
      MsgB2:     msg_byte = result64_msg_byte(8'h42, 8'h32, CacheLineHoldWrite,
                                             cache_lram_hold_write_q, idx);
      MsgB3:     msg_byte = result64_msg_byte(8'h42, 8'h33, CacheLineLowCollision,
                                             cache_lram_low_collision_q, idx);
      MsgB4:     msg_byte = result64_msg_byte(8'h42, 8'h34, CacheLineHighCollision,
                                             cache_lram_high_collision_q, idx);
      MsgX0:     msg_byte = result64_msg_byte(8'h58, 8'h30, CacheLineFill, dcache_arr_dbg_read_q, idx);
      MsgX1:     msg_byte = result64_msg_byte(8'h58, 8'h31, CacheLineFill, dcache_arr_core_read_q, idx);
      MsgZ0:     msg_byte = result8_msg_byte(8'h5A, 8'h30, DcacheMetaArrayDiagExpected,
                                             dcache_md_array_diag_q, idx);
      MsgZ1:     msg_byte = result8_msg_byte(8'h5A, 8'h31, DcacheDataAllDiagExpected,
                                             dcache_data_all_diag_q, idx);
      MsgZ2:     msg_byte = result8_msg_byte(8'h5A, 8'h32, DcacheDataHalfDiagExpected,
                                             dcache_data_low_diag_q, idx);
      MsgZ3:     msg_byte = result8_msg_byte(8'h5A, 8'h33, DcacheDataHalfDiagExpected,
                                             dcache_data_high_diag_q, idx);
      MsgZ4:     msg_byte = result8_msg_byte(8'h5A, 8'h34, DcacheDataIsoDiagExpected,
                                             dcache_data_iso_diag_q, idx);
      MsgZ5:     msg_byte = result8_msg_byte(8'h5A, 8'h35, DcacheLeafHitDiagExpected,
                                             dcache_leaf_hit_diag_q, idx);
      MsgZ6:     msg_byte = result8_msg_byte(8'h5A, 8'h36, CacheLramReadGateDiagExpected,
                                             cache_lram_read_gate_diag_q, idx);
      MsgZ7:     msg_byte = result8_msg_byte(8'h5A, 8'h37, DcacheTimingDiagExpected,
                                             dcache_timing_diag_q, idx);
      MsgZ8:     msg_byte = result8_msg_byte(8'h5A, 8'h38, DcacheArrayHoldDiagExpected,
                                             dcache_array_hold_diag_q, idx);
      MsgZ9:     msg_byte = result8_msg_byte(8'h5A, 8'h39, DcacheMissSuppressDiagExpected,
                                             dcache_miss_suppress_diag_q, idx);
      MsgZ10:    msg_byte = result8_msg_byte(8'h5A, 8'h41, DcacheBackToBackDiagExpected,
                                             dcache_back_to_back_diag_q, idx);
      MsgZ11:    msg_byte = result8_msg_byte(8'h5A, 8'h42, DcacheSweepDiagExpected,
                                             dcache_sweep_diag_q, idx);
      MsgZ12:    msg_byte = result8_msg_byte(8'h5A, 8'h43, DcachePipelineDiagExpected,
                                             dcache_pipeline_diag_q, idx);
      MsgZ13:    msg_byte = result8_msg_byte(8'h5A, 8'h44, DcacheQualDiagExpected,
                                             dcache_qual_diag_q, idx);
      MsgZ14:    msg_byte = result8_msg_byte(8'h5A, 8'h45, DcacheStressDiagExpected,
                                             dcache_stress_diag_q, idx);
      default:   msg_byte = status_msg_byte(error_count_q, idx);
    endcase
  endfunction

  function automatic msg_kind_e next_msg_kind(input msg_kind_e kind);
    unique case (kind)
      MsgBoot:   next_msg_kind = MsgP0;
      MsgP0:     next_msg_kind = MsgP1;
      MsgP1:     next_msg_kind = MsgP2;
      MsgP2:     next_msg_kind = MsgP3;
      MsgP3:     next_msg_kind = MsgP4;
      MsgP4:     next_msg_kind = MsgD0;
      MsgD0:     next_msg_kind = MsgD1;
      MsgD1:     next_msg_kind = MsgD2;
      MsgD2:     next_msg_kind = MsgD3;
      MsgD3:     next_msg_kind = MsgF0;
      MsgF0:     next_msg_kind = MsgF1;
      MsgF1:     next_msg_kind = MsgF2;
      MsgF2:     next_msg_kind = MsgF3;
      MsgF3:     next_msg_kind = MsgF4;
      MsgF4:     next_msg_kind = MsgF5;
      MsgF5:     next_msg_kind = MsgR0;
      MsgR0:     next_msg_kind = MsgR1;
      MsgR1:     next_msg_kind = MsgR2;
      MsgR2:     next_msg_kind = MsgC0;
      MsgC0:     next_msg_kind = MsgC1;
      MsgC1:     next_msg_kind = MsgC2;
      MsgC2:     next_msg_kind = MsgA0;
      MsgA0:     next_msg_kind = MsgA1;
      MsgA1:     next_msg_kind = MsgA2;
      MsgA2:     next_msg_kind = MsgT0;
      MsgT0:     next_msg_kind = MsgT1;
      MsgT1:     next_msg_kind = MsgT2;
      MsgT2:     next_msg_kind = MsgT3;
      MsgT3:     next_msg_kind = MsgV0;
      MsgV0:     next_msg_kind = MsgV1;
      MsgV1:     next_msg_kind = MsgV2;
      MsgV2:     next_msg_kind = MsgQ0;
      MsgQ0:     next_msg_kind = MsgQ1;
      MsgQ1:     next_msg_kind = MsgQ2;
      MsgQ2:     next_msg_kind = MsgS0;
      MsgS0:     next_msg_kind = MsgS1;
      MsgS1:     next_msg_kind = MsgS2;
      MsgS2:     next_msg_kind = MsgW0;
      MsgW0:     next_msg_kind = MsgW1;
      MsgW1:     next_msg_kind = MsgW2;
      MsgW2:     next_msg_kind = MsgY0;
      MsgY0:     next_msg_kind = MsgY1;
      MsgY1:     next_msg_kind = MsgY2;
      MsgY2:     next_msg_kind = MsgM0;
      MsgM0:     next_msg_kind = MsgM1;
      MsgM1:     next_msg_kind = MsgL0;
      MsgL0:     next_msg_kind = MsgL1;
      MsgL1:     next_msg_kind = MsgL2;
      MsgL2:     next_msg_kind = MsgB0;
      MsgB0:     next_msg_kind = MsgB1;
      MsgB1:     next_msg_kind = MsgB2;
      MsgB2:     next_msg_kind = MsgB3;
      MsgB3:     next_msg_kind = MsgB4;
      MsgB4:     next_msg_kind = MsgX0;
      MsgX0:     next_msg_kind = MsgX1;
      MsgX1:     next_msg_kind = MsgZ0;
      MsgZ0:     next_msg_kind = MsgZ1;
      MsgZ1:     next_msg_kind = MsgZ2;
      MsgZ2:     next_msg_kind = MsgZ3;
      MsgZ3:     next_msg_kind = MsgZ4;
      MsgZ4:     next_msg_kind = MsgZ5;
      MsgZ5:     next_msg_kind = MsgZ6;
      MsgZ6:     next_msg_kind = MsgZ7;
      MsgZ7:     next_msg_kind = MsgZ8;
      MsgZ8:     next_msg_kind = MsgZ9;
      MsgZ9:     next_msg_kind = MsgZ10;
      MsgZ10:    next_msg_kind = MsgZ11;
      MsgZ11:    next_msg_kind = MsgZ12;
      MsgZ12:    next_msg_kind = MsgZ13;
      MsgZ13:    next_msg_kind = MsgZ14;
      MsgZ14:    next_msg_kind = MsgStatus;
      default:   next_msg_kind = MsgStatus;
    endcase
  endfunction

  always_comb begin
    preview_wr_next = 1'b0;
    preview_wr_en = 1'b0;
    preview_wr_addr = '0;
    preview_wr_data = '0;
    preview_rd_addr = '0;

    preview_contract_hold_hi = 1'b0;
    preview_contract_wr_next = 1'b0;
    preview_contract_wr_en = 1'b0;
    preview_contract_wr_addr = '0;
    preview_contract_wr_data = '0;
    preview_contract_rd_addr = '0;

    diff_wr_next = '0;
    diff_wr_en = '0;
    diff_wr_addr = 1'b0;
    diff_wr_data = '0;
    diff_rd_addr = '0;

    front_wr_next = '0;
    front_wr_en = '0;
    front_wr_addr = 1'b0;
    front_wr_data = '0;
    front_rd_addr = '0;

    reg_wr_next = 1'b0;
    reg_wr_data = '0;
    reg_wr_addr = '0;
    reg_wr_en = 1'b0;
    reg_rd_addr = '0;
    reg_rd_en = 1'b0;

    dec_wr_next = 1'b0;
    dec_wr_en = 1'b0;
    dec_rd_addr = '0;
    dec_wr_addr = '0;
    dec_wr_data = '0;

    par_wr_next = 1'b0;
    par_wr_en = 1'b0;
    par_wr_addr = '0;
    par_wr_data = '0;

    two_wr_next = 1'b0;
    two_wr_en = 1'b0;
    two_rd_addr_a = '0;
    two_rd_addr_b = '0;
    two_wr_addr = '0;
    two_wr_data = '0;

    tri_wr_next_a = 1'b0;
    tri_wr_next_b = 1'b0;
    tri_wr_en_a = 1'b0;
    tri_wr_en_b = 1'b0;
    tri_rd_addr_a = '0;
    tri_rd_addr_b = '0;
    tri_rd_addr_c = '0;
    tri_wr_addr_a = '0;
    tri_wr_addr_b = '0;
    tri_wr_data_a = '0;
    tri_wr_data_b = '0;

    rq_wr_next = 1'b0;
    rq_wr_en = 1'b0;
    rq_addr = '0;
    rq_data = '0;

    single_wr_next = 1'b0;
    single_wr_en = 1'b0;
    single_wr_data = '0;

    chain_mem_wen = 1'b0;
    chain_wdata = '0;

    dual_hold_hi = 1'b0;
    gated_preview_en = 1'b0;
    dual_p_wr_next = 1'b0;
    dual_p_wr_en = 1'b0;
    dual_p_wr_addr = '0;
    dual_p_wr_data = '0;
    dual_p_rd_addr = '0;
    dual_diff_wr_next = '0;
    dual_diff_wr_en = '0;
    dual_diff_wr_addr = 1'b0;
    dual_diff_wr_data = '0;
    dual_diff_rd_addr = '0;
    dual_front_wr_next = '0;
    dual_front_wr_en = '0;
    dual_front_wr_addr = 1'b0;
    dual_front_wr_data = '0;
    dual_front_rd_addr = '0;
    gated_front_wr_next = '0;
    gated_front_wr_en = '0;
    gated_front_wr_addr = 1'b0;
    gated_front_wr_data = '0;
    gated_front_rd_addr = '0;
    dual_reg_wr_next = 1'b0;
    dual_reg_wr_data = '0;
    dual_reg_wr_addr = '0;
    dual_reg_wr_en = 1'b0;
    dual_reg_rd_addr = '0;
    dual_reg_rd_en = 1'b0;
    dual_dec_wr_next = 1'b0;
    dual_dec_wr_en = 1'b0;
    dual_dec_rd_addr = '0;
    dual_dec_wr_addr = '0;
    dual_dec_wr_data = '0;
    dual_par_wr_next = 1'b0;
    dual_par_wr_en = 1'b0;
    dual_par_wr_addr = '0;
    dual_par_wr_data = '0;
    dual_two_wr_next = 1'b0;
    dual_two_wr_en = 1'b0;
    dual_two_rd_addr_a = '0;
    dual_two_rd_addr_b = '0;
    dual_two_wr_addr = '0;
    dual_two_wr_data = '0;
    dual_tri_wr_next_a = 1'b0;
    dual_tri_wr_next_b = 1'b0;
    dual_tri_wr_en_a = 1'b0;
    dual_tri_wr_en_b = 1'b0;
    dual_tri_rd_addr_a = '0;
    dual_tri_rd_addr_b = '0;
    dual_tri_rd_addr_c = '0;
    dual_tri_wr_addr_a = '0;
    dual_tri_wr_addr_b = '0;
    dual_tri_wr_data_a = '0;
    dual_tri_wr_data_b = '0;
    dual_single_wr_next = 1'b0;
    dual_single_wr_en = 1'b0;
    dual_single_wr_data = '0;

    cache_meta_wr_next = 1'b0;
    cache_meta_wr_en = 1'b0;
    cache_meta_wr_addr = '0;
    cache_meta_wr_data = '0;
    cache_meta_rd_addr = '0;

    cache_lram_wr_next = 1'b0;
    cache_lram_wr_en = 1'b0;
    cache_lram_wr_next_mask = 2'b11;
    cache_lram_wr_en_mask = 2'b11;
    cache_lram_wr_addr = '0;
    cache_lram_wr_data = '0;
    cache_lram_rd_en = 1'b0;
    cache_lram_rd_addr = '0;

    dcache_arr_read_valid = 1'b0;
    dcache_arr_s3_write_valid = 1'b0;
    dcache_arr_s4_write_valid = 1'b0;
    dcache_md_read_set = '0;
    dcache_md_write_valid = 1'b0;
    dcache_md_write_valid_prev = 1'b0;
    dcache_md_write_req = '0;
    dcache_md_clear_all = 1'b0;
    dcache_md_clear_low = 1'b0;
    dcache_timing_s1_en = 1'b0;
    dcache_miss_s1_en = 1'b0;
    dcache_pipeline_s0_valid = 1'b0;
    dcache_qual_s0_valid = 1'b0;
    dcache_qual_s0_is_bypass = 1'b0;
    dcache_stress_s0_valid = 1'b0;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
      dcache_arr_read_req[i] = '0;
      dcache_arr_write_req[i] = '0;
    end

    unique case (state_q)
      StP0Write: begin
        preview_wr_next = 1'b1;
        preview_wr_en = 1'b1;
        preview_wr_addr = 2'd1;
        preview_wr_data = PreviewSeed;
      end

      StP0Check: begin
        preview_rd_addr = 2'd1;
      end

      StP1Write: begin
        preview_wr_en = 1'b1;
        preview_wr_addr = 2'd2;
        preview_wr_data = PreviewNoise;
      end

      StP1Check: begin
        preview_rd_addr = 2'd2;
      end

      StP2Arm: begin
        preview_wr_next = 1'b1;
        preview_wr_addr = 2'd3;
        preview_wr_data = PreviewSplit;
      end

      StP2Commit: begin
        preview_wr_en = 1'b1;
        preview_wr_addr = 2'd3;
        preview_wr_data = 8'hBC;
      end

      StP2Check: begin
        preview_rd_addr = 2'd3;
      end

      StP3SeedArm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_addr = 2'd1;
        preview_contract_wr_data = PreviewSeed;
      end

      StP3SeedCommit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd1;
        preview_contract_wr_data = PreviewNoise;
      end

      StP3Disarm: begin
        preview_contract_wr_addr = 2'd1;
        preview_contract_wr_data = PreviewNoise;
      end

      StP3BlockedArm: begin
        preview_contract_hold_hi = 1'b1;
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_addr = 2'd1;
        preview_contract_wr_data = PreviewBlocked;
      end

      StP3BlockedCommit: begin
        preview_contract_hold_hi = 1'b1;
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd1;
        preview_contract_wr_data = PreviewBlocked;
      end

      StP3Check: begin
        preview_contract_rd_addr = 2'd1;
      end

      StP4Seed0Arm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h10;
      end

      StP4Seed0Commit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd0;
        preview_contract_wr_data = 8'hE0;
      end

      StP4Seed1Arm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h11;
      end

      StP4Seed1Commit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd1;
        preview_contract_wr_data = 8'hE1;
      end

      StP4Seed2Arm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h12;
      end

      StP4Seed2Commit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd2;
        preview_contract_wr_data = 8'hE2;
      end

      StP4Seed3Arm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h13;
      end

      StP4Seed3Commit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd3;
        preview_contract_wr_data = 8'hE3;
      end

      StP4PreviewOnlyArm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h21;
      end

      StP4PreviewOnlyCheck,
      StP4CommitOnlyCheck: begin
        preview_contract_rd_addr = 2'd1;
      end

      StP4CommitOnly: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd1;
        preview_contract_wr_data = 8'hE1;
      end

      StP4DataArm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h32;
      end

      StP4DataCommit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd2;
        preview_contract_wr_data = 8'hE2;
      end

      StP4DataCheck,
      StP4HoldCheck: begin
        preview_contract_rd_addr = 2'd2;
      end

      StP4DelayArm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h43;
      end

      StP4DelayWait: begin
        preview_contract_wr_data = 8'hE3;
      end

      StP4DelayCommit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd3;
        preview_contract_wr_data = 8'hE4;
      end

      StP4DelayCheck,
      StP4MultiCheck: begin
        preview_contract_rd_addr = 2'd3;
      end

      StP4AddrArm: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h54;
      end

      StP4AddrCommit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd0;
        preview_contract_wr_data = 8'hE5;
      end

      StP4AddrCheck,
      StP4FinalCheck: begin
        preview_contract_rd_addr = 2'd0;
      end

      StP4HoldArm: begin
        preview_contract_hold_hi = 1'b1;
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h65;
      end

      StP4HoldCommit: begin
        preview_contract_hold_hi = 1'b1;
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd2;
        preview_contract_wr_data = 8'hE6;
      end

      StP4MultiArmA: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h76;
      end

      StP4MultiArmB: begin
        preview_contract_wr_next = 1'b1;
        preview_contract_wr_data = 8'h87;
      end

      StP4MultiCommit: begin
        preview_contract_wr_en = 1'b1;
        preview_contract_wr_addr = 2'd3;
        preview_contract_wr_data = 8'hE7;
      end

      StD0Write: begin
        diff_wr_next = 2'b11;
        diff_wr_en = 2'b11;
        diff_wr_addr = 1'b1;
        diff_wr_data = DiffSeed;
      end

      StD0Check: begin
        diff_rd_addr = 2'd2;
      end

      StD1Write: begin
        diff_wr_next = 2'b01;
        diff_wr_en = 2'b11;
        diff_wr_addr = 1'b1;
        diff_wr_data = DiffLowOnly;
      end

      StD1Check: begin
        diff_rd_addr = 2'd2;
      end

      StD2Check: begin
        diff_rd_addr = 2'd3;
      end

      StD3Arm: begin
        diff_wr_next = 2'b10;
        diff_wr_addr = 1'b0;
        diff_wr_data = DiffHighOnly;
      end

      StD3Commit: begin
        diff_wr_en = 2'b10;
        diff_wr_addr = 1'b0;
        diff_wr_data = DiffHighOnly;
      end

      StD3Check: begin
        diff_rd_addr = 2'd1;
      end

      StF0Arm: begin
        front_wr_next = 8'hFF;
        front_wr_addr = 1'b0;
        front_wr_data = FrontendLine0;
      end

      StF0Commit: begin
        front_wr_en = 8'hFF;
        front_wr_addr = 1'b0;
        front_wr_data = FrontendLine0;
      end

      StF0Check: begin
        front_rd_addr = 5'd0;
      end

      StF1Check: begin
        front_rd_addr = 5'd1;
      end

      StF2Arm: begin
        front_wr_next = 8'hFF;
        front_wr_addr = 1'b1;
        front_wr_data = FrontendLine1;
      end

      StF2Commit: begin
        front_wr_en = 8'hFF;
        front_wr_addr = 1'b1;
        front_wr_data = FrontendLine1;
      end

      StF2Check: begin
        front_rd_addr = 5'd15;
      end

      StF3Arm: begin
        front_wr_next = 8'h01;
        front_wr_addr = 1'b0;
        front_wr_data[31:0] = FrontendPartialWord;
      end

      StF3Commit: begin
        front_wr_en = 8'h01;
        front_wr_addr = 1'b0;
        front_wr_data[31:0] = FrontendPartialWord;
      end

      StF3Check: begin
        front_rd_addr = 5'd0;
      end

      StF4LateArm: begin
        front_wr_next = 8'hFF;
        front_wr_addr = 1'b1;
        front_wr_data = '0;
      end

      StF4LateCommit: begin
        // Current-cycle write data must not bypass the RF preview staging point.
        front_wr_en = 8'hFF;
        front_wr_addr = 1'b1;
        front_wr_data = FrontendLine0;
      end

      StF4LateRead0,
      StF4LateCheck0: begin
        front_rd_addr = 5'd16;
      end

      StF5LateRead1,
      StF5LateCheck1: begin
        front_rd_addr = 5'd17;
      end

      StR0Write: begin
        reg_wr_next = 1'b1;
        reg_wr_en = 1'b1;
        reg_wr_addr = 3'd3;
        reg_wr_data = RegSeed;
      end

      StR0ReadArm: begin
        reg_rd_en = 1'b1;
        reg_rd_addr = 3'd3;
      end

      StR1Write: begin
        reg_wr_en = 1'b1;
        reg_wr_addr = 3'd4;
        reg_wr_data = RegNoise;
      end

      StR1ReadArm: begin
        reg_rd_en = 1'b1;
        reg_rd_addr = 3'd4;
      end

      StR2Arm: begin
        reg_wr_next = 1'b1;
        reg_wr_addr = 3'd5;
        reg_wr_data = RegSplit;
      end

      StR2Commit: begin
        reg_wr_en = 1'b1;
        reg_wr_addr = 3'd5;
        reg_wr_data = 16'h1234;
      end

      StR2ReadArm: begin
        reg_rd_en = 1'b1;
        reg_rd_addr = 3'd5;
      end

      StC0Arm: begin
        dec_wr_next = 1'b1;
        dec_wr_data = DecSeed;
      end

      StC0Commit: begin
        dec_wr_en = 1'b1;
        dec_wr_addr = 8'h04;
        dec_wr_data = 16'h6A6A;
      end

      StC0Check: begin
        dec_rd_addr = 8'h04;
      end

      StC1Write: begin
        dec_wr_en = 1'b1;
        dec_wr_addr = 8'h18;
        dec_wr_data = 16'h8888;
      end

      StC1Check: begin
        dec_rd_addr = 8'h18;
      end

      StC2Arm: begin
        dec_wr_next = 1'b1;
        dec_wr_data = DecRearm;
      end

      StC2Commit: begin
        dec_wr_en = 1'b1;
        dec_wr_addr = 8'h42;
        dec_wr_data = 16'hFFFF;
      end

      StC2Check: begin
        dec_rd_addr = 8'h42;
      end

      StA0Arm: begin
        par_wr_next = 1'b1;
        par_wr_data = 8'h12;
      end

      StA0Commit: begin
        par_wr_en = 1'b1;
        par_wr_addr = 2'd0;
        par_wr_data = 8'h56;
      end

      StA1Write: begin
        par_wr_en = 1'b1;
        par_wr_addr = 2'd1;
        par_wr_data = 8'hCD;
      end

      StA2Arm: begin
        par_wr_next = 1'b1;
        par_wr_data = 8'h9A;
      end

      StA2Commit: begin
        par_wr_en = 1'b1;
        par_wr_addr = 2'd3;
        par_wr_data = 8'hBC;
      end

      StT0Arm: begin
        two_wr_next = 1'b1;
        two_wr_data = TwoSeed;
      end

      StT0Commit: begin
        two_wr_en = 1'b1;
        two_wr_addr = 3'd1;
        two_wr_data = 16'hABCD;
      end

      StT0Check: begin
        two_rd_addr_a = 3'd1;
        two_rd_addr_b = 3'd1;
      end

      StT1Write: begin
        two_wr_en = 1'b1;
        two_wr_addr = 3'd2;
        two_wr_data = 16'hFFFF;
      end

      StT1Check: begin
        two_rd_addr_a = 3'd2;
        two_rd_addr_b = 3'd1;
      end

      StT2Arm: begin
        two_wr_next = 1'b1;
        two_wr_data = 16'hEEEE;
      end

      StT2Commit: begin
        two_wr_en = 1'b1;
        two_wr_addr = 3'd0;
        two_wr_data = 16'h7777;
      end

      StT2Check: begin
        two_rd_addr_a = 3'd0;
        two_rd_addr_b = 3'd0;
      end

      StT3Arm: begin
        two_wr_next = 1'b1;
        two_wr_data = TwoRearm;
      end

      StT3Commit: begin
        two_wr_en = 1'b1;
        two_wr_addr = 3'd7;
        two_wr_data = 16'h1111;
      end

      StT3Check: begin
        two_rd_addr_a = 3'd7;
        two_rd_addr_b = 3'd1;
      end

      StV0Arm: begin
        tri_wr_next_a = 1'b1;
        tri_wr_next_b = 1'b1;
        tri_wr_data_a = TriSeedA;
        tri_wr_data_b = TriSeedB;
      end

      StV0Commit: begin
        tri_wr_en_a = 1'b1;
        tri_wr_addr_a = 3'd1;
        tri_wr_data_a = 16'hAAAA;
        tri_wr_en_b = 1'b1;
        tri_wr_addr_b = 3'd2;
        tri_wr_data_b = 16'hBBBB;
      end

      StV0Check: begin
        tri_rd_addr_a = 3'd1;
        tri_rd_addr_b = 3'd2;
        tri_rd_addr_c = 3'd1;
      end

      StV1Write: begin
        tri_wr_en_a = 1'b1;
        tri_wr_addr_a = 3'd3;
        tri_wr_data_a = 16'h5555;
        tri_wr_en_b = 1'b1;
        tri_wr_addr_b = 3'd4;
        tri_wr_data_b = 16'h6666;
      end

      StV1Check: begin
        tri_rd_addr_a = 3'd3;
        tri_rd_addr_b = 3'd4;
        tri_rd_addr_c = 3'd3;
      end

      StV2Arm: begin
        tri_wr_next_a = 1'b1;
        tri_wr_next_b = 1'b1;
        tri_wr_data_a = TriRearmA;
        tri_wr_data_b = 16'h8888;
      end

      StV2Commit: begin
        tri_wr_en_a = 1'b1;
        tri_wr_addr_a = 3'd5;
        tri_wr_data_a = 16'h9999;
        tri_wr_en_b = 1'b1;
        tri_wr_addr_b = 3'd5;
        tri_wr_data_b = 16'hAAAA;
      end

      StV2Check: begin
        tri_rd_addr_a = 3'd5;
        tri_rd_addr_b = 3'd5;
        tri_rd_addr_c = 3'd5;
      end

      StQ0Arm: begin
        rq_wr_next = 1'b1;
        rq_addr = 2'd0;
        rq_data = QueuePushSeed;
      end

      StQ0Commit: begin
        rq_wr_en = 1'b1;
        rq_addr = 2'd0;
        rq_data = 8'h43;
      end

      StQ1Arm: begin
        rq_wr_next = 1'b1;
        rq_addr = 2'd2;
        rq_data = QueueRearmSeed;
      end

      StQ1Commit: begin
        rq_wr_en = 1'b1;
        rq_addr = 2'd2;
        rq_data = 8'hCD;
      end

      StQ2Write: begin
        rq_wr_en = 1'b1;
        rq_addr = 2'd1;
        rq_data = 8'h77;
      end

      StS0Arm: begin
        single_wr_next = 1'b1;
        single_wr_data = SingleSeed;
      end

      StS0Commit: begin
        single_wr_en = 1'b1;
        single_wr_data = 16'h1357;
      end

      StS1Write: begin
        single_wr_en = 1'b1;
        single_wr_data = ChainNoise;
      end

      StS2Arm: begin
        single_wr_next = 1'b1;
        single_wr_data = SingleRearm;
      end

      StS2Commit: begin
        single_wr_en = 1'b1;
        single_wr_data = 16'h9999;
      end

      StW0Arm: begin
        chain_mem_wen = 1'b1;
        chain_wdata = ChainSeed;
      end

      StW0Stage: begin
        chain_wdata = ChainSeed;
      end

      StW1Check: begin
        chain_wdata = ChainNoise;
      end

      StW2Arm: begin
        chain_mem_wen = 1'b1;
        chain_wdata = ChainRearm;
      end

      StW2Stage: begin
        chain_wdata = ChainRearm;
      end

      StW2Check: begin
        gated_preview_en = 1'b1;
      end

      StY0SeedArm: begin
        gated_preview_en = 1'b1;
        dual_p_wr_next = 1'b1;
        dual_p_wr_addr = 2'd1;
        dual_p_wr_data = DualPreviewSeed;
        dual_diff_wr_next = 2'b11;
        dual_diff_wr_addr = 1'b1;
        dual_diff_wr_data = DualDiffSeed;
        dual_front_wr_next = 8'hFF;
        dual_front_wr_addr = 1'b0;
        dual_front_wr_data = DualFrontSeedLine;
        gated_front_wr_next = 8'hFF;
        gated_front_wr_addr = 1'b0;
        gated_front_wr_data = DualFrontSeedLine;
        dual_reg_wr_next = 1'b1;
        dual_reg_wr_addr = 3'd3;
        dual_reg_wr_data = DualRegSeed;
        dual_dec_wr_next = 1'b1;
        dual_dec_wr_addr = 8'h08;
        dual_dec_wr_data = DualDecSeed;
        dual_par_wr_next = 1'b1;
        dual_par_wr_addr = 2'd2;
        dual_par_wr_data = DualParSeed;
        dual_two_wr_next = 1'b1;
        dual_two_wr_addr = 3'd7;
        dual_two_wr_data = DualTwoSeed;
        dual_tri_wr_next_a = 1'b1;
        dual_tri_wr_next_b = 1'b1;
        dual_tri_wr_addr_a = 3'd1;
        dual_tri_wr_addr_b = 3'd2;
        dual_tri_wr_data_a = DualTriSeedA;
        dual_tri_wr_data_b = DualTriSeedB;
        dual_single_wr_next = 1'b1;
        dual_single_wr_data = DualSingleSeed;
      end

      StY0SeedCommit: begin
        gated_preview_en = 1'b1;
        dual_p_wr_en = 1'b1;
        dual_p_wr_addr = 2'd1;
        dual_p_wr_data = DualPreviewBlock;
        dual_diff_wr_en = 2'b11;
        dual_diff_wr_addr = 1'b1;
        dual_diff_wr_data = DualDiffBlock;
        dual_front_wr_en = 8'hFF;
        dual_front_wr_addr = 1'b0;
        dual_front_wr_data = DualFrontBlockLine;
        gated_front_wr_en = 8'hFF;
        gated_front_wr_addr = 1'b0;
        gated_front_wr_data = DualFrontBlockLine;
        dual_reg_wr_en = 1'b1;
        dual_reg_wr_addr = 3'd3;
        dual_reg_wr_data = DualRegBlock;
        dual_dec_wr_en = 1'b1;
        dual_dec_wr_addr = 8'h08;
        dual_dec_wr_data = DualDecBlock;
        dual_par_wr_en = 1'b1;
        dual_par_wr_addr = 2'd2;
        dual_par_wr_data = DualParBlock;
        dual_two_wr_en = 1'b1;
        dual_two_wr_addr = 3'd7;
        dual_two_wr_data = DualTwoBlock;
        dual_tri_wr_en_a = 1'b1;
        dual_tri_wr_en_b = 1'b1;
        dual_tri_wr_addr_a = 3'd1;
        dual_tri_wr_addr_b = 3'd2;
        dual_tri_wr_data_a = DualTriBlockA;
        dual_tri_wr_data_b = DualTriBlockB;
        dual_single_wr_en = 1'b1;
        dual_single_wr_data = DualSingleBlock;
      end

      StY1BlockArm: begin
        dual_hold_hi = 1'b1;
        dual_p_wr_next = 1'b1;
        dual_p_wr_addr = 2'd1;
        dual_p_wr_data = DualPreviewBlock;
        dual_diff_wr_next = 2'b11;
        dual_diff_wr_addr = 1'b1;
        dual_diff_wr_data = DualDiffBlock;
        dual_front_wr_next = 8'hFF;
        dual_front_wr_addr = 1'b0;
        dual_front_wr_data = DualFrontBlockLine;
        dual_reg_wr_next = 1'b1;
        dual_reg_wr_addr = 3'd3;
        dual_reg_wr_data = DualRegBlock;
        dual_dec_wr_next = 1'b1;
        dual_dec_wr_addr = 8'h08;
        dual_dec_wr_data = DualDecBlock;
        dual_par_wr_next = 1'b1;
        dual_par_wr_addr = 2'd2;
        dual_par_wr_data = DualParBlock;
        dual_two_wr_next = 1'b1;
        dual_two_wr_addr = 3'd7;
        dual_two_wr_data = DualTwoBlock;
        dual_tri_wr_next_a = 1'b1;
        dual_tri_wr_next_b = 1'b1;
        dual_tri_wr_addr_a = 3'd1;
        dual_tri_wr_addr_b = 3'd2;
        dual_tri_wr_data_a = DualTriBlockA;
        dual_tri_wr_data_b = DualTriBlockB;
        dual_single_wr_next = 1'b1;
        dual_single_wr_data = DualSingleBlock;
      end

      StY1BlockCommit: begin
        dual_hold_hi = 1'b1;
        dual_p_wr_en = 1'b1;
        dual_p_wr_addr = 2'd1;
        dual_p_wr_data = DualPreviewBlock;
        dual_diff_wr_en = 2'b11;
        dual_diff_wr_addr = 1'b1;
        dual_diff_wr_data = DualDiffBlock;
        dual_front_wr_en = 8'hFF;
        dual_front_wr_addr = 1'b0;
        dual_front_wr_data = DualFrontBlockLine;
        dual_reg_wr_en = 1'b1;
        dual_reg_wr_addr = 3'd3;
        dual_reg_wr_data = DualRegBlock;
        dual_dec_wr_en = 1'b1;
        dual_dec_wr_addr = 8'h08;
        dual_dec_wr_data = DualDecBlock;
        dual_par_wr_en = 1'b1;
        dual_par_wr_addr = 2'd2;
        dual_par_wr_data = DualParBlock;
        dual_two_wr_en = 1'b1;
        dual_two_wr_addr = 3'd7;
        dual_two_wr_data = DualTwoBlock;
        dual_tri_wr_en_a = 1'b1;
        dual_tri_wr_en_b = 1'b1;
        dual_tri_wr_addr_a = 3'd1;
        dual_tri_wr_addr_b = 3'd2;
        dual_tri_wr_data_a = DualTriBlockA;
        dual_tri_wr_data_b = DualTriBlockB;
        dual_single_wr_en = 1'b1;
        dual_single_wr_data = DualSingleBlock;
      end

      StY2RearmArm: begin
        gated_preview_en = 1'b1;
        dual_p_wr_next = 1'b1;
        dual_p_wr_addr = 2'd1;
        dual_p_wr_data = DualPreviewRearm;
        dual_diff_wr_next = 2'b11;
        dual_diff_wr_addr = 1'b1;
        dual_diff_wr_data = DualDiffRearm;
        dual_front_wr_next = 8'hFF;
        dual_front_wr_addr = 1'b0;
        dual_front_wr_data = DualFrontRearmLine;
        gated_front_wr_next = 8'hFF;
        gated_front_wr_addr = 1'b0;
        gated_front_wr_data = DualFrontRearmLine;
        dual_reg_wr_next = 1'b1;
        dual_reg_wr_addr = 3'd3;
        dual_reg_wr_data = DualRegRearm;
        dual_dec_wr_next = 1'b1;
        dual_dec_wr_addr = 8'h08;
        dual_dec_wr_data = DualDecRearm;
        dual_par_wr_next = 1'b1;
        dual_par_wr_addr = 2'd2;
        dual_par_wr_data = DualParRearm;
        dual_two_wr_next = 1'b1;
        dual_two_wr_addr = 3'd7;
        dual_two_wr_data = DualTwoRearm;
        dual_tri_wr_next_a = 1'b1;
        dual_tri_wr_next_b = 1'b1;
        dual_tri_wr_addr_a = 3'd1;
        dual_tri_wr_addr_b = 3'd2;
        dual_tri_wr_data_a = DualTriRearmA;
        dual_tri_wr_data_b = DualTriRearmB;
        dual_single_wr_next = 1'b1;
        dual_single_wr_data = DualSingleRearm;
      end

      StY2RearmCommit: begin
        gated_preview_en = 1'b1;
        dual_p_wr_en = 1'b1;
        dual_p_wr_addr = 2'd1;
        dual_p_wr_data = DualPreviewBlock;
        dual_diff_wr_en = 2'b11;
        dual_diff_wr_addr = 1'b1;
        dual_diff_wr_data = DualDiffBlock;
        dual_front_wr_en = 8'hFF;
        dual_front_wr_addr = 1'b0;
        dual_front_wr_data = DualFrontBlockLine;
        gated_front_wr_en = 8'hFF;
        gated_front_wr_addr = 1'b0;
        gated_front_wr_data = DualFrontBlockLine;
        dual_reg_wr_en = 1'b1;
        dual_reg_wr_addr = 3'd3;
        dual_reg_wr_data = DualRegBlock;
        dual_dec_wr_en = 1'b1;
        dual_dec_wr_addr = 8'h08;
        dual_dec_wr_data = DualDecBlock;
        dual_par_wr_en = 1'b1;
        dual_par_wr_addr = 2'd2;
        dual_par_wr_data = DualParBlock;
        dual_two_wr_en = 1'b1;
        dual_two_wr_addr = 3'd7;
        dual_two_wr_data = DualTwoBlock;
        dual_tri_wr_en_a = 1'b1;
        dual_tri_wr_en_b = 1'b1;
        dual_tri_wr_addr_a = 3'd1;
        dual_tri_wr_addr_b = 3'd2;
        dual_tri_wr_data_a = DualTriBlockA;
        dual_tri_wr_data_b = DualTriBlockB;
        dual_single_wr_en = 1'b1;
        dual_single_wr_data = DualSingleBlock;
      end

      StY0SeedReadArm,
      StY0SeedCheck,
      StY1BlockReadArm,
      StY2RearmReadArm,
      StY2RearmCheck: begin
        dual_p_rd_addr = 2'd1;
        dual_diff_rd_addr = 2'd2;
        dual_front_rd_addr = 5'd0;
        gated_front_rd_addr = 5'd0;
        dual_reg_rd_en = 1'b1;
        dual_reg_rd_addr = 3'd3;
        dual_dec_rd_addr = 8'h08;
        dual_two_rd_addr_a = 3'd7;
        dual_two_rd_addr_b = 3'd7;
        dual_tri_rd_addr_a = 3'd1;
        dual_tri_rd_addr_b = 3'd2;
        dual_tri_rd_addr_c = 3'd1;
      end

      StY1BlockCheck: begin
        dual_p_rd_addr = 2'd1;
        dual_diff_rd_addr = 2'd2;
        dual_front_rd_addr = 5'd0;
        gated_preview_en = 1'b1;
        gated_front_rd_addr = 5'd0;
        dual_reg_rd_en = 1'b1;
        dual_reg_rd_addr = 3'd3;
        dual_dec_rd_addr = 8'h08;
        dual_two_rd_addr_a = 3'd7;
        dual_two_rd_addr_b = 3'd7;
        dual_tri_rd_addr_a = 3'd1;
        dual_tri_rd_addr_b = 3'd2;
        dual_tri_rd_addr_c = 3'd1;
      end

      StM0Arm: begin
        cache_meta_wr_next = 1'b1;
        cache_meta_wr_data = CacheMetaFill;
      end

      StM0Commit: begin
        cache_meta_wr_en = 1'b1;
        cache_meta_wr_addr = 3'd5;
        cache_meta_wr_data = CacheMetaNoise;
      end

      StM0Check: begin
        cache_meta_rd_addr = 3'd5;
      end

      StM1Arm: begin
        cache_meta_wr_next = 1'b1;
        cache_meta_wr_data = CacheMetaStore;
      end

      StM1Commit: begin
        cache_meta_wr_en = 1'b1;
        cache_meta_wr_addr = 3'd5;
        cache_meta_wr_data = CacheMetaNoise;
      end

      StM1Bubble,
      StM1Check: begin
        cache_meta_rd_addr = 3'd5;
      end

      StL0SentinelArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineSeed;
      end

      StL0SentinelCommit: begin
        cache_lram_wr_en = 1'b1;
        cache_lram_wr_addr = CacheLineSeedAddr;
        cache_lram_wr_data = CacheLineNoise;
      end

      StL0FillArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineFill;
      end

      StL0FillCommit: begin
        cache_lram_wr_en = 1'b1;
        cache_lram_wr_addr = CacheLineTestAddr;
        cache_lram_wr_data = CacheLineNoise;
      end

      StL0ReadArm,
      StL0Check: begin
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineTestAddr;
      end

      StL1StoreArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineStore;
      end

      StL1StoreCommit: begin
        cache_lram_wr_en = 1'b1;
        cache_lram_wr_addr = CacheLineTestAddr;
        cache_lram_wr_data = CacheLineNoise;
      end

      StL1ReadArm,
      StL1Check: begin
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineTestAddr;
      end

      StL2ReadArm,
      StL2Check: begin
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineSeedAddr;
      end

      StB0BaseArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineCollideOld;
      end

      StB0BaseCommit: begin
        cache_lram_wr_en = 1'b1;
        cache_lram_wr_addr = CacheLineCollideAddr;
        cache_lram_wr_data = CacheLineNoise;
      end

      StB1BaseArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineOtherOld;
      end

      StB1BaseCommit: begin
        cache_lram_wr_en = 1'b1;
        cache_lram_wr_addr = CacheLineOtherAddr;
        cache_lram_wr_data = CacheLineNoise;
      end

      StB0NewArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineRwSame;
      end

      StB0RwSame,
      StB0Check: begin
        cache_lram_wr_en = (state_q == StB0RwSame);
        cache_lram_wr_addr = CacheLineCollideAddr;
        cache_lram_wr_data = CacheLineNoise;
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineCollideAddr;
      end

      StB1NewArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineRwDiff;
      end

      StB1RwDiff,
      StB1Check: begin
        cache_lram_wr_en = (state_q == StB1RwDiff);
        cache_lram_wr_addr = CacheLineCollideAddr;
        cache_lram_wr_data = CacheLineNoise;
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineOtherAddr;
      end

      StB2ReadArm: begin
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineCollideAddr;
      end

      StB2HoldArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_data = CacheLineHoldWrite;
        cache_lram_rd_addr = CacheLineOtherAddr;
      end

      StB2HoldCommit,
      StB2Check: begin
        cache_lram_wr_en = (state_q == StB2HoldCommit);
        cache_lram_wr_addr = CacheLineCollideAddr;
        cache_lram_wr_data = CacheLineNoise;
        cache_lram_rd_addr = CacheLineOtherAddr;
      end

      StB3LowArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_next_mask = 2'b01;
        cache_lram_wr_data = CacheLineLowOnly;
      end

      StB3LowRwSame,
      StB3Check: begin
        cache_lram_wr_en = (state_q == StB3LowRwSame);
        cache_lram_wr_en_mask = 2'b01;
        cache_lram_wr_addr = CacheLineCollideAddr;
        cache_lram_wr_data = CacheLineNoise;
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineCollideAddr;
      end

      StB4HighArm: begin
        cache_lram_wr_next = 1'b1;
        cache_lram_wr_next_mask = 2'b10;
        cache_lram_wr_data = CacheLineHighOnly;
      end

      StB4HighRwSame,
      StB4Check: begin
        cache_lram_wr_en = (state_q == StB4HighRwSame);
        cache_lram_wr_en_mask = 2'b10;
        cache_lram_wr_addr = CacheLineCollideAddr;
        cache_lram_wr_data = CacheLineNoise;
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineCollideAddr;
      end

      StX0FillArm: begin
        dcache_arr_s3_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayAddr;
        dcache_arr_write_req[DcacheArrayBank].data = CacheLineFill;
      end

      StX0FillCommit: begin
        dcache_arr_s4_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayAddr;
        dcache_arr_write_req[DcacheArrayBank].data = CacheLineNoise;
      end

      StX0DbgReadArm,
      StX0DbgReadCheck: begin
        dcache_arr_read_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_read_req[i].valid_l = 1'b1;
          dcache_arr_read_req[i].valid_h = 1'b1;
          dcache_arr_read_req[i].way = DcacheArrayWay;
          dcache_arr_read_req[i].addr = DcacheArrayAddr;
        end
      end

      StX1CoreReadArm,
      StX1CoreReadCheck: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ0MetaArrayArm: begin
        dcache_md_write_valid_prev = 1'b1;
        dcache_md_write_req.set_idx = DcacheLookupSet;
        dcache_md_write_req.way_en = minion_dcache_pkg::dcache_way_decode(DcacheArrayWay);
        dcache_md_write_req.data.coh_state = minion_dcache_pkg::StateModified;
        dcache_md_write_req.data.tag = DcacheLookupTag;
      end

      StZ0MetaArrayCommit: begin
        dcache_md_write_valid = 1'b1;
        dcache_md_read_set = DcacheLookupSet;
        dcache_md_write_req.set_idx = DcacheLookupSet;
        dcache_md_write_req.way_en = minion_dcache_pkg::dcache_way_decode(DcacheArrayWay);
        dcache_md_write_req.data.coh_state = minion_dcache_pkg::StateModified;
        dcache_md_write_req.data.tag = DcacheLookupTag;
      end

      StZ0MetaArrayCheck: begin
        dcache_md_read_set = DcacheLookupSet;
      end

      StZ1DataAllArm: begin
        dcache_arr_s3_write_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_write_req[i].valid_l = 1'b1;
          dcache_arr_write_req[i].valid_h = 1'b1;
          dcache_arr_write_req[i].way = DcacheArrayWay;
          dcache_arr_write_req[i].addr = DcacheArrayAddr;
          dcache_arr_write_req[i].data = dcache_bank_line(i);
        end
      end

      StZ1DataAllCommit: begin
        dcache_arr_s4_write_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_write_req[i].valid_l = 1'b1;
          dcache_arr_write_req[i].valid_h = 1'b1;
          dcache_arr_write_req[i].way = DcacheArrayWay;
          dcache_arr_write_req[i].addr = DcacheArrayAddr;
          dcache_arr_write_req[i].data = CacheLineNoise;
        end
      end

      StZ1DataAllReadArm,
      StZ1DataAllCheck: begin
        dcache_arr_read_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_read_req[i].valid_l = 1'b1;
          dcache_arr_read_req[i].valid_h = 1'b1;
          dcache_arr_read_req[i].way = DcacheArrayWay;
          dcache_arr_read_req[i].addr = DcacheArrayAddr;
        end
      end

      StZ2DataLowArm: begin
        dcache_arr_s3_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayAddr;
        dcache_arr_write_req[DcacheArrayBank].data = DcacheLowHalfLine;
      end

      StZ2DataLowCommit: begin
        dcache_arr_s4_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayAddr;
        dcache_arr_write_req[DcacheArrayBank].data = CacheLineNoise;
      end

      StZ2DataLowReadArm,
      StZ2DataLowCheck: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ3DataHighArm: begin
        dcache_arr_s3_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayAddr;
        dcache_arr_write_req[DcacheArrayBank].data = DcacheHighHalfLine;
      end

      StZ3DataHighCommit: begin
        dcache_arr_s4_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayAddr;
        dcache_arr_write_req[DcacheArrayBank].data = CacheLineNoise;
      end

      StZ3DataHighReadArm,
      StZ3DataHighCheck: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ4DataIsoArm: begin
        dcache_arr_s3_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayIsoWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayIsoAddr;
        dcache_arr_write_req[DcacheArrayBank].data = DcacheIsoLine;
      end

      StZ4DataIsoCommit: begin
        dcache_arr_s4_write_valid = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_write_req[DcacheArrayBank].way = DcacheArrayIsoWay;
        dcache_arr_write_req[DcacheArrayBank].addr = DcacheArrayIsoAddr;
        dcache_arr_write_req[DcacheArrayBank].data = CacheLineNoise;
      end

      StZ4DataIsoReadOrigArm,
      StZ4DataIsoReadOrigCheck: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ4DataIsoReadNewArm,
      StZ4DataIsoReadNewCheck: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayIsoWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayIsoAddr;
      end

      StZ5HitLaunch,
      StZ5HitCheck: begin
        dcache_md_read_set = DcacheLookupSet;
        dcache_arr_read_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_read_req[i].valid_l = 1'b1;
          dcache_arr_read_req[i].valid_h = 1'b1;
          dcache_arr_read_req[i].way = DcacheArrayWay;
          dcache_arr_read_req[i].addr = DcacheArrayAddr;
        end
      end

      StZ6HoldArm: begin
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineTestAddr;
      end

      StZ6HoldBlock,
      StZ6HoldCheck: begin
        cache_lram_rd_addr = CacheLineSeedAddr;
      end

      StZ6ResumeArm,
      StZ6ResumeCheck: begin
        cache_lram_rd_en = 1'b1;
        cache_lram_rd_addr = CacheLineSeedAddr;
      end

      StZ7TimingLaunch: begin
        dcache_timing_s1_en = 1'b1;
        dcache_md_read_set = DcacheLookupSet;
        dcache_arr_read_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_read_req[i].valid_l = 1'b1;
          dcache_arr_read_req[i].valid_h = 1'b1;
          dcache_arr_read_req[i].way = DcacheArrayWay;
          dcache_arr_read_req[i].addr = DcacheArrayAddr;
        end
      end

      StZ8ArrayHoldLaunch: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ8ArrayHoldDrop0,
      StZ8ArrayHoldCheck0: begin
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayIsoWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayIsoAddr;
      end

      StZ8ArrayResumeLaunch,
      StZ8ArrayResumeCheck: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayIsoWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayIsoAddr;
      end

      StZ8ArrayHoldDrop1,
      StZ8ArrayHoldCheck1: begin
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ9MissLaunch: begin
        dcache_miss_s1_en = 1'b1;
        dcache_md_read_set = DcacheLookupSet;
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayIsoWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayIsoAddr;
      end

      StZ10BtbLaunchOrig: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ10BtbSwitchIso: begin
        dcache_arr_read_valid = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayIsoWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayIsoAddr;
      end

      StZ10BtbCheckIso: begin
        dcache_arr_read_req[DcacheArrayBank].valid_l = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].valid_h = 1'b1;
        dcache_arr_read_req[DcacheArrayBank].way = DcacheArrayWay;
        dcache_arr_read_req[DcacheArrayBank].addr = DcacheArrayAddr;
      end

      StZ11SweepWriteArm: begin
        dcache_arr_s3_write_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_write_req[i].valid_l = 1'b1;
          dcache_arr_write_req[i].valid_h = 1'b1;
          dcache_arr_write_req[i].way = dcache_sweep_way(dcache_sweep_slot_q);
          dcache_arr_write_req[i].addr = dcache_sweep_addr(dcache_sweep_slot_q);
          dcache_arr_write_req[i].data = dcache_sweep_line(dcache_sweep_slot_q, i);
        end
      end

      StZ11SweepWriteCommit: begin
        dcache_arr_s4_write_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_write_req[i].valid_l = 1'b1;
          dcache_arr_write_req[i].valid_h = 1'b1;
          dcache_arr_write_req[i].way = dcache_sweep_way(dcache_sweep_slot_q);
          dcache_arr_write_req[i].addr = dcache_sweep_addr(dcache_sweep_slot_q);
          dcache_arr_write_req[i].data = CacheLineNoise;
        end
      end

      StZ11SweepReadArm,
      StZ11SweepReadCheck: begin
        dcache_arr_read_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_read_req[i].valid_l = 1'b1;
          dcache_arr_read_req[i].valid_h = 1'b1;
          dcache_arr_read_req[i].way = dcache_sweep_way(dcache_sweep_slot_q);
          dcache_arr_read_req[i].addr = dcache_sweep_addr(dcache_sweep_slot_q);
        end
      end

      StZ12PipeS0Launch: begin
        dcache_pipeline_s0_valid = 1'b1;
      end

      StZ12PipeS1Capture,
      StZ12PipeS2Check: begin
        dcache_md_read_set = dcache_pipeline_s1_set_q;
      end

      StZ13QualLaunch: begin
        dcache_qual_s0_valid = 1'b1;
      end

      StZ13BypassLaunch: begin
        dcache_qual_s0_valid = 1'b1;
        dcache_qual_s0_is_bypass = 1'b1;
      end

      StZ13QualS1Capture,
      StZ13QualS2Check,
      StZ13BypassS1Capture,
      StZ13BypassS2Check: begin
        dcache_md_read_set = dcache_qual_s1_set_q;
      end

      StZ14StressDecoyArm: begin
        dcache_md_write_valid_prev = 1'b1;
        dcache_md_write_req.set_idx = DcacheLookupSet;
        dcache_md_write_req.way_en = ~minion_dcache_pkg::dcache_way_decode(DcacheArrayWay);
        dcache_md_write_req.data.tag = DcacheDecoyTag;
        dcache_md_write_req.data.coh_state = minion_dcache_pkg::StateModified;
      end

      StZ14StressDecoyCommit: begin
        dcache_md_write_valid = 1'b1;
        dcache_md_write_req.set_idx = DcacheLookupSet;
        dcache_md_write_req.way_en = ~minion_dcache_pkg::dcache_way_decode(DcacheArrayWay);
        dcache_md_write_req.data.tag = DcacheDecoyTag;
        dcache_md_write_req.data.coh_state = minion_dcache_pkg::StateModified;
      end

      StZ14StressLaunch: begin
        dcache_stress_s0_valid = 1'b1;
        dcache_md_read_set = DcacheLookupSet;
        dcache_arr_read_valid = 1'b1;
        for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
          dcache_arr_read_req[i].valid_l = 1'b1;
          dcache_arr_read_req[i].valid_h = 1'b1;
          dcache_arr_read_req[i].way = DcacheArrayWay;
          dcache_arr_read_req[i].addr = DcacheArrayAddr;
        end
      end

      StZ14StressS1Capture,
      StZ14StressS2Check,
      StZ14StressS3Check: begin
        dcache_md_read_set = dcache_stress_s1_set_q;
      end

      default: begin
      end
    endcase
  end

  /* verilator lint_off SYNCASYNCNET */  // The harness reset is async locally and also feeds original-style synchronous-reset dcache metadata state.
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q <= StInit;
      msg_kind_q <= MsgBoot;
      msg_idx_q <= '0;
      error_count_q <= '0;
      preview_read0_q <= '0;
      preview_read1_q <= '0;
      preview_read2_q <= '0;
      preview_contract_read_q <= '0;
      preview_protocol_diag_q <= '0;
      diff_read0_q <= '0;
      diff_read1_q <= '0;
      diff_read2_q <= '0;
      diff_read3_q <= '0;
      front_read0_q <= '0;
      front_read1_q <= '0;
      front_read2_q <= '0;
      front_read3_q <= '0;
      front_read4_q <= '0;
      front_read5_q <= '0;
      reg_read0_q <= '0;
      reg_read1_q <= '0;
      reg_read2_q <= '0;
      dec_read0_q <= '0;
      dec_read1_q <= '0;
      dec_read2_q <= '0;
      par_read0_q <= '0;
      par_read1_q <= '0;
      par_read2_q <= '0;
      two_read0_q <= '0;
      two_read1_q <= '0;
      two_read2_q <= '0;
      two_read3_q <= '0;
      tri_read0_q <= '0;
      tri_read1_q <= '0;
      tri_read2_q <= '0;
      rq_read0_q <= '0;
      rq_read1_q <= '0;
      rq_read2_q <= '0;
      single_read0_q <= '0;
      single_read1_q <= '0;
      single_read2_q <= '0;
      chain_read0_q <= '0;
      chain_read1_q <= '0;
      chain_read2_q <= '0;
      dual_seed_diag_q <= '0;
      dual_block_diag_q <= '0;
      dual_rearm_diag_q <= '0;
      cache_meta_read0_q <= '0;
      cache_meta_read1_q <= '0;
      cache_lram_read0_q <= '0;
      cache_lram_read1_q <= '0;
      cache_lram_read2_q <= '0;
      cache_lram_rw_same_q <= '0;
      cache_lram_rw_diff_q <= '0;
      cache_lram_hold_write_q <= '0;
      cache_lram_low_collision_q <= '0;
      cache_lram_high_collision_q <= '0;
      dcache_arr_dbg_read_q <= '0;
      dcache_arr_core_read_q <= '0;
      dcache_md_array_diag_q <= '0;
      dcache_data_all_diag_q <= '0;
      dcache_data_low_diag_q <= '0;
      dcache_data_high_diag_q <= '0;
      dcache_data_iso_diag_q <= '0;
      dcache_leaf_hit_diag_q <= '0;
      cache_lram_read_gate_diag_q <= '0;
      dcache_timing_diag_q <= '0;
      dcache_array_hold_diag_q <= '0;
      dcache_miss_suppress_diag_q <= '0;
      dcache_back_to_back_diag_q <= '0;
      dcache_sweep_diag_q <= '0;
      dcache_pipeline_diag_q <= '0;
      dcache_qual_diag_q <= '0;
      dcache_stress_diag_q <= '0;
      dcache_sweep_slot_q <= '0;
      dcache_sweep_seen_bank_q <= '0;
      dcache_sweep_seen_way_q <= '0;
      dcache_sweep_seen_set_q <= '0;
      dcache_sweep_seen_idx_q <= '0;
      dcache_sweep_fail_q <= 1'b0;
    end else begin
      unique case (state_q)
        StInit: begin
          state_q <= StP0Write;
        end

        StP0Write: begin
          state_q <= StP0Check;
        end

        StP0Check: begin
          preview_read0_q <= preview_rd_data;
          if (preview_rd_data != PreviewSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StP1Write;
        end

        StP1Write: begin
          state_q <= StP1Check;
        end

        StP1Check: begin
          preview_read1_q <= preview_rd_data;
          if (preview_rd_data != PreviewSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StP2Arm;
        end

        StP2Arm: begin
          state_q <= StP2Commit;
        end

        StP2Commit: begin
          state_q <= StP2Check;
        end

        StP2Check: begin
          preview_read2_q <= preview_rd_data;
          if (preview_rd_data != PreviewSplit) error_count_q <= error_count_q + 8'd1;
          state_q <= StP3SeedArm;
        end

        StP3SeedArm: begin
          state_q <= StP3SeedCommit;
        end

        StP3SeedCommit: begin
          state_q <= StP3Disarm;
        end

        StP3Disarm: begin
          state_q <= StP3BlockedArm;
        end

        StP3BlockedArm: begin
          state_q <= StP3BlockedCommit;
        end

        StP3BlockedCommit: begin
          state_q <= StP3Check;
        end

        StP3Check: begin
          preview_contract_read_q <= preview_contract_rd_data;
          if (preview_contract_rd_data != PreviewSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StP4Seed0Arm;
        end

        StP4Seed0Arm: begin
          preview_protocol_diag_q <= '0;
          state_q <= StP4Seed0Commit;
        end

        StP4Seed0Commit: begin
          state_q <= StP4Seed1Arm;
        end

        StP4Seed1Arm: begin
          state_q <= StP4Seed1Commit;
        end

        StP4Seed1Commit: begin
          state_q <= StP4Seed2Arm;
        end

        StP4Seed2Arm: begin
          state_q <= StP4Seed2Commit;
        end

        StP4Seed2Commit: begin
          state_q <= StP4Seed3Arm;
        end

        StP4Seed3Arm: begin
          state_q <= StP4Seed3Commit;
        end

        StP4Seed3Commit: begin
          state_q <= StP4PreviewOnlyArm;
        end

        StP4PreviewOnlyArm: begin
          state_q <= StP4PreviewOnlyCheck;
        end

        StP4PreviewOnlyCheck: begin
          if (preview_contract_rd_data == 8'h11) begin
            preview_protocol_diag_q[0] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StP4CommitOnly;
        end

        StP4CommitOnly: begin
          state_q <= StP4CommitOnlyCheck;
        end

        StP4CommitOnlyCheck: begin
          if (preview_contract_rd_data == 8'h21) begin
            preview_protocol_diag_q[1] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StP4DataArm;
        end

        StP4DataArm: begin
          state_q <= StP4DataCommit;
        end

        StP4DataCommit: begin
          state_q <= StP4DataCheck;
        end

        StP4DataCheck: begin
          if (preview_contract_rd_data == 8'h32) begin
            preview_protocol_diag_q[2] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StP4DelayArm;
        end

        StP4DelayArm: begin
          state_q <= StP4DelayWait;
        end

        StP4DelayWait: begin
          state_q <= StP4DelayCommit;
        end

        StP4DelayCommit: begin
          state_q <= StP4DelayCheck;
        end

        StP4DelayCheck: begin
          if (preview_contract_rd_data == 8'h43) begin
            preview_protocol_diag_q[3] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StP4AddrArm;
        end

        StP4AddrArm: begin
          state_q <= StP4AddrCommit;
        end

        StP4AddrCommit: begin
          state_q <= StP4AddrCheck;
        end

        StP4AddrCheck: begin
          if (preview_contract_rd_data == 8'h54) begin
            preview_protocol_diag_q[4] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StP4HoldArm;
        end

        StP4HoldArm: begin
          state_q <= StP4HoldCommit;
        end

        StP4HoldCommit: begin
          state_q <= StP4HoldCheck;
        end

        StP4HoldCheck: begin
          if (preview_contract_rd_data == 8'h54) begin
            preview_protocol_diag_q[5] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StP4MultiArmA;
        end

        StP4MultiArmA: begin
          state_q <= StP4MultiArmB;
        end

        StP4MultiArmB: begin
          state_q <= StP4MultiCommit;
        end

        StP4MultiCommit: begin
          state_q <= StP4MultiCheck;
        end

        StP4MultiCheck: begin
          if (preview_contract_rd_data == 8'h87) begin
            preview_protocol_diag_q[6] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StP4FinalCheck;
        end

        StP4FinalCheck: begin
          if (preview_contract_rd_data == 8'h54) begin
            preview_protocol_diag_q[7] <= 1'b1;
          end else begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StD0Write;
        end

        StD0Write: begin
          state_q <= StD0Check;
        end

        StD0Check: begin
          diff_read0_q <= diff_rd_data;
          if (diff_rd_data != 8'h12) error_count_q <= error_count_q + 8'd1;
          state_q <= StD1Write;
        end

        StD1Write: begin
          state_q <= StD1Check;
        end

        StD1Check: begin
          diff_read1_q <= diff_rd_data;
          if (diff_rd_data != 8'hCD) error_count_q <= error_count_q + 8'd1;
          state_q <= StD2Check;
        end

        StD2Check: begin
          diff_read2_q <= diff_rd_data;
          if (diff_rd_data != 8'h34) error_count_q <= error_count_q + 8'd1;
          state_q <= StD3Arm;
        end

        StD3Arm: begin
          state_q <= StD3Commit;
        end

        StD3Commit: begin
          state_q <= StD3Check;
        end

        StD3Check: begin
          diff_read3_q <= diff_rd_data;
          if (diff_rd_data != 8'hBB) error_count_q <= error_count_q + 8'd1;
          state_q <= StF0Arm;
        end

        StF0Arm: begin
          state_q <= StF0Commit;
        end

        StF0Commit: begin
          state_q <= StF0Check;
        end

        StF0Check: begin
          front_read0_q <= front_rd_data;
          if (front_rd_data != FrontendWord0) error_count_q <= error_count_q + 8'd1;
          state_q <= StF1Check;
        end

        StF1Check: begin
          front_read1_q <= front_rd_data;
          if (front_rd_data != FrontendOddReadExpected) error_count_q <= error_count_q + 8'd1;
          state_q <= StF2Arm;
        end

        StF2Arm: begin
          state_q <= StF2Commit;
        end

        StF2Commit: begin
          state_q <= StF2Check;
        end

        StF2Check: begin
          front_read2_q <= front_rd_data;
          if (front_rd_data != FrontendWrapReadExpected) error_count_q <= error_count_q + 8'd1;
          state_q <= StF3Arm;
        end

        StF3Arm: begin
          state_q <= StF3Commit;
        end

        StF3Commit: begin
          state_q <= StF3Check;
        end

        StF3Check: begin
          front_read3_q <= front_rd_data;
          if (front_rd_data != FrontendPartialWord) error_count_q <= error_count_q + 8'd1;
          state_q <= StF4LateArm;
        end

        StF4LateArm: begin
          state_q <= StF4LateCommit;
        end

        StF4LateCommit: begin
          state_q <= StF4LateRead0;
        end

        StF4LateRead0: begin
          state_q <= StF4LateCheck0;
        end

        StF4LateCheck0: begin
          front_read4_q <= front_rd_data;
          if (front_rd_data != FrontendLateNoBypassExpected) error_count_q <= error_count_q + 8'd1;
          state_q <= StF5LateRead1;
        end

        StF5LateRead1: begin
          state_q <= StF5LateCheck1;
        end

        StF5LateCheck1: begin
          front_read5_q <= front_rd_data;
          if (front_rd_data != FrontendLateNoBypassExpected) error_count_q <= error_count_q + 8'd1;
          state_q <= StR0Write;
        end

        StR0Write: begin
          state_q <= StR0ReadArm;
        end

        StR0ReadArm: begin
          state_q <= StR0Check;
        end

        StR0Check: begin
          reg_read0_q <= reg_rd_data;
          if (reg_rd_data != RegSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StR1Write;
        end

        StR1Write: begin
          state_q <= StR1ReadArm;
        end

        StR1ReadArm: begin
          state_q <= StR1Check;
        end

        StR1Check: begin
          reg_read1_q <= reg_rd_data;
          if (reg_rd_data != RegSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StR2Arm;
        end

        StR2Arm: begin
          state_q <= StR2Commit;
        end

        StR2Commit: begin
          state_q <= StR2ReadArm;
        end

        StR2ReadArm: begin
          state_q <= StR2Check;
        end

        StR2Check: begin
          reg_read2_q <= reg_rd_data;
          if (reg_rd_data != RegSplit) error_count_q <= error_count_q + 8'd1;
          state_q <= StC0Arm;
        end

        StC0Arm: begin
          state_q <= StC0Commit;
        end

        StC0Commit: begin
          state_q <= StC0Check;
        end

        StC0Check: begin
          dec_read0_q <= dec_rd_data;
          if (dec_rd_data != DecSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StC1Write;
        end

        StC1Write: begin
          state_q <= StC1Check;
        end

        StC1Check: begin
          dec_read1_q <= dec_rd_data;
          if (dec_rd_data != DecSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StC2Arm;
        end

        StC2Arm: begin
          state_q <= StC2Commit;
        end

        StC2Commit: begin
          state_q <= StC2Check;
        end

        StC2Check: begin
          dec_read2_q <= dec_rd_data;
          if (dec_rd_data != DecRearm) error_count_q <= error_count_q + 8'd1;
          state_q <= StA0Arm;
        end

        StA0Arm: begin
          state_q <= StA0Commit;
        end

        StA0Commit: begin
          state_q <= StA0Check;
        end

        StA0Check: begin
          par_read0_q <= par_rd_data;
          if (par_rd_data != ParSeedPacked) error_count_q <= error_count_q + 8'd1;
          state_q <= StA1Write;
        end

        StA1Write: begin
          state_q <= StA1Check;
        end

        StA1Check: begin
          par_read1_q <= par_rd_data;
          if (par_rd_data != ParStalePacked) error_count_q <= error_count_q + 8'd1;
          state_q <= StA2Arm;
        end

        StA2Arm: begin
          state_q <= StA2Commit;
        end

        StA2Commit: begin
          state_q <= StA2Check;
        end

        StA2Check: begin
          par_read2_q <= par_rd_data;
          if (par_rd_data != ParRearmPacked) error_count_q <= error_count_q + 8'd1;
          state_q <= StT0Arm;
        end

        StT0Arm: begin
          state_q <= StT0Commit;
        end

        StT0Commit: begin
          state_q <= StT0Check;
        end

        StT0Check: begin
          two_read0_q <= two_rd_data_a;
          if (two_rd_data_a != TwoSeed) error_count_q <= error_count_q + 8'd1;
          if (two_rd_data_b != TwoSeed) error_count_q <= error_count_q + 8'd1;
          if (two_rd_par != 16'h0000) error_count_q <= error_count_q + 8'd1;
          state_q <= StT1Write;
        end

        StT1Write: begin
          state_q <= StT1Check;
        end

        StT1Check: begin
          two_read1_q <= two_rd_data_a;
          if (two_rd_data_a != TwoSeed) error_count_q <= error_count_q + 8'd1;
          if (two_rd_data_b != TwoSeed) error_count_q <= error_count_q + 8'd1;
          if (two_rd_par != 16'h0000) error_count_q <= error_count_q + 8'd1;
          state_q <= StT2Arm;
        end

        StT2Arm: begin
          state_q <= StT2Commit;
        end

        StT2Commit: begin
          state_q <= StT2Check;
        end

        StT2Check: begin
          two_read2_q <= two_rd_data_a;
          if (two_rd_data_a != 16'h0000) error_count_q <= error_count_q + 8'd1;
          if (two_rd_data_b != 16'h0000) error_count_q <= error_count_q + 8'd1;
          if (two_rd_par != 16'h0000) error_count_q <= error_count_q + 8'd1;
          state_q <= StT3Arm;
        end

        StT3Arm: begin
          state_q <= StT3Commit;
        end

        StT3Commit: begin
          state_q <= StT3Check;
        end

        StT3Check: begin
          two_read3_q <= two_rd_par;
          if (two_rd_data_a != TwoRearm) error_count_q <= error_count_q + 8'd1;
          if (two_rd_data_b != TwoSeed) error_count_q <= error_count_q + 8'd1;
          if (two_rd_par != TwoRearm) error_count_q <= error_count_q + 8'd1;
          state_q <= StV0Arm;
        end

        StV0Arm: begin
          state_q <= StV0Commit;
        end

        StV0Commit: begin
          state_q <= StV0Check;
        end

        StV0Check: begin
          tri_read0_q <= tri_rd_data_a;
          if (tri_rd_data_a != TriSeedA) error_count_q <= error_count_q + 8'd1;
          if (tri_rd_data_b != TriSeedB) error_count_q <= error_count_q + 8'd1;
          if (tri_rd_data_c != TriSeedA) error_count_q <= error_count_q + 8'd1;
          state_q <= StV1Write;
        end

        StV1Write: begin
          state_q <= StV1Check;
        end

        StV1Check: begin
          tri_read1_q <= tri_rd_data_a;
          if (tri_rd_data_a != TriSeedA) error_count_q <= error_count_q + 8'd1;
          if (tri_rd_data_b != TriSeedB) error_count_q <= error_count_q + 8'd1;
          if (tri_rd_data_c != TriSeedA) error_count_q <= error_count_q + 8'd1;
          state_q <= StV2Arm;
        end

        StV2Arm: begin
          state_q <= StV2Commit;
        end

        StV2Commit: begin
          state_q <= StV2Check;
        end

        StV2Check: begin
          tri_read2_q <= tri_rd_data_a;
          if (tri_rd_data_a != TriRearmA) error_count_q <= error_count_q + 8'd1;
          if (tri_rd_data_b != TriRearmA) error_count_q <= error_count_q + 8'd1;
          if (tri_rd_data_c != TriRearmA) error_count_q <= error_count_q + 8'd1;
          state_q <= StQ0Arm;
        end

        StQ0Arm: begin
          state_q <= StQ0Commit;
        end

        StQ0Commit: begin
          state_q <= StQ0Check;
        end

        StQ0Check: begin
          rq_read0_q <= rq_rd_data;
          if (rq_rd_data != QueuePacked0) error_count_q <= error_count_q + 8'd1;
          state_q <= StQ1Arm;
        end

        StQ1Arm: begin
          state_q <= StQ1Commit;
        end

        StQ1Commit: begin
          state_q <= StQ1Check;
        end

        StQ1Check: begin
          rq_read1_q <= rq_rd_data;
          if (rq_rd_data != QueuePacked1) error_count_q <= error_count_q + 8'd1;
          state_q <= StQ2Write;
        end

        StQ2Write: begin
          state_q <= StQ2Check;
        end

        StQ2Check: begin
          rq_read2_q <= rq_rd_data;
          if (rq_rd_data != QueuePacked2) error_count_q <= error_count_q + 8'd1;
          state_q <= StS0Arm;
        end

        StS0Arm: begin
          state_q <= StS0Commit;
        end

        StS0Commit: begin
          state_q <= StS0Check;
        end

        StS0Check: begin
          single_read0_q <= single_rd_data;
          if (single_rd_data != SingleSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StS1Write;
        end

        StS1Write: begin
          state_q <= StS1Check;
        end

        StS1Check: begin
          single_read1_q <= single_rd_data;
          if (single_rd_data != SingleSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StS2Arm;
        end

        StS2Arm: begin
          state_q <= StS2Commit;
        end

        StS2Commit: begin
          state_q <= StS2Check;
        end

        StS2Check: begin
          single_read2_q <= single_rd_data;
          if (single_rd_data != SingleRearm) error_count_q <= error_count_q + 8'd1;
          state_q <= StW0Arm;
        end

        StW0Arm: begin
          state_q <= StW0Stage;
        end

        StW0Stage: begin
          state_q <= StW0Check;
        end

        StW0Check: begin
          chain_read0_q <= chain_wdata_1p;
          if (chain_wdata_1p != ChainSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StW1Check;
        end

        StW1Check: begin
          chain_read1_q <= chain_wdata_1p;
          if (chain_wdata_1p != ChainSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StW2Arm;
        end

        StW2Arm: begin
          state_q <= StW2Stage;
        end

        StW2Stage: begin
          state_q <= StW2Check;
        end

        StW2Check: begin
          chain_read2_q <= chain_wdata_1p;
          if (chain_wdata_1p != ChainRearm) error_count_q <= error_count_q + 8'd1;
          state_q <= StY0SeedArm;
        end

        StY0SeedArm: begin
          dual_seed_diag_q <= '0;
          dual_block_diag_q <= '0;
          dual_rearm_diag_q <= '0;
          state_q <= StY0SeedCommit;
        end

        StY0SeedCommit: begin
          state_q <= StY0SeedReadArm;
        end

        StY0SeedReadArm: begin
          state_q <= StY0SeedCheck;
        end

        StY0SeedCheck: begin
          dual_seed_diag_q <= dual_seed_diag;
          if (dual_seed_diag != DualDiagExpected) error_count_q <= error_count_q + 8'd1;
          state_q <= StY1BlockArm;
        end

        StY1BlockArm: begin
          state_q <= StY1BlockCommit;
        end

        StY1BlockCommit: begin
          state_q <= StY1BlockReadArm;
        end

        StY1BlockReadArm: begin
          state_q <= StY1BlockCheck;
        end

        StY1BlockCheck: begin
          dual_block_diag_q <= dual_block_diag;
          if (dual_block_diag != DualDiagExpected) error_count_q <= error_count_q + 8'd1;
          state_q <= StY2RearmArm;
        end

        StY2RearmArm: begin
          state_q <= StY2RearmCommit;
        end

        StY2RearmCommit: begin
          state_q <= StY2RearmReadArm;
        end

        StY2RearmReadArm: begin
          state_q <= StY2RearmCheck;
        end

        StY2RearmCheck: begin
          dual_rearm_diag_q <= dual_rearm_diag;
          if (dual_rearm_diag != DualDiagExpected) error_count_q <= error_count_q + 8'd1;
          state_q <= StM0Arm;
        end

        StM0Arm: begin
          state_q <= StM0Commit;
        end

        StM0Commit: begin
          state_q <= StM0Check;
        end

        StM0Check: begin
          cache_meta_read0_q <= cache_meta_rd_data;
          if (cache_meta_rd_data != CacheMetaFill) error_count_q <= error_count_q + 8'd1;
          state_q <= StM1Arm;
        end

        StM1Arm: begin
          state_q <= StM1Commit;
        end

        StM1Commit: begin
          state_q <= StM1Bubble;
        end

        StM1Bubble: begin
          state_q <= StM1Check;
        end

        StM1Check: begin
          cache_meta_read1_q <= cache_meta_rd_data;
          if (cache_meta_rd_data != CacheMetaStore) error_count_q <= error_count_q + 8'd1;
          state_q <= StL0SentinelArm;
        end

        StL0SentinelArm: begin
          state_q <= StL0SentinelCommit;
        end

        StL0SentinelCommit: begin
          state_q <= StL0FillArm;
        end

        StL0FillArm: begin
          state_q <= StL0FillCommit;
        end

        StL0FillCommit: begin
          state_q <= StL0ReadArm;
        end

        StL0ReadArm: begin
          state_q <= StL0Check;
        end

        StL0Check: begin
          cache_lram_read0_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineFill) error_count_q <= error_count_q + 8'd1;
          state_q <= StL1StoreArm;
        end

        StL1StoreArm: begin
          state_q <= StL1StoreCommit;
        end

        StL1StoreCommit: begin
          state_q <= StL1Bubble;
        end

        StL1Bubble: begin
          state_q <= StL1ReadArm;
        end

        StL1ReadArm: begin
          state_q <= StL1Check;
        end

        StL1Check: begin
          cache_lram_read1_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineStore) error_count_q <= error_count_q + 8'd1;
          state_q <= StL2ReadArm;
        end

        StL2ReadArm: begin
          state_q <= StL2Check;
        end

        StL2Check: begin
          cache_lram_read2_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineSeed) error_count_q <= error_count_q + 8'd1;
          state_q <= StB0BaseArm;
        end

        StB0BaseArm: begin
          state_q <= StB0BaseCommit;
        end

        StB0BaseCommit: begin
          state_q <= StB1BaseArm;
        end

        StB1BaseArm: begin
          state_q <= StB1BaseCommit;
        end

        StB1BaseCommit: begin
          state_q <= StB0NewArm;
        end

        StB0NewArm: begin
          state_q <= StB0RwSame;
        end

        StB0RwSame: begin
          state_q <= StB0Check;
        end

        StB0Check: begin
          cache_lram_rw_same_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineRwSame) error_count_q <= error_count_q + 8'd1;
          state_q <= StB1NewArm;
        end

        StB1NewArm: begin
          state_q <= StB1RwDiff;
        end

        StB1RwDiff: begin
          state_q <= StB1Check;
        end

        StB1Check: begin
          cache_lram_rw_diff_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineOtherOld) error_count_q <= error_count_q + 8'd1;
          state_q <= StB2ReadArm;
        end

        StB2ReadArm: begin
          state_q <= StB2HoldArm;
        end

        StB2HoldArm: begin
          state_q <= StB2HoldCommit;
        end

        StB2HoldCommit: begin
          state_q <= StB2Check;
        end

        StB2Check: begin
          cache_lram_hold_write_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineHoldWrite) error_count_q <= error_count_q + 8'd1;
          state_q <= StB3LowArm;
        end

        StB3LowArm: begin
          state_q <= StB3LowRwSame;
        end

        StB3LowRwSame: begin
          state_q <= StB3Check;
        end

        StB3Check: begin
          cache_lram_low_collision_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineLowCollision) error_count_q <= error_count_q + 8'd1;
          state_q <= StB4HighArm;
        end

        StB4HighArm: begin
          state_q <= StB4HighRwSame;
        end

        StB4HighRwSame: begin
          state_q <= StB4Check;
        end

        StB4Check: begin
          cache_lram_high_collision_q <= cache_lram_rd_data;
          if (cache_lram_rd_data != CacheLineHighCollision) error_count_q <= error_count_q + 8'd1;
          state_q <= StX0FillArm;
        end

        StX0FillArm: begin
          state_q <= StX0FillCommit;
        end

        StX0FillCommit: begin
          state_q <= StX0DbgReadArm;
        end

        StX0DbgReadArm: begin
          state_q <= StX0DbgReadCheck;
        end

        StX0DbgReadCheck: begin
          dcache_arr_dbg_read_q <= dcache_arr_read_resp[DcacheArrayBank];
          if (dcache_arr_read_resp[DcacheArrayBank] != CacheLineFill) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StX1CoreReadArm;
        end

        StX1CoreReadArm: begin
          state_q <= StX1CoreReadCheck;
        end

        StX1CoreReadCheck: begin
          dcache_arr_core_read_q <= dcache_arr_read_resp[DcacheArrayBank];
          if (dcache_arr_read_resp[DcacheArrayBank] != CacheLineFill) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ0MetaArrayArm;
        end

        StZ0MetaArrayArm: begin
          state_q <= StZ0MetaArrayCommit;
        end

        StZ0MetaArrayCommit: begin
          state_q <= StZ0MetaArrayCheck;
        end

        StZ0MetaArrayCheck: begin
          dcache_md_array_diag_q <= dcache_md_array_diag;
          if (dcache_md_array_diag != DcacheMetaArrayDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ1DataAllArm;
        end

        StZ1DataAllArm: begin
          state_q <= StZ1DataAllCommit;
        end

        StZ1DataAllCommit: begin
          state_q <= StZ1DataAllReadArm;
        end

        StZ1DataAllReadArm: begin
          state_q <= StZ1DataAllCheck;
        end

        StZ1DataAllCheck: begin
          dcache_data_all_diag_q <= dcache_data_all_diag;
          if (dcache_data_all_diag != DcacheDataAllDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ2DataLowArm;
        end

        StZ2DataLowArm: begin
          state_q <= StZ2DataLowCommit;
        end

        StZ2DataLowCommit: begin
          state_q <= StZ2DataLowReadArm;
        end

        StZ2DataLowReadArm: begin
          state_q <= StZ2DataLowCheck;
        end

        StZ2DataLowCheck: begin
          dcache_data_low_diag_q <= dcache_data_low_diag;
          if (dcache_data_low_diag != DcacheDataHalfDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ3DataHighArm;
        end

        StZ3DataHighArm: begin
          state_q <= StZ3DataHighCommit;
        end

        StZ3DataHighCommit: begin
          state_q <= StZ3DataHighReadArm;
        end

        StZ3DataHighReadArm: begin
          state_q <= StZ3DataHighCheck;
        end

        StZ3DataHighCheck: begin
          dcache_data_high_diag_q <= dcache_data_high_diag;
          if (dcache_data_high_diag != DcacheDataHalfDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_data_iso_diag_q <= '0;
          state_q <= StZ4DataIsoArm;
        end

        StZ4DataIsoArm: begin
          state_q <= StZ4DataIsoCommit;
        end

        StZ4DataIsoCommit: begin
          state_q <= StZ4DataIsoReadOrigArm;
        end

        StZ4DataIsoReadOrigArm: begin
          state_q <= StZ4DataIsoReadOrigCheck;
        end

        StZ4DataIsoReadOrigCheck: begin
          dcache_data_iso_diag_q[0] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheHighHalfExpected);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheHighHalfExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ4DataIsoReadNewArm;
        end

        StZ4DataIsoReadNewArm: begin
          state_q <= StZ4DataIsoReadNewCheck;
        end

        StZ4DataIsoReadNewCheck: begin
          dcache_data_iso_diag_q[1] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheIsoLine);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheIsoLine) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ5HitLaunch;
        end

        StZ5HitLaunch: begin
          state_q <= StZ5HitCheck;
        end

        StZ5HitCheck: begin
          dcache_leaf_hit_diag_q <= dcache_leaf_hit_diag;
          if (dcache_leaf_hit_diag != DcacheLeafHitDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          cache_lram_read_gate_diag_q <= '0;
          state_q <= StZ6HoldArm;
        end

        StZ6HoldArm: begin
          state_q <= StZ6HoldBlock;
        end

        StZ6HoldBlock: begin
          state_q <= StZ6HoldCheck;
        end

        StZ6HoldCheck: begin
          cache_lram_read_gate_diag_q[0] <= (cache_lram_rd_data == CacheLineStore);
          if (cache_lram_rd_data != CacheLineStore) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ6ResumeArm;
        end

        StZ6ResumeArm: begin
          state_q <= StZ6ResumeCheck;
        end

        StZ6ResumeCheck: begin
          cache_lram_read_gate_diag_q[1] <= (cache_lram_rd_data == CacheLineSeed);
          if (cache_lram_rd_data != CacheLineSeed) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_timing_diag_q <= '0;
          state_q <= StZ7TimingLaunch;
        end

        StZ7TimingLaunch: begin
          state_q <= StZ7TimingS2Check;
        end

        StZ7TimingS2Check: begin
          dcache_timing_diag_q[0] <= dcache_timing_s2_valid_q;
          dcache_timing_diag_q[1] <= dcache_timing_s2_tag_match_q;
          dcache_timing_diag_q[2] <= dcache_timing_s2_state_hit_q;
          dcache_timing_diag_q[3] <= dcache_timing_s2_hit;
          dcache_timing_diag_q[4] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheHighHalfExpected);
          state_q <= StZ7TimingS3Check;
        end

        StZ7TimingS3Check: begin
          dcache_timing_diag_q <= dcache_timing_final_diag;
          if (dcache_timing_final_diag != DcacheTimingDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_array_hold_diag_q <= '0;
          state_q <= StZ8ArrayHoldLaunch;
        end

        StZ8ArrayHoldLaunch: begin
          state_q <= StZ8ArrayHoldDrop0;
        end

        StZ8ArrayHoldDrop0: begin
          dcache_array_hold_diag_q[0] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheHighHalfExpected);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheHighHalfExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ8ArrayHoldCheck0;
        end

        StZ8ArrayHoldCheck0: begin
          dcache_array_hold_diag_q[1] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheHighHalfExpected);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheHighHalfExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ8ArrayResumeLaunch;
        end

        StZ8ArrayResumeLaunch: begin
          state_q <= StZ8ArrayResumeCheck;
        end

        StZ8ArrayResumeCheck: begin
          dcache_array_hold_diag_q[2] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheIsoLine);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheIsoLine) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ8ArrayHoldDrop1;
        end

        StZ8ArrayHoldDrop1: begin
          dcache_array_hold_diag_q[3] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheIsoLine);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheIsoLine) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ8ArrayHoldCheck1;
        end

        StZ8ArrayHoldCheck1: begin
          dcache_miss_suppress_diag_q <= '0;
          state_q <= StZ9MissLaunch;
        end

        StZ9MissLaunch: begin
          state_q <= StZ9MissS2Check;
        end

        StZ9MissS2Check: begin
          dcache_miss_suppress_diag_q[0] <= dcache_miss_s2_valid_q;
          dcache_miss_suppress_diag_q[1] <= !dcache_miss_s2_hit_q;
          dcache_miss_suppress_diag_q[2] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheIsoLine);
          dcache_miss_suppress_diag_q[3] <= !(dcache_miss_s2_valid_q && dcache_miss_s2_hit_q);
          if (!dcache_miss_s2_valid_q || dcache_miss_s2_hit_q ||
              (dcache_arr_read_resp[DcacheArrayBank] != DcacheIsoLine)) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ9MissS3Check;
        end

        StZ9MissS3Check: begin
          dcache_miss_suppress_diag_q[4] <= !dcache_miss_s3_valid_q;
          dcache_miss_suppress_diag_q[5] <= (dcache_miss_s3_data_q == '0);
          if (dcache_miss_s3_valid_q || (dcache_miss_s3_data_q != '0)) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_back_to_back_diag_q <= '0;
          state_q <= StZ10BtbLaunchOrig;
        end

        StZ10BtbLaunchOrig: begin
          state_q <= StZ10BtbSwitchIso;
        end

        StZ10BtbSwitchIso: begin
          dcache_back_to_back_diag_q[0] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheHighHalfExpected);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheHighHalfExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ10BtbCheckIso;
        end

        StZ10BtbCheckIso: begin
          dcache_back_to_back_diag_q[1] <= (dcache_arr_read_resp[DcacheArrayBank] == DcacheIsoLine);
          if (dcache_arr_read_resp[DcacheArrayBank] != DcacheIsoLine) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_sweep_diag_q <= '0;
          dcache_sweep_slot_q <= '0;
          dcache_sweep_seen_bank_q <= '0;
          dcache_sweep_seen_way_q <= '0;
          dcache_sweep_seen_set_q <= '0;
          dcache_sweep_seen_idx_q <= '0;
          dcache_sweep_fail_q <= 1'b0;
          state_q <= StZ11SweepWriteArm;
        end

        StZ11SweepWriteArm: begin
          state_q <= StZ11SweepWriteCommit;
        end

        StZ11SweepWriteCommit: begin
          state_q <= StZ11SweepReadArm;
        end

        StZ11SweepReadArm: begin
          state_q <= StZ11SweepReadCheck;
        end

        StZ11SweepReadCheck: begin
          dcache_sweep_seen_bank_q <= 4'hF;
          dcache_sweep_seen_way_q <= dcache_sweep_seen_way_q | dcache_sweep_way_mask(dcache_sweep_slot_q);
          dcache_sweep_seen_set_q <= dcache_sweep_seen_set_q | dcache_sweep_set_mask(dcache_sweep_slot_q[1:0]);
          dcache_sweep_seen_idx_q <= dcache_sweep_seen_idx_q |
                                      dcache_sweep_idx_mask(dcache_sweep_slot_q[2] ^ dcache_sweep_slot_q[0]);
          dcache_sweep_fail_q <= dcache_sweep_fail_q || !dcache_sweep_read_ok;
          if (dcache_sweep_slot_q == 3'd7) begin
            state_q <= StZ11SweepDone;
          end else begin
            dcache_sweep_slot_q <= dcache_sweep_slot_q + 3'd1;
            state_q <= StZ11SweepWriteArm;
          end
        end

        StZ11SweepDone: begin
          dcache_sweep_diag_q <= dcache_sweep_final_diag;
          if (dcache_sweep_final_diag != DcacheSweepDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_pipeline_diag_q <= '0;
          state_q <= StZ12PipeS0Launch;
        end

        StZ12PipeS0Launch: begin
          state_q <= StZ12PipeS1Capture;
        end

        StZ12PipeS1Capture: begin
          state_q <= StZ12PipeS2Check;
        end

        StZ12PipeS2Check: begin
          dcache_pipeline_diag_q <= dcache_pipeline_final_diag;
          if (dcache_pipeline_final_diag != DcachePipelineDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_qual_diag_q <= '0;
          state_q <= StZ13QualLaunch;
        end

        StZ13QualLaunch: begin
          state_q <= StZ13QualS1Capture;
        end

        StZ13QualS1Capture: begin
          state_q <= StZ13QualS2Check;
        end

        StZ13QualS2Check: begin
          dcache_qual_diag_q[6:0] <= dcache_qual_hit_diag;
          if (dcache_qual_hit_diag != 7'h7F) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StZ13BypassLaunch;
        end

        StZ13BypassLaunch: begin
          state_q <= StZ13BypassS1Capture;
        end

        StZ13BypassS1Capture: begin
          state_q <= StZ13BypassS2Check;
        end

        StZ13BypassS2Check: begin
          dcache_qual_diag_q <= {dcache_qual_bypass_suppressed, dcache_qual_diag_q[6:0]};
          if ({dcache_qual_bypass_suppressed, dcache_qual_diag_q[6:0]} != DcacheQualDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          dcache_stress_diag_q <= '0;
          state_q <= StZ14StressDecoyArm;
        end

        StZ14StressDecoyArm: begin
          state_q <= StZ14StressDecoyCommit;
        end

        StZ14StressDecoyCommit: begin
          state_q <= StZ14StressLaunch;
        end

        StZ14StressLaunch: begin
          state_q <= StZ14StressS1Capture;
        end

        StZ14StressS1Capture: begin
          state_q <= StZ14StressS2Check;
        end

        StZ14StressS2Check: begin
          state_q <= StZ14StressS3Check;
        end

        StZ14StressS3Check: begin
          dcache_stress_diag_q <= dcache_stress_final_diag;
          if (dcache_stress_final_diag != DcacheStressDiagExpected) begin
            error_count_q <= error_count_q + 8'd1;
          end
          state_q <= StPrint;
          msg_kind_q <= MsgBoot;
          msg_idx_q <= '0;
        end

        StPrint: begin
          if (tx_fire) begin
            if (msg_idx_q == msg_last_idx(msg_kind_q)) begin
              if (msg_kind_q == MsgStatus) begin
                state_q <= StDone;
              end else begin
                msg_kind_q <= next_msg_kind(msg_kind_q);
                msg_idx_q <= '0;
              end
            end else begin
              msg_idx_q <= msg_idx_q + 6'd1;
            end
          end
        end

        StDone: begin
        end

        default: begin
          state_q <= StDone;
          error_count_q <= error_count_q + 8'd1;
        end
      endcase
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      dcache_timing_s2_valid_q <= 1'b0;
      dcache_timing_s2_tag_match_q <= 1'b0;
      dcache_timing_s2_state_hit_q <= 1'b0;
      dcache_timing_s3_valid_q <= 1'b0;
      dcache_timing_s3_way_q <= '0;
      dcache_timing_s3_data_q <= '0;
      dcache_miss_s2_valid_q <= 1'b0;
      dcache_miss_s2_hit_q <= 1'b0;
      dcache_miss_s3_valid_q <= 1'b0;
      dcache_miss_s3_data_q <= '0;
      dcache_pipeline_s1_valid_q <= 1'b0;
      dcache_pipeline_s1_valid_seen_q <= 1'b0;
      dcache_pipeline_s1_set_q <= '0;
      dcache_pipeline_s1_tag_q <= '0;
      dcache_pipeline_s1_clk_en_q <= 1'b0;
      dcache_pipeline_s1_way_valid_q <= 1'b0;
      dcache_pipeline_s1_tag_match_q <= 1'b0;
      dcache_pipeline_s1_state_hit_q <= 1'b0;
      dcache_pipeline_s2_valid_q <= 1'b0;
      dcache_pipeline_s2_tag_match_q <= '0;
      dcache_pipeline_s2_state_hit_q <= 1'b0;
      dcache_qual_s1_valid_q <= 1'b0;
      dcache_qual_s1_valid_seen_q <= 1'b0;
      dcache_qual_s1_set_q <= '0;
      dcache_qual_s1_tag_q <= '0;
      dcache_qual_s1_clk_en_q <= 1'b0;
      dcache_qual_s1_is_msg_q <= 1'b0;
      dcache_qual_s1_is_amo_q <= 1'b0;
      dcache_qual_s1_is_bypass_q <= 1'b0;
      dcache_qual_s1_bypass_dcache_q <= 1'b0;
      dcache_qual_s1_tag_match_q <= 1'b0;
      dcache_qual_s2_valid_q <= 1'b0;
      dcache_qual_s2_tag_match_q <= '0;
      dcache_qual_s2_is_msg_q <= 1'b0;
      dcache_qual_s2_is_amo_q <= 1'b0;
      dcache_qual_s2_is_bypass_q <= 1'b0;
      dcache_qual_s2_bypass_dcache_q <= 1'b0;
      dcache_qual_s2_way_hit_state_q <= minion_dcache_pkg::StateInvalid;
      dcache_stress_s1_valid_q <= 1'b0;
      dcache_stress_s1_valid_seen_q <= 1'b0;
      dcache_stress_s1_set_q <= '0;
      dcache_stress_s1_tag_q <= '0;
      dcache_stress_s1_clk_en_q <= 1'b0;
      dcache_stress_s1_way_valid_q <= '0;
      dcache_stress_s1_tag_match_q <= '0;
      dcache_stress_s2_valid_q <= 1'b0;
      dcache_stress_s2_tag_match_q <= '0;
      dcache_stress_s2_state_hit_q <= 1'b0;
      dcache_stress_s2_data_ok_q <= '0;
      dcache_stress_s3_valid_q <= 1'b0;
      dcache_stress_s3_data_q <= '0;
    end else begin
      if (dcache_timing_s1_en) begin
        dcache_timing_s2_valid_q <= 1'b1;
        dcache_timing_s2_tag_match_q <= dcache_leaf_valid && dcache_leaf_tag_match;
        dcache_timing_s2_state_hit_q <= dcache_leaf_coh_ok;
      end

      if (state_q == StZ7TimingS2Check) begin
        dcache_timing_s3_valid_q <= dcache_timing_s2_hit;
        dcache_timing_s3_way_q <= dcache_timing_s2_hit
            ? minion_dcache_pkg::dcache_way_decode(DcacheArrayWay) : '0;
        dcache_timing_s3_data_q <= dcache_arr_read_resp[DcacheArrayBank];
      end

      if (dcache_miss_s1_en) begin
        dcache_miss_s2_valid_q <= 1'b1;
        dcache_miss_s2_hit_q <= dcache_md_read_valid[DcacheArrayIsoWay]
            && (dcache_md_read_resp[DcacheArrayIsoWay].tag == DcacheLookupTag)
            && (dcache_md_read_resp[DcacheArrayIsoWay].coh_state >= minion_dcache_pkg::StateShared);
        dcache_miss_s3_valid_q <= 1'b0;
        dcache_miss_s3_data_q <= '0;
      end

      if (state_q == StZ9MissS2Check) begin
        dcache_miss_s3_valid_q <= dcache_miss_s2_valid_q && dcache_miss_s2_hit_q;
        if (dcache_miss_s2_valid_q && dcache_miss_s2_hit_q) begin
          dcache_miss_s3_data_q <= dcache_arr_read_resp[DcacheArrayBank];
        end else begin
          dcache_miss_s3_data_q <= '0;
        end
      end

      if (dcache_pipeline_s0_valid) begin
        dcache_pipeline_s1_valid_q <= 1'b1;
        dcache_pipeline_s1_set_q <= DcacheLookupSet;
        dcache_pipeline_s1_tag_q <= DcacheLookupTag;
        dcache_pipeline_s1_clk_en_q <= 1'b1;
      end

      if (state_q == StZ12PipeS1Capture) begin
        dcache_pipeline_s1_valid_seen_q <= dcache_pipeline_s1_valid_q;
        dcache_pipeline_s1_way_valid_q <= dcache_pipeline_s1_way_valid;
        dcache_pipeline_s1_tag_match_q <= dcache_pipeline_s1_tag_match;
        dcache_pipeline_s1_state_hit_q <= dcache_pipeline_s1_state_hit;
        dcache_pipeline_s2_valid_q <= dcache_pipeline_s1_valid_q;
        dcache_pipeline_s2_tag_match_q <=
            dcache_pipeline_s1_tag_match ? minion_dcache_pkg::dcache_way_decode(DcacheArrayWay) : '0;
        dcache_pipeline_s2_state_hit_q <= dcache_pipeline_s1_state_hit;
        dcache_pipeline_s1_valid_q <= 1'b0;
        dcache_pipeline_s1_clk_en_q <= 1'b0;
      end

      if (dcache_qual_s0_valid) begin
        dcache_qual_s1_valid_q <= 1'b1;
        dcache_qual_s1_set_q <= DcacheLookupSet;
        dcache_qual_s1_tag_q <= DcacheLookupTag;
        dcache_qual_s1_clk_en_q <= 1'b1;
        dcache_qual_s1_is_msg_q <= 1'b0;
        dcache_qual_s1_is_amo_q <= 1'b0;
        dcache_qual_s1_is_bypass_q <= dcache_qual_s0_is_bypass;
        dcache_qual_s1_bypass_dcache_q <= 1'b0;
      end

      if ((state_q == StZ13QualS1Capture) || (state_q == StZ13BypassS1Capture)) begin
        dcache_qual_s1_valid_seen_q <= dcache_qual_s1_valid_q;
        dcache_qual_s1_tag_match_q <= dcache_qual_s1_tag_match;
        dcache_qual_s2_valid_q <= dcache_qual_s1_valid_q;
        dcache_qual_s2_tag_match_q <= dcache_qual_s1_tag_match
            ? minion_dcache_pkg::dcache_way_decode(DcacheArrayWay) : '0;
        dcache_qual_s2_is_msg_q <= dcache_qual_s1_is_msg_q;
        dcache_qual_s2_is_amo_q <= dcache_qual_s1_is_amo_q;
        dcache_qual_s2_is_bypass_q <= dcache_qual_s1_is_bypass_q;
        dcache_qual_s2_bypass_dcache_q <= dcache_qual_s1_bypass_dcache_q;
        dcache_qual_s2_way_hit_state_q <= dcache_md_read_resp[DcacheArrayWay].coh_state;
        dcache_qual_s1_valid_q <= 1'b0;
        dcache_qual_s1_clk_en_q <= 1'b0;
      end

      if (dcache_stress_s0_valid) begin
        dcache_stress_s1_valid_q <= 1'b1;
        dcache_stress_s1_set_q <= DcacheLookupSet;
        dcache_stress_s1_tag_q <= DcacheLookupTag;
        dcache_stress_s1_clk_en_q <= 1'b1;
      end

      if (state_q == StZ14StressS1Capture) begin
        dcache_stress_s1_valid_seen_q <= dcache_stress_s1_valid_q;
        dcache_stress_s1_way_valid_q <= dcache_stress_s1_way_valid;
        dcache_stress_s1_tag_match_q <= dcache_stress_s1_tag_match;
        dcache_stress_s2_valid_q <= dcache_stress_s1_valid_q;
        dcache_stress_s2_tag_match_q <= dcache_stress_s1_tag_match;
        dcache_stress_s2_state_hit_q <= |(dcache_stress_s1_tag_match & dcache_stress_s1_state_hit_vec);
        dcache_stress_s2_data_ok_q <= dcache_stress_s1_data_ok;
        dcache_stress_s1_valid_q <= 1'b0;
        dcache_stress_s1_clk_en_q <= 1'b0;
      end

      if (state_q == StZ14StressS2Check) begin
        dcache_stress_s3_valid_q <= dcache_stress_s2_hit;
        dcache_stress_s3_data_q <= dcache_arr_read_resp[DcacheArrayBank];
      end
    end
  end
  /* verilator lint_on SYNCASYNCNET */

  assign tx_valid = (state_q == StPrint);
  assign tx_data = msg_byte(msg_kind_q, msg_idx_q);
  assign tx_fire = tx_valid && tx_ready;
  assign preview_contract_clk = preview_contract_hold_hi ? 1'b1 : clk_i;
  assign dual_preview_clk = dual_hold_hi ? 1'b1 : clk_i;
  assign dcache_leaf_valid = dcache_md_read_valid[DcacheArrayWay];
  assign dcache_leaf_tag_match = dcache_md_read_resp[DcacheArrayWay].tag == DcacheLookupTag;
  assign dcache_leaf_coh_ok =
      dcache_md_read_resp[DcacheArrayWay].coh_state >= minion_dcache_pkg::StateShared;
  assign dcache_leaf_meta_hit = dcache_leaf_valid && dcache_leaf_tag_match && dcache_leaf_coh_ok;
  assign dcache_leaf_data0_ok = dcache_arr_read_resp[DcacheArrayBank] == DcacheHighHalfExpected;
  assign dcache_leaf_data1_ok = dcache_arr_read_resp[1] == DcacheBank1Line;
  assign dcache_leaf_iso_clear = !dcache_md_read_valid[DcacheArrayIsoWay];
  assign dcache_timing_s2_hit =
      dcache_timing_s2_valid_q && dcache_timing_s2_tag_match_q && dcache_timing_s2_state_hit_q;
  assign dcache_pipeline_s1_way_valid =
      dcache_pipeline_s1_clk_en_q && dcache_md_read_valid[DcacheArrayWay];
  assign dcache_pipeline_s1_tag_match =
      dcache_pipeline_s1_way_valid && (dcache_md_read_resp[DcacheArrayWay].tag == dcache_pipeline_s1_tag_q);
  assign dcache_pipeline_s1_state_hit =
      dcache_pipeline_s1_way_valid
      && (dcache_md_read_resp[DcacheArrayWay].coh_state >= minion_dcache_pkg::StateShared);
  assign dcache_pipeline_s2_hit =
      dcache_pipeline_s2_valid_q && dcache_pipeline_s2_tag_match_q[DcacheArrayWay]
      && dcache_pipeline_s2_state_hit_q;
  assign dcache_qual_s1_way_valid =
      dcache_qual_s1_clk_en_q && dcache_md_read_valid[DcacheArrayWay];
  assign dcache_qual_s1_tag_match =
      dcache_qual_s1_way_valid && (dcache_md_read_resp[DcacheArrayWay].tag == dcache_qual_s1_tag_q);
  assign dcache_qual_s2_tag_match_qual =
      dcache_qual_s2_tag_match_q
      & {minion_dcache_pkg::DcacheWays{!dcache_qual_s2_is_msg_q}}
      & {minion_dcache_pkg::DcacheWays{!dcache_qual_s2_is_amo_q}}
      & {minion_dcache_pkg::DcacheWays{!dcache_qual_s2_is_bypass_q}}
      & {minion_dcache_pkg::DcacheWays{!dcache_qual_s2_bypass_dcache_q}};
  assign dcache_qual_s2_state_hit =
      dcache_qual_s2_way_hit_state_q >= minion_dcache_pkg::StateShared;
  assign dcache_qual_s2_hit =
      dcache_qual_s2_valid_q && dcache_qual_s2_tag_match_qual[DcacheArrayWay]
      && dcache_qual_s2_state_hit;
  assign dcache_qual_hit_diag = {
    dcache_qual_s2_hit,
    dcache_qual_s2_state_hit,
    dcache_qual_s2_tag_match_qual[DcacheArrayWay],
    dcache_qual_s2_valid_q,
    dcache_qual_s2_tag_match_q[DcacheArrayWay],
    dcache_qual_s1_tag_match_q,
    dcache_qual_s1_valid_seen_q
  };
  assign dcache_qual_bypass_suppressed =
      dcache_qual_s2_valid_q && dcache_qual_s2_tag_match_q[DcacheArrayWay]
      && !dcache_qual_s2_tag_match_qual[DcacheArrayWay] && !dcache_qual_s2_hit;
  assign dcache_stress_s2_tag_match_qual = dcache_stress_s2_tag_match_q;
  assign dcache_stress_s2_hit =
      dcache_stress_s2_valid_q
      && dcache_stress_s2_tag_match_qual[DcacheArrayWay]
      && dcache_stress_s2_state_hit_q;
  assign dcache_timing_final_diag = {
    dcache_timing_s3_way_q == minion_dcache_pkg::dcache_way_decode(DcacheArrayWay),
    dcache_timing_s3_data_q == DcacheHighHalfExpected,
    dcache_timing_s3_valid_q,
    dcache_timing_diag_q[4:0]
  };
  assign dcache_pipeline_final_diag = {
    dcache_pipeline_s2_hit,
    dcache_pipeline_s2_state_hit_q,
    dcache_pipeline_s2_tag_match_q[DcacheArrayWay],
    dcache_pipeline_s2_valid_q,
    dcache_pipeline_s1_state_hit_q,
    dcache_pipeline_s1_tag_match_q,
    dcache_pipeline_s1_way_valid_q,
    dcache_pipeline_s1_valid_seen_q
  };
  assign dcache_stress_final_diag = {
    dcache_stress_s3_valid_q && (dcache_stress_s3_data_q == DcacheHighHalfExpected),
    dcache_stress_s2_data_ok_q == 4'hF,
    dcache_stress_s2_state_hit_q,
    dcache_stress_s2_tag_match_qual == minion_dcache_pkg::dcache_way_decode(DcacheArrayWay),
    dcache_stress_s2_valid_q,
    dcache_stress_s1_tag_match_q == minion_dcache_pkg::dcache_way_decode(DcacheArrayWay),
    dcache_stress_s1_way_valid_q == 4'hF,
    dcache_stress_s1_valid_seen_q
  };
  assign dcache_leaf_hit_diag = {
    dcache_leaf_meta_hit && dcache_leaf_data0_ok && dcache_leaf_data1_ok,
    dcache_leaf_iso_clear,
    dcache_leaf_data1_ok,
    dcache_leaf_data0_ok,
    dcache_leaf_meta_hit,
    dcache_leaf_coh_ok,
    dcache_leaf_tag_match,
    dcache_leaf_valid
  };
  assign dcache_md_array_diag = {
    2'b00,
    dcache_md_read_valid == minion_dcache_pkg::dcache_way_decode(DcacheArrayWay),
    !dcache_md_read_valid[DcacheArrayIsoWay],
    dcache_md_read_valid[DcacheArrayWay] &&
        (dcache_md_read_resp[DcacheArrayWay].tag == DcacheLookupTag) &&
        (dcache_md_read_resp[DcacheArrayWay].coh_state >= minion_dcache_pkg::StateShared),
    dcache_md_read_resp[DcacheArrayWay].coh_state >= minion_dcache_pkg::StateShared,
    dcache_md_read_resp[DcacheArrayWay].tag == DcacheLookupTag,
    dcache_md_read_valid[DcacheArrayWay]
  };
  assign dcache_sweep_final_diag = {
    !dcache_sweep_fail_q,
    dcache_sweep_seen_set_q == 4'hF,
    dcache_sweep_seen_idx_q == 2'b11,
    dcache_sweep_seen_way_q == 4'hF,
    dcache_sweep_seen_bank_q == 4'hF,
    !dcache_sweep_fail_q,
    !dcache_sweep_fail_q,
    !dcache_sweep_fail_q
  };

  always_comb begin
    dual_seed_diag = '0;
    dual_block_diag = '0;
    dual_rearm_diag = '0;

    // Bits: 0=1r1w, 1=diff-8, 2=frontend diff, 3=reg, 4=dec,
    // 5=par, 6=2r1w, 7=3r2w, 8=single-entry par,
    // 9=frontend diff with prim_clk_gate-produced preview clock.
    dual_seed_diag[0] = (dual_p_rd_data == DualPreviewSeed);
    dual_seed_diag[1] = (dual_diff_rd_data == DualDiffSeed[7:0]);
    dual_seed_diag[2] = (dual_front_rd_data == DualFrontSeedWord);
    dual_seed_diag[3] = (dual_reg_rd_data == DualRegSeed);
    dual_seed_diag[4] = (dual_dec_rd_data == DualDecSeed);
    dual_seed_diag[5] = (dual_par_rd_data[2*8 +: 8] == DualParSeed);
    dual_seed_diag[6] =
        (dual_two_rd_data_a == DualTwoSeed)
        && (dual_two_rd_data_b == DualTwoSeed)
        && (dual_two_rd_par == DualTwoSeed);
    dual_seed_diag[7] =
        (dual_tri_rd_data_a == DualTriSeedA)
        && (dual_tri_rd_data_b == DualTriSeedB)
        && (dual_tri_rd_data_c == DualTriSeedA);
    dual_seed_diag[8] = (dual_single_rd_data == DualSingleSeed);
    dual_seed_diag[9] = (gated_front_rd_data == DualFrontSeedWord);

    dual_block_diag[0] = (dual_p_rd_data == DualPreviewSeed);
    dual_block_diag[1] = (dual_diff_rd_data == DualDiffSeed[7:0]);
    dual_block_diag[2] = (dual_front_rd_data == DualFrontSeedWord);
    dual_block_diag[3] = (dual_reg_rd_data == DualRegSeed);
    dual_block_diag[4] = (dual_dec_rd_data == DualDecSeed);
    dual_block_diag[5] = (dual_par_rd_data[2*8 +: 8] == DualParSeed);
    dual_block_diag[6] =
        (dual_two_rd_data_a == DualTwoSeed)
        && (dual_two_rd_data_b == DualTwoSeed)
        && (dual_two_rd_par == DualTwoSeed);
    dual_block_diag[7] =
        (dual_tri_rd_data_a == DualTriSeedA)
        && (dual_tri_rd_data_b == DualTriSeedB)
        && (dual_tri_rd_data_c == DualTriSeedA);
    dual_block_diag[8] = (dual_single_rd_data == DualSingleSeed);
    dual_block_diag[9] = (gated_front_rd_data == DualFrontSeedWord);

    dual_rearm_diag[0] = (dual_p_rd_data == DualPreviewRearm);
    dual_rearm_diag[1] = (dual_diff_rd_data == DualDiffRearm[7:0]);
    dual_rearm_diag[2] = (dual_front_rd_data == DualFrontRearmWord);
    dual_rearm_diag[3] = (dual_reg_rd_data == DualRegRearm);
    dual_rearm_diag[4] = (dual_dec_rd_data == DualDecRearm);
    dual_rearm_diag[5] = (dual_par_rd_data[2*8 +: 8] == DualParRearm);
    dual_rearm_diag[6] =
        (dual_two_rd_data_a == DualTwoRearm)
        && (dual_two_rd_data_b == DualTwoRearm)
        && (dual_two_rd_par == DualTwoRearm);
    dual_rearm_diag[7] =
        (dual_tri_rd_data_a == DualTriRearmA)
        && (dual_tri_rd_data_b == DualTriRearmB)
        && (dual_tri_rd_data_c == DualTriRearmA);
    dual_rearm_diag[8] = (dual_single_rd_data == DualSingleRearm);
    dual_rearm_diag[9] = (gated_front_rd_data == DualFrontRearmWord);
  end

  always_comb begin
    dcache_data_all_diag = '0;
    dcache_sweep_read_ok = 1'b1;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheLramNumBanks; i++) begin
      dcache_data_all_diag[i] = (dcache_arr_read_resp[i] == dcache_bank_line(i));
      dcache_sweep_read_ok &= (dcache_arr_read_resp[i] == dcache_sweep_line(dcache_sweep_slot_q, i));
    end

    dcache_data_low_diag = {
      6'b000000,
      dcache_arr_read_resp[DcacheArrayBank][63:32] == DcacheBank0Line[63:32],
      dcache_arr_read_resp[DcacheArrayBank][31:0] == DcacheLowHalfLine[31:0]
    };
    dcache_data_high_diag = {
      6'b000000,
      dcache_arr_read_resp[DcacheArrayBank][31:0] == DcacheLowHalfLine[31:0],
      dcache_arr_read_resp[DcacheArrayBank][63:32] == DcacheHighHalfLine[63:32]
    };

    dcache_stress_s1_way_valid =
        dcache_md_read_valid & {minion_dcache_pkg::DcacheWays{dcache_stress_s1_clk_en_q}};
    dcache_stress_s1_tag_match = '0;
    dcache_stress_s1_state_hit_vec = '0;
    for (int unsigned i = 0; i < minion_dcache_pkg::DcacheWays; i++) begin
      dcache_stress_s1_tag_match[i] =
          dcache_stress_s1_way_valid[i]
          && (dcache_md_read_resp[i].tag == dcache_stress_s1_tag_q);
      dcache_stress_s1_state_hit_vec[i] =
          dcache_stress_s1_way_valid[i]
          && (dcache_md_read_resp[i].coh_state >= minion_dcache_pkg::StateShared);
    end

    dcache_stress_s1_data_ok = '0;
    dcache_stress_s1_data_ok[0] = dcache_arr_read_resp[0] == DcacheHighHalfExpected;
    dcache_stress_s1_data_ok[1] = dcache_arr_read_resp[1] == DcacheBank1Line;
    dcache_stress_s1_data_ok[2] = dcache_arr_read_resp[2] == DcacheBank2Line;
    dcache_stress_s1_data_ok[3] = dcache_arr_read_resp[3] == DcacheBank3Line;
  end

  prim_rf_1r1w_preview #(
    .Width (8),
    .Depth (4)
  ) u_preview_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (preview_wr_next),
    .wr_en_i              (preview_wr_en),
    .wr_addr_i            (preview_wr_addr),
    .wr_data_i            (preview_wr_data),
    .rd_addr_i            (preview_rd_addr),
    .rd_data_o            (preview_rd_data)
  );

  prim_rf_1r1w_preview #(
    .Width (8),
    .Depth (4),
    .PreviewClkSameAsRf (1'b0)
  ) u_preview_contract_rf (
    .preview_clk_i        (preview_contract_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (preview_contract_wr_next),
    .wr_en_i              (preview_contract_wr_en),
    .wr_addr_i            (preview_contract_wr_addr),
    .wr_data_i            (preview_contract_wr_data),
    .rd_addr_i            (preview_contract_rd_addr),
    .rd_data_o            (preview_contract_rd_data)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (8),
    .RAlignment (8),
    .WWidth     (16),
    .Entries    (2)
  ) u_diff_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (diff_wr_next),
    .rd_data_o            (diff_rd_data),
    .rd_addr_i            (diff_rd_addr),
    .wr_data_i            (diff_wr_data),
    .wr_addr_i            (diff_wr_addr),
    .wr_en_i              (diff_wr_en)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (32),
    .RAlignment (16),
    .WWidth     (256),
    .Entries    (2)
  ) u_frontend_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (front_wr_next),
    .rd_data_o            (front_rd_data),
    .rd_addr_i            (front_rd_addr),
    .wr_data_i            (front_wr_data),
    .wr_addr_i            (front_wr_addr),
    .wr_en_i              (front_wr_en)
  );

  prim_rf_1r1w_reg_preview #(
    .Width (16),
    .Depth (8)
  ) u_reg_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (reg_wr_next),
    .wr_data_i            (reg_wr_data),
    .wr_addr_i            (reg_wr_addr),
    .wr_en_i              (reg_wr_en),
    .rd_addr_i            (reg_rd_addr),
    .rd_en_i              (reg_rd_en),
    .rd_data_o            (reg_rd_data)
  );

  prim_rf_1r1w_dec_preview #(
    .Width   (16),
    .Entries (8)
  ) u_dec_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (dec_wr_next),
    .rd_addr_i            (dec_rd_addr),
    .rd_data_o            (dec_rd_data),
    .wr_data_i            (dec_wr_data),
    .wr_addr_i            (dec_wr_addr),
    .wr_en_i              (dec_wr_en)
  );

  prim_rf_1r1w_par_preview #(
    .Width (8),
    .Depth (4)
  ) u_par_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (par_wr_next),
    .rd_data_o            (par_rd_data),
    .wr_data_i            (par_wr_data),
    .wr_addr_i            (par_wr_addr),
    .wr_en_i              (par_wr_en)
  );

  prim_rf_2r1w_preview #(
    .Width     (16),
    .Entries   (8),
    .Zero      (8'b0000_0001),
    .Parallel  (8'b1000_0000),
    .ParallelW (16)
  ) u_two_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .rd_addr_a_i          (two_rd_addr_a),
    .rd_data_a_o          (two_rd_data_a),
    .rd_addr_b_i          (two_rd_addr_b),
    .rd_data_b_o          (two_rd_data_b),
    .rd_par_o             (two_rd_par),
    .wr_en_i              (two_wr_en),
    .wr_data_en_1p_next_i (two_wr_next),
    .wr_addr_i            (two_wr_addr),
    .wr_data_i            (two_wr_data)
  );

  prim_rf_3r2w_preview #(
    .Width   (16),
    .Entries (8)
  ) u_tri_rf (
    .preview_clk_i          (clk_i),
    .rf_clk_i               (clk_i),
    .rd_addr_a_i            (tri_rd_addr_a),
    .rd_data_a_o            (tri_rd_data_a),
    .rd_addr_b_i            (tri_rd_addr_b),
    .rd_data_b_o            (tri_rd_data_b),
    .rd_addr_c_i            (tri_rd_addr_c),
    .rd_data_c_o            (tri_rd_data_c),
    .wr_en_a_i              (tri_wr_en_a),
    .wr_addr_a_i            (tri_wr_addr_a),
    .wr_data_a_en_1p_next_i (tri_wr_next_a),
    .wr_data_a_i            (tri_wr_data_a),
    .wr_en_b_i              (tri_wr_en_b),
    .wr_addr_b_i            (tri_wr_addr_b),
    .wr_data_b_en_1p_next_i (tri_wr_next_b),
    .wr_data_b_i            (tri_wr_data_b)
  );

  prim_rf_1r1w_par_preview #(
    .Width (8),
    .Depth (4)
  ) u_rq_par_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (rq_wr_next),
    .rd_data_o            (rq_rd_data),
    .wr_data_i            (rq_data),
    .wr_addr_i            (rq_addr),
    .wr_en_i              (rq_wr_en)
  );

  prim_rf_single_1r1w_par_preview #(
    .Width (16)
  ) u_single_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .rd_data_o            (single_rd_data),
    .wr_data_i            (single_wr_data),
    .wr_data_en_1p_next_i (single_wr_next),
    .wr_en_i              (single_wr_en)
  );

  prim_rf_1r1w_preview #(
    .Width (8),
    .Depth (4),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_preview_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (dual_p_wr_next),
    .wr_en_i              (dual_p_wr_en),
    .wr_addr_i            (dual_p_wr_addr),
    .wr_data_i            (dual_p_wr_data),
    .rd_addr_i            (dual_p_rd_addr),
    .rd_data_o            (dual_p_rd_data)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (8),
    .RAlignment (8),
    .WWidth     (16),
    .Entries    (2),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_diff_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (dual_diff_wr_next),
    .rd_data_o            (dual_diff_rd_data),
    .rd_addr_i            (dual_diff_rd_addr),
    .wr_data_i            (dual_diff_wr_data),
    .wr_addr_i            (dual_diff_wr_addr),
    .wr_en_i              (dual_diff_wr_en)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (32),
    .RAlignment (16),
    .WWidth     (256),
    .Entries    (2),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_frontend_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (dual_front_wr_next),
    .rd_data_o            (dual_front_rd_data),
    .rd_addr_i            (dual_front_rd_addr),
    .wr_data_i            (dual_front_wr_data),
    .wr_addr_i            (dual_front_wr_addr),
    .wr_en_i              (dual_front_wr_en)
  );

  prim_clk_gate u_gated_preview_clk (
    .clk_i (clk_i),
    .en_i  (gated_preview_en),
    .dft_i (dft_pkg::dft_t'('0)),
    .clk_o (gated_preview_clk)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (32),
    .RAlignment (16),
    .WWidth     (256),
    .Entries    (2),
    .PreviewClkSameAsRf (1'b0)
  ) u_gated_frontend_rf (
    .preview_clk_i        (gated_preview_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (gated_front_wr_next),
    .rd_data_o            (gated_front_rd_data),
    .rd_addr_i            (gated_front_rd_addr),
    .wr_data_i            (gated_front_wr_data),
    .wr_addr_i            (gated_front_wr_addr),
    .wr_en_i              (gated_front_wr_en)
  );

  prim_rf_1r1w_reg_preview #(
    .Width (16),
    .Depth (8),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_reg_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (dual_reg_wr_next),
    .wr_data_i            (dual_reg_wr_data),
    .wr_addr_i            (dual_reg_wr_addr),
    .wr_en_i              (dual_reg_wr_en),
    .rd_addr_i            (dual_reg_rd_addr),
    .rd_en_i              (dual_reg_rd_en),
    .rd_data_o            (dual_reg_rd_data)
  );

  prim_rf_1r1w_dec_preview #(
    .Width   (16),
    .Entries (8),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_dec_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (dual_dec_wr_next),
    .rd_addr_i            (dual_dec_rd_addr),
    .rd_data_o            (dual_dec_rd_data),
    .wr_data_i            (dual_dec_wr_data),
    .wr_addr_i            (dual_dec_wr_addr),
    .wr_en_i              (dual_dec_wr_en)
  );

  prim_rf_1r1w_par_preview #(
    .Width (8),
    .Depth (4),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_par_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (dual_par_wr_next),
    .rd_data_o            (dual_par_rd_data),
    .wr_data_i            (dual_par_wr_data),
    .wr_addr_i            (dual_par_wr_addr),
    .wr_en_i              (dual_par_wr_en)
  );

  prim_rf_2r1w_preview #(
    .Width     (16),
    .Entries   (8),
    .Zero      (8'b0000_0001),
    .Parallel  (8'b1000_0000),
    .ParallelW (16),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_two_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .rd_addr_a_i          (dual_two_rd_addr_a),
    .rd_data_a_o          (dual_two_rd_data_a),
    .rd_addr_b_i          (dual_two_rd_addr_b),
    .rd_data_b_o          (dual_two_rd_data_b),
    .rd_par_o             (dual_two_rd_par),
    .wr_en_i              (dual_two_wr_en),
    .wr_data_en_1p_next_i (dual_two_wr_next),
    .wr_addr_i            (dual_two_wr_addr),
    .wr_data_i            (dual_two_wr_data)
  );

  prim_rf_3r2w_preview #(
    .Width   (16),
    .Entries (8),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_tri_rf (
    .preview_clk_i          (dual_preview_clk),
    .rf_clk_i               (clk_i),
    .rd_addr_a_i            (dual_tri_rd_addr_a),
    .rd_data_a_o            (dual_tri_rd_data_a),
    .rd_addr_b_i            (dual_tri_rd_addr_b),
    .rd_data_b_o            (dual_tri_rd_data_b),
    .rd_addr_c_i            (dual_tri_rd_addr_c),
    .rd_data_c_o            (dual_tri_rd_data_c),
    .wr_en_a_i              (dual_tri_wr_en_a),
    .wr_addr_a_i            (dual_tri_wr_addr_a),
    .wr_data_a_en_1p_next_i (dual_tri_wr_next_a),
    .wr_data_a_i            (dual_tri_wr_data_a),
    .wr_en_b_i              (dual_tri_wr_en_b),
    .wr_addr_b_i            (dual_tri_wr_addr_b),
    .wr_data_b_en_1p_next_i (dual_tri_wr_next_b),
    .wr_data_b_i            (dual_tri_wr_data_b)
  );

  prim_rf_single_1r1w_par_preview #(
    .Width (16),
    .PreviewClkSameAsRf (1'b0)
  ) u_dual_single_rf (
    .preview_clk_i        (dual_preview_clk),
    .rf_clk_i             (clk_i),
    .rd_data_o            (dual_single_rd_data),
    .wr_data_i            (dual_single_wr_data),
    .wr_data_en_1p_next_i (dual_single_wr_next),
    .wr_en_i              (dual_single_wr_en)
  );

  prim_write_preview_en #(
    .Width (1)
  ) u_chain_mem_wen_p2 (
    .clk_i (clk_i),
    .en_i  (1'b1),
    .d_i   (chain_mem_wen),
    .q_o   (chain_mem_wen_p2)
  );

  prim_write_preview_en #(
    .Width (16)
  ) u_chain_wdata_1p (
    .clk_i (clk_i),
    .en_i  (chain_mem_wen_p2),
    .d_i   (chain_wdata),
    .q_o   (chain_wdata_1p)
  );

  prim_rf_1r1w_preview #(
    .Width (32),
    .Depth (8)
  ) u_cache_meta_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (cache_meta_wr_next),
    .wr_en_i              (cache_meta_wr_en),
    .wr_addr_i            (cache_meta_wr_addr),
    .wr_data_i            (cache_meta_wr_data),
    .rd_addr_i            (cache_meta_rd_addr),
    .rd_data_o            (cache_meta_rd_data)
  );

  minion_dcache_metadata_array u_dcache_metadata_array (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .s1_read_set_i          (dcache_md_read_set),
    .s1_write_valid_i       (dcache_md_write_valid),
    .s1_write_valid_prev_i  (dcache_md_write_valid_prev),
    .s1_write_req_i         (dcache_md_write_req),
    .s1_read_resp_valid_o   (dcache_md_read_valid),
    .s1_read_resp_o         (dcache_md_read_resp),
    .cfg_clear_all_i        (dcache_md_clear_all),
    .cfg_clear_low_i        (dcache_md_clear_low)
  );

  prim_rf_1r1w_reg_preview #(
    .Width (32),
    .Depth (128)
  ) u_cache_lram_lo_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (cache_lram_wr_next && cache_lram_wr_next_mask[0]),
    .wr_data_i            (cache_lram_wr_data[31:0]),
    .wr_addr_i            (cache_lram_wr_addr),
    .wr_en_i              (cache_lram_wr_en && cache_lram_wr_en_mask[0]),
    .rd_addr_i            (cache_lram_rd_addr),
    .rd_en_i              (cache_lram_rd_en),
    .rd_data_o            (cache_lram_rd_data[31:0])
  );

  prim_rf_1r1w_reg_preview #(
    .Width (32),
    .Depth (128)
  ) u_cache_lram_hi_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (cache_lram_wr_next && cache_lram_wr_next_mask[1]),
    .wr_data_i            (cache_lram_wr_data[63:32]),
    .wr_addr_i            (cache_lram_wr_addr),
    .wr_en_i              (cache_lram_wr_en && cache_lram_wr_en_mask[1]),
    .rd_addr_i            (cache_lram_rd_addr),
    .rd_en_i              (cache_lram_rd_en),
    .rd_data_o            (cache_lram_rd_data[63:32])
  );

  minion_dcache_data_array u_dcache_array (
    .clk_i            (clk_i),
    .s1_read_valid_i  (dcache_arr_read_valid),
    .s1_read_req_i    (dcache_arr_read_req),
    .s2_read_resp_o   (dcache_arr_read_resp),
    .s3_write_valid_i (dcache_arr_s3_write_valid),
    .s4_write_valid_i (dcache_arr_s4_write_valid),
    .s4_write_req_i   (dcache_arr_write_req),
    .ram_cfg_i        (ram_cfg_pkg::ram_cfg_t'('0))
  );

  ulx3s_uart_tx #(
    .ClkHz    (ClkHz),
    .BaudRate (BaudRate)
  ) u_uart_tx (
    .clk_i,
    .rst_ni,
    .valid_i (tx_valid),
    .data_i  (tx_data),
    .ready_o (tx_ready),
    .tx_o    (uart_tx_o),
    .busy_o  (tx_busy)
  );

  assign led_o = {
    (state_q == StDone) && (error_count_q == 8'd0),
    (state_q == StDone) && (error_count_q != 8'd0),
    error_count_q[5:0]
  };

endmodule
