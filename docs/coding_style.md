# Coding Style Guide

Based on the [lowRISC Verilog style guide](https://github.com/lowRISC/style-guides/blob/master/VerilogCodingStyle.md).

## Naming

| Element | Convention | Example |
|---------|-----------|---------|
| Signals | `lower_snake_case` | `write_enable` |
| Modules | `lower_snake_case` | `prim_fifo_sync` |
| Parameters | `UpperCamelCase` | `DataWidth`, `NumLanes` |
| Types | `lower_snake_case_t` | `state_t`, `ctrl_reg_t` |
| Enums | type `_e`, values `UpperCamelCase` | `state_e`, `StIdle` |
| Packages | `lower_snake_case_pkg` | `rbox_pkg` |
| Ports | suffix `_i`, `_o`, `_io` | `clk_i`, `data_o` |
| Next-state / flopped | `_d` / `_q` | `state_d`, `state_q` |
| Active-low | `_n` suffix (or `_ni` for ports) | `rst_ni` |

## RTL constructs

### Always blocks

```systemverilog
// Combinational — always_comb, never always @*
always_comb begin
  state_d = state_q;
  unique case (state_q)
    StIdle:   if (start_i) state_d = StActive;
    StActive: if (done_i)  state_d = StIdle;
    default:  state_d = StIdle;
  endcase
end

// Sequential — always_ff with async-assert/sync-deassert reset
always_ff @(posedge clk_i or negedge rst_ni) begin
  if (!rst_ni) begin
    state_q <= StIdle;
  end else begin
    state_q <= state_d;
  end
end
```

### Parameters over defines

```systemverilog
// Good — scoped, type-safe
module my_block #(
  parameter int unsigned Width = 8,
  parameter int unsigned Depth = 4
) ( ... );

// Bad — global namespace pollution
`define MY_BLOCK_WIDTH 8
```

Use packages for shared constants:

```systemverilog
package my_block_pkg;
  typedef enum logic [1:0] {
    StIdle   = 2'b00,
    StActive = 2'b01,
    StDone   = 2'b10
  } state_e;
endpackage
```

### Structs over wide buses

```systemverilog
// Good
typedef struct packed {
  logic        valid;
  logic [31:0] addr;
  logic [63:0] data;
} req_t;

// Bad
logic [96:0] req;  // what is bit 47?
```

### Case statements

```systemverilog
// Good — tool-checked, no synopsys pragmas needed
unique case (sel)
  2'b00: ...
  2'b01: ...
  2'b10: ...
  2'b11: ...
endcase

// Bad
case (sel)  // synopsys full_case parallel_case
```

## Reset strategy

**Async-assert, sync-deassert** — works on both FPGA and ASIC:

```systemverilog
always_ff @(posedge clk_i or negedge rst_ni) begin
  if (!rst_ni) begin
    // reset values
  end else begin
    // normal operation
  end
end
```

- Active-low reset signal named `rst_ni`
- Use a reset synchronizer at the top-level only
- No `initial` blocks for register initialization (ASIC-incompatible)

## FPGA-friendly patterns

- **Avoid introducing new ad hoc latches in ordinary RTL.** But when translating original CORE-ET logic, preserve intentional latch-based and phase-sensitive behavior explicitly; do not collapse `LATCH` / `LATCH_P2`-style logic or latch-backed RF timing into edge-triggered flops just because it is simpler to code.
- **No ad hoc gated clocks.** Use `prim_clk_gate` for intentional clock gating rather than hand-rolled gating logic in functional RTL.
- **BRAM inference:** Registered output, synchronous read:
  ```systemverilog
  logic [W-1:0] mem [D];
  always_ff @(posedge clk_i) begin
    if (we_i) mem[waddr_i] <= wdata_i;
    rdata_o <= mem[raddr_i];
  end
  ```
- **No tri-states** inside the fabric.
- **`$clog2`** for address sizing, not magic numbers.

## Yosys / slang compatibility

The synthesis flow uses Yosys with the slang SystemVerilog frontend. These patterns can work in simulation but fail or elaborate differently in synthesis:

### No parameterized-width casts (`W'(expr)`)

Parameterized casts in constant-sensitive expressions are fragile across synthesis frontends. Use explicit slices or zero/sign extension instead.

```systemverilog
// BAD — fragile in synthesis
result = HartsL'(m);

// GOOD — explicit bit-select, same behavior
result = m[HartsL-1:0];

// GOOD — for all-ones constants
result = {HartsL{1'b1}};  // instead of HartsL'(63)
```

### No runtime-dependent `for` loop bounds

Yosys requires `for` loop bounds to be constant for unrolling. Move runtime conditions inside the loop body:

```systemverilog
// BAD — Yosys cannot unroll (limit is runtime)
for (int m = 0; (m < N) && (m <= limit_q); m++)
    result = data[m];

// GOOD — constant bound, runtime condition inside
for (int m = 0; m < N; m++)
    if (m[W-1:0] <= limit_q)
        result = data[m];
```

## File organization

- One module per file. File name = module name.
- Package file: `<block>_pkg.sv`.
- Include guards not needed (packages, not headers).

## Assertions

Use Verilator-compatible style:

```systemverilog
// synthesis translate_off
`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    if (rst_ni && condition)
      $error("message");
  end
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni) ...)
  else $error("message");
`endif
// synthesis translate_on
```
