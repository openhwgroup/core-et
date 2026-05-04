# prim_rst_sync

Reset synchronizer: async assert, sync deassert, with DFT bypass.

When `rst_ni` goes low, `rst_no` asserts immediately (asynchronously). When `rst_ni` is released, `rst_no` deasserts on the next rising clock edge. In scan mode (`dft_i.scanmode=1`), the synchronizer is bypassed and `rst_no` follows `dft_i.scan_reset_n` directly.

## Ports

| Port | Direction | Description |
|------|-----------|-------------|
| `clk_i` | input | Clock |
| `rst_ni` | input | Reset input (active-low, async assert) |
| `dft_i` | input | DFT control — bypasses synchronizer in scan mode |
| `rst_no` | output | Reset output (active-low, sync deassert) |

## Differences from CORE-ET `rst_repeat`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Reset polarity | Active-high (input and output) | Active-low (`rst_ni`, `rst_no`) | Project coding style |
| DFT port | Individual `dft__reset_byp`, `dft__reset` | `dft_pkg::dft_t dft_i` | Consolidated DFT struct |
| ASIC cell | `rst_repeat_grp_cells` with foundry cells | Behavioral `always_ff` | Technology abstraction |

Post-reset functional behavior (async assert, sync deassert, DFT bypass) is identical.

## Tests

```bash
make -C dv test
```
