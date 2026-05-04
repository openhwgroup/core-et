# test_frontend

ULX3S-ready frontend trace demo for the repo's `minion_frontend`.

## What it does

This project instantiates the real `minion_frontend` block, feeds it from a
small built-in fake ICache, and prints each issued instruction over the ULX3S
FT231X UART. The harness checks the first eight issued instructions against
expected PC, instruction word, and key decode flags, then emits:

```text
TEST FRONTEND PASSED
```

This makes the design visible on the host while still exercising actual repo
RTL instead of a board-only blink.

## IPs used

- `minion_frontend` — real fetch/decode frontend under test
- `prim_clk_gate` — clock gate primitive used inside the frontend
- `prim_arb_lru` — frontend ICache request arbiter
- `prim_rf_1r1w_diff` — frontend fetch buffer RF
- `prim_fifo_sync` — UART transmit FIFO
- `ulx3s_uart_tx` — local UART transmitter reused from ULX3S bring-up

## Project structure

```text
test_frontend/
  rtl/
    test_frontend.sv
  ulx3s/
    Makefile
    test_frontend_ulx3s.sv
    ulx3s_v20.lpf
  verilator/
    Makefile
    test_frontend_test.cc
  srcs.mk
```

## Shared top ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Core clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `uart_tx_o` | output | 1 | UART TX to host |
| `led_o` | output | 8 | Status LEDs |

## ULX3S head ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_25mhz` | input | 1 | ULX3S onboard 25 MHz clock |
| `btn_pwr_n` | input | 1 | Power button, active-low |
| `led` | output | 8 | User LEDs |
| `ftdi_rxd` | output | 1 | FPGA TX into FT231X RX |
| `ftdi_txd` | input | 1 | FT231X TX into FPGA, unused in this demo |

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

## Trace program

The fake ICache returns one 256-bit line containing these RV64 instructions:

1. `addi x0, x0, 0`
2. `addi x1, x0, 1`
3. `addi x3, x0, 3`
4. `lw x2, 0(x1)`
5. `sw x2, 4(x1)`
6. `beq x1, x2, 8`
7. `jal x0, 8`
8. `jalr x0, x1, 0`

The harness prints one line per issued instruction with:

- issued PC and instruction word
- selected decode flags (`legal`, `mem`, `br`, `jal`, `jalr`)
- redirect debug (`RV`, `RP`)
- a compact snapshot of internal frontend bookkeeping (`CV`, `CP`, `FP`, `FN`,
  `PU`, `CE`, `IV`, `IA`, `RA`, `BE`, `IF`, `MP`)

So the UART output is intentionally richer than a pure instruction trace. It is
meant to expose frontend PC/update behavior directly on FPGA when bring-up is
focused on the fetch path.
