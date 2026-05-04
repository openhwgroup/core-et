// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_prim_mul_div_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        stim_req_valid,
  input  logic        stim_req_valid_early,
  input  logic [4:0]  stim_req_fn,
  input  logic [4:0]  stim_req_fn_early,
  input  logic        stim_req_dw,
  input  logic [63:0] stim_req_in1,
  input  logic [63:0] stim_req_in2,
  input  logic [6:0]  stim_req_dest,
  input  logic        stim_kill,
  input  logic        stim_chicken_bit,
  input  logic        stim_resp_ready,
  output logic        new_req_ready,
  output logic        new_resp_valid,
  output logic        new_resp_valid_early,
  output logic [63:0] new_resp_data,
  output logic [6:0]  new_resp_dest,
  output logic        orig_req_ready,
  output logic        orig_resp_valid,
  output logic        orig_resp_valid_early,
  output logic [63:0] orig_resp_data,
  output logic [6:0]  orig_resp_dest
);

  import minion_pkg::*;
  import dft_pkg::*;

  minion_reg_dest_t new_resp_dest_s;

  prim_mul_div u_new (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .dft_i               ('0),
    .req_ready           (new_req_ready),
    .req_valid           (stim_req_valid),
    .req_valid_early     (stim_req_valid_early),
    .req_fn              (alu_func_e'(stim_req_fn)),
    .req_fn_early        (alu_func_e'(stim_req_fn_early)),
    .req_dw              (stim_req_dw),
    .req_in1             (stim_req_in1),
    .req_in2             (stim_req_in2),
    .req_dest            (minion_reg_dest_t'(stim_req_dest)),
    .kill                (stim_kill),
    .chicken_bit_mul_div (stim_chicken_bit),
    .resp_ready          (stim_resp_ready),
    .resp_valid          (new_resp_valid),
    .resp_valid_early    (new_resp_valid_early),
    .resp_data           (new_resp_data),
    .resp_dest           (new_resp_dest_s)
  );
  assign new_resp_dest = new_resp_dest_s;

  logic orig_reset;
  assign orig_reset = !rst_ni;

  minion_reg_dest orig_resp_dest_s;

  intpipe_mul_div_top_orig u_orig (
    .clock_aon           (clk_i),
    .reset               (orig_reset),
    .req_ready           (orig_req_ready),
    .req_valid           (stim_req_valid),
    .req_valid_early     (stim_req_valid_early),
    .req_fn              (core_alu_func'(stim_req_fn)),
    .req_fn_early        (core_alu_func'(stim_req_fn_early)),
    .req_dw              (stim_req_dw),
    .req_in1             (stim_req_in1),
    .req_in2             (stim_req_in2),
    .req_dest            (minion_reg_dest'(stim_req_dest)),
    .kill                (stim_kill),
    .chicken_bit_mul_div (stim_chicken_bit),
    .resp_ready          (stim_resp_ready),
    .resp_valid          (orig_resp_valid),
    .resp_valid_early    (orig_resp_valid_early),
    .resp_data           (orig_resp_data),
    .resp_dest           (orig_resp_dest_s)
  );
  assign orig_resp_dest = orig_resp_dest_s;

endmodule
