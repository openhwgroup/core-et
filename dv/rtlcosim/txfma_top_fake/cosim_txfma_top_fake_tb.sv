// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_top_fake_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                                      clk_i,
  input  logic                                      rst_ni,
  input  logic                                      in_valid_i,
  input  logic                                      use_prev_sigs_i,
  input  logic [VpuDataSSz-1:0]                     in1_i,
  input  logic [VpuDataSSz-1:0]                     in2_i,
  input  logic [VpuDataSSz-1:0]                     in3_i,
  input  logic [$bits(vpu_ctrl_sigs_t)-1:0]         sigs_i,
  input  logic [19:0]                               imm_i,
  input  logic [2:0]                                rm_i,
  input  logic [1:0]                                typ_i,
  input  logic [$bits(trans_fma_coefficients)-1:0]  trans_coeff_i,
  output logic [VpuDataSSz-1:0]                     orig_data_o,
  output logic [VpuFlagsSz-1:0]                     orig_exc_o,
  output logic                                      orig_comp_o,
  output logic [VpuDataSSz-1:0]                     new_data_o,
  output logic [VpuFlagsSz-1:0]                     new_exc_o,
  output logic                                      new_comp_o
);

  vpu_input_t in_data;
  trans_fma_coefficients trans_coeff;
  vpu_output_t orig_out_data;
  vpu_output_t new_out_data;

  always_comb begin
    in_data = '0;
    in_data.use_prev_sigs = use_prev_sigs_i;
    in_data.sigs = minion_pkg::vpu_ctrl_sigs_t'(sigs_i);
    in_data.imm = imm_i;
    in_data.rm = rm_i;
    in_data.typ = typ_i;
    in_data.in1 = in1_i;
    in_data.in2 = in2_i;
    in_data.in3 = in3_i;
    trans_coeff = trans_fma_coefficients'(trans_coeff_i);
  end

  txfma_top_fake_orig u_orig (
    .clock             (clk_i),
    .reset             (!rst_ni),
    .sleep             (1'b0),
    .in_valid          (in_valid_i),
    .in_data           (in_data),
    .trans_coefficients(trans_coeff),
    .out_data          (orig_out_data)
  );

  txfma_top_fake u_new (
    .clock             (clk_i),
    .reset             (!rst_ni),
    .in_valid          (in_valid_i),
    .in_data           (in_data),
    .trans_coefficients(trans_coeff),
    .out_data_res      (new_out_data),
    .out_comp_res      (new_comp_o)
  );

  assign orig_data_o = orig_out_data.data;
  assign orig_exc_o = orig_out_data.exc;
  assign orig_comp_o = 1'b0;
  assign new_data_o = new_out_data.data;
  assign new_exc_o = new_out_data.exc;

endmodule : cosim_txfma_top_fake_tb
