// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_rspmux + shire_cache_bank_rspmux_xbar_arb
// vs new shirecache_bank_rspmux.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.
//
// The reqq_state array is 64 entries of packed struct. Only a few entries
// are set by the test; the rest are tied to zero. C++ sets specific entries
// via indexed scalar ports.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
module cosim_rspmux_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumPorts     = Ports,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Bank rsp output signals (consumed by xbar) ─────────────
  input  logic [NumPorts-1:0]         bank_rsp_ready_i,
  input  logic [NumPorts-1:0]         bank_rsp_2_credits_i,

  // ── Source 0: pipe rbuf read response ──────────────────────
  input  logic                        pipe_rbuf_rd_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_rbuf_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_rbuf_rsp_reqq_id_i,
  input  logic                        pipe_rbuf_rsp_l3_source_i,
  // 512-bit data — use VlWide<16> in C++
  input  logic [LineDataSize-1:0]     pipe_rbuf_rsp_data_i,

  // ── Source 1: pipe data RAM response ───────────────────────
  input  logic                        pipe_data_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_data_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_reqq_id_i,
  input  logic                        pipe_data_rsp_type_i,      // 0=NormalRsp, 1=VictimData
  input  logic                        pipe_data_rsp_tag_sbe_i,
  input  logic [LineDataSize-1:0]     pipe_data_rsp_data_i,
  input  logic [LineQwSize-1:0]       pipe_data_rsp_qwen_i,
  input  logic [DataRamAddrSize-1:0]  pipe_data_rsp_data_index_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_sbe_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_dbe_i,
  // Modifier fields
  input  logic [DvTransIdSize-1:0]    pipe_data_rsp_mod_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_mod_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_mod_dataq_id_i,
  input  logic [1:0]                  pipe_data_rsp_mod_addr_space_i,
  input  logic [1:0]                  pipe_data_rsp_mod_reqq_source_i,
  input  logic [1:0]                  pipe_data_rsp_mod_which_mesh_i,
  input  logic                        pipe_data_rsp_mod_disable_mesh_bypass_i,
  input  logic                        pipe_data_rsp_mod_redirect_i,
  input  logic [PortIdSize-1:0]       pipe_data_rsp_mod_port_id_i,
  input  logic                        pipe_data_rsp_mod_rsp_type_i,

  // ── Source 2: L3 mesh response ─────────────────────────────
  input  logic                        to_l3_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_l3_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_dataq_id_i,
  input  logic [LineDataSize-1:0]     to_l3_mesh_rsp_data_i,
  input  logic                        to_l3_mesh_rsp_zero_data_i,
  input  logic                        to_l3_mesh_rsp_err_i,

  // ── Source 3: Sys mesh response ────────────────────────────
  input  logic                        to_sys_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_sys_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_dataq_id_i,
  input  logic [LineDataSize-1:0]     to_sys_mesh_rsp_data_i,
  input  logic                        to_sys_mesh_rsp_zero_data_i,
  input  logic                        to_sys_mesh_rsp_err_i,

  // ── Source 4: reqq no-data response ────────────────────────
  input  logic                        reqq_rspmux_valid_nodata_i,
  input  logic [DvTransIdSize-1:0]    reqq_rspmux_trans_id_i,
  input  logic [ReqqIdSize-1:0]       reqq_rspmux_reqq_id_i,
  input  logic [LineDataSize-1:0]     reqq_rspmux_data_i,
  input  logic                        reqq_rspmux_with_data_i,
  input  logic                        reqq_rspmux_zero_data_i,
  input  logic [LineQwSize-1:0]       reqq_rspmux_qwen_i,

  // ── Source 5: dataq response ───────────────────────────────
  input  logic                        dataq_rspmux_valid_i,
  input  logic [DvTransIdSize-1:0]    dataq_rspmux_trans_id_i,
  input  logic [ReqqIdSize-1:0]       dataq_rspmux_reqq_id_i,
  input  logic [LineDataSize-1:0]     dataq_rspmux_data_i,
  input  logic                        dataq_rspmux_with_data_i,
  input  logic                        dataq_rspmux_zero_data_i,
  input  logic [LineQwSize-1:0]       dataq_rspmux_qwen_i,

  // ── ESR control (only the relevant bits) ───────────────────
  input  logic                        esr_err_rsp_enable_i,
  input  logic                        esr_err_int_sbe_enable_i,
  input  logic                        esr_err_int_dbe_enable_i,

  // ── Reqq state entries (up to 4 configurable entries) ──────
  // Entry index to configure
  input  logic [ReqqIdSize-1:0]       rs_idx0_i,
  input  logic [PortIdSize-1:0]       rs_port_id0_i,
  input  logic [4:0]                  rs_orig_opcode0_i,
  input  logic [ReqqTagIdSize-1:0]    rs_tag_id0_i,
  input  logic [ReqqSourceSize-1:0]   rs_source0_i,
  input  logic                        rs_err_detected0_i,
  input  logic                        rs_err_zero_data0_i,
  input  logic                        rs_traced0_i,
  input  logic [2:0]                  rs_size0_i,
  input  logic [LineQwSize-1:0]       rs_rspmux_qwen0_i,
  input  logic [3:0]                  rs_readcoop_dest0_i,

  input  logic [ReqqIdSize-1:0]       rs_idx1_i,
  input  logic [PortIdSize-1:0]       rs_port_id1_i,
  input  logic [4:0]                  rs_orig_opcode1_i,
  input  logic [ReqqTagIdSize-1:0]    rs_tag_id1_i,
  input  logic [ReqqSourceSize-1:0]   rs_source1_i,
  input  logic                        rs_err_detected1_i,
  input  logic                        rs_err_zero_data1_i,
  input  logic                        rs_traced1_i,
  input  logic [2:0]                  rs_size1_i,
  input  logic [LineQwSize-1:0]       rs_rspmux_qwen1_i,
  input  logic [3:0]                  rs_readcoop_dest1_i,

  // ── Outputs (original) ─────────────────────────────────────
  output logic [NumPorts-1:0]         orig_bank_rsp_valid_o,
  output logic [PortIdSize-1:0]       orig_bank_rsp_port_id_o,
  output logic [DvTransIdSize-1:0]    orig_bank_rsp_trans_id_o,
  output logic                        orig_bank_rsp_src_is_uc_o,
  output logic [7:0]                  orig_bank_rsp_id_o,
  output logic [3:0]                  orig_bank_rsp_dest_o,
  output logic                        orig_bank_rsp_wdata_o,
  output logic [1:0]                  orig_bank_rsp_opcode_o,
  output logic [2:0]                  orig_bank_rsp_size_o,
  output logic [3:0]                  orig_bank_rsp_qwen_o,

  output logic                        orig_pipe_rbuf_rsp_ready_o,
  output logic                        orig_pipe_data_rsp_ready_o,
  output logic                        orig_to_l3_mesh_rsp_ready_o,
  output logic                        orig_to_sys_mesh_rsp_ready_o,
  output logic                        orig_reqq_rspmux_ready_nodata_o,
  output logic                        orig_dataq_rspmux_ready_o,
  output logic [NumEntries-1:0]       orig_rspmux_sent_valid_hot_o,
  output logic                        orig_rspmux_trace_sent_valid_o,
  output logic [ReqqIdSize-1:0]       orig_rspmux_trace_sent_reqq_id_o,
  output logic                        orig_rspmux_trace_sent_traced_o,
  output logic                        orig_a_rspmux_rsp_is_valid_o,

  // ── Outputs (new) ──────────────────────────────────────────
  output logic [NumPorts-1:0]         new_bank_rsp_valid_o,
  output logic [PortIdSize-1:0]       new_bank_rsp_port_id_o,
  output logic [DvTransIdSize-1:0]    new_bank_rsp_trans_id_o,
  output logic                        new_bank_rsp_src_is_uc_o,
  output logic [7:0]                  new_bank_rsp_id_o,
  output logic [3:0]                  new_bank_rsp_dest_o,
  output logic                        new_bank_rsp_wdata_o,
  output logic [1:0]                  new_bank_rsp_opcode_o,
  output logic [2:0]                  new_bank_rsp_size_o,
  output logic [3:0]                  new_bank_rsp_qwen_o,

  output logic                        new_pipe_rbuf_rsp_ready_o,
  output logic                        new_pipe_data_rsp_ready_o,
  output logic                        new_to_l3_mesh_rsp_ready_o,
  output logic                        new_to_sys_mesh_rsp_ready_o,
  output logic                        new_reqq_rspmux_ready_nodata_o,
  output logic                        new_dataq_rspmux_ready_o,
  output logic [NumEntries-1:0]       new_rspmux_sent_valid_hot_o,
  output logic                        new_rspmux_trace_sent_valid_o,
  output logic [ReqqIdSize-1:0]       new_rspmux_trace_sent_reqq_id_o,
  output logic                        new_rspmux_trace_sent_traced_o,
  output logic                        new_a_rspmux_rsp_is_valid_o
);

  // ════════════════════════════════════════════════════════════
  // Build reqq_state array from scalar inputs
  // ════════════════════════════════════════════════════════════

  reqq_entry_state_t [NumEntries-1:0] reqq_state;
  sc_reqq_entry_state_t [NumEntries-1:0] orig_reqq_state;

  // Default all entries to zero, then override specific entries
  always_comb begin
    reqq_state = '0;
    orig_reqq_state = '0;

    // Entry 0
    reqq_state[rs_idx0_i].port_id        = rs_port_id0_i;
    reqq_state[rs_idx0_i].orig_opcode    = reqq_orig_opcode_e'(rs_orig_opcode0_i);
    reqq_state[rs_idx0_i].tag_id         = rs_tag_id0_i;
    reqq_state[rs_idx0_i].source         = rs_source0_i;
    reqq_state[rs_idx0_i].err_detected   = rs_err_detected0_i;
    reqq_state[rs_idx0_i].err_zero_data  = rs_err_zero_data0_i;
    reqq_state[rs_idx0_i].traced         = rs_traced0_i;
    reqq_state[rs_idx0_i].size           = sc_size_t'(rs_size0_i);
    reqq_state[rs_idx0_i].rspmux_qwen   = rs_rspmux_qwen0_i;
    reqq_state[rs_idx0_i].readcoop_dest  = rs_readcoop_dest0_i;

    orig_reqq_state[rs_idx0_i].port_id        = rs_port_id0_i;
    orig_reqq_state[rs_idx0_i].orig_opcode    = sc_reqq_orig_opcode_t'(rs_orig_opcode0_i);
    orig_reqq_state[rs_idx0_i].tag_id         = rs_tag_id0_i;
    orig_reqq_state[rs_idx0_i].source         = rs_source0_i;
    orig_reqq_state[rs_idx0_i].err_detected   = rs_err_detected0_i;
    orig_reqq_state[rs_idx0_i].err_zero_data  = rs_err_zero_data0_i;
    orig_reqq_state[rs_idx0_i].traced         = rs_traced0_i;
    orig_reqq_state[rs_idx0_i].size           = et_link_size_t'(rs_size0_i);
    orig_reqq_state[rs_idx0_i].rspmux_qwen   = rs_rspmux_qwen0_i;
    orig_reqq_state[rs_idx0_i].readcoop_dest  = rs_readcoop_dest0_i;

    // Entry 1
    reqq_state[rs_idx1_i].port_id        = rs_port_id1_i;
    reqq_state[rs_idx1_i].orig_opcode    = reqq_orig_opcode_e'(rs_orig_opcode1_i);
    reqq_state[rs_idx1_i].tag_id         = rs_tag_id1_i;
    reqq_state[rs_idx1_i].source         = rs_source1_i;
    reqq_state[rs_idx1_i].err_detected   = rs_err_detected1_i;
    reqq_state[rs_idx1_i].err_zero_data  = rs_err_zero_data1_i;
    reqq_state[rs_idx1_i].traced         = rs_traced1_i;
    reqq_state[rs_idx1_i].size           = sc_size_t'(rs_size1_i);
    reqq_state[rs_idx1_i].rspmux_qwen   = rs_rspmux_qwen1_i;
    reqq_state[rs_idx1_i].readcoop_dest  = rs_readcoop_dest1_i;

    orig_reqq_state[rs_idx1_i].port_id        = rs_port_id1_i;
    orig_reqq_state[rs_idx1_i].orig_opcode    = sc_reqq_orig_opcode_t'(rs_orig_opcode1_i);
    orig_reqq_state[rs_idx1_i].tag_id         = rs_tag_id1_i;
    orig_reqq_state[rs_idx1_i].source         = rs_source1_i;
    orig_reqq_state[rs_idx1_i].err_detected   = rs_err_detected1_i;
    orig_reqq_state[rs_idx1_i].err_zero_data  = rs_err_zero_data1_i;
    orig_reqq_state[rs_idx1_i].traced         = rs_traced1_i;
    orig_reqq_state[rs_idx1_i].size           = et_link_size_t'(rs_size1_i);
    orig_reqq_state[rs_idx1_i].rspmux_qwen   = rs_rspmux_qwen1_i;
    orig_reqq_state[rs_idx1_i].readcoop_dest  = rs_readcoop_dest1_i;
  end

  // ════════════════════════════════════════════════════════════
  // Build ESR control struct
  // ════════════════════════════════════════════════════════════

  bank_esr_ctl_t esr_ctl;
  sc_bank_esr_ctl_t orig_esr_ctl;

  always_comb begin
    esr_ctl = '0;
    esr_ctl.esr_sc_err_rsp_enable = esr_err_rsp_enable_i;
    esr_ctl.esr_sc_err_interrupt_enable[ErrLogEccSbe] = esr_err_int_sbe_enable_i;
    esr_ctl.esr_sc_err_interrupt_enable[ErrLogEccDbe] = esr_err_int_dbe_enable_i;

    orig_esr_ctl = '0;
    orig_esr_ctl.esr_sc_err_rsp_enable = esr_err_rsp_enable_i;
    orig_esr_ctl.esr_sc_err_interrupt_enable[`SC_ERR_LOG_ECC_SBE] = esr_err_int_sbe_enable_i;
    orig_esr_ctl.esr_sc_err_interrupt_enable[`SC_ERR_LOG_ECC_DBE] = esr_err_int_dbe_enable_i;
  end

  // ════════════════════════════════════════════════════════════
  // Build struct inputs from scalars
  // ════════════════════════════════════════════════════════════

  // Source 0: rbuf
  rbuf_rd_rsp_t pipe_rbuf_rd_rsp_info;
  sc_pipe_rbuf_rd_rsp_t orig_pipe_rbuf_rd_rsp_info;
  always_comb begin
    pipe_rbuf_rd_rsp_info.trans_id  = pipe_rbuf_rsp_trans_id_i;
    pipe_rbuf_rd_rsp_info.reqq_id   = pipe_rbuf_rsp_reqq_id_i;
    pipe_rbuf_rd_rsp_info.l3_source = pipe_rbuf_rsp_l3_source_i;
    pipe_rbuf_rd_rsp_info.data      = pipe_rbuf_rsp_data_i;

    orig_pipe_rbuf_rd_rsp_info.trans_id  = pipe_rbuf_rsp_trans_id_i;
    orig_pipe_rbuf_rd_rsp_info.reqq_id   = pipe_rbuf_rsp_reqq_id_i;
    orig_pipe_rbuf_rd_rsp_info.l3_source = pipe_rbuf_rsp_l3_source_i;
    orig_pipe_rbuf_rd_rsp_info.data      = pipe_rbuf_rsp_data_i;
  end

  // Source 1: pipe data rsp
  data_rsp_t pipe_data_rsp_info;
  sc_pipe_data_rsp_t orig_pipe_data_rsp_info;
  always_comb begin
    pipe_data_rsp_info.trans_id      = pipe_data_rsp_trans_id_i;
    pipe_data_rsp_info.reqq_id       = pipe_data_rsp_reqq_id_i;
    pipe_data_rsp_info.rsp_type      = pipe_data_rsp_type_e'(pipe_data_rsp_type_i);
    pipe_data_rsp_info.tag_sbe       = pipe_data_rsp_tag_sbe_i;
    pipe_data_rsp_info.data          = pipe_data_rsp_data_i;
    pipe_data_rsp_info.qwen          = pipe_data_rsp_qwen_i;
    pipe_data_rsp_info.data_index    = pipe_data_rsp_data_index_i;
    pipe_data_rsp_info.data_ram_sbe  = pipe_data_rsp_sbe_i;
    pipe_data_rsp_info.data_ram_dbe  = pipe_data_rsp_dbe_i;

    orig_pipe_data_rsp_info.trans_id      = pipe_data_rsp_trans_id_i;
    orig_pipe_data_rsp_info.reqq_id       = pipe_data_rsp_reqq_id_i;
    orig_pipe_data_rsp_info.rsp_type      = sc_pipe_data_rsp_type_t'(pipe_data_rsp_type_i);
    orig_pipe_data_rsp_info.tag_sbe       = pipe_data_rsp_tag_sbe_i;
    orig_pipe_data_rsp_info.data          = pipe_data_rsp_data_i;
    orig_pipe_data_rsp_info.qwen          = pipe_data_rsp_qwen_i;
    orig_pipe_data_rsp_info.data_index    = pipe_data_rsp_data_index_i;
    orig_pipe_data_rsp_info.data_ram_sbe  = pipe_data_rsp_sbe_i;
    orig_pipe_data_rsp_info.data_ram_dbe  = pipe_data_rsp_dbe_i;
  end

  data_rsp_modifier_t pipe_data_rsp_modifier_info;
  sc_pipe_data_rsp_modifier_t orig_pipe_data_rsp_modifier_info;
  always_comb begin
    pipe_data_rsp_modifier_info.trans_id                = pipe_data_rsp_mod_trans_id_i;
    pipe_data_rsp_modifier_info.reqq_id                 = pipe_data_rsp_mod_reqq_id_i;
    pipe_data_rsp_modifier_info.dataq_id                = pipe_data_rsp_mod_dataq_id_i;
    pipe_data_rsp_modifier_info.address_space            = address_space_e'(pipe_data_rsp_mod_addr_space_i);
    pipe_data_rsp_modifier_info.reqq_source             = reqq_source_e'(pipe_data_rsp_mod_reqq_source_i);
    pipe_data_rsp_modifier_info.which_mesh              = mesh_dest_e'(pipe_data_rsp_mod_which_mesh_i);
    pipe_data_rsp_modifier_info.disable_mesh_bypass     = pipe_data_rsp_mod_disable_mesh_bypass_i;
    pipe_data_rsp_modifier_info.redirect_normal_rsp_to_mesh = pipe_data_rsp_mod_redirect_i;
    pipe_data_rsp_modifier_info.port_id                 = pipe_data_rsp_mod_port_id_i;
    pipe_data_rsp_modifier_info.rsp_type                = pipe_data_rsp_type_e'(pipe_data_rsp_mod_rsp_type_i);

    orig_pipe_data_rsp_modifier_info.trans_id               = pipe_data_rsp_mod_trans_id_i;
    orig_pipe_data_rsp_modifier_info.reqq_id                = pipe_data_rsp_mod_reqq_id_i;
    orig_pipe_data_rsp_modifier_info.dataq_id               = pipe_data_rsp_mod_dataq_id_i;
    orig_pipe_data_rsp_modifier_info.address_space           = sc_address_space_t'(pipe_data_rsp_mod_addr_space_i);
    orig_pipe_data_rsp_modifier_info.reqq_source            = sc_reqq_source_t'(pipe_data_rsp_mod_reqq_source_i);
    orig_pipe_data_rsp_modifier_info.which_mesh             = sc_mesh_dest_t'(pipe_data_rsp_mod_which_mesh_i);
    orig_pipe_data_rsp_modifier_info.disable_mesh_bypass    = pipe_data_rsp_mod_disable_mesh_bypass_i;
    orig_pipe_data_rsp_modifier_info.redirect_normal_rsp_to_mesh = pipe_data_rsp_mod_redirect_i;
    orig_pipe_data_rsp_modifier_info.port_id                = pipe_data_rsp_mod_port_id_i;
    orig_pipe_data_rsp_modifier_info.rsp_type               = sc_pipe_data_rsp_type_t'(pipe_data_rsp_mod_rsp_type_i);
  end

  // Source 2: L3 mesh
  reqq_mesh_rsp_t to_l3_mesh_rsp_info;
  sc_reqq_mesh_rsp_t orig_to_l3_mesh_rsp_info;
  always_comb begin
    to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    to_l3_mesh_rsp_info.data      = to_l3_mesh_rsp_data_i;
    to_l3_mesh_rsp_info.zero_data = to_l3_mesh_rsp_zero_data_i;
    to_l3_mesh_rsp_info.err       = to_l3_mesh_rsp_err_i;

    orig_to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    orig_to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    orig_to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    orig_to_l3_mesh_rsp_info.data      = to_l3_mesh_rsp_data_i;
    orig_to_l3_mesh_rsp_info.zero_data = to_l3_mesh_rsp_zero_data_i;
    orig_to_l3_mesh_rsp_info.err       = to_l3_mesh_rsp_err_i;
  end

  // Source 3: Sys mesh
  reqq_mesh_rsp_t to_sys_mesh_rsp_info;
  sc_reqq_mesh_rsp_t orig_to_sys_mesh_rsp_info;
  always_comb begin
    to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    to_sys_mesh_rsp_info.data      = to_sys_mesh_rsp_data_i;
    to_sys_mesh_rsp_info.zero_data = to_sys_mesh_rsp_zero_data_i;
    to_sys_mesh_rsp_info.err       = to_sys_mesh_rsp_err_i;

    orig_to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    orig_to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    orig_to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    orig_to_sys_mesh_rsp_info.data      = to_sys_mesh_rsp_data_i;
    orig_to_sys_mesh_rsp_info.zero_data = to_sys_mesh_rsp_zero_data_i;
    orig_to_sys_mesh_rsp_info.err       = to_sys_mesh_rsp_err_i;
  end

  // Source 4: reqq nodata
  rspmux_t reqq_rspmux_info;
  sc_rspmux_t orig_reqq_rspmux_info;
  always_comb begin
    reqq_rspmux_info.trans_id  = reqq_rspmux_trans_id_i;
    reqq_rspmux_info.reqq_id   = reqq_rspmux_reqq_id_i;
    reqq_rspmux_info.data      = reqq_rspmux_data_i;
    reqq_rspmux_info.with_data = reqq_rspmux_with_data_i;
    reqq_rspmux_info.zero_data = reqq_rspmux_zero_data_i;
    reqq_rspmux_info.qwen      = reqq_rspmux_qwen_i;

    orig_reqq_rspmux_info.trans_id  = reqq_rspmux_trans_id_i;
    orig_reqq_rspmux_info.reqq_id   = reqq_rspmux_reqq_id_i;
    orig_reqq_rspmux_info.data      = reqq_rspmux_data_i;
    orig_reqq_rspmux_info.with_data = reqq_rspmux_with_data_i;
    orig_reqq_rspmux_info.zero_data = reqq_rspmux_zero_data_i;
    orig_reqq_rspmux_info.qwen      = reqq_rspmux_qwen_i;
  end

  // Source 5: dataq
  rspmux_t dataq_rspmux_info;
  sc_rspmux_t orig_dataq_rspmux_info;
  always_comb begin
    dataq_rspmux_info.trans_id  = dataq_rspmux_trans_id_i;
    dataq_rspmux_info.reqq_id   = dataq_rspmux_reqq_id_i;
    dataq_rspmux_info.data      = dataq_rspmux_data_i;
    dataq_rspmux_info.with_data = dataq_rspmux_with_data_i;
    dataq_rspmux_info.zero_data = dataq_rspmux_zero_data_i;
    dataq_rspmux_info.qwen      = dataq_rspmux_qwen_i;

    orig_dataq_rspmux_info.trans_id  = dataq_rspmux_trans_id_i;
    orig_dataq_rspmux_info.reqq_id   = dataq_rspmux_reqq_id_i;
    orig_dataq_rspmux_info.data      = dataq_rspmux_data_i;
    orig_dataq_rspmux_info.with_data = dataq_rspmux_with_data_i;
    orig_dataq_rspmux_info.zero_data = dataq_rspmux_zero_data_i;
    orig_dataq_rspmux_info.qwen      = dataq_rspmux_qwen_i;
  end

  // ════════════════════════════════════════════════════════════
  // Original module instantiation
  // ════════════════════════════════════════════════════════════

  logic [NumPorts-1:0]   orig_bank_rsp_valid;
  sc_xbar_rsp_t          orig_bank_rsp_info;
  logic                  orig_pipe_rbuf_rsp_ready;
  logic                  orig_pipe_data_rsp_ready;
  logic                  orig_to_l3_mesh_rsp_ready;
  logic                  orig_to_sys_mesh_rsp_ready;
  logic                  orig_reqq_rspmux_ready_nodata;
  logic                  orig_dataq_rspmux_ready;
  logic [NumEntries-1:0] orig_rspmux_sent_valid_hot;
  sc_rspmux_sent_t       orig_rspmux_trace_sent;
  logic                  orig_a_rspmux_rsp_is_valid;

  shire_cache_bank_rspmux #(
    .NUM_ENTRIES    (NumEntries),
    .NUM_ENTRIES_ID (NumEntriesId),
    .NUM_PORTS      (NumPorts)
  ) u_orig (
    .clock                          (clk_i),
    .reset                          (~rst_ni),

    .bank_rsp_valid                 (orig_bank_rsp_valid),
    .bank_rsp_info                  (orig_bank_rsp_info),
    .bank_rsp_ready                 (bank_rsp_ready_i),
    .bank_rsp_2_credits             (bank_rsp_2_credits_i),

    .pipe_rbuf_rd_rsp_valid         (pipe_rbuf_rd_rsp_valid_i),
    .pipe_rbuf_rd_rsp_info          (orig_pipe_rbuf_rd_rsp_info),
    .pipe_rbuf_rd_rsp_ready         (orig_pipe_rbuf_rsp_ready),

    .pipe_data_rsp_valid            (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info             (orig_pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info    (orig_pipe_data_rsp_modifier_info),
    .pipe_data_rsp_ready            (orig_pipe_data_rsp_ready),

    .to_l3_mesh_rsp_valid_rspmux_l2 (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info            (orig_to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready_rspmux_l2 (orig_to_l3_mesh_rsp_ready),

    .to_sys_mesh_rsp_valid_rspmux_l2(to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info           (orig_to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_rspmux_l2(orig_to_sys_mesh_rsp_ready),

    .reqq_rspmux_valid_nodata       (reqq_rspmux_valid_nodata_i),
    .reqq_rspmux_info               (orig_reqq_rspmux_info),
    .reqq_rspmux_ready_nodata       (orig_reqq_rspmux_ready_nodata),

    .dataq_rspmux_valid             (dataq_rspmux_valid_i),
    .dataq_rspmux_info              (orig_dataq_rspmux_info),
    .dataq_rspmux_ready             (orig_dataq_rspmux_ready),

    .rspmux_sent_valid_hot          (orig_rspmux_sent_valid_hot),
    .rspmux_trace_sent              (orig_rspmux_trace_sent),
    .a_rspmux_rsp_is_valid          (orig_a_rspmux_rsp_is_valid),

    .reqq_state                     (orig_reqq_state),
    .esr_ctl                        (orig_esr_ctl)
  );

  // ════════════════════════════════════════════════════════════
  // New module instantiation
  // ════════════════════════════════════════════════════════════

  logic [NumPorts-1:0]   new_bank_rsp_valid;
  xbar_rsp_t             new_bank_rsp_info;
  logic                  new_pipe_rbuf_rsp_ready;
  logic                  new_pipe_data_rsp_ready;
  logic                  new_to_l3_mesh_rsp_ready;
  logic                  new_to_sys_mesh_rsp_ready;
  logic                  new_reqq_rspmux_ready_nodata;
  logic                  new_dataq_rspmux_ready;
  logic [NumEntries-1:0] new_rspmux_sent_valid_hot;
  rspmux_sent_t          new_rspmux_trace_sent;
  logic                  new_a_rspmux_rsp_is_valid;

  shirecache_bank_rspmux #(
    .NumEntries (NumEntries),
    .NumPorts   (NumPorts)
  ) u_new (
    .clk_i,
    .rst_ni,

    .bank_rsp_valid_o               (new_bank_rsp_valid),
    .bank_rsp_info_o                (new_bank_rsp_info),
    .bank_rsp_ready_i               (bank_rsp_ready_i),
    .bank_rsp_2_credits_i           (bank_rsp_2_credits_i),

    .pipe_rbuf_rd_rsp_valid_i       (pipe_rbuf_rd_rsp_valid_i),
    .pipe_rbuf_rd_rsp_info_i        (pipe_rbuf_rd_rsp_info),
    .pipe_rbuf_rd_rsp_ready_o       (new_pipe_rbuf_rsp_ready),

    .pipe_data_rsp_valid_i          (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info_i           (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i  (pipe_data_rsp_modifier_info),
    .pipe_data_rsp_ready_o          (new_pipe_data_rsp_ready),

    .to_l3_mesh_rsp_valid_i         (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info_i          (to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready_o         (new_to_l3_mesh_rsp_ready),

    .to_sys_mesh_rsp_valid_i        (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info_i         (to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_o        (new_to_sys_mesh_rsp_ready),

    .reqq_rspmux_valid_nodata_i     (reqq_rspmux_valid_nodata_i),
    .reqq_rspmux_info_i             (reqq_rspmux_info),
    .reqq_rspmux_ready_nodata_o     (new_reqq_rspmux_ready_nodata),

    .dataq_rspmux_valid_i           (dataq_rspmux_valid_i),
    .dataq_rspmux_info_i            (dataq_rspmux_info),
    .dataq_rspmux_ready_o           (new_dataq_rspmux_ready),

    .rspmux_sent_valid_hot_o        (new_rspmux_sent_valid_hot),
    .rspmux_trace_sent_o            (new_rspmux_trace_sent),
    .a_rspmux_rsp_is_valid_o        (new_a_rspmux_rsp_is_valid),

    .reqq_state_i                   (reqq_state),
    .esr_ctl_i                      (esr_ctl)
  );

  // ════════════════════════════════════════════════════════════
  // Decompose outputs to scalars for C++
  // ════════════════════════════════════════════════════════════

  // Original
  assign orig_bank_rsp_valid_o           = orig_bank_rsp_valid;
  assign orig_bank_rsp_port_id_o         = orig_bank_rsp_info.port_id;
  assign orig_bank_rsp_trans_id_o        = orig_bank_rsp_info.trans_id;
  assign orig_bank_rsp_src_is_uc_o       = orig_bank_rsp_info.src_is_uc;
  assign orig_bank_rsp_id_o              = orig_bank_rsp_info.rsp_info.id;
  assign orig_bank_rsp_dest_o            = orig_bank_rsp_info.rsp_info.dest;
  assign orig_bank_rsp_wdata_o           = orig_bank_rsp_info.rsp_info.wdata;
  assign orig_bank_rsp_opcode_o          = orig_bank_rsp_info.rsp_info.opcode;
  assign orig_bank_rsp_size_o            = orig_bank_rsp_info.rsp_info.size;
  assign orig_bank_rsp_qwen_o            = orig_bank_rsp_info.rsp_info.qwen;

  assign orig_pipe_rbuf_rsp_ready_o      = orig_pipe_rbuf_rsp_ready;
  assign orig_pipe_data_rsp_ready_o      = orig_pipe_data_rsp_ready;
  assign orig_to_l3_mesh_rsp_ready_o     = orig_to_l3_mesh_rsp_ready;
  assign orig_to_sys_mesh_rsp_ready_o    = orig_to_sys_mesh_rsp_ready;
  assign orig_reqq_rspmux_ready_nodata_o = orig_reqq_rspmux_ready_nodata;
  assign orig_dataq_rspmux_ready_o       = orig_dataq_rspmux_ready;
  assign orig_rspmux_sent_valid_hot_o    = orig_rspmux_sent_valid_hot;
  assign orig_rspmux_trace_sent_valid_o  = orig_rspmux_trace_sent.valid;
  assign orig_rspmux_trace_sent_reqq_id_o = orig_rspmux_trace_sent.reqq_id;
  assign orig_rspmux_trace_sent_traced_o = orig_rspmux_trace_sent.traced;
  assign orig_a_rspmux_rsp_is_valid_o    = orig_a_rspmux_rsp_is_valid;

  // New
  assign new_bank_rsp_valid_o            = new_bank_rsp_valid;
  assign new_bank_rsp_port_id_o          = new_bank_rsp_info.port_id;
  assign new_bank_rsp_trans_id_o         = new_bank_rsp_info.trans_id;
  assign new_bank_rsp_src_is_uc_o        = new_bank_rsp_info.src_is_uc;
  assign new_bank_rsp_id_o               = new_bank_rsp_info.rsp_info.id;
  assign new_bank_rsp_dest_o             = new_bank_rsp_info.rsp_info.dest;
  assign new_bank_rsp_wdata_o            = new_bank_rsp_info.rsp_info.wdata;
  assign new_bank_rsp_opcode_o           = new_bank_rsp_info.rsp_info.opcode;
  assign new_bank_rsp_size_o             = new_bank_rsp_info.rsp_info.size;
  assign new_bank_rsp_qwen_o             = new_bank_rsp_info.rsp_info.qwen;

  assign new_pipe_rbuf_rsp_ready_o       = new_pipe_rbuf_rsp_ready;
  assign new_pipe_data_rsp_ready_o       = new_pipe_data_rsp_ready;
  assign new_to_l3_mesh_rsp_ready_o      = new_to_l3_mesh_rsp_ready;
  assign new_to_sys_mesh_rsp_ready_o     = new_to_sys_mesh_rsp_ready;
  assign new_reqq_rspmux_ready_nodata_o  = new_reqq_rspmux_ready_nodata;
  assign new_dataq_rspmux_ready_o        = new_dataq_rspmux_ready;
  assign new_rspmux_sent_valid_hot_o     = new_rspmux_sent_valid_hot;
  assign new_rspmux_trace_sent_valid_o   = new_rspmux_trace_sent.valid;
  assign new_rspmux_trace_sent_reqq_id_o = new_rspmux_trace_sent.reqq_id;
  assign new_rspmux_trace_sent_traced_o  = new_rspmux_trace_sent.traced;
  assign new_a_rspmux_rsp_is_valid_o     = new_a_rspmux_rsp_is_valid;

endmodule : cosim_rspmux_tb
