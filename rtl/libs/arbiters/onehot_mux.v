// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// This module implements a one-hot mux select.  
// If sel=0, then data_out=0
// NOTE: mostly used in arb_rr_data so kept in the libs/arbiters directory

module onehot_mux #(parameter 
   WIDTH=8,
   NUM_OH_SEL=4
) (
   output logic [WIDTH-1:0]            data_out,
   input  logic [NUM_OH_SEL-1:0]       sel,
   input  logic [NUM_OH_SEL*WIDTH-1:0] data_in
);


   always_comb begin : oh_sel_block
      integer ii;
      data_out = {WIDTH {1'b0}};
      for (ii=0; ii<NUM_OH_SEL; ii=ii+1) begin
         data_out = data_out | ({WIDTH {sel[ii]}} & data_in[ii*WIDTH +: WIDTH]);
      end
   end

endmodule
