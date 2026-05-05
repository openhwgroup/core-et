// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package vpu_pkg;

  /* verilator lint_off UNUSEDPARAM */  // The package is landing ahead of the full VPU subtree, so some widths are for upcoming modules.
  // Original VPU functional / implementation-selection `ifdef`s become
  // explicit RTL-visible config constants as modules are translated.
  parameter bit          EnableExtraTransDefault    = 1'b0;
  parameter bit          UseMmiVpuRfDefault         = 1'b0;
  parameter bit          UseMmiVpuRfNetlistDefault  = 1'b0;
  parameter bit          UseMmiTimaDefault          = 1'b0;
  parameter bit          UseMmiTxfmaDefault         = 1'b0;
  parameter bit          UseFakeTxfmaDefault        = 1'b0;
  parameter bit          EnableRcg2Default          = 1'b1;
  parameter int unsigned Float16M                  = 10;
  parameter int unsigned Float16W                  = 16;
  parameter int unsigned Float32M                  = 23;
  parameter int unsigned Float32W                  = 32;
  parameter int unsigned VpuRegfileNum             = 32;
  parameter int unsigned VpuFlagsSz                = 6;
  parameter int unsigned CoreFcsrFlagBitsSz        = VpuFlagsSz;
  parameter int unsigned VpuTenaSize               = 2;
  parameter int unsigned VpuTenaAddrW              = $clog2(VpuTenaSize);
  parameter int unsigned VpuTenbRfSize             = minion_pkg::VpuTenbSize / 2;
  parameter int unsigned VpuTenbRfAddrW            = $clog2(VpuTenbRfSize);
  parameter int unsigned VpuTencSize               = 16;
  parameter int unsigned VpuTencAddrW              = $clog2(VpuTencSize);
  parameter int unsigned VpuTentmpSize             = 8;
  parameter int unsigned VpuTentmpAddrW            = $clog2(VpuTentmpSize);
  parameter int unsigned TimaInt8MulNum            = 4;
  parameter int unsigned VpuMlLoadCtrlWaddrSz      = $clog2(VpuRegfileNum);
  parameter int unsigned VpuMlLoadCtrlChunkIdxSz   = $clog2(minion_pkg::VpuDataSz / minion_pkg::VpuDataSSz);
  parameter int unsigned TxfmaA3                   = 63;
  parameter int unsigned TxfmaA2                   = 62;
  parameter int unsigned TxfmaA1                   = 61;
  parameter int unsigned TxfmaA0                   = 60;
  parameter int unsigned TxfmaB27                  = 32;
  parameter int unsigned TxfmaB31                  = 28;
  parameter int unsigned TxfmaB45                  = 14;
  parameter int unsigned TxfmaB47                  = 12;
  parameter int unsigned TxfmaB59                  = 0;
  parameter int unsigned TxfmaPsDpathSz            = TxfmaA3 - TxfmaB47 + 1;
  parameter int unsigned TxfmaLxdPsSz              = 7;
  parameter int unsigned TxfmaEtrzPsSz             = 5;
  parameter int unsigned TxfmaWspsEtrzSz           = 6;
  parameter int unsigned TxfmaExpPsSz              = 8;
  parameter int unsigned TxfmaExpSextPsSz          = 10;
  parameter int unsigned TransC2MaxSize            = 17;
  parameter int unsigned TransC0MaxSize            = 32;
  parameter int unsigned TransRomMaxIndexSize      = 8;
  parameter int unsigned TransMaskSize             = 7;
  localparam logic [31:0] Float32PosCqnan          = 32'h7fc0_0000;
  /* verilator lint_on UNUSEDPARAM */

  typedef enum logic [2:0] {
    VpuFlagInexact  = 3'd0,
    VpuFlagUnf      = 3'd1,
    VpuFlagOvf      = 3'd2,
    VpuFlagDiv0     = 3'd3,
    VpuFlagInvalid  = 3'd4,
    VpuFlagDenormal = 3'd5
  } vpu_flag_e;

  typedef minion_pkg::minion_reg_dest_t  minion_reg_dest;
  typedef minion_pkg::minion_mreg_dest_t minion_mreg_dest;
  typedef minion_pkg::vpu_scoreboard_t   vpu_minion_scoreboard;

  typedef struct packed {
    logic [minion_pkg::VpuDataSSz-1:0] data;
    logic [VpuFlagsSz-1:0]             exc;
  } vpu_output_t;

  typedef struct packed {
    logic                      use_prev_sigs;
    minion_pkg::vpu_ctrl_sigs_t sigs;
    logic [19:0]               imm;
    logic [2:0]                rm;
    logic [1:0]                typ;
    logic [minion_pkg::VpuDataSSz-1:0] in1;
    logic [minion_pkg::VpuDataSSz-1:0] in2;
    logic [minion_pkg::VpuDataSSz-1:0] in3;
  } vpu_input_t;

  typedef struct packed {
    logic       tensorfma_start;
    logic [7:0] tensorfma_scp_a;
    logic [7:0] tensorfma_scp_b;
  } vpu_core_ctrl_t;
  typedef vpu_core_ctrl_t vpu_core_ctrl;

  typedef struct packed {
    logic                            wen;
    logic                            thread_id;
    logic [VpuMlLoadCtrlChunkIdxSz-1:0] broadcast_sel;
    logic [VpuMlLoadCtrlWaddrSz-1:0]    waddr;
  } vpu_ml_load_ctrl_t;
  typedef vpu_ml_load_ctrl_t vpu_ml_load_ctrl;

  typedef struct packed {
    logic                            thread_id;
    logic                            vector;
    logic [minion_pkg::VpuLaneNum-1:0] wmask;
    logic [minion_pkg::FregAddrSize-1:0] waddr;
  } vpu_trans_div_info_t;
  typedef vpu_trans_div_info_t vpu_trans_div_info_type_t;

  typedef struct packed {
    logic txfma_in0;
    logic txfma_in1;
    logic txfma_in2;
    logic shsw_in1;
    logic shsw_in2;
  } vpu_bypass_force_ctrl_t;
  typedef vpu_bypass_force_ctrl_t vpu_bypass_force_ctrl;

  typedef enum logic [4:0] {
    TransNullState,
    TransRcpFma1State,
    TransRcpFma2State,
    TransRsqrtFma1State,
    TransRsqrtFma2State,
    TransLogFma1State,
    TransLogFma2State,
    TransLogMulState,
    TransExpRrState,
    TransExpFma1State,
    TransExpFma2State,
    TransSinTrState,
    TransSinRrState,
    TransSinP1State,
    TransSinP2State,
    TransSinP3State,
    TransNrState,
    TransDirtyState
  } trans_state_id_e;
  typedef trans_state_id_e trans_state_id;

  typedef struct packed {
    trans_state_id_e                 id;
    logic                            thread_id;
    logic [minion_pkg::FregAddrSize-1:0] rs;
    logic [minion_pkg::FregAddrSize-1:0] rd;
    logic                            mask_valid;
    logic [minion_pkg::VpuLaneNum-1:0] mask;
  } trans_state_t;
  typedef trans_state_t trans_state;

  typedef struct packed {
    logic                            en;
    logic [minion_pkg::VpuLaneNum-1:0] mask;
    logic                            thread_id;
    logic                            fcsr_en;
  } trans_write_ctrl_t;
  typedef trans_write_ctrl_t trans_write_ctrl;

  typedef enum logic [3:0] {
    TransRomNoneId,
    TransRomRcp1Id,
    TransRomRcp2Id,
    TransRomRsqrt1Id,
    TransRomRsqrt2Id,
    TransRomLog1Id,
    TransRomLog2Id,
    TransRomExp1Id,
    TransRomExp2Id,
    TransRomSin1Id,
    TransRomSin2Id,
    TransRomSin3Id
  } trans_rom_id_e;
  typedef trans_rom_id_e trans_rom_id;

  typedef logic [TransRomMaxIndexSize-1:0] trans_rom_index_t;
  typedef trans_rom_index_t trans_rom_index;

  typedef struct packed {
    trans_rom_id_e                   cmd;
    trans_rom_index_t                index;
  } trans_rom_ctrl_t;
  typedef trans_rom_ctrl_t trans_rom_ctrl;

  typedef struct packed {
    logic                            exc;
    logic [TransMaskSize-1:0]        mask;
    logic                            taylor;
    logic [TransC2MaxSize-1:0]       c2;
    logic [TransC0MaxSize-1:0]       c0;
  } trans_fma_coefficients_t;
  typedef trans_fma_coefficients_t trans_fma_coefficients;

  typedef struct packed {
    logic                            rs_valid;
    minion_reg_dest                  rs;
    logic                            rd_valid;
    minion_reg_dest                  rd;
  } trans_scoreboard_slot_t;
  typedef trans_scoreboard_slot_t trans_scoreboard_slot;

endpackage
