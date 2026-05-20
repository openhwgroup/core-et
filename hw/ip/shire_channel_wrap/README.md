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

- high-voltage clocks/resets: reference/step/PLL/DLL/NOC clocks, separate
  cold/warm/system/system-debug/NOC reset inputs, generated shire/neighborhood
  clocks, and active-low reset fanout;
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
  ports, the shire ESR port, and RBOX. The retained I-cache APB lane remains at
  the channel boundary for compatibility but is idle in this wrapper because the
  original BPAM/debug source that drove it is outside the retained native scope.
- `prim_rst_sync` is used for the low-voltage system, debug, and NOC reset
  repeat stages. The NOC reset stage is seeded by `rst_noc_ni`, not collapsed
  into `rst_system_ni`; the high-voltage reset manager keeps the original
  two-stage sync style before deriving cold/warm/debug reset domains.
- `rst_warm_to_neigh_no` preserves the original `reset_warm_to_neigh =
  {NUM_NEIGH{reset_warm}}` fanout. It follows only the raw external warm-reset
  seed (converted to active-low polarity) and is intentionally independent of
  the derived cold/warm/debug/gpio/ndmreset reset domains.
- Global and minion power-control outputs preserve the reset mask behavior:
  `nsleepin` is forced asserted during cold reset and isolation is masked off
  during cold reset.

## Verification

- Unit test: `hw/ip/shire_channel_wrap/dv/shire_channel_wrap_test.cc` performs
  66 directed checks covering system/NOC reset independence, NOC reset bypass,
  raw warm-reset fanout, DFT
  struct propagation, SYS/SBM/APB lane responses plus the idle retained I-cache
  APB lane, RAM-config writes, retained PLL/DLL/clock-control fanout,
  interrupt/status fanout, cooperative TLoad fanout, FLB/I-cache seams, and
  neighborhood/minion power reset masking.
- RTL cosim: `dv/rtlcosim/shire_channel_wrap/` instantiates an original
  retained-behavior wrapper slice from `/home/glguida/ainekko/et-soc` alongside
  the native wrapper. It records explicit input-toggle coverage for every
  retained cosim input port and compares 296,731 retained wrapper observations:
  resets, clocks, raw warm-reset fanout, APB/SBM enable and response vectors,
  SYS AXI handshakes, retained AXI/cache/uncached valid/ready controls and
  valid payloads, run-control, interrupts, cooperative TLoad, ESR/control fanout,
  RAM configuration, and clock-gate/power outputs.
- The cosim intentionally compares wrapper-shell retained behavior. Reset-release
  drain windows cover known original reset-repeat vs native synchronizer latency;
  all retained observations are compared on every non-drain comparison cycle.
  Standalone child cosims remain responsible for deeper `shire_channel`, cache,
  uncached, and RBOX datapath scenarios; removed third-party/debug/sensor/
  hard-macro surfaces are not reinstated or compared.

## Intentional differences from original CORE-ET wrapper

| Difference | Rationale |
|------------|-----------|
| Active-low native resets and `_i`/`_o` naming | Project convention and async-assert/sync-deassert reset strategy. |
| Consolidated DFT structs | Maps original DFT reset/clock behavior into `dft_pkg::dft_t`. |
| Project PLL/DLL clock abstraction | Replaces hard-macro pins with `shire_pll_wrapper` clock-selection behavior. |
| Removed debug/sensor/proprietary/hard-macro/generated surfaces | These surfaces are outside the retained Ainekko-owned native shell contract. |
| Native APB/SBM packages | Uses `shire_sbm_pkg` and `neigh_ch_apb_mux_pkg` typed buses instead of original include-defined buses. |
| Separate native `rst_noc_ni` input | The project-native top contract keeps the AXI/NOC reset domain distinct from `rst_system_ni`; the retained original reset cells are reused in DV with separate native seeds. |
| RAM configuration surface | Native wrapper exposes standardized `ram_cfg_pkg::ram_cfg_t`; wrapper DV writes the original ESR RAM-config fields and compares the retained packed configuration bits through the native abstraction. |

The implementation intentionally does not recreate removed compatibility pins or
placeholder modules for omitted third-party/debug/sensor infrastructure.
