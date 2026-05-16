// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vclevel_shft_h2l #(
  parameter int unsigned WIDTH = 1
) (
  input  logic [WIDTH-1:0] inp_hv,
  output logic [WIDTH-1:0] out_lv
);
  assign out_lv = inp_hv;
endmodule : vclevel_shft_h2l
