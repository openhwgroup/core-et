# prim_write_commit_en

ECP5 implementation of `prim_write_commit_en`.

Current mapping:

- implemented as a falling-edge write-commit capture with enable
- intended for write-driven state updates on FPGA
- does not depend on the raw high-phase latch primitive

Contract:

- when `en_i` is active, `q_o` commits `d_i`
- otherwise `q_o` holds its previous value

RTL: `rtl/prim_write_commit_en.sv`
