// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_ioshire_noc_ints #(
  parameter int unsigned NocIntNum = shire_channel_leaves_pkg::NocIntNum
) (
  input  logic                         clk_noc_i,
  input  logic                         rst_noc_ni,
  input  logic [NocIntNum-1:0]         noc_err_int_srcs_i,
  output logic [NocIntNum-1:0]         noc_all_err_int_srcs_o,
  output logic                         ioshire_noc_err_int_o
);

  assign noc_all_err_int_srcs_o = noc_err_int_srcs_i;

  always_ff @(posedge clk_noc_i or negedge rst_noc_ni) begin
    if (!rst_noc_ni) begin
      ioshire_noc_err_int_o <= 1'b0;
    end else begin
      ioshire_noc_err_int_o <= |noc_err_int_srcs_i;
    end
  end

endmodule
