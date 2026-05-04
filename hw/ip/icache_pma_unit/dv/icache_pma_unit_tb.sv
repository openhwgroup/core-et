// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for sim_ctrl.h, but the DUT is combinational
module icache_pma_unit_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import icache_pkg::*;
(
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       ioshire_i,
  input  logic [PaSize-1:0] paddr_i,
  input  logic [$bits(esr_mprot_t)-1:0] mprot_i,
  input  logic [1:0] ptw_prv_i,
  output logic       access_fault_o,
  output logic       cacheable_o
);

  icache_pma_resp_t resp_data;

  icache_pma_unit u_dut (
    .ioshire_i,
    .paddr_i,
    .mprot_i     (esr_mprot_t'(mprot_i)),
    .ptw_prv_i,
    .resp_data_o (resp_data)
  );

  assign access_fault_o = resp_data.access_fault;
  assign cacheable_o = resp_data.cacheable;

endmodule : icache_pma_unit_tb
/* verilator lint_on UNUSED */
