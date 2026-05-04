// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f2 
(
  input                                         ctrl_f1_clk,  
  input                                         ctrl_f2_clk,  
  input  [`TXFMA_A3:`TXFMA_B27]                 fa_f2a_h,
  input  [`TXFMA_A3:`TXFMA_B27]                 fc_f2a_h,
  input  [`VPU_DATA_S_SZ-1:0]                   booth_fCin_f2a_h, 
  input  [`VPU_DATA_S_SZ-1:0]                   booth_fSxt_f2a_h, 
  input  [32:0]                                 booth_fpp17_f2a_h,
  input  [`TXFMA_A3:0]                          wallace_fC21_f2a_h, 
  input  [`TXFMA_A3:0]                          wallace_fC22_f2a_h, 
  input  [`TXFMA_A3:0]                          wallace_fS21_f2a_h, 
  input  [`TXFMA_A3:0]                          wallace_fS22_f2a_h,
  input                                         paddin1_sels_f1a_h,
  input                                         paddin2_selc_f1a_h,
  input                                         paddin2_selfcneg_f1a_h,
  input                                         padd_cin_f1a_h,
  input                                         sel_famask_f1a_h,
  output logic [`TXFMA_A3:`TXFMA_B27]           fa_f3a_h,
  output logic [`TXFMA_A3:`TXFMA_B27]           fc_f3a_h,
  output logic [`TXFMA_A3:`TXFMA_B59]           prodsum_f3a_h
);

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////

logic [`TXFMA_A3:`TXFMA_B59]         wallace_fC41_f2a_h; 
logic [`TXFMA_A3:`TXFMA_B59]         wallace_fS41_f2a_h;
logic [`TXFMA_A3:`TXFMA_B59]         ext_fa_f2a_h; 
logic [`TXFMA_A3:`TXFMA_B59]         ext_fc_f2a_h; 
logic [`TXFMA_A3:`TXFMA_B59]         padd_in2_f2a_h; 
logic [`TXFMA_A3:`TXFMA_B59]         padd_in1_f2a_h;
logic [`TXFMA_A3:`TXFMA_B27]         fa_out_f2a_h;
logic [`TXFMA_A3:`TXFMA_B59]         prodsum_f2a_h;
logic                                paddin1_sels_f2a_h;
logic                                paddin2_selc_f2a_h;
logic                                paddin2_selfcneg_f2a_h;
logic                                padd_cin_f2a_h;
logic                                sel_famask_f2a_h;


///////////////////////////////////////////////////////////////////////////////
// F3 flops; Moved from txfma_top 
///////////////////////////////////////////////////////////////////////////////

 //    CLK               DOUT                    DIN
  `FF (ctrl_f1_clk, paddin1_sels_f2a_h,     paddin1_sels_f1a_h     )
  `FF (ctrl_f1_clk, paddin2_selc_f2a_h,     paddin2_selc_f1a_h     )
  `FF (ctrl_f1_clk, paddin2_selfcneg_f2a_h, paddin2_selfcneg_f1a_h )
  `FF (ctrl_f1_clk, padd_cin_f2a_h,         padd_cin_f1a_h         )
  `FF (ctrl_f1_clk, sel_famask_f2a_h,       sel_famask_f1a_h       )
  // Flop O/P Data
  `FF (ctrl_f2_clk, prodsum_f3a_h,          prodsum_f2a_h          )
  `FF (ctrl_f2_clk, fa_f3a_h,               fa_out_f2a_h           )
  `FF (ctrl_f2_clk, fc_f3a_h,               fc_f2a_h               )


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
     case (paddin1_sels_f2a_h)
       1'b0:    padd_in1_f2a_h = ext_fa_f2a_h; 
       1'b1:    padd_in1_f2a_h = wallace_fS41_f2a_h; 
  endcase   
end

//////////////////////////////////////////////////////////////////////////////////////////////
//  Add in2 mux 
//////////////////////////////////////////////////////////////////////////////////////////////
assign ext_fc_f2a_h = {3'b0, fc_f2a_h, 29'b0};

always_comb
begin
     case (paddin2_selc_f2a_h)
       1'b0:    padd_in2_f2a_h = {($bits(ext_fc_f2a_h)){paddin2_selfcneg_f2a_h}} ^ ext_fc_f2a_h; 
       1'b1:    padd_in2_f2a_h = {wallace_fC41_f2a_h[`TXFMA_A2:0], 1'b0};
  endcase   
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
  .a    ( padd_in2_f2a_h ),
  .b    ( padd_in1_f2a_h ),
  .cin  ( padd_cin_f2a_h ),
  .sum  ( prodsum_f2a_h  )
);

///////////////////////////////////////////////////////////////////////////////
//  FA MUX
///////////////////////////////////////////////////////////////////////////////

assign fa_out_f2a_h[`TXFMA_A3:`TXFMA_B19]  = {1'b0,{23{sel_famask_f2a_h}}} | fa_f2a_h[`TXFMA_A3:`TXFMA_B19];
assign fa_out_f2a_h[`TXFMA_B20:`TXFMA_B27] = fa_f2a_h[`TXFMA_B20:`TXFMA_B27];

endmodule
