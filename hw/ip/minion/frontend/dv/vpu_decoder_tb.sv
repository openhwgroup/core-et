// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for the standard sim_ctrl harness; vpu_decoder is combinational.
module vpu_decoder_tb
  import minion_pkg::*;
#(
  parameter bit EnableExtraTrans = 1'b0
) (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [31:0] inst_i,
  output logic        enable_extra_trans_o,
  output logic        is_vpu_o,
  output logic [6:0]  cmd_o
);
/* verilator lint_on UNUSED */

  /* verilator lint_off UNUSEDSIGNAL */  // The TB intentionally exposes only cmd/id_vpu_insn for the focused decode contract.
  vpu_ctrl_sigs_t id_sigs;
  /* verilator lint_on UNUSEDSIGNAL */
  logic           id_vpu_insn;

  vpu_decoder #(
    .EnableExtraTrans(EnableExtraTrans)
  ) u_dut (
    .id_inst(inst_i),
    .id_sigs(id_sigs),
    .id_vpu_insn(id_vpu_insn)
  );

  assign enable_extra_trans_o = EnableExtraTrans;
  assign is_vpu_o             = id_vpu_insn;
  assign cmd_o                = id_sigs.cmd;

endmodule
