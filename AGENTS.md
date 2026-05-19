# SoC Project Guidelines

## What this is

Clean reimplementation of the CORE-ET architecture. FPGA-friendly AND tapeout-friendly. Open-source tooling first.
Ainekko owns the CORE-ET IP and is reimplementing only the Ainekko-owned CORE-ET blocks in modern, minimal SystemVerilog.

This project targets **both FPGA prototyping and ASIC tapeout**. All tapeout infrastructure (DFT, clock gating, RAM wrappers, ECC, BIST hooks, power domain awareness) must be present. The approach is to use clean technology abstraction so the same RTL works across simulation, FPGA, and ASIC.

## Copyright header

Every source file must start with:
```
// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
```

## Repository layout

```
hw/ip/<block>/rtl/       Synthesizable RTL (one module per file, filename = module name)
hw/ip/<block>/dv/        Block-level testbench and tests
hw/ip/<block>/data/      CSR definitions, config (if needed)
hw/ip/<block>/doc/       Block-level spec (if needed)
hw/ip/tech_generic/<module>/  Technology primitives: behavioral (simulation, default)
hw/ip/tech_ice40/<module>/    Technology primitives: Lattice iCE40 FPGA
hw/ip/tech_xilinx/<module>/   Technology primitives: Xilinx 7-series / Ultrascale+
hw/ip/tech_asic/rtl/     Technology primitives: ASIC foundry-specific (private, future)
hw/top/                  Top-level chip integration
fpga/<project>/          Projects — combine IPs, target multiple backends (see Projects section)
  rtl/                     Shared project RTL
  <head>/                  Per-backend head (ice40/, xilinx/, verilator/, etc.)
dv/common/               Shared DV utilities (sim_ctrl.h)
dv/rtlcosim/             RTL co-simulation (old vs new module comparison)
mk/                      Build infrastructure (verilator.mk, yosys.mk)
fpga/                    FPGA constraints and wrappers
syn/                     Synthesis scripts
sw/                      Firmware and software
vendor/                  Third-party IP
docs/                    Project-wide documentation
```

RTL and its testbench live together under each IP block. Do not create separate global RTL and TB trees.

## Adding a new IP block

1. Create `hw/ip/<name>/README.md` — describe the module, what it is, and how to use it (ports, parameters, integration notes)
2. Create `hw/ip/<name>/rtl/<name>.sv`
3. Create `hw/ip/<name>/dv/Makefile` — set `TB_TOP`, `RTL_SRCS`, `CC_SRCS`, include `$(REPO_ROOT)/mk/verilator.mk`
4. Create `hw/ip/<name>/dv/<name>_test.cc` using `sim_ctrl.h` from `dv/common/`
5. The top-level `make test` auto-discovers all IP blocks with a `dv/Makefile`

Unit-test CI uses the same top-level discovery through `make ci-unit-test-dirs`
and `make ci-unit-test-matrix`. New unit-test frameworks must live under
`hw/ip/*/dv/Makefile` or `hw/ip/*/*/dv/Makefile` so they run as independent CI
matrix entries. If a test must use a different layout, update the top-level
Makefile discovery and CI helper in the same change.

Every IP block **must** have a `README.md` at its root (`hw/ip/<name>/README.md`) that describes:
- What the module does
- Parameters and their meaning
- Port interface (inputs, outputs, protocol)
- Usage example or integration notes
- Any design constraints or assumptions
- Differences from the original CORE-ET module (if reimplementing one)

## Original bug tracking

When analysis uncovers a bug in the original CORE-ET repository, document it immediately in a `BUGS.md` file at the relevant IP root (for example `hw/ip/<block>/BUGS.md`). If the file already exists, append a new entry instead of replacing it.

Every such entry must:
- state clearly in the heading that it is an **original repository bug**
- identify the original module(s) and the reimplemented module(s) affected
- describe the symptom, root cause, reachability, and system impact in report style
- cite the relevant original file paths and signal names or code fragments precisely enough for an engineer to audit quickly
- state whether the current reimplementation intentionally preserves the original behavior or intentionally diverges

Do not silently "fix" original-design bugs while claiming strict equivalence. If a correctness fix is chosen over equivalence, document that decision in both `BUGS.md` and the affected module `README.md`.

## Technology abstraction

Primitives whose implementation differs between FPGA and ASIC are **technology primitives**. They have the same module name and ports across all targets, but different implementations live in technology-specific directories:

```
hw/ip/tech_generic/prim_clk_gate/rtl/prim_clk_gate.sv    # behavioral (simulation, default)
hw/ip/tech_ice40/prim_clk_gate/rtl/prim_clk_gate.sv      # iCE40: negedge FF + AND
hw/ip/tech_xilinx/prim_clk_gate/rtl/prim_clk_gate.sv     # Xilinx: negedge FF + AND
hw/ip/tech_asic/rtl/prim_clk_gate.sv       # future: foundry ICG cell
```

Each technology tree must also carry a root `README.md` that explains the
primitive set at the right level:

- `hw/ip/tech_generic/README.md` describes each technology primitive,
  what it does, and the contract it presents to functional RTL. Treat this
  as the primitive-family behavioral/specification README.
- `hw/ip/tech_<specific>/README.md` describes how that technology
  implements the same primitive contract on that specific platform
  (resource mapping, vendor cells, FPGA-safe substitutions, limitations,
  and differences from `tech_generic`).

`mk/prim.mk` selects the right implementation based on `TECH` variable (see Build system section). The build system includes the correct source file — no `ifdef` in RTL.

**Technology primitives** (live in `tech_*/<module>/rtl/`):

