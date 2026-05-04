// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// generic memory 1 port memory
module gen_mem1p
( clock, address, en, we, din, dout );

   // size parameters
   parameter WIDTH = 32;
   parameter WORDS = 256;
`ifndef CDC_ANALYSIS
//synopsys translate_off
`endif
   parameter ALLOW_RESET = 0;
   parameter RAM_NAME = "UNKNOWN";
   parameter ECC_BITS = {WIDTH{1'b1}};
   parameter ECC_SECTIONS = 1;
   parameter ECC_COUNT = 1;
   parameter ECC_PROTECTED = 0;
`ifndef CDC_ANALYSIS
//synopsys translate_on
`endif
   localparam AD_BITS = $clog2(WORDS);

   // ports
   input clock;
   input [AD_BITS-1:0] address;
   input               en;
   input               we;
   input [WIDTH-1:0]   din;
   output wire [WIDTH-1:0]  dout;

   // implementation
   reg [WIDTH-1:0] mem[WORDS-1:0];
   reg [WIDTH-1:0] dout_r;

   //synopsys translate_off
   reg [WIDTH-1:0] reset_mem[WORDS-1:0];
   reg             reset_flag;
   always_comb begin
      for (int i = 0 ; i < WORDS; i++)
        reset_mem[i] = '0;
   end
   //synopsys translate_on

   wire [WIDTH-1:0] ecc_err;

   always @(posedge clock) begin
      //synopsys translate_off
      if (reset_flag === 1'b1 && ALLOW_RESET) begin
         $display("backdoor resetting contents of %m");
         mem <= reset_mem;
         reset_flag <= 0;
      end
      //synopsys translate_on

      if (en) begin
         if(we) mem[address] <= din;
         else dout_r <= mem[address];
      end
   end // always @ (posedge clock)

   assign dout = dout_r ^ ecc_err;

   //synopsys translate_off
   task reset;
      begin
         reset_flag <= 1;
      end
   endtask // reset
   //synopsys translate_on

`ifdef EVL_SIMULATION
   logic [WIDTH-1:0] errB_unused;
   genvar gen_it;
   generate
      if (ECC_PROTECTED == 1) begin :ge
         for (gen_it = 0; gen_it < ECC_SECTIONS; gen_it++) begin : ge
            ecc_err_gen #(
               .WIDTH(WIDTH/ECC_SECTIONS),
               .WORDS(WORDS),
               .RAM_NAME(RAM_NAME),
               .ECC_COUNT(ECC_COUNT),
               .ECC_BITS(ECC_BITS)
            ) err_gen (
               .clock    (clock),
               .addressA (address),
               .enA      (en),
               .weA      (we),
               .errA     (ecc_err[WIDTH/ECC_SECTIONS * gen_it +: WIDTH/ECC_SECTIONS]),
               .addressB ('0),
               .enB      ('0),
               .weB      ('0),
               .errB     (errB_unused[WIDTH/ECC_SECTIONS * gen_it +: WIDTH/ECC_SECTIONS])
            );
         end
      end
      else begin
         assign ecc_err = '0;
      end
   endgenerate
`else
      assign ecc_err = '0;
`endif

endmodule // gen_mem1p
