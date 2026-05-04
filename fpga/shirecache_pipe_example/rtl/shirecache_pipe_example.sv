// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache pipe example project top.
//
// Wraps shirecache_pipe with default parameters. Demonstrates
// synthesis of a complex pipeline module that instantiates
// sub_bank ×4, sub_bank_mem ×4, cbuf, rbuf, idx_cop_sm, and
// transitively uses most primitive IPs.

module shirecache_pipe_example
  import shirecache_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic                        rst_c_ni,

  // DFT
  input  dft_t                        dft_i,
  input  logic                        dft_sram_clk_i,

  // RAM configuration
  input  ram_cfg_t                    ram_cfg_i,

  // Request interface from reqq
  input  logic                        pipe_start_i,
  input  reqq_pipe_req_t              pipe_req_info_i,
  output logic                        pipe_active_o,
  output pipe_busy_t                  pipe_busy_o,
  output logic                        pipe_squash_o,

  // Response interfaces to reqq
  output logic                        pipe_tag_rsp_valid_o,
  output tag_rsp_t                    pipe_tag_rsp_info_o,

  output logic                        pipe_data_rsp_pre_valid_o,
  output data_rsp_pre_t               pipe_data_rsp_pre_info_o,

  output logic                        pipe_data_rsp_valid_o,
  output data_rsp_t                   pipe_data_rsp_info_o,

  output logic                        pipe_cbuf_rsp_valid_o,
  output cbuf_rsp_t                   pipe_cbuf_rsp_info_o,

  // Read buffer clear request from reqq
  input  rbuf_clear_req_t             pipe_rbuf_clear_req_info_i,

  // Read buffer request from reqq
  output logic                        pipe_rbuf_rd_req_ready_o,
  input  logic                        pipe_rbuf_rd_req_valid_i,
  input  rbuf_rd_req_t                pipe_rbuf_rd_req_info_i,

  // Read buffer data response to reqq/mesh
  input  logic                        pipe_rbuf_rd_rsp_ready_i,
  output logic                        pipe_rbuf_rd_rsp_valid_o,
  output rbuf_rd_rsp_t                pipe_rbuf_rd_rsp_info_o,

  // Read buffer status to reqq
  output rbuf_state_t                 pipe_rbuf_state_o,

  // Data interface to dataq
  output logic                        pipe_dataq_rd_req_valid_o,
  output dataq_rd_req_t               pipe_dataq_rd_req_info_o,
  input  dataq_rd_rsp_t               pipe_dataq_rd_rsp_info_i,

  // Request interface to reqq (idx_cop)
  output logic                        pipe_idx_cop_aborted_o,
  output logic                        pipe_idx_cop_l2_busy_o,
  output logic                        pipe_idx_cop_l3_busy_o,
  input  logic                        pipe_idx_cop_req_ready_i,
  output logic                        pipe_idx_cop_req_valid_o,
  output idx_cop_req_t                pipe_idx_cop_req_info_o,
  output logic                        pipe_idx_cop_is_active_o,

  // BIST
  input  logic                        mbist_on_i,
  input  bist_req_t                   pipe_bist_req_info_i,
  output bist_rsp_t                   pipe_bist_rsp_info_o,

  // Performance monitor
  input  perfmon_req_info_t           perfmon_req_info_i,
  output perfmon_rsp_info_t           perfmon_rsp_info_o,

  // ESRs and constants
  input  bank_esr_ctl_t               esr_ctl_i,
  output pipe_esr_status_t            pipe_esr_status_o,
  output sbe_dbe_overflow_t           pipe_sbe_dbe_overflow_o,
  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [L3ShireIdSize-1:0]    my_l3_virtual_shire_id_i,
  input  logic [ScpShireIdSize-1:0]   my_scp_virtual_shire_id_i
);

  shirecache_pipe u_pipe (
    .clk_i,
    .rst_ni,
    .rst_c_ni,
    .dft_i,
    .dft_sram_clk_i,
    .ram_cfg_i,
    .pipe_start_i,
    .pipe_req_info_i,
    .pipe_active_o,
    .pipe_busy_o,
    .pipe_squash_o,
    .pipe_tag_rsp_valid_o,
    .pipe_tag_rsp_info_o,
    .pipe_data_rsp_pre_valid_o,
    .pipe_data_rsp_pre_info_o,
    .pipe_data_rsp_valid_o,
    .pipe_data_rsp_info_o,
    .pipe_cbuf_rsp_valid_o,
    .pipe_cbuf_rsp_info_o,
    .pipe_rbuf_clear_req_info_i,
    .pipe_rbuf_rd_req_ready_o,
    .pipe_rbuf_rd_req_valid_i,
    .pipe_rbuf_rd_req_info_i,
    .pipe_rbuf_rd_rsp_ready_i,
    .pipe_rbuf_rd_rsp_valid_o,
    .pipe_rbuf_rd_rsp_info_o,
    .pipe_rbuf_state_o,
    .pipe_dataq_rd_req_valid_o,
    .pipe_dataq_rd_req_info_o,
    .pipe_dataq_rd_rsp_info_i,
    .pipe_idx_cop_aborted_o,
    .pipe_idx_cop_l2_busy_o,
    .pipe_idx_cop_l3_busy_o,
    .pipe_idx_cop_req_ready_i,
    .pipe_idx_cop_req_valid_o,
    .pipe_idx_cop_req_info_o,
    .pipe_idx_cop_is_active_o,
    .mbist_on_i,
    .pipe_bist_req_info_i,
    .pipe_bist_rsp_info_o,
    .perfmon_req_info_i,
    .perfmon_rsp_info_o,
    .esr_ctl_i,
    .pipe_esr_status_o,
    .pipe_sbe_dbe_overflow_o,
    .my_bank_id_i,
    .my_l3_virtual_shire_id_i,
    .my_scp_virtual_shire_id_i
  );

endmodule