- **`prim_clk_gate`** — Clock gating cell. Generic: latch + AND (ASIC ICG pattern). FPGA: negedge FF + AND (no latch). Must exist as a real primitive, not replaced by clock enables.
- **`prim_clk_gate_n`** — Negative-phase clock gate used by translated low-phase / phase-2 capture logic.
- **`prim_clk_mux`** — Clock multiplexer. Used for DFT SRAM clock override.
- **`prim_phase_pair_lo_hi`** — Named composite for a direct low-phase capture followed by a direct high-phase capture when both phase outputs remain architecturally live inside shared RTL.
- **`prim_phase_pair_hi_lo`** — Named composite for a direct high-phase capture followed by a direct low-phase capture when both phase outputs remain architecturally live inside shared RTL.
- **`prim_write_preview_en`** — Narrow behavior-named write-preview seam for staging one-phase-early qualifiers or payload data before a later commit on the same base clock.
- **`prim_write_commit_en`** — Narrow behavior-named write-commit seam for enabled state updates without their own reset contract.
- **`prim_write_commit_rst_en`** — Narrow behavior-named write-commit seam for enabled state updates with a defined reset value.
- **`prim_mul_div`** — Technology seam for the Minion mul/div unit; keeps the `intpipe_mul_div_top` visible contract stable while allowing technology-specific implementation changes behind the primitive boundary.
- **`prim_rf_1r1w_preview`** — Named composite for a low-phase write-preview seam feeding a same-width latch-timed 1R1W RF.
- **`prim_rf_1r1w_diff_preview`** — Named composite for a low-phase write-preview seam feeding a mixed-width latch-timed 1R1W RF.
- **`prim_ram_1p`** — Single-port RAM wrapper. Generic: `logic mem[]`. FPGA: `(* ram_style = "block" *)` for explicit BRAM. ASIC: foundry SRAM macro + ECC + BIST hooks. All RAM instantiations go through this wrapper.
- **`prim_ram_2p`** — Dual-port RAM wrapper. Same abstraction as 1p.
- **`prim_rst_sync`** — Reset synchronizer with DFT bypass port.
- **`prim_fifo_async_hiv`** — Async CDC FIFO, write=high-voltage, read=low-voltage. On ASIC, has foundry-specific level shifters and CDC cells.
- **`prim_fifo_async_lov`** — Async CDC FIFO, write=low-voltage, read=high-voltage. On ASIC, different level shifter placement from hiv.

**Library IP** (pure RTL, same on all targets — stay in `hw/ip/prim_*/rtl/`):

- **`prim_arb_lru`**, **`prim_arb_rr`**, **`prim_arb_prio`** — Arbiters (pure combinational + sequential logic).
- **`prim_ecc_enc`**, **`prim_ecc_dec`** — ECC encoder/decoder (pure XOR combinational logic).
- **`prim_fifo_sync`**, **`prim_fifo_reg`**, **`prim_fifo_sram`** — FIFOs (pure logic, or wrappers around tech primitives).
- **`prim_rf_1r1w`** — Register file (same read/write width).
- **`prim_rf_1r1w_reg`** — Register file with registered read address.
- **`prim_rf_1r1w_par`** — Register file with full parallel readback.
- **`prim_rf_1r2w_par`** — Dual-write register file with full parallel readback.
- **`prim_rf_3r2w`** — Three-read / two-write register file.
- **`prim_rf_1r1w_diff`** — Register file with different read/write widths and sub-word read alignment.
- **`prim_rf_single_1r1w_par`** — Single-entry write-capture primitive with direct readback.
- **`prim_rf_2r1w`** — Two-read / one-write register file.

**Never use bare `logic [W-1:0] mem [D]` in functional RTL.** Always instantiate `prim_ram_1p` or `prim_ram_2p`. The RAM wrapper is the insertion point for foundry SRAMs, ECC, BIST, and timing configuration.

## DFT strategy

DFT (Design for Test) ports are **required** on all primitives and on modules
whose original implementation already has a DFT control surface. Use a clean,
minimal DFT interface:

```systemverilog
package dft_pkg;
  typedef struct packed {
    logic scanmode;          // Scan mode active — bypass functional clocks/resets
    logic scan_reset_n;      // Scan reset (active-low), used when scanmode=1
    logic sram_clk_override; // Select DFT SRAM clock via prim_clk_mux
    logic ram_rei;           // RAM read enable inhibit (DFT)
    logic ram_wei;           // RAM write enable inhibit (DFT)
  } dft_t;
endpackage
```

Modules that need DFT take `dft_pkg::dft_t dft_i` as an input port. In normal operation `dft_i = '0` (scanmode off, SRAM override disabled, RAM inhibits disabled). During scan or RAM test modes, the scan/test controller drives these signals.

When translating original CORE-ET RTL, preserve existing DFT surfaces and
translate them to the project DFT abstractions, but do not invent new
module-level DFT ports or clock muxes just because a module contains SRAMs,
clocks, or resets. For example, an original `et_clk_mux2` SRAM-clock override
maps to `prim_clk_mux`; an SRAM path with only a functional `clock` remains a
single functional clock unless a documented wrapper boundary explicitly adds
the DFT hook.

Specifically:
- **`prim_rst_sync`** must accept `dft_i` and bypass the synchronizer in scan mode.
- **`prim_clk_gate`** must accept `dft_i` and force the clock on in scan mode.
- **`prim_ram_*`** must accept a RAM config struct for timing margins and test modes.
- **`prim_fifo_async_hiv`** / **`prim_fifo_async_lov`** must accept `dft_hv_i`/`dft_lv_i` for per-domain reset bypass.

## RAM configuration

All RAM wrappers accept a standardized configuration struct:

```systemverilog
package ram_cfg_pkg;
  typedef struct packed {
    logic       test_en;     // RAM test mode enable
    logic [3:0] rm;          // Read margin
    logic       rme;         // Read margin enable
    logic [1:0] ra;          // Read assist
    logic [1:0] wa;          // Write assist
    logic [2:0] wpulse;      // Write pulse width
    logic       deep_sleep;  // RAM power-down (low-leakage retention mode)
    logic       shut_down;   // RAM shutdown (data lost)
  } ram_cfg_t;
endpackage
```

This replaces the CORE-ET `esr_shire_cache_ram_cfg_t`. In simulation and FPGA, `ram_cfg_i = '0`. For ASIC, these are driven by ESR registers for post-silicon timing tuning.

