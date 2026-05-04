// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vim: se syn=systemverilog:

`include "soc.vh"

module icache_pma_unit (
  // Request to the PMA
  input  logic[`PA_RANGE]   paddr,
  input  esr_mprot_t        mprot,
  // PTW info
  input  [`CSR_PRV_SZ-1:0]  ptw_prv,
  // Response
  output icache_pma_resp    resp_data
  );

// See "minion_types.vh" for the address map
logic icache_req_mram_mbox_permitted;
logic icache_req_mram_sbox_permitted;

always_comb begin

    icache_req_mram_mbox_permitted = (ptw_prv == `CSR_PRV_M);
    icache_req_mram_sbox_permitted = (ptw_prv == `CSR_PRV_M) || (ptw_prv == `CSR_PRV_S);

    resp_data.cacheable = paddr_is_mram_space(paddr)
                       || paddr_is_sram_space(paddr)
                       || paddr_is_bootrom_space(paddr);

    resp_data.access_fault =
        !( (paddr_is_bootrom_space(paddr)                                       ) ||
           (paddr_is_mram_mbox(paddr, mprot) && icache_req_mram_mbox_permitted  ) ||
           (paddr_is_mram_sbox(paddr, mprot) && icache_req_mram_sbox_permitted  ) ||
           (paddr_is_mram_other(paddr, mprot)                                   ) ||
           (paddr_is_sram_space(paddr)                                          ) );
end

endmodule
