# prim_clk_mux

Xilinx-specific implementation of `prim_clk_mux`.

Why this exists
- Clock muxing is a real technology seam on Xilinx devices.
- A plain behavioral mux is acceptable as the generic contract reference, but
  it should not be the actual Xilinx clock-path implementation.

How this implementation realizes the contract
- With `UseTechClockMux=1` (the default), the clock selection is routed through
  `BUFGCTRL`, the common Xilinx glitch-managed clock mux primitive across
  7-series and UltraScale families.
- The select is driven through `S0`/`S1`, which matches AMD's recommended
  glitch-free switching mode.
- `UseTechClockMux=0` forwards `clk0_i` and is only for explicit FPGA-only
  tied-off DFT paths.

Why it is implemented this way
- `prim_clk_mux` is currently used for SRAM DFT clock override, so this target
  still assumes the select changes only in a safe, quiescent test context.
- Even with that narrow usage, using a real clock primitive is the correct
  Xilinx-specific ownership point for this seam.

RTL: `rtl/prim_clk_mux.sv`