## SystemVerilog coding style (lowRISC conventions)

### Naming

- Signals: `lower_snake_case`
- Modules: `lower_snake_case` (e.g., `rbox_ctrl`, `prim_fifo_sync`)
- Parameters/localparams: `UpperCamelCase` (e.g., `DataWidth`, `NumLanes`)
- Types: `lower_snake_case_t` (e.g., `state_t`, `ctrl_reg_t`)
- Enums: type name ends `_e`, values `UpperCamelCase` (e.g., `state_e`, `StIdle`)
- Packages: `lower_snake_case_pkg` (e.g., `rbox_pkg`)
- Ports: suffix `_i` (input), `_o` (output), `_io` (inout)
- Next-state / flopped: `_d` / `_q`
- Active-low: `_n` suffix (or `_ni` for input ports)

### RTL rules

- **`always_ff` / `always_comb` only.** Never use bare `always @*` or `always @(posedge clk)`.
- **Parameters, not `define`.** Use `parameter`/`localparam` for constants. Use packages (`_pkg.sv`) for shared types. Only use `` `define `` for tool guards (`SYNTHESIS`, `VERILATOR`).
- **Original feature `ifdef`s become parameters.** If the original RTL uses preprocessor switches to enable or disable functional features in synthesizable logic (for example `ENABLE_EXTRA_TRANS`), translate that feature control into explicit parameters or shared package constants propagated through the affected modules. Do not keep synthesizable feature selection behind translated `ifdef`s. If the repo intentionally supports only one configuration for now, encode that choice explicitly in RTL/package parameters and document it.
- **Packed structs and enums** for all multi-field buses. No anonymous wide bit vectors.
- **`unique case` / `priority case`** instead of bare `case` with synopsys pragmas. Use `priority case (1'b1)` for priority encoders where multiple branches can match — NOT `unique case (1'b1)`.
- **No parameterized-width casts** like `W'(expr)` — they are fragile across the Yosys synthesis frontend and can be rejected or mis-elaborated. Use `expr[W-1:0]` or zero-pad `{{W-2{1'b0}}, field}` instead. See `docs/coding_style.md` for details.
- **Constant `for` loop bounds only** — Yosys requires loop bounds to be compile-time constant. Move runtime conditions inside the loop body as `if` statements.
- **`{N, expr}` is concatenation, not replication.** `{$bits(x), expr}` concatenates the integer with the expression. Replication is `{$bits(x){expr}}` (double braces). The original CORE-ET code uses concatenation in some places — replicate the exact syntax.
- **No tri-states** inside the fabric.
- **One `always_ff` per flop group** with a matching `always_comb` for next-state logic. Use `_d`/`_q` naming:

```systemverilog
always_comb begin
  state_d = state_q;
  unique case (state_q)
    StIdle:   if (start_i) state_d = StActive;
    StActive: if (done_i)  state_d = StIdle;
    default:  state_d = StIdle;
  endcase
end

always_ff @(posedge clk_i or negedge rst_ni) begin
  if (!rst_ni) state_q <= StIdle;
  else         state_q <= state_d;
end
```

### Reset strategy

Async-assert, sync-deassert. Active-low `rst_ni`. Works on both FPGA and ASIC:

```systemverilog
always_ff @(posedge clk_i or negedge rst_ni) begin
  if (!rst_ni) begin
    // reset values
  end else begin
    // normal operation
  end
end
```

- No `initial` blocks for register initialization (ASIC-incompatible).
- Reset synchronizer at top-level only.

### FPGA and ASIC compatible patterns

- Avoid introducing new ad hoc latches in ordinary RTL.
- Preserve intentional latch-based and phase-sensitive behavior when translating the original design. If the original uses `LATCH` / `LATCH_P2`-style logic, latch-backed RF protocols, or low-phase capture semantics, keep that behavior explicit in the new RTL rather than collapsing it into edge-triggered flops.
- When translated shared RTL contains a recurring raw two-phase latch chain with both phase nodes still live, prefer a named semantic composite over open-coding the latch pair repeatedly. Keep the shared RTL at the level of the original phase contract, and give each technology tree a single implementation seam.
- The goal of these composites is not to redesign timing. The shared RTL must still describe the same parent-visible cycle timing and the same original phase ordering; only the implementation point moves behind the primitive boundary.
- Use `prim_clk_gate` for intentional clock gating — maps to ICG on ASIC and a safe FPGA-friendly implementation. Do not hand-roll gated clocks in functional RTL.
- Use `prim_ram_1p` / `prim_ram_2p` for all memories — maps to foundry SRAM on ASIC, BRAM on FPGA.
- `$clog2` for address width sizing, not magic numbers.
- No division/modulo by non-power-of-2 in synthesizable code.

### Verilator lint warnings

`make lint` must pass with zero warnings. When a warning is structurally benign, suppress it with an **inline pragma** at the source, with a comment explaining why:

```systemverilog
/* verilator lint_off UNUSEDSIGNAL */  // dft_t carries RAM fields unused by this reset-only module
...
/* verilator lint_on UNUSEDSIGNAL */
```

**Benign warning categories and when to suppress:**

- **UNUSEDPARAM** — Package constants defined for future modules not yet implemented. Suppress at package level.
- **UNUSEDSIGNAL** — Packed structs passed whole to modules that only read some fields (e.g., `dft_t` has 5 fields, `prim_rst_sync` uses 2). Suppress per-module.
- **VARHIDDEN** — Module parameters intentionally shadowing package constants for parameterization (e.g., `SetsPerSubBank`). Suppress per-module.
- **PINCONNECTEMPTY** — Intentionally unconnected RAM primitive outputs (`alert_o`, write-port `rdata_o`). Suppress around the instantiation.
- **SYNCASYNCNET** — Async-assert/sync-deassert reset used in both `always_ff` sensitivity lists and assertion logic. Expected from our reset strategy. Suppress per-module.
- **UNOPTFLAT** — Packed structs with independent fields assigned from separate combinational paths. Verilator treats the struct as one signal and reports a false circular dependency. Verify the individual field paths are acyclic before suppressing.

**Never** use `-Wno-fatal` to blanket-suppress warnings. Each suppression must be targeted and documented.

### Assertions

Use Verilator-compatible style:

```systemverilog
// synthesis translate_off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */
  always_ff @(posedge clk_i) begin
    if (rst_ni && bad_condition)
      $error("message");
  end
  /* verilator lint_on SYNCASYNCNET */
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni) ...)
  else $error("message");
