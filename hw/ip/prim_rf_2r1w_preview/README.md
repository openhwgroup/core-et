# prim_rf_2r1w_preview

Two-read, one-write register file with a low-phase write-preview contract,
optional hard-zero entries, and optional parallel readback taps.

This primitive is a technology seam for latch-timed 2R1W RFs whose write
payload is previewed one phase before the RF commit. Both read ports are
combinational. Entries selected by `Zero` are hardwired to zero and ignore
writes. Entries selected by `Parallel` are packed into `rd_par_o`.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 32 | Data width of each RF entry |
| `Entries` | `int unsigned` | 32 | Number of RF entries |
| `Zero` | `logic [Entries-1:0]` | `'0` | Hard-zero entry mask |
| `Parallel` | `logic [Entries-1:0]` | `'0` | Parallel-read tap mask |
| `ParallelW` | `int unsigned` | 32 | Packed parallel output width |
| `Level2CkGate` | `int unsigned` | 0 | Preserved technology parameter from the original seam |
| `PreviewClkSameAsRf` | `bit` | 1 | Set to 1 only when `preview_clk_i` and `rf_clk_i` are the same physical clock; set to 0 for gated/split preview clocks |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `preview_clk_i` | input | 1 | Low-phase preview clock |
| `rf_clk_i` | input | 1 | RF commit clock |
| `rd_addr_a_i` | input | `$clog2(Entries)` | Read port A address |
| `rd_data_a_o` | output | `Width` | Read port A data |
| `rd_addr_b_i` | input | `$clog2(Entries)` | Read port B address |
| `rd_data_b_o` | output | `Width` | Read port B data |
| `rd_par_o` | output | `ParallelW` | Packed readback of entries selected by `Parallel` |
| `wr_en_i` | input | 1 | Commit enable sampled on `posedge rf_clk_i` |
| `wr_data_en_1p_next_i` | input | 1 | Preview qualifier sampled while `preview_clk_i` is low |
| `wr_addr_i` | input | `$clog2(Entries)` | Encoded write address |
| `wr_data_i` | input | `Width` | Write payload to preview and later commit |

## Timing Contract

`wr_data_en_1p_next_i` is a low-phase preview qualifier. While
`preview_clk_i` is low, that qualifier is transparent to the payload staging
decision. On the following low phase of `rf_clk_i`, the primitive captures
`wr_data_i` when the preview qualifier is active. On `posedge rf_clk_i`, the
previously staged payload commits to `rf_q[wr_addr_i]` when `wr_en_i` is
asserted and `Zero[wr_addr_i]` is clear.

`rd_addr_a_i` and `rd_addr_b_i` are combinational read addresses.
`rd_data_a_o`, `rd_data_b_o`, and `rd_par_o` have zero architectural read
latency and reflect the currently visible RF contents.

```
preview_clk_i      ___-----____-----___
wr_data_en_next       =======           preview qualifier transparent while low
rf_clk_i           ___-----____-----___
wr_data_i             D0      D1
wr_data_del_q         -> D0   -> D1     captured on rf low phase when previewed
wr_en_i                    ===
wr_addr_i                  A
rd_data_*                  -> D0        committed on rf rising edge unless Zero[A]
```

## Parallel Output Packing

For each entry `i` with `Parallel[i] == 1`, the entry is packed into
`rd_par_o[Width*par_position(i) +: Width]`, where `par_position(i)` is the
number of set `Parallel` bits below `i`. Entries not selected by `Parallel` are
not present in `rd_par_o`. If no entries are selected, `rd_par_o` is zero.

## Integration Rules

- `wr_data_en_1p_next_i` controls payload preview only; it must not be gated by
  `wr_en_i` inside this primitive.
- `wr_en_i` controls only the RF commit on `posedge rf_clk_i`.
- Reads are asynchronous/combinational from the currently visible RF state.
- Hard-zero entries must read zero on both read ports and on the parallel
  output, and must ignore writes.
- The caller must keep the preview and commit phases aligned so the staged
  payload corresponds to the later commit.
- Set `PreviewClkSameAsRf=0` when `preview_clk_i` can be gated, held, or
  otherwise separated while `rf_clk_i` continues to toggle.
- Do not compose this primitive from other public primitives in a technology
  implementation. It is the implementation seam.

## Differences From The Original

The interface uses Ainekko naming and explicit `preview_clk_i` / `rf_clk_i`
ports. The externally visible phase ordering, hard-zero behavior, packed
parallel readback, zero-latency read behavior, and write-preview-before-commit
contract are preserved.

## Tests

```bash
make -C hw/ip/prim_rf_2r1w_preview/dv lint
make -C hw/ip/prim_rf_2r1w_preview/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_rf_2r1w_preview test
```
