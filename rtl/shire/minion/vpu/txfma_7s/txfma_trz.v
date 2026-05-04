// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_trz 
# (parameter trz_out = `TXFMA_ETRZ_PS_SZ, parameter max = `FLOAT32_M)
(
  input [max:0] frac,
  output logic [trz_out-1:0] trz
);

always_comb begin
  trz = max+1'b1;  // by default we assume that the input operand is '0'
  for (integer i=max; i>=0; i=i-1)
    if (frac[i]==1) trz = $unsigned(i);  
end 

endmodule
