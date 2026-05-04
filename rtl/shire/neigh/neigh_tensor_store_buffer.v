// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_tensor_store_buffer(
  // System signals
  input  logic                                       clock,
  input  logic                                       reset,
  // Evict request from minion
  input  et_link_neigh_req_info_t [`MIN_PER_N-1:0]   evict_req,
  input  logic [`MIN_PER_N-1:0]                      evict_valid,
  output logic [`MIN_PER_N-1:0]                      evict_ready,
  // Request going out
  input  logic                                       evict_out_ready,
  input  logic                                       evict_out_disable_next,
  output logic                                       evict_out_valid,
  output et_link_neigh_req_info_t                    evict_out_req,
  // Gets Acks going back to minions
  input  logic [`MIN_PER_N-1:0]                      tstore_min_ack,
  // Sends Acks to slave minions
  input  logic [`MIN_PER_N-1:0]                      tstore_min_resp_ready,
  output logic [`MIN_PER_N-1:0]                      tstore_min_resp_valid,
  output et_link_minion_rsp_info_t [`MIN_PER_N-1:0]  tstore_min_resp,
  // Output debug signals going to Status Monitor
  output tstore_dbg_sm_t                             dbg_sm_signals
);


// Instantiate 2 modules, one for every group of 4 minions, and then combine
// them with an intermediate arbiter that registers again the data before 
// sending the request out

// Signals
// -----------------------------------------------------------------------------
logic                      evict_out_ready_north;
logic                      evict_out_valid_north;
logic                      evict_out_valid_1st_north;
logic                      evict_out_valid_2nd_north;
et_link_neigh_req_info_t   evict_out_req_north;

logic                      evict_out_ready_south;
logic                      evict_out_valid_south;
logic                      evict_out_valid_1st_south;
logic                      evict_out_valid_2nd_south;
et_link_neigh_req_info_t   evict_out_req_south;

logic                      evict_out_disable;

tstore_block_dbg_sm_t      dbg_sm_signals_ts_buffer_north;
tstore_block_dbg_sm_t      dbg_sm_signals_ts_buffer_south;

// North instance
// -----------------------------------------------------------------------------
neigh_tensor_store_buffer_block #(
  .NUM_MINIONS   ( `MIN_PER_N/2 ),
  .SOURCE_OFFSET ( 0            )
) neigh_tensor_store_buffer_north (
  // System signals
  .clock                  ( clock                                   ),
  .reset                  ( reset                                   ),
  // Evict request from minion
  .evict_req              ( evict_req[`MIN_PER_N/2-1:0]             ),
  .evict_valid            ( evict_valid[`MIN_PER_N/2-1:0]           ),
  .evict_ready            ( evict_ready[`MIN_PER_N/2-1:0]           ),
  // Request going out
  .evict_out_ready        ( evict_out_ready_north                   ),
  .evict_out_disable_next ( 1'b0                                    ),
  .evict_out_valid        ( evict_out_valid_north                   ),
  .evict_out_valid_1st    ( evict_out_valid_1st_north               ),
  .evict_out_valid_2nd    ( evict_out_valid_2nd_north               ),
  .evict_out_req          ( evict_out_req_north                     ),
  // Gets Acks going back to minions
  .tstore_min_ack         ( tstore_min_ack[`MIN_PER_N/2-1:0]        ),
  // Sends Acks to slave minions
  .tstore_min_resp_ready  ( tstore_min_resp_ready[`MIN_PER_N/2-1:0] ),
  .tstore_min_resp_valid  ( tstore_min_resp_valid[`MIN_PER_N/2-1:0] ),
  .tstore_min_resp        ( tstore_min_resp[`MIN_PER_N/2-1:0]       ),
  // Output debug signals going to Status Monitor
  .dbg_sm_signals         ( dbg_sm_signals_ts_buffer_north          )
);

// South instance
// -----------------------------------------------------------------------------
neigh_tensor_store_buffer_block #(
  .NUM_MINIONS   ( `MIN_PER_N/2 ),
  .SOURCE_OFFSET ( `MIN_PER_N/2 )
) neigh_tensor_store_buffer_south (
  // System signals
  .clock                  ( clock                                             ),
  .reset                  ( reset                                             ),
  // Evict request from minion
  .evict_req              ( evict_req[`MIN_PER_N/2+:`MIN_PER_N/2]             ),
  .evict_valid            ( evict_valid[`MIN_PER_N/2+:`MIN_PER_N/2]           ),
  .evict_ready            ( evict_ready[`MIN_PER_N/2+:`MIN_PER_N/2]           ),
  // Request going out
  .evict_out_ready        ( evict_out_ready_south                             ),
  .evict_out_disable_next ( 1'b0                                              ),
  .evict_out_valid        ( evict_out_valid_south                             ),
  .evict_out_valid_1st    ( evict_out_valid_1st_south                         ),
  .evict_out_valid_2nd    ( evict_out_valid_2nd_south                         ),
  .evict_out_req          ( evict_out_req_south                               ),
  // Gets Acks going back to minions
  .tstore_min_ack         ( tstore_min_ack[`MIN_PER_N/2+:`MIN_PER_N/2]        ),
  // Sends Acks to slave minions
  .tstore_min_resp_ready  ( tstore_min_resp_ready[`MIN_PER_N/2+:`MIN_PER_N/2] ),
  .tstore_min_resp_valid  ( tstore_min_resp_valid[`MIN_PER_N/2+:`MIN_PER_N/2] ),
  .tstore_min_resp        ( tstore_min_resp[`MIN_PER_N/2+:`MIN_PER_N/2]       ),
  // Output debug signals going to Status Monitor
  .dbg_sm_signals         ( dbg_sm_signals_ts_buffer_south                    )
);

// Request pipeline stage
// -----------------------------------------------------------------------------

logic                    fifo_write;
logic                    fifo_pop;
logic                    fifo_valid;
logic                    fifo_full;
et_link_neigh_req_info_t fifo_data_in;
et_link_neigh_req_info_t fifo_data_out;

// FIFO with data
logic tensor_store_req_fifo_full_next_unused_ok;
logic tensor_store_req_fifo_valid_next_unused_ok;
gen_fifo #(
  .ELEM_SIZE   ( $bits(et_link_neigh_req_info_t) ),
  .NUM_ELEMS   ( 2                               ),
  .USE_LATCHES ( 0                               )
) tensor_store_req_fifo (
  // System signals
  .clock       ( clock                           ),
  .reset       ( reset                           ),
  // Push data
  .push_early  ( 1'b0                            ), // Only used when implemented with latches
  .push        ( fifo_write                      ),
  .push_data   ( fifo_data_in                    ),
  .full        ( fifo_full                       ),
  .full_next   ( tensor_store_req_fifo_full_next_unused_ok ),
  // Pop data
  .pop         ( fifo_pop                        ),
  .pop_data    ( fifo_data_out                   ),
  .valid       ( fifo_valid                      ),
  .valid_next  ( tensor_store_req_fifo_valid_next_unused_ok )
);

// Arbitrate between north and south requests
logic   grant_north;
logic   grant_north_next;

always_comb begin
  // Arbiter selects either north or south and alternates from one to the other unless a transfer is for two cycles
  grant_north_next = ( grant_north & evict_out_valid_north & evict_out_valid_1st_north &  evict_out_ready_north) ? 1'b1 :  // Keep north if 1st pass accepted in north
                     ( grant_north & evict_out_valid_north & evict_out_valid_2nd_north & !evict_out_ready_north) ? 1'b1 :  // Keep north until 2nd pass accepted
                     ( grant_north & evict_out_valid_south & (!evict_out_valid_north | evict_out_ready_north)  ) ? 1'b0 :  // Go south if south does request and north is not already waiting
                     (!grant_north & evict_out_valid_south & evict_out_valid_1st_south &  evict_out_ready_south) ? 1'b0 :  // Keep south if first pass accepted in south
                     (!grant_north & evict_out_valid_south & evict_out_valid_2nd_south & !evict_out_ready_south) ? 1'b0 :  // Keep south until 2nd pass accepted
                     (!grant_north & evict_out_valid_north & (!evict_out_valid_south | evict_out_ready_south)  ) ? 1'b1 :  // Go north if north does request and south is not already waiting
                      grant_north;                                                                                         // By default, keep same value

  // Mux input to FIFO
  fifo_write = !fifo_full && ((grant_north & evict_out_valid_north) || (!grant_north && evict_out_valid_south));
  fifo_data_in = grant_north ? evict_out_req_north : evict_out_req_south;
end

always_comb begin
  evict_out_ready_north = grant_north & !fifo_full;
  evict_out_ready_south = !grant_north & !fifo_full;
end

`RST_FF(clock, reset, grant_north,       grant_north_next,       1'b1)
`RST_FF(clock, reset, evict_out_disable, evict_out_disable_next, 1'b0)

// Output interface
always_comb begin
  evict_out_valid = fifo_valid & !evict_out_disable;
  evict_out_req   = fifo_data_out;
  fifo_pop        = fifo_valid & evict_out_ready;
end

// Debug signals
always_comb
begin
  dbg_sm_signals.tstore_north_dbg_sm_signals = dbg_sm_signals_ts_buffer_north;
  dbg_sm_signals.tstore_south_dbg_sm_signals = dbg_sm_signals_ts_buffer_south;
end

endmodule
