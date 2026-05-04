// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// Code for managing the mtime and mtimecmp registers for RV timer.
// Also increments mtime register when tick is generated
//
// Informs mtip_gen when mtimecmp has been written so that mtip_gen can
// clear the timer interrupt.

`include "soc.vh"

module cpu_clint_timer (
    input logic clock_ext,
    input logic clock,
    input logic reset,
    input cpu_clint_timer_regs_we_t regs_we_ip,
    input cpu_clint_timer_regs_t regs_ip,
    output cpu_clint_timer_regs_t regs_op,
    output logic mtip_gen
);

localparam unsigned PRESCALAR_THR_WIDTH = 7;
localparam unsigned TIME_CONFIG_WIDTH = (PRESCALAR_THR_WIDTH + 1);
localparam unsigned TIME_CONFIG_PAD_WIDTH = (64 - TIME_CONFIG_WIDTH);

// defaults
localparam logic [63:0] MTIMECMP_DEF = `ESR_SHIRE_MTIMECMP_RESET_VAL;
localparam logic [63:0] MTIME_DEF = `ESR_SHIRE_MTIME_RESET_VAL;
localparam esr_time_config_t TIME_CONFIG_DEF = esr_time_config_t'(`ESR_SHIRE_TIME_CONFIG_RESET_VAL);
localparam logic [`THREADS_PER_N-1:0] MTIME_LOCAL_TARGET_DEF = `ESR_SHIRE_MTIME_LOCAL_TARGET_RESET_VAL;

// skip syncing timecfg to mtime domain and read back right away
esr_time_config_t time_config;
//         CLK    RST    EN                      DOUT         DIN                                         DEF
`RST_EN_FF(clock, reset, regs_we_ip.time_config, time_config, regs_ip.time_config[TIME_CONFIG_WIDTH-1:0], TIME_CONFIG_DEF)
assign regs_op.time_config = {{TIME_CONFIG_PAD_WIDTH{1'b0}}, time_config};

//------------------------------------------------------------------------------
// reset sync to ext
logic reset_ext;
sync_rst i_sync_rst_ext (
    .clock (clock_ext),
    .reset_in (reset),
    .reset_out (reset_ext),
    .dft__reset_byp (1'b0),
    .dft__reset (1'b0)
);

//------------------------------------------------------------------------------
// clock source select

logic clock_m; // clock driving mtime
gf_clkmux #(
    .DEFAULT_SEL (TIME_CONFIG_DEF.ref_clock_mux)
) i_gf_clkmux (
    .clk0 (clock_ext),
    .clk1 (clock),
    .rst0 (reset_ext),
    .rst1 (reset),
    .sel (time_config.ref_clock_mux),
    .clk_off ('0 /* clk_off_int */),
    .scan_clk_stop ('0),
    .testmode ('0 /* testmode */),
    .clkout (clock_m)
);

//------------------------------------------------------------------------------
// reset sync to mtime clock domain
logic reset_m;
sync_rst i_sync_rst_m (
    .clock (clock_m),
    .reset_in (reset),
    .reset_out (reset_m),
    .dft__reset_byp (1'b0),
    .dft__reset (1'b0)
);

//------------------------------------------------------------------------------
// sync to mtime clock domain

logic time_config_thr_sync_valid;
logic [PRESCALAR_THR_WIDTH-1:0] time_config_thr_sync;
logic mtime_sync_valid;
logic [63:0] mtime_sync;
logic mtimecmp_sync_valid;
logic [63:0] mtimecmp_sync;

cpu_clint_timer_sync_in #(
    .PRESCALAR_THR_WIDTH (PRESCALAR_THR_WIDTH)
) i_cpu_clint_timer_sync_in (
    .clock (clock),
    .clock_m (clock_m),
    .reset (reset),
    // time_config_thr
    .push_time_config_thr (regs_we_ip.time_config),
    .push_time_config_thr_data (regs_ip.time_config.prescalar_threshold),
    .valid_time_config_thr (time_config_thr_sync_valid),
    .pop_time_config_thr_data (time_config_thr_sync),
    // mtime
    .push_mtime (regs_we_ip.mtime),
    .push_mtime_data (regs_ip.mtime),
    .valid_mtime (mtime_sync_valid),
    .pop_mtime_data (mtime_sync),
    // mtimecmp
    .push_mtimecmp (regs_we_ip.mtimecmp),
    .push_mtimecmp_data (regs_ip.mtimecmp),
    .valid_mtimecmp (mtimecmp_sync_valid),
    .pop_mtimecmp_data (mtimecmp_sync)
);

// ESRs
logic [PRESCALAR_THR_WIDTH-1:0] time_config_thr;
logic [63:0] mtime;
logic [63:0] mtimecmp;

logic esr_mtime_en; // tick for mtime
logic mtime_write;
assign mtime_write = (mtime_sync_valid || esr_mtime_en);

logic [63:0] mtime_next;
assign mtime_next = mtime_sync_valid ? mtime_sync : (mtime + 'h1);

//         CLK      RST      EN                          DOUT             DIN                   DEF
`RST_EN_FF(clock_m, reset_m, time_config_thr_sync_valid, time_config_thr, time_config_thr_sync, TIME_CONFIG_DEF.prescalar_threshold)
`RST_EN_FF(clock_m, reset_m, mtime_write,                mtime,           mtime_next,           MTIME_DEF)
`RST_EN_FF(clock_m, reset_m, mtimecmp_sync_valid,        mtimecmp,        mtimecmp_sync,        MTIMECMP_DEF)

// scale tick signal so that rv timer will increment at a 10MHz frequency
// input clock is assumed to be 200MHz
// generate a tick every time_config_thr cycles to increment mtime
logic [PRESCALAR_THR_WIDTH-1:0] tick_cntr;
always @ (posedge clock_m) begin
    if(reset_m) begin
        tick_cntr <= 'b0;
    end else begin
        if (tick_cntr >= time_config_thr-1) tick_cntr <= 'b0;
        else tick_cntr <= (tick_cntr + 'b1);
    end
end

assign esr_mtime_en = (tick_cntr == time_config_thr-1); // inc mtime on match

//------------------------------------------------------------------------------
// mtip gen

logic mtimecmp_updated;
//      CLK      RST      DOUT              DIN                  DEF
`RST_FF(clock_m, reset_m, mtimecmp_updated, mtimecmp_sync_valid, 1'b0)

// let mtip_gen know if the mtimecmp register was written
// when that happens mtip_gen will clear the mtip interrupt (if it is asserted)
logic mtip_gen_m;
cpu_mtip_gen i_cpu_mtip_gen(
    .clock (clock_m),
    .reset (reset_m),
    .esr_mtime_ip (mtime),
    .esr_mtimecmp_ip (mtimecmp),
    .esr_mtimecmp_updated_ip (mtimecmp_updated),
    .mtip (mtip_gen_m) // timer interrupt in clock_m domain
);

//------------------------------------------------------------------------------
// sync back to cpu subsystem domain

logic [63:0] mtime_sync_back, mtimecmp_sync_back;
logic mtime_sync_back_valid, mtimecmp_sync_back_valid;

cpu_clint_timer_sync_out i_cpu_clint_timer_sync_out (
    .clock (clock),
    .clock_m (clock_m),
    .reset_m (reset_m),
    // mtime
    .push_mtime (mtime_write),
    .push_mtime_data (mtime_next),
    .valid_mtime (mtime_sync_back_valid),
    .pop_mtime_data (mtime_sync_back),
    // mtimecmp
    .push_mtimecmp (mtimecmp_sync_valid),
    .push_mtimecmp_data (mtimecmp_sync),
    .valid_mtimecmp (mtimecmp_sync_back_valid),
    .pop_mtimecmp_data (mtimecmp_sync_back)
);

logic mtip_gen_sync_back;
et_cdc_sync mtip_gen_sync (
    .clock (clock),
    .reset (reset),
    .din (mtip_gen_m),
    .dout (mtip_gen_sync_back)
);

//------------------------------------------------------------------------------
logic [`THREADS_PER_N-1:0] mtime_lt; // local_target
//         CLK    RST    EN                             DOUT      DIN                         DEF
`RST_EN_FF(clock, reset, regs_we_ip.mtime_local_target, mtime_lt, regs_ip.mtime_local_target, MTIME_LOCAL_TARGET_DEF)
assign regs_op.mtime_local_target = mtime_lt;

// send values of mtime and mtimecmp registers to mtip_gen so that it can
// compare and determine if a timer interrupt should be generated
logic [63:0] mtime_sync_out;
logic [63:0] mtimecmp_sync_out;
//         CLK    RST    EN                        DOUT               DIN                 DEF
`RST_EN_FF(clock, reset, mtime_sync_back_valid,    mtime_sync_out,    mtime_sync_back,    MTIME_DEF)
`RST_EN_FF(clock, reset, mtimecmp_sync_back_valid, mtimecmp_sync_out, mtimecmp_sync_back, MTIMECMP_DEF)

assign regs_op.mtime = mtime_sync_out;
assign regs_op.mtimecmp = mtimecmp_sync_out;
assign mtip_gen = mtip_gen_sync_back;

endmodule // cpu_clint_timer
