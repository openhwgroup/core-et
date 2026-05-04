<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_data_ram_wrap

`icache_data_ram_wrap` is the standalone-Minion I-cache data SRAM wrapper from the
original CORE-ET shell. It arbitrates three access classes onto the I-cache data
RAM panels:

- mission-mode I-cache read/write traffic
- APB debug read/modify/write access
- per-panel I-cache MBIST access

It also converts incoming L2 fill data into ECC-protected SRAM words and returns
read or write data back to the I-cache frontend-facing SRAM interface.

## Parameters

- `Id`
  Neighborhood index used to accept only the matching MBIST address slice.
- `StandaloneMinion`
  Preserved for original interface parity. The translated RAM path does not branch
  on it because the ASIC SRAM selection is abstracted behind `prim_ram_1p`.

## Interface

### System / DFT

- `clk_i`, `rst_ni`
  Functional clock and active-low async reset.
- `dft_sram_clk_i`, `dft_i`
  SRAM DFT clock override and inhibit controls.
- `ram_cfg_i`
  Standardized SRAM timing/test configuration.

### I-cache SRAM side

- `icache_req_write_i`
  Read/write select for the SRAM-side request.
- `icache_req_addr_i`
  SRAM word address plus row select.
- `icache_req_valid_i`, `icache_req_ready_o`
  Request handshake.
- `icache_resp_dout_o`, `icache_resp_valid_o`, `icache_resp_ready_i`
  Response handshake. Write requests also echo the written line data, matching the
  original behavior used by the micro-cache fill path.

### L2 fill side

- `neigh_sc_rsp_info_i`, `neigh_sc_rsp_valid_i`, `neigh_sc_rsp_ready_i`
  Neighborhood response bus used to catch I-cache fill data and ECC-protect it
  before SRAM writeback.

### MBIST

- `bist_req_info_i`, `bist_rsp_info_o`
  Raw per-panel I-cache MBIST access path.

### Debug APB

- `apb_paddr_i`, `apb_pwrite_i`, `apb_psel_i`, `apb_penable_i`, `apb_pwdata_i`
  APB-side debug transaction inputs.
- `apb_pready_o`, `apb_prdata_o`, `apb_pslverr_o`
  APB debug response outputs.

## Integration notes

- The translated wrapper assumes the generic/FPGA RAM abstraction provided by
  `prim_ram_1p` and DFT SRAM clock override through `prim_clk_mux`.
- `ram_cfg_i` is passed through to `prim_ram_1p`; in simulation and FPGA bring-up
  it is typically tied to `'0`.
- The debug APB path is a read-first, read/modify/write protocol exactly like the
  original wrapper.

## Intentional differences from the original

| Original | Translation |
|----------|-------------|
| `clock`, active-high `reset` | `clk_i`, active-low `rst_ni` |
| `dft__*` scalar ports | `dft_pkg::dft_t dft_i` plus `dft_sram_clk_i` |
| `esr_shire_cache_ram_cfg_t` | `ram_cfg_pkg::ram_cfg_t` |
| `et_clk_mux2`, `gen_mem1p`, `ecc_gen` | `prim_clk_mux`, `prim_ram_1p`, `prim_ecc_enc` |
| `soc.vh` defines/types | package-based types/constants |

No intentional functional differences are introduced.
