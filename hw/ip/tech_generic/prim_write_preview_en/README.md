# prim_write_preview_en

Generic/default implementation of `prim_write_preview_en`.

## System role

`prim_write_preview_en` captures a staged preview copy of write payload data
before the later commit/update point on the same base clock.

Use it when:

- write payload data must be previewed and held stable for later commit logic
- the pattern is explicitly a write-stage seam
- the intent is narrower than a general low-phase latch primitive

Do not use it as a generic replacement for arbitrary low-phase latch sites.

## Contract

- when the preview-enable condition is active, `q_o` captures `d_i`
- otherwise `q_o` holds its previous value
- the previewed value is intended to feed later write-commit logic on the same
  base clock

The generic implementation preserves the original low-phase preview behavior
directly inside this primitive.

RTL: `rtl/prim_write_preview_en.sv`
