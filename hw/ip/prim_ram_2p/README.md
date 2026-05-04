# prim_ram_2p

Dual-port RAM wrapper.

Behavioral (generic) implementation uses inferred dual-port RAM with registered read outputs (1-cycle latency per port). Both ports are symmetric (read + write capable). For ASIC, swap for the foundry dual-port SRAM macro.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 32 | Data width in bits |
| `Depth` | `int unsigned` | 256 | Number of entries |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `a_req_i` | input | 1 | Port A request enable |
| `a_we_i` | input | 1 | Port A write enable |
| `a_addr_i` | input | `$clog2(Depth)` | Port A address |
| `a_wdata_i` | input | `Width` | Port A write data |
| `a_rdata_o` | output | `Width` | Port A read data (registered) |
| `b_req_i` | input | 1 | Port B request enable |
| `b_we_i` | input | 1 | Port B write enable |
| `b_addr_i` | input | `$clog2(Depth)` | Port B address |
| `b_wdata_i` | input | `Width` | Port B write data |
| `b_rdata_o` | output | `Width` | Port B read data (registered) |
| `cfg_i` | input | `ram_cfg_t` | SRAM timing configuration |
| `dft_i` | input | `dft_t` | DFT control |
| `alert_o` | output | 1 | ECC error alert (future) |

## Read-during-write behavior

Simultaneous read and write to the same address on the same port: read returns old value (read-first).

Simultaneous access to the same address on different ports (one reading, one writing): read returns old value. This is the standard FPGA BRAM behavior. For ASIC dual-port SRAMs, this may be undefined — avoid in functional code.

## Differences from CORE-ET `gen_mem2p`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Same-addr collision | Returns `'x` in simulation | Returns old value (read-first) | Defined behavior, FPGA compatible |
| `WRITE_THROUGH` param | Optional forwarding | Not implemented (add if needed) | Simplicity |
| ECC | `ecc_err` XOR | `alert_o` sideband | Clean interface |
| Config | Implicit via `esr_shire_cache_ram_cfg_t` | `ram_cfg_pkg::ram_cfg_t cfg_i` | Standardized |

## Tests

```bash
make -C dv test
```
