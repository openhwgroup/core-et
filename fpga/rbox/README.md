# RBOX FPGA Project

Synthesizes the full `rbox_top` — GPU rasterization and depth-testing unit.
~7K lines of RTL across 18 modules.

## Backends

| Directory | Target | Command |
|-----------|--------|---------|
| `verilator/` | Verilator simulation | `make -C verilator test` |
| `ice40/` | Lattice iCE40 (Yosys) | `make -C ice40 synth` |
| `xilinx/` | Xilinx Ultrascale+ (Yosys) | `make -C xilinx synth` |

## Resource utilization

| Target | LUTs | FFs | BRAM | Other |
|--------|------|-----|------|-------|
| iCE40 (synth_ice40) | 114,826 SB_LUT4 | 25,350 SB_DFF* | 96 SB_RAM40_4K | 4,971 SB_CARRY |

## Module hierarchy

```
rbox_top
├── rbox_esr          (APB register block, 8 registers)
├── rbox_dbg_ram      (APB debug/ESR address mux)
├── rbox_ctrl         (master controller)
│   ├── rbox_input_fifo   (prim_ram_1p ×2)
│   └── rbox_output_buffer (prim_fifo_sram ×2)
├── rbox_traverse     (9-stage triangle rasterizer)
│   ├── rbox_sampler ×12  (rbox_step_mul ×4 each)
│   ├── rbox_sample_to_fp32 ×8
│   ├── rbox_depth_offset ×4 (rbox_fp_adder_2stg each)
│   ├── rbox_fp_adder_2stg ×4
│   ├── rbox_depth_to_fp ×4
│   └── rbox_depth_to_un ×8
├── rbox_test         (depth/stencil test pipeline)
│   ├── rbox_depth_buffer (prim_ram_2p)
│   └── prim_fifo_sram
└── prim_clk_gate     (clock gating)
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
make -C verilator clean
make -C ice40 synth-clean
make -C xilinx synth-clean
```
