<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# Shire channel leaves exclusions

This IP implements only the retained Ainekko-owned channel leaf behavior named in
`docs/compute_shire_contract.md`. The following original surfaces are intentionally
excluded from the native RTL.

| Original surface | Evidence | Native decision |
|------------------|----------|-----------------|
| Debug-NoC interrupt sources in `shire_ioshire_noc_ints` | `rtl/shire/shire_channel/shire_ioshire_noc_ints.v:8-11` declares normal NoC sources, debug-NoC sources, a concatenated normal+debug status bus, and one registered aggregate interrupt. | Retain normal NoC interrupt/status sources only. The native module exposes `noc_err_int_srcs_i`, `noc_all_err_int_srcs_o`, and registered `ioshire_noc_err_int_o`; debug-NoC inputs are third-party/debug fabric and are omitted. |
| PLL/DLL hard macro APB pins and analog/probe/test pins in `shire_pll_wrapper` | `rtl/shire/shire_channel/shire_pll_wrapper.v:21-36` contains retained PLL/DLL auto-config controls; `:37-80` declares delay-tap, feedback, test, serial scan/TDR, SRAM-clock, OCC, yin/yang, and macro-test pins; `:271-279` and `:411-418` show the original local simulation model tying PLL/DLL macro outputs/statuses. | Retain software-visible auto-config sequencers, clock mux/gate behavior, and lock/status synchronization. Replace hard macro pins with native clock inputs (`clk_ref_i`, `clk_step_i`, `clk_pll_i`, `clk_dll_i`) and native lock inputs (`pll_lock_i`, `dll_lock_i`). |
| Serial TDR chain and PLL debug block in `shire_pll_wrapper` | `rtl/shire/shire_channel/shire_pll_wrapper.v:216-263` and `:1634-1668` connect PLL/DLL serial TDR muxing and debug-chain observation logic. | Omit serial TDR/debug-chain compatibility surfaces. Project DFT is represented by `dft_pkg::dft_t` and technology primitives only. |
| OCC/test-clock insertion and generated SRAM test clock in `shire_pll_wrapper` | `rtl/shire/shire_channel/shire_pll_wrapper.v:192-197`, `:574-579`, and `:690-695` instantiate OCC/test-clock insertion around shire, neighborhood, and SRAM clocks. | Omit OCC/test-clock insertion. The native compute-shire contract supplies functional clocks and a separate `dft_sram_clk_i` to RAM wrappers. |
| Delay tap controls and hard clock-delay cells in `shire_pll_wrapper` | `rtl/shire/shire_channel/shire_pll_wrapper.v:178-189`, `:525-570`, and `:619-627` connect delay-tap controls and delay cells. | Omit hard delay-cell surfaces from this leaf. The native wrapper preserves clock selection and gating at the functional boundary; technology-specific clock implementation is handled outside this RTL. |
| Original RAM configuration type in `icache_mems` | `rtl/shire/shire_channel/icache/icache_mems.v:22-28` passes `esr_shire_cache_ram_cfg_t` to every `icache_data_ram_wrap`. | Replace with `ram_cfg_pkg::ram_cfg_t` and `dft_pkg::dft_t`; `icache_mems` still instantiates only the translated `icache_data_ram_wrap`, which owns primitive-backed SRAM behavior. |

The other leaves in this IP (`shire_dmctrl`, `shire_bpam_run_control`,
`shire_and_or_tree_daisychain`, `shire_coop_tload_bus`, and
`shire_xll_control`) have no third-party/debug/sensor/hard-macro sub-surfaces in
the retained logic. Their differences from the original are coding-style,
reset-polarity, and package/type naming changes documented in the IP README.
