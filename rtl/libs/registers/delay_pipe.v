// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module delay_pipe
  #(
    parameter WIDTH=3,
    parameter STAGES=3
    )
   (
    input                    clock,
    input                    reset,
    
    input                    in_valid,
    input [WIDTH-1:0]        in_data,
    output logic             in_ready,

    output logic             out_valid,
    output logic [WIDTH-1:0] out_data,
    input                    out_ready
    );

   logic [STAGES-1:0][WIDTH-1:0] data;
   logic [STAGES-1:0]            stall;
   logic [STAGES-1:0]            valid;
   
   always_ff@(posedge clock) begin
      if (reset) begin
         valid <= '0;
      end
      else begin
         if (in_ready && in_valid) begin
            data[0] <= in_data;
         end
         if( in_ready) valid[0] <= in_valid;
         
         for ( int i = 1; i < STAGES; i++) begin
            if (!stall[i]) begin
               data[i] <= data[i-1];
               valid[i] <= valid[i-1];
            end
         end
      end // else: !if(reset)
   end // always_ff@

   
   always_comb begin
      out_valid = valid[STAGES-1];
      out_data = data[STAGES-1];
      
      stall[STAGES-1] = out_valid && !out_ready;
      
      for ( int i = STAGES-2; i >= 0; i--) begin
         stall[i] = valid[i] && stall[i+1];
      end
      
      in_ready = !stall[0];
   end
   
endmodule // delay_pipe
