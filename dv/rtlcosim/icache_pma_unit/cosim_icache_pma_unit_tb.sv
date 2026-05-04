// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_icache_pma_unit_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import icache_pkg::*;
(
  input  logic                clk_i,
  input  logic                rst_ni,
  input  logic                ioshire_i,
  input  logic [PaSize-1:0]   paddr_i,
  input  logic [$bits(esr_mprot_t)-1:0] mprot_i,
  input  logic [1:0]            ptw_prv_i,
  output logic                new_access_fault_o,
  output logic                new_cacheable_o,
  output logic                orig_access_fault_o,
  output logic                orig_cacheable_o
);

  icache_pma_resp_t new_resp_data;
  icache_pma_resp   orig_resp_data;

  icache_pma_unit u_new (
    .ioshire_i,
    .paddr_i,
    .mprot_i     (esr_mprot_t'(mprot_i)),
    .ptw_prv_i,
    .resp_data_o (new_resp_data)
  );

  icache_pma_unit_orig u_orig (
    .ioshire   (ioshire_i),
    .paddr     (paddr_i),
    .mprot     (esr_mprot_t'(mprot_i)),
    .ptw_prv   (ptw_prv_i),
    .resp_data (orig_resp_data)
  );

  assign new_access_fault_o = new_resp_data.access_fault;
  assign new_cacheable_o = new_resp_data.cacheable;
  assign orig_access_fault_o = orig_resp_data.access_fault;
  assign orig_cacheable_o = orig_resp_data.cacheable;

endmodule : cosim_icache_pma_unit_tb
