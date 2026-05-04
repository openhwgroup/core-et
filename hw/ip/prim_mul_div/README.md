# prim_mul_div

Technology seam for the Minion integer mul/div unit.

This is the public primitive-family entry for the technology-specific
`prim_mul_div` implementations in:

- `hw/ip/tech_generic/prim_mul_div/rtl/`
- `hw/ip/tech_ecp5/prim_mul_div/rtl/`

The shared `intpipe_mul_div_top` wrapper remains the intpipe-facing module, but
the implementation body now lives under this primitive family.

## Contract

- request-side backpressure through `req_ready`
- one-cycle-early writeback reservation through `resp_valid_early`
- variable-latency completion through `resp_valid`
- result routing through `resp_data` and `resp_dest`
- kill/squash behavior matching the selected technology implementation

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Root clock |
| `rst_ni` | input | 1 | Active-low reset |
| `dft_i` | input | `dft_t` | DFT override bundle |
| `req_ready` | output | 1 | Request acceptance/backpressure |
| `req_valid` | input | 1 | Request valid |
| `req_valid_early` | input | 1 | One-cycle-early request preview |
| `req_fn` | input | 5 | Mul/div ALU function |
| `req_fn_early` | input | 5 | Early preview of the mul/div ALU function |
| `req_dw` | input | 1 | 32-bit/64-bit operation selector |
| `req_in1` | input | 64 | Operand A |
| `req_in2` | input | 64 | Operand B |
| `req_dest` | input | 7 | Packed destination descriptor |
| `kill` | input | 1 | Kill/squash request |
| `chicken_bit_mul_div` | input | 1 | Force-on/debug control for the local clock path |
| `resp_ready` | input | 1 | Response acceptance |
| `resp_valid` | output | 1 | Response valid |
| `resp_valid_early` | output | 1 | One-cycle-early writeback reservation |
| `resp_data` | output | 64 | Result data |
| `resp_dest` | output | 7 | Packed destination descriptor |

## Tests

```bash
make -C dv
ORIG_ROOT=/path/to/etcore-soc make -C ../../../../dv/rtlcosim/prim_mul_div test
```
