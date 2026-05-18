// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# Shire channel

`shire_channel` is the project-native integration container for the compute-shire
core channel. It ties together the translated shire cache, RBOX, uncached path,
shire ESR/APB blocks, ICache SRAM wrapper, and the Ainekko-owned channel leaf
logic. The module replaces the original CORE-ET `shire_channel` while using the
native typed package interfaces and technology primitives required by this repo.

## Integrated blocks

- `shirecache_top` handles cache banks, cache crossbar, L3/SYS AXI ports, BIST,
  cache ESR control/status, cache error reporting, trace, and L2 HPF outputs.
- `rbox_top` is connected as a cache port and receives the translated run-control
  and RBOX APB lane.
- `uncacheable` handles the cache uncached miss path, FLB/FCC, SYS slave ingress,
  uncached L3/SYS AXI master paths, and SBM handoff signals.
- `esr_cache_bank` and `esr_shire_other` implement cache-bank and shire-level
  APB/ESR state, including shire ID, enables, IPI, mtime, clock-control, power,
  RAM config, and run-control status aggregation.
- `icache_mems` wraps `icache_data_ram_wrap` instances and preserves ICache SRAM
  APB/debug RAM, BIST, RAM config, DFT, and SRAM-clock override plumbing.
- `shire_bpam_run_control`, `shire_and_or_tree_daisychain`,
  `shire_ioshire_noc_ints`, and `shire_coop_tload_bus` provide the translated
  BPAM/run-control, debug/status tree, normal NoC interrupt combiner, and
  cooperative TLoad fanout logic.

## Parameters

| Parameter | Default | Description |
|-----------|--------:|-------------|
| `NumNeigh` | `shire_esr_pkg::NumNeigh` | Number of neighborhoods connected to the channel. |
| `NumRbox` | `shirecache_pkg::RboxPerShire` | Number of RBOX cache ports. Current native configuration uses one. |
| `NumBanks` | `shirecache_pkg::Banks` | Number of shire-cache banks. |
| `NumUc` | `1` | Number of uncached targets behind the cache crossbar. |
| `NumPorts` | `NumNeigh + NumRbox` | Cache requester ports. |
| `NumL3MasterPorts` | `shirecache_pkg::L3MasterPorts` | Cache-to-L3 AXI master ports. |
| `NumL3SlavePorts` | `shirecache_pkg::L3SlavePorts` | L3-to-cache AXI slave ports. |
| `NumSysPorts` | `shirecache_pkg::SysPorts` | Cache-to-SYS AXI master ports. |
| `NumBankReqqEntries` | `shirecache_pkg::ReqqDepth` | Cache bank request-queue entries. |
| `SetsPerSubBankParam` | `shirecache_pkg::SetsPerSubBank` | Cache sub-bank set count passed to cache-bank ESR and RAM paths. |
| `UseNocClkGating` | `1` | Pass-through cache mesh clock-gating configuration. |
| `IcacheMemsImplemented` | `1` | Enables native `icache_mems`; when `0`, the ICache/APB/BIST outputs are tied off for cosim/smoke configurations. |
| `L2hpfImplemented` | `0` | Pass-through cache L2 HPF configuration. |
| `ApbSlaves` | `NumBanks + NumRbox + (IcacheMemsImplemented ? 1 : 0) + 1` | APB lane count: cache banks, shire ESR, RBOX, and optional ICache. |

## Port protocol

### Clocks and resets

- `clk_i` is the high-voltage shire/cache/RBOX/uncached/control clock.
- `noc_clk_i` / `noc_rst_ni` are the native NoC/low-voltage clock and reset used
  by the cache mesh and uncached voltage-crossing owners.
- `rst_c_ni`, `rst_w_ni`, and `rst_d_ni` are separate cold, warm, and debug
  reset domains. They are not collapsed.
- `rst_w_icache_ni[NumNeigh]` is the separate ICache warm-reset input from the
  neighborhood path.
- Reset outputs to neighborhoods, cache, and RBOX are independently gated by ESR
  enables and repeated through `prim_rst_sync` where the original channel used
  reset repeaters. The public `rst_rbox_no` output intentionally follows the raw
  active-low warm-reset/RBOX-enable expression (`rst_w_ni & rbox_en`), while the
  internal `rbox_top` warm reset uses the repeated/synchronized net, matching the
  original public `reset_rbox` timing.

### DFT and RAM configuration

- `dft_hv_i` is passed to high-voltage reset repeaters, cache/RBOX/ICache RAM
  owners, and translated primitives.
- `dft_lv_i` is passed to the uncached low-voltage crossing owner.
- `dft_sram_clk_i` is the separate SRAM test clock used by RAM wrappers through
  the selected technology clock muxes.
- `dft_mbist_en_i` is synchronized in the cache reset domain before reaching
  `shirecache_top`.
