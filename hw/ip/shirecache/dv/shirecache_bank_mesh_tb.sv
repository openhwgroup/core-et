// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_mesh.
// Exposes key scalar ports for C++ driving.
// Instantiates both ToL3=1 and ToL3=0 configurations.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module shirecache_bank_mesh_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries = ReqqDepth
) (
  input  logic                          clk_i,
  input  logic                          rst_ni,

  // ── Control ────────────────────────────────────────────────
  input  logic                          mesh_master_bank_clk_en_i,
  input  logic [BankIdSize-1:0]         my_bank_id_i,

  // ── Reqq nodata request ────────────────────────────────────
  input  logic                          reqq_valid_i,
  input  logic [ReqqIdSize-1:0]         reqq_reqq_id_i,
  input  logic [ReqqIdSize-1:0]         reqq_dataq_id_i,
  input  logic                          reqq_with_data_i,
  input  logic                          reqq_rd_ben_i,

  // ── Dataq request ──────────────────────────────────────────
  input  logic                          dataq_valid_i,
  input  logic [ReqqIdSize-1:0]         dataq_reqq_id_i,
  input  logic [ReqqIdSize-1:0]         dataq_dataq_id_i,
  input  logic                          dataq_with_data_i,

  // ── Pipe victim ────────────────────────────────────────────
  input  logic                          pipe_valid_i,
  input  logic [ReqqIdSize-1:0]         pipe_reqq_id_i,
  input  logic                          pipe_mod_which_mesh_i,
  input  logic                          pipe_mod_disable_bypass_i,
  input  logic                          pipe_mod_redirect_i,
  input  logic                          pipe_rsp_type_i,
  input  logic [ReqqIdSize-1:0]         pipe_mod_dataq_id_i,

  // ── Mesh req output ready ──────────────────────────────────
  input  logic                          mesh_req_ready_i,

  // ── Mesh rsp input ─────────────────────────────────────────
  input  logic                          mesh_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]         mesh_rsp_id_i,
  input  logic                          mesh_rsp_err_i,

  // ── Response destination readies ───────────────────────────
  input  logic                          rsp_ready_reqq_i,
  input  logic                          rsp_ready_dataq_i,
  input  logic                          rsp_ready_rspmux_l2_i,
  input  logic                          rsp_ready_l3_slave_i,

  // ── ESR control bits ───────────────────────────────────────
  input  logic                          esr_l2_bypass_i,
  input  logic                          esr_l3_bypass_i,
  input  logic                          esr_remote_l3_enable_i,

  // ── Reqq state config (2 indexed entries) ──────────────────
  input  logic [ReqqIdSize-1:0]         rs_idx0_i,
  input  logic [5:0]                    rs_opcode0_i,
  input  logic [4:0]                    rs_orig_opcode0_i,
  input  logic [1:0]                    rs_reqq_source0_i,
  input  logic [2:0]                    rs_address_space0_i,
  input  logic [2:0]                    rs_size0_i,
  input  logic                          rs_qos0_i,
  input  logic [LineQwSize-1:0]         rs_qwen0_i,
  input  logic [PaSize-1:0]             rs_address0_i,
  input  logic [ReqqIdSize-1:0]         rs_dataq_id0_i,

  input  logic [ReqqIdSize-1:0]         rs_idx1_i,
  input  logic [5:0]                    rs_opcode1_i,
  input  logic [4:0]                    rs_orig_opcode1_i,
  input  logic [1:0]                    rs_reqq_source1_i,
  input  logic [2:0]                    rs_address_space1_i,
  input  logic [2:0]                    rs_size1_i,
  input  logic                          rs_qos1_i,
  input  logic [LineQwSize-1:0]         rs_qwen1_i,
  input  logic [PaSize-1:0]             rs_address1_i,
  input  logic [ReqqIdSize-1:0]         rs_dataq_id1_i,

  // ══════════════════════════════════════════════════════════
  // ToL3 instance outputs
  // ══════════════════════════════════════════════════════════
  output logic                          l3_reqq_ready_o,
  output logic                          l3_dataq_ready_o,
  output logic                          l3_bypassed_victim_o,
  output logic                          l3_req_valid_o,
  output logic [4:0]                    l3_req_opcode_o,
  output logic [PaSize-1:0]             l3_req_address_o,
  output logic [ReqqIdSize-1:0]         l3_req_id_o,
  output logic [MeshMasterSourceSize-1:0] l3_req_source_o,
  output logic [AxiAxqosSize-1:0]       l3_req_qos_o,
  output logic                          l3_req_wdata_o,
  output logic [LineQwSize-1:0]         l3_req_qwen_o,
  output logic                          l3_req_ben_en_o,
  output logic [2:0]                    l3_req_size_o,
  output logic                          l3_rsp_ready_o,
  output logic                          l3_rsp_valid_reqq_o,
  output logic                          l3_rsp_valid_dataq_o,
  output logic                          l3_rsp_valid_rspmux_l2_o,
  output logic                          l3_rsp_valid_l3_slave_o,
  output logic [ReqqIdSize-1:0]         l3_rsp_reqq_id_o,
  output logic                          l3_rsp_err_o,

  // ══════════════════════════════════════════════════════════
  // ToSys instance outputs
  // ══════════════════════════════════════════════════════════
  output logic                          sys_reqq_ready_o,
  output logic                          sys_dataq_ready_o,
  output logic                          sys_bypassed_victim_o,
  output logic                          sys_req_valid_o,
  output logic [4:0]                    sys_req_opcode_o,
  output logic [PaSize-1:0]             sys_req_address_o,
  output logic [ReqqIdSize-1:0]         sys_req_id_o,
  output logic [AxiAxqosSize-1:0]       sys_req_qos_o,
  output logic                          sys_req_wdata_o,
  output logic [2:0]                    sys_req_size_o,
  output logic                          sys_rsp_ready_o,
  output logic                          sys_rsp_valid_reqq_o,
  output logic                          sys_rsp_valid_dataq_o,
  output logic                          sys_rsp_valid_rspmux_l2_o,
  output logic                          sys_rsp_valid_l3_slave_o,
  output logic [ReqqIdSize-1:0]         sys_rsp_reqq_id_o,
  output logic                          sys_rsp_err_o
);

  // ──────────────────────────────────────────────────────────
  // Reqq state array — initialize to zero, configure via indexed ports
  // ──────────────────────────────────────────────────────────
  reqq_entry_state_t [NumEntries-1:0] reqq_state;

  always_comb begin
    reqq_state = '0;
    // Entry 0
    reqq_state[rs_idx0_i].opcode        = reqq_opcode_e'(rs_opcode0_i);
    reqq_state[rs_idx0_i].orig_opcode   = reqq_orig_opcode_e'(rs_orig_opcode0_i);
    reqq_state[rs_idx0_i].reqq_source   = reqq_source_e'(rs_reqq_source0_i);
    reqq_state[rs_idx0_i].address_space = address_space_e'(rs_address_space0_i);
    reqq_state[rs_idx0_i].size          = sc_size_t'(rs_size0_i);
    reqq_state[rs_idx0_i].qos           = {3'b0, rs_qos0_i};
    reqq_state[rs_idx0_i].qwen          = rs_qwen0_i;
    reqq_state[rs_idx0_i].address       = rs_address0_i;
    reqq_state[rs_idx0_i].dataq_id      = rs_dataq_id0_i;
    // Entry 1
    reqq_state[rs_idx1_i].opcode        = reqq_opcode_e'(rs_opcode1_i);
    reqq_state[rs_idx1_i].orig_opcode   = reqq_orig_opcode_e'(rs_orig_opcode1_i);
    reqq_state[rs_idx1_i].reqq_source   = reqq_source_e'(rs_reqq_source1_i);
    reqq_state[rs_idx1_i].address_space = address_space_e'(rs_address_space1_i);
    reqq_state[rs_idx1_i].size          = sc_size_t'(rs_size1_i);
    reqq_state[rs_idx1_i].qos           = {3'b0, rs_qos1_i};
    reqq_state[rs_idx1_i].qwen          = rs_qwen1_i;
    reqq_state[rs_idx1_i].address       = rs_address1_i;
    reqq_state[rs_idx1_i].dataq_id      = rs_dataq_id1_i;
  end

  // ──────────────────────────────────────────────────────────
  // Shared input structs
  // ──────────────────────────────────────────────────────────

  reqq_mesh_req_t reqq_req_info;
  assign reqq_req_info.trans_id  = '0;
  assign reqq_req_info.reqq_id   = reqq_reqq_id_i;
  assign reqq_req_info.dataq_id  = reqq_dataq_id_i;
  assign reqq_req_info.data      = '0;
  assign reqq_req_info.with_data = reqq_with_data_i;
  assign reqq_req_info.rd_ben    = reqq_rd_ben_i;
  assign reqq_req_info.qwen      = '0;
  assign reqq_req_info.ben       = '0;

  reqq_mesh_req_t dataq_req_info;
  assign dataq_req_info.trans_id  = '0;
  assign dataq_req_info.reqq_id   = dataq_reqq_id_i;
  assign dataq_req_info.dataq_id  = dataq_dataq_id_i;
  assign dataq_req_info.data      = '0;
  assign dataq_req_info.with_data = dataq_with_data_i;
  assign dataq_req_info.rd_ben    = 1'b0;
  assign dataq_req_info.qwen      = '0;
  assign dataq_req_info.ben       = '0;

  data_rsp_t pipe_data_rsp_info;
  assign pipe_data_rsp_info.trans_id     = '0;
  assign pipe_data_rsp_info.reqq_id      = pipe_reqq_id_i;
  assign pipe_data_rsp_info.rsp_type     = pipe_data_rsp_type_e'(pipe_rsp_type_i);
  assign pipe_data_rsp_info.tag_sbe      = 1'b0;
  assign pipe_data_rsp_info.data         = '0;
  assign pipe_data_rsp_info.qwen         = '0;
  assign pipe_data_rsp_info.data_index   = '0;
  assign pipe_data_rsp_info.data_ram_sbe = '0;
  assign pipe_data_rsp_info.data_ram_dbe = '0;

  data_rsp_modifier_t pipe_data_rsp_modifier_info;
  assign pipe_data_rsp_modifier_info.trans_id                  = '0;
  assign pipe_data_rsp_modifier_info.reqq_id                   = '0;
  assign pipe_data_rsp_modifier_info.dataq_id                  = pipe_mod_dataq_id_i;
  assign pipe_data_rsp_modifier_info.address_space             = AddressL2;
  assign pipe_data_rsp_modifier_info.reqq_source               = ReqqSrcL2;
  assign pipe_data_rsp_modifier_info.which_mesh                = mesh_dest_e'(pipe_mod_which_mesh_i);
  assign pipe_data_rsp_modifier_info.disable_mesh_bypass       = pipe_mod_disable_bypass_i;
  assign pipe_data_rsp_modifier_info.redirect_normal_rsp_to_mesh = pipe_mod_redirect_i;
  assign pipe_data_rsp_modifier_info.port_id                   = '0;
  assign pipe_data_rsp_modifier_info.rsp_type                  = VictimData;

  mesh_master_rsp_t mesh_rsp_info;
  assign mesh_rsp_info.trans_id = '0;
  assign mesh_rsp_info.id       = mesh_rsp_id_i;
  assign mesh_rsp_info.port_id  = '0;
  assign mesh_rsp_info.source   = '0;
  assign mesh_rsp_info.err      = mesh_rsp_err_i;
  assign mesh_rsp_info.wdata    = 1'b0;
  assign mesh_rsp_info.data     = '0;

  bank_esr_ctl_t esr_ctl;
  always_comb begin
    esr_ctl = '0;
    esr_ctl.esr_sc_l2_bypass        = esr_l2_bypass_i;
    esr_ctl.esr_sc_l3_bypass        = esr_l3_bypass_i;
    esr_ctl.esr_sc_remote_l3_enable = esr_remote_l3_enable_i;
  end

  // ──────────────────────────────────────────────────────────
  // ToL3 instance
  // ──────────────────────────────────────────────────────────

  mesh_master_req_t l3_req_info;
  reqq_mesh_rsp_t   l3_rsp_info;

  shirecache_bank_mesh #(.IsToL3(1)) u_l3 (
    .clk_i, .rst_ni,
    .dft_i('0),
    .mesh_master_bank_clk_en_i,
    .mesh_master_bank_req_valid_o(l3_req_valid_o),
    .mesh_master_bank_req_info_o(l3_req_info),
    .mesh_master_bank_req_ready_i(mesh_req_ready_i),
    .mesh_master_bank_rsp_valid_i(mesh_rsp_valid_i),
    .mesh_master_bank_rsp_info_i(mesh_rsp_info),
    .mesh_master_bank_rsp_ready_o(l3_rsp_ready_o),
    .my_bank_id_i,
    .reqq_mesh_req_valid_nodata_i(reqq_valid_i),
    .reqq_mesh_req_info_i(reqq_req_info),
    .reqq_mesh_req_ready_nodata_o(l3_reqq_ready_o),
    .dataq_mesh_req_valid_i(dataq_valid_i),
    .dataq_mesh_req_info_i(dataq_req_info),
    .dataq_mesh_req_ready_o(l3_dataq_ready_o),
    .pipe_data_rsp_valid_i(pipe_valid_i),
    .pipe_data_rsp_info_i(pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i(pipe_data_rsp_modifier_info),
    .mesh_bypassed_victim_o(l3_bypassed_victim_o),
    .mesh_rsp_info_o(l3_rsp_info),
    .mesh_rsp_valid_reqq_o(l3_rsp_valid_reqq_o),
    .mesh_rsp_ready_reqq_i(rsp_ready_reqq_i),
    .mesh_rsp_valid_dataq_o(l3_rsp_valid_dataq_o),
    .mesh_rsp_ready_dataq_i(rsp_ready_dataq_i),
    .mesh_rsp_valid_rspmux_l2_o(l3_rsp_valid_rspmux_l2_o),
    .mesh_rsp_ready_rspmux_l2_i(rsp_ready_rspmux_l2_i),
    .mesh_rsp_valid_l3_slave_o(l3_rsp_valid_l3_slave_o),
    .mesh_rsp_ready_l3_slave_i(rsp_ready_l3_slave_i),
    .reqq_state_i(reqq_state),
    .esr_ctl_i(esr_ctl)
  );

  assign l3_req_opcode_o  = l3_req_info.opcode;
  assign l3_req_address_o = l3_req_info.address;
  assign l3_req_id_o      = l3_req_info.id;
  assign l3_req_source_o  = l3_req_info.source;
  assign l3_req_qos_o     = l3_req_info.qos;
  assign l3_req_wdata_o   = l3_req_info.wdata;
  assign l3_req_qwen_o    = l3_req_info.qwen;
  assign l3_req_ben_en_o  = l3_req_info.ben_en;
  assign l3_req_size_o    = l3_req_info.size;
  assign l3_rsp_reqq_id_o = l3_rsp_info.reqq_id;
  assign l3_rsp_err_o     = l3_rsp_info.err;

  // ──────────────────────────────────────────────────────────
  // ToSys instance
  // ──────────────────────────────────────────────────────────

  mesh_master_req_t sys_req_info;
  reqq_mesh_rsp_t   sys_rsp_info;

  shirecache_bank_mesh #(.IsToL3(0)) u_sys (
    .clk_i, .rst_ni,
    .dft_i('0),
    .mesh_master_bank_clk_en_i,
    .mesh_master_bank_req_valid_o(sys_req_valid_o),
    .mesh_master_bank_req_info_o(sys_req_info),
    .mesh_master_bank_req_ready_i(mesh_req_ready_i),
    .mesh_master_bank_rsp_valid_i(mesh_rsp_valid_i),
    .mesh_master_bank_rsp_info_i(mesh_rsp_info),
    .mesh_master_bank_rsp_ready_o(sys_rsp_ready_o),
    .my_bank_id_i,
    .reqq_mesh_req_valid_nodata_i(reqq_valid_i),
    .reqq_mesh_req_info_i(reqq_req_info),
    .reqq_mesh_req_ready_nodata_o(sys_reqq_ready_o),
    .dataq_mesh_req_valid_i(dataq_valid_i),
    .dataq_mesh_req_info_i(dataq_req_info),
    .dataq_mesh_req_ready_o(sys_dataq_ready_o),
    .pipe_data_rsp_valid_i(pipe_valid_i),
    .pipe_data_rsp_info_i(pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i(pipe_data_rsp_modifier_info),
    .mesh_bypassed_victim_o(sys_bypassed_victim_o),
    .mesh_rsp_info_o(sys_rsp_info),
    .mesh_rsp_valid_reqq_o(sys_rsp_valid_reqq_o),
    .mesh_rsp_ready_reqq_i(rsp_ready_reqq_i),
    .mesh_rsp_valid_dataq_o(sys_rsp_valid_dataq_o),
    .mesh_rsp_ready_dataq_i(rsp_ready_dataq_i),
    .mesh_rsp_valid_rspmux_l2_o(sys_rsp_valid_rspmux_l2_o),
    .mesh_rsp_ready_rspmux_l2_i(rsp_ready_rspmux_l2_i),
    .mesh_rsp_valid_l3_slave_o(sys_rsp_valid_l3_slave_o),
    .mesh_rsp_ready_l3_slave_i(rsp_ready_l3_slave_i),
    .reqq_state_i(reqq_state),
    .esr_ctl_i(esr_ctl)
  );

  assign sys_req_opcode_o  = sys_req_info.opcode;
  assign sys_req_address_o = sys_req_info.address;
  assign sys_req_id_o      = sys_req_info.id;
  assign sys_req_qos_o     = sys_req_info.qos;
  assign sys_req_wdata_o   = sys_req_info.wdata;
  assign sys_req_size_o    = sys_req_info.size;
  assign sys_rsp_reqq_id_o = sys_rsp_info.reqq_id;
  assign sys_rsp_err_o     = sys_rsp_info.err;

  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
