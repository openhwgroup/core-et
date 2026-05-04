# tech_ice40

Lattice iCE40 FPGA implementations of technology primitives.

## Status

This technology tree is left for reference only. The current IP blocks in this
repository are too large for practical iCE40 validation, so these implementations
are not an actively tested or supported FPGA target.

Do not use `TECH := ice40` results for signoff, resource planning, or claims of
FPGA support unless the specific project has been reduced to an iCE40-sized
design and validated on the intended iCE40 flow.

Optimized for iCE40 synthesis via Yosys `synth_ice40`.

This tree should only contain modules that are meaningfully different from
the generic contract:
- latch-based clock primitives rewritten into iCE40-friendly FF patterns
- RAM wrappers that carry iCE40/Yosys inference policy or narrow the contract
  to what iCE40 EBRs can actually realize efficiently
- reset or storage seams that need an explicit iCE40-owned contract even when
  they still map onto ordinary LUT/FF fabric

If a primitive does not need a real iCE40-specific body, it should not exist
here and `mk/prim.mk` should fall back to `tech_generic`.

Each primitive lives in its own module directory:

```
tech_ice40/
  prim_clk_gate/
    README.md
    rtl/prim_clk_gate.sv
  ...
```

## Primitives

| Module | Description | Difference from generic |
|--------|-------------|------------------------|
| `prim_clk_gate` | Positive-phase clock gate | Samples enable with a negedge FF instead of a low-transparent latch |
| `prim_clk_gate_n` | Negative-phase clock gate | Samples enable with a posedge FF instead of a high-transparent latch |
| `prim_ram_1p` | Single-port RAM | Keeps the generic behavior but adds `ram_style="block"` to bias Yosys toward `SB_RAM40_4K` |
| `prim_ram_2p` | Dual-port RAM | Narrows the generic symmetric wrapper into the actual iCE40 EBR shape: port A write, port B read |
| `prim_rst_sync` | Reset repeater | Keeps the single-flop contract but makes the DFT-selected async reset source explicit |
| `prim_rf_1r1w` | Latch-timed 1R1W register file | Replaces the latch-array abstraction with an explicit FF bank and decoded write enables |
| `prim_rf_1r1w_diff` | Mixed-width latch-timed 1R1W register file | Uses an explicit FF bank with per-slice enables instead of latch-and-gate storage |
| `prim_rf_2r1w` | Latch-timed 2R1W register file | Uses an explicit FF bank while preserving dual combinational reads, zero rows, and parallel taps |

Primitives not listed here fall back to `tech_generic` via `mk/prim.mk`.

Notably, `prim_fifo_async_hiv` and `prim_fifo_async_lov` stay on the generic
implementation for iCE40. The current shirecache payload widths are far wider
than the 16-bit per-port EBR width, so a dedicated EBR-backed override would
cost dozens of blocks per FIFO and is not the right target-specific tradeoff.

`prim_clk_mux` also stays on the generic fallback for iCE40. That is
intentional: iCE40 does not have a general internal glitchless clock-mux
primitive, and this seam is only used for static SRAM DFT clock override rather
than live functional clock switching.

## Usage

Reference only. `TECH := ice40` may select these files through `mk/prim.mk`,
but current repository IPs are not validated as iCE40 targets.
