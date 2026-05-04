// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_4_2_compressor
(
  input x0,
  input x1,
  input x2,
  input x3,
  input cin,
  output logic cout,
  output logic c,
  output logic s 
);

logic s0;

txfma_csa #1 csa0(x0, x1, x2, cout, s0);
txfma_csa #1 csa1(s0, x3, cin, c, s);

endmodule
