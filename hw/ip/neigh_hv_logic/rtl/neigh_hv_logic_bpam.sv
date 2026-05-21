// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_bpam #(
  parameter int unsigned BpamRcSize = neigh_hv_logic_pkg::BpamRcSize
) (
  input  logic                  rst_push_ni,
  input  logic                  clk_push_i,
  input  logic [BpamRcSize-1:0] bpam_run_control_i,
  input  logic                  dmctrl_write_enable_i,
  output logic                  bpam_run_control_vcfifo_push_o
);

  logic [BpamRcSize-1:0] bpam_run_control_q;

  assign bpam_run_control_vcfifo_push_o =
      (bpam_run_control_i != bpam_run_control_q) | dmctrl_write_enable_i;

  always_ff @(posedge clk_push_i or negedge rst_push_ni) begin
    if (!rst_push_ni) begin
      bpam_run_control_q <= '0;
    end else begin
      bpam_run_control_q <= bpam_run_control_i;
    end
  end

endmodule : neigh_hv_logic_bpam
