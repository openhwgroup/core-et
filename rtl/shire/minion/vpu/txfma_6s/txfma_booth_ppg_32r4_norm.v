// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_booth_ppg_32r4_norm(mulcand, r4input, sign, pp);
input wire [`VPU_DATA_S_SZ-1:0] mulcand;
input wire [2:0] r4input;
input wire sign;

output logic[32:0] pp;

logic[4:0] booth;

always_comb
  case (r4input) 
    3'b000, 3'b111 : booth = 5'b00001;
    3'b001, 3'b010 : booth = 5'b00010;
    3'b101, 3'b110 : booth = 5'b00100;
    3'b011         : booth = 5'b01000;
    3'b100         : booth = 5'b10000;
  endcase

always_comb
  case (booth)
    5'b00001: pp = 33'h0;// ^ {33{r4input[2]}};
    5'b00010: pp = (sign == 0) ? {1'b0, mulcand[`VPU_DATA_S_SZ-1:0]} : {mulcand[31], mulcand[`VPU_DATA_S_SZ-1:0]};
    5'b00100: pp = ~((sign == 0) ? {1'b0, mulcand[`VPU_DATA_S_SZ-1:0]} : {mulcand[31], mulcand[`VPU_DATA_S_SZ-1:0]});
    5'b01000: pp = {mulcand[`VPU_DATA_S_SZ-1:0], 1'b0}; 
    5'b10000: pp = ~({mulcand[`VPU_DATA_S_SZ-1:0], 1'b0});
    default:  pp = 33'h0;
  endcase
  
/*assign pp[32:0] = ((r4input[1]^r4input[0] == 0)?
                   ((r4input[2]^r4input[0] == 0)?0:{mulcand[`VPU_DATA_S_SZ-1:0], 1'b0}):
                   ((sign == 0)?{1'b0, mulcand[`VPU_DATA_S_SZ-1:0]}:{mulcand[31], mulcand[`VPU_DATA_S_SZ-1:0]})) ^ {33{r4input[2]}};
*/    
    
endmodule
