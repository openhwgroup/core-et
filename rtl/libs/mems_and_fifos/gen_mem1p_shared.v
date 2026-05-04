// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

module gen_mem1p_shared #(parameter WIDTH=20, parameter WORDS=16, parameter ABITS=4,
                      parameter READERS=1, parameter WRITERS=1 )
   ( 
     input                          clock,
     input                          reset,
     
     // read ports
     input [READERS-1:0]            rd_valid,
     output [READERS-1:0]           rd_ready,
     input [READERS-1:0][ABITS-1:0] rd_address,
     output [WIDTH-1:0]             rd_data,
     output [READERS-1:0]           rd_data_valid,
     
     //write ports     
     input [WRITERS-1:0]            wr_valid,
     output [WRITERS-1:0]           wr_ready,
     input [WRITERS-1:0][ABITS-1:0] wr_address,
     input [WRITERS-1:0][WIDTH-1:0] wr_data
     );
   
   localparam ARB_SZ = READERS + WRITERS;
   localparam ARB_L = $clog2(ARB_SZ);

   // memory instance      
   wire [ABITS-1:0] address;
   wire               en;
   wire               we;
   wire [WIDTH-1:0]   din;
   wire [WIDTH-1:0]   dout;
   
   gen_mem1p #( .WIDTH(WIDTH), .WORDS(WORDS) ) u_mem
     (
      .clock(clock),
      .en(en),
      .we(we),
      .address(address),
      .din(din),
      .dout(dout)
      );

   // mux accesses (round robin)
   wire [ARB_SZ-1:0]  valid;
   reg [ARB_SZ-1:0]   mask;
   wire [ARB_L-1:0] arbSel;
   reg [READERS-1:0] readValid;
   `RST_EN_FF(clock, reset, |valid, mask,  ~(1<<arbSel), {(ARB_SZ) {1'b0}})
   `RST_FF(clock, reset, readValid, rd_valid & rd_ready,  {(READERS) {1'b0}})
   
   assign arbSel = roundRobin(valid, mask);
   assign valid = { wr_valid, rd_valid };
   assign en = | valid;
   assign we = arbSel >= READERS;
   assign address = we? wr_address[arbSel-READERS] : rd_address[arbSel];
   assign din = wr_data[arbSel-READERS];
   assign rd_data = dout;
   assign {wr_ready, rd_ready} = 1'b1<<arbSel;
   assign rd_data_valid = readValid;
      
   function automatic [ARB_L-1:0] roundRobin;
      input [ARB_SZ-1:0]   valid;
      input [ARB_SZ-1:0]   mask;
      integer              i;
      begin
         roundRobin = ARB_SZ-1;
         for (i=ARB_SZ-2; i>=0; i=i-1)
           if(valid[i]) roundRobin = i[ARB_L-1:0];
         for (i=ARB_SZ-1; i>=0; i=i-1)
           if(valid[i] && mask[i]) roundRobin = i[ARB_L-1:0];
      end
   endfunction
   
endmodule // mem1p_shared
