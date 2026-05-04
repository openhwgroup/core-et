// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module leading_zero_counter #(parameter N_BITS = 8) (n, leading_zeros, is_zero);

// LOCAL PARAMETERS
localparam LZ_BITS = $clog2(N_BITS);
localparam MAX_COUNT = N_BITS-1;

// INPUT/OUTPUT INTERFACE
input logic [N_BITS-1:0] n;
output logic [LZ_BITS-1:0] leading_zeros;
output logic is_zero;

logic [N_BITS-1:0][LZ_BITS-1:0] tmp;

always_comb begin

   tmp[N_BITS-1] = MAX_COUNT[LZ_BITS-1:0];

   for (integer i = N_BITS-2; i >= 0 ; i--)
      tmp[i] = n[N_BITS-1-i] ? i[LZ_BITS-1:0] : tmp[i+1];
end

// If all bits are zero, the final shift value is not important
assign leading_zeros = tmp[0];
assign is_zero = n == {N_BITS{1'b0}};

endmodule
