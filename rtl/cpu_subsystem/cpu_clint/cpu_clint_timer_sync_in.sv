// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// module to cross between cpu_subsystem clock domain, and mtime's clock domain

`include "soc.vh"

module cpu_clint_timer_sync_in #(
    parameter unsigned PRESCALAR_THR_WIDTH = 7
) (
    input logic clock,
    input logic clock_m,
    input logic reset,
    // time_config_thr
    input logic push_time_config_thr,
    input logic [PRESCALAR_THR_WIDTH-1:0] push_time_config_thr_data,
    output logic valid_time_config_thr,
    output logic [PRESCALAR_THR_WIDTH-1:0] pop_time_config_thr_data,
    // mtime
    input logic push_mtime,
    input logic [63:0] push_mtime_data,
    output logic valid_mtime,
    output logic [63:0] pop_mtime_data,
    // mtimecmp
    input logic push_mtimecmp,
    input logic [63:0] push_mtimecmp_data,
    output logic valid_mtimecmp,
    output logic [63:0] pop_mtimecmp_data
);

logic ready_unused_time_config_thr;
logic ready_unused_mtime;
logic ready_unused_mtimecmp;

fifo_gcd #(
    .ELEM_SIZE(PRESCALAR_THR_WIDTH), // data
    .NUM_ELEMS(5) // 1GHz push, 200MHz or 1GHz pop
) time_config_thr_sync (
    .clock_push (clock), // comes in on cpu_subsystem
    .reset_push (reset),
    .clock_pop (clock_m), // goes out on mtime clk i.e. cpu_subsystem OR external
    .reset_pop (1'b0), // FIFO uses cross-reset synchronization, only one reset necessary
    .push (push_time_config_thr),
    .push_data (push_time_config_thr_data),
    .ready (ready_unused_time_config_thr),
    .pop (1'b1), // pop right away
    .valid (valid_time_config_thr),
    .pop_data (pop_time_config_thr_data),
    .dft__reset_byp_push (1'b0),
    .dft__reset_push (1'b0),
    .dft__reset_byp_pop (1'b0),
    .dft__reset_pop (1'b0)
);

fifo_gcd #(
    .ELEM_SIZE(64),
    .NUM_ELEMS(5) // 1GHz push, 200MHz or 1GHz pop
) mtime_sync (
    .clock_push (clock), // comes in on cpu_subsystem
    .reset_push (reset),
    .clock_pop (clock_m), // goes out on mtime clk i.e. cpu_subsystem OR external
    .reset_pop (1'b0), // FIFO uses cross-reset synchronization, only one reset necessary
    .push (push_mtime),
    .push_data (push_mtime_data),
    .ready (ready_unused_mtime),
    .pop (1'b1), // pop right away
    .valid (valid_mtime),
    .pop_data (pop_mtime_data),
    .dft__reset_byp_push (1'b0),
    .dft__reset_push (1'b0),
    .dft__reset_byp_pop (1'b0),
    .dft__reset_pop (1'b0)
);

fifo_gcd #(
    .ELEM_SIZE(64),
    .NUM_ELEMS(5) // 1GHz push, 200MHz or 1GHz pop
) mtimecmp_sync (
    .clock_push (clock), // comes in on cpu_subsystem
    .reset_push (reset),
    .clock_pop (clock_m), // goes out on mtime clk i.e. cpu_subsystem OR external
    .reset_pop (1'b0), // FIFO uses cross-reset synchronization, only one reset necessary
    .push (push_mtimecmp),
    .push_data (push_mtimecmp_data),
    .ready (ready_unused_mtimecmp),
    .pop (1'b1), // pop right away
    .valid (valid_mtimecmp),
    .pop_data (pop_mtimecmp_data),
    .dft__reset_byp_push (1'b0),
    .dft__reset_push (1'b0),
    .dft__reset_byp_pop (1'b0),
    .dft__reset_pop (1'b0)
);


endmodule
