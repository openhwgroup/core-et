# prim_fifo_async_hiv / prim_fifo_async_lov

Asynchronous CDC FIFOs for clock domain crossing between voltage domains.

These are **technology primitives** — implementations live in per-module technology directories such as `hw/ip/tech_generic/prim_fifo_async_hiv/rtl/` and will differ per target technology (ASIC implementations use foundry-specific level shifters and CDC cells).

| Module | Write domain | Read domain | Replaces |
|--------|-------------|-------------|----------|
| `prim_fifo_async_hiv` | High-voltage (e.g. shire cache) | Low-voltage (e.g. NOC) | `vcfifo_wr_hiv_gcd` |
| `prim_fifo_async_lov` | Low-voltage (e.g. NOC) | High-voltage (e.g. shire cache) | `vcfifo_wr_lov_gcd` |

The only functional difference is which DFT domain signal (`dft_hv_i` vs `dft_lv_i`) maps to the write vs read reset bypass. On ASIC, the level shifter placement also differs.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `Width` | 32 | Data width |
| `Depth` | 8 | Number of entries |
| `SyncStages` | 2 | CDC synchronizer depth |

## Ports

| Port | Direction | Description |
|------|-----------|-------------|
| `clk_wr_i` / `rst_wr_ni` | input | Write clock and reset |
| `push_i` / `wdata_i` / `ready_o` | in/in/out | Write interface |
| `clk_rd_i` / `rst_rd_ni` | input | Read clock and reset |
| `pop_i` / `rdata_o` / `valid_o` | in/out/out | Read interface |
| `dft_hv_i` / `dft_lv_i` | input | Per-domain DFT control |

## Tests

Unit test and cosim in this directory test the `_hiv` variant.

```bash
make -C dv test
```
