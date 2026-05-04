// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_rspmux.
// Breaks packed struct ports into scalars for easy C++ driving.
// Reqq state array: 64 entries, set via indexed scalar ports (up to 2 entries).

/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields intentionally unused
/* verilator lint_off UNOPTFLAT */  // reqq_state array assigned from two independent index paths
module shirecache_rspmux_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries = ReqqDepth,
  parameter int unsigned NumPorts   = Ports
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Bank rsp xbar ──────────────────────────────────────────
  input  logic [NumPorts-1:0]         bank_rsp_ready_i,
  input  logic [NumPorts-1:0]         bank_rsp_2_credits_i,

  // ── Source 0: rbuf read rsp ────────────────────────────────
  input  logic                        pipe_rbuf_rd_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_rbuf_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_rbuf_rsp_reqq_id_i,
  input  logic                        pipe_rbuf_rsp_l3_source_i,

  // ── Source 1: pipe data rsp ────────────────────────────────
  input  logic                        pipe_data_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_data_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_reqq_id_i,
  input  logic                        pipe_data_rsp_type_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_sbe_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_dbe_i,
  input  logic [1:0]                  pipe_data_rsp_mod_reqq_source_i,
  input  logic                        pipe_data_rsp_mod_rsp_type_i,
  input  logic [PortIdSize-1:0]       pipe_data_rsp_mod_port_id_i,

  // ── Source 2: L3 mesh rsp ──────────────────────────────────
  input  logic                        to_l3_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_l3_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_reqq_id_i,

  // ── Source 3: Sys mesh rsp ─────────────────────────────────
  input  logic                        to_sys_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_sys_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_reqq_id_i,

  // ── Source 4: reqq nodata ──────────────────────────────────
  input  logic                        reqq_rspmux_valid_nodata_i,
  input  logic [DvTransIdSize-1:0]    reqq_rspmux_trans_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_rspmux_reqq_id_i,
  input  logic                        reqq_rspmux_with_data_i,

  // ── Source 5: dataq ────────────────────────────────────────
  input  logic                        dataq_rspmux_valid_i,
  input  logic [DvTransIdSize-1:0]    dataq_rspmux_trans_id_i,
  input  logic [ReqqIdSize-1:0]       dataq_rspmux_reqq_id_i,

  // ── ESR control ────────────────────────────────────────────
  input  logic                        esr_err_rsp_enable_i,
  input  logic                        esr_err_int_sbe_enable_i,
  input  logic                        esr_err_int_dbe_enable_i,

  // ── Reqq state config (2 entries) ──────────────────────────
  input  logic [ReqqIdSize-1:0]       rs_idx0_i,
  input  logic [PortIdSize-1:0]       rs_port_id0_i,
  input  logic [4:0]                  rs_orig_opcode0_i,
  input  logic [ReqqTagIdSize-1:0]    rs_tag_id0_i,
  input  logic [ReqqSourceSize-1:0]   rs_source0_i,
  input  logic                        rs_err_detected0_i,
  input  logic                        rs_err_zero_data0_i,
  input  logic [2:0]                  rs_size0_i,
  input  logic [LineQwSize-1:0]       rs_rspmux_qwen0_i,
  input  logic [3:0]                  rs_readcoop_dest0_i,

  input  logic [ReqqIdSize-1:0]       rs_idx1_i,
  input  logic [PortIdSize-1:0]       rs_port_id1_i,

  // ── Outputs ────────────────────────────────────────────────
  output logic [NumPorts-1:0]         bank_rsp_valid_o,
  output logic [PortIdSize-1:0]       bank_rsp_port_id_o,
  output logic                        bank_rsp_wdata_o,
  output logic [1:0]                  bank_rsp_opcode_o,
  output logic [7:0]                  bank_rsp_id_o,
  output logic [3:0]                  bank_rsp_dest_o,

  output logic                        pipe_rbuf_rsp_ready_o,
  output logic                        pipe_data_rsp_ready_o,
  output logic                        to_l3_mesh_rsp_ready_o,
  output logic                        to_sys_mesh_rsp_ready_o,
  output logic                        reqq_rspmux_ready_nodata_o,
  output logic                        dataq_rspmux_ready_o,
  output logic [NumEntries-1:0]       rspmux_sent_valid_hot_o,
  output logic                        a_rspmux_rsp_is_valid_o
);

  // ── Build reqq state ────────────────────────────────────────
  /* verilator lint_off WIDTHCONCAT */  // 64-entry state array zeroed then selectively populated
  reqq_entry_state_t [NumEntries-1:0] reqq_state;
  always_comb begin
    reqq_state = '0;
  /* verilator lint_on WIDTHCONCAT */
    reqq_state[rs_idx0_i].port_id        = rs_port_id0_i;
    reqq_state[rs_idx0_i].orig_opcode    = reqq_orig_opcode_e'(rs_orig_opcode0_i);
    reqq_state[rs_idx0_i].tag_id         = rs_tag_id0_i;
    reqq_state[rs_idx0_i].source         = rs_source0_i;
    reqq_state[rs_idx0_i].err_detected   = rs_err_detected0_i;
    reqq_state[rs_idx0_i].err_zero_data  = rs_err_zero_data0_i;
    reqq_state[rs_idx0_i].size           = sc_size_t'(rs_size0_i);
    reqq_state[rs_idx0_i].rspmux_qwen   = rs_rspmux_qwen0_i;
    reqq_state[rs_idx0_i].readcoop_dest  = rs_readcoop_dest0_i;

    reqq_state[rs_idx1_i].port_id = rs_port_id1_i;
  end

  // ── Build ESR ctl ───────────────────────────────────────────
  bank_esr_ctl_t esr_ctl;
  always_comb begin
    esr_ctl = '0;
    esr_ctl.esr_sc_err_rsp_enable = esr_err_rsp_enable_i;
    esr_ctl.esr_sc_err_interrupt_enable[int'(ErrLogEccSbe)] = esr_err_int_sbe_enable_i;
    esr_ctl.esr_sc_err_interrupt_enable[int'(ErrLogEccDbe)] = esr_err_int_dbe_enable_i;
  end

  // ── Build struct inputs ─────────────────────────────────────
  rbuf_rd_rsp_t pipe_rbuf_rd_rsp_info;
  always_comb begin
    pipe_rbuf_rd_rsp_info.trans_id  = pipe_rbuf_rsp_trans_id_i;
    pipe_rbuf_rd_rsp_info.reqq_id   = pipe_rbuf_rsp_reqq_id_i;
    pipe_rbuf_rd_rsp_info.l3_source = pipe_rbuf_rsp_l3_source_i;
    pipe_rbuf_rd_rsp_info.data      = '0;
  end

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
    pipe_data_rsp_modifier_info.rsp_type    = pipe_data_rsp_type_e'(pipe_data_rsp_mod_rsp_type_i);
    pipe_data_rsp_modifier_info.port_id     = pipe_data_rsp_mod_port_id_i;
  end

  reqq_mesh_rsp_t to_l3_mesh_rsp_info;
  always_comb begin
    to_l3_mesh_rsp_info = '0;
    to_l3_mesh_rsp_info.trans_id = to_l3_mesh_rsp_trans_id_i;
    to_l3_mesh_rsp_info.reqq_id  = to_l3_mesh_rsp_reqq_id_i;
  end

  reqq_mesh_rsp_t to_sys_mesh_rsp_info;
  always_comb begin
    to_sys_mesh_rsp_info = '0;
    to_sys_mesh_rsp_info.trans_id = to_sys_mesh_rsp_trans_id_i;
    to_sys_mesh_rsp_info.reqq_id  = to_sys_mesh_rsp_reqq_id_i;
  end

  rspmux_t reqq_rspmux_info;
  always_comb begin
    reqq_rspmux_info = '0;
    reqq_rspmux_info.trans_id  = reqq_rspmux_trans_id_i;
    reqq_rspmux_info.reqq_id   = reqq_rspmux_reqq_id_i;
    reqq_rspmux_info.with_data = reqq_rspmux_with_data_i;
  end

  rspmux_t dataq_rspmux_info;
  always_comb begin
    dataq_rspmux_info = '0;
    dataq_rspmux_info.trans_id  = dataq_rspmux_trans_id_i;
    dataq_rspmux_info.reqq_id   = dataq_rspmux_reqq_id_i;
    dataq_rspmux_info.with_data = 1'b1;
  end

  // ── DUT instantiation ──────────────────────────────────────
  xbar_rsp_t bank_rsp_info;
  rspmux_sent_t rspmux_trace_sent;

  shirecache_bank_rspmux #(
    .NumEntries (NumEntries),
    .NumPorts   (NumPorts)
  ) u_dut (
    .clk_i,
    .rst_ni,

    .bank_rsp_valid_o,
    .bank_rsp_info_o                (bank_rsp_info),
    .bank_rsp_ready_i,
    .bank_rsp_2_credits_i,

    .pipe_rbuf_rd_rsp_valid_i,
    .pipe_rbuf_rd_rsp_info_i        (pipe_rbuf_rd_rsp_info),
    .pipe_rbuf_rd_rsp_ready_o       (pipe_rbuf_rsp_ready_o),

    .pipe_data_rsp_valid_i,
    .pipe_data_rsp_info_i           (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i  (pipe_data_rsp_modifier_info),
    .pipe_data_rsp_ready_o          (pipe_data_rsp_ready_o),

    .to_l3_mesh_rsp_valid_i,
    .to_l3_mesh_rsp_info_i          (to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready_o,

    .to_sys_mesh_rsp_valid_i,
    .to_sys_mesh_rsp_info_i         (to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_o,

    .reqq_rspmux_valid_nodata_i,
    .reqq_rspmux_info_i             (reqq_rspmux_info),
    .reqq_rspmux_ready_nodata_o,

    .dataq_rspmux_valid_i,
    .dataq_rspmux_info_i            (dataq_rspmux_info),
    .dataq_rspmux_ready_o,

    .rspmux_sent_valid_hot_o,
    .rspmux_trace_sent_o            (rspmux_trace_sent),
    .a_rspmux_rsp_is_valid_o,

    .reqq_state_i                   (reqq_state),
    .esr_ctl_i                      (esr_ctl)
  );

  assign bank_rsp_port_id_o = bank_rsp_info.port_id;
  assign bank_rsp_wdata_o   = bank_rsp_info.rsp_info.wdata;
  assign bank_rsp_opcode_o  = bank_rsp_info.rsp_info.opcode;
  assign bank_rsp_id_o      = bank_rsp_info.rsp_info.id;
  assign bank_rsp_dest_o    = bank_rsp_info.rsp_info.dest;

endmodule : shirecache_rspmux_tb
