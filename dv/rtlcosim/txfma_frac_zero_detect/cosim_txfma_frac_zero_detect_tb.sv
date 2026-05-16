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

module cosim_txfma_frac_zero_detect_tb
#(
  parameter int left_bit  = TXFMA_A3,
  parameter int right_bit = TXFMA_B28
) (
  input  logic [left_bit:right_bit] in_data,
  output logic                      orig_zero,
  output logic                      new_zero
);

  txfma_frac_zero_detect_orig #(
    .left_bit (left_bit),
    .right_bit(right_bit)
  ) u_orig (
    .in_data(in_data),
    .zero(orig_zero)
  );

  txfma_frac_zero_detect #(
    .left_bit (left_bit),
    .right_bit(right_bit)
  ) u_new (
    .in_data(in_data),
    .zero(new_zero)
  );

endmodule : cosim_txfma_frac_zero_detect_tb
