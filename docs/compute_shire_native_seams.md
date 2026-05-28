# Compute Shire Native Seams Audit

This audit checks the compute-shire CDC/reset/DFT/clock-control patterns against
`docs/compute_shire_contract.md`. The result is documentation-only: no new or
changed RTL primitive/seam is required now.

## Decision

No project-wide native primitive or reusable seam is missing for the frozen
compute-shire contract.

Rationale:

- Architecturally retained reset repeaters already map to `prim_rst_sync`.
- Architecturally retained clock gates and SRAM DFT clock selects already map to
  `prim_clk_gate`, `prim_clk_gate_n`, and `prim_clk_mux`.
- Architecturally retained payload CDC/voltage crossings already map to
  `prim_fifo_async_hiv`, `prim_fifo_async_lov`, `prim_fifo_semisync_hiv`, and
  `prim_fifo_semisync_lov`.
- Original scalar `et_cdc_sync` uses in the compute-shire path are owner-local
  level/status/control synchronizers. Current translated RTL already implements
  this class as private two-flop logic where needed, so a public scalar CDC
  primitive is not required by the contract today.
- Original `sync_rst`/`sync_rstn` behavior is reset-manager-local when it is not
  simply `rst_repeat`; it should be implemented privately in the owning reset
  manager, not as a new public primitive.
- Original serial TDR, scan hub, OCC override, hard PLL/DLL, sensor, and debug
  fabric surfaces are explicitly omitted by the compute-shire contract.

## Audit table

