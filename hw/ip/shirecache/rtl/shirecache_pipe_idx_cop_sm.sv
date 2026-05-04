// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache pipe index cache-op state machine (idx_cop_sm).
//
// One-hot FSM for cache maintenance operations. 7 states:
//   RESET → ALL_INV → SYNC → IDLE  (power-up path)
//   IDLE  → {ALL_INV, ACTIVE, CB_INV, DBG} → SYNC → IDLE  (ESR-triggered)
//
// Features:
//   - Post-reset automatic ALL_INV with cold/warm reset detection
//   - 2-cycle start/abort pipeline (multicycle ESR timing)
//   - Set/way/subbank/qw counters for iterating over cache entries
//   - CBuf invalidation (iterates over cbuf entries, sends L2_Evict per valid)
//   - Debug read/write (single set/way/subbank/qw access)
//   - SCP zero mode (fabricates SCP region address MSBs)
//   - Sync flush after CB_INV operations
//
// Replaces: shire_cache_pipe_idx_cop_sm (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni / rst_c_ni)
//   - lowRISC naming conventions (_i/_o, _d/_q)
//   - prim_rst_sync instead of rst_repeat
//   - Package types instead of `define types
//   - Explicit always_ff/always_comb instead of RST_EN_FF/EN_FF macros
//   - DFT consolidated into dft_pkg::dft_t struct
//   - SkipPowerUpAllInv parameter replaces sim-only dv_skip_power_up_all_inv_const
//   - trans_id tied to 0 (synthesis behavior; sim-only testbench driving removed)

