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

module rf_latch_2r_1w
#(
    parameter WIDTH                  = 'd32,
    parameter ENTRIES                = 'd32,
    parameter [ENTRIES-1:0] ZERO     = 'b0,
    parameter SIZE                   = $clog2(ENTRIES),
    parameter [ENTRIES-1:0] PARALLEL  = 'b0,
    parameter PARALLEL_W             = 'd32,
    parameter LEVEL2_CLK_GATE        = 0
)
(
    // System signals
    input  logic             clk,
    input  logic             test_en,
    // Read port A
    input  logic [SIZE-1:0]  rd_addr_a,
    output logic [WIDTH-1:0] rd_data_a,
    // Read port B
    input  logic [SIZE-1:0]  rd_addr_b,
    output logic [WIDTH-1:0] rd_data_b,
    // parallel read
    output [PARALLEL_W-1:0]  rd_par,
    // Write port
    input  logic             wr_en_a,
    input  logic             wr_data_a_en_1p,
    input  logic [SIZE-1:0]  wr_addr_a,
    input  logic [WIDTH-1:0] wr_data_a
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
   genvar                    i;
   logic [WIDTH-1:0]           rf[ENTRIES-1:0];
   logic [WIDTH-1:0]           wr_data_a_del;
   always_ff@(negedge clk) 
     if (wr_data_a_en_1p) wr_data_a_del <= wr_data_a;

   
   // the gated clocks for the write
   generate
      for(i = 0; i < ENTRIES; i = i + 1) begin : rf_c
         if(ZERO[i] == 0) begin
           `EN_FF(clk, wr_en_a && wr_addr_a == i, rf[i], wr_data_a_del)
         end
         else assign rf[i] = '0;
      end
   endgenerate
   
`else
//--------------------------------------------------------------------------
// Write Data Clock Gaters & Latches
//--------------------------------------------------------------------------
logic [WIDTH-1:0] wr_data_a_1p;    // Ph2 latched write data A
logic             clk_wr_data_a_n; // Write data A Ph2 clock

et_clk_gate_n
cbuf_wr_data_a
(
    .enclk_n ( clk_wr_data_a_n ),
    .en_1p   ( wr_data_a_en_1p ),
    .clk     ( clk             ),
    .te      ( test_en         )
);

always @(clk_wr_data_a_n or wr_data_a)
begin
    if(!clk_wr_data_a_n)
        wr_data_a_1p <= wr_data_a;
end

//--------------------------------------------------------------------------
// Register File
//--------------------------------------------------------------------------
logic [ENTRIES-1:0][WIDTH-1:0] rf;        // Latch Array (RAM)
logic [ENTRIES-1:0]            dec_wr_en; // write from either port A or port B
logic [ENTRIES-1:0]            clk_rf_wr;

// decode write enables
assign dec_wr_en = ({{ENTRIES-1{1'b0}},1'b1} << wr_addr_a) & {ENTRIES{wr_en_a || test_en}};

genvar i;

// second-level clock gate
wire                 clk_wr;
generate
  if (LEVEL2_CLK_GATE != 0) begin : clk_wr_gen
     et_clk_gate cbuf_wr(.enclk(clk_wr), .en(wr_en_a), .clk(clk), .te(1'b0));
  end else begin
     assign clk_wr = clk;
  end
endgenerate

// the gated clocks for the write
generate
    for(i = 0; i < ENTRIES; i = i + 1)
    begin : rf_c
        if(ZERO[i] == 0)
        begin : rf_c_zero
            // disable test_en to avoid current spike
            et_clk_gate
            cbuf_rf_wr
            (
                .enclk ( clk_rf_wr[i] ),
                .en    ( dec_wr_en[i] ),
                .clk   ( clk_wr       ),
                .te    ( 1'b0         )
            );
        end
    end
endgenerate

generate
    // Generates the registers
     for(i = 0; i < ENTRIES; i = i + 1)
    begin : rf_
        if(ZERO[i])
            assign rf[i] = {WIDTH{1'b0}};
        else
	    always @(clk_rf_wr[i] or wr_data_a_1p)
            begin
                if(clk_rf_wr[i])
                    rf[i] <= wr_data_a_1p;
            end
    end
endgenerate
`endif // !`ifdef ENABLE_VERILATOR_MODEL
   
   //--------------------------------------------------------------------------
   // Read Data Ports
   //--------------------------------------------------------------------------
   
   always_comb
     begin
        rd_data_a = rf[rd_addr_a];
        rd_data_b = rf[rd_addr_b];
     end
   
   
   generate
      if ( |PARALLEL == 1'b0) begin: NO_PAR
         assign rd_par = '0;
      end
      else 
        for ( i = 0 ; i < ENTRIES; i++) begin: RD_PAR
           if (PARALLEL[i])
             assign rd_par[WIDTH*par_position(i)+:WIDTH] = rf[i];
        end
   endgenerate
   
   function automatic integer par_position;
      input integer pos;
      begin
         par_position = 0;
         for ( int j = 0; j < pos; j++)
           if (PARALLEL[j]) par_position++;
      end
   endfunction // par_position
   
endmodule

`include "timescale.vh"
