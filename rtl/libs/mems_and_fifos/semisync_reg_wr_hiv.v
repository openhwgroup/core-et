// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// semisync_reg_wr_hiv - voltage domain crossing write high voltage
//   write in high-voltage domain, read in low-voltage domain
//   NOTE: This register crossing only works for 1:1 clock ratios
//         of clock_wr:clock_rd.  The independent 
//         clocks are controlled by a DLL and are offset 
//         relative to one another.  The jitter 
//         of these independent clocks has to be managed
//         and the timing constraints met to allow
//         guaranteed signal hand-off between edges
//         without any meta-stability!
//         
//   NOTE: latency is 1.5 clock cycles

module semisync_reg_wr_hiv #(parameter
   WIDTH = 1
) (
   input  logic              clock_wr,
   input  logic              reset_wr,
   input  logic              clock_rd,
   input  logic              reset_rd,

   input  logic              wr_en,
   input  logic [WIDTH-1:0]  wr_data,

   output logic              rd_valid,  // indicates when wr_en would have been high in rd domain, so that the read data could be taken by an enabled flop
   output logic [WIDTH-1:0]  rd_data
);
   
   // shared wires between write/read domains
   logic             wr_en_d1_hv;
   logic [WIDTH-1:0] wr_data_d1_hv;
   logic             wr_en_d1_lv;
   logic [WIDTH-1:0] wr_data_d1_lv;

   //------------------------------------------------------------------
   // Write (High Voltage Domain)
   //------------------------------------------------------------------
   semisync_reg_wr #(
      .WIDTH(WIDTH)
   ) semisync_reg_wr_pd_hv (
      .clock_wr         (clock_wr),
      .reset_wr         (reset_wr),
      .wr_en            (wr_en),
      .wr_data          (wr_data),
      .wr_en_d1         (wr_en_d1_hv),
      .wr_data_d1       (wr_data_d1_hv)
   );
   

   //----------------------------------------------------------------------------------
   // Shared signals (Mixed Voltage Domain)
   //----------------------------------------------------------------------------------
   // Write -> Read
   vclevel_shft_h2l #(.WIDTH(1))     ls_h2l_wr_en_d1   (.out_lv (wr_en_d1_lv),   .inp_hv (wr_en_d1_hv));
   vclevel_shft_h2l #(.WIDTH(WIDTH)) ls_h2l_wr_data_d1 (.out_lv (wr_data_d1_lv), .inp_hv (wr_data_d1_hv));


   //------------------------------------------------------------------
   // Read (Low Voltage Domain)
   //------------------------------------------------------------------
   semisync_reg_rd #(
      .WIDTH(WIDTH)
   ) semisync_reg_rd_pd_lv (
      .clock_rd         (clock_rd),
      .reset_rd         (reset_rd),
      .wr_en_d1         (wr_en_d1_lv),
      .wr_data_d1       (wr_data_d1_lv),
      .rd_valid         (rd_valid),
      .rd_data          (rd_data)
   );


endmodule
