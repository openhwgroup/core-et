# Compute Shire Native Contract

This note freezes the project-native external contract for the compute shire
before the remaining RTL translation tasks start. It is a contract note, not an
RTL implementation.

## Contract decisions

- The public compute-shire IP is `shire_top`, but implementation is split into
  smaller IP blocks matching the downstream tasks: native seams, shire ESR/APB,
  uncached path, shire bus master, channel leaves, `shire_channel`,
  `shire_channel_wrap`, and the final `shire_top` wrapper.
- The original generated mesh-stop and debug mesh surface is not preserved. The
  native top exposes typed AXI channel payloads and valid/ready signals.
- Third-party/debug/sensor/hard-macro surfaces are omitted completely. Required
  behavior crossing those surfaces is replaced by project-native structs,
  primitives, APB/AXI ports, and reset/DFT/RAM seams.
- DFT uses `dft_pkg::dft_t` per voltage/clock domain plus a separate
  `dft_sram_clk_i`. RAM tuning uses `ram_cfg_pkg::ram_cfg_t` internally and is
  driven by the shire ESR/APB path in the final integration.
- Reset-domain separation is mandatory: cold, warm, debug, neighborhood,
  cache, RBOX, uncached/NOC, and ICache warm-reset paths remain distinct after
  conversion to active-low `_ni` naming.

## Native top-level boundary

`shire_top` shall use lowRISC-style ports and existing packages where possible:

- `axi_pkg` for cache/uncached/L3/SYS AXI payload structs.
- `apb_pkg` for internal ESR APB request/response structs.
- `dft_pkg` and `ram_cfg_pkg` for DFT and RAM timing/test control.
- `esr_pkg`, `neigh_pkg`, `shirecache_pkg`, `rbox_pkg`, `icache_pkg`, and a new
  `shire_pkg` only for compute-shire cross-block structs not already defined.

### Parameters

The top-level defaults are the current Gen1 configuration:

| Parameter | Default source | Meaning |
|---|---:|---|
| `NumNeigh` | `neigh_pkg::NumNeigh` = 4 | Neighborhoods per shire. |
| `NumBanks` | `shirecache_pkg::Banks` = 4 | Shire-cache banks. |
| `NumRbox` | 1 | RBOX ports integrated with the cache. |
| `NumUc` | 1 | Uncached target behind the cache crossbar. |
| `NumL3MasterPorts` | `shirecache_pkg::L3MasterPorts` = 4 | Cache-to-L3 AXI master ports. |
| `NumL3SlavePorts` | `shirecache_pkg::L3SlavePorts` = 4 | L3-to-cache AXI slave ports. |
| `NumSysPorts` | `shirecache_pkg::SysPorts` = 1 | Cache-to-system AXI master ports. |

Simulation-only original parameters that select stubs, DPI monitors, generated
NoC stimuli, or third-party debug paths are not top-level parameters. If a real
functional feature remains configurable, later RTL must express it as a typed
parameter in the owning module and document it in that IP README.

### Clock ports

The native compute shire does not instantiate a hard PLL/DLL macro. It accepts
already generated functional clocks from the SoC clock controller:

| Native port | Original source/domain | Contract |
|---|---|---|
| `clk_shire_i` | `shire_clock` from `shire_pll_wrapper` | Main high-voltage shire/cache/RBOX/uncached/control clock. |
| `clk_debug_i` | `shire_debug_clock` | Debug/run-control clock domain; not merged with `clk_shire_i` in the contract. |
| `clk_neigh_i[NumNeigh]` | `clock_neigh[gen]` | Per-neighborhood/minion clock inputs. FPGA builds may tie all entries to one physical clock behind the integration head, but RTL keeps the array. |
| `clk_noc_i` | `clk__noc` / `clk__noc_lv` | AXI/NOC-facing clock for native fabric crossings. |
| `dft_sram_clk_i` | original `dft__sram_clock` | SRAM DFT clock source for `prim_clk_mux`; it is a clock, not a `dft_t` field. |

The original `ref_clock`, `step_clock`, PLL/DLL analog/debug pins, and
Yin/Yang PLL debug pins are omitted from `shire_top`. Later clock-control RTL
may preserve ESR-visible lock/busy/status behavior with a project-native clock
control leaf, but it must not add hard-macro compatibility pins.

