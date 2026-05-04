// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_mul_div_tb
  import minion_pkg::*;
  import dft_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        req_valid_i,
  input  logic        req_valid_early_i,
  input  logic [4:0]  req_fn_i,
  input  logic [4:0]  req_fn_early_i,
  input  logic        req_dw_i,
  input  logic [63:0] req_in1_i,
  input  logic [63:0] req_in2_i,
  input  logic [6:0]  req_dest_i,
  input  logic        kill_i,
  input  logic        chicken_bit_mul_div_i,
  input  logic        resp_ready_i,
  output logic        req_ready_o,
  output logic        resp_valid_o,
  output logic        resp_valid_early_o,
  output logic [63:0] resp_data_o,
  output logic [6:0]  resp_dest_o
);

  minion_reg_dest_t req_dest_s;
  minion_reg_dest_t resp_dest_s;
  /* verilator lint_off UNUSEDSIGNAL */  // Lint anchor keeps the low-phase clock-gate primitive reachable when this wrapper includes the full translated mul/div primitive set.
  logic             lint_clk_no;
  /* verilator lint_on UNUSEDSIGNAL */

  prim_clk_gate_n u_lint_clk_gate_n (
    .clk_i  (clk_i),
    .en_i   (1'b0),
    .clk_no (lint_clk_no)
  );

  assign req_dest_s = minion_reg_dest_t'(req_dest_i);
  assign resp_dest_o = resp_dest_s;

  prim_mul_div u_dut (
    .clk_i,
    .rst_ni,
    .dft_i               ('0),
    .req_ready           (req_ready_o),
    .req_valid           (req_valid_i),
    .req_valid_early     (req_valid_early_i),
    .req_fn              (alu_func_e'(req_fn_i)),
    .req_fn_early        (alu_func_e'(req_fn_early_i)),
    .req_dw              (req_dw_i),
    .req_in1             (req_in1_i),
    .req_in2             (req_in2_i),
    .req_dest            (req_dest_s),
    .kill                (kill_i),
    .chicken_bit_mul_div (chicken_bit_mul_div_i),
    .resp_ready          (resp_ready_i),
    .resp_valid          (resp_valid_o),
    .resp_valid_early    (resp_valid_early_o),
    .resp_data           (resp_data_o),
    .resp_dest           (resp_dest_s)
  );

endmodule
