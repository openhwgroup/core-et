// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_hv_logic_neigh_sc (
  input  logic                                  clock,
  input  logic                                  reset,
  // BANK FIFO outputs (512 bits)
  input et_link_req_info_t [`SC_BANKS-1:0]      miss_req_bank_info,
  input  logic [`SC_BANKS-1:0]                  miss_req_bank_valid,
  output logic [`SC_BANKS-1:0]                  miss_req_bank_ready,
  // UC FIFO output
  input et_link_nodata_req_info_t               uc_req_info_out,
  input  logic [`UC_DATA_SIZE-1:0]              uc_req_data_out,
  input  logic                                  uc_req_info_pop_valid,
  output logic                                  uc_req_info_pop,
  output logic                                  uc_req_data_pop,
  // Output to Shire Cache
  output et_link_req_info_t                     neigh_sc_req_info,
  output logic [(`SC_BANKS+1)-1:0]              neigh_sc_req_valid,
  input  logic [(`SC_BANKS+1)-1:0]              neigh_sc_req_ready
  );

  et_link_req_info_t  out_req_info;
  et_link_req_info_t  out_req_info_from_uc;
  logic [`SC_BANKS:0] out_req_bid;
  logic [`SC_BANKS:0] out_req_ready;
  logic [`SC_BANKS:0] out_req_valid;
  logic               out_req_fifo_empty;

  // //////////////////////////////////////////////////////////////////////////////
  // UC FIFO pop control
  // //////////////////////////////////////////////////////////////////////////////
  assign out_req_fifo_empty       = ~uc_req_info_pop_valid;

  assign out_req_valid[`SC_BANKS] = ~out_req_fifo_empty;
  assign uc_req_info_pop          = out_req_ready[`SC_BANKS];
  assign uc_req_data_pop          = uc_req_info_pop & uc_req_info_out.wdata;


  // //////////////////////////////////////////////////////////////////////////////
  // This is the miss request bank / UC FIFO multiplexer to the L2
  // //////////////////////////////////////////////////////////////////////////////

  // As it is a one-hot multiplexer, implement it as a and-or tree style mux
  always_comb begin
    out_req_info = '0;

    for (integer i = 0; i < `SC_BANKS; i = i+1)
    begin
      if (out_req_ready[i])
        out_req_info |= miss_req_bank_info[i];
    end

    // Select UC FIFO
    out_req_info_from_uc.id        = uc_req_info_out.id;
    out_req_info_from_uc.source    = uc_req_info_out.source;
    out_req_info_from_uc.wdata     = uc_req_info_out.wdata;
    out_req_info_from_uc.opcode    = uc_req_info_out.opcode;
    out_req_info_from_uc.subopcode = uc_req_info_out.subopcode;
    out_req_info_from_uc.address   = uc_req_info_out.address;
    // As transfer size is under 512 bits,
    // replicate data into high and low half word to have it properly aligned in any case
    out_req_info_from_uc.data      = {uc_req_data_out,uc_req_data_out};
    out_req_info_from_uc.size      = uc_req_info_out.size;
    out_req_info_from_uc.qwen      = uc_req_info_out.qwen;
    if (out_req_ready[`SC_BANKS])
      out_req_info |= out_req_info_from_uc;
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Output request arbiter to the L2
  // //////////////////////////////////////////////////////////////////////////////

  assign out_req_valid[`SC_BANKS-1:0] = miss_req_bank_valid;
  assign out_req_bid   = out_req_valid & neigh_sc_req_ready;

  arb_rr #(
    .NUM_REQS ( `SC_BANKS+1  )
  ) out_req_arbiter (
    .clock    ( clock          ),
    .reset    ( reset          ),
    .reqs     ( out_req_bid    ),
    .grants   ( out_req_ready  ),
    .pop      ( |out_req_ready )
  );

  assign miss_req_bank_ready = out_req_ready[`SC_BANKS-1:0];
  assign neigh_sc_req_info   = out_req_info;
  assign neigh_sc_req_valid  = out_req_ready;

endmodule