### Reset ports and internal reset mapping

Top-level reset inputs are active-low and asynchronously asserted:

| Native reset | Original source | Use |
|---|---|---|
| `rst_cold_ni` | `reset_cold` | Cold reset seed for shire ESR/control, cache cold reset, and neighborhood cold resets. |
| `rst_warm_ni` | `reset_warm` | Warm reset seed for shire data-path, RBOX, uncached, and neighborhood warm resets. |
| `rst_system_ni` | `reset_n_system` | System reset input to the reset-manager seam. |
| `rst_system_debug_ni` | `reset_n_system_debug` | System debug reset input to the reset-manager seam. |
| `rst_noc_ni` | `reset_noc` / `reset_noc_lv` | Native AXI/NOC reset domain. |

`dmctrl_i.ndmreset`, `dmctrl_i.dmactive`, and the translated BPAM GPIO reset
request remain inputs to the reset-manager leaf that produces internal
`rst_c_ni`, `rst_w_ni`, and `rst_d_ni`. Those domains are then repeated or
synchronized with `prim_rst_sync` before each endpoint:

- per-neighborhood `rst_c_shire_ni`, `rst_d_shire_ni`, `rst_w_shire_ni`
  remain separate and are gated by the neighborhood enable ESR before reset
  synchronization;
- RBOX receives `rst_d_ni` and `rst_w_ni` only, matching the original absence
  of a cold-reset input to `rbox_top`;
- shire cache receives cold, warm, and debug resets separately;
- ICache SRAM wrappers receive the neighborhood-returned ICache warm reset and
  the shire warm reset as a distinct ICache reset path;
- uncached/NOC paths use warm reset and `rst_noc_ni` as distinct domains.

Do not collapse these domains into a single `rst_ni` in later tasks.

### AXI boundary

The native top exposes AXI as typed payload structs plus scalar valid/ready
signals. Direction follows AXI master/slave convention; no SV interfaces are
used.

| Port group | Count | Payload type | Source/destination | Notes |
|---|---:|---|---|---|
| `to_l3_axi_*` | `NumL3MasterPorts` | `axi_pkg::sc_master_*_t` | Shire cache to L3 fabric | Existing `shirecache_top` ports of the same role. |
| `uc_to_l3_axi_*` | 1 | `axi_pkg::sc_master_*_t` | Uncached path to L3 fabric | Preserves original uncached atomic/L3 master path. |
| `to_sys_axi_*` | `NumSysPorts` | `axi_pkg::sc_master_*_t` | Shire cache to system fabric | Existing cache-to-system master path. |
| `uc_to_sys_axi_*` | 1 | `axi_pkg::sc_master_*_t` | Uncached path to system fabric | Preserves original uncached system master path. |
| `l3_axi_*` | `NumL3SlavePorts` | `axi_pkg::sc_slave_*_t` | L3 fabric into shire cache | Existing cache slave path. |
| `sys_axi_*` | 1 | new `axi_pkg::sys_slave_*_t` | System fabric into shire uncached/SBM path | `AR/AW/B` use the `sc_slave` ID/address shape; `W/R` use the original 256-bit SYS data width. |

`axi_pkg` already defines the `sc_master_*` and `sc_slave_*` payloads used by
`shirecache_top`. The SBM/uncached task must extend `axi_pkg` with
`SysSlaveDataSize = 256`, `sys_slave_w_t`, `sys_slave_r_t`, and the matching
channel valid/ready convention before adding `sys_axi_*` RTL. This is a package
extension, not a source-code compatibility wrapper for the original generated
mesh.

### APB/ESR access

There is no independent external APB port on `shire_top`. External ESR access
enters through the native `sys_axi_*` slave and the translated shire bus master,
which fan out internal APB requests to neighborhoods, shire-cache bank ESRs,
RBOX, ICache debug RAM, and shire-level ESRs.

Internal APB is represented with `apb_pkg::req_t` and `apb_pkg::rsp_t` unless an
existing translated block already has a narrower typed APB struct:

- `rbox_top` keeps `apb_pkg::req_t apb_req_i` / `apb_pkg::rsp_t apb_rsp_o`.
- `neigh_top` keeps `neigh_ch_apb_mux_pkg::apb_to_neigh_t` and
  `apb_from_neigh_t` at its boundary, with adapters from the shire APB fanout.
