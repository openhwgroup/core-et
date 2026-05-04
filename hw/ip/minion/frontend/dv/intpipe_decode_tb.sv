// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for the standard sim_ctrl harness; intpipe_decode is combinational.
module intpipe_decode_tb
  import minion_pkg::*;
#(
  parameter bit EnableExtraTrans = 1'b0
) (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [31:0] inst_i,
  output logic        enable_extra_trans_o,
  output logic        legal_o,
  output logic        mcode_o,
  output logic        fp_o
);
/* verilator lint_on UNUSED */

  /* verilator lint_off UNUSEDSIGNAL */  // The TB intentionally exposes only the legal/mcode/fp seam for focused decoder checks.
  minion_control_t inst_ctrl;
  /* verilator lint_on UNUSEDSIGNAL */

  intpipe_decode #(
    .EnableExtraTrans(EnableExtraTrans)
  ) u_dut (
    .inst_bits(inst_i),
    .inst_ctrl(inst_ctrl)
  );

  assign enable_extra_trans_o = EnableExtraTrans;
  assign legal_o              = inst_ctrl.legal;
  assign mcode_o              = inst_ctrl.mcode;
  assign fp_o                 = inst_ctrl.fp;

endmodule
