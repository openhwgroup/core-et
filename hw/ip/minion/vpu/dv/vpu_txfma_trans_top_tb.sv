// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_txfma_trans_top_tb
  import minion_pkg::*;
  import vpu_pkg::*;
  #(
    parameter bit UseFakeTxfma = 1'b1
  )
(
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic                             ex_rom_valid_i,
  input  logic                             ex_txfma_valid_i,
  input  logic                             use_prev_sigs_i,
  input  logic [$bits(vpu_ctrl_sigs_t)-1:0] sigs_i,
  input  logic [6:0]                       cmd_i,
  input  logic [3:0]                       dtype_i,
  input  logic [19:0]                      imm_i,
  input  logic [2:0]                       rm_i,
  input  logic [1:0]                       typ_i,
  input  logic [VpuDataSSz-1:0]            in1_i,
  input  logic [VpuDataSSz-1:0]            in2_i,
  input  logic [VpuDataSSz-1:0]            in3_i,
  output logic [VpuDataSSz-1:0]            f8_data_o,
  output logic [VpuFlagsSz-1:0]            f8_exc_o,
  output logic                             f8_comp_o,
  output logic [1:0]                       txfma_trans_dbg_o,
  output logic                             use_fake_txfma_o
);

  vpu_input_t  ex_in_bits;
  vpu_output_t f8_txfma_res;

  always_comb begin
    ex_in_bits = '0;
    ex_in_bits.use_prev_sigs = use_prev_sigs_i;
    ex_in_bits.sigs = vpu_ctrl_sigs_t'(sigs_i);
    ex_in_bits.sigs.cmd = vpu_cmd_e'(cmd_i);
    ex_in_bits.sigs.dtype = vpu_dtype_e'(dtype_i);
    ex_in_bits.imm = imm_i;
    ex_in_bits.rm = rm_i;
    ex_in_bits.typ = typ_i;
    ex_in_bits.in1 = in1_i;
    ex_in_bits.in2 = in2_i;
    ex_in_bits.in3 = in3_i;
  end

  vpu_txfma_trans_top #(
    .UseFakeTxfma (UseFakeTxfma)
  ) u_dut (
    .clock_txfma       (clk_i),
    .clock_trans_rom   (clk_i),
    .reset             (!rst_ni),
    .ex_rom_valid      (ex_rom_valid_i),
    .ex_in_bits        (ex_in_bits),
    .ex_txfma_valid    (ex_txfma_valid_i),
    .f8_txfma_res      (f8_txfma_res),
    .f8_txfma_comp_res (f8_comp_o),
    .txfma_trans_dbg   (txfma_trans_dbg_o)
  );

  assign f8_data_o = f8_txfma_res.data;
  assign f8_exc_o = f8_txfma_res.exc;
  assign use_fake_txfma_o = UseFakeTxfma;

endmodule : vpu_txfma_trans_top_tb