- `shirecache_top` consumes per-bank `shirecache_pkg::bank_esr_ctl_t` and
  produces `bank_esr_status_t`; `compute-shire-esr-apb` owns the APB bank-ESR
  translation that creates those structs.
- ICache memory debug APB remains behind `icache_data_ram_wrap`; the channel
  wrapper adapts from the shire APB fanout to that wrapper's scalar APB pins.
- `esr_pkg::esr_ms_dmctrl_t dmctrl_i` remains a top-level direct debug-control
  input because the original top receives `dmctrl_in` outside the APB path.

### Interrupt, status, and error seams

Native top-level interrupt/status ports are:

| Native port | Direction | Contract |
|---|---|---|
| `plic_mtip_i`, `plic_meip_i`, `plic_seip_i` | input | Original PLIC external interrupt inputs. `mtime` targeting remains controlled by shire ESRs before per-minion distribution. |
| `ioshire_combined_err_int_o` | output | CDC-synchronized OR of shire-cache and ICache error-log interrupt sources. |
| `ioshire_noc_err_int_o` | output | OR of native normal-fabric error sources supplied through a project-native `shire_pkg::noc_err_status_t` seam. Original debug-NoC error sources are omitted. |
| `and_or_tree_l2_e2w_i/o`, `and_or_tree_l2_w2e_i/o` | input/output | Ainekko-owned hart debug/status daisy-chain equivalent. The type belongs in `shire_pkg` or `esr_pkg` before `shire_and_or_tree_daisychain` is translated. |

UltraSoc/status-monitor message streams, generated debug-NoC message/event
ports, and sensor scan outputs are not part of the native status contract.
Architecturally visible halt/resume/run status is preserved through `dmctrl_i`,
BPAM run-control logic, neighborhood ESR status structs, and the and/or-tree
status seam.

### Power/control seams

Power-control fields are internal ESR-to-neighborhood connections, not top-level
hard-macro pins:

- `esr_power_ctrl_glb_nsleepin`, `esr_power_ctrl_glb_iso`, and corresponding
  `nsleepout` stay as the global neighborhood power-control handshake.
- `esr_power_ctrl_neigh_nsleepin`, `esr_power_ctrl_neigh_iso`, and per-minion
  `nsleepout` stay per-neighborhood/per-minion handshakes.
- Neighborhood clock-gate override and minion memory override remain typed ESR
  control fields into `neigh_top`.
- Voltage-monitor analog pins from the original top are omitted. If a future
  chip-level power controller needs observability, it must use a new reviewed
  project-native status struct, not original sensor/analog compatibility pins.

## Native DFT shape

The compute shire uses two DFT structs at the top boundary:

| Native port | Domain |
|---|---|
| `dft_hv_i` | shire/cache/RBOX/neighborhood high-voltage domain. |
| `dft_lv_i` | native AXI/NOC low-voltage or fabric-crossing domain. |

`dft_pkg::dft_t` fields map as follows:

| `dft_t` field | Original signal family | Use |
|---|---|---|
| `scanmode` | `dft__scan_mode`, reset-bypass/testmode groups | Forces clock gates on and enables scan reset bypass in primitives. |
| `scan_reset_n` | `dft__reset` after polarity conversion | Scan reset source for `prim_rst_sync` and CDC/FIFO primitives. |
| `sram_clk_override` | `dft__sram_clock_override` / `dft__clk_override` select | Selects `dft_sram_clk_i` through `prim_clk_mux` in RAM wrappers. |
| `ram_rei`, `ram_wei` | original RAM read/write inhibit fields in `esr_shire_cache_ram_cfg_t` | DFT RAM read/write inhibits. |

Separate ports remain for:

- `dft_sram_clk_i`, because clocks are not packed data fields;
- `dft_mbist_en_i`, because MBIST insertion is a test-mode control for cache and
  ICache RAM wrappers, not normal functional DFT state.

The original shire DFT hub, scan chain in/out buses, SMS/WIR pins, OCC override
ports, PLL/DLL TDR select/test ports, TDR serial pins, and generated scan
feedthrough buffers are intentionally omitted from the native top. Later RTL
must not create placeholders for them. Existing primitive DFT ports remain:
`prim_rst_sync` bypasses reset synchronization in scan mode, `prim_clk_gate`
forces gated clocks on in scan mode, `prim_clk_mux` selects the SRAM DFT clock,
and async FIFO primitives receive per-domain DFT structs.