| Pattern | Original source and signals | Architecturally required by the contract? | Project-native mapping | New or changed RTL needed now? | Downstream constraints |
|---|---|---|---|---|---|
| Scalar CDC synchronizer (`et_cdc_sync`) | Definition: `rtl/libs/mems_and_fifos/et_cdc_sync.v:23-66` (`clock`, `reset`, `din`, `dout`, `RESET_VAL`). Compute uses include `rtl/shire/shire_channel/shire_dmctrl.v:20-28` (`dmctrl_in.write_enable`), `rtl/shire/shire_channel/shire_channel_wrap.v:904-914` (`ioshire_log_err_int`), `:963-975` (`noc_int_presync`), `rtl/shire/shire_channel/orig/shire_channel.v:717-725` (`dft__mbist_en`), and clock-control uses in `rtl/shire/shire_channel/shire_pll_wrapper.v:128-141`, `:598-603`, and `:646-651`. | Partially. Retained scalar/status/control crossings remain required for DM control, normal interrupt/status capture, MBIST enable staging, and native clock-control leaves. Crossings that only served omitted hard PLL/TDR/debug/OCC surfaces are not required. | Keep scalar CDC inline/private in the owning module, preserving the original number of stages and reset value. Use `prim_fifo_async_*`/`prim_fifo_semisync_*` for payload or handshake CDC. Do not add a public `prim_cdc_sync` now. | No. | `compute-shire-channel-leaves`: translate `shire_dmctrl` with a private resettable two-stage synchronizer for `write_enable`; translate native clock-control scalar enables privately. `compute-shire-channel-wrap`: synchronize retained interrupt/status levels privately in the destination clock. `compute-shire-channel`: pass scalar `dft_mbist_en_i` as the contract requires and stage it only where the owner needs the original timing. |
| Reset synchronizers (`sync_rst`, `sync_rstn`) | Definitions: `rtl/libs/mems_and_fifos/sync_rst.v:10-39` and `sync_rstn.v:10-40`. Reset-manager uses: `rtl/libs/rf_latches/sys_gasket_reset.v:37-72`; NOC reset uses: `rtl/libs/rf_latches/sys_gasket_noc_reset_sync.v:14-30`. PLL-wrapper local reset uses include `rtl/shire/shire_channel/shire_pll_wrapper.v:119-126`, `:291-298`, `:590-598`, `:637-644`, and `:668-675`. | Yes for retained reset-manager behavior and local reset-domain release timing. No for omitted hard PLL/DLL macro reset/test paths. | Use `prim_rst_sync` where the original pattern is `rst_repeat`. For true two-stage `sync_rst`/`sync_rstn` input resynchronization, keep a private helper inside the reset-manager/clock-control owner with `dft_pkg::dft_t` scan bypass semantics and active-low project reset naming. Preserve separate cold/warm/debug/system/NOC reset domains. | No. | `compute-shire-channel-wrap`: own the native reset manager; preserve the original warm/cold/debug derivation, debug reset pulse, and distinct system/system-debug/NOC resets. Preserve the original NOC reset phase relationship when translating `sys_gasket_noc_reset_sync` rather than collapsing it into a single `rst_ni`. |
| Reset repeater (`rst_repeat`) | Definition: `rtl/libs/mems_and_fifos/rst_repeat.v:15-39`; repeated throughout shire-cache and channel reset fanout. | Yes. Endpoint reset repeaters remain required. | Existing `prim_rst_sync` is the project reset repeater. The current compatibility wrapper `hw/ip/neigh_channel/rtl/rst_repeat.sv:4-32` shows the active-high-to-active-low adaptation. | No. | All downstream tasks: use `prim_rst_sync` for retained endpoint reset repeaters and keep reset-domain separation. Do not substitute a single global reset. |
| SRAM DFT clock mux (`et_clk_mux2`) | Definition: `rtl/libs/rf_latches/et_clk_mux2.v:4-18`. ICache use: `rtl/shire/shire_channel/icache/icache_data_ram_wrap.v:492-497`; PLL/TDR bypass muxes in `rtl/shire/shire_channel/shire_pll_wrapper.v:233-234` and `:261-262`. | Yes only for SRAM DFT clock selection. TDR bypass uses are omitted with the TDR surface. | Existing `prim_clk_mux` selected by `mk/prim.mk:38`; RAM wrappers use `dft_sram_clk_i` plus `dft_i.sram_clk_override`. | No. | `compute-shire-channel-leaves`: `icache_mems` must wrap `icache_data_ram_wrap` and pass `dft_sram_clk_i`, `dft_hv_i`, and `ram_cfg_t`. `compute-shire-esr-apb`: drive `ram_cfg_t`; do not recreate the original RAM config struct. |
| OCC/test clock override (`et_dft_occ_insert`) | Definition: `rtl/libs/rf_latches/et_dft_occ_insert.v:8-38`. PLL-wrapper uses: `rtl/shire/shire_channel/shire_pll_wrapper.v:192-197`, `:574-579`, and `:690-695`. | No. The contract omits shire DFT hub scan chains, OCC override ports, and hard PLL/DLL surfaces. Functional clocks are native inputs. | Omit OCC insertion. Retained local functional gating uses `prim_clk_gate`; retained SRAM test clock selection uses `prim_clk_mux`. | No. | `compute-shire-channel-leaves` and `compute-shire-channel-wrap`: native clock-control RTL must not add OCC/select/test clock compatibility ports. `compute-shire-top`: expose functional clocks and `dft_sram_clk_i` only. |
| Serial TDR path (`et_tdr_reg`, PLL/DLL TDR muxing, shire TDR overrides) | Definition: `rtl/libs/registers/et_tdr_reg.v:3-37`. Channel override register: `rtl/shire/shire_channel/orig/shire_channel.v:1241-1260`. PLL/DLL TDR chains: `rtl/shire/shire_channel/shire_pll_wrapper.v:216-263`. | No. The contract omits serial TDR pins, PLL/DLL TDR select/test ports, and scan hub feedthroughs. RAM tuning and reset/enable control remain through project-native ESR/DFT structs. | Omit. Use `dft_pkg::dft_t`, `ram_cfg_pkg::ram_cfg_t`, and translated ESR/APB state instead of serial TDR overrides. | No. | `compute-shire-esr-apb`: own APB-programmable RAM config and retained shire controls. `compute-shire-channel` and `compute-shire-channel-wrap`: do not instantiate or expose serial TDR state; do not preserve TDR reset/clock/RAM override ports. |
| Reset/voltage manager (`sys_gasket_hiv` and helpers) | Definitions: `rtl/libs/rf_latches/sys_gasket_hiv.v:4-129`, `sys_gasket_reset.v:4-157`, `sys_gasket_noc_reset_sync.v:4-30`, and `sys_gasket_noc_reset.v:4-38`. | Yes for reset-domain derivation, DFT reset-domain split, NOC reset synchronization, and debug reset pulse behavior. No for third-party level-shifter module instances and TDR override pins. | Implement as a downstream project-native reset manager in `shire_channel_wrap`, using `dft_hv_i`/`dft_lv_i`, `prim_rst_sync`, and private two-stage reset synchronizers where the original used `sync_rst`/`sync_rstn`. Treat level shifting as technology/integration intent, not a public compatibility surface. | No for this task. The reset manager is block-specific downstream RTL, not a shared primitive. | `compute-shire-channel-wrap`: translate the reset manager there; preserve cold/warm/debug/system/NOC separation and debug reset pulse timing. `compute-shire-channel`: consume separate reset outputs. |
| Payload/handshake CDC and voltage crossing FIFOs | Original async FIFO families are used by shire-cache, uncached, and neighborhood voltage crossings. | Yes for retained AXI/ET-Link/uncached/neighborhood payload movement. | Existing `prim_fifo_async_hiv`, `prim_fifo_async_lov`, `prim_fifo_semisync_hiv`, and `prim_fifo_semisync_lov` with per-domain DFT structs. | No. | `compute-shire-uncached-path`: use async FIFO primitives for HV/LV crossings. `compute-shire-channel` and `compute-shire-channel-wrap`: pass `dft_hv_i`/`dft_lv_i` through the crossing owners. |
| Local functional clock gates | Original `et_clk_gate`/`et_clk_gate_n` in clock-control, shire-cache, and leaf logic. | Yes where the Ainekko-owned function uses real local gated clocks or phase behavior. | Existing `prim_clk_gate` and `prim_clk_gate_n`. | No. | `compute-shire-channel-leaves`: use `prim_clk_gate` for retained native clock-control gated clocks and preserve scanmode force-on through `dft_t`; do not hand-roll gated clocks. |

