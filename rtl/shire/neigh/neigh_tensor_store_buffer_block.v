// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_tensor_store_buffer_block #(
  parameter int NUM_MINIONS = 4,
  parameter int SOURCE_OFFSET = 0
  ) (
  // System signals
  input  logic                                               clock,
  input  logic                                               reset,
  // Evict request from minion
  input  et_link_neigh_req_info_t [NUM_MINIONS-1:0]          evict_req,
  input  logic [NUM_MINIONS-1:0]                             evict_valid,
  output logic [NUM_MINIONS-1:0]                             evict_ready,
  // Request going out
  input  logic                                               evict_out_ready,
  input  logic                                               evict_out_disable_next,
  output logic                                               evict_out_valid,
  output logic                                               evict_out_valid_1st,
  output logic                                               evict_out_valid_2nd,
  output et_link_neigh_req_info_t                            evict_out_req,
  // Gets Acks going back to minions
  input  logic [NUM_MINIONS-1:0]                             tstore_min_ack,
  // Sends Acks to slave minions
  input  logic [NUM_MINIONS-1:0]                             tstore_min_resp_ready,
  output logic [NUM_MINIONS-1:0]                             tstore_min_resp_valid,
  output et_link_minion_rsp_info_t [NUM_MINIONS-1:0]         tstore_min_resp,
  // Output debug signals going to Status Monitor
  output tstore_block_dbg_sm_t                               dbg_sm_signals
  );

  localparam NUM_MINIONS_L = $clog2(NUM_MINIONS);

  // //////////////////////////////////////////////////////////////////////////////
  // Computes ready and generates out evict
  // //////////////////////////////////////////////////////////////////////////////

  `define NEIGH_TS_COOP_MASK_2WAY 1'b0
  `define NEIGH_TS_COOP_MASK_4WAY 1'b1

  logic [NUM_MINIONS-1:0]      min_ready_no_coop;
  logic [NUM_MINIONS-1:0]      min_ready_coop;
  logic [NUM_MINIONS-1:0]      min_ready_coop_reg; // Registered version of the ready signal
  logic [NUM_MINIONS/2-1:0]    min_ready_coop_qual; // Qualified version of the ready signal, only for master minions
  logic [NUM_MINIONS-1:0]      min_can_accept_new_wa; // Minion can accept a new WriteAround (no coop mask conflict with outstanding requests)
  logic [NUM_MINIONS-1:0]      evict_coop_mask; // Type of cooperative
  logic [NUM_MINIONS/2-1:0]    evict_out_entry_winner; // Arbiter grant signal
  logic [NUM_MINIONS-1:0]      evict_out_entry_winner_ext;
  logic [NUM_MINIONS-1:0]      evict_out_winner_select; // Selection for data to output request

  always_comb begin : ready_generation
    integer min_id;

    // Computes if write around and entry is ready without coop. Minimize number of bits that are used to make the detection
    // Assume that input is already properly classified in previous stages
    for (integer i = 0; i < NUM_MINIONS; i++) begin
      if (evict_req[i].subopcode[0]) // ET_LINK_WriteAround2Way = 7'd1, ET_LINK_WriteAround4Way = 7'd2
      evict_coop_mask[i] = `NEIGH_TS_COOP_MASK_2WAY;
      else
      evict_coop_mask[i] = `NEIGH_TS_COOP_MASK_4WAY;
      min_ready_no_coop[i] = evict_valid[i] && min_can_accept_new_wa[i];
    end

    // Checks final ready
    for (integer i = 0; i < NUM_MINIONS; i++) begin
      min_ready_coop[i] = min_ready_no_coop[i] && ~(evict_out_valid & evict_out_entry_winner_ext[i]); // Take no coop by default
      // 2 way cooperative
      if (evict_coop_mask[i] == `NEIGH_TS_COOP_MASK_2WAY) begin
        min_id = i / 2;

        // Only master minion arbitrates
        if ((min_id * 2) != i)
        min_ready_coop[i] &= !(evict_coop_mask[i] == evict_coop_mask[min_id * 2]);
        // Goes through the pair of minions
        for (integer j = 0; j < 2; j++) begin
          // Skip same minion (timing savings)
          if ((min_id * 2 + j) != i)
          min_ready_coop[i] &= min_ready_no_coop[min_id * 2 + j] && (evict_coop_mask[i] == evict_coop_mask[min_id * 2 + j]);
        end
      end else begin
        // Otherwise, 4 way cooperative
        min_id = i / 4;

        // Only master minion arbitrates
        if ((min_id * 4) != i)
        min_ready_coop[i] &= !(evict_coop_mask[i] == evict_coop_mask[min_id * 4]);

        // Goes through the pairing minions
        for (integer j = 0; j < 4; j++) begin
          // Skip same minion (timing savings)
          if ((min_id * 4 + j) != i)
          min_ready_coop[i] &= min_ready_no_coop[min_id * 4 + j] && (evict_coop_mask[i] == evict_coop_mask[min_id * 4 + j]);
        end
      end
    end
  end

  // CLK    RST    EN  DOUT                DIN             DEF
  `RST_FF (clock, reset,     min_ready_coop_reg, min_ready_coop, {NUM_MINIONS{1'b0}})

  // //////////////////////////////////////////////////////////////////////////////
  // Selects which of the two evicts goes out this cycle
  // For timing reasons, the arbitration is flopped and we use one cycle
  // for request analysis before passing the result to next stage
  // //////////////////////////////////////////////////////////////////////////////

  logic [NUM_MINIONS-1:0]   analyze_done; // Flag indicating that we have completed the request analysis
  logic [NUM_MINIONS-1:0]   analyze_done_next;

  logic [NUM_MINIONS/2-1:0] entry_grant_qual; // Arbiter grant signal
  logic [NUM_MINIONS-1:0]   entry_grant_ext;
  logic                     evict_out_available; // Flag indicating that we are requesting
  logic                     evict_out_available_next;
  logic                     evict_out_valid_next; // Next value for the actual request
  logic                     evict_out_1st_pass; // Flag indicating that request is in first pass
  logic                     evict_out_1st_pass_next;
  logic                     evict_out_2nd_pass; // Flag indicating that request is in second pass
  logic                     evict_out_2nd_pass_next;
  logic                     evict_pop; // Get next evict request
  logic                     evict_granted_line; // Transfer size for the granted request is one 512-bit line
  logic                     evict_out_up; // Update output request

  // Registered information during the
  logic [NUM_MINIONS-1:0]                         evict_coop_mask_reg;
  logic [NUM_MINIONS-1:0]                         evict_req_hline; // Request size decoded to be HLine or QWord
  logic [NUM_MINIONS-1:0]                         evict_req_qword;
  logic [NUM_MINIONS-1:0][`ET_LINK_QWEN_SIZE-1:0] evict_req_qwen_reg;


  // Request for next cycle
  et_link_neigh_req_info_t evict_out_req_next;

  // Two-state control to provide one extra cycle to determine which requester is being granted
  always_comb begin
    for (integer i = 0; i < NUM_MINIONS; i++) begin
      analyze_done_next[i] = !analyze_done[i] || (analyze_done[i] & !evict_ready[i]);
    end
  end

  // CLK    RST    EN   DOUT           DIN                 DEF
  `RST_FF(clock, reset,      analyze_done,  analyze_done_next,  {NUM_MINIONS{1'b0}})

  always_comb begin
    // Register that we have a request going out. We update the request with the grant when
    // we don't have a request or when the request is served
    evict_out_available_next = evict_out_available;

    if (!evict_out_available || (evict_out_ready & !evict_out_1st_pass))
    evict_out_available_next = |entry_grant_ext;

    // Generate a request out. We need to clear the request when we are requested to disable it
    // but restore it while we have an outstanding request
    if (evict_out_disable_next)
    evict_out_valid_next = 1'b0;
    else
    evict_out_valid_next = evict_out_available_next;

    // Pop data into the out register when register is free or is going to become free
    evict_pop = ~evict_out_available | (evict_out_ready & !evict_out_1st_pass);

    // Compute the transfer size that has been granted
    evict_granted_line = 1'b0;
    for(integer i = 0; i < NUM_MINIONS; i++) begin
      if (entry_grant_ext[i]) begin
        if ( (evict_req_hline[i] && (evict_coop_mask_reg[i] == `NEIGH_TS_COOP_MASK_2WAY))  ||
        (evict_req_qword[i] && (evict_coop_mask_reg[i] == `NEIGH_TS_COOP_MASK_4WAY))
        ) begin
          evict_granted_line = 1'b1;
        end
      end
    end

    // Update request registers out
    evict_out_up = |evict_valid;

  end

  always_comb begin
    // Indicate that the request will have two passes. We activate the flag when not active
    // and we are going to have a request of Line size and deactivate it after the first word handshake
    // It must always return to zero before before we can accept another transaction
    if (!evict_out_1st_pass)
    evict_out_1st_pass_next = (evict_out_available_next &
    (!evict_out_available | evict_out_ready)) &
    evict_granted_line & !evict_out_2nd_pass_next;
    else
    evict_out_1st_pass_next = !(evict_out_valid & evict_out_ready);
  end

  always_comb begin
    // Flag for second cycle of a 2-pass transfer
    if (!evict_out_2nd_pass)
    evict_out_2nd_pass_next = evict_out_1st_pass & evict_out_valid & evict_out_ready;
    else
    evict_out_2nd_pass_next = !(evict_out_valid & evict_out_ready);
  end

  // CLK    RST    EN         DOUT                    DIN                        DEF
  `RST_FF(clock, reset,            evict_out_available,    evict_out_available_next,  1'b0)
  `RST_FF(clock, reset,            evict_out_valid,        evict_out_valid_next,      1'b0)
  `RST_FF(clock, reset,            evict_out_1st_pass,     evict_out_1st_pass_next,   1'b0)
  `RST_FF(clock, reset,            evict_out_2nd_pass,     evict_out_2nd_pass_next,   1'b0)
  `EN_FF (clock,        evict_pop, evict_out_entry_winner, entry_grant_qual               )

  assign evict_out_valid_1st = evict_out_1st_pass;
  assign evict_out_valid_2nd = evict_out_2nd_pass;

  // Only master minions can arbitrate, so reduce arbiter size
  always_comb begin
    for (integer i = 0; i < NUM_MINIONS/2; i++) begin
      min_ready_coop_qual[i] = min_ready_coop_reg[2*i] & analyze_done[2*i] & ~(evict_out_valid & evict_out_entry_winner_ext[2*i]);
    end
  end

  arb_rr #(
  .NUM_REQS ( NUM_MINIONS/2     )
  ) evict_arb (
  // System signals
  .clock    ( clock               ),
  .reset    ( reset               ),
  // Bidding
  .reqs     ( min_ready_coop_qual ),
  .pop      ( evict_pop           ),
  // Grant
  .grants   ( entry_grant_qual    )
  );

  // Expand the arbiter output to match again the number of minions ... zeros will be simplified ...
  always_comb begin
    evict_out_entry_winner_ext = {NUM_MINIONS{1'b0}};
    entry_grant_ext            = {NUM_MINIONS{1'b0}};
    for (integer i = 0; i < NUM_MINIONS/2; i++) begin
      evict_out_entry_winner_ext[2*i] = evict_out_entry_winner[i];
      entry_grant_ext[2*i]            = entry_grant_qual[i];
    end
  end

  // Informative parameters that are used to implement the control logic can be registered

  for(genvar gen_min = 0; gen_min < NUM_MINIONS; gen_min++) begin
    // CLK    EN                    DOUT                          DIN
    `EN_FF (clock, evict_valid[gen_min], evict_req_qwen_reg[gen_min],  evict_req[gen_min].qwen)
    `EN_FF (clock, evict_valid[gen_min], evict_coop_mask_reg[gen_min], evict_coop_mask[gen_min])
    `EN_FF (clock, evict_valid[gen_min], evict_req_hline[gen_min],     (evict_req[gen_min].size == ET_LINK_HLine))
    `EN_FF (clock, evict_valid[gen_min], evict_req_qword[gen_min],     (evict_req[gen_min].size == ET_LINK_QWord))
  end


  // Single bit decoding of the size of the request. Line size unless in 2-way and size is QWord
  logic [NUM_MINIONS-1:0]  evict_req_size_line;
  always_comb begin
    evict_req_size_line = '0;
    for (integer i = 0; i < NUM_MINIONS; i+=2) begin
      evict_req_size_line[i] = !((evict_coop_mask_reg[i] == `NEIGH_TS_COOP_MASK_2WAY) && evict_req_qword[i]);
    end
  end

  // Outputs to requesters and to request out
  logic [NUM_MINIONS_L-1:0] evict_out_mux_sel;
  logic [NUM_MINIONS_L-1:0] evict_out_mux_sel_2way;
  logic [NUM_MINIONS_L-1:0] evict_out_mux_sel_4way_1;
  logic [NUM_MINIONS_L-1:0] evict_out_mux_sel_4way_2;
  logic [NUM_MINIONS_L-1:0] evict_out_mux_sel_4way_3;

  always_comb begin
    integer min_id;

    evict_ready = {NUM_MINIONS{1'b0}}; // Clear ready by default
    // Generates the ready
    for (integer i = 0; i < NUM_MINIONS; i++) begin
      evict_ready[i] |= evict_out_entry_winner_ext[i] & evict_out_ready & !evict_out_1st_pass; // Take no coop by default
      // 2 way cooperative
      if (~i[0] && (evict_coop_mask_reg[i] == `NEIGH_TS_COOP_MASK_2WAY)) begin
        min_id = i / 2;
        // Goes through the pair of minions
        for (integer j = 0; j < 2; j++)
        evict_ready[min_id * 2 + j] |= (evict_ready[i] && (evict_coop_mask_reg[i] == evict_coop_mask_reg[min_id * 2 + j]));
      end else if ((i[1:0] == 2'b00) && (evict_coop_mask_reg[i] == `NEIGH_TS_COOP_MASK_4WAY)) begin
        // 4 way cooperative
        min_id = i / 4;
        // Goes through the pairing minions
        for (integer j = 0; j < 4; j++)
        evict_ready[min_id * 4 + j] |= (evict_ready[i] && (evict_coop_mask_reg[i] == evict_coop_mask_reg[min_id * 4 + j]));
      end
    end

    // Mux out selection. Only even minions may be requesters
    evict_out_winner_select = evict_pop ? entry_grant_ext : evict_out_entry_winner_ext;
    evict_out_mux_sel = {NUM_MINIONS_L{1'b0}};
    for (integer i = 0; i < NUM_MINIONS; i+=2) begin
      if (evict_out_winner_select[i]) begin
        evict_out_mux_sel = {i[NUM_MINIONS_L-1:1],1'b0};
        break;
      end
    end

    // Selects winner data
    evict_out_req_next.id        = (`ET_LINK_ID_SIZE)'(ET_LINK_Core_Miss_Id_Tensor_Store_Coop);
    evict_out_req_next.opcode    = ET_LINK_REQ_WriteAround;
    evict_out_req_next.subopcode = ET_LINK_WriteAround;

    evict_out_req_next.source    = ET_LINK_NEIGH_SRC_Minion0 + SOURCE_OFFSET + evict_out_mux_sel;
    evict_out_req_next.address   = evict_req[evict_out_mux_sel].address;
    evict_out_req_next.size      = evict_req_size_line[evict_out_mux_sel] ? ET_LINK_Line : ET_LINK_HLine;
    evict_out_req_next.qwen      = evict_req_qwen_reg[evict_out_mux_sel];
    evict_out_req_next.data      = evict_req[evict_out_mux_sel].data;
    evict_out_req_next.wdata     = evict_req[evict_out_mux_sel].wdata;

    if (evict_coop_mask_reg[evict_out_mux_sel] == `NEIGH_TS_COOP_MASK_2WAY) begin
      // 128b
      if(!evict_req_size_line[evict_out_mux_sel]) begin
        evict_out_req_next.qwen = evict_req_qwen_reg[evict_out_mux_sel] | evict_req_qwen_reg[evict_out_mux_sel_2way];
        // Merges data with next minion.
        // First minion data must be in qwen[0] or qwen[2]
        // Second minion data must be in qwen[1] or qwen[3]
        evict_out_req_next.data[1 * 128 +: 128] = evict_req[evict_out_mux_sel_2way].data[1 * 128 +: 128];
      end else begin
        // 256b
        evict_out_req_next.qwen = 4'b1111;
        // Second pass will take data from secondary minion
        if (evict_out_2nd_pass_next)
        evict_out_req_next.data = evict_req[evict_out_mux_sel_2way].data;
      end
    end else if (evict_coop_mask_reg[evict_out_mux_sel] == `NEIGH_TS_COOP_MASK_4WAY) begin
      // 4 way cooperative
      // Always 128b
      evict_out_req_next.qwen = 4'b1111;
      // First pass will take data in the upper bits from minion '1'
      // Second pass will take data from minion '2' and '3'
      if (!evict_out_2nd_pass_next)
      evict_out_req_next.data[1 * 128 +: 128] = evict_req[evict_out_mux_sel_4way_1].data[1 * 128 +: 128];
      else
      evict_out_req_next.data = {evict_req[evict_out_mux_sel_4way_3].data[1 * 128 +: 128],evict_req[evict_out_mux_sel_4way_2].data[0 * 128 +: 128]};
    end
  end

  // Partner for 2-way cooperation
  generate
  if (NUM_MINIONS > 2)
  assign evict_out_mux_sel_2way = {evict_out_mux_sel[NUM_MINIONS_L-1:1],1'b1};
  else
  assign evict_out_mux_sel_2way = 1'b1;
  endgenerate

  // Partners for 4-way cooperation
  generate
  if (NUM_MINIONS > 4) begin
    assign evict_out_mux_sel_4way_1 = {evict_out_mux_sel[NUM_MINIONS_L-1:2],2'b01};
    assign evict_out_mux_sel_4way_2 = {evict_out_mux_sel[NUM_MINIONS_L-1:2],2'b10};
    assign evict_out_mux_sel_4way_3 = {evict_out_mux_sel[NUM_MINIONS_L-1:2],2'b11};
  end else begin
    assign evict_out_mux_sel_4way_1 = 2'b01;
    assign evict_out_mux_sel_4way_2 = 2'b10;
    assign evict_out_mux_sel_4way_3 = 2'b11;
  end
  endgenerate

  // CLK     EN            DOUT           DIN
  `EN_FF (clock,  evict_out_up, evict_out_req, evict_out_req_next)

  // //////////////////////////////////////////////////////////////////////////////
  // Stores for each minion outstanding WriteArounds and cooperative mask. We
  // can't start a different cooperative mask until all Acks are back.
  // //////////////////////////////////////////////////////////////////////////////

  logic [NUM_MINIONS-1:0][4:0] min_pending_ack; // Pending Acks per minion
  logic [NUM_MINIONS-1:0][3:0] min_wa_counter; // Per minion write around counter
  logic [NUM_MINIONS-1:0][3:0] min_wa_counter_next;
  logic [NUM_MINIONS-1:0]      min_wa_counter_up; // Update the counter
  logic [NUM_MINIONS-1:0]      min_coop_mask; // Per minion cooperative mask
  logic [NUM_MINIONS-1:0]      min_ack_back; // Received an Ack


  for(genvar gen_min = 0; gen_min < NUM_MINIONS; gen_min++) begin
    // CLK    RST    EN                          DOUT                     DIN                           DEF
    `RST_EN_FF(clock, reset, min_wa_counter_up[gen_min], min_wa_counter[gen_min], min_wa_counter_next[gen_min], 4'b0)
    `EN_FF    (clock,        evict_ready[gen_min],       min_coop_mask[gen_min],  evict_coop_mask_reg[gen_min])
  end


  always_comb begin
    for (integer i = 0; i < NUM_MINIONS; i++) begin
      // An Ack is coming back
      min_ack_back[i] = tstore_min_ack[i];

      // Can accept a new write around for the minion
      min_can_accept_new_wa[i] = (min_wa_counter[i] == 0) // No outstanding, can start always with whatever mask
      || (min_coop_mask[i] == evict_coop_mask_reg[i]) // Outstanding trans, can start if same mask
      || ((min_wa_counter[i] + min_pending_ack[i]) < 5'd31); // Can't run out of pending Acks for a minion

      // Update counter if new request or Ack is coming back
      min_wa_counter_up[i] = evict_ready[i] // Ready and no cooperation
      || min_ack_back[i];

      min_wa_counter_next[i] = min_wa_counter[i];
      if (evict_ready[i])  min_wa_counter_next[i] = min_wa_counter_next[i] + 4'b1; // Increment if sending a new request
      if (min_ack_back[i]) min_wa_counter_next[i] = min_wa_counter_next[i] - 4'b1; // Decrement upon arrival
    end
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Need to send Acks for the non-main minions of cooperative Tensor Stores
  // //////////////////////////////////////////////////////////////////////////////

  logic [NUM_MINIONS-1:0][4:0] min_pending_ack_next;
  logic [NUM_MINIONS-1:0]      min_pending_ack_up; // Update the pending Ack counter


  for (genvar gen_min = 0; gen_min < NUM_MINIONS; gen_min++) begin
    // CLK    RST    EN                           DOUT                      DIN                            DEF
    `RST_EN_FF(clock, reset, min_pending_ack_up[gen_min], min_pending_ack[gen_min], min_pending_ack_next[gen_min], 5'b0)
  end


  always_comb begin
    integer min_id;

    min_pending_ack_next = min_pending_ack;
    min_pending_ack_up   = {NUM_MINIONS{1'b0}};

    for(integer i = 0; i < NUM_MINIONS; i++) begin
      // 2 way cooperative Ack back. Send Ack to slave minions
      if (min_ack_back[i] && ~i[0] && (min_coop_mask[i] == `NEIGH_TS_COOP_MASK_2WAY)) begin
        min_id = i / 2;
        // Goes through the pair of minions
        for(integer j = 1; j < 2; j++) begin
          min_pending_ack_next[min_id * 2 + j] = min_pending_ack_next[min_id * 2 + j] + 5'b1;
          min_pending_ack_up[min_id * 2 + j]   = 1'b1;
        end
      end
      // 4 way cooperative Ack back. Send ack to slave minions
      else if (min_ack_back[i] && (i[1:0] == 2'b00) && (min_coop_mask[i] == `NEIGH_TS_COOP_MASK_4WAY)) begin
        min_id = i / 4;
        // Goes through the pair of minions
        for(integer j = 1; j < 4; j++) begin
          min_pending_ack_next[min_id * 4 + j] = min_pending_ack_next[min_id * 4 + j] + 5'b1;
          min_pending_ack_up[min_id * 4 + j]   = 1'b1;
        end
      end
    end

    // Computes the update
    for (integer i = 0; i < NUM_MINIONS; i += 1) begin
      // Decrement
      if (tstore_min_resp_ready[i] && tstore_min_resp_valid[i]) min_pending_ack_next[i] = min_pending_ack_next[i] - 5'b1;

      // Counter update
      min_pending_ack_up[i] |= tstore_min_resp_ready[i] && tstore_min_resp_valid[i];
    end
  end

  always_comb begin
    tstore_min_resp = '0;

    // For the minions that can have pending Acks to be sent
    for (integer i = 0; i < NUM_MINIONS; i++) begin
      tstore_min_resp_valid[i]  = (min_pending_ack[i] > 0);
      tstore_min_resp[i].id     = `ET_LINK_ID_SIZE'(ET_LINK_Core_Miss_Id_Tensor_Store_Coop);
      tstore_min_resp[i].dest   = 1'b0;
      tstore_min_resp[i].wdata  = 1'b0;
      tstore_min_resp[i].opcode = ET_LINK_RSP_Ack;
      tstore_min_resp[i].data   = 'b0;
      tstore_min_resp[i].size   = ET_LINK_Byte;
      tstore_min_resp[i].qwen   = 4'b0000;
    end
  end

  // Debug signals
  always_comb begin
    dbg_sm_signals.tstore_internal_dbg_signals = tstore_block_dbg_sm_t'({
    // Signals for the evict requests that are going out
    analyze_done, // 4 bits, 1 per minion
    entry_grant_qual, // 2 bits
    evict_out_available, // 1 bit
    evict_granted_line, // 1 bit
    evict_out_up, // 1 bit
    evict_out_1st_pass, // 1 bit
    evict_out_2nd_pass, // 1 bit
    evict_pop, // 1 bit
    evict_out_mux_sel, // 2 bits
    // Signals for the outstanding WA and coop mask
    min_ack_back, // 4 bits
    min_coop_mask // 4 bits
    });
  end

endmodule

