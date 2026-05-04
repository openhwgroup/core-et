// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_trz_tb #(
  parameter int unsigned TrzOut = 5,
  parameter int unsigned Max    = 23
) (
  input  logic [Max:0]      frac_i,
  output logic [TrzOut-1:0] orig_trz_o,
  output logic [TrzOut-1:0] new_trz_o
);

  txfma_trz_orig #(
    .trz_out (TrzOut),
    .max     (Max)
  ) u_orig (
    .frac (frac_i),
    .trz  (orig_trz_o)
  );

  txfma_trz #(
    .TrzOut (TrzOut),
    .Max    (Max)
  ) u_new (
    .frac_i,
    .trz_o (new_trz_o)
  );

endmodule : cosim_txfma_trz_tb
