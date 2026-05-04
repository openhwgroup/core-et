// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_pma_unit
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import icache_pkg::*;
(
  input  logic             ioshire_i,
  input  logic [PaSize-1:0] paddr_i,
  /* verilator lint_off UNUSEDSIGNAL */  // Only the memory-protection bits that affect I-cache fetch permission are consumed here; the remaining ESR fields are preserved in the shared struct contract.
  input  esr_mprot_t       mprot_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic [1:0]       ptw_prv_i,
  output icache_pma_resp_t resp_data_o
);

  logic icache_req_dram_mcode_permitted;
  logic icache_req_dram_mdata_permitted;
  logic icache_req_dram_scode_permitted;
  logic icache_req_dram_sdata_permitted;
  logic icache_req_dram_osbox_permitted;
  logic icache_req_dram_other_permitted;
  logic icache_req_sp_sram_code_permitted;
  logic icache_req_sp_sram_data_permitted;

  always_comb begin
    icache_req_dram_mcode_permitted = !ioshire_i && (ptw_prv_i == PrvM);
    icache_req_dram_mdata_permitted = !ioshire_i && !mprot_i.enable_secure_memory && (ptw_prv_i == PrvM);
    icache_req_dram_scode_permitted = mprot_i.enable_secure_memory
                                    ? (!ioshire_i && (ptw_prv_i == PrvS))
                                    : (!ioshire_i && !mprot_i.disable_osbox_access);
    icache_req_dram_sdata_permitted = !ioshire_i && !mprot_i.enable_secure_memory && !mprot_i.disable_osbox_access;
    icache_req_dram_osbox_permitted = !ioshire_i && !mprot_i.disable_osbox_access &&
                                      (!mprot_i.enable_secure_memory || (ptw_prv_i == PrvU));
    icache_req_dram_other_permitted = !ioshire_i && paddr_is_dram_cacheable(paddr_i) &&
                                      (!mprot_i.enable_secure_memory || (ptw_prv_i == PrvU));
    icache_req_sp_sram_code_permitted = ioshire_i && (ptw_prv_i != PrvU);
    icache_req_sp_sram_data_permitted = ioshire_i && (ptw_prv_i == PrvM);

    resp_data_o.cacheable = paddr_is_dram(paddr_i) || (ioshire_i && paddr_is_sp_cacheable(paddr_i));
    resp_data_o.access_fault =
        !((paddr_is_sp_rom(paddr_i)                     && ioshire_i) ||
          (paddr_is_sp_sram_code(paddr_i)              && icache_req_sp_sram_code_permitted) ||
          (paddr_is_sp_sram_data(paddr_i)              && icache_req_sp_sram_data_permitted) ||
          (paddr_is_dram_mcode(paddr_i)                && icache_req_dram_mcode_permitted) ||
          (paddr_is_dram_mdata(paddr_i)                && icache_req_dram_mdata_permitted) ||
          (paddr_is_dram_scode(paddr_i)                && icache_req_dram_scode_permitted) ||
          (paddr_is_dram_sdata(paddr_i)                && icache_req_dram_sdata_permitted) ||
          (paddr_is_dram_osbox(paddr_i)                && icache_req_dram_osbox_permitted) ||
          (paddr_is_dram_other(paddr_i, mprot_i.dram_size) && icache_req_dram_other_permitted));
  end

endmodule : icache_pma_unit
