// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_err_logger.
// Decomposes struct inputs/outputs to scalar ports for C++ driving.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off WIDTHEXPAND */

module shirecache_err_logger_tb
  import shirecache_pkg::*;
#(
  parameter int unsigned NumPorts     = Ports,
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,

  // ── Outputs ───────────────────────────────────────────────
  output logic                              err_detected_o,
  output logic                              err_logged_o,
  output logic [NumEntries-1:0]             set_reqq_err_detected_hot_o,
  output logic [NumEntries-1:0]             set_reqq_err_zero_data_hot_o,

  // ESR info + address
  output logic [EsrStatusSize-1:0]          esr_info_o,
  output logic [EsrStatusSize-1:0]          esr_address_o,
  output logic [EsrStatusSize-1:0]          esr_reqq_debug0_o,
  output logic [EsrStatusSize-1:0]          esr_reqq_debug1_o,
  output logic [EsrStatusSize-1:0]          esr_reqq_debug2_o,
  output logic [EsrStatusSize-1:0]          esr_reqq_debug3_o,

  // ── Allocation decode error ───────────────────────────────
  input  logic                              alloc_l2_valid_i,
  input  logic [ReqqIdSize-1:0]             alloc_l2_reqq_id_i,
  input  logic                              alloc_l2_err_valid_i,
  input  logic [4:0]                        alloc_l2_err_type_i,
  input  logic                              alloc_l2_err_imprecise_i,

  input  logic                              alloc_l3_valid_i,
  input  logic [ReqqIdSize-1:0]             alloc_l3_reqq_id_i,
  input  logic                              alloc_l3_err_valid_i,
  input  logic [4:0]                        alloc_l3_err_type_i,
  input  logic                              alloc_l3_err_imprecise_i,

  // ── Tag error ─────────────────────────────────────────────
  input  logic                              pipe_tag_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             pipe_tag_rsp_reqq_id_i,
  input  logic [4:0]                        pipe_tag_rsp_err_rsp_i,
  input  logic [SetIdSize-1:0]              pipe_tag_rsp_tag_index_i,
  input  logic [PaSize-1:0]                 pipe_tag_rsp_orig_address_i,
  input  logic                              pipe_tag_rsp_tag_state_ram_sbe_i,
  input  logic                              pipe_tag_rsp_tag_state_ram_dbe_i,
  input  logic [Ways-1:0]                   pipe_tag_rsp_tag_ram_sbe_i,
  input  logic [Ways-1:0]                   pipe_tag_rsp_tag_ram_dbe_i,

  // ── Data error ────────────────────────────────────────────
  input  logic                              pipe_data_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             pipe_data_rsp_reqq_id_i,
  input  logic [DataRamAddrSize-1:0]        pipe_data_rsp_data_index_i,
  input  logic [LineDwSize-1:0]             pipe_data_rsp_data_ram_sbe_i,
  input  logic [LineDwSize-1:0]             pipe_data_rsp_data_ram_dbe_i,

  // ── Mesh response errors ──────────────────────────────────
  input  logic                              to_l3_mesh_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             to_l3_mesh_rsp_reqq_id_i,
  input  logic                              to_l3_mesh_rsp_err_i,
  input  logic                              to_sys_mesh_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             to_sys_mesh_rsp_reqq_id_i,
  input  logic                              to_sys_mesh_rsp_err_i,

  // ── ECC overflow ──────────────────────────────────────────
  input  logic                              tag_state_sbe_overflow_i,
  input  logic                              tag_state_dbe_overflow_i,
  input  logic                              tag_sbe_overflow_i,
  input  logic                              tag_dbe_overflow_i,
  input  logic                              data_sbe_overflow_i,
  input  logic                              data_dbe_overflow_i,

  // ── Perfmon overflow ──────────────────────────────────────
  input  logic                              cyc_cntr_overflow_i,
  input  logic                              p0_cntr_overflow_i,
  input  logic                              p1_cntr_overflow_i,

  // ── Reqq state config (1 indexed entry at a time) ─────────
  input  logic [ReqqIdSize-1:0]             rs_idx_i,
  input  logic                              rs_valid_i,
  input  logic                              rs_err_imprecise_i,
  input  logic [PaSize-1:0]                 rs_address_i,
  input  logic [4:0]                        rs_orig_opcode_i,
  input  logic [1:0]                        rs_reqq_source_i,
  input  logic [ReqqTagIdSize-1:0]          rs_tag_id_i,
  input  logic [ReqqSourceSize-1:0]         rs_source_i,
  input  logic [PortIdSize-1:0]             rs_port_id_i,
  input  logic                              rs_traced_i,

  // ── I/O valid/ready ───────────────────────────────────────
  input  logic                              bank_req_valid_i,
  input  logic                              bank_req_ready_i,
  input  logic [NumPorts-1:0]               bank_rsp_valid_i,
  input  logic [NumPorts-1:0]               bank_rsp_ready_i,
  input  logic                              to_l3_mm_req_valid_i,
  input  logic                              to_l3_mm_req_ready_i,
  input  logic                              to_l3_mm_rsp_valid_i,
  input  logic                              to_l3_mm_rsp_ready_i,
  input  logic                              to_sys_mm_req_valid_i,
  input  logic                              to_sys_mm_req_ready_i,
  input  logic                              to_sys_mm_rsp_valid_i,
  input  logic                              to_sys_mm_rsp_ready_i,
  input  logic                              l3_ms_req_valid_i,
  input  logic                              l3_ms_req_ready_i,
  input  logic                              l3_ms_rsp_valid_i,
  input  logic                              l3_ms_rsp_ready_i,

  // ── ESR control ───────────────────────────────────────────
  input  logic                              esr_err_rsp_enable_i,
  input  logic [ErrIntSize-1:0]             esr_err_int_enable_i,
  input  logic [TraceEnSize-1:0]            esr_trace_type_hot_enable_i,
  input  logic                              esr_wr_en_err_log_info_i,
  input  logic                              esr_wr_en_reqq_debug_ctl_i,
  input  logic [DwDataSize-1:0]             esr_wr_data_i
);

  // Build struct inputs
  reqq_alloc_t alloc_l2_info, alloc_l3_info;
  always_comb begin
    alloc_l2_info = '0;
    alloc_l2_info.reqq_id       = alloc_l2_reqq_id_i;
    alloc_l2_info.err_valid     = alloc_l2_err_valid_i;
    alloc_l2_info.err_type      = err_rsp_e'(alloc_l2_err_type_i);
    alloc_l2_info.err_imprecise = alloc_l2_err_imprecise_i;

    alloc_l3_info = '0;
    alloc_l3_info.reqq_id       = alloc_l3_reqq_id_i;
    alloc_l3_info.err_valid     = alloc_l3_err_valid_i;
    alloc_l3_info.err_type      = err_rsp_e'(alloc_l3_err_type_i);
    alloc_l3_info.err_imprecise = alloc_l3_err_imprecise_i;
  end

  tag_rsp_t pipe_tag_rsp_info;
  always_comb begin
    pipe_tag_rsp_info = '0;
    pipe_tag_rsp_info.reqq_id            = pipe_tag_rsp_reqq_id_i;
    pipe_tag_rsp_info.err_rsp            = err_rsp_e'(pipe_tag_rsp_err_rsp_i);
    pipe_tag_rsp_info.tag_index          = pipe_tag_rsp_tag_index_i;
    pipe_tag_rsp_info.orig_address       = pipe_tag_rsp_orig_address_i;
    pipe_tag_rsp_info.tag_state_ram_sbe  = pipe_tag_rsp_tag_state_ram_sbe_i;
    pipe_tag_rsp_info.tag_state_ram_dbe  = pipe_tag_rsp_tag_state_ram_dbe_i;
    pipe_tag_rsp_info.tag_ram_sbe        = pipe_tag_rsp_tag_ram_sbe_i;
    pipe_tag_rsp_info.tag_ram_dbe        = pipe_tag_rsp_tag_ram_dbe_i;
  end

  data_rsp_t pipe_data_rsp_info;
  always_comb begin
    pipe_data_rsp_info = '0;
    pipe_data_rsp_info.reqq_id       = pipe_data_rsp_reqq_id_i;
    pipe_data_rsp_info.data_index    = pipe_data_rsp_data_index_i;
    pipe_data_rsp_info.data_ram_sbe  = pipe_data_rsp_data_ram_sbe_i;
    pipe_data_rsp_info.data_ram_dbe  = pipe_data_rsp_data_ram_dbe_i;
  end

  reqq_mesh_rsp_t to_l3_mesh_rsp_info, to_sys_mesh_rsp_info;
  always_comb begin
    to_l3_mesh_rsp_info = '0;
    to_l3_mesh_rsp_info.reqq_id = to_l3_mesh_rsp_reqq_id_i;
    to_l3_mesh_rsp_info.err     = to_l3_mesh_rsp_err_i;

    to_sys_mesh_rsp_info = '0;
    to_sys_mesh_rsp_info.reqq_id = to_sys_mesh_rsp_reqq_id_i;
    to_sys_mesh_rsp_info.err     = to_sys_mesh_rsp_err_i;
  end

  sbe_dbe_overflow_t pipe_sbe_dbe_overflow;
  assign pipe_sbe_dbe_overflow.tag_state_ram_sbe_overflow = tag_state_sbe_overflow_i;
  assign pipe_sbe_dbe_overflow.tag_state_ram_dbe_overflow = tag_state_dbe_overflow_i;
  assign pipe_sbe_dbe_overflow.tag_ram_sbe_overflow       = tag_sbe_overflow_i;
  assign pipe_sbe_dbe_overflow.tag_ram_dbe_overflow       = tag_dbe_overflow_i;
  assign pipe_sbe_dbe_overflow.data_ram_sbe_overflow      = data_sbe_overflow_i;
  assign pipe_sbe_dbe_overflow.data_ram_dbe_overflow      = data_dbe_overflow_i;

  perfmon_cntr_overflow_t perfmon_cntr_overflow;
  assign perfmon_cntr_overflow.cyc_cntr_overflow = cyc_cntr_overflow_i;
  assign perfmon_cntr_overflow.p0_cntr_overflow  = p0_cntr_overflow_i;
  assign perfmon_cntr_overflow.p1_cntr_overflow  = p1_cntr_overflow_i;

  // Build reqq state array
  /* verilator lint_off WIDTHCONCAT */
  reqq_entry_state_t [NumEntries-1:0] reqq_state;
  always_comb begin
    reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */
    reqq_state[rs_idx_i].valid         = rs_valid_i;
    reqq_state[rs_idx_i].err_imprecise = rs_err_imprecise_i;
    reqq_state[rs_idx_i].address       = rs_address_i;
    reqq_state[rs_idx_i].orig_opcode   = reqq_orig_opcode_e'(rs_orig_opcode_i);
    reqq_state[rs_idx_i].reqq_source   = reqq_source_e'(rs_reqq_source_i);
    reqq_state[rs_idx_i].tag_id        = rs_tag_id_i;
    reqq_state[rs_idx_i].source        = rs_source_i;
    reqq_state[rs_idx_i].port_id       = rs_port_id_i;
    reqq_state[rs_idx_i].traced        = rs_traced_i;
  end

  // Build ESR control
  bank_esr_ctl_t esr_ctl;
  always_comb begin
    esr_ctl = '0;
    esr_ctl.esr_sc_err_rsp_enable          = esr_err_rsp_enable_i;
    esr_ctl.esr_sc_err_interrupt_enable    = esr_err_int_enable_i;
    esr_ctl.esr_sc_trace_type_hot_enable   = esr_trace_type_hot_enable_i;
    esr_ctl.wr_en_err_log_info             = esr_wr_en_err_log_info_i;
    esr_ctl.wr_en_reqq_debug_ctl           = esr_wr_en_reqq_debug_ctl_i;
    esr_ctl.wr_data                        = esr_wr_data_i;
  end

  err_log_esr_status_t err_log_esr_status;
  reqq_entry_state_t traced_reqq_state;

  shirecache_bank_err_logger #(
    .NumPorts     (NumPorts),
    .NumEntries   (NumEntries),
    .NumEntriesId (NumEntriesId)
  ) u_dut (
    .clk_i                                 (clk_i),
    .rst_ni                                (rst_ni),
    .err_detected_o                        (err_detected_o),
    .err_logged_o                          (err_logged_o),
    .set_reqq_err_detected_hot_o           (set_reqq_err_detected_hot_o),
    .set_reqq_err_zero_data_hot_o          (set_reqq_err_zero_data_hot_o),
    .traced_reqq_state_o                   (traced_reqq_state),
    .alloc_l2_valid_ad_i                   (alloc_l2_valid_i),
    .alloc_l2_info_ad_i                    (alloc_l2_info),
    .alloc_l3_valid_ad_i                   (alloc_l3_valid_i),
    .alloc_l3_info_ad_i                    (alloc_l3_info),
    .pipe_tag_rsp_valid_i                  (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info_i                   (pipe_tag_rsp_info),
    .pipe_data_rsp_valid_i                 (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info_i                  (pipe_data_rsp_info),
    .to_l3_mesh_rsp_valid_i                (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info_i                 (to_l3_mesh_rsp_info),
    .to_sys_mesh_rsp_valid_i               (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info_i                (to_sys_mesh_rsp_info),
    .pipe_sbe_dbe_overflow_i               (pipe_sbe_dbe_overflow),
    .perfmon_cntr_overflow_i               (perfmon_cntr_overflow),
    .reqq_state_i                          (reqq_state),
    .bank_req_valid_i                      (bank_req_valid_i),
    .bank_req_ready_i                      (bank_req_ready_i),
    .bank_rsp_valid_i                      (bank_rsp_valid_i),
    .bank_rsp_ready_i                      (bank_rsp_ready_i),
    .to_l3_mesh_master_bank_req_valid_i    (to_l3_mm_req_valid_i),
    .to_l3_mesh_master_bank_req_ready_i    (to_l3_mm_req_ready_i),
    .to_l3_mesh_master_bank_rsp_valid_i    (to_l3_mm_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_ready_i    (to_l3_mm_rsp_ready_i),
    .to_sys_mesh_master_bank_req_valid_i   (to_sys_mm_req_valid_i),
    .to_sys_mesh_master_bank_req_ready_i   (to_sys_mm_req_ready_i),
    .to_sys_mesh_master_bank_rsp_valid_i   (to_sys_mm_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_ready_i   (to_sys_mm_rsp_ready_i),
    .l3_mesh_slave_bank_req_valid_i        (l3_ms_req_valid_i),
    .l3_mesh_slave_bank_req_ready_i        (l3_ms_req_ready_i),
    .l3_mesh_slave_bank_rsp_valid_i        (l3_ms_rsp_valid_i),
    .l3_mesh_slave_bank_rsp_ready_i        (l3_ms_rsp_ready_i),
    .err_log_esr_status_o                  (err_log_esr_status),
    .esr_ctl_i                             (esr_ctl)
  );

  assign esr_info_o         = err_log_esr_status.info;
  assign esr_address_o      = err_log_esr_status.address;
  assign esr_reqq_debug0_o  = err_log_esr_status.reqq_debug0;
  assign esr_reqq_debug1_o  = err_log_esr_status.reqq_debug1;
  assign esr_reqq_debug2_o  = err_log_esr_status.reqq_debug2;
  assign esr_reqq_debug3_o  = err_log_esr_status.reqq_debug3;

  logic unused_ok;
  assign unused_ok = &{1'b0, traced_reqq_state};

endmodule
