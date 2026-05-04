// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original ecc_gen/ecc_corr vs new prim_ecc_enc/prim_ecc_dec.
// Tests all three width configurations: ECC6 (23-bit), ECC7 (33-bit), ECC8 (64-bit).

`include "macros.vh"

module cosim_ecc_tb (
  // ECC8 (64-bit data, 8-bit ECC) — primary test path
  input  logic [63:0]  data8_i,
  input  logic [71:0]  data_and_ecc8_i,

  // Encoder outputs
  output logic [71:0]  new_enc8_o,
  output logic [71:0]  orig_enc8_o,

  // Decoder outputs
  output logic [63:0]  new_dec8_data_o,
  output logic         new_dec8_sbe_o,
  output logic         new_dec8_dbe_o,
  output logic [63:0]  orig_dec8_data_o,
  output logic         orig_dec8_sbe_o,
  output logic         orig_dec8_dbe_o,

  // ECC7 (33-bit data, 7-bit ECC)
  input  logic [32:0]  data7_i,
  input  logic [39:0]  data_and_ecc7_i,

  output logic [39:0]  new_enc7_o,
  output logic [39:0]  orig_enc7_o,
  output logic [32:0]  new_dec7_data_o,
  output logic         new_dec7_sbe_o,
  output logic         new_dec7_dbe_o,
  output logic [32:0]  orig_dec7_data_o,
  output logic         orig_dec7_sbe_o,
  output logic         orig_dec7_dbe_o,

  // ECC6 (23-bit data, 6-bit ECC)
  input  logic [22:0]  data6_i,
  input  logic [28:0]  data_and_ecc6_i,

  output logic [28:0]  new_enc6_o,
  output logic [28:0]  orig_enc6_o,
  output logic [22:0]  new_dec6_data_o,
  output logic         new_dec6_sbe_o,
  output logic         new_dec6_dbe_o,
  output logic [22:0]  orig_dec6_data_o,
  output logic         orig_dec6_sbe_o,
  output logic         orig_dec6_dbe_o
);

  // ── ECC8: New ─────────────────────────────────────────────
  prim_ecc_enc #(.DataWidth(64)) u_new_enc8 (.data_i(data8_i), .data_and_ecc_o(new_enc8_o));
  prim_ecc_dec #(.DataWidth(64)) u_new_dec8 (.data_and_ecc_i(data_and_ecc8_i),
    .data_o(new_dec8_data_o), .sbe_o(new_dec8_sbe_o), .dbe_o(new_dec8_dbe_o));

  // ── ECC8: Original ────────────────────────────────────────
  ecc_gen  #(.DATA_SIZE(64)) u_orig_enc8 (.data(data8_i), .data_and_ecc(orig_enc8_o));
  ecc_corr #(.DATA_SIZE(64)) u_orig_dec8 (.data_and_ecc(data_and_ecc8_i),
    .data_corr(orig_dec8_data_o), .sbe(orig_dec8_sbe_o), .dbe(orig_dec8_dbe_o));

  // ── ECC7: New ─────────────────────────────────────────────
  prim_ecc_enc #(.DataWidth(33)) u_new_enc7 (.data_i(data7_i), .data_and_ecc_o(new_enc7_o));
  prim_ecc_dec #(.DataWidth(33)) u_new_dec7 (.data_and_ecc_i(data_and_ecc7_i),
    .data_o(new_dec7_data_o), .sbe_o(new_dec7_sbe_o), .dbe_o(new_dec7_dbe_o));

  // ── ECC7: Original ────────────────────────────────────────
  ecc_gen  #(.DATA_SIZE(33)) u_orig_enc7 (.data(data7_i), .data_and_ecc(orig_enc7_o));
  ecc_corr #(.DATA_SIZE(33)) u_orig_dec7 (.data_and_ecc(data_and_ecc7_i),
    .data_corr(orig_dec7_data_o), .sbe(orig_dec7_sbe_o), .dbe(orig_dec7_dbe_o));

  // ── ECC6: New ─────────────────────────────────────────────
  prim_ecc_enc #(.DataWidth(23)) u_new_enc6 (.data_i(data6_i), .data_and_ecc_o(new_enc6_o));
  prim_ecc_dec #(.DataWidth(23)) u_new_dec6 (.data_and_ecc_i(data_and_ecc6_i),
    .data_o(new_dec6_data_o), .sbe_o(new_dec6_sbe_o), .dbe_o(new_dec6_dbe_o));

  // ── ECC6: Original ────────────────────────────────────────
  ecc_gen  #(.DATA_SIZE(23)) u_orig_enc6 (.data(data6_i), .data_and_ecc(orig_enc6_o));
  ecc_corr #(.DATA_SIZE(23)) u_orig_dec6 (.data_and_ecc(data_and_ecc6_i),
    .data_corr(orig_dec6_data_o), .sbe(orig_dec6_sbe_o), .dbe(orig_dec6_dbe_o));

endmodule : cosim_ecc_tb
