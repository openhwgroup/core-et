// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements a binary to one-hot translate

module bin2hot #(parameter 
   HOT_W=8,
   BIN_W=$clog2(HOT_W)
) (
   output logic [HOT_W-1:0] hot,
   input  logic [BIN_W-1:0] bin
);

   always_comb begin : hot_array_block
      hot = '0;
      hot[bin] = 1'b1;
   end

endmodule
