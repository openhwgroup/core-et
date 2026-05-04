// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_pipe_cbuf.
// Breaks packed struct ports into scalars for easy C++ driving.

// TB exposes only selected struct fields; unused fields and perfmon are expected.
/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields and perfmon intentionally unused
module shirecache_cbuf_tb
  import shirecache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // Simplified request interface
  input  logic                        invalidate_req_valid_i,
  input  logic                        req_valid_i,
  input  logic [2:0]                  req_opcode_i,
  input  logic                        req_rsp_required_i,
  input  logic [PaSize-1:0]           req_install_address_i,
  input  logic [PaSize-1:0]           req_clear_address_i,
  input  logic [ReqqIdSize-1:0]       req_reqq_id_i,

  // Response
  output logic                        rsp_valid_o,
  output logic                        rsp_victim_o,
  output logic [PaSize-1:0]           rsp_victim_address_o,
  output logic [ReqqIdSize-1:0]       rsp_reqq_id_o,

  // State
  output logic [CbufSize-1:0]         state_valid_o,

  // Config
  input  logic [CbufSize-1:0]         esr_cbuf_entry_enable_i,
  input  logic [BankIdSize-1:0]       my_bank_id_i
);

  cbuf_req_t  req_info;
  assign req_info.trans_id        = '0;
  assign req_info.reqq_id         = req_reqq_id_i;
  assign req_info.rsp_required    = req_rsp_required_i;
  assign req_info.opcode          = cbuf_opcode_e'(req_opcode_i);
  assign req_info.install_address = req_install_address_i;
  assign req_info.clear_address   = req_clear_address_i;

  cbuf_rsp_t   rsp_info;
  cbuf_state_t state_info;

  assign rsp_victim_o         = rsp_info.victim;
  assign rsp_victim_address_o = rsp_info.victim_address;
  assign rsp_reqq_id_o        = rsp_info.reqq_id;
  assign state_valid_o        = state_info.cbuf_mem_valid;

  perfmon_req_info_t perfmon_req;
  assign perfmon_req = '0;
  perfmon_rsp_info_t perfmon_rsp;

  shirecache_pipe_cbuf u_dut (
    .clk_i                   (clk_i),
    .rst_ni                  (rst_ni),
    .invalidate_req_valid_i  (invalidate_req_valid_i),
    .req_valid_i             (req_valid_i),
    .req_info_i              (req_info),
    .rsp_valid_o             (rsp_valid_o),
    .rsp_info_o              (rsp_info),
    .state_o                 (state_info),
    .perfmon_req_info_i      (perfmon_req),
    .perfmon_rsp_info_o      (perfmon_rsp),
    .esr_cbuf_entry_enable_i (esr_cbuf_entry_enable_i),
    .my_bank_id_i            (my_bank_id_i)
  );

endmodule : shirecache_cbuf_tb
