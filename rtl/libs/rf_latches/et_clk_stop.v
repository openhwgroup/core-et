// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_stop.v
//
// This module provides the ability to cleanly stop clocks that are not sourced from a gf_clk_mux

module et_clk_stop
#(
    parameter SYNC_STAGES = 2
)
(
    // Output stoppable clock
    output              clk_out,

    // Input clock to stop
    input               clk_in,

    // Optional functional clk gate signal, tie to 0 if not used
    // (for clocks that already have a functional clock gate)
    input               clk_en,

    // Clock gate cell te signals
    input               clk_gate_te,

    // Scan dump mode enable
    input               sd_mode,

    // Clock stop arm/enable signal
    input               sd_cs_st_en_clk,
    
    // Clock stop signal
    input               sd_cs_st_clk,
    
    // Stop clock asynchronously, in case clocks are dead (bypasses synchronizer flops)
    input               sd_cs_st_as_clk
);

   et_clk_stop_grp_cells #(.SYNC_STAGES(SYNC_STAGES)) ET_GROUP_CELLS_et_clk_stop_grp_cells (
       .clk_out (clk_out),
       .clk_in (clk_in),
       .clk_en (clk_en),
       .clk_gate_te (clk_gate_te),
       .sd_mode (sd_mode),
       .sd_cs_st_en_clk (sd_cs_st_en_clk),
       .sd_cs_st_clk (sd_cs_st_clk),
       .sd_cs_st_as_clk (sd_cs_st_as_clk)
   );

endmodule

// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_clk_stop_grp_cells #(parameter SYNC_STAGES = 2)
(
    output              clk_out,
    input               clk_in,
    input               clk_en,
    input               clk_gate_te,
    input               sd_mode,
    input               sd_cs_st_en_clk,
    input               sd_cs_st_clk,
    input               sd_cs_st_as_clk
);

   wire                    sd_cs_st_clk_sync;
   wire                    raw_stop_clk;
   wire                    stop_clk_n;
   wire                    clk_gate_clk_en;

   // Synchronize clock stop signal to clock domain to be stopped
   et_cdc_sync
   #(
       .SYNC_STAGES(SYNC_STAGES)
   )
   et_cdc_sync_sd_cs_st_clk
   (
       .clock(clk_in),
       .reset(1'b0),
       .din(sd_cs_st_clk),
       .dout(sd_cs_st_clk_sync)
   );
   
   // Mux between synchronous clock stop signal and asynchronous clock stop signal
   et_combo_mux2
   et_combo_mux2_sel_sync_cs
   (
       .d0(sd_cs_st_clk_sync),
       .d1(sd_cs_st_clk),
       .s(sd_cs_st_as_clk),
       .x(raw_stop_clk)
   );
   
   // Require sd_mode, sd_cs_st_en_clk, and the actual clock stop signal to be 1
   // to actually stop the clock
   et_combo_nand3
   et_combo_nand3_cs
   (
       .a1(sd_mode),
       .a2(sd_cs_st_en_clk),
       .a3(raw_stop_clk),
       .x(stop_clk_n)
   );
   
   // Commbine clock stop signal and functional clk_en
   et_combo_and
   et_combo_and_cs
   (
       .a1(clk_en),
       .a2(stop_clk_n),
       .x(clk_gate_clk_en)
   );
   
   et_clk_gate
   et_clk_gate_cs
   (
       .en(clk_gate_clk_en),
       .clk(clk_in),
       .te(clk_gate_te),
       .enclk(clk_out)
   );

endmodule

