// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_vpu_txfma_trans_top_tb
(
  input  logic                                           clk_i,
  input  logic                                           rst_ni,
  input  logic                                           ex_rom_valid_i,
  input  logic                                           ex_txfma_valid_i,
  input  logic                                           use_prev_sigs_i,
  input  logic [$bits(minion_pkg::vpu_ctrl_sigs_t)-1:0]  sigs_i,
  input  logic [6:0]                                     cmd_i,
  input  logic [3:0]                                     dtype_i,
  input  logic [19:0]                                    imm_i,
  input  logic [2:0]                                     rm_i,
  input  logic [1:0]                                     typ_i,
  input  logic [minion_pkg::VpuDataSSz-1:0]              in1_i,
  input  logic [minion_pkg::VpuDataSSz-1:0]              in2_i,
  input  logic [minion_pkg::VpuDataSSz-1:0]              in3_i,
  output logic [minion_pkg::VpuDataSSz-1:0]              orig_f8_data_o,
  output logic [vpu_pkg::VpuFlagsSz-1:0]                 orig_f8_exc_o,
  output logic                                           orig_f8_comp_o,
  output logic [1:0]                                     orig_txfma_trans_dbg_o,
  output logic [minion_pkg::VpuDataSSz-1:0]              new_f8_data_o,
  output logic [vpu_pkg::VpuFlagsSz-1:0]                 new_f8_exc_o,
  output logic                                           new_f8_comp_o,
  output logic [1:0]                                     new_txfma_trans_dbg_o
);

  vpu_input_t            orig_ex_in_bits;
  vpu_output_t           orig_f8_txfma_res;
  vpu_pkg::vpu_input_t   new_ex_in_bits;
  vpu_pkg::vpu_output_t  new_f8_txfma_res;

  always_comb begin
    orig_ex_in_bits = '0;
    new_ex_in_bits = '0;

    orig_ex_in_bits.use_prev_sigs = use_prev_sigs_i;
    orig_ex_in_bits.sigs = sigs_i;
    orig_ex_in_bits.sigs.cmd = cmd_i;
    orig_ex_in_bits.sigs.dtype = dtype_i;
    orig_ex_in_bits.imm = imm_i;
    orig_ex_in_bits.rm = rm_i;
    orig_ex_in_bits.typ = typ_i;
    orig_ex_in_bits.in1 = in1_i;
    orig_ex_in_bits.in2 = in2_i;
    orig_ex_in_bits.in3 = in3_i;

    new_ex_in_bits.use_prev_sigs = use_prev_sigs_i;
    new_ex_in_bits.sigs = minion_pkg::vpu_ctrl_sigs_t'(sigs_i);
    new_ex_in_bits.sigs.cmd = minion_pkg::vpu_cmd_e'(cmd_i);
    new_ex_in_bits.sigs.dtype = minion_pkg::vpu_dtype_e'(dtype_i);
    new_ex_in_bits.imm = imm_i;
    new_ex_in_bits.rm = rm_i;
    new_ex_in_bits.typ = typ_i;
    new_ex_in_bits.in1 = in1_i;
    new_ex_in_bits.in2 = in2_i;
    new_ex_in_bits.in3 = in3_i;
  end

  // mk/rtlcosim.mk renames complete original module identifiers with `_orig`.
  vpu_txfma_trans_top_orig u_orig (
    .clock_txfma       (clk_i),
    .clock_trans_rom   (clk_i),
    .reset             (~rst_ni),
    .ex_rom_valid      (ex_rom_valid_i),
    .ex_in_bits        (orig_ex_in_bits),
    .ex_txfma_valid    (ex_txfma_valid_i),
    .f8_txfma_res      (orig_f8_txfma_res),
    .f8_txfma_comp_res (orig_f8_comp_o),
    .txfma_trans_dbg   (orig_txfma_trans_dbg_o)
  );

  vpu_txfma_trans_top #(
    .UseFakeTxfma (1'b0)
  ) u_new (
    .clock_txfma       (clk_i),
    .clock_trans_rom   (clk_i),
    .reset             (~rst_ni),
    .ex_rom_valid      (ex_rom_valid_i),
    .ex_in_bits        (new_ex_in_bits),
    .ex_txfma_valid    (ex_txfma_valid_i),
    .f8_txfma_res      (new_f8_txfma_res),
    .f8_txfma_comp_res (new_f8_comp_o),
    .txfma_trans_dbg   (new_txfma_trans_dbg_o)
  );

  assign orig_f8_data_o = orig_f8_txfma_res.data;
  assign orig_f8_exc_o = orig_f8_txfma_res.exc;

  assign new_f8_data_o = new_f8_txfma_res.data;
  assign new_f8_exc_o = new_f8_txfma_res.exc;

endmodule
