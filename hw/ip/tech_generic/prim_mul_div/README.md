# prim_mul_div

Generic/default implementation of `prim_mul_div`.

## System role

`prim_mul_div` is the technology seam for the Minion integer mul/div block.
It owns the full external contract currently presented by
`intpipe_mul_div_top`:

- request-side backpressure via `req_ready`
- variable-latency completion via `resp_valid`
- one-cycle-early writeback reservation via `resp_valid_early`
- destination/data return via `resp_dest` and `resp_data`
- squash/kill handling

The public `intpipe_mul_div_top` module remains the intpipe-facing wrapper.
This primitive exists so FPGA or ASIC targets can replace only the mul/div
implementation while keeping the pipeline contract stable.

## Contract

- Ports, parameterization, and cycle-by-cycle request/response semantics match
  the current translated `intpipe_mul_div_top`.
- `resp_valid_early` remains a real part of the contract and must assert one
  cycle before the matching `resp_valid`.
- `req_ready` must deassert while the unit is busy or while an unconsumed
  response is outstanding.
- `kill` must preserve the current visible squash behavior, including any
  original-design quirks documented in `hw/ip/minion/BUGS.md`.

## Why generic looks like this

- The generic body preserves the current translated latch-based mul/div
  implementation exactly.
- The seam is structural: it creates a single ownership point for future
  FPGA-specific or ASIC-specific mul/div replacements without perturbing the
  surrounding intpipe logic.

RTL:
- `rtl/prim_mul_div.sv`
- `rtl/intpipe_mul_div_ctl.sv`
- `rtl/intpipe_mul_div_dp.sv`

The generic primitive owns the legacy translated latch-based helper modules
directly. `intpipe_mul_div_top` remains the public intpipe-facing wrapper, but
the implementation body now lives entirely under this primitive directory.
