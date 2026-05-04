// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//-----------------------------------------------------------------------------
// Title         : Synchronous register-based fifo - wrapper of gen_fifo_reg
// Project       : Minion
//-----------------------------------------------------------------------------
// File          : fifo_reg_sync.v
//-----------------------------------------------------------------------------
// Description :
// Wrapper of gen_fifo_reg, to make parameters and IO look more like vcfifo_wr_*v_gcd async fifos
// is a register based synchronous fifo, with single cycle latency and registered output
//
// NUM_ELEMS may be 1 to n
//-----------------------------------------------------------------------------


module fifo_reg_sync #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 2
) (
   // clock and reset are single domain, vs. push_clock and pop_clock of async fifo
   input logic                  clock,
   input logic                  reset,

   // These ports are similar to the async fifo:
   output logic                 ready,
   input logic                  push,
   input logic [ELEM_SIZE-1:0]  push_data,


   output logic                 valid,
   input logic                  pop,
   output logic [ELEM_SIZE-1:0] pop_data

);

   wire                         full;
   wire [NUM_ELEMS:0]           level_unused;

   gen_fifo_reg #(.WIDTH(ELEM_SIZE), .DEPTH(NUM_ELEMS)) fifo
   (
    .wdata(push_data),
    .level(level_unused),
    .rdata(pop_data),
    .*
    );

   assign ready = !full;

   wire unused_ok = &{ // ri lint_check_waive UNARY_CONCAT
`ifndef ET_ASCENT_LINT
      1'b0,
`endif
      level_unused
   };

endmodule

