// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// dcache_data_array.v
// One-read, one-write register file
// Default: 128 words x 32 bits, synchronous write, synchronous read
// Verilog-2001 (no SystemVerilog constructs)

`timescale 1ns/1ps

module dcache_128x32_1r1w_lram #(
    parameter WIDTH     = 32,   // bits per word
    parameter DEPTH     = 128,  // number of words
    parameter ADDR_BITS = 7     // log2(DEPTH), 128 -> 7
)(
    input                  clk,        // clock
    input                  wr_enable,  // write enable (active-high)
    input                  rd_enable,  // read enable (active-high)
    input  [ADDR_BITS-1:0] wr_addr,    // write address
    input  [WIDTH-1:0]     wr_data,    // write data
    input  [ADDR_BITS-1:0] rd_addr,    // read address
    output reg [WIDTH-1:0] rd_data     // read data (synchronous)
);

    // Storage array
    reg [WIDTH-1:0] mem [0:DEPTH-1];

    // Temporary combinational value for match-or-x result
    reg [WIDTH-1:0] y;

    // Function: for each bit i, if a[i] === b[i], return that bit; else 'x'
    function [WIDTH-1:0] match_or_x_fn;
        input [WIDTH-1:0] a;
        input [WIDTH-1:0] b;
        integer i;
        begin
            for (i = 0; i < WIDTH; i = i + 1) begin
                match_or_x_fn[i] = (a[i] === b[i]) ? a[i] : 1'bx;
            end
        end
    endfunction

    // Synchronous read/write
    always @(posedge clk) begin
        // Simultaneous read & write to the same address
        if (wr_enable && rd_enable && (wr_addr == rd_addr)) begin
            if (wr_data !== mem[rd_addr]) begin
                y = match_or_x_fn(wr_data, mem[rd_addr]); // temporary combinational
                rd_data      <= y;        // return same vector on read
            end else begin
                rd_data      <= wr_data;  // read returns the (same) value
            end
			mem[wr_addr] <= wr_data; // In case of decoupled read port the write should always be executed. 
        end else begin
            // Normal independent write/read operations
            if (wr_enable) begin
                mem[wr_addr] <= wr_data;
            end
            if (rd_enable) begin
                rd_data <= mem[rd_addr];
            end
        end
    end

endmodule