`endif
// synthesis translate_on
```

## DV / testing

- Primary simulator: **Verilator 5+** (open-source, CI-friendly)
- Test harness: `dv/common/sim_ctrl.h` — provides `tick()`, `reset()`, `check()`, `finish()`
- Tests are C++ files that drive the Verilator model directly
- `make test` must always pass. `make lint` must always pass.
- Each test prints `TEST PASSED` or `TEST FAILED: N error(s)` followed by coverage percentage
- VCD tracing via `make -C hw/ip/<block>/dv test-trace`
- Coverage is always collected (`--coverage`). `make coverage-report` shows per-IP coverage from the last run.

Every reimplemented module **must** have **both**:
1. **Unit tests** in `hw/ip/<block>/dv/` with explicit `check()` assertions verifying specific expected values
2. **RTL cosim** in `dv/rtlcosim/<module>/` comparing against the original cycle-by-cycle

Do not commit a module with only one of these. Both are required, delivered together.

### Cosim requirements

Every reimplemented module **must** have a corresponding RTL co-simulation test in `dv/rtlcosim/<module>/` that:
- Instantiates both the original CORE-ET module and the new reimplementation in the same Verilator testbench
- Drives identical stimulus to both
- Compares **ALL outputs** cycle-by-cycle using `CosimCtrl::compare()` — **no exceptions, no skipped signals**
- Reports `COSIM PASSED` with zero mismatches

This includes **every new primitive**. Do not write "tested via parent module cosim" — the primitive must have its own standalone cosim against the original it replaces.

**Cosim coverage requirements:** The cosim must exercise **every input port** with non-trivial stimulus. A cosim that passes 100K comparisons but never drives a port to 1 is a bad cosim — it will miss bugs in that path. Specifically:
- Every input port must be driven to both 0 and 1 at least once during random stimulus
- Miss/fill sequences, debug paths, halt/resume, fault injection — if the original has these paths, the cosim must test them
- Use multiple random seeds when feasible
- When a module is integrated into a parent, the parent's cosim often catches bugs the standalone cosim missed — but this is not an excuse for a weak standalone cosim

The original modules are referenced via `ORIG_ROOT`, never copied into this repo. The cosim SV wrapper adapts port differences (reset polarity, DFT struct vs individual ports, etc.).

When a cosim fails, **fix the module**, not the test. The only exception is Verilator simulation artifacts (e.g., initialization differences) which must be documented explicitly in the test.

### Cosim thoroughness

Cosims must be proportional to module complexity:
- Simple combinational modules: 2,000+ comparisons
- Pipelined modules (~200 LOC): 5,000+ comparisons
- Complex FSMs (~500 LOC): 50,000+ comparisons
- Large controllers (~1000+ LOC): 500,000+ comparisons
- Include random stimulus phases (1,000–5,000 cycles depending on complexity)
- Test every FSM state, every packet type, every operating mode — not just happy paths

Run all co-simulations: `make -C dv/rtlcosim test`

Coverage is always collected for both unit tests and cosim. View coverage:
- `make coverage-report` — generate LCOV `.info` files (line/branch/toggle) and print summary
- `make coverage-html` — generate interactive coverage dashboard (`build/coverview/index.html`)
- LCOV `.info` files are written to `build/coverage/` for use with `genhtml`, `lcov`, or other tools

Coverage file policy:
- `build/coverage/*.info` and `build/coverview/index.html` are generated outputs. Do not commit anything under `build/`.
- Unit-test coverage is consumed locally or in CI from generated `build/coverage/*.info`. Do not check in unit-test coverage snapshots.
- Cosim coverage snapshots are checked in at `dv/rtlcosim/coverage/*.info`. These are the only committed coverage data files.
- `dv/coverview/index.html` is the checked-in Coverview frontend/template asset, not a run-specific generated report. Keep it tracked.

### Cosim coverage in CI

CI does not run cosim tests (they require a local original CORE-ET RTL checkout). Instead, cosim LCOV `.info` files are checked in at `dv/rtlcosim/coverage/` and used by CI to build the combined coverage dashboard.
CI generates the final dashboard artifact from:
- current unit-test coverage in `build/coverage/*.info`
- checked-in cosim coverage in `dv/rtlcosim/coverage/*.info`
- the checked-in Coverview assets in `dv/coverview/`

The CI-uploaded dashboard is `build/coverview/index.html`. That generated file must not be committed.

When cosim tests change (new modules, updated tests), regenerate and commit the checked-in `.info` files:

```bash
make -C dv/rtlcosim test                  # run all cosims
make update-cosim-coverage                # regenerate dv/rtlcosim/coverage/*.info
git add dv/rtlcosim/coverage/
git commit -m "Update cosim coverage data"
```

For scoped jobs that intentionally run only new or changed cosims, do **not**
commit direct `make update-cosim-coverage` output after a clean plus targeted
run if it replaces the global checked-in snapshots with targeted-only records.
Instead, preserve the checked-in `dv/rtlcosim/coverage/*.info` baseline, save the
targeted `dv/rtlcosim/build/coverage/coverage_*_cosim.info` output, then merge or
append only the targeted module records into the checked-in files. Before review,
verify `git diff -- dv/rtlcosim/coverage/` is scoped to the targeted cosims
(insertion-only for newly added cosims). If a safe targeted merge cannot be
produced and audited, run the full flow above. Generated `build/` outputs remain
untracked and must not be committed.

