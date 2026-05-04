# prim_write_commit_rst_en

Behavior-named write-commit primitive with an explicit reset value.

This is the public primitive-family entry for the technology-specific
`prim_write_commit_rst_en` implementations in:

- `hw/ip/tech_generic/prim_write_commit_rst_en/rtl/`
- `hw/ip/tech_ecp5/prim_write_commit_rst_en/rtl/`

Use it for a narrowed write-pipeline seam where state updates happen only on an
explicit write/commit event and the stored state must return to a defined reset
value when reset is asserted.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 1 | Width of the committed state |
| `ResetValue` | `logic [Width-1:0]` | `'0` | Value driven while reset is asserted |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Base clock for the commit stage |
| `rst_i` | input | 1 | Active-high reset for the stored state |
| `en_i` | input | 1 | Commit enable |
| `d_i` | input | `Width` | Commit payload |
| `q_o` | output | `Width` | Held committed state |

## Tests

```bash
make -C dv
ORIG_ROOT=/path/to/etcore-soc make -C ../../../../dv/rtlcosim/prim_write_commit_rst_en test
```
