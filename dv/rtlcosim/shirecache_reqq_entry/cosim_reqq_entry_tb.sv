// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank_reqq_entry vs new
// shirecache_bank_reqq_entry.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.
//
// Struct inputs are built from flat scalars so that C++ can drive them.
// Struct outputs are decomposed to scalars so C++ can compare field-by-field.

`include "soc.vh"
`include "macros.vh"

module cosim_reqq_entry_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Core ──
  input  logic [ReqqIdSize-1:0]       my_reqq_id_i,

  // ── Allocation ag ──
  input  logic                        alloc_me_ag_i,
  input  logic                        alloc_any_ag_i,
  input  logic                        coalesce_victim_match_alloc_ag_i,

  // Alloc info (flat scalars)
  input  logic [ReqqIdSize-1:0]       alloc_reqq_id_i,
  input  logic [DvTransIdSize-1:0]    alloc_trans_id_i,
  input  logic [ReqqTagIdSize-1:0]    alloc_tag_id_i,
  input  logic [ReqqSourceSize-1:0]   alloc_source_i,
  input  logic [PortIdSize-1:0]       alloc_port_id_i,
  input  logic [AxiAxqosSize-1:0]     alloc_qos_i,
  input  logic [2:0]                  alloc_address_space_i,
  input  logic [1:0]                  alloc_reqq_source_i,
  input  logic [1:0]                  alloc_cache_level_start_i,
  input  logic [1:0]                  alloc_cache_level_dest_i,
  input  logic                        alloc_scp_is_remote_i,
  input  logic [NeighCoopMaskSize-1:0] alloc_readcoop_dest_i,
  input  logic [PaSize-1:0]           alloc_address_i,
  input  logic [LineQwSize-1:0]       alloc_qwen_i,
  input  logic [2:0]                  alloc_size_i,
  input  logic [4:0]                  alloc_orig_opcode_i,
  input  logic [5:0]                  alloc_opcode_i,
  input  logic                        alloc_zero_data_i,
  input  logic                        alloc_scp_fill_source_is_scp_i,
  input  logic                        alloc_dep_valid_i,
  input  logic                        alloc_dep_non_meshll_head_i,
  input  logic [ReqqIdSize-1:0]       alloc_dep_id_i,
  input  logic [ReqqIdSize-1:0]       alloc_paired_id_i,
  input  logic                        alloc_rbuf_valid_i,
  input  logic                        alloc_rbuf_pending_valid_i,
  input  logic [RbufIdSize-1:0]       alloc_rbuf_id_i,
  input  logic                        alloc_err_valid_i,
  input  logic [4:0]                  alloc_err_type_i,
  input  logic                        alloc_err_imprecise_i,
  input  logic                        alloc_traced_i,

  // ── Allocation ad ──
  input  logic                        alloc_me_ad_i,

  // ── Dependency ──
  input  logic [ReqqDepth-1:0]        clear_dep_hot_i,
  input  logic [ReqqDepth-1:0]        clear_dep_rbuf_1hot_pre_i,

  // ── Victim dep ──
  input  logic                        tag_victim_dep_valid_i,
  input  logic [ReqqIdSize-1:0]       tag_victim_dep_id_i,
  input  logic                        tag_victim_dep_tail_i,
  input  logic                        tag_victim_insert_behind_me_i,
  input  logic                        tag_victim_insert_ahead_of_me_i,
  input  logic                        tag_victim_upgrade_behind_me_i,
  input  logic                        tag_victim_address_match_i,
  input  logic                        upgrade_me_to_meshll_i,
  input  logic                        upgrade_ahead_of_me_to_meshll_i,

  // ── Coalesce victim ──
  input  logic                        coalesce_victim_me_i,
  input  logic                        coalesce_victim_dep_valid_i,
  input  logic [ReqqIdSize-1:0]       coalesce_victim_dep_id_i,
  input  logic                        coalesce_victim_dep_non_meshll_head_i,
  input  logic                        coalesce_victim_insert_behind_me_i,

  // ── DataQ ──
  input  logic                        dataq_write_status_any_i,
  input  logic                        dataq_write_status_me_i,
  input  logic [ReqqIdSize-1:0]       dataq_write_status_id_i,
  input  logic                        dataq_read_status_me_i,

  // ── Pipeline ──
  input  logic                        pipe_req_picked_me_i,
  input  logic [ReqqIdSize-1:0]       pipe_req_reqq_id_any_i,
  input  logic                        pipe_req_picked_any_i,

  // Tag response (flat)
  input  logic                        pipe_tag_rsp_valid_any_i,
  input  logic                        pipe_tag_rsp_valid_me_i,
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

  // Data response (flat)
  input  logic                        pipe_data_rsp_valid_me_i,
  input  logic [DvTransIdSize-1:0]    pipe_data_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_reqq_id_i,
  input  logic                        pipe_data_rsp_rsp_type_i,
  input  logic                        pipe_data_rsp_tag_sbe_i,
  input  logic [LineDataSize-1:0]     pipe_data_rsp_data_i,
  input  logic [LineQwSize-1:0]       pipe_data_rsp_qwen_i,
  input  logic [DataRamAddrSize-1:0]  pipe_data_rsp_data_index_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_data_ram_sbe_i,
  input  logic [LineDwSize-1:0]       pipe_data_rsp_data_ram_dbe_i,

  // Cbuf response (flat)
  input  logic                        pipe_cbuf_rsp_valid_any_i,
  input  logic [DvTransIdSize-1:0]    pipe_cbuf_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       pipe_cbuf_rsp_reqq_id_i,
  input  logic                        pipe_cbuf_rsp_victim_i,
  input  logic [PaSize-1:0]           pipe_cbuf_rsp_victim_address_i,

  // ── Rbuf ──
  input  logic                        rbuf_req_picked_me_i,

  // ── Mesh (L3) ──
  input  logic                        to_l3_mesh_req_picked_i,
  input  logic                        to_l3_mesh_rsp_valid_me_i,
  input  logic                        to_l3_mesh_rsp_valid_any_i,
  input  logic [DvTransIdSize-1:0]    to_l3_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_l3_mesh_rsp_dataq_id_i,
  input  logic [LineDataSize-1:0]     to_l3_mesh_rsp_data_i,
  input  logic                        to_l3_mesh_rsp_zero_data_i,
  input  logic                        to_l3_mesh_rsp_err_i,

  // ── Mesh (Sys) ──
  input  logic                        to_sys_mesh_req_picked_i,
  input  logic                        to_sys_mesh_rsp_valid_me_i,
  input  logic                        to_sys_mesh_rsp_valid_any_i,
  input  logic [DvTransIdSize-1:0]    to_sys_mesh_rsp_trans_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_reqq_id_i,
  input  logic [ReqqIdSize-1:0]       to_sys_mesh_rsp_dataq_id_i,
  input  logic [LineDataSize-1:0]     to_sys_mesh_rsp_data_i,
  input  logic                        to_sys_mesh_rsp_zero_data_i,
  input  logic                        to_sys_mesh_rsp_err_i,

  input  logic                        mesh_bypassed_victim_i,

  // ── Rspmux ──
  input  logic                        rspmux_l2_picked_i,
  input  logic                        rspmux_l3_picked_i,
  input  logic                        rspmux_bypassed_data_rsp_i,
  input  logic                        rspmux_sent_valid_i,

  // ── Priority ──
  input  logic                        reqq_2nd_pass_need_priority_i,
  input  logic                        reqq_idx_cop_need_priority_i,
  input  logic                        pipe_idx_cop_aborted_i,

  // ── Error ──
  input  logic                        set_reqq_err_detected_me_i,
  input  logic                        set_reqq_err_zero_data_me_i,

  // ── ESR bypass overrides (scalar) ──
  input  logic                        esr_l2_bypass_i,
  input  logic                        esr_l3_bypass_i,
  input  logic                        esr_cbuf_enable_i,
  input  logic                        esr_remote_l3_enable_i,

  // ═══════════════════════════════════════════════════════
  // Outputs (scalar, both modules)
  // ═══════════════════════════════════════════════════════

  output logic                        new_reqq_entry_valid_o,
  output logic                        orig_reqq_entry_valid_o,
  output logic                        new_pipe_req_eligible_o,
  output logic                        orig_pipe_req_eligible_o,
  output logic                        new_rbuf_req_eligible_o,
  output logic                        orig_rbuf_req_eligible_o,
  output logic                        new_to_l3_mesh_req_eligible_o,
  output logic                        orig_to_l3_mesh_req_eligible_o,
  output logic                        new_to_sys_mesh_req_eligible_o,
  output logic                        orig_to_sys_mesh_req_eligible_o,
  output logic                        new_rspmux_l2_eligible_o,
  output logic                        orig_rspmux_l2_eligible_o,
  output logic                        new_rspmux_l3_eligible_o,
  output logic                        orig_rspmux_l3_eligible_o,
  output logic                        new_clear_dep_behind_me_o,
  output logic                        orig_clear_dep_behind_me_o,
  output logic [5:0]                  new_opcode_o,
  output logic [5:0]                  orig_opcode_o,
  output logic                        new_reqq_entry_alloc_o,
  output logic                        orig_reqq_entry_alloc_o,
  output logic                        new_reqq_entry_dealloc_o,
  output logic                        orig_reqq_entry_dealloc_o
);

  // Active-high synchronous reset for original
  wire reset = !rst_ni;

  // ═══════════════════════════════════════════════════════
  // Build structured types from scalar inputs
  // ═══════════════════════════════════════════════════════

  // ── reqq_alloc_t (new) ──
  reqq_alloc_t new_alloc_info;
  always_comb begin
    new_alloc_info.reqq_id                = alloc_reqq_id_i;
    new_alloc_info.trans_id               = alloc_trans_id_i;
    new_alloc_info.tag_id                 = alloc_tag_id_i;
    new_alloc_info.source                 = alloc_source_i;
    new_alloc_info.port_id                = alloc_port_id_i;
    new_alloc_info.qos                    = alloc_qos_i;
    new_alloc_info.address_space          = address_space_e'(alloc_address_space_i);
    new_alloc_info.reqq_source            = reqq_source_e'(alloc_reqq_source_i);
    new_alloc_info.cache_level_start      = cache_level_e'(alloc_cache_level_start_i);
    new_alloc_info.cache_level_dest       = cache_level_e'(alloc_cache_level_dest_i);
    new_alloc_info.scp_is_remote          = alloc_scp_is_remote_i;
    new_alloc_info.readcoop_dest          = alloc_readcoop_dest_i;
    new_alloc_info.address                = alloc_address_i;
    new_alloc_info.qwen                   = alloc_qwen_i;
    new_alloc_info.size                   = sc_size_t'(alloc_size_i);
    new_alloc_info.orig_opcode            = reqq_orig_opcode_e'(alloc_orig_opcode_i);
    new_alloc_info.opcode                 = reqq_opcode_e'(alloc_opcode_i);
    new_alloc_info.zero_data              = alloc_zero_data_i;
    new_alloc_info.scp_fill_source_is_scp = alloc_scp_fill_source_is_scp_i;
    new_alloc_info.dep_valid              = alloc_dep_valid_i;
    new_alloc_info.dep_non_meshll_head    = alloc_dep_non_meshll_head_i;
    new_alloc_info.dep_id                 = alloc_dep_id_i;
    new_alloc_info.paired_id              = alloc_paired_id_i;
    new_alloc_info.rbuf_valid             = alloc_rbuf_valid_i;
    new_alloc_info.rbuf_pending_valid     = alloc_rbuf_pending_valid_i;
    new_alloc_info.rbuf_id                = alloc_rbuf_id_i;
    new_alloc_info.err_valid              = alloc_err_valid_i;
    new_alloc_info.err_type               = err_rsp_e'(alloc_err_type_i);
    new_alloc_info.err_imprecise          = alloc_err_imprecise_i;
    new_alloc_info.traced                 = alloc_traced_i;
  end

  // ── sc_reqq_alloc_t (original) ──
  sc_reqq_alloc_t orig_alloc_info;
  always_comb begin
    orig_alloc_info.reqq_id                = alloc_reqq_id_i;
    orig_alloc_info.trans_id               = alloc_trans_id_i;
    orig_alloc_info.tag_id                 = alloc_tag_id_i;
    orig_alloc_info.source                 = alloc_source_i;
    orig_alloc_info.port_id                = alloc_port_id_i;
    orig_alloc_info.qos                    = alloc_qos_i;
    orig_alloc_info.address_space          = sc_address_space_t'(alloc_address_space_i);
    orig_alloc_info.reqq_source            = sc_reqq_source_t'(alloc_reqq_source_i);
    orig_alloc_info.cache_level_start      = sc_cache_level_t'(alloc_cache_level_start_i);
    orig_alloc_info.cache_level_dest       = sc_cache_level_t'(alloc_cache_level_dest_i);
    orig_alloc_info.scp_is_remote          = alloc_scp_is_remote_i;
    orig_alloc_info.readcoop_dest          = alloc_readcoop_dest_i;
    orig_alloc_info.address                = alloc_address_i;
    orig_alloc_info.qwen                   = alloc_qwen_i;
    orig_alloc_info.size                   = sc_size_t'(alloc_size_i);
    orig_alloc_info.orig_opcode            = sc_reqq_orig_opcode_t'(alloc_orig_opcode_i);
    orig_alloc_info.opcode                 = sc_reqq_opcode_t'(alloc_opcode_i);
    orig_alloc_info.zero_data              = alloc_zero_data_i;
    orig_alloc_info.scpFill_source_is_scp  = alloc_scp_fill_source_is_scp_i;
    orig_alloc_info.dep_valid              = alloc_dep_valid_i;
    orig_alloc_info.dep_non_meshll_head    = alloc_dep_non_meshll_head_i;
    orig_alloc_info.dep_id                 = alloc_dep_id_i;
    orig_alloc_info.paired_id              = alloc_paired_id_i;
    orig_alloc_info.rbuf_valid             = alloc_rbuf_valid_i;
    orig_alloc_info.rbuf_pending_valid     = alloc_rbuf_pending_valid_i;
    orig_alloc_info.rbuf_id                = alloc_rbuf_id_i;
    orig_alloc_info.err_valid              = alloc_err_valid_i;
    orig_alloc_info.err_type               = sc_err_rsp_t'(alloc_err_type_i);
    orig_alloc_info.err_imprecise          = alloc_err_imprecise_i;
    orig_alloc_info.traced                 = alloc_traced_i;
  end

  // ── tag_rsp_t (new) ──
  tag_rsp_t new_pipe_tag_rsp_info;
  always_comb begin
    new_pipe_tag_rsp_info.trans_id           = pipe_tag_rsp_trans_id_i;
    new_pipe_tag_rsp_info.reqq_id            = pipe_tag_rsp_reqq_id_i;
    new_pipe_tag_rsp_info.hit                = pipe_tag_rsp_hit_i;
    new_pipe_tag_rsp_info.orig_address       = pipe_tag_rsp_orig_address_i;
    new_pipe_tag_rsp_info.victim             = pipe_tag_rsp_victim_i;
    new_pipe_tag_rsp_info.victim_silent      = pipe_tag_rsp_victim_silent_i;
    new_pipe_tag_rsp_info.victim_address     = pipe_tag_rsp_victim_address_i;
    new_pipe_tag_rsp_info.victim_address_space = address_space_e'(pipe_tag_rsp_victim_address_space_i);
    new_pipe_tag_rsp_info.victim_write_around = pipe_tag_rsp_victim_write_around_i;
    new_pipe_tag_rsp_info.victim_qwen        = pipe_tag_rsp_victim_qwen_i;
    new_pipe_tag_rsp_info.err_rsp            = err_rsp_e'(pipe_tag_rsp_err_rsp_i);
    new_pipe_tag_rsp_info.tag_index          = pipe_tag_rsp_tag_index_i;
    new_pipe_tag_rsp_info.bubble             = pipe_tag_rsp_bubble_i;
    new_pipe_tag_rsp_info.opcode             = reqq_opcode_e'(pipe_tag_rsp_opcode_i);
    new_pipe_tag_rsp_info.tag_ram_hit_way    = pipe_tag_rsp_tag_ram_hit_way_i;
    new_pipe_tag_rsp_info.tag_ram_hit_qwen   = pipe_tag_rsp_tag_ram_hit_qwen_i;
    new_pipe_tag_rsp_info.tag_state_ram_sbe  = pipe_tag_rsp_tag_state_ram_sbe_i;
    new_pipe_tag_rsp_info.tag_state_ram_dbe  = pipe_tag_rsp_tag_state_ram_dbe_i;
    new_pipe_tag_rsp_info.tag_ram_sbe        = pipe_tag_rsp_tag_ram_sbe_i;
    new_pipe_tag_rsp_info.tag_ram_dbe        = pipe_tag_rsp_tag_ram_dbe_i;
  end

  // ── sc_pipe_tag_rsp_t (original) ──
  sc_pipe_tag_rsp_t orig_pipe_tag_rsp_info;
  always_comb begin
    orig_pipe_tag_rsp_info.trans_id           = pipe_tag_rsp_trans_id_i;
    orig_pipe_tag_rsp_info.reqq_id            = pipe_tag_rsp_reqq_id_i;
    orig_pipe_tag_rsp_info.hit                = pipe_tag_rsp_hit_i;
    orig_pipe_tag_rsp_info.orig_address       = pipe_tag_rsp_orig_address_i;
    orig_pipe_tag_rsp_info.victim             = pipe_tag_rsp_victim_i;
    orig_pipe_tag_rsp_info.victim_silent      = pipe_tag_rsp_victim_silent_i;
    orig_pipe_tag_rsp_info.victim_address     = pipe_tag_rsp_victim_address_i;
    orig_pipe_tag_rsp_info.victim_address_space = sc_address_space_t'(pipe_tag_rsp_victim_address_space_i);
    orig_pipe_tag_rsp_info.victim_write_around = pipe_tag_rsp_victim_write_around_i;
    orig_pipe_tag_rsp_info.victim_qwen        = pipe_tag_rsp_victim_qwen_i;
    orig_pipe_tag_rsp_info.err_rsp            = sc_err_rsp_t'(pipe_tag_rsp_err_rsp_i);
    orig_pipe_tag_rsp_info.tag_index          = pipe_tag_rsp_tag_index_i;
    orig_pipe_tag_rsp_info.bubble             = pipe_tag_rsp_bubble_i;
    orig_pipe_tag_rsp_info.opcode             = sc_reqq_opcode_t'(pipe_tag_rsp_opcode_i);
    orig_pipe_tag_rsp_info.tag_ram_hit_way    = pipe_tag_rsp_tag_ram_hit_way_i;
    orig_pipe_tag_rsp_info.tag_ram_hit_qwen   = pipe_tag_rsp_tag_ram_hit_qwen_i;
    orig_pipe_tag_rsp_info.tag_state_ram_sbe  = pipe_tag_rsp_tag_state_ram_sbe_i;
    orig_pipe_tag_rsp_info.tag_state_ram_dbe  = pipe_tag_rsp_tag_state_ram_dbe_i;
    orig_pipe_tag_rsp_info.tag_ram_sbe        = pipe_tag_rsp_tag_ram_sbe_i;
    orig_pipe_tag_rsp_info.tag_ram_dbe        = pipe_tag_rsp_tag_ram_dbe_i;
  end

  // ── data_rsp_t (new) ──
  data_rsp_t new_pipe_data_rsp_info;
  always_comb begin
    new_pipe_data_rsp_info.trans_id      = pipe_data_rsp_trans_id_i;
    new_pipe_data_rsp_info.reqq_id       = pipe_data_rsp_reqq_id_i;
    new_pipe_data_rsp_info.rsp_type      = pipe_data_rsp_type_e'(pipe_data_rsp_rsp_type_i);
    new_pipe_data_rsp_info.tag_sbe       = pipe_data_rsp_tag_sbe_i;
    new_pipe_data_rsp_info.data          = pipe_data_rsp_data_i;
    new_pipe_data_rsp_info.qwen          = pipe_data_rsp_qwen_i;
    new_pipe_data_rsp_info.data_index    = pipe_data_rsp_data_index_i;
    new_pipe_data_rsp_info.data_ram_sbe  = pipe_data_rsp_data_ram_sbe_i;
    new_pipe_data_rsp_info.data_ram_dbe  = pipe_data_rsp_data_ram_dbe_i;
  end

  // ── sc_pipe_data_rsp_t (original) ──
  sc_pipe_data_rsp_t orig_pipe_data_rsp_info;
  always_comb begin
    orig_pipe_data_rsp_info.trans_id      = pipe_data_rsp_trans_id_i;
    orig_pipe_data_rsp_info.reqq_id       = pipe_data_rsp_reqq_id_i;
    orig_pipe_data_rsp_info.rsp_type      = sc_pipe_data_rsp_type_t'(pipe_data_rsp_rsp_type_i);
    orig_pipe_data_rsp_info.tag_sbe       = pipe_data_rsp_tag_sbe_i;
    orig_pipe_data_rsp_info.data          = pipe_data_rsp_data_i;
    orig_pipe_data_rsp_info.qwen          = pipe_data_rsp_qwen_i;
    orig_pipe_data_rsp_info.data_index    = pipe_data_rsp_data_index_i;
    orig_pipe_data_rsp_info.data_ram_sbe  = pipe_data_rsp_data_ram_sbe_i;
    orig_pipe_data_rsp_info.data_ram_dbe  = pipe_data_rsp_data_ram_dbe_i;
  end

  // ── cbuf_rsp_t (new) ──
  cbuf_rsp_t new_pipe_cbuf_rsp_info;
  always_comb begin
    new_pipe_cbuf_rsp_info.trans_id       = pipe_cbuf_rsp_trans_id_i;
    new_pipe_cbuf_rsp_info.reqq_id        = pipe_cbuf_rsp_reqq_id_i;
    new_pipe_cbuf_rsp_info.victim         = pipe_cbuf_rsp_victim_i;
    new_pipe_cbuf_rsp_info.victim_address = pipe_cbuf_rsp_victim_address_i;
  end

  // ── sc_pipe_cbuf_rsp_t (original) ──
  sc_pipe_cbuf_rsp_t orig_pipe_cbuf_rsp_info;
  always_comb begin
    orig_pipe_cbuf_rsp_info.trans_id       = pipe_cbuf_rsp_trans_id_i;
    orig_pipe_cbuf_rsp_info.reqq_id        = pipe_cbuf_rsp_reqq_id_i;
    orig_pipe_cbuf_rsp_info.victim         = pipe_cbuf_rsp_victim_i;
    orig_pipe_cbuf_rsp_info.victim_address = pipe_cbuf_rsp_victim_address_i;
  end

  // ── rbuf_state_t / rbuf_clear_req_t — tie off to zero ──
  rbuf_state_t     new_rbuf_state;
  assign new_rbuf_state = '0;
  rbuf_clear_req_t new_rbuf_clear_req;
  assign new_rbuf_clear_req = '0;

  sc_pipe_rbuf_state_t     orig_rbuf_state;
  assign orig_rbuf_state = '0;
  sc_pipe_rbuf_clear_req_t orig_rbuf_clear_req;
  assign orig_rbuf_clear_req = '0;

  // ── mesh rsp structs ──
  reqq_mesh_rsp_t new_to_l3_mesh_rsp_info;
  always_comb begin
    new_to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    new_to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    new_to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    new_to_l3_mesh_rsp_info.data      = to_l3_mesh_rsp_data_i;
    new_to_l3_mesh_rsp_info.zero_data = to_l3_mesh_rsp_zero_data_i;
    new_to_l3_mesh_rsp_info.err       = to_l3_mesh_rsp_err_i;
  end

  sc_reqq_mesh_rsp_t orig_to_l3_mesh_rsp_info;
  always_comb begin
    orig_to_l3_mesh_rsp_info.trans_id  = to_l3_mesh_rsp_trans_id_i;
    orig_to_l3_mesh_rsp_info.reqq_id   = to_l3_mesh_rsp_reqq_id_i;
    orig_to_l3_mesh_rsp_info.dataq_id  = to_l3_mesh_rsp_dataq_id_i;
    orig_to_l3_mesh_rsp_info.data      = to_l3_mesh_rsp_data_i;
    orig_to_l3_mesh_rsp_info.zero_data = to_l3_mesh_rsp_zero_data_i;
    orig_to_l3_mesh_rsp_info.err       = to_l3_mesh_rsp_err_i;
  end

  reqq_mesh_rsp_t new_to_sys_mesh_rsp_info;
  always_comb begin
    new_to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    new_to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    new_to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    new_to_sys_mesh_rsp_info.data      = to_sys_mesh_rsp_data_i;
    new_to_sys_mesh_rsp_info.zero_data = to_sys_mesh_rsp_zero_data_i;
    new_to_sys_mesh_rsp_info.err       = to_sys_mesh_rsp_err_i;
  end

  sc_reqq_mesh_rsp_t orig_to_sys_mesh_rsp_info;
  always_comb begin
    orig_to_sys_mesh_rsp_info.trans_id  = to_sys_mesh_rsp_trans_id_i;
    orig_to_sys_mesh_rsp_info.reqq_id   = to_sys_mesh_rsp_reqq_id_i;
    orig_to_sys_mesh_rsp_info.dataq_id  = to_sys_mesh_rsp_dataq_id_i;
    orig_to_sys_mesh_rsp_info.data      = to_sys_mesh_rsp_data_i;
    orig_to_sys_mesh_rsp_info.zero_data = to_sys_mesh_rsp_zero_data_i;
    orig_to_sys_mesh_rsp_info.err       = to_sys_mesh_rsp_err_i;
  end

  // ── ESR / swizzle / DFT — construct from scalar overrides ──
  bank_esr_ctl_t       new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_l2_bypass        = esr_l2_bypass_i;
    new_esr_ctl.esr_sc_l3_bypass        = esr_l3_bypass_i;
    new_esr_ctl.esr_sc_cbuf_enable      = esr_cbuf_enable_i;
    new_esr_ctl.esr_sc_remote_l3_enable = esr_remote_l3_enable_i;
  end
  l3_swizzle_ctl_t     new_swizzle_ctl;
  assign new_swizzle_ctl = '0;
  dft_t                new_dft;
  assign new_dft = '0;

  sc_bank_esr_ctl_t                  orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.esr_sc_l2_bypass        = esr_l2_bypass_i;
    orig_esr_ctl.esr_sc_l3_bypass        = esr_l3_bypass_i;
    orig_esr_ctl.esr_sc_cbuf_enable      = esr_cbuf_enable_i;
    orig_esr_ctl.esr_sc_remote_l3_enable = esr_remote_l3_enable_i;
  end
  esr_sc_l3_shire_swizzle_ctl_t      orig_swizzle_ctl;
  assign orig_swizzle_ctl = '0;

  // ═══════════════════════════════════════════════════════
  // New module instance
  // ═══════════════════════════════════════════════════════

  reqq_pipe_req_t     new_pipe_req_info;
  rbuf_rd_req_t       new_rbuf_req_info;
  reqq_mesh_req_t     new_to_l3_mesh_req_info;
  reqq_mesh_req_t     new_to_sys_mesh_req_info;
  rspmux_t            new_rspmux_l2_info;
  rspmux_t            new_rspmux_l3_info;
  reqq_entry_state_t  new_entry_state;

  shirecache_bank_reqq_entry u_new (
    .clk_i                              (clk_i),
    .rst_ni                             (rst_ni),
    .my_reqq_id_i                       (my_reqq_id_i),

    .alloc_me_ag_i                      (alloc_me_ag_i),
    .alloc_info_ag_i                    (new_alloc_info),
    .alloc_any_ag_i                     (alloc_any_ag_i),
    .alloc_any_info_ag_i                (new_alloc_info),
    .coalesce_victim_match_alloc_ag_i   (coalesce_victim_match_alloc_ag_i),

    .alloc_me_ad_i                      (alloc_me_ad_i),

    .clear_dep_behind_me_o              (new_clear_dep_behind_me_o),
    .clear_dep_hot_i                    (clear_dep_hot_i),
    .clear_dep_rbuf_1hot_pre_i          (clear_dep_rbuf_1hot_pre_i),

    .tag_victim_dep_valid_i             (tag_victim_dep_valid_i),
    .tag_victim_dep_id_i                (tag_victim_dep_id_i),
    .tag_victim_dep_tail_i              (tag_victim_dep_tail_i),
    .tag_victim_insert_behind_me_i      (tag_victim_insert_behind_me_i),
    .tag_victim_insert_ahead_of_me_i    (tag_victim_insert_ahead_of_me_i),
    .tag_victim_upgrade_behind_me_i     (tag_victim_upgrade_behind_me_i),
    .tag_victim_address_match_i         (tag_victim_address_match_i),
    .upgrade_me_to_meshll_i             (upgrade_me_to_meshll_i),
    .upgrade_ahead_of_me_to_meshll_i    (upgrade_ahead_of_me_to_meshll_i),

    .coalesce_victim_me_i               (coalesce_victim_me_i),
    .coalesce_victim_dep_valid_i        (coalesce_victim_dep_valid_i),
    .coalesce_victim_dep_id_i           (coalesce_victim_dep_id_i),
    .coalesce_victim_dep_non_meshll_head_i (coalesce_victim_dep_non_meshll_head_i),
    .coalesce_victim_insert_behind_me_i (coalesce_victim_insert_behind_me_i),

    .dataq_write_status_any_i           (dataq_write_status_any_i),
    .dataq_write_status_me_i            (dataq_write_status_me_i),
    .dataq_write_status_id_i            (dataq_write_status_id_i),
    .dataq_read_status_me_i             (dataq_read_status_me_i),

    .pipe_req_eligible_o                (new_pipe_req_eligible_o),
    .pipe_req_info_me_o                 (new_pipe_req_info),
    .pipe_req_picked_me_i               (pipe_req_picked_me_i),
    .pipe_req_reqq_id_any_i             (pipe_req_reqq_id_any_i),
    .pipe_req_picked_any_i              (pipe_req_picked_any_i),

    .pipe_tag_rsp_valid_any_i           (pipe_tag_rsp_valid_any_i),
    .pipe_tag_rsp_valid_me_i            (pipe_tag_rsp_valid_me_i),
    .pipe_tag_rsp_info_i                (new_pipe_tag_rsp_info),
    .pipe_data_rsp_valid_me_i           (pipe_data_rsp_valid_me_i),
    .pipe_data_rsp_info_i               (new_pipe_data_rsp_info),
    .pipe_cbuf_rsp_valid_any_i          (pipe_cbuf_rsp_valid_any_i),
    .pipe_cbuf_rsp_info_i               (new_pipe_cbuf_rsp_info),

    .rbuf_req_eligible_o                (new_rbuf_req_eligible_o),
    .rbuf_req_info_me_o                 (new_rbuf_req_info),
    .rbuf_req_picked_me_i               (rbuf_req_picked_me_i),
    .pipe_rbuf_state_i                  (new_rbuf_state),
    .pipe_rbuf_clear_req_info_i         (new_rbuf_clear_req),

    .to_l3_mesh_clk_en_o               (),
    .to_l3_mesh_req_eligible_o          (new_to_l3_mesh_req_eligible_o),
    .to_l3_mesh_req_info_o              (new_to_l3_mesh_req_info),
    .to_l3_mesh_req_picked_i            (to_l3_mesh_req_picked_i),
    .to_l3_mesh_rsp_valid_me_i          (to_l3_mesh_rsp_valid_me_i),
    .to_l3_mesh_rsp_valid_any_i         (to_l3_mesh_rsp_valid_any_i),
    .to_l3_mesh_rsp_info_i              (new_to_l3_mesh_rsp_info),

    .to_sys_mesh_clk_en_o               (),
    .to_sys_mesh_req_eligible_o          (new_to_sys_mesh_req_eligible_o),
    .to_sys_mesh_req_info_o              (new_to_sys_mesh_req_info),
    .to_sys_mesh_req_picked_i            (to_sys_mesh_req_picked_i),
    .to_sys_mesh_rsp_valid_me_i          (to_sys_mesh_rsp_valid_me_i),
    .to_sys_mesh_rsp_valid_any_i         (to_sys_mesh_rsp_valid_any_i),
    .to_sys_mesh_rsp_info_i              (new_to_sys_mesh_rsp_info),

    .mesh_bypassed_victim_i              (mesh_bypassed_victim_i),

    .rspmux_l2_eligible_o                (new_rspmux_l2_eligible_o),
    .rspmux_l2_info_o                    (new_rspmux_l2_info),
    .rspmux_l2_picked_i                  (rspmux_l2_picked_i),

    .rspmux_l3_eligible_o                (new_rspmux_l3_eligible_o),
    .rspmux_l3_info_o                    (new_rspmux_l3_info),
    .rspmux_l3_picked_i                  (rspmux_l3_picked_i),

    .rspmux_bypassed_data_rsp_i          (rspmux_bypassed_data_rsp_i),
    .rspmux_sent_valid_i                 (rspmux_sent_valid_i),

    .reqq_2nd_pass_need_priority_i       (reqq_2nd_pass_need_priority_i),
    .reqq_idx_cop_need_priority_i        (reqq_idx_cop_need_priority_i),

    .pipe_idx_cop_aborted_i              (pipe_idx_cop_aborted_i),

    .reqq_entry_valid_o                  (new_reqq_entry_valid_o),
    .reqq_entry_state_o                  (new_entry_state),

    .reqq_entry_alloc_o                  (new_reqq_entry_alloc_o),
    .reqq_entry_dealloc_o                (new_reqq_entry_dealloc_o),

    .set_reqq_err_detected_me_i          (set_reqq_err_detected_me_i),
    .set_reqq_err_zero_data_me_i         (set_reqq_err_zero_data_me_i),

    .esr_ctl_i                           (new_esr_ctl),
    .esr_sc_l3_shire_swizzle_ctl_i       (new_swizzle_ctl),

    .dft_i                               (new_dft)
  );

  assign new_opcode_o = new_entry_state.opcode;

  // ═══════════════════════════════════════════════════════
  // Original module instance
  // ═══════════════════════════════════════════════════════

  sc_reqq_pipe_req_t     orig_pipe_req_info;
  sc_pipe_rbuf_rd_req_t  orig_rbuf_req_info;
  sc_reqq_mesh_req_t     orig_to_l3_mesh_req_info;
  sc_reqq_mesh_req_t     orig_to_sys_mesh_req_info;
  sc_rspmux_t            orig_rspmux_l2_info;
  sc_rspmux_t            orig_rspmux_l3_info;
  sc_reqq_entry_state_t  orig_entry_state;

  shire_cache_bank_reqq_entry u_orig (
    .fclock                              (clk_i),
    .reset_in                            (reset),
    .my_reqq_id                          (my_reqq_id_i),

    .alloc_me_ag                         (alloc_me_ag_i),
    .alloc_info_ag                       (orig_alloc_info),
    .alloc_any_ag                        (alloc_any_ag_i),
    .alloc_any_info_ag                   (orig_alloc_info),
    .coalesce_victim_match_alloc_ag      (coalesce_victim_match_alloc_ag_i),

    .alloc_me_ad                         (alloc_me_ad_i),

    .clear_dep_behind_me                 (orig_clear_dep_behind_me_o),
    .clear_dep_hot                       (clear_dep_hot_i),
    .clear_dep_rbuf_1hot_pre             (clear_dep_rbuf_1hot_pre_i),

    .tag_victim_dep_valid                (tag_victim_dep_valid_i),
    .tag_victim_dep_id                   (tag_victim_dep_id_i),
    .tag_victim_dep_tail                 (tag_victim_dep_tail_i),
    .tag_victim_insert_behind_me         (tag_victim_insert_behind_me_i),
    .tag_victim_insert_ahead_of_me       (tag_victim_insert_ahead_of_me_i),
    .tag_victim_upgrade_behind_me        (tag_victim_upgrade_behind_me_i),
    .tag_victim_address_match            (tag_victim_address_match_i),
    .upgrade_me_to_meshll                (upgrade_me_to_meshll_i),
    .upgrade_ahead_of_me_to_meshll       (upgrade_ahead_of_me_to_meshll_i),

    .coalesce_victim_me                  (coalesce_victim_me_i),
    .coalesce_victim_dep_valid           (coalesce_victim_dep_valid_i),
    .coalesce_victim_dep_id              (coalesce_victim_dep_id_i),
    .coalesce_victim_dep_non_meshll_head (coalesce_victim_dep_non_meshll_head_i),
    .coalesce_victim_insert_behind_me    (coalesce_victim_insert_behind_me_i),

    .dataq_write_status_any              (dataq_write_status_any_i),
    .dataq_write_status_me               (dataq_write_status_me_i),
    .dataq_write_status_id               (dataq_write_status_id_i),
    .dataq_read_status_me                (dataq_read_status_me_i),

    .pipe_req_eligible                   (orig_pipe_req_eligible_o),
    .pipe_req_info_me                    (orig_pipe_req_info),
    .pipe_req_picked_me                  (pipe_req_picked_me_i),
    .pipe_req_reqq_id_any                (pipe_req_reqq_id_any_i),
    .pipe_req_picked_any                 (pipe_req_picked_any_i),

    .pipe_tag_rsp_valid_any              (pipe_tag_rsp_valid_any_i),
    .pipe_tag_rsp_valid_me               (pipe_tag_rsp_valid_me_i),
    .pipe_tag_rsp_info                   (orig_pipe_tag_rsp_info),
    .pipe_data_rsp_valid_me              (pipe_data_rsp_valid_me_i),
    .pipe_data_rsp_info                  (orig_pipe_data_rsp_info),
    .pipe_cbuf_rsp_valid_any             (pipe_cbuf_rsp_valid_any_i),
    .pipe_cbuf_rsp_info                  (orig_pipe_cbuf_rsp_info),

    .rbuf_req_eligible                   (orig_rbuf_req_eligible_o),
    .rbuf_req_info_me                    (orig_rbuf_req_info),
    .rbuf_req_picked_me                  (rbuf_req_picked_me_i),
    .pipe_rbuf_state                     (orig_rbuf_state),
    .pipe_rbuf_clear_req_info            (orig_rbuf_clear_req),

    .to_l3_mesh_clk_en                   (),
    .to_l3_mesh_req_eligible             (orig_to_l3_mesh_req_eligible_o),
    .to_l3_mesh_req_info                 (orig_to_l3_mesh_req_info),
    .to_l3_mesh_req_picked               (to_l3_mesh_req_picked_i),
    .to_l3_mesh_rsp_valid_me             (to_l3_mesh_rsp_valid_me_i),
    .to_l3_mesh_rsp_valid_any            (to_l3_mesh_rsp_valid_any_i),
    .to_l3_mesh_rsp_info                 (orig_to_l3_mesh_rsp_info),

    .to_sys_mesh_clk_en                  (),
    .to_sys_mesh_req_eligible            (orig_to_sys_mesh_req_eligible_o),
    .to_sys_mesh_req_info                (orig_to_sys_mesh_req_info),
    .to_sys_mesh_req_picked              (to_sys_mesh_req_picked_i),
    .to_sys_mesh_rsp_valid_me            (to_sys_mesh_rsp_valid_me_i),
    .to_sys_mesh_rsp_valid_any           (to_sys_mesh_rsp_valid_any_i),
    .to_sys_mesh_rsp_info                (orig_to_sys_mesh_rsp_info),

    .mesh_bypassed_victim                (mesh_bypassed_victim_i),

    .rspmux_l2_eligible                  (orig_rspmux_l2_eligible_o),
    .rspmux_l2_info                      (orig_rspmux_l2_info),
    .rspmux_l2_picked                    (rspmux_l2_picked_i),

    .rspmux_l3_eligible                  (orig_rspmux_l3_eligible_o),
    .rspmux_l3_info                      (orig_rspmux_l3_info),
    .rspmux_l3_picked                    (rspmux_l3_picked_i),

    .rspmux_bypassed_data_rsp            (rspmux_bypassed_data_rsp_i),
    .rspmux_sent_valid                   (rspmux_sent_valid_i),

    .reqq_2nd_pass_need_priority         (reqq_2nd_pass_need_priority_i),
    .reqq_idx_cop_need_priority          (reqq_idx_cop_need_priority_i),

    .pipe_idx_cop_aborted                (pipe_idx_cop_aborted_i),

    .reqq_entry_valid                    (orig_reqq_entry_valid_o),
    .reqq_entry_state                    (orig_entry_state),

    .reqq_entry_alloc                    (orig_reqq_entry_alloc_o),
    .reqq_entry_dealloc                  (orig_reqq_entry_dealloc_o),

    .set_reqq_err_detected_me            (set_reqq_err_detected_me_i),
    .set_reqq_err_zero_data_me           (set_reqq_err_zero_data_me_i),

    .esr_ctl                             (orig_esr_ctl),
    .esr_sc_l3_shire_swizzle_ctl         (orig_swizzle_ctl),

    .dft__reset_byp_hv                   (1'b0),
    .dft__reset_hv                       (1'b0)
  );

  assign orig_opcode_o = orig_entry_state.opcode;

endmodule : cosim_reqq_entry_tb
