// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# shire_channel_wrap

`shire_channel_wrap` is the project-native shell around `shire_channel`. It keeps
the retained Ainekko-owned chip-integration behavior: clock/reset distribution,
DFT reset bypass, PLL/DLL clock-selection abstraction, the synchronized debug
control register path, SYS AXI to `sbm_top` APB fanout, neighborhood status and
power-control fanout, interrupt fanout, and the existing native `shire_channel`
cache/RBOX/uncached/ESR container.

## Parameters

- `NumNeigh`, `NumRbox`, `NumBanks`, `NumUc`, `NumPorts` — channel topology.
- `NumL3MasterPorts`, `NumL3SlavePorts`, `NumSysPorts` — AXI topology.
- `ThreadsPerNeigh`, `NumMinionsPerShire`, `NumGlobPwrDomains` — wrapper fanout
  geometry derived from the shire ESR constants.
- `UseNocClkGating`, `IcacheMemsImplemented`, `L2hpfImplemented` — passed to
  `shire_channel`.
- `ChannelApbSlaves`, `SbmApbSlaves` — APB lane counts for channel-local and SBM
  fanout buses.
- `DebugResetPulseCycles` — length of the synchronized debug reset pulse.

## Interface

The wrapper exposes native project seams:

- high-voltage clocks/resets: reference/step/PLL/DLL/NOC clocks, cold/warm/debug
  reset inputs, generated shire/neighborhood clocks, and active-low reset fanout;
- `dft_pkg::dft_t` high- and low-voltage DFT controls plus SRAM clock and MBIST
  controls;
- SYS AXI slave ingress, L3/SYS AXI master/slave seams, uncached AXI seams, and
  the internal SBM response loop;
- neighborhood APB request/response arrays using `neigh_ch_apb_mux_pkg` types;
- neighborhood run-control, interrupt, power, clock-gate, DLL status, I-cache,
  cooperative-load, and NoC interrupt/status fanout;
- `ram_cfg_pkg::ram_cfg_t` from the underlying channel ESRs.

## Integration notes

- The wrapper instantiates `shire_channel`, `sbm_top`, `shire_dmctrl`, and
  `shire_pll_wrapper` only through project-native RTL and technology primitives.
- APB lanes from `sbm_top` are mapped to neighborhood ports, shire-cache bank
  ports, the shire ESR port, RBOX, and the retained I-cache APB lane.
- `prim_rst_sync` is used for the low-voltage NOC reset repeat stage. The
  high-voltage reset manager keeps the original two-stage sync style before
  deriving cold/warm/debug reset domains.
- Global and minion power-control outputs preserve the reset mask behavior:
  `nsleepin` is forced asserted during cold reset and isolation is masked off
  during cold reset.

## Intentional differences from original CORE-ET wrapper

| Difference | Rationale |
|------------|-----------|
| Active-low native resets and `_i`/`_o` naming | Project convention and async-assert/sync-deassert reset strategy. |
| Consolidated DFT structs | Maps original DFT reset/clock behavior into `dft_pkg::dft_t`. |
| Project PLL/DLL clock abstraction | Replaces hard-macro pins with `shire_pll_wrapper` clock-selection behavior. |
| Removed debug/sensor/proprietary/hard-macro/generated surfaces | These surfaces are outside the retained Ainekko-owned native shell contract. |
| Native APB/SBM packages | Uses `shire_sbm_pkg` and `neigh_ch_apb_mux_pkg` typed buses instead of original include-defined buses. |

The implementation intentionally does not recreate removed compatibility pins or
placeholder modules for omitted third-party/debug/sensor infrastructure.
