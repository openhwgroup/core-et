// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_booth_ppg_32r4_msb(mulcand,msb,sign,pp);
input wire[`VPU_DATA_S_SZ-1:0] mulcand;
input wire msb;
input wire sign;

output wire[32:0] pp;

assign pp = ((sign == 0)&&(msb == 1))? {1'b0, mulcand} : 33'b0;
    
endmodule
