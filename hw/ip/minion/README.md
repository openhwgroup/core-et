# minion

RISC-V minion core — the processing heart of each Shire tile.

## Architecture

The minion is a dual-threaded RV64IMFC + Zicsr + Zifencei core with optional VPU
(vector/graphics coprocessor). It is NOT a collection of independent IPs —
`frontend`, `intpipe`, `dcache`, and `tlb` are tightly coupled sub-blocks
that share types and control signals at the minion level.

```
minion_top
├── core_top
│   ├── frontend_top          ← instruction fetch, decode, issue
│   │   ├── thread_buffer ×N  ← per-thread fetch engine + RVC expand
│   │   ├── thread_sched      ← round-robin thread arbitration
│   │   ├── intpipe_decode    ← integer instruction decoder (SHARED)
│   │   ├── vpu_decoder       ← VPU instruction decoder (SHARED)
│   │   └── arb (ICache)      ← ICache request arbitration
│   │
│   ├── intpipe_top           ← integer pipeline (decode → ALU → WB)
│   │   ├── intpipe_decode    ← same shared module as in frontend
│   │   ├── intpipe_alu
│   │   ├── intpipe_mul_div
│   │   ├── intpipe_rf
│   │   ├── intpipe_csr_file
│   │   └── scoreboards
│   │
│   ├── dcache_top            ← L1 data cache + miss handler
│   │   ├── data/metadata/lru arrays
│   │   ├── miss_handler
│   │   ├── writeback_unit
│   │   ├── atomic_alu
│   │   └── tensor ops (VPU)
│   │
│   ├── tlb_top               ← address translation
│   └── vpu_top (optional)    ← vector/graphics coprocessor
│       └── vpu_decoder       ← same shared module as in frontend
│
├── minion_debug_apb_slv
├── vpu_top                   ← default real translated VPU (`VpuEn=1`)
├── null_vpu                  ← optional integer-only bring-up helper (`VpuEn=0`, non-faithful)
└── pseudo_lru
```

## Decoders

The instruction decoders (`intpipe_decode`, `vpu_decoder`) are
instantiated inside `frontend_top`. Their decoded outputs flow to
intpipe and VPU as inputs — the decoders are NOT shared modules,
they are frontend internals.

## Shared types (minion_pkg)

`minion_pkg.sv` defines types that flow between sub-blocks:

| Type | Flows | Description |
|------|-------|-------------|
| `minion_control_t` | frontend → FIFO → intpipe | Decoded integer pipeline control signals |
| `vm_status_t` | dcache → frontend → ICache | Virtual memory status for TLB |
| `fe_req_t` | intpipe → frontend | Mispredict/redirect request |
| `xcpt_cause_e` | intpipe, dcache, frontend | Exception cause codes |
| `alu_func_e` | intpipe_decode, intpipe | ALU function codes |

VPU boundary types live in `minion_pkg` because `core_top`, translated
`vpu_top`, and the optional integer-only `null_vpu` helper all share the same
request/control bundles.

VPU-internal types live in `vpu/vpu_pkg.sv`; only the minion-facing boundary
bundles stay in `minion_pkg`.

## Directory structure

