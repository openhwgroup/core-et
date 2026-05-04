// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache texsend.
//
// Replaces: dcache_texsend
//
// This translation currently implements the exact non-GFX_ENABLED behavior from
// the original RTL. The graphics-enabled path remains to be ported separately.

module minion_dcache_texsend
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                              clk_i,
  input  logic                              rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0] neigh_min_id_i,
  input  logic                              chicken_bit_dcache_i,
  output logic                              idle_o,
  output logic                              tex_send_ready_o,
  input  logic                              tex_send_valid_i,
  input  texsend_control_t                  tex_send_ctrl_i,
  input  logic                              l2_resp_valid_i,
  input  et_link_minion_rsp_info_t          l2_resp_i,
  input  logic                              l2_req_ready_i,
  output logic                              l2_req_valid_o,
  output et_link_minion_miss_req_info_t     l2_req_o,
  input  logic                              writeback_ready_i,
  output logic                              writeback_valid_o,
  output dcache_wb_req_t                    writeback_req_o,
  input  logic                              writeback_done_i,
  output logic [minion_dcache_pkg::DcacheDbgCsrTxsndSize-1:0]  csr_debug_signals_o
);

  logic req_state_unused;
  logic pull_state_unused;

  assign idle_o = 1'b1;
  assign tex_send_ready_o = 1'b1;
  assign l2_req_valid_o = 1'b0;
  assign l2_req_o = '0;
  assign writeback_valid_o = 1'b0;
  assign writeback_req_o = '0;
  assign csr_debug_signals_o = '0;
  assign req_state_unused = 1'b0;
  assign pull_state_unused = 1'b0;

  /* verilator lint_off UNUSEDSIGNAL */  // Ports are intentionally preserved from the original non-GFX stub interface.
  logic _unused_inputs;
  assign _unused_inputs = clk_i ^ rst_ni ^ chicken_bit_dcache_i ^ tex_send_valid_i ^
                          l2_resp_valid_i ^ l2_req_ready_i ^ writeback_ready_i ^
                          writeback_done_i ^ (^neigh_min_id_i) ^ (^tex_send_ctrl_i) ^
                          (^l2_resp_i);
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : minion_dcache_texsend
