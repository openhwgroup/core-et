# prim_ram_2p

Generic/default implementation of `prim_ram_2p`.

## System role

`prim_ram_2p` is the dual-port SRAM replacement seam. Current users include:
- shirecache tag/state RAM wrapper in dual-port mode
- minion local RAM wrappers that preserve original 1R1W timing
- rbox depth buffer
- `prim_fifo_sram` when configured for two memory ports

Although the interface is symmetric, current system usage is predominantly
single-clock 1R1W: one port used for writes and the other for reads. As with
`prim_ram_1p`, most wrappers mask request and write enables with DFT SRAM
inhibit signals before driving the primitive.

## Contract

- Two clocked ports share one memory array and one root clock.
- Each read result is registered with one-cycle latency.
- When `a_req_i=0` or `b_req_i=0`, the corresponding read output holds its
  previous value.
- Read-versus-write to the same address on opposite ports is read-first in the
  generic model: the read side sees the old value.
- Simultaneous write-write to the same address is outside the supported
  contract and must be treated as undefined.
- Memory contents are not reset by `rst_ni`; reset is present only for wrapper
  interface stability across technologies.
- `cfg_i`, `dft_i`, and `alert_o` remain part of the wrapper contract even when
  unused by the generic body.

## Why generic looks like this

- The generic body is inferred `logic mem[]` storage because it is portable and
  easy to compare against the original `gen_mem2p` behavior.
- The generic implementation is intentionally simple, but any target-specific
  override must keep the current 1R1W usage efficient and semantically stable.
- FPGA targets may preserve the interface while biasing inference toward legal
  hard-RAM mappings.
- ASIC targets will replace the body with macro-specific wrappers and test/ECC
  integration.

RTL: `rtl/prim_ram_2p.sv`
