// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_pma_unit vs new minion_dcache_pma_unit.

`include "soc.vh"

/* verilator lint_off UNUSED */  // Cosim keeps the standard clk/reset harness ports even for this combinational leaf.
module cosim_minion_dcache_pma_unit_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        ioshire_i,
  input  logic        af_skip_i,
  input  logic        valid_addr_i,
  input  logic [39:0] paddr_i,
  input  logic [3:0]  typ_i,
  input  logic        amo_i,
  input  logic        write_i,
  input  logic        mem_global_i,
  input  logic        ts_tl_co_i,
  input  logic [$bits(minion_dcache_pkg::esr_mprot_t)-1:0] mprot_i,
  input  logic [$bits(minion_dcache_pkg::vm_status_t)-1:0] vm_status_i,
  output logic        new_access_fault_o,
  output logic        new_cacheable_o,
  output logic        orig_access_fault_o,
  output logic        orig_cacheable_o
);
/* verilator lint_on UNUSED */

  minion_dcache_pkg::dcache_pma_req_t  new_req_data;
  minion_dcache_pkg::dcache_pma_resp_t new_resp_data;
  dcache_pma_req                       orig_req_data;
  dcache_pma_resp                      orig_resp_data;

  always_comb begin
    new_req_data = '0;
    new_req_data.paddr = paddr_i;
    new_req_data.typ = minion_dcache_pkg::dcache_type_e'(typ_i);
    new_req_data.amo = amo_i;
    new_req_data.write = write_i;
    new_req_data.mem_global = mem_global_i;
    new_req_data.ts_tl_co = ts_tl_co_i;

    orig_req_data = '0;
    orig_req_data.paddr = paddr_i;
    orig_req_data.typ = dcache_type'(typ_i);
    orig_req_data.amo = amo_i;
    orig_req_data.write = write_i;
    orig_req_data.mem_global = mem_global_i;
    orig_req_data.ts_tl_co = ts_tl_co_i;
  end

  minion_dcache_pma_unit u_new (
    .ioshire_i,
    .af_skip_i,
    .req_data_i   (new_req_data),
    .mprot_i      (minion_dcache_pkg::esr_mprot_t'(mprot_i)),
    .valid_addr_i,
    .vm_status_i  (minion_dcache_pkg::vm_status_t'(vm_status_i)),
    .resp_data_o  (new_resp_data)
  );

  dcache_pma_unit u_orig (
    .ioshire    (ioshire_i),
    .af_skip    (af_skip_i),
    .req_data   (orig_req_data),
    .mprot      (esr_mprot_t'(mprot_i)),
    .valid_addr (valid_addr_i),
    .vm_status  (minion_vm_status'(vm_status_i)),
    .resp_data  (orig_resp_data)
  );

  assign new_access_fault_o = new_resp_data.access_fault;
  assign new_cacheable_o = new_resp_data.cacheable;
  assign orig_access_fault_o = orig_resp_data.access_fault;
  assign orig_cacheable_o = orig_resp_data.cacheable;

endmodule
