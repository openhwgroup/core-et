// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_inst_bits_stage (new) vs (original).
//
// Tests 3 parameter configurations:
//   Config 0: Mask=0x0000707F BrMask=0xFE000F80 (opcode+funct3, rd+funct7)
//   Config 1: Mask=0xFFFFFFFF BrMask=0x00000000 (all bits always, no br bits)
//   Config 2: Mask=0x00000000 BrMask=0xFFFFFFFF (no always bits, all br bits)

`include "soc.vh"

module cosim_intpipe_inst_bits_stage_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic        stim_enable,
  input  logic        stim_is_csr,
  input  logic        stim_is_br_jal,
  input  logic        stim_te_enable,
  input  logic        stim_xcpt,
  input  logic [31:0] stim_in,

  // Config 0 outputs
  output logic [31:0] new_out_0_o,
  output logic [31:0] orig_out_0_o,
  // Config 1 outputs
  output logic [31:0] new_out_1_o,
  output logic [31:0] orig_out_1_o,
  // Config 2 outputs
  output logic [31:0] new_out_2_o,
  output logic [31:0] orig_out_2_o
);

  import minion_pkg::*;

  // Config 0: realistic masks
  intpipe_inst_bits_stage #(.Mask(32'h0000707F), .BrMask(32'hFE000F80)) u_new_0 (
    .clk_i, .enable_i(stim_enable), .is_csr_i(stim_is_csr),
    .is_br_jal_i(stim_is_br_jal), .te_enable_i(stim_te_enable),
    .xcpt_i(stim_xcpt), .in_i(stim_in), .out_o(new_out_0_o)
  );
  intpipe_inst_bits_stage_orig #(.MASK(32'h0000707F), .BR_MASK(32'hFE000F80)) u_orig_0 (
    .clock(clk_i), .enable(stim_enable), .is_csr(stim_is_csr),
    .is_br_jal(stim_is_br_jal), .te_enable(stim_te_enable),
    .xcpt(stim_xcpt), .in(stim_in), .out(orig_out_0_o)
  );

  // Config 1: all bits in Mask
  intpipe_inst_bits_stage #(.Mask(32'hFFFFFFFF), .BrMask(32'h00000000)) u_new_1 (
    .clk_i, .enable_i(stim_enable), .is_csr_i(stim_is_csr),
    .is_br_jal_i(stim_is_br_jal), .te_enable_i(stim_te_enable),
    .xcpt_i(stim_xcpt), .in_i(stim_in), .out_o(new_out_1_o)
  );
  intpipe_inst_bits_stage_orig #(.MASK(32'hFFFFFFFF), .BR_MASK(32'h00000000)) u_orig_1 (
    .clock(clk_i), .enable(stim_enable), .is_csr(stim_is_csr),
    .is_br_jal(stim_is_br_jal), .te_enable(stim_te_enable),
    .xcpt(stim_xcpt), .in(stim_in), .out(orig_out_1_o)
  );

  // Config 2: all bits in BrMask
  intpipe_inst_bits_stage #(.Mask(32'h00000000), .BrMask(32'hFFFFFFFF)) u_new_2 (
    .clk_i, .enable_i(stim_enable), .is_csr_i(stim_is_csr),
    .is_br_jal_i(stim_is_br_jal), .te_enable_i(stim_te_enable),
    .xcpt_i(stim_xcpt), .in_i(stim_in), .out_o(new_out_2_o)
  );
  intpipe_inst_bits_stage_orig #(.MASK(32'h00000000), .BR_MASK(32'hFFFFFFFF)) u_orig_2 (
    .clock(clk_i), .enable(stim_enable), .is_csr(stim_is_csr),
    .is_br_jal(stim_is_br_jal), .te_enable(stim_te_enable),
    .xcpt(stim_xcpt), .in(stim_in), .out(orig_out_2_o)
  );

endmodule