### Writing a new cosim test

A cosim test has three files in `dv/rtlcosim/<module>/`:

1. **`Makefile`** — includes `mk/rtlcosim.mk` (see below)
2. **`cosim_<name>_tb.sv`** — SystemVerilog wrapper that instantiates both old and new modules, adapts port differences (reset polarity, DFT struct, naming), and exposes all outputs for comparison
3. **`cosim_<name>_test.cc`** — C++ test driver using `CosimCtrl<>` from `dv/rtlcosim/common/cosim_ctrl.h`

The C++ test:
```cpp
#include "Vcosim_<name>_tb.h"
#include "cosim_ctrl.h"
int main(int argc, char** argv) {
    CosimCtrl<Vcosim_<name>_tb> sim(argc, argv);
    sim.reset();
    // Drive inputs, call sim.tick()
    // Compare outputs each cycle:
    sim.compare("signal_name", sim.dut->orig_out, sim.dut->new_out);
    return sim.finish();
}
```

The SV testbench adapts differences between old and new:
- Reset polarity: old uses active-high `reset`, new uses active-low `rst_ni`
- DFT: old uses individual `dft__*` ports, new uses `dft_pkg::dft_t` struct
- Naming: old uses `clock`/`reset`, new uses `clk_i`/`rst_ni`
- When old and new have the same module name, use `ORIG_RENAME` (see below)

### Cosim Makefile structure

Cosim Makefiles use `mk/rtlcosim.mk` for shared build rules. A typical cosim Makefile:

```makefile
REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := <short_name>     # matches cosim_<short_name>_tb.sv

NEW_RTL  := ...                # new module .sv files (packages first)
ORIG_RTL := ...                # original module .v files

# Optional:
ORIG_LIBDIRS := ...            # -y search paths for original RTL dependencies
EXTRA_FLAGS  := -DET_ASSERT_OFF  # extra Verilator flags
SIM_ARGS     := +verilator+noassert  # extra runtime arguments
ORIG_RENAME  := <module_name>  # rename ORIG_RTL modules to <name>_orig (see below)

include $(REPO_ROOT)/mk/rtlcosim.mk
```

### Module renaming (ORIG_RENAME)

When old and new modules share the same name, Verilator can't compile both in one testbench. `ORIG_RENAME` handles this by copying each ORIG_RTL file and renaming the listed module names with an `_orig` suffix. The SV testbench then instantiates `<name>` (new) and `<name>_orig` (old).

- **Single module:** `ORIG_RENAME := intpipe_alu`
- **Multiple modules** (integration cosims): list all explicitly, longest names first to avoid partial matches:
  ```makefile
  ORIG_RENAME := intpipe_csr_file_fl_barrier \
                 intpipe_csr_file_conv \
                 intpipe_csr_file \
                 intpipe_csr_replay \
                 intpipe_csr_msgs \
                 intpipe_csr_pmu_read_interface
  ```

Every name in the list is renamed in **every** ORIG_RTL file (both module declarations and instantiations).

### Before committing

Before committing any module change, **always**:

```bash
# 1. Run full lint
make lint

# 2. Run all unit tests
make test

# 3. Run all cosim tests
make -C dv/rtlcosim test

# 4. Update the checked-in cosim coverage data
make update-cosim-coverage

# 5. Commit everything together (including dv/rtlcosim/coverage/, but not build/)
git add -A
git commit
```

Plain `git diff --check` only checks tracked changes; it does **not** inspect
untracked new files. Before review, and again before integration, jobs that add
new source, DV, cosim, documentation, or coverage files must make those paths
visible to a whitespace check. Use one of these safe workflows:

```bash
# Option A: keep files unstaged but mark new approved paths as intent-to-add.
git add -N <new-approved-paths>
git diff --check -- <new-approved-paths>

# Option B: stage only the approved paths for a commit-style check, then unstage
# if the job should leave the worktree unstaged for review.
git add <approved-paths>
git diff --cached --check
git reset -- <approved-paths>
```

Do not stage generated `build/` outputs, unit-test coverage, or unrelated local
changes just to make a whitespace check see new files. If generated or unrelated
paths are present, scope the `git add -N`, `git add`, and diff-check commands to
the approved artifact paths only.

Do not commit or close out work with failing lint or tests. Do not commit without updating the cosim coverage `.info` files when cosim behavior changes — CI uses these for the coverage dashboard. Do not commit generated `build/coverage/*` or `build/coverview/index.html`.

## STATUS.md

`STATUS.md` tracks every module, test, and cosim in the project. **Update it whenever you:**
- Add a new module (add a row with its status)
- Add or update tests (update the checks count)
- Add or update a cosim (update the comparisons count)
- Change a module's status (Not started → Done, etc.)
- Update the totals at the bottom

Follow the existing table format. Keep the same section grouping (Packages, Primitives, Shire Cache sub-sections). Always keep the totals accurate.

## Build system

- `mk/verilator.mk` — shared Verilator build rules with coverage, included by each IP's `dv/Makefile`. Supports single-test and multi-test modes. Multi-test per-test variables: `<t>_TOP`, `<t>_SRCS`, `<t>_ARGS`, `<t>_FLAGS`. With `TESTS := foo bar`, the runnable per-test targets are `test-foo`, `test-bar`, `test-xrand-foo`, and `test-xrand-bar`; raw `foo`/`bar` targets do not exist unless a local Makefile defines them separately.
- `mk/rtlcosim.mk` — shared RTL co-simulation build rules, included by each cosim `dv/rtlcosim/<module>/Makefile`. Handles `ORIG_ROOT`, coverage, module renaming, and per-file coverage reporting.
- `mk/yosys.mk` — shared Yosys FPGA synthesis rules using the slang SystemVerilog frontend, included by project synthesis heads
- `mk/prim.mk` — technology-specific primitive selection. Set `TECH` (generic, ice40, xilinx) before including; provides `PRIM_*` variables pointing to the correct source files
- Per-IP Makefiles set: `TB_TOP`, `RTL_SRCS`, `CC_SRCS`, optionally `INCDIRS` and `SIM_ARGS`
- Top-level `Makefile` auto-discovers all `hw/ip/*/dv/Makefile` and `hw/ip/*/*/dv/Makefile` targets. CI consumes this discovery via `ci-unit-test-dirs` and `ci-unit-test-matrix`, so keep new unit tests in those layouts or update the helper and workflow together.
- `dv/rtlcosim/Makefile` — runs all cosim tests

