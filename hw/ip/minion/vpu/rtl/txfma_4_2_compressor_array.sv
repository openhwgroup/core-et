// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_4_2_compressor_array #(
  parameter int unsigned Width = 64
) (
  input  logic [Width-1:0] x0_i,
  input  logic [Width-1:0] x1_i,
  input  logic [Width-1:0] x2_i,
  input  logic [Width-1:0] x3_i,
  output logic [Width-1:0] c_o,
  output logic [Width-1:0] s_o
);

  /* verilator lint_off UNUSEDSIGNAL */
  // The original array exposes only the per-bit c/s outputs and drops the
  // final carry-out from the ripple chain.
  logic [Width:0] connect_carry;
  /* verilator lint_on UNUSEDSIGNAL */

  assign connect_carry[0] = 1'b0;

  for (genvar i = 0; i < Width; ++i) begin : gen_connect
    txfma_4_2_compressor u_unit_compressor (
      .x0_i   (x0_i[i]),
      .x1_i   (x1_i[i]),
      .x2_i   (x2_i[i]),
      .x3_i   (x3_i[i]),
      .cin_i  (connect_carry[i]),
      .cout_o (connect_carry[i+1]),
      .c_o    (c_o[i]),
      .s_o    (s_o[i])
    );
  end

endmodule : txfma_4_2_compressor_array
