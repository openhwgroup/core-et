# Minion Integer Pipeline (intpipe)

RISC-V integer/control execution pipeline for the Minion RV64IMFC + Zicsr + Zifencei core. Handles instruction decode, register file access, ALU execution, multiply/divide, CSR operations, scoreboard hazard tracking, and debug breakpoints.

## Architecture

5-stage in-order pipeline: ID → EX → TAG → MEM → WB, with gather/scatter (GSC) extension.

### Submodules

| Module | Description | LOC |
|--------|-------------|-----|
| `intpipe_top` | Top-level pipeline integration | 2,542 |
| `intpipe_alu` | ALU (ADD/SUB, shifts, logic, compares, PACKB, BITMIXB) | 163 |
| `intpipe_imm` | Immediate generator (S/Sb/U/Uj/I/Z formats) | 47 |
| `intpipe_rf` | Register file wrapper (2R1W, x0 hardwired zero, x31 parallel read) | 80 |
| `intpipe_inst_bits_stage` | Per-bit instruction pipeline stage with mask/branch-mask parameters | 42 |
| `intpipe_mul_div_top` | Public mul/div contract wrapper; implementation selected through `prim_mul_div` | 191 |
| `intpipe_int_scoreboard` | Integer register hazard scoreboard | 80 |
| `intpipe_fp_scoreboard` | FP register hazard scoreboard | 55 |
| `intpipe_mask_scoreboard` | VPU mask register scoreboard | 45 |
| `debug_breakpoint` | Hardware breakpoint (instruction or data trigger) | 140 |

### CSR Subsystem (`intpipe/csr/`)

| Module | Description | LOC |
|--------|-------------|-----|
| `intpipe_csr_file` | Main CSR register file (211 registers, auto-generated sections) | 2,160 |
| `intpipe_csr_file_fl_barrier` | Fast Local Barrier FSM | 94 |
| `intpipe_csr_file_conv` | Tensor convolution boundary computation | 148 |
| `intpipe_csr_pmu_read_interface` | PMU counter read interface | 93 |
| `intpipe_csr_replay` | CSR replay/stall condition detection | 235 |
| `intpipe_csr_msgs` | Message port control and configuration | 406 |
| `intpipe_csr_pkg` | CSR type definitions and address enum (auto-generated) | 489 |

## CSR Auto-Generation

The CSR register declarations, decode logic, write logic, and type conversion functions are auto-generated from CSV data by `hw/ip/esr/scripts/gen_esr.py`:

```
python3 hw/ip/esr/scripts/gen_esr.py \
  --csv hw/ip/minion/intpipe/csr/data/csr.csv \
  --types-dir hw/ip/minion/intpipe/csr/data/csr_types/ \
  --out hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_auto.svh \
  --pkg-out hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv
```

`--out` supplies the basename for the generated section files. The generator
produces 8 include fragments (`*_declarations.svh`, `*_decode_cases.svh`,
etc.) consumed by `intpipe_csr_file.sv`; it does not emit a monolithic
`intpipe_csr_file_auto.svh` include.

## Primitive Mapping

| Original (CORE-ET source tree) | Replacement | Notes |
|--------------------------|-------------|-------|
| `et_clk_gate` | `prim_clk_gate` | Posedge clock gating cell |
| `et_clk_gate_n` | `prim_clk_gate_n` | Negedge clock gating cell |
| `intpipe_mul_div_top` implementation body | `prim_mul_div` | Technology seam for mul/div latency/implementation experiments without changing the intpipe-visible contract; generic helper RTL now lives under `hw/ip/tech_generic/prim_mul_div/rtl/` |
| Write-preview seam | `prim_write_preview_en` | Narrowed write-pipeline primitive for staging one-phase-early write qualifiers or payload data before commit |
| CSR write-commit seam | `prim_write_commit_en` | Narrowed write-pipeline primitive for enabled state commit without reset |
| CSR resettable write-commit seam | `prim_write_commit_rst_en` | Narrowed write-pipeline primitive for enabled state commit with reset value |
| Two-phase handoff with both phase outputs live | `prim_phase_pair_lo_hi` / `prim_phase_pair_hi_lo` | Self-contained public seam for preserved phase-pair behavior |
| `LATCH_P2` + `rf_latch_2r_1w` | `prim_rf_2r1w_preview` | 2-read 1-write register file with the original write-preview contract kept inside one seam |
| `r32cmp` | `prim_cmp_32` | 3:2 compressor |
| `r42cmp` | `prim_cmp_42` | 4:2 compressor |
| `gen_fifo` | Inline FIFO (in msgs) | OOB queue storage |
| `RST_FF`, `EN_FF`, etc. | Explicit `always_ff` | No macros |
| `CSR_DECLARATION_*` macros | `gen_esr.py` auto-generation | Clean SV from CSV |

## Differences from Original

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|------------------|-----------|
| Reset | Active-high synchronous (`reset`) | Active-low async (`rst_ni`) | Ainekko convention |
| Reset domains | `reset_c`, `reset_w`, `reset_d` | `rst_ni`, `rst_w_ni`, `rst_d_ni` | Preserved, renamed |
| DFT | Individual `dft__*` ports | `core_top` adapts project `dft_t` to the preserved intpipe-local `dft__reset_byp` / `dft__reset` signals | Keeps the minion-level DFT surface consolidated while preserving the original intpipe reset-bypass contract internally |
| CSR macros | `CSR_DECLARATION_*`, `CSR_DECODE_*` | Auto-generated `.svh` includes | No macros, clean SV |
| Clock gating | `et_clk_gate` / `et_clk_gate_n` | `prim_clk_gate` / `prim_clk_gate_n` | Technology-swappable |
| CSR write staging/commit helpers | Helper clocks plus latch macros | Behavioral write-preview / write-commit primitives | Narrows the CSR seam so FPGA targets can replace the write path without redefining the full latch family |
| Local helper latch behavior | Named phase pairs or behavior-named write seams | Self-contained public primitives | Keeps shared RTL close to the original while pushing any remaining exact transparent behavior behind primitive boundaries |
| Types | `include "soc.vh"` | `import minion_pkg::*` | Package-based |
| Naming | Mixed conventions | lowRISC `_i`/`_o`/`_d`/`_q` | Consistent style |

## Co-simulation

Every module has a standalone RTL co-simulation in `dv/rtlcosim/` comparing against the original cycle-by-cycle. All pass with 0 mismatches.

## Follow-Up

- Add focused unit tests in `hw/ip/minion/intpipe/dv/` when convenient.
  Current confidence comes from standalone cosims plus frontend decoder and
  minion/core integration tests; unit tests would mainly make local debug
  faster for leaves such as `intpipe_alu`, the scoreboards,
  `debug_breakpoint`, `intpipe_csr_replay`, and `intpipe_csr_msgs`.
