// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: vpu_decoder (new) vs vpu_decoder (original).
//
// Both modules are purely combinational. The testbench exposes both
// raw decode output vectors for bit-by-bit comparison in C++.

`include "soc.vh"

module cosim_vpu_decoder_tb #(
  parameter bit EnableExtraTrans = 1'b0
) (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [31:0] stim_i,

  output logic [44:0] new_sigs_o,
  output logic        new_vpu_o,
  output logic [44:0] orig_sigs_o,
  output logic        orig_vpu_o
);

  // ── New module (SystemVerilog, uses minion_frontend_pkg) ──
  import minion_frontend_pkg::*;

  vpu_ctrl_sigs_t new_sigs;
  logic           new_vpu;

  vpu_decoder #(
    .EnableExtraTrans(EnableExtraTrans)
  ) u_new (
    .id_inst     (stim_i),
    .id_sigs     (new_sigs),
    .id_vpu_insn (new_vpu)
  );

  assign new_sigs_o = $bits(vpu_ctrl_sigs_t)'(new_sigs);
  assign new_vpu_o  = new_vpu;

  // ── Original module (Verilog, uses `include "soc.vh") ──
  vpu_ctrl_sigs_t orig_sigs;
  logic           orig_vpu;

  vpu_decoder_orig u_orig (
    .id_inst     (stim_i),
    .id_sigs     (orig_sigs),
    .id_vpu_insn (orig_vpu)
  );

  assign orig_sigs_o = $bits(vpu_ctrl_sigs_t)'(orig_sigs);
  assign orig_vpu_o  = orig_vpu;

endmodule
