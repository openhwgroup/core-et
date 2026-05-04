// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: shire_cache_bank_mesh vs shirecache_bank_mesh.
// Uses scalar inputs to avoid type-system conflicts between original and new.
// Instantiates both ToL3=1 and ToL3=0 configs.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module cosim_bank_mesh_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries = ReqqDepth
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,

  // ── Shared control ─────────────────────────────────────────
  input  logic                              mesh_master_bank_clk_en_i,
  input  logic [BankIdSize-1:0]             my_bank_id_i,

  // ── Reqq request ───────────────────────────────────────────
  input  logic                              reqq_valid_i,
  input  logic [DvTransIdSize-1:0]          reqq_trans_id_i,
  input  logic [ReqqIdSize-1:0]             reqq_reqq_id_i,
  input  logic [ReqqIdSize-1:0]             reqq_dataq_id_i,
  input  logic [LineDataSize-1:0]           reqq_data_i,
  input  logic                              reqq_with_data_i,
  input  logic                              reqq_rd_ben_i,
  input  logic [LineQwSize-1:0]             reqq_qwen_i,
  input  logic [LineByteSize-1:0]           reqq_ben_i,

  // ── Dataq request ──────────────────────────────────────────
  input  logic                              dataq_valid_i,
  input  logic [DvTransIdSize-1:0]          dataq_trans_id_i,
  input  logic [ReqqIdSize-1:0]             dataq_reqq_id_i,
  input  logic [ReqqIdSize-1:0]             dataq_dataq_id_i,
  input  logic [LineDataSize-1:0]           dataq_data_i,
  input  logic                              dataq_with_data_i,
  input  logic                              dataq_rd_ben_i,
  input  logic [LineQwSize-1:0]             dataq_qwen_i,
  input  logic [LineByteSize-1:0]           dataq_ben_i,

  // ── Pipe victim ────────────────────────────────────────────
  input  logic                              pipe_valid_i,
  input  logic [DvTransIdSize-1:0]          pipe_trans_id_i,
  input  logic [ReqqIdSize-1:0]             pipe_reqq_id_i,
  input  logic                              pipe_rsp_type_i,
  input  logic [LineDataSize-1:0]           pipe_data_i,
  input  logic [ReqqIdSize-1:0]             pipe_mod_dataq_id_i,
  input  logic                              pipe_mod_which_mesh_i,
  input  logic                              pipe_mod_disable_bypass_i,
  input  logic                              pipe_mod_redirect_i,

  // ── Mesh req/rsp ───────────────────────────────────────────
  input  logic                              mesh_req_ready_i,
  input  logic                              mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]          mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]             mesh_rsp_id_i,
  input  logic                              mesh_rsp_err_i,
  input  logic                              mesh_rsp_wdata_i,
  input  logic [LineDataSize-1:0]           mesh_rsp_data_i,

  // ── Response readies ───────────────────────────────────────
  input  logic                              rsp_ready_reqq_i,
  input  logic                              rsp_ready_dataq_i,
  input  logic                              rsp_ready_rspmux_l2_i,
  input  logic                              rsp_ready_l3_slave_i,

  // ── Reqq state (2 entries) ─────────────────────────────────
  input  logic [ReqqIdSize-1:0]             rs_idx0_i,
  input  logic [5:0]                        rs_opcode0_i,
  input  logic [4:0]                        rs_orig_opcode0_i,
  input  logic [1:0]                        rs_reqq_source0_i,
  input  logic [2:0]                        rs_address_space0_i,
  input  logic [2:0]                        rs_size0_i,
  input  logic [3:0]                        rs_qos0_i,
  input  logic [LineQwSize-1:0]             rs_qwen0_i,
  input  logic [PaSize-1:0]                 rs_address0_i,
  input  logic [ReqqIdSize-1:0]             rs_dataq_id0_i,
  input  logic [DvTransIdSize-1:0]          rs_trans_id0_i,
  input  logic [ReqqIdSize-1:0]             rs_idx1_i,
  input  logic [5:0]                        rs_opcode1_i,
  input  logic [4:0]                        rs_orig_opcode1_i,
  input  logic [1:0]                        rs_reqq_source1_i,
  input  logic [2:0]                        rs_address_space1_i,
  input  logic [2:0]                        rs_size1_i,
  input  logic [3:0]                        rs_qos1_i,
  input  logic [LineQwSize-1:0]             rs_qwen1_i,
  input  logic [PaSize-1:0]                 rs_address1_i,
  input  logic [ReqqIdSize-1:0]             rs_dataq_id1_i,
  input  logic [DvTransIdSize-1:0]          rs_trans_id1_i,

  // ── ESR ────────────────────────────────────────────────────
  input  logic                              esr_l2_bypass_i,
  input  logic                              esr_l3_bypass_i,
  input  logic                              esr_remote_l3_enable_i,

  // ══════════════════════════════════════════════════════════
  // Outputs — scalar comparisons
  // ══════════════════════════════════════════════════════════
  // ToL3: new
  output logic                              l3_new_reqq_ready_o,
  output logic                              l3_new_dataq_ready_o,
  output logic                              l3_new_bypassed_victim_o,
  output logic                              l3_new_req_valid_o,
  output logic [4:0]                        l3_new_req_opcode_o,
  output logic [PaSize-1:0]                 l3_new_req_address_o,
  output logic [ReqqIdSize-1:0]             l3_new_req_id_o,
  output logic [MeshMasterSourceSize-1:0]   l3_new_req_source_o,
  output logic [AxiAxqosSize-1:0]           l3_new_req_qos_o,
  output logic                              l3_new_req_wdata_o,
  output logic [LineQwSize-1:0]             l3_new_req_qwen_o,
  output logic [LineDataSize-1:0]           l3_new_req_data_o,
  output logic                              l3_new_req_ben_en_o,
  output logic [LineByteSize-1:0]           l3_new_req_ben_o,
  output logic [2:0]                        l3_new_req_size_o,
  output logic                              l3_new_rsp_ready_o,
  output logic                              l3_new_rsp_valid_reqq_o,
  output logic                              l3_new_rsp_valid_dataq_o,
  output logic                              l3_new_rsp_valid_rspmux_l2_o,
  output logic                              l3_new_rsp_valid_l3_slave_o,
  output logic [ReqqIdSize-1:0]             l3_new_rsp_reqq_id_o,
  output logic [LineDataSize-1:0]           l3_new_rsp_data_o,
  output logic                              l3_new_rsp_err_o,
  // ToL3: orig
  output logic                              l3_orig_reqq_ready_o,
  output logic                              l3_orig_dataq_ready_o,
  output logic                              l3_orig_bypassed_victim_o,
  output logic                              l3_orig_req_valid_o,
  output logic [4:0]                        l3_orig_req_opcode_o,
  output logic [PaSize-1:0]                 l3_orig_req_address_o,
  output logic [ReqqIdSize-1:0]             l3_orig_req_id_o,
  output logic [MeshMasterSourceSize-1:0]   l3_orig_req_source_o,
  output logic [AxiAxqosSize-1:0]           l3_orig_req_qos_o,
  output logic                              l3_orig_req_wdata_o,
  output logic [LineQwSize-1:0]             l3_orig_req_qwen_o,
  output logic [LineDataSize-1:0]           l3_orig_req_data_o,
  output logic                              l3_orig_req_ben_en_o,
  output logic [LineByteSize-1:0]           l3_orig_req_ben_o,
  output logic [2:0]                        l3_orig_req_size_o,
  output logic                              l3_orig_rsp_ready_o,
  output logic                              l3_orig_rsp_valid_reqq_o,
  output logic                              l3_orig_rsp_valid_dataq_o,
  output logic                              l3_orig_rsp_valid_rspmux_l2_o,
  output logic                              l3_orig_rsp_valid_l3_slave_o,
  output logic [ReqqIdSize-1:0]             l3_orig_rsp_reqq_id_o,
  output logic [LineDataSize-1:0]           l3_orig_rsp_data_o,
  output logic                              l3_orig_rsp_err_o,
  // ToSys: new
  output logic                              sys_new_reqq_ready_o,
  output logic                              sys_new_dataq_ready_o,
  output logic                              sys_new_bypassed_victim_o,
  output logic                              sys_new_req_valid_o,
  output logic                              sys_new_rsp_ready_o,
  output logic                              sys_new_rsp_valid_reqq_o,
  output logic                              sys_new_rsp_valid_dataq_o,
  output logic                              sys_new_rsp_valid_rspmux_l2_o,
  output logic                              sys_new_rsp_valid_l3_slave_o,
  // ToSys: orig
  output logic                              sys_orig_reqq_ready_o,
  output logic                              sys_orig_dataq_ready_o,
  output logic                              sys_orig_bypassed_victim_o,
  output logic                              sys_orig_req_valid_o,
  output logic                              sys_orig_rsp_ready_o,
  output logic                              sys_orig_rsp_valid_reqq_o,
  output logic                              sys_orig_rsp_valid_dataq_o,
  output logic                              sys_orig_rsp_valid_rspmux_l2_o,
  output logic                              sys_orig_rsp_valid_l3_slave_o
);

  logic reset;
  assign reset = ~rst_ni;

  // ──────────────────────────────────────────────────────────
  // Build new (shirecache_pkg) structs from scalar inputs
  // ──────────────────────────────────────────────────────────
  reqq_mesh_req_t new_reqq_info, new_dataq_info;
  assign new_reqq_info = '{trans_id: reqq_trans_id_i, reqq_id: reqq_reqq_id_i,
    dataq_id: reqq_dataq_id_i, data: reqq_data_i, with_data: reqq_with_data_i,
    rd_ben: reqq_rd_ben_i, qwen: reqq_qwen_i, ben: reqq_ben_i};
  assign new_dataq_info = '{trans_id: dataq_trans_id_i, reqq_id: dataq_reqq_id_i,
    dataq_id: dataq_dataq_id_i, data: dataq_data_i, with_data: dataq_with_data_i,
    rd_ben: dataq_rd_ben_i, qwen: dataq_qwen_i, ben: dataq_ben_i};

  data_rsp_t new_pipe_info;
  assign new_pipe_info = '{trans_id: pipe_trans_id_i, reqq_id: pipe_reqq_id_i,
    rsp_type: pipe_data_rsp_type_e'(pipe_rsp_type_i), tag_sbe: 1'b0,
    data: pipe_data_i, qwen: '0, data_index: '0, data_ram_sbe: '0, data_ram_dbe: '0};

  data_rsp_modifier_t new_pipe_mod;
  assign new_pipe_mod = '{trans_id: '0, reqq_id: '0,
    dataq_id: pipe_mod_dataq_id_i, address_space: AddressL2,
    reqq_source: ReqqSrcL2, which_mesh: mesh_dest_e'(pipe_mod_which_mesh_i),
    disable_mesh_bypass: pipe_mod_disable_bypass_i,
    redirect_normal_rsp_to_mesh: pipe_mod_redirect_i,
    port_id: '0, rsp_type: VictimData};

  mesh_master_rsp_t new_mesh_rsp;
  assign new_mesh_rsp = '{trans_id: mesh_rsp_trans_id_i, id: mesh_rsp_id_i,
    port_id: '0, source: '0, err: mesh_rsp_err_i,
    wdata: mesh_rsp_wdata_i, data: mesh_rsp_data_i};

  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_l2_bypass        = esr_l2_bypass_i;
    new_esr_ctl.esr_sc_l3_bypass        = esr_l3_bypass_i;
    new_esr_ctl.esr_sc_remote_l3_enable = esr_remote_l3_enable_i;
  end

  reqq_entry_state_t [NumEntries-1:0] new_reqq_state;
  always_comb begin
    new_reqq_state = '0;
    new_reqq_state[rs_idx0_i].opcode      = reqq_opcode_e'(rs_opcode0_i);
    new_reqq_state[rs_idx0_i].orig_opcode = reqq_orig_opcode_e'(rs_orig_opcode0_i);
    new_reqq_state[rs_idx0_i].reqq_source = reqq_source_e'(rs_reqq_source0_i);
    new_reqq_state[rs_idx0_i].address_space = address_space_e'(rs_address_space0_i);
    new_reqq_state[rs_idx0_i].size        = sc_size_t'(rs_size0_i);
    new_reqq_state[rs_idx0_i].qos         = rs_qos0_i;
    new_reqq_state[rs_idx0_i].qwen        = rs_qwen0_i;
    new_reqq_state[rs_idx0_i].address     = rs_address0_i;
    new_reqq_state[rs_idx0_i].dataq_id    = rs_dataq_id0_i;
    new_reqq_state[rs_idx0_i].trans_id    = rs_trans_id0_i;
    new_reqq_state[rs_idx1_i].opcode      = reqq_opcode_e'(rs_opcode1_i);
    new_reqq_state[rs_idx1_i].orig_opcode = reqq_orig_opcode_e'(rs_orig_opcode1_i);
    new_reqq_state[rs_idx1_i].reqq_source = reqq_source_e'(rs_reqq_source1_i);
    new_reqq_state[rs_idx1_i].address_space = address_space_e'(rs_address_space1_i);
    new_reqq_state[rs_idx1_i].size        = sc_size_t'(rs_size1_i);
    new_reqq_state[rs_idx1_i].qos         = rs_qos1_i;
    new_reqq_state[rs_idx1_i].qwen        = rs_qwen1_i;
    new_reqq_state[rs_idx1_i].address     = rs_address1_i;
    new_reqq_state[rs_idx1_i].dataq_id    = rs_dataq_id1_i;
    new_reqq_state[rs_idx1_i].trans_id    = rs_trans_id1_i;
  end

  // ──────────────────────────────────────────────────────────
  // Build original structs from same scalars
  // ──────────────────────────────────────────────────────────
  sc_reqq_mesh_req_t orig_reqq_info, orig_dataq_info;
  assign orig_reqq_info.trans_id  = reqq_trans_id_i;
  assign orig_reqq_info.reqq_id   = reqq_reqq_id_i;
  assign orig_reqq_info.dataq_id  = reqq_dataq_id_i;
  assign orig_reqq_info.data      = reqq_data_i;
  assign orig_reqq_info.with_data = reqq_with_data_i;
  assign orig_reqq_info.rd_ben    = reqq_rd_ben_i;
  assign orig_reqq_info.qwen      = reqq_qwen_i;
  assign orig_reqq_info.ben       = reqq_ben_i;
  assign orig_dataq_info.trans_id  = dataq_trans_id_i;
  assign orig_dataq_info.reqq_id   = dataq_reqq_id_i;
  assign orig_dataq_info.dataq_id  = dataq_dataq_id_i;
  assign orig_dataq_info.data      = dataq_data_i;
  assign orig_dataq_info.with_data = dataq_with_data_i;
  assign orig_dataq_info.rd_ben    = dataq_rd_ben_i;
  assign orig_dataq_info.qwen      = dataq_qwen_i;
  assign orig_dataq_info.ben       = dataq_ben_i;

  sc_pipe_data_rsp_t orig_pipe_info;
  assign orig_pipe_info.trans_id     = pipe_trans_id_i;
  assign orig_pipe_info.reqq_id      = pipe_reqq_id_i;
  assign orig_pipe_info.rsp_type     = sc_pipe_data_rsp_type_t'(pipe_rsp_type_i);
  assign orig_pipe_info.tag_sbe      = 1'b0;
  assign orig_pipe_info.data         = pipe_data_i;
  assign orig_pipe_info.qwen         = '0;
  assign orig_pipe_info.data_index   = '0;
  assign orig_pipe_info.data_ram_sbe = '0;
  assign orig_pipe_info.data_ram_dbe = '0;

  sc_pipe_data_rsp_modifier_t orig_pipe_mod;
  assign orig_pipe_mod.trans_id                    = '0;
  assign orig_pipe_mod.reqq_id                     = '0;
  assign orig_pipe_mod.dataq_id                    = pipe_mod_dataq_id_i;
  assign orig_pipe_mod.address_space               = sc_address_space_t'(AddressL2);
  assign orig_pipe_mod.reqq_source                 = sc_reqq_source_t'(ReqqSrcL2);
  assign orig_pipe_mod.which_mesh                  = sc_mesh_dest_t'(pipe_mod_which_mesh_i);
  assign orig_pipe_mod.disable_mesh_bypass         = pipe_mod_disable_bypass_i;
  assign orig_pipe_mod.redirect_normal_rsp_to_mesh = pipe_mod_redirect_i;
  assign orig_pipe_mod.port_id                     = '0;
  assign orig_pipe_mod.rsp_type                    = sc_pipe_data_rsp_type_t'(SC_VictimData);

  sc_mesh_master_rsp_t orig_mesh_rsp;
  assign orig_mesh_rsp.trans_id = mesh_rsp_trans_id_i;
  assign orig_mesh_rsp.id       = mesh_rsp_id_i;
  assign orig_mesh_rsp.port_id  = '0;
  assign orig_mesh_rsp.source   = '0;
  assign orig_mesh_rsp.err      = mesh_rsp_err_i;
  assign orig_mesh_rsp.wdata    = mesh_rsp_wdata_i;
  assign orig_mesh_rsp.data     = mesh_rsp_data_i;

  sc_bank_esr_ctl_t orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.esr_sc_l2_bypass        = esr_l2_bypass_i;
    orig_esr_ctl.esr_sc_l3_bypass        = esr_l3_bypass_i;
    orig_esr_ctl.esr_sc_remote_l3_enable = esr_remote_l3_enable_i;
  end

  sc_reqq_entry_state_t [NumEntries-1:0] orig_reqq_state;
  always_comb begin
    orig_reqq_state = '0;
    orig_reqq_state[rs_idx0_i].opcode        = sc_reqq_opcode_t'(rs_opcode0_i);
    orig_reqq_state[rs_idx0_i].orig_opcode   = sc_reqq_orig_opcode_t'(rs_orig_opcode0_i);
    orig_reqq_state[rs_idx0_i].reqq_source   = sc_reqq_source_t'(rs_reqq_source0_i);
    orig_reqq_state[rs_idx0_i].address_space  = sc_address_space_t'(rs_address_space0_i);
    orig_reqq_state[rs_idx0_i].size           = sc_size_t'(rs_size0_i);
    orig_reqq_state[rs_idx0_i].qos            = rs_qos0_i;
    orig_reqq_state[rs_idx0_i].qwen           = rs_qwen0_i;
    orig_reqq_state[rs_idx0_i].address        = rs_address0_i;
    orig_reqq_state[rs_idx0_i].dataq_id       = rs_dataq_id0_i;
    orig_reqq_state[rs_idx0_i].trans_id       = rs_trans_id0_i;
    orig_reqq_state[rs_idx1_i].opcode        = sc_reqq_opcode_t'(rs_opcode1_i);
    orig_reqq_state[rs_idx1_i].orig_opcode   = sc_reqq_orig_opcode_t'(rs_orig_opcode1_i);
    orig_reqq_state[rs_idx1_i].reqq_source   = sc_reqq_source_t'(rs_reqq_source1_i);
    orig_reqq_state[rs_idx1_i].address_space  = sc_address_space_t'(rs_address_space1_i);
    orig_reqq_state[rs_idx1_i].size           = sc_size_t'(rs_size1_i);
    orig_reqq_state[rs_idx1_i].qos            = rs_qos1_i;
    orig_reqq_state[rs_idx1_i].qwen           = rs_qwen1_i;
    orig_reqq_state[rs_idx1_i].address        = rs_address1_i;
    orig_reqq_state[rs_idx1_i].dataq_id       = rs_dataq_id1_i;
    orig_reqq_state[rs_idx1_i].trans_id       = rs_trans_id1_i;
  end

  // ══════════════════════════════════════════════════════════
  // ToL3 — New
  // ══════════════════════════════════════════════════════════
  mesh_master_req_t l3_new_req_info;
  reqq_mesh_rsp_t   l3_new_rsp_info;

  shirecache_bank_mesh #(.IsToL3(1)) u_new_l3 (
    .clk_i, .rst_ni, .dft_i('0), .mesh_master_bank_clk_en_i,
    .mesh_master_bank_req_valid_o(l3_new_req_valid_o),
    .mesh_master_bank_req_info_o(l3_new_req_info),
    .mesh_master_bank_req_ready_i(mesh_req_ready_i),
    .mesh_master_bank_rsp_valid_i(mesh_rsp_valid_i),
    .mesh_master_bank_rsp_info_i(new_mesh_rsp),
    .mesh_master_bank_rsp_ready_o(l3_new_rsp_ready_o),
    .my_bank_id_i,
    .reqq_mesh_req_valid_nodata_i(reqq_valid_i),
    .reqq_mesh_req_info_i(new_reqq_info),
    .reqq_mesh_req_ready_nodata_o(l3_new_reqq_ready_o),
    .dataq_mesh_req_valid_i(dataq_valid_i),
    .dataq_mesh_req_info_i(new_dataq_info),
    .dataq_mesh_req_ready_o(l3_new_dataq_ready_o),
    .pipe_data_rsp_valid_i(pipe_valid_i),
    .pipe_data_rsp_info_i(new_pipe_info),
    .pipe_data_rsp_modifier_info_i(new_pipe_mod),
    .mesh_bypassed_victim_o(l3_new_bypassed_victim_o),
    .mesh_rsp_info_o(l3_new_rsp_info),
    .mesh_rsp_valid_reqq_o(l3_new_rsp_valid_reqq_o),
    .mesh_rsp_ready_reqq_i(rsp_ready_reqq_i),
    .mesh_rsp_valid_dataq_o(l3_new_rsp_valid_dataq_o),
    .mesh_rsp_ready_dataq_i(rsp_ready_dataq_i),
    .mesh_rsp_valid_rspmux_l2_o(l3_new_rsp_valid_rspmux_l2_o),
    .mesh_rsp_ready_rspmux_l2_i(rsp_ready_rspmux_l2_i),
    .mesh_rsp_valid_l3_slave_o(l3_new_rsp_valid_l3_slave_o),
    .mesh_rsp_ready_l3_slave_i(rsp_ready_l3_slave_i),
    .reqq_state_i(new_reqq_state),
    .esr_ctl_i(new_esr_ctl)
  );
  assign l3_new_req_opcode_o  = l3_new_req_info.opcode;
  assign l3_new_req_address_o = l3_new_req_info.address;
  assign l3_new_req_id_o      = l3_new_req_info.id;
  assign l3_new_req_source_o  = l3_new_req_info.source;
  assign l3_new_req_qos_o     = l3_new_req_info.qos;
  assign l3_new_req_wdata_o   = l3_new_req_info.wdata;
  assign l3_new_req_qwen_o    = l3_new_req_info.qwen;
  assign l3_new_req_data_o    = l3_new_req_info.data;
  assign l3_new_req_ben_en_o  = l3_new_req_info.ben_en;
  assign l3_new_req_ben_o     = l3_new_req_info.ben;
  assign l3_new_req_size_o    = l3_new_req_info.size;
  assign l3_new_rsp_reqq_id_o = l3_new_rsp_info.reqq_id;
  assign l3_new_rsp_data_o    = l3_new_rsp_info.data;
  assign l3_new_rsp_err_o     = l3_new_rsp_info.err;

  // ══════════════════════════════════════════════════════════
  // ToL3 — Original
  // ══════════════════════════════════════════════════════════
  sc_mesh_master_req_t l3_orig_req_info;
  sc_reqq_mesh_rsp_t   l3_orig_rsp_info;

  shire_cache_bank_mesh #(.TO_L3(1)) u_orig_l3 (
    .clockf(clk_i), .reset(reset), .mesh_master_bank_clk_en(mesh_master_bank_clk_en_i),
    .mesh_master_bank_req_valid(l3_orig_req_valid_o),
    .mesh_master_bank_req_info(l3_orig_req_info),
    .mesh_master_bank_req_ready(mesh_req_ready_i),
    .mesh_master_bank_rsp_valid(mesh_rsp_valid_i),
    .mesh_master_bank_rsp_info(orig_mesh_rsp),
    .mesh_master_bank_rsp_ready(l3_orig_rsp_ready_o),
    .my_bank_id(my_bank_id_i),
    .reqq_mesh_req_valid_nodata(reqq_valid_i),
    .reqq_mesh_req_info(orig_reqq_info),
    .reqq_mesh_req_ready_nodata(l3_orig_reqq_ready_o),
    .dataq_mesh_req_valid(dataq_valid_i),
    .dataq_mesh_req_info(orig_dataq_info),
    .dataq_mesh_req_ready(l3_orig_dataq_ready_o),
    .pipe_data_rsp_valid(pipe_valid_i),
    .pipe_data_rsp_info(orig_pipe_info),
    .pipe_data_rsp_modifier_info(orig_pipe_mod),
    .mesh_bypassed_victim(l3_orig_bypassed_victim_o),
    .mesh_rsp_info(l3_orig_rsp_info),
    .mesh_rsp_valid_reqq(l3_orig_rsp_valid_reqq_o),
    .mesh_rsp_ready_reqq(rsp_ready_reqq_i),
    .mesh_rsp_valid_dataq(l3_orig_rsp_valid_dataq_o),
    .mesh_rsp_ready_dataq(rsp_ready_dataq_i),
    .mesh_rsp_valid_rspmux_l2(l3_orig_rsp_valid_rspmux_l2_o),
    .mesh_rsp_ready_rspmux_l2(rsp_ready_rspmux_l2_i),
    .mesh_rsp_valid_l3_slave(l3_orig_rsp_valid_l3_slave_o),
    .mesh_rsp_ready_l3_slave(rsp_ready_l3_slave_i),
    .reqq_state(orig_reqq_state),
    .esr_ctl(orig_esr_ctl)
  );
  assign l3_orig_req_opcode_o  = l3_orig_req_info.opcode;
  assign l3_orig_req_address_o = l3_orig_req_info.address;
  assign l3_orig_req_id_o      = l3_orig_req_info.id;
  assign l3_orig_req_source_o  = l3_orig_req_info.source;
  assign l3_orig_req_qos_o     = l3_orig_req_info.qos;
  assign l3_orig_req_wdata_o   = l3_orig_req_info.wdata;
  assign l3_orig_req_qwen_o    = l3_orig_req_info.qwen;
  assign l3_orig_req_data_o    = l3_orig_req_info.data;
  assign l3_orig_req_ben_en_o  = l3_orig_req_info.ben_en;
  assign l3_orig_req_ben_o     = l3_orig_req_info.ben;
  assign l3_orig_req_size_o    = l3_orig_req_info.size;
  assign l3_orig_rsp_reqq_id_o = l3_orig_rsp_info.reqq_id;
  assign l3_orig_rsp_data_o    = l3_orig_rsp_info.data;
  assign l3_orig_rsp_err_o     = l3_orig_rsp_info.err;

  // ══════════════════════════════════════════════════════════
  // ToSys — New
  // ══════════════════════════════════════════════════════════
  shirecache_bank_mesh #(.IsToL3(0)) u_new_sys (
    .clk_i, .rst_ni, .dft_i('0), .mesh_master_bank_clk_en_i,
    .mesh_master_bank_req_valid_o(sys_new_req_valid_o),
    .mesh_master_bank_req_info_o(),
    .mesh_master_bank_req_ready_i(mesh_req_ready_i),
    .mesh_master_bank_rsp_valid_i(mesh_rsp_valid_i),
    .mesh_master_bank_rsp_info_i(new_mesh_rsp),
    .mesh_master_bank_rsp_ready_o(sys_new_rsp_ready_o),
    .my_bank_id_i,
    .reqq_mesh_req_valid_nodata_i(reqq_valid_i),
    .reqq_mesh_req_info_i(new_reqq_info),
    .reqq_mesh_req_ready_nodata_o(sys_new_reqq_ready_o),
    .dataq_mesh_req_valid_i(dataq_valid_i),
    .dataq_mesh_req_info_i(new_dataq_info),
    .dataq_mesh_req_ready_o(sys_new_dataq_ready_o),
    .pipe_data_rsp_valid_i(pipe_valid_i),
    .pipe_data_rsp_info_i(new_pipe_info),
    .pipe_data_rsp_modifier_info_i(new_pipe_mod),
    .mesh_bypassed_victim_o(sys_new_bypassed_victim_o),
    .mesh_rsp_info_o(),
    .mesh_rsp_valid_reqq_o(sys_new_rsp_valid_reqq_o),
    .mesh_rsp_ready_reqq_i(rsp_ready_reqq_i),
    .mesh_rsp_valid_dataq_o(sys_new_rsp_valid_dataq_o),
    .mesh_rsp_ready_dataq_i(rsp_ready_dataq_i),
    .mesh_rsp_valid_rspmux_l2_o(sys_new_rsp_valid_rspmux_l2_o),
    .mesh_rsp_ready_rspmux_l2_i(rsp_ready_rspmux_l2_i),
    .mesh_rsp_valid_l3_slave_o(sys_new_rsp_valid_l3_slave_o),
    .mesh_rsp_ready_l3_slave_i(rsp_ready_l3_slave_i),
    .reqq_state_i(new_reqq_state),
    .esr_ctl_i(new_esr_ctl)
  );

  // ══════════════════════════════════════════════════════════
  // ToSys — Original
  // ══════════════════════════════════════════════════════════
  shire_cache_bank_mesh #(.TO_L3(0)) u_orig_sys (
    .clockf(clk_i), .reset(reset), .mesh_master_bank_clk_en(mesh_master_bank_clk_en_i),
    .mesh_master_bank_req_valid(sys_orig_req_valid_o),
    .mesh_master_bank_req_info(),
    .mesh_master_bank_req_ready(mesh_req_ready_i),
    .mesh_master_bank_rsp_valid(mesh_rsp_valid_i),
    .mesh_master_bank_rsp_info(orig_mesh_rsp),
    .mesh_master_bank_rsp_ready(sys_orig_rsp_ready_o),
    .my_bank_id(my_bank_id_i),
    .reqq_mesh_req_valid_nodata(reqq_valid_i),
    .reqq_mesh_req_info(orig_reqq_info),
    .reqq_mesh_req_ready_nodata(sys_orig_reqq_ready_o),
    .dataq_mesh_req_valid(dataq_valid_i),
    .dataq_mesh_req_info(orig_dataq_info),
    .dataq_mesh_req_ready(sys_orig_dataq_ready_o),
    .pipe_data_rsp_valid(pipe_valid_i),
    .pipe_data_rsp_info(orig_pipe_info),
    .pipe_data_rsp_modifier_info(orig_pipe_mod),
    .mesh_bypassed_victim(sys_orig_bypassed_victim_o),
    .mesh_rsp_info(),
    .mesh_rsp_valid_reqq(sys_orig_rsp_valid_reqq_o),
    .mesh_rsp_ready_reqq(rsp_ready_reqq_i),
    .mesh_rsp_valid_dataq(sys_orig_rsp_valid_dataq_o),
    .mesh_rsp_ready_dataq(rsp_ready_dataq_i),
    .mesh_rsp_valid_rspmux_l2(sys_orig_rsp_valid_rspmux_l2_o),
    .mesh_rsp_ready_rspmux_l2(rsp_ready_rspmux_l2_i),
    .mesh_rsp_valid_l3_slave(sys_orig_rsp_valid_l3_slave_o),
    .mesh_rsp_ready_l3_slave(rsp_ready_l3_slave_i),
    .reqq_state(orig_reqq_state),
    .esr_ctl(orig_esr_ctl)
  );

  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
