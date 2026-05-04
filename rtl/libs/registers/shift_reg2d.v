// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Parameterized shift register.
// It can be used for synopsys retiming

module shift_reg2d #(parameter latency=2, parameter width=32, parameter syncReset=1)
   (
    input              clk, reset, enableIn,
    input [width-1:0]  data_in,
    output             enableOut,
    output [width-1:0] data_out
    );
   
   logic [latency:0] [width-1:0]                                                                         connect_data;
   logic [latency:0]                                                                                     connect_enable;
   
   assign data_out = connect_data[latency];
   assign enableOut = connect_enable[latency];
   assign connect_data[0] = data_in;
   assign connect_enable[0] = enableIn;
   
   genvar                                                                                                i;
   generate
      for (i=1; i <= latency; i=i+1)
        begin
           if (syncReset) begin
              sync_reg #(width) Reg(clk, reset, connect_enable[i-1],  connect_data[i-1], connect_enable[i], connect_data[i]);
           end else begin
              async_reg #(width) Reg(clk, reset, connect_enable[i-1], connect_data[i-1], connect_enable[i], connect_data[i]);         
           end
        end
   endgenerate

endmodule
