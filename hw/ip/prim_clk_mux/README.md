# prim_clk_mux

Glitch-free 2:1 clock multiplexer.

Selects between `clk0_i` (when `sel_i=0`) and `clk1_i` (when `sel_i=1`). Used for DFT SRAM clock override — in scan mode, the functional clock is replaced by a test clock.

Behavioral implementation is a simple mux. For ASIC tapeout, swap for the foundry glitch-free clock mux cell.

Implementation selection:
- `mk/prim.mk` exposes this primitive as `PRIM_CLK_MUX`
- The generic contract lives under `tech_generic/prim_clk_mux`
- `tech_xilinx/prim_clk_mux` maps the seam onto a real Xilinx clock mux
- `tech_ecp5/prim_clk_mux` can map the seam onto a scarce ECP5 `DCSC` when
  `UseTechClockMux=1`
- iCE40 intentionally falls back to the generic body because the current use is
  static SRAM DFT clock override rather than live functional clock switching

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `UseTechClockMux` | target-specific | Selects the real target clock-mux resource when available. A target may support `0` as an explicit FPGA-only passthrough for tied-off DFT paths that only ever use `clk0_i`. |

## Ports

| Port | Direction | Description |
|------|-----------|-------------|
| `clk0_i` | input | Clock input 0 (selected when `sel_i=0`) |
| `clk1_i` | input | Clock input 1 (selected when `sel_i=1`) |
| `sel_i` | input | Select |
| `clk_o` | output | Selected clock output |

## Differences from CORE-ET `et_clk_mux2`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| ASIC cell | Foundry glitch-free mux cell | Behavioral mux | Technology abstraction |
| Naming | `et_clk_mux2` | `prim_clk_mux` | Naming convention |
