# prim_write_commit_en

Generic/default implementation of `prim_write_commit_en`.

## System role

`prim_write_commit_en` is an enabled state-commit primitive for write-driven
updates.

Use it when:

- state changes only on an explicit write/commit event
- the pattern belongs to a write pipeline rather than a general latch network
- the stored value does not need its own reset contract

Do not use it as a generic replacement for arbitrary high-phase latch sites.

## Contract

- when the commit-enable condition is active, `q_o` updates from `d_i`
- otherwise `q_o` holds its previous value

The generic implementation preserves the original high-phase write-commit
behavior directly inside this primitive.

RTL: `rtl/prim_write_commit_en.sv`
