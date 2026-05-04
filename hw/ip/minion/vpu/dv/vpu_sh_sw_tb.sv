// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_sh_sw_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic [$clog2(VpuLaneNum)-1:0] lane_id_i,
  input  logic                        ex_in_valid_i,
  input  logic [$bits(vpu_cmd_e)-1:0] ex_cmd_i,
  input  logic [VpuDataSSz-1:0]       ex_in1_i,
  input  logic [VpuDataSSz-1:0]       ex_in2_i,
  input  logic [VpuDataSSz-1:0]       ex_in3_i,
  input  logic [19:0]                 ex_imm_i,
  input  logic [$bits(vpu_dtype_e)-1:0] ex_dtype_i,
  input  logic                        ex_mask_in1_i,
  input  logic [VpuDataSSz-1:0]       f2_swizz_in1_i,
  input  logic [VpuDataSSz-1:0]       f2_swizz_in2_i,
  input  logic [VpuDataSSz-1:0]       f2_swizz_in3_i,
  output logic [VpuDataSSz-1:0]       f2_fswizz_rdata_o,
  output logic [VpuDataSSz-1:0]       f3_out_data_o,
  output logic [VpuFlagsSz-1:0]       f3_out_exc_o
);

  vpu_output_t f3_out_bits;

  vpu_sh_sw u_dut (
    .lane_id_i,
    .clk_i,
    .rst_ni,
    .ex_in_valid_i,
    .ex_cmd_i          (vpu_cmd_e'(ex_cmd_i)),
    .ex_in1_i,
    .ex_in2_i,
    .ex_in3_i,
    .ex_imm_i,
    .ex_dtype_i        (vpu_dtype_e'(ex_dtype_i)),
    .ex_mask_in1_i,
    .f2_swizz_in1_i,
    .f2_swizz_in2_i,
    .f2_swizz_in3_i,
    .f2_fswizz_rdata_o,
    .f3_out_bits_o     (f3_out_bits)
  );

  assign f3_out_data_o = f3_out_bits.data;
  assign f3_out_exc_o  = f3_out_bits.exc;

endmodule
