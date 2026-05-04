// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_booth_ppg_32r4
(
  input                              mode, 
  input         [`VPU_DATA_S_SZ-1:0] mpcnd,
  input         [`VPU_DATA_S_SZ-1:0] mplr,
  input                              sign,
  output logic  [`VPU_DATA_S_SZ-1:0] Sxt,
  output logic  [`VPU_DATA_S_SZ-1:0] Cin, 
  output        [32:0]               pp1, 
  output        [32:0]               pp2, 
  output        [32:0]               pp3, 
  output        [32:0]               pp4, 
  output        [32:0]               pp5, 
  output        [32:0]               pp6, 
  output        [32:0]               pp7, 
  output        [32:0]               pp8, 
  output        [32:0]               pp9, 
  output        [32:0]               pp10, 
  output        [32:0]               pp11, 
  output        [32:0]               pp12, 
  output        [32:0]               pp13, 
  output        [32:0]               pp14, 
  output        [32:0]               pp15, 
  output        [32:0]               pp16, 
  output        [32:0]               pp17
);


logic [34:0] ext_mplr;
logic [33:0] ext_sxt;
logic [33:0] ext_cin;
logic [32:0] pp1_raw; 
logic [32:0] pp2_raw; 
logic [32:0] pp3_raw; 
logic [32:0] pp4_raw; 
logic [32:0] pp5_raw; 
logic [32:0] pp6_raw; 
logic [32:0] pp7_raw; 
logic [32:0] pp8_raw; 
logic [32:0] pp9_raw; 
logic [32:0] pp10_raw; 
logic [32:0] pp11_raw; 
logic [32:0] pp12_raw; 
logic [32:0] pp13_raw; 
logic [32:0] pp14_raw; 
logic [32:0] pp15_raw; 
logic [32:0] pp16_raw; 
logic [32:0] pp17_raw; 
logic ph;

assign ph = mode; 

assign ext_mplr = {2'b0, mplr, 1'b0};

// cin, sxt
genvar i;
generate
for (i=0; i < 17; i=i+1) begin
  assign ext_cin[2*i] = ext_mplr[2*i+2] & !(ext_mplr[2*i+2] & ext_mplr[2*i+1] & ext_mplr[2*i]);
  assign ext_cin[2*i+1] = 1'b0;
  assign ext_sxt[2*i] = ~((ext_mplr[2*i+2] ^ (mpcnd[31] & sign)) & ~((ext_mplr[2*i+:3] == 3'b111) | (ext_mplr[2*i+:3] == 3'b000)));
  assign ext_sxt[2*i+1] = 1'b1;
end
endgenerate

always_comb
begin
  Sxt = {ext_sxt[30:0], ext_cin[32]};
  Cin = ext_cin[`VPU_DATA_S_SZ-1:0];
    
  if (ph) begin
    Sxt = {ext_sxt[30:20], 2'b0, ext_cin[32:20], 6'h0};
    Cin = {7'h0, ext_sxt[11:0], ext_cin[12:0]}; 
  end
end


txfma_booth_ppg_32r4_norm ppg1
( .mulcand(mpcnd), 
  .r4input({mplr[1:0],1'b0}), 
  .sign(sign), 
  .pp(pp1_raw)
);

txfma_booth_ppg_32r4_norm ppg2(
  .mulcand(mpcnd), 
  .r4input(mplr[3:1]),        
  .sign(sign), 
  .pp(pp2_raw)
);

txfma_booth_ppg_32r4_norm ppg3( 
  .mulcand(mpcnd), 
  .r4input(mplr[5:3]),        
  .sign(sign), 
  .pp(pp3_raw)
);

txfma_booth_ppg_32r4_norm ppg4( 
  .mulcand(mpcnd), 
  .r4input(mplr[7:5]),        
  .sign(sign), 
  .pp(pp4_raw)
);

txfma_booth_ppg_32r4_norm ppg5( 
  .mulcand(mpcnd), 
  .r4input(mplr[9:7]),        
  .sign(sign), 
  .pp(pp5_raw)
);

txfma_booth_ppg_32r4_norm ppg6( 
  .mulcand(mpcnd), 
  .r4input(mplr[11:9]),       
  .sign(sign), 
  .pp(pp6_raw)
);

txfma_booth_ppg_32r4_norm ppg7( 
  .mulcand(mpcnd), 
  .r4input(mplr[13:11]),      
  .sign(sign), 
  .pp(pp7_raw)
);

txfma_booth_ppg_32r4_norm ppg8( 
  .mulcand(mpcnd), 
  .r4input(mplr[15:13]),      
  .sign(sign), 
  .pp(pp8_raw)
);

txfma_booth_ppg_32r4_norm ppg9( 
  .mulcand(mpcnd), 
  .r4input(mplr[17:15]),      
  .sign(sign), 
  .pp(pp9_raw)
);

txfma_booth_ppg_32r4_norm ppg10(
  .mulcand(mpcnd), 
  .r4input(mplr[19:17]),      
  .sign(sign), 
  .pp(pp10_raw)
);

txfma_booth_ppg_32r4_norm ppg11(
  .mulcand(mpcnd), 
  .r4input(mplr[21:19]),      
  .sign(sign), 
  .pp(pp11_raw)
);

txfma_booth_ppg_32r4_norm ppg12(
  .mulcand(mpcnd), 
  .r4input(mplr[23:21]),        
  .sign(sign), 
  .pp(pp12_raw)
);

txfma_booth_ppg_32r4_norm ppg13(
  .mulcand(mpcnd), 
  .r4input(mplr[25:23]),      
  .sign(sign), 
  .pp(pp13_raw)
);

txfma_booth_ppg_32r4_norm ppg14(
  .mulcand(mpcnd), 
  .r4input(mplr[27:25]),      
  .sign(sign), 
  .pp(pp14_raw)
);

txfma_booth_ppg_32r4_norm ppg15(
  .mulcand(mpcnd), 
  .r4input(mplr[29:27]),      
  .sign(sign), 
  .pp(pp15_raw)
);

txfma_booth_ppg_32r4_norm ppg16(
  .mulcand(mpcnd), 
  .r4input(mplr[31:29]),      
  .sign(sign), 
  .pp(pp16_raw)
);

txfma_booth_ppg_32r4_msb  ppg17(
  .mulcand(mpcnd), 
  .msb(mplr[31]),             
  .sign(sign), 
  .pp(pp17_raw)
);


assign pp1 = pp1_raw & {{20{~ph}} , 13'h1fff};
assign pp2 = pp2_raw & {{20{~ph}} , 13'h1fff};
assign pp3 = pp3_raw & {{20{~ph}} , 13'h1fff};
assign pp4 = pp4_raw & {{20{~ph}} , 13'h1fff};
assign pp5 = pp5_raw & {{20{~ph}} , 13'h1fff};
assign pp6 = pp6_raw & {{20{~ph}} , 13'h1fff};
assign pp7 = pp7_raw & {{20{~ph}} , 13'h1fff};
assign pp8 = pp8_raw;
assign pp9 = pp9_raw;
assign pp10 = pp10_raw;
assign pp11 = pp11_raw & {15'h7fff, {18{~ph}}};
assign pp12 = pp12_raw & {15'h7fff, {18{~ph}}};
assign pp13 = pp13_raw & {15'h7fff, {18{~ph}}};
assign pp14 = pp14_raw & {15'h7fff, {18{~ph}}};
assign pp15 = pp15_raw & {15'h7fff, {18{~ph}}};
assign pp16 = pp16_raw & {15'h7fff, {18{~ph}}};
assign pp17 = pp17_raw & {15'h7fff, {18{~ph}}};


endmodule