```
hw/ip/minion/
  README.md                   This file
  rtl/
    minion_pkg.sv             Shared minion-level types
    core_top.sv               Core integration wrapper (frontend + intpipe + dcache)
    minion_top.sv             Top-level minion wrapper (default real VPU; optional null_vpu with VpuEn=0)
    minion_debug_apb_slv.sv   Minion-local debug APB slave
    null_vpu.sv               Integer-only bring-up helper, not a VPU translation
  frontend/
    rtl/
      minion_frontend.sv      Frontend top
      minion_frontend_pkg.sv  Frontend types
      minion_frontend_thread_buffer.sv
      minion_frontend_thread_sched.sv
      minion_frontend_rvc_expander.sv
      intpipe_decode.sv                Integer instruction decoder
      vpu_decoder.sv                   VPU instruction decoder
    dv/                       Frontend tests
  vpu/
    rtl/
      vpu_pkg.sv              Internal VPU-only types and constants
      vpu_bypass.sv           Lane-local RF bypass and short-path pipeline
      vpu_mask.sv             Mask RF, mask ops, and mask scoreboard leaf
      vpu_rf.sv               Lane scalar/vector RF wrapper with latch/MMI modes
      vpu_sh_sw.sv            Short-latency shift/swizzle/compare leaf
      txfma_csa.sv            TXFMA carry-save adder leaf
      txfma_4_2_compressor.sv TXFMA scalar 4:2 compressor leaf
      txfma_4_2_compressor_array.sv  TXFMA width-parameterized 4:2 compressor array
      txfma_booth_ppg_32r4_norm.sv   TXFMA radix-4 Booth partial-product leaf
      txfma_booth_ppg_32r4_msb.sv    TXFMA final-MSB Booth helper leaf
      txfma_booth_ppg_32r4.sv        TXFMA full 17-lane radix-4 Booth front end
      txfma_wallace1.sv              TXFMA first two Wallace reduction levels
      txfma_wallace2.sv              TXFMA final Wallace reduction stage
      txfma_lxd.sv                   TXFMA PS-side leading-distance anticipator leaf
      txfma_trz.sv                   TXFMA early trailing-zero encoder leaf
      txfma_trz2.sv                  TXFMA adder-side trailing-zero anticipator leaf
      txfma_adder.sv                 TXFMA carry-propagate adder leaf
      txfma_align_shf.sv             TXFMA alignment right-shift leaf
      txfma_norm_shf.sv              TXFMA normalization shift leaf
      tima_adder.sv           TIMA 32-bit adder leaf
      tima_top.sv             Three-stage TIMA datapath leaf
      trans_*_rom_case_*.sv   Generated original-equivalent trans ROM helper modules
      trans_top.sv            Transcendental ROM/pipeline leaf
      vpu_trans.sv            Transcendental micro-op sequencer
      vpu_tensora_rf.sv       Tensor A RF wrapper with low-phase write capture
      vpu_tensorb_rf.sv       Tensor B RF wrapper with low-phase write capture
      vpu_tensorc_rf.sv       Tensor C RF wrapper with low-phase write capture
      vpu_tensortmp_rf.sv     Tensor TMP RF wrapper with low-phase write capture
      vpu_lane_tima.sv        Tensor B/C + gated TIMA lane wrapper
      vpu_uinst_decoder.sv    Internal transcendental micro-op decoder
    dv/                       VPU leaf tests
  intpipe/                    Integer pipeline
  dcache/                     L1 data cache
  tlb/                        Address translation lookaside buffer
```

## Feature Parameters

The public feature switches on the translated top wrappers are:

- `minion_top.DebugApbEn`
- `minion_top.DebugMonEn`
- `minion_top.TraceEn`
- `minion_top.VpuEn`
- `core_top.DebugApbEn`
- `core_top.DebugMonEn`
- `core_top.TraceEn`
- `core_top.VpuEn`

These default to `1'b1` so the full professional-grade build remains the
default behavior.

Current scope:

- `DebugApbEn=0` removes the debug APB slave/plumbing in `core_top` and the
  dcache APB debug access path inside `minion_dcache_top`; selected APB access
  phases still complete with `pslverr_o=1` instead of hanging the bus
- `DebugMonEn=0` removes the top-level debug monitor mux in `minion_top` and
  ties the sideband monitor output idle
- `TraceEn=0` forces the exported trace stream idle
- `VpuEn=1` (default) instantiates the translated real `vpu_top` and keeps the
  original minion/VPU/DCache/CSR integration path live
- `VpuEn=0` disables the frontend `vpu_decoder` and forces the exported VPU
  decode/control signals idle while the top uses the intentional integer-only
  `null_vpu`; the same parameter also drives the dcache and CSR-file
  tensor/offload cuts, which remove the dcache tensor/offload engines and
  silence the matching `intpipe` CSR/replay traffic while preserving the scalar
  cache/MMU contract

