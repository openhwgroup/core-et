// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_cbuf vs new shirecache_pipe_cbuf.
//
// SV wrapper that instantiates both modules with adapted interfaces.
// Scalar ports for easy C++ driving.

`include "soc.vh"

module cosim_cbuf_tb
  import shirecache_pkg::*;
(
  input  logic                      clk_i,
  input  logic                      rst_ni,

  // Shared inputs
  input  logic                      invalidate_req_valid_i,
  input  logic                      req_valid_i,
  input  logic [2:0]                req_opcode_i,
  input  logic                      req_rsp_required_i,
  input  logic [PaSize-1:0]         req_install_address_i,
  input  logic [PaSize-1:0]         req_clear_address_i,
  input  logic [ReqqIdSize-1:0]     req_reqq_id_i,
  input  logic [CbufSize-1:0]       esr_cbuf_entry_enable_i,
  input  logic [BankIdSize-1:0]     my_bank_id_i,

  // New module outputs
  output logic                      new_rsp_valid_o,
  output logic                      new_rsp_victim_o,
  output logic [PaSize-1:0]         new_rsp_victim_address_o,
  output logic [CbufSize-1:0]       new_state_valid_o,

  // Original module outputs
  output logic                      orig_rsp_valid_o,
  output logic                      orig_rsp_victim_o,
  output logic [PaSize-1:0]         orig_rsp_victim_address_o,
  output logic [CbufSize-1:0]       orig_state_valid_o
);

  // Active-high synchronous reset for original
  wire reset = !rst_ni;

  // ── New module ────────────────────────────────────────
  cbuf_req_t new_req;
  assign new_req.trans_id        = '0;
  assign new_req.reqq_id         = req_reqq_id_i;
  assign new_req.rsp_required    = req_rsp_required_i;
  assign new_req.opcode          = cbuf_opcode_e'(req_opcode_i);
  assign new_req.install_address = req_install_address_i;
  assign new_req.clear_address   = req_clear_address_i;

  cbuf_rsp_t   new_rsp;
  cbuf_state_t new_state;

  assign new_rsp_valid_o          = new_rsp_valid_raw;
  assign new_rsp_victim_o         = new_rsp.victim;
  assign new_rsp_victim_address_o = new_rsp.victim_address;
  assign new_state_valid_o        = new_state.cbuf_mem_valid;

  logic new_rsp_valid_raw;
  perfmon_req_info_t pm_req;
  assign pm_req = '0;
  perfmon_rsp_info_t pm_rsp;

  shirecache_pipe_cbuf u_new (
    .clk_i                   (clk_i),
    .rst_ni                  (rst_ni),
    .invalidate_req_valid_i  (invalidate_req_valid_i),
    .req_valid_i             (req_valid_i),
    .req_info_i              (new_req),
    .rsp_valid_o             (new_rsp_valid_raw),
    .rsp_info_o              (new_rsp),
    .state_o                 (new_state),
    .perfmon_req_info_i      (pm_req),
    .perfmon_rsp_info_o      (pm_rsp),
    .esr_cbuf_entry_enable_i (esr_cbuf_entry_enable_i),
    .my_bank_id_i            (my_bank_id_i)
  );

  // ── Original module ───────────────────────────────────
  sc_pipe_cbuf_req_t orig_req;
  assign orig_req.trans_id        = '0;
  assign orig_req.reqq_id         = req_reqq_id_i;
  assign orig_req.rsp_required    = req_rsp_required_i;
  assign orig_req.opcode          = sc_cbuf_opcode_t'(req_opcode_i);
  assign orig_req.install_address = req_install_address_i;
  assign orig_req.clear_address   = req_clear_address_i;

  sc_pipe_cbuf_rsp_t   orig_rsp;
  sc_pipe_cbuf_state_t orig_state;

  logic orig_rsp_valid_raw;
  assign orig_rsp_valid_o          = orig_rsp_valid_raw;
  assign orig_rsp_victim_o         = orig_rsp.victim;
  assign orig_rsp_victim_address_o = orig_rsp.victim_address;
  assign orig_state_valid_o        = orig_state.cbuf_mem_valid;

  sc_perfmon_req_info_t orig_pm_req;
  assign orig_pm_req = '0;
  sc_perfmon_rsp_info_t orig_pm_rsp;

  shire_cache_pipe_cbuf u_orig (
    .clock                          (clk_i),
    .reset                          (reset),
    .pipe_cbuf_invalidate_req_valid (invalidate_req_valid_i),
    .pipe_cbuf_req_valid            (req_valid_i),
    .pipe_cbuf_req_info             (orig_req),
    .pipe_cbuf_rsp_valid            (orig_rsp_valid_raw),
    .pipe_cbuf_rsp_info             (orig_rsp),
    .pipe_cbuf_state_info           (orig_state),
    .perfmon_req_info               (orig_pm_req),
    .perfmon_rsp_info               (orig_pm_rsp),
    .esr_sc_cbuf_entry_enable       (esr_cbuf_entry_enable_i),
    .my_bank_id                     (my_bank_id_i)
  );

endmodule : cosim_cbuf_tb
