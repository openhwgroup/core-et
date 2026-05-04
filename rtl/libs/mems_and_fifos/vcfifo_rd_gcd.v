// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_rd_gcd

`include "macros.vh"

module vcfifo_rd_gcd #(parameter
   NUM_ELEMS = 8,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (
   output logic              valid,
   input  logic              pop,

   output logic [PTR_W-1:0]  rd_ptr,
   output logic [CNT_W-1:0]  rd_cnt_gcd,
   input  logic [CNT_W-1:0]  wr_cnt_gcd,

   input  logic              clock,
   input  logic              reset
);
   
   localparam [PTR_W-1:0] LAST_ENTRY = NUM_ELEMS - 1'b1;

   logic [CNT_W-1:0] rd_cnt;
   wire              pop_en          = pop && valid;
   wire  [PTR_W-1:0] rd_ptr_next     = (rd_ptr == LAST_ENTRY) ? '0 : rd_ptr + 1'b1;
   wire  [CNT_W-1:0] rd_cnt_next     = rd_cnt + 1'b1;
   wire  [CNT_W-1:0] rd_cnt_gcd_next = {1'b0, rd_cnt_next[1 +: (CNT_W-1)]} ^ rd_cnt_next;

  `RST_EN_FF(clock, reset, pop_en,  rd_ptr,      rd_ptr_next,      '0)
  `RST_EN_FF(clock, reset, pop_en,  rd_cnt,      rd_cnt_next,      '0)
  `RST_EN_FF(clock, reset, pop_en,  rd_cnt_gcd,  rd_cnt_gcd_next,  '0)

   wire   empty = rd_cnt_gcd == wr_cnt_gcd;
   assign valid = !empty && !reset;

endmodule

