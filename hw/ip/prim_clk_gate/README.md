# prim_clk_gate

Clock gating cell with DFT override.

Behavioral (generic) implementation uses a latch-based ICG model: enable is latched on the low phase of the clock, then ANDed with the clock on the high phase. This produces a glitch-free gated clock. In scan mode (`dft_i.scanmode=1`), the clock is forced on.

For ASIC tapeout, swap this module for the foundry ICG cell. For FPGA, synthesis tools convert the pattern to clock enable automatically.

## Ports

| Port | Direction | Description |
|------|-----------|-------------|
| `clk_i` | input | Input clock |
| `en_i` | input | Clock enable |
| `dft_i` | input | DFT control — forces clock on in scan mode |
| `clk_o` | output | Gated clock |

## Differences from CORE-ET `et_clk_gate`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| DFT port | `te` (test enable, single bit) | `dft_pkg::dft_t` struct | Consistent DFT interface |
| ASIC cell | `HDBULT08_CKGTPLT_V7Y2_2` (foundry cell name in RTL) | Behavioral latch model | Technology abstraction — foundry cell swapped at build time |
| Naming | `et_clk_gate` | `prim_clk_gate` | Naming convention |
