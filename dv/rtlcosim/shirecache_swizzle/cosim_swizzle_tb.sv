// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original l3_swizzle_get/put vs new shirecache_pipe_l3_swizzle_get/put.

`include "soc.vh"

module cosim_swizzle_tb (
  // Shared inputs for GET
  input  logic [`PA_SIZE-1:0]              get_address_i,
  input  esr_sc_l3_shire_swizzle_ctl_t     swizzle_ctl_i,

  // GET outputs
  output logic [`SC_SUB_BANK_ID_SIZE-1:0]  new_get_sub_bank_o,
  output logic [`SC_BANK_ID_SIZE-1:0]      new_get_bank_o,
  output logic [`SC_L3_SHIRE_ID_SIZE-1:0]  new_get_shire_o,
  output logic [`SC_SUB_BANK_ID_SIZE-1:0]  orig_get_sub_bank_o,
  output logic [`SC_BANK_ID_SIZE-1:0]      orig_get_bank_o,
  output logic [`SC_L3_SHIRE_ID_SIZE-1:0]  orig_get_shire_o,

  // Shared inputs for PUT
  input  logic [`SC_SUB_BANK_ID_SIZE-1:0]  put_sub_bank_i,
  input  logic [`SC_BANK_ID_SIZE-1:0]      put_bank_i,
  input  logic [`SC_L3_SHIRE_ID_SIZE-1:0]  put_shire_i,

  // PUT outputs
  output logic [`PA_SIZE-1:0]              new_put_address_o,
  output logic [`PA_SIZE-1:0]              orig_put_address_o
);

  // ── New GET ───────────────────────────────────────────

  shirecache_pipe_l3_swizzle_get u_new_get (
    .address_i      (get_address_i),
    .swizzle_ctl_i  (shirecache_pkg::l3_swizzle_ctl_t'(swizzle_ctl_i)),
    .sub_bank_id_o  (new_get_sub_bank_o),
    .bank_id_o      (new_get_bank_o),
    .shire_id_o     (new_get_shire_o)
  );

  // ── Original GET ──────────────────────────────────────

  shire_cache_pipe_l3_swizzle_get u_orig_get (
    .address                    (get_address_i),
    .esr_sc_l3_shire_swizzle_ctl(swizzle_ctl_i),
    .sub_bank_id                (orig_get_sub_bank_o),
    .bank_id                    (orig_get_bank_o),
    .shire_id                   (orig_get_shire_o)
  );

  // ── New PUT ───────────────────────────────────────────

  shirecache_pipe_l3_swizzle_put u_new_put (
    .sub_bank_id_i  (put_sub_bank_i),
    .bank_id_i      (put_bank_i),
    .shire_id_i     (put_shire_i),
    .swizzle_ctl_i  (shirecache_pkg::l3_swizzle_ctl_t'(swizzle_ctl_i)),
    .address_put_o  (new_put_address_o)
  );

  // ── Original PUT ──────────────────────────────────────

  shire_cache_pipe_l3_swizzle_put u_orig_put (
    .sub_bank_id                (put_sub_bank_i),
    .bank_id                    (put_bank_i),
    .shire_id                   (put_shire_i),
    .esr_sc_l3_shire_swizzle_ctl(swizzle_ctl_i),
    .address_put                (orig_put_address_o)
  );

endmodule
