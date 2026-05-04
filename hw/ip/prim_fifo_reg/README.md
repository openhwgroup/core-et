# prim_fifo_reg

Register-based synchronous FIFO for small depths (1-4 entries).

Data at `rdata_o` is available combinationally when `valid_o` is high (zero-latency read). Includes an output register stage that enables direct push-to-output bypass when the FIFO is empty. Depth includes this output register.

## Differences from CORE-ET `gen_fifo_reg`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert | Project coding style |
| Clock gating | `en_ff_gated_struct` for memory | Plain `always_ff` with enable | No clock gating cells |
| Flip-flop macros | `RST_EN_FF`, `EN_FF` | Explicit `always_ff` | No macro dependencies |
| `gating_type_t` param | Controls clock gating strategy | Dropped | Not applicable without gated clocks |

Post-reset functional behavior (push/pop timing, level tracking, bypass paths) is identical.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 8 | Data width in bits |
| `Depth` | `int unsigned` | 4 | Total entries including output register |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `push_i` | input | 1 | Push data (write) |
| `wdata_i` | input | `Width` | Write data |
| `full_o` | output | 1 | FIFO is full |
| `pop_i` | input | 1 | Pop data (read) |
| `valid_o` | output | 1 | Read data valid |
| `rdata_o` | output | `Width` | Read data (head of FIFO) |
| `level_o` | output | `Depth+1` | One-hot occupancy level |

## Level encoding

`level_o` is a one-hot thermometer code:
- `level_o[0] = 1` → empty
- `level_o[Depth] = 1` → full
- Shifts left on push, right on pop

## Tests

```bash
make -C dv test
```
