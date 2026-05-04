// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_mul_div_tap -- Project-local mul/div wrapper for the primitive seam.

module test_mul_div_tap
  import minion_pkg::*;
  import dft_pkg::*;
(
  input  logic                clk_i,
  input  logic                rst_ni,
  input  dft_t                dft_i,
  output logic                req_ready,
  input  logic                req_valid,
  input  logic                req_valid_early,
  input  alu_func_e           req_fn,
  input  alu_func_e           req_fn_early,
  input  logic                req_dw,
  input  logic [XregSize-1:0] req_in1,
  input  logic [XregSize-1:0] req_in2,
  input  minion_reg_dest_t    req_dest,
  input  logic                kill,
  input  logic                chicken_bit_mul_div,
  input  logic                resp_ready,
  output logic                resp_valid,
  output logic                resp_valid_early,
  output logic [XregSize-1:0] resp_data,
  output minion_reg_dest_t    resp_dest,
  output logic                debug_clock_enable_o,
  output logic                debug_start_mul_1p_o,
  output logic                debug_start_mul_2p_o,
  output logic                debug_mul_op_1p_o,
  output logic                debug_mul_op_2p_o,
  output logic [6:0]          debug_loop_cnt_1p_o,
  output logic [6:0]          debug_loop_cnt_2p_o,
  output logic [11:0]         debug_ctl_bits_o,
  output logic [67:0]         debug_mpcnd_o,
  output logic [67:0]         debug_mul_lo_dvsr_o,
  output logic [63:0]         debug_mul_lo_o,
  output logic [67:0]         debug_acc_v1_o,
  output logic [67:0]         debug_acc_v2_o,
  output logic [63:0]         debug_resp_data_o
);

  intpipe_mul_div_top u_mul_div (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .dft_i               (dft_i),
    .req_ready           (req_ready),
    .req_valid           (req_valid),
    .req_valid_early     (req_valid_early),
    .req_fn              (req_fn),
    .req_fn_early        (req_fn_early),
    .req_dw              (req_dw),
    .req_in1             (req_in1),
    .req_in2             (req_in2),
    .req_dest            (req_dest),
    .kill                (kill),
    .chicken_bit_mul_div (chicken_bit_mul_div),
    .resp_ready          (resp_ready),
    .resp_valid          (resp_valid),
    .resp_valid_early    (resp_valid_early),
    .resp_data           (resp_data),
    .resp_dest           (resp_dest)
  );

  assign debug_clock_enable_o = 1'b0;
  assign debug_start_mul_1p_o = 1'b0;
  assign debug_start_mul_2p_o = 1'b0;
  assign debug_mul_op_1p_o    = 1'b0;
  assign debug_mul_op_2p_o    = 1'b0;
  assign debug_loop_cnt_1p_o  = '0;
  assign debug_loop_cnt_2p_o  = '0;
  assign debug_ctl_bits_o     = '0;
  assign debug_mpcnd_o        = '0;
  assign debug_mul_lo_dvsr_o  = '0;
  assign debug_mul_lo_o       = '0;
  assign debug_acc_v1_o       = '0;
  assign debug_acc_v2_o       = '0;
  assign debug_resp_data_o    = '0;

endmodule
