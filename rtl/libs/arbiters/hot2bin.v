// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements a one-hot to binary translate

module hot2bin #(parameter 
   HOT_W=8,
   BIN_W=$clog2(HOT_W)
) (
   output logic [BIN_W-1:0] bin,
   input  logic [HOT_W-1:0] hot
);

   logic [HOT_W-1:0][BIN_W-1:0] bin_array;

   always_comb begin : bin_array_block
      integer ii;
      for (ii=0; ii<HOT_W; ii=ii+1) begin
         bin_array[ii] = ii[BIN_W-1:0];
      end
   end

   onehot_mux #( 
      .WIDTH(BIN_W),
      .NUM_OH_SEL(HOT_W)
   ) u_onehot_mux (
      .data_out ( bin ),
      .sel      ( hot ),
      .data_in  ( bin_array)
   );

endmodule
