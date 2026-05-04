// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_l3_slave.
// Breaks packed struct ports into scalars for easy C++ driving.
// Reqq state array: 64 entries, set via indexed scalar ports (up to 2 entries).

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module shirecache_l3_slave_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,

  // ── L3 mesh slave request (scalar inputs) ─────────────────
  input  logic                              req_valid_i,
  input  logic [MeshMasterAxiIdSize-1:0]    req_id_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   req_port_id_i,
  input  logic [MeshSourceBridgeIdSize-1:0] req_source_i,
  input  logic [4:0]                        req_opcode_i,
  input  logic [PaSize-1:0]                 req_address_i,
  input  logic [AxiAxqosSize-1:0]           req_qos_i,
  input  logic                              req_wdata_i,
  input  logic [LineQwSize-1:0]             req_qwen_i,
  input  logic                              req_ben_en_i,
  input  logic [2:0]                        req_size_i,

  // ── Bank ready ────────────────────────────────────────────
  input  logic                              bank_l3_ready_i,

  // ── Source 0: pipe data rsp ───────────────────────────────
  input  logic                              pipe_data_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             pipe_data_rsp_reqq_id_i,
  input  logic [DvTransIdSize-1:0]          pipe_data_rsp_trans_id_i,
  input  logic                              pipe_data_rsp_type_i,
  input  logic [LineDwSize-1:0]             pipe_data_rsp_sbe_i,
  input  logic [LineDwSize-1:0]             pipe_data_rsp_dbe_i,
  input  logic [1:0]                        pipe_data_rsp_mod_reqq_source_i,
  input  logic                              pipe_data_rsp_mod_redirect_i,

  // ── Source 1: to_sys mesh rsp ─────────────────────────────
  input  logic                              to_sys_mesh_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             to_sys_mesh_rsp_reqq_id_i,
  input  logic [DvTransIdSize-1:0]          to_sys_mesh_rsp_trans_id_i,

  // ── Source 2: reqq nodata ─────────────────────────────────
  input  logic                              reqq_rspmux_valid_nodata_i,
  input  logic [ReqqIdSize-1:0]             reqq_rspmux_reqq_id_i,
  input  logic [DvTransIdSize-1:0]          reqq_rspmux_trans_id_i,
  input  logic                              reqq_rspmux_with_data_i,

  // ── Source 3: dataq ───────────────────────────────────────
  input  logic                              dataq_rspmux_valid_i,
  input  logic [ReqqIdSize-1:0]             dataq_rspmux_reqq_id_i,
  input  logic [DvTransIdSize-1:0]          dataq_rspmux_trans_id_i,

  // ── Source 4: rbuf read rsp ───────────────────────────────
  input  logic                              pipe_rbuf_rd_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]             pipe_rbuf_rsp_reqq_id_i,
  input  logic [DvTransIdSize-1:0]          pipe_rbuf_rsp_trans_id_i,
  input  logic                              pipe_rbuf_rsp_l3_source_i,

  // ── ESR control ───────────────────────────────────────────
  input  logic                              esr_err_rsp_enable_i,
  input  logic                              esr_err_int_sbe_enable_i,
  input  logic                              esr_err_int_dbe_enable_i,

  // ── Reqq state config (2 entries) ─────────────────────────
  input  logic [ReqqIdSize-1:0]             rs_idx0_i,
  input  logic [ReqqTagIdSize-1:0]          rs_tag_id0_i,
  input  logic [ReqqSourceSize-1:0]         rs_source0_i,
  input  logic [PortIdSize-1:0]             rs_port_id0_i,
  input  logic                              rs_err_detected0_i,
  input  logic                              rs_err_zero_data0_i,

  input  logic [ReqqIdSize-1:0]             rs_idx1_i,
  input  logic [ReqqTagIdSize-1:0]          rs_tag_id1_i,
  input  logic [ReqqSourceSize-1:0]         rs_source1_i,
  input  logic [PortIdSize-1:0]             rs_port_id1_i,
  input  logic                              rs_err_detected1_i,
  input  logic                              rs_err_zero_data1_i,

  // ── Mesh rsp ready (output FIFO consumer) ─────────────────
  input  logic                              mesh_rsp_ready_i,

  // ══════════════════════════════════════════════════════════
  // Outputs
  // ══════════════════════════════════════════════════════════

  // Request path
  output logic                              req_ready_o,
  output logic                              bank_l3_valid_o,
  output logic [4:0]                        bank_l3_opcode_o,
  output logic [ReqqTagIdSize-1:0]          bank_l3_id_o,
  output logic [ReqqSourceSize-1:0]         bank_l3_source_o,
  output logic [AxiAxqosSize-1:0]           bank_l3_qos_o,
  output logic                              bank_l3_wdata_o,
  output logic                              bank_l3_ben_en_o,
  output logic [PaSize-1:0]                 bank_l3_address_o,
  output logic [PortIdSize-1:0]             bank_l3_port_id_o,

  // Per-source ready
  output logic                              pipe_data_rsp_ready_o,
  output logic                              to_sys_mesh_rsp_ready_o,
  output logic                              reqq_rspmux_ready_nodata_o,
  output logic                              dataq_rspmux_ready_o,
  output logic                              pipe_rbuf_rd_rsp_ready_o,

  // Mesh slave response output
  output logic                              rsp_valid_o,
  output logic                              rsp_wdata_o,
  output logic                              rsp_err_o,
  output logic [MeshMasterAxiIdSize-1:0]    rsp_id_o,
  output logic [MeshSourceBridgeIdSize-1:0] rsp_source_o,

  // Rspmux sent notification
  output logic                              rspmux_sent_valid_o,
  output logic [NumEntriesId-1:0]           rspmux_sent_id_o,
  output logic [NumEntries-1:0]             rspmux_sent_valid_hot_o,
  output logic                              a_rspmux_rsp_is_valid_o
);

  // ── Build reqq state ────────────────────────────────────────
  reqq_entry_state_t [NumEntries-1:0] reqq_state;
  always_comb begin
    reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */
    reqq_state[rs_idx0_i].tag_id        = rs_tag_id0_i;
    reqq_state[rs_idx0_i].source        = rs_source0_i;
    reqq_state[rs_idx0_i].port_id       = rs_port_id0_i;
    reqq_state[rs_idx0_i].err_detected  = rs_err_detected0_i;
    reqq_state[rs_idx0_i].err_zero_data = rs_err_zero_data0_i;

    reqq_state[rs_idx1_i].tag_id        = rs_tag_id1_i;
    reqq_state[rs_idx1_i].source        = rs_source1_i;
    reqq_state[rs_idx1_i].port_id       = rs_port_id1_i;
    reqq_state[rs_idx1_i].err_detected  = rs_err_detected1_i;
    reqq_state[rs_idx1_i].err_zero_data = rs_err_zero_data1_i;
  end

  // ── Build ESR ctl ───────────────────────────────────────────
  bank_esr_ctl_t esr_ctl;
  always_comb begin
    esr_ctl = '0;
    esr_ctl.esr_sc_err_rsp_enable = esr_err_rsp_enable_i;
    esr_ctl.esr_sc_err_interrupt_enable[int'(ErrLogEccSbe)] = esr_err_int_sbe_enable_i;
    esr_ctl.esr_sc_err_interrupt_enable[int'(ErrLogEccDbe)] = esr_err_int_dbe_enable_i;
  end

  // ── Build mesh slave request struct ─────────────────────────
  mesh_slave_req_t mesh_slave_req;
  always_comb begin
    mesh_slave_req          = '0;
    mesh_slave_req.id       = req_id_i;
    mesh_slave_req.port_id  = req_port_id_i;
    mesh_slave_req.source   = req_source_i;
    mesh_slave_req.opcode   = etlink_pkg::req_opcode_e'(req_opcode_i);
    mesh_slave_req.address  = req_address_i;
    mesh_slave_req.qos      = req_qos_i;
    mesh_slave_req.wdata    = req_wdata_i;
    mesh_slave_req.qwen     = req_qwen_i;
    mesh_slave_req.ben_en   = req_ben_en_i;
    mesh_slave_req.size     = sc_size_t'(req_size_i);
  end

  // ── Build pipe data rsp struct ──────────────────────────────
  data_rsp_t pipe_data_rsp_info;
  always_comb begin
    pipe_data_rsp_info = '0;
    pipe_data_rsp_info.trans_id     = pipe_data_rsp_trans_id_i;
    pipe_data_rsp_info.reqq_id      = pipe_data_rsp_reqq_id_i;
    pipe_data_rsp_info.rsp_type     = pipe_data_rsp_type_e'(pipe_data_rsp_type_i);
    pipe_data_rsp_info.data_ram_sbe = pipe_data_rsp_sbe_i;
    pipe_data_rsp_info.data_ram_dbe = pipe_data_rsp_dbe_i;
  end

  data_rsp_modifier_t pipe_data_rsp_modifier_info;
  always_comb begin
    pipe_data_rsp_modifier_info = '0;
    pipe_data_rsp_modifier_info.reqq_source = reqq_source_e'(pipe_data_rsp_mod_reqq_source_i);
    pipe_data_rsp_modifier_info.redirect_normal_rsp_to_mesh = pipe_data_rsp_mod_redirect_i;
  end

  // ── Build to_sys mesh rsp struct ────────────────────────────
  reqq_mesh_rsp_t to_sys_mesh_rsp_info;
  always_comb begin
    to_sys_mesh_rsp_info = '0;
    to_sys_mesh_rsp_info.trans_id = to_sys_mesh_rsp_trans_id_i;
    to_sys_mesh_rsp_info.reqq_id  = to_sys_mesh_rsp_reqq_id_i;
  end

  // ── Build reqq rspmux struct ────────────────────────────────
  rspmux_t reqq_rspmux_info;
  always_comb begin
    reqq_rspmux_info = '0;
    reqq_rspmux_info.trans_id  = reqq_rspmux_trans_id_i;
    reqq_rspmux_info.reqq_id   = reqq_rspmux_reqq_id_i;
    reqq_rspmux_info.with_data = reqq_rspmux_with_data_i;
  end

  // ── Build dataq rspmux struct ───────────────────────────────
  rspmux_t dataq_rspmux_info;
  always_comb begin
    dataq_rspmux_info = '0;
    dataq_rspmux_info.trans_id  = dataq_rspmux_trans_id_i;
    dataq_rspmux_info.reqq_id   = dataq_rspmux_reqq_id_i;
    dataq_rspmux_info.with_data = 1'b1;
  end

  // ── Build rbuf rd rsp struct ────────────────────────────────
  rbuf_rd_rsp_t pipe_rbuf_rd_rsp_info;
  always_comb begin
    pipe_rbuf_rd_rsp_info = '0;
    pipe_rbuf_rd_rsp_info.trans_id  = pipe_rbuf_rsp_trans_id_i;
    pipe_rbuf_rd_rsp_info.reqq_id   = pipe_rbuf_rsp_reqq_id_i;
    pipe_rbuf_rd_rsp_info.l3_source = pipe_rbuf_rsp_l3_source_i;
  end

  // ── DUT instantiation ──────────────────────────────────────
  reqq_bank_req_t   bank_l3_info;
  mesh_slave_rsp_t  rsp_info;

  shirecache_bank_l3_slave #(
    .NumEntries  (NumEntries),
    .NumEntriesId(NumEntriesId)
  ) u_dut (
    .clk_i,
    .rst_ni,

    .l3_mesh_slave_bank_req_valid_i (req_valid_i),
    .l3_mesh_slave_bank_req_info_i  (mesh_slave_req),
    .l3_mesh_slave_bank_req_ready_o (req_ready_o),

    .l3_mesh_slave_bank_rsp_valid_o (rsp_valid_o),
    .l3_mesh_slave_bank_rsp_info_o  (rsp_info),
    .l3_mesh_slave_bank_rsp_ready_i (mesh_rsp_ready_i),

    .bank_l3_valid_o,
    .bank_l3_info_o                 (bank_l3_info),
    .bank_l3_ready_i,

    .pipe_data_rsp_valid_i,
    .pipe_data_rsp_info_i           (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i  (pipe_data_rsp_modifier_info),
    .pipe_data_rsp_ready_o,

    .to_sys_mesh_rsp_valid_l3_slave_i (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info_i           (to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_l3_slave_o (to_sys_mesh_rsp_ready_o),

    .reqq_rspmux_valid_nodata_i,
    .reqq_rspmux_info_i             (reqq_rspmux_info),
    .reqq_rspmux_ready_nodata_o,

    .dataq_rspmux_valid_i,
    .dataq_rspmux_info_i            (dataq_rspmux_info),
    .dataq_rspmux_ready_o,

    .pipe_rbuf_rd_rsp_valid_i,
    .pipe_rbuf_rd_rsp_info_i        (pipe_rbuf_rd_rsp_info),
    .pipe_rbuf_rd_rsp_ready_o,

    .rspmux_sent_valid_o,
    .rspmux_sent_id_o,
    .rspmux_sent_valid_hot_o,
    .a_rspmux_rsp_is_valid_o,

    .reqq_state_i                   (reqq_state),
    .esr_ctl_i                      (esr_ctl)
  );

  // ── Extract scalar outputs from structs ─────────────────────
  assign bank_l3_opcode_o  = bank_l3_info.opcode;
  assign bank_l3_id_o      = bank_l3_info.id;
  assign bank_l3_source_o  = bank_l3_info.source;
  assign bank_l3_qos_o     = bank_l3_info.qos;
  assign bank_l3_wdata_o   = bank_l3_info.wdata;
  assign bank_l3_ben_en_o  = bank_l3_info.write_is_partial;
  assign bank_l3_address_o = bank_l3_info.address;
  assign bank_l3_port_id_o = bank_l3_info.port_id;

  assign rsp_wdata_o  = rsp_info.wdata;
  assign rsp_err_o    = rsp_info.err;
  assign rsp_id_o     = rsp_info.id;
  assign rsp_source_o = rsp_info.source;

  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : shirecache_l3_slave_tb
