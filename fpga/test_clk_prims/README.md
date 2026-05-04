// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# test_clk_prims

FPGA/Verilator bring-up project for clock technology primitives.

The shared RTL instantiates:

- `prim_clk_gate`
- `prim_clk_gate_n`
- `prim_clk_mux`

The test drives deterministic enable and select windows, counts output clock
edges, and prints a UART summary. It is intended to catch technology mappings
that elaborate but do not implement the primitive contract on real FPGA.

## Expected UART Output

The exact counts can vary by technology implementation latency, but the final
line must be:

```text
TEST CLK PRIMS PASSED
```

If a primitive violates the contract, the summary still prints the measured
counts and ends with:

```text
TEST CLK PRIMS FAILED
```

## Heads

- `verilator/`: generic primitive simulation.
- `ulx3s/`: ECP5/ULX3S synthesis/programming head.

## ECP5 Mux Expectation

The ECP5 `prim_clk_mux` implementation can use the `DCSC` clock-select cell.
This project opts into that path with `UseTechClockMux=1` and intentionally
exercises `sel_i=1` with a slower second clock; a passing ULX3S run should show
`M1` significantly smaller than `M0`.
