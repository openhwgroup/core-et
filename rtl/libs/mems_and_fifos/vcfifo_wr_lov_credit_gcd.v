// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_lov_credit_gcd
//   
//   write low voltage credit based gcd fifo
//   NOTE: sending valid data (expected to be taken, i.e. auto-pop)

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_lov_credit_gcd #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   DOWNSTREAM_CREDITS = 3,
   WR_PTR_SYNC_STAGES = 2,
   RD_PTR_SYNC_STAGES = 2,
   FF_MEM_IMPL = 1   // set to 1 to use flop based memory vs latch based memory
) (
   input  logic                  clock_push,
   input  logic                  reset_push,
   input  logic                  clock_pop,
   input  logic                  reset_pop,

   output logic                  push_credit,
   output logic                  ready,
   input  logic                  push,
   input  logic [ELEM_SIZE-1:0]  push_data,
   
   input  logic                  pop_credit,
   output logic                  empty,
   output logic                  valid,    
   output logic [ELEM_SIZE-1:0]  pop_data,

   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push,
   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);

   //----------------------------------------------------------------------------------
   // Localparams
   //----------------------------------------------------------------------------------
   localparam WR_LOV             = 1;
   localparam PTR_W              = $clog2(NUM_ELEMS);
   localparam CNT_W              = $clog2(NUM_ELEMS+1);

   //----------------------------------------------------------------------------------
   // Wires
   //----------------------------------------------------------------------------------
   logic                 clock_push_gv;
   logic                 reset_push_gv;
   logic                 dft__reset_byp_push_gv;
   logic                 dft__reset_push_gv;

   logic [ELEM_SIZE-1:0] pop_data_gv;

   logic [PTR_W-1:0]     rd_ptr;
   logic [CNT_W-1:0]     wr_cnt_gcd;
   logic [CNT_W-1:0]     rd_cnt_gcd_sync;
   logic                 reset_push_sync;


   //----------------------------------------------------------------------------------
   // Level shifters
   //----------------------------------------------------------------------------------

   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_clock_push          ( .inp_lv (clock_push)           , .out_hv (clock_push_gv)          );
   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_reset_push          ( .inp_lv (reset_push)           , .out_hv (reset_push_gv)          );
   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_byp_push ( .inp_lv (dft__reset_byp_push)  , .out_hv (dft__reset_byp_push_gv) );
   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_push     ( .inp_lv (dft__reset_push)      , .out_hv (dft__reset_push_gv)     );
   //                                                                                                   
   // vclevel_shft_l2h #(.WIDTH(ELEM_SIZE)) ls_l2h_pop_data            ( .inp_lv (pop_data_gv)          , .out_hv (pop_data)               );

   // NOTE: the vclevel_shft_* above caused additional level shifters because the buffers were placed in high_voltage
   // assign clock_push_gv          = clock_push;
   // assign reset_push_gv          = reset_push;
   // assign dft__reset_byp_push_gv = dft__reset_byp_push;
   // assign dft__reset_push_gv     = dft__reset_push;
   // assign pop_data               = pop_data_gv;

   // NOTE: replacing above with wrapper modules to allow tunneling through PD region
   vcfifo_ls_resets_gv resets_pd_gv (
      .clock_gv          ( clock_push_gv          ),
      .reset_gv          ( reset_push_gv          ),
      .dft__reset_byp_gv ( dft__reset_byp_push_gv ),
      .dft__reset_gv     ( dft__reset_push_gv     ),
      .clock_lv          ( clock_push             ),
      .reset_lv          ( reset_push             ),
      .dft__reset_byp_lv ( dft__reset_byp_push    ),
      .dft__reset_lv     ( dft__reset_push        )
   );

   vcfifo_wr_lov_dout_gv #(.ELEM_SIZE (ELEM_SIZE)) dout_pd_gv (
      .pop_data_hv ( pop_data    ),
      .pop_data_gv ( pop_data_gv )
   );


   //----------------------------------------------------------------------------------
   // Global to Low Voltage Domain
   //----------------------------------------------------------------------------------
   vcfifo_wr_lov_wr_pd_gv_gcd_wrapper #(
      .ELEM_SIZE (ELEM_SIZE),
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .FF_MEM_IMPL (FF_MEM_IMPL)
   ) vcfifo_wr_lov_wr_pd_gv (
      .ready               ( ready                  ),
      .push                ( push                   ),
      .push_data           ( push_data              ),
      .push_credit         ( push_credit            ),
      .clock_push          ( clock_push             ),
      .reset_push_sync     ( reset_push_sync        ),
      .wr_cnt_gcd          ( wr_cnt_gcd             ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync        ),
      .pop_data            ( pop_data_gv            ),
      .rd_ptr              ( rd_ptr                 ),
      .dft__reset_byp_push ( dft__reset_byp_push    ),
      .dft__reset_push     ( dft__reset_push        )
   );



   //----------------------------------------------------------------------------------
   // Global to High Voltage Domain
   //----------------------------------------------------------------------------------
   wire pop_one = 1'b1;
   vcfifo_wr_lov_rd_pd_gv_gcd_wrapper #(
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .RD_PTR_SYNC_STAGES (RD_PTR_SYNC_STAGES),
      .WR_PTR_SYNC_STAGES (WR_PTR_SYNC_STAGES)
   ) vcfifo_wr_lov_rd_pd_gv (
      .valid               ( valid                  ),
      .empty               ( empty                  ),
      .pop                 ( pop_one                ),
      .pop_credit          ( pop_credit             ),
      .clock_pop           ( clock_pop              ),
      .reset_pop           ( reset_pop              ),
      .rd_ptr              ( rd_ptr                 ),
      .wr_cnt_gcd          ( wr_cnt_gcd             ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync        ),
      .clock_push          ( clock_push_gv          ),
      .reset_push          ( reset_push_gv          ),
      .reset_push_sync     ( reset_push_sync        ),
      .dft__reset_byp_push ( dft__reset_byp_push_gv ),
      .dft__reset_push     ( dft__reset_push_gv     ),
      .dft__reset_byp_pop  ( dft__reset_byp_pop     ),
      .dft__reset_pop      ( dft__reset_pop         )
   );


endmodule

