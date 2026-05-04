// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

////////////////////////////////////////////////////////////////////////////
// Each dcache has a FF to store a fill coming from the L2. When a fill is
// received (512b) it sequences the data to the dcache in chunks. Once
// the data is drained, a new fill can be accepted for this minion
////////////////////////////////////////////////////////////////////////////

module neigh_fill_ff (
  // System signals
  input  logic                               clock,
  input  logic                               reset,
  // Fills from L2
  input  et_link_rsp_info_t                  fill_fifo_resp,
  input  logic                               fill_fifo_valid,
  output logic                               fill_fifo_ready,
  // Response from cooperative TensorStore
  input  et_link_minion_rsp_info_t           tstore_min_resp,
  input  logic                               tstore_min_resp_valid,
  output logic                               tstore_min_resp_ready,
  // Reduce local interconnect
  input  et_link_neigh_rsp_info_t            local_message_info,
  input  logic                               local_message_valid,
  output logic                               local_message_ready,
  // Messages from TBOX
  input  et_link_neigh_rsp_info_t            tbox_min_rsp_info,
  input  logic                               tbox_min_rsp_valid,
  output logic                               tbox_min_rsp_ready,
  // Data to minion
  output et_link_minion_rsp_info_t           l2_resp,
  output logic                               l2_resp_valid,
  input  logic                               l2_resp_ready,
  // Indication to tensor store buffer of the Ack
  output logic                               tstore_min_ack,
  // Output debug signals going to Status Monitor
  output fill_ff_dbg_sm_t                    dbg_sm_signals
  );

  localparam BLOCK_SIZE = `CORE_L2_BLOCK_EXT_SIZE;
  localparam PASSES     = `CORE_L2_BLOCK_EXT_PASSES;

  // INTERNAL DECLARATIONS
  et_link_neigh_msg_dest_t           fill_fifo_resp_msg_dest;
  et_link_neigh_msg_dest_t           local_message_info_msg_dest;
  et_link_neigh_msg_dest_t           tbox_min_rsp_info_msg_dest;
  logic                              fill_write_l2;       // Do a write from L2
  logic                              fill_write_tstore;   // Do a write from tensor store coop
  logic                              fill_write_fln;      // Do a write from local message network
  logic                              fill_write_tbox;     // Do a write from TBOX
  logic                              fill_write;          // Do a write to the fill FF
  et_link_minion_nodata_rsp_info_t   fill_info_mux;       // Muxed fill ET Link response info between local responses and L2
  logic [`ET_LINE_DATA_SIZE-1:0]     fill_data_mux;       // Muxed fill ET Link response data between local responses and L2
  logic                              fill_pop;            // Pop ET Link response from fill FF FIFO
  logic                              fill_full_next;      // Fill FF FIFO will be full next cycle
  et_link_minion_nodata_rsp_info_t   fill_rsp_info;       // ET Link response info
  logic [PASSES-1:0][BLOCK_SIZE-1:0] fill_rsp_data;       // ET Link response data to sequence to minion
  logic                              fill_rsp_valid;      // Valid ET Link response to sequence to minion
  logic                              fill_rsp_ready;      // Output ready to accept ET Link response
  logic                              fill_out_accepted;   // Fill output is accepted
  logic [$clog2(PASSES)-1:0]         fill_out_count;      // Counts the number of output fills
  logic [$clog2(PASSES)-1:0]         fill_out_count_next; // Counts the number of output fills
  et_link_minion_rsp_info_t          l2_resp_next;
  logic                              l2_resp_valid_next;
  logic                              tstore_min_ack_next;

  ////////////////////////////////////////////////////////////////////////////
  // Select input
  ////////////////////////////////////////////////////////////////////////////
  logic local_message_ready_next;
  logic tbox_min_rsp_ready_next;
  logic fill_ff_ready_for_no_msg;
  logic fill_ff_ready_for_no_msg_next;

  //      CLK    RST    DOUT                      DIN                            DEF
  `RST_FF(clock, reset, local_message_ready,      local_message_ready_next,      1'b0)
  `RST_FF(clock, reset, tbox_min_rsp_ready,       tbox_min_rsp_ready_next,       1'b0)
  `RST_FF(clock, reset, fill_ff_ready_for_no_msg, fill_ff_ready_for_no_msg_next, 1'b0)

  // If FIFO will be available next cycle and either FLN or TBOX are to write, pre-reserve next cycle
  assign local_message_ready_next      = ~fill_full_next & local_message_valid;
  assign tbox_min_rsp_ready_next       = ~fill_full_next & ~local_message_valid & tbox_min_rsp_valid;
  assign fill_ff_ready_for_no_msg_next = ~fill_full_next & ~local_message_valid & ~tbox_min_rsp_valid;

  // Can receive data from FILL FIFO when fill FF available and
  // access is not reserved to either FLN or TBOX
  assign fill_fifo_ready = fill_ff_ready_for_no_msg;
  // Can receive data from TSTORE buffer when fill FF available,
  // access is not reserved to either FLN or TBOX and not receiving from FILL FIFO
  assign tstore_min_resp_ready = fill_ff_ready_for_no_msg && !fill_fifo_valid;

  assign fill_write_l2     = fill_fifo_valid       & fill_fifo_ready;
  assign fill_write_tstore = tstore_min_resp_valid & tstore_min_resp_ready;
  assign fill_write_fln    = local_message_valid   & local_message_ready;
  assign fill_write_tbox   = tbox_min_rsp_valid    & tbox_min_rsp_ready;
  assign fill_write        = fill_write_l2 | fill_write_tstore | fill_write_fln | fill_write_tbox;

  always_comb begin
    // For messages, minion id and thread id are contained in the dest field
    fill_fifo_resp_msg_dest = et_link_neigh_msg_dest_t'(fill_fifo_resp.dest);

    // Get fill info from L2
    fill_info_mux.id     = fill_fifo_resp.id;
    fill_info_mux.dest   = fill_fifo_resp_msg_dest.thread_id;
    fill_info_mux.wdata  = fill_fifo_resp.wdata;
    fill_info_mux.opcode = fill_fifo_resp.opcode;
    // Re-align data in chunks of BLOCK_SIZE bits, according to qwen info
    fill_data_mux[0 +: BLOCK_SIZE] = |fill_fifo_resp.qwen[1:0] ? fill_fifo_resp.data[0 +: BLOCK_SIZE]
        : fill_fifo_resp.data[BLOCK_SIZE +: BLOCK_SIZE];
    fill_data_mux[BLOCK_SIZE +: BLOCK_SIZE] = fill_fifo_resp.data[BLOCK_SIZE +: BLOCK_SIZE];
    fill_info_mux.size   = fill_fifo_resp.size;
    fill_info_mux.qwen   = fill_fifo_resp.qwen;

    if (fill_write_fln) begin
      // For messages, minion id and thread id are contained in the dest field
      local_message_info_msg_dest = et_link_neigh_msg_dest_t'(local_message_info.dest);

      // If writing from local, get needed info only
      fill_info_mux.id                 = local_message_info.id;
      fill_info_mux.dest               = local_message_info_msg_dest.thread_id;
      fill_info_mux.wdata              = 1'b1;
      fill_info_mux.opcode             = ET_LINK_RSP_MsgRcvData;
      fill_data_mux[`NEIGH_DATA_RANGE] = local_message_info.data;
      fill_info_mux.size               = local_message_info.size;
      fill_info_mux.qwen               = local_message_info.qwen;
    end
    else if (fill_write_tbox) begin
      // For messages, minion id and thread id are contained in the dest field
      tbox_min_rsp_info_msg_dest = et_link_neigh_msg_dest_t'(tbox_min_rsp_info.dest);

      // If writing from TBOX, get needed info only
      fill_info_mux.id                 = tbox_min_rsp_info.id;
      fill_info_mux.dest               = tbox_min_rsp_info_msg_dest.thread_id;
      fill_info_mux.wdata              = 1'b1;
      fill_info_mux.opcode             = ET_LINK_RSP_MsgRcvData;
      fill_data_mux[`NEIGH_DATA_RANGE] = tbox_min_rsp_info.data;
      fill_info_mux.size               = tbox_min_rsp_info.size;
      fill_info_mux.qwen               = tbox_min_rsp_info.qwen;
    end
    else if (fill_write_tstore) begin
      fill_info_mux.id     = tstore_min_resp.id;
      fill_info_mux.dest   = tstore_min_resp.dest;
      fill_info_mux.wdata  = tstore_min_resp.wdata;
      fill_info_mux.opcode = tstore_min_resp.opcode;
      fill_info_mux.size   = tstore_min_resp.size;
      fill_info_mux.qwen   = tstore_min_resp.qwen;
    end
  end

  ////////////////////////////////////////////////////////////////////////////
  // Store selected input
  ////////////////////////////////////////////////////////////////////////////

  // FIFO with ET Link response info
  logic fill_ff_info_fifo_full_unused_ok;
  logic fill_ff_info_fifo_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE   ( $bits(et_link_minion_nodata_rsp_info_t) ),
    .NUM_ELEMS   ( 2                                       ),
    .USE_LATCHES ( 0                                       )
  ) fill_ff_info_fifo (
    // System signals
    .clock       ( clock                                  ),
    .reset       ( reset                                  ),
    // Push data
    .push_early  ( 1'b0                                   ), // Only used when implemented with latches
    .push        ( fill_write                             ),
    .push_data   ( fill_info_mux                          ),
    .full        ( fill_ff_info_fifo_full_unused_ok       ),
    .full_next   ( fill_full_next                         ),
    // Pop data
    .pop         ( fill_pop                               ),
    .pop_data    ( fill_rsp_info                          ),
    .valid       ( fill_rsp_valid                         ),
    .valid_next  ( fill_ff_info_fifo_valid_next_unused_ok )
  );

  // FIFO with ET Link response data
  logic fill_ff_data_fifo_full_unused_ok;
  logic fill_ff_data_fifo_full_next_unused_ok;
  logic fill_ff_data_fifo_valid_unused_ok;
  logic fill_ff_data_fifo_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE   ( `ET_LINE_DATA_SIZE                ),
    .NUM_ELEMS   ( 2                                 ),
    .USE_LATCHES ( 0                                 )
  ) fill_ff_data_fifo (
    // System signals
    .clock       ( clock                             ),
    .reset       ( reset                             ),
    // Push data
    .push_early  ( 1'b0                              ), // Only used when implemented with latches
    .push        ( fill_write && fill_info_mux.wdata ),
    .push_data   ( fill_data_mux                     ),
    .full        ( fill_ff_data_fifo_full_unused_ok ),
    .full_next   ( fill_ff_data_fifo_full_next_unused_ok ),
    // Pop data
    .pop         ( fill_pop && fill_rsp_info.wdata   ),
    .pop_data    ( fill_rsp_data                     ),
    .valid       ( fill_ff_data_fifo_valid_unused_ok ),
    .valid_next  ( fill_ff_data_fifo_valid_next_unused_ok )
  );

  // Pop response from FIFO when doing last fill
  assign fill_pop = fill_out_accepted && (fill_out_count == lastFillCounter(fill_rsp_info.wdata,fill_rsp_info.size));

  ////////////////////////////////////////////////////////////////////////////
  // Compose output
  ////////////////////////////////////////////////////////////////////////////

  //         CLK    RST     EN                 DOUT            DIN                  DEF
  `RST_EN_FF(clock, reset,  fill_out_accepted, fill_out_count, fill_out_count_next, '0)
  `EN_FF    (clock,         fill_out_accepted, l2_resp,        l2_resp_next)
  `RST_EN_FF(clock, reset,  fill_rsp_ready,    l2_resp_valid,  l2_resp_valid_next,  1'b0)
  `RST_FF   (clock, reset,                     tstore_min_ack, tstore_min_ack_next, 1'b0)

  // Update output fill counter
  always_comb begin
    fill_out_count_next = fill_out_count + 1'b1;
    if (fill_pop)
      fill_out_count_next = '0;
  end

  // Ack indication for Tensor Store Cooperative.
  // Register it for timing purposes, one cycle of latency is not an issue.
  assign tstore_min_ack_next = fill_out_accepted
      && (fill_rsp_info.opcode == ET_LINK_RSP_Ack)
      && (et_link_core_miss_id_t'(fill_rsp_info.id[`ET_CORE_MISS_ID_RANGE]) 
      == ET_LINK_Core_Miss_Id_Tensor_Store_Coop);

  // Deliver output to minion
  assign fill_rsp_ready    = l2_resp_ready | ~l2_resp_valid;
  assign fill_out_accepted = fill_rsp_valid & fill_rsp_ready;

  assign l2_resp_next.id     = fill_rsp_info.id;
  assign l2_resp_next.dest   = fill_rsp_info.dest;
  assign l2_resp_next.wdata  = fill_rsp_info.wdata;
  assign l2_resp_next.opcode = fill_rsp_info.opcode;
  assign l2_resp_next.data   = fill_rsp_data[fill_out_count];
  assign l2_resp_next.size   = fill_rsp_info.size;
  assign l2_resp_next.qwen   = fill_rsp_info.qwen;
  assign l2_resp_valid_next  = fill_rsp_valid;

  ////////////////////////////////////////////////////////////////////////////
  // Connect output debug signals
  ////////////////////////////////////////////////////////////////////////////
  assign dbg_sm_signals = '{local_message_ready: local_message_ready,
                           tbox_min_rsp_ready:       tbox_min_rsp_ready,
                           fill_ff_ready_for_no_msg: fill_ff_ready_for_no_msg};

  ////////////////////////////////////////////////////////////////////////////
  // Helper functions
  ////////////////////////////////////////////////////////////////////////////
  function automatic [$clog2(PASSES)-1:0] lastFillCounter;
    input logic          fill_wdata;
    input et_link_size_t fill_size; 
    begin
      if (fill_wdata) begin
        case(fill_size)
          ET_LINK_Line:  lastFillCounter = 512/BLOCK_SIZE - 1;
          // ET_LINK_HLine not used
          default:       lastFillCounter = 0;
        endcase
      end
      else
        lastFillCounter = 0;
    end
  endfunction

endmodule
