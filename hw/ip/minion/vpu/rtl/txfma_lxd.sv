// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_lxd
  import vpu_pkg::*;
#(
  parameter int unsigned InWidth      = TxfmaPsDpathSz,
  parameter int unsigned AftA0BitsNum = TxfmaA3 - TxfmaA0 + 1,
  parameter int unsigned OutWidth     = TxfmaLxdPsSz
) (
  input  logic [InWidth-1:0]  in1_i,
  input  logic [InWidth-1:0]  in2_i,
  input  logic                sub_op_i,
  output logic [OutWidth-1:0] lxd_o
);
  localparam int unsigned DefaultCount = InWidth - AftA0BitsNum;

  /* verilator lint_off UNUSEDSIGNAL */  // The original carries the full G/P/K vectors even though p[1:0] and the synthetic top G/K bits are not consumed.
  logic [InWidth:0]   g;
  logic [InWidth:0]   p;
  logic [InWidth:0]   k;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [InWidth-1:0] indicator;
  /* verilator lint_off UNUSEDSIGNAL */  // The original computes the distance in a full 32-bit arithmetic domain, but only the encoded low OutWidth bits are exposed.
  logic [31:0]        count_val;
  /* verilator lint_on UNUSEDSIGNAL */

  assign p[InWidth] = sub_op_i;
  assign g[InWidth] = 1'b0;
  assign k[InWidth] = 1'b0;

  for (genvar n = 0; n < InWidth; ++n) begin : gen_gpk
    assign g[n] = in1_i[n] & in2_i[n];
    assign p[n] = in1_i[n] ^ in2_i[n];
    assign k[n] = ~in1_i[n] & ~in2_i[n];
  end

  assign indicator[0] = 1'b1;

  for (genvar n = 1; n < InWidth; ++n) begin : gen_indicator
    assign indicator[n] =
        (p[n+1]  & ((g[n] & ~k[n-1]) | (k[n] & ~g[n-1]))) |
        (~p[n+1] & ((k[n] & ~k[n-1]) | (g[n] & ~g[n-1])));
  end

  always_comb begin
    count_val = DefaultCount;
    for (int i = 0; i < InWidth; ++i) begin
      if (indicator[i]) begin
        count_val = DefaultCount - i;
      end
    end
    lxd_o = count_val[OutWidth-1:0];
  end

endmodule : txfma_lxd
