// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f2 
(
  input  [`VPU_DATA_S_SZ-1:0]                   fa_f2a_h,
  input  [`VPU_DATA_S_SZ-1:0]                   fc_f2a_h,
  input  [`VPU_FCMD_SZ-1:0]                     cmd_f2a_h,
  input  [`VPU_DATA_S_SZ-1:0]                   booth_fCin_f2a_h, 
  input  [`VPU_DATA_S_SZ-1:0]                   booth_fSxt_f2a_h, 
  input  [32:0]                                 booth_fpp17_f2a_h,
  input  [`TXFMA_A3:0]                          wallace_fC21_f2a_h, 
  input  [`TXFMA_A3:0]                          wallace_fC22_f2a_h, 
  input  [`TXFMA_A3:0]                          wallace_fS21_f2a_h, 
  input  [`TXFMA_A3:0]                          wallace_fS22_f2a_h,
  input                                         use_product_f2a_h,
  input                                         fc_neg_f2a_h,
  output logic [`TXFMA_A3:0]                    prodsum_f2a_h,
  output logic [`VPU_DATA_S_SZ-1:0]             fa_out_f2a_h
);
  

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [`TXFMA_A3:0]                  wallace_fC41_f2a_h; 
logic [`TXFMA_A3:0]                  wallace_fS41_f2a_h;
logic [`TXFMA_A3:0]                  ext_fa_f2a_h; 
logic [`TXFMA_A3:0]                  ext_fc_f2a_h; 
logic [`TXFMA_A3:0]                  ain_f2a_h; 
logic [`TXFMA_A3:0]                  bin_f2a_h;


///////////////////////////////////////////////////////////////////////////////
//  Wallace2 
///////////////////////////////////////////////////////////////////////////////

txfma_wallace2 
wallace2
(
  .Sxt          ( booth_fSxt_f2a_h   ),
  .Cin          ( booth_fCin_f2a_h   ),
  .pp17         ( booth_fpp17_f2a_h  ),
  .C21          ( wallace_fC21_f2a_h ), 
  .C22          ( wallace_fC22_f2a_h ), 
  .S21          ( wallace_fS21_f2a_h ), 
  .S22          ( wallace_fS22_f2a_h ),
  .C41          ( wallace_fC41_f2a_h ),
  .S41          ( wallace_fS41_f2a_h )
);

///////////////////////////////////////////////////////////////////////////////
//  Add in1 mux 
///////////////////////////////////////////////////////////////////////////////
//  Extending fa to the datapath width with zeros
assign ext_fa_f2a_h = {3'b0, fa_f2a_h, 29'b0};

always_comb
begin
  ain_f2a_h = ext_fa_f2a_h;

  if (use_product_f2a_h)
    ain_f2a_h = wallace_fS41_f2a_h;
end     

//////////////////////////////////////////////////////////////////////////////////////////////
//  Add in2 mux 
//////////////////////////////////////////////////////////////////////////////////////////////
assign ext_fc_f2a_h = {3'b0, fc_f2a_h, 29'b0};

always_comb
begin
  bin_f2a_h = {($bits(ext_fc_f2a_h)){fc_neg_f2a_h}} ^ ext_fc_f2a_h;
  
  if (use_product_f2a_h)
    bin_f2a_h = {wallace_fC41_f2a_h[`TXFMA_A2:0], 1'b0};
end

///////////////////////////////////////////////////////////////////////////////
//  Product adder (64)
///////////////////////////////////////////////////////////////////////////////
txfma_adder
#(
  .WIDTH( `TXFMA_A3+1 )
)
prod_adder_64b
(
  .a    ( ain_f2a_h      ),
  .b    ( bin_f2a_h      ),
  .cin  ( fc_neg_f2a_h   ),
  .sum  ( prodsum_f2a_h  )
);

///////////////////////////////////////////////////////////////////////////////
//  FA mux
///////////////////////////////////////////////////////////////////////////////
assign fa_out_f2a_h = ((cmd_f2a_h == `VPU_FCMD_FFRC) | (cmd_f2a_h==`VPU_FCMD_FFRC_SIN)) ? `VPU_DATA_S_SZ'h7fffffff : fa_f2a_h;

endmodule
