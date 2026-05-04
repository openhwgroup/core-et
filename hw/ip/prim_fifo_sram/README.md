# prim_fifo_sram

SRAM-backed FIFO with 1-cycle read latency.

Matches the behavior of the original CORE-ET `rbox_fifo`: write is acknowledged same cycle, read data appears one cycle after `rd_en_i` is asserted. No backpressure — the caller is responsible for tracking occupancy.

Uses `prim_ram_1p` (Ports=1) or `prim_ram_2p` (Ports=2) for the backing store, providing ASIC SRAM swap, ECC, BIST, and timing configuration hooks.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 64 | Data width in bits |
| `Depth` | `int unsigned` | 64 | Number of entries (must be power of 2) |
| `Ports` | `int unsigned` | 2 | 1 = single-port (read blocks write), 2 = dual-port |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `wr_en_i` | input | 1 | Write enable |
| `wr_data_i` | input | `Width` | Write data |
| `wr_done_o` | output | 1 | Write acknowledged (combinational) |
| `rd_en_i` | input | 1 | Read request |
| `rd_valid_o` | output | 1 | Read data valid (1 cycle after `rd_en_i`) |
| `rd_data_o` | output | `Width` | Read data |
| `ram_cfg_i` | input | `ram_cfg_t` | SRAM timing configuration |
| `dft_i` | input | `dft_t` | DFT control |

## Differences from CORE-ET `rbox_fifo`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert | Project coding style |
| Memory | `gen_mem2p`/`gen_mem1p` or foundry SRAM | `prim_ram_2p`/`prim_ram_1p` | Technology-abstracted RAM wrapper |
| Clock gating | `et_clk_gate` on SRAM ports | Handled inside `prim_ram_*` | Abstracted |
| DFT | `esr_shire_cache_ram_cfg` with DFT fields | `dft_pkg::dft_t dft_i` | Consolidated DFT struct |
| RAM config | `esr_shire_cache_ram_cfg` with timing fields | `ram_cfg_pkg::ram_cfg_t ram_cfg_i` | Standardized config |
| ECC | `ecc_err` XOR on `gen_mem2p` output | `alert_o` on `prim_ram_*` (future) | Clean sideband |
| Flip-flop style | `RST_FF` / `EN_FF` macros | Explicit `always_ff` | No macro dependencies |

Post-reset functional behavior (cycle-by-cycle timing, handshake semantics, pointer wrapping) is identical for both Ports=1 and Ports=2 modes.

## Tests

```bash
make -C dv test
```
