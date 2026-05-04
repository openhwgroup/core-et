# fifo_example

Template project demonstrating how to combine IPs and target multiple backends
(FPGA synthesis and Verilator simulation) from a shared RTL top.

## What it does

Instantiates `prim_fifo_sync` (8-bit wide, depth 4) as a simple ready/valid
FIFO. Serves as the reference for creating new projects.

## Project structure

```
fifo_example/
  rtl/
    fifo_example.sv       Project top — shared across all heads
  ice40/
    Makefile              iCE40 HX8K synthesis head (mk/yosys.mk)
  xilinx/
    Makefile              Xilinx Alveo U200 / Ultrascale+ synthesis head (mk/yosys.mk)
  verilator/
    Makefile              Simulation head (mk/verilator.mk)
    fifo_example_test.cc  C++ test driver
```

## Usage

```bash
# Simulate with Verilator
make -C verilator test

# Synthesize for iCE40
make -C ice40 synth
make -C ice40 report

# Synthesize for Xilinx Ultrascale+
make -C xilinx synth
make -C xilinx report

# Clean
make -C ice40 synth-clean
make -C xilinx synth-clean
make -C verilator clean
```

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `Width` | 8 | Data width in bits |
| `Depth` | 4 | FIFO depth (number of entries) |

Override at synthesis time:
```bash
make -C ice40 synth CHPARAMS="-set Width 16 -set Depth 8"
```

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `clr_i` | input | 1 | Synchronous clear |
| `wvalid_i` | input | 1 | Write valid |
| `wready_o` | output | 1 | Write ready |
| `wdata_i` | input | Width | Write data |
| `rvalid_o` | output | 1 | Read valid |
| `rready_i` | input | 1 | Read ready |
| `rdata_o` | output | Width | Read data |
| `depth_o` | output | DepthW | Current fill level |
| `full_o` | output | 1 | FIFO full |
| `empty_o` | output | 1 | FIFO empty |

## IPs used

- `prim_fifo_sync` — synchronous FIFO with ready/valid handshake

## Resource utilization

| Target | LUTs | FFs | BRAM | Notes |
|--------|------|-----|------|-------|
| iCE40 (synth_ice40) | 49 SB_LUT4 | 39 (32 DFFE + 7 DFFER) | 0 | Distributed RAM |
| Xilinx Ultrascale+ (synth_xilinx -family xcup) | 14 LCs | 7 FDCE | 0 | RAM32M16 |
