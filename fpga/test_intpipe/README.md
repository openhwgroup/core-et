# test_intpipe

ULX3S-ready focused bring-up project for the repo's `intpipe_top`.

## What it does

This project instantiates `intpipe_top` directly, drives it from a tiny fake
frontend response source, and prints a one-shot UART summary covering:

1. every accepted instruction issued into ID
2. every redirect request emitted back toward the frontend
3. every retired instruction observed in WB

The built-in program is intentionally broad:

1. integer ALU and dependent RF reads/writes
2. not-taken and taken branches
3. `jal` redirect
4. `mul` and `div`
5. CSR write (`mscratch`) followed by a delayed CSR read after several independent instructions
6. taken branch that depends on the CSR readback value, so the CSR p2 write-data path is architecturally checked
7. wrong-path suppression after taken control flow

The expected UART terminates with:

```text
TEST INTPIPE PASSED
```

The source model is intentionally conservative around control-flow
instructions: after issuing a branch/jump, it stops feeding new instructions
until the redirect arrives or the control-flow instruction retires without a
redirect. That keeps this project focused on `intpipe_top` behavior rather than
recreating frontend speculation.

## IPs used

- `intpipe_top` — integer pipeline under test
- `intpipe_decode` — instruction decoder used to derive `id_ctrl`
- `prim_fifo_sync` — UART transmit FIFO
- `ulx3s_uart_tx` — local UART transmitter reused from ULX3S bring-up

## Primitive inventory

The direct primitive surface exercised by this project is:

- ID/WB trace path
  - no direct technology primitives
  - exercised modules:
    - `intpipe_top`
    - `intpipe_inst_bits_stage`
- RF / bypass / writeback seam
  - `prim_write_preview_en`
  - `prim_rf_2r1w`
  - exercised module:
    - `intpipe_rf`
- Mul/div
  - all targets: `prim_mul_div`
  - generic/original body: `prim_clk_gate`, `prim_phase_pair_lo_hi`,
    `prim_phase_pair_hi_lo`, `prim_cmp_32`, `prim_cmp_42`, plus private
    transparent helper behavior inside `hw/ip/tech_generic/prim_mul_div/rtl/`
  - ECP5 body: only the ECP5 `prim_mul_div` replacement itself
  - exercised modules:
    - `intpipe_mul_div_top`
- CSR
  - `prim_clk_gate`
  - `prim_write_preview_en`
  - `prim_write_commit_en`
  - `prim_write_commit_rst_en`
  - exercised modules:
    - `intpipe_csr_file`
    - `intpipe_csr_file_fl_barrier`
    - `intpipe_csr_file_conv`
    - `intpipe_csr_replay`
    - `intpipe_csr_msgs`
    - generated CSR write-preview / write-commit logic in `intpipe_csr_file_auto*.svh`

The retire-trace path itself is ordinary synchronous RTL in `intpipe_top` and
does not directly instantiate technology primitives.

## Project structure

```text
test_intpipe/
  rtl/
    test_intpipe.sv
  ulx3s/
    Makefile
    test_intpipe_ulx3s.sv
    test_intpipe_ulx3s_pll.sv
    ulx3s_v20.lpf
  verilator/
    Makefile
    test_intpipe_test.cc
  srcs.mk
```

## Usage

```bash
# Verilator test
make -C verilator test

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

The ULX3S head does not use a fake boot delay in the test harness. The board
wrapper holds `rst_ni` low until the PLL reports `pll_locked`, then keeps
reset asserted for about one second before releasing the design.
