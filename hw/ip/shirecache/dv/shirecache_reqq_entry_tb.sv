// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_reqq_entry.
// Breaks packed struct ports into scalars for easy C++ driving.

// TB exposes only selected struct fields; unused fields expected.
/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields intentionally unused
module shirecache_reqq_entry_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic [ReqqIdSize-1:0]       my_reqq_id_i,

  // ── Allocation (flat scalars → reqq_alloc_t) ──────────────
  input  logic                        alloc_me_ag_i,
  input  logic [4:0]                  alloc_orig_opcode_i,
  input  logic [5:0]                  alloc_opcode_i,
  input  logic [2:0]                  alloc_address_space_i,
  input  logic [1:0]                  alloc_reqq_source_i,
  input  logic [PaSize-1:0]           alloc_address_i,
  input  logic [LineQwSize-1:0]       alloc_qwen_i,
  input  logic [2:0]                  alloc_size_i,
  input  logic [DvTransIdSize-1:0]    alloc_trans_id_i,
  input  logic                        alloc_zero_data_i,
  input  logic                        alloc_dep_valid_i,
  input  logic [ReqqIdSize-1:0]       alloc_dep_id_i,
  input  logic                        alloc_dep_non_meshll_head_i,
  input  logic                        alloc_err_valid_i,
  input  logic [4:0]                  alloc_err_type_i,
  input  logic                        alloc_err_imprecise_i,
  input  logic [ReqqIdSize-1:0]       alloc_paired_id_i,
  input  logic [1:0]                  alloc_cache_level_start_i,
  input  logic [1:0]                  alloc_cache_level_dest_i,

  // alloc_any — tied to alloc_me for single-entry test
  input  logic                        alloc_me_ad_i,

  // ── Dependency ────────────────────────────────────────────
  input  logic [ReqqDepth-1:0]        clear_dep_hot_i,
  input  logic [ReqqDepth-1:0]        clear_dep_rbuf_1hot_pre_i,

  // ── Pipeline interface ────────────────────────────────────
  input  logic                        pipe_req_picked_me_i,
  input  logic                        pipe_req_picked_any_i,
  input  logic [ReqqIdSize-1:0]       pipe_req_reqq_id_any_i,

  // ── Tag response (flat scalars → tag_rsp_t) ──────────────
  input  logic                        pipe_tag_rsp_valid_any_i,
  input  logic                        pipe_tag_rsp_valid_me_i,
  input  logic                        tag_rsp_hit_i,
  input  logic                        tag_rsp_victim_i,
  input  logic                        tag_rsp_victim_silent_i,
  input  logic [PaSize-1:0]           tag_rsp_victim_address_i,
  input  logic [2:0]                  tag_rsp_victim_address_space_i,
  input  logic [LineQwSize-1:0]       tag_rsp_victim_qwen_i,
  input  logic                        tag_rsp_bubble_i,
  input  logic [5:0]                  tag_rsp_opcode_i,
  input  logic [4:0]                  tag_rsp_err_rsp_i,

  // ── Data response (flat scalars → data_rsp_t) ────────────
  input  logic                        pipe_data_rsp_valid_me_i,
  input  logic                        data_rsp_type_i,         // 0=NormalRsp, 1=VictimData

  // ── Cbuf response ────────────────────────────────────────
  input  logic                        pipe_cbuf_rsp_valid_any_i,

  // ── Dataq status ─────────────────────────────────────────
  input  logic                        dataq_write_status_any_i,
  input  logic                        dataq_write_status_me_i,
  input  logic [ReqqIdSize-1:0]       dataq_write_status_id_i,
  input  logic                        dataq_read_status_me_i,

  // ── Mesh interface ────────────────────────────────────────
  input  logic                        to_l3_mesh_req_picked_i,
  input  logic                        to_l3_mesh_rsp_valid_me_i,
  input  logic                        to_l3_mesh_rsp_valid_any_i,
  input  logic                        to_sys_mesh_req_picked_i,
  input  logic                        to_sys_mesh_rsp_valid_me_i,
  input  logic                        to_sys_mesh_rsp_valid_any_i,
  input  logic                        mesh_bypassed_victim_i,

  // ── Rspmux interface ─────────────────────────────────────
  input  logic                        rspmux_l2_picked_i,
  input  logic                        rspmux_l3_picked_i,
  input  logic                        rspmux_bypassed_data_rsp_i,
  input  logic                        rspmux_sent_valid_i,

  // ── Other ─────────────────────────────────────────────────
  input  logic                        reqq_2nd_pass_need_priority_i,
  input  logic                        reqq_idx_cop_need_priority_i,
  input  logic                        pipe_idx_cop_aborted_i,

  // ── Rbuf interface (picked) ───────────────────────────────
  input  logic                        rbuf_req_picked_me_i,

  // ── Key outputs (scalars) ─────────────────────────────────
  output logic                        reqq_entry_valid_o,
  output logic [5:0]                  opcode_o,
  output logic                        pipe_req_eligible_o,
  output logic                        rbuf_req_eligible_o,
  output logic                        to_l3_mesh_req_eligible_o,
  output logic                        to_sys_mesh_req_eligible_o,
  output logic                        rspmux_l2_eligible_o,
  output logic                        rspmux_l3_eligible_o,
  output logic                        clear_dep_behind_me_o,
  output logic                        reqq_entry_alloc_o,
  output logic                        reqq_entry_dealloc_o
);

  // ── Build reqq_alloc_t from scalars ─────────────────────────
  reqq_alloc_t alloc_info;
  always_comb begin
    alloc_info                    = '0;
    alloc_info.reqq_id            = my_reqq_id_i;
    alloc_info.trans_id           = alloc_trans_id_i;
    alloc_info.orig_opcode        = reqq_orig_opcode_e'(alloc_orig_opcode_i);
    alloc_info.opcode             = reqq_opcode_e'(alloc_opcode_i);
    alloc_info.address_space      = address_space_e'(alloc_address_space_i);
    alloc_info.reqq_source        = reqq_source_e'(alloc_reqq_source_i);
    alloc_info.address            = alloc_address_i;
    alloc_info.qwen               = alloc_qwen_i;
    alloc_info.size               = sc_size_t'(alloc_size_i);
    alloc_info.zero_data          = alloc_zero_data_i;
    alloc_info.dep_valid          = alloc_dep_valid_i;
    alloc_info.dep_id             = alloc_dep_id_i;
    alloc_info.dep_non_meshll_head = alloc_dep_non_meshll_head_i;
    alloc_info.err_valid          = alloc_err_valid_i;
    alloc_info.err_type           = err_rsp_e'(alloc_err_type_i);
    alloc_info.err_imprecise      = alloc_err_imprecise_i;
    alloc_info.paired_id          = alloc_paired_id_i;
    alloc_info.cache_level_start  = cache_level_e'(alloc_cache_level_start_i);
    alloc_info.cache_level_dest   = cache_level_e'(alloc_cache_level_dest_i);
  end

  // ── Build tag_rsp_t from scalars ────���───────────────────────
  tag_rsp_t tag_rsp;
  always_comb begin
    tag_rsp                       = '0;
    tag_rsp.hit                   = tag_rsp_hit_i;
    tag_rsp.victim                = tag_rsp_victim_i;
    tag_rsp.victim_silent         = tag_rsp_victim_silent_i;
    tag_rsp.victim_address        = tag_rsp_victim_address_i;
    tag_rsp.victim_address_space  = address_space_e'(tag_rsp_victim_address_space_i);
    tag_rsp.victim_qwen           = tag_rsp_victim_qwen_i;
    tag_rsp.bubble                = tag_rsp_bubble_i;
    tag_rsp.opcode                = reqq_opcode_e'(tag_rsp_opcode_i);
    tag_rsp.err_rsp               = err_rsp_e'(tag_rsp_err_rsp_i);
  end

  // ── Build data_rsp_t from scalars ──────────���────────────────
  data_rsp_t data_rsp;
  always_comb begin
    data_rsp          = '0;
    data_rsp.rsp_type = pipe_data_rsp_type_e'(data_rsp_type_i);
  end

  // ── Build cbuf_rsp_t (all zeros — cbuf not tested here) ────
  cbuf_rsp_t cbuf_rsp;
  assign cbuf_rsp = '0;

  // ��─ Mesh response structs (zeroed — only valid bits matter) ─
  reqq_mesh_rsp_t to_l3_mesh_rsp_info;
  assign to_l3_mesh_rsp_info = '0;

  reqq_mesh_rsp_t to_sys_mesh_rsp_info;
  assign to_sys_mesh_rsp_info = '0;

  // ── Rbuf state / clear_req (tied off — rbuf tested elsewhere) ─
  rbuf_state_t     rbuf_state;
  rbuf_clear_req_t rbuf_clear_req;
  assign rbuf_state     = '0;
  assign rbuf_clear_req = '0;

  // ── ESR / DFT tie-offs ──────────────────────────────────────
  bank_esr_ctl_t    esr_ctl;
  l3_swizzle_ctl_t  swizzle_ctl;
  dft_t             dft;
  assign esr_ctl      = '0;
  assign swizzle_ctl  = '0;
  assign dft          = '0;

  // ── DUT outputs ─────────────────────────────────────────────
  reqq_entry_state_t  entry_state;
  reqq_pipe_req_t     pipe_req_info;
  rbuf_rd_req_t       rbuf_req_info;
  logic               to_l3_mesh_clk_en;
  reqq_mesh_req_t     to_l3_mesh_req_info;
  logic               to_sys_mesh_clk_en;
  reqq_mesh_req_t     to_sys_mesh_req_info;
  rspmux_t            rspmux_l2_info;
  rspmux_t            rspmux_l3_info;

  // ── DUT instantiation ───────────────────────────────────────
  shirecache_bank_reqq_entry u_dut (
    .clk_i                             (clk_i),
    .rst_ni                            (rst_ni),
    .my_reqq_id_i                      (my_reqq_id_i),

    // Allocation
    .alloc_me_ag_i                     (alloc_me_ag_i),
    .alloc_info_ag_i                   (alloc_info),
    .alloc_any_ag_i                    (alloc_me_ag_i),
    .alloc_any_info_ag_i               (alloc_info),
    .coalesce_victim_match_alloc_ag_i  (1'b0),
    .alloc_me_ad_i                     (alloc_me_ad_i),

    // Dependencies
    .clear_dep_behind_me_o             (clear_dep_behind_me_o),
    .clear_dep_hot_i                   (clear_dep_hot_i),
    .clear_dep_rbuf_1hot_pre_i         (clear_dep_rbuf_1hot_pre_i),

    // Victim dependency management (inactive)
    .tag_victim_dep_valid_i            (1'b0),
    .tag_victim_dep_id_i               ('0),
    .tag_victim_dep_tail_i             (1'b0),
    .tag_victim_insert_behind_me_i     (1'b0),
    .tag_victim_insert_ahead_of_me_i   (1'b0),
    .tag_victim_upgrade_behind_me_i    (1'b0),
    .tag_victim_address_match_i        (1'b0),
    .upgrade_me_to_meshll_i            (1'b0),
    .upgrade_ahead_of_me_to_meshll_i   (1'b0),

    // Coalescing (inactive)
    .coalesce_victim_me_i              (1'b0),
    .coalesce_victim_dep_valid_i       (1'b0),
    .coalesce_victim_dep_id_i          ('0),
    .coalesce_victim_dep_non_meshll_head_i (1'b0),
    .coalesce_victim_insert_behind_me_i(1'b0),

    // Dataq status
    .dataq_write_status_any_i          (dataq_write_status_any_i),
    .dataq_write_status_me_i           (dataq_write_status_me_i),
    .dataq_write_status_id_i           (dataq_write_status_id_i),
    .dataq_read_status_me_i            (dataq_read_status_me_i),

    // Pipeline
    .pipe_req_eligible_o               (pipe_req_eligible_o),
    .pipe_req_info_me_o                (pipe_req_info),
    .pipe_req_picked_me_i              (pipe_req_picked_me_i),
    .pipe_req_reqq_id_any_i            (pipe_req_reqq_id_any_i),
    .pipe_req_picked_any_i             (pipe_req_picked_any_i),

    // Tag response
    .pipe_tag_rsp_valid_any_i          (pipe_tag_rsp_valid_any_i),
    .pipe_tag_rsp_valid_me_i           (pipe_tag_rsp_valid_me_i),
    .pipe_tag_rsp_info_i               (tag_rsp),

    // Data response
    .pipe_data_rsp_valid_me_i          (pipe_data_rsp_valid_me_i),
    .pipe_data_rsp_info_i              (data_rsp),

    // Cbuf response
    .pipe_cbuf_rsp_valid_any_i         (pipe_cbuf_rsp_valid_any_i),
    .pipe_cbuf_rsp_info_i              (cbuf_rsp),

    // Rbuf
    .rbuf_req_eligible_o               (rbuf_req_eligible_o),
    .rbuf_req_info_me_o                (rbuf_req_info),
    .rbuf_req_picked_me_i              (rbuf_req_picked_me_i),
    .pipe_rbuf_state_i                 (rbuf_state),
    .pipe_rbuf_clear_req_info_i        (rbuf_clear_req),

    // L3 mesh
    .to_l3_mesh_clk_en_o              (to_l3_mesh_clk_en),
    .to_l3_mesh_req_eligible_o         (to_l3_mesh_req_eligible_o),
    .to_l3_mesh_req_info_o             (to_l3_mesh_req_info),
    .to_l3_mesh_req_picked_i           (to_l3_mesh_req_picked_i),
    .to_l3_mesh_rsp_valid_me_i         (to_l3_mesh_rsp_valid_me_i),
    .to_l3_mesh_rsp_valid_any_i        (to_l3_mesh_rsp_valid_any_i),
    .to_l3_mesh_rsp_info_i             (to_l3_mesh_rsp_info),

    // Sys mesh
    .to_sys_mesh_clk_en_o             (to_sys_mesh_clk_en),
    .to_sys_mesh_req_eligible_o        (to_sys_mesh_req_eligible_o),
    .to_sys_mesh_req_info_o            (to_sys_mesh_req_info),
    .to_sys_mesh_req_picked_i          (to_sys_mesh_req_picked_i),
    .to_sys_mesh_rsp_valid_me_i        (to_sys_mesh_rsp_valid_me_i),
    .to_sys_mesh_rsp_valid_any_i       (to_sys_mesh_rsp_valid_any_i),
    .to_sys_mesh_rsp_info_i            (to_sys_mesh_rsp_info),
    .mesh_bypassed_victim_i            (mesh_bypassed_victim_i),

    // Rspmux L2
    .rspmux_l2_eligible_o              (rspmux_l2_eligible_o),
    .rspmux_l2_info_o                  (rspmux_l2_info),
    .rspmux_l2_picked_i                (rspmux_l2_picked_i),

    // Rspmux L3
    .rspmux_l3_eligible_o              (rspmux_l3_eligible_o),
    .rspmux_l3_info_o                  (rspmux_l3_info),
    .rspmux_l3_picked_i                (rspmux_l3_picked_i),

    .rspmux_bypassed_data_rsp_i        (rspmux_bypassed_data_rsp_i),
    .rspmux_sent_valid_i               (rspmux_sent_valid_i),

    // Priority
    .reqq_2nd_pass_need_priority_i     (reqq_2nd_pass_need_priority_i),
    .reqq_idx_cop_need_priority_i      (reqq_idx_cop_need_priority_i),
    .pipe_idx_cop_aborted_i            (pipe_idx_cop_aborted_i),

    // State
    .reqq_entry_valid_o                (reqq_entry_valid_o),
    .reqq_entry_state_o                (entry_state),
    .reqq_entry_alloc_o                (reqq_entry_alloc_o),
    .reqq_entry_dealloc_o              (reqq_entry_dealloc_o),

    // Error
    .set_reqq_err_detected_me_i        (1'b0),
    .set_reqq_err_zero_data_me_i       (1'b0),

    // ESR / DFT
    .esr_ctl_i                         (esr_ctl),
    .esr_sc_l3_shire_swizzle_ctl_i     (swizzle_ctl),
    .dft_i                             (dft)
  );

  // ── Decompose key state outputs to scalars ──────────────────
  assign opcode_o = entry_state.opcode;

endmodule : shirecache_reqq_entry_tb
