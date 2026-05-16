// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_apb_req_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic apb_pready_neigh_i,
  input  logic apb_esr_req_apb_psel_i,
  input  logic apb_esr_req_apb_penable_i,
  output logic orig_apb_esr_req_vcfifo_push_o,
  output logic new_apb_esr_req_vcfifo_push_o
);

  neigh_hv_logic_apb_req u_new (
    .rst_push_ni                (rst_ni),
    .clk_push_i                 (clk_i),
    .apb_pready_neigh_i         (apb_pready_neigh_i),
    .apb_esr_req_apb_psel_i     (apb_esr_req_apb_psel_i),
    .apb_esr_req_apb_penable_i  (apb_esr_req_apb_penable_i),
    .apb_esr_req_vcfifo_push_o  (new_apb_esr_req_vcfifo_push_o)
  );

  neigh_hv_logic_apb_req_orig u_orig (
    .reset_push                 (!rst_ni),
    .clock_push                 (clk_i),
    .apb_pready_neigh           (apb_pready_neigh_i),
    .apb_esr_req_apb_psel       (apb_esr_req_apb_psel_i),
    .apb_esr_req_apb_penable    (apb_esr_req_apb_penable_i),
    .apb_esr_req_vcfifo_push    (orig_apb_esr_req_vcfifo_push_o)
  );

endmodule : cosim_neigh_hv_logic_apb_req_tb
