// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache PMA unit.
//
// Reimplements `dcache_pma_unit` from the original CORE-ET minion.

/* verilator lint_off UNUSED */  // vm_status_t carries sum/mxr/debug for shared minion compatibility; PMA only uses prv/mprv/mpp
module minion_dcache_pma_unit
  import minion_dcache_pkg::*;
(
  input  logic            ioshire_i,
  input  logic            af_skip_i,
  input  dcache_pma_req_t req_data_i,
  input  esr_mprot_t      mprot_i,
  input  logic            valid_addr_i,
  input  vm_status_t      vm_status_i,
  output dcache_pma_resp_t resp_data_o
);

  logic [1:0] privilege_mode;
  logic       dcache_req_io_permitted;
  logic       dcache_req_sp_permitted;
  logic       dcache_req_scp_permitted;
  logic       dcache_req_esr_permitted;
  logic       dcache_req_pcie_permitted;
  logic       dcache_req_mcode_permitted;
  logic       dcache_req_mdata_permitted;
  logic       dcache_req_scode_permitted;
  logic       dcache_req_sdata_permitted;
  logic       dcache_req_osbox_permitted;
  logic       dcache_req_sp_sram_code_permitted;
  logic       dcache_req_sp_sram_data_permitted;

  always_comb begin
    privilege_mode = vm_status_i.mprv ? vm_status_i.mpp : vm_status_i.prv;

    dcache_req_io_permitted = !req_data_i.amo
                           && !req_data_i.ts_tl_co
                           && (mprot_i.io_access_mode <= privilege_mode)
                           && (mprot_i.io_access_mode != MprotIoAccessModeF);

    dcache_req_sp_sram_code_permitted = req_data_i.write
                                      ? (privilege_mode == PrvM)
                                      : (privilege_mode != PrvU);

    dcache_req_sp_sram_data_permitted = privilege_mode != PrvU;

    dcache_req_sp_permitted = !req_data_i.amo
                           && (!req_data_i.ts_tl_co || paddr_is_sp_cacheable(req_data_i.paddr))
                           && (dcache_req_sp_sram_code_permitted || !paddr_is_sp_sram_code(req_data_i.paddr))
                           && (dcache_req_sp_sram_data_permitted || !paddr_is_sp_sram_data(req_data_i.paddr))
                           && ioshire_i;

    dcache_req_scp_permitted = !req_data_i.amo || req_data_i.mem_global;

    dcache_req_esr_permitted = !req_data_i.amo
                            && !req_data_i.ts_tl_co
                            && (req_data_i.typ == DcTypeD)
                            && (req_data_i.paddr[2:0] == 3'b000)
                            && (req_data_i.paddr[31:30] <= privilege_mode)
                            && (ioshire_i || (req_data_i.paddr[31:30] != 2'b10));

    dcache_req_pcie_permitted = !req_data_i.amo
                             && !req_data_i.ts_tl_co
                             && (ioshire_i || !mprot_i.disable_pcie_access);

    dcache_req_mcode_permitted = ioshire_i
                              || ((privilege_mode == PrvM) && !(req_data_i.write && mprot_i.enable_secure_memory));

    dcache_req_mdata_permitted = ioshire_i || (privilege_mode == PrvM);

    dcache_req_scode_permitted = mprot_i.enable_secure_memory
                               ? (ioshire_i || (privilege_mode == PrvM)
                                 || ((privilege_mode == PrvS) && !req_data_i.write))
                               : (ioshire_i || !mprot_i.disable_osbox_access);

    dcache_req_sdata_permitted = mprot_i.enable_secure_memory
                               ? (ioshire_i || (privilege_mode != PrvU))
                               : (ioshire_i || !mprot_i.disable_osbox_access);

    dcache_req_osbox_permitted = ioshire_i || !mprot_i.disable_osbox_access;

    resp_data_o.cacheable = !req_data_i.amo
                         && (paddr_is_dram_cacheable(req_data_i.paddr)
                          || paddr_is_scratchpad(req_data_i.paddr)
                          || paddr_is_sp_cacheable(req_data_i.paddr));

    resp_data_o.access_fault = valid_addr_i
                            && !af_skip_i
                            && ((paddr_is_io_space(req_data_i.paddr)         && !dcache_req_io_permitted)
                             || (paddr_is_sp_space(req_data_i.paddr)         && !dcache_req_sp_permitted)
                             || (paddr_is_scratchpad(req_data_i.paddr)       && !dcache_req_scp_permitted)
                             || (paddr_is_esr_space(req_data_i.paddr)        && !dcache_req_esr_permitted)
                             ||  paddr_is_reserved(req_data_i.paddr)
                             || (paddr_is_pcie_space(req_data_i.paddr)       && !dcache_req_pcie_permitted)
                             || (paddr_is_dram_mcode(req_data_i.paddr)       && !dcache_req_mcode_permitted)
                             || (paddr_is_dram_mdata(req_data_i.paddr)       && !dcache_req_mdata_permitted)
                             || (paddr_is_dram_scode(req_data_i.paddr)       && !dcache_req_scode_permitted)
                             || (paddr_is_dram_sdata(req_data_i.paddr)       && !dcache_req_sdata_permitted)
                             || (paddr_is_dram_osbox(req_data_i.paddr)       && !dcache_req_osbox_permitted)
                             || (paddr_is_dram_uncacheable(req_data_i.paddr) && !ioshire_i)
                             ||  paddr_is_dram_reserved(req_data_i.paddr, mprot_i.dram_size));
  end

endmodule
