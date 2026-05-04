# prim_eco_ports

Generic/default implementation of `prim_eco_ports`.

## System role

`prim_eco_ports` is the technology seam for the original CORE-ET
`et_eco_ports` wrapper used in top-level integration blocks. Functionally the
generic body is intentionally trivial:

- `eco_o` is tied low
- `eco_i` is preserved in the logic cone so spare pins stay visible to tooling

This is not a datapath primitive. It is the insertion point for future
technology-specific spare/ECO buffering in ASIC flows.

## Contract

- `eco_o` is always all-zero.
- `eco_i` has no functional effect on `eco_o`.
- The generic implementation must still consume `eco_i` so the spare inputs are
  not optimized away as completely dead ports.

## Why generic looks like this

- It matches the original non-std-cell `et_eco_ports` fallback behavior.
- It keeps simulation and FPGA builds simple.
- It leaves a clean override point for a future ASIC implementation that may
  need preserved buffers or foundry-specific spare-cell hookup.

RTL: `rtl/prim_eco_ports.sv`
