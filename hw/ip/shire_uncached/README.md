// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# Shire uncached path

`shire_uncached` is the project-native SystemVerilog translation of the CORE-ET
compute-shire uncached request/response path.  It covers uncached minion
requests, AXI conversion toward L3 and SYS, the SYS slave response path, FLB/FCC
ESR helpers, and high/low-voltage CDC wrappers.

## Main modules

- `uncacheable` — top-level wrapper tying the uncached NoC, FLB/FCC helpers, SYS
  slave routing, and HV/LV crossing blocks together.
- `uncacheable_noc` — request queue, broadcast sequencing, AXI request emission,
  decode-error/ESR responses, and minion response arbitration.
- `uncacheable_noc_rsp` — arbitration and response conversion for SYS-slave,
  L3, and SYS master responses.
- `noc_etlink_to_axi` — ET-Link uncached request to AXI AR/AW/W conversion.
- `uncached_hi_voltage_cross` / `uncached_lo_voltage_cross` — CDC wrappers using
  `prim_fifo_async_hiv` and `prim_fifo_async_lov`.
- `uncacheable_flb` / `uncacheable_fcc` — FL barrier and FCC ESR helpers.
- `uncached_arb_lru_grant` — local translation of the original `arb_lru_grant`
  contract, where grant remains visible while priority updates are stalled.

## Parameters

| Parameter | Module | Description |
|-----------|--------|-------------|
| `NumNeighP` | `uncacheable`, `uncacheable_fcc`, `uncacheable_flb` | Number of neighbor directions. Defaults to `shirecache_pkg::NumNeigh`. |
| `NumPortsP` | `uncacheable`, `uncacheable_noc` | Number of shire-cache response ports. Defaults to neighbors plus RBox port. |
| `ReqqSize` | `uncacheable_noc`, response handlers | Request queue depth. Defaults to `UcReqqSize` (32). |

## Interfaces

All AXI channels use packed structs from `axi_pkg`:

- `sc_master_*_t` for the CORE-ET shire-cache master ports toward L3/SYS.
- `sys_slave_*_t` for the SYS slave path, including 256-bit R/W data.

Minion request/response traffic uses `shirecache_pkg::xbar_req_t` and
`shirecache_pkg::xbar_rsp_t`.  ESR control uses `shire_esr_pkg::esr_uc_config_t`
and `shire_uncached_pkg` FLB/FCC helper structs.

The top-level `uncacheable` has separate high-voltage/core (`clk_i`, `rst_ni`)
and low-voltage/NoC (`noc_clk_i`, `noc_rst_ni`) domains.  CDC is performed only
inside the voltage crossing modules.  DFT controls are passed as `dft_pkg::dft_t`
for the high- and low-voltage domains.

## Integration notes

- Include packages before modules: `axi_pkg`, `etlink_pkg`, `shirecache_pkg`,
  `shire_esr_pkg`, then `shire_uncached_pkg`.
- Include selected primitives from `mk/prim.mk`: `PRIM_FIFO_REG`,
  `PRIM_ARB_LRU`, `PRIM_RF_3R2W`, `PRIM_CLK_GATE`, `PRIM_FIFO_ASYNC_HIV`, and
  `PRIM_FIFO_ASYNC_LOV`.
- In normal FPGA/simulation operation drive DFT structs with `'0`.
- SYS slave write VC bit 0 routes to the uncached path and bit 1 routes to SBM,
  matching the original `UC_SYS_SLAVE_VSI_ID` / `SBM_SYS_SLAVE_VSI_ID` split.

## Verification

`hw/ip/shire_uncached/dv` builds `shire_uncached_unit_tb`, a directed unit test
suite with 88 explicit `check()` assertions.  It covers FLB/FCC helper behavior,
ET-Link-to-AXI conversion, response handlers and response arbitration, voltage
crossing ready/valid/credit transfer plus DFT reset bypass, reset behavior,
`uncached_arb_lru_grant` stall-visible grants, directed `uncacheable_noc`
unicast/broadcast/error paths, and a top-level `uncacheable` smoke check.

Standalone RTL cosims live under `dv/rtlcosim/` for every module listed above.
The voltage-crossing and top-level cosims compare payload outputs every cycle;
only unused padding bits outside the packed signal widths are masked.

## Intentional differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Active-low async reset ports (`rst_ni`) replace active-high original resets. | Project reset convention. |
| AXI channels are packed structs from `axi_pkg` instead of textual include typedefs. | Project-native typed interface. |
| DFT controls are consolidated into `dft_pkg::dft_t`. | Project DFT abstraction. |
| CDC FIFOs use `prim_fifo_async_hiv/lov`. | Technology abstraction for voltage crossings and level-shifter insertion. |
| `rf_latch_3r_1w` request queue storage is implemented through `prim_rf_3r2w` with the second write port tied off. | Existing project primitive seam for the translated latch RF contract. |

No functional bug fixes are intended; cycle-level behavior is meant to match the
CORE-ET modules except for the documented reset polarity/style and technology
abstractions above.
