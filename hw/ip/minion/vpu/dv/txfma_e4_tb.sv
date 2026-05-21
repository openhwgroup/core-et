// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_e4_tb (
  input  logic e_sticky1_i,
  input  logic e_sticky2_i,
  output logic ph_comb_stiky_o
);

  txfma_e4 u_dut (
    .e_sticky1_f4a_h    (e_sticky1_i),
    .e_sticky2_f4a_h    (e_sticky2_i),
    .ph_comb_stiky_f4a_h(ph_comb_stiky_o)
  );

endmodule : txfma_e4_tb