## Projects

A **project** combines IPs into a design. It has a shared project top module and multiple **heads** — one per backend target. Each head has its own Makefile that includes the appropriate shared rules (`mk/yosys.mk` or `mk/verilator.mk`).

### Project directory structure

```
fpga/<project_name>/
  README.md               Required — describe the project, ports, IPs used, resource utilization
  rtl/
    <project_name>.sv     Project top module — shared across all heads
  ice40/
    Makefile              iCE40 synthesis head (includes mk/yosys.mk)
  xilinx/
    Makefile              Xilinx synthesis head (includes mk/yosys.mk)
  verilator/
    Makefile              Simulation head (includes mk/verilator.mk)
    <project_name>_test.cc  C++ test driver using sim_ctrl.h
```

Not all heads are required. A project may have only synthesis heads, only a simulation head, or any combination.

### Synthesis head Makefile

Each synthesis head Makefile sets variables and includes `mk/yosys.mk`:

```makefile
REPO_ROOT := $(shell git rev-parse --show-toplevel)

SYNTH_TOP := <project_name>
SYNTH_CMD := synth_ice40                       # or: synth_xilinx -family xcup
RTL_SRCS  := $(REPO_ROOT)/hw/ip/<ip>/rtl/<ip>.sv \
             $(CURDIR)/../rtl/<project_name>.sv

include $(REPO_ROOT)/mk/yosys.mk
```

Variables for `mk/yosys.mk`:

| Variable | Required | Description |
|----------|----------|-------------|
| `SYNTH_TOP` | yes | Top module name for synthesis |
| `RTL_SRCS` | yes | All SV source files (packages listed before modules that import them) |
| `SYNTH_CMD` | yes | Yosys synthesis command (e.g. `synth_ice40`, `synth_xilinx -family xcup`, `synth_ecp5`) |
| `CHPARAMS` | no | Yosys `chparam` overrides (e.g. `-set Width 16`) |

Targets provided: `synth`, `report`, `synth-clean`.

Synthesis uses Yosys with the **slang** SystemVerilog frontend. Add project-specific include directories or frontend options through `YOSYS_SLANG_FLAGS`, not by preprocessing RTL outside the shared rule.

### Simulation head Makefile

Same pattern as IP-level `dv/Makefile` — sets `TB_TOP`, `RTL_SRCS`, `CC_SRCS` and includes `mk/verilator.mk`.

### Adding a new project

1. Create `fpga/<project_name>/README.md` — describe the project, its ports, IPs used, and resource utilization
2. Create `fpga/<project_name>/rtl/<project_name>.sv` — project top module
3. Create one or more heads with Makefiles that include the appropriate `mk/*.mk`
4. For simulation heads, create a C++ test using `sim_ctrl.h` from `dv/common/`

See `fpga/fifo_example/` as the reference template.

## Reimplementation rules

Before changing translated RTL, read `docs/translation.md`. That file is the
maintained project-wide CORE-ET -> ETASP translation reference. Do not rely
on memory, scattered comments, or one block's local README when deciding how a
translated pattern should be represented.

### Translation checklist

For every translated site, check all of these before you commit:

1. Confirm the original behavior is being preserved, not redesigned.
2. Confirm reset-domain separation still matches the original.
3. Confirm the translated form uses the project primitive or named composite
   primitive when one exists, instead of an open-coded local variant.
4. Confirm latch/phase-sensitive behavior is still explicit where the original
   depends on it.
5. Confirm any FPGA-specific simplification is confined to the technology tree,
   not hidden in shared RTL.
6. Confirm the affected IP README documents any intentional difference from the
   original.
7. Confirm standalone unit test and standalone cosim both cover the changed
   primitive/module, not only a parent integration.

**The reimplementation is a translation, not a redesign.** Every signal, every condition, every pipeline stage must map 1:1 to the original. Do not make architectural decisions — convert the Verilog mechanically to SystemVerilog. Do not replace memory models (e.g. latches with flip-flops), do not remove ports, do not change write masks or timing, do not skip signals because you think they are "just for clock gating."

Each reimplemented module must **preserve the exact same behavior and interface** as the original CORE-ET module:

- **Same cycle-by-cycle timing.** If the original has 1-cycle read latency, the reimplementation must too. Do not change pipeline depths, handshake protocols, or signal timing.
- **Same latch/phase semantics where they matter.** If the original uses transparent latches, low-phase capture, or explicit phase-2 enables, preserve that behavior. Do not replace it with negedge flops or clock enables just because the rewritten code looks simpler.
- **Same port semantics.** Every parameter, port, and protocol of the original must be replicated. If the original has a `Ports` parameter that changes behavior, include it.
- **Preserve configurable features as configuration, not preprocessor.** When the original uses `ifdef`s to switch real functionality on or off across a block family, the reimplementation should model that as explicit parameters shared consistently across the translated modules. Do not silently hard-wire a compile-time feature off in one module while treating it as configurable elsewhere.
- **Document differences.** Each IP README must include a table listing every intentional difference from the original (reset polarity, naming convention, DFT struct vs individual ports, etc.) with rationale.
- **Allowed differences** are limited to:
  - Reset polarity/style (active-low async-assert/sync-deassert instead of active-high synchronous)
  - Naming conventions (lowRISC `_i`/`_o`/`_d`/`_q` instead of `_ff`/`_nxt`)
  - DFT port consolidation (`dft_pkg::dft_t` struct instead of individual `dft__*` signals)
  - RAM wrapper abstraction (`prim_ram_1p` instead of direct foundry SRAM instantiation)
  - Technology-specific cell abstraction (`prim_clk_gate` instead of foundry ICG cell name)
  - Coding style (explicit `always_ff` instead of `RST_FF`/`EN_FF` macros, packages instead of `define` headers)
