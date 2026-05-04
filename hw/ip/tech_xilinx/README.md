# tech_xilinx

Xilinx FPGA implementations of technology primitives.

## Status

This technology tree is currently untested. Treat these implementations as
reference-only sketches for a future Xilinx target, not as supported production
RTL.

Do not use `TECH := xilinx` results for signoff, resource planning, or claims of
FPGA support until each primitive has dedicated lint, unit-test, cosim, and
synthesis coverage on the intended Xilinx flow.

Optimized for AMD 7-series and UltraScale/UltraScale+ targets through Yosys
`synth_xilinx`.

This tree should only contain modules that are meaningfully different from the
generic contract:
- clock-path primitives that must avoid latch-based clock logic or route
  through dedicated Xilinx clocking resources
- storage primitives that need explicit Xilinx resource policy
- register-file seams where the logical contract is not a native BRAM shape and
  needs an owned Xilinx realization

If a primitive does not need a real Xilinx-specific body, it should not exist
here and `mk/prim.mk` should fall back to `tech_generic`.

Each primitive lives in its own module directory:

```
tech_xilinx/
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
| `prim_clk_mux` | Clock mux for SRAM DFT override | Uses `BUFGCTRL` instead of a LUT mux in the clock path |
| `prim_ram_1p` | Single-port RAM | `(* ram_style = "block" *)` for explicit BRAM inference |
| `prim_ram_2p` | Dual-port RAM | `(* ram_style = "block" *)` for explicit BRAM inference |
| `prim_rf_1r1w` | Latch-timed 1R1W register file | Uses LUTRAM-friendly async-read/sync-write storage with `ram_style="distributed"` |
| `prim_rf_1r1w_diff` | Mixed-width latch-timed 1R1W register file | Uses an explicit FF-bank because the overlapping aligned read view is not a native Xilinx RAM port shape |
| `prim_rf_2r1w` | Latch-timed 2R1W register file | Duplicates distributed storage for the two async read ports and keeps parallel taps in a shadow register bank |

Primitives not listed here fall back to `tech_generic` via `mk/prim.mk`.

Notably, `prim_rst_sync` stays on the generic implementation for Xilinx. The
generic body already matches the Xilinx-friendly reset contract: asynchronous
assertion, synchronous release. A dedicated Xilinx wrapper around XPM reset
macros would be a library choice, not a distinct storage or clocking seam in
the current open-source flow.

`prim_fifo_async_hiv` and `prim_fifo_async_lov` also stay on the generic
fallback. AMD has XPM asynchronous FIFOs, but those would tie this target to a
Vivado-specific library interface. In this repository the real technology seam
for those blocks is still logical CDC plus future ASIC level-shifter insertion,
not Xilinx-specific FIFO macros.

## Usage

Reference only. `TECH := xilinx` may select these files through `mk/prim.mk`,
but that path is not currently validated.
