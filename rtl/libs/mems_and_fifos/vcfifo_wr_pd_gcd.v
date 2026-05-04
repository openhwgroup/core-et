// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_pd_gcd

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_pd_gcd #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   WR_LOV = 0,
   DOWNSTREAM_CREDITS = 0,
   FF_MEM_IMPL = 1,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (

   // write (push) interface
   output logic                 ready,
   input  logic                 push,
   input  logic [ELEM_SIZE-1:0] push_data,
   output logic                 push_credit, // only used by credit based fifo
   input  logic                 clock_push,
   input logic                  reset_push_sync,

   // cnt interface
   output logic [CNT_W-1:0]     wr_cnt_gcd,
   input  logic [CNT_W-1:0]     rd_cnt_gcd_sync,

   // rd domain read interface
   output logic [ELEM_SIZE-1:0] pop_data,
   input  logic [PTR_W-1:0]     rd_ptr,

   input  logic                 dft__reset_byp_push,
   input  logic                 dft__reset_push

);


   //----------------------------------------------------------------------------------
   // optional resync flops for negedge/posedge handoff
   //----------------------------------------------------------------------------------
   logic [CNT_W-1:0] int_rd_cnt_gcd_sync;
   logic             int_reset_push_sync;
   generate
      if (WR_LOV == 1) begin : wr_lov
         rst_repeat int_reset_push_sync_inst (
            .reset_out      ( int_reset_push_sync ), 
            .reset_in       ( reset_push_sync     ),
            .clock          ( clock_push          ),
            .dft__reset_byp ( dft__reset_byp_push ),
            .dft__reset     ( dft__reset_push     )
         );
         always @(posedge clock_push or posedge int_reset_push_sync) begin
            if (int_reset_push_sync) begin
               int_rd_cnt_gcd_sync <= '0;
            end 
            else begin
               int_rd_cnt_gcd_sync <= rd_cnt_gcd_sync;
            end
         end
      end
      else begin : wr_hiv
         assign int_rd_cnt_gcd_sync = rd_cnt_gcd_sync;
         assign int_reset_push_sync = reset_push_sync;
         wire dft_unused_ok = &{ // ri lint_check_waive UNARY_CONCAT
      `ifndef ET_ASCENT_LINT
            1'b0,
      `endif
            dft__reset_byp_push,
            dft__reset_push
         };
      end
   endgenerate


   //----------------------------------------------------------------------------------
   // credit interface
   //----------------------------------------------------------------------------------
   // wires 
   logic [CNT_W-1:0]     rd_cnt; 
   logic [CNT_W-1:0]     rd_cnt_shadow;

   // credit control
   // send credit back to master if rd_cnt != rd_cnt_shadow
   // rd_cnt = f(int_rd_cnt_gcd_sync)
   always_comb begin
      rd_cnt[CNT_W-1] = int_rd_cnt_gcd_sync[CNT_W-1];
      for (int unsigned ii=CNT_W-1; ii>0; ii--) begin
         rd_cnt[ii-1] = int_rd_cnt_gcd_sync[ii-1] ^ rd_cnt[ii];
      end
   end
   `RST_EN_FF(clock_push, int_reset_push_sync, push_credit, rd_cnt_shadow,  rd_cnt_shadow + 1'b1, '0) 
   assign push_credit = rd_cnt != rd_cnt_shadow;

   localparam CRED_W = CNT_W;
   logic [CRED_W-1:0] credits;
   logic [CRED_W-1:0] credits_next;
   wire credits_enable = push ^ push_credit;
   always_comb begin
      credits_next = push ? credits - 1'b1 : credits + 1'b1;
   end
   `RST_EN_FF(clock_push, int_reset_push_sync, credits_enable, credits,  credits_next, NUM_ELEMS[CRED_W-1:0]) 
   generate
      if (DOWNSTREAM_CREDITS > 0) begin : cred_check
         `ASSERT_ALWAYS(ERROR_VCFIFO_WR_PD_HV_CREDIT_GCD_push_on_full,  clock_push, int_reset_push_sync, push |-> ready )
      end
      else begin : no_cred_check
         wire param_unused_ok = &{1'b0, &DOWNSTREAM_CREDITS};
      end
   endgenerate


   //----------------------------------------------------------------------------------
   // vcfifo_wr_gcd
   //----------------------------------------------------------------------------------
   logic                    wr_en;
   logic [PTR_W-1:0]        wr_ptr;
   vcfifo_wr_gcd #(
      .NUM_ELEMS(NUM_ELEMS)
   ) vcfifo_wr_gcd (
      .ready      ( ready               ),
      .push       ( push                ),
      .wr_en      ( wr_en               ),
      .wr_ptr     ( wr_ptr              ),
      .wr_cnt_gcd ( wr_cnt_gcd          ),
      .rd_cnt_gcd ( int_rd_cnt_gcd_sync ),
      .clock      ( clock_push          ),
      .reset      ( int_reset_push_sync )
   );

   //----------------------------------------------------------------------------------
   // vcfifo_mem
   //----------------------------------------------------------------------------------
   vcfifo_mem  #(
      .ELEM_SIZE(ELEM_SIZE), 
      .NUM_ELEMS(NUM_ELEMS),
      .FF_MEM_IMPL (FF_MEM_IMPL)
   ) vcfifo_mem (
      .rd_addr ( rd_ptr     ),
      .rd_data ( pop_data   ),
      .wr_en   ( wr_en      ),
      .wr_addr ( wr_ptr     ),
      .wr_data ( push_data  ),
      .clk_wr  ( clock_push ) // ri lint_check_waive CLOCK_USE
   ); 
   

endmodule
