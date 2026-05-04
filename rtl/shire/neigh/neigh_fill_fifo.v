// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_fill_fifo (
  // System signals
  input  logic                                           clock,
  input  logic                                           reset,
  // Push data
  input  logic                                           push,
  input  et_link_rsp_info_t                              push_data,
  input  coop_tload_tag_table_data_t                     coop_data,
  output logic                                           full,
  // Output data
  input  logic [`NEIGH_NUM_RESP_AGENTS-1:0]              out_rsp_ready,
  output logic [`NEIGH_NUM_RESP_AGENTS-1:0]              out_rsp_valid,
  output et_link_rsp_info_t [`NEIGH_NUM_RESP_AGENTS-1:0] out_rsp_info
  );

  // INTERNAL DECLARATIONS
  fill_fifo_data_t [`NEIGH_FILL_FIFO_SIZE-1:0] fill_data;           // Data for pending fills
  logic [`NEIGH_FILL_FIFO_RANGE]               entry_written;       // Index of the currently written entry
  logic [`NEIGH_FILL_FIFO_SIZE-1:0]            entry_ready;         // Entries that are ready to do a fill
  logic [`NEIGH_FILL_FIFO_SIZE-1:0]            entry_partial_ready; // Entries that are ready to do a partial cooperative fill
  // Arbiter
  logic [`NEIGH_FILL_FIFO_SIZE-1:0]            entry_bidding;       // Final arbiter bidding
  logic [`NEIGH_FILL_FIFO_RANGE]               entry_winner;        // Winner entry
  logic                                        valid;
  logic                                        stall;
  // Update cooperative minion mask
  logic                                        coop_min_mask_up;
  fill_fifo_data_t                             coop_min_mask_up_value;

  ////////////////////////////////////////////////////////////////////////////////
  // Store cooperative information from cooperative TLoad tag table into the
  // fill FIFO along with the ET Link response and some pre-calculated info
  ////////////////////////////////////////////////////////////////////////////////
  fill_fifo_data_t                             entry_data;
  logic                                        push_data_is_msg;
  logic                                        push_data_is_coop;
  logic [`MIN_PER_N-1:0][`ET_LINK_ID_SIZE-1:0] push_data_coop_min_id;
  et_link_neigh_source_t                       push_data_dest;
  et_link_neigh_msg_dest_t                     push_data_msg_dest;

  assign push_data_is_msg  = push_data.opcode == ET_LINK_RSP_MsgRcvData;
  assign push_data_is_coop = ~push_data_is_msg & 
                          (et_link_neigh_source_t'(push_data.dest) == ET_LINK_NEIGH_SRC_Coop);
  always_comb begin
    for (integer i = 0; i < `MIN_PER_N; i++)
    push_data_coop_min_id[i] = push_data_is_coop ?
    // If it is a cooperative load
    // `SHIRE_COOP_MISS_ID_SIZE LSBs of the miss_id of each minion are kept in the tag table
    // MSBs must be filled with a core_miss_id cooperative base code
    (`ET_LINK_ID_SIZE)'(`ET_CORE_MISS_ID_COOP | `ET_CORE_MISS_ID_SIZE'(coop_data.min_miss_id[i])):
    push_data.id;

  end
  assign push_data_msg_dest = et_link_neigh_msg_dest_t'(push_data.dest);
  // For messages, minion id and thread id are contained in the dest field
  // For other destinations, it is directly indicated by the dest field
  assign push_data_dest     = push_data_is_msg ? et_link_neigh_source_t'(push_data_msg_dest.min_id)
                            : et_link_neigh_source_t'(push_data.dest);
  assign entry_data         = '{dest:          push_data_dest,
  is_coop:       push_data_is_coop,
  coop_min_mask: coop_data.coop_min_mask,
  coop_min_id:   push_data_coop_min_id,
  rsp:           push_data};

  ////////////////////////////////////////////////////////////////////////////////
  // Store responses and used entries
  ////////////////////////////////////////////////////////////////////////////////
  logic [`NEIGH_FILL_FIFO_SIZE-1:0] used_entries, used_entries_next; // Used entries

  //      CLK    RST    DOUT          DIN                DEF
  `RST_FF(clock, reset, used_entries, used_entries_next, {`NEIGH_FILL_FIFO_SIZE{1'b0}})

  // New entries are written through port A
  // Updates of the cooperative minion mask are done through port B

  rf_latch_1r_2w_par #(
    .WIDTH           ( $bits(fill_fifo_data_t)   ),
    .ENTRIES         ( `NEIGH_FILL_FIFO_SIZE     ),
    .LEVEL2_CLK_GATE ( 1                         )
  ) rf_buffer_array
  (
    // System signals
    .clk             ( clock                     ),
    .test_en         ( 1'b0                      ),
    // Read port
    .rd_data_a       ( fill_data                 ),
    // Write ports
    .wr_addr_a       ( entry_written             ),
    .wr_data_a       ( entry_data                ),
    .wr_en_a         ( push && !full             ),
    .wr_data_a_en_1p ( 1'b1                      ), // Cannot create this signal as it depends on external signal with complex dependencies
    .wr_addr_b       ( entry_winner              ),
    .wr_data_b       ( coop_min_mask_up_value    ),
    .wr_en_b         ( coop_min_mask_up          ),
    .wr_data_b_en_1p ( 1'b1                      )  // Cannot create this signal as it depends on external signal with complex dependencies
  );

  ////////////////////////////////////////////////////////////////////////////////
  // Updates the occupied entries and the contents
  ////////////////////////////////////////////////////////////////////////////////
  logic [`NEIGH_FILL_FIFO_SIZE-1:0]  first_entry_available; // First entry available
  logic                              entry_available_found; // If we found an entry available

  always_comb begin
    used_entries_next = used_entries;
    full              = &used_entries;

    // Find the 1st available entry
    first_entry_available = `ZX(`NEIGH_FILL_FIFO_SIZE, !used_entries[0]);
    entry_available_found = !used_entries[0];
    for (integer i = 1; i < `NEIGH_FILL_FIFO_SIZE; i++) begin
      first_entry_available[i] = !used_entries[i] && !entry_available_found;
      entry_available_found |= !used_entries[i];
    end

    // Update if push and available entry
    if (push && !full)
    used_entries_next |= first_entry_available;

    // Update entry when sent and not doing a partial cooperative fill
    if (valid && !entry_partial_ready[entry_winner] && !stall)
    used_entries_next[entry_winner] = 1'b0;
  end

  // Obtain address of the entry written
  always_comb begin
    entry_written = 0;
    for(integer i = 1; i < `NEIGH_FILL_FIFO_SIZE; i++)
    if(first_entry_available[i]) entry_written = i;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Check which entries are ready to be sent and select one
  ////////////////////////////////////////////////////////////////////////////////
  logic [`NEIGH_FILL_FIFO_SIZE-1:0][`MIN_PER_N-1:0] fill_data_coop_min_mask; // Per-entry cooperative minion mask
  et_link_rsp_info_t                           out_data_rsp;
  logic [`MIN_PER_N-1:0][`ET_LINK_ID_SIZE-1:0] out_data_coop_min_id;
  fill_fifo_data_t                             out_data_next;
  logic [`NEIGH_NUM_RESP_AGENTS-1:0]           out_valid;
  logic [`NEIGH_NUM_RESP_AGENTS-1:0]           out_valid_next;
  logic [`NEIGH_NUM_RESP_AGENTS-1:0]           out_rsp_next_ready;
  logic                                        out_data_is_ack;

  // Check if a response is being delivered now
  // We assume that agents receiving the response will not be ready next cycle to receive a new one (except for RspAck)
  // This will reduce the throughput to a specific agent, but will better distribute throughput among different agents
  assign out_rsp_next_ready = out_rsp_ready 
      & ~(out_rsp_valid & {`NEIGH_NUM_RESP_AGENTS{!out_data_is_ack}});
  always_comb begin
    for(integer i = 0; i < `NEIGH_FILL_FIFO_SIZE; i++) begin
      fill_data_coop_min_mask[i] = fill_data[i].coop_min_mask;

      // Entries that are ready to do a fill
      entry_ready[i] = used_entries[i] &&
      (fill_data[i].is_coop ?
      // For cooperative loads, check the minion mask
      // Check that all minions indicated in the mask have FF available
      &(out_rsp_next_ready[`NEIGH_AGENT_MINION +: `MIN_PER_N] | ~fill_data_coop_min_mask[i]) :
      // Otherwise, send to destination
      out_rsp_next_ready[fill_data[i].dest]
      );

      // Entries that are partially ready
      // They are ready to do a fill in some, but not all, of the cooperating minions
      entry_partial_ready[i] = used_entries[i]                                                      // Valid entry
          & fill_data[i].is_coop                                                                    // Cooperative fill
          & (|(out_rsp_next_ready[`NEIGH_AGENT_MINION +: `MIN_PER_N] & fill_data_coop_min_mask[i]))    // A FF entry is available
          & ~(&(out_rsp_next_ready[`NEIGH_AGENT_MINION +: `MIN_PER_N] | ~fill_data_coop_min_mask[i])); // Not all FF entries available
    end
    valid = |(entry_ready | entry_partial_ready);

    // By default regular ready entries bid
    entry_bidding = entry_ready;
    // If no regular entries, allow partial coop ready entries
    if (!(|entry_bidding))
    entry_bidding = entry_partial_ready;
  end

  // If more than one entry is ready, uses the oldest fill
  queue_arb_lru #(
    .NUM_CLIENTS ( `NEIGH_FILL_FIFO_SIZE )
  ) entry_ready_arb
  (
    // System signals
    .clock       ( clock                 ),
    .reset       ( reset                 ),
    // Update priority
    .prio_up     ( push && !full         ),
    .prio_entry  ( entry_written         ),
    // Bidding
    .bid         ( entry_bidding         ),
    .winner      ( entry_winner          )
  );

  assign out_data_next = fill_data[entry_winner];

  // Generate valids for every agent
  always_comb begin
    // If the pipe is stalled, but some of the outputs are being actually delivered
    // (e.g. a cooperative output is addressed to two minions, but only one of them
    // is accepting it this cycle), then deassert valid signals for those already delivered
    if (stall)
    out_valid_next = out_valid & ~out_rsp_ready;
    else begin
      // Valids for minions (contain logic for messages and cooperatives)
      for (integer i = 0; i < `MIN_PER_N; i++) begin
        out_valid_next[i] = valid &
        (out_data_next.is_coop ?
        // For cooperative loads, check the minion mask
        // Send to all minions indicated in the mask that have FF available
        out_rsp_next_ready[i] & out_data_next.coop_min_mask[i] :
        // Otherwise, send to destination
        out_data_next.dest == et_link_neigh_source_t'(i[`ET_LINK_SOURCE_SIZE-1:0])
        );
      end

      // Outputs for agents other than minions
      for (integer i = `MIN_PER_N; i < `NEIGH_NUM_RESP_AGENTS; i++)
        out_valid_next[i] = valid & 
            (out_data_next.dest == et_link_neigh_source_t'(i[`ET_LINK_SOURCE_SIZE-1:0]));
    end
  end

  // Flop arbiter output

  //      CLK    RST    EN              DOUT                  DIN             DEF
  `RST_FF(clock, reset,                 out_valid,            out_valid_next, 1'b0)
  `EN_FF (clock,        ~stall & valid, out_data_rsp,         out_data_next.rsp)
  `EN_FF (clock,        ~stall & valid, out_data_coop_min_id, out_data_next.coop_min_id)
  `EN_FF (clock,        ~stall & valid, out_data_is_ack,      (out_data_next.rsp.opcode == ET_LINK_RSP_Ack))

  ////////////////////////////////////////////////////////////////////////////////
  // Send to output
  ////////////////////////////////////////////////////////////////////////////////
  always_comb begin
    // Outputs for minions (contain logic for messages and cooperatives)
    for (integer i = 0; i < `MIN_PER_N; i++) begin
      out_rsp_info[i] = out_data_rsp;
      // id field for minions is pre-calculated in the coop_min_id field (in case it is a cooperative)
        out_rsp_info[i].id = out_data_coop_min_id[i];
      end

      // Outputs for agents other than minions
      for (integer i = `MIN_PER_N; i < `NEIGH_NUM_RESP_AGENTS; i++)
      out_rsp_info[i]  = out_data_rsp;
    end

    assign out_rsp_valid = out_valid;

    // If an agent deasserts the ready signal right when a response is delivered
    // to it, stall the pipe
    assign stall = |(~out_rsp_ready & out_rsp_valid);

    ////////////////////////////////////////////////////////////////////////////////
    // Update the cooperative minion mask for partial fills
    ////////////////////////////////////////////////////////////////////////////////
    always_comb begin
      coop_min_mask_up       = entry_partial_ready[entry_winner] & ~stall;
      coop_min_mask_up_value = out_data_next;

      // Clear minions that were already filled
      coop_min_mask_up_value.coop_min_mask = out_data_next.coop_min_mask 
          & ~out_rsp_next_ready[`NEIGH_AGENT_MINION +: `MIN_PER_N];
    end

  endmodule

