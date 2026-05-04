# shirecache_pipe_example

Synthesis and simulation of the Shire Cache pipeline controller
(`shirecache_pipe`), the most complex completed module in the project.

## What it does

Wraps `shirecache_pipe` — the L2 cache pipeline that instantiates:
- `shirecache_pipe_sub_bank` x4 (tag/data compare, LRU, ECC)
- `shirecache_pipe_sub_bank_mem` x4 (tag, tag-state, data RAMs)
- `shirecache_pipe_cbuf` (write coalescing buffer)
- `shirecache_pipe_rbuf` (read buffer with pending install tracking)
- `shirecache_pipe_idx_cop_sm` (cache maintenance FSM)

Transitively uses: `prim_arb_lru`, `prim_fifo_reg`, `prim_rst_sync`,
`prim_ecc_enc/dec`, `prim_clk_gate`, `prim_clk_mux`, `prim_ram_1p/2p`.

~7,900 lines of RTL across 26 source files.

## Project structure

```
shirecache_pipe_example/
  srcs.mk                       Shared RTL source list (included by all heads)
  rtl/
    shirecache_pipe_example.sv   Project top
  ice40/
    Makefile                     iCE40 synthesis head
  xilinx/
    Makefile                     Xilinx Ultrascale+ synthesis head
  verilator/
    Makefile                     Simulation head
    shirecache_pipe_example_test.cc  Smoke test
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

## IPs used

4 packages, 9 primitives, 15 shirecache sub-modules.

## Resource utilization

| Target | LUTs | FFs | BRAM | Notes |
|--------|------|-----|------|-------|
| iCE40 (synth_ice40) | 47,093 SB_LUT4 | 26,087 | 2,460 SB_RAM40_4K | Down from 52,598 / 31,721; still exceeds HX8K |
| Xilinx Ultrascale+ (synth_xilinx -family xcup) | ~49,838 LCs | 28,010 (all FD*) | 256 RAMB36E2 + 40 RAMB18E2 | 12 BUFGCTRL and 40 RAM32M16 inferred |

The refreshed runs are smaller on both FPGA targets:
- iCE40 dropped by `5,505` `SB_LUT4` cells and `5,634` FFs with BRAM usage unchanged.
- Xilinx dropped from `~51,613` to `~49,838` estimated LCs while keeping the same block RAM count.
- The old Xilinx `-nolutram` note no longer applies to this run; Yosys now infers `RAM32M16` cells alongside the explicit BRAMs, and the `prim_clk_mux` override maps into `BUFGCTRL`.
