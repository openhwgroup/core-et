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

module txfma_e4
(
  input                                    e_sticky1_f4a_h,
  input                                    e_sticky2_f4a_h,
  output                                   ph_comb_stiky_f4a_h
);

assign ph_comb_stiky_f4a_h = e_sticky1_f4a_h | e_sticky2_f4a_h;


endmodule
