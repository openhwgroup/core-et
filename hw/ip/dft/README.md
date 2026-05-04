# dft

DFT (Design for Test) type definitions package.

Provides a consolidated `dft_t` struct that replaces the CORE-ET convention of individual `dft__*` signals. Modules that interact with clocks, resets, or SRAMs accept `dft_t` as an input. In normal operation, `dft_i = '0`.

## Package: `dft_pkg`

```systemverilog
import dft_pkg::*;
```

### Types

| Type | Width | Description |
|------|-------|-------------|
| `dft_t` | 5 bits | DFT control struct |

### Fields of `dft_t`

| Field | Width | Description |
|-------|-------|-------------|
| `scanmode` | 1 | Scan mode active — bypass functional clocks/resets |
| `scan_reset_n` | 1 | Scan reset (active-low), used when `scanmode=1` |
| `sram_clk_override` | 1 | Enable DFT SRAM clock override (selects `dft_sram_clk_i` via `prim_clk_mux`) |
| `ram_rei` | 1 | RAM read enable inhibit (DFT) |
| `ram_wei` | 1 | RAM write enable inhibit (DFT) |

Note: The DFT SRAM override clock itself (`dft_sram_clk_i`) is passed as a separate port because it's a clock signal and cannot be part of a packed data struct.

### Differences from CORE-ET

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Interface style | Individual signals (`dft__reset_byp`, `dft__reset`, `dft__ram_wei`, `dft__ram_rei`, `dft__sram_clock`, etc.) | Single packed struct `dft_t` | Clean, one port instead of many |
| Scope | Per-module ad-hoc selection of DFT signals | Uniform struct on every DFT-sensitive primitive | Consistent interface |
