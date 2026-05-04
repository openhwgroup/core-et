// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_trz
  import vpu_pkg::*;
#(
  parameter int unsigned TrzOut = TxfmaEtrzPsSz,
  parameter int unsigned Max    = Float32M
) (
  input  logic [Max:0]      frac_i,
  output logic [TrzOut-1:0] trz_o
);

  localparam int unsigned FracWidth = Max + 1;

  always_comb begin
    trz_o = FracWidth[TrzOut-1:0];
    for (int i = Max; i >= 0; --i) begin
      if (frac_i[i]) begin
        trz_o = i[TrzOut-1:0];
      end
    end
  end

endmodule : txfma_trz
