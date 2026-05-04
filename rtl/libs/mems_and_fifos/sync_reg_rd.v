// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// sync_reg_rd

module sync_reg_rd #(parameter
   WIDTH = 1,
   WR_PTR_SYNC_STAGES = 2
) (
   input  logic             clock_rd,
   input  logic             reset_rd,
   input  logic             reset_wr,

   input  logic             wr_cnt_gcd_d1,
   input  logic [WIDTH-1:0] wr_data_d1,

   output logic             rd_cnt_gcd,
   output logic [WIDTH-1:0] rd_data,
   output logic             rd_valid,

   input  logic             dft__reset_byp_rd,
   input  logic             dft__reset_rd

);
   

   //----------------------------------------------------------------------------------
   // Sync, Combine resets
   //----------------------------------------------------------------------------------
   logic reset_sync;
   sync_rst sync_rst (
      .reset_out      ( reset_sync           ), 
      .reset_in       ( reset_wr || reset_rd ),
      .clock          ( clock_rd             ),
      .dft__reset_byp ( dft__reset_byp_rd    ),
      .dft__reset     ( dft__reset_rd        )
   );
   
   //------------------------------------------------------------------
   // Read Domain
   //------------------------------------------------------------------
   logic wr_cnt_gcd_sync;
   et_cdc_sync #(
      .WIDTH(1), 
      .SYNC_STAGES(WR_PTR_SYNC_STAGES)
   ) rd_ack_sync (
      .clock   ( clock_rd        ),
      .reset   ( reset_sync      ),
      .din     ( wr_cnt_gcd_d1   ),
      .dout    ( wr_cnt_gcd_sync )
   );

   assign rd_cnt_gcd = wr_cnt_gcd_sync; // pass back gcd pointer to write domain after it is synchronized into read domain

   logic rd_cnt_gcd_d1;
   wire  rd_data_en = rd_cnt_gcd_d1 ^ rd_cnt_gcd;
   `RST_EN_FF(clock_rd, reset_sync, rd_data_en,   rd_cnt_gcd_d1, rd_cnt_gcd, '0)
   `RST_FF(   clock_rd, reset_sync,               rd_valid,      rd_data_en, '0)
   `EN_FF(    clock_rd,             rd_data_en,   rd_data,       wr_data_d1)

endmodule
