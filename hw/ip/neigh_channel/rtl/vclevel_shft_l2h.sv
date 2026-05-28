// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vclevel_shft_l2h #(
  parameter int unsigned WIDTH = 1
) (
  input  logic [WIDTH-1:0] inp_lv,
  output logic [WIDTH-1:0] out_hv
);
  assign out_hv = inp_lv;
endmodule : vclevel_shft_l2h
