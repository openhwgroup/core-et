`include "soc.vh"
module test;
   logic [`INST_RANGE] in_bits;
   logic [`INST_RANGE] out_bits;
   
   frontend_rvc_expander dut(.*);


   initial begin
      in_bits='x;
      for (int lsb=0; lsb < 3; lsb++) begin
         in_bits[1:0] = lsb;
         for (int msb = 0; msb < (1<<14); msb++) begin
            in_bits[15:2] = msb;
            #1;
            $display("TABLE: %x %x", in_bits[15:0], out_bits);
         end
      end
      $finish;
   end // initial begin
   
endmodule // test
