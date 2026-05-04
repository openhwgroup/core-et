// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Latch based register with parallel output and single entry
// Project       : ??
//-----------------------------------------------------------------------------
// File          : rf_latch_single_1r_1w_par.v
// Author        : 
// Created       : 8.08.2018
// Last modified : 8.08.2018
//-----------------------------------------------------------------------------
// Description :
//
// Modification history :
// 8.08.2018 : created
//-----------------------------------------------------------------------------

// units of time and time resolution for this run
//`timescale 1ps / 1ps

module rf_latch_single_1r_1w_par (/*AUTOARG*/
   // Outputs
   rd_data_a,
   // Inputs
   wr_data_a, 
   wr_data_a_en_1p, 
   wr_en_a,
   clk, 
   test_en
   ) ;

   /*
                             _______         _______         _______         _______
                         ___/       \_______/       \_______/       \_______/       \_______

    wr_en                                   -     val_1     -     val_2     -
    wr_data                                 -     val_1     -     val_2     -
    wr_data_en_1p                  -    val_1       -     val_2    -

    Notes on interface:
    1) wr_data_en_1p precedes the write control signals (data,en) by 1 phase
       This is to allow sufficient time to gate the write data clock. Set this to
       1'b1, if it is impractical to generate this.

    2) No read interface, all register file is delivered in parallel.

    */

   parameter WIDTH = 'd32;

   output reg [WIDTH-1:0] rd_data_a; // Output Port A

   input [WIDTH-1:0]  wr_data_a; // Write Data - port A
   input              wr_data_a_en_1p; // Write Data Enable - port A (from Ph2 latch)
   input              wr_en_a;    // Write Enable port A

   input              clk;
   input              test_en;

`ifdef ENABLE_VERILATOR_MODEL
 
   reg [WIDTH-1:0]    rf;
   
   logic [WIDTH-1:0]  wr_data_a_del;
   always_ff@(negedge clk) 
     if (wr_data_a_en_1p) wr_data_a_del <= wr_data_a;
   
   always_ff@(posedge clk) begin
      if (wr_en_a)
        rf <= wr_data_a_del;
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
   reg [WIDTH-1:0]    rf; // Latch Array (RAM)


   // Write enables
   wire    dec_wr_en;
   wire    clk_rf_wr;

   // decode write enables
   assign dec_wr_en = wr_en_a || test_en;


   // disable test_en to avoid current spike
   et_clk_gate cbuf_rf_wr(.enclk(clk_rf_wr), .en(dec_wr_en), .clk(clk), .te(1'b0));

   always @(clk_rf_wr or wr_data_a_1p)
     if (clk_rf_wr)
       rf <= wr_data_a_1p;
`endif
   //--------------------------------------------------------------------------
   // Output Data Ports (parallel output)
   //--------------------------------------------------------------------------
   always_comb
   begin
      rd_data_a = rf;
   end

endmodule

`include "timescale.vh"
