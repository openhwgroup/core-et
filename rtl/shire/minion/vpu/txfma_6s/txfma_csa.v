// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_csa
#(
parameter bits = 4
)
(a,b,cin,cout,sum);


input wire [bits-1:0] a;
input wire [bits-1:0] b;
input wire [bits-1:0] cin;
output wire [bits-1:0] cout;
output wire [bits-1:0] sum;

assign sum = a^b^cin;
assign cout = (a&b) | (a&cin) | (b&cin);

endmodule
