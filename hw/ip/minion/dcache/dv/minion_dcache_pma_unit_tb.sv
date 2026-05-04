// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for sim_ctrl.h, but the DUT is combinational
module minion_dcache_pma_unit_tb (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       ioshire_i,
  input  logic       af_skip_i,
  input  logic       valid_addr_i,
  input  logic [39:0] paddr_i,
  input  logic [3:0] typ_i,
  input  logic       amo_i,
  input  logic       write_i,
  input  logic       mem_global_i,
  input  logic       ts_tl_co_i,
  input  logic [$bits(minion_dcache_pkg::esr_mprot_t)-1:0] mprot_i,
  input  logic [$bits(minion_dcache_pkg::vm_status_t)-1:0] vm_status_i,
  output logic       access_fault_o,
  output logic       cacheable_o
);

  minion_dcache_pkg::dcache_pma_req_t  req_data;
  minion_dcache_pkg::dcache_pma_resp_t resp_data;

  always_comb begin
    req_data = '0;
    req_data.paddr = paddr_i;
    req_data.typ = minion_dcache_pkg::dcache_type_e'(typ_i);
    req_data.amo = amo_i;
    req_data.write = write_i;
    req_data.mem_global = mem_global_i;
    req_data.ts_tl_co = ts_tl_co_i;
  end

  minion_dcache_pma_unit u_dut (
    .ioshire_i,
    .af_skip_i,
    .req_data_i   (req_data),
    .mprot_i      (minion_dcache_pkg::esr_mprot_t'(mprot_i)),
    .valid_addr_i,
    .vm_status_i  (minion_dcache_pkg::vm_status_t'(vm_status_i)),
    .resp_data_o  (resp_data)
  );

  assign access_fault_o = resp_data.access_fault;
  assign cacheable_o = resp_data.cacheable;

endmodule
