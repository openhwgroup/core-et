// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// sync_reg_wr

module sync_reg_wr #(parameter
   WIDTH = 1,
   RD_PTR_SYNC_STAGES = 2
) (
   input  logic             clock_wr,
   input  logic             reset_wr,
   input  logic             reset_rd,

   output logic             ready,
   input  logic             wr_en,
   input  logic [WIDTH-1:0] wr_data,

   output logic             wr_cnt_gcd_d1,
   output logic [WIDTH-1:0] wr_data_d1,

   input  logic             rd_cnt_gcd,

   input  logic             dft__reset_byp_wr,
   input  logic             dft__reset_wr

);
   
   
   //----------------------------------------------------------------------------------
   // Sync, Combine resets
   //----------------------------------------------------------------------------------
   logic reset_sync;
   sync_rst sync_rst (
      .reset_out      ( reset_sync           ), 
      .reset_in       ( reset_wr || reset_rd ),
      .clock          ( clock_wr             ),
      .dft__reset_byp ( dft__reset_byp_wr    ),
      .dft__reset     ( dft__reset_wr        )

   );
   
   
   //------------------------------------------------------------------
   // Write Domain
   //------------------------------------------------------------------
   logic rd_cnt_gcd_sync;
   et_cdc_sync #(
      .WIDTH(1), 
      .SYNC_STAGES(RD_PTR_SYNC_STAGES)
   ) rd_ack_sync (
      .clock   ( clock_wr        ),
      .reset   ( reset_sync      ),
      .din     ( rd_cnt_gcd      ),
      .dout    ( rd_cnt_gcd_sync )
   );


   logic rd_cnt_gcd_sync_d1;

   wire  push_w = wr_en && ready;  // take new data
   wire  rd_ack = rd_cnt_gcd_sync_d1 ^ rd_cnt_gcd_sync;

   `RST_EN_FF(clock_wr, reset_sync, push_w || rd_ack,   ready,              !push_w,               '1)
   `RST_EN_FF(clock_wr, reset_sync, push_w,             wr_cnt_gcd_d1,      wr_cnt_gcd_d1 + 1'b1,  '0)
   `EN_FF(    clock_wr,             push_w,             wr_data_d1,         wr_data)
   `RST_EN_FF(clock_wr, reset_sync, rd_ack,             rd_cnt_gcd_sync_d1, rd_cnt_gcd_sync,       '0)


endmodule
