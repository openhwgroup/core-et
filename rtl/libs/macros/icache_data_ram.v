// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// 512 x 144-bit synchronous SRAM with CE and WE
module icache_data_ram (
    input  wire         clk,      // clock
    input  wire         ce,       // chip enable
    input  wire         we,       // write enable
    input  wire [8:0]   addr,     // 512-word address
    input  wire [143:0] din,      // write data
    output reg  [143:0] dout      // read data
);

    // Memory array: 512 words of 144 bits
    reg [143:0] mem [0:511];

    always @(posedge clk) begin
        if (ce) begin
            if (we) begin
                // Write operation
                mem[addr] <= din;
            end else begin
                // Read operation
                dout <= mem[addr];
            end
        end
    end

endmodule


