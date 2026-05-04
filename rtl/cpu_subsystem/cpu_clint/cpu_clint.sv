// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


`include "soc.vh"

module cpu_clint (
    input logic clock_ext,
    input logic clock,
    input logic reset,
    // timer
    input cpu_clint_timer_regs_we_t timer_regs_we,
    input cpu_clint_timer_regs_t timer_regs_write,
    output cpu_clint_timer_regs_t timer_regs_read,
    output logic [`THREADS_PER_N-1:0] mtip, // Machine Timer Interrupt Pending
    // ipi
    input logic [`THREADS_PER_N-1:0] esr_ipi_trigger,
    output logic [`THREADS_PER_N-1:0] msip // Machine Software Interrupt Pending
);

//------------------------------------------------------------------------------
// timer interrupt
logic mtip_gen;

// logic to manage reads and writes to timer registers
cpu_clint_timer i_cpu_clint_timer (
    .clock_ext (clock_ext),
    .clock (clock),
    .reset (reset),
    .regs_we_ip (timer_regs_we),
    .regs_ip (timer_regs_write),
    .regs_op (timer_regs_read),
    .mtip_gen (mtip_gen)
);

assign mtip = (
    {`THREADS_PER_N{mtip_gen}} & timer_regs_read.mtime_local_target
);

//------------------------------------------------------------------------------
// software interrupt
assign msip = esr_ipi_trigger; // as written to ESR bank, bit per hart

endmodule
