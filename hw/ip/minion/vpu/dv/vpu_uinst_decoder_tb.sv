// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_uinst_decoder_tb
  import minion_pkg::*;
  #(
    parameter bit EnableExtraTrans = 1'b0
  )
(
  input  logic [31:0]                    id_inst_i,
  output logic [$bits(vpu_ctrl_sigs_t)-1:0] id_sigs_o,
  output logic                            enable_extra_trans_o
);

  vpu_ctrl_sigs_t id_sigs;

  vpu_uinst_decoder #(
    .EnableExtraTrans (EnableExtraTrans)
  ) u_dut (
    .id_inst_i,
    .id_sigs_o (id_sigs)
  );

  assign id_sigs_o = id_sigs;
  assign enable_extra_trans_o = EnableExtraTrans;

endmodule
