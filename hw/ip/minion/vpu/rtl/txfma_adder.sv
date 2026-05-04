// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_adder #(
  parameter int unsigned Width = 0
) (
  input  logic [Width-1:0] a_i,
  input  logic [Width-1:0] b_i,
  input  logic             cin_i,
  output logic [Width-1:0] sum_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // The original keeps the carry-out in a widened temporary and then intentionally drops it.
  logic [Width:0] sum_tmp;
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    sum_tmp = a_i + b_i + {{(Width-1){1'b0}}, cin_i};
  end

  assign sum_o = sum_tmp[Width-1:0];

endmodule : txfma_adder
