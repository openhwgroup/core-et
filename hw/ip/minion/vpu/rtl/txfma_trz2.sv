// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_trz2
  import vpu_pkg::*;
#(
  parameter int unsigned InWidth  = TxfmaPsDpathSz,
  parameter int unsigned OutWidth = TxfmaWspsEtrzSz
) (
  input  logic [InWidth-1:0]  in1_i,
  input  logic [InWidth-1:0]  in2_i,
  input  logic                add_cin_i,
  output logic [OutWidth-1:0] trz_o
);

  logic [InWidth-1:0] p;
  /* verilator lint_off UNUSEDSIGNAL */  // The original builds the full kill vector even though only k[InWidth-2:0] feed the indicator chain.
  logic [InWidth-1:0] k;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [InWidth-1:0] indicator;

  assign p = in1_i ^ in2_i;
  assign k = ~in1_i & ~in2_i;

  assign indicator[0] = p[0] ^ add_cin_i;

  for (genvar n = 1; n < InWidth; ++n) begin : gen_indicator
    assign indicator[n] = (p[n] & k[n-1]) | (~p[n] & ~k[n-1]);
  end

  always_comb begin
    trz_o = InWidth[OutWidth-1:0];
    for (int i = InWidth - 1; i >= 0; --i) begin
      if (indicator[i]) begin
        trz_o = i[OutWidth-1:0];
      end
    end
  end

endmodule : txfma_trz2
