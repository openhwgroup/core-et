// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import minion_pkg::*;
import vpu_pkg::*;
/* verilator lint_on IMPORTSTAR */

module vpu_txfma_trans_top
#(
    parameter bit UseFakeTxfma = UseFakeTxfmaDefault
)
(
  // System signals
  input  logic             clock_txfma,
  input  logic             clock_trans_rom,
  input  logic             reset,
  input  logic             ex_rom_valid,
  input  vpu_pkg::vpu_input_t  ex_in_bits,
  input  logic             ex_txfma_valid,
  output vpu_pkg::vpu_output_t f8_txfma_res,
  output logic             f8_txfma_comp_res,
  output logic [1:0]       txfma_trans_dbg
 );

vpu_pkg::trans_fma_coefficients f8_trans_rom_response;


////////////////////////////////////////////////////////////////////////////////
// Trans unit
// Contains the ROMs for trans instructions
////////////////////////////////////////////////////////////////////////////////

trans_top
trans
(
  .clk_i             ( clock_trans_rom       ),
  .rst_ni            ( ~reset                ),
  .ex_valid_i        ( ex_rom_valid          ),
  .ex_txfma_exc_i    ( f8_txfma_res.exc      ),
  .ex_in_bits_i      ( ex_in_bits            ),
  .f8_rom_response_o ( f8_trans_rom_response )
);



///////////////////////////////////////////////////////////////////////////////
// TXFMA
///////////////////////////////////////////////////////////////////////////////

generate
if (UseFakeTxfma) begin : gen_fake_txfma
  txfma_top_fake txfma (
    .clock              ( clock_txfma           ),
    .reset              ( reset                 ),
    .in_valid           ( ex_txfma_valid        ),
    .in_data            ( ex_in_bits            ),
    .trans_coefficients ( f8_trans_rom_response ),
    .out_data_res       ( f8_txfma_res          ),
    .out_comp_res       ( f8_txfma_comp_res     )
  );
end else begin : gen_real_txfma
  txfma_top txfma (
    .clock              ( clock_txfma           ),
    .reset              ( reset                 ),
    .in_valid           ( ex_txfma_valid        ),
    .in_data            ( ex_in_bits            ),
    .trans_coefficients ( f8_trans_rom_response ),
    .out_data_res       ( f8_txfma_res          ),
    .out_comp_res       ( f8_txfma_comp_res     )
  );
end
endgenerate


//debug signals
always_comb begin
  txfma_trans_dbg = {f8_trans_rom_response.exc,f8_trans_rom_response.taylor};
end

endmodule
