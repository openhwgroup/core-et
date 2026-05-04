// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// L3 address swizzle — get (extract) fields.
//
// Based on ESR swizzle configuration, extracts the sub_bank, bank,
// and shire_id fields from an L3 physical address. Pure combinational.

/* verilator lint_off UNUSEDSIGNAL */  // full PaSize address and swizzle_ctl_t passed in; only selected bit-slices used
module shirecache_pipe_l3_swizzle_get
  import shirecache_pkg::*;
(
  input  logic [PaSize-1:0]          address_i,
  input  l3_swizzle_ctl_t            swizzle_ctl_i,

  output logic [SubBankIdSize-1:0]   sub_bank_id_o,
  output logic [BankIdSize-1:0]      bank_id_o,
  output logic [L3ShireIdSize-1:0]   shire_id_o
);

  logic [SubBankIdSizeMax-1:0] sub_bank_max;
  logic [BankIdSizeMax-1:0]    bank_max;
  logic [L3ShireIdSizeMax-1:0] shire_max;

  logic [L3SwizzleSelSize-1:0] rel_addr;
  assign rel_addr = address_i[LineOffsetIdSize +: L3SwizzleSelSize];

  always_comb begin
    sub_bank_max[2] = rel_addr[swizzle_ctl_i.sub_bank_sel_b2];
    sub_bank_max[1] = rel_addr[swizzle_ctl_i.sub_bank_sel_b1];
    sub_bank_max[0] = rel_addr[swizzle_ctl_i.sub_bank_sel_b0];
    bank_max[2]     = rel_addr[swizzle_ctl_i.bank_sel_b2];
    bank_max[1]     = rel_addr[swizzle_ctl_i.bank_sel_b1];
    bank_max[0]     = rel_addr[swizzle_ctl_i.bank_sel_b0];
    shire_max[5]    = rel_addr[swizzle_ctl_i.shire_sel_b5];
    shire_max[4]    = rel_addr[swizzle_ctl_i.shire_sel_b4];
    shire_max[3]    = rel_addr[swizzle_ctl_i.shire_sel_b3];
    shire_max[2]    = rel_addr[swizzle_ctl_i.shire_sel_b2];
    shire_max[1]    = rel_addr[swizzle_ctl_i.shire_sel_b1];
    shire_max[0]    = rel_addr[swizzle_ctl_i.shire_sel_b0];
  end

  assign sub_bank_id_o = sub_bank_max[SubBankIdSize-1:0];
  assign bank_id_o     = bank_max[BankIdSize-1:0];
  assign shire_id_o    = shire_max[L3ShireIdSize-1:0];

endmodule
/* verilator lint_on UNUSEDSIGNAL */
