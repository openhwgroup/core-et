// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_rd_pd_gcd

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_rd_pd_gcd #(parameter
   NUM_ELEMS = 8,
   WR_LOV = 0,
   DOWNSTREAM_CREDITS = 0,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (

   // read (pop) interface
   output logic                  valid,
   output logic                  empty,
   input  logic                  pop,   
   input  logic                  pop_credit,
   input  logic                  clock_pop,
   input  logic                  reset_pop_sync,

   // rd ptr to write domain to select rd_domain mux
   output logic [PTR_W-1:0]      rd_ptr,

   // cnt interface
   output logic [CNT_W-1:0]      rd_cnt_gcd,
   input  logic [CNT_W-1:0]      wr_cnt_gcd_sync,

   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);
   
   //----------------------------------------------------------------------------------
   // optional resync flops for negedge/posedge handoff
   //----------------------------------------------------------------------------------
   logic [CNT_W-1:0] int_wr_cnt_gcd_sync;
   logic             int_reset_pop_sync;
   generate
      if (WR_LOV == 0) begin : wr_hiv
         rst_repeat int_reset_pop_sync_inst (
            .reset_out      ( int_reset_pop_sync ), 
            .reset_in       ( reset_pop_sync     ),
            .clock          ( clock_pop          ),
            .dft__reset_byp ( dft__reset_byp_pop ),
            .dft__reset     ( dft__reset_pop     )
         );
         always @(posedge clock_pop or posedge int_reset_pop_sync) begin
            if (int_reset_pop_sync) begin
               int_wr_cnt_gcd_sync <= '0;
            end 
            else begin
               int_wr_cnt_gcd_sync <= wr_cnt_gcd_sync;
            end
         end
      end
      else begin : wr_lov
         assign int_wr_cnt_gcd_sync = wr_cnt_gcd_sync;
         assign int_reset_pop_sync = reset_pop_sync;
         wire dft_unused_ok = &{ // ri lint_check_waive UNARY_CONCAT
      `ifndef ET_ASCENT_LINT
            1'b0,
      `endif
            dft__reset_byp_pop,
            dft__reset_pop
         };
      end
   endgenerate
      
   
   //----------------------------------------------------------------------------------
   // credit interface
   //----------------------------------------------------------------------------------
   localparam CRED_W = (DOWNSTREAM_CREDITS == 0) ? 1 : $clog2(DOWNSTREAM_CREDITS+1);
   logic [CRED_W-1:0] credits;
   logic [CRED_W-1:0] credits_next;
   wire credits_enable = valid ^ pop_credit;
   always_comb begin
      credits_next = valid ? credits - 1'b1 : credits + 1'b1;
   end
   `RST_EN_FF(clock_pop, int_reset_pop_sync, credits_enable, credits,  credits_next, DOWNSTREAM_CREDITS[CRED_W-1:0]) 
   wire any_credit = |credits;
   logic int_any_credit;
   logic int_valid;
   logic int_pop;
   assign empty = !int_valid;
   generate
      if (DOWNSTREAM_CREDITS == 0) begin : no_credits
         assign int_any_credit = any_credit || 1'b1; // force int_any_credit always high; synth will remove this gate
         assign int_pop = pop;
      end
      else begin : use_credit
         wire pop_unused = pop;
         assign int_any_credit = any_credit;
         assign int_pop = valid;
      end
   endgenerate
   assign valid = int_any_credit && int_valid;
   
   
   //----------------------------------------------------------------------------------
   // vcfifo_rd_gcd
   //----------------------------------------------------------------------------------
   vcfifo_rd_gcd #(
      .NUM_ELEMS(NUM_ELEMS)
   ) vcfifo_rd_gcd(
      .valid      ( int_valid           ),
      .pop        ( int_pop             ),
      .rd_ptr     ( rd_ptr              ),
      .rd_cnt_gcd ( rd_cnt_gcd          ),
      .wr_cnt_gcd ( int_wr_cnt_gcd_sync ),
      .clock      ( clock_pop           ),
      .reset      ( int_reset_pop_sync  )
   );
   
endmodule
