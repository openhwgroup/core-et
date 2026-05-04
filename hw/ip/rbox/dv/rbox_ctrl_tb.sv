// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_ctrl.
// Uses single clock domain. Ties off most interfaces.
// Exposes ESR config, start, status, and key control outputs.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off PINCONNECTEMPTY */
module rbox_ctrl_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // ESR interface
  input  logic [63:0]       esr_config_i,
  input  logic              esr_config_en_i,
  input  logic [63:0]       esr_in_buf_pg_i,
  input  logic              esr_in_buf_pg_en_i,
  input  logic [63:0]       esr_in_buf_cfg_i,
  input  logic              esr_in_buf_cfg_en_i,
  input  logic [31:0]       esr_out_buf_pg_i,
  input  logic              esr_out_buf_pg_en_i,
  input  logic [63:0]       esr_out_buf_cfg_i,
  input  logic              esr_out_buf_cfg_en_i,
  input  logic [31:0]       esr_start_i,
  input  logic              esr_start_en_i,
  input  logic [31:0]       esr_consume_i,
  input  logic              esr_consume_en_i,
  output logic [31:0]       esr_status_o,

  // Enable output
  output logic              rbox_enable_o,

  // Tile valid output (to traverse)
  output logic              tr_tile_valid_o,

  // Traverse/Test feedback
  input  logic              tr_accept_i,
  input  logic              tr_empty_i,
  input  logic              ts_empty_i,

  // SC request (observe any valid)
  output logic              sc_req_valid_any_o,
  input  logic              sc_req_ready_i,

  // Flush
  output logic              db_flush_o,
  input  logic              db_flush_done_i,

  // Halted
  input  logic              rbox_halted_i
);

  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  req_t                rbox_sc_req;
  logic [ScBanks:0]    rbox_sc_req_valid;
  logic [ScBanks:0]    rbox_sc_req_ready;
  rsp_t                sc_rbox_rsp;

  assign rbox_sc_req_ready = {(ScBanks+1){sc_req_ready_i}};
  assign sc_req_valid_any_o = |rbox_sc_req_valid;
  assign sc_rbox_rsp = '0;

  // Stubbed ports
  req_t                db_sc_req;
  logic [ScBanks:0]    db_sc_req_valid;
  rsp_t                sc_db_rsp;
  logic                sc_db_rsp_valid;
  quad_data_t          tr_quad;
  logic                tr_quad_valid;
  logic [3:0]          ts_result_mask;
  logic                ts_result_valid;

  assign db_sc_req = '0;
  assign db_sc_req_valid = '0;
  assign tr_quad = '0;
  assign tr_quad_valid = 0;
  assign ts_result_mask = '0;
  assign ts_result_valid = 0;

  rbox_ctrl u_dut (
    .clk_i                (clk_i),
    .clk_rbox_i           (clk_i),
    .rst_ni               (rst_ni),
    .ram_cfg_i            ('0),
    .dft_i                ('0),
    .rbox_enable_o        (rbox_enable_o),
    .rbox_sc_req_o        (rbox_sc_req),
    .rbox_sc_req_valid_o  (rbox_sc_req_valid),
    .rbox_sc_req_ready_i  (rbox_sc_req_ready),
    .sc_rbox_rsp_i        (sc_rbox_rsp),
    .sc_rbox_rsp_valid_i  (1'b0),
    .sc_rbox_rsp_ready_o  (),
    .db_sc_req_i          (db_sc_req),
    .db_sc_req_valid_i    (db_sc_req_valid),
    .db_sc_req_ready_o    (),
    .sc_db_rsp_o          (sc_db_rsp),
    .sc_db_rsp_valid_o    (sc_db_rsp_valid),
    .esr_config_i         (esr_config_i),
    .esr_config_en_i      (esr_config_en_i),
    .esr_in_buf_pg_i      (esr_in_buf_pg_i),
    .esr_in_buf_pg_en_i   (esr_in_buf_pg_en_i),
    .esr_in_buf_cfg_i     (esr_in_buf_cfg_i),
    .esr_in_buf_cfg_en_i  (esr_in_buf_cfg_en_i),
    .esr_out_buf_pg_i     (esr_out_buf_pg_i),
    .esr_out_buf_pg_en_i  (esr_out_buf_pg_en_i),
    .esr_out_buf_cfg_i    (esr_out_buf_cfg_i),
    .esr_out_buf_cfg_en_i (esr_out_buf_cfg_en_i),
    .esr_start_i          (esr_start_i),
    .esr_start_en_i       (esr_start_en_i),
    .esr_consume_i        (esr_consume_i),
    .esr_consume_en_i     (esr_consume_en_i),
    .esr_status_o         (esr_status_o),
    .tr_msaa_enable_o     (),
    .tr_msaa_mode_o       (),
    .tr_depth_is_fp32_o   (),
    .tr_depth_is_un16_o   (),
    .tr_depth_is_un24_o   (),
    .tr_scissor_min_x_o   (),
    .tr_scissor_max_x_o   (),
    .tr_scissor_min_y_o   (),
    .tr_scissor_max_y_o   (),
    .tr_depth_clamp_enable_o (),
    .tr_depth_min_o       (),
    .tr_depth_max_o       (),
    .tr_depth_bound_enable_o (),
    .tr_depth_bound_min_o (),
    .tr_depth_bound_max_o (),
    .tr_triangle_o        (),
    .tr_tile_o            (),
    .tr_tile_valid_o      (tr_tile_valid_o),
    .tr_accept_i          (tr_accept_i),
    .tr_empty_i           (tr_empty_i),
    .tr_quad_i            (tr_quad),
    .tr_quad_valid_i      (tr_quad_valid),
    .ct_quad_accept_o     (),
    .ts_state_o           (),
    .ts_front_triangle_o  (),
    .ts_empty_i           (ts_empty_i),
    .ts_result_mask_i     (ts_result_mask),
    .ts_result_valid_i    (ts_result_valid),
    .ct_result_accept_o   (),
    .db_flush_o           (db_flush_o),
    .db_flush_done_i      (db_flush_done_i),
    .ct_sm_data_o         (),
    .rbox_halted_i        (rbox_halted_i)
  );

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNOPTFLAT */
