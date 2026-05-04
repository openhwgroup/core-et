// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_texsend.

module minion_dcache_texsend_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0] neigh_min_id_i,
  input  logic chicken_bit_dcache_i,
  output logic idle_o,
  output logic tex_send_ready_o,
  input  logic tex_send_valid_i,
  input  logic [$bits(texsend_control_t)-1:0] tex_send_ctrl_i,
  input  logic l2_resp_valid_i,
  input  logic [$bits(et_link_minion_rsp_info_t)-1:0] l2_resp_i,
  input  logic l2_req_ready_i,
  output logic l2_req_valid_o,
  output logic [$bits(et_link_minion_miss_req_info_t)-1:0] l2_req_o,
  input  logic writeback_ready_i,
  output logic writeback_valid_o,
  output logic [$bits(dcache_wb_req_t)-1:0] writeback_req_o,
  input  logic writeback_done_i,
  output logic [DcacheDbgCsrTxsndSize-1:0] csr_debug_signals_o
);

  texsend_control_t              tex_send_ctrl;
  et_link_minion_rsp_info_t      l2_resp;
  et_link_minion_miss_req_info_t l2_req;
  dcache_wb_req_t                writeback_req;

  assign tex_send_ctrl = texsend_control_t'(tex_send_ctrl_i);
  assign l2_resp = et_link_minion_rsp_info_t'(l2_resp_i);
  assign l2_req_o = l2_req;
  assign writeback_req_o = writeback_req;

  minion_dcache_texsend u_dut (
    .clk_i,
    .rst_ni,
    .neigh_min_id_i,
    .chicken_bit_dcache_i,
    .idle_o,
    .tex_send_ready_o,
    .tex_send_valid_i,
    .tex_send_ctrl_i      (tex_send_ctrl),
    .l2_resp_valid_i,
    .l2_resp_i            (l2_resp),
    .l2_req_ready_i,
    .l2_req_valid_o,
    .l2_req_o             (l2_req),
    .writeback_ready_i,
    .writeback_valid_o,
    .writeback_req_o      (writeback_req),
    .writeback_done_i,
    .csr_debug_signals_o
  );

endmodule : minion_dcache_texsend_tb
