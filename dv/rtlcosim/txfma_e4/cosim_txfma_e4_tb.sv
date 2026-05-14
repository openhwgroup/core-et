// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_e4_tb (
  input  logic e_sticky1_i,
  input  logic e_sticky2_i,
  output logic orig_ph_comb_stiky_o,
  output logic new_ph_comb_stiky_o
);

  txfma_e4_orig u_orig (
    .e_sticky1_f4a_h    (e_sticky1_i),
    .e_sticky2_f4a_h    (e_sticky2_i),
    .ph_comb_stiky_f4a_h(orig_ph_comb_stiky_o)
  );

  txfma_e4 u_new (
    .e_sticky1_f4a_h    (e_sticky1_i),
    .e_sticky2_f4a_h    (e_sticky2_i),
    .ph_comb_stiky_f4a_h(new_ph_comb_stiky_o)
  );

endmodule : cosim_txfma_e4_tb
