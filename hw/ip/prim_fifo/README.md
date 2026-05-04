# prim_fifo

Generic synchronous FIFO matching CORE-ET `gen_fifo`.

This is the translation target for original blocks that still depend on the
exact `gen_fifo` contract rather than the narrower `prim_fifo_reg` /
`prim_fifo_sync` interfaces.

## What It Does

- Stores `Depth` elements of width `Width`
- Exposes registered `full_o` / `valid_o`
- Exposes combinational lookahead `full_next_o` / `valid_next_o`
- Preserves raw push/pop semantics from the original:
  pointer and occupancy state move on asserted `push_i` / `pop_i`, not on an
  inferred ready/valid handshake
- Supports both storage styles from the original:
  - `UseLatches=0`: plain FF/LUTRAM-style array
  - `UseLatches=1`: latch-timed RF write path via `push_early_i`

## Parameters

| Parameter | Default | Meaning |
|-----------|---------|---------|
| `Width` | `32` | Element width in bits |
| `Depth` | `32` | Number of elements |
| `UseLatches` | `0` | Preserve original latch-timed storage path |

## Ports

| Port | Dir | Description |
|------|-----|-------------|
| `clk_i` | in | Clock |
| `rst_ni` | in | Active-low async reset |
| `push_early_i` | in | Low-phase write-data preview used only when `UseLatches=1` |
| `push_i` | in | Push request |
| `push_data_i` | in | Data to enqueue |
| `full_o` | out | Registered full flag |
| `full_next_o` | out | Combinational next-cycle full prediction |
| `pop_i` | in | Pop request |
| `pop_data_o` | out | Current head data |
| `valid_o` | out | Registered valid flag |
| `valid_next_o` | out | Combinational next-cycle valid prediction |

## Differences from CORE-ET `gen_fifo`

| Aspect | Original | New |
|--------|----------|-----|
| Reset | active-high `reset` | active-low `rst_ni` |
| Macro style | `RST_FF`, `RST_EN_FF`, `LATCH_P2` | explicit `always_ff` + named primitives |
| Latch storage path | `rf_latch_1r_1w` + macro latch | `prim_rf_1r1w_preview` |

Functional timing and pointer/count behavior are preserved.

## Tests

```bash
make -C hw/ip/prim_fifo/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_fifo test
```
