# prim_rf_single_1r1w_par_preview

Single-entry write-capture register with direct readback and a low-phase
write-preview contract.

This primitive is the single-entry form of the latch-timed RF preview seam. It
has no read address and no write address. The single stored entry is
continuously visible on `rd_data_o`.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 32 | Stored data width |
| `PreviewClkSameAsRf` | `bit` | 1 | Set to 1 only when `preview_clk_i` and `rf_clk_i` are the same physical clock; set to 0 for gated/split preview clocks |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `preview_clk_i` | input | 1 | Low-phase preview clock |
| `rf_clk_i` | input | 1 | Commit clock |
| `wr_data_en_1p_next_i` | input | 1 | Preview qualifier sampled while `preview_clk_i` is low |
| `rd_data_o` | output | `Width` | Direct readback of the stored entry |
| `wr_data_i` | input | `Width` | Write payload to preview and later commit |
| `wr_en_i` | input | 1 | Commit enable sampled on `posedge rf_clk_i` |

## Timing Contract

`wr_data_en_1p_next_i` is a low-phase preview qualifier. While
`preview_clk_i` is low, that qualifier is transparent to the payload staging
decision. On the following low phase of `rf_clk_i`, the primitive captures
`wr_data_i` when the preview qualifier is active. On `posedge rf_clk_i`, the
previously staged payload commits to the single stored entry when `wr_en_i` is
asserted.

`rd_data_o` is combinational direct readback. It has zero architectural read
latency and continuously reflects the currently visible stored entry.

```
preview_clk_i      ___-----____-----___
wr_data_en_next       =======           preview qualifier transparent while low
rf_clk_i           ___-----____-----___
wr_data_i             D0      D1
wr_data_del_q         -> D0   -> D1     captured on rf low phase when previewed
wr_en_i                    ===
rd_data_o                  -> D0        committed on rf rising edge
```

## Integration Rules

- `wr_data_en_1p_next_i` controls payload preview only; it must not be gated by
  `wr_en_i` inside this primitive.
- `wr_en_i` controls only the single-entry commit on `posedge rf_clk_i`.
- The caller must keep the preview and commit phases aligned so the staged
  payload corresponds to the later commit.
- Set `PreviewClkSameAsRf=0` when `preview_clk_i` can be gated, held, or
  otherwise separated while `rf_clk_i` continues to toggle.
- Do not compose this primitive from other public primitives in a technology
  implementation. It is the implementation seam.

## Differences From The Original

The interface uses Ainekko naming and explicit `preview_clk_i` / `rf_clk_i`
ports. The externally visible phase ordering, zero-latency direct readback, and
write-preview-before-commit contract are preserved.

## Tests

```bash
make -C hw/ip/prim_rf_single_1r1w_par_preview/dv lint
make -C hw/ip/prim_rf_single_1r1w_par_preview/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_rf_single_1r1w_par_preview test
```
