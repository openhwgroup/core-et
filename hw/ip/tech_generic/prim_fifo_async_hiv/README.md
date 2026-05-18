# prim_fifo_async_hiv

Generic/default implementation of `prim_fifo_async_hiv`.

## System role

`prim_fifo_async_hiv` is the CDC primitive for transfers written in the
high-voltage domain and consumed in the low-voltage domain. In the current tree
it is used at shirecache mesh and bridge boundaries for shire-to-NOC traffic.

The HV/LV naming is intentional even on FPGA and in simulation. Today it
documents direction; later it is also the insertion point for level shifters
and domain-specific test handling.

## Contract

- `push_i`, `wdata_i`, and `ready_o` are synchronous to `clk_wr_i`.
- `pop_i`, `valid_o`, and `rdata_o` are synchronous to `clk_rd_i`.
- `Depth` is the usable FIFO capacity. Internal Gray counters use a
  power-of-two modulo width, while data addresses wrap at exactly `Depth`, so
  non-power-of-two depths keep the original full/empty and payload order
  behavior.
- `rdata_o` is show-ahead: when `valid_o=1`, it reflects the current head
  entry before `pop_i`.
- `dft_hv_i` controls reset bypass on the write/HV side and `dft_lv_i`
  controls reset bypass on the read/LV side.
- `ready_o` and `valid_o` are masked low while their respective effective reset
  is asserted, matching the original reset-visible handshake behavior.
- The generic model is only a transport and reset-bypass contract. It does not
  model any physical level shifting.

## Why generic looks like this

- The generic body is a pure RTL Gray-pointer asynchronous FIFO, which is easy
  to simulate and matches the intended logical behavior. Standalone cosim covers
  it against `vcfifo_wr_hiv_gcd`.
- Keeping the voltage-direction split in the interface preserves the correct
  system intent for future ASIC implementations even though the generic body is
  symmetric.

RTL: `rtl/prim_fifo_async_hiv.sv`