This is intentionally only the first-stage cut. Architectural debug inside
`intpipe_top` / `intpipe_csr_file` is still present when `DebugApbEn=0` or
`DebugMonEn=0`.

## Differences from original (CORE-ET source tree)

### Structural changes

The original code is flat — all modules in one directory, all types in
one global `soc.vh` include chain. This reimplementation uses:

- **SystemVerilog packages** instead of `include` headers for type definitions
- **Sub-block directories** (`frontend/`, `intpipe/`, etc.) instead of flat layout
- **Shared modules at minion level** instead of duplicated across sub-blocks

### Type ownership

Original: everything in `minion_types.vh`, `vpu_types.vh`, `frontend_types.vh`
via global `soc.vh` include. No concept of package ownership.

Reimplementation:
- `minion_pkg` — types shared across sub-blocks (replaces `minion_types.vh`)
- `minion_frontend_pkg` — frontend-internal types (replaces `frontend_types.vh`)
- `vpu_pkg` — VPU-internal types (replaces `vpu_types.vh` + `trans_types.vh`)

### Coding style

See project-level `AGENTS.md` for full coding style. Key differences
from the original:

| Aspect | Original | Reimplementation |
|--------|----------|------------------|
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert |
| Naming | Verilog `clock`/`reset` | lowRISC `clk_i`/`rst_ni` |
| Types | `include` headers | SystemVerilog packages |
| FF macros | `RST_FF`, `EN_FF` | Explicit `always_ff` with `_d`/`_q` |
| Clock gating | `et_clk_gate` | `prim_clk_gate` |
| Memories | `rf_latch_1r_1w` / `rf_latch_1r_1w_diff_widths` | `prim_rf_1r1w` / `prim_rf_1r1w_diff` |

## Current status

`STATUS.md` is the authoritative source for current test and cosim counts.
High-level status:

| Sub-block | Status | Notes |
|-----------|--------|-------|
| core_top | Done | `core_top.sv` has a passing minion-level smoke test (9 checks), a debug-APB-off test (12 checks), and a passing `dv/rtlcosim/core_top` run (1,539,072 comparisons); `minion_debug_apb_slv.sv` is exercised through that integration path |
| minion_top | Done | `minion_top.sv` defaults to the translated real `vpu_top` and preserves the intentional integer-only `VpuEn=0` `null_vpu` path; it has passing top-level smoke, VPU-off, debug-APB-off, debug-off, retire-path execution tests, and a passing `dv/rtlcosim/minion_top` run against the original wrapper with real-VPU stimulus |
| null_vpu | Done | `null_vpu.sv` is a new integer-only bring-up helper with a standalone smoke test; it is intentionally non-faithful and is meant for `minion_top` configurations that trap all FP/VPU/ML usage rather than execute it |
| frontend | Done | `minion_frontend`, thread buffer/scheduler, both decoders, and the frontend top cosim all pass; see `frontend/README.md` for current comparison counts |
| minion_pkg | Done | Shared minion types extracted from the original global include chain |
| intpipe | Done | RTL and cosims are in place; standalone unit tests still need their own `hw/ip/minion/intpipe/dv/` subtree |
| dcache | Done | Complete for the original non-graphics minion configuration; the `GFX_ENABLED` texsend path remains intentionally deferred |
| tlb | Done | `minion_tlb` has 123 unit checks and 517,146 cosim comparisons |
| vpu | Done for current Minion integration | `hw/ip/minion/vpu/` has translated packages, leaf datapaths/RFs, TXFMA control/exponent/fraction/top wrappers, transcendental ROM/pipe blocks, tensor reduce, tensor FMA/quant/ML datapath integration, standalone closures for `vpu_ctrl`, `vpu_lane`, and `vpu_top`, and real `vpu_top` integrated into default `minion_top`; the separate standalone shell now lives under `hw/ip/standalone_minion/` and is tracked in `STATUS.md` |
