// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_hiv_gcd
// 
//    write low voltage gcd fifo

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_hiv_gcd #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   WR_PTR_SYNC_STAGES = 2,
   RD_PTR_SYNC_STAGES = 2,
   FF_MEM_IMPL = 1   // set to 1 to use flop based memory vs latch based memory
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

   //----------------------------------------------------------------------------------
   // Localparams
   //----------------------------------------------------------------------------------
   localparam WR_LOV             = 0;
   localparam DOWNSTREAM_CREDITS = 0;
   localparam PTR_W              = $clog2(NUM_ELEMS);
   localparam CNT_W              = $clog2(NUM_ELEMS+1);

   //----------------------------------------------------------------------------------
   // Wires
   //----------------------------------------------------------------------------------
   logic                 clock_pop_gv;
   logic                 reset_pop_gv;
   logic                 dft__reset_byp_pop_gv;
   logic                 dft__reset_pop_gv;

   logic [ELEM_SIZE-1:0] pop_data_gv;

   logic [PTR_W-1:0]     rd_ptr;
   logic [CNT_W-1:0]     wr_cnt_gcd_sync;
   logic [CNT_W-1:0]     rd_cnt_gcd;
   logic                 reset_pop_sync;


   //----------------------------------------------------------------------------------
   // Level shifters
   //----------------------------------------------------------------------------------

   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_clock_pop           ( .inp_lv (clock_pop)           , .out_hv (clock_pop_gv)           );
   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_reset_pop           ( .inp_lv (reset_pop)           , .out_hv (reset_pop_gv)           );
   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_byp_pop  ( .inp_lv (dft__reset_byp_pop)  , .out_hv (dft__reset_byp_pop_gv)  );
   // vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_pop      ( .inp_lv (dft__reset_pop)      , .out_hv (dft__reset_pop_gv)      );
   // 
   // vclevel_shft_h2l #(.WIDTH(ELEM_SIZE)) ls_h2l_pop_data            ( .inp_hv (pop_data_gv)         , .out_lv (pop_data)               );

   // NOTE: the vclevel_shft_* above caused additional level shifters because the buffers were placed in high_voltage
   // assign clock_pop_gv          = clock_pop;
   // assign reset_pop_gv          = reset_pop;
   // assign dft__reset_byp_pop_gv = dft__reset_byp_pop;
   // assign dft__reset_pop_gv     = dft__reset_pop;
   // assign pop_data              = pop_data_gv;

   // NOTE: replacing above with wrapper modules to allow tunneling through PD region
   vcfifo_ls_resets_gv resets_pd_gv (
      .clock_gv          ( clock_pop_gv          ),
      .reset_gv          ( reset_pop_gv          ),
      .dft__reset_byp_gv ( dft__reset_byp_pop_gv ),
      .dft__reset_gv     ( dft__reset_pop_gv     ),
      .clock_lv          ( clock_pop             ),
      .reset_lv          ( reset_pop             ),
      .dft__reset_byp_lv ( dft__reset_byp_pop    ),
      .dft__reset_lv     ( dft__reset_pop        )
   );

   vcfifo_wr_hiv_dout_gv #(.ELEM_SIZE (ELEM_SIZE)) dout_pd_gv (
      .pop_data_lv ( pop_data    ),
      .pop_data_gv ( pop_data_gv )
   );


   //----------------------------------------------------------------------------------
   // Global to High Voltage Domain
   //----------------------------------------------------------------------------------
   wire push_credit_unused;
   vcfifo_wr_hiv_wr_pd_gv_gcd_wrapper #(
      .ELEM_SIZE (ELEM_SIZE),
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .FF_MEM_IMPL (FF_MEM_IMPL),
      .RD_PTR_SYNC_STAGES (RD_PTR_SYNC_STAGES),
      .WR_PTR_SYNC_STAGES (WR_PTR_SYNC_STAGES),
      .PTR_W (PTR_W),
      .CNT_W (CNT_W) 
   ) vcfifo_wr_hiv_wr_pd_gv (
      .ready               ( ready                 ),
      .push                ( push                  ),
      .push_data           ( push_data             ),
      .push_credit         ( push_credit_unused    ),
      .clock_push          ( clock_push            ),
      .reset_push          ( reset_push            ),
      .pop_data            ( pop_data_gv           ),
      .rd_ptr              ( rd_ptr                ),
      .wr_cnt_gcd_sync     ( wr_cnt_gcd_sync       ),
      .rd_cnt_gcd          ( rd_cnt_gcd            ),
      .clock_pop           ( clock_pop_gv          ),
      .reset_pop           ( reset_pop_gv          ),
      .reset_pop_sync      ( reset_pop_sync        ),
      .dft__reset_byp_push ( dft__reset_byp_push   ),
      .dft__reset_push     ( dft__reset_push       ),
      .dft__reset_byp_pop  ( dft__reset_byp_pop_gv ),
      .dft__reset_pop      ( dft__reset_pop_gv     )
   );

                
   //----------------------------------------------------------------------------------
   // Global to Low Voltage Domain
   //----------------------------------------------------------------------------------
   wire pop_credit_zero = 1'b0;
   wire empty_unused;
   vcfifo_wr_hiv_rd_pd_gv_gcd_wrapper #(
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS)
   ) vcfifo_wr_hiv_rd_pd_gv (
      .valid              ( valid                 ),
      .empty              ( empty_unused          ),
      .pop                ( pop                   ),
      .pop_credit         ( pop_credit_zero       ),
      .clock_pop          ( clock_pop             ),
      .reset_pop_sync     ( reset_pop_sync        ),
      .rd_ptr             ( rd_ptr                ),
      .rd_cnt_gcd         ( rd_cnt_gcd            ),
      .wr_cnt_gcd_sync    ( wr_cnt_gcd_sync       ),
      .dft__reset_byp_pop ( dft__reset_byp_pop    ),
      .dft__reset_pop     ( dft__reset_pop        )

   );
      

endmodule

