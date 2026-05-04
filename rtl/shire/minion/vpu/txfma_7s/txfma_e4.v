// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_e4 
(
  input                                    e_sticky1_f4a_h,
  input                                    e_sticky2_f4a_h,
  output                                   ph_comb_stiky_f4a_h
);

assign ph_comb_stiky_f4a_h = e_sticky1_f4a_h | e_sticky2_f4a_h;  


endmodule
