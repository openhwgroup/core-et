<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_data_array

`icache_data_array` is the standalone-Minion I-cache data-array pipeline stage.
It encodes read/write requests for the SRAM wrapper, accepts line reads back from
the SRAM panels, corrects per-doubleword ECC, and returns the corrected cache
line plus SBE/DBE flags.

## Interface

### System

- `clk_i`, `rst_ni`
  Functional clock and active-low asynchronous reset.

### F1 request side

- `f1_read_req_valid_i`, `f1_read_req_way_i`, `f1_read_req_set_i`
  Read request entering the array pipe.
- `f1_write_req_valid_i`, `f1_write_req_way_i`, `f1_write_req_set_i`
  Write request entering the array pipe. Write requests take priority over reads,
  matching the original arbitration.

### F0 response side

- `f0_read_resp_early_valid_o`
  One-cycle-early indication that an SRAM response has been captured and ECC
  correction is in flight.
- `f0_read_resp_valid_o`
  Corrected response valid.
- `f0_read_resp_data_o`
  Corrected full cache line.
- `f0_read_resp_sbe_per_block_o`, `f0_read_resp_dbe_per_block_o`
  Per-64b ECC status for the returned line.

### SRAM wrapper side

- `f2_mem_req_write_o`, `f2_mem_req_addr_o`, `f2_mem_req_valid_o`,
  `f2_mem_req_ready_i`
  Outbound SRAM request handshake.
- `f0_mem_resp_dout_i`, `f0_mem_resp_valid_i`, `f0_mem_resp_ready_o`
  Inbound SRAM read response handshake.

## Integration notes

- The SRAM interface matches `icache_data_ram_wrap`.
- ECC correction uses `prim_ecc_dec`, preserving the original `ecc_corr`
  behavior per 64-bit block.
- The module does not gate read-response acceptance; `f0_mem_resp_ready_o` is
  always high, exactly like the original.

## Intentional differences from the original

| Original | Translation |
|----------|-------------|
| `clock`, active-high `reset` | `clk_i`, active-low `rst_ni` |
| `soc.vh` defines | package-based constants from `icache_geom_pkg` / `icache_pkg` |
| `ecc_corr` | `prim_ecc_dec` |

No intentional functional differences are introduced.
