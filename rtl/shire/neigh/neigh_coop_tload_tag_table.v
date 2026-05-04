// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_coop_tload_tag_table #(
  DEPTH = 8
) (
  // System signals
  input  logic                            clock,
  input  logic                            reset,
  // Write ports
  input  coop_tload_tag_table_req_t [1:0] wr_req,
  input  logic [1:0]                      wr_valid,
  output logic [1:0]                      wr_ready,
  // CAM port
  input  logic                            cam_pop,
  input  [`SHIRE_COOP_LOAD_ID_RANGE]      cam_tag,
  output coop_tload_tag_table_data_t      cam_data
);

localparam ADDR_SZ = $clog2(DEPTH);

// INTERNAL DECLARATIONS
logic [1:0]                            wr_en;
logic [1:0][ADDR_SZ-1:0]               wr_addr;
coop_tload_tag_table_req_t [DEPTH-1:0] tag_table;
logic [ADDR_SZ-1:0]                    cam_addr;

////////////////////////////////////////////////////////////////////////////////
// Update used entries
////////////////////////////////////////////////////////////////////////////////
logic [DEPTH-1:0]                      used_entries, used_entries_next; // Used entries
logic                                  full, full_next;
logic [DEPTH-1:0]                      first_entry_available;           // First entry available
logic                                  first_entry_available_found;     // If we found an entry available
logic [DEPTH-1:0]                      second_entry_available;          // Second entry available
logic                                  second_entry_available_found;    // If we found a second entry available

//      CLK    RST    DOUT          DIN                DEF
`RST_FF(clock, reset, used_entries, used_entries_next, '0)
`RST_FF(clock, reset, full,         full_next,         1'b0)

always_comb begin
  used_entries_next = used_entries;
  // Find the 1st and 2nd available entries
  first_entry_available[0] = ~used_entries[0];
  second_entry_available[0] = 1'b0;
  first_entry_available_found = ~used_entries[0];
  second_entry_available_found = 1'b0;

  for (integer i = 1; i < DEPTH; i++) begin
    first_entry_available[i] = ~used_entries[i] & ~first_entry_available_found;
    second_entry_available[i] = ~used_entries[i] & ~second_entry_available_found & ~first_entry_available[i];
    first_entry_available_found |= ~used_entries[i];
    second_entry_available_found |= ~used_entries[i] & ~first_entry_available[i];
  end
    // If writing through and port, update first available entry
  if (|wr_en)
    used_entries_next |= first_entry_available;
      // If writing through both ports, update second available entry as well
  if (&wr_en)
          used_entries_next |= second_entry_available;
  
    // Free entry when popped by CAM port
  if (cam_pop)
        used_entries_next[cam_addr] = 1'b0;
end

assign full_next = &used_entries_next;

////////////////////////////////////////////////////////////////////////////////
// Obtain address of the currently written entry
////////////////////////////////////////////////////////////////////////////////
always_comb begin
  wr_addr = '0;
  for (integer i = 1; i < DEPTH; i++) begin
      if (first_entry_available[i])  wr_addr[0] = i[ADDR_SZ-1:0];
      if (second_entry_available[i]) wr_addr[1] = i[ADDR_SZ-1:0];
  end
end

////////////////////////////////////////////////////////////////////////////////
// Pre-arbitrate write ports in case there is only one available entry
////////////////////////////////////////////////////////////////////////////////
logic       wr_arb_stall;
logic [1:0] wr_arb_grant;
logic [1:0] wr_arb_grant_next;

logic wr_arb_winner_unused_ok;
arb_lru #(
  .NUM_CLIENTS ( 2                 )
) wr_arb (
  // System signals
  .clock       ( clock             ),
  .reset       ( reset             ),
  // Bidding
  .bid         ( wr_valid          ),
  .stall       ( wr_arb_stall      ),
  // Grant
  .grant       ( wr_arb_grant_next ),
  .winner      ( wr_arb_winner_unused_ok )
);

// Grant is flopped to avoid timing paths
// This may add some additional latency, but should not affect overall throughput

//         CLK    RST    EN             DOUT          DIN                DEF
`RST_EN_FF(clock, reset, ~wr_arb_stall, wr_arb_grant, wr_arb_grant_next, 2'b01)

// Update winner only when actual arbitration happens
assign wr_arb_stall = full | second_entry_available_found;

// Access is given to both ports if two available entries are found
// Otherwise, access is arbitrated
assign wr_ready = {2{~full}} & ({2{second_entry_available_found}} | wr_arb_grant);
assign wr_en    = wr_valid & wr_ready;

////////////////////////////////////////////////////////////////////////////////
// Store write request data
////////////////////////////////////////////////////////////////////////////////

always_ff @(posedge clock) begin
  // New data to first available entry
  if (|wr_en) begin
    // If write port 0 is given access, write data from port 0 to first available entry
    if (wr_en[0])
        tag_table[wr_addr[0]] <= wr_req[0];
    // Otherwise, write data from port 1
    else
        tag_table[wr_addr[0]] <= wr_req[1];
  end

    // New data to second available entry
    // If both ports are given access, write data from port 1 to second available entry
  if (&wr_en)
    tag_table[wr_addr[1]] <= wr_req[1];
end

////////////////////////////////////////////////////////////////////////////////
// CAM port
////////////////////////////////////////////////////////////////////////////////
logic [DEPTH-1:0]   cam_hit_array;

// Obtain the matching entry
always_comb begin
  for (integer i = 0; i < DEPTH; i++)
      cam_hit_array[i] = (cam_tag == tag_table[i].tag) & used_entries[i];
end

// Encoded address
always_comb begin
  cam_addr = '0;
  for (integer i = 0; i < DEPTH; i++) 
    if (cam_hit_array[i])
      cam_addr |= i[ADDR_SZ-1:0];
  
end

// Returned data for matching tag
assign cam_data = tag_table[cam_addr].data;

endmodule
