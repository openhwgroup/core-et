# prim_write_commit_rst_en

ECP5 implementation of `prim_write_commit_rst_en`.

Current mapping:

- implemented as a falling-edge write-commit capture with enable and reset
- intended for write-driven state updates whose stored value has a defined
  reset state
- does not depend on the raw resettable high-phase latch primitive

Contract:

- when `rst_i` is asserted, `q_o` is forced to `ResetValue`
- otherwise, when `en_i` is active, `q_o` commits `d_i`
- otherwise `q_o` holds its previous value

RTL: `rtl/prim_write_commit_rst_en.sv`
