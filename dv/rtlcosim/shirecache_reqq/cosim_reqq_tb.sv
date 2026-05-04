// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_reqq vs new
// shirecache_bank_reqq.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.
//
// Struct inputs are built from flat scalars so that C++ can drive them.
// Struct outputs are decomposed to scalars so C++ can compare field-by-field.

`include "soc.vh"
`include "macros.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
module cosim_reqq_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumPorts     = Ports,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── L2 bank request (flat scalars) ────────────────────────
  input  logic                        bank_l2_valid_i,
  input  logic [4:0]                  bank_l2_opcode_i,
  input  logic [6:0]                  bank_l2_subopcode_i,
  input  logic [PaSize-1:0]           bank_l2_address_i,
  input  logic [LineQwSize-1:0]       bank_l2_qwen_i,
  input  logic [2:0]                  bank_l2_size_i,
  input  logic [PortIdSize-1:0]       bank_l2_port_id_i,
  input  logic [ReqqSourceSize-1:0]   bank_l2_source_i,
  input  logic [DvTransIdSize-1:0]    bank_l2_trans_id_i,
  input  logic                        bank_l2_write_is_partial_i,
  input  logic [AxiAxqosSize-1:0]     bank_l2_qos_i,
  input  logic [ReqqTagIdSize-1:0]    bank_l2_id_i,
  input  logic                        bank_l2_wdata_i,
  input  logic                        dataq_bank_l2_ready_i,

  // ── L3 bank request (flat scalars) ────────────────────────
  input  logic                        bank_l3_valid_i,
  input  logic [4:0]                  bank_l3_opcode_i,
  input  logic [6:0]                  bank_l3_subopcode_i,
  input  logic [PaSize-1:0]           bank_l3_address_i,
  input  logic [LineQwSize-1:0]       bank_l3_qwen_i,
  input  logic [2:0]                  bank_l3_size_i,
  input  logic [PortIdSize-1:0]       bank_l3_port_id_i,
  input  logic [ReqqSourceSize-1:0]   bank_l3_source_i,
  input  logic [DvTransIdSize-1:0]    bank_l3_trans_id_i,
  input  logic                        bank_l3_write_is_partial_i,
  input  logic [AxiAxqosSize-1:0]     bank_l3_qos_i,
  input  logic [ReqqTagIdSize-1:0]    bank_l3_id_i,
  input  logic                        bank_l3_wdata_i,
  input  logic                        dataq_bank_l3_ready_i,

  // ── Idx CacheOp FSM ──────────────────────────────────────
  input  logic                        pipe_idx_cop_aborted_i,
  input  logic                        pipe_idx_cop_l2_busy_i,
  input  logic                        pipe_idx_cop_l3_busy_i,
  input  logic                        pipe_idx_cop_req_valid_i,
  input  logic [5:0]                  pipe_idx_cop_req_opcode_i,
  input  logic [PaSize-1:0]           pipe_idx_cop_req_address_i,
  input  logic [DvTransIdSize-1:0]    pipe_idx_cop_req_trans_id_i,
  input  logic                        pipe_idx_cop_req_sync_flush_victims_i,

  // ── Dataq write status ───────────────────────────────────
  input  logic                        dataq_write_status_any_i,
  input  logic [NumEntries-1:0]       dataq_write_status_hot_i,
  input  logic [NumEntriesId-1:0]     dataq_write_status_id_i,
  input  logic [NumEntries-1:0]       dataq_read_status_hot_i,

  // ── Pipeline busy ────────────────────────────────────────
  input  logic [SubBanks-1:0]         pipe_busy_sub_bank_i,
  input  logic                        pipe_squash_i,

  // ── Tag response (flat) ──────────────────────────────────
  input  logic                        pipe_tag_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_tag_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_tag_rsp_reqq_id_i,
  input  logic                        pipe_tag_rsp_hit_i,
  input  logic [PaSize-1:0]           pipe_tag_rsp_orig_address_i,
  input  logic                        pipe_tag_rsp_victim_i,
  input  logic                        pipe_tag_rsp_victim_silent_i,
  input  logic [PaSize-1:0]           pipe_tag_rsp_victim_address_i,
  input  logic [2:0]                  pipe_tag_rsp_victim_address_space_i,
  input  logic                        pipe_tag_rsp_victim_write_around_i,
  input  logic [LineQwSize-1:0]       pipe_tag_rsp_victim_qwen_i,
  input  logic [4:0]                  pipe_tag_rsp_err_rsp_i,
  input  logic [SetIdSize-1:0]        pipe_tag_rsp_tag_index_i,
  input  logic                        pipe_tag_rsp_bubble_i,
  input  logic [5:0]                  pipe_tag_rsp_opcode_i,
  input  logic [WayIdSize-1:0]        pipe_tag_rsp_tag_ram_hit_way_i,
  input  logic [LineQwSize-1:0]       pipe_tag_rsp_tag_ram_hit_qwen_i,
  input  logic                        pipe_tag_rsp_tag_state_ram_sbe_i,
  input  logic                        pipe_tag_rsp_tag_state_ram_dbe_i,
  input  logic [Ways-1:0]             pipe_tag_rsp_tag_ram_sbe_i,
  input  logic [Ways-1:0]             pipe_tag_rsp_tag_ram_dbe_i,

  // ── Data response (flat) ─────────────────────────────────
  input  logic                        pipe_data_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_data_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_reqq_id_i,
  input  logic                        pipe_data_rsp_rsp_type_i,
  input  logic                        pipe_data_rsp_tag_sbe_i,
  input  logic [LineQwSize-1:0]       pipe_data_rsp_qwen_i,
  input  logic [DataRamAddrSize-1:0]  pipe_data_rsp_data_index_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_data_ram_sbe_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_data_ram_dbe_i,

  // ── Data response pre (flat) ─────────────────────────────
  input  logic                        pipe_data_rsp_pre_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_data_rsp_pre_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_pre_reqq_id_i,
  input  logic                        pipe_data_rsp_pre_rsp_type_i,

  // ── Cbuf response (flat) ─────────────────────────────────
  input  logic                        pipe_cbuf_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    pipe_cbuf_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_cbuf_rsp_reqq_id_i,
  input  logic                        pipe_cbuf_rsp_victim_i,
  input  logic [PaSize-1:0]           pipe_cbuf_rsp_victim_address_i,

  // ── Bypass flags ─────────────────────────────────────────
  input  logic                        mesh_bypassed_victim_i,
  input  logic                        rspmux_bypassed_data_rsp_i,

  // ── Error logger ─────────────────────────────────────────
  input  logic [NumEntries-1:0]       set_reqq_err_detected_hot_i,
  input  logic [NumEntries-1:0]       set_reqq_err_zero_data_hot_i,

  // ── Rbuf ─────────────────────────────────────────────────
  input  logic                        pipe_rbuf_rd_req_ready_i,

  // ── Mesh ready ───────────────────────────────────────────
  input  logic                        reqq_to_l3_mesh_req_ready_nodata_i,
  input  logic                        reqq_to_l3_mesh_req_ready_wdata_i,
  input  logic                        reqq_to_sys_mesh_req_ready_nodata_i,
  input  logic                        reqq_to_sys_mesh_req_ready_wdata_i,

  // ── Mesh response (flat) ─────────────────────────────────
  input  logic                        to_l3_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_l3_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_dataq_id_i,
  input  logic                        to_l3_mesh_rsp_zero_data_i,
  input  logic                        to_l3_mesh_rsp_err_i,

  input  logic                        to_sys_mesh_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]    to_sys_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_dataq_id_i,
  input  logic                        to_sys_mesh_rsp_zero_data_i,
  input  logic                        to_sys_mesh_rsp_err_i,

  // ── Rspmux ready ─────────────────────────────────────────
  input  logic                        reqq_rspmux_l2_ready_nodata_i,
  input  logic                        reqq_rspmux_l2_ready_wdata_i,
  input  logic                        reqq_rspmux_l3_ready_nodata_i,
  input  logic                        reqq_rspmux_l3_ready_wdata_i,

  // ── Rspmux sent ──────────────────────────────────────────
  input  logic [NumEntries-1:0]       rspmux_sent_valid_hot_i,

  // ── Mbist ────────────────────────────────────────────────
  input  logic                        mbist_on_i,

  // ═══════════════════════════════════════════════════════
  // Outputs (scalar, both modules)
  // ═══════════════════════════════════════════════════════

  output logic                        new_bank_l2_ready_o,
  output logic                        orig_bank_l2_ready_o,
  output logic                        new_bank_l3_ready_o,
  output logic                        orig_bank_l3_ready_o,
  output logic                        new_pipe_start_o,
  output logic                        orig_pipe_start_o,
  output logic [ReqqIdSize-1:0]       new_pipe_req_reqq_id_o,
  output logic [ReqqIdSize-1:0]       orig_pipe_req_reqq_id_o,
  output logic                        new_pipe_idx_cop_req_ready_o,
  output logic                        orig_pipe_idx_cop_req_ready_o,
  output logic                        new_a_reqq_entry_is_valid_o,
  output logic                        orig_a_reqq_entry_is_valid_o,

  // Mesh outputs
  output logic                        new_to_l3_mesh_req_valid_nodata_o,
  output logic                        orig_to_l3_mesh_req_valid_nodata_o,
  output logic                        new_to_l3_mesh_req_valid_wdata_o,
  output logic                        orig_to_l3_mesh_req_valid_wdata_o,
  output logic                        new_to_sys_mesh_req_valid_nodata_o,
  output logic                        orig_to_sys_mesh_req_valid_nodata_o,
  output logic                        new_to_sys_mesh_req_valid_wdata_o,
  output logic                        orig_to_sys_mesh_req_valid_wdata_o,

  // Rspmux outputs
  output logic                        new_rspmux_l2_valid_nodata_o,
  output logic                        orig_rspmux_l2_valid_nodata_o,
  output logic                        new_rspmux_l2_valid_wdata_o,
  output logic                        orig_rspmux_l2_valid_wdata_o,
  output logic                        new_rspmux_l3_valid_nodata_o,
  output logic                        orig_rspmux_l3_valid_nodata_o,
  output logic                        new_rspmux_l3_valid_wdata_o,
  output logic                        orig_rspmux_l3_valid_wdata_o,

  // Rbuf outputs
  output logic                        new_pipe_rbuf_rd_req_valid_o,
  output logic                        orig_pipe_rbuf_rd_req_valid_o,

  // Alloc id
  output logic [NumEntriesId-1:0]     new_reqq_alloc_l2_id_o,
  output logic [NumEntriesId-1:0]     orig_reqq_alloc_l2_id_o,
  output logic [NumEntriesId-1:0]     new_reqq_alloc_l3_id_o,
  output logic [NumEntriesId-1:0]     orig_reqq_alloc_l3_id_o,

  // Pipe req opcode (when pipe_start)
  output logic [5:0]                  new_pipe_req_opcode_o,
  output logic [5:0]                  orig_pipe_req_opcode_o
);

  // Active-high synchronous reset for original
  wire reset = !rst_ni;

  // ═══════════════════════════════════════════════════════
  // Build structured types from scalar inputs
  // ═══════════════════════════════════════════════════════

  // ── reqq_bank_req_t (new) ──
  reqq_bank_req_t new_bank_l2_info;
  always_comb begin
    new_bank_l2_info           = '0;
    new_bank_l2_info.opcode    = etlink_pkg::req_opcode_e'(bank_l2_opcode_i);
    new_bank_l2_info.subopcode = bank_l2_subopcode_i;
    new_bank_l2_info.address   = bank_l2_address_i;
    new_bank_l2_info.qwen      = bank_l2_qwen_i;
    new_bank_l2_info.size      = sc_size_t'(bank_l2_size_i);
    new_bank_l2_info.port_id   = bank_l2_port_id_i;
    new_bank_l2_info.source    = bank_l2_source_i;
    new_bank_l2_info.trans_id  = bank_l2_trans_id_i;
    new_bank_l2_info.write_is_partial = bank_l2_write_is_partial_i;
    new_bank_l2_info.qos       = bank_l2_qos_i;
    new_bank_l2_info.id        = bank_l2_id_i;
    new_bank_l2_info.wdata     = bank_l2_wdata_i;
  end

  reqq_bank_req_t new_bank_l3_info;
  always_comb begin
    new_bank_l3_info           = '0;
    new_bank_l3_info.opcode    = etlink_pkg::req_opcode_e'(bank_l3_opcode_i);
    new_bank_l3_info.subopcode = bank_l3_subopcode_i;
    new_bank_l3_info.address   = bank_l3_address_i;
    new_bank_l3_info.qwen      = bank_l3_qwen_i;
    new_bank_l3_info.size      = sc_size_t'(bank_l3_size_i);
    new_bank_l3_info.port_id   = bank_l3_port_id_i;
    new_bank_l3_info.source    = bank_l3_source_i;
    new_bank_l3_info.trans_id  = bank_l3_trans_id_i;
    new_bank_l3_info.write_is_partial = bank_l3_write_is_partial_i;
    new_bank_l3_info.qos       = bank_l3_qos_i;
    new_bank_l3_info.id        = bank_l3_id_i;
    new_bank_l3_info.wdata     = bank_l3_wdata_i;
  end

  // ── sc_reqq_bank_req_t (original) ──
  sc_reqq_bank_req_t orig_bank_l2_info;
  always_comb begin
    orig_bank_l2_info           = '0;
    orig_bank_l2_info.opcode    = et_link_req_opcode_t'(bank_l2_opcode_i);
    orig_bank_l2_info.subopcode = bank_l2_subopcode_i;
    orig_bank_l2_info.address   = bank_l2_address_i;
    orig_bank_l2_info.qwen      = bank_l2_qwen_i;
    orig_bank_l2_info.size      = et_link_size_t'(bank_l2_size_i);
    orig_bank_l2_info.port_id   = bank_l2_port_id_i;
    orig_bank_l2_info.source    = bank_l2_source_i;
    orig_bank_l2_info.trans_id  = bank_l2_trans_id_i;
    orig_bank_l2_info.write_is_partial = bank_l2_write_is_partial_i;
    orig_bank_l2_info.qos       = bank_l2_qos_i[0];
    orig_bank_l2_info.id        = bank_l2_id_i;
    orig_bank_l2_info.wdata     = bank_l2_wdata_i;
  end

  sc_reqq_bank_req_t orig_bank_l3_info;
  always_comb begin
    orig_bank_l3_info           = '0;
    orig_bank_l3_info.opcode    = et_link_req_opcode_t'(bank_l3_opcode_i);
    orig_bank_l3_info.subopcode = bank_l3_subopcode_i;
    orig_bank_l3_info.address   = bank_l3_address_i;
    orig_bank_l3_info.qwen      = bank_l3_qwen_i;
    orig_bank_l3_info.size      = et_link_size_t'(bank_l3_size_i);
    orig_bank_l3_info.port_id   = bank_l3_port_id_i;
    orig_bank_l3_info.source    = bank_l3_source_i;
    orig_bank_l3_info.trans_id  = bank_l3_trans_id_i;
    orig_bank_l3_info.write_is_partial = bank_l3_write_is_partial_i;
    orig_bank_l3_info.qos       = bank_l3_qos_i[0];
    orig_bank_l3_info.id        = bank_l3_id_i;
    orig_bank_l3_info.wdata     = bank_l3_wdata_i;
  end

  // ── idx_cop_req_t (new) ──
  idx_cop_req_t new_idx_cop_req_info;
  always_comb begin
    new_idx_cop_req_info                       = '0;
    new_idx_cop_req_info.opcode                = reqq_opcode_e'(pipe_idx_cop_req_opcode_i);
    new_idx_cop_req_info.address               = pipe_idx_cop_req_address_i;
    new_idx_cop_req_info.trans_id              = pipe_idx_cop_req_trans_id_i;
    new_idx_cop_req_info.sync_flush_victims    = pipe_idx_cop_req_sync_flush_victims_i;
  end

  // ── sc_pipe_idx_cop_req_t (original) ──
  sc_pipe_idx_cop_req_t orig_idx_cop_req_info;
  always_comb begin
    orig_idx_cop_req_info                      = '0;
    orig_idx_cop_req_info.opcode               = sc_reqq_opcode_t'(pipe_idx_cop_req_opcode_i);
    orig_idx_cop_req_info.address              = pipe_idx_cop_req_address_i;
    orig_idx_cop_req_info.trans_id             = pipe_idx_cop_req_trans_id_i;
    orig_idx_cop_req_info.sync_flush_victims   = pipe_idx_cop_req_sync_flush_victims_i;
  end

  // ── pipe_busy_t (new) ──
  pipe_busy_t new_pipe_busy;
  always_comb begin
    new_pipe_busy = '0;
    new_pipe_busy.sub_bank_busy = pipe_busy_sub_bank_i;
  end

  // ── sc_pipe_busy_t (original) ──
  sc_pipe_busy_t orig_pipe_busy;
  always_comb begin
    orig_pipe_busy = '0;
    orig_pipe_busy.sub_bank_busy = pipe_busy_sub_bank_i;
  end

  // ── tag_rsp_t (new) ──
  tag_rsp_t new_pipe_tag_rsp_info;
  always_comb begin
    new_pipe_tag_rsp_info                        = '0;
    new_pipe_tag_rsp_info.trans_id               = pipe_tag_rsp_trans_id_i;
    new_pipe_tag_rsp_info.reqq_id                = pipe_tag_rsp_reqq_id_i;
    new_pipe_tag_rsp_info.hit                    = pipe_tag_rsp_hit_i;
    new_pipe_tag_rsp_info.orig_address           = pipe_tag_rsp_orig_address_i;
    new_pipe_tag_rsp_info.victim                 = pipe_tag_rsp_victim_i;
    new_pipe_tag_rsp_info.victim_silent          = pipe_tag_rsp_victim_silent_i;
    new_pipe_tag_rsp_info.victim_address         = pipe_tag_rsp_victim_address_i;
    new_pipe_tag_rsp_info.victim_address_space   = address_space_e'(pipe_tag_rsp_victim_address_space_i);
    new_pipe_tag_rsp_info.victim_write_around    = pipe_tag_rsp_victim_write_around_i;
    new_pipe_tag_rsp_info.victim_qwen            = pipe_tag_rsp_victim_qwen_i;
    new_pipe_tag_rsp_info.err_rsp                = err_rsp_e'(pipe_tag_rsp_err_rsp_i);
    new_pipe_tag_rsp_info.tag_index              = pipe_tag_rsp_tag_index_i;
    new_pipe_tag_rsp_info.bubble                 = pipe_tag_rsp_bubble_i;
    new_pipe_tag_rsp_info.opcode                 = reqq_opcode_e'(pipe_tag_rsp_opcode_i);
    new_pipe_tag_rsp_info.tag_ram_hit_way        = pipe_tag_rsp_tag_ram_hit_way_i;
    new_pipe_tag_rsp_info.tag_ram_hit_qwen       = pipe_tag_rsp_tag_ram_hit_qwen_i;
    new_pipe_tag_rsp_info.tag_state_ram_sbe      = pipe_tag_rsp_tag_state_ram_sbe_i;
    new_pipe_tag_rsp_info.tag_state_ram_dbe      = pipe_tag_rsp_tag_state_ram_dbe_i;
    new_pipe_tag_rsp_info.tag_ram_sbe            = pipe_tag_rsp_tag_ram_sbe_i;
    new_pipe_tag_rsp_info.tag_ram_dbe            = pipe_tag_rsp_tag_ram_dbe_i;
  end

  // ── sc_pipe_tag_rsp_t (original) ──
  sc_pipe_tag_rsp_t orig_pipe_tag_rsp_info;
  always_comb begin
    orig_pipe_tag_rsp_info                        = '0;
    orig_pipe_tag_rsp_info.trans_id               = pipe_tag_rsp_trans_id_i;
    orig_pipe_tag_rsp_info.reqq_id                = pipe_tag_rsp_reqq_id_i;
    orig_pipe_tag_rsp_info.hit                    = pipe_tag_rsp_hit_i;
    orig_pipe_tag_rsp_info.orig_address           = pipe_tag_rsp_orig_address_i;
    orig_pipe_tag_rsp_info.victim                 = pipe_tag_rsp_victim_i;
    orig_pipe_tag_rsp_info.victim_silent          = pipe_tag_rsp_victim_silent_i;
    orig_pipe_tag_rsp_info.victim_address         = pipe_tag_rsp_victim_address_i;
    orig_pipe_tag_rsp_info.victim_address_space   = sc_address_space_t'(pipe_tag_rsp_victim_address_space_i);
    orig_pipe_tag_rsp_info.victim_write_around    = pipe_tag_rsp_victim_write_around_i;
    orig_pipe_tag_rsp_info.victim_qwen            = pipe_tag_rsp_victim_qwen_i;
    orig_pipe_tag_rsp_info.err_rsp                = sc_err_rsp_t'(pipe_tag_rsp_err_rsp_i);
    orig_pipe_tag_rsp_info.tag_index              = pipe_tag_rsp_tag_index_i;
    orig_pipe_tag_rsp_info.bubble                 = pipe_tag_rsp_bubble_i;
    orig_pipe_tag_rsp_info.opcode                 = sc_reqq_opcode_t'(pipe_tag_rsp_opcode_i);
    orig_pipe_tag_rsp_info.tag_ram_hit_way        = pipe_tag_rsp_tag_ram_hit_way_i;
    orig_pipe_tag_rsp_info.tag_ram_hit_qwen       = pipe_tag_rsp_tag_ram_hit_qwen_i;
    orig_pipe_tag_rsp_info.tag_state_ram_sbe      = pipe_tag_rsp_tag_state_ram_sbe_i;
    orig_pipe_tag_rsp_info.tag_state_ram_dbe      = pipe_tag_rsp_tag_state_ram_dbe_i;
    orig_pipe_tag_rsp_info.tag_ram_sbe            = pipe_tag_rsp_tag_ram_sbe_i;
    orig_pipe_tag_rsp_info.tag_ram_dbe            = pipe_tag_rsp_tag_ram_dbe_i;
  end

  // ── data_rsp_t (new) ──
  data_rsp_t new_pipe_data_rsp_info;
  always_comb begin
    new_pipe_data_rsp_info              = '0;
    new_pipe_data_rsp_info.trans_id     = pipe_data_rsp_trans_id_i;
    new_pipe_data_rsp_info.reqq_id      = pipe_data_rsp_reqq_id_i;
    new_pipe_data_rsp_info.rsp_type     = pipe_data_rsp_type_e'(pipe_data_rsp_rsp_type_i);
    new_pipe_data_rsp_info.tag_sbe      = pipe_data_rsp_tag_sbe_i;
    new_pipe_data_rsp_info.qwen         = pipe_data_rsp_qwen_i;
    new_pipe_data_rsp_info.data_index   = pipe_data_rsp_data_index_i;
    new_pipe_data_rsp_info.data_ram_sbe = pipe_data_rsp_data_ram_sbe_i;
    new_pipe_data_rsp_info.data_ram_dbe = pipe_data_rsp_data_ram_dbe_i;
  end

  // ── sc_pipe_data_rsp_t (original) ──
  sc_pipe_data_rsp_t orig_pipe_data_rsp_info;
  always_comb begin
    orig_pipe_data_rsp_info              = '0;
    orig_pipe_data_rsp_info.trans_id     = pipe_data_rsp_trans_id_i;
    orig_pipe_data_rsp_info.reqq_id      = pipe_data_rsp_reqq_id_i;
    orig_pipe_data_rsp_info.rsp_type     = sc_pipe_data_rsp_type_t'(pipe_data_rsp_rsp_type_i);
    orig_pipe_data_rsp_info.tag_sbe      = pipe_data_rsp_tag_sbe_i;
    orig_pipe_data_rsp_info.qwen         = pipe_data_rsp_qwen_i;
    orig_pipe_data_rsp_info.data_index   = pipe_data_rsp_data_index_i;
    orig_pipe_data_rsp_info.data_ram_sbe = pipe_data_rsp_data_ram_sbe_i;
    orig_pipe_data_rsp_info.data_ram_dbe = pipe_data_rsp_data_ram_dbe_i;
  end

  // ── data_rsp_pre_t (new) ──
  data_rsp_pre_t new_pipe_data_rsp_pre_info;
  always_comb begin
    new_pipe_data_rsp_pre_info          = '0;
    new_pipe_data_rsp_pre_info.trans_id = pipe_data_rsp_pre_trans_id_i;
    new_pipe_data_rsp_pre_info.reqq_id  = pipe_data_rsp_pre_reqq_id_i;
    new_pipe_data_rsp_pre_info.rsp_type = pipe_data_rsp_type_e'(pipe_data_rsp_pre_rsp_type_i);
  end

  // ── sc_pipe_data_rsp_pre_t (original) ──
  sc_pipe_data_rsp_pre_t orig_pipe_data_rsp_pre_info;
  always_comb begin
    orig_pipe_data_rsp_pre_info          = '0;
    orig_pipe_data_rsp_pre_info.trans_id = pipe_data_rsp_pre_trans_id_i;
    orig_pipe_data_rsp_pre_info.reqq_id  = pipe_data_rsp_pre_reqq_id_i;
    orig_pipe_data_rsp_pre_info.rsp_type = sc_pipe_data_rsp_type_t'(pipe_data_rsp_pre_rsp_type_i);
  end

  // ── cbuf_rsp_t (new) ──
  cbuf_rsp_t new_pipe_cbuf_rsp_info;
  always_comb begin
    new_pipe_cbuf_rsp_info                 = '0;
    new_pipe_cbuf_rsp_info.trans_id        = pipe_cbuf_rsp_trans_id_i;
    new_pipe_cbuf_rsp_info.reqq_id         = pipe_cbuf_rsp_reqq_id_i;
    new_pipe_cbuf_rsp_info.victim          = pipe_cbuf_rsp_victim_i;
    new_pipe_cbuf_rsp_info.victim_address  = pipe_cbuf_rsp_victim_address_i;
  end

  // ── sc_pipe_cbuf_rsp_t (original) ──
  sc_pipe_cbuf_rsp_t orig_pipe_cbuf_rsp_info;
  always_comb begin
    orig_pipe_cbuf_rsp_info                 = '0;
    orig_pipe_cbuf_rsp_info.trans_id        = pipe_cbuf_rsp_trans_id_i;
    orig_pipe_cbuf_rsp_info.reqq_id         = pipe_cbuf_rsp_reqq_id_i;
    orig_pipe_cbuf_rsp_info.victim          = pipe_cbuf_rsp_victim_i;
    orig_pipe_cbuf_rsp_info.victim_address  = pipe_cbuf_rsp_victim_address_i;
  end

  // ── mesh rsp structs ──
  reqq_mesh_rsp_t new_to_l3_mesh_rsp_info;
  always_comb begin
    new_to_l3_mesh_rsp_info           = '0;
    new_to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    new_to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    new_to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    new_to_l3_mesh_rsp_info.zero_data = to_l3_mesh_rsp_zero_data_i;
    new_to_l3_mesh_rsp_info.err       = to_l3_mesh_rsp_err_i;
  end

  sc_reqq_mesh_rsp_t orig_to_l3_mesh_rsp_info;
  always_comb begin
    orig_to_l3_mesh_rsp_info           = '0;
    orig_to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    orig_to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    orig_to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    orig_to_l3_mesh_rsp_info.zero_data = to_l3_mesh_rsp_zero_data_i;
    orig_to_l3_mesh_rsp_info.err       = to_l3_mesh_rsp_err_i;
  end

  reqq_mesh_rsp_t new_to_sys_mesh_rsp_info;
  always_comb begin
    new_to_sys_mesh_rsp_info           = '0;
    new_to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    new_to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    new_to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    new_to_sys_mesh_rsp_info.zero_data = to_sys_mesh_rsp_zero_data_i;
    new_to_sys_mesh_rsp_info.err       = to_sys_mesh_rsp_err_i;
  end

  sc_reqq_mesh_rsp_t orig_to_sys_mesh_rsp_info;
  always_comb begin
    orig_to_sys_mesh_rsp_info           = '0;
    orig_to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    orig_to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    orig_to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    orig_to_sys_mesh_rsp_info.zero_data = to_sys_mesh_rsp_zero_data_i;
    orig_to_sys_mesh_rsp_info.err       = to_sys_mesh_rsp_err_i;
  end

  // ── ESR / perfmon / DFT — all zero ──
  bank_esr_ctl_t       new_esr_ctl;
  assign new_esr_ctl = '0;
  esr_shire_config_t   new_esr_shire_config;
  assign new_esr_shire_config = '0;
  l3_swizzle_ctl_t     new_swizzle_ctl;
  assign new_swizzle_ctl = '0;
  perfmon_req_info_t   new_perfmon_p0_req;
  assign new_perfmon_p0_req = '0;
  perfmon_req_info_t   new_perfmon_p1_req;
  assign new_perfmon_p1_req = '0;
  dft_t                new_dft;
  assign new_dft = '0;

  sc_bank_esr_ctl_t    orig_esr_ctl;
  assign orig_esr_ctl = '0;
  esr_shire_config_t   orig_esr_shire_config;
  assign orig_esr_shire_config = '0;
  esr_sc_l3_shire_swizzle_ctl_t orig_swizzle_ctl;
  assign orig_swizzle_ctl = '0;
  sc_perfmon_req_info_t orig_perfmon_p0_req;
  assign orig_perfmon_p0_req = '0;
  sc_perfmon_req_info_t orig_perfmon_p1_req;
  assign orig_perfmon_p1_req = '0;

  // ── rbuf_state_t — tie to zero ──
  rbuf_state_t         new_rbuf_state;
  assign new_rbuf_state = '0;
  sc_pipe_rbuf_state_t orig_rbuf_state;
  assign orig_rbuf_state = '0;

  // ═══════════════════════════════════════════════════════
  // New module instance
  // ═══════════════════════════════════════════════════════

  reqq_pipe_req_t                       new_pipe_req_info;
  data_rsp_modifier_t                   new_data_rsp_modifier;
  rbuf_clear_req_t                      new_rbuf_clear_req;
  rbuf_rd_req_t                         new_rbuf_rd_req;
  reqq_mesh_req_t                       new_to_l3_mesh_req_info;
  reqq_mesh_req_t                       new_to_sys_mesh_req_info;
  rspmux_t                              new_rspmux_l2_info;
  rspmux_t                              new_rspmux_l3_info;
  reqq_entry_state_t [NumEntries-1:0]   new_reqq_state;
  reqq_alloc_t                          new_alloc_l2_info_ag;
  reqq_alloc_t                          new_alloc_l3_info_ag;
  reqq_alloc_t                          new_alloc_l2_info_ad;
  reqq_alloc_t                          new_alloc_l3_info_ad;
  perfmon_rsp_info_t                    new_perfmon_p0_rsp;
  perfmon_rsp_info_t                    new_perfmon_p1_rsp;

  shirecache_bank_reqq u_new (
    .clk_i                                (clk_i),
    .rst_ni                               (rst_ni),
    .dft_i                                (new_dft),

    .bank_l2_ready_o                      (new_bank_l2_ready_o),
    .bank_l2_valid_i                      (bank_l2_valid_i),
    .bank_l2_info_i                       (new_bank_l2_info),
    .reqq_alloc_l2_id_ag_o                (new_reqq_alloc_l2_id_o),
    .dataq_bank_l2_ready_i                (dataq_bank_l2_ready_i),

    .bank_l3_ready_o                      (new_bank_l3_ready_o),
    .bank_l3_valid_i                      (bank_l3_valid_i),
    .bank_l3_info_i                       (new_bank_l3_info),
    .reqq_alloc_l3_id_ag_o                (new_reqq_alloc_l3_id_o),
    .dataq_bank_l3_ready_i                (dataq_bank_l3_ready_i),

    .alloc_l2_valid_ag_o                  (),
    .alloc_l2_info_ag_o                   (new_alloc_l2_info_ag),
    .alloc_l3_valid_ag_o                  (),
    .alloc_l3_info_ag_o                   (new_alloc_l3_info_ag),
    .alloc_l2_valid_ad_o                  (),
    .alloc_l2_info_ad_o                   (new_alloc_l2_info_ad),
    .alloc_l3_valid_ad_o                  (),
    .alloc_l3_info_ad_o                   (new_alloc_l3_info_ad),

    .set_reqq_err_detected_hot_i          (set_reqq_err_detected_hot_i),
    .set_reqq_err_zero_data_hot_i         (set_reqq_err_zero_data_hot_i),

    .pipe_idx_cop_aborted_i               (pipe_idx_cop_aborted_i),
    .pipe_idx_cop_l2_busy_i               (pipe_idx_cop_l2_busy_i),
    .pipe_idx_cop_l3_busy_i               (pipe_idx_cop_l3_busy_i),
    .pipe_idx_cop_req_ready_o             (new_pipe_idx_cop_req_ready_o),
    .pipe_idx_cop_req_valid_i             (pipe_idx_cop_req_valid_i),
    .pipe_idx_cop_req_info_i              (new_idx_cop_req_info),

    .dataq_write_status_any_i             (dataq_write_status_any_i),
    .dataq_write_status_hot_i             (dataq_write_status_hot_i),
    .dataq_write_status_id_i              (dataq_write_status_id_i),
    .dataq_read_status_hot_i              (dataq_read_status_hot_i),

    .pipe_start_o                         (new_pipe_start_o),
    .pipe_req_info_o                      (new_pipe_req_info),
    .pipe_busy_i                          (new_pipe_busy),
    .pipe_squash_i                        (pipe_squash_i),
    .pipe_tag_rsp_valid_i                 (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info_i                  (new_pipe_tag_rsp_info),
    .pipe_data_rsp_valid_i                (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info_i                 (new_pipe_data_rsp_info),
    .pipe_data_rsp_pre_valid_i            (pipe_data_rsp_pre_valid_i),
    .pipe_data_rsp_pre_info_i             (new_pipe_data_rsp_pre_info),
    .pipe_cbuf_rsp_valid_i                (pipe_cbuf_rsp_valid_i),
    .pipe_cbuf_rsp_info_i                 (new_pipe_cbuf_rsp_info),
    .mesh_bypassed_victim_i               (mesh_bypassed_victim_i),
    .rspmux_bypassed_data_rsp_i           (rspmux_bypassed_data_rsp_i),

    .pipe_data_rsp_modifier_info_o        (new_data_rsp_modifier),

    .pipe_rbuf_clear_req_info_o           (new_rbuf_clear_req),
    .pipe_rbuf_rd_req_valid_o             (new_pipe_rbuf_rd_req_valid_o),
    .pipe_rbuf_rd_req_info_o              (new_rbuf_rd_req),
    .pipe_rbuf_rd_req_ready_i             (pipe_rbuf_rd_req_ready_i),
    .pipe_rbuf_state_i                    (new_rbuf_state),

    .to_l3_mesh_master_bank_clk_en_o      (),
    .reqq_to_l3_mesh_req_valid_nodata_o   (new_to_l3_mesh_req_valid_nodata_o),
    .reqq_to_l3_mesh_req_ready_nodata_i   (reqq_to_l3_mesh_req_ready_nodata_i),
    .reqq_to_l3_mesh_req_valid_wdata_o    (new_to_l3_mesh_req_valid_wdata_o),
    .reqq_to_l3_mesh_req_ready_wdata_i    (reqq_to_l3_mesh_req_ready_wdata_i),
    .reqq_to_l3_mesh_req_info_o           (new_to_l3_mesh_req_info),
    .to_l3_mesh_rsp_valid_i               (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info_i                (new_to_l3_mesh_rsp_info),

    .to_sys_mesh_master_bank_clk_en_o     (),
    .reqq_to_sys_mesh_req_valid_nodata_o  (new_to_sys_mesh_req_valid_nodata_o),
    .reqq_to_sys_mesh_req_ready_nodata_i  (reqq_to_sys_mesh_req_ready_nodata_i),
    .reqq_to_sys_mesh_req_valid_wdata_o   (new_to_sys_mesh_req_valid_wdata_o),
    .reqq_to_sys_mesh_req_ready_wdata_i   (reqq_to_sys_mesh_req_ready_wdata_i),
    .reqq_to_sys_mesh_req_info_o          (new_to_sys_mesh_req_info),
    .to_sys_mesh_rsp_valid_i              (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info_i               (new_to_sys_mesh_rsp_info),

    .reqq_rspmux_l2_valid_nodata_o        (new_rspmux_l2_valid_nodata_o),
    .reqq_rspmux_l2_ready_nodata_i        (reqq_rspmux_l2_ready_nodata_i),
    .reqq_rspmux_l2_valid_wdata_o         (new_rspmux_l2_valid_wdata_o),
    .reqq_rspmux_l2_ready_wdata_i         (reqq_rspmux_l2_ready_wdata_i),
    .reqq_rspmux_l2_info_o                (new_rspmux_l2_info),

    .reqq_rspmux_l3_valid_nodata_o        (new_rspmux_l3_valid_nodata_o),
    .reqq_rspmux_l3_ready_nodata_i        (reqq_rspmux_l3_ready_nodata_i),
    .reqq_rspmux_l3_valid_wdata_o         (new_rspmux_l3_valid_wdata_o),
    .reqq_rspmux_l3_ready_wdata_i         (reqq_rspmux_l3_ready_wdata_i),
    .reqq_rspmux_l3_info_o                (new_rspmux_l3_info),

    .rspmux_sent_valid_hot_i              (rspmux_sent_valid_hot_i),

    .reqq_state_o                         (new_reqq_state),
    .a_reqq_entry_is_valid_o              (new_a_reqq_entry_is_valid_o),

    .mbist_on_i                           (mbist_on_i),

    .perfmon_p0_req_info_i                (new_perfmon_p0_req),
    .perfmon_p0_rsp_info_o                (new_perfmon_p0_rsp),
    .perfmon_p1_req_info_i                (new_perfmon_p1_req),
    .perfmon_p1_rsp_info_o                (new_perfmon_p1_rsp),

    .esr_shire_config_i                   (new_esr_shire_config),
    .esr_ctl_i                            (new_esr_ctl),
    .esr_sc_l3_shire_swizzle_ctl_i        (new_swizzle_ctl)
  );

  assign new_pipe_req_reqq_id_o = new_pipe_req_info.reqq_id;
  assign new_pipe_req_opcode_o  = new_pipe_req_info.opcode;

  // ═══════════════════════════════════════════════════════
  // Original module instance
  // ═══════════════════════════════════════════════════════

  sc_reqq_pipe_req_t                            orig_pipe_req_info;
  sc_pipe_data_rsp_modifier_t                   orig_data_rsp_modifier;
  sc_pipe_rbuf_clear_req_t                      orig_rbuf_clear_req;
  sc_pipe_rbuf_rd_req_t                         orig_rbuf_rd_req;
  sc_reqq_mesh_req_t                            orig_to_l3_mesh_req_info;
  sc_reqq_mesh_req_t                            orig_to_sys_mesh_req_info;
  sc_rspmux_t                                   orig_rspmux_l2_info;
  sc_rspmux_t                                   orig_rspmux_l3_info;
  sc_reqq_entry_state_t [NumEntries-1:0]        orig_reqq_state;
  sc_reqq_alloc_t                               orig_alloc_l2_info_ag;
  sc_reqq_alloc_t                               orig_alloc_l3_info_ag;
  sc_reqq_alloc_t                               orig_alloc_l2_info_ad;
  sc_reqq_alloc_t                               orig_alloc_l3_info_ad;
  sc_perfmon_rsp_info_t                         orig_perfmon_p0_rsp;
  sc_perfmon_rsp_info_t                         orig_perfmon_p1_rsp;

  shire_cache_bank_reqq u_orig (
    .clock                                (clk_i),
    .reset_in                             (reset),

    .bank_l2_ready                        (orig_bank_l2_ready_o),
    .bank_l2_valid                        (bank_l2_valid_i),
    .bank_l2_info                         (orig_bank_l2_info),
    .reqq_alloc_l2_id_ag                  (orig_reqq_alloc_l2_id_o),
    .dataq_bank_l2_ready                  (dataq_bank_l2_ready_i),

    .bank_l3_ready                        (orig_bank_l3_ready_o),
    .bank_l3_valid                        (bank_l3_valid_i),
    .bank_l3_info                         (orig_bank_l3_info),
    .reqq_alloc_l3_id_ag                  (orig_reqq_alloc_l3_id_o),
    .dataq_bank_l3_ready                  (dataq_bank_l3_ready_i),

    .alloc_l2_valid_ag                    (),
    .alloc_l2_info_ag                     (orig_alloc_l2_info_ag),
    .alloc_l3_valid_ag                    (),
    .alloc_l3_info_ag                     (orig_alloc_l3_info_ag),
    .alloc_l2_valid_ad                    (),
    .alloc_l2_info_ad                     (orig_alloc_l2_info_ad),
    .alloc_l3_valid_ad                    (),
    .alloc_l3_info_ad                     (orig_alloc_l3_info_ad),

    .set_reqq_err_detected_hot            (set_reqq_err_detected_hot_i),
    .set_reqq_err_zero_data_hot           (set_reqq_err_zero_data_hot_i),

    .pipe_idx_cop_aborted                 (pipe_idx_cop_aborted_i),
    .pipe_idx_cop_l2_busy                 (pipe_idx_cop_l2_busy_i),
    .pipe_idx_cop_l3_busy                 (pipe_idx_cop_l3_busy_i),
    .pipe_idx_cop_req_ready               (orig_pipe_idx_cop_req_ready_o),
    .pipe_idx_cop_req_valid               (pipe_idx_cop_req_valid_i),
    .pipe_idx_cop_req_info                (orig_idx_cop_req_info),

    .dataq_write_status_any               (dataq_write_status_any_i),
    .dataq_write_status_hot               (dataq_write_status_hot_i),
    .dataq_write_status_id                (dataq_write_status_id_i),
    .dataq_read_status_hot                (dataq_read_status_hot_i),

    .pipe_start                           (orig_pipe_start_o),
    .pipe_req_info                        (orig_pipe_req_info),
    .pipe_busy                            (orig_pipe_busy),
    .pipe_squash                          (pipe_squash_i),
    .pipe_tag_rsp_valid                   (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info                    (orig_pipe_tag_rsp_info),
    .pipe_data_rsp_valid                  (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info                   (orig_pipe_data_rsp_info),
    .pipe_data_rsp_pre_valid              (pipe_data_rsp_pre_valid_i),
    .pipe_data_rsp_pre_info               (orig_pipe_data_rsp_pre_info),
    .pipe_cbuf_rsp_valid                  (pipe_cbuf_rsp_valid_i),
    .pipe_cbuf_rsp_info                   (orig_pipe_cbuf_rsp_info),
    .mesh_bypassed_victim                 (mesh_bypassed_victim_i),
    .rspmux_bypassed_data_rsp             (rspmux_bypassed_data_rsp_i),

    .pipe_data_rsp_modifier_info          (orig_data_rsp_modifier),

    .pipe_rbuf_clear_req_info             (orig_rbuf_clear_req),
    .pipe_rbuf_rd_req_valid               (orig_pipe_rbuf_rd_req_valid_o),
    .pipe_rbuf_rd_req_info                (orig_rbuf_rd_req),
    .pipe_rbuf_rd_req_ready               (pipe_rbuf_rd_req_ready_i),
    .pipe_rbuf_state                      (orig_rbuf_state),

    .to_l3_mesh_master_bank_clk_en        (),
    .reqq_to_l3_mesh_req_valid_nodata     (orig_to_l3_mesh_req_valid_nodata_o),
    .reqq_to_l3_mesh_req_ready_nodata     (reqq_to_l3_mesh_req_ready_nodata_i),
    .reqq_to_l3_mesh_req_valid_wdata      (orig_to_l3_mesh_req_valid_wdata_o),
    .reqq_to_l3_mesh_req_ready_wdata      (reqq_to_l3_mesh_req_ready_wdata_i),
    .reqq_to_l3_mesh_req_info             (orig_to_l3_mesh_req_info),
    .to_l3_mesh_rsp_valid                 (to_l3_mesh_rsp_valid_i),
    .to_l3_mesh_rsp_info                  (orig_to_l3_mesh_rsp_info),

    .to_sys_mesh_master_bank_clk_en       (),
    .reqq_to_sys_mesh_req_valid_nodata    (orig_to_sys_mesh_req_valid_nodata_o),
    .reqq_to_sys_mesh_req_ready_nodata    (reqq_to_sys_mesh_req_ready_nodata_i),
    .reqq_to_sys_mesh_req_valid_wdata     (orig_to_sys_mesh_req_valid_wdata_o),
    .reqq_to_sys_mesh_req_ready_wdata     (reqq_to_sys_mesh_req_ready_wdata_i),
    .reqq_to_sys_mesh_req_info            (orig_to_sys_mesh_req_info),
    .to_sys_mesh_rsp_valid                (to_sys_mesh_rsp_valid_i),
    .to_sys_mesh_rsp_info                 (orig_to_sys_mesh_rsp_info),

    .reqq_rspmux_l2_valid_nodata          (orig_rspmux_l2_valid_nodata_o),
    .reqq_rspmux_l2_ready_nodata          (reqq_rspmux_l2_ready_nodata_i),
    .reqq_rspmux_l2_valid_wdata           (orig_rspmux_l2_valid_wdata_o),
    .reqq_rspmux_l2_ready_wdata           (reqq_rspmux_l2_ready_wdata_i),
    .reqq_rspmux_l2_info                  (orig_rspmux_l2_info),

    .reqq_rspmux_l3_valid_nodata          (orig_rspmux_l3_valid_nodata_o),
    .reqq_rspmux_l3_ready_nodata          (reqq_rspmux_l3_ready_nodata_i),
    .reqq_rspmux_l3_valid_wdata           (orig_rspmux_l3_valid_wdata_o),
    .reqq_rspmux_l3_ready_wdata           (reqq_rspmux_l3_ready_wdata_i),
    .reqq_rspmux_l3_info                  (orig_rspmux_l3_info),

    .rspmux_sent_valid_hot                (rspmux_sent_valid_hot_i),

    .reqq_state                           (orig_reqq_state),
    .a_reqq_entry_is_valid                (orig_a_reqq_entry_is_valid_o),

    .mbist_on                             (mbist_on_i),

    .perfmon_p0_req_info                  (orig_perfmon_p0_req),
    .perfmon_p0_rsp_info                  (orig_perfmon_p0_rsp),
    .perfmon_p1_req_info                  (orig_perfmon_p1_req),
    .perfmon_p1_rsp_info                  (orig_perfmon_p1_rsp),

    .esr_shire_config                     (orig_esr_shire_config),
    .esr_ctl                              (orig_esr_ctl),
    .esr_sc_l3_shire_swizzle_ctl          (orig_swizzle_ctl),

    .dft__reset_byp_hv                    (1'b0),
    .dft__reset_hv                        (1'b0)
  );

  assign orig_pipe_req_reqq_id_o = orig_pipe_req_info.reqq_id;
  assign orig_pipe_req_opcode_o  = orig_pipe_req_info.opcode;

  // ═══════════════════════════════════════════════════════
  // Unused signal suppression
  // ═══════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{
    1'b0,
    new_pipe_req_info,
    orig_pipe_req_info,
    new_data_rsp_modifier,
    orig_data_rsp_modifier,
    new_rbuf_clear_req,
    orig_rbuf_clear_req,
    new_rbuf_rd_req,
    orig_rbuf_rd_req,
    new_to_l3_mesh_req_info,
    orig_to_l3_mesh_req_info,
    new_to_sys_mesh_req_info,
    orig_to_sys_mesh_req_info,
    new_rspmux_l2_info,
    orig_rspmux_l2_info,
    new_rspmux_l3_info,
    orig_rspmux_l3_info,
    new_reqq_state,
    orig_reqq_state,
    new_alloc_l2_info_ag,
    orig_alloc_l2_info_ag,
    new_alloc_l3_info_ag,
    orig_alloc_l3_info_ag,
    new_alloc_l2_info_ad,
    orig_alloc_l2_info_ad,
    new_alloc_l3_info_ad,
    orig_alloc_l3_info_ad,
    new_perfmon_p0_rsp,
    new_perfmon_p1_rsp,
    orig_perfmon_p0_rsp,
    orig_perfmon_p1_rsp
  };

endmodule : cosim_reqq_tb
