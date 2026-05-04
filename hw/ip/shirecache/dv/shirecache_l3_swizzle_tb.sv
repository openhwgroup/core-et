// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for L3 swizzle get+put round-trip testing.

module shirecache_l3_swizzle_tb
  import shirecache_pkg::*;
(
  // Swizzle config (shared)
  input  l3_swizzle_ctl_t              swizzle_ctl_i,

  // Put inputs
  input  logic [SubBankIdSize-1:0]     put_sub_bank_id_i,
  input  logic [BankIdSize-1:0]        put_bank_id_i,
  input  logic [L3ShireIdSize-1:0]     put_shire_id_i,
  output logic [PaSize-1:0]            put_address_o,

  // Get inputs
  input  logic [PaSize-1:0]            get_address_i,
  output logic [SubBankIdSize-1:0]     get_sub_bank_id_o,
  output logic [BankIdSize-1:0]        get_bank_id_o,
  output logic [L3ShireIdSize-1:0]     get_shire_id_o
);

  shirecache_pipe_l3_swizzle_put u_put (
    .sub_bank_id_i  (put_sub_bank_id_i),
    .bank_id_i      (put_bank_id_i),
    .shire_id_i     (put_shire_id_i),
    .swizzle_ctl_i,
    .address_put_o  (put_address_o)
  );

  shirecache_pipe_l3_swizzle_get u_get (
    .address_i      (get_address_i),
    .swizzle_ctl_i,
    .sub_bank_id_o  (get_sub_bank_id_o),
    .bank_id_o      (get_bank_id_o),
    .shire_id_o     (get_shire_id_o)
  );

endmodule
