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
//
// Modification history :
// 20.07.2017 : created
//-----------------------------------------------------------------------------

// units of time and time resolution for this run
//`timescale 1ps / 1ps

module rf_latch_7r_1w (/*AUTOARG*/
   // Outputs
   rd_data_a,
   rd_data_b,
   rd_data_c,
   rd_data_d,
   rd_data_e,
   rd_data_f,
   rd_data_g,
   // Inputs
   rd_addr_a,
   rd_addr_b,
   rd_addr_c,
   rd_addr_d,
   rd_addr_e,
   rd_addr_f,
   rd_addr_g,
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
    rd_data (out)                           -     data_1    -     data_2    -

    Notes on interface:
    1) wr_data_en_1p precedes the write control signals (addr, data,en) by 1 phase
       This is to allow sufficient time to gate the write data clock. Set this to
       1'b1, if it is impractical to generate this.

    2) The read address produces a combinatorial output in the the same clock
       cycle (read latency = 0).

    */

   parameter WIDTH = 'd32, ENTRIES = 'd32;
   parameter LEVEL2_CLK_GATE = 0;
   localparam SIZE = $clog2(ENTRIES);

   output [WIDTH-1:0] rd_data_a; // Read Port A
   output [WIDTH-1:0] rd_data_b; // Read Port B
   output [WIDTH-1:0] rd_data_c; // Read Port C
   output [WIDTH-1:0] rd_data_d; // Read Port D
   output [WIDTH-1:0] rd_data_e; // Read Port E
   output [WIDTH-1:0] rd_data_f; // Read Port F
   output [WIDTH-1:0] rd_data_g; // Read Port G

   input [SIZE-1:0]   rd_addr_a; // Read Address for port A
   input [SIZE-1:0]   rd_addr_b; // Read Address for port B
   input [SIZE-1:0]   rd_addr_c; // Read Address for port C
   input [SIZE-1:0]   rd_addr_d; // Read Address for port D
   input [SIZE-1:0]   rd_addr_e; // Read Address for port E
   input [SIZE-1:0]   rd_addr_f; // Read Address for port F
   input [SIZE-1:0]   rd_addr_g; // Read Address for port G

   input [WIDTH-1:0]  wr_data_a; // Write Data - port A
   input              wr_data_a_en_1p; // Write Data Enable - port A (from Ph2 latch)
   input [SIZE-1:0]   wr_addr_a;  // Write Address - port A
   input              wr_en_a;    // Write Enable port A

   input              clk;
   input              test_en;
   
`ifdef ENABLE_VERILATOR_MODEL
   
   reg [WIDTH-1:0]           rf[ENTRIES-1:0];
   
   logic [WIDTH-1:0]         wr_data_a_del;
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
      for (i = 0; i < ENTRIES; i=i+1) begin  : rf_gen
         always @(clk_rf_wr[i] or wr_data_a_1p)
           if (clk_rf_wr[i])
             rf[i] <= wr_data_a_1p;
      end
   endgenerate
`endif
   //--------------------------------------------------------------------------
   // Read Data Ports
   //--------------------------------------------------------------------------
   assign rd_data_a = rf[rd_addr_a];
   assign rd_data_b = rf[rd_addr_b];
   assign rd_data_c = rf[rd_addr_c];
   assign rd_data_d = rf[rd_addr_d];
   assign rd_data_e = rf[rd_addr_e];
   assign rd_data_f = rf[rd_addr_f];
   assign rd_data_g = rf[rd_addr_g];


endmodule

`include "timescale.vh"
