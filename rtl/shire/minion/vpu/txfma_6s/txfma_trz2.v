// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_trz2 
#(
  parameter in_width=`TXFMA_PS_DPATH_SZ,
  parameter out_width=`TXFMA_WSPS_ETRZ_SZ
)
(
  input         [in_width-1:0]  in1,
  input         [in_width-1:0]  in2,
  input                         add_cin,
  output logic  [out_width-1:0] trz
);


//P(b45) & ~CIN | ~P(b45) & CIN

//i:A3:b44
//TRZ(i)	P(i) & K(i-1) |~P(i) & ~K(i-1)


logic [in_width-1:0] P,K;
logic [in_width-1:0] indicator;


  //Propagate and Kill vectors 
genvar n;
generate
  for(n=0; n<in_width; n=n+1) begin:lzd
    assign P[n] = in1[n] ^ in2[n];
    assign K[n] = ~in1[n] & ~in2[n];
  end
endgenerate


  //Indicator vector
assign indicator[0] = (P[0] ^ add_cin);

    //bits in_width-1:0 = pattern matching for each index (i,i-1)  = P(i) & K(i-1) | ~P(i) & ~K(i-1)
generate
  for(n=in_width-1; n>0; n=n-1) begin:indicators
    assign indicator[n]=( ( P[n] & K[n-1] ) | ( ~P[n] & ~K[n-1]) );
  end
endgenerate


  //shift count
always_comb begin
  trz = $unsigned(in_width);  // by default we assume that all mantissa are '0'
  for (integer i=in_width-1; i>=0; i=i-1)
    if (indicator[i]==1) trz = $unsigned(i);
end 


endmodule
