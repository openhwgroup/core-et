# prim_rf_1r1w_dec_preview

Decoded one-read, one-write register file with a low-phase write-preview
contract.

This primitive is a technology seam for latch-timed RFs whose write payload is
previewed one phase before the RF commit. The read and write addresses are
decoded one-hot vectors. Reads return the bitwise OR of every selected entry.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 32 | Data width of each RF entry |
| `Entries` | `int unsigned` | 8 | Number of decoded one-hot entries |
| `PreviewClkSameAsRf` | `bit` | 1 | Set to 1 only when `preview_clk_i` and `rf_clk_i` are the same physical clock; set to 0 for gated/split preview clocks |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `preview_clk_i` | input | 1 | Low-phase preview clock |
| `rf_clk_i` | input | 1 | RF commit clock |
| `wr_data_en_1p_next_i` | input | 1 | Preview qualifier sampled while `preview_clk_i` is low |
| `rd_addr_i` | input | `Entries` | Decoded one-hot read select |
| `rd_data_o` | output | `Width` | Bitwise OR of selected RF entries |
| `wr_data_i` | input | `Width` | Write payload to preview and later commit |
| `wr_addr_i` | input | `Entries` | Decoded one-hot write select |
| `wr_en_i` | input | 1 | Commit enable sampled on `posedge rf_clk_i` |

## Timing Contract

`wr_data_en_1p_next_i` is a low-phase preview qualifier. While
`preview_clk_i` is low, that qualifier is transparent to the payload staging
decision. On the following low phase of `rf_clk_i`, the primitive captures
`wr_data_i` when the preview qualifier is active. On `posedge rf_clk_i`, the
previously staged payload commits to every entry selected by `wr_addr_i` when
`wr_en_i` is asserted.

`rd_addr_i` is combinational. `rd_data_o` has zero architectural read latency:
it reflects the OR of the currently visible selected entries. If multiple
`rd_addr_i` bits are set, all selected entries are OR-reduced. If no bits are
set, `rd_data_o` is zero.

```
preview_clk_i      ___-----____-----___
wr_data_en_next       =======           preview qualifier transparent while low
rf_clk_i           ___-----____-----___
wr_data_i             D0      D1
wr_data_del_q         -> D0   -> D1     captured on rf low phase when previewed
wr_en_i                    ===
wr_addr_i                  A
rf_q[A]                    -> D0        committed on rf rising edge
```

## Integration Rules

- `wr_data_en_1p_next_i` controls payload preview only; it must not be gated by
  `wr_en_i` inside this primitive.
- `wr_en_i` controls only the RF commit on `posedge rf_clk_i`.
- `wr_addr_i` and `rd_addr_i` are decoded vectors. Multi-hot values are legal
  when the caller intentionally writes or reads multiple entries.
- The caller must keep the preview and commit phases aligned so the staged
  payload corresponds to the later commit.
- Set `PreviewClkSameAsRf=0` when `preview_clk_i` can be gated, held, or
  otherwise separated while `rf_clk_i` continues to toggle.
- Do not compose this primitive from other public primitives in a technology
  implementation. It is the implementation seam.

## Differences From The Original

The interface uses Ainekko naming and explicit `preview_clk_i` / `rf_clk_i`
ports. The externally visible phase ordering, decoded addressing, zero-latency
read behavior, and write-preview-before-commit contract are preserved.

## Tests

```bash
make -C hw/ip/prim_rf_1r1w_dec_preview/dv lint
make -C hw/ip/prim_rf_1r1w_dec_preview/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_rf_1r1w_dec_preview test
```
