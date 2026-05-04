# prim_ram_1p

Single-port RAM wrapper.

Behavioral (generic) implementation uses inferred RAM (`logic mem[]`) with registered read output (1-cycle read latency). For ASIC, swap for the foundry SRAM macro with ECC and BIST hooks.

All RAM instantiations in functional RTL must go through this wrapper — never use bare `logic mem[]`.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 32 | Data width in bits |
| `Depth` | `int unsigned` | 256 | Number of entries |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `req_i` | input | 1 | Request enable (gates read and write) |
| `we_i` | input | 1 | Write enable (when `req_i` is high) |
| `addr_i` | input | `$clog2(Depth)` | Address |
| `wdata_i` | input | `Width` | Write data |
| `rdata_o` | output | `Width` | Read data (registered, 1-cycle latency) |
| `cfg_i` | input | `ram_cfg_t` | SRAM timing configuration (ASIC) |
| `dft_i` | input | `dft_t` | DFT control |
| `alert_o` | output | 1 | ECC error alert (future, tied to 0 in behavioral) |

## Read behavior

- When `req_i=1` and `we_i=0`: `rdata_o` is updated with `mem[addr_i]` on the next clock edge.
- When `req_i=1` and `we_i=1`: write occurs AND `rdata_o` is updated (read-during-write returns old value).
- When `req_i=0`: `rdata_o` holds its previous value.

## Differences from CORE-ET `gen_mem1p`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| ECC | `ecc_err` XOR on output | `alert_o` sideband (tied to 0 in behavioral) | Clean ECC interface |
| Config | `esr_shire_cache_ram_cfg_t` threading | `ram_cfg_pkg::ram_cfg_t cfg_i` | Standardized config struct |
| DFT | No explicit DFT port | `dft_pkg::dft_t dft_i` | Consistent DFT interface |
| ASIC swap | `ifdef ET_USE_ASIC_MEMS` with foundry cell name | Technology library swap at build time | No foundry names in RTL |

## Tests

```bash
make -C dv test
```
