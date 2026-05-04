# RBOX (Render Box) — Programming Manual

## Overview

RBOX is a fixed-function rasterization and depth-testing unit. It receives triangles defined by edge equations from software, determines per-pixel coverage via 2×2 quad scanning, performs depth/stencil testing against a cached depth buffer, and outputs barycentric coordinates and coverage masks for fragment shading by software.

```
vertex shader   →  triangle setup  →  RBOX  →  fragment shader  →  blending
  (software)        (software)      (hardware)    (software)       (software)
```

Software is responsible for: vertex transformation, computing edge equations from screen-space vertices, tile binning, fragment shading, texture sampling, and framebuffer blending. RBOX handles the fixed-function middle stage: rasterization and depth/stencil testing.

## System integration

```
                         Shire Tile
  ┌──────────────────────────────────────────────────────┐
  │                                                      │
  │  ┌────────────────────────────────┐                  │
  │  │      Minion Cores (×64)        │                  │
  │  │  vertex shader, triangle setup,│                  │
  │  │  fragment shader, blending     │                  │
  │  └──────────┬─────────────────────┘                  │
  │             │ (memory-mapped)                         │
  │             ▼                                         │
  │  ┌──────────────────────┐    ET-Link req/rsp         │
  │  │                      │◄──────────────────────┐    │
  │  │    Shire Cache (L2)  │───────────────────────┤    │
  │  │    4 banks, 64B line │    ET-Link req/rsp    │    │
  │  │                      │◄──────────────┐       │    │
  │  └──────────────────────┘               │       │    │
  │             ▲                            │       │    │
  │             │ APB                  ┌─────┴───────┴─┐ │
  │             │                      │    rbox_top    │ │
  │  ┌──────────┴──────────┐          │               │ │
  │  │    BPAM / Debug     │──APB────▶│  apb_req/rsp  │ │
  │  │   (run control)     │──RC─────▶│  rc / rc_ack  │ │
  │  └─────────────────────┘          └───────────────┘ │
  │                                                      │
  └──────────────────────────────────────────────────────┘
```

### Port interface

| Port | Dir | Type | Width | Description |
|------|-----|------|-------|-------------|
| `clk_i` | in | `logic` | 1 | System clock |
| `rst_d_ni` | in | `logic` | 1 | Debug reset (active-low async) |
| `rst_w_ni` | in | `logic` | 1 | Warm reset (active-low async) |
| `shire_id_i` | in | `logic` | 8 | Shire tile ID |
| `rbox_sc_req_o` | out | `req_t` | 584 | ET-Link request to Shire Cache |
| `rbox_sc_req_valid_o` | out | `logic` | 5 | Per-bank + shared request valid |
| `rbox_sc_req_ready_i` | in | `logic` | 5 | Per-bank + shared request ready |
| `sc_rbox_rsp_i` | in | `rsp_t` | 534 | ET-Link response from Shire Cache |
| `sc_rbox_rsp_valid_i` | in | `logic` | 1 | Response valid |
| `sc_rbox_rsp_ready_o` | out | `logic` | 1 | Response ready (always accepts) |
| `rc_i` | in | `rc_t` | 6 | Debug run control |
| `rc_ack_o` | out | `rc_ack_t` | 3 | Run control acknowledge |
| `apb_req_i` | in | `apb_req_t` | 83 | APB register access request |
| `apb_rsp_o` | out | `apb_rsp_t` | 66 | APB register access response |
| `ram_cfg_i` | in | `ram_cfg_t` | 15 | SRAM timing configuration |
| `dft_i` | in | `dft_t` | 5 | DFT/scan control |

## Programming sequence

### Step 1: Prepare the input buffer in memory

Software writes a sequence of packets to a contiguous memory region. Each packet occupies either 256 or 512 bits. Packets are packed into a stream of 256-bit words stored in 64-byte cache lines.

```
Packet sequence:
  RBOX_STATE          — render configuration (required, once)
  FSH_STATE           — fragment shader addresses (required, starts a drawcall)
  TRI_WITH_TILE       — triangle + tile (one per triangle-tile pair)
  TRI_WITH_TILE       — ...more triangles...
  FSH_STATE           — optional: start a new drawcall
  TRI_WITH_TILE       — ...more triangles for the new drawcall...
  END_OF_BUFFER       — required, terminates processing
```

### Step 2: Prepare the depth buffer in memory

