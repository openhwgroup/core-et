// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_fl_barrier (
  // System signals
  input  logic                                         clock,
  input  logic                                         reset,
  // Fast Local Barrier request interface between cores and neigh
  input  logic [`MIN_PER_N-1:0]                        flb_core_neigh_req_valid,
  input  logic [`MIN_PER_N-1:0][`CSR_FL_BARRIER_RANGE] flb_core_neigh_req_data,
  // Fast Local Barrier response interface between cores and neigh
  output logic [`MIN_PER_N-1:0]                        flb_neigh_core_resp_valid,
  output logic                                         flb_neigh_core_resp_data,
  // Fast Local Barrier request interface between neigh and L2
  output logic                                         flb_neigh_l2_req_valid,
  output logic [`CSR_FL_BARRIER_RANGE]                 flb_neigh_l2_req_data,
  // Fast Local Barrier response interface between neigh and L2
  input  logic                                         flb_l2_neigh_resp_valid,
  input  logic                                         flb_l2_neigh_resp_data
);

///////////////////////////////////////////////////////////////////////////////
// As a request is a small amount of bits, we store the request per minion so
// we don't have a long wire arbiter between minions to neigh and back to
// minions.
///////////////////////////////////////////////////////////////////////////////

logic [`MIN_PER_N-1:0][`CSR_FL_BARRIER_RANGE] data;          // Per minion data for the FLB operation
logic [`MIN_PER_N-1:0]                        pending;       // Per minion pending FLB operation to be sent to L2
logic [`MIN_PER_N-1:0]                        pending_next;
logic [`MIN_PER_N_R]                          arb_winner;    // Winner of the arbitration
logic                                         req_fifo_full; // Request FIFO to L2 is full

for(genvar gen_it = 0; gen_it < `MIN_PER_N; gen_it++) begin
  //      CLK    RST    EN                                DOUT             DIN                     DEF
  `RST_FF(clock, reset,                                   pending[gen_it], pending_next[gen_it],   1'b0)
  `EN_FF (clock,        flb_core_neigh_req_valid[gen_it], data[gen_it],    flb_core_neigh_req_data[gen_it])
end


always_comb begin
  pending_next = pending;
  // Clears when request sent to L2
  if (!req_fifo_full)
      pending_next[arb_winner] = 1'b0;
    // Sets new pendings
  for (integer i = 0; i < `MIN_PER_N; i++) begin
        if(flb_core_neigh_req_valid[i]) pending_next[i] = 1'b1;
  end
end

///////////////////////////////////////////////////////////////////////////////
// There's an arbiter to send accesses to the fast local barrier unit in the
// L2. It pushes to a local small FIFO which entry was selected, so it knows
// for each response who it belongs to
///////////////////////////////////////////////////////////////////////////////

logic flb_neigh_l2_req_valid_next; // Sending valid FLB request to L2 next cycle

// It is an LRU between the different minions
logic [`MIN_PER_N-1:0] arb_to_l2_grant_unused_ok;
arb_lru #(
    .NUM_CLIENTS ( `MIN_PER_N    )
) arb_to_l2 (
    // System signals
    .clock       ( clock         ),
    .reset       ( reset         ),
    // Bidding
    .bid         ( pending       ),
    .stall       ( req_fifo_full ),
    // Grant     
    .grant       ( arb_to_l2_grant_unused_ok ),
    .winner      ( arb_winner    )
);

// Results are FF to meet timing
//      CLK    RST    EN                           DOUT                    DIN                          DEF
`RST_FF(clock, reset,                              flb_neigh_l2_req_valid, flb_neigh_l2_req_valid_next, 1'b0)
`EN_FF (clock,        flb_neigh_l2_req_valid_next, flb_neigh_l2_req_data,  data[arb_winner])

always_comb begin
  flb_neigh_l2_req_valid_next = |pending && !req_fifo_full;
end

///////////////////////////////////////////////////////////////////////////////
// FIFO that stores the outstanding requests to the L2 fast local barrier unit
///////////////////////////////////////////////////////////////////////////////

logic [`MIN_PER_N_R] req_fifo_pop_data; // Next minion to receive response from FLB

logic req_fifo_full_next_unused_ok;
logic req_fifo_valid_unused_ok;
logic req_fifo_valid_next_unused_ok;
gen_fifo #(
  .ELEM_SIZE   ( `MIN_PER_N_L                ),
  .NUM_ELEMS   ( `FLB_ELEMS_PER_NEIGH        ),
  .USE_LATCHES ( 0                           )
) req_fifo (
  // System signals
  .clock        ( clock                       ),
  .reset        ( reset                       ),
  // Push data  
  .push_early   ( 1'b0                        ), // Only used if implemented with latches
  .push         ( flb_neigh_l2_req_valid_next ),
  .push_data    ( arb_winner                  ),
  .full         ( req_fifo_full               ),
  .full_next    ( req_fifo_full_next_unused_ok ),
  // Pop data
  .pop          ( flb_l2_neigh_resp_valid     ),
  .pop_data     ( req_fifo_pop_data           ),
  .valid        ( req_fifo_valid_unused_ok ),
  .valid_next   ( req_fifo_valid_next_unused_ok )
);

///////////////////////////////////////////////////////////////////////////////
// Flops the returned value from the L2 and sends it to the head of the FIFO
// minion
///////////////////////////////////////////////////////////////////////////////

logic [`MIN_PER_N-1:0] flb_neigh_core_resp_valid_next; // Hot-mask array of valid to minions with the return data

//      CLK    RST    EN                       DOUT                       DIN                             DEF
`RST_FF(clock, reset,                          flb_neigh_core_resp_valid, flb_neigh_core_resp_valid_next, `MIN_PER_N'b0)
`EN_FF (clock,        flb_l2_neigh_resp_valid, flb_neigh_core_resp_data,  flb_l2_neigh_resp_data)

always_comb begin
    flb_neigh_core_resp_valid_next = {`MIN_PER_N{flb_l2_neigh_resp_valid}} & (`MIN_PER_N'b1 << req_fifo_pop_data);
end

endmodule

