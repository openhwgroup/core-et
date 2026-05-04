// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_trans_top_tb
  #(
    parameter bit EnableExtraTrans = 1'b0
  )
(
  input  logic                                        clk_i,
  input  logic                                        rst_ni,
  input  logic                                        ex_valid_i,
  input  logic [6:0]                                  ex_cmd_i,
  input  logic [minion_pkg::VpuDataSSz-1:0]           ex_in2_i,
  input  logic [vpu_pkg::VpuFlagsSz-1:0]              ex_txfma_exc_i,
  output logic [$bits(vpu_pkg::trans_fma_coefficients_t)-1:0] orig_f8_rom_response_o,
  output logic [$bits(vpu_pkg::trans_fma_coefficients_t)-1:0] new_f8_rom_response_o,
  output logic                                        enable_extra_trans_o
);

  vpu_input_t                      orig_ex_in_bits;
  vpu_pkg::vpu_input_t             new_ex_in_bits;
  trans_fma_coefficients           orig_f8_rom_response;
  vpu_pkg::trans_fma_coefficients_t new_f8_rom_response;

  always_comb begin
    orig_ex_in_bits = '0;
    new_ex_in_bits = '0;
    orig_ex_in_bits.sigs.cmd = ex_cmd_i;
    orig_ex_in_bits.in2 = ex_in2_i;
    new_ex_in_bits.sigs.cmd = minion_pkg::vpu_cmd_e'(ex_cmd_i);
    new_ex_in_bits.in2 = ex_in2_i;
  end

  trans_top_orig u_orig (
    .clock           (clk_i),
    .reset           (~rst_ni),
    .ex_valid        (ex_valid_i),
    .ex_in_bits      (orig_ex_in_bits),
    .ex_txfma_exc    (ex_txfma_exc_i),
    .f8_rom_response (orig_f8_rom_response)
  );

  trans_top #(
    .EnableExtraTrans (EnableExtraTrans)
  ) u_new (
    .clk_i,
    .rst_ni,
    .ex_valid_i,
    .ex_in_bits_i      (new_ex_in_bits),
    .ex_txfma_exc_i,
    .f8_rom_response_o (new_f8_rom_response)
  );

  assign orig_f8_rom_response_o = orig_f8_rom_response;
  assign new_f8_rom_response_o = new_f8_rom_response;
  assign enable_extra_trans_o = EnableExtraTrans;

endmodule
