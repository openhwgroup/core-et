// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_hv_logic_l2_bank (
  input  logic                        clock,
  input  logic                        reset,
  // BANK FIFO output (256 bits)
  input  logic                        bank_req_out_is_first_half,
  input  et_link_nodata_req_info_t    bank_req_info_out,
  input  logic [`NEIGH_DATA_RANGE]    bank_req_data_out,
  input  logic                        bank_req_info_valid,
  output logic                        bank_req_info_pop,
  output logic                        bank_req_data_pop,
  // BANK FIFO output (512 bits)
  output et_link_req_info_t           miss_req_bank_info,
  output logic                        miss_req_bank_valid,
  input  logic                        miss_req_bank_ready
);

  logic                          bank_req_first_half_grant;
  logic                          bank_req_is_second_half;
  logic                          bank_req_is_second_half_next;
  logic [`NEIGH_DATA_RANGE]      bank_evict_req_first_half;
  
  // Bank FIFO pop control
  ////////////////////////////////////////////////////////////////////////////////
  assign bank_req_first_half_grant = bank_req_info_valid & bank_req_out_is_first_half;
  assign bank_req_info_pop         = bank_req_info_valid & (bank_req_first_half_grant | miss_req_bank_ready);
  assign bank_req_data_pop         = bank_req_info_pop & bank_req_info_out.wdata;

  ////////////////////////////////////////////////////////////////////////////////
  // FF to store first half of 512-bit evict request
  ////////////////////////////////////////////////////////////////////////////////

  //          CLK    RST    EN                                                    DOUT                       DIN                           DEF
  `RST_EN_FF (clock, reset, bank_req_is_second_half^bank_req_is_second_half_next, bank_req_is_second_half,   bank_req_is_second_half_next, 1'b0) 
  `EN_FF     (clock,        bank_req_first_half_grant,                            bank_evict_req_first_half, bank_req_data_out)

  // If evict requests are to write 512 bits, the first half is read from the FIFO and stored here
  // Then, it is combined with the second half and sent to the L2
  always_comb begin
    bank_req_is_second_half_next = bank_req_is_second_half;

    if (bank_req_first_half_grant)
      bank_req_is_second_half_next = 1'b1;
    else if (bank_req_info_pop && bank_req_is_second_half)
      bank_req_is_second_half_next = 1'b0;
  end

  assign miss_req_bank_info.id        = bank_req_info_out.id;
  assign miss_req_bank_info.source    = bank_req_info_out.source;
  assign miss_req_bank_info.wdata     = bank_req_info_out.wdata;
  assign miss_req_bank_info.opcode    = bank_req_info_out.opcode;
  assign miss_req_bank_info.subopcode = bank_req_info_out.subopcode;
  assign miss_req_bank_info.address   = bank_req_info_out.address;
  assign miss_req_bank_info.data      = bank_req_is_second_half ? {bank_req_data_out,bank_evict_req_first_half}
                                      // If transfer size is under 512 bits,
                                      // replicate data into high and low half
                                      // word to have it properly aligned in any case
                                      : {bank_req_data_out,bank_req_data_out};
  assign miss_req_bank_info.size      = bank_req_info_out.size;
  assign miss_req_bank_info.qwen      = bank_req_info_out.qwen;

  // Valid to L2 request
  assign miss_req_bank_valid          = bank_req_info_valid & ~bank_req_out_is_first_half;

endmodule
