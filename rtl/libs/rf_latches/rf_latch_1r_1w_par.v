// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Latch based Register Files with parallel output
// Project       : ??
//-----------------------------------------------------------------------------
// File          : rf_latch_1r_1w_par.v
// Author        : 
// Created       : 10.05.2018
// Last modified : 10.05.2018
//-----------------------------------------------------------------------------
// Description :
//
// Modification history :
// 10.05.2018 : created
//-----------------------------------------------------------------------------

// units of time and time resolution for this run
//`timescale 1ps / 1ps

module rf_latch_1r_1w_par (/*AUTOARG*/
   // Outputs
   rd_data_a,
   // Inputs
   wr_data_a, 
   wr_data_a_en_1p, 
   wr_addr_a, 
   wr_en_a,
   clk, 
   test_en
   ) ;

   /*
                             _______         _______         _______         _______
                         ___/       \_______/       \_______/       \_______/       \_______

    wr_addr                                 -     val_1     -     val_2     -
    wr_en                                   -     val_1     -     val_2     -
    wr_data                                 -     val_1     -     val_2     -
    wr_data_en_1p                  -    val_1       -     val_2    -

    Notes on interface:
    1) wr_data_en_1p precedes the write control signals (addr, data,en) by 1 phase
       This is to allow sufficient time to gate the write data clock. Set this to
       1'b1, if it is impractical to generate this.

    2) No read interface, all register file is delivered in parallel.

    */

   parameter WIDTH = 'd32, ENTRIES = 'd8;
   parameter LEVEL2_CLK_GATE = 0;
   localparam SIZE = $clog2(ENTRIES);

   output reg [WIDTH*ENTRIES-1:0] rd_data_a; // Output Port A

   input [WIDTH-1:0]  wr_data_a; // Write Data - port A
   input              wr_data_a_en_1p; // Write Data Enable - port A (from Ph2 latch)
   input [SIZE-1:0]   wr_addr_a;  // Write Address - port A
   input              wr_en_a;    // Write Enable port A

   input              clk;
   input              test_en;

`ifdef ENABLE_VERILATOR_MODEL
 
   reg [WIDTH-1:0]    rf[ENTRIES-1:0];
   
   logic [WIDTH-1:0]  wr_data_a_del;
   always_ff@(negedge clk) 
     if (wr_data_a_en_1p) wr_data_a_del <= wr_data_a;
   
   always_ff@(posedge clk) begin
      if (wr_en_a)
        rf[wr_addr_a] <= wr_data_a_del;
   end

`else

   //--------------------------------------------------------------------------
   // Write Data Clock Gaters & Latches
   //--------------------------------------------------------------------------
   reg [WIDTH-1:0]     wr_data_a_1p;    // Ph2 latched write data A

   wire               clk_wr_data_a_n; // Write data A Ph2 clock

   et_clk_gate_n cbuf_wr_data_a(.enclk_n(clk_wr_data_a_n), .en_1p(wr_data_a_en_1p), .clk(clk), .te(test_en));

   always @(clk_wr_data_a_n or wr_data_a)
     if (!clk_wr_data_a_n)
       wr_data_a_1p <= wr_data_a;

   //--------------------------------------------------------------------------
   // Register File
   //--------------------------------------------------------------------------
   reg [WIDTH-1:0]    rf[ENTRIES-1:0]; // Latch Array (RAM)


   // Write enables per ENTRIES
   wire [ENTRIES-1:0]   dec_wr_en;   // write from either port A or port B
   wire [ENTRIES-1:0]   clk_rf_wr;

   // second-level clock gate
   wire                 clk_wr;
   generate
     if (LEVEL2_CLK_GATE != 0) begin : clk_wr_gen
        et_clk_gate cbuf_wr(.enclk(clk_wr), .en(wr_en_a), .clk(clk), .te(1'b0));
     end else begin
        assign clk_wr = clk;
     end
   endgenerate

   // decode write enables
   assign dec_wr_en = ({{ENTRIES-1{1'b0}},1'b1} << wr_addr_a) & {ENTRIES{wr_en_a || test_en}};

   genvar             i;

   generate                     // the gated clocks for the write
      for (i = 0; i < ENTRIES; i = i + 1) begin : rf_c
         // disable test_en to avoid current spike
         et_clk_gate cbuf_rf_wr(.enclk(clk_rf_wr[i]), .en(dec_wr_en[i]), .clk(clk_wr), .te(1'b0));
      end
   endgenerate

   generate
      for (i = 0; i < ENTRIES; i=i+1) begin  : rf_
         always @(clk_rf_wr[i] or wr_data_a_1p)
           if (clk_rf_wr[i])
             rf[i] <= wr_data_a_1p;
      end
   endgenerate
`endif
   //--------------------------------------------------------------------------
   // Output Data Ports (parallel output)
   //--------------------------------------------------------------------------
   integer l;
   always_comb
   begin
     for (l=0; l<ENTRIES; l=l+1)
      rd_data_a[WIDTH*l+:WIDTH] = rf[l];
   end

endmodule

`include "timescale.vh"
