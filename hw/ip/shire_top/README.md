// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# `shire_top`

`shire_top` is the project-native public top for one compute Shire.  It
integrates one `shire_channel_wrap` with `NumNeigh` `neigh_top` instances and
exposes the native compute-shire boundary defined in
`docs/compute_shire_contract.md`.

The module is an integration wrapper, not a behavioral replacement for its
children.  Cache, uncached, RBOX, shire ESR/APB, SBM/APB, ICache SRAM, reset,
clock, power, and status behavior remains owned by `shire_channel_wrap`,
`shire_channel`, `shirecache_top`, `rbox_top`, `shire_uncached`, `sbm_top`, and
`neigh_top`.

## Parameters

| Parameter | Default | Meaning |
|-----------|---------|---------|
| `NumNeigh` | `shire_esr_pkg::NumNeigh` | Number of neighborhoods in the Shire. |
| `NumRbox` | `shirecache_pkg::RboxPerShire` | RBOX request ports connected to the cache. |
| `NumBanks` | `shirecache_pkg::Banks` | Shire-cache bank count. |
| `NumUc` | `1` | Uncached request port count. |
| `NumPorts` | `NumNeigh + NumRbox` | Cache client-port count. |
| `NumL3MasterPorts` | package default | Native cache-to-L3 AXI master ports. |
| `NumL3SlavePorts` | package default | Native L3-to-cache AXI slave ports. |
| `NumSysPorts` | package default | Native cache-to-system AXI master ports. |
| `ThreadsPerNeigh` | `ThreadsPerShire / NumNeigh` | ESR/IPI thread slice width per neighborhood. |
| `NumMinionsPerShire` | package default | Total minions across all neighborhoods. |
| `NumGlobPwrDomains` | `NumNeigh` | Global power-control domains. |
| `UseNocClkGating` | `1` | Passed to `shire_channel_wrap`. |
| `IcacheMemsImplemented` | `1` | Enables shared ICache SRAM wrapper path. |
| `L2hpfImplemented` | `0` | Enables L2 HPF path when implemented. |
| `EnableGfx` | `0` | Graphics/TBOX neighborhood mode.  Default native top keeps TBOX external surfaces omitted. |
| `DisableMinions` | all `0` | Per-neighborhood minion disable mask; production default keeps neighborhoods/minions live. |
| `StubMinions` | all `0` | Per-neighborhood minion stub mask; production default keeps neighborhoods/minions live. |
| `ChannelApbSlaves` | package expression | `shire_channel_wrap` APB fanout count. |
| `SbmApbSlaves` | `ShireApbInterfaces` | SBM/APB lane count. |
| `DebugResetPulseCycles` | `32` | Debug reset pulse width in wrapper reset logic. |

## Interface groups

- **Clocks/resets:** contracted functional clocks `clk_shire_i`,
  `clk_debug_i`, per-neighborhood `clk_neigh_i`, `clk_noc_i`, plus separate
  cold, warm, system, and system-debug active-low resets.  The top preserves the
  child reset-domain split and returns `rst_system_lv_no` and
  `rst_system_debug_lv_no`.
- **DFT/RAM:** high-voltage and low-voltage `dft_pkg::dft_t`, DFT SRAM clock,
  MBIST enable, and `ram_cfg_pkg::ram_cfg_t` output from shire ESR.
- **Native AXI:** typed AXI ports for `to_l3`, `to_sys`, `l3_axi`,
  `uc_to_l3`, `uc_to_sys`, and `sys_axi` using `axi_pkg` structs.
- **ESR/APB/status:** shire ID reset value, physical ID, clock/PLL/DLL controls,
  debug and/or tree, NoC/error interrupts, selected status monitor bank, and
  cache trace/L2HPF status outputs.
- **Neighborhood seams:** APB, ET-Link cache, uncached FCC/FLB, ICache SRAM,
  cooperative TLoad, power-control, interrupts, BPAM run-control, DLL delay
  estimator, ESR controls/status, and raw warm-reset fanout are internal between
  `shire_channel_wrap` and each `neigh_top`.

## Integration notes

Instantiate one `shire_top` per compute Shire and connect only the native ports
listed above.  Do not reintroduce original generated mesh-stop, scan hub,
UltraSoc, sensor, hard PLL/DLL analog/debug/TDR/OCC, or debug message mesh
compatibility pins at this level.  Child pins that remain in `neigh_top` only to
preserve a translated superset are tied off or left internal here.

Production defaults keep `DisableMinions='0` and `StubMinions='0`, matching
Gen1/current child-package behavior so an SoC integration instantiates live
neighborhoods and minions unless it explicitly overrides the masks.  The unit
and cosim smoke wrappers override both masks to all ones to keep top-level DV
focused on retained shell wiring while lower-level neighborhood/minion behavior
remains covered by their standalone tests.

## Design constraints and assumptions

- `shire_top` instantiates exactly one `shire_channel_wrap` and `NumNeigh`
  `neigh_top` instances.
- Reset-domain separation from the original shell is preserved; cold, warm,
  debug/system, NoC, cache/RBOX, neighborhood, and raw warm-to-neighborhood
  paths are not collapsed.
- DFT and RAM test controls use only the project `dft_t`, DFT SRAM clock, MBIST,
  and `ram_cfg_t` abstractions.
- The top does not instantiate bare memories or technology cells directly.

## Differences from original CORE-ET `shire_top`

| Difference | Rationale |
|------------|-----------|
| Native typed AXI ports replace generated mesh-stop compatibility ports. | Project-native public contract from `docs/compute_shire_contract.md`. |
| Active-low reset naming/style (`*_ni`, `*_no`) replaces original active-high/reset macro naming. | Project reset convention. |
| DFT controls are consolidated into `dft_pkg::dft_t` plus explicit SRAM clock/MBIST inputs. | Project DFT abstraction. |
| Third-party UltraSoc/debug-message/sensor/hard PLL/DLL analog/TDR/OCC/scan-hub public pins are omitted. | Out of scope for Ainekko-owned native top; child-only preserved pins are tied off internally. |
| RAM configuration uses `ram_cfg_pkg::ram_cfg_t`. | Replaces original foundry-specific SRAM config struct. |
| DV wrappers override `DisableMinions`/`StubMinions` to all ones, but the public RTL defaults are all zeros. | Keeps top-level smoke tests independent of downstream minion/TBOX activity without changing production integration defaults. |

## Verification

- Unit/integration smoke: `make -C hw/ip/shire_top/dv test` (46 directed
  checks for reset fanout, DFT/RAM config, APB/SYS path, power-control,
  interrupts/status, ET-Link/uncached/ICache/FLB/cooperative seams).
- Lint: `make -C hw/ip/shire_top/dv lint`.
- RTL cosim: `ORIG_ROOT=/home/glguida/ainekko/et-soc make -C dv/rtlcosim/shire_top test`.
  The cosim runs 566,632 retained-output comparisons with zero mismatches. It
  uses original CORE-ET sources from `ORIG_ROOT` through the retained
  `shire_channel_wrap` reference plus explicit original top stub-neighborhood
  feedthroughs because the full original public top depends on removed
  third-party/generated surfaces. The cosim sources document the retained slice
  and compare independent original-side observations against the new
  `shire_top`.
