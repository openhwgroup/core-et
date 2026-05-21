// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_bpam_rc_tbox_ack_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic bpam_rc_tbox_ack_halt_i,
  input  logic bpam_rc_tbox_ack_resume_i,
  output logic orig_bpam_rc_tbox_ack_halt_o,
  output logic new_bpam_rc_tbox_ack_halt_o,
  output logic orig_bpam_rc_tbox_ack_resume_o,
  output logic new_bpam_rc_tbox_ack_resume_o
);

  logic [1:0] orig_ack_in;
  logic [1:0] orig_ack_out;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_ack_in;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_ack_out;

  assign orig_ack_in = {bpam_rc_tbox_ack_halt_i, bpam_rc_tbox_ack_resume_i};
  assign new_ack_in.halt_ack = bpam_rc_tbox_ack_halt_i;
  assign new_ack_in.resume_ack = bpam_rc_tbox_ack_resume_i;
  assign orig_bpam_rc_tbox_ack_halt_o = orig_ack_out[1];
  assign orig_bpam_rc_tbox_ack_resume_o = orig_ack_out[0];
  assign new_bpam_rc_tbox_ack_halt_o = new_ack_out.halt_ack;
  assign new_bpam_rc_tbox_ack_resume_o = new_ack_out.resume_ack;

  neigh_hv_logic_bpam_rc_tbox_ack u_new (
    .clk_hv_i                       (clk_i),
    .rst_hv_ni                      (rst_ni),
    .bpam_rc_tbox_ack_hi_reg_i      (new_ack_in),
    .bpam_rc_tbox_ack_hi_reg_o      (new_ack_out)
  );

  neigh_hv_logic_bpam_rc_tbox_ack_orig u_orig (
    .clock_hv                       (clk_i),
    .reset_hv                       (!rst_ni),
    .bpam_rc_tbox_ack_hi_reg_in     (orig_ack_in),
    .bpam_rc_tbox_ack_hi_reg_out    (orig_ack_out)
  );

endmodule : cosim_neigh_hv_logic_bpam_rc_tbox_ack_tb
