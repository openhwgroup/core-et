// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// L3 address swizzle — put (insert) fields.
//
// Based on ESR swizzle configuration, inserts sub_bank, bank, and
// shire_id fields into an L3 physical address. Output can be OR'd
// with a partial address to form the complete L3 eviction address.
// Pure combinational.

/* verilator lint_off UNUSEDSIGNAL */  // swizzle_ctl_t aliasing fields unused; only sel bits drive the put logic
module shirecache_pipe_l3_swizzle_put
  import shirecache_pkg::*;
(
  input  logic [SubBankIdSize-1:0]   sub_bank_id_i,
  input  logic [BankIdSize-1:0]      bank_id_i,
  input  logic [L3ShireIdSize-1:0]   shire_id_i,
  input  l3_swizzle_ctl_t            swizzle_ctl_i,

  output logic [PaSize-1:0]          address_put_o
);

  // swizzle_ctl_t aliasing fields unused here — only sel bits matter for put
  /* verilator lint_off UNUSEDSIGNAL */  // *_max wires zero-extended from narrower IDs; upper bits intentionally unused

  logic [SubBankIdSizeMax-1:0] sub_bank_max;
  logic [BankIdSizeMax-1:0]    bank_max;
  logic [L3ShireIdSizeMax-1:0] shire_max;

  assign sub_bank_max = SubBankIdSizeMax'(sub_bank_id_i);
  assign bank_max     = BankIdSizeMax'(bank_id_i);
  assign shire_max    = L3ShireIdSizeMax'(shire_id_i);

  logic [L3SwizzleSelSize-1:0] rel_addr;

  always_comb begin
    rel_addr = '0;
    rel_addr |= (L3SwizzleSelSize'(sub_bank_max[2]) << swizzle_ctl_i.sub_bank_sel_b2);
    rel_addr |= (L3SwizzleSelSize'(sub_bank_max[1]) << swizzle_ctl_i.sub_bank_sel_b1);
    rel_addr |= (L3SwizzleSelSize'(sub_bank_max[0]) << swizzle_ctl_i.sub_bank_sel_b0);
    rel_addr |= (L3SwizzleSelSize'(bank_max[2])     << swizzle_ctl_i.bank_sel_b2);
    rel_addr |= (L3SwizzleSelSize'(bank_max[1])     << swizzle_ctl_i.bank_sel_b1);
    rel_addr |= (L3SwizzleSelSize'(bank_max[0])     << swizzle_ctl_i.bank_sel_b0);
    rel_addr |= (L3SwizzleSelSize'(shire_max[5])    << swizzle_ctl_i.shire_sel_b5);
    rel_addr |= (L3SwizzleSelSize'(shire_max[4])    << swizzle_ctl_i.shire_sel_b4);
    rel_addr |= (L3SwizzleSelSize'(shire_max[3])    << swizzle_ctl_i.shire_sel_b3);
    rel_addr |= (L3SwizzleSelSize'(shire_max[2])    << swizzle_ctl_i.shire_sel_b2);
    rel_addr |= (L3SwizzleSelSize'(shire_max[1])    << swizzle_ctl_i.shire_sel_b1);
    rel_addr |= (L3SwizzleSelSize'(shire_max[0])    << swizzle_ctl_i.shire_sel_b0);
  end

  assign address_put_o = PaSize'({rel_addr, {LineOffsetIdSize{1'b0}}});

endmodule
/* verilator lint_on UNUSEDSIGNAL */
