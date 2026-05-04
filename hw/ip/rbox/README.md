# RBOX (Render Box)

Reimplementation of the CORE-ET `rbox_top` — a GPU-style rasterization and depth-testing unit that receives triangle/tile packets via ET-Link, traverses triangles to generate 2x2 quads, performs depth/stencil testing against a cached depth buffer, and outputs fragment-shading work packets.

## What it does

RBOX implements the core fixed-function stages of a 3D graphics pipeline: **rasterization** and **depth/stencil testing**. It sits between triangle setup (done in software on minion cores) and fragment shading (also software):

```
vertex shader (software) → triangle setup (software) → RBOX → fragment shader (software)
```

- **Input**: triangles defined by edge equations + screen-space tiles
- **Processing**: determines which pixels each triangle covers, tests against a depth buffer, performs stencil operations
- **Output**: barycentric coordinates (i, j), depth values, and coverage masks for surviving pixels

This enables rendering of textured 3D meshes, MSAA antialiasing (2x/4x/8x/16x), all Vulkan depth comparison and stencil operations, scissor clipping, depth clamping, and multi-drawcall rendering — with up to 64 minion cores doing fragment shading in parallel.

RBOX does not handle vertex transformation, triangle setup, fragment shading, texture sampling (TBOX), or framebuffer blending. Those stages are implemented in software on the minion cores.

See [doc/rbox.md](doc/rbox.md) for the full architecture, software programming model, ESR registers, and packet format documentation.

## Status

**Done** — all 18 modules implemented with cosims (~3.3M comparisons, 0 mismatches). Dual reset domains (rst_d_ni for debug, rst_w_ni for warm) correctly preserved in rbox_top, rbox_dbg_ram, and rbox_esr.

## Module hierarchy

```
rbox_top                          (top-level, clock gating, ESR, debug)
├── rbox_ctrl                     (master control: input/output buffers, ESR decode)
│   ├── rbox_input_fifo           (32-entry input FIFO via prim_ram_1p)
│   └── rbox_output_buffer        (128-entry output, 2x prim_fifo_sram)
├── rbox_traverse                 (triangle rasterization, quad generation)
│   ├── rbox_sampler              (edge function sampling, 4x rbox_step_mul)
│   ├── rbox_sample_to_fp32       (edge sample → FP32, 2-stage)
│   └── rbox_depth_offset         (depth interpolation offset)
│       └── rbox_fp_adder_2stg    (2-stage IEEE754 FP adder)
│           └── rbox_fp_shifter   (bidirectional barrel shifter)
├── rbox_test                     (depth/stencil testing)
│   ├── rbox_depth_buffer         (64-entry depth cache, prim_ram_2p)
│   ├── rbox_depth_to_fp          (internal depth → FP32)
│   ├── rbox_depth_to_un          (FP32 → UNORM)
│   └── rbox_fp_fast_adder        (combinational IEEE754 FP adder)
│       └── rbox_fp_shifter
├── rbox_esr                      (APB register block, 8 registers)
└── rbox_dbg_ram                  (APB debug RAM interface)
```

## Package

`rbox_pkg` defines all constants, types, and utility functions:
- 60+ `localparam` constants (dimensions, buffer sizes, bit widths)
- 12 enumerations (packet types, tile sizes, depth formats, MSAA modes)
- 30+ packed structs (edge equations, packets, render state, ESR config)
- 22 utility functions (coefficient extraction, MSAA positions, hart mapping)

## Port interface (rbox_top)

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | `logic` | System clock |
| `rst_d_ni` | input | `logic` | Debug reset (active-low async) — halt/resume FSM |
| `rst_w_ni` | input | `logic` | Warm reset (active-low async) — functional state, ESR, clock gate |
| `shire_id_i` | input | `logic [7:0]` | Shire tile ID |
| `rbox_sc_req_o` | output | `etlink_pkg::req_t` | Cache request |
| `rbox_sc_req_valid_o` | output | `logic [4:0]` | Per-bank + shared valid |
| `rbox_sc_req_ready_i` | input | `logic [4:0]` | Per-bank + shared ready |
| `sc_rbox_rsp_i` | input | `etlink_pkg::rsp_t` | Cache response |
| `sc_rbox_rsp_valid_i` | input | `logic` | Response valid |
| `sc_rbox_rsp_ready_o` | output | `logic` | Response ready |
| `dft_i` | input | `dft_pkg::dft_t` | DFT control |
| `ram_cfg_i` | input | `ram_cfg_pkg::ram_cfg_t` | RAM timing config |
| `rc_i` | input | `rbox_pkg::rc_t` | Debug run control |
| `rc_ack_o` | output | `rbox_pkg::rc_ack_t` | Run control ack |
| `apb_req_i` | input | `apb_pkg::req_t` | APB ESR request |
| `apb_rsp_o` | output | `apb_pkg::rsp_t` | APB ESR response |

## Differences from original

| Aspect | Original (`rbox_top`) | Reimplementation | Rationale |
|--------|----------------------|------------------|-----------|
| Reset | Active-high sync `reset_d`/`reset_w` (two resets) | Active-low async `rst_d_ni`/`rst_w_ni` (two resets) | Preserves original reset domains: debug (halt/resume FSM) and warm (functional state, ESR, clock gate) |
| Naming | `_ff`/`_nxt`, bare ports | `_q`/`_d`, `_i`/`_o` suffixes | lowRISC convention |
| DFT | Individual `test_en` port | `dft_pkg::dft_t dft_i` struct | Consolidated DFT |
| RAM config | `esr_shire_cache_ram_cfg_t` | `ram_cfg_pkg::ram_cfg_t` + `dft_pkg::dft_t` | Separate config and DFT ports |
| Clock gate | `et_clk_gate` | `prim_clk_gate` | Technology abstraction |
| Memories | `gen_mem1p`/`gen_mem2p`/ASIC SRAMs | `prim_ram_1p`/`prim_ram_2p` | RAM wrapper abstraction |
| FIFOs | `rbox_fifo` | `prim_fifo_sram` | Unified FIFO primitive |
| Types | `` `include "soc.vh" `` + `` `defines `` | `rbox_pkg::` package | No global defines |
| Macros | `` `RST_FF ``, `` `EN_FF ``, `` `SX ``, `` `ZX `` | Explicit `always_ff`, SV casts | No flip-flop macros |
| ET-Link | `et_link_req_info_t` / `et_link_rsp_info_t` | `etlink_pkg::req_t` / `rsp_t` | Clean package types |
| APB | `APB_to_rbox_ESRs_t` / `APB_from_rbox_ESRs_t` | `apb_pkg::req_t` / `rsp_t` | Generic reusable types |
| Debug RC | `bpam_rc_rbox_t` / `bpam_rc_rbox_ack_t` | `rbox_pkg::rc_t` / `rc_ack_t` | Self-contained in rbox_pkg |
| ESR | `esr_rbox` + `esr_rbox_auto.v` with macros | `gen_esr.py` → clean `rbox_esr.sv` | CSV-driven generator, no macros |
| UltraSoC | Status monitor (`rbox_sm_*` ports) | Dropped entirely | Third-party IP; own debug later |
| FP shifter | Duplicated in both adder files | Extracted to `rbox_fp_shifter` | One module per file |

## Design constraints

- All floating-point operations use Round-To-Zero (no IEEE754 round-to-nearest)
- Subnormal FP inputs are flushed to zero
- MSAA supports 2x, 4x, 8x, 16x modes
- Depth buffer is 64 entries x 512 bits with LRU replacement
- Maximum render dimension: 32768 pixels per axis
