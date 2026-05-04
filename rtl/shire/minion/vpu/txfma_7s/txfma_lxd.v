// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_lxd 
#(
  parameter in_width         = `TXFMA_PS_DPATH_SZ, 
  parameter aft_a0_bits_num  = `TXFMA_A3-`TXFMA_A0+1,   
  parameter out_width        = `TXFMA_LXD_PS_SZ
)
(
  input         [in_width-1:0]        in1, 
  input         [in_width-1:0]        in2,
  input                               sub_op,
  output logic  [out_width-1:0]       lxd
);

logic [in_width:0]   G,P,K;
logic [in_width-1:0] indicator;



  //Propagate, Generate and Kill vectors
assign P[in_width] = sub_op;
assign G[in_width] = 1'b0;
assign K[in_width] = 1'b0;
 
genvar n;
generate
  for(n=0; n<in_width; n=n+1) begin:lzd
    assign G[n] = in1[n] & in2[n];
    assign P[n] = in1[n] ^ in2[n];
    assign K[n] = ~in1[n] & ~in2[n];
  end
endgenerate


  //Indicator vector

  //LSB 
assign indicator[0] = 1'b1;

    //bits in_width-1:0 = pattern matching for each index (i+1,i,i-1)  = (PG~K | ~PK~K) | (PK~G | ~PG~G)
generate
  for(n=in_width-1; n>0; n=n-1) begin:indicators
    assign indicator[n]=(  P[n+1] & ( (G[n] & ~K[n-1]) | (K[n] & ~G[n-1]) ) ) |
                        ( ~P[n+1] & ( (K[n] & ~K[n-1]) | (G[n] & ~G[n-1]) ) );
  end
endgenerate


  //shift count
always_comb begin
  lxd = {1'b0,$unsigned(in_width)};
  for (integer i=0; i<in_width; i=i+1)
    if (indicator[i]==1) lxd = $unsigned(in_width)-$unsigned(aft_a0_bits_num)-$unsigned(i);
end 

endmodule
