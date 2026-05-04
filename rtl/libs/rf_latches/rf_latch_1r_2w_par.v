// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Latch based Register Files with parallel output
//                 and 2 write ports
// Project       : ??
//-----------------------------------------------------------------------------
// File          : rf_latch_1r_2w_par.v
// Author        : Ildefonso Gomariz
// Created       : 17.01.2019
// Last modified : 17.01.2019
//-----------------------------------------------------------------------------
// Description :
//
// Modification history :
// 17.01.2019 : created
//-----------------------------------------------------------------------------

// units of time and time resolution for this run
//`timescale 1ps / 1ps

module rf_latch_1r_2w_par
#(
   parameter WIDTH   = 'd32,
   parameter ENTRIES = 'd8,
   parameter SIZE    = $clog2(ENTRIES),
   parameter LEVEL2_CLK_GATE = 0
)
(
   // System signals
   input                          clk,
   input                          test_en,
   // Read port
   output reg [WIDTH*ENTRIES-1:0] rd_data_a,
   // Write ports
   input                          wr_en_a,
   input [SIZE-1:0]               wr_addr_a,
   input                          wr_data_a_en_1p,
   input [WIDTH-1:0]              wr_data_a,
   input                          wr_en_b,
   input [SIZE-1:0]               wr_addr_b,
   input                          wr_data_b_en_1p,
   input [WIDTH-1:0]              wr_data_b
);

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

`ifdef ENABLE_VERILATOR_MODEL
 
   reg [WIDTH-1:0]    rf[ENTRIES-1:0];
   
   logic [WIDTH-1:0]  wr_data_a_del;
   logic [WIDTH-1:0]  wr_data_b_del;
   always_ff@(negedge clk) begin
     if (wr_data_a_en_1p) wr_data_a_del <= wr_data_a;
     if (wr_data_b_en_1p) wr_data_b_del <= wr_data_b;
   end
   
   always_ff@(posedge clk) begin
      if (wr_en_b)
        rf[wr_addr_b] <= wr_data_b_del;
      if (wr_en_a)
        rf[wr_addr_a] <= wr_data_a_del;
   end

`else

   //--------------------------------------------------------------------------
   // Write Data Clock Gaters & Latches
   //--------------------------------------------------------------------------
   reg [WIDTH-1:0]     wr_data_a_1p;    // Ph2 latched write data A
   reg [WIDTH-1:0]     wr_data_b_1p;    // Ph2 latched write data B

   wire               clk_wr_data_a_n; // Write data A Ph2 clock
   wire               clk_wr_data_b_n; // Write data B Ph2 clock

   et_clk_gate_n cbuf_wr_data_a(.enclk_n(clk_wr_data_a_n), .en_1p(wr_data_a_en_1p), .clk(clk), .te(test_en));
   et_clk_gate_n cbuf_wr_data_b(.enclk_n(clk_wr_data_b_n), .en_1p(wr_data_b_en_1p), .clk(clk), .te(test_en));

   always @(clk_wr_data_a_n or wr_data_a)
     if (!clk_wr_data_a_n)
       wr_data_a_1p <= wr_data_a;

   
   always @(clk_wr_data_b_n or wr_data_b)
     if (!clk_wr_data_b_n)
       wr_data_b_1p <= wr_data_b;

   //--------------------------------------------------------------------------
   // Register File
   //--------------------------------------------------------------------------
   reg [WIDTH-1:0]    rf[ENTRIES-1:0]; // Latch Array (RAM)


   // Write enables per ENTRIES
   wire [ENTRIES-1:0]   dec_wr_en_a; // decoded write enables for port A
   wire [ENTRIES-1:0]   dec_wr_en_b; // decoded write enables for port B
   wire [ENTRIES-1:0]   dec_wr_en;   // write from either port A or port B
   wire [ENTRIES-1:0]   clk_rf_wr;

   // decode write enables
   assign dec_wr_en_a = ({{ENTRIES-1{1'b0}},1'b1} << wr_addr_a) & {ENTRIES{wr_en_a || test_en}};
   assign dec_wr_en_b = ({{ENTRIES-1{1'b0}},1'b1} << wr_addr_b) & {ENTRIES{wr_en_b || test_en}};
   assign dec_wr_en = dec_wr_en_a | dec_wr_en_b;

   // second-level clock gate
   wire                 clk_wr;
   generate
     if (LEVEL2_CLK_GATE != 0) begin : clk_wr_gen
        et_clk_gate cbuf_wr(.enclk(clk_wr), .en(wr_en_a | wr_en_b), .clk(clk), .te(1'b0));
     end else begin
        assign clk_wr = clk;
     end
   endgenerate

   genvar             i;

   generate                     // the gated clocks for the write
      for (i = 0; i < ENTRIES; i = i + 1) begin : rf_c
         // disable test_en to avoid current spike
         et_clk_gate cbuf_rf_wr(.enclk(clk_rf_wr[i]), .en(dec_wr_en[i]), .clk(clk_wr), .te(1'b0));
      end
   endgenerate

   // latched version of write enable to pick between two write ports
   reg [ENTRIES-1:0]   dec_wr_en_a_1p; // Ph2 latched write enable
   always @(clk_wr or dec_wr_en_a)
     if (!clk_wr)
       dec_wr_en_a_1p <= dec_wr_en_a;

   // Pick between two write ports
   reg [WIDTH-1:0] wr_data_1p [ENTRIES-1:0];
   always_comb
      for (integer idx = 0; idx < ENTRIES; idx=idx+1)
         wr_data_1p[idx] = dec_wr_en_a_1p[idx] ? wr_data_a_1p : wr_data_b_1p;

   generate
      for (i = 0; i < ENTRIES; i=i+1) begin  : rf_

         always @(clk_rf_wr[i] or wr_data_1p[i])
           if (clk_rf_wr[i])
             rf[i] <= wr_data_1p[i];
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
