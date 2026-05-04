// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef CPU_CLINT_TYPES
`define CPU_CLINT_TYPES

`include "soc.vh"

typedef struct packed {
    logic [63:0] mtime;
    logic [63:0] mtimecmp;
    esr_time_config_t time_config;
    logic [`THREADS_PER_N-1:0] mtime_local_target;
} cpu_clint_timer_regs_t;

typedef struct packed {
    logic mtime;
    logic mtimecmp;
    logic time_config;
    logic mtime_local_target;
} cpu_clint_timer_regs_we_t;

typedef struct packed {
    logic [`THREADS_PER_N-1:0] mipi_trigger;
    logic [`THREADS_PER_N-1:0] mipi_trigger_clear;
} ipi_esr_regs_t;

typedef struct packed {
    logic mipi_trigger;
    logic mipi_trigger_clear;
} ipi_esr_regs_we_t;

`endif // CPU_CLINT_TYPES
