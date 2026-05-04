# prim_ram_1p

Generic/default implementation of `prim_ram_1p`.

## System role

`prim_ram_1p` is the single-port SRAM replacement seam. Current users include:
- shirecache tag and data RAM wrappers
- shirecache tag/state wrapper in single-port mode
- rbox input FIFO backing storage
- `prim_fifo_sram` when configured for a single memory port

In the current design, callers usually apply DFT SRAM read/write inhibits before
the primitive by masking `req_i` and `we_i` with `dft_i.ram_rei` and
`dft_i.ram_wei`. The wrapper still carries `dft_i` and `cfg_i` because those
signals belong to the tapeout-facing RAM contract, even when the generic body
does not consume them.

## Contract

- One clocked request port with mutually exclusive read or write behavior.
- Read data is registered with one-cycle latency.
- When `req_i=0`, `rdata_o` holds its previous value.
- On a write cycle, memory contents update; the primitive does not define a
  same-cycle read result because reads and writes share one request port.
- Memory contents are not reset by `rst_ni`. Reset is present only to keep a
  stable interface across targets and wrappers.
- `cfg_i` is reserved for ASIC SRAM timing and assist controls.
- `dft_i` is reserved for DFT-visible SRAM control, even when the generic body
  ignores it.
- `alert_o` is the future ECC/error reporting path and is tied low in the
  generic implementation.

## Why generic looks like this

- The generic body is inferred `logic mem[]` storage because it is portable,
  easy to simulate, and easy to compare against the original `gen_mem1p`
  behavior.
- FPGA targets may keep the same semantics but add synthesis-policy hints.
- ASIC targets will eventually replace the body with macro-specific wrappers,
  ECC plumbing, and BIST/test integration.

RTL: `rtl/prim_ram_1p.sv`
