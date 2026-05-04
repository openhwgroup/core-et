// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vim: se syn=systemverilog:

`include "soc.vh"

module dcache_pma_unit
(
    // Do not generate access fault
    input logic       af_skip,
    // Request to the PMA
    input             dcache_pma_req req_data,
    input esr_mprot_t mprot,
    input logic       valid_addr,
    input prv_t       prv,
    // Response
    output            dcache_pma_resp resp_data
);

  // See "minion_types.vh" for the address map

logic [`CSR_PRV_SZ-1:0] privilege_mode;
logic                   dcache_req_systemreg_permitted;
logic                   dcache_req_mramreg_permitted;
logic                   dcache_req_io_permitted;
logic                   dcache_req_xspi_permitted;
logic                   dcache_req_bootrom_permitted;
logic                   dcache_req_sram_permitted;
logic                   dcache_req_mram_mbox_permitted;
logic                   dcache_req_mram_sbox_permitted;
logic                   dcache_req_otp_permitted;
logic                   dcache_req_esr_permitted;
logic                   dcache_req_plic_permitted;
logic                   dcache_req_nic_permitted;

  always_comb begin
    // Current privilege
    privilege_mode = prv;

    dcache_req_systemreg_permitted = !req_data.amo
                            && !req_data.ts_tl_co
                            && (`CSR_PRV_S <= privilege_mode)
                            && (req_data.typ == dcache_type_W)
                            && (req_data.paddr[2:0] == 0);

    dcache_req_mramreg_permitted = !req_data.amo
                            && !req_data.ts_tl_co
                            && (`CSR_PRV_M <= privilege_mode)
                            && (req_data.typ == dcache_type_D)
                            && (req_data.paddr[2:0] == 0);

    dcache_req_io_permitted = !req_data.amo
                            && !req_data.ts_tl_co
                            && (`CSR_PRV_S <= privilege_mode)
                            && (req_data.typ == dcache_type_W)
                            && (req_data.paddr[2:0] == 0);

    dcache_req_xspi_permitted = !req_data.amo
                            && !req_data.ts_tl_co
                            && (`CSR_PRV_M <= privilege_mode)
                            && (req_data.typ == dcache_type_D)
                            && (req_data.paddr[2:0] == 0);

    dcache_req_nic_permitted = !req_data.amo
                            && !req_data.ts_tl_co
                            && (`CSR_PRV_M <= privilege_mode)
                            && (req_data.typ == dcache_type_W)
                            && (req_data.paddr[1:0] == 0);

    dcache_req_bootrom_permitted = !req_data.amo
                              && !req_data.ts_tl_co
                              && !req_data.write;

    dcache_req_sram_permitted = !req_data.amo;

    dcache_req_otp_permitted = !req_data.amo
                              && !req_data.ts_tl_co
                              && !req_data.write;

    dcache_req_mram_mbox_permitted  = (privilege_mode == `CSR_PRV_M);
    dcache_req_mram_sbox_permitted  = (privilege_mode >= `CSR_PRV_S);

    // The ESR region only permits load/store of size 64b, aligned at 64bit.
    //
    // The DRAM region permits local and global atomics.
    // Other regions do not permit atomics.

    dcache_req_esr_permitted = !req_data.amo
                            && !req_data.ts_tl_co
                            && (req_data.typ == dcache_type_D)
                            && (req_data.paddr[2:0] == 0)
                            && (req_data.paddr[`AD_ESR_PROT_BITS_RANGE] <= privilege_mode)
                            && (req_data.paddr[`AD_ESR_PROT_BITS_RANGE] != 2'b10);

    dcache_req_plic_permitted = !req_data.amo
                            && !req_data.ts_tl_co
                            && (req_data.typ == dcache_type_W)
                            && (req_data.paddr[1:0] == 0)
                            && (`CSR_PRV_S <= privilege_mode);

    resp_data.cacheable = !req_data.amo && 
                          (paddr_is_mram_space(req_data.paddr)      ||
                           paddr_is_sram_space(req_data.paddr)      ||
                           paddr_is_bootrom_space(req_data.paddr)   );
    

    resp_data.access_fault =
        valid_addr &&
        !af_skip &&
        !( (paddr_is_systemreg_space(req_data.paddr)        && dcache_req_systemreg_permitted   ) ||
           (paddr_is_mramreg_space(req_data.paddr)          && dcache_req_mramreg_permitted     ) ||
           (paddr_is_io_space_space(req_data.paddr)         && dcache_req_io_permitted          ) ||
           (paddr_is_xspi_space(req_data.paddr)             && dcache_req_xspi_permitted        ) ||
           (paddr_is_nic_space(req_data.paddr)              && dcache_req_nic_permitted         ) ||
           (paddr_is_bootrom_space(req_data.paddr)          && dcache_req_bootrom_permitted     ) ||
           (paddr_is_sram_space(req_data.paddr)             && dcache_req_sram_permitted        ) ||
           (paddr_is_mram_mbox(req_data.paddr, mprot)       && dcache_req_mram_mbox_permitted   ) ||
           (paddr_is_mram_sbox(req_data.paddr, mprot)       && dcache_req_mram_sbox_permitted   ) ||
           (paddr_is_mram_other(req_data.paddr, mprot)                                          ) ||
           (paddr_is_otp_space(req_data.paddr)              && dcache_req_otp_permitted         ) ||
           (paddr_is_esr_space(req_data.paddr)              && dcache_req_esr_permitted         ) ||
           (paddr_is_plic_space(req_data.paddr)             && dcache_req_plic_permitted        ) );
end

endmodule
