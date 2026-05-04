# RBOX Depth-Test Demo

Verilator demo that drives the real `rbox_top` RTL to render two overlapping
triangles with depth testing.  Triangle A (white, depth=0.3) occludes
Triangle B (gray, depth=0.7) where they overlap.  Outputs a 64x64 PPM image.

## What it does

The demo acts as the full system in C++: CPU, Shire Cache, and framebuffer.
It programs the RBOX via APB, feeds triangle packets through a memory-backed
cache model, collects the rasterized output, performs fragment shading in
software, and writes the result as a PPM image.

### Pipeline exercised

1. APB ESR configuration (CONFIG, IN_BUF_PG/CFG, OUT_BUF_PG/CFG, START)
2. Input buffer fetch (ET-Link Read requests, cache line responses)
3. Packet decode (RBOX_STATE, FSH_STATE, TRI_WITH_TILE_64x64, END_OF_BUFFER)
4. Triangle rasterization (edge equation evaluation, scissor test)
5. Depth test (CompOpLess, D32_SFLOAT format, pre-filled depth buffer at 1.0)
6. Output buffer write (quad info + barycentric i/j packets)
7. Hart notification (MsgSendData messages)
8. Credit replenishment (CONSUME register, 16 harts)

### Triangles

```
Triangle A (white, depth=0.3): vertices (10,8), (50,8), (30,56) — points down
Triangle B (gray,  depth=0.7): vertices (14,56), (54,56), (34,8) — points up
```

Single 64x64 tile, no MSAA, D32_SFLOAT depth buffer initialized to 1.0.
16 harts (shire_layout 2x2, hart_tile 2x2), max_pckts_msg=7 (128 packets/msg).

## Build and run

```
make -C fpga/rbox_demo/verilator test
```

Output: `fpga/rbox_demo/verilator/depth_test.ppm`

## Known limitations

- Pipeline skew: the traverse has a 2-quad-row latency between the position
  counter and the edge sample accumulator.  This produces phantom coverage
  rows at triangle boundaries (mask=0 quads at y=62).  Both the original and
  reimplemented RTL exhibit this behavior.

- The `rbox_sample_to_fp32` converter produces undefined output for zero
  input (the leading-one detector wraps).  This matches the original.

- RTL assertion warnings about non-1-hot `rbox_sc_req_valid_o` appear when
  the RBOX drives multiple cache bank requests simultaneously.  The demo
  cache model accepts all banks, so this does not affect correctness.
