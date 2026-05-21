// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_shire_and_or_tree_daisychain_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [7:0]  shire_id_i,
  input  logic [10:0] l2_bits_i,
  input  logic [39:0] l0_bits_i,
  output logic [10:0] orig_l2_bits_o,
  output logic [10:0] new_l2_bits_o,
  output logic [10:0] orig_l1_bits_o,
  output logic [10:0] new_l1_bits_o
);
  localparam int unsigned NumNeigh = 4;
  localparam int unsigned L0Width = $bits(esr_and_or_tree_L0_t);

  esr_and_or_tree_L2_t orig_l2_i;
  esr_and_or_tree_L2_t orig_l2_o;
  esr_and_or_tree_L0_t [NumNeigh-1:0] orig_l0;
  esr_and_or_tree_L1_t orig_l1;

  shire_channel_leaves_pkg::esr_and_or_tree_l2_t new_l2_i;
  shire_channel_leaves_pkg::esr_and_or_tree_l2_t new_l2_o;
  esr_pkg::esr_and_or_tree_l0_t new_l0 [NumNeigh-1:0];
  shire_esr_pkg::esr_and_or_tree_l1_t new_l1;

  always_comb begin
    orig_l2_i = l2_bits_i;
    new_l2_i = l2_bits_i;
    for (int unsigned n = 0; n < NumNeigh; n++) begin
      orig_l0[n] = l0_bits_i[(n*L0Width) +: L0Width];
      new_l0[n] = l0_bits_i[(n*L0Width) +: L0Width];
    end
    orig_l2_bits_o = orig_l2_o;
    new_l2_bits_o = new_l2_o;
    orig_l1_bits_o = orig_l1;
    new_l1_bits_o = new_l1;
  end

  shire_and_or_tree_daisychain_orig u_orig (
    .shire_id                   (shire_id_i),
    .debug_and_or_tree_L2_in    (orig_l2_i),
    .debug_and_or_tree_L2_out   (orig_l2_o),
    .esr_and_or_tree_L0_ip      (orig_l0),
    .esr_and_or_tree_L1_local_op(orig_l1)
  );

  shire_and_or_tree_daisychain u_new (
    .shire_id_i                 (shire_id_i),
    .debug_and_or_tree_l2_i     (new_l2_i),
    .debug_and_or_tree_l2_o     (new_l2_o),
    .esr_and_or_tree_l0_i       (new_l0),
    .esr_and_or_tree_l1_local_o (new_l1)
  );

  logic unused_clk_rst;
  assign unused_clk_rst = ^{clk_i, rst_ni};
endmodule
