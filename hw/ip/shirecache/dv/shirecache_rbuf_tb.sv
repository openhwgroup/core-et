// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_pipe_rbuf.
// Breaks packed struct ports into scalars for easy C++ driving.

// TB exposes only selected struct fields; unused fields and perfmon expected.
/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields and perfmon intentionally unused
module shirecache_rbuf_tb
  import shirecache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // Simplified interfaces
  input  logic                        invalidate_i,
  input  logic                        install_valid_i,
  input  logic                        install_scp_i,
  input  logic [PaSize-1:0]           install_address_i,

  input  logic                        evict_valid_i,
  input  logic                        evict_scp_i,
  input  logic [PaSize-1:0]           evict_address_i,

  input  logic                        data_valid_i,
  input  logic                        data_scp_i,
  input  logic [PaSize-1:0]           data_address_i,
  input  logic [LineDataSize-1:0]     data_data_i,

  // Clear — simplified: just the hot vectors
  input  logic [RbufSize-1:0]         clear_mem_hot_i,
  input  logic [RbufSize-1:0]         clear_pend_hot_i,

  // Rd req
  input  logic                        rd_req_valid_i,
  input  logic [RbufIdSize-1:0]       rd_req_rbuf_id_i,
  output logic                        rd_req_ready_o,

  // Rd rsp
  output logic                        rd_rsp_valid_o,
  input  logic                        rd_rsp_ready_i,
  output logic [LineDataSize-1:0]     rd_rsp_data_o,

  // State (selected fields)
  output logic [RbufSize-1:0]         state_valid_o,
  output logic                        state_evict_o,
  output logic                        state_pending_install_o,

  // Config
  input  logic [RamDelaySize-1:0]     esr_ram_delay_i,
  input  logic                        esr_l2_rbuf_enable_i,
  input  logic [BankIdSize-1:0]       my_bank_id_i
);

  rbuf_install_req_t install_req;
  assign install_req.trans_id = '0;
  assign install_req.reqq_id  = '0;
  assign install_req.scp      = install_scp_i;
  assign install_req.address  = install_address_i;

  rbuf_evict_req_t evict_req;
  assign evict_req.trans_id = '0;
  assign evict_req.reqq_id  = '0;
  assign evict_req.scp      = evict_scp_i;
  assign evict_req.address  = evict_address_i;

  rbuf_data_req_t data_req;
  assign data_req.trans_id = '0;
  assign data_req.reqq_id  = '0;
  assign data_req.scp      = data_scp_i;
  assign data_req.opcode   = L2Read;
  assign data_req.address  = data_address_i;
  assign data_req.data     = data_data_i;

  rbuf_clear_req_t clear_req;
  assign clear_req.rbuf_clear_mem_valid_hot            = clear_mem_hot_i;
  assign clear_req.rbuf_clear_mem_scp                  = '0;
  assign clear_req.rbuf_clear_mem_address              = '0;
  assign clear_req.rbuf_clear_pending_install_hot      = clear_pend_hot_i;
  assign clear_req.rbuf_clear_pending_install_scp      = '0;
  assign clear_req.rbuf_clear_pending_install_address   = '0;

  rbuf_rd_req_t rd_req;
  assign rd_req.trans_id  = '0;
  assign rd_req.reqq_id   = '0;
  assign rd_req.l3_source = '0;
  assign rd_req.scp       = '0;
  assign rd_req.address   = '0;
  assign rd_req.rbuf_id   = rd_req_rbuf_id_i;

  rbuf_rd_rsp_t rd_rsp;
  rbuf_state_t  state;

  assign state_valid_o           = state.rbuf_mem_valid;
  assign state_evict_o           = state.rbuf_evict;
  assign state_pending_install_o = state.rbuf_pending_install;
  assign rd_rsp_data_o           = rd_rsp.data;

  perfmon_req_info_t pm_req;
  assign pm_req = '0;
  perfmon_rsp_info_t pm_rsp;

  shirecache_pipe_rbuf u_dut (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .invalidate_req_valid_i (invalidate_i),
    .install_req_valid_i(install_valid_i),
    .install_req_info_i (install_req),
    .evict_req_valid_i  (evict_valid_i),
    .evict_req_info_i   (evict_req),
    .data_req_valid_i   (data_valid_i),
    .data_req_info_i    (data_req),
    .clear_req_info_i   (clear_req),
    .rd_req_ready_o     (rd_req_ready_o),
    .rd_req_valid_i     (rd_req_valid_i),
    .rd_req_info_i      (rd_req),
    .rd_rsp_ready_i     (rd_rsp_ready_i),
    .rd_rsp_valid_o     (rd_rsp_valid_o),
    .rd_rsp_info_o      (rd_rsp),
    .state_o            (state),
    .perfmon_req_info_i (pm_req),
    .perfmon_rsp_info_o (pm_rsp),
    .esr_ram_delay_i    (esr_ram_delay_i),
    .esr_l2_rbuf_enable_i  (esr_l2_rbuf_enable_i),
    .esr_scp_rbuf_enable_i (1'b0),
    .my_bank_id_i       (my_bank_id_i)
  );

endmodule : shirecache_rbuf_tb
