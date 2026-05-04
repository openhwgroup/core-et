// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache bank performance monitor top.
//
// Wraps perfmon_ctl (counter control, ESR interface) and two instances
// of bank_perfmon_qual (one for P0, one for P1). Extracts ESR control
// fields, routes qualifier/event/mode to each qual instance, packs
// output structs (perfmon_esr_status, perfmon_cntr_overflow), and
// distributes perfmon_cgater to both req_info outputs.
//
// Replaces: shire_cache_bank_perfmon (etcore-soc)

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_bank_perfmon
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       clk_free_i,
  input  logic                       rst_ni,
  input  dft_t                       dft_i,

  // Performance monitor for resource and event
  output perfmon_req_info_t          perfmon_p0_req_info_o,
  input  perfmon_rsp_info_t          perfmon_p0_rsp_info_i,
  output perfmon_req_info_t          perfmon_p1_req_info_o,
  input  perfmon_rsp_info_t          perfmon_p1_rsp_info_i,

  // Performance monitor saturation errors
  output perfmon_cntr_overflow_t     perfmon_cntr_overflow_o,

  // ESRs
  input  bank_esr_ctl_t              esr_ctl_i,
  output perfmon_esr_status_t        perfmon_esr_status_o
);
  // P0 and P1
  // Maximum incr count per cyc is 64
  localparam int unsigned EsrPerfmonNumPerfCntr  = 2;
  localparam int unsigned EsrPerfmonStatusLsb    = 32;
  localparam int unsigned EsrPerfmonPerfQualLsb  = 0;
  localparam int unsigned EsrPerfmonCycCntrLsb   = 0;
  localparam int unsigned EsrPerfmonPerfCntrLsb  = 0;

  // Cyc and Perf counters have an overflow bit
  // Counter size is 40 but set to 41 for overflow bit
  // Qual size set to max qual option
  localparam int unsigned EsrPerfmonStatusSize   = 6;
  localparam int unsigned EsrPerfmonCsrSize      = EsrPerfmonStatusLsb + EsrPerfmonStatusSize;

  localparam int unsigned EsrPerfmonCntrSize     = 40;
  localparam int unsigned EsrPerfmonCovfSize     = EsrPerfmonCntrSize + 1;
  localparam int unsigned EsrPerfmonModeSize     = 8;
  // Max qual size for event or resource
  localparam int unsigned EsrPerfmonPerfQualSize = 51;
  localparam int unsigned EsrPerfmonRegSize      = DwDataSize;

  // Control and Status
  localparam int unsigned EsrPerfmonP0Event      = 8;
  localparam int unsigned EsrPerfmonP0Mode       = 9;
  localparam int unsigned EsrPerfmonP1Event      = 21;
  localparam int unsigned EsrPerfmonP1Mode       = 22;

  logic                                                                  perfmon_cgater;
  logic [EsrPerfmonNumPerfCntr-1:0]                                      perfmon_enable;
  logic [EsrPerfmonNumPerfCntr-1:0][EsrPerfmonPerfQualSize-1:0]          perfmon_qual;
  logic [EsrPerfmonNumPerfCntr-1:0][EsrPerfmonModeSize-1:0]              perfmon_mode;
  logic [EsrPerfmonNumPerfCntr-1:0][EsrPerfmonCovfSize-1:0]              esr_perfmon_cntr;
  logic [EsrPerfmonNumPerfCntr-1:0]                                      perfmon_event;
  logic [EsrPerfmonPerfQualSize-1:0]                                     esr_perfmon_p0_qual;
  logic [EsrPerfmonPerfQualSize-1:0]                                     esr_perfmon_p1_qual;
  logic [EsrPerfmonCsrSize-1:0]                                          esr_perfmon_ctl_status;
  logic [EsrPerfmonCovfSize-1:0]                                         esr_perfmon_cyc_cntr;
  logic [EsrPerfmonCovfSize-1:0]                                         esr_perfmon_p0_cntr;
  logic [EsrPerfmonCovfSize-1:0]                                         esr_perfmon_p1_cntr;
  logic [EsrPerfmonNumPerfCntr-1:0]                                      perfmon_update_cntr;
  logic [EsrPerfmonNumPerfCntr-1:0][EsrPerfmonCovfSize-1:0]              esr_perfmon_update_cntr;

  // ESR Control
  logic [EsrPerfmonRegSize-1:0]                                          esr_wr_data;
  logic                                                                  esr_wr_en_perfmon_ctl_status;
  logic                                                                  esr_wr_en_perfmon_cyc_cntr;
  logic                                                                  esr_wr_en_perfmon_p0_cntr;
  logic                                                                  esr_wr_en_perfmon_p1_cntr;
  logic                                                                  esr_wr_en_perfmon_p0_qual;
  logic                                                                  esr_wr_en_perfmon_p1_qual;

  perfmon_rsp_info_t [EsrPerfmonNumPerfCntr-1:0]                         perfmon_rsp_info;
  perfmon_req_info_t [EsrPerfmonNumPerfCntr-1:0]                         perfmon_req_info;

  logic                                                                  cyc_cntr_overflow_intr;
  logic                                                                  p0_cntr_overflow_intr;
  logic                                                                  p1_cntr_overflow_intr;
  logic [EsrPerfmonRegSize-1:0]                                          esr_perfmon_ctl_status_read;

  logic                                                                  perfmon_p0_cntr_start_reset;
  logic                                                                  perfmon_p1_cntr_start_reset;
  logic                                                                  unused_esr_perfmon_p0_qual2;
  logic                                                                  unused_esr_perfmon_p1_qual2;

  logic                                                                  clk_gated;

  //
  // convert perf counter signals to/from the shire cache specific signals
  //
  always_comb begin
    perfmon_cntr_overflow_o        = '0;
    perfmon_esr_status_o.cyc_cntr  = '0;
    perfmon_esr_status_o.p0_cntr   = '0;
    perfmon_esr_status_o.p1_cntr   = '0;
    perfmon_esr_status_o.p0_qual   = '0;
    perfmon_esr_status_o.p1_qual   = '0;
    perfmon_esr_status_o.ctl_status = esr_perfmon_ctl_status_read;

    perfmon_cntr_overflow_o.cyc_cntr_overflow = cyc_cntr_overflow_intr;
    perfmon_cntr_overflow_o.p0_cntr_overflow  = p0_cntr_overflow_intr;
    perfmon_cntr_overflow_o.p1_cntr_overflow  = p1_cntr_overflow_intr;

    perfmon_esr_status_o.cyc_cntr[EsrPerfmonCycCntrLsb+:EsrPerfmonCntrSize]       = esr_perfmon_cyc_cntr[EsrPerfmonCycCntrLsb+:EsrPerfmonCntrSize];
    perfmon_esr_status_o.p0_cntr[EsrPerfmonPerfCntrLsb+:EsrPerfmonCntrSize]       = esr_perfmon_p0_cntr[EsrPerfmonPerfCntrLsb+:EsrPerfmonCntrSize];
    perfmon_esr_status_o.p1_cntr[EsrPerfmonPerfCntrLsb+:EsrPerfmonCntrSize]       = esr_perfmon_p1_cntr[EsrPerfmonPerfCntrLsb+:EsrPerfmonCntrSize];
    perfmon_esr_status_o.p0_qual[EsrPerfmonPerfQualLsb+:EsrPerfmonPerfQualSize]   = esr_perfmon_p0_qual[EsrPerfmonPerfQualLsb+:EsrPerfmonPerfQualSize];
    perfmon_esr_status_o.p1_qual[EsrPerfmonPerfQualLsb+:EsrPerfmonPerfQualSize]   = esr_perfmon_p1_qual[EsrPerfmonPerfQualLsb+:EsrPerfmonPerfQualSize];

    esr_wr_data                  = esr_ctl_i.wr_data;
    esr_wr_en_perfmon_ctl_status = esr_ctl_i.wr_en_perfmon_ctl_status;
    esr_wr_en_perfmon_cyc_cntr   = esr_ctl_i.wr_en_perfmon_cyc_cntr;
    esr_wr_en_perfmon_p0_cntr    = esr_ctl_i.wr_en_perfmon_p0_cntr;
    esr_wr_en_perfmon_p1_cntr    = esr_ctl_i.wr_en_perfmon_p1_cntr;
    esr_wr_en_perfmon_p0_qual    = esr_ctl_i.wr_en_perfmon_p0_qual;
    esr_wr_en_perfmon_p1_qual    = esr_ctl_i.wr_en_perfmon_p1_qual;
  end

  //
  // Performance monitor control logic/registers
  //
  shirecache_perfmon_ctl #(
    .EsrPerfmonRegSize (EsrPerfmonRegSize)
  ) u_perfmon_ctl (
    .clk_free_i                      (clk_free_i),
    .clk_gated_o                     (clk_gated),
    .rst_ni                          (rst_ni),
    .dft_i                           (dft_i),
    .cyc_cntr_overflow_intr_o        (cyc_cntr_overflow_intr),
    .p0_cntr_overflow_intr_o         (p0_cntr_overflow_intr),
    .p1_cntr_overflow_intr_o         (p1_cntr_overflow_intr),

    .esr_wr_data_i                   (esr_wr_data),
    .esr_wr_en_perfmon_ctl_status_i  (esr_wr_en_perfmon_ctl_status),
    .esr_wr_en_perfmon_cyc_cntr_i    (esr_wr_en_perfmon_cyc_cntr),
    .esr_wr_en_perfmon_p0_cntr_i     (esr_wr_en_perfmon_p0_cntr),
    .esr_wr_en_perfmon_p1_cntr_i     (esr_wr_en_perfmon_p1_cntr),
    .esr_wr_en_perfmon_p0_qual_i     (esr_wr_en_perfmon_p0_qual),
    .esr_wr_en_perfmon_p1_qual_i     (esr_wr_en_perfmon_p1_qual),
    .esr_wr_en_perfmon_p0_qual2_i    (1'b0), // qual2 is unused
    .esr_wr_en_perfmon_p1_qual2_i    (1'b0), // qual2 is unused

    .esr_perfmon_ctl_status_read_o   (esr_perfmon_ctl_status_read),
    .perfmon_cgater_o                (perfmon_cgater),
    .perfmon_enable_o                (perfmon_enable),
    .esr_perfmon_p0_qual_o           (esr_perfmon_p0_qual),
    .esr_perfmon_p1_qual_o           (esr_perfmon_p1_qual),
    .esr_perfmon_p0_qual2_o          (unused_esr_perfmon_p0_qual2),
    .esr_perfmon_p1_qual2_o          (unused_esr_perfmon_p1_qual2),
    .esr_perfmon_ctl_status_o        (esr_perfmon_ctl_status),
    .esr_perfmon_cyc_cntr_o          (esr_perfmon_cyc_cntr),
    .esr_perfmon_p0_cntr_o           (esr_perfmon_p0_cntr),
    .esr_perfmon_p1_cntr_o           (esr_perfmon_p1_cntr),
    .perfmon_update_cntr_i           (perfmon_update_cntr),
    .esr_perfmon_update_cntr_i       (esr_perfmon_update_cntr),
    .perfmon_p0_cntr_start_reset_o   (perfmon_p0_cntr_start_reset),
    .perfmon_p1_cntr_start_reset_o   (perfmon_p1_cntr_start_reset)
  );

  //
  // Performance monitor qualifier logic
  //
  for (genvar gen_it = 0; gen_it < int'(EsrPerfmonNumPerfCntr); gen_it++) begin : gen_qual

    always_comb begin
      perfmon_qual[gen_it]  = (gen_it == 0)
        ? esr_perfmon_p0_qual
        : esr_perfmon_p1_qual;
      perfmon_event[gen_it] = (gen_it == 0)
        ? esr_perfmon_ctl_status[EsrPerfmonP0Event]
        : esr_perfmon_ctl_status[EsrPerfmonP1Event];
      perfmon_mode[gen_it] = (gen_it == 0)
        ? esr_perfmon_ctl_status[EsrPerfmonP0Mode+:EsrPerfmonModeSize]
        : esr_perfmon_ctl_status[EsrPerfmonP1Mode+:EsrPerfmonModeSize];
      esr_perfmon_cntr[gen_it] = (gen_it == 0)
        ? esr_perfmon_p0_cntr
        : esr_perfmon_p1_cntr;
    end

    shirecache_bank_perfmon_qual #(
      .IsP0            (gen_it == 0),
      .PerfmonCovfSize (EsrPerfmonCovfSize),
      .PerfmonModeSize (EsrPerfmonModeSize),
      .PerfmonQualLsb  (EsrPerfmonPerfQualLsb),
      .PerfmonQualSize (EsrPerfmonPerfQualSize)
    ) u_qual (
      // Performance monitor for resource and event
      .perfmon_req_info_o           (perfmon_req_info[gen_it]),
      .perfmon_rsp_info_i           (perfmon_rsp_info[gen_it]),

      // Esr counter
      .perfmon_update_cntr_o        (perfmon_update_cntr[gen_it]),
      .esr_sc_perfmon_update_cntr_o (esr_perfmon_update_cntr[gen_it]),
      .esr_sc_perfmon_cntr_i        (esr_perfmon_cntr[gen_it]),

      // ESRs
      .perfmon_enable_i             (perfmon_enable[gen_it]),
      .perfmon_event_i              (perfmon_event[gen_it]),
      .perfmon_mode_i               (perfmon_mode[gen_it]),
      .perfmon_qual_i               (perfmon_qual[gen_it])
    );
  end

  always_comb begin
    perfmon_rsp_info[0] = '0;
    perfmon_rsp_info[1] = '0;

    perfmon_rsp_info[0].ev_rsp   = perfmon_p0_rsp_info_i.ev_rsp | perfmon_p1_rsp_info_i.ev_rsp;
    perfmon_rsp_info[0].rsrc_rsp = perfmon_p0_rsp_info_i.rsrc_rsp;
    perfmon_rsp_info[1].ev_rsp   = perfmon_p0_rsp_info_i.ev_rsp | perfmon_p1_rsp_info_i.ev_rsp;
    perfmon_rsp_info[1].rsrc_rsp = perfmon_p1_rsp_info_i.rsrc_rsp;
  end

  always_comb begin
    perfmon_p0_req_info_o = '0;
    perfmon_p1_req_info_o = '0;

    perfmon_p0_req_info_o.cgater    = perfmon_cgater;
    perfmon_p0_req_info_o.ev_mode   = perfmon_req_info[0].ev_mode | perfmon_req_info[1].ev_mode;
    perfmon_p0_req_info_o.rsrc_mode = perfmon_req_info[0].rsrc_mode;
    perfmon_p1_req_info_o.cgater    = perfmon_cgater;
    perfmon_p1_req_info_o.ev_mode   = perfmon_req_info[0].ev_mode | perfmon_req_info[1].ev_mode;
    perfmon_p1_req_info_o.rsrc_mode = perfmon_req_info[1].rsrc_mode;
  end

  // LINT
  logic unused_ok;
  assign unused_ok = &{1'b0,
    perfmon_req_info,
    perfmon_p0_rsp_info_i,
    perfmon_p1_rsp_info_i,
    perfmon_p0_cntr_start_reset,
    perfmon_p1_cntr_start_reset,
    esr_perfmon_ctl_status,
    esr_perfmon_cyc_cntr,
    esr_wr_data,
    unused_esr_perfmon_p0_qual2,
    unused_esr_perfmon_p1_qual2,
    clk_gated,
    clk_i,
    esr_ctl_i,
    unused_ok
  };

/* verilator lint_on UNUSEDSIGNAL */
endmodule
