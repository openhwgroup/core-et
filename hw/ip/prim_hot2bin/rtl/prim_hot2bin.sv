// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// One-hot to binary encoder.
//
// Converts a one-hot input to its binary index.
// If no bits are set, output is zero.
// If multiple bits are set, output is the OR of their indices
// (same behavior as original hot2bin + onehot_mux).
//
// Replaces: hot2bin + onehot_mux (etcore-soc)

module prim_hot2bin #(
  parameter int unsigned HotW = 8,
  localparam int unsigned BinW = $clog2(HotW)
) (
  input  logic [HotW-1:0] hot_i,
  output logic [BinW-1:0] bin_o
);

  always_comb begin
    bin_o = '0;
    for (int i = 0; i < HotW; i++) begin
      bin_o = bin_o | ({BinW{hot_i[i]}} & i[BinW-1:0]);
    end
  end

endmodule
