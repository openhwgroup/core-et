# prim_write_preview_en

Behavior-named write-preview primitive.

This is the public primitive-family entry for the technology-specific
`prim_write_preview_en` implementations in:

- `hw/ip/tech_generic/prim_write_preview_en/rtl/`
- `hw/ip/tech_ecp5/prim_write_preview_en/rtl/`

Use it for a narrowed write-pipeline seam where data or a write qualifier must
be previewed one phase earlier and held for a later write/commit point on the
same base clock.

Do not use it as a generic replacement for arbitrary latch-heavy logic.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 1 | Width of the previewed bus |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Base clock for the preview stage |
| `en_i` | input | 1 | Preview enable |
| `d_i` | input | `Width` | Preview payload |
| `q_o` | output | `Width` | Held preview value |

## Tests

```bash
make -C dv
ORIG_ROOT=/path/to/etcore-soc make -C ../../../../dv/rtlcosim/prim_write_preview_en test
```
