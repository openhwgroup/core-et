# prim_fifo_sync

Synchronous FIFO with ready/valid handshake on both ports.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 8 | Data width in bits |
| `Depth` | `int unsigned` | 4 | Number of entries |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `clr_i` | input | 1 | Synchronous clear (flushes FIFO) |
| `wvalid_i` | input | 1 | Write data valid |
| `wready_o` | output | 1 | FIFO accepts write (not full) |
| `wdata_i` | input | `Width` | Write data |
| `rvalid_o` | output | 1 | Read data valid (not empty) |
| `rready_i` | input | 1 | Consumer accepts read data |
| `rdata_o` | output | `Width` | Read data (head of FIFO) |
| `depth_o` | output | `$clog2(Depth+1)` | Current fill level |
| `full_o` | output | 1 | FIFO is full |
| `empty_o` | output | 1 | FIFO is empty |

## Protocol

Standard ready/valid handshake:
- A write occurs when `wvalid_i && wready_o` on the rising edge of `clk_i`.
- A read occurs when `rvalid_o && rready_i` on the rising edge of `clk_i`.
- Simultaneous read and write is supported at any fill level.
- `rdata_o` reflects the head of the FIFO combinationally (no output register).

## Usage

```systemverilog
prim_fifo_sync #(
  .Width (32),
  .Depth (8)
) u_fifo (
  .clk_i,
  .rst_ni,
  .clr_i    (1'b0),
  .wvalid_i (producer_valid),
  .wready_o (producer_ready),
  .wdata_i  (producer_data),
  .rvalid_o (consumer_valid),
  .rready_i (consumer_ready),
  .rdata_o  (consumer_data),
  .depth_o  (),
  .full_o   (),
  .empty_o  ()
);
```

## Design notes

- FPGA-friendly: infers distributed RAM (LUT) for small depths, BRAM for large depths.
- Async-assert / sync-deassert reset for FPGA + ASIC compatibility.
- `clr_i` resets pointers and depth synchronously without affecting the memory contents.

## Tests

```bash
make -C dv test          # run tests
make -C dv test-trace    # run with VCD waveform
make -C dv lint          # lint RTL only
```
