// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_booth_ppg_32r4_msb_tb
  import minion_pkg::*;
(
  input  logic [VpuDataSSz-1:0] mulcand_i,
  input  logic                  msb_i,
  input  logic                  sign_i,
  output logic [VpuDataSSz:0]   pp_o
);

  txfma_booth_ppg_32r4_msb u_dut (
    .mulcand_i,
    .msb_i,
    .sign_i,
    .pp_o
  );

endmodule : txfma_booth_ppg_32r4_msb_tb
