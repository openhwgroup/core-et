// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _LOG2_
`define _LOG2_

//returns number of bits to represent from 0 to v-1
// if v is 1, it returns 1 rather than 0
// 0 and negative values return 0
function automatic integer log2;
   input integer v; 
   begin
      if (v<=0) log2 = 0;
      else if (v==1) log2 = 1;
      else begin
         v=v-1;
         for (log2=0; v>0; log2=log2+1) 
           v = v>>1;
      end
   end 
endfunction 

`endif // _LOG2_

