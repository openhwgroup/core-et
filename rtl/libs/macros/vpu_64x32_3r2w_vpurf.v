// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`timescale 1ns/1ps

module vpu_64x32_3r2w_vpurf #(
    parameter WIDTH     = 32,  // bits per word
    parameter DEPTH     = 64,  // number of words
    parameter ADDR_BITS = 6    // log2(DEPTH) = 6 for 64 words
)(
    input                       clk,

    // ------------------- Write Port 0 -------------------
    input                       wr_enable0,
    input      [ADDR_BITS-1:0]  wr_addr0,
    input      [WIDTH-1:0]      wr_data0,

    // ------------------- Write Port 1 -------------------
    input                       wr_enable1,
    input      [ADDR_BITS-1:0]  wr_addr1,
    input      [WIDTH-1:0]      wr_data1,

    // ------------------- Read Port A --------------------
    input                       rd_enable0,
    input      [ADDR_BITS-1:0]  rd_addr0,
    output reg [WIDTH-1:0]      rd_data0,

    // ------------------- Read Port B --------------------
    input                       rd_enable1,
    input      [ADDR_BITS-1:0]  rd_addr1,
    output reg [WIDTH-1:0]      rd_data1,

    // ------------------- Read Port C --------------------
    input                       rd_enable2,
    input      [ADDR_BITS-1:0]  rd_addr2,
    output reg [WIDTH-1:0]      rd_data2
);

    // ----------------------------------------------------------------
    // Storage array
    // ----------------------------------------------------------------
    reg [WIDTH-1:0] mem [0:DEPTH-1];

    // ----------------------------------------------------------------
    // Utility function: bitwise "match-or-X"
    // For each bit i:
    //   - if a[i] === b[i], return that bit
    //   - else return 1'bx
    // ----------------------------------------------------------------
    function [WIDTH-1:0] match_or_x_fn;
        input [WIDTH-1:0] a;
        input [WIDTH-1:0] b;
        integer i;
        begin
            match_or_x_fn = {WIDTH{1'bx}}; // default to X
            for (i = 0; i < WIDTH; i = i + 1) begin
                match_or_x_fn[i] = (a[i] === b[i]) ? a[i] : 1'bx;
            end
        end
    endfunction

    // ----------------------------------------------------------------
    // Precompute combined-write conditions/data
    // ----------------------------------------------------------------
    wire both_wr_same_addr = wr_enable0 && wr_enable1 && (wr_addr0 == wr_addr1);

    // If both write same address, this is the value actually written:
    // - matching bits take that bit value
    // - mismatching bits become X
    wire [WIDTH-1:0] merged_wr_data = match_or_x_fn(wr_data0, wr_data1);

    // ----------------------------------------------------------------
    // Synchronous read + write behavior
    // - Reads are registered (1-cycle latency)
    // - Read hazards produce match(write_data, old_mem) per spec
    // - Memory writes occur after read RHS have sampled old mem values
    // ----------------------------------------------------------------
    always @(posedge clk) begin
        // =========================
        // READ PORT A (registered)
        // =========================
        if (rd_enable0) begin
            // Old memory content sampled at this edge (RHS is old value)
            // Use local wire to emphasize read-before-write in this cycle
            // (nonblocking assignment ensures RHS is old mem)
            reg [WIDTH-1:0] old_a;
            old_a = mem[rd_addr0];

            if (both_wr_same_addr && (rd_addr0 == wr_addr0)) begin
                // Both writes to same address and read hits that address:
                // - memory will get merged_wr_data
                // - read returns match(merged_wr_data, old_a)
                rd_data0 <= match_or_x_fn(merged_wr_data, old_a);
            end else if (wr_enable0 && (rd_addr0 == wr_addr0)) begin
                // Single write port 0 hazard
                rd_data0 <= match_or_x_fn(wr_data0, old_a);
            end else if (wr_enable1 && (rd_addr0 == wr_addr1)) begin
                // Single write port 1 hazard
                rd_data0 <= match_or_x_fn(wr_data1, old_a);
            end else begin
                // No hazard: return old memory data
                rd_data0 <= old_a;
            end
        end

        // =========================
        // READ PORT B (registered)
        // =========================
        if (rd_enable1) begin
            reg [WIDTH-1:0] old_b;
            old_b = mem[rd_addr1];

            if (both_wr_same_addr && (rd_addr1 == wr_addr0)) begin
                rd_data1 <= match_or_x_fn(merged_wr_data, old_b);
            end else if (wr_enable0 && (rd_addr1 == wr_addr0)) begin
                rd_data1 <= match_or_x_fn(wr_data0, old_b);
            end else if (wr_enable1 && (rd_addr1 == wr_addr1)) begin
                rd_data1 <= match_or_x_fn(wr_data1, old_b);
            end else begin
                rd_data1 <= old_b;
            end
        end

        // =========================
        // READ PORT C (registered)
        // =========================
        if (rd_enable2) begin
            reg [WIDTH-1:0] old_c;
            old_c = mem[rd_addr2];

            if (both_wr_same_addr && (rd_addr2 == wr_addr0)) begin
                rd_data2 <= match_or_x_fn(merged_wr_data, old_c);
            end else if (wr_enable0 && (rd_addr2 == wr_addr0)) begin
                rd_data2 <= match_or_x_fn(wr_data0, old_c);
            end else if (wr_enable1 && (rd_addr2 == wr_addr1)) begin
                rd_data2 <= match_or_x_fn(wr_data1, old_c);
            end else begin
                rd_data2 <= old_c;
            end
        end

        // =========================
        // WRITES
        // =========================
        if (both_wr_same_addr) begin
            // One write to the common address with merged data
            mem[wr_addr0] <= merged_wr_data;
        end else begin
            // Independent writes to potentially different addresses
            if (wr_enable0) mem[wr_addr0] <= wr_data0;
            if (wr_enable1) mem[wr_addr1] <= wr_data1;
        end
    end

endmodule