- `ram_cfg_o` is produced by `esr_shire_other` and drives cache, RBOX, and ICache
  RAM wrappers.

### Traffic interfaces

- Neighborhood and RBOX cache request/response paths use `etlink_pkg::req_t` and
  `etlink_pkg::rsp_t` plus per-target valid/ready bits.
- Cache L3/SYS and uncached L3/SYS AXI channels use `axi_pkg::sc_master_*_t` and
  `axi_pkg::sc_slave_*_t` payloads with scalar valid/ready signals.
- SYS slave ingress and SBM handoff use `axi_pkg::sys_slave_*_t` payloads and the
  translated uncached/SBM credit split.
- APB lanes use `shire_sbm_pkg::shire_apb_req_t` and `shire_apb_rsp_t`. Lane
  order is cache banks, shire ESR, RBOX, and optional ICache.

### Control/status interfaces

The channel exposes shire ID, TBOX enables/IDs, minion feature controls, ICache
prefetch controls, IPI/mtime outputs, power-control handshakes, clock/DLL/PLL ESR
controls, normal NoC error interrupt aggregation, cache/ICache error interrupt
aggregation, debug/status and/or-tree output, BPAM run-control fanout, and
cooperative TLoad fanout.

## Integration notes

- Include packages before modules: `dft_pkg`, `ram_cfg_pkg`, `etlink_pkg`,
  `axi_pkg`, `apb_pkg`, ICache/minion/cache/ESR/neighborhood/RBOX/SBM/uncached
  packages, then `shire_channel_leaves_pkg`.
- Include technology primitives through `mk/prim.mk`: reset sync, RAMs, clock
  gates/muxes, FIFOs, arbiters, RF/ECC/hot2bin helpers used by child IPs.
- Drive `dft_hv_i`, `dft_lv_i`, `dft_sram_clk_i`, and RAM/MBIST controls from
  the wrapper/top-level DFT contract. In normal simulation/FPGA operation drive
  DFT structs with `'0` unless a test deliberately exercises DFT bypass.
- The wrapper around `shire_channel` owns global reset derivation and functional
  clock generation. This module consumes already separated reset domains.

## Verification

`hw/ip/shire_channel/dv` contains `shire_channel_tb` and a directed C++ test with
56 explicit `check()` assertions. The test covers reset-domain sequencing,
including DFT scan reset and the raw public RBOX reset behavior; APB/ESR writes,
readback, and cache-bank/RBOX/ICache APB lane fanout; shire ID/TBOX IDs/enables;
IPI, mtime, power control, RAM config, and clock-control fields; normal NoC/error
interrupt aggregation; cooperative TLoad fanout; ICache/cache readiness; and DFT
struct/MBIST/run-control propagation.

`dv/rtlcosim/shire_channel` instantiates the original CORE-ET `shire_channel`
and the new native container together with cache/ICache/RBOX/uncached children
enabled. The cosim exposes and compares every retained original/native output
pair cycle-by-cycle, including resets, ESR/APB, run-control/status, cache and
ICache request/response outputs, uncached FLB/FCC/SBM paths, power-control and
RAM/clock-control fields, L2 HPF/trace outputs, and L3/SYS AXI channels. It also
checks native-only wrapper-level leaves such as the normal NoC interrupt combiner
and cooperative TLoad fanout when no original `shire_channel` output exists.

## Intentional differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Active-low `_ni`/`_no` reset naming replaces active-high original reset pins. | Project reset convention; cold/warm/debug/ICache/NoC domains remain separate. |
| DFT controls are `dft_pkg::dft_t` plus `dft_sram_clk_i` and `dft_mbist_en_i`. | Project DFT abstraction; clocks are not packed into structs. |
| RAM tuning is `ram_cfg_pkg::ram_cfg_t` from `esr_shire_other`. | Replaces the original cache RAM config type with the project RAM config contract. |
| AXI/APB/ET-Link channels use project packed structs. | Native package-based interfaces replace textual include typedefs. |
| `shirecache_top`, `rbox_top`, `uncacheable`, `icache_mems`, and translated leaf IPs replace original child hierarchies. | Child IPs are translated and verified independently; the channel integrates the native instances while the standalone channel cosim compares retained top-level behavior with those children enabled. |
| Original scan/SMS/TDR/debug-monitor/status-monitor/hard-macro compatibility surfaces are omitted. | Required by `docs/compute_shire_contract.md`; retained behavior is represented through native DFT, RAM, ESR/APB, AXI, interrupt, and run-control seams. |
| The cooperative TLoad bus and normal NoC interrupt combiner are native channel outputs even though they were wrapper-level leaves in the original hierarchy. | The compute-shire native split places these Ainekko-owned leaves at the channel integration boundary. |

No functional bug fixes are intended. Retained Ainekko-owned behavior is meant to
match the original cycle-by-cycle except for the documented reset polarity,
technology abstraction, child-IP replacement, and third-party surface omissions.