If depth testing is enabled, the depth buffer memory region must be initialized. For `D32_SFLOAT` format: fill with `0x3F800000` (1.0) at every 32-bit position for "clear to far plane". The depth buffer address is set in RBOX_STATE.

### Step 3: Configure ESR registers via APB

Write registers in this order:

1. **CONFIG** = `{enable: 1}` — enables the RBOX clock gate. Must be first.
2. **IN_BUF_PG** — input buffer physical page addresses.
3. **IN_BUF_CFG** — start offset and line count.
4. **OUT_BUF_PG** — output buffer physical page address.
5. **OUT_BUF_CFG** — output buffer parameters.

### Step 4: Start

Write **START** = `{start: 1}`. Optionally set `flush: 1` to flush the depth buffer when done. STATUS transitions to WORKING.

### Step 5: Process output

RBOX fetches input packets from cache, rasterizes triangles, tests depth, and writes output packets to per-hart buffers in cache. It sends MsgSendData messages to notify harts.

Each hart runs a fragment shader loop:
1. Wait for MsgSendData
2. Read output packets from its buffer
3. Shade pixels using barycentric coordinates
4. Write CONSUME register to release credits
5. Repeat

### Step 6: Completion

Poll STATUS register. When it reads FINISHED (0x02), all processing is complete. If `flush=1` was set, the depth buffer has been written back to cache.

## ESR register map

APB address bit 15 selects ESR (0) or debug RAM (1).

### 0x0 — CONFIG (RW, 64-bit)

| Bits | Field | Description |
|------|-------|-------------|
| [0] | enable | Gates the RBOX processing clock. Must be set before any other register. |
| [63:1] | reserved | |

### 0x1 — IN_BUF_PG (RW, 64-bit)

| Bits | Field | Description |
|------|-------|-------------|
| [18:0] | page0 | Physical address bits [39:21] of input buffer page 0 (2MB-aligned) |
| [30:19] | reserved0 | |
| [31] | page_active0 | Page 0 is valid |
| [50:32] | page1 | Physical address bits [39:21] of input buffer page 1 |
| [62:51] | reserved1 | |
| [63] | page_active1 | Page 1 is valid |

The input buffer can span two 2MB pages. `start_offset` (in IN_BUF_CFG) selects which page the first line is on. If the buffer crosses the page boundary, the upper bit of the line offset selects page1.

### 0x2 — IN_BUF_CFG (RW, 64-bit)

| Bits | Field | Width | Description |
|------|-------|-------|-------------|
| [15:0] | start_offset | 16 | First cache line offset within the page(s) |
| [31:16] | reserved0 | 16 | |
| [47:32] | line_count | 16 | Number of 64-byte cache lines to fetch |
| [63:48] | reserved1 | 16 | |

### 0x3 — OUT_BUF_PG (RW, 32-bit)

| Bits | Field | Description |
|------|-------|-------------|
| [18:0] | page | Physical address bits [39:21] of output buffer page |
| [30:19] | reserved | |
| [31] | page_active | Page is valid |

### 0x4 — OUT_BUF_CFG (RW, 64-bit)

| Bits | Field | Width | Description |
|------|-------|-------|-------------|
| [15:0] | start_offset | 16 | First entry offset in the output buffer |
| [34:32] | hart_buf_lines | 3 | Per-hart buffer size = `2^hart_buf_lines` entries. Initial pckt credits = `2^hart_buf_lines`. |
| [36:35] | hart_port_id | 2 | Message port ID for MsgSendData |
| [38:37] | hart_max_msg | 2 | Initial message credits = `hart_max_msg + 1` |
| [41:39] | max_pckts_msg | 3 | Message sent every `2^max_pckts_msg` packets |
| [63:42] | reserved | | |

### 0x5 — STATUS (RO, 32-bit)

| Bits | Field | Values |
|------|-------|--------|
| [7:0] | status | 0x00=READY, 0x01=WORKING, 0x02=FINISHED, 0x80=ERROR |
| [31:8] | reserved | |

### 0x6 — START (RW, 32-bit)

| Bits | Field | Description |
|------|-------|-------------|
| [0] | start | Write 1 to begin processing |
| [15:1] | reserved | |
| [16] | flush | Write 1 to flush depth buffer on completion |
| [31:17] | reserved | |

### 0x7 — CONSUME (RW, 32-bit)

| Bits | Field | Width | Description |
|------|-------|-------|-------------|
| [7:0] | pckt_credits | 8 | Packet credits to ADD to the hart's counter |
| [9:8] | msg_credits | 2 | Message credits to ADD to the hart's counter |
| [15:10] | reserved | | |
| [21:16] | hart_id | 6 | Target hart |
| [31:22] | reserved | | |

