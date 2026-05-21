// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# neigh_channel

`neigh_channel` is the translated CORE-ET neighborhood integration layer. It
connects the low-voltage neighborhood datapath, minion L2 request/response
fifos, shared I-cache/PTW resources, fast-local barrier, cooperative tensor
load/store paths, debug/status monitor routing, APB muxing, ESRs, and high/low
voltage crossing wrappers.

## Parameters

| Parameter | Description |
| --- | --- |
| `EnableGfx` | Preserves the original optional TBOX/GFX routing path. `0` ties the path off. |
| `StubMinions` | Preserved top-level compatibility mask for future minion stubbing. |

## Port interface

The module preserves the original `neigh_channel` protocol surface with project
naming and typed packed structs. Major port groups are:

| Group | Direction | Protocol / purpose |
| --- | --- | --- |
| Clocks/resets | input/output | High-voltage shire clock/resets in; generated low-voltage minion/TBOX clocks and synchronized active-low reset outputs out. |
| DFT/reset control | input/output | `dft_pkg::dft_t` high-voltage control in, LV DFT struct out, plus the original shire TDR reset override controls. |
| Shire/ESR configuration | input/output | Shire/neighborhood IDs, thread enables, feature bits, memory override, MPRoT, VM page size, boot address, DLL delay-estimator controls/status. |
| Shire-cache ET-link | input/output | Neighborhood request valid/info to shire cache, response ready back, and response valid/info from shire cache. |
| L2 dcache/fill/evict | input/output | Per-minion miss/evict request valid/info/ready and L2 response valid/info/ready. |
| Shared I-cache/PTW | input/output | Frontend I-cache requests/responses, SRAM request/response sideband, PTW request/response arrays, SATP/MATP and TLB invalidation inputs. |
| APB/debug/PMU | input/output | Shire APB request/response muxing, per-minion debug inputs/outputs, PMU read/write/count sidebands, and status-monitor debug buses. |
| Neighborhood cooperation | input/output | Fast-local barrier, local-message network, cooperative TLoad done/ready channels, and tensor-store buffer paths. |
| TBOX | input/output | TBOX reset/ID/control/status, ET-link request/response, APB, debug, and BPAM run-control sidebands. |

Ready/valid payloads are unchanged from the original CORE-ET module: producers
hold payload stable while valid is asserted until the matching ready is observed.
Reset outputs are active-low at the translated boundary; the cosim wrapper
inverts them when comparing against the original active-high reset outputs.

## Integration notes

Instantiate one `neigh_channel` per neighborhood. Drive high-voltage shire-side
clock/reset and high-voltage DFT controls through the provided inputs. The
module produces the low-voltage minion/TBOX clock outputs and synchronized reset
outputs for downstream minion, TBOX, and I-cache logic.

All RAM, FIFO, reset, and voltage-crossing helper behavior is kept behind project
primitive abstractions or local compatibility adapters. The local `gen_fifo`,
`rst_repeat`, semisync FIFO, level-shifter, arbitration, and sys-gasket adapters
exist only to preserve original `neigh_channel` integration timing; the FIFO and
reset adapters wrap the already-cosimmed `prim_fifo`, semisync FIFO, and
`prim_rst_sync` primitives and should not be used as new public primitives
outside this translated block.

## Design assumptions

- `clk_i` and `clk_shire_i` may be tied in single-clock simulation/FPGA bring-up;
  the voltage-crossing FIFOs still preserve the original push/pop-domain reset
  separation.
- `dft_hv_i = '0` and RAM/DFT config fields tied low are the normal functional
  operating mode.
- `StubMinions` is retained for interface compatibility; this translation does
  not add new minion-stubbing behavior beyond what lower blocks already expose.

## Verification

- Unit smoke test: `hw/ip/neigh_channel/dv` performs 21 explicit idle/reset
  checks through the full translated integration shell.
- RTL cosim: `dv/rtlcosim/neigh_channel` compares all outputs as 512 64-bit
  chunks for 5,000 compared cycles after a reset/DFT exercise warm-up, for
  2,560,000 output comparisons against the original CORE-ET RTL.

## Differences from original CORE-ET

| Difference | Rationale |
| --- | --- |
| Active-low reset ports | Project-wide async-assert/sync-deassert reset convention. |
| DFT struct output for LV domain | Consolidates CORE-ET individual DFT reset signals into `dft_pkg::dft_t`. |
| Technology wrappers | Original level shifter/FIFO/reset helper instances are translated through project primitives or local compatibility adapters. |
| SystemVerilog packages/types | Replaces include-file macros and anonymous vectors with package constants and packed structs. |
