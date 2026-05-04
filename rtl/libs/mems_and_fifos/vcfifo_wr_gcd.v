// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_gcd

`include "macros.vh"

module vcfifo_wr_gcd #(parameter
   NUM_ELEMS = 8,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (

   // input interface
   output logic                ready,
   input  logic                push,

   // vcfifo_mem interface
   output logic                wr_en,
   output logic [PTR_W-1:0]    wr_ptr,

   // gcd cnts
   output logic [CNT_W-1:0]    wr_cnt_gcd,
   input  logic [CNT_W-1:0]    rd_cnt_gcd,

   // global
   input  logic                clock,
   input  logic                reset
);

   localparam [PTR_W-1:0] LAST_ENTRY = NUM_ELEMS - 1'b1;

   logic [CNT_W-1:0] wr_cnt;
   assign            wr_en           = push && ready;
   wire  [PTR_W-1:0] wr_ptr_next     = (wr_ptr == LAST_ENTRY) ? '0 : wr_ptr + 1'b1;
   wire  [CNT_W-1:0] wr_cnt_next     = wr_cnt + 1'b1;
   wire  [CNT_W-1:0] wr_cnt_gcd_next = {1'b0, wr_cnt_next[1 +: (CNT_W-1)]} ^ wr_cnt_next;

   `RST_EN_FF(clock, reset, wr_en,  wr_ptr,      wr_ptr_next,      '0)
   `RST_EN_FF(clock, reset, wr_en,  wr_cnt,      wr_cnt_next,      '0)
   `RST_EN_FF(clock, reset, wr_en,  wr_cnt_gcd,  wr_cnt_gcd_next,  '0)

   // convert gcd to cnt   
   logic [CNT_W-1:0] rd_cnt;
   always_comb begin
      rd_cnt[CNT_W-1] = rd_cnt_gcd[CNT_W-1];
      for (int unsigned ii=CNT_W-1; ii>0; ii--) begin
         rd_cnt[ii-1] = rd_cnt_gcd[ii-1] ^ rd_cnt[ii];
      end
   end

   wire [CNT_W-1:0] level = (wr_cnt - rd_cnt);
   wire             full = (level == NUM_ELEMS[CNT_W-1:0]);

   // NOTE: some designs are attempting to push while fifo is in reset, so mask to be safe
   assign           ready = !full && !reset;

endmodule