Credits are unsigned 8-bit (pckt) and 3-bit (msg) counters. Software must not overflow them. Each CONSUME write adds the specified credits to the target hart's counters.

Note: register write enables are gated by `|wdata` — writing all-zeros has no effect.

## Input packet formats

### Packet types

| Type | `inpck_type` | Size | Words | Description |
|------|-------------|------|-------|-------------|
| TRI_WITH_TILE_64x64 | 0 | 512b | 2 | Triangle + 64×64 tile |
| TRI_WITH_TILE_128x128 | 1 | 512b | 2 | Triangle + 128×128 tile |
| LARGE_TRIANGLE | 2 | 512b | 2 | Triangle only (tile follows separately) |
| FULLY_COVERED_TILE | 3 | 256b | 1 | All pixels inside triangle |
| LARGE_TRIANGLE_TILE | 4 | 256b | 1 | Tile for preceding large triangle |
| RBOX_STATE | 5 | 512b | 2 | Render state configuration |
| FSH_STATE | 6 | 256b | 1 | Fragment shader addresses (starts drawcall) |
| END_OF_BUFFER | 7 | 256b | 1 | End marker |

### Word stream packing

The input FIFO operates on 256-bit words. Each 64-byte cache line holds 2 words. A 2-word packet (like TRI_WITH_TILE) occupies a full cache line. A 1-word packet (like FSH_STATE) occupies the lower 256 bits; the upper 256 bits of that cache line must contain the start of the next packet.

Example: RBOX_STATE(2w) + FSH_STATE(1w) + TRI(2w) + END(1w) = 6 words = 3 cache lines:

```
Cache line 0:  [ RBOX_STATE[511:256]   | RBOX_STATE[255:0]    ]  words 1,0
Cache line 1:  [ TRI[255:0]           | FSH_STATE[255:0]      ]  words 3,2
Cache line 2:  [ END_OF_BUFFER[255:0] | TRI[511:256]          ]  words 5,4
```

Set `line_count = 3` in IN_BUF_CFG.

### TRI_WITH_TILE_64x64 (type 0, 512 bits)

```
Bit layout (MSB to LSB):
  [511:480]  depth_sample        32   FP32 depth at tile origin
  [479:448]  edge_sample[2]      32   {reserved[2:0], sample[28:0]} signed 15.14
  [447:416]  edge_sample[1]      32   same
  [415:384]  edge_sample[0]      32   same
  [383:320]  triangle_data_addr  64   opaque pointer, passed through to output
  [319:256]  depth_eq            64   {b_coef[31:0], a_coef[31:0]} FP32
  [255:192]  edge_eq[2]          64   {b_reserved[8:0], b_coef[22:0],
                                       a_reserved[8:0], a_coef[22:0]} signed 9.14
  [191:128]  edge_eq[1]          64   same
  [127:64]   edge_eq[0]          64   same
  [63:33]    reserved0           31
  [32:30]    tile_size            3   0=64×64, 1=64×32, 2=32×32, 3=16×16, 4=8×8, 5=4×4
  [29]       tri_facing           1   0=front, 1=back
  [28:16]    tile_top            13   tile Y position in 4-pixel units
  [15:3]     tile_left           13   tile X position in 4-pixel units
  [2:0]      inpck_type           3   0
```

#### Edge equation convention

Edge `i` is the edge **opposite** vertex `i`. For a triangle with vertices A, B, C:

- `edge[0]` goes from B to C (opposite A). `bary_i` output = edge[0] value = weight of vertex A.
- `edge[1]` goes from C to A (opposite B). `bary_j` output = edge[1] value = weight of vertex B.
- `edge[2]` goes from A to B (opposite C). Weight of vertex C = `1 - bary_i/area2 - bary_j/area2`.

#### Computing edge equations from vertices

Given screen-space triangle vertices `(ax,ay)`, `(bx,by)`, `(cx,cy)`:

```
2*area = (bx-ax)*(cy-ay) - (cx-ax)*(by-ay)

For edge[0] (B→C, opposite A):
  a = cy - by
  b = -(cx - bx)
  c = bx*cy - cx*by    (= sample at origin (0,0))

For edge[1] (C→A, opposite B):
  a = ay - cy
  b = -(ax - cx)
  c = cx*ay - ax*cy

For edge[2] (A→B, opposite C):
  a = by - ay
  b = -(bx - ax)
  c = ax*by - bx*ay
```

