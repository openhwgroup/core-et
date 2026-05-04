// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache performance monitor control.
//
// Monitors performance signals and counts based on the perf counter qualifier.
// Three independent state machines (cyc, P0, P1) with states:
//   IDLE → START → DLY1 → DLY2 → DLY3 → ACTIVE
// ESR register write interface for ctl_status, cyc/p0/p1 counters and qualifiers.
// Counter increment/overflow/saturation logic with overflow interrupt generation.
//
// Replaces: shire_cache_perfmon_ctl (etcore-soc)

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_perfmon_ctl
  import dft_pkg::*;
#(
  parameter int unsigned EsrPerfmonNumPerfCntr  = 2,
  parameter int unsigned EsrPerfmonPerfQualSize = 51,
  parameter int unsigned EsrPerfmonPerfQual2Size = 1,
  parameter int unsigned EsrPerfmonStatusLsb    = 32,
  parameter int unsigned EsrPerfmonStatusSize   = 6,
  parameter int unsigned EsrPerfmonCsrSize      = EsrPerfmonStatusLsb + EsrPerfmonStatusSize,
  parameter int unsigned EsrPerfmonCntrSize     = 40,
  parameter int unsigned EsrPerfmonCovfSize     = EsrPerfmonCntrSize + 1,
  parameter int unsigned EsrPerfmonRegSize      = 64
) (
  // Free-running and perfmon gated clock
  input  logic                                                                clk_free_i,
  output logic                                                                clk_gated_o,
  input  logic                                                                rst_ni,
  input  dft_t                                                                dft_i,

  // Performance monitor saturation errors
  output logic                                                                cyc_cntr_overflow_intr_o,
  output logic                                                                p0_cntr_overflow_intr_o,
  output logic                                                                p1_cntr_overflow_intr_o,

  // ESRs
  input  logic [EsrPerfmonRegSize-1:0]                                        esr_wr_data_i,
  input  logic                                                                esr_wr_en_perfmon_ctl_status_i,
  input  logic                                                                esr_wr_en_perfmon_cyc_cntr_i,
  input  logic                                                                esr_wr_en_perfmon_p0_cntr_i,
  input  logic                                                                esr_wr_en_perfmon_p1_cntr_i,
  input  logic                                                                esr_wr_en_perfmon_p0_qual_i,
  input  logic                                                                esr_wr_en_perfmon_p1_qual_i,
  input  logic                                                                esr_wr_en_perfmon_p0_qual2_i,
  input  logic                                                                esr_wr_en_perfmon_p1_qual2_i,

  // perfmon_qual interface
  output logic                                                                perfmon_cgater_o,
  output logic [EsrPerfmonNumPerfCntr-1:0]                                    perfmon_enable_o,
  output logic [EsrPerfmonPerfQualSize-1:0]                                   esr_perfmon_p0_qual_o,
  output logic [EsrPerfmonPerfQualSize-1:0]                                   esr_perfmon_p1_qual_o,
  output logic [EsrPerfmonPerfQual2Size-1:0]                                  esr_perfmon_p0_qual2_o,
  output logic [EsrPerfmonPerfQual2Size-1:0]                                  esr_perfmon_p1_qual2_o,
  output logic [EsrPerfmonCsrSize-1:0]                                        esr_perfmon_ctl_status_o,
  output logic [EsrPerfmonRegSize-1:0]                                        esr_perfmon_ctl_status_read_o,
  output logic [EsrPerfmonCovfSize-1:0]                                       esr_perfmon_cyc_cntr_o,
  output logic [EsrPerfmonCovfSize-1:0]                                       esr_perfmon_p0_cntr_o,
  output logic [EsrPerfmonCovfSize-1:0]                                       esr_perfmon_p1_cntr_o,
  input  logic [EsrPerfmonNumPerfCntr-1:0]                                    perfmon_update_cntr_i,
  input  logic [EsrPerfmonNumPerfCntr-1:0][EsrPerfmonCovfSize-1:0]            esr_perfmon_update_cntr_i,
  output logic                                                                perfmon_p0_cntr_start_reset_o,
  output logic                                                                perfmon_p1_cntr_start_reset_o
);

  // ════════════════════════════════════════════════════════════
  // Perfmon state machine parameters — one-hot encoding
  // ════════════════════════════════════════════════════════════

  localparam int unsigned PERFMON_IDLE   = 0;
  localparam int unsigned PERFMON_START  = 1;
  localparam int unsigned PERFMON_DLY1   = 2;
  localparam int unsigned PERFMON_DLY2   = 3;
  localparam int unsigned PERFMON_DLY3   = 4;
  localparam int unsigned PERFMON_ACTIVE = 5;
  localparam int unsigned PERFMON_STATES = 6;

  logic [PERFMON_STATES-1:0] cyc_cntr_state_d;
  logic [PERFMON_STATES-1:0] cyc_cntr_state_q;
  logic [PERFMON_STATES-1:0] p0_cntr_state_d;
  logic [PERFMON_STATES-1:0] p0_cntr_state_q;
  logic [PERFMON_STATES-1:0] p1_cntr_state_d;
  logic [PERFMON_STATES-1:0] p1_cntr_state_q;

  // ════════════════════════════════════════════════════════════
  // Local parameters — ESR field positions and sizes
  // ════════════════════════════════════════════════════════════

  localparam int unsigned ESR_PERFMON_CTL_LSB        = 0;
  localparam int unsigned ESR_PERFMON_CYC_CNTR_LSB   = 0;
  localparam int unsigned ESR_PERFMON_PERF_QUAL_LSB  = 0;
  localparam int unsigned ESR_PERFMON_PERF_QUAL2_LSB = 0;
  localparam int unsigned ESR_PERFMON_PERF_CNTR_LSB  = 0;

  localparam int unsigned ESR_PERFMON_CTL_SIZE       = 31;

  // Control and Status bit positions
  localparam int unsigned ESR_PERFMON_CYC_START_VALID        = 0;
  localparam int unsigned ESR_PERFMON_CYC_START_RESET        = 1;
  localparam int unsigned ESR_PERFMON_CYC_STOP_OVERFLOW      = 2;
  localparam int unsigned ESR_PERFMON_CYC_STOP_INTR_OVERFLOW = 3;
  localparam int unsigned ESR_PERFMON_CYC_STATUS_ACTIVE      = 32;
  localparam int unsigned ESR_PERFMON_CYC_STATUS_OVERFLOW    = 33;

  localparam int unsigned ESR_PERFMON_P0_START_VALID         = 4;
  localparam int unsigned ESR_PERFMON_P0_START_RESET         = 5;
  localparam int unsigned ESR_PERFMON_P0_STOP_OVERFLOW       = 6;
  localparam int unsigned ESR_PERFMON_P0_STOP_INTR_OVERFLOW  = 7;
  localparam int unsigned ESR_PERFMON_P0_STATUS_ACTIVE       = 34;
  localparam int unsigned ESR_PERFMON_P0_STATUS_OVERFLOW     = 35;

  localparam int unsigned ESR_PERFMON_P1_START_VALID         = 17;
  localparam int unsigned ESR_PERFMON_P1_START_RESET         = 18;
  localparam int unsigned ESR_PERFMON_P1_STOP_OVERFLOW       = 19;
  localparam int unsigned ESR_PERFMON_P1_STOP_INTR_OVERFLOW  = 20;
  localparam int unsigned ESR_PERFMON_P1_STATUS_ACTIVE       = 36;
  localparam int unsigned ESR_PERFMON_P1_STATUS_OVERFLOW     = 37;

  localparam int unsigned ESR_PERFMON_ANY_STOP_OVERFLOW      = 30;

  // ════════════════════════════════════════════════════════════
  // Clock gater
  // ════════════════════════════════════════════════════════════

  logic clk_gated;
  logic esr_wr_en_perfmon;
  logic perfmon_active;
  logic clock_en;

  assign clock_en = perfmon_active | esr_wr_en_perfmon | ~rst_ni;

  prim_clk_gate u_clk_gate (
    .clk_i (clk_free_i),
    .en_i  (clock_en),
    .dft_i (dft_i),
    .clk_o (clk_gated)
  );

  assign clk_gated_o = clk_gated;

  // ════════════════════════════════════════════════════════════
  // ESR interface — load values from write data
  // ════════════════════════════════════════════════════════════

  logic [EsrPerfmonCsrSize-1:0]       esr_perfmon_ctl_status_ld_d;
  logic [EsrPerfmonCovfSize-1:0]      esr_perfmon_cyc_cntr_ld_d;
  logic [EsrPerfmonCovfSize-1:0]      esr_perfmon_p0_cntr_ld_d;
  logic [EsrPerfmonCovfSize-1:0]      esr_perfmon_p1_cntr_ld_d;
  logic [EsrPerfmonPerfQualSize-1:0]  esr_perfmon_p0_qual_ld_d;
  logic [EsrPerfmonPerfQualSize-1:0]  esr_perfmon_p1_qual_ld_d;
  logic [EsrPerfmonPerfQual2Size-1:0] esr_perfmon_p0_qual2_ld_d;
  logic [EsrPerfmonPerfQual2Size-1:0] esr_perfmon_p1_qual2_ld_d;

  logic [EsrPerfmonCsrSize-1:0]       esr_perfmon_ctl_status_d;
  logic [EsrPerfmonCovfSize-1:0]      esr_perfmon_cyc_cntr_d;
  logic [EsrPerfmonCovfSize-1:0]      esr_perfmon_p0_cntr_d;
  logic [EsrPerfmonCovfSize-1:0]      esr_perfmon_p1_cntr_d;
  logic [EsrPerfmonPerfQualSize-1:0]  esr_perfmon_p0_qual_d;
  logic [EsrPerfmonPerfQualSize-1:0]  esr_perfmon_p1_qual_d;
  logic [EsrPerfmonPerfQual2Size-1:0] esr_perfmon_p0_qual2_d;
  logic [EsrPerfmonPerfQual2Size-1:0] esr_perfmon_p1_qual2_d;

  logic [EsrPerfmonCovfSize-1:0]      perfmon_cyc_cntr_d;
  logic [EsrPerfmonCovfSize-1:0]      perfmon_p0_cntr_d;
  logic [EsrPerfmonCovfSize-1:0]      perfmon_p1_cntr_d;

  always_comb begin
    esr_perfmon_ctl_status_ld_d = '0;
    esr_perfmon_cyc_cntr_ld_d   = '0;
    esr_perfmon_p0_cntr_ld_d    = '0;
    esr_perfmon_p1_cntr_ld_d    = '0;
    esr_perfmon_p0_qual_ld_d    = '0;
    esr_perfmon_p1_qual_ld_d    = '0;
    esr_perfmon_p0_qual2_ld_d   = '0;
    esr_perfmon_p1_qual2_ld_d   = '0;

    // ESR write value
    esr_perfmon_ctl_status_ld_d[ESR_PERFMON_CTL_LSB+:ESR_PERFMON_CTL_SIZE] =
      esr_wr_data_i[ESR_PERFMON_CTL_LSB+:ESR_PERFMON_CTL_SIZE];

    esr_perfmon_cyc_cntr_ld_d = {1'b0, esr_wr_data_i[ESR_PERFMON_CYC_CNTR_LSB+:EsrPerfmonCntrSize]};
    esr_perfmon_p0_cntr_ld_d  = {1'b0, esr_wr_data_i[ESR_PERFMON_PERF_CNTR_LSB+:EsrPerfmonCntrSize]};
    esr_perfmon_p1_cntr_ld_d  = {1'b0, esr_wr_data_i[ESR_PERFMON_PERF_CNTR_LSB+:EsrPerfmonCntrSize]};
    esr_perfmon_p0_qual_ld_d  = esr_wr_data_i[ESR_PERFMON_PERF_QUAL_LSB+:EsrPerfmonPerfQualSize];
    esr_perfmon_p1_qual_ld_d  = esr_wr_data_i[ESR_PERFMON_PERF_QUAL_LSB+:EsrPerfmonPerfQualSize];
    esr_perfmon_p0_qual2_ld_d = esr_wr_data_i[ESR_PERFMON_PERF_QUAL2_LSB+:EsrPerfmonPerfQual2Size];
    esr_perfmon_p1_qual2_ld_d = esr_wr_data_i[ESR_PERFMON_PERF_QUAL2_LSB+:EsrPerfmonPerfQual2Size];

    esr_wr_en_perfmon = esr_wr_en_perfmon_ctl_status_i | esr_wr_en_perfmon_cyc_cntr_i |
                        esr_wr_en_perfmon_p0_cntr_i    | esr_wr_en_perfmon_p1_cntr_i  |
                        esr_wr_en_perfmon_p0_qual_i    | esr_wr_en_perfmon_p1_qual_i  |
                        esr_wr_en_perfmon_p0_qual2_i   | esr_wr_en_perfmon_p1_qual2_i;
  end

  // ESR writes program the ESR registers.
  // Once the counters are activated, they update the ESR registers.
  assign esr_perfmon_ctl_status_d = esr_wr_en_perfmon_ctl_status_i ? esr_perfmon_ctl_status_ld_d : '0;
  assign esr_perfmon_cyc_cntr_d   = esr_wr_en_perfmon_cyc_cntr_i  ? esr_perfmon_cyc_cntr_ld_d   : perfmon_cyc_cntr_d;
  assign esr_perfmon_p0_cntr_d    = esr_wr_en_perfmon_p0_cntr_i   ? esr_perfmon_p0_cntr_ld_d    : perfmon_p0_cntr_d;
  assign esr_perfmon_p1_cntr_d    = esr_wr_en_perfmon_p1_cntr_i   ? esr_perfmon_p1_cntr_ld_d    : perfmon_p1_cntr_d;
  assign esr_perfmon_p0_qual_d    = esr_wr_en_perfmon_p0_qual_i   ? esr_perfmon_p0_qual_ld_d    : '0;
  assign esr_perfmon_p1_qual_d    = esr_wr_en_perfmon_p1_qual_i   ? esr_perfmon_p1_qual_ld_d    : '0;
  assign esr_perfmon_p0_qual2_d   = esr_wr_en_perfmon_p0_qual2_i  ? esr_perfmon_p0_qual2_ld_d   : '0;
  assign esr_perfmon_p1_qual2_d   = esr_wr_en_perfmon_p1_qual2_i  ? esr_perfmon_p1_qual2_ld_d   : '0;

  // ════════════════════════════════════════════════════════════
  // Control and Status register
  // ════════════════════════════════════════════════════════════

  logic cyc_cntr_overflow_q;
  logic p0_cntr_overflow_q;
  logic p1_cntr_overflow_q;
  logic perfmon_cyc_cntr_overflow;
  logic perfmon_p0_cntr_overflow;
  logic perfmon_p1_cntr_overflow;

  logic cyc_stop_overflow;
  logic p0_stop_overflow;
  logic p1_stop_overflow;

  assign cyc_stop_overflow =
    esr_perfmon_ctl_status_o[ESR_PERFMON_CYC_STOP_OVERFLOW] |
    esr_perfmon_ctl_status_o[ESR_PERFMON_ANY_STOP_OVERFLOW];
  assign p0_stop_overflow =
    esr_perfmon_ctl_status_o[ESR_PERFMON_P0_STOP_OVERFLOW] |
    esr_perfmon_ctl_status_o[ESR_PERFMON_ANY_STOP_OVERFLOW];
  assign p1_stop_overflow =
    esr_perfmon_ctl_status_o[ESR_PERFMON_P1_STOP_OVERFLOW] |
    esr_perfmon_ctl_status_o[ESR_PERFMON_ANY_STOP_OVERFLOW];

  // ────────────────────────────────────────────────────────────
  // Cycle counter
  // ────────────────────────────────────────────────────────────

  logic perfmon_cyc_cntr_start;
  logic perfmon_cyc_cntr_stop;

  assign perfmon_cyc_cntr_start =
    esr_wr_en_perfmon_ctl_status_i &
    esr_perfmon_ctl_status_ld_d[ESR_PERFMON_CYC_START_VALID];
  assign perfmon_cyc_cntr_stop =
    esr_wr_en_perfmon_ctl_status_i &
    ~esr_perfmon_ctl_status_ld_d[ESR_PERFMON_CYC_START_VALID];

  logic perfmon_cyc_cntr_start_reset;
  assign perfmon_cyc_cntr_start_reset =
    cyc_cntr_state_q[PERFMON_START] &
    esr_perfmon_ctl_status_o[ESR_PERFMON_CYC_START_RESET];

  logic perfmon_cyc_cntr_incr_valid;
  assign perfmon_cyc_cntr_incr_valid =
    cyc_cntr_state_q[PERFMON_ACTIVE] &
    (~cyc_stop_overflow |
     (~cyc_cntr_overflow_q &
      ~esr_perfmon_cyc_cntr_o[EsrPerfmonCovfSize-1]));

  assign perfmon_cyc_cntr_overflow =
    cyc_cntr_state_q[PERFMON_ACTIVE] &
    ~cyc_cntr_overflow_q &
    esr_perfmon_cyc_cntr_o[EsrPerfmonCovfSize-1];

  logic perfmon_cyc_cntr_saturate;
  assign perfmon_cyc_cntr_saturate =
    perfmon_cyc_cntr_overflow &
    cyc_stop_overflow;

  logic perfmon_cyc_cntr_en;
  assign perfmon_cyc_cntr_en =
    perfmon_cyc_cntr_start_reset |
    perfmon_cyc_cntr_saturate    |
    perfmon_cyc_cntr_incr_valid;

  assign perfmon_cyc_cntr_d =
    perfmon_cyc_cntr_start_reset ? '0 :
    perfmon_cyc_cntr_saturate    ? {1'b0, {EsrPerfmonCntrSize{1'b1}}} :
    perfmon_cyc_cntr_incr_valid  ? (esr_perfmon_cyc_cntr_o + 1'b1) :
                                   esr_perfmon_cyc_cntr_o;

  // ────────────────────────────────────────────────────────────
  // P0 counter
  // ────────────────────────────────────────────────────────────

  logic perfmon_p0_cntr_start;
  logic perfmon_p0_cntr_stop;

  assign perfmon_p0_cntr_start =
    esr_wr_en_perfmon_ctl_status_i &
    esr_perfmon_ctl_status_ld_d[ESR_PERFMON_P0_START_VALID];
  assign perfmon_p0_cntr_stop =
    esr_wr_en_perfmon_ctl_status_i &
    ~esr_perfmon_ctl_status_ld_d[ESR_PERFMON_P0_START_VALID];

  // Note: original uses _ld_next (write data value) for P0/P1 start_reset,
  // not the current register value as cyc does.
  assign perfmon_p0_cntr_start_reset_o =
    p0_cntr_state_q[PERFMON_START] &
    esr_perfmon_ctl_status_ld_d[ESR_PERFMON_P0_START_RESET];

  logic perfmon_p0_cntr_update;
  assign perfmon_p0_cntr_update =
    p0_cntr_state_q[PERFMON_ACTIVE] &
    perfmon_update_cntr_i[0] &
    (~p0_stop_overflow |
     (~p0_cntr_overflow_q &
      ~esr_perfmon_p0_cntr_o[EsrPerfmonCovfSize-1]));

  assign perfmon_p0_cntr_overflow =
    p0_cntr_state_q[PERFMON_ACTIVE] &
    ~p0_cntr_overflow_q &
    esr_perfmon_p0_cntr_o[EsrPerfmonCovfSize-1];

  logic perfmon_p0_cntr_saturate;
  assign perfmon_p0_cntr_saturate =
    perfmon_p0_cntr_overflow &
    p0_stop_overflow;

  logic perfmon_p0_cntr_en;
  assign perfmon_p0_cntr_en =
    perfmon_p0_cntr_start_reset_o |
    perfmon_p0_cntr_saturate      |
    perfmon_p0_cntr_update;

  assign perfmon_p0_cntr_d =
    perfmon_p0_cntr_start_reset_o ? '0 :
    perfmon_p0_cntr_saturate      ? {1'b0, {EsrPerfmonCntrSize{1'b1}}} :
    perfmon_p0_cntr_update        ? esr_perfmon_update_cntr_i[0] :
                                    esr_perfmon_p0_cntr_o;

  // ────────────────────────────────────────────────────────────
  // P1 counter
  // ────────────────────────────────────────────────────────────

  logic perfmon_p1_cntr_start;
  logic perfmon_p1_cntr_stop;

  assign perfmon_p1_cntr_start =
    esr_wr_en_perfmon_ctl_status_i &
    esr_perfmon_ctl_status_ld_d[ESR_PERFMON_P1_START_VALID];
  assign perfmon_p1_cntr_stop =
    esr_wr_en_perfmon_ctl_status_i &
    ~esr_perfmon_ctl_status_ld_d[ESR_PERFMON_P1_START_VALID];

  assign perfmon_p1_cntr_start_reset_o =
    p1_cntr_state_q[PERFMON_START] &
    esr_perfmon_ctl_status_ld_d[ESR_PERFMON_P1_START_RESET];

  logic perfmon_p1_cntr_update;
  assign perfmon_p1_cntr_update =
    p1_cntr_state_q[PERFMON_ACTIVE] &
    perfmon_update_cntr_i[1] &
    (~p1_stop_overflow |
     (~p1_cntr_overflow_q &
      ~esr_perfmon_p1_cntr_o[EsrPerfmonCovfSize-1]));

  assign perfmon_p1_cntr_overflow =
    p1_cntr_state_q[PERFMON_ACTIVE] &
    ~p1_cntr_overflow_q &
    esr_perfmon_p1_cntr_o[EsrPerfmonCovfSize-1];

  logic perfmon_p1_cntr_saturate;
  assign perfmon_p1_cntr_saturate =
    perfmon_p1_cntr_overflow &
    p1_stop_overflow;

  logic perfmon_p1_cntr_en;
  assign perfmon_p1_cntr_en =
    perfmon_p1_cntr_start_reset_o |
    perfmon_p1_cntr_saturate      |
    perfmon_p1_cntr_update;

  assign perfmon_p1_cntr_d =
    perfmon_p1_cntr_start_reset_o ? '0 :
    perfmon_p1_cntr_saturate      ? {1'b0, {EsrPerfmonCntrSize{1'b1}}} :
    perfmon_p1_cntr_update        ? esr_perfmon_update_cntr_i[1] :
                                    esr_perfmon_p1_cntr_o;

  // ════════════════════════════════════════════════════════════
  // ESR register flops (EN_FF — no reset, gated clock)
  // ════════════════════════════════════════════════════════════

  // ctl_status
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_ctl_status_i) begin
      esr_perfmon_ctl_status_o <= esr_perfmon_ctl_status_d;
    end
  end

  // cyc_cntr
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_cyc_cntr_i | perfmon_cyc_cntr_en) begin
      esr_perfmon_cyc_cntr_o <= esr_perfmon_cyc_cntr_d;
    end
  end

  // p0_cntr
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_p0_cntr_i | perfmon_p0_cntr_en) begin
      esr_perfmon_p0_cntr_o <= esr_perfmon_p0_cntr_d;
    end
  end

  // p1_cntr
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_p1_cntr_i | perfmon_p1_cntr_en) begin
      esr_perfmon_p1_cntr_o <= esr_perfmon_p1_cntr_d;
    end
  end

  // p0_qual
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_p0_qual_i) begin
      esr_perfmon_p0_qual_o <= esr_perfmon_p0_qual_d;
    end
  end

  // p1_qual
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_p1_qual_i) begin
      esr_perfmon_p1_qual_o <= esr_perfmon_p1_qual_d;
    end
  end

  // p0_qual2
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_p0_qual2_i) begin
      esr_perfmon_p0_qual2_o <= esr_perfmon_p0_qual2_d;
    end
  end

  // p1_qual2
  always_ff @(posedge clk_gated) begin
    if (esr_wr_en_perfmon_p1_qual2_i) begin
      esr_perfmon_p1_qual2_o <= esr_perfmon_p1_qual2_d;
    end
  end

  // ════════════════════════════════════════════════════════════
  // ESR read interface
  // ════════════════════════════════════════════════════════════

  logic cyc_cntr_state_en;
  logic p0_cntr_state_en;
  logic p1_cntr_state_en;

  always_comb begin
    esr_perfmon_ctl_status_read_o = '0;

    // Read value
    esr_perfmon_ctl_status_read_o[ESR_PERFMON_CTL_LSB+:ESR_PERFMON_CTL_SIZE] =
      esr_perfmon_ctl_status_o[ESR_PERFMON_CTL_LSB+:ESR_PERFMON_CTL_SIZE];
    esr_perfmon_ctl_status_read_o[ESR_PERFMON_CYC_STATUS_ACTIVE]   = cyc_cntr_state_q[PERFMON_ACTIVE];
    esr_perfmon_ctl_status_read_o[ESR_PERFMON_CYC_STATUS_OVERFLOW] = cyc_cntr_overflow_q;
    esr_perfmon_ctl_status_read_o[ESR_PERFMON_P0_STATUS_ACTIVE]    = p0_cntr_state_q[PERFMON_ACTIVE];
    esr_perfmon_ctl_status_read_o[ESR_PERFMON_P0_STATUS_OVERFLOW]  = p0_cntr_overflow_q;
    esr_perfmon_ctl_status_read_o[ESR_PERFMON_P1_STATUS_ACTIVE]    = p1_cntr_state_q[PERFMON_ACTIVE];
    esr_perfmon_ctl_status_read_o[ESR_PERFMON_P1_STATUS_OVERFLOW]  = p1_cntr_overflow_q;

    perfmon_active = ~cyc_cntr_state_q[PERFMON_IDLE] | cyc_cntr_state_en |
                     ~p0_cntr_state_q[PERFMON_IDLE]  | p0_cntr_state_en  |
                     ~p1_cntr_state_q[PERFMON_IDLE]  | p1_cntr_state_en;

    perfmon_cgater_o = ~p0_cntr_state_q[PERFMON_IDLE] |
                       ~p1_cntr_state_q[PERFMON_IDLE];
  end

  // ════════════════════════════════════════════════════════════
  // Performance monitor qual logic
  // ════════════════════════════════════════════════════════════

  logic perfmon_p0_stop_cntr;
  logic perfmon_p1_stop_cntr;

  for (genvar gen_it = 0; gen_it < EsrPerfmonNumPerfCntr; gen_it++) begin : gen_qual
    always_comb begin
      perfmon_enable_o[gen_it] = (gen_it == 0)
        ? (p0_cntr_state_q[PERFMON_ACTIVE] & ~perfmon_p0_stop_cntr)
        : (p1_cntr_state_q[PERFMON_ACTIVE] & ~perfmon_p1_stop_cntr);
    end
  end

  // ════════════════════════════════════════════════════════════
  // Overflow interrupt outputs
  // ════════════════════════════════════════════════════════════

  assign cyc_cntr_overflow_intr_o = cyc_cntr_overflow_q
                                  & cyc_cntr_state_q[PERFMON_ACTIVE]
                                  & esr_perfmon_ctl_status_o[ESR_PERFMON_CYC_STOP_INTR_OVERFLOW];

  assign p0_cntr_overflow_intr_o  = p0_cntr_overflow_q
                                  & p0_cntr_state_q[PERFMON_ACTIVE]
                                  & esr_perfmon_ctl_status_o[ESR_PERFMON_P0_STOP_INTR_OVERFLOW];

  assign p1_cntr_overflow_intr_o  = p1_cntr_overflow_q
                                  & p1_cntr_state_q[PERFMON_ACTIVE]
                                  & esr_perfmon_ctl_status_o[ESR_PERFMON_P1_STOP_INTR_OVERFLOW];

  // ════════════════════════════════════════════════════════════
  // Start/stop request latching (RST_EN_FF — async reset, gated clock)
  // ════════════════════════════════════════════════════════════

  logic cyc_cntr_start_q;
  logic cyc_cntr_stop_q;
  logic p0_cntr_start_q;
  logic p0_cntr_stop_q;
  logic p1_cntr_start_q;
  logic p1_cntr_stop_q;

  // cyc_cntr_start
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                                       cyc_cntr_start_q <= '0;
    else if (cyc_cntr_start_q | perfmon_cyc_cntr_start) cyc_cntr_start_q <= perfmon_cyc_cntr_start;
  end

  // cyc_cntr_stop
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                                     cyc_cntr_stop_q <= '0;
    else if (cyc_cntr_stop_q | perfmon_cyc_cntr_stop) cyc_cntr_stop_q <= perfmon_cyc_cntr_stop;
  end

  // p0_cntr_start
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                                     p0_cntr_start_q <= '0;
    else if (p0_cntr_start_q | perfmon_p0_cntr_start) p0_cntr_start_q <= perfmon_p0_cntr_start;
  end

  // p0_cntr_stop
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                                   p0_cntr_stop_q <= '0;
    else if (p0_cntr_stop_q | perfmon_p0_cntr_stop) p0_cntr_stop_q <= perfmon_p0_cntr_stop;
  end

  // p1_cntr_start
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                                     p1_cntr_start_q <= '0;
    else if (p1_cntr_start_q | perfmon_p1_cntr_start) p1_cntr_start_q <= perfmon_p1_cntr_start;
  end

  // p1_cntr_stop
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                                   p1_cntr_stop_q <= '0;
    else if (p1_cntr_stop_q | perfmon_p1_cntr_stop) p1_cntr_stop_q <= perfmon_p1_cntr_stop;
  end

  // ════════════════════════════════════════════════════════════
  // Overflow tracking
  // ════════════════════════════════════════════════════════════

  logic clear_cntr_overflow;
  assign clear_cntr_overflow =
    cyc_cntr_state_q[PERFMON_START] |
    p0_cntr_state_q[PERFMON_START]  |
    p1_cntr_state_q[PERFMON_START];

  logic cyc_cntr_overflow_en;
  logic cyc_cntr_overflow_d;
  assign cyc_cntr_overflow_en = perfmon_cyc_cntr_overflow | clear_cntr_overflow;
  assign cyc_cntr_overflow_d  = perfmon_cyc_cntr_overflow & ~clear_cntr_overflow;

  logic p0_cntr_overflow_en;
  logic p0_cntr_overflow_d;
  assign p0_cntr_overflow_en = perfmon_p0_cntr_overflow | clear_cntr_overflow;
  assign p0_cntr_overflow_d  = perfmon_p0_cntr_overflow & ~clear_cntr_overflow;

  logic p1_cntr_overflow_en;
  logic p1_cntr_overflow_d;
  assign p1_cntr_overflow_en = perfmon_p1_cntr_overflow | clear_cntr_overflow;
  assign p1_cntr_overflow_d  = perfmon_p1_cntr_overflow & ~clear_cntr_overflow;

  // cyc_cntr_overflow
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                    cyc_cntr_overflow_q <= '0;
    else if (cyc_cntr_overflow_en)  cyc_cntr_overflow_q <= cyc_cntr_overflow_d;
  end

  // p0_cntr_overflow
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                   p0_cntr_overflow_q <= '0;
    else if (p0_cntr_overflow_en)  p0_cntr_overflow_q <= p0_cntr_overflow_d;
  end

  // p1_cntr_overflow
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                   p1_cntr_overflow_q <= '0;
    else if (p1_cntr_overflow_en)  p1_cntr_overflow_q <= p1_cntr_overflow_d;
  end

  // ════════════════════════════════════════════════════════════
  // Stop-on-overflow logic
  // ════════════════════════════════════════════════════════════

  logic perfmon_any_stop_cntr;
  assign perfmon_any_stop_cntr =
    esr_perfmon_ctl_status_o[ESR_PERFMON_ANY_STOP_OVERFLOW] &
    ( (cyc_cntr_overflow_q & cyc_cntr_state_q[PERFMON_ACTIVE]) |
      (p0_cntr_overflow_q  & p0_cntr_state_q[PERFMON_ACTIVE])  |
      (p1_cntr_overflow_q  & p1_cntr_state_q[PERFMON_ACTIVE]) );

  logic perfmon_cyc_stop_cntr;
  assign perfmon_cyc_stop_cntr =
    perfmon_any_stop_cntr |
    (cyc_cntr_overflow_q & cyc_cntr_state_q[PERFMON_ACTIVE] &
     esr_perfmon_ctl_status_o[ESR_PERFMON_CYC_STOP_OVERFLOW]);

  assign perfmon_p0_stop_cntr =
    perfmon_any_stop_cntr |
    (p0_cntr_overflow_q & p0_cntr_state_q[PERFMON_ACTIVE] &
     esr_perfmon_ctl_status_o[ESR_PERFMON_P0_STOP_OVERFLOW]);

  assign perfmon_p1_stop_cntr =
    perfmon_any_stop_cntr |
    (p1_cntr_overflow_q & p1_cntr_state_q[PERFMON_ACTIVE] &
     esr_perfmon_ctl_status_o[ESR_PERFMON_P1_STOP_OVERFLOW]);

  // ════════════════════════════════════════════════════════════
  // State machines
  // ════════════════════════════════════════════════════════════

  always_comb begin
    cyc_cntr_state_d = '0;
    p0_cntr_state_d  = '0;
    p1_cntr_state_d  = '0;

    cyc_cntr_state_d = perfmon_state_next(cyc_cntr_state_q, cyc_cntr_start_q,
                                          cyc_cntr_stop_q | perfmon_cyc_stop_cntr);
    cyc_cntr_state_en = |(cyc_cntr_state_q ^ cyc_cntr_state_d);

    p0_cntr_state_d = perfmon_state_next(p0_cntr_state_q, p0_cntr_start_q,
                                         p0_cntr_stop_q | perfmon_p0_stop_cntr);
    p0_cntr_state_en = |(p0_cntr_state_q ^ p0_cntr_state_d);

    p1_cntr_state_d = perfmon_state_next(p1_cntr_state_q, p1_cntr_start_q,
                                         p1_cntr_stop_q | perfmon_p1_stop_cntr);
    p1_cntr_state_en = |(p1_cntr_state_q ^ p1_cntr_state_d);
  end

  localparam [PERFMON_STATES-1:0] PERFMON_IDLE_STATE = {{PERFMON_STATES-1{1'b0}}, 1'b1};

  // cyc_cntr_state
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                  cyc_cntr_state_q <= PERFMON_IDLE_STATE;
    else if (cyc_cntr_state_en)   cyc_cntr_state_q <= cyc_cntr_state_d;
  end

  // p0_cntr_state
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                 p0_cntr_state_q <= PERFMON_IDLE_STATE;
    else if (p0_cntr_state_en)   p0_cntr_state_q <= p0_cntr_state_d;
  end

  // p1_cntr_state
  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni)                 p1_cntr_state_q <= PERFMON_IDLE_STATE;
    else if (p1_cntr_state_en)   p1_cntr_state_q <= p1_cntr_state_d;
  end

  // ════════════════════════════════════════════════════════════
  // Performance counter state machine function
  // ════════════════════════════════════════════════════════════

  function automatic logic [PERFMON_STATES-1:0] perfmon_state_next;
    input [PERFMON_STATES-1:0] perfmon_state;
    input logic perfmon_start;
    input logic perfmon_stop;
    begin
      perfmon_state_next = '0;
      case (1'b1)
        perfmon_state[PERFMON_IDLE]: begin
          if (perfmon_start) begin
            perfmon_state_next[PERFMON_START] = 1'b1;
          end else begin
            perfmon_state_next[PERFMON_IDLE] = 1'b1;
          end
        end
        perfmon_state[PERFMON_START]: begin
          perfmon_state_next[PERFMON_DLY1] = 1'b1;
        end
        perfmon_state[PERFMON_DLY1]: begin
          perfmon_state_next[PERFMON_DLY2] = 1'b1;
        end
        perfmon_state[PERFMON_DLY2]: begin
          perfmon_state_next[PERFMON_DLY3] = 1'b1;
        end
        perfmon_state[PERFMON_DLY3]: begin
          perfmon_state_next[PERFMON_ACTIVE] = 1'b1;
        end
        perfmon_state[PERFMON_ACTIVE]: begin
          if (perfmon_stop) begin
            perfmon_state_next[PERFMON_IDLE] = 1'b1;
          end else begin
            perfmon_state_next[PERFMON_ACTIVE] = 1'b1;
          end
        end
        default: begin
          perfmon_state_next[PERFMON_IDLE] = 1'b1;
        end
      endcase
    end
  endfunction

/* verilator lint_on UNUSEDSIGNAL */
endmodule
