// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_vpu_sh_sw_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic [`CORE_GSC_CNT_RANGE]  lane_id_i,
  input  logic                        ex_in_valid_i,
  input  logic [6:0]                  ex_cmd_i,
  input  logic [31:0]                 ex_in1_i,
  input  logic [31:0]                 ex_in2_i,
  input  logic [31:0]                 ex_in3_i,
  input  logic [19:0]                 ex_imm_i,
  input  logic [3:0]                  ex_dtype_i,
  input  logic                        ex_mask_in1_i,
  input  logic [31:0]                 f2_swizz_in1_i,
  input  logic [31:0]                 f2_swizz_in2_i,
  input  logic [31:0]                 f2_swizz_in3_i,
  output logic [31:0]                 orig_f2_fswizz_rdata_o,
  output logic [31:0]                 new_f2_fswizz_rdata_o,
  output logic [31:0]                 orig_f3_out_data_o,
  output logic [31:0]                 new_f3_out_data_o,
  output logic [5:0]                  orig_f3_out_exc_o,
  output logic [5:0]                  new_f3_out_exc_o
);

  vpu_output_t orig_f3_out_bits;
  vpu_pkg::vpu_output_t new_f3_out_bits;

  vpu_sh_sw_orig u_orig (
    .lane_id         (lane_id_i),
    .clock           (clk_i),
    .reset           (!rst_ni),
    .ex_in_valid     (ex_in_valid_i),
    .ex_cmd          (ex_cmd_i),
    .ex_in1          (ex_in1_i),
    .ex_in2          (ex_in2_i),
    .ex_in3          (ex_in3_i),
    .ex_imm          (ex_imm_i),
    .ex_dtype        (ex_dtype_i),
    .ex_mask_in1     (ex_mask_in1_i),
    .f2_swizz_in1    (f2_swizz_in1_i),
    .f2_swizz_in2    (f2_swizz_in2_i),
    .f2_swizz_in3    (f2_swizz_in3_i),
    .f2_fswizz_rdata (orig_f2_fswizz_rdata_o),
    .f3_out_bits     (orig_f3_out_bits)
  );

  vpu_sh_sw u_new (
    .lane_id_i         (lane_id_i),
    .clk_i             (clk_i),
    .rst_ni            (rst_ni),
    .ex_in_valid_i     (ex_in_valid_i),
    .ex_cmd_i          (vpu_cmd_e'(ex_cmd_i)),
    .ex_in1_i          (ex_in1_i),
    .ex_in2_i          (ex_in2_i),
    .ex_in3_i          (ex_in3_i),
    .ex_imm_i          (ex_imm_i),
    .ex_dtype_i        (vpu_dtype_e'(ex_dtype_i)),
    .ex_mask_in1_i     (ex_mask_in1_i),
    .f2_swizz_in1_i    (f2_swizz_in1_i),
    .f2_swizz_in2_i    (f2_swizz_in2_i),
    .f2_swizz_in3_i    (f2_swizz_in3_i),
    .f2_fswizz_rdata_o (new_f2_fswizz_rdata_o),
    .f3_out_bits_o     (new_f3_out_bits)
  );

  assign orig_f3_out_data_o = orig_f3_out_bits.data;
  assign orig_f3_out_exc_o  = orig_f3_out_bits.exc;
  assign new_f3_out_data_o  = new_f3_out_bits.data;
  assign new_f3_out_exc_o   = new_f3_out_bits.exc;

endmodule
