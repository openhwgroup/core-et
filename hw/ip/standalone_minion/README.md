<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# standalone_minion

`standalone_minion` is the translated CORE-ET single-Minion standalone shell. It
wraps the full translated `minion_top` (default real `vpu_top` path), SPIO ESRs,
APB debug mux, I-cache, neighborhood miss/evict/fill queues, shared PTW, ECO
ports, and ET-Link request/response glue.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `ShireId` | `254` | Local shire ID value used by the I-cache/standalone shell. |
| `DftScanWidth` | `12` | Preserved scan-chain vector width. Scan data is tied idle in this abstraction. |
| `StandaloneApbAddrWidth` | `16` | Width of the BPAM APB master address. |
| `NumMissAgents` | `4` | Miss arbitration agents: D-cache miss, D-cache evict, I-cache miss, PTW miss. |
| `MissFifoDepth` | `3` | Depth of the serialized ET-Link miss request FIFO. |

## Interfaces

- `clk_i`, `rst_ni`, `rst_debug_ni`, `rst_non_debug_ni`: base clock plus the
  translated global, debug, and non-debug reset domains. The wrapper derives the
  original warm reset as `rst_ni & rst_non_debug_ni` and the Minion debug reset
  as `rst_ni & rst_debug_ni`.
- `dft_i`, `dft_sram_clk_i`: project DFT abstraction and SRAM clock override.
  The current project DFT bundle does not carry scan-chain data, so
  `dft_scanout_o` and `dft_occ_scanout_o` are tied idle. Top-level RAM
  inhibit/clock-override controls are kept on the standalone I-cache data RAM
  wrapper, while the wrapped Minion receives only the scan/reset subset; this
  preserves the original shell's tied-off `minion_top.dft__cntl` connection.
- `eco_i`/`eco_o`: ECO spare ports through `prim_eco_ports`.
- Interrupt, boot, feature, memory-override, debug, SPIO, and trace ports drive
  the wrapped `minion_top`.
- I-cache error log, BIST, bypass, and RAM-config ports drive the translated
  I-cache and data RAM wrappers.
- `etl_req_*` / `etl_rsp_*`: single ET-Link request/response interface used by
  D-cache miss/evict, I-cache miss, and PTW traffic.
- `apb_*`: 64-bit BPAM APB debug window routed by `sp_apb_mux` to Minion debug,
  SPIO ESR, I-cache control, and I-cache data targets.

## APB integration notes

The APB window follows the original `sp_apb_mux` decode. Common smoke-test
addresses are:

- `0x0000`: Minion debug region
- `0x3002`: SPIO `SPDMCTRL`
- `0x3003`: SPIO `SPHASTATUS`
- `0xa000`: I-cache debug/control region
- `0xc000`: I-cache data RAM region

Drive legal APB setup/access phases (`psel=1, penable=0` followed by
`psel=1, penable=1`) when writing or reading shell registers.

## Design constraints

- The wrapped `minion_top` uses its default `VpuEn=1`, so this standalone shell
  targets the real translated VPU integration. The `VpuEn=0` null-VPU path is
  only for internal Minion bring-up tests and is not a standalone shell target.
- All I-cache/data memories are reached through translated RAM wrappers and
  receive `ram_cfg_i` plus DFT SRAM clock override controls.
- ET-Link line-write responses are routed to the same Minion, I-cache, and PTW
  response agents as the original shell.

## Differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Active-low `rst_*_ni` ports replace active-high `reset`, `reset_debug`, and `reset_non_debug`. | Project reset convention; reset-domain separation is preserved. |
| `dft_pkg::dft_t` replaces individual functional DFT control signals. | Project DFT abstraction. Scan-chain outputs are tied idle because the shared `dft_t` has no scan data payload. |
| Top-level DFT RAM inhibit and SRAM clock-override fields are not forwarded into the wrapped `minion_top`. | The original standalone shell forwarded Minion scan/reset DFT controls but tied `minion_top.dft__cntl`, test mode, clock-gate, and scan-chain controls to zero; RAM DFT controls remain live only at the standalone I-cache data RAM wrapper. |
| `ram_cfg_pkg::ram_cfg_t` replaces CORE-ET SRAM config structures. | Technology-abstracted RAM timing/test configuration. |
| Technology primitives (`prim_clk_mux`, `prim_eco_ports`, RAM wrappers, FIFOs, arbiters) replace foundry/library-specific cells. | FPGA/ASIC portability while preserving cycle behavior. |
| lowRISC-style port names and packages replace CORE-ET include macros. | Repository coding style and Verilator compatibility. |

## DV

Unit DV lives in `hw/ip/standalone_minion/dv/` and runs a shell smoke test with
APB/SPIO access, reset/DFT/ECO checks, VPU debug visibility, ET-Link readiness,
and I-cache request visibility.

RTL co-simulation lives in `dv/rtlcosim/standalone_minion/` and compares all
standalone outputs cycle-by-cycle against
`rtl/shire/standalone_minion/standalone_minion.v` from `ORIG_ROOT`.