- **Not allowed:** dropping tapeout features (DFT, clock gating, ECC, BIST hooks, RAM config), changing handshake timing, adding or removing pipeline stages, altering data path widths, or modifying the functional interface.
- **Never "fix bugs" in the original.** If the original has a specific behavior — even one that looks wrong — replicate it exactly. The original was taped out and verified. If a cosim shows a mismatch, fix YOUR code, not the test.
- **Never skip cosim comparisons.** Every output must be compared on every cycle. If you exclude a signal "because it differs," that means your module is wrong. Fix it.
- **Never collapse separate signals.** If the original has separate reset domains (`reset_d`, `reset_w`), separate clock domains, or separate control paths, preserve them all. Do not merge them into one signal for "simplicity."
- **Preserve reset domains.** If the original resets register X with `reset_d` and register Y with `reset_w`, the reimplementation must have corresponding separate reset inputs. Map `reset_d` → `rst_d_ni`, `reset_w` → `rst_w_ni` (both active-low async). Do NOT collapse into a single `rst_ni` unless the original truly uses only one reset.
- **Never "fix bugs" in the original.** If the original has a specific behavior — even one that looks wrong — replicate it exactly. The original was taped out and verified. If a cosim shows a mismatch, fix YOUR code, not the test.
- **Never skip cosim comparisons.** Every output must be compared on every cycle. If you exclude a signal "because it differs," that means your module is wrong. Fix it.
- **Never collapse separate signals.** If the original has separate reset domains (`reset_d`, `reset_w`), separate clock domains, or separate control paths, preserve them all. Do not merge them into one signal for "simplicity."
- **Preserve reset domains.** If the original resets register X with `reset_d` and register Y with `reset_w`, the reimplementation must have corresponding separate reset inputs. Map `reset_d` → `rst_d_ni`, `reset_w` → `rst_w_ni` (both active-low async). Do NOT collapse into a single `rst_ni` unless the original truly uses only one reset.

## Primitive mapping (CORE-ET -> ETASP)

When reimplementing modules that use CORE-ET library primitives, use these replacements:

