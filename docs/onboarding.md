# Working on ETASP

This document captures everything needed to work productively on this project. Read it before writing any code.

## What this project is

Ainekko owns the CORE-ET IP and is reimplementing CORE-ET modules in clean, minimal SystemVerilog. The original Verilog is referenced through `ORIG_ROOT` for co-simulation. Only Ainekko-owned CORE-ET blocks are reimplemented — all third-party IP (Synopsys, UltraSoC, Movellus, etc.) is dropped and replaced with clean abstractions.

Targets both FPGA prototyping and ASIC tapeout. All tapeout infrastructure (DFT, clock gating, RAM wrappers, ECC, BIST hooks) must be present.

## Mandatory reading before ANY work

1. `AGENTS.md` — The definitive rules. Every rule is enforced.
2. `docs/coding_style.md` — lowRISC conventions.
3. `docs/translation.md` — project-wide CORE-ET -> ETASP translation rules.
4. `STATUS.md` — Current state of every module. Update whenever you change anything.

## The workflow for EVERY module (no exceptions)

Every reimplemented module requires ALL of these, delivered together:

1. **RTL** in `hw/ip/<block>/rtl/<module>.sv`
2. **Unit test** in `hw/ip/<block>/dv/` — TB wrapper `.sv` + test `.cc` with explicit `sim.check()` assertions
3. **RTL cosim** in `dv/rtlcosim/<module>/` — comparing against original cycle-by-cycle
4. **README.md** at `hw/ip/<block>/README.md` with differences table
5. **STATUS.md** updated

Never commit without all five. Never ship only a cosim without unit tests.

## Key conversion rules (CORE-ET -> ETASP)

This section is a quick summary only. The maintained project-wide translation
reference is `docs/translation.md`.

### Naming
| Original | New |
|----------|-----|
| `clock` | `clk_i` |
| `clock_<block>` (gated) | `clk_<block>_i` |
| `reset` (active-high sync) | `rst_ni` (active-low async-assert/sync-deassert) |
| `signal_ff` | `signal_q` |
| `signal_nxt` | `signal_d` |
| `input_port` | `input_port_i` |
| `output_port` | `output_port_o` |

### Macros → explicit RTL
| Original | New |
|----------|-----|
| `` `RST_FF(clk, rst, q, d, def) `` | `always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) q <= def; else q <= d;` |
| `` `EN_FF(clk, en, q, d) `` | `always_ff @(posedge clk_i) if (en) q <= d;` |
| `` `RST_EN_FF(clk, rst, en, q, d, def) `` | `always_ff @(posedge clk_i or negedge rst_ni) if (!rst_ni) q <= def; else if (en) q <= d;` |
| `` `ZX(W, sig) `` | `sig[W-1:0]` (NOT `W'(sig)` — Yosys synthesis is fragile around parameterized casts) |
| `` `SX(W, sig) `` | `W'($signed(sig))` or explicit sign-extension |
| `` `include "soc.vh" `` | `import <block>_pkg::*;` |

### Primitives
| Original | New |
|----------|-----|
| `gen_mem1p` | `prim_ram_1p` (in `tech_generic/prim_ram_1p/rtl/`) |
| `gen_mem2p` | `prim_ram_2p` |
| `rbox_fifo` | `prim_fifo_sram` |
| `et_clk_gate` | `prim_clk_gate` |
| `et_clk_gate_n` | `prim_clk_gate_n` |
| `et_clk_gate` + `rlatch` narrowed to write-state commit | `prim_write_commit_en` |
| `et_clk_gate` + resettable `rlatch` narrowed to write-state commit | `prim_write_commit_rst_en` |
| `et_clk_gate_n` + low-phase latch capture narrowed to preview staging | `prim_write_preview_en` |
| narrowed write-preview / write-commit path | `prim_write_preview_en` + `prim_write_commit_en` / `prim_write_commit_rst_en` |
| `esr_shire_cache_ram_cfg_t` | `ram_cfg_pkg::ram_cfg_t` + `dft_pkg::dft_t` (separate ports) |

### Helper clocks on FPGA

Do not open-code helper phase clocks in translated shared RTL when a named
composite primitive exists.

The current project rule is:

- real functional/local clocks stay real clocks
- helper-clock behavior should be expressed through named primitives instead
  of open-coded generated clocks
- raw high-phase/low-phase latch leaves are retired as public seams; keep any
  remaining direct transparency local to the owning primitive or block
- if a site has been narrowed to a pure write-preview / write-commit role, use
  the behavior-named write seams instead of the raw latch family
- if both phase nodes remain architecturally live, use `prim_phase_pair_lo_hi`
  or `prim_phase_pair_hi_lo`

See `docs/translation.md` for the exact policy and rationale.

### Types
All `<block>_*_t` types become `<block>_pkg::*` (without the prefix). All `<BLOCK>_*` defines become `<block>_pkg::*` localparams with `UpperCamelCase` names.

### Case statements
- `case (1'b1)` used as priority encoder → `priority case (1'b1)` (NOT `unique case` — multiple matches possible)
- `casez` with default → `unique casez`
- `{$bits(x), expr}` is a CONCATENATION in the original, NOT a replication `{$bits(x){expr}}` — keep the original's exact syntax

### Synthesis compatibility (Yosys + slang)
- No `W'(expr)` parameterized-width casts — use `expr[W-1:0]` instead
- No runtime-dependent `for` loop bounds — use constant bounds with `if` inside
- See `docs/coding_style.md` for full details