If `2*area > 0` (CCW winding), negate all `a`, `b`, `c` so that interior pixels produce positive edge values.

If `2*area < 0` (CW winding), the values are already in the correct sign convention.

The coefficients `a` and `b` are stored in 9.14 signed fixed-point (23 bits). The sample `c` is stored in 15.14 signed fixed-point (29 bits). The sample represents the edge function value at the tile origin `(tile_left * 4, tile_top * 4)`.

#### Coverage rule

A pixel at `(x,y)` is inside the triangle when all three edge values are strictly positive, OR exactly zero on a "top or left" edge. A top-or-left edge is one where `a > 0`, or `a == 0 && b < 0`.

#### Depth equation

For flat-shaded depth (constant across the triangle): `a_coef = 0`, `b_coef = 0`, `depth_sample = FP32(depth_value)`.

For interpolated depth: `a_coef` and `b_coef` are FP32 depth gradients per pixel in X and Y.

#### triangle_data_addr

An opaque 64-bit value passed through unchanged to the output QUAD_INFO packet. Software uses it to identify which triangle produced each fragment (e.g., to look up vertex attributes for shading).

### RBOX_STATE (type 5, 512 bits)

Configures the render pipeline. Must precede any triangle packets. Fields packed MSB to LSB in QW (64-bit) groups:

```
QW0 [63:8] (56 bits):
  msaa_enable                 1    Enable MSAA
  msaa_samples               4    MSAA mode (0=2x, 1=4x, 2=8x, 3=16x)
  msaa_shading_enable        1    Per-sample shading
  msaa_alpha_to_coverage     1
  msaa_alpha_to_one          1
  msaa_sample_mask          16    Per-sample enable mask
  msaa_min_sample_shading   32    FP32

QW1 [127:64] (64 bits):
  ds_buffer_addr             64   Physical address of depth/stencil buffer

QW2 [191:128] (64 bits):
  ds_buffer_format            9   Depth format (126 = D32_SFLOAT)
  ds_buffer_tiled             1   Tiled depth buffer layout
  ds_buffer_row_pitch        13   Row pitch
  depth_clamp_enable          1
  depth_bound_enable          1
  depth_test_enable           1   Enable depth testing
  depth_test_write_enable     1   Write passing fragments to depth buffer
  depth_test_compare_op       3   0=Never 1=Less 2=Equal 3=LessOrEqual
                                  4=Greater 5=NotEqual 6=GreaterOrEqual 7=Always
  early_frag_tests_enable     1
  stencil_test_enable         1
  fragment_shader_disabled    1   Skip fragment shading output
  fragment_shader_reads_bary  1   Include bary i/j in output
  fragment_shader_reads_depth 1   Include depth in output
  fragment_shader_reads_cov   1   Include MSAA coverage in output
  fragment_shader_per_sample  1
  hart_tile_width             2   Per-hart tile = 2^(val+1) pixels wide
  hart_tile_height            2   Per-hart tile = 2^(val+1) pixels tall
  shire_layout_width          2   Hart grid = 2^val wide
  shire_layout_height         2   Hart grid = 2^val tall
  reserved                   19

QW3 [255:192] (64 bits):
  depth_bound_min            32   FP32
  depth_bound_max            32   FP32

QW4 [319:256] (64 bits):
  depth_min                  32   FP32 (for depth clamping)
  depth_max                  32   FP32

QW5 [383:320] (64 bits):
  scissor_start_x            14   Scissor rectangle left
  scissor_start_y            14   Scissor rectangle top
  scissor_height             14   Scissor rectangle height
  scissor_width              14   Scissor rectangle width
  reserved                    8

QW6 [447:384] (64 bits):
  stencil_front_state        36   Front-face stencil ops
  reserved                   28

QW7 [511:448] (64 bits):
  stencil_back_state         36   Back-face stencil ops
  reserved                   28

[7:0] Header:
  [2:0] inpck_type = 5
  [7:3] reserved
```

### FSH_STATE (type 6, 256 bits)

Starts a new drawcall. All subsequent triangles until the next FSH_STATE or END_OF_BUFFER belong to this drawcall.

```
  [2:0]      inpck_type           3   6
  [63:3]     reserved0           61
  [127:64]   fshader_function_addr 64  Physical address of fragment shader code
  [191:128]  fshader_data_addr   64   Physical address of shader data
  [255:192]  reserved1           64
```

