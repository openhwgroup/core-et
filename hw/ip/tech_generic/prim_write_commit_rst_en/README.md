# prim_write_commit_rst_en

Generic/default implementation of `prim_write_commit_rst_en`.

## System role

`prim_write_commit_rst_en` is an enabled state-commit primitive for
write-driven updates whose stored value has a defined reset state.

Use it when:

- state changes only on an explicit write/commit event
- the stored value must return to `ResetValue` when `rst_i` is asserted
- the pattern belongs to a write pipeline rather than a general latch network

## Contract

- when `rst_i` is asserted, `q_o` is forced to `ResetValue`
- otherwise, when the commit-enable condition is active, `q_o` updates from
  `d_i`
- otherwise `q_o` holds its previous value

The generic implementation preserves the original resettable high-phase
write-commit behavior directly inside this primitive.

RTL: `rtl/prim_write_commit_rst_en.sv`