## RAM configuration and memory abstraction

All SRAM-like storage stays behind project RAM primitives or existing wrappers
that instantiate those primitives:

| User | Contract |
|---|---|
| Shire cache data/tag/tag-state/sub-bank RAMs | `shirecache_top` receives `ram_cfg_pkg::ram_cfg_t` and `dft_hv_i`; bank/pipe wrappers pass both to `prim_ram_1p`/`prim_ram_2p`. ESR deep-sleep/shutdown fields merge into the RAM config before sub-bank RAMs. |
| ICache SRAMs | `icache_mems` must be a wrapper around `icache_data_ram_wrap`; it passes `ram_cfg_t`, `dft_hv_i`, and `dft_sram_clk_i` to the RAM wrapper and preserves APB debug access. |
| RBOX buffers/depth RAMs | `rbox_top` receives the same shire RAM config and DFT struct; RBOX RAM users continue through `prim_ram_*` or `prim_fifo_sram`. |
| Neighborhood/minion RAM users | Existing neighborhood/minion wrappers keep their own primitive-backed RAM seams and receive DFT/RAM config only where the translated module already has that surface. |
| Uncached path | Any FIFO/register RAMs translate through `prim_fifo_reg`, `prim_fifo_sync`, `prim_fifo_sram`, or `prim_ram_*`; no bare memories are allowed. |

The final shire-channel ESR/APB block owns the APB-programmable RAM config
state that replaces `esr_shire_cache_ram_cfg_t`. Simulation/FPGA reset value is
`'0`. ASIC/tapeout integration drives timing margin, test, power-down, and
shutdown fields through the same `ram_cfg_t` struct.

## IP organization and source order

Use split IP blocks, not one monolithic `shire_top` block:

| IP block | Owns |
|---|---|
| `shire_pkg` | Compute-shire-only shared constants/types such as SYS AXI data width, native NoC error status, and L2 and/or-tree status types when not already in `axi_pkg`/`esr_pkg`. |
| `shire_native_seams` | Any missing CDC/reset/clock-control seams proven necessary by this contract. Do not add TDR/scan/vendor placeholders. |
| `shire_esr` | `esr_shire_other`, `esr_cache_bank` equivalent, `apb_esr_ff`, and adapters from APB structs to cache/RBOX/neighborhood/ICache controls. |
| `shire_uncached` | `uncacheable` and uncached NOC/AXI/voltage-crossing leaves. |
| `shire_sbm` | `sbm_top`, `shire_bus_master`, and AXI-to-APB conversion. |
| `shire_channel_leaves` | Small Ainekko-owned control leaves: run control, and/or tree, cooperative TLoad bus, native NOC interrupt combiner, clock-control wrapper, and `icache_mems`. |
| `shire_channel` | Integrates ESR/APB, cache, RBOX, uncached, ICache memory wrapper, and channel leaves. |
| `shire_channel_wrap` | Native reset/clock/voltage shell around `shire_channel`. |
| `shire_top` | Final public compute-shire wrapper integrating `shire_channel_wrap` and `neigh_top` instances and exposing the native AXI/interrupt/DFT/reset contract. |

Source order for later Makefiles:

1. foundational packages: `dft_pkg`, `ram_cfg_pkg`, `apb_pkg`, `axi_pkg`,
   `etlink_pkg`, minion/front-end/dcache/TLB packages;
2. ESR/neighborhood/cache/RBOX/ICache packages, then `shire_pkg` if it imports
   them;
3. technology primitives selected by `mk/prim.mk`;
4. pure library primitives;
5. leaf modules;
6. containers in dependency order: uncached/SBM/ESR leaves, `shire_channel`,
   `shire_channel_wrap`, `shire_top`.

## Downstream task contract

