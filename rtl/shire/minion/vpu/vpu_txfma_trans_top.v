// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_txfma_trans_top (
  // System signals
  input  logic             clock_txfma,
  input  logic             clock_trans_rom,
  input  logic             reset,
  input  logic             ex_rom_valid,
  input  vpu_input_t       ex_in_bits,
  input  logic             ex_txfma_valid,
  output vpu_output_t      f8_txfma_res,
  output logic             f8_txfma_comp_res,
  output logic [1:0]       txfma_trans_dbg
);


trans_fma_coefficients f8_trans_rom_response;


////////////////////////////////////////////////////////////////////////////////
// Trans unit
// Contains the ROMs for trans instructions
////////////////////////////////////////////////////////////////////////////////

trans_top trans (
  .clock           ( clock_trans_rom       ),
  .reset           ( reset                 ),
  .ex_valid        ( ex_rom_valid          ),
  .ex_txfma_exc    ( f8_txfma_res.exc      ),
  .ex_in_bits      ( ex_in_bits            ),
  .f8_rom_response ( f8_trans_rom_response )
);



///////////////////////////////////////////////////////////////////////////////
// TXFMA
///////////////////////////////////////////////////////////////////////////////

`ifdef USE_FAKE_TXFMA
txfma_top_fake
`else
txfma_top 
`endif
txfma (
  .clock                 ( clock_txfma           ),
  .reset                 ( reset                 ),
  .in_valid              ( ex_txfma_valid        ),
  .in_data               ( ex_in_bits            ),
  .trans_coefficients    ( f8_trans_rom_response ),
  .out_data_res          ( f8_txfma_res          ),
  .out_comp_res          ( f8_txfma_comp_res     )
);


//debug signals
always_comb begin
  txfma_trans_dbg = {f8_trans_rom_response.exc,f8_trans_rom_response.taylor};
end

endmodule
