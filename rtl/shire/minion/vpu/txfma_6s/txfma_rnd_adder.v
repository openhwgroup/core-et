// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_rnd_adder 
#(
  parameter left_bit     = `TXFMA_A0, 
  parameter right_bit    = `TXFMA_B31,
  parameter rnd_ctl_sz   = `TXFMA_RND_CTL_HI_SZ,
  parameter ftz_sz       = `TXFMA_FTZ_HI_SZ,
  parameter rnd_ps       = 1  
) 
(
  input  [left_bit:right_bit]         in,
  input  [rnd_ctl_sz-1:0]             rnd_ctl,
  input  [ftz_sz-1:0]                 ftz,
  output logic [left_bit:right_bit+1] out
);

logic [left_bit:right_bit] round_vector;
logic [left_bit:right_bit] rnd_adder_sum;


  //round vector
generate 
  if (rnd_ps == 1)
  begin : ROUND_VECTOR_PS_PH_INT 
    always_comb begin
      round_vector = '0;

      round_vector[`TXFMA_PH_HI_L:`TXFMA_PH_HI_R] = rnd_ctl[5:4]; //ph hi
      round_vector[`TXFMA_PS_L:`TXFMA_PS_R]       = rnd_ctl[3:2]; //ps               
      round_vector[`TXFMA_INT_L:`TXFMA_INT_R]     = rnd_ctl[1:0]; //int
    end 
  end else begin
    always_comb begin
      round_vector = '0;
      round_vector[`TXFMA_PH_LO_L:`TXFMA_PH_LO_R] = rnd_ctl[1:0]; //ph lo
    end
  end
endgenerate


///////////////////////////////////////////////////////////////////////////////
// round adder 
///////////////////////////////////////////////////////////////////////////////
localparam rnd_adder_width = left_bit-right_bit+1;
txfma_adder
#(
  .WIDTH(rnd_adder_width)
)
rnd_adder
(
  .a    (in),
  .b    (round_vector),
  .cin  (1'b0),
  .sum  (rnd_adder_sum)  
);

  //clear L bit
generate 
  if (rnd_ps == 1)
  begin : CLEAR_L_BIT 
    always_comb begin
      out = rnd_adder_sum[left_bit:right_bit+1];
      out[`TXFMA_PH_HI_L] = rnd_adder_sum[`TXFMA_PH_HI_L] & !ftz[2]; //ph hi
      out[`TXFMA_PS_L]    = rnd_adder_sum[`TXFMA_PS_L]    & !ftz[1]; //ps
      out[`TXFMA_INT_L]   = rnd_adder_sum[`TXFMA_INT_L]   & !ftz[0]; //int
    end 
  end else begin
    always_comb begin
      out = rnd_adder_sum[left_bit:right_bit+1];
      out[`TXFMA_PH_LO_L] = rnd_adder_sum[`TXFMA_PH_LO_L] & !ftz[0]; //ph lo
    end
  end
endgenerate


endmodule
