# test_mul_div

ULX3S-ready focused bring-up project for the repo's `intpipe_mul_div_top`.

## What it does

This project instantiates `intpipe_mul_div_top` through a thin local wrapper and
checks it with a fixed UART summary against a built-in golden table.
`intpipe_mul_div_top` remains the public wrapper; the actual implementation now
sits behind the technology seam `prim_mul_div`, so the same FPGA leaf test can
exercise either the preserved generic body or the ECP5-specific replacement.

The synthesized FPGA head intentionally keeps its UART reporting at the
interface-summary level. Raw internal latch/phase-pair state is not exported
through root-clocked logger flops, because that creates artificial
root-clock hold failures once the ECP5 primitives preserve the original
transparent-latch semantics again.

The built-in coverage is intentionally leaf-focused:

1. all eight mul/div operations on 64-bit paths
2. all eight mul/div operations on 32-bit paths
3. signed overflow and divide-by-zero corner cases
4. response backpressure hold behavior
5. kill and recovery behavior

The intended golden UART terminates with:

```text
TEST MUL DIV PASSED
```

With the preserved/original generic body, the final kill/recovery vector still
fails and terminates with:

```text
K15 F=10 W=1 P=0 G=000000000000004D E=000000000000004D D=33 N=0
TEST MUL DIV FAILED
```

That is a real leaf-level bug in the original mul/div kill path, not a harness
artifact. See [hw/ip/minion/BUGS.md](../../hw/ip/minion/BUGS.md).

With `TECH=ecp5`, the same leaf test uses the FF-based ECP5 `prim_mul_div`
replacement and the full UART sequence passes locally.

## IPs used

- `intpipe_mul_div_top` — mul/div leaf under test
- `ulx3s_uart_tx` — local UART transmitter reused from ULX3S bring-up

## Primitive inventory

The direct primitive surface exercised by this project depends on `TECH`:

- all targets: `prim_mul_div`
- generic/original body: `prim_clk_gate`, `prim_phase_pair_lo_hi`,
  `prim_phase_pair_hi_lo`, `prim_cmp_32`, `prim_cmp_42`, plus private
  transparent helper behavior inside `hw/ip/tech_generic/prim_mul_div/rtl/`
- ECP5 body: only the ECP5 `prim_mul_div` replacement itself

## Project structure

```text
test_mul_div/
  rtl/
    test_mul_div.sv
  ulx3s/
    Makefile
    test_mul_div_ulx3s.sv
    test_mul_div_ulx3s_pll.sv
    ulx3s_v20.lpf
  verilator/
    Makefile
    test_mul_div_test.cc
  srcs.mk
```

## Usage

```bash
# Verilator test
make -C verilator test

# Verilator test of the ECP5-specific prim_mul_div body
make -C verilator test TECH=ecp5

# Build ULX3S bitstream
make -C ulx3s bit

# Program ULX3S SRAM
make -C ulx3s prog
```

On the host:

```bash
stty -F /dev/ttyUSB0 115200 raw -echo
cat /dev/ttyUSB0
```
