// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_apb_rsp_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        apb_esr_rsp_vcfifo_valid_i,
  input  logic [63:0] apb_esr_rsp_pop_prdata_i,
  input  logic        apb_esr_rsp_pop_pready_i,
  input  logic        apb_esr_rsp_pop_pslverr_i,
  output logic [63:0] orig_apb_esr_rsp_sync_prdata_o,
  output logic [63:0] new_apb_esr_rsp_sync_prdata_o,
  output logic        orig_apb_esr_rsp_sync_pready_o,
  output logic        new_apb_esr_rsp_sync_pready_o,
  output logic        orig_apb_esr_rsp_sync_pslverr_o,
  output logic        new_apb_esr_rsp_sync_pslverr_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Dummy clock/reset ports keep the common C++ cosim harness shape for this combinational leaf.
  logic unused_common_ports;
  assign unused_common_ports = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

  logic [65:0] orig_pop_bits;
  logic [65:0] orig_sync_bits;
  neigh_hv_logic_pkg::apb_from_neigh_t new_pop;
  neigh_hv_logic_pkg::apb_from_neigh_t new_sync;

  assign orig_pop_bits = {apb_esr_rsp_pop_prdata_i,
                          apb_esr_rsp_pop_pready_i,
                          apb_esr_rsp_pop_pslverr_i};
  assign new_pop.apb_prdata = apb_esr_rsp_pop_prdata_i;
  assign new_pop.apb_pready = apb_esr_rsp_pop_pready_i;
  assign new_pop.apb_pslverr = apb_esr_rsp_pop_pslverr_i;

  assign orig_apb_esr_rsp_sync_prdata_o = orig_sync_bits[65:2];
  assign orig_apb_esr_rsp_sync_pready_o = orig_sync_bits[1];
  assign orig_apb_esr_rsp_sync_pslverr_o = orig_sync_bits[0];
  assign new_apb_esr_rsp_sync_prdata_o = new_sync.apb_prdata;
  assign new_apb_esr_rsp_sync_pready_o = new_sync.apb_pready;
  assign new_apb_esr_rsp_sync_pslverr_o = new_sync.apb_pslverr;

  neigh_hv_logic_apb_rsp u_new (
    .apb_esr_rsp_vcfifo_valid_i (apb_esr_rsp_vcfifo_valid_i),
    .apb_esr_rsp_pop_i          (new_pop),
    .apb_esr_rsp_sync_o         (new_sync)
  );

  neigh_hv_logic_apb_rsp_orig u_orig (
    .apb_esr_rsp_vcfifo_valid (apb_esr_rsp_vcfifo_valid_i),
    .apb_esr_rsp_pop          (orig_pop_bits),
    .apb_esr_rsp_sync         (orig_sync_bits)
  );

endmodule : cosim_neigh_hv_logic_apb_rsp_tb
