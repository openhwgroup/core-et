// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_bpam_run_control #(
  parameter int unsigned NumNeigh = shire_channel_leaves_pkg::NumNeigh
) (
  input  shire_channel_leaves_pkg::bpam_run_control_t         bpam_run_control_i,
  input  logic [NumNeigh-1:0]                                 tbox_single_step_i,
  input  logic                                                rbox_single_step_i,

  output neigh_voltage_cross_pkg::bpam_run_control_neigh_t    bpam_run_control_neigh_o [NumNeigh-1:0],
  output rbox_pkg::rc_t                                       bpam_run_control_rbox_o
);

  always_comb begin
    for (int unsigned neigh_idx = 0; neigh_idx < NumNeigh; neigh_idx++) begin
      bpam_run_control_neigh_o[neigh_idx].tbox_single_step = tbox_single_step_i[neigh_idx];

      bpam_run_control_neigh_o[neigh_idx].gpio.ndmreset =
          bpam_run_control_i.gpio.ndmreset;
      bpam_run_control_neigh_o[neigh_idx].gpio.halt_req =
          bpam_run_control_i.gpio.halt_req;
      bpam_run_control_neigh_o[neigh_idx].gpio.resume_req =
          bpam_run_control_i.gpio.resume_req;
      bpam_run_control_neigh_o[neigh_idx].gpio.tbox_resume_req =
          bpam_run_control_i.gpio.tbox_resume_req;
      bpam_run_control_neigh_o[neigh_idx].gpio.hart_reset_req =
          bpam_run_control_i.gpio.hart_reset_req;
      bpam_run_control_neigh_o[neigh_idx].gpio.tbox_halt_req =
          bpam_run_control_i.gpio.tbox_halt_req;

      bpam_run_control_neigh_o[neigh_idx].events.halt_req =
          bpam_run_control_i.events.halt_req;
      bpam_run_control_neigh_o[neigh_idx].events.resume_req =
          bpam_run_control_i.events.resume_req;
      bpam_run_control_neigh_o[neigh_idx].events.tbox_halt_req =
          bpam_run_control_i.events.tbox_halt_req;
      bpam_run_control_neigh_o[neigh_idx].events.tbox_resume_req =
          bpam_run_control_i.events.tbox_resume_req;
    end

    bpam_run_control_rbox_o.gpio.halt_req = bpam_run_control_i.gpio.rbox_halt_req;
    bpam_run_control_rbox_o.gpio.resume_req = bpam_run_control_i.gpio.rbox_resume_req;
    bpam_run_control_rbox_o.gpio.reset_req = bpam_run_control_i.gpio.rbox_reset_req;

    bpam_run_control_rbox_o.events.halt_req = bpam_run_control_i.events.rbox_halt_req;
    bpam_run_control_rbox_o.events.resume_req = bpam_run_control_i.events.rbox_resume_req;
    bpam_run_control_rbox_o.single_step = rbox_single_step_i;
  end

endmodule