### END_OF_BUFFER (type 7, 256 bits)

```
  [2:0]      inpck_type           3   7
  [255:3]    reserved           253
```

## Output packet formats

Output packets are 256-bit HLINE writes to per-hart circular buffers in cache. Each hart's buffer base address is `OUT_BUF_PG.page << 21 + hart_id * (2^hart_buf_lines * 32)`.

### Packet types (bits [1:0] of word 0)

| Type | Value | Description |
|------|-------|-------------|
| STATE_INFO | 0 | Fragment shader state OR barycentric/depth data |
| QUAD_INFO | 1 | Quad pair positions and coverage |
| END_PHASE | 2 | End of drawcall for this hart |

### Output sequence per quad pair

```
[Once per hart per drawcall]  STATE_INFO: fragment shader addresses
[Per quad pair]               QUAD_INFO:  positions, mask, triangle pointer
                              STATE_INFO: bary_i (4×FP32 per quad, 8 total)
                              STATE_INFO: bary_j (4×FP32 per quad, 8 total)
                              STATE_INFO: depth (optional, if reads_depth)
                              STATE_INFO: coverage (optional, if reads_coverage)
[End of drawcall]             END_PHASE
```

### QUAD_INFO layout (256 bits)

```
  [1:0]      outpck_type          2   1
  [15:2]     reserved0           14
  [30:16]    quad0_x             15   Quad 0 pixel X position
  [31]       reserved1            1
  [46:32]    quad1_x             15   Quad 1 pixel X position
  [47]       reserved2            1
  [62:48]    quad0_y             15   Quad 0 pixel Y position
  [63]       reserved3            1
  [78:64]    quad1_y             15   Quad 1 pixel Y position
  [79]       reserved4            1
  [83:80]    sample_idx           4   MSAA sample index
  [87:84]    reserved5            4
  [95:88]    mask                 8   Coverage: [3:0]=quad0, [7:4]=quad1
  [127:96]   reserved6           32
  [191:128]  quad0_triangle_ptr  64   triangle_data_addr for quad 0
  [255:192]  quad1_triangle_ptr  64   triangle_data_addr for quad 1
```

Each quad is a 2×2 pixel block. Pixel mapping within a quad:
- Pixel 0: `(quad_x, quad_y)`
- Pixel 1: `(quad_x+1, quad_y)`
- Pixel 2: `(quad_x, quad_y+1)`
- Pixel 3: `(quad_x+1, quad_y+1)`

Mask bit `i` = 1 means pixel `i` is covered. Bits [3:0] for quad 0, bits [7:4] for quad 1.

### Barycentric data layout (256 bits)

```
  [31:0]     quad0_pixel0        32   FP32 barycentric value
  [63:32]    quad0_pixel1        32   FP32
  [95:64]    quad0_pixel2        32   FP32
  [127:96]   quad0_pixel3        32   FP32
  [159:128]  quad1_pixel0        32   FP32
  [191:160]  quad1_pixel1        32   FP32
  [223:192]  quad1_pixel2        32   FP32
  [255:224]  quad1_pixel3        32   FP32
```

The first STATE_INFO after QUAD_INFO carries `bary_i` (edge[0] values = vertex A weight). The second carries `bary_j` (edge[1] values = vertex B weight). Values are raw edge function evaluations in FP32; divide by `2*area` to normalize to [0,1].

## Credit management

The output path is flow-controlled by per-hart credit counters.

### Initial credits (set at START)

- Packet credits: `2^hart_buf_lines` (8-bit unsigned counter, max 128 with hart_buf_lines=7)
- Message credits: `hart_max_msg + 1` (3-bit unsigned counter, max 4 with hart_max_msg=3)

### Credit consumption

- Each HLINE write to a hart's buffer decrements that hart's packet credits by 1
- Each MsgSendData to a hart decrements that hart's message credits by 1
- When either counter reaches 0, the output path **stalls** for that hart
- A message is triggered when `2^max_pckts_msg` packets accumulate for a hart

### Credit release

Software writes CONSUME with `{hart_id, pckt_credits, msg_credits}`. The credits are **added** to the target hart's counters. Software must ensure counters don't overflow (8-bit unsigned for pckt, 3-bit for msg).

### Stall behavior

When the output path stalls on credits, the traverse pipeline also stalls (backpressure through `ct_quad_accept`). The traverse interprets sustained stalls as "no coverage" and may trigger early row termination, skipping subsequent rows in the tile. To avoid this, software must release credits promptly.

