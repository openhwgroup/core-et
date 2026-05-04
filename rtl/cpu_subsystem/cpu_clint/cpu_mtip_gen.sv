// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Generates RV timer interrupt. Also generates tick signal to increment
// mtime register to ensure that register counts at a 10Mhz frequency.

`include "soc.vh"

module cpu_mtip_gen (
    input logic clock,
    input logic reset,
    // Mtime counter
    input  logic [63:0] esr_mtime_ip,
    input  logic [63:0] esr_mtimecmp_ip,
    input  logic        esr_mtimecmp_updated_ip,
    // Interrupt
    output logic mtip
);

// mtip generation
// Truth table
//         update  mtip_prev  mtime_cmp  mtip_next
// case  0:   0         0         0        0       Int is not cleared, Int was not active, Nothing happens
// case  1:   0         0         1        1       Int is not cleared, Int was not active, mtime_cmp sets the int
// case  2:   0         1         0        1       Int is not cleared, so it holds mtip_prev. mtime_cmp could change due to mtime counter wraparound
// case  3:   0         1         1        1       Int is not cleared, so it holds mtip_prev. mtime_cmp could change due to mtime counter wraparound
// case  4:   1         0         0        0       Int is cleared, it was not active, mtime_cmp doesnt active it
// case  5:   1         0         1        1       Int is cleared, it was not active, but mtime_cmp actives it.
// case  6:   1         1         0        0       Int is cleared when it was active (regular use)
// case  7:   1         1         1        1       Int is cleared, it was active, but at the same cycle mtime_cmp actives it.

logic mtimecmp_updated;
always @(posedge clock) begin
    if(reset) mtimecmp_updated <= 'b0;
    else mtimecmp_updated <= esr_mtimecmp_updated_ip;
end

wire mtime_cmp;
assign mtime_cmp = (esr_mtime_ip >= esr_mtimecmp_ip);

logic mtip_int;
// Update/clear signal so it only depends on mtime_cmp new condition
// or no update/clear signal so mtip holds mtip state or sets mtip if mtime_cmp
always @(posedge clock) begin
    if(reset) begin
        mtip_int <= 'b0;
    end else begin
        if (mtimecmp_updated) mtip_int <= mtime_cmp;
        else mtip_int <= (mtip_int | mtime_cmp);
    end
end

assign mtip = mtip_int;

endmodule // ios_mtip_gen
