// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module neigh_evict_ff_unit (
  // System signals
  input  logic                                              clock,
  input  logic                                              reset,
  input  logic  [`AD_ESR_SHIRE_ID_SIZE-1:0]                 shire_id,
  input  logic  [`NUM_NEIGH_R]                              neigh_id,
  // Evict request from minion
  input  et_link_minion_evict_req_info_t                    l2_dcache_req,
  input  logic                                              l2_dcache_req_valid,
  output logic                                              l2_dcache_req_ready,
  // Reduce local interconnect
  input  logic [`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_channel_mask,
  input  logic [`NEIGH_MAX_LOCAL_CONNECT-1:0][`MIN_PER_N_R] local_dest_id,
  input  logic                                              local_message_ready,
  output logic [`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_message_valid,
  output et_link_neigh_rsp_info_t                           local_message_info,
  // Data to L2
  output et_link_neigh_req_info_t                           evict_req,
  output logic                                              evict_valid,
  input  logic                                              evict_ready,
  // Output debug signals going to Status Monitor
  output logic                                              dbg_sm_signals
  );

  ////////////////////////////////////////////////////////////////////////////////
  // This unit stores all the outstanding evict requests for a given minion
  // pending to be sent to the L2 and already out of the minion
  ////////////////////////////////////////////////////////////////////////////////
  logic                                                           evict_ff_available;        // Stores if FF is avaiable
  logic                                                           evict_ff_available_next;
  logic                                                           evict_ff_in_accepted;      // Evict being accepted this cycle
  logic                                                           evict_ff_in_update;        // Update evict FF (only first pass)
  logic [`CORE_L2_BLOCK_EXT_PASSES_RANGE]                         evict_ff_in_counter;       // Counts number of input chunks written
  logic [`CORE_L2_BLOCK_EXT_PASSES_RANGE]                         evict_ff_in_counter_next;
  logic [`CORE_L2_BLOCK_EXT_PASSES_RANGE]                         evict_ff_last_in_pass;     // Number of input chunks to be written - 1
  logic                                                           is_last_in_pass;           // Last input pass of current evict
  et_link_nodata_req_info_t                                       evict_ff_req;
  et_link_nodata_req_info_t                                       evict_ff_req_next;
  logic [`CORE_L2_BLOCK_EXT_PASSES-1:0][`CORE_L2_BLOCK_EXT_RANGE] evict_ff_data;
  logic [`CORE_L2_BLOCK_EXT_PASSES-1:0]                           evict_ff_data_en;          // Enable signal to each of the segments of evict data buffer
  logic                                                           evict_ff_out_accepted;     // Evict being sent this cycle
  logic [`NEIGH_BLOCK_PASSES_RANGE]                               evict_ff_out_counter;      // Counts number of output chunks written
  logic [`NEIGH_BLOCK_PASSES_RANGE]                               evict_ff_out_counter_next;
  logic [`NEIGH_BLOCK_PASSES_RANGE]                               evict_ff_last_out_pass;    // Number of output chunks to be written - 1
  logic                                                           is_last_out_pass;          // Last output pass of current evict
  logic [`NEIGH_MAX_LOCAL_CONNECT-1:0]                            evict_goes_to_local;       // Evict goes to local message network
  logic                                                           out_ready;                 // Output is ready to receive data

  // Implementation
  // -----------------------------------------------------------------------------

  // Send ready signal to dcache
  assign l2_dcache_req_ready = evict_ff_available;

  ////////////////////////////////////////////////////////////////////////////////
  // Generate the evict FF
  ////////////////////////////////////////////////////////////////////////////////
  //         CLK    RST                          EN                     DOUT                 DIN                       DEF
  `RST_FF   (clock, reset,                                              evict_ff_available,  evict_ff_available_next,  1'b1)
  `RST_EN_FF(clock, reset | ~evict_ff_available, evict_ff_in_accepted,  evict_ff_in_counter, evict_ff_in_counter_next, '0)
  `EN_FF    (clock,                              evict_ff_in_update,    evict_ff_req,        evict_ff_req_next)

  for (genvar i = 0; i < `CORE_L2_BLOCK_EXT_PASSES; i++) begin : evict_ff_data_loop
    //     CLK                                 EN                     DOUT                 DIN
    `EN_FF(clock,                              evict_ff_data_en[i],   evict_ff_data[i],    l2_dcache_req.data)
    // Load data into corresponding position of the evict data buffer
    assign evict_ff_data_en[i] = evict_ff_in_accepted && (evict_ff_in_counter == i[`CORE_L2_BLOCK_EXT_PASSES_RANGE]);
  end


  assign evict_ff_req_next.id        = (`ET_LINK_ID_SIZE)'(l2_dcache_req.id);
  assign evict_ff_req_next.source    = '0; // Unused (assigned at neighborhood channel)
  assign evict_ff_req_next.wdata     = 1'b1;
  assign evict_ff_req_next.opcode    = l2_dcache_req.opcode;
  assign evict_ff_req_next.address   = l2_dcache_req.address;
  assign evict_ff_req_next.size      = l2_dcache_req.size;
  assign evict_ff_req_next.qwen      = l2_dcache_req.qwen;
  assign evict_ff_req_next.subopcode = l2_dcache_req.subopcode;

  assign evict_ff_in_accepted     = l2_dcache_req_valid && evict_ff_available;
  // Only update evict FF in the first pass
  assign evict_ff_in_update       = evict_ff_in_accepted && (evict_ff_in_counter == 0);
  assign evict_ff_in_counter_next = evict_ff_in_counter + 1'b1;

  always_comb begin
    // Check size to obtain number of passes
    case (l2_dcache_req.size)
      ET_LINK_Line: evict_ff_last_in_pass = `CORE_L2_BLOCK_EXT_PASSES   - 1;
      default:      evict_ff_last_in_pass = `CORE_L2_BLOCK_EXT_PASSES/2 - 1;
    endcase
  end

  assign is_last_in_pass = evict_ff_in_counter == evict_ff_last_in_pass;

  always_comb begin
    // If FF still available
    if (evict_ff_available) begin
      // Not available anymore when last input chunk is valid
      if (l2_dcache_req_valid && is_last_in_pass)
      evict_ff_available_next = 1'b0;
      else
      evict_ff_available_next = 1'b1;
    end
    // Not available, wait for arbiter grant
    else begin
      // Grant recevied and last output chunk, FF available again
      if (out_ready && is_last_out_pass)
      evict_ff_available_next = 1'b1;
      else
      evict_ff_available_next = 1'b0;
    end
  end

  ////////////////////////////////////////////////////////////////////////////
  // Send output
  ////////////////////////////////////////////////////////////////////////////

  //         CLK    RST                         EN                     DOUT                  DIN                        DEF
  `RST_EN_FF(clock, reset | evict_ff_available, evict_ff_out_accepted, evict_ff_out_counter, evict_ff_out_counter_next, '0)

  // Check when output is accepted
  assign evict_ff_out_accepted     = ~evict_ff_available & out_ready;
  assign evict_ff_out_counter_next = evict_ff_out_counter + 1'b1;

  always_comb begin
    // Check size to obtain number of output passes
    case (evict_ff_req.size)
      ET_LINK_Line: evict_ff_last_out_pass = `NEIGH_BLOCK_PASSES   - 1;
      default:      evict_ff_last_out_pass = `NEIGH_BLOCK_PASSES/2 - 1;
    endcase
  end

  assign is_last_out_pass = evict_ff_out_counter == evict_ff_last_out_pass;

  // Send request to regular output
  assign evict_req.id        = evict_ff_req.id;
  assign evict_req.source    = evict_ff_req.source;
  assign evict_req.wdata     = evict_ff_req.wdata;
  assign evict_req.opcode    = evict_ff_req.opcode;
  assign evict_req.address   = evict_ff_req.address;
  assign evict_req.data      = evict_ff_data[evict_ff_out_counter];
  assign evict_req.size      = evict_ff_req.size;
  assign evict_req.qwen      = evict_ff_req.qwen;
  assign evict_req.subopcode = evict_ff_req.subopcode;
  // Evict request is valid if it does not go to local
  assign evict_valid         = ~evict_ff_available & ~(|evict_goes_to_local);

  // Send request to local message network
  assign local_message_info.id     = evict_ff_req.address[`AD_ESR_MESSAGE_ID_RANGE];
  assign local_message_info.dest   = evict_ff_req.address[`AD_ESR_NEIGH_HART_ID_RANGE];
  assign local_message_info.wdata  = 1'b1;
  assign local_message_info.opcode = ET_LINK_RSP_MsgRcvData;
  assign local_message_info.data   = evict_ff_data[0];
  assign local_message_info.size   = evict_ff_req.size;
  assign local_message_info.qwen   = evict_ff_req.qwen;

  for (genvar i = 0; i < `NEIGH_MAX_LOCAL_CONNECT; i++) begin
    // Select output channel
    // Evicts go to fast local message network if they are a message directed to a local minion
    // which is wired to receive from this minion and data size is lower than or equal to 256 bits
    assign evict_goes_to_local[i] = local_channel_mask[i] &&
                                   (evict_ff_req.opcode == ET_LINK_REQ_MsgSendData) &&
                                   (evict_ff_req.address[`AD_ESR_REGION_RANGE] == `AD_ESR_REGION) &&
                                   ((evict_ff_req.address[`AD_ESR_SHIRE_ID_RANGE] == shire_id) ||
                                   (evict_ff_req.address[`AD_ESR_SHIRE_ID_RANGE] == `LOCAL_SHIRE_ID)) &&
                                   (evict_ff_req.address[`AD_ESR_AGENT_TYPE_RANGE] == `AD_ESR_AGENT_TYPE_MINION) &&
                                   (evict_ff_req.address[`AD_ESR_SHIRE_MIN_ID_RANGE] == (`AD_ESR_SHIRE_MIN_ID_SIZE)'
                                   ({neigh_id,local_dest_id[i]})) && (evict_ff_req.size <= ET_LINK_HLine);

    assign local_message_valid[i] = ~evict_ff_available & evict_goes_to_local[i];
  end

  // Check if selected output channel is ready
  assign out_ready = |evict_goes_to_local ? local_message_ready : evict_ready;

  ////////////////////////////////////////////////////////////////////////////////
  // Connect output debug signals
  ////////////////////////////////////////////////////////////////////////////////
  assign dbg_sm_signals = evict_ff_available;

endmodule

