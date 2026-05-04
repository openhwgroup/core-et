// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache bank error logger.
//
// Monitors 12 error sources, logs errors into ESR registers with
// priority arbitration (enabled errors over masked/disabled errors),
// signals error detection for global interrupts.
//
// Replaces: shire_cache_bank_err_logger (etcore-soc)

module shirecache_bank_err_logger
  import shirecache_pkg::*;
#(
  parameter int unsigned NumPorts     = Ports,
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                                clk_i,
  input  logic                                rst_ni,

  // Output to IO-Shire
  output logic                                err_detected_o,
  output logic                                err_logged_o,

  // Output to reqq
  output logic [NumEntries-1:0]               set_reqq_err_detected_hot_o,
  output logic [NumEntries-1:0]               set_reqq_err_zero_data_hot_o,

  // Traced reqq state (from reuse of reqq mux)
  output reqq_entry_state_t                   traced_reqq_state_o,

  // Allocation decode error
  input  logic                                alloc_l2_valid_ad_i,
  input  reqq_alloc_t                         alloc_l2_info_ad_i,
  input  logic                                alloc_l3_valid_ad_i,
  input  reqq_alloc_t                         alloc_l3_info_ad_i,
  // Tag error
  input  logic                                pipe_tag_rsp_valid_i,
  input  tag_rsp_t                            pipe_tag_rsp_info_i,
  // Data error
  input  logic                                pipe_data_rsp_valid_i,
  input  data_rsp_t                           pipe_data_rsp_info_i,
  // Mesh response errors
  input  logic                                to_l3_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t                      to_l3_mesh_rsp_info_i,
  input  logic                                to_sys_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t                      to_sys_mesh_rsp_info_i,
  // ECC saturation errors
  input  sbe_dbe_overflow_t                   pipe_sbe_dbe_overflow_i,
  // Performance monitor saturation errors
  input  perfmon_cntr_overflow_t              perfmon_cntr_overflow_i,

  // reqq state
  input  reqq_entry_state_t [NumEntries-1:0]  reqq_state_i,

  // I/O valid/ready for reqq debug read out
  input  logic                                bank_req_valid_i,
  input  logic                                bank_req_ready_i,
  input  logic [NumPorts-1:0]                 bank_rsp_valid_i,
  input  logic [NumPorts-1:0]                 bank_rsp_ready_i,
  input  logic                                to_l3_mesh_master_bank_req_valid_i,
  input  logic                                to_l3_mesh_master_bank_req_ready_i,
  input  logic                                to_l3_mesh_master_bank_rsp_valid_i,
  input  logic                                to_l3_mesh_master_bank_rsp_ready_i,
  input  logic                                to_sys_mesh_master_bank_req_valid_i,
  input  logic                                to_sys_mesh_master_bank_req_ready_i,
  input  logic                                to_sys_mesh_master_bank_rsp_valid_i,
  input  logic                                to_sys_mesh_master_bank_rsp_ready_i,
  input  logic                                l3_mesh_slave_bank_req_valid_i,
  input  logic                                l3_mesh_slave_bank_req_ready_i,
  input  logic                                l3_mesh_slave_bank_rsp_valid_i,
  input  logic                                l3_mesh_slave_bank_rsp_ready_i,

  // ESRs
  output err_log_esr_status_t                 err_log_esr_status_o,
  input  bank_esr_ctl_t                       esr_ctl_i
);

  /* verilator lint_off UNUSEDSIGNAL */
  /* verilator lint_off UNOPTFLAT */
  /* verilator lint_off WIDTHTRUNC */
  /* verilator lint_off WIDTHEXPAND */
  /* verilator lint_off MULTIDRIVEN */  // err_info[] has separate always_comb per error source, each writing distinct indices

  // ══════════════════════════════════════════════════════════════
  // Module-internal error log types (match original shire_cache_types.vh)
  // ══════════════════════════════════════════════════════════════

  // ECC error format (64-bit info register)
  typedef struct packed {
    logic [11:0]                     reserved_63_52;
    err_log_ram_e                    ram;
    logic [7:0]                      bits;
    logic [31:0]                     index;
    err_log_code_e                   code;
    logic                            imprecise;
    logic                            enabled;
    logic                            multi;
    logic                            valid;
  } err_log_info_ecc_t;

  // ECC saturation format (64-bit info register)
  typedef struct packed {
    logic [10:0]                     reserved_63_53;
    logic                            dbe;
    err_log_ram_e                    ram;
    logic [39:0]                     reserved_47_8;
    err_log_code_e                   code;
    logic                            imprecise;
    logic                            enabled;
    logic                            multi;
    logic                            valid;
  } err_log_info_ecc_sat_t;

  // Decode error format (64-bit info register)
  typedef struct packed {
    logic [3:0]                      reserved_63_60;
    logic [3:0]                      port;
    logic [11:0]                     et_link_source;
    logic [11:0]                     et_link_tag_id;
    logic [7:0]                      reqq_id;
    reqq_source_e                    src;
    logic                            opcode_reserved;
    reqq_orig_opcode_e               opcode;
    logic [8-$bits(err_rsp_e)-1:0]   err_type_reserved;
    err_rsp_e                        err_type;
    err_log_code_e                   code;
    logic                            imprecise;
    logic                            enabled;
    logic                            multi;
    logic                            valid;
  } err_log_info_decode_t;

  // Perfmon saturation format (64-bit info register)
  typedef struct packed {
    logic [52:0]                     reserved_63_11;
    logic                            p1_cntr_overflow;
    logic                            p0_cntr_overflow;
    logic                            cyc_cntr_overflow;
    err_log_code_e                   code;
    logic                            imprecise;
    logic                            enabled;
    logic                            multi;
    logic                            valid;
  } err_log_info_perfmon_sat_t;

  // Union of all info formats
  typedef union packed {
    err_log_info_ecc_t           ecc_err;
    err_log_info_ecc_sat_t       ecc_sat_err;
    err_log_info_decode_t        decode_err;
    err_log_info_perfmon_sat_t   perfmon_sat_err;
  } err_log_info_t;

  // Error log entry — one per error source
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    err_log_info_t               info;
    logic                        address_valid;
    logic [EsrStatusSize-1:0]    address;
  } err_log_err_t;

  // Static (non-union) version — used for prim_arb_prio data width
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    logic [EsrStatusSize-1:0]    info;
    logic                        address_valid;
    logic [EsrStatusSize-1:0]    address;
  } err_log_err_static_t;

  // ══════════════════════════════════════════════════════════════
  // Debug control signals
  // ══════════════════════════════════════════════════════════════
  logic reqq_debug_query_reqq_entry;
  logic reqq_debug_query_reqq_valid;

  // ══════════════════════════════════════════════════════════════
  // Map to common err_log_err_t — 12 error sources
  // ══════════════════════════════════════════════════════════════
  localparam int unsigned NumErr = 12;
  err_log_err_t [NumErr-1:0] err_info;

  // ── L2/L3 allocation decode errors (highest priority) ────────
  reqq_alloc_t [1:0] alloc_info;
  logic         [1:0] alloc_valid;
  assign alloc_valid[0] = alloc_l2_valid_ad_i;
  assign alloc_info[0]  = alloc_l2_info_ad_i;
  assign alloc_valid[1] = alloc_l3_valid_ad_i;
  assign alloc_info[1]  = alloc_l3_info_ad_i;

  for (genvar gen_it = (NumErr-2); gen_it < NumErr; gen_it++) begin : gen_alloc_err
    always_comb begin
      err_info[gen_it] = '0;
      err_info[gen_it].reqq_id                                          = alloc_info[(NumErr-1)-gen_it].reqq_id;
      err_info[gen_it].trans_id                                         = alloc_info[(NumErr-1)-gen_it].trans_id;
      err_info[gen_it].info.decode_err.valid                            = alloc_info[(NumErr-1)-gen_it].err_valid & alloc_valid[(NumErr-1)-gen_it];
      err_info[gen_it].info.decode_err.imprecise                        = ~esr_ctl_i.esr_sc_err_rsp_enable | alloc_info[(NumErr-1)-gen_it].err_imprecise;
      err_info[gen_it].info.decode_err.code                             = ErrLogDecode;
      err_info[gen_it].info.decode_err.enabled                          = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogDecode];
      err_info[gen_it].info.decode_err.err_type                         = alloc_info[(NumErr-1)-gen_it].err_type;
      err_info[gen_it].info.decode_err.reqq_id[NumEntriesId-1:0]        = alloc_info[(NumErr-1)-gen_it].reqq_id;
    end
  end

  // ── Tag decode error ─────────────────────────────────────────
  localparam int unsigned TagDecode = NumErr - 3;
  always_comb begin
    err_info[TagDecode] = '0;
    err_info[TagDecode].reqq_id                                          = pipe_tag_rsp_info_i.reqq_id;
    err_info[TagDecode].trans_id                                         = pipe_tag_rsp_info_i.trans_id;
    err_info[TagDecode].info.decode_err.imprecise                        = ~esr_ctl_i.esr_sc_err_rsp_enable | reqq_state_i[pipe_tag_rsp_info_i.reqq_id].err_imprecise;
    err_info[TagDecode].info.decode_err.code                             = ErrLogDecode;
    err_info[TagDecode].info.decode_err.enabled                          = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogDecode];
    err_info[TagDecode].info.decode_err.err_type                         = pipe_tag_rsp_info_i.err_rsp;
    err_info[TagDecode].info.decode_err.reqq_id[NumEntriesId-1:0]        = pipe_tag_rsp_info_i.reqq_id;
    err_info[TagDecode].address_valid                                    = 1'b1;
    err_info[TagDecode].address                                          = pipe_tag_rsp_info_i.orig_address;
    if (pipe_tag_rsp_valid_i &
        ~(pipe_tag_rsp_info_i.err_rsp == ErrNone)) begin
      err_info[TagDecode].info.decode_err.valid = 1'b1;
    end
  end

  // ── To_L3 mesh response error ────────────────────────────────
  localparam int unsigned ToL3Mesh = TagDecode - 1;
  always_comb begin
    err_info[ToL3Mesh] = '0;
    err_info[ToL3Mesh].reqq_id                                          = to_l3_mesh_rsp_info_i.reqq_id;
    err_info[ToL3Mesh].info.decode_err.imprecise                        = ~esr_ctl_i.esr_sc_err_rsp_enable | reqq_state_i[to_l3_mesh_rsp_info_i.reqq_id].err_imprecise;
    err_info[ToL3Mesh].info.decode_err.code                             = ErrLogDecode;
    err_info[ToL3Mesh].info.decode_err.enabled                          = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogDecode];
    err_info[ToL3Mesh].info.decode_err.err_type                         = ReqqErrMeshResp;
    err_info[ToL3Mesh].info.decode_err.reqq_id[NumEntriesId-1:0]        = to_l3_mesh_rsp_info_i.reqq_id;
    err_info[ToL3Mesh].address_valid                                    = 1'b1;
    err_info[ToL3Mesh].address                                          = reqq_state_i[to_l3_mesh_rsp_info_i.reqq_id].address;
    if (to_l3_mesh_rsp_valid_i & to_l3_mesh_rsp_info_i.err) begin
      err_info[ToL3Mesh].info.decode_err.valid = 1'b1;
    end
  end

  // ── To_SYS mesh response error ───────────────────────────────
  localparam int unsigned ToSysMesh = ToL3Mesh - 1;
  always_comb begin
    err_info[ToSysMesh] = '0;
    err_info[ToSysMesh].reqq_id                                          = to_sys_mesh_rsp_info_i.reqq_id;
    err_info[ToSysMesh].trans_id                                         = to_sys_mesh_rsp_info_i.trans_id;
    err_info[ToSysMesh].info.decode_err.imprecise                        = ~esr_ctl_i.esr_sc_err_rsp_enable | reqq_state_i[to_sys_mesh_rsp_info_i.reqq_id].err_imprecise;
    err_info[ToSysMesh].info.decode_err.code                             = ErrLogDecode;
    err_info[ToSysMesh].info.decode_err.enabled                          = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogDecode];
    err_info[ToSysMesh].info.decode_err.err_type                         = ReqqErrMeshResp;
    err_info[ToSysMesh].info.decode_err.reqq_id[NumEntriesId-1:0]        = to_sys_mesh_rsp_info_i.reqq_id;
    err_info[ToSysMesh].address_valid                                    = 1'b1;
    err_info[ToSysMesh].address                                          = reqq_state_i[to_sys_mesh_rsp_info_i.reqq_id].address;
    if (to_sys_mesh_rsp_valid_i & to_sys_mesh_rsp_info_i.err) begin
      err_info[ToSysMesh].info.decode_err.valid = 1'b1;
    end
  end

  // ── Tag ECC error ────────────────────────────────────────────
  localparam int unsigned TagEcc = ToSysMesh - 1;
  always_comb begin
    err_info[TagEcc] = '0;
    err_info[TagEcc].reqq_id                                    = pipe_tag_rsp_info_i.reqq_id;
    err_info[TagEcc].trans_id                                   = pipe_tag_rsp_info_i.trans_id;
    err_info[TagEcc].info.ecc_err.imprecise                     = ~esr_ctl_i.esr_sc_err_rsp_enable | reqq_state_i[pipe_tag_rsp_info_i.reqq_id].err_imprecise;
    err_info[TagEcc].info.ecc_err.index[SetIdSize-1:0]          = pipe_tag_rsp_info_i.tag_index;
    err_info[TagEcc].info.ecc_err.ram                           = ErrLogRamTag;
    err_info[TagEcc].address_valid                              = 1'b1;
    err_info[TagEcc].address                                    = pipe_tag_rsp_info_i.orig_address;
    if (pipe_tag_rsp_valid_i & (|pipe_tag_rsp_info_i.tag_ram_sbe)) begin
      err_info[TagEcc].info.ecc_err.valid                       = 1'b1;
      err_info[TagEcc].info.ecc_err.bits[Ways-1:0]              = pipe_tag_rsp_info_i.tag_ram_sbe;
      err_info[TagEcc].info.ecc_err.code                        = ErrLogEccSbe;
      err_info[TagEcc].info.ecc_err.enabled                     = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccSbe];
    end
    else if (pipe_tag_rsp_valid_i & (|pipe_tag_rsp_info_i.tag_ram_dbe)) begin
      err_info[TagEcc].info.ecc_err.valid                       = 1'b1;
      err_info[TagEcc].info.ecc_err.bits[Ways-1:0]              = pipe_tag_rsp_info_i.tag_ram_dbe;
      err_info[TagEcc].info.ecc_err.code                        = ErrLogEccDbe;
      err_info[TagEcc].info.ecc_err.enabled                     = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccDbe];
    end
  end

  // ── Tag state ECC error ──────────────────────────────────────
  localparam int unsigned StateEcc = TagEcc - 1;
  always_comb begin
    err_info[StateEcc] = '0;
    err_info[StateEcc].reqq_id                                    = pipe_tag_rsp_info_i.reqq_id;
    err_info[StateEcc].trans_id                                   = pipe_tag_rsp_info_i.trans_id;
    err_info[StateEcc].info.ecc_err.imprecise                     = ~esr_ctl_i.esr_sc_err_rsp_enable | reqq_state_i[pipe_tag_rsp_info_i.reqq_id].err_imprecise;
    err_info[StateEcc].info.ecc_err.index[SetIdSize-1:0]          = pipe_tag_rsp_info_i.tag_index;
    err_info[StateEcc].info.ecc_err.ram                           = ErrLogRamTagState;
    err_info[StateEcc].address_valid                              = 1'b1;
    err_info[StateEcc].address                                    = pipe_tag_rsp_info_i.orig_address;
    if (pipe_tag_rsp_valid_i & pipe_tag_rsp_info_i.tag_state_ram_sbe) begin
      err_info[StateEcc].info.ecc_err.valid                       = 1'b1;
      err_info[StateEcc].info.ecc_err.bits[0]                     = pipe_tag_rsp_info_i.tag_state_ram_sbe;
      err_info[StateEcc].info.ecc_err.code                        = ErrLogEccSbe;
      err_info[StateEcc].info.ecc_err.enabled                     = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccSbe];
    end
    else if (pipe_tag_rsp_valid_i & pipe_tag_rsp_info_i.tag_state_ram_dbe) begin
      err_info[StateEcc].info.ecc_err.valid                       = 1'b1;
      err_info[StateEcc].info.ecc_err.bits[0]                     = pipe_tag_rsp_info_i.tag_state_ram_dbe;
      err_info[StateEcc].info.ecc_err.code                        = ErrLogEccDbe;
      err_info[StateEcc].info.ecc_err.enabled                     = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccDbe];
    end
  end

  // ── Data ECC error ───────────────────────────────────────────
  localparam int unsigned DataEcc = StateEcc - 1;
  always_comb begin
    err_info[DataEcc] = '0;
    err_info[DataEcc].reqq_id                                         = pipe_data_rsp_info_i.reqq_id;
    err_info[DataEcc].trans_id                                        = pipe_data_rsp_info_i.trans_id;
    err_info[DataEcc].info.ecc_err.imprecise                          = ~esr_ctl_i.esr_sc_err_rsp_enable | reqq_state_i[pipe_data_rsp_info_i.reqq_id].err_imprecise;
    err_info[DataEcc].info.ecc_err.index[DataRamAddrSize-1:0]         = pipe_data_rsp_info_i.data_index;
    err_info[DataEcc].info.ecc_err.ram                                = ErrLogRamData;
    if (pipe_data_rsp_valid_i & (|pipe_data_rsp_info_i.data_ram_sbe)) begin
      err_info[DataEcc].info.ecc_err.valid                            = 1'b1;
      err_info[DataEcc].info.ecc_err.bits                             = pipe_data_rsp_info_i.data_ram_sbe;
      err_info[DataEcc].info.ecc_err.code                             = ErrLogEccSbe;
      err_info[DataEcc].info.ecc_err.enabled                          = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccSbe];
    end
    else if (pipe_data_rsp_valid_i & (|pipe_data_rsp_info_i.data_ram_dbe)) begin
      err_info[DataEcc].info.ecc_err.valid                            = 1'b1;
      err_info[DataEcc].info.ecc_err.bits                             = pipe_data_rsp_info_i.data_ram_dbe;
      err_info[DataEcc].info.ecc_err.code                             = ErrLogEccDbe;
      err_info[DataEcc].info.ecc_err.enabled                          = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccDbe];
    end
  end

  // ══════════════════════════════════════════════════════════════
  // Rising-edge detection on ECC/perfmon overflow (2-cycle pipeline)
  // ══════════════════════════════════════════════════════════════
  sbe_dbe_overflow_t       pipe_sbe_dbe_overflow_r1_q;
  perfmon_cntr_overflow_t  perfmon_cntr_overflow_r1_q;
  sbe_dbe_overflow_t       pipe_sbe_dbe_overflow_r2_q;
  perfmon_cntr_overflow_t  perfmon_cntr_overflow_r2_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pipe_sbe_dbe_overflow_r1_q  <= '0;
      perfmon_cntr_overflow_r1_q  <= '0;
    end else begin
      if (|{pipe_sbe_dbe_overflow_i, pipe_sbe_dbe_overflow_r1_q})
        pipe_sbe_dbe_overflow_r1_q <= pipe_sbe_dbe_overflow_i;
      if (|{perfmon_cntr_overflow_i, perfmon_cntr_overflow_r1_q})
        perfmon_cntr_overflow_r1_q <= perfmon_cntr_overflow_i;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pipe_sbe_dbe_overflow_r2_q  <= '0;
      perfmon_cntr_overflow_r2_q  <= '0;
    end else begin
      if (|{pipe_sbe_dbe_overflow_r1_q, pipe_sbe_dbe_overflow_r2_q})
        pipe_sbe_dbe_overflow_r2_q <= pipe_sbe_dbe_overflow_r1_q;
      if (|{perfmon_cntr_overflow_r1_q, perfmon_cntr_overflow_r2_q})
        perfmon_cntr_overflow_r2_q <= perfmon_cntr_overflow_r1_q;
    end
  end

  logic tag_state_ram_sbe_overflow_re;
  logic tag_state_ram_dbe_overflow_re;
  logic tag_ram_sbe_overflow_re;
  logic tag_ram_dbe_overflow_re;
  logic data_ram_sbe_overflow_re;
  logic data_ram_dbe_overflow_re;
  logic cyc_cntr_overflow_re;
  logic p0_cntr_overflow_re;
  logic p1_cntr_overflow_re;

  assign tag_state_ram_sbe_overflow_re = pipe_sbe_dbe_overflow_r1_q.tag_state_ram_sbe_overflow & ~pipe_sbe_dbe_overflow_r2_q.tag_state_ram_sbe_overflow;
  assign tag_state_ram_dbe_overflow_re = pipe_sbe_dbe_overflow_r1_q.tag_state_ram_dbe_overflow & ~pipe_sbe_dbe_overflow_r2_q.tag_state_ram_dbe_overflow;
  assign tag_ram_sbe_overflow_re       = pipe_sbe_dbe_overflow_r1_q.tag_ram_sbe_overflow       & ~pipe_sbe_dbe_overflow_r2_q.tag_ram_sbe_overflow;
  assign tag_ram_dbe_overflow_re       = pipe_sbe_dbe_overflow_r1_q.tag_ram_dbe_overflow       & ~pipe_sbe_dbe_overflow_r2_q.tag_ram_dbe_overflow;
  assign data_ram_sbe_overflow_re      = pipe_sbe_dbe_overflow_r1_q.data_ram_sbe_overflow      & ~pipe_sbe_dbe_overflow_r2_q.data_ram_sbe_overflow;
  assign data_ram_dbe_overflow_re      = pipe_sbe_dbe_overflow_r1_q.data_ram_dbe_overflow      & ~pipe_sbe_dbe_overflow_r2_q.data_ram_dbe_overflow;
  assign cyc_cntr_overflow_re          = perfmon_cntr_overflow_r1_q.cyc_cntr_overflow          & ~perfmon_cntr_overflow_r2_q.cyc_cntr_overflow;
  assign p0_cntr_overflow_re           = perfmon_cntr_overflow_r1_q.p0_cntr_overflow           & ~perfmon_cntr_overflow_r2_q.p0_cntr_overflow;
  assign p1_cntr_overflow_re           = perfmon_cntr_overflow_r1_q.p1_cntr_overflow           & ~perfmon_cntr_overflow_r2_q.p1_cntr_overflow;

  // ── Tag state ECC counter saturation ─────────────────────────
  localparam int unsigned TagStateEccSat = DataEcc - 1;
  always_comb begin
    err_info[TagStateEccSat] = '0;
    err_info[TagStateEccSat].info.ecc_sat_err.imprecise = 1'b1;
    err_info[TagStateEccSat].info.ecc_sat_err.ram       = ErrLogRamTagState;
    err_info[TagStateEccSat].info.ecc_sat_err.code      = ErrLogEccSat;
    err_info[TagStateEccSat].info.ecc_sat_err.enabled   = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccSat];
    if (tag_state_ram_sbe_overflow_re) begin
      err_info[TagStateEccSat].info.ecc_sat_err.valid   = 1'b1;
      err_info[TagStateEccSat].info.ecc_sat_err.dbe     = 1'b0;
    end
    else if (tag_state_ram_dbe_overflow_re) begin
      err_info[TagStateEccSat].info.ecc_sat_err.valid   = 1'b1;
      err_info[TagStateEccSat].info.ecc_sat_err.dbe     = 1'b1;
    end
  end

  // ── Tag ECC counter saturation ───────────────────────────────
  localparam int unsigned TagEccSat = TagStateEccSat - 1;
  always_comb begin
    err_info[TagEccSat] = '0;
    err_info[TagEccSat].info.ecc_sat_err.imprecise = 1'b1;
    err_info[TagEccSat].info.ecc_sat_err.ram       = ErrLogRamTag;
    err_info[TagEccSat].info.ecc_sat_err.code      = ErrLogEccSat;
    err_info[TagEccSat].info.ecc_sat_err.enabled   = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccSat];
    if (tag_ram_sbe_overflow_re) begin
      err_info[TagEccSat].info.ecc_sat_err.valid   = 1'b1;
      err_info[TagEccSat].info.ecc_sat_err.dbe     = 1'b0;
    end
    else if (tag_ram_dbe_overflow_re) begin
      err_info[TagEccSat].info.ecc_sat_err.valid   = 1'b1;
      err_info[TagEccSat].info.ecc_sat_err.dbe     = 1'b1;
    end
  end

  // ── Data ECC counter saturation ──────────────────────────────
  localparam int unsigned DataEccSat = TagEccSat - 1;
  always_comb begin
    err_info[DataEccSat] = '0;
    err_info[DataEccSat].info.ecc_sat_err.imprecise = 1'b1;
    err_info[DataEccSat].info.ecc_sat_err.ram       = ErrLogRamData;
    err_info[DataEccSat].info.ecc_sat_err.code      = ErrLogEccSat;
    err_info[DataEccSat].info.ecc_sat_err.enabled   = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogEccSat];
    if (data_ram_sbe_overflow_re) begin
      err_info[DataEccSat].info.ecc_sat_err.valid   = 1'b1;
      err_info[DataEccSat].info.ecc_sat_err.dbe     = 1'b0;
    end
    else if (data_ram_dbe_overflow_re) begin
      err_info[DataEccSat].info.ecc_sat_err.valid   = 1'b1;
      err_info[DataEccSat].info.ecc_sat_err.dbe     = 1'b1;
    end
  end

  // ── Performance monitor counter saturation (lowest priority) ─
  localparam int unsigned PerfmonCntrSat = DataEccSat - 1;
  always_comb begin
    err_info[PerfmonCntrSat] = '0;
    err_info[PerfmonCntrSat].info.perfmon_sat_err.imprecise = 1'b1;
    err_info[PerfmonCntrSat].info.perfmon_sat_err.code      = ErrLogPerfmonSat;
    err_info[PerfmonCntrSat].info.perfmon_sat_err.enabled   = esr_ctl_i.esr_sc_err_interrupt_enable[ErrLogPerfmonSat];
    if (cyc_cntr_overflow_re) begin
      err_info[PerfmonCntrSat].info.perfmon_sat_err.valid              |= 1'b1;
      err_info[PerfmonCntrSat].info.perfmon_sat_err.cyc_cntr_overflow  |= 1'b1;
    end
    if (p0_cntr_overflow_re) begin
      err_info[PerfmonCntrSat].info.perfmon_sat_err.valid             |= 1'b1;
      err_info[PerfmonCntrSat].info.perfmon_sat_err.p0_cntr_overflow  |= 1'b1;
    end
    if (p1_cntr_overflow_re) begin
      err_info[PerfmonCntrSat].info.perfmon_sat_err.valid             |= 1'b1;
      err_info[PerfmonCntrSat].info.perfmon_sat_err.p1_cntr_overflow  |= 1'b1;
    end
  end

  // ══════════════════════════════════════════════════════════════
  // Signal set_err_detected to reqq for error response
  // ══════════════════════════════════════════════════════════════
  always_comb begin
    set_reqq_err_detected_hot_o  = '0;
    set_reqq_err_zero_data_hot_o = '0;
    for (int ii = 0; ii < NumErr; ii++) begin
      if (err_info[ii].info.decode_err.valid &
          err_info[ii].info.decode_err.enabled &
          ~err_info[ii].info.decode_err.imprecise) begin
        set_reqq_err_detected_hot_o[err_info[ii].reqq_id] |= 1'b1;
      end
      if (err_info[ii].info.decode_err.valid &
          err_info[ii].info.decode_err.code inside {ErrLogDecode}) begin
        set_reqq_err_zero_data_hot_o[err_info[ii].reqq_id] |= 1'b1;
      end
    end
  end

  // ══════════════════════════════════════════════════════════════
  // Priority arbitration — select one error to log
  // ══════════════════════════════════════════════════════════════
  logic [NumErr-1:0] err_valid_hot;
  logic [NumErr-1:0] enabled_err_valid_hot;
  logic [NumErr-1:0] masked_err_valid_hot;

  always_comb begin
    for (int ii = 0; ii < NumErr; ii++) begin
      err_valid_hot[ii]         = err_info[ii].info.decode_err.valid;
      enabled_err_valid_hot[ii] = err_info[ii].info.decode_err.valid &  err_info[ii].info.decode_err.enabled;
      masked_err_valid_hot[ii]  = err_info[ii].info.decode_err.valid & ~err_info[ii].info.decode_err.enabled;
    end
  end

  // Select enabled errors over masked errors
  logic [(2*NumErr)-1:0] err_grant_unused;
  err_log_err_t picked_err_r0;
  prim_arb_prio #(
    .NumClients (2 * NumErr),
    .Width      ($bits(err_log_err_static_t))
  ) u_arb_prio (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    ({enabled_err_valid_hot, masked_err_valid_hot}),
    .stall_i  (1'b0),
    .grant_o  (err_grant_unused),
    .data_i   ({2{err_info}}),
    .data_o   (picked_err_r0)
  );

  // Register result
  logic valid_r0;
  assign valid_r0 = |err_valid_hot;

  logic valid_r1_q;
  err_log_err_t picked_err_r1_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      valid_r1_q <= 1'b0;
    end else if (valid_r0 | valid_r1_q) begin
      valid_r1_q <= valid_r0;
    end
  end

  always_ff @(posedge clk_i) begin
    if (valid_r0) begin
      picked_err_r1_q <= picked_err_r0;
    end
  end

  // ══════════════════════════════════════════════════════════════
  // Look up remaining error info from reqq_state
  // ══════════════════════════════════════════════════════════════

  // reqq_state mux: used for err_log info, reqq debug, or reqq trace
  logic [NumEntries-1:0] reqq_traced_hot;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      reqq_traced_hot[ii] = reqq_state_i[ii].valid & reqq_state_i[ii].traced;
    end
  end

  logic [$clog2(NumEntries)-1:0] traced_reqq_id_bin;
  logic [ReqqIdSize-1:0] traced_reqq_id;
  prim_hot2bin #(
    .HotW (NumEntries)
  ) u_traced_hot2bin (
    .hot_i (reqq_traced_hot),
    .bin_o (traced_reqq_id_bin)
  );
  assign traced_reqq_id = traced_reqq_id_bin[ReqqIdSize-1:0];

  reqq_entry_state_t reqq_state_r1;
  logic [ReqqIdSize-1:0] reqq_id_r1;
  err_log_err_t err_info_r1;

  assign reqq_id_r1 = (reqq_debug_query_reqq_entry) ? esr_ctl_i.wr_data[ReqqIdSize-1:0] :
                       (valid_r1_q) ? picked_err_r1_q.reqq_id :
                       (esr_ctl_i.esr_sc_trace_type_hot_enable[TraceEnReqq]) ? traced_reqq_id :
                       '0;
  assign reqq_state_r1 = reqq_state_i[reqq_id_r1];

  assign traced_reqq_state_o = reqq_state_r1;

  always_comb begin
    err_info_r1 = picked_err_r1_q;
    if (err_info_r1.info.decode_err.code == ErrLogDecode) begin
      err_info_r1.info.decode_err.opcode                              = reqq_state_r1.orig_opcode;
      err_info_r1.info.decode_err.src                                 = reqq_state_r1.reqq_source;
      err_info_r1.info.decode_err.et_link_tag_id[ReqqTagIdSize-1:0]   = reqq_state_r1.tag_id;
      err_info_r1.info.decode_err.et_link_source[ReqqSourceSize-1:0]  = reqq_state_r1.source;
      err_info_r1.info.decode_err.port[PortIdSize-1:0]                = reqq_state_r1.port_id;
    end
    if (err_info_r1.info.decode_err.code inside {ErrLogDecode, ErrLogEccSbe, ErrLogEccDbe}) begin
      if (~err_info_r1.address_valid) begin
        err_info_r1.address_valid         = 1'b1;
        err_info_r1.address[PaSize-1:0]   = reqq_state_r1.address;
      end
    end
  end

  // ══════════════════════════════════════════════════════════════
  // Update error log register values
  // ══════════════════════════════════════════════════════════════
  err_log_err_t err_log_q;
  err_log_err_t err_log_d;

  err_log_info_decode_t esr_wr_data_err_log_info_decode;
  assign esr_wr_data_err_log_info_decode = err_log_info_decode_t'(esr_ctl_i.wr_data);

  logic clear_err;
  assign clear_err = esr_ctl_i.wr_en_err_log_info & esr_ctl_i.wr_data[0] &
                     (esr_wr_data_err_log_info_decode.code == err_log_q.info.decode_err.code);

  logic load_err;
  assign load_err =
    (valid_r1_q & ~err_log_q.info.decode_err.valid) |
    (valid_r1_q & clear_err) |
    (valid_r1_q & err_info_r1.info.decode_err.enabled & ~err_log_q.info.decode_err.enabled);

  logic set_multi;
  assign set_multi =
    valid_r1_q                         & err_info_r1.info.decode_err.enabled &
    err_log_q.info.decode_err.valid    & err_log_q.info.decode_err.enabled &
    ~clear_err;

  always_comb begin
    err_log_d = err_log_q;
    if (clear_err) begin
      err_log_d.info.decode_err.valid = 1'b0;
      err_log_d.info.decode_err.multi = 1'b0;
    end
    if (load_err) begin
      err_log_d = err_info_r1;
    end
    if (set_multi) begin
      err_log_d.info.decode_err.multi = 1'b1;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      err_log_q <= '0;
    end else if (clear_err | load_err | set_multi) begin
      err_log_q <= err_log_d;
    end
  end

  assign err_log_esr_status_o.info    = err_log_q.info;
  assign err_log_esr_status_o.address = err_log_q.address;

  // ══════════════════════════════════════════════════════════════
  // Signal err_detected and err_logged
  // ══════════════════════════════════════════════════════════════
  logic err_detected_d;
  assign err_detected_d =
    err_log_d.info.decode_err.valid &
    err_log_d.info.decode_err.enabled &
    err_log_d.info.decode_err.imprecise;

  logic err_logged_d;
  assign err_logged_d =
    err_log_d.info.decode_err.valid &
    err_log_d.info.decode_err.enabled;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)                              err_logged_o <= 1'b0;
    else if (clear_err | load_err | set_multi) err_logged_o <= err_logged_d;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)                              err_detected_o <= 1'b0;
    else if (clear_err | load_err | set_multi) err_detected_o <= err_detected_d;
  end

  // ══════════════════════════════════════════════════════════════
  // REQQ debug
  // ══════════════════════════════════════════════════════════════
  assign reqq_debug_query_reqq_entry = esr_ctl_i.wr_en_reqq_debug_ctl & ~esr_ctl_i.wr_data[8];
  assign reqq_debug_query_reqq_valid = esr_ctl_i.wr_en_reqq_debug_ctl & (esr_ctl_i.wr_data[8:0] == 9'h100);

  logic [NumEntries-1:0] reqq_valid;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      reqq_valid[ii] = reqq_state_i[ii].valid;
    end
  end

  logic [3*EsrStatusSize-1:0] io_valid_ready;
  assign io_valid_ready = (3*EsrStatusSize)'({
    bank_req_valid_i,
    bank_req_ready_i,
    bank_rsp_valid_i,
    bank_rsp_ready_i,
    to_l3_mesh_master_bank_req_valid_i,
    to_l3_mesh_master_bank_req_ready_i,
    to_l3_mesh_master_bank_rsp_valid_i,
    to_l3_mesh_master_bank_rsp_ready_i,
    to_sys_mesh_master_bank_req_valid_i,
    to_sys_mesh_master_bank_req_ready_i,
    to_sys_mesh_master_bank_rsp_valid_i,
    to_sys_mesh_master_bank_rsp_ready_i,
    l3_mesh_slave_bank_req_valid_i,
    l3_mesh_slave_bank_req_ready_i,
    l3_mesh_slave_bank_rsp_valid_i,
    l3_mesh_slave_bank_rsp_ready_i
    });

  logic [4*EsrStatusSize-1:0] reqq_debug_status_d;
  logic [4*EsrStatusSize-1:0] reqq_debug_status_q;
  assign reqq_debug_status_d =
    (reqq_debug_query_reqq_entry) ? (4*EsrStatusSize)'(reqq_state_r1) :
    (reqq_debug_query_reqq_valid) ? (4*EsrStatusSize)'(reqq_valid) :
                                    io_valid_ready;

  always_ff @(posedge clk_i) begin
    if (esr_ctl_i.wr_en_reqq_debug_ctl) begin
      reqq_debug_status_q <= reqq_debug_status_d;
    end
  end

  assign {err_log_esr_status_o.reqq_debug3,
          err_log_esr_status_o.reqq_debug2,
          err_log_esr_status_o.reqq_debug1,
          err_log_esr_status_o.reqq_debug0} = reqq_debug_status_q;

  // ══════════════════════════════════════════════════════════════
  // Unused signals
  // ══════════════════════════════════════════════════════════════
  logic unused_ok;
  assign unused_ok = &{
    1'b0,
    pipe_tag_rsp_info_i,
    pipe_data_rsp_info_i,
    to_l3_mesh_rsp_info_i,
    to_sys_mesh_rsp_info_i,
    alloc_info,
    alloc_l2_info_ad_i,
    alloc_l3_info_ad_i,
    esr_ctl_i,
    esr_wr_data_err_log_info_decode,
    reqq_state_i,
    err_grant_unused
  };

  /* verilator lint_on MULTIDRIVEN */
  /* verilator lint_on WIDTHEXPAND */
  /* verilator lint_on WIDTHTRUNC */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : shirecache_bank_err_logger
