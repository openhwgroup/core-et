// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// wrapper for one of et_clk_gate or et_clk_gate_n depending on parameters


module esr_clk_gate #( parameter EDGE_ = 1)
   (
   output clk /*verilator clocker*/,
   input  en,
   input  eclk,
   input  testmode
    );

   generate
      if ( EDGE_ == 0) begin: LBUF
         et_clk_gate_n u_buf (
                       .enclk_n(clk),
                       .en_1p(en),
                       .clk(eclk),
                       .te(testmode)
                       );
      end
      else if ( EDGE_ == 1) begin: LBUF
         et_clk_gate u_buf (
                     .enclk(clk),
                     .en(en),
                     .clk(eclk),
                     .te(testmode)
                     );
      end
   endgenerate
   
endmodule // esr_clk_gate




                                                       


   

   

