// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_top_tb
  import minion_pkg::*;
  import vpu_pkg::*;
  #(
    parameter bit EnableExtraTrans = 1'b0
  )
(
  input  logic                  clk_i,
  input  logic                  rst_ni,
  input  logic                  ex_valid_i,
  input  logic [6:0]            ex_cmd_i,
  input  logic [VpuDataSSz-1:0] ex_in2_i,
  input  logic [VpuFlagsSz-1:0] ex_txfma_exc_i,
  output logic [57:0]           f8_rom_response_o,
  output logic                  enable_extra_trans_o
);

  vpu_input_t              ex_in_bits;
  trans_fma_coefficients_t f8_rom_response;

  always_comb begin
    ex_in_bits = '0;
    ex_in_bits.sigs.cmd = vpu_cmd_e'(ex_cmd_i);
    ex_in_bits.in2 = ex_in2_i;
  end

  trans_top #(
    .EnableExtraTrans (EnableExtraTrans)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .ex_valid_i,
    .ex_in_bits_i      (ex_in_bits),
    .ex_txfma_exc_i,
    .f8_rom_response_o (f8_rom_response)
  );

  assign f8_rom_response_o = f8_rom_response;
  assign enable_extra_trans_o = EnableExtraTrans;

endmodule : trans_top_tb
