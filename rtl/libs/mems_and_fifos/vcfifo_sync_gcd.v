// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_sync_gcd

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_sync_gcd #(parameter
   NUM_ELEMS = 8,
   RD_PTR_SYNC_STAGES = 2,
   WR_PTR_SYNC_STAGES = 2,
   WR_LOV = 0,
   FIFO_GCD = 0, // only set to 1 within fifo_gcd instantiation of this module which prevent the inversion of either of the clocks to the sync_rst designs
   // below considered localparam do not override
   CNT_W = $clog2(NUM_ELEMS+1)
) (
   input  logic [CNT_W-1:0]      wr_cnt_gcd,
   output logic [CNT_W-1:0]      wr_cnt_gcd_sync,

   input  logic [CNT_W-1:0]      rd_cnt_gcd,
   output logic [CNT_W-1:0]      rd_cnt_gcd_sync,

   input  logic                  clock_push,
   input  logic                  clock_pop,

   input  logic                  reset_push,
   output logic                  reset_push_sync,

   input  logic                  reset_pop,
   output logic                  reset_pop_sync,

   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push,
   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);


   //----------------------------------------------------------------------------------
   // optional clock inversion
   //----------------------------------------------------------------------------------
   logic int_clock_push;
   logic int_clock_pop;
   generate
      if (FIFO_GCD == 1) begin : fifo_gcd
         assign int_clock_push =  clock_push;
         assign int_clock_pop  =  clock_pop;
      end
      else begin : non_fifo_gcd
         if (WR_LOV == 1) begin :wr_lov
            assign int_clock_push = !clock_push;
            assign int_clock_pop  =  clock_pop;
         end
         else begin : wr_hiv
            assign int_clock_push =  clock_push;
            assign int_clock_pop  = !clock_pop;
         end
      end
   endgenerate


   //----------------------------------------------------------------------------------
   // reset syncs
   //----------------------------------------------------------------------------------
   wire any_reset = reset_push || reset_pop;
   sync_rst reset_push_sync_inst (
      .reset_out      ( reset_push_sync     ), 
      .reset_in       ( any_reset           ),
      .clock          ( int_clock_push      ),
      .dft__reset_byp ( dft__reset_byp_push ),
      .dft__reset     ( dft__reset_push     )
   );

   sync_rst reset_pop_sync_inst (
      .reset_out      ( reset_pop_sync      ), 
      .reset_in       ( any_reset           ),
      .clock          ( int_clock_pop       ),
      .dft__reset_byp ( dft__reset_byp_pop  ),
      .dft__reset     ( dft__reset_pop      )

   );

   //----------------------------------------------------------------------------------
   // gcd syncs
   //----------------------------------------------------------------------------------
   et_cdc_sync #(
      .WIDTH(CNT_W), 
      .SYNC_STAGES(RD_PTR_SYNC_STAGES)
   ) rd_cnt_sync (
      .clock ( int_clock_push    ),
      .reset ( reset_push_sync   ),
      .din   ( rd_cnt_gcd        ),
      .dout  ( rd_cnt_gcd_sync   )
   );

   et_cdc_sync #(
      .WIDTH(CNT_W), 
      .SYNC_STAGES(WR_PTR_SYNC_STAGES)
   ) wr_cnt_sync (
      .clock   ( int_clock_pop   ),
      .reset   ( reset_pop_sync  ),
      .din     ( wr_cnt_gcd      ),
      .dout    ( wr_cnt_gcd_sync )
   );

endmodule

