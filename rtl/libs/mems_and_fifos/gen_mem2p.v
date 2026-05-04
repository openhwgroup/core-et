// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// generic memory 2 port memory

module gen_mem2p
( clock,
  addressA, enA, weA, dinA, doutA,
  addressB, enB, weB, dinB, doutB);

   // size parameters
   parameter WIDTH = 32;
   parameter WORDS = 256;
   parameter WRITE_THROUGH = 1'b0;
`ifndef CDC_ANALYSIS
//synopsys translate_off
`endif
   parameter RAM_NAME = "UNKNOWN";
   parameter ECC_BITS = {WIDTH{1'b1}};
   parameter ECC_SECTIONS = 1;
   parameter ECC_PROTECTED = 0;
`ifndef CDC_ANALYSIS
//synopsys translate_on
`endif
   localparam AD_BITS = $clog2(WORDS);

   // ports
   input clock;

   input [AD_BITS-1:0]     addressA;
   input                   enA;
   input                   weA;
   input [WIDTH-1:0]       dinA;
   output wire [WIDTH-1:0] doutA;

   input [AD_BITS-1:0]     addressB;
   input                   enB;
   input                   weB;
   input [WIDTH-1:0]       dinB;
   output wire [WIDTH-1:0] doutB;

   // implementation
   reg [WIDTH-1:0]         mem [WORDS-1:0];

   reg [WIDTH-1:0]   doutA_r;
   reg [WIDTH-1:0]   doutB_r;

   wire [WIDTH-1:0] ecc_errA;
   wire [WIDTH-1:0] ecc_errB;

   generate if (!WRITE_THROUGH) begin : nwt
     always @(posedge clock) begin
        if (enA) begin
           if(weA) mem[addressA] <= dinA;
           else doutA_r <= mem[addressA];
        end

        if (enB)  begin
           if(weB) mem[addressB] <= dinB;
           else doutB_r <= mem[addressB];
        end

// synopsys translate_off
        if (enB && weB && enA && weA && addressA==addressB) begin
           $warning("%m: both memory ports writing at the same address");
           mem[addressA] <= { (WIDTH) {1'bx} };
        end
// synopsys translate_on

// synopsys translate_off
        if (enB && enA && weA!=weB && addressA==addressB) begin
           // $warning("%m: reading and writing to the same address and no write through");
           // mem[addressA] <= { (WIDTH) {1'bx} };
           if (!weA) doutA_r <= 'x;
           if (!weB) doutB_r <= 'x;
        end
// synopsys translate_on
     end // always @
   end
   else begin : wt
      always @(posedge clock) begin
        if (enA) begin
           if(weA) mem[addressA] <= dinA;
           else doutA_r <= (enB && weB && (addressB==addressA)) ? dinB : mem[addressA];
        end

        if (enB)  begin
           if(weB) mem[addressB] <= dinB;
           else doutB_r <= (enA && weA && (addressA==addressB)) ? dinA : mem[addressB];
        end
      end // always @
   end // else: !if(WRITE_THROUGH)
   endgenerate

   assign doutA = doutA_r ^ ecc_errA;
   assign doutB = doutB_r ^ ecc_errB;

`ifdef EVL_SIMULATION
   genvar gen_it;
   generate
      if (ECC_PROTECTED == 1) begin : ge
         for (gen_it = 0; gen_it < ECC_SECTIONS; gen_it++) begin : ge
            ecc_err_gen #(
               .WIDTH(WIDTH/ECC_SECTIONS),
               .WORDS(WORDS),
               .RAM_NAME(RAM_NAME),
               .ECC_BITS(ECC_BITS)
            ) err_gen (
               .clock    (clock),
               .addressA (addressA),
               .enA      (enA),
               .weA      (weA),
               .errA     (ecc_errA[WIDTH/ECC_SECTIONS * gen_it +: WIDTH/ECC_SECTIONS]),
               .addressB (addressB),
               .enB      (enB),
               .weB      (weB),
               .errB     (ecc_errB[WIDTH/ECC_SECTIONS * gen_it +: WIDTH/ECC_SECTIONS])
            );
         end
      end
      else begin
         assign ecc_errA = '0;
         assign ecc_errB = '0;
      end
   endgenerate
`else
      assign ecc_errA = '0;
      assign ecc_errB = '0;
`endif


endmodule // gen_mem2p


