// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_trz_tb #(
  parameter int unsigned TrzOut = 5,
  parameter int unsigned Max    = 23
) (
  input  logic [Max:0]      frac_i,
  output logic [TrzOut-1:0] trz_o
);

  txfma_trz #(
    .TrzOut (TrzOut),
    .Max    (Max)
  ) u_dut (
    .frac_i,
    .trz_o
  );

endmodule : txfma_trz_tb
