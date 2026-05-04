// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_rbuf vs new shirecache_pipe_rbuf.

`include "soc.vh"

module cosim_rbuf_tb
  import shirecache_pkg::*;
(
  input  logic                      clk_i,
  input  logic                      rst_ni,

  // Shared inputs (scalar)
  input  logic                      invalidate_i,
  input  logic                      install_valid_i,
  input  logic                      install_scp_i,
  input  logic [`PA_SIZE-1:0]       install_address_i,
  input  logic [`SC_REQQ_ID_SIZE-1:0] install_reqq_id_i,

  input  logic                      evict_valid_i,
  input  logic                      evict_scp_i,
  input  logic [`PA_SIZE-1:0]       evict_address_i,

  input  logic                      data_valid_i,
  input  logic                      data_scp_i,
  input  logic [`PA_SIZE-1:0]       data_address_i,
  input  logic [`SC_LINE_DATA_SIZE-1:0] data_data_i,

  input  logic [`SC_RBUF_SIZE-1:0]  clear_mem_hot_i,
  input  logic [`SC_RBUF_SIZE-1:0]  clear_pend_hot_i,

  input  logic                      rd_req_valid_i,
  input  logic [`SC_RBUF_ID_SIZE-1:0] rd_req_rbuf_id_i,
  input  logic                      rd_rsp_ready_i,

  input  logic [`SC_RAM_DELAY_SIZE-1:0] esr_ram_delay_i,
  input  logic                      esr_l2_rbuf_enable_i,
  input  logic                      esr_scp_rbuf_enable_i,
  input  logic [`SC_BANK_ID_SIZE-1:0] my_bank_id_i,

  // New module outputs
  output logic                      new_rd_req_ready_o,
  output logic                      new_rd_rsp_valid_o,
  output logic [`SC_LINE_DATA_SIZE-1:0] new_rd_rsp_data_o,
  output logic [`SC_RBUF_SIZE-1:0]  new_state_valid_o,
  output logic                      new_state_evict_o,
  output logic [`SC_RBUF_ID_SIZE-1:0] new_state_evict_id_o,
  output logic [`PA_SIZE-1:0]       new_state_evict_addr_o,
  output logic                      new_state_pend_install_o,

  // Original module outputs
  output logic                      orig_rd_req_ready_o,
  output logic                      orig_rd_rsp_valid_o,
  output logic [`SC_LINE_DATA_SIZE-1:0] orig_rd_rsp_data_o,
  output logic [`SC_RBUF_SIZE-1:0]  orig_state_valid_o,
  output logic                      orig_state_evict_o,
  output logic [`SC_RBUF_ID_SIZE-1:0] orig_state_evict_id_o,
  output logic [`PA_SIZE-1:0]       orig_state_evict_addr_o,
  output logic                      orig_state_pend_install_o
);

  wire reset = !rst_ni;

  // ── Build structs for new module ──────────────────────
  rbuf_install_req_t new_install_req;
  assign new_install_req.trans_id = '0;
  assign new_install_req.reqq_id  = install_reqq_id_i;
  assign new_install_req.scp      = install_scp_i;
  assign new_install_req.address  = install_address_i;

  rbuf_evict_req_t new_evict_req;
  assign new_evict_req.trans_id = '0;
  assign new_evict_req.reqq_id  = '0;
  assign new_evict_req.scp      = evict_scp_i;
  assign new_evict_req.address  = evict_address_i;

  rbuf_data_req_t new_data_req;
  assign new_data_req.trans_id = '0;
  assign new_data_req.reqq_id  = '0;
  assign new_data_req.scp      = data_scp_i;
  assign new_data_req.opcode   = L2Read;
  assign new_data_req.address  = data_address_i;
  assign new_data_req.data     = data_data_i;

  rbuf_clear_req_t new_clear_req;
  assign new_clear_req.rbuf_clear_mem_valid_hot            = clear_mem_hot_i;
  assign new_clear_req.rbuf_clear_mem_scp                  = '0;
  assign new_clear_req.rbuf_clear_mem_address              = '0;
  assign new_clear_req.rbuf_clear_pending_install_hot      = clear_pend_hot_i;
  assign new_clear_req.rbuf_clear_pending_install_scp      = '0;
  assign new_clear_req.rbuf_clear_pending_install_address   = '0;

  rbuf_rd_req_t new_rd_req;
  assign new_rd_req.trans_id  = '0;
  assign new_rd_req.reqq_id   = '0;
  assign new_rd_req.l3_source = '0;
  assign new_rd_req.scp       = '0;
  assign new_rd_req.address   = '0;
  assign new_rd_req.rbuf_id   = rd_req_rbuf_id_i;

  rbuf_rd_rsp_t  new_rd_rsp;
  rbuf_state_t   new_state;
  perfmon_req_info_t new_pm_req;
  assign new_pm_req = '0;
  perfmon_rsp_info_t new_pm_rsp;

  assign new_rd_rsp_data_o       = new_rd_rsp.data;
  assign new_state_valid_o       = new_state.rbuf_mem_valid;
  assign new_state_evict_o       = new_state.rbuf_evict;
  assign new_state_evict_id_o    = new_state.rbuf_evict_id;
  assign new_state_evict_addr_o  = new_state.rbuf_evict_address;
  assign new_state_pend_install_o = new_state.rbuf_pending_install;

  shirecache_pipe_rbuf u_new (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .invalidate_req_valid_i (invalidate_i),
    .install_req_valid_i(install_valid_i),
    .install_req_info_i (new_install_req),
    .evict_req_valid_i  (evict_valid_i),
    .evict_req_info_i   (new_evict_req),
    .data_req_valid_i   (data_valid_i),
    .data_req_info_i    (new_data_req),
    .clear_req_info_i   (new_clear_req),
    .rd_req_ready_o     (new_rd_req_ready_o),
    .rd_req_valid_i     (rd_req_valid_i),
    .rd_req_info_i      (new_rd_req),
    .rd_rsp_ready_i     (rd_rsp_ready_i),
    .rd_rsp_valid_o     (new_rd_rsp_valid_o),
    .rd_rsp_info_o      (new_rd_rsp),
    .state_o            (new_state),
    .perfmon_req_info_i (new_pm_req),
    .perfmon_rsp_info_o (new_pm_rsp),
    .esr_ram_delay_i    (esr_ram_delay_i),
    .esr_l2_rbuf_enable_i (esr_l2_rbuf_enable_i),
    .esr_scp_rbuf_enable_i(esr_scp_rbuf_enable_i),
    .my_bank_id_i       (my_bank_id_i)
  );

  // ── Build structs for original module ─────────────────
  sc_pipe_rbuf_install_req_t orig_install_req;
  assign orig_install_req.trans_id = '0;
  assign orig_install_req.reqq_id  = install_reqq_id_i;
  assign orig_install_req.scp      = install_scp_i;
  assign orig_install_req.address  = install_address_i;

  sc_pipe_rbuf_evict_req_t orig_evict_req;
  assign orig_evict_req.trans_id = '0;
  assign orig_evict_req.reqq_id  = '0;
  assign orig_evict_req.scp      = evict_scp_i;
  assign orig_evict_req.address  = evict_address_i;

  sc_pipe_rbuf_data_req_t orig_data_req;
  assign orig_data_req.trans_id = '0;
  assign orig_data_req.reqq_id  = '0;
  assign orig_data_req.scp      = data_scp_i;
  assign orig_data_req.opcode   = SC_L2_Read;
  assign orig_data_req.address  = data_address_i;
  assign orig_data_req.data     = data_data_i;

  sc_pipe_rbuf_clear_req_t orig_clear_req;
  assign orig_clear_req.rbuf_clear_mem_valid_hot            = clear_mem_hot_i;
  assign orig_clear_req.rbuf_clear_mem_scp                  = '0;
  assign orig_clear_req.rbuf_clear_mem_address              = '0;
  assign orig_clear_req.rbuf_clear_pending_install_hot      = clear_pend_hot_i;
  assign orig_clear_req.rbuf_clear_pending_install_scp      = '0;
  assign orig_clear_req.rbuf_clear_pending_install_address   = '0;

  sc_pipe_rbuf_rd_req_t orig_rd_req;
  assign orig_rd_req.trans_id  = '0;
  assign orig_rd_req.reqq_id   = '0;
  assign orig_rd_req.l3_source = '0;
  assign orig_rd_req.scp       = '0;
  assign orig_rd_req.address   = '0;
  assign orig_rd_req.rbuf_id   = rd_req_rbuf_id_i;

  sc_pipe_rbuf_rd_rsp_t  orig_rd_rsp;
  sc_pipe_rbuf_state_t   orig_state;
  sc_perfmon_req_info_t orig_pm_req;
  assign orig_pm_req = '0;
  sc_perfmon_rsp_info_t orig_pm_rsp;

  assign orig_rd_rsp_data_o       = orig_rd_rsp.data;
  assign orig_state_valid_o       = orig_state.rbuf_mem_valid;
  assign orig_state_evict_o       = orig_state.rbuf_evict;
  assign orig_state_evict_id_o    = orig_state.rbuf_evict_id;
  assign orig_state_evict_addr_o  = orig_state.rbuf_evict_address;
  assign orig_state_pend_install_o = orig_state.rbuf_pending_install;

  shire_cache_pipe_rbuf u_orig (
    .clock                          (clk_i),
    .reset                          (reset),
    .pipe_rbuf_invalidate_req_valid (invalidate_i),
    .pipe_rbuf_install_req_valid    (install_valid_i),
    .pipe_rbuf_install_req_info     (orig_install_req),
    .pipe_rbuf_evict_req_valid      (evict_valid_i),
    .pipe_rbuf_evict_req_info       (orig_evict_req),
    .pipe_rbuf_data_req_valid       (data_valid_i),
    .pipe_rbuf_data_req_info        (orig_data_req),
    .pipe_rbuf_clear_req_info       (orig_clear_req),
    .pipe_rbuf_rd_req_ready         (orig_rd_req_ready_o),
    .pipe_rbuf_rd_req_valid         (rd_req_valid_i),
    .pipe_rbuf_rd_req_info          (orig_rd_req),
    .pipe_rbuf_rd_rsp_ready         (rd_rsp_ready_i),
    .pipe_rbuf_rd_rsp_valid         (orig_rd_rsp_valid_o),
    .pipe_rbuf_rd_rsp_info          (orig_rd_rsp),
    .pipe_rbuf_state                (orig_state),
    .perfmon_req_info               (orig_pm_req),
    .perfmon_rsp_info               (orig_pm_rsp),
    .esr_sc_ram_delay               (esr_ram_delay_i),
    .esr_sc_l2_rbuf_enable          (esr_l2_rbuf_enable_i),
    .esr_sc_scp_rbuf_enable         (esr_scp_rbuf_enable_i),
    .my_bank_id                     (my_bank_id_i)
  );

endmodule : cosim_rbuf_tb
