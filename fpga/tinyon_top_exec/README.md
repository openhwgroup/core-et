# tinyon_top_exec

Minimal ULX3S-ready execution demo for the repo's translated `minion_top`.

This project reuses the proven `minion_top` smoke-test integration shape, serves
a constant fake I-cache line filled with scalar NOPs, and uses the translated
trace encoder to check that the first two retired instructions appear at
`0x1000` and `0x1004` without exceptions.

When the check completes, the design emits the original compact UART summary:

```text
Ainekko tinyon_top_exec
R0 P=0000000000001000 I=00000013 X=0
R1 P=0000000000001004 I=00000013 X=0
TINYON TOP EXEC TEST PASSED
```

The project is intentionally narrow:

- real `minion_top` integration
- fake I-cache only
- no D-cache/L2 servicing
- integer-only scalar execution path

For FPGA bring-up, the shared `minion_top` instance in this project is built
with:

- `DebugApbEn=0`
- `DebugMonEn=0`
- `TraceEn=1`
- `VpuEn=0`

The debug APB slave, top-level debug monitor fabric, dcache APB/debug path,
and frontend `vpu_decoder` are removed for this head, while the trace stream is
kept enabled for the bring-up harness.

That makes it the shortest realistic ECP5 board bring-up for `minion_top`
without turning the demo into a full SoC-memory-system project.

## Layout

```text
tinyon_top_exec/
  README.md
  srcs.mk
  rtl/
    tinyon_top_exec.sv
  xilinx/
    Makefile
  verilator/
    Makefile
    tinyon_top_exec_test.cc
  ulx3s/
    Makefile
    tinyon_top_exec_ulx3s.sv
    tinyon_top_exec_ulx3s_pll.sv
    ulx3s_v20.lpf
```

## IP Used

- `minion_top`
- `core_top`
- `minion_frontend`
- `minion_tlb`
- `minion_dcache_top`
- `minion_debug_apb_slv`
- `null_vpu`
- `prim_fifo_sync`
- `ulx3s_uart_tx`

## Shared Top Ports

| Port | Dir | Width | Description |
|------|-----|-------|-------------|
| `clk_i` | input | 1 | Core/demo clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `uart_tx_o` | output | 1 | UART TX summary output |
| `led_o` | output | 8 | Demo status LEDs |

## ULX3S Head Ports

| Port | Dir | Width | Description |
|------|-----|-------|-------------|
| `clk_25mhz` | input | 1 | ULX3S onboard 25 MHz clock |
| `btn_pwr_n` | input | 1 | ULX3S power button / reset source |
| `led` | output | 8 | ULX3S user LEDs |
| `ftdi_rxd` | output | 1 | FPGA UART TX to FT231X |
| `ftdi_txd` | input | 1 | FT231X UART RX (unused by this demo) |

The ULX3S head uses an ECP5 PLL to derive the Minion core/demo clock from the
board's fixed 25 MHz oscillator. The current head configuration runs the design
at 10 MHz to stay within the open-source ECP5 timing margin while bring-up is
ongoing.

## ECP5 Bring-Up Choices

The current ULX3S/ECP5 head carries a few explicit FPGA-only choices. These are
documented here so users do not mistake them for generic Minion behavior:

- Root clocking uses a real PLL and currently runs the design at `10 MHz`.
  The board oscillator is `25 MHz`, but the open-source ECP5 flow did not yet
  close timing on the always-on root domain at that frequency.
- `nextpnr-ecp5` is allowed to promote the PLL/root clock to a global. Do not
  pass `--no-promote-globals` here: the translated Minion clock-gate primitives
  may also instantiate explicit `DCCA` local clocks, and blocking root clock
  promotion can leave the design with a bad global-clock routing shape. The
  expected route log has the PLL/root clock plus any explicit `DCCA` outputs on
  globals.
- The ECP5 `prim_clk_mux` implementation currently forwards the functional SRAM
  clock and does not implement the DFT SRAM clock override path.
- ECP5 now preserves the public phase-pair seams directly
  (`prim_phase_pair_lo_hi`, `prim_phase_pair_hi_lo`), while the narrowed
  `prim_write_*` seams use documented edge-triggered mappings and any remaining
  exact transparent behavior stays private inside owning primitive bodies.

## Build

```bash
# Verilator check
make -C verilator test

# Xilinx UltraScale+ synthesis estimate
make -C xilinx synth
make -C xilinx report

# Build ULX3S bitstream
make -C ulx3s bit

# Program ULX3S SRAM
make -C ulx3s prog
```

Read the board UART with:

```bash
stty -F /dev/serial/by-id/usb-FER-RADIONA-EMARD_ULX3S_FPGA_85K_v3.0.8_D00455-if00-port0 115200 raw -echo -ixon -ixoff -crtscts
cat /dev/serial/by-id/usb-FER-RADIONA-EMARD_ULX3S_FPGA_85K_v3.0.8_D00455-if00-port0
```

## Resource Utilization

Not yet measured on the `ulx3s/` head in this README. Build logs provide the
authoritative ECP5 usage numbers once synthesis/place-route has been run.

The `xilinx/` head is intentionally synthesis-only today:

- it runs Yosys `synth_xilinx -family xcup` against the shared
  `tinyon_top_exec` top for UltraScale+ resource estimation
- it does not yet provide a U200 board wrapper, XDC, PCIe shell integration,
  or a loadable Alveo artifact

This matters for the U200 specifically. The AMD Alveo U200 card uses the
UltraScale+ `XCU200-FSGD2104-2-E`, but AMD's supported board flows are either:

- Vivado RTL/IP-integrator flow with the U200 board support/XDC files, or
- Vitis/XRT flow that produces an `.xclbin` for the Alveo shell

So this repo's `xilinx/` head is the right Yosys command for synthesis, but it
is not by itself enough to program a U200 card.
