// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_vpu_uinst_decoder_tb
  import minion_pkg::*;
  #(
    parameter bit EnableExtraTrans = 1'b0
  )
(
  input  logic                              clk_i,
  input  logic                              rst_ni,
  input  logic [31:0]                    id_inst_i,
  output logic [$bits(vpu_ctrl_sigs_t)-1:0] orig_id_sigs_o,
  output logic [$bits(vpu_ctrl_sigs_t)-1:0] new_id_sigs_o
);

  /* verilator lint_off UNUSED */  // CosimCtrl ticks a dummy clock/reset even though this decoder wrapper is combinational.
  logic unused_ok;
  assign unused_ok = &{1'b0, clk_i, rst_ni};
  /* verilator lint_on UNUSED */

  vpu_ctrl_sigs_t orig_id_sigs;
  vpu_ctrl_sigs_t new_id_sigs;

  vpu_uinst_decoder_orig u_orig (
    .id_inst (id_inst_i),
    .id_sigs (orig_id_sigs)
  );

  vpu_uinst_decoder #(
    .EnableExtraTrans (EnableExtraTrans)
  ) u_new (
    .id_inst_i,
    .id_sigs_o (new_id_sigs)
  );

  assign orig_id_sigs_o = orig_id_sigs;
  assign new_id_sigs_o = new_id_sigs;

endmodule
