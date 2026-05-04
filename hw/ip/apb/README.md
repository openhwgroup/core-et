# APB Package

Defines AMBA APB3 protocol types for ESR (ET System Register) access across the SoC.

## Package

`apb_pkg` provides two packed structs:

| Type | Description |
|------|-------------|
| `req_t` | Master-to-slave: paddr (16b), pwrite, psel, penable, pwdata (64b) |
| `rsp_t` | Slave-to-master: pready, prdata (64b), pslverr |

Default widths (16-bit address, 64-bit data) match the CORE-ET ESR APB interface.

## Usage

```systemverilog
import apb_pkg::*;

module my_esr (
  input  apb_pkg::req_t apb_req_i,
  output apb_pkg::rsp_t apb_rsp_o,
  ...
);
```

## Differences from original

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|------------------|-----------|
| Types | `APB_to_rbox_ESRs_t` / `APB_from_rbox_ESRs_t` per-block | Generic `apb_pkg::req_t` / `apb_pkg::rsp_t` | Single reusable package |
| Definition | Auto-generated in `esr_types_auto.vh` | Hand-written package | Clean, maintainable |