With multiple harts, the work is distributed: each hart receives only `1/(layout_w × layout_h)` of the quads. This reduces credit pressure per hart proportionally.

## Quad-to-hart mapping

```
hart_id = (quad_y >> (hart_tile_height + 1)) * (1 << shire_layout_width)
        + (quad_x >> (hart_tile_width  + 1))
```

Active harts: `2^shire_layout_width × 2^shire_layout_height` (1 to 64).

Each hart owns a rectangular tile of the screen. With `hart_tile_width=2, hart_tile_height=2`: each hart tile is 8×8 pixels. With `shire_layout_width=2, shire_layout_height=2`: 16 harts in a 4×4 grid.

## Cache interface

RBOX is an ET-Link bus master. All memory access goes through the Shire Cache.

| Source | Opcode | Size | Purpose |
|--------|--------|------|---------|
| Input buffer | Read | Line (64B) | Fetch input packets |
| Depth buffer | Read | Line (64B) | Fill depth cache line |
| Depth buffer | Write | Line (64B) | Spill dirty depth cache line |
| Output buffer | Write | HLine (32B) | Write output packets |
| Output buffer | MsgSendData | QWord (8B) | Notify hart |

Request arbitration priority: Depth buffer > Input buffer > Output message > Output packet.

Response routing: ID bit 7 = 0 routes to input buffer / output ack path; ID bit 7 = 1 routes to depth buffer.

## Clock domains

| Clock | Source | Used by |
|-------|--------|---------|
| `clk_i` | Always-on | CONFIG register, APB, halt/resume FSM |
| `clk_rbox_i` | Gated (`clk_i & enable`) | All processing pipelines |

The clock gate enable = `CONFIG.enable OR !rst_w_ni`. The processing clock runs during reset (for initialization) and after CONFIG.enable is set.

Other ESR registers are latched on `clk_rbox_i`. CONFIG must be written **first** to enable the gated clock before other registers take effect.

## Reset domains

| Reset | Controls |
|-------|----------|
| `rst_d_ni` (debug) | Halt/resume FSM only |
| `rst_w_ni` (warm) | All functional state, ESR registers, clock gate |

## Known limitations

- The traverse pipeline has a 2-quad-row (4-pixel) latency between the quad position counter and the edge sample accumulator. At triangle edges near the bottom of a tile, this can produce 1-2 extra rows of phantom coverage. Both pixels at the boundary and 2 rows beyond may be reported as covered. This is a characteristic of the pipelined rasterizer, not a bug.

- The `rbox_sample_to_fp32` converter produces an undefined (non-zero) FP32 value when the input fixed-point sample is exactly zero. The edge coverage test operates on the raw fixed-point samples (not FP32) and is correct; the FP32 output is only used for barycentric shading and does not affect coverage.

- The input FIFO uses single-ported SRAMs. A cache response push and a FIFO SRAM read cannot occur simultaneously; the push is deferred. With real cache latency this is not an issue. In zero-latency simulation, responses must be staggered.

## Key parameters

| Parameter | Value |
|-----------|-------|
| Max harts per shire | 64 |
| Depth cache | 64 entries × 512 bits, LRU |
| Input FIFO | 32 entries × 512 bits (64 × 256-bit words) |
| Output buffer | 128 entries per hart |
| Max render dimension | 32768 pixels per axis |
| Tile sizes | 64×64, 64×32, 32×32, 16×16, 8×8, 4×4 |
| Traverse pipeline | 9 stages |
| Depth test pipeline | 4 stages |
| Edge coef (64×64 tile) | Signed 9.14 fixed-point (23 bits) |
| Edge sample (64×64 tile) | Signed 15.14 fixed-point (29 bits) |
| Internal edge sample | Signed 18.25 (43 bits) |
| Internal edge coef | Signed 9.25 (34 bits) |
| FP arithmetic | Round-to-zero, subnormals flushed |
| Depth compare ops | Never, Less, Equal, LessOrEqual, Greater, NotEqual, GreaterOrEqual, Always |
| Stencil ops | Keep, Zero, Replace, IncrementClamp, DecrementClamp, Invert, IncrementWrap, DecrementWrap |
| MSAA modes | 2×, 4×, 8×, 16× (Vulkan standard positions) |
| Depth formats | D16_UNORM, D32_SFLOAT, X8_D24_UNORM_PACK32, D24_UNORM_S8_UINT |