| Task | Unblocked contract constraints |
|---|---|
| `compute-shire-native-seams` | Add only seams needed for the native DFT/reset/CDC/clock-control contract. Use `dft_hv_i`/`dft_lv_i`; no TDR/scan/vendor placeholders. Extend `axi_pkg` with SYS slave types if not handled by the SBM task. |
| `compute-shire-esr-apb` | Produce APB structs/control/status for shire-level ESRs, cache-bank ESRs, and `apb_esr_ff`; generate `ram_cfg_t` internally from ESR state and preserve APB timing. |
| `compute-shire-uncached-path` | Use `axi_pkg::sc_master_*_t` for `uc_to_l3`/`uc_to_sys`, the new `sys_slave_*_t` for system ingress, and async FIFO primitives for HV/LV crossings with both DFT structs. |
| `compute-shire-sbm-apb-master` | Translate SYS AXI slave to internal `apb_pkg` fanout; preserve broadcast, credit, reset gating, and error-return behavior. |
| `compute-shire-channel-leaves` | Translate only Ainekko-owned leaves. Preserve run control, and/or tree, cooperative TLoad, normal NOC error interrupt combiner, native clock-control behavior, and `icache_mems`. Omit UltraSoc/debug/sensor leaves. |
| `compute-shire-channel` | Integrate existing `shirecache_top`, `rbox_top`, `neigh_top` contracts, uncached, SBM, ESR, ICache memory wrapper, and channel leaves. Keep reset domains and DFT/RAM config propagation distinct. |
| `compute-shire-channel-wrap` | Build the native shell around `shire_channel`: reset-manager, clock-control, DFT domain split, power/control wiring, and interrupt CDC. Do not reintroduce original generated debug mesh or sensor pins. |
| `compute-shire-top` | Expose the native AXI, clock/reset, DFT, interrupt/status, and ID ports; instantiate `NumNeigh` `neigh_top` blocks and one channel wrapper; no original mesh-stop wrapper. |
| `compute-shire-final-integration` | Verify all retained native outputs, update status/coverage, and document any top-level comparison exclusions solely as third-party-surface omissions. |

## Intentional differences from the original external surface

| Original surface | Native decision | Rationale |
|---|---|---|
| Generated mesh-stop include ports and wrapper | Replaced by explicit typed AXI channel ports. | Generated/vendor-style mesh wrapper is outside the Ainekko-owned RTL contract. |
| Debug mesh/message bridge and UltraSoc status monitor ports | Omitted. | Third-party debug fabric is excluded; Ainekko-owned run control/status remains through ESR/BPAM/status-tree seams. |
| Shire DFT hub scan chains, SMS/WIR pins, OCC controls, serial TDR pins | Omitted from `shire_top`; primitive DFT structs remain. | Project DFT contract is `dft_pkg::dft_t`, `dft_sram_clk_i`, MBIST enable, and primitive DFT ports. |
| PLL/DLL hard-macro analog/debug/TDR/Yin-Yang pins | Omitted; functional clocks are input ports. | Hard macros are technology/platform integration, not compute-shire RTL. |
| Temperature/process sensor analog/scan pins and voltage-monitor analog pins | Omitted. | Third-party/sensor IP is excluded. |
| DPI/UVM/simulation monitor ports and stub/lite build variants | Omitted from public RTL. | Project verification uses Verilator/C++ tests and explicit parameters only for real functional features. |
| Original reset polarity/naming | Converted to active-low `_ni`/`_no`; domains preserved. | Project reset convention. |
| Original `esr_shire_cache_ram_cfg_t` | Replaced by `ram_cfg_pkg::ram_cfg_t` plus `dft_t.ram_rei/ram_wei`. | Standard RAM config/DFT abstraction. |

## Source evidence

Original source evidence:

- `rtl/shire/shiretop/shire_top.v:34-39` declares original cold/warm resets,
  `ref_clock`, `step_clock`, and shire IDs; `:50-57` declares shire-level DFT
  scan/ATE pins; `:65-74` declares TDR pins; `:87-91` includes generated normal
  and debug mesh top ports; `:95-130` declares `dmctrl_in`, debug/status,
  sensor/debug bridge, PLIC, and interrupt ports; `:138-157` declares SMS pins.
- `rtl/shire/shiretop/shire_top.v:200-282` creates cache/uncached/L3/SYS AXI
  channel wires; `:340-435` creates per-neighborhood reset, ET-Link, power,
  APB, uncached FCC, FLB, and ICache SRAM wires.
- `rtl/shire/shiretop/shire_top.v:700`, `:715`, `:1033`, `:1212`, and `:1653`
  instantiate top buffers, logic wrapper, DFT hub, `shire_channel_wrap`, and
  `neigh_top` respectively.
