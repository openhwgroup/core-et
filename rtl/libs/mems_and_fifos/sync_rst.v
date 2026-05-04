// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// sync_rst
// 
//    reset output: asynchronous assertion (high), synchronous deassertion (low)
//    
//    active high reset in, active high sync reset out

`include "libs_defines.vh"
`include "soctop_defines.vh" // required for SYNC_STAGES

module sync_rst (
   output logic reset_out,            // active high sync reset out
   input  logic reset_in,             // active high reset in
   input  logic clock,
   input  logic dft__reset_byp,
   input  logic dft__reset
);

`ifdef ET_LIBS_USE_STD_CELLS
   sync_rst_grp_cells ET_GROUP_CELLS_sync_rst_grp_cells (.reset_out(reset_out), .reset_in(reset_in), .clock(clock), .dft__reset_byp(dft__reset_byp), .dft__reset(dft__reset));
`else

   // per Snps data for all PD >= 585mV use SYNC_STAGES=2
   localparam SYNC_STAGES = 2;

   // simple behavioral model
   logic [SYNC_STAGES-1:0] sync_rst_meta_flops;
   
   // mux select int_reset_in_n
   // invert reset so cell can use active low async reset   
   wire int_reset_in_n = dft__reset_byp ? !dft__reset : !reset_in;

   // async reset high, sync deassert low
   always @(posedge clock or negedge int_reset_in_n) begin
      if (!int_reset_in_n) begin
        sync_rst_meta_flops <= {SYNC_STAGES{1'b1}};
      end
      else begin
        sync_rst_meta_flops <= {sync_rst_meta_flops[SYNC_STAGES-2:0], 1'b0};
      end
   end
   
   // dft mux bypass of final reset_out
   assign reset_out = dft__reset_byp ? dft__reset : sync_rst_meta_flops[SYNC_STAGES-1];

`endif

   
endmodule


`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module sync_rst_grp_cells (
   output logic reset_out,            // active high sync reset out
   input  logic reset_in,             // active high reset in
   input  logic clock,
   input  logic dft__reset_byp,
   input  logic dft__reset
);

   generate
      if (1) begin : ET_DONT_TOUCH_sync_rst_wrap
         // mux select int_reset_in_n
         // invert reset so cell can use active low async reset   
         logic reset_in_n;
         logic dft__reset_n;
         HDBULT11_INV_1 reset_inv      (.X(reset_in_n),   .A(reset_in));
         HDBULT11_INV_1 dft__reset_inv (.X(dft__reset_n), .A(dft__reset));
         logic int_reset_in_n;
         HDBULT11_MUX2_MM_2 int_reset_in_n_mux (.X(int_reset_in_n), .D0(reset_in_n), .D1(dft__reset_n), .S(dft__reset_byp));
         
         // async reset high, sync deassert low
         logic reset_in_d2;
         HDBULT08_SYNC2SBMSFQ_Y4_2 sync_rst_meta_flops (.Q(reset_in_d2), .CK(clock), .D(1'b0), .SD(int_reset_in_n), .SI (1'b0), .SE (1'b0));
         
         // dft mux bypass of final reset_out
         wire int_reset_out = reset_in_d2;
         HDBULT11_MUX2_MM_2 reset_out_mux (.X(reset_out), .D0(int_reset_out), .D1(dft__reset), .S(dft__reset_byp));
      end
   endgenerate

endmodule
`endif  // ET_LIBS_USE_STD_CELLS


