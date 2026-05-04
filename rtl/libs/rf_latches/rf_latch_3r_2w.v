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

module rf_latch_3r_2w
#(
   parameter WIDTH   = 'd32,
   parameter ENTRIES = 'd32,
   parameter SIZE    = $clog2(ENTRIES),
   parameter LEVEL2_CLK_GATE = 0
)
(
    // System signals
    input  logic             clk,
    input  logic             test_en,
    // Read ports
    input  logic [SIZE-1:0]  rd_addr_a,
    output logic [WIDTH-1:0] rd_data_a,
    input  logic [SIZE-1:0]  rd_addr_b,
    output logic [WIDTH-1:0] rd_data_b,
    input  logic [SIZE-1:0]  rd_addr_c,
    output logic [WIDTH-1:0] rd_data_c,
    // Write ports
    input  logic             wr_en_a,
    input  logic [SIZE-1:0]  wr_addr_a,
    input  logic             wr_data_a_en_1p,
    input  logic [WIDTH-1:0] wr_data_a,
    input  logic             wr_en_b,
    input  logic [SIZE-1:0]  wr_addr_b,
    input  logic             wr_data_b_en_1p,
    input  logic [WIDTH-1:0] wr_data_b
);

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

    2) The read address is flopped before producing a combinatorial output in the 
       following cycle (read latency = 1).

    */
`ifdef ENABLE_VERILATOR_MODEL
   
   reg [WIDTH-1:0]           vpu_rf[ENTRIES-1:0];
 
   logic [WIDTH-1:0]         wr_data_a_del;
   logic [WIDTH-1:0]         wr_data_b_del;
   always_ff@(negedge clk) begin
      if(wr_data_a_en_1p) wr_data_a_del <= wr_data_a;
      if(wr_data_b_en_1p) wr_data_b_del <= wr_data_b;
   end
   
   always_ff@(posedge clk) begin
      if (wr_en_b)
        vpu_rf[wr_addr_b] <= wr_data_b_del;
      if (wr_en_a)
        vpu_rf[wr_addr_a] <= wr_data_a_del;
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
   reg [WIDTH-1:0]    vpu_rf[ENTRIES-1:0]; // Latch Array (RAM)


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


   // latched  version of write enable to  pick between two write ports
   reg [ENTRIES-1:0]   dec_wr_en_a_1p; // Ph2 latched write enable
   always @(clk_wr or dec_wr_en_a)
     if (!clk_wr)
       dec_wr_en_a_1p <= dec_wr_en_a;

   generate
      for (i = 0; i < ENTRIES; i=i+1) begin  : rf_
         always @(clk_rf_wr[i] or wr_data_a_1p or wr_data_b_1p  or dec_wr_en_a_1p[i])
           if (clk_rf_wr[i])
             vpu_rf[i] <= dec_wr_en_a_1p[i] ? wr_data_a_1p : wr_data_b_1p;
      end
   endgenerate

`endif // !`ifdef ENABLE_VERILATOR_MODEL
   //--------------------------------------------------------------------------
   // Read Data Ports
   //--------------------------------------------------------------------------
   assign rd_data_a = vpu_rf[rd_addr_a];
   assign rd_data_b = vpu_rf[rd_addr_b];
   assign rd_data_c = vpu_rf[rd_addr_c];


   

   
endmodule // rf_3r2w_l

`include "timescale.vh"
