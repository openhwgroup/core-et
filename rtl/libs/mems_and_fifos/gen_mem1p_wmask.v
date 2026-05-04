// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// generic memory 1 port memory with bit write mask
module gen_mem1p_wmask
( clock, address, en, we, wmask, din, dout );
   
   // size parameters
   parameter WIDTH = 32;
   parameter WORDS = 256;
//synopsys translate_off
   parameter ALLOW_RESET = 0;
//synopsys translate_on
   localparam AD_BITS = $clog2(WORDS);
   
   // ports
   input clock;
   input [AD_BITS-1:0] address;
   input               en;
   input               we;
   input [WIDTH-1:0]   wmask;
   input [WIDTH-1:0]   din;
   output reg [WIDTH-1:0]  dout;
   
   // implementation
   reg [WIDTH-1:0] mem[WORDS-1:0];

   //synopsys translate_off
   reg [WIDTH-1:0] reset_mem[WORDS-1:0];
   reg             reset_flag;
   always_comb begin
      for (int i = 0 ; i < WORDS; i++)
        reset_mem[i] = '0;
   end
   //synopsys translate_on
  
   always @(posedge clock) begin
      //synopsys translate_off
      if (reset_flag === 1'b1 && ALLOW_RESET) begin
         $display("backdoor resetting contents of %m");
         mem <= reset_mem;
         reset_flag <= 0;
      end
      //synopsys translate_on
      
      if (en) begin
         if(we) mem[address] <= (mem[address] & ~wmask) | (din & wmask);
         else dout<=mem[address];
      end
   end // always @ (posedge clock)
   
   //synopsys translate_off
   task reset;
      begin
         reset_flag <= 1;
      end
   endtask // reset
   //synopsys translate_on
   
endmodule // gen_mem1p
