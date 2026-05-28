// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncacheable_flb_tb #(
  parameter int unsigned NumNeighP = shire_uncached_pkg::NumNeigh
) (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(shire_uncached_pkg::uc_esr_flb_enable_sigs_t)-1:0] esr_enables_bits_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] esr_wdata_i,
  input  logic [NumNeighP-1:0] flb_neigh_l2_req_valid_i,
  input  logic [NumNeighP*shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_bits_i,
  output logic [$bits(shire_uncached_pkg::uc_esr_barrier_values_t)-1:0] orig_esr_barrier_values_bits_o,
  output logic [$bits(shire_uncached_pkg::uc_esr_barrier_values_t)-1:0] new_esr_barrier_values_bits_o,
  output logic [NumNeighP-1:0] orig_flb_l2_neigh_resp_valid_o,
  output logic [NumNeighP-1:0] new_flb_l2_neigh_resp_valid_o,
  output logic [NumNeighP-1:0] orig_flb_l2_neigh_resp_data_o,
  output logic [NumNeighP-1:0] new_flb_l2_neigh_resp_data_o
);

  uc_esr_flb_enable_sigs orig_esr_enables;
  shire_uncached_pkg::uc_esr_flb_enable_sigs_t new_esr_enables;
  uc_esr_barrier_values_t orig_esr_barrier_values;
  shire_uncached_pkg::uc_esr_barrier_values_t new_esr_barrier_values;
  logic [NumNeighP-1:0][`CSR_FL_BARRIER_RANGE] orig_flb_neigh_l2_req_data;
  logic [NumNeighP-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] new_flb_neigh_l2_req_data;

  assign orig_esr_enables = esr_enables_bits_i[$bits(uc_esr_flb_enable_sigs)-1:0];
  assign new_esr_enables = esr_enables_bits_i;
  assign orig_flb_neigh_l2_req_data = flb_neigh_l2_req_data_bits_i;
  assign new_flb_neigh_l2_req_data = flb_neigh_l2_req_data_bits_i;
  assign orig_esr_barrier_values_bits_o = orig_esr_barrier_values;
  assign new_esr_barrier_values_bits_o = new_esr_barrier_values;

  uncacheable_flb_orig #(.NUM_NEIGH(NumNeighP)) u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .esr_enables(orig_esr_enables),
    .esr_wdata(esr_wdata_i),
    .esr_barrier_values(orig_esr_barrier_values),
    .flb_neigh_l2_req_valid(flb_neigh_l2_req_valid_i),
    .flb_neigh_l2_req_data(orig_flb_neigh_l2_req_data),
    .flb_l2_neigh_resp_valid(orig_flb_l2_neigh_resp_valid_o),
    .flb_l2_neigh_resp_data(orig_flb_l2_neigh_resp_data_o)
  );

  uncacheable_flb #(.NumNeighP(NumNeighP)) u_new (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .esr_enables_i(new_esr_enables),
    .esr_wdata_i(esr_wdata_i),
    .esr_barrier_values_o(new_esr_barrier_values),
    .flb_neigh_l2_req_valid_i(flb_neigh_l2_req_valid_i),
    .flb_neigh_l2_req_data_i(new_flb_neigh_l2_req_data),
    .flb_l2_neigh_resp_valid_o(new_flb_l2_neigh_resp_valid_o),
    .flb_l2_neigh_resp_data_o(new_flb_l2_neigh_resp_data_o)
  );

endmodule : cosim_uncacheable_flb_tb
