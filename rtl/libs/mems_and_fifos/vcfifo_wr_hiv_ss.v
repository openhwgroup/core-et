// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

// vcfifo_wr_hiv_ss - voltage change write high voltage semisync fifo
//   write in high-voltage domain, read in low-voltage domain
//   NOTE: This fifo only works for 1:1 clock ratios
//         of clock_push:clock_pop.  The independent 
//         clocks are controlled by a DLL and are offset 
//         relative to one another.  The jitter 
//         of these independent clocks has to be managed
//         and the timing constraints met to allow
//         guaranteed signal hand-off between edges
//         without any meta-stability!
//         
//  NOTE: minimum latency is 1.5
//        minimum depth is 3
//        If adding push_relay actual depth is increased by 1 internally, latency is increased by 1
//        If adding pop_relay  actual depth is increased by 1 internally      


module vcfifo_wr_hiv_ss #(parameter
   ELEM_SIZE       = 32,  // WIDTH
   NUM_ELEMS       = 3,   // DEPTH
   PUSH_RELAY      = 0,   // set to 1 to use an extra relay stage on the push side
   POP_RELAY       = 0    // set to 1 to use an extra relay stage on the pop side
) (
   input  logic                  clock_push,
   input  logic                  reset_push,
   input  logic                  clock_pop,
   input  logic                  reset_pop,

   output logic                  ready,
   input  logic                  push,
   input  logic [ELEM_SIZE-1:0]  push_data,

   output logic                  valid,
   input  logic                  pop,
   output logic [ELEM_SIZE-1:0]  pop_data,

   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push,
   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);


   localparam WIDTH = ELEM_SIZE;

   // shared wires between write/read domains
   logic                  push_d1_hv;
   logic                  push_d1_lv;
   logic [ELEM_SIZE-1:0]  push_data_d1_hv;
   logic [ELEM_SIZE-1:0]  push_data_d1_lv;
   logic                  pop_d1_hv;
   logic                  pop_d1_lv;
   logic                  reset_push_lv;
   logic                  reset_pop_hv;

   //----------------------------------------------------------------------------------
   // Write (High Voltage Domain)
   //----------------------------------------------------------------------------------
   vcfifo_wr_pd_ss #(
      .ELEM_SIZE (ELEM_SIZE), 
      .NUM_ELEMS (NUM_ELEMS), 
      .PUSH_RELAY (PUSH_RELAY), 
      .POP_RELAY (POP_RELAY)
   ) wr_pd_hv (
      .clock_push          ( clock_push          ),
      .reset_push          ( reset_push          ),
      .reset_pop           ( reset_pop_hv        ),
      .push                ( push                ),
      .push_data           ( push_data           ),
      .ready               ( ready               ),
      .push_d1             ( push_d1_hv          ),
      .push_data_d1        ( push_data_d1_hv     ),
      .pop_d1              ( pop_d1_hv           ),
      .dft__reset_byp_push ( dft__reset_byp_push ),
      .dft__reset_push     ( dft__reset_push     )
   );

   //----------------------------------------------------------------------------------
   // Shared signals (Mixed Voltage Domain)
   //----------------------------------------------------------------------------------
   // Write -> Read
   vclevel_shft_h2l #(.WIDTH(1))     ls_h2l_push_d1_pd_hv      (.out_lv (push_d1_lv),      .inp_hv (push_d1_hv));
   vclevel_shft_h2l #(.WIDTH(WIDTH)) ls_h2l_push_data_d1_pd_hv (.out_lv (push_data_d1_lv), .inp_hv (push_data_d1_hv));
   // Read -> Write
   vclevel_shft_l2h #(.WIDTH(1))     ls_l2h_pop_d1             (.out_hv (pop_d1_hv),       .inp_lv (pop_d1_lv));
   // Resets                                               
   vclevel_shft_h2l #(.WIDTH(1))     ls_h2l_reset_push_pd_hv   (.out_lv (reset_push_lv),   .inp_hv (reset_push));
   vclevel_shft_l2h #(.WIDTH(1))     ls_l2h_reset_pop          (.out_hv (reset_pop_hv),    .inp_lv (reset_pop));
   
      
   //----------------------------------------------------------------------------------
   // Read (Low Voltage Domain)
   //----------------------------------------------------------------------------------
   vcfifo_rd_pd_ss #(
      .ELEM_SIZE (ELEM_SIZE),
      .NUM_ELEMS (NUM_ELEMS), 
      .PUSH_RELAY (PUSH_RELAY), 
      .POP_RELAY (POP_RELAY)
   ) rd_pd_lv (
      .clock_pop          ( clock_pop          ),
      .reset_pop          ( reset_pop          ),
      .reset_push         ( reset_push_lv      ),
      .pop                ( pop                ),
      .valid              ( valid              ),
      .pop_data           ( pop_data           ),
      .push_d1            ( push_d1_lv         ),
      .push_data_d1       ( push_data_d1_lv    ),
      .pop_d1             ( pop_d1_lv          ),
      .dft__reset_byp_pop ( dft__reset_byp_pop ),
      .dft__reset_pop     ( dft__reset_pop     )
   );

      
endmodule
