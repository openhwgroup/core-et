// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# neigh_top

`neigh_top` is the translated CORE-ET neighborhood top shell. It wraps the
already translated `neigh_channel`, power-isolation stub, ECO hooks, interrupt
synchronizers, optional TBOX-facing path, and the generated minion array.

`neigh_top_pwrstub` is the power-isolation boundary used by the top shell. It
masks traffic that crosses into or out of the powered neighborhood domain and
returns safe ready/error/tie-off values while `pwr_ctrl_glb_isolate_i` is set.

## Parameters

| Parameter | Description |
| --- | --- |
| `ShireId`, `NeighId` | Preserve the original `ET_SIMULATION` ID parameters for simulation-only minion instance naming. Functional RTL does not consume them. |
| `EnableGfx` | Explicit replacement for original `GFX_ENABLED`. `0` ties the TBOX/GFX inputs off exactly as the original non-GFX build; `1` passes the TBOX path through `neigh_channel`. |
| `DisableMinions` | Explicit replacement for original `DISABLE_MINIONS`; set bits do not instantiate `minion_top` and drive the original stub tie-offs. |
| `StubMinions` | Explicit replacement for original `STUB_MINIONS`; set bits use the same tie-off behavior as disabled minions. |

## Port interface

The top-level port groups mirror the original CORE-ET `neigh_top` surface using
project naming and packed package types:

- high-voltage clocks/resets and warm reset (`clk_i`, `clk_shire_i`, `rst_*_ni`)
- consolidated DFT reset controls in `dft_pkg::dft_t`, plus preserved scan/OCC
  spare pins that are tied idle in the non-inserted DFT build
- ECO and power-control pins
- shire/cache ET-link request and response buses
- ESR/APB, debug/BPAM, PMU/status-monitor, interrupts/IPI/FCC, DLL delay-estimator,
  fast-local-barrier, shared I-cache SRAM, cooperative TLoad, and optional TBOX
  sidebands

Reset outputs use the project active-low convention (`*_no`). The standalone
`neigh_top_pwrstub` keeps the original active-high `reset_w_icache` payload
semantics internally so its cosim can compare directly against the original
power-stub module; `neigh_top` inverts that payload at the public boundary.

## Integration notes and source order

Build packages before modules. A complete non-minion-stub source list starts with:

1. `hw/ip/dft/rtl/dft_pkg.sv`
2. `hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv`
3. `hw/ip/etlink/rtl/etlink_pkg.sv`
4. `hw/ip/shirecache/rtl/shirecache_pkg.sv`
5. `hw/ip/esr/rtl/esr_pkg.sv`
6. `hw/ip/minion/rtl/minion_pkg.sv`
7. `hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv`
8. `hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv`
9. `hw/ip/icache_pkg/rtl/icache_geom_pkg.sv`
10. `hw/ip/icache_pkg/rtl/icache_pkg.sv`
11. neighborhood packages (`neigh_pkg`, `neigh_ch_apb_mux_pkg`,
    `neigh_hv_logic_pkg`, `neigh_voltage_cross_pkg`, `neigh_ch_dbg_pkg`)
12. primitives, channel dependencies, `neigh_channel`, `neigh_top_pwrstub`, and
    `neigh_top`

If any minion bit is not stubbed or disabled, include the translated `minion_top`
source tree as well.

## Design constraints and assumptions

- The non-GFX configuration is the current project default and is the configuration
  used by the standalone full-hierarchy cosim.
- `ET_USE_EVENT_FIFOS` and EVL replay/monitor-only code from the original are not
  synthesizable top-shell behavior in this reimplementation.
- DFT insertion scan chains are not inserted in this open RTL build; scanout/OCC
  scanout are tied to the same idle values as the original `ET_DFT_INSERTION=0`
  configuration.
- Minion stubs preserve the original tie-offs for L2/I-cache/PTW/APB/debug/PMU
  sidebands. As in the original, `StubMinions`/`DisableMinions` affect only the
  generated minion leaves; `neigh_channel` observes the resulting tied-off
  per-minion buses rather than a separate channel stub parameter. Real minions
  require the translated `minion_top` source tree.

## Verification

- Unit DV in `hw/ip/neigh_top/dv` checks `neigh_top_pwrstub` pass-through and
  isolation values plus `neigh_top` reset/DFT/power/tie-off smoke behavior.
- RTL cosim `dv/rtlcosim/neigh_top_pwrstub` compares every pwrstub output against
  the original `neigh_top_pwrstub`.
- RTL cosim `dv/rtlcosim/neigh_top` instantiates the original and translated
  full top shell with all minions stubbed, warms through reset/DFT/isolation
  stimulus, then compares all original outputs plus translated always-present
  TBOX outputs. Non-GFX-only translated TBOX debug-mux/DFT pins are compared
  against their documented tie-offs while all practical top-level input ports are
  driven through deterministic 0/1 toggle phases. Reset-control, isolation, and
  protocol-valid fields that would otherwise keep resetting the hierarchy or pop
  idle FIFOs are randomized during the reset/isolation exercise window, then held
  benign for the long strict-compare phase; the cosim `input_toggle_ok_o` check is
  the toggle evidence for those constrained fields.

## Differences from original CORE-ET

| Difference | Rationale |
| --- | --- |
| Active-low reset port names (`*_ni`, `*_no`) | Project-wide reset convention; cosims invert against original active-high reset ports. |
| `dft_pkg::dft_t` for functional DFT reset controls | Consolidates original reset-bypass/reset controls while preserving scan/test pins as explicit top-shell ports. |
| `EnableGfx` parameter | Replaces original `GFX_ENABLED` preprocessor selection. |
| Non-GFX `tbox_dbg_signals_mux_o` tied low | Original non-GFX builds have only an internal `tbox_dbg_signals_mux` wire and no parent-visible port. The translated superset port is held at a quiet tie-off when `EnableGfx=0`; `EnableGfx=1` passes through the channel debug mux. |
| `ShireId`/`NeighId` parameters | Explicit replacement for original `ET_SIMULATION` conditional parameters. |
| EVL/event-fifo monitor code omitted | Original EVL replay and event FIFO blocks are simulation/monitor-only optional code and are not part of the synthesizable translated top shell. |
| Technology cells replaced by primitives or simple translated adapters | `et_eco_ports`, level shifters, and voltage-monitor ties map to project primitives or logic-level equivalents. |
