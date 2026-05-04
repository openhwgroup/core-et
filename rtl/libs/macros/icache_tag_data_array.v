// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// icache_tag_array.v
// Two-read, one-write register file
// Default: 128 words x 16 bits, synchronous write, synchronous read
// Verilog-2001 (no SystemVerilog constructs)

`timescale 1ns/1ps

module icache_tag_data_array #(
    parameter WIDTH     = 27,   // bits per word
    parameter DEPTH     = 128,  // number of words
    parameter ADDR_BITS = 7     // log2(DEPTH), 128 -> 7
)(
    input                  clk,          // clock
    input                  wr_enable,    // write enable (active-high)
    input                  rd_enable_a,  // read enable port A (active-high)
    input                  rd_enable_b,  // read enable port B (active-high)
    input  [ADDR_BITS-1:0] wr_addr,      // write address
    input  [WIDTH-1:0]     wr_data,      // write data
    input  [ADDR_BITS-1:0] rd_addr_a,    // read address port A
    input  [ADDR_BITS-1:0] rd_addr_b,    // read address port B
    output reg [WIDTH-1:0] rd_data_a,    // read data port A (registered)
    output reg [WIDTH-1:0] rd_data_b     // read data port B (registered)
);

    // Storage array
    reg [WIDTH-1:0] mem [0:DEPTH-1];

    // Function: for each bit i, if a[i] === b[i], return that bit; else 'x'
    function automatic [WIDTH-1:0] match_or_x_fn;
        input [WIDTH-1:0] a;
        input [WIDTH-1:0] b;
        integer i;
        begin
            for (i = 0; i < WIDTH; i = i + 1) begin
                match_or_x_fn[i] = (a[i] === b[i]) ? a[i] : 1'bx;
            end
        end
    endfunction

    // Hazard wires: read and write to the same address this cycle
    wire hazard_a = wr_enable && rd_enable_a && (wr_addr == rd_addr_a);
    wire hazard_b = wr_enable && rd_enable_b && (wr_addr == rd_addr_b);

    // Synchronous read/write
    always @(posedge clk) begin
        // ----------------
        // READS (registered)
        // ----------------
        if (rd_enable_a) begin
            if (hazard_a) begin
                // Compare against OLD data (mem[rd_addr_a]) at this edge
                if (wr_data !== mem[rd_addr_a]) begin
                    rd_data_a <= match_or_x_fn(wr_data, mem[rd_addr_a]);
                end else begin
                    rd_data_a <= wr_data;
                end
            end else begin
                rd_data_a <= mem[rd_addr_a];
            end
        end

        if (rd_enable_b) begin
            if (hazard_b) begin
                // Compare against OLD data (mem[rd_addr_b]) at this edge
                if (wr_data !== mem[rd_addr_b]) begin
                    rd_data_b <= match_or_x_fn(wr_data, mem[rd_addr_b]);
                end else begin
                    rd_data_b <= wr_data;
                end
            end else begin
                rd_data_b <= mem[rd_addr_b];
            end
        end

        // ----------------
        // WRITE
        // ----------------
        if (wr_enable) begin
            mem[wr_addr] <= wr_data;
            end
    end

endmodule