- `rtl/shire/shiretop/shire_top_logic_wrapper.v:550-781` documents and connects
  the original mesh-stop AXI roles (`to_l3`, `uc_to_l3`, `to_sys`,
  `uc_to_sys`, `l3_slave`, `sys_slave`) through the generated wrapper.
- `rtl/shire/shire_channel/shire_channel_wrap.v:27-64` declares original
  reset/clock/DFT control pins; `:179-194` declares neighborhood/cache ET-Link
  paths; `:308-410` declares native AXI roles before the generated wrapper;
  `:933`, `:1377`, `:1677`, `:2002`, `:2505`, and `:2598` instantiate the NOC
  interrupt combiner, control/status top, cooperative TLoad bus, channel,
  PLL wrapper, and reset gasket.
- `rtl/shire/shire_channel/orig/shire_channel.v:24-37` declares shire channel
  reset domains; `:70-82` declares neighborhood/cache and IPI/FCC paths;
  `:105-146` declares APB/ESR, mtime, power, PLL/DLL, and clock-control paths;
  `:149-248` declares cache/uncached/L3/SYS AXI paths; `:386`, `:541`, `:735`,
  `:869`, `:1056`, `:1096`, `:1241`, and `:1293` instantiate APB flops, RBOX,
  shire cache, uncached path, and/or tree, shire ESR, TDR register, and ICache
  memories.
- `rtl/shire/shire_channel/shire_control_status_top.v:169`, `:214`, `:363`,
  and `:425-464` show Ainekko-owned dmctrl/SBM behavior mixed with omitted
  UltraSoc and sensor surfaces.
- `rtl/shire/esr/esr_shire_other.v`, `esr_cache_bank.v`, `esr_rbox.v`,
  `esr_neigh.v`, and `esr_spio.v` show the APB two-phase `psel`/`penable`
  timing and the original reset-domain split (`reset_c`, `reset_w`, `reset_d`).
- `rtl/inc/axi_defines.vh:34-68` and `rtl/inc/axi_types.vh:55-115` define the
  original AXI widths: 40-bit addresses, 9-bit cache master IDs, 19-bit slave
  IDs, 512-bit cache data, and 256-bit SYS slave data.

Current translated source evidence:

- `hw/ip/axi/rtl/axi_pkg.sv:34-47` defines cache AXI widths and constants;
  `:55-147` defines `sc_master_*` and `sc_slave_*` payload structs.
- `hw/ip/apb/rtl/apb_pkg.sv:12-31` defines the project APB request/response
  structs with 16-bit addresses and 64-bit data.
- `hw/ip/dft/rtl/dft_pkg.sv:12-24` defines `dft_pkg::dft_t` and documents the
  separate SRAM DFT clock.
- `hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv:12-24` defines `ram_cfg_pkg::ram_cfg_t`.
- `hw/ip/etlink/rtl/etlink_pkg.sv:20-28` defines 40-bit physical addresses,
  8-bit ET-Link IDs, 4-bit source fields, and 512-bit data; `:105-154` defines
  request/response structs used between neighborhoods/RBOX/cache.
- `hw/ip/shirecache/rtl/shirecache_top.sv:41-55` exposes cache clocks, resets,
  DFT, and RAM config; `:59-155` exposes neighborhood, uncached, ESR, and AXI
  ports.
- `hw/ip/rbox/rtl/rbox_top.sv:41-68` exposes separate debug/warm resets,
  ET-Link, APB, RAM config, and DFT.
- `hw/ip/neigh_top/rtl/neigh_top.sv:24-170` exposes shire/neighborhood clocks,
  cold/debug/warm resets, DFT, power controls, ESRs, ET-Link, APB, interrupts,
  ICache SRAM, TBOX, and cooperative TLoad seams.
- `hw/ip/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv:4-25` exposes the
  ICache SRAM request/response, DFT SRAM clock, `dft_t`, and `ram_cfg_t` seam.
- `hw/ip/tech_generic/README.md` documents the primitive contracts for clock
  gates, reset synchronizers, SRAM clock muxes, RAM wrappers, and CDC FIFOs;
  `prim_rst_sync.sv`, `prim_clk_gate.sv`, `prim_clk_mux.sv`, and
  `prim_ram_1p.sv`/`prim_ram_2p.sv` implement the referenced generic contracts.
