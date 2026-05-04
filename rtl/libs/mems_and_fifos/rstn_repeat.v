// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// rstn_repeat
// 
//    synchronous active low reset in, synchronous active low reset out.
//    This module has an "async assertion" using a synchronous reset signal
//    and adds a single cycle of delay to the reset release
//       NOTE: This module does not synchronize the input like sync_rstn
//             but is only used as a singe flop repeater with 
//             dft controllability

`include "libs_defines.vh"
`include "soctop_defines.vh" // required for SYNC_STAGES

module rstn_repeat (
   output logic reset_out_n,          // active low sync reset out
   input  logic reset_in_n,           // active low reset in
   input  logic clock,
   input  logic dft__reset_byp,
   input  logic dft__reset
) ;


`ifdef ET_LIBS_USE_STD_CELLS
   rstn_repeat_grp_cells ET_GROUP_CELLS_rstn_repeat_grp_cells (.reset_out_n (reset_out_n), .reset_in_n (reset_in_n), .clock (clock), .dft__reset_byp (dft__reset_byp), .dft__reset (dft__reset));
`else

   // simple behavioral model
   logic rst_repeat_flop_n;
   
   // mux select int_reset_in_n
   wire int_reset_in_n = dft__reset_byp ? !dft__reset : reset_in_n;

   // async reset low, sync deassert high   
   always @(posedge clock or negedge int_reset_in_n) begin
      if (!int_reset_in_n) begin
         rst_repeat_flop_n <= 1'b0;
      end
      else begin
         rst_repeat_flop_n <= 1'b1;
      end
   end
   
   // dft mux bypass of final reset_out_n
   assign reset_out_n = dft__reset_byp ? !dft__reset : rst_repeat_flop_n;

`endif

   
endmodule


`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module rstn_repeat_grp_cells (
   output logic reset_out_n,          // active low sync reset out
   input  logic reset_in_n,           // active low reset in
   input  logic clock,
   input  logic dft__reset_byp,
   input  logic dft__reset
) ;

   generate
      if (1) begin : ET_DONT_TOUCH_rstn_repeat_wrap
         // mux select int_reset_in_n
         logic dft__reset_n;
         HDBULT11_INV_1 dft__reset_inv (.X(dft__reset_n), .A(dft__reset));
         logic int_reset_in_n;
         HDBULT11_MUX2_MM_2 int_reset_in_n_mux (.X(int_reset_in_n), .D0(reset_in_n), .D1(dft__reset_n), .S(dft__reset_byp));
         
         
         // async reset low, sync deassert high   
         logic reset_in_n_d1;
         HDBULT08_FSDPRBQ_CBV2Y2_1 rstn_repeat_flop_n (.Q(reset_in_n_d1), .CK(clock), .D(1'b1), .RD(int_reset_in_n), .SI (1'b0), .SE (1'b0));
         
         // dft mux bypas of final reset_out_n
         wire int_reset_out_n = reset_in_n_d1;
         HDBULT11_MUX2_MM_2 reset_out_n_mux (.X(reset_out_n), .D0(int_reset_out_n), .D1(dft__reset_n), .S(dft__reset_byp));
      end
   endgenerate
endmodule
`endif  // ET_LIBS_USE_STD_CELLS

