// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_tdr_reg
//   
module et_tdr_reg #(parameter
   WIDTH    = 150,
   READ_TDR = 1
) (
   input  logic             tdr_tck,
   input  logic             tdr_trstn,
   input  logic             tdr_ena,
   input  logic             tdr_shiftdr, 
   input  logic             tdr_updatedr,  
   input  logic             tdr_capturedr, 
   input  logic             tdr_tdi,
   output logic             tdr_tdo,
   input  logic [WIDTH-1:0] tdr_in,
   output logic [WIDTH-1:0] tdr_out
);


   logic [WIDTH-1:0]  tdr_shift;
   logic [WIDTH-1:0]  tdr_capture;

   assign tdr_capture = READ_TDR ? tdr_in : tdr_out;

   assign tdr_tdo = tdr_shift[0];

   always @ (posedge tdr_tck or negedge tdr_trstn) begin
      if (!tdr_trstn) begin
         tdr_shift <= {WIDTH{1'b0}};
      end
      else if (tdr_ena) begin
         tdr_shift <= tdr_capturedr ? tdr_capture : (tdr_shiftdr ? {tdr_tdi, tdr_shift[WIDTH-1:1]} : tdr_shift);
      end
   end

   always @ (negedge tdr_tck or negedge tdr_trstn) begin
      if (!tdr_trstn) begin
	       tdr_out <= {WIDTH{1'b0}};
      end
      else if (tdr_ena) begin
	       tdr_out <= tdr_updatedr ? tdr_shift : tdr_out;
      end
   end

endmodule
