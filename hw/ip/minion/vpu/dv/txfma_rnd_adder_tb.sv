// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

module txfma_rnd_adder_tb
#(
  parameter int left_bit   = 32,
  parameter int right_bit  = 0,
  parameter int rnd_ctl_sz = 5,
  parameter int rnd_ps     = 1
) (
  input  logic [left_bit:right_bit]         in,
  input  logic [rnd_ctl_sz-1:0]             rnd_ctl,
  input  logic                              cin,
  output logic [left_bit-1:right_bit]       out
);

  txfma_rnd_adder #(
    .left_bit  (left_bit),
    .right_bit (right_bit),
    .rnd_ctl_sz(rnd_ctl_sz),
    .rnd_ps    (rnd_ps)
  ) u_dut (
    .in,
    .rnd_ctl,
    .cin,
    .out
  );

endmodule : txfma_rnd_adder_tb
