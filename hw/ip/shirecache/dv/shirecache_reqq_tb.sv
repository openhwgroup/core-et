// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_bank_reqq.
// Breaks packed struct ports into scalars for easy C++ driving.
// Ties off most inputs to zero; focuses on bank_l2 path only.

/* verilator lint_off UNUSEDSIGNAL */  // TB exposes subset of struct fields; rest unused
/* verilator lint_off UNOPTFLAT */  // packed struct fields from separate combinational paths
/* verilator lint_off UNUSEDPARAM */  // package constants not all used in TB
/* verilator lint_off PINCONNECTEMPTY */  // intentionally unconnected outputs in simplified TB
/* verilator lint_off SYNCASYNCNET */  // rst_n used in both async reset and clock gate enable
/* verilator lint_off WIDTHCONCAT */  // large struct array zeroing
/* verilator lint_off VARHIDDEN */
module shirecache_reqq_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
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

  // ── Dataq write status ───────────────────────────────────
  input  logic                        dataq_write_status_any_i,
  input  logic [NumEntries-1:0]       dataq_write_status_hot_i,
  input  logic [NumEntriesId-1:0]     dataq_write_status_id_i,

  // ── Pipeline ─────────────────────────────────────────────
  input  logic [SubBanks-1:0]         pipe_busy_sub_bank_i,
  input  logic                        pipe_squash_i,

  // ── Tag response (flat) ──────────────────────────────────
  input  logic                        pipe_tag_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]       pipe_tag_rsp_reqq_id_i,
  input  logic                        pipe_tag_rsp_hit_i,
  input  logic [5:0]                  pipe_tag_rsp_opcode_i,

  // ── Data response (flat) ─────────────────────────────────
  input  logic                        pipe_data_rsp_valid_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_reqq_id_i,
  input  logic                        pipe_data_rsp_rsp_type_i,
  input  logic [LineQwSize-1:0]       pipe_data_rsp_qwen_i,

  // ── Data rsp pre (flat) ──────────────────────────────────
  input  logic                        pipe_data_rsp_pre_valid_i,
  input  logic [ReqqIdSize-1:0]       pipe_data_rsp_pre_reqq_id_i,
  input  logic                        pipe_data_rsp_pre_rsp_type_i,

  // ── Rspmux sent ──────────────────────────────────────────
  input  logic [NumEntries-1:0]       rspmux_sent_valid_hot_i,

  // ── Key outputs ──────────────────────────────────────────
  output logic                        bank_l2_ready_o,
  output logic                        pipe_start_o,
  output logic [NumEntriesId-1:0]     pipe_req_reqq_id_o,
  output logic                        a_reqq_entry_is_valid_o,
  output logic [NumEntriesId-1:0]     reqq_alloc_l2_id_o,
  output logic                        rspmux_l2_valid_nodata_o,
  output logic                        rspmux_l2_valid_wdata_o
);

  // Build struct inputs from scalars
  reqq_bank_req_t bank_l2_info;
  always_comb begin
    bank_l2_info           = '0;
    bank_l2_info.opcode    = etlink_pkg::req_opcode_e'(bank_l2_opcode_i);
    bank_l2_info.subopcode = bank_l2_subopcode_i;
    bank_l2_info.address   = bank_l2_address_i;
    bank_l2_info.qwen      = bank_l2_qwen_i;
    bank_l2_info.size      = sc_size_t'(bank_l2_size_i);
    bank_l2_info.port_id   = bank_l2_port_id_i;
    bank_l2_info.source    = bank_l2_source_i;
    bank_l2_info.trans_id  = bank_l2_trans_id_i;
    bank_l2_info.write_is_partial = bank_l2_write_is_partial_i;
    bank_l2_info.qos       = bank_l2_qos_i;
    bank_l2_info.id        = bank_l2_id_i;
    bank_l2_info.wdata     = bank_l2_wdata_i;
  end

  pipe_busy_t pipe_busy;
  always_comb begin
    pipe_busy = '0;
    pipe_busy.sub_bank_busy = pipe_busy_sub_bank_i;
  end

  tag_rsp_t pipe_tag_rsp_info;
  always_comb begin
    pipe_tag_rsp_info         = '0;
    pipe_tag_rsp_info.reqq_id = pipe_tag_rsp_reqq_id_i;
    pipe_tag_rsp_info.hit     = pipe_tag_rsp_hit_i;
    pipe_tag_rsp_info.opcode  = reqq_opcode_e'(pipe_tag_rsp_opcode_i);
  end

  data_rsp_t pipe_data_rsp_info;
  always_comb begin
    pipe_data_rsp_info          = '0;
    pipe_data_rsp_info.reqq_id  = pipe_data_rsp_reqq_id_i;
    pipe_data_rsp_info.rsp_type = pipe_data_rsp_type_e'(pipe_data_rsp_rsp_type_i);
    pipe_data_rsp_info.qwen     = pipe_data_rsp_qwen_i;
  end

  data_rsp_pre_t pipe_data_rsp_pre_info;
  always_comb begin
    pipe_data_rsp_pre_info          = '0;
    pipe_data_rsp_pre_info.reqq_id  = pipe_data_rsp_pre_reqq_id_i;
    pipe_data_rsp_pre_info.rsp_type = pipe_data_rsp_type_e'(pipe_data_rsp_pre_rsp_type_i);
  end

  // Tie-offs
  reqq_bank_req_t            bank_l3_info_tieoff;
  assign bank_l3_info_tieoff = '0;
  idx_cop_req_t              idx_cop_req_tieoff;
  assign idx_cop_req_tieoff  = '0;
  cbuf_rsp_t                 cbuf_rsp_tieoff;
  assign cbuf_rsp_tieoff     = '0;
  rbuf_state_t               rbuf_state_tieoff;
  assign rbuf_state_tieoff   = '0;
  reqq_mesh_rsp_t            mesh_rsp_tieoff;
  assign mesh_rsp_tieoff     = '0;
  perfmon_req_info_t         perfmon_req_tieoff;
  assign perfmon_req_tieoff  = '0;
  bank_esr_ctl_t             esr_ctl_tieoff;
  assign esr_ctl_tieoff      = '0;
  esr_shire_config_t         esr_config_tieoff;
  assign esr_config_tieoff   = '0;
  l3_swizzle_ctl_t           swizzle_tieoff;
  assign swizzle_tieoff      = '0;
  dft_t                      dft_tieoff;
  assign dft_tieoff          = '0;

  // DUT outputs
  reqq_pipe_req_t                       pipe_req_info;
  data_rsp_modifier_t                   data_rsp_modifier;
  rbuf_clear_req_t                      rbuf_clear_req;
  rbuf_rd_req_t                         rbuf_rd_req;
  reqq_mesh_req_t                       to_l3_mesh_req_info;
  reqq_mesh_req_t                       to_sys_mesh_req_info;
  rspmux_t                              rspmux_l2_info;
  rspmux_t                              rspmux_l3_info;
  reqq_entry_state_t [NumEntries-1:0]   reqq_state;
  reqq_alloc_t                          alloc_l2_info_ag;
  reqq_alloc_t                          alloc_l3_info_ag;
  reqq_alloc_t                          alloc_l2_info_ad;
  reqq_alloc_t                          alloc_l3_info_ad;
  perfmon_rsp_info_t                    perfmon_p0_rsp;
  perfmon_rsp_info_t                    perfmon_p1_rsp;

  shirecache_bank_reqq u_dut (
    .clk_i                                (clk_i),
    .rst_ni                               (rst_ni),
    .dft_i                                (dft_tieoff),

    .bank_l2_ready_o                      (bank_l2_ready_o),
    .bank_l2_valid_i                      (bank_l2_valid_i),
    .bank_l2_info_i                       (bank_l2_info),
    .reqq_alloc_l2_id_ag_o                (reqq_alloc_l2_id_o),
    .dataq_bank_l2_ready_i                (dataq_bank_l2_ready_i),

    .bank_l3_ready_o                      (),
    .bank_l3_valid_i                      (1'b0),
    .bank_l3_info_i                       (bank_l3_info_tieoff),
    .reqq_alloc_l3_id_ag_o                (),
    .dataq_bank_l3_ready_i                (1'b1),

    .alloc_l2_valid_ag_o                  (),
    .alloc_l2_info_ag_o                   (alloc_l2_info_ag),
    .alloc_l3_valid_ag_o                  (),
    .alloc_l3_info_ag_o                   (alloc_l3_info_ag),
    .alloc_l2_valid_ad_o                  (),
    .alloc_l2_info_ad_o                   (alloc_l2_info_ad),
    .alloc_l3_valid_ad_o                  (),
    .alloc_l3_info_ad_o                   (alloc_l3_info_ad),

    .set_reqq_err_detected_hot_i          ('0),
    .set_reqq_err_zero_data_hot_i         ('0),

    .pipe_idx_cop_aborted_i               (1'b0),
    .pipe_idx_cop_l2_busy_i               (1'b0),
    .pipe_idx_cop_l3_busy_i               (1'b0),
    .pipe_idx_cop_req_ready_o             (),
    .pipe_idx_cop_req_valid_i             (1'b0),
    .pipe_idx_cop_req_info_i              (idx_cop_req_tieoff),

    .dataq_write_status_any_i             (dataq_write_status_any_i),
    .dataq_write_status_hot_i             (dataq_write_status_hot_i),
    .dataq_write_status_id_i              (dataq_write_status_id_i),
    .dataq_read_status_hot_i              ('0),

    .pipe_start_o                         (pipe_start_o),
    .pipe_req_info_o                      (pipe_req_info),
    .pipe_busy_i                          (pipe_busy),
    .pipe_squash_i                        (pipe_squash_i),
    .pipe_tag_rsp_valid_i                 (pipe_tag_rsp_valid_i),
    .pipe_tag_rsp_info_i                  (pipe_tag_rsp_info),
    .pipe_data_rsp_valid_i                (pipe_data_rsp_valid_i),
    .pipe_data_rsp_info_i                 (pipe_data_rsp_info),
    .pipe_data_rsp_pre_valid_i            (pipe_data_rsp_pre_valid_i),
    .pipe_data_rsp_pre_info_i             (pipe_data_rsp_pre_info),
    .pipe_cbuf_rsp_valid_i                (1'b0),
    .pipe_cbuf_rsp_info_i                 (cbuf_rsp_tieoff),
    .mesh_bypassed_victim_i               (1'b0),
    .rspmux_bypassed_data_rsp_i           (1'b0),

    .pipe_data_rsp_modifier_info_o        (data_rsp_modifier),

    .pipe_rbuf_clear_req_info_o           (rbuf_clear_req),
    .pipe_rbuf_rd_req_valid_o             (),
    .pipe_rbuf_rd_req_info_o              (rbuf_rd_req),
    .pipe_rbuf_rd_req_ready_i             (1'b1),
    .pipe_rbuf_state_i                    (rbuf_state_tieoff),

    .to_l3_mesh_master_bank_clk_en_o      (),
    .reqq_to_l3_mesh_req_valid_nodata_o   (),
    .reqq_to_l3_mesh_req_ready_nodata_i   (1'b1),
    .reqq_to_l3_mesh_req_valid_wdata_o    (),
    .reqq_to_l3_mesh_req_ready_wdata_i    (1'b1),
    .reqq_to_l3_mesh_req_info_o           (to_l3_mesh_req_info),
    .to_l3_mesh_rsp_valid_i               (1'b0),
    .to_l3_mesh_rsp_info_i                (mesh_rsp_tieoff),

    .to_sys_mesh_master_bank_clk_en_o     (),
    .reqq_to_sys_mesh_req_valid_nodata_o  (),
    .reqq_to_sys_mesh_req_ready_nodata_i  (1'b1),
    .reqq_to_sys_mesh_req_valid_wdata_o   (),
    .reqq_to_sys_mesh_req_ready_wdata_i   (1'b1),
    .reqq_to_sys_mesh_req_info_o          (to_sys_mesh_req_info),
    .to_sys_mesh_rsp_valid_i              (1'b0),
    .to_sys_mesh_rsp_info_i               (mesh_rsp_tieoff),

    .reqq_rspmux_l2_valid_nodata_o        (rspmux_l2_valid_nodata_o),
    .reqq_rspmux_l2_ready_nodata_i        (1'b1),
    .reqq_rspmux_l2_valid_wdata_o         (rspmux_l2_valid_wdata_o),
    .reqq_rspmux_l2_ready_wdata_i         (1'b1),
    .reqq_rspmux_l2_info_o                (rspmux_l2_info),

    .reqq_rspmux_l3_valid_nodata_o        (),
    .reqq_rspmux_l3_ready_nodata_i        (1'b1),
    .reqq_rspmux_l3_valid_wdata_o         (),
    .reqq_rspmux_l3_ready_wdata_i         (1'b1),
    .reqq_rspmux_l3_info_o                (rspmux_l3_info),

    .rspmux_sent_valid_hot_i              (rspmux_sent_valid_hot_i),

    .reqq_state_o                         (reqq_state),
    .a_reqq_entry_is_valid_o              (a_reqq_entry_is_valid_o),

    .mbist_on_i                           (1'b0),

    .perfmon_p0_req_info_i                (perfmon_req_tieoff),
    .perfmon_p0_rsp_info_o                (perfmon_p0_rsp),
    .perfmon_p1_req_info_i                (perfmon_req_tieoff),
    .perfmon_p1_rsp_info_o                (perfmon_p1_rsp),

    .esr_shire_config_i                   (esr_config_tieoff),
    .esr_ctl_i                            (esr_ctl_tieoff),
    .esr_sc_l3_shire_swizzle_ctl_i        (swizzle_tieoff)
  );

  assign pipe_req_reqq_id_o = pipe_req_info.reqq_id;

  // Unused signal suppression
  logic unused_ok;
  assign unused_ok = &{
    1'b0,
    pipe_req_info,
    data_rsp_modifier,
    rbuf_clear_req,
    rbuf_rd_req,
    to_l3_mesh_req_info,
    to_sys_mesh_req_info,
    rspmux_l2_info,
    rspmux_l3_info,
    reqq_state,
    alloc_l2_info_ag,
    alloc_l3_info_ag,
    alloc_l2_info_ad,
    alloc_l3_info_ad,
    perfmon_p0_rsp,
    perfmon_p1_rsp
  };

endmodule : shirecache_reqq_tb
