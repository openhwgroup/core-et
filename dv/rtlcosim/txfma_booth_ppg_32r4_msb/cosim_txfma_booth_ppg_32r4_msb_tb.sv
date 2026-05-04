// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_booth_ppg_32r4_msb_tb
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0] mulcand_i,
  input  logic                  msb_i,
  input  logic                  sign_i,
  output logic [VpuDataSSz:0]   orig_pp_o,
  output logic [VpuDataSSz:0]   new_pp_o
);

  txfma_booth_ppg_32r4_msb_orig u_orig (
    .mulcand (mulcand_i),
    .msb     (msb_i),
    .sign    (sign_i),
    .pp      (orig_pp_o)
  );

  txfma_booth_ppg_32r4_msb u_new (
    .mulcand_i,
    .msb_i,
    .sign_i,
    .pp_o (new_pp_o)
  );

endmodule : cosim_txfma_booth_ppg_32r4_msb_tb
