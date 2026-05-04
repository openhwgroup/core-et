# prim_write_commit_en

Behavior-named write-commit primitive without an intrinsic reset value.

This is the public primitive-family entry for the technology-specific
`prim_write_commit_en` implementations in:

- `hw/ip/tech_generic/prim_write_commit_en/rtl/`
- `hw/ip/tech_ecp5/prim_write_commit_en/rtl/`

Use it for a narrowed write-pipeline seam where state updates happen only on an
explicit write/commit event and the stored value does not carry its own reset
contract.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 1 | Width of the committed state |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Base clock for the commit stage |
| `en_i` | input | 1 | Commit enable |
| `d_i` | input | `Width` | Commit payload |
| `q_o` | output | `Width` | Held committed state |

## Tests

```bash
make -C dv
ORIG_ROOT=/path/to/etcore-soc make -C ../../../../dv/rtlcosim/prim_write_commit_en test
```
