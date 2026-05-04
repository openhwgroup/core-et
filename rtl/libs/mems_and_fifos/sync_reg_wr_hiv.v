// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// sync_reg_wr_hiv
//   
//  This module is used for getting slow changing data signals from a high voltage domain
//  to a low voltage domain.
//
//    This is a lossy signal cdc synchronizer.  
//    A new value is accepted when both wr_en and an internal "dv_ready_unused" are high.
//    Once a new value is accepted, dv_ready_unused will go low and 
//    subsequent wr_en's are ignored until dv_ready_unused goes high again.
//    
//    On a successful write, after ~(WR_PTR_SYNC_STAGES + 1) clock_rd cycles, the accepted wr_data
//    appears on rd_data and a single pulse of rd_valid will go high.  The rd_data will remain 
//    unchanged until the next time rd_valid goes high indicating new rd_data is available.
//    The rd_valid could be used to enable a register to accept the data in the rd_domain
//    and allows power saving so that it doesn't have to be clocked on every cycle.
//    
//    A new value will be able to be taken after ~(RD_PTR_SYNC_STAGES + 1) clock_wr cycles 
//    following rd_valid going high.

module sync_reg_wr_hiv #(parameter
   WIDTH = 1,
   WR_PTR_SYNC_STAGES = 2,
   RD_PTR_SYNC_STAGES = 2
) (
   input  logic              clock_wr,
   input  logic              reset_wr,
   input  logic              clock_rd,
   input  logic              reset_rd,
   
   input  logic              wr_en,
   input  logic [WIDTH-1:0]  wr_data,
   
   output logic              rd_valid,  // indicates when wr_en would have been high in rd domain, so that the read data could be taken by an enabled flop
   output logic [WIDTH-1:0]  rd_data,

   input  logic              dft__reset_byp_wr,
   input  logic              dft__reset_wr,
   input  logic              dft__reset_byp_rd,
   input  logic              dft__reset_rd
);
   
   
   logic             wr_cnt_gcd_d1_hv;
   logic [WIDTH-1:0] wr_data_d1_hv;
   logic             wr_cnt_gcd_d1_lv;
   logic [WIDTH-1:0] wr_data_d1_lv;
   logic             rd_cnt_gcd_lv;
   logic             rd_cnt_gcd_hv;
   
   //------------------------------------------------------------------
   // Write (High Voltage Domain)
   //------------------------------------------------------------------
   // below wire is a dv-only tap point
   logic              dv_ready_unused; // ri lint_check_waive NOT_READ, HIER_NET_NOT_READ 
   sync_reg_wr #(
      .WIDTH(WIDTH), 
      .RD_PTR_SYNC_STAGES(RD_PTR_SYNC_STAGES)
   ) sync_reg_wr_pd_hv (
      .clock_wr          ( clock_wr          ),
      .reset_wr          ( reset_wr          ),
      .reset_rd          ( reset_rd          ),
      .ready             ( dv_ready_unused   ),
      .wr_en             ( wr_en             ),
      .wr_data           ( wr_data           ),
      .wr_cnt_gcd_d1     ( wr_cnt_gcd_d1_hv  ),
      .wr_data_d1        ( wr_data_d1_hv     ),
      .rd_cnt_gcd        ( rd_cnt_gcd_hv     ),
      .dft__reset_byp_wr ( dft__reset_byp_wr ),
      .dft__reset_wr     ( dft__reset_wr     )
   );
   

   //----------------------------------------------------------------------------------
   // Shared signals (Mixed Voltage Domain)
   //----------------------------------------------------------------------------------
   // Write -> Read
   vclevel_shft_h2l #(.WIDTH(1))     ls_h2l_wr_cnt_gcd_d1 (.out_lv (wr_cnt_gcd_d1_lv),  .inp_hv (wr_cnt_gcd_d1_hv));
   vclevel_shft_h2l #(.WIDTH(WIDTH)) ls_h2l_wr_data_d1    (.out_lv (wr_data_d1_lv),     .inp_hv (wr_data_d1_hv));
   vclevel_shft_l2h #(.WIDTH(1))     ls_l2h_rd_cnt_gcd_d1 (.out_hv (rd_cnt_gcd_hv),     .inp_lv (rd_cnt_gcd_lv));

   //------------------------------------------------------------------
   // Read (Low Voltage Domain)
   //------------------------------------------------------------------
   sync_reg_rd #(
      .WIDTH(WIDTH), 
      .WR_PTR_SYNC_STAGES(WR_PTR_SYNC_STAGES)
   ) sync_reg_rd_pd_lv (
      .clock_rd          ( clock_rd          ),
      .reset_rd          ( reset_rd          ),
      .reset_wr          ( reset_wr          ),
      .wr_cnt_gcd_d1     ( wr_cnt_gcd_d1_lv  ),
      .wr_data_d1        ( wr_data_d1_lv     ),
      .rd_cnt_gcd        ( rd_cnt_gcd_lv     ),
      .rd_data           ( rd_data           ),
      .rd_valid          ( rd_valid          ),
      .dft__reset_byp_rd ( dft__reset_byp_rd ),
      .dft__reset_rd     ( dft__reset_rd     )
   );


endmodule