module shirecache_pipe_idx_cop_sm
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter bit SkipPowerUpAllInv = 1'b0  // 0 for synthesis/cosim, 1 to skip power-up ALL_INV
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,     // warm reset (active-low)
  input  logic                        rst_c_ni,   // cold reset condition (active-low)

  // DFT
  input  dft_t                        dft_i,

  // MBIST
  input  logic                        mbist_on_i,

  // Request interface to reqq
  output logic                        pipe_idx_cop_aborted_o,
  output logic                        pipe_idx_cop_l2_busy_o,
  output logic                        pipe_idx_cop_l3_busy_o,
  input  logic                        pipe_idx_cop_req_ready_i,
  output logic                        pipe_idx_cop_req_valid_o,
  output idx_cop_req_t                pipe_idx_cop_req_info_o,

  // FSM active (keep clock enabled)
  output logic                        pipe_idx_cop_is_active_o,

  // State from cbuf
  input  cbuf_state_t                 pipe_cbuf_state_info_i,

  // Debug write interface
  output idx_cop_sm_dbg_write_t       esr_dbg_wr_info_o,

  // Debug read interface
  input  idx_cop_sm_dbg_read_t        dbg_rd_info_i,

  // ESRs
  input  bank_esr_ctl_t               esr_ctl_i,
  output idx_cop_sm_esr_status_t      pipe_idx_cop_sm_esr_status_o,
  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [ScpShireIdSize-1:0]   my_scp_virtual_shire_id_i
);

  /* verilator lint_off SYNCASYNCNET */  // rst_ni/rst_c_ni are async-assert/sync-deassert, used in both always_ff and assertions

  // ═══════════════════════════════════════════════════════════
  // Local parameters
  // ═══════════════════════════════════════════════════════════

  localparam int unsigned SetCountSize        = SetAllocSize;
  localparam int unsigned AllInvSetCountFull   = SetsPerSubBank;
  localparam int unsigned CbInvCountFull       = CbufSize + 8;
  localparam int unsigned CbufCountFull        = CbufSize;
  localparam logic [SetIdSize:0] AllInvSetCount = AllInvSetCountFull[SetIdSize:0];
  localparam logic [SetIdSize:0] CbInvCount     = CbInvCountFull[SetIdSize:0];
  localparam logic [SetIdSize:0] CbufCount      = CbufCountFull[SetIdSize:0];

  // One-hot state encoding
  localparam int unsigned StReset  = 0;
  localparam int unsigned StAllInv = 1;
  localparam int unsigned StIdle   = 2;
  localparam int unsigned StActive = 3;
  localparam int unsigned StCbInv  = 4;
  localparam int unsigned StDbg    = 5;
  localparam int unsigned StSync   = 6;
  localparam int unsigned States   = 7;

  localparam logic [States-1:0] ResetState = {{States-1{1'b0}}, 1'b1};

  // ESR register size
  localparam int unsigned EsrIdxCopRegSize = DwDataSize; // 64

  // ESR ctl field positions
  localparam int unsigned SmOpcodeSize         = $bits(idx_cop_sm_opcode_e);
  localparam int unsigned LogicalRamSize       = 2;
  localparam logic [LogicalRamSize-1:0] LogicalRamTagState = 2'd0;
  localparam logic [LogicalRamSize-1:0] LogicalRamTag      = 2'd1;
  localparam logic [LogicalRamSize-1:0] LogicalRamData     = 2'd2;
  localparam int unsigned EsrIdxCtlGoLsb        = 0;
  localparam int unsigned EsrIdxCtlAbortLsb     = 1;
  localparam int unsigned EsrIdxCtlSmOpcodeLsb  = 8;
  localparam int unsigned EsrIdxCtlEccWrEnLsb   = 13;
  localparam int unsigned EsrIdxCtlLogicalRamLsb= 14;
  localparam int unsigned EsrIdxCtlSmStateLsb   = 24;

  // ESR physical_index field positions
  localparam int unsigned EsrIdxQwLsb      = 0;
  localparam int unsigned EsrIdxSubBankLsb = 8;
  localparam int unsigned EsrIdxWayLsb     = 16;
  localparam int unsigned EsrIdxSetLsb     = 32;

  // ESR data0 field positions
  localparam int unsigned EsrData0TagStateSpaceLsb  = 40;
  localparam int unsigned EsrData0TagStateSpaceSize = $bits(address_space_e);
  localparam int unsigned EsrData0TagWay0Lsb        = 0;
  localparam int unsigned EsrData0TagWay1Lsb        = 32;

  // ESR data1 field positions
  localparam int unsigned EsrData1TagWay2Lsb = 0;
  localparam int unsigned EsrData1TagWay3Lsb = 32;

  // Index size for address construction
  localparam int unsigned IdxSize = SetAllocSize + WayIdSize + SubBankIdSize;

  // Reset counter
  localparam int unsigned ResetCountSize = 8;

  // QW data+ECC size per quadword
  localparam int unsigned QwDataAndEccSize = DataRamDataSize / LineQwSize;

  // ═══════════════════════════════════════════════════════════
  // Reset synchronizers (replace rst_repeat)
  // ═══════════════════════════════════════════════════════════

  logic rst_n;   // synchronized warm reset
  logic rst_c_n; // synchronized cold reset

  prim_rst_sync u_rst_sync_warm (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_n)
  );

  prim_rst_sync u_rst_sync_cold (
    .clk_i  (clk_i),
    .rst_ni (rst_c_ni),
    .dft_i  (dft_i),
    .rst_no (rst_c_n)
  );

  // ═══════════════════════════════════════════════════════════
  // State register
  // ═══════════════════════════════════════════════════════════

  logic [States-1:0] state_q, state_d;

  // ═══════════════════════════════════════════════════════════
  // reset_c_save — remembers if a cold reset occurred
  // ═══════════════════════════════════════════════════════════
  //
  // Set when cold reset is active (!rst_c_n). Cleared by
  // clr_reset_c_save (checked just before power_up_all_inv, or on abort).
  // Not affected by warm reset — survives warm resets intentionally.

  logic clr_reset_c_save;
  logic reset_c_save_q;

  always_ff @(posedge clk_i) begin
    if (clr_reset_c_save)   reset_c_save_q <= 1'b0;
    else if (!rst_c_n)       reset_c_save_q <= 1'b1;
  end

  // ═══════════════════════════════════════════════════════════
  // ESR registers (internal storage)
  // ═══════════════════════════════════════════════════════════

  logic [EsrIdxCopRegSize-1:0] esr_ctl_q;
  logic [EsrIdxCopRegSize-1:0] esr_pidx_q;
  logic [EsrIdxCopRegSize-1:0] esr_data0_q;
  logic [EsrIdxCopRegSize-1:0] esr_data1_q;
  logic [EsrIdxCopRegSize-1:0] esr_ecc_q;

  // ── Field extraction from ESR ctl register ──────────────
  logic [SmOpcodeSize-1:0]    esr_sm_opcode_w;
  logic                       esr_ecc_wr_en;
  logic [LogicalRamSize-1:0]  esr_logical_ram;
  idx_cop_sm_opcode_e         esr_sm_opcode;

  assign esr_sm_opcode_w = esr_ctl_q[EsrIdxCtlSmOpcodeLsb +: SmOpcodeSize];
  assign esr_ecc_wr_en   = esr_ctl_q[EsrIdxCtlEccWrEnLsb];
  assign esr_logical_ram = esr_ctl_q[EsrIdxCtlLogicalRamLsb +: LogicalRamSize];
  assign esr_sm_opcode   = idx_cop_sm_opcode_e'(esr_sm_opcode_w);

  // ── Field extraction from ESR physical_index register ────
  logic [LineQwIdSize-1:0]    esr_qw;
  logic [SubBankIdSize-1:0]   esr_sub_bank;
  logic [WayIdSize-1:0]       esr_way;
  logic [SetIdSize-1:0]       esr_set;

  assign esr_qw       = esr_pidx_q[EsrIdxQwLsb       +: LineQwIdSize];
  assign esr_sub_bank = esr_pidx_q[EsrIdxSubBankLsb   +: SubBankIdSize];
  assign esr_way      = esr_pidx_q[EsrIdxWayLsb       +: WayIdSize];
  assign esr_set      = esr_pidx_q[EsrIdxSetLsb       +: SetIdSize];

  // ── Field extraction: tag_state ──────────────────────────
  logic [TagStateDataSize-1:0] esr_tag_state;
  logic [TagStateEccSize-1:0]  esr_tag_state_ecc;
  assign esr_tag_state     = esr_data0_q[0 +: TagStateDataSize];
  assign esr_tag_state_ecc = esr_ecc_q[0 +: TagStateEccSize];

  // ── Field extraction: tags (per way) ────────────────────
  logic [TagDataSize-1:0] esr_tag_way0, esr_tag_way1, esr_tag_way2, esr_tag_way3;
  logic [TagEccSize-1:0]  esr_tag_way0_ecc, esr_tag_way1_ecc, esr_tag_way2_ecc, esr_tag_way3_ecc;
  assign esr_tag_way0     = esr_data0_q[EsrData0TagWay0Lsb +: TagDataSize];
  assign esr_tag_way1     = esr_data0_q[EsrData0TagWay1Lsb +: TagDataSize];
  assign esr_tag_way2     = esr_data1_q[EsrData1TagWay2Lsb +: TagDataSize];
  assign esr_tag_way3     = esr_data1_q[EsrData1TagWay3Lsb +: TagDataSize];
  assign esr_tag_way0_ecc = esr_ecc_q[0*TagEccSize +: TagEccSize];
  assign esr_tag_way1_ecc = esr_ecc_q[1*TagEccSize +: TagEccSize];
  assign esr_tag_way2_ecc = esr_ecc_q[2*TagEccSize +: TagEccSize];
  assign esr_tag_way3_ecc = esr_ecc_q[3*TagEccSize +: TagEccSize];

  // ── Field extraction: data ──────────────────────────────
  logic [DwDataSize-1:0] esr_data_dwd0, esr_data_dwd1;
  logic [DwEccSize-1:0]  esr_data_dwd0_ecc, esr_data_dwd1_ecc;
  assign esr_data_dwd0     = esr_data0_q;
  assign esr_data_dwd1     = esr_data1_q;
  assign esr_data_dwd0_ecc = esr_ecc_q[0*DwEccSize +: DwEccSize];
  assign esr_data_dwd1_ecc = esr_ecc_q[1*DwEccSize +: DwEccSize];

  // ═══════════════════════════════════════════════════════════
  // Debug write output (esr_dbg_wr_info_o)
  // ═══════════════════════════════════════════════════════════

  logic [LineQwSize-1:0] qwen_d1_q;

  assign esr_dbg_wr_info_o.ecc_wr_en         = esr_ecc_wr_en;
  assign esr_dbg_wr_info_o.tag_state_en      = (esr_logical_ram == LogicalRamTagState);
  assign esr_dbg_wr_info_o.tag_state         = esr_tag_state;
  assign esr_dbg_wr_info_o.tag_state_and_ecc = {esr_tag_state_ecc, esr_tag_state};
  assign esr_dbg_wr_info_o.tags_en           = (esr_logical_ram == LogicalRamTag);
  assign esr_dbg_wr_info_o.tags              = {esr_tag_way3, esr_tag_way2, esr_tag_way1, esr_tag_way0};
  assign esr_dbg_wr_info_o.tags_and_ecc      = {esr_tag_way3_ecc, esr_tag_way3,
                                                 esr_tag_way2_ecc, esr_tag_way2,
                                                 esr_tag_way1_ecc, esr_tag_way1,
                                                 esr_tag_way0_ecc, esr_tag_way0};
  assign esr_dbg_wr_info_o.data_en           = (esr_logical_ram == LogicalRamData);
  assign esr_dbg_wr_info_o.data              = {LineQwSize{esr_data_dwd1, esr_data_dwd0}};
  assign esr_dbg_wr_info_o.data_and_ecc      = {LineQwSize{esr_data_dwd1_ecc, esr_data_dwd1,
                                                            esr_data_dwd0_ecc, esr_data_dwd0}};
  assign esr_dbg_wr_info_o.data_qwen         = qwen_d1_q;

  // ═══════════════════════════════════════════════════════════
  // ESR register next values (on ESR write)
  // ═══════════════════════════════════════════════════════════

  logic [EsrIdxCopRegSize-1:0] esr_ctl_ld_d;
  logic [EsrIdxCopRegSize-1:0] esr_pidx_ld_d;
  logic [EsrIdxCopRegSize-1:0] esr_data0_ld_d;
  logic [EsrIdxCopRegSize-1:0] esr_data1_ld_d;
  logic [EsrIdxCopRegSize-1:0] esr_ecc_ld_d;

  always_comb begin
    esr_ctl_ld_d  = '0;
    esr_pidx_ld_d = '0;
    esr_ecc_ld_d  = '0;

    // ctl write value (go/abort are captured on write, not stored)
    esr_ctl_ld_d[EsrIdxCtlSmOpcodeLsb   +: SmOpcodeSize]   = esr_ctl_i.wr_data[EsrIdxCtlSmOpcodeLsb   +: SmOpcodeSize];
    esr_ctl_ld_d[EsrIdxCtlEccWrEnLsb    +: 1]              = esr_ctl_i.wr_data[EsrIdxCtlEccWrEnLsb    +: 1];
    esr_ctl_ld_d[EsrIdxCtlLogicalRamLsb +: LogicalRamSize] = esr_ctl_i.wr_data[EsrIdxCtlLogicalRamLsb +: LogicalRamSize];

    // physical_index write value
    esr_pidx_ld_d[EsrIdxQwLsb      +: LineQwIdSize]  = esr_ctl_i.wr_data[EsrIdxQwLsb      +: LineQwIdSize];
    esr_pidx_ld_d[EsrIdxSubBankLsb +: SubBankIdSize] = esr_ctl_i.wr_data[EsrIdxSubBankLsb +: SubBankIdSize];
    esr_pidx_ld_d[EsrIdxWayLsb     +: WayIdSize]     = esr_ctl_i.wr_data[EsrIdxWayLsb     +: WayIdSize];
    esr_pidx_ld_d[EsrIdxSetLsb     +: SetIdSize]     = esr_ctl_i.wr_data[EsrIdxSetLsb     +: SetIdSize];

    // data0/data1 — full 64-bit write data
    esr_data0_ld_d = esr_ctl_i.wr_data;
    esr_data1_ld_d = esr_ctl_i.wr_data;

    // ecc
    esr_ecc_ld_d[0 +: Ways*TagEccSize] = esr_ctl_i.wr_data[0 +: Ways*TagEccSize];
  end

  // ═══════════════════════════════════════════════════════════
  // Start/abort pipeline (2-cycle delay for multicycle ESR timing)
  // ═══════════════════════════════════════════════════════════

  idx_cop_sm_opcode_e esr_ctl_idx_cop_sm_opcode;
  assign esr_ctl_idx_cop_sm_opcode = idx_cop_sm_opcode_e'(esr_ctl_i.wr_data[EsrIdxCtlSmOpcodeLsb +: SmOpcodeSize]);

  logic idx_cop_sm_start_pre2;
  assign idx_cop_sm_start_pre2 =
    (esr_ctl_i.wr_en_idx_cop_sm_ctl && esr_ctl_i.wr_data[EsrIdxCtlGoLsb])
    && !((esr_ctl_i.esr_sc_l2_set_size == '0) && (esr_ctl_idx_cop_sm_opcode inside {IdxCopL2Inv, IdxCopL2Flush, IdxCopL2Evict}))
    && !((esr_ctl_i.esr_sc_l3_set_size == '0) && (esr_ctl_idx_cop_sm_opcode inside {IdxCopL3Inv, IdxCopL3Flush, IdxCopL3Evict}))
    && !((esr_ctl_i.esr_sc_scp_set_size == '0) && (esr_ctl_idx_cop_sm_opcode inside {IdxCopScpZero}));

  logic idx_cop_sm_abort_pre2;
  assign idx_cop_sm_abort_pre2 =
    (mbist_on_i && (state_q[StReset] || state_q[StAllInv])) ||
    (esr_ctl_i.wr_en_idx_cop_sm_ctl && esr_ctl_i.wr_data[EsrIdxCtlAbortLsb]);

  logic idx_cop_sm_start_pre1_q, idx_cop_sm_start_q;
  logic idx_cop_sm_abort_pre1_q, idx_cop_sm_abort_q, idx_cop_sm_abort_d1_q;

  logic start_abort_en;
  assign start_abort_en =
    idx_cop_sm_start_q || idx_cop_sm_start_pre1_q || idx_cop_sm_start_pre2 ||
    idx_cop_sm_abort_q || idx_cop_sm_abort_pre1_q || idx_cop_sm_abort_pre2 ||
    idx_cop_sm_abort_d1_q;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      idx_cop_sm_start_pre1_q <= '0;
      idx_cop_sm_start_q      <= '0;
      idx_cop_sm_abort_pre1_q <= '0;
      idx_cop_sm_abort_q      <= '0;
      idx_cop_sm_abort_d1_q   <= '0;
    end else if (start_abort_en) begin
      idx_cop_sm_start_pre1_q <= idx_cop_sm_start_pre2;
      idx_cop_sm_start_q      <= idx_cop_sm_start_pre1_q;
      idx_cop_sm_abort_pre1_q <= idx_cop_sm_abort_pre2;
      idx_cop_sm_abort_q      <= idx_cop_sm_abort_pre1_q;
      idx_cop_sm_abort_d1_q   <= idx_cop_sm_abort_q;
    end
  end

  // ═══════════════════════════════════════════════════════════
  // Debug read — next ESR values from sub_bank RAM readback
  // ═══════════════════════════════════════════════════════════

  logic [EsrIdxCopRegSize-1:0] dbg_rd_data0_d, dbg_rd_data1_d, dbg_rd_ecc_d;

  // Split tag_state_and_ecc
  logic [TagStateDataSize-1:0] dbg_rd_tag_state;
  logic [TagStateEccSize-1:0]  dbg_rd_tag_state_ecc;
  address_space_e              dbg_rd_tag_state_space;
  assign {dbg_rd_tag_state_ecc, dbg_rd_tag_state} = dbg_rd_info_i.tag_state_and_ecc;
  assign dbg_rd_tag_state_space = dbg_rd_info_i.tag_state_space;

  // Split tags_and_ecc
  logic [TagDataSize-1:0] dbg_rd_tag_way0, dbg_rd_tag_way1, dbg_rd_tag_way2, dbg_rd_tag_way3;
  logic [TagEccSize-1:0]  dbg_rd_tag_way0_ecc, dbg_rd_tag_way1_ecc, dbg_rd_tag_way2_ecc, dbg_rd_tag_way3_ecc;
  assign {dbg_rd_tag_way3_ecc, dbg_rd_tag_way3,
          dbg_rd_tag_way2_ecc, dbg_rd_tag_way2,
          dbg_rd_tag_way1_ecc, dbg_rd_tag_way1,
          dbg_rd_tag_way0_ecc, dbg_rd_tag_way0} = dbg_rd_info_i.tags_and_ecc;

  // Split data_and_ecc per selected QW
  logic [DwDataSize-1:0] dbg_rd_data_qwx_dw0, dbg_rd_data_qwx_dw1;
  logic [DwEccSize-1:0]  dbg_rd_data_qwx_dw0_ecc, dbg_rd_data_qwx_dw1_ecc;
  logic [QwDataAndEccSize-1:0] dbg_rd_data_and_ecc_qwx;
  assign dbg_rd_data_and_ecc_qwx = dbg_rd_info_i.data_and_ecc[esr_qw*QwDataAndEccSize +: QwDataAndEccSize];
  assign {dbg_rd_data_qwx_dw1_ecc, dbg_rd_data_qwx_dw1,
          dbg_rd_data_qwx_dw0_ecc, dbg_rd_data_qwx_dw0} = dbg_rd_data_and_ecc_qwx;

  always_comb begin
    logic [EsrIdxCopRegSize-1:0] ts_data0, ts_data1, ts_ecc;
    logic [EsrIdxCopRegSize-1:0] tg_data0, tg_data1, tg_ecc;
    logic [EsrIdxCopRegSize-1:0] dt_data0, dt_data1, dt_ecc;

    // tag_state
    ts_data0 = '0;
    ts_data0[0 +: TagStateDataSize]                                    = dbg_rd_tag_state;
    ts_data0[EsrData0TagStateSpaceLsb +: EsrData0TagStateSpaceSize]    = dbg_rd_tag_state_space;
    ts_data1 = '0;
    ts_ecc   = '0;
    ts_ecc[0 +: TagStateEccSize] = dbg_rd_tag_state_ecc;

    // tags
    tg_data0 = '0;
    tg_data0[EsrData0TagWay0Lsb +: TagDataSize] = dbg_rd_tag_way0;
    tg_data0[EsrData0TagWay1Lsb +: TagDataSize] = dbg_rd_tag_way1;
    tg_data1 = '0;
    tg_data1[EsrData1TagWay2Lsb +: TagDataSize] = dbg_rd_tag_way2;
    tg_data1[EsrData1TagWay3Lsb +: TagDataSize] = dbg_rd_tag_way3;
    tg_ecc = '0;
    tg_ecc[0 +: Ways*TagEccSize] = {dbg_rd_tag_way3_ecc, dbg_rd_tag_way2_ecc,
                                     dbg_rd_tag_way1_ecc, dbg_rd_tag_way0_ecc};

    // data
    dt_data0 = dbg_rd_data_qwx_dw0;
    dt_data1 = dbg_rd_data_qwx_dw1;
    dt_ecc   = '0;
    dt_ecc[0 +: 2*DwEccSize] = {dbg_rd_data_qwx_dw1_ecc, dbg_rd_data_qwx_dw0_ecc};

    // Priority mux: tag_state > tags > data
    dbg_rd_data0_d = dbg_rd_info_i.tag_state_and_ecc_valid ? ts_data0 :
                     dbg_rd_info_i.tags_and_ecc_valid      ? tg_data0 : dt_data0;
    dbg_rd_data1_d = dbg_rd_info_i.tag_state_and_ecc_valid ? ts_data1 :
                     dbg_rd_info_i.tags_and_ecc_valid      ? tg_data1 : dt_data1;
    dbg_rd_ecc_d   = dbg_rd_info_i.tag_state_and_ecc_valid ? ts_ecc :
                     dbg_rd_info_i.tags_and_ecc_valid      ? tg_ecc : dt_ecc;
  end

  logic dbg_rd_valid;
  assign dbg_rd_valid = dbg_rd_info_i.tag_state_and_ecc_valid ||
                        dbg_rd_info_i.tags_and_ecc_valid ||
                        dbg_rd_info_i.data_and_ecc_valid;

  // ═══════════════════════════════════════════════════════════
  // ESR register mux (ESR write vs debug read) and storage
  // ═══════════════════════════════════════════════════════════

  logic [EsrIdxCopRegSize-1:0] esr_ctl_d;
  logic [EsrIdxCopRegSize-1:0] esr_pidx_d;
  logic [EsrIdxCopRegSize-1:0] esr_data0_d, esr_data1_d, esr_ecc_d;

  assign esr_ctl_d   = esr_ctl_ld_d;
  assign esr_pidx_d  = esr_pidx_ld_d;
  assign esr_data0_d = esr_ctl_i.wr_en_idx_cop_sm_data0 ? esr_data0_ld_d : dbg_rd_data0_d;
  assign esr_data1_d = esr_ctl_i.wr_en_idx_cop_sm_data1 ? esr_data1_ld_d : dbg_rd_data1_d;
  assign esr_ecc_d   = esr_ctl_i.wr_en_idx_cop_sm_ecc   ? esr_ecc_ld_d   : dbg_rd_ecc_d;

  always_ff @(posedge clk_i) begin
    if (esr_ctl_i.wr_en_idx_cop_sm_ctl)            esr_ctl_q   <= esr_ctl_d;
    if (esr_ctl_i.wr_en_idx_cop_sm_physical_index)  esr_pidx_q  <= esr_pidx_d;
    if (esr_ctl_i.wr_en_idx_cop_sm_data0 || dbg_rd_valid) esr_data0_q <= esr_data0_d;
    if (esr_ctl_i.wr_en_idx_cop_sm_data1 || dbg_rd_valid) esr_data1_q <= esr_data1_d;
    if (esr_ctl_i.wr_en_idx_cop_sm_ecc   || dbg_rd_valid) esr_ecc_q   <= esr_ecc_d;
  end

  // ═══════════════════════════════════════════════════════════
  // ESR register read interface
  // ═══════════════════════════════════════════════════════════

  logic [EsrIdxCopRegSize-1:0] esr_ctl_read;
  logic [EsrIdxCopRegSize-1:0] esr_pidx_read;

  always_comb begin
    esr_ctl_read = '0;
    esr_ctl_read[EsrIdxCtlGoLsb        +: 1]              = 1'b0; // read-back 0
    esr_ctl_read[EsrIdxCtlAbortLsb     +: 1]              = 1'b0; // read-back 0
    esr_ctl_read[EsrIdxCtlSmOpcodeLsb  +: SmOpcodeSize]   = esr_ctl_q[EsrIdxCtlSmOpcodeLsb  +: SmOpcodeSize];
    esr_ctl_read[EsrIdxCtlEccWrEnLsb   +: 1]              = esr_ctl_q[EsrIdxCtlEccWrEnLsb   +: 1];
    esr_ctl_read[EsrIdxCtlLogicalRamLsb+: LogicalRamSize] = esr_ctl_q[EsrIdxCtlLogicalRamLsb+: LogicalRamSize];
    esr_ctl_read[EsrIdxCtlSmStateLsb   +: States]         = state_q; // read-only

    esr_pidx_read = '0;
    esr_pidx_read[EsrIdxQwLsb      +: LineQwIdSize]  = esr_pidx_q[EsrIdxQwLsb      +: LineQwIdSize];
    esr_pidx_read[EsrIdxSubBankLsb +: SubBankIdSize] = esr_pidx_q[EsrIdxSubBankLsb +: SubBankIdSize];
    esr_pidx_read[EsrIdxWayLsb     +: WayIdSize]     = esr_pidx_q[EsrIdxWayLsb     +: WayIdSize];
    esr_pidx_read[EsrIdxSetLsb     +: SetIdSize]     = esr_pidx_q[EsrIdxSetLsb     +: SetIdSize];
  end

  assign pipe_idx_cop_sm_esr_status_o.ctl            = esr_ctl_read;
  assign pipe_idx_cop_sm_esr_status_o.physical_index = esr_pidx_read;
  assign pipe_idx_cop_sm_esr_status_o.data0          = esr_data0_q;
  assign pipe_idx_cop_sm_esr_status_o.data1          = esr_data1_q;
  assign pipe_idx_cop_sm_esr_status_o.ecc            = EsrIdxCopRegSize'(esr_ecc_q[0 +: Ways*TagEccSize]);

  // ═══════════════════════════════════════════════════════════
  // Reset counter
  // ═══════════════════════════════════════════════════════════
  //
  // Counts down from 255 after reset. When it reaches 0, triggers
  // the power_up_all_inv (unless warm-reset-only or mbist_on).

  logic [ResetCountSize-1:0] reset_count_q;
  logic                      reset_count_enable;
  logic [ResetCountSize-1:0] reset_count_d;

  assign reset_count_enable = |reset_count_q;
  assign reset_count_d      = reset_count_q - ResetCountSize'(1);

  logic idx_cop_sm_start_power_up_all_inv_abort_check;
  logic idx_cop_sm_start_power_up_all_inv_abort;
  logic idx_cop_sm_start_power_up_all_inv;

  assign idx_cop_sm_start_power_up_all_inv_abort_check =
    state_q[StReset] && reset_count_enable && (reset_count_d == {{ResetCountSize-1{1'b0}}, 1'b1});
  assign idx_cop_sm_start_power_up_all_inv_abort =
    idx_cop_sm_start_power_up_all_inv_abort_check && !reset_c_save_q;
  assign idx_cop_sm_start_power_up_all_inv =
    state_q[StReset] && reset_count_enable && (reset_count_d == '0);

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                    reset_count_q <= '1;
    else if (reset_count_enable)   reset_count_q <= reset_count_d;
  end

  // Clear reset_c_save after checking, or on abort (not on mbist_on)
  assign clr_reset_c_save = idx_cop_sm_start_power_up_all_inv_abort_check || idx_cop_sm_abort_q;

  // ═══════════════════════════════════════════════════════════
  // Load and count enable
  // ═══════════════════════════════════════════════════════════

  logic idx_cop_sm_load;
  logic idx_cop_sm_count_enable;

  assign idx_cop_sm_load =
    !idx_cop_sm_abort_q &&
    ((state_q[StReset] && idx_cop_sm_start_power_up_all_inv) ||
     (state_q[StIdle]  && idx_cop_sm_start_q));

  assign idx_cop_sm_count_enable =
    state_q[StAllInv] || state_q[StActive] || state_q[StCbInv] || state_q[StDbg] || state_q[StSync];

  // ═══════════════════════════════════════════════════════════
  // State machine — next state
  // ═══════════════════════════════════════════════════════════

  logic last_index_taken_d0;
  logic sync_taken_d1;

  idx_cop_sm_opcode_e esr_sm_opcode_d;

  always_comb begin
    state_d = '0;
    if (!rst_n) begin
      state_d[StReset] = 1'b1;
    end else begin
      unique case (1'b1)
        state_q[StReset]: begin
          if (idx_cop_sm_abort_q || mbist_on_i || SkipPowerUpAllInv || idx_cop_sm_start_power_up_all_inv_abort) begin
            state_d[StIdle] = 1'b1;
          end else if (idx_cop_sm_start_power_up_all_inv) begin
            state_d[StAllInv] = 1'b1;
          end else begin
            state_d[StReset] = 1'b1;
          end
        end
        state_q[StAllInv]: begin
          if (idx_cop_sm_abort_q)         state_d[StIdle]   = 1'b1;
          else if (last_index_taken_d0)   state_d[StSync]   = 1'b1;
          else                            state_d[StAllInv] = 1'b1;
        end
        state_q[StIdle]: begin
          if (!idx_cop_sm_abort_q && idx_cop_sm_start_q) begin
            if (esr_sm_opcode == IdxCopAllInv)                                    state_d[StAllInv] = 1'b1;
            else if (esr_sm_opcode == IdxCopCBInv)                                state_d[StCbInv]  = 1'b1;
            else if (esr_sm_opcode inside {IdxCopDbgRead, IdxCopDbgWrite})        state_d[StDbg]    = 1'b1;
            else                                                                  state_d[StActive] = 1'b1;
          end else begin
            state_d[StIdle] = 1'b1;
          end
        end
        state_q[StActive]: begin
          if (idx_cop_sm_abort_q)         state_d[StIdle]   = 1'b1;
          else if (last_index_taken_d0)   state_d[StSync]   = 1'b1;
          else                            state_d[StActive] = 1'b1;
        end
        state_q[StCbInv]: begin
          if (idx_cop_sm_abort_q)         state_d[StIdle]  = 1'b1;
          else if (last_index_taken_d0)   state_d[StSync]  = 1'b1;
          else                            state_d[StCbInv] = 1'b1;
        end
        state_q[StDbg]: begin
          if (idx_cop_sm_abort_q || last_index_taken_d0) state_d[StIdle] = 1'b1;
          else                                           state_d[StDbg]  = 1'b1;
        end
        state_q[StSync]: begin
          if (idx_cop_sm_abort_q || sync_taken_d1) state_d[StIdle] = 1'b1;
          else                                     state_d[StSync] = 1'b1;
        end
        default: state_d[StReset] = 1'b1;
      endcase
    end
  end

  logic state_en;
  assign state_en = (state_d != state_q);

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)          state_q <= ResetState;
    else if (state_en)   state_q <= state_d;
  end

  assign pipe_idx_cop_is_active_o = ~state_q[StIdle];

  // ═══════════════════════════════════════════════════════════
  // Operation parameters (calculated at start)
  // ═══════════════════════════════════════════════════════════

  logic [SetCountSize-1:0]       base_d;
  logic [SetCountSize-1:0]       size_d;
  logic [SetCountSize-1:0]       start_set_d;
  logic [WayIdSize-1:0]          start_way_d;
  logic [SubBankIdSize-1:0]      start_sbb_d;
  logic [LineQwIdSize-1:0]       start_qwd_d;
  logic [SetCountSize-1:0]       compare_set_d;
  reqq_opcode_e                  req_opcode_d;

  always_comb begin
    if (idx_cop_sm_start_power_up_all_inv) begin
      base_d = '0;
      size_d = AllInvSetCount;
    end else begin
      unique case (esr_sm_opcode)
        IdxCopAllInv: begin
          base_d = '0;
          size_d = AllInvSetCount;
        end
        IdxCopL2Inv, IdxCopL2Flush, IdxCopL2Evict: begin
          base_d = SetCountSize'(esr_ctl_i.esr_sc_l2_set_base);
          size_d = esr_ctl_i.esr_sc_l2_set_size;
        end
        IdxCopL3Inv, IdxCopL3Flush, IdxCopL3Evict: begin
          base_d = SetCountSize'(esr_ctl_i.esr_sc_l3_set_base);
          size_d = esr_ctl_i.esr_sc_l3_set_size;
        end
        IdxCopDbgRead, IdxCopDbgWrite: begin
          base_d = SetCountSize'(esr_set);
          size_d = SetCountSize'(1'b1);
        end
        IdxCopScpZero: begin
          base_d = SetCountSize'(esr_ctl_i.esr_sc_scp_set_base);
          size_d = esr_ctl_i.esr_sc_scp_set_size;
        end
        default: begin // IdxCopCBInv
          base_d = '0;
          size_d = CbInvCount;
        end
      endcase
    end

    start_set_d   = base_d;
    compare_set_d = base_d + size_d;

    start_way_d = (!idx_cop_sm_start_power_up_all_inv &&
                   (esr_sm_opcode inside {IdxCopDbgRead, IdxCopDbgWrite})) ? esr_way : '0;
    start_sbb_d = (!idx_cop_sm_start_power_up_all_inv &&
                   (esr_sm_opcode inside {IdxCopDbgRead, IdxCopDbgWrite})) ? esr_sub_bank : '0;
    start_qwd_d = (!idx_cop_sm_start_power_up_all_inv &&
                   (esr_sm_opcode inside {IdxCopDbgRead, IdxCopDbgWrite})) ? esr_qw : '0;

    if (idx_cop_sm_start_power_up_all_inv) begin
      esr_sm_opcode_d = IdxCopAllInv;
      req_opcode_d    = IdxAllInv;
    end else begin
      esr_sm_opcode_d = esr_sm_opcode;
      unique case (esr_sm_opcode)
        IdxCopAllInv  : req_opcode_d = IdxAllInv;
        IdxCopL2Inv   : req_opcode_d = IdxL2Inv;
        IdxCopL2Flush : req_opcode_d = IdxL2Flush;
        IdxCopL2Evict : req_opcode_d = IdxL2Evict;
        IdxCopL3Inv   : req_opcode_d = IdxL3Inv;
        IdxCopL3Flush : req_opcode_d = IdxL3Flush;
        IdxCopL3Evict : req_opcode_d = IdxL3Evict;
        IdxCopDbgRead : req_opcode_d = IdxRead;
        IdxCopDbgWrite: req_opcode_d = IdxWrite;
        IdxCopScpZero : req_opcode_d = ScpZero;
        default       : req_opcode_d = L2Evict; // CB_Inv
      endcase
    end
  end

  // ── Latched operation registers ─────────────────────────
  logic [SetCountSize-1:0]  compare_set_xx_q;
  reqq_opcode_e             req_opcode_xx_q;
  idx_cop_sm_opcode_e       esr_sm_opcode_xx_q;
  logic                     cb_inv_op_xx_q;

  always_ff @(posedge clk_i) begin
    if (idx_cop_sm_load) begin
      compare_set_xx_q  <= compare_set_d;
      req_opcode_xx_q   <= req_opcode_d;
      esr_sm_opcode_xx_q<= esr_sm_opcode_d;
      cb_inv_op_xx_q    <= (esr_sm_opcode_d == IdxCopCBInv);
    end
  end

  // ═══════════════════════════════════════════════════════════
  // Set/way/subbank/qw counters
  // ═══════════════════════════════════════════════════════════

  logic [SetCountSize-1:0]       set_d0_q;
  logic [WayIdSize-1:0]          way_d0_q;
  logic [SubBankIdSize-1:0]      sbb_d0_q;
  logic [LineQwIdSize-1:0]       qwd_d0_q;

  logic [SetCountSize-1:0]       set_d0_d;
  logic [WayIdSize-1:0]          way_d0_d;
  logic [SubBankIdSize-1:0]      sbb_d0_d;
  logic [LineQwIdSize-1:0]       qwd_d0_d;

  always_comb begin
    if (idx_cop_sm_load) begin
      set_d0_d = start_set_d;
      way_d0_d = start_way_d;
      sbb_d0_d = start_sbb_d;
      qwd_d0_d = start_qwd_d;
    end else begin
      set_d0_d = (((way_d0_q == '1) || state_q[StAllInv] || state_q[StCbInv]) &&
                  ((sbb_d0_q == '1) || state_q[StCbInv]) &&
                  ((qwd_d0_q == '1) || !state_q[StDbg])) ? set_d0_q + SetCountSize'(1) : set_d0_q;
      way_d0_d = (!state_q[StAllInv] && !state_q[StCbInv] &&
                  (sbb_d0_q == '1) &&
                  ((qwd_d0_q == '1) || !state_q[StDbg])) ? way_d0_q + WayIdSize'(1) : way_d0_q;
      sbb_d0_d = (!state_q[StCbInv] &&
                  ((qwd_d0_q == '1) || !state_q[StDbg])) ? sbb_d0_q + SubBankIdSize'(1) : sbb_d0_q;
      qwd_d0_d = (!state_q[StDbg]) ? qwd_d0_q + LineQwIdSize'(1) : qwd_d0_q;
    end
  end

  logic incr_count_d0;
  assign incr_count_d0 = idx_cop_sm_count_enable && (!pipe_idx_cop_req_valid_o || pipe_idx_cop_req_ready_i);

  always_ff @(posedge clk_i) begin
    if (idx_cop_sm_load || incr_count_d0) begin
      set_d0_q <= set_d0_d;
      way_d0_q <= way_d0_d;
      sbb_d0_q <= sbb_d0_d;
      qwd_d0_q <= qwd_d0_d;
    end
  end

  // ═══════════════════════════════════════════════════════════
  // L2/L3 busy
  // ═══════════════════════════════════════════════════════════

  logic l2_busy_d, l3_busy_d;
  assign l2_busy_d = idx_cop_sm_start_power_up_all_inv ? 1'b1 :
    (esr_sm_opcode inside {IdxCopAllInv, IdxCopL2Inv, IdxCopL2Flush, IdxCopL2Evict, IdxCopScpZero});
  assign l3_busy_d = idx_cop_sm_start_power_up_all_inv ? 1'b1 :
    (esr_sm_opcode inside {IdxCopAllInv, IdxCopL3Inv, IdxCopL3Flush, IdxCopL3Evict, IdxCopScpZero});

  assign pipe_idx_cop_aborted_o = idx_cop_sm_abort_d1_q;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      pipe_idx_cop_l2_busy_o <= !SkipPowerUpAllInv;
      pipe_idx_cop_l3_busy_o <= !SkipPowerUpAllInv;
    end else if (idx_cop_sm_load || (!state_q[StIdle] && state_d[StIdle])) begin
      pipe_idx_cop_l2_busy_o <= idx_cop_sm_load && l2_busy_d;
      pipe_idx_cop_l3_busy_o <= idx_cop_sm_load && l3_busy_d;
    end
  end

  // ═══════════════════════════════════════════════════════════
  // D0 stage — address/opcode/valid computation
  // ═══════════════════════════════════════════════════════════

  reqq_opcode_e req_opcode_d0;

  // SCP address fabrication
  logic [PaSize-1:0] addr_msbs_d0;
  assign addr_msbs_d0 = (esr_sm_opcode_xx_q == IdxCopScpZero) ?
    (PaSize'({my_scp_virtual_shire_id_i, {ScpShireLsb{1'b0}}})) |
    ({ScpRegionId, {ScpRegionLsb{1'b0}}}) :
    '0;

  logic                   last_index_d0;
  logic [IdxSize-1:0]     index_d0;
  logic [PaSize-1:0]      address_from_index_d0;
  logic                   cb_set_valid_d0;
  logic [PaSize-1:0]      cb_addr_d0;
  logic                   cb_valid_d0;
  logic                   valid_d0;
  logic [PaSize-1:0]      address_d0;
  logic                   sync_flush_victims_d0;
  logic [LineQwSize-1:0]  qwen_d0;

  assign last_index_d0         = (set_d0_d == compare_set_xx_q) || state_q[StDbg];
  assign index_d0              = {set_d0_q, way_d0_q, sbb_d0_q};
  assign address_from_index_d0 = PaSize'({index_d0, my_bank_id_i, {BankLsb{1'b0}}}) | addr_msbs_d0;
  assign cb_set_valid_d0       = set_d0_q < CbufCount;
  assign cb_addr_d0            = pipe_cbuf_state_info_i.cbuf_mem[set_d0_q[0 +: CbufIdSize]];
  assign cb_valid_d0           = pipe_cbuf_state_info_i.cbuf_mem_valid[set_d0_q[0 +: CbufIdSize]];
  assign valid_d0              = state_q[StCbInv] ? (cb_valid_d0 && cb_set_valid_d0) : 1'b1;
  assign address_d0            = state_q[StCbInv] ? cb_addr_d0 : address_from_index_d0;
  assign req_opcode_d0         = state_q[StSync]  ? OpSync : req_opcode_xx_q;
  assign sync_flush_victims_d0 = state_q[StSync] && cb_inv_op_xx_q;
  assign last_index_taken_d0   = incr_count_d0 && last_index_d0;
  assign qwen_d0               = LineQwSize'(1'b1) << qwd_d0_q;

  // ═══════════════════════════════════════════════════════════
  // D1 stage — output pipeline register
  // ═══════════════════════════════════════════════════════════

  logic                   valid_d1_q;
  logic [PaSize-1:0]      address_d1_q;
  reqq_opcode_e           req_opcode_d1_q;
  logic                   sync_flush_victims_d1_q;

  logic valid_d1_enable;
  logic valid_d1_d;
  assign valid_d1_enable = idx_cop_sm_abort_q || (valid_d1_q && pipe_idx_cop_req_ready_i) || incr_count_d0;
  assign valid_d1_d      = !idx_cop_sm_abort_q && (incr_count_d0 && valid_d0 && (!state_d[StIdle] || state_q[StDbg]));

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)                valid_d1_q <= '0;
    else if (valid_d1_enable)  valid_d1_q <= valid_d1_d;
  end

  always_ff @(posedge clk_i) begin
    if (incr_count_d0) begin
      address_d1_q            <= address_d0;
      req_opcode_d1_q         <= req_opcode_d0;
      sync_flush_victims_d1_q <= sync_flush_victims_d0;
      qwen_d1_q               <= qwen_d0;
    end
  end

  assign sync_taken_d1 = (req_opcode_d1_q == OpSync) && pipe_idx_cop_req_ready_i;

  // ═══════════════════════════════════════════════════════════
  // Output assignments
  // ═══════════════════════════════════════════════════════════

  assign pipe_idx_cop_req_valid_o                   = valid_d1_q;
  assign pipe_idx_cop_req_info_o.trans_id           = '0;
  assign pipe_idx_cop_req_info_o.sync_flush_victims = sync_flush_victims_d1_q;
  assign pipe_idx_cop_req_info_o.address            = address_d1_q;
  assign pipe_idx_cop_req_info_o.opcode             = req_opcode_d1_q;

  // ═══════════════════════════════════════════════════════════
  // Assertions
  // ═══════════════════════════════════════════════════════════

  // synthesis translate_off
  `ifdef VERILATOR
  /* verilator lint_off UNUSEDSIGNAL */  // assertion-only helper signals not read by synthesizable logic
  logic l2_busy_assert, l3_busy_assert;
  assign l2_busy_assert = (esr_sm_opcode_xx_q inside {IdxCopAllInv, IdxCopL2Inv, IdxCopL2Flush, IdxCopL2Evict, IdxCopScpZero});
  assign l3_busy_assert = (esr_sm_opcode_xx_q inside {IdxCopAllInv, IdxCopL3Inv, IdxCopL3Flush, IdxCopL3Evict, IdxCopScpZero});
  /* verilator lint_on UNUSEDSIGNAL */

  always_ff @(posedge clk_i) begin
    if (rst_n && l2_busy_assert && (state_q[StAllInv] || state_q[StActive] || state_q[StSync]))
      if (!pipe_idx_cop_l2_busy_o)
        $error("idx_cop_sm: l2_busy must be high during operation");
    if (rst_n && l3_busy_assert && (state_q[StAllInv] || state_q[StActive] || state_q[StSync]))
      if (!pipe_idx_cop_l3_busy_o)
        $error("idx_cop_sm: l3_busy must be high during operation");
  end
  `endif
  // synthesis translate_on

  // Unused signal lint suppression
  logic unused_ok;
  assign unused_ok = &{1'b0, esr_ctl_i, my_scp_virtual_shire_id_i,
                       esr_ctl_q, esr_pidx_q, esr_ecc_q};

  /* verilator lint_on SYNCASYNCNET */

endmodule : shirecache_pipe_idx_cop_sm
