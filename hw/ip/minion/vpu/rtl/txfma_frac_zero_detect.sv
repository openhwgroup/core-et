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

module txfma_frac_zero_detect
#(
  parameter left_bit     = TXFMA_A3,
  parameter right_bit    = TXFMA_B28
)
(
  input  [left_bit:right_bit] in_data,
  output                      zero
);

assign zero = ~(|in_data);

endmodule
