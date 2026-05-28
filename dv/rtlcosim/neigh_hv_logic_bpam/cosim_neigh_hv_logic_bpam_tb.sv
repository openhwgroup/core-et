// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_bpam_tb (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic [3:0] bpam_run_control_i,
  input  logic       dmctrl_write_enable_i,
  output logic       orig_bpam_run_control_vcfifo_push_o,
  output logic       new_bpam_run_control_vcfifo_push_o
);

  neigh_hv_logic_bpam u_new (
    .rst_push_ni                     (rst_ni),
    .clk_push_i                      (clk_i),
    .bpam_run_control_i              (bpam_run_control_i),
    .dmctrl_write_enable_i           (dmctrl_write_enable_i),
    .bpam_run_control_vcfifo_push_o  (new_bpam_run_control_vcfifo_push_o)
  );

  neigh_hv_logic_bpam_orig u_orig (
    .reset_push                       (!rst_ni),
    .clock_push                       (clk_i),
    .bpam_run_control                 (bpam_run_control_i),
    .dmctrl_write_enable              (dmctrl_write_enable_i),
    .bpam_run_control_vcfifo_push     (orig_bpam_run_control_vcfifo_push_o)
  );

endmodule : cosim_neigh_hv_logic_bpam_tb