### Reset domains
- If the original has separate resets (`reset_d`, `reset_w`), preserve them as `rst_d_ni`, `rst_w_ni`
- NEVER collapse separate resets into a single `rst_ni`

## Build system

- Include `mk/prim.mk` in Makefiles for primitive paths (`$(PRIM_RAM_1P)`, etc.)
- Unit tests use multi-test mode in `mk/verilator.mk`
- Cosim Makefiles are standalone with `-Wno-fatal` for original code warnings
- ESR generation: `make -C hw/ip/esr` reads `hw/ip/esr/data/esr.csv`
- `ORIG_ROOT` points cosims at the local original CORE-ET RTL checkout

## Testing requirements

- Unit tests: `sim.check(condition, message)` with explicit expected values
- Cosims: `sim.compare(name, orig, new)` cycle-by-cycle against original
- When cosim fails: **fix the module, not the test** (unless Verilator artifact, which must be documented)
- Thoroughness: test EVERY FSM path. A 1300 LOC module needs 500K+ comparisons. Random stimulus 1000-5000+ cycles.

## Common pitfalls

1. **`unique case` vs `priority case`** — `case (1'b1)` priority encoders can have multiple matches. Use `priority case`.
2. **`if`/`else if` chaining** — Check if the original has standalone `if` statements that are NOT chained. Don't add `else` where the original doesn't have one.
3. **Bit width in arithmetic** — Don't force narrow casts (e.g., `8'(...)`) on overflow detection arithmetic. Let natural width prevent wraparound.
4. **Leading-one detectors** — Original uses short-circuit `for` loops that don't synthesize. Use LSB-to-MSB scan: `for(b=0; b<N; b++) if(val[b]) result = b;`
5. **Bit-select ranges** — Verify every bit-select against the original defines. `left[14:6]` is very different from `left[14:2]`.
6. **`{$bits(x), expr}`** — This is concatenation (comma), not replication (double braces). The original's exact syntax must be matched.
7. **Pipeline register vs combinational** — Some originals intentionally use combinational signals in later pipeline stages (e.g., LDA `sub_op`). Don't "fix" this by using the registered version.

## Architecture overview

```
Shire (processing tile)
├── Neighborhoods (×4)
│   └── Minions (×8 RISC-V cores per neighborhood)
├── Shire Cache (L2, 4 banks)
│   ├── Pipeline (sub_bank ×4, cbuf, rbuf, idx_cop_sm)
│   ├── Request Queue (reqq_alloc, reqq_entry ×64, dataq)
│   └── Integration (xbar, mesh, bridge)
├── RBOX (GPU rasterization)
├── TBOX (GPU texture)
└── Channel (interconnect)
```

## Git workflow

- Never force-push to main/master
- Create feature branches (e.g., `rbox`, `shirecache/reqq`)
- Squash into thematic commits before merge
- Rebase onto master before pushing
- Commit messages: describe what and why, no AI attribution
