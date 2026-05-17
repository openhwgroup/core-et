<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# Shire channel leaves

`hw/ip/shire_channel_leaves` contains the small Ainekko-owned control, status,
clock-control, and I-cache fanout leaves used by the native compute-shire
channel. The modules are grouped because they are narrow leaves consumed by
`shire_channel` / `shire_channel_wrap`, not standalone subsystems.

## Source order

Use the package before the leaf RTL:

1. foundational packages: `dft_pkg`, `ram_cfg_pkg`, `etlink_pkg`, `minion_pkg`,
   `icache_geom_pkg`, `icache_pkg`, `shirecache_pkg`, `esr_pkg`,
   `shire_esr_pkg`, `neigh_pkg`, `neigh_voltage_cross_pkg`, `rbox_pkg`
2. `shire_channel_leaves_pkg.sv`
3. technology primitives selected by `mk/prim.mk`: `prim_clk_mux`,
   `prim_clk_gate`, and `prim_ram_1p` when `icache_mems` is included
4. `prim_ecc_enc` and `icache_data_ram_wrap` when `icache_mems` is included
5. leaf RTL files in any order that keeps `shire_xll_control` before
   `shire_pll_wrapper`

## Modules and interfaces

| Module | Original CORE-ET module | Function and protocol |
|--------|--------------------------|-----------------------|
| `shire_dmctrl` | `shire_dmctrl` | Takes `esr_pkg::esr_ms_dmctrl_t dmctrl_i`, synchronizes and delays `write_enable`, captures the payload on either write-enable edge, and emits one-cycle `dmctrl_o.write_enable` pulses. Parameters `SyncStages=2` and `DelayStages=19` preserve the original filter/delay depths. |
| `shire_bpam_run_control` | `shire_bpam_run_control` | Combinationally maps `bpam_run_control_t` GPIO/event controls plus per-neighborhood TBOX single-step and RBOX single-step inputs to neighborhood run-control bundles and `rbox_pkg::rc_t`. Parameter `NumNeigh` defaults to 4. |
| `shire_and_or_tree_daisychain` | `shire_and_or_tree_daisychain` | Combinationally reduces per-neighborhood `esr_and_or_tree_l0_t` status into local L1 status and merges it into the incoming L2 daisy-chain status according to `shire_id_i`. |
| `shire_coop_tload_bus` | `shire_coop_tload_bus` | Pure combinational cross-connect that presents every neighborhood with the cooperative TLoad ready/done buses from all other neighborhoods. Parameters set neighborhood and cooperative-ID geometry. |
| `shire_ioshire_noc_ints` | `shire_ioshire_noc_ints` | Retained normal NoC interrupt/status combiner. `noc_all_err_int_srcs_o` is the normal-source pass-through; `ioshire_noc_err_int_o` is the registered OR of normal sources in `clk_noc_i` / `rst_noc_ni`. |
| `shire_xll_control` | `shire_xll_control` | APB-like auto-configuration sequencer used for PLL/DLL register windows. It generates divided `pclk_o`, `preset_no`, APB address/control/write data/strobes, and readback ready/data. Parameter set controls register widths and APB data width. |
| `shire_pll_wrapper` | `shire_pll_wrapper` | Native clock-control leaf. It keeps retained clock mux/gate behavior, private scalar synchronizers for clock select/gate controls and lock status, and the PLL/DLL auto-config sequencers. Functional clocks and lock indications are native inputs; omitted macro/debug surfaces are documented in `doc/exclusions.md`. |
| `icache_mems` | `icache_mems` | Four-neighborhood wrapper around translated `icache_data_ram_wrap`. It passes I-cache request/response, L2 fill response, BIST, APB debug, `dft_sram_clk_i`, `dft_pkg::dft_t`, and `ram_cfg_pkg::ram_cfg_t` to each child and OR/muxes child BIST/APB responses as in the original. |

## Package types and parameters

`shire_channel_leaves_pkg.sv` provides:

- geometry constants (`NumNeigh`, `NumOtherNeigh`, `NumShireIds`,
  `NumShireIdsBits`, `NocIntNum`, `ShireCoopIdWidth`)
- PLL/DLL auto-config sequencer widths used by `shire_pll_wrapper`
- `bpam_run_control_t` and sub-structs matching the retained BPAM run-control
  fields
- `esr_and_or_tree_l2_t` for the retained shire-level debug/status daisy chain
- `noc_err_status_t` for normal native NoC interrupt sources

## Primitive and abstraction mappings

| Original construct | Native mapping |
|--------------------|----------------|
| `et_cdc_sync` in `shire_dmctrl` and clock-control leaves | Private two-stage synchronizers inside the owning module. |
| `sync_rst` in the clock-control leaf | Private active-low reset synchronizers with `dft_t` scan-reset bypass where retained. |
| `et_clk_gate` | `prim_clk_gate` in `shire_pll_wrapper`. |
| `gf_clkmux_wrap` / clock mux tree | `prim_clk_mux` over native functional clock inputs. |
| `esr_shire_cache_ram_cfg_t` | `ram_cfg_pkg::ram_cfg_t` plus `dft_pkg::dft_t` fields. |
| Original SRAMs under I-cache memories | Existing `icache_data_ram_wrap` and its primitive-backed RAMs. |

## Intentional differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Active-high reset ports are translated to active-low `_ni` ports. | Project reset convention; reset domains remain separate where present. |
| Individual DFT pins are replaced by `dft_pkg::dft_t` where retained, with a separate `dft_sram_clk_i` for SRAM clock override. | Project DFT contract. |
| `shire_ioshire_noc_ints` omits original debug-NoC interrupt inputs and the debug portion of the concatenated status bus. | Debug-NoC fabric is a removed third-party/debug surface; retained normal NoC status is still compared in cosim. |
| `shire_pll_wrapper` omits hard PLL/DLL macro pins, serial TDR, PLL debug-chain, OCC/test-clock insertion, delay-tap hard-cell, SRAM-clock generation, and analog/debug surfaces. | These are hard-macro/vendor/debug/tapeout-infrastructure surfaces outside the native compute-shire RTL contract. The native leaf accepts functional clock and lock inputs instead. |
| `icache_mems` uses `ram_cfg_pkg::ram_cfg_t` and `dft_pkg::dft_t` instead of the original RAM config and DFT scalar pins. | Project RAM/DFT abstraction; child storage remains behind `icache_data_ram_wrap` / `prim_ram_1p`. |
| Struct and signal names use lowRISC/project naming. | Coding-style translation only. |

Detailed exclusion evidence is in `doc/exclusions.md`.

## Verification

- Unit test: `make -C hw/ip/shire_channel_leaves/dv test` — 32 checks.
- Standalone cosims:
  - `shire_dmctrl`: 2,200 comparisons
  - `shire_bpam_run_control`: 4,096 comparisons
  - `shire_and_or_tree_daisychain`: 8,192 comparisons
  - `shire_coop_tload_bus`: 12,288 comparisons
  - `shire_ioshire_noc_ints`: 4,096 comparisons over retained normal NoC outputs
  - `shire_xll_control`: 31,460 comparisons
  - `shire_pll_wrapper`: 43,560 comparisons over retained/native-equivalent outputs
  - `icache_mems`: 181,056 comparisons
