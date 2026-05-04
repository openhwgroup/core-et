// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_l3_slave
// vs new shirecache_bank_l3_slave.
//
// Both modules receive identical stimulus via scalar inputs.
// Struct types are built independently for each type system
// (sc_* from soc.vh vs shirecache_pkg).

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module cosim_l3_slave_tb
  import shirecache_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // -- L3 mesh slave request (scalar) --
  input  logic                              req_valid_i,
  input  logic [DvTransIdSize-1:0]          req_trans_id_i,
  input  logic [MeshMasterAxiIdSize-1:0]    req_id_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   req_port_id_i,
  input  logic [MeshSourceBridgeIdSize-1:0] req_source_i,
  input  logic [4:0]                        req_opcode_i,
  input  logic [PaSize-1:0]                 req_address_i,
  input  logic [2:0]                        req_size_i,
  input  logic [AxiAxqosSize-1:0]           req_qos_i,
  input  logic                              req_wdata_i,
  input  logic [LineQwSize-1:0]             req_qwen_i,
  input  logic [LineDataSize-1:0]           req_data_i,
  input  logic                              req_ben_en_i,
  input  logic [LineByteSize-1:0]           req_ben_i,

  // -- L3 mesh slave rsp ready --
  input  logic                              rsp_ready_i,

  // -- Bank l3 ready (from reqq) --
  input  logic                              bank_l3_ready_i,

  // -- Source 0: pipe data rsp (scalars) --
  input  logic                              pipe_data_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]          pipe_data_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]             pipe_data_rsp_reqq_id_i,
  input  logic                              pipe_data_rsp_type_i,      // 0=NormalRsp, 1=VictimData
  input  logic                              pipe_data_rsp_tag_sbe_i,
  input  logic [LineDataSize-1:0]           pipe_data_rsp_data_i,
  input  logic [LineQwSize-1:0]             pipe_data_rsp_qwen_i,
  input  logic [DataRamAddrSize-1:0]        pipe_data_rsp_data_index_i,
  input  logic [LineDwSize-1:0]             pipe_data_rsp_sbe_i,
  input  logic [LineDwSize-1:0]             pipe_data_rsp_dbe_i,
  // Modifier
  input  logic [DvTransIdSize-1:0]          pipe_data_rsp_mod_trans_id_i,
  input  logic [ReqqIdSize-1:0]             pipe_data_rsp_mod_reqq_id_i,
  input  logic [ReqqIdSize-1:0]             pipe_data_rsp_mod_dataq_id_i,
  input  logic [2:0]                        pipe_data_rsp_mod_addr_space_i,
  input  logic [1:0]                        pipe_data_rsp_mod_reqq_source_i,
  input  logic [1:0]                        pipe_data_rsp_mod_which_mesh_i,
  input  logic                              pipe_data_rsp_mod_disable_mesh_bypass_i,
  input  logic                              pipe_data_rsp_mod_redirect_i,
  input  logic [PortIdSize-1:0]             pipe_data_rsp_mod_port_id_i,
  input  logic                              pipe_data_rsp_mod_rsp_type_i,

  // -- Source 1: to_sys mesh rsp --
  input  logic                              to_sys_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]          to_sys_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]             to_sys_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]             to_sys_rsp_dataq_id_i,
  input  logic [LineDataSize-1:0]           to_sys_rsp_data_i,
  input  logic                              to_sys_rsp_zero_data_i,
  input  logic                              to_sys_rsp_err_i,

  // -- Source 2: reqq rspmux nodata --
  input  logic                              reqq_rspmux_valid_i,
  input  logic [DvTransIdSize-1:0]          reqq_rspmux_trans_id_i,
  input  logic [ReqqIdSize-1:0]             reqq_rspmux_reqq_id_i,
  input  logic [LineDataSize-1:0]           reqq_rspmux_data_i,
  input  logic                              reqq_rspmux_with_data_i,
  input  logic                              reqq_rspmux_zero_data_i,
  input  logic [LineQwSize-1:0]             reqq_rspmux_qwen_i,

  // -- Source 3: dataq rspmux --
  input  logic                              dataq_rspmux_valid_i,
  input  logic [DvTransIdSize-1:0]          dataq_rspmux_trans_id_i,
  input  logic [ReqqIdSize-1:0]             dataq_rspmux_reqq_id_i,
  input  logic [LineDataSize-1:0]           dataq_rspmux_data_i,
  input  logic                              dataq_rspmux_with_data_i,
  input  logic                              dataq_rspmux_zero_data_i,
  input  logic [LineQwSize-1:0]             dataq_rspmux_qwen_i,

  // -- Source 4: rbuf read rsp --
  input  logic                              rbuf_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]          rbuf_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]             rbuf_rsp_reqq_id_i,
  input  logic                              rbuf_rsp_l3_source_i,
  input  logic [LineDataSize-1:0]           rbuf_rsp_data_i,

  // -- ESR control (relevant bits) --
  input  logic                              esr_err_rsp_enable_i,
  input  logic [7:0]                        esr_err_int_enable_i,

  // -- Reqq state (2 indexed entries, scalar fields) --
  input  logic [ReqqIdSize-1:0]             rs_idx0_i,
  input  logic [ReqqTagIdSize-1:0]          rs_tag_id0_i,
  input  logic [ReqqSourceSize-1:0]         rs_source0_i,
  input  logic [PortIdSize-1:0]             rs_port_id0_i,
  input  logic [4:0]                        rs_orig_opcode0_i,
  input  logic                              rs_err_detected0_i,
  input  logic                              rs_err_zero_data0_i,

  input  logic [ReqqIdSize-1:0]             rs_idx1_i,
  input  logic [ReqqTagIdSize-1:0]          rs_tag_id1_i,
  input  logic [ReqqSourceSize-1:0]         rs_source1_i,
  input  logic [PortIdSize-1:0]             rs_port_id1_i,
  input  logic [4:0]                        rs_orig_opcode1_i,
  input  logic                              rs_err_detected1_i,
  input  logic                              rs_err_zero_data1_i,

  // ============================================================
  // Outputs -- original
  // ============================================================
  output logic                              orig_req_ready_o,
  output logic                              orig_bank_l3_valid_o,
  output logic                              orig_pipe_data_rsp_ready_o,
  output logic                              orig_to_sys_rsp_ready_o,
  output logic                              orig_reqq_rspmux_ready_o,
  output logic                              orig_dataq_rspmux_ready_o,
  output logic                              orig_rbuf_rsp_ready_o,
  output logic                              orig_rsp_valid_o,
  output logic                              orig_rsp_wdata_o,
  output logic                              orig_rsp_err_o,
  output logic                              orig_rspmux_sent_valid_o,
  output logic [ReqqIdSize-1:0]             orig_rspmux_sent_id_o,
  output logic                              orig_a_rspmux_rsp_is_valid_o,

  // ============================================================
  // Outputs -- new
  // ============================================================
  output logic                              new_req_ready_o,
  output logic                              new_bank_l3_valid_o,
  output logic                              new_pipe_data_rsp_ready_o,
  output logic                              new_to_sys_rsp_ready_o,
  output logic                              new_reqq_rspmux_ready_o,
  output logic                              new_dataq_rspmux_ready_o,
  output logic                              new_rbuf_rsp_ready_o,
  output logic                              new_rsp_valid_o,
  output logic                              new_rsp_wdata_o,
  output logic                              new_rsp_err_o,
  output logic                              new_rspmux_sent_valid_o,
  output logic [ReqqIdSize-1:0]             new_rspmux_sent_id_o,
  output logic                              new_a_rspmux_rsp_is_valid_o,

  // Wide outputs for data path comparison
  output logic [PaSize-1:0]                 orig_bank_l3_address_o,
  output logic [PaSize-1:0]                 new_bank_l3_address_o,
  output logic [LineDataSize-1:0]           orig_bank_l3_data_o,
  output logic [LineDataSize-1:0]           new_bank_l3_data_o,
  output logic [LineByteSize-1:0]           orig_bank_l3_ben_o,
  output logic [LineByteSize-1:0]           new_bank_l3_ben_o,
  output logic [LineDataSize-1:0]           orig_rsp_data_o,
  output logic [LineDataSize-1:0]           new_rsp_data_o
);

  logic reset;
  assign reset = ~rst_ni;

  // ================================================================
  // Build new (shirecache_pkg) structs from scalar inputs
  // ================================================================

  // -- Request --
  mesh_slave_req_t new_req_info;
  always_comb begin
    new_req_info.trans_id = req_trans_id_i;
    new_req_info.id       = req_id_i;
    new_req_info.port_id  = req_port_id_i;
    new_req_info.source   = req_source_i;
    new_req_info.opcode   = etlink_pkg::req_opcode_e'(req_opcode_i);
    new_req_info.address  = req_address_i;
    new_req_info.size     = sc_size_t'(req_size_i);
    new_req_info.qos      = req_qos_i;
    new_req_info.wdata    = req_wdata_i;
    new_req_info.qwen     = req_qwen_i;
    new_req_info.data     = req_data_i;
    new_req_info.ben_en   = req_ben_en_i;
    new_req_info.ben      = req_ben_i;
  end

  // -- pipe data rsp --
  data_rsp_t new_pipe_data_rsp;
  always_comb begin
    new_pipe_data_rsp.trans_id     = pipe_data_rsp_trans_id_i;
    new_pipe_data_rsp.reqq_id      = pipe_data_rsp_reqq_id_i;
    new_pipe_data_rsp.rsp_type     = pipe_data_rsp_type_e'(pipe_data_rsp_type_i);
    new_pipe_data_rsp.tag_sbe      = pipe_data_rsp_tag_sbe_i;
    new_pipe_data_rsp.data         = pipe_data_rsp_data_i;
    new_pipe_data_rsp.qwen         = pipe_data_rsp_qwen_i;
    new_pipe_data_rsp.data_index   = pipe_data_rsp_data_index_i;
    new_pipe_data_rsp.data_ram_sbe = pipe_data_rsp_sbe_i;
    new_pipe_data_rsp.data_ram_dbe = pipe_data_rsp_dbe_i;
  end

  data_rsp_modifier_t new_pipe_data_rsp_mod;
  always_comb begin
    new_pipe_data_rsp_mod.trans_id                    = pipe_data_rsp_mod_trans_id_i;
    new_pipe_data_rsp_mod.reqq_id                     = pipe_data_rsp_mod_reqq_id_i;
    new_pipe_data_rsp_mod.dataq_id                    = pipe_data_rsp_mod_dataq_id_i;
    new_pipe_data_rsp_mod.address_space               = address_space_e'(pipe_data_rsp_mod_addr_space_i);
    new_pipe_data_rsp_mod.reqq_source                 = reqq_source_e'(pipe_data_rsp_mod_reqq_source_i);
    new_pipe_data_rsp_mod.which_mesh                  = mesh_dest_e'(pipe_data_rsp_mod_which_mesh_i);
    new_pipe_data_rsp_mod.disable_mesh_bypass         = pipe_data_rsp_mod_disable_mesh_bypass_i;
    new_pipe_data_rsp_mod.redirect_normal_rsp_to_mesh = pipe_data_rsp_mod_redirect_i;
    new_pipe_data_rsp_mod.port_id                     = pipe_data_rsp_mod_port_id_i;
    new_pipe_data_rsp_mod.rsp_type                    = pipe_data_rsp_type_e'(pipe_data_rsp_mod_rsp_type_i);
  end

  // -- to_sys mesh rsp --
  reqq_mesh_rsp_t new_to_sys_rsp;
  always_comb begin
    new_to_sys_rsp.trans_id  = to_sys_rsp_trans_id_i;
    new_to_sys_rsp.reqq_id   = to_sys_rsp_reqq_id_i;
    new_to_sys_rsp.dataq_id  = to_sys_rsp_dataq_id_i;
    new_to_sys_rsp.data      = to_sys_rsp_data_i;
    new_to_sys_rsp.zero_data = to_sys_rsp_zero_data_i;
    new_to_sys_rsp.err       = to_sys_rsp_err_i;
  end

  // -- reqq rspmux nodata --
  rspmux_t new_reqq_rspmux;
  always_comb begin
    new_reqq_rspmux.trans_id  = reqq_rspmux_trans_id_i;
    new_reqq_rspmux.reqq_id   = reqq_rspmux_reqq_id_i;
    new_reqq_rspmux.data      = reqq_rspmux_data_i;
    new_reqq_rspmux.with_data = reqq_rspmux_with_data_i;
    new_reqq_rspmux.zero_data = reqq_rspmux_zero_data_i;
    new_reqq_rspmux.qwen      = reqq_rspmux_qwen_i;
  end

  // -- dataq rspmux --
  rspmux_t new_dataq_rspmux;
  always_comb begin
    new_dataq_rspmux.trans_id  = dataq_rspmux_trans_id_i;
    new_dataq_rspmux.reqq_id   = dataq_rspmux_reqq_id_i;
    new_dataq_rspmux.data      = dataq_rspmux_data_i;
    new_dataq_rspmux.with_data = dataq_rspmux_with_data_i;
    new_dataq_rspmux.zero_data = dataq_rspmux_zero_data_i;
    new_dataq_rspmux.qwen      = dataq_rspmux_qwen_i;
  end

  // -- rbuf read rsp --
  rbuf_rd_rsp_t new_rbuf_rsp;
  always_comb begin
    new_rbuf_rsp.trans_id  = rbuf_rsp_trans_id_i;
    new_rbuf_rsp.reqq_id   = rbuf_rsp_reqq_id_i;
    new_rbuf_rsp.l3_source = rbuf_rsp_l3_source_i;
    new_rbuf_rsp.data      = rbuf_rsp_data_i;
  end

  // -- ESR ctl --
  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_err_rsp_enable       = esr_err_rsp_enable_i;
    new_esr_ctl.esr_sc_err_interrupt_enable  = esr_err_int_enable_i;
  end

  // -- Reqq state --
  reqq_entry_state_t [NumEntries-1:0] new_reqq_state;
  always_comb begin
    new_reqq_state = '0;
    new_reqq_state[rs_idx0_i].tag_id       = rs_tag_id0_i;
    new_reqq_state[rs_idx0_i].source       = rs_source0_i;
    new_reqq_state[rs_idx0_i].port_id      = rs_port_id0_i;
    new_reqq_state[rs_idx0_i].orig_opcode  = reqq_orig_opcode_e'(rs_orig_opcode0_i);
    new_reqq_state[rs_idx0_i].err_detected = rs_err_detected0_i;
    new_reqq_state[rs_idx0_i].err_zero_data = rs_err_zero_data0_i;

    new_reqq_state[rs_idx1_i].tag_id       = rs_tag_id1_i;
    new_reqq_state[rs_idx1_i].source       = rs_source1_i;
    new_reqq_state[rs_idx1_i].port_id      = rs_port_id1_i;
    new_reqq_state[rs_idx1_i].orig_opcode  = reqq_orig_opcode_e'(rs_orig_opcode1_i);
    new_reqq_state[rs_idx1_i].err_detected = rs_err_detected1_i;
    new_reqq_state[rs_idx1_i].err_zero_data = rs_err_zero_data1_i;
  end

  // ================================================================
  // Build original (sc_*) structs from same scalar inputs
  // ================================================================

  sc_mesh_slave_req_t orig_req_info;
  always_comb begin
    orig_req_info.trans_id = req_trans_id_i;
    orig_req_info.id       = req_id_i;
    orig_req_info.port_id  = req_port_id_i;
    orig_req_info.source   = req_source_i;
    orig_req_info.opcode   = et_link_req_opcode_t'(req_opcode_i);
    orig_req_info.address  = req_address_i;
    orig_req_info.size     = et_link_size_t'(req_size_i);
    orig_req_info.qos      = req_qos_i;
    orig_req_info.wdata    = req_wdata_i;
    orig_req_info.qwen     = req_qwen_i;
    orig_req_info.data     = req_data_i;
    orig_req_info.ben_en   = req_ben_en_i;
    orig_req_info.ben      = req_ben_i;
  end

  sc_pipe_data_rsp_t orig_pipe_data_rsp;
  always_comb begin
    orig_pipe_data_rsp.trans_id     = pipe_data_rsp_trans_id_i;
    orig_pipe_data_rsp.reqq_id      = pipe_data_rsp_reqq_id_i;
    orig_pipe_data_rsp.rsp_type     = sc_pipe_data_rsp_type_t'(pipe_data_rsp_type_i);
    orig_pipe_data_rsp.tag_sbe      = pipe_data_rsp_tag_sbe_i;
    orig_pipe_data_rsp.data         = pipe_data_rsp_data_i;
    orig_pipe_data_rsp.qwen         = pipe_data_rsp_qwen_i;
    orig_pipe_data_rsp.data_index   = pipe_data_rsp_data_index_i;
    orig_pipe_data_rsp.data_ram_sbe = pipe_data_rsp_sbe_i;
    orig_pipe_data_rsp.data_ram_dbe = pipe_data_rsp_dbe_i;
  end

  sc_pipe_data_rsp_modifier_t orig_pipe_data_rsp_mod;
  always_comb begin
    orig_pipe_data_rsp_mod.trans_id                    = pipe_data_rsp_mod_trans_id_i;
    orig_pipe_data_rsp_mod.reqq_id                     = pipe_data_rsp_mod_reqq_id_i;
    orig_pipe_data_rsp_mod.dataq_id                    = pipe_data_rsp_mod_dataq_id_i;
    orig_pipe_data_rsp_mod.address_space               = sc_address_space_t'(pipe_data_rsp_mod_addr_space_i);
    orig_pipe_data_rsp_mod.reqq_source                 = sc_reqq_source_t'(pipe_data_rsp_mod_reqq_source_i);
    orig_pipe_data_rsp_mod.which_mesh                  = sc_mesh_dest_t'(pipe_data_rsp_mod_which_mesh_i);
    orig_pipe_data_rsp_mod.disable_mesh_bypass         = pipe_data_rsp_mod_disable_mesh_bypass_i;
    orig_pipe_data_rsp_mod.redirect_normal_rsp_to_mesh = pipe_data_rsp_mod_redirect_i;
    orig_pipe_data_rsp_mod.port_id                     = pipe_data_rsp_mod_port_id_i;
    orig_pipe_data_rsp_mod.rsp_type                    = sc_pipe_data_rsp_type_t'(pipe_data_rsp_mod_rsp_type_i);
  end

  sc_reqq_mesh_rsp_t orig_to_sys_rsp;
  always_comb begin
    orig_to_sys_rsp.trans_id  = to_sys_rsp_trans_id_i;
    orig_to_sys_rsp.reqq_id   = to_sys_rsp_reqq_id_i;
    orig_to_sys_rsp.dataq_id  = to_sys_rsp_dataq_id_i;
    orig_to_sys_rsp.data      = to_sys_rsp_data_i;
    orig_to_sys_rsp.zero_data = to_sys_rsp_zero_data_i;
    orig_to_sys_rsp.err       = to_sys_rsp_err_i;
  end

  sc_rspmux_t orig_reqq_rspmux;
  always_comb begin
    orig_reqq_rspmux.trans_id  = reqq_rspmux_trans_id_i;
    orig_reqq_rspmux.reqq_id   = reqq_rspmux_reqq_id_i;
    orig_reqq_rspmux.data      = reqq_rspmux_data_i;
    orig_reqq_rspmux.with_data = reqq_rspmux_with_data_i;
    orig_reqq_rspmux.zero_data = reqq_rspmux_zero_data_i;
    orig_reqq_rspmux.qwen      = reqq_rspmux_qwen_i;
  end

  sc_rspmux_t orig_dataq_rspmux;
  always_comb begin
    orig_dataq_rspmux.trans_id  = dataq_rspmux_trans_id_i;
    orig_dataq_rspmux.reqq_id   = dataq_rspmux_reqq_id_i;
    orig_dataq_rspmux.data      = dataq_rspmux_data_i;
    orig_dataq_rspmux.with_data = dataq_rspmux_with_data_i;
    orig_dataq_rspmux.zero_data = dataq_rspmux_zero_data_i;
    orig_dataq_rspmux.qwen      = dataq_rspmux_qwen_i;
  end

  sc_pipe_rbuf_rd_rsp_t orig_rbuf_rsp;
  always_comb begin
    orig_rbuf_rsp.trans_id  = rbuf_rsp_trans_id_i;
    orig_rbuf_rsp.reqq_id   = rbuf_rsp_reqq_id_i;
    orig_rbuf_rsp.l3_source = rbuf_rsp_l3_source_i;
    orig_rbuf_rsp.data      = rbuf_rsp_data_i;
  end

  sc_bank_esr_ctl_t orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.esr_sc_err_rsp_enable       = esr_err_rsp_enable_i;
    orig_esr_ctl.esr_sc_err_interrupt_enable  = esr_err_int_enable_i;
  end

  sc_reqq_entry_state_t [NumEntries-1:0] orig_reqq_state;
  always_comb begin
    orig_reqq_state = '0;
    orig_reqq_state[rs_idx0_i].tag_id       = rs_tag_id0_i;
    orig_reqq_state[rs_idx0_i].source       = rs_source0_i;
    orig_reqq_state[rs_idx0_i].port_id      = rs_port_id0_i;
    orig_reqq_state[rs_idx0_i].orig_opcode  = sc_reqq_orig_opcode_t'(rs_orig_opcode0_i);
    orig_reqq_state[rs_idx0_i].err_detected = rs_err_detected0_i;
    orig_reqq_state[rs_idx0_i].err_zero_data = rs_err_zero_data0_i;

    orig_reqq_state[rs_idx1_i].tag_id       = rs_tag_id1_i;
    orig_reqq_state[rs_idx1_i].source       = rs_source1_i;
    orig_reqq_state[rs_idx1_i].port_id      = rs_port_id1_i;
    orig_reqq_state[rs_idx1_i].orig_opcode  = sc_reqq_orig_opcode_t'(rs_orig_opcode1_i);
    orig_reqq_state[rs_idx1_i].err_detected = rs_err_detected1_i;
    orig_reqq_state[rs_idx1_i].err_zero_data = rs_err_zero_data1_i;
  end

  // ================================================================
  // Original module instantiation
  // ================================================================

  logic                          orig_req_ready;
  logic                          orig_bank_l3_valid;
  sc_reqq_bank_req_t             orig_bank_l3_info;
  logic                          orig_rsp_valid;
  sc_mesh_slave_rsp_t            orig_rsp_info;
  logic                          orig_pipe_data_rsp_ready;
  logic                          orig_to_sys_rsp_ready;
  logic                          orig_reqq_rspmux_ready;
  logic                          orig_dataq_rspmux_ready;
  logic                          orig_rbuf_rsp_ready;
  logic                          orig_rspmux_sent_valid;
  logic [NumEntriesId-1:0]       orig_rspmux_sent_id;
  logic [NumEntries-1:0]         orig_rspmux_sent_valid_hot;
  logic                          orig_a_rspmux_rsp_is_valid;

  shire_cache_bank_l3_slave #(
    .NUM_ENTRIES    (NumEntries),
    .NUM_ENTRIES_ID (NumEntriesId)
  ) u_orig (
    .clock                            (clk_i),
    .reset                            (reset),

    .l3_mesh_slave_bank_req_valid     (req_valid_i),
    .l3_mesh_slave_bank_req_info      (orig_req_info),
    .l3_mesh_slave_bank_req_ready     (orig_req_ready),

    .l3_mesh_slave_bank_rsp_valid     (orig_rsp_valid),
    .l3_mesh_slave_bank_rsp_info      (orig_rsp_info),
    .l3_mesh_slave_bank_rsp_ready     (rsp_ready_i),

    .bank_l3_valid                    (orig_bank_l3_valid),
    .bank_l3_info                     (orig_bank_l3_info),
    .bank_l3_ready                    (bank_l3_ready_i),

    .pipe_data_rsp_valid              (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info               (orig_pipe_data_rsp),
    .pipe_data_rsp_modifier_info      (orig_pipe_data_rsp_mod),
    .pipe_data_rsp_ready              (orig_pipe_data_rsp_ready),

    .to_sys_mesh_rsp_valid_l3_slave   (to_sys_rsp_valid_i),
    .to_sys_mesh_rsp_info             (orig_to_sys_rsp),
    .to_sys_mesh_rsp_ready_l3_slave   (orig_to_sys_rsp_ready),

    .reqq_rspmux_valid_nodata         (reqq_rspmux_valid_i),
    .reqq_rspmux_info                 (orig_reqq_rspmux),
    .reqq_rspmux_ready_nodata         (orig_reqq_rspmux_ready),

    .dataq_rspmux_valid               (dataq_rspmux_valid_i),
    .dataq_rspmux_info                (orig_dataq_rspmux),
    .dataq_rspmux_ready               (orig_dataq_rspmux_ready),

    .pipe_rbuf_rd_rsp_valid           (rbuf_rsp_valid_i),
    .pipe_rbuf_rd_rsp_info            (orig_rbuf_rsp),
    .pipe_rbuf_rd_rsp_ready           (orig_rbuf_rsp_ready),

    .rspmux_sent_valid                (orig_rspmux_sent_valid),
    .rspmux_sent_id                   (orig_rspmux_sent_id),
    .rspmux_sent_valid_hot            (orig_rspmux_sent_valid_hot),
    .a_rspmux_rsp_is_valid            (orig_a_rspmux_rsp_is_valid),

    .reqq_state                       (orig_reqq_state),
    .esr_ctl                          (orig_esr_ctl)
  );

  // ================================================================
  // New module instantiation
  // ================================================================

  logic                          new_req_ready;
  logic                          new_bank_l3_valid;
  reqq_bank_req_t                new_bank_l3_info;
  logic                          new_rsp_valid;
  mesh_slave_rsp_t               new_rsp_info;
  logic                          new_pipe_data_rsp_ready;
  logic                          new_to_sys_rsp_ready;
  logic                          new_reqq_rspmux_ready;
  logic                          new_dataq_rspmux_ready;
  logic                          new_rbuf_rsp_ready;
  logic                          new_rspmux_sent_valid;
  logic [NumEntriesId-1:0]       new_rspmux_sent_id;
  logic [NumEntries-1:0]         new_rspmux_sent_valid_hot;
  logic                          new_a_rspmux_rsp_is_valid;

  shirecache_bank_l3_slave #(
    .NumEntries   (NumEntries),
    .NumEntriesId (NumEntriesId)
  ) u_new (
    .clk_i,
    .rst_ni,

    .l3_mesh_slave_bank_req_valid_i  (req_valid_i),
    .l3_mesh_slave_bank_req_info_i   (new_req_info),
    .l3_mesh_slave_bank_req_ready_o  (new_req_ready),

    .l3_mesh_slave_bank_rsp_valid_o  (new_rsp_valid),
    .l3_mesh_slave_bank_rsp_info_o   (new_rsp_info),
    .l3_mesh_slave_bank_rsp_ready_i  (rsp_ready_i),

    .bank_l3_valid_o                 (new_bank_l3_valid),
    .bank_l3_info_o                  (new_bank_l3_info),
    .bank_l3_ready_i                 (bank_l3_ready_i),

    .pipe_data_rsp_valid_i           (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info_i            (new_pipe_data_rsp),
    .pipe_data_rsp_modifier_info_i   (new_pipe_data_rsp_mod),
    .pipe_data_rsp_ready_o           (new_pipe_data_rsp_ready),

    .to_sys_mesh_rsp_valid_l3_slave_i(to_sys_rsp_valid_i),
    .to_sys_mesh_rsp_info_i          (new_to_sys_rsp),
    .to_sys_mesh_rsp_ready_l3_slave_o(new_to_sys_rsp_ready),

    .reqq_rspmux_valid_nodata_i      (reqq_rspmux_valid_i),
    .reqq_rspmux_info_i              (new_reqq_rspmux),
    .reqq_rspmux_ready_nodata_o      (new_reqq_rspmux_ready),

    .dataq_rspmux_valid_i            (dataq_rspmux_valid_i),
    .dataq_rspmux_info_i             (new_dataq_rspmux),
    .dataq_rspmux_ready_o            (new_dataq_rspmux_ready),

    .pipe_rbuf_rd_rsp_valid_i        (rbuf_rsp_valid_i),
    .pipe_rbuf_rd_rsp_info_i         (new_rbuf_rsp),
    .pipe_rbuf_rd_rsp_ready_o        (new_rbuf_rsp_ready),

    .rspmux_sent_valid_o             (new_rspmux_sent_valid),
    .rspmux_sent_id_o                (new_rspmux_sent_id),
    .rspmux_sent_valid_hot_o         (new_rspmux_sent_valid_hot),
    .a_rspmux_rsp_is_valid_o         (new_a_rspmux_rsp_is_valid),

    .reqq_state_i                    (new_reqq_state),
    .esr_ctl_i                       (new_esr_ctl)
  );

  // ================================================================
  // Map outputs to scalar ports
  // ================================================================

  assign orig_req_ready_o             = orig_req_ready;
  assign orig_bank_l3_valid_o         = orig_bank_l3_valid;
  assign orig_pipe_data_rsp_ready_o   = orig_pipe_data_rsp_ready;
  assign orig_to_sys_rsp_ready_o      = orig_to_sys_rsp_ready;
  assign orig_reqq_rspmux_ready_o     = orig_reqq_rspmux_ready;
  assign orig_dataq_rspmux_ready_o    = orig_dataq_rspmux_ready;
  assign orig_rbuf_rsp_ready_o        = orig_rbuf_rsp_ready;
  assign orig_rsp_valid_o             = orig_rsp_valid;
  assign orig_rsp_wdata_o             = orig_rsp_info.wdata;
  assign orig_rsp_err_o               = orig_rsp_info.err;
  assign orig_rspmux_sent_valid_o     = orig_rspmux_sent_valid;
  assign orig_rspmux_sent_id_o        = orig_rspmux_sent_id;
  assign orig_a_rspmux_rsp_is_valid_o = orig_a_rspmux_rsp_is_valid;

  assign new_req_ready_o              = new_req_ready;
  assign new_bank_l3_valid_o          = new_bank_l3_valid;
  assign new_pipe_data_rsp_ready_o    = new_pipe_data_rsp_ready;
  assign new_to_sys_rsp_ready_o       = new_to_sys_rsp_ready;
  assign new_reqq_rspmux_ready_o      = new_reqq_rspmux_ready;
  assign new_dataq_rspmux_ready_o     = new_dataq_rspmux_ready;
  assign new_rbuf_rsp_ready_o         = new_rbuf_rsp_ready;
  assign new_rsp_valid_o              = new_rsp_valid;
  assign new_rsp_wdata_o              = new_rsp_info.wdata;
  assign new_rsp_err_o                = new_rsp_info.err;
  assign new_rspmux_sent_valid_o      = new_rspmux_sent_valid;
  assign new_rspmux_sent_id_o         = new_rspmux_sent_id;
  assign new_a_rspmux_rsp_is_valid_o  = new_a_rspmux_rsp_is_valid;

  // Wide data-path outputs
  assign orig_bank_l3_address_o       = orig_bank_l3_info.address;
  assign new_bank_l3_address_o        = new_bank_l3_info.address;
  assign orig_bank_l3_data_o          = orig_bank_l3_info.data;
  assign new_bank_l3_data_o           = new_bank_l3_info.data;
  assign orig_bank_l3_ben_o           = orig_bank_l3_info.ben;
  assign new_bank_l3_ben_o            = new_bank_l3_info.ben;
  assign orig_rsp_data_o              = orig_rsp_info.data;
  assign new_rsp_data_o               = new_rsp_info.data;

  // ================================================================
  // Unused signals
  // ================================================================

  logic unused_ok;
  assign unused_ok = &{1'b0,
    orig_bank_l3_info,
    new_bank_l3_info,
    orig_rsp_info,
    new_rsp_info,
    orig_rspmux_sent_valid_hot,
    new_rspmux_sent_valid_hot,
    unused_ok};

endmodule