| Original CORE-ET source | Replacement (this repo) | Notes |
|--------------------------|------------------------|-------|
| `gen_mem1p` | `prim_ram_1p` | RAM wrapper — behavioral for sim/FPGA, foundry SRAM for ASIC |
| `gen_mem2p` | `prim_ram_2p` | Dual-port RAM wrapper — same abstraction |
| Synopsys ASIC SRAMs (`saduls0g4l1p*`, etc.) | `prim_ram_1p` / `prim_ram_2p` | Abstracted behind wrapper, never instantiated directly |
| `gen_fifo_reg` | `prim_fifo_reg` | Register FIFO (depth 1-4) |
| `rbox_fifo` | `prim_fifo_sram` | SRAM-backed FIFO with 1-cycle read latency |
| `rst_repeat` | `prim_rst_sync` | Reset synchronizer with DFT bypass via `dft_t` |
| `arb_lru_grant` | Precisely matched grant-visible-on-stall arbiter seam/helper | Original keeps `grant` visible while `stall` is asserted and stalls only priority updates; `prim_arb_lru` gates `grant_o` with `~stall_i`, so do not use it when grant-visible-on-stall behavior is required. |
| `hot2bin` + `onehot_mux` | `prim_hot2bin` | One-hot to binary encoder |
| `vcfifo_wr_hiv_gcd` | `prim_fifo_async_hiv` | Async CDC FIFO, write=high-voltage, read=low-voltage. DFT bypass via `dft_hv_i`/`dft_lv_i` |
| `vcfifo_wr_lov_gcd` | `prim_fifo_async_lov` | Async CDC FIFO, write=low-voltage, read=high-voltage. DFT bypass via `dft_lv_i`/`dft_hv_i` |
| `et_clk_gate` | `prim_clk_gate` | Technology-swappable clock gating cell with DFT override |
| `et_clk_gate_n` | `prim_clk_gate_n` | Negative-phase clock gating cell for translated low-phase capture paths |
| `et_clk_mux2` | `prim_clk_mux` | Clock multiplexer for DFT SRAM clock override |
| `et_clk_gate` + `rlatch` narrowed to an enabled write-state commit | `prim_write_commit_en` | Behavior-named seam for write-driven state updates without reset |
| `et_clk_gate` + resettable `rlatch` narrowed to an enabled write-state commit | `prim_write_commit_rst_en` | Behavior-named seam for write-driven state updates with reset |
| `et_clk_gate_n` + `rlatchn` / low-phase capture narrowed to preview staging | `prim_write_preview_en` | Behavior-named seam for one-phase-early write qualifiers or payload staging |
| direct `rlatchn` -> `rlatch` pair with both phase outputs live | `prim_phase_pair_lo_hi` | Named composite for low-phase to high-phase handoff |
| direct `rlatch` -> `rlatchn` pair with both phase outputs live | `prim_phase_pair_hi_lo` | Named composite for high-phase to low-phase handoff |
| low-phase preview latch feeding `rf_latch_1r_1w` | `prim_rf_1r1w_preview` | Named composite for a semantic same-width RF write-preview seam |
| low-phase preview latch feeding `rf_latch_1r_1w_diff_widths` | `prim_rf_1r1w_diff_preview` | Named composite for a semantic mixed-width RF write-preview seam |
| `` `RST_FF ``, `` `EN_FF ``, `` `RST_EN_FF `` | Explicit `always_ff` with `_d`/`_q` | No flip-flop macros |
| `` `ZX(W, sig) `` | `sig[W-1:0]` | Avoid parameterized-width casts in shared RTL |
| `` `SX(W, sig) `` | Explicit sign-extension | Avoid parameterized-width casts in shared RTL |
| `rf_latch_1r_1w` | `prim_rf_1r1w` | Same-width register file |
| `rf_latch_1r_1w_reg` | `prim_rf_1r1w_reg` | Same-width register file with registered read address |
| `rf_latch_1r_1w_par` | `prim_rf_1r1w_par` | Same-width register file with parallel readback |
| `rf_latch_1r_2w_par` | `prim_rf_1r2w_par` | Same-width dual-write register file with parallel readback |
| `rf_latch_3r_2w` | `prim_rf_3r2w` | Three-read / two-write register file |
| `rf_latch_1r_1w_diff_widths` | `prim_rf_1r1w_diff` | Different read/write width register file |
| `rf_latch_single_1r_1w_par` | `prim_rf_single_1r1w_par` | Single-entry write-capture primitive with direct readback |
| `rf_latch_2r_1w` | `prim_rf_2r1w` | Two-read / one-write register file |
| `en_ff_gated_struct` | `prim_clk_gate` + `always_ff` | Clock gating via primitive, not custom wrapper |
| `esr_shire_cache_ram_cfg_t` | `ram_cfg_pkg::ram_cfg_t` | Clean standardized RAM config struct |
| DFT ports (`dft__*`) | `dft_pkg::dft_t dft_i` | Consolidated into single struct |
| UltraSoC modules | Drop (replace with own debug IP later) | Third-party debug/trace IP |

Do not silently substitute a primitive whose stall semantics differ from the
original. In particular, an `arb_lru_grant` site that observes `grant` while
`stall` is asserted needs a precisely matched primitive or named local helper
with the required unit test, standalone cosim, `STATUS.md`, and checked-in
cosim coverage deliverables.

### Latch-heavy translation policy

Raw high-phase/low-phase latch leaves are not public translation seams in the
current tree.

Use these rules instead:

- If a translated site is only an internal latch step inside a larger
  primitive or technology-specific block, keep that phase-transparent behavior
  local to the owner.
- If the site is a reusable contract at the translated block boundary, use a
  self-contained named seam such as `prim_phase_pair_*`,
  `prim_write_*`, `prim_rf_*_preview`, or
  `prim_mul_div`.

### Latch Translation Policy

- Raw latch leaves are no longer public translation seams. Do not introduce new
  shared-RTL dependencies on `prim_latch_*`.
- If a translated site is really a write-preview or write-commit behavior, use
  `prim_write_preview_en`, `prim_write_commit_en`, or
  `prim_write_commit_rst_en`.
- If a translated site really needs both phase nodes architecturally live, use
  `prim_phase_pair_lo_hi` or `prim_phase_pair_hi_lo`.
- If exact transparent behavior still has to exist and does not fit one of the
  public seams above, keep that behavior private inside the owning primitive or
  technology-specific implementation. Public primitives should be self-contained.
- FPGA-specific simplifications belong in `tech_*`, not in shared RTL.
- Do not keep one public primitive layered on top of deleted raw latch leaves
  just to spell out its internals.
- Any FPGA-specific approximation must stay behind the named seam, not leak
  into shared RTL.

When to introduce a new named composite instead of more raw latches:

- The original pattern is repeated many times inside one translated block.
- Both phase outputs remain live in the translated shared RTL, so the pattern
  is more than a single enabled latch.
- The pattern has a stable semantic meaning at the block boundary, such as a
  two-phase loop counter, ping-pong accumulator state, or a phase-local status
  handoff.

What must not change when doing this:

- parent-visible cycle timing
- original phase ordering
- reset-domain separation
- DFT/control surfaces

The implementation may differ only inside the technology tree. For example:

- `tech_generic` may still realize the composite as the exact original
  gate/latch chain
- `tech_ecp5` may realize the same composite as a phase-correct dual-edge or
  CE-based approximation if that is what the backend can build

## Architecture context

This SoC is derived from the CORE-ET architecture:
- **Shire** — RISC-V processing tile (minion core, ESR, rbox, tbox, cache, channels, neighborhood)
- **Mesh NoC** — Network-on-Chip interconnect
- **Memshire** — Memory controller subsystem
- **IOshire** — IO peripherals, accelerator interface
- **Pshire** — System control, PCIe, boot

Only Ainekko-owned CORE-ET IP is reimplemented. All third-party IP (Synopsys, UltraSoC, Movellus, Moortec, InsideSecure, TSMC) is dropped — replaced by clean technology-abstracted primitives or Ainekko-owned alternatives.

## What NOT to do

- Do not use `` `define `` for constants or types. Use parameters and packages.
- Do not use `always @*` or `always @(posedge clk)`. Use `always_comb` / `always_ff`.
- Do not add `initial` blocks in synthesizable RTL.
- Do not add UVM infrastructure — use Verilator + C++ tests.
- Do not use SV interfaces for inter-module ports — use packed struct bundles for Verilator compatibility.
- Do not instantiate bare `logic mem[]` in functional RTL — use `prim_ram_1p` / `prim_ram_2p`.
- Do not instantiate foundry cells directly — use `prim_*` technology abstractions.
- Do not drop tapeout features (DFT, clock gating, ECC, BIST, RAM config). Abstract them cleanly.
- Do not create global `rtl/` or `tb/` directories — keep RTL and DV co-located per IP block.

## Subagent instructions

When spawning subagents (via the Agent tool) to implement modules, **always** include in the prompt:
1. "Read AGENTS.md at `<repo_root>/AGENTS.md` first and follow all rules."
2. An explicit checklist of ALL required deliverables per AGENTS.md:
   - RTL source file
   - README.md for the IP block
   - Unit test (Makefile + C++ test using `sim_ctrl.h`)
   - RTL cosim (Makefile + SV TB + C++ test using `cosim_ctrl.h`)
   - Makefile updates (LINT_SRCS, test entries)
   - STATUS.md update
   - `make lint` must pass clean
3. Testing must be **thorough** — every FSM state, every opcode, every major path. Shallow tests that only cover happy paths are not acceptable.