## Scalar CDC downstream rule

Do not introduce a shared scalar CDC primitive for the current compute-shire
work. Translate each retained `et_cdc_sync` site privately in its owning module
when all of these are true:

1. The crossing is a level/status/control signal, not a payload stream.
2. The original site had no ready/valid backpressure and no externally visible
   module boundary for the synchronizer itself.
3. The owner can preserve `SYNC_STAGES` and `RESET_VAL` locally.
4. Reset-domain ownership is clear from the original site and remains separate
   in the translated module.

If a later task finds a repeated scalar CDC contract that needs technology-cell
selection, DFT pins beyond reset bypass, or a stable public boundary, it should
request a new primitive through a separate reviewed task. That condition is not
present in the frozen compute-shire contract.

## Downstream task instructions

| Task | Native-seam constraints |
|---|---|
| `compute-shire-esr-apb` | Generate `ram_cfg_pkg::ram_cfg_t` and retained shire control/status through APB/ESR state. Do not recreate serial TDR override state or the original RAM-config type. Preserve APB timing and reset-domain split. |
| `compute-shire-uncached-path` | Use `prim_fifo_async_hiv`/`prim_fifo_async_lov` for HV/LV payload crossings and `prim_fifo_reg`/`prim_fifo_sync` for local FIFOs. Use private scalar synchronizers only for level controls/status; do not use them for AXI payload transfer. |
| `compute-shire-sbm-apb-master` | Keep SYS AXI ingress and APB fanout in typed packages. No extra DFT/OCC/TDR seam is needed. Preserve reset-domain behavior with local reset synchronization where the original logic required it. |
| `compute-shire-channel-leaves` | Translate `shire_dmctrl` with private scalar CDC for `write_enable`. Translate native clock-control behavior without hard PLL/DLL, OCC, or TDR compatibility pins; use functional clock inputs, `prim_clk_gate`, and private scalar CDC for retained clock-gate enables/status. Translate `icache_mems` around `icache_data_ram_wrap` with `dft_sram_clk_i`, `dft_hv_i`, and `ram_cfg_t`. |
| `compute-shire-channel` | Integrate existing `shirecache_top`, `rbox_top`, and ICache wrappers with separate cold/warm/debug resets. Route scalar `dft_mbist_en_i` and `ram_cfg_t` per the contract. Do not instantiate the original shire TDR register or expose removed debug/scan surfaces. |
| `compute-shire-channel-wrap` | Own the project-native reset manager equivalent for `sys_gasket_hiv`: preserve reset derivation, debug reset pulse timing, HV/LV DFT split, system/system-debug/NOC reset separation, and interrupt/status CDC. Do not preserve third-party level-shifter module names or original debug mesh/sensor/OCC/TDR ports. |

## Intentionally omitted surfaces

These omissions are required by `docs/compute_shire_contract.md` and are not
missing seams:

