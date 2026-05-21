// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_fl_barrier_tb
  import minion_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [MinPerN-1:0] flb_core_neigh_req_valid_i,
  input  logic [(MinPerN*CsrFlBarrierSize)-1:0] flb_core_neigh_req_data_bits_i,
  input  logic flb_l2_neigh_resp_valid_i,
  input  logic flb_l2_neigh_resp_data_i,

  output logic [MinPerN-1:0] orig_flb_neigh_core_resp_valid_o,
  output logic [MinPerN-1:0] new_flb_neigh_core_resp_valid_o,
  output logic orig_flb_neigh_core_resp_data_o,
  output logic new_flb_neigh_core_resp_data_o,
  output logic orig_flb_neigh_l2_req_valid_o,
  output logic new_flb_neigh_l2_req_valid_o,
  output logic [CsrFlBarrierSize-1:0] orig_flb_neigh_l2_req_data_o,
  output logic [CsrFlBarrierSize-1:0] new_flb_neigh_l2_req_data_o
);

  logic [MinPerN-1:0][CsrFlBarrierSize-1:0] req_data;

  always_comb begin
    for (int unsigned m = 0; m < MinPerN; m++) begin
      req_data[m] = flb_core_neigh_req_data_bits_i[m*CsrFlBarrierSize +: CsrFlBarrierSize];
    end
  end

  neigh_fl_barrier_orig u_orig (
    .clock                     (clk_i),
    .reset                     (!rst_ni),
    .flb_core_neigh_req_valid  (flb_core_neigh_req_valid_i),
    .flb_core_neigh_req_data   (req_data),
    .flb_neigh_core_resp_valid (orig_flb_neigh_core_resp_valid_o),
    .flb_neigh_core_resp_data  (orig_flb_neigh_core_resp_data_o),
    .flb_neigh_l2_req_valid    (orig_flb_neigh_l2_req_valid_o),
    .flb_neigh_l2_req_data     (orig_flb_neigh_l2_req_data_o),
    .flb_l2_neigh_resp_valid   (flb_l2_neigh_resp_valid_i),
    .flb_l2_neigh_resp_data    (flb_l2_neigh_resp_data_i)
  );

  neigh_fl_barrier u_new (
    .clk_i,
    .rst_ni,
    .flb_core_neigh_req_valid_i  (flb_core_neigh_req_valid_i),
    .flb_core_neigh_req_data_i   (req_data),
    .flb_neigh_core_resp_valid_o (new_flb_neigh_core_resp_valid_o),
    .flb_neigh_core_resp_data_o  (new_flb_neigh_core_resp_data_o),
    .flb_neigh_l2_req_valid_o    (new_flb_neigh_l2_req_valid_o),
    .flb_neigh_l2_req_data_o     (new_flb_neigh_l2_req_data_o),
    .flb_l2_neigh_resp_valid_i   (flb_l2_neigh_resp_valid_i),
    .flb_l2_neigh_resp_data_i    (flb_l2_neigh_resp_data_i)
  );

endmodule : cosim_neigh_fl_barrier_tb
