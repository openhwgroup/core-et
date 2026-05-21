# prim_fifo_async_lov

Generic/default implementation of `prim_fifo_async_lov`.

## System role

`prim_fifo_async_lov` is the CDC primitive for transfers written in the
low-voltage domain and consumed in the high-voltage domain. In the current tree
it is used at shirecache mesh and bridge boundaries for NOC-to-shire traffic.

Like `prim_fifo_async_hiv`, the LV/HV split is part of the primitive contract,
not just a comment. It preserves the future ASIC insertion point for
level-shifter placement and domain-specific test behavior.

## Contract

- `push_i`, `wdata_i`, and `ready_o` are synchronous to `clk_wr_i`.
- `pop_i`, `valid_o`, and `rdata_o` are synchronous to `clk_rd_i`.
- `Depth` is the usable FIFO capacity. Internal Gray counters use a
  power-of-two modulo width, while data addresses wrap at exactly `Depth`, so
  non-power-of-two depths keep the original occupancy and payload order
  behavior.
- `rdata_o` is show-ahead: when `valid_o=1`, it reflects the current head
  entry before `pop_i`.
- `dft_lv_i` controls reset bypass on the write/LV side and `dft_hv_i`
  controls reset bypass on the read/HV side.
- `ready_o` and `valid_o` are masked low while their respective effective reset
  is asserted, matching the original reset-visible handshake behavior.
- The generic model does not include physical level-shifting behavior; it only
  defines the logical FIFO and per-domain reset-bypass contract.

## Why generic looks like this

- The generic body is a pure RTL Gray-pointer asynchronous FIFO because that is
  the simplest portable reference implementation. Standalone cosim covers it
  against `vcfifo_wr_lov_gcd`.
- A future ASIC implementation may use different cells on the LV and HV sides
  even though the generic body is logically symmetric.

RTL: `rtl/prim_fifo_async_lov.sv`
