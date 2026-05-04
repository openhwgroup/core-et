// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

module vcfifo_mem #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   FF_MEM_IMPL = 1,
   // below considered localparam do not override
   NUM_ELEMS_LOG = $clog2(NUM_ELEMS)
) (
   input  logic [NUM_ELEMS_LOG-1:0] rd_addr,
   output logic [ELEM_SIZE-1:0]     rd_data,

   input  logic                     wr_en,
   input  logic [NUM_ELEMS_LOG-1:0] wr_addr,
   input  logic [ELEM_SIZE-1:0]     wr_data,
   input  logic                     clk_wr
);
   

   logic [NUM_ELEMS-1:0][ELEM_SIZE-1:0] rf;        // FF or Latch Array of fifo (RAM)
   logic [NUM_ELEMS-1:0]                dec_wr_en; // decode write enable
   logic [NUM_ELEMS-1:0]                clk_rf_wr;
   genvar                               ii;

   assign dec_wr_en = ({{NUM_ELEMS-1{1'b0}},1'b1} << wr_addr) & {NUM_ELEMS{wr_en}};

   generate
      // the gated clocks for the write
      for(ii = 0; ii < NUM_ELEMS; ii = ii + 1) begin : g_cg
         et_clk_gate cbuf_rf_wr (
            .enclk ( clk_rf_wr[ii] ),
            .en    ( dec_wr_en[ii] ),
            .clk   ( clk_wr       ),
            .te    ( 1'b0         )
         );
      end
      if (FF_MEM_IMPL == 1) begin : ff_mem
         //--------------------------------------------------------------------------
         // Flip-flop Based Memory
         //--------------------------------------------------------------------------
         for(ii = 0; ii < NUM_ELEMS; ii = ii + 1) begin : g_rf
            `FF( clk_rf_wr[ii], rf[ii],  wr_data)
         end
         assign rd_data = rf[rd_addr] ;
      end
      else begin : latch_mem
         //--------------------------------------------------------------------------
         // Latch Based Memory
         //--------------------------------------------------------------------------
         // ri lint_check_off LATCH
         logic [ELEM_SIZE-1:0]  wr_data_1p;
         always @(clk_wr or wr_data) begin
           if (!clk_wr) begin
             wr_data_1p <= wr_data;   // ri lint_check_waive CLOCK_USE
           end
         end
         // ri lint_check_on LATCH
         // latch storage
         for(ii = 0; ii < NUM_ELEMS; ii = ii + 1) begin : g_rf
            always @(clk_rf_wr[ii] or wr_data_1p) begin
               if (clk_rf_wr[ii]) begin
                  rf[ii] <= wr_data_1p;
               end
            end
         end
         assign rd_data = rf[rd_addr] ;
      end
   endgenerate
   

endmodule
