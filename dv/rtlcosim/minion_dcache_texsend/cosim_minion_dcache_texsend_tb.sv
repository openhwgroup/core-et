// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_texsend vs new minion_dcache_texsend.

`include "soc.vh"

module cosim_minion_dcache_texsend_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$clog2(ShireCoopMinMaskSize)-1:0] neigh_min_id_i,
  input  logic chicken_bit_dcache_i,
  input  logic tex_send_valid_i,
  input  logic [$bits(texsend_control_t)-1:0] tex_send_ctrl_i,
  input  logic l2_resp_valid_i,
  input  logic [$bits(et_link_minion_rsp_info_t)-1:0] l2_resp_i,
  input  logic l2_req_ready_i,
  input  logic writeback_ready_i,
  input  logic writeback_done_i,
  output logic orig_idle_o,
  output logic new_idle_o,
  output logic orig_tex_send_ready_o,
  output logic new_tex_send_ready_o,
  output logic orig_l2_req_valid_o,
  output logic new_l2_req_valid_o,
  output logic [$bits(et_link_minion_miss_req_info_t)-1:0] orig_l2_req_o,
  output logic [$bits(et_link_minion_miss_req_info_t)-1:0] new_l2_req_o,
  output logic orig_writeback_valid_o,
  output logic new_writeback_valid_o,
  output logic [$bits(dcache_wb_req_t)-1:0] orig_writeback_req_o,
  output logic [$bits(dcache_wb_req_t)-1:0] new_writeback_req_o,
  output logic [DcacheDbgCsrTxsndSize-1:0] orig_csr_debug_signals_o,
  output logic [DcacheDbgCsrTxsndSize-1:0] new_csr_debug_signals_o
);

  texsend_control_t              tex_send_ctrl;
  et_link_minion_rsp_info_t      l2_resp;
  et_link_minion_miss_req_info_t orig_l2_req;
  et_link_minion_miss_req_info_t new_l2_req;
  dcache_wb_req_t                orig_writeback_req;
  dcache_wb_req_t                new_writeback_req;

  assign tex_send_ctrl = texsend_control_t'(tex_send_ctrl_i);
  assign l2_resp = et_link_minion_rsp_info_t'(l2_resp_i);
  assign orig_l2_req_o = orig_l2_req;
  assign new_l2_req_o = new_l2_req;
  assign orig_writeback_req_o = orig_writeback_req;
  assign new_writeback_req_o = new_writeback_req;

  minion_dcache_texsend u_new (
    .clk_i,
    .rst_ni,
    .neigh_min_id_i,
    .chicken_bit_dcache_i,
    .idle_o                 (new_idle_o),
    .tex_send_ready_o       (new_tex_send_ready_o),
    .tex_send_valid_i,
    .tex_send_ctrl_i        (tex_send_ctrl),
    .l2_resp_valid_i,
    .l2_resp_i              (l2_resp),
    .l2_req_ready_i,
    .l2_req_valid_o         (new_l2_req_valid_o),
    .l2_req_o               (new_l2_req),
    .writeback_ready_i,
    .writeback_valid_o      (new_writeback_valid_o),
    .writeback_req_o        (new_writeback_req),
    .writeback_done_i,
    .csr_debug_signals_o    (new_csr_debug_signals_o)
  );

  dcache_texsend u_orig (
    .clock                  (clk_i),
    .reset                  (!rst_ni),
    .neigh_min_id           (neigh_min_id_i),
    .chicken_bit_dcache     (chicken_bit_dcache_i),
    .idle                   (orig_idle_o),
    .tex_send_ready         (orig_tex_send_ready_o),
    .tex_send_valid         (tex_send_valid_i),
    .tex_send_ctrl          (texsend_control'(tex_send_ctrl_i)),
    .l2_resp_valid          (l2_resp_valid_i),
    .l2_resp                (et_link_minion_rsp_info_t'(l2_resp_i)),
    .l2_req_ready           (l2_req_ready_i),
    .l2_req_valid           (orig_l2_req_valid_o),
    .l2_req                 (orig_l2_req),
    .writeback_ready        (writeback_ready_i),
    .writeback_valid        (orig_writeback_valid_o),
    .writeback_req          (orig_writeback_req),
    .writeback_done         (writeback_done_i),
    .csr_debug_signals      (orig_csr_debug_signals_o)
  );

endmodule : cosim_minion_dcache_texsend_tb
