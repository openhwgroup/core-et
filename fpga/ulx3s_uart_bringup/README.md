# ulx3s_uart_bringup

Minimal ULX3S bring-up project for the onboard FT231X USB-serial link.

## What it does

This project uses the repo's existing `prim_fifo_sync` as a transmit buffer and
adds a small UART RX/TX pair around it. On reset it sends:

```text
Ainekko ULX3S UART TEST PASSED\r\n
```

After the boot banner has been queued, the design echoes every byte received on
the FT231X UART back to the host.

The design is intended as the first real ULX3S hardware smoke test for this
repository:

- host-visible immediately on `/dev/ttyUSB0`
- tiny enough to be low-risk on ECP5
- reuses repo RTL instead of being a standalone board-only demo

## Project structure

```text
ulx3s_uart_bringup/
  rtl/
    ulx3s_uart_bringup.sv    Shared project top
    ulx3s_uart_tx.sv         Local UART transmitter
    ulx3s_uart_rx.sv         Local UART receiver
  ulx3s/
    Makefile                ULX3S / ECP5 synthesis + place/route head
    ulx3s_uart_bringup_ulx3s.sv
    ulx3s_v20.lpf
  verilator/
    Makefile
    ulx3s_uart_bringup_test.cc
```

## Shared top ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Core clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `uart_rx_i` | input | 1 | UART RX from host |
| `uart_tx_o` | output | 1 | UART TX to host |
| `led_o` | output | 8 | Status LEDs |

## ULX3S head ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_25mhz` | input | 1 | ULX3S onboard 25 MHz clock |
| `btn_pwr_n` | input | 1 | Power button, active-low |
| `led` | output | 8 | User LEDs |
| `ftdi_rxd` | output | 1 | FPGA TX into FT231X RX |
| `ftdi_txd` | input | 1 | FPGA RX from FT231X TX |

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

## IPs used

- `prim_fifo_sync` — repo FIFO used as the TX queue

## Resource utilization

From the `ulx3s/` head on `LFE5U-85F-6BG381C`:

| Metric | Result |
|--------|--------|
| Logic LUT4s before packing | 348 |
| `TRELLIS_COMB` after packing | 386 |
| `TRELLIS_FF` | 102 |
| `TRELLIS_RAMW` | 8 |
| `DP16KD` | 0 |
| Reported Fmax @ 25 MHz constraint | 158.28 MHz |

This is a very small design on the 85F and leaves ample headroom for a more
substantial follow-on demo.