| Original surface | Contract decision |
|---|---|
| Shire DFT hub scan chains, SMS/WIR pins, scan feedthrough buffers, and OCC override ports | Omitted from the native top; primitive DFT structs remain. |
| Serial TDR pins, `et_tdr_reg` shire override path, and PLL/DLL TDR select/test paths | Omitted; retained control moves through ESR/APB, `dft_t`, and `ram_cfg_t`. |
| Hard PLL/DLL macro analog/debug pins and Yin/Yang debug pins | Omitted; functional clocks are inputs from the SoC clock controller. |
| Third-party debug/status/message fabric and sensor scan/analog pins | Omitted; retained Ainekko-owned run control/status remains through DM control, BPAM/status-tree, APB/ESR, and native interrupt/status seams. |
| Original level-shifter hard-macro instances in reset gaskets | Omitted as source-code compatibility surfaces; voltage direction is represented by HV/LV DFT structs and FIFO primitive direction. |

## Source evidence

Original source evidence:

- `rtl/libs/mems_and_fifos/et_cdc_sync.v:23-66` defines the resettable
  `SYNC_STAGES` CDC flop chain; `:209` asserts the intended two-stage setting.
- `rtl/shire/shire_channel/shire_dmctrl.v:20-28` uses `et_cdc_sync` for the DM
  control write-enable metastability filter.
- `rtl/shire/shire_channel/shire_channel_wrap.v:904-914` and `:963-975` use
  `et_cdc_sync` for output interrupt/status capture.
- `rtl/shire/shire_channel/orig/shire_channel.v:717-725` uses `et_cdc_sync` for
  `dft__mbist_en`; `:1241-1260` instantiates the shire TDR override register.
- `rtl/shire/shire_channel/shire_pll_wrapper.v:119-141`, `:192-197`,
  `:233-234`, `:574-603`, `:637-651`, and `:668-695` show reset sync, scalar
  CDC, OCC insertion, TDR muxing, clock-gate control sync, and SRAM DFT clock
  generation in the original clock wrapper.
- `rtl/libs/mems_and_fifos/sync_rst.v:10-39` and `sync_rstn.v:10-40` define the
  original two-stage reset synchronizers.
- `rtl/libs/rf_latches/sys_gasket_hiv.v:4-129`, `sys_gasket_reset.v:4-157`,
  `sys_gasket_noc_reset_sync.v:4-30`, and `sys_gasket_noc_reset.v:4-38` define
  the original reset/voltage manager behavior.
- `rtl/libs/rf_latches/et_clk_mux2.v:4-18` and
  `rtl/libs/rf_latches/et_dft_occ_insert.v:8-38` define the original clock mux
  and OCC insertion anchor.
- `rtl/libs/registers/et_tdr_reg.v:3-37` defines the original serial TDR
  register.

Current project evidence:

- `docs/compute_shire_contract.md:176-205` freezes the native DFT shape and
  explicitly omits scan/OCC/TDR surfaces while retaining primitive DFT ports.
- `docs/compute_shire_contract.md:254-264` assigns native-seam, leaf, channel,
  and wrapper responsibilities to downstream tasks.
- `docs/compute_shire_contract.md:274-275` documents omission of shire DFT hub,
  OCC, serial TDR, and PLL/DLL hard-macro surfaces.
- `hw/ip/tech_generic/README.md:22-35` lists the existing clock, reset, RAM, and
  CDC FIFO technology seams; `:244-288` documents their contracts.
- `mk/prim.mk:38` selects `prim_clk_mux`; `:58-62` select `prim_rst_sync` and
  the async/semisync FIFO primitives.
- `hw/ip/shirecache/rtl/shirecache_top.sv:162-180` shows current reset-repeat
  translation to `prim_rst_sync`.
- `hw/ip/shirecache/rtl/shirecache_mesh_master.sv:92-99` shows the existing
  private scalar CDC style for an `et_cdc_sync` replacement.
- `hw/ip/shirecache/README.md:289-296` documents inline CDC for scalar sync and
  FIFO primitives for payload CDC.
- `hw/ip/neigh_channel/rtl/rst_repeat.sv:4-32` adapts the legacy active-high
  reset repeater wrapper to `prim_rst_sync`.
- `hw/ip/neigh_channel/rtl/sys_gasket_lov.sv:81-159` and
  `sys_gasket_lov_sync_rst.sv:5-30` show reset-manager-local two-stage reset
  synchronization, followed by local reset repeaters.
- `hw/ip/neigh_voltage_cross/rtl/neigh_lo_voltage_cross.sv:65-96` shows current
  reset repeat and semi-synchronous FIFO mapping with `dft_hv_i`/`dft_lv_i`.
