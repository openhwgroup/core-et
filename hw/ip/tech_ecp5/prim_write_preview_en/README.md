# prim_write_preview_en

ECP5 implementation of `prim_write_preview_en`.

Current mapping:

- implemented as a rising-edge write-preview capture
- intended for staged write payload data on FPGA
- does not depend on the raw low-phase latch primitive

Contract:

- captures a stable preview value when `en_i` indicates a write-preview event
- otherwise holds its previous value
- is intended to feed a later write-commit stage on the same base clock

RTL: `rtl/prim_write_preview_en.sv`
