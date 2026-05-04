// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_write_commit_rst_en #(
  parameter int unsigned      Width      = 1,
  parameter logic [Width-1:0] ResetValue = '0
) (
  input  logic             clk_i,
  input  logic             rst_i,
  input  logic             en_i,
  input  logic [Width-1:0] d_i,
  output logic [Width-1:0] q_o
);

  always_ff @(negedge clk_i or posedge rst_i) begin
    if (rst_i) begin
      q_o <= ResetValue;
    end else if (en_i) begin
      q_o <= d_i;
    end
  end

endmodule
