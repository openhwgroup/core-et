// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_shire_bpam_run_control_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [14:0] bpam_bits_i,
  input  logic [3:0]  tbox_single_step_i,
  input  logic        rbox_single_step_i,
  output logic [43:0] orig_neigh_bits_o,
  output logic [43:0] new_neigh_bits_o,
  output logic [5:0]  orig_rbox_bits_o,
  output logic [5:0]  new_rbox_bits_o
);
  localparam int unsigned NumNeigh = 4;
  localparam int unsigned NeighWidth = $bits(bpam_run_control_neigh_t);

  bpam_run_control_t orig_bpam;
  bpam_run_control_neigh_t [NumNeigh-1:0] orig_neigh;
  bpam_rc_rbox_t orig_rbox;

  shire_channel_leaves_pkg::bpam_run_control_t new_bpam;
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t new_neigh [NumNeigh-1:0];
  rbox_pkg::rc_t new_rbox;

  always_comb begin
    orig_bpam = '0;
    orig_bpam.gpio.ndmreset = bpam_bits_i[14];
    orig_bpam.gpio.halt_req = bpam_bits_i[13];
    orig_bpam.gpio.resume_req = bpam_bits_i[12];
    orig_bpam.gpio.hartReset_req = bpam_bits_i[11];
    orig_bpam.gpio.tbox_halt_req = bpam_bits_i[10];
    orig_bpam.gpio.tbox_resume_req = bpam_bits_i[9];
    orig_bpam.gpio.rbox_halt_req = bpam_bits_i[8];
    orig_bpam.gpio.rbox_resume_req = bpam_bits_i[7];
    orig_bpam.gpio.rbox_reset_req = bpam_bits_i[6];
    orig_bpam.events.halt_req = bpam_bits_i[5];
    orig_bpam.events.resume_req = bpam_bits_i[4];
    orig_bpam.events.tbox_halt_req = bpam_bits_i[3];
    orig_bpam.events.tbox_resume_req = bpam_bits_i[2];
    orig_bpam.events.rbox_halt_req = bpam_bits_i[1];
    orig_bpam.events.rbox_resume_req = bpam_bits_i[0];

    new_bpam = bpam_bits_i;
    for (int unsigned n = 0; n < NumNeigh; n++) begin
      orig_neigh_bits_o[(n*NeighWidth) +: NeighWidth] = orig_neigh[n];
      new_neigh_bits_o[(n*NeighWidth) +: NeighWidth] = new_neigh[n];
    end
    orig_rbox_bits_o = orig_rbox;
    new_rbox_bits_o = new_rbox;
  end

  shire_bpam_run_control_orig u_orig (
    .bpam_run_control_ip       (orig_bpam),
    .tbox_single_step_ip       (tbox_single_step_i),
    .rbox_single_step_ip       (rbox_single_step_i),
    .bpam_run_control_neigh_op (orig_neigh),
    .bpam_run_control_rbox_op  (orig_rbox)
  );

  shire_bpam_run_control u_new (
    .bpam_run_control_i       (new_bpam),
    .tbox_single_step_i       (tbox_single_step_i),
    .rbox_single_step_i       (rbox_single_step_i),
    .bpam_run_control_neigh_o (new_neigh),
    .bpam_run_control_rbox_o  (new_rbox)
  );

  logic unused_clk_rst;
  assign unused_clk_rst = ^{clk_i, rst_ni};
endmodule
