// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_tbox_router (
  // System signals
  input  logic                      clock,
  input  logic                      reset,
  // Minion TBOX request bus
  input  et_link_neigh_req_info_t   min_tbox_req_info,
  input  logic                      min_tbox_req_valid_early,
  input  logic                      min_tbox_req_valid,
  output logic                      min_tbox_req_ready,
  // Minion TBOX reply bus
  output et_link_neigh_rsp_info_t   tbox_min_rsp_info,
  output logic                      tbox_min_rsp_valid,
  input  logic                      tbox_min_rsp_ready,
  // TBOX L2 request bus
  input  logic                      tbox_sc_req_disable_next,
  output et_link_nodata_req_info_t  tbox_sc_req_info,
  output logic                      tbox_sc_req_valid,
  input  logic                      tbox_sc_req_ready,
  // TBOX L2 reply bus
  input  et_link_rsp_info_t         sc_tbox_rsp_info,
  input  logic                      sc_tbox_rsp_valid,
  output logic                      sc_tbox_rsp_ready,
  // TBOX neighborhood request bus
  input  et_link_neigh_req_info_t   tbox_neigh_req_info,
  input  logic                      tbox_neigh_req_valid,
  output logic                      tbox_neigh_req_ready,
  // TBOX neighborhood response bus
  output et_link_rsp_info_t         tbox_neigh_rsp_info,
  output logic                      tbox_neigh_rsp_valid,
  input  logic                      tbox_neigh_rsp_ready
  );

  // //////////////////////////////////////////////////////////////////////////////
  // Minion to TBOX message requests are converted into message replies
  // //////////////////////////////////////////////////////////////////////////////
  logic                    min_tbox_req_push_early;
  logic                    min_tbox_req_push;
  logic                    min_tbox_req_fifo_full;
  et_link_neigh_req_info_t min_tbox_req_pop_data;
  logic                    min_tbox_req_pop_valid;
  logic                    min_tbox_req_pop_valid_next;
  logic                    min_tbox_req_pop;
  et_link_rsp_info_t       min_tbox_rsp_info;
  logic                    min_tbox_rsp_valid;
  logic                    min_tbox_rsp_valid_next;
  logic                    min_tbox_rsp_ready;

  gen_fifo #(
    .ELEM_SIZE   ( $bits(et_link_neigh_req_info_t) ),
    .NUM_ELEMS   ( `NEIGH_MIN_TBOX_REQ_FIFO_SIZE   ),
    .USE_LATCHES ( 1                               )
  ) min_tbox_req_fifo (
    // System signals
    .clock       ( clock                           ),
    .reset       ( reset                           ),
    // Push data
    .push_early  ( min_tbox_req_push_early         ),
    .push        ( min_tbox_req_push               ),
    .push_data   ( min_tbox_req_info               ),
    .full        ( min_tbox_req_fifo_full          ),
    .full_next   (                                 ),
    // Pop data
    .pop         ( min_tbox_req_pop                ),
    .pop_data    ( min_tbox_req_pop_data           ),
    .valid       ( min_tbox_req_pop_valid          ),
    .valid_next  ( min_tbox_req_pop_valid_next     )
  );

  assign min_tbox_req_push_early = min_tbox_req_valid_early;
  assign min_tbox_req_push       = min_tbox_req_valid & ~min_tbox_req_fifo_full;
  assign min_tbox_req_ready      = ~min_tbox_req_fifo_full;

  // Convert minion requests to 512-bit MsgRcvData replies

  assign min_tbox_rsp_info.id     = min_tbox_req_pop_data.address[`AD_ESR_MESSAGE_ID_RANGE];
  assign min_tbox_rsp_info.dest   = '0;
  assign min_tbox_rsp_info.wdata  = 1'b1;
  assign min_tbox_rsp_info.opcode = ET_LINK_RSP_MsgRcvData;
  assign min_tbox_rsp_info.data   = (`ET_LINE_DATA_SIZE)'(min_tbox_req_pop_data.data);
  assign min_tbox_rsp_info.size   = min_tbox_req_pop_data.size;
  assign min_tbox_rsp_info.qwen   = min_tbox_req_pop_data.qwen;
  assign min_tbox_rsp_valid       = min_tbox_req_pop_valid;
  assign min_tbox_rsp_valid_next  = min_tbox_req_pop_valid_next;
  assign min_tbox_req_pop         = min_tbox_rsp_ready;

  // //////////////////////////////////////////////////////////////////////////////
  // TBOX neighborhood request FIFO
  // //////////////////////////////////////////////////////////////////////////////
  logic                    tbox_neigh_req_push;
  logic                    tbox_neigh_req_fifo_full;
  et_link_neigh_req_info_t tbox_neigh_req_pop_data;
  logic                    tbox_neigh_req_pop_valid;
  logic                    tbox_neigh_req_pop;
  logic                    tbox_neigh_req_goes_to_sc;
  logic                    tbox_sc_req_valid_next;
  logic                    tbox_min_rsp_valid_next;

  // CLK    RST    DOUT                DIN                      DEF
  `RST_FF (clock, reset, tbox_sc_req_valid,  tbox_sc_req_valid_next,  1'b0)
  `RST_FF (clock, reset, tbox_min_rsp_valid, tbox_min_rsp_valid_next, 1'b0)

  assign tbox_neigh_req_push = tbox_neigh_req_valid & ~tbox_neigh_req_fifo_full;
  assign tbox_neigh_req_ready = ~tbox_neigh_req_fifo_full;

  gen_fifo #(
    .ELEM_SIZE   ( $bits(et_link_neigh_req_info_t) ),
    .NUM_ELEMS   ( `TBOX_NEIGH_REQ_FIFO_SIZE       ),
    .USE_LATCHES ( 0                               )
  ) tbox_neigh_req_fifo (
    // System signals
    .clock       ( clock                           ),
    .reset       ( reset                           ),
    // Push data
    .push_early  ( 1'b0                            ), // Only used when implemented with latches
    .push        ( tbox_neigh_req_push             ),
    .push_data   ( tbox_neigh_req_info             ),
    .full        ( tbox_neigh_req_fifo_full        ),
    .full_next   (                                 ),
    // Pop data
    .pop         ( tbox_neigh_req_pop              ),
    .pop_data    ( tbox_neigh_req_pop_data         ),
    .valid       ( tbox_neigh_req_pop_valid        ),
    .valid_next  (                                 )
  );

  // TBOX message destination is forced to minions
  assign tbox_neigh_req_goes_to_sc = tbox_neigh_req_pop_data.opcode != ET_LINK_REQ_MsgSendData;
  assign tbox_neigh_req_pop        = tbox_neigh_req_goes_to_sc ? tbox_sc_req_valid 
                                   & tbox_sc_req_ready : tbox_min_rsp_valid & tbox_min_rsp_ready;

  always_comb begin
    tbox_sc_req_valid_next = tbox_sc_req_valid;
    tbox_min_rsp_valid_next = tbox_min_rsp_valid;

    // Valid for requests going to Shire Cache
    if (tbox_sc_req_valid == 1'b0) begin
      if (tbox_neigh_req_pop_valid && tbox_neigh_req_goes_to_sc && !tbox_sc_req_disable_next)
      tbox_sc_req_valid_next = 1'b1;
    end
    else begin
      if (tbox_sc_req_ready || tbox_sc_req_disable_next)
      tbox_sc_req_valid_next = 1'b0;
    end

    // Valid for requests going to minions
    if (tbox_min_rsp_valid == 1'b0) begin
      if (tbox_neigh_req_pop_valid && !tbox_neigh_req_goes_to_sc)
      tbox_min_rsp_valid_next = 1'b1;
    end
    else begin
      if (tbox_min_rsp_ready)
      tbox_min_rsp_valid_next = 1'b0;
    end
  end

  // TBOX requests going to L2
  assign tbox_sc_req_info.id        = tbox_neigh_req_pop_data.id;
  assign tbox_sc_req_info.source    = tbox_neigh_req_pop_data.source;
  assign tbox_sc_req_info.wdata     = tbox_neigh_req_pop_data.wdata;
  assign tbox_sc_req_info.opcode    = tbox_neigh_req_pop_data.opcode;
  assign tbox_sc_req_info.subopcode = tbox_neigh_req_pop_data.subopcode;
  assign tbox_sc_req_info.address   = tbox_neigh_req_pop_data.address;
  assign tbox_sc_req_info.size      = tbox_neigh_req_pop_data.size;
  assign tbox_sc_req_info.qwen      = tbox_neigh_req_pop_data.qwen;

  // TBOX to minion message requests are converted into message replies
  assign tbox_min_rsp_info.id       = tbox_neigh_req_pop_data.address[`AD_ESR_MESSAGE_ID_RANGE];
  assign tbox_min_rsp_info.dest     = tbox_neigh_req_pop_data.address[`AD_ESR_NEIGH_HART_ID_RANGE];
  assign tbox_min_rsp_info.wdata    = 1'b1;
  assign tbox_min_rsp_info.opcode   = ET_LINK_RSP_MsgRcvData;
  assign tbox_min_rsp_info.data     = tbox_neigh_req_pop_data.data;
  assign tbox_min_rsp_info.size     = tbox_neigh_req_pop_data.size;
  assign tbox_min_rsp_info.qwen     = tbox_neigh_req_pop_data.qwen;

  // //////////////////////////////////////////////////////////////////////////////
  // TBOX L2 reply FIFO
  // //////////////////////////////////////////////////////////////////////////////
  logic              sc_tbox_rsp_push;
  logic              sc_tbox_rsp_fifo_full;
  et_link_rsp_info_t sc_tbox_rsp_pop_data;
  logic              sc_tbox_rsp_pop_valid;
  logic              sc_tbox_rsp_pop_valid_next;
  logic              sc_tbox_rsp_pop;

  assign sc_tbox_rsp_push = sc_tbox_rsp_valid && ~sc_tbox_rsp_fifo_full;
  assign sc_tbox_rsp_ready = ~sc_tbox_rsp_fifo_full;

  gen_fifo #(
    .ELEM_SIZE   ( $bits(et_link_rsp_info_t)    ),
    .NUM_ELEMS   ( `NEIGH_SC_TBOX_RSP_FIFO_SIZE ),
    .USE_LATCHES ( 0                            )
  ) sc_tbox_rsp_fifo (
    // System signals
    .clock       ( clock                        ),
    .reset       ( reset                        ),
    // Push data
    .push_early  ( 1'b0                         ), // Only used when implemented with latches
    .push        ( sc_tbox_rsp_push             ),
    .push_data   ( sc_tbox_rsp_info             ),
    .full        ( sc_tbox_rsp_fifo_full        ),
    .full_next   (                              ),
    // Pop data
    .pop         ( sc_tbox_rsp_pop              ),
    .pop_data    ( sc_tbox_rsp_pop_data         ),
    .valid       ( sc_tbox_rsp_pop_valid        ),
    .valid_next  ( sc_tbox_rsp_pop_valid_next   )
  );

  // //////////////////////////////////////////////////////////////////////////////
  // Responses to TBOX
  // //////////////////////////////////////////////////////////////////////////////

  logic              sc_tbox_rsp_grant;
  logic              min_tbox_rsp_grant;
  logic              tbox_neigh_rsp_push_early;
  logic              tbox_neigh_rsp_push;
  et_link_rsp_info_t tbox_neigh_rsp_push_data;
  logic              tbox_neigh_rsp_full;
  logic              tbox_neigh_rsp_pop;
  et_link_rsp_info_t tbox_neigh_rsp_pop_data;
  logic              tbox_neigh_rsp_pop_valid;
  logic              tbox_neigh_rsp_pop_ready;

  // Arbiter L2 responses and minion messages
  arb_rr_data #(
    .NUM_REQS ( 2                                          ),
    .WIDTH    ( $bits(et_link_rsp_info_t)                  )
  ) tbox_rsp_arb (
    // System signals
    .clock    ( clock                                      ),
    .reset    ( reset                                      ),
    // Bidding
    .reqs     ( {sc_tbox_rsp_pop_valid,min_tbox_rsp_valid} ),
    .pop      ( ~tbox_neigh_rsp_full                       ),
    // Data inputs
    .data_in  ( {sc_tbox_rsp_pop_data,min_tbox_rsp_info}   ),
    // Result
    .grants   ( {sc_tbox_rsp_grant,min_tbox_rsp_grant}     ),
    .data_out ( tbox_neigh_rsp_push_data                   )
  );

  assign tbox_neigh_rsp_push_early = sc_tbox_rsp_pop_valid_next | min_tbox_rsp_valid_next
      | (|{sc_tbox_rsp_pop_valid,min_tbox_rsp_valid} & tbox_neigh_rsp_full);

  assign tbox_neigh_rsp_push       = |{sc_tbox_rsp_pop_valid,min_tbox_rsp_valid} 
                                   & ~tbox_neigh_rsp_full;
  assign sc_tbox_rsp_pop           = sc_tbox_rsp_grant & ~tbox_neigh_rsp_full;
  assign min_tbox_rsp_ready        = min_tbox_rsp_grant & ~tbox_neigh_rsp_full;

  // Neighborhood TBOX reply FIFO
  gen_fifo #(
    .ELEM_SIZE   ( $bits(et_link_rsp_info_t)    ),
    .NUM_ELEMS   ( `NEIGH_SC_TBOX_RSP_FIFO_SIZE ),
    .USE_LATCHES ( 1                            )
  ) tbox_neigh_rsp_fifo (
    // System signals
    .clock       ( clock                        ),
    .reset       ( reset                        ),
    // Push data
    .push_early  ( tbox_neigh_rsp_push_early    ),
    .push        ( tbox_neigh_rsp_push          ),
    .push_data   ( tbox_neigh_rsp_push_data     ),
    .full        ( tbox_neigh_rsp_full          ),
    .full_next   (                              ),
    // Pop data
    .pop         ( tbox_neigh_rsp_pop           ),
    .pop_data    ( tbox_neigh_rsp_pop_data      ),
    .valid       ( tbox_neigh_rsp_pop_valid     ),
    .valid_next  (                              )
  );

  assign tbox_neigh_rsp_pop = tbox_neigh_rsp_pop_valid & tbox_neigh_rsp_pop_ready;

  // Flop FIFO output
  // CLK    RST    EN                        DOUT                  DIN                       DEF
  `EN_FF     (clock,        tbox_neigh_rsp_pop,       tbox_neigh_rsp_info,  tbox_neigh_rsp_pop_data)
  `RST_EN_FF (clock, reset, tbox_neigh_rsp_pop_ready, tbox_neigh_rsp_valid, tbox_neigh_rsp_pop_valid, 1'b0)

  assign tbox_neigh_rsp_pop_ready = tbox_neigh_rsp_ready | ~tbox_neigh_rsp_valid;

endmodule
