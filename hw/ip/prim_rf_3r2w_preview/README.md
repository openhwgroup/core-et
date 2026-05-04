# prim_rf_3r2w_preview

Three-read, two-write register file with independent low-phase write-preview
contracts for each write port.

This primitive is a technology seam for latch-timed 3R2W RFs whose write
payloads are previewed one phase before the RF commit. All three read ports are
combinational. When both write ports commit to the same entry in the same RF
rising edge, write port A has priority over write port B.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 32 | Data width of each RF entry |
| `Entries` | `int unsigned` | 32 | Number of RF entries |
| `Level2CkGate` | `int unsigned` | 0 | Preserved technology parameter from the original seam |
| `PreviewClkSameAsRf` | `bit` | 1 | Set to 1 only when `preview_clk_i` and `rf_clk_i` are the same physical clock; set to 0 for gated/split preview clocks |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `preview_clk_i` | input | 1 | Low-phase preview clock for both write ports |
| `rf_clk_i` | input | 1 | RF commit clock |
| `rd_addr_a_i` | input | `$clog2(Entries)` | Read port A address |
| `rd_data_a_o` | output | `Width` | Read port A data |
| `rd_addr_b_i` | input | `$clog2(Entries)` | Read port B address |
| `rd_data_b_o` | output | `Width` | Read port B data |
| `rd_addr_c_i` | input | `$clog2(Entries)` | Read port C address |
| `rd_data_c_o` | output | `Width` | Read port C data |
| `wr_en_a_i` | input | 1 | Write port A commit enable |
| `wr_addr_a_i` | input | `$clog2(Entries)` | Write port A address |
| `wr_data_a_en_1p_next_i` | input | 1 | Write port A preview qualifier |
| `wr_data_a_i` | input | `Width` | Write port A payload |
| `wr_en_b_i` | input | 1 | Write port B commit enable |
| `wr_addr_b_i` | input | `$clog2(Entries)` | Write port B address |
| `wr_data_b_en_1p_next_i` | input | 1 | Write port B preview qualifier |
| `wr_data_b_i` | input | `Width` | Write port B payload |

## Timing Contract

`wr_data_a_en_1p_next_i` and `wr_data_b_en_1p_next_i` are independent
low-phase preview qualifiers. While `preview_clk_i` is low, each qualifier is
transparent to its payload staging decision. On the following low phase of
`rf_clk_i`, the primitive captures the corresponding write payload when that
port's preview qualifier is active.

On `posedge rf_clk_i`, write port B commits first when `wr_en_b_i` is
asserted, then write port A commits when `wr_en_a_i` is asserted. This gives
port A priority for same-address conflicts.

`rd_addr_a_i`, `rd_addr_b_i`, and `rd_addr_c_i` are combinational read
addresses. The read data outputs have zero architectural read latency and
reflect the currently visible RF contents.

```
preview_clk_i       ___-----____-----___
wr_data_*_en_next      =======           preview qualifiers transparent while low
rf_clk_i            ___-----____-----___
wr_data_a_i            A0      A1
wr_data_b_i            B0      B1
wr_data_*_del_q        ->      ->        captured on rf low phase when previewed
wr_en_b_i                   ===          B commits first
wr_en_a_i                   ===          A commits second and wins conflicts
rd_data_*                   -> A/B       visible after commit
```

## Integration Rules

- Preview qualifiers control payload preview only; they must not be gated by
  their corresponding write enables inside this primitive.
- `wr_en_a_i` and `wr_en_b_i` control only RF commit on `posedge rf_clk_i`.
- Reads are asynchronous/combinational from the currently visible RF state.
- Same-address write conflicts are legal; port A wins over port B.
- The caller must keep each preview and commit phase aligned so the staged
  payload corresponds to the later commit.
- Set `PreviewClkSameAsRf=0` when `preview_clk_i` can be gated, held, or
  otherwise separated while `rf_clk_i` continues to toggle.
- Do not compose this primitive from other public primitives in a technology
  implementation. It is the implementation seam.

## Differences From The Original

The interface uses Ainekko naming and explicit `preview_clk_i` / `rf_clk_i`
ports. The externally visible phase ordering, three combinational read ports,
two write ports, port-A conflict priority, and write-preview-before-commit
contract are preserved.

## Tests

```bash
make -C hw/ip/prim_rf_3r2w_preview/dv lint
make -C hw/ip/prim_rf_3r2w_preview/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_rf_3r2w_preview test
```
