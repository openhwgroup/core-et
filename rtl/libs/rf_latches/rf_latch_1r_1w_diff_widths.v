// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Latch based Register Files
// Project       : ??
//-----------------------------------------------------------------------------
// File          : rf_latches.v
// Author        :   <jay@konstruisto.esperantotech.com>
// Created       : 20.07.2017
// Last modified : 20.07.2017
//-----------------------------------------------------------------------------
// Description : 
//  same as rf_latch_1r_1w.v, but with different read and write data widths
// ENTRIES refers to 'write' width entries
// Modification history :
// 20.07.2017 : created
//-----------------------------------------------------------------------------



// units of time and time resolution for this run
//`timescale 1ps / 1ps

module rf_latch_1r_1w_diff_widths (/*AUTOARG*/
   // Outputs
   rd_data_a,
   // Inputs
   rd_addr_a,
   wr_data_a, wr_data_a_en_1p, wr_addr_a, wr_en_a,
   clk, test_en
   ) ;

   /*
                             _______         _______         _______         _______
                         ___/       \_______/       \_______/       \_______/       \_______

    wr_addr                                 -     val_1     -     val_2     -
    wr_en                                   -     val_1     -     val_2     -
    wr_data                                 -     val_1     -     val_2     -
    wr_data_en_1p                  -    val_1       -     val_2    -

    rd_addr                 -     val_1     -     val_2     -
    rd_en                   -     val_1     -     val_2     -
    rd_data (out)                 data_1    -     data_2    -

    Notes on interface:
    1) wr_data_en_1p precedes the write control signals (addr, data,en) by 1 phase
       This is to allow sufficient time to gate the write data clock. Set this to
       1'b1, if it is impractical to generate this.

   
    */

   parameter R_WIDTH = 'd32, W_WIDTH = 'd32, ENTRIES = 'd32, R_ALIGNMENT = 'd32;
   parameter LEVEL2_CLK_GATE = 0;
   localparam R_ENTRIES = (ENTRIES * W_WIDTH)/R_WIDTH * (R_WIDTH / R_ALIGNMENT);
   localparam R_SIZE = $clog2(R_ENTRIES);
   localparam W_SIZE = $clog2(ENTRIES);
   localparam R2W_RATIO = W_WIDTH / R_WIDTH;


   output [R_WIDTH-1:0]         rd_data_a; // Read Port A
   
   input [R_SIZE-1:0]           rd_addr_a; // Read Address for port A
   
   input [W_WIDTH-1:0]          wr_data_a; // Write Data - port A
   input [R2W_RATIO-1:0]        wr_data_a_en_1p; // Write Data Enable - port A (from Ph2 latch)
   input [W_SIZE-1:0]           wr_addr_a;  // Write Address - port A
   input [R2W_RATIO-1:0]        wr_en_a;    // Write Enable port A
   
   input                        clk;
   input                        test_en;
   
   genvar                       i, ii;
   
`ifdef ENABLE_VERILATOR_MODEL
   reg [W_WIDTH-1:0]    rf[ENTRIES-1:0];
   
   logic [W_WIDTH-1:0]  wr_data_a_del;
   always_ff@(negedge clk)
     for ( int j = 0 ; j < R2W_RATIO; j++)
       if (wr_data_a_en_1p[j])
         wr_data_a_del[j*R_WIDTH+:R_WIDTH] <= wr_data_a[j*R_WIDTH+:R_WIDTH];
   
   always_ff@(posedge clk) begin
      for ( int j = 0 ; j < R2W_RATIO; j++)
      if (wr_en_a[j])
        rf[wr_addr_a][j*R_WIDTH+:R_WIDTH] <= wr_data_a_del[j*R_WIDTH+:R_WIDTH];
   end

   
`else

   //--------------------------------------------------------------------------
   // Write Data Clock Gaters & Latches
   //--------------------------------------------------------------------------
   reg [W_WIDTH-1:0]    wr_data_a_1p;    // Ph2 latched write data A

   generate for ( i = 0 ; i < R2W_RATIO; i++) begin: MASTER_LATCH
      wire               clk_wr_data_a_n; // Write data A Ph2 clock
      et_clk_gate_n cbuf_wr_data_a(.enclk_n(clk_wr_data_a_n), .en_1p(wr_data_a_en_1p[i]), .clk(clk), .te(test_en));
      always @(clk_wr_data_a_n or wr_data_a[i*R_WIDTH+:R_WIDTH])
        if (!clk_wr_data_a_n)
          wr_data_a_1p[i*R_WIDTH+:R_WIDTH] <= wr_data_a[i*R_WIDTH+:R_WIDTH];
   end
   endgenerate
   
   //--------------------------------------------------------------------------
   // Register File
   //--------------------------------------------------------------------------
   reg [W_WIDTH-1:0]  rf[ENTRIES-1:0]; // Latch Array (RAM)


   // Write enables per ENTRIES

   wire [ENTRIES-1:0][R2W_RATIO-1:0]   clk_rf_wr;
   
   // second-level clock gate (not fully optimized)
   wire [R2W_RATIO-1:0]                clk_wr;
   generate
      if (LEVEL2_CLK_GATE != 0) begin : clk_wr_gen
         for ( i = 0; i < R2W_RATIO; i++) begin: WR_CG
           et_clk_gate cbuf_wr(.enclk(clk_wr[i]), .en(wr_en_a[i]), .clk(clk), .te(1'b0));
         end
     end else begin
        assign clk_wr = { (R2W_RATIO) {clk}};
     end
   endgenerate


   generate  
      for (i = 0; i < ENTRIES; i = i + 1) begin : rf_c
         // decode write enables
         logic dec_wr_en;
         assign dec_wr_en = (wr_addr_a == i);
         
         // the gated clocks for the write
         // disable test_en to avoid current spike
         for (ii = 0 ; ii < R2W_RATIO;  ii++) begin: WR_CK
            et_clk_gate cbuf_rf_wr(.enclk(clk_rf_wr[i][ii]), .en(dec_wr_en), .clk(clk_wr[ii]), .te(1'b0));
         
            // and write
            logic [R_WIDTH-1:0] d;
            assign d = wr_data_a_1p[ii*R_WIDTH+:R_WIDTH];
            always @(clk_rf_wr[i][ii] or d) begin
              if (clk_rf_wr[i][ii])
                rf[i][ii*R_WIDTH+:R_WIDTH] <= d;
            end
         end
      end // block: rf_c
   endgenerate
   
   
`endif
   //--------------------------------------------------------------------------
   // Read Data Ports
   //--------------------------------------------------------------------------

   // create rf_read array... just assignments, but easier to address
   reg [W_WIDTH*ENTRIES-1:0]  rf_full;
   reg [W_WIDTH*ENTRIES+R_ALIGNMENT-1:0]  rf_full_ext;
   reg [R_WIDTH-1:0]          rf_read [R_ENTRIES-1:0];
   always_comb begin
      for ( int j = 0; j < ENTRIES; j++) rf_full[j*W_WIDTH+:W_WIDTH] = rf[j];
      rf_full_ext = {rf_full[R_ALIGNMENT-1:0], rf_full}; // extra msb ( last entry might need to wrap around)
      
      for ( int j = 0 ; j < R_ENTRIES; j++) begin
         rf_read[j] = rf_full_ext[j*R_ALIGNMENT+:R_WIDTH];
      end
   end
   
   // and assign output
   assign rd_data_a = rf_read[rd_addr_a];
   


endmodule // rf_latch_1r_1w_diff_widths


`include "timescale.vh"
