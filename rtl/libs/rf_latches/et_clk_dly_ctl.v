// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_dly_ctl.v
//   controlable delay for clock 

module et_clk_dly_ctl  (
   input        clk_in,
   output       clk_out,
   input [5:0]  sel_taps
);

`ifdef ZEBU
  assign clk_out = clk_in; 
`else 
  et_clk_dly_mux 
  clk_clk_dly_mux (
     .clk_out ( clk_out   ),
     .clk_in  ( clk_in    ),
     .s       ( sel_taps  )
  );      
`endif

endmodule

