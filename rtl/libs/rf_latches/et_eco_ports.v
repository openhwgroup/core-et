// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_eco_ports
//    wrapper module for extra ECO ports that are attached
//    to "DONT_TOUCH bufs.  
//    - input ports are assigned low at instantiation before driving input bufs
//    - output bufs are assigned low before connecting to output ports

module et_eco_ports #(parameter 
   INPUT_WIDTH  = 4,
   OUTPUT_WIDTH = 4
) ( 
   output logic [OUTPUT_WIDTH-1:0] eco_o,
   input  logic [INPUT_WIDTH-1:0]  eco_i
);
   

`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_eco_ports
         logic [INPUT_WIDTH-1:0] eco_i_in;
         et_combo_buf eco_o_buf [OUTPUT_WIDTH-1:0] (.x(eco_o),    .a(1'b0) );
         et_combo_buf eco_i_buf [INPUT_WIDTH-1:0]  (.x(eco_i_in), .a(eco_i));
         wire unused_ok = &{ // ri lint_check_waive UNARY_CONCAT
      `ifndef ET_ASCENT_LINT
            1'b0,
      `endif
            eco_i_in
         };
      end
   endgenerate
`else
   assign eco_o = '0;
   wire unused_ok = &{ // ri lint_check_waive UNARY_CONCAT
`ifndef ET_ASCENT_LINT
      1'b0,
`endif
      eco_i
   };
`endif

endmodule

